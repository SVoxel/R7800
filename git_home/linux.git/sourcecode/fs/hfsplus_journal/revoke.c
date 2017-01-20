/*
 * linux/fs/hfsplus_journal/revoke.c
 *
 * Written by Stephen C. Tweedie <sct@redhat.com>, 2000
 *
 * Copyright 2000 Red Hat corp --- All Rights Reserved
 *
 * This file is part of the Linux kernel and is made available under
 * the terms of the GNU General Public License, version 2, or at your
 * option, any later version, incorporated herein by reference.
 *
 * Journal revoke routines for the generic filesystem journaling code;
 * part of the ext2fs journaling system.
 *
 * Revoke is the mechanism used to prevent old log records for deleted
 * metadata from being replayed on top of newer data using the same
 * blocks.  The revoke mechanism is used in two separate places:
 *
 * + Commit: during commit we write the entire list of the current
 *   transaction's revoked blocks to the journal
 *
 * + Recovery: during recovery we record the transaction ID of all
 *   revoked blocks.  If there are multiple revoke records in the log
 *   for a single block, only the last one counts, and if there is a log
 *   entry for a block beyond the last revoke, then that log entry still
 *   gets replayed.
 *
 * We can get interactions between revokes and new log data within a
 * single transaction:
 *
 * Block is revoked and then journaled:
 *   The desired end result is the journaling of the new block, so we
 *   cancel the revoke before the transaction commits.
 *
 * Block is journaled and then revoked:
 *   The revoke must take precedence over the write of the block, so we
 *   need either to cancel the journal entry or to write the revoke
 *   later in the log than the log block.  In this case, we choose the
 *   latter: journaling a block cancels any revoke record for that block
 *   in the current transaction, so any revoke for that block in the
 *   transaction must have happened after the block was journaled and so
 *   the revoke must take precedence.
 *
 * Block is revoked and then written as data:
 *   The data write is allowed to succeed, but the revoke is _not_
 *   cancelled.  We still need to prevent old log records from
 *   overwriting the new data.  We don't even need to clear the revoke
 *   bit here.
 *
 * We cache revoke status of a buffer in the current transaction in b_states
 * bits.  As the name says, revokevalid flag indicates that the cached revoke
 * status of a buffer is valid and we can rely on the cached status.
 *
 * Revoke information on buffers is a tri-state value:
 *
 * RevokeValid clear:	no cached revoke status, need to look it up
 * RevokeValid set, Revoked clear:
 *			buffer has not been revoked, and cancel_revoke
 *			need do nothing.
 * RevokeValid set, Revoked set:
 *			buffer has been revoked.
 *
 * Locking rules:
 * We keep two hash tables of revoke records. One hashtable belongs to the
 * running transaction (is pointed to by journal->j_revoke), the other one
 * belongs to the committing transaction. Accesses to the second hash table
 * happen only from the kjournald and no other thread touches this table.  Also
 * hfsplus_jbd_switch_revoke_table() which switches which hashtable belongs to the
 * running and which to the committing transaction is called only from
 * kjournald. Therefore we need no locks when accessing the hashtable belonging
 * to the committing transaction.
 *
 * All users operating on the hash table belonging to the running transaction
 * have a handle to the transaction. Therefore they are safe from kjournald
 * switching hash tables under them. For operations on the lists of entries in
 * the hash table j_revoke_lock is used.
 *
 * Finally, also replay code uses the hash tables but at this moment no one else
 * can touch them (filesystem isn't mounted yet) and hence no locking is
 * needed.
 */

#include <linux/time.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/list.h>
//#include <linux/smp_lock.h>
#include <linux/init.h>
#include "hfsplus_jbd.h"
#include <linux/bio.h>
#include <linux/log2.h>

static struct kmem_cache *hfsplus_revoke_record_cache;
static struct kmem_cache *hfsplus_revoke_table_cache;

