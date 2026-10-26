#include "sdk_ps2.h"
#include "common.h"
#include "sdf_resource.h"
#include "sdf.h"
#include "sdf_movie_stream.h"
#include "sdf_movie_state.h"
#include "sdf_stream_read.h"
#include "sdf_dev_event.h"
#include "sdf_dev_state.h"

s32 sdfFileCmdFileSize(sdfFileCmdHandle_t *state);
extern s32 movieStreamSema;

/* Queue up to 0x4000 bytes into the linear ring; state 5 means insufficient room. */
void movieReadNextChunk(movieStream_t *movie) {
    MovLinearStream *stream = movie->stream.linear;
    s32 remaining = movie->remainingBytes;
    s32 readSize;
    u8 *bufferPosition;
    u8 *streamPosition;
    u8 *ringStart;
    u8 *ringEnd;
    sdfFileCmdHandle_t *deviceState;

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
    sdfFileCmdRead(deviceState, bufferPosition, readSize);
}

/* Consume device completions and stop requests for the linear stream; always return 0. */
s32 movieGetStreamSize(
    sdfFileCmdHandle_t *deviceState, SdfMovieDeviceEvent deviceEvent,
    void *data, s32 bytesRead, movieStream_t *movie) {
    MovLinearStream *stream;
    s32 restoreInterrupts;

    movie->deviceState = deviceState;
    stream = movie->stream.linear;

    if (movie->stopRequested != 0 &&
        (movie->state < SDF_MOVIE_STATE_DEVICE_RELEASE_CALLBACK ||
         movie->state > SDF_MOVIE_STATE_STOP_REQUESTED)) {
        movie->state = SDF_MOVIE_STATE_STOP_REQUESTED;
        sdfFileCmdClose(deviceState);
        return 0;
    }

    switch (movie->state) {
    case SDF_MOVIE_STATE_INITIAL:
        if (deviceEvent == SDF_DEV_EVENT_OPENED) {
            movie->state = SDF_MOVIE_STATE_CONTROL_REQUEST;
            sdfFileCmdFileSize(deviceState);
        }
        break;
    case SDF_MOVIE_STATE_CONTROL_REQUEST:
        if (deviceEvent == SDF_DEV_EVENT_SIZE_REPLY) {
            movie->totalBytes = bytesRead;
            movie->remainingBytes = bytesRead;
            movieReadNextChunk(movie);
        }
        break;
    case SDF_MOVIE_STATE_DATA_READ:
        if (deviceEvent == SDF_DEV_EVENT_READ_REPLY) {
            restoreInterrupts = DIntr();
            stream->bufferedBytes += bytesRead;
            movie->remainingBytes -= bytesRead;
            if (restoreInterrupts != 0) {
                EIntr();
            }
            if (movie->remainingBytes == 0) {
                movie->state = SDF_MOVIE_STATE_STOP_REQUESTED;
                sdfFileCmdClose(deviceState);
            } else {
                movieReadNextChunk(movie);
            }
        }
        break;
    case SDF_MOVIE_STATE_STOP_REQUESTED:
        if (deviceEvent == SDF_DEV_EVENT_CLOSED) {
            movie->deviceState = NULL;
            movie->state = SDF_MOVIE_STATE_DEVICE_RELEASE_CALLBACK;
            sdfFileCmdFree(deviceState);
        }
        break;
    }

    return 0;
}

/* Queue a movie-PAC block only when both ring-buffer room predicates allow it. */
void movieReadHeaderChunk(movieStream_t *movie) {
    MovPacStream *stream;
    s32 remaining;

    stream = movie->stream.pac;
    remaining = movie->remainingBytes;
    if (remaining == 0) {
        return;
    }
    if ((0x10000 - stream->pacBufferedBytes) < 0x4000 || ((stream->unk6C - stream->ringLength) + 0x10000) < 0x4000) {
        movie->state = SDF_MOVIE_STATE_WAITING_FOR_BUFFER_SPACE;
        return;
    }
    movie->state = SDF_MOVIE_STATE_DATA_READ;
    sdfFileCmdRead(movie->deviceState, stream->pacInputBuffer, remaining <= 0x4000 ? remaining : 0x4000);
}

