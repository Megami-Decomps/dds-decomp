#include "common.h"
#include "itf.h"
#include "sdf.h"
#include "itf_panel_draw.h"

typedef struct SdfDrawPacket SdfDrawPacket;

#define ITF_BYTE_MASK 0xFF
#define ITF_NODE_HEADER_BYTES 8
#define ITF_ALLOCATION_HANDLE_BYTES 4
#define ITF_VECTOR_WORD_COUNT 4
#define ITF_RGBA_COMPONENT_COUNT 4
#define ITF_FIXED_FRACTION_BITS 16
#define ITF_FIXED_COMPONENT_MAX 0xff0000
#define ITF_NEUTRAL_COLOR 0x80808080
#define ITF_GS_X_BIAS 0x7000
#define ITF_GS_Y_BIAS 0x7900
#define ITF_TRIANGLE_VERTEX_COUNT 3
#define ITF_QUAD_VERTEX_COUNT 4
#define ITF_STATE_PACKET_BYTES 0x30
#define ITF_TEST_FIRST_CONTEXT 0x47
#define ITF_TEST_SECOND_CONTEXT 0x48
#define ITF_ALPHA_FIRST_CONTEXT 0x42
#define ITF_ALPHA_SECOND_CONTEXT 0x43
#define ITF_TRIANGLE_PRIMITIVE_BITS 0x4B
#define ITF_QUAD_FAN_PRIMITIVE_BITS 0x4D
#define ITF_TEXTURED_FAN_PRIMITIVE_BITS 0x5D
#define ITF_TEXTURED_SPRITE_PRIMITIVE_BITS 0x156
#define ITF_COLORED_SPRITE_PRIMITIVE_BITS 0x15E
#define ITF_SPRITE_PRIMITIVE_BITS 0x46
#define ITF_TRIANGLE_STRIP_PRIMITIVE_BITS 0x4C
#define ITF_LINE_STRIP_PRIMITIVE_BITS 0x14A
#define ITF_LINES_PRIMITIVE_BITS 0x49


extern SdfMemBlock *sdfAllocGeneralBlock(s32);
extern u32 sdfResourceRetainAddress(SdfMemBlock *);
extern void sdfReleaseResourceAllocation(SdfMemBlock *);
extern void sdfTexReleaseReference(SdfTex *);

extern SdfTex *sdfTexAcquireResourceTexture(void *);
extern s32 dds3AdminGetActiveMode(void);
extern void dds3AdminSetControlFlag(void);
extern void func_003003F0(const char *);
extern SdfMemBlock *sdfReadNamedResource(const char *, u32 *, u32 *);

extern u32 itfFontTestScriptTask;
extern s64 kwlnTaskGetRegisteredState(u32);

extern u32 D_003BB18C;

extern SdfTex *itfBackgroundSpriteTexture;

extern u8 D_003BB188[];
extern FrFontGlyph *func_001951C8(void *, s8, s8, s8, FrFontGlyph *);
extern void frFontCheckPendingGlyphState(FrFontCtx *);
extern void frFontAdvanceContextCursor(FrFontCtx *);
extern void func_00196220(u8, FrFontCtx *);
extern u8 frFontSharedGlyphFlags;
extern void mnuSetTitleVoicePrefixIndex(s32);
extern void mnuPlayTitleVoiceFile(char *);
extern s32 mnuGetTitleEffectFrameCounter(void);

extern FrFontGlyph *frFontLinkGlyphAfterPrevious(FrFontGlyph *, FrFontGlyph *);
extern FrFontGlyph *frFontLinkGlyph(FrFontGlyph *, FrFontGlyph *, s32);

extern void frFontSetEntryFlag(s32 kind, u64 flag);

extern void frFontAddSharedGlyphFlags(s32 value);

extern u8 frFontClearFlagBits(u8 value);

extern u8 D_00357980[];
extern u8 D_00357988[];
extern void frFontSetChainFlag(FrFontGlyph *glyph, u8 value);

extern s32 frFontDefaultGlyphCellSize;

extern u32 D_003BB170;

extern u32 D_003BB16C;
extern u32 D_003BD818;
void frFontEnsureSlotLoaded(s32 id, const char *path);
extern u16 itfGlyphDecodeTable[];
extern u32 strlen(const char *str);

/* 8-byte node header; payload follows (itfDequeueMemNode/itfEnqueueMemNode). */
typedef struct MemNode {
    u32 slotIndex;         /* 0x0: zero identifies the ring's sentinel */
    struct MemNode *next;  /* 0x4 */
} MemNode;

/* Allocation handle precedes the first queue node by four bytes. */
typedef struct MemRingHeader {
    SdfMemBlock *allocation;
    MemNode first;
} MemRingHeader;

/* Field block split by itfSplitRelativeSegments. */
typedef struct MemBlock {
    s32 firstOffset; /* 0x0 */
    s32 secondDelta; /* 0x4 */
    u8 pad08[0x10]; /* 0x8 */
    s32 thirdDelta; /* 0x18 */
} MemBlock;

typedef struct MemOut {
    void *first; /* 0x0 */
    void *second; /* 0x4 */
    void *third; /* 0x8 */
} MemOut;

/* Two consecutive entries in the four-byte-stride offset table bound a span. */
typedef struct ItfBitRange {
    u32 start;
    u32 end;
} ItfBitRange;


typedef struct TextPoolNode {
    struct TextPoolNode *previous;
    struct TextPoolNode *next;
    s32 index;
} TextPoolNode;

typedef struct TextPool {
    TextPoolNode *activeHead;
    TextPoolNode *activeTail;
    TextPoolNode *freeHead;
    TextPoolNode *freeTail;
} TextPool;


typedef struct TextVector {
    s32 x;
    s32 y;
    s32 z;
    s32 w;
} TextVector;

extern s32 func_00197068(FrFontCtx *args);

/* Subtract one modulo 256 from the first byte, then advance over the whole pair. */
u32 itfReadEncodedTextLead(FrFontCtx *args) {
    s32 *position = &args->offset;
    u8 *byte = args->bytes + *position;
    u32 value = *byte;

    *position += 2;
    return (value + ITF_BYTE_MASK) & ITF_BYTE_MASK;
}

/* Decode a little-endian pair; encoded high byte 0xFF is the zero escape.
 * Neither reader checks the input length. */
u32 itfReadEncodedCode(FrFontCtx *args) {
    u8 *bytes = args->bytes;
    u32 first;
    u32 second;

    first = (bytes[args->offset++] + ITF_BYTE_MASK) & ITF_BYTE_MASK;
    second = bytes[args->offset++];
    if (second == ITF_BYTE_MASK) {
        second = 0;
    } else {
        second = (second + ITF_BYTE_MASK) & ITF_BYTE_MASK;
    }
    return (second << 8) | first;
}

