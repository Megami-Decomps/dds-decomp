#ifndef SDF_DEV_WORKER_H
#define SDF_DEV_WORKER_H

#include "common.h"

struct DevState;

/* Shared queue/thread slot from the device-worker table. */
typedef struct DevWorkerEntry {
    s32 threadId;
    s32 semaphore;
    struct DevState *first;
    struct DevState *last;
    u8 opaque10[8];
} DevWorkerEntry;

typedef char DevWorkerEntry_size_must_be_0x18[
    (sizeof(DevWorkerEntry) == 0x18) ? 1 : -1];
typedef char DevWorkerEntry_semaphore_offset_must_be_0x04[
    ((u32)&((DevWorkerEntry *)0)->semaphore == 0x04) ? 1 : -1];
typedef char DevWorkerEntry_first_offset_must_be_0x08[
    ((u32)&((DevWorkerEntry *)0)->first == 0x08) ? 1 : -1];
typedef char DevWorkerEntry_last_offset_must_be_0x0C[
    ((u32)&((DevWorkerEntry *)0)->last == 0x0C) ? 1 : -1];

#endif /* SDF_DEV_WORKER_H */
