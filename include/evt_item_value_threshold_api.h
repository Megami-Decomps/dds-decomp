#ifndef EVT_ITEM_VALUE_THRESHOLD_API_H
#define EVT_ITEM_VALUE_THRESHOLD_API_H

#include "common.h"

typedef u8 EvtItemValueThresholdResult;
enum {
    EVT_ITEM_VALUE_THRESHOLD_NOT_MET = 0,
    EVT_ITEM_VALUE_THRESHOLD_MET = 1
};

/* Flag items test their model flag and ignore minimumQuantity; others compare count against it. */
EvtItemValueThresholdResult evtCheckValueThreshold(s32 itemId, s32 minimumQuantity);

#endif /* EVT_ITEM_VALUE_THRESHOLD_API_H */
