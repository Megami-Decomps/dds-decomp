#include "fld_area_work.h"
#include "fld_npc_motion.h"
#include "fld_label_rows.h"
#include "sdf_packet_list.h"
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
#include "mdl.h"
#include "field_stage.h"
#include "eff_blur.h"
#include "pcp_vu0.h"
#include "fpu.h"
#include "fld.h"
#include "evt_world.h"
#include "dds3obj.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "dat_state.h"
#include "kwln_task_lifecycle.h"
/* Declared before first use (were implicit int). */
extern s32 fldFindTableEntry(s32 index);
extern void fldResetViewState(void);
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

extern void fldInitializeAreaWork(void);

extern void fldResetSparkRewardProfileIndex(void);

extern FieldStageCoordinate D_00389170[];




extern u8 D_003A25A8[];

extern s16 D_00387D70[];

struct EffWorldNode;

extern void fldStepValueByPad(f32 *controlledValue, u8 *padState, f32 min, f32 max, f32 step, f32 bigStep);

extern u8 D_0037F530[];

extern void fldStepIntByPad(void *, s32, s64, s64, s64, s64, s8 *);

extern s32 fldStepColorChannelByPad(u32 *, s32, s8 *);

extern s32 fldTestDrawUpdate(KwlnTask *task);

extern u8 fldGetCampSceneControlMode(void);

extern void fldSubmitFrameQuad(s32, s32, s32, s32, s32, s32, s32, s32);

extern void fldSetGsAlphaBlendEquation(s32);

extern void func_00136EF8(void);

extern void fldDrawSkyGradient(void);

extern void fldSubmitTexturedOverlaySprite(void);

extern u32 D_00389988[];

extern void fldSelectDisplayBuffer(u32);

extern void fldSelectDisplayBuffer(u32);

extern u32 D_00389988[];

extern u32 D_00389988[];

extern char D_00412B90[];


extern s32 D_00435EE0;

extern s32 D_00435EE4;

extern s32 D_00435EE8;

extern s32 D_00435EEC;

extern s32 D_00435EF0;

extern s32 D_00435EF4;

extern s32 D_00435EF8;

extern void fldSetCameraMoveMode(s32 mode);

extern void fldClearCameraMoveMode(void);


extern void fldSubmitBackgroundResourcePacket(void);

extern void fldSubmitBackgroundDescriptorPacket(void);

extern s32 D_00389780[];


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

INCLUDE_ASM(const s32, "game/code_0011F208", func_0011F518);

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
    fldStepValueByPad(value, D_0037F530, min, max, step, bigStep);
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
    fldStepIntByPad(valueAddress, storageType, min, max, step, bigStep, (s8 *)D_0037F530);
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
    fldStepColorChannelByPad(color, channel, (s8 *)D_0037F530);
}

/* Emit one whole-value character and two fractional digits after rounding.
 * At whole >= 10 only the fractional digits become 9; whole is not clamped. */
void fldFormatSecondsText(f32 displayValue, char *text) {
    s32 whole;
    s32 tenths;
    s32 hundredths;
    displayValue += 0.005f;
    whole = (s32)displayValue;
    if (whole >= 10) {
        tenths = 9;
        hundredths = 9;
    } else {
        displayValue -= whole;
        displayValue *= 10.0f;
        tenths = (s32)displayValue;
        displayValue -= tenths;
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

extern u8 D_00386480[];

extern char *D_00386488[];

/* Draw three RGB value rows and their clamped colour preview. */
void fldDrawRgbEditor(void *packetList, s32 x, s32 y, s32 selectedChannel, f32 *rgbValues) {
    char text[16];
    u32 previewColor = 0x80000000;
    s32 channelIndex;
    s32 selectedStyle;

    if (rgbValues != NULL) {
        previewColor = 0;
        for (channelIndex = 0; channelIndex != 3; channelIndex++) {
            s32 channelValue = rgbValues[channelIndex] * 128.0f;
            if (channelValue > 0) {
                if (channelValue >= 256) {
                    channelValue = 255;
                }
                previewColor |= channelValue << (channelIndex * 8);
            }
        }
        previewColor |= 0x80000000;
    }
    sdfAppendPacket(packetList, (u32)sdfCreateBorderedRectanglePacket(x + 0x510, y + 0x18,
                     0xFF0080, 0x240, 0xF0, previewColor, 0x40806020));
    for (channelIndex = 0; channelIndex != 3; channelIndex++) {
        sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(x, y,
                        0xFF0080, D_00386480[channelIndex], D_00386488[channelIndex]));
        selectedStyle = selectedChannel == channelIndex ? 6 : 0;
        if (rgbValues == NULL) {
            text[0] = '-';
            text[1] = '-';
            text[2] = '-';
            text[3] = '-';
            text[4] = 0;
        } else {
            fldFormatSecondsText(rgbValues[channelIndex], text);
        }
        sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(x + 0x180,
                        y, 0xFF0080, selectedStyle, text));
        y += 0x60;
    }
}

extern char D_00435EB8[];

extern char D_00435EC0[];

extern char D_00435EC8[];

extern char D_00435ED0[];

extern char D_00435ED8[];

