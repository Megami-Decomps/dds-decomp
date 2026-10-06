#include "ee_mmi.h"
#include "pcp_vu0.h"
#include "common.h"
#include "eff_blur.h"
#include "fpu.h"
#include "fld.h"
#include "evt_world.h"
#include "dat_state.h"

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

extern u32 D_003BABE0;

extern s32 fldDeferredCommand;

extern u32 fldSceneControlFlags;

extern u32 fldSceneLifecycleFlags;
extern s32 D_003BABF0;

extern u32 D_003BABD0;

extern u64 dds3GetWorldSecondaryObject(void);
extern s64 dds3GetWorldValueCount(u64);
extern u64 dds3ReadIndexedWorldObjectWord(u64);
extern s64 dds3AdvanceObjectValueCursor(u64);
extern u64 dds3CopyWorldListToValueChain(u64, u64);
extern s64 evtGetObjectTransitionWork(u64);


extern s32 sdfAllocPacketAligned(s32 size);
extern u32 fldDeferredCommandParameter;
extern s32 D_003BAB08;
extern s32 D_003BAB0C;
extern s32 D_003BAB10;
extern s32 D_003BAB14;
extern s32 D_003BAB18;
extern s32 D_003BAB1C;
extern s32 D_003BAB20;
/* Native field-area work prefix, shared with the camera/motion unit.
 * Position is XYZ followed immediately by saved XYZ history, not a Vec4.
 * DDS2 inserts twelve bytes before the model variant and position/history tail. */
typedef struct FldAreaWork {
    u8 pad00[4];
    char *fallbackResourceName; /* Last choice after override and saved scene names. */
    u8 pad08[4];
    s32 consumedFlags; /* Consumption markers, separate from game-state work flags. */
    s32 area;
    s32 floor; /* Zero-based; coordinate lookups use floor + 1. */
    u8 pad18[8];
    s32 unk20; /* Sequence initializers set this when reusing the current area. */
    u8 pad24[0x2C];
    s32 mode;
    u8 pad54[4];
    s32 rowIdx;
    u8 pad5C[8];
    f32 negatedAngle;
    u8 pad68[4];
    f32 dist;
    s32 sceneMode;
    s32 sceneState;
    u8 pad78[0xC];
    s32 positionPending;
    u8 pad88[8];
    s32 unk90;
    s32 unk94;
    u8 pad98[0x28];
    s32 unkC0;
    u8 padC4[0x24];
    s32 unkE8;
    u8 padEC[4];
    s32 nextArea;
    s32 nextFloor; /* Both queued values at -1 mean no request. */
    u8 padF8[8];
    s32 unk100; /* Consumed before pending-resource selection. */
    u8 pad104[0x14];
    s32 unk118;
    u8 pad11C[0xC];
    s16 sceneCommand;
    u8 pad12A[2];
    s32 commandEnabled;
    u8 pad130[0xC];
    s32 playerModelVariant; /* Cached 0/1 player variant, or 2 for the location override. */
    f32 x;
    f32 y;
    f32 z;
    f32 previousX; /* History starts here, not a homogeneous position W. */
    f32 previousY;
    f32 previousZ;
    f32 targetX; /* XYZ installed when positionPending is consumed. */
    f32 targetY;
    f32 targetZ;
    f32 angle; /* Current player heading in degrees. */
    f32 targetAngle; /* Desired heading for the motion-unit updater. */
    f32 unk16C;
    f32 unk170;
    f32 unk174;
    s32 positionMode;
    s32 unk17C;
    s32 verticalStepDirection; /* Positive lowers Y; negative raises it. */
    s32 unk184;
    u32 pointState;
    f32 facingPointX;
    f32 facingPointZ;
    u32 angleState;
    f32 overrideAngle;
} FldAreaWork;
extern FldAreaWork fldAreaState;
extern s32 D_0032E4DC[];
extern u8 D_00324F88[];
extern u8 D_003257F8[];
extern u8 D_00324530[];
extern char D_0039FA00[];
extern char D_0039FCA0[];
extern char D_0039FCB0[];
extern char D_0039FCC8[];
extern void fldStepValueByPad(f32 *value, u8 *padState, f32 min, f32 max, f32 step, f32 bigStep);
extern s32 fldTestDrawUpdate(void);
extern void fldClearCameraMoveMode(void);
extern void fldSetCameraMoveMode(s32);
extern void func_0013E5A8(u32 arg0);
extern u32 fldGetSceneReadyFlag(void);
extern s32 scrFindNamedProcessNode(const char *arg0);
extern void sdfStoreMessageWordsAndNotifyConsumer(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
extern s32 kwlnTaskDestroyWithHierarchyByName(const char *name, s32 arg1);
extern u32 fldPlayerObject;
extern u32 fldPlayerModelResource;
extern u32 D_0032E498[];
extern u32 D_003BAB58;
extern u32 D_0032E3D0[];
extern u32 D_003BABE8;
extern u32 fldCameraModelObject;
extern void effObjFetchInnerFirstVec(u32);
extern void effObjFetchInnerSecondVecNorm(u32);
extern void fldSetPendingAreaAndFloor(s32, s32);
extern s32 fldIsAreaResourceReady(void);
extern void fldPollAreaResourceLoad(void);
extern s32 fldGetResourceReadyFlag(void);
extern void fldFreeDisplayObjects(void);
extern s32 sdfCheckPendingWorkWithInterrupts(void);
extern s32 dds3AdminGetRequestedMode(void);
extern s32 dds3AdminReadPreviousUnsignedSample(void);
extern void func_0013F100(s32, u32);
extern u32 D_0032E4EC[];
extern void sdfQueueNonzeroResourceId(u32 arg0);
extern s32 D_003BABEC;
extern u8 D_0034C8F0[];
extern void fldActivateObjectById(s32);
extern void mdlFlagSet(s32);
extern void func_0011B150(s32);
void fldDispatchDeferredFieldCommand(void);

/* Contiguous player-scene work: saved transform, status words and deferred resource.
 * The data also exports a label at +0x3C for separate object-slot consumers. */
typedef struct FieldPlayerSceneWork {
    u128 position;
    u128 rotation;
    u8 pad20[0x14];
    u32 primaryState;
    u8 pad38[0x24];
    u32 secondaryState;
    u8 pad60[0x20];
    s8 resourceName[0x20];
} FieldPlayerSceneWork;

extern FieldPlayerSceneWork D_0032F1A0;
extern s16 D_0032DDB0[];
extern u8 D_0033F068[];
extern u8 D_00342868[];
extern u32 dds3AdvanceWorldCounter(void);
extern void *kwlnTaskGetTaskByName(const char *);
extern void dds3WorkClear(void);
extern char D_0039FBC0[]; /* "fldProcSequence" */
extern void dds3SetWorldEntryCallbackTarget(void *, const char *);
extern u8 D_003BAB3C;
extern u8 D_0032C9A0[];
extern s16 D_0032C9B0[];
extern u8 D_00346068[];
extern u32 mnuAcknowledgeCampState(void);
void fldClearSceneCommandFlag(void);
void fldClearFieldTransitionFlag(void);
void fldClearSecondarySceneFlag(void);
void fldClearPrimarySceneFlag(void);
void fldClearSceneControlFlags(u32 arg0);
extern u32 D_003BAB64;
extern u32 D_003BAB60;
extern u32 D_003BAB5C;
extern u32 sdfAllocGeneralBlock(u32);
extern void *sdfMemoryGetBlockAddress(u32);
extern void mdlLoadViewerPackage(s32, s32, s32, void *, u32);
void fldLoadPlayerModel(void);
u8 fldTestSceneControlFlags(u32 arg0);

u32 *fldGetPlayerSceneStateAddress(void);

extern u32 D_003BAC08[2];
extern void fldResetEventSceneState(void);
extern u32 func_0014D100(void);
extern void fldResetTaskSlots(void);
extern void fldSetSceneLifecycleFlags(u32);
extern void func_00220178(void);

void fldSetPacketArgumentPair(u32 *packet, u32 first, u32 second) {
    packet[4] = first;
    packet[5] = second;
}

s32 sdfCreateResetPacketList(void) {
    s32 packet;

    packet = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(packet);
    return packet;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011D3E8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011D570);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011D6B0);

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
void fldFormatSecondsText(char *text, f32 value) {
    s32 whole;
    s32 tenths;
    s32 hundredths;

    value += 0.005f;
    whole = (s32)value;
    if (whole >= 10) {
        tenths = 9;
        hundredths = 9;
    } else {
        value -= (f32)whole;
        value *= 10.0f;
        tenths = (s32)value;
        value -= (f32)tenths;
        hundredths = (s32)(value * 10.0f);
    }
    text[0] = whole + '0';
    text[1] = '.';
    text[2] = tenths + '0';
    text[3] = hundredths + '0';
    text[4] = 0;
}


