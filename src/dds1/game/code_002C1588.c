#include "sdf_gs_header.h"
#include "common.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "fpu.h"
#include "eff.h"
#include "eff_resource_slots.h"
#include "eff_resource_records.h"
#include "itf_grid_text.h"
#include "itf_draw_grid.h"
#include "sdf.h"
#include "sdf_draw.h"
#include "sdf_projection.h"
#include "pcp_vu0.h"

extern GridTextListItem *itfFindGridNodeByKey(u32, GridTextWidget *);

extern GridTextListItem *itfRemoveSelectedGridTextItem(GridTextWidget *);

extern s32 sdfGridSeekSelectedNodeByIndex(s32, GridTextWidget *);

void sdfSubmitGsAlphaOneRegisterPacket(u32 data, u32 kind);

void sdfSubmitGsTestOneRegisterPacket(u64 data, u32 kind);

void uiDrawUniformColorRect(u32 x, u32 y, u32 z, u32 width, u32 height, u32 color, u32 surfaceIndex);

void uiDrawSurfaceAtNearDepth(u32 surface) {
    sdfSubmitGsTestOneRegisterPacket(0x30000, surface);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, surface);
    uiDrawUniformColorRect(0, 0, 0, 0x2000, 0xe00, 0, surface);
}

/* Scale packed RGB by a Q8 factor, preserving the low alpha byte. */
u32 uiScaleColorRgb(u32 color, u32 scaleFactor) {
    u32 red = color >> 24;
    u32 green = (color >> 16) & 0xFF;
    u32 blue = (color & 0xFF00) >> 8;
    u32 alpha = color & 0xFF;
    red = (red * scaleFactor) >> 8;
    green = (green * scaleFactor) >> 8;
    blue = (blue * scaleFactor) >> 8;
    return (red << 24) | (green << 16) | (blue << 8) | alpha;
}

/* Blend two packed 8-bit-channel colors: weight t (mirrored above 0x100) for the first, 0x100 - t for the second. */
u32 uiBlendColors(u32 c0, u32 c1, u32 t) {
    u32 a0 = c0 >> 24;
    u32 r0 = (c0 & 0xFF0000) >> 16;
    u32 g0 = (c0 & 0xFF00) >> 8;
    u32 b0 = c0 & 0xFF;
    u32 a1 = c1 >> 24;
    u32 r1 = (c1 & 0xFF0000) >> 16;
    u32 g1 = (c1 & 0xFF00) >> 8;
    u32 b1 = c1 & 0xFF;
    u32 inv;

    if (t > 0x100) {
        t = 0x200 - t;
    }
    inv = 0x100 - t;
    a0 = (a0 * t + a1 * inv) >> 8;
    r0 = (r0 * t + r1 * inv) >> 8;
    g0 = (g0 * t + g1 * inv) >> 8;
    b0 = (b0 * t + b1 * inv) >> 8;
    return (a0 << 24) | (r0 << 16) | (g0 << 8) | b0;
}

void func_002C16E0(void) {
}

void func_002C16E8(void) {
}

u32 func_002C16F0(void) {
    return 0;
}

s32 itfActivateGridTextWidget(GridTextWidget *widget) {
    if (widget == 0) {
        return 0;
    }
    widget->flags = (widget->flags & -2) | 2;
    return 1;
}

/* Apply parent flags and enable bit 1 on the linked child widget, if present. */
s32 itfSetWidgetFlagsAndActivateChild(GridTextWidget *widget, u32 flags) {
    GridTextListItem *childLink;
    if (widget == 0) {
        return 0;
    }
    widget->flags = (widget->flags & ~2) | flags;
    childLink = widget->selected;
    if (childLink != 0) {
        GridTextWidget *childWidget = childLink->child;
        if (childWidget != 0) {
            childWidget->flags |= 2;
        }
    }
    return 1;
}

