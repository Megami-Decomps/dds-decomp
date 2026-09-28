#include "common.h"

extern u8 D_004389E0;

extern u32 D_004389E4;

extern u32 D_004389E8;

extern u32 D_00439140;

extern u32 D_00439144;

typedef struct SdfSemaObj {
    s32 unk0; /* 0x0: semaphore id */
    void *unk4; /* 0x4 */
    void *unk8; /* 0x8 */
    void *unkC; /* 0xC */
    s32 unk10; /* 0x10 */
} SdfSemaObj;

typedef struct SdfTexRef {
    void *unk0; /* 0x0 */
    s32 unk4; /* 0x4 */
} SdfTexRef;

typedef struct SdfTexBuf {
    s32 unk0; /* 0x0 */
    u8 pad4[0xC]; /* 0x4 */
    u64 unk10; /* 0x10 */
    u64 unk18; /* 0x18 */
    u64 unk20; /* 0x20 */
    u64 unk28; /* 0x28 */
    u64 unk30; /* 0x30 */
} SdfTexBuf;

typedef struct SdfTex {
    struct SdfTex *unk0; /* 0x0 */
    struct SdfTex *unk4; /* 0x4 */
    SdfTexRef *unk8; /* 0x8 */
    s16 unkC; /* 0xC */
    s16 unkE; /* 0xE */
    void *unk10; /* 0x10 */
    void *unk14; /* 0x14 */
    u8 unk18; /* 0x18 */
    u8 unk19; /* 0x19 */
    u8 unk1A; /* 0x1A */
    u8 unk1B; /* 0x1B */
    u16 unk1C; /* 0x1C */
    u8 unk1E; /* 0x1E */
    u8 unk1F; /* 0x1F */
    s32 unk20; /* 0x20 */
    s32 unk24; /* 0x24 */
    SdfTexBuf *unk28; /* 0x28 */
    SdfTexBuf *unk2C; /* 0x2C */
    void *unk30; /* 0x30 */
    s32 unk34; /* 0x34 */
    s32 unk38; /* 0x38 */
    void *unk3C; /* 0x3C */
} SdfTex;

extern SdfTex *D_004389F8;

void func_00329F60(u32 arg0, u32 arg1) {
    D_004389E4 = arg0;
    D_004389E8 = arg1;
    D_004389E0 = 1;
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_00329F78);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A1C8);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A200);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A230);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A378);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A440);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A548);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A5A0);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A5F0);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A648);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A688);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A7A8);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A8C8);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A968);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A9D8);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032AA40);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032AAB8);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032AAD8);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032AB20);

u32 func_0032ABC0(void) {
    return D_00439140;
}

u32 func_0032ABC8(void) {
    return D_00439144;
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032ABD0);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032AC30);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032AEA0);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032AF20);

void func_0032AFD8(SdfSemaObj *arg0) {
    arg0->unk4 = NULL;
    arg0->unk8 = NULL;
    arg0->unkC = NULL;
    arg0->unk10 = 0;
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032AFF0);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B018);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B170);

u32 func_0032B1B0(s32 arg0) {
    return *(u32 *)(arg0 + 0x28);
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B1B8);

s32 func_0032B1E0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x2c);
    if (temp_v0 == 0) {
        func_0032BE30();
        temp_v0 = *(s32 *)(arg0 + 0x2c);
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B218);

u8 func_0032B240(s32 arg0) {
    return *(u8 *)(arg0 + 0x18);
}

u32 func_0032B248(s32 arg0) {
    u32 temp_v0;

    temp_v0 = 0;
    if (*(s32 *)(arg0 + 0x14) != 0) {
        temp_v0 = *(u32 *)(*(s32 *)(arg0 + 0x14) + 0xc);
    }
    return temp_v0;
}

u32 func_0032B260(s32 arg0) {
    return *(u32 *)(*(s32 *)(arg0 + 0x10) + 0xc);
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B270);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B2C0);

u64 func_0032B318(s32 arg0) {
    return *(u64 *)(*(s32 *)(arg0 + 0x28) + 0x20);
}

u64 func_0032B328(s32 arg0) {
    return *(u64 *)(*(s32 *)(arg0 + 0x28) + 0x10);
}

u64 func_0032B338(s32 arg0) {
    return *(u64 *)(*(s32 *)(arg0 + 0x28) + 0x30);
}

void func_0032B348(SdfTex *arg0, s32 arg1, s32 arg2) {
    SdfTexBuf *buf;

    buf = arg0->unk28;
    buf->unk10 = (buf->unk10 & ~0x1E0) | (arg1 << 5) | (arg2 << 6);
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B370);

void func_0032B3E0(s32 arg0, u8 arg1) {
    *(u8 *)(arg0 + 0x1f) = arg1;
    func_0032BE60();
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B3F8);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B500);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B558);

void sdfTexListInsert(SdfTex *arg0) {
    arg0->unk0 = NULL;
    if (D_004389F8 != NULL) {
        arg0->unk4 = D_004389F8;
        D_004389F8->unk0 = arg0;
    } else {
        arg0->unk4 = NULL;
    }
    D_004389F8 = arg0;
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B5D8);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B6B0);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B800);

INCLUDE_SDATA(const s32, "game/code_00329F60", D_004389E0);

INCLUDE_SDATA(const s32, "game/code_00329F60", D_004389E1);

INCLUDE_SDATA(const s32, "game/code_00329F60", D_004389E4);

INCLUDE_SDATA(const s32, "game/code_00329F60", D_004389E8);

INCLUDE_SDATA(const s32, "game/code_00329F60", D_004389EC);

INCLUDE_SDATA(const s32, "game/code_00329F60", D_004389F0);

INCLUDE_SDATA(const s32, "game/code_00329F60", D_004389F2);

INCLUDE_SDATA(const s32, "game/code_00329F60", D_004389F4);

INCLUDE_SDATA(const s32, "game/code_00329F60", D_004389F8);

