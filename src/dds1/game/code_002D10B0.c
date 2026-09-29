#include "common.h"
#include "sdf.h"

typedef struct SdfTexHead {
    SdfTex *next; /* 0x0: SdfTex-compatible linked-list prefix */
    SdfTex *prev; /* 0x4 */
    void *unk8; /* 0x8 */
    void *unkC; /* 0xC */
    s32 unk10; /* 0x10 */
    void *unk14; /* 0x14 */
    void *unk18; /* 0x18 */
} SdfTexHead;

extern SdfTexHead *D_003BD9E4;
extern SdfTexHead *D_003BD9E0;
extern s8 D_003BD300[2];

extern volatile s8 D_003BD302;
extern SdfTex *D_003BD308;
extern u8 D_003BD9E8;
extern SdfSemaObj D_003EB848;
extern u8 D_003BD2F0;
extern u32 D_003BD2F4;
extern u32 D_003BD2F8;

void *func_002CFEB8(s32 size);
s32 sdfCreateSemaphore(s32 arg0, s32 arg1, s32 arg2);
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

void func_002D1318(s32 buffer, s32 next) {
    if (D_003BD300[0] == buffer) {
        D_003BD300[0] = buffer ^ 1;
    }
    if (D_003BD300[1] == buffer) {
        D_003BD300[1] = buffer ^ 1;
    }
    D_003BD302 = next;
}

void func_002D1350(s32 both, s32 value, s32 index) {
    if (both == 0) {
        D_003BD300[0] = value;
        D_003BD300[1] = value;
    } else {
        D_003BD300[index] = value;
    }
    D_003BD302 = -1;
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1380);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D14C8);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1590);

extern vu8 D_003BD2EA;
extern void func_002E1218(void);

void func_002D1698(void) {
    s8 buffer = D_003BD2EA ^ 1;

    while (D_003BD302 == buffer) {
    }
    D_003BD2EA = buffer;
    sdfSelectDoubleBuffer((s8)D_003BD2EA);
    func_002E1218();
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D16F0);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1740);

s32 func_002D1798(SdfTex *target) {
    SdfTex *node = (SdfTex *)D_003BD9E0;

    if (node == NULL) {
        return 0;
    }
    do {
        if (node == target) {
            return 1;
        }
        node = node->prev;
    } while (node != NULL);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D17D8);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D18F8);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1A18);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1AB8);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1B28);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1B90);

void func_002D1C08(s32 arg0) {
    func_002D3C30(&D_003BD9E8, arg0);
}

void sdfTexInitializeLists(void) {
    SdfTexHead *head;

    head = func_002CFEB8(0x1C);
    head->unk10 = 0x100000;
    head->next = NULL;
    head->prev = NULL;
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

void sdfTexInitializeSemaphore(void) {
    SdfSemaObj *obj;

    obj = &D_003EB848;
    obj->unk0 = sdfCreateSemaphore(1, 0x7F, 0);
    func_002D2128(obj);
}

u32 func_002D2300(SdfTex *texture) {
    return (u32)texture->unk28;
}

s32 func_002D2308(SdfTex *tex) {
    SdfTexBuf *buf = tex->unk28;

    if (buf == NULL) {
        return 0;
    }
    return ((buf->unk0 & 0x7FFF) + 1) << 4;
}

s32 sdfTexGetOrInitializeSecondaryBuffer(SdfTex *texture) {
    SdfTexBuf *buf;

    buf = texture->unk2C;
    if (buf == NULL) {
        func_002D2F80();
        buf = texture->unk2C;
    }
    return (s32)buf;
}

s32 func_002D2368(SdfTex *tex) {
    SdfTexBuf *buf = tex->unk2C;

    if (buf == NULL) {
        return 0;
    }
    return ((buf->unk0 & 0x7FFF) + 1) << 4;
}

u8 func_002D2390(SdfTex *texture) {
    return texture->unk18;
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

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D23C0);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2410);

u64 func_002D2468(SdfTex *texture) {
    return texture->unk28->unk20;
}

u64 func_002D2478(SdfTex *texture) {
    return texture->unk28->unk10;
}

u64 func_002D2488(SdfTex *texture) {
    return texture->unk28->unk30;
}

void func_002D2498(SdfTex *texture, s32 arg1, s32 arg2) {
    SdfTexBuf *buf;

    buf = texture->unk28;
    buf->unk10 = (buf->unk10 & ~0x1E0) | (arg1 << 5) | (arg2 << 6);
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D24C0);

void func_002D2530(SdfTex *texture, u8 value) {
    texture->unk1F = value;
    func_002D2FB0();
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2548);

extern void func_002D2548();

void func_002D2650(SdfTex *tex, s32 arg1, u8 *arg2, s32 arg3) {
    s32 width;
    s32 height;

    if (tex->unk1A == 0x13 || tex->unk1A == 0x1B) {
        width = 0x10;
        height = 0x10;
    } else {
        width = 8;
        height = 2;
    }
    func_002D2548(arg1, width, height, tex->unk19, arg2, arg3);
}

void func_002D26A8(SdfTex *tex) {
    if (tex->secondaryResource != NULL) {
        func_002D2650(tex, sdfTexGetSecondaryResourceWord(tex), tex->data, 0);
    }
}

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

