#include "common.h"
#include "libsdf.h"

#include "ps2asm.h"

#define sdfTexFree sdfTexDestroy
#define sdfTexFreeDelayed sdfTexReleaseReferenceViaHandler
extern sdfTexHandle_t *sdfTexHandleLink;
extern sdfDelayedResrcCB_t sdfTexDelayedResrcCB;

extern void sdfDelayedResrcInitCB(sdfDelayedResrcCB_t *aCB, sdfDelayedResrcFreeFunc aFreeFunc);
extern void sdfDelayedResrcEntry(sdfDelayedResrcCB_t *aCB, void *aItem);

extern void *sdfChipAllocDirect(s32 aSize);
extern void *sdfChipAllocZeroDirect(s32 aSize);
extern void sdfChipFreeDirect(void *aPtr);
extern void *sdfAllocAuto(s32 size);
extern void sdfFreeAuto(void *ptr);
extern void sdfGsMemFree(sdfGsMemSlice_t *aHandle);
extern sdfGsMemSlice_t *sdfGsMemAllocFromTail(s32 aSize, s32 aType);
extern void *memcpy(void *a, const void *b, size_t c);

extern sdfTexHandle_t *sdfTexSetEx(TMX_HEADER_t *aTex, s32 aMemMode);

extern void *sdfTexTransData(s32 aGsAddr, s32 aWidth, s32 aHeight, s32 aPixelFormat, void *aSrcData,
                             s32 aMemMode);
extern void sdfTexSetPacket(sdfTexPacket_t *aPacket, s32 aWidth, s32 aHeight, s32 aGsAddr,
                            s32 aPixelFormat, s32 aClutAddr, s32 aClutFormat, s32 aColorComp,
                            s32 aNumOfMipmap, s32 aMipmapParm, s32 aWrapMode, s32 aContext);

sdfGsMemSlice_t *sdfTexAllocClut(s32 aPixelFormat, s32 aCLUTFormat, s32 aNumOfCLUT);
void sdfTexAllocClutBuff(sdfTexHandle_t *aHandle);
void sdfTexCopyClutData(sdfTexHandle_t *aHandle, void *aData);
void sdfTexMakeGIFPacket(sdfTexHandle_t *aHandle);
void sdfTexMakeGIFPacketCtx2(sdfTexHandle_t *aHandle);
void sdfTexRebuildPacket(sdfTexHandle_t *aHandle);

/* One image transfer request; dimensions and source rectangle are halfwords. */
typedef struct
{
    void *pixels;
    sdfMemHandle_t *allocation;
    u8 allocationMode;
    u8 format;
    s16 bufferWidth;
    u32 destination;
    s16 x;
    s16 y;
    s16 width;
    s16 height;
} sdfImageUploadRequest_t;

extern s32 sdfImageDataSize(s32 format, s32 width, s32 height);
extern void sdfImageSetReq(sdfImageUploadRequest_t *parm);
extern sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);
extern void *sdfMemLock(sdfMemHandle_t *aHandle);

sdfTexPacket_t *sdfTexGetPacket(sdfTexHandle_t *aHandle)
{
    return aHandle->packet;
}

s32 sdfTexGetPacketLength(sdfTexHandle_t *aHandle)
{
    sdfTexPacket_t *packet;

    packet = aHandle->packet;
    if (packet == NULL) {
        return 0;
    }
    return (((u32)packet->giftag.parm & 0x7FFF) + 1) << 4;
}

sdfTexPacket_t *sdfTexGetPacketCtx2(sdfTexHandle_t *aHandle)
{
    sdfTexPacket_t *packet;

    packet = aHandle->packetCtx2;
    if (packet == NULL) {
        sdfTexMakeGIFPacketCtx2(aHandle);
        packet = aHandle->packetCtx2;
    }
    return packet;
}

s32 sdfTexGetPacketCtx2Length(sdfTexHandle_t *aHandle)
{
    sdfTexPacket_t *packet;

    packet = aHandle->packetCtx2;
    if (packet == NULL) {
        return 0;
    }
    return (((u32)packet->giftag.parm & 0x7FFF) + 1) << 4;
}

