#include "common.h"

typedef struct SdfTexRef {
    void *unk0;
    s32 unk4;
} SdfTexRef;

typedef struct SdfTexBuf {
    u8 pad[0x20];
    u64 unk20;
} SdfTexBuf;

typedef struct SdfTex {
    struct SdfTex *unk0;
    struct SdfTex *unk4;
    SdfTexRef *unk8;
    s16 unkC;
    s16 unkE;
    void *unk10;
    void *unk14;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u16 unk1C;
    u8 unk1E;
    u8 unk1F;
    s32 unk20;
    s32 unk24;
    SdfTexBuf *unk28;
    SdfTexBuf *unk2C;
    void *unk30;
    s32 unk34;
    s32 unk38;
    void *unk3C;
} SdfTex;

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
void *func_002D23B0();
void *func_002D2398(void *arg0);
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

void *func_002D2A98(SdfTex *arg0, void *arg1) {
    return memcpy(arg0->unk30, arg1, arg0->unk34);
}

INCLUDE_ASM(const s32, "sdf/sdfTex", func_002D2AB8);

void func_002D2C30(SdfTex *arg0) {
    SdfTex *cur;
    SdfTex *prev;

    if (arg0->unk8->unk0 == NULL) {
        func_002D1B90(arg0->unk10);
    }
    func_002D1B90(arg0->unk14);
    func_002CFF98(arg0->unk28);
    func_002CFF98(arg0->unk2C);
    cur = arg0->unk0;
    prev = arg0->unk4;
    if (prev != NULL) {
        prev->unk0 = cur;
    }
    if (cur != NULL) {
        cur->unk4 = prev;
    } else {
        D_003BD308 = prev;
    }
    func_002CF570(arg0->unk30);
    func_002CF570(arg0->unk3C);
    func_002CFF98(arg0->unk8);
    func_002CFF98(arg0);
}

void func_002D2CB8(SdfTex *arg0) {
    SdfTexRef *ref;
    s32 count;

    if (arg0 != NULL) {
        ref = arg0->unk8;
        count = ref->unk4 - 1;
        ref->unk4 = count;
        if (count == 0) {
            arg0->unk20 = 0;
            func_002D2C30(arg0);
        }
    }
}

void func_002D2D00(SdfTex *arg0) {
    SdfTexRef *ref;
    s32 count;

    if (arg0 != NULL) {
        ref = arg0->unk8;
        count = ref->unk4 - 1;
        ref->unk4 = count;
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
    tmp1 = func_002D23B0(arg0);
    tmp2 = func_002D2398(arg0);
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
