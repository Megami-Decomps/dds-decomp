#ifndef BILL_WORK_H
#define BILL_WORK_H

#include "common.h"

/* Partial view shared by the billboard value setters; surrounding bytes stay opaque. */
typedef struct BillWork {
    u8 opaque00[0x64];
    u8 currentValue;
    u8 opaque65[0x4B];
    u16 pendingCount;
    u16 queuedCount;
    u8 opaqueB4[0x0C];
    u8 stagedValue;
} BillWork;

typedef char BillWorkCurrentValueOffsetCheck[
    ((u32)&((BillWork *)0)->currentValue == 0x64) ? 1 : -1];
typedef char BillWorkPendingCountOffsetCheck[
    ((u32)&((BillWork *)0)->pendingCount == 0xB0) ? 1 : -1];
typedef char BillWorkQueuedCountOffsetCheck[
    ((u32)&((BillWork *)0)->queuedCount == 0xB2) ? 1 : -1];
typedef char BillWorkStagedValueOffsetCheck[
    ((u32)&((BillWork *)0)->stagedValue == 0xC0) ? 1 : -1];

#endif