/* Each revoke record represents one single revoked block.  During
   journal replay, this involves recording the transaction ID of the
   last transaction to revoke this block. */

struct hfsplus_jbd_revoke_record_s
{
	struct list_head  hash;
	hfsplus_jbd_tid_t		  sequence;	/* Used for recovery only */
	unsigned int	  blocknr;
};


/* The revoke table is just a simple hash table of revoke records. */
struct hfsplus_jbd_revoke_table_s
{
	/* It is conceivable that we might want a larger hash table
	 * for recovery.  Must be a power of two. */
	int		  hash_size;
	int		  hash_shift;
	struct list_head *hash_table;
};


#ifdef __KERNEL__
static void write_one_revoke_record(hfsplus_jbd_t *, hfsplus_transaction_t *,
				    struct hfsplus_jbd_head **, int *,
				    struct hfsplus_jbd_revoke_record_s *, int);
static void flush_descriptor(hfsplus_jbd_t *, struct hfsplus_jbd_head *, int, int);
#endif

/* Utility functions to maintain the revoke table */

/* Borrowed from buffer.c: this is a tried and tested block hash function */
static inline int hash(hfsplus_jbd_t *journal, unsigned int block)
{
	struct hfsplus_jbd_revoke_table_s *table = journal->j_revoke;
	int hash_shift = table->hash_shift;

	return ((block << (hash_shift - 6)) ^
		(block >> 13) ^
		(block << (hash_shift - 12))) & (table->hash_size - 1);
}

static int insert_revoke_hash(hfsplus_jbd_t *journal, unsigned int blocknr,
			      hfsplus_jbd_tid_t seq)
{
	struct list_head *hash_list;
	struct hfsplus_jbd_revoke_record_s *record;

repeat:
	record = kmem_cache_alloc(hfsplus_revoke_record_cache, GFP_NOFS);
	if (!record)
		goto oom;

	record->sequence = seq;
	record->blocknr = blocknr;
	hash_list = &journal->j_revoke->hash_table[hash(journal, blocknr)];
	spin_lock(&journal->j_revoke_lock);
	list_add(&record->hash, hash_list);
	spin_unlock(&journal->j_revoke_lock);
	return 0;

oom:
	if (!hfsplus_jbd_oom_retry)
		return -ENOMEM;
	hfsplus_jbd_debug(1, "ENOMEM in %s, retrying\n", __func__);
	yield();
	goto repeat;
}

/* Find a revoke record in the journal's hash table. */

static struct hfsplus_jbd_revoke_record_s *find_revoke_record(hfsplus_jbd_t *journal,
						      unsigned int blocknr)
{
	struct list_head *hash_list;
	struct hfsplus_jbd_revoke_record_s *record;

	hash_list = &journal->j_revoke->hash_table[hash(journal, blocknr)];

	spin_lock(&journal->j_revoke_lock);
	record = (struct hfsplus_jbd_revoke_record_s *) hash_list->next;
	while (&(record->hash) != hash_list) {
		if (record->blocknr == blocknr) {
			spin_unlock(&journal->j_revoke_lock);
			return record;
		}
		record = (struct hfsplus_jbd_revoke_record_s *) record->hash.next;
	}
	spin_unlock(&journal->j_revoke_lock);
	return NULL;
}

void hfsplus_jbd_destroy_revoke_caches(void)
{
	if (hfsplus_revoke_record_cache) {
		kmem_cache_destroy(hfsplus_revoke_record_cache);
		hfsplus_revoke_record_cache = NULL;
	}
	if (hfsplus_revoke_table_cache) {
		kmem_cache_destroy(hfsplus_revoke_table_cache);
		hfsplus_revoke_table_cache = NULL;
	}
}