extern u32 D_003BB15C;
extern u32 D_003D6E20[];
extern FrFontSystem frFontWork;
extern s32 D_003BAA98;
extern s32 D_003BAA9C;

s32 func_001964F8(s32 code, FrFontCtx *stream) {
    s32 *position = &stream->offset;
    u8 *bytes = stream->bytes;
    s32 payloadWords = code & 0xF;
    s32 payloadPosition = *position;

    code = (code << 8) | bytes[payloadPosition++];
    *position = payloadPosition;
    switch (code) {
    case 0xF206:
        stream->channel0 = bytes[payloadPosition] - 1;
        *position = payloadPosition + 2;
        break;
    case 0xF202:
        stream->channel1 = bytes[payloadPosition] - 1;
        *position = payloadPosition + 2;
        break;
    case 0xF209:
        stream->channel2 = bytes[payloadPosition] - 1;
        *position = payloadPosition + 2;
        break;
    case 0xF207:
        stream->channel3 = bytes[payloadPosition] - 1;
        *position = payloadPosition + 2;
        break;
    case 0xF203:
        if (D_003D6E20[bytes[*position] - 1] != 0) {
            frFontCheckPendingGlyphState(stream);
            func_00196220((u8)(stream->bytes[*position] - 1), stream);
        }
        *position += 2;
        break;
    case 0xF20E:
        *position = payloadPosition + 1;
        break;
    case 0xF10F:
        if (!(frFontSharedGlyphFlags & 4)) {
            goto advanceLine;
        }
        break;
    case 0xF110:
        D_003BB15C |= 2;
        return 1;
    case 0xF111:
        if (!(frFontSharedGlyphFlags & 8)) {
            goto checkAutomaticLine;
        }
        /* This flagged form requests the same stop as opcode F104. */
    case 0xF104:
        D_003BB15C |= 1;
        return 1;

checkAutomaticLine:
        if (!(frFontSharedGlyphFlags & 0x20)) {
            break;
        }
advanceLine:
        frFontAdvanceContextCursor(stream);
        break;
    case 0xF112:
        D_003BB15C |= 4;
        break;
    case 0xF413:
        mnuSetTitleVoicePrefixIndex(itfReadEncodedTextLead(stream));
        mnuPlayTitleVoiceFile((char *)itfReadEncodedCode(stream));
        break;
    case 0xF214:
        if (stream->glyphChain->unk34 != 0) {
            stream->glyphChain->unk38 = 1;
        }
        stream->glyphChain->unk30 = code;
        stream->glyphChain->unk3C = itfReadEncodedCode(stream);
        break;
    case 0xF215:
        if (stream->glyphChain->unk34 != 0) {
            stream->glyphChain->unk38 = 1;
        }
        stream->glyphChain->unk30 = code;
        stream->glyphChain->unk3C = itfReadEncodedCode(stream);
        if (stream->glyphChain->unk3C != 0xFFFF) {
            s32 frame = mnuGetTitleEffectFrameCounter();
            stream->glyphChain->unk3C -= frame;
        }
        if (stream->glyphChain->unk3C < 0) {
            stream->glyphChain->unk3C = 0;
        }
        break;
    case 0xF416:
        D_003BB16C = itfReadEncodedTextLead(stream);
        D_003BB170 = itfReadEncodedTextLead(stream);
        D_003BB15C |= 8;
        break;
    case 0xF117:
        stream->glyphChain->unk34 = code;
        break;
    case 0xF20A:
    case 0xF20B:
    case 0xF20C:
    case 0xF20D:
        break;
    default:
        stream->offset += (payloadWords - 1) << 1;
        break;
    }
    if (stream->pendingCreate == 0) {
        stream->pendingCreate = 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00196478", func_001968C0);

FrFontGlyph *itfDrawDefaultColorText(s32 x, s32 y, u8 *encodedText, FrFontGlyph *sub) {
    return itfDrawEncodedTextStream(x, y, 0, 0, 0, 0, 0x80, encodedText, sub);
}

FrFontGlyph *itfDrawCustomColorText(s32 x, s32 y, s32 channel0, s32 channel1, s32 channel2, s32 channel3, u8 *encodedText, FrFontGlyph *sub) {
    return itfDrawEncodedTextStream(x, y, 0, channel0 & 0xFF, channel1 & 0xFF, channel2 & 0xFF, channel3 & 0xFF, encodedText, sub);
}

extern FrFontGlyph *func_001968C0(FrFontCtx *);

FrFontGlyph *itfDrawEncodedTextStream(s32 x, s32 y, s32 depth, s32 channel0, s32 channel1, s32 channel2, s32 channel3, u8 *encodedText, FrFontGlyph *sub) {
    FrFontCtx args;

    args.x = x;
    args.y = y;
    args.z = depth << 4;
    args.channel0 = channel0;
    args.channel1 = channel1;
    args.channel2 = channel2;
    args.channel3 = channel3;
    args.bytes = encodedText;
    args.glyphChain = sub;
    args.offset = 0;
    args.pendingCreate = 1;
    args.pendingPosition = 1;
    return func_001968C0(&args);
}

FrFontGlyph *itfDrawColor(s32 x, s32 y, s32 depth, s32 channel0, s32 channel1,
                   s32 channel2, s32 channel3, u8 *encodedText, FrFontGlyph *unusedSub) {
    return itfDrawEncodedTextStream(x, y, depth, channel0 & 0xff, channel1 & 0xff,
                  channel2 & 0xff, channel3 & 0xff, encodedText, 0);
}

u32 itfTestTextInterfaceMask(u32 mask) {
    return D_003BB15C & mask;
}

void func_00196BC0(s32 index, s32 value) {
    D_003D6E20[index] = value;
}

u32 func_00196BD8(void) {
    return D_003BB16C;
}

u32 func_00196BE0(void) {
    return D_003BB170;
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00196BE8);

INCLUDE_ASM(const s32, "game/code_00196478", func_00196ED0);

INCLUDE_ASM(const s32, "game/code_00196478", func_00197068);

s32 itfInitTextDrawArgs(u8 *encodedText, FrFontGlyph *sub) {
    FrFontCtx args;

    args.x = 0;
    args.y = 0;
    args.z = 0;
    args.channel0 = 0;
    args.channel1 = 0;
    args.channel2 = 0;
    args.channel3 = 0;
    args.bytes = encodedText;
    args.glyphChain = sub;
    args.offset = 0;
    args.pendingCreate = 1;
    args.pendingPosition = 1;
    return func_00197068(&args);
}

/* Return the selected resource header's cell width; index is unchecked. */
u16 frFontGetSlotCellWidth(s32 index) {
    return frFontWork.entries[index].resourceHeader->cellWidth;
}

/* Return the selected resource header's cell height; index is unchecked. */
u16 frFontGetSlotCellHeight(s32 index) {
    return frFontWork.entries[index].resourceHeader->cellHeight;
}

void itfSetTextDrawLimit(s32 limit) {
    if (limit < 1) {
        limit = 0x14;
    }
    frFontDefaultGlyphCellSize = limit;
}

FrFontGlyph *frFontBuildColoredGlyphWithSharedFlags(u32 x, u32 y, s32 depth, s32 alt, s32 measureFlag, u64 entryFlag, u32 colors, void *source) {
    FrFontGlyph *glyph;
    s32 kind = 4;

    if (alt) {
        kind = 5;
    }
    frFontSetEntryFlag(kind, entryFlag);
    frFontAddSharedGlyphFlags(1);
    frFontClearFlagBits(2);
    frFontClearFlagBits(0x10);
    glyph = frFontAppendGlyphFromData(source, kind, 0, 0, 0);
    frFontAddSharedGlyphFlags(0x10);
    frFontAddSharedGlyphFlags(2);
    frFontClearFlagBits(1);
    frFontSetFlagAndMeasureGlyphs(glyph, measureFlag);
    frFontSetContextPair(glyph, x, y);
    frFontStoreShiftedContextValue(glyph, depth << 4);
    frFontSetChildColors(glyph, colors);
    frFontSetChainFlag(glyph, 5);
    return glyph;
}

void mnuLoadStaffFonts(void) {
    frFontEnsureSlotLoaded(4, "/font/staff1.fnt");
    frFontEnsureSlotLoaded(5, "/font/staff2.fnt");
}

void mnuUnloadStaffFonts(void) {
    frFontFreeEntry(4);
    frFontFreeEntry(5);
}

/* Decode a two-byte glyph through the DDS1 lookup table; 0xFFFF is missing. */
u32 itfDecodeGlyph(u32 value) {
    s32 adjusted = (value & 0xffff) + 0xffff7f80;
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
void itfAttachGlyph16x18(u32 x, u32 y, s32 depth, u32 colors,
                                    void *glyphSource, FrFontGlyph *parent) {
    FrFontGlyph *glyph;

    glyph = frFontAppendGlyphFromData(glyphSource, 0, 0, 0, 0);
    frFontSetGlyphChainDimensions(glyph, 0x10, 0x12);
    frFontSetContextPair(glyph, x, y);
    frFontStoreShiftedContextValue(glyph, depth << 4);
    frFontSetChildColors(glyph, colors);
    frFontSetFlagAndMeasureGlyphs(glyph, -4);
    frFontLinkGlyph(parent, glyph, 0);
}

FrFontGlyph *itfAppendTextGlyphChain(s32 x, s32 y, s32 depth, u32 colors, const char *text,
                         s8 glyphMode, s8 sharedFlag, FrFontGlyph *previousGlyph) {
    char glyphData[3];
    s32 length;
    s32 position = 0;
    s32 glyphLength;
    FrFontGlyph *chain = previousGlyph;

    length = strlen(text);
    if (sharedFlag == 0) {
        frFontAddSharedGlyphFlags(1);
    }
    frFontClearFlagBits(2);

    if (length > 0) {
        do {
            const char *character = text + position;
            FrFontGlyph *glyph;

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
            glyph = frFontAppendGlyphFromData(glyphData, glyphMode, 0, 0, 0);
            frFontStoreShiftedContextValue(glyph, depth << 4);
            frFontSetChildColors(glyph, colors);
            frFontSetFlagAndMeasureGlyphs(glyph, 3);
            chain = frFontLinkGlyphAfterPrevious(chain, glyph);
            if (position == 0) {
                glyph->x = x;
            }
            position += glyphLength;
            glyph->y = y;
        } while (position < length);
    }

    frFontAddSharedGlyphFlags(2);
    if (sharedFlag == 0) {
        frFontClearFlagBits(1);
    }
    return chain;
}

FrFontGlyph *frFontCreateMeasuredFlaggedGlyph(x, y, depth, colors, text, parent)
s32 x;
s32 y;
s32 depth;
s32 colors;
const char *text;
FrFontGlyph *parent;
{
    FrFontGlyph *handle = itfAppendTextGlyphChain(x, y, depth, colors, text, 1, 0, parent);

    frFontSetFlagAndMeasureGlyphs(handle, 3);
    return handle;
}

void func_00197748(void) {
    frFontCreateMeasuredFlaggedGlyph();
}

/* Decode two-byte glyph codes before building and linking the text glyph. */
FrFontGlyph *itfCreateConvertedTextGlyph(s32 x, s32 y, s32 depth, u32 colors, const u8 *text, FrFontGlyph *parent) {
    u8 buffer[0x400];
    s32 i;
    s32 length = strlen((const char *)text);
    FrFontGlyph *glyph;

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
    frFontAddSharedGlyphFlags(1);
    glyph = frFontAppendGlyphFromData(buffer, 1, 0, 0, 0);
    frFontAddSharedGlyphFlags(2);
    frFontClearFlagBits(1);
    frFontSetContextPair(glyph, x, y);
    frFontStoreShiftedContextValue(glyph, depth << 4);
    frFontSetChildColors(glyph, colors);
    return frFontLinkGlyph(parent, glyph, 0);
}

FrFontGlyph *func_001978E8(s32 x, s32 y, s32 depth, u32 colors, char *text, FrFontGlyph *previousGlyph) {
    FrFontGlyph *glyph;

    glyph = frFontAppendGlyphFromData(D_003BB188, 0, 0, 0, previousGlyph);
    frFontClearFlagBits(2);
    glyph = func_001951C8(text, 2, 0, 0, glyph);
    frFontSetFlagAndMeasureGlyphs(glyph, -1);
    frFontAddSharedGlyphFlags(2);
    frFontSetContextPair(glyph, x, y);
    frFontStoreShiftedContextValue(glyph, depth << 4);
    frFontSetChildColors(glyph, colors);
    return glyph;
}

FrFontGlyph *func_001979C8(s32 x, s32 y, s32 depth, s32 colors, char *text, FrFontGlyph *previousGlyph) {
    FrFontGlyph *glyph;

    glyph = frFontAppendGlyphFromData(D_003BB188, 0, 0, 0, previousGlyph);
    frFontClearFlagBits(2);
    glyph = func_001951C8(text, 3, 0, 0, glyph);
    frFontAddSharedGlyphFlags(2);
    frFontSetContextPair(glyph, x, y);
    frFontStoreShiftedContextValue(glyph, depth << 4);
    frFontSetChildColors(glyph, colors);
    return glyph;
}

FrFontGlyph *func_00197A98(s32 x, s32 y, s32 depth, s32 colors, char *text, FrFontGlyph *previousGlyph) {
    FrFontGlyph *glyph;

    glyph = frFontAppendGlyphFromData(D_003BB188, 0, 0, 0, previousGlyph);
    frFontClearFlagBits(2);
    glyph = func_001951C8(text, 3, 0, 0, glyph);
    frFontSetFlagAndMeasureGlyphs(glyph, -2);
    frFontAddSharedGlyphFlags(2);
    frFontSetContextPair(glyph, x, y);
    frFontStoreShiftedContextValue(glyph, depth << 4);
    frFontSetChildColors(glyph, colors);
    return glyph;
}

/* Attach the alternate font glyph, with a 12x16 cell, to its parent. */
void itfAttachGlyph12x16(u32 x, u32 y, s32 depth, u32 colors,
                                    void *glyphSource, FrFontGlyph *parent) {
    FrFontGlyph *glyph;

    glyph = func_001951C8(glyphSource, 0, 0, 0, 0);
    frFontSetGlyphChainDimensions(glyph, 0xc, 0x10);
    frFontSetFlagAndMeasureGlyphs(glyph, 3);
    frFontSetContextPair(glyph, x, y);
    frFontStoreShiftedContextValue(glyph, depth << 4);
    frFontSetChildColors(glyph, colors);
    frFontLinkGlyph(parent, glyph, 0);
}

extern u8 *func_001961B0(s32 textId, FrFontTextBank *bank, s32 mode);
extern s32 func_00195E60(FrFontGlyph *text);
extern s32 func_00195ED8(s32 line, FrFontGlyph *text);
extern void frFontMoveChainTo(s32 x, s32 y, FrFontGlyph *text);

FrFontGlyph *itfDrawBankTextWithLayoutFlags(s32 x, s32 y, s32 depth, u16 textId, FrFontTextBank *bank, s32 flags) {
    u8 *text = func_001961B0(textId, bank, 0);
    u32 mode;
    FrFontGlyph *handle;

    if (text == 0) {
        return 0;
    }
    mode = (u16)flags;
    switch (mode) {
    case 1:
        frFontClearFlagBits(4);
        break;
    case 2:
        frFontAddSharedGlyphFlags(8);
        break;
    case 4:
        frFontAddSharedGlyphFlags(0x20);
        break;
    }
    handle = itfDrawColor(x, y, depth, 1, 0, 0, 0x80, text, 0);
    if (flags & 0x10000) {
        s32 maxWidth = 0;
        s32 i;
        s32 width;
        for (i = 0; i < func_00195E60(handle); i++) {
            width = func_00195ED8(i, handle);
            if (maxWidth < width) {
                maxWidth = width;
            }
        }
        frFontMoveChainTo(x - maxWidth / 2, y, handle);
    }
    switch (mode) {
    case 1:
        frFontAddSharedGlyphFlags(4);
        break;
    case 2:
        frFontClearFlagBits(8);
        break;
    case 4:
        frFontClearFlagBits(0x20);
        break;
    }
    return handle;
}

FrFontGlyph *itfDrawTextWithSelectedFontMode(s32 x, s32 y, s32 depth, s8 fontMode, u16 textId, s32 flags) {
    FrFontGlyph *result = NULL;

    itfSetTextDrawLimit(0x13);
    switch (fontMode) {
    case 0:
        result = itfDrawBankTextWithLayoutFlags(x, y, depth, textId, (FrFontTextBank *)D_003BAA98, flags);
        break;
    case 1:
        result = itfDrawBankTextWithLayoutFlags(x, y, depth, textId, (FrFontTextBank *)D_003BAA9C, flags);
        break;
    }
    itfSetTextDrawLimit(-1);
    return result;
}


FrFontGlyph *itfDrawUnderscoreTextSegment(x, y, depth, color, text, segmentIndex)
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

u32 func_00198008(void) {
    return 0;
}

void func_00198010(void) {
}

void func_00198018(void) {
}

u32 func_00198020(u32 unused0, u32 unused1, u32 value) {
    return value;
}

u32 func_00198028(void) {
    return D_003BD818;
}

u32 func_00198030(u32 value) {
    return value;
}

/* Resolve three signed, cumulative relative offsets without validating bounds. */
void itfSplitRelativeSegments(MemBlock *block, MemOut *out) {
    s32 firstOffset = block->firstOffset;
    s32 secondOffset = firstOffset + block->secondDelta;
    s32 thirdOffset = secondOffset + block->thirdDelta;

    out->first = (u8 *)block + firstOffset;
    out->second = (u8 *)block + secondOffset;
    out->third = (u8 *)block + thirdOffset;
}

u32 func_00198068(u32 object) {
    return *(u32 *)(func_00198030(object) + 0x10);
}

/* Decode the option's selected bit span through the resource's six-byte
 * lookup records, appending the byte carried by each terminal record. */
s32 func_00198088(u8 *dst, s32 option, u32 block, MemOut *segments) {
    ItfBitRange range;
    s32 produced;
    u8 *table = segments->first;
    u16 *bitWords;
    u32 wordIndex;
    u32 start;
    s32 remaining;
    u32 state;
    u16 *root;
    u16 *node;

    memcpy(&range.start, (u8 *)segments->second + option * 4, 4);
    memcpy(&range.end, (u8 *)segments->second + option * 4 + 4, 4);
    produced = 0;
    func_00198030(block);
    start = range.start;
    bitWords = segments->third;
    wordIndex = start >> 4;
    remaining = range.end - start;
    state = (bitWords[wordIndex] | 0x10000) >> (start & 0xF);
    root = (u16 *)(table + 2);
    node = root;
    do {
        u16 index = node[state & 1];

        node = (u16 *)(table + index * 6 + 2);
        if (*node == 0) {
            dst[produced++] = ((u8 *)node)[2];
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
u32 itfCreateMemNodeRing(s32 payloadBytes, s32 count) {
    SdfMemBlock *buffer;
    u8 *list;
    MemNode *cursor;
    MemNode *next;
    s32 i;

    buffer = sdfAllocGeneralBlock((payloadBytes + ITF_NODE_HEADER_BYTES) * (count + 1) + ITF_ALLOCATION_HANDLE_BYTES);
    list = (u8 *)sdfResourceRetainAddress(buffer);
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
    return (u32)list;
}

/* Remove the next free node, or return NULL at the index-zero sentinel. */
void *itfDequeueMemNode(MemNode *queue) {
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
s32 itfEnqueueMemNode(void *payload, MemNode *queue) {
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
u32 itfReleaseMemNodeBuffer(u8 *ringBase) {
    sdfReleaseResourceAllocation(((MemRingHeader *)(ringBase - ITF_ALLOCATION_HANDLE_BYTES))->allocation);
    return 1;
}

/* Acquire the background texture, then release the temporary file allocation. */
void itfLoadBackgroundSprite(void) {
    u32 resource;
    SdfMemBlock *fileAllocation = sdfReadNamedResource("/sprite/bg00.tmx", &resource, 0);

    itfBackgroundSpriteTexture = sdfTexAcquireResourceTexture((void *)resource);
    sdfReleaseResourceAllocation(fileAllocation);
}

/* Drop the held texture reference without clearing the global. */
void itfReleaseBackgroundSpriteTexture(void) {
    sdfTexReleaseReference(itfBackgroundSpriteTexture);
}


/* Draw only when a texture is present; native X/Y extent scaling differs. */
void itfDrawBackgroundSprite(void) {
    s32 origin[ITF_VECTOR_WORD_COUNT];
    s32 color[ITF_RGBA_COMPONENT_COUNT];
    SdfTex *panel = itfBackgroundSpriteTexture;

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

extern s32 kwlnTaskCreate(s32, s32, s32, s32, s32, s32, s32);
extern s32 scrCreateProcessTaskFromResource(s32, char *, s32);
extern u32 itfDrawBackgroundAndGetTaskReadyMask(void);
extern void itfReleaseFontTestTaskResources(void);

/* Start the background draw task and host-file font-test process. */
void itfStartFontTestScene(void) {
    itfLoadBackgroundSprite();
    kwlnTaskCreate((s32)"test_font", 0x2B06, 0, 0, (s32)itfDrawBackgroundAndGetTaskReadyMask, (s32)itfReleaseFontTestTaskResources, 0);
    itfFontTestScriptTask = scrCreateProcessTaskFromResource(0x258, "host0:../../../dds3data/font/test.bf", 0);
}

/* Advance/retain the test glyph and release its background texture reference. */
void itfReleaseFontTestTaskResources(void) {
    frFontAdvanceOrRetainFadingGlyph(D_003BB18C);
    itfReleaseBackgroundSpriteTexture();
}

/* Draw first, then report all bits set only for registered task state three. */
u32 itfDrawBackgroundAndGetTaskReadyMask(void) {
    s64 taskState;
    u32 readyMask;

    itfDrawBackgroundSprite();
    taskState = kwlnTaskGetRegisteredState(itfFontTestScriptTask);
    readyMask = 0xffffffff;
    if (taskState != 3) {
        readyMask = 0;
    }
    return readyMask;
}

/* Initialize a FIFO free list and empty active list.
 * Native do/while writes at least two nodes; the s8 index can wrap. */
void itfInitPool(TextPool *pool, TextPoolNode *nodes, s32 count, s32 stride) {
    s8 index = 0;
    TextPoolNode *previous = NULL;
    TextPoolNode *node = nodes;
    TextPoolNode *next;

    do {
        count--;
        next = (TextPoolNode *)((u8 *)node + stride);
        node->previous = previous;
        node->index = index;
        index++;
        node->next = next;
        previous = node;
        node = next;
    } while (count >= 2);
    node->previous = previous;
    node->index = index;
    node->next = NULL;
    pool->freeTail = node;
    pool->freeHead = nodes;
    pool->activeTail = NULL;
    pool->activeHead = NULL;
}

/* Move the free-list head to the active-list tail; exhausted pools return NULL. */
TextPoolNode *itfAcquirePoolNode(TextPool *pool) {
    TextPoolNode *node = pool->freeHead;
    TextPoolNode *next;

    if (node == NULL) {
        return NULL;
    }
    next = node->next;
    if (pool->activeHead != NULL) {
        node->previous = pool->activeTail;
        pool->activeTail->next = node;
    } else {
        node->previous = NULL;
        pool->activeHead = node;
    }
    node->next = NULL;
    pool->activeTail = node;
    if (next != NULL) {
        next->previous = NULL;
    } else {
        pool->freeTail = NULL;
    }
    pool->freeHead = next;
    return node;
}

/* Unlink an active node and append it to the free-list tail; membership is trusted. */
void itfReleasePoolNode(TextPoolNode *node, TextPool *pool) {
    TextPoolNode *previous = node->previous;
    TextPoolNode *next = node->next;

    if (previous != NULL) {
        previous->next = next;
    } else {
        pool->activeHead = next;
    }
    if (next != NULL) {
        next->previous = previous;
    } else {
        pool->activeTail = previous;
    }
    node->next = NULL;
    {
        TextPoolNode *freeTail = pool->freeTail;
        node->previous = freeTail;
        if (freeTail != NULL) {
            freeTail->next = node;
        }
    }
    pool->freeTail = node;
    if (pool->freeHead == NULL) {
        pool->freeHead = node;
    }
}

/* Multiply XYZ, cap only the positive product at 255 << 16, then shift by 16.
 * Negative products are not clamped; W comes from the argument. */
void itfScaleVectors(TextVector *output, s32 scaleX, s32 scaleY, s32 scaleZ,
                   s32 w, const TextVector *input, s32 count) {
    while (count > 0) {
        s32 x = scaleX * input->x;
        s32 y = scaleY * input->y;
        s32 z = scaleZ * input->z;

        output->w = w;
        if (x > ITF_FIXED_COMPONENT_MAX) x = ITF_FIXED_COMPONENT_MAX;
        if (y > ITF_FIXED_COMPONENT_MAX) y = ITF_FIXED_COMPONENT_MAX;
        if (z > ITF_FIXED_COMPONENT_MAX) z = ITF_FIXED_COMPONENT_MAX;
        output->x = x >> ITF_FIXED_FRACTION_BITS;
        output->y = y >> ITF_FIXED_FRACTION_BITS;
        output->z = z >> ITF_FIXED_FRACTION_BITS;
        output++;
        input++;
        count--;
    }
}

/* Replace each child color; parent color words are not modified. */
void itfSetStyleColor(FrFontGlyph *entry, u32 color) {
    for (; entry != NULL; entry = entry->previous) {
        FrFontGlyph *child;
        for (child = entry->link1C.firstChild; child != NULL; child = child->next) {
            child->u10.word = color;
        }
    }
}

/* Clear each child's low byte, then OR unmasked bits into the full color word. */
void itfSetStyleColorBits(FrFontGlyph *entry, u32 colorBits) {
    for (; entry != NULL; entry = entry->previous) {
        FrFontGlyph *child;
        for (child = entry->link1C.firstChild; child != NULL; child = child->next) {
            child->u10.word = (child->u10.word & ~ITF_BYTE_MASK) | colorBits;
        }
    }
}

/* Translate parent entries only; child coordinates are left unchanged. */
void itfTranslateStyleEntries(FrFontGlyph *entry, u32 xOffset, u32 yOffset) {
    for (; entry != NULL; entry = entry->previous) {
        entry->x += xOffset;
        entry->y += yOffset;
    }
}

extern s32 sdfAllocPacketAligned(s32);
extern s32 sdfConsCalculateDrawPacketSize(s32, s32);
extern void *sdfConsInitPacketHeader(SdfDrawPacket *, s32, s32, s64, s32);
extern s32 sdfConsMeasurePacketWithHeader(s32);
extern void sdfAppendPacket(SdfListHead *, u32);
extern s32 sdfConsCreateDrawPacket(SdfListHead *, SdfTex *, s32);
extern u32 sdfConsFinalizePacketHeader(u32, s32);
extern u64 D_00357998[];

/* Keep the sprite texture handle while releasing the temporary file allocation. */
SdfTex *itfLoadTextureFromAsset(const char *path) {
    SdfMemBlock *fileAllocation;
    SdfTex *textureHandle;
    u32 assetInfo[ITF_VECTOR_WORD_COUNT];

    fileAllocation = sdfReadNamedResource(path, assetInfo, 0);
    textureHandle = sdfTexAcquireResourceTexture((void *)assetInfo[0]);
    sdfReleaseResourceAllocation(fileAllocation);
    return textureHandle;
}

/* Emit three per-vertex RGBA/XYZ2 records; PRIM's low bits select a triangle.
 * Coordinates receive the native GS screen biases; no clipping is performed. */
void itfDrawTriFlat3(DrawVertex *vertices, DrawColorRec *colors, s32 xOffset, s32 yOffset, u32 tail, SdfListHead *command) {
    s32 packet;
    u64 *dst;
    s32 i;

    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, ITF_TRIANGLE_VERTEX_COUNT));
    sdfConsInitPacketHeader((SdfDrawPacket *)packet, ITF_TRIANGLE_PRIMITIVE_BITS, 2, 0x51, ITF_TRIANGLE_VERTEX_COUNT);
    dst = (u64 *)sdfConsMeasurePacketWithHeader(packet);
    for (i = 0; i < ITF_TRIANGLE_VERTEX_COUNT; i++) {
        dst[0] = (u64)colors->components[0] | ((u64)colors->components[1] << 32);
        dst[1] = (u64)colors->components[2] | ((u64)colors->components[3] << 32);
        colors++;
        dst += 2;
        dst[1] = (u64)tail;
        dst[0] = (u64)(u32)(vertices->x + xOffset + ITF_GS_X_BIAS) | ((u64)(vertices->y + yOffset + ITF_GS_Y_BIAS) << 32);
        vertices++;
        dst += 2;
    }
    sdfAppendPacket(command, packet);
}

/* Emit an indexed four-vertex triangle fan with a color record per vertex. */
void itfDrawQuadFlat4(DrawVertex *vertices, DrawColorRec *colors, u8 *vertexIndex, u8 *colorIndex, u32 tail, SdfListHead *command) {
    s32 packet;
    u64 *dst;
    s32 i;

    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, ITF_QUAD_VERTEX_COUNT));
    sdfConsInitPacketHeader((SdfDrawPacket *)packet, ITF_QUAD_FAN_PRIMITIVE_BITS, 2, 0x51, ITF_QUAD_VERTEX_COUNT);
    dst = (u64 *)sdfConsMeasurePacketWithHeader(packet);
    for (i = 0; i < ITF_QUAD_VERTEX_COUNT; i++) {
        DrawVertex *vertex = &vertices[*vertexIndex++];
        DrawColorRec *color = &colors[*colorIndex++];

        dst[0] = (u64)color->components[0] | ((u64)color->components[1] << 32);
        dst[1] = (u64)color->components[2] | ((u64)color->components[3] << 32);
        dst += 2;
        dst[1] = (u64)tail;
        dst[0] = (u64)(u32)(vertex->x + ITF_GS_X_BIAS) | ((u64)(vertex->y + ITF_GS_Y_BIAS) << 32);
        dst += 2;
    }
    sdfAppendPacket(command, packet);
}

/* Draw three adjacent quads using zero-alpha/source-alpha edge color records.
 * Border widths and resulting signed middle width are not clamped. */
void func_00198990(DrawVertex *bounds, DrawColorRec *color, u32 tail, s32 borderWidth, SdfListHead *command) {
    DrawVertex vertices[ITF_QUAD_VERTEX_COUNT];
    DrawColorRec colors[2];
    s32 middleWidth;

    colors[0].components[0] = color->components[0];
    colors[0].components[1] = color->components[1];
    colors[0].components[2] = color->components[2];
    colors[0].components[3] = 0;
    colors[1].components[0] = color->components[0];
    colors[1].components[1] = color->components[1];
    colors[1].components[2] = color->components[2];
    colors[1].components[3] = color->components[3];

    middleWidth = bounds[1].x - bounds[0].x - borderWidth * 2;
    vertices[0].x = bounds[0].x;
    vertices[0].y = bounds[0].y;
    vertices[1].x = bounds[0].x + borderWidth;
    vertices[1].y = bounds[0].y;
    vertices[2].x = bounds[0].x + borderWidth;
    vertices[2].y = bounds[1].y;
    vertices[3].x = bounds[0].x;
    vertices[3].y = bounds[1].y;

    itfDrawQuadFlat4(vertices, colors, D_00357980, D_00357988, tail, command);
    vertices[0].x += borderWidth;
    vertices[1].x += middleWidth;
    vertices[2].x += middleWidth;
    vertices[3].x += borderWidth;
    itfDrawQuadFlat4(vertices, colors, D_00357980, D_00357988 + 4, tail, command);
    vertices[0].x += middleWidth;
    vertices[1].x += borderWidth;
    vertices[2].x += borderWidth;
    vertices[3].x += middleWidth;
    itfDrawQuadFlat4(vertices, colors, D_00357980, D_00357988 + 8, tail, command);
}

/* Emit a textured triangle fan: each four-float input supplies S/T/Q only.
 * The fourth float is skipped, and the context word is shifted without masking. */
void itfDrawQuadTextured4(DrawVertex *vertices, f32 *uvs, DrawColorRec *colors, u32 tail, s32 flag, SdfListHead *command) {
    s32 packet;
    u64 *dst;
    s32 i;

    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(3, ITF_QUAD_VERTEX_COUNT));
    sdfConsInitPacketHeader((SdfDrawPacket *)packet, (flag << 9) | ITF_TEXTURED_FAN_PRIMITIVE_BITS, 3, 0x512, ITF_QUAD_VERTEX_COUNT);
    dst = (u64 *)sdfConsMeasurePacketWithHeader(packet);
    for (i = 0; i < ITF_QUAD_VERTEX_COUNT; i++) {
        f32 *uvDst = (f32 *)dst;

        uvDst[0] = uvs[0];
        uvDst[1] = uvs[1];
        uvDst[2] = uvs[2];
        uvs += 4;
        dst += 2;
        dst[0] = (u64)colors->components[0] | ((u64)colors->components[1] << 32);
        dst[1] = (u64)colors->components[2] | ((u64)colors->components[3] << 32);
        colors++;
        dst += 2;
        dst[1] = (u64)tail;
        dst[0] = (u64)(u32)(vertices->x + ITF_GS_X_BIAS) | ((u64)(vertices->y + ITF_GS_Y_BIAS) << 32);
        vertices++;
        dst += 2;
    }
    sdfAppendPacket(command, packet);
}

/* Bind the texture and emit one two-corner sprite using two packed UV pairs.
 * Unwritten padding words in the native packet remain untouched. */
void itfQueueTextureBoundQuadPacket(void *vertexData, void *uvData, void *colorData, s32 tail, SdfTex *texture, s32 flag, SdfListHead *command) {
    DrawVertex *vertices = vertexData;
    DrawColorRec *uv = uvData;
    DrawColorRec *colors = colorData;
    s32 packet;
    u64 *dst;
    s32 x0, y0, x1, y1;

    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(5, 1));
    sdfConsInitPacketHeader((SdfDrawPacket *)packet, (flag << 9) | ITF_TEXTURED_SPRITE_PRIMITIVE_BITS, 5, 0x53531, 1);
    x0 = vertices[0].x + ITF_GS_X_BIAS;
    y0 = vertices[0].y + ITF_GS_Y_BIAS;
    x1 = vertices[1].x + ITF_GS_X_BIAS;
    y1 = vertices[1].y + ITF_GS_Y_BIAS;
    dst = (u64 *)sdfConsMeasurePacketWithHeader(packet);
    dst[0] = (u64)colors->components[0] | ((u64)colors->components[1] << 32);
    dst[1] = (u64)colors->components[2] | ((u64)colors->components[3] << 32);
    dst[2] = (u64)uv->components[0] | ((u64)uv->components[1] << 32);
    dst[4] = (u64)(u32)x0 | ((u64)y0 << 32);
    dst[5] = (u64)(u32)tail;
    dst[6] = (u64)uv->components[2] | ((u64)uv->components[3] << 32);
    dst[8] = (u64)(u32)x1 | ((u64)y1 << 32);
    dst[9] = (u64)(u32)tail;
    sdfConsCreateDrawPacket(command, texture, flag);
    sdfAppendPacket(command, packet);
}

/* Emit a colored textured sprite without binding a texture in this function. */
void itfQueueColoredTexturedQuadPacket(DrawVertex *vertices, DrawColorRec *uv, DrawColorRec *colors, u32 tail, s32 flag, SdfListHead *command) {
    s32 packet;
    u64 *dst;
    s32 x0, y0, x1, y1;

    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(5, 1));
    sdfConsInitPacketHeader((SdfDrawPacket *)packet, (flag << 9) | ITF_COLORED_SPRITE_PRIMITIVE_BITS, 5, 0x53531, 1);
    x0 = vertices[0].x + ITF_GS_X_BIAS;
    y0 = vertices[0].y + ITF_GS_Y_BIAS;
    x1 = vertices[1].x + ITF_GS_X_BIAS;
    y1 = vertices[1].y + ITF_GS_Y_BIAS;
    dst = (u64 *)sdfConsMeasurePacketWithHeader(packet);
    dst[0] = (u64)colors->components[0] | ((u64)colors->components[1] << 32);
    dst[1] = (u64)colors->components[2] | ((u64)colors->components[3] << 32);
    dst[2] = (u64)uv->components[0] | ((u64)uv->components[1] << 32);
    dst[4] = (u64)(u32)x0 | ((u64)y0 << 32);
    dst[5] = (u64)tail;
    dst[6] = (u64)uv->components[2] | ((u64)uv->components[3] << 32);
    dst[8] = (u64)(u32)x1 | ((u64)y1 << 32);
    dst[9] = (u64)tail;
    sdfAppendPacket(command, packet);
}

/* Despite the current name, PRIM selects a two-corner sprite, not a line. */
void itfEmitColoredLinePacket(DrawVertex *vertices, DrawColorRec *colors, u32 tail, s32 flag, SdfListHead *command) {
    s32 packet;
    u64 *dst;

    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(3, 1));
    sdfConsInitPacketHeader((SdfDrawPacket *)packet, (flag << 9) | ITF_SPRITE_PRIMITIVE_BITS, 3, 0x551, 1);
    dst = (u64 *)sdfConsMeasurePacketWithHeader(packet);
    dst[0] = (u64)colors->components[0] | ((u64)colors->components[1] << 32);
    dst[1] = (u64)colors->components[2] | ((u64)colors->components[3] << 32);
    dst += 2;
    dst[1] = (u64)tail;
    dst[0] = (u64)(u32)(vertices[0].x + ITF_GS_X_BIAS) | ((u64)(vertices[0].y + ITF_GS_Y_BIAS) << 32);
    dst += 2;
    dst[1] = (u64)tail;
    dst[0] = (u64)(u32)(vertices[1].x + ITF_GS_X_BIAS) | ((u64)(vertices[1].y + ITF_GS_Y_BIAS) << 32);
    sdfAppendPacket(command, packet);
}

/* Emit an indexed triangle strip, packing two vertices per GIF loop.
 * Header loop count truncates odd vertex counts; the native writer does not. */
void itfEmitQuadListWide(DrawVertex *vertices, DrawColorRec *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, SdfListHead *command) {
    s32 packet;
    u64 *dst;
    s32 pairCount = count >> 1;
    s32 i;

    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(4, pairCount));
    sdfConsInitPacketHeader((SdfDrawPacket *)packet, ITF_TRIANGLE_STRIP_PRIMITIVE_BITS, 4, 0x5151, pairCount);
    dst = (u64 *)sdfConsMeasurePacketWithHeader(packet);
    for (i = 0; i < count; i++) {
        DrawVertex *vertex = &vertices[*vertexIndex++];
        DrawColorRec *color = &colors[*colorIndex++];

        dst[0] = (u64)color->components[0] | ((u64)color->components[1] << 32);
        dst[1] = (u64)color->components[2] | ((u64)color->components[3] << 32);
        dst += 2;
        dst[1] = (u64)tail;
        dst[0] = (u64)(u32)(vertex->x + ITF_GS_X_BIAS) | ((u64)(vertex->y + ITF_GS_Y_BIAS) << 32);
        dst += 2;
    }
    sdfAppendPacket(command, packet);
}

