#ifndef BTL_ENTRY_HEIGHT_LIMIT_API_H
#define BTL_ENTRY_HEIGHT_LIMIT_API_H

#include "common.h"

typedef s32 BtlEntryHeightLimitResult;

enum {
    BTL_ENTRY_HEIGHT_LIMIT_RESULT_EXCEEDS_LIMIT = 0,
    BTL_ENTRY_HEIGHT_LIMIT_RESULT_WITHIN_LIMIT = 1,
};

BtlEntryHeightLimitResult btlIsEntryHeightWithinLimit(void);

#endif /* BTL_ENTRY_HEIGHT_LIMIT_API_H */