int __init hfsplus_jbd_init_revoke_caches(void)
{
	HFSPLUS_J_ASSERT(!hfsplus_revoke_record_cache);
	HFSPLUS_J_ASSERT(!hfsplus_revoke_table_cache);

	hfsplus_revoke_record_cache = kmem_cache_create("hfsplus_jbd_revoke_record",
					   sizeof(struct hfsplus_jbd_revoke_record_s),
					   0,
					   SLAB_HWCACHE_ALIGN|SLAB_TEMPORARY,
					   NULL);
	if (!hfsplus_revoke_record_cache)
		goto record_cache_failure;

	hfsplus_revoke_table_cache = kmem_cache_create("hfsplus_jbd_revoke_table",
					   sizeof(struct hfsplus_jbd_revoke_table_s),
					   0, SLAB_TEMPORARY, NULL);
	if (!hfsplus_revoke_table_cache)
		goto table_cache_failure;

	return 0;

table_cache_failure:
	hfsplus_jbd_destroy_revoke_caches();
record_cache_failure:
	return -ENOMEM;
}

static struct hfsplus_jbd_revoke_table_s *hfsplus_jbd_init_revoke_table(int hash_size)
{
	int shift = 0;
	int tmp = hash_size;
	struct hfsplus_jbd_revoke_table_s *table;

	table = kmem_cache_alloc(hfsplus_revoke_table_cache, GFP_KERNEL);
	if (!table)
		goto out;

	while((tmp >>= 1UL) != 0UL)
		shift++;

	table->hash_size = hash_size;
	table->hash_shift = shift;
	table->hash_table =
		kmalloc(hash_size * sizeof(struct list_head), GFP_KERNEL);
	if (!table->hash_table) {
		kmem_cache_free(hfsplus_revoke_table_cache, table);
		table = NULL;
		goto out;
	}

	for (tmp = 0; tmp < hash_size; tmp++)
		INIT_LIST_HEAD(&table->hash_table[tmp]);

out:
	return table;
}

static void hfsplus_jbd_destroy_revoke_table(struct hfsplus_jbd_revoke_table_s *table)
{
	int i;
	struct list_head *hash_list;

	for (i = 0; i < table->hash_size; i++) {
		hash_list = &table->hash_table[i];
		HFSPLUS_J_ASSERT(list_empty(hash_list));
	}

	kfree(table->hash_table);
	kmem_cache_free(hfsplus_revoke_table_cache, table);
}

/* Initialise the revoke table for a given journal to a given size. */
int hfsplus_jbd_init_revoke(hfsplus_jbd_t *journal, int hash_size)
{
	HFSPLUS_J_ASSERT(journal->j_revoke_table[0] == NULL);
	HFSPLUS_J_ASSERT(is_power_of_2(hash_size));

	journal->j_revoke_table[0] = hfsplus_jbd_init_revoke_table(hash_size);
	if (!journal->j_revoke_table[0])
		goto fail0;

	journal->j_revoke_table[1] = hfsplus_jbd_init_revoke_table(hash_size);
	if (!journal->j_revoke_table[1])
		goto fail1;

	journal->j_revoke = journal->j_revoke_table[1];

	spin_lock_init(&journal->j_revoke_lock);

	return 0;

fail1:
	hfsplus_jbd_destroy_revoke_table(journal->j_revoke_table[0]);
fail0:
	return -ENOMEM;
}

/* Destroy a journal's revoke table.  The table must already be empty! */
void hfsplus_jbd_destroy_revoke(hfsplus_jbd_t *journal)
{
	journal->j_revoke = NULL;
	if (journal->j_revoke_table[0])
		hfsplus_jbd_destroy_revoke_table(journal->j_revoke_table[0]);
	if (journal->j_revoke_table[1])
		hfsplus_jbd_destroy_revoke_table(journal->j_revoke_table[1]);
}


#ifdef __KERNEL__

