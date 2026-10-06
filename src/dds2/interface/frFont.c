#include "common.h"

typedef struct FrFontRecord {
    u16 id;      /* 0x00 */
    u8 unk02[2];
    u16 refs;    /* 0x04 */
    u8 unk06[2];
    void *list;  /* 0x08 */
} FrFontRecord;

typedef struct FntNode {
    void *unk0;
    s32 x;
    s32 y;
    u8 pad0C[8];
    FrFontRecord *item;
    struct FntNode *prev;
    struct FntNode *next;
} FntNode;

typedef struct FrFontValueRecord {
    u8 unk0[0xE];
    u16 glyphCount;
    u16 cellWidth;
    u16 cellHeight;
    u8 unk14[2];
    u8 useGlyphMetrics;
    u8 pad17;
} FrFontValueRecord;

typedef struct FrFontEntry {
    u8 unk00[4];
    FrFontValueRecord *valueRecord; /* 0x04 */
    s32 count;         /* 0x08 */
    u8 unk0C[4];
    s8 *table;       /* 0x10: paired signed glyph start/end metrics */
    u8 unk14[4];
    s32 *slots;        /* 0x18 */
    void *first;       /* 0x1C */
    u8 unk20[4];
} FrFontEntry; /* 0x24 */

extern u32 frFontMeasureGlyphChain(void *chain);

extern u32 frFontSharedRenderFlags;

extern u32 frFontContextCursorSpacing;

/* Glyph/record chain walked by func_001958A0/func_00195B78. */
typedef struct FrFontGlyph {
    union {
        s16 h;                        /* 0x0: halfword view */
        struct { s8 b0; s8 b1; } b;   /* 0x0: byte views */
    } u0;
    u16 unk2;         /* 0x2 */
    s32 x;            /* 0x4: horizontal position */
    s32 y;            /* 0x8: vertical position */
    s32 advance;      /* 0xC: advance shifted by four when linking glyphs */
    union {
        u32 word;     /* 0x10: word view */
        u16 half[2];  /* 0x10: halfword views */
        u8 byte[4];   /* 0x10: byte views */
    } u10;
    union {
        u32 w;        /* 0x14: word view */
        u8 b[4];      /* 0x14: byte views */
    } u14;
    union {
        u32 w;            /* 0x18: word view */
        u8 b[4];          /* 0x18: byte views */
    } unk18;
    struct FrFontGlyph *firstChild; /* 0x1C: child glyph chain */
    struct FrFontGlyph *unk20; /* 0x20 */
    struct FrFontGlyph *previous; /* 0x24: back-link in the glyph chain */
    struct FrFontGlyph *next; /* 0x28: next glyph in chain */
    struct FrFontGlyph *chainHead; /* 0x2C: first glyph in the linked chain */
    u32 unk30;        /* 0x30 */
    u32 unk34;        /* 0x34 */
    u32 unk38;        /* 0x38 */
    s32 unk3C;        /* 0x3C */
    s32 unk40;        /* 0x40 */
} FrFontGlyph;

extern FrFontGlyph *D_004528B4[];

extern u32 kwlnGetDrawBufferIndex(void);

extern s32 func_0019BA00(s32 x, s32 y, u8 width, u8 halfHeight, u8 style,
                         s32 flags, s32 color, s32 enabled, s32 sourceY,
                         void *table, s32 drawFlags);
extern u8 D_00452880[];
extern s32 D_0043656C;

extern FrFontGlyph *frFontLinkGlyph(FrFontGlyph *previous, FrFontGlyph *next, s32 positionNext);

/* Live font-counter view at frFontWork + 0x144; preserve its word types. */
typedef struct FrFontSave {
    u32 cachedItemCount; /* 0x0: retained font-cache records */
    u32 itemCount;       /* 0x4: pooled child glyphs */
    u32 glyphCount;      /* 0x8: pooled parent glyphs */
} FrFontSave;

extern FrFontSave D_00452864;

extern u8 frFontSharedGlyphFlags;

/* Record shared by the matched helpers below; offsets are from retail.
 * func_00195388 receives the message-window node itself (itfMesManager
 * func_0019DB40 passes its chain node straight in). */
typedef struct FrFontCtx {
    union {
        u32 word;            /* 0x0: whole word read by func_001963E0 */
        struct {
            u8 unk0;         /* 0x0 */
            u8 flag1;        /* 0x1: set by func_001953A8 */
            u8 unk2[2];      /* 0x2 */
        } bytes;
    } u0;
    u32 contextCursor;     /* 0x4: advanced by frFontAdvanceContextCursor */
    u32 unk8;                /* 0x8 */
    union {
        u32 w;                   /* 0xC: word view */
        struct { u8 pad; s8 bD; s8 bE; u8 bF; } b; /* 0xC: byte views */
    } uC;                        /* 0xC: refreshed by func_001953A8 */
    u32 unk10;               /* 0x10 */
    union {
        u32 shifted;         /* 0x14: value stored shifted by func_00195460 */
        void *ptr;           /* 0x14: child pointer read by func_001963E0 */
    } u14;
    u32 unk18;               /* 0x18 */
    s8 flag1C;               /* 0x1C */
    s8 flag1D;               /* 0x1D */
    u8 unk1E[0x22];          /* 0x1E */
    u32 mode40;              /* 0x40: set by func_00195388 */
} FrFontCtx;

extern void frFontSetContextEncodedByte(FrFontCtx *ctx, s32 inputValue);

extern void frFontSetContextPair(FrFontCtx *ctx, u32 first, u32 second);

void frFontCreateContext();

extern u8 D_00436578[];

typedef struct MemNode MemNode;
extern void *itfDequeueMemNode(MemNode *);

/* Font system at frFontWork (see game/code_0019B840.c); the two glyph slots
 * at +0x194/+0x198 are selected by func_00195B10. */
