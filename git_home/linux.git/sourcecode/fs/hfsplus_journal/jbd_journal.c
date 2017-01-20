/*
 * linux/fs/hfsplus_journal/jbd_journal.c
 *
 * Written by Stephen C. Tweedie <sct@redhat.com>, 1998
 *
 * Copyright 1998 Red Hat corp --- All Rights Reserved
 *
 * This file is part of the Linux kernel and is made available under
 * the terms of the GNU General Public License, version 2, or at your
 * option, any later version, incorporated herein by reference.
 *
 * Generic filesystem journal-writing code; part of the ext2fs
 * journaling system.
 *
 * This file manages journals: areas of disk reserved for logging
 * transactional updates.  This includes the kernel journaling thread
 * which is responsible for scheduling updates to the log.
 *
 * We do not actually manage the physical storage of the journal in this
 * file: that is left to a per-journal policy function, which allows us
 * to store the journal within a filesystem-specified area for ext2
 * journaling (ext2 can use a reserved inode for storing the log).
 */

#include <linux/module.h>
#include <linux/time.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/slab.h>
//#include <linux/smp_lock.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/suspend.h>
#include <linux/freezer.h>
#include <linux/pagemap.h>
#include <linux/kthread.h>
#include <linux/poison.h>
#include <linux/proc_fs.h>
#include <linux/debugfs.h>
#include <linux/ratelimit.h>

#define CREATE_TRACE_POINTS
#include <trace/events/jbd.h>

#include <asm/uaccess.h>
#include <asm/page.h>
#include <linux/proc_fs.h>
#include "hfsplus_jbd.h"
#include "hfsplus_fs.h"

EXPORT_SYMBOL(hfsplus_jbd_start);
EXPORT_SYMBOL(hfsplus_jbd_restart);
EXPORT_SYMBOL(hfsplus_jbd_extend);
EXPORT_SYMBOL(hfsplus_jbd_stop);
EXPORT_SYMBOL(hfsplus_jbd_lock_updates);
EXPORT_SYMBOL(hfsplus_jbd_unlock_updates);
EXPORT_SYMBOL(hfsplus_jbd_get_write_access);
EXPORT_SYMBOL(hfsplus_jbd_get_create_access);
EXPORT_SYMBOL(hfsplus_jbd_get_undo_access);
EXPORT_SYMBOL(hfsplus_jbd_dirty_data);
EXPORT_SYMBOL(hfsplus_jbd_dirty_metadata);
EXPORT_SYMBOL(hfsplus_jbd_release_buffer);
EXPORT_SYMBOL(hfsplus_jbd_forget);
#if 0
EXPORT_SYMBOL(hfsplus_jbd_sync_buffer);
#endif
EXPORT_SYMBOL(hfsplus_jbd_flush);
EXPORT_SYMBOL(hfsplus_jbd_revoke);

EXPORT_SYMBOL(hfsplus_jbd_init_dev);
EXPORT_SYMBOL(hfsplus_jbd_init_inode);
EXPORT_SYMBOL(hfsplus_jbd_update_format);
EXPORT_SYMBOL(hfsplus_jbd_check_used_features);
EXPORT_SYMBOL(hfsplus_jbd_check_available_features);
EXPORT_SYMBOL(hfsplus_jbd_set_features);
EXPORT_SYMBOL(hfsplus_jbd_create);
EXPORT_SYMBOL(hfsplus_jbd_load);
EXPORT_SYMBOL(hfsplus_jbd_destroy);
EXPORT_SYMBOL(hfsplus_jbd_update_superblock);
EXPORT_SYMBOL(hfsplus_jbd_abort);
EXPORT_SYMBOL(hfsplus_jbd_errno);
EXPORT_SYMBOL(hfsplus_jbd_ack_err);
EXPORT_SYMBOL(hfsplus_jbd_clear_err);
EXPORT_SYMBOL(hfsplus_jbd_log_wait_commit);
EXPORT_SYMBOL(hfsplus_jbd_start_commit);
EXPORT_SYMBOL(hfsplus_jbd_force_commit_nested);
EXPORT_SYMBOL(hfsplus_jbd_wipe);
EXPORT_SYMBOL(hfsplus_jbd_blocks_per_page);
EXPORT_SYMBOL(hfsplus_jbd_invalidatepage);
EXPORT_SYMBOL(hfsplus_jbd_try_to_free_buffers);
EXPORT_SYMBOL(hfsplus_jbd_force_commit);

static int hfsplus_jbd_convert_superblock_v1(hfsplus_jbd_t *, hfsplus_jbd_superblock_t *);
static void __hfsplus_jbd_abort_soft (hfsplus_jbd_t *journal, int errno);
static const char *hfsplus_jbd_dev_name(hfsplus_jbd_t *journal, char *buffer);

/*
 * Helper function used to manage commit timeouts
 */

static void commit_timeout(unsigned long __data)
{
	struct task_struct * p = (struct task_struct *) __data;

	wake_up_process(p);
}

/*
 * hfsplus_kjournald: The main thread function used to manage a logging device
 * journal.
 *
 * This kernel thread is responsible for two things:
 *
 * 1) COMMIT:  Every so often we need to commit the current state of the
 *    filesystem to disk.  The journal thread is responsible for writing
 *    all of the metadata buffers to disk.
 *
 * 2) CHECKPOINT: We cannot reuse a used section of the log file until all
 *    of the data in that part of the log has been rewritten elsewhere on
 *    the disk.  Flushing these old buffers to reclaim space in the log is
 *    known as checkpointing, and this thread is responsible for that job.
 */

static int hfsplus_kjournald(void *arg)
{
	hfsplus_jbd_t *journal = (hfsplus_jbd_t *) arg;
	hfsplus_transaction_t *transaction;

	/*
	 * Set up an interval timer which can be used to trigger a commit wakeup
	 * after the commit interval expires
	 */
	setup_timer(&journal->j_commit_timer, commit_timeout,
			(unsigned long)current);

	set_freezable();

	/* Record that the journal thread is running */
	journal->j_task = current;
	wake_up(&journal->j_wait_done_commit);

	printk(KERN_INFO "hfsplus_kjournald starting.  Commit interval %ld seconds\n",
			journal->j_commit_interval / HZ);

	/*
	 * And now, wait forever for commit wakeup events.
	 */
	spin_lock(&journal->j_state_lock);

loop:
	if (journal->j_flags & JFS_UNMOUNT)
		goto end_loop;

	hfsplus_jbd_debug(1, "commit_sequence=%d, commit_request=%d\n",
		journal->j_commit_sequence, journal->j_commit_request);

	if (journal->j_commit_sequence != journal->j_commit_request) {
		hfsplus_jbd_debug(1, "OK, requests differ\n");
		spin_unlock(&journal->j_state_lock);
		del_timer_sync(&journal->j_commit_timer);
		hfsplus_jbd_commit_transaction(journal);
		spin_lock(&journal->j_state_lock);
		goto loop;
	}

	wake_up(&journal->j_wait_done_commit);
	if (freezing(current)) {
		/*
		 * The simpler the better. Flushing journal isn't a
		 * good idea, because that depends on threads that may
		 * be already stopped.
		 */
		hfsplus_jbd_debug(1, "Now suspending hfsplus_kjournald\n");
		spin_unlock(&journal->j_state_lock);
		try_to_freeze();
		spin_lock(&journal->j_state_lock);
	} else {
		/*
		 * We assume on resume that commits are already there,
		 * so we don't sleep
		 */
		DEFINE_WAIT(wait);
		int should_sleep = 1;

		prepare_to_wait(&journal->j_wait_commit, &wait,
				TASK_INTERRUPTIBLE);
		if (journal->j_commit_sequence != journal->j_commit_request)
			should_sleep = 0;
		transaction = journal->j_running_transaction;
		if (transaction && time_after_eq(jiffies,
						transaction->t_expires))
			should_sleep = 0;
		if (journal->j_flags & JFS_UNMOUNT)
			should_sleep = 0;
		if (should_sleep) {
			spin_unlock(&journal->j_state_lock);
			schedule();
			spin_lock(&journal->j_state_lock);
		}
		finish_wait(&journal->j_wait_commit, &wait);
	}

	hfsplus_jbd_debug(1, "hfsplus_kjournald wakes\n");

	/*
	 * Were we woken up by a commit wakeup event?
	 */
	transaction = journal->j_running_transaction;
	if (transaction && time_after_eq(jiffies, transaction->t_expires)) {
		journal->j_commit_request = transaction->t_tid;
		hfsplus_jbd_debug(1, "woke because of timeout\n");
	}
	goto loop;

end_loop:
	spin_unlock(&journal->j_state_lock);
	del_timer_sync(&journal->j_commit_timer);
	journal->j_task = NULL;
	wake_up(&journal->j_wait_done_commit);
	hfsplus_jbd_debug(1, "Journal thread exiting.\n");
	return 0;
}

static int hfsplus_jbd_start_thread(hfsplus_jbd_t *journal)
{
	struct task_struct *t;

	t = kthread_run(hfsplus_kjournald, journal, "hfsplus_kjournald");
	if (IS_ERR(t))
		return PTR_ERR(t);

	wait_event(journal->j_wait_done_commit, journal->j_task != NULL);
	return 0;
}

