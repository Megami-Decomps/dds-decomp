#include "common.h"

/* Circular doubly-linked list node; prev/next at +0x18/+0x1C. */
typedef struct FntNode {
    u8 unk0[0x18];          /* 0x0 */
    struct FntNode *prev;   /* 0x18 */
    struct FntNode *next;   /* 0x1C */
} FntNode;

/* List header at frFontResourceList: entry count plus sentinel node link. */
typedef struct FntList {
    u8 unk0[0x18];  /* 0x0 */
    s32 count;      /* 0x18 */
    FntNode *head;  /* 0x1C */
} FntList;

/* 0x24-byte table entry in the frFontWork font system (one per index & 0xFF). */
typedef struct FrFontEntry {
    void *buffer;  /* 0x0: released by frFontFreeEntry */
    void *unk4;  /* 0x4: value record (u16 pair read via frFontResourceRecords view) */
    u32 unk8;    /* 0x8 */
    u32 unkC;    /* 0xC */
    u8 *flagBytes; /* 0x10: first byte enables entry, second byte stores value + 1 */
    void *unk14; /* 0x14 */
    void *unk18; /* 0x18 */
    void *unk1C; /* 0x1C */
    u32 unk20;   /* 0x20 */
} FrFontEntry;

/* Font system at frFontWork: 9 entries followed by shared control words. */
typedef struct FrFontSysLocal {
    FrFontEntry entries[9]; /* 0x0 */
    s32 unk144;             /* 0x144 */
    s32 unk148;             /* 0x148 */
    s32 unk14C;             /* 0x14C */
    void *unk150;           /* 0x150: passed to itfReleaseMemNodeBuffer by frFontReleaseAll */
    void *unk154;           /* 0x154: passed to itfReleaseMemNodeBuffer by frFontReleaseAll */
    void *unk158;           /* 0x158: passed to sdfUpdateTextureHeadsWithInterruptsMasked by frFontReleaseAll */
    void *unk15C;           /* 0x15C: passed to sdfUpdateTextureHeadsWithInterruptsMasked by frFontReleaseAll */
    s32 width;              /* 0x160 */
    s32 unk164;             /* 0x164 */
    s32 height;             /* 0x168 */
    s32 unk16C;             /* 0x16C */
    s32 gsBuffer;           /* 0x170 */
    s32 gsFormat;           /* 0x174 */
    u8 unk178[0x1C];        /* 0x178 */
    void *glyphSlots[2];     /* 0x194: glyph chain slots */
} FrFontSysLocal;

extern FntList frFontResourceList;
extern FrFontSysLocal frFontWork;
extern u32 frFontSlotLoadedFlags[];
extern void frFontFreeAllEntries(void);
extern void *frFontReleaseGlyphChain(void *arg0);
extern u32 itfReleaseMemNodeBuffer(s32 arg0);
extern void fmGslReleaseActiveResourceBuffers(void);
extern void sdfReleaseResourceAllocation(void *arg0);
extern void sdfUpdateTextureHeadsWithInterruptsMasked(void *arg0);

void frFontListInsert(FntNode *node) {
    FntNode *head = frFontResourceList.head;
    FntNode *next = head->next;

    node->prev = head;
    node->next = next;
    frFontResourceList.count += 1;
    head->next = node;
    next->prev = node;
}

void frFontSetEntryFlag(s32 index, s32 value) {
    FrFontEntry *entry = &frFontWork.entries[index & 0xFF];

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

typedef union FrFontDmaTag {
    u128 value;
    u32 words[4];
} FrFontDmaTag;

typedef struct FrFontGsWrite {
    u64 value;
    u64 reg;
} FrFontGsWrite;

typedef struct FrFontGsPacket {
    FrFontDmaTag dma;
    u64 gifTag;
    u64 gifRegisters;
    FrFontGsWrite writes[6];
} FrFontGsPacket;

void frFontBuildGsSetupPacket(FrFontGsPacket *packet) {
    packet->dma.value = 0;
    packet->dma.words[0] = 0x70000007;
    packet->dma.words[2] = 0;
    packet->dma.words[3] = 0x50000007;
    packet->gifTag = 0x1000000000008006ULL;
    packet->gifRegisters = 0xE;
    packet->writes[0].value = 0;
    packet->writes[0].reg = 0x3F;
    packet->writes[1].value = 0x44;
    packet->writes[1].reg = 0x42;
    packet->writes[2].value = 0;
    packet->writes[2].reg = 0x49;
    packet->writes[3].value = 0x8080;
    packet->writes[3].reg = 0x3B;
    packet->writes[4].value = 0;
    packet->writes[4].reg = 0x4A;
    packet->writes[5].value = 0x517ED;
    packet->writes[5].reg = 0x47;
}

INCLUDE_ASM(const s32, "game/code_00193C08", func_00193D70);

INCLUDE_ASM(const s32, "game/code_00193C08", func_00193FD0);

extern volatile s32 sdfGsImageUploadSemaphore; /* semaphore handle shared with the IOP/interrupt side; declared volatile */
extern void sceGsSetDefLoadImage(void *, s16, s16, s32, s32, s32, s16, s16);
extern s32 sdfAllocGeneralBlock(s32);
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
    block = sdfAllocGeneralBlock(size);
    image = (void *)sdfResourceRetainAddress(block);
    memset(image, 0, size);
    sceGsSetDefLoadImage(loadImage, (s16)frFontWork.gsBuffer, (s16)frFontWork.gsFormat, 0x14, 0, 0,
                         (s16)frFontWork.width, (s16)frFontWork.height);
    WaitSema(sdfGsImageUploadSemaphore);
    FlushCache(0);
    sceGsExecLoadImage(loadImage, (s32)image);
    sceGsSyncPath(0, 0);
    SignalSema(sdfGsImageUploadSemaphore);
    sdfReleaseResourceAllocation((void *)block);
}