s32 sdfTexGetCLUTCount(sdfTexHandle_t *aHandle)
{
    return aHandle->numOfCLUT;
}

u32 sdfTexGetCLUTAddr(sdfTexHandle_t *aHandle)
{
    sdfGsMemSlice_t *slice;

    slice = aHandle->clutGsHandle;
    if (slice == NULL) {
        return 0;
    }
    return slice->addr;
}

u32 sdfTexGetPixelAddr(sdfTexHandle_t *aHandle)
{
    return aHandle->pixelGsHandle->addr;
}

s32 sdfTexGetPixelBitCount(s32 aFormat)
{
    switch (aFormat) {
    case 0:
        return 32;
    case 1:
        return 24;
    case 2:
    case 10:
        return 16;
    case 19:
    case 27:
        return 8;
    default:
        return 4;
    }
}

s32 sdfTexGetTexelBitCount(s32 aFormat)
{
    s32 bitCount;

    switch (aFormat) {
    case 19:
        bitCount = 8;
        break;
    case 20:
        bitCount = 4;
        break;
    case 2:
    case 10:
        bitCount = 16;
        break;
    default:
        bitCount = 32;
        break;
    }
    return bitCount;
}

u64 sdfTexGetTex0Reg(sdfTexHandle_t *aHandle)
{
    return aHandle->packet->tex0.data;
}

u64 sdfTexGetTex1Reg(sdfTexHandle_t *aHandle)
{
    return aHandle->packet->tex1.data;
}

u64 sdfTexGetClampReg(sdfTexHandle_t *aHandle)
{
    return aHandle->packet->clamp.data;
}

void sdfTexSetFilter(sdfTexHandle_t *aHandle, s32 aMagMode, s32 aMinMode)
{
    sdfTexPacket_t *packet;

    packet = aHandle->packet;
    packet->tex1.data = (packet->tex1.data & ~0x1E0) | (aMagMode << 5) | (aMinMode << 6);
}

void sdfTexSetFilterCtx2(sdfTexHandle_t *aHandle, s32 aMagMode, s32 aMinMode)
{
    sdfTexPacket_t *packet;

    packet = aHandle->packetCtx2;
    if (packet == NULL) {
        sdfTexMakeGIFPacketCtx2(aHandle);
        packet = aHandle->packetCtx2;
    }
    packet->tex1.data = (packet->tex1.data & ~0x1E0) | (aMagMode << 5) | (aMinMode << 6);
}

void sdfTexSetWrapMode(sdfTexHandle_t *aHandle, s32 aMode)
{
    aHandle->texWrapMode = aMode;
    sdfTexRebuildPacket(aHandle);
}

void *sdfTexTransData(s32 aGsAddr, s32 aWidth, s32 aHeight, s32 aPixelFormat, void *aSrcData, s32 aMemMode) {
    sdfImageUploadRequest_t request;
    s32 imageBytes = sdfImageDataSize(aPixelFormat, aWidth, aHeight) * 16;

    if (aMemMode == 0) {
        if (imageBytes > 0x400) {
            request.allocation = sdfMemAllocDirect(imageBytes);
            request.pixels = (void *)sdfMemLock(request.allocation);
            request.allocationMode = 1;
        } else {
            request.pixels = sdfChipAllocDirect(imageBytes);
            request.allocationMode = 2;
        }
        memcpy(request.pixels, aSrcData, imageBytes);
    } else {
        request.pixels = aSrcData;
        request.allocationMode = 0;
    }
    request.format = aPixelFormat;
    request.destination = aGsAddr;
    request.bufferWidth = aWidth;
    request.width = aWidth;
    request.height = aHeight;
    request.x = 0;
    request.y = 0;
    sdfImageSetReq(&request);
    return (u8 *)aSrcData + imageBytes;
}