static void hfsplus_jbd_kill_thread(hfsplus_jbd_t *journal)
{
	spin_lock(&journal->j_state_lock);
	journal->j_flags |= JFS_UNMOUNT;

	while (journal->j_task) {
		wake_up(&journal->j_wait_commit);
		spin_unlock(&journal->j_state_lock);
		wait_event(journal->j_wait_done_commit,
				journal->j_task == NULL);
		spin_lock(&journal->j_state_lock);
	}
	spin_unlock(&journal->j_state_lock);
}

/*
 * hfsplus_jbd_write_metadata_buffer: write a metadata buffer to the journal.
 *
 * Writes a metadata buffer to a given disk block.  The actual IO is not
 * performed but a new buffer_head is constructed which labels the data
 * to be written with the correct destination disk block.
 *
 * Any magic-number escaping which needs to be done will cause a
 * copy-out here.  If the buffer happens to start with the
 * JFS_MAGIC_NUMBER, then we can't write it to the log directly: the
 * magic number is only written to the log for descripter blocks.  In
 * this case, we copy the data and replace the first word with 0, and we
 * return a result code which indicates that this buffer needs to be
 * marked as an escaped buffer in the corresponding log descriptor
 * block.  The missing word can then be restored when the block is read
 * during recovery.
 *
 * If the source buffer has already been modified by a new transaction
 * since we took the last commit snapshot, we use the frozen copy of
 * that data for IO.  If we end up using the existing buffer_head's data
 * for the write, then we *have* to lock the buffer to prevent anyone
 * else from using and possibly modifying it while the IO is in
 * progress.
 *
 * The function returns a pointer to the buffer_heads to be used for IO.
 *
 * We assume that the journal has already been locked in this function.
 *
 * Return value:
 *  <0: Error
 * >=0: Finished OK
 *
 * On success:
 * Bit 0 set == escape performed on the data
 * Bit 1 set == buffer copy-out performed (kfree the data after IO)
 */

int hfsplus_jbd_write_metadata_buffer(hfsplus_transaction_t *transaction,
				  struct hfsplus_jbd_head  *jh_in,
				  struct hfsplus_jbd_head **jh_out,
				  unsigned int blocknr)
{
	int need_copy_out = 0;
	int done_copy_out = 0;
	int do_escape = 0;
	char *mapped_data;
	struct buffer_head *new_bh;
	struct hfsplus_jbd_head *new_jh;
	struct page *new_page;
	unsigned int new_offset;
	struct buffer_head *bh_in = hfsplus_jh2bh(jh_in);
	hfsplus_jbd_t *journal = transaction->t_journal;

	/*
	 * The buffer really shouldn't be locked: only the current committing
	 * transaction is allowed to write it, so nobody else is allowed
	 * to do any IO.
	 *
	 * akpm: except if we're journalling data, and write() output is
	 * also part of a shared mapping, and another thread has
	 * decided to launch a writepage() against this buffer.
	 */
	HFSPLUS_J_ASSERT_BH(bh_in, buffer_hfsplus_jbddirty(bh_in));

	new_bh = alloc_buffer_head(GFP_NOFS|__GFP_NOFAIL);
	/* keep subsequent assertions sane */
	new_bh->b_state = 0;
	init_buffer(new_bh, NULL, NULL);
	atomic_set(&new_bh->b_count, 1);
	new_jh = hfsplus_jbd_add_journal_head(new_bh);	/* This sleeps */

	/*
	 * If a new transaction has already done a buffer copy-out, then
	 * we use that version of the data for the commit.
	 */
	hfsplus_jbd_lock_bh_state(bh_in);
repeat:
	if (jh_in->b_frozen_data) {
		done_copy_out = 1;
		new_page = virt_to_page(jh_in->b_frozen_data);
		new_offset = offset_in_page(jh_in->b_frozen_data);
	} else {
		new_page = hfsplus_jh2bh(jh_in)->b_page;
		new_offset = offset_in_page(hfsplus_jh2bh(jh_in)->b_data);
	}

	mapped_data = kmap_atomic(new_page);
	/*
	 * Check for escaping
	 */
	if (*((__be32 *)(mapped_data + new_offset)) ==
				cpu_to_be32(JFS_MAGIC_NUMBER)) {
		need_copy_out = 1;
		do_escape = 1;
	}
	kunmap_atomic(mapped_data);

	/*
	 * Do we need to do a data copy?
	 */
	if (need_copy_out && !done_copy_out) {
		char *tmp;

		hfsplus_jbd_unlock_bh_state(bh_in);
		tmp = hfsplus_jbd_alloc(bh_in->b_size, GFP_NOFS);
		hfsplus_jbd_lock_bh_state(bh_in);
		if (jh_in->b_frozen_data) {
			hfsplus_jbd_free(tmp, bh_in->b_size);
			goto repeat;
		}

		jh_in->b_frozen_data = tmp;
		mapped_data = kmap_atomic(new_page);
		memcpy(tmp, mapped_data + new_offset, hfsplus_jh2bh(jh_in)->b_size);
		kunmap_atomic(mapped_data);

		new_page = virt_to_page(tmp);
		new_offset = offset_in_page(tmp);
		done_copy_out = 1;
	}

	/*
	 * Did we need to do an escaping?  Now we've done all the
	 * copying, we can finally do so.
	 */
	if (do_escape) {
		mapped_data = kmap_atomic(new_page);
		*((unsigned int *)(mapped_data + new_offset)) = 0;
		kunmap_atomic(mapped_data);
	}

	set_bh_page(new_bh, new_page, new_offset);
	new_jh->b_transaction = NULL;
	new_bh->b_size = hfsplus_jh2bh(jh_in)->b_size;
	new_bh->b_bdev = transaction->t_journal->j_dev;
	new_bh->b_blocknr = blocknr;
	set_buffer_mapped(new_bh);
	set_buffer_dirty(new_bh);

	*jh_out = new_jh;

	/*
	 * The to-be-written buffer needs to get moved to the io queue,
	 * and the original buffer whose contents we are shadowing or
	 * copying is moved to the transaction's shadow queue.
	 */
	HFSPLUS_JBUFFER_TRACE(jh_in, "file as HFSPLUS_BJ_Shadow");
	spin_lock(&journal->j_list_lock);
	__hfsplus_jbd_file_buffer(jh_in, transaction, HFSPLUS_BJ_Shadow);
	spin_unlock(&journal->j_list_lock);
	hfsplus_jbd_unlock_bh_state(bh_in);

	HFSPLUS_JBUFFER_TRACE(new_jh, "file as HFSPLUS_BJ_IO");
	hfsplus_jbd_file_buffer(new_jh, transaction, HFSPLUS_BJ_IO);

	return do_escape | (done_copy_out << 1);
}

/*
 * Allocation code for the journal file.  Manage the space left in the
 * journal, so that we can begin checkpointing when appropriate.
 */

/*
 * __hfsplus__log_space_left: Return the number of free blocks left in the journal.
 *
 * Called with the journal already locked.
 *
 * Called under j_state_lock
 */

int __hfsplus__log_space_left(hfsplus_jbd_t *journal)
{
	int left = journal->j_free;

	assert_spin_locked(&journal->j_state_lock);

	/*
	 * Be pessimistic here about the number of those free blocks which
	 * might be required for log descriptor control blocks.
	 */

#define MIN_LOG_RESERVED_BLOCKS 32 /* Allow for rounding errors */

	left -= MIN_LOG_RESERVED_BLOCKS;

	if (left <= 0)
		return 0;
	left -= (left >> 3);
	return left;
}

/*
 * Called under j_state_lock.  Returns true if a transaction commit was started.
 */
int __hfsplus__log_start_commit(hfsplus_jbd_t *journal, hfsplus_jbd_tid_t target)
{
	/*
	 * The only transaction we can possibly wait upon is the
	 * currently running transaction (if it exists).  Otherwise,
	 * the target tid must be an old one.
	 */
	if (journal->j_running_transaction &&
	    journal->j_running_transaction->t_tid == target) {
		/*
		 * We want a new commit: OK, mark the request and wakeup the
		 * commit thread.  We do _not_ do the commit ourselves.
		 */

		journal->j_commit_request = target;
		hfsplus_jbd_debug(1, "JBD: requesting commit %d/%d\n",
			  journal->j_commit_request,
			  journal->j_commit_sequence);
		wake_up(&journal->j_wait_commit);
		return 1;
	} else if (!hfsplus_tid_geq(journal->j_commit_request, target))
		/* This should never happen, but if it does, preserve
		   the evidence before hfsplus_kjournald goes into a loop and
		   increments j_commit_sequence beyond all recognition. */
		WARN_ONCE(1, "jbd: bad hfsplus_log_start_commit: %u %u %u %u\n",
		    journal->j_commit_request, journal->j_commit_sequence,
		    target, journal->j_running_transaction ?
		    journal->j_running_transaction->t_tid : 0);
	return 0;
}

int hfsplus_log_start_commit(hfsplus_jbd_t *journal, hfsplus_jbd_tid_t tid)
{
	int ret;

	spin_lock(&journal->j_state_lock);
	ret = __hfsplus__log_start_commit(journal, tid);
	spin_unlock(&journal->j_state_lock);
	return ret;
}

/*
 * Force and wait upon a commit if the calling process is not within
 * transaction.  This is used for forcing out undo-protected data which contains
 * bitmaps, when the fs is running out of space.
 *
 * We can only force the running transaction if we don't have an active handle;
 * otherwise, we will deadlock.
 *
 * Returns true if a transaction was started.
 */
