#ifndef MENU_FADE_ENTRY_H
#define MENU_FADE_ENTRY_H

#include "common.h"

struct MenuWindowContainer;

/* Shared fade-list entry. The unobserved word at 0x04 remains opaque. */
typedef struct MenuFadeEntry {
    u32 active; /* 0x00 */
    u32 unknown_04;
    s32 x; /* 0x08: stored draw position. */
    s32 y; /* 0x0C: stored draw position. */
    struct MenuWindowContainer *window; /* 0x10 */
    s32 fade; /* 0x14 */
} MenuFadeEntry;

typedef char MenuFadeEntry_extent_must_be_0x18[
    (sizeof(MenuFadeEntry) == 0x18) ? 1 : -1];
typedef char MenuFadeEntry_x_offset_must_be_0x08[
    ((u32)&((MenuFadeEntry *)0)->x == 0x08) ? 1 : -1];
typedef char MenuFadeEntry_y_offset_must_be_0x0C[
    ((u32)&((MenuFadeEntry *)0)->y == 0x0C) ? 1 : -1];
typedef char MenuFadeEntry_window_offset_must_be_0x10[
    ((u32)&((MenuFadeEntry *)0)->window == 0x10) ? 1 : -1];
typedef char MenuFadeEntry_fade_offset_must_be_0x14[
    ((u32)&((MenuFadeEntry *)0)->fade == 0x14) ? 1 : -1];

#endif /* MENU_FADE_ENTRY_H */
