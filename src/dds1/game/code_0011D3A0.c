#include "fld_area_work.h"
#include "fld_npc_motion.h"
#include "sdf_packet_list.h"
#include "ee_mmi.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "pcp_vu0.h"
#include "common.h"
#include "fld_scene_ready_api.h"
#include "fld_encounter_runtime_api.h"
#include "fld_activation_entry.h"
#include "fld_camera_follow_row.h"
#include "fld_named_camera_table.h"
#include "mdl_flag_api.h"
#include "sdf_trig.h"
#include "dds3_path.h"
#include "dds3_object_flags.h"
#include "sdf_dev_state.h"
#include "sdf_resource.h"
#include "field_stage.h"
#include "eff_blur.h"
#include "fpu.h"
#include "fld.h"
#include "evt_world.h"
#include "dds3obj.h"
#include "mdl.h"
#include "dat_state.h"
#include "kwln_task_lifecycle.h"
/* Declared before first use (were implicit int). */
extern s32 fldFindTableEntry(s32 index);
extern void * memset(void *, s32, u32);

/* Signed selectors read signed storage; all writes retain the selected width. */
enum {
    FIELD_EDIT_UNSIGNED_BYTE = 1,
    FIELD_EDIT_UNSIGNED_HALFWORD = 2,
    FIELD_EDIT_UNSIGNED_WORD = 4,
    FIELD_EDIT_SIGNED_BYTE = -1,
    FIELD_EDIT_SIGNED_HALFWORD = -2,
    FIELD_EDIT_SIGNED_WORD = -4,
    FIELD_EDIT_WRAP_INPUT = 0x80,
    FIELD_EDIT_CLAMP_INPUT = 2,
    FIELD_EDIT_COLOR_COARSE_STEP = 10
};

/* Native lookup bounds differ even when two searches share the same table. */
enum {
    FIELD_COORDINATE_SCAN_LIMIT = 512,
    FIELD_MAP_COORDINATE_SCAN_LIMIT = 256,
    FIELD_LOCATION_COORDINATE_SCAN_LIMIT = 640,
    FIELD_STAGE_INDEX_SCAN_LIMIT = 0x280,
    FIELD_STAGE_RECORD_SCAN_LIMIT = 0x60
};
#include "dds3_world_secondary.h"
#include "sdf_packet_alloc.h"
#include "fld_camera_tracking.h"

extern s32 D_003BAB08;

extern s32 D_003BAB0C;

extern s32 D_003BAB10;

extern s32 D_003BAB14;

extern s32 D_003BAB18;

extern s32 D_003BAB1C;

extern s32 D_003BAB20;

extern u8 D_00324530[];

extern char D_0039FA00[];

extern void fldStepValueByPad(f32 *controlledValue, u8 *padState, f32 min, f32 max, f32 step, f32 bigStep);

extern s32 fldTestDrawUpdate(KwlnTask *task);

extern void fldClearCameraMoveMode(void);

extern void fldSetCameraMoveMode(s32);



extern FldActivationEntry D_0034C8F0[256];

extern void fldActivateObjectById(s32);


extern void ptyReturnOne(s32);

extern FieldStageCoordinate D_0032DDB0[];

extern u8 D_0033F068[];

extern u8 D_00342868[];

struct EffWorldNode;

extern s16 D_0032C9B0[];

extern u8 D_00346068[];

void fldSetPacketArgumentPair(u32 *packet, u32 first, u32 second) {
    packet[4] = first;
    packet[5] = second;
}

struct SdfListHead *sdfCreateResetPacketList(void) {
    struct SdfListHead *packet = (struct SdfListHead *)sdfAllocPacketAligned(0x20);

    sdfInitPacketList(packet);
    return packet;
}

/* Build the 0x80-byte bordered rectangle packet: the rectangle with its two color words, then its outline corners. */
u64 *sdfCreateBorderedRectanglePacket(s32 x, s32 y, s32 depth, s32 width, s32 height, u32 colorA, u32 colorB) {
    u64 *packet;
    s32 right;
    s32 bottom;
    u64 depthHigh;

    packet = (u64 *)sdfAllocPacketAligned(0x80);
    packet[0] = 7;
    right = x + width;
    bottom = y + height;
    depthHigh = (u64)(u32)depth << 32;
    packet[1] = 0x5000000700000000ULL;
    packet[2] = 0xB400000000008001ULL;
    packet[3] = 0x55555105510ULL;
    packet[4] = 0x146;
    packet[5] = colorA | 0x3F80000000000000ULL;
    packet[6] = (u32)(((u32)x & 0xFFFF) | ((u32)y << 16)) | depthHigh;
    packet[7] = (u32)(((u32)right & 0xFFFF) | ((u32)bottom << 16)) | depthHigh;
    packet[8] = 0x142;
    packet[9] = colorB | 0x3F80000000000000ULL;
    packet[14] = (u32)(((u32)(x - 0x10) & 0xFFFF) | ((u32)(y - 8) << 16)) | depthHigh;
    packet[10] = packet[14];
    packet[11] = (u32)(((u32)right & 0xFFFF) | ((u32)(y - 8) << 16)) | depthHigh;
    packet[12] = (u32)(((u32)right & 0xFFFF) | ((u32)bottom << 16)) | depthHigh;
    packet[13] = (u32)(((u32)(x - 0x10) & 0xFFFF) | ((u32)bottom << 16)) | depthHigh;
    packet[15] = 0;
    return packet;
}

u64 *fldCreateColoredLinePacket(s32 x0, s32 y0, s32 z0, s32 color0, s32 x1, s32 y1, s32 z1, s32 color1, s32 flags) {
    u64 *packet = (u64 *)sdfAllocPacketAligned(0x50);

    packet[0] = 4;
    packet[1] = 0x5000000400000000ULL;
    packet[2] = 0x5400000000008001ULL;
    packet[3] = 0x51510;
    packet[4] = flags | 0x109;
    packet[5] = (u32)color0 | ((u64)0xFE00 << 46);
    packet[6] = (u32)((x0 & 0xFFFF) | (y0 << 16)) | ((u64)((u32)z0 >> 4) << 32);
    packet[7] = (u32)color1 | ((u64)0xFE00 << 46);
    packet[8] = (u32)((x1 & 0xFFFF) | (y1 << 16)) | ((u64)((u32)z1 >> 4) << 32);
    packet[9] = 0;
    return packet;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011D6B0);

/* Wrap input cycles past the bounds; clamp input saturates there.
 * Coarse controls take priority over fine controls. */
void fldStepValueByPad(f32 *controlledValue, u8 *padState, f32 minimum, f32 maximum, f32 step, f32 bigStep) {
    f32 adjustedValue = *controlledValue;

    if ((s8)padState[5] & FIELD_EDIT_WRAP_INPUT) {
        adjustedValue += bigStep;
        if (maximum < adjustedValue) {
            adjustedValue = minimum;
        }
    } else if (padState[5] & FIELD_EDIT_CLAMP_INPUT) {
        adjustedValue += bigStep;
        if (maximum < adjustedValue) {
            adjustedValue = maximum;
        }
    } else if ((s8)padState[4] & FIELD_EDIT_WRAP_INPUT) {
        adjustedValue -= bigStep;
        if (adjustedValue < minimum) {
            adjustedValue = maximum;
        }
    } else if (padState[4] & FIELD_EDIT_CLAMP_INPUT) {
        adjustedValue -= bigStep;
        if (adjustedValue < minimum) {
            adjustedValue = minimum;
        }
    } else if ((s8)padState[7] & FIELD_EDIT_WRAP_INPUT) {
        adjustedValue += step;
        if (maximum < adjustedValue) {
            adjustedValue = minimum;
        }
    } else if (padState[7] & FIELD_EDIT_CLAMP_INPUT) {
        adjustedValue += step;
        if (maximum < adjustedValue) {
            adjustedValue = maximum;
        }
    } else if ((s8)padState[6] & FIELD_EDIT_WRAP_INPUT) {
        adjustedValue -= step;
        if (adjustedValue < minimum) {
            adjustedValue = maximum;
        }
    } else if (padState[6] & FIELD_EDIT_CLAMP_INPUT) {
        adjustedValue -= step;
        if (adjustedValue < minimum) {
            adjustedValue = minimum;
        }
    } else {
        return;
    }
    *controlledValue = adjustedValue;
}

void fldStepValueByCurrentPad(f32 *value, f32 min, f32 max, f32 step, f32 bigStep) {
    fldStepValueByPad(value, D_00324530, min, max, step, bigStep);
}

/* Read the selected signed/unsigned integer, apply pad stepping, then truncate
 * back to its original width. Unsupported storage selectors leave it untouched. */
void fldStepIntByPad(void *valueAddress, s32 storageType, s64 min, s64 max, s64 step, s64 bigStep, s8 *padState) {
    s64 adjustedValue;
    switch (storageType) {
    case FIELD_EDIT_UNSIGNED_BYTE:
        adjustedValue = *(u8 *)valueAddress;
        break;
    case FIELD_EDIT_UNSIGNED_HALFWORD:
        adjustedValue = *(u16 *)valueAddress;
        break;
    case FIELD_EDIT_UNSIGNED_WORD:
        adjustedValue = *(u32 *)valueAddress;
        break;
    case FIELD_EDIT_SIGNED_BYTE:
        adjustedValue = *(s8 *)valueAddress;
        break;
    case FIELD_EDIT_SIGNED_HALFWORD:
        adjustedValue = *(s16 *)valueAddress;
        break;
    case FIELD_EDIT_SIGNED_WORD:
        adjustedValue = *(s32 *)valueAddress;
        break;
    default:
        return;
    }
    if (padState[5] & FIELD_EDIT_WRAP_INPUT) {
        adjustedValue += bigStep;
        if (max < adjustedValue) {
            adjustedValue = min;
        }
    } else if (padState[5] & FIELD_EDIT_CLAMP_INPUT) {
        adjustedValue += bigStep;
        if (max < adjustedValue) {
            adjustedValue = max;
        }
    } else if (padState[4] & FIELD_EDIT_WRAP_INPUT) {
        adjustedValue -= bigStep;
        if (adjustedValue < min) {
            adjustedValue = max;
        }
    } else if (padState[4] & FIELD_EDIT_CLAMP_INPUT) {
        adjustedValue -= bigStep;
        if (adjustedValue < min) {
            adjustedValue = min;
        }
    } else if (padState[7] & FIELD_EDIT_WRAP_INPUT) {
        adjustedValue += step;
        if (max < adjustedValue) {
            adjustedValue = min;
        }
    } else if (padState[7] & FIELD_EDIT_CLAMP_INPUT) {
        adjustedValue += step;
        if (max < adjustedValue) {
            adjustedValue = max;
        }
    } else if (padState[6] & FIELD_EDIT_WRAP_INPUT) {
        adjustedValue -= step;
        if (adjustedValue < min) {
            adjustedValue = max;
        }
    } else if (padState[6] & FIELD_EDIT_CLAMP_INPUT) {
        adjustedValue -= step;
        if (adjustedValue < min) {
            adjustedValue = min;
        }
    } else {
        return;
    }
    switch (storageType) {
    case FIELD_EDIT_SIGNED_BYTE:
    case FIELD_EDIT_UNSIGNED_BYTE:
        *(u8 *)valueAddress = adjustedValue;
        break;
    case FIELD_EDIT_SIGNED_HALFWORD:
    case FIELD_EDIT_UNSIGNED_HALFWORD:
        *(u16 *)valueAddress = adjustedValue;
        break;
    case FIELD_EDIT_SIGNED_WORD:
    case FIELD_EDIT_UNSIGNED_WORD:
        *(u32 *)valueAddress = adjustedValue;
        break;
    }
}