int hfsplus_jbd_force_commit_nested(hfsplus_jbd_t *journal)
{
	hfsplus_transaction_t *transaction = NULL;
	hfsplus_jbd_tid_t tid;

	spin_lock(&journal->j_state_lock);
	if (journal->j_running_transaction && !current->journal_info) {
		transaction = journal->j_running_transaction;
		__hfsplus__log_start_commit(journal, transaction->t_tid);
	} else if (journal->j_committing_transaction)
		transaction = journal->j_committing_transaction;

	if (!transaction) {
		spin_unlock(&journal->j_state_lock);
		return 0;	/* Nothing to retry */
	}

	tid = transaction->t_tid;
	spin_unlock(&journal->j_state_lock);
	hfsplus_jbd_log_wait_commit(journal, tid);
	return 1;
}

/*
 * Start a commit of the current running transaction (if any).  Returns true
 * if a transaction is going to be committed (or is currently already
 * committing), and fills its tid in at *ptid
 */
int hfsplus_jbd_start_commit(hfsplus_jbd_t *journal, hfsplus_jbd_tid_t *ptid)
{
	int ret = 0;

	spin_lock(&journal->j_state_lock);
	if (journal->j_running_transaction) {
		hfsplus_jbd_tid_t tid = journal->j_running_transaction->t_tid;

		ret = __hfsplus__log_start_commit(journal, tid);
		/* There's a running transaction and we've just made sure
		 * it's commit has been scheduled. */
		if (ret && ptid)
			*ptid = tid;

	} else if (journal->j_committing_transaction && ptid) {
		/*
		 * If ext3_write_super() recently started a commit, then we
		 * have to wait for completion of that transaction
		 */

			*ptid = journal->j_committing_transaction->t_tid;
		ret = 1;
	}
	spin_unlock(&journal->j_state_lock);
	return ret;
}

/*
 * Wait for a specified commit to complete.
 * The caller may not hold the journal lock.
 */
int hfsplus_jbd_log_wait_commit(hfsplus_jbd_t *journal, hfsplus_jbd_tid_t tid)
{
	int err = 0;

#ifdef CONFIG_JBD_DEBUG
	spin_lock(&journal->j_state_lock);
	if (!hfsplus_tid_geq(journal->j_commit_request, tid)) {
		printk(KERN_EMERG
		       "%s: error: j_commit_request=%d, tid=%d\n",
		       __func__, journal->j_commit_request, tid);
	}
	spin_unlock(&journal->j_state_lock);
#endif
	spin_lock(&journal->j_state_lock);
	while (hfsplus_tid_gt(tid, journal->j_commit_sequence)) {
		hfsplus_jbd_debug(1, "JBD: want %d, j_commit_sequence=%d\n",
				  tid, journal->j_commit_sequence);
		wake_up(&journal->j_wait_commit);
		spin_unlock(&journal->j_state_lock);
		wait_event(journal->j_wait_done_commit,
				!hfsplus_tid_gt(tid, journal->j_commit_sequence));
		spin_lock(&journal->j_state_lock);
	}
	spin_unlock(&journal->j_state_lock);

	if (unlikely(is_hfsplus_jbd_aborted(journal))) {
		printk(KERN_EMERG "journal commit I/O error\n");
		err = -EIO;
	}
	return err;
}

/*
 * Return 1 if a given transaction has not yet sent barrier request
 * connected with a transaction commit. If 0 is returned, transaction
 * may or may not have sent the barrier. Used to avoid sending barrier
 * twice in common cases.
 */
int hfsplus_jbd_trans_will_send_data_barrier(hfsplus_jbd_t *journal, hfsplus_jbd_tid_t tid)
{
	int ret = 0;
	hfsplus_transaction_t *commit_trans;

	if (!(journal->j_flags & JFS_BARRIER))
		return 0;
	spin_lock(&journal->j_state_lock);
	/* Transaction already committed? */
	if (hfsplus_tid_geq(journal->j_commit_sequence, tid))
		goto out;
	/*
	 * Transaction is being committed and we already proceeded to
	 * writing commit record?
	 */
	commit_trans = journal->j_committing_transaction;
	if (commit_trans && commit_trans->t_tid == tid &&
	    commit_trans->t_state >= HFSPLUS_T_COMMIT_RECORD)
		goto out;
	ret = 1;
out:
	spin_unlock(&journal->j_state_lock);
	return ret;
}
EXPORT_SYMBOL(hfsplus_jbd_trans_will_send_data_barrier);

/*
 * Log buffer allocation routines:
 */

int hfsplus_jbd_next_log_block(hfsplus_jbd_t *journal, unsigned int *retp)
{
	unsigned int blocknr;

	spin_lock(&journal->j_state_lock);
	HFSPLUS_J_ASSERT(journal->j_free > 1);

	blocknr = journal->j_head;
	journal->j_head++;
	journal->j_free--;
	if (journal->j_head == journal->j_last)
		journal->j_head = journal->j_first;
	spin_unlock(&journal->j_state_lock);
	return hfsplus_jbd_bmap(journal, blocknr, retp);
}

/*
 * Conversion of logical to physical block numbers for the journal
 *
 * On external journals the journal blocks are identity-mapped, so
 * this is a no-op.  If needed, we can use j_blk_offset - everything is
 * ready.
 */
int hfsplus_jbd_bmap(hfsplus_jbd_t *journal, unsigned int blocknr,
		 unsigned int *retp)
{
	int err = 0;
	unsigned int ret;

	if (journal->j_inode) {
		ret = bmap(journal->j_inode, blocknr);
		if (ret)
			*retp = ret;
		else {
			char b[BDEVNAME_SIZE];

			printk(KERN_ALERT "%s: journal block not found "
					"at offset %u on %s\n",
				__func__,
				blocknr,
				bdevname(journal->j_dev, b));
			err = -EIO;
			__hfsplus_jbd_abort_soft(journal, err);
		}
	} else {
		*retp = blocknr; /* +journal->j_blk_offset */
	}
	return err;
}

/*
 * We play buffer_head aliasing tricks to write data/metadata blocks to
 * the journal without copying their contents, but for journal
 * descriptor blocks we do need to generate bona fide buffers.
 *
 * After the caller of hfsplus_jbd_get_descriptor_buffer() has finished modifying
 * the buffer's contents they really should run flush_dcache_page(bh->b_page).
 * But we don't bother doing that, so there will be coherency problems with
 * mmaps of blockdevs which hold live JBD-controlled filesystems.
 */
struct hfsplus_jbd_head *hfsplus_jbd_get_descriptor_buffer(hfsplus_jbd_t *journal)
{
	struct buffer_head *bh;
	unsigned int blocknr;
	int err;

	err = hfsplus_jbd_next_log_block(journal, &blocknr);

	if (err)
		return NULL;

	bh = __getblk(journal->j_dev, blocknr, journal->j_blocksize);
	if (!bh)
		return NULL;
	lock_buffer(bh);
	memset(bh->b_data, 0, journal->j_blocksize);
	set_buffer_uptodate(bh);
	unlock_buffer(bh);
	HFSPLUS_BUFFER_TRACE(bh, "return this buffer");
	return hfsplus_jbd_add_journal_head(bh);
}

/*
 * Management for journal control blocks: functions to create and
 * destroy hfsplus_jbd_t structures, and to initialise and read existing
 * journal blocks from disk.  */

/* First: create and setup a hfsplus_jbd_t object in memory.  We initialise
 * very few fields yet: that has to wait until we have created the
 * journal structures from from scratch, or loaded them from disk. */

static hfsplus_jbd_t * hfsplus_jbd_init_common (void)
{
	hfsplus_jbd_t *journal;
	int err;

	journal = kzalloc(sizeof(*journal), GFP_KERNEL);
	if (!journal)
		goto fail;

	init_waitqueue_head(&journal->j_wait_transaction_locked);
	init_waitqueue_head(&journal->j_wait_logspace);
	init_waitqueue_head(&journal->j_wait_done_commit);
	init_waitqueue_head(&journal->j_wait_checkpoint);
	init_waitqueue_head(&journal->j_wait_commit);
	init_waitqueue_head(&journal->j_wait_updates);
	mutex_init(&journal->j_checkpoint_mutex);
	spin_lock_init(&journal->j_revoke_lock);
	spin_lock_init(&journal->j_list_lock);
	spin_lock_init(&journal->j_state_lock);

	journal->j_commit_interval = (HZ * HFSPLUS_JBD_DEFAULT_MAX_COMMIT_AGE);

	/* The journal is marked for error until we succeed with recovery! */
	journal->j_flags = JFS_ABORT;

	/* Set up a default-sized revoke table for the new mount. */
	err = hfsplus_jbd_init_revoke(journal, HFSPLUS_JBD_REVOKE_DEFAULT_HASH);
	if (err) {
		kfree(journal);
		goto fail;
	}
	return journal;
fail:
	return NULL;
}

/* hfsplus_jbd_init_dev and hfsplus_jbd_init_inode:
 *
 * Create a journal structure assigned some fixed set of disk blocks to
 * the journal.  We don't actually touch those disk blocks yet, but we
 * need to set up all of the mapping information to tell the journaling
 * system where the journal blocks are.
 *
 */

/**
 *  hfsplus_jbd_t * hfsplus_jbd_init_dev() - creates and initialises a journal structure
 *  @bdev: Block device on which to create the journal
 *  @fs_dev: Device which hold journalled filesystem for this journal.
 *  @start: Block nr Start of journal.
 *  @len:  Length of the journal in blocks.
 *  @blocksize: blocksize of journalling device
 *
 *  Returns: a newly created hfsplus_jbd_t *
 *
 *  hfsplus_jbd_init_dev creates a journal which maps a fixed contiguous
 *  range of blocks on an arbitrary block device.
 *
 */
