/*
* HFSPlus journal implementation Tathagata Das 2010
*/

#include <linux/fs.h>
#include <linux/blkdev.h>
#include <linux/pagemap.h>
#include <linux/slab.h>

#include <asm/current.h>
#include <asm/unaligned.h>

#include "hfsplus_jbd.h"
#include "hfsplus_fs.h"
#include "hfsplus_raw.h"

/* Calculate chesum of ptr of size len */
static int calc_checksum(unsigned char *ptr, int len)
{
	int i, chksum = 0;

	for (i=0; i<len; i++, ptr++)
		chksum = (chksum << 8) ^ (chksum + *ptr);

	return (~chksum);
}

void swap_block_list_header(struct hfsplus_blhdr *blhdr)
{
	int i;

	blhdr->num_blocks = swab16(blhdr->num_blocks);
	blhdr->bytes_used = swab32(blhdr->bytes_used);
	blhdr->checksum = swab32(blhdr->checksum);

	for (i=1; i<blhdr->num_blocks; i++) {
		blhdr->binfo[i].bnum = swab64(blhdr->binfo[i].bnum);
		blhdr->binfo[i].bsize = swab32(blhdr->binfo[i].bsize);
	}
}

static void swap_journal_header(struct hfsplus_journal_header *jh)
{
	jh->magic      = swab32(jh->magic);
	jh->endian     = swab32(jh->endian);
	jh->start      = swab64(jh->start);
	jh->end        = swab64(jh->end);
	jh->size       = swab64(jh->size);
	jh->blhdr_size = swab32(jh->blhdr_size);
	jh->checksum   = swab32(jh->checksum);
	jh->jhdr_size  = swab32(jh->jhdr_size);
}

void print_volume_header(struct super_block *sb)
{
	int i;
	unsigned char *vh_ptr = (unsigned char *)HFSPLUS_SB(sb)->s_vhdr;

	dprint(DBG_JOURNAL, "VOLUME HEADER\n");
	for (i=0; i<102; i++)
		dprint(DBG_JOURNAL, "%x ", vh_ptr[i]);
	dprint(DBG_JOURNAL, "\n");
}

static void print_journal_header(struct hfsplus_journal_header *jh)
{
	dprint(DBG_JOURNAL, "HFS+-fs: magic: %x\n endian: %x\n start: %llx\n end: %llx\n size: %llx\n blhdr_size: %x\n checksum: %x\n jhdr_size: %x\n", jh->magic, jh->endian, jh->start, jh->end, jh->size, jh->blhdr_size, jh->checksum, jh->jhdr_size);
}

static int map_journal_header(struct super_block *sb)
{
	struct hfsplus_journal *jnl = &(HFSPLUS_SB(sb)->jnl);
	u32 jh_block_number;

	jnl->jh_offset = be64_to_cpu(jnl->jibhdr->offset);
	jh_block_number = jnl->jh_offset >> sb->s_blocksize_bits;
	dprint(DBG_JOURNAL, "HFS+-fs: jh_block_number: %x\n", jh_block_number);
	jnl->jh_bh = sb_bread(sb, HFSPLUS_SB(sb)->blockoffset + jh_block_number);
	if (!jnl->jh_bh) {
		printk("HFS+-fs Line=%d: Error in buffer read\n", __LINE__);
		return HFSPLUS_JOURNAL_FAIL;
	}
	jnl->jhdr = (struct hfsplus_journal_header *)(jnl->jh_bh->b_data);

	return HFSPLUS_JOURNAL_SUCCESS;
}

/* START: JBD specfic functions */
static void hfsplus_abort(struct super_block * sb, const char * function,
		 const char * fmt, ...)
{
	va_list args;

	printk (KERN_ERR "hfsplus_abort called.\n");

	va_start(args, fmt);
	printk(KERN_ERR "HFS+-fs error (device %s): %s: ",sb->s_id, function);
	vprintk(fmt, args);
	printk("\n");
	va_end(args);

	if (sb->s_flags & MS_RDONLY)
		return;

	printk(KERN_ERR "Remounting filesystem read-only\n");
	sb->s_flags |= MS_RDONLY;
	hfsplus_jbd_abort(HFSPLUS_SB(sb)->jnl.s_journal, -EIO);
}

static void hfsplus_journal_abort_handle(const char *err_no, struct buffer_head *bh, hfsplus_handle_t *hfsplus_handle)
{
	if (hfsplus_jbd_is_handle_aborted(hfsplus_handle->handle))
		return;

	printk(KERN_ERR "HFS+-fs: Aborting transaction\n");
	hfsplus_jbd_abort_handle(hfsplus_handle->handle);
}

int hfsplus_journal_get_write_access(const char *err_fn, hfsplus_handle_t *hfsplus_handle, struct buffer_head *bh)
{
	int err;

	if (hfsplus_handle->journaled != HFSPLUS_JOURNAL_PRESENT)
		return HFSPLUS_JOURNAL_SUCCESS;

	err = hfsplus_jbd_get_write_access(hfsplus_handle->handle, bh);
	if (err) {
		printk(KERN_ERR "HFS+-fs: %s() unable to get journal write access\n", err_fn);
		hfsplus_journal_abort_handle(err_fn, bh, hfsplus_handle);
	}

	return err;
}

