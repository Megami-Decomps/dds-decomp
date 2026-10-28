#ifndef BTL_ACTION_ENTRY_IS_EMPTY_API_H
#define BTL_ACTION_ENTRY_IS_EMPTY_API_H

#include "btl_command_id.h"
#include "btl_task.h"

/* Test entry emptiness after command/group exclusions; invalid IDs skip command checks. */
typedef s32 BtlActionEntryEmptyResult;

#define BTL_ACTION_ENTRY_NOT_EMPTY ((BtlActionEntryEmptyResult)0)
#define BTL_ACTION_ENTRY_EMPTY ((BtlActionEntryEmptyResult)1)

BtlActionEntryEmptyResult btlActionEntryIsEmpty(
    BtlCommandId commandId, BtlOperandGroup *group, BtlOperandEntry *entry);

#endif /* BTL_ACTION_ENTRY_IS_EMPTY_API_H */
