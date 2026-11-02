#include "sdk_ps2.h"
#include "common.h"
#include "eeregs.h"
#include "sdf_timer.h"
#include "sdf_texture_queue.h"
#include "sdf_image_upload.h"
#include "sdf_thread.h"
#include "ee_mmi.h"
#include "sdf_chip.h"
#include "sdf_chip_range_api.h"
#include "sdf_heap_api.h"
#include "sdf.h"
#include "sdf_gs_mem_delayed_free_api.h"
#include "sdf_graph_buffer_state.h"
#include "sdf_display_thread_state.h"
#include "sdf_draw_frame_execution_state.h"
#include "sdf_semaphore_api.h"
#include "sdf_texture_primary_buffer_api.h"
#include "sdf_texture_submit.h"
#include "sdf_texture_lists.h"
#include "sdf_texture_registry.h"
#include "sdf_pending.h"
#include "sdf_texture_release.h"
#include "sdf_image_packets.h"
#include "sdf_tex_copy_image_data_api.h"
#include "sdf_vu_transform_api.h"
#include "sdf_handler_node.h"
#include "sdf_texture_file.h"
#include "sdf_tex_lifetime.h"
#include "sdf_tex_create.h"
#include "sdf_tex_packet.h"
#include "btl.h"
#include "sdf_scene_packet.h"
#include "sdf_gs_gouraud_textured.h"
#include "sdf_gs_scene_state.h"
#include "sdf_gs_textured_shapes.h"
#include "sdf_request.h"
#include "sdf_gs_context.h"
#include "sdf_gs_gouraud_shapes.h"
#include "sdf_gs_header.h"
#include "sdf_gs_geometry.h"
#include "sdf_texture_flush.h"
#include "sdf_gs_blend.h"
#include "sdf_delayed_resource_check_api.h"
#include "sdf_vu0.h"
#include "sdf_packet_list.h"
#include "sdf_packet_slot.h"
#include "sdf_queued_resource_api.h"
#include "sdf_linked_packet.h"
#include "sdf_packet_builders.h"
#include "sdf_gs_flat_shapes.h"
#include "sdf_gs_packet.h"
#include "pcp_vu0.h"
#include "sdf_draw.h"
#include "sdf_model.h"
#include "sdf_dev_slot_api.h"
#include "sdf_dma_tag.h"
#include "sdf_packet_alloc.h"

/* GS pixel-storage modes, using the same private names as sdfTex.c. */
enum { SDF_PSMT4HL = 0x24, SDF_PSMT4HH = 0x2C,
    SDF_PSMZ32 = 0x30, SDF_PSMZ24 = 0x31, SDF_PSMZ16 = 0x32,
    SDF_PSMZ16S = 0x3A

};

#define SDF_GIF_LOOP_COUNT_MASK 0x7FFF

#define SDF_QWORD_BYTE_SHIFT 4

#define SDF_TEX_FILTER_MASK 0x1E0

#define SDF_MAG_FILTER_SHIFT 5

#define SDF_MIN_FILTER_SHIFT 6

#define SDF_UPLOAD_CHIP_MAX_BYTES 0x400

extern SdfGraphBufferState sdfDispFrameId[2];

/* Busy-buffer index is published by the slot setters and polled below;
 * volatile prevents the wait loop from reusing an earlier read. */
extern volatile SdfGraphBufferState sdfGrDrawBusy;

extern u8 sdfGrCaptureBuff;

extern u8 *sdfGrCaptureReq;

extern void (*sdfGrCaptureTarget)(u8 *);

extern void (*sdfGsErrorHandler)(s32 size, s32 allocationMode);

struct sdfGsMemSlice_t *sdfGsMemAllocFromHead(s32 size, s32 allocationMode);

void sdfTexMakeGIFPacketCtx2(sdfTexHandle_t *texture);

void sdfTexRebuildPacket(sdfTexHandle_t *texture);

extern volatile SdfDisplayThreadState sdfDispThreadFlag;

extern s32 dds3GsThreadId;

sdfGsMemSlice_t *sdfGsMemAllocFromHead(s32 size, s32 allocationMode);

