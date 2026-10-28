#ifndef BTL_PRIORITY_INPUT_AVAILABILITY_API_H
#define BTL_PRIORITY_INPUT_AVAILABILITY_API_H

#include "common.h"

typedef s32 BtlPriorityInputAvailability;
enum {
    BTL_PRIORITY_INPUT_UNAVAILABLE = 0,
    BTL_PRIORITY_INPUT_AVAILABLE = 1,
};

/* Whether command-panel selection should enter priority-input state. */
BtlPriorityInputAvailability btlHasHighPriorityState(void);

#endif /* BTL_PRIORITY_INPUT_AVAILABILITY_API_H */