void fldDrawPackedRgbEditor(void *packetList, s32 x, s32 y, s32 selectedChannel,
                   u32 packedColor, s32 showNormalizedValues) {
    s32 channelIndex;
    s32 selectedStyle;
    s32 channelValue;
    const char *label;

    packedColor = (packedColor & 0xFFFFFF) | 0x80000000;
    sdfAppendPacket(packetList, (u32)sdfCreateBorderedRectanglePacket(x + 0x510, y + 0x18,
                    0xFF0080, 0x240, 0xF0, packedColor, 0x40806020));
    for (channelIndex = 0; channelIndex != 3; channelIndex++) {
        switch (channelIndex) {
        case 0:
            selectedStyle = 2;
            label = D_00435EC8;
            channelValue = packedColor & 0xFF;
            break;
        case 1:
            selectedStyle = 4;
            label = D_00435EC0;
            channelValue = (packedColor >> 8) & 0xFF;
            break;
        default:
            selectedStyle = 5;
            label = D_00435EB8;
            channelValue = (packedColor >> 16) & 0xFF;
            break;
        }
        sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(
                        x, y, 0xFF0080, selectedStyle, label));
        sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(
                        x + 0x180, y, 0xFF0080, channelIndex == selectedChannel ? 6 : 0,
                        D_00435ED0, channelValue));
        if (showNormalizedValues != 0) {
            sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(
                            x, y + 0x180, 0xFF0080, selectedStyle, label));
            sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(
                            x + 0x240, y + 0x180, 0xFF0080, 0,
                            D_00435ED8, channelValue * (1.0f / 255.0f)));
        }
        y += 0x60;
    }
}

u32 fldIsUpdateInhibited(void) {
    return 0;
}



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
        cosA = sdfEvaluateCosineViaSinePhaseShift(a) * radius;
        b = (f32)prev * 3.14f / 180.0f;
        sinB = sdfSinPoly(b) * radius;
        cosB = sdfEvaluateCosineViaSinePhaseShift(b) * radius;
        fldSubmitGsTriangle(color, 0x80101010, 0x80101010,
                            x + 0.0f, y - 2.0f, z + 0.0f,
                            x + sinA, y - 2.0f, z + cosA,
                            x + sinB, y - 2.0f, z + cosB);
    }
    fldSetGsAlphaBlendEquation(0);
}

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
    func_00136EF8();
    fldDrawSkyGradient();
    if (D_00389988[9] != 0) {
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
    kwlnTaskCreate(D_00412B90, 0x2AF8, 0, 0, fldTestDrawUpdate, NULL, 0);
}

void fldTestDrawDestroy(void) {
    kwlnTaskDestroyWithHierarchyByName(D_00412B90, 1);
}

extern void frFontSetSharedRenderFlags(u32);
extern u8 D_00387C20[][4];
extern void fldDrawConvertedTextGlyph(s32, s32, u32, const u8 *);
static inline void fldDrawPercentGlyph(s32 x, s32 y, u32 color, s32 glyph) {
    fldDrawConvertedTextGlyph(x, y / 2, color, D_00387C20[glyph]);
}

void fldDrawPercentValue(s32 x, s32 y, s32 value) {
    s32 negative = 0;
    s32 digit;

    frFontSetSharedRenderFlags(0x54);

    if (value >= 1001) {
        fldDrawPercentGlyph(x, y, 0xA09DC380, 12);
        return;
    }

    if (value < 0) {
        x += 16;
        fldDrawPercentGlyph(x, y, 0xA09DC380, 10);
        value = -value;
        negative = 1;
    }

    do {
        x += 13;
        digit = value / 10;
    } while (0);
    if (digit > 0 && digit < 10) {
        fldDrawPercentGlyph(x, y, 0xA09DC380, digit);
        x += 11;
    }

    digit = value % 10;
    if (digit < 10) {
        fldDrawPercentGlyph(x, y, 0xA09DC380, digit);
    }

    if (negative == 0) {
        fldDrawPercentGlyph(x + 11, y, 0xA09DC380, 11);
    }
}

void fldConfigureCameraMoveTracking(s32 start, s32 end) {
    s32 first;
    s32 endEntry;

    first = fldFindTableEntry(start);
    D_00435EF8 = 0;
    D_00435EE4 = first;
    if (first > 1000) {
        D_00435EE0 = first % 1000;
    } else {
        D_00435EE0 = first;
    }
    if (first > 1000) {
        D_00435EE8 = first;
        D_00435EE4 = first % 1000;
    } else {
        D_00435EE8 = -999;
    }

    endEntry = fldFindTableEntry(end);
    D_00435EEC = endEntry;
    if (endEntry > 1000) {
        D_00435EF0 = endEntry;
        D_00435EEC = endEntry % 1000;
    } else {
        D_00435EF0 = -999;
    }
    if (start == end) {
        D_00435EF4 = -999;
    } else {
        D_00435EF4 = 0;
    }
    if (D_00435EE0 != -999 && D_00435EF4 != -999) {
        D_00435EF4++;
        if (D_00435EE4 < D_00435EEC) {
            fldSetCameraMoveMode(1);
        } else {
            fldSetCameraMoveMode(-1);
        }
    }
}

/* Clear the move mode and invalidate all six stored tracking values. */
void fldResetCameraMoveTracking(void) {
    D_00435EF8 = 0;
    D_00435EE0 = FIELD_CAMERA_TRACKING_UNSET;
    D_00435EE4 = FIELD_CAMERA_TRACKING_UNSET;
    D_00435EE8 = FIELD_CAMERA_TRACKING_UNSET;
    D_00435EEC = FIELD_CAMERA_TRACKING_UNSET;
    D_00435EF0 = FIELD_CAMERA_TRACKING_UNSET;
    D_00435EF4 = FIELD_CAMERA_TRACKING_UNSET;
    fldClearCameraMoveMode();
}

/* Report the pending camera move mode, kicking the field camera when the
 * stored start and end indices agree. */