void fldAdjustIntegerUsingMainPad(void *valueAddress, s32 storageType, s64 min, s64 max, s64 step, s64 bigStep) {
    fldStepIntByPad(valueAddress, storageType, min, max, step, bigStep, (s8 *)D_00324530);
}

/* Step a packed color byte by 10 or 1; return whether the word changed.
 * Wrap input only wraps at endpoints. An invalid channel reads the low byte
 * but replaces the high byte, preserving the native selector fallback. */
s32 fldStepColorChannelByPad(u32 *color, s32 channel, s8 *padState) {
    s32 originalColor = *color;
    s32 channelValue = originalColor;
    s32 updatedColor;
    switch (channel) {
    case 0:
        break;
    case 1:
        channelValue = originalColor >> 8;
        break;
    case 2:
        channelValue = originalColor >> 16;
        break;
    case 3:
        channelValue = originalColor >> 24;
        break;
    }
    channelValue &= 0xFF;
    if (((padState[5] & FIELD_EDIT_WRAP_INPUT) || (padState[7] & FIELD_EDIT_WRAP_INPUT)) && channelValue == 0xFF) {
        channelValue = 0;
    } else if (padState[5] & FIELD_EDIT_CLAMP_INPUT) {
        channelValue += FIELD_EDIT_COLOR_COARSE_STEP;
        if (channelValue >= 0x100) {
            channelValue = 0xFF;
        }
    } else if (((padState[4] & FIELD_EDIT_WRAP_INPUT) || (padState[6] & FIELD_EDIT_WRAP_INPUT)) && channelValue == 0) {
        channelValue = 0xFF;
    } else if (padState[4] & FIELD_EDIT_CLAMP_INPUT) {
        channelValue -= FIELD_EDIT_COLOR_COARSE_STEP;
        if (channelValue < 0) {
            channelValue = 0;
        }
    } else if (padState[7] & FIELD_EDIT_CLAMP_INPUT) {
        channelValue += 1;
        if (channelValue >= 0x100) {
            channelValue = 0xFF;
        }
    } else if (padState[6] & FIELD_EDIT_CLAMP_INPUT) {
        channelValue -= 1;
        if (channelValue < 0) {
            channelValue = 0;
        }
    } else {
        return 0;
    }
    switch (channel) {
    case 0:
        updatedColor = (originalColor & 0xFFFFFF00) | channelValue;
        break;
    case 1:
        updatedColor = (originalColor & 0xFFFF00FF) | (channelValue << 8);
        break;
    case 2:
        updatedColor = (originalColor & 0xFF00FFFF) | (channelValue << 16);
        break;
    default:
        updatedColor = (originalColor & 0x00FFFFFF) | (channelValue << 24);
        break;
    }
    *color = updatedColor;
    return updatedColor != originalColor;
}

void fldStepColorChannelByCurrentPad(u32 *color, s32 channel) {
    fldStepColorChannelByPad(color, channel, D_00324530);
}

/* Emit one whole-value character and two fractional digits after rounding.
 * At whole >= 10 only the fractional digits become 9; whole is not clamped. */
void fldFormatSecondsText(char *text, f32 displayValue) {
    s32 whole;
    s32 tenths;
    s32 hundredths;

    displayValue += 0.005f;
    whole = (s32)displayValue;
    if (whole >= 10) {
        tenths = 9;
        hundredths = 9;
    } else {
        displayValue -= (f32)whole;
        displayValue *= 10.0f;
        tenths = (s32)displayValue;
        displayValue -= (f32)tenths;
        hundredths = (s32)(displayValue * 10.0f);
    }
    text[0] = whole + '0';
    text[1] = '.';
    text[2] = tenths + '0';
    text[3] = hundredths + '0';
    text[4] = 0;
}

extern u64 *sdfCreateBorderedRectanglePacket(s32, s32, s32, s32, s32, u32, u32);

extern void *sdfCreateFormattedSifCommand(s32, s32, s32, s32, const char *, ...);

extern u8 D_0032B0A0[];

extern char *D_0032B0A8[];

/* Draw three RGB value rows and their clamped colour preview. */
void fldDrawRgbEditor(void *packetList, s32 x, s32 y, s32 selected, f32 *values) {
    char text[16];
    u32 color = 0x80000000;
    s32 i;
    s32 style;

    if (values != NULL) {
        color = 0;
        for (i = 0; i != 3; i++) {
            s32 channel = values[i] * 128.0f;
            if (channel > 0) {
                if (channel >= 256) {
                    channel = 255;
                }
                color |= channel << (i * 8);
            }
        }
        color |= 0x80000000;
    }
    sdfAppendPacket(packetList, (u32)sdfCreateBorderedRectanglePacket(x + 0x510, y + 0x18,
                     0xFF0080, 0x240, 0xF0, color, 0x40806020));
    for (i = 0; i != 3; i++) {
        sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(x, y,
                        0xFF0080, D_0032B0A0[i], D_0032B0A8[i]));
        style = selected == i ? 6 : 0;
        if (values == NULL) {
            text[0] = '-';
            text[1] = '-';
            text[2] = '-';
            text[3] = '-';
            text[4] = 0;
        } else {
            fldFormatSecondsText(text, values[i]);
        }
        sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(x + 0x180,
                        y, 0xFF0080, style, text));
        y += 0x60;
    }
}

extern char D_003BAAE0[];

extern char D_003BAAE8[];

extern char D_003BAAF0[];

extern char D_003BAAF8[];

extern char D_003BAB00[];

void fldDrawPackedRgbEditor(void *packetList, s32 x, s32 y, s32 selected,
                   u32 color, s32 showNormalized) {
    s32 i;
    s32 style;
    s32 channel;
    const char *label;

    color = (color & 0xFFFFFF) | 0x80000000;
    sdfAppendPacket(packetList, (u32)sdfCreateBorderedRectanglePacket(x + 0x510, y + 0x18,
                    0xFF0080, 0x240, 0xF0, color, 0x40806020));
    for (i = 0; i != 3; i++) {
        switch (i) {
        case 0:
            style = 2;
            label = D_003BAAF0;
            channel = color & 0xFF;
            break;
        case 1:
            style = 4;
            label = D_003BAAE8;
            channel = (color >> 8) & 0xFF;
            break;
        default:
            style = 5;
            label = D_003BAAE0;
            channel = (color >> 16) & 0xFF;
            break;
        }
        sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(
                        x, y, 0xFF0080, style, label));
        sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(
                        x + 0x180, y, 0xFF0080, i == selected ? 6 : 0,
                        D_003BAAF8, channel));
        if (showNormalized != 0) {
            sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(
                            x, y + 0x180, 0xFF0080, style, label));
            sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(
                            x + 0x240, y + 0x180, 0xFF0080, 0,
                            D_003BAB00, channel * (1.0f / 255.0f)));
        }
        y += 0x60;
    }
}

u32 fldIsUpdateInhibited(void) {
    return 0;
}



extern void fldSetGsAlphaBlendEquation(s32);

extern void fldSelectDisplayBuffer(u32);

extern void fldSubmitGsTriangle(s32 color0, s32 color1, s32 color2,
                                f32 x0, f32 y0, f32 z0,
                                f32 x1, f32 y1, f32 z1,
                                f32 x2, f32 y2, f32 z2);

/* Filled disc in the field; radius shrinks in the close-up areas. */
void fldDrawShadowDisc(f32 x, f32 y, f32 z, f32 radius, s32 fade) {
    s32 color;
    s32 angle;
    s32 prev;
    s32 i;
    f32 a;
    f32 b;
    f32 sinA;
    f32 cosA;
    f32 sinB;
    f32 cosB;

    if ((u32)(fldAreaState.area - 0xC9) < 0x12B) {
        y = 0.0f;
    } else {
        radius *= 0.7f;
    }
    if (fade == 0) {
        return;
    }
    if ((u32)(fldAreaState.area - 0xC9) < 0x12B) {
        fldSelectDisplayBuffer(0x20);
    } else {
        fldSelectDisplayBuffer(0x39);
    }
    fldSetGsAlphaBlendEquation(5);
    fade = fade / 2;
    color = (fade * 7 * 16 / 128) & 0xFF;
    if (color > 0x70) {
        color = 0x70;
    }
    color = 0x80 - color;
    if (color >= 0x80) {
        color = 0x7F;
    }
    if (color < 0x10) {
        color = 0x10;
    }
    color = (color << 24) | 0x10101;
    for (i = 0; i < 360; i += 10) {
        angle = (i + 360) % 360;
        prev = (i + 350) % 360;
        a = (f32)angle * 3.14f / 180.0f;
        sinA = sdfSinPoly(a) * radius;
        cosA = sdfCos(a) * radius;
        b = (f32)prev * 3.14f / 180.0f;
        sinB = sdfSinPoly(b) * radius;
        cosB = sdfCos(b) * radius;
        fldSubmitGsTriangle(color, 0x80101010, 0x80101010,
                            x + 0.0f, y - 2.0f, z + 0.0f,
                            x + sinA, y - 2.0f, z + cosA,
                            x + sinB, y - 2.0f, z + cosB);
    }
    fldSetGsAlphaBlendEquation(0);
}

extern u8 fldGetCampSceneControlMode(void);

extern void fldSubmitFrameQuad(s32, s32, s32, s32, s32, s32, s32, s32);

extern void fldSetGsAlphaBlendEquation(s32);

extern void fldDrawCameraColorScrollingGradient(void);

extern void fldDrawSkyGradient(void);

extern void fldSubmitTexturedOverlaySprite(void);

extern u32 D_0032E570[];

extern void fldSelectDisplayBuffer(u32);

s32 fldTestDrawUpdate(KwlnTask *task) {
    if (fldGetEncounterRuntimeResult() != 0) {
        return 0;
    }
    if (fldGetCampSceneControlMode() == 1) {
        return 0;
    }
    fldSelectDisplayBuffer(0x53);
    fldSubmitFrameQuad(1, 0, 0x81, 3, 0, 0, 1, 1);
    fldSetGsAlphaBlendEquation(0);
    fldDrawCameraColorScrollingGradient();
    fldDrawSkyGradient();
    if (D_0032E570[9] != 0) {
        if (fldGetSceneReadyFlag() == 0) {
            fldSelectDisplayBuffer(0x53);
        } else {
            fldSelectDisplayBuffer(0x5E);
        }
        fldSubmitTexturedOverlaySprite();
    }
    fldSelectDisplayBuffer(0x27);
    fldSubmitFrameQuad(1, 5, 0x80, 1, 0, 0, 1, 2);
    fldSetGsAlphaBlendEquation(0);
    fldSelectDisplayBuffer(0x39);
    fldSubmitFrameQuad(1, 5, 0x80, 1, 0, 0, 1, 2);
    fldSetGsAlphaBlendEquation(0);
    return 0;
}

void fldTestDrawCreate(void) {
    dds3StartProcess(D_0039FA00, 0x2AF8, 0, 0, fldTestDrawUpdate, 0, 0);
}