typedef struct FrFontSys {
    FrFontEntry entries[9];   /* 0x0 */
    s32 cachedItemCount;      /* 0x144: incremented on a new cache record */
    s32 itemCount;            /* 0x148 */
    s32 glyphCount;           /* 0x14C */
    MemNode *itemPool;        /* 0x150 */
    MemNode *glyphPool;       /* 0x154 */
    u8 unk158[8];
    s32 atlasBufferWidth;     /* 0x160 */
    u8 unk164[0xC];
    s32 atlasBase;            /* 0x170 */
    u8 unk174[4];
    s32 imageBuffers[6];      /* 0x178: GS upload destinations */
    u8 unk190[4];             /* 0x190 */
    FrFontGlyph *slots[2];    /* 0x194 */
} FrFontSys;

extern FrFontSys frFontWork;


extern s32 frFontDefaultGlyphCellSize;

extern FrFontGlyph *func_0019CE78(void *text, s8 fontIndex, s8 firstOption, s8 secondOption, FrFontGlyph *existingGlyph);

extern void *sdfAllocSizeClassBlock(s32 size);

typedef struct FrFontSegments {
    void *first;
    void *second;
    void *third;
} FrFontSegments;

extern void itfSplitRelativeSegments(void *block, FrFontSegments *out);
extern void func_001A00B8(void *dst, s32 option, void *block, FrFontSegments *segments);

extern FrFontGlyph *frFontReleaseGlyphChain(FrFontGlyph *glyph);

typedef struct TextStyleNode {
    u8 pad00[4];
    u32 x;
    u32 y;
    u8 pad0C[4];
    u32 color;
    u8 pad14[8];
    struct TextStyleNode *firstChild;
    u8 pad20[4];
    struct TextStyleNode *next;
    struct TextStyleNode *nextChild;
} TextStyleNode;

extern s32 frFontAdvanceGlyphFade(FrFontGlyph *glyph);

extern FrFontGlyph *frFontAppendGlyphReference(FrFontGlyph *source, FrFontGlyph *destination);
void frFontSetupGlyph(FrFontGlyph *, s32, s32, s32, s32, s32);
void frFontInitGlyph(FrFontGlyph *);
u32 frFontGetGlyphCellWidth(u8);
u32 frFontGetGlyphCellHeight(u8);

FrFontGlyph *frFontLinkGlyphAfterPrevious(FrFontGlyph *previous, FrFontGlyph *next);

void frFontDrawGlyphWithSharedFlags(FrFontGlyph *glyph, s8 mode);

extern s32 func_0019D550(FrFontGlyph *glyph, s8 mode, u32 flags);

extern void frFontEnsureSlotLoaded(s32, const char *);

#define FR_FONT_ENTRY_CAPACITY 9
#define FR_FONT_IMAGE_WORD_COUNT 16
#define FR_FONT_IMAGE_LAST_WORD 15
#define FR_FONT_IMAGE_BUFFER_COUNT 6
#define FR_FONT_IMAGE_WORD_BYTES 4
#define FR_FONT_PIXEL_FORMAT 0x14
#define FR_FONT_UPLOAD_BORROWED 0
#define FR_FONT_UPLOAD_OWNED 2
#define FR_FONT_ATLAS_POSITION_SHIFT 4
#define FR_FONT_ATLAS_DEST_SHIFT 6
#define FR_FONT_RESOURCE_CLONE_BYTES 0x120

#define FR_FONT_BYTE_MASK 0xFF
#define FR_FONT_CONTEXT_BYTE_THRESHOLD 0x81
#define FR_FONT_CONTEXT_BYTE_CAP (-0x80)
#define FR_FONT_CONTEXT_ENABLE_VALUE 0x80
#define FR_FONT_CONTEXT_VALUE_SHIFT 4
#define FR_FONT_FADE_OPTION 2
#define FR_FONT_FADE_VALUE_STEP 8
#define FR_FONT_FADE_Y_STEP 0x10
#define FR_FONT_CACHED_SLOT_STRIDE 4
#define FR_FONT_CACHED_SLOT_BASE 0x194
#define FR_FONT_POSITION_SHIFT 4
#define FR_FONT_TEMPORARY_SLOT 8
#define FR_FONT_CONTEXT_CURSOR_SCALE 8
#define FR_FONT_DEFAULT_CURSOR_SPACING 0x19
#define FR_FONT_DEFAULT_CELL_SLOT_COUNT 2
#define FR_FONT_POSITION_BAND_HEIGHT 0x64

/* Ensure the four named default font files are installed, in slot order. */
void frFontLoadDefaultFonts(void) {
    frFontEnsureSlotLoaded(0, "/font/font0.fnt");
    frFontEnsureSlotLoaded(1, "/font/font1.fnt");
    frFontEnsureSlotLoaded(2, "/font/font2.fnt");
    frFontEnsureSlotLoaded(3, "/font/font3.fnt");
}


/* Free each installed entry, including the temporary slot, without reloading. */
void frFontFreeAllEntries(void) {
    FrFontEntry *entries = (FrFontEntry *)&frFontWork;
    s32 fontIndex;

    for (fontIndex = 0; fontIndex < FR_FONT_ENTRY_CAPACITY; fontIndex++) {
        if (entries[fontIndex].first != NULL) {
            frFontFreeEntry(fontIndex & 0xFF);
        }
    }
}

extern u8 D_003B2DA8[];
/* This caller passes the full image-buffer word; the callee consumes its low half. */
extern void sdfUploadGsImageUnderSemaphore(s32 buffer, s32 image);

/* Assemble six 16-word image blocks from little-endian source bytes and upload
 * each to its matching GS buffer. The inner countdown includes zero. */
void func_0019C358(void) {
    u32 imageWords[FR_FONT_IMAGE_WORD_COUNT];
    s32 bufferIndex = 0;
    s32 sourceWordOffset = 0;
    FrFontSys *work = &frFontWork;
    s32 *imageBuffer = work->imageBuffers;
    u8 *sourceTable = D_003B2DA8;

    do {
        u32 *outputWord = imageWords;
        u8 *sourceBytes = (u8 *)(sourceWordOffset * FR_FONT_IMAGE_WORD_BYTES + (u32)sourceTable);
        s32 wordCountdown = FR_FONT_IMAGE_LAST_WORD;

        do {
            *outputWord = (((sourceBytes[3] << 8) | sourceBytes[2]) << 8 |
                       sourceBytes[1]) << 8 | sourceBytes[0];
            sourceBytes += FR_FONT_IMAGE_WORD_BYTES;
            outputWord++;
            wordCountdown--;
        } while (wordCountdown >= 0);

        sdfUploadGsImageUnderSemaphore(*imageBuffer++, (s32)imageWords);
        bufferIndex++;
        sourceWordOffset += FR_FONT_IMAGE_WORD_COUNT;
    } while (bufferIndex < FR_FONT_IMAGE_BUFFER_COUNT);
}

