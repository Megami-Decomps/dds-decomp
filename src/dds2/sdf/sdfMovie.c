#include "common.h"

typedef struct DevState DevState;
typedef struct MemBlock MemBlock;

typedef struct MovSub {
    u8 pad0[0x4];
    u8 *bufferStart;
    u8 *readCursor;
    u8 *writeCursor;
    s32 bufferedBytes;
    u8 pad14[0x4];
    s32 packetBytes;
    s32 blockBytes;
    u8 pad20[0x20];
    MemBlock *payloadAllocation;
    u8 *blockMask;
    s32 blockIndex;
    u8 pad4C[0x4];
    void *pendingCursor;
    u8 *linearBuffer;
    s32 linearBaseOffset;
    s32 linearWriteOffset;
    u8 *ringBuffer;
    s32 ringOffset;
    s32 ringLength;
    s32 unk6C;
} MovSub;

typedef struct MovObj {
    u8 unk0;
    u8 state;
    u8 stopRequested;
    u8 pad3;
    u8 pad4[0xC];
    DevState *deviceState;
    s32 totalBytes;
    s32 remainingBytes;
    MovSub *stream;
} MovObj;

s32 sdfDevQueueRead(DevState *state, void *data, s32 size);
s32 sdfDevQueueControlRequest(DevState *state);
s32 sdfDevQueueActiveOperation(DevState *state);
s32 sdfDevQueueReleaseState(DevState *state);
s32 sdfAllocGeneralBlock(s32 size);
u8 *sdfResourceRetainAddress(MemBlock *block);
s32 WaitSema(s32 semaphore);
s32 SignalSema(s32 semaphore);
void *memcpy(void *destination, const void *source, u32 size);
s32 func_0036DE70(void);
s32 EIntr(void);
extern s32 D_00438D08;

void func_00345E18(MovObj *movie) {
    MovSub *stream = movie->stream;
    s32 remaining = movie->remainingBytes;
    s32 readSize;
    u8 *bufferPosition;
    u8 *streamPosition;
    u8 *ringStart;
    u8 *ringEnd;
    DevState *deviceState;

    if (remaining == 0) {
        return;
    }
    if (0x20000 - stream->bufferedBytes < 0x4000) {
        movie->state = 5;
        return;
    }

    movie->state = 4;
    readSize = 0x4000;
    if (remaining <= 0x4000) {
        readSize = remaining;
    }

    bufferPosition = stream->writeCursor;
    ringStart = stream->bufferStart;
    ringEnd = ringStart + 0x20000;
    deviceState = movie->deviceState;
    streamPosition = bufferPosition + readSize;
    if (streamPosition >= ringEnd) {
        streamPosition = ringStart;
    }
    stream->writeCursor = streamPosition;
    sdfDevQueueRead(deviceState, bufferPosition, readSize);
}

s32 func_00345EB0(DevState *deviceState, s32 operation, void *data, s32 bytesRead, MovObj *movie) {
    MovSub *stream;
    s32 restoreInterrupts;

    movie->deviceState = deviceState;
    stream = movie->stream;

    if (movie->stopRequested != 0 && (movie->state < 6 || movie->state > 7)) {
        movie->state = 7;
        sdfDevQueueActiveOperation(deviceState);
        return 0;
    }

    switch (movie->state) {
    case 0:
        if (operation == 2) {
            movie->state = 1;
            sdfDevQueueControlRequest(deviceState);
        }
        break;
    case 1:
        if (operation == 4) {
            movie->totalBytes = bytesRead;
            movie->remainingBytes = bytesRead;
            func_00345E18(movie);
        }
        break;
    case 4:
        if (operation == 5) {
            restoreInterrupts = func_0036DE70();
            stream->bufferedBytes += bytesRead;
            movie->remainingBytes -= bytesRead;
            if (restoreInterrupts != 0) {
                EIntr();
            }
            if (movie->remainingBytes == 0) {
                movie->state = 7;
                sdfDevQueueActiveOperation(deviceState);
            } else {
                func_00345E18(movie);
            }
        }
        break;
    case 7:
        if (operation == 7) {
            movie->deviceState = NULL;
            movie->state = 6;
            sdfDevQueueReleaseState(deviceState);
        }
        break;
    }

    return 0;
}