extern void *func_0011D3E8(s32, s32, s32, s32, s32, u32, u32);
extern void *sdfCreateFormattedSifCommand(s32, s32, s32, s32, const char *, ...);
extern void sdfAppendPacket(void *, void *);
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
    sdfAppendPacket(packetList, func_0011D3E8(x + 0x510, y + 0x18,
                     0xFF0080, 0x240, 0xF0, color, 0x40806020));
    for (i = 0; i != 3; i++) {
        sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(x, y,
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
        sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(x + 0x180,
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
    sdfAppendPacket(packetList, func_0011D3E8(x + 0x510, y + 0x18,
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
        sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(
                        x, y, 0xFF0080, style, label));
        sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(
                        x + 0x180, y, 0xFF0080, i == selected ? 6 : 0,
                        D_003BAAF8, channel));
        if (showNormalized != 0) {
            sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(
                            x, y + 0x180, 0xFF0080, style, label));
            sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(
                            x + 0x240, y + 0x180, 0xFF0080, 0,
                            D_003BAB00, channel * (1.0f / 255.0f)));
        }
        y += 0x60;
    }
}

u32 func_0011E278(void) {
    return 0;
}

extern f32 sdfSinPoly(f32);
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);
extern void func_00129900(s32);
extern void fldSelectDisplayBuffer(s32);
extern void fldSubmitGsTriangle(s32, s32, s32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

/* Filled disc in the field; radius shrinks in the close-up areas. */
void func_0011E280(s32 fade, f32 x, f32 y, f32 z, f32 radius) {
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
    func_00129900(5);
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
    func_00129900(0);
}


extern u8 fldGetCampSceneControlMode(void);
extern void fldSubmitFrameQuad(s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_00129900(s32);
extern void func_00134348(void);
extern void func_00132010(void);
extern void func_0012AEB0(void);
extern u32 D_0032E570[];
extern void fldSelectDisplayBuffer(s32);
s32 fldTestDrawUpdate(void) {
    if (fldGetEncounterRuntimeResult() != 0) {
        return 0;
    }
    if (fldGetCampSceneControlMode() == 1) {
        return 0;
    }
    fldSelectDisplayBuffer(0x53);
    fldSubmitFrameQuad(1, 0, 0x81, 3, 0, 0, 1, 1);
    func_00129900(0);
    func_00134348();
    func_00132010();
    if (D_0032E570[9] != 0) {
        if (fldGetSceneReadyFlag() == 0) {
            fldSelectDisplayBuffer(0x53);
        } else {
            fldSelectDisplayBuffer(0x5E);
        }
        func_0012AEB0();
    }
    fldSelectDisplayBuffer(0x27);
    fldSubmitFrameQuad(1, 5, 0x80, 1, 0, 0, 1, 2);
    func_00129900(0);
    fldSelectDisplayBuffer(0x39);
    fldSubmitFrameQuad(1, 5, 0x80, 1, 0, 0, 1, 2);
    func_00129900(0);
    return 0;
}

void fldTestDrawCreate(void) {
    kwlnTaskCreate((s32)D_0039FA00, 0x2AF8, 0, 0, (s32)fldTestDrawUpdate, 0, 0);
}

void fldTestDrawDestroy(void) {
    kwlnTaskDestroyWithHierarchyByName(D_0039FA00, 1);
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011E6C0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011E810);

enum {
    FIELD_CAMERA_TRACKING_UNSET = -999
};

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

extern u32 dds3ResetObjectValueCursor(u64);
extern s32 dds3TestObjectFlags(u64, s32);
extern void dds3DestroyWorldIndexNode(u64);
extern void fldSelectDisplayBuffer(s32);
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
    u64 objectChain;
    u64 worldObject;

    objectChain = dds3CopyWorldListToValueChain(dds3GetWorldSecondaryObject(), 5);
    if (objectChain != 0) {
        if (dds3ResetObjectValueCursor(objectChain) != 0) {
            do {
                worldObject = dds3ReadIndexedWorldObjectWord(objectChain);
                if (dds3TestObjectFlags(worldObject, FIELD_BACKGROUND_REQUIRED_OBJECT_FLAG) != 0) {
                    if (dds3TestObjectFlags(worldObject, FIELD_BACKGROUND_EXCLUDED_OBJECT_FLAG) == 0) {
                        hasEligibleBackground = 1;
                    }
                }
            } while (dds3AdvanceObjectValueCursor(objectChain) != 0);
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

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FA28);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011ECC8);

extern u8 D_0033EC90[], D_00347C68[], D_003482A8[], D_00349030[], D_0032EF18[], D_0032EFE0[], D_0032F510[], fldCameraFollowRows[], D_0032E5C8[], D_00336A60[];
extern void *fldCameraSettings;
extern u32 sdfDevCreateCommandState(const char *);
extern u32 sdfDevQueueReadAndWait(u32, void *, u32);
extern void sdfDevWaitThenReleaseCommandState(u32);
void fldLoadFieldTables(void) {
    u32 command = sdfDevCreateCommandState("/fld/f/bin/FLDALL.TBL");

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
    sdfDevQueueReadAndWait(command, D_00336A60, 0x1200);
    sdfDevWaitThenReleaseCommandState(command);
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00120AE8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00120C08);

extern u32 D_0032E570[];
extern void func_00131580(s32, void *, s32);
extern void func_00131590(void);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00120EC8);


/* Map IDs select 1920-byte banks of 30-byte slots; flag banks are halfwords.
 * The guards check only the upper map-ID bound: negative IDs and slot/bit
 * ranges remain unchecked. */
enum {
    FIELD_MAP_ID_LIMIT = 40,
    FIELD_MAP_ID_MODULUS = 100,
    FIELD_MAP_EMPTY_VALUE = 0xff,
};

INCLUDE_ASM(const s32, "game/code_0011D3A0", fldSetRoomModeFlag);

INCLUDE_ASM(const s32, "game/code_0011D3A0", fldTestRoomModeFlag);

INCLUDE_ASM(const s32, "game/code_0011D3A0", fldSetRoomObjectModeFlag);

INCLUDE_ASM(const s32, "game/code_0011D3A0", fldTestRoomObjectModeFlag);

INCLUDE_ASM(const s32, "game/code_0011D3A0", fldSetRoomSceneFlag);

INCLUDE_ASM(const s32, "game/code_0011D3A0", fldTestRoomSceneFlag);

INCLUDE_ASM(const s32, "game/code_0011D3A0", fldSetMapTargetFlag);

INCLUDE_ASM(const s32, "game/code_0011D3A0", fldTestMapTargetFlag);

INCLUDE_ASM(const s32, "game/code_0011D3A0", fldSetAlternateMapTargetFlag);

INCLUDE_ASM(const s32, "game/code_0011D3A0", fldTestAlternateMapTargetFlag);

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

INCLUDE_ASM(const s32, "game/code_0011D3A0", fldSetMapSlotAuxiliaryFlag);

INCLUDE_ASM(const s32, "game/code_0011D3A0", fldTestMapSlotAuxiliaryFlag);

INCLUDE_ASM(const s32, "game/code_0011D3A0", fldSetMapSlotValueFlag);

INCLUDE_ASM(const s32, "game/code_0011D3A0", fldTestMapSlotValueFlag);


INCLUDE_ASM(const s32, "game/code_0011D3A0", fldActivateFlaggedObject);

u8 fldTestObjectActivationFlag(u32 flagIndex) {
    return (datGameState->activationFlags[(s32)flagIndex >> 3] >> (flagIndex & 7)) & 1;
}

/* Search records 1..511 of the 28-byte coordinate table; return zero on miss. */
s32 func_00121818(s32 x, s32 y) {
    u8 *records = D_0033F068;
    u8 *yColumn = records + 2;
    s32 index = 1;
    s32 offset = 28;
    do {
        if (x == *(s16 *)(offset + (s32)records) && y == *(s16 *)(offset + (s32)yColumn)) {
            return index;
        }
        index++;
        offset += 28;
    } while (index < FIELD_COORDINATE_SCAN_LIMIT);
    return 0;
}

/* Search the second 28-byte coordinate table, also reserving index zero. */
s32 func_00121870(s32 x, s32 y) {
    u8 *records = D_00342868;
    u8 *yColumn = records + 2;
    s32 index = 1;
    s32 offset = 28;
    do {
        if (x == *(s16 *)(offset + (s32)records) && y == *(s16 *)(offset + (s32)yColumn)) {
            return index;
        }
        index++;
        offset += 28;
    } while (index < FIELD_COORDINATE_SCAN_LIMIT);
    return 0;
}

/* Search the 28-byte map records, skipping row zero; zero denotes a miss. */
s32 fldFindMapCoordinateIndex(s32 x, s32 y) {
    u8 *records = D_00346068;
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
    u8 *records = (u8 *)D_0032C9B0;
    u8 *yColumn = records + 2;
    s32 index = 1;
    do {
        s32 offset = index * 8;
        index++;
        if (x == *(s16 *)(offset + (s32)records) && y == *(s16 *)(offset + (s32)yColumn)) {
            return (s16 *)(offset + (s32)records);
        }
    } while (index < FIELD_LOCATION_COORDINATE_SCAN_LIMIT);
    return D_0032C9B0;
}

/* Read the matching location record's fourth signed halfword; zero on miss. */
s32 fldGetLocationCoordinateValue(s32 x, s32 y) {
    u8 *records = (u8 *)D_0032C9B0;
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
    s16 *record = D_0032DDB0;
    s32 index = 0;
    s32 visited = 0;

    do {
        if (record[0] == x && record[1] == y) {
            return index;
        }
        index++;
        visited++;
        record += 8;
    } while (visited < FIELD_STAGE_INDEX_SCAN_LIMIT);
    return index;
}

/* This pointer lookup searches only the first 96 stage entries; NULL on miss. */
s16 * fldFindStageCoordinateRecord(s32 x, s32 y) {
    s16 *record = D_0032DDB0;
    s32 index = 0;

    do {
        if (record[0] == x && record[1] == y) {
            return record;
        }
        index++;
        record += 8;
    } while (index < FIELD_STAGE_RECORD_SCAN_LIMIT);
    return NULL;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121A58);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121B88);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121DE0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121ED8);