/* Remove an unreferenced cached item from its font table, return its resource
 * node to the list and decrement the live cache count. NULL item is a no-op. */
void frFontReleaseUnreferencedGlyphItem(FrFontGlyph *glyph) {
    FrFontRecord *cachedItem = (FrFontRecord *)glyph->firstChild;

    if (cachedItem != NULL) {
        if (cachedItem->refs == 0) {
            frFontWork.entries[glyph->u14.b[1]].slots[cachedItem->id] = 0;
            frFontListInsert(cachedItem->list);
            frFontWork.cachedItemCount--;
        }
    }
}

/* Retain the chain if any fade value changed; otherwise release it and return NULL. */
FrFontGlyph *frFontAdvanceOrRetainFadingGlyph(FrFontGlyph *glyph) {
    if (frFontAdvanceGlyphFade(glyph) != 0) {
        return glyph;
    }
    return frFontReleaseGlyphChain(glyph);
}

extern s32 itfEnqueueMemNode(void *node, MemNode *pool);

/* Release parents backward and children forward, returning both to their pools.
 * Children with unk20 set bypass cached-item reference release. Return NULL. */
FrFontGlyph *frFontReleaseGlyphChain(FrFontGlyph *glyph) {
    FrFontGlyph *currentGlyph = glyph;
    FrFontGlyph *childGlyph;
    FrFontGlyph *nextChildGlyph;
    FrFontGlyph *previousGlyph;

    if (currentGlyph == NULL) {
        return NULL;
    }
    do {
        childGlyph = currentGlyph->firstChild;
        while (childGlyph != NULL) {
            nextChildGlyph = childGlyph->next;
            if (childGlyph->unk20 == NULL) {
                ((FrFontRecord *)childGlyph->firstChild)->refs--;
                frFontReleaseUnreferencedGlyphItem(childGlyph);
            }
            itfEnqueueMemNode(childGlyph, frFontWork.itemPool);
            frFontWork.itemCount--;
            childGlyph = nextChildGlyph;
        }
        previousGlyph = currentGlyph->previous;
        itfEnqueueMemNode(currentGlyph, frFontWork.glyphPool);
        currentGlyph = previousGlyph;
        frFontWork.glyphCount--;
    } while (currentGlyph != NULL);
    return NULL;
}

/* Link without repositioning into the draw-buffer-indexed queue; return 0.
 * The low-byte buffer index is used directly, without a two-slot bounds check. */
s32 frFontQueueGlyphInSelectedSlot(FrFontGlyph *glyph) {
    FrFontGlyph **queueSlot = &D_004528B4[kwlnGetDrawBufferIndex() & 0xFF];

    *queueSlot = frFontLinkGlyph(*queueSlot, glyph, 0);
    return 0;
}

/* Return the live count of retained font-cache records. */
s32 func_0019C608(void) {
    return D_00452864.cachedItemCount;
}

/* Return the live pooled child-glyph count. */
s32 func_0019C618(void) {
    return D_00452864.itemCount;
}

/* Return the live pooled parent-glyph count. */
s32 func_0019C628(void) {
    return D_00452864.glyphCount;
}

u32 func_0019C638() {
    return 0;
}

INCLUDE_ASM(const s32, "interface/frFont", func_0019C640);

extern s8 D_003B2DA0[];

/* Append a source-glyph reference, allocating a chain head when absent. */
FrFontGlyph *frFontAppendGlyphReference(FrFontGlyph *source, FrFontGlyph *destination) {
    FrFontGlyph *glyph;
    FrFontGlyph *previous;

    if (destination == NULL) {
        destination = itfDequeueMemNode(frFontWork.glyphPool);
        frFontWork.glyphCount++;
        frFontInitGlyph(destination);
    }
    glyph = itfDequeueMemNode(frFontWork.itemPool);
    previous = destination->unk20;
    frFontWork.itemCount++;
    frFontSetupGlyph(glyph, 0, 0, 0, 0xA09DC300, 0);
    if (previous == NULL) {
        destination->firstChild = glyph;
    } else {
        previous->next = glyph;
    }
    glyph->unk20 = source;
    glyph->advance = D_003B2DA0[0];
    glyph->unk18.b[0] = frFontGetGlyphCellWidth(0);
    glyph->unk18.b[1] = frFontGetGlyphCellHeight(0);
    glyph->previous = previous;
    destination->unk20 = glyph;
    destination->unk18.w++;
    destination->advance += glyph->advance;
    destination->u10.half[0] = glyph->unk18.b[0];
    destination->u10.half[1] = glyph->unk18.b[1];
    return destination;
}

/* Append a source-glyph reference wrapper, not a deep copy of source storage.
 * If wrapper creation returns NULL, preserve the original destination. */
FrFontGlyph *frFontAppendClonedGlyph(FrFontGlyph *source, FrFontGlyph *destination) {
    FrFontGlyph *referenceChain = frFontAppendGlyphReference(source, 0);

    if (referenceChain == NULL) {
        return destination;
    }
    return frFontLinkGlyphAfterPrevious(destination, referenceChain);
}

typedef struct SdfImageUploadRequest {
    void *pixels;
    s32 allocation;
    u8 allocationMode;
    u8 format;
    u16 bufferWidth;
    u32 destination;
    u16 x;
    u16 y;
    u16 width;
    u16 height;
} SdfImageUploadRequest;

extern FntNode *frFontDetachFirstResourceNode(void);
extern void func_0032AC30(SdfImageUploadRequest *);

/* Retain one atlas item and submit its pixel upload with the requested ownership
 * mode. Coordinates are converted from sixteenths; pool exhaustion is unchecked. */