sdfGsMemSlice_t *sdfGsMemAllocFromTail(s32 size, s32 allocationMode);

/* Transfer size in 16-byte quadwords; low bits of a partial quadword are discarded. */ s32 sdfImageDataSize(u32 format, s32 width, s32 height);

/* Queue chunked GS image DMA and its pixel/allocation ownership metadata under the semaphore. */ void sdfImageSetReq(const SdfImageUploadRequest *request);

const sdfTexPacket_t *sdfTexGetPacket(const sdfTexHandle_t *texture) {
    return texture->primaryBuffer;
}

/* Size in bytes of a packed primary texture buffer: only the low 15 bits
 * contribute to its 16-byte block count. */
s32 sdfTexGetPacketLength(const sdfTexHandle_t *texture) {
    const sdfTexPacket_t *buffer = texture->primaryBuffer;

    if (buffer == NULL) {
        return 0;
    }
    return ((buffer->gifTagWord & SDF_GIF_LOOP_COUNT_MASK) + 1) << SDF_QWORD_BYTE_SHIFT;
}

const sdfTexPacket_t *sdfTexGetPacketCtx2(sdfTexHandle_t *texture) {
    sdfTexPacket_t *buf;

    buf = texture->secondaryBuffer;
    if (buf == NULL) {
        sdfTexMakeGIFPacketCtx2(texture);
        buf = texture->secondaryBuffer;
    }
    return buf;
}

/* Mirror the primary-buffer size calculation for the secondary buffer. */
s32 sdfTexGetPacketCtx2Length(const sdfTexHandle_t *texture) {
    const sdfTexPacket_t *buffer = texture->secondaryBuffer;

    if (buffer == NULL) {
        return 0;
    }
    return ((buffer->gifTagWord & SDF_GIF_LOOP_COUNT_MASK) + 1) << SDF_QWORD_BYTE_SHIFT;
}

u8 sdfTexGetCLUTCount(sdfTexHandle_t *texture) {
    return texture->paletteCount;
}

u32 sdfTexGetCLUTAddr(sdfTexHandle_t *texture) {
    u32 word;

    word = 0;
    if (texture->secondaryResource != NULL) {
        word = texture->secondaryResource->word;
    }
    return word;
}

u32 sdfTexGetPixelAddr(sdfTexHandle_t *texture) {
    return texture->primaryResource->word;
}

/* Color-format pixel depth; every unlisted format falls through to four bits. */
s32 sdfTexGetPixelBitCount(u32 format) {
    switch (format) {
    case SDF_PSMCT32:
        return 0x20;
    case SDF_PSMCT24:
        return 0x18;
    case SDF_PSMCT16:
    case SDF_PSMCT16S:
        return 0x10;
    case SDF_PSMT8:
    case SDF_PSMT8H:
        return 8;
    default:
        return 4;
    }
}

/* Storage-depth subset for indexed and 16-bit color formats; all other values return 32. */
s32 sdfTexGetTexelBitCount(s32 format) {
    s32 storageBitsPerPixel;

    switch (format) {
    case SDF_PSMT8:
        storageBitsPerPixel = 8;
        break;
    case SDF_PSMT4:
        storageBitsPerPixel = 4;
        break;
    case SDF_PSMCT16:
    case SDF_PSMCT16S:
        storageBitsPerPixel = 16;
        break;
    default:
        storageBitsPerPixel = 32;
        break;
    }
    return storageBitsPerPixel;
}

u64 sdfTexGetTex0Reg(sdfTexHandle_t *texture) {
    return texture->primaryBuffer->textureState;
}

u64 sdfTexGetTex1Reg(sdfTexHandle_t *texture) {
    return texture->primaryBuffer->samplingState;
}

u64 sdfTexGetClampReg(sdfTexHandle_t *texture) {
    return texture->primaryBuffer->clampState;
}

/* Clear TEX1 MMAG/MMIN and OR in the supplied filters; input values are not masked. */
void sdfTexSetFilter(sdfTexHandle_t *texture, s32 magFilter, s32 minFilter) {
    sdfTexPacket_t *buffer;

    buffer = texture->primaryBuffer;
    buffer->samplingState = (buffer->samplingState & ~SDF_TEX_FILTER_MASK) | (magFilter << SDF_MAG_FILTER_SHIFT) | (minFilter << SDF_MIN_FILTER_SHIFT);
}