/* Emit indexed RGBA/XYZ2 records with the line-strip primitive. */
void itfEmitQuadListA(DrawVertex *vertices, DrawColorRec *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, SdfListHead *command) {
    s32 packet;
    u64 *dst;
    s32 i;

    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, count));
    sdfConsInitPacketHeader((SdfDrawPacket *)packet, ITF_LINE_STRIP_PRIMITIVE_BITS, 2, 0x51, count);
    dst = (u64 *)sdfConsMeasurePacketWithHeader(packet);
    for (i = 0; i < count; i++) {
        DrawVertex *vertex = &vertices[*vertexIndex++];
        DrawColorRec *color = &colors[*colorIndex++];

        dst[0] = (u64)color->components[0] | ((u64)color->components[1] << 32);
        dst[1] = (u64)color->components[2] | ((u64)color->components[3] << 32);
        dst += 2;
        dst[1] = (u64)tail;
        dst[0] = (u64)(u32)(vertex->x + ITF_GS_X_BIAS) | ((u64)(vertex->y + ITF_GS_Y_BIAS) << 32);
        dst += 2;
    }
    sdfAppendPacket(command, packet);
}

/* Emit indexed RGBA/XYZ2 records with independent line primitives. */
void itfEmitQuadListB(DrawVertex *vertices, DrawColorRec *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, SdfListHead *command) {
    s32 packet;
    u64 *dst;
    s32 i;

    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, count));
    sdfConsInitPacketHeader((SdfDrawPacket *)packet, ITF_LINES_PRIMITIVE_BITS, 2, 0x51, count);
    dst = (u64 *)sdfConsMeasurePacketWithHeader(packet);
    for (i = 0; i < count; i++) {
        DrawVertex *vertex = &vertices[*vertexIndex++];
        DrawColorRec *color = &colors[*colorIndex++];

        dst[0] = (u64)color->components[0] | ((u64)color->components[1] << 32);
        dst[1] = (u64)color->components[2] | ((u64)color->components[3] << 32);
        dst += 2;
        dst[1] = (u64)tail;
        dst[0] = (u64)(u32)(vertex->x + ITF_GS_X_BIAS) | ((u64)(vertex->y + ITF_GS_Y_BIAS) << 32);
        dst += 2;
    }
    sdfAppendPacket(command, packet);
}

