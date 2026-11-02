#include "sdk_ps2.h"
#include "common.h"
#include "sdf_handler_node.h"
#include "eeregs.h"
#include "snd_command_packet.h"
#include "mnu_voice_resource_path_api.h"
#include "sdf_resource.h"
#include "sdf_packed_resource.h"
#include "sdf_pac_reloc.h"
#include "sdf.h"
#include "sdf_gs_mem_delayed_free_api.h"
#include "sdf_semaphore_api.h"
#include "sdf_movie_stream.h"
#include "sdf_packet_alloc.h"
#include "sdf_packet_list.h"
#include "sdf_gs_packet.h"
#include "sdf_texture_lists.h"
#include "sdf_draw.h"
#include "sdf_resource_list_item.h"
#include "ee_mmi.h"
#include "sdf_stream_read.h"
#include "sdf_stream_read_api.h"
#include "sdf_dev_event.h"
#include "sdf_dev_state.h"
#include "mdl_object_stream.h"
#include "sdf_texture_offset_list.h"
#include "sdf_texture_file.h"
#include "sdf_texture_queue.h"
#include "sdf_stream_input_dma.h"
#include "sdf_sound_rpc_records.h"
#include "snd_midi_state_views.h"
#include "mnu_title_sound.h"
#include "sdf_movie_draw_packet.h"
#include "sdf_movie_state.h"
#include "sdf_chip.h"
#include "sdf_movie_api.h"
#include "file_pac.h"
#include "sdf_pac_callback.h"

extern u32 p2sdPcmState;

extern u32 p2sdPcmBusy;

extern u32 D_00438D0C;

extern s32 sdfMovieDrawLeft;

extern s32 sdfMovieDrawTop;

extern s32 sdfMovieDrawRight;

extern s32 sdfMovieDrawBottom;

extern u32 sdfMovieDrawColor;

extern SdfSoundCommand D_00438B84;

extern char sdfGsMemoryTypeFormat[]; /* "%d" */

extern char D_00438BE0[];

extern char D_00438BE8[];

extern char D_00438BF0[];

extern char D_00438BF8[];

extern char D_00438C00[];

extern char D_00438C08[];

extern char D_00438C10[];

extern char D_00438C18[];

extern char D_00438C20[];

extern char D_00438CD0[];

extern char D_00438CD8[];

/* Send opcode 0x120 with the channel's group (bits 3-6) unless a command is * pending; returns nonzero when busy. */ s32 p2sdPlayPcm(u8 channel);

SdfSoundReadCallback p2sdSetPcmCallback(SdfSoundReadCallback callback);

extern u32 D_00438D04;

/* Print a string followed by a newline through libc puts. */
void sdfPrintTextLine(const char *text) {
    puts(text);
}

#ifdef NON_MATCHING
#include "common.h"
#include "sdf.h"
#include "sdf_movie_stream.h"
#include "sdk_libkernl.h"

#define MOV_PAC_RING_BYTES 0x10000

extern movieStream_t *movieSemaId;            /* the movie whose PAC stream feeds the sound driver */
extern s32 movieSemaCount;                /* the frame at which the PAC ring last ran out of room */
extern s32 movieStreamSema;

/* The sound driver's block allocator for a PAC movie (queued through
 * p2sdSetPcmCallback): reserve size bytes of the stream's ring, first
 * from the pending scratch span, then from the ring itself, wrapping at
 * 64 KB. NULL when the ring is full, after recording the frame that ran out
 * of room. Upstream: INCLUDE_ASM in both games; written from the retail
 * routine with upstream's movie types. The same loads, stores and calls in the
 * same order. */