/* Return to-minus-from after integer-degree reduction and one wrap adjustment. */
f32 fldAngleDifference(f32 fromAngle, f32 toAngle) {
    f32 difference;

    if (fromAngle < 0.0f || toAngle < 0.0f) {
        fromAngle += 360.0f;
        toAngle += 360.0f;
    }
    fromAngle = (s32)fromAngle % 360;
    toAngle = (s32)toAngle % 360;
    difference = fromAngle - toAngle;
    if (difference > 180.0f || difference < -180.0f) {
        if (fromAngle < toAngle) {
            fromAngle += 360.0f;
        } else {
            toAngle += 360.0f;
        }
    }
    return toAngle - fromAngle;
}

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FB18);

f32 fldSnapAngleToCompassPoint(f32 angle) {
    f32 compass[16] = {0.0f, 22.5f, 45.0f, 67.5f, 90.0f, 112.5f, 135.0f, 157.5f, 180.0f, 202.5f, 225.0f, 247.5f, 270.0f, 292.5f, 315.0f, 337.5f};
    f32 best;
    s32 index;
    f32 diff;

    while (angle >= 360.0f) {
        angle -= 360.0f;
    }
    while (angle < 0.0f) {
        angle += 360.0f;
    }
    best = 900.0f;
    index = -1;
    diff = fabsf(fldAngleDifference(0.0f, angle));
    if (diff < best) {
        best = diff;
        index = 0;
    }
    diff = fabsf(fldAngleDifference(22.5f, angle));
    if (diff < best) {
        best = diff;
        index = 1;
    }
    diff = fabsf(fldAngleDifference(45.0f, angle));
    if (diff < best) {
        best = diff;
        index = 2;
    }
    diff = fabsf(fldAngleDifference(67.5f, angle));
    if (diff < best) {
        best = diff;
        index = 3;
    }
    diff = fabsf(fldAngleDifference(90.0f, angle));
    if (diff < best) {
        best = diff;
        index = 4;
    }
    diff = fabsf(fldAngleDifference(112.5f, angle));
    if (diff < best) {
        best = diff;
        index = 5;
    }
    diff = fabsf(fldAngleDifference(135.0f, angle));
    if (diff < best) {
        best = diff;
        index = 6;
    }
    diff = fabsf(fldAngleDifference(157.5f, angle));
    if (diff < best) {
        best = diff;
        index = 7;
    }
    diff = fabsf(fldAngleDifference(180.0f, angle));
    if (diff < best) {
        best = diff;
        index = 8;
    }
    diff = fabsf(fldAngleDifference(202.5f, angle));
    if (diff < best) {
        best = diff;
        index = 9;
    }
    diff = fabsf(fldAngleDifference(225.0f, angle));
    if (diff < best) {
        best = diff;
        index = 10;
    }
    diff = fabsf(fldAngleDifference(247.5f, angle));
    if (diff < best) {
        best = diff;
        index = 11;
    }
    diff = fabsf(fldAngleDifference(270.0f, angle));
    if (diff < best) {
        best = diff;
        index = 12;
    }
    diff = fabsf(fldAngleDifference(292.5f, angle));
    if (diff < best) {
        best = diff;
        index = 13;
    }
    diff = fabsf(fldAngleDifference(315.0f, angle));
    if (diff < best) {
        best = diff;
        index = 14;
    }
    diff = fabsf(fldAngleDifference(337.5f, angle));
    if (diff < best) {
        index = 15;
    }
    return compass[index];
}

/* Snap an angle in degrees to the nearest of the eight compass directions. */
f32 fldSnapAngleToCompassOctant(f32 angle) {
    f32 best;
    s32 index;
    f32 diff;
    while (angle >= 360.0f) {
        angle -= 360.0f;
    }
    while (angle < 0.0f) {
        angle += 360.0f;
    }
    best = 900.0f;
    index = -1;
    diff = fabsf(fldAngleDifference(0.0f, angle));
    if (diff < best) {
        best = diff;
        index = 0;
    }
    diff = fabsf(fldAngleDifference(45.0f, angle));
    if (diff < best) {
        best = diff;
        index = 1;
    }
    diff = fabsf(fldAngleDifference(90.0f, angle));
    if (diff < best) {
        best = diff;
        index = 2;
    }
    diff = fabsf(fldAngleDifference(135.0f, angle));
    if (diff < best) {
        best = diff;
        index = 3;
    }
    diff = fabsf(fldAngleDifference(180.0f, angle));
    if (diff < best) {
        best = diff;
        index = 4;
    }
    diff = fabsf(fldAngleDifference(225.0f, angle));
    if (diff < best) {
        best = diff;
        index = 5;
    }
    diff = fabsf(fldAngleDifference(270.0f, angle));
    if (diff < best) {
        best = diff;
        index = 6;
    }
    diff = fabsf(fldAngleDifference(315.0f, angle));
    if (diff < best) {
        index = 7;
    }
    switch (index) {
    case 0:
        angle = 0.0f;
        break;
    case 1:
        angle = 45.0f;
        break;
    case 2:
        angle = 90.0f;
        break;
    case 3:
        angle = 135.0f;
        break;
    case 4:
        angle = 180.0f;
        break;
    case 5:
        angle = 225.0f;
        break;
    case 6:
        angle = 270.0f;
        break;
    case 7:
        angle = 315.0f;
        break;
    }
    return angle;
}

