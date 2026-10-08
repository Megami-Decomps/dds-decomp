#include "common.h"
#include "sdf_texture_file.h"
#include "sdf.h"
#include "ee_mmi.h"

/* PlayStation 2 GS pixel storage formats used to size indexed palettes. */
enum { SDF_PSMCT32 = 0, SDF_PSMT8 = 0x13, SDF_PSMT8H = 0x1B };

extern SdfTex *sdfResourceListHead;
extern u8 sdfTextureReleaseQueue;

void *sdfAllocateBlockBySizeThreshold(s32 arg0);
void *memcpy(void *arg0, void *arg1, u32 arg2);
void sdfUpdateTextureHeadsWithInterruptsMasked(void *arg0);
void sdfReleaseChipBlock(void *arg0);
void sdfFreeMemoryFromEitherHeap(void *arg0);
void sdfPendingQueuePush(void *arg0, void *arg1);
SdfTexBuf *sdfTexCreateResourcePacket(SdfTex *texture, s32 variant);
SdfTex *sdfTexCreateFromFileHeader(SdfTextureFileHeader *header, s32 mode);
void *sdfAllocSizeClassBlock(s32 arg0);
u32 sdfTexGetPrimaryResourceWord(SdfTex *texture);
u32 sdfTexGetSecondaryResourceWord(SdfTex *texture);
void sdfBuildTextureStatePacket(SdfTexBuf *packet, s32 width, s32 height,
    u32 primaryResourceWord, s32 pixelFormat, u32 secondaryResourceWord,
    s32 clutFormat, s32 textureColorComponents, s32 maxMipLevel,
    s32 lodParameters, s32 clampMode, s32 variant);
/* Return palette bytes: 8-bit indices use 256 colors, other indices 16. */
s32 sdfTexGetPaletteByteSize(s32 textureFormat, s32 paletteFormat, s32 paletteCount) {
    s32 bytesPerColor = (paletteFormat == SDF_PSMCT32) ? 4 : 2;
    s32 colorsPerPalette;

    if ((textureFormat == SDF_PSMT8) || (textureFormat == SDF_PSMT8H)) {
        colorsPerPalette = 0x100;
    } else {
        colorsPerPalette = 0x10;
    }
    return colorsPerPalette * bytesPerColor * paletteCount;
}

INCLUDE_ASM(const s32, "sdf/sdfTex", func_002D2A58);

/* Copy retained palette bytes without changing the texture resource metadata. */
void *sdfTexCopyImageData(SdfTex *texture, void *source) {
    return memcpy(texture->paletteData, source, texture->paletteDataSize);
}

INCLUDE_ASM(const s32, "sdf/sdfTex", sdfTexCreateWithAllocatedResources);

/* Release both GPU resources and unlink the texture from the active list. */
void sdfTexRelease(SdfTex *texture) {
    SdfTex *next;
    SdfTex *prev;

    if (texture->reference->cloneSource == NULL) {
        sdfUpdateTextureHeadsWithInterruptsMasked(texture->primaryResource);
    }
    sdfUpdateTextureHeadsWithInterruptsMasked(texture->secondaryResource);
    sdfReleaseChipBlock(texture->primaryBuffer);
    sdfReleaseChipBlock(texture->secondaryBuffer);
    next = texture->next;
    prev = texture->prev;
    if (prev != NULL) {
        prev->next = next;
    }
    if (next != NULL) {
        next->prev = prev;
    } else {
        sdfResourceListHead = prev;
    }
    sdfFreeMemoryFromEitherHeap(texture->paletteData);
    sdfFreeMemoryFromEitherHeap(texture->intensityMap);
    sdfReleaseChipBlock(texture->reference);
    sdfReleaseChipBlock(texture);
}

/* Release a texture when the final ordinary reference is dropped. */
void sdfTexReleaseReference(SdfTex *texture) {
    SdfTexRef *ref;
    s32 count;

    if (texture != NULL) {
        ref = texture->reference;
        count = ref->refCount - 1;
        ref->refCount = count;
        if (count == 0) {
            texture->resourceKey = 0;
            sdfTexRelease(texture);
        }
    }
}

/* Hand the last reference to sdfPendingQueuePush rather than releasing it here. */
void sdfTexReleaseReferenceViaHandler(SdfTex *texture) {
    SdfTexRef *ref;
    s32 count;

    if (texture != NULL) {
        ref = texture->reference;
        count = ref->refCount - 1;
        ref->refCount = count;
        if (count == 0) {
            texture->resourceKey = 0;
            sdfPendingQueuePush(&sdfTextureReleaseQueue, texture);
        }
    }
}

INCLUDE_ASM(const s32, "sdf/sdfTex", sdfBuildTextureStatePacket);

/* Allocate and populate one of the two resource packet variants. */
SdfTexBuf *sdfTexCreateResourcePacket(SdfTex *texture, s32 variant) {
    SdfTexBuf *packet;
    u32 primary;
    u32 secondary;

    packet = sdfAllocSizeClassBlock(0x40);
    primary = sdfTexGetPrimaryResourceWord(texture);
    secondary = sdfTexGetSecondaryResourceWord(texture);
    sdfBuildTextureStatePacket(packet, texture->width, texture->height, primary, texture->pixelFormat, secondary, texture->clutFormat, 1, texture->maxMipLevel, texture->lodParameters, texture->clampMode, variant);
    return packet;
}