const u8 *movieWaitSema(s32 size) {
    movieStream_t *movie = movieSemaId;
    MovPacStream *pac = movie->stream.pac;
    u8 *address;

    WaitSema(movieStreamSema);
    if (pac->scratchSize > 0) {
        pac->scratchSize -= size;
        address = pac->scratch;
        pac->scratch = address + size;
    } else {
        s32 offset = pac->ringOffset;

        address = pac->ringBuffer + offset;
        if (pac->ringLength < size) {
            address = NULL;
            movieSemaCount = movie->soundNode.playbackFrameIndex;
        } else {
            pac->ringLength -= size;
            offset += size;
            if (offset >= MOV_PAC_RING_BYTES) {
                offset -= MOV_PAC_RING_BYTES;
            }
            pac->ringOffset = offset;
        }
    }
    SignalSema(movieStreamSema);
    return address;
}
#else
INCLUDE_ASM(const s32, "sdf/sdfMovie", movieWaitSema);
#endif

/* Store the stream-frame draw-path mode; movie viewport setup selects 1. */
void sdfSetStreamFrameDrawMode(u32 value) {
    D_00438D0C = value;
}

void sdfSetGridScaledDrawBounds(s32 firstStart, s32 secondStart, s32 firstLength, s32 secondLength, u32 value) {
    sdfMovieDrawLeft = firstStart * 0x10 + 0x7000;
    sdfMovieDrawTop = secondStart * 8 + 0x7900;
    sdfMovieDrawRight = sdfMovieDrawLeft + firstLength * 0x10;
    sdfMovieDrawBottom = sdfMovieDrawTop + secondLength * 8;
    sdfMovieDrawColor = value;
}

#ifdef NON_MATCHING
#include "common.h"
#include "eeregs.h"
#include "snd_command_packet.h"
#include "mnu_voice_resource_path_api.h"
#include "ee_mmi.h"
#include "sdf_resource.h"
#include "sdf_packed_resource.h"
#include "sdf_pac_reloc.h"
#include "sdf.h"
#include "sdf_texture_lists.h"
#include "sdf_draw.h"
#include "sdf_resource_list_item.h"
#include "sdf_stream_read.h"
#include "sdf_dev_event.h"
#include "sdf_dev_state.h"
#include "mdl_object_stream.h"
#include "sdf_texture_offset_list.h"
#include "sdf_texture_file.h"
#include "sdf_texture_queue.h"
#include "sdf_stream_input_dma.h"
#include "sdf_sound_rpc_records.h"
#include "snd_midi_state_views.h"
#include "sdf_movie_stream.h"
#include "sdf_packet_alloc.h"
#include "sdf_packet_list.h"
extern s32 p2sdPlayPcm(u8 channel);

extern s32 sdfMovieDrawLeft;
extern s32 sdfMovieDrawTop;
extern s32 sdfMovieDrawRight;
extern s32 sdfMovieDrawBottom;
extern u32 sdfMovieDrawColor;

extern movieStream_t *movieSemaId;


/* The bit length of x > 0: 31 - plzcw(x). For x = size - 1 it is the size's
 * exponent rounded up, as TEX0's TW and TH take it. */
static inline s32 bitLength(s32 x) {
    s32 leading;

    EE_MMI_PLZCW(leading, x);
    return 31 - (leading & 0xFF);
}

static inline u64 movieVertexWord(s32 x, s32 y, u32 depth) {
    return ((u64)depth << 32) | ((u32)y << 16) | (u32)x;
}

/* Queue the decoded movie frame as a sprite over the viewport bounds and color
 * set by sdfSetGridScaledDrawBounds, once playback has begun; with the PAC
 * stream enabled, start its sound once the ring holds a page. sdfMovieDrawFrame
 * calls it when the stream-frame draw mode is 1. Upstream: INCLUDE_ASM in
 * both games; written from the retail routine. It differs from retail
 * only in register allocation, scheduling, the way the color word is masked,
 * and the handwritten addi that loads 31 for the texture sizes. */
