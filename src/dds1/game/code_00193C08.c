#include "common.h"

#define FR_FONT_BYTE_MASK 0xFF
#define FR_FONT_ENTRY_ENABLED 1
#define FR_FONT_LOADED_STATE 1
#define FR_FONT_SYSTEM_SLOT 1
#define FR_FONT_FIRST_RELEASABLE_SLOT 2
#define FR_FONT_BYTE_SHIFT 8
#define FR_FONT_OUTER_BYTE_SHIFT 24
#define FR_FONT_MIDDLE_BYTE_MASK 0x0000FF00
#define FR_FONT_TABLE_ENTRY_SHIFT 6
#define FR_FONT_WORD_BYTES 4
#define FR_FONT_IMAGE_DESCRIPTOR_BYTES 0x60
#define FR_FONT_GS_PSMT4 0x14
#define FR_FONT_GLYPH_HEIGHT_SHIFT 1
#define FR_FONT_GLYPH_SOURCE_Y_BIAS 4

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
    void *resourceHeader; /* 0x4: retained resource bytes, read as FrFontHeader */
    u32 unk8;    /* 0x8 */
    u32 unkC;    /* 0xC */
    u8 *flagBytes; /* 0x10: first byte enables entry, second byte stores value + 1 */
    void *unk14; /* 0x14 */
    void *unk18; /* 0x18 */
    void *unk1C; /* 0x1C */
    u32 unk20;   /* 0x20 */
} FrFontEntry;

/* Glyph fields consumed by the low-level packet submission wrapper. */
typedef struct FrFontDrawGlyph {
    u8 pad00[4];
    s32 x;
    s32 y;
    s32 advance;
    u32 flags;
    union {
        u32 word;
        u8 bytes[4];
    } style;
    union {
        u32 word;
        u8 bytes[4];
    } size;
    struct FrFontDrawGlyph *firstChild;
} FrFontDrawGlyph;

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

/* Insert after the list's fixed anchor and update the entry count. */
void frFontListInsert(FntNode *node) {
    FntNode *anchor = frFontResourceList.head;
    FntNode *nextNode = anchor->next;

    node->prev = anchor;
    node->next = nextNode;
    frFontResourceList.count += 1;
    anchor->next = node;
    nextNode->prev = node;
}

/* Select by the low byte of slotId; enable the entry and store value + 1 as a byte. */
void frFontSetEntryFlag(s32 slotId, s32 value) {
    FrFontEntry *entry = &frFontWork.entries[slotId & FR_FONT_BYTE_MASK];

    entry->flagBytes[0] = FR_FONT_ENTRY_ENABLED;
    entry->flagBytes[1] = value + 1;
}

/* Reverse the four byte positions of a 32-bit word. */
u32 func_00193C70(u32 word) {
    u32 shiftedWord = word >> FR_FONT_BYTE_SHIFT;
    u32 lowerMiddleByte = word & FR_FONT_MIDDLE_BYTE_MASK;
    u32 outerBytes = word << FR_FONT_OUTER_BYTE_SHIFT;

    word >>= FR_FONT_OUTER_BYTE_SHIFT;
    shiftedWord &= FR_FONT_MIDDLE_BYTE_MASK;
    lowerMiddleByte <<= FR_FONT_BYTE_SHIFT;
    outerBytes |= word;
    shiftedWord |= lowerMiddleByte;
    return outerBytes | shiftedWord;
}

