#include "common.h"
#include "pcp_vu0.h"
#include "fpu.h"

#include "fld.h"
#include "kwln.h"

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

extern s32 fldDeferredCommand;

extern void *dds3GetWorldSecondaryObject(void);

extern s64 dds3GetWorldValueCount(u64);

extern u64 dds3ReadIndexedWorldObjectWord(u64);

extern s64 dds3AdvanceObjectValueCursor(u64);

extern u64 dds3CopyWorldListToValueChain(u64, u64);

extern s64 evtGetObjectTransitionWork(u64);

extern s32 sdfAllocPacketAligned(s32 size);

extern s32 datGameState;

extern u32 D_00435F60;

extern u32 fldSceneLifecycleFlags;

extern u32 fldSceneControlFlags;

extern u32 D_00435F70;

extern s16 D_00389170[];

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

extern FieldPlayerSceneWork D_0038A640;

extern u32 fldPlayerModelResource;

extern u32 D_003898B8[];

extern void sdfQueueNonzeroResourceId(u32 arg0);

extern u32 fldPlayerObject;

extern u32 D_00389858[];

void fldClearSceneControlFlags(u32 arg0);

void fldClearSceneCommandFlag(void);

extern s32 D_0038989C[];

void fldClearFieldTransitionFlag(void);

void fldClearSecondarySceneFlag(void);

void fldClearPrimarySceneFlag(void);

extern char D_00412F30[];

extern s32 scrFindNamedProcessNode(const char *arg0);

extern char D_00412F58[];

extern char D_00412F40[];

extern s32 D_00435F7C;

extern u32 mnuAcknowledgeCampState(void);

u8 fldTestSceneControlFlags(u32 arg0);

extern s32 D_00435F80;

extern u32 fldGetSceneReadyFlag(void);

extern u32 fldDeferredCommandParameter;

extern void func_001411F8(u32 arg0);

extern u8 D_0039E1A8[];

extern u8 D_0039A5A8[];

extern u8 D_003A25A8[];

extern s16 D_00387D70[];

extern void *kwlnTaskGetTaskByName(const char *);

extern void dds3WorkClear(void);

extern char D_00412D50[]; /* "fldProcSequence" */
extern u8 D_00435F24;

extern u8 D_00387D60[];

/* Sequence command packet: the submission contract copies all 0xA0 bytes. */
typedef struct FieldSequenceRecord {
    u8 unk_00[0x30];
    u32 unk_30;
    u32 unk_34;
    u32 unk_38;
    u32 unk_3c;
    char name[16];
    s32 stage;
    s32 kind;
    s32 enabled;
    s32 mode;
    u16 code;
    u16 unk_62;
    s32 link;
    u8 unk_68[8];
    char detail[16];
    char note[16];
    u32 options; /* 0x90: record options */
    u8 pad94[0xC];
} FieldSequenceRecord;

typedef struct FieldStageCoordinate {
    s16 x;
    s16 y;
    f32 originX;
    f32 originZ;
    u8 cols;
    u8 rows;
    s16 cellSize;
} FieldStageCoordinate;

extern u32 D_00435F38;

extern u32 D_00435F44;

extern u32 D_00435F40;

extern u32 D_00435F3C;

extern u32 sdfAllocGeneralBlock(u32);

extern void *sdfMemoryGetBlockAddress(u32);

extern void mdlLoadViewerPackage(s32, s32, s32, void *, u32);

void func_001258B8(void);

extern u32 D_00389790[];

extern s32 fldIsAreaResourceReady(void);

extern u32 fldPollAreaResourceLoad(void);

extern u32 fldGetResourceReadyFlag(void);

extern void fldFreeDisplayObjects(void);

extern s32 sdfCheckPendingWorkWithInterrupts(void);

extern u32 dds3AdvanceWorldCounter(void);

extern void dds3SetWorldEntryCallbackTarget(void *, const char *);

extern f32 func_001248E8(f32, f32, f32, f32);

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

extern s32 kwlnTaskDestroyWithHierarchyByName(const char *name, s32 arg1);

extern s32 D_00435EE0;

extern s32 D_00435EE4;

extern s32 D_00435EE8;

extern s32 D_00435EEC;

extern s32 D_00435EF0;

extern s32 D_00435EF4;

extern s32 D_00435EF8;

extern void fldSetCameraMoveMode(s32 mode);


extern void fldClearCameraMoveMode(void);

extern u32 dds3ResetObjectValueCursor(u64);

extern s32 dds3TestObjectFlags(u64, s32);

extern void dds3DestroyWorldIndexNode(u64);

extern void fldSubmitBackgroundResourcePacket(void);

extern void fldSubmitBackgroundDescriptorPacket(void);

extern s32 D_00389780[];

extern void mdlFlagSet(s32);

extern void sdfResetGameRuntime(s32);

extern s32 fileLoadStateChanged(void);

extern void fileCacheSlotFlagsFromState(void);

extern void fileRestoreSlotFlagsToState(void);

void fldSetDeferredFieldCommand(u32, u32);

extern void dds3AdminSubmitModeRequest(s32, void *, s32, s32);

u32 *fldGetPlayerSceneStateAddress(void);

extern void fldResetPlayerSceneTransformState(void);

extern u32 D_00435F78;

extern void fldSetPendingAreaAndFloor(s32, s32);

void fldUnloadPlayerModel(void);

extern void dds3ClearObjectFlags(u32, s32);

extern void dds3SetWorldPlayerObject(void *object, u32 value);

extern void func_00112058(u32, s32, s32);

extern u32 dds3SpawnCameraSlotObj5(s32, f32 *, f32 *);

extern s32 D_0038A67C[];

extern char D_00412EC0[]; /* "PLAYER_UNIT" */

extern s32 D_00435F30;

extern u8 D_0037FF88[];

extern u8 D_003807F8[];

extern void sdfStoreMessageWordsAndNotifyConsumer(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

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
    u8 pad130[0x18];
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
    f32 unk178;
    f32 unk17C;
    f32 unk180;
    s32 positionMode;
    s32 unk188;
    s32 verticalStepDirection; /* Positive lowers Y; negative raises it. */
    s32 unk190;
    u32 pointState;
    f32 facingPointX;
    f32 facingPointZ;
    u32 angleState;
    f32 overrideAngle;
} FldAreaWork;
extern FldAreaWork fldAreaState;

typedef struct FldEncEntry {
    s16 stage;
    s16 flag;
    s16 chance;
    s16 result;
} FldEncEntry;

extern FldEncEntry *fldEncounterRollTable;

extern s32 mdlFlagTest();

extern s32 effMiscRand();

extern u32 D_00435F98[2];

extern void fldResetEventSceneState(void);

extern u32 func_00151498(void);