FrFontRecord *frFontCreateAtlasItem(s32 width, s32 height, void *pixels, s8 ownsPixels) {
    s32 imagePosition[2];
    SdfImageUploadRequest uploadRequest;
    FntNode *atlasNode;
    FrFontRecord *cachedItem;

    atlasNode = frFontDetachFirstResourceNode();
    cachedItem = atlasNode->item;
    /* Convert fixed-point atlas placement to integer image coordinates. */
    imagePosition[0] = atlasNode->x >> FR_FONT_ATLAS_POSITION_SHIFT;
    imagePosition[1] = atlasNode->y >> FR_FONT_ATLAS_POSITION_SHIFT;
    cachedItem->refs = 1;
    cachedItem->list = atlasNode;
    if (ownsPixels == 0) {
        uploadRequest.allocationMode = FR_FONT_UPLOAD_BORROWED;
    } else {
        uploadRequest.allocationMode = FR_FONT_UPLOAD_OWNED;
    }
    uploadRequest.pixels = pixels;
    uploadRequest.format = FR_FONT_PIXEL_FORMAT;
    uploadRequest.width = width;
    uploadRequest.height = height;
    uploadRequest.bufferWidth = frFontWork.atlasBufferWidth;
    uploadRequest.destination = frFontWork.atlasBase << FR_FONT_ATLAS_DEST_SHIFT;
    uploadRequest.x = imagePosition[0];
    uploadRequest.y = imagePosition[1];
    func_0032AC30(&uploadRequest);
    return cachedItem;
}

/* Allocate a fixed-size clone and process the selected font resource's segments.
 * The processing option remains opaque; neither allocation nor index is checked. */
void *frFontCloneEntryResource(u8 fontIndex, s32 resourceOption) {
    FrFontEntry *entry = &frFontWork.entries[fontIndex];
    void *clonedResource = sdfAllocSizeClassBlock(FR_FONT_RESOURCE_CLONE_BYTES);
    FrFontSegments segments;

    itfSplitRelativeSegments(entry->first, &segments);
    func_001A00B8(clonedResource, resourceOption, entry->first, &segments);
    return clonedResource;
}

extern u16 frFontGetSlotCellWidth(s32 index);
extern u16 frFontGetSlotCellHeight(s32 index);

/* Retain a cached table item, or create/upload a new one and cache it.
 * New records start with one reference; itemIndex is not bounds-checked. */
FrFontRecord *frFontRetainOrCreateCachedItem(FrFontGlyph *glyph, s32 itemIndex) {
    FrFontEntry *entry = &frFontWork.entries[glyph->u14.b[1]];
    FrFontRecord *cachedItem = ((FrFontRecord **)entry->slots)[itemIndex];
    void *clonedResource;

    if (cachedItem != NULL) {
        cachedItem->refs++;
        return cachedItem;
    }
    clonedResource = frFontCloneEntryResource(glyph->u14.b[1], itemIndex);
    cachedItem = frFontCreateAtlasItem(frFontGetSlotCellWidth(glyph->u14.b[1]), frFontGetSlotCellHeight(glyph->u14.b[1]), clonedResource, 1);
    cachedItem->id = itemIndex;
    ((FrFontRecord **)entry->slots)[itemIndex] = cachedItem;
    frFontWork.cachedItemCount++;
    return cachedItem;
}

/* Store the font/options, narrowed glyph code and high flag bits, then clear
 * positioning/links. Other glyph fields are deliberately not fully reset here. */
void frFontSetupGlyph(FrFontGlyph *glyph, s32 glyphId, s32 fontIndex, s32 firstOption, s32 packedFlags, s32 secondOption) {
    glyph->u14.b[1] = fontIndex;
    glyph->u14.b[0] = firstOption;
    glyph->u14.b[2] = secondOption;
    glyph->u0.h = glyphId;
    glyph->u10.word = packedFlags & ~FR_FONT_BYTE_MASK;
    glyph->u14.b[3] = frFontSharedGlyphFlags;
    glyph->x = 0;
    glyph->y = 0;
    glyph->advance = 0;
    glyph->unk2 = 0;
    glyph->firstChild = NULL;
    glyph->unk20 = NULL;
    glyph->previous = NULL;
    glyph->next = NULL;
}

/* Reset a standalone chain head: code byte 0x80, no children and zero counters. */
void frFontInitGlyph(FrFontGlyph *glyph) {
    glyph->u0.b.b0 = -0x80;
    glyph->x = 0;
    glyph->y = 0;
    glyph->u0.b.b1 = 0;
    glyph->advance = 0;
    glyph->u14.w = 0;
    glyph->previous = NULL;
    glyph->next = NULL;
    glyph->chainHead = glyph;
    glyph->firstChild = NULL;
    glyph->unk20 = NULL;
    glyph->unk18.w = 0;
    glyph->unk30 = 0;
    glyph->unk34 = 0;
    glyph->unk38 = 0;
    glyph->unk3C = 0;
    glyph->unk40 = 0;
}

extern void func_0019C640(void *, s32);

FrFontGlyph *func_0019CCC0(u16 glyphId, s32 fontIndexArg, u8 firstOption, u8 secondOption) {
    FrFontGlyph *glyph;
    s32 glyphIndex;
    s32 fontIndex = fontIndexArg & 0xFF;

    glyph = itfDequeueMemNode(frFontWork.itemPool);
    frFontWork.itemCount++;
    frFontSetupGlyph(glyph, glyphId, fontIndex, firstOption, 0xA09DC300, secondOption);

    if ((u16)glyph->u0.h < 0x80) {
        glyphIndex = (u16)glyph->u0.h - 0x20;
    } else {
        s32 adjusted = (u16)glyph->u0.h - 0x8080;

        glyphIndex = ((adjusted & 0xFF00) >> 1) + (adjusted & 0x7F);
    }
    if (glyphIndex >=
        ((FrFontEntry *)((u8 *)&frFontWork + fontIndex * sizeof(FrFontEntry)))->valueRecord->glyphCount) {
        glyphIndex = 0x147;
    }
    glyph->firstChild = (FrFontGlyph *)frFontRetainOrCreateCachedItem(glyph, glyphIndex);
    func_0019C640(glyph, glyphIndex);
    if (glyph->u14.b[3] & 0x10) {
        glyph->y = func_0019C638((s8)fontIndex, glyphIndex);
    } else {
        glyph->y = 0;
    }
    return glyph;
}

