#include "sdk_ps2.h"
#include "fld_integer_editor_api.h"
#include "fld_area_work.h"
#include "fld_camera_move_tracking.h"
#include "fld_runtime_reset_api.h"
#include "fld_npc_motion.h"
#include "fld_label_rows.h"
#include "sdf_packet_list.h"
#include "common.h"
#include "sdf_vu0.h"
#include "fld_scene_ready_api.h"
#include "fld_camp_check_state_api.h"
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
#include "sdf_vu_matrix.h"
#include "fpu.h"
#include "fld.h"
#include "evt_world.h"
#include "dds3obj.h"
#include "dds3_world_primary.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "dat_state.h"
#include "kwln_task_lifecycle.h"
/* Declared before first use (were implicit int). */
extern s32 fldDoor_ElvTakasaToFloor(s32 index);
extern void fldResetViewState(void);

/* Signed selectors read signed storage; all writes retain the selected width. */
enum {
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

extern void fldRunDanteClear(void);

extern FieldStageCoordinate fldStageCoordinateRecords[];




extern u8 D_003A25A8[];

extern s16 D_00387D70[];

struct dds3Basic_t;

extern void fldStepValueByPad(f32 *controlledValue, u8 *padState, f32 min, f32 max, f32 step, f32 bigStep);

extern u8 D_0037F530[];


extern s32 fldStepColorChannelByPad(u32 *, s32, s8 *);

extern s32 fldTestDrawUpdate(dds3ProcessID_t *task);


extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);

extern void rsetAlpha(s32);

extern void fldDrawCameraColorScrollingGradient(void);

extern void fldSkyDrawFilter(void);

extern void fldSubmitTexturedOverlaySprite(void);

extern u32 fldRunDanteGunFlag[];

extern void rsetOtOfs(u32);

extern void rsetOtOfs(u32);

extern u32 fldRunDanteGunFlag[];

extern u32 fldRunDanteGunFlag[];

extern char D_00412B90[];


extern s32 gFldElvFloorNow;

extern s32 fldElvFloorFrom;

extern s32 fldElvFloorNum;

extern s32 fldElvFloorTo;

extern s32 fldElvFloorDisp;

extern s32 fldElvFloorTarget;

extern s32 fldElvState;

extern void fldSetElvCameraScene(s32 mode);

extern void fldClearCameraMoveMode(void);


extern void fldDrwZSave(void);

extern void fldDrwZLoad(void);

extern s32 D_00389780[];


void fldSetPacketArgumentPair(u32 *packet, u32 first, u32 second) {
    packet[4] = first;
    packet[5] = second;
}

struct sdfDrawTag_t *dds3FontReqAllocDrawTag(void) {
    struct sdfDrawTag_t *packet = (struct sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);

    sdfDrawTagInit(packet);
    return packet;
}

