#include "common.h"
#include "pcp_vu0.h"
#include "fpu.h"

#include "fld.h"

extern s32 fldDeferredCommand;

extern u64 dds3GetWorldSecondaryObject(void);

extern s64 dds3GetWorldValueCount(u64);

extern u64 dds3ReadIndexedWorldObjectWord(u64);

extern s64 dds3AdvanceObjectValueCursor(u64);

extern u64 dds3CopyWorldListToValueChain(u64, u64);

extern s64 evtGetObjectTransitionWork(u64);

extern u64 sdfAllocPacketAligned(u64);

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

extern void *func_00101740(const char *);

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

extern void fldPollAreaResourceLoad(void);

extern s32 fldGetResourceReadyFlag(void);

extern void fldFreeDisplayObjects(void);

extern s32 sdfCheckPendingWorkWithInterrupts(void);

extern void *dds3AdvanceWorldCounter(void);

extern void dds3SetWorldEntryCallbackTarget(void *, const char *);

extern f32 func_001248E8(f32, f32, f32, f32);

extern void fldStepValueByPad(f32 *value, u8 *padState, f32 min, f32 max, f32 step, f32 bigStep);

extern u8 D_0037F530[];

extern void fldStepIntByPad(void *, s32, s64, s64, s64, s64, s8 *);

extern s32 fldStepColorChannelByPad(u32 *, s32, s8 *);

extern s32 fldTestDrawUpdate(void);

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

extern s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

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

extern void dds3SetWorldPlayerObject(u64, u32);

extern void func_00112058(u32, s32, s32);

extern u32 dds3SpawnCameraSlotObj5(void *, f32 *, f32 *);

extern s32 D_0038A67C[];

extern char D_00412EC0[]; /* "PLAYER_UNIT" */

extern s32 D_00435F30;

extern u8 D_0037FF88[];

extern u8 D_003807F8[];

