#include "common.h"
#include "sdf.h"

extern SdfTex *D_003BD308;
extern u8 D_003BD9F0;

void *func_002CF530(s32 arg0);
void *memcpy(void *arg0, void *arg1, u32 arg2);
void func_002D1B90(void *arg0);
void func_002CFF98(void *arg0);
void func_002CF570(void *arg0);
void func_002D3C30(void *arg0, void *arg1);
void *func_002D2EB0(SdfTex *arg0, s32 arg1);
void *func_002D30C8(void *arg0, s32 arg1);
void *func_002CFEB8(s32 arg0);
void *sdfTexGetPrimaryResourceWord();
void *sdfTexGetSecondaryResourceWord(void *arg0);
void func_002D2D48(void *arg0, s32 arg1, s32 arg2, void *arg3, s32 arg4, void *arg5, s32 arg6, s64 arg7, s32 arg8, s32 arg9, s32 arg10, s32 arg11);
s32 func_002D2A28(s32 textureFormat, s32 paletteFormat, s32 paletteCount) {
    s32 bytesPerColor = (paletteFormat == 0) ? 4 : 2;
    s32 colorsPerPalette;

    if ((textureFormat == 0x13) || (textureFormat == 0x1B)) {
        colorsPerPalette = 0x100;
    } else {
        colorsPerPalette = 0x10;
    }
    return colorsPerPalette * bytesPerColor * paletteCount;
}

INCLUDE_ASM(const s32, "sdf/sdfTex", func_002D2A58);

void *func_002D2A98(SdfTex *texture, void *source) {
    return memcpy(texture->data, source, texture->dataSize);
}

INCLUDE_ASM(const s32, "sdf/sdfTex", func_002D2AB8);

void sdfTexRelease(SdfTex *texture) {
    SdfTex *next;
    SdfTex *prev;

    if (texture->reference->unk0 == NULL) {
        func_002D1B90(texture->primaryResource);
    }
    func_002D1B90(texture->secondaryResource);
    func_002CFF98(texture->unk28);
    func_002CFF98(texture->unk2C);
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
    func_002CF570(texture->data);
    func_002CF570(texture->unk3C);
    func_002CFF98(texture->reference);
    func_002CFF98(texture);
}

void func_002D2CB8(SdfTex *texture) {
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

void func_002D2D00(SdfTex *texture) {
    SdfTexRef *ref;
    s32 count;

    if (texture != NULL) {
        ref = texture->reference;
        count = ref->refCount - 1;
        ref->refCount = count;
        if (count == 0) {
            texture->unk20 = 0;
            func_002D3C30(&D_003BD9F0, texture);
        }
    }
}

INCLUDE_ASM(const s32, "sdf/sdfTex", func_002D2D48);

void *func_002D2EB0(SdfTex *texture, s32 variant) {
    void *packet;
    void *primary;
    void *secondary;

    packet = func_002CFEB8(0x40);
    primary = sdfTexGetPrimaryResourceWord(texture);
    secondary = sdfTexGetSecondaryResourceWord(texture);
    func_002D2D48(packet, texture->unkC, texture->unkE, primary, texture->unk1A, secondary, texture->unk19, 1, texture->unk1B, texture->unk1C, texture->unk1F, variant);
    return packet;
}

void func_002D2F50(SdfTex *texture) {
    texture->unk28 = func_002D2EB0(texture, 0);
}

void func_002D2F80(SdfTex *texture) {
    texture->unk2C = func_002D2EB0(texture, 1);
}

INCLUDE_ASM(const s32, "sdf/sdfTex", func_002D2FB0);

INCLUDE_ASM(const s32, "sdf/sdfTex", func_002D30C8);

void func_002D3288(void *arg0) {
    func_002D30C8(arg0, 0);
}

void func_002D32A0(void *arg0) {
    func_002D30C8(arg0, 1);
}

INCLUDE_ASM(const s32, "sdf/sdfTex", func_002D32B8);
