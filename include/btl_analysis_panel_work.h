#ifndef BTL_ANALYSIS_PANEL_WORK_H
#define BTL_ANALYSIS_PANEL_WORK_H

#include "common.h"
#include "btl.h"

/* The battle analysis panel's work. */
#ifdef VERSION_DDS1
typedef struct BtlAnalysisPanelWork {
    s8 state;
    u8 pad01[3];
    s32 entry;
    s32 duration;
    s32 frame;
    u8 pad10[4];
    s16 alpha;
    u8 pad16[0x12];
    s32 x;
    s32 y;
    s8 page;
    u8 pad31[0xF];
    f32 anchor[4];
    f32 corners[4][4];
    s16 edgeAlpha;
    u8 pad92[0xE];
} BtlAnalysisPanelWork;
#else
typedef struct BtlAnalysisPanelWork {
    s8 state;
    u8 pad01[3];
    BtlUnit *unit;
    s32 duration;
    s32 frame;
    u8 pad10[4];
    s16 fade;
    u8 pad16[0x12];
    s32 x;
    s32 y;
    s8 page;
    u8 pad31[0xF];
    f32 anchor[4];
    f32 corners[4][4];
    s16 edgeAlpha;
    u8 pad92[0xE];
} BtlAnalysisPanelWork;
#endif

#endif