extern void sdfStoreMessageWordsAndNotifyConsumer(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

extern s32 fldAreaState[];

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

void fldSetPacketArgumentPair(u32 *state, u32 firstValue, u32 secondValue) {
    state[4] = firstValue;
    state[5] = secondValue;
}

u64 sdfCreateResetPacketList(void) {
    u64 packet;

    packet = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(packet);
    return packet;
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_0011F250);

INCLUDE_ASM(const s32, "game/code_0011F208", func_0011F3D8);

INCLUDE_ASM(const s32, "game/code_0011F208", func_0011F518);

/* Press edge (0x80) wraps at the bounds; held input (0x02) clamps there. */
void fldStepValueByPad(f32 *value, u8 *padState, f32 min, f32 max, f32 step, f32 bigStep) {
    f32 v = *value;

    if ((s8)padState[5] & 0x80) {
        v += bigStep;
        if (max < v) {
            v = min;
        }
    } else if (padState[5] & 2) {
        v += bigStep;
        if (max < v) {
            v = max;
        }
    } else if ((s8)padState[4] & 0x80) {
        v -= bigStep;
        if (v < min) {
            v = max;
        }
    } else if (padState[4] & 2) {
        v -= bigStep;
        if (v < min) {
            v = min;
        }
    } else if ((s8)padState[7] & 0x80) {
        v += step;
        if (max < v) {
            v = min;
        }
    } else if (padState[7] & 2) {
        v += step;
        if (max < v) {
            v = max;
        }
    } else if ((s8)padState[6] & 0x80) {
        v -= step;
        if (v < min) {
            v = max;
        }
    } else if (padState[6] & 2) {
        v -= step;
        if (v < min) {
            v = min;
        }
    } else {
        return;
    }
    *value = v;
}

void fldStepValueByCurrentPad(f32 *value, f32 min, f32 max, f32 step, f32 bigStep) {
    fldStepValueByPad(value, D_0037F530, min, max, step, bigStep);
}

void fldStepIntByPad(void *ptr, s32 type, s64 min, s64 max, s64 small, s64 big, s8 *pad) {
    s64 value;
    switch (type) {
    case 1:
        value = *(u8 *)ptr;
        break;
    case 2:
        value = *(u16 *)ptr;
        break;
    case 4:
        value = *(u32 *)ptr;
        break;
    case -1:
        value = *(s8 *)ptr;
        break;
    case -2:
        value = *(s16 *)ptr;
        break;
    case -4:
        value = *(s32 *)ptr;
        break;
    default:
        return;
    }
    if (pad[5] & 0x80) {
        value += big;
        if (max < value) {
            value = min;
        }
    } else if (pad[5] & 2) {
        value += big;
        if (max < value) {
            value = max;
        }
    } else if (pad[4] & 0x80) {
        value -= big;
        if (value < min) {
            value = max;
        }
    } else if (pad[4] & 2) {
        value -= big;
        if (value < min) {
            value = min;
        }
    } else if (pad[7] & 0x80) {
        value += small;
        if (max < value) {
            value = min;
        }
    } else if (pad[7] & 2) {
        value += small;
        if (max < value) {
            value = max;
        }
    } else if (pad[6] & 0x80) {
        value -= small;
        if (value < min) {
            value = max;
        }
    } else if (pad[6] & 2) {
        value -= small;
        if (value < min) {
            value = min;
        }
    } else {
        return;
    }
    switch (type) {
    case -1:
    case 1:
        *(u8 *)ptr = value;
        break;
    case -2:
    case 2:
        *(u16 *)ptr = value;
        break;
    case -4:
    case 4:
        *(u32 *)ptr = value;
        break;
    }
}


void fldAdjustIntegerUsingMainPad(void *ptr, s32 type, s64 min, s64 max, s64 step, s64 bigStep) {
    fldStepIntByPad(ptr, type, min, max, step, bigStep, (s8 *)D_0037F530);
}

s32 fldStepColorChannelByPad(u32 *color, s32 channel, s8 *pad) {
    s32 old = *color;
    s32 byte = old;
    s32 value;
    switch (channel) {
    case 0:
        break;
    case 1:
        byte = old >> 8;
        break;
    case 2:
        byte = old >> 16;
        break;
    case 3:
        byte = old >> 24;
        break;
    }
    byte &= 0xFF;
    if (((pad[5] & 0x80) || (pad[7] & 0x80)) && byte == 0xFF) {
        byte = 0;
    } else if (pad[5] & 2) {
        byte += 10;
        if (byte >= 0x100) {
            byte = 0xFF;
        }
    } else if (((pad[4] & 0x80) || (pad[6] & 0x80)) && byte == 0) {
        byte = 0xFF;
    } else if (pad[4] & 2) {
        byte -= 10;
        if (byte < 0) {
            byte = 0;
        }
    } else if (pad[7] & 2) {
        byte += 1;
        if (byte >= 0x100) {
            byte = 0xFF;
        }
    } else if (pad[6] & 2) {
        byte -= 1;
        if (byte < 0) {
            byte = 0;
        }
    } else {
        return 0;
    }
    switch (channel) {
    case 0:
        value = (old & 0xFFFFFF00) | byte;
        break;
    case 1:
        value = (old & 0xFFFF00FF) | (byte << 8);
        break;
    case 2:
        value = (old & 0xFF00FFFF) | (byte << 16);
        break;
    default:
        value = (old & 0x00FFFFFF) | (byte << 24);
        break;
    }
    *color = value;
    return value != old;
}


void fldStepColorChannelByCurrentPad(u32 *color, s32 channel) {
    fldStepColorChannelByPad(color, channel, (s8 *)D_0037F530);
}

void fldFormatSecondsText(f32 value, char *out) {
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
    out[0] = whole + '0';
    out[1] = '.';
    out[2] = tenths + '0';
    out[3] = hundredths + '0';
    out[4] = 0;
}


INCLUDE_ASM(const s32, "game/code_0011F208", func_0011FD18);

INCLUDE_ASM(const s32, "game/code_0011F208", func_0011FEE8);

u32 func_001200E0(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_001200E8);

s32 fldTestDrawUpdate(void) {
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
    kwlnTaskCreate((s32)D_00412B90, 0x2AF8, 0, 0, (s32)fldTestDrawUpdate, 0, 0);
}

void fldTestDrawDestroy(void) {
    kwlnTaskDestroyWithHierarchyByName(D_00412B90, 1);
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_00120528);

INCLUDE_ASM(const s32, "game/code_0011F208", func_001206D0);

void fldResetCameraMoveTracking(void) {
    D_00435EF8 = 0;
    D_00435EE0 = -999;
    D_00435EE4 = -999;
    D_00435EE8 = -999;
    D_00435EEC = -999;
    D_00435EF0 = -999;
    D_00435EF4 = -999;
    fldClearCameraMoveMode();
}

/* Report the pending camera move mode, kicking the field camera when the
 * stored start and end indices agree. */
s32 fldEvaluateCameraMoveTracking(void) {
    if (D_00435EF8 == 1) {
        return 2;
    }
    if (D_00435EF4 != -999 && D_00435EE0 == D_00435EEC) {
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

void fldSubmitVisibleWorldBackground(void) {
    s32 found = 0;
    u64 list;
    u64 item;

    list = dds3CopyWorldListToValueChain(dds3GetWorldSecondaryObject(), 5);
    if (list != 0) {
        if (dds3ResetObjectValueCursor(list) != 0) {
            do {
                item = dds3ReadIndexedWorldObjectWord(list);
                if (dds3TestObjectFlags(item, 0x200) != 0) {
                    if (dds3TestObjectFlags(item, 1) == 0) {
                        found = 1;
                    }
                }
            } while (dds3AdvanceObjectValueCursor(list) != 0);
        }
        dds3DestroyWorldIndexNode(list);
    }
    if (D_00389780[0] == 1) {
        found = 0;
    }
    if (found != 0) {
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
void func_00122828(void) {
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

#define FIELD_MAP_SLOT_OFFSET 0x1450

void fldSetRoomModeFlag(s32 map, s32 slot, s32 bit, s32 enabled) {
    if (map < 40) {
        s32 index = map % 100;
        if (enabled != 0) {
            s32 byteOffset = slot * 30 + index * 1920 + FIELD_MAP_SLOT_OFFSET;
            u16 *flags = &((FieldMapSlot *)(datGameState + byteOffset))->flagBanks[0];
            *flags |= 1 << bit;
        } else {
            s32 byteOffset = slot * 30 + index * 1920 + FIELD_MAP_SLOT_OFFSET;
            u16 *flags = &((FieldMapSlot *)(datGameState + byteOffset))->flagBanks[0];
            *flags &= ~(1 << bit);
        }
    }
}

u8 fldTestRoomModeFlag(s32 map, u32 slot, u32 bit) {
    s32 mapIndex;
    u8 *slotBase;
    u32 flags;

    if (map < 0x28) {
        mapIndex = map % 100;
        slotBase = (u8 *)(slot * 30 + mapIndex * 1920);
        slotBase += datGameState;
        flags = ((FieldMapSlot *)(slotBase + FIELD_MAP_SLOT_OFFSET))->flagBanks[0];
        return (flags >> bit) & 1;
    }
    return 0;
}

void fldSetRoomObjectModeFlag(s32 map, s32 slot, s32 bit, s32 enabled) {
    if (map < 40) {
        s32 index = map % 100;
        if (enabled != 0) {
            u16 *flags = &((FieldMapSlot *)(slot * 30 + index * 1920 + datGameState + FIELD_MAP_SLOT_OFFSET))->flagBanks[1];
            *flags |= 1 << bit;
        } else {
            u16 *flags = &((FieldMapSlot *)(slot * 30 + index * 1920 + datGameState + FIELD_MAP_SLOT_OFFSET))->flagBanks[1];
            *flags &= ~(1 << bit);
        }
    }
}

u8 fldTestRoomObjectModeFlag(s32 map, u32 slot, u32 bit) {
    s32 mapIndex;
    u8 *slotBase;
    u32 flags;

    if (map < 0x28) {
        mapIndex = map % 100;
        slotBase = (u8 *)(slot * 30 + mapIndex * 1920);
        slotBase += datGameState;
        flags = ((FieldMapSlot *)(slotBase + FIELD_MAP_SLOT_OFFSET))->flagBanks[1];
        return (flags >> bit) & 1;
    }
    return 0;
}

void fldSetRoomSceneFlag(s32 map, s32 slot, s32 bit, s32 enabled) {
    if (map < 40) {
        s32 index = map % 100;
        if (enabled != 0) {
            u16 *flags = &((FieldMapSlot *)(slot * 30 + index * 1920 + datGameState + FIELD_MAP_SLOT_OFFSET))->flagBanks[2];
            *flags |= 1 << bit;
        } else {
            u16 *flags = &((FieldMapSlot *)(slot * 30 + index * 1920 + datGameState + FIELD_MAP_SLOT_OFFSET))->flagBanks[2];
            *flags &= ~(1 << bit);
        }
    }
}

u8 fldTestRoomSceneFlag(s32 map, u32 slot, u32 bit) {
    s32 mapIndex;
    u8 *slotBase;
    u32 flags;

    if (map < 0x28) {
        mapIndex = map % 100;
        slotBase = (u8 *)(slot * 30 + mapIndex * 1920);
        slotBase += datGameState;
        flags = ((FieldMapSlot *)(slotBase + FIELD_MAP_SLOT_OFFSET))->flagBanks[2];
        return (flags >> bit) & 1;
    }
    return 0;
}

void fldSetMapTargetFlag(s32 map, s32 slot, s32 bit, s32 enabled) {
    if (map < 40) {
        s32 index = map % 100;
        if (enabled != 0) {
            u16 *flags = &((FieldMapSlot *)(slot * 30 + index * 1920 + datGameState + FIELD_MAP_SLOT_OFFSET))->flagBanks[3];
            *flags |= 1 << bit;
        } else {
            u16 *flags = &((FieldMapSlot *)(slot * 30 + index * 1920 + datGameState + FIELD_MAP_SLOT_OFFSET))->flagBanks[3];
            *flags &= ~(1 << bit);
        }
    }
}

u8 fldTestMapTargetFlag(s32 map, u32 slot, u32 bit) {
    s32 mapIndex;
    u8 *slotBase;
    u32 flags;

    if (map < 0x28) {
        mapIndex = map % 100;
        slotBase = (u8 *)(slot * 30 + mapIndex * 1920);
        slotBase += datGameState;
        flags = ((FieldMapSlot *)(slotBase + FIELD_MAP_SLOT_OFFSET))->flagBanks[3];
        return (flags >> bit) & 1;
    }
    return 0;
}

void fldSetAlternateMapTargetFlag(s32 map, s32 slot, s32 bit, s32 enabled) {
    if (map < 40) {
        s32 index = map % 100;
        if (enabled != 0) {
            u16 *flags = &((FieldMapSlot *)(slot * 30 + index * 1920 + datGameState + FIELD_MAP_SLOT_OFFSET))->flagBanks[4];
            *flags |= 1 << bit;
        } else {
            u16 *flags = &((FieldMapSlot *)(slot * 30 + index * 1920 + datGameState + FIELD_MAP_SLOT_OFFSET))->flagBanks[4];
            *flags &= ~(1 << bit);
        }
    }
}

u8 fldTestAlternateMapTargetFlag(s32 map, u32 slot, u32 bit) {
    s32 mapIndex;
    u8 *slotBase;
    u32 flags;

    if (map < 0x28) {
        mapIndex = map % 100;
        slotBase = (u8 *)(slot * 30 + mapIndex * 1920);
        slotBase += datGameState;
        flags = ((FieldMapSlot *)(slotBase + FIELD_MAP_SLOT_OFFSET))->flagBanks[4];
        return (flags >> bit) & 1;
    }
    return 0;
}

void fldSetMapSlotByte(s32 map, u32 slot, s32 offset, s32 value) {
    if (map < 40) {
        s32 index = map % 100;
        s32 displacement = offset + index * 1920;
        u8 *entry = (u8 *)(slot * 30 + displacement);
        entry += datGameState;
        ((FieldMapSlot *)(entry + FIELD_MAP_SLOT_OFFSET))->values[0] = value;
    }
}

s32 fldGetMapSlotByte(s32 map, u32 slot, s32 offset) {
    u8 value = 0;
    if (map < 40) {
        s32 index = map % 100;
        s32 displacement = offset + index * 1920;
        u8 *entry = (u8 *)(slot * 30 + displacement);
        entry += datGameState;
        value = ((FieldMapSlot *)(entry + FIELD_MAP_SLOT_OFFSET))->values[0];
    }
    return value == 0xff ? -1 : value;
}

void fldSetMapSlotAuxiliaryFlag(s32 map, s32 slot, s32 bit, s32 enabled) {
    if (map < 40) {
        s32 index = map % 100;
        if (enabled != 0) {
            u16 *flags = &((FieldMapSlot *)(slot * 30 + index * 1920 + datGameState + FIELD_MAP_SLOT_OFFSET))->trailingFlagBanks[0];
            *flags |= 1 << bit;
        } else {
            u16 *flags = &((FieldMapSlot *)(slot * 30 + index * 1920 + datGameState + FIELD_MAP_SLOT_OFFSET))->trailingFlagBanks[0];
            *flags &= ~(1 << bit);
        }
    }
}

u8 fldTestMapSlotAuxiliaryFlag(s32 map, u32 slot, u32 bit) {
    s32 mapIndex;
    u8 *slotBase;
    u32 flags;

    if (map < 0x28) {
        mapIndex = map % 100;
        slotBase = (u8 *)(slot * 30 + mapIndex * 1920);
        slotBase += datGameState;
        flags = ((FieldMapSlot *)(slotBase + FIELD_MAP_SLOT_OFFSET))->trailingFlagBanks[0];
        return (flags >> bit) & 1;
    }
    return 0;
}

void fldSetMapSlotValueFlag(s32 map, s32 slot, s32 bit, s32 enabled) {
    if (map < 40) {
        s32 index = map % 100;
        if (enabled != 0) {
            u16 *flags = &((FieldMapSlot *)(slot * 30 + index * 1920 + datGameState + FIELD_MAP_SLOT_OFFSET))->trailingFlagBanks[1];
            *flags |= 1 << bit;
        } else {
            u16 *flags = &((FieldMapSlot *)(slot * 30 + index * 1920 + datGameState + FIELD_MAP_SLOT_OFFSET))->trailingFlagBanks[1];
            *flags &= ~(1 << bit);
        }
    }
}

u8 fldTestMapSlotValueFlag(s32 map, u32 slot, u32 bit) {
    s32 mapIndex;
    u8 *slotBase;
    u32 flags;

    if (map < 0x28) {
        mapIndex = map % 100;
        slotBase = (u8 *)(slot * 30 + mapIndex * 1920);
        slotBase += datGameState;
        flags = ((FieldMapSlot *)(slotBase + FIELD_MAP_SLOT_OFFSET))->trailingFlagBanks[1];
        return (flags >> bit) & 1;
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


u8 fldTestObjectActivationFlag(u32 arg0) {
    return (*(u8 *)(((s32)arg0 >> 3) + datGameState + 0x110d0) >> (arg0 & 7)) & 1;
}

s32 func_001237B0(s32 a, s32 b) {
    s32 i;
    for (i = 1; i < 0x200; i++) {
        if (a == *(s16 *)(D_0039A5A8 + i * 0x1E) && b == *(s16 *)(D_0039A5A8 + i * 0x1E + 2)) {
            return i;
        }
    }
    return 0;
}

s32 func_00123808(s32 a, s32 b) {
    s32 i;
    for (i = 1; i < 0x200; i++) {
        if (a == *(s16 *)(D_0039E1A8 + i * 0x22) && b == *(s16 *)(D_0039E1A8 + i * 0x22 + 2)) {
            return i;
        }
    }
    return 0;
}

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
    } while (index < 256);
    return 0;
}

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
    } while (index < 640);
    return D_00387D70;
}

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
    } while (index < 640);
    return 0;
}

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
    } while (visited < 0x280);
    return index;
}

