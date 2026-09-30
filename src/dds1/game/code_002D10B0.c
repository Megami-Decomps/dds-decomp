#include "common.h"
#include "sdf.h"

typedef struct SdfTexHead {
    SdfTex *next; /* 0x0: SdfTex-compatible linked-list prefix */
    SdfTex *prev; /* 0x4 */
    void *unk8; /* 0x8 */
    void *unkC; /* 0xC */
    s32 unk10; /* 0x10 */
    s16 width; /* 0x14 */
    s16 height; /* 0x16 */
    s32 format; /* 0x18 */
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
struct SdfTexHead *func_002D17D8(s32 size, s32 arg1);
void func_002D1B90(void *arg0);
void sdfTexCreateSecondPacket(void);
void func_002D2FB0(void);
void func_002D3C30(void *arg0, s32 arg1);
void sdfInitializeSynchronizedRequest(void *arg0, void (*arg1)(void *));
void func_002D10B0(u32 arg0, u32 arg1) {
    D_003BD2F4 = arg0;
    D_003BD2F8 = arg1;
    D_003BD2F0 = 1;
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D10C8);

/* Switch both references off the finished double-buffer slot before
 * publishing the slot currently in use. */
void sdfSwapBufferSlots(s32 oldBuffer, s32 nextBuffer) {
    if (D_003BD300[0] == oldBuffer) {
        D_003BD300[0] = oldBuffer ^ 1;
    }
    if (D_003BD300[1] == oldBuffer) {
        D_003BD300[1] = oldBuffer ^ 1;
    }
    D_003BD302 = nextBuffer;
}

void sdfSetBufferSlot(s32 singleBuffer, s32 value, s32 index) {
    if (singleBuffer == 0) {
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

/* Wait until the other buffer is no longer busy before selecting it. */
void sdfWaitAndSelectBuffer(void) {
    s8 buffer = D_003BD2EA ^ 1;

    /* Do not select a buffer while its index is the busy-buffer status. */
    while (D_003BD302 == buffer) {
    }
    D_003BD2EA = buffer;
    sdfSelectDoubleBuffer((s8)D_003BD2EA);
    func_002E1218();
}

s32 sdfFormatBitsPerPixelA(u32 format) {
    switch (format) {
    case 0x0:
    case 0x1:
    case 0x1B:
    case 0x24:
    case 0x2C:
    case 0x30:
    case 0x31:
        return 0x20;
    case 0x2:
    case 0xA:
    case 0x32:
    case 0x3A:
        return 0x10;
    case 0x13:
        return 8;
    case 0x14:
        return 4;
    default:
        return 0;
    }
}

s32 sdfFormatBitsPerPixelB(u32 format) {
    switch (format) {
    case 0x0:
    case 0x30:
        return 0x20;
    case 0x1:
    case 0x31:
        return 0x18;
    case 0x2:
    case 0xA:
    case 0x32:
    case 0x3A:
        return 0x10;
    case 0x13:
    case 0x1B:
        return 8;
    case 0x14:
    case 0x24:
    case 0x2C:
        return 4;
    default:
        return 0;
    }
}

s32 sdfTexListContains(SdfTex *target) {
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
    sdfInitializeSynchronizedRequest(&D_003BD9E8, func_002D1B90);
}

SdfTexHead *sdfAllocImageBuffer(s32 width, s32 height, s32 format) {
    s32 alignedWidth = (width + 0x3F) & -0x40;
    s32 alignedHeight = (height + 0x1F) & -0x20;
    SdfTexHead *node;

    switch (format) {
    case 0x2:
    case 0xA:
    case 0x32:
    case 0x3A:
        alignedWidth >>= 1;
        break;
    case 0x0:
    case 0x1:
    case 0x30:
    case 0x31:
        break;
    }
    node = func_002D17D8((alignedWidth * alignedHeight + 0x7FF) & -0x800, 1);
    node->width = width;
    node->height = height;
    node->format = format;
    return node;
}

SdfTexHead *func_002D1D10(void) {
    return D_003BD9E0;
}

SdfTexHead *func_002D1D18(void) {
    return D_003BD9E4;
}

s32 sdfFormatImageSize(u32 format, s32 width, s32 height) {
    s32 bits;

    switch (format) {
    case 0x1:
    case 0x31:
        bits = 0x18;
        break;
    case 0x2:
    case 0xA:
    case 0x32:
    case 0x3A:
        bits = 0x10;
        break;
    case 0x13:
    case 0x1B:
        bits = 8;
        break;
    case 0x14:
    case 0x24:
    case 0x2C:
        bits = 4;
        break;
    default:
        bits = 0x20;
        break;
    }
    return (bits * width * height) >> 7;
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1D80);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1FF0);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2070);

void sdfResetSemaphoreState(SdfSemaObj *arg0) {
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
    sdfResetSemaphoreState(obj);
}

u32 sdfTexGetPrimaryBuffer(SdfTex *texture) {
    return (u32)texture->unk28;
}

/* Size in bytes of a packed primary texture buffer: only the low 15 bits
 * contribute to its 16-byte block count. */
s32 sdfTexGetPrimaryBufferSize(SdfTex *tex) {
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
        sdfTexCreateSecondPacket();
        buf = texture->unk2C;
    }
    return (s32)buf;
}

/* Mirror the primary-buffer size calculation for the secondary buffer. */
s32 sdfTexGetSecondaryBufferSize(SdfTex *tex) {
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

s32 sdfFormatBitsPerPixelC(u32 format) {
    switch (format) {
    case 0:
        return 0x20;
    case 1:
        return 0x18;
    case 2:
    case 10:
        return 0x10;
    case 19:
    case 27:
        return 8;
    default:
        return 4;
    }
}

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

void sdfTexSetPrimaryBufferModeBits(SdfTex *texture, s32 arg1, s32 arg2) {
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

void func_002D2650(SdfTex *texture, s32 resourceWord, u8 *pixels, s32 mode) {
    s32 width;
    s32 height;

    if (texture->unk1A == 0x13 || texture->unk1A == 0x1B) {
        width = 0x10;
        height = 0x10;
    } else {
        width = 8;
        height = 2;
    }
    func_002D2548(resourceWord, width, height, texture->unk19, pixels, mode);
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

