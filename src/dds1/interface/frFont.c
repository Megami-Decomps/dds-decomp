#include "common.h"
#include "itf.h"



extern u32 frFontContextCursorSpacing;

extern u8 frFontSharedGlyphFlags;

extern u32 frFontSharedRenderFlags;

extern u8 D_003BB180[];


extern s32 frFontDefaultGlyphCellSize;


extern u32 frFontMeasureGlyphChain(FrFontGlyph *parentGlyph);


void frFontCreateContext();


extern s32 kwlnGetDrawBufferIndex(void);

extern s32 frFontAdvanceGlyphFade(FrFontGlyph *glyph);


extern FrFontGlyph *frFontReleaseGlyphChain(FrFontGlyph *glyph);

typedef struct MemNode MemNode;
extern void *itfDequeueMemNode(MemNode *queue);

extern FrFontGlyph *frFontAppendGlyphReference(FrFontRecord *source, FrFontGlyph *destination);
extern void frFontSetupGlyph(FrFontGlyph *glyph, s32 glyphId, s32 fontIndex, s32 firstOption, s32 packedFlags, s32 secondOption);
extern void frFontInitGlyph(FrFontGlyph *glyph);
extern u32 frFontGetGlyphCellWidth(u8 fontIndex);
extern u32 frFontGetGlyphCellHeight(u8 fontIndex);
extern s8 D_00356470[];

extern FrFontGlyph *frFontLinkGlyph(FrFontGlyph *previous, FrFontGlyph *next, s32 positionNext);

extern s32 func_001958A0(FrFontGlyph *glyph, s8 mode, u32 flags);


extern FrFontGlyph *func_001951C8(void *text, s8 fontIndex, s8 firstOption, s8 secondOption, FrFontGlyph *existingGlyph);


FrFontGlyph *frFontLinkGlyphAfterPrevious(FrFontGlyph *previous, FrFontGlyph *next);


extern void frFontEnsureSlotLoaded(s32 id, const char *path);

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
#define FR_FONT_POSITION_SHIFT 4
#define FR_FONT_TEMPORARY_SLOT 8
#define FR_FONT_CONTEXT_CURSOR_SCALE 8
#define FR_FONT_DEFAULT_CURSOR_SPACING 0x15
#define FR_FONT_DEFAULT_CELL_SLOT_COUNT 2
#define FR_FONT_POSITION_BAND_HEIGHT 0x54

/* Ensure the four named default font files are installed, in slot order. */
void frFontLoadDefaultFonts(void) {
    frFontEnsureSlotLoaded(0, "/font/font0.fnt");
    frFontEnsureSlotLoaded(1, "/font/font1.fnt");
    frFontEnsureSlotLoaded(2, "/font/font2.fnt");
    frFontEnsureSlotLoaded(3, "/font/font3.fnt");
}

/* Free each installed entry, including the temporary slot, without reloading. */
void frFontFreeAllEntries(void) {
    s32 fontIndex;

    for (fontIndex = 0; fontIndex < FR_FONT_ENTRY_CAPACITY; fontIndex++) {
        if (frFontWork.entries[fontIndex].resource != NULL) {
            frFontFreeEntry((u8)fontIndex);
        }
    }
}

extern u8 D_00356478[];
/* This caller passes the full image-buffer word; the callee consumes its low half. */
extern void sdfUploadGsImageUnderSemaphore(s32 buffer, s32 image);

/* Assemble six 16-word image blocks from little-endian source bytes and upload
 * each to its matching GS buffer. The inner countdown includes zero. */
