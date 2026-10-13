#ifndef EFF_RING_COLOR_SLOT_H
#define EFF_RING_COLOR_SLOT_H

#include "common.h"

typedef struct EffRingColorSlot {
    s32 colors[5];
} EffRingColorSlot;

typedef char EffRingColorSlot_size[(sizeof(EffRingColorSlot) == 0x14) ? 1 : -1];

#endif