/* Build the 0x80-byte bordered rectangle packet: the rectangle with its two color words, then its outline corners. */
u64 *dds3PrimFrameRect(s32 x, s32 y, s32 depth, s32 width, s32 height, u32 colorA, u32 colorB) {
    u64 *packet;
    s32 right;
    s32 bottom;
    u64 depthHigh;

    packet = (u64 *)sdfGrAllocPacketBuff(0x80);
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

u64 *dds3PrimLine(s32 x0, s32 y0, s32 z0, s32 color0, s32 x1, s32 y1, s32 z1, s32 color1, s32 flags) {
    u64 *packet = (u64 *)sdfGrAllocPacketBuff(0x50);

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

extern f32 D_0040B540[4];
extern u8 sdfViewMatrix[0x40];
extern const f32 D_00386370[13][4];
extern const f32 D_00386470[4];
extern const u8 D_00386440[20][2];

/* Build the light-direction gizmo packet: 20 GS lines over 13 transformed outline points. */
u64 *dds3PrimLine3D(s32 x, s32 y, s32 z, s32 highlighted) {
    u64 xyz[13];
    u64 color[13];
    volatile f32 axis[4] __attribute__((aligned(16)));
    volatile s32 point[4] __attribute__((aligned(16)));
    u64 *packet;
    u64 *endColor;
    u64 *endXyz;
    const f32 (*outline)[4];
    const u8 (*pair)[2];
    s32 i;
    s32 shade;

    SDF_VU0_NORMALIZE();
    SDF_VU0_VECTOR_W_SET_ZERO();
    SDF_VU0_SET_ROW2();
    SDF_VU0_STOREVECTOR(axis);
    if (ffabsf(axis[0]) < 0.000001f && ffabsf(axis[2]) < 0.000001f) {
        axis[2] = -axis[1];
        axis[0] = 0.0f;
        axis[1] = 0.0f;
        SDF_VU0_LOADVECTOR_B(axis);
    } else {
        SDF_VU0_LOADVECTOR_B(D_0040B540);
    }
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_VECTOR_W_SET_ZERO();
    SDF_VU0_SET_ROW0();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_GET_ROW2();
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_VECTOR_W_SET_ZERO();
    SDF_VU0_SET_ROW1();
    SDF_VU0_RESET_ROW3();
    sdfPostmultiplyVuMatrixFromMemory((const struct Matrix4 *)sdfViewMatrix);
    SDF_VU0_LOADVECTOR(D_00386470);
    SDF_VU0_SCALE_XYZ_REV();
    axis[0] = (f32)x;
    axis[1] = (f32)y;
    axis[2] = 160.0f;
    SDF_VU0_LOAD_ROW3(axis);

    shade = highlighted ? 0x101 : 0x10101;
    outline = D_00386370;
    for (i = 0; i != 13; i++) {
        SDF_VU0_LOADVECTOR(outline[i]);
        SDF_VU0_APPLYMATRIX_XYZ1();
        SDF_VU0_FTOI0();
        SDF_VU0_STOREVECTOR(point);
        xyz[i] = (u32)((point[0] & 0xFFFF) | (point[1] << 16)) | ((u64)z << 32);
        color[i] = (u32)(shade * point[2] | 0x80000000) | ((u64)0x3F800000 << 32);
    }

    packet = (u64 *)sdfGrAllocPacketBuff(0x2C0);
    packet[0] = 0x2B;
    packet[1] = 0x5000002B00000000ULL;
    packet[2] = 0x1400000000000001ULL;
    packet[3] = 0;
    packet[4] = 0x189;
    packet[5] = 0;
    packet[6] = 0x4400000000008014ULL;
    packet[7] = 0x5151;
    endColor = packet + 10;
    endXyz = packet + 11;
    pair = D_00386440;
    for (i = 0; i != 20; i++) {
        s32 index = pair[i][0];

        endColor[i * 4 - 2] = color[index];
        endXyz[i * 4 - 2] = xyz[index];
        index = pair[i][1];
        endColor[i * 4] = color[index];
        endXyz[i * 4] = xyz[index];
    }
    return packet;
}

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

void dds3DebugFloatEdit(f32 *value, f32 min, f32 max, f32 step, f32 bigStep) {
    fldStepValueByPad(value, D_0037F530, min, max, step, bigStep);
}

/* Read the selected signed/unsigned integer, apply pad stepping, then truncate
 * back to its original width. Unsupported storage selectors leave it untouched. */
void fldStepIntByPad(void *valueAddress, FldIntegerStorageType storageType, s64 min, s64 max, s64 step,
                     s64 bigStep, s8 *padState) {
    s64 adjustedValue;
    switch (storageType) {
    case FLD_INTEGER_STORAGE_UNSIGNED_BYTE:
        adjustedValue = *(u8 *)valueAddress;
        break;
    case FLD_INTEGER_STORAGE_UNSIGNED_HALFWORD:
        adjustedValue = *(u16 *)valueAddress;
        break;
    case FLD_INTEGER_STORAGE_UNSIGNED_WORD:
        adjustedValue = *(u32 *)valueAddress;
        break;
    case FLD_INTEGER_STORAGE_SIGNED_BYTE:
        adjustedValue = *(s8 *)valueAddress;
        break;
    case FLD_INTEGER_STORAGE_SIGNED_HALFWORD:
        adjustedValue = *(s16 *)valueAddress;
        break;
    case FLD_INTEGER_STORAGE_SIGNED_WORD:
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
    case FLD_INTEGER_STORAGE_SIGNED_BYTE:
    case FLD_INTEGER_STORAGE_UNSIGNED_BYTE:
        *(u8 *)valueAddress = adjustedValue;
        break;
    case FLD_INTEGER_STORAGE_SIGNED_HALFWORD:
    case FLD_INTEGER_STORAGE_UNSIGNED_HALFWORD:
        *(u16 *)valueAddress = adjustedValue;
        break;
    case FLD_INTEGER_STORAGE_SIGNED_WORD:
    case FLD_INTEGER_STORAGE_UNSIGNED_WORD:
        *(u32 *)valueAddress = adjustedValue;
        break;
    }
}

void dds3DebugIntEdit(void *valueAddress, FldIntegerStorageType storageType, s64 min, s64 max, s64 step,
                      s64 bigStep) {
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

void dds3DebugColorEdit(u32 *color, s32 channel) {
    fldStepColorChannelByPad(color, channel, (s8 *)D_0037F530);
}

/* Emit one whole-value character and two fractional digits after rounding.
 * At whole >= 10 only the fractional digits become 9; whole is not clamped. */
void dds3MenuStepValue(f32 displayValue, char *text) {
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

extern u64 *dds3PrimFrameRect(s32, s32, s32, s32, s32, u32, u32);

extern void *sdfDevPrintf(s32, s32, s32, s32, const char *, ...);

extern u8 D_00386480[];

extern char *D_00386488[];

/* Draw three RGB value rows and their clamped colour preview. */
void dds3PrimDebugTextBox(sdfDrawTag_t *packetList, s32 x, s32 y, s32 selectedChannel, f32 *rgbValues) {
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
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(x + 0x510, y + 0x18,
                     0xFF0080, 0x240, 0xF0, previewColor, 0x40806020));
    for (channelIndex = 0; channelIndex != 3; channelIndex++) {
        sdfDrawTagLink(packetList, (u32)sdfDevPrintf(x, y,
                        0xFF0080, D_00386480[channelIndex], D_00386488[channelIndex]));
        selectedStyle = selectedChannel == channelIndex ? 6 : 0;
        if (rgbValues == NULL) {
            text[0] = '-';
            text[1] = '-';
            text[2] = '-';
            text[3] = '-';
            text[4] = 0;
        } else {
            dds3MenuStepValue(rgbValues[channelIndex], text);
        }
        sdfDrawTagLink(packetList, (u32)sdfDevPrintf(x + 0x180,
                        y, 0xFF0080, selectedStyle, text));
        y += 0x60;
    }
}

extern char D_00435EB8[];

extern char D_00435EC0[];

extern char D_00435EC8[];

extern char D_00435ED0[];

extern char D_00435ED8[];

void dds3PrimDebugTextBoxRGB(sdfDrawTag_t *packetList, s32 x, s32 y, s32 selectedChannel,
                   u32 packedColor, s32 showNormalizedValues) {
    s32 channelIndex;
    s32 selectedStyle;
    s32 channelValue;
    const char *label;

    packedColor = (packedColor & 0xFFFFFF) | 0x80000000;
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(x + 0x510, y + 0x18,
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
        sdfDrawTagLink(packetList, (u32)sdfDevPrintf(
                        x, y, 0xFF0080, selectedStyle, label));
        sdfDrawTagLink(packetList, (u32)sdfDevPrintf(
                        x + 0x180, y, 0xFF0080, channelIndex == selectedChannel ? 6 : 0,
                        D_00435ED0, channelValue));
        if (showNormalizedValues != 0) {
            sdfDrawTagLink(packetList, (u32)sdfDevPrintf(
                            x, y + 0x180, 0xFF0080, selectedStyle, label));
            sdfDrawTagLink(packetList, (u32)sdfDevPrintf(
                            x + 0x240, y + 0x180, 0xFF0080, 0,
                            D_00435ED8, channelValue * (1.0f / 255.0f)));
        }
        y += 0x60;
    }
}

u32 dds3NullDrawProc(void) {
    return 0;
}



extern void fldDrawPoly3(s32 color0, s32 color1, s32 color2,
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

    if ((u32)(fldGlobalBase.area - 0xC9) < 0x12B) {
        y = 0.0f;
    } else {
        radius *= 0.7f;
    }
    if (fade == 0) {
        return;
    }
    if ((u32)(fldGlobalBase.area - 0xC9) < 0x12B) {
        rsetOtOfs(0x20);
    } else {
        rsetOtOfs(0x39);
    }
    rsetAlpha(5);
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
        sinA = sdfSin(a) * radius;
        cosA = sdfCos(a) * radius;
        b = (f32)prev * 3.14f / 180.0f;
        sinB = sdfSin(b) * radius;
        cosB = sdfCos(b) * radius;
        fldDrawPoly3(color, 0x80101010, 0x80101010,
                            x + 0.0f, y - 2.0f, z + 0.0f,
                            x + sinA, y - 2.0f, z + cosA,
                            x + sinB, y - 2.0f, z + cosB);
    }
    rsetAlpha(0);
}

s32 fldTestDrawUpdate(dds3ProcessID_t *task) {
    if (fldGetEncounterRuntimeResult() != 0) {
        return 0;
    }
    if (fldProcCampChk() == FLD_CAMP_CHECK_STATE_MENU_ACTIVE) {
        return 0;
    }
    rsetOtOfs(0x53);
    rsetTest(1, 0, 0x81, 3, 0, 0, 1, 1);
    rsetAlpha(0);
    fldDrawCameraColorScrollingGradient();
    fldSkyDrawFilter();
    if (fldRunDanteGunFlag[9] != 0) {
        if (fldGetSceneReadyFlag() == FLD_SCENE_NOT_READY) {
            rsetOtOfs(0x53);
        } else {
            rsetOtOfs(0x5E);
        }
        fldSubmitTexturedOverlaySprite();
    }
    rsetOtOfs(0x27);
    rsetTest(1, 5, 0x80, 1, 0, 0, 1, 2);
    rsetAlpha(0);
    rsetOtOfs(0x39);
    rsetTest(1, 5, 0x80, 1, 0, 0, 1, 2);
    rsetAlpha(0);
    return 0;
}

void InitOoyTest(void) {
    dds3StartProcess(D_00412B90, 0x2AF8, 0, 0, fldTestDrawUpdate, NULL, 0);
}

void KillOoyTest(void) {
    dds3KillProcessName(D_00412B90, 1);
}

extern void frSetFontDrawOT(u32);
extern u8 D_00387C20[][4];
extern void fldKanjiDraw(s32, s32, u32, const u8 *);
static inline void fldDrawPercentGlyph(s32 x, s32 y, u32 color, s32 glyph) {
    fldKanjiDraw(x, y / 2, color, D_00387C20[glyph]);
}

void fldDrawPercentValue(s32 x, s32 y, s32 value) {
    s32 negative = 0;
    s32 digit;

    frSetFontDrawOT(0x54);

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

void fldElvSetFloor(s32 start, s32 end) {
    s32 first;
    s32 endEntry;

    first = fldDoor_ElvTakasaToFloor(start);
    fldElvState = 0;
    fldElvFloorFrom = first;
    if (first > 1000) {
        gFldElvFloorNow = first % 1000;
    } else {
        gFldElvFloorNow = first;
    }
    if (first > 1000) {
        fldElvFloorNum = first;
        fldElvFloorFrom = first % 1000;
    } else {
        fldElvFloorNum = -999;
    }

    endEntry = fldDoor_ElvTakasaToFloor(end);
    fldElvFloorTo = endEntry;
    if (endEntry > 1000) {
        fldElvFloorDisp = endEntry;
        fldElvFloorTo = endEntry % 1000;
    } else {
        fldElvFloorDisp = -999;
    }
    if (start == end) {
        fldElvFloorTarget = -999;
    } else {
        fldElvFloorTarget = 0;
    }
    if (gFldElvFloorNow != -999 && fldElvFloorTarget != -999) {
        fldElvFloorTarget++;
        if (fldElvFloorFrom < fldElvFloorTo) {
            fldSetElvCameraScene(1);
        } else {
            fldSetElvCameraScene(-1);
        }
    }
}

/* Clear the move mode and invalidate all six stored tracking values. */
void fldResetCameraMoveTracking(void) {
    fldElvState = 0;
    gFldElvFloorNow = FIELD_CAMERA_TRACKING_UNSET;
    fldElvFloorFrom = FIELD_CAMERA_TRACKING_UNSET;
    fldElvFloorNum = FIELD_CAMERA_TRACKING_UNSET;
    fldElvFloorTo = FIELD_CAMERA_TRACKING_UNSET;
    fldElvFloorDisp = FIELD_CAMERA_TRACKING_UNSET;
    fldElvFloorTarget = FIELD_CAMERA_TRACKING_UNSET;
    fldClearCameraMoveMode();
}

/* Report camera-tracking state and start a move when its target is reached. */
FldCameraTrackingResult fldEvaluateCameraMoveTracking(void) {
    if (fldElvState == 1) {
        return FLD_CAMERA_TRACKING_MODE_PENDING;
    }
    if (fldElvFloorTarget != FIELD_CAMERA_TRACKING_UNSET && gFldElvFloorNow == fldElvFloorTo) {
        if (fldElvFloorFrom < gFldElvFloorNow) {
            fldSetElvCameraScene(2);
        } else {
            fldSetElvCameraScene(-2);
        }
        return FLD_CAMERA_TRACKING_MOVE_STARTED;
    }
    return FLD_CAMERA_TRACKING_NO_ACTION;
}

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412B90);

INCLUDE_ASM(const s32, "game/code_0011F208", fldElvDrawFloorNum);

enum {
    FIELD_BACKGROUND_REQUIRED_OBJECT_FLAG = 0x200,
    FIELD_BACKGROUND_EXCLUDED_OBJECT_FLAG = 1
};

/* Submit both background packets when a world-list-5 object passes the flag checks.
 * The whole value chain is visited and destroyed before the scene override. */
void drawSceneBasicList(void) {
    s32 hasEligibleBackground = 0;
    WorldIndexNode *objectChain;
    dds3Basic_t *worldObject;

    objectChain = dds3SceneBasicGetList(dds3GetControlScene(), 5);
    if (objectChain != 0) {
        if (dds3SetFirstBasicArray((dds3BasicArrayHandle_t *)objectChain) != 0) {
            do {
                worldObject = (dds3Basic_t *)dds3GetBasicArray((dds3BasicArrayHandle_t *)objectChain);
                if (dds3ObjectBaseCheckFlag(worldObject, FIELD_BACKGROUND_REQUIRED_OBJECT_FLAG) != 0) {
                    if (dds3ObjectBaseCheckFlag(worldObject, FIELD_BACKGROUND_EXCLUDED_OBJECT_FLAG) == 0) {
                        hasEligibleBackground = 1;
                    }
                }
            } while (dds3SetNextBasicArray((dds3BasicArrayHandle_t *)objectChain) != 0);
        }
        dds3FreeBasicArrayHandle(objectChain);
    }
    if (D_00389780[0] == 1) {
        hasEligibleBackground = 0;
    }
    if (hasEligibleBackground != 0) {
        rsetOtOfs(0x24);
        fldDrwZSave();
        rsetOtOfs(0x26);
        fldDrwZLoad();
    }
}

#include "fld_shadow_strip.h"
#include "fld_area_work.h"
#include "pcp_vu0.h"
#include "mdl.h"
#include "eff_object.h"
#include "evt_unit.h"
#include "sdf_primitive.h"
#include "sdf_primitive_packet_api.h"
#include "sdf_packet_append.h"
#include "sdf_packet_list.h"
#include "sdf.h"

/* All declarations below refer to the existing primary runtime owners. */
extern evtUnitHandle_t *dds3GetUnitHandleObjectBasic(dds3Basic_t *);
extern dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *, u32, s32);
extern s32 sdfModelGetMarkPos(sdfModelRootNode_t *, s32);
extern void fldMakeEveHitPlanes(const f32 *, f32 *, f32 *, f32 *, f32 *);
extern void aDrawBoxF(s32, s32, s32, s32, u32, u32, u32, u32);
extern void aDrawBoxF_Direct(s32, s32, s32, s32, u32, u32, u32, u32);
extern void fldSubmitPrimaryFramePacket(void);
extern void fldSubmitAlternateFramePacket(void);
extern void fldPrepareProjectionMatrix(void);
extern SdfPoolNode sdfGrOtTable[];
extern u32 rsetFrameMaskOtOffset;
extern u32 D_0043607C;
extern s32 D_00435EFC, D_00435F00, D_00435F04, D_00435F08;
extern f32 D_0043E5E0[4];
extern f32 D_0043E5F0[50][3][4];
extern s8 D_0043EF50[26][26][2];
extern f32 D_0043F4A0[26][26][4];
extern f32 D_00441EE0[26][26][4];
extern f32 aViewTarget[4], sdfViewTargetVector[4];
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
static inline void fldFlushShadowStrip(sdfDrawTag_t *list, SdfPrimitiveRequest *request) {
    sdfDrawTagLink(list, (u32)sdf3DPrimMakePoly(request));
    request->stripWordCount = 0;
    request->vertexCount = 0;
}

static inline SdfPoolNode *fldGetDisplaySurfaceBase(void) {
    return sdfGrOtTable;
}

/* Appends a finished packet list to the active display surface. */
static inline void fldAppendToDisplaySurface(sdfDrawTag_t *list) {
    SdfPoolNode *surface = &fldGetDisplaySurfaceBase()[rsetFrameMaskOtOffset];

    surface->append(surface, list);
}

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412BB8);

