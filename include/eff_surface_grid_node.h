#ifndef EFF_SURFACE_GRID_NODE_H
#define EFF_SURFACE_GRID_NODE_H

#include "common.h"
#include "sdf.h"

/* Surface grid node: the record at the end of a retained block, after the vertex rows. */
#ifdef VERSION_DDS1
typedef struct EffSurfaceGridNode {
    u8 pad_00[0x10];
    s32 rows;       // 0x10
    s32 field_14;   // 0x14
    s32 field_18;   // 0x18
    s32 columns;    // 0x1C
    u32 type;       // 0x20
    u128 *buffer;     // 0x24
    u32 *tail;       // 0x28
    SdfAsset *handle; // 0x2C
    u128 *queueA;     // 0x30
    u32 *queueB;     // 0x34
    struct SdfMemBlock *allocation; // 0x38
} EffSurfaceGridNode;

typedef char EffSurfaceGridNodeSizeCheck[(sizeof(EffSurfaceGridNode) == 0x3C) ? 1 : -1];
#else
typedef struct EffSurfaceGridNode {
    u8 pad_00[0x20];
    s32 rows;           // 0x20
    s32 field_24;       // 0x24
    s32 field_28;       // 0x28
    s32 columns;        // 0x2C
    u32 type;           // 0x30
    u128 *buffer;         // 0x34
    u32 *tail;           // 0x38
    SdfAsset *handle;     // 0x3C
    u128 *queueA;         // 0x40
    u32 *queueB;          // 0x44
    struct SdfMemBlock *allocation; // 0x48
} EffSurfaceGridNode;

typedef char EffSurfaceGridNodeSizeCheck[(sizeof(EffSurfaceGridNode) == 0x4C) ? 1 : -1];
#endif

#endif