void sdfMovieQueueFrameSprite(movieStream_t *movie, sdfGrOt_t *pool) {
    u64 *packet;
    sdfDrawTag_t *packetList;
    u32 width;
    u32 height;
    s32 heightLog2;
    s32 widthLog2;
    u32 pageCount;
    u64 pageBase;
    u64 textureWord;

    if (movie->soundNode.playbackPhase < 2) {
        return;
    }

    if (movie->pacEnabled != 0 && movie->pad21 == 0 &&
        movie->stream.pac->ringLength >= 0x1000) {
        movie->pad21 = 1;
        movieSemaId = movie;
        p2sdSetPcmCallback(movieWaitSema);
        p2sdPlayPcm(movie->packetLimit);
    }

    packet = (u64 *)(u32)sdfGrAllocPacketBuff(0xC0);
    packet[0] = 0x0100000BULL;
    packet[1] = 0x5000000B00000000ULL;
    packet[2] = 0x10AB40000000800AULL;
    packet[3] = 0xE;
    packet[4] = 0x21;
    packet[5] = 0x14;

    height = movie->soundNode.height;
    width = movie->soundNode.width;
    heightLog2 = bitLength(height - 1);
    widthLog2 = bitLength(width - 1);
    pageCount = width >> 6;
    if (pageCount == 0) {
        pageCount = 1;
    }
    pageBase = (u32)movie->soundNode.resourceWord >> 6;
    textureWord = ((u64)pageCount << 14) | ((u64)heightLog2 << 30) |
                  (u64)(widthLog2 << 26) | (1ULL << 34);
    if (movie->soundNode.audioMode != 0) {
        textureWord |= pageBase;
        pageBase = 0x200000;
    }
    packet[6] = textureWord | pageBase;

    packet[7] = 6;
    packet[8] = 5;
    packet[9] = 8;
    packet[10] = 0x0000004000000080ULL;
    packet[11] = 0x3B;
    packet[12] = ((u64)(sdfMovieDrawColor >> 24) << 32) | 0x64;
    packet[13] = 0x42;
    packet[14] = (u64)(s64)(s32)(0x80000000U | (sdfMovieDrawColor & 0xFFFFFF));
    packet[15] = 1;
    packet[16] = 0;
    packet[17] = 3;
    packet[18] = movieVertexWord(sdfMovieDrawLeft, sdfMovieDrawTop, movie->drawZ);
    packet[19] = 5;
    packet[20] = (u64)(s64)(s32)((height << 20) | (width << 4));
    packet[21] = 3;
    packet[22] = movieVertexWord(sdfMovieDrawRight, sdfMovieDrawBottom, movie->drawZ);
    packet[23] = 5;

    packetList = sdfPrimAllocDrawTag(NULL);
    sdfDrawTagLink(packetList, (u32)packet);
    pool->append(pool, packetList);
}
#else
INCLUDE_ASM(const s32, "sdf/sdfMovie", sdfMovieQueueFrameSprite);
#endif

extern movieStream_t *movieSemaId;

extern const u8 *movieWaitSema(s32);

extern void sdfMovieQueueFrameSprite(movieStream_t *, sdfGrOt_t *);

