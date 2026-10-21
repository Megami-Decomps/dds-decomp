#ifndef BTL_REGISTERED_PANEL_WORK_H
#define BTL_REGISTERED_PANEL_WORK_H

#include "common.h"

/* Work shared by the registered battle-panel task in both titles. */
typedef struct BattleRegisteredPanelWork {
    s32 state;
    s32 mode;
    s32 variant;
    s32 x;
    s32 y;
    s32 spriteAlpha;
    s32 overlayAlpha;
    s32 backdropAlpha;
} BattleRegisteredPanelWork;

typedef char BattleRegisteredPanelWorkSizeCheck[
    (sizeof(BattleRegisteredPanelWork) == 0x20) ? 1 : -1];
typedef char BattleRegisteredPanelWorkModeOffsetCheck[
    ((u32)&((BattleRegisteredPanelWork *)0)->mode == 0x04) ? 1 : -1];
typedef char BattleRegisteredPanelWorkVariantOffsetCheck[
    ((u32)&((BattleRegisteredPanelWork *)0)->variant == 0x08) ? 1 : -1];
typedef char BattleRegisteredPanelWorkXOffsetCheck[
    ((u32)&((BattleRegisteredPanelWork *)0)->x == 0x0C) ? 1 : -1];
typedef char BattleRegisteredPanelWorkYOffsetCheck[
    ((u32)&((BattleRegisteredPanelWork *)0)->y == 0x10) ? 1 : -1];
typedef char BattleRegisteredPanelWorkSpriteAlphaOffsetCheck[
    ((u32)&((BattleRegisteredPanelWork *)0)->spriteAlpha == 0x14) ? 1 : -1];
typedef char BattleRegisteredPanelWorkOverlayAlphaOffsetCheck[
    ((u32)&((BattleRegisteredPanelWork *)0)->overlayAlpha == 0x18) ? 1 : -1];
typedef char BattleRegisteredPanelWorkBackdropAlphaOffsetCheck[
    ((u32)&((BattleRegisteredPanelWork *)0)->backdropAlpha == 0x1C) ? 1 : -1];

#endif
