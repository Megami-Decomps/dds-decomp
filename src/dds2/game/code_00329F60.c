#include "common.h"

#include "sdf.h"

typedef struct SdfTexPacketTail {
    u64 tag;      /* 0x00 */
    u64 next;     /* 0x08 */
} SdfTexPacketTail;

typedef struct SdfTexBlock {
    struct SdfTexBlock *next; /* 0x00 */
    struct SdfTexBlock *prev; /* 0x04 */
    s32 used;                 /* 0x08 */
    s32 unk0C;
    s32 size;                 /* 0x10 */
} SdfTexBlock;

extern void func_00328E48();
s32 func_0032A968(SdfTexBlock *block);

typedef struct SdfTexReleaseEntry {
    struct SdfTexReleaseEntry *next; /* 0x00 */
    s32 address;                     /* 0x04 */
    s32 handle;                      /* 0x08 */
    u8 mode;                         /* 0x0C: 1 = handle, 2 = chip memory address */
    u8 pad0D[0x93];
} SdfTexReleaseEntry; /* 0xA0 */

extern s32 sdfChipIsInRange();
extern s32 func_00329930();


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

s32 func_0032AA40(void *arg0);

void sdfInitializeSynchronizedRequest(void *arg0, void (*arg1)(void *));

extern SdfSemaObj D_004681F8;

s32 sdfCreateSemaphore(s32 arg0, s32 arg1, s32 arg2);

extern s8 D_004389F0[2];

extern volatile s8 D_004389F2;

extern vu8 D_004389DA;

extern void func_0033A0C8(void);

extern void func_0032B3F8();

void func_00329F60(u32 arg0, u32 arg1) {
    D_004389E4 = arg0;
    D_004389E8 = arg1;
    D_004389E0 = 1;
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_00329F78);

/* Switch both references off the finished double-buffer slot before
 * publishing the slot currently in use. */
void sdfSwapBufferSlots(s32 oldBuffer, s32 nextBuffer) {
    if (D_004389F0[0] == oldBuffer) {
        D_004389F0[0] = oldBuffer ^ 1;
    }
    if (D_004389F0[1] == oldBuffer) {
        D_004389F0[1] = oldBuffer ^ 1;
    }
    D_004389F2 = nextBuffer;
}

void sdfSetBufferSlot(s32 singleBuffer, s32 value, s32 index) {
    if (singleBuffer == 0) {
        D_004389F0[0] = value;
        D_004389F0[1] = value;
    } else {
        D_004389F0[index] = value;
    }
    D_004389F2 = -1;
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A230);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A378);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A440);

/* Wait until the other buffer is no longer busy before selecting it. */
void sdfWaitAndSelectBuffer(void) {
    s8 buffer = D_004389DA ^ 1;

    /* Do not select a buffer while its index is the busy-buffer status. */
    while (D_004389F2 == buffer) {
    }
    D_004389DA = buffer;
    sdfSelectDoubleBuffer((s8)D_004389DA);
    func_0033A0C8();
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
    SdfTex *node = (SdfTex *)D_00439140;

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

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A688);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A7A8);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A8C8);

s32 func_0032A968(SdfTexBlock *block) {
    SdfTexBlock *prev = block->prev;

    if (prev != NULL) {
        if (prev->used == 0) {
            SdfTexBlock *before = prev->prev;

            block->size = block->size + prev->size;
            block->prev = before;
            if (before != NULL) {
                prev->prev->next = block;
            } else {
                D_00439144 = (SdfTexHead *)block;
            }
            func_00328E48(prev, block, prev);
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A9D8);

extern s32 func_0036DE70();


s32 func_0032AA40(void *node) {
    s32 interruptsEnabled;
    u8 *next;

    if (node != NULL) {
        interruptsEnabled = func_0036DE70();
        *(s32 *)((u8 *)node + 8) = 0;
        func_0032A968(node);
        next = *(u8 **)node;
        if (next != NULL && *(s32 *)(next + 8) == 0) {
            func_0032A968(next);
        }
        if (interruptsEnabled != 0) {
            EIntr();
        }
    }
}

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
    sdfInitializeSynchronizedRequest(&D_00439148, func_0032AA40);
}

INCLUDE_ASM(const s32, "game/code_00329F60", sdfAllocImageBuffer);

u32 func_0032ABC0(void) {
    return D_00439140;
}

u32 func_0032ABC8(void) {
    return D_00439144;
}

INCLUDE_ASM(const s32, "game/code_00329F60", sdfFormatImageSize);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032AC30);

