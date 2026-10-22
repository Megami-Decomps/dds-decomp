#ifndef MNU_PANEL_ITEM_H
#define MNU_PANEL_ITEM_H

#include "common.h"
#include "itf_grid_slot.h"

#ifdef VERSION_DDS2
/* DDS2's panel item is wider than the DDS1 variant, with five points at +0x74. */
typedef struct MenuPanelItem {
    u8 pad00[0x10];
    s32 value10; /* Pixel span used by signed stat scaling. */
    s32 value14;
    s32 value18;
    u32 option;
    s32 selection;
    u32 value24;
    u32 value28;
    MenuGridSlot spriteGridSlots[9]; /* 0x2C */
    MenuGridSlot gridSlots[5]; /* 0x74 */
    u8 pad9C[4];
    u32 initialValue; /* 0xA0 */
    s32 selectionRamp; /* 0xA4: signed; the detail meter steps and clamps it */
    s32 phase;
} MenuPanelItem;

typedef char MenuPanelItem_dds2_size_check[(sizeof(MenuPanelItem) == 0xAC) ? 1 : -1];
#endif

#endif
