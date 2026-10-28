#ifndef BTL_ACTION_ENTRY_IS_EMPTY_API_H
#define BTL_ACTION_ENTRY_IS_EMPTY_API_H

#include "btl_command_id.h"
#include "btl_task.h"

/* Test entry emptiness after command/group exclusions; invalid IDs skip command checks. */
s32 btlActionEntryIsEmpty(BtlCommandId commandId, BtlOperandGroup *group,
                          BtlOperandEntry *entry);

#endif /* BTL_ACTION_ENTRY_IS_EMPTY_API_H */
