#ifndef BTL_PHASE_PANEL_WORK_H
#define BTL_PHASE_PANEL_WORK_H

#include "btl_ui.h"

/* Shared phase-change panel task state. Both constructors allocate this full
 * 0x138-byte work block; the first three current points form the slide strip,
 * while the updater also uses the remaining points and all eight fade rows. */
typedef s8 BtlPhasePanelStripAnimationPhase;
enum {
    BTL_PHASE_PANEL_STRIP_ANIMATION_INITIAL_ENTRY = 0,
    BTL_PHASE_PANEL_STRIP_ANIMATION_FIRST_STRIP_SHIFT = 1,
    BTL_PHASE_PANEL_STRIP_ANIMATION_FIRST_TWO_STRIPS_SHIFT = 2,
    BTL_PHASE_PANEL_STRIP_ANIMATION_ALL_THREE_STRIPS_SHIFT = 3,
    BTL_PHASE_PANEL_STRIP_ANIMATION_ALL_THREE_STRIPS_SHIFT_WITH_EDGE_FADE = 4,
    BTL_PHASE_PANEL_STRIP_ANIMATION_PHASE_COUNT = 5, /* Exclusive render limit, not an active phase. */
};

typedef struct BattlePhasePanelWork {
    s32 frames;
    s32 variant;
    BtlPhasePanelStripAnimationPhase stripAnimationPhase; /* +0x08 */
    s8 secondaryPhase;                                    /* +0x09 */
    u8 pad0A[2];
    s32 echoCount;
    u8 pad10[5];
    s8 flags[2];
    u8 pad17;
    s32 waitCounter; /* 0x18: delay before the first slide */
    u8 pad1C[8];
    s32 counter;
    s32 angle;
    u8 pad2C[0xC];
    s32 current[8][2]; /* 0x38 */
    s32 saved[8][2];   /* 0x78 */
    s32 fade[8][4];                    /* 0xB8 */
} BattlePhasePanelWork;

typedef char BattlePhasePanelWork_size_must_be_0x138[
    (sizeof(BattlePhasePanelWork) == 0x138) ? 1 : -1];
typedef char BattlePhasePanelWork_stripAnimationPhase_offset_check[
    ((u32)&((BattlePhasePanelWork *)0)->stripAnimationPhase == 0x08) ? 1 : -1];
typedef char BattlePhasePanelWork_waitCounter_offset_check[
    ((u32)&((BattlePhasePanelWork *)0)->waitCounter == 0x18) ? 1 : -1];
typedef char BattlePhasePanelWork_current_offset_check[
    ((u32)&((BattlePhasePanelWork *)0)->current == 0x38) ? 1 : -1];
typedef char BattlePhasePanelWork_saved_offset_check[
    ((u32)&((BattlePhasePanelWork *)0)->saved == 0x78) ? 1 : -1];
typedef char BattlePhasePanelWork_fade_offset_check[
    ((u32)&((BattlePhasePanelWork *)0)->fade == 0xB8) ? 1 : -1];

#endif /* BTL_PHASE_PANEL_WORK_H */
