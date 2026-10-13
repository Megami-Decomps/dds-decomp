#ifndef BTL_WORLD_CAMERA_VIEW_H
#define BTL_WORLD_CAMERA_VIEW_H

#include "common.h"

/* Narrow access view used when a battle selects its current world camera. */
typedef struct WorldObjectSub {
    u8 opaque00[0x34];
    s32 handle; /* 0x34: converted to the selected EffWorldNode pointer */
} WorldObjectSub;

typedef struct WorldObjectHead {
    u8 opaque00[8];
    WorldObjectSub *sub; /* 0x08 */
} WorldObjectHead;

typedef struct WorldObj {
    u8 opaque00[0x18];
    WorldObjectHead *head; /* 0x18 */
} WorldObj;

typedef char WorldObjectSubHandleOffsetCheck[((u32)&((WorldObjectSub *)0)->handle) == 0x34 ? 1 : -1];
typedef char WorldObjectHeadSubOffsetCheck[((u32)&((WorldObjectHead *)0)->sub) == 8 ? 1 : -1];
typedef char WorldObjHeadOffsetCheck[((u32)&((WorldObj *)0)->head) == 0x18 ? 1 : -1];

#endif