/*
 * hfsplus_jbd_revoke: revoke a given buffer_head from the journal.  This
 * prevents the block from being replayed during recovery if we take a
 * crash after this current transaction commits.  Any subsequent
 * metadata writes of the buffer in this transaction cancel the
 * revoke.
 *
 * Note that this call may block --- it is up to the caller to make
 * sure that there are no further calls to journal_write_metadata
 * before the revoke is complete.  In ext3, this implies calling the
 * revoke before clearing the block bitmap when we are deleting
 * metadata.
 *
 * Revoke performs a hfsplus_jbd_forget on any buffer_head passed in as a
 * parameter, but does _not_ forget the buffer_head if the bh was only
 * found implicitly.
 *
 * bh_in may not be a journalled buffer - it may have come off
 * the hash tables without an attached hfsplus_jbd_head.
 *
 * If bh_in is non-zero, hfsplus_jbd_revoke() will decrement its b_count
 * by one.
 */

int hfsplus_jbd_revoke(hfsplus_jbd_handle_t *handle, unsigned int blocknr,
		   struct buffer_head *bh_in)
{
	struct buffer_head *bh = NULL;
	hfsplus_jbd_t *journal;
	struct block_device *bdev;
	int err;

	might_sleep();
	if (bh_in)
		HFSPLUS_BUFFER_TRACE(bh_in, "enter");

	journal = handle->h_transaction->t_journal;
	if (!hfsplus_jbd_set_features(journal, 0, 0, JFS_FEATURE_INCOMPAT_REVOKE)){
		HFSPLUS_J_ASSERT (!"Cannot set revoke feature!");
		return -EINVAL;
	}

	bdev = journal->j_fs_dev;
	bh = bh_in;

	if (!bh) {
		bh = __find_get_block(bdev, blocknr, journal->j_blocksize);
		if (bh)
			HFSPLUS_BUFFER_TRACE(bh, "found on hash");
	}
#ifdef JBD_EXPENSIVE_CHECKING
	else {
		struct buffer_head *bh2;

		/* If there is a different buffer_head lying around in
		 * memory anywhere... */
		bh2 = __find_get_block(bdev, blocknr, journal->j_blocksize);
		if (bh2) {
			/* ... and it has RevokeValid status... */
			if (bh2 != bh && buffer_revokevalid(bh2))
				/* ...then it better be revoked too,
				 * since it's illegal to create a revoke
				 * record against a buffer_head which is
				 * not marked revoked --- that would
				 * risk missing a subsequent revoke
				 * cancel. */
				HFSPLUS_J_ASSERT_BH(bh2, buffer_hfsplus_jbd_revoked(bh2));
			put_bh(bh2);
		}
	}
#endif

	/* We really ought not ever to revoke twice in a row without
           first having the revoke cancelled: it's illegal to free a
           block twice without allocating it in between! */
	if (bh) {
		if (!HFSPLUS_J_EXPECT_BH(bh, !buffer_hfsplus_jbd_revoked(bh),
				 "inconsistent data on disk")) {
			if (!bh_in)
				brelse(bh);
			return -EIO;
		}
		set_buffer_hfsplus_jbd_revoked(bh);
		set_buffer_hfsplus_jbd_revokevalid(bh);
		if (bh_in) {
			HFSPLUS_BUFFER_TRACE(bh_in, "call hfsplus_jbd_forget");
			hfsplus_jbd_forget(handle, bh_in);
		} else {
			HFSPLUS_BUFFER_TRACE(bh, "call brelse");
			__brelse(bh);
		}
	}

	hfsplus_jbd_debug(2, "insert revoke for block %u, bh_in=%p\n", blocknr, bh_in);
	err = insert_revoke_hash(journal, blocknr,
				handle->h_transaction->t_tid);
	HFSPLUS_BUFFER_TRACE(bh_in, "exit");
	return err;
}

