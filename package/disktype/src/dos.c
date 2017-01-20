/*
 * dos.c
 * Detection of DOS parition maps and file systems
 *
 * Copyright (c) 2003-2006 Christoph Pfisterer
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 */

#include "global.h"

/*
 * DOS partition types
 *
 * Taken from fdisk/i386_sys_types.c and fdisk/common.h of
 * util-linux 2.11n (as packaged by Debian), Feb 08, 2003.
 */

struct systypes {
  unsigned char type;
  char *name;
};

struct systypes i386_sys_types[] = {
  { 0x00, "Empty" },
  { 0x01, "FAT12" },
  { 0x02, "XENIX root" },
  { 0x03, "XENIX usr" },
  { 0x04, "FAT16 <32M" },
  { 0x05, "Extended" },
  { 0x06, "FAT16" },
  { 0x07, "HPFS/NTFS" },
  { 0x08, "AIX" },
  { 0x09, "AIX bootable" },
  { 0x0a, "OS/2 Boot Manager" },
  { 0x0b, "Win95 FAT32" },
  { 0x0c, "Win95 FAT32 (LBA)" },
  { 0x0e, "Win95 FAT16 (LBA)" },
  { 0x0f, "Win95 Ext'd (LBA)" },
  { 0x10, "OPUS" },
  { 0x11, "Hidden FAT12" },
  { 0x12, "Compaq diagnostics" },
  { 0x14, "Hidden FAT16 <32M" },
  { 0x16, "Hidden FAT16" },
  { 0x17, "Hidden HPFS/NTFS" },
  { 0x18, "AST SmartSleep" },
  { 0x1b, "Hidden Win95 FAT32" },
  { 0x1c, "Hidden Win95 FAT32 (LBA)" },
  { 0x1e, "Hidden Win95 FAT16 (LBA)" },
  { 0x24, "NEC DOS" },
  { 0x39, "Plan 9" },
  { 0x3c, "PartitionMagic recovery" },
  { 0x40, "Venix 80286" },
  { 0x41, "PPC PReP Boot" },
  { 0x42, "SFS / MS LDM" },
  { 0x4d, "QNX4.x" },
  { 0x4e, "QNX4.x 2nd part" },
  { 0x4f, "QNX4.x 3rd part" },
  { 0x50, "OnTrack DM" },
  { 0x51, "OnTrack DM6 Aux1" },
  { 0x52, "CP/M" },
  { 0x53, "OnTrack DM6 Aux3" },
  { 0x54, "OnTrackDM6" },
  { 0x55, "EZ-Drive" },
  { 0x56, "Golden Bow" },
  { 0x5c, "Priam Edisk" },
  { 0x61, "SpeedStor" },
  { 0x63, "GNU HURD or SysV" },
  { 0x64, "Novell Netware 286" },
  { 0x65, "Novell Netware 386" },
  { 0x70, "DiskSecure Multi-Boot" },
  { 0x75, "PC/IX" },
  { 0x78, "XOSL" },
  { 0x80, "Old Minix" },
  { 0x81, "Minix / old Linux" },
  { 0x82, "Linux swap / Solaris" },
  { 0x83, "Linux" },
  { 0x84, "OS/2 hidden C: drive" },
  { 0x85, "Linux extended" },
  { 0x86, "NTFS volume set" },
  { 0x87, "NTFS volume set" },
  { 0x8e, "Linux LVM" },
  { 0x93, "Amoeba" },
  { 0x94, "Amoeba BBT" },
  { 0x9f, "BSD/OS" },
  { 0xa0, "IBM Thinkpad hibernation" },
  { 0xa5, "FreeBSD" },
  { 0xa6, "OpenBSD" },
  { 0xa7, "NeXTSTEP" },
  { 0xa9, "NetBSD" },
  { 0xaf, "Mac OS X" },
  { 0xb7, "BSDI fs" },
  { 0xb8, "BSDI swap" },
  { 0xbb, "Boot Wizard hidden" },
  { 0xc1, "DRDOS/sec (FAT-12)" },
  { 0xc4, "DRDOS/sec (FAT-16 < 32M)" },
  { 0xc6, "DRDOS/sec (FAT-16)" },
  { 0xc7, "Syrinx" },
  { 0xda, "Non-FS data" },
  { 0xdb, "CP/M / CTOS / ..." },
  { 0xde, "Dell Utility" },
  { 0xdf, "BootIt" },
  { 0xe1, "DOS access" },
  { 0xe3, "DOS R/O" },
  { 0xe4, "SpeedStor" },
  { 0xeb, "BeOS fs" },
  { 0xee, "EFI GPT protective" },
  { 0xef, "EFI System (FAT)" },
  { 0xf0, "Linux/PA-RISC boot" },
  { 0xf1, "SpeedStor" },
  { 0xf4, "SpeedStor" },
  { 0xf2, "DOS secondary" },
  { 0xfd, "Linux raid autodetect" },
  { 0xfe, "LANstep" },
  { 0xff, "BBT" },
  { 0, 0 }
};


