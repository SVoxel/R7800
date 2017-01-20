/*
 * linux/fs/hfsplus_journal/commit.c
 *
 * Written by Stephen C. Tweedie <sct@redhat.com>, 1998
 *
 * Copyright 1998 Red Hat corp --- All Rights Reserved
 *
 * This file is part of the Linux kernel and is made available under
 * the terms of the GNU General Public License, version 2, or at your
 * option, any later version, incorporated herein by reference.
 *
 * Journal commit routines for the generic filesystem journaling code;
 * part of the ext2fs journaling system.
 */

#include <linux/time.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/pagemap.h>
#include <linux/bio.h>
#include <linux/blkdev.h>
#include <trace/events/jbd.h>
//#include <linux/smp_lock.h>
#include "hfsplus_jbd.h"
#include "hfsplus_fs.h"

/*
 * Default IO end handler for temporary HFSPLUS_BJ_IO buffer_heads.
 */
static void hfsplus_jbd_end_buffer_io_sync(struct buffer_head *bh, int uptodate)
{
	HFSPLUS_BUFFER_TRACE(bh, "");
	if (uptodate)
		set_buffer_uptodate(bh);
	else
		clear_buffer_uptodate(bh);
	unlock_buffer(bh);
}

/*
 * When an ext3-ordered file is truncated, it is possible that many pages are
 * not successfully freed, because they are attached to a committing transaction.
 * After the transaction commits, these pages are left on the LRU, with no
 * ->mapping, and with attached buffers.  These pages are trivially reclaimable
 * by the VM, but their apparent absence upsets the VM accounting, and it makes
 * the numbers in /proc/meminfo look odd.
 *
 * So here, we have a buffer which has just come off the forget list.  Look to
 * see if we can strip all buffers from the backing page.
 *
 * Called under journal->j_list_lock.  The caller provided us with a ref
 * against the buffer, and we drop that here.
 */
static void release_buffer_page(struct buffer_head *bh)
{
	struct page *page;

	if (buffer_dirty(bh))
		goto nope;
	if (atomic_read(&bh->b_count) != 1)
		goto nope;
	page = bh->b_page;
	if (!page)
		goto nope;
	if (page->mapping)
		goto nope;

	/* OK, it's a truncated page */
	if (!trylock_page(page))
		goto nope;

	page_cache_get(page);
	__brelse(bh);
	try_to_free_buffers(page);
	unlock_page(page);
	page_cache_release(page);
	return;

nope:
	__brelse(bh);
}

/*
 * Decrement reference counter for data buffer. If it has been marked
 * 'BH_Freed', release it and the page to which it belongs if possible.
 */
static void release_data_buffer(struct buffer_head *bh)
{
	if (buffer_hfsplus_jbd_freed(bh)) {
		clear_buffer_hfsplus_jbd_freed(bh);
		release_buffer_page(bh);
	} else
		put_bh(bh);
}

/*
 * Try to acquire hfsplus_jbd_lock_bh_state() against the buffer, when j_list_lock is
 * held.  For ranking reasons we must trylock.  If we lose, schedule away and
 * return 0.  j_list_lock is dropped in this case.
 */
static int inverted_lock(hfsplus_jbd_t *journal, struct buffer_head *bh)
{
	if (!hfsplus_jbd_trylock_bh_state(bh)) {
		spin_unlock(&journal->j_list_lock);
		schedule();
		return 0;
	}
	return 1;
}

/* Done it all: now write the commit record.  We should have
 * cleaned up our previous buffers by now, so if we are in abort
 * mode we can now just skip the rest of the journal write
 * entirely.
 *
 * Returns 1 if the journal needs to be aborted or 0 on success
 */
static int hfsplus_jbd_write_commit_record(hfsplus_jbd_t *journal,
					hfsplus_transaction_t *commit_transaction)
{
	struct hfsplus_jbd_head *descriptor;
	struct buffer_head *bh;
	hfsplus_jbd_header_t *header;
	int ret;

#ifdef HFSPLUS_JOURNAL_MAC_COMPATIBLE
	dprint(DBG_JCOMMIT, "Skip writing commit block into the disk\n");
	return 0;
#endif

	if (is_hfsplus_jbd_aborted(journal))
		return 0;

	descriptor = hfsplus_jbd_get_descriptor_buffer(journal);
	if (!descriptor)
		return 1;

	bh = hfsplus_jh2bh(descriptor);

	header = (hfsplus_jbd_header_t *)(bh->b_data);
	header->h_magic = cpu_to_be32(JFS_MAGIC_NUMBER);
	header->h_blocktype = cpu_to_be32(JFS_COMMIT_BLOCK);
	header->h_sequence = cpu_to_be32(commit_transaction->t_tid);

	HFSPLUS_JBUFFER_TRACE(descriptor, "write commit block");
	set_buffer_dirty(bh);

	if (journal->j_flags & JFS_BARRIER)
		ret = __sync_dirty_buffer(bh, WRITE_SYNC | WRITE_FLUSH_FUA);
	else
		ret = sync_dirty_buffer(bh);

	put_bh(bh);		/* One for getblk() */
	hfsplus_jbd_put_journal_head(descriptor);

	return (ret == -EIO);
}

