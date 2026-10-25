#ifndef ITF_MES_ENTRY_ITEMS_RESULT_H
#define ITF_MES_ENTRY_ITEMS_RESULT_H

#include "common.h"

/* The item-list word is an address-or-sentinel result, not always a pointer. */
typedef u32 ItfMesEntryItemsResult;
enum {
    ITF_MES_ENTRY_ITEMS_NONE = 0,
    ITF_MES_ENTRY_ITEMS_BRANCH = 1,
};

#endif /* ITF_MES_ENTRY_ITEMS_RESULT_H */
