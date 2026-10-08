#ifndef SDF_MOVIE_STREAM_H
#define SDF_MOVIE_STREAM_H

#include "common.h"

struct SdfMemBlock;

/* Chip-heap work record for the linear movie stream (0x14 bytes). */
typedef struct MovLinearStream {
    struct SdfMemBlock *allocation;
    u8 *bufferStart;
    u8 *readCursor;
    u8 *writeCursor;
    s32 bufferedBytes;
} MovLinearStream;

typedef char MovLinearStream_size_must_be_0x14[
    (sizeof(MovLinearStream) == 0x14) ? 1 : -1];

/* Chip-heap work record for movie-PAC streaming (0x78 bytes). The
 * block-mask allocation and the backing ring allocation are separate owners. */
typedef struct MovPacStream {
    u8 pad00[0x18];
    s32 packetBytes;
    s32 blockBytes;
    u8 scratchBuffer[0x20];
    struct SdfMemBlock *payloadAllocation;
    u8 *blockMask;
    s32 blockIndex;
    struct SdfMemBlock *allocation;
    void *pendingCursor;
    u8 *pacBuffer;
    s32 pacReadOffset;
    s32 pacBufferedBytes;
    u8 *ringBuffer;
    s32 ringOffset;
    s32 ringLength;
    s32 unk6C;
    s32 scratchSize;
    u8 *scratch;
} MovPacStream;

typedef char MovPacStream_size_must_be_0x78[
    (sizeof(MovPacStream) == 0x78) ? 1 : -1];
typedef char MovPacStream_scratch_must_be_at_0x20[
    ((u32)&((MovPacStream *)0)->scratchBuffer == 0x20) ? 1 : -1];
typedef char MovPacStream_payload_allocation_must_be_at_0x40[
    ((u32)&((MovPacStream *)0)->payloadAllocation == 0x40) ? 1 : -1];
typedef char MovPacStream_allocation_must_be_at_0x4C[
    ((u32)&((MovPacStream *)0)->allocation == 0x4C) ? 1 : -1];
typedef char MovPacStream_scratch_pointer_must_be_at_0x74[
    ((u32)&((MovPacStream *)0)->scratch == 0x74) ? 1 : -1];

#endif /* SDF_MOVIE_STREAM_H */