extern void *sdfReadNamedResource();
extern void frFontBindResourceSections(u8, u8 *, void *);

/* Load font `index` once (index 1 uses the system's first entry buffer, other fonts load `path`) and mark it loaded. */
void frFontEnsureSlotLoaded(s32 index, s32 path) {
    s32 slot = index & 0xFF;
    FrFontSysLocal *sys = &frFontWork;

    if (frFontSlotLoadedFlags[slot] != 1) {
        if (slot == 1) {
            frFontBindResourceSections(1, 0, sys->entries[0].buffer);
        } else {
            frFontBindResourceSections(slot, 0, sdfReadNamedResource(path, 0, 0));
        }
        frFontSlotLoadedFlags[slot] = 1;
    }
}

INCLUDE_ASM(const s32, "game/code_00193C08", func_00194228);

void frFontReleaseAll(void) {
    frFontFreeAllEntries();
    frFontReleaseGlyphChain(frFontWork.glyphSlots[0]);
    frFontReleaseGlyphChain(frFontWork.glyphSlots[1]);
    itfReleaseMemNodeBuffer(frFontWork.unk150);
    itfReleaseMemNodeBuffer(frFontWork.unk154);
    fmGslReleaseActiveResourceBuffers();
    sdfUpdateTextureHeadsWithInterruptsMasked(frFontWork.unk158);
    sdfUpdateTextureHeadsWithInterruptsMasked(frFontWork.unk15C);
}

typedef struct FrFontHeader {
    u32 tableOffset;
    u8 pad04[6];
    u8 tableCount;
    u8 pad0B[3];
    u16 widthCount;
    u8 pad10[6];
    u8 hasExtra;
    u8 pad17;
    u32 lookupOffset;
} FrFontHeader;

void frFontBindResourceSections(u8 index, u8 *header, void *buffer) {
    FrFontEntry *entry;
    s32 offset;
    u32 lookup;

    if (header == NULL) {
        if (buffer != NULL) {
            header = (u8 *)sdfResourceRetainAddress((s32)buffer);
        }
    }
    entry = &frFontWork.entries[index];
    entry->buffer = buffer;
    entry->unk4 = header;
    offset = ((FrFontHeader *)header)->tableOffset + (((FrFontHeader *)header)->tableCount << 6);
    if (((FrFontHeader *)header)->hasExtra != 0) {
        s32 *flags = (s32 *)(header + offset);
        s32 flagSize = *flags;
        s32 *values;
        s32 valueSize;
        s32 flagBlockSize;
        s32 valueBlockSize;

        entry->flagBytes = (u8 *)(flags + 1);
        entry->unk8 = flagSize;
        flagBlockSize = flagSize + 4;
        offset += flagBlockSize;
        values = (s32 *)(header + offset);
        valueSize = *values;
        entry->unk14 = values + 1;
        entry->unkC = valueSize;
        valueBlockSize = valueSize + 4;
        offset += valueBlockSize;
    } else {
        entry->flagBytes = NULL;
        entry->unk14 = NULL;
        entry->unk8 = 0;
        entry->unkC = 0;
    }
    entry->unk18 = header + offset;
    offset += ((FrFontHeader *)entry->unk4)->widthCount * 4;
    entry->unk1C = header + offset;
    lookup = ((FrFontHeader *)entry->unk4)->lookupOffset;
    if (lookup != 0) {
        entry->unk20 = (u32)(header + lookup);
    } else {
        entry->unk20 = 0;
    }
}

void frFontFreeEntry(s32 index) {
    u32 slot = index & 0xFF;
    FrFontEntry *entry;

    if (slot < 2) {
        return;
    }
    frFontSlotLoadedFlags[slot] = 0;
    entry = &frFontWork.entries[slot];
    if (entry->buffer != NULL) {
        sdfReleaseResourceAllocation(entry->buffer);
        entry->unk1C = NULL;
    }
    entry->unk18 = NULL;
}