void fldTestDrawDestroy(void) {
    dds3KillProcessName(D_0039FA00, 1);
}

extern void frSetFontDrawOT(u32);
extern void fldDrawConvertedTextGlyph(s32 x, s32 y, u32 first, const u8 *second);
extern char *strcat(char *, const char *);
extern char D_0032C840[13][4];

static inline char *fldNumberGlyphText(s32 index) {
    char *table = (char *)D_0032C840;

    return table + index * 4;
}

void fldDrawPercentValue(s32 x, s32 y, s32 value) {
    char text[32];
    s32 digit;

    frSetFontDrawOT(0x54);
    if (value > 1000) {
        s32 halfY = y / 2;
        fldDrawConvertedTextGlyph(x, halfY, 0xA09DC380, (const u8 *)fldNumberGlyphText(12));
        return;
    }
    text[0] = 0;
    if (value < 0) {
        strcat(text, fldNumberGlyphText(10));
        value = -value;
    }
    /* One-pass block (CONTRIBUTING "A one-pass block inside a function").
     * cc1probe (compile with the block removed vs. kept, rtl.14.combine/
     * rtl.19.lreg, sandbox_H2/f1/ca and cb, hooked == unhooked): the block adds
     * NOTE_INSN_LOOP_BEG/CONT/END around the division, which gives `digit`
     * loop-depth weight. Without it `digit` is tied to the divide result and
     * loses its own register (retail keeps `move $4,$3` after mflo and
     * `move $4,$16` after mfhi); 39 of 82 words differ. */
    do {
        digit = value / 10;
    } while (0);
    if (digit > 0 && digit < 10) {
        strcat(text, fldNumberGlyphText(digit));
    }
    digit = value % 10;
    if (digit < 10) {
        strcat(text, fldNumberGlyphText(digit));
    }
    x += 0x10;
    strcat(text, fldNumberGlyphText(11));
    fldDrawConvertedTextGlyph(x, y / 2, 0xA09DC380, (const u8 *)text);
}

void fldConfigureCameraMoveTracking(s32 start, s32 end) {
    s32 first;
    s32 endEntry;

    first = fldFindTableEntry(start);
    D_003BAB20 = 0;
    D_003BAB0C = first;
    if (first > 1000) {
        D_003BAB08 = first % 1000;
    } else {
        D_003BAB08 = first;
    }
    if (first > 1000) {
        D_003BAB10 = first;
        D_003BAB0C = first % 1000;
    } else {
        D_003BAB10 = -999;
    }

    endEntry = fldFindTableEntry(end);
    D_003BAB14 = endEntry;
    if (endEntry > 1000) {
        D_003BAB18 = endEntry;
        D_003BAB14 = endEntry % 1000;
    } else {
        D_003BAB18 = -999;
    }
    if (start == end) {
        D_003BAB1C = -999;
    } else {
        D_003BAB1C = 0;
    }
    if (D_003BAB08 != -999 && D_003BAB1C != -999) {
        D_003BAB1C++;
        if (D_003BAB0C < D_003BAB14) {
            fldSetCameraMoveMode(1);
        } else {
            fldSetCameraMoveMode(-1);
        }
    }
}

/* Clear the move mode and invalidate all six stored tracking values. */
void fldResetCameraMoveTracking(void) {
    D_003BAB20 = 0;
    D_003BAB08 = FIELD_CAMERA_TRACKING_UNSET;
    D_003BAB0C = FIELD_CAMERA_TRACKING_UNSET;
    D_003BAB10 = FIELD_CAMERA_TRACKING_UNSET;
    D_003BAB14 = FIELD_CAMERA_TRACKING_UNSET;
    D_003BAB18 = FIELD_CAMERA_TRACKING_UNSET;
    D_003BAB1C = FIELD_CAMERA_TRACKING_UNSET;
    fldClearCameraMoveMode();
}

/* Return 2 for pending mode, 1 when tracking requests a move, or 0 otherwise. */
s32 fldEvaluateCameraMoveTracking(void) {
    if (D_003BAB20 == 1) {
        return 2;
    }
    if (D_003BAB1C != FIELD_CAMERA_TRACKING_UNSET && D_003BAB08 == D_003BAB14) {
        if (D_003BAB0C < D_003BAB08) {
            fldSetCameraMoveMode(2);
        } else {
            fldSetCameraMoveMode(-2);
        }
        return 1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FA00);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011EA10);


extern void fldSelectDisplayBuffer(u32);

extern void fldSubmitBackgroundResourcePacket(void);

extern void fldSubmitBackgroundDescriptorPacket(void);

extern s32 D_0032E3C0[];

enum {
    FIELD_BACKGROUND_REQUIRED_OBJECT_FLAG = 0x200,
    FIELD_BACKGROUND_EXCLUDED_OBJECT_FLAG = 1
};

/* Submit both background packets when a world-list-5 object passes the flag checks.
 * The whole value chain is visited and destroyed before the scene override. */
void fldSubmitVisibleWorldBackground(void) {
    s32 hasEligibleBackground = 0;
    WorldIndexNode *objectChain;
    EffWorldNode *worldObject;

    objectChain = dds3CopyWorldListToValueChain(dds3GetWorldSecondaryObject(), 5);
    if (objectChain != 0) {
        if (dds3ResetObjectValueCursor((WorldValueIndices *)objectChain) != 0) {
            do {
                worldObject = (EffWorldNode *)dds3ReadIndexedWorldObjectWord((WorldValueIndices *)objectChain);
                if (dds3ObjectBaseCheckFlag(worldObject, FIELD_BACKGROUND_REQUIRED_OBJECT_FLAG) != 0) {
                    if (dds3ObjectBaseCheckFlag(worldObject, FIELD_BACKGROUND_EXCLUDED_OBJECT_FLAG) == 0) {
                        hasEligibleBackground = 1;
                    }
                }
            } while (dds3AdvanceObjectValueCursor((WorldValueIndices *)objectChain) != 0);
        }
        dds3DestroyWorldIndexNode(objectChain);
    }
    if (D_0032E3C0[0] == 1) {
        hasEligibleBackground = 0;
    }
    if (hasEligibleBackground != 0) {
        fldSelectDisplayBuffer(0x24);
        fldSubmitBackgroundResourcePacket();
        fldSelectDisplayBuffer(0x26);
        fldSubmitBackgroundDescriptorPacket();
    }
}

#include "fld_shadow_strip.h"
#include "fld_area_work.h"
#include "pcp_vu0.h"
#include "mdl.h"
#include "eff_object.h"
#include "evt_unit.h"
#include "sdf_primitive.h"
#include "sdf_packet_append.h"
#include "sdf_packet_list.h"
#include "sdf.h"

/* All declarations below refer to the existing primary runtime owners. */
extern EvtUnit *effObjGetTransitionWork(EffWorldNode *);
extern void *dds3GetWorldObject(void);
extern EffWorldNode *dds3FindWorldObjectNodeByKey(EffWorldNode *, u32, s32);
extern s32 sdfLoadMapRecordPositionVector(SdfModel *, s32);
extern void fldCalcTrianglePlane(f32 *, f32 *, f32 *, f32 *, f32 *);
extern void fldSubmitGsQuadTagged(s32, s32, s32, s32, u32, u32, u32, u32);
extern void fldSubmitGsRect(s32, s32, s32, s32, u32, u32, u32, u32);
extern void fldSubmitPrimaryFramePacket(void);
extern void fldSubmitAlternateFramePacket(void);
extern void fldPrepareProjectionMatrix(void);
extern void *sdfBuildPrimitiveAttributeVifPacket(SdfPrimitiveRequest *);
extern SdfPoolNode kwlnDrawSurfaces[];
extern u32 fldDisplayRow;
extern u32 D_003BACEC;
extern s32 D_003BAB24, D_003BAB28, D_003BAB2C, D_003BAB30;
extern f32 D_003C2E90[4];
extern f32 D_003C2EA0[50][3][4];
extern s8 D_003C3800[26][26][2];
extern f32 D_003C3D50[26][26][4];
extern f32 D_003C6790[26][26][4];
extern f32 sdfViewEyeVector[4], sdfViewTargetVector[4];
extern SdfLightSources D_00324770;

extern f32 D_00330670[16];
extern f32 D_003246B0[16];

extern FldShadowStrip D_0032C880;
extern FldShadowStrip D_0032C8C0;
extern const f32 D_0039FA28[18];

/* Packed XYZ geometry, followed by triples of signed vertex indices. */
extern f32 D_0032B0C0[][3], D_0032B438[][3], D_0032B528[][3];
extern f32 D_0032B640[][3], D_0032B778[][3], D_0032B820[][3];
extern f32 D_0032B938[][3], D_0032BA70[][3], D_0032BB88[][3];
extern f32 D_0032BC88[][3], D_0032BD78[][3], D_0032BE90[][3];
extern f32 D_0032BF90[][3], D_0032C080[][3], D_0032C180[][3];
extern f32 D_0032C280[][3], D_0032C380[][3], D_0032C480[][3];
extern f32 D_0032C598[][3], D_0032C6B0[][3];
extern s32 D_0032B1F8[][3], D_0032B498[][3], D_0032B598[][3];
extern s32 D_0032B6B8[][3], D_0032B7C0[][3], D_0032B890[][3];
extern s32 D_0032B9B0[][3], D_0032BAE0[][3], D_0032BBF8[][3];
extern s32 D_0032BCE8[][3], D_0032BDE8[][3], D_0032BF00[][3];
extern s32 D_0032BFF0[][3], D_0032C0F0[][3], D_0032C1F0[][3];
extern s32 D_0032C2F0[][3], D_0032C3F0[][3], D_0032C4F0[][3];
extern s32 D_0032C608[][3], D_0032C750[][3];


/* Submits the accumulated strip request and starts an empty one. */
static inline void fldFlushShadowStrip(SdfListHead *list, SdfPrimitiveRequest *request) {
    sdfAppendPacket(list, (u32)sdfBuildPrimitiveAttributeVifPacket(request));
    request->stripWordCount = 0;
    request->vertexCount = 0;
}

static inline SdfPoolNode *fldGetDisplaySurfaceBase(void) {
    return kwlnDrawSurfaces;
}

/* Appends a finished packet list to the active display surface. */
static inline void fldAppendToDisplaySurface(SdfListHead *list) {
    SdfPoolNode *surface = &fldGetDisplaySurfaceBase()[fldDisplayRow];

    surface->append(surface, list);
}

extern void *memcpy(void *, const void *, unsigned int);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FA28);