extern void fldResetTaskSlots(void);

extern void fldSetSceneLifecycleFlags(u32);

extern void func_0023ACE8(void);

extern void evtStartSceneResourceTask(u64, void *);

extern void func_00150800(void);

extern s32 D_00435F84, D_00435F74;

extern char D_00412FD0[], D_00412FE0[];

extern void kwlnFadeResetBackground(void);

extern void fldDestroyPanelTaskIfPresent(void), evtSetSolarOverlayFullyTransparent(void), fldDestroyTask(void);

extern void effBlurReleaseFirstResource(s32), mnuDestroyCampTasks(void), scrDestroyAllNamedProcesses(void);

extern void fldReleaseMenuSlotsAfterWait(void);

void fldSetSceneControlFlags(u32 mask);
u32 func_001266D8(void);
extern s32 dds3InvokeSlot1Handler(u32 object, void *context);
extern void fldUpdateCameraTarget(void);
extern void func_00139EC0(f32 *position);

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

INCLUDE_ASM(const s32, "game/code_0011F208", func_0011F250);

INCLUDE_ASM(const s32, "game/code_0011F208", func_0011F3D8);

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
extern void sdfAppendPacket(void *, void *);
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
    sdfAppendPacket(packetList, func_0011F250(x + 0x510, y + 0x18,
                     0xFF0080, 0x240, 0xF0, color, 0x40806020));
    for (i = 0; i != 3; i++) {
        sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(x, y,
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
        sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(x + 0x180,
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
    sdfAppendPacket(packetList, func_0011F250(x + 0x510, y + 0x18,
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
        sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(
                        x, y, 0xFF0080, style, label));
        sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(
                        x + 0x180, y, 0xFF0080, i == selected ? 6 : 0,
                        D_00435ED0, channel));
        if (showNormalized != 0) {
            sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(
                            x, y + 0x180, 0xFF0080, style, label));
            sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(
                            x + 0x240, y + 0x180, 0xFF0080, 0,
                            D_00435ED8, channel * (1.0f / 255.0f)));
        }
        y += 0x60;
    }
}

u32 func_001200E0(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_001200E8);

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
extern u32 sdfDevCreateCommandState(const char *);
extern u32 sdfDevQueueReadAndWait(u32, void *, u32);
extern void sdfDevWaitThenReleaseCommandState(u32);

/* Load every field table from FLDALL.TBL, then build D_003899F0: for each stage id (1..0x1E) the running total of the coordinate rows that precede its first record. Reads the coordinate table through the array each time: a pointer local assigned from D_00389170 would share its address with the argument use in a saved register across the calls. */
void fldLoadFieldTablesAndIndexStages(void) {
    u32 command = sdfDevCreateCommandState("/fld/f/bin/FLDALL.TBL");
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
        s16 id = ((FieldStageCoordinate *)D_00389170)[i].x;

        if (id > 0) {
            if (id < 0x1F) {
                if (id != previous) {
                    D_003899F0[id] = sum;
                }
                previous = id;
                sum += ((FieldStageCoordinate *)D_00389170)[i].rows;
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_00122A38);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00122B58);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00122E50);

/* A 30-byte map slot: bank 3 contains the map-target flags. */
typedef struct FieldMapSlot {
    u16 flagBanks[5];
    u8 values[0x10];
    u16 trailingFlagBanks[2];
} FieldMapSlot;

/* Map IDs select 1920-byte banks of 30-byte slots; flag banks are halfwords.
 * The guards check only the upper map-ID bound: negative IDs and slot/bit
 * ranges remain unchecked. */
enum {
    FIELD_MAP_ID_LIMIT = 40,
    FIELD_MAP_ID_MODULUS = 100,
    FIELD_MAP_BANK_BYTES = 1920,
    FIELD_MAP_SLOT_BYTES = 30,
    FIELD_MAP_EMPTY_VALUE = 0xff,
    FIELD_ROOM_MODE_BANK = 0,
    FIELD_ROOM_OBJECT_BANK = 1,
    FIELD_ROOM_SCENE_BANK = 2,
    FIELD_MAP_TARGET_BANK = 3,
    FIELD_MAP_ALTERNATE_TARGET_BANK = 4,
    FIELD_MAP_AUXILIARY_BANK = 0,
    FIELD_MAP_VALUE_BANK = 1
};

#define FIELD_MAP_SLOT_OFFSET 0x1450

/* Set or clear a room-mode bit in the selected map slot. */
void fldSetRoomModeFlag(s32 mapId, s32 slotIndex, s32 bitIndex, s32 enabled) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        if (enabled != 0) {
            s32 byteOffset = slotIndex * FIELD_MAP_SLOT_BYTES + mapIndex * FIELD_MAP_BANK_BYTES + FIELD_MAP_SLOT_OFFSET;
            u16 *flagBank = &((FieldMapSlot *)(datGameState + byteOffset))->flagBanks[FIELD_ROOM_MODE_BANK];
            *flagBank |= 1 << bitIndex;
        } else {
            s32 byteOffset = slotIndex * FIELD_MAP_SLOT_BYTES + mapIndex * FIELD_MAP_BANK_BYTES + FIELD_MAP_SLOT_OFFSET;
            u16 *flagBank = &((FieldMapSlot *)(datGameState + byteOffset))->flagBanks[FIELD_ROOM_MODE_BANK];
            *flagBank &= ~(1 << bitIndex);
        }
    }
}

/* Return the selected room-mode bit, or zero when mapId reaches the limit. */
u8 fldTestRoomModeFlag(s32 mapId, u32 slotIndex, u32 bitIndex) {
    s32 mapIndex;
    u8 *slotBase;
    u32 flagBits;

    if (mapId < FIELD_MAP_ID_LIMIT) {
        mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        slotBase = (u8 *)(slotIndex * FIELD_MAP_SLOT_BYTES + mapIndex * FIELD_MAP_BANK_BYTES);
        slotBase += datGameState;
        flagBits = ((FieldMapSlot *)(slotBase + FIELD_MAP_SLOT_OFFSET))->flagBanks[FIELD_ROOM_MODE_BANK];
        return (flagBits >> bitIndex) & 1;
    }
    return 0;
}

/* Set or clear a room-object-mode bit in the selected map slot. */
void fldSetRoomObjectModeFlag(s32 mapId, s32 slotIndex, s32 bitIndex, s32 enabled) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        if (enabled != 0) {
            u16 *flagBank = &((FieldMapSlot *)(slotIndex * FIELD_MAP_SLOT_BYTES + mapIndex * FIELD_MAP_BANK_BYTES + datGameState + FIELD_MAP_SLOT_OFFSET))->flagBanks[FIELD_ROOM_OBJECT_BANK];
            *flagBank |= 1 << bitIndex;
        } else {
            u16 *flagBank = &((FieldMapSlot *)(slotIndex * FIELD_MAP_SLOT_BYTES + mapIndex * FIELD_MAP_BANK_BYTES + datGameState + FIELD_MAP_SLOT_OFFSET))->flagBanks[FIELD_ROOM_OBJECT_BANK];
            *flagBank &= ~(1 << bitIndex);
        }
    }
}