s32 fldEvaluateCameraMoveTracking(void) {
    if (D_00435EF8 == 1) {
        return 2;
    }
    if (D_00435EF4 != FIELD_CAMERA_TRACKING_UNSET && D_00435EE0 == D_00435EEC) {
        if (D_00435EE4 < D_00435EE0) {
            fldSetCameraMoveMode(2);
        } else {
            fldSetCameraMoveMode(-2);
        }
        return 1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412B90);

INCLUDE_ASM(const s32, "game/code_0011F208", func_001208D0);

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
                if (dds3TestObjectFlags(worldObject, FIELD_BACKGROUND_REQUIRED_OBJECT_FLAG) != 0) {
                    if (dds3TestObjectFlags(worldObject, FIELD_BACKGROUND_EXCLUDED_OBJECT_FLAG) == 0) {
                        hasEligibleBackground = 1;
                    }
                }
            } while (dds3AdvanceObjectValueCursor((WorldValueIndices *)objectChain) != 0);
        }
        dds3DestroyWorldIndexNode(objectChain);
    }
    if (D_00389780[0] == 1) {
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
extern u32 D_0043607C;
extern s32 D_00435EFC, D_00435F00, D_00435F04, D_00435F08;
extern f32 D_0043E5E0[4];
extern f32 D_0043E5F0[50][3][4];
extern s8 D_0043EF50[26][26][2];
extern f32 D_0043F4A0[26][26][4];
extern f32 D_00441EE0[26][26][4];
extern f32 sdfViewEyeVector[4], sdfViewTargetVector[4];
extern SdfLightSources D_0037F770;

extern f32 D_0038BB10[16];
extern f32 D_0037F6B0[16];

extern FldShadowStrip D_00387C60;
extern FldShadowStrip D_00387CA0;
extern const f32 D_00412BB8[18];

/* Packed XYZ geometry, followed by triples of signed vertex indices. */
extern f32 D_003864A0[][3], D_00386818[][3], D_00386908[][3];
extern f32 D_00386A20[][3], D_00386B58[][3], D_00386C00[][3];
extern f32 D_00386D18[][3], D_00386E50[][3], D_00386F68[][3];
extern f32 D_00387068[][3], D_00387158[][3], D_00387270[][3];
extern f32 D_00387370[][3], D_00387460[][3], D_00387560[][3];
extern f32 D_00387660[][3], D_00387760[][3], D_00387860[][3];
extern f32 D_00387978[][3], D_00387A90[][3];
extern s32 D_003865D8[][3], D_00386878[][3], D_00386978[][3];
extern s32 D_00386A98[][3], D_00386BA0[][3], D_00386C70[][3];
extern s32 D_00386D90[][3], D_00386EC0[][3], D_00386FD8[][3];
extern s32 D_003870C8[][3], D_003871C8[][3], D_003872E0[][3];
extern s32 D_003873D0[][3], D_003874D0[][3], D_003875D0[][3];
extern s32 D_003876D0[][3], D_003877D0[][3], D_003878D0[][3];
extern s32 D_003879E8[][3], D_00387B30[][3];


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

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412BB8);

