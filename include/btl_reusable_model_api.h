#ifndef BTL_REUSABLE_MODEL_API_H
#define BTL_REUSABLE_MODEL_API_H

#include "common.h"

/* Available means an active unit can supply matching loaded model state. */
typedef s32 BtlReusableModelAvailability;
enum {
    BTL_REUSABLE_MODEL_UNAVAILABLE = 0,
    BTL_REUSABLE_MODEL_AVAILABLE = 1,
};

BtlReusableModelAvailability btlHasMatchingModel(s32 effectId, s32 modelId);

#endif /* BTL_REUSABLE_MODEL_API_H */
