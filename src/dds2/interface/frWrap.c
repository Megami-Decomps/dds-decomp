#include "sdk_ps2.h"
#include "common.h"
#include "fr_font_measure.h"
#include "sdf_heap_api.h"
#include "itf.h"
#include "itf_mem_node.h"
#include "itf_resource_segments.h"
#include "sdf_texture_file.h"
#include "btl.h"

extern void frDelFontStyle(s32 slotId);
extern void frSetWhiteSpaceW(s32 slotId, s32 entryFlag);
#define ITF_NODE_HEADER_BYTES 8
#define ITF_ALLOCATION_HANDLE_BYTES 4
#define ITF_VECTOR_WORD_COUNT 4
#define ITF_RGBA_COMPONENT_COUNT 4
#define ITF_NEUTRAL_COLOR 0x80808080
extern u8 *datSkillMessages;
extern u8 *datItemMessages;
extern void itfSetTextDrawLimit(s32);
extern _FRQ *itfDrawBankTextWithSegmentCounter(s32, s32, s32, u16, FrFontTextBank *, s32);
extern sdfTexHandle_t *itfBackgroundSpriteTexture;
extern u8 frWrapEmptyString[];
extern _FRQ *frCombineFontRequest(_FRQ *, _FRQ *);
extern _FRQ *frFontLinkGlyph(_FRQ *, _FRQ *, s32);
extern void frEnableMode(s32 value);
extern u8 frDisableMode(u8 value);
void frFontEnsureSlotLoaded(s32 id, const char *path);
extern u16 D_003B2F58[];
extern void frDelFontStyle(s32 slotId);
extern _FRQ *frFontLinkGlyph(_FRQ *previous, _FRQ *next, s32 positionNext);
_FRQ *frFontBuildGlyphChain(const char *text, s8 fontIndex, s8 firstOption, s8 secondOption, _FRQ *head);
_FRQ *frCombineFontRequest(_FRQ *previous, _FRQ *next);
extern void frFontEnsureSlotLoaded(s32 slotId, const char *path);