int hfsplus_journal_dirty_metadata(const char *err_fn, struct buffer_head *bh, hfsplus_handle_t *hfsplus_handle)
{
	int err;
	
	if (hfsplus_handle->journaled != HFSPLUS_JOURNAL_PRESENT)
		return HFSPLUS_JOURNAL_SUCCESS;

	err = hfsplus_jbd_dirty_metadata(hfsplus_handle->handle, bh);
	if (err) {
		if (!hfsplus_handle->handle->h_err)
			hfsplus_handle->handle->h_err = err;

		if (hfsplus_jbd_is_handle_aborted(hfsplus_handle->handle))
			return err;

		printk(KERN_ERR "HFS+-fs: %s() aborting transaction:\n", err_fn);

		hfsplus_jbd_abort_handle(hfsplus_handle->handle);
	} else
		hfsplus_handle->hcnt++;

	return err;
}

/*
 * Force the running and committing transactions to commit,
 * and wait on the commit.
 */
int hfsplus_force_commit(struct super_block *sb)
{
	hfsplus_jbd_t *journal;
	int ret;
	
	if (sb->s_flags & MS_RDONLY)
		return 0;

	journal = HFSPLUS_SB(sb)->jnl.s_journal; 
	sb->s_dirt = 0;
	ret = hfsplus_jbd_force_commit(journal); 
	return ret;
}

int hfsplus_journal_start(const char *err_fn, struct super_block *sb, hfsplus_handle_t *hfsplus_handle)
{
	hfsplus_jbd_t *journal = HFSPLUS_SB(sb)->jnl.s_journal;

	hfsplus_handle->journaled = HFSPLUS_SB(sb)->jnl.journaled;
	if ((hfsplus_handle->journaled != HFSPLUS_JOURNAL_PRESENT) || (journal == NULL)) {
		hfsplus_handle->handle = NULL;
		dprint(DBG_JTRANS, "HFS+-fs: Journal was not loaded\n");
		return HFSPLUS_JOURNAL_SUCCESS;
	}

	if (is_hfsplus_jbd_aborted(journal)) {
		hfsplus_abort(sb, __FUNCTION__, "Detected aborted journal");
		return -EROFS;
	}

	hfsplus_handle->hcnt = 0;
	hfsplus_handle->maxblock = HFSPLUS_SB(sb)->jnl.journal_maxblock;
	hfsplus_handle->handle = hfsplus_jbd_start(journal, HFSPLUS_SB(sb)->jnl.journal_maxblock, hfsplus_handle);
	if (IS_ERR(hfsplus_handle->handle)) {
		printk(KERN_ERR "HFS+fs: journal cannot be started from %s. Error number: %ld\n", err_fn, PTR_ERR(hfsplus_handle->handle));
		return PTR_ERR(hfsplus_handle->handle);
	}

	return HFSPLUS_JOURNAL_SUCCESS;
}

int hfsplus_journal_stop(hfsplus_handle_t *hfsplus_handle)
{
	if ((hfsplus_handle->journaled != HFSPLUS_JOURNAL_PRESENT) || (hfsplus_handle->handle == NULL)) {
		return HFSPLUS_JOURNAL_SUCCESS;
	}

	return hfsplus_jbd_stop(hfsplus_handle->handle);
}

/* Fill in the initial static fields in the new journal superblock */
static int hfsplus_journal_superblock_init(hfsplus_jbd_t *journal, int start)
{
	hfsplus_jbd_superblock_t *jsb;
	//struct buffer_head *bh;
	//unsigned long blocknr;
	//int i;
   
	if (journal->j_superblock == NULL)
		return HFSPLUS_JOURNAL_FAIL;

	jsb = journal->j_superblock;
	memset(jsb->mac_padding, 0, sizeof(jsb->mac_padding));

	if (jsb->s_header.h_magic == cpu_to_be32(JFS_MAGIC_NUMBER)) {
		dprint(DBG_JOURNAL, "HFS+-fs: Journal superblock is already initialized\n");
	}

	dprint(DBG_JOURNAL, "HFS+-fs: Zeroing out journal blocks...\n");
#if 0 
   for (i = start; i < journal->j_maxlen; i++) {
		blocknr = i;
      bh = __getblk(journal->j_dev, blocknr, journal->j_blocksize);
      lock_buffer(bh);
		if (i == start)
      	memset(bh->b_data + HFSPLUS_SECTOR_SIZE, 0, journal->j_blocksize - HFSPLUS_SECTOR_SIZE);
		else
      	memset(bh->b_data, 0, journal->j_blocksize);
      mark_buffer_dirty(bh);
      set_buffer_uptodate(bh);
      unlock_buffer(bh);
      __brelse(bh);
   }
#endif

   sync_blockdev(journal->j_dev);
   dprint(DBG_JOURNAL, "HFS+-fs: journal cleared.\n");

	jsb->s_header.h_magic  = cpu_to_be32(JFS_MAGIC_NUMBER);
	jsb->s_header.h_blocktype = cpu_to_be32(JFS_SUPERBLOCK_V2);

	jsb->s_blocksize   = cpu_to_be32(journal->j_blocksize);
	jsb->s_maxlen   = cpu_to_be32(journal->j_maxlen);
	jsb->s_first = cpu_to_be32(start+1); /* First block is for Mac's journal header and JBD superblock */

	journal->j_transaction_sequence = 1;

	journal->j_flags &= ~JFS_ABORT;
	journal->j_format_version = 2;
#if 1
	jsb->s_feature_ro_compat = cpu_to_be32(JFS_KNOWN_ROCOMPAT_FEATURES);
	jsb->s_feature_incompat = cpu_to_be32(JFS_KNOWN_INCOMPAT_FEATURES);
#endif

	/* Update journal superblock on disk and wait for the IO to complete.*/
	hfsplus_jbd_update_superblock(journal, 1);

	return HFSPLUS_JOURNAL_SUCCESS;
}