/* The packet builder returns a 0x30-byte command with two trailing qwords. */
typedef struct TextPacketTail {
    u8 pad00[0x20];
    u64 value;
    u64 registerCode;
} TextPacketTail;

/* Write TEST for the selected GS context, not the ALPHA blend register. */
void itfSendBlendPacket(SdfListHead *command, u64 value, s32 flag) {
    s32 packet = sdfAllocPacketAligned(ITF_STATE_PACKET_BYTES);
    TextPacketTail *dst = (TextPacketTail *)sdfConsFinalizePacketHeader(packet, ITF_STATE_PACKET_BYTES);

    dst->value = value;
    dst->registerCode = flag ? ITF_TEST_SECOND_CONTEXT : ITF_TEST_FIRST_CONTEXT;
    sdfAppendPacket(command, packet);
}

/* Write an unchecked table entry to ALPHA for the selected GS context. */
void itfSendTablePacket(SdfListHead *command, s32 index, s32 flag) {
    s32 packet = sdfAllocPacketAligned(ITF_STATE_PACKET_BYTES);
    TextPacketTail *dst = (TextPacketTail *)sdfConsFinalizePacketHeader(packet, ITF_STATE_PACKET_BYTES);

    dst->value = D_00357998[index];
    dst->registerCode = flag ? ITF_ALPHA_SECOND_CONTEXT : ITF_ALPHA_FIRST_CONTEXT;
    sdfAppendPacket(command, packet);
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00199560);