void fldDrawEffectObjectShadow(dds3Basic_t *object) {
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
    sdfModelRootNode_t *model;
    dds3ObjectBaseControlWorkData_t *base;
    s32 vertexCount;
    EffectObjectData *data;
    evtUnitHandle_t *transition;
    dds3Basic_t *target;
    sdfModelNode_t *drawNode;
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
    sdfDrawTag_t *quadList;
    sdfDrawTag_t *triangleList;
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
    base = dds3GetBaseControlWorkData(object);
    model = ((dds3ModelHandle_t *)dds3ObjectBaseGetModelHandle(object))->inner;
    if (dds3ObjectBaseCheckFlag(object, 0x4000)) {
        transition = dds3GetUnitHandleObjectBasic(object);
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
    rsetOtOfs(0x25);
    if (fldGlobalBase.area >= 500) {
        transition = dds3GetUnitHandleObjectBasic(object);
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
                target = dds3SearchSceneBasic(dds3GetActiveScene(), data->activeId, 9);
                transition = dds3GetUnitHandleObjectBasic(object);
                if (sdfModelGetMarkPos(transition->owner->inner, 0)) {
                    SDF_VU0_STOREVECTOR_NOMEM(anchorPosition);
                    D_0043E5E0[0] = anchorPosition[0] - target->inner->position[0];
                    D_0043E5E0[1] = anchorPosition[1] - target->inner->position[1];
                    D_0043E5E0[2] = anchorPosition[2] - target->inner->position[2];
                } else {
                    D_0043E5E0[0] = fldGlobalBase.x - target->inner->position[0];
                    /* Retail 0x0011F134..0x0011F15C uses field Z for both Y and Z. */
                    D_0043E5E0[1] = fldGlobalBase.z - target->inner->position[1];
                    D_0043E5E0[2] = fldGlobalBase.z - target->inner->position[2];
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
            if (fldGlobalBase.area < 200) {
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
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(D_0043E5E0);
    /* Retail still stores this full-length vector at11F43C..11F44C;
     * the later per-slot geometry uses slotExtrusion instead. */
    extrusion[0] = D_0043E5E0[0] * length;
    extrusion[1] = D_0043E5E0[1] * length;
    extrusion[2] = D_0043E5E0[2] * length;
    extrusion[3] = 1.0f;
    VU0_LOAD_VF_PTR(vf10, D_0043E5E0);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(D_0043E5E0);
    for (slot = 0; slot < base->resourceSlotCount; slot++) {
        length = extrusionLengths[slot];
        slotExtrusion[slot][0] = D_0043E5E0[0] * length;
        slotExtrusion[slot][1] = D_0043E5E0[1] * length;
        slotExtrusion[slot][2] = D_0043E5E0[2] * length;
        slotExtrusion[slot][3] = 1.0f;
    }
    viewDirection[0] = aViewTarget[0] - sdfViewTargetVector[0];
    viewDirection[1] = aViewTarget[1] - sdfViewTargetVector[1];
    viewDirection[2] = aViewTarget[2] - sdfViewTargetVector[2];
    viewDirection[3] = 1.0f;
    VU0_LOAD_VF_PTR(vf10, viewDirection);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(viewDirection);
    rsetTest(1, 0, 0x80, 1, 0, 0, 1, 1);
    rsetAlpha(1);
    aDrawBoxF(0, 0, 512, 224, 0, 0, 0, 0);

    for (slot = 0; slot < base->resourceSlotCount; slot++) {
        i = base->resourceSlots[slot];
        if (i < 0) continue;
        drawNode = ((sdfModelNode_t **)model->list->buffer)[i];
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
            if (fldGlobalBase.area >= 500) continue;
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
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_STOREVECTOR_NOMEM(firstVertex);
            secondVertex[0] = vertices[triangle[1]][0];
            secondVertex[1] = vertices[triangle[1]][1];
            secondVertex[2] = vertices[triangle[1]][2];
            VU0_LOAD_VF_PTR(vf10, secondVertex);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_STOREVECTOR_NOMEM(secondVertex);
            thirdVertex[0] = vertices[triangle[2]][0];
            thirdVertex[1] = vertices[triangle[2]][1];
            thirdVertex[2] = vertices[triangle[2]][2];
            VU0_LOAD_VF_PTR(vf10, thirdVertex);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_STOREVECTOR_NOMEM(thirdVertex);
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
            fldMakeEveHitPlanes(planeVertices[0], &plane[0], &plane[1], &plane[2], &plane[3]);
            VU0_LOAD_VF_PTR(vf10, plane);
            SDF_VU0_NORMALIZE();
            VU0_LOAD_VF_PTR(vf11, D_0043E5E0);
            SDF_VU0_INNERPRODUCT(facing);
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
                if (fldGlobalBase.area >= 500) {
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
        rsetTest(1, 0, 0x80, 1, 0, 0, 1, 3);
        fldSubmitPrimaryFramePacket();
        for (pass = 0; pass < 2; pass++) {
            quadList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
            sdfDrawTagInit(quadList);
            sdf3DPrimLinkResetTm(quadList, NULL);
            sdf3DPrimLinkMat(quadList, (void *)D_0043607C, NULL);
            memset(&quadRequest, 0, sizeof(quadRequest));
            quadRequest.positions = quadVertices;
            quadRequest.strip = quadStrip.words;
            quadRequest.stripWordCount = 0;
            quadRequest.vertexCount = 0;
            if (pass == 0) quadRequest.color = 0x80000000;
            else quadRequest.color = 0;
            triangleList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
            sdfDrawTagInit(triangleList);
            sdf3DPrimLinkResetTm(triangleList, NULL);
            sdf3DPrimLinkMat(triangleList, (void *)D_0043607C, NULL);
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
                        SDF_VU0_APPLYMATRIX_XYZ1();
                        SDF_VU0_PERSVECTOR();
                        VU0_APPLY_VIEWPORT(vf10, vf10, vf11, vf12);
                        SDF_VU0_STOREVECTOR_NOMEM(projectedStart);
                        if (projectedStart[0] < minimumX) minimumX = projectedStart[0];
                        else if (projectedStart[0] > maximumX) maximumX = projectedStart[0];
                        if (projectedStart[1] < minimumY) minimumY = projectedStart[1];
                        else if (projectedStart[1] > maximumY) maximumY = projectedStart[1];
                        VU0_LOAD_VF_PTR(vf10, edgeEnd);
                        SDF_VU0_APPLYMATRIX_XYZ1();
                        SDF_VU0_PERSVECTOR();
                        VU0_APPLY_VIEWPORT(vf10, vf10, vf11, vf12);
                        SDF_VU0_STOREVECTOR_NOMEM(projectedEnd);
                        if (projectedEnd[0] < minimumX) minimumX = projectedEnd[0];
                        else if (projectedEnd[0] > maximumX) maximumX = projectedEnd[0];
                        if (projectedEnd[1] < minimumY) minimumY = projectedEnd[1];
                        else if (projectedEnd[1] > maximumY) maximumY = projectedEnd[1];
                        VU0_LOAD_VF_PTR(vf10, extrudedEnd);
                        SDF_VU0_APPLYMATRIX_XYZ1();
                        SDF_VU0_PERSVECTOR();
                        VU0_APPLY_VIEWPORT(vf10, vf10, vf11, vf12);
                        SDF_VU0_STOREVECTOR_NOMEM(projectedExtrudedEnd);
                        if (projectedExtrudedEnd[0] < minimumX) minimumX = projectedExtrudedEnd[0];
                        else if (projectedExtrudedEnd[0] > maximumX) maximumX = projectedExtrudedEnd[0];
                        if (projectedExtrudedEnd[1] < minimumY) minimumY = projectedExtrudedEnd[1];
                        else if (projectedExtrudedEnd[1] > maximumY) maximumY = projectedExtrudedEnd[1];
                        VU0_LOAD_VF_PTR(vf10, extrudedStart);
                        SDF_VU0_APPLYMATRIX_XYZ1();
                        SDF_VU0_PERSVECTOR();
                        VU0_APPLY_VIEWPORT(vf10, vf10, vf11, vf12);
                        SDF_VU0_STOREVECTOR_NOMEM(projectedExtrudedStart);
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
        rsetTest(1, 1, 0x80, 1, 1, 1, 1, 3);
        rsetAlpha(0);
        alpha = (s32)((f32)D_00435F00 * opacity / 100.0f);
        if (alpha < 0) alpha = 0;
        aDrawBoxF_Direct((s32)(minimumX * 16.0f), (s32)(minimumY * 16.0f),
                        (s32)(maximumX * 16.0f), (s32)(maximumY * 16.0f),
                        0, 0, 0, alpha);
    }
    rsetTest(1, 5, 0x80, 3, 0, 0, 1, 2);
    rsetAlpha(0);
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
    sdfFileCmdHandle_t *command = sdfFileOpen("/fld/f/bin/FLDALL.TBL");
    s32 sum;
    s16 previous;
    s32 i;

    sdfFileRead(command, D_0039A1D0, 0x3D8);
    sdfFileRead(command, D_0039A5A8, 0x3C00);
    sdfFileRead(command, D_0039E1A8, 0x4400);
    sdfFileRead(command, D_003A25A8, 0x1C00);
    sdfFileRead(command, D_003A41A8, 0x640);
    sdfFileRead(command, D_003A47E8, 0xC80);
    sdfFileRead(command, D_003A55F0, 0x3840);
    sdfFileRead(command, D_003A8EB0, 0x1000);
    sdfFileRead(command, D_0038A3B8, 0xC8);
    sdfFileRead(command, D_0038A480, 0x190);
    sdfFileRead(command, D_0038A9B0, 0x500);
    sdfFileRead(command, fldCameraFollowRows, 0xC00);
    sdfFileRead(command, fldCameraSettings, 0x2A0);
    sdfFileRead(command, D_00387D70, 0x1400);
    sdfFileRead(command, fldStageCoordinateRecords, 0x600);
    sdfFileRead(command, D_00389A70, 0x880);
    sdfFileRead(command, D_00391FA0, 0x1200); /* 64 FldNpcMotion rows */
    sdfFileClose(command);
    sum = 0;
    previous = 0;
    for (i = 0; i < 0x1F; i++) {
        D_003899F0[i] = 0;
    }
    for (i = 0; i < 0x60; i++) {
        s16 id = (fldStageCoordinateRecords)[i].x;

        if (id > 0) {
            if (id < 0x1F) {
                if (id != previous) {
                    D_003899F0[id] = sum;
                }
                previous = id;
                sum += (fldStageCoordinateRecords)[i].rows;
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
        DDS3_GBWK->activationFlags[bankIndex] = 0;
    }

    for (bankIndex = 0; bankIndex < 31; bankIndex++) {
        for (slotIndex = 0; slotIndex < 64; slotIndex++) {
            DDS3_GBWK->maps[bankIndex].slots[slotIndex].roomModeFlags = 0;
            DDS3_GBWK->maps[bankIndex].slots[slotIndex].roomObjectModeFlags = 0;
            DDS3_GBWK->maps[bankIndex].slots[slotIndex].roomSceneFlags = 0;
            DDS3_GBWK->maps[bankIndex].slots[slotIndex].mapTargetFlags = 0;
            DDS3_GBWK->maps[bankIndex].slots[slotIndex].alternateMapTargetFlags = 0;
            for (valueIndex = 0; valueIndex < 16; valueIndex++) {
                DDS3_GBWK->maps[bankIndex].slots[slotIndex].values[valueIndex] = 0xFF;
            }
            DDS3_GBWK->maps[bankIndex].slots[slotIndex].auxiliaryFlags = 0;
            DDS3_GBWK->maps[bankIndex].slots[slotIndex].valueFlags = 0;
        }
    }

    fldClearAllAreaFloorFlags();
    memset(DDS3_GBWK->stageCellBits, 0, sizeof(DDS3_GBWK->stageCellBits));
}

#include "fld_vr_reset_view.h"

extern u32 fldRunDanteGunFlag[];
void fldResetFieldRuntimeState(FldRuntimeResetMode mode) {
    FldAreaWork *area = &fldGlobalBase;
    s32 i;

    area->unk2C = 0x10;
    area->area = 0;
    area->floor = 0;
    area->unk18 = 0;
    area->cachedArea = 0;
    area->reuseAreaResources = FLD_AREA_RESOURCE_REUSE_DISABLED;
    area->consumedFlags = 0;
    area->mode = 0;
    area->unk54 = 0;
    area->cameraTargetGraceTicks = 0;
    area->sceneMode = 0;
    area->sceneState = 0;
    area->selectedRoomIndex = 0;
    area->unk8C = 0;
    area->primaryMovingMatterIndex = -1;
    area->secondaryMovingMatterIndex = -1;
    area->unk98 = -1;
    area->primaryMapLabelSelection = -1;
    area->secondaryMapLabelSelection = -1;
    area->overlayMode = 1;
    area->overlayCounter = 0;
    area->mapBannerFadeMode = 1;
    area->unkD8 = -1;
    area->rowIdx = 0;
    area->sequenceCode = 1;
    area->cameraSupportHeightOffset = 0.0f;
    area->unk60 = 0.0f;
    area->negatedAngle = 0.0f;
    area->cameraTargetAngle = 0.0f;
    area->dist = 0.0f;
    memset(area->pad124, 0, sizeof(area->pad124));
    area->movementSoundActive = 0;
    area->terrainAttritionKind = FLD_TERRAIN_ATTRITION_NONE;
    area->attritionDistance = 100.0f;
    area->sceneCommand = 0;
    area->colorEffectSuppressed = 0;
    area->defaultEncounterId = 0;
    area->encounterOverrideId = 0;
    area->encounterMode = 0;
    area->transitionCount = 0;
    area->frameTravelDistance = 0.0f;
    area->titleFade = 0;
    area->playerModelAlternate = 0;
    area->cameraTiltActive = 0;
    area->unkB4 = 0;
    area->cameraTilt = 0.0f;
    area->pendingEntryDialogDisposition = FLD_ENTRY_DIALOG_NONE;
    area->specialEventActive = 0;
    area->collectedCount = 0;
    area->score = 0;
    memset(&area->unk10C, 0, sizeof(area->unk10C));
    area->pendingSceneRequest = FLD_SCENE_ADVANCE_REQUEST_NONE;
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
    area->positionPending = FLD_POSITION_UPDATE_NONE;
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
    area->cameraSupportHeightDelta = 0.0f;
    area->modeOneCameraHeadingTarget = 0.0f;
    area->fieldTextures[0].texture = NULL;
    area->fieldTextures[1].texture = NULL;
    area->fieldTextures[2].texture = NULL;
    area->fieldTextures[3].texture = NULL;
    fldRunDanteGunFlag[0] = 0;
    fldRunDanteGunFlag[1] = 0;
    fldRunDanteGunFlag[2] = 0;
    fldRunDanteGunFlag[3] = 0;
    fldRunDanteGunFlag[4] = 0;
    fldRunDanteGunFlag[5] = 0;
    fldRunDanteGunFlag[6] = 0;
    fldRunDanteGunFlag[7] = 0;
    fldRunDanteGunFlag[8] = 0;
    fldRunDanteGunFlag[9] = 1;
    fldRunDanteGunFlag[10] = 1;
    fldRunDanteGunFlag[11] = 0;
    fldRunDanteGunFlag[12] = 0;
    fldRunDanteGunFlag[13] = 0;
    fldRunDanteGunFlag[14] = 0;
    fldRunDanteGunFlag[15] = 0;
    fldRunDanteGunFlag[16] = 0;
    fldRunDanteGunFlag[19] = 1;
    fldRunDanteGunFlag[18] = 0;
    fldRunDanteGunFlag[21] = 0;
    fldRunDanteGunFlag[22] = 0;
    fldRunDanteGunFlag[23] = 1;
    fldRunDanteGunFlag[24] = 1;
    fldRunDanteGunFlag[20] = 1;
    if (mode == FLD_RUNTIME_RESET_CLEAR_MAP_STATE) {
        fldResetFieldMapState();
        DDS3_GBWK->vr.previewX = 0;
        DDS3_GBWK->vr.previewY = 0;
        {
            u32 *word = ((FldVrResetView *)&DDS3_GBWK->vr)->words;
            for (i = 14; i >= 0; i--) {
                *word++ = 0;
            }
        }
        DDS3_GBWK->header.magic[0] = 'V';
        DDS3_GBWK->header.magic[1] = 'E';
        DDS3_GBWK->header.magic[2] = 'R';
        DDS3_GBWK->header.version = 1;
        DDS3_GBWK->header.mapGroup = 0;
        DDS3_GBWK->header.mapIndex = 0;
        DDS3_GBWK->header.pad06[0] = 0;
        DDS3_GBWK->header.pad06[1] = 0;
        DDS3_GBWK->header.playTicks = 0;
        for (i = 0; i < 8; i++) {
            DDS3_GBWK->header.partyIds[i] = 0;
            DDS3_GBWK->header.partyLevels[i] = 0;
        }
        DDS3_GBWK->header.savedMoney = 0;
        DDS3_GBWK->header.saveFlags = 0;
        DDS3_GBWK->header.saveFlags1 = 0;
        DDS3_GBWK->header.saveFlags2 = 0;
        DDS3_GBWK->header.status = 0;
        DDS3_GBWK->header.transition = 0;
    }
    fldResetViewState();
    fldRunDanteClear();
}

extern void fldClearCameraFacingPointRequest(void);
extern void fldClearCameraHeadingRequest(void);

/* Reset the per-area controller work: -1 selections, the attrition step and the
 * adjacent request words, then clear the camera facing/heading requests. */
void fldInitializeAreaWork(void) {
    FldAreaWork *area = &fldGlobalBase;

    area->unk2C = 0x10;
    area->unk138 = 0;
    area->consumedFlags = 0;
    area->unk54 = 0;
    area->unk8C = 0;
    area->primaryMovingMatterIndex = -1;
    area->secondaryMovingMatterIndex = -1;
    area->unk98 = -1;
    area->unkA0 = -1;
    area->unkA4 = -1;
    area->overrideSupportRecordIndex = -1;
    area->unkAC = -1;
    area->overlayMode = 1;
    area->overlayCounter = 0;
    area->unkD8 = -1;
    area->terrainAttritionKind = FLD_TERRAIN_ATTRITION_NONE;
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
    area->mapBannerFadeMode = 1;
    fldRunDanteGunFlag[0] = 0;
    fldRunDanteGunFlag[1] = 0;
    fldRunDanteGunFlag[2] = 0;
    fldRunDanteGunFlag[3] = 0;
    fldRunDanteGunFlag[4] = 0;
    fldRunDanteGunFlag[5] = 0;
    fldRunDanteGunFlag[11] = 0;
    fldRunDanteGunFlag[12] = 0;
    fldRunDanteGunFlag[13] = 0;
    fldRunDanteGunFlag[14] = 0;
    fldRunDanteGunFlag[16] = 0;
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
            DDS3_GBWK->maps[mapIndex].slots[slotIndex].roomModeFlags |= 1 << bit;
        } else {
            DDS3_GBWK->maps[mapIndex].slots[slotIndex].roomModeFlags &= ~(1 << bit);
        }
    }
}

u8 fldTestRoomModeFlag(s32 mapId, u32 slotIndex, s32 bit) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        u32 flags = DDS3_GBWK->maps[mapIndex].slots[slotIndex].roomModeFlags;
        return (flags >> bit) & 1;
    }
    return 0;
}

void fldSetRoomObjectModeFlag(s32 mapId, u32 slotIndex, s32 bit, s32 enabled) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        if (enabled) {
            DDS3_GBWK->maps[mapIndex].slots[slotIndex].roomObjectModeFlags |= 1 << bit;
        } else {
            DDS3_GBWK->maps[mapIndex].slots[slotIndex].roomObjectModeFlags &= ~(1 << bit);
        }
    }
}

u8 fldTestRoomObjectModeFlag(s32 mapId, u32 slotIndex, s32 bit) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        u32 flags = DDS3_GBWK->maps[mapIndex].slots[slotIndex].roomObjectModeFlags;
        return (flags >> bit) & 1;
    }
    return 0;
}

