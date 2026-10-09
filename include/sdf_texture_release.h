#ifndef SDF_TEXTURE_RELEASE_H
#define SDF_TEXTURE_RELEASE_H

#include "common.h"

struct SdfMemBlock;

enum {
    SDF_TEX_RELEASE_GENERAL_ALLOCATION = 1,
    SDF_TEX_RELEASE_CHIP_ADDRESS = 2
};

/* Shared release metadata precedes both fixed nodes and variable upload packets. */
typedef struct SdfTextureReleaseHead {
    struct SdfTextureReleaseHead *next; /* 0x00 */
    void *chipMemory;                   /* 0x04 */
    struct SdfMemBlock *allocation;     /* 0x08 */
    u8 releaseMode;                     /* 0x0C */
    u8 reserved0D[3];
} SdfTextureReleaseHead;

/* Standalone deferred releases allocate 0xA0 bytes; upload packets use only
 * the common prefix and retain their separately calculated allocation size. */
typedef struct SdfTexReleaseEntry {
    SdfTextureReleaseHead release;
    u8 reserved10[0x90];
} SdfTexReleaseEntry;

typedef char SdfTextureReleaseHead_layout[
    (sizeof(SdfTextureReleaseHead) == 0x10 &&
     (u32)&((SdfTextureReleaseHead *)0)->next == 0x00 &&
     (u32)&((SdfTextureReleaseHead *)0)->chipMemory == 0x04 &&
     (u32)&((SdfTextureReleaseHead *)0)->allocation == 0x08 &&
     (u32)&((SdfTextureReleaseHead *)0)->releaseMode == 0x0C) ? 1 : -1];
typedef char SdfTexReleaseEntry_layout_must_match_native[
    (sizeof(SdfTexReleaseEntry) == 0xA0 &&
     (u32)&((SdfTexReleaseEntry *)0)->release == 0x00) ? 1 : -1];

#endif /* SDF_TEXTURE_RELEASE_H */
