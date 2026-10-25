#ifndef EVT_MENU_TABLE_ENTRY_AVAILABILITY_H
#define EVT_MENU_TABLE_ENTRY_AVAILABILITY_H

#include "common.h"

typedef s32 EvtMenuTableEntryAvailability;
enum {
    EVT_MENU_TABLE_ENTRY_DISABLED = 0,
    EVT_MENU_TABLE_ENTRY_ENABLED = 1
};

EvtMenuTableEntryAvailability evtIsMenuTableEntryEnabled(s32 *index);

#endif /* EVT_MENU_TABLE_ENTRY_AVAILABILITY_H */