_FRQ *frwGetStaffFontFRQ(u32 x, u32 y, s32 depth, s32 alt, s32 measureFlag, s32 entryFlag, u32 colors, const char *source) {
    _FRQ *glyph;
    s32 kind = 4;

    if (alt) {
        kind = 5;
    }
    frSetWhiteSpaceW(kind, entryFlag);
    frEnableMode(1);
    frDisableMode(2);
    frDisableMode(0x10);
    glyph = frFontAppendTextToGlyphChain(source, kind, 0, 0, 0);
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

/* DDS2 uses the larger 0xA10-entry glyph table; 0xFFFF is missing. */
u32 itfDecodeGlyph(u32 glyphCode) {
    s32 adjusted = (glyphCode & 0xffff) + 0xffff7f80;
    s32 index = ((adjusted & 0xff00) >> 1) + (adjusted & 0x7f);

    if ((u32)index < 0xA10) {
        return D_003B2F58[index];
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
void itfAttachGlyph16x18(u32 x, u32 y, s32 depth, u32 colors,
                                    void *glyphSource, _FRQ *parent) {
    _FRQ *glyph;

    glyph = frFontAppendTextToGlyphChain(glyphSource, 0, 0, 0, 0);
    frSetFontSize(glyph, 0x10, 0x12);
    frSetDrawPosition(glyph, x, y);
    frSetPriority(glyph, depth << 4);
    frSetFontColor(glyph, colors);
    frSetFontSpace(glyph, -4);
    frFontLinkGlyph(parent, glyph, 0);
}

_FRQ *frwGetFontEx(s32 x, s32 y, s32 depth, u32 colors, const char *text,
                         s8 glyphMode, s8 sharedFlag, _FRQ *previousGlyph) {
    char glyphData[3];
    s32 length;
    s32 position = 0;
    s32 glyphLength;
    _FRQ *chain = previousGlyph;

    length = strlen(text);
    if (sharedFlag == 0) {
        frEnableMode(1);
    }
    frDisableMode(2);

    if (length > 0) {
        do {
            const char *character = text + position;
            _FRQ *glyph;

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
            glyph = frFontAppendTextToGlyphChain(glyphData, glyphMode, 0, 0, 0);
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

_FRQ *frwGetSmallFontFRQ(x, y, depth, colors, text, parent)
    s32 x;
    s32 y;
    s32 depth;
    s32 colors;
    const char *text;
    _FRQ *parent;
{
    _FRQ *handle = frwGetFontEx(x, y, depth, colors, text, 1, 0, parent);
    frSetFontSpace(handle, 3);
    return handle;
}

/* Create positioned, colored text with measured spacing and link it to parent. */
_FRQ *itfCreateMeasuredTextGlyph(s32 x, s32 y, s32 depth, s32 colors,
                           const char *text, _FRQ *parent) {
    return frwGetSmallFontFRQ(x, y, depth, colors, text, parent);
}

/* Decode two-byte glyph codes before building and linking the text glyph. */
_FRQ *itfCreateConvertedTextGlyph(s32 x, s32 y, s32 depth, u32 colors, const u8 *text, _FRQ *parent) {
    u8 buffer[0x400];
    s32 i;
    s32 length = strlen((const char *)text);
    _FRQ *glyph;

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
    glyph = frFontAppendTextToGlyphChain(buffer, 1, 0, 0, 0);
    frEnableMode(2);
    frDisableMode(1);
    frSetDrawPosition(glyph, x, y);
    frSetPriority(glyph, depth << 4);
    frSetFontColor(glyph, colors);
    return frFontLinkGlyph(parent, glyph, 0);
}

_FRQ *itfCreateFont2TextGlyph(s32 x, s32 y, s32 depth, u32 colors, char *text, _FRQ *previousGlyph) {
    _FRQ *glyph;

    glyph = frFontAppendTextToGlyphChain((const char *)frWrapEmptyString, 0, 0, 0, previousGlyph);
    frDisableMode(2);
    glyph = frFontBuildGlyphChain(text, 2, 0, 0, glyph);
    frSetFontSpace(glyph, -1);
    frEnableMode(2);
    frSetDrawPosition(glyph, x, y);
    frSetPriority(glyph, depth << 4);
    frSetFontColor(glyph, colors);
    return glyph;
}

/* Build text with font index 3; retain the existing glyph chain's spacing. */
_FRQ *itfCreateFont3TextGlyph(s32 x, s32 y, s32 depth, u32 colors, const char *text, _FRQ *previousGlyph) {
    _FRQ *glyph;

    glyph = frFontAppendTextToGlyphChain((const char *)frWrapEmptyString, 0, 0, 0, previousGlyph);
    frDisableMode(2);
    glyph = frFontBuildGlyphChain(text, 3, 0, 0, glyph);
    frEnableMode(2);
    frSetDrawPosition(glyph, x, y);
    frSetPriority(glyph, depth << 4);
    frSetFontColor(glyph, colors);
    return glyph;
}

/* Build font-index-3 text with signed spacing -2 and return its glyph chain. */
_FRQ *itfCreateTightFont3TextGlyph(s32 x, s32 y, s32 depth, u32 colors, char *text, _FRQ *previousGlyph) {
    _FRQ *glyph;

    glyph = frFontAppendTextToGlyphChain((const char *)frWrapEmptyString, 0, 0, 0, previousGlyph);
    frDisableMode(2);
    glyph = frFontBuildGlyphChain(text, 3, 0, 0, glyph);
    frSetFontSpace(glyph, -2);
    frEnableMode(2);
    frSetDrawPosition(glyph, x, y);
    frSetPriority(glyph, depth << 4);
    frSetFontColor(glyph, colors);
    return glyph;
}

/* The sequel uses the negative flag variant on its alternate 12x16 glyph. */
void itfAttachGlyph12x16(u32 x, u32 y, s32 depth, u32 colors,
                                    void *glyphSource, _FRQ *parent) {
    _FRQ *glyph;

    glyph = frFontBuildGlyphChain(glyphSource, 0, 0, 0, 0);
    frSetFontSize(glyph, 0xc, 0x10);
    frSetFontSpace(glyph, -3);
    frSetDrawPosition(glyph, x, y);
    frSetPriority(glyph, depth << 4);
    frSetFontColor(glyph, colors);
    frFontLinkGlyph(parent, glyph, 0);
}

/* Count 0xFx0F separators in the packed text stream. */
s32 itfCountTextSeparators(const char *text) {
    s32 separatorCount = 0;

    while (*text != 0) {
        if ((*(u8 *)text & 0xF0) == 0xF0) {
            if (text[1] == 0xF) {
                separatorCount++;
            }
            text++;
        }
        text++;
    }
    return separatorCount;
}

/* Copy one plain-text segment; other two-byte control codes pass through. */
void itfCopyTextSegment(const char *src, char *dst, s32 segmentIndex) {
    s32 currentSegment = 0;

    while (*src != 0) {
        if ((*(u8 *)src & 0xF0) == 0xF0) {
            if (src[1] == 0xF) {
                currentSegment++;
            } else {
                dst[0] = *(u8 *)src;
                dst[1] = src[1];
                dst += 2;
            }
            src++;
        } else if (currentSegment == segmentIndex) {
            *dst = *(u8 *)src;
            dst++;
        }
        src++;
    }
    *dst = 0;
}

/* Cycling text cache follows the shared empty-string bytes. */
extern u16 D_00436582;
extern s32 D_00436584;
extern s32 D_00436588;
extern const u8 *MessageNumSearch(s32 textId, const FrFontTextBank *bank, s32 mode);
extern s32 frFontCountGlyphBands(_FRQ *text);
extern void frFontMoveChainTo(s32 x, s32 y, _FRQ *text);

/* Cycle a segment counter every 30 calls, but draw the full bank text stream.
 * Return NULL for absent text; optionally center the resulting glyph chain. */
_FRQ *itfDrawBankTextWithSegmentCounter(s32 x, s32 y, s32 depth, u16 textId, FrFontTextBank *bank, s32 flags) {
    char buffer[0x80];
    const u8 *text;
    u32 mode;
    _FRQ *handle;
    s32 separators;

    if (D_00436582 != textId) {
        D_00436582 = textId;
        D_00436584 = 0;
        D_00436588 = 0;
    }
    text = MessageNumSearch(textId, bank, 0);
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
    separators = itfCountTextSeparators((const char *)text);
    if (++D_00436588 == 30) {
        D_00436588 = 0;
        if (++D_00436584 > separators) {
            D_00436584 = 0;
        }
    }
    itfCopyTextSegment((const char *)text, buffer, D_00436584);
    /* The original draw uses the full stream rather than the copied segment. */
    handle = itfDrawPlainEncodedTextWithByteColors(x, y, depth, 1, 0, 0, 0x80, text, 0);
    if (flags & 0x10000) {
        s32 maxWidth = 0;
        s32 i;
        s32 width;
        for (i = 0; i < frFontCountGlyphBands(handle); i++) {
            width = frFontMeasureLineWidth(i, handle);
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

_FRQ *itfDrawBankTextWithLayoutFlags(s32 x, s32 y, s32 depth, u16 textId, FrFontTextBank *bank, s32 flags) {
    const u8 *text = MessageNumSearch(textId, bank, 0);
    u32 mode;
    _FRQ *handle;

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
    handle = itfDrawPlainEncodedTextWithByteColors(x, y, depth, 1, 0, 0, 0x80, text, 0);
    if (flags & 0x10000) {
        s32 maxWidth = 0;
        s32 i;
        s32 width;
        for (i = 0; i < frFontCountGlyphBands(handle); i++) {
            width = frFontMeasureLineWidth(i, handle);
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

_FRQ *itfDrawTextWithSelectedFontMode(s32 x, s32 y, s32 depth, s8 fontMode, u16 textId, s32 flags) {
    _FRQ *result = NULL;

    itfSetTextDrawLimit(0x13);
    switch (fontMode) {
    case 0:
        result = itfDrawBankTextWithSegmentCounter(x, y, depth, textId, (FrFontTextBank *)datSkillMessages, flags);
        break;
    case 1:
        result = itfDrawBankTextWithSegmentCounter(x, y, depth, textId, (FrFontTextBank *)datItemMessages, flags);
        break;
    case 2:
        result = itfDrawBankTextWithSegmentCounter(x, y, depth, textId, (FrFontTextBank *)datSkillMessages, flags);
        break;
    }
    itfSetTextDrawLimit(-1);
    return result;
}

_FRQ *itfDrawUnderscoreTextSegment(x, y, depth, color, text, segmentIndex)
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

void func_001A0040(void) {
}

void func_001A0048(void) {
}

s32 itfReturnThirdCallbackValue(s32 unused0, s32 unused1, s32 value) {
    return value;
}

extern s32 D_00438F20;
/* Return the byte count saved by the most recent packed-resource decode. */
s32 itfGetLastDecodedByteCount(void) {
    return D_00438F20;
}

u32 frGetFontHeader(u32 resourceBase) {
    return resourceBase;
}

/* Resolve three signed, cumulative relative offsets without validating bounds. */
void frGetFontImagePtrs(ItfRelativeSegmentResourceView *block, ItfPackedResourceSegments *segments) {
    s32 firstOffset = block->firstOffset;
    s32 secondOffset = firstOffset + block->secondDelta;
    s32 thirdOffset = secondOffset + block->thirdDelta;

    segments->treeNodes = (u8 *)block + firstOffset;
    segments->entryBitRanges = (ItfBitRange *)((u8 *)block + secondOffset);
    segments->packedBitstream = (u16 *)((u8 *)block + thirdOffset);
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
    D_00438F20 = produced;
    return produced;
}

/* Build count usable nodes plus index-zero sentinel, retaining each payload gap.
 * The allocation handle is stored four bytes before the returned ring base. */
MemNode *frAllocFontTable(s32 payloadBytes, s32 count) {
    sdfMemHandle_t *handle = sdfMemAllocDirect((payloadBytes + ITF_NODE_HEADER_BYTES) * (count + 1) + ITF_ALLOCATION_HANDLE_BYTES);
    u8 *list = (u8 *)sdfMemLock(handle);
    MemNode *node;
    MemNode *next;
    s32 i = 0;
    memcpy(list, &handle, ITF_ALLOCATION_HANDLE_BYTES);
    list += ITF_ALLOCATION_HANDLE_BYTES;
    node = (MemNode *)list;
    if (count > 0) {
        do {
            node->slotIndex = i;
            i++;
            next = (MemNode *)((u8 *)node + payloadBytes + ITF_NODE_HEADER_BYTES);
            node->next = next;
            node = next;
        } while (i < count);
    }
    node->slotIndex = count;
    node->next = (MemNode *)list;
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
extern void itfDrawTexturedSpriteRect(s32 x, s32 y, u32 depth, s32 width, s32 height,
                                      const struct EffSpriteUV *uvRect,
                                      const union EffSpriteColor *color, u32 flipFlags,
                                      u32 blendKind, s32 mode, struct sdfTexHandle_t *texture,
                                      s32 surfaceId);

/* Draw only when a texture is present; native X/Y extent scaling differs. */
void itfDrawBackgroundSprite(void) {
    s32 origin[ITF_VECTOR_WORD_COUNT];
    s32 color[ITF_RGBA_COMPONENT_COUNT];
    s16 width;
    s16 height;
    sdfTexHandle_t *panel = itfBackgroundSpriteTexture;
    if (panel != NULL) {
        width = panel->width;
        height = panel->height;
        origin[0] = 0;
        origin[1] = 0;
        origin[2] = width;
        origin[3] = height;
        color[0] = ITF_NEUTRAL_COLOR;
        color[1] = ITF_NEUTRAL_COLOR;
        color[2] = ITF_NEUTRAL_COLOR;
        color[3] = ITF_NEUTRAL_COLOR;
        itfDrawTexturedSpriteRect(0, 0, 0, width * 0x10, height * 8,
                                  (const struct EffSpriteUV *)origin,
                                  (const union EffSpriteColor *)color, 0, 0, 1,
                                  itfBackgroundSpriteTexture, 0x52);
    }
}

INCLUDE_SDATA(const s32, "interface/frWrap", frWrapEmptyString);

INCLUDE_SDATA(const s32, "interface/frWrap", D_00436582);

INCLUDE_SDATA(const s32, "interface/frWrap", D_00436584);

INCLUDE_SDATA(const s32, "interface/frWrap", D_00436588);