f32 fldSnapAngleToCardinalDirection(f32 angle) {
    f32 best;
    s32 index;
    f32 diff;
    while (angle >= 360.0f) {
        angle -= 360.0f;
    }
    while (angle < 0.0f) {
        angle += 360.0f;
    }
    best = 900.0f;
    index = -1;
    diff = fabsf(fldAngleDifference(0.0f, angle));
    if (diff < best) {
        best = diff;
        index = 0;
    }
    diff = fabsf(fldAngleDifference(90.0f, angle));
    if (diff < best) {
        best = diff;
        index = 2;
    }
    diff = fabsf(fldAngleDifference(180.0f, angle));
    if (diff < best) {
        best = diff;
        index = 4;
    }
    diff = fabsf(fldAngleDifference(270.0f, angle));
    if (diff < best) {
        index = 6;
    }
    switch (index) {
    case 0:
        angle = 0.0f;
        break;
    case 2:
        angle = 90.0f;
        break;
    case 4:
        angle = 180.0f;
        break;
    case 6:
        angle = 270.0f;
        break;
    }
    return angle;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001228D8);

extern f32 func_001228D8();

f32 fldGetNormalizedComplementaryAngle(void) {
    s32 angle = (s32)(360.0f - func_001228D8() + 90.0f);
    return (f32)(angle % 360);
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00122A00);

f32 fldPointDistance(f32 ax, f32 ay, f32 az, f32 bx, f32 by, f32 bz) {
    f32 dx = ax - bx;
    f32 dy = ay - by;
    f32 dz = az - bz;
    return fsqrtf(dx * dx + dy * dy + dz * dz);
}

/* For objects in transition state 4, set state 3 or clear it to 0.
 * status honestly covers the count, transition state and cursor result. */
void fldToggleWorldNodeState(s64 clearMode) {
    u64 valueChain;
    s64 status;
    u64 worldObject;

    valueChain = dds3GetWorldSecondaryObject();
    valueChain = dds3CopyWorldListToValueChain(valueChain, 6);
    status = dds3GetWorldValueCount(valueChain);
    if (status == 0) {
        return;
    }
    dds3ResetObjectValueCursor(valueChain);
    do {
        worldObject = dds3ReadIndexedWorldObjectWord(valueChain);
        status = evtGetObjectTransitionWork(worldObject);
        if (status == 4) {
            if (clearMode == 0) {
                evtSetObjectTransitionWork(worldObject, 3);
            }
            else {
                evtSetObjectTransitionWork(worldObject, 0);
            }
        }
        status = dds3AdvanceObjectValueCursor(valueChain);
    } while (status != 0);
    dds3DestroyWorldIndexNode(valueChain);
}

extern u32 D_0032E570[];
extern void sdfResetGameRuntime(s32);
extern s32 fileLoadStateChanged(void);
extern void fileCacheSlotFlagsFromState(void);
extern void fileRestoreSlotFlagsToState(void);
void fldSetDeferredFieldCommand(u32, u32);
extern void dds3AdminSubmitModeRequest(s32, void *, s32, s32);

void fldPrepareDeferredSceneTransition(void) {
    if (datGameState->header.transition != 0) {
        sdfResetGameRuntime(1);
    } else {
        sdfResetGameRuntime(0);
    }
    if (fileLoadStateChanged() == 0) {
        fileCacheSlotFlagsFromState();
    } else {
        fileRestoreSlotFlagsToState();
    }
    mdlFlagSet(0xc0f);
    {
        s32 code;
        D_0032E570[0x44 / 4] = 1;
        code = 0x259;
        D_0032E570[0x4c / 4] = 0;
        fldSetDeferredFieldCommand(0x15, 0x259);
        dds3AdminSubmitModeRequest(6, &code, 4, 0);
    }
}


extern s32 fileGetSelectionPendingFlag(void);
extern void func_00120C08(s32);
extern char D_003BAB68[];

void fldStartSequenceRecord(void) {
    FieldSequenceRecord record;

    if (fileGetSelectionPendingFlag() == 1) {
        return;
    }
    if (datGameState->header.transition != 0) {
        fldPrepareDeferredSceneTransition();
        return;
    }
    func_00120C08(1);
    D_0032E570[0x4C / 4] = 0;
    D_0032E570[0x44 / 4] = 1;
    mdlFlagSet(0xC0F);
    fldSetDeferredFieldCommand(0, 0);
    fldInitializeSequenceAndResetFlags(&record, 1, 1, D_003BAB68);
    record.options = 1;
    dds3AdminSubmitModeRequest(5, &record, 0xA0, 0);
}

extern u64 D_003BAB70[], D_003BAB78[], D_003BAB80[], D_003BAB88[], D_003BAB90[];
extern u64 D_003BAB98[], D_003BABA0[], D_003BABA8[], D_003BABB0[];
extern u8 D_0032F240[];
extern void func_003003F0(void *);
extern void fldInitDisplayObjects(void);
extern void fldResetPlayerSceneTransformState(void);
extern void fldResetPendingSounds(void);
extern void effMiscSeedRandom(void *, s32);
extern void fldParseMixLb(void);
extern void fldLoadBattleSkyAndFilter(void);
extern void func_001426E0(void);

void fldInitializeDisplayAndTables(void) {
    func_003003F0(D_003BAB70);
    fldInitDisplayObjects();
    func_003003F0(D_003BAB78);
    fldResetPlayerSceneTransformState();
    func_003003F0(D_003BAB80);
    fldResetPendingSounds();
    func_003003F0(D_003BAB88);
    effMiscSeedRandom(D_0032F240, 0x1e240);
    func_003003F0(D_003BAB90);
    fldParseMixLb();
    func_003003F0(D_003BAB98);
    fldLoadBattleSkyAndFilter();
    func_003003F0(D_003BABA0);
    func_001426E0();
    func_003003F0(D_003BABA8);
    fldLoadFieldTables();
    func_003003F0(D_003BABB0);
}

void fldResetPlayerSceneTransformState(void) {
    FieldPlayerSceneWork *sceneWork = &D_0032F1A0;

    VU0_STORE_VF(vf0, &sceneWork->position);
    VU0_STORE_VF(vf0, &sceneWork->rotation);
    fldPlayerObject = 0;
    *fldGetPlayerSceneStateAddress() = 0;
}


/* Fill a sequence packet for stage/kind/name, then clear the temporary override.
 * Unspecified packet bytes deliberately retain their previous contents. */
void fldInitializeSequenceAndResetFlags(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name) {
    if (kwlnTaskGetTaskByName(D_0039FBC0) != NULL) {
        if (fldAreaState.area == stage) {
            fldAreaState.unk20 = 1;
        } else {
            dds3WorkClear();
        }
    }
    record->unk_30 = 0;
    record->unk_3c = 0;
    record->unk_38 = 0;
    strcpy(record->name, name);
    record->stage = stage;
    record->enabled = 1;
    record->mode = 2;
    record->kind = kind;
    record->code = 0;
    record->unk_62 = 0;
    record->link = 0;
    memset(record->detail, 0, sizeof(record->detail));
    memset(record->note, 0, sizeof(record->note));
    record->options = 0;
    D_003BAB3C = 0;
    D_0032C9A0[0] = 0;
}

/* Fill the alternate-mode sequence packet for stage/kind/name.
 * Reuse a running sequence in the same area; otherwise request a work clear. */