void *sdfTexTransCLUT(sdfTexHandle_t *aHandle, s32 aGsAddr, void *aDataPtr, s32 aMemMode)
{
    s32 width;
    s32 height;

    if (aHandle->pixelFormat == 19 || aHandle->pixelFormat == 27) {
        width = 16;
        height = 16;
    } else {
        width = 8;
        height = 2;
    }
    return sdfTexTransData(aGsAddr, width, height, aHandle->CLUTFormat, aDataPtr, aMemMode);
}

void sdfTexClutToGs(sdfTexHandle_t *aHandle)
{
    if (aHandle->clutGsHandle != NULL) {
        sdfTexTransCLUT(aHandle, sdfTexGetCLUTAddr(aHandle), aHandle->clutData, 0);
    }
}

void sdfTexLink(sdfTexHandle_t *handle)
{
    handle->prev = NULL;
    if (sdfTexHandleLink != NULL) {
        handle->next = sdfTexHandleLink;
        sdfTexHandleLink->prev = handle;
    } else {
        handle->next = NULL;
    }
    sdfTexHandleLink = handle;
}

sdfTexHandle_t *sdfTexNew(s32 aWidth, s32 aHeight, s32 aPixelFormat, s32 aNumOfMipmap,
                          sdfGsMemSlice_t *aPixelGsHandle, s32 aCLUTFormat, s32 aNumOfCLUT,
                          sdfGsMemSlice_t *aCLUTGsHandle)
{
    sdfTexHandle_t *handle = sdfChipAllocZeroDirect(sizeof(sdfTexHandle_t));
    sdfTexRef_t *ref = sdfChipAllocZeroDirect(sizeof(sdfTexRef_t));

    ref->refCount = 1;
    handle->numOfCLUT = aNumOfCLUT;
    handle->CLUTFormat = aCLUTFormat;
    handle->width = aWidth;
    handle->height = aHeight;
    handle->pixelFormat = aPixelFormat;
    handle->numOfMipmap = aNumOfMipmap;
    handle->clutGsHandle = aCLUTGsHandle;
    handle->pixelGsHandle = aPixelGsHandle;
    handle->ref = ref;
    sdfTexLink(handle);
    handle->clutColor = 0x80808080;
    return handle;
}

sdfTexHandle_t *sdfTexDup(sdfTexHandle_t *aHandle)
{
    sdfTexHandle_t *dup;
    sdfTexRef_t *ref;
    sdfTexHandle_t *origin;

    dup = sdfChipAllocDirect(64);
    *dup = *aHandle;
    ref = sdfChipAllocZeroDirect(sizeof(*ref));
    origin = ref->origin;
    ref->refCount = 1;
    dup->ref = ref;
    if (origin == NULL) {
        origin = aHandle;
        ref->origin = origin;
    }
    dup->packet = NULL;
    dup->packetCtx2 = NULL;
    dup->clutData = NULL;
    dup->monoClutData = NULL;
    sdfTexLink(dup);
    if (dup->clutGsHandle != NULL) {
        dup->clutGsHandle = sdfTexAllocClut(dup->pixelFormat, dup->CLUTFormat, dup->numOfCLUT);
        sdfTexAllocClutBuff(dup);
        sdfTexCopyClutData(dup, origin->clutData);
        dup->clutColor = 0x80808080;
    }
    sdfTexMakeGIFPacket(dup);
    sdfTexClutToGs(dup);
    return dup;
}

sdfGsMemSlice_t *sdfTexAllocClut(s32 aPixelFormat, s32 aCLUTFormat, s32 aNumOfCLUT)
{
    sdfGsMemSlice_t *slice;
    s32 bitsPerEntry;
    s32 size;
    s16 height;

    slice = NULL;
    switch (aCLUTFormat) {
    case 0:
        bitsPerEntry = 32;
        break;
    case 2:
    case 10:
        bitsPerEntry = 16;
        break;
    default:
        return NULL;
    }

    switch (aPixelFormat) {
    case 19:
    case 27:
        size = bitsPerEntry * 8;
        height = 32;
        break;
    case 20:
    case 36:
    case 44:
        size = 64;
        height = 8;
        break;
    default:
        return slice;
    }

    slice = sdfGsMemAllocFromTail(size, 3);
    slice->w = 8;
    slice->h = height;
    slice->pixelFormat = aCLUTFormat;
    return slice;
}