hfsplus_jbd_t * hfsplus_jbd_init_dev(struct block_device *bdev,
			struct block_device *fs_dev,
			int start, int len, int blocksize)
{
	hfsplus_jbd_t *journal = hfsplus_jbd_init_common();
	struct buffer_head *bh;
	int n;

	if (!journal)
		return NULL;

	journal->j_dev = bdev;
	journal->j_fs_dev = fs_dev;
	journal->j_blk_offset = start;
	journal->j_maxlen = len;
	journal->j_blocksize = blocksize;

	bh = __getblk(journal->j_dev, start, journal->j_blocksize);
	HFSPLUS_J_ASSERT(bh != NULL);
	journal->j_sb_buffer = bh;
	journal->j_superblock = (hfsplus_jbd_superblock_t *)bh->b_data;

	/* journal descriptor can store up to n blocks -bzzz */
	n = (journal->j_blocksize - HFSPLUS_SECTOR_SIZE) / sizeof(hfsplus_jbd_block_tag_t);
	journal->j_wbufsize = n;
	journal->j_wbuf = kmalloc(n * sizeof(struct buffer_head*), GFP_KERNEL);
	if (!journal->j_wbuf) {
		printk(KERN_ERR "%s: Can't allocate bhs for commit thread\n",
			__func__);
		kfree(journal);
		journal = NULL;
	}

	return journal;
}

/**
 *  hfsplus_jbd_t * hfsplus_jbd_init_inode () - creates a journal which maps to a inode.
 *  @inode: An inode to create the journal in
 *
 * hfsplus_jbd_init_inode creates a journal which maps an on-disk inode as
 * the journal.  The inode must exist already, must support bmap() and
 * must have all data blocks preallocated.
 */
hfsplus_jbd_t * hfsplus_jbd_init_inode (struct inode *inode)
{
	struct buffer_head *bh;
	hfsplus_jbd_t *journal = hfsplus_jbd_init_common();
	int err;
	int n;
	unsigned int blocknr;

	if (!journal)
		return NULL;

	journal->j_dev = journal->j_fs_dev = inode->i_sb->s_bdev;
	journal->j_inode = inode;
	hfsplus_jbd_debug(1,
		  "journal %p: inode %s/%ld, size %Ld, bits %d, blksize %ld\n",
		  journal, inode->i_sb->s_id, inode->i_ino,
		  (long long) inode->i_size,
		  inode->i_sb->s_blocksize_bits, inode->i_sb->s_blocksize);

	journal->j_maxlen = inode->i_size >> inode->i_sb->s_blocksize_bits;
	journal->j_blocksize = inode->i_sb->s_blocksize;

	/* journal descriptor can store up to n blocks -bzzz */
	n = journal->j_blocksize / sizeof(hfsplus_jbd_block_tag_t);
	journal->j_wbufsize = n;
	journal->j_wbuf = kmalloc(n * sizeof(struct buffer_head*), GFP_KERNEL);
	if (!journal->j_wbuf) {
		printk(KERN_ERR "%s: Can't allocate bhs for commit thread\n",
			__func__);
		kfree(journal);
		return NULL;
	}

	err = hfsplus_jbd_bmap(journal, 0, &blocknr);
	/* If that failed, give up */
	if (err) {
		printk(KERN_ERR "%s: Cannot locate journal superblock\n",
		       __func__);
		kfree(journal);
		return NULL;
	}

	bh = __getblk(journal->j_dev, blocknr, journal->j_blocksize);
    HFSPLUS_J_ASSERT(bh != NULL);

	journal->j_sb_buffer = bh;
	journal->j_superblock = (hfsplus_jbd_superblock_t *)bh->b_data;

	return journal;
}

/*
 * If the journal init or create aborts, we need to mark the journal
 * superblock as being NULL to prevent the journal destroy from writing
 * back a bogus superblock.
 */
static void hfsplus_jbd_fail_superblock (hfsplus_jbd_t *journal)
{
	struct buffer_head *bh = journal->j_sb_buffer;
	brelse(bh);
	journal->j_sb_buffer = NULL;
}

/*
 * Given a hfsplus_jbd_t structure, initialise the various fields for
 * startup of a new journaling session.  We use this both when creating
 * a journal, and after recovering an old journal to reset it for
 * subsequent use.
 */

static int hfsplus_jbd_reset(hfsplus_jbd_t *journal)
{
	hfsplus_jbd_superblock_t *sb = journal->j_superblock;
	unsigned int first, last;

	first = be32_to_cpu(sb->s_first);
	last = be32_to_cpu(sb->s_maxlen);
	if (first + JFS_MIN_JOURNAL_BLOCKS > last + 1) {
		printk(KERN_ERR "JBD: Journal too short (blocks %u-%u).\n",
		       first, last);
		hfsplus_jbd_fail_superblock(journal);
		return -EINVAL;
	}

	journal->j_first = first;
	journal->j_last = last;

	journal->j_head = first;
	journal->j_tail = first;
	journal->j_free = last - first;

	journal->j_tail_sequence = journal->j_transaction_sequence;
	journal->j_commit_sequence = journal->j_transaction_sequence - 1;
	journal->j_commit_request = journal->j_commit_sequence;

	journal->j_max_transaction_buffers = journal->j_maxlen / 4;

	/* Add the dynamic fields and write it to disk. */
	hfsplus_jbd_update_superblock(journal, 1);
	return hfsplus_jbd_start_thread(journal);
}

/**
 * int hfsplus_jbd_create() - Initialise the new journal file
 * @journal: Journal to create. This structure must have been initialised
 *
 * Given a hfsplus_jbd_t structure which tells us which disk blocks we can
 * use, create a new journal superblock and initialise all of the
 * journal fields from scratch.
 **/
int hfsplus_jbd_create(hfsplus_jbd_t *journal)
{
	unsigned int blocknr;
	struct buffer_head *bh;
	hfsplus_jbd_superblock_t *sb;
	int i, err;

	if (journal->j_maxlen < JFS_MIN_JOURNAL_BLOCKS) {
		printk (KERN_ERR "Journal length (%d blocks) too short.\n",
			journal->j_maxlen);
		hfsplus_jbd_fail_superblock(journal);
		return -EINVAL;
	}

	if (journal->j_inode == NULL) {
		/*
		 * We don't know what block to start at!
		 */
		printk(KERN_EMERG
		       "%s: creation of journal on external device!\n",
		       __func__);
		BUG();
	}

	/* Zero out the entire journal on disk.  We cannot afford to
	   have any blocks on disk beginning with JFS_MAGIC_NUMBER. */
	hfsplus_jbd_debug(1, "JBD: Zeroing out journal blocks...\n");
	for (i = 0; i < journal->j_maxlen; i++) {
		err = hfsplus_jbd_bmap(journal, i, &blocknr);
		if (err)
			return err;
		bh = __getblk(journal->j_dev, blocknr, journal->j_blocksize);
		if (unlikely(!bh))
			return -ENOMEM;
		lock_buffer(bh);
		memset (bh->b_data, 0, journal->j_blocksize);
		HFSPLUS_BUFFER_TRACE(bh, "marking dirty");
		mark_buffer_dirty(bh);
		HFSPLUS_BUFFER_TRACE(bh, "marking uptodate");
		set_buffer_uptodate(bh);
		unlock_buffer(bh);
		__brelse(bh);
	}

	sync_blockdev(journal->j_dev);
	hfsplus_jbd_debug(1, "JBD: journal cleared.\n");

	/* OK, fill in the initial static fields in the new superblock */
	sb = journal->j_superblock;

	sb->s_header.h_magic	 = cpu_to_be32(JFS_MAGIC_NUMBER);
	sb->s_header.h_blocktype = cpu_to_be32(JFS_SUPERBLOCK_V2);

	sb->s_blocksize	= cpu_to_be32(journal->j_blocksize);
	sb->s_maxlen	= cpu_to_be32(journal->j_maxlen);
	sb->s_first	= cpu_to_be32(1);

	journal->j_transaction_sequence = 1;

	journal->j_flags &= ~JFS_ABORT;
	journal->j_format_version = 2;

	return hfsplus_jbd_reset(journal);
}

/**
 * void hfsplus_jbd_update_superblock() - Update journal sb on disk.
 * @journal: The journal to update.
 * @wait: Set to '0' if you don't want to wait for IO completion.
 *
 * Update a journal's dynamic superblock fields and write it to disk,
 * optionally waiting for the IO to complete.
 */