/* Build text and position its chain after the previous glyph. A NULL build
 * result preserves the existing integer-address return convention. */
FrFontCtx *frFontAppendGlyphFromData(void *text, s8 fontIndex, s8 firstOption, s8 secondOption, s32 previousGlyphAddress) {
    FrFontGlyph *newGlyphChain = func_0019CE78(text, fontIndex, firstOption, secondOption, 0);

    if (newGlyphChain == NULL) {
        return (FrFontCtx *)previousGlyphAddress;
    }
    return (FrFontCtx *)frFontLinkGlyphAfterPrevious((FrFontGlyph *)previousGlyphAddress, newGlyphChain);
}

INCLUDE_ASM(const s32, "interface/frFont", func_0019CE78);

/* Double the masked input byte and saturate the stored byte at 0x80.
 * Keep the original signed constant used for the saturated byte assignment. */
void frFontSetContextEncodedByte(FrFontCtx *ctx, s32 inputValue) {
    s32 encodedValue = (inputValue & FR_FONT_BYTE_MASK) * 2;

    if (encodedValue >= FR_FONT_CONTEXT_BYTE_THRESHOLD) {
        ctx->u0.bytes.unk0 = FR_FONT_CONTEXT_BYTE_CAP;
    } else {
        ctx->u0.bytes.unk0 = encodedValue;
    }
}

/* Mark context mode enabled and initialize its encoded byte to the cap. */
void frFontEnableContextMode(FrFontCtx *ctx) {
    ctx->mode40 = 1;
    frFontSetContextEncodedByte(ctx, FR_FONT_CONTEXT_ENABLE_VALUE);
}

/* Store the requested flag byte and refresh the cached child-chain advance. */
void frFontSetFlagAndMeasureGlyphs(FrFontCtx *ctx, u8 requestedFlag) {
    u32 measuredAdvance;

    ctx->u0.bytes.flag1 = requestedFlag;
    measuredAdvance = frFontMeasureGlyphChain(ctx);
    ctx->uC.w = measuredAdvance;
}

/* Set the initial glyph's halfword dimensions and every visited child's advance/
 * byte dimensions, then remeasure only the initial glyph. Input must be non-NULL. */
void frFontSetGlyphChainDimensions(FrFontGlyph *glyph, s32 cellAdvance, s32 cellHeight) {
    FrFontGlyph *targetGlyph = glyph;
    FrFontGlyph *childGlyph;

    targetGlyph->u10.half[0] = cellAdvance;
    targetGlyph->u10.half[1] = cellHeight;
    for (; glyph != NULL; glyph = glyph->previous) {
        for (childGlyph = glyph->firstChild; childGlyph != NULL; childGlyph = childGlyph->next) {
            childGlyph->advance = cellAdvance;
            childGlyph->unk18.b[0] = cellAdvance;
            childGlyph->unk18.b[1] = cellHeight;
        }
    }
    targetGlyph->advance = frFontMeasureGlyphChain(targetGlyph);
}

/* Store the ordered pair without scaling; the second field remains opaque. */
void frFontSetContextPair(FrFontCtx *ctx, u32 first, u32 second) {
    ctx->contextCursor = first;
    ctx->unk8 = second;
}

/* Store an unsigned value divided by 16; do not reinterpret its union view. */
void frFontStoreShiftedContextValue(FrFontCtx *ctx, u32 unshiftedValue) {
    ctx->u14.shifted = unshiftedValue >> FR_FONT_CONTEXT_VALUE_SHIFT;
}

/* Assign the first option byte across all visited children; NULL is a no-op. */
void frFontSetChainFlag(FrFontGlyph *glyph, u8 flagValue) {
    FrFontGlyph *childGlyph;

    for (; glyph != NULL; glyph = glyph->previous) {
        for (childGlyph = glyph->firstChild; childGlyph != NULL; childGlyph = childGlyph->next) {
            childGlyph->u14.b[0] = flagValue;
        }
    }
}

/* Replace each child's full color word across the style-node chain. */
void frFontSetChildColors(TextStyleNode *parentNode, u32 colorWord) {
    for (; parentNode != NULL; parentNode = parentNode->next) {
        TextStyleNode *childNode;
        for (childNode = parentNode->firstChild; childNode != NULL; childNode = childNode->nextChild) {
            childNode->color = colorWord;
        }
    }
}

/* OR the requested mask into the shared byte, preserving implicit narrowing. */
void frFontAddSharedGlyphFlags(s32 mask) {
    mask |= frFontSharedGlyphFlags;
    frFontSharedGlyphFlags = mask;
}

/* Clear flag bits from the shared font flag byte; returns the previous value. */
u8 frFontClearFlagBits(u8 mask) {
    u8 previousFlags = frFontSharedGlyphFlags;

    frFontSharedGlyphFlags = previousFlags & ~mask;
    return previousFlags;
}

/* Replace the complete shared render-flag word used by subsequent drawing. */
void frFontSetSharedRenderFlags(u32 renderFlags) {
    frFontSharedRenderFlags = renderFlags;
}

/* Step only second-option-2 children: decrease the low byte by eight, clamp at
 * zero and move y by sixteen while changing it. Return whether any step occurred,
 * not whether the resulting chain still has a nonzero fade value. */
s32 frFontAdvanceGlyphFade(FrFontGlyph *glyph) {
    FrFontGlyph *childGlyph;
    s32 didChange = 0;

    for (; glyph != NULL; glyph = glyph->previous) {
        for (childGlyph = glyph->firstChild; childGlyph != NULL; childGlyph = childGlyph->next) {
            if (childGlyph->u14.b[2] == FR_FONT_FADE_OPTION) {
                u32 glyphWord = childGlyph->u10.word;
                s32 fadeValue = glyphWord & FR_FONT_BYTE_MASK;

                if (fadeValue != 0) {
                    fadeValue -= FR_FONT_FADE_VALUE_STEP;
                    if (fadeValue < 0) {
                        fadeValue = 0;
                    }
                    childGlyph->y += FR_FONT_FADE_Y_STEP;
                    childGlyph->u10.word = (glyphWord & ~FR_FONT_BYTE_MASK) | fadeValue;
                    didChange = 1;
                }
            }
        }
    }
    return didChange;
}

