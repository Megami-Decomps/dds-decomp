#ifndef MNU_SCENE_WORK_H
#define MNU_SCENE_WORK_H

#include "common.h"
#include "sdf_grid.h"
#include "mnu_scene_list.h"

typedef struct MenuScenePoint {
    s16 x;
    s16 y;
} MenuScenePoint;

typedef struct MenuSceneWork {
    struct SdfMemBlock *allocation; /* 0x000: retained general-heap owner */
    u8 pad004[0x480];
    SdfGrid *gridHandle; /* 0x484 */
    u32 gridRefreshControl[2]; /* 0x488 */
    u8 pad490[0x5C];
    s32 pendingMantras[8]; /* 0x4EC */
    u8 pad50C[0x34];
    s32 coordinateA; /* 0x540 */
    s32 coordinateB; /* 0x544 */
    u32 unk548; /* 0x548 */
    u8 pad54C[4];
    s32 transitionState; /* 0x550 */
    u8 pad554[0x30];
    MnuSceneListHead displayList584; /* 0x584 */
    MnuSceneListHead displayList590; /* 0x590 */
    MenuScenePoint entryPosition; /* 0x59C */
    MenuScenePoint cursorPosition; /* 0x5A0 */
    s16 scrollX; /* 0x5A4 */
    s16 scrollY; /* 0x5A6 */
    u8 pad5A8[4];
    u8 boundsFlags; /* 0x5AC */
    u8 cursorMoving; /* 0x5AD */
    u8 pad5AE[2];
} MenuSceneWork;

typedef char MenuSceneWorkLayoutAssert[
    (sizeof(MenuSceneWork) == 0x5B0 &&
     (u32)&((MenuSceneWork *)0)->gridHandle == 0x484 &&
     (u32)&((MenuSceneWork *)0)->unk548 == 0x548 &&
     (u32)&((MenuSceneWork *)0)->transitionState == 0x550 &&
     (u32)&((MenuSceneWork *)0)->displayList584 == 0x584 &&
     (u32)&((MenuSceneWork *)0)->displayList590 == 0x590 &&
     (u32)&((MenuSceneWork *)0)->boundsFlags == 0x5AC)
        ? 1 : -1];

#endif
