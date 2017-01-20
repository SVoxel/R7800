#
## Makefile for the linux hfsplus filesystem routines.
#

obj-$(CONFIG_HFSPLUS_JOURNAL_FS) += hfsplus.o

hfsplus-objs := super.o options.o inode.o ioctl.o extents.o catalog.o dir.o btree.o \
		bnode.o brec.o bfind.o tables.o unicode.o wrapper.o bitmap.o part_tbl.o journal.o \
		transaction.o commit.o recovery.o checkpoint.o revoke.o jbd_journal.o

