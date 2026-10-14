#ifndef ITF_COLOR_PANEL_ROW_H
#define ITF_COLOR_PANEL_ROW_H

#include "common.h"

typedef struct ItfColorPanelRow {
    const char *title;
    s32 highlight;
} ItfColorPanelRow;

typedef char ItfColorPanelRow_size[(sizeof(ItfColorPanelRow) == 0x08) ? 1 : -1];
typedef char ItfColorPanelRow_title_offset[((u32)&((ItfColorPanelRow *)0)->title == 0x00) ? 1 : -1];
typedef char ItfColorPanelRow_highlight_offset[((u32)&((ItfColorPanelRow *)0)->highlight == 0x04) ? 1 : -1];

#endif