s32 __attribute__((const)) sdfTexGetClutDataSize(s32 aPixelFormat, s32 aCLUTFormat, s32 aNumOfCLUT)
{
    s32 entries;
    s32 bytesPerEntry;

    bytesPerEntry = (aCLUTFormat == 0) ? 4 : 2;
    entries = (aPixelFormat == 19 || aPixelFormat == 27) ? 256 : 16;
    return entries * bytesPerEntry * aNumOfCLUT;
}

void sdfTexAllocClutBuff(sdfTexHandle_t *aHandle)
{
    s32 size;

    size = sdfTexGetClutDataSize(aHandle->pixelFormat, aHandle->CLUTFormat, aHandle->numOfCLUT);
    aHandle->clutDataSize = size;
    aHandle->clutData = sdfAllocAuto(size);
}

void sdfTexCopyClutData(sdfTexHandle_t *aHandle, void *aData)
{
    memcpy(aHandle->clutData, aData, aHandle->clutDataSize);
}

sdfTexHandle_t *sdfTexAlloc(s32 aWidth, s32 aHeight, s32 aPixelFormat, s32 aNumOfMipmap,
                            s32 aCLUTFormat, s32 aNumOfCLUT)
{
    s32 indexed;
    s32 bitsPerPixel;
    sdfGsMemSlice_t *clutSlice;
    sdfGsMemSlice_t *pixelSlice;
    s32 mipWidth;
    s32 mipHeight;
    s32 words;
    s32 mipLevel;

    indexed = 0;
    switch (aPixelFormat) {
    case 0:
    case 1:
        bitsPerPixel = 32;
        break;
    case 2:
        bitsPerPixel = 16;
        break;
    case 10:
        bitsPerPixel = 16;
        break;
    case 19:
        indexed = 1;
        bitsPerPixel = 8;
        break;
    case 20:
        indexed = 1;
        bitsPerPixel = 4;
        break;
    default:
        return NULL;
    }

    clutSlice = NULL;
    if (indexed) {
        clutSlice = sdfTexAllocClut(aPixelFormat, aCLUTFormat, aNumOfCLUT);
    }

    mipWidth = aWidth;
    mipHeight = aHeight;
    words = 0;
    mipLevel = 0;
    do {
        words += (mipWidth * mipHeight * bitsPerPixel) >> 5;
        mipWidth >>= 1;
        mipHeight >>= 1;
    } while (mipLevel++ != aNumOfMipmap);

    pixelSlice = sdfGsMemAllocFromTail(words, 2);
    pixelSlice->w = aWidth;
    pixelSlice->h = aHeight;
    pixelSlice->pixelFormat = aPixelFormat;
    return sdfTexNew(aWidth, aHeight, aPixelFormat, aNumOfMipmap, pixelSlice, aCLUTFormat,
                     aNumOfCLUT, clutSlice);
}

void sdfTexFree(sdfTexHandle_t *aHandle)
{
    sdfTexHandle_t *next;
    sdfTexHandle_t *prev;

    if (aHandle->ref->origin == NULL) {
        sdfGsMemFree(aHandle->pixelGsHandle);
    }
    sdfGsMemFree(aHandle->clutGsHandle);
    sdfChipFreeDirect(aHandle->packet);
    sdfChipFreeDirect(aHandle->packetCtx2);
    prev = aHandle->prev;
    next = aHandle->next;
    if (next != NULL) {
        next->prev = prev;
    }
    if (prev != NULL) {
        prev->next = next;
    } else {
        sdfTexHandleLink = next;
    }
    sdfFreeAuto(aHandle->clutData);
    sdfFreeAuto(aHandle->monoClutData);
    sdfChipFreeDirect(aHandle->ref);
    sdfChipFreeDirect(aHandle);
}