void func_0032AEA0(s32 address, void *packet) {
    SdfSemaObj *obj = &D_004681F8;
    SdfTexPacketTail *last;

    WaitSema(obj->unk0);
    last = (SdfTexPacketTail *)obj->unk10;
    if (last != NULL) {
        last->next = 0;
        last->tag = ((u64)(address & 0x0FFFFFFF) << 32) | 0x20000000;
    } else {
        obj->unkC = (void *)address;
    }
    obj->unk10 = (s32)packet;
    SignalSema(obj->unk0);
}

void func_0032AF20(s32 address) {
    SdfSemaObj *obj = &D_004681F8;
    SdfTexReleaseEntry *entry;

    if (address != 0) {
        entry = func_00328E18(0xA0);
        if (sdfChipIsInRange(address) != 0) {
            entry->address = address;
            entry->mode = 2;
        } else {
            entry->mode = 1;
            entry->handle = func_00329930(address);
        }
        WaitSema(obj->unk0);
        if (obj->unk8 != NULL) {
            ((SdfTexReleaseEntry *)obj->unk8)->next = entry;
        } else {
            obj->unk4 = entry;
        }
        obj->unk8 = entry;
        SignalSema(obj->unk0);
    }
}

void sdfResetSemaphoreState(SdfSemaObj *semaphore) {
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
    SdfTexBuf *buffer;

    buffer = texture->unk2C;
    if (buffer == NULL) {
        sdfTexCreateSecondPacket();
        buffer = texture->unk2C;
    }
    return (s32)buffer;
}

/* Mirror the primary-buffer size calculation for the secondary buffer. */
s32 sdfTexGetSecondaryBufferSize(SdfTex *tex) {
    SdfTexBuf *buf = tex->unk2C;

    if (buf == NULL) {
        return 0;
    }
    return ((buf->unk0 & 0x7FFF) + 1) << 4;
}

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

void sdfTexSetPrimaryBufferModeBits(SdfTex *arg0, s32 arg1, s32 arg2) {
    SdfTexBuf *buf;

    buf = arg0->unk28;
    buf->unk10 = (buf->unk10 & ~0x1E0) | (arg1 << 5) | (arg2 << 6);
}

void func_0032B370(SdfTex *tex, s32 arg1, s32 arg2) {
    SdfTexBuf *buf = tex->unk2C;

    if (buf == NULL) {
        sdfTexCreateSecondPacket();
        buf = tex->unk2C;
    }
    buf->unk10 = (arg2 << 6) | ((arg1 << 5) | (buf->unk10 & ~0x1E0));
}

void func_0032B3E0(SdfTex *texture, u8 value) {
    texture->unk1F = value;
    func_0032BE60();
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B3F8);

void func_0032B500(SdfTex *texture, s32 resourceWord, u8 *pixels, s32 mode) {
    s32 width;
    s32 height;

    if (texture->unk1A == 0x13 || texture->unk1A == 0x1B) {
        width = 0x10;
        height = 0x10;
    } else {
        width = 8;
        height = 2;
    }
    func_0032B3F8(resourceWord, width, height, texture->unk19, pixels, mode);
}

void func_0032B558(SdfTex *tex) {
    if (tex->secondaryResource != NULL) {
        func_0032B500(tex, sdfTexGetSecondaryResourceWord(tex), tex->data, 0);
    }
}

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

extern void *func_00328E18(s32 size);

SdfTex *func_0032B5D8(s32 x, s32 y, s32 arg2, s32 arg3, s32 primary, s32 arg5, s32 arg6, s32 secondary) {
    SdfTex *tex = func_00328E18(0x40);
    SdfTexRef *ref = func_00328E18(8);

    ref->refCount = 1;
    tex->unk18 = arg6;
    tex->unk19 = arg5;
    tex->unkC = x;
    tex->unkE = y;
    tex->unk1A = arg2;
    tex->unk1B = arg3;
    tex->secondaryResource = (SdfTexResource *)secondary;
    tex->primaryResource = (SdfTexResource *)primary;
    tex->reference = ref;
    sdfTexListInsert(tex);
    tex->unk38 = 0x80808080;
    return tex;
}

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