/* Create the secondary packet if absent, then update TEX1 filters without masking input values. */
void sdfTexSetFilterCtx2(sdfTexHandle_t *texture, s32 magFilter, s32 minFilter) {
    sdfTexPacket_t *buffer = texture->secondaryBuffer;

    if (buffer == NULL) {
        sdfTexMakeGIFPacketCtx2(texture);
        buffer = texture->secondaryBuffer;
    }
    buffer->samplingState = (buffer->samplingState & ~SDF_TEX_FILTER_MASK) | (magFilter << SDF_MAG_FILTER_SHIFT) | (minFilter << SDF_MIN_FILTER_SHIFT);
}

void sdfTexSetWrapMode(sdfTexHandle_t *texture, SdfTextureClampMode mode) {
    texture->clampMode = mode;
    sdfTexRebuildPacket(texture);
}

/* Borrow pixels or copy them to the selected heap; return the source cursor after the transfer bytes. */
const u8 *sdfTexTransData(u32 destination, s32 width, s32 height, u32 format,
                    const u8 *pixels,
                    SdfTextureUploadOwnership borrowPixels) {
    SdfImageUploadRequest request;
    s32 imageBytes = sdfImageDataSize(format, width, height) * 16;

    if (borrowPixels == SDF_TEXTURE_UPLOAD_COPY) {
        if (imageBytes > SDF_UPLOAD_CHIP_MAX_BYTES) {
            request.allocation = sdfMemAllocDirect(imageBytes);
            request.pixels = (void *)sdfMemLock(request.allocation);
            request.allocationMode = SDF_UPLOAD_GENERAL_HEAP;
        } else {
            request.pixels = sdfChipAllocDirect(imageBytes);
            request.allocationMode = SDF_UPLOAD_CHIP_HEAP;
        }
        memcpy(request.pixels, pixels, imageBytes);
    } else {
        /* The deferred queue keeps this address for DMA; BORROW mode never frees it. */
        request.pixels = (u8 *)pixels;
        request.allocationMode = SDF_UPLOAD_BORROWED;
    }
    request.format = format;
    request.destination = destination;
    request.bufferWidth = width;
    request.width = width;
    request.height = height;
    request.x = 0;
    request.y = 0;
    sdfImageSetReq(&request);
    return pixels + imageBytes;
}

/* Upload the CLUT as 16x16 for 8-bit indexed formats, otherwise 8x2; forward pixel ownership. */
const u8 *sdfTexTransCLUT(sdfTexHandle_t *texture, u32 destination,
                    const u8 *pixels,
                    SdfTextureUploadOwnership borrowPixels) {
    s32 paletteWidth;
    s32 paletteHeight;

    if (texture->pixelFormat == SDF_PSMT8 || texture->pixelFormat == SDF_PSMT8H) {
        paletteWidth = 0x10;
        paletteHeight = 0x10;
    } else {
        paletteWidth = 8;
        paletteHeight = 2;
    }
    return sdfTexTransData(destination, paletteWidth, paletteHeight, texture->clutFormat, pixels, borrowPixels);
}

void sdfTexClutToGs(sdfTexHandle_t *tex) {
    if (tex->secondaryResource != NULL) {
        sdfTexTransCLUT(tex, sdfTexGetCLUTAddr(tex), tex->paletteData,
                        SDF_TEXTURE_UPLOAD_COPY);
    }
}

void sdfTexLink(sdfTexHandle_t *texture) {
    texture->next = NULL;
    if (sdfTexHandleLink != NULL) {
        texture->prev = sdfTexHandleLink;
        sdfTexHandleLink->next = texture;
    } else {
        texture->prev = NULL;
    }
    sdfTexHandleLink = texture;
}