void sdfTexReleaseReference(sdfTexHandle_t *aHandle)
{
    sdfTexRef_t *ref;
    s32 count;

    if (aHandle != NULL) {
        ref = aHandle->ref;
        count = ref->refCount - 1;
        ref->refCount = count;
        if (count == 0) {
            aHandle->texId = 0;
            sdfTexFree(aHandle);
        }
    }
}

void sdfTexFreeDelayed(sdfTexHandle_t *aHandle)
{
    sdfTexRef_t *ref;
    s32 count;

    if (aHandle != NULL) {
        ref = aHandle->ref;
        count = ref->refCount - 1;
        ref->refCount = count;
        if (count == 0) {
            aHandle->texId = 0;
            sdfDelayedResrcEntry(&sdfTexDelayedResrcCB, aHandle);
        }
    }
}

/* NON_MATCHING: reference draft. 15 of 90 words, first differing at +0x0;
 * same 360 bytes, same instruction mix, and every GS field and both OR trees
 * are exact. What is left is the schedule of the first fifty instructions:
 * cc1 models the four-instruction bit-length asm block as one unit-latency
 * insn, so the ready list around it differs from the hand-written order, and
 * swapping the two bit-length statements leaves the output byte-identical.
 * Behaviour checked against retail: 4 of 4 PCSX2 call vectors give the same
 * packet bytes, battle savestate (sdfport a1f0fdd, tools/funcvec, tests/funcvec.sh). */
#ifdef NON_MATCHING
void sdfTexSetPacket(sdfTexPacket_t *aPacket, s32 aWidth, s32 aHeight, s32 aGsAddr,
                     s32 aPixelFormat, s32 aClutAddr, s32 aClutFormat, s32 aColorComp,
                     s32 aNumOfMipmap, s32 aMipmapParm, s32 aWrapMode, s32 aContext)
{
    s64 cpsm = (s64)aClutFormat << 51;
    s64 cbp = (s64)(aClutAddr >> 6) << 37;
    s64 tcc = (s64)aColorComp << 34;
    s64 th = (s64)PS2_WORD_BIT_LENGTH(aHeight - 1) << 30;
    s32 tw = PS2_WORD_BIT_LENGTH(aWidth - 1) << 26;
    s32 psm = aPixelFormat << 20;
    s32 tbw = (aWidth >> 6) << 14;
    s32 tbp0 = aGsAddr >> 6;
    s64 k = (s64)(aMipmapParm & 0xFFF) << 32;
    s64 l = (s64)((aMipmapParm >> 12) & 3) << 19;
    s64 mxl = aNumOfMipmap << 2;
    s64 tex0;
    s64 tex1;

    aPacket->giftag.regs = 0xE;
    aPacket->giftag.parm = 0x1000000000008003;
    tex0 = cpsm | cbp | tcc | th | tw | psm | tbw | tbp0;
    if (aNumOfMipmap == 0) {
        tex1 = 0x61;
    } else {
        tex1 = k | l | 0x360 | mxl;
    }
    aPacket->tex1.data = tex1;
    aPacket->tex1.reg = aContext == 0 ? 0x14 : 0x15;
    if (aClutAddr != 0) {
        tex0 = cpsm | cbp | ((s64)4 << 61) | tcc | th | tw | psm | tbw | tbp0;
    }
    aPacket->tex0.data = tex0;
    aPacket->tex0.reg = aContext == 0 ? 6 : 7;
    if ((aWrapMode & 0xFF) == 0xFF) {
        aPacket->clamp.data = 0;
    } else {
        aPacket->clamp.data = aWrapMode & 0xF;
    }
    aPacket->clamp.reg = aContext == 0 ? 8 : 9;
}
#else
INCLUDE_ASM(const s32, "sdf/sdfTex", sdfTexSetPacket);
#endif

