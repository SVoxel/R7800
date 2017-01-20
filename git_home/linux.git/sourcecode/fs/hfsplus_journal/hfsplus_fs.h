/*
 *  linux/include/linux/hfsplus_fs.h
 *
 * Copyright (C) 1999
 * Brad Boyer (flar@pants.nu)
 * (C) 2003 Ardis Technologies <roman@ardistech.com>
 *
 */

#ifndef _LINUX_HFSPLUS_FS_H
#define _LINUX_HFSPLUS_FS_H

#include <linux/fs.h>
#include <linux/mutex.h>
#include <linux/buffer_head.h>
#include "hfsplus_jbd.h"
#include <linux/blkdev.h>
#include "hfsplus_raw.h"

#define DBG_BNODE_REFS	0x00000001
#define DBG_BNODE_MOD	0x00000002
#define DBG_CAT_MOD	0x00000004
#define DBG_INODE	0x00000008
#define DBG_SUPER	0x00000010
#define DBG_EXTENT	0x00000020
#define DBG_BITMAP	0x00000040
#define DBG_JOURNAL	0x00000080
#define DBG_JREPLAY	0x00000100
#define DBG_JTRANS	0x00000200
#define DBG_JCOMMIT	0x00000400
#define DBG_JCHKPT	0x00000800

#if 0
#define DBG_MASK	(DBG_EXTENT|DBG_INODE|DBG_BNODE_MOD)
#define DBG_MASK	(DBG_BNODE_MOD|DBG_CAT_MOD|DBG_INODE)
#define DBG_MASK	(DBG_CAT_MOD|DBG_BNODE_REFS|DBG_INODE|DBG_EXTENT)
#endif
//#define DBG_MASK	(0)
#define DBG_MASK		(DBG_JOURNAL|DBG_JREPLAY)

#define HFSPLUS_JOURNAL_PRESENT			1
#define HFSPLUS_JOURNAL_CONSISTENT		0
#define HFSPLUS_JOURNAL_INCONSISTENT	1
#define HFSPLUS_JOURNAL_UIBYTE			0x5A /* Unimportant byte value */
#define HFSPLUS_JOURNAL_SUCCESS			0
#define HFSPLUS_JOURNAL_FAIL				1
#define HFSPLUS_JOURNAL_SWAP				1

