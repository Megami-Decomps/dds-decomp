#include "common.h"
#include "sdf_resource.h"
#include "sdf.h"
#include "sdf_movie_stream.h"
#include "sdf_movie_state.h"
#include "sdf_stream_read.h"
#include "sdf_dev_event.h"
#include "sdf_dev_state.h"

s32 sdfDevQueueControlRequest(DevState *state);
s32 sdfDevQueueActiveOperation(DevState *state);
s32 WaitSema(s32 semaphore);
s32 SignalSema(s32 semaphore);
void *memcpy(void *destination, const void *source, u32 size);
s32 func_00312C08(void);
s32 EIntr(void);
extern s32 D_003BD618;

/* Queue up to 0x4000 bytes into the linear ring; state 5 means insufficient room. */
void func_002ECF70(MovObj *movie) {
    MovLinearStream *stream = movie->stream;
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
        movie->state = SDF_MOVIE_STATE_WAITING_FOR_BUFFER_SPACE;
        return;
    }

    movie->state = SDF_MOVIE_STATE_DATA_READ;
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

/* Consume device completions and stop requests for the linear stream; always return 0. */
s32 sdfMovieHandleLinearDeviceEvent(DevState *deviceState, s32 operation, void *data, s32 bytesRead, MovObj *movie) {
    MovLinearStream *stream;
    s32 restoreInterrupts;

    movie->deviceState = deviceState;
    stream = movie->stream;

    if (movie->stopRequested != 0 &&
        (movie->state < SDF_MOVIE_STATE_DEVICE_RELEASE_CALLBACK ||
         movie->state > SDF_MOVIE_STATE_STOP_REQUESTED)) {
        movie->state = SDF_MOVIE_STATE_STOP_REQUESTED;
        sdfDevQueueActiveOperation(deviceState);
        return 0;
    }

    switch (movie->state) {
    case SDF_MOVIE_STATE_INITIAL:
        if (operation == SDF_DEV_EVENT_OPENED) {
            movie->state = SDF_MOVIE_STATE_CONTROL_REQUEST;
            sdfDevQueueControlRequest(deviceState);
        }
        break;
    case SDF_MOVIE_STATE_CONTROL_REQUEST:
        if (operation == SDF_DEV_EVENT_SIZE_REPLY) {
            movie->totalBytes = bytesRead;
            movie->remainingBytes = bytesRead;
            func_002ECF70(movie);
        }
        break;
    case SDF_MOVIE_STATE_DATA_READ:
        if (operation == SDF_DEV_EVENT_READ_REPLY) {
            restoreInterrupts = func_00312C08();
            stream->bufferedBytes += bytesRead;
            movie->remainingBytes -= bytesRead;
            if (restoreInterrupts != 0) {
                EIntr();
            }
            if (movie->remainingBytes == 0) {
                movie->state = SDF_MOVIE_STATE_STOP_REQUESTED;
                sdfDevQueueActiveOperation(deviceState);
            } else {
                func_002ECF70(movie);
            }
        }
        break;
    case SDF_MOVIE_STATE_STOP_REQUESTED:
        if (operation == SDF_DEV_EVENT_CLOSED) {
            movie->deviceState = NULL;
            movie->state = SDF_MOVIE_STATE_DEVICE_RELEASE_CALLBACK;
            sdfDevQueueReleaseState(deviceState);
        }
        break;
    }

    return 0;
}

/* Queue a movie-PAC block only when both ring-buffer room predicates allow it. */
void sdfMovieProcessPendingData(MovObj *movie) {
    MovPacStream *stream;
    s32 remaining;

    stream = movie->stream;
    remaining = movie->remainingBytes;
    if (remaining == 0) {
        return;
    }
    if ((0x10000 - stream->pacBufferedBytes) < 0x4000 || ((stream->unk6C - stream->ringLength) + 0x10000) < 0x4000) {
        movie->state = SDF_MOVIE_STATE_WAITING_FOR_BUFFER_SPACE;
        return;
    }
    movie->state = SDF_MOVIE_STATE_DATA_READ;
    sdfDevQueueRead(movie->deviceState, stream->pendingCursor, remaining <= 0x4000 ? remaining : 0x4000);
}