extern u8 D_003BB198[8];
extern u8 D_003BB1A0[8];
extern u8 D_003579B0[];
extern u8 D_003579B8[];
extern TextVector D_003579C0[4];

/* Draw filled and remaining quad regions. The optional highlight is produced
 * as signed fixed-point vectors and passed as their four-word RGBA image. */
void func_00199638(s32 value, s32 limit, s32 highlight,
                   DrawVertex *bounds, u32 tail, DrawColorRec *filledColor,
                   DrawColorRec *remainingColor, SdfListHead *command) {
    DrawVertex vertices[6];
    TextVector highlightColors[4];
    s32 left = bounds[0].x;
    s32 right = bounds[1].x;
    s32 width = (right - left) >> 4;
    s32 filledWidth;

    if (limit > 0) {
        filledWidth = (((value << 16) / limit) * width) >> 12;
    } else {
        filledWidth = 0;
    }
    vertices[0].x = left;
    vertices[0].y = bounds[0].y;
    vertices[1].x = filledWidth + left;
    vertices[1].y = bounds[0].y;
    vertices[2].x = right;
    vertices[2].y = bounds[0].y;
    vertices[3].x = left;
    vertices[3].y = bounds[1].y;
    vertices[4].x = filledWidth + left;
    vertices[4].y = bounds[1].y;
    vertices[5].x = right;
    vertices[5].y = bounds[1].y;
    itfDrawQuadFlat4(vertices, filledColor, D_003BB198, D_003BB1A0, tail, command);
    itfDrawQuadFlat4(vertices, remainingColor, D_003BB198 + 4, D_003BB1A0, tail, command);
    if (highlight != 0 && filledWidth >= 3) {
        itfScaleVectors(highlightColors, filledColor->components[0],
                        filledColor->components[1], filledColor->components[2],
                        filledColor->components[3], D_003579C0, 4);
        itfEmitQuadListA(vertices, (DrawColorRec *)highlightColors,
                        D_003579B0, D_003579B8, 3, tail, command);
        itfEmitQuadListA(vertices, (DrawColorRec *)highlightColors,
                        D_003579B0 + 3, D_003579B8 + 3, 3, tail, command);
    }
}