sdfTexPacket_t *sdfTexMakeGIFPacketSub(sdfTexHandle_t *aHandle, s32 aContext)
{
    sdfTexPacket_t *packet;
    u32 pixelAddr;
    u32 clutAddr;

    packet = sdfChipAllocDirect(64);
    pixelAddr = sdfTexGetPixelAddr(aHandle);
    clutAddr = sdfTexGetCLUTAddr(aHandle);
    sdfTexSetPacket(packet, aHandle->width, aHandle->height, pixelAddr, aHandle->pixelFormat,
                    clutAddr, aHandle->CLUTFormat, 1, aHandle->numOfMipmap, aHandle->mipmapParam,
                    aHandle->texWrapMode, aContext);
    return packet;
}

void sdfTexMakeGIFPacket(sdfTexHandle_t *aHandle)
{
    aHandle->packet = sdfTexMakeGIFPacketSub(aHandle, 0);
}

void sdfTexMakeGIFPacketCtx2(sdfTexHandle_t *aHandle)
{
    aHandle->packetCtx2 = sdfTexMakeGIFPacketSub(aHandle, 1);
}

void sdfTexRebuildPacket(sdfTexHandle_t *aHandle)
{
    sdfTexPacket_t *packet;

    packet = aHandle->packet;
    if (packet != NULL) {
        sdfTexSetPacket(packet, aHandle->width, aHandle->height, sdfTexGetPixelAddr(aHandle),
                        aHandle->pixelFormat, sdfTexGetCLUTAddr(aHandle), aHandle->CLUTFormat,
                        (packet->tex0.data >> 34) & 1, aHandle->numOfMipmap, aHandle->mipmapParam,
                        aHandle->texWrapMode, 0);
    }

    packet = aHandle->packetCtx2;
    if (packet != NULL) {
        sdfTexSetPacket(packet, aHandle->width, aHandle->height, sdfTexGetPixelAddr(aHandle),
                        aHandle->pixelFormat, sdfTexGetCLUTAddr(aHandle), aHandle->CLUTFormat,
                        (packet->tex0.data >> 34) & 1, aHandle->numOfMipmap, aHandle->mipmapParam,
                        aHandle->texWrapMode, 1);
    }
}

sdfTexHandle_t *sdfTexSetEx(TMX_HEADER_t *aTex, s32 aMemMode)
{
    sdfTexHandle_t *handle;
    void *data;
    s32 gsAddr;
    s32 width;
    s32 height;
    s32 level;
    s32 pixelFormat;
    s32 bitCount;
    s32 key = aTex->resourceKey;

    if (key != 0) {
        sdfTexHandle_t *existing = sdfTexHandleLink;
        while (existing != NULL) {
            if (existing->texId == key) {
                existing->ref->refCount++;
                return existing;
            }
            existing = existing->next;
        }
    }

    handle = sdfTexAlloc(aTex->width, aTex->height, aTex->pixelFormat, aTex->numOfMipmap,
                         aTex->CLUTFormat, aTex->numOfCLUT);
    handle->mipmapParam = aTex->mipmapParam;
    handle->flags = aTex->flags;
    handle->texWrapMode = aTex->texWrapMode;
    handle->texId = aTex->resourceKey;
    handle->clutId = aTex->battleTextureSlot;
    data = (u8 *)aTex + (aTex->compression & 0xF0) + sizeof(TMX_HEADER_t);
    if (sdfTexGetCLUTCount(handle) != 0) {
        sdfTexAllocClutBuff(handle);
        sdfTexCopyClutData(handle, data);
        data = sdfTexTransCLUT(handle, sdfTexGetCLUTAddr(handle), data, aMemMode);
    }
    pixelFormat = handle->pixelFormat;
    width = handle->width;
    height = handle->height;
    level = handle->numOfMipmap;
    gsAddr = sdfTexGetPixelAddr(handle);
    bitCount = sdfTexGetTexelBitCount(pixelFormat);
    do {
        data = sdfTexTransData(gsAddr, width, height, pixelFormat, data, aMemMode);
        level--;
        gsAddr += (bitCount * width * height) >> 5;
        width >>= 1;
        height >>= 1;
    } while (level != -1);
    sdfTexMakeGIFPacket(handle);
    return handle;
}

