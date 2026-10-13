#ifndef SDF_STREAM_STATE_H
#define SDF_STREAM_STATE_H

#include "sdf_dev_state.h"

typedef struct SdfStreamReadRequest {
    u8 opaque00[4];
    s32 destinationAddress;
    s32 byteCount;
} SdfStreamReadRequest;

typedef struct SdfStreamState {
    DevState *deviceState;
    s32 semaphore;
    s32 readResult;
    s32 remainingBytes;
    s32 destinationAddress;
    s32 transferredBytes;
} SdfStreamState;

typedef char SdfStreamReadDestinationOffsetCheck[
    (((unsigned int)&((SdfStreamReadRequest *)0)->destinationAddress) == 4) ? 1 : -1];
typedef char SdfStreamReadByteCountOffsetCheck[
    (((unsigned int)&((SdfStreamReadRequest *)0)->byteCount) == 8) ? 1 : -1];
typedef char SdfStreamSemaphoreOffsetCheck[
    (((unsigned int)&((SdfStreamState *)0)->semaphore) == 4) ? 1 : -1];
typedef char SdfStreamRemainingBytesOffsetCheck[
    (((unsigned int)&((SdfStreamState *)0)->remainingBytes) == 0x0C) ? 1 : -1];
typedef char SdfStreamTransferredBytesOffsetCheck[
    (((unsigned int)&((SdfStreamState *)0)->transferredBytes) == 0x14) ? 1 : -1];

#endif /* SDF_STREAM_STATE_H */
