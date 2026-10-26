#ifndef ITF_GRID_TEXT_COLOR_API_H
#define ITF_GRID_TEXT_COLOR_API_H

#include "common.h"

struct GridTextWidget;
struct GridTextListItem;

/* Index consumed by sdfPktInit and resolved through sdfDevConsColorTable. */
typedef s32 ItfGridTextColorTableIndex;

/* Select the message-color table entry for a grid row. */
ItfGridTextColorTableIndex itfGetGridTextColorTableIndex(
    const struct GridTextWidget *widget, const struct GridTextListItem *target);

/* Submit formatted row text using an sdfDevConsColorTable index. */
void fldLmapSubmitPositionedCommandPacket(s32 x, s32 y, s32 z,
    ItfGridTextColorTableIndex colorTableIndex, const char *command, s32 surfaceIndex);

#endif /* ITF_GRID_TEXT_COLOR_API_H */