sdfTexHandle_t *sdfTexNew(s32 x, s32 y, s32 pixelFormat, s32 maxMipLevel, sdfGsMemSlice_t *primary, s32 paletteFormat, s32 paletteCount, sdfGsMemSlice_t *secondary) {
    sdfTexHandle_t *tex = sdfChipAllocZeroDirect(sizeof(sdfTexHandle_t));
    SdfTexRef *ref = sdfChipAllocZeroDirect(sizeof(SdfTexRef));

    ref->refCount = 1;
    tex->paletteCount = paletteCount;
    tex->clutFormat = paletteFormat;
    tex->width = x;
    tex->height = y;
    tex->pixelFormat = pixelFormat;
    tex->maxMipLevel = maxMipLevel;
    tex->secondaryResource = secondary;
    tex->primaryResource = primary;
    tex->reference = ref;
    sdfTexLink(tex);
    tex->unk38 = 0x80808080;
    return tex;
}

extern sdfGsMemSlice_t *sdfTexAllocClut(s32, s32, s32);

extern void sdfTexMakeGIFPacket(sdfTexHandle_t *);

sdfTexHandle_t *sdfTexDup(sdfTexHandle_t *source) {
    sdfTexHandle_t *texture;
    SdfTexRef *reference;
    sdfTexHandle_t *original;

    texture = sdfChipAllocDirect(sizeof(*texture));
    *texture = *source;
    reference = sdfChipAllocZeroDirect(sizeof(*reference));
    original = reference->cloneSource;
    reference->refCount = 1;
    texture->reference = reference;
    if (original == NULL) {
        original = source;
        reference->cloneSource = original;
    }
    texture->primaryBuffer = NULL;
    texture->secondaryBuffer = NULL;
    texture->paletteData = NULL;
    texture->intensityMap = NULL;
    sdfTexLink(texture);
    if (texture->secondaryResource != NULL) {
        texture->secondaryResource = sdfTexAllocClut(
            texture->pixelFormat, texture->clutFormat, texture->paletteCount);
        sdfTexAllocClutBuff(texture);
        sdfTexCopyClutData(texture, original->paletteData);
        texture->unk38 = 0x80808080;
    }
    sdfTexMakeGIFPacket(texture);
    sdfTexClutToGs(texture);
    return texture;
}

/* Allocate a texture head for a 0x20-byte (kind 0) or 0x10-byte (kind 2/10) unit; types 19/27 use unit*8 bytes and 0x20 rows, 20/36/44 use 0x40 bytes and 8 rows. */
sdfGsMemSlice_t *sdfTexAllocClut(s32 type, s32 kind, s32 unused) {
    sdfGsMemSlice_t *head = NULL;
    s32 unit;
    s32 size;
    s16 height;

    switch (kind) {
    case 0:
        unit = 0x20;
        break;
    case 2:
    case 10:
        unit = 0x10;
        break;
    default:
        return NULL;
    }
    switch (type) {
    case 19:
    case 27:
        size = unit * 8;
        height = 0x20;
        break;
    case 20:
    case 36:
    case 44:
        size = 0x40;
        height = 8;
        break;
    default:
        return head;
    }
    head = sdfGsMemAllocFromTail(size, SDF_TEX_RESOURCE_CLUT);
    head->width = 8;
    head->height = height;
    head->format = kind;
    return head;
}

/* Declared before first use (were implicit int). */
extern sdfGsMemSlice_t * sdfTexAllocClut(s32 type, s32 kind, s32 unused);

extern sdfGsMemSlice_t * sdfGsMemAllocFromTail(s32 size, s32 allocationMode);

extern sdfTexHandle_t * sdfTexNew(s32 x, s32 y, s32 pixelFormat, s32 maxMipLevel, sdfGsMemSlice_t *primary, s32 paletteFormat, s32 paletteCount, sdfGsMemSlice_t *secondary);

sdfTexPacket_t *sdfTexMakeGIFPacketSub(sdfTexHandle_t *texture, SdfGsContext context);

extern SdfPendingRequest sdfTexDelayedResrcCB;