hfsplus_jbd_t * hfsplus_get_dev_journal(struct super_block *sb)
{
	struct hfsplus_journal *jnl = &(HFSPLUS_SB(sb)->jnl);
	int start, len, blocksize;
	hfsplus_jbd_t *journal = NULL;
	struct hfsplus_journal_header *jh;

	jnl->journal_maxblock = (jnl->jhdr->blhdr_size / sizeof(struct hfsplus_block_info)) - 1;
	jh = (struct hfsplus_journal_header *)(HFSPLUS_SB(sb)->jnl.jhdr);
	blocksize = sb->s_blocksize;
	start = (int)(jnl->jh_offset)/blocksize;
	len = (int)(jh->size + jnl->jh_offset)/blocksize; 
	jnl->journal_maxblock = 512;
	dprint(DBG_JOURNAL, "start: %d, len: %d, blocksize: %d, journal_maxblock: %d\n", start, len, blocksize, jnl->journal_maxblock); 
	journal = hfsplus_jbd_init_dev(sb->s_bdev, sb->s_bdev, start, len, blocksize);

	if (journal) {
		dprint(DBG_JOURNAL, "HFS+-fs: Able to create device journal\n");
		journal->j_private = sb;
		ll_rw_block(READ, 1, &journal->j_sb_buffer);
		wait_on_buffer(journal->j_sb_buffer);
		if (!buffer_uptodate(journal->j_sb_buffer)) {
			printk(KERN_ERR "HFS+-fs: I/O error on journal device\n");
			goto out_journal;
		}

		/* Initialize journal superblock for the first time. 
		 * Otherwise hfsplus_jbd_load will fail. 
		*/
		if (hfsplus_journal_superblock_init(journal, start) == HFSPLUS_JOURNAL_SUCCESS) {
			dprint(DBG_JOURNAL, "HFS+-fs: success in initializeing journal superblock.\n");
		} else {
			printk(KERN_ERR "HFS+-fs: error in initializeing journal superblock.\n");
			goto out_journal;
		}
	}
		
	return journal;

out_journal:
	hfsplus_jbd_destroy(journal);
	return NULL;
}

/* Caller should lock the page before calling this function */
static int hfsplus_journalled_write_full_page(hfsplus_handle_t *hfsplus_handle, struct page *page)
{
	int err, ret;

	//ret = block_prepare_write(page, 0, PAGE_CACHE_SIZE, hfsplus_get_block);
	//if (ret) {
	//	printk("%s: Error in block_prepare_write\n", __FUNCTION__);
	//	return ret;
	//}

	ret = hfsplus_walk_page_buffers(hfsplus_handle, page_buffers(page), 0,
		PAGE_CACHE_SIZE, NULL, hfsplus_do_journal_get_write_access);

	err = hfsplus_walk_page_buffers(hfsplus_handle, page_buffers(page), 0,
			PAGE_CACHE_SIZE, NULL, hfsplus_commit_write_fn);
	if (ret == 0)
		ret = err;

	return ret;
}

int hfsplus_journalled_set_page_dirty(hfsplus_handle_t *hfsplus_handle, struct page *page)
{
	if (hfsplus_handle->journaled != HFSPLUS_JOURNAL_PRESENT) {
		set_page_dirty(page);
		return HFSPLUS_JOURNAL_SUCCESS;
	}

	/* Page contains metadata or data */
	if (page->mapping->a_ops == &hfsplus_journalled_btree_aops) {
		if (hfsplus_handle->hcnt < hfsplus_handle->maxblock) {
			int ret;

			lock_page(page);
			ret = hfsplus_journalled_write_full_page(hfsplus_handle, page);
			unlock_page(page);
			return ret;
		} else {
			//printk("BTREE: HFS+-fs: Count exceeds (count %d, maxblock: %d)\n", hfsplus_handle->hcnt++, hfsplus_handle->maxblock);
			set_page_dirty(page);
			return HFSPLUS_JOURNAL_SUCCESS;
		}
	}
	else if (page->mapping->a_ops == &hfsplus_journalled_aops) {
      if (hfsplus_handle->hcnt < hfsplus_handle->maxblock) {
         int ret;

         lock_page(page);
         ret = hfsplus_journalled_write_full_page(hfsplus_handle, page);
         unlock_page(page);
         return ret;
      } else {
			printk("HFS+-fs: Count exceeds (count %d, maxblock: %d)\n", hfsplus_handle->hcnt++, hfsplus_handle->maxblock);
         set_page_dirty(page);
         return HFSPLUS_JOURNAL_SUCCESS;
      }
   } else {
		set_page_dirty(page);
		return HFSPLUS_JOURNAL_SUCCESS;
	}
}