/* Advance one rendered glyph's fade state and return its transient X jitter. */
s32 func_0019D288(FrFontGlyph *parent, FrFontGlyph *glyph, u8 threshold, u8 step) {
    FrFontGlyph *previousParent;
    FrFontGlyph *previousGlyph;
    u32 *fadeWord;
    u8 previousOption;
    u8 fade = 0x80;
    s32 ready = 0;
    s32 canStopLips = 1;
    s32 jitter = 0;

    previousGlyph = glyph->previous;
    previousOption = 0;
    if (previousGlyph == NULL) {
        previousParent = parent->previous;
        if (previousParent != NULL && previousParent->unk20 != NULL) {
            fade = previousParent->unk20->u10.byte[0];
            previousOption = previousParent->unk20->u14.b[2];
        }
    } else {
        fade = previousGlyph->u10.byte[0];
        previousParent = parent->previous;
    }

    fadeWord = &glyph->u10.word;
    if (previousOption == 0 || previousOption == glyph->u14.b[2]) {
        if (fade >= threshold) {
            ready = 1;
        }
    } else {
        ready = fade == 0x80;
    }

    if (previousParent != NULL && parent->firstChild == glyph && parent->unk40 == 0) {
        switch (previousParent->unk30) {
        case 0xF214:
            if (previousParent->unk3C > 0) {
                ready = 0;
                if (fade == 0x80) {
                    previousParent->unk3C = previousParent->unk3C - 1;
                }
            }
            break;
        case 0xF215:
            if (previousParent->unk3C != 0xFFFF) {
                if (previousParent->unk3C > 0) {
                    previousParent->unk3C--;
                    ready = 0;
                }
            } else if (mnuQueryTitleSoundBusy() != 0) {
                ready = 0;
            }
            break;
        }
    }

    if (parent->unk34 == 0xF117 && parent->unk20->u10.byte[0] == 0x80) {
        if (parent->unk38 == 0 && parent->unk3C > 0) {
            if (parent->unk3C == 0xFFFF) {
                if (mnuQueryTitleSoundBusy() != 0) {
                    canStopLips = 0;
                }
            } else {
                canStopLips = 0;
            }
        }
        if (canStopLips != 0) {
            evtLipsStopFunction();
            parent->unk34 = 0;
        }
    }

    if (ready != 0) {
        u32 word;

        if ((s8)step >= 0) {
            step = (step * 22) / (glyph->advance + parent->u0.b.b1);
        }
        word = *fadeWord;
        if ((u32)(0x80 - (word & 0xFF)) >= step) {
            *fadeWord = word + step;
        } else {
            *fadeWord = (word & ~0xFF) | 0x80;
        }
    }

    {
        s8 currentFade = fadeWord[0];
        u16 glyphValue = glyph->unk2;

        if (currentFade >= 0) {
            if (glyph->u14.b[2] == 1) {
                jitter = -((((glyphValue * 2) % 5) - 2) << 4);
            }
        }
    }
    return jitter;
}

/* Draw through the shared render flags with mode zero. */
void frFontDrawGlyphInDefaultMode(FrFontGlyph *glyph) {
    frFontDrawGlyphWithSharedFlags(glyph, 0);
}

/* Pass the selected mode and current shared render-flag word to the renderer. */
void frFontDrawGlyphWithSharedFlags(FrFontGlyph *glyph, s8 mode) {
    func_0019D550(glyph, mode, frFontSharedRenderFlags);
}

/* Draw every child glyph, advance each parent chain, and report its measured
 * width only while all control/fade conditions remain ready. DDS2 additionally
 * suppresses one result per positive draw-delay count. */
s32 func_0019D550(FrFontGlyph *glyph, s8 mode, u32 flags) {
    FrFontGlyph *child;
    s32 ready = 1;
    s32 enabled = 1;
    s32 result = 0;
    s32 totalAdvance = 0;
    s32 titleSoundBusy;
    u8 *table;

    if (glyph != NULL) {
        glyph = glyph->chainHead;
        if (glyph != NULL) {
            do {
                s32 x = glyph->x;
                s32 y = glyph->y;
                s8 spacing = glyph->u0.b.b1;

                child = glyph->firstChild;
                if (child != NULL) {
                    table = D_00452880;
                    do {
                        s32 xOffset = mode != 0 ? 0 :
                            func_0019D288(glyph, child, 0x1C, (u8)glyph->u0.b.b0);
                        u32 glyphState;
                        if (child->unk20 == NULL) {
                            if ((u16)child->u0.h < 0x80) {
                                enabled = 1;
                            }
                            func_0019BA00(x + child->x + xOffset, y + child->y,
                                         child->unk18.b[0], child->unk18.b[1] >> 1,
                                         child->u14.b[0], child->u10.word,
                                         glyph->u14.w, enabled,
                                         child->firstChild->y + 4,
                                         table, flags);
                        } else {
                            func_0019BA00(x + child->x + xOffset, y + child->y,
                                         child->unk18.b[0], child->unk18.b[1] >> 1,
                                         child->u14.b[0], child->u10.word,
                                         glyph->u14.w, enabled,
                                         child->unk20->y + 4,
                                         table, flags);
                        }
                        glyphState = child->u10.byte[0];
                        if (glyphState != 0) {
                            child->unk2++;
                        }
                        if (glyphState < 0x80) {
                            ready = 0;
                        }
                        x += (child->advance + spacing) << 4;
                        child = child->next;
                    } while (child != NULL);
                }

                if (glyph->next == NULL && glyph->unk40 == 0) {
                    switch (glyph->unk30) {
                    case 0xF214:
                        if (ready != 0 && glyph->unk3C > 0) {
                            glyph->unk3C--;
                            ready = 0;
                        }
                        break;
                    case 0xF215:
                        if (glyph->unk3C != 0xFFFF) {
                            if (glyph->unk3C > 0) {
                                glyph->unk3C--;
                                ready = 0;
                            }
                        } else {
                            titleSoundBusy = mnuQueryTitleSoundBusy();
                            if (titleSoundBusy != 0) {
                                ready = 0;
                            }
                        }
                        break;
                    }
                }
                totalAdvance += glyph->unk18.w;
                glyph = glyph->next;
            } while (glyph != NULL);
        }
        if (D_0043656C > 0) {
            D_0043656C--;
            return 0;
        }
        result = totalAdvance;
        if (ready == 0) {
            result = 0;
        }
    }
    return result;
}

