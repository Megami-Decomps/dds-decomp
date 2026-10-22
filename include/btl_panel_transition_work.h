#ifndef BTL_PANEL_TRANSITION_WORK_H
#define BTL_PANEL_TRANSITION_WORK_H

#include "btl_ui.h"
#include "kwln.h"

/* Shared skill-name panel task state in both battle titles. */
typedef struct BtlPanelTransitionWork {
    dds3ProcessID_t *task;
    const u8 *text;
    s32 elapsed;
    s32 frames;
    s32 width;
    s32 unk14;
    s32 verticalShift;
    s8 state;
    u8 pad1D;
    s16 fade;
    s16 fadeLevels[4];
    BattleSelectionPosition initial[2];
    s8 phase;
    u8 pad39[3];
    BattleSelectionPosition current[2];
} BtlPanelTransitionWork;

typedef char BtlPanelTransitionWorkSizeCheck[
    (sizeof(BtlPanelTransitionWork) == 0x4C) ? 1 : -1];
typedef char BtlPanelTransitionWorkElapsedOffsetCheck[
    ((u32)&((BtlPanelTransitionWork *)0)->elapsed == 0x08) ? 1 : -1];
typedef char BtlPanelTransitionWorkFadeOffsetCheck[
    ((u32)&((BtlPanelTransitionWork *)0)->fade == 0x1E) ? 1 : -1];
typedef char BtlPanelTransitionWorkFadeLevelsOffsetCheck[
    ((u32)&((BtlPanelTransitionWork *)0)->fadeLevels == 0x20) ? 1 : -1];
typedef char BtlPanelTransitionWorkInitialOffsetCheck[
    ((u32)&((BtlPanelTransitionWork *)0)->initial == 0x28) ? 1 : -1];
typedef char BtlPanelTransitionWorkPhaseOffsetCheck[
    ((u32)&((BtlPanelTransitionWork *)0)->phase == 0x38) ? 1 : -1];
typedef char BtlPanelTransitionWorkCurrentOffsetCheck[
    ((u32)&((BtlPanelTransitionWork *)0)->current == 0x3C) ? 1 : -1];

#endif