int hfsplus_journalled_mark_inode_dirty(const char *err_fn, hfsplus_handle_t *hfsplus_handle, struct inode *inode)
{
	struct hfsplus_vh *vhdr;
	int ret = 0;
	
	if (inode->i_ino != 3)
	if (hfsplus_handle->journaled != HFSPLUS_JOURNAL_PRESENT) {
		mark_inode_dirty(inode);
		return HFSPLUS_JOURNAL_SUCCESS;
	}

	dprint(DBG_JTRANS, "hfsplus_journalled_mark_inode_dirty: %lu, called from: %s\n", inode->i_ino, err_fn);
	hfsplus_ext_write_extent(hfsplus_handle, inode);
	if (inode->i_ino >= HFSPLUS_FIRSTUSER_CNID) {
		return hfsplus_cat_write_inode(hfsplus_handle, inode);
	}
	vhdr = HFSPLUS_SB(inode->i_sb)->s_vhdr;
	switch (inode->i_ino) {
	case HFSPLUS_ROOT_CNID:
		ret = hfsplus_cat_write_inode(hfsplus_handle, inode);
		break;
	case HFSPLUS_EXT_CNID:
		if (vhdr->ext_file.total_size != cpu_to_be64(inode->i_size)) {
			HFSPLUS_SB(inode->i_sb)->flags |= HFSPLUS_SB_WRITEBACKUP;
			inode->i_sb->s_dirt = 1;
		}
		hfsplus_inode_write_fork(inode, &vhdr->ext_file);
		hfs_btree_write(hfsplus_handle, HFSPLUS_SB(inode->i_sb)->ext_tree);
		break;
	case HFSPLUS_CAT_CNID:
		if (vhdr->cat_file.total_size != cpu_to_be64(inode->i_size)) {
			HFSPLUS_SB(inode->i_sb)->flags |= HFSPLUS_SB_WRITEBACKUP;
			inode->i_sb->s_dirt = 1;
		}
		hfsplus_inode_write_fork(inode, &vhdr->cat_file);
		hfs_btree_write(hfsplus_handle, HFSPLUS_SB(inode->i_sb)->cat_tree);
		break;
	case HFSPLUS_ALLOC_CNID:
		if (vhdr->alloc_file.total_size != cpu_to_be64(inode->i_size)) {
			HFSPLUS_SB(inode->i_sb)->flags |= HFSPLUS_SB_WRITEBACKUP;
			inode->i_sb->s_dirt = 1;
		}
		hfsplus_inode_write_fork(inode, &vhdr->alloc_file);
		break;
	case HFSPLUS_START_CNID:
		if (vhdr->start_file.total_size != cpu_to_be64(inode->i_size)) {
			HFSPLUS_SB(inode->i_sb)->flags |= HFSPLUS_SB_WRITEBACKUP;
			inode->i_sb->s_dirt = 1;
		}
		hfsplus_inode_write_fork(inode, &vhdr->start_file);
		break;
	case HFSPLUS_ATTR_CNID:
		if (vhdr->attr_file.total_size != cpu_to_be64(inode->i_size)) {
			HFSPLUS_SB(inode->i_sb)->flags |= HFSPLUS_SB_WRITEBACKUP;
			inode->i_sb->s_dirt = 1;
		}
		hfsplus_inode_write_fork(inode, &vhdr->attr_file);
		hfs_btree_write(hfsplus_handle, HFSPLUS_SB(inode->i_sb)->attr_tree);
		break;
	}
	if (ret) 
		printk(KERN_ERR "HFS+-fs: [%s] unable to do hfsplus_journalled_mark_inode_dirty()\n", err_fn);
	return ret;
}

void hfsplus_jhdr_checksum_calculate(hfsplus_jbd_t *journal)
{
	struct hfsplus_journal_header *jh = &(journal->j_superblock->mac_jh);

	jh->checksum = 0;
	jh->checksum = calc_checksum((unsigned char *)jh, sizeof(struct hfsplus_journal_header));
}

void hfsplus_journal_header_start_update(hfsplus_jbd_t *journal, unsigned long freed)
{
	struct super_block *sb = journal->j_private;
	hfsplus_jbd_superblock_t *jsb = journal->j_superblock;
	struct hfsplus_journal_header *jh = &(jsb->mac_jh);
	__be32 mac_jh_start_blocknum = (jh->start + HFSPLUS_SB(sb)->jnl.jh_offset) >> sb->s_blocksize_bits;

	dprint(DBG_JCHKPT, "jh->start:%llx, jh_offset: %llx, mac_jh_start_blocknum: %x, freed: %lx, last: %x, j_first: %x\n", 
		jh->start, HFSPLUS_SB(sb)->jnl.jh_offset, mac_jh_start_blocknum, freed, journal->j_last, journal->j_first); 
	if ((mac_jh_start_blocknum + freed) < journal->j_last) {
		jh->start += freed * sb->s_blocksize;
	}
	else {
		jh->start = (journal->j_first + (freed - (journal->j_last - mac_jh_start_blocknum)))*sb->s_blocksize;
	}
}

void inline hfsplus_journal_header_end_update(hfsplus_jbd_t *journal, struct hfsplus_journal_header *jh)
{
	struct super_block *sb = journal->j_private;

	jh->end = ((journal->j_head - journal->j_first) * sb->s_blocksize) + jh->blhdr_size;
}

void hfsplus_journal_mark_journal_empty(hfsplus_jbd_t *journal)
{
	struct hfsplus_journal_header *jh = &(journal->j_superblock->mac_jh);

	jh->start = jh->end;
	jh->checksum = 0;
	jh->checksum = calc_checksum((unsigned char *)jh, sizeof(struct hfsplus_journal_header));
}
/* END: JBD specfic functions */


