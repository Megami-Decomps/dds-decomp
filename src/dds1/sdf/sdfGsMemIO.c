#include "common.h"
#include "sdf_types.h"
#include "sdf_resource.h"
#include "eeregs.h"

struct sdfPreDrawHook_t;

struct sdfPrimBitBltElem_t;

struct sdfLoadImagePacket_t;

struct sdfGsMemSlice_t;

struct sdfGrFrameSet_t;

struct sdfDrawTag_t;

struct sdfStoreImagePacket_t;

struct sdfStoreFrameImagePacket_t;

struct sdfPreDrawTag_t;

/* The frame worker passes the node and queued buffer index. The registered
 * handlers project the node into their distinct complete packet owners. */
typedef void (*SdfPacketPatchCallback)(void *, s32);

#define SDF_DMA_QWC_MASK 0xFFFF

#define SDF_PATCHABLE_PACKET_BYTES 0x100

#define SDF_PATCHABLE_PACKET_TAIL_OFFSET 0xD0

#define SDF_DESCRIPTOR_PACKET_BYTES 0xB0

#define SDF_DESCRIPTOR_PACKET_TAIL_OFFSET 0x80

/* Pre-draw hook: the engine's record continues with two option words, which
 * each owner reads under its own name. */
typedef struct sdfPreDrawHook_t {
    struct sdfPreDrawHook_t *next;
    SdfPacketPatchCallback predrawFunc;
    u32 option0;
    u32 option1;
} sdfPreDrawHook_t;

/* The four A+D writes controlling a GS image transfer. */
typedef struct sdfPrimBitBltElem_t {
    sdfGrTagStdReg_t bitbltbuf;
    sdfGrTagStdReg_t trxpos;
    sdfGrTagStdReg_t trxreg;
    sdfGrTagStdReg_t trxdir;
} sdfPrimBitBltElem_t;

/* Host-to-local image transfer packet followed by its flush terminal tag (0xB0). */
typedef struct sdfLoadImagePacket_t {
    u64 header[4];
    sdfPrimBitBltElem_t bitblt;
    u64 imageReferenceTag;
    u64 imageReferencePad;
    u64 imageGifTag0;
    u64 imageGifTag1;
    u64 flushTag0;
    u64 flushTag1;
    u64 flushGifTag;
    u64 flushRegister;
    u64 zero;
    u64 finishRegister;
} sdfLoadImagePacket_t;


/* Graph target: two color buffers followed by the auxiliary/depth buffer (0x14). */
typedef struct sdfGrFrameSet_t {
    s16 width;
    s16 h;
    s16 height;
    u8 bufferFormat;
    u8 auxiliaryFormat;
    sdfGsMemSlice_t *buffers[3];
} sdfGrFrameSet_t;


void sdfDrawTagLink2(struct sdfDrawTag_t *aTag, addr_t aHeadPacket,
                          addr_t aTailPacket);

/* Resource upload payload and its terminal primitive-reset tag (0xF0). */
typedef struct sdfStoreImagePacket_t {
    u64 header[14];
    sdfPrimBitBltElem_t bitblt;
    s32 leng;
    u32 addr;
    u32 callback;
    u32 callbackParam;
    u64 tail[6];
} sdfStoreImagePacket_t;

/* Metadata node followed by the resource-transfer payload (0x100). */
typedef struct sdfStoreFrameImagePacket_t {
    sdfPreDrawHook_t worker;
    sdfStoreImagePacket_t packet;
} sdfStoreFrameImagePacket_t;

/* Complete linked-packet list, distinct from the 0x20-byte DMA list head. */
typedef struct sdfPreDrawTag_t {
    u32 next;
    sdfPreDrawHook_t *head;
    sdfPreDrawHook_t *tail;
    u32 pad;
} sdfPreDrawTag_t;

extern sdfGrFrameSet_t sdfGrFrameSet;

s32 sdfGetTexelBitCount(u32 aFormat);

typedef struct effDebugPolyParams_t {
    s16 parameterCount;
    s16 vertexCount;
    u16 primitive;
    u16 mask;
    u32 color;
    const u32 *parameters;
    u128 *positions;
    u128 *normals;
    f32 *texcoords;
    u32 *extraTexcoords;
    u32 *colors;
    void *(*allocate)(s32);
    f32 depth;
} effDebugPolyParams_t;

