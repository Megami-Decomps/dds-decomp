#ifndef FILE_MENU_START_MODE_H
#define FILE_MENU_START_MODE_H

#include "common.h"

/* Entry modes for the paired memory-card menu start functions. */
typedef s32 FileMenuStartMode;
enum {
    FILE_MENU_START_MODE_LOAD = 0,
    FILE_MENU_START_MODE_SLOT_SELECTION = 1,
    FILE_MENU_START_MODE_NEW_CYCLE = 2,
    FILE_MENU_START_MODE_BOOT = 3,
    FILE_MENU_START_MODE_ORIGINAL_TITLE = 4
};

#endif /* FILE_MENU_START_MODE_H */