void fldInitializeAlternateSequence(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name) {
    if (kwlnTaskGetTaskByName(D_0039FBC0) != NULL) {
        if (fldAreaState.area == stage) {
            fldAreaState.unk20 = 1;
        } else {
            dds3WorkClear();
        }
    }
    record->unk_30 = 0;
    record->unk_3c = 0;
    record->unk_38 = 0;
    strcpy(record->name, name);
    record->stage = stage;
    record->enabled = 1;
    record->mode = 3;
    record->kind = kind;
    record->code = 0;
    record->unk_62 = 0;
    record->link = 0;
    memset(record->detail, 0, sizeof(record->detail));
    memset(record->note, 0, sizeof(record->note));
    record->options = 0;
}

/* Fill a field sequence packet, including its narrowed code, link and detail name.
 * A running sequence in the same area requests reuse rather than a work clear. */
void fldInitializeFieldSequenceRecord(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name,
                    s32 code, s32 link, const char *subname) {
    if (kwlnTaskGetTaskByName(D_0039FBC0) != NULL) {
        if (fldAreaState.area == stage) {
            fldAreaState.unk20 = 1;
        } else {
            dds3WorkClear();
        }
    }
    record->unk_30 = 0;
    record->unk_3c = 0;
    record->unk_38 = 0;
    strcpy(record->name, name);
    record->stage = stage;
    record->enabled = 1;
    record->kind = kind;
    record->code = code;
    record->link = link;
    record->mode = 2;
    record->unk_62 = 0;
    strcpy(record->detail, subname);
    memset(record->note, 0, sizeof(record->note));
    record->options = 0;
}

/* Fill a sequence packet with a narrowed code and note instead of a detail/link.
 * A running sequence in the same area requests reuse rather than a work clear. */
void fldInitializeSequenceWithNote(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name,
                    s32 code, const char *subname) {
    if (kwlnTaskGetTaskByName(D_0039FBC0) != NULL) {
        if (fldAreaState.area == stage) {
            fldAreaState.unk20 = 1;
        } else {
            dds3WorkClear();
        }
    }
    record->unk_30 = 0;
    record->unk_3c = 0;
    record->unk_38 = 0;
    strcpy(record->name, name);
    record->stage = stage;
    record->enabled = 1;
    record->mode = 2;
    record->kind = kind;
    record->code = code;
    record->unk_62 = 0;
    record->link = 0;
    memset(record->detail, 0, sizeof(record->detail));
    strcpy(record->note, subname);
    record->options = 0;
}

void fldInitializeLinkedSequence(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name,
                    s32 code, s32 link, const char *subname) {
    record->unk_30 = 0;
    record->unk_3c = 0;
    record->unk_38 = 0;
    strcpy(record->name, name);
    record->enabled = 1;
    record->stage = stage;
    record->kind = kind;
    record->code = code;
    record->link = link;
    record->mode = 2;
    record->unk_62 = 0;
    strcpy(record->detail, subname);
    memset(record->note, 0, sizeof(record->note));
    record->options = 0;
}

/* Converts the field work's primary and secondary state words into a
 * compact scene status, giving the primary state precedence. */
u32 fldGetSceneStatusCode(void) {
    FieldPlayerSceneWork *sceneWork = &D_0032F1A0;

    if (sceneWork->primaryState == 1) {
        return 1;
    }
    if (sceneWork->secondaryState == 3) {
        return 3;
    }
    if (sceneWork->secondaryState == 4) {
        return 4;
    }
    if (sceneWork->secondaryState == 5) {
        return 5;
    }
    if (sceneWork->secondaryState == 6) {
        return 2;
    }
    return sceneWork->primaryState != 0 ? 2 : 0;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001233D0);

void fldUnloadPlayerModel(void);
extern u32 sdfReadNamedResource(const char *, u32 *, u32 *);
/* Load the coordinate/field-selected player variant only when it changes or its
 * resource is absent; the cached variant is part of native area work. */
INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FBC0);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FBD0);

void fldLoadPlayerModel(void) {
    s32 model;

    if (fldGetLocationCoordinateValue(fldAreaState.area, fldAreaState.floor + 1) & 0x20) {
        model = 2;
    } else {
        model = fldAreaState.unk118 != 0;
    }
    if (fldAreaState.playerModelVariant != model || fldPlayerModelResource == 0) {
        if (fldPlayerModelResource != 0) {
            fldUnloadPlayerModel();
        }
        switch (model) {
        case 0:
            fldPlayerModelResource = sdfReadNamedResource("/model/field/player_a.PB", &D_003BAB5C, &D_003BAB64);
            break;
        case 1:
            fldPlayerModelResource = sdfReadNamedResource("/model/field/player_b.PB", &D_003BAB5C, &D_003BAB64);
            break;
        default:
            fldPlayerModelResource = sdfReadNamedResource("/model/field/player_l.PB", &D_003BAB5C, &D_003BAB64);
            break;
        }
        fldAreaState.playerModelVariant = model;
    }
}

void fldUnloadPlayerModel(void) {
    if (fldPlayerModelResource != 0) {
        sdfQueueNonzeroResourceId(fldPlayerModelResource);
        fldPlayerModelResource = 0;
        D_0032E4EC[0] = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001239C8);

void fldPrepareResourceBuffer(void) {
    void *source;
    void *buffer;
    fldLoadPlayerModel();
    D_003BAB60 = D_003BAB64;
    D_003BAB58 = sdfAllocGeneralBlock(D_003BAB64);
    source = sdfMemoryGetBlockAddress(fldPlayerModelResource);
    buffer = sdfMemoryGetBlockAddress(D_003BAB58);
    memcpy(buffer, source, D_003BAB60);
    D_003BAB5C = (u32)buffer;
    mdlLoadViewerPackage(2, 0, 0x101, buffer, D_003BAB60);
    sdfQueueNonzeroResourceId(D_003BAB58);
    D_003BAB58 = 0;
}

void fldReleaseResources(void) {
    if (D_003BAB58 != 0) {
        sdfQueueNonzeroResourceId(D_003BAB58);
        D_003BAB58 = 0;
    }
    if (D_0032E3D0[0] == 0 && D_003BABE8 == 0) {
        fldUnloadPlayerModel();
        fldSetPendingAreaAndFloor(0, 0);
    }
}

void fldReleasePlayerSceneResources(void) {
    FieldPlayerSceneWork *sceneWork;

    if (fldPlayerObject != 0) {
        if (dds3GetWorldSecondaryObject() != 0) {
            sceneWork = &D_0032F1A0;
            effObjFetchInnerFirstVec(fldPlayerObject);
            VU0_STORE_VF(vf10, &sceneWork->position);
            effObjFetchInnerSecondVecNorm(fldPlayerObject);
            VU0_STORE_VF(vf10, &sceneWork->rotation);
        }
        fldPlayerObject = 0;
        fldCameraModelObject = 0;
        *fldGetPlayerSceneStateAddress() = 0;
        fldReleaseResources();
    }
}

void fldCleanupFieldScene(void) {
    fldResetTaskSlots();
    fldTestDrawDestroy();
    fldReleasePlayerSceneResources();
    evtDestroySecondaryWorldNode();
    fldReleaseCampSceneTasks();
}

u32 * fldGetPlayerSceneStateAddress(void) {
    return &D_003BABD0;
}

u32 fldGetPlayerSceneState(void) {
    return *fldGetPlayerSceneStateAddress();
}

u32 func_001243C0(void);
extern void func_001372D0(f32 *position);
void fldSetSceneControlFlags(u32 mask);

/* Raise the scene-state minima, clear pending slots and aim ten units above
 * the fetched player position. Existing larger mode/state values are retained. */
void fldPreparePlayerSceneCameraTarget(void) {
    f32 position[4];

    if (fldPlayerObject != 0) {
        fldSetSceneControlFlags(0x40);
        dds3InvokeSlot1Handler(fldPlayerObject, func_001243C0);
    }
    fldAreaState.unkE8 = 4;
    if (fldAreaState.sceneMode < 4) {
        fldAreaState.sceneMode = 4;
    }
    if (fldAreaState.sceneState < 5) {
        fldAreaState.sceneState = 5;
    }
    fldAreaState.unk90 = -1;
    fldAreaState.unk94 = -1;
    fldUpdateCameraTarget();
    effObjFetchInnerFirstVec(fldPlayerObject);
    VU0_STORE_VF(vf10, position);
    position[1] += 10.0f;
    func_001372D0(position);
}

void fldResetPlayerSceneObjectState(void) {
    if (fldPlayerObject != 0) {
        fldClearSceneControlFlags(0x40);
        dds3InvokeSlot1Handler(fldPlayerObject, 0);
    }
    D_0032E498[0] = 4;
}

extern void dds3ClearObjectFlags(u32, s32);
extern void dds3SetWorldPlayerObject(u64, u32);
extern void func_00111E30(u32, s32, s32);
extern u32 dds3SpawnCameraSlotObj5(s32, f32 *, f32 *);
extern s32 D_0032F1DC[];
extern char D_0039FC50[]; /* "PLAYER_UNIT" */
extern s32 D_003BAB50;
/* Initialize the homogeneous vectors and create/register the player if absent. */
void fldCreatePlayerObject(void) {
    f32 position[4];
    f32 rotation[4];

    memset(position, 0, sizeof(position));
    position[3] = 1.0f;
    memset(rotation, 0, sizeof(rotation));
    rotation[3] = 1.0f;
    if (fldPlayerObject == 0) {
        fldPlayerObject = dds3SpawnCameraSlotObj5(dds3AdvanceWorldCounter(), position, rotation);
        dds3SetWorldEntryCallbackTarget((void *)fldPlayerObject, D_0039FC50);
        dds3SetWorldPlayerObject(dds3GetWorldSecondaryObject(), fldPlayerObject);
        if (D_003BAB50 != 0) {
            dds3ClearObjectFlags(fldPlayerObject, 0x20);
        }
        fldPrepareResourceBuffer();
        func_00111E30(fldPlayerObject, 2, D_0032F1DC[0]);
    }
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00123FB8);

typedef struct FieldVec4 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} FieldVec4;