void func_0011ECC8(EffWorldNode *object) {
    f32 anchorPosition[4];
    f32 edgeStart[4];
    f32 edgeEnd[4];
    f32 extrudedEnd[4];
    f32 extrudedStart[4];
    f32 projectedStart[4];
    f32 projectedEnd[4];
    f32 projectedExtrudedEnd[4];
    f32 projectedExtrudedStart[4];
    f32 firstVertex[4];
    f32 secondVertex[4];
    f32 thirdVertex[4];
    f32 extrusion[4];
    f32 slotExtrusion[18][4];
    f32 viewDirection[4];
    SdfPrimitiveRequest quadRequest;
    /* Native buffer spans sp+0x230..0x630 in both twins. The two-word
     * quad strip flushes at eight quads, before this sixteen-quad capacity. */
    f32 quadVertices[16 * 4][4];
    SdfPrimitiveRequest triangleRequest;
    f32 triangleVertices[16 * 3][4];
    FldShadowStrip triangleStrip;
    FldShadowStrip quadStrip;
    f32 extrusionLengths[18];
    /* The plane workspace occupies sp+0xA30..0xA70 in both twins. */
    f32 planeVertices[4][4];
    f32 plane[4];
    s32 slot;
    SdfModel *model;
    ObjBase *base;
    s32 vertexCount;
    EffectObjectData *data;
    EvtUnit *transition;
    EffWorldNode *target;
    SdfDrawNode *drawNode;
    f32 (*vertices)[3];
    s32 (*triangles)[3];
    s32 *triangle;
    s32 triangleCount;
    s32 face;
    s32 first;
    s32 second;
    s32 minimum;
    s32 maximum;
    s32 reversed;
    s32 pass;
    s32 i;
    s32 j;
    s32 edgeFrom;
    s32 alpha;
    SdfListHead *quadList;
    SdfListHead *triangleList;
    f32 length = 300.0f;
    f32 opacity;
    f32 horizontalDistance;
    f32 verticalDistance;
    f32 facing;
    f32 minimumX;
    f32 maximumX;
    f32 minimumY;
    f32 maximumY;

    memset(anchorPosition, 0, sizeof(anchorPosition));
    anchorPosition[3] = 1.0f;
    memset(edgeStart, 0, sizeof(edgeStart));
    edgeStart[3] = 1.0f;
    memset(edgeEnd, 0, sizeof(edgeEnd));
    edgeEnd[3] = 1.0f;
    memset(extrudedEnd, 0, sizeof(extrudedEnd));
    extrudedEnd[3] = 1.0f;
    memset(extrudedStart, 0, sizeof(extrudedStart));
    extrudedStart[3] = 1.0f;
    memset(firstVertex, 0, sizeof(firstVertex));
    firstVertex[3] = 1.0f;
    memset(secondVertex, 0, sizeof(secondVertex));
    secondVertex[3] = 1.0f;
    memset(thirdVertex, 0, sizeof(thirdVertex));
    thirdVertex[3] = 1.0f;
    memset(viewDirection, 0, sizeof(viewDirection));
    viewDirection[3] = 1.0f;
    memcpy(extrusionLengths, D_0039FA28, sizeof(extrusionLengths));
    triangleStrip = D_0032C8C0;
    quadStrip = D_0032C880;
    opacity = 100.0f;
    base = dds3GetObjectOwnedHandle(object);
    model = ((MdlCtx *)dds3ObjectBaseGetModelHandle(object))->inner;
    if (dds3ObjectBaseCheckFlag(object, 0x4000)) {
        transition = effObjGetTransitionWork(object);
        D_003BAB28 = transition->shadowLevel;
        D_003BAB2C = 1;
    } else if (D_003BAB2C == 1) {
        D_003BAB28 = 16;
        D_003BAB2C = 0;
    }
    firstVertex[0] = 0.0f;
    firstVertex[1] = 0.0f;
    firstVertex[2] = 0.0f;
    firstVertex[3] = 1.0f;
    secondVertex[0] = 0.0f;
    secondVertex[1] = 0.0f;
    secondVertex[2] = 0.0f;
    secondVertex[3] = 1.0f;
    fldSelectDisplayBuffer(0x25);
    if (fldAreaState.area >= 500) {
        transition = effObjGetTransitionWork(object);
        if (transition != NULL) {
            if (transition->flags & 0x40000) {
                D_003C2E90[0] = transition->vec10[0];
                D_003C2E90[1] = 2.0f;
                D_003C2E90[2] = transition->vec10[2];
                length = 200.0f;
            } else {
                D_003C2E90[0] = D_00324770[0][1][0];
                D_003C2E90[1] = 2.0f;
                D_003C2E90[2] = D_00324770[0][1][2];
            }
        }
    } else {
        data = object->data;
        if (data->activeId != -1) {
            if (D_003BAB30 == 0) {
                if (D_003BAB28 > 20) {
                    D_003BAB28 -= 2;
                } else {
                    D_003BAB30 = 1;
                    D_003BAB28 = 8;
                }
                length = 200.0f;
            } else {
                target = dds3FindWorldObjectNodeByKey(dds3GetWorldObject(), data->activeId, 9);
                transition = effObjGetTransitionWork(object);
                if (sdfLoadMapRecordPositionVector(transition->owner->inner, 0)) {
                    VU0_STORE_VF_UNCLOBBERED(vf10, anchorPosition);
                    D_003C2E90[0] = anchorPosition[0] - target->inner->position[0];
                    D_003C2E90[1] = anchorPosition[1] - target->inner->position[1];
                    D_003C2E90[2] = anchorPosition[2] - target->inner->position[2];
                } else {
                    D_003C2E90[0] = fldAreaState.x - target->inner->position[0];
                    /* Retail 0x0011F134..0x0011F15C uses field Z for both Y and Z. */
                    D_003C2E90[1] = fldAreaState.z - target->inner->position[1];
                    D_003C2E90[2] = fldAreaState.z - target->inner->position[2];
                }
                horizontalDistance = fsqrtf(D_003C2E90[0] * D_003C2E90[0] + D_003C2E90[2] * D_003C2E90[2]);
                if (D_003C2E90[1] < 0.0f) {
                    verticalDistance = -D_003C2E90[1];
                } else {
                    verticalDistance = D_003C2E90[1];
                }
                if (verticalDistance < 10.0f) verticalDistance = 10.0f;
                if (verticalDistance > 200.0f) verticalDistance = 200.0f;
                verticalDistance -= 10.0f;
                verticalDistance /= 25.0f;
                if (horizontalDistance < 0.01f) {
                    D_003C2E90[0] = 0.1f;
                    D_003C2E90[1] = 2.0f;
                    D_003C2E90[2] = 0.1f;
                } else {
                    D_003C2E90[0] /= horizontalDistance;
                    D_003C2E90[1] = verticalDistance + 1.0f;
                    D_003C2E90[2] /= horizontalDistance;
                }
                if (horizontalDistance < 100.0f) horizontalDistance = 100.0f;
                if (horizontalDistance > 300.0f) horizontalDistance = 300.0f;
                horizontalDistance -= 100.0f;
                horizontalDistance /= 2.6f;
                opacity = 100.0f - horizontalDistance;
                if (D_003BAB28 < 48) D_003BAB28 += 2;
                length = 200.0f;
            }
        } else if (D_003BAB30 == 0) {
            if (fldAreaState.area < 200) {
                D_003C2E90[0] = D_00330670[4];
                D_003C2E90[1] = D_00330670[5];
                D_003C2E90[2] = D_00330670[6];
            } else {
                D_003C2E90[0] = D_003246B0[4];
                D_003C2E90[1] = D_003246B0[5];
                D_003C2E90[2] = D_003246B0[6];
            }
            horizontalDistance = fsqrtf(D_003C2E90[0] * D_003C2E90[0] + D_003C2E90[2] * D_003C2E90[2]);
            if (horizontalDistance < 0.01f) {
                D_003C2E90[0] = 0.2f;
                D_003C2E90[1] = 2.0f;
                D_003C2E90[2] = 0.1f;
            } else {
                D_003C2E90[0] /= horizontalDistance;
                D_003C2E90[1] = 2.0f;
                D_003C2E90[2] /= horizontalDistance;
            }
            D_003C2E90[0] += 0.1f;
            if (D_003BAB28 > 20) D_003BAB28 -= 2;
            if (D_003BAB28 < 20) D_003BAB28 += 2;
            length = 200.0f;
        } else {
            if (D_003BAB28 > 20) {
                D_003BAB28 /= 3;
            } else {
                D_003BAB30 = 0;
                D_003BAB28 = 6;
            }
            length = 200.0f;
        }
    }
    D_003C2E90[3] = 1.0f;
    VU0_LOAD_VF_PTR(vf10, D_003C2E90);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF_UNCLOBBERED(vf10, D_003C2E90);
    /* Retail still stores this full-length vector at11F43C..11F44C;
     * the later per-slot geometry uses slotExtrusion instead. */
    extrusion[0] = D_003C2E90[0] * length;
    extrusion[1] = D_003C2E90[1] * length;
    extrusion[2] = D_003C2E90[2] * length;
    extrusion[3] = 1.0f;
    VU0_LOAD_VF_PTR(vf10, D_003C2E90);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF_UNCLOBBERED(vf10, D_003C2E90);
    for (slot = 0; slot < base->resourceSlotCount; slot++) {
        length = extrusionLengths[slot];
        slotExtrusion[slot][0] = D_003C2E90[0] * length;
        slotExtrusion[slot][1] = D_003C2E90[1] * length;
        slotExtrusion[slot][2] = D_003C2E90[2] * length;
        slotExtrusion[slot][3] = 1.0f;
    }
    viewDirection[0] = sdfViewEyeVector[0] - sdfViewTargetVector[0];
    viewDirection[1] = sdfViewEyeVector[1] - sdfViewTargetVector[1];
    viewDirection[2] = sdfViewEyeVector[2] - sdfViewTargetVector[2];
    viewDirection[3] = 1.0f;
    VU0_LOAD_VF_PTR(vf10, viewDirection);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF_UNCLOBBERED(vf10, viewDirection);
    fldSubmitFrameQuad(1, 0, 0x80, 1, 0, 0, 1, 1);
    fldSetGsAlphaBlendEquation(1);
    fldSubmitGsQuadTagged(0, 0, 512, 224, 0, 0, 0, 0);

    for (slot = 0; slot < base->resourceSlotCount; slot++) {
        i = base->resourceSlots[slot];
        if (i < 0) continue;
        drawNode = ((SdfDrawNode **)model->list->buffer)[i];
        D_003BAB24 = 0;
        switch (slot) {
        case 0:
            if (!(base->resourceSlotFamily & 0xF000)) {
                vertices = D_0032C180; triangles = D_0032C1F0;
                triangleCount = 12; vertexCount = 9;
            } else {
                vertices = D_0032C080; triangles = D_0032C0F0;
                triangleCount = 12; vertexCount = 9;
            }
            break;
        case 1:
            if (!(base->resourceSlotFamily & 0xF000)) {
                vertices = D_0032C380; triangles = D_0032C3F0;
                triangleCount = 12; vertexCount = 9;
            } else {
                vertices = D_0032C280; triangles = D_0032C2F0;
                triangleCount = 12; vertexCount = 9;
            }
            break;
        case 2:
            if (!(base->resourceSlotFamily & 0xF000)) {
                vertices = D_0032C598; triangles = D_0032C608;
                triangleCount = 14; vertexCount = 9;
            } else {
                vertices = D_0032C480; triangles = D_0032C4F0;
                triangleCount = 14; vertexCount = 9;
            }
            break;
        case 3:
            if (fldAreaState.area >= 500) continue;
            vertices = D_0032B438; triangles = D_0032B498;
            triangleCount = 12; vertexCount = 8;
            break;
        case 4:
            vertices = D_0032B0C0; triangles = D_0032B1F8;
            triangleCount = 48; vertexCount = 26;
            break;
        case 5:
            vertices = D_0032B820; triangles = D_0032B890;
            triangleCount = 14; vertexCount = 9;
            break;
        case 6:
            vertices = D_0032B938; triangles = D_0032B9B0;
            triangleCount = 16; vertexCount = 10;
            break;
        case 8:
            vertices = D_0032B528; triangles = D_0032B598;
            triangleCount = 14; vertexCount = 9;
            break;
        case 9:
            vertices = D_0032B640; triangles = D_0032B6B8;
            triangleCount = 16; vertexCount = 10;
            break;
        case 7:
        case 10:
            vertices = D_0032B778; triangles = D_0032B7C0;
            triangleCount = 8; vertexCount = 6;
            break;
        case 11:
            vertices = D_0032BD78; triangles = D_0032BDE8;
            triangleCount = 14; vertexCount = 9;
            break;
        case 12:
            vertices = D_0032BE90; triangles = D_0032BF00;
            triangleCount = 12; vertexCount = 9;
            break;
        case 13:
            vertices = D_0032BF90; triangles = D_0032BFF0;
            triangleCount = 12; vertexCount = 8;
            break;
        case 14:
            vertices = D_0032BA70; triangles = D_0032BAE0;
            triangleCount = 14; vertexCount = 9;
            break;
        case 15:
            vertices = D_0032BB88; triangles = D_0032BBF8;
            triangleCount = 12; vertexCount = 9;
            break;
        case 16:
            vertices = D_0032BC88; triangles = D_0032BCE8;
            triangleCount = 12; vertexCount = 8;
            break;
        case 17:
            vertices = D_0032C6B0; triangles = D_0032C750;
            triangleCount = 20; vertexCount = 13;
            break;
        default:
            continue;
        }
        for (edgeFrom = 0; edgeFrom < vertexCount; edgeFrom++) {
            for (j = 0; j < vertexCount; j++) {
                D_003C3800[edgeFrom][j][0] = 0;
            }
        }
        for (i = 0; i < triangleCount; i++) {
            VU0_LOAD_MATRIX(drawNode->worldMatrix);
            triangle = triangles[i];
            firstVertex[0] = vertices[triangle[0]][0];
            firstVertex[1] = vertices[triangle[0]][1];
            firstVertex[2] = vertices[triangle[0]][2];
            VU0_LOAD_VF_PTR(vf10, firstVertex);
            VU0_APPLY_MATRIX(vf10, vf10);
            VU0_STORE_VF_UNCLOBBERED(vf10, firstVertex);
            secondVertex[0] = vertices[triangle[1]][0];
            secondVertex[1] = vertices[triangle[1]][1];
            secondVertex[2] = vertices[triangle[1]][2];
            VU0_LOAD_VF_PTR(vf10, secondVertex);
            VU0_APPLY_MATRIX(vf10, vf10);
            VU0_STORE_VF_UNCLOBBERED(vf10, secondVertex);
            thirdVertex[0] = vertices[triangle[2]][0];
            thirdVertex[1] = vertices[triangle[2]][1];
            thirdVertex[2] = vertices[triangle[2]][2];
            VU0_LOAD_VF_PTR(vf10, thirdVertex);
            VU0_APPLY_MATRIX(vf10, vf10);
            VU0_STORE_VF_UNCLOBBERED(vf10, thirdVertex);
            planeVertices[0][0] = firstVertex[0];
            planeVertices[0][1] = firstVertex[1];
            planeVertices[0][2] = firstVertex[2];
            planeVertices[0][3] = firstVertex[3];
            planeVertices[1][0] = secondVertex[0];
            planeVertices[1][1] = secondVertex[1];
            planeVertices[1][2] = secondVertex[2];
            planeVertices[1][3] = secondVertex[3];
            planeVertices[2][0] = thirdVertex[0];
            planeVertices[2][1] = thirdVertex[1];
            planeVertices[2][2] = thirdVertex[2];
            planeVertices[2][3] = thirdVertex[3];
            fldCalcTrianglePlane(planeVertices[0], &plane[0], &plane[1], &plane[2], &plane[3]);
            VU0_LOAD_VF_PTR(vf10, plane);
            VU0_NORMALIZE_VF10();
            VU0_LOAD_VF_PTR(vf11, D_003C2E90);
            VU0_DOT_XYZ(facing, vf10, vf11);
            if (facing < 0.0f) {
                first = triangle[0]; second = triangle[1];
                if (first < second) {
                    minimum = first; maximum = second; reversed = 0;
                } else {
                    minimum = second; maximum = first; reversed = 1;
                }
                D_003C3800[minimum][maximum][0]++;
                D_003C3800[minimum][maximum][1] = reversed;
                if (reversed == 0) {
                    PCP_COPY_VECTOR_UNCLOBBERED(D_003C3D50[minimum][maximum], firstVertex);
                    PCP_COPY_VECTOR_UNCLOBBERED(D_003C6790[minimum][maximum], secondVertex);
                } else {
                    PCP_COPY_VECTOR_UNCLOBBERED(D_003C3D50[minimum][maximum], secondVertex);
                    PCP_COPY_VECTOR_UNCLOBBERED(D_003C6790[minimum][maximum], firstVertex);
                }
                first = triangle[1]; second = triangle[2];
                if (first < second) {
                    minimum = first; maximum = second; reversed = 0;
                } else {
                    minimum = second; maximum = first; reversed = 1;
                }
                D_003C3800[minimum][maximum][0]++;
                D_003C3800[minimum][maximum][1] = reversed;
                if (reversed == 0) {
                    PCP_COPY_VECTOR_UNCLOBBERED(D_003C3D50[minimum][maximum], secondVertex);
                    PCP_COPY_VECTOR_UNCLOBBERED(D_003C6790[minimum][maximum], thirdVertex);
                } else {
                    PCP_COPY_VECTOR_UNCLOBBERED(D_003C3D50[minimum][maximum], thirdVertex);
                    PCP_COPY_VECTOR_UNCLOBBERED(D_003C6790[minimum][maximum], secondVertex);
                }
                second = triangle[2]; first = triangle[0];
                if (second < first) {
                    minimum = second; maximum = first; reversed = 0;
                } else {
                    minimum = first; maximum = second; reversed = 1;
                }
                D_003C3800[minimum][maximum][0]++;
                D_003C3800[minimum][maximum][1] = reversed;
                if (reversed == 0) {
                    PCP_COPY_VECTOR_UNCLOBBERED(D_003C3D50[minimum][maximum], thirdVertex);
                    PCP_COPY_VECTOR_UNCLOBBERED(D_003C6790[minimum][maximum], firstVertex);
                } else {
                    PCP_COPY_VECTOR_UNCLOBBERED(D_003C3D50[minimum][maximum], firstVertex);
                    PCP_COPY_VECTOR_UNCLOBBERED(D_003C6790[minimum][maximum], thirdVertex);
                }
                PCP_COPY_VECTOR_UNCLOBBERED(D_003C2EA0[D_003BAB24][0], firstVertex);
                PCP_COPY_VECTOR_UNCLOBBERED(D_003C2EA0[D_003BAB24][1], secondVertex);
                PCP_COPY_VECTOR_UNCLOBBERED(D_003C2EA0[D_003BAB24][2], thirdVertex);
                D_003BAB24++;
            } else {
                if (fldAreaState.area >= 500) {
                    switch (slot) {
                    case 2: case 3: case 4: case 7:
                    case 10: case 11: case 12: case 13:
                    case 14: case 15: case 16: case 17:
                        D_003C2EA0[D_003BAB24][0][0] = firstVertex[0] + slotExtrusion[slot][0];
                        D_003C2EA0[D_003BAB24][0][1] = firstVertex[1] + slotExtrusion[slot][1];
                        D_003C2EA0[D_003BAB24][0][2] = firstVertex[2] + slotExtrusion[slot][2];
                        D_003C2EA0[D_003BAB24][1][0] = secondVertex[0] + slotExtrusion[slot][0];
                        D_003C2EA0[D_003BAB24][1][1] = secondVertex[1] + slotExtrusion[slot][1];
                        D_003C2EA0[D_003BAB24][1][2] = secondVertex[2] + slotExtrusion[slot][2];
                        D_003C2EA0[D_003BAB24][2][0] = thirdVertex[0] + slotExtrusion[slot][0];
                        D_003C2EA0[D_003BAB24][2][1] = thirdVertex[1] + slotExtrusion[slot][1];
                        D_003C2EA0[D_003BAB24][2][2] = thirdVertex[2] + slotExtrusion[slot][2];
                        D_003BAB24++;
                        break;
                    }
                } else {
                    D_003C2EA0[D_003BAB24][0][0] = firstVertex[0] + slotExtrusion[slot][0];
                    D_003C2EA0[D_003BAB24][0][1] = firstVertex[1] + slotExtrusion[slot][1];
                    D_003C2EA0[D_003BAB24][0][2] = firstVertex[2] + slotExtrusion[slot][2];
                    D_003C2EA0[D_003BAB24][1][0] = secondVertex[0] + slotExtrusion[slot][0];
                    D_003C2EA0[D_003BAB24][1][1] = secondVertex[1] + slotExtrusion[slot][1];
                    D_003C2EA0[D_003BAB24][1][2] = secondVertex[2] + slotExtrusion[slot][2];
                    D_003C2EA0[D_003BAB24][2][0] = thirdVertex[0] + slotExtrusion[slot][0];
                    D_003C2EA0[D_003BAB24][2][1] = thirdVertex[1] + slotExtrusion[slot][1];
                    D_003C2EA0[D_003BAB24][2][2] = thirdVertex[2] + slotExtrusion[slot][2];
                    D_003BAB24++;
                }
            }
        }
        minimumX = minimumY = 4096.0f;
        maximumX = maximumY = -4096.0f;
        fldSubmitFrameQuad(1, 0, 0x80, 1, 0, 0, 1, 3);
        fldSubmitPrimaryFramePacket();
        for (pass = 0; pass < 2; pass++) {
            quadList = (SdfListHead *)sdfAllocPacketAligned(0x20);
            sdfInitPacketList(quadList);
            sdfConsAppendClearPacket(quadList, NULL);
            sdfConsAppendAssetPacket(quadList, (void *)D_003BACEC, NULL);
            memset(&quadRequest, 0, sizeof(quadRequest));
            quadRequest.positions = quadVertices;
            quadRequest.strip = quadStrip.words;
            quadRequest.stripWordCount = 0;
            quadRequest.vertexCount = 0;
            if (pass == 0) quadRequest.color = 0x80000000;
            else quadRequest.color = 0;
            triangleList = (SdfListHead *)sdfAllocPacketAligned(0x20);
            sdfInitPacketList(triangleList);
            sdfConsAppendClearPacket(triangleList, NULL);
            sdfConsAppendAssetPacket(triangleList, (void *)D_003BACEC, NULL);
            memset(&triangleRequest, 0, sizeof(triangleRequest));
            triangleRequest.positions = triangleVertices;
            triangleRequest.strip = triangleStrip.words;
            triangleRequest.stripWordCount = 0;
            triangleRequest.vertexCount = 0;
            if (pass == 0) triangleRequest.color = 0x80000000;
            else triangleRequest.color = 0;
            for (face = 0; face < D_003BAB24; face++) {
                if (pass == 0) {
                    f32 (*output)[4] = &triangleVertices[triangleRequest.vertexCount];
                    PCP_COPY_VECTOR_UNCLOBBERED(output[0], D_003C2EA0[face][0]);
                    PCP_COPY_VECTOR_UNCLOBBERED(output[1], D_003C2EA0[face][1]);
                    PCP_COPY_VECTOR_UNCLOBBERED(output[2], D_003C2EA0[face][2]);
                    triangleRequest.stripWordCount++;
                    triangleRequest.vertexCount += 3;
                } else {
                    f32 (*output)[4] = &triangleVertices[triangleRequest.vertexCount];
                    PCP_COPY_VECTOR_UNCLOBBERED(output[0], D_003C2EA0[face][2]);
                    PCP_COPY_VECTOR_UNCLOBBERED(output[1], D_003C2EA0[face][1]);
                    PCP_COPY_VECTOR_UNCLOBBERED(output[2], D_003C2EA0[face][0]);
                    triangleRequest.stripWordCount++;
                    triangleRequest.vertexCount += 3;
                }
                if (triangleRequest.stripWordCount >= 16) {
                    fldFlushShadowStrip(triangleList, &triangleRequest);
                }
            }
            if (triangleRequest.stripWordCount != 0) {
                fldFlushShadowStrip(triangleList, &triangleRequest);
            }
            fldAppendToDisplaySurface(triangleList);
            if (pass == 0) fldPrepareProjectionMatrix();
            for (edgeFrom = 0; edgeFrom < vertexCount; edgeFrom++) {
                for (j = 0; j < vertexCount; j++) {
                    if (D_003C3800[edgeFrom][j][0] != 1) continue;
                    PCP_COPY_VECTOR_UNCLOBBERED(firstVertex, D_003C3D50[edgeFrom][j]);
                    PCP_COPY_VECTOR_UNCLOBBERED(secondVertex, D_003C6790[edgeFrom][j]);
                    PCP_COPY_VECTOR_UNCLOBBERED(edgeStart, firstVertex);
                    PCP_COPY_VECTOR_UNCLOBBERED(edgeEnd, secondVertex);
                    extrudedEnd[0] = secondVertex[0] + slotExtrusion[slot][0];
                    extrudedEnd[1] = secondVertex[1] + slotExtrusion[slot][1];
                    extrudedEnd[2] = secondVertex[2] + slotExtrusion[slot][2];
                    extrudedStart[0] = firstVertex[0] + slotExtrusion[slot][0];
                    extrudedStart[1] = firstVertex[1] + slotExtrusion[slot][1];
                    extrudedStart[2] = firstVertex[2] + slotExtrusion[slot][2];
                    if (pass == 0) {
                        VU0_LOAD_VF_PTR(vf10, edgeStart);
                        VU0_TRANSFORM_POINT(vf10, vf10);
                        VU0_PERSPECTIVE_DIVIDE_VF10();
                        VU0_APPLY_VIEWPORT(vf10, vf10, vf11, vf12);
                        VU0_STORE_VF_UNCLOBBERED(vf10, projectedStart);
                        if (projectedStart[0] < minimumX) minimumX = projectedStart[0];
                        else if (projectedStart[0] > maximumX) maximumX = projectedStart[0];
                        if (projectedStart[1] < minimumY) minimumY = projectedStart[1];
                        else if (projectedStart[1] > maximumY) maximumY = projectedStart[1];
                        VU0_LOAD_VF_PTR(vf10, edgeEnd);
                        VU0_TRANSFORM_POINT(vf10, vf10);
                        VU0_PERSPECTIVE_DIVIDE_VF10();
                        VU0_APPLY_VIEWPORT(vf10, vf10, vf11, vf12);
                        VU0_STORE_VF_UNCLOBBERED(vf10, projectedEnd);
                        if (projectedEnd[0] < minimumX) minimumX = projectedEnd[0];
                        else if (projectedEnd[0] > maximumX) maximumX = projectedEnd[0];
                        if (projectedEnd[1] < minimumY) minimumY = projectedEnd[1];
                        else if (projectedEnd[1] > maximumY) maximumY = projectedEnd[1];
                        VU0_LOAD_VF_PTR(vf10, extrudedEnd);
                        VU0_TRANSFORM_POINT(vf10, vf10);
                        VU0_PERSPECTIVE_DIVIDE_VF10();
                        VU0_APPLY_VIEWPORT(vf10, vf10, vf11, vf12);
                        VU0_STORE_VF_UNCLOBBERED(vf10, projectedExtrudedEnd);
                        if (projectedExtrudedEnd[0] < minimumX) minimumX = projectedExtrudedEnd[0];
                        else if (projectedExtrudedEnd[0] > maximumX) maximumX = projectedExtrudedEnd[0];
                        if (projectedExtrudedEnd[1] < minimumY) minimumY = projectedExtrudedEnd[1];
                        else if (projectedExtrudedEnd[1] > maximumY) maximumY = projectedExtrudedEnd[1];
                        VU0_LOAD_VF_PTR(vf10, extrudedStart);
                        VU0_TRANSFORM_POINT(vf10, vf10);
                        VU0_PERSPECTIVE_DIVIDE_VF10();
                        VU0_APPLY_VIEWPORT(vf10, vf10, vf11, vf12);
                        VU0_STORE_VF_UNCLOBBERED(vf10, projectedExtrudedStart);
                        if (projectedExtrudedStart[0] < minimumX) minimumX = projectedExtrudedStart[0];
                        else if (projectedExtrudedStart[0] > maximumX) maximumX = projectedExtrudedStart[0];
                        if (projectedExtrudedStart[1] < minimumY) minimumY = projectedExtrudedStart[1];
                        else if (projectedExtrudedStart[1] > maximumY) maximumY = projectedExtrudedStart[1];
                    }
                    {
                        if (D_003C3800[edgeFrom][j][1]) {
                            if (pass == 0) {
                                f32 (*output)[4] = &quadVertices[quadRequest.vertexCount];
                                PCP_COPY_VECTOR_UNCLOBBERED(output[0], edgeStart);
                                PCP_COPY_VECTOR_UNCLOBBERED(output[1], edgeEnd);
                                PCP_COPY_VECTOR_UNCLOBBERED(output[2], extrudedEnd);
                                PCP_COPY_VECTOR_UNCLOBBERED(output[3], extrudedStart);
                                quadRequest.stripWordCount += 2;
                                quadRequest.vertexCount += 4;
                            } else {
                                f32 (*output)[4] = &quadVertices[quadRequest.vertexCount];
                                PCP_COPY_VECTOR_UNCLOBBERED(output[0], edgeStart);
                                PCP_COPY_VECTOR_UNCLOBBERED(output[1], extrudedStart);
                                PCP_COPY_VECTOR_UNCLOBBERED(output[2], extrudedEnd);
                                PCP_COPY_VECTOR_UNCLOBBERED(output[3], edgeEnd);
                                quadRequest.stripWordCount += 2;
                                quadRequest.vertexCount += 4;
                            }
                        } else {
                            if (pass == 0) {
                                f32 (*output)[4] = &quadVertices[quadRequest.vertexCount];
                                PCP_COPY_VECTOR_UNCLOBBERED(output[0], edgeStart);
                                PCP_COPY_VECTOR_UNCLOBBERED(output[1], extrudedStart);
                                PCP_COPY_VECTOR_UNCLOBBERED(output[2], extrudedEnd);
                                PCP_COPY_VECTOR_UNCLOBBERED(output[3], edgeEnd);
                                quadRequest.stripWordCount += 2;
                                quadRequest.vertexCount += 4;
                            } else {
                                f32 (*output)[4] = &quadVertices[quadRequest.vertexCount];
                                PCP_COPY_VECTOR_UNCLOBBERED(output[0], edgeStart);
                                PCP_COPY_VECTOR_UNCLOBBERED(output[1], edgeEnd);
                                PCP_COPY_VECTOR_UNCLOBBERED(output[2], extrudedEnd);
                                PCP_COPY_VECTOR_UNCLOBBERED(output[3], extrudedStart);
                                quadRequest.stripWordCount += 2;
                                quadRequest.vertexCount += 4;
                            }
                        }
                    }
                    if (quadRequest.stripWordCount >= 15) {
                        fldFlushShadowStrip(quadList, &quadRequest);
                    }
                }
            }
            if (quadRequest.stripWordCount != 0) {
                fldFlushShadowStrip(quadList, &quadRequest);
            }
            fldAppendToDisplaySurface(quadList);
        }
        fldSubmitAlternateFramePacket();
        fldSubmitFrameQuad(1, 1, 0x80, 1, 1, 1, 1, 3);
        fldSetGsAlphaBlendEquation(0);
        alpha = (s32)((f32)D_003BAB28 * opacity / 100.0f);
        if (alpha < 0) alpha = 0;
        fldSubmitGsRect((s32)(minimumX * 16.0f), (s32)(minimumY * 16.0f),
                        (s32)(maximumX * 16.0f), (s32)(maximumY * 16.0f),
                        0, 0, 0, alpha);
    }
    fldSubmitFrameQuad(1, 5, 0x80, 3, 0, 0, 1, 2);
    fldSetGsAlphaBlendEquation(0);
}

