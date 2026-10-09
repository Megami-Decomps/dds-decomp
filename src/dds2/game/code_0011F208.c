#include "fld_area_work.h"
#include "fld_label_rows.h"
#include "sdf_packet_list.h"
#include "common.h"
#include "dds3_path.h"
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
#include "dat_state.h"
#include "kwln_task_lifecycle.h"

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

extern void *dds3GetWorldSecondaryObject(void);




extern s32 sdfAllocPacketAligned(s32 size);

extern void func_00122E50(void);

extern void func_001512D8(void);

extern FieldStageCoordinate D_00389170[];

extern u32 fldGetSceneReadyFlag(void);



extern u8 D_003A25A8[];

extern s16 D_00387D70[];

struct EffWorldNode;

extern void fldStepValueByPad(f32 *value, u8 *padState, f32 min, f32 max, f32 step, f32 bigStep);

extern u8 D_0037F530[];

extern void fldStepIntByPad(void *, s32, s64, s64, s64, s64, s8 *);

extern s32 fldStepColorChannelByPad(u32 *, s32, s8 *);

extern s32 fldTestDrawUpdate(KwlnTask *task);

extern s32 fldGetEncounterRuntimeResult(void);

extern u8 fldGetCampSceneControlMode(void);

extern void fldSubmitFrameQuad(s32, s32, s32, s32, s32, s32, s32, s32);

extern void func_0012BE18(s32);

extern void func_00136EF8(void);

extern void func_00134A18(void);

extern void func_0012D3E0(void);

extern u32 D_00389988[];

extern void fldSelectDisplayBuffer(s32);

extern void fldSelectDisplayBuffer(s32);

extern u32 D_00389988[];

extern u32 D_00389988[];

extern char D_00412B90[];

extern KwlnTask *kwlnTaskCreate(const char *name, u32 priority, s32 startDelay, s32 destroyDelay, TaskUpdate update, TaskDestroy destroy, u32 userValue);

extern s32 D_00435EE0;

extern s32 D_00435EE4;

extern s32 D_00435EE8;

extern s32 D_00435EEC;

extern s32 D_00435EF0;

extern s32 D_00435EF4;

extern s32 D_00435EF8;

extern void fldSetCameraMoveMode(s32 mode);

extern void fldClearCameraMoveMode(void);


extern u8 dds3TestObjectFlags(void *object, s32 mask);


extern void fldSubmitBackgroundResourcePacket(void);

extern void fldSubmitBackgroundDescriptorPacket(void);

extern s32 D_00389780[];

extern void mdlFlagSet(s32);

void fldSetPacketArgumentPair(u32 *packet, u32 first, u32 second) {
    packet[4] = first;
    packet[5] = second;
}

struct SdfListHead *sdfCreateResetPacketList(void) {
    struct SdfListHead *packet = (struct SdfListHead *)sdfAllocPacketAligned(0x20);

    sdfInitPacketList(packet);
    return packet;
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_0011F250);

void *func_0011F3D8(s32 x0, s32 y0, s32 z0, s32 color0, s32 x1, s32 y1, s32 z1, s32 color1, s32 flags) {
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
void fldStepValueByPad(f32 *value, u8 *padState, f32 min, f32 max, f32 step, f32 bigStep) {
    f32 adjustedValue = *value;

    if ((s8)padState[5] & FIELD_EDIT_WRAP_INPUT) {
        adjustedValue += bigStep;
        if (max < adjustedValue) {
            adjustedValue = min;
        }
    } else if (padState[5] & FIELD_EDIT_CLAMP_INPUT) {
        adjustedValue += bigStep;
        if (max < adjustedValue) {
            adjustedValue = max;
        }
    } else if ((s8)padState[4] & FIELD_EDIT_WRAP_INPUT) {
        adjustedValue -= bigStep;
        if (adjustedValue < min) {
            adjustedValue = max;
        }
    } else if (padState[4] & FIELD_EDIT_CLAMP_INPUT) {
        adjustedValue -= bigStep;
        if (adjustedValue < min) {
            adjustedValue = min;
        }
    } else if ((s8)padState[7] & FIELD_EDIT_WRAP_INPUT) {
        adjustedValue += step;
        if (max < adjustedValue) {
            adjustedValue = min;
        }
    } else if (padState[7] & FIELD_EDIT_CLAMP_INPUT) {
        adjustedValue += step;
        if (max < adjustedValue) {
            adjustedValue = max;
        }
    } else if ((s8)padState[6] & FIELD_EDIT_WRAP_INPUT) {
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
    *value = adjustedValue;
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
void fldFormatSecondsText(f32 value, char *text) {
    s32 whole;
    s32 tenths;
    s32 hundredths;
    value += 0.005f;
    whole = (s32)value;
    if (whole >= 10) {
        tenths = 9;
        hundredths = 9;
    } else {
        value -= whole;
        value *= 10.0f;
        tenths = (s32)value;
        value -= tenths;
        hundredths = (s32)(value * 10.0f);
    }
    text[0] = whole + '0';
    text[1] = '.';
    text[2] = tenths + '0';
    text[3] = hundredths + '0';
    text[4] = 0;
}

extern void *func_0011F250(s32, s32, s32, s32, s32, u32, u32);

extern void *sdfCreateFormattedSifCommand(s32, s32, s32, s32, const char *, ...);

extern u8 D_00386480[];

extern char *D_00386488[];

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
    sdfAppendPacket(packetList, (u32)func_0011F250(x + 0x510, y + 0x18,
                     0xFF0080, 0x240, 0xF0, color, 0x40806020));
    for (i = 0; i != 3; i++) {
        sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(x, y,
                        0xFF0080, D_00386480[i], D_00386488[i]));
        style = selected == i ? 6 : 0;
        if (values == NULL) {
            text[0] = '-';
            text[1] = '-';
            text[2] = '-';
            text[3] = '-';
            text[4] = 0;
        } else {
            fldFormatSecondsText(values[i], text);
        }
        sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(x + 0x180,
                        y, 0xFF0080, style, text));
        y += 0x60;
    }
}