extern FieldVec4 D_0039FC80;
extern FieldVec4 D_0039FC90;
extern char D_003BABC0[];
extern u32 dds3CreateConfiguredCameraObject(s32, FieldVec4 *, FieldVec4 *, FieldVec4 *);
extern void dds3SetCameraVector(u32, FieldVec4 *);
extern void effObjSetInnerFloat(u32, f32);
extern void dds3SetWorldCameraObject(u64, u32);

/* Create the secondary camera at target origin with the stored eye/up vectors. */
void fldCreateSecondaryWorldCamera(void) {
    FieldVec4 localUp = D_0039FC80;
    FieldVec4 targetPosition;
    FieldVec4 worldEye;
    u32 *cameraObjectSlot = &D_003BABD0;
    u32 cameraObject;
    memset(&targetPosition, 0, sizeof(targetPosition));
    targetPosition.w = 1.0f;
    worldEye = D_0039FC90;
    cameraObject = dds3CreateConfiguredCameraObject(dds3AdvanceWorldCounter(), &targetPosition, &worldEye, &localUp);
    *cameraObjectSlot = cameraObject;
    dds3SetWorldEntryCallbackTarget((void *)cameraObject, D_003BABC0);
    dds3SetCameraVector(*cameraObjectSlot, &worldEye);
    effObjSetInnerFloat(*cameraObjectSlot, 2.0f);
    dds3SetWorldCameraObject(dds3GetWorldSecondaryObject(), *cameraObjectSlot);
}

u32 func_001243C0(void) {
    return 0;
}

s32 fldSelectSceneCommand(void) {
    s32 mode = fldAreaState.area;
    s32 result = 0x64;
    if (mode == 0x15) {
        s32 variant = fldAreaState.floor;
        result = 0x66;
        if (variant == 2) result = 0x64;
        if (variant == 0x17) result = 0x65;
    }
    if (mode == 0x1a) {
        s32 variant = fldAreaState.floor;
        result = 0x66;
        if (variant == 0x12) result = 0x65;
        if (variant == 0x13) result = 0x65;
        if (variant == 0x15) result = 0x65;
    }
    if (mode == 0x1d) {
        s32 variant = fldAreaState.floor;
        result = 0x66;
        if (variant == 1) result = 0x64;
        if (variant == 9) result = 0x65;
        if (variant == 0xa) result = 0x65;
    }
    if (mode == 0x25 && fldAreaState.floor == 0xf) {
        result = 0x64;
    }
    return result;
}

void fldConsumeSceneCommandFlag(void) {
    if ((datGameState->world.fieldFlags & 8) != 0) {
        fldClearSceneCommandFlag();
        fldAreaState.consumedFlags |= 1;
    }
}

void fldClearSceneCommandFlag(void) {
    datGameState->world.fieldFlags &= ~8;
    fldAreaState.consumedFlags &= ~1;
}


extern void fldPlayFieldSeVolumePan(s32);
extern void func_00133640(s16, s32);
void fldEnterSceneCommand(void) {
    s32 code;

    if (fldAreaState.commandEnabled == 0) {
        return;
    }
    if ((datGameState->world.fieldFlags & 8) == 0) {
        fldPlayFieldSeVolumePan(0x29);
    }
    code = fldSelectSceneCommand();
    fldAreaState.sceneCommand = code;
    datGameState->world.fieldFlags |= 8;
    fldAreaState.consumedFlags &= ~1;
    func_00133640(code, 0);
}

s32 fldGetSceneCommandState(void) {
    if ((datGameState->world.fieldFlags & 8) != 0) {
        return 1;
    }
    if (D_0032E4DC[0] != 0) {
        return 0;
    }
    return -1;
}


/* Latch the area/floor command while enabled and flagged; stop it when either
 * gate clears. Disable passes transition parameter 0; flag removal passes 20. */
void fldUpdateSceneCommand(void) {
    FldAreaWork *state = &fldAreaState;
    s32 code;

    if (state->commandEnabled == 0) {
        if (state->sceneCommand != 0) {
            state->sceneCommand = 0;
            func_00133640(0, 0);
        }
    } else if ((datGameState->world.fieldFlags & 8) != 0) {
        if (state->sceneCommand == 0) {
            code = fldSelectSceneCommand();
            state->sceneCommand = code;
            func_00133640(code, 0x14);
        }
    } else if (state->sceneCommand != 0) {
        state->sceneCommand = 0;
        func_00133640(0, 0x14);
        fldPlayFieldSeVolumePan(0x2A);
    }
}

/* Game-state work flags and area-state consumption markers use different bits. */
enum {
    FIELD_TRANSITION_WORK_FLAG = 4,
    FIELD_SECONDARY_SCENE_WORK_FLAG = 2,
    FIELD_PRIMARY_SCENE_WORK_FLAG = 1,
    FIELD_TRANSITION_CONSUMED_FLAG = 2,
    FIELD_SECONDARY_SCENE_CONSUMED_FLAG = 4,
    FIELD_PRIMARY_SCENE_CONSUMED_FLAG = 8
};

/* Clear a set transition flag and mark its consumption in area state. */
void fldConsumeFieldTransitionFlag(void) {
    if ((datGameState->world.fieldFlags & FIELD_TRANSITION_WORK_FLAG) != 0) {
        fldClearFieldTransitionFlag();
        fldAreaState.consumedFlags |= FIELD_TRANSITION_CONSUMED_FLAG;
    }
}

/* Clear the transition work flag and its area-state consumption marker. */
void fldClearFieldTransitionFlag(void) {
    datGameState->world.fieldFlags &= ~FIELD_TRANSITION_WORK_FLAG;
    fldAreaState.consumedFlags &= ~FIELD_TRANSITION_CONSUMED_FLAG;
}

