#include "common.h"

typedef struct SdfTexRef {
    void *unk0; /* 0x0 */
    s32 refCount; /* 0x4 */
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

typedef struct {
    u8 pad00[0xC];
    u32 word; /* 0x0C */
} SdfTexResource;

typedef struct SdfTex {
    struct SdfTex *next; /* 0x0 */
    struct SdfTex *prev; /* 0x4 */
    SdfTexRef *reference; /* 0x8 */
    s16 unkC; /* 0xC */
    s16 unkE; /* 0xE */
    SdfTexResource *primaryResource; /* 0x10 */
    SdfTexResource *secondaryResource; /* 0x14 */
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

typedef struct SdfTexHead {
    SdfTex *unk0; /* 0x0 */
    SdfTex *unk4; /* 0x4 */
    void *unk8; /* 0x8 */
    void *unkC; /* 0xC */
    s32 unk10; /* 0x10 */
    void *unk14; /* 0x14 */
    void *unk18; /* 0x18 */
} SdfTexHead;

typedef struct SdfSemaObj {
    s32 unk0; /* 0x0: semaphore id */
    void *unk4; /* 0x4 */
    void *unk8; /* 0x8 */
    void *unkC; /* 0xC */
    s32 unk10; /* 0x10 */
} SdfSemaObj;

extern SdfTexHead *D_003BD9E4;
extern SdfTexHead *D_003BD9E0;
extern s8 D_003BD300[2];

extern s8 D_003BD302;
extern SdfTex *D_003BD308;
extern u8 D_003BD9E8;
extern SdfSemaObj D_003EB848;
extern u8 D_003BD2F0;
extern u32 D_003BD2F4;
extern u32 D_003BD2F8;

void *func_002CFEB8(s32 size);
s32 createSemaphore(s32 arg0, s32 arg1, s32 arg2);
void func_002D1B90(void *arg0);
void func_002D2F80(void);
void func_002D2FB0(void);
void func_002D3C30(void *arg0, s32 arg1);
void func_002D3BE0(void *arg0, void (*arg1)(void *));
void func_002D10B0(u32 arg0, u32 arg1) {
    D_003BD2F4 = arg0;
    D_003BD2F8 = arg1;
    D_003BD2F0 = 1;
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D10C8);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1318);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1350);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1380);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D14C8);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1590);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1698);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D16F0);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1740);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1798);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D17D8);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D18F8);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1A18);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1AB8);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1B28);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1B90);

void func_002D1C08(s32 arg0) {
    func_002D3C30(&D_003BD9E8, arg0);
}

void func_002D1C28(void) {
    SdfTexHead *head;

    head = func_002CFEB8(0x1C);
    head->unk10 = 0x100000;
    head->unk0 = NULL;
    head->unk4 = NULL;
    head->unk8 = NULL;
    head->unkC = NULL;
    D_003BD9E0 = head;
    D_003BD9E4 = head;
    func_002D3BE0(&D_003BD9E8, func_002D1B90);
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1C70);

SdfTexHead *func_002D1D10(void) {
    return D_003BD9E0;
}

SdfTexHead *func_002D1D18(void) {
    return D_003BD9E4;
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1D20);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1D80);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1FF0);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2070);

void func_002D2128(SdfSemaObj *arg0) {
    arg0->unk4 = NULL;
    arg0->unk8 = NULL;
    arg0->unkC = NULL;
    arg0->unk10 = 0;
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2140);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2168);

void func_002D22C0(void) {
    SdfSemaObj *obj;

    obj = &D_003EB848;
    obj->unk0 = createSemaphore(1, 0x7F, 0);
    func_002D2128(obj);
}

u32 func_002D2300(SdfTex *arg0) {
    return (u32)arg0->unk28;
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2308);

s32 func_002D2330(SdfTex *arg0) {
    SdfTexBuf *buf;

    buf = arg0->unk2C;
    if (buf == NULL) {
        func_002D2F80();
        buf = arg0->unk2C;
    }
    return (s32)buf;
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2368);

u8 func_002D2390(SdfTex *arg0) {
    return arg0->unk18;
}

u32 func_002D2398(SdfTex *texture) {
    u32 val;

    val = 0;
    if (texture->secondaryResource != NULL) {
        val = texture->secondaryResource->word;
    }
    return val;
}

u32 func_002D23B0(SdfTex *texture) {
    return texture->primaryResource->word;
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D23C0);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2410);

u64 func_002D2468(SdfTex *arg0) {
    return arg0->unk28->unk20;
}

u64 func_002D2478(SdfTex *arg0) {
    return arg0->unk28->unk10;
}

u64 func_002D2488(SdfTex *arg0) {
    return arg0->unk28->unk30;
}

void func_002D2498(SdfTex *arg0, s32 arg1, s32 arg2) {
    SdfTexBuf *buf;

    buf = arg0->unk28;
    buf->unk10 = (buf->unk10 & ~0x1E0) | (arg1 << 5) | (arg2 << 6);
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D24C0);

void func_002D2530(SdfTex *arg0, u8 arg1) {
    arg0->unk1F = arg1;
    func_002D2FB0();
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2548);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2650);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D26A8);

void sdfTexListInsert(SdfTex *texture) {
    texture->next = NULL;
    if (D_003BD308 != NULL) {
        texture->prev = D_003BD308;
        D_003BD308->next = texture;
    } else {
        texture->prev = NULL;
    }
    D_003BD308 = texture;
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2728);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2800);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2950);

INCLUDE_SDATA(const s32, "game/code_002D10B0", D_003BD2F0);

INCLUDE_SDATA(const s32, "game/code_002D10B0", D_003BD2F1);

INCLUDE_SDATA(const s32, "game/code_002D10B0", D_003BD2F4);

INCLUDE_SDATA(const s32, "game/code_002D10B0", D_003BD2F8);

INCLUDE_SDATA(const s32, "game/code_002D10B0", D_003BD2FC);

INCLUDE_SDATA(const s32, "game/code_002D10B0", D_003BD300);

INCLUDE_SDATA(const s32, "game/code_002D10B0", D_003BD302);

INCLUDE_SDATA(const s32, "game/code_002D10B0", D_003BD304);

INCLUDE_SDATA(const s32, "game/code_002D10B0", D_003BD308);

