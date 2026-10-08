#ifndef SCE_IO_H
#define SCE_IO_H

#include "common.h"

/* Project spellings for the SCE iox layout, with retail's ordinary word alignment. */
typedef struct SceIoStat {
    u32 mode;
    u32 attributes;
    u32 size;
    u8 creationTime[8];
    u8 accessTime[8];
    u8 modificationTime[8];
    u32 highSize;
    u32 privateData[6];
} SceIoStat;

typedef struct SceDirent {
    SceIoStat stat;
    char name[0x100];
    void *privateData;
} SceDirent;

typedef char SceIoStat_size_must_be_0x40[
    (sizeof(SceIoStat) == 0x40) ? 1 : -1];
typedef char SceDirent_size_must_be_0x144[
    (sizeof(SceDirent) == 0x144) ? 1 : -1];
typedef char SceDirent_name_must_be_at_0x40[
    ((u32)&((SceDirent *)0)->name == 0x40) ? 1 : -1];
typedef char SceDirent_private_data_must_be_at_0x140[
    ((u32)&((SceDirent *)0)->privateData == 0x140) ? 1 : -1];

#endif /* SCE_IO_H */