/* Set the transition work flag and reset its area-state consumption marker. */
void fldSetFieldTransitionFlag(void) {
    datGameState->world.fieldFlags |= FIELD_TRANSITION_WORK_FLAG;
    fldAreaState.consumedFlags &= ~FIELD_TRANSITION_CONSUMED_FLAG;
}

/* Return whether the transition work flag is set. */
u8 fldTestFieldTransitionFlag(void) {
    s32 fieldFlags = datGameState->world.fieldFlags;
    fieldFlags &= FIELD_TRANSITION_WORK_FLAG;
    return fieldFlags != 0;
}

/* Clear a set secondary-scene flag and mark its consumption in area state. */
void fldConsumeSecondarySceneFlag(void) {
    if ((datGameState->world.fieldFlags & FIELD_SECONDARY_SCENE_WORK_FLAG) != 0) {
        fldClearSecondarySceneFlag();
        fldAreaState.consumedFlags |= FIELD_SECONDARY_SCENE_CONSUMED_FLAG;
    }
}

/* Clear the secondary-scene work flag and its consumption marker. */
void fldClearSecondarySceneFlag(void) {
    datGameState->world.fieldFlags &= ~FIELD_SECONDARY_SCENE_WORK_FLAG;
    fldAreaState.consumedFlags &= ~FIELD_SECONDARY_SCENE_CONSUMED_FLAG;
}

/* Select secondary rather than primary, resetting secondary's consume marker. */
void fldSetSecondarySceneFlag(void) {
    datGameState->world.fieldFlags = (datGameState->world.fieldFlags | FIELD_SECONDARY_SCENE_WORK_FLAG) & ~FIELD_PRIMARY_SCENE_WORK_FLAG;
    fldAreaState.consumedFlags &= ~FIELD_SECONDARY_SCENE_CONSUMED_FLAG;
}

/* Return whether the secondary-scene work flag is set. */
u8 fldTestSecondarySceneFlag(void) {
    s32 fieldFlags = datGameState->world.fieldFlags;
    fieldFlags &= FIELD_SECONDARY_SCENE_WORK_FLAG;
    return fieldFlags != 0;
}

/* Clear a set primary-scene flag and mark its consumption in area state. */
void fldConsumePrimarySceneFlag(void) {
    if ((datGameState->world.fieldFlags & FIELD_PRIMARY_SCENE_WORK_FLAG) != 0) {
        fldClearPrimarySceneFlag();
        fldAreaState.consumedFlags |= FIELD_PRIMARY_SCENE_CONSUMED_FLAG;
    }
}

/* Clear the primary-scene work flag and its area-state consumption marker. */
void fldClearPrimarySceneFlag(void) {
    datGameState->world.fieldFlags &= ~FIELD_PRIMARY_SCENE_WORK_FLAG;
    fldAreaState.consumedFlags &= ~FIELD_PRIMARY_SCENE_CONSUMED_FLAG;
}

/* Select primary rather than secondary, resetting primary's consume marker. */
void fldSetPrimarySceneFlag(void) {
    datGameState->world.fieldFlags = (datGameState->world.fieldFlags | FIELD_PRIMARY_SCENE_WORK_FLAG) & ~FIELD_SECONDARY_SCENE_WORK_FLAG;
    fldAreaState.consumedFlags &= ~FIELD_PRIMARY_SCENE_CONSUMED_FLAG;
}

/* Return whether the primary-scene work flag is set. */
u8 fldIsFlagActive(void) {
    s32 fieldFlags = datGameState->world.fieldFlags;
    fieldFlags &= FIELD_PRIMARY_SCENE_WORK_FLAG;
    if (fieldFlags == 0) return 0;
    return 1;
}

void func_001248D0(void) {
    sdfStoreMessageWordsAndNotifyConsumer(D_003257F8, (s32)D_00324F88, (s32)(D_00324F88 + 0xC0), (s32)(D_00324F88 + 0x100), (s32)(D_00324F88 + 0xE0));
}


typedef struct FldEncEntry {
    s16 stage;
    s16 flag;
    s16 chance;
    s16 result;
} FldEncEntry;
extern FldEncEntry *fldEncounterRollTable;
extern s32 mdlFlagTest();
extern s32 effMiscRand();
enum {
    FIELD_ENCOUNTER_ROLL_ENTRY_COUNT = 16
};

#define FIELD_ENCOUNTER_PERCENT_RANGE 100U

/* Return the first successful stage/flag-gated roll, or -1 when none succeed.
 * Each eligible row rolls independently; chance retains its unsigned cast. */
s16 fldRollEncounter(void) {
    s32 encounterIndex;
    s32 eligible;

    for (encounterIndex = 0; encounterIndex < FIELD_ENCOUNTER_ROLL_ENTRY_COUNT; encounterIndex++) {
        if (fldEncounterRollTable[encounterIndex].stage == fldAreaState.area) {
            eligible = 1;
            if (fldEncounterRollTable[encounterIndex].flag != -1) {
                eligible = mdlFlagTest(fldEncounterRollTable[encounterIndex].flag) != 0;
            }
            if (eligible != 0) {
                if ((u32)effMiscRand(0) % FIELD_ENCOUNTER_PERCENT_RANGE < (u32)fldEncounterRollTable[encounterIndex].chance) {
                    return fldEncounterRollTable[encounterIndex].result;
                }
            }
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001249E0);

/* Queue the current area and next floor, reset the scene lifecycle, and return -1.
 * The pending pair is consumed later with a 200-entry offset on the area value. */
s32 fldAdvanceToNextScene(void) {
    u32 scene;
    u32 area;

    fldResetEventSceneState();
    D_003BAC08[0] = 0;
    D_003BAC08[1] = func_0014D100();
    scene = fldAreaState.floor;
    area = fldAreaState.area;
    fldAreaState.nextArea = area;
    fldAreaState.nextFloor = scene + 1;
    fldResetTaskSlots();
    fldSetSceneLifecycleFlags(1);
    fldSetSceneLifecycleFlags(2);
    fldResetPlayerSceneObjectState();
    func_00220178();
    return -1;
}

u8 fldHasKiretaLabelProcess(void) {
    return scrFindNamedProcessNode(D_0039FCA0) != 0;
}

u8 fldHasHirakenaiLabelProcess(void) {
    return scrFindNamedProcessNode(D_0039FCC8) != 0;
}

u8 fldHasBadkaifukuLabelProcess(void) {
    return scrFindNamedProcessNode(D_0039FCB0) != 0;
}

u8 fldGetCampSceneControlMode(void) {
    if (D_003BABEC > 0) {
        return 2;
    }
    if (mnuAcknowledgeCampState() != 0) {
        return 1;
    }
    return fldTestSceneControlFlags(0x20) != 0 ? 0 : 3;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00124F58);

u8 fldGetSceneReadyOrPendingState(void) {
    if (D_003BABF0 > 0) {
        return 2;
    }
    return fldGetSceneReadyFlag() != 0;
}

extern void *dds3GetWorldObject(void);
extern void dds3SetWorldObjectDataValue(EvtWorldObject *, s8);
extern void kwlnFadeStartIn(s32);
extern void kwlnFadeInStart(s8, s8, s8, s32);
extern void func_00145B18(void);
extern void func_00131688(void);
extern u8 fldHasPendingSceneFlags(void);
extern void fldSetCameraNodeModeWithTen(void);
extern void func_00121B88(s32, s32, f32, f32, f32);
extern void evtStartSceneResourceTask(u64, void *);

/* Gate next-floor input on camp/readiness flags, then start the native scene
 * transition or its delayed fade path. All return paths retain zero. */
s32 fldUpdateNextFloorTransition(void) {
    s32 transitionInput = 0;
    FldAreaWork *scene;

    if (fldGetCampSceneControlMode() != 0) {
        return 0;
    }
    if (D_0032E570[0x54 / 4] & fldTestSceneControlFlags(0x40)) {
        if ((s8)D_00324530[2] != 0) {
            transitionInput = 1;
        }
    } else if ((s8)D_00324530[2] < 0) {
        transitionInput = 1;
    }
    scene = &fldAreaState;
    if (fldFindLocationCoordinateRecord(scene->area, scene->floor + 1)[2] <= 0) {
        if (scene->sceneMode == 0) {
            if (fldTestSceneControlFlags(0x40) != 0 && transitionInput != 0) {
                scene->sceneMode = 4;
                scene->sceneState = 5;
                fldResetPlayerSceneObjectState();
                evtStartSceneResourceTask((u64)dds3GetWorldObject(), D_0039FCC8);
            }
        }
    } else {
        if (D_003BABF0 > 0) {
            D_003BABF0--;
            if (D_003BABF0 == 0) {
                dds3SetWorldObjectDataValue(dds3GetWorldObject(), 0);
                kwlnFadeStartIn(4);
                func_00145B18();
            }
            return 0;
        }
        if (fldGetSceneReadyOrPendingState() != 0) {
            return 0;
        }
        func_00121B88(scene->floor, scene->unkC0, scene->x, scene->z, 50.0f);
        if (scene->sceneMode == 0) {
            if (fldHasPendingSceneFlags() != 0) {
                return 0;
            }
            if (fldTestSceneControlFlags(0x40) != 0 && transitionInput != 0 && D_0032E570[0x48 / 4] == 0) {
                scene->sceneMode = 4;
                scene->sceneState = 5;
                fldResetPlayerSceneObjectState();
                func_00131688();
                kwlnFadeInStart(0, 0, 0, 4);
                D_003BABF0 = 5;
                fldSetCameraNodeModeWithTen();
            }
        }
    }
    return 0;
}

extern void func_0014C468(void);
/* Dispatch one pending field command, preferring the temporary override
 * over the scene-work buffer and its saved fallback. */
s32 fldDispatchPendingSceneResource(void) {
    FieldPlayerSceneWork *sceneWork = &D_0032F1A0;
    u32 overrideFlags;

    if (fldAreaState.unk100 == 1) {
        func_0014C468();
    }
    fldAreaState.unk100 = 0;
    if ((D_003BAB3C & 2) && *(s8 *)D_0032C9A0 != 0) {
        evtStartSceneResourceTask(dds3GetWorldSecondaryObject(), D_0032C9A0);
        overrideFlags = D_003BAB3C;
        if (!(overrideFlags & 1)) {
            D_0032C9A0[0] = 0;
            D_003BAB3C = overrideFlags & 0xFB;
        }
        return 1;
    }
    if (sceneWork->primaryState == 0 && sceneWork->resourceName[0] != 0) {
        evtStartSceneResourceTask(dds3GetWorldSecondaryObject(), sceneWork->resourceName);
        return 1;
    }
    if (fldAreaState.fallbackResourceName != 0) {
        evtStartSceneResourceTask(dds3GetWorldSecondaryObject(), fldAreaState.fallbackResourceName);
        return 1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FC40);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FC50);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FC60);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FC70);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FC80);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FC90);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FCA0);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FCB0);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FCC8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", fldProcSequence);