/* Process movie-PAC header, block-mask and payload completions; always return 0. */
s32 sdfMovieHandlePacDeviceEvent(DevState *deviceState, s32 operation, void *data, s32 bytesRead, MovObj *movie) {
    MovPacStream *stream;

    movie->deviceState = deviceState;
    stream = movie->stream;

    if (movie->stopRequested != 0 &&
        (movie->state < SDF_MOVIE_STATE_DEVICE_RELEASE_CALLBACK ||
         movie->state > SDF_MOVIE_STATE_STOP_REQUESTED)) {
        movie->state = SDF_MOVIE_STATE_STOP_REQUESTED;
        sdfDevQueueActiveOperation(deviceState);
        return 0;
    }

    switch (movie->state) {
    case SDF_MOVIE_STATE_INITIAL:
        if (operation == 2) {
            movie->state = SDF_MOVIE_STATE_CONTROL_REQUEST;
            sdfDevQueueControlRequest(deviceState);
        }
        break;
    case SDF_MOVIE_STATE_CONTROL_REQUEST:
        if (operation == 4) {
            movie->totalBytes = bytesRead;
            movie->remainingBytes = bytesRead;
            movie->state = SDF_MOVIE_STATE_PAC_HEADER_READ;
            sdfDevQueueRead(deviceState, stream, 0x40);
        }
        break;
    case SDF_MOVIE_STATE_PAC_HEADER_READ:
        if (operation == 5) {
            s32 payloadBytes = stream->packetBytes - 0x40;
            stream->payloadAllocation = sdfAllocGeneralBlock(payloadBytes);
            stream->blockMask = (u8 *)sdfResourceRetainAddress(stream->payloadAllocation);
            movie->state = SDF_MOVIE_STATE_PAC_MASK_READ;
            sdfDevQueueRead(deviceState, stream->blockMask, payloadBytes);
            movie->remainingBytes -= stream->packetBytes;
        }
        break;
    case SDF_MOVIE_STATE_PAC_MASK_READ:
        if (operation == 5) {
            sdfMovieProcessPendingData(movie);
        }
        break;
    case SDF_MOVIE_STATE_DATA_READ: {
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

                    WaitSema(D_003BD618);
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
                    SignalSema(D_003BD618);
                } else {
                    s32 restoreInterrupts = func_00312C08();

                    writeOffset = stream->pacReadOffset + stream->pacBufferedBytes;
                    writeOffset += (writeOffset > 0xFFFF) * -0x10000;
                    memcpy(stream->pacBuffer + writeOffset, source, blockBytes);
                    stream->pacBufferedBytes += blockBytes;
                    if (restoreInterrupts != 0) {
                        EIntr();
                    }
                }
                source += blockBytes;
                stream->blockIndex++;
            } while (--blocksRemaining != 0);

            movie->remainingBytes -= bytesRead;
            if (movie->remainingBytes == 0) {
                movie->state = SDF_MOVIE_STATE_STOP_REQUESTED;
                sdfDevQueueActiveOperation(deviceState);
            } else {
                sdfMovieProcessPendingData(movie);
            }
        }
        break;
    }
    case SDF_MOVIE_STATE_STOP_REQUESTED:
        if (operation == 7) {
            movie->deviceState = NULL;
            movie->state = SDF_MOVIE_STATE_DEVICE_RELEASE_CALLBACK;
            sdfDevQueueReleaseState(deviceState);
        }
        break;
    }

    return 0;
}

/* Sound/IPU source operations: report available bytes/EOF, copy data, or resume reads. */
s32 sdfMovieLinearStreamReadCallback(SdfStreamFrameNode *unused, u32 movieAddress, s32 operation, void *data, s32 size) {
    MovObj *movie = (MovObj *)movieAddress;
    MovLinearStream *stream = movie->stream;

    switch (operation) {
    case SDF_STREAM_READ_QUERY:
        if (movie->stopRequested != 0) {
            *(u8 *)data = 1;
            return 0;
        }
        if (movie->remainingBytes == 0) {
            *(u8 *)data = 1;
        }
        return stream->bufferedBytes;
    case SDF_STREAM_READ_COPY:
        if (movie->stopRequested != 0) {
            return 0;
        }
        {
            u8 *readCursor = stream->readCursor;
            u8 *bufferStart = stream->bufferStart;
            s32 wrappedBytes = (readCursor - bufferStart) + size - 0x20000;

            if (wrappedBytes >= 0) {
                s32 firstSpan = size - wrappedBytes;
                memcpy(data, readCursor, firstSpan);
                if (wrappedBytes > 0) {
                    memcpy((u8 *)data + firstSpan, bufferStart, wrappedBytes);
                }
                stream->readCursor = bufferStart + wrappedBytes;
            } else {
                memcpy(data, readCursor, size);
                stream->readCursor = readCursor + size;
            }
            stream->bufferedBytes -= size;
            return movie->remainingBytes;
        }
    case SDF_STREAM_READ_RESUME:
        if (movie->stopRequested != 0) {
            return 0;
        }
        if (movie->state == SDF_MOVIE_STATE_WAITING_FOR_BUFFER_SPACE) {
            func_002ECF70(movie);
        }
        break;
    }

    return 0;
}

/* Sound/IPU source operations for the movie-PAC ring; retain the native copy helper. */
s32 sdfMoviePacStreamReadCallback(SdfStreamFrameNode *unused, u32 movieAddress, s32 operation, void *data, s32 size) {
    MovObj *movie = (MovObj *)movieAddress;
    void func_002ED740(void *destination, const void *source, u32 byteCount) {
        memcpy(destination, source, byteCount);
    }
    MovPacStream *stream = movie->stream;

    switch (operation) {
    case SDF_STREAM_READ_QUERY:
        if (movie->stopRequested != 0) {
            *(u8 *)data = 1;
            return 0;
        }
        if (movie->remainingBytes == 0) {
            *(u8 *)data = 1;
        }
        return stream->pacBufferedBytes;
    case SDF_STREAM_READ_COPY:
        if (movie->stopRequested != 0) {
            return 0;
        }
        {
            u8 *bufferStart = stream->pacBuffer;
            s32 readOffset = stream->pacReadOffset;
            s32 wrappedBytes = readOffset + size - 0x10000;

            if (wrappedBytes >= 0) {
                s32 firstSpan = size - wrappedBytes;
                func_002ED740(data, bufferStart + readOffset, firstSpan);
                if (wrappedBytes > 0) {
                    func_002ED740((u8 *)data + firstSpan, bufferStart, wrappedBytes);
                }
                stream->pacReadOffset = wrappedBytes;
            } else {
                func_002ED740(data, bufferStart + readOffset, size);
                stream->pacReadOffset = readOffset + size;
            }
            stream->pacBufferedBytes -= size;
            return movie->remainingBytes;
        }
    case SDF_STREAM_READ_RESUME:
        if (movie->stopRequested != 0) {
            return 0;
        }
        if (movie->state == SDF_MOVIE_STATE_WAITING_FOR_BUFFER_SPACE) {
            sdfMovieProcessPendingData(movie);
        }
        break;
    }

    return 0;
}