/*
 * Cancel an outstanding revoke.  For use only internally by the
 * journaling code (called from journal_get_write_access).
 *
 * We trust buffer_hfsplus_jbd_revoked() on the buffer if the buffer is already
 * being journaled: if there is no revoke pending on the buffer, then we
 * don't do anything here.
 *
 * This would break if it were possible for a buffer to be revoked and
 * discarded, and then reallocated within the same transaction.  In such
 * a case we would have lost the revoked bit, but when we arrived here
 * the second time we would still have a pending revoke to cancel.  So,
 * do not trust the Revoked bit on buffers unless RevokeValid is also
 * set.
 */
int hfsplus_jbd_cancel_revoke(hfsplus_jbd_handle_t *handle, struct hfsplus_jbd_head *jh)
{
	struct hfsplus_jbd_revoke_record_s *record;
	hfsplus_jbd_t *journal = handle->h_transaction->t_journal;
	int need_cancel;
	int did_revoke = 0;	/* akpm: debug */
	struct buffer_head *bh = hfsplus_jh2bh(jh);

	hfsplus_jbd_debug(4, "hfsplus_jbd_head %p, cancelling revoke\n", jh);

	/* Is the existing Revoke bit valid?  If so, we trust it, and
	 * only perform the full cancel if the revoke bit is set.  If
	 * not, we can't trust the revoke bit, and we need to do the
	 * full search for a revoke record. */
	if (test_set_buffer_hfsplus_jbd_revokevalid(bh)) {
		need_cancel = test_clear_buffer_hfsplus_jbd_revoked(bh);
	} else {
		need_cancel = 1;
		clear_buffer_hfsplus_jbd_revoked(bh);
	}

	if (need_cancel) {
		record = find_revoke_record(journal, bh->b_blocknr);
		if (record) {
			hfsplus_jbd_debug(4, "cancelled existing revoke on "
				  "blocknr %llu\n", (unsigned long long)bh->b_blocknr);
			spin_lock(&journal->j_revoke_lock);
			list_del(&record->hash);
			spin_unlock(&journal->j_revoke_lock);
			kmem_cache_free(hfsplus_revoke_record_cache, record);
			did_revoke = 1;
		}
	}

#ifdef JBD_EXPENSIVE_CHECKING
	/* There better not be one left behind by now! */
	record = find_revoke_record(journal, bh->b_blocknr);
	HFSPLUS_J_ASSERT_JH(jh, record == NULL);
#endif

	/* Finally, have we just cleared revoke on an unhashed
	 * buffer_head?  If so, we'd better make sure we clear the
	 * revoked status on any hashed alias too, otherwise the revoke
	 * state machine will get very upset later on. */
	if (need_cancel) {
		struct buffer_head *bh2;
		bh2 = __find_get_block(bh->b_bdev, bh->b_blocknr, bh->b_size);
		if (bh2) {
			if (bh2 != bh)
				clear_buffer_hfsplus_jbd_revoked(bh2);
			__brelse(bh2);
		}
	}
	return did_revoke;
}

/*
 * hfsplus_jbd_clear_revoked_flags clears revoked flag of buffers in
 * revoke table to reflect there is no revoked buffer in the next
 * transaction which is going to be started.
 */
void hfsplus_jbd_clear_buffer_revoked_flags(hfsplus_jbd_t *journal)
{
	struct hfsplus_jbd_revoke_table_s *revoke = journal->j_revoke;
	int i = 0;

	for (i = 0; i < revoke->hash_size; i++) {
		struct list_head *hash_list;
		struct list_head *list_entry;
		hash_list = &revoke->hash_table[i];

		list_for_each(list_entry, hash_list) {
			struct hfsplus_jbd_revoke_record_s *record;
			struct buffer_head *bh;
			record = (struct hfsplus_jbd_revoke_record_s *)list_entry;
			bh = __find_get_block(journal->j_fs_dev,
					      record->blocknr,
					      journal->j_blocksize);
			if (bh) {
				clear_buffer_hfsplus_jbd_revoked(bh);
				__brelse(bh);
			}
		}
	}
}

