#ifndef NEWLIB_STAT_H
#define NEWLIB_STAT_H

#include "common.h"

/* Historical newlib stat ABI with EE's 64-bit long. Native fstat writes mode
 * at +4 and block size at +0x48; see docs/sdk-matching.md for provenance. */
struct stat {
    s16 st_dev;
    u16 st_ino;
    u32 st_mode;
    u16 st_nlink, st_uid, st_gid;
    s16 st_rdev;
    long st_size, st_atime;
    long st_spare1, st_mtime;
    long st_spare2, st_ctime;
    long st_spare3, st_blksize, st_blocks;
    long st_spare4[2];
};

typedef char newlib_stat_size_check[sizeof(struct stat) == 0x68 ? 1 : -1];

#endif