extern u8 D_0033EC90[], D_00347C68[], D_003482A8[], D_00349030[], D_0032EF18[], D_0032EFE0[], D_0032E5C8[];
extern FldCamRow fldCameraFollowRows[];
extern FldNpcMotion D_00336A60[];

extern FldCameraSetting *fldCameraSettings;

void fldLoadFieldTables(void) {
    DevState *command = sdfDevCreateCommandState("/fld/f/bin/FLDALL.TBL");

    sdfDevQueueReadAndWait(command, D_0033EC90, 0x3D8);
    sdfDevQueueReadAndWait(command, D_0033F068, 0x3800);
    sdfDevQueueReadAndWait(command, D_00342868, 0x3800);
    sdfDevQueueReadAndWait(command, D_00346068, 0x1C00);
    sdfDevQueueReadAndWait(command, D_00347C68, 0x640);
    sdfDevQueueReadAndWait(command, D_003482A8, 0xC80);
    sdfDevQueueReadAndWait(command, D_00349030, 0x3840);
    sdfDevQueueReadAndWait(command, D_0034C8F0, 0x1000);
    sdfDevQueueReadAndWait(command, D_0032EF18, 0xC8);
    sdfDevQueueReadAndWait(command, D_0032EFE0, 0x190);
    sdfDevQueueReadAndWait(command, D_0032F510, 0x500);
    sdfDevQueueReadAndWait(command, fldCameraFollowRows, 0xC00);
    sdfDevQueueReadAndWait(command, fldCameraSettings, 0x2A0);
    sdfDevQueueReadAndWait(command, D_0032C9B0, 0x1400);
    sdfDevQueueReadAndWait(command, D_0032DDB0, 0x600);
    sdfDevQueueReadAndWait(command, D_0032E5C8, 0x880);
    sdfDevQueueReadAndWait(command, D_00336A60, 0x1200); /* 64 FldNpcMotion rows */
    sdfDevWaitThenReleaseCommandState(command);
}

