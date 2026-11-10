#include "sdk_ps2.h"
#include "common.h"
#include "sdf_heap_api.h"
#include "itf.h"
#include "fr_font_glyph_chain_count.h"
#include "fr_font_measure.h"
#include "itf_mem_node.h"
#include "itf_resource_segments.h"
#include "sdf_texture_file.h"
#include "btl.h"

extern void frDelFontStyle(s32 slotId);
#define ITF_NODE_HEADER_BYTES 8
#define ITF_ALLOCATION_HANDLE_BYTES 4
#define ITF_VECTOR_WORD_COUNT 4
#define ITF_RGBA_COMPONENT_COUNT 4
#define ITF_NEUTRAL_COLOR 0x80808080
extern sdfTexHandle_t *itfBackgroundSpriteTexture;
extern u8 frWrapEmptyString[];
extern frFontHandle_t *frCombineFontRequest(frFontHandle_t *, frFontHandle_t *);
extern frFontHandle_t *frCombineFontRequestEx(frFontHandle_t *, frFontHandle_t *, s32);
extern void frSetWhiteSpaceW(s32 StyleNo, u64 Width); /* retail passes a 64-bit width here */
extern void frEnableMode(s32 value);
extern u8 frDisableMode(u8 value);
extern s32 D_003BD818;
void frFontEnsureSlotLoaded(s32 id, const char *path);
extern u16 itfGlyphDecodeTable[];
extern void frDelFontStyle(s32 slotId);
extern frFontHandle_t *frCombineFontRequestEx(frFontHandle_t *previous, frFontHandle_t *next, s32 positionNext);
frFontHandle_t *frCombineFontRequest(frFontHandle_t *previous, frFontHandle_t *next);
frFontHandle_t *frSetFontString(const char *text, s8 fontIndex, s8 firstOption, s8 secondOption, frFontHandle_t *existingGlyph);
extern void frFontEnsureSlotLoaded(s32 slotId, const char *path);
extern u8 *datSkillMessages;
extern u8 *datItemMessages;
extern frFontHandle_t * frSetFontString(const char *text, s8 fontIndex, s8 firstOption, s8 secondOption, frFontHandle_t *existingGlyph);

frFontHandle_t *frwGetStaffFontFRQ(u32 x, u32 y, s32 depth, s32 alt, s32 measureFlag, u64 entryFlag, u32 colors, const char *source) {
    frFontHandle_t *glyph;
    s32 kind = 4;

    if (alt) {
        kind = 5;
    }
    frSetWhiteSpaceW(kind, entryFlag);
    frEnableMode(1);
    frDisableMode(2);
    frDisableMode(0x10);
    glyph = frLinkFontString(source, kind, 0, 0, 0);
    frEnableMode(0x10);
    frEnableMode(2);
    frDisableMode(1);
    frSetFontSpace(glyph, measureFlag);
    frSetDrawPosition(glyph, x, y);
    frSetPriority(glyph, depth << 4);
    frSetFontColor(glyph, colors);
    frSetFontColorNo(glyph, 5);
    return glyph;
}

void frwLoadStaffFont(void) {
    frFontEnsureSlotLoaded(4, "/font/staff1.fnt");
    frFontEnsureSlotLoaded(5, "/font/staff2.fnt");
}

void frwFreeStaffFont(void) {
    frDelFontStyle(4);
    frDelFontStyle(5);
}

/* Decode a two-byte glyph through the DDS1 lookup table; 0xFFFF is missing. */
u32 itfDecodeGlyph(u32 glyphCode) {
    s32 adjusted = (glyphCode & 0xffff) + 0xffff7f80;
    s32 index = ((adjusted & 0xff00) >> 1) + (adjusted & 0x7f);

    if ((u32)index < 0x9b0) {
        return itfGlyphDecodeTable[index];
    }
    return 0xffff;
}

/* Preserve single-byte text and substitute 0x8080 for unmapped glyph pairs. */
void itfConvertText(u8 *output, const char *input) {
    s32 i;
    s32 length = strlen(input);

    for (i = 0; i < length; i++, output++) {
        if (input[i] >= 0) {
            output[0] = input[i];
        } else {
            u32 value = itfDecodeGlyph((u8)input[i + 1] | ((u8)input[i] << 8));
            if (value != 0xffff) {
                output[0] = value >> 8;
                output[1] = value;
            } else {
                output[0] = 0x80;
                output[1] = 0x80;
            }
            output++;
            i++;
        }
    }
}

