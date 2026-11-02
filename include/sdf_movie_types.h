#ifndef SDF_MOVIE_TYPES_H
#define SDF_MOVIE_TYPES_H

#include "libsdf.h"

struct sdfMovieCb_t;
struct sdfIpuInitParm_t;
struct sdfGsMemSlice_t;
struct sdfIpuDmaTag_t;

/* The to-IPU and from-IPU DMA channel words the decoder keeps, plus the bit
 * position and IPU_CTRL it restores. The remaster's control block stops before
 * these. */
typedef struct sdfIpuDmaRegs_t
{
    u32 inputAddress;
    u32 inputTagAddress;
    u32 inputQwords;
    u32 inputControl;
    u32 outputAddress;
    u32 outputQwords;
    u32 outputControl;
    u32 bitPointer;
    u32 ipuControl;
} sdfIpuDmaRegs_t;

typedef struct sdfIpuDecodeCb_t
{
    struct sdfIpuDecodeCb_t *prev;
    struct sdfIpuDecodeCb_t *streamNext;
    struct sdfIpuDecodeCb_t *next;
    u8 active;
    u8 queueLinked;
    u8 ipuCycleInitialized; /* First-use IPU setup has run for this playback cycle. */
    u8 drained;
    u8 playbackPhase;
    u8 inputFeedEnabled; /* Restart input feeding after DMA completion while input remains. */
    u8 inputDmaStartPending;
    u8 imageTrans; /* Queued output submissions not yet retired by a playback update. */
    u8 rgbMode;
    u8 loopMode;
    u8 fps;
    u8 playbackCadenceRemainder;
    u8 bufferIndex;
    u8 getBuffId;
    u8 activeBuffCount; /* Completed output buffers waiting for playback submission. */
    u8 pad1B;
    s32 buffSize;
    void *frameBuffers[2];
    void *transferPacketBuffers[2];
    u32 transferPacketBytes;
    s32 resourceWord;
    sdfGsMemSlice_t *textureHead;
    u16 width;
    u16 height;
    u32 frameCount;
    u32 tickCount;
    s32 frame;
    struct sdfIpuDmaTag_t *inputDmaChain;
    u8 headerReady;
    u8 done;
    u8 filledSlots;
    u8 firstSlot;
    u8 *scratchBuffer;
    u8 pad58[4];
    s32 (*read)(struct sdfIpuDecodeCb_t *, struct sdfMovieCb_t *, s32, void *, s32);
    struct sdfMovieCb_t *strmCallbackParm; /* Owner context passed to the read callback. */
    u8 inputFeedDmaInFlight;
    u8 strmToIpuDmaStep;
    u8 pad66[2];
    sdfIpuDmaRegs_t dma;
} sdfIpuDecodeCb_t;

typedef struct sdfIpuInitParm_t
{
    u8 rgbMode;
    u8 ditherMode;
    u8 loopMode;
    u8 fps;
} sdfIpuInitParm_t;

typedef struct sdfMovieInitParm_t
{
    u16 x;
    u16 y;
    u32 z;
    u16 width;
    u16 height;
    s32 resourceWord;
    u8 rgbMode;
    u8 ditherMode;
    u8 fps;
} sdfMovieInitParm_t;

/* Decode-node callback only; file-command callbacks have a different ABI. */
typedef s32 (*SdfStreamRead)(sdfIpuDecodeCb_t *, struct sdfMovieCb_t *, s32, void *, s32);

/* Ring buffer of raw file data for a plain (non PAC) movie stream. */
typedef struct sdfMovieStrmLinear_t
{
    sdfMemHandle_t *allocation;
    u8 *bufferStart;
    u8 *readCursor;
    u8 *writeCursor;
    s32 bufferedBytes;
} sdfMovieStrmLinear_t;

typedef struct sdfMovieStrmPac_t
{
    u8 pad00[0x18];
    s32 packetBytes;
    s32 blockBytes;
    u8 scratchBuffer[0x20];
    sdfMemHandle_t *payloadAllocation;
    u8 *blockMask;
    s32 blockIndex;
    sdfMemHandle_t *allocation;
    u8 *pacInputBuffer;
    u8 *pacBuffer;
    s32 pacReadOffset;
    s32 pacBufferedBytes;
    u8 *ringBuffer;
    s32 ringOffset;
    s32 ringLength;
    s32 unk6C;
    s32 scratchSize;
    u8 *scratch;
} sdfMovieStrmPac_t;

typedef union movieDecodeCtx_t
{
    sdfMovieStrmLinear_t *linear;
    sdfMovieStrmPac_t *pac;
} movieDecodeCtx_t;

typedef struct sdfMovieCb_t
{
    u8 stat;
    u8 readStat;
    u8 cancel;
    u8 mode;
    u16 x;
    u16 y;
    u32 z;
    u16 width;
    u16 height;
    void *fileHandle;
    s32 fileSize;
    s32 readRemain;
    movieDecodeCtx_t readCb;
    u8 sndActive;
    u8 sndPlaying;
    u8 sndVolume;
    u8 reserve;
    sdfIpuDecodeCb_t decodeCb;
} sdfMovieCb_t;

#endif