void hfsplus_jbd_update_superblock(hfsplus_jbd_t *journal, int wait)
{
	hfsplus_jbd_superblock_t *sb = journal->j_superblock;
	struct buffer_head *bh = journal->j_sb_buffer;

	/*
	 * As a special case, if the on-disk copy is already marked as needing
	 * no recovery (s_start == 0) and there are no outstanding transactions
	 * in the filesystem, then we can safely defer the superblock update
	 * until the next commit by setting JFS_FLUSHED.  This avoids
	 * attempting a write to a potential-readonly device.
	 */
	if (sb->s_start == 0 && journal->j_tail_sequence ==
				journal->j_transaction_sequence) {
		hfsplus_jbd_debug(1,"JBD: Skipping superblock update on recovered sb "
			"(start %u, seq %d, errno %d)\n",
			journal->j_tail, journal->j_tail_sequence,
			journal->j_errno);
		goto out;
	}

	if (buffer_write_io_error(bh)) {
		char b[BDEVNAME_SIZE];
		/*
		 * Oh, dear.  A previous attempt to write the journal
		 * superblock failed.  This could happen because the
		 * USB device was yanked out.  Or it could happen to
		 * be a transient write error and maybe the block will
		 * be remapped.  Nothing we can do but to retry the
		 * write and hope for the best.
		 */
		printk(KERN_ERR "JBD: previous I/O error detected "
		       "for journal superblock update for %s.\n",
		       hfsplus_jbd_dev_name(journal, b));
		clear_buffer_write_io_error(bh);
		set_buffer_uptodate(bh);
	}

	spin_lock(&journal->j_state_lock);
	hfsplus_jbd_debug(1,"JBD: updating superblock (start %u, seq %d, errno %d)\n",
		  journal->j_tail, journal->j_tail_sequence, journal->j_errno);

	sb->s_sequence = cpu_to_be32(journal->j_tail_sequence);
	sb->s_start    = cpu_to_be32(journal->j_tail);
	sb->s_errno    = cpu_to_be32(journal->j_errno);
    hfsplus_jhdr_checksum_calculate(journal);
	spin_unlock(&journal->j_state_lock);

	HFSPLUS_BUFFER_TRACE(bh, "marking dirty");
	mark_buffer_dirty(bh);
	if (wait) {
		sync_dirty_buffer(bh);
		if (buffer_write_io_error(bh)) {
			char b[BDEVNAME_SIZE];
			printk(KERN_ERR "JBD: I/O error detected "
			       "when updating journal superblock for %s.\n",
			       hfsplus_jbd_dev_name(journal, b));
			clear_buffer_write_io_error(bh);
			set_buffer_uptodate(bh);
		}
	} else
		write_dirty_buffer(bh, WRITE);

	//trace_jbd_update_superblock_end(journal, wait);
out:
	/* If we have just flushed the log (by marking s_start==0), then
	 * any future commit will have to be careful to update the
	 * superblock again to re-record the true start of the log. */

	spin_lock(&journal->j_state_lock);
	if (sb->s_start)
		journal->j_flags &= ~JFS_FLUSHED;
	else
		journal->j_flags |= JFS_FLUSHED;
	spin_unlock(&journal->j_state_lock);
}

/*
 * Read the superblock for a given journal, performing initial
 * validation of the format.
 */

static int hfsplus_jbd_get_superblock(hfsplus_jbd_t *journal)
{
	struct buffer_head *bh;
	hfsplus_jbd_superblock_t *sb;
	int err = -EIO;

	bh = journal->j_sb_buffer;

	HFSPLUS_J_ASSERT(bh != NULL);
	if (!buffer_uptodate(bh)) {
		ll_rw_block(READ, 1, &bh);
		wait_on_buffer(bh);
		if (!buffer_uptodate(bh)) {
			printk (KERN_ERR
				"JBD: IO error reading journal superblock\n");
			goto out;
		}
	}

	sb = journal->j_superblock;

	err = -EINVAL;

	if (sb->s_header.h_magic != cpu_to_be32(JFS_MAGIC_NUMBER) ||
	    sb->s_blocksize != cpu_to_be32(journal->j_blocksize)) {
		printk(KERN_WARNING "JBD: no valid journal superblock found\n");
		goto out;
	}

	switch(be32_to_cpu(sb->s_header.h_blocktype)) {
	case JFS_SUPERBLOCK_V1:
		journal->j_format_version = 1;
		break;
	case JFS_SUPERBLOCK_V2:
		journal->j_format_version = 2;
		break;
	default:
		printk(KERN_WARNING "JBD: unrecognised superblock format ID\n");
		goto out;
	}

	if (be32_to_cpu(sb->s_maxlen) < journal->j_maxlen)
		journal->j_maxlen = be32_to_cpu(sb->s_maxlen);
	else if (be32_to_cpu(sb->s_maxlen) > journal->j_maxlen) {
		printk (KERN_WARNING "JBD: journal file too short\n");
		goto out;
	}

	if (be32_to_cpu(sb->s_first) == 0 ||
	    be32_to_cpu(sb->s_first) >= journal->j_maxlen) {
		printk(KERN_WARNING
			"JBD: Invalid start block of journal: %u\n",
			be32_to_cpu(sb->s_first));
		goto out;
	}

	return 0;

out:
	hfsplus_jbd_fail_superblock(journal);
	return err;
}

/*
 * Load the on-disk journal superblock and read the key fields into the
 * hfsplus_jbd_t.
 */

static int load_superblock(hfsplus_jbd_t *journal)
{
	int err;
	hfsplus_jbd_superblock_t *sb;

	err = hfsplus_jbd_get_superblock(journal);
	if (err)
		return err;

	sb = journal->j_superblock;

	journal->j_tail_sequence = be32_to_cpu(sb->s_sequence);
	journal->j_tail = be32_to_cpu(sb->s_start);
	journal->j_first = be32_to_cpu(sb->s_first);
	journal->j_last = be32_to_cpu(sb->s_maxlen);
	journal->j_errno = be32_to_cpu(sb->s_errno);

	return 0;
}


/**
 * int hfsplus_jbd_load() - Read journal from disk.
 * @journal: Journal to act on.
 *
 * Given a hfsplus_jbd_t structure which tells us which disk blocks contain
 * a journal, read the journal from disk to initialise the in-memory
 * structures.
 */
int hfsplus_jbd_load(hfsplus_jbd_t *journal)
{
	int err;
	hfsplus_jbd_superblock_t *sb;

	err = load_superblock(journal);
	if (err)
		return err;

	sb = journal->j_superblock;
	/* If this is a V2 superblock, then we have to check the
	 * features flags on it. */

	if (journal->j_format_version >= 2) {
        hfsplus_jbd_superblock_t *sb = journal->j_superblock;
		if ((sb->s_feature_ro_compat &
		     ~cpu_to_be32(JFS_KNOWN_ROCOMPAT_FEATURES)) ||
		    (sb->s_feature_incompat &
		     ~cpu_to_be32(JFS_KNOWN_INCOMPAT_FEATURES))) {
			printk (KERN_WARNING
				"JBD: Unrecognised features on journal\n");
			return -EINVAL;
		}
	}

	/* Let the recovery code check whether it needs to recover any
	 * data from the journal. */
	if (hfsplus_jbd_recover(journal))
		goto recovery_error;

	/* OK, we've finished with the dynamic journal bits:
	 * reinitialise the dynamic contents of the superblock in memory
	 * and reset them on disk. */
	if (hfsplus_jbd_reset(journal))
		goto recovery_error;

	journal->j_flags &= ~JFS_ABORT;
	journal->j_flags |= JFS_LOADED;
	return 0;

recovery_error:
	printk (KERN_WARNING "JBD: recovery failed\n");
	return -EIO;
}

/**
 * void hfsplus_jbd_destroy() - Release a hfsplus_jbd_t structure.
 * @journal: Journal to act on.
 *
 * Release a hfsplus_jbd_t structure once it is no longer in use by the
 * journaled object.
 * Return <0 if we couldn't clean up the journal.
 */
int hfsplus_jbd_destroy(hfsplus_jbd_t *journal)
{
	int err = 0;

	
	/* Wait for the commit thread to wake up and die. */
	hfsplus_jbd_kill_thread(journal);

	/* Force a final log commit */
	if (journal->j_running_transaction)
		hfsplus_jbd_commit_transaction(journal);

	/* Force any old transactions to disk */

	/* Totally anal locking here... */
	spin_lock(&journal->j_list_lock);
	while (journal->j_checkpoint_transactions != NULL) {
		spin_unlock(&journal->j_list_lock);
		hfsplus_jbd_log_do_checkpoint(journal);
		spin_lock(&journal->j_list_lock);
	}

	HFSPLUS_J_ASSERT(journal->j_running_transaction == NULL);
	HFSPLUS_J_ASSERT(journal->j_committing_transaction == NULL);
	HFSPLUS_J_ASSERT(journal->j_checkpoint_transactions == NULL);
	spin_unlock(&journal->j_list_lock);

	/* We can now mark the journal as empty. */
	journal->j_tail = 0;
	journal->j_tail_sequence = ++journal->j_transaction_sequence;
#ifdef HFSPLUS_JOURNAL_MAC_COMPATIBLE
	hfsplus_journal_mark_journal_empty(journal);
#endif

	if (journal->j_sb_buffer) {
		if (!is_hfsplus_jbd_aborted(journal)) {
			/* We can now mark the journal as empty. */
			journal->j_tail = 0;
			journal->j_tail_sequence =
				++journal->j_transaction_sequence;
			hfsplus_jbd_update_superblock(journal, 1);
		} else {
			err = -EIO;
		}
		brelse(journal->j_sb_buffer);
	}

	if (journal->j_inode)
		iput(journal->j_inode);
	if (journal->j_revoke)
		hfsplus_jbd_destroy_revoke(journal);
	kfree(journal->j_wbuf);
	kfree(journal);

	return err;
}


/**
 *int hfsplus_jbd_check_used_features () - Check if features specified are used.
 * @journal: Journal to check.
 * @compat: bitmask of compatible features
 * @ro: bitmask of features that force read-only mount
 * @incompat: bitmask of incompatible features
 *
 * Check whether the journal uses all of a given set of
 * features.  Return true (non-zero) if it does.
 **/