void sdfMovieProcessPendingData(MovObj *movie) {
    MovSub *stream;
    s32 remaining;

    stream = movie->stream;
    remaining = movie->remainingBytes;
    if (remaining == 0) {
        return;
    }
    if ((0x10000 - stream->linearWriteOffset) < 0x4000 || ((stream->unk6C - stream->ringLength) + 0x10000) < 0x4000) {
        movie->state = 5;
        return;
    }
    movie->state = 4;
    sdfDevQueueRead(movie->deviceState, stream->pendingCursor, remaining <= 0x4000 ? remaining : 0x4000);
}

s32 func_003460D8(DevState *deviceState, s32 operation, void *data, s32 bytesRead, MovObj *movie) {
    MovSub *stream;

    movie->deviceState = deviceState;
    stream = movie->stream;

    if (movie->stopRequested != 0 && (movie->state < 6 || movie->state > 7)) {
        movie->state = 7;
        sdfDevQueueActiveOperation(deviceState);
        return 0;
    }

    switch (movie->state) {
    case 0:
        if (operation == 2) {
            movie->state = 1;
            sdfDevQueueControlRequest(deviceState);
        }
        break;
    case 1:
        if (operation == 4) {
            movie->totalBytes = bytesRead;
            movie->remainingBytes = bytesRead;
            movie->state = 2;
            sdfDevQueueRead(deviceState, stream, 0x40);
        }
        break;
    case 2:
        if (operation == 5) {
            s32 payloadBytes = stream->packetBytes - 0x40;
            stream->payloadAllocation = (MemBlock *)sdfAllocGeneralBlock(payloadBytes);
            stream->blockMask = sdfResourceRetainAddress(stream->payloadAllocation);
            movie->state = 3;
            sdfDevQueueRead(deviceState, stream->blockMask, payloadBytes);
            movie->remainingBytes -= stream->packetBytes;
        }
        break;
    case 3:
        if (operation == 5) {
            sdfMovieProcessPendingData(movie);
        }
        break;
    case 4: {
        if (operation == 5) {
            u8 *source = data;
            s32 blockBytes = stream->blockBytes;
            s32 blocksRemaining = bytesRead / blockBytes;
            do {
                s32 blockIndex = stream->blockIndex;
                s32 byteIndex = blockIndex / 8;
                s32 bitIndex = blockIndex - byteIndex * 8;
                s32 blockFlag = 1 << bitIndex;
                s32 writeOffset;
                if ((stream->blockMask[byteIndex] & blockFlag) != 0) {
                    s32 ringLength;
                    s32 wrappedBytes;

                    WaitSema(D_00438D08);
                    ringLength = stream->ringLength;
                    writeOffset = stream->ringOffset + ringLength;
                    writeOffset += (writeOffset > 0xFFFF) * -0x10000;
                    wrappedBytes = writeOffset + blockBytes - 0x10000;
                    if (wrappedBytes > 0) {
                        memcpy(stream->ringBuffer + writeOffset, source, blockBytes - wrappedBytes);
                        memcpy(stream->ringBuffer, source + blockBytes - wrappedBytes, wrappedBytes);
                    } else {
                        memcpy(stream->ringBuffer + writeOffset, source, blockBytes);
                    }
                    wrappedBytes = ringLength + blockBytes;
                    if (wrappedBytes > 0x10000) {
                        stream->ringOffset += wrappedBytes - 0x10000;
                        if (stream->ringOffset > 0xFFFF) {
                            stream->ringOffset -= 0x10000;
                        }
                        stream->ringLength = 0x10000;
                    } else {
                        stream->ringLength = wrappedBytes;
                    }
                    SignalSema(D_00438D08);
                } else {
                    s32 restoreInterrupts = func_0036DE70();

                    writeOffset = stream->linearBaseOffset + stream->linearWriteOffset;
                    writeOffset += (writeOffset > 0xFFFF) * -0x10000;
                    memcpy(stream->linearBuffer + writeOffset, source, blockBytes);
                    stream->linearWriteOffset += blockBytes;
                    if (restoreInterrupts != 0) {
                        EIntr();
                    }
                }
                source += blockBytes;
                stream->blockIndex++;
            } while (--blocksRemaining != 0);

            movie->remainingBytes -= bytesRead;
            if (movie->remainingBytes == 0) {
                movie->state = 7;
                sdfDevQueueActiveOperation(deviceState);
            } else {
                sdfMovieProcessPendingData(movie);
            }
        }
        break;
    }
    case 7:
        if (operation == 7) {
            movie->deviceState = NULL;
            movie->state = 6;
            sdfDevQueueReleaseState(deviceState);
        }
        break;
    }

    return 0;
}

INCLUDE_ASM(const s32, "sdf/sdfMovie", func_00346468);
