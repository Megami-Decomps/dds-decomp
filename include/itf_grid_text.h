#ifndef DDS_ITF_GRID_TEXT_H
#define DDS_ITF_GRID_TEXT_H

#include "common.h"

typedef struct GridTextWidget GridTextWidget;
typedef struct GridTextListItem GridTextListItem;

/* Numeric item formatting modes stored in GridNumericDescriptor.mode. */
enum {
    GRID_NUMERIC_FORMAT_DECIMAL = 0,
    GRID_NUMERIC_FORMAT_HEXADECIMAL = 1,
    GRID_NUMERIC_FORMAT_FLOAT = 2 /* One fractional digit. */
};

/* Complete 0x14-byte descriptor copied into a numeric item's parameter. */
typedef struct GridNumericDescriptor {
    s32 mode;       /* 0x00 */
    f32 minimum;    /* 0x04 */
    f32 maximum;    /* 0x08 */
    f32 step;       /* 0x0C */
    f32 value;      /* 0x10 */
} GridNumericDescriptor;

/* Display behavior in GridTextWidget.flags. */
enum {
    GRID_TEXT_HIGHLIGHT_SELECTION = 0x4,
    GRID_TEXT_HIGHLIGHT_VALUES = 0x8,
    GRID_TEXT_HIDE_ROWS = 0x40,
    GRID_TEXT_PREFIX_ROW_INDEX = 0x100,
    GRID_TEXT_HEX_ROW_INDEX = 0x200
};

/* Shared grid and local-map list owner. */
struct GridTextWidget {
    char *text;                     /* 0x00 */
    u16 textLength;                 /* 0x04 */
    u16 rows;                       /* 0x06 */
    s16 cursorRow;                  /* 0x08 */
    s16 itemCount;                  /* 0x0A */
    u32 flags;                      /* 0x0C */
    GridTextListItem *firstVisible; /* 0x10 */
    GridTextListItem *head;         /* 0x14 */
    GridTextListItem *selected;     /* 0x18 */
    GridTextListItem *tail;         /* 0x1C */
    s32 x;                          /* 0x20 */
    s32 y;                          /* 0x24 */
    s32 width;                      /* 0x28 */
    s32 height;                     /* 0x2C */
    u32 reference;                  /* 0x30 */
    void (*onSelect)(GridTextWidget *); /* 0x34 */
    void (*onDraw)(s32, s32, s32, GridTextWidget *, s32); /* 0x38 */
    s32 rowOffset;                  /* 0x3C */
};

/* Text rows share this 0x2C-byte node layout in both renderers. */
struct GridTextListItem {
    char *text;                     /* 0x00 */
    u16 textLength;                 /* 0x04 */
    u16 index;                      /* 0x06 */
    void *parameter;                /* 0x08 */
    f32 number;                     /* 0x0C */
    s32 formatWidth;                /* 0x10 */
    u32 value;                      /* 0x14 */
    GridTextListItem *previous;     /* 0x18 */
    GridTextListItem *next;         /* 0x1C */
    GridTextWidget *child;          /* 0x20 */
    void (*select)(GridTextWidget *); /* 0x24 */
    void (*format)(void *, void *, char *, s32); /* 0x28 */
};

typedef char GridTextWidgetSizeCheck[sizeof(GridTextWidget) == 0x40 ? 1 : -1];
typedef char GridTextListItemSizeCheck[sizeof(GridTextListItem) == 0x2C ? 1 : -1];
typedef char GridNumericDescriptorSizeCheck[sizeof(GridNumericDescriptor) == 0x14 ? 1 : -1];

#endif
