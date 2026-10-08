#ifndef MNU_SCENE_H
#define MNU_SCENE_H

#include "common.h"

#ifndef VERSION_DDS2

typedef struct DspParticle {
    s16 x;
    s16 y;
    s16 timer;
    s16 timerMax;
    s16 z;
    s8 phase;
    u8 size;
} DspParticle;

typedef struct {
    s32 countdown;
    s32 period;
    f32 strength;
    DspParticle particles[8];
} DspParticleState;

/* Mantra-scene state shared by its controller and animated display. */
enum MenuSceneState {
    MENU_SCENE_STATE_SELECTING = 0,
    MENU_SCENE_STATE_PRIMARY_LABELS = 1,
    MENU_SCENE_STATE_ALTERNATE_LABELS = 2,
    MENU_SCENE_STATE_THIRD_LABELS = 3,
    MENU_SCENE_STATE_SIGNAL_A = 4,
    MENU_SCENE_STATE_SIGNAL_B = 5,
    MENU_SCENE_STATE_SIGNAL_C = 6,
    MENU_SCENE_STATE_SIGNAL_D = 7,
    MENU_SCENE_STATE_SIGNAL_E = 8,
    MENU_SCENE_STATE_WAITING_FOR_MESSAGE_WINDOW = 9,
    MENU_SCENE_STATE_WAITING_FOR_MESSAGE_COMPLETION = 10,
};

typedef struct MenuSceneMetadata {
    u8 pad00[0x0C];
    s32 messageWindowResource;
    u8 pad10[8];
    s32 state;
    u8 pad1C[4];
    s32 messageShadeFrames;
    s32 attachedEffect;
    u32 attachedEffectControl;
    u16 entryId;
    u8 pad02E[0x19E];
    DspParticleState sparkles; /* 0x1CC: shared timer and eight particles */
    u8 pad238[4];
    s32 displayedCurrency;
    s32 currencyFrame;
    u16 pendingProfileId;
    s8 stageFinished;
    s8 stageStarted;
} MenuSceneMetadata;

typedef char MenuSceneParticleLayoutAssert[
    (sizeof(DspParticle) == 0x0C && sizeof(DspParticleState) == 0x6C &&
     sizeof(MenuSceneMetadata) == 0x248 &&
     (unsigned long)&((MenuSceneMetadata *)0)->sparkles == 0x1CC &&
     (unsigned long)&((MenuSceneMetadata *)0)->displayedCurrency == 0x23C)
        ? 1 : -1];

#endif

#endif
