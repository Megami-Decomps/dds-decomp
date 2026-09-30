#include "common.h"
#include "sdf.h"

/* PlayStation 2 GS pixel storage formats used to size indexed palettes. */
enum { SDF_PSMCT32 = 0, SDF_PSMT8 = 0x13, SDF_PSMT8H = 0x1B };

extern SdfTex *D_003BD308;
extern u8 D_003BD9F0;

void *func_002CF530(s32 arg0);
void *memcpy(void *arg0, void *arg1, u32 arg2);
void func_002D1B90(void *arg0);
void sdfReleaseChipBlock(void *arg0);
void sdfFreeMemoryFromEitherHeap(void *arg0);
void sdfPendingQueuePush(void *arg0, void *arg1);
void *sdfTexCreateResourcePacket(SdfTex *arg0, s32 arg1);
void *func_002D30C8(void *arg0, s32 arg1);
void *func_002CFEB8(s32 arg0);
void *sdfTexGetPrimaryResourceWord();
void *sdfTexGetSecondaryResourceWord(void *arg0);
void func_002D2D48(void *arg0, s32 arg1, s32 arg2, void *arg3, s32 arg4, void *arg5, s32 arg6, s64 arg7, s32 arg8, s32 arg9, s32 arg10, s32 arg11);
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

/* Copy the texture's backing image without changing the resource metadata. */
void *sdfTexCopyImageData(SdfTex *texture, void *source) {
    return memcpy(texture->data, source, texture->dataSize);
}

INCLUDE_ASM(const s32, "sdf/sdfTex", func_002D2AB8);

/* Release both GPU resources and unlink the texture from the active list. */
void sdfTexRelease(SdfTex *texture) {
    SdfTex *next;
    SdfTex *prev;

    if (texture->reference->unk0 == NULL) {
        func_002D1B90(texture->primaryResource);
    }
    func_002D1B90(texture->secondaryResource);
    sdfReleaseChipBlock(texture->unk28);
    sdfReleaseChipBlock(texture->unk2C);
    next = texture->next;
    prev = texture->prev;
    if (prev != NULL) {
        prev->next = next;
    }
    if (next != NULL) {
        next->prev = prev;
    } else {
        D_003BD308 = prev;
    }
    sdfFreeMemoryFromEitherHeap(texture->data);
    sdfFreeMemoryFromEitherHeap(texture->unk3C);
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
            sdfPendingQueuePush(&D_003BD9F0, texture);
        }
    }
}

INCLUDE_ASM(const s32, "sdf/sdfTex", func_002D2D48);

/* Allocate and populate one of the two resource packet variants. */
void *sdfTexCreateResourcePacket(SdfTex *texture, s32 variant) {
    void *packet;
    void *primary;
    void *secondary;

    packet = func_002CFEB8(0x40);
    primary = sdfTexGetPrimaryResourceWord(texture);
    secondary = sdfTexGetSecondaryResourceWord(texture);
    func_002D2D48(packet, texture->unkC, texture->unkE, primary, texture->unk1A, secondary, texture->unk19, 1, texture->unk1B, texture->unk1C, texture->unk1F, variant);
    return packet;
}

/* Store the first packet on the texture. */
void sdfTexCreateFirstPacket(SdfTex *texture) {
    texture->unk28 = sdfTexCreateResourcePacket(texture, 0);
}

/* Store the second packet on the texture. */
void sdfTexCreateSecondPacket(SdfTex *texture) {
    texture->unk2C = sdfTexCreateResourcePacket(texture, 1);
}

INCLUDE_ASM(const s32, "sdf/sdfTex", func_002D2FB0);

INCLUDE_ASM(const s32, "sdf/sdfTex", func_002D30C8);

/* Process a resource address with packet variant zero. */
void func_002D3288(void *resourceAddress) {
    func_002D30C8(resourceAddress, 0);
}

/* Process a resource address with packet variant one. */
void func_002D32A0(void *resourceAddress) {
    func_002D30C8(resourceAddress, 1);
}

INCLUDE_ASM(const s32, "sdf/sdfTex", func_002D32B8);