u32 func_001997E0(void) {
    return 0;
}

u32 func_001997E8(void) {
    return 1;
}

extern char D_003A13F0[]; /* "Camp process halted.\n", followed by padding no C emits */

void mnuReportCampProcessHalted(void) {
    if (dds3AdminGetActiveMode() != 5) {
        dds3AdminSetControlFlag();
    }
    func_003003F0(D_003A13F0);
}


extern s32 D_00357AB8[];

UiSprite *func_00199828(s32 kind, u32 value) {
    SdfMemBlock *allocation = sdfAllocGeneralBlock(sizeof(UiSprite));
    UiSprite *work = (UiSprite *)sdfResourceRetainAddress(allocation);

    work->kind = kind;
    work->allocation = allocation;
    allocation = sdfAllocGeneralBlock(D_00357AB8[kind]);
    work->payloadAllocation = allocation;
    work->payload = (u32 *)sdfResourceRetainAddress(allocation);
    if (value != 0) {
        switch (kind) {
        case 6:
            *work->payload = value;
            break;
        case 7:
            ((UiSpriteTexturePayload *)work->payload)->texture = (SdfTex *)value;
            break;
        case 8:
            ((UiSpriteBandPayload *)work->payload)->texture = (SdfTex *)value;
            break;
        case 9:
            *work->payload = value;
            break;
        }
    }
    return work;
}

INCLUDE_RODATA(const s32, "game/code_00196478", D_003A13F0);

INCLUDE_SDATA(const s32, "game/code_00196478", D_003BB188);

INCLUDE_SDATA(const s32, "game/code_00196478", D_003BB18C);

INCLUDE_SDATA(const s32, "game/code_00196478", itfFontTestScriptTask);

INCLUDE_SDATA(const s32, "game/code_00196478", D_003BB198);

INCLUDE_SDATA(const s32, "game/code_00196478", D_003BB1A0);

