#ifndef BTL_ANIMATION_MODE_ENTRY_H
#define BTL_ANIMATION_MODE_ENTRY_H

#include "common.h"

/* Shared animation table row. Only mode and delay are read; the intervening
 * three bytes remain opaque. */
typedef struct BtlAnimationModeRow {
    u8 mode;
    u8 opaque_01[3];
    s32 delay;
} BtlAnimationModeRow;

typedef char BtlAnimationModeRowSizeCheck[
    sizeof(BtlAnimationModeRow) == 8 ? 1 : -1];

#endif