/* Return the selected room-object-mode bit as a boolean. */
u8 fldTestRoomObjectModeFlag(s32 mapId, u32 slotIndex, u32 bitIndex) {
    s32 mapIndex;
    u8 *slotBase;
    u32 flagBits;

    if (mapId < FIELD_MAP_ID_LIMIT) {
        mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        slotBase = (u8 *)(slotIndex * FIELD_MAP_SLOT_BYTES + mapIndex * FIELD_MAP_BANK_BYTES);
        slotBase += datGameState;
        flagBits = ((FieldMapSlot *)(slotBase + FIELD_MAP_SLOT_OFFSET))->flagBanks[FIELD_ROOM_OBJECT_BANK];
        return (flagBits >> bitIndex) & 1;
    }
    return 0;
}

/* Set or clear a room-scene bit in the selected map slot. */
void fldSetRoomSceneFlag(s32 mapId, s32 slotIndex, s32 bitIndex, s32 enabled) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        if (enabled != 0) {
            u16 *flagBank = &((FieldMapSlot *)(slotIndex * FIELD_MAP_SLOT_BYTES + mapIndex * FIELD_MAP_BANK_BYTES + datGameState + FIELD_MAP_SLOT_OFFSET))->flagBanks[FIELD_ROOM_SCENE_BANK];
            *flagBank |= 1 << bitIndex;
        } else {
            u16 *flagBank = &((FieldMapSlot *)(slotIndex * FIELD_MAP_SLOT_BYTES + mapIndex * FIELD_MAP_BANK_BYTES + datGameState + FIELD_MAP_SLOT_OFFSET))->flagBanks[FIELD_ROOM_SCENE_BANK];
            *flagBank &= ~(1 << bitIndex);
        }
    }
}

/* Return the selected room-scene bit as a boolean. */
u8 fldTestRoomSceneFlag(s32 mapId, u32 slotIndex, u32 bitIndex) {
    s32 mapIndex;
    u8 *slotBase;
    u32 flagBits;

    if (mapId < FIELD_MAP_ID_LIMIT) {
        mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        slotBase = (u8 *)(slotIndex * FIELD_MAP_SLOT_BYTES + mapIndex * FIELD_MAP_BANK_BYTES);
        slotBase += datGameState;
        flagBits = ((FieldMapSlot *)(slotBase + FIELD_MAP_SLOT_OFFSET))->flagBanks[FIELD_ROOM_SCENE_BANK];
        return (flagBits >> bitIndex) & 1;
    }
    return 0;
}

/* Set or clear a map-target bit in the selected map slot. */
void fldSetMapTargetFlag(s32 mapId, s32 slotIndex, s32 bitIndex, s32 enabled) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        if (enabled != 0) {
            u16 *flagBank = &((FieldMapSlot *)(slotIndex * FIELD_MAP_SLOT_BYTES + mapIndex * FIELD_MAP_BANK_BYTES + datGameState + FIELD_MAP_SLOT_OFFSET))->flagBanks[FIELD_MAP_TARGET_BANK];
            *flagBank |= 1 << bitIndex;
        } else {
            u16 *flagBank = &((FieldMapSlot *)(slotIndex * FIELD_MAP_SLOT_BYTES + mapIndex * FIELD_MAP_BANK_BYTES + datGameState + FIELD_MAP_SLOT_OFFSET))->flagBanks[FIELD_MAP_TARGET_BANK];
            *flagBank &= ~(1 << bitIndex);
        }
    }
}

/* Return the selected map-target bit as a boolean. */
u8 fldTestMapTargetFlag(s32 mapId, u32 slotIndex, u32 bitIndex) {
    s32 mapIndex;
    u8 *slotBase;
    u32 flagBits;

    if (mapId < FIELD_MAP_ID_LIMIT) {
        mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        slotBase = (u8 *)(slotIndex * FIELD_MAP_SLOT_BYTES + mapIndex * FIELD_MAP_BANK_BYTES);
        slotBase += datGameState;
        flagBits = ((FieldMapSlot *)(slotBase + FIELD_MAP_SLOT_OFFSET))->flagBanks[FIELD_MAP_TARGET_BANK];
        return (flagBits >> bitIndex) & 1;
    }
    return 0;
}

/* Set or clear an alternate-map-target bit in the selected map slot. */
void fldSetAlternateMapTargetFlag(s32 mapId, s32 slotIndex, s32 bitIndex, s32 enabled) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        if (enabled != 0) {
            u16 *flagBank = &((FieldMapSlot *)(slotIndex * FIELD_MAP_SLOT_BYTES + mapIndex * FIELD_MAP_BANK_BYTES + datGameState + FIELD_MAP_SLOT_OFFSET))->flagBanks[FIELD_MAP_ALTERNATE_TARGET_BANK];
            *flagBank |= 1 << bitIndex;
        } else {
            u16 *flagBank = &((FieldMapSlot *)(slotIndex * FIELD_MAP_SLOT_BYTES + mapIndex * FIELD_MAP_BANK_BYTES + datGameState + FIELD_MAP_SLOT_OFFSET))->flagBanks[FIELD_MAP_ALTERNATE_TARGET_BANK];
            *flagBank &= ~(1 << bitIndex);
        }
    }
}

/* Return the selected alternate-map-target bit as a boolean. */
u8 fldTestAlternateMapTargetFlag(s32 mapId, u32 slotIndex, u32 bitIndex) {
    s32 mapIndex;
    u8 *slotBase;
    u32 flagBits;

    if (mapId < FIELD_MAP_ID_LIMIT) {
        mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        slotBase = (u8 *)(slotIndex * FIELD_MAP_SLOT_BYTES + mapIndex * FIELD_MAP_BANK_BYTES);
        slotBase += datGameState;
        flagBits = ((FieldMapSlot *)(slotBase + FIELD_MAP_SLOT_OFFSET))->flagBanks[FIELD_MAP_ALTERNATE_TARGET_BANK];
        return (flagBits >> bitIndex) & 1;
    }
    return 0;
}

/* Store a byte at valueOffset within the selected slot's values area. */
void fldSetMapSlotByte(s32 mapId, u32 slotIndex, s32 valueOffset, s32 value) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        s32 byteDisplacement = valueOffset + mapIndex * FIELD_MAP_BANK_BYTES;
        u8 *slotCursor = (u8 *)(slotIndex * FIELD_MAP_SLOT_BYTES + byteDisplacement);
        slotCursor += datGameState;
        ((FieldMapSlot *)(slotCursor + FIELD_MAP_SLOT_OFFSET))->values[0] = value;
    }
}