char * get_name_for_mbrtype(int type)
{
  int i;

  for (i = 0; i386_sys_types[i].name; i++)
    if (i386_sys_types[i].type == type)
      return i386_sys_types[i].name;
  return "Unknown";
}

/*
 * DOS-style partition map / MBR
 */

static void detect_dos_partmap_ext(SECTION *section, u8 extbase,
				   int *extpartnum);

void detect_dos_partmap(SECTION *section)
{
  unsigned char *buf;
  int i, off, used, type, types[4], bootflags[4];
  u4 start, size, starts[4], sizes[4];
  int extpartnum = 5;
  char s[256], append[64];

  /* partition maps only occur at the start of a device */
  if (section->pos != 0)
    return;

  if (get_buffer(section, 0, 512, (void **)&buf) < 512)
    return;

  /* check signature */
  if (buf[510] != 0x55 || buf[511] != 0xAA)
    return;

  /* get entries and check */
  used = 0;
  for (off = 446, i = 0; i < 4; i++, off += 16) {
    /* get data */
    bootflags[i] = buf[off];
    types[i] = buf[off + 4];
    starts[i] = get_le_long(buf + off + 8);
    sizes[i] = get_le_long(buf + off + 12);

    /* bootable flag: either on or off */
    if (bootflags[i] != 0x00 && bootflags[i] != 0x80)
      return;
    /* size non-zero -> entry in use */
    if (starts[i] && sizes[i])
      used = 1;
  }
  if (!used)
    return;

  /* parse the data for real */
  print_line("DOS/MBR partition map");
  for (i = 0; i < 4; i++) {
    start = starts[i];
    size = sizes[i];
    type = types[i];
    if (start == 0 || size == 0)
      continue;

    sprintf(append, " from %lu", start);
    if (bootflags[i] == 0x80)
      strcat(append, ", bootable");
    format_blocky_size(s, size, 512, "sectors", append);
    print_line("\nPartition %d: %s",
	       i+1, s);

    print_line("Type 0x%02X (%s)", type, get_name_for_mbrtype(type));

    if (type == 0x05 || type == 0x0f || type == 0x85) {
      /* extended partition */
      detect_dos_partmap_ext(section, start, &extpartnum);
    }
  }
}

static void detect_dos_partmap_ext(SECTION *section, u8 extbase,
				   int *extpartnum)
{
  unsigned char *buf;
  u8 tablebase, nexttablebase;
  int i, off, type, types[4];
  u4 start, size, starts[4], sizes[4];
  char s[256], append[64];

  for (tablebase = extbase; tablebase; tablebase = nexttablebase) {
    /* read sector from linked list */
    if (get_buffer(section, tablebase << 9, 512, (void **)&buf) < 512)
      return;

    /* check signature */
    if (buf[510] != 0x55 || buf[511] != 0xAA) {
      print_line("Signature missing");
      return;
    }

    /* get entries */
    for (off = 446, i = 0; i < 4; i++, off += 16) {
      types[i] = buf[off + 4];
      starts[i] = get_le_long(buf + off + 8);
      sizes[i] = get_le_long(buf + off + 12);
    }

    /* parse the data for real */
    nexttablebase = 0;
    for (i = 0; i < 4; i++) {
      start = starts[i];
      size = sizes[i];
      type = types[i];
      if (size == 0)
	continue;

      if (type == 0x05 || type == 0x85) {
	/* inner extended partition */

	nexttablebase = extbase + start;

      } else {
	/* logical partition */

	sprintf(append, " from %llu+%lu", tablebase, start);
	format_blocky_size(s, size, 512, "sectors", append);
	print_line("\nPartition %d: %s",
		   *extpartnum, s);
	(*extpartnum)++;
	print_line("Type 0x%02X (%s)", type, get_name_for_mbrtype(type));

      }
    }
  }
}

/*
 * EFI GPT partition map
 */

struct gpttypes {
  char *guid;
  char *name;
};