/* Own a NUL-terminated text copy; store X scaled by 16 and Y scaled by 8. */
GridTextWidget *itfCreateGridTextWidget(const char *text, s32 x, s32 y, s32 columns, s32 rows,
                               u32 reference) {
    GridTextWidget *widget = (GridTextWidget *)sdfAllocSizeClassBlock(0x40);
    u32 textBytes;
    char *textCopy;

    memset(widget, 0, 0x40);
    textBytes = strlen(text) + 1;
    textCopy = (char *)sdfAllocSizeClassBlock(textBytes);
    widget->textLength = textBytes;
    widget->text = textCopy;
    memcpy(textCopy, text, textBytes);
    widget->firstVisible = NULL;
    widget->x = x << 4;
    widget->y = y << 3;
    widget->reference = reference;
    widget->rows = rows;
    widget->width = columns * 12 + 6;
    widget->height = rows * 14 + 6;
    widget->head = NULL;
    widget->selected = NULL;
    widget->tail = NULL;
    widget->flags = 0;
    widget->cursorRow = 0;
    widget->itemCount = 0;
    return widget;
}

/* A zero column or row count leaves that dimension unchanged. */
void itfSetGridDimensions(GridTextWidget *widget, s32 columns, s32 rows) {
    s32 columnWidth = columns * 12 + 6;
    s32 rowHeight = rows * 14 + 6;

    if (columns != 0) {
        widget->width = columnWidth;
    }
    if (rows != 0) {
        widget->height = rowHeight;
        widget->rows = rows;
    }
}

u32 itfDestroyGridTextWidget(GridTextWidget *widget) {
    GridTextListItem *next;

    sdfReleaseChipBlock(widget->text);
    do {
        next = itfRemoveSelectedGridTextItem(widget);
    } while (next != NULL);
    sdfReleaseChipBlock(widget);
    return 1;
}

/* Destroy linked child widgets recursively before releasing the parent widget. */
u32 itfDestroyGridTextWidgetTree(GridTextWidget *widget) {
    GridTextListItem *childLink;

    sdfReleaseChipBlock(widget->text);
    childLink = widget->selected;
    if (childLink != NULL) {
        do {
            GridTextWidget *childWidget = childLink->child;
            if (childWidget != NULL) {
                itfDestroyGridTextWidgetTree(childWidget);
            }
            childLink = itfRemoveSelectedGridTextItem(widget);
        } while (childLink != NULL);
    }
    sdfReleaseChipBlock(widget);
    return 1;
}

/* Grow only; numbered prefixes reserve four columns, plus two for hexadecimal. */
void itfExpandWidgetColumnWidth(s32 columns, GridTextWidget *widget) {
    s32 flags = widget->flags;
    s32 requiredWidth;
    if (flags & 0x100) {
        columns += 4;
        if (flags & 0x200) {
            columns += 2;
        }
    }
    requiredWidth = columns * 12 + 6;
    if (widget->width < requiredWidth) {
        widget->width = requiredWidth;
    }
}

/* Bit 0: the visible node has a predecessor. Bit 1: at least rows successors remain. */
u32 itfGetGridListLinkFlags(GridTextWidget *owner) {
    GridTextListItem *node = owner->firstVisible;
    u32 flags;
    s32 i;

    if (node == 0) {
        return 0;
    }
    flags = node->previous != 0;
    for (i = 0; i < (u16)owner->rows; i++) {
        node = node->next;
        if (node == 0) {
            flags &= ~2;
            return flags;
        }
    }
    flags |= 2;
    return flags;
}

GridTextListItem *itfAppendGridTextItem(GridTextWidget *owner, const char *text, u32 value) {
    GridTextListItem *item = (GridTextListItem *)sdfAllocSizeClassBlock(0x2C);
    GridTextListItem *tail;
    s32 length;
    s32 allocation;
    char *copy;

    memset(item, 0, 0x2C);
    if (owner->itemCount == 0) {
        owner->firstVisible = item;
        owner->selected = item;
        owner->head = item;
    }
    length = strlen(text);
    allocation = length + 1;
    copy = (char *)sdfAllocSizeClassBlock(allocation);
    item->textLength = allocation;
    item->text = copy;
    memcpy(copy, text, allocation);
    itfExpandWidgetColumnWidth(length, owner);

    /* Initialize the node links, then append it after the current tail. */
    item->previous = owner->tail;
    item->next = 0;
    item->value = value;
    tail = owner->tail;
    item->parameter = NULL;
    item->previous = tail;
    if (tail != 0) {
        tail->next = item;
    }
    owner->tail = item;
    item->index = owner->itemCount;
    owner->itemCount++;
    return item;
}