extern void fldClearAllAreaFloorFlags(void);

void fldResetFieldMapState(void) {
    s32 bankIndex;
    s32 slotIndex;
    s32 valueIndex;

    for (bankIndex = 0; bankIndex < 32; bankIndex++) {
        datGameState->activationFlags[bankIndex] = 0;
    }

    for (bankIndex = 0; bankIndex < 40; bankIndex++) {
        for (slotIndex = 0; slotIndex < 64; slotIndex++) {
            datGameState->maps[bankIndex].slots[slotIndex].roomModeFlags = 0;
            datGameState->maps[bankIndex].slots[slotIndex].roomObjectModeFlags = 0;
            datGameState->maps[bankIndex].slots[slotIndex].roomSceneFlags = 0;
            datGameState->maps[bankIndex].slots[slotIndex].mapTargetFlags = 0;
            datGameState->maps[bankIndex].slots[slotIndex].alternateMapTargetFlags = 0;
            for (valueIndex = 0; valueIndex < 16; valueIndex++) {
                datGameState->maps[bankIndex].slots[slotIndex].values[valueIndex] = 0xFF;
            }
            datGameState->maps[bankIndex].slots[slotIndex].auxiliaryFlags = 0;
            datGameState->maps[bankIndex].slots[slotIndex].valueFlags = 0;
        }
    }

    fldClearAllAreaFloorFlags();
    memset(datGameState->pad159D0, 0, sizeof(datGameState->pad159D0));
}