/* Release queue slot 1 when the draw-buffer index's low byte is zero, otherwise
 * slot 0; return 0. This is buffer selection, not a current-font selection. */
/* Keep byte-base arithmetic: indexing FrFontSys.slots changes ee-gcc codegen. */
s32 frFontAdvanceSelectedGlyphSlot(void) {
    s32 queueIndex = (kwlnGetDrawBufferIndex() & FR_FONT_BYTE_MASK) == 0;
    u8 *workBytes = (u8 *)&frFontWork;
    FrFontGlyph **queueSlot = (FrFontGlyph **)(workBytes + queueIndex * FR_FONT_CACHED_SLOT_STRIDE + FR_FONT_CACHED_SLOT_BASE);

    *queueSlot = frFontReleaseGlyphChain(*queueSlot);
    return 0;
}

/* Link chains with the renderer's relative-positioning option enabled. */
FrFontGlyph *frFontLinkGlyphAfterPrevious(FrFontGlyph *previous, FrFontGlyph *next) {
    return frFontLinkGlyph(previous, next, 1);
}

/* Splice chains and return the next chain endpoint. Only option 1 repositions
 * that endpoint; other nonzero options do not. NULL inputs pass through. */
FrFontGlyph *frFontLinkGlyph(FrFontGlyph *previous, FrFontGlyph *next, s32 positionNext) {
    if (previous == NULL) {
        return next;
    }
    if (next == NULL) {
        return previous;
    }
    previous->next = next->chainHead;
    next->chainHead->previous = previous;
    next->chainHead = previous->chainHead;
    if (positionNext == 1) {
        next->x = previous->x + (previous->advance << FR_FONT_POSITION_SHIFT);
        next->y = previous->y;
    }
    return next;
}

/* Bind the caller's font data into the reserved temporary entry. */
void frFontLoadTemporaryEntry(u32 fontData) {
    frFontBindResourceSections(FR_FONT_TEMPORARY_SLOT, fontData, 0);
}

/* Free the reserved temporary font entry. */
void frFontFreeTemporaryEntry(void) {
    frFontFreeEntry(FR_FONT_TEMPORARY_SLOT);
}

/* Count characters until a zero lead byte. Any negative signed lead consumes
 * two bytes without checking its trailer; input must be a valid packed stream. */
s32 frFontCountChars(s8 *text) {
    s32 characterCount = 0;

    while (*text != 0) {
        if (*text >= 0) {
            text++;
        } else {
            text += 2;
        }
        characterCount++;
    }
    return characterCount;
}

/* Sum one parent's child advances and signed spacing after every child, even
 * the last. Require a non-NULL parent; preserve the existing unsigned result. */
u32 frFontMeasureGlyphChain(void *chain) {
    FrFontGlyph *parentGlyph = chain;
    FrFontGlyph *childGlyph = parentGlyph->firstChild;
    s32 totalAdvance = 0;

    if (childGlyph != NULL) {
        s8 childSpacing = parentGlyph->u0.b.b1;

        do {
            totalAdvance += childGlyph->advance;
            childGlyph = childGlyph->next;
            totalAdvance += childSpacing;
        } while (childGlyph != NULL);
    }
    return totalAdvance;
}

/* Sum all child advances/spacing from the chain origin. This is neither a line
 * count nor a maximum line width; require a non-NULL glyphChain. */
u32 frFontMeasureLines(FrFontGlyph *glyphChain) {
    FrFontGlyph *parentGlyph;
    FrFontGlyph *childGlyph;
    s32 totalAdvance = 0;

    for (parentGlyph = glyphChain->chainHead; parentGlyph != NULL; parentGlyph = parentGlyph->next) {
        childGlyph = parentGlyph->firstChild;
        if (childGlyph != NULL) {
            s8 childSpacing = parentGlyph->u0.b.b1;

            do {
                totalAdvance += childGlyph->advance;
                childGlyph = childGlyph->next;
                totalAdvance += childSpacing;
            } while (childGlyph != NULL);
        }
    }
    return totalAdvance;
}

typedef struct FrFontGlyphMeasureWork {
    u16 code;
    u8 pad02[0xA];
    s32 advance;
    u8 pad10[5];
    u8 fontIndex;
    u8 pad16;
    u8 mode;
    u8 cellWidth;
    u8 cellHeight;
    u8 pad1A[0x16];
} FrFontGlyphMeasureWork;

extern void func_0019C640(void *work, s32 code);

s32 func_0019D9A8(const u8 *text, u8 fontIndex, u8 mode) {
    FrFontGlyphMeasureWork work;
    s32 total = 0;
    u32 offset = 0;
    u32 length;

    work.fontIndex = fontIndex;
    work.mode = mode;
    work.advance = 0;
    work.cellWidth = frFontGetGlyphCellWidth(work.fontIndex);
    work.cellHeight = frFontGetGlyphCellHeight(work.fontIndex);
    length = strlen((const char *)text);

    while (offset < length) {
        u32 code = text[offset];
        s32 glyphIndex;

        if (code >= 0x80) {
            offset++;
            code = (code << 8) | text[offset];
        }
        work.code = code;
        if (code < 0x80) {
            glyphIndex = code - 0x20;
        } else {
            u32 adjusted = code - 0x8080;
            glyphIndex = ((adjusted & 0xFF00) >> 1) + (adjusted & 0x7F);
        }
        func_0019C640(&work, glyphIndex);
        offset++;
        total += work.advance;
    }
    return total;
}

/* Slots 0/1 use the shared default size; later slots read their resource width.
 * Retain the nested signed check and the absence of upper-bound validation. */
u32 frFontGetGlyphCellWidth(u8 fontIndex) {
    s32 entryIndex = fontIndex;

    if (entryIndex < FR_FONT_DEFAULT_CELL_SLOT_COUNT) {
        if (entryIndex >= 0) {
            return frFontDefaultGlyphCellSize;
        }
    }
    return frFontWork.entries[entryIndex].valueRecord->cellWidth;
}

