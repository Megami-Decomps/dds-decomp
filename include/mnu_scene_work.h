#ifndef MNU_SCENE_WORK_H
#define MNU_SCENE_WORK_H

#include "common.h"
#include "sdf_grid.h"
#include "mnu_scene_list.h"

typedef struct MenuScenePoint {
    s16 x;
    s16 y;
} MenuScenePoint;

typedef struct MenuSceneCoordinate {
    s32 x;
    s32 y;
} MenuSceneCoordinate;

typedef struct MantraPulseAnimationWork {
    s32 frame;
    u8 pad568[4];
    s16 alpha[4];
} MantraPulseAnimationWork;

typedef struct MenuSceneWork {
    struct SdfMemBlock *allocation; /* 0x000: retained general-heap owner */
    u8 pad004[0x480];
    SdfGrid *gridHandle; /* 0x484 */
    u32 gridRefreshControl[2]; /* 0x488 */
    s32 gridFrame; /* 0x490: pulse-grid animation frame */
    u8 pad494[8];
    MenuSceneCoordinate coordinates[10]; /* 0x49C: copied grid-entry positions */
    s32 pendingMantras[8]; /* 0x4EC */
    u8 pad50C[0x34];
    s32 scenePhase; /* 0x540: main menu-scene state-machine selector */
    s32 phaseFrame; /* 0x544: elapsed frame within the current phase */
    u32 unk548; /* 0x548 */
    u8 pad54C[4];
    s32 transitionBlendCounter; /* 0x550: signed blend step, driven between +/-10 and +/-5 */
    u8 pad554[0x0C];
    s32 displayAlpha; /* 0x560: alpha used by mantra-entry status drawing */
    MantraPulseAnimationWork pulse; /* 0x564: four pulse-channel fade values */
    u8 pad574[0x10];
    MnuSceneListHead sceneTransitionList; /* 0x584 */
    MnuSceneListHead costTransitionList; /* 0x590 */
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
     (u32)&((MenuSceneWork *)0)->gridFrame == 0x490 &&
     (u32)&((MenuSceneWork *)0)->coordinates == 0x49C &&
     (u32)&((MenuSceneWork *)0)->unk548 == 0x548 &&
     (u32)&((MenuSceneWork *)0)->transitionBlendCounter == 0x550 &&
     (u32)&((MenuSceneWork *)0)->displayAlpha == 0x560 &&
     (u32)&((MenuSceneWork *)0)->pulse == 0x564 &&
     (u32)&((MenuSceneWork *)0)->sceneTransitionList == 0x584 &&
     (u32)&((MenuSceneWork *)0)->costTransitionList == 0x590 &&
     (u32)&((MenuSceneWork *)0)->entryPosition == 0x59C &&
     (u32)&((MenuSceneWork *)0)->cursorPosition == 0x5A0 &&
     (u32)&((MenuSceneWork *)0)->scrollX == 0x5A4 &&
     (u32)&((MenuSceneWork *)0)->boundsFlags == 0x5AC)
        ? 1 : -1];

typedef char MantraPulseAnimationWorkSizeAssert[
    (sizeof(MantraPulseAnimationWork) == 0x10 &&
     (u32)&((MantraPulseAnimationWork *)0)->alpha == 0x08)
        ? 1 : -1];

#endif
