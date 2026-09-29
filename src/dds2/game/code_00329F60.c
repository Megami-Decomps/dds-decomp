#include "common.h"

#include "sdf.h"

extern u8 D_004389E0;

extern u32 D_004389E4;

extern u32 D_004389E8;

extern SdfTex *D_004389F8;

extern u8 D_00439148;

void func_0032CAE0(void *arg0, s32 arg1);

typedef struct SdfTexHead {
    SdfTex *unk0; /* 0x0 */
    SdfTex *unk4; /* 0x4 */
    void *unk8; /* 0x8 */
    void *unkC; /* 0xC */
    s32 unk10; /* 0x10 */
    void *unk14; /* 0x14 */
    void *unk18; /* 0x18 */
} SdfTexHead;

extern SdfTexHead *D_00439144;

extern SdfTexHead *D_00439140;

void *func_00328D68(s32 size);

void func_0032AA40(void *arg0);

void func_0032CA90(void *arg0, void (*arg1)(void *));

extern SdfSemaObj D_004681F8;

s32 sdfCreateSemaphore(s32 arg0, s32 arg1, s32 arg2);

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

void func_0032AAB8(s32 arg0) {
    func_0032CAE0(&D_00439148, arg0);
}

void sdfTexInitializeLists(void) {
    SdfTexHead *head;

    head = func_00328D68(0x1C);
    head->unk10 = 0x100000;
    head->unk0 = NULL;
    head->unk4 = NULL;
    head->unk8 = NULL;
    head->unkC = NULL;
    D_00439140 = head;
    D_00439144 = head;
    func_0032CA90(&D_00439148, func_0032AA40);
}

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

void func_0032AFD8(SdfSemaObj *semaphore) {
    semaphore->unk4 = NULL;
    semaphore->unk8 = NULL;
    semaphore->unkC = NULL;
    semaphore->unk10 = 0;
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032AFF0);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B018);

void sdfTexInitializeSemaphore(void) {
    SdfSemaObj *obj;

    obj = &D_004681F8;
    obj->unk0 = sdfCreateSemaphore(1, 0x7F, 0);
    func_0032AFD8(obj);
}

u32 func_0032B1B0(s32 arg0) {
    return *(u32 *)(arg0 + 0x28);
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B1B8);

s32 sdfTexGetOrInitializeSecondaryBuffer(SdfTex *texture) {
    SdfTexBuf *buffer;

    buffer = texture->unk2C;
    if (buffer == NULL) {
        func_0032BE30();
        buffer = texture->unk2C;
    }
    return (s32)buffer;
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B218);

u8 func_0032B240(s32 arg0) {
    return *(u8 *)(arg0 + 0x18);
}

u32 sdfTexGetSecondaryResourceWord(SdfTex *texture) {
    u32 word;

    word = 0;
    if (texture->secondaryResource != NULL) {
        word = texture->secondaryResource->word;
    }
    return word;
}

u32 sdfTexGetPrimaryResourceWord(SdfTex *texture) {
    return texture->primaryResource->word;
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B270);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B2C0);

u64 func_0032B318(SdfTex *texture) {
    return texture->unk28->unk20;
}

u64 func_0032B328(SdfTex *texture) {
    return texture->unk28->unk10;
}

u64 func_0032B338(SdfTex *texture) {
    return texture->unk28->unk30;
}

void func_0032B348(SdfTex *arg0, s32 arg1, s32 arg2) {
    SdfTexBuf *buf;

    buf = arg0->unk28;
    buf->unk10 = (buf->unk10 & ~0x1E0) | (arg1 << 5) | (arg2 << 6);
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B370);

void func_0032B3E0(SdfTex *texture, u8 value) {
    texture->unk1F = value;
    func_0032BE60();
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B3F8);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B500);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B558);

void sdfTexListInsert(SdfTex *arg0) {
    arg0->next = NULL;
    if (D_004389F8 != NULL) {
        arg0->prev = D_004389F8;
        D_004389F8->next = arg0;
    } else {
        arg0->prev = NULL;
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