void sdfSetStoreImage(sdfStoreImagePacket_t *aPacket, sdfGsMemSlice_t *aGsHandle,
    s32 aSrcX, s32 aSrcY, s32 aWidth, s32 aHeight, u32 aBuffer, u32 aCallback, u32 aCallbackParam);

void sdfLinkStoreImage(sdfDrawTag_t *aDrawTag, sdfGsMemSlice_t *aGsHandle, s32 aSrcX,
                   s32 aSrcY, s32 aWidth, s32 aHeight, saddr_t aBuffer, s32 aCallback,
                   s32 aCallbackParam, s32 (*aAllocator)(s32));

void sdfLinkStoreFrameImagePredraw(sdfStoreFrameImagePacket_t *aPre, s32 aEvenOdd);

void sdfLinkStoreFrameImage(sdfDrawTag_t *aDrawTag, sdfPreDrawTag_t *aPreDrawTag, s32 aSrcX, s32 aSrcY,
                   s32 aWidth, s32 aHeight, s32 aBuffer, s32 aCallback, s32 aCallbackParam,
                   s32 (*aAllocator)(s32));

void sdfSetLoadImage(sdfLoadImagePacket_t *aPacket, sdfGsMemSlice_t *aGsHandle,
                   s64 aDstX, s64 aDstY, s32 aWidth,
                   s32 aHeight, u32 aBuffer);

void sdfLinkLoadImage(sdfDrawTag_t *aDrawTag, sdfGsMemSlice_t *aGsHandle,
                   s32 aDstX, s32 aDstY, s32 aWidth, s32 aHeight,
                   saddr_t aBuffer, s32 (*aAllocator)(s32));

s32 sdfGrAllocPacketBuff(s32 aSize);

void sdfPreDrawTagLink(sdfPreDrawTag_t *aTag, sdfPreDrawHook_t *aHook);

void sdfPrimSetBitBltElem(sdfPrimBitBltElem_t *aElem, u32 aDstGsAddr, s32 aDstBuffWidth,
                  s64 aDstPixelFormat, s64 aDstX, s64 aDstY,
                  u32 aSrcGsAddr, s32 aSrcBuffWidth, s32 aSrcPixelFormat,
                  s32 aSrcX, s32 aSrcY, s32 aWidth, s32 aHeight, s32 aDirMode);

void sdfSetStoreImage(sdfStoreImagePacket_t *aPacket, sdfGsMemSlice_t *aGsHandle,
    s32 aSrcX, s32 aSrcY, s32 aWidth, s32 aHeight, u32 aBuffer, u32 aCallback, u32 aCallbackParam) {
    s32 quadwordCount;

    aPacket->header[0] = 0x90000001ULL;
    aPacket->header[1] = 0x10000000ULL;
    aPacket->header[2] = 0;
    aPacket->header[3] = 0;
    aPacket->header[4] = 0x90000002ULL;
    aPacket->header[5] = 0x5000000206008000ULL;
    aPacket->header[6] = 0x1000000000000001ULL;
    aPacket->header[7] = 0xE;
    aPacket->header[8] = 0;
    aPacket->header[9] = 0x61;
    aPacket->header[10] = SDF_DMA_TAG(EE_ADDR(aPacket->tail), 0xA0000005ULL);
    aPacket->header[11] = 0x5000000500000000ULL;
    aPacket->header[12] = 0x1000000000000004ULL;
    aPacket->header[13] = 0xE;
    sdfPrimSetBitBltElem(&aPacket->bitblt, 0, 0, 0, 0, 0,
        aGsHandle->addr, aGsHandle->w, aGsHandle->pixelFormat,
        aSrcX, aSrcY, aWidth, aHeight, 1);
    quadwordCount = (sdfGetTexelBitCount(aGsHandle->pixelFormat) * aWidth * aHeight) >> 7;
    aPacket->leng = quadwordCount;
    aPacket->addr = aBuffer;
    aPacket->callback = aCallback;
    aPacket->callbackParam = aCallbackParam;
    aPacket->tail[0] = 0x20000002ULL;
    aPacket->tail[1] = 0x5000000206000000ULL;
    aPacket->tail[2] = 0x1000000000008001ULL;
    aPacket->tail[3] = 0xE;
    aPacket->tail[4] = 0x46;
    aPacket->tail[5] = 0;
}