/* Write journal header during replay */
static int hfsplus_replay_write_journal_header(struct super_block *sb)
{
	struct hfsplus_journal_header *jh = (struct hfsplus_journal_header *)(HFSPLUS_SB(sb)->jnl.jhdr);

	jh->start = sb->s_blocksize;
	jh->end = sb->s_blocksize;
	jh->blhdr_size = sb->s_blocksize;

	if (HFSPLUS_SB(sb)->jnl.flags == HFSPLUS_JOURNAL_SWAP) {
		swap_journal_header(jh);
		jh->checksum = 0;
		jh->checksum = swab32(calc_checksum((unsigned char *)jh, sizeof(struct hfsplus_journal_header)));
	}
	else {
		jh->checksum = 0;
		jh->checksum = calc_checksum((unsigned char *)jh, sizeof(struct hfsplus_journal_header));
	}

	/* Write it to disk */
	mark_buffer_dirty(HFSPLUS_SB(sb)->jnl.jh_bh);
	sync_dirty_buffer(HFSPLUS_SB(sb)->jnl.jh_bh);

	if (HFSPLUS_SB(sb)->jnl.flags == HFSPLUS_JOURNAL_SWAP)
		swap_journal_header(jh);

	return HFSPLUS_JOURNAL_SUCCESS;
}

static int hfsplus_write_journal_header(struct super_block *sb)
{
	struct hfsplus_journal_header *jh = (struct hfsplus_journal_header *)(HFSPLUS_SB(sb)->jnl.jhdr);

	jh->checksum = 0;
	jh->checksum = calc_checksum((unsigned char *)jh, sizeof(struct hfsplus_journal_header));

	/* Write it to disk */
	mark_buffer_dirty(HFSPLUS_SB(sb)->jnl.jh_bh);

	return HFSPLUS_JOURNAL_SUCCESS;
}

/* Create journal header, journal buffer and initialize them 
 * Assume that presence of journal is already verified
*/
int hfsplus_journalled_create(struct super_block *sb)
{
	struct hfsplus_journal_header *jhdr; 
	u64 jibsize = be64_to_cpu(HFSPLUS_SB(sb)->jnl.jibhdr->offset);

	dprint(DBG_JOURNAL, "sb->s_blocksize: %lx, jibsize: %llx\n", sb->s_blocksize, jibsize);

	/* Journal size is not aligned */
	if (((jibsize >> sb->s_blocksize_bits) << sb->s_blocksize_bits) != jibsize) {
		printk("HFS+-fs: journal size is not aligned\n");
		return HFSPLUS_JOURNAL_FAIL;
	}

	if (map_journal_header(sb) == HFSPLUS_JOURNAL_FAIL) {
		printk("HFS+-fs: Error in mapping journal header\n");
		return HFSPLUS_JOURNAL_FAIL;
	}

	jhdr = (struct hfsplus_journal_header *)HFSPLUS_SB(sb)->jnl.jhdr;

	/* Populate journal header and write it to the disk */
	jhdr->magic = HFSPLUS_JOURNAL_HEADER_MAGIC;
	jhdr->endian = HFSPLUS_JOURNAL_HEADER_ENDIAN;
	jhdr->start = sb->s_blocksize; /* First block is for journal header itself */
	jhdr->end = sb->s_blocksize; /* Initially journal buffer is empty */
	jhdr->size = 0x800000; /* Default size of journal log is 8MB */
	jhdr->blhdr_size = sb->s_blocksize;
	jhdr->jhdr_size = sb->s_blocksize; /* Assign first block for journal header */

	if (jhdr->start != jhdr->end) {
		printk("HFS+-fs: hfsplus_write_journal_header fail: Journal is not empty\n");
		return HFSPLUS_JOURNAL_FAIL;
	}

	return hfsplus_write_journal_header(sb);
}

void hfsplus_test_block_list_header(const char *func, struct hfsplus_journal_header *jh, struct hfsplus_journal *jnl) 
{
	u32 start_block_number;
	struct buffer_head *blhdr_bh = NULL;
	struct hfsplus_blhdr *blhdr;
	struct super_block *sb = jnl->sbp;

	printk("called from %s()\n", func);
	start_block_number = (jh->start + jnl->jh_offset) >> sb->s_blocksize_bits;
	blhdr_bh = sb_bread(sb, start_block_number);
	if (!blhdr_bh) {
		printk("HFS+-fs Line=%d: Error in read\n", __LINE__);
		return;
	}
	blhdr = (hfsplus_blhdr_t *)blhdr_bh->b_data;
	if (jnl->flags == HFSPLUS_JOURNAL_SWAP)
		swap_block_list_header(blhdr);
	printk("start block number: %x, max_blocks: %x, num_blocks: %x, bytes_used: %x\n", start_block_number, blhdr->max_blocks, blhdr->num_blocks, blhdr->bytes_used);
	brelse(blhdr_bh);
}