/* Read a slot value, translating stored 0xff to -1; IDs >= the limit return zero. */
s32 fldGetMapSlotByte(s32 mapId, u32 slotIndex, s32 valueOffset) {
    u8 value = 0;
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        s32 byteDisplacement = valueOffset + mapIndex * FIELD_MAP_BANK_BYTES;
        u8 *slotCursor = (u8 *)(slotIndex * FIELD_MAP_SLOT_BYTES + byteDisplacement);
        slotCursor += datGameState;
        value = ((FieldMapSlot *)(slotCursor + FIELD_MAP_SLOT_OFFSET))->values[0];
    }
    return value == FIELD_MAP_EMPTY_VALUE ? -1 : value;
}

/* Set or clear an auxiliary bit in the first trailing halfword bank. */
void fldSetMapSlotAuxiliaryFlag(s32 mapId, s32 slotIndex, s32 bitIndex, s32 enabled) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        if (enabled != 0) {
            u16 *flagBank = &((FieldMapSlot *)(slotIndex * FIELD_MAP_SLOT_BYTES + mapIndex * FIELD_MAP_BANK_BYTES + datGameState + FIELD_MAP_SLOT_OFFSET))->trailingFlagBanks[FIELD_MAP_AUXILIARY_BANK];
            *flagBank |= 1 << bitIndex;
        } else {
            u16 *flagBank = &((FieldMapSlot *)(slotIndex * FIELD_MAP_SLOT_BYTES + mapIndex * FIELD_MAP_BANK_BYTES + datGameState + FIELD_MAP_SLOT_OFFSET))->trailingFlagBanks[FIELD_MAP_AUXILIARY_BANK];
            *flagBank &= ~(1 << bitIndex);
        }
    }
}

/* Return the selected auxiliary bit as a boolean. */
u8 fldTestMapSlotAuxiliaryFlag(s32 mapId, u32 slotIndex, u32 bitIndex) {
    s32 mapIndex;
    u8 *slotBase;
    u32 flagBits;

    if (mapId < FIELD_MAP_ID_LIMIT) {
        mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        slotBase = (u8 *)(slotIndex * FIELD_MAP_SLOT_BYTES + mapIndex * FIELD_MAP_BANK_BYTES);
        slotBase += datGameState;
        flagBits = ((FieldMapSlot *)(slotBase + FIELD_MAP_SLOT_OFFSET))->trailingFlagBanks[FIELD_MAP_AUXILIARY_BANK];
        return (flagBits >> bitIndex) & 1;
    }
    return 0;
}

/* Set or clear a value flag in the second trailing halfword bank. */
void fldSetMapSlotValueFlag(s32 mapId, s32 slotIndex, s32 bitIndex, s32 enabled) {
    if (mapId < FIELD_MAP_ID_LIMIT) {
        s32 mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        if (enabled != 0) {
            u16 *flagBank = &((FieldMapSlot *)(slotIndex * FIELD_MAP_SLOT_BYTES + mapIndex * FIELD_MAP_BANK_BYTES + datGameState + FIELD_MAP_SLOT_OFFSET))->trailingFlagBanks[FIELD_MAP_VALUE_BANK];
            *flagBank |= 1 << bitIndex;
        } else {
            u16 *flagBank = &((FieldMapSlot *)(slotIndex * FIELD_MAP_SLOT_BYTES + mapIndex * FIELD_MAP_BANK_BYTES + datGameState + FIELD_MAP_SLOT_OFFSET))->trailingFlagBanks[FIELD_MAP_VALUE_BANK];
            *flagBank &= ~(1 << bitIndex);
        }
    }
}

/* Return the selected value flag as a boolean. */
u8 fldTestMapSlotValueFlag(s32 mapId, u32 slotIndex, u32 bitIndex) {
    s32 mapIndex;
    u8 *slotBase;
    u32 flagBits;

    if (mapId < FIELD_MAP_ID_LIMIT) {
        mapIndex = mapId % FIELD_MAP_ID_MODULUS;
        slotBase = (u8 *)(slotIndex * FIELD_MAP_SLOT_BYTES + mapIndex * FIELD_MAP_BANK_BYTES);
        slotBase += datGameState;
        flagBits = ((FieldMapSlot *)(slotBase + FIELD_MAP_SLOT_OFFSET))->trailingFlagBanks[FIELD_MAP_VALUE_BANK];
        return (flagBits >> bitIndex) & 1;
    }
    return 0;
}

extern void fldActivateObjectById(s32);
extern void func_0011C6A0(s32);

#define FIELD_ACTIVATION_FLAGS_OFFSET 0x110D0

void fldActivateFlaggedObject(s32 flagIndex) {
    s32 byteOffset = (flagIndex >> 3) + FIELD_ACTIVATION_FLAGS_OFFSET;
    u8 *byte = (u8 *)(datGameState + byteOffset);
    FieldActivationRecord *entry;
    *byte |= 1 << (flagIndex & 7);
    fldActivateObjectById(flagIndex);
    if ((u32)(flagIndex - 0xF0) < 16) {
        mdlFlagSet(flagIndex + 0x610);
    }
    entry = (FieldActivationRecord *)(flagIndex * 16 + (s32)D_003A8EB0);
    if (entry->kind == 2) {
        func_0011C6A0(entry->parameter);
    }
}


u8 fldTestObjectActivationFlag(u32 flagIndex) {
    return (*(u8 *)(((s32)flagIndex >> 3) + datGameState + FIELD_ACTIVATION_FLAGS_OFFSET) >> (flagIndex & 7)) & 1;
}

/* Search records 1..511 of the 30-byte coordinate table; return zero on miss. */
s32 func_001237B0(s32 x, s32 y) {
    s32 index;
    for (index = 1; index < FIELD_COORDINATE_SCAN_LIMIT; index++) {
        if (x == *(s16 *)(D_0039A5A8 + index * 0x1E) && y == *(s16 *)(D_0039A5A8 + index * 0x1E + 2)) {
            return index;
        }
    }
    return 0;
}