/* Build a glyph with a fixed 16x18 cell, then attach it to its parent. */
void frwGetSmallFont1FRQ(u32 x, u32 y, s32 depth, u32 colors,
                                    void *glyphSource, frFontHandle_t *parent) {
    frFontHandle_t *glyph;

    glyph = frLinkFontString(glyphSource, 0, 0, 0, 0);
    frSetFontSize(glyph, 0x10, 0x12);
    frSetDrawPosition(glyph, x, y);
    frSetPriority(glyph, depth << 4);
    frSetFontColor(glyph, colors);
    frSetFontSpace(glyph, -4);
    frCombineFontRequestEx(parent, glyph, 0);
}

frFontHandle_t *frwGetFontEx(s32 x, s32 y, s32 depth, u32 colors, const char *text,
                         s8 glyphMode, s8 sharedFlag, frFontHandle_t *previousGlyph) {
    char glyphData[3];
    s32 length;
    s32 position = 0;
    s32 glyphLength;
    frFontHandle_t *chain = previousGlyph;

    length = strlen(text);
    if (sharedFlag == 0) {
        frEnableMode(1);
    }
    frDisableMode(2);

    if (length > 0) {
        do {
            const char *character = text + position;
            frFontHandle_t *glyph;

            if (character[0] >= 0) {
                glyphData[0] = character[0];
                glyphLength = 1;
                glyphData[1] = 0;
            } else {
                glyphData[0] = character[0];
                glyphData[1] = character[1];
                glyphLength = 2;
                glyphData[2] = 0;
            }
            glyph = frLinkFontString(glyphData, glyphMode, 0, 0, 0);
            frSetPriority(glyph, depth << 4);
            frSetFontColor(glyph, colors);
            frSetFontSpace(glyph, 3);
            chain = frCombineFontRequest(chain, glyph);
            if (position == 0) {
                glyph->x = x;
            }
            position += glyphLength;
            glyph->y = y;
        } while (position < length);
    }

    frEnableMode(2);
    if (sharedFlag == 0) {
        frDisableMode(1);
    }
    return chain;
}

frFontHandle_t *frwGetSmallFontFRQ(x, y, depth, colors, text, parent)
s32 x;
s32 y;
s32 depth;
s32 colors;
const char *text;
frFontHandle_t *parent;
{
    frFontHandle_t *handle = frwGetFontEx(x, y, depth, colors, text, 1, 0, parent);

    frSetFontSpace(handle, 3);
    return handle;
}

/* Create positioned, colored text with measured spacing and link it to parent. */
frFontHandle_t *itfCreateMeasuredTextGlyph(s32 x, s32 y, s32 depth, s32 colors,
                           const char *text, frFontHandle_t *parent) {
    return frwGetSmallFontFRQ(x, y, depth, colors, text, parent);
}

/* Decode two-byte glyph codes before building and linking the text glyph. */
frFontHandle_t *itfCreateConvertedTextGlyph(s32 x, s32 y, s32 depth, u32 colors, const u8 *text, frFontHandle_t *parent) {
    u8 buffer[0x400];
    s32 i;
    s32 length = strlen((const char *)text);
    frFontHandle_t *glyph;

    buffer[length] = 0;
    for (i = 0; i < length; i++) {
        u16 code = text[i];

        if (code < 0x80) {
            buffer[i] = code;
        } else {
            u32 decoded;

            code = (code << 8) | text[i + 1];
            decoded = itfDecodeGlyph(code);
            if (decoded != 0xffff) {
                buffer[i] = decoded >> 8;
                buffer[i + 1] = decoded;
            } else {
                buffer[i] = 0x80;
                buffer[i + 1] = 0x80;
            }
            i++;
        }
    }
    frEnableMode(1);
    glyph = frLinkFontString(buffer, 1, 0, 0, 0);
    frEnableMode(2);
    frDisableMode(1);
    frSetDrawPosition(glyph, x, y);
    frSetPriority(glyph, depth << 4);
    frSetFontColor(glyph, colors);
    return frCombineFontRequestEx(parent, glyph, 0);
}