#define dprint(flg, fmt, args...) \
	if (flg & DBG_MASK) \
		printk(fmt , ## args)

/* Runtime config options */
#define HFSPLUS_DEF_CR_TYPE    0x3F3F3F3F  /* '????' */

#define HFSPLUS_TYPE_DATA 0x00
#define HFSPLUS_TYPE_RSRC 0xFF

typedef int (*btree_keycmp)(const hfsplus_btree_key *,
		const hfsplus_btree_key *);

#define NODE_HASH_SIZE	256

/* An HFS+ BTree held in memory */
struct hfs_btree {
	struct super_block *sb;
	struct inode *inode;
	btree_keycmp keycmp;

	u32 cnid;
	u32 root;
	u32 leaf_count;
	u32 leaf_head;
	u32 leaf_tail;
	u32 node_count;
	u32 free_nodes;
	u32 attributes;

	unsigned int node_size;
	unsigned int node_size_shift;
	unsigned int max_key_len;
	unsigned int depth;

	struct mutex tree_lock;

	unsigned int pages_per_bnode;
	spinlock_t hash_lock;
	struct hfs_bnode *node_hash[NODE_HASH_SIZE];
	int node_hash_cnt;
};

struct page;

/* An HFS+ BTree node in memory */
struct hfs_bnode {
	struct hfs_btree *tree;

	u32 prev;
	u32 this;
	u32 next;
	u32 parent;

	u16 num_recs;
	u8 type;
	u8 height;

	struct hfs_bnode *next_hash;
	unsigned long flags;
	wait_queue_head_t lock_wq;
	atomic_t refcnt;
	unsigned int page_offset;
	struct page *page[0];
};

#define HFS_BNODE_LOCK		0
#define HFS_BNODE_ERROR		1
#define HFS_BNODE_NEW		2
#define HFS_BNODE_DIRTY		3
#define HFS_BNODE_DELETED	4

/* An HFS+ Journal held in memory */
struct hfsplus_journal {
	struct semaphore jnl_lock;
	u32 journaled;
	u32 flags;

	/* Journal info block specific */
	struct buffer_head *jib_bh;
	struct hfsplus_journal_info_block *jibhdr;
	u64 jib_offset;

	/* Journal header specific */
	struct buffer_head *jh_bh;
	u32 jh_bh_size;
	u64 jh_offset;
	struct hfsplus_journal_header *jhdr;

	/* block number of meta-data */
	u32 ext_block;
	u32 alloc_block;
	u32 catalog_block;
	u32 attr_block;

	/* Pointer of JBD */
	struct hfsplus_jbd_s *s_journal;

	/* Maximum number of blocks allowed in a single transaction */
	u16 journal_maxblock;

	struct super_block *sbp;
};

/*
 * HFS+ superblock info (built from Volume Header on disk)
 */

struct hfsplus_vh;
struct hfs_btree;

struct hfsplus_sb_info {
	void *s_vhdr_buf;
	struct hfsplus_vh *s_vhdr;
	void *s_backup_vhdr_buf;
	struct hfsplus_vh *s_backup_vhdr;
	struct hfs_btree *ext_tree;
	struct hfs_btree *cat_tree;
	struct hfs_btree *attr_tree;
	struct inode *alloc_file;
	struct inode *hidden_dir;
	struct nls_table *nls;

	/* Runtime variables */
	u32 blockoffset;
	sector_t part_start;
	sector_t sect_count;
	int fs_shift;

	/* immutable data from the volume header */
	struct hfsplus_journal jnl;
	u32 alloc_blksz;
	int alloc_blksz_shift;
	u32 total_blocks;
	u32 data_clump_blocks, rsrc_clump_blocks;

	/* mutable data from the volume header, protected by alloc_mutex */
	u32 free_blocks;
	struct mutex alloc_mutex;

	/* mutable data from the volume header, protected by vh_mutex */
	u32 next_cnid;
	u32 file_count;
	u32 folder_count;
	struct mutex vh_mutex;

	/* Config options */
	u32 creator;
	u32 type;

	umode_t umask;
	uid_t uid;
	gid_t gid;

	int part, session;

	unsigned long flags;
};

#define HFSPLUS_SB_WRITEBACKUP	0
#define HFSPLUS_SB_NODECOMPOSE	1
#define HFSPLUS_SB_FORCE	2
#define HFSPLUS_SB_HFSX		3
#define HFSPLUS_SB_CASEFOLD	4
#define HFSPLUS_SB_NOBARRIER	5

static inline struct hfsplus_sb_info *HFSPLUS_SB(struct super_block *sb)
{
	return sb->s_fs_info;
}


struct hfsplus_inode_info {
	atomic_t opencnt;

	/*
	 * Extent allocation information, protected by extents_lock.
	 */
	u32 first_blocks;
	u32 clump_blocks;
	u32 alloc_blocks;
	u32 cached_start;
	u32 cached_blocks;
	hfsplus_extent_rec first_extents;
	hfsplus_extent_rec cached_extents;
	unsigned int extent_state;
	struct mutex extents_lock;

	/*
	 * Immutable data.
	 */
	struct inode *rsrc_inode;
	__be32 create_date;

	/*
	 * Protected by sbi->vh_mutex.
	 */
	u32 linkid;

	/*
	 * Accessed using atomic bitops.
	 */
	unsigned long flags;

	/*
	 * Protected by i_mutex.
	 */
	sector_t fs_blocks;
	u8 userflags;		/* BSD user file flags */
	struct list_head open_dir_list;
	loff_t phys_size;

	struct inode vfs_inode;
};

#define HFSPLUS_EXT_DIRTY	0x0001
#define HFSPLUS_EXT_NEW		0x0002

#define HFSPLUS_I_RSRC		0	/* represents a resource fork */
#define HFSPLUS_I_CAT_DIRTY	1	/* has changes in the catalog tree */
#define HFSPLUS_I_EXT_DIRTY	2	/* has changes in the extent tree */
#define HFSPLUS_I_ALLOC_DIRTY	3	/* has changes in the allocation file */

#define HFSPLUS_IS_RSRC(inode) \
	test_bit(HFSPLUS_I_RSRC, &HFSPLUS_I(inode)->flags)

static inline struct hfsplus_inode_info *HFSPLUS_I(struct inode *inode)
{
	return list_entry(inode, struct hfsplus_inode_info, vfs_inode);
}

/*
 * Mark an inode dirty, and also mark the btree in which the
 * specific type of metadata is stored.
 * For data or metadata that gets written back by into the catalog btree
 * by hfsplus_write_inode a plain mark_inode_dirty call is enough.
 */
static inline void hfsplus_mark_inode_dirty(struct inode *inode,
		unsigned int flag)
{
	set_bit(flag, &HFSPLUS_I(inode)->flags);
	mark_inode_dirty(inode);
}

struct hfs_find_data {
	/* filled by caller */
	hfsplus_btree_key *search_key;
	hfsplus_btree_key *key;
	/* filled by find */
	struct hfs_btree *tree;
	struct hfs_bnode *bnode;
	/* filled by findrec */
	int record;
	int keyoffset, keylength;
	int entryoffset, entrylength;
};

struct hfsplus_readdir_data {
	struct list_head list;
	struct file *file;
	struct hfsplus_cat_key key;
};

/*
 * Find minimum acceptible I/O size for an hfsplus sb.
 */
static inline unsigned short hfsplus_min_io_size(struct super_block *sb)
{
	return max_t(unsigned short, bdev_logical_block_size(sb->s_bdev),
		     HFSPLUS_SECTOR_SIZE);
}

#define hfs_btree_open hfsplus_btree_open
#define hfs_btree_close hfsplus_btree_close
#define hfs_btree_write hfsplus_btree_write
#define hfs_bmap_alloc hfsplus_bmap_alloc
#define hfs_bmap_free hfsplus_bmap_free
#define hfs_bnode_read hfsplus_bnode_read
#define hfs_bnode_read_u16 hfsplus_bnode_read_u16
#define hfs_bnode_read_u8 hfsplus_bnode_read_u8
#define hfs_bnode_read_key hfsplus_bnode_read_key
#define hfs_bnode_write hfsplus_bnode_write
#define hfs_bnode_write_u16 hfsplus_bnode_write_u16
#define hfs_bnode_clear hfsplus_bnode_clear
#define hfs_bnode_copy hfsplus_bnode_copy
#define hfs_bnode_move hfsplus_bnode_move
#define hfs_bnode_dump hfsplus_bnode_dump
#define hfs_bnode_unlink hfsplus_bnode_unlink
#define hfs_bnode_findhash hfsplus_bnode_findhash
#define hfs_bnode_find hfsplus_bnode_find
#define hfs_bnode_unhash hfsplus_bnode_unhash
#define hfs_bnode_free hfsplus_bnode_free
#define hfs_bnode_create hfsplus_bnode_create
#define hfs_bnode_get hfsplus_bnode_get
#define hfs_bnode_put hfsplus_bnode_put
#define hfs_brec_lenoff hfsplus_brec_lenoff
#define hfs_brec_keylen hfsplus_brec_keylen
#define hfs_brec_insert hfsplus_brec_insert
#define hfs_brec_remove hfsplus_brec_remove
#define hfs_find_init hfsplus_find_init
#define hfs_find_exit hfsplus_find_exit
#define __hfs_brec_find __hplusfs_brec_find
#define hfs_brec_find hfsplus_brec_find
#define hfs_brec_read hfsplus_brec_read
#define hfs_brec_goto hfsplus_brec_goto
#define hfs_part_find hfsplus_part_find

/*
 * definitions for ext2 flag ioctls (linux really needs a generic
 * interface for this).
 */

/* ext2 ioctls (EXT2_IOC_GETFLAGS and EXT2_IOC_SETFLAGS) to support
 * chattr/lsattr */
#define HFSPLUS_IOC_EXT2_GETFLAGS	FS_IOC_GETFLAGS
#define HFSPLUS_IOC_EXT2_SETFLAGS	FS_IOC_SETFLAGS


/*
 * hfs+-specific ioctl for making the filesystem bootable
 */
#define HFSPLUS_IOC_BLESS _IO('h', 0x80)

/*
 * Functions in any *.c used in other files
 */

/* bitmap.c */
int hfsplus_block_allocate(hfsplus_handle_t *, struct super_block *, u32, u32, u32 *);
int hfsplus_block_free(hfsplus_handle_t *, struct super_block *, u32, u32);

/* btree.c */
struct hfs_btree *hfs_btree_open(struct super_block *, u32);
void hfs_btree_close(struct hfs_btree *);
void hfs_btree_write(hfsplus_handle_t *, struct hfs_btree *);
struct hfs_bnode *hfs_bmap_alloc(hfsplus_handle_t *, struct hfs_btree *);
void hfs_bmap_free(hfsplus_handle_t *, struct hfs_bnode *);

/* bnode.c */
void hfs_bnode_read(struct hfs_bnode *, void *, int, int);
u16 hfs_bnode_read_u16(struct hfs_bnode *, int);
u8 hfs_bnode_read_u8(struct hfs_bnode *, int);
void hfs_bnode_read_key(struct hfs_bnode *, void *, int);
void hfs_bnode_write(hfsplus_handle_t *, struct hfs_bnode *, void *, int, int);
void hfs_bnode_write_u16(hfsplus_handle_t *, struct hfs_bnode *, int, u16);
void hfs_bnode_clear(hfsplus_handle_t *, struct hfs_bnode *, int, int);
void hfs_bnode_copy(hfsplus_handle_t *, struct hfs_bnode *, int,
		    struct hfs_bnode *, int, int);
void hfs_bnode_move(hfsplus_handle_t *, struct hfs_bnode *, int, int, int);
void hfs_bnode_dump(struct hfs_bnode *);
void hfs_bnode_unlink(hfsplus_handle_t *, struct hfs_bnode *);
struct hfs_bnode *hfs_bnode_findhash(struct hfs_btree *, u32);
struct hfs_bnode *hfs_bnode_find(hfsplus_handle_t *, struct hfs_btree *, u32);
void hfs_bnode_unhash(struct hfs_bnode *);
void hfs_bnode_free(struct hfs_bnode *);
struct hfs_bnode *hfs_bnode_create(hfsplus_handle_t *, struct hfs_btree *, u32);
void hfs_bnode_get(struct hfs_bnode *);
void hfs_bnode_put(hfsplus_handle_t *, struct hfs_bnode *);

/* brec.c */
u16 hfs_brec_lenoff(struct hfs_bnode *, u16, u16 *);
u16 hfs_brec_keylen(struct hfs_bnode *, u16);
int hfs_brec_insert(hfsplus_handle_t *, struct hfs_find_data *, void *, int);
int hfs_brec_remove(hfsplus_handle_t *, struct hfs_find_data *);

/* bfind.c */
int hfs_find_init(struct hfs_btree *, struct hfs_find_data *);
void hfs_find_exit(hfsplus_handle_t *, struct hfs_find_data *);
int hfsplus_journalled_find_init(struct hfs_btree *, struct hfs_find_data *);
void hfsplus_journalled_find_exit(hfsplus_handle_t *, struct hfs_find_data *);
int __hfs_brec_find(struct hfs_bnode *, struct hfs_find_data *);
int hfs_brec_find(hfsplus_handle_t *, struct hfs_find_data *);
int hfs_brec_read(hfsplus_handle_t *, struct hfs_find_data *, void *, int);
int hfs_brec_goto(hfsplus_handle_t *, struct hfs_find_data *, int);

/* catalog.c */
int hfsplus_cat_case_cmp_key(const hfsplus_btree_key *,
		const hfsplus_btree_key *);
int hfsplus_cat_bin_cmp_key(const hfsplus_btree_key *,
		const hfsplus_btree_key *);
void hfsplus_cat_build_key(struct super_block *sb,
		hfsplus_btree_key *, u32, struct qstr *);
int hfsplus_find_cat(hfsplus_handle_t *, struct super_block *, u32, struct hfs_find_data *);
int hfsplus_create_cat(hfsplus_handle_t *, u32, struct inode *, struct qstr *, struct inode *);
int hfsplus_delete_cat(hfsplus_handle_t *, u32, struct inode *, struct qstr *);
int hfsplus_rename_cat(hfsplus_handle_t *, u32, struct inode *, struct qstr *,
		       struct inode *, struct qstr *);
void hfsplus_cat_set_perms(struct inode *inode, struct hfsplus_perm *perms);

/* dir.c */
extern const struct inode_operations hfsplus_dir_inode_operations;
extern const struct file_operations hfsplus_dir_operations;

/* extents.c */
int hfsplus_ext_cmp_key(const hfsplus_btree_key *, const hfsplus_btree_key *);
int hfsplus_ext_write_extent(hfsplus_handle_t *, struct inode *);
int hfsplus_get_block(struct inode *, sector_t, struct buffer_head *, int);
int hfsplus_free_fork(hfsplus_handle_t *, struct super_block *, u32,
		struct hfsplus_fork_raw *, int);
int hfsplus_file_extend(hfsplus_handle_t *, struct inode *);
void hfsplus_file_truncate(struct inode *);

/* inode.c */
extern const struct address_space_operations hfsplus_aops;
extern const struct address_space_operations hfsplus_btree_aops;
extern const struct dentry_operations hfsplus_dentry_operations;
extern const struct address_space_operations hfsplus_journalled_btree_aops;
extern const struct address_space_operations hfsplus_journalled_aops;

void hfsplus_inode_read_fork(struct inode *, struct hfsplus_fork_raw *);
void hfsplus_inode_write_fork(struct inode *, struct hfsplus_fork_raw *);
int hfsplus_cat_read_inode(struct inode *, struct hfs_find_data *);
int hfsplus_cat_write_inode(hfsplus_handle_t *, struct inode *);
struct inode *hfsplus_new_inode(hfsplus_handle_t *, struct super_block *, umode_t);
void hfsplus_delete_inode(hfsplus_handle_t *, struct inode *);
int hfsplus_file_fsync(struct file *file, loff_t start, loff_t end,
		       int datasync);
int hfsplus_do_journal_get_write_access(hfsplus_handle_t *, struct buffer_head *);
int hfsplus_commit_write_fn(hfsplus_handle_t *, struct buffer_head *);
int hfsplus_walk_page_buffers(hfsplus_handle_t *, struct buffer_head *, unsigned ,
				unsigned , int *, int (*fn)(hfsplus_handle_t *, struct buffer_head *));

/* ioctl.c */
long hfsplus_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
int hfsplus_setxattr(struct dentry *dentry, const char *name,
		     const void *value, size_t size, int flags);
ssize_t hfsplus_getxattr(struct dentry *dentry, const char *name,
			 void *value, size_t size);
ssize_t hfsplus_listxattr(struct dentry *dentry, char *buffer, size_t size);

/* options.c */
int hfsplus_parse_options(char *, struct hfsplus_sb_info *);
int hfsplus_parse_options_remount(char *input, int *force);
void hfsplus_fill_defaults(struct hfsplus_sb_info *);
int hfsplus_show_options(struct seq_file *, struct dentry *);

/* super.c */
struct inode *hfsplus_iget(struct super_block *, unsigned long);
int hfsplus_sync_fs(struct super_block *sb, int wait);

/* tables.c */
extern u16 hfsplus_case_fold_table[];
extern u16 hfsplus_decompose_table[];
extern u16 hfsplus_compose_table[];

/* unicode.c */
int hfsplus_strcasecmp(const struct hfsplus_unistr *,
		const struct hfsplus_unistr *);
int hfsplus_strcmp(const struct hfsplus_unistr *,
		const struct hfsplus_unistr *);
int hfsplus_uni2asc(struct super_block *,
		const struct hfsplus_unistr *, char *, int *);
int hfsplus_asc2uni(struct super_block *,
		struct hfsplus_unistr *, const char *, int);
int hfsplus_hash_dentry(const struct dentry *dentry,
		const struct inode *inode, struct qstr *str);
int hfsplus_compare_dentry(const struct dentry *parent,
		const struct inode *pinode,
		const struct dentry *dentry, const struct inode *inode,
		unsigned int len, const char *str, const struct qstr *name);

/* wrapper.c */
int hfsplus_read_wrapper(struct super_block *);
int hfs_part_find(struct super_block *, sector_t *, sector_t *);
int hfsplus_submit_bio(struct super_block *sb, sector_t sector,
		void *buf, void **data, int rw);

/* journal.c */
void hfsplus_journalled_init(struct super_block *, struct hfsplus_vh *);
void hfsplus_journalled_deinit(struct super_block *);
int hfsplus_journalled_create(struct super_block *);
int hfsplus_journalled_check(struct super_block *);
void print_volume_header(struct super_block *);
void swap_block_list_header(struct hfsplus_blhdr *);
hfsplus_jbd_t * hfsplus_get_dev_journal(struct super_block *);
int hfsplus_journalled_mark_inode_dirty(const char *, hfsplus_handle_t *, struct inode *);
int hfsplus_journalled_set_page_dirty(hfsplus_handle_t *, struct page *);
int hfsplus_journal_start(const char *, struct super_block *sb, hfsplus_handle_t *hfsplus_handle);
int hfsplus_journal_stop(hfsplus_handle_t *);
int hfsplus_journal_get_write_access(const char *, hfsplus_handle_t *, struct buffer_head *);
int hfsplus_journal_dirty_metadata(const char *, struct buffer_head *, hfsplus_handle_t *);
int hfsplus_force_commit(struct super_block *);
void hfsplus_jhdr_checksum_calculate(hfsplus_jbd_t *);
void hfsplus_journal_header_start_update(hfsplus_jbd_t *, unsigned long);
void hfsplus_journal_header_end_update(hfsplus_jbd_t *, struct hfsplus_journal_header *);
void hfsplus_journal_mark_journal_empty(hfsplus_jbd_t *);
void hfsplus_test_block_list_header(const char *, struct hfsplus_journal_header *, struct hfsplus_journal *);

#define sb_bread512(sb, sec, data) ({                   \
        struct buffer_head *__bh;                       \
        sector_t __block;                               \
        loff_t __start;                                 \
        int __offset;                                   \
                                                        \
        __start = (loff_t)(sec) << HFSPLUS_SECTOR_SHIFT;\
        __block = __start >> (sb)->s_blocksize_bits;    \
        __offset = __start & ((sb)->s_blocksize - 1);   \
        __bh = sb_bread((sb), __block);                 \
        if (likely(__bh != NULL))                       \
                data = (void *)(__bh->b_data + __offset);\
        else                                            \
                data = NULL;                            \
        __bh;                                           \
})

/* time macros */
#define __hfsp_mt2ut(t)		(be32_to_cpu(t) - 2082844800U)
#define __hfsp_ut2mt(t)		(cpu_to_be32(t + 2082844800U))

/* compatibility */
#define hfsp_mt2ut(t)		(struct timespec){ .tv_sec = __hfsp_mt2ut(t) }
#define hfsp_ut2mt(t)		__hfsp_ut2mt((t).tv_sec)
#define hfsp_now2mt()		__hfsp_ut2mt(get_seconds())

#endif