/* If the journal consists transaction then write them to disk.
 * Return success if it brings the file system into consistent state.
 * Otherwise return fail.
*/
static int hfsplus_journal_replay(struct super_block *sb)
{
	struct hfsplus_journal *jnl = &(HFSPLUS_SB(sb)->jnl);
	struct buffer_head *blhdr_bh = NULL, *tr_bh = NULL, *disk_bh = NULL;
	struct hfsplus_blhdr *blhdr;
	u32 start_sector_number, tr_sector_number, disk_sector_number, i, ret = HFSPLUS_JOURNAL_FAIL; 
	u64 tr_offset, disk_offset;
	struct hfsplus_journal_header *jh = (struct hfsplus_journal_header *)(HFSPLUS_SB(sb)->jnl.jhdr);
	unsigned char *tr_buf, *disk_buf;
	__be32 bufsize;

	if (jh->start == jh->end) {
		dprint(DBG_JREPLAY, "HFS+-fs: Journal is empty, nothing to replay\n");
		ret = hfsplus_replay_write_journal_header(sb);
		return ret;
	}

	if ((jh->start > jh->size) || (jh->end > jh->size)) {
		printk("HFS+-fs: Wrong start or end offset, start: %llx, end: %llx, jh_offset: %llx, size: %llx\n", jh->start, jh->end, jnl->jh_offset, jh->size);
		return ret;
	}

	//hfsplus_test_block_list_header(__FUNCTION__, jh, jnl);
	
	if (jh->start == jh->size)
		jh->start = jh->jhdr_size; 

	down(&jnl->jnl_lock);
	/* Go through each transaction */
	while (jh->start != jh->end) {
		if (blhdr_bh)
			brelse(blhdr_bh);

		start_sector_number = (jh->start + jnl->jh_offset) >> HFSPLUS_SECTOR_SHIFT;
		dprint(DBG_JREPLAY, "start: %llx, start_sector_number: %x\n", jh->start, start_sector_number);
		blhdr_bh = sb_bread512(sb, HFSPLUS_SB(sb)->blockoffset + start_sector_number, blhdr);
		if (!blhdr_bh) {
			printk("HFS+-fs Line=%d: Error in read\n", __LINE__);
			up(&jnl->jnl_lock);
			return ret;
		}

		if (jnl->flags == HFSPLUS_JOURNAL_SWAP)
			swap_block_list_header(blhdr);

		dprint(DBG_JREPLAY, "HFS+-fs: num_blocks: %x, bytes_used: %x\n", blhdr->num_blocks, blhdr->bytes_used);
		/* Point to the second block in the Volume, first block is already in block list header */
		tr_offset = jnl->jh_offset + jh->start + jh->blhdr_size;

		for (i=1; i<blhdr->num_blocks; i++) {
			bufsize = blhdr->binfo[i].bsize;
			disk_offset = blhdr->binfo[i].bnum << HFSPLUS_SECTOR_SHIFT;

			dprint(DBG_JREPLAY, "[i:%x] bnum: %llx, bsize: %x, bufsize: %x, blocksize: %lx\n", i, blhdr->binfo[i].bnum, blhdr->binfo[i].bsize, bufsize, sb->s_blocksize);

			while (bufsize > 0) {
				/* Read one block */
				tr_sector_number = tr_offset >> HFSPLUS_SECTOR_SHIFT; 
				dprint(DBG_JREPLAY, "tr_sector_number: %x, tr_offset: %llx\n", tr_sector_number, tr_offset); 
				tr_bh = sb_bread512(sb, HFSPLUS_SB(sb)->blockoffset + tr_sector_number, tr_buf);
				if (!tr_bh) {
					printk("HFS+-fs Line=%d: Error in read\n", __LINE__);
					if (blhdr_bh)
						brelse(blhdr_bh);
					up(&jnl->jnl_lock);
					return ret;
				}

				disk_sector_number = disk_offset >> HFSPLUS_SECTOR_SHIFT;
				dprint(DBG_JREPLAY, "disk_sector_number: %x, disk_offset: %llx, bufsize: %x\n", disk_sector_number, disk_offset, bufsize); 
				/* Read the same sector from the Volume */
				disk_bh = sb_bread512(sb, HFSPLUS_SB(sb)->blockoffset + disk_sector_number, disk_buf);
				if (!disk_bh) {
					printk("HFS+-fs Line=%d: Error in read\n", __LINE__);
					if (blhdr_bh)
						brelse(blhdr_bh);
					if (tr_bh)
						brelse(tr_bh);
					up(&jnl->jnl_lock);
					return ret;
				}

				/* Write transaction block to the disk block in sector wise */
				memcpy(disk_buf, tr_buf, HFSPLUS_SECTOR_SIZE);
				mark_buffer_dirty(disk_bh);
				sync_dirty_buffer(disk_bh);

				/* Free buffer heads */
				brelse(disk_bh);
				brelse(tr_bh);

				tr_offset += HFSPLUS_SECTOR_SIZE;
				disk_offset += HFSPLUS_SECTOR_SIZE;
				bufsize -= HFSPLUS_SECTOR_SIZE;

				/* Check tr_offset reaches at the end of journal buffer */
				if (tr_offset == (jnl->jh_offset + jh->size)) {
					printk("tr_offset: %llx, jh->size: %llx, jh_offset: %llx\n", tr_offset, jh->size, jnl->jh_offset);
					tr_offset = jnl->jh_offset + jh->jhdr_size; /* Set to the beginning of journal buffer */
				}
			}
		}

		/* Check position of start index, wrap around if necessary */
		if ((jh->start + blhdr->bytes_used) < jh->size)
			jh->start += blhdr->bytes_used;
		else
			jh->start = jh->jhdr_size + (jh->start + blhdr->bytes_used) - jh->size;
	}

	if (blhdr_bh)
		brelse(blhdr_bh);

	if (jh->start == jh->end) {
		ret = hfsplus_replay_write_journal_header(sb);
	} else {
		printk("HFS+-fs: %s Error in journal replay\n", __func__);
	}

	/* Populate Volume Header with new values */
	if (ret == HFSPLUS_JOURNAL_SUCCESS && HFSPLUS_SB(sb)->s_vhdr) {
		struct hfsplus_vh *vhdr = HFSPLUS_SB(sb)->s_vhdr;
		struct buffer_head *bh;

		dprint(DBG_JREPLAY, "Populate Volume Header again\n");
		HFSPLUS_SB(sb)->s_vhdr->attributes |= cpu_to_be32(HFSPLUS_VOL_UNMNT);
		//mark_buffer_dirty(HFSPLUS_SB(sb)->s_vhbh);
		//sync_dirty_buffer(HFSPLUS_SB(sb)->s_vhbh);

		//if (HFSPLUS_SB(sb)->s_vhbh)
		//	brelse(HFSPLUS_SB(sb)->s_vhbh);

		bh = sb_bread512(sb, HFSPLUS_SB(sb)->blockoffset + HFSPLUS_VOLHEAD_SECTOR, vhdr);
		if (!bh) {
			printk("HFS+-fs Line=%d: Error in read\n", __LINE__);
			HFSPLUS_SB(sb)->s_vhdr = NULL;
			up(&jnl->jnl_lock);
			return HFSPLUS_JOURNAL_FAIL;
		}

		/* should still be the same... */
		if (be16_to_cpu(vhdr->signature) != HFSPLUS_VOLHEAD_SIG) {
			printk("Volume header signature (%x) is wrong\n", be16_to_cpu(vhdr->signature));
			brelse(bh);
			HFSPLUS_SB(sb)->s_vhdr = NULL;
			up(&jnl->jnl_lock);
			return HFSPLUS_JOURNAL_FAIL;
		}

		//HFSPLUS_SB(sb)->s_vhbh = bh;
		HFSPLUS_SB(sb)->s_vhdr = vhdr;
	}

	up(&jnl->jnl_lock);
	return ret;
}