void sdfMovieDrawFrame(movieStream_t *movie, sdfGrOt_t *pool) {
    SdfMovieDrawPacket *packet;
    sdfDrawTag_t *list;
    u16 width;
    u16 height;
    s32 widthLog2;
    s32 heightLog2;
    u32 bufferWidth;
    u32 basePointer;

    if (movie->soundNode.playbackPhase < 2) {
        return;
    }
    if (D_00438D0C == 1) {
        sdfMovieQueueFrameSprite(movie, pool);
        return;
    }
    if (movie->pacEnabled != 0 && movie->pad21 == 0 &&
        movie->stream.pac->ringLength >= 0x1000) {
        movie->pad21 = 1;
        movieSemaId = movie;
        p2sdSetPcmCallback(movieWaitSema);
        p2sdPlayPcm(movie->packetLimit);
    }

    packet = (SdfMovieDrawPacket *)sdfGrAllocPacketBuff(sizeof(SdfMovieDrawPacket));
    packet->header.dmaTag = 0x100000A;
    height = movie->soundNode.height;
    width = movie->soundNode.width;
    packet->header.vifCommands = 0x5000000A00000000ULL;
    packet->header.gifTag = 0x108B400000008009ULL;
    packet->header.gifRegisters = SDF_GIF_REGISTER_AD;
    packet->sampling.value = 0x21;
    packet->sampling.registerId = SDF_GS_TEX1_1;
    heightLog2 = height - 1;
    widthLog2 = width - 1;
    bufferWidth = width >> 6;
    EE_MMI_BIT_LENGTH(heightLog2);
    EE_MMI_BIT_LENGTH(widthLog2);
    basePointer = (u32)movie->soundNode.resourceWord >> 6;
    if (bufferWidth == 0) {
        bufferWidth = 1;
    }
    packet->texture.value = ((u64)1 << 34) | ((u64)heightLog2 << 30) |
        (widthLog2 << 26) | (movie->soundNode.audioMode != 0 ? 0x200000 : 0) |
        ((u64)bufferWidth << 14) | (u64)basePointer;
    packet->texture.registerId = SDF_GS_TEX0_1;
    packet->clamp.value = 5;
    packet->clamp.registerId = SDF_GS_CLAMP_1;
    packet->textureAlpha.value = 0x4000000080ULL;
    packet->textureAlpha.registerId = SDF_GS_TEXA;
    packet->color.value = (s32)0x80808080;
    packet->color.registerId = SDF_GS_RGBAQ;
    packet->sourceOrigin.value = 0;
    packet->sourceOrigin.registerId = 3;
    packet->vertexOrigin.value = ((u64)movie->drawZ << 32) |
        (u32)(movie->drawY << 16) | (u32)movie->drawX;
    packet->vertexOrigin.registerId = SDF_GS_XYZ2;
    packet->sourceExtent.value = (height << 20) | (width << 4);
    packet->sourceExtent.registerId = 3;
    packet->vertexExtent.value = ((u64)movie->drawZ << 32) |
        (u32)((movie->drawY + movie->drawHeight) << 16) |
        (u32)(movie->drawX + movie->drawWidth);
    packet->vertexExtent.registerId = SDF_GS_XYZ2;
    list = sdfPrimAllocDrawTag(0);
    sdfDrawTagLink(list, (u32)packet);
    pool->append(pool, list);
}

SdfFileCommandQueueResult sdfFileCmdRequestFileSize(sdfFileCmdHandle_t *state);

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
    sdfFileCmdHandle_t *deviceState, SdfDevEvent deviceEvent,
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
            sdfFileCmdRequestFileSize(deviceState);
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
    sdfFileCmdHandle_t *deviceState, SdfDevEvent deviceEvent,
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
            sdfFileCmdRequestFileSize(deviceState);
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

typedef struct PacAlloc {
    s32 entryCount;
    s32 entryIndex;
    u8 pad8[24];
    s32 resource;
} PacAlloc;

extern sdfFileCmdHandle_t *sdfFileCmdReadOpen(const char *path, void *callback, void *callbackContext);

extern s32 movieSemaCount;

extern u8 D_00438D28[];

