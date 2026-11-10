#include "sdk_ps2.h"
#include "common.h"
#include "fr_font_text_slots.h"
#include "sdf_packet_alloc.h"
#include "sdf_packet_list.h"
#include "sdf_heap_api.h"
#include "itf.h"
#include "itf_mem_node.h"
#include "font_mem_control_api.h"
#include "fr_font_draw_glyph.h"
#include "fr_font_position.h"
#include "sdf_image_upload.h"
#include "sdf_chip.h"
#include "fr_font_measure.h"
#include "fr_font_glyph_measure.h"
#include "fr_font_clone_entry_resource.h"
#include "itf_resource_segments.h"
#include "dds3_draw_buffer.h"
#include "dds3_task_runner.h"
#include "btl.h"
#include "mnu_title_sound.h"
#include "scr.h"
#include "itf_bit_range.h"
#include "dds3_admin_control_api.h"
#include "itf_text_vector.h"
#include "itf_text_slots_api.h"
#include "sdf_packet_header.h"
#include "itf_gs_state_packet.h"
#include "fr_font.h"
#include "kernel.h"
#include "dds3_task_create.h"
#include "dds3_task_state.h"
#include "dds3_task_state_query.h"
#include "itf_panel_api.h"
#include "fr_font_glyph_chain_count.h"
#include "sdf.h"
#include "itf_panel_draw.h"
#include "sdf_texture_file.h"
#include "fr_font.h"

/* Declared before first use (were implicit int). */
extern _FRQ * frDestroyFontRequest(_FRQ *glyph);
extern void frDelFontStyle(s32 slotId);
/* Original-style implicit int, kept: matched code depends on it: itfDrawTexturedSpriteRect. */

#define ITF_BYTE_MASK 0xFF

#include "sdf_debug_api.h"

extern void frCreateNewHandle(FrFontCtx *);
extern void frMessageReturnCode(FrFontCtx *);
extern void frFontAppendIndexedTextToGlyphChain(u8, FrFontCtx *);
extern u8 gModeFlag;
extern void mnuSetTitleVoicePrefixIndex(s32);
extern void mnuPlayTitleVoiceFile(char *);
extern s32 mnuGetTitleEffectFrameCounter(void);

extern _FRQ *frCombineFontRequest(_FRQ *, _FRQ *);
extern _FRQ *frCombineFontRequestEx(_FRQ *, _FRQ *, s32);

extern void frEnableMode(s32 value);

extern u8 frDisableMode(u8 value);

extern s32 frDefaultFontHandle;

extern u32 itfTextLipsMotionId;

extern u32 itfTextLipsUnitId;
void frFontEnsureSlotLoaded(s32 id, const char *path);

extern s32 itfScanEncodedTextControls(FrFontCtx *args);

#ifdef NON_MATCHING
extern void fmRetFontMem(FntNode *node);
#endif
/* Declared before first use (were implicit int). */
extern void evtLipsStopFunction(void);
extern void frDelFontStyle(s32 slotId);
/* fmRetFontMem is void in the game font TU; preserve C89 call scope here.
 * An early void declaration moves the cache-count update at +0x5C/+0x60/+0x64
 * from retail $3 to $2 in frFlushGsFontTable. */

extern u32 gMessageStepY;

extern u8 gModeFlag;

extern u32 gFontDrawOT;

extern u8 D_003BB180[];

extern s32 frDefaultFontHandle;

void frFontCreateContext(FrFontCtx *ctx);

extern s32 frEraseEffect(_FRQ *glyph);

extern _FRQ *frFontReleaseGlyphChain(_FRQ *glyph);

extern _FRQ *frSetUserImage(GsFont_t *source, _FRQ *destination);
extern void frInitTextHandle(_TEXT *glyph, s32 glyphId, s32 fontIndex, s32 firstOption, s32 packedFlags, s32 secondOption);
extern void frInitFontHandle(_FRQ *glyph);
extern u32 frFontGetGlyphCellWidth(u8 fontIndex);
extern u32 frFontGetGlyphCellHeight(u8 fontIndex);
extern s8 gFontBaseWidth[];

extern _FRQ *frCombineFontRequestEx(_FRQ *previous, _FRQ *next, s32 positionNext);

_FRQ *frCombineFontRequest(_FRQ *previous, _FRQ *next);
extern u8 D_003BB17C;
_FRQ *frSetFontString(const char *text, s8 fontIndex, s8 firstOption, s8 secondOption, _FRQ *existingGlyph);
_TEXT *frSetFont(u16 glyphId, s32 fontIndexArg, u8 firstOption, u8 secondOption);
extern void frFontEnsureSlotLoaded(s32 slotId, const char *path);

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
#define FR_FONT_DEFAULT_CURSOR_SPACING 0x15
#define FR_FONT_DEFAULT_CELL_SLOT_COUNT 2
#define FR_FONT_POSITION_BAND_HEIGHT 0x54

/* Declared before first use (were implicit int). */
extern void frLoadStyleAll(void);
extern sdfGsMemSlice_t * sdfGsMemAllocFromTail(s32 size, s32 allocationMode);

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

extern u32 frFontLoaded[];
extern void frDelStyleAll(void);
extern _FRQ *frFontReleaseGlyphChain(_FRQ *glyph);

/* Select by the low byte of slotId; enable the entry and store value + 1 as a byte. */
void frSetWhiteSpaceW(s32 slotId, s32 entryFlag) {
    FrFontEntry *entry = &frFontWork.entries[slotId & FR_FONT_BYTE_MASK];

    entry->flagBytes[0] = FR_FONT_ENTRY_ENABLED;
    entry->flagBytes[1] = entryFlag + 1;
}