int hfsplus_jbd_check_used_features (hfsplus_jbd_t *journal, unsigned long compat,
				 unsigned long ro, unsigned long incompat)
{
	hfsplus_jbd_superblock_t *sb;

	if (!compat && !ro && !incompat)
		return 1;
	if (journal->j_format_version == 1)
		return 0;

	sb = journal->j_superblock;

	if (((be32_to_cpu(sb->s_feature_compat) & compat) == compat) &&
	    ((be32_to_cpu(sb->s_feature_ro_compat) & ro) == ro) &&
	    ((be32_to_cpu(sb->s_feature_incompat) & incompat) == incompat))
		return 1;

	return 0;
}

/**
 * int hfsplus_jbd_check_available_features() - Check feature set in journalling layer
 * @journal: Journal to check.
 * @compat: bitmask of compatible features
 * @ro: bitmask of features that force read-only mount
 * @incompat: bitmask of incompatible features
 *
 * Check whether the journaling code supports the use of
 * all of a given set of features on this journal.  Return true
 * (non-zero) if it can. */

int hfsplus_jbd_check_available_features (hfsplus_jbd_t *journal, unsigned long compat,
				      unsigned long ro, unsigned long incompat)
{
	if (!compat && !ro && !incompat)
		return 1;

	/* We can support any known requested features iff the
	 * superblock is in version 2.  Otherwise we fail to support any
	 * extended sb features. */

	if (journal->j_format_version != 2)
		return 0;

	if ((compat   & JFS_KNOWN_COMPAT_FEATURES) == compat &&
	    (ro       & JFS_KNOWN_ROCOMPAT_FEATURES) == ro &&
	    (incompat & JFS_KNOWN_INCOMPAT_FEATURES) == incompat)
		return 1;

	return 0;
}

/**
 * int hfsplus_jbd_set_features () - Mark a given journal feature in the superblock
 * @journal: Journal to act on.
 * @compat: bitmask of compatible features
 * @ro: bitmask of features that force read-only mount
 * @incompat: bitmask of incompatible features
 *
 * Mark a given journal feature as present on the
 * superblock.  Returns true if the requested features could be set.
 *
 */

int hfsplus_jbd_set_features (hfsplus_jbd_t *journal, unsigned long compat,
			  unsigned long ro, unsigned long incompat)
{
	hfsplus_jbd_superblock_t *sb;

	if (hfsplus_jbd_check_used_features(journal, compat, ro, incompat))
		return 1;

	if (!hfsplus_jbd_check_available_features(journal, compat, ro, incompat))
		return 0;

	hfsplus_jbd_debug(1, "Setting new features 0x%lx/0x%lx/0x%lx\n",
		  compat, ro, incompat);

	sb = journal->j_superblock;

	sb->s_feature_compat    |= cpu_to_be32(compat);
	sb->s_feature_ro_compat |= cpu_to_be32(ro);
	sb->s_feature_incompat  |= cpu_to_be32(incompat);

	return 1;
}


/**
 * int hfsplus_jbd_update_format () - Update on-disk journal structure.
 * @journal: Journal to act on.
 *
 * Given an initialised but unloaded journal struct, poke about in the
 * on-disk structure to update it to the most recent supported version.
 */
int hfsplus_jbd_update_format (hfsplus_jbd_t *journal)
{
	hfsplus_jbd_superblock_t *sb;
	int err;

	err = hfsplus_jbd_get_superblock(journal);
	if (err)
		return err;

	sb = journal->j_superblock;

	switch (be32_to_cpu(sb->s_header.h_blocktype)) {
	case JFS_SUPERBLOCK_V2:
		return 0;
	case JFS_SUPERBLOCK_V1:
		return hfsplus_jbd_convert_superblock_v1(journal, sb);
	default:
		break;
	}
	return -EINVAL;
}

static int hfsplus_jbd_convert_superblock_v1(hfsplus_jbd_t *journal,
					 hfsplus_jbd_superblock_t *sb)
{
	int offset, blocksize;
	struct buffer_head *bh;

	printk(KERN_WARNING
		"JBD: Converting superblock from version 1 to 2.\n");

	/* Pre-initialise new fields to zero */
	offset = ((char *) &(sb->s_feature_compat)) - ((char *) sb);
	blocksize = be32_to_cpu(sb->s_blocksize);
	memset(&sb->s_feature_compat, 0, blocksize-offset);

	sb->s_nr_users = cpu_to_be32(1);
	sb->s_header.h_blocktype = cpu_to_be32(JFS_SUPERBLOCK_V2);
	journal->j_format_version = 2;

	bh = journal->j_sb_buffer;
	HFSPLUS_BUFFER_TRACE(bh, "marking dirty");
	mark_buffer_dirty(bh);
	sync_dirty_buffer(bh);
	return 0;
}


/**
 * int hfsplus_jbd_flush () - Flush journal
 * @journal: Journal to act on.
 *
 * Flush all data for a given journal to disk and empty the journal.
 * Filesystems can use this when remounting readonly to ensure that
 * recovery does not need to happen on remount.
 */

int hfsplus_jbd_flush(hfsplus_jbd_t *journal)
{
	int err = 0;
	hfsplus_transaction_t *transaction = NULL;
	unsigned int old_tail;

	spin_lock(&journal->j_state_lock);

	/* Force everything buffered to the log... */
	if (journal->j_running_transaction) {
		transaction = journal->j_running_transaction;
		__hfsplus__log_start_commit(journal, transaction->t_tid);
	} else if (journal->j_committing_transaction)
		transaction = journal->j_committing_transaction;

	/* Wait for the log commit to complete... */
	if (transaction) {
		hfsplus_jbd_tid_t tid = transaction->t_tid;

		spin_unlock(&journal->j_state_lock);
		hfsplus_jbd_log_wait_commit(journal, tid);
	} else {
		spin_unlock(&journal->j_state_lock);
	}

	/* ...and flush everything in the log out to disk. */
	spin_lock(&journal->j_list_lock);
	while (!err && journal->j_checkpoint_transactions != NULL) {
		spin_unlock(&journal->j_list_lock);
		mutex_lock(&journal->j_checkpoint_mutex);
		err = hfsplus_jbd_log_do_checkpoint(journal);
		mutex_unlock(&journal->j_checkpoint_mutex);
		spin_lock(&journal->j_list_lock);
	}
	spin_unlock(&journal->j_list_lock);

	if (is_hfsplus_jbd_aborted(journal))
		return -EIO;

	cleanup_hfsplus_jbd_tail(journal);

	/* Finally, mark the journal as really needing no recovery.
	 * This sets s_start==0 in the underlying superblock, which is
	 * the magic code for a fully-recovered superblock.  Any future
	 * commits of data to the journal will restore the current
	 * s_start value. */
	spin_lock(&journal->j_state_lock);
	old_tail = journal->j_tail;
	journal->j_tail = 0;
	spin_unlock(&journal->j_state_lock);
	hfsplus_jbd_update_superblock(journal, 1);
	spin_lock(&journal->j_state_lock);
	journal->j_tail = old_tail;

	HFSPLUS_J_ASSERT(!journal->j_running_transaction);
	HFSPLUS_J_ASSERT(!journal->j_committing_transaction);
	HFSPLUS_J_ASSERT(!journal->j_checkpoint_transactions);
	HFSPLUS_J_ASSERT(journal->j_head == journal->j_tail);
	HFSPLUS_J_ASSERT(journal->j_tail_sequence == journal->j_transaction_sequence);
	spin_unlock(&journal->j_state_lock);
	return 0;
}

/**
 * int hfsplus_jbd_wipe() - Wipe journal contents
 * @journal: Journal to act on.
 * @write: flag (see below)
 *
 * Wipe out all of the contents of a journal, safely.  This will produce
 * a warning if the journal contains any valid recovery information.
 * Must be called between hfsplus_jbd_init_*() and hfsplus_jbd_load().
 *
 * If 'write' is non-zero, then we wipe out the journal on disk; otherwise
 * we merely suppress recovery.
 */

int hfsplus_jbd_wipe(hfsplus_jbd_t *journal, int write)
{
	int err = 0;

	HFSPLUS_J_ASSERT (!(journal->j_flags & JFS_LOADED));

	err = load_superblock(journal);
	if (err)
		return err;

	if (!journal->j_tail)
		goto no_recovery;

	printk (KERN_WARNING "JBD: %s recovery information on journal\n",
		write ? "Clearing" : "Ignoring");

	err = hfsplus_jbd_skip_recovery(journal);
	if (write)
		hfsplus_jbd_update_superblock(journal, 1);

 no_recovery:
	return err;
}

/*
 * hfsplus_jbd_dev_name: format a character string to describe on what
 * device this journal is present.
 */

static const char *hfsplus_jbd_dev_name(hfsplus_jbd_t *journal, char *buffer)
{
	struct block_device *bdev;

	if (journal->j_inode)
		bdev = journal->j_inode->i_sb->s_bdev;
	else
		bdev = journal->j_dev;

	return bdevname(bdev, buffer);
}

/*
 * Journal abort has very specific semantics, which we describe
 * for journal abort.
 *
 * Two internal function, which provide abort to te jbd layer
 * itself are here.
 */

