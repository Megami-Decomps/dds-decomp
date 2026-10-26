#ifndef SDF_STREAM_READ_API_H
#define SDF_STREAM_READ_API_H

#include "sdf_stream_read_contract.h"

struct SoundFormat;

void sdfIpuStrmDecodeInit(
    struct sdfIpuDecodeCb_t *node, struct SoundFormat *format,
    SdfStreamReadCallback read, struct movieStream_t *readContext);
void sdfSoundInitFormattedAndAppendNode(
    struct sdfIpuDecodeCb_t *node, struct SoundFormat *format,
    SdfStreamReadCallback read, struct movieStream_t *readContext,
    s32 resource);
SdfStreamReadResult movieCopyChunk(
    struct sdfIpuDecodeCb_t *node, struct movieStream_t *movie,
    SdfStreamReadOperation operation, void *dataBuffer, s32 byteCount);
SdfStreamReadResult movieParseHeader(
    struct sdfIpuDecodeCb_t *node, struct movieStream_t *movie,
    SdfStreamReadOperation operation, void *dataBuffer, s32 byteCount);

#endif /* SDF_STREAM_READ_API_H */