/* Search the second coordinate table (34-byte records), reserving index zero. */
s32 func_00123808(s32 x, s32 y) {
    s32 index;
    for (index = 1; index < FIELD_COORDINATE_SCAN_LIMIT; index++) {
        if (x == *(s16 *)(D_0039E1A8 + index * 0x22) && y == *(s16 *)(D_0039E1A8 + index * 0x22 + 2)) {
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
    FieldStageCoordinate *record = (FieldStageCoordinate *)D_00389170;
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
    FieldStageCoordinate *records = (FieldStageCoordinate *)D_00389170;
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
s16 *fldFindStageCoordinateRecord(s32 x, s32 y) {
    FieldStageCoordinate *record = (FieldStageCoordinate *)D_00389170;
    s32 index = 0;

    do {
        if (record->x == x && record->y == y) {
            return (s16 *)record;
        }
        index++;
        record++;
    } while (index < FIELD_STAGE_RECORD_SCAN_LIMIT);
    return NULL;
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_00123A58);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00123B88);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00123DE8);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00123EE0);

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

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412CA8);

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

extern f32 fldAngleDifference(f32, f32);

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


INCLUDE_ASM(const s32, "game/code_0011F208", func_001248E8);

f32 fldGetNormalizedComplementaryAngle(f32 ax, f32 ay, f32 bx, f32 by) {
    s32 angle = (s32)(360.0f - func_001248E8(ax, ay, bx, by) + 90.0f);
    return (f32)(angle % 360);
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_00124A10);

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

void fldPrepareDeferredSceneTransition(void) {
    if (*(s16 *)(datGameState + 0xe) != 0) {
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
        D_00389988[0x44 / 4] = 1;
        code = 0x259;
        D_00389988[0x4c / 4] = 0;
        fldSetDeferredFieldCommand(0x15, 0x259);
        dds3AdminSubmitModeRequest(6, &code, 4, 0);
    }
}

extern u32 fileGetSelectionPendingFlag(void);
extern void func_00122B58();
extern char D_00435F48[];

void fldStartSequenceRecord(void) {
    FieldSequenceRecord record;
    u32 mode;

    if (fileGetSelectionPendingFlag() == 1) {
        return;
    }
    if (*(s16 *)(datGameState + 0xE) != 0) {
        mode = 1;
        dds3AdminSubmitModeRequest(0x1E, &mode, 4, 0);
        return;
    }
    func_00122B58(1);
    D_00389988[0x4C / 4] = 0;
    D_00389988[0x44 / 4] = 1;
    mdlFlagSet(0xC0F);
    fldSetDeferredFieldCommand(0, 0);
    fldInitializeSequenceAndResetFlags(&record, 1, 1, D_00435F48);
    record.options = 1;
    dds3AdminSubmitModeRequest(5, &record, 0xA0, 0);
}

extern void fldInitDisplayObjects();
extern void fldResetPendingSounds();
extern void effMiscSeedRandom();
extern void fldParseMixLb();
extern void func_001343E8();
extern void func_00145818();
extern void fldLoadFieldTablesAndIndexStages();
extern u8 D_0038A6E0[];

void fldInitializeDisplayAndSceneSound(void) {
    fldInitDisplayObjects();
    fldResetPlayerSceneTransformState();
    fldResetPendingSounds();
    effMiscSeedRandom(D_0038A6E0, 0x1E240);
    fldParseMixLb();
    func_001343E8();
    func_00145818();
    fldLoadFieldTablesAndIndexStages();
}

void fldResetPlayerSceneTransformState(void) {
    FieldPlayerSceneWork *sceneWork = &D_0038A640;
    VU0_STORE_VF(vf0, &sceneWork->position);
    VU0_STORE_VF(vf0, &sceneWork->rotation);
    fldPlayerObject = 0;
    *fldGetPlayerSceneStateAddress() = 0;
}

/* Fill a sequence packet for stage/kind/name, then clear the temporary override.
 * Unspecified packet bytes deliberately retain their previous contents. */
void fldInitializeSequenceAndResetFlags(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name) {
    if (kwlnTaskGetTaskByName(D_00412D50) != NULL) {
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
    D_00435F24 = 0;
    D_00387D60[0] = 0;
}

/* Fill the alternate-mode sequence packet for stage/kind/name.
 * Reuse a running sequence in the same area; otherwise request a work clear. */
void fldInitializeAlternateSequence(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name) {
    if (kwlnTaskGetTaskByName(D_00412D50) != NULL) {
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
    if (kwlnTaskGetTaskByName(D_00412D50) != NULL) {
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
    record->kind = kind;
    record->enabled = 1;
    record->mode = 2;
    record->code = code;
    record->unk_62 = 0;
    record->link = link;
    strcpy(record->detail, subname);
    memset(record->note, 0, sizeof(record->note));
    record->options = 0;
}


/* Fill a sequence packet with a narrowed code and note instead of a detail/link.
 * A running sequence in the same area requests reuse rather than a work clear. */
void fldInitializeSequenceWithNote(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name,
                    s32 code, const char *subname) {
    if (kwlnTaskGetTaskByName(D_00412D50) != NULL) {
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

u32 fldGetSceneStatusCode(void) {
    FieldPlayerSceneWork *sceneWork = &D_0038A640;

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

INCLUDE_ASM(const s32, "game/code_0011F208", func_00125380);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412D50);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412D60);

INCLUDE_ASM(const s32, "game/code_0011F208", func_001258B8);

void fldUnloadPlayerModel(void) {
    if (fldPlayerModelResource != 0) {
        sdfQueueNonzeroResourceId(fldPlayerModelResource);
        fldPlayerModelResource = 0;
        D_003898B8[0] = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_00125B10);

void fldPrepareResourceBuffer(void) {
    void *source;
    void *buffer;
    func_001258B8();
    D_00435F40 = D_00435F44;
    D_00435F38 = sdfAllocGeneralBlock(D_00435F44);
    source = sdfMemoryGetBlockAddress(fldPlayerModelResource);
    buffer = sdfMemoryGetBlockAddress(D_00435F38);
    memcpy(buffer, source, D_00435F40);
    D_00435F3C = (u32)buffer;
    mdlLoadViewerPackage(2, 0, 0x101, buffer, D_00435F40);
    sdfQueueNonzeroResourceId(D_00435F38);
    D_00435F38 = 0;
}

void fldReleaseResources(void) {
    if (D_00435F38 != 0) {
        sdfQueueNonzeroResourceId(D_00435F38);
        D_00435F38 = 0;
    }
    if (D_00389790[0] == 0 && D_00435F78 == 0) {
        fldUnloadPlayerModel();
        fldSetPendingAreaAndFloor(0, 0);
    }
}

extern u32 fldSecondarySceneObject;
extern u32 fldCameraModelObject;
extern u32 fldSecondarySceneModelHandle;
extern void effObjFetchInnerFirstVec(u32);
extern void effObjFetchInnerSecondVecNorm(u32);

void fldSnapshotAndReleasePlayerSceneObject(void) {
    FieldPlayerSceneWork *sceneWork;
    if (fldPlayerObject != 0) {
        if (dds3GetWorldSecondaryObject() != 0) {
            sceneWork = &D_0038A640;
            effObjFetchInnerFirstVec(fldPlayerObject);
            VU0_STORE_VF(vf10, &sceneWork->position);
            effObjFetchInnerSecondVecNorm(fldPlayerObject);
            VU0_STORE_VF(vf10, &sceneWork->rotation);
        }
        fldPlayerObject = 0;
        fldSecondarySceneObject = 0;
        fldCameraModelObject = 0;
        fldSecondarySceneModelHandle = 0;
        *fldGetPlayerSceneStateAddress() = 0;
        fldReleaseResources();
    }
}


void fldShutdownSceneTasksAndWorld(void) {
    fldResetTaskSlots();
    fldTestDrawDestroy();
    fldSnapshotAndReleasePlayerSceneObject();
    evtDestroySecondaryWorldNode();
    fldReleaseCampSceneTasks();
    fldResetTargetViewAndSound();
}

u32 * fldGetPlayerSceneStateAddress(void) {
    return &D_00435F60;
}

u32 fldGetPlayerSceneState(void) {
    return *fldGetPlayerSceneStateAddress();
}

/* Raise the scene-state minima, clear pending slots and aim ten units above
 * the fetched player position. Existing larger mode/state values are retained. */
void fldPreparePlayerSceneCameraTarget(void) {
    f32 position[4];

    if (fldPlayerObject != 0) {
        fldSetSceneControlFlags(0x40);
        dds3InvokeSlot1Handler(fldPlayerObject, func_001266D8);
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
    func_00139EC0(position);
}

void fldResetPlayerSceneObjectState(void) {
    if (fldPlayerObject != 0) {
        fldClearSceneControlFlags(0x40);
        dds3InvokeSlot1Handler(fldPlayerObject, 0);
    }
    D_00389858[0] = 4;
}

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
        dds3SetWorldEntryCallbackTarget((void *)fldPlayerObject, D_00412EC0);
        dds3SetWorldPlayerObject(dds3GetWorldSecondaryObject(), fldPlayerObject);
        if (D_00435F30 != 0) {
            dds3ClearObjectFlags(fldPlayerObject, 0x20);
        }
        fldPrepareResourceBuffer();
        func_00112058(fldPlayerObject, 2, D_0038A67C[0]);
    }
}

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412EB0);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412EC0);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412ED0);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412EE0);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412EF0);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00126110);