/* journal_switch_revoke table select j_revoke for next transaction
 * we do not want to suspend any processing until all revokes are
 * written -bzzz
 */
void hfsplus_jbd_switch_revoke_table(hfsplus_jbd_t *journal)
{
	int i;

	if (journal->j_revoke == journal->j_revoke_table[0])
		journal->j_revoke = journal->j_revoke_table[1];
	else
		journal->j_revoke = journal->j_revoke_table[0];

	for (i = 0; i < journal->j_revoke->hash_size; i++)
		INIT_LIST_HEAD(&journal->j_revoke->hash_table[i]);
}

/*
 * Write revoke records to the journal for all entries in the current
 * revoke hash, deleting the entries as we go.
 */
void hfsplus_jbd_write_revoke_records(hfsplus_jbd_t *journal,
				  hfsplus_transaction_t *transaction, int write_op)
{
	struct hfsplus_jbd_head *descriptor;
	struct hfsplus_jbd_revoke_record_s *record;
	struct hfsplus_jbd_revoke_table_s *revoke;
	struct list_head *hash_list;
	int i, offset, count;

	descriptor = NULL;
	offset = 0;
	count = 0;

	/* select revoke table for committing transaction */
	revoke = journal->j_revoke == journal->j_revoke_table[0] ?
		journal->j_revoke_table[1] : journal->j_revoke_table[0];

	for (i = 0; i < revoke->hash_size; i++) {
		hash_list = &revoke->hash_table[i];

		while (!list_empty(hash_list)) {
			record = (struct hfsplus_jbd_revoke_record_s *)
				hash_list->next;
			write_one_revoke_record(journal, transaction,
						&descriptor, &offset,
						record, write_op);
			count++;
			list_del(&record->hash);
			kmem_cache_free(hfsplus_revoke_record_cache, record);
		}
	}
	if (descriptor)
		flush_descriptor(journal, descriptor, offset, write_op);
	hfsplus_jbd_debug(1, "Wrote %d revoke records\n", count);
}

/*
 * Write out one revoke record.  We need to create a new descriptor
 * block if the old one is full or if we have not already created one.
 */

static void write_one_revoke_record(hfsplus_jbd_t *journal,
				    hfsplus_transaction_t *transaction,
				    struct hfsplus_jbd_head **descriptorp,
				    int *offsetp,
				    struct hfsplus_jbd_revoke_record_s *record,
				    int write_op)
{
	struct hfsplus_jbd_head *descriptor;
	int offset;
	hfsplus_jbd_header_t *header;

	/* If we are already aborting, this all becomes a noop.  We
           still need to go round the loop in
           hfsplus_jbd_write_revoke_records in order to free all of the
           revoke records: only the IO to the journal is omitted. */
	if (is_hfsplus_jbd_aborted(journal))
		return;

	descriptor = *descriptorp;
	offset = *offsetp;

	/* Make sure we have a descriptor with space left for the record */
	if (descriptor) {
		if (offset == journal->j_blocksize) {
			flush_descriptor(journal, descriptor, offset, write_op);
			descriptor = NULL;
		}
	}

	if (!descriptor) {
		descriptor = hfsplus_jbd_get_descriptor_buffer(journal);
		if (!descriptor)
			return;
		header = (hfsplus_jbd_header_t *) &hfsplus_jh2bh(descriptor)->b_data[0];
		header->h_magic     = cpu_to_be32(JFS_MAGIC_NUMBER);
		header->h_blocktype = cpu_to_be32(JFS_REVOKE_BLOCK);
		header->h_sequence  = cpu_to_be32(transaction->t_tid);

		/* Record it so that we can wait for IO completion later */
		HFSPLUS_JBUFFER_TRACE(descriptor, "file as HFSPLUS_BJ_LogCtl");
		hfsplus_jbd_file_buffer(descriptor, transaction, HFSPLUS_BJ_LogCtl);

		offset = sizeof(hfsplus_jbd_revoke_header_t);
		*descriptorp = descriptor;
	}

	* ((__be32 *)(&hfsplus_jh2bh(descriptor)->b_data[offset])) =
		cpu_to_be32(record->blocknr);
	offset += 4;
	*offsetp = offset;
}

