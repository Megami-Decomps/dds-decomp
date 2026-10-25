#include "sdk_ps2.h"
#include "scr.h"
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
#include "kwln.h"
#include "kwln_task_create.h"
#include "kwln_task_state.h"
#include "sdf_resource.h"
#include "itf.h"
#include "itf_panel_api.h"
#include "fr_font_glyph_chain_count.h"
#include "itf_mem_node.h"
#include "itf_resource_segments.h"
#include "sdf.h"
#include "itf_panel_draw.h"
#include "sdf_texture_file.h"
#include "btl.h"
#include "fr_font.h"
/* Declared before first use (were implicit int). */
extern _FRQ * frDestroyFontRequest(_FRQ *glyph);
extern void frDelFontStyle(s32 slotId);
/* Original-style implicit int, kept: matched code depends on it: itfDrawTexturedSpriteRect. */


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

#include "sdf_debug_api.h"

extern dds3ProcessID_t *itfFontTestScriptTask;
extern u32 D_003BB18C;

extern sdfTexHandle_t *itfBackgroundSpriteTexture;

extern u8 D_003BB188[];
extern void frFontCheckPendingGlyphState(FrFontCtx *);
extern void frMessageReturnCode(FrFontCtx *);
extern void frFontAppendIndexedTextToGlyphChain(u8, FrFontCtx *);
extern u8 gModeFlag;
extern void mnuSetTitleVoicePrefixIndex(s32);
extern void mnuPlayTitleVoiceFile(char *);
extern s32 mnuGetTitleEffectFrameCounter(void);

extern _FRQ *frCombineFontRequest(_FRQ *, _FRQ *);
extern _FRQ *frFontLinkGlyph(_FRQ *, _FRQ *, s32);

extern void frSetWhiteSpaceW(s32 kind, u64 flag);

extern void frEnableMode(s32 value);

extern u8 frDisableMode(u8 value);

extern u8 D_00357980[];
extern u8 D_00357988[];

extern s32 frDefaultFontHandle;

extern u32 itfTextLipsMotionId;

extern u32 itfTextLipsUnitId;
extern s32 D_003BD818;
void frFontEnsureSlotLoaded(s32 id, const char *path);
extern u16 itfGlyphDecodeTable[];

extern s32 itfScanEncodedTextControls(FrFontCtx *args);

/* Subtract one modulo 256 from the first byte, then advance over the whole pair. */
u32 itfReadEncodedTextLead(FrFontCtx *args) {
    s32 *position = &args->encodedTextOffset;
    u8 *byte = args->encodedText + *position;
    u32 value = *byte;

    *position += 2;
    return (value + ITF_BYTE_MASK) & ITF_BYTE_MASK;
}

/* Decode a little-endian pair; encoded high byte 0xFF is the zero escape.
 * Neither reader checks the input length. */
u32 itfReadEncodedCode(FrFontCtx *args) {
    u8 *bytes = args->encodedText;
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
extern u8 *datSkillMessages;
extern u8 *datItemMessages;

s32 itfProcessEncodedTextControl(s32 code, FrFontCtx *stream) {
    s32 *position = &stream->encodedTextOffset;
    u8 *bytes = stream->encodedText;
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
            frFontCheckPendingGlyphState(stream);
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

extern s8 frFontEncodedTextGlyphSpacing;

extern _FRQ * frFontBuildGlyphChain(const char *text, s8 fontIndex, s8 firstOption, s8 secondOption, _FRQ *existingGlyph);

_FRQ *itfBuildGlyphChainFromEncodedStream(FrFontCtx *stream) {
    u8 encodedText[3];
    s32 value;
    u8 *bytes = stream->encodedText;

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
            frFontCheckPendingGlyphState(stream);
            if (value < 0x80) {
                encodedText[0] = value;
                encodedText[1] = 0;
                stream->glyphChain = frFontBuildGlyphChain((const char *)encodedText, stream->fontIndex,
                    stream->firstOption, stream->secondOption, stream->glyphChain);
            } else {
                value = (value << 8) | stream->encodedText[stream->encodedTextOffset++];
                if (value <= 0xC07F) {
                    encodedText[0] = (u32)value >> 8;
                    encodedText[1] = value;
                    encodedText[2] = 0;
                    stream->glyphChain = frFontBuildGlyphChain((const char *)encodedText, stream->fontIndex,
                        stream->firstOption, stream->secondOption, stream->glyphChain);
                } else {
                    value -= 0x4000;
                    encodedText[0] = (u32)value >> 8;
                    encodedText[1] = value;
                    encodedText[2] = 0;
                    stream->glyphChain = frFontBuildGlyphChain((const char *)encodedText, 8,
                        stream->firstOption, stream->secondOption, stream->glyphChain);
                }
            }
            frSetFontSpace(stream->glyphChain, frFontEncodedTextGlyphSpacing);
            frSetPriority(stream->glyphChain, stream->z);
        }
        bytes = stream->encodedText;
    }
    frDisableMode(1);
    frEnableMode(2);
    return stream->glyphChain;
}

