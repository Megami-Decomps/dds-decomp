#ifndef BTL_EVENT_SEQUENCE_API_H
#define BTL_EVENT_SEQUENCE_API_H

#include "common.h"

typedef s32 BtlEventSequenceTaskAvailability;
enum {
    BTL_EVENT_SEQUENCE_TASK_NOT_READY = 0,
    BTL_EVENT_SEQUENCE_TASK_READY = 1,
};

BtlEventSequenceTaskAvailability btlIsEventSequenceTaskReady(void);

#endif /* BTL_EVENT_SEQUENCE_API_H */