frFontHandle_t *itfCreateFont2TextGlyph(s32 x, s32 y, s32 depth, u32 colors, char *text, frFontHandle_t *previousGlyph) {
    frFontHandle_t *glyph;

    glyph = frLinkFontString((const char *)frWrapEmptyString, 0, 0, 0, previousGlyph);
    frDisableMode(2);
    glyph = frSetFontString(text, 2, 0, 0, glyph);
    frSetFontSpace(glyph, -1);
    frEnableMode(2);
    frSetDrawPosition(glyph, x, y);
    frSetPriority(glyph, depth << 4);
    frSetFontColor(glyph, colors);
    return glyph;
}

/* Build text with font index 3; retain the existing glyph chain's spacing. */
frFontHandle_t *itfCreateFont3TextGlyph(s32 x, s32 y, s32 depth, u32 colors, const char *text, frFontHandle_t *previousGlyph) {
    frFontHandle_t *glyph;

    glyph = frLinkFontString((const char *)frWrapEmptyString, 0, 0, 0, previousGlyph);
    frDisableMode(2);
    glyph = frSetFontString(text, 3, 0, 0, glyph);
    frEnableMode(2);
    frSetDrawPosition(glyph, x, y);
    frSetPriority(glyph, depth << 4);
    frSetFontColor(glyph, colors);
    return glyph;
}

/* Build font-index-3 text with signed spacing -2 and return its glyph chain. */
frFontHandle_t *itfCreateTightFont3TextGlyph(s32 x, s32 y, s32 depth, s32 colors, char *text, frFontHandle_t *previousGlyph) {
    frFontHandle_t *glyph;

    glyph = frLinkFontString((const char *)frWrapEmptyString, 0, 0, 0, previousGlyph);
    frDisableMode(2);
    glyph = frSetFontString(text, 3, 0, 0, glyph);
    frSetFontSpace(glyph, -2);
    frEnableMode(2);
    frSetDrawPosition(glyph, x, y);
    frSetPriority(glyph, depth << 4);
    frSetFontColor(glyph, colors);
    return glyph;
}

/* Attach the alternate font glyph, with a 12x16 cell, to its parent. */
void itfAttachGlyph12x16(u32 x, u32 y, s32 depth, u32 colors,
                                    void *glyphSource, frFontHandle_t *parent) {
    frFontHandle_t *glyph;

    glyph = frSetFontString(glyphSource, 0, 0, 0, 0);
    frSetFontSize(glyph, 0xc, 0x10);
    frSetFontSpace(glyph, 3);
    frSetDrawPosition(glyph, x, y);
    frSetPriority(glyph, depth << 4);
    frSetFontColor(glyph, colors);
    frCombineFontRequestEx(parent, glyph, 0);
}

extern const u8 *MessageNumSearch(s32 textId, const frTextBank_t *bank, s32 mode);
extern void frFontMoveChainTo(s32 x, s32 y, frFontHandle_t *text);

frFontHandle_t *itfDrawBankTextWithLayoutFlags(s32 x, s32 y, s32 depth, u16 textId, frTextBank_t *bank, s32 flags) {
    const u8 *text = MessageNumSearch(textId, bank, 0);
    u32 mode;
    frFontHandle_t *handle;

    if (text == 0) {
        return 0;
    }
    mode = (u16)flags;
    switch (mode) {
    case 1:
        frDisableMode(4);
        break;
    case 2:
        frEnableMode(8);
        break;
    case 4:
        frEnableMode(0x20);
        break;
    }
    handle = MessageRequestNS(x, y, depth, 1, 0, 0, 0x80, text, 0);
    if (flags & 0x10000) {
        s32 maxWidth = 0;
        s32 i;
        s32 width;
        for (i = 0; i < frFontCountGlyphChainNodes(handle); i++) {
            width = frFontMeasureGlyphStripWidth(i, handle);
            if (maxWidth < width) {
                maxWidth = width;
            }
        }
        frFontMoveChainTo(x - maxWidth / 2, y, handle);
    }
    switch (mode) {
    case 1:
        frEnableMode(4);
        break;
    case 2:
        frDisableMode(8);
        break;
    case 4:
        frDisableMode(0x20);
        break;
    }
    return handle;
}

frFontHandle_t *itfDrawTextWithSelectedFontMode(s32 x, s32 y, s32 depth, s8 fontMode, u16 textId, s32 flags) {
    frFontHandle_t *result = NULL;

    itfSetTextDrawLimit(0x13);
    switch (fontMode) {
    case 0:
        result = itfDrawBankTextWithLayoutFlags(x, y, depth, textId, (frTextBank_t *)datSkillMessages, flags);
        break;
    case 1:
        result = itfDrawBankTextWithLayoutFlags(x, y, depth, textId, (frTextBank_t *)datItemMessages, flags);
        break;
    }
    itfSetTextDrawLimit(-1);
    return result;
}