INCLUDE_ASM(const s32, "game/code_0011D3A0", fldProcDraw);

/* Independent flag words: fldSceneLifecycleFlags and fldSceneControlFlags each have
 * their own set, clear and test operations. */
void fldSetSceneLifecycleFlags(u32 mask) {
    u32 *flags = &fldSceneLifecycleFlags;
    *flags |= mask;
}

void fldClearSceneLifecycleFlags(u32 mask) {
    u32 *flags = &fldSceneLifecycleFlags;
    *flags &= ~mask;
}

u8 fldTestSceneLifecycleFlags(u32 mask) {
    return (fldSceneLifecycleFlags & mask) != 0;
}

void fldSetSceneControlFlags(u32 mask) {
    fldSceneControlFlags = fldSceneControlFlags | mask;
}

void fldClearSceneControlFlags(u32 mask) {
    fldSceneControlFlags = fldSceneControlFlags & ~mask;
}

u8 fldTestSceneControlFlags(u32 mask) {
    return (fldSceneControlFlags & mask) != 0;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00125E08);

extern s32 D_003BABF4, D_003BABE4;
extern char D_0039FD30[], D_0039FD40[];
extern void kwlnFadeResetBackground(void);
extern void fldDestroyPanelTaskIfPresent(void), evtSetSolarOverlayFullyTransparent(void), fldDestroyTask(void);
extern void mnuDestroyCampTasks(void), scrDestroyAllNamedProcesses(void);
extern void fldReleaseMenuSlotsAfterWait(void);

void fldReleaseCampSceneTasks(void) {
    if (D_003BABF4 == 0) return;
    D_003BABF4 = 0;
    kwlnFadeResetBackground();
    fldClearSceneControlFlags(0x10);
    fldClearSceneControlFlags(0x20);
    fldSetSceneLifecycleFlags(1);
    fldSetSceneLifecycleFlags(2);
    kwlnTaskDestroyWithHierarchyByName(D_0039FD30, 1);
    kwlnTaskDestroyWithHierarchyByName(D_0039FD40, 1);
    fldDestroyPanelTaskIfPresent();
    evtSetSolarOverlayFullyTransparent();
    fldDestroyTask();
    if (D_003BABE4 != 0) {
        effBlurReleaseFirstResource((EffBlurScatterWork *)D_003BABE4);
        D_003BABE4 = 0;
    }
    mnuDestroyCampTasks();
    scrDestroyAllNamedProcesses();
    fldReleaseMenuSlotsAfterWait();
}

u8 fldIsFieldResourceWaitFinished(void) {
    if (D_0032E3D0[0] == 0 && fldIsAreaResourceReady() != 0) {
        fldPollAreaResourceLoad();
        if (fldGetResourceReadyFlag() == 1) return 0;
        fldFreeDisplayObjects();
        return 0;
    }
    return sdfCheckPendingWorkWithInterrupts() == 0;
}

void func_00126038(void) {
    fldSelectActorFromSceneIndexTables();
}

void fldStopSceneBgm(void) {
    fldStopCurrentBgm();
}

void fldProcessDeferredSceneCommand(void) {
    if (fldDeferredCommand != 0) {
        fldDispatchDeferredFieldCommand();
        return;
    }
    func_0013EC68();
}

void fldSetDeferredFieldCommand(u32 command, u32 parameter) {
    fldDeferredCommand = command;
    fldDeferredCommandParameter = parameter;
}

void fldDispatchDeferredFieldCommand(void) {
    if (fldDeferredCommand == 0) return;
    if (dds3AdminGetRequestedMode() > 0) return;
    if (dds3AdminGetRequestedMode() < 0 && (dds3AdminReadPreviousUnsignedSample() & 1) != 0) return;
    func_0013F100(fldDeferredCommand, fldDeferredCommandParameter);
    fldDeferredCommand = 0;
}

void fldSetPendingSceneAction(u32 argument) {
    D_003BABE0 = argument;
}

void fldRunPendingSceneAction(void) {
    u32 argument;

    argument = D_003BABE0;
    if (argument != 0) {
        func_0013E5A8(argument);
        D_003BABE0 = 0;
    }
}

/* Consume the pending pair of field-script values. -1 in both slots means
 * no request; the first value is returned with its 200-entry base offset. */
s32 fldConsumeNextSceneRequest(s32 *outCode, s32 *outParameter) {
    if (fldAreaState.nextArea == -1 && fldAreaState.nextFloor == -1) {
        return 0;
    }
    *outCode = fldAreaState.nextArea + 0xC8;
    *outParameter = fldAreaState.nextFloor;
    fldAreaState.nextArea = -1;
    fldAreaState.nextFloor = -1;
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FD30);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FD40);

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

INCLUDE_SDATA(const s32, "game/code_0011D3A0", fldCameraModelObject);

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

