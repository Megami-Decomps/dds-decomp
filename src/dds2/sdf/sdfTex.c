#include "common.h"

#include "sdf.h"

extern SdfTex *D_004389F8;

void func_0032AA40(void *arg0);

void func_00328E48(void *arg0);

void func_00328420(void *arg0);

void *func_0032BD60(SdfTex *arg0, s32 arg1);

void *func_00328D68(s32 arg0);

void *sdfTexGetPrimaryResourceWord();

void *sdfTexGetSecondaryResourceWord(void *arg0);

void func_0032BBF8(void *arg0, s32 arg1, s32 arg2, void *arg3, s32 arg4, void *arg5, s32 arg6, s64 arg7, s32 arg8, s32 arg9, s32 arg10, s32 arg11);

extern u8 D_00439150;

void func_0032CAE0(void *arg0, void *arg1);

s32 func_0032B8D8(s32 textureFormat, s32 paletteFormat, s32 paletteCount) {
    s32 bytesPerColor = (paletteFormat == 0) ? 4 : 2;
    s32 colorsPerPalette;

    if ((textureFormat == 0x13) || (textureFormat == 0x1B)) {
        colorsPerPalette = 0x100;
    } else {
        colorsPerPalette = 0x10;
    }
    return colorsPerPalette * bytesPerColor * paletteCount;
}

INCLUDE_ASM(const s32, "sdf/sdfTex", func_0032B908);

void func_0032B948(SdfTex *texture, void *source) {
    memcpy(texture->data, source, texture->dataSize);
}

INCLUDE_ASM(const s32, "sdf/sdfTex", func_0032B968);

void sdfTexRelease(SdfTex *texture) {
    SdfTex *next;
    SdfTex *prev;

    if (texture->reference->unk0 == NULL) {
        func_0032AA40(texture->primaryResource);
    }
    func_0032AA40(texture->secondaryResource);
    func_00328E48(texture->unk28);
    func_00328E48(texture->unk2C);
    next = texture->next;
    prev = texture->prev;
    if (prev != NULL) {
        prev->next = next;
    }
    if (next != NULL) {
        next->prev = prev;
    } else {
        D_004389F8 = prev;
    }
    func_00328420(texture->data);
    func_00328420(texture->unk3C);
    func_00328E48(texture->reference);
    func_00328E48(texture);
}

void func_0032BB68(SdfTex *texture) {
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

void func_0032BBB0(SdfTex *texture) {
    SdfTexRef *ref;
    s32 count;

    if (texture != NULL) {
        ref = texture->reference;
        count = ref->refCount - 1;
        ref->refCount = count;
        if (count == 0) {
            texture->unk20 = 0;
            func_0032CAE0(&D_00439150, texture);
        }
    }
}

INCLUDE_ASM(const s32, "sdf/sdfTex", func_0032BBF8);

void *func_0032BD60(SdfTex *texture, s32 variant) {
    void *packet;
    void *primary;
    void *secondary;

    packet = func_00328D68(0x40);
    primary = sdfTexGetPrimaryResourceWord(texture);
    secondary = sdfTexGetSecondaryResourceWord(texture);
    func_0032BBF8(packet, texture->unkC, texture->unkE, primary, texture->unk1A, secondary, texture->unk19, 1, texture->unk1B, texture->unk1C, texture->unk1F, variant);
    return packet;
}

void func_0032BE00(SdfTex *texture) {
    texture->unk28 = func_0032BD60(texture, 0);
}

void func_0032BE30(SdfTex *texture) {
    texture->unk2C = func_0032BD60(texture, 1);
}

INCLUDE_ASM(const s32, "sdf/sdfTex", func_0032BE60);

INCLUDE_ASM(const s32, "sdf/sdfTex", func_0032BF78);

void func_0032C138(u32 arg0) {
    func_0032BF78(arg0, 0);
}

void func_0032C150(u32 arg0) {
    func_0032BF78(arg0, 1);
}

INCLUDE_ASM(const s32, "sdf/sdfTex", func_0032C168);