extern char D_00435EB8[];

extern char D_00435EC0[];

extern char D_00435EC8[];

extern char D_00435ED0[];

extern char D_00435ED8[];

void fldDrawPackedRgbEditor(void *packetList, s32 x, s32 y, s32 selected,
                   u32 color, s32 showNormalized) {
    s32 i;
    s32 style;
    s32 channel;
    const char *label;

    color = (color & 0xFFFFFF) | 0x80000000;
    sdfAppendPacket(packetList, (u32)func_0011F250(x + 0x510, y + 0x18,
                    0xFF0080, 0x240, 0xF0, color, 0x40806020));
    for (i = 0; i != 3; i++) {
        switch (i) {
        case 0:
            style = 2;
            label = D_00435EC8;
            channel = color & 0xFF;
            break;
        case 1:
            style = 4;
            label = D_00435EC0;
            channel = (color >> 8) & 0xFF;
            break;
        default:
            style = 5;
            label = D_00435EB8;
            channel = (color >> 16) & 0xFF;
            break;
        }
        sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(
                        x, y, 0xFF0080, style, label));
        sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(
                        x + 0x180, y, 0xFF0080, i == selected ? 6 : 0,
                        D_00435ED0, channel));
        if (showNormalized != 0) {
            sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(
                            x, y + 0x180, 0xFF0080, style, label));
            sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(
                            x + 0x240, y + 0x180, 0xFF0080, 0,
                            D_00435ED8, channel * (1.0f / 255.0f)));
        }
        y += 0x60;
    }
}

u32 func_001200E0(void) {
    return 0;
}

extern f32 sdfSinPoly(f32);

extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);

extern void fldSubmitGsTriangle(s32, s32, s32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

/* Filled disc in the field; radius shrinks in the close-up areas. */
void func_001200E8(f32 x, f32 y, f32 z, f32 radius, s32 fade) {
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
    func_0012BE18(5);
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
    func_0012BE18(0);
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
    func_0012BE18(0);
    func_00136EF8();
    func_00134A18();
    if (D_00389988[9] != 0) {
        if (fldGetSceneReadyFlag() == 0) {
            fldSelectDisplayBuffer(0x53);
        } else {
            fldSelectDisplayBuffer(0x5E);
        }
        func_0012D3E0();
    }
    fldSelectDisplayBuffer(0x27);
    fldSubmitFrameQuad(1, 5, 0x80, 1, 0, 0, 1, 2);
    func_0012BE18(0);
    fldSelectDisplayBuffer(0x39);
    fldSubmitFrameQuad(1, 5, 0x80, 1, 0, 0, 1, 2);
    func_0012BE18(0);
    return 0;
}

void fldTestDrawCreate(void) {
    kwlnTaskCreate(D_00412B90, 0x2AF8, 0, 0, fldTestDrawUpdate, NULL, 0);
}

void fldTestDrawDestroy(void) {
    kwlnTaskDestroyWithHierarchyByName(D_00412B90, 1);
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_00120528);

INCLUDE_ASM(const s32, "game/code_0011F208", func_001206D0);

enum {
    FIELD_CAMERA_TRACKING_UNSET = -999
};

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

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412BB8);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00120B88);

typedef struct FieldActivationRecord {
    s32 kind;
    s16 parameter;
    u8 pad06[0xA];
} FieldActivationRecord;

extern FieldActivationRecord D_003A8EB0[];

extern u8 D_0039A1D0[], D_003A41A8[], D_003A47E8[], D_003A55F0[], D_0038A3B8[], D_0038A480[], D_0038A9B0[], fldCameraFollowRows[], D_00389A70[], D_00391FA0[];