GridTextListItem *itfRemoveSelectedGridTextItem(GridTextWidget *widget) {
    GridTextListItem *node;
    GridTextListItem *cursor;
    GridTextListItem *previous;
    GridTextListItem *next;

    if (widget->itemCount == 0) {
        return NULL;
    }
    node = widget->selected;
    if (node == NULL) {
        return NULL;
    }
    cursor = node;
    do {
        if (node->index > 0) {
            node->index--;
        }
        node = node->next;
    } while (node != NULL);
    node = cursor;
    previous = node->previous;
    next = node->next;
    if (widget->tail->index - widget->firstVisible->index + 1 <= widget->rows) {
        if (widget->firstVisible != widget->head) {
            cursor = previous;
            widget->firstVisible = widget->firstVisible->previous;
            widget->selected = previous;
        } else if (node == widget->firstVisible) {
            if (next != NULL) {
                widget->firstVisible = next;
                cursor = next;
                widget->selected = next;
            } else {
                cursor = previous;
                widget->firstVisible = previous;
                widget->selected = previous;
                widget->cursorRow--;
            }
        } else if (next != NULL) {
            widget->selected = next;
            cursor = next;
        } else {
            cursor = previous;
            widget->selected = previous;
            widget->cursorRow--;
        }
    } else if (next != NULL) {
        widget->selected = next;
        cursor = next;
    }
    if (cursor == NULL) {
        widget->firstVisible = NULL;
        widget->head = NULL;
        widget->tail = NULL;
        widget->cursorRow = 0;
    }
    if (previous != NULL) {
        previous->next = next;
    }
    if (next != NULL) {
        next->previous = previous;
    }
    if (previous == NULL) {
        widget->firstVisible = next;
        widget->head = next;
    }
    if (next == NULL) {
        widget->tail = previous;
    }
    sdfReleaseChipBlock(node->text);
    if (node->parameter != NULL) {
        sdfReleaseChipBlock(node->parameter);
    }
    sdfReleaseChipBlock(node);
    widget->itemCount--;
    return widget->selected;
}

/* Replace the owned text and grow the optional parent column to fit its byte length. */
void itfReplaceGridTextAndExpandColumn(GridTextWidget *widget, GridTextListItem *item, const char *text) {
    s32 textLength;
    s32 textBytes;
    char *textCopy;

    sdfReleaseChipBlock(item->text);
    textLength = strlen(text);
    textBytes = textLength + 1;
    textCopy = (char *)sdfAllocSizeClassBlock(textBytes);
    item->textLength = textBytes;
    item->text = textCopy;
    memcpy(textCopy, text, textBytes);
    if (widget != NULL) {
        itfExpandWidgetColumnWidth(textLength, widget);
    }
}

s32 itfSetGridNumericItemDescriptor(GridTextWidget *widget, GridTextListItem *item,
                  GridNumericDescriptor *descriptor) {
    GridNumericDescriptor *copy;
    f32 maximum;
    s32 width = 1;
    s32 length;

    copy = (GridNumericDescriptor *)sdfAllocSizeClassBlock(sizeof(GridNumericDescriptor));
    item->parameter = copy;
    memcpy(copy, descriptor, sizeof(GridNumericDescriptor));
    item->number = descriptor->value;
    length = strlen(item->text);
    maximum = ((GridNumericDescriptor *)item->parameter)->maximum;

    switch (descriptor->mode) {
        case GRID_NUMERIC_FORMAT_HEXADECIMAL:
            while (maximum >= 16.0f) {
                maximum *= 0.0625f;
                width++;
            }
            width += 2;
            break;
        case GRID_NUMERIC_FORMAT_FLOAT:
            while (maximum >= 10.0f) {
                maximum /= 10.0f;
                width++;
            }
            width += 2;
            break;
        default:
            while (maximum >= 10.0f) {
                maximum /= 10.0f;
                width++;
            }
            break;
    }

    item->formatWidth = width;
    if (widget->flags & GRID_TEXT_PREFIX_ROW_INDEX) {
        length += width;
    } else {
        length += width + 1;
    }
    itfExpandWidgetColumnWidth(length, widget);
    return 1;
}

