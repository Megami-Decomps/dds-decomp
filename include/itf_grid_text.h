#ifndef DDS_ITF_GRID_TEXT_H
#define DDS_ITF_GRID_TEXT_H

#include "common.h"

typedef struct GridTextWidget GridTextWidget;
typedef struct GridTextListItem GridTextListItem;

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

#endif
