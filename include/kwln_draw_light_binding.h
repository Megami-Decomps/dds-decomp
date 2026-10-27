#ifndef KWLN_DRAW_LIGHT_BINDING_H
#define KWLN_DRAW_LIGHT_BINDING_H

#include "common.h"

/* The draw table has three installable light sources followed by a null slot. */
typedef s32 KwlnLightSourceSlot;
enum {
    KWLN_LIGHT_SOURCE_SLOT_0 = 0,
    KWLN_LIGHT_SOURCE_SLOT_1 = 1,
    KWLN_LIGHT_SOURCE_SLOT_2 = 2,
    KWLN_LIGHT_SOURCE_SLOT_COUNT = 3,
};

/* Install caller-owned mutable light data in a draw slot. The draw table
 * retains this address without copying or freeing it, and light-update code
 * may write through it. The source must remain valid while it is installed. */
void dds3SetLight(f32 (*lightSource)[4], KwlnLightSourceSlot slot);

#endif /* KWLN_DRAW_LIGHT_BINDING_H */