typedef struct FieldVec4 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} FieldVec4;

extern FieldVec4 D_00412F10;
extern FieldVec4 D_00412F20;
extern char D_00435F58[];
extern u32 dds3CreateConfiguredCameraObject(s32, FieldVec4 *, FieldVec4 *, FieldVec4 *);
extern void dds3SetCameraVector(u32, FieldVec4 *);
extern void effObjSetInnerFloat(u32, f32);
extern void dds3SetWorldCameraObject(u64, u32);

/* Create the secondary camera at target origin with the stored eye/up vectors. */
void fldCreateSecondaryWorldCamera(void) {
    FieldVec4 localUp = D_00412F10;
    FieldVec4 targetPosition;
    FieldVec4 worldEye;
    u32 *cameraObjectSlot = &D_00435F60;
    u32 cameraObject;
    memset(&targetPosition, 0, sizeof(targetPosition));
    targetPosition.w = 1.0f;
    worldEye = D_00412F20;
    cameraObject = dds3CreateConfiguredCameraObject(dds3AdvanceWorldCounter(), &targetPosition, &worldEye, &localUp);
    *cameraObjectSlot = cameraObject;
    dds3SetWorldEntryCallbackTarget((void *)cameraObject, D_00435F58);
    dds3SetCameraVector(*cameraObjectSlot, &worldEye);
    effObjSetInnerFloat(*cameraObjectSlot, 2.0f);
    dds3SetWorldCameraObject(dds3GetWorldSecondaryObject(), *cameraObjectSlot);
}

u32 func_001266D8(void) {
    return 0;
}

u32 func_001266E0(void) {
    return 100;
}

void fldConsumeSceneCommandFlag(void) {
    if ((((FldWorkFlags *)datGameState)->fieldFlags & 8) != 0) {
        fldClearSceneCommandFlag();
        fldAreaState.consumedFlags |= 1;
    }
}

extern void mdlFlagClear(s32);
extern void evtClearSolarOverlayControl(void);

void fldClearSceneCommandFlag(void) {
    ((FldWorkFlags *)datGameState)->fieldFlags &= ~8;
    fldAreaState.consumedFlags &= ~1;
    mdlFlagClear(0x818);
    evtClearSolarOverlayControl();
}


extern void fldPlayMenuSound(s32);
extern void func_001360B8(s16, s32);

void fldActivatePendingSceneCommand(void) {
    u32 value;
    if (fldAreaState.commandEnabled != 0) {
        if ((((FldWorkFlags *)datGameState)->fieldFlags & 8) == 0) {
            fldPlayMenuSound(0x29);
        }
        mdlFlagSet(0x818);
        value = func_001266E0();
        fldAreaState.sceneCommand = value;
        ((FldWorkFlags *)datGameState)->fieldFlags |= 8;
        fldAreaState.consumedFlags &= ~1;
        func_001360B8(value, 0);
    }
}


s32 fldGetSceneCommandState(void) {
    if ((((FldWorkFlags *)datGameState)->fieldFlags & 8) != 0) {
        return 1;
    }
    if (D_0038989C[0] != 0) {
        return 0;
    }
    return -1;
}


/* Latch the scene command while enabled and flagged; stop it when either gate
 * clears. Disable passes transition parameter 0; flag removal passes 20. */
