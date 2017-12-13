/* /home3/dni/haiyan.zhuang/r7800-test.git/build_dir/target-arm_v7-a_uClibc-0.9.33.2_eabi/samba-4.6.4/bin/default/source4/heimdal/lib/wind/map_table.h */
/* Automatically generated at 2017-09-30T09:16:06.768857 */

#ifndef MAP_TABLE_H
#define MAP_TABLE_H 1

#include "windlocl.h"

struct translation {
  uint32_t key;
  unsigned short val_len;
  unsigned short val_offset;
  wind_profile_flags flags;
};

extern const struct translation _wind_map_table[];

extern const size_t _wind_map_table_size;

extern const uint32_t _wind_map_table_val[];

#endif /* MAP_TABLE_H */