static void journal_do_submit_data(struct buffer_head **wbuf, int bufs,
				   int write_op)
{
	int i;

	for (i = 0; i < bufs; i++) {
		wbuf[i]->b_end_io = end_buffer_write_sync;
		/* We use-up our safety reference in submit_bh() */
		submit_bh(write_op, wbuf[i]);
	}
}

/*
 *  Submit all the data buffers to disk
 */
static int journal_submit_data_buffers(hfsplus_jbd_t *journal,
				       hfsplus_transaction_t *commit_transaction,
				       int write_op)
{
	struct hfsplus_jbd_head *jh;
	struct buffer_head *bh;
	int locked;
	int bufs = 0;
	struct buffer_head **wbuf = journal->j_wbuf;
	int err = 0;

	/*
	 * Whenever we unlock the journal and sleep, things can get added
	 * onto ->t_sync_datalist, so we have to keep looping back to
	 * write_out_data until we *know* that the list is empty.
	 *
	 * Cleanup any flushed data buffers from the data list.  Even in
	 * abort mode, we want to flush this out as soon as possible.
	 */
write_out_data:
	cond_resched();
	spin_lock(&journal->j_list_lock);

	while (commit_transaction->t_sync_datalist) {
		jh = commit_transaction->t_sync_datalist;
		bh = hfsplus_jh2bh(jh);
		locked = 0;

		/* Get reference just to make sure buffer does not disappear
		 * when we are forced to drop various locks */
		get_bh(bh);
		/* If the buffer is dirty, we need to submit IO and hence
		 * we need the buffer lock. We try to lock the buffer without
		 * blocking. If we fail, we need to drop j_list_lock and do
		 * blocking lock_buffer().
		 */
		if (buffer_dirty(bh)) {
			if (!trylock_buffer(bh)) {
				HFSPLUS_BUFFER_TRACE(bh, "needs blocking lock");
				spin_unlock(&journal->j_list_lock);
				//trace_jbd_do_submit_data(journal, commit_transaction);
				/* Write out all data to prevent deadlocks */
				journal_do_submit_data(wbuf, bufs, write_op);
				bufs = 0;
				lock_buffer(bh);
				spin_lock(&journal->j_list_lock);
			}
			locked = 1;
		}
		/* We have to get bh_state lock. Again out of order, sigh. */
		if (!inverted_lock(journal, bh)) {
			hfsplus_jbd_lock_bh_state(bh);
			spin_lock(&journal->j_list_lock);
		}
		/* Someone already cleaned up the buffer? */
		if (!buffer_hfsplus_jbd(bh) || hfsplus_bh2jh(bh) != jh
			|| jh->b_transaction != commit_transaction
			|| jh->b_jlist != HFSPLUS_BJ_SyncData) {
			hfsplus_jbd_unlock_bh_state(bh);
			if (locked)
				unlock_buffer(bh);
			HFSPLUS_BUFFER_TRACE(bh, "already cleaned up");
			release_data_buffer(bh);
			continue;
		}
		if (locked && test_clear_buffer_dirty(bh)) {
			HFSPLUS_BUFFER_TRACE(bh, "needs writeout, adding to array");
			wbuf[bufs++] = bh;
			__hfsplus_jbd_file_buffer(jh, commit_transaction,
						HFSPLUS_BJ_Locked);
			hfsplus_jbd_unlock_bh_state(bh);
			if (bufs == journal->j_wbufsize) {
				spin_unlock(&journal->j_list_lock);
				//trace_jbd_do_submit_data(journal, commit_transaction);
				journal_do_submit_data(wbuf, bufs, write_op);
				bufs = 0;
				goto write_out_data;
			}
		} else if (!locked && buffer_locked(bh)) {
			__hfsplus_jbd_file_buffer(jh, commit_transaction,
						HFSPLUS_BJ_Locked);
			hfsplus_jbd_unlock_bh_state(bh);
			put_bh(bh);
		} else {
			HFSPLUS_BUFFER_TRACE(bh, "writeout complete: unfile");
			if (unlikely(!buffer_uptodate(bh)))
				err = -EIO;
			__hfsplus_jbd_unfile_buffer(jh);
			hfsplus_jbd_unlock_bh_state(bh);
			if (locked)
				unlock_buffer(bh);
			release_data_buffer(bh);
		}

		if (need_resched() || spin_needbreak(&journal->j_list_lock)) {
			spin_unlock(&journal->j_list_lock);
			goto write_out_data;
		}
	}
	spin_unlock(&journal->j_list_lock);
	//trace_jbd_do_submit_data(journal, commit_transaction);
	journal_do_submit_data(wbuf, bufs, write_op);

	return err;
}

#ifdef HFSPLUS_JOURNAL_MAC_COMPATIBLE
static void hfsplus_journaled_swap_blhdr(hfsplus_blhdr_t *blhdr)
{
	int i;

	blhdr->bytes_used = cpu_to_be32(blhdr->bytes_used);
	blhdr->checksum = cpu_to_be32(blhdr->checksum);

	for (i=1; i<blhdr->num_blocks; i++) {
		blhdr->binfo[i].bnum = cpu_to_be64(blhdr->binfo[i].bnum);
		blhdr->binfo[i].bsize = cpu_to_be32(blhdr->binfo[i].bsize);
	}

	blhdr->num_blocks = cpu_to_be16(blhdr->num_blocks);
}
#endif

