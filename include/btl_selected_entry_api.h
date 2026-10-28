#ifndef BTL_SELECTED_ENTRY_API_H
#define BTL_SELECTED_ENTRY_API_H

#include "btl_command_id.h"
#include "btl_command_hit_result.h"

struct BtlUnit;

/* The selected entry is an index into datCommandRecords; -1 means none. */
BtlCommandHitResult btlGetActionRecordLookupValue(BtlCommandId commandId);
void btlSetActorSelectedEntryIndex(struct BtlUnit *actor, BtlCommandId commandId);
void btlClearActorSelectedEntryIndex(struct BtlUnit *actor);

#endif /* BTL_SELECTED_ENTRY_API_H */