_FRQ *MessageRequest(s32 x, s32 y, u8 *encodedText, _FRQ *sub) {
    return MessageRequestEx2(x, y, 0, 0, 0, 0, 0x80, encodedText, sub);
}

_FRQ *MessageRequestEx(s32 x, s32 y, s32 channel0, s32 channel1, s32 channel2, s32 channel3, u8 *encodedText, _FRQ *sub) {
    return MessageRequestEx2(x, y, 0, channel0 & 0xFF, channel1 & 0xFF, channel2 & 0xFF, channel3 & 0xFF, encodedText, sub);
}

extern _FRQ *itfBuildGlyphChainFromEncodedStream(FrFontCtx *);

_FRQ *MessageRequestEx2(s32 x, s32 y, s32 depth, s32 channel0, s32 channel1, s32 channel2, s32 channel3, u8 *encodedText, _FRQ *sub) {
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

_FRQ *itfDrawColor(s32 x, s32 y, s32 depth, s32 channel0, s32 channel1,
                   s32 channel2, s32 channel3, u8 *encodedText, _FRQ *unusedSub) {
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

INCLUDE_ASM(const s32, "game/code_00196478", frDumpFontTexture);

s32 itfSkipEncodedTextOpcode(s32 code, FrFontCtx *stream) {
    s32 *position = &stream->encodedTextOffset;
    u8 *bytes = stream->encodedText;
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
    u8 *bytes = stream->encodedText;

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

s32 itfInitTextDrawArgs(u8 *encodedText, _FRQ *sub) {
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
u16 frFontGetSlotCellWidth(s32 slotIndex) {
    return frFontWork.entries[slotIndex].resourceHeader->cellWidth;
}

/* Return the selected resource header's cell height; index is unchecked. */
u16 frFontGetSlotCellHeight(s32 slotIndex) {
    return frFontWork.entries[slotIndex].resourceHeader->cellHeight;
}

void itfSetTextDrawLimit(s32 limit) {
    if (limit < 1) {
        limit = 0x14;
    }
    frDefaultFontHandle = limit;
}

_FRQ *frwGetStaffFontFRQ(u32 x, u32 y, s32 depth, s32 alt, s32 measureFlag, u64 entryFlag, u32 colors, const char *source) {
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

    glyph = frFontAppendTextToGlyphChain((const char *)D_003BB188, 0, 0, 0, previousGlyph);
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
_FRQ *itfCreateFont3TextGlyph(s32 x, s32 y, s32 depth, u32 colors, char *text, _FRQ *previousGlyph) {
    _FRQ *glyph;

    glyph = frFontAppendTextToGlyphChain((const char *)D_003BB188, 0, 0, 0, previousGlyph);
    frDisableMode(2);
    glyph = frFontBuildGlyphChain(text, 3, 0, 0, glyph);
    frEnableMode(2);
    frSetDrawPosition(glyph, x, y);
    frSetPriority(glyph, depth << 4);
    frSetFontColor(glyph, colors);
    return glyph;
}

/* Build font-index-3 text with signed spacing -2 and return its glyph chain. */
_FRQ *itfCreateTightFont3TextGlyph(s32 x, s32 y, s32 depth, s32 colors, char *text, _FRQ *previousGlyph) {
    _FRQ *glyph;

    glyph = frFontAppendTextToGlyphChain((const char *)D_003BB188, 0, 0, 0, previousGlyph);
    frDisableMode(2);
    glyph = frFontBuildGlyphChain(text, 3, 0, 0, glyph);
    frSetFontSpace(glyph, -2);
    frEnableMode(2);
    frSetDrawPosition(glyph, x, y);
    frSetPriority(glyph, depth << 4);
    frSetFontColor(glyph, colors);
    return glyph;
}

/* Attach the alternate font glyph, with a 12x16 cell, to its parent. */
void itfAttachGlyph12x16(u32 x, u32 y, s32 depth, u32 colors,
                                    void *glyphSource, _FRQ *parent) {
    _FRQ *glyph;

    glyph = frFontBuildGlyphChain(glyphSource, 0, 0, 0, 0);
    frSetFontSize(glyph, 0xc, 0x10);
    frSetFontSpace(glyph, 3);
    frSetDrawPosition(glyph, x, y);
    frSetPriority(glyph, depth << 4);
    frSetFontColor(glyph, colors);
    frFontLinkGlyph(parent, glyph, 0);
}

extern u8 *MessageNumSearch(s32 textId, FrFontTextBank *bank, s32 mode);
extern s32 frFontMeasureGlyphStripWidth(s32 line, _FRQ *text);
extern void frFontMoveChainTo(s32 x, s32 y, _FRQ *text);

_FRQ *itfDrawBankTextWithLayoutFlags(s32 x, s32 y, s32 depth, u16 textId, FrFontTextBank *bank, s32 flags) {
    u8 *text = MessageNumSearch(textId, bank, 0);
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
    handle = itfDrawColor(x, y, depth, 1, 0, 0, 0x80, text, 0);
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

_FRQ *itfDrawTextWithSelectedFontMode(s32 x, s32 y, s32 depth, s8 fontMode, u16 textId, s32 flags) {
    _FRQ *result = NULL;

    itfSetTextDrawLimit(0x13);
    switch (fontMode) {
    case 0:
        result = itfDrawBankTextWithLayoutFlags(x, y, depth, textId, (FrFontTextBank *)datSkillMessages, flags);
        break;
    case 1:
        result = itfDrawBankTextWithLayoutFlags(x, y, depth, textId, (FrFontTextBank *)datItemMessages, flags);
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

extern dds3ProcessID_t *scrStartScriptByName(u32, const char *, ScrProcedureIndex);
extern s32 itfDrawBackgroundAndGetTaskReadyMask(dds3ProcessID_t *task);
extern void itfReleaseFontTestTaskResources(dds3ProcessID_t *task);

/* Start the background draw task and host-file font-test process. */
void itfStartFontTestScene(void) {
    nbLoadDeadMoonTex();
    dds3StartProcess("test_font", 0x2B06, 0, 0, itfDrawBackgroundAndGetTaskReadyMask, itfReleaseFontTestTaskResources, 0);
    itfFontTestScriptTask = scrStartScriptByName(0x258, "host0:../../../dds3data/font/test.bf", 0);
}

/* Advance/retain the test glyph and release its background texture reference. */
void itfReleaseFontTestTaskResources(dds3ProcessID_t *task) {
    frDestroyFontRequest(D_003BB18C);
    itfReleaseBackgroundSpriteTexture();
}

/* Draw first, then report all bits set only for a task awaiting destruction. */
s32 itfDrawBackgroundAndGetTaskReadyMask(dds3ProcessID_t *task) {
    KwlnTaskState taskState;
    u32 readyMask;

    itfDrawBackgroundSprite();
    taskState = dds3GetProcessStat(itfFontTestScriptTask);
    readyMask = 0xffffffff;
    if (taskState != KWLN_TASK_DESTROY_PENDING) {
        readyMask = 0;
    }
    return readyMask;
}

/* Initialize a FIFO free list and empty active list.
 * Native do/while writes at least two nodes; the s8 index can wrap. */
void itfMiscLinkInitialize(ItfMesPool *pool, mesflow_tag *nodes, s32 count, s32 stride) {
    s8 index = 0;
    mesflow_tag *previous = NULL;
    mesflow_tag *node = nodes;
    mesflow_tag *next;

    do {
        count--;
        next = (mesflow_tag *)((u8 *)node + stride);
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
    pool->lastFree = node;
    pool->firstFree = nodes;
    pool->activeTail = NULL;
    pool->activeHead = NULL;
}

/* Move the free-list head to the active-list tail; exhausted pools return NULL. */
mesflow_tag *itfMiscLinkTake(ItfMesPool *pool) {
    mesflow_tag *node = pool->firstFree;
    mesflow_tag *next;

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
        pool->lastFree = NULL;
    }
    pool->firstFree = next;
    return node;
}

/* Unlink an active node and append it to the free-list tail; membership is trusted. */
void itfMiscLinkGive(mesflow_tag *node, ItfMesPool *pool) {
    mesflow_tag *previous = node->previous;
    mesflow_tag *next = node->next;

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
        mesflow_tag *lastFree = pool->lastFree;
        node->previous = lastFree;
        if (lastFree != NULL) {
            lastFree->next = node;
        }
    }
    pool->lastFree = node;
    if (pool->firstFree == NULL) {
        pool->firstFree = node;
    }
}

/* Multiply XYZ, cap only the positive product at 255 << 16, then shift by 16.
 * Negative products are not clamped; W comes from the argument. */
void itfMiscSetRatioRGB(DrawColorRec *output, s32 scaleX, s32 scaleY, s32 scaleZ,
                   s32 w, const TextVector *input, s32 count) {
    while (count > 0) {
        s32 x = scaleX * input->x;
        s32 y = scaleY * input->y;
        s32 z = scaleZ * input->z;

        output->components[3] = w;
        if (x > ITF_FIXED_COMPONENT_MAX) x = ITF_FIXED_COMPONENT_MAX;
        if (y > ITF_FIXED_COMPONENT_MAX) y = ITF_FIXED_COMPONENT_MAX;
        if (z > ITF_FIXED_COMPONENT_MAX) z = ITF_FIXED_COMPONENT_MAX;
        output->components[0] = x >> ITF_FIXED_FRACTION_BITS;
        output->components[1] = y >> ITF_FIXED_FRACTION_BITS;
        output->components[2] = z >> ITF_FIXED_FRACTION_BITS;
        output++;
        input++;
        count--;
    }
}

/* Replace each child color; parent color words are not modified. */
void itfMiscChangeFontVertexColor(_FRQ *entry, u32 color) {
    for (; entry != NULL; entry = entry->previous) {
        _TEXT *child;
        for (child = entry->firstChild; child != NULL; child = child->next) {
            child->parentDimensionsOrRenderWord.renderWord = color;
        }
    }
}

/* Clear each child's low byte, then OR unmasked bits into the full color word. */
void itfMiscChangeFontAlpha(_FRQ *entry, u32 colorBits) {
    for (; entry != NULL; entry = entry->previous) {
        _TEXT *child;
        for (child = entry->firstChild; child != NULL; child = child->next) {
            child->parentDimensionsOrRenderWord.renderWord = (child->parentDimensionsOrRenderWord.renderWord & ~ITF_BYTE_MASK) | colorBits;
        }
    }
}

/* Translate parent entries only; child coordinates are left unchanged. */
void itfMiscMoveFontPosition(_FRQ *entry, u32 xOffset, u32 yOffset) {
    for (; entry != NULL; entry = entry->previous) {
        entry->x += xOffset;
        entry->y += yOffset;
    }
}

#include "sdf_packet_alloc.h"
extern s32 sdf2DGetPrimPacketSize(s32 registerCount, s32 loopCount);
extern u8 *sdf2DGetPrimPtr(SdfDrawPacket *packet);
extern void *sdf2DMakeStdPacket(void *, s32);
extern u64 D_00357998[];

/* Keep the sprite texture handle while releasing the temporary file allocation. */
sdfTexHandle_t *itfMiscLoadTexture(const char *path) {
    sdfMemHandle_t *fileAllocation;
    sdfTexHandle_t *textureHandle;
    u32 assetInfo[ITF_VECTOR_WORD_COUNT];

    fileAllocation = sdfDevLoadFileEx(path, assetInfo, 0);
    textureHandle = sdfTexSet((SdfTextureFileHeader *)(assetInfo[0]));
    sdfMemFreeDirect(fileAllocation);
    return textureHandle;
}

/* Emit three per-vertex RGBA/XYZ2 records; PRIM's low bits select a triangle.
 * Coordinates receive the native GS screen biases; no clipping is performed. */
void itfMiscDrawTrianglePrim(DrawVertex *vertices, DrawColorRec *colors, s32 xOffset, s32 yOffset, u32 tail, sdfDrawTag_t *command) {
    SdfDrawPacket *packet;
    u64 *dst;
    s32 i;

    packet = (SdfDrawPacket *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, ITF_TRIANGLE_VERTEX_COUNT));
    sdf2DMakePrimPacketHeader(packet, ITF_TRIANGLE_PRIMITIVE_BITS, 2, 0x51, ITF_TRIANGLE_VERTEX_COUNT);
    dst = (u64 *)sdf2DGetPrimPtr(packet);
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
    sdfDrawTagLink(command, (u32)packet);
}

/* Emit an indexed four-vertex triangle fan with a color record per vertex. */
void itfMiscDrawRectanglePrim(DrawVertex *vertices, DrawColorRec *colors, u8 *vertexIndex, u8 *colorIndex, u32 tail, sdfDrawTag_t *command) {
    SdfDrawPacket *packet;
    u64 *dst;
    s32 i;

    packet = (SdfDrawPacket *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, ITF_QUAD_VERTEX_COUNT));
    sdf2DMakePrimPacketHeader(packet, ITF_QUAD_FAN_PRIMITIVE_BITS, 2, 0x51, ITF_QUAD_VERTEX_COUNT);
    dst = (u64 *)sdf2DGetPrimPtr(packet);
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
    sdfDrawTagLink(command, (u32)packet);
}

/* Draw three adjacent quads using zero-alpha/source-alpha edge color records.
 * Border widths and resulting signed middle width are not clamped. */
void itfDrawThreePanelBorderBands(DrawVertex *bounds, DrawColorRec *color, u32 tail, s32 borderWidth, sdfDrawTag_t *command) {
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

    itfMiscDrawRectanglePrim(vertices, colors, D_00357980, D_00357988, tail, command);
    vertices[0].x += borderWidth;
    vertices[1].x += middleWidth;
    vertices[2].x += middleWidth;
    vertices[3].x += borderWidth;
    itfMiscDrawRectanglePrim(vertices, colors, D_00357980, D_00357988 + 4, tail, command);
    vertices[0].x += middleWidth;
    vertices[1].x += borderWidth;
    vertices[2].x += borderWidth;
    vertices[3].x += middleWidth;
    itfMiscDrawRectanglePrim(vertices, colors, D_00357980, D_00357988 + 8, tail, command);
}

/* Emit a textured triangle fan: each four-float input supplies S/T/Q only.
 * The fourth float is skipped, and the context word is shifted without masking. */
void itfMiscDrawTexPoly(DrawVertex *vertices, f32 *uvs, DrawColorRec *colors, u32 tail, s32 flag, sdfDrawTag_t *command) {
    SdfDrawPacket *packet;
    u64 *dst;
    s32 i;

    packet = (SdfDrawPacket *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(3, ITF_QUAD_VERTEX_COUNT));
    sdf2DMakePrimPacketHeader(packet, (flag << 9) | ITF_TEXTURED_FAN_PRIMITIVE_BITS, 3, 0x512, ITF_QUAD_VERTEX_COUNT);
    dst = (u64 *)sdf2DGetPrimPtr(packet);
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
    sdfDrawTagLink(command, (u32)packet);
}

/* Bind the texture and emit one two-corner sprite using two packed UV pairs.
 * Unwritten padding words in the native packet remain untouched. */
void itfMiscDrawSpritePrim(DrawVertex *vertices, DrawColorRec *uv,
                            DrawColorRec *colors, s32 tail,
                            const sdfTexHandle_t *borrowedTexture, s32 flag,
                            sdfDrawTag_t *command) {
    SdfDrawPacket *packet;
    u64 *dst;
    s32 x0, y0, x1, y1;

    packet = (SdfDrawPacket *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(5, 1));
    sdf2DMakePrimPacketHeader(packet, (flag << 9) | ITF_TEXTURED_SPRITE_PRIMITIVE_BITS, 5, 0x53531, 1);
    x0 = vertices[0].x + ITF_GS_X_BIAS;
    y0 = vertices[0].y + ITF_GS_Y_BIAS;
    x1 = vertices[1].x + ITF_GS_X_BIAS;
    y1 = vertices[1].y + ITF_GS_Y_BIAS;
    dst = (u64 *)sdf2DGetPrimPtr(packet);
    dst[0] = (u64)colors->components[0] | ((u64)colors->components[1] << 32);
    dst[1] = (u64)colors->components[2] | ((u64)colors->components[3] << 32);
    dst[2] = (u64)uv->components[0] | ((u64)uv->components[1] << 32);
    dst[4] = (u64)(u32)x0 | ((u64)y0 << 32);
    dst[5] = (u64)(u32)tail;
    dst[6] = (u64)uv->components[2] | ((u64)uv->components[3] << 32);
    dst[8] = (u64)(u32)x1 | ((u64)y1 << 32);
    dst[9] = (u64)(u32)tail;
    /* The legacy packet API is mutable-typed; this path only reads GS words. */
    sdf2DLinkTexPacket(command, (sdfTexHandle_t *)borrowedTexture, flag);
    sdfDrawTagLink(command, (u32)packet);
}

/* Emit a colored textured sprite without binding a texture in this function. */
void itfMiscDrawSpritePrim2(DrawVertex *vertices, DrawColorRec *uv, DrawColorRec *colors, u32 tail, s32 flag, sdfDrawTag_t *command) {
    SdfDrawPacket *packet;
    u64 *dst;
    s32 x0, y0, x1, y1;

    packet = (SdfDrawPacket *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(5, 1));
    sdf2DMakePrimPacketHeader(packet, (flag << 9) | ITF_COLORED_SPRITE_PRIMITIVE_BITS, 5, 0x53531, 1);
    x0 = vertices[0].x + ITF_GS_X_BIAS;
    y0 = vertices[0].y + ITF_GS_Y_BIAS;
    x1 = vertices[1].x + ITF_GS_X_BIAS;
    y1 = vertices[1].y + ITF_GS_Y_BIAS;
    dst = (u64 *)sdf2DGetPrimPtr(packet);
    dst[0] = (u64)colors->components[0] | ((u64)colors->components[1] << 32);
    dst[1] = (u64)colors->components[2] | ((u64)colors->components[3] << 32);
    dst[2] = (u64)uv->components[0] | ((u64)uv->components[1] << 32);
    dst[4] = (u64)(u32)x0 | ((u64)y0 << 32);
    dst[5] = (u64)tail;
    dst[6] = (u64)uv->components[2] | ((u64)uv->components[3] << 32);
    dst[8] = (u64)(u32)x1 | ((u64)y1 << 32);
    dst[9] = (u64)tail;
    sdfDrawTagLink(command, (u32)packet);
}

/* Despite the current name, PRIM selects a two-corner sprite, not a line. */
void itfMiscDrawSpritePlanePrim(DrawVertex *vertices, DrawColorRec *colors, u32 tail, s32 flag, sdfDrawTag_t *command) {
    SdfDrawPacket *packet;
    u64 *dst;

    packet = (SdfDrawPacket *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(3, 1));
    sdf2DMakePrimPacketHeader(packet, (flag << 9) | ITF_SPRITE_PRIMITIVE_BITS, 3, 0x551, 1);
    dst = (u64 *)sdf2DGetPrimPtr(packet);
    dst[0] = (u64)colors->components[0] | ((u64)colors->components[1] << 32);
    dst[1] = (u64)colors->components[2] | ((u64)colors->components[3] << 32);
    dst += 2;
    dst[1] = (u64)tail;
    dst[0] = (u64)(u32)(vertices[0].x + ITF_GS_X_BIAS) | ((u64)(vertices[0].y + ITF_GS_Y_BIAS) << 32);
    dst += 2;
    dst[1] = (u64)tail;
    dst[0] = (u64)(u32)(vertices[1].x + ITF_GS_X_BIAS) | ((u64)(vertices[1].y + ITF_GS_Y_BIAS) << 32);
    sdfDrawTagLink(command, (u32)packet);
}

/* Emit an indexed triangle strip, packing two vertices per GIF loop.
 * Header loop count truncates odd vertex counts; the native writer does not. */
void itfMiscDrawTriangleStripPrim(DrawVertex *vertices, DrawColorRec *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command) {
    SdfDrawPacket *packet;
    u64 *dst;
    s32 pairCount = count >> 1;
    s32 i;

    packet = (SdfDrawPacket *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(4, pairCount));
    sdf2DMakePrimPacketHeader(packet, ITF_TRIANGLE_STRIP_PRIMITIVE_BITS, 4, 0x5151, pairCount);
    dst = (u64 *)sdf2DGetPrimPtr(packet);
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
    sdfDrawTagLink(command, (u32)packet);
}

/* Emit indexed RGBA/XYZ2 records with the line-strip primitive. */
void itfMiscDrawLineStripPrim(DrawVertex *vertices, DrawColorRec *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command) {
    SdfDrawPacket *packet;
    u64 *dst;
    s32 i;

    packet = (SdfDrawPacket *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, count));
    sdf2DMakePrimPacketHeader(packet, ITF_LINE_STRIP_PRIMITIVE_BITS, 2, 0x51, count);
    dst = (u64 *)sdf2DGetPrimPtr(packet);
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
    sdfDrawTagLink(command, (u32)packet);
}

/* Emit indexed RGBA/XYZ2 records with independent line primitives. */
void itfMiscDrawLinePrim(DrawVertex *vertices, DrawColorRec *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command) {
    SdfDrawPacket *packet;
    u64 *dst;
    s32 i;

    packet = (SdfDrawPacket *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, count));
    sdf2DMakePrimPacketHeader(packet, ITF_LINES_PRIMITIVE_BITS, 2, 0x51, count);
    dst = (u64 *)sdf2DGetPrimPtr(packet);
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
    sdfDrawTagLink(command, (u32)packet);
}

/* Write TEST for the selected GS context, not the ALPHA blend register. */
void itfMiscDrawTestPrim(sdfDrawTag_t *command, u64 value, s32 flag) {
    void *packet = (void *)sdfGrAllocPacketBuff(ITF_STATE_PACKET_BYTES);
    ItfGsStatePacket *dst = (ItfGsStatePacket *)sdf2DMakeStdPacket(packet, ITF_STATE_PACKET_BYTES);

    dst->value = value;
    dst->registerCode = flag ? ITF_TEST_SECOND_CONTEXT : ITF_TEST_FIRST_CONTEXT;
    sdfDrawTagLink(command, (u32)packet);
}

/* Write an unchecked table entry to ALPHA for the selected GS context. */
void itfMiscDrawAlphaPrim(sdfDrawTag_t *command, s32 index, s32 flag) {
    void *packet = (void *)sdfGrAllocPacketBuff(ITF_STATE_PACKET_BYTES);
    ItfGsStatePacket *dst = (ItfGsStatePacket *)sdf2DMakeStdPacket(packet, ITF_STATE_PACKET_BYTES);

    dst->value = D_00357998[index];
    dst->registerCode = flag ? ITF_ALPHA_SECOND_CONTEXT : ITF_ALPHA_FIRST_CONTEXT;
    sdfDrawTagLink(command, (u32)packet);
}

/* Write CLAMP for the selected GS context. */
void itfMiscDrawClampPrim(sdfDrawTag_t *command, s32 flag, s32 wms, s32 wmt, s32 minu, s32 maxu, s32 minv, s32 maxv) {
    void *packet = (void *)sdfGrAllocPacketBuff(ITF_STATE_PACKET_BYTES);
    ItfGsStatePacket *dst = (ItfGsStatePacket *)sdf2DMakeStdPacket(packet, ITF_STATE_PACKET_BYTES);

    dst->value = ((u64)maxv << 34) | ((u64)minv << 24) | (maxu << 14) | (minu << 4) | (wmt << 2) | wms;
    dst->registerCode = flag ? SDF_GS_CLAMP_2 : SDF_GS_CLAMP_1;
    sdfDrawTagLink(command, (u32)packet);
}

extern u8 D_003BB198[8];
extern u8 D_003BB1A0[8];
extern u8 D_003579B0[];
extern u8 D_003579B8[];
extern TextVector D_003579C0[4];

/* Draw filled and remaining quad regions. The optional highlight is produced
 * as signed fixed-point vectors and passed as their four-word RGBA image. */
void itfMiscDrawGauge(s32 value, s32 limit, s32 highlight,
                   DrawVertex *bounds, u32 tail, DrawColorRec *filledColor,
                   DrawColorRec *remainingColor, sdfDrawTag_t *command) {
    DrawVertex vertices[6];
    DrawColorRec highlightColors[4];
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
    itfMiscDrawRectanglePrim(vertices, filledColor, D_003BB198, D_003BB1A0, tail, command);
    itfMiscDrawRectanglePrim(vertices, remainingColor, D_003BB198 + 4, D_003BB1A0, tail, command);
    if (highlight != 0 && filledWidth >= 3) {
        itfMiscSetRatioRGB(highlightColors, filledColor->components[0],
                        filledColor->components[1], filledColor->components[2],
                        filledColor->components[3], D_003579C0, 4);
        itfMiscDrawLineStripPrim(vertices, highlightColors,
                        D_003579B0, D_003579B8, 3, tail, command);
        itfMiscDrawLineStripPrim(vertices, highlightColors,
                        D_003579B0 + 3, D_003579B8 + 3, 3, tail, command);
    }
}


/* Retail "DbgDevDump" task callback: perform no dump work and return zero. */
s32 dds3DebugDevDumpProcess(dds3ProcessID_t *task) {
    return 0;
}

u32 itfCamp(void) {
    return 1;
}

extern char D_003A13F0[]; /* "Camp process halted.\n", followed by padding no C emits */

void itfCampStartProcess(void) {
    if (dds3AdminiGetActiveSequence() != 5) {
        dds3AdminiExitSequence();
    }
    printf(D_003A13F0);
}


extern s32 D_00357AB8[];

itfpanel_t *itfCreateUiSpriteByKind(s32 kind, const sdfTexHandle_t *borrowedTexture) {
    sdfMemHandle_t *allocation = sdfMemAllocDirect(sizeof(itfpanel_t));
    itfpanel_t *work = (itfpanel_t *)sdfMemLock(allocation);

    work->kind = kind;
    work->allocation = allocation;
    allocation = sdfMemAllocDirect(D_00357AB8[kind]);
    work->payloadAllocation = allocation;
    work->payload = (u32 *)sdfMemLock(allocation);
    if (borrowedTexture != NULL) {
        switch (kind) {
        case 6:
            *work->payload = (u32)borrowedTexture;
            break;
        case 7:
            ((UiSpriteTexturePayload *)work->payload)->borrowedTexture = borrowedTexture;
            break;
        case 8:
            ((UiSpriteBandPayload *)work->payload)->borrowedTexture = borrowedTexture;
            break;
        case 9:
            *work->payload = (u32)borrowedTexture;
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