/* Process movie-PAC header, block-mask and payload completions; always return 0. */
s32 movieReadChunk(
    sdfFileCmdHandle_t *deviceState, SdfMovieDeviceEvent deviceEvent,
    const void *readBuffer, s32 bytesRead, movieStream_t *movie) {
    MovPacStream *stream;

    movie->deviceState = deviceState;
    stream = movie->stream.pac;

    if (movie->stopRequested != 0 &&
        (movie->state < SDF_MOVIE_STATE_DEVICE_RELEASE_CALLBACK ||
         movie->state > SDF_MOVIE_STATE_STOP_REQUESTED)) {
        movie->state = SDF_MOVIE_STATE_STOP_REQUESTED;
        sdfFileCmdClose(deviceState);
        return 0;
    }

    switch (movie->state) {
    case SDF_MOVIE_STATE_INITIAL:
        if (deviceEvent == SDF_DEV_EVENT_OPENED) {
            movie->state = SDF_MOVIE_STATE_CONTROL_REQUEST;
            sdfFileCmdFileSize(deviceState);
        }
        break;
    case SDF_MOVIE_STATE_CONTROL_REQUEST:
        if (deviceEvent == SDF_DEV_EVENT_SIZE_REPLY) {
            movie->totalBytes = bytesRead;
            movie->remainingBytes = bytesRead;
            movie->state = SDF_MOVIE_STATE_PAC_HEADER_READ;
            sdfFileCmdRead(deviceState, stream, 0x40);
        }
        break;
    case SDF_MOVIE_STATE_PAC_HEADER_READ:
        if (deviceEvent == SDF_DEV_EVENT_READ_REPLY) {
            s32 payloadBytes = stream->packetBytes - 0x40;
            stream->payloadAllocation = sdfMemAllocDirect(payloadBytes);
            stream->blockMask = (u8 *)sdfMemLock(stream->payloadAllocation);
            movie->state = SDF_MOVIE_STATE_PAC_MASK_READ;
            sdfFileCmdRead(deviceState, stream->blockMask, payloadBytes);
            movie->remainingBytes -= stream->packetBytes;
        }
        break;
    case SDF_MOVIE_STATE_PAC_MASK_READ:
        if (deviceEvent == SDF_DEV_EVENT_READ_REPLY) {
            movieReadHeaderChunk(movie);
        }
        break;
    case SDF_MOVIE_STATE_DATA_READ: {
        if (deviceEvent == SDF_DEV_EVENT_READ_REPLY) {
            const u8 *source = readBuffer;
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

                    WaitSema(movieStreamSema);
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
                    SignalSema(movieStreamSema);
                } else {
                    s32 restoreInterrupts = DIntr();

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
                sdfFileCmdClose(deviceState);
            } else {
                movieReadHeaderChunk(movie);
            }
        }
        break;
    }
    case SDF_MOVIE_STATE_STOP_REQUESTED:
        if (deviceEvent == SDF_DEV_EVENT_CLOSED) {
            movie->deviceState = NULL;
            movie->state = SDF_MOVIE_STATE_DEVICE_RELEASE_CALLBACK;
            sdfFileCmdFree(deviceState);
        }
        break;
    }

    return 0;
}

/* Sound/IPU source operations: report available bytes/EOF, copy data, or resume reads. */
SdfStreamReadResult movieCopyChunk(sdfIpuDecodeCb_t *unused, movieStream_t *movie, SdfStreamReadOperation operation, void *dataBuffer, s32 byteCount) {
    MovLinearStream *stream = movie->stream.linear;

    switch (operation) {
    case SDF_STREAM_READ_QUERY:
        if (movie->stopRequested != 0) {
            *(u8 *)dataBuffer = 1;
            return 0;
        }
        if (movie->remainingBytes == 0) {
            *(u8 *)dataBuffer = 1;
        }
        return stream->bufferedBytes;
    case SDF_STREAM_READ_COPY:
        if (movie->stopRequested != 0) {
            return 0;
        }
        {
            u8 *readCursor = stream->readCursor;
            u8 *bufferStart = stream->bufferStart;
            s32 wrappedBytes = (readCursor - bufferStart) + byteCount - 0x20000;

            if (wrappedBytes >= 0) {
                s32 firstSpan = byteCount - wrappedBytes;
                memcpy(dataBuffer, readCursor, firstSpan);
                if (wrappedBytes > 0) {
                    memcpy((u8 *)dataBuffer + firstSpan, bufferStart, wrappedBytes);
                }
                stream->readCursor = bufferStart + wrappedBytes;
            } else {
                memcpy(dataBuffer, readCursor, byteCount);
                stream->readCursor = readCursor + byteCount;
            }
            stream->bufferedBytes -= byteCount;
            return movie->remainingBytes;
        }
    case SDF_STREAM_READ_RESUME:
        if (movie->stopRequested != 0) {
            return 0;
        }
        if (movie->state == SDF_MOVIE_STATE_WAITING_FOR_BUFFER_SPACE) {
            movieReadNextChunk(movie);
        }
        break;
    }

    return 0;
}

/* Sound/IPU source operations for the movie-PAC ring; retain the native copy helper. */
SdfStreamReadResult movieParseHeader(sdfIpuDecodeCb_t *unused, movieStream_t *movie, SdfStreamReadOperation operation, void *dataBuffer, s32 byteCount) {
    void movieMemcpy(void *destination, const void *source, u32 byteCount) {
        memcpy(destination, source, byteCount);
    }
    MovPacStream *stream = movie->stream.pac;

    switch (operation) {
    case SDF_STREAM_READ_QUERY:
        if (movie->stopRequested != 0) {
            *(u8 *)dataBuffer = 1;
            return 0;
        }
        if (movie->remainingBytes == 0) {
            *(u8 *)dataBuffer = 1;
        }
        return stream->pacBufferedBytes;
    case SDF_STREAM_READ_COPY:
        if (movie->stopRequested != 0) {
            return 0;
        }
        {
            u8 *bufferStart = stream->pacBuffer;
            s32 readOffset = stream->pacReadOffset;
            s32 wrappedBytes = readOffset + byteCount - 0x10000;

            if (wrappedBytes >= 0) {
                s32 firstSpan = byteCount - wrappedBytes;
                movieMemcpy(dataBuffer, bufferStart + readOffset, firstSpan);
                if (wrappedBytes > 0) {
                    movieMemcpy((u8 *)dataBuffer + firstSpan, bufferStart, wrappedBytes);
                }
                stream->pacReadOffset = wrappedBytes;
            } else {
                movieMemcpy(dataBuffer, bufferStart + readOffset, byteCount);
                stream->pacReadOffset = readOffset + byteCount;
            }
            stream->pacBufferedBytes -= byteCount;
            return movie->remainingBytes;
        }
    case SDF_STREAM_READ_RESUME:
        if (movie->stopRequested != 0) {
            return 0;
        }
        if (movie->state == SDF_MOVIE_STATE_WAITING_FOR_BUFFER_SPACE) {
            movieReadHeaderChunk(movie);
        }
        break;
    }

    return 0;
}