/*
 * Quick version for internal journal use (doesn't lock the journal).
 * Aborts hard --- we mark the abort as occurred, but do _nothing_ else,
 * and don't attempt to make any other journal updates.
 */
void __hfsplus_jbd_abort_hard(hfsplus_jbd_t *journal)
{
	hfsplus_transaction_t *transaction;
	char b[BDEVNAME_SIZE];

	if (journal->j_flags & JFS_ABORT)
		return;

	printk(KERN_ERR "Aborting journal on device %s.\n",
		hfsplus_jbd_dev_name(journal, b));

	spin_lock(&journal->j_state_lock);
	journal->j_flags |= JFS_ABORT;
	transaction = journal->j_running_transaction;
	if (transaction)
		__hfsplus__log_start_commit(journal, transaction->t_tid);
	spin_unlock(&journal->j_state_lock);
}

/* Soft abort: record the abort error status in the journal superblock,
 * but don't do any other IO. */
static void __hfsplus_jbd_abort_soft (hfsplus_jbd_t *journal, int errno)
{
	if (journal->j_flags & JFS_ABORT)
		return;

	if (!journal->j_errno)
		journal->j_errno = errno;

	__hfsplus_jbd_abort_hard(journal);

	if (errno)
		hfsplus_jbd_update_superblock(journal, 1);
}

/**
 * void hfsplus_jbd_abort () - Shutdown the journal immediately.
 * @journal: the journal to shutdown.
 * @errno:   an error number to record in the journal indicating
 *           the reason for the shutdown.
 *
 * Perform a complete, immediate shutdown of the ENTIRE
 * journal (not of a single transaction).  This operation cannot be
 * undone without closing and reopening the journal.
 *
 * The hfsplus_jbd_abort function is intended to support higher level error
 * recovery mechanisms such as the ext2/ext3 remount-readonly error
 * mode.
 *
 * Journal abort has very specific semantics.  Any existing dirty,
 * unjournaled buffers in the main filesystem will still be written to
 * disk by bdflush, but the journaling mechanism will be suspended
 * immediately and no further transaction commits will be honoured.
 *
 * Any dirty, journaled buffers will be written back to disk without
 * hitting the journal.  Atomicity cannot be guaranteed on an aborted
 * filesystem, but we _do_ attempt to leave as much data as possible
 * behind for fsck to use for cleanup.
 *
 * Any attempt to get a new transaction handle on a journal which is in
 * ABORT state will just result in an -EROFS error return.  A
 * journal_stop on an existing handle will return -EIO if we have
 * entered abort state during the update.
 *
 * Recursive transactions are not disturbed by journal abort until the
 * final journal_stop, which will receive the -EIO error.
 *
 * Finally, the hfsplus_jbd_abort call allows the caller to supply an errno
 * which will be recorded (if possible) in the journal superblock.  This
 * allows a client to record failure conditions in the middle of a
 * transaction without having to complete the transaction to record the
 * failure to disk.  ext3_error, for example, now uses this
 * functionality.
 *
 * Errors which originate from within the journaling layer will NOT
 * supply an errno; a null errno implies that absolutely no further
 * writes are done to the journal (unless there are any already in
 * progress).
 *
 */

void hfsplus_jbd_abort(hfsplus_jbd_t *journal, int errno)
{
	__hfsplus_jbd_abort_soft(journal, errno);
}

/**
 * int hfsplus_jbd_errno () - returns the journal's error state.
 * @journal: journal to examine.
 *
 * This is the errno numbet set with hfsplus_jbd_abort(), the last
 * time the journal was mounted - if the journal was stopped
 * without calling abort this will be 0.
 *
 * If the journal has been aborted on this mount time -EROFS will
 * be returned.
 */
int hfsplus_jbd_errno(hfsplus_jbd_t *journal)
{
	int err;

	spin_lock(&journal->j_state_lock);
	if (journal->j_flags & JFS_ABORT)
		err = -EROFS;
	else
		err = journal->j_errno;
	spin_unlock(&journal->j_state_lock);
	return err;
}

/**
 * int hfsplus_jbd_clear_err () - clears the journal's error state
 * @journal: journal to act on.
 *
 * An error must be cleared or Acked to take a FS out of readonly
 * mode.
 */
int hfsplus_jbd_clear_err(hfsplus_jbd_t *journal)
{
	int err = 0;

	spin_lock(&journal->j_state_lock);
	if (journal->j_flags & JFS_ABORT)
		err = -EROFS;
	else
		journal->j_errno = 0;
	spin_unlock(&journal->j_state_lock);
	return err;
}

/**
 * void hfsplus_jbd_ack_err() - Ack journal err.
 * @journal: journal to act on.
 *
 * An error must be cleared or Acked to take a FS out of readonly
 * mode.
 */
void hfsplus_jbd_ack_err(hfsplus_jbd_t *journal)
{
	spin_lock(&journal->j_state_lock);
	if (journal->j_errno)
		journal->j_flags |= JFS_ACK_ERR;
	spin_unlock(&journal->j_state_lock);
}

int hfsplus_jbd_blocks_per_page(struct inode *inode)
{
	return 1 << (PAGE_CACHE_SHIFT - inode->i_sb->s_blocksize_bits);
}

/*
 * Simple support for retrying memory allocations.  Introduced to help to
 * debug different VM deadlock avoidance strategies. 
 */
void * __hfsplus_jbd_kmalloc (const char *where, size_t size, gfp_t flags, int retry)
{
	return kmalloc(size, flags | (retry ? __GFP_NOFAIL : 0));
}

/*
 * Journal_head storage management
 */
static struct kmem_cache *hfsplus_jbd_head_cache;
#ifdef CONFIG_JBD_DEBUG
static atomic_t nr_hfsplus_jbd_heads = ATOMIC_INIT(0);
#endif

static int hfsplus_jbd_init_journal_head_cache(void)
{
	int retval;

	HFSPLUS_J_ASSERT(hfsplus_jbd_head_cache == NULL);
	hfsplus_jbd_head_cache = kmem_cache_create("hfsplus_jbd_head",
				sizeof(struct hfsplus_jbd_head),
				0,		/* offset */
				SLAB_TEMPORARY,	/* flags */
				NULL);		/* ctor */
	retval = 0;
	if (!hfsplus_jbd_head_cache) {
		retval = -ENOMEM;
		printk(KERN_EMERG "JBD: no memory for hfsplus_jbd_head cache\n");
	}
	return retval;
}

static void hfsplus_jbd_destroy_hfsplus_jbd_head_cache(void)
{
	if (hfsplus_jbd_head_cache) {
		kmem_cache_destroy(hfsplus_jbd_head_cache);
		hfsplus_jbd_head_cache = NULL;
	}
}

/*
 * hfsplus_jbd_head splicing and dicing
 */
static struct hfsplus_jbd_head *hfsplus_jbd_alloc_journal_head(void)
{
	struct hfsplus_jbd_head *ret;

#ifdef CONFIG_JBD_DEBUG
	atomic_inc(&nr_hfsplus_jbd_heads);
#endif
	ret = kmem_cache_alloc(hfsplus_jbd_head_cache, GFP_NOFS);
	if (ret == NULL) {
		hfsplus_jbd_debug(1, "out of memory for hfsplus_jbd_head\n");
		printk_ratelimited(KERN_NOTICE "ENOMEM in %s, retrying.\n",
				   __func__);

		while (ret == NULL) {
			yield();
			ret = kmem_cache_alloc(hfsplus_jbd_head_cache, GFP_NOFS);
			if (ret == 0)
				printk(KERN_ERR "Again out of memory for hfsplus_jbd_head\n");
		}
	}
	return ret;
}

static void hfsplus_jbd_free_journal_head(struct hfsplus_jbd_head *jh)
{
#ifdef CONFIG_JBD_DEBUG
	atomic_dec(&nr_hfsplus_jbd_heads);
	memset(jh, 0x5b, sizeof(*jh));
#endif
	kmem_cache_free(hfsplus_jbd_head_cache, jh);
}

/*
 * A hfsplus_jbd_head is attached to a buffer_head whenever JBD has an
 * interest in the buffer.
 *
 * Whenever a buffer has an attached hfsplus_jbd_head, its ->b_state:BH_JBD bit
 * is set.  This bit is tested in core kernel code where we need to take
 * JBD-specific actions.  Testing the zeroness of ->b_private is not reliable
 * there.
 *
 * When a buffer has its BH_JBD bit set, its ->b_count is elevated by one.
 *
 * When a buffer has its BH_JBD bit set it is immune from being released by
 * core kernel code, mainly via ->b_count.
 *
 * A hfsplus_jbd_head is detached from its buffer_head when the hfsplus_jbd_head's
 * b_jcount reaches zero. Running transaction (b_transaction) and checkpoint
 * transaction (b_cp_transaction) hold their references to b_jcount.
 *
 * Various places in the kernel want to attach a hfsplus_jbd_head to a buffer_head
 * _before_ attaching the hfsplus_jbd_head to a transaction.  To protect the
 * hfsplus_jbd_head in this situation, hfsplus_jbd_add_journal_head elevates the
 * hfsplus_jbd_head's b_jcount refcount by one.  The caller must call
 * hfsplus_jbd_put_journal_head() to undo this.
 *
 * So the typical usage would be:
 *
 *	(Attach a hfsplus_jbd_head if needed.  Increments b_jcount)
 *	struct hfsplus_jbd_head *jh = hfsplus_jbd_add_journal_head(bh);
 *	...
 *      (Get another reference for transaction)
 *      hfsplus_jbd_grab_journal_head(bh);
 *      jh->b_transaction = xxx;
 *      (Put original reference)
 *      hfsplus_jbd_put_journal_head(jh);
 */

