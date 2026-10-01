#include "common.h"

/* Circular doubly-linked list node; prev/next at +0x18/+0x1C. */
typedef struct FntNode {
    u8 unk0[0x18];          /* 0x0 */
    struct FntNode *prev;   /* 0x18 */
    struct FntNode *next;   /* 0x1C */
} FntNode;

/* List header at D_003D68C0: entry count plus sentinel node link. */
typedef struct FntList {
    u8 unk0[0x18];  /* 0x0 */
    s32 count;      /* 0x18 */
    FntNode *head;  /* 0x1C */
} FntList;

/* 0x24-byte table entry in the D_003D6C80 font system (one per index & 0xFF). */
typedef struct FrFontEntry {
    void *buffer;  /* 0x0: released by frFontFreeEntry */
    void *unk4;  /* 0x4: value record (u16 pair read via D_003D6C84 view) */
    u32 unk8;    /* 0x8 */
    u32 unkC;    /* 0xC */
    u8 *flagBytes; /* 0x10: first byte enables entry, second byte stores value + 1 */
    void *unk14; /* 0x14 */
    void *unk18; /* 0x18 */
    void *unk1C; /* 0x1C */
    u32 unk20;   /* 0x20 */
} FrFontEntry;

/* Font system at D_003D6C80: 9 entries followed by shared control words. */
typedef struct FrFontSysLocal {
    FrFontEntry entries[9]; /* 0x0 */
    s32 unk144;             /* 0x144 */
    s32 unk148;             /* 0x148 */
    s32 unk14C;             /* 0x14C */
    void *unk150;           /* 0x150: passed to itfReleaseMemNodeBuffer by frFontReleaseAll */
    void *unk154;           /* 0x154: passed to itfReleaseMemNodeBuffer by frFontReleaseAll */
    void *unk158;           /* 0x158: passed to sdfUpdateTextureHeadsWithInterruptsMasked by frFontReleaseAll */
    void *unk15C;           /* 0x15C: passed to sdfUpdateTextureHeadsWithInterruptsMasked by frFontReleaseAll */
    u8 unk160[0x34];        /* 0x160 */
    void *glyphSlots[2];     /* 0x194: glyph chain slots */
} FrFontSysLocal;

extern FntList D_003D68C0;
extern FrFontSysLocal D_003D6C80;
extern u32 D_003565F8[];
extern void frFontFreeAllEntries(void);
extern void *func_00194840(void *arg0);
extern u32 itfReleaseMemNodeBuffer(s32 arg0);
extern void fmGslReleaseActiveResourceBuffers(void);
extern void func_002D0918(void *arg0);
extern void sdfUpdateTextureHeadsWithInterruptsMasked(void *arg0);

void frFontListInsert(FntNode *node) {
    FntNode *head = D_003D68C0.head;
    FntNode *next = head->next;

    node->prev = head;
    node->next = next;
    D_003D68C0.count += 1;
    head->next = node;
    next->prev = node;
}

void frFontSetEntryFlag(s32 index, s32 value) {
    FrFontEntry *entry = &D_003D6C80.entries[index & 0xFF];

    entry->flagBytes[0] = 1;
    entry->flagBytes[1] = value + 1;
}

INCLUDE_ASM(const s32, "game/code_00193C08", func_00193C70);

/* Return the one-based highest set bit, wrapped to a byte; zero stays zero. */
s32 frFontHighestSetBitIndex(u32 value) {
    s32 count = 0;

    if (value == 0) {
        return 0;
    }
    do {
        value = value >> 1;
        count += 1;
    } while (value != 0);
    return (count + 0xFF) & 0xFF;
}

INCLUDE_ASM(const s32, "game/code_00193C08", func_00193CD8);

INCLUDE_ASM(const s32, "game/code_00193C08", func_00193D70);

INCLUDE_ASM(const s32, "game/code_00193C08", func_00193FD0);

extern volatile s32 D_003BD2EC; /* semaphore handle shared with the IOP/interrupt side; declared volatile */
extern void sceGsSetDefLoadImage(void *, s16, s32, s32, s32, s32, s32, s32);
extern void sceGsExecLoadImage(void *, s32);
extern void sceGsSyncPath(s32, s32);
extern void FlushCache(s32);
extern s32 WaitSema(s32);
extern s32 SignalSema(s32);

/* Upload an image to GS memory at the given buffer, serialized by the GS semaphore. */
void func_00194038(s16 buffer, s32 image) {
    u8 loadImage[0x60];

    sceGsSetDefLoadImage(loadImage, buffer, 1, 0, 0, 0, 8, 2);
    WaitSema(D_003BD2EC);
    FlushCache(0);
    sceGsExecLoadImage(loadImage, image);
    sceGsSyncPath(0, 0);
    SignalSema(D_003BD2EC);
}

INCLUDE_ASM(const s32, "game/code_00193C08", func_001940B8);

INCLUDE_ASM(const s32, "game/code_00193C08", func_00194190);

INCLUDE_ASM(const s32, "game/code_00193C08", func_00194228);

void frFontReleaseAll(void) {
    frFontFreeAllEntries();
    func_00194840(D_003D6C80.glyphSlots[0]);
    func_00194840(D_003D6C80.glyphSlots[1]);
    itfReleaseMemNodeBuffer(D_003D6C80.unk150);
    itfReleaseMemNodeBuffer(D_003D6C80.unk154);
    fmGslReleaseActiveResourceBuffers();
    sdfUpdateTextureHeadsWithInterruptsMasked(D_003D6C80.unk158);
    sdfUpdateTextureHeadsWithInterruptsMasked(D_003D6C80.unk15C);
}

INCLUDE_ASM(const s32, "game/code_00193C08", func_001944A0);

void frFontFreeEntry(s32 index) {
    u32 slot = index & 0xFF;
    FrFontEntry *entry;

    if (slot < 2) {
        return;
    }
    D_003565F8[slot] = 0;
    entry = &D_003D6C80.entries[slot];
    if (entry->buffer != NULL) {
        func_002D0918(entry->buffer);
        entry->unk1C = NULL;
    }
    entry->unk18 = NULL;
}