frFontHandle_t *itfDrawUnderscoreTextSegment(x, y, depth, color, text, segmentIndex)
    s32 x;
    s32 y;
    s32 depth;
    u32 color;
    const u8 *text;
    s32 segmentIndex;
{
    u8 buffer[0x200];
    s32 length = strlen((const char *)text);
    s32 segment;
    s32 start;
    s32 count;
    s32 i;

    if (length >= 0x200) {
        return 0;
    }
    segment = 0;
    start = 0;
    count = 0;
    for (i = 0; i < length; i++) {
        s32 separator = 0;

        if (text[i] == '_') {
            segment++;
            separator = 1;
        }
        if (segment == segmentIndex) {
            count++;
        }
        if (segmentIndex < segment || (i >= length - 1 && count > 0)) {
            memcpy(buffer, text + start, count);
            buffer[count] = 0;
            return itfCreateConvertedTextGlyph(x, y, depth, color, buffer, 0);
        }
        if (separator != 0) {
            count = 0;
            start = i + 1;
        }
    }
    return 0;
}

u32 itfReturnZero(void) {
    return 0;
}

void func_00198010(void) {
}

void func_00198018(void) {
}

u32 itfReturnThirdCallbackValue(u32 unused0, u32 unused1, u32 value) {
    return value;
}

/* Return the byte count saved by the most recent packed-resource decode. */
s32 itfGetLastDecodedByteCount(void) {
    return D_003BD818;
}

u32 frGetFontHeader(u32 resourceBase) {
    return resourceBase;
}

/* Resolve three signed, cumulative relative offsets without validating bounds. */
void frGetFontImagePtrs(ItfRelativeSegmentResourceView *block, ItfPackedResourceSegments *out) {
    s32 firstOffset = block->firstOffset;
    s32 secondOffset = firstOffset + block->secondDelta;
    s32 thirdOffset = secondOffset + block->thirdDelta;

    out->treeNodes = (u8 *)block + firstOffset;
    out->entryBitRanges = (ItfBitRange *)((u8 *)block + secondOffset);
    out->packedBitstream = (u16 *)((u8 *)block + thirdOffset);
}

/* Read the opaque header word at 0x10; its role is not established. */
u32 itfGetPackedResourceHeaderWord(u32 resourceBase) {
    return *(u32 *)(frGetFontHeader(resourceBase) + 0x10);
}

/* Decode entryIndex's bit span through the resource's six-byte prefix-tree records.
 * Write terminal bytes to decodedBytes and return/cache the count. The font atlas
 * uses these bytes as pixels; neither entry nor output bounds are checked. */
s32 frDecodeFontRow(u8 *decodedBytes, s32 entryIndex, ItfRelativeSegmentResourceView *resourceBase,
                    ItfPackedResourceSegments *segments) {
    ItfBitRange range;
    s32 produced;
    u8 *table = segments->treeNodes;
    u16 *bitWords;
    u32 wordIndex;
    u32 start;
    s32 remaining;
    u32 state;
    u16 *root;
    u16 *node;

    memcpy(&range.start, (u8 *)segments->entryBitRanges + entryIndex * 4, 4);
    memcpy(&range.end, (u8 *)segments->entryBitRanges + entryIndex * 4 + 4, 4);
    produced = 0;
    frGetFontHeader((u32)resourceBase);
    start = range.start;
    bitWords = segments->packedBitstream;
    wordIndex = start >> 4;
    remaining = range.end - start;
    state = (bitWords[wordIndex] | 0x10000) >> (start & 0xF);
    root = (u16 *)(table + ITF_PREFIX_TREE_ROOT_OFFSET);
    node = root;
    do {
        u16 index = node[state & 1];

        node = (u16 *)(table + index * ITF_PREFIX_TREE_NODE_BYTES + ITF_PREFIX_TREE_ROOT_OFFSET);
        if (*node == 0) {
            decodedBytes[produced++] = ((u8 *)node)[2];
            node = root;
        }
        state >>= 1;
        if (state == 1) {
            wordIndex++;
            state = bitWords[wordIndex] | 0x10000;
        }
        remaining--;
    } while (remaining != 0);
    D_003BD818 = produced;
    return produced;
}