/* Reverse the four byte positions of a 32-bit word. */
u32 frSwapColor(u32 word) {
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
s32 frConvertVal2N(u32 bitValue) {
    s32 shiftCount = 0;

    if (bitValue == 0) {
        return 0;
    }
    do {
        bitValue = bitValue >> 1;
        shiftCount += 1;
    } while (bitValue != 0);
    return (shiftCount + FR_FONT_BYTE_MASK) & FR_FONT_BYTE_MASK;
}

void frCreateDrawEnvPacket(FrFontGsPacket *packet) {
    QUAD_ZERO(packet->dma.value);
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

/* This allocation owns one 0x90-byte DMA/GIF sprite packet. The command
 * header is emitted as 32-bit words and the GS payload as 64-bit fields;
 * both are direct views of the same newly reserved packet storage.
 */
typedef union FrFontSpritePacketView {
    FrFontSpritePacket sprite;
    u32 words[36];
} FrFontSpritePacketView;

typedef char FrFontSpritePacketViewSizeCheck[
    sizeof(FrFontSpritePacketView) == 0x90 ? 1 : -1];
typedef char FrFontSpritePacketViewLayoutCheck[
    ((u32)&((FrFontSpritePacketView *)0)->sprite.setupTag == 0x10 &&
     (u32)&((FrFontSpritePacketView *)0)->sprite.tex0 == 0x20 &&
     (u32)&((FrFontSpritePacketView *)0)->sprite.corners == 0x60) ? 1 : -1];

extern u32 D_003D6DF8[];
extern sdfGrOt_t sdfGrOtTable[];

/* Build and submit an atlas glyph sprite, preserving the two-corner UV order. */
void frFontDrawAtlasGlyph(s32 x, s32 y, s32 width, s32 height, u8 style,
    u32 color, u32 depth, s32 filtered, const FrFontUvRect *uv,
    const FrFontAtlas *atlas, s32 surface) {
    FrFontSpritePacketView *packet;
    sdfDrawTag_t *list;
    sdfGrOt_t *drawSurface;
    u32 reversedColor;
    s32 right, bottom;
    u64 rgbaq;
    u64 z;
    u64 texBase, texWidth, texBuffer, texHeight;

    reversedColor = frSwapColor(color);
    packet = (FrFontSpritePacketView *)sdfGrAllocPacketBuff(sizeof(FrFontSpritePacketView));
    packet->words[0] = 0x70000008;
    packet->words[3] = 0x50000008;
    packet->sprite.tex1.value = ((u64)filtered << 5) | ((u64)filtered << 6);
    packet->sprite.setupTag = 0x10AF400000000003ULL;
    packet->sprite.setupRegisters = 0xE;
    packet->sprite.tex1.reg = 0x14;
    packet->words[1] = 0;
    packet->words[2] = 0;
    x += 0x7000;
    width <<= 4;
    y += 0x7900;
    height <<= 4;
    right = x + width;
    bottom = y + height;
    texBase = atlas->bufferBase | 0x8000000000000000ULL;
    texWidth = ((u64)atlas->widthExponent << 26) | 0x400000000ULL;
    texBuffer = ((u64)atlas->bufferWidth << 14) | 0x1400000;
    texHeight = ((u64)atlas->heightExponent << 30) | ((u64)D_003D6DF8[style] << 37);
    texBase |= texWidth;
    texBuffer |= texHeight;
    packet->sprite.tex0.reg = 6;
    packet->sprite.clamp.reg = 8;
    packet->sprite.spriteTag = 0x3400000000008002ULL;
    packet->sprite.spriteRegisters = 0x413;
    packet->sprite.tex0.value = texBase | texBuffer;
    packet->sprite.clamp.value = 0;
    rgbaq = reversedColor | 0x3F80000000000000ULL;
    z = (u64)depth << 32;
    packet->sprite.corners[0].uv = (uv->v1 << 16) | uv->u1;
    packet->sprite.corners[0].rgbaq = rgbaq;
    packet->sprite.corners[0].xyz = (u32)((right & 0xFFFF) | (bottom << 16)) | z;
    packet->sprite.corners[1].uv = (uv->v0 << 16) | uv->u0;
    packet->sprite.corners[1].rgbaq = rgbaq;
    packet->sprite.corners[1].xyz = (u32)((x & 0xFFFF) | (y << 16)) | z;
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(sizeof(sdfDrawTag_t));
    sdfDrawTagInit(list);
    sdfDrawTagLink(list, (u32)packet);
    drawSurface = &sdfGrOtTable[surface];
    drawSurface->regFunc(drawSurface, list);
}

/* Submit glyph-relative coordinates and the cache node's owned UV rectangle. */
void frCreateUserFontPacket(s32 x, s32 y, s32 depth, _TEXT *glyph,
                  s32 drawFlags) {
    frFontDrawAtlasGlyph(x + glyph->x, y + glyph->y,
                 glyph->cellDimensions.cellWidth,
                 glyph->cellDimensions.cellHeight >> FR_FONT_GLYPH_HEIGHT_SHIFT,
                 glyph->renderValueOrSetupOrShade.setupBytes.firstOption, glyph->parentDimensionsOrRenderWord.renderWord, depth, 1,
                 &glyph->cachedItem->list->uv, &frFontWork.atlas, drawFlags);
}

extern volatile s32 dds3GsSema; 
/* Upload an image to GS memory at the given buffer, serialized by the GS semaphore. */
void frCopyFontClutToVram(s32 buffer, const u32 *image) {
    u8 loadImage[0x60];

    sceGsSetDefLoadImage(loadImage, (s16)buffer, 1, 0, 0, 0, 8, 2);
    WaitSema(dds3GsSema);
    FlushCache(0);
    sceGsExecLoadImage(loadImage, (s32)image);
    sceGsSyncPath(0, 0);
    SignalSema(dds3GsSema);
}

/* Clear and upload a PSMT4 font bitmap under the GS semaphore, then free the staging allocation. */
void frFontUploadClearedTexture(void) {
    u8 loadImage[FR_FONT_IMAGE_DESCRIPTOR_BYTES];
    s32 imageSize;
    struct sdfMemHandle_t *allocation;
    void *pixels;

    imageSize = frFontWork.atlas.height * frFontWork.atlas.width;
    imageSize = (u32)imageSize >> 1;
    allocation = sdfMemAllocDirect(imageSize);
    pixels = (void *)sdfMemLock(allocation);
    memset(pixels, 0, imageSize);
    sceGsSetDefLoadImage(loadImage, (s16)frFontWork.atlas.bufferBase, (s16)frFontWork.atlas.bufferWidth, FR_FONT_GS_PSMT4, 0, 0,
                         (s16)frFontWork.atlas.width, (s16)frFontWork.atlas.height);
    WaitSema(dds3GsSema);
    FlushCache(0);
    sceGsExecLoadImage(loadImage, (s32)pixels);
    sceGsSyncPath(0, 0);
    SignalSema(dds3GsSema);
    sdfMemFreeDirect(allocation);
}

/* Load only when the slot word is not exactly one; slot one borrows entry zero's allocation. */
void frFontEnsureSlotLoaded(s32 slotId, const char *path) {
    s32 slotIndex = slotId & FR_FONT_BYTE_MASK;
    FrFontSystem *fontSystem = &frFontWork;

    if (frFontLoaded[slotIndex] != FR_FONT_LOADED_STATE) {
        if (slotIndex == FR_FONT_SYSTEM_SLOT) {
            frAddFontStyle(FR_FONT_SYSTEM_SLOT, 0, fontSystem->entries[0].allocation);
        } else {
            frAddFontStyle(slotIndex, 0, sdfDevLoadFileEx(path, 0, 0));
        }
        frFontLoaded[slotIndex] = FR_FONT_LOADED_STATE;
    }
}

extern s32 fmInitFontMemControl(u32 bufferBase, s32 columns, s32 rows, s32 cellWidth,
                                       s32 cellHeight, s32 recordBytes);
extern void frSetFontClut(void);
/* Native interior labels refer to fields of this single font-system owner. */
typedef char FrFontResourceHeaderAliasCheck[((u32)&((FrFontSystem *)0)->entries[0].resourceHeader == 0x4) ? 1 : -1];
typedef char FrFontCachedCountAliasCheck[((u32)&((FrFontSystem *)0)->cachedItemCount == 0x144) ? 1 : -1];
typedef char FrFontItemCountAliasCheck[((u32)&((FrFontSystem *)0)->itemCount == 0x148) ? 1 : -1];
typedef char FrFontGlyphCountAliasCheck[((u32)&((FrFontSystem *)0)->glyphCount == 0x14C) ? 1 : -1];
typedef char FrFontAtlasAliasCheck[((u32)&((FrFontSystem *)0)->atlas == 0x160) ? 1 : -1];
typedef char FrFontImageBuffersAliasCheck[((u32)&((FrFontSystem *)0)->imageBuffers == 0x178) ? 1 : -1];
typedef char FrFontGlyphSlotsAliasCheck[((u32)&((FrFontSystem *)0)->glyphSlots == 0x194) ? 1 : -1];
FrFontSystem frFontWork __attribute__((section(".bss"))) = {0};
u32 frFontIndexedTextSlots[FR_FONT_INDEXED_TEXT_SLOT_COUNT] __attribute__((section(".bss")));
extern s32 D_003BD814;

void frInitFontTable(s32 rowBudget, s32 poolCount) {
    s32 i;
    s32 rowCount;
    s32 glyphsPerRow;
    s32 rowTotal;
    s32 glyphRingCount;
    sdfGsMemSlice_t *atlasHead;

    frFontWork.cachedItemCount = 0;
    frFontWork.itemCount = 0;
    frFontWork.glyphCount = 0;
    for (i = 0; i < 9; i++) {
        frFontWork.entries[i].resource = NULL;
    }
    glyphsPerRow = 21;
    rowCount = rowBudget / glyphsPerRow;
    rowTotal = rowCount;
    glyphRingCount = poolCount;
    poolCount = glyphRingCount * 4;
    frFontWork.atlas.width = 0x200;
    frFontWork.atlas.widthExponent = frConvertVal2N(0x200);
    frFontWork.atlas.height = 1 << (frConvertVal2N(rowTotal * 0x18) + 1);
    frFontWork.atlas.heightExponent = frConvertVal2N(frFontWork.atlas.height);
    frFontWork.textureHead1 = sdfGsMemAllocFromTail(0x180, 3);
    for (i = 0; i < 6; i++) {
        frFontWork.imageBuffers[i] = (frFontWork.textureHead1->addr >> 6) + i;
    }
    atlasHead = sdfGsMemAllocFromTail((u32)(frFontWork.atlas.width * frFontWork.atlas.height) >> 3, 2);
    frFontWork.textureHead0 = atlasHead;
    frFontWork.atlas.bufferBase = atlasHead->addr >> 6;
    frFontWork.atlas.bufferWidth = (u32)frFontWork.atlas.width >> 6;
    for (i = 0; i < 2; i++) {
        frFontWork.glyphSlots[i] = NULL;
    }
    fmInitFontMemControl(frFontWork.atlas.bufferBase, glyphsPerRow, rowCount, 0x18, 0x18, 0xC);
    frFontWork.itemPool = frAllocFontTable(0x2C, poolCount);
    frFontWork.glyphPool = frAllocFontTable(0x44, glyphRingCount);
    frLoadStyleAll();
    frSetFontClut();
    for (i = 0; i < FR_FONT_INDEXED_TEXT_SLOT_COUNT; i++) {
        frFontIndexedTextSlots[i] = 0;
    }
    frFontUploadClearedTexture();
    D_003BD814 = glyphsPerRow * rowTotal;
}

/* Release slot allocations, both glyph chains/rings, and the shared GS resource buffers. */
void frDestroyFontTable(dds3ProcessID_t *task) {
    frDelStyleAll();
    frFontReleaseGlyphChain(frFontWork.glyphSlots[0]);
    frFontReleaseGlyphChain(frFontWork.glyphSlots[1]);
    frFreeFontTable(frFontWork.itemPool);
    frFreeFontTable(frFontWork.glyphPool);
    fmDestroyFontMemControl();
    sdfGsMemFree(frFontWork.textureHead0);
    sdfGsMemFree(frFontWork.textureHead1);
}

/* Bind sections from resource bytes or a retained allocation.
 * Optional flag/value sections each start with a byte-length word; their data
 * precedes the word table and remaining resource data. Offsets are unchecked.
 */
void frAddFontStyle(
    u8 slotIndex, u8 *resourceBytes, struct sdfMemHandle_t *allocation
) {
    FrFontEntry *entry;
    s32 sectionOffset;
    u32 lookupOffset;

    if (resourceBytes == NULL) {
        if (allocation != NULL) {
            resourceBytes = (u8 *)sdfMemLock(allocation);
        }
    }
    entry = &frFontWork.entries[slotIndex];
    entry->allocation = allocation;
    entry->resourceHeader = (FrFontHeader *)resourceBytes;
    sectionOffset = entry->resourceHeader->tableOffset + (entry->resourceHeader->tableCount << FR_FONT_TABLE_ENTRY_SHIFT);
    if (entry->resourceHeader->hasExtra != 0) {
        s32 *flagSection = (s32 *)(resourceBytes + sectionOffset);
        s32 flagDataBytes = *flagSection;
        s32 *valueSection;
        s32 valueDataBytes;
        s32 flagSectionBytes;
        s32 valueSectionBytes;

        entry->flagBytes = (s8 *)(flagSection + 1);
        entry->metricByteCount = flagDataBytes;
        flagSectionBytes = flagDataBytes + FR_FONT_WORD_BYTES;
        sectionOffset += flagSectionBytes;
        valueSection = (s32 *)(resourceBytes + sectionOffset);
        valueDataBytes = *valueSection;
        entry->unk14 = valueSection + 1;
        entry->valueByteCount = valueDataBytes;
        valueSectionBytes = valueDataBytes + FR_FONT_WORD_BYTES;
        sectionOffset += valueSectionBytes;
    } else {
        entry->flagBytes = NULL;
        entry->unk14 = NULL;
        entry->metricByteCount = 0;
        entry->valueByteCount = 0;
    }
    entry->slots = (GsFont_t **)(resourceBytes + sectionOffset);
    sectionOffset += entry->resourceHeader->widthCount * FR_FONT_WORD_BYTES;
    entry->resource = (struct ItfRelativeSegmentResourceView *)(resourceBytes + sectionOffset);
    lookupOffset = entry->resourceHeader->lookupOffset;
    if (lookupOffset != 0) {
        entry->unk20 = (u32)(resourceBytes + lookupOffset);
    } else {
        entry->unk20 = 0;
    }
}

/* Slots zero/one are not freed here; clear the data pointer only when an allocation exists. */
void frDelFontStyle(s32 slotId) {
    u32 slotIndex = slotId & FR_FONT_BYTE_MASK;
    FrFontEntry *entry;

    if (slotIndex < FR_FONT_FIRST_RELEASABLE_SLOT) {
        return;
    }
    frFontLoaded[slotIndex] = 0;
    entry = &frFontWork.entries[slotIndex];
    if (entry->allocation != NULL) {
        sdfMemFreeDirect(entry->allocation);
        entry->resource = NULL;
    }
    entry->slots = NULL;
}

/* Ensure the four named default font files are installed, in slot order. */
void frLoadStyleAll(void) {
    frFontEnsureSlotLoaded(0, "/font/font0.fnt");
    frFontEnsureSlotLoaded(1, "/font/font1.fnt");
    frFontEnsureSlotLoaded(2, "/font/font2.fnt");
    frFontEnsureSlotLoaded(3, "/font/font3.fnt");
}

/* Free each installed entry, including the temporary slot, without reloading. */
void frDelStyleAll(void) {
    s32 fontIndex;

    for (fontIndex = 0; fontIndex < FR_FONT_ENTRY_CAPACITY; fontIndex++) {
        if (frFontWork.entries[fontIndex].resource != NULL) {
            frDelFontStyle((u8)fontIndex);
        }
    }
}

extern u8 frFontClutTable[];
/* This caller passes the full image-buffer word; the callee consumes its low half. */
extern void frCopyFontClutToVram(s32 buffer, const u32 *image);

/* Assemble six 16-word image blocks from little-endian source bytes and upload
 * each to its matching GS buffer. The inner countdown includes zero. */
void frSetFontClut(void) {
    u32 imageWords[FR_FONT_IMAGE_WORD_COUNT];
    s32 bufferIndex = 0;
    s32 sourceWordOffset = 0;
    FrFontSystem *work = &frFontWork;
    u32 *imageBuffer = work->imageBuffers;
    const u8 *sourceTable = frFontClutTable;

    do {
        u32 *outputWord = imageWords;
        const u8 *sourceBytes = (const u8 *)(sourceWordOffset * FR_FONT_IMAGE_WORD_BYTES + (u32)sourceTable);
        s32 wordCountdown = FR_FONT_IMAGE_LAST_WORD;

        do {
            *outputWord = (((sourceBytes[3] << 8) | sourceBytes[2]) << 8 |
                       sourceBytes[1]) << 8 | sourceBytes[0];
            sourceBytes += FR_FONT_IMAGE_WORD_BYTES;
            outputWord++;
            wordCountdown--;
        } while (wordCountdown >= 0);

        frCopyFontClutToVram(*imageBuffer++, imageWords);
        bufferIndex++;
        sourceWordOffset += FR_FONT_IMAGE_WORD_COUNT;
    } while (bufferIndex < FR_FONT_IMAGE_BUFFER_COUNT);
}

/* Remove an unreferenced cached item from its font table, return its resource
 * node to the list and decrement the live cache count. NULL item is a no-op. */
void frFlushGsFontTable(_TEXT *glyph) {
    GsFont_t *cachedItem = glyph->cachedItem;

    if (cachedItem != NULL) {
        if (cachedItem->refs == 0) {
            frFontWork.entries[glyph->renderValueOrSetupOrShade.setupBytes.fontIndex].slots[cachedItem->id] = 0;
            fmRetFontMem(cachedItem->list);
            frFontWork.cachedItemCount--;
        }
    }
}

/* Retain the chain if any fade value changed; otherwise release it and return NULL. */
_FRQ *frDestroyFontRequest(_FRQ *glyph) {
    if (frEraseEffect(glyph) != 0) {
        return glyph;
    }
    return frFontReleaseGlyphChain(glyph);
}

/* Release parents backward and children forward, returning both to their pools.
 * Children with a borrowed source record bypass cached-item reference release. Return NULL. */
_FRQ *frFontReleaseGlyphChain(_FRQ *glyph) {
    _FRQ *currentGlyph = glyph;
    _TEXT *childGlyph;
    _TEXT *nextChildGlyph;
    _FRQ *previousGlyph;

    if (currentGlyph == NULL) {
        return NULL;
    }
    do {
        childGlyph = currentGlyph->firstChild;
        while (childGlyph != NULL) {
            nextChildGlyph = childGlyph->next;
            if (childGlyph->sourceItem == NULL) {
                childGlyph->cachedItem->refs--;
                frFlushGsFontTable(childGlyph);
            }
            frFreeFontNode(childGlyph, frFontWork.itemPool);
            frFontWork.itemCount--;
            childGlyph = nextChildGlyph;
        }
        previousGlyph = currentGlyph->previous;
        frFreeFontNode(currentGlyph, frFontWork.glyphPool);
        currentGlyph = previousGlyph;
        frFontWork.glyphCount--;
    } while (currentGlyph != NULL);
    return NULL;
}

/* Link without repositioning into the draw-buffer-indexed queue; return 0.
 * The low-byte buffer index is used directly, without a two-slot bounds check. */
s32 frDestroyFontRequestAfter(_FRQ *glyph) {
    _FRQ **queueSlot = &frFontWork.glyphSlots[dds3GetFrameID() & 0xFF];

    *queueSlot = frCombineFontRequestEx(*queueSlot, glyph, 0);
    return 0;
}

/* Return the live count of retained font-cache records. */
s32 frGetGsFontNum(void) {
    return frFontWork.cachedItemCount;
}

/* Return the live pooled child-glyph count. */
s32 frGetTextHandleNum(void) {
    return frFontWork.itemCount;
}

/* Return the live pooled parent-glyph count. */
s32 frGetFontHandleNum(void) {
    return frFontWork.glyphCount;
}

/* Glyph setup queries its initial Y here for flag 0x10; retail always returns zero. */
u32 frGetYShiftLev(s8 fontIndex, s32 glyphIndex) {
    (void)fontIndex;
    (void)glyphIndex;
    return 0;
}

extern u16 frGetFontWidth(s32 index);

/* Fill cell dimensions, advance and horizontal bearing for one glyph. */
void frGetXShiftLev(FrFontGlyphMeasureWork *work, s32 glyphIndex) {
    s32 fontIndex;
    s32 defaultSlot = 0;
    s8 right;
    s32 advance;
    s32 leftOffset;
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
    work->advance = gFontBaseWidth[work->fontIndex];

    if (defaultSlot != 0) {
        scale = (f32)(work->cellWidth + 4) /
                (f32)frGetFontWidth(work->fontIndex);
    } else {
        scale = 1.0f;
    }

    if ((work->mode & FR_FONT_TEXT_MEASURE_HALF_ASCII_WIDTH) != 0 && defaultSlot == 0 && work->code < 0x80) {
        work->advance >>= 1;
        work->cellWidth >>= 1;
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
    if (work->code == 0x81CE) {
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
    {
        s32 left = entry->flagBytes[glyphIndex];
        s32 codeIndex;
        s32 code;

        leftOffset = (s32)((u32)left << 4);
        advance = right - left;
        if (defaultSlot != 0) {
            advance = (s32)((f32)advance + (scale + 2.0f));
        }
        codeIndex = glyphIndex >> 1;
        code = codeIndex + 0x20;
        if (code == 0x21 || code == 0x31) {
            leftOffset -= 0x20;
            advance += 4;
        }
        work->advance = advance;
        work->x = -leftOffset;
    }
}

/* Create a glyph wrapper for source and append it to an optional destination chain. */
_FRQ *frSetUserImage(GsFont_t *source, _FRQ *destination) {
    _TEXT *glyph;
    _TEXT *previous;

    if (destination == NULL) {
        destination = frAllocFontNode(frFontWork.glyphPool);
        frFontWork.glyphCount++;
        frInitFontHandle(destination);
    }
    glyph = frAllocFontNode(frFontWork.itemPool);
    previous = destination->lastChild;
    frFontWork.itemCount++;
    frInitTextHandle(glyph, 0, 0, 0, 0xA09DC300, 0);
    if (previous == NULL) {
        destination->firstChild = glyph;
    } else {
        previous->next = glyph;
    }
    glyph->sourceItem = source;
    glyph->advance = gFontBaseWidth[0];
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
_FRQ *frLinkUserImage(GsFont_t *source, _FRQ *destination) {
    _FRQ *referenceChain = frSetUserImage(source, 0);

    if (referenceChain == NULL) {
        return destination;
    }
    return frCombineFontRequest(destination, referenceChain);
}

extern FntNode *fmRentFontMem(void);

/* Retain one atlas item and submit its pixel upload with the requested ownership
 * mode. Coordinates are converted from sixteenths; pool exhaustion is unchecked. */
GsFont_t *frTranslateUserData(s32 width, s32 height, u8 *pixels, FrFontPixelOwnership pixelOwnership) {
    s32 imagePosition[2];
    SdfImageUploadRequest uploadRequest;
    FntNode *atlasNode;
    GsFont_t *cachedItem;

    atlasNode = fmRentFontMem();
    cachedItem = atlasNode->item;
    /* Convert fixed-point atlas placement to integer image coordinates. */
    imagePosition[0] = atlasNode->uv.u0 >> FR_FONT_ATLAS_POSITION_SHIFT;
    imagePosition[1] = atlasNode->uv.v0 >> FR_FONT_ATLAS_POSITION_SHIFT;
    cachedItem->refs = 1;
    cachedItem->list = atlasNode;
    if (pixelOwnership == FR_FONT_PIXELS_BORROWED) {
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
    sdfImageSetReq(&uploadRequest);
    return cachedItem;
}

/* Decode one packed font-resource entry into a fixed-size clone.
 * Neither allocation nor entry index is checked. */
u8 *frDecodeFontImage(u8 fontIndex, s32 entryIndex) {
    FrFontEntry *entry = &frFontWork.entries[fontIndex];
    u8 *clonedResource = sdfChipAllocDirect(FR_FONT_RESOURCE_CLONE_BYTES);
    ItfPackedResourceSegments segments;

    frGetFontImagePtrs(entry->resource, &segments);
    frDecodeFontRow(clonedResource, entryIndex, entry->resource, &segments);
    return clonedResource;
}

extern u16 frGetFontHeight(s32 index);

/* Retain a cached table item, or create/upload a new one and cache it.
 * New records start with one reference; itemIndex is not bounds-checked. */
GsFont_t *frTranslateFont2GS(_TEXT *glyph, s32 itemIndex) {
    FrFontEntry *entry = &frFontWork.entries[glyph->renderValueOrSetupOrShade.setupBytes.fontIndex];
    GsFont_t *cachedItem = entry->slots[itemIndex];
    u8 *clonedResource;

    if (cachedItem != NULL) {
        cachedItem->refs++;
        return cachedItem;
    }
    clonedResource = frDecodeFontImage(glyph->renderValueOrSetupOrShade.setupBytes.fontIndex, itemIndex);
    cachedItem = frTranslateUserData(frGetFontWidth(glyph->renderValueOrSetupOrShade.setupBytes.fontIndex), frGetFontHeight(glyph->renderValueOrSetupOrShade.setupBytes.fontIndex), clonedResource, FR_FONT_PIXELS_TRANSFERRED_TO_UPLOAD);
    cachedItem->id = itemIndex;
    entry->slots[itemIndex] = cachedItem;
    frFontWork.cachedItemCount++;
    return cachedItem;
}

/* Store the font/options, narrowed glyph code and high flag bits, then clear
 * positioning/links. Other glyph fields are deliberately not fully reset here. */
void frInitTextHandle(_TEXT *glyph, s32 glyphId, s32 fontIndex, s32 firstOption, s32 packedFlags, s32 secondOption) {
    glyph->renderValueOrSetupOrShade.setupBytes.fontIndex = fontIndex;
    glyph->renderValueOrSetupOrShade.setupBytes.firstOption = firstOption;
    glyph->renderValueOrSetupOrShade.setupBytes.secondOption = secondOption;
    glyph->glyphCodeOrContext.glyphCode = glyphId;
    glyph->parentDimensionsOrRenderWord.renderWord = packedFlags & ~FR_FONT_BYTE_MASK;
    glyph->renderValueOrSetupOrShade.setupBytes.sharedFlags = gModeFlag;
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
void frInitFontHandle(_FRQ *glyph) {
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

extern void frGetXShiftLev(FrFontGlyphMeasureWork *work, s32 glyphIndex);

_TEXT *frSetFont(u16 glyphId, s32 fontIndexArg, u8 firstOption, u8 secondOption) {
    _TEXT *glyph;
    s32 glyphIndex;
    s32 fontIndex = fontIndexArg & 0xFF;
    FrFontEntry *entry;

    glyph = frAllocFontNode(frFontWork.itemPool);
    frFontWork.itemCount++;
    frInitTextHandle(glyph, glyphId, fontIndex, firstOption, 0xA09DC300, secondOption);

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
    glyph->cachedItem = frTranslateFont2GS(glyph, glyphIndex);
    frGetXShiftLev((FrFontGlyphMeasureWork *)glyph, glyphIndex);
    if (glyph->renderValueOrSetupOrShade.setupBytes.sharedFlags & 0x10) {
        glyph->y = frGetYShiftLev((s8)fontIndex, glyphIndex);
    } else {
        glyph->y = 0;
    }
    return glyph;
}

/* Build text and position its chain after the previous glyph. A NULL build
 * preserves the previous chain. */
_FRQ *frLinkFontString(const char *text, s8 fontIndex, s8 firstOption, s8 secondOption, _FRQ *previousGlyph) {
    _FRQ *newGlyphChain = frSetFontString(text, fontIndex, firstOption, secondOption, 0);

    if (newGlyphChain == NULL) {
        return previousGlyph;
    }
    return frCombineFontRequest(previousGlyph, newGlyphChain);
}

/* Decode text into child glyphs appended to an existing or newly pooled head. */
_FRQ *frSetFontString(const char *text, s8 fontIndex, s8 firstOption, s8 secondOption, _FRQ *existingGlyph) {
    _FRQ *head = existingGlyph;
    _TEXT *previous;
    _TEXT *glyph;
    u32 length = 0;
    u32 textByteOffset;
    u16 code;

    if (fontIndex >= 0) {
        D_003BB17C = fontIndex;
    }
    if (frFontWork.entries[D_003BB17C].resource == NULL) {
        return NULL;
    }
    if (text != NULL) {
        length = strlen(text);
    }
    if (head == NULL) {
        head = frAllocFontNode(frFontWork.glyphPool);
        frFontWork.glyphCount++;
        frInitFontHandle(head);
    }
    previous = head->lastChild;
    glyph = NULL;
    for (textByteOffset = 0; textByteOffset < length; textByteOffset++) {
        code = (u8)text[textByteOffset];
        if (code >= 0x80) {
            textByteOffset++;
            code = (code << 8) | (u8)text[textByteOffset];
        }
        glyph = frSetFont(code, D_003BB17C, (u8)firstOption, (u8)secondOption);
        if (previous == NULL) {
            head->firstChild = glyph;
        } else {
            previous->next = glyph;
        }
        head->advance += glyph->advance;
        head->childCount++;
        glyph->previous = previous;
        previous = glyph;
    }
    if (glyph != NULL) {
        head->lastChild = glyph;
        head->parentDimensionsOrRenderWord.parentDimensions.cellAdvance = glyph->cellDimensions.cellWidth;
        head->parentDimensionsOrRenderWord.parentDimensions.cellHeight = glyph->cellDimensions.cellHeight;
    }
    return head;
}

/* Double the masked input byte and saturate the stored byte at 0x80.
 * Keep the original signed constant used for the saturated byte assignment. */
void frSetDrawSpeed(_FRQ *glyph, s32 inputValue) {
    s32 encodedValue = (inputValue & FR_FONT_BYTE_MASK) * 2;

    if (encodedValue >= FR_FONT_CONTEXT_BYTE_THRESHOLD) {
        glyph->glyphCodeOrContext.byteRoles.encodedContextByte = FR_FONT_CONTEXT_BYTE_CAP;
    } else {
        glyph->glyphCodeOrContext.byteRoles.encodedContextByte = encodedValue;
    }
}

/* Enable the glyph mode and initialize its encoded byte to the cap. */
void frFontEnableContextMode(_FRQ *glyph) {
    glyph->contextModeEnabled = 1;
    frSetDrawSpeed(glyph, FR_FONT_CONTEXT_ENABLE_VALUE);
}

/* Store the signed spacing byte and refresh the cached child-chain advance. */
void frSetFontSpace(_FRQ *glyph, s32 spacing) {
    glyph->glyphCodeOrContext.byteRoles.spacing = spacing;
    glyph->advance = frGetStrWidth(glyph);
}

/* Set the initial glyph's halfword dimensions and every visited child's advance/
 * byte dimensions, then remeasure only the initial glyph. Input must be non-NULL. */
void frSetFontSize(_FRQ *glyph, s32 cellAdvance, s32 cellHeight) {
    _FRQ *targetGlyph = glyph;
    _TEXT *childGlyph;

    targetGlyph->parentDimensionsOrRenderWord.parentDimensions.cellAdvance = cellAdvance;
    targetGlyph->parentDimensionsOrRenderWord.parentDimensions.cellHeight = cellHeight;
    for (; glyph != NULL; glyph = glyph->previous) {
        for (childGlyph = glyph->firstChild; childGlyph != NULL; childGlyph = childGlyph->next) {
            childGlyph->advance = cellAdvance;
            childGlyph->cellDimensions.cellWidth = cellAdvance;
            childGlyph->cellDimensions.cellHeight = cellHeight;
        }
    }
    targetGlyph->advance = frGetStrWidth(targetGlyph);
}

/* Store signed draw coordinates without scaling. */
void frSetDrawPosition(_FRQ *glyph, s32 x, s32 y) {
    glyph->x = x;
    glyph->y = y;
}

/* Store the glyph render value in sixteenths. */
void frSetPriority(_FRQ *glyph, u32 unshiftedValue) {
    glyph->renderValueOrSetupOrShade.renderValue = unshiftedValue >> FR_FONT_CONTEXT_VALUE_SHIFT;
}

/* Assign the first option byte across all visited children; NULL is a no-op. */
void frSetFontColorNo(_FRQ *glyph, u8 firstOption) {
    _TEXT *childGlyph;

    for (; glyph != NULL; glyph = glyph->previous) {
        for (childGlyph = glyph->firstChild; childGlyph != NULL; childGlyph = childGlyph->next) {
            childGlyph->renderValueOrSetupOrShade.setupBytes.firstOption = firstOption;
        }
    }
}

/* Replace each child's full color word across the parent glyph chain. */
void frSetFontColor(_FRQ *parentNode, u32 colorWord) {
    for (; parentNode != NULL; parentNode = parentNode->previous) {
        _TEXT *childNode;
        for (childNode = parentNode->firstChild; childNode != NULL; childNode = childNode->next) {
            childNode->parentDimensionsOrRenderWord.renderWord = colorWord;
        }
    }
}

/* OR the requested mask into the shared byte, preserving implicit narrowing. */
void frEnableMode(s32 mask) {
    mask |= gModeFlag;
    gModeFlag = mask;
}

/* Clear flag bits from the shared font flag byte; returns the previous value. */
u8 frDisableMode(u8 mask) {
    u8 previousFlags = gModeFlag;

    gModeFlag = previousFlags & ~mask;
    return previousFlags;
}

/* Replace the complete shared render-flag word used by subsequent drawing. */
void frSetFontDrawOT(u32 renderFlags) {
    gFontDrawOT = renderFlags;
}

/* Step only second-option-2 children: decrease the low byte by eight, clamp at
 * zero and move y by sixteen while changing it. Return whether any step occurred,
 * not whether the resulting chain still has a nonzero fade value. */
s32 frEraseEffect(_FRQ *glyph) {
    _TEXT *childGlyph;
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
s32 frDrawEffect(_FRQ *parent, _TEXT *glyph, u8 threshold, u8 step) {
    _FRQ *previousParent;
    _TEXT *previousGlyph;
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
            } else if (mnuQueryTitleSoundBusy() != MNU_TITLE_SOUND_IDLE) {
                canAdvanceFade = 0;
            }
            break;
        }
    }

    if (parent->pendingLipsStopCode == ITF_GLYPH_CONTROL_STOP_LIPS && parent->lastChild->parentDimensionsOrRenderWord.fadeByte.value == FR_FONT_FADE_COMPLETE) {
        if (parent->skipLipsStopWait == 0 && parent->remainingWaitFrames > 0) {
            if (parent->remainingWaitFrames == ITF_GLYPH_WAIT_FOR_SOUND_SENTINEL) {
                if (mnuQueryTitleSoundBusy() != MNU_TITLE_SOUND_IDLE) {
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
s32 frDrawFont(_FRQ *glyph) {
    return frDrawFontEx(glyph, 0);
}

/* Pass the selected mode and current shared render-flag word to the renderer. */
s32 frDrawFontEx(_FRQ *glyph, s8 mode) {
    return frDrawFontOt(glyph, mode, gFontDrawOT);
}

/* Draw every child glyph, advance each parent chain, and report its total
 * child-glyph count only while all control/fade conditions remain ready. */
s32 frDrawFontOt(_FRQ *glyph, s8 mode, u32 renderFlags) {
    _TEXT *child;
    s32 ready = 1;
    s32 enabled = 1;
    s32 result = 0;
    s32 totalGlyphCount = 0;
    MnuTitleSoundBusyState titleSoundBusy;
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
                            frDrawEffect(glyph, child, FR_FONT_FADE_PREDECESSOR_THRESHOLD, (u8)glyph->glyphCodeOrContext.byteRoles.encodedContextByte);
                        u32 glyphState;
                        if (child->sourceItem == NULL) {
                            if ((u16)child->glyphCodeOrContext.glyphCode < 0x80) {
                                enabled = 1;
                            }
                            frFontDrawAtlasGlyph(x + child->x + xOffset, y + child->y,
                                         child->cellDimensions.cellWidth,
                                         child->cellDimensions.cellHeight >> 1,
                                         child->renderValueOrSetupOrShade.setupBytes.firstOption, child->parentDimensionsOrRenderWord.renderWord,
                                         glyph->renderValueOrSetupOrShade.renderValue, enabled,
                                         &child->cachedItem->list->uv,
                                         atlas, renderFlags);
                        } else {
                            frFontDrawAtlasGlyph(x + child->x + xOffset, y + child->y,
                                         child->cellDimensions.cellWidth,
                                         child->cellDimensions.cellHeight >> 1,
                                         child->renderValueOrSetupOrShade.setupBytes.firstOption, child->parentDimensionsOrRenderWord.renderWord,
                                         glyph->renderValueOrSetupOrShade.renderValue, enabled,
                                         &child->sourceItem->list->uv,
                                         atlas, renderFlags);
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
                            if (titleSoundBusy != MNU_TITLE_SOUND_IDLE) {
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
        result = totalGlyphCount;
        if (ready == 0) {
            result = 0;
        }
    }
    return result;
}

/* Release the selected frame queue and retain this scheduler callback. */
dds3TaskUpdateResult_t frFlipPacketBuf(dds3ProcessID_t *task) {
    s32 queueIndex = (dds3GetFrameID() & FR_FONT_BYTE_MASK) == 0;

    frFontWork.glyphSlots[queueIndex] = frFontReleaseGlyphChain(frFontWork.glyphSlots[queueIndex]);
    return DDS3_TASK_UPDATE_RETAIN_CALLBACK;
}

/* Link chains with the renderer's relative-positioning option enabled. */
_FRQ *frCombineFontRequest(_FRQ *previous, _FRQ *next) {
    return frCombineFontRequestEx(previous, next, 1);
}

/* Splice chains and return the next chain endpoint. Only option 1 repositions
 * that endpoint; other nonzero options do not. NULL inputs pass through. */
_FRQ *frCombineFontRequestEx(_FRQ *previous, _FRQ *next, s32 positionNext) {
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
void frSetExtDic(u8 *fontData) {
    frAddFontStyle(FR_FONT_TEMPORARY_SLOT, fontData, 0);
}

/* Free the reserved temporary font entry. */
void frDestroyExtDic(void) {
    frDelFontStyle(FR_FONT_TEMPORARY_SLOT);
}

/* Count characters until a zero lead byte. Any negative signed lead consumes
 * two bytes without checking its trailer; input must be a valid packed stream. */
s32 frStrLen(s8 *text) {
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
 * the last. Require a non-NULL parent; return the signed total. */
s32 frGetStrWidth(const _FRQ *parentGlyph) {
    const _TEXT *childGlyph = parentGlyph->firstChild;
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

/* Sum all child advances and signed spacing from the chain origin. This is
 * neither a line count nor a maximum line width; require a non-NULL glyphChain. */
s32 frGetStrWidth2(const _FRQ *glyphChain) {
    const _FRQ *parentGlyph;
    const _TEXT *childGlyph;
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

u32 frFontGetGlyphCellWidth(u8 fontIndex);
u32 frFontGetGlyphCellHeight(u8 fontIndex);
extern void frGetXShiftLev(FrFontGlyphMeasureWork *work, s32 glyphIndex);

s32 frFontMeasureTextAdvance(const u8 *text, u8 fontIndex, FrFontTextMeasureMode mode) {
    FrFontGlyphMeasureWork work;
    s32 totalAdvance = 0;
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
        frGetXShiftLev(&work, glyphIndex);
        offset++;
        totalAdvance += work.advance;
    }
    return totalAdvance;
}

/* Slots 0/1 use the shared default size; later slots read their resource width.
 * Retain the nested signed check and the absence of upper-bound validation. */
u32 frFontGetGlyphCellWidth(u8 fontIndex) {
    s32 entryIndex = fontIndex;

    if (entryIndex < FR_FONT_DEFAULT_CELL_SLOT_COUNT) {
        if (entryIndex >= 0) {
            return frDefaultFontHandle;
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
            return frDefaultFontHandle;
        }
    }
    return frFontWork.entries[entryIndex].resourceHeader->cellHeight;
}

/* Restore this game's cursor spacing (DDS1: 21, DDS2: 25). */
void frFontResetContextCursorSpacing(void) {
    gMessageStepY = FR_FONT_DEFAULT_CURSOR_SPACING;
}

/* Store unscaled cursor spacing; advancement applies its factor of eight later. */
void frFontSetContextCursorSpacing(u32 spacing) {
    gMessageStepY = spacing;
}

/* Count the y-band rows of the glyph chain. DDS1 scans the band but advances
 * only one next node, as frFontGetGlyphPositionAtIndex does. */
s32 frFontCountGlyphChainNodes(const _FRQ *glyph) {
    s32 count = 0;
    const _FRQ *currentNode;
    const _FRQ *scanNode;
    s32 bandLimit;
    s32 currentY;

    if (glyph == NULL) {
        return 0;
    }
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
            currentNode = currentNode->next;
            count++;
        } while (currentNode != NULL);
    }
    return count;
}

s32 frFontMeasureGlyphStripWidth(s32 requestedLine, const _FRQ *glyph) {
    const _FRQ *node;
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
                        width += frGetStrWidth(node);
                        node = node->next;
                    } while (node != NULL && node->y < bottom);
                }
            }
            if (line++ == requestedLine) {
                break;
            }
            glyph = glyph->next;
        } while (glyph != NULL);
    }
    return (s32)(width << FR_FONT_POSITION_SHIFT);
}

/* Write each visited position and stop at requestedIndex. Missing indices leave
 * the last visited position; empty input leaves output unchanged. DDS1 still
 * scans the y-band but advances only one next node, unlike DDS2's band advance. */
void frFontGetGlyphPositionAtIndex(FrFontPosition *position, s32 requestedIndex, const _FRQ *glyph) {
    s32 positionIndex = 0;
    const _FRQ *currentNode;
    const _FRQ *scanNode;
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
void frFontMoveChainTo(s32 targetX, s32 targetY, _FRQ *glyphChain) {
    _FRQ *currentGlyph;
    s32 dx;
    s32 dy;

    if (glyphChain != NULL) {
        currentGlyph = glyphChain->chainHead;
        dx = targetX - currentGlyph->x;
        dy = targetY - currentGlyph->y;
        for (; currentGlyph != NULL; currentGlyph = currentGlyph->next) {
            currentGlyph->x += dx;
            currentGlyph->y += dy;
        }
    }
}

#ifdef NON_MATCHING
#include "common.h"

#define FR_FONT_POSITION_BAND_HEIGHT 0x54 /* a text line's band: glyphs within it share a line */

/* Move a glyph chain so its first line starts at y and each line is centred
 * on x: a line is the glyphs from its first one whose y is within the band
 * height of it. Every glyph moves by the first line's y offset; each line's
 * glyphs move by x minus the line's first x, less half the line's measured
 * width (in 1/16 pixels). Retail reads the chain head's y before checking
 * the head for null; this reads it after. Upstream: INCLUDE_ASM in
 * interface/frFont; written from the retail routine. The same calls and stores;
 * scheduling differs. DDS2's body is this one with its own band height
 * (game/dds2/bodies/interface/frFont/). */
void frFontCenterGlyphChainAt(s32 x, s32 y, _FRQ *chain) {
    _FRQ *glyph;
    s32 deltaY;

    if (chain == NULL || chain->chainHead == NULL) {
        return;
    }
    glyph = chain->chainHead;
    deltaY = y - glyph->y;
    do {
        s32 limit = glyph->y + FR_FONT_POSITION_BAND_HEIGHT;
        s32 width = 0;
        s32 deltaX;
        _FRQ *scan;

        for (scan = glyph; scan != NULL && scan->y < limit; scan = scan->next) {
            width += frGetStrWidth(scan);
        }
        deltaX = x - glyph->x;
        while (glyph != NULL && glyph->y < limit) {
            glyph->y += deltaY;
            glyph->x += deltaX - (width / 2) * 16;
            glyph = glyph->next;
        }
    } while (glyph != NULL);
}
#else
INCLUDE_ASM(const s32, "interface/frFont", frFontCenterGlyphChainAt);
#endif

/* Return a borrowed read-only bank view. Read the packed table count, skip the
 * table and trailing metadata word; the parser copies that metadata too. */
const u8 *MessageNumSearch(s32 textId, const FrFontTextBank *bank, s32 mode) {
    u32 offset;
    u32 tableCount;
    u32 textMetadata;

    if (textId >= bank->count) {
        return NULL;
    }
    offset = bank->entries[textId].offset + 0x38;
    tableCount = 0;
    memcpy(&tableCount, (const u8 *)bank + offset, 2);
    offset += 4;
    offset += tableCount * 4;
    memcpy(&textMetadata, (const u8 *)bank + offset, sizeof(textMetadata));
    offset += sizeof(textMetadata);
    return (const u8 *)bank + offset;
}

extern s8 gMessageSpaceX;

void frFontAppendIndexedTextToGlyphChain(u8 fontIndex, FrFontCtx *ctx) {
    char glyphText[3];
    const char *textData;
    s32 textByteOffset;
    s32 textLength;
    s8 previousMode;
    s8 mode;
    _FRQ *context;
    _FRQ *existingGlyph;

    textData = (const char *)frFontIndexedTextSlots[fontIndex & 0xFF];
    memset(glyphText, 0, sizeof(glyphText));
    if (textData == NULL) {
        return;
    }

    textLength = strlen(textData);
    previousMode = ctx->fontIndex;
    for (textByteOffset = 0; textByteOffset < textLength; ) {
        glyphText[0] = textData[textByteOffset++];
        glyphText[1] = textData[textByteOffset++];
        mode = ctx->fontIndex;

        if (mode != previousMode) {
            context = ctx->glyphChain;
            if (context != NULL) {
                if (context->firstChild != NULL) {
                    ctx->glyphChain = frLinkFontString(
                        (const char *)D_003BB180, 0, ctx->firstOption,
                        ctx->secondOption, context);
                }
            } else {
                ctx->glyphChain = frLinkFontString(
                    (const char *)D_003BB180, 0, ctx->firstOption,
                    ctx->secondOption, NULL);
            }
            frSetDrawSpeed(ctx->glyphChain, ctx->contextEncodedByte);
            previousMode = mode;
        }

        existingGlyph = frSetFontString(glyphText, mode, ctx->firstOption,
                                       ctx->secondOption, ctx->glyphChain);
        ctx->glyphChain = existingGlyph;
        frSetFontSpace(existingGlyph, gMessageSpaceX);
    }
}

/* Append the shared-text child context, apply its cached encoded byte and clear
 * the pending-create flag. */
void frFontCreateContext(FrFontCtx *ctx) {
    _FRQ *childGlyph = frLinkFontString((const char *)&D_003BB180, 0, ctx->firstOption, ctx->secondOption, ctx->glyphChain);

    ctx->glyphChain = childGlyph;
    frSetDrawSpeed(childGlyph, ctx->contextEncodedByte);
    ctx->pendingCreate = 0;
}

/* Cancel creation for an empty child, process pending creation, then process
 * the independent pair refresh. The refresh phase has no NULL-child guard. */
void frCreateNewHandle(FrFontCtx *ctx) {
    s8 pending;

    if (ctx->glyphChain == NULL) {
        pending = ctx->pendingCreate;
    } else {
        if (ctx->glyphChain->firstChild == NULL) {
            ctx->pendingCreate = 0;
        }
        pending = ctx->pendingCreate;
    }
    if (pending == 0) {
        pending = ctx->pendingPosition;
    } else {
#ifdef NON_MATCHING
        frFontCreateContext(ctx);
#else
        /* Retail passes nothing and leaves ctx in $a0 from entry. */
        ((void (*)())frFontCreateContext)();
#endif
        pending = ctx->pendingPosition;
    }
    if (pending != 0) {
        frSetDrawPosition(ctx->glyphChain, ctx->x, ctx->y);
        ctx->pendingPosition = 0;
    }
}

/* Advance by eight times cursor spacing and request both context updates. */
void frMessageReturnCode(FrFontCtx *ctx) {
    ctx->y += gMessageStepY * FR_FONT_CONTEXT_CURSOR_SCALE;
    ctx->pendingCreate = 1;
    ctx->pendingPosition = 1;
}

s8 gMessageSpaceX __attribute__((section(".sdata"))) = 0;

u32 gMessageStepY __attribute__((section(".sdata"))) = 21;

s32 frDefaultFontHandle __attribute__((section(".sdata"))) = 20;

u32 itfTextLipsUnitId __attribute__((section(".sdata"))) = 0;

u32 itfTextLipsMotionId __attribute__((section(".sdata"))) = 0;

u8 gModeFlag __attribute__((section(".sdata"))) = 0x16;

u32 gFontDrawOT __attribute__((section(".sdata"))) = 0x54;

/* Subtract one modulo 256 from the first byte, then advance over the whole pair. */
u32 itfReadEncodedTextLead(FrFontCtx *args) {
    s32 *position = &args->encodedTextOffset;
    const u8 *byte = args->encodedText + *position;
    u32 value = *byte;

    *position += 2;
    return (value + ITF_BYTE_MASK) & ITF_BYTE_MASK;
}

/* Decode a little-endian pair; encoded high byte 0xFF is the zero escape.
 * Neither reader checks the input length. */
u32 itfReadEncodedCode(FrFontCtx *args) {
    const u8 *bytes = args->encodedText;
    u32 first;
    u32 second;

    first = (bytes[args->encodedTextOffset++] + ITF_BYTE_MASK) & ITF_BYTE_MASK;
    second = bytes[args->encodedTextOffset++];
    if (second == ITF_BYTE_MASK) {
        second = 0;
    } else {
        second = (second + ITF_BYTE_MASK) & ITF_BYTE_MASK;
    }
    return (second << 8) | first;
}

extern u32 D_003BB15C;

extern FrFontSystem frFontWork;

s32 itfProcessEncodedTextControl(s32 code, FrFontCtx *stream) {
    s32 *position = &stream->encodedTextOffset;
    const u8 *bytes = stream->encodedText;
    s32 payloadWords = code & 0xF;
    s32 payloadPosition = *position;

    code = (code << 8) | bytes[payloadPosition++];
    *position = payloadPosition;
    switch (code) {
    case 0xF206:
        stream->fontIndex = bytes[payloadPosition] - 1;
        *position = payloadPosition + 2;
        break;
    case 0xF202:
        stream->firstOption = bytes[payloadPosition] - 1;
        *position = payloadPosition + 2;
        break;
    case 0xF209:
        stream->secondOption = bytes[payloadPosition] - 1;
        *position = payloadPosition + 2;
        break;
    case 0xF207:
        stream->contextEncodedByte = bytes[payloadPosition] - 1;
        *position = payloadPosition + 2;
        break;
    case 0xF203:
        if (frFontIndexedTextSlots[bytes[*position] - 1] != 0) {
            frCreateNewHandle(stream);
            frFontAppendIndexedTextToGlyphChain((u8)(stream->encodedText[*position] - 1), stream);
        }
        *position += 2;
        break;
    case 0xF20E:
        *position = payloadPosition + 1;
        break;
    case 0xF10F:
        if (!(gModeFlag & 4)) {
            /* Retail 0x00196730 b .L00196774: advanceLine is also entered by
             * fall-through from the beql at 0x0019676C. */
            goto advanceLine;
        }
        break;
    case 0xF110:
        D_003BB15C |= 2;
        return 1;
    case 0xF111:
        if (!(gModeFlag & 8)) {
            /* Retail 0x00196750 beqz .L00196768: checkAutomaticLine has one entry
             * and falls into the shared advanceLine call at 0x00196774. */
            goto checkAutomaticLine;
        }
        /* This flagged form requests the same stop as opcode F104. */
    case 0xF104:
        D_003BB15C |= 1;
        return 1;

checkAutomaticLine:
        if (!(gModeFlag & 0x20)) {
            break;
        }
advanceLine:
        frMessageReturnCode(stream);
        break;
    case 0xF112:
        D_003BB15C |= 4;
        break;
    case 0xF413:
        mnuSetTitleVoicePrefixIndex(itfReadEncodedTextLead(stream));
        mnuPlayTitleVoiceFile((char *)itfReadEncodedCode(stream));
        break;
    case ITF_GLYPH_CONTROL_WAIT_FRAMES:
        if (stream->glyphChain->pendingLipsStopCode != 0) {
            stream->glyphChain->skipLipsStopWait = 1;
        }
        stream->glyphChain->timedControlCode = code;
        stream->glyphChain->remainingWaitFrames = itfReadEncodedCode(stream);
        break;
    case ITF_GLYPH_CONTROL_WAIT_FRAME_OR_SOUND:
        if (stream->glyphChain->pendingLipsStopCode != 0) {
            stream->glyphChain->skipLipsStopWait = 1;
        }
        stream->glyphChain->timedControlCode = code;
        stream->glyphChain->remainingWaitFrames = itfReadEncodedCode(stream);
        if (stream->glyphChain->remainingWaitFrames != ITF_GLYPH_WAIT_FOR_SOUND_SENTINEL) {
            s32 frame = mnuGetTitleEffectFrameCounter();
            stream->glyphChain->remainingWaitFrames -= frame;
        }
        if (stream->glyphChain->remainingWaitFrames < 0) {
            stream->glyphChain->remainingWaitFrames = 0;
        }
        break;
    case 0xF416:
        itfTextLipsUnitId = itfReadEncodedTextLead(stream);
        itfTextLipsMotionId = itfReadEncodedTextLead(stream);
        D_003BB15C |= 8;
        break;
    case ITF_GLYPH_CONTROL_STOP_LIPS:
        stream->glyphChain->pendingLipsStopCode = code;
        break;
    case 0xF20A:
    case 0xF20B:
    case 0xF20C:
    case 0xF20D:
        break;
    default:
        stream->encodedTextOffset += (payloadWords - 1) << 1;
        break;
    }
    if (stream->pendingCreate == 0) {
        stream->pendingCreate = 1;
    }
    return 0;
}

extern s8 gMessageSpaceX;

extern _FRQ * frSetFontString(const char *text, s8 fontIndex, s8 firstOption, s8 secondOption, _FRQ *existingGlyph);

_FRQ *itfBuildGlyphChainFromEncodedStream(FrFontCtx *stream) {
    u8 encodedText[3];
    s32 value;
    const u8 *bytes = stream->encodedText;

    if ((bytes[0] & 0xF0) != 0xF0 && bytes[1] == 8) {
        stream->encodedTextOffset += 4;
    }
    D_003BB15C &= ~1;
    D_003BB15C &= ~2;
    D_003BB15C &= ~4;
    D_003BB15C &= ~8;
    frEnableMode(1);
    frDisableMode(2);
    bytes = stream->encodedText;
    for (;;) {
        value = bytes[stream->encodedTextOffset++];
        if (value == 0) {
            break;
        }
        if ((value & 0xF0) == 0xF0) {
            if (itfProcessEncodedTextControl(value, stream)) {
                break;
            }
        } else if (value == 10) {
            if (!(gModeFlag & 4)) {
                continue;
            }
            frMessageReturnCode(stream);
        } else {
            frCreateNewHandle(stream);
            if (value < 0x80) {
                encodedText[0] = value;
                encodedText[1] = 0;
                stream->glyphChain = frSetFontString((const char *)encodedText, stream->fontIndex,
                    stream->firstOption, stream->secondOption, stream->glyphChain);
            } else {
                value = (value << 8) | stream->encodedText[stream->encodedTextOffset++];
                if (value <= 0xC07F) {
                    encodedText[0] = (u32)value >> 8;
                    encodedText[1] = value;
                    encodedText[2] = 0;
                    stream->glyphChain = frSetFontString((const char *)encodedText, stream->fontIndex,
                        stream->firstOption, stream->secondOption, stream->glyphChain);
                } else {
                    value -= 0x4000;
                    encodedText[0] = (u32)value >> 8;
                    encodedText[1] = value;
                    encodedText[2] = 0;
                    stream->glyphChain = frSetFontString((const char *)encodedText, 8,
                        stream->firstOption, stream->secondOption, stream->glyphChain);
                }
            }
            frSetFontSpace(stream->glyphChain, gMessageSpaceX);
            frSetPriority(stream->glyphChain, stream->z);
        }
        bytes = stream->encodedText;
    }
    frDisableMode(1);
    frEnableMode(2);
    return stream->glyphChain;
}

_FRQ *MessageRequest(s32 x, s32 y, const u8 *encodedText, _FRQ *sub) {
    return MessageRequestEx2(x, y, 0, 0, 0, 0, 0x80, encodedText, sub);
}

_FRQ *MessageRequestEx(s32 x, s32 y, s32 channel0, s32 channel1, s32 channel2, s32 channel3, const u8 *encodedText, _FRQ *sub) {
    return MessageRequestEx2(x, y, 0, channel0 & 0xFF, channel1 & 0xFF, channel2 & 0xFF, channel3 & 0xFF, encodedText, sub);
}

extern _FRQ *itfBuildGlyphChainFromEncodedStream(FrFontCtx *);

_FRQ *MessageRequestEx2(s32 x, s32 y, s32 depth, s32 channel0, s32 channel1, s32 channel2, s32 channel3, const u8 *encodedText, _FRQ *sub) {
    FrFontCtx args;

    args.x = x;
    args.y = y;
    args.z = depth << 4;
    args.fontIndex = channel0;
    args.firstOption = channel1;
    args.secondOption = channel2;
    args.contextEncodedByte = channel3;
    args.encodedText = encodedText;
    args.glyphChain = sub;
    args.encodedTextOffset = 0;
    args.pendingCreate = 1;
    args.pendingPosition = 1;
    return itfBuildGlyphChainFromEncodedStream(&args);
}

_FRQ *MessageRequestNS(s32 x, s32 y, s32 depth, s32 channel0, s32 channel1,
                   s32 channel2, s32 channel3, const u8 *encodedText, _FRQ *unusedSub) {
    return MessageRequestEx2(x, y, depth, channel0 & 0xff, channel1 & 0xff,
                  channel2 & 0xff, channel3 & 0xff, encodedText, 0);
}

u32 frGetMessageResult(u32 mask) {
    return D_003BB15C & mask;
}

/* Store a replacement-text byte pointer in the font's indexed table. */
void frSetVariableRequest(s32 slotIndex, const u8 *encodedText) {
    frFontIndexedTextSlots[slotIndex] = (u32)encodedText;
}

/* Return the unit identifier saved by text lip-sync control 0xF416. */
u32 itfGetTextLipsUnitId(void) {
    return itfTextLipsUnitId;
}

/* Return the motion number saved by text lip-sync control 0xF416. */
u32 itfGetTextLipsMotionId(void) {
    return itfTextLipsMotionId;
}

#ifdef NON_MATCHING
#include "common.h"
#include "itf_bit_range.h"
#include "dds3_admin_control_api.h"
#include "itf_text_vector.h"
#include "itf_text_slots_api.h"
#include "fr_font_text_slots.h"
#include "sdf_packet_list.h"
#include "sdf_packet_header.h"
#include "itf_gs_state_packet.h"
#include "fr_font.h"
#include "kernel.h"
#include "dds3_task_create.h"
#include "dds3_task_state.h"
#include "sdf_heap_api.h"
#include "itf.h"
#include "fr_font_glyph_chain_count.h"
#include "itf_mem_node.h"
#include "itf_resource_segments.h"
#include "sdf.h"
#include "itf_panel_draw.h"
#include "sdf_texture_file.h"
#include "btl.h"
#include "fr_font.h"
#include "sdf_debug_api.h"
#include "fr_font_atlas_packet.h"
#include "dds3_draw_buffer.h"
extern void frCreateDrawEnvPacket(FrFontGsPacket *packet);

extern FrFontSystem frFontWork;

extern sdfGrOt_t sdfGrOtTable[];
extern sdfDrawTag_t frFontSetupPacketLists[2];
extern FrFontGsPacket frFontSetupPackets[2];
extern sdfDrawTag_t frFontAtlasPacketLists[2];
extern FrFontAtlasPacket frDumpFontTexturePacket[2];

#define FR_FONT_ATLAS_SURFACE 83 /* the draw surface the atlas is submitted through */

/* Draws the whole glyph atlas as a textured four-corner strip at a screen
 * position and submits it, with the GS setup packet, through the current draw
 * buffer's packet lists. The matching
 * build uses INCLUDE_ASM; this is written from the retail routine. It differs from retail only in register allocation
 * and in stack spills of intermediate values. */
void frDumpFontTexture(s32 x, s32 y, s32 depth) {
    FrFontAtlasPacket *packet;
    FrFontGsPacket *setup;
    FrFontAtlasDraw *draw;
    s32 x1;
    s32 y1;
    s32 u1;
    s32 v1;
    u8 buffer;

    buffer = dds3GetFrameID();
    x = (x << 4) + 0x7000;
    y = (y << 3) + 0x7900;
    x1 = x + (frFontWork.atlas.width << 4);
    y1 = y + (frFontWork.atlas.height << 3);
    u1 = frFontWork.atlas.width << 4;
    v1 = frFontWork.atlas.height << 4;

    packet = &frDumpFontTexturePacket[buffer];
    setup = &frFontSetupPackets[buffer];
    draw = &packet->draw;
    packet->dma.words[0] = 0x70000010;
    packet->dma.words[1] = 0;
    packet->dma.words[2] = 0;
    packet->dma.words[3] = 0x50000010;
    draw->gifTag = 0x10AE40000000800FULL;
    draw->gifRegisters = 0xE;
    draw->tex0.value = (u64)frFontWork.atlas.bufferBase |
                       ((u64)frFontWork.atlas.bufferWidth << 14) | (0x14ULL << 20) |
                       ((u64)frFontWork.atlas.widthExponent << 26) |
                       ((u64)frFontWork.atlas.heightExponent << 30) | (1ULL << 34) |
                       ((u64)frFontWork.imageBuffers[0] << 37) | (4ULL << 61);
    draw->tex0.reg = 6;
    draw->tex1.value = 0x60;
    draw->tex1.reg = 0x14;
    draw->clamp.value = 5;
    draw->clamp.reg = 8;

    draw->corners[0].uv.value = (u64)u1 | ((u64)v1 << 16);
    draw->corners[0].uv.reg = 3;
    draw->corners[0].rgbaq.value = 0x3F800000FFFFFFFFULL;
    draw->corners[0].rgbaq.reg = 1;
    draw->corners[0].xyz.value = (u64)x1 | ((u64)y1 << 16) | ((u64)depth << 32);
    draw->corners[0].xyz.reg = 4;
    draw->corners[1].uv.value = (u64)v1 << 16;
    draw->corners[1].uv.reg = 3;
    draw->corners[1].rgbaq.value = 0x3F800000FFFFFFFFULL;
    draw->corners[1].rgbaq.reg = 1;
    draw->corners[1].xyz.value = (u64)x | ((u64)y1 << 16) | ((u64)depth << 32);
    draw->corners[1].xyz.reg = 4;
    draw->corners[2].uv.value = (u64)u1;
    draw->corners[2].uv.reg = 3;
    draw->corners[2].rgbaq.value = 0x3F800000FFFFFFFFULL;
    draw->corners[2].rgbaq.reg = 1;
    draw->corners[2].xyz.value = (u64)x1 | ((u64)y << 16) | ((u64)depth << 32);
    draw->corners[2].xyz.reg = 4;
    draw->corners[3].uv.value = 0;
    draw->corners[3].uv.reg = 3;
    draw->corners[3].rgbaq.value = 0x3F800000FFFFFFFFULL;
    draw->corners[3].rgbaq.reg = 1;
    draw->corners[3].xyz.value = (u64)x | ((u64)y << 16) | ((u64)depth << 32);
    draw->corners[3].xyz.reg = 4;

    frCreateDrawEnvPacket(setup);
    sdfDrawTagInit(&frFontSetupPacketLists[buffer]);
    sdfDrawTagLink(&frFontSetupPacketLists[buffer], (u32)setup);
    sdfGrOtTable[FR_FONT_ATLAS_SURFACE].regFunc(&sdfGrOtTable[FR_FONT_ATLAS_SURFACE],
                                                   &frFontSetupPacketLists[buffer]);
    sdfDrawTagInit(&frFontAtlasPacketLists[buffer]);
    sdfDrawTagLink(&frFontAtlasPacketLists[buffer], (u32)packet);
    sdfGrOtTable[FR_FONT_ATLAS_SURFACE].regFunc(&sdfGrOtTable[FR_FONT_ATLAS_SURFACE],
                                                   &frFontAtlasPacketLists[buffer]);
}
#else
INCLUDE_ASM(const s32, "interface/frFont", frDumpFontTexture);
#endif

s32 itfSkipEncodedTextOpcode(s32 code, FrFontCtx *stream) {
    s32 *position = &stream->encodedTextOffset;
    const u8 *bytes = stream->encodedText;
    s32 payloadWords = code & 0xF;
    s32 payloadPosition = *position;

    code = (code << 8) | bytes[payloadPosition++];
    *position = payloadPosition;
    switch (code) {
    case 0xF20E:
        *position = payloadPosition + 1;
        break;
    case 0xF413:
        *position = payloadPosition + 4;
        return 2;
    case 0xF206:
        *position = payloadPosition + 2;
        break;
    case 0xF202:
        *position = payloadPosition + 2;
        break;
    case 0xF209:
        *position = payloadPosition + 2;
        break;
    case 0xF207:
        *position = payloadPosition + 2;
        break;
    case 0xF203:
        *position = payloadPosition + 2;
        break;
    case 0xF214:
        *position = payloadPosition + 2;
        break;
    case 0xF215:
        *position = payloadPosition + 2;
        break;
    case 0xF416:
        *position = payloadPosition + 4;
        break;
    case 0xF10F:
        break;
    case 0xF110:
        return 1;
    case 0xF111:
    case 0xF112:
        break;
    case 0xF104:
        return 1;
    case 0xF117:
        break;
    case 0xF20A:
    case 0xF20B:
    case 0xF20C:
    case 0xF20D:
        break;
    default:
        stream->encodedTextOffset += (payloadWords - 1) << 1;
        break;
    }
    return 0;
}

s32 itfScanEncodedTextControls(FrFontCtx *stream) {
    u8 encodedText[3];
    s32 value;
    const u8 *bytes = stream->encodedText;

    if ((bytes[0] & 0xF0) != 0xF0 && bytes[1] == 8) {
        stream->encodedTextOffset += 4;
    }
    bytes = stream->encodedText;
    for (;;) {
        value = bytes[stream->encodedTextOffset++];
        if (value == 0) {
            break;
        }
        if ((value & 0xF0) == 0xF0) {
            s32 result = itfSkipEncodedTextOpcode(value, stream);

            if (result == 1) {
                break;
            }
            if (result == 2) {
                return 1;
            }
        } else if (value == 10) {
            continue;
        } else if (value < 0x80) {
            encodedText[0] = value;
            encodedText[1] = 0;
        } else {
            value = (value << 8) | stream->encodedText[stream->encodedTextOffset++];
            if (value <= 0xC07F) {
                encodedText[0] = (u32)value >> 8;
                encodedText[1] = value;
                encodedText[2] = 0;
            } else {
                value -= 0x4000;
                encodedText[0] = (u32)value >> 8;
                encodedText[1] = value;
                encodedText[2] = 0;
            }
        }
        bytes = stream->encodedText;
    }
    return 0;
}

s32 itfInitTextDrawArgs(const u8 *encodedText, _FRQ *sub) {
    FrFontCtx args;

    args.x = 0;
    args.y = 0;
    args.z = 0;
    args.fontIndex = 0;
    args.firstOption = 0;
    args.secondOption = 0;
    args.contextEncodedByte = 0;
    args.encodedText = encodedText;
    args.glyphChain = sub;
    args.encodedTextOffset = 0;
    args.pendingCreate = 1;
    args.pendingPosition = 1;
    return itfScanEncodedTextControls(&args);
}

/* Return the selected resource header's cell width; index is unchecked. */
u16 frGetFontWidth(s32 slotIndex) {
    return frFontWork.entries[slotIndex].resourceHeader->cellWidth;
}

/* Return the selected resource header's cell height; index is unchecked. */
u16 frGetFontHeight(s32 slotIndex) {
    return frFontWork.entries[slotIndex].resourceHeader->cellHeight;
}

void itfSetTextDrawLimit(s32 limit) {
    if (limit < 1) {
        limit = 0x14;
    }
    frDefaultFontHandle = limit;
}

INCLUDE_SDATA(const s32, "interface/frFont", D_003BB17C);

INCLUDE_SDATA(const s32, "interface/frFont", D_003BB180);

