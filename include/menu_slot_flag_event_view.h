#ifndef MENU_SLOT_FLAG_EVENT_VIEW_H
#define MENU_SLOT_FLAG_EVENT_VIEW_H

#include "common.h"
#include "evt_world.h"

/* The terminal-scene prefix read by its flag-event message callbacks. */
typedef struct MenuSlotFlagEventView {
    u8 pad00[0x5C];
    EvtResourcePair messageResources; /* +0x5C: dataAddress at +0x60 */
    u8 pad64[0x20];
    s32 reduced; /* +0x84 */
} MenuSlotFlagEventView;

typedef char MenuSlotFlagEventView_size_check[
    (sizeof(MenuSlotFlagEventView) == 0x88) ? 1 : -1];
typedef char MenuSlotFlagEventView_message_data_check[
    ((u32)&((MenuSlotFlagEventView *)0)->messageResources.dataAddress == 0x60) ? 1 : -1];
typedef char MenuSlotFlagEventView_reduced_check[
    ((u32)&((MenuSlotFlagEventView *)0)->reduced == 0x84) ? 1 : -1];

#endif /* MENU_SLOT_FLAG_EVENT_VIEW_H */
