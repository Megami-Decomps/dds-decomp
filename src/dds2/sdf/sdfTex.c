#include "common.h"

#include "sdf.h"

/* PlayStation 2 GS pixel storage formats used to size indexed palettes. */
enum { SDF_PSMCT32 = 0, SDF_PSMT8 = 0x13, SDF_PSMT8H = 0x1B };

extern SdfTex *sdfResourceListHead;

void sdfUpdateTextureHeadsWithInterruptsMasked(void *arg0);

void sdfReleaseChipBlock(void *arg0);

void sdfFreeMemoryFromEitherHeap(void *arg0);

void *sdfTexCreateResourcePacket(SdfTex *arg0, s32 arg1);

void *func_00328D68(s32 arg0);

u32 sdfTexGetPrimaryResourceWord(SdfTex *texture);

u32 sdfTexGetSecondaryResourceWord(SdfTex *texture);

void func_0032BBF8(void *arg0, s32 arg1, s32 arg2, u32 arg3, s32 arg4, u32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10, s32 arg11);

extern u8 sdfTextureReleaseQueue;

void sdfPendingQueuePush(void *arg0, void *arg1);

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

INCLUDE_ASM(const s32, "sdf/sdfTex", func_0032B908);

/* Copy the texture's backing image without changing the resource metadata. */
void sdfTexCopyImageData(SdfTex *texture, void *source) {
    memcpy(texture->data, source, texture->dataSize);
}

INCLUDE_ASM(const s32, "sdf/sdfTex", func_0032B968);

/* Release both GPU resources and unlink the texture from the active list. */
void sdfTexRelease(SdfTex *texture) {
    SdfTex *next;
    SdfTex *prev;

    if (texture->reference->unk0 == NULL) {
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
    sdfFreeMemoryFromEitherHeap(texture->data);
    sdfFreeMemoryFromEitherHeap(texture->auxiliaryAllocation);
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
            texture->unk20 = 0;
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
            texture->unk20 = 0;
            sdfPendingQueuePush(&sdfTextureReleaseQueue, texture);
        }
    }
}

INCLUDE_ASM(const s32, "sdf/sdfTex", func_0032BBF8);

/* Allocate and populate one of the two resource packet variants. */
void *sdfTexCreateResourcePacket(SdfTex *texture, s32 variant) {
    void *packet;
    u32 primary;
    u32 secondary;

    packet = func_00328D68(0x40);
    primary = sdfTexGetPrimaryResourceWord(texture);
    secondary = sdfTexGetSecondaryResourceWord(texture);
    func_0032BBF8(packet, texture->width, texture->height, primary, texture->pixelFormat, secondary, texture->clutFormat, 1, texture->maxMipLevel, texture->lodParameters, texture->clampMode, variant);
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
void func_0032BE60(SdfTex *texture) {
    SdfTexBuf *buffer;

    buffer = texture->primaryBuffer;
    if (buffer != NULL) {
        func_0032BBF8(buffer, texture->width, texture->height,
                     sdfTexGetPrimaryResourceWord(texture), texture->pixelFormat,
                     sdfTexGetSecondaryResourceWord(texture), texture->clutFormat,
                     (buffer->textureState >> 34) & 1,
                     texture->maxMipLevel, texture->lodParameters,
                     texture->clampMode, 0);
    }

    buffer = texture->secondaryBuffer;
    if (buffer != NULL) {
        func_0032BBF8(buffer, texture->width, texture->height,
                     sdfTexGetPrimaryResourceWord(texture), texture->pixelFormat,
                     sdfTexGetSecondaryResourceWord(texture), texture->clutFormat,
                     (buffer->textureState >> 34) & 1,
                     texture->maxMipLevel, texture->lodParameters,
                     texture->clampMode, 1);
    }
}

INCLUDE_ASM(const s32, "sdf/sdfTex", func_0032BF78);

/* Process a resource address with packet variant zero. */
void sdfTexAcquireResourceTexture(u32 resourceAddress) {
    func_0032BF78(resourceAddress, 0);
}

/* Process a resource address with packet variant one. */
void func_0032C150(u32 resourceAddress) {
    func_0032BF78(resourceAddress, 1);
}

INCLUDE_ASM(const s32, "sdf/sdfTex", func_0032C168);