void func_001946C8(void) {
    u32 imageWords[FR_FONT_IMAGE_WORD_COUNT];
    s32 bufferIndex = 0;
    s32 sourceWordOffset = 0;
    FrFontSystem *work = &frFontWork;
    u32 *imageBuffer = work->imageBuffers;
    u8 *sourceTable = D_00356478;

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
    FrFontRecord *cachedItem = glyph->link1C.cachedItem;

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

extern s32 itfEnqueueMemNode(void *node, s32 pool);

/* Release parents backward and children forward, returning both to their pools.
 * Children with a borrowed source record bypass cached-item reference release. Return NULL. */
FrFontGlyph *frFontReleaseGlyphChain(FrFontGlyph *glyph) {
    FrFontGlyph *currentGlyph = glyph;
    FrFontGlyph *childGlyph;
    FrFontGlyph *nextChildGlyph;
    FrFontGlyph *previousGlyph;

    if (currentGlyph == NULL) {
        return NULL;
    }
    do {
        childGlyph = currentGlyph->link1C.firstChild;
        while (childGlyph != NULL) {
            nextChildGlyph = childGlyph->next;
            if (childGlyph->link20.sourceItem == NULL) {
                childGlyph->link1C.cachedItem->refs--;
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
    FrFontGlyph **queueSlot = &frFontWork.glyphSlots[kwlnGetDrawBufferIndex() & 0xFF];

    *queueSlot = frFontLinkGlyph(*queueSlot, glyph, 0);
    return 0;
}

/* Return the live count of retained font-cache records. */
s32 func_00194978(void) {
    return frFontWork.cachedItemCount;
}

/* Return the live pooled child-glyph count. */
s32 func_00194988(void) {
    return frFontWork.itemCount;
}

/* Return the live pooled parent-glyph count. */
s32 func_00194998(void) {
    return frFontWork.glyphCount;
}

u32 func_001949A8() {
    return 0;
}

INCLUDE_ASM(const s32, "interface/frFont", func_001949B0);

/* Create a glyph wrapper for source and append it to an optional destination chain. */
FrFontGlyph *frFontAppendGlyphReference(FrFontRecord *source, FrFontGlyph *destination) {
    FrFontGlyph *glyph;
    FrFontGlyph *previous;

    if (destination == NULL) {
        destination = itfDequeueMemNode(frFontWork.glyphPool);
        frFontWork.glyphCount++;
        frFontInitGlyph(destination);
    }
    glyph = itfDequeueMemNode(frFontWork.itemPool);
    previous = destination->link20.linkedGlyph;
    frFontWork.itemCount++;
    frFontSetupGlyph(glyph, 0, 0, 0, 0xA09DC300, 0);
    if (previous == NULL) {
        destination->link1C.firstChild = glyph;
    } else {
        previous->next = glyph;
    }
    glyph->link20.sourceItem = source;
    glyph->advance = D_00356470[0];
    glyph->unk18.b[0] = frFontGetGlyphCellWidth(0);
    glyph->unk18.b[1] = frFontGetGlyphCellHeight(0);
    glyph->previous = previous;
    destination->link20.linkedGlyph = glyph;
    destination->unk18.w++;
    destination->advance += glyph->advance;
    destination->u10.half[0] = glyph->unk18.b[0];
    destination->u10.half[1] = glyph->unk18.b[1];
    return destination;
}

/* Append a source-glyph reference wrapper, not a deep copy of source storage.
 * If wrapper creation returns NULL, preserve the original destination. */
FrFontGlyph *frFontAppendClonedGlyph(FrFontRecord *source, FrFontGlyph *destination) {
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
extern void func_002D1D80(SdfImageUploadRequest *);

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
    imagePosition[0] = atlasNode->uv.u0 >> FR_FONT_ATLAS_POSITION_SHIFT;
    imagePosition[1] = atlasNode->uv.v0 >> FR_FONT_ATLAS_POSITION_SHIFT;
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
    uploadRequest.bufferWidth = frFontWork.atlas.width;
    uploadRequest.destination = frFontWork.atlas.bufferBase << FR_FONT_ATLAS_DEST_SHIFT;
    uploadRequest.x = imagePosition[0];
    uploadRequest.y = imagePosition[1];
    func_002D1D80(&uploadRequest);
    return cachedItem;
}

typedef struct FrFontSegments {
    void *first;
    void *second;
    void *third;
} FrFontSegments;

extern void *sdfAllocSizeClassBlock(s32 size);
extern void itfSplitRelativeSegments(void *block, FrFontSegments *out);
extern void func_00198088(void *dst, s32 option, void *block, FrFontSegments *segments);

/* Allocate a fixed-size clone and process the selected font resource's segments.
 * The processing option remains opaque; neither allocation nor index is checked. */
void *frFontCloneEntryResource(u8 fontIndex, s32 resourceOption) {
    FrFontEntry *entry = &frFontWork.entries[fontIndex];
    void *clonedResource = sdfAllocSizeClassBlock(FR_FONT_RESOURCE_CLONE_BYTES);
    FrFontSegments segments;

    itfSplitRelativeSegments(entry->resource, &segments);
    func_00198088(clonedResource, resourceOption, entry->resource, &segments);
    return clonedResource;
}

extern u16 frFontGetSlotCellWidth(s32 index);
extern u16 frFontGetSlotCellHeight(s32 index);

/* Retain a cached table item, or create/upload a new one and cache it.
 * New records start with one reference; itemIndex is not bounds-checked. */
FrFontRecord *frFontRetainOrCreateCachedItem(FrFontGlyph *glyph, s32 itemIndex) {
    FrFontEntry *entry = &frFontWork.entries[glyph->u14.b[1]];
    FrFontRecord *cachedItem = entry->slots[itemIndex];
    void *clonedResource;

    if (cachedItem != NULL) {
        cachedItem->refs++;
        return cachedItem;
    }
    clonedResource = frFontCloneEntryResource(glyph->u14.b[1], itemIndex);
    cachedItem = frFontCreateAtlasItem(frFontGetSlotCellWidth(glyph->u14.b[1]), frFontGetSlotCellHeight(glyph->u14.b[1]), clonedResource, 1);
    cachedItem->id = itemIndex;
    entry->slots[itemIndex] = cachedItem;
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
    glyph->link1C.firstChild = NULL;
    glyph->link20.linkedGlyph = NULL;
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
    glyph->link1C.firstChild = NULL;
    glyph->link20.linkedGlyph = NULL;
    glyph->unk18.w = 0;
    glyph->unk30 = 0;
    glyph->unk34 = 0;
    glyph->unk38 = 0;
    glyph->unk3C = 0;
    glyph->unk40 = 0;
}

extern void func_001949B0(void *, s32);

FrFontGlyph *func_00195010(u16 glyphId, s32 fontIndexArg, u8 firstOption, u8 secondOption) {
    FrFontGlyph *glyph;
    s32 glyphIndex;
    s32 fontIndex = fontIndexArg & 0xFF;
    FrFontEntry *entry;

    glyph = itfDequeueMemNode(frFontWork.itemPool);
    frFontWork.itemCount++;
    frFontSetupGlyph(glyph, glyphId, fontIndex, firstOption, 0xA09DC300, secondOption);

    if ((u16)glyph->u0.h < 0x80) {
        glyphIndex = (u16)glyph->u0.h - 0x20;
    } else {
        s32 adjusted = (u16)glyph->u0.h - 0x8080;

        glyphIndex = ((adjusted & 0xFF00) >> 1) + (adjusted & 0x7F);
    }
    entry = &frFontWork.entries[fontIndex];
    if (glyphIndex >= entry->resourceHeader->widthCount) {
        glyphIndex = 0x147;
    }
    glyph->link1C.cachedItem = frFontRetainOrCreateCachedItem(glyph, glyphIndex);
    func_001949B0(glyph, glyphIndex);
    if (glyph->u14.b[3] & 0x10) {
        glyph->y = func_001949A8((s8)fontIndex, glyphIndex);
    } else {
        glyph->y = 0;
    }
    return glyph;
}

/* Build text and position its chain after the previous glyph. A NULL build
 * preserves the previous chain. */
FrFontGlyph *frFontAppendGlyphFromData(void *text, s8 fontIndex, s8 firstOption, s8 secondOption, FrFontGlyph *previousGlyph) {
    FrFontGlyph *newGlyphChain = func_001951C8(text, fontIndex, firstOption, secondOption, 0);

    if (newGlyphChain == NULL) {
        return previousGlyph;
    }
    return frFontLinkGlyphAfterPrevious(previousGlyph, newGlyphChain);
}

INCLUDE_ASM(const s32, "interface/frFont", func_001951C8);

/* Double the masked input byte and saturate the stored byte at 0x80.
 * Keep the original signed constant used for the saturated byte assignment. */
void frFontSetContextEncodedByte(FrFontGlyph *glyph, s32 inputValue) {
    s32 encodedValue = (inputValue & FR_FONT_BYTE_MASK) * 2;

    if (encodedValue >= FR_FONT_CONTEXT_BYTE_THRESHOLD) {
        glyph->u0.b.b0 = FR_FONT_CONTEXT_BYTE_CAP;
    } else {
        glyph->u0.b.b0 = encodedValue;
    }
}

/* Enable the glyph mode and initialize its encoded byte to the cap. */
void frFontEnableContextMode(FrFontGlyph *glyph) {
    glyph->unk40 = 1;
    frFontSetContextEncodedByte(glyph, FR_FONT_CONTEXT_ENABLE_VALUE);
}

/* Store the requested flag byte and refresh the cached child-chain advance. */
void frFontSetFlagAndMeasureGlyphs(FrFontGlyph *glyph, s32 requestedFlag) {
    glyph->u0.b.b1 = requestedFlag;
    glyph->advance = frFontMeasureGlyphChain(glyph);
}

/* Set the initial glyph's halfword dimensions and every visited child's advance/
 * byte dimensions, then remeasure only the initial glyph. Input must be non-NULL. */
void frFontSetGlyphChainDimensions(FrFontGlyph *glyph, s32 cellAdvance, s32 cellHeight) {
    FrFontGlyph *targetGlyph = glyph;
    FrFontGlyph *childGlyph;

    targetGlyph->u10.half[0] = cellAdvance;
    targetGlyph->u10.half[1] = cellHeight;
    for (; glyph != NULL; glyph = glyph->previous) {
        for (childGlyph = glyph->link1C.firstChild; childGlyph != NULL; childGlyph = childGlyph->next) {
            childGlyph->advance = cellAdvance;
            childGlyph->unk18.b[0] = cellAdvance;
            childGlyph->unk18.b[1] = cellHeight;
        }
    }
    targetGlyph->advance = frFontMeasureGlyphChain(targetGlyph);
}

/* Store the glyph position without scaling. */
void frFontSetContextPair(FrFontGlyph *glyph, u32 first, u32 second) {
    glyph->x = first;
    glyph->y = second;
}

/* Store the glyph render value in sixteenths. */
void frFontStoreShiftedContextValue(FrFontGlyph *glyph, u32 unshiftedValue) {
    glyph->u14.w = unshiftedValue >> FR_FONT_CONTEXT_VALUE_SHIFT;
}

/* Assign the first option byte across all visited children; NULL is a no-op. */
void frFontSetChainFlag(FrFontGlyph *glyph, u8 flagValue) {
    FrFontGlyph *childGlyph;

    for (; glyph != NULL; glyph = glyph->previous) {
        for (childGlyph = glyph->link1C.firstChild; childGlyph != NULL; childGlyph = childGlyph->next) {
            childGlyph->u14.b[0] = flagValue;
        }
    }
}

/* Replace each child's full color word across the parent glyph chain. */
void frFontSetChildColors(FrFontGlyph *parentNode, u32 colorWord) {
    for (; parentNode != NULL; parentNode = parentNode->previous) {
        FrFontGlyph *childNode;
        for (childNode = parentNode->link1C.firstChild; childNode != NULL; childNode = childNode->next) {
            childNode->u10.word = colorWord;
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
        for (childGlyph = glyph->link1C.firstChild; childGlyph != NULL; childGlyph = childGlyph->next) {
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
s32 func_001955D8(FrFontGlyph *parent, FrFontGlyph *glyph, u8 threshold, u8 step) {
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
        if (previousParent != NULL && previousParent->link20.linkedGlyph != NULL) {
            fade = previousParent->link20.linkedGlyph->u10.byte[0];
            previousOption = previousParent->link20.linkedGlyph->u14.b[2];
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

    if (previousParent != NULL && parent->link1C.firstChild == glyph && parent->unk40 == 0) {
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

    if (parent->unk34 == 0xF117 && parent->link20.linkedGlyph->u10.byte[0] == 0x80) {
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
s32 frFontDrawGlyphInDefaultMode(FrFontGlyph *glyph) {
    return frFontDrawGlyphWithSharedFlags(glyph, 0);
}

/* Pass the selected mode and current shared render-flag word to the renderer. */
s32 frFontDrawGlyphWithSharedFlags(FrFontGlyph *glyph, s8 mode) {
    return func_001958A0(glyph, mode, frFontSharedRenderFlags);
}

/* Draw every child glyph, advance each parent chain, and report its measured
 * width only while all control/fade conditions remain ready. */
s32 func_001958A0(FrFontGlyph *glyph, s8 mode, u32 flags) {
    FrFontGlyph *child;
    s32 ready = 1;
    s32 enabled = 1;
    s32 result = 0;
    s32 totalAdvance = 0;
    s32 titleSoundBusy;
    const FrFontAtlas *atlas;

    if (glyph != NULL) {
        glyph = glyph->chainHead;
        if (glyph != NULL) {
            do {
                s32 x = glyph->x;
                s32 y = glyph->y;
                s8 spacing = glyph->u0.b.b1;

                child = glyph->link1C.firstChild;
                if (child != NULL) {
                    atlas = &frFontWork.atlas;
                    do {
                        s32 xOffset = mode != 0 ? 0 :
                            func_001955D8(glyph, child, 0x1C, (u8)glyph->u0.b.b0);
                        u32 glyphState;
                        if (child->link20.sourceItem == NULL) {
                            if ((u16)child->u0.h < 0x80) {
                                enabled = 1;
                            }
                            func_00193D70(x + child->x + xOffset, y + child->y,
                                         child->unk18.b[0], child->unk18.b[1] >> 1,
                                         child->u14.b[0], child->u10.word,
                                         glyph->u14.w, enabled,
                                         &child->link1C.cachedItem->list->uv,
                                         atlas, flags);
                        } else {
                            func_00193D70(x + child->x + xOffset, y + child->y,
                                         child->unk18.b[0], child->unk18.b[1] >> 1,
                                         child->u14.b[0], child->u10.word,
                                         glyph->u14.w, enabled,
                                         &child->link20.sourceItem->list->uv,
                                         atlas, flags);
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
        result = totalAdvance;
        if (ready == 0) {
            result = 0;
        }
    }
    return result;
}

/* Release queue slot 1 when the draw-buffer index's low byte is zero, otherwise
 * slot 0; return 0. This is buffer selection, not a current-font selection. */
s32 frFontAdvanceSelectedGlyphSlot(void) {
    s32 queueIndex = (kwlnGetDrawBufferIndex() & FR_FONT_BYTE_MASK) == 0;

    frFontWork.glyphSlots[queueIndex] = frFontReleaseGlyphChain(frFontWork.glyphSlots[queueIndex]);
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
u32 frFontMeasureGlyphChain(FrFontGlyph *parentGlyph) {
    FrFontGlyph *childGlyph = parentGlyph->link1C.firstChild;
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
        childGlyph = parentGlyph->link1C.firstChild;
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

u32 frFontGetGlyphCellWidth(u8 fontIndex);
u32 frFontGetGlyphCellHeight(u8 fontIndex);
extern void func_001949B0(void *work, s32 code);

s32 func_00195CD8(const u8 *text, u8 fontIndex, u8 mode) {
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
        func_001949B0(&work, glyphIndex);
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
    return frFontWork.entries[entryIndex].resourceHeader->cellWidth;
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
    return frFontWork.entries[entryIndex].resourceHeader->cellHeight;
}

/* Restore this game's cursor spacing (DDS1: 21, DDS2: 25). */
void frFontResetContextCursorSpacing(void) {
    frFontContextCursorSpacing = FR_FONT_DEFAULT_CURSOR_SPACING;
}

/* Store unscaled cursor spacing; advancement applies its factor of eight later. */
void frFontSetContextCursorSpacing(u32 spacing) {
    frFontContextCursorSpacing = spacing;
}

INCLUDE_ASM(const s32, "interface/frFont", func_00195E60);

INCLUDE_ASM(const s32, "interface/frFont", func_00195ED8);

typedef struct FrFontPosition {
    s32 x;
    s32 y;
} FrFontPosition;

/* Write each visited position and stop at requestedIndex. Missing indices leave
 * the last visited position; empty input leaves output unchanged. DDS1 still
 * scans the y-band but advances only one next node, unlike DDS2's band advance. */
void func_00195FA8(FrFontPosition *position, s32 requestedIndex, FrFontGlyph *glyph) {
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
                currentNode = currentNode->next;
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

INCLUDE_ASM(const s32, "interface/frFont", func_00196088);

INCLUDE_ASM(const s32, "interface/frFont", func_001961B0);

INCLUDE_ASM(const s32, "interface/frFont", func_00196220);

/* Append the shared-text child context, apply its cached encoded byte and clear
 * the pending-create flag. Preserve the old-style implicit-argument interface. */
/* Old-style definition: callers invoke it without arguments and rely on $a0. */
void frFontCreateContext(ctx)
    FrFontCtx *ctx;

{
    FrFontGlyph *childGlyph = frFontAppendGlyphFromData(&D_003BB180, 0, ctx->channel1, ctx->channel2, ctx->glyphChain);

    ctx->glyphChain = childGlyph;
    frFontSetContextEncodedByte(childGlyph, ctx->channel3);
    ctx->pendingCreate = 0;
}

/* Cancel creation for an empty child, process pending creation, then process
 * the independent pair refresh. The refresh phase has no NULL-child guard. */
void frFontCheckPendingGlyphState(FrFontCtx *ctx) {
    s8 pending;

    if (ctx->glyphChain == NULL) {
        pending = ctx->pendingCreate;
    } else {
        if (ctx->glyphChain->link1C.firstChild == NULL) {
            ctx->pendingCreate = 0;
        }
        pending = ctx->pendingCreate;
    }
    if (pending == 0) {
        pending = ctx->pendingPosition;
    } else {
        frFontCreateContext();
        pending = ctx->pendingPosition;
    }
    if (pending != 0) {
        frFontSetContextPair(ctx->glyphChain, ctx->x, ctx->y);
        ctx->pendingPosition = 0;
    }
}

/* Advance by eight times cursor spacing and request both context updates. */
void frFontAdvanceContextCursor(FrFontCtx *ctx) {
    ctx->y += frFontContextCursorSpacing * FR_FONT_CONTEXT_CURSOR_SCALE;
    ctx->pendingCreate = 1;
    ctx->pendingPosition = 1;
}

INCLUDE_SDATA(const s32, "interface/frFont", D_003BB160);

INCLUDE_SDATA(const s32, "interface/frFont", frFontContextCursorSpacing);

INCLUDE_SDATA(const s32, "interface/frFont", frFontDefaultGlyphCellSize);

INCLUDE_SDATA(const s32, "interface/frFont", D_003BB16C);

INCLUDE_SDATA(const s32, "interface/frFont", D_003BB170);

INCLUDE_SDATA(const s32, "interface/frFont", frFontSharedGlyphFlags);

INCLUDE_SDATA(const s32, "interface/frFont", frFontSharedRenderFlags);

INCLUDE_SDATA(const s32, "interface/frFont", D_003BB17C);

INCLUDE_SDATA(const s32, "interface/frFont", D_003BB180);