/* Check consistency of journal log file in hfsplus volume 
*/
int hfsplus_journalled_check(struct super_block *sb)
{
	struct hfsplus_journal_info_block *jib;
	struct hfsplus_journal_header *jh;
	u32 checksum, org_checksum;

	print_volume_header(sb);

	if (HFSPLUS_SB(sb)->jnl.journaled != HFSPLUS_JOURNAL_PRESENT) {
		printk("HFS+-fs: Journal is not present\n");
		return HFSPLUS_JOURNAL_CONSISTENT;
	}

	jib = (struct hfsplus_journal_info_block *)(HFSPLUS_SB(sb)->jnl.jibhdr);
	dprint(DBG_JOURNAL, "HFS+-fs: be32_to_cpu(jib->flags): %x\n", be32_to_cpu(jib->flags));

	/* Journal is on another volume, and the "on this volume" flag
	* isn't set 
	*/
	if(be32_to_cpu(jib->flags) & HFSPLUS_JOURNAL_ON_OTHER_DEVICE &&
		!(be32_to_cpu(jib->flags) & HFSPLUS_JOURNAL_IN_FS)) {
		printk("HFS+-fs: Unable to access the journal.\n");
		return HFSPLUS_JOURNAL_INCONSISTENT;
	}

	/* Journal should be created in initialization. 
	* Mark inconsistent if the journal is still not created yet 
	*/
	if (be32_to_cpu(jib->flags) & HFSPLUS_JOURNAL_NEED_INIT) {
		printk("HFS+-fs: Error, journal is not created\n");
		return HFSPLUS_JOURNAL_INCONSISTENT;
	}

	dprint(DBG_JOURNAL, "HFS+-fs: Found Info Block and verified successfully.\n");
	jh = (struct hfsplus_journal_header *)(HFSPLUS_SB(sb)->jnl.jhdr);

	org_checksum = jh->checksum;
	jh->checksum = 0;

	if (jh->magic == swab32(HFSPLUS_JOURNAL_HEADER_MAGIC)) {
		org_checksum = swab32(org_checksum);
		checksum = calc_checksum((unsigned char *)jh, sizeof(struct hfsplus_journal_header));
		swap_journal_header(jh);
		HFSPLUS_SB(sb)->jnl.flags = HFSPLUS_JOURNAL_SWAP;
	}
	else
		checksum = calc_checksum((unsigned char *)jh, sizeof(struct hfsplus_journal_header));

	print_journal_header(jh);

	/* Verify the journal header */
	if(jh->magic != HFSPLUS_JOURNAL_HEADER_MAGIC || jh->endian != HFSPLUS_JOURNAL_HEADER_ENDIAN){
		printk("HFS+-fs: Journal header verification failed.\n");
		return HFSPLUS_JOURNAL_INCONSISTENT;
	}

	if (checksum != org_checksum) {
		jh->checksum = checksum;
		printk("HFS+-fs: Error in journal header checksum checksum: %x, org_checksum: %x\n", checksum, org_checksum);
		return HFSPLUS_JOURNAL_INCONSISTENT;
	}
	jh->checksum = checksum;

	dprint(DBG_JOURNAL, "HFS+-fs: No problem in magic number, endian and checksum\n");

	/* Compare start to end */
	if(jh->start == jh->end) {
		hfsplus_replay_write_journal_header(sb);
		/* If they're the same, we can mount, it's clean */
		printk("HFS+-fs: Journal is empty means consistent\n");
		return HFSPLUS_JOURNAL_CONSISTENT;
	} else {
		/* Replay journal and bring the file system in consistent state */
		if (hfsplus_journal_replay(sb) == HFSPLUS_JOURNAL_FAIL) {
			/* Unable to replay */
			printk("HFS+-fs: Journal is non empty means inconsistent, please run fsck.hfsplus\n");
			return HFSPLUS_JOURNAL_INCONSISTENT;
		} else
			dprint(DBG_JOURNAL, "HFS+-fs: Journal replay done\n");
	}

	return HFSPLUS_JOURNAL_CONSISTENT;
}

