#ifndef ITF_MES_ENTRY_START_API_H
#define ITF_MES_ENTRY_START_API_H

#include "common.h"

/* Outcome reported by starting an entry in a message window. */
typedef s32 ItfMesEntryStartResult;
enum {
    ITF_MES_ENTRY_START_EMPTY_TABLE = 0,
    ITF_MES_ENTRY_START_STARTED = 1
};

ItfMesEntryStartResult itfMesStartEntry(s32 window, s32 entryIndex, s32 itemIndex);

#endif /* ITF_MES_ENTRY_START_API_H */