extern u32 D_003899F0[];

extern void *fldCameraSettings;

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
    sdfDevQueueReadAndWait(command, D_00391FA0, 0x1200);
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

INCLUDE_ASM(const s32, "game/code_0011F208", func_00122A38);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00122B58);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00122E50);

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

/* Store a byte at valueOffset within the selected slot's values area. */
void fldSetMapSlotByte(s32 mapId, u32 slotIndex, s32 valueOffset, s32 value) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        datGameState->maps[mapIndex].slots[slotIndex].values[valueOffset] = value;
    }
}

/* Read a slot value, translating stored 0xff to -1; IDs >= the limit return zero. */
s32 fldGetMapSlotByte(s32 mapId, u32 slotIndex, s32 valueOffset) {
    u8 value = 0;
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        value = datGameState->maps[mapIndex].slots[slotIndex].values[valueOffset];
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

extern void func_0011C6A0(s32);

void fldActivateFlaggedObject(u32 flagIndex) {
    datGameState->activationFlags[(s32)flagIndex >> 3] |= 1 << (flagIndex & 7);
    fldActivateObjectById(flagIndex);
    if (flagIndex >= 0xF0 && flagIndex < 0x100) {
        mdlFlagSet(flagIndex + 0x610);
    }
    if (D_003A8EB0[flagIndex].kind == 2) {
        func_0011C6A0(D_003A8EB0[flagIndex].parameter);
    }
}

u8 fldTestObjectActivationFlag(u32 flagIndex) {
    return (datGameState->activationFlags[(s32)flagIndex >> 3] >> (flagIndex & 7)) & 1;
}

/* Search records 1..511 of the 30-byte coordinate table; return zero on miss. */
s32 func_001237B0(s32 x, s32 y) {
    s32 index;
    for (index = 1; index < FIELD_COORDINATE_SCAN_LIMIT; index++) {
        if (x == D_0039A5A8[index].x && y == D_0039A5A8[index].y) {
            return index;
        }
    }
    return 0;
}

/* Search the second coordinate table (34-byte records), reserving index zero. */
s32 func_00123808(s32 x, s32 y) {
    s32 index;
    for (index = 1; index < FIELD_COORDINATE_SCAN_LIMIT; index++) {
        if (x == D_0039E1A8[index].x && y == D_0039E1A8[index].y) {
            return index;
        }
    }
    return 0;
}

/* Search the 28-byte map records, skipping row zero; zero denotes a miss. */
s32 fldFindMapCoordinateIndex(s32 x, s32 y) {
    u8 *records = D_003A25A8;
    u8 *yColumn = records + 2;
    s32 index = 1;
    s32 offset = 28;
    do {
        if (x == *(s16 *)(offset + (s32)records) && y == *(s16 *)(offset + (s32)yColumn)) {
            return index;
        }
        index++;
        offset += 28;
    } while (index < FIELD_MAP_COORDINATE_SCAN_LIMIT);
    return 0;
}

/* Search location records 1..639; a miss returns the reserved first record,
 * not NULL, so callers can read the fallback record's fields. */
s16 *fldFindLocationCoordinateRecord(s32 x, s32 y) {
    u8 *records = (u8 *)D_00387D70;
    u8 *yColumn = records + 2;
    s32 index = 1;
    do {
        s32 offset = index * 8;
        index++;
        if (x == *(s16 *)(offset + (s32)records) && y == *(s16 *)(offset + (s32)yColumn)) {
            return (s16 *)(offset + (s32)records);
        }
    } while (index < FIELD_LOCATION_COORDINATE_SCAN_LIMIT);
    return D_00387D70;
}

/* Read the matching location record's fourth signed halfword; zero on miss. */
s32 fldGetLocationCoordinateValue(s32 x, s32 y) {
    u8 *records = (u8 *)D_00387D70;
    u8 *yColumn = records + 2;
    u8 *valueColumn = records + 6;
    s32 index = 1;
    do {
        s32 offset = index * 8;
        index++;
        if (x == *(s16 *)(offset + (s32)records) && y == *(s16 *)(offset + (s32)yColumn)) {
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
s32 func_00123A58(s32 scene, s32 floor, f32 xOffset, f32 zOffset,
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

void func_00123B88(s32 room, s32 stage, f32 x, f32 z, f32 unused) {
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
            datGameState->pad11130[(neighborZ + rowOffset) * 10 + column] |= 1 << bit;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_00123DE8);

s32 func_00123EE0(s32 floor, f32 x, f32 z) {
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
        if ((datGameState->pad11130[(row + rowOffset) * 10 + byte] >> bit) & 1) {
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

INCLUDE_SDATA(const s32, "game/code_0011F208", fldCameraModelObject);

INCLUDE_SDATA(const s32, "game/code_0011F208", fldSecondarySceneModelHandle);

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

