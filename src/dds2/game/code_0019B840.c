#include "common.h"

extern s32 D_00436558;

/* Value with u16 pair read by func_001971E0/func_00197200. */
typedef struct Unk6C84Val {
    u8 unk0[0x10]; /* 0x0 */
    u16 unk10;     /* 0x10 */
    u16 unk12;     /* 0x12 */
} Unk6C84Val;

/* 0x24-byte record pointing at the value. */
typedef struct Unk6C84Rec {
    Unk6C84Val *unk0; /* 0x0 */
    u8 unk4[0x20];    /* 0x4 */
} Unk6C84Rec;

extern Unk6C84Rec frFontResourceRecords[];

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

extern FntList frFontResourceList;

/* 0x24-byte table entry in the D_003D6C80 font system (one per index & 0xFF). */
typedef struct FrFontEntry {
    void *buffer; /* 0x0: released by frFontFreeEntry */
    void *unk4;  /* 0x4: value record (u16 pair read via D_003D6C84 view) */
    u32 unk8;    /* 0x8 */
    u32 unkC;    /* 0xC */
    u8 *flagBytes; /* 0x10: first byte enables entry, second stores value + 1 */
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
    void *unk150;           /* 0x150: passed to func_001982A0 by func_00194440 */
    void *unk154;           /* 0x154: passed to func_001982A0 by func_00194440 */
    void *unk158;           /* 0x158: passed to func_002D1B90 by func_00194440 */
    void *unk15C;           /* 0x15C: passed to func_002D1B90 by func_00194440 */
    s32 width;              /* 0x160 */
    s32 unk164;             /* 0x164 */
    s32 height;             /* 0x168 */
    s32 unk16C;             /* 0x16C */
    s32 gsBuffer;           /* 0x170 */
    s32 gsFormat;           /* 0x174 */
    u8 unk178[0x1C];        /* 0x178 */
    void *glyphSlots[2]; /* 0x194: glyph chain slots */
} FrFontSysLocal;

extern FrFontSysLocal frFontWork;

extern void frFontFreeAllEntries(void);

extern void *func_0019C4D0(void *arg0);

extern u32 itfReleaseMemNodeBuffer(s32 arg0);

extern void fmGslReleaseActiveResourceBuffers(void);

extern void sdfUpdateTextureHeadsWithInterruptsMasked(void *arg0);

extern u32 D_003B2F30[];

extern void func_003297C8(void *arg0);

void frFontListInsert(FntNode *node) {
    FntNode *head = frFontResourceList.head;
    FntNode *next = head->next;

    node->prev = head;
    node->next = next;
    frFontResourceList.count += 1;
    head->next = node;
    next->prev = node;
}

u16 func_0019B870(s32 index) {
    return frFontResourceRecords[index].unk0->unk10;
}

u16 func_0019B890(s32 index) {
    return frFontResourceRecords[index].unk0->unk12;
}

void itfSetTextDrawLimit(s32 value) {
    if (value < 1) {
        value = 0x14;
    }
    D_00436558 = value;
}

void frFontSetEntryFlag(s32 index, s32 flag) {
    FrFontEntry *entry = &frFontWork.entries[index & 0xFF];

    entry->flagBytes[0] = 1;
    entry->flagBytes[1] = flag + 1;
}

INCLUDE_ASM(const s32, "game/code_0019B840", func_0019B900);

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

INCLUDE_ASM(const s32, "game/code_0019B840", func_0019B968);

INCLUDE_ASM(const s32, "game/code_0019B840", func_0019BA00);

INCLUDE_ASM(const s32, "game/code_0019B840", func_0019BC60);

extern volatile s32 sdfGsImageUploadSemaphore; /* semaphore handle shared with the IOP/interrupt side; declared volatile */
extern void sceGsSetDefLoadImage(void *, s16, s16, s32, s32, s32, s16, s16);
extern s32 func_003292A8(s32);
extern s32 sdfResourceRetainAddress(s32);
extern void sceGsExecLoadImage(void *, s32);
extern void sceGsSyncPath(s32, s32);
extern void FlushCache(s32);
extern s32 WaitSema(s32);
extern s32 SignalSema(s32);

/* Upload an image to GS memory at the given buffer, serialized by the GS semaphore. */
void sdfUploadGsImageUnderSemaphore(s16 buffer, s32 image) {
    u8 loadImage[0x60];

    sceGsSetDefLoadImage(loadImage, buffer, 1, 0, 0, 0, 8, 2);
    WaitSema(sdfGsImageUploadSemaphore);
    FlushCache(0);
    sceGsExecLoadImage(loadImage, image);
    sceGsSyncPath(0, 0);
    SignalSema(sdfGsImageUploadSemaphore);
}

/* Upload the font's bitmap: clear a (width * height / 2)-byte block and load it to GS memory under the GS semaphore. */
void frFontUploadClearedTexture(void) {
    u8 loadImage[0x60];
    s32 size;
    s32 block;
    void *image;

    size = frFontWork.height * frFontWork.width;
    size = (u32)size >> 1;
    block = func_003292A8(size);
    image = (void *)sdfResourceRetainAddress(block);
    memset(image, 0, size);
    sceGsSetDefLoadImage(loadImage, (s16)frFontWork.gsBuffer, (s16)frFontWork.gsFormat, 0x14, 0, 0,
                         (s16)frFontWork.width, (s16)frFontWork.height);
    WaitSema(sdfGsImageUploadSemaphore);
    FlushCache(0);
    sceGsExecLoadImage(loadImage, (s32)image);
    sceGsSyncPath(0, 0);
    SignalSema(sdfGsImageUploadSemaphore);
    func_003297C8((void *)block);
}

extern void *func_00343ED0();
extern void func_0019C130(s32, s32, void *);

/* Load font `index` once (index 1 uses the system's first entry buffer, other fonts load `path`) and mark it loaded. */
void frFontEnsureSlotLoaded(s32 index, s32 path) {
    s32 slot = index & 0xFF;
    FrFontSysLocal *sys = &frFontWork;

    if (D_003B2F30[slot] != 1) {
        if (slot == 1) {
            func_0019C130(1, 0, sys->entries[0].buffer);
        } else {
            func_0019C130(slot, 0, func_00343ED0(path, 0, 0));
        }
        D_003B2F30[slot] = 1;
    }
}

INCLUDE_ASM(const s32, "game/code_0019B840", func_0019BEB8);

void frFontReleaseAll(void) {
    frFontFreeAllEntries();
    func_0019C4D0(frFontWork.glyphSlots[0]);
    func_0019C4D0(frFontWork.glyphSlots[1]);
    itfReleaseMemNodeBuffer(frFontWork.unk150);
    itfReleaseMemNodeBuffer(frFontWork.unk154);
    fmGslReleaseActiveResourceBuffers();
    sdfUpdateTextureHeadsWithInterruptsMasked(frFontWork.unk158);
    sdfUpdateTextureHeadsWithInterruptsMasked(frFontWork.unk15C);
}

INCLUDE_ASM(const s32, "game/code_0019B840", func_0019C130);

void frFontFreeEntry(s32 index) {
    u32 slot = index & 0xFF;
    FrFontEntry *entry;

    if (slot < 2) {
        return;
    }
    D_003B2F30[slot] = 0;
    entry = &frFontWork.entries[slot];
    if (entry->buffer != NULL) {
        func_003297C8(entry->buffer);
        entry->unk1C = NULL;
    }
    entry->unk18 = NULL;
}
