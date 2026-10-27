#ifndef BTL_ACTOR_STATUS_TABLE_API_H
#define BTL_ACTOR_STATUS_TABLE_API_H

#include "common.h"

/* Return a borrowed pointer into the selected side's actor-status bank. Entries
 * are separated by 0x270 bytes, and the returned storage is not a separate
 * allocation for the caller to release. Callers interpret the payload through
 * the appropriate record/resource view. */
void *btlGetSideIndexedActorStatusTable(s32 side, s32 index);

#endif
