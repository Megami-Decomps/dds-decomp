#ifndef SDF_TEXTURE_RELEASE_H
#define SDF_TEXTURE_RELEASE_H

#include "common.h"

struct SdfMemBlock;

enum {
    SDF_TEX_RELEASE_GENERAL_ALLOCATION = 1,
    SDF_TEX_RELEASE_CHIP_ADDRESS = 2
};

/* Deferred texture release queue entry allocated as a 0xA0-byte chip block.
 * Only the first 0x0D bytes have known fields; the worker frees the complete
 * entry after releasing the selected resource. */
typedef struct SdfTexReleaseEntry {
    struct SdfTexReleaseEntry *next; /* 0x00 */
    void *chipMemory;                 /* 0x04: used for chip-heap releases */
    struct SdfMemBlock *allocation;  /* 0x08: used for general-heap releases */
    u8 releaseMode;                  /* 0x0C */
    u8 reserved0D[0x93];             /* 0x0D */
} SdfTexReleaseEntry;

typedef char SdfTexReleaseEntry_layout_must_match_native[
    (sizeof(SdfTexReleaseEntry) == 0xA0 &&
     (u32)&((SdfTexReleaseEntry *)0)->next == 0x00 &&
     (u32)&((SdfTexReleaseEntry *)0)->chipMemory == 0x04 &&
     (u32)&((SdfTexReleaseEntry *)0)->allocation == 0x08 &&
     (u32)&((SdfTexReleaseEntry *)0)->releaseMode == 0x0C)
        ? 1 : -1];

#endif /* SDF_TEXTURE_RELEASE_H */
