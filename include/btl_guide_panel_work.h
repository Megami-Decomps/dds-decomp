#ifndef BTL_GUIDE_PANEL_WORK_H
#define BTL_GUIDE_PANEL_WORK_H

#include "common.h"

typedef struct BtlGuidePanelWork {
    s8 state;
    u8 pad01[7];
    s16 fadeLevel;
    u8 pad0A[2];
    struct {
        u32 values[4];
    } colors; /* TL, TR, BR, BL */
    s32 unk1C;
    s32 y;
    s32 windowIndex;
    s32 entryIndex;
} BtlGuidePanelWork;

typedef char BtlGuidePanelWorkSize[
    (sizeof(BtlGuidePanelWork) == 0x2C) ? 1 : -1];
typedef char BtlGuidePanelWorkFadeOffset[
    ((unsigned long)&((BtlGuidePanelWork *)0)->fadeLevel == 0x08) ? 1 : -1];
typedef char BtlGuidePanelWorkColorsOffset[
    ((unsigned long)&((BtlGuidePanelWork *)0)->colors == 0x0C) ? 1 : -1];
typedef char BtlGuidePanelWorkPositionOffset[
    ((unsigned long)&((BtlGuidePanelWork *)0)->y == 0x20) ? 1 : -1];
typedef char BtlGuidePanelWorkWindowOffset[
    ((unsigned long)&((BtlGuidePanelWork *)0)->windowIndex == 0x24) ? 1 : -1];
typedef char BtlGuidePanelWorkEntryOffset[
    ((unsigned long)&((BtlGuidePanelWork *)0)->entryIndex == 0x28) ? 1 : -1];

#endif
