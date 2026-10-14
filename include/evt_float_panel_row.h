#ifndef EVT_FLOAT_PANEL_ROW_H
#define EVT_FLOAT_PANEL_ROW_H

#include "common.h"

/* Four-entry XY editor table; the leading halfword is not consumed here. */
typedef struct EvtFloatPanelRow {
    u16 opaque00;
    u16 y;
    s32 commandIndex;
    const char *format;
} EvtFloatPanelRow;

typedef char EvtFloatPanelRow_size[(sizeof(EvtFloatPanelRow) == 0x0C) ? 1 : -1];
typedef char EvtFloatPanelRow_y_offset[
    ((u32)&((EvtFloatPanelRow *)0)->y == 0x02) ? 1 : -1];
typedef char EvtFloatPanelRow_commandIndex_offset[
    ((u32)&((EvtFloatPanelRow *)0)->commandIndex == 0x04) ? 1 : -1];
typedef char EvtFloatPanelRow_format_offset[
    ((u32)&((EvtFloatPanelRow *)0)->format == 0x08) ? 1 : -1];

#endif /* EVT_FLOAT_PANEL_ROW_H */