void fldUpdateSceneCommandSpeed(void) {
    FldAreaWork *scene = &fldAreaState;
    u32 value;
    if (scene->commandEnabled == 0) {
        if (scene->sceneCommand != 0) {
            scene->sceneCommand = 0;
            func_001360B8(0, 0);
        }
    } else if ((((FldWorkFlags *)datGameState)->fieldFlags & 8) != 0) {
        if (scene->sceneCommand == 0) {
            value = func_001266E0();
            scene->sceneCommand = value;
            func_001360B8(value, 0x14);
        }
    } else if (scene->sceneCommand != 0) {
        scene->sceneCommand = 0;
        func_001360B8(0, 0x14);
        fldPlayMenuSound(0x2A);
        mdlFlagClear(0x818);
        evtClearSolarOverlayControl();
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
    if ((((FldWorkFlags *)datGameState)->fieldFlags & FIELD_TRANSITION_WORK_FLAG) != 0) {
        fldClearFieldTransitionFlag();
        fldAreaState.consumedFlags |= FIELD_TRANSITION_CONSUMED_FLAG;
    }
}

extern void func_00243320(void);

/* Clear the transition work flag and its area-state consumption marker. */
void fldClearFieldTransitionFlag(void) {
    ((FldWorkFlags *)datGameState)->fieldFlags &= ~FIELD_TRANSITION_WORK_FLAG;
    fldAreaState.consumedFlags &= ~FIELD_TRANSITION_CONSUMED_FLAG;
    func_00243320();
}


/* Set the transition work flag and reset its area-state consumption marker. */
void fldSetFieldTransitionFlag(void) {
    ((FldWorkFlags *)datGameState)->fieldFlags |= FIELD_TRANSITION_WORK_FLAG;
    fldAreaState.consumedFlags &= ~FIELD_TRANSITION_CONSUMED_FLAG;
}

/* Return whether the transition work flag is set. */
u8 fldTestFieldTransitionFlag(void) {
    s32 fieldFlags = ((FldWorkFlags *)datGameState)->fieldFlags;
    fieldFlags &= FIELD_TRANSITION_WORK_FLAG;
    return fieldFlags != 0;
}

/* Clear a set secondary-scene flag and mark its consumption in area state. */
void fldConsumeSecondarySceneFlag(void) {
    if ((((FldWorkFlags *)datGameState)->fieldFlags & FIELD_SECONDARY_SCENE_WORK_FLAG) != 0) {
        fldClearSecondarySceneFlag();
        fldAreaState.consumedFlags |= FIELD_SECONDARY_SCENE_CONSUMED_FLAG;
    }
}

/* Clear the secondary-scene work flag and its consumption marker. */
void fldClearSecondarySceneFlag(void) {
    ((FldWorkFlags *)datGameState)->fieldFlags &= ~FIELD_SECONDARY_SCENE_WORK_FLAG;
    fldAreaState.consumedFlags &= ~FIELD_SECONDARY_SCENE_CONSUMED_FLAG;
}

/* Select secondary rather than primary, resetting secondary's consume marker. */
void fldSetSecondarySceneFlag(void) {
    ((FldWorkFlags *)datGameState)->fieldFlags = (((FldWorkFlags *)datGameState)->fieldFlags | FIELD_SECONDARY_SCENE_WORK_FLAG) & ~FIELD_PRIMARY_SCENE_WORK_FLAG;
    fldAreaState.consumedFlags &= ~FIELD_SECONDARY_SCENE_CONSUMED_FLAG;
}

/* Return whether the secondary-scene work flag is set. */
u8 fldTestSecondarySceneFlag(void) {
    s32 fieldFlags = ((FldWorkFlags *)datGameState)->fieldFlags;
    fieldFlags &= FIELD_SECONDARY_SCENE_WORK_FLAG;
    return fieldFlags != 0;
}

/* Clear a set primary-scene flag and mark its consumption in area state. */
void fldConsumePrimarySceneFlag(void) {
    if ((((FldWorkFlags *)datGameState)->fieldFlags & FIELD_PRIMARY_SCENE_WORK_FLAG) != 0) {
        fldClearPrimarySceneFlag();
        fldAreaState.consumedFlags |= FIELD_PRIMARY_SCENE_CONSUMED_FLAG;
    }
}

/* Clear the primary-scene work flag and its area-state consumption marker. */
void fldClearPrimarySceneFlag(void) {
    ((FldWorkFlags *)datGameState)->fieldFlags &= ~FIELD_PRIMARY_SCENE_WORK_FLAG;
    fldAreaState.consumedFlags &= ~FIELD_PRIMARY_SCENE_CONSUMED_FLAG;
}

/* Select primary rather than secondary, resetting primary's consume marker. */
void fldSetPrimarySceneFlag(void) {
    ((FldWorkFlags *)datGameState)->fieldFlags = (((FldWorkFlags *)datGameState)->fieldFlags | FIELD_PRIMARY_SCENE_WORK_FLAG) & ~FIELD_SECONDARY_SCENE_WORK_FLAG;
    fldAreaState.consumedFlags &= ~FIELD_PRIMARY_SCENE_CONSUMED_FLAG;
}

/* Return whether the primary-scene work flag is set. */
u8 fldIsFlagActive(void) {
    s32 fieldFlags = ((FldWorkFlags *)datGameState)->fieldFlags;
    fieldFlags &= FIELD_PRIMARY_SCENE_WORK_FLAG;
    if (fieldFlags == 0) return 0;
    return 1;
}

void func_00126B70(void) {
    sdfStoreMessageWordsAndNotifyConsumer(D_003807F8, (s32)D_0037FF88, (s32)(D_0037FF88 + 0xC0), (s32)(D_0037FF88 + 0x100), (s32)(D_0037FF88 + 0xE0));
}

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

INCLUDE_ASM(const s32, "game/code_0011F208", func_00126C80);

/* Queue the current area and next floor, reset the scene lifecycle, and return -1.
 * The pending pair is consumed later with a 200-entry offset on the area value. */
s32 fldAdvanceToNextScene(void) {
    u32 scene;
    u32 area;

    fldResetEventSceneState();
    D_00435F98[0] = 0;
    D_00435F98[1] = func_00151498();
    scene = fldAreaState.floor;
    area = fldAreaState.area;
    fldAreaState.nextArea = area;
    fldAreaState.nextFloor = scene + 1;
    fldResetTaskSlots();
    fldSetSceneLifecycleFlags(1);
    fldSetSceneLifecycleFlags(2);
    fldResetPlayerSceneObjectState();
    func_0023ACE8();
    return -1;
}

u8 fldHasKiretaLabelProcess(void) {
    return scrFindNamedProcessNode(D_00412F30) != 0;
}

u8 fldHasHirakenaiLabelProcess(void) {
    return scrFindNamedProcessNode(D_00412F58) != 0;
}

u8 fldHasBadkaifukuLabelProcess(void) {
    return scrFindNamedProcessNode(D_00412F40) != 0;
}

u8 fldGetCampSceneControlMode(void) {
    if (D_00435F7C > 0) {
        return 2;
    }
    if (mnuAcknowledgeCampState() != 0) {
        return 1;
    }
    return fldTestSceneControlFlags(0x20) != 0 ? 0 : 3;
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_001273E8);

u8 fldGetSceneReadyOrPendingState(void) {
    if (D_00435F80 > 0) {
        return 2;
    }
    return fldGetSceneReadyFlag() != 0;
}

extern void *dds3GetWorldObject(void);
typedef struct WorldObject WorldObject;
extern void dds3SetWorldObjectDataValue(WorldObject *, s8);
extern void kwlnFadeStartIn(s32);
extern void kwlnFadeInStart(s8, s8, s8, s32);
extern void func_00149A00(void);
extern void func_00133F08(void);
extern u8 fldHasPendingSceneFlags(void);
extern void fldSetCameraNodeModeWithTen(void);
extern void func_00123B88(s32, s32, f32, f32, f32);

/* Gate next-floor input on camp/readiness flags, then start the native scene
 * transition or its delayed fade path. All return paths retain zero. */
s32 fldUpdateNextFloorTransition(void) {
    s32 transitionInput = 0;
    FldAreaWork *scene;

    if (fldGetCampSceneControlMode() != 0) {
        return 0;
    }
    if (D_00389988[0x58 / 4] & fldTestSceneControlFlags(0x40)) {
        if ((s8)D_0037F530[2] != 0) {
            transitionInput = 1;
        }
    } else if ((s8)D_0037F530[2] < 0) {
        transitionInput = 1;
    }
    scene = &fldAreaState;
    if (fldFindLocationCoordinateRecord(scene->area, scene->floor + 1)[2] <= 0) {
        if (scene->sceneMode == 0) {
            if (fldTestSceneControlFlags(0x40) != 0 && transitionInput != 0) {
                scene->sceneMode = 4;
                scene->sceneState = 5;
                fldResetPlayerSceneObjectState();
                evtStartSceneResourceTask((u64)dds3GetWorldObject(), D_00412F58);
            }
        }
    } else {
        if (D_00435F80 > 0) {
            D_00435F80--;
            if (D_00435F80 == 0) {
                dds3SetWorldObjectDataValue(dds3GetWorldObject(), 0);
                kwlnFadeStartIn(4);
                func_00149A00();
            }
            return 0;
        }
        if (fldGetSceneReadyOrPendingState() != 0) {
            return 0;
        }
        func_00123B88(scene->floor, scene->unkC0, scene->x, scene->z, 50.0f);
        if (scene->sceneMode == 0) {
            if (fldHasPendingSceneFlags() != 0) {
                return 0;
            }
            if (fldTestSceneControlFlags(0x40) != 0 && transitionInput != 0 && D_00389988[0x48 / 4] == 0) {
                scene->sceneMode = 4;
                scene->sceneState = 5;
                fldResetPlayerSceneObjectState();
                func_00133F08();
                kwlnFadeInStart(0, 0, 0, 4);
                D_00435F80 = 5;
                fldSetCameraNodeModeWithTen();
            }
        }
    }
    return 0;
}

/* Dispatch one pending field command, preferring the temporary override
 * over the scene-work buffer and its saved fallback. */
s32 fldDispatchPendingSceneResource(void) {
    FieldPlayerSceneWork *sceneWork = &D_0038A640;
    u32 overrideFlags;

    if (fldAreaState.unk100 == 1) {
        func_00150800();
    }
    fldAreaState.unk100 = 0;
    if ((D_00435F24 & 2) && *(s8 *)D_00387D60 != 0) {
        evtStartSceneResourceTask(dds3GetWorldSecondaryObject(), D_00387D60);
        overrideFlags = D_00435F24;
        if (!(overrideFlags & 1)) {
            D_00387D60[0] = 0;
            D_00435F24 = overrideFlags & 0xFB;
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

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412F10);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412F20);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412F30);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412F40);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412F58);