/*
 * Flush a revoke descriptor out to the journal.  If we are aborting,
 * this is a noop; otherwise we are generating a buffer which needs to
 * be waited for during commit, so it has to go onto the appropriate
 * journal buffer list.
 */

static void flush_descriptor(hfsplus_jbd_t *journal,
			     struct hfsplus_jbd_head *descriptor,
			     int offset, int write_op)
{
	hfsplus_jbd_revoke_header_t *header;
	struct buffer_head *bh = hfsplus_jh2bh(descriptor);

	if (is_hfsplus_jbd_aborted(journal)) {
		put_bh(bh);
		return;
	}

	header = (hfsplus_jbd_revoke_header_t *) hfsplus_jh2bh(descriptor)->b_data;
	header->r_count = cpu_to_be32(offset);
	set_buffer_hfsplus_jbd_jwrite(bh);
	HFSPLUS_BUFFER_TRACE(bh, "write");
	set_buffer_dirty(bh);
	write_dirty_buffer(bh, write_op);
}
#endif

/*
 * Revoke support for recovery.
 *
 * Recovery needs to be able to:
 *
 *  record all revoke records, including the tid of the latest instance
 *  of each revoke in the journal
 *
 *  check whether a given block in a given transaction should be replayed
 *  (ie. has not been revoked by a revoke record in that or a subsequent
 *  transaction)
 *
 *  empty the revoke table after recovery.
 */

/*
 * First, setting revoke records.  We create a new revoke record for
 * every block ever revoked in the log as we scan it for recovery, and
 * we update the existing records if we find multiple revokes for a
 * single block.
 */

int hfsplus_jbd_set_revoke(hfsplus_jbd_t *journal,
		       unsigned int blocknr,
		       hfsplus_jbd_tid_t sequence)
{
	struct hfsplus_jbd_revoke_record_s *record;

	record = find_revoke_record(journal, blocknr);
	if (record) {
		/* If we have multiple occurrences, only record the
		 * latest sequence number in the hashed record */
		if (hfsplus_tid_gt(sequence, record->sequence))
			record->sequence = sequence;
		return 0;
	}
	return insert_revoke_hash(journal, blocknr, sequence);
}

/*
 * Test revoke records.  For a given block referenced in the log, has
 * that block been revoked?  A revoke record with a given transaction
 * sequence number revokes all blocks in that transaction and earlier
 * ones, but later transactions still need replayed.
 */

int hfsplus_jbd_test_revoke(hfsplus_jbd_t *journal,
			unsigned int blocknr,
			hfsplus_jbd_tid_t sequence)
{
	struct hfsplus_jbd_revoke_record_s *record;

	record = find_revoke_record(journal, blocknr);
	if (!record)
		return 0;
	if (hfsplus_tid_gt(sequence, record->sequence))
		return 0;
	return 1;
}

/*
 * Finally, once recovery is over, we need to clear the revoke table so
 * that it can be reused by the running filesystem.
 */

void hfsplus_jbd_clear_revoke(hfsplus_jbd_t *journal)
{
	int i;
	struct list_head *hash_list;
	struct hfsplus_jbd_revoke_record_s *record;
	struct hfsplus_jbd_revoke_table_s *revoke;

	revoke = journal->j_revoke;

	for (i = 0; i < revoke->hash_size; i++) {
		hash_list = &revoke->hash_table[i];
		while (!list_empty(hash_list)) {
			record = (struct hfsplus_jbd_revoke_record_s*) hash_list->next;
			list_del(&record->hash);
			kmem_cache_free(hfsplus_revoke_record_cache, record);
		}
	}
}