/* Return palette bytes: 8-bit indices use 256 colors, other indices 16. */
s32 __attribute__((const)) sdfTexGetClutDataSize(s32 textureFormat, s32 paletteFormat, s32 paletteCount) {
    s32 bytesPerColor = (paletteFormat == SDF_PSMCT32) ? 4 : 2;
    s32 colorsPerPalette;

    if ((textureFormat == SDF_PSMT8) || (textureFormat == SDF_PSMT8H)) {
        colorsPerPalette = 0x100;
    } else {
        colorsPerPalette = 0x10;
    }
    return colorsPerPalette * bytesPerColor * paletteCount;
}

void sdfTexAllocClutBuff(sdfTexHandle_t *texture) {
    s32 size = sdfTexGetClutDataSize(texture->pixelFormat, texture->clutFormat,
                                      texture->paletteCount);
    texture->paletteDataSize = size;
    texture->paletteData = sdfAllocAuto(size);
}

/* Copy retained palette bytes without changing the texture resource metadata. */
void sdfTexCopyClutData(sdfTexHandle_t *texture, const u8 *source) {
    memcpy(texture->paletteData, source, texture->paletteDataSize);
}

sdfTexHandle_t *sdfTexAlloc(s32 width, s32 height,
    SdfGsPixelStorageFormat format, u32 maxMipLevel,
    SdfGsPixelStorageFormat paletteFormat, u32 paletteCount) {
    s32 indexed = 0;
    s32 bitsPerPixel;
    sdfGsMemSlice_t *secondary;
    sdfGsMemSlice_t *primary;
    s32 mipWidth;
    s32 mipHeight;
    s32 words;
    u32 mipLevel;

    switch (format) {
    case SDF_PSMCT32:
    case SDF_PSMCT24:
        bitsPerPixel = 32;
        break;
    case SDF_PSMCT16:
        bitsPerPixel = 16;
        break;
    case SDF_PSMCT16S:
        bitsPerPixel = 16;
        break;
    case SDF_PSMT8:
        indexed = 1;
        bitsPerPixel = 8;
        break;
    case SDF_PSMT4:
        indexed = 1;
        bitsPerPixel = 4;
        break;
    default:
        return NULL;
    }
    secondary = NULL;
    if (indexed) {
        secondary = sdfTexAllocClut(format, paletteFormat, paletteCount);
    }
    mipWidth = width;
    mipHeight = height;
    words = 0;
    mipLevel = 0;
    do {
        words += (mipWidth * mipHeight * bitsPerPixel) >> 5;
        mipWidth >>= 1;
        mipHeight >>= 1;
    } while (mipLevel++ != maxMipLevel);
    primary = sdfGsMemAllocFromTail(words, 2);
    primary->width = width;
    primary->height = height;
    primary->format = format;
    return sdfTexNew(width, height, format, maxMipLevel,
        primary, paletteFormat, paletteCount, secondary);
}

/* Destroy both GPU resources and unlink the texture from the active list. */
void sdfTexDestroy(sdfTexHandle_t *texture) {
    sdfTexHandle_t *next;
    sdfTexHandle_t *prev;

    if (texture->reference->cloneSource == NULL) {
        sdfGsMemFree(texture->primaryResource);
    }
    sdfGsMemFree(texture->secondaryResource);
    sdfChipFreeDirect(texture->primaryBuffer);
    sdfChipFreeDirect(texture->secondaryBuffer);
    next = texture->next;
    prev = texture->prev;
    if (prev != NULL) {
        prev->next = next;
    }
    if (next != NULL) {
        next->prev = prev;
    } else {
        sdfTexHandleLink = prev;
    }
    sdfFreeAuto(texture->paletteData);
    sdfFreeAuto(texture->intensityMap);
    sdfChipFreeDirect(texture->reference);
    sdfChipFreeDirect(texture);
}

/* Release a texture when the final ordinary reference is dropped. */
void sdfTexReleaseReference(sdfTexHandle_t *texture) {
    SdfTexRef *ref;
    s32 remainingReferenceCount;

    if (texture != NULL) {
        ref = texture->reference;
        remainingReferenceCount = ref->refCount - 1;
        ref->refCount = remainingReferenceCount;
        if (remainingReferenceCount == 0) {
            texture->resourceKey = 0;
            sdfTexDestroy(texture);
        }
    }
}

