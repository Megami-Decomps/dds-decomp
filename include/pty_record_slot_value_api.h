#ifndef PTY_RECORD_SLOT_VALUE_API_H
#define PTY_RECORD_SLOT_VALUE_API_H

#include "common.h"
#include "dat_stat.h"

/* Return the indexed item's record value plus its stored bonus for one base stat. */
s32 ptyGetCombinedRecordAndSlotValue(s32 itemId, DatBaseStatIndex baseStatIndex);

#endif /* PTY_RECORD_SLOT_VALUE_API_H */