/* Check journal present or not and initialize hfsplus_journal accordingly 
 * Assume that super block and volume header are already initialized
*/
void hfsplus_journalled_init(struct super_block *sb, struct hfsplus_vh *vhdr)
{
	struct hfsplus_journal *jnl = &(HFSPLUS_SB(sb)->jnl);
	u32 jib_flags;
	int n;

	jnl->journaled = !HFSPLUS_JOURNAL_PRESENT; /* Initialize as non-journaled */
	jnl->sbp = NULL;
	jnl->jh_bh = NULL;
	jnl->alloc_block = be32_to_cpu(vhdr->alloc_file.extents[0].start_block);
	jnl->ext_block = be32_to_cpu(vhdr->ext_file.extents[0].start_block);
	jnl->catalog_block = be32_to_cpu(vhdr->cat_file.extents[0].start_block);
	dprint(DBG_JOURNAL, "alloc_block: %x, ext_block: %x, catalog_block: %x\n", jnl->alloc_block, jnl->ext_block, jnl->catalog_block);

	if (vhdr->attributes & cpu_to_be32(HFSPLUS_VOL_JOURNALED)) {
		dprint(DBG_JOURNAL,"HFS+-fs: Journaled filesystem\n");
		jnl->jib_offset = be32_to_cpu(vhdr->journal_info_block);
		n = be32_to_cpu(vhdr->blocksize)/sb->s_blocksize;
		//printk("############ n: %d , blocksize: %x, sb->s_blocksize: %x, ##########\n", n, be32_to_cpu(vhdr->blocksize), sb->s_blocksize);
		jnl->jib_offset = jnl->jib_offset * n;
		//dprint(DBG_JOURNAL, "HFS+-fs: jib_offset: %x, blockoffset: %x\n", jnl->jib_offset, HFSPLUS_SB(sb)->blockoffset);
		/* Check the journal info block to find the block # of the journal */
		jnl->jib_bh = sb_bread(sb, HFSPLUS_SB(sb)->blockoffset + jnl->jib_offset);
		if (!jnl->jib_bh) {
			printk("HFS+-fs Line=%d: Error in buffer read\n", __LINE__);
			return;
		}
		jnl->jibhdr = (struct hfsplus_journal_info_block *)(jnl->jib_bh->b_data);
		jib_flags = be32_to_cpu(jnl->jibhdr->flags);
		dprint(DBG_JOURNAL, "HFS+-fs: jib_flags: %x\n", jib_flags);
		if ((jib_flags & HFSPLUS_JOURNAL_ON_OTHER_DEVICE) && !(jib_flags & HFSPLUS_JOURNAL_IN_FS))
			goto init_fail;
 
		dprint(DBG_JOURNAL, "HFS+-fs: jib size: %x\n", be32_to_cpu(jnl->jibhdr->size));
		if (jib_flags & HFSPLUS_JOURNAL_NEED_INIT) {
			dprint(DBG_JOURNAL, "HFS+-fs: Journal is not created\n");
			if (hfsplus_journalled_create(sb) == 0) {
				HFSPLUS_SB(sb)->jnl.jibhdr->flags &= be32_to_cpu(~HFSPLUS_JOURNAL_NEED_INIT);
				/* write it to disk */
				mark_buffer_dirty(HFSPLUS_SB(sb)->jnl.jib_bh);
				sync_dirty_buffer(HFSPLUS_SB(sb)->jnl.jib_bh);
			} else {
				printk("HFS+-fs: Fail to create journal\n");
				goto init_fail;
			}
		}

		/* Check already initialize in journal create */
		if (jnl->jh_bh == NULL) {
			if (map_journal_header(sb) == HFSPLUS_JOURNAL_FAIL) {
				printk("HFS+-fs Line=%d: Error in buffer read\n", __LINE__);
				goto init_fail; 
			}
		}

		//mutex_init(&jnl->jnl_lock);
		jnl->sbp = sb;
		jnl->flags = !HFSPLUS_JOURNAL_SWAP;
		jnl->journaled = HFSPLUS_JOURNAL_PRESENT;
	}

	return;

init_fail:
	printk("HFS+-fs: Journal initialization fails\n");
	if (jnl->jib_bh)
		brelse(jnl->jib_bh);
}

/* Deinitialize journal if it is present */
void hfsplus_journalled_deinit(struct super_block *sb)
{
	if (HFSPLUS_SB(sb)->jnl.journaled != HFSPLUS_JOURNAL_PRESENT) {
		return;
	}

	if (HFSPLUS_SB(sb)->jnl.s_journal != NULL) {
		hfsplus_jbd_destroy(HFSPLUS_SB(sb)->jnl.s_journal);
	}

	if (HFSPLUS_SB(sb)->jnl.jib_bh)
		brelse(HFSPLUS_SB(sb)->jnl.jib_bh);

	if (HFSPLUS_SB(sb)->jnl.jh_bh)
		brelse(HFSPLUS_SB(sb)->jnl.jh_bh);
	
	HFSPLUS_SB(sb)->jnl.journaled = !HFSPLUS_JOURNAL_PRESENT;
}