/* Hand the last reference to sdfDelayedResrcEntry rather than releasing it here. */
void sdfTexReleaseReferenceViaHandler(sdfTexHandle_t *texture) {
    SdfTexRef *ref;
    s32 remainingReferenceCount;

    if (texture != NULL) {
        ref = texture->reference;
        remainingReferenceCount = ref->refCount - 1;
        ref->refCount = remainingReferenceCount;
        if (remainingReferenceCount == 0) {
            texture->resourceKey = 0;
            sdfDelayedResrcEntry(&sdfTexDelayedResrcCB, texture);
        }
    }
}

#ifdef NON_MATCHING
#include "common.h"
#include "ee_mmi.h"
#include "sdf.h"
#include "sdf_tex_packet.h"

/* log2 of the next power of two at or above size (GS TW/TH): 31 minus
 * plzcw of size - 1, as retail computes it. */
static s32 sdfTexSizeLog2(s32 size) {
    s32 bits;

    size--;
    EE_MMI_PLZCW(bits, size);
    return 0x1F - (bits & 0xFF);
}

/* A three-register A+D packet: TEX1 (filtering and LOD), TEX0 (buffer,
 * size, format, CLUT) and CLAMP, for GS context 1 or 2. Upstream:
 * INCLUDE_ASM in both games. */
void sdfTexSetPacket(struct sdfTexPacket_t *packet, s32 width, s32 height, u32 primaryResourceWord,
                                s32 pixelFormat, u32 secondaryResourceWord, s32 clutFormat,
                                s32 textureColorComponents, s32 maxMipLevel, s32 lodParameters, s32 clampMode,
                                SdfGsContext context) {
    u64 tbw = (u64)(u32)((width >> 6) << 14);
    u64 tw = (u64)(u32)(sdfTexSizeLog2(width) << 26);
    u64 th = (u64)sdfTexSizeLog2(height) << 30;
    u64 psm = (u64)(u32)(pixelFormat << 20);
    u64 cbp = (u64)((s32)secondaryResourceWord >> 6) << 37;
    u64 cpsm = (u64)clutFormat << 51;
    u64 tcc = (u64)textureColorComponents << 34;
    u64 tbp = (u64)(s64)((s32)primaryResourceWord >> 6);
    u64 textureState = cpsm | tbp | th | psm | cbp | tcc | tw | tbw;

    *(u64 *)packet = 0x1000000000008003ULL; /* NLOOP 3, EOP, NREG 1 */
    packet->gifTagRegisters = 0xE;          /* A+D */
    if (maxMipLevel != 0) {
        packet->samplingState = ((u64)(maxMipLevel << 2) | ((u64)(lodParameters & 0xFFF) << 32) |
                                 ((u64)((lodParameters >> 12) & 3) << 19)) |
                                0x360;
    } else {
        packet->samplingState = 0x61;
    }
    packet->samplingRegister = context != SDF_GS_CONTEXT_1 ? 0x15 : 0x14; /* TEX1_2 / TEX1_1 */
    if (secondaryResourceWord != 0) {
        textureState |= 1ULL << 63; /* CLD 4 */
    }
    packet->textureState = textureState;
    packet->textureRegister = context != SDF_GS_CONTEXT_1 ? 7 : 6; /* TEX0_2 / TEX0_1 */
    packet->clampState = (clampMode & 0xFF) == 0xFF ? 0 : (u64)(clampMode & 0xF);
    packet->clampRegister = context != SDF_GS_CONTEXT_1 ? 9 : 8; /* CLAMP_2 / CLAMP_1 */
}
#else
INCLUDE_ASM(const s32, "sdf/sdfTex", sdfTexSetPacket);
#endif

/* Allocate and populate one of the two GS-context resource packets. */
sdfTexPacket_t *sdfTexMakeGIFPacketSub(sdfTexHandle_t *texture, SdfGsContext context) {
    sdfTexPacket_t *packet;
    u32 primary;
    u32 secondary;

    packet = sdfChipAllocDirect(0x40);
    primary = sdfTexGetPixelAddr(texture);
    secondary = sdfTexGetCLUTAddr(texture);
    sdfTexSetPacket(packet, texture->width, texture->height, primary, texture->pixelFormat, secondary, texture->clutFormat, 1, texture->maxMipLevel, texture->lodParameters, texture->clampMode, context);
    return packet;
}