/* Store the first packet on the texture. */
void sdfTexCreateFirstPacket(SdfTex *texture) {
    texture->primaryBuffer = sdfTexCreateResourcePacket(texture, 0);
}

/* Store the second packet on the texture. */
void sdfTexCreateSecondPacket(SdfTex *texture) {
    texture->secondaryBuffer = sdfTexCreateResourcePacket(texture, 1);
}

/* Rebuild existing texture packets while preserving each packet's TEX0 TCC bit. */
void sdfTexRefreshResourcePackets(SdfTex *texture) {
    SdfTexBuf *buffer;

    buffer = texture->primaryBuffer;
    if (buffer != NULL) {
        sdfBuildTextureStatePacket(buffer, texture->width, texture->height,
                     sdfTexGetPrimaryResourceWord(texture), texture->pixelFormat,
                     sdfTexGetSecondaryResourceWord(texture), texture->clutFormat,
                     (buffer->textureState >> 34) & 1,
                     texture->maxMipLevel, texture->lodParameters,
                     texture->clampMode, 0);
    }

    buffer = texture->secondaryBuffer;
    if (buffer != NULL) {
        sdfBuildTextureStatePacket(buffer, texture->width, texture->height,
                     sdfTexGetPrimaryResourceWord(texture), texture->pixelFormat,
                     sdfTexGetSecondaryResourceWord(texture), texture->clutFormat,
                     (buffer->textureState >> 34) & 1,
                     texture->maxMipLevel, texture->lodParameters,
                     texture->clampMode, 1);
    }
}

extern SdfTex *sdfTexCreateWithAllocatedResources(s32, s32, u32, u32, u32, u32);
extern u8 sdfTexGetPaletteCount(SdfTex *);
extern void func_002D2A58(SdfTex *);
extern u8 *sdfTexSubmitPixelsForFormat(SdfTex *texture, u32 destination, u8 *pixels, s32 borrowPixels);
extern s32 sdfTexFormatSizeHint(s32);
extern u8 *sdfTexSubmitImageCopy(u32, s32, s32, u32, u8 *, s32);

SdfTex *sdfTexCreateFromFileHeader(SdfTextureFileHeader *header, s32 mode) {
    SdfTex *texture;
    u8 *pixels;
    s32 format;
    s32 width;
    s32 height;
    s32 levels;
    u32 destination;
    s32 bits;
    s32 key = header->resourceKey;

    if (key != 0) {
        SdfTex *existing = sdfResourceListHead;
        while (existing != NULL) {
            if (existing->resourceKey == key) {
                existing->reference->refCount++;
                return existing;
            }
            existing = existing->prev;
        }
    }
    texture = sdfTexCreateWithAllocatedResources(header->width, header->height, header->pixelFormat, header->clutFormat, header->unk11, header->unk10);
    texture->lodParameters = header->lodParameters;
    texture->unk1E = header->unk1A;
    texture->clampMode = header->clampMode;
    texture->resourceKey = header->resourceKey;
    texture->battleTextureSlot = header->unk20;
    pixels = (u8 *)header + (header->flags & 0xF0) + sizeof(*header);
    if (sdfTexGetPaletteCount(texture) != 0) {
        func_002D2A58(texture);
        sdfTexCopyImageData(texture, pixels);
        pixels = sdfTexSubmitPixelsForFormat(texture, sdfTexGetSecondaryResourceWord(texture), pixels, mode);
    }
    format = texture->pixelFormat;
    width = texture->width;
    height = texture->height;
    levels = texture->maxMipLevel;
    destination = sdfTexGetPrimaryResourceWord(texture);
    bits = sdfTexFormatSizeHint(format);
    do {
        pixels = sdfTexSubmitImageCopy(destination, width, height, format, pixels, mode);
        levels--;
        destination += (bits * width * height) >> 5;
        width >>= 1;
        height >>= 1;
    } while (levels != -1);
    sdfTexCreateFirstPacket(texture);
    return texture;
}

/* Build the texture and its packet with variant zero. */
SdfTex *sdfTexAcquireResourceTexture(void *resourceAddress) {
    return sdfTexCreateFromFileHeader(resourceAddress, 0);
}

/* Build the texture and its packet with variant one. */
SdfTex *sdfTexAcquireAlternateResourceTexture(void *resourceAddress) {
    return sdfTexCreateFromFileHeader(resourceAddress, 1);
}

/* Build weighted-RGB intensity bytes from retained palette color data. */
void sdfTexBuildIntensityMap(SdfTex *texture) {
    s32 stride;
    s32 count;
    u8 *output;
    u8 *source;
    u32 color;
    u32 first;
    u32 second;
    u32 third;

    if (texture->clutFormat == 0) {
        stride = 4;
        count = (u32)texture->paletteDataSize >> 2;
    } else {
        stride = 2;
        count = (u32)texture->paletteDataSize >> 1;
    }
    output = texture->intensityMap;
    if (output == NULL) {
        output = sdfAllocateBlockBySizeThreshold(count);
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
        count--;
        *output = ((first * 77) >> 8) + ((second * 150) >> 8) + ((third * 29) >> 8);
        output++;
    } while (count != 0);
}
