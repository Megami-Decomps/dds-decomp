#include "common.h"
#include "libsdf.h"
#include "ps2asm.h"

#include "sdf_movie_types.h"

void puts(const char *aStr);
s32 sdfFileCmdRead(void *aHandle, void *aBuffer, s32 aLength);
s32 sdfFileCmdClose(void *aHandle);
void sdfDummyFunc(void);
void sdfMemFreeDirect(sdfMemHandle_t *aHandle);
void sdfChipFreeDirect(void *aPtr);
void sdfIpuFree(sdfIpuDecodeCb_t *cb);
s32 p2sdSyncMovie(void);
void *memcpy(void *a, const void *b, size_t c);
s32 sdfFileCmdFileSize(void *aHandle);
s32 sdfFileCmdFree(void *aHandle);
s32 DIntr(void);
s32 EIntr(void);
void *memset(void *a, s32 b, size_t c);
char *strrchr(const char *aStr, s32 aChar);
s32 strcasecmp(const char *aLeft, const char *aRight);
void *sdfChipAllocZeroDirect(s32 aSize);
sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);
void *sdfMemLock(sdfMemHandle_t *aHandle);
void *sdfFileCmdReadOpen(const char *aFileName, SdfStreamRead aCallback, sdfMovieCb_t *aUserData);
void movieAttachDecodeTask(sdfIpuDecodeCb_t *aNode, sdfIpuInitParm_t *aFormat, SdfStreamRead aRead, sdfMovieCb_t *aContext, s32 aResourceWord);
s32 sdfCreateSema(s32 initCount, s32 maxCount, s32 option);
s32 WaitSema(s32 sema);
s32 SignalSema(s32 sema);
s32 movieReadChunk(void *aHandle, s32 aOp, void *aBuf, s32 aSize, sdfMovieCb_t *stream);
s32 movieCopyChunk(sdfIpuDecodeCb_t *aNode, sdfMovieCb_t *stream, s32 aOp, void *aBuf, s32 aSize);
s32 movieParseHeader(sdfIpuDecodeCb_t *aNode, sdfMovieCb_t *stream, s32 aOp, void *aBuf, s32 aSize);
extern s32 movieStreamSema;
extern u32 movieSemaCount;
extern sdfMovieCb_t *movieSemaId;
extern const char sdfMovieIpsExt[];


extern u32 movieDrawMode;
extern s32 sdfMovieDrawLeft;
extern s32 sdfMovieDrawTop;
extern s32 sdfMovieDrawRight;
extern s32 sdfMovieDrawBottom;
extern u32 sdfMovieDrawColor;

void sdfPrintTextLine(const char *aStr)
{
    puts(aStr);
}

u8 *movieWaitSema(s32 aSize)
{
    sdfMovieCb_t *mov = movieSemaId;
    sdfMovieStrmPac_t *obj = mov->readCb.pac;
    u8 *ptr;

    WaitSema(movieStreamSema);
    if (obj->scratchSize > 0) {
        ptr = obj->scratch;
        obj->scratchSize -= aSize;
        obj->scratch = ptr + aSize;
    } else {
        s32 offset = obj->ringOffset;
        ptr = obj->ringBuffer + offset;
        if (obj->ringLength < aSize) {
            /* Retail copies the old count before reusing its register for
             * the XOR predicate: countScratch has those two live phases.
             * Ordinary scalar reuse keeps the select; these local constraints
             * recover native allocation and XOR operand order. This is a
             * reconstruction aid, not original-source evidence. */
            FAKEMATCH_ASM_REG(s32, frame, "$2") = mov->decodeCb.frame;
            FAKEMATCH_ASM_REG(u32, countScratch, "$3") = movieSemaCount;
            u32 count;

            ptr = NULL;
            count = countScratch;
            countScratch = (u32)frame ^ countScratch;
            FAKEMATCH_ASM_LIVE(countScratch);
            if (countScratch != 0) {
                count = frame;
            }
            movieSemaCount = count;
        } else {
            obj->ringLength -= aSize;
            offset += aSize;
            if (offset > 0xFFFF) {
                offset -= 0x10000;
            }
            obj->ringOffset = offset;
        }
    }
    SignalSema(movieStreamSema);
    return ptr;
}

