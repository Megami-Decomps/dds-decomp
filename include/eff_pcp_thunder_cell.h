#ifndef EFF_PCP_THUNDER_CELL_H
#define EFF_PCP_THUNDER_CELL_H

#include "eff.h"

/* Per-particle countdown state in the paired thunder-cell implementations. */
typedef struct EffThunderCell {
    f32 directionX; /* 0x00 */
    f32 directionY; /* 0x04 */
    f32 directionZ; /* 0x08 */
    u32 delayFrames; /* 0x0C */
    u32 activeFrames; /* 0x10 */
} EffThunderCell;

/* Copied parameter prefix for the counted-down cell effect. */
typedef struct EffThunderCellParams {
    f32 origin[4];
    u16 systemParam;
    u8 pad12[2];
    f32 length;
    u32 cellCount;
    u8 opaque1C[8];
    u32 startDelayRange;
    u32 activeFrameRange;
    u16 halfLife;
    u8 pad2E[2];
    f32 coreWidth;
    u32 arg34;
    f32 bandWidth;
    u32 arg3C;
    f32 edgeWidth;
    u32 arg44;
} EffThunderCellParams;

/* Header followed by a variable-length EffThunderCell array in one allocation. */
typedef struct EffThunderCellWork {
    EffThunderCellParams head;
    EffThunderCell *cells; /* 0x48 */
    u32 unk4C; /* 0x4C: settable, otherwise unobserved */
    ParSystem *system; /* 0x50 */
    SdfMemBlock *allocationHandle; /* 0x54 */
} EffThunderCellWork;

typedef char EffThunderCell_size_must_be_0x14[
    sizeof(EffThunderCell) == 0x14 ? 1 : -1];
typedef char EffThunderCellParams_size_must_be_0x48[
    sizeof(EffThunderCellParams) == 0x48 ? 1 : -1];
typedef char EffThunderCellParams_cellCount_offset_must_be_0x18[
    ((u32)&((EffThunderCellParams *)0)->cellCount) == 0x18 ? 1 : -1];
typedef char EffThunderCellWork_size_must_be_0x58[
    sizeof(EffThunderCellWork) == 0x58 ? 1 : -1];
typedef char EffThunderCellWork_cells_offset_must_be_0x48[
    ((u32)&((EffThunderCellWork *)0)->cells) == 0x48 ? 1 : -1];
typedef char EffThunderCellWork_system_offset_must_be_0x50[
    ((u32)&((EffThunderCellWork *)0)->system) == 0x50 ? 1 : -1];
typedef char EffThunderCellWork_allocationHandle_offset_must_be_0x54[
    ((u32)&((EffThunderCellWork *)0)->allocationHandle) == 0x54 ? 1 : -1];

#endif