void fldSetRoomSceneFlag(s32 mapId, u32 slotIndex, s32 bit, s32 enabled) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        if (enabled) {
            DDS3_GBWK->maps[mapIndex].slots[slotIndex].roomSceneFlags |= 1 << bit;
        } else {
            DDS3_GBWK->maps[mapIndex].slots[slotIndex].roomSceneFlags &= ~(1 << bit);
        }
    }
}

u8 fldTestRoomSceneFlag(s32 mapId, u32 slotIndex, s32 bit) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        u32 flags = DDS3_GBWK->maps[mapIndex].slots[slotIndex].roomSceneFlags;
        return (flags >> bit) & 1;
    }
    return 0;
}

void fldSetMapTargetFlag(s32 mapId, u32 slotIndex, s32 bit, s32 enabled) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        if (enabled) {
            DDS3_GBWK->maps[mapIndex].slots[slotIndex].mapTargetFlags |= 1 << bit;
        } else {
            DDS3_GBWK->maps[mapIndex].slots[slotIndex].mapTargetFlags &= ~(1 << bit);
        }
    }
}

u8 fldTestMapTargetFlag(s32 mapId, u32 slotIndex, s32 bit) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        u32 flags = DDS3_GBWK->maps[mapIndex].slots[slotIndex].mapTargetFlags;
        return (flags >> bit) & 1;
    }
    return 0;
}

