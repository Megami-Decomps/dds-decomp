#ifndef BTL_ACTOR_RESOURCE_BANK_API_H
#define BTL_ACTOR_RESOURCE_BANK_API_H

#include "common.h"
#include "btl_actor_resource_table_kind.h"

/* Return a borrowed pointer into the selected actor-resource bank. The party
 * setup paths use selector 0; opposing-actor paths use selector 1. The provider
 * routes every nonzero selector to the second bank. Entries are separated by
 * 0x270 bytes. Callers interpret the shared payload through the appropriate
 * record/resource view; the returned storage is not a separate allocation. */
void *btlGetActorResourceBankEntry(
    s32 resourceTableKind, s32 index);

#endif
