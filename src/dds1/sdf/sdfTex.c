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
s32 func_002D2A28(s32 arg0, s32 arg1, s32 arg2) {
    s32 v1 = (arg1 == 0) ? 4 : 2;
    s32 v2;

    if ((arg0 == 0x13) || (arg0 == 0x1B)) {
        v2 = 0x100;
    } else {
        v2 = 0x10;
    }
    return v2 * v1 * arg2;
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

void func_002D2CB8(SdfTex *arg0) {
    SdfTexRef *ref;
    s32 count;

    if (arg0 != NULL) {
        ref = arg0->reference;
        count = ref->refCount - 1;
        ref->refCount = count;
        if (count == 0) {
            arg0->unk20 = 0;
            sdfTexRelease(arg0);
        }
    }
}

void func_002D2D00(SdfTex *arg0) {
    SdfTexRef *ref;
    s32 count;

    if (arg0 != NULL) {
        ref = arg0->reference;
        count = ref->refCount - 1;
        ref->refCount = count;
        if (count == 0) {
            arg0->unk20 = 0;
            func_002D3C30(&D_003BD9F0, arg0);
        }
    }
}

INCLUDE_ASM(const s32, "sdf/sdfTex", func_002D2D48);

void *func_002D2EB0(SdfTex *arg0, s32 arg1) {
    void *buf;
    void *tmp1;
    void *tmp2;

    buf = func_002CFEB8(0x40);
    tmp1 = sdfTexGetPrimaryResourceWord(arg0);
    tmp2 = sdfTexGetSecondaryResourceWord(arg0);
    func_002D2D48(buf, arg0->unkC, arg0->unkE, tmp1, arg0->unk1A, tmp2, arg0->unk19, 1, arg0->unk1B, arg0->unk1C, arg0->unk1F, arg1);
    return buf;
}

void func_002D2F50(SdfTex *arg0) {
    arg0->unk28 = func_002D2EB0(arg0, 0);
}

void func_002D2F80(SdfTex *arg0) {
    arg0->unk2C = func_002D2EB0(arg0, 1);
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