void fldSetAlternateMapTargetFlag(s32 mapId, u32 slotIndex, s32 bit, s32 enabled) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        if (enabled) {
            DDS3_GBWK->maps[mapIndex].slots[slotIndex].alternateMapTargetFlags |= 1 << bit;
        } else {
            DDS3_GBWK->maps[mapIndex].slots[slotIndex].alternateMapTargetFlags &= ~(1 << bit);
        }
    }
}

u8 fldTestAlternateMapTargetFlag(s32 mapId, u32 slotIndex, s32 bit) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        u32 flags = DDS3_GBWK->maps[mapIndex].slots[slotIndex].alternateMapTargetFlags;
        return (flags >> bit) & 1;
    }
    return 0;
}

/* Store a byte at valueIndex within the selected slot's values area. */
void fldSetMapSlotByte(s32 mapId, u32 slotIndex, s32 valueIndex, s32 byteValue) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        DDS3_GBWK->maps[mapIndex].slots[slotIndex].values[valueIndex] = byteValue;
    }
}

/* Read a slot value, translating stored 0xff to -1; IDs >= the limit return zero. */
s32 fldGetMapSlotByte(s32 mapId, u32 slotIndex, s32 valueIndex) {
    u8 value = 0;
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        value = DDS3_GBWK->maps[mapIndex].slots[slotIndex].values[valueIndex];
    }
    return value == FIELD_MAP_EMPTY_VALUE ? -1 : value;
}