/* Slots 0/1 use the shared default size; later slots read their resource height.
 * Retain the nested signed check and the absence of upper-bound validation. */
u32 frFontGetGlyphCellHeight(u8 fontIndex) {
    s32 entryIndex = fontIndex;

    if (entryIndex < FR_FONT_DEFAULT_CELL_SLOT_COUNT) {
        if (entryIndex >= 0) {
            return frFontDefaultGlyphCellSize;
        }
    }
    return frFontWork.entries[entryIndex].valueRecord->cellHeight;
}

/* Restore this game's cursor spacing (DDS1: 21, DDS2: 25). */
void frFontResetContextCursorSpacing(void) {
    frFontContextCursorSpacing = FR_FONT_DEFAULT_CURSOR_SPACING;
}

/* Store unscaled cursor spacing; advancement applies its factor of eight later. */
void frFontSetContextCursorSpacing(u32 spacing) {
    frFontContextCursorSpacing = spacing;
}

INCLUDE_ASM(const s32, "interface/frFont", func_0019DB30);

INCLUDE_ASM(const s32, "interface/frFont", func_0019DBA8);

typedef struct FrFontPosition {
    s32 x;
    s32 y;
} FrFontPosition;

/* Write each visited position and stop at requestedIndex. Missing indices leave
 * the last visited position; empty input leaves output unchanged. DDS2 advances
 * to the first node outside its y-band, unlike DDS1's single-node advance. */
void func_0019DC68(FrFontPosition *position, s32 requestedIndex, FrFontGlyph *glyph) {
    s32 positionIndex = 0;
    FrFontGlyph *currentNode;
    FrFontGlyph *scanNode;
    s32 bandLimit;
    s32 currentY;

    if (glyph != NULL) {
        currentNode = glyph->chainHead;
        if (currentNode != NULL) {
            do {
                scanNode = currentNode;
                position->x = currentNode->x;
                position->y = currentNode->y;
                currentY = currentNode->y;
                if (currentNode != NULL) {
                    bandLimit = currentY + FR_FONT_POSITION_BAND_HEIGHT;
                    if (currentY < bandLimit) {
                        do {
                            scanNode = scanNode->next;
                        } while (scanNode != NULL && scanNode->y < bandLimit);
                    }
                }
                if (positionIndex++ == requestedIndex) {
                    break;
                }
                currentNode = scanNode;
            } while (currentNode != NULL);
        }
    }
}

/* Translate the complete next-linked chain to place its origin at x/y.
 * NULL glyphChain is a no-op; a non-NULL chain requires a non-NULL origin. */
void frFontMoveChainTo(s32 x, s32 y, FrFontGlyph *glyphChain) {
    FrFontGlyph *currentGlyph;
    s32 dx;
    s32 dy;

    if (glyphChain != NULL) {
        currentGlyph = glyphChain->chainHead;
        dx = x - currentGlyph->x;
        dy = y - currentGlyph->y;
        for (; currentGlyph != NULL; currentGlyph = currentGlyph->next) {
            currentGlyph->x += dx;
            currentGlyph->y += dy;
        }
    }
}

INCLUDE_ASM(const s32, "interface/frFont", func_0019DD48);

INCLUDE_ASM(const s32, "interface/frFont", func_0019DE70);

INCLUDE_ASM(const s32, "interface/frFont", func_0019DEE0);

/* Append the shared-text child context, apply its cached encoded byte and clear
 * the pending-create flag. Preserve the old-style implicit-argument interface. */
/* Old-style definition: callers invoke it without arguments and rely on $a0. */
void frFontCreateContext(ctx)
    FrFontCtx *ctx;

{
    FrFontCtx *childContext = frFontAppendGlyphFromData(&D_00436578, 0, ctx->uC.b.bD, ctx->uC.b.bE, ctx->u14.shifted);

    ctx->u14.ptr = childContext;
    frFontSetContextEncodedByte(childContext, ctx->uC.b.bF);
    ctx->flag1C = 0;
}

/* Cancel creation for an empty child, process pending creation, then process
 * the independent pair refresh. The refresh phase has no NULL-child guard. */
void frFontCheckPendingGlyphState(FrFontCtx *ctx) {
    s8 pending;

    if (ctx->u14.ptr == NULL) {
        pending = ctx->flag1C;
    }
    else {
        if (((FrFontGlyph *)ctx->u14.ptr)->firstChild == NULL) {
            ctx->flag1C = 0;
        }
        pending = ctx->flag1C;
    }
    if (pending == '\0') {
        pending = ctx->flag1D;
    }
    else {
        frFontCreateContext();
        pending = ctx->flag1D;
    }
    if (pending != '\0') {
        frFontSetContextPair(ctx->u14.ptr, ctx->u0.word, ctx->contextCursor);
        ctx->flag1D = 0;
    }
}

/* Advance by eight times cursor spacing and request both context updates. */
void frFontAdvanceContextCursor(FrFontCtx *ctx) {
    ctx->contextCursor += frFontContextCursorSpacing * FR_FONT_CONTEXT_CURSOR_SCALE;
    ctx->flag1C = 1;
    ctx->flag1D = 1;
}

INCLUDE_SDATA(const s32, "interface/frFont", D_00436550);

INCLUDE_SDATA(const s32, "interface/frFont", frFontContextCursorSpacing);

INCLUDE_SDATA(const s32, "interface/frFont", frFontDefaultGlyphCellSize);

INCLUDE_SDATA(const s32, "interface/frFont", D_0043655C);

INCLUDE_SDATA(const s32, "interface/frFont", D_00436560);

INCLUDE_SDATA(const s32, "interface/frFont", frFontSharedGlyphFlags);

INCLUDE_SDATA(const s32, "interface/frFont", frFontSharedRenderFlags);

INCLUDE_SDATA(const s32, "interface/frFont", D_0043656C);

INCLUDE_SDATA(const s32, "interface/frFont", D_00436570);

INCLUDE_SDATA(const s32, "interface/frFont", D_00436578);

INCLUDE_SDATA(const s32, "interface/frFont", D_00436580);

