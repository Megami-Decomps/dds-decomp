#ifndef EFF_DIRENT_H
#define EFF_DIRENT_H

#include "common.h"

/* SDK directory entry filled by the directory-read provider. */
typedef struct EffDirEnt {
    u32 flags;
    u8 opaque04[0x3C];
    char name[0x100];
    void *privateData;
} EffDirEnt;

typedef char EffDirEntSizeAssert[(sizeof(EffDirEnt) == 0x144) ? 1 : -1];
typedef char EffDirEntLayoutAssert[
    ((u32)&((EffDirEnt *)0)->name == 0x40 &&
     (u32)&((EffDirEnt *)0)->privateData == 0x140) ? 1 : -1];

#endif