void fldSetMapSlotAuxiliaryFlag(s32 mapId, u32 slotIndex, s32 bit, s32 enabled) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        if (enabled) {
            DDS3_GBWK->maps[mapIndex].slots[slotIndex].auxiliaryFlags |= 1 << bit;
        } else {
            DDS3_GBWK->maps[mapIndex].slots[slotIndex].auxiliaryFlags &= ~(1 << bit);
        }
    }
}

u8 fldTestMapSlotAuxiliaryFlag(s32 mapId, u32 slotIndex, s32 bit) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        u32 flags = DDS3_GBWK->maps[mapIndex].slots[slotIndex].auxiliaryFlags;
        return (flags >> bit) & 1;
    }
    return 0;
}

void fldSetMapSlotValueFlag(s32 mapId, u32 slotIndex, s32 bit, s32 enabled) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        if (enabled) {
            DDS3_GBWK->maps[mapIndex].slots[slotIndex].valueFlags |= 1 << bit;
        } else {
            DDS3_GBWK->maps[mapIndex].slots[slotIndex].valueFlags &= ~(1 << bit);
        }
    }
}

u8 fldTestMapSlotValueFlag(s32 mapId, u32 slotIndex, s32 bit) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        u32 flags = DDS3_GBWK->maps[mapIndex].slots[slotIndex].valueFlags;
        return (flags >> bit) & 1;
    }
    return 0;
}

