#ifndef BTL_ANALYSIS_PANEL_WORK_H
#define BTL_ANALYSIS_PANEL_WORK_H

#include "common.h"
#include "btl.h"

enum {
    BTL_ANALYSIS_PANEL_TRANSITION_ENTERING = 0,
    BTL_ANALYSIS_PANEL_TRANSITION_ACTIVE = 1,
    BTL_ANALYSIS_PANEL_TRANSITION_CLOSING = 2,
};

/* The battle analysis panel's work. */
#ifdef VERSION_DDS1
typedef struct BtlAnalysisPanelWork {
    s8 transitionState;
    u8 pad01[3];
    BtlUnit *unit;
    s32 duration;
    s32 frame;
    u8 pad10[4];
    s16 alpha;
    u8 pad16[0x12];
    s32 x;
    s32 y;
    s8 page;
    s8 edgePhase;
    u8 pad32[2];
    s32 timer;
    u8 pad38[8];
    f32 anchor[4];
    f32 corners[4][4];
    s16 edgeAlpha;
    u8 pad92[0xE];
} BtlAnalysisPanelWork;
#else
typedef struct BtlAnalysisPanelWork {
    s8 transitionState;
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
    s8 edgePhase;
    u8 pad32[2];
    s32 timer;
    u8 pad38[8];
    f32 anchor[4];
    f32 corners[4][4];
    s16 edgeAlpha;
    u8 pad92[0xE];
} BtlAnalysisPanelWork;
#endif

#endif