/* Initialize an inline sound/IPU node and the selected linear or movie-PAC stream. */
void movieOpenStream(movieStream_t *owner, const SdfMovieDescriptor *descriptor, const char *name) {
    const SdfMovieDescriptor *borrowedDescriptor = descriptor;
    SoundFormat soundFormat;
    struct sdfMemHandle_t *allocation;
    u8 *resource;
    MovLinearStream *stream;
    MovPacStream *pacWork;
    char *extension;
    s32 isPac;

    memset(owner, 0, 0xB0);
    owner->active = 1;
    extension = strrchr(name, '.');
    isPac = 0;
    if (extension != NULL) {
        isPac = strcasecmp(extension, (const char *)&D_00438D28) == 0;
    }
    owner->isPac = isPac;
    owner->remainingBytes = 0x7FFFFFFF;
    owner->drawX = borrowedDescriptor->drawX;
    owner->drawZ = borrowedDescriptor->drawZ;
    owner->drawY = borrowedDescriptor->drawY;
    owner->drawWidth = borrowedDescriptor->drawWidth;
    owner->drawHeight = borrowedDescriptor->drawHeight;
    soundFormat.hasAudio = borrowedDescriptor->hasAudio;
    soundFormat.stereo = borrowedDescriptor->stereo;
    soundFormat.loopMode = 0;
    soundFormat.playbackCadenceStep = borrowedDescriptor->playbackCadenceStep;

    if (!isPac) {
        stream = sdfAllocAndClearQuadwords(sizeof(*stream));
        owner->stream.linear = stream;
        allocation = sdfMemAllocDirect(0x20000);
        stream->allocation = allocation;
        {
            u8 *buffer = (u8 *)sdfMemLock(allocation);
            stream->bufferStart = buffer;
            stream->readCursor = buffer;
            stream->writeCursor = buffer;
        }
        owner->state = SDF_MOVIE_STATE_INITIAL;
        owner->deviceState = sdfFileCmdReadOpen(name,
                                                        (void *)movieGetStreamSize, owner);
        sdfSoundInitFormattedAndAppendNode(&owner->soundNode, &soundFormat,
                                            movieCopyChunk, owner, borrowedDescriptor->source);
        return;
    }

    if (movieStreamSema < 0) {
        movieStreamSema = sdfCreateSema(1, 1, 0);
    }
    movieSemaCount = -1;
    pacWork = sdfAllocAndClearQuadwords(sizeof(*pacWork));
    owner->stream.pac = pacWork;
    allocation = sdfMemAllocDirect(0x24000);
    pacWork->allocation = allocation;
    resource = (u8 *)sdfMemLock(allocation);
    pacWork->pacInputBuffer = resource;
    pacWork->pacBuffer = resource + 0x4000;
    pacWork->ringBuffer = resource + 0x14000;
    pacWork->scratchSize = 0x20;
    pacWork->scratch = pacWork->scratchBuffer;
    owner->state = SDF_MOVIE_STATE_INITIAL;
    owner->deviceState = sdfFileCmdReadOpen(name,
                                                    (void *)movieReadChunk, owner);
    owner->pacEnabled = 1;
    owner->packetLimit = 0x7F;
    sdfSoundInitFormattedAndAppendNode(&owner->soundNode, &soundFormat,
                                        movieParseHeader, owner, borrowedDescriptor->source);
}

/* This no-op also serves as a void(u32) completion callback and ignores its word. */
extern void sdfCreateSemaphoreFromOptions();

extern void sdfIpuFree(sdfIpuDecodeCb_t *);

extern void p2sdSyncMovie(void);

/* Wait for active work's release phase, free its owned buffers, then deactivate it. */
void movieCloseStream(movieStream_t *job) {
    if (job->active != 0) {
        job->stopRequested = 1;
        /* Queue phase 5 as phase 7; phase 6 is the release gate below. */
        if (job->state == SDF_MOVIE_STATE_WAITING_FOR_BUFFER_SPACE) {
            job->state = SDF_MOVIE_STATE_STOP_REQUESTED;
            sdfFileCmdClose(job->deviceState);
        }
        while (job->state != SDF_MOVIE_STATE_DEVICE_RELEASE_CALLBACK) {
            sdfCreateSemaphoreFromOptions();
        }
        if (job->isPac == 0) {
            MovLinearStream *stream = job->stream.linear;
            sdfMemFreeDirect(stream->allocation);
            sdfChipFreeDirect(stream);
        } else {
            MovPacStream *stream = job->stream.pac;
            sdfMemFreeDirect(stream->payloadAllocation);
            sdfMemFreeDirect(stream->allocation);
            sdfChipFreeDirect(stream);
        }
        sdfIpuFree(&job->soundNode);
        if (job->pacEnabled != 0) {
            p2sdSyncMovie();
        }
        job->active = 0;
    }
}

/* Decoder completion requires drained input and no completed output buffers pending. */
SdfMovieCompletionResult sdfMovieIsFinished(movieStream_t *job) {
    if (job->soundNode.drained == 0) {
        return SDF_MOVIE_COMPLETION_INCOMPLETE;
    }
    return job->soundNode.completedBufferCount == 0;
}
INCLUDE_SDATA(const s32, "sdf/sdfMovie", movieStreamSema);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", D_00438D0C);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", sdfMovieDrawLeft);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", sdfMovieDrawTop);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", sdfMovieDrawRight);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", sdfMovieDrawBottom);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", sdfMovieDrawColor);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", D_00438D24);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", D_00438D28);