extern void fldFindTakaraSlot(s32);

extern void ptyReturnOne(s32);

void fldActivateFlaggedObject(u32 flagIndex) {
    DDS3_GBWK->activationFlags[(s32)flagIndex >> 3] |= 1 << (flagIndex & 7);
    fldFindTakaraSlot(flagIndex);
    if (flagIndex >= 0xF0 && flagIndex < 0x100) {
        evtBitOn(flagIndex + 0x610);
    }
    if (D_003A8EB0[flagIndex].kind == 2) {
        ptyReturnOne(D_003A8EB0[flagIndex].parameter);
    }
}

u8 fldTestObjectActivationFlag(u32 flagIndex) {
    return (DDS3_GBWK->activationFlags[(s32)flagIndex >> 3] >> (flagIndex & 7)) & 1;
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
const s16 *fldFindLocationCoordinateRecord(s32 area, s32 locationIndex) {
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

/* Read the matching location record's signed flags; zero on miss. */
s16 fldGetLocationCoordinateFlags(s32 area, s32 locationIndex) {
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
    FieldStageCoordinate *record = fldStageCoordinateRecords;
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
    FieldStageCoordinate *records = fldStageCoordinateRecords;
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
    FieldStageCoordinate *record = fldStageCoordinateRecords;
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

extern void fldAllIconDel(s32 index, f32 *x, f32 *z);

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

    fldAllIconDel(scene, &entryX, &entryZ);
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

extern void fldAllIconDel(s32 index, f32 *x, f32 *z);

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

    fldAllIconDel(room, &originX, &originZ);
    area = &fldGlobalBase;
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
            DDS3_GBWK->stageCellBits[(neighborZ + rowOffset) * 10 + column] |= 1 << bit;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0011F208", fldMarkStageExplorationBitmapRegion);

u8 fldIsStageExplorationCellMarked(s32 floor, f32 x, f32 z) {
    FieldStageCoordinate *record = fldFindStageCoordinateRecord(fldGlobalBase.area, floor);
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
    fldFindStageCoordinateIndex(fldGlobalBase.area, floor);
    rowOffset = fldFindStageCoordinateRowOffset(fldGlobalBase.area, floor);
    if (byte >= 0 && row >= 0 && byte < record->cols && row < record->rows) {
        if ((DDS3_GBWK->stageCellBits[(row + rowOffset) * 10 + byte] >> bit) & 1) {
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

INCLUDE_SDATA(const s32, "game/code_0011F208", gFldElvFloorNow);

INCLUDE_SDATA(const s32, "game/code_0011F208", fldElvFloorFrom);

INCLUDE_SDATA(const s32, "game/code_0011F208", fldElvFloorNum);

INCLUDE_SDATA(const s32, "game/code_0011F208", fldElvFloorTo);

INCLUDE_SDATA(const s32, "game/code_0011F208", fldElvFloorDisp);

INCLUDE_SDATA(const s32, "game/code_0011F208", fldElvFloorTarget);

INCLUDE_SDATA(const s32, "game/code_0011F208", fldElvState);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435EFC);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F00);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F04);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F08);

INCLUDE_SDATA(struct dds3Basic_t * const, "game/code_0011F208", fldPlayerObject);

INCLUDE_SDATA(struct dds3Basic_t * const, "game/code_0011F208", fldSecondarySceneObject);

INCLUDE_SDATA(dds3ModelHandle_t * const, "game/code_0011F208", fldPlayerModelContext);

INCLUDE_SDATA(dds3ModelHandle_t * const, "game/code_0011F208", fldSecondarySceneModelContext);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F1C);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F20);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F24);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F28);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F30);

INCLUDE_SDATA(struct sdfMemHandle_t * const, "game/code_0011F208", fldPlayerModelResource);

INCLUDE_SDATA(struct sdfMemHandle_t * const, "game/code_0011F208", D_00435F38);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F3C);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F40);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F44);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F48);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F50);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F58);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F60);

INCLUDE_SDATA(const s32, "game/code_0011F208", fldNext_fld);

INCLUDE_SDATA(const s32, "game/code_0011F208", fldDeferredCommandParameter);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F70);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F74);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F78);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F7C);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F80);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F84);

INCLUDE_SDATA(const s32, "game/code_0011F208", fldAutoMapSeqFlags);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F8C);

INCLUDE_SDATA(const s32, "game/code_0011F208", fldEncountFlags);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F98);