u32 fldFindStageCoordinateRowOffset(s32 x, s32 y) {
    FieldStageCoordinate *table = (FieldStageCoordinate *)D_00389170;
    s32 sum = 0;
    s32 i = 0;
    do {
        if (table[i].x == x) {
            if (table[i].y == y) {
                return D_003899F0[x] + sum;
            }
            sum += table[i].rows;
        }
        i++;
    } while (i < 0x280);
    return 0;
}


s16 *fldFindStageCoordinateRecord(s32 x, s32 y) {
    FieldStageCoordinate *record = (FieldStageCoordinate *)D_00389170;
    s32 index = 0;

    do {
        if (record->x == x && record->y == y) {
            return (s16 *)record;
        }
        index++;
        record++;
    } while (index < 0x60);
    return NULL;
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_00123A58);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00123B88);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00123DE8);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00123EE0);

f32 fldAngleDifference(f32 a, f32 b) {
    f32 diff;

    if (a < 0.0f || b < 0.0f) {
        a += 360.0f;
        b += 360.0f;
    }
    a = (s32)a % 360;
    b = (s32)b % 360;
    diff = a - b;
    if (diff > 180.0f || diff < -180.0f) {
        if (a < b) {
            a += 360.0f;
        } else {
            b += 360.0f;
        }
    }
    return b - a;
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

void fldToggleWorldNodeState(s64 mode) {
    u64 iterator;
    s64 hasEntry;
    u64 node;

    iterator = dds3GetWorldSecondaryObject();
    iterator = dds3CopyWorldListToValueChain(iterator, 6);
    hasEntry = dds3GetWorldValueCount(iterator);
    if (hasEntry == 0) {
        return;
    }
    dds3ResetObjectValueCursor(iterator);
    do {
        node = dds3ReadIndexedWorldObjectWord(iterator);
        hasEntry = evtGetObjectTransitionWork(node);
        if (hasEntry == 4) {
            if (mode == 0) {
                evtSetObjectTransitionWork(node, 3);
            }
            else {
                evtSetObjectTransitionWork(node, 0);
            }
        }
        hasEntry = dds3AdvanceObjectValueCursor(iterator);
    } while (hasEntry != 0);
    dds3DestroyWorldIndexNode(iterator);
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
extern void func_00122828();
extern u8 D_0038A6E0[];

void fldInitializeDisplayAndSceneSound(void) {
    fldInitDisplayObjects();
    fldResetPlayerSceneTransformState();
    fldResetPendingSounds();
    effMiscSeedRandom(D_0038A6E0, 0x1E240);
    fldParseMixLb();
    func_001343E8();
    func_00145818();
    func_00122828();
}

void fldResetPlayerSceneTransformState(void) {
    FieldPlayerSceneWork *sceneWork = &D_0038A640;
    VU0_STORE_VF(vf0, &sceneWork->position);
    VU0_STORE_VF(vf0, &sceneWork->rotation);
    fldPlayerObject = 0;
    *fldGetPlayerSceneStateAddress() = 0;
}

void fldInitializeSequenceAndResetFlags(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name) {
    if (func_00101740(D_00412D50) != NULL) {
        if (fldAreaState[4] == stage) {
            fldAreaState[8] = 1;
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

void fldInitializeAlternateSequence(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name) {
    if (func_00101740(D_00412D50) != NULL) {
        if (fldAreaState[4] == stage) {
            fldAreaState[8] = 1;
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

void fldInitializeFieldSequenceRecord(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name,
                   s32 code, s32 link, const char *subname) {
    if (func_00101740(D_00412D50) != NULL) {
        if (fldAreaState[4] == stage) {
            fldAreaState[8] = 1;
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


void fldInitializeSequenceWithNote(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name,
                    s32 code, const char *subname) {
    if (func_00101740(D_00412D50) != NULL) {
        if (fldAreaState[4] == stage) {
            fldAreaState[8] = 1;
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

void func_00125F58(void) {
    f32 position[4];

    if (fldPlayerObject != 0) {
        fldSetSceneControlFlags(0x40);
        dds3InvokeSlot1Handler(fldPlayerObject, func_001266D8);
    }
    fldAreaState[0x3A] = 4;
    if (fldAreaState[0x1C] < 4) {
        fldAreaState[0x1C] = 4;
    }
    if (fldAreaState[0x1D] < 5) {
        fldAreaState[0x1D] = 5;
    }
    fldAreaState[0x24] = -1;
    fldAreaState[0x25] = -1;
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

void fldCreatePlayerObject(void) {
    f32 pos[4];
    f32 rot[4];

    memset(pos, 0, sizeof(pos));
    pos[3] = 1.0f;
    memset(rot, 0, sizeof(rot));
    rot[3] = 1.0f;
    if (fldPlayerObject == 0) {
        fldPlayerObject = dds3SpawnCameraSlotObj5(dds3AdvanceWorldCounter(), pos, rot);
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
extern u32 dds3CreateConfiguredCameraObject(void *, FieldVec4 *, FieldVec4 *, FieldVec4 *);
extern void dds3SetCameraVector(u32, FieldVec4 *);
extern void effObjSetInnerFloat(u32, f32);
extern void dds3SetWorldCameraObject(u64, u32);

void fldCreateSecondaryWorldCamera(void) {
    FieldVec4 a = D_00412F10;
    FieldVec4 b;
    FieldVec4 c;
    u32 *world = &D_00435F60;
    u32 object;
    memset(&b, 0, sizeof(b));
    b.w = 1.0f;
    c = D_00412F20;
    object = dds3CreateConfiguredCameraObject(dds3AdvanceWorldCounter(), &b, &c, &a);
    *world = object;
    dds3SetWorldEntryCallbackTarget((void *)object, D_00435F58);
    dds3SetCameraVector(*world, &c);
    effObjSetInnerFloat(*world, 2.0f);
    dds3SetWorldCameraObject(dds3GetWorldSecondaryObject(), *world);
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
        fldAreaState[3] |= 1;
    }
}

extern void mdlFlagClear(s32);
extern void evtClearSolarOverlayControl(void);

void fldClearSceneCommandFlag(void) {
    ((FldWorkFlags *)datGameState)->fieldFlags &= ~8;
    fldAreaState[3] &= ~1;
    mdlFlagClear(0x818);
    evtClearSolarOverlayControl();
}


extern void fldPlayMenuSound(s32);
extern void func_001360B8(s16, s32);

void fldActivatePendingSceneCommand(void) {
    u32 value;
    if (fldAreaState[0x4B] != 0) {
        if ((((FldWorkFlags *)datGameState)->fieldFlags & 8) == 0) {
            fldPlayMenuSound(0x29);
        }
        mdlFlagSet(0x818);
        value = func_001266E0();
        ((s16 *)fldAreaState)[0x94] = value;
        ((FldWorkFlags *)datGameState)->fieldFlags |= 8;
        fldAreaState[3] &= ~1;
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

typedef struct FldSceneState {
    u8 pad00[0xC];
    s32 flags;               /* 0x0C */
    s32 area;
    s32 floor;
    u8 pad18[0x58];
    s32 sceneMode;
    s32 sceneState;
    u8 pad78[0x48];
    s32 unkC0;
    u8 padC4[0x64];
    s16 speed;               /* 0x128 */
    u8 pad12A[2];
    s32 commandActive;       /* 0x12C */
    u8 pad130[0x1C];
    FieldVec4 position;
} FldSceneState;

void fldUpdateSceneCommandSpeed(void) {
    FldSceneState *scene = (FldSceneState *)fldAreaState;
    u32 value;
    if (scene->commandActive == 0) {
        if (scene->speed != 0) {
            scene->speed = 0;
            func_001360B8(0, 0);
        }
    } else if ((((FldWorkFlags *)datGameState)->fieldFlags & 8) != 0) {
        if (scene->speed == 0) {
            value = func_001266E0();
            scene->speed = value;
            func_001360B8(value, 0x14);
        }
    } else if (scene->speed != 0) {
        scene->speed = 0;
        func_001360B8(0, 0x14);
        fldPlayMenuSound(0x2A);
        mdlFlagClear(0x818);
        evtClearSolarOverlayControl();
    }
}


void fldConsumeFieldTransitionFlag(void) {
    if ((((FldWorkFlags *)datGameState)->fieldFlags & 4) != 0) {
        fldClearFieldTransitionFlag();
        fldAreaState[3] |= 2;
    }
}

extern void func_00243320(void);

void fldClearFieldTransitionFlag(void) {
    ((FldWorkFlags *)datGameState)->fieldFlags &= ~4;
    fldAreaState[3] &= ~2;
    func_00243320();
}


void fldSetFieldTransitionFlag(void) {
    ((FldWorkFlags *)datGameState)->fieldFlags |= 4;
    fldAreaState[3] &= ~2;
}

u8 fldTestFieldTransitionFlag(void) {
    s32 flags = ((FldWorkFlags *)datGameState)->fieldFlags;
    flags &= 4;
    return flags != 0;
}

void fldConsumeSecondarySceneFlag(void) {
    if ((((FldWorkFlags *)datGameState)->fieldFlags & 2) != 0) {
        fldClearSecondarySceneFlag();
        fldAreaState[3] |= 4;
    }
}

void fldClearSecondarySceneFlag(void) {
    ((FldWorkFlags *)datGameState)->fieldFlags &= ~2;
    fldAreaState[3] &= ~4;
}

void fldSetSecondarySceneFlag(void) {
    ((FldWorkFlags *)datGameState)->fieldFlags = (((FldWorkFlags *)datGameState)->fieldFlags | 2) & ~1;
    fldAreaState[3] &= ~4;
}

u8 fldTestSecondarySceneFlag(void) {
    s32 flags = ((FldWorkFlags *)datGameState)->fieldFlags;
    flags &= 2;
    return flags != 0;
}

void fldConsumePrimarySceneFlag(void) {
    if ((((FldWorkFlags *)datGameState)->fieldFlags & 1) != 0) {
        fldClearPrimarySceneFlag();
        fldAreaState[3] |= 8;
    }
}

void fldClearPrimarySceneFlag(void) {
    ((FldWorkFlags *)datGameState)->fieldFlags &= ~1;
    fldAreaState[3] &= ~8;
}

void fldSetPrimarySceneFlag(void) {
    ((FldWorkFlags *)datGameState)->fieldFlags = (((FldWorkFlags *)datGameState)->fieldFlags | 1) & ~2;
    fldAreaState[3] &= ~8;
}

u8 fldIsFlagActive(void) {
    s32 flags = ((FldWorkFlags *)datGameState)->fieldFlags;
    flags &= 1;
    if (flags == 0) return 0;
    return 1;
}

void func_00126B70(void) {
    sdfStoreMessageWordsAndNotifyConsumer(D_003807F8, (s32)D_0037FF88, (s32)(D_0037FF88 + 0xC0), (s32)(D_0037FF88 + 0x100), (s32)(D_0037FF88 + 0xE0));
}

s16 fldRollEncounter(void) {
    s32 i;
    s32 enabled;

    for (i = 0; i < 16; i++) {
        if (fldEncounterRollTable[i].stage == fldAreaState[4]) {
            enabled = 1;
            if (fldEncounterRollTable[i].flag != -1) {
                enabled = mdlFlagTest(fldEncounterRollTable[i].flag) != 0;
            }
            if (enabled != 0) {
                if ((u32)effMiscRand(0) % 100U < (u32)fldEncounterRollTable[i].chance) {
                    return fldEncounterRollTable[i].result;
                }
            }
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_00126C80);

s32 fldAdvanceToNextScene(void) {
    u32 scene;
    u32 area;

    fldResetEventSceneState();
    D_00435F98[0] = 0;
    D_00435F98[1] = func_00151498();
    scene = fldAreaState[5];
    area = fldAreaState[4];
    fldAreaState[0x3C] = area;
    fldAreaState[0x3D] = scene + 1;
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
extern s32 fldHasPendingSceneFlags(void);
extern void fldSetCameraNodeModeWithTen(void);
extern void func_00123B88(s32, s32, f32, f32, f32);

s32 func_00127600(void) {
    s32 pressed = 0;
    FldSceneState *state;

    if (fldGetCampSceneControlMode() != 0) {
        return 0;
    }
    if (D_00389988[0x58 / 4] & fldTestSceneControlFlags(0x40)) {
        if ((s8)D_0037F530[2] != 0) {
            pressed = 1;
        }
    } else if ((s8)D_0037F530[2] < 0) {
        pressed = 1;
    }
    state = (FldSceneState *)fldAreaState;
    if (fldFindLocationCoordinateRecord(state->area, state->floor + 1)[2] <= 0) {
        if (state->sceneMode == 0) {
            if (fldTestSceneControlFlags(0x40) != 0 && pressed != 0) {
                state->sceneMode = 4;
                state->sceneState = 5;
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
        func_00123B88(state->floor, state->unkC0, state->position.x, state->position.z, 50.0f);
        if (state->sceneMode == 0) {
            if (fldHasPendingSceneFlags() != 0) {
                return 0;
            }
            if (fldTestSceneControlFlags(0x40) != 0 && pressed != 0 && D_00389988[0x48 / 4] == 0) {
                state->sceneMode = 4;
                state->sceneState = 5;
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

s32 fldDispatchPendingSceneResource(void) {
    FieldPlayerSceneWork *sceneWork = &D_0038A640;
    u32 flags;

    if (fldAreaState[64] == 1) {
        func_00150800();
    }
    fldAreaState[64] = 0;
    if ((D_00435F24 & 2) && *(s8 *)D_00387D60 != 0) {
        evtStartSceneResourceTask(dds3GetWorldSecondaryObject(), D_00387D60);
        flags = D_00435F24;
        if (!(flags & 1)) {
            D_00387D60[0] = 0;
            D_00435F24 = flags & 0xFB;
        }
        return 1;
    }
    if (sceneWork->primaryState == 0 && sceneWork->resourceName[0] != 0) {
        evtStartSceneResourceTask(dds3GetWorldSecondaryObject(), sceneWork->resourceName);
        return 1;
    }
    if (fldAreaState[1] != 0) {
        evtStartSceneResourceTask(dds3GetWorldSecondaryObject(), (void *)fldAreaState[1]);
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

void fldSetDeferredFieldCommand(u32 arg0, u32 arg1) {
    fldDeferredCommand = arg0;
    fldDeferredCommandParameter = arg1;
}

extern s32 dds3AdminGetRequestedMode(void);
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

void fldSetPendingSceneAction(u32 command) {
    D_00435F70 = command;
}

void fldRunPendingSceneAction(void) {
    u32 command;

    command = D_00435F70;
    if (command != 0) {
        func_001411F8(command);
        D_00435F70 = 0;
    }
}

s32 fldConsumeNextSceneRequest(s32 *out0, s32 *out1) {
    if (fldAreaState[0x3C] == -1 && fldAreaState[0x3D] == fldAreaState[0x3C]) {
        return 0;
    }
    *out0 = fldAreaState[0x3C] + 0xC8;
    *out1 = fldAreaState[0x3D];
    fldAreaState[0x3C] = -1;
    fldAreaState[0x3D] = -1;
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