/* Store the stream-frame draw-path mode; movie viewport setup selects 1. */
void sdfSetStreamFrameDrawMode(u32 value) {
    movieDrawMode = value;
}

void sdfSetGridScaledDrawBounds(s32 column, s32 row, s32 width, s32 height, u32 mode) {
    sdfMovieDrawLeft = column * 0x10 + 0x7000;
    sdfMovieDrawTop = row * 8 + 0x7900;
    sdfMovieDrawRight = sdfMovieDrawLeft + width * 0x10;
    sdfMovieDrawBottom = sdfMovieDrawTop + height * 8;
    sdfMovieDrawColor = mode;
}

#ifdef NON_MATCHING

extern s32 sdfGrAllocPacketBuff(s32 size);
extern sdfDrawTag_t *sdfPrimAllocDrawTag(s32 (*allocator)(s32));
extern void sdfDrawTagLink(sdfDrawTag_t *tag, addr_t packet);
extern u32 p2sdSetPcmCallback(addr_t callback);
extern s32 p2sdPlayPcm(u8 channel);

extern s32 sdfMovieDrawLeft;
extern s32 sdfMovieDrawTop;
extern s32 sdfMovieDrawRight;
extern s32 sdfMovieDrawBottom;
extern u32 sdfMovieDrawColor;

extern sdfMovieCb_t *movieSemaId;


/* The bit length of x > 0: 31 - plzcw(x). For x = size - 1 it is the size's
 * exponent rounded up, as TEX0's TW and TH take it. */
static inline s32 bitLength(s32 x) {
    s32 leading;

    MMI_PLZCW(leading, x);
    return 31 - (leading & 0xFF);
}

static inline u64 movieVertexWord(s32 x, s32 y, u32 depth) {
    return ((u64)depth << 32) | ((u32)y << 16) | (u32)x;
}

/* Queue the decoded movie frame as a sprite over the viewport bounds and color
 * set by sdfSetGridScaledDrawBounds, once playback has begun; with the PAC
 * stream enabled, start its sound once the ring holds a page. release_movieobj2
 * calls it when the stream-frame draw mode is 1. Upstream: INCLUDE_ASM in
 * both games; written from the retail routine. It differs from retail
 * only in register allocation, scheduling, the way the color word is masked,
 * and the handwritten addi that loads 31 for the texture sizes. */