/*
 * Give a buffer_head a hfsplus_jbd_head.
 *
 * May sleep.
 */
struct hfsplus_jbd_head *hfsplus_jbd_add_journal_head(struct buffer_head *bh)
{
	struct hfsplus_jbd_head *jh;
	struct hfsplus_jbd_head *new_jh = NULL;

repeat:
	if (!buffer_hfsplus_jbd(bh)) {
		new_jh = hfsplus_jbd_alloc_journal_head();
		if (!new_jh) {
			printk(KERN_ERR "Error in allocating journal head\n");
			return NULL;
		}
		memset(new_jh, 0, sizeof(*new_jh));
	}

	hfsplus_jbd_lock_bh_hfsplus_jbd_head(bh);
	if (buffer_hfsplus_jbd(bh)) {
		jh = hfsplus_bh2jh(bh);
	} else {
		HFSPLUS_J_ASSERT_BH(bh,
			(atomic_read(&bh->b_count) > 0) ||
			(bh->b_page && bh->b_page->mapping));

		if (!new_jh) {
			hfsplus_jbd_unlock_bh_hfsplus_jbd_head(bh);
			goto repeat;
		}

		jh = new_jh;
		new_jh = NULL;		/* We consumed it */
		set_buffer_hfsplus_jbd(bh);
		bh->b_private = jh;
		jh->b_bh = bh;
		get_bh(bh);
		HFSPLUS_BUFFER_TRACE(bh, "added hfsplus_jbd_head");
	}
	jh->b_jcount++;
	hfsplus_jbd_unlock_bh_hfsplus_jbd_head(bh);
	if (new_jh)
		hfsplus_jbd_free_journal_head(new_jh);
	return bh->b_private;
}

/*
 * Grab a ref against this buffer_head's hfsplus_jbd_head.  If it ended up not
 * having a hfsplus_jbd_head, return NULL
 */
struct hfsplus_jbd_head *hfsplus_jbd_grab_journal_head(struct buffer_head *bh)
{
	struct hfsplus_jbd_head *jh = NULL;

	hfsplus_jbd_lock_bh_hfsplus_jbd_head(bh);
	if (buffer_hfsplus_jbd(bh)) {
		jh = hfsplus_bh2jh(bh);
		jh->b_jcount++;
	}
	hfsplus_jbd_unlock_bh_hfsplus_jbd_head(bh);
	return jh;
}

static void __hfsplus_jbd_remove_journal_head(struct buffer_head *bh)
{
	struct hfsplus_jbd_head *jh = hfsplus_bh2jh(bh);

	HFSPLUS_J_ASSERT_JH(jh, jh->b_jcount >= 0);
	HFSPLUS_J_ASSERT_JH(jh, jh->b_transaction == NULL);
	HFSPLUS_J_ASSERT_JH(jh, jh->b_next_transaction == NULL);
	HFSPLUS_J_ASSERT_JH(jh, jh->b_cp_transaction == NULL);
	HFSPLUS_J_ASSERT_JH(jh, jh->b_jlist == HFSPLUS_BJ_None);
	HFSPLUS_J_ASSERT_BH(bh, buffer_hfsplus_jbd(bh));
	HFSPLUS_J_ASSERT_BH(bh, hfsplus_jh2bh(jh) == bh);
	HFSPLUS_BUFFER_TRACE(bh, "remove hfsplus_jbd_head");
	if (jh->b_frozen_data) {
		printk(KERN_WARNING "%s: freeing b_frozen_data\n", __func__);
		hfsplus_jbd_free(jh->b_frozen_data, bh->b_size);
	}
	if (jh->b_committed_data) {
		printk(KERN_WARNING "%s: freeing b_committed_data\n", __func__);
		hfsplus_jbd_free(jh->b_committed_data, bh->b_size);
	}
	bh->b_private = NULL;
	jh->b_bh = NULL;	/* debug, really */
	clear_buffer_hfsplus_jbd(bh);
	hfsplus_jbd_free_journal_head(jh);
}

/*
 * Drop a reference on the passed hfsplus_jbd_head.  If it fell to zero then
 * release the hfsplus_jbd_head from the buffer_head.
 */
void hfsplus_jbd_put_journal_head(struct hfsplus_jbd_head *jh)
{
	struct buffer_head *bh = hfsplus_jh2bh(jh);

	hfsplus_jbd_lock_bh_hfsplus_jbd_head(bh);
	HFSPLUS_J_ASSERT_JH(jh, jh->b_jcount > 0);
	--jh->b_jcount;
	if (!jh->b_jcount) {
		__hfsplus_jbd_remove_journal_head(bh);
		hfsplus_jbd_unlock_bh_hfsplus_jbd_head(bh);
		__brelse(bh);
	} else
		hfsplus_jbd_unlock_bh_hfsplus_jbd_head(bh);
}

/*
 * debugfs tunables
 */
#ifdef CONFIG_JBD_DEBUG
int hfsplus_jbd_enable_debug;
EXPORT_SYMBOL(hfsplus_jbd_enable_debug);
#endif

#if defined(CONFIG_JBD_DEBUG) && defined(CONFIG_PROC_FS)
static struct proc_dir_entry *proc_hfsplus_jbd_debug;

static int read_hfsplus_jbd_debug(char *page, char **start, off_t off,
			  int count, int *eof, void *data)
{
	int ret;

	ret = sprintf(page + off, "%d\n", hfsplus_jbd_enable_debug);
	*eof = 1;
	return ret;
}

static int write_hfsplus_jbd_debug(struct file *file, const char __user *buffer,
			   unsigned long count, void *data)
{
	char buf[32];

	if (count > ARRAY_SIZE(buf) - 1)
		count = ARRAY_SIZE(buf) - 1;
	if (copy_from_user(buf, buffer, count))
		return -EFAULT;
	buf[ARRAY_SIZE(buf) - 1] = '\0';
	hfsplus_jbd_enable_debug = simple_strtoul(buf, NULL, 10);
	return count;
}

#define JBD_PROC_NAME "sys/fs/jbd-debug"

static void __init create_jbd_proc_entry(void)
{
	proc_hfsplus_jbd_debug = create_proc_entry(JBD_PROC_NAME, 0644, NULL);
	if (proc_hfsplus_jbd_debug) {
		/* Why is this so hard? */
		proc_hfsplus_jbd_debug->read_proc = read_hfsplus_jbd_debug;
		proc_hfsplus_jbd_debug->write_proc = write_hfsplus_jbd_debug;
	}
}

static void __exit remove_jbd_proc_entry(void)
{
	if (proc_hfsplus_jbd_debug)
		remove_proc_entry(JBD_PROC_NAME, NULL);
}

#else

#define create_jbd_proc_entry() do {} while (0)
#define remove_jbd_proc_entry() do {} while (0)

#endif

struct kmem_cache *hfsplus_jbd_handle_cache;

static int __init hfsplus_jbd_init_handle_cache(void)
{
	hfsplus_jbd_handle_cache = kmem_cache_create("hfsplus_jbd_handle",
				sizeof(hfsplus_jbd_handle_t),
				0,		/* offset */
				SLAB_TEMPORARY,	/* flags */
				NULL);		/* ctor */
	if (hfsplus_jbd_handle_cache == NULL) {
		printk(KERN_EMERG "JBD: failed to create handle cache\n");
		return -ENOMEM;
	}
	return 0;
}

static void hfsplus_jbd_destroy_handle_cache(void)
{
	if (hfsplus_jbd_handle_cache)
		kmem_cache_destroy(hfsplus_jbd_handle_cache);
}

/*
 * Module startup and shutdown
 */

static int __init hfsplus_jbd_init_caches(void)
{
	int ret;

	ret = hfsplus_jbd_init_revoke_caches();
	if (ret == 0)
		ret = hfsplus_jbd_init_journal_head_cache();
	if (ret == 0)
		ret = hfsplus_jbd_init_handle_cache();
	return ret;
}

static void hfsplus_jbd_destroy_caches(void)
{
	hfsplus_jbd_destroy_revoke_caches();
	hfsplus_jbd_destroy_hfsplus_jbd_head_cache();
	hfsplus_jbd_destroy_handle_cache();
}

int __init hfsplus_jbd_init(void)
{
	int ret;

/* Static check for data structure consistency.  There's no code
 * invoked --- we'll just get a linker failure if things aren't right.
 */
	extern void hfsplus_jbd_bad_superblock_size(void);
	if (sizeof(struct hfsplus_jbd_superblock_s) != 1024)
		hfsplus_jbd_bad_superblock_size();

	ret = hfsplus_jbd_init_caches();
	if (ret != 0)
		hfsplus_jbd_destroy_caches();
	create_jbd_proc_entry();
	return ret;
}

void  hfsplus_jbd_exit(void)
{
#ifdef CONFIG_JBD_DEBUG
	int n = atomic_read(&nr_hfsplus_jbd_heads);
	if (n)
		printk(KERN_EMERG "JBD: leaked %d hfsplus_jbd_heads!\n", n);
#endif
	remove_jbd_proc_entry();
	hfsplus_jbd_destroy_caches();
}

MODULE_LICENSE("GPL");
late_initcall(hfsplus_jbd_init);
module_exit(hfsplus_jbd_exit);

