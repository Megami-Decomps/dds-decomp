#ifndef SDF_STREAM_READ_CONTRACT_H
#define SDF_STREAM_READ_CONTRACT_H

#include "common.h"

/* QUERY returns buffered bytes and writes an EOF byte to dataBuffer; COPY reads
 * byteCount bytes and returns movie->remainingBytes; RESUME may queue another
 * read when the consumer has room and returns zero. Results are operation-specific. */
typedef s32 SdfStreamReadResult;
typedef s32 SdfStreamReadOperation;
enum SdfStreamReadOperationValue {
    SDF_STREAM_READ_QUERY = 0,
    SDF_STREAM_READ_COPY = 1,
    SDF_STREAM_READ_RESUME = 2
};

struct sdfIpuDecodeCb_t;
struct movieStream_t;

typedef SdfStreamReadResult (*SdfStreamReadCallback)(
    struct sdfIpuDecodeCb_t *node, struct movieStream_t *context,
    SdfStreamReadOperation operation, void *dataBuffer, s32 byteCount);

#endif /* SDF_STREAM_READ_CONTRACT_H */