/* Build count usable nodes plus index-zero sentinel, retaining each payload gap.
 * The allocation handle is stored four bytes before the returned ring base. */
MemNode *frAllocFontTable(s32 payloadBytes, s32 count) {
    sdfMemHandle_t *buffer;
    u8 *list;
    MemNode *cursor;
    MemNode *next;
    s32 i;

    buffer = sdfMemAllocDirect((payloadBytes + ITF_NODE_HEADER_BYTES) * (count + 1) + ITF_ALLOCATION_HANDLE_BYTES);
    list = (u8 *)sdfMemLock(buffer);
    i = 0;
    memcpy(list, &buffer, ITF_ALLOCATION_HANDLE_BYTES);
    list += ITF_ALLOCATION_HANDLE_BYTES;
    cursor = (MemNode *)list;
    for (; i < count; i++) {
        next = (MemNode *)((u8 *)cursor + payloadBytes + ITF_NODE_HEADER_BYTES);
        cursor->slotIndex = i;
        cursor->next = next;
        cursor = next;
    }
    cursor->slotIndex = count;
    cursor->next = (MemNode *)list;
    return (MemNode *)list;
}

/* Remove the next free node, or return NULL at the index-zero sentinel. */
void *frAllocFontNode(MemNode *queue) {
    MemNode *head = queue->next;

    if (head->slotIndex == 0) {
        return NULL;
    }
    queue->next = head->next;
    head->next = NULL;
    return head + 1;
}

/* Prepend a detached payload's header; NULL or an already linked node fails.
 * Header pointer formation before the NULL test is retained unchanged. */
s32 frFreeFontNode(void *payload, MemNode *queue) {
    MemNode *node = (MemNode *)payload - 1;
    if (payload == NULL) {
        return 0;
    }
    if (node->next != NULL) {
        return 0;
    }
    node->next = queue->next;
    queue->next = node;
    return 1;
}

/* Release the handle preceding the original ring base, not an acquired payload. */
u32 frFreeFontTable(MemNode *ringBase) {
    sdfMemFreeDirect(((MemRingHeader *)((u8 *)ringBase - ITF_ALLOCATION_HANDLE_BYTES))->allocation);
    return 1;
}

/* Acquire the background texture, then release the temporary file allocation. */
void nbLoadDeadMoonTex(void) {
    u32 resource;
    sdfMemHandle_t *fileAllocation = sdfDevLoadFileEx("/sprite/bg00.tmx", &resource, 0);

    itfBackgroundSpriteTexture = sdfTexSet((SdfTextureFileHeader *)(resource));
    sdfMemFreeDirect(fileAllocation);
}

/* Drop the held texture reference without clearing the global. */
void itfReleaseBackgroundSpriteTexture(void) {
    sdfTexReleaseReference(itfBackgroundSpriteTexture);
}

struct EffSpriteUV;
struct sdfTexHandle_t;
union EffSpriteColor;
extern void itfDrawTexturedSpriteRect(s32 x, s32 y, u32 depth, s32 width, s32 height, const struct EffSpriteUV *uvRect, const union EffSpriteColor *color, u32 flipFlags, u32 blendKind, s32 mode, struct sdfTexHandle_t *texture, s32 surfaceId);

/* Draw only when a texture is present; native X/Y extent scaling differs. */
void itfDrawBackgroundSprite(void) {
    s32 origin[ITF_VECTOR_WORD_COUNT];
    s32 color[ITF_RGBA_COMPONENT_COUNT];
    sdfTexHandle_t *panel = itfBackgroundSpriteTexture;

    if (panel != NULL) {
        s32 x = panel->width;
        s32 y = panel->height;

        origin[0] = 0;
        origin[1] = 0;
        origin[2] = x;
        origin[3] = y;
        color[0] = ITF_NEUTRAL_COLOR;
        color[1] = ITF_NEUTRAL_COLOR;
        color[2] = ITF_NEUTRAL_COLOR;
        color[3] = ITF_NEUTRAL_COLOR;
        itfDrawTexturedSpriteRect(0, 0, 0, x << 4, y << 3, origin, color, 0, 0, 1, (u8 *)panel, 0x52);
    }
}

INCLUDE_SDATA(const s32, "interface/frWrap", frWrapEmptyString);

