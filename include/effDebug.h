#ifndef EFF_DEBUG_H
#define EFF_DEBUG_H

#include "common.h"

/* Native editor items are 0x18 bytes; each type interprets its value words. */
typedef struct effDebugMenuItem_t {
    const char *label;
    u8 type;
    u8 pad5[3];
    void *value;
    union { void *value2; s32 minValue; };
    s32 maxValue;
    s32 step;
} effDebugMenuItem_t;

/* Shared by effect setup and the interactive editor. Setup copies source to
 * destination; the reset action restores source from that saved destination.
 * Native accesses the recreate callback at +0x3C beyond the IDB's 0x3C extent. */
typedef struct effDebugMenu_t {
    const char *title;
    s32 x;
    s32 y;
    s32 selected;
    effDebugMenuItem_t *items;
    s32 itemCount;
    struct effDebugMenu_t *parent;
    void *source;
    void *destination;
    u32 copyBytes;
    s32 mode;
    void *(*create)(void *);
    void (*update)(void *);
    void (*destroy)(void *);
    void **param;
    void *(*recreate)(void *);
} effDebugMenu_t;

effDebugMenu_t *dblurUpdateHandle(effDebugMenu_t *menu);
void effDebugDrawMenu(effDebugMenu_t *menu);

#endif
