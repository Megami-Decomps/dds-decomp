#ifndef BTL_ACTOR_STATUS_TABLE_API_H
#define BTL_ACTOR_STATUS_TABLE_API_H

#include "common.h"

/* Return the selected side's actor-status entry; entries are separated by 0x270 bytes.
 * Callers interpret the payload through the appropriate record/resource view. */
void *btlGetSideIndexedActorStatusTable(s32 side, s32 index);

#endif
