/*
 * include/linux/journal-head.h
 *
 * buffer_head fields for JBD
 *
 * 27 May 2001 Andrew Morton
 *	Created - pulled out of fs.h
 */

#ifndef HFSPLUS_JOURNAL_HEAD_H_INCLUDED
#define HFSPLUS_JOURNAL_HEAD_H_INCLUDED

typedef unsigned int		hfsplus_jbd_tid_t;		/* Unique transaction ID */
typedef struct hfsplus_transaction_s	hfsplus_transaction_t;	/* Compound transaction type */


struct buffer_head;

struct hfsplus_jbd_head {
	/*
	 * Points back to our buffer_head. [jbd_lock_bh_hfsplus_jbd_head()]
	 */
	struct buffer_head *b_bh;

	/*
	 * Reference count - see description in journal.c
	 * [jbd_lock_bh_hfsplus_jbd_head()]
	 */
	int b_jcount;

	/*
	 * Journalling list for this buffer [jbd_lock_bh_state()]
	 */
	unsigned b_jlist;

	/*
	 * This flag signals the buffer has been modified by
	 * the currently running transaction
	 * [jbd_lock_bh_state()]
	 */
	unsigned b_modified;

	/*
	 * This feild tracks the last transaction id in which this buffer
	 * has been cowed
	 * [jbd_lock_bh_state()]
	 */
	hfsplus_jbd_tid_t b_cow_tid;

	/*
	 * Copy of the buffer data frozen for writing to the log.
	 * [jbd_lock_bh_state()]
	 */
	char *b_frozen_data;

	/*
	 * Pointer to a saved copy of the buffer containing no uncommitted
	 * deallocation references, so that allocations can avoid overwriting
	 * uncommitted deletes. [jbd_lock_bh_state()]
	 */
	char *b_committed_data;

	/*
	 * Pointer to the compound transaction which owns this buffer's
	 * metadata: either the running transaction or the committing
	 * transaction (if there is one).  Only applies to buffers on a
	 * transaction's data or metadata journaling list.
	 * [j_list_lock] [jbd_lock_bh_state()]
	 * Either of these locks is enough for reading, both are needed for
	 * changes.
	 */
	hfsplus_transaction_t *b_transaction;

	/*
	 * Pointer to the running compound transaction which is currently
	 * modifying the buffer's metadata, if there was already a transaction
	 * committing it when the new transaction touched it.
	 * [t_list_lock] [jbd_lock_bh_state()]
	 */
	hfsplus_transaction_t *b_next_transaction;

	/*
	 * Doubly-linked list of buffers on a transaction's data, metadata or
	 * forget queue. [t_list_lock] [jbd_lock_bh_state()]
	 */
	struct hfsplus_jbd_head *b_tnext, *b_tprev;

	/*
	 * Pointer to the compound transaction against which this buffer
	 * is checkpointed.  Only dirty buffers can be checkpointed.
	 * [j_list_lock]
	 */
	hfsplus_transaction_t *b_cp_transaction;

	/*
	 * Doubly-linked list of buffers still remaining to be flushed
	 * before an old transaction can be checkpointed.
	 * [j_list_lock]
	 */
	struct hfsplus_jbd_head *b_cpnext, *b_cpprev;

	/* Trigger type */
	struct jbd2_buffer_trigger_type *b_triggers;

	/* Trigger type for the committing transaction's frozen data */
	struct jbd2_buffer_trigger_type *b_frozen_triggers;
};

#endif		/* HFSPLUS_JOURNAL_HEAD_H_INCLUDED */