/* Return the zero-based highest set bit; both zero and one return zero. */
s32 frFontHighestSetBitIndex(u32 value) {
    s32 shiftCount = 0;

    if (value == 0) {
        return 0;
    }
    do {
        value = value >> 1;
        shiftCount += 1;
    } while (value != 0);
    return (shiftCount + FR_FONT_BYTE_MASK) & FR_FONT_BYTE_MASK;
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

extern u8 D_003D6DE0[];
extern s32 func_00193D70(s32 x, s32 y, u8 width, u8 halfHeight, u8 style,
                         s32 flags, s32 color, s32 enabled, s32 sourceY,
                         void *table, s32 drawFlags);

/* Submit glyph-relative coordinates, half its byte height, and child source Y + 4. */
s32 func_00193FD0(s32 x, s32 y, s32 color, FrFontDrawGlyph *glyph,
                  s32 drawFlags) {
    return func_00193D70(x + glyph->x, y + glyph->y,
                         glyph->size.bytes[0], glyph->size.bytes[1] >> FR_FONT_GLYPH_HEIGHT_SHIFT,
                         glyph->style.bytes[0], glyph->flags, color, 1,
                         glyph->firstChild->y + FR_FONT_GLYPH_SOURCE_Y_BIAS, D_003D6DE0, drawFlags);
}

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
void sdfUploadGsImageUnderSemaphore(s32 buffer, s32 image) {
    u8 loadImage[0x60];

    sceGsSetDefLoadImage(loadImage, (s16)buffer, 1, 0, 0, 0, 8, 2);
    WaitSema(sdfGsImageUploadSemaphore);
    FlushCache(0);
    sceGsExecLoadImage(loadImage, image);
    sceGsSyncPath(0, 0);
    SignalSema(sdfGsImageUploadSemaphore);
}

/* Clear and upload a PSMT4 font bitmap under the GS semaphore, then free the staging allocation. */
void frFontUploadClearedTexture(void) {
    u8 loadImage[FR_FONT_IMAGE_DESCRIPTOR_BYTES];
    s32 imageSize;
    s32 allocation;
    void *pixels;

    imageSize = frFontWork.height * frFontWork.width;
    imageSize = (u32)imageSize >> 1;
    allocation = sdfAllocGeneralBlock(imageSize);
    pixels = (void *)sdfResourceRetainAddress(allocation);
    memset(pixels, 0, imageSize);
    sceGsSetDefLoadImage(loadImage, (s16)frFontWork.gsBuffer, (s16)frFontWork.gsFormat, FR_FONT_GS_PSMT4, 0, 0,
                         (s16)frFontWork.width, (s16)frFontWork.height);
    WaitSema(sdfGsImageUploadSemaphore);
    FlushCache(0);
    sceGsExecLoadImage(loadImage, (s32)pixels);
    sceGsSyncPath(0, 0);
    SignalSema(sdfGsImageUploadSemaphore);
    sdfReleaseResourceAllocation((void *)allocation);
}

extern void *sdfReadNamedResource();
extern void frFontBindResourceSections(u8, u8 *, void *);

/* Load only when the slot word is not exactly one; slot one borrows entry zero's allocation. */
void frFontEnsureSlotLoaded(s32 slotId, s32 path) {
    s32 slotIndex = slotId & FR_FONT_BYTE_MASK;
    FrFontSysLocal *fontSystem = &frFontWork;

    if (frFontSlotLoadedFlags[slotIndex] != FR_FONT_LOADED_STATE) {
        if (slotIndex == FR_FONT_SYSTEM_SLOT) {
            frFontBindResourceSections(FR_FONT_SYSTEM_SLOT, 0, fontSystem->entries[0].buffer);
        } else {
            frFontBindResourceSections(slotIndex, 0, sdfReadNamedResource(path, 0, 0));
        }
        frFontSlotLoadedFlags[slotIndex] = FR_FONT_LOADED_STATE;
    }
}

INCLUDE_ASM(const s32, "game/code_00193C08", func_00194228);

/* Release slot allocations, both glyph chains/rings, and the shared GS resource buffers. */
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

/* Bind sections from resource bytes or a retained allocation.
 * Optional flag/value sections each start with a byte-length word; their data
 * precedes the word table and remaining resource data. Offsets are unchecked.
 */
void frFontBindResourceSections(u8 slotIndex, u8 *resourceBytes, void *allocation) {
    FrFontEntry *entry;
    s32 sectionOffset;
    u32 lookupOffset;

    if (resourceBytes == NULL) {
        if (allocation != NULL) {
            resourceBytes = (u8 *)sdfResourceRetainAddress((s32)allocation);
        }
    }
    entry = &frFontWork.entries[slotIndex];
    entry->buffer = allocation;
    entry->resourceHeader = resourceBytes;
    sectionOffset = ((FrFontHeader *)resourceBytes)->tableOffset + (((FrFontHeader *)resourceBytes)->tableCount << FR_FONT_TABLE_ENTRY_SHIFT);
    if (((FrFontHeader *)resourceBytes)->hasExtra != 0) {
        s32 *flagSection = (s32 *)(resourceBytes + sectionOffset);
        s32 flagDataBytes = *flagSection;
        s32 *valueSection;
        s32 valueDataBytes;
        s32 flagSectionBytes;
        s32 valueSectionBytes;

        entry->flagBytes = (u8 *)(flagSection + 1);
        entry->unk8 = flagDataBytes;
        flagSectionBytes = flagDataBytes + FR_FONT_WORD_BYTES;
        sectionOffset += flagSectionBytes;
        valueSection = (s32 *)(resourceBytes + sectionOffset);
        valueDataBytes = *valueSection;
        entry->unk14 = valueSection + 1;
        entry->unkC = valueDataBytes;
        valueSectionBytes = valueDataBytes + FR_FONT_WORD_BYTES;
        sectionOffset += valueSectionBytes;
    } else {
        entry->flagBytes = NULL;
        entry->unk14 = NULL;
        entry->unk8 = 0;
        entry->unkC = 0;
    }
    entry->unk18 = resourceBytes + sectionOffset;
    sectionOffset += ((FrFontHeader *)entry->resourceHeader)->widthCount * FR_FONT_WORD_BYTES;
    entry->unk1C = resourceBytes + sectionOffset;
    lookupOffset = ((FrFontHeader *)entry->resourceHeader)->lookupOffset;
    if (lookupOffset != 0) {
        entry->unk20 = (u32)(resourceBytes + lookupOffset);
    } else {
        entry->unk20 = 0;
    }
}

/* Slots zero/one are not freed here; clear the data pointer only when an allocation exists. */
void frFontFreeEntry(s32 slotId) {
    u32 slotIndex = slotId & FR_FONT_BYTE_MASK;
    FrFontEntry *entry;

    if (slotIndex < FR_FONT_FIRST_RELEASABLE_SLOT) {
        return;
    }
    frFontSlotLoadedFlags[slotIndex] = 0;
    entry = &frFontWork.entries[slotIndex];
    if (entry->buffer != NULL) {
        sdfReleaseResourceAllocation(entry->buffer);
        entry->unk1C = NULL;
    }
    entry->unk18 = NULL;
}
