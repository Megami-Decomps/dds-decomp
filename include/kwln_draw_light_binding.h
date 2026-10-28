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
    KWLN_LIGHT_SOURCE_NULL_SLOT = KWLN_LIGHT_SOURCE_SLOT_COUNT,
    KWLN_LIGHT_SOURCE_BINDING_COUNT = 4,
};

/* The runtime binding table has one reserved null entry after its three live
 * sources; packet-building inputs use only the three live entries. */
typedef f32 (*KwlnDrawLightSourceTable[KWLN_LIGHT_SOURCE_BINDING_COUNT])[4];
typedef char KwlnDrawLightSourceTable_size_must_be_0x10[
    (sizeof(KwlnDrawLightSourceTable) == 0x10) ? 1 : -1];

/* Install caller-owned mutable light data in a draw slot. The draw table
 * retains this address without copying or freeing it, and light-update code
 * may write through it. The source must remain valid while it is installed. */
void dds3SetLight(f32 (*lightSource)[4], KwlnLightSourceSlot slot);

#endif /* KWLN_DRAW_LIGHT_BINDING_H */