/*
 * hfsplus_jbd_commit_transaction
 *
 * The primary function for committing a transaction to the log.  This
 * function is called by the journal thread to begin a complete commit.
 */
void hfsplus_jbd_commit_transaction(hfsplus_jbd_t *journal)
{
	hfsplus_transaction_t *commit_transaction;
	struct hfsplus_jbd_head *jh, *new_jh, *descriptor;
	struct buffer_head **wbuf = journal->j_wbuf;
	int bufs;
	int flags;
	int err;
	unsigned int blocknr;
	ktime_t start_time;
	u64 commit_time;
#ifdef HFSPLUS_JOURNAL_MAC_COMPATIBLE
	hfsplus_blhdr_t *blhdr = NULL;
	struct super_block *sb = NULL;
	struct hfsplus_journal *jnl = NULL; 
#else
	char *tagp = NULL;
	hfsplus_jbd_header_t *header;
	hfsplus_jbd_block_tag_t *tag = NULL;
	int space_left = 0;
	int first_tag = 0;
	int tag_flag;
#endif
	int i;
	struct blk_plug plug;

	/*
	 * First job: lock down the current transaction and wait for
	 * all outstanding updates to complete.
	 */

	/* Do we need to erase the effects of a prior hfsplus_jbd_flush? */
	if (journal->j_flags & JFS_FLUSHED) {
		jbd_debug(3, "super block updated\n");
		hfsplus_jbd_update_superblock(journal, 1);
	} else {
		jbd_debug(3, "superblock not updated\n");
	}

	HFSPLUS_J_ASSERT(journal->j_running_transaction != NULL);
	HFSPLUS_J_ASSERT(journal->j_committing_transaction == NULL);

	commit_transaction = journal->j_running_transaction;
	HFSPLUS_J_ASSERT(commit_transaction->t_state == HFSPLUS_T_RUNNING);

	//trace_jbd_start_commit(journal, commit_transaction);
	jbd_debug(1, "JBD: starting commit of transaction %d\n",
			commit_transaction->t_tid);

	spin_lock(&journal->j_state_lock);
	commit_transaction->t_state = HFSPLUS_T_LOCKED;

	//trace_jbd_commit_locking(journal, commit_transaction);
	spin_lock(&commit_transaction->t_handle_lock);
	while (commit_transaction->t_updates) {
		DEFINE_WAIT(wait);

		prepare_to_wait(&journal->j_wait_updates, &wait,
					TASK_UNINTERRUPTIBLE);
		if (commit_transaction->t_updates) {
			spin_unlock(&commit_transaction->t_handle_lock);
			spin_unlock(&journal->j_state_lock);
			schedule();
			spin_lock(&journal->j_state_lock);
			spin_lock(&commit_transaction->t_handle_lock);
		}
		finish_wait(&journal->j_wait_updates, &wait);
	}
	spin_unlock(&commit_transaction->t_handle_lock);

	HFSPLUS_J_ASSERT (commit_transaction->t_outstanding_credits <=
			journal->j_max_transaction_buffers);

	/*
	 * First thing we are allowed to do is to discard any remaining
	 * BJ_Reserved buffers.  Note, it is _not_ permissible to assume
	 * that there are no such buffers: if a large filesystem
	 * operation like a truncate needs to split itself over multiple
	 * transactions, then it may try to do a journal_restart() while
	 * there are still BJ_Reserved buffers outstanding.  These must
	 * be released cleanly from the current transaction.
	 *
	 * In this case, the filesystem must still reserve write access
	 * again before modifying the buffer in the new transaction, but
	 * we do not require it to remember exactly which old buffers it
	 * has reserved.  This is consistent with the existing behaviour
	 * that multiple journal_get_write_access() calls to the same
	 * buffer are perfectly permissible.
	 */
	while (commit_transaction->t_reserved_list) {
		jh = commit_transaction->t_reserved_list;
		HFSPLUS_JBUFFER_TRACE(jh, "reserved, unused: refile");
		/*
		 * A journal_get_undo_access()+journal_release_buffer() may
		 * leave undo-committed data.
		 */
		if (jh->b_committed_data) {
			struct buffer_head *bh = hfsplus_jh2bh(jh);

			hfsplus_jbd_lock_bh_state(bh);
			hfsplus_jbd_free(jh->b_committed_data, bh->b_size);
			jh->b_committed_data = NULL;
			hfsplus_jbd_unlock_bh_state(bh);
		}
		hfsplus_jbd_refile_buffer(journal, jh);
	}

	/*
	 * Now try to drop any written-back buffers from the journal's
	 * checkpoint lists.  We do this *before* commit because it potentially
	 * frees some memory
	 */
	spin_lock(&journal->j_list_lock);
	__hfsplus_jbd_clean_checkpoint_list(journal);
	spin_unlock(&journal->j_list_lock);

	jbd_debug (3, "JBD: commit phase 1\n");

	/*
	 * Clear revoked flag to reflect there is no revoked buffers
	 * in the next transaction which is going to be started.
	 */
	hfsplus_jbd_clear_buffer_revoked_flags(journal);

	/*
	 * Switch to a new revoke table.
	 */
	hfsplus_jbd_switch_revoke_table(journal);

	//trace_jbd_commit_flushing(journal, commit_transaction);
	commit_transaction->t_state = HFSPLUS_T_FLUSH;
	journal->j_committing_transaction = commit_transaction;
	journal->j_running_transaction = NULL;
	start_time = ktime_get();
	commit_transaction->t_log_start = journal->j_head;
	wake_up(&journal->j_wait_transaction_locked);
	spin_unlock(&journal->j_state_lock);

	jbd_debug (3, "JBD: commit phase 2\n");

	/*
	 * Now start flushing things to disk, in the order they appear
	 * on the transaction lists.  Data blocks go first.
	 */
	blk_start_plug(&plug);
	err = journal_submit_data_buffers(journal, commit_transaction,
					  WRITE_SYNC);
	blk_finish_plug(&plug);

	/*
	 * Wait for all previously submitted IO to complete.
	 */
	spin_lock(&journal->j_list_lock);
	while (commit_transaction->t_locked_list) {
		struct buffer_head *bh;

		jh = commit_transaction->t_locked_list->b_tprev;
		bh = hfsplus_jh2bh(jh);
		get_bh(bh);
		if (buffer_locked(bh)) {
			spin_unlock(&journal->j_list_lock);
			wait_on_buffer(bh);
			spin_lock(&journal->j_list_lock);
		}
		if (unlikely(!buffer_uptodate(bh))) {
			if (!trylock_page(bh->b_page)) {
				spin_unlock(&journal->j_list_lock);
				lock_page(bh->b_page);
				spin_lock(&journal->j_list_lock);
			}
			if (bh->b_page->mapping)
				set_bit(AS_EIO, &bh->b_page->mapping->flags);

			unlock_page(bh->b_page);
			SetPageError(bh->b_page);
			err = -EIO;
		}
		if (!inverted_lock(journal, bh)) {
			put_bh(bh);
			spin_lock(&journal->j_list_lock);
			continue;
		}
		if (buffer_hfsplus_jbd(bh) && hfsplus_bh2jh(bh) == jh &&
		    jh->b_transaction == commit_transaction &&
		    jh->b_jlist == HFSPLUS_BJ_Locked)
			__hfsplus_jbd_unfile_buffer(jh);
		hfsplus_jbd_unlock_bh_state(bh);
		release_data_buffer(bh);
		cond_resched_lock(&journal->j_list_lock);
	}
	spin_unlock(&journal->j_list_lock);

	if (err) {
		char b[BDEVNAME_SIZE];

		printk(KERN_WARNING
			"JBD: Detected IO errors while flushing file data "
			"on %s\n", bdevname(journal->j_fs_dev, b));
		if (journal->j_flags & JFS_ABORT_ON_SYNCDATA_ERR)
			hfsplus_jbd_abort(journal, err);
		err = 0;
	}

	blk_start_plug(&plug);

	hfsplus_jbd_write_revoke_records(journal, commit_transaction, WRITE_SYNC);

	/*
	 * If we found any dirty or locked buffers, then we should have
	 * looped back up to the write_out_data label.  If there weren't
	 * any then journal_clean_data_list should have wiped the list
	 * clean by now, so check that it is in fact empty.
	 */
	HFSPLUS_J_ASSERT (commit_transaction->t_sync_datalist == NULL);

	jbd_debug (3, "JBD: commit phase 3\n");

	/*
	 * Way to go: we have now written out all of the data for a
	 * transaction!  Now comes the tricky part: we need to write out
	 * metadata.  Loop over the transaction's entire buffer list:
	 */
	spin_lock(&journal->j_state_lock);
	commit_transaction->t_state = HFSPLUS_T_COMMIT;
	spin_unlock(&journal->j_state_lock);

	//trace_jbd_commit_logging(journal, commit_transaction);
	HFSPLUS_J_ASSERT(commit_transaction->t_nr_buffers <=
		 commit_transaction->t_outstanding_credits);

	descriptor = NULL;
#ifdef HFSPLUS_JOURNAL_MAC_COMPATIBLE
	sb = (struct super_block *)journal->j_private;
	jnl = &(HFSPLUS_SB(sb)->jnl);
#endif
	bufs = 0;
	while (commit_transaction->t_buffers) {

		/* Find the next buffer to be journaled... */

		jh = commit_transaction->t_buffers;

		/* If we're in abort mode, we just un-journal the buffer and
		   release it. */

		if (is_hfsplus_jbd_aborted(journal)) {
			clear_buffer_hfsplus_jbddirty(hfsplus_jh2bh(jh));
			HFSPLUS_JBUFFER_TRACE(jh, "journal is aborting: refile");
			hfsplus_jbd_refile_buffer(journal, jh);
			/* If that was the last one, we need to clean up
			 * any descriptor buffers which may have been
			 * already allocated, even if we are now
			 * aborting. */
			if (!commit_transaction->t_buffers)
				goto start_hfsplus_jbd_io;
			continue;
		}

		/* Make sure we have a descriptor block in which to
		   record the metadata buffer. */

		if (!descriptor) {
			struct buffer_head *bh;

			HFSPLUS_J_ASSERT (bufs == 0);

			jbd_debug(4, "JBD: get descriptor\n");

			descriptor = hfsplus_jbd_get_descriptor_buffer(journal);
			if (!descriptor) {
				hfsplus_jbd_abort(journal, -EIO);
				continue;
			}

			bh = hfsplus_jh2bh(descriptor);
			jbd_debug(4, "JBD: got buffer %llu (%p)\n",
				(unsigned long long)bh->b_blocknr, bh->b_data);
#ifdef HFSPLUS_JOURNAL_MAC_COMPATIBLE
			/* Populate block list header */
			blhdr = (hfsplus_blhdr_t *)bh->b_data;
			blhdr->max_blocks = (jnl->jhdr->blhdr_size / sizeof(struct hfsplus_block_info)) - 1;
			blhdr->num_blocks = 1; /* One is for header */
			blhdr->bytes_used = jnl->jhdr->blhdr_size; 
			blhdr->binfo[0].next = 0; /* Only one Mac transaction */
			hfsplus_journal_header_end_update(journal, jnl->jhdr);
#else
			header = (hfsplus_jbd_header_t *)&bh->b_data[0];
			header->h_magic     = cpu_to_be32(JFS_MAGIC_NUMBER);
			header->h_blocktype = cpu_to_be32(JFS_DESCRIPTOR_BLOCK);
			header->h_sequence  = cpu_to_be32(commit_transaction->t_tid);

			tagp = &bh->b_data[sizeof(hfsplus_jbd_header_t)];
			space_left = bh->b_size - sizeof(hfsplus_jbd_header_t);
			first_tag = 1;
#endif
			set_buffer_hfsplus_jbd_jwrite(bh);
			set_buffer_dirty(bh);
			wbuf[bufs++] = bh;

			/* Record it so that we can wait for IO
                           completion later */
			HFSPLUS_BUFFER_TRACE(bh, "ph3: file as descriptor");
			hfsplus_jbd_file_buffer(descriptor, commit_transaction,
					HFSPLUS_BJ_LogCtl);
		}

		/* Where is the buffer to be written? */

		err = hfsplus_jbd_next_log_block(journal, &blocknr);
		/* If the block mapping failed, just abandon the buffer
		   and repeat this loop: we'll fall into the
		   refile-on-abort condition above. */
		if (err) {
			hfsplus_jbd_abort(journal, err);
			continue;
		}

		/*
		 * start_this_handle() uses t_outstanding_credits to determine
		 * the free space in the log, but this counter is changed
		 * by hfsplus_jbd_next_log_block() also.
		 */
		commit_transaction->t_outstanding_credits--;

		/* Bump b_count to prevent truncate from stumbling over
                   the shadowed buffer!  @@@ This can go if we ever get
                   rid of the HFSPLUS_BJ_IO/BJ_Shadow pairing of buffers. */
		get_bh(hfsplus_jh2bh(jh));

		/* Make a temporary IO buffer with which to write it out
                   (this will requeue both the metadata buffer and the
                   temporary IO buffer). new_bh goes on HFSPLUS_BJ_IO*/

		set_buffer_hfsplus_jbd_jwrite(hfsplus_jh2bh(jh));
		/*
		 * akpm: hfsplus_jbd_write_metadata_buffer() sets
		 * new_bh->b_transaction to commit_transaction.
		 * We need to clean this up before we release new_bh
		 * (which is of type HFSPLUS_BJ_IO)
		 */
		HFSPLUS_JBUFFER_TRACE(jh, "ph3: write metadata");
		flags = hfsplus_jbd_write_metadata_buffer(commit_transaction,
						      jh, &new_jh, blocknr);
		set_buffer_hfsplus_jbd_jwrite(hfsplus_jh2bh(new_jh));
#ifdef HFSPLUS_JOURNAL_MAC_COMPATIBLE
		blhdr->binfo[bufs].bnum = (hfsplus_jh2bh(jh)->b_blocknr * sb->s_blocksize) >> HFSPLUS_SECTOR_SHIFT;
		blhdr->binfo[bufs].bsize = hfsplus_jh2bh(jh)->b_size; 
		blhdr->binfo[bufs].next = 0;
		blhdr->bytes_used += blhdr->binfo[bufs].bsize;
		blhdr->num_blocks++;
		hfsplus_journal_header_end_update(journal, jnl->jhdr);
#endif
		wbuf[bufs++] = hfsplus_jh2bh(new_jh);

#ifndef HFSPLUS_JOURNAL_MAC_COMPATIBLE
		/* Record the new block's tag in the current descriptor
                   buffer */

		tag_flag = 0;
		if (flags & 1)
			tag_flag |= JFS_FLAG_ESCAPE;
		if (!first_tag)
			tag_flag |= JFS_FLAG_SAME_UUID;

		tag = (hfsplus_jbd_block_tag_t *) tagp;
		tag->t_blocknr = cpu_to_be32(hfsplus_jh2bh(jh)->b_blocknr);
		tag->t_flags = cpu_to_be32(tag_flag);
		tagp += sizeof(hfsplus_jbd_block_tag_t);
		space_left -= sizeof(hfsplus_jbd_block_tag_t);

		if (first_tag) {
			memcpy (tagp, journal->j_uuid, 16);
			tagp += 16;
			space_left -= 16;
			first_tag = 0;
		}
#endif

		/* If there's no more to do, or if the descriptor is full,
		   let the IO rip! */

#ifdef HFSPLUS_JOURNAL_MAC_COMPATIBLE
		if (bufs == journal->j_wbufsize ||
				commit_transaction->t_buffers == NULL ||
				bufs == blhdr->max_blocks)
#else
		if (bufs == journal->j_wbufsize ||
		    commit_transaction->t_buffers == NULL ||
		    space_left < sizeof(hfsplus_jbd_block_tag_t) + 16) 
#endif
        {

#ifdef HFSPLUS_JOURNAL_MAC_COMPATIBLE
			dprint(DBG_JCOMMIT, "start: %llx, end: %llx, num_blocks: %#x, bytes_used: %#x, j_head: %#x, j_first: %#x\n", jnl->jhdr->start, jnl->jhdr->end, blhdr->num_blocks, blhdr->bytes_used, journal->j_head, journal->j_first);
			blhdr->max_blocks = HFSPLUS_JBD_MAGIC_NUMBER;
			if (jnl->flags == HFSPLUS_JOURNAL_SWAP)
				hfsplus_journaled_swap_blhdr(blhdr);
#endif
			jbd_debug(4, "JBD: Submit %d IOs\n", bufs);

			/* Write an end-of-descriptor marker before
                           submitting the IOs.  "tag" still points to
                           the last tag we set up. */
#ifndef HFSPLUS_JOURNAL_MAC_COMPATIBLE
			tag->t_flags |= cpu_to_be32(JFS_FLAG_LAST_TAG);
#endif

start_hfsplus_jbd_io:
			for (i = 0; i < bufs; i++) {
				struct buffer_head *bh = wbuf[i];
				lock_buffer(bh);
				clear_buffer_dirty(bh);
				set_buffer_uptodate(bh);
				bh->b_end_io = hfsplus_jbd_end_buffer_io_sync;
				submit_bh(WRITE_SYNC, bh);
			}
			cond_resched();

			/* Force a new descriptor to be generated next
                           time round the loop. */
			descriptor = NULL;
			bufs = 0;
		}
	}

	blk_finish_plug(&plug);

	/* Lo and behold: we have just managed to send a transaction to
           the log.  Before we can commit it, wait for the IO so far to
           complete.  Control buffers being written are on the
           transaction's t_log_list queue, and metadata buffers are on
           the t_iobuf_list queue.

	   Wait for the buffers in reverse order.  That way we are
	   less likely to be woken up until all IOs have completed, and
	   so we incur less scheduling load.
	*/

	dprint(DBG_JCOMMIT, "JBD: commit phase 4\n");

	/*
	 * akpm: these are HFSPLUS_BJ_IO, and j_list_lock is not needed.
	 * See __hfsplus_jbd_try_to_free_buffer.
	 */
wait_for_iobuf:
	while (commit_transaction->t_iobuf_list != NULL) {
		struct buffer_head *bh;

		jh = commit_transaction->t_iobuf_list->b_tprev;
		bh = hfsplus_jh2bh(jh);
		if (buffer_locked(bh)) {
			wait_on_buffer(bh);
			goto wait_for_iobuf;
		}
		if (cond_resched())
			goto wait_for_iobuf;

		if (unlikely(!buffer_uptodate(bh)))
			err = -EIO;

		clear_buffer_hfsplus_jbd_jwrite(bh);

		HFSPLUS_JBUFFER_TRACE(jh, "ph4: unfile after journal write");
		hfsplus_jbd_unfile_buffer(journal, jh);

		/*
		 * ->t_iobuf_list should contain only dummy buffer_heads
		 * which were created by hfsplus_jbd_write_metadata_buffer().
		 */
		HFSPLUS_BUFFER_TRACE(bh, "dumping temporary bh");
		hfsplus_jbd_put_journal_head(jh);
		__brelse(bh);
		HFSPLUS_J_ASSERT_BH(bh, atomic_read(&bh->b_count) == 0);
		free_buffer_head(bh);

		/* We also have to unlock and free the corresponding
                   shadowed buffer */
		jh = commit_transaction->t_shadow_list->b_tprev;
		bh = hfsplus_jh2bh(jh);
		clear_buffer_hfsplus_jbd_jwrite(bh);
		HFSPLUS_J_ASSERT_BH(bh, buffer_hfsplus_jbddirty(bh));

		/* The metadata is now released for reuse, but we need
                   to remember it against this transaction so that when
                   we finally commit, we can do any checkpointing
                   required. */
		HFSPLUS_JBUFFER_TRACE(jh, "file as HFSPLUS_BJ_Forget");
		hfsplus_jbd_file_buffer(jh, commit_transaction, HFSPLUS_BJ_Forget);
		/*
		 * Wake up any transactions which were waiting for this
		 * IO to complete. The barrier must be here so that changes
		 * by hfsplus_jbd_file_buffer() take effect before wake_up_bit()
		 * does the waitqueue check.
		 */
		smp_mb();
		wake_up_bit(&bh->b_state, BH_HFSPLUS_Unshadow);
		HFSPLUS_JBUFFER_TRACE(jh, "brelse shadowed buffer");
		__brelse(bh);
	}

	HFSPLUS_J_ASSERT (commit_transaction->t_shadow_list == NULL);

	dprint(DBG_JCOMMIT, "JBD: commit phase 5\n");

	/* Here we wait for the revoke record and descriptor record buffers */
 wait_for_ctlbuf:
	while (commit_transaction->t_log_list != NULL) {
		struct buffer_head *bh;

		jh = commit_transaction->t_log_list->b_tprev;
		bh = hfsplus_jh2bh(jh);
		if (buffer_locked(bh)) {
			wait_on_buffer(bh);
			goto wait_for_ctlbuf;
		}
		if (cond_resched())
			goto wait_for_ctlbuf;

		if (unlikely(!buffer_uptodate(bh)))
			err = -EIO;

		HFSPLUS_BUFFER_TRACE(bh, "ph5: control buffer writeout done: unfile");
		clear_buffer_hfsplus_jbd_jwrite(bh);
		hfsplus_jbd_unfile_buffer(journal, jh);
		hfsplus_jbd_put_journal_head(jh);
		__brelse(bh);		/* One for getblk */
		/* AKPM: bforget here */
	}

	if (err)
		hfsplus_jbd_abort(journal, err);

	dprint(DBG_JCOMMIT, "JBD: commit phase 6\n");

	/* All metadata is written, now write commit record and do cleanup */
	spin_lock(&journal->j_state_lock);
	HFSPLUS_J_ASSERT(commit_transaction->t_state == HFSPLUS_T_COMMIT);
	commit_transaction->t_state = HFSPLUS_T_COMMIT_RECORD;
	spin_unlock(&journal->j_state_lock);

	if (hfsplus_jbd_write_commit_record(journal, commit_transaction))
		err = -EIO;

	if (err)
		hfsplus_jbd_abort(journal, err);

	/* End of a transaction!  Finally, we can do checkpoint
           processing: any buffers committed as a result of this
           transaction can be removed from any checkpoint list it was on
           before. */

	dprint(DBG_JCOMMIT, "JBD: commit phase 7\n");

	HFSPLUS_J_ASSERT(commit_transaction->t_sync_datalist == NULL);
	HFSPLUS_J_ASSERT(commit_transaction->t_buffers == NULL);
	HFSPLUS_J_ASSERT(commit_transaction->t_checkpoint_list == NULL);
	HFSPLUS_J_ASSERT(commit_transaction->t_iobuf_list == NULL);
	HFSPLUS_J_ASSERT(commit_transaction->t_shadow_list == NULL);
	HFSPLUS_J_ASSERT(commit_transaction->t_log_list == NULL);

restart_loop:
	/*
	 * As there are other places (hfsplus_jbd_unmap_buffer()) adding buffers
	 * to this list we have to be careful and hold the j_list_lock.
	 */
	spin_lock(&journal->j_list_lock);
	while (commit_transaction->t_forget) {
		hfsplus_transaction_t *cp_transaction;
		struct buffer_head *bh;
		int try_to_free = 0;

		jh = commit_transaction->t_forget;
		spin_unlock(&journal->j_list_lock);
		bh = hfsplus_jh2bh(jh);
		/*
		 * Get a reference so that bh cannot be freed before we are
		 * done with it.
		 */
		get_bh(bh);
		hfsplus_jbd_lock_bh_state(bh);
		HFSPLUS_J_ASSERT_JH(jh,	jh->b_transaction == commit_transaction ||
			jh->b_transaction == journal->j_running_transaction);

		/*
		 * If there is undo-protected committed data against
		 * this buffer, then we can remove it now.  If it is a
		 * buffer needing such protection, the old frozen_data
		 * field now points to a committed version of the
		 * buffer, so rotate that field to the new committed
		 * data.
		 *
		 * Otherwise, we can just throw away the frozen data now.
		 */
		if (jh->b_committed_data) {
			hfsplus_jbd_free(jh->b_committed_data, bh->b_size);
			jh->b_committed_data = NULL;
			if (jh->b_frozen_data) {
				jh->b_committed_data = jh->b_frozen_data;
				jh->b_frozen_data = NULL;
			}
		} else if (jh->b_frozen_data) {
			hfsplus_jbd_free(jh->b_frozen_data, bh->b_size);
			jh->b_frozen_data = NULL;
		}

		spin_lock(&journal->j_list_lock);
		cp_transaction = jh->b_cp_transaction;
		if (cp_transaction) {
			HFSPLUS_JBUFFER_TRACE(jh, "remove from old cp transaction");
			__hfsplus_jbd_remove_checkpoint(jh);
		}

		/* Only re-checkpoint the buffer_head if it is marked
		 * dirty.  If the buffer was added to the HFSPLUS_BJ_Forget list
		 * by hfsplus_jbd_forget, it may no longer be dirty and
		 * there's no point in keeping a checkpoint record for
		 * it. */

		/* A buffer which has been freed while still being
		 * journaled by a previous transaction may end up still
		 * being dirty here, but we want to avoid writing back
		 * that buffer in the future after the "add to orphan"
		 * operation been committed,  That's not only a performance
		 * gain, it also stops aliasing problems if the buffer is
		 * left behind for writeback and gets reallocated for another
		 * use in a different page. */
		if (buffer_hfsplus_jbd_freed(bh) && !jh->b_next_transaction) {
			clear_buffer_hfsplus_jbd_freed(bh);
			clear_buffer_hfsplus_jbddirty(bh);
		}

		if (buffer_hfsplus_jbddirty(bh)) {
			HFSPLUS_JBUFFER_TRACE(jh, "add to new checkpointing trans");
			__hfsplus_jbd_insert_checkpoint(jh, commit_transaction);
			if (is_hfsplus_jbd_aborted(journal))
				clear_buffer_hfsplus_jbddirty(bh);
		} else {
			HFSPLUS_J_ASSERT_BH(bh, !buffer_dirty(bh));
			/*
			 * The buffer on HFSPLUS_BJ_Forget list and not jbddirty means
			 * it has been freed by this transaction and hence it
			 * could not have been reallocated until this
			 * transaction has committed. *BUT* it could be
			 * reallocated once we have written all the data to
			 * disk and before we process the buffer on HFSPLUS_BJ_Forget
			 * list.
			 */
			if (!jh->b_next_transaction)
				try_to_free = 1;
		}
		HFSPLUS_JBUFFER_TRACE(jh, "refile or unfile freed buffer");
		__hfsplus_jbd_refile_buffer(jh);
		hfsplus_jbd_unlock_bh_state(bh);
		if (try_to_free)
			release_buffer_page(bh);
		else
			__brelse(bh);
		cond_resched_lock(&journal->j_list_lock);
	}
	spin_unlock(&journal->j_list_lock);
	/*
	 * This is a bit sleazy.  We use j_list_lock to protect transition
	 * of a transaction into T_FINISHED state and calling
	 * __hfsplus_jbd_drop_transaction(). Otherwise we could race with
	 * other checkpointing code processing the transaction...
	 */
	spin_lock(&journal->j_state_lock);
	spin_lock(&journal->j_list_lock);
	/*
	 * Now recheck if some buffers did not get attached to the transaction
	 * while the lock was dropped...
	 */
	if (commit_transaction->t_forget) {
		spin_unlock(&journal->j_list_lock);
		spin_unlock(&journal->j_state_lock);
		goto restart_loop;
	}

	/* Done with this transaction! */

	dprint(DBG_JCOMMIT, "JBD: commit phase 8\n");

	HFSPLUS_J_ASSERT(commit_transaction->t_state == HFSPLUS_T_COMMIT_RECORD);

	commit_transaction->t_state = HFSPLUS_T_FINISHED;
	HFSPLUS_J_ASSERT(commit_transaction == journal->j_committing_transaction);
	journal->j_commit_sequence = commit_transaction->t_tid;
	journal->j_committing_transaction = NULL;
	commit_time = ktime_to_ns(ktime_sub(ktime_get(), start_time));

	/*
	 * weight the commit time higher than the average time so we don't
	 * react too strongly to vast changes in commit time
	 */
	if (likely(journal->j_average_commit_time))
		journal->j_average_commit_time = (commit_time*3 +
				journal->j_average_commit_time) / 4;
	else
		journal->j_average_commit_time = commit_time;

	spin_unlock(&journal->j_state_lock);

	if (commit_transaction->t_checkpoint_list == NULL &&
	    commit_transaction->t_checkpoint_io_list == NULL) {
		__hfsplus_jbd_drop_transaction(journal, commit_transaction);
	} else {
		if (journal->j_checkpoint_transactions == NULL) {
			journal->j_checkpoint_transactions = commit_transaction;
			commit_transaction->t_cpnext = commit_transaction;
			commit_transaction->t_cpprev = commit_transaction;
		} else {
			commit_transaction->t_cpnext =
				journal->j_checkpoint_transactions;
			commit_transaction->t_cpprev =
				commit_transaction->t_cpnext->t_cpprev;
			commit_transaction->t_cpnext->t_cpprev =
				commit_transaction;
			commit_transaction->t_cpprev->t_cpnext =
				commit_transaction;
		}
	}
	spin_unlock(&journal->j_list_lock);

	//trace_jbd_end_commit(journal, commit_transaction);
	dprint(DBG_JCOMMIT, "JBD: commit %d complete, head %d\n", journal->j_commit_sequence, journal->j_tail_sequence);

	wake_up(&journal->j_wait_done_commit);
}
