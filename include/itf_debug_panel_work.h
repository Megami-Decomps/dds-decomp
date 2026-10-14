#ifndef ITF_DEBUG_PANEL_WORK_H
#define ITF_DEBUG_PANEL_WORK_H

#include "common.h"

/* Shared edit, selection, and blink state for the debug adjustment panels. */
typedef struct ItfDebugPanelWork {
    s16 editing;
    s16 selection;
    u32 blink;
    u8 pad08[8];
} ItfDebugPanelWork;

typedef char ItfDebugPanelWork_size[
    (sizeof(ItfDebugPanelWork) == 0x10) ? 1 : -1];
typedef char ItfDebugPanelWork_editing_offset[
    ((u32)&((ItfDebugPanelWork *)0)->editing == 0x00) ? 1 : -1];
typedef char ItfDebugPanelWork_selection_offset[
    ((u32)&((ItfDebugPanelWork *)0)->selection == 0x02) ? 1 : -1];
typedef char ItfDebugPanelWork_blink_offset[
    ((u32)&((ItfDebugPanelWork *)0)->blink == 0x04) ? 1 : -1];

#endif