/* Store the first packet on the texture. */
void sdfTexMakeGIFPacket(sdfTexHandle_t *texture) {
    texture->primaryBuffer = sdfTexMakeGIFPacketSub(texture, SDF_GS_CONTEXT_1);
}

/* Store the second packet on the texture. */
void sdfTexMakeGIFPacketCtx2(sdfTexHandle_t *texture) {
    texture->secondaryBuffer = sdfTexMakeGIFPacketSub(texture, SDF_GS_CONTEXT_2);
}

/* Rebuild existing texture packets while preserving each packet's TEX0 TCC bit. */
void sdfTexRebuildPacket(sdfTexHandle_t *texture) {
    sdfTexPacket_t *buffer;

    buffer = texture->primaryBuffer;
    if (buffer != NULL) {
        sdfTexSetPacket(buffer, texture->width, texture->height,
                     sdfTexGetPixelAddr(texture), texture->pixelFormat,
                     sdfTexGetCLUTAddr(texture), texture->clutFormat,
                     (buffer->textureState >> 34) & 1,
                     texture->maxMipLevel, texture->lodParameters,
                     texture->clampMode, SDF_GS_CONTEXT_1);
    }

    buffer = texture->secondaryBuffer;
    if (buffer != NULL) {
        sdfTexSetPacket(buffer, texture->width, texture->height,
                     sdfTexGetPixelAddr(texture), texture->pixelFormat,
                     sdfTexGetCLUTAddr(texture), texture->clutFormat,
                     (buffer->textureState >> 34) & 1,
                     texture->maxMipLevel, texture->lodParameters,
                     texture->clampMode, SDF_GS_CONTEXT_2);
    }
}

sdfTexHandle_t *sdfTexSetEx(const SdfTextureFileHeader *header,
                                           SdfTextureUploadOwnership uploadOwnership) {
    sdfTexHandle_t *texture;
    const u8 *pixels;
    s32 format;
    s32 width;
    s32 height;
    s32 levels;
    u32 destination;
    s32 bits;
    s32 key = header->resourceKey;

    if (key != 0) {
        sdfTexHandle_t *existing = sdfTexHandleLink;
        while (existing != NULL) {
            if (existing->resourceKey == key) {
                existing->reference->refCount++;
                return existing;
            }
            existing = existing->prev;
        }
    }
    texture = sdfTexAlloc(header->width, header->height, header->pixelFormat, header->maxMipLevel, header->clutFormat, header->paletteCount);
    texture->lodParameters = header->lodParameters;
    texture->unk1E = header->unk1A;
    texture->clampMode = header->clampMode;
    texture->resourceKey = header->resourceKey;
    texture->battleTextureSlot = header->battleTextureSlot;
    pixels = (const u8 *)header + (header->payloadOffsetFlags & SDF_TEXTURE_PAYLOAD_EXTRA_OFFSET_MASK) + sizeof(*header);
    if (sdfTexGetCLUTCount(texture) != 0) {
        sdfTexAllocClutBuff(texture);
        sdfTexCopyClutData(texture, pixels);
        pixels = sdfTexTransCLUT(texture, sdfTexGetCLUTAddr(texture), pixels, uploadOwnership);
    }
    format = texture->pixelFormat;
    width = texture->width;
    height = texture->height;
    levels = texture->maxMipLevel;
    destination = sdfTexGetPixelAddr(texture);
    bits = sdfTexGetTexelBitCount(format);
    do {
        pixels = sdfTexTransData(destination, width, height, format, pixels, uploadOwnership);
        levels--;
        destination += (bits * width * height) >> 5;
        width >>= 1;
        height >>= 1;
    } while (levels != -1);
    sdfTexMakeGIFPacket(texture);
    return texture;
}

