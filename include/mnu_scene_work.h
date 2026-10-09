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

/* Ring animation state that also schedules the one-shot profile-match cue. */
typedef struct MnuGridFeedbackState {
    s32 frame;
    s32 mode; /* 0: looping ring, 1: fading ring and one-shot cue */
} MnuGridFeedbackState;

/* Requirement groups evaluated while building the scene grid. The final bit
 * is also set when model flag 0x908 is already active. */
#ifndef VERSION_DDS2
enum MenuSceneRequirementGroupFlag {
    MENU_SCENE_REQUIREMENT_GROUP_0_MET = 0x01,
    MENU_SCENE_REQUIREMENT_GROUP_1_MET = 0x02,
    MENU_SCENE_REQUIREMENT_GROUP_2_MET = 0x04,
};
#endif

typedef struct MantraPulseAnimationWork {
    s32 frame;
    u8 pad568[4];
    s16 alpha[4];
} MantraPulseAnimationWork;

/* Scene-owned grid records: 00253208 writes four halfwords and the state word. */
typedef struct MnuMantraNodeState {
    s16 x;
    s16 y;
    s16 xRemainder;
    s16 yRemainder;
    s32 state;
} MnuMantraNodeState;

typedef char MnuMantraNodeStateLayoutAssert[
    (sizeof(MnuMantraNodeState) == 0x0C &&
     (u32)&((MnuMantraNodeState *)0)->y == 2 &&
     (u32)&((MnuMantraNodeState *)0)->xRemainder == 4 &&
     (u32)&((MnuMantraNodeState *)0)->yRemainder == 6 &&
     (u32)&((MnuMantraNodeState *)0)->state == 8)
        ? 1 : -1];

typedef struct MenuSceneWork {
    struct SdfMemBlock *allocation; /* 0x000: retained general-heap owner */
    MnuMantraNodeState nodes[96]; /* 0x004: grid userData; 0x480 bytes */
    SdfGrid *gridHandle; /* 0x484 */
    MnuGridFeedbackState gridFeedback; /* 0x488 */
    s32 gridFrame; /* 0x490: pulse-grid animation frame */
    u8 pad494[8];
    MenuSceneCoordinate coordinates[10]; /* 0x49C: copied grid-entry positions */
    s32 pendingMantras[8]; /* 0x4EC */
    u8 pad50C[0x34];
    s32 scenePhase; /* 0x540: main menu-scene state-machine selector */
    s32 phaseFrame; /* 0x544: elapsed frame within the current phase */
    u32 entryMessageInProgress; /* 0x548: gates selected-entry drawing during DSP entry */
    u32 cursorInputMask; /* 0x54C: selected grid-navigation input gate */
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
    s8 boundsFlags; /* 0x5AC */
    s8 cursorMoving; /* 0x5AD */
    u8 pendingPhase13; /* 0x5AE: consumed when the phase-12 handoff completes */
    u8 pad5AF;
} MenuSceneWork;

typedef char MenuSceneWorkLayoutAssert[
    (sizeof(MenuSceneWork) == 0x5B0 &&
     (u32)&((MenuSceneWork *)0)->nodes == 4 &&
     sizeof(((MenuSceneWork *)0)->nodes) == 0x480 &&
     (u32)&((MenuSceneWork *)0)->gridHandle == 0x484 &&
     (u32)&((MenuSceneWork *)0)->gridFeedback == 0x488 &&
     (u32)&((MenuSceneWork *)0)->gridFrame == 0x490 &&
     (u32)&((MenuSceneWork *)0)->coordinates == 0x49C &&
     (u32)&((MenuSceneWork *)0)->entryMessageInProgress == 0x548 &&
     (u32)&((MenuSceneWork *)0)->cursorInputMask == 0x54C &&
     (u32)&((MenuSceneWork *)0)->transitionBlendCounter == 0x550 &&
     (u32)&((MenuSceneWork *)0)->displayAlpha == 0x560 &&
     (u32)&((MenuSceneWork *)0)->pulse == 0x564 &&
     (u32)&((MenuSceneWork *)0)->sceneTransitionList == 0x584 &&
     (u32)&((MenuSceneWork *)0)->costTransitionList == 0x590 &&
     (u32)&((MenuSceneWork *)0)->entryPosition == 0x59C &&
     (u32)&((MenuSceneWork *)0)->cursorPosition == 0x5A0 &&
     (u32)&((MenuSceneWork *)0)->scrollX == 0x5A4 &&
     (u32)&((MenuSceneWork *)0)->boundsFlags == 0x5AC &&
     (u32)&((MenuSceneWork *)0)->pendingPhase13 == 0x5AE)
        ? 1 : -1];

typedef char MnuGridFeedbackStateSizeAssert[
    (sizeof(MnuGridFeedbackState) == 0x08) ? 1 : -1];

typedef char MantraPulseAnimationWorkSizeAssert[
    (sizeof(MantraPulseAnimationWork) == 0x10 &&
     (u32)&((MantraPulseAnimationWork *)0)->alpha == 0x08)
        ? 1 : -1];

#endif