/* Advance by at least one configured step; crossing the maximum wraps to minimum. */
void itfAdvanceGridScrollPosition(GridTextWidget *widget, u32 key, s32 steps) {
    GridNumericDescriptor *range;
    GridTextListItem *entry;
    float *position;
    float delta;
    float previous;

    entry = itfFindGridNodeByKey(key, widget);
    range = entry->parameter;
    position = &entry->number;
    delta = range->step;
    if (1 < steps) {
        delta = delta * (float)(s32)steps;
    }
    previous = *position;
    *position = previous + delta;
    if (range->maximum < previous + delta) {
        *position = range->minimum;
    }
}

void itfAdvanceSelectedGridScroll(GridTextWidget *widget, u32 steps) {
    if ((widget->flags & 1) != 0) {
        itfAdvanceGridScrollPosition(widget, (u32)widget->selected->index + widget->rowOffset, steps);
        return;
    }
}

/* Reverse by at least one configured step; crossing the minimum wraps to maximum. */
void itfReverseGridScrollPosition(GridTextWidget *widget, u32 key, s32 steps) {
    GridNumericDescriptor *range;
    GridTextListItem *entry;
    float *position;
    float delta;
    float previous;

    entry = itfFindGridNodeByKey(key, widget);
    range = entry->parameter;
    position = &entry->number;
    delta = range->step;
    if (1 < steps) {
        delta = delta * (float)(s32)steps;
    }
    previous = *position;
    *position = previous - delta;
    if (previous - delta < range->minimum) {
        *position = range->maximum;
    }
}

void itfReverseSelectedGridScroll(GridTextWidget *widget, u32 steps) {
    if ((widget->flags & 1) != 0) {
        itfReverseGridScrollPosition(widget, (u32)widget->selected->index + widget->rowOffset, steps);
        return;
    }
}

s32 itfGetGridChildLayoutMode(GridTextWidget *widget, GridTextListItem *target) {
    u32 flags = widget->flags;

    if (flags & 2) {
        if (target == widget->selected) {
            return (flags & 1) ? 6 : 4;
        }
        return 0;
    }
    if (flags & 0x80) {
        if (target == widget->selected) {
            return 12;
        }
    } else if (target == widget->selected && (flags & 1)) {
        return 6;
    }
    return 0;
}

extern s32 strlen(const char *);

extern char *strcpy(char *, const char *);

extern s32 func_003014F0(char *, const char *, ...);

extern double fptodp(f32);

/* Format a native value row, then optionally prefix its decimal/hex row number. */
void itfFormatGridValueEntryText(GridTextWidget *widget, GridTextListItem *entry, char *out) {
    char text[0x100];
    char prefix[0x100];
    char format[0x100];

    if (entry->format != NULL) {
        entry->format(widget, entry, text, 0x100);
    } else if (entry->parameter == NULL) {
        func_003014F0(text, "%s", entry->text);
    } else {
        if (strlen(entry->text) == 0) {
            strcpy(prefix, "");
        } else {
            func_003014F0(prefix, "%s ", entry->text);
        }
        switch (((GridNumericDescriptor *)entry->parameter)->mode) {
        case GRID_NUMERIC_FORMAT_DECIMAL:
            func_003014F0(format, "%%s%%0%dd", entry->formatWidth);
            func_003014F0(text, format, prefix, (s32)entry->number);
            break;
        case GRID_NUMERIC_FORMAT_HEXADECIMAL:
            func_003014F0(format, "%%s0x%%0%dX", entry->formatWidth - 2);
            func_003014F0(text, format, prefix, (s32)entry->number);
            break;
        case GRID_NUMERIC_FORMAT_FLOAT:
            func_003014F0(format, "%%s%%0%d.1f", entry->formatWidth);
            func_003014F0(text, format, prefix, (double)entry->number);
            break;
        }
    }
    if (widget->flags & GRID_TEXT_PREFIX_ROW_INDEX) {
        s32 row = entry->index + widget->rowOffset;

        if (!(widget->flags & GRID_TEXT_HEX_ROW_INDEX)) {
            func_003014F0(out, "%03d:%s", row, text);
        } else {
            func_003014F0(out, "0x%03X:%s", row, text);
        }
    } else {
        strcpy(out, text);
    }
}

extern void fldLmapSubmitPositionedCommandPacket(s32, s32, s32, s32, const char *, s32);

