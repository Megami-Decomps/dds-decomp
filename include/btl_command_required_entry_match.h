#ifndef BTL_COMMAND_REQUIRED_ENTRY_MATCH_H
#define BTL_COMMAND_REQUIRED_ENTRY_MATCH_H

#include "btl_command_id.h"

typedef s32 BtlCommandRequiredEntryMatchResult;

enum {
    BTL_COMMAND_REQUIRED_ENTRY_MATCH_NONE = 0,
    BTL_COMMAND_REQUIRED_ENTRY_MATCHED = 3,
};

struct BtlIndexList;

/* Return MATCHED when the command's required entry codes match the selected list. */
BtlCommandRequiredEntryMatchResult btlCheckCommandRequiredEntryMatches(
    struct BtlIndexList *indexList, BtlCommandId commandId);

#endif /* BTL_COMMAND_REQUIRED_ENTRY_MATCH_H */