void sdfLinkStoreImage(sdfDrawTag_t *aDrawTag, sdfGsMemSlice_t *aGsHandle, s32 aSrcX,
                   s32 aSrcY, s32 aWidth, s32 aHeight, saddr_t aBuffer, s32 aCallback,
                   s32 aCallbackParam, s32 (*aAllocator)(s32)) {
    s32 packetAddress;

    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packetAddress = aAllocator(0xf0);
    sdfSetStoreImage(EE_ADDR_AS(sdfStoreImagePacket_t *, packetAddress), aGsHandle,
        aSrcX, aSrcY, aWidth, aHeight, aBuffer, aCallback, aCallbackParam);
    sdfDrawTagLink2(aDrawTag, packetAddress, packetAddress + 0xc0);
}

void sdfLinkStoreFrameImagePredraw(sdfStoreFrameImagePacket_t *aPre, s32 aEvenOdd) {
    aPre->packet.bitblt.bitbltbuf.data =
        (aPre->packet.bitblt.bitbltbuf.data & ~0x3FFF) |
        (u64)(u32)(sdfGrFrameSet.buffers[aEvenOdd]->addr >> 6);
}

void sdfLinkStoreFrameImage(sdfDrawTag_t *aDrawTag, sdfPreDrawTag_t *aPreDrawTag, s32 aSrcX, s32 aSrcY,
                   s32 aWidth, s32 aHeight, s32 aBuffer, s32 aCallback, s32 aCallbackParam,
                   s32 (*aAllocator)(s32)) {
    sdfStoreFrameImagePacket_t *packet;

    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packet = EE_ADDR_AS(sdfStoreFrameImagePacket_t *, aAllocator(SDF_PATCHABLE_PACKET_BYTES));
    packet->worker.predrawFunc = sdfLinkStoreFrameImagePredraw;
    sdfSetStoreImage(&packet->packet,
        sdfGrFrameSet.buffers[0], aSrcX, aSrcY, aWidth, aHeight,
        aBuffer, aCallback, aCallbackParam);
    sdfPreDrawTagLink(aPreDrawTag, &packet->worker);
    sdfDrawTagLink2(aDrawTag, EE_ADDR(&packet->packet),
        EE_ADDR(packet) + SDF_PATCHABLE_PACKET_TAIL_OFFSET);
}

void sdfSetLoadImage(sdfLoadImagePacket_t *aPacket, sdfGsMemSlice_t *aGsHandle,
                   s64 aDstX, s64 aDstY, s32 aWidth,
                   s32 aHeight, u32 aBuffer) {
    s32 qwc;

    aPacket->header[0] = 0x10000006;
    aPacket->header[1] = 0x5000000611000000;
    aPacket->header[2] = 0x1000000000000004;
    aPacket->header[3] = 0xE;
    sdfPrimSetBitBltElem(&aPacket->bitblt, aGsHandle->addr, aGsHandle->w,
                  aGsHandle->pixelFormat, aDstX, aDstY, 0, 0, 0, 0, 0,
                  aWidth, aHeight, 0);

    qwc = (sdfGetTexelBitCount(aGsHandle->pixelFormat) * aWidth * aHeight) >> 7;
    aPacket->imageReferenceTag = 0x0800000000000000 | qwc;
    aPacket->imageReferencePad = 0;
    aPacket->imageGifTag0 = SDF_DMA_TAG_WORD_FIRST(
        (u32)((qwc & SDF_DMA_QWC_MASK) | 0x30000000), aBuffer);
    aPacket->imageGifTag1 = (u64)(0x51000000 | qwc) << 32;
    aPacket->flushTag0 = 0x20000002;
    aPacket->flushTag1 = 0x5000000200000000;
    aPacket->flushGifTag = 0x1000000000008001;
    aPacket->flushRegister = 0xE;
    aPacket->zero = 0;
    aPacket->finishRegister = 0x3F;
}

void sdfLinkLoadImage(sdfDrawTag_t *aDrawTag, sdfGsMemSlice_t *aGsHandle,
                   s32 aDstX, s32 aDstY, s32 aWidth, s32 aHeight,
                   saddr_t aBuffer, s32 (*aAllocator)(s32)) {
    s32 packetAddress;
    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packetAddress = aAllocator(SDF_DESCRIPTOR_PACKET_BYTES);
    sdfSetLoadImage(EE_ADDR_AS(sdfLoadImagePacket_t *, packetAddress), aGsHandle,
                  aDstX, aDstY, aWidth, aHeight, aBuffer);
    sdfDrawTagLink2(aDrawTag, packetAddress, packetAddress + SDF_DESCRIPTOR_PACKET_TAIL_OFFSET);
}
