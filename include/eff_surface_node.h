#ifndef EFF_SURFACE_NODE_H
#define EFF_SURFACE_NODE_H

#include "common.h"
#include "eff.h"
#include "file.h"
#include "file_slot.h"
#include "sdf.h"

/* Surface parameters copied into the node: 0x1C bytes at +0x10. */
typedef struct EffSurfaceParams {
    u32 opaque00[6];
    f32 scale;
} EffSurfaceParams;

/* An effect surface node: the surface kind, its copied parameters and the
 * resources and jobs the surface owns. */
#ifdef VERSION_DDS1
typedef struct EffectSurfaceNode {
    u32 handleCount;
    u32 color;
    f32 opacity;
    u32 kind;
    EffSurfaceParams params; /* 0x10: copied source header */
    u32 index;
    struct SdfMemBlock *handleBuffer; // 0x30
    BillObj *resource;
    FileJobPayload **jobs;          // 0x38
    struct SdfMemBlock *jobAllocation;      // 0x3C
    struct EffExpandedList *resourceHolder; // 0x40, released separately from the grid record
    FileSlotTable *record; // 0x44: retained grid record table
    u16 count;          // 0x48: initialized to 1; remaining role unknown
} EffectSurfaceNode;
typedef char EffectSurfaceNode_handleBuffer_offset_check[
    ((u32)&((EffectSurfaceNode *)0)->handleBuffer == 0x30) ? 1 : -1];
#else
typedef struct EffectSurfaceNode {
    u32 handleCount;
    u32 color;
    f32 opacity;
    u32 kind;
    EffSurfaceParams params; /* 0x10: copied source header */
    u32 index;
    struct SdfMemBlock *handleBuffer; // 0x30
    BillObj *resource;
    FileJobPayload **jobs;          // 0x38
    struct SdfMemBlock *jobAllocation;      // 0x3C
    struct FileQueue **queues;      // 0x40: cloned queues, one per active record
    struct SdfMemBlock *queueAllocation;    // 0x44: backing allocation descriptor
    struct EffExpandedList *resourceHolder; // 0x48
    FileSlotTable *record; // 0x4C: retained grid record table
    u16 active;         // 0x50: initialized to 1
} EffectSurfaceNode;
typedef char EffectSurfaceNode_handleBuffer_offset_check[
    ((u32)&((EffectSurfaceNode *)0)->handleBuffer == 0x30) ? 1 : -1];
#endif

#endif
