#ifndef DDS_FLD_LMAP_TASK_H
#define DDS_FLD_LMAP_TASK_H

#include "common.h"
#include "sdf.h"

/* User data allocated by fldStartLmapTask and passed to the LmapMain task. */
typedef struct LmapTaskState {
    u32 unknown00;                /* 0x00 */
    s32 phase;                    /* 0x04: task lifecycle phase */
    s32 variant;                  /* 0x08: selected local-map variant */
    u32 spriteSlots[25];          /* 0x0C: SdfSlotSet-compatible slot array */
    SdfMemBlock *allocation;      /* 0x70: loaded resource allocation */
    u32 resourceAddress;          /* 0x74: loaded resource address */
    u32 unknown78;                /* 0x78 */
    u32 unknown7C;                /* 0x7C */
    s16 loaderState;              /* 0x80 */
    s16 loaderIndex;              /* 0x82 */
    u32 unknown84;                /* 0x84 */
} LmapTaskState;

extern LmapTaskState *D_0043888C;

typedef char LmapTaskState_size_must_be_0x88[(sizeof(LmapTaskState) == 0x88) ? 1 : -1];
#define LMAP_TASK_OFFSET(member) ((u32)&(((LmapTaskState *)0)->member))
typedef char LmapTaskState_spriteSlots_must_be_0x0C[(LMAP_TASK_OFFSET(spriteSlots) == 0x0C) ? 1 : -1];
typedef char LmapTaskState_allocation_must_be_0x70[(LMAP_TASK_OFFSET(allocation) == 0x70) ? 1 : -1];
typedef char LmapTaskState_resourceAddress_must_be_0x74[(LMAP_TASK_OFFSET(resourceAddress) == 0x74) ? 1 : -1];
typedef char LmapTaskState_loaderState_must_be_0x80[(LMAP_TASK_OFFSET(loaderState) == 0x80) ? 1 : -1];
typedef char LmapTaskState_loaderIndex_must_be_0x82[(LMAP_TASK_OFFSET(loaderIndex) == 0x82) ? 1 : -1];
#undef LMAP_TASK_OFFSET

#endif