/* Build the texture and packet using queue-owned copies of the input pixels. */
sdfTexHandle_t *sdfTexSet(const SdfTextureFileHeader *header) {
    return sdfTexSetEx(header, SDF_TEXTURE_UPLOAD_COPY);
}

/* Build the texture and packet while borrowing input pixels through transfer. */
sdfTexHandle_t *sdfTexSetDirect(const SdfTextureFileHeader *header) {
    return sdfTexSetEx(header, SDF_TEXTURE_UPLOAD_BORROW);
}

/* Build weighted-RGB intensity bytes from retained palette color data. */
void sdfTexSetMonoClut(sdfTexHandle_t *texture) {
    s32 stride;
    s32 paletteColorCount;
    u8 *output;
    u8 *source;
    u32 color;
    u32 first;
    u32 second;
    u32 third;

    if (texture->clutFormat == 0) {
        stride = 4;
        paletteColorCount = (u32)texture->paletteDataSize >> 2;
    } else {
        stride = 2;
        paletteColorCount = (u32)texture->paletteDataSize >> 1;
    }
    output = texture->intensityMap;
    if (output == NULL) {
        output = sdfAllocAuto(paletteColorCount);
        texture->intensityMap = output;
    }
    source = texture->paletteData;
    do {
        if (stride == 4) {
            color = *(u32 *)source;
            source += 4;
        } else {
            u16 packed = *(u16 *)source;
            source += 2;
            EE_MMI_PEXT5(color, packed);
            color |= (color >> 5) & 0x070707;
        }
        second = (color >> 8) & 0xFF;
        first = color & 0xFF;
        third = (color >> 16) & 0xFF;
        paletteColorCount--;
        *output = ((first * 77) >> 8) + ((second * 150) >> 8) + ((third * 29) >> 8);
        output++;
    } while (paletteColorCount != 0);
}

extern void sdfTexSetMonoClut(sdfTexHandle_t *texture);

extern void sdfTexClutToGs(sdfTexHandle_t *texture);

/* vu0 routine: blend clone-source palette RGB toward intensity-scaled tint, preserving alpha. */
void sdfTexSetClutColor(sdfTexHandle_t *texture, s32 tint, f32 amount) {
    sdfTexHandle_t *source = texture->reference->cloneSource;
    u32 count;
    u8 *intensity;

    intensity = source->intensityMap;
    if (intensity == NULL) {
        sdfTexSetMonoClut(source);
        intensity = source->intensityMap;
    }

    /* Move the FPU scalars into VU0, then broadcast tint and its scale. */
    EE_MMI_PALETTE_TINT_SETUP(amount, tint, 1.0f / 255.0f);

    if (texture->clutFormat == 0) {
        u32 *sourcePalette = (u32 *)source->paletteData;
        u32 *destinationPalette = (u32 *)texture->paletteData;
        count = (u32)texture->paletteDataSize >> 2;
        do {
            u32 value = *intensity;
            u32 color = *sourcePalette;

            EE_MMI_PALETTE_TINT_RGBA(value, color);
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
        count = (u32)texture->paletteDataSize >> 1;
        sourcePalette = (u16 *)source->paletteData;
        destinationPalette = (u16 *)texture->paletteData;
        do {
            u32 value = *intensity;
            u16 color = *sourcePalette;

            EE_MMI_PALETTE_TINT_RGB5(color, value, fiveBitExpandMask);
            count--;
            *destinationPalette = color;
            sourcePalette++;
            intensity++;
            destinationPalette++;
        } while (count != 0);
    }
    sdfTexClutToGs(texture);
}

/* Borrow a matching live texture by key without changing its reference count. */
sdfTexHandle_t *sdfTexSearch(s32 resourceKey) {
    sdfTexHandle_t *texture = sdfTexHandleLink;

    while (texture != NULL) {
        if (texture->resourceKey == resourceKey) {
            return texture;
        }
        texture = texture->prev;
    }
    return NULL;
}

void sdfTexInit(void) {
    sdfDelayedResrcInitCB(&sdfTexDelayedResrcCB, (SdfPendingEntryCallback)sdfTexDestroy);
}

INCLUDE_SDATA(const s32, "sdf/sdfTex", sdfTexHandleLink);