void func_00120B88(EffWorldNode *object) {
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
    memcpy(extrusionLengths, D_00412BB8, sizeof(extrusionLengths));
    triangleStrip = D_00387CA0;
    quadStrip = D_00387C60;
    opacity = 100.0f;
    base = dds3GetObjectOwnedHandle(object);
    model = ((MdlCtx *)dds3GetObjectBaseResourceHandle(object))->inner;
    if (dds3TestObjectFlags(object, 0x4000)) {
        transition = effObjGetTransitionWork(object);
        D_00435F00 = transition->shadowLevel;
        D_00435F04 = 1;
    } else if (D_00435F04 == 1) {
        D_00435F00 = 16;
        D_00435F04 = 0;
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
                D_0043E5E0[0] = transition->vec10[0];
                D_0043E5E0[1] = 2.0f;
                D_0043E5E0[2] = transition->vec10[2];
                length = 200.0f;
            } else {
                D_0043E5E0[0] = D_0037F770[0][1][0];
                D_0043E5E0[1] = 2.0f;
                D_0043E5E0[2] = D_0037F770[0][1][2];
            }
        }
    } else {
        data = object->data;
        if (data->activeId != -1) {
            if (D_00435F08 == 0) {
                if (D_00435F00 > 20) {
                    D_00435F00 -= 2;
                } else {
                    D_00435F08 = 1;
                    D_00435F00 = 8;
                }
                length = 200.0f;
            } else {
                target = dds3FindWorldObjectNodeByKey(dds3GetWorldObject(), data->activeId, 9);
                transition = effObjGetTransitionWork(object);
                if (sdfLoadMapRecordPositionVector(transition->owner->inner, 0)) {
                    VU0_STORE_VF_UNCLOBBERED(vf10, anchorPosition);
                    D_0043E5E0[0] = anchorPosition[0] - target->inner->position[0];
                    D_0043E5E0[1] = anchorPosition[1] - target->inner->position[1];
                    D_0043E5E0[2] = anchorPosition[2] - target->inner->position[2];
                } else {
                    D_0043E5E0[0] = fldAreaState.x - target->inner->position[0];
                    /* Retail 0x0011F134..0x0011F15C uses field Z for both Y and Z. */
                    D_0043E5E0[1] = fldAreaState.z - target->inner->position[1];
                    D_0043E5E0[2] = fldAreaState.z - target->inner->position[2];
                }
                horizontalDistance = fsqrtf(D_0043E5E0[0] * D_0043E5E0[0] + D_0043E5E0[2] * D_0043E5E0[2]);
                if (D_0043E5E0[1] < 0.0f) {
                    verticalDistance = -D_0043E5E0[1];
                } else {
                    verticalDistance = D_0043E5E0[1];
                }
                if (verticalDistance < 10.0f) verticalDistance = 10.0f;
                if (verticalDistance > 200.0f) verticalDistance = 200.0f;
                verticalDistance -= 10.0f;
                verticalDistance /= 25.0f;
                if (horizontalDistance < 0.01f) {
                    D_0043E5E0[0] = 0.1f;
                    D_0043E5E0[1] = 2.0f;
                    D_0043E5E0[2] = 0.1f;
                } else {
                    D_0043E5E0[0] /= horizontalDistance;
                    D_0043E5E0[1] = verticalDistance + 1.0f;
                    D_0043E5E0[2] /= horizontalDistance;
                }
                if (horizontalDistance < 100.0f) horizontalDistance = 100.0f;
                if (horizontalDistance > 300.0f) horizontalDistance = 300.0f;
                horizontalDistance -= 100.0f;
                horizontalDistance /= 2.6f;
                opacity = 100.0f - horizontalDistance;
                if (D_00435F00 < 48) D_00435F00 += 2;
                length = 200.0f;
            }
        } else if (D_00435F08 == 0) {
            if (fldAreaState.area < 200) {
                D_0043E5E0[0] = D_0038BB10[4];
                D_0043E5E0[1] = D_0038BB10[5];
                D_0043E5E0[2] = D_0038BB10[6];
            } else {
                D_0043E5E0[0] = D_0037F6B0[4];
                D_0043E5E0[1] = D_0037F6B0[5];
                D_0043E5E0[2] = D_0037F6B0[6];
            }
            horizontalDistance = fsqrtf(D_0043E5E0[0] * D_0043E5E0[0] + D_0043E5E0[2] * D_0043E5E0[2]);
            if (horizontalDistance < 0.01f) {
                D_0043E5E0[0] = 0.2f;
                D_0043E5E0[1] = 2.0f;
                D_0043E5E0[2] = 0.1f;
            } else {
                D_0043E5E0[0] /= horizontalDistance;
                D_0043E5E0[1] = 2.0f;
                D_0043E5E0[2] /= horizontalDistance;
            }
            D_0043E5E0[0] += 0.1f;
            if (D_00435F00 > 20) D_00435F00 -= 2;
            if (D_00435F00 < 20) D_00435F00 += 2;
            length = 200.0f;
        } else {
            if (D_00435F00 > 20) {
                D_00435F00 /= 3;
            } else {
                D_00435F08 = 0;
                D_00435F00 = 6;
            }
            length = 200.0f;
        }
    }
    D_0043E5E0[3] = 1.0f;
    VU0_LOAD_VF_PTR(vf10, D_0043E5E0);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF_UNCLOBBERED(vf10, D_0043E5E0);
    /* Retail still stores this full-length vector at11F43C..11F44C;
     * the later per-slot geometry uses slotExtrusion instead. */
    extrusion[0] = D_0043E5E0[0] * length;
    extrusion[1] = D_0043E5E0[1] * length;
    extrusion[2] = D_0043E5E0[2] * length;
    extrusion[3] = 1.0f;
    VU0_LOAD_VF_PTR(vf10, D_0043E5E0);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF_UNCLOBBERED(vf10, D_0043E5E0);
    for (slot = 0; slot < base->resourceSlotCount; slot++) {
        length = extrusionLengths[slot];
        slotExtrusion[slot][0] = D_0043E5E0[0] * length;
        slotExtrusion[slot][1] = D_0043E5E0[1] * length;
        slotExtrusion[slot][2] = D_0043E5E0[2] * length;
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
        D_00435EFC = 0;
        switch (slot) {
        case 0:
            if (!(base->resourceSlotFamily & 0xF000)) {
                vertices = D_00387560; triangles = D_003875D0;
                triangleCount = 12; vertexCount = 9;
            } else {
                vertices = D_00387460; triangles = D_003874D0;
                triangleCount = 12; vertexCount = 9;
            }
            break;
        case 1:
            if (!(base->resourceSlotFamily & 0xF000)) {
                vertices = D_00387760; triangles = D_003877D0;
                triangleCount = 12; vertexCount = 9;
            } else {
                vertices = D_00387660; triangles = D_003876D0;
                triangleCount = 12; vertexCount = 9;
            }
            break;
        case 2:
            if (!(base->resourceSlotFamily & 0xF000)) {
                vertices = D_00387978; triangles = D_003879E8;
                triangleCount = 14; vertexCount = 9;
            } else {
                vertices = D_00387860; triangles = D_003878D0;
                triangleCount = 14; vertexCount = 9;
            }
            break;
        case 3:
            if (fldAreaState.area >= 500) continue;
            vertices = D_00386818; triangles = D_00386878;
            triangleCount = 12; vertexCount = 8;
            break;
        case 4:
            vertices = D_003864A0; triangles = D_003865D8;
            triangleCount = 48; vertexCount = 26;
            break;
        case 5:
            vertices = D_00386C00; triangles = D_00386C70;
            triangleCount = 14; vertexCount = 9;
            break;
        case 6:
            vertices = D_00386D18; triangles = D_00386D90;
            triangleCount = 16; vertexCount = 10;
            break;
        case 8:
            vertices = D_00386908; triangles = D_00386978;
            triangleCount = 14; vertexCount = 9;
            break;
        case 9:
            vertices = D_00386A20; triangles = D_00386A98;
            triangleCount = 16; vertexCount = 10;
            break;
        case 7:
        case 10:
            vertices = D_00386B58; triangles = D_00386BA0;
            triangleCount = 8; vertexCount = 6;
            break;
        case 11:
            vertices = D_00387158; triangles = D_003871C8;
            triangleCount = 14; vertexCount = 9;
            break;
        case 12:
            vertices = D_00387270; triangles = D_003872E0;
            triangleCount = 12; vertexCount = 9;
            break;
        case 13:
            vertices = D_00387370; triangles = D_003873D0;
            triangleCount = 12; vertexCount = 8;
            break;
        case 14:
            vertices = D_00386E50; triangles = D_00386EC0;
            triangleCount = 14; vertexCount = 9;
            break;
        case 15:
            vertices = D_00386F68; triangles = D_00386FD8;
            triangleCount = 12; vertexCount = 9;
            break;
        case 16:
            vertices = D_00387068; triangles = D_003870C8;
            triangleCount = 12; vertexCount = 8;
            break;
        case 17:
            vertices = D_00387A90; triangles = D_00387B30;
            triangleCount = 20; vertexCount = 13;
            break;
        default:
            continue;
        }
        for (edgeFrom = 0; edgeFrom < vertexCount; edgeFrom++) {
            for (j = 0; j < vertexCount; j++) {
                D_0043EF50[edgeFrom][j][0] = 0;
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
            VU0_LOAD_VF_PTR(vf11, D_0043E5E0);
            VU0_DOT_XYZ(facing, vf10, vf11);
            if (facing < 0.0f) {
                first = triangle[0]; second = triangle[1];
                if (first < second) {
                    minimum = first; maximum = second; reversed = 0;
                } else {
                    minimum = second; maximum = first; reversed = 1;
                }
                D_0043EF50[minimum][maximum][0]++;
                D_0043EF50[minimum][maximum][1] = reversed;
                if (reversed == 0) {
                    PCP_COPY_VECTOR_UNCLOBBERED(D_0043F4A0[minimum][maximum], firstVertex);
                    PCP_COPY_VECTOR_UNCLOBBERED(D_00441EE0[minimum][maximum], secondVertex);
                } else {
                    PCP_COPY_VECTOR_UNCLOBBERED(D_0043F4A0[minimum][maximum], secondVertex);
                    PCP_COPY_VECTOR_UNCLOBBERED(D_00441EE0[minimum][maximum], firstVertex);
                }
                first = triangle[1]; second = triangle[2];
                if (first < second) {
                    minimum = first; maximum = second; reversed = 0;
                } else {
                    minimum = second; maximum = first; reversed = 1;
                }
                D_0043EF50[minimum][maximum][0]++;
                D_0043EF50[minimum][maximum][1] = reversed;
                if (reversed == 0) {
                    PCP_COPY_VECTOR_UNCLOBBERED(D_0043F4A0[minimum][maximum], secondVertex);
                    PCP_COPY_VECTOR_UNCLOBBERED(D_00441EE0[minimum][maximum], thirdVertex);
                } else {
                    PCP_COPY_VECTOR_UNCLOBBERED(D_0043F4A0[minimum][maximum], thirdVertex);
                    PCP_COPY_VECTOR_UNCLOBBERED(D_00441EE0[minimum][maximum], secondVertex);
                }
                second = triangle[2]; first = triangle[0];
                if (second < first) {
                    minimum = second; maximum = first; reversed = 0;
                } else {
                    minimum = first; maximum = second; reversed = 1;
                }
                D_0043EF50[minimum][maximum][0]++;
                D_0043EF50[minimum][maximum][1] = reversed;
                if (reversed == 0) {
                    PCP_COPY_VECTOR_UNCLOBBERED(D_0043F4A0[minimum][maximum], thirdVertex);
                    PCP_COPY_VECTOR_UNCLOBBERED(D_00441EE0[minimum][maximum], firstVertex);
                } else {
                    PCP_COPY_VECTOR_UNCLOBBERED(D_0043F4A0[minimum][maximum], firstVertex);
                    PCP_COPY_VECTOR_UNCLOBBERED(D_00441EE0[minimum][maximum], thirdVertex);
                }
                PCP_COPY_VECTOR_UNCLOBBERED(D_0043E5F0[D_00435EFC][0], firstVertex);
                PCP_COPY_VECTOR_UNCLOBBERED(D_0043E5F0[D_00435EFC][1], secondVertex);
                PCP_COPY_VECTOR_UNCLOBBERED(D_0043E5F0[D_00435EFC][2], thirdVertex);
                D_00435EFC++;
            } else {
                if (fldAreaState.area >= 500) {
                    switch (slot) {
                    case 2: case 3: case 4: case 7:
                    case 10: case 11: case 12: case 13:
                    case 14: case 15: case 16: case 17:
                        D_0043E5F0[D_00435EFC][0][0] = firstVertex[0] + slotExtrusion[slot][0];
                        D_0043E5F0[D_00435EFC][0][1] = firstVertex[1] + slotExtrusion[slot][1];
                        D_0043E5F0[D_00435EFC][0][2] = firstVertex[2] + slotExtrusion[slot][2];
                        D_0043E5F0[D_00435EFC][1][0] = secondVertex[0] + slotExtrusion[slot][0];
                        D_0043E5F0[D_00435EFC][1][1] = secondVertex[1] + slotExtrusion[slot][1];
                        D_0043E5F0[D_00435EFC][1][2] = secondVertex[2] + slotExtrusion[slot][2];
                        D_0043E5F0[D_00435EFC][2][0] = thirdVertex[0] + slotExtrusion[slot][0];
                        D_0043E5F0[D_00435EFC][2][1] = thirdVertex[1] + slotExtrusion[slot][1];
                        D_0043E5F0[D_00435EFC][2][2] = thirdVertex[2] + slotExtrusion[slot][2];
                        D_00435EFC++;
                        break;
                    }
                } else {
                    D_0043E5F0[D_00435EFC][0][0] = firstVertex[0] + slotExtrusion[slot][0];
                    D_0043E5F0[D_00435EFC][0][1] = firstVertex[1] + slotExtrusion[slot][1];
                    D_0043E5F0[D_00435EFC][0][2] = firstVertex[2] + slotExtrusion[slot][2];
                    D_0043E5F0[D_00435EFC][1][0] = secondVertex[0] + slotExtrusion[slot][0];
                    D_0043E5F0[D_00435EFC][1][1] = secondVertex[1] + slotExtrusion[slot][1];
                    D_0043E5F0[D_00435EFC][1][2] = secondVertex[2] + slotExtrusion[slot][2];
                    D_0043E5F0[D_00435EFC][2][0] = thirdVertex[0] + slotExtrusion[slot][0];
                    D_0043E5F0[D_00435EFC][2][1] = thirdVertex[1] + slotExtrusion[slot][1];
                    D_0043E5F0[D_00435EFC][2][2] = thirdVertex[2] + slotExtrusion[slot][2];
                    D_00435EFC++;
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
            sdfConsAppendAssetPacket(quadList, (void *)D_0043607C, NULL);
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
            sdfConsAppendAssetPacket(triangleList, (void *)D_0043607C, NULL);
            memset(&triangleRequest, 0, sizeof(triangleRequest));
            triangleRequest.positions = triangleVertices;
            triangleRequest.strip = triangleStrip.words;
            triangleRequest.stripWordCount = 0;
            triangleRequest.vertexCount = 0;
            if (pass == 0) triangleRequest.color = 0x80000000;
            else triangleRequest.color = 0;
            for (face = 0; face < D_00435EFC; face++) {
                if (pass == 0) {
                    f32 (*output)[4] = &triangleVertices[triangleRequest.vertexCount];
                    PCP_COPY_VECTOR_UNCLOBBERED(output[0], D_0043E5F0[face][0]);
                    PCP_COPY_VECTOR_UNCLOBBERED(output[1], D_0043E5F0[face][1]);
                    PCP_COPY_VECTOR_UNCLOBBERED(output[2], D_0043E5F0[face][2]);
                    triangleRequest.stripWordCount++;
                    triangleRequest.vertexCount += 3;
                } else {
                    f32 (*output)[4] = &triangleVertices[triangleRequest.vertexCount];
                    PCP_COPY_VECTOR_UNCLOBBERED(output[0], D_0043E5F0[face][2]);
                    PCP_COPY_VECTOR_UNCLOBBERED(output[1], D_0043E5F0[face][1]);
                    PCP_COPY_VECTOR_UNCLOBBERED(output[2], D_0043E5F0[face][0]);
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
                    if (D_0043EF50[edgeFrom][j][0] != 1) continue;
                    PCP_COPY_VECTOR_UNCLOBBERED(firstVertex, D_0043F4A0[edgeFrom][j]);
                    PCP_COPY_VECTOR_UNCLOBBERED(secondVertex, D_00441EE0[edgeFrom][j]);
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
                        if (D_0043EF50[edgeFrom][j][1]) {
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
        alpha = (s32)((f32)D_00435F00 * opacity / 100.0f);
        if (alpha < 0) alpha = 0;
        fldSubmitGsRect((s32)(minimumX * 16.0f), (s32)(minimumY * 16.0f),
                        (s32)(maximumX * 16.0f), (s32)(maximumY * 16.0f),
                        0, 0, 0, alpha);
    }
    fldSubmitFrameQuad(1, 5, 0x80, 3, 0, 0, 1, 2);
    fldSetGsAlphaBlendEquation(0);
}

extern FldActivationEntry D_003A8EB0[];

extern u8 D_0039A1D0[], D_003A41A8[], D_003A47E8[], D_003A55F0[], D_0038A3B8[], D_0038A480[], D_00389A70[];
extern FldCamRow fldCameraFollowRows[];
extern FldNpcMotion D_00391FA0[];

extern u32 D_003899F0[];

extern FldCameraSetting *fldCameraSettings;

/* Load the field tables and index each stage by the coordinate-row count
 * preceding its first record. */
void fldLoadFieldTablesAndIndexStages(void) {
    DevState *command = sdfDevCreateCommandState("/fld/f/bin/FLDALL.TBL");
    s32 sum;
    s16 previous;
    s32 i;

    sdfDevQueueReadAndWait(command, D_0039A1D0, 0x3D8);
    sdfDevQueueReadAndWait(command, D_0039A5A8, 0x3C00);
    sdfDevQueueReadAndWait(command, D_0039E1A8, 0x4400);
    sdfDevQueueReadAndWait(command, D_003A25A8, 0x1C00);
    sdfDevQueueReadAndWait(command, D_003A41A8, 0x640);
    sdfDevQueueReadAndWait(command, D_003A47E8, 0xC80);
    sdfDevQueueReadAndWait(command, D_003A55F0, 0x3840);
    sdfDevQueueReadAndWait(command, D_003A8EB0, 0x1000);
    sdfDevQueueReadAndWait(command, D_0038A3B8, 0xC8);
    sdfDevQueueReadAndWait(command, D_0038A480, 0x190);
    sdfDevQueueReadAndWait(command, D_0038A9B0, 0x500);
    sdfDevQueueReadAndWait(command, fldCameraFollowRows, 0xC00);
    sdfDevQueueReadAndWait(command, fldCameraSettings, 0x2A0);
    sdfDevQueueReadAndWait(command, D_00387D70, 0x1400);
    sdfDevQueueReadAndWait(command, D_00389170, 0x600);
    sdfDevQueueReadAndWait(command, D_00389A70, 0x880);
    sdfDevQueueReadAndWait(command, D_00391FA0, 0x1200); /* 64 FldNpcMotion rows */
    sdfDevWaitThenReleaseCommandState(command);
    sum = 0;
    previous = 0;
    for (i = 0; i < 0x1F; i++) {
        D_003899F0[i] = 0;
    }
    for (i = 0; i < 0x60; i++) {
        s16 id = (D_00389170)[i].x;

        if (id > 0) {
            if (id < 0x1F) {
                if (id != previous) {
                    D_003899F0[id] = sum;
                }
                previous = id;
                sum += (D_00389170)[i].rows;
            }
        }
    }
}

extern void fldClearAllAreaFloorFlags(void);

void fldResetFieldMapState(void) {
    s32 bankIndex;
    s32 slotIndex;
    s32 valueIndex;

    for (bankIndex = 0; bankIndex < 32; bankIndex++) {
        datGameState->activationFlags[bankIndex] = 0;
    }

    for (bankIndex = 0; bankIndex < 31; bankIndex++) {
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
    memset(datGameState->stageCellBits, 0, sizeof(datGameState->stageCellBits));
}

#include "fld_vr_reset_view.h"

extern u32 D_00389988[];
void fldResetFieldRuntimeState(s32 mode) {
    FldAreaWork *area = &fldAreaState;
    s32 i;

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
    area->movementSoundActive = 0;
    area->terrainAttritionKind = 0;
    area->attritionDistance = 100.0f;
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
    area->targetGuideEnabled = 0;
    area->targetGuideActive = 0;
    area->overlayFadeInFrames = 0;
    area->playerModelVariant = 0;
    area->x = 0.0f;
    area->y = 0.0f;
    area->z = 0.0f;
    area->previousX = 0.0f;
    area->previousY = 0.0f;
    area->positionPending = 0;
    area->positionMode = 0;
    area->unk188 = 0;
    area->verticalStepDirection = 0;
    area->unk190 = 0;
    area->previousZ = 0.0f;
    area->targetX = 0.0f;
    area->targetY = 0.0f;
    area->targetZ = 0.0f;
    area->targetAngle = 180.0f;
    area->angle = 0.0f;
    area->previousCameraModelFrame = 0.0f;
    area->unk17C = 0.0f;
    area->unk180 = 0.0f;
    area->fieldTextures[0].texture = NULL;
    area->fieldTextures[1].texture = NULL;
    area->fieldTextures[2].texture = NULL;
    area->fieldTextures[3].texture = NULL;
    D_00389988[0] = 0;
    D_00389988[1] = 0;
    D_00389988[2] = 0;
    D_00389988[3] = 0;
    D_00389988[4] = 0;
    D_00389988[5] = 0;
    D_00389988[6] = 0;
    D_00389988[7] = 0;
    D_00389988[8] = 0;
    D_00389988[9] = 1;
    D_00389988[10] = 1;
    D_00389988[11] = 0;
    D_00389988[12] = 0;
    D_00389988[13] = 0;
    D_00389988[14] = 0;
    D_00389988[15] = 0;
    D_00389988[16] = 0;
    D_00389988[19] = 1;
    D_00389988[18] = 0;
    D_00389988[21] = 0;
    D_00389988[22] = 0;
    D_00389988[23] = 1;
    D_00389988[24] = 1;
    D_00389988[20] = 1;
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
    fldResetViewState();
    fldResetSparkRewardProfileIndex();
}

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
    area->unk190 = 0;
    area->cameraTiltActive = 0;
    area->targetGuideEnabled = 0;
    area->targetGuideActive = 0;
    area->overlayFadeInFrames = 0;
    area->nextArea = -1;
    area->skipFade = 0;
    area->attritionDistance = 100.0f;
    area->movementSoundActive = 0;
    area->colorEffectSuppressed = 0;
    area->unkB4 = 0.0f;
    area->cameraTilt = 0.0f;
    area->frameTravelDistance = 0.0f;
    area->nextFloor = -1;
    area->unkD4 = 1;
    D_00389988[0] = 0;
    D_00389988[1] = 0;
    D_00389988[2] = 0;
    D_00389988[3] = 0;
    D_00389988[4] = 0;
    D_00389988[5] = 0;
    D_00389988[11] = 0;
    D_00389988[12] = 0;
    D_00389988[13] = 0;
    D_00389988[14] = 0;
    D_00389988[16] = 0;
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

extern void fldActivateObjectById(s32);

extern void ptyReturnOne(s32);

void fldActivateFlaggedObject(u32 flagIndex) {
    datGameState->activationFlags[(s32)flagIndex >> 3] |= 1 << (flagIndex & 7);
    fldActivateObjectById(flagIndex);
    if (flagIndex >= 0xF0 && flagIndex < 0x100) {
        mdlFlagSet(flagIndex + 0x610);
    }
    if (D_003A8EB0[flagIndex].kind == 2) {
        ptyReturnOne(D_003A8EB0[flagIndex].parameter);
    }
}

u8 fldTestObjectActivationFlag(u32 flagIndex) {
    return (datGameState->activationFlags[(s32)flagIndex >> 3] >> (flagIndex & 7)) & 1;
}

/* Search records 1..511 of the 30-byte coordinate table; return zero on miss. */
s32 fldFindMapLabelIndexByCoordinate(s32 mapId, s32 floorIndex) {
    s32 index;
    for (index = 1; index < FIELD_COORDINATE_SCAN_LIMIT; index++) {
        if (mapId == D_0039A5A8[index].x && floorIndex == D_0039A5A8[index].y) {
            return index;
        }
    }
    return 0;
}

/* Search the second coordinate table (34-byte records), reserving index zero. */
s32 fldFindSecondaryMapLabelIndexByCoordinate(s32 mapId, s32 floorIndex) {
    s32 index;
    for (index = 1; index < FIELD_COORDINATE_SCAN_LIMIT; index++) {
        if (mapId == D_0039E1A8[index].x && floorIndex == D_0039E1A8[index].y) {
            return index;
        }
    }
    return 0;
}

/* Search the 28-byte map records, skipping row zero; zero denotes a miss. */
s32 fldFindMapCoordinateIndex(s32 mapId, s32 floorIndex) {
    u8 *records = D_003A25A8;
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
    u8 *records = (u8 *)D_00387D70;
    u8 *yColumn = records + 2;
    s32 index = 1;
    do {
        s32 offset = index * 8;
        index++;
        if (area == *(s16 *)(offset + (s32)records) && locationIndex == *(s16 *)(offset + (s32)yColumn)) {
            return (s16 *)(offset + (s32)records);
        }
    } while (index < FIELD_LOCATION_COORDINATE_SCAN_LIMIT);
    return D_00387D70;
}

/* Read the matching location record's fourth signed halfword; zero on miss. */
s32 fldGetLocationCoordinateValue(s32 area, s32 locationIndex) {
    u8 *records = (u8 *)D_00387D70;
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
    FieldStageCoordinate *record = D_00389170;
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

/* Return the area's base row plus rows preceding the matching stage record.
 * A missing coordinate pair returns zero, unlike the index lookup's sentinel. */
u32 fldFindStageCoordinateRowOffset(s32 x, s32 y) {
    FieldStageCoordinate *records = D_00389170;
    s32 rowOffset = 0;
    s32 index = 0;
    do {
        if (records[index].x == x) {
            if (records[index].y == y) {
                return D_003899F0[x] + rowOffset;
            }
            rowOffset += records[index].rows;
        }
        index++;
    } while (index < FIELD_STAGE_INDEX_SCAN_LIMIT);
    return 0;
}

/* This pointer lookup searches only the first 96 stage entries; NULL on miss. */
FieldStageCoordinate *fldFindStageCoordinateRecord(s32 x, s32 y) {
    FieldStageCoordinate *record = D_00389170;
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
    record = fldFindStageCoordinateRecord(D_00389780[0], floor);
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

extern void fldGetSceneEntryPosition(s32 index, f32 *x, f32 *z);

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
    u32 rowOffset;
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
    fldFindStageCoordinateIndex(area->area, stage);
    rowOffset = fldFindStageCoordinateRowOffset(area->area, stage);
    for (i = 0; i < 9; i++) {
        neighborX = cellX + neighbors[i][0];
        neighborZ = -(cellZ + neighbors[i][1]);
        bit = neighborX % 8;
        column = neighborX / 8;
        if (column >= 0 && neighborZ >= 0 && column < record->cols && neighborZ < record->rows) {
            datGameState->stageCellBits[(neighborZ + rowOffset) * 10 + column] |= 1 << bit;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0011F208", fldMarkStageExplorationBitmapRegion);

s32 fldIsStageExplorationCellMarked(s32 floor, f32 x, f32 z) {
    FieldStageCoordinate *record = fldFindStageCoordinateRecord(fldAreaState.area, floor);
    s32 cellX;
    s32 row;
    s32 byte;
    s32 bit;
    u32 rowOffset;

    if (record == NULL) {
        return 0;
    }
    cellX = (s32)(x - record->originX) / record->cellSize;
    row = -((s32)(z - record->originZ) / record->cellSize);
    bit = cellX % 8;
    byte = cellX / 8;
    fldFindStageCoordinateIndex(fldAreaState.area, floor);
    rowOffset = fldFindStageCoordinateRowOffset(fldAreaState.area, floor);
    if (byte >= 0 && row >= 0 && byte < record->cols && row < record->rows) {
        if ((datGameState->stageCellBits[(row + rowOffset) * 10 + byte] >> bit) & 1) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435EB8);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435EC0);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435EC8);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435ED0);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435ED8);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435EE0);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435EE4);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435EE8);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435EEC);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435EF0);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435EF4);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435EF8);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435EFC);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F00);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F04);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F08);

INCLUDE_SDATA(const s32, "game/code_0011F208", fldPlayerObject);

INCLUDE_SDATA(const s32, "game/code_0011F208", fldSecondarySceneObject);

INCLUDE_SDATA(const s32, "game/code_0011F208", fldPlayerModelContext);

INCLUDE_SDATA(const s32, "game/code_0011F208", fldSecondarySceneModelContext);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F1C);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F20);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F24);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F28);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F30);

INCLUDE_SDATA(const s32, "game/code_0011F208", fldPlayerModelResource);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F38);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F3C);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F40);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F44);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F48);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F50);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F58);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F60);

INCLUDE_SDATA(const s32, "game/code_0011F208", fldDeferredCommand);

INCLUDE_SDATA(const s32, "game/code_0011F208", fldDeferredCommandParameter);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F70);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F74);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F78);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F7C);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F80);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F84);

INCLUDE_SDATA(const s32, "game/code_0011F208", fldSceneLifecycleFlags);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F8C);

INCLUDE_SDATA(const s32, "game/code_0011F208", fldSceneControlFlags);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F98);