struct gpttypes gpt_types[] = {
  { "\x28\x73\x2A\xC1\x1F\xF8\xD2\x11\xBA\x4B\x00\xA0\xC9\x3E\xC9\x3B", "EFI System (FAT)" },
  { "\x41\xEE\x4D\x02\xE7\x33\xD3\x11\x9D\x69\x00\x08\xC7\x81\xF3\x9F", "MBR partition scheme" },
  { "\x16\xE3\xC9\xE3\x5C\x0B\xB8\x4D\x81\x7D\xF9\x2D\xF0\x02\x15\xAE", "MS Reserved" },
  { "\xA2\xA0\xD0\xEB\xE5\xB9\x33\x44\x87\xC0\x68\xB6\xB7\x26\x99\xC7", "Basic Data" },
  { "\xAA\xC8\x08\x58\x8F\x7E\xE0\x42\x85\xD2\xE1\xE9\x04\x34\xCF\xB3", "MS LDM Metadata" },
  { "\xA0\x60\x9B\xAF\x31\x14\x62\x4F\xBC\x68\x33\x11\x71\x4A\x69\xAD", "MS LDM Data" },
  { "\x1E\x4C\x89\x75\xEB\x3A\xD3\x11\xB7\xC1\x7B\x03\xA0\x00\x00\x00", "HP/UX Data" },
  { "\x28\xE7\xA1\xE2\xE3\x32\xD6\x11\xA6\x82\x7B\x03\xA0\x00\x00\x00", "HP/UX Service" },
  { "\x0F\x88\x9D\xA1\xFC\x05\x3B\x4D\xA0\x06\x74\x3F\x0F\x84\x91\x1E", "Linux RAID" },
  { "\x6D\xFD\x57\x06\xAB\xA4\xC4\x43\x84\xE5\x09\x33\xC8\x4B\x4F\x4F", "Linux Swap" },
  { "\x79\xD3\xD6\xE6\x07\xF5\xC2\x44\xA2\x3C\x23\x8F\x2A\x3D\xF9\x28", "Linux LVM" },
  { "\x39\x33\xA6\x8D\x07\x00\xC0\x60\xC4\x36\x08\x3A\xC8\x23\x09\x08", "Linux Reserved" },
  { "\xB4\x7C\x6E\x51\xCF\x6E\xD6\x11\x8F\xF8\x00\x02\x2D\x09\x71\x2B", "FreeBSD Data" },
  { "\xB5\x7C\x6E\x51\xCF\x6E\xD6\x11\x8F\xF8\x00\x02\x2D\x09\x71\x2B", "FreeBSD Swap" },
  { "\xB6\x7C\x6E\x51\xCF\x6E\xD6\x11\x8F\xF8\x00\x02\x2D\x09\x71\x2B", "FreeBSD UFS" },
  { "\xB8\x7C\x6E\x51\xCF\x6E\xD6\x11\x8F\xF8\x00\x02\x2D\x09\x71\x2B", "FreeBSD Vinum" },
  { "\x00\x53\x46\x48\x00\x00\xAA\x11\xAA\x11\x00\x30\x65\x43\xEC\xAC", "Mac HFS+" },
  { 0, 0 }
};

static char * get_name_for_guid(void *guid)
{
  int i;

  for (i = 0; gpt_types[i].name; i++)
    if (memcmp(gpt_types[i].guid, guid, 16) == 0)
      return gpt_types[i].name;
  return "Unknown";
}

void detect_gpt_partmap(SECTION *section)
{
  unsigned char *buf,*back_buf;
  u8 diskblocks, partmap_start, start, end, size;
  u4 partmap_count, partmap_entry_size;
  u4 i;
  char s[256],back_s[256],append[64];
  int last_unused;
	
  /* partition maps only occur at the start of a device */
  if (section->pos != 0)
    return;

  /* get LBA 1: GPT header */
  if (get_buffer(section, 512, 512, (void **)&buf) < 512)
    return;

  /*GPT backup header LBA*/
  diskblocks = get_le_quad(buf + 0x20) + 1;

    /*get GPT backup header,the last LBA*/
  if (get_buffer(section, 512*(diskblocks-1), 512, (void **)&back_buf) < 512)
    return;

  /* check signature */
  if (memcmp(buf, "EFI PART", 8) != 0 || memcmp(back_buf, "EFI PART", 8) != 0)
    return;

  /* get header information */
  if (get_le_quad(buf + 0x18) != 1)
    return;
  partmap_start = get_le_quad(buf + 0x48);
  partmap_count = get_le_long(buf + 0x50);
  partmap_entry_size = get_le_long(buf + 0x54);

  print_line("\nGPT partition map, %d entries", (int)partmap_count);
  format_blocky_size(s, diskblocks, 512, "sectors", NULL);
  print_line("Disk size %s", s);
  format_guid(buf + 0x38, s);
  format_guid(back_buf + 0x38, back_s);

  /*If the GUID is not the same as the GUID in backup GPT,then return*/
  if(memcmp(s,back_s,16) != 0 )
    return;
  print_line("Disk GUID %s", s);

  /* get entries */
  last_unused = 0;
  for (i = 0; i < partmap_count; i++) {
    if (get_buffer(section, (partmap_start * 512) + i * partmap_entry_size, partmap_entry_size, (void **)&buf) < partmap_entry_size)
      return;

    if (memcmp(buf, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 16) == 0) {
      if (last_unused == 0)
      last_unused = 1;
      continue;
    }
    last_unused = 0;

    /* size */
    start = get_le_quad(buf + 0x20);
    end = get_le_quad(buf + 0x28);
    size = end + 1 - start;

    sprintf(append, " from %llu", start);
    format_blocky_size(s, size, 512, "sectors", append);
    print_line("\nPartition %d# %s", i+1, s);

    /* type */
    format_guid(buf, s);
    print_line("Type %s (GUID %s)", get_name_for_guid(buf), s);

    /* partition name */
    format_utf16_le(buf + 0x38, 72, s);
    //print_line("Partition Name \"%s\"", s);

    /* GUID */
    format_guid(buf + 0x10, s);
    print_line("Partition GUID %s", s);
  }
}

/* EOF */