sdfTexHandle_t *sdfTexSet(TMX_HEADER_t *aTex)
{
    return sdfTexSetEx(aTex, 0);
}

sdfTexHandle_t *sdfTexSetDirect(TMX_HEADER_t *aTex)
{
    return sdfTexSetEx(aTex, 1);
}

void sdfTexSetMonoClut(sdfTexHandle_t *aHandle)
{
    s32 bytesPerEntry;
    s32 entryCount;
    u8 *out;
    u8 *src;
    u32 color;
    u32 r;
    u32 g;
    u32 b;

    if (aHandle->CLUTFormat == 0) {
        bytesPerEntry = 4;
        entryCount = (u32)aHandle->clutDataSize >> 2;
    } else {
        bytesPerEntry = 2;
        entryCount = (u32)aHandle->clutDataSize >> 1;
    }
    out = aHandle->monoClutData;
    if (out == NULL) {
        out = sdfAllocAuto(entryCount);
        aHandle->monoClutData = out;
    }
    src = aHandle->clutData;
    do {
        if (bytesPerEntry == 4) {
            color = *(u32 *)src;
            src += 4;
        } else {
            u16 packed = *(u16 *)src;

            src += 2;
            /* 1555 to 8888: pext5 spreads the five-bit fields, then the top
             * three bits of each are copied down into the low bits. */
            MMI_PEXT5(color, packed);
            color |= (color >> 5) & 0x070707;
        }
        g = (color >> 8) & 0xFF;
        r = color & 0xFF;
        b = (color >> 16) & 0xFF;
        entryCount--;
        *out = ((r * 77) >> 8) + ((g * 150) >> 8) + ((b * 29) >> 8);
        out++;
    } while (entryCount != 0);
}

void sdfTexSetClutColor(sdfTexHandle_t *texture, s32 tint, f32 amount)
{
    sdfTexHandle_t *source = texture->ref->origin;
    u32 count;
    u8 *intensity;

    intensity = source->monoClutData;
    if (intensity == NULL) {
        sdfTexSetMonoClut(source);
        intensity = source->monoClutData;
    }

    MMI_PALETTE_TINT_SETUP(amount, tint, 1.0f / 255.0f);

    if (texture->CLUTFormat == 0) {
        u32 *sourcePalette = (u32 *)source->clutData;
        u32 *destinationPalette = (u32 *)texture->clutData;
        count = (u32)texture->clutDataSize >> 2;
        do {
            u32 value = *intensity;
            u32 color = *sourcePalette;

            MMI_PALETTE_TINT_RGBA(value, color);
            count--;
            *destinationPalette = value;
            sourcePalette++;
            intensity++;
            destinationPalette++;
        } while (count != 0);
    } else {
        u16 *sourcePalette;
        u16 *destinationPalette;
        u32 fiveBitExpandMask = 0x070707;

        count = (u32)texture->clutDataSize >> 1;
        sourcePalette = (u16 *)source->clutData;
        destinationPalette = (u16 *)texture->clutData;
        do {
            u32 value = *intensity;
            u16 color = *sourcePalette;

            MMI_PALETTE_TINT_RGB5(color, value, fiveBitExpandMask);
            count--;
            *destinationPalette = color;
            sourcePalette++;
            intensity++;
            destinationPalette++;
        } while (count != 0);
    }

    sdfTexClutToGs(texture);
}

sdfTexHandle_t *sdfTexSearch(u32 id)
{
    sdfTexHandle_t *handle;

    for (handle = sdfTexHandleLink; handle != NULL; handle = handle->next) {
        if (handle->texId == id) {
            return handle;
        }
    }
    return NULL;
}

void sdfTexInit(void)
{
    sdfDelayedResrcInitCB(&sdfTexDelayedResrcCB, (sdfDelayedResrcFreeFunc)sdfTexFree);
}

INCLUDE_SDATA(const s32, "sdf/sdfTex", sdfTexHandleLink);