INCLUDE_ASM(const s32, "game/code_0011F208", func_001278D0);

INCLUDE_ASM(const s32, "game/code_0011F208", fldProcDraw);

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

INCLUDE_ASM(const s32, "game/code_0011F208", func_001283B8);

void fldReleaseCampSceneTasks(void) {
    if (D_00435F84 == 0) return;
    D_00435F84 = 0;
    kwlnFadeResetBackground();
    fldClearSceneControlFlags(0x10);
    fldClearSceneControlFlags(0x20);
    fldSetSceneLifecycleFlags(1);
    fldSetSceneLifecycleFlags(2);
    kwlnTaskDestroyWithHierarchyByName(D_00412FD0, 1);
    kwlnTaskDestroyWithHierarchyByName(D_00412FE0, 1);
    fldDestroyPanelTaskIfPresent();
    evtSetSolarOverlayFullyTransparent();
    fldDestroyTask();
    if (D_00435F74 != 0) {
        effBlurReleaseFirstResource(D_00435F74);
        D_00435F74 = 0;
    }
    mnuDestroyCampTasks();
    scrDestroyAllNamedProcesses();
    fldReleaseMenuSlotsAfterWait();
}

u8 fldIsFieldResourceWaitFinished(void) {
    if (D_00389790[0] == 0 && fldIsAreaResourceReady() != 0) {
        fldPollAreaResourceLoad();
        if (fldGetResourceReadyFlag() == 1) return 0;
        fldFreeDisplayObjects();
        return 0;
    }
    return sdfCheckPendingWorkWithInterrupts() == 0;
}

void func_001285E8(void) {
    fldSelectActorFromSceneIndexTables();
}

extern void fldDispatchDeferredFieldCommand(void);

void fldStopSceneBgm(void) {
    fldStopCurrentBgm();
}

void fldProcessDeferredSceneCommand(void) {
    if (fldDeferredCommand != 0) {
        fldDispatchDeferredFieldCommand();
        return;
    }
    func_00141898();
}

void fldSetDeferredFieldCommand(u32 command, u32 parameter) {
    fldDeferredCommand = command;
    fldDeferredCommandParameter = parameter;
}

extern s8 dds3AdminGetRequestedMode(void);
extern s32 dds3AdminReadPreviousUnsignedSample(void);
extern s32 func_00141CF0(s32, u32);

void fldDispatchDeferredFieldCommand(void) {
    if (fldDeferredCommand == 0) {
        return;
    }
    if (dds3AdminGetRequestedMode() > 0) {
        return;
    }
    if (dds3AdminGetRequestedMode() < 0 && (dds3AdminReadPreviousUnsignedSample() & 1) != 0) {
        return;
    }
    if (func_00141CF0(fldDeferredCommand, fldDeferredCommandParameter) == 0) {
        fldDeferredCommand = 0;
    }
}

void fldSetPendingSceneAction(u32 argument) {
    D_00435F70 = argument;
}

void fldRunPendingSceneAction(void) {
    u32 argument;

    argument = D_00435F70;
    if (argument != 0) {
        func_001411F8(argument);
        D_00435F70 = 0;
    }
}

/* Consume the pending pair of field-script values. -1 in both slots means
 * no request; the first value is returned with its 200-entry base offset. */
s32 fldConsumeNextSceneRequest(s32 *outCode, s32 *outParameter) {
    if (fldAreaState.nextArea == -1 && fldAreaState.nextFloor == fldAreaState.nextArea) {
        return 0;
    }
    *outCode = fldAreaState.nextArea + 0xC8;
    *outParameter = fldAreaState.nextFloor;
    fldAreaState.nextArea = -1;
    fldAreaState.nextFloor = -1;
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412FD0);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412FE0);

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

