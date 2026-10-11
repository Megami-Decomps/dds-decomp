#include "common.h"
#include "sdf_image_upload.h"
#include "sdf_chip.h"
#include "fr_font_measure.h"
#include "itf.h"
#include "itf_mem_node.h"


extern s32 mnuQueryTitleSoundBusy(void);

extern u32 frFontSharedRenderFlags;

extern u32 frFontContextCursorSpacing;



extern u32 kwlnGetDrawBufferIndex(void);

extern s32 D_0043656C;

extern FrFontGlyph *frFontLinkGlyph(FrFontGlyph *previous, FrFontGlyph *next, s32 positionNext);


extern u8 frFontSharedGlyphFlags;




void frFontCreateContext();

extern u8 D_00436578[];

extern s32 frFontDefaultGlyphCellSize;


typedef struct FrFontSegments {
    void *first;
    void *second;
    void *third;
} FrFontSegments;

extern void itfSplitRelativeSegments(void *block, FrFontSegments *out);
extern void func_001A00B8(void *dst, s32 option, void *block, FrFontSegments *segments);

extern FrFontGlyph *frFontReleaseGlyphChain(FrFontGlyph *glyph);


extern s32 frFontAdvanceGlyphFade(FrFontGlyph *glyph);

extern FrFontGlyph *frFontAppendGlyphReference(FrFontRecord *source, FrFontGlyph *destination);
void frFontSetupGlyph(FrFontChildGlyph *, s32, s32, s32, s32, s32);
void frFontInitGlyph(FrFontGlyph *);
u32 frFontGetGlyphCellWidth(u8);
u32 frFontGetGlyphCellHeight(u8);
extern u8 D_00436570;
FrFontGlyph *frFontBuildGlyphChain(const char *text, s8 fontIndex, s8 firstOption, s8 secondOption, FrFontGlyph *head);
FrFontChildGlyph *frFontCreateGlyphFromCode(u16 glyphId, s32 fontIndexArg, u8 firstOption, u8 secondOption);
u32 frFontMeasureGlyphChain(FrFontGlyph *parentGlyph);

FrFontGlyph *frFontLinkGlyphAfterPrevious(FrFontGlyph *previous, FrFontGlyph *next);


extern s32 frFontDrawGlyphChain(FrFontGlyph *glyph, s8 mode, u32 flags);

extern void frFontEnsureSlotLoaded(s32, const char *);

#define FR_FONT_ENTRY_CAPACITY 9
#define FR_FONT_IMAGE_WORD_COUNT 16
#define FR_FONT_IMAGE_LAST_WORD 15
#define FR_FONT_IMAGE_BUFFER_COUNT 6
#define FR_FONT_IMAGE_WORD_BYTES 4
#define FR_FONT_PIXEL_FORMAT 0x14
#define FR_FONT_ATLAS_POSITION_SHIFT 4
#define FR_FONT_ATLAS_DEST_SHIFT 6
#define FR_FONT_RESOURCE_CLONE_BYTES 0x120