/* The reset includes the first word of the following render region. */
#include "fld_vr_reset_view.h"

extern u32 D_0032E570[];
void fldResetFieldRuntimeState(s32 mode) {
    FldAreaWork *area = &fldAreaState;
    s32 i;

    area->attritionDistance = 100.0f;
    area->unk2C = 0x10;
    area->area = 0;
    area->floor = 0;
    area->unk18 = 0;
    area->cachedArea = 0;
    area->reuseAreaResources = 0;
    area->consumedFlags = 0;
    area->mode = 0;
    area->unk54 = 0;
    area->cameraTargetGraceTicks = 0;
    area->sceneMode = 0;
    area->sceneState = 0;
    area->unk88 = 0;
    area->unk8C = 0;
    area->unk90 = -1;
    area->unk94 = -1;
    area->unk98 = -1;
    area->unkC4 = -1;
    area->unkC8 = -1;
    area->overlayMode = 1;
    area->overlayCounter = 0;
    area->unkD4 = 1;
    area->unkD8 = -1;
    area->rowIdx = 0;
    area->sequenceCode = 1;
    area->unk5C = 0.0f;
    area->unk60 = 0.0f;
    area->negatedAngle = 0.0f;
    area->cameraTargetAngle = 0.0f;
    area->dist = 0.0f;
    memset(area->pad124, 0, sizeof(area->pad124));
    area->terrainAttritionKind = 0;
    area->sceneCommand = 0;
    area->colorEffectSuppressed = 0;
    area->defaultEncounterId = 0;
    area->unkE0 = 0;
    area->encounterMode = 0;
    area->transitionCount = 0;
    area->frameTravelDistance = 0.0f;
    area->titleFade = 0;
    area->playerModelAlternate = 0;
    area->cameraTiltActive = 0;
    area->unkB4 = 0;
    area->cameraTilt = 0.0f;
    area->unk100 = 0;
    area->transitionMode = 0;
    area->collectedCount = 0;
    area->score = 0;
    memset(&area->unk10C, 0, sizeof(area->unk10C));
    area->pendingSceneRequest = 0;
    area->deferredExit = 0;
    area->nextFloor = area->nextArea = -1;
    area->commandEnabled = 0;
    area->sceneBgmSuppressed = 0;
    area->skipFade = 0;
    area->unk138 = 0;
    area->playerModelVariant = 0;
    area->x = 0.0f;
    area->y = 0.0f;
    area->z = 0.0f;
    area->previousX = 0.0f;
    area->previousY = 0.0f;
    area->previousZ = 0.0f;
    area->targetX = 0.0f;
    area->targetY = 0.0f;
    area->targetZ = 0.0f;
    area->positionPending = 0;
    area->positionMode = 0;
    area->unk17C = 0;
    area->verticalStepDirection = 0;
    area->angle = 0.0f;
    area->previousCameraModelFrame = 0.0f;
    area->unk170 = 0.0f;
    area->unk174 = 0.0f;
    area->targetAngle = 180.0f;
    area->unk184 = 0;
    D_0032E570[0] = 0;
    D_0032E570[1] = 0;
    D_0032E570[2] = 0;
    D_0032E570[3] = 0;
    D_0032E570[4] = 0;
    D_0032E570[5] = 0;
    D_0032E570[6] = 0;
    D_0032E570[7] = 0;
    D_0032E570[8] = 0;
    D_0032E570[9] = 1;
    D_0032E570[10] = 1;
    D_0032E570[11] = 0;
    D_0032E570[12] = 0;
    D_0032E570[13] = 0;
    D_0032E570[14] = 0;
    D_0032E570[15] = 0;
    D_0032E570[16] = 0;
    D_0032E570[18] = 0;
    D_0032E570[19] = 1;
    D_0032E570[20] = 0;
    D_0032E570[21] = 0;
    if (mode == 0) {
        fldResetFieldMapState();
        datGameState->vr.previewX = 0;
        datGameState->vr.previewY = 0;
        {
            u32 *word = ((FldVrResetView *)&datGameState->vr)->words;
            for (i = 14; i >= 0; i--) {
                *word++ = 0;
            }
        }
        datGameState->header.magic[0] = 'V';
        datGameState->header.magic[1] = 'E';
        datGameState->header.magic[2] = 'R';
        datGameState->header.version = 1;
        datGameState->header.mapGroup = 0;
        datGameState->header.mapIndex = 0;
        datGameState->header.pad06[0] = 0;
        datGameState->header.pad06[1] = 0;
        datGameState->header.playTicks = 0;
        for (i = 0; i < 8; i++) {
            datGameState->header.partyIds[i] = 0;
            datGameState->header.partyLevels[i] = 0;
        }
        datGameState->header.savedMoney = 0;
        datGameState->header.saveFlags = 0;
        datGameState->header.saveFlags1 = 0;
        datGameState->header.saveFlags2 = 0;
        datGameState->header.status = 0;
        datGameState->header.transition = 0;
    }
}

extern u32 D_0032E570[];

extern void fldClearCameraFacingPointRequest(void);

extern void fldClearCameraHeadingRequest(void);

/* Reset the per-area controller work: -1 selections, the attrition step and the
 * adjacent request words, then clear the camera facing/heading requests. */
void fldInitializeAreaWork(void) {
    FldAreaWork *area = &fldAreaState;

    area->unk2C = 0x10;
    area->unk138 = 0;
    area->consumedFlags = 0;
    area->unk54 = 0;
    area->unk8C = 0;
    area->unk90 = -1;
    area->unk94 = -1;
    area->unk98 = -1;
    area->unkA0 = -1;
    area->unkA4 = -1;
    area->overrideSupportRecordIndex = -1;
    area->unkAC = -1;
    area->overlayMode = 1;
    area->overlayCounter = 0;
    area->unkD8 = -1;
    area->terrainAttritionKind = 0;
    area->unk184 = 0;
    area->cameraTiltActive = 0;
    area->nextArea = -1;
    area->skipFade = 0;
    area->attritionDistance = 100.0f;
    area->colorEffectSuppressed = 0;
    area->unkB4 = 0.0f;
    area->cameraTilt = 0.0f;
    area->frameTravelDistance = 0.0f;
    area->nextFloor = -1;
    area->unkD4 = 1;
    D_0032E570[0] = 0;
    D_0032E570[1] = 0;
    D_0032E570[2] = 0;
    D_0032E570[3] = 0;
    D_0032E570[4] = 0;
    D_0032E570[5] = 0;
    D_0032E570[11] = 0;
    D_0032E570[12] = 0;
    D_0032E570[13] = 0;
    D_0032E570[14] = 0;
    D_0032E570[16] = 0;
    fldClearCameraFacingPointRequest();
    fldClearCameraHeadingRequest();
}

/* Map IDs select 1920-byte banks of 30-byte slots; flag banks are halfwords.
 * The guards check only the upper map-ID bound: negative IDs and slot/bit
 * ranges remain unchecked. */
enum {
    FIELD_MAP_ID_LIMIT = 40,
    FIELD_MAP_ID_MODULUS = 100,
    FIELD_MAP_EMPTY_VALUE = 0xff,
};

void fldSetRoomModeFlag(s32 mapId, u32 slotIndex, s32 bit, s32 enabled) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        if (enabled) {
            datGameState->maps[mapIndex].slots[slotIndex].flagBanks[0] |= 1 << bit;
        } else {
            datGameState->maps[mapIndex].slots[slotIndex].flagBanks[0] &= ~(1 << bit);
        }
    }
}

s32 fldTestRoomModeFlag(s32 mapId, u32 slotIndex, s32 bit) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        u32 flags = datGameState->maps[mapIndex].slots[slotIndex].flagBanks[0];
        return (flags >> bit) & 1;
    }
    return 0;
}

void fldSetRoomObjectModeFlag(s32 mapId, u32 slotIndex, s32 bit, s32 enabled) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        if (enabled) {
            datGameState->maps[mapIndex].slots[slotIndex].roomObjectModeFlags |= 1 << bit;
        } else {
            datGameState->maps[mapIndex].slots[slotIndex].roomObjectModeFlags &= ~(1 << bit);
        }
    }
}

s32 fldTestRoomObjectModeFlag(s32 mapId, u32 slotIndex, s32 bit) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        u32 flags = datGameState->maps[mapIndex].slots[slotIndex].flagBanks[1];
        return (flags >> bit) & 1;
    }
    return 0;
}

void fldSetRoomSceneFlag(s32 mapId, u32 slotIndex, s32 bit, s32 enabled) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        if (enabled) {
            datGameState->maps[mapIndex].slots[slotIndex].roomSceneFlags |= 1 << bit;
        } else {
            datGameState->maps[mapIndex].slots[slotIndex].roomSceneFlags &= ~(1 << bit);
        }
    }
}

s32 fldTestRoomSceneFlag(s32 mapId, u32 slotIndex, s32 bit) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        u32 flags = datGameState->maps[mapIndex].slots[slotIndex].flagBanks[2];
        return (flags >> bit) & 1;
    }
    return 0;
}

void fldSetMapTargetFlag(s32 mapId, u32 slotIndex, s32 bit, s32 enabled) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        if (enabled) {
            datGameState->maps[mapIndex].slots[slotIndex].mapTargetFlags |= 1 << bit;
        } else {
            datGameState->maps[mapIndex].slots[slotIndex].mapTargetFlags &= ~(1 << bit);
        }
    }
}

s32 fldTestMapTargetFlag(s32 mapId, u32 slotIndex, s32 bit) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        u32 flags = datGameState->maps[mapIndex].slots[slotIndex].flagBanks[3];
        return (flags >> bit) & 1;
    }
    return 0;
}

void fldSetAlternateMapTargetFlag(s32 mapId, u32 slotIndex, s32 bit, s32 enabled) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        if (enabled) {
            datGameState->maps[mapIndex].slots[slotIndex].alternateMapTargetFlags |= 1 << bit;
        } else {
            datGameState->maps[mapIndex].slots[slotIndex].alternateMapTargetFlags &= ~(1 << bit);
        }
    }
}

