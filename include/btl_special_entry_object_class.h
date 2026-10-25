#ifndef BTL_SPECIAL_ENTRY_OBJECT_CLASS_H
#define BTL_SPECIAL_ENTRY_OBJECT_CLASS_H

#include "common.h"

typedef s32 BtlSpecialEntryObjectClass;

enum BtlSpecialEntryObjectClass {
    BTL_SPECIAL_ENTRY_CLASS_NONE = 0,
    BTL_SPECIAL_ENTRY_CLASS_RECORD_FLAG = 1, /* Party-record flag bit 14 is set. */
    BTL_SPECIAL_ENTRY_CLASS_UNIT_ID_CASE = 2 /* Provider's explicit unit-ID cases. */
};

#endif /* BTL_SPECIAL_ENTRY_OBJECT_CLASS_H */
