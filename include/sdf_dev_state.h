#ifndef SDF_DEV_STATE_H
#define SDF_DEV_STATE_H

#include "common.h"

/* Complete 0x40-byte owner allocated by sdfDevAllocState. */
typedef struct DevState {
    struct DevState *next; /* 0x00 */
    struct DevState *previous; /* 0x04 */
    struct DevState *workerNext; /* 0x08 */
    struct DevState *workerPrev; /* 0x0C */
    char *resolvedPath; /* 0x10 */
    u8 workerIndex; /* 0x14 */
    u8 operation; /* 0x15 */
    s8 state; /* 0x16 */
    u8 pad17; /* 0x17 */
    s32 operationArg; /* 0x18 */
    s32 requestExtra; /* 0x1C */
    void *requestData; /* 0x20 */
    s32 options; /* 0x24 */
    s32 resourceId; /* 0x28 */
    s32 result; /* 0x2C */
    s32 transferred; /* 0x30 */
    u8 pad34[4]; /* 0x34 */
    void (*callback)(struct DevState *, s32, s32, s32, s32); /* 0x38 */
    s32 callbackContext; /* 0x3C: opaque callback-context word */
} DevState;

typedef char DevState_size_must_be_0x40[(sizeof(DevState) == 0x40) ? 1 : -1];
typedef char DevState_resolvedPath_offset_must_be_0x10[
    ((u32)&((DevState *)0)->resolvedPath == 0x10) ? 1 : -1];
typedef char DevState_result_offset_must_be_0x2C[
    ((u32)&((DevState *)0)->result == 0x2C) ? 1 : -1];
typedef char DevState_transferred_offset_must_be_0x30[
    ((u32)&((DevState *)0)->transferred == 0x30) ? 1 : -1];
typedef char DevState_callback_offset_must_be_0x38[
    ((u32)&((DevState *)0)->callback == 0x38) ? 1 : -1];
typedef char DevState_callbackContext_offset_must_be_0x3C[
    ((u32)&((DevState *)0)->callbackContext == 0x3C) ? 1 : -1];

/* Stable state APIs. Callback factories keep caller-specific callback types. */
DevState *sdfDevCreateCommandState(const char *path);
s32 sdfDevQueueRead(DevState *state, void *data, s32 byteCount);
s32 sdfDevQueueWrite(DevState *state, void *data, s32 byteCount);
s32 sdfDevQueueReleaseState(DevState *state);
s32 sdfDevReactivate(DevState *state);
void sdfDevQueueReadAndWait(DevState *state, void *buffer, s32 byteCount);
u32 sdfDevQueueControlAndWait(DevState *state);
void sdfDevWaitThenReleaseCommandState(DevState *state);

#endif /* SDF_DEV_STATE_H */
