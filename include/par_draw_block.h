#ifndef PAR_DRAW_BLOCK_H
#define PAR_DRAW_BLOCK_H

#include "common.h"

struct SdfAsset;
struct SdfMemBlock;

/* The allocator places this owner after its vertex and color arrays. */
typedef struct ParBlock {
    s32 count;                // 0x00
    u32 color;                // 0x04
    u128 *positions;          // 0x08: vertex quadwords
    u32 *colors;              // 0x0C: one packed color per vertex
    struct SdfAsset *asset;    // 0x10
    struct SdfMemBlock *allocation; // 0x14
} ParBlock;

typedef char ParBlockSizeCheck[sizeof(ParBlock) == 0x18 ? 1 : -1];
typedef char ParBlockAssetOffsetCheck[
    ((u32)&((ParBlock *)0)->asset == 0x10) ? 1 : -1];
typedef char ParBlockAllocationOffsetCheck[
    ((u32)&((ParBlock *)0)->allocation == 0x14) ? 1 : -1];

#endif
