#include "sdk_ps2.h"
#include "sdf_scene_packet.h"
#include "common.h"
#include "sdf_packet_list.h"
#include "btl.h"

extern u64 *dds3PrimFrameRect(s32, s32, s32, s32, s32, u32, u32);
#define DDS3_PAD_INPUTS_PER_SET 16
#define DDS3_PAD_REPEAT_BIT 2
#define DDS3_ALT_CONFIRM_INPUT 0x11
#define DDS3_ALT_CANCEL_INPUT 0x13
#define DDS3_CELL_COLUMN_SPAN 0xC0
#define DDS3_CELL_ROW_SPAN 0x60

extern s8 dds3PadButtonState[64];

/* Fresh presses wrap; repeat-bit input stops at the implemented end bounds.
 * Without scrolling, forward motion is bounded by visibleRows, not itemCount. */
void SelectSub(s32 *scrollOffset, s32 *cursorRow, s32 itemCount, s32 visibleRows, s32 padSet, s32 forwardInput, s32 backwardInput) {
    s32 backwardState;
    s32 forwardState;

    if (cursorRow == NULL) {
        return;
    }
    if (itemCount < 2) {
        return;
    }
    backwardState = dds3PadButtonState[backwardInput + padSet * DDS3_PAD_INPUTS_PER_SET];
    if (backwardState < 0) {
        if (scrollOffset == NULL) {
            if (*cursorRow > 0) {
                (*cursorRow)--;
            } else if (itemCount < visibleRows) {
                *cursorRow = itemCount - 1;
            } else {
                *cursorRow = visibleRows - 1;
            }
        } else if (*cursorRow > 0) {
            (*cursorRow)--;
        } else if (*scrollOffset > 0) {
            (*scrollOffset)--;
        } else if (visibleRows < itemCount) {
            *scrollOffset = itemCount - visibleRows;
            *cursorRow = visibleRows - 1;
        } else {
            *cursorRow = itemCount - 1;
        }
    } else if (dds3PadButtonState[backwardInput + padSet * DDS3_PAD_INPUTS_PER_SET] & DDS3_PAD_REPEAT_BIT) {
        if (scrollOffset == NULL) {
            if (*cursorRow > 0) {
                (*cursorRow)--;
            }
        } else if (*cursorRow > 0) {
            (*cursorRow)--;
        } else if (*scrollOffset > 0) {
            (*scrollOffset)--;
        }
    }
    forwardState = dds3PadButtonState[forwardInput + padSet * DDS3_PAD_INPUTS_PER_SET];
    if (forwardState < 0) {
        if (scrollOffset == NULL) {
            if (*cursorRow < visibleRows - 1) {
                (*cursorRow)++;
            } else {
                *cursorRow = 0;
            }
        } else if (*cursorRow < visibleRows - 1) {
            (*cursorRow)++;
        } else if (*scrollOffset + visibleRows < itemCount) {
            (*scrollOffset)++;
        } else {
            *scrollOffset = 0;
            *cursorRow = 0;
        }
    } else if (dds3PadButtonState[forwardInput + padSet * DDS3_PAD_INPUTS_PER_SET] & DDS3_PAD_REPEAT_BIT) {
        if (scrollOffset == NULL) {
            if (*cursorRow < visibleRows - 1) {
                (*cursorRow)++;
            }
        } else if (*cursorRow < visibleRows - 1) {
            (*cursorRow)++;
        } else if (*scrollOffset + visibleRows < itemCount) {
            (*scrollOffset)++;
        }
    }
}

/* Step the primary then secondary cursor pair; return 1/-1 for confirm/cancel.
 * Any nonzero padSet tests set 1's confirm/cancel, while movement uses padSet. */
s32 dds3SelectMenu(s32 padSet, s32 secondaryCount, s32 primaryCount, s32 secondaryVisible, s32 primaryVisible, s32 *secondaryScroll, s32 *primaryScroll, s32 *secondaryCursor, s32 *primaryCursor) {
    SelectSub(primaryScroll, primaryCursor, primaryCount, primaryVisible, padSet, 7, 6);
    SelectSub(secondaryScroll, secondaryCursor, secondaryCount, secondaryVisible, padSet, 5, 4);
    if (padSet == 0) {
        if (dds3PadButtonState[1] < 0) {
            return 1;
        }
        if (dds3PadButtonState[3] < 0) {
            return -1;
        }
    } else {
        if (dds3PadButtonState[DDS3_ALT_CONFIRM_INPUT] < 0) {
            return 1;
        }
        if (dds3PadButtonState[DDS3_ALT_CANCEL_INPUT] < 0) {
            return -1;
        }
    }
    return 0;
}

/* Append a cell rectangle; rowSpan also supplies the horizontal padding. */
void dds3ConsPanelTagLink(sdfDrawTag_t *packetList, s32 column, s32 row, s32 columnCount, s32 rowCount) {
    s32 columnSpan = DDS3_CELL_COLUMN_SPAN, rowSpan = DDS3_CELL_ROW_SPAN;
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(column * 0x10 + 0x6FD0, row * 8 + 0x78E8, 0xFEFFFF,
                                           columnCount * columnSpan + rowSpan, rowCount * rowSpan + 0x30,
                                           0x60000000, 0x40806020));
}

/* Append the same cell rectangle with caller-selected depth. */
void dds3ConsPanelTagLinkZ(sdfDrawTag_t *packetList, s32 column, s32 row, s32 columnCount, s32 rowCount, s32 depth) {
    s32 columnSpan = DDS3_CELL_COLUMN_SPAN, rowSpan = DDS3_CELL_ROW_SPAN;
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(column * 0x10 + 0x6FD0, row * 8 + 0x78E8, depth,
                                           columnCount * columnSpan + rowSpan, rowCount * rowSpan + 0x30,
                                           0x60000000, 0x40806020));
}