void sdfMovieQueueFrameSprite(sdfMovieCb_t *movie, sdfGrOt_t *pool) {
    u64 *packet;
    sdfDrawTag_t *packetList;
    u32 width;
    u32 height;
    s32 heightLog2;
    s32 widthLog2;
    u32 pageCount;
    u64 pageBase;
    u64 textureWord;

    if (movie->decodeCb.playbackPhase < 2) {
        return;
    }

    if (movie->sndActive != 0 && movie->sndPlaying == 0 &&
        movie->readCb.pac->ringLength >= 0x1000) {
        movie->sndPlaying = 1;
        movieSemaId = movie;
        p2sdSetPcmCallback(movieWaitSema);
        p2sdPlayPcm(movie->sndVolume);
    }

    packet = (u64 *)(u32)sdfGrAllocPacketBuff(0xC0);
    packet[0] = 0x0100000BULL;
    packet[1] = 0x5000000B00000000ULL;
    packet[2] = 0x10AB40000000800AULL;
    packet[3] = 0xE;
    packet[4] = 0x21;
    packet[5] = 0x14;

    height = movie->decodeCb.height;
    width = movie->decodeCb.width;
    heightLog2 = bitLength(height - 1);
    widthLog2 = bitLength(width - 1);
    pageCount = width >> 6;
    if (pageCount == 0) {
        pageCount = 1;
    }
    pageBase = (u32)movie->decodeCb.resourceWord >> 6;
    textureWord = ((u64)pageCount << 14) | ((u64)heightLog2 << 30) |
                  (u64)(widthLog2 << 26) | (1ULL << 34);
    if (movie->decodeCb.rgbMode != 0) {
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
    packet[18] = movieVertexWord(sdfMovieDrawLeft, sdfMovieDrawTop, movie->z);
    packet[19] = 5;
    packet[20] = (u64)(s64)(s32)((height << 20) | (width << 4));
    packet[21] = 3;
    packet[22] = movieVertexWord(sdfMovieDrawRight, sdfMovieDrawBottom, movie->z);
    packet[23] = 5;

    packetList = sdfPrimAllocDrawTag(NULL);
    sdfDrawTagLink(packetList, (u32)packet);
    pool->regFunc(pool, packetList);
}
#else
INCLUDE_ASM(const s32, "sdf/sdfMovie", sdfMovieQueueFrameSprite);
#endif

extern void sdfMovieQueueFrameSprite(sdfMovieCb_t *movie, sdfGrOt_t *ot);

/* DMA/VIF/GIF header followed by nine real A+D writes for one movie sprite. */
typedef struct sdfMovieDrawPacket_t {
    u64 dmaTag;
    u64 vifCommands;
    sdfGifTag_t gifTag;
    sdfGrTagStdReg_t sampling;
    sdfGrTagStdReg_t texture;
    sdfGrTagStdReg_t clamp;
    sdfGrTagStdReg_t textureAlpha;
    sdfGrTagStdReg_t color;
    sdfGrTagStdReg_t sourceOrigin;
    sdfGrTagStdReg_t vertexOrigin;
    sdfGrTagStdReg_t sourceExtent;
    sdfGrTagStdReg_t vertexExtent;
} sdfMovieDrawPacket_t;

extern u8 *movieWaitSema(s32 size);
extern s32 sdfGrAllocPacketBuff(s32 size);
extern sdfDrawTag_t *sdfPrimAllocDrawTag(s32 (*allocator)(s32));
extern void sdfDrawTagLink(sdfDrawTag_t *tag, addr_t packet);
extern u32 p2sdSetPcmCallback(addr_t callback);
extern s32 p2sdPlayPcm(u8 channel);

void release_movieobj2(sdfMovieCb_t *obj, sdfGrOt_t *ot)
{
    sdfMovieDrawPacket_t *packet;
    sdfDrawTag_t *tag;
    u16 width;
    u16 height;
    s32 widthLog2;
    s32 heightLog2;
    u32 bufferWidth;
    u32 basePointer;

    if (obj->decodeCb.playbackPhase < 2) {
        return;
    }
    if (movieDrawMode == 1) {
        sdfMovieQueueFrameSprite(obj, ot);
        return;
    }
    if (obj->sndActive != 0 && obj->sndPlaying == 0 && obj->readCb.pac->ringLength >= 0x1000) {
        obj->sndPlaying = 1;
        movieSemaId = obj;
        p2sdSetPcmCallback(EE_ADDR(movieWaitSema));
        p2sdPlayPcm(obj->sndVolume);
    }

    packet = EE_ADDR_AS(sdfMovieDrawPacket_t *, sdfGrAllocPacketBuff(sizeof(sdfMovieDrawPacket_t)));
    packet->dmaTag = 0x100000A;
    height = obj->decodeCb.height;
    width = obj->decodeCb.width;
    packet->vifCommands = 0x5000000A00000000ULL;
    packet->gifTag.parm = 0x108B400000008009ULL;
    packet->gifTag.regs = 0x0E;
    packet->sampling.data = 0x21;
    packet->sampling.reg = 0x14;
    heightLog2 = height - 1;
    widthLog2 = width - 1;
    bufferWidth = width >> 6;
    heightLog2 = PS2_WORD_BIT_LENGTH(heightLog2);
    widthLog2 = PS2_WORD_BIT_LENGTH(widthLog2);
    basePointer = (u32)obj->decodeCb.resourceWord >> 6;
    if (bufferWidth == 0) {
        bufferWidth = 1;
    }
    packet->texture.data = ((u64)1 << 34) | ((u64)heightLog2 << 30) |
        (widthLog2 << 26) | (obj->decodeCb.rgbMode != 0 ? 0x200000 : 0) |
        ((u64)bufferWidth << 14) | (u64)basePointer;
    packet->texture.reg = 0x06;
    packet->clamp.data = 5;
    packet->clamp.reg = 0x08;
    packet->textureAlpha.data = 0x4000000080ULL;
    packet->textureAlpha.reg = 0x3B;
    packet->color.data = (s32)0x80808080;
    packet->color.reg = 0x01;
    packet->sourceOrigin.data = 0;
    packet->sourceOrigin.reg = 0x03;
    packet->vertexOrigin.data = ((u64)obj->z << 32) |
        ((u32)(s16)obj->y << 16) | (u32)(s16)obj->x;
    packet->vertexOrigin.reg = 0x05;
    packet->sourceExtent.data = (s32)(((u32)height << 20) | ((u32)width << 4));
    packet->sourceExtent.reg = 0x03;
    packet->vertexExtent.data = ((u64)obj->z << 32) |
        ((u32)((s16)obj->y + (s16)obj->height) << 16) |
        (u32)((s16)obj->x + (s16)obj->width);
    packet->vertexExtent.reg = 0x05;
    tag = sdfPrimAllocDrawTag(NULL);
    sdfDrawTagLink(tag, EE_ADDR(packet));
    ot->regFunc(ot, tag);
}

void movieReadNextChunk(sdfMovieCb_t *aMov)
{
    s32 remaining = aMov->readRemain;
    sdfMovieStrmLinear_t *stream = aMov->readCb.linear;
    s32 size;
    u8 *dst;
    u8 *next;

    if (remaining != 0) {
        if (0x20000 - stream->bufferedBytes < 0x4000) {
            aMov->readStat = 5;
        } else {
            aMov->readStat = 4;
            size = remaining < 0x4001 ? remaining : 0x4000;
            dst = stream->writeCursor;
            next = dst + size;
            if (next >= stream->bufferStart + 0x20000) {
                next = stream->bufferStart;
            }
            stream->writeCursor = next;
            sdfFileCmdRead(aMov->fileHandle, dst, size);
        }
    }
}

s32 movieGetStreamSize(void *aHandle, s32 aOp, void *aBuf, s32 aSize, sdfMovieCb_t *aMov)
{
    sdfMovieStrmLinear_t *stream = aMov->readCb.linear;
    s32 enabled;

    aMov->fileHandle = aHandle;
    if (aMov->cancel != 0) {
        if (aMov->readStat != 6 && aMov->readStat != 7) {
            aMov->readStat = 7;
            sdfFileCmdClose(aHandle);
            return 0;
        }
    }
    switch (aMov->readStat) {
    case 0:
        if (aOp == 2) {
            aMov->readStat = 1;
            sdfFileCmdFileSize(aHandle);
        }
        break;
    case 1:
        if (aOp == 4) {
            aMov->fileSize = aSize;
            aMov->readRemain = aSize;
            movieReadNextChunk(aMov);
        }
        break;
    case 4:
        if (aOp == 5) {
            enabled = DIntr();
            stream->bufferedBytes += aSize;
            aMov->readRemain -= aSize;
            if (enabled != 0) {
                EIntr();
            }
            if (aMov->readRemain == 0) {
                aMov->readStat = 7;
                sdfFileCmdClose(aHandle);
            } else {
                movieReadNextChunk(aMov);
            }
        }
        break;
    case 7:
        if (aOp == 7) {
            aMov->fileHandle = NULL;
            aMov->readStat = 6;
            sdfFileCmdFree(aHandle);
        }
        break;
    }
    return 0;
}

void movieReadHeaderChunk(sdfMovieCb_t *aMov)
{
    s32 remaining = aMov->readRemain;
    sdfMovieStrmPac_t *stream = aMov->readCb.pac;
    s32 size;

    if (remaining != 0) {
        if (0x10000 - stream->pacBufferedBytes < 0x4000) {
            aMov->readStat = 5;
        } else if (0x10000 + (stream->unk6C - stream->ringLength) < 0x4000) {
            aMov->readStat = 5;
        } else {
            aMov->readStat = 4;
            size = remaining < 0x4001 ? remaining : 0x4000;
            sdfFileCmdRead(aMov->fileHandle, stream->pacInputBuffer, size);
        }
    }
}

s32 movieReadChunk(void *file, s32 operation, void *readBuffer, s32 bytesRead, sdfMovieCb_t *movie) {
    sdfMovieStrmPac_t *stream;

    movie->fileHandle = file;
    stream = movie->readCb.pac;

    if (movie->cancel != 0 &&
        (movie->readStat < 6 ||
         movie->readStat > 7)) {
        movie->readStat = 7;
        sdfFileCmdClose(file);
        return 0;
    }

    switch (movie->readStat) {
    case 0:
        if (operation == 2) {
            movie->readStat = 1;
            sdfFileCmdFileSize(file);
        }
        break;
    case 1:
        if (operation == 4) {
            movie->fileSize = bytesRead;
            movie->readRemain = bytesRead;
            movie->readStat = 2;
            sdfFileCmdRead(file, stream, 0x40);
        }
        break;
    case 2:
        if (operation == 5) {
            s32 payloadBytes = stream->packetBytes - 0x40;
            stream->payloadAllocation = sdfMemAllocDirect(payloadBytes);
            stream->blockMask = (u8 *)sdfMemLock(stream->payloadAllocation);
            movie->readStat = 3;
            sdfFileCmdRead(file, stream->blockMask, payloadBytes);
            movie->readRemain -= stream->packetBytes;
        }
        break;
    case 3:
        if (operation == 5) {
            movieReadHeaderChunk(movie);
        }
        break;
    case 4: {
        if (operation == 5) {
            u8 *source = readBuffer;
            s32 unitSize = stream->blockBytes;
            s32 blocksRemaining = bytesRead / unitSize;
            do {
                s32 blockIndex = stream->blockIndex;
                s32 byteIndex = blockIndex / 8;
                s32 bitIndex = blockIndex - byteIndex * 8;
                s32 blockFlag = 1 << bitIndex;
                s32 writeOffset;
                if ((stream->blockMask[byteIndex] & blockFlag) != 0) {
                    s32 ringFill;
                    s32 wrappedBytes;

                    WaitSema(movieStreamSema);
                    ringFill = stream->ringLength;
                    writeOffset = stream->ringOffset + ringFill;
                    writeOffset += (writeOffset > 0xFFFF) * -0x10000;
                    wrappedBytes = writeOffset + unitSize - 0x10000;
                    if (wrappedBytes > 0) {
                        memcpy(stream->ringBuffer + writeOffset, source, unitSize - wrappedBytes);
                        memcpy(stream->ringBuffer, source + unitSize - wrappedBytes, wrappedBytes);
                    } else {
                        memcpy(stream->ringBuffer + writeOffset, source, unitSize);
                    }
                    wrappedBytes = ringFill + unitSize;
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
                    memcpy(stream->pacBuffer + writeOffset, source, unitSize);
                    stream->pacBufferedBytes += unitSize;
                    if (restoreInterrupts != 0) {
                        EIntr();
                    }
                }
                source += unitSize;
                stream->blockIndex++;
            } while (--blocksRemaining != 0);

            movie->readRemain -= bytesRead;
            if (movie->readRemain == 0) {
                movie->readStat = 7;
                sdfFileCmdClose(file);
            } else {
                movieReadHeaderChunk(movie);
            }
        }
        break;
    }
    case 7:
        if (operation == 7) {
            movie->fileHandle = NULL;
            movie->readStat = 6;
            sdfFileCmdFree(file);
        }
        break;
    }

    return 0;
}

s32 movieCopyChunk(sdfIpuDecodeCb_t *aNode, sdfMovieCb_t *aMov, s32 aOp, void *aBuf, s32 aSize)
{
    sdfMovieStrmLinear_t *stream = aMov->readCb.linear;
    u8 *src;
    u8 *base;
    s32 over;
    s32 first;

    switch (aOp) {
    case 0:
        if (aMov->cancel != 0) {
            *(u8 *)aBuf = 1;
            return 0;
        }
        if (aMov->readRemain == 0) {
            *(u8 *)aBuf = 1;
        }
        return stream->bufferedBytes;
    case 1:
        if (aMov->cancel != 0) {
            return 0;
        }
        src = stream->readCursor;
        base = stream->bufferStart;
        over = (src - base) + aSize - 0x20000;
        if (over >= 0) {
            first = aSize - over;
            memcpy(aBuf, src, first);
            if (over > 0) {
                memcpy((u8 *)aBuf + first, base, over);
            }
            stream->readCursor = base + over;
        } else {
            memcpy(aBuf, src, aSize);
            stream->readCursor = src + aSize;
        }
        stream->bufferedBytes -= aSize;
        return aMov->readRemain;
    case 2:
        if (aMov->cancel != 0) {
            return 0;
        }
        if (aMov->readStat == 5) {
            movieReadNextChunk(aMov);
        }
        return 0;
    }
    return 0;
}

#ifdef NON_MATCHING
/* clang has no GNU nested functions. The matching build below keeps the
 * nested form: ee-gcc loads the (unused) static chain pointer before each
 * call, and lifting the function to file scope drops that load, moving the
 * retail image. */
static void movieMemcpy(void *aDst, const void *aSrc, s32 aCount)
{
    memcpy(aDst, aSrc, aCount);
}
#endif

s32 movieParseHeader(sdfIpuDecodeCb_t *aNode, sdfMovieCb_t *aMov, s32 aOp, void *aBuf, s32 aSize)
{
#ifndef NON_MATCHING
    /* Keep the retail helper symbol while retaining GCC's nested static chain. */
    auto void movieMemcpy(void *, const void *, s32) FAKEMATCH_ASM_NAME("movieMemcpy");
    void movieMemcpy(void *aDst, const void *aSrc, s32 aCount)
    {
        memcpy(aDst, aSrc, aCount);
    }
#endif
    sdfMovieStrmPac_t *stream = aMov->readCb.pac;
    u8 *body;
    s32 off;
    s32 over;
    s32 first;

    switch (aOp) {
    case 0:
        if (aMov->cancel != 0) {
            *(u8 *)aBuf = 1;
            return 0;
        }
        if (aMov->readRemain == 0) {
            *(u8 *)aBuf = 1;
        }
        return stream->pacBufferedBytes;
    case 1:
        if (aMov->cancel != 0) {
            return 0;
        }
        off = stream->pacReadOffset;
        body = stream->pacBuffer;
        over = off + aSize - 0x10000;
        if (over >= 0) {
            first = aSize - over;
            movieMemcpy(aBuf, body + off, first);
            if (over > 0) {
                movieMemcpy((u8 *)aBuf + first, body, over);
            }
            stream->pacReadOffset = over;
        } else {
            movieMemcpy(aBuf, body + off, aSize);
            stream->pacReadOffset = off + aSize;
        }
        stream->pacBufferedBytes -= aSize;
        return aMov->readRemain;
    case 2:
        if (aMov->cancel != 0) {
            return 0;
        }
        if (aMov->readStat == 5) {
            movieReadHeaderChunk(aMov);
        }
        return 0;
    }
    return 0;
}

void movieOpenStream(sdfMovieCb_t *stream, sdfMovieInitParm_t *desc, const char *path)
{
    sdfIpuInitParm_t fmt;
    sdfMovieStrmLinear_t *linear;
    sdfMovieStrmPac_t *pac;
    char *ext;
    u8 isPac;

    memset(stream, 0, sizeof(sdfMovieCb_t));
    stream->stat = 1;
    ext = strrchr(path, '.');
    isPac = 0;
    if (ext != NULL) {
        isPac = strcasecmp(ext, sdfMovieIpsExt) == 0;
    }
    stream->mode = isPac;
    fmt.loopMode = 0;
    stream->readRemain = 0x7FFFFFFF;
    stream->x = desc->x;
    stream->z = desc->z;
    stream->y = desc->y;
    stream->width = desc->width;
    stream->height = desc->height;
    fmt.rgbMode = desc->rgbMode;
    fmt.ditherMode = desc->ditherMode;
    fmt.fps = desc->fps;
    if (isPac == 0) {
        sdfMemHandle_t *handle;
        u8 *base;

        linear = sdfChipAllocZeroDirect(sizeof(sdfMovieStrmLinear_t));
        stream->readCb.linear = linear;
        handle = sdfMemAllocDirect(0x20000);
        linear->allocation = handle;
        base = sdfMemLock(handle);
        linear->bufferStart = base;
        linear->readCursor = base;
        linear->writeCursor = base;
        stream->readStat = 0;
        stream->fileHandle = sdfFileCmdReadOpen(path, (SdfStreamRead)movieGetStreamSize, stream);
        movieAttachDecodeTask(&stream->decodeCb, &fmt, movieCopyChunk, stream, desc->resourceWord);
    } else {
        sdfMemHandle_t *handle;
        u8 *base;
        u8 *ring;
        u8 *body;
        u8 *scr;
        s32 n;

        if (movieStreamSema < 0) {
            movieStreamSema = sdfCreateSema(1, 1, 0);
        }
        movieSemaCount = -1;
        pac = sdfChipAllocZeroDirect(sizeof(sdfMovieStrmPac_t));
        stream->readCb.pac = pac;
        handle = sdfMemAllocDirect(0x24000);
        pac->allocation = handle;
        base = sdfMemLock(handle);
        n = 0x20;
        ring = base + 0x14000;
        body = base + 0x4000;
        scr = pac->scratchBuffer;
        pac->pacBuffer = body;
        pac->scratch = scr;
        pac->ringBuffer = ring;
        pac->scratchSize = n;
        pac->pacInputBuffer = base;
        stream->readStat = 0;
        stream->fileHandle = sdfFileCmdReadOpen(path, (SdfStreamRead)movieReadChunk, stream);
        stream->sndActive = 1;
        stream->sndVolume = 0x7F;
        movieAttachDecodeTask(&stream->decodeCb, &fmt, movieParseHeader, stream, desc->resourceWord);
    }
}

void movieCloseStream(sdfMovieCb_t *aMov)
{
    u8 state;
    sdfMovieStrmLinear_t *stream;
    sdfMovieStrmPac_t *pac;

    if (aMov->stat == 0) {
        return;
    }
    state = aMov->readStat;
    aMov->cancel = 1;
    if (state == 5) {
        aMov->readStat = 7;
        sdfFileCmdClose(aMov->fileHandle);
        state = aMov->readStat;
    }
    if (state != 6) {
        do {
            sdfDummyFunc();
        } while (aMov->readStat != 6);
    }
    if (aMov->mode == 0) {
        stream = aMov->readCb.linear;
        sdfMemFreeDirect(stream->allocation);
        sdfChipFreeDirect(stream);
    } else {
        pac = aMov->readCb.pac;
        sdfMemFreeDirect(pac->payloadAllocation);
        sdfMemFreeDirect(pac->allocation);
        sdfChipFreeDirect(pac);
    }
    sdfIpuFree(&aMov->decodeCb);
    if (aMov->sndActive != 0) {
        p2sdSyncMovie();
    }
    aMov->stat = 0;
}

s32 sdfMovieIsFinished(sdfMovieCb_t *aMov)
{
    if (aMov->decodeCb.drained == 0) {
        return 0;
    }
    return aMov->decodeCb.activeBuffCount == 0;
}
INCLUDE_SDATA(const s32, "sdf/sdfMovie", movieStreamSema);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", movieDrawMode);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", sdfMovieDrawLeft);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", sdfMovieDrawTop);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", sdfMovieDrawRight);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", sdfMovieDrawBottom);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", sdfMovieDrawColor);

SDATA_PAD(4);

INCLUDE_SDATA(const s32, "sdf/sdfMovie", sdfMovieIpsExt);