/* Draw visible local-map rows and invoke the selected row callback once. */
void itfDrawGridTextRows(s32 offsetX, s32 offsetY, s32 z, GridTextWidget *widget,
                   s32 surfaceIndex) {
    char text[0x100];
    GridTextListItem *item;
    s32 rowEnd;
    u32 column;
    s32 itemIndex = 0;
    s32 invokeSelected = 1;

    offsetY = widget->y + offsetY + 0x10;
    rowEnd = offsetY + ((widget->height << 3) + widget->y) - 0x6C;
    offsetX = widget->x + offsetX + 0x20;
    item = widget->firstVisible;
    if (item != NULL) {
        if (offsetY < rowEnd) {
            if ((u16)widget->rows != 0) {
                do {
                    s32 layout = itfGetGridChildLayoutMode(widget, item);
                    s32 drawMode = 0;

                    if (invokeSelected != 0) {
                        drawMode = layout;
                    }
                    if (!(widget->flags & GRID_TEXT_HIDE_ROWS)) {
                        if (item == widget->selected && (widget->flags & GRID_TEXT_HIGHLIGHT_SELECTION)) {
                            column = (s32)item->number;

                            if (widget->flags & GRID_TEXT_PREFIX_ROW_INDEX) {
                                column += 4;
                                if (widget->flags & GRID_TEXT_HEX_ROW_INDEX) {
                                    column += 2;
                                }
                            }
                            column = (column * 3) << 6;
                            uiDrawUniformColorRect(offsetX + column, offsetY, z,
                                                   0xD0, 0x78, 0x40408080,
                                                   surfaceIndex);
                        }
                        if ((widget->flags & GRID_TEXT_HIGHLIGHT_VALUES) && item->value != 0) {
                            s32 width = strlen(item->text) * 0xC0 + 0x10;
                            s32 indent;

                            if (widget->flags & GRID_TEXT_PREFIX_ROW_INDEX) {
                                indent = 0x300;
                                if (widget->flags & GRID_TEXT_HEX_ROW_INDEX) {
                                    indent = 0x480;
                                }
                            } else {
                                indent = 0;
                            }
                            uiDrawUniformColorRect(offsetX + indent, offsetY, z,
                                                   width, 0x78,
                                                   0x40408080, surfaceIndex);
                        }
                        itfFormatGridValueEntryText(widget, item, text);
                        fldLmapSubmitPositionedCommandPacket(offsetX, offsetY, z,
                                                             drawMode, text,
                                                             surfaceIndex);
                    }
                    if (item->select != NULL && item == widget->selected &&
                        invokeSelected != 0) {
                        invokeSelected = 0;
                        item->select(widget);
                    }
                    offsetY += 0x70;
                    item = item->next;
                    itemIndex++;
                    if (item == NULL) {
                        break;
                    }
                    if (offsetY >= rowEnd) {
                        break;
                    }
                } while (itemIndex < (u16)widget->rows);
            }
        }
    }
}

GridTextListItem *itfFindGridNodeByKey(u32 key, GridTextWidget *widget) {
    GridTextListItem *item = widget->head;

    while (item != NULL && item->index != key) {
        item = item->next;
    }
    return item;
}

s32 sdfGridSeekSelectedNodeByIndex(s32 index, GridTextWidget *widget) {
    s16 count = widget->itemCount;
    u16 width;
    GridTextListItem *first;

    if (index >= count) {
        return 0;
    }
    first = widget->head;
    widget->cursorRow = 0;
    widget->firstVisible = first;
    widget->selected = first;
    if (index > 0) {
        width = (u16)widget->rows;
        do {
            GridTextListItem *current = widget->firstVisible;
            if (width >= count - current->index) {
                widget->cursorRow++;
            } else {
                widget->firstVisible = current->next;
            }
            current = widget->selected;
            widget->selected = current->next;
        } while (--index != 0);
    }
    return 1;
}

s32 sdfGridSeekFirstNode(GridTextWidget *widget) {
    return sdfGridSeekSelectedNodeByIndex(0, widget);
}

s32 sdfGridSeekLastNode(GridTextWidget *widget) {
    return sdfGridSeekSelectedNodeByIndex(widget->itemCount - 1, widget);
}

INCLUDE_RODATA(const s32, "game/code_002C1588", fldLocalMapTaskName);