s32 fldTestAlternateMapTargetFlag(s32 mapId, u32 slotIndex, s32 bit) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        u32 flags = datGameState->maps[mapIndex].slots[slotIndex].flagBanks[4];
        return (flags >> bit) & 1;
    }
    return 0;
}

/* Store a byte at valueIndex within the selected slot's values area. */
void fldSetMapSlotByte(s32 mapId, u32 slotIndex, s32 valueIndex, s32 byteValue) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        datGameState->maps[mapIndex].slots[slotIndex].values[valueIndex] = byteValue;
    }
}

/* Read a slot value, translating stored 0xff to -1; IDs >= the limit return zero. */
s32 fldGetMapSlotByte(s32 mapId, u32 slotIndex, s32 valueIndex) {
    u8 value = 0;
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        value = datGameState->maps[mapIndex].slots[slotIndex].values[valueIndex];
    }
    return value == FIELD_MAP_EMPTY_VALUE ? -1 : value;
}

void fldSetMapSlotAuxiliaryFlag(s32 mapId, u32 slotIndex, s32 bit, s32 enabled) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        if (enabled) {
            datGameState->maps[mapIndex].slots[slotIndex].trailingFlagBanks[0] |= 1 << bit;
        } else {
            datGameState->maps[mapIndex].slots[slotIndex].trailingFlagBanks[0] &= ~(1 << bit);
        }
    }
}

s32 fldTestMapSlotAuxiliaryFlag(s32 mapId, u32 slotIndex, s32 bit) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        u32 flags = datGameState->maps[mapIndex].slots[slotIndex].trailingFlagBanks[0];
        return (flags >> bit) & 1;
    }
    return 0;
}

void fldSetMapSlotValueFlag(s32 mapId, u32 slotIndex, s32 bit, s32 enabled) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        if (enabled) {
            datGameState->maps[mapIndex].slots[slotIndex].trailingFlagBanks[1] |= 1 << bit;
        } else {
            datGameState->maps[mapIndex].slots[slotIndex].trailingFlagBanks[1] &= ~(1 << bit);
        }
    }
}

s32 fldTestMapSlotValueFlag(s32 mapId, u32 slotIndex, s32 bit) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        u32 flags = datGameState->maps[mapIndex].slots[slotIndex].trailingFlagBanks[1];
        return (flags >> bit) & 1;
    }
    return 0;
}

void fldActivateFlaggedObject(u32 flagIndex) {
    datGameState->activationFlags[(s32)flagIndex >> 3] |= 1 << (flagIndex & 7);
    fldActivateObjectById(flagIndex);
    if (flagIndex >= 0xF0 && flagIndex < 0x100) {
        mdlFlagSet(flagIndex + 0x610);
    }
    if (D_0034C8F0[flagIndex].kind == 2) {
        ptyReturnOne(D_0034C8F0[flagIndex].parameter);
    }
}

u8 fldTestObjectActivationFlag(u32 flagIndex) {
    return (datGameState->activationFlags[(s32)flagIndex >> 3] >> (flagIndex & 7)) & 1;
}

/* Search records 1..511 of the 28-byte coordinate table; return zero on miss. */
s32 fldFindMapLabelIndexByCoordinate(s32 mapId, s32 floorIndex) {
    u8 *records = D_0033F068;
    u8 *yColumn = records + 2;
    s32 index = 1;
    s32 offset = 28;
    do {
        if (mapId == *(s16 *)(offset + (s32)records) &&
            floorIndex == *(s16 *)(offset + (s32)yColumn)) {
            return index;
        }
        index++;
        offset += 28;
    } while (index < FIELD_COORDINATE_SCAN_LIMIT);
    return 0;
}

/* Search the second 28-byte coordinate table, also reserving index zero. */
s32 fldFindSecondaryMapLabelIndexByCoordinate(s32 mapId, s32 floorIndex) {
    u8 *records = D_00342868;
    u8 *yColumn = records + 2;
    s32 index = 1;
    s32 offset = 28;
    do {
        if (mapId == *(s16 *)(offset + (s32)records) &&
            floorIndex == *(s16 *)(offset + (s32)yColumn)) {
            return index;
        }
        index++;
        offset += 28;
    } while (index < FIELD_COORDINATE_SCAN_LIMIT);
    return 0;
}

/* Search the 28-byte map records, skipping row zero; zero denotes a miss. */
s32 fldFindMapCoordinateIndex(s32 mapId, s32 floorIndex) {
    u8 *records = D_00346068;
    u8 *yColumn = records + 2;
    s32 index = 1;
    s32 offset = 28;
    do {
        if (mapId == *(s16 *)(offset + (s32)records) &&
            floorIndex == *(s16 *)(offset + (s32)yColumn)) {
            return index;
        }
        index++;
        offset += 28;
    } while (index < FIELD_MAP_COORDINATE_SCAN_LIMIT);
    return 0;
}

/* Search location records 1..639; a miss returns the reserved first record,
 * not NULL, so callers can read the fallback record's fields. */
s16 *fldFindLocationCoordinateRecord(s32 area, s32 locationIndex) {
    u8 *records = (u8 *)D_0032C9B0;
    u8 *yColumn = records + 2;
    s32 index = 1;
    do {
        s32 offset = index * 8;
        index++;
        if (area == *(s16 *)(offset + (s32)records) && locationIndex == *(s16 *)(offset + (s32)yColumn)) {
            return (s16 *)(offset + (s32)records);
        }
    } while (index < FIELD_LOCATION_COORDINATE_SCAN_LIMIT);
    return D_0032C9B0;
}

/* Read the matching location record's fourth signed halfword; zero on miss. */
s32 fldGetLocationCoordinateValue(s32 area, s32 locationIndex) {
    u8 *records = (u8 *)D_0032C9B0;
    u8 *yColumn = records + 2;
    u8 *valueColumn = records + 6;
    s32 index = 1;
    do {
        s32 offset = index * 8;
        index++;
        if (area == *(s16 *)(offset + (s32)records) && locationIndex == *(s16 *)(offset + (s32)yColumn)) {
            return *(s16 *)(offset + (s32)valueColumn);
        }
    } while (index < FIELD_LOCATION_COORDINATE_SCAN_LIMIT);
    return 0;
}

/* Search all 640 stage entries; the miss result is 640 rather than zero. */
u32 fldFindStageCoordinateIndex(s32 x, s32 y) {
    FieldStageCoordinate *record = D_0032DDB0;
    s32 index = 0;
    s32 visited = 0;

    do {
        if (record->x == x && record->y == y) {
            return index;
        }
        index++;
        visited++;
        record++;
    } while (visited < FIELD_STAGE_INDEX_SCAN_LIMIT);
    return index;
}

/* This pointer lookup searches only the first 96 stage entries; NULL on miss. */
FieldStageCoordinate *fldFindStageCoordinateRecord(s32 x, s32 y) {
    FieldStageCoordinate *record = D_0032DDB0;
    s32 index = 0;

    do {
        if (record->x == x && record->y == y) {
            return record;
        }
        index++;
        record++;
    } while (index < FIELD_STAGE_RECORD_SCAN_LIMIT);
    return NULL;
}

extern void fldGetSceneEntryPosition(s32 index, f32 *x, f32 *z);

/* Convert a scene-relative offset to a signed bitmap byte, row and bit. */
s32 fldGetStageBitmapBitCoordinates(s32 scene, s32 floor, f32 xOffset, f32 zOffset,
                  s32 *byteOut, s32 *rowOut, s32 *bitOut) {
    f32 entryX;
    f32 entryZ;
    FieldStageCoordinate *record;
    s32 cellX;
    s32 row;
    s32 byte;
    s32 bit;

    fldGetSceneEntryPosition(scene, &entryX, &entryZ);
    record = fldFindStageCoordinateRecord(D_0032E3C0[0], floor);
    if (record == NULL) {
        return 0;
    }
    cellX = (s32)(xOffset + entryX - record->originX) / record->cellSize;
    row = -((s32)(zOffset + entryZ - record->originZ) / record->cellSize);
    bit = cellX % 8;
    byte = cellX / 8;
    *byteOut = byte;
    *rowOut = row;
    *bitOut = bit;
    if (byte >= 0 && row >= 0 && byte < record->cols && row < record->rows) {
        return 1;
    }
    return 0;
}

void fldMarkStageExplorationCellsAroundPosition(s32 room, s32 stage, f32 x, f32 z, f32 unused) {
    s32 neighbors[9][2] = {
        {-1, -1}, {0, -1}, {1, -1},
        {-1, 0}, {0, 0}, {1, 0},
        {-1, 1}, {0, 1}, {1, 1}
    };
    f32 originX;
    f32 originZ;
    FieldStageCoordinate *record;
    FldAreaWork *area;
    s32 cellX;
    s32 cellZ;
    u32 stageIndex;
    s32 i;
    s32 neighborX;
    s32 neighborZ;
    s32 column;
    s32 bit;

    fldGetSceneEntryPosition(room, &originX, &originZ);
    area = &fldAreaState;
    record = fldFindStageCoordinateRecord(area->area, stage);
    if (record == NULL) {
        return;
    }
    cellX = (s32)((x + originX) - record->originX) / record->cellSize;
    cellZ = (s32)((z + originZ) - record->originZ) / record->cellSize;
    stageIndex = fldFindStageCoordinateIndex(area->area, stage);
    for (i = 0; i < 9; i++) {
        neighborX = cellX + neighbors[i][0];
        neighborZ = -(cellZ + neighbors[i][1]);
        bit = neighborX % 8;
        column = neighborX / 8;
        if (column >= 0 && neighborZ >= 0 && column < record->cols && neighborZ < record->rows) {
            datGameState->pad159D0[neighborZ * 10 + stageIndex * 800 + column] |= 1 << bit;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", fldMarkStageExplorationBitmapRegion);

s32 fldTestStageExplorationCell(s32 floor, f32 x, f32 z) {
    FieldStageCoordinate *record = fldFindStageCoordinateRecord(fldAreaState.area, floor);
    s32 cellX;
    s32 row;
    s32 byte;
    s32 bit;
    u32 stage;

    if (record == NULL) {
        return 0;
    }
    cellX = (s32)(x - record->originX) / record->cellSize;
    row = -((s32)(z - record->originZ) / record->cellSize);
    bit = cellX % 8;
    byte = cellX / 8;
    stage = fldFindStageCoordinateIndex(fldAreaState.area, floor);
    if (byte >= 0 && row >= 0 && byte < record->cols && row < record->rows) {
        if ((datGameState->pad159D0[row * 10 + stage * 800 + byte] >> bit) & 1) {
            return 1;
        }
    }
    return 0;
}
INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAAE0);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAAE8);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAAF0);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAAF8);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB00);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB08);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB0C);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB10);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB14);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB18);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB1C);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB20);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB24);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB28);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB2C);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB30);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", fldPlayerObject);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", fldPlayerModelContext);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB3C);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB40);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB50);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", fldPlayerModelResource);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB58);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB5C);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB60);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB64);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB68);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB70);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB78);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB80);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB88);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB90);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB98);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABA0);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABA8);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABB0);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABB8);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABC0);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABD0);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", fldDeferredCommand);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", fldDeferredCommandParameter);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABE0);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABE4);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABE8);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABEC);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABF0);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABF4);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", fldSceneLifecycleFlags);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABFC);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", fldSceneControlFlags);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAC08);