#define FR_FONT_BYTE_MASK 0xFF
#define FR_FONT_CONTEXT_BYTE_THRESHOLD 0x81
#define FR_FONT_CONTEXT_BYTE_CAP (-0x80)
#define FR_FONT_CONTEXT_ENABLE_VALUE 0x80
#define FR_FONT_CONTEXT_VALUE_SHIFT 4
#define FR_FONT_FADE_OPTION 2
#define FR_FONT_FADE_COMPLETE 0x80
#define FR_FONT_FADE_ADVANCE_SCALE 22
#define FR_FONT_JITTER_OPTION 1
#define FR_FONT_JITTER_PERIOD 5
#define FR_FONT_JITTER_CENTER 2
#define FR_FONT_FADE_PREDECESSOR_THRESHOLD 0x1C
#define FR_FONT_FADE_VALUE_STEP 8
#define FR_FONT_FADE_Y_STEP 0x10
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
    FrFontEntry *entries = frFontWork.entries;
    s32 fontIndex;

    for (fontIndex = 0; fontIndex < FR_FONT_ENTRY_CAPACITY; fontIndex++) {
        if (entries[fontIndex].resource != NULL) {
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
    FrFontSystem *work = &frFontWork;
    u32 *imageBuffer = work->imageBuffers;
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
void frFontReleaseUnreferencedGlyphItem(FrFontChildGlyph *glyph) {
    FrFontRecord *cachedItem = glyph->cachedItem;

    if (cachedItem != NULL) {
        if (cachedItem->refs == 0) {
            frFontWork.entries[glyph->renderValueOrSetupOrShade.setupBytes.fontIndex].slots[cachedItem->id] = 0;
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

/* Release parents backward and children forward, returning both to their pools.
 * Children with a borrowed source record bypass cached-item reference release. Return NULL. */
FrFontGlyph *frFontReleaseGlyphChain(FrFontGlyph *glyph) {
    FrFontGlyph *currentGlyph = glyph;
    FrFontChildGlyph *childGlyph;
    FrFontChildGlyph *nextChildGlyph;
    FrFontGlyph *previousGlyph;

    if (currentGlyph == NULL) {
        return NULL;
    }
    do {
        childGlyph = currentGlyph->firstChild;
        while (childGlyph != NULL) {
            nextChildGlyph = childGlyph->next;
            if (childGlyph->sourceItem == NULL) {
                childGlyph->cachedItem->refs--;
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
s32 frFontQueueGlyphForCurrentDrawBuffer(FrFontGlyph *glyph) {
    FrFontGlyph **queueSlot = &frFontWork.glyphSlots[kwlnGetDrawBufferIndex() & 0xFF];

    *queueSlot = frFontLinkGlyph(*queueSlot, glyph, 0);
    return 0;
}

/* Return the live count of retained font-cache records. */
s32 frFontGetCachedItemCount(void) {
    return frFontWork.cachedItemCount;
}

/* Return the live pooled child-glyph count. */
s32 frFontGetChildGlyphCount(void) {
    return frFontWork.itemCount;
}

/* Return the live pooled parent-glyph count. */
s32 frFontGetParentGlyphCount(void) {
    return frFontWork.glyphCount;
}

u32 func_0019C638() {
    return 0;
}

/* Metric workspace also supplies the fields shared by pooled child glyphs. */
typedef struct FrFontGlyphMeasureWork {
    u16 code;
    u8 pad02[2];
    s32 x;
    s32 y;
    s32 advance;
    u8 pad10[5];
    u8 fontIndex;
    u8 pad16;
    u8 mode;
    u8 cellWidth;
    u8 cellHeight;
    u8 pad1A[0x16];
} FrFontGlyphMeasureWork;

extern u16 frFontGetSlotCellWidth(s32 index);
extern s8 D_003B2DA0[];

/* Fill cell dimensions, advance and horizontal bearing for one glyph. */
void func_0019C640(void *opaqueWork, s32 glyphIndex) {
    FrFontGlyphMeasureWork *work = opaqueWork;
    s32 fontIndex;
    s32 defaultSlot = 0;
    s32 advance;
    s32 left;
    s32 leftOffset;
    s32 codeIndex;
    s8 right;
    f32 scale;
    FrFontEntry *entry;

    {
        fontIndex = work->fontIndex;
        switch (fontIndex) {
        case 0:
        case 1:
            defaultSlot = 1;
            break;
        }
        entry = &frFontWork.entries[fontIndex];
        work->cellWidth = frFontGetGlyphCellWidth(fontIndex);
    }
    work->cellHeight = frFontGetGlyphCellHeight(work->fontIndex);
    work->advance = D_003B2DA0[work->fontIndex];

    if (defaultSlot != 0) {
        scale = (f32)(work->cellWidth + 3) /
                (f32)frFontGetSlotCellWidth(work->fontIndex);
    } else {
        scale = 0.8f;
    }

    if ((work->mode & 2) != 0 && defaultSlot == 0 && work->code < 0x80) {
        work->advance >>= 1;
        work->cellWidth >>= 3;
    }

    if (work->code == 0x20) {
        if (work->fontIndex == 1) {
            work->advance >>= 2;
            return;
        }
        if (work->fontIndex == 0) {
            work->advance >>= 2;
            return;
        }
    }

    if (entry->resourceHeader->hasExtra == 0) {
        return;
    }
    if (work->code == 0x85C1) {
        work->x = 0x10;
        work->advance = 8;
    }

    glyphIndex = (s32)((u32)glyphIndex << 1);
    if (glyphIndex >= entry->metricByteCount) {
        return;
    }
    right = entry->flagBytes[glyphIndex + 1];
    if (right == 0) {
        return;
    }

    left = entry->flagBytes[glyphIndex];
    advance = right - left;
    leftOffset = (s32)((u32)left << 4);
    if (defaultSlot != 0) {
        advance = (s32)((f32)advance + (scale + 2.0f));
    }
    codeIndex = glyphIndex >> 1;
    switch (codeIndex) {
    case 1:
    case 14:
    case 17:
        leftOffset -= 8;
        advance++;
        break;
    case 7:
    case 8:
        leftOffset -= 0x20;
        advance++;
        break;
    case 45:
    case 77:
    case 78:
        advance++;
        break;
    case 79:
        leftOffset--;
        advance++;
        break;
    }
    work->advance = advance;
    work->x = -leftOffset;
}


/* Append a source-glyph reference, allocating a chain head when absent. */
FrFontGlyph *frFontAppendGlyphReference(FrFontRecord *source, FrFontGlyph *destination) {
    FrFontChildGlyph *glyph;
    FrFontChildGlyph *previous;

    if (destination == NULL) {
        destination = itfDequeueMemNode(frFontWork.glyphPool);
        frFontWork.glyphCount++;
        frFontInitGlyph(destination);
    }
    glyph = itfDequeueMemNode(frFontWork.itemPool);
    previous = destination->lastChild;
    frFontWork.itemCount++;
    frFontSetupGlyph(glyph, 0, 0, 0, 0xA09DC300, 0);
    if (previous == NULL) {
        destination->firstChild = glyph;
    } else {
        previous->next = glyph;
    }
    glyph->sourceItem = source;
    glyph->advance = D_003B2DA0[0];
    glyph->cellDimensions.cellWidth = frFontGetGlyphCellWidth(0);
    glyph->cellDimensions.cellHeight = frFontGetGlyphCellHeight(0);
    glyph->previous = previous;
    destination->lastChild = glyph;
    destination->childCount++;
    destination->advance += glyph->advance;
    destination->parentDimensionsOrRenderWord.parentDimensions.cellAdvance = glyph->cellDimensions.cellWidth;
    destination->parentDimensionsOrRenderWord.parentDimensions.cellHeight = glyph->cellDimensions.cellHeight;
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



extern FntNode *frFontDetachFirstResourceNode(void);
extern void sdfTexQueueImageUpload(SdfImageUploadRequest *);

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
        uploadRequest.allocationMode = SDF_UPLOAD_BORROWED;
    } else {
        uploadRequest.allocationMode = SDF_UPLOAD_CHIP_HEAP;
    }
    uploadRequest.pixels = pixels;
    uploadRequest.format = FR_FONT_PIXEL_FORMAT;
    uploadRequest.width = width;
    uploadRequest.height = height;
    uploadRequest.bufferWidth = frFontWork.atlas.width;
    uploadRequest.destination = frFontWork.atlas.bufferBase << FR_FONT_ATLAS_DEST_SHIFT;
    uploadRequest.x = imagePosition[0];
    uploadRequest.y = imagePosition[1];
    sdfTexQueueImageUpload(&uploadRequest);
    return cachedItem;
}

/* Allocate a fixed-size clone and process the selected font resource's segments.
 * The processing option remains opaque; neither allocation nor index is checked. */
void *frFontCloneEntryResource(u8 fontIndex, s32 resourceOption) {
    FrFontEntry *entry = &frFontWork.entries[fontIndex];
    void *clonedResource = sdfAllocSizeClassBlock(FR_FONT_RESOURCE_CLONE_BYTES);
    FrFontSegments segments;

    itfSplitRelativeSegments(entry->resource, &segments);
    func_001A00B8(clonedResource, resourceOption, entry->resource, &segments);
    return clonedResource;
}

extern u16 frFontGetSlotCellHeight(s32 index);

/* Retain a cached table item, or create/upload a new one and cache it.
 * New records start with one reference; itemIndex is not bounds-checked. */
FrFontRecord *frFontRetainOrCreateCachedItem(FrFontChildGlyph *glyph, s32 itemIndex) {
    FrFontEntry *entry = &frFontWork.entries[glyph->renderValueOrSetupOrShade.setupBytes.fontIndex];
    FrFontRecord *cachedItem = entry->slots[itemIndex];
    void *clonedResource;

    if (cachedItem != NULL) {
        cachedItem->refs++;
        return cachedItem;
    }
    clonedResource = frFontCloneEntryResource(glyph->renderValueOrSetupOrShade.setupBytes.fontIndex, itemIndex);
    cachedItem = frFontCreateAtlasItem(frFontGetSlotCellWidth(glyph->renderValueOrSetupOrShade.setupBytes.fontIndex), frFontGetSlotCellHeight(glyph->renderValueOrSetupOrShade.setupBytes.fontIndex), clonedResource, 1);
    cachedItem->id = itemIndex;
    entry->slots[itemIndex] = cachedItem;
    frFontWork.cachedItemCount++;
    return cachedItem;
}

/* Store the font/options, narrowed glyph code and high flag bits, then clear
 * positioning/links. Other glyph fields are deliberately not fully reset here. */
void frFontSetupGlyph(FrFontChildGlyph *glyph, s32 glyphId, s32 fontIndex, s32 firstOption, s32 packedFlags, s32 secondOption) {
    glyph->renderValueOrSetupOrShade.setupBytes.fontIndex = fontIndex;
    glyph->renderValueOrSetupOrShade.setupBytes.firstOption = firstOption;
    glyph->renderValueOrSetupOrShade.setupBytes.secondOption = secondOption;
    glyph->glyphCodeOrContext.glyphCode = glyphId;
    glyph->parentDimensionsOrRenderWord.renderWord = packedFlags & ~FR_FONT_BYTE_MASK;
    glyph->renderValueOrSetupOrShade.setupBytes.sharedFlags = frFontSharedGlyphFlags;
    glyph->x = 0;
    glyph->y = 0;
    glyph->advance = 0;
    glyph->drawCount = 0;
    glyph->cachedItem = NULL;
    glyph->sourceItem = NULL;
    glyph->previous = NULL;
    glyph->next = NULL;
}

/* Reset standalone chain-head state and context controls; leave drawCount intact. */
void frFontInitGlyph(FrFontGlyph *glyph) {
    glyph->glyphCodeOrContext.byteRoles.encodedContextByte = -0x80;
    glyph->x = 0;
    glyph->y = 0;
    glyph->glyphCodeOrContext.byteRoles.spacing = 0;
    glyph->advance = 0;
    glyph->renderValueOrSetupOrShade.renderValue = 0;
    glyph->previous = NULL;
    glyph->next = NULL;
    glyph->chainHead = glyph;
    glyph->firstChild = NULL;
    glyph->lastChild = NULL;
    glyph->childCount = 0;
    glyph->timedControlCode = 0;
    glyph->pendingLipsStopCode = 0;
    glyph->skipLipsStopWait = 0;
    glyph->remainingWaitFrames = 0;
    glyph->contextModeEnabled = 0;
}

extern void func_0019C640(void *, s32);

FrFontChildGlyph *frFontCreateGlyphFromCode(u16 glyphId, s32 fontIndexArg, u8 firstOption, u8 secondOption) {
    FrFontChildGlyph *glyph;
    s32 glyphIndex;
    s32 fontIndex = fontIndexArg & 0xFF;
    FrFontEntry *entry;

    glyph = itfDequeueMemNode(frFontWork.itemPool);
    frFontWork.itemCount++;
    frFontSetupGlyph(glyph, glyphId, fontIndex, firstOption, 0xA09DC300, secondOption);

    if ((u16)glyph->glyphCodeOrContext.glyphCode < 0x80) {
        glyphIndex = (u16)glyph->glyphCodeOrContext.glyphCode - 0x20;
    } else {
        s32 adjusted = (u16)glyph->glyphCodeOrContext.glyphCode - 0x8080;

        glyphIndex = ((adjusted & 0xFF00) >> 1) + (adjusted & 0x7F);
    }
    entry = &frFontWork.entries[fontIndex];
    if (glyphIndex >= entry->resourceHeader->widthCount) {
        glyphIndex = 0x147;
    }
    glyph->cachedItem = frFontRetainOrCreateCachedItem(glyph, glyphIndex);
    func_0019C640(glyph, glyphIndex);
    if (glyph->renderValueOrSetupOrShade.setupBytes.sharedFlags & 0x10) {
        glyph->y = func_0019C638((s8)fontIndex, glyphIndex);
    } else {
        glyph->y = 0;
    }
    return glyph;
}

/* Build text and position its chain after the previous glyph. A NULL build
 * preserves the previous chain. */
FrFontGlyph *frFontAppendTextToGlyphChain(const char *text, s8 fontIndex, s8 firstOption, s8 secondOption, FrFontGlyph *previousGlyph) {
    FrFontGlyph *newGlyphChain = frFontBuildGlyphChain(text, fontIndex, firstOption, secondOption, 0);

    if (newGlyphChain == NULL) {
        return previousGlyph;
    }
    return frFontLinkGlyphAfterPrevious(previousGlyph, newGlyphChain);
}

/* Build a glyph chain for a byte string, appending to a supplied head. */
FrFontGlyph *frFontBuildGlyphChain(const char *text, s8 fontIndex, s8 firstOption, s8 secondOption, FrFontGlyph *head) {
    u32 length = 0;
    u32 offset;
    u32 code;
    FrFontChildGlyph *previous;
    FrFontChildGlyph *created;

    if (fontIndex >= 0) {
        D_00436570 = fontIndex;
    }
    if (frFontWork.entries[D_00436570].resource == NULL) {
        return NULL;
    }
    if (text != NULL) {
        length = strlen(text);
    }
    if (head == NULL) {
        head = itfDequeueMemNode(frFontWork.glyphPool);
        frFontWork.glyphCount++;
        frFontInitGlyph(head);
    }
    previous = head->lastChild;
    offset = 0;
    created = NULL;
    if (length != 0) {
        do {
            code = (u8)text[offset];
            if (code >= 0x80) {
                offset++;
                code = (u8)text[offset] | (code << 8);
            }
            created = frFontCreateGlyphFromCode(code, D_00436570, firstOption & 0xFF, secondOption & 0xFF);
            if (previous == NULL) {
                head->firstChild = created;
            } else {
                previous->next = created;
            }
            offset++;
            head->advance += created->advance;
            head->childCount++;
            created->previous = previous;
            previous = created;
        } while (offset < length);
    }
    if (created != NULL) {
        head->lastChild = created;
        head->parentDimensionsOrRenderWord.parentDimensions.cellAdvance = created->cellDimensions.cellWidth;
        head->parentDimensionsOrRenderWord.parentDimensions.cellHeight = created->cellDimensions.cellHeight;
    }
    return head;
}

/* Double the masked input byte and saturate the stored byte at 0x80.
 * Keep the original signed constant used for the saturated byte assignment. */
void frFontSetContextEncodedByte(FrFontGlyph *glyph, s32 inputValue) {
    s32 encodedValue = (inputValue & FR_FONT_BYTE_MASK) * 2;

    if (encodedValue >= FR_FONT_CONTEXT_BYTE_THRESHOLD) {
        glyph->glyphCodeOrContext.byteRoles.encodedContextByte = FR_FONT_CONTEXT_BYTE_CAP;
    } else {
        glyph->glyphCodeOrContext.byteRoles.encodedContextByte = encodedValue;
    }
}

/* Enable the glyph mode and initialize its encoded byte to the cap. */
void frFontEnableContextMode(FrFontGlyph *glyph) {
    glyph->contextModeEnabled = 1;
    frFontSetContextEncodedByte(glyph, FR_FONT_CONTEXT_ENABLE_VALUE);
}

/* Store the signed spacing byte and refresh the cached child-chain advance. */
void frFontSetSpacingAndMeasureGlyphs(FrFontGlyph *glyph, s32 spacing) {
    u32 measuredAdvance;

    glyph->glyphCodeOrContext.byteRoles.spacing = spacing;
    measuredAdvance = frFontMeasureGlyphChain(glyph);
    glyph->advance = measuredAdvance;
}

/* Set the initial glyph's halfword dimensions and every visited child's advance/
 * byte dimensions, then remeasure only the initial glyph. Input must be non-NULL. */
void frFontSetGlyphChainDimensions(FrFontGlyph *glyph, s32 cellAdvance, s32 cellHeight) {
    FrFontGlyph *targetGlyph = glyph;
    FrFontChildGlyph *childGlyph;

    targetGlyph->parentDimensionsOrRenderWord.parentDimensions.cellAdvance = cellAdvance;
    targetGlyph->parentDimensionsOrRenderWord.parentDimensions.cellHeight = cellHeight;
    for (; glyph != NULL; glyph = glyph->previous) {
        for (childGlyph = glyph->firstChild; childGlyph != NULL; childGlyph = childGlyph->next) {
            childGlyph->advance = cellAdvance;
            childGlyph->cellDimensions.cellWidth = cellAdvance;
            childGlyph->cellDimensions.cellHeight = cellHeight;
        }
    }
    targetGlyph->advance = frFontMeasureGlyphChain(targetGlyph);
}

/* Store the glyph position without scaling. */
void frFontSetGlyphPosition(FrFontGlyph *glyph, u32 x, u32 y) {
    glyph->x = x;
    glyph->y = y;
}

/* Store the glyph render value in sixteenths. */
void frFontStoreShiftedRenderValue(FrFontGlyph *glyph, u32 unshiftedValue) {
    glyph->renderValueOrSetupOrShade.renderValue = unshiftedValue >> FR_FONT_CONTEXT_VALUE_SHIFT;
}

/* Assign the first option byte across all visited children; NULL is a no-op. */
void frFontSetChildChainFirstOption(FrFontGlyph *glyph, u8 firstOption) {
    FrFontChildGlyph *childGlyph;

    for (; glyph != NULL; glyph = glyph->previous) {
        for (childGlyph = glyph->firstChild; childGlyph != NULL; childGlyph = childGlyph->next) {
            childGlyph->renderValueOrSetupOrShade.setupBytes.firstOption = firstOption;
        }
    }
}

/* Replace each child's full color word across the parent glyph chain. */
void frFontSetChildColors(FrFontGlyph *parentNode, u32 colorWord) {
    for (; parentNode != NULL; parentNode = parentNode->previous) {
        FrFontChildGlyph *childNode;
        for (childNode = parentNode->firstChild; childNode != NULL; childNode = childNode->next) {
            childNode->parentDimensionsOrRenderWord.renderWord = colorWord;
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
    FrFontChildGlyph *childGlyph;
    s32 didChange = 0;

    for (; glyph != NULL; glyph = glyph->previous) {
        for (childGlyph = glyph->firstChild; childGlyph != NULL; childGlyph = childGlyph->next) {
            if (childGlyph->renderValueOrSetupOrShade.setupBytes.secondOption == FR_FONT_FADE_OPTION) {
                u32 glyphWord = childGlyph->parentDimensionsOrRenderWord.renderWord;
                s32 fadeValue = glyphWord & FR_FONT_BYTE_MASK;

                if (fadeValue != 0) {
                    fadeValue -= FR_FONT_FADE_VALUE_STEP;
                    if (fadeValue < 0) {
                        fadeValue = 0;
                    }
                    childGlyph->y += FR_FONT_FADE_Y_STEP;
                    childGlyph->parentDimensionsOrRenderWord.renderWord = (glyphWord & ~FR_FONT_BYTE_MASK) | fadeValue;
                    didChange = 1;
                }
            }
        }
    }
    return didChange;
}

/* Advance one rendered glyph's fade state and return its transient X jitter. */
s32 frFontAdvanceChildFadeAndGetJitter(FrFontGlyph *parent, FrFontChildGlyph *glyph, u8 threshold, u8 step) {
    FrFontGlyph *previousParent;
    FrFontChildGlyph *previousGlyph;
    u32 *fadeWord;
    u8 previousOption;
    u8 previousFade = FR_FONT_FADE_COMPLETE;
    s32 canAdvanceFade = 0;
    s32 canStopLips = 1;
    s32 glyphJitter = 0;

    previousGlyph = glyph->previous;
    previousOption = 0;
    if (previousGlyph == NULL) {
        previousParent = parent->previous;
        if (previousParent != NULL && previousParent->lastChild != NULL) {
            previousFade = previousParent->lastChild->parentDimensionsOrRenderWord.fadeByte.value;
            previousOption = previousParent->lastChild->renderValueOrSetupOrShade.setupBytes.secondOption;
        }
    } else {
        previousFade = previousGlyph->parentDimensionsOrRenderWord.fadeByte.value;
        previousParent = parent->previous;
    }

    fadeWord = &glyph->parentDimensionsOrRenderWord.renderWord;
    if (previousOption == 0 || previousOption == glyph->renderValueOrSetupOrShade.setupBytes.secondOption) {
        if (previousFade >= threshold) {
            canAdvanceFade = 1;
        }
    } else {
        canAdvanceFade = previousFade == FR_FONT_FADE_COMPLETE;
    }

    if (previousParent != NULL && parent->firstChild == glyph && parent->contextModeEnabled == 0) {
        switch (previousParent->timedControlCode) {
        case ITF_GLYPH_CONTROL_WAIT_FRAMES:
            if (previousParent->remainingWaitFrames > 0) {
                canAdvanceFade = 0;
                if (previousFade == FR_FONT_FADE_COMPLETE) {
                    previousParent->remainingWaitFrames = previousParent->remainingWaitFrames - 1;
                }
            }
            break;
        case ITF_GLYPH_CONTROL_WAIT_FRAME_OR_SOUND:
            if (previousParent->remainingWaitFrames != ITF_GLYPH_WAIT_FOR_SOUND_SENTINEL) {
                if (previousParent->remainingWaitFrames > 0) {
                    previousParent->remainingWaitFrames--;
                    canAdvanceFade = 0;
                }
            } else if (mnuQueryTitleSoundBusy() != 0) {
                canAdvanceFade = 0;
            }
            break;
        }
    }

    if (parent->pendingLipsStopCode == ITF_GLYPH_CONTROL_STOP_LIPS && parent->lastChild->parentDimensionsOrRenderWord.fadeByte.value == FR_FONT_FADE_COMPLETE) {
        if (parent->skipLipsStopWait == 0 && parent->remainingWaitFrames > 0) {
            if (parent->remainingWaitFrames == ITF_GLYPH_WAIT_FOR_SOUND_SENTINEL) {
                if (mnuQueryTitleSoundBusy() != 0) {
                    canStopLips = 0;
                }
            } else {
                canStopLips = 0;
            }
        }
        if (canStopLips != 0) {
            evtLipsStopFunction();
            parent->pendingLipsStopCode = 0;
        }
    }

    if (canAdvanceFade != 0) {
        u32 currentFadeWord;

        if ((s8)step >= 0) {
            step = (step * FR_FONT_FADE_ADVANCE_SCALE) / (glyph->advance + parent->glyphCodeOrContext.byteRoles.spacing);
        }
        currentFadeWord = *fadeWord;
        if ((u32)(FR_FONT_FADE_COMPLETE - (currentFadeWord & FR_FONT_BYTE_MASK)) >= step) {
            *fadeWord = currentFadeWord + step;
        } else {
            *fadeWord = (currentFadeWord & ~FR_FONT_BYTE_MASK) | FR_FONT_FADE_COMPLETE;
        }
    }

    {
        s8 currentFade = fadeWord[0];
        u16 glyphValue = glyph->drawCount;

        if (currentFade >= 0) {
            if (glyph->renderValueOrSetupOrShade.setupBytes.secondOption == FR_FONT_JITTER_OPTION) {
                glyphJitter = -((((glyphValue * 2) % FR_FONT_JITTER_PERIOD) - FR_FONT_JITTER_CENTER) << FR_FONT_POSITION_SHIFT);
            }
        }
    }
    return glyphJitter;
}

/* Draw through the shared render flags with mode zero. */
s32 frFontDrawGlyphInDefaultMode(FrFontGlyph *glyph) {
    return frFontDrawGlyphWithSharedFlags(glyph, 0);
}

/* Pass the selected mode and current shared render-flag word to the renderer. */
s32 frFontDrawGlyphWithSharedFlags(FrFontGlyph *glyph, s8 mode) {
    return frFontDrawGlyphChain(glyph, mode, frFontSharedRenderFlags);
}

/* Draw every child glyph, advance each parent chain, and report its total
 * child-glyph count only while all control/fade conditions remain ready. DDS2 additionally
 * suppresses one result per positive draw-delay count. */
s32 frFontDrawGlyphChain(FrFontGlyph *glyph, s8 mode, u32 flags) {
    FrFontChildGlyph *child;
    s32 ready = 1;
    s32 enabled = 1;
    s32 result = 0;
    s32 totalGlyphCount = 0;
    s32 titleSoundBusy;
    const FrFontAtlas *atlas;

    if (glyph != NULL) {
        glyph = glyph->chainHead;
        if (glyph != NULL) {
            do {
                s32 x = glyph->x;
                s32 y = glyph->y;
                s8 spacing = glyph->glyphCodeOrContext.byteRoles.spacing;

                child = glyph->firstChild;
                if (child != NULL) {
                    atlas = &frFontWork.atlas;
                    do {
                        s32 xOffset = mode != 0 ? 0 :
                            frFontAdvanceChildFadeAndGetJitter(glyph, child, FR_FONT_FADE_PREDECESSOR_THRESHOLD, (u8)glyph->glyphCodeOrContext.byteRoles.encodedContextByte);
                        u32 glyphState;
                        if (child->sourceItem == NULL) {
                            if ((u16)child->glyphCodeOrContext.glyphCode < 0x80) {
                                enabled = 1;
                            }
                            func_0019BA00(x + child->x + xOffset, y + child->y,
                                         child->cellDimensions.cellWidth,
                                         child->cellDimensions.cellHeight >> 1,
                                         child->renderValueOrSetupOrShade.setupBytes.firstOption, child->parentDimensionsOrRenderWord.renderWord,
                                         glyph->renderValueOrSetupOrShade.renderValue, enabled,
                                         &child->cachedItem->list->uv,
                                         atlas, flags);
                        } else {
                            func_0019BA00(x + child->x + xOffset, y + child->y,
                                         child->cellDimensions.cellWidth,
                                         child->cellDimensions.cellHeight >> 1,
                                         child->renderValueOrSetupOrShade.setupBytes.firstOption, child->parentDimensionsOrRenderWord.renderWord,
                                         glyph->renderValueOrSetupOrShade.renderValue, enabled,
                                         &child->sourceItem->list->uv,
                                         atlas, flags);
                        }
                        glyphState = child->parentDimensionsOrRenderWord.fadeByte.value;
                        if (glyphState != 0) {
                            child->drawCount++;
                        }
                        if (glyphState < FR_FONT_FADE_COMPLETE) {
                            ready = 0;
                        }
                        x += (child->advance + spacing) << FR_FONT_POSITION_SHIFT;
                        child = child->next;
                    } while (child != NULL);
                }

                if (glyph->next == NULL && glyph->contextModeEnabled == 0) {
                    switch (glyph->timedControlCode) {
                    case ITF_GLYPH_CONTROL_WAIT_FRAMES:
                        if (ready != 0 && glyph->remainingWaitFrames > 0) {
                            glyph->remainingWaitFrames--;
                            ready = 0;
                        }
                        break;
                    case ITF_GLYPH_CONTROL_WAIT_FRAME_OR_SOUND:
                        if (glyph->remainingWaitFrames != ITF_GLYPH_WAIT_FOR_SOUND_SENTINEL) {
                            if (glyph->remainingWaitFrames > 0) {
                                glyph->remainingWaitFrames--;
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
                totalGlyphCount += glyph->childCount;
                glyph = glyph->next;
            } while (glyph != NULL);
        }
        if (D_0043656C > 0) {
            D_0043656C--;
            return 0;
        }
        result = totalGlyphCount;
        if (ready == 0) {
            result = 0;
        }
    }
    return result;
}

/* Release queue slot 1 when the draw-buffer index's low byte is zero, otherwise
 * slot 0; return 0. This is buffer selection, not a current-font selection. */
s32 frFontReleaseOppositeDrawBufferGlyphs(void) {
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
    frFontBindResourceSections(FR_FONT_TEMPORARY_SLOT, (u8 *)fontData, 0);
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
    FrFontChildGlyph *childGlyph = parentGlyph->firstChild;
    s32 totalAdvance = 0;

    if (childGlyph != NULL) {
        s8 childSpacing = parentGlyph->glyphCodeOrContext.byteRoles.spacing;

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
    FrFontChildGlyph *childGlyph;
    s32 totalAdvance = 0;

    for (parentGlyph = glyphChain->chainHead; parentGlyph != NULL; parentGlyph = parentGlyph->next) {
        childGlyph = parentGlyph->firstChild;
        if (childGlyph != NULL) {
            s8 childSpacing = parentGlyph->glyphCodeOrContext.byteRoles.spacing;

            do {
                totalAdvance += childGlyph->advance;
                childGlyph = childGlyph->next;
                totalAdvance += childSpacing;
            } while (childGlyph != NULL);
        }
    }
    return totalAdvance;
}

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

/* Count the y-bands in a glyph chain, using the same band walk as func_0019DC68. */
s32 func_0019DB30(FrFontGlyph *glyph) {
    s32 bandCount = 0;
    FrFontGlyph *currentNode;
    FrFontGlyph *scanNode;
    s32 bandLimit;
    s32 currentY;

    if (glyph != NULL) {
        currentNode = glyph->chainHead;
        if (currentNode != NULL) {
            do {
                scanNode = currentNode;
                currentY = currentNode->y;
                if (currentNode != NULL) {
                    bandLimit = currentY + FR_FONT_POSITION_BAND_HEIGHT;
                    if (currentY < bandLimit) {
                        do {
                            scanNode = scanNode->next;
                        } while (scanNode != NULL && scanNode->y < bandLimit);
                    }
                }
                bandCount++;
                currentNode = scanNode;
            } while (currentNode != NULL);
        }
        return bandCount;
    }
    return 0;
}

u32 frFontMeasureLineWidth(s32 requestedLine, FrFontGlyph *glyph) {
    FrFontGlyph *node;
    s32 bottom;
    s32 currentY;
    u32 width = 0;
    s32 line = 0;

    if (glyph == NULL) {
        return 0;
    }
    glyph = glyph->chainHead;
    if (glyph != NULL) {
        do {
            width = 0;
            node = glyph;
            currentY = glyph->y;
            /* Same null-tolerant band walk as the position query below. */
            if (glyph != NULL) {
                bottom = currentY + FR_FONT_POSITION_BAND_HEIGHT;
                if (currentY < bottom) {
                    do {
                        width += frFontMeasureGlyphChain(node);
                        node = node->next;
                    } while (node != NULL && node->y < bottom);
                }
            }
            if (line++ == requestedLine) {
                break;
            }
            glyph = node;
        } while (glyph != NULL);
    }
    return width << FR_FONT_POSITION_SHIFT;
}

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

/* Read the packed table count, then skip the table and its trailing metadata
 * word to reach the encoded text. The native parser copies that metadata even
 * though this lookup does not use its value. */
u8 *func_0019DE70(s32 textId, FrFontTextBank *bank, s32 mode) {
    u32 offset;
    u32 tableCount;
    u32 textMetadata;

    if (textId >= bank->count) {
        return NULL;
    }
    offset = bank->entries[textId].offset + 0x38;
    tableCount = 0;
    memcpy(&tableCount, (u8 *)bank + offset, 2);
    offset += 4;
    offset += tableCount * 4;
    memcpy(&textMetadata, (u8 *)bank + offset, sizeof(textMetadata));
    offset += sizeof(textMetadata);
    return (u8 *)bank + offset;
}

extern u32 D_004528C0[];
extern s8 D_00436550;

void func_0019DEE0(u8 fontIndex, FrFontCtx *ctx) {
    char glyphText[3];
    const char *textData;
    s32 index;
    s32 textLength;
    s8 previousMode;
    s8 mode;
    FrFontGlyph *context;
    FrFontGlyph *existingGlyph;

    textData = (const char *)D_004528C0[fontIndex & 0xFF];
    memset(glyphText, 0, sizeof(glyphText));
    if (textData == NULL) {
        return;
    }

    textLength = strlen(textData);
    previousMode = ctx->fontIndex;
    for (index = 0; index < textLength; ) {
        glyphText[0] = textData[index++];
        glyphText[1] = textData[index++];
        mode = ctx->fontIndex;

        if (mode != previousMode) {
            context = ctx->glyphChain;
            if (context != NULL) {
                if (context->firstChild != NULL) {
                    ctx->glyphChain = frFontAppendTextToGlyphChain(
                        (const char *)&D_00436578, 0, ctx->firstOption,
                        ctx->secondOption, context);
                }
            } else {
                ctx->glyphChain = frFontAppendTextToGlyphChain(
                    (const char *)&D_00436578, 0, ctx->firstOption,
                    ctx->secondOption, NULL);
            }
            frFontSetContextEncodedByte(ctx->glyphChain, ctx->contextEncodedByte);
            previousMode = mode;
        }

        existingGlyph = frFontBuildGlyphChain(glyphText, mode, ctx->firstOption,
                                              ctx->secondOption, ctx->glyphChain);
        ctx->glyphChain = existingGlyph;
        frFontSetSpacingAndMeasureGlyphs(existingGlyph, D_00436550);
    }
}

/* Append the shared-text child context, apply its cached encoded byte and clear
 * the pending-create flag. Preserve the old-style implicit-argument interface. */
/* Old-style definition: callers invoke it without arguments and rely on $a0. */
void frFontCreateContext(ctx)
    FrFontCtx *ctx;

{
    FrFontGlyph *childGlyph = frFontAppendTextToGlyphChain((const char *)&D_00436578, 0, ctx->firstOption, ctx->secondOption, ctx->glyphChain);

    ctx->glyphChain = childGlyph;
    frFontSetContextEncodedByte(childGlyph, ctx->contextEncodedByte);
    ctx->pendingCreate = 0;
}

/* Cancel creation for an empty child, process pending creation, then process
 * the independent pair refresh. The refresh phase has no NULL-child guard. */
void frFontCheckPendingGlyphState(FrFontCtx *ctx) {
    s8 pending;

    if (ctx->glyphChain == NULL) {
        pending = ctx->pendingCreate;
    }
    else {
        if (ctx->glyphChain->firstChild == NULL) {
            ctx->pendingCreate = 0;
        }
        pending = ctx->pendingCreate;
    }
    if (pending == '\0') {
        pending = ctx->pendingPosition;
    }
    else {
        frFontCreateContext();
        pending = ctx->pendingPosition;
    }
    if (pending != '\0') {
        frFontSetGlyphPosition(ctx->glyphChain, ctx->x, ctx->y);
        ctx->pendingPosition = 0;
    }
}

/* Advance by eight times cursor spacing and request both context updates. */
void frFontAdvanceContextCursor(FrFontCtx *ctx) {
    ctx->y += frFontContextCursorSpacing * FR_FONT_CONTEXT_CURSOR_SCALE;
    ctx->pendingCreate = 1;
    ctx->pendingPosition = 1;
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

INCLUDE_SDATA(const s32, "interface/frFont", D_00436582);

