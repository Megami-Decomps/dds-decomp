#include "ee_mmi.h"
#include "pcp_vu0.h"
#include "common.h"
#include "fpu.h"
#include "fld.h"

extern u32 D_003BABE0;

extern s32 D_003BABD8;

extern u32 D_003BAC00;

extern u32 D_003BABF8;
extern s32 D_003BABF0;

extern u32 D_003BABD0;

extern u64 dds3GetWorldSecondaryObject(void);
extern s64 func_00110400(u64);
extern u64 func_00110458(u64);
extern s64 dds3AdvanceObjectValueCursor(u64);
extern u64 func_00110AB0(u64, u64);
extern s64 func_00113E30(u64);

extern s32 D_003BAA00;

extern u64 sdfAllocPacketAligned(u64);
extern u32 D_003BABDC;
extern s32 D_003BAB08;
extern s32 D_003BAB0C;
extern s32 D_003BAB10;
extern s32 D_003BAB14;
extern s32 D_003BAB18;
extern s32 D_003BAB1C;
extern s32 D_003BAB20;
extern s32 D_0032E3B0[];
extern s32 D_0032E4DC[];
extern u8 D_00324F88[];
extern u8 D_003257F8[];
extern u8 D_00324530[];
extern char D_0039FA00[];
extern char D_0039FCA0[];
extern char D_0039FCB0[];
extern char D_0039FCC8[];
extern void fldStepValueByPad(f32 *value, u8 *pad, f32 min, f32 max, f32 step, f32 bigStep);
extern s32 fldTestDrawUpdate(void);
extern void fldClearCameraMoveMode(void);
extern void fldSetCameraMoveMode(s32);
extern void func_0013E5A8(u32 arg0);
extern u32 fldGetSceneReadyFlag(void);
extern s32 scrFindNamedProcessNode(const char *arg0);
extern void func_002D8C88(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
extern s32 kwlnTaskDestroyWithHierarchyByName(const char *name, s32 arg1);
extern u32 D_003BAB34;
extern u32 D_003BAB54;
extern u32 D_0032E498[];
extern u32 D_003BAB58;
extern u32 D_0032E3D0[];
extern u32 D_003BABE8;
extern u32 D_003BAB38;
extern void effObjFetchInnerFirstVec(u32);
extern void effObjFetchInnerSecondVecNorm(u32);
extern void fldSetPendingAreaAndFloor(s32, s32);
extern s32 fldIsAreaResourceReady(void);
extern void fldPollAreaResourceLoad(void);
extern s32 fldGetResourceReadyFlag(void);
extern void fldFreeDisplayObjects(void);
extern s32 sdfCheckPendingWorkWithInterrupts(void);
extern s32 func_00102A60(void);
extern s32 dds3AdminReadPreviousUnsignedSample(void);
extern void func_0013F100(s32, u32);
extern u32 D_0032E4EC[];
extern void func_002D0A10(u32 arg0);
extern s32 D_003BABEC;
extern u8 D_0034C8F0[];
extern void fldActivateObjectById(s32);
extern void mdlFlagSet(s32);
extern void func_0011B150(s32);
void fldDispatchDeferredFieldCommand(void);
extern u32 D_0032F1A0[];
extern s16 D_0032DDB0[];
extern u8 D_0033F068[];
extern u8 D_00342868[];
extern void *dds3AdvanceWorldCounter(void);
extern void *kwlnTaskGetTaskByName(const char *);
extern void dds3WorkClear(void);
extern char D_0039FBC0[]; /* "fldProcSequence" */
extern void *dds3SpawnInnerVecObj6(void *, u32 *, u32 *);
extern void dds3SetWorldEntryCallbackTarget(void *, const char *);
extern char D_0039FD50[]; /* "FLD_DMY_MATTER" */
extern u8 D_003BAB3C;
extern u8 D_0032C9A0[];
extern s16 D_0032C9B0[];
extern u8 D_00346068[];
extern u32 mnuAcknowledgeCampState(void);
void fldClearSceneCommandFlag(void);
void fldClearFieldTransitionFlag(void);
void func_00124788(void);
void func_00124850(void);
void fldClearSceneControlFlags(u32 arg0);
extern u32 D_003BAB64;
extern u32 D_003BAB60;
extern u32 D_003BAB5C;
extern u32 func_002D03F8(u32);
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

void func_0011D3A0(u32 *packet, u32 first, u32 second) {
    packet[4] = first;
    packet[5] = second;
}

u64 sdfCreateResetPacketList(void) {
    u64 packet;

    packet = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(packet);
    return packet;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011D3E8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011D570);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011D6B0);

void fldStepValueByPad(f32 *value, u8 *pad, f32 min, f32 max, f32 step, f32 bigStep) {
    f32 v = *value;

    if ((s8)pad[5] & 0x80) {
        v += bigStep;
        if (max < v) {
            v = min;
        }
    } else if (pad[5] & 2) {
        v += bigStep;
        if (max < v) {
            v = max;
        }
    } else if ((s8)pad[4] & 0x80) {
        v -= bigStep;
        if (v < min) {
            v = max;
        }
    } else if (pad[4] & 2) {
        v -= bigStep;
        if (v < min) {
            v = min;
        }
    } else if ((s8)pad[7] & 0x80) {
        v += step;
        if (max < v) {
            v = min;
        }
    } else if (pad[7] & 2) {
        v += step;
        if (max < v) {
            v = max;
        }
    } else if ((s8)pad[6] & 0x80) {
        v -= step;
        if (v < min) {
            v = max;
        }
    } else if (pad[6] & 2) {
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
    fldStepValueByPad(value, D_00324530, min, max, step, bigStep);
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

void func_0011DC50(void *ptr, s32 type, s64 min, s64 max, s64 step, s64 bigStep) {
    fldStepIntByPad(ptr, type, min, max, step, bigStep, (s8 *)D_00324530);
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

void fldStepColorChannelByCurrentPad(u32 *arg0, s32 arg1) {
    fldStepColorChannelByPad(arg0, arg1, D_00324530);
}

/* Formats a time in seconds as "d.dd" (digits saturate at 9.99). */
void fldFormatSecondsText(char *digits, f32 value) {
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
    digits[0] = whole + '0';
    digits[1] = '.';
    digits[2] = tenths + '0';
    digits[3] = hundredths + '0';
    digits[4] = 0;
}


INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011DEB0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011E080);

u32 func_0011E278(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011E280);

extern s32 fldGetEncounterRuntimeResult(void);
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

void fldResetCameraMoveTracking(void) {
    D_003BAB20 = 0;
    D_003BAB08 = -999;
    D_003BAB0C = -999;
    D_003BAB10 = -999;
    D_003BAB14 = -999;
    D_003BAB18 = -999;
    D_003BAB1C = -999;
    fldClearCameraMoveMode();
}


s32 fldEvaluateCameraMoveTracking(void) {
    if (D_003BAB20 == 1) {
        return 2;
    }
    if (D_003BAB1C != -999 && D_003BAB08 == D_003BAB14) {
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
void fldSubmitVisibleWorldBackground(void) {
    s32 found = 0;
    u64 list;
    u64 item;

    list = func_00110AB0(dds3GetWorldSecondaryObject(), 5);
    if (list != 0) {
        if (dds3ResetObjectValueCursor(list) != 0) {
            do {
                item = func_00110458(list);
                if (dds3TestObjectFlags(item, 0x200) != 0) {
                    if (dds3TestObjectFlags(item, 1) == 0) {
                        found = 1;
                    }
                }
            } while (dds3AdvanceObjectValueCursor(list) != 0);
        }
        dds3DestroyWorldIndexNode(list);
    }
    if (D_0032E3C0[0] == 1) {
        found = 0;
    }
    if (found != 0) {
        fldSelectDisplayBuffer(0x24);
        fldSubmitBackgroundResourcePacket();
        fldSelectDisplayBuffer(0x26);
        fldSubmitBackgroundDescriptorPacket();
    }
}

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FA28);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011ECC8);

extern u8 D_0033EC90[], D_00347C68[], D_003482A8[], D_00349030[], D_0032EF18[], D_0032EFE0[], D_0032F510[], D_0032FA10[], D_0032E5C8[], D_00336A60[];
extern void *D_003BAD64;
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
    sdfDevQueueReadAndWait(command, D_0032FA10, 0xC00);
    sdfDevQueueReadAndWait(command, D_003BAD64, 0x2A0);
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

/* Five adjacent halfword flag banks per 30-byte map slot; the bank
 * offsets are data-layout offsets, not independent map indices. */
void func_00120FA0(s32 map, s32 slot, s32 bit, s32 enabled) {
    if (map < 40) {
        s32 index = map % 100;
        if (enabled != 0) {
            s32 byteOffset = slot * 30 + index * 1920 + 0x1370;
            u16 *flags = (u16 *)(D_003BAA00 + byteOffset);
            *flags |= 1 << bit;
        } else {
            s32 byteOffset = slot * 30 + index * 1920 + 0x1370;
            u16 *flags = (u16 *)(D_003BAA00 + byteOffset);
            *flags &= ~(1 << bit);
        }
    }
}

u8 func_00121048(s32 map, u32 slot, u32 bit) {
    s32 mapIndex;
    u8 *slotBase;
    u32 flags;

    if (map < 0x28) {
        mapIndex = map % 100;
        slotBase = (u8 *)(slot * 30 + mapIndex * 1920);
        slotBase += D_003BAA00;
        flags = *(u16 *)(slotBase + 0x1370);
        return (flags >> bit) & 1;
    }
    return 0;
}

void func_001210A0(s32 map, s32 slot, s32 bit, s32 enabled) {
    if (map < 40) {
        s32 index = map % 100;
        if (enabled != 0) {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_003BAA00 + 0x1372);
            *flags |= 1 << bit;
        } else {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_003BAA00 + 0x1372);
            *flags &= ~(1 << bit);
        }
    }
}

u8 func_00121148(s32 map, u32 slot, u32 bit) {
    s32 mapIndex;
    u8 *slotBase;
    u32 flags;

    if (map < 0x28) {
        mapIndex = map % 100;
        slotBase = (u8 *)(slot * 30 + mapIndex * 1920);
        slotBase += D_003BAA00;
        flags = *(u16 *)(slotBase + 0x1372);
        return (flags >> bit) & 1;
    }
    return 0;
}

void func_001211A0(s32 map, s32 slot, s32 bit, s32 enabled) {
    if (map < 40) {
        s32 index = map % 100;
        if (enabled != 0) {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_003BAA00 + 0x1374);
            *flags |= 1 << bit;
        } else {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_003BAA00 + 0x1374);
            *flags &= ~(1 << bit);
        }
    }
}

u8 func_00121248(s32 map, u32 slot, u32 bit) {
    s32 mapIndex;
    u8 *slotBase;
    u32 flags;

    if (map < 0x28) {
        mapIndex = map % 100;
        slotBase = (u8 *)(slot * 30 + mapIndex * 1920);
        slotBase += D_003BAA00;
        flags = *(u16 *)(slotBase + 0x1374);
        return (flags >> bit) & 1;
    }
    return 0;
}

void fldSetMapTargetFlag(s32 map, s32 slot, s32 bit, s32 enabled) {
    if (map < 40) {
        s32 index = map % 100;
        if (enabled != 0) {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_003BAA00 + 0x1376);
            *flags |= 1 << bit;
        } else {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_003BAA00 + 0x1376);
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
        slotBase += D_003BAA00;
        flags = *(u16 *)(slotBase + 0x1376);
        return (flags >> bit) & 1;
    }
    return 0;
}

void func_001213A0(s32 map, s32 slot, s32 bit, s32 enabled) {
    if (map < 40) {
        s32 index = map % 100;
        if (enabled != 0) {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_003BAA00 + 0x1378);
            *flags |= 1 << bit;
        } else {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_003BAA00 + 0x1378);
            *flags &= ~(1 << bit);
        }
    }
}

u8 func_00121448(s32 map, u32 slot, u32 bit) {
    s32 mapIndex;
    u8 *slotBase;
    u32 flags;

    if (map < 0x28) {
        mapIndex = map % 100;
        slotBase = (u8 *)(slot * 30 + mapIndex * 1920);
        slotBase += D_003BAA00;
        flags = *(u16 *)(slotBase + 0x1378);
        return (flags >> bit) & 1;
    }
    return 0;
}

void fldSetMapSlotByte(s32 map, u32 slot, s32 offset, s32 value) {
    if (map < 40) {
        s32 index = map % 100;
        s32 displacement = offset + index * 1920;
        u8 *entry = (u8 *)(slot * 30 + displacement);
        entry += D_003BAA00;
        entry[0x137A] = value;
    }
}

s32 fldGetMapSlotByte(s32 map, u32 slot, s32 offset) {
    u8 value = 0;
    if (map < 40) {
        s32 index = map % 100;
        s32 displacement = offset + index * 1920;
        u8 *entry = (u8 *)(slot * 30 + displacement);
        entry += D_003BAA00;
        value = entry[0x137A];
    }
    return value == 0xff ? -1 : value;
}

void func_00121550(s32 map, s32 slot, s32 bit, s32 enabled) {
    if (map < 40) {
        s32 index = map % 100;
        if (enabled != 0) {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_003BAA00 + 0x138A);
            *flags |= 1 << bit;
        } else {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_003BAA00 + 0x138A);
            *flags &= ~(1 << bit);
        }
    }
}

u8 func_001215F8(s32 map, u32 slot, u32 bit) {
    s32 mapIndex;
    u8 *slotBase;
    u32 flags;

    if (map < 0x28) {
        mapIndex = map % 100;
        slotBase = (u8 *)(slot * 30 + mapIndex * 1920);
        slotBase += D_003BAA00;
        flags = *(u16 *)(slotBase + 0x138A);
        return (flags >> bit) & 1;
    }
    return 0;
}

void func_00121650(s32 map, s32 slot, s32 bit, s32 enabled) {
    if (map < 40) {
        s32 index = map % 100;
        if (enabled != 0) {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_003BAA00 + 0x138C);
            *flags |= 1 << bit;
        } else {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_003BAA00 + 0x138C);
            *flags &= ~(1 << bit);
        }
    }
}

u8 func_001216F8(s32 map, u32 slot, u32 bit) {
    s32 mapIndex;
    u8 *slotBase;
    u32 flags;

    if (map < 0x28) {
        mapIndex = map % 100;
        slotBase = (u8 *)(slot * 30 + mapIndex * 1920);
        slotBase += D_003BAA00;
        flags = *(u16 *)(slotBase + 0x138C);
        return (flags >> bit) & 1;
    }
    return 0;
}

typedef struct FieldActivationRecord {
    s32 kind;
    s16 parameter;
    u8 pad06[10];
} FieldActivationRecord;

#define FIELD_ACTIVATION_FLAGS_OFFSET 0x15970

void fldActivateFlaggedObject(s32 flagIndex) {
    s32 byteOffset = (flagIndex >> 3) + FIELD_ACTIVATION_FLAGS_OFFSET;
    u8 *byte = (u8 *)(D_003BAA00 + byteOffset);
    FieldActivationRecord *entry;
    *byte |= 1 << (flagIndex & 7);
    fldActivateObjectById(flagIndex);
    if ((u32)(flagIndex - 0xF0) < 16) {
        mdlFlagSet(flagIndex + 0x610);
    }
    entry = (FieldActivationRecord *)(flagIndex * 16 + (s32)D_0034C8F0);
    if (entry->kind == 2) {
        func_0011B150(entry->parameter);
    }
}

u8 fldTestObjectActivationFlag(u32 flagIndex) {
    return (*(u8 *)(((s32)flagIndex >> 3) + D_003BAA00 + FIELD_ACTIVATION_FLAGS_OFFSET) >> (flagIndex & 7)) & 1;
}

/* Coordinate tables have 28-byte records; the first record is reserved. */
s32 func_00121818(s32 x, s32 y) {
    u8 *records = D_0033F068;
    u8 *second = records + 2;
    s32 index = 1;
    s32 offset = 28;
    do {
        if (x == *(s16 *)(offset + (s32)records) && y == *(s16 *)(offset + (s32)second)) {
            return index;
        }
        index++;
        offset += 28;
    } while (index < 512);
    return 0;
}

s32 func_00121870(s32 x, s32 y) {
    u8 *records = D_00342868;
    u8 *second = records + 2;
    s32 index = 1;
    s32 offset = 28;
    do {
        if (x == *(s16 *)(offset + (s32)records) && y == *(s16 *)(offset + (s32)second)) {
            return index;
        }
        index++;
        offset += 28;
    } while (index < 512);
    return 0;
}

s32 fldFindMapCoordinateIndex(s32 x, s32 y) {
    u8 *records = D_00346068;
    u8 *second = records + 2;
    s32 index = 1;
    s32 offset = 28;
    do {
        if (x == *(s16 *)(offset + (s32)records) && y == *(s16 *)(offset + (s32)second)) {
            return index;
        }
        index++;
        offset += 28;
    } while (index < 256);
    return 0;
}

s16 *fldFindLocationCoordinateRecord(s32 x, s32 y) {
    u8 *records = (u8 *)D_0032C9B0;
    u8 *second = records + 2;
    s32 index = 1;
    do {
        s32 offset = index * 8;
        index++;
        if (x == *(s16 *)(offset + (s32)records) && y == *(s16 *)(offset + (s32)second)) {
            return (s16 *)(offset + (s32)records);
        }
    } while (index < 640);
    return D_0032C9B0;
}

s32 fldGetLocationCoordinateValue(s32 x, s32 y) {
    u8 *records = (u8 *)D_0032C9B0;
    u8 *second = records + 2;
    u8 *result = records + 6;
    s32 index = 1;
    do {
        s32 offset = index * 8;
        index++;
        if (x == *(s16 *)(offset + (s32)records) && y == *(s16 *)(offset + (s32)second)) {
            return *(s16 *)(offset + (s32)result);
        }
    } while (index < 640);
    return 0;
}

u32 fldFindStageCoordinateIndex(s32 x, s32 y) {
    s16 *entry = D_0032DDB0;
    s32 index = 0;
    s32 checked = 0;

    do {
        if (entry[0] == x && entry[1] == y) {
            return index;
        }
        index++;
        checked++;
        entry += 8;
    } while (checked < 0x280);
    return index;
}

s16 * fldFindStageCoordinateRecord(s32 x, s32 y) {
    s16 *entry = D_0032DDB0;
    s32 checked = 0;

    do {
        if (entry[0] == x && entry[1] == y) {
            return entry;
        }
        checked++;
        entry += 8;
    } while (checked < 0x60);
    return NULL;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121A58);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121B88);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121DE0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121ED8);

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

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00122100);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FB18);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FB60);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00122498);

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

f32 func_001229A8(void) {
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

void fldToggleWorldNodeState(s64 clearMode) {
    u64 list;
    s64 status;
    u64 object;

    list = dds3GetWorldSecondaryObject();
    list = func_00110AB0(list, 6);
    status = func_00110400(list);
    if (status == 0) {
        return;
    }
    dds3ResetObjectValueCursor(list);
    do {
        object = func_00110458(list);
        status = func_00113E30(object);
        if (status == 4) {
            if (clearMode == 0) {
                func_00113E20(object, 3);
            }
            else {
                func_00113E20(object, 0);
            }
        }
        status = dds3AdvanceObjectValueCursor(list);
    } while (status != 0);
    dds3DestroyWorldIndexNode(list);
}

extern u32 D_0032E570[];
extern void func_00118020(s32);
extern s32 fileLoadStateChanged(void);
extern void fileCacheSlotFlagsFromState(void);
extern void fileRestoreSlotFlagsToState(void);
void fldSetDeferredFieldCommand(u32, u32);
extern void func_001028E8(s32, void *, s32, s32);

void func_00122CB8(void) {
    if (*(s16 *)(D_003BAA00 + 0xe) != 0) {
        func_00118020(1);
    } else {
        func_00118020(0);
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
        func_001028E8(6, &code, 4, 0);
    }
}


extern s32 fileGetSelectionPendingFlag(void);
extern void func_00120C08(s32);
extern char D_003BAB68[];
void fldStartSequenceRecord(void) {
    u8 buffer[0xA0];

    if (fileGetSelectionPendingFlag() == 1) {
        return;
    }
    if (*(s16 *)(D_003BAA00 + 0xE) != 0) {
        func_00122CB8();
        return;
    }
    func_00120C08(1);
    D_0032E570[0x4C / 4] = 0;
    D_0032E570[0x44 / 4] = 1;
    mdlFlagSet(0xC0F);
    fldSetDeferredFieldCommand(0, 0);
    fldInitializeSequenceAndResetFlags(buffer, 1, 1, D_003BAB68);
    *(u32 *)(buffer + 0x90) = 1;
    /* Record options at +0x90; command submission copies the padded 0xA0-byte packet. */
    func_001028E8(5, buffer, 0xA0, 0);
}

extern u64 D_003BAB70[], D_003BAB78[], D_003BAB80[], D_003BAB88[], D_003BAB90[];
extern u64 D_003BAB98[], D_003BABA0[], D_003BABA8[], D_003BABB0[];
extern u8 D_0032F240[];
extern void func_003003F0(void *);
extern void fldInitDisplayObjects(void);
extern void func_00122ED0(void);
extern void fldResetPendingSounds(void);
extern void func_002E8430(void *, s32);
extern void fldParseMixLb(void);
extern void func_00131A88(void);
extern void func_001426E0(void);

void fldInitializeDisplayAndTables(void) {
    func_003003F0(D_003BAB70);
    fldInitDisplayObjects();
    func_003003F0(D_003BAB78);
    func_00122ED0();
    func_003003F0(D_003BAB80);
    fldResetPendingSounds();
    func_003003F0(D_003BAB88);
    func_002E8430(D_0032F240, 0x1e240);
    func_003003F0(D_003BAB90);
    fldParseMixLb();
    func_003003F0(D_003BAB98);
    func_00131A88();
    func_003003F0(D_003BABA0);
    func_001426E0();
    func_003003F0(D_003BABA8);
    fldLoadFieldTables();
    func_003003F0(D_003BABB0);
}

void func_00122ED0(void) {
    u32 *buffer = D_0032F1A0;

    VU0_STORE_VF(vf0, buffer);
    VU0_STORE_VF(vf0, buffer + 4);
    D_003BAB34 = 0;
    *fldGetPlayerSceneStateAddress() = 0;
}

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
    u32 options;
} FieldSequenceRecord;

void fldInitializeSequenceAndResetFlags(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name) {
    if (kwlnTaskGetTaskByName(D_0039FBC0) != NULL) {
        if (D_0032E3B0[4] == stage) {
            D_0032E3B0[8] = 1;
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

void fldInitializeAlternateSequence(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name) {
    if (kwlnTaskGetTaskByName(D_0039FBC0) != NULL) {
        if (D_0032E3B0[4] == stage) {
            D_0032E3B0[8] = 1;
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
    if (kwlnTaskGetTaskByName(D_0039FBC0) != NULL) {
        if (D_0032E3B0[4] == stage) {
            D_0032E3B0[8] = 1;
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

void fldInitializeSequenceWithNote(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name,
                    s32 code, const char *subname) {
    if (kwlnTaskGetTaskByName(D_0039FBC0) != NULL) {
        if (D_0032E3B0[4] == stage) {
            D_0032E3B0[8] = 1;
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
    u32 *sceneWork = D_0032F1A0;

    if (sceneWork[0xD] == 1) {
        return 1;
    }
    if (sceneWork[0x17] == 3) {
        return 3;
    }
    if (sceneWork[0x17] == 4) {
        return 4;
    }
    if (sceneWork[0x17] == 5) {
        return 5;
    }
    if (sceneWork[0x17] == 6) {
        return 2;
    }
    return sceneWork[0xD] != 0 ? 2 : 0;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001233D0);

void fldUnloadPlayerModel(void);
extern u32 func_002EB028(const char *, u32 *, u32 *);
INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FBC0);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FBD0);

void fldLoadPlayerModel(void) {
    s32 model;

    if (fldGetLocationCoordinateValue(D_0032E3B0[4], D_0032E3B0[5] + 1) & 0x20) {
        model = 2;
    } else {
        model = D_0032E3B0[70] != 0;
    }
    if (D_0032E3B0[79] != model || D_003BAB54 == 0) {
        if (D_003BAB54 != 0) {
            fldUnloadPlayerModel();
        }
        switch (model) {
        case 0:
            D_003BAB54 = func_002EB028("/model/field/player_a.PB", &D_003BAB5C, &D_003BAB64);
            break;
        case 1:
            D_003BAB54 = func_002EB028("/model/field/player_b.PB", &D_003BAB5C, &D_003BAB64);
            break;
        default:
            D_003BAB54 = func_002EB028("/model/field/player_l.PB", &D_003BAB5C, &D_003BAB64);
            break;
        }
        D_0032E3B0[79] = model;
    }
}

void fldUnloadPlayerModel(void) {
    if (D_003BAB54 != 0) {
        func_002D0A10(D_003BAB54);
        D_003BAB54 = 0;
        D_0032E4EC[0] = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001239C8);

void fldPrepareResourceBuffer(void) {
    void *source;
    void *buffer;
    fldLoadPlayerModel();
    D_003BAB60 = D_003BAB64;
    D_003BAB58 = func_002D03F8(D_003BAB64);
    source = sdfMemoryGetBlockAddress(D_003BAB54);
    buffer = sdfMemoryGetBlockAddress(D_003BAB58);
    memcpy(buffer, source, D_003BAB60);
    D_003BAB5C = (u32)buffer;
    mdlLoadViewerPackage(2, 0, 0x101, buffer, D_003BAB60);
    func_002D0A10(D_003BAB58);
    D_003BAB58 = 0;
}

void fldReleaseResources(void) {
    if (D_003BAB58 != 0) {
        func_002D0A10(D_003BAB58);
        D_003BAB58 = 0;
    }
    if (D_0032E3D0[0] == 0 && D_003BABE8 == 0) {
        fldUnloadPlayerModel();
        fldSetPendingAreaAndFloor(0, 0);
    }
}

void fldReleasePlayerSceneResources(void) {
    u32 *buffer;

    if (D_003BAB34 != 0) {
        if (dds3GetWorldSecondaryObject() != 0) {
            buffer = D_0032F1A0;
            effObjFetchInnerFirstVec(D_003BAB34);
            VU0_STORE_VF(vf10, buffer);
            effObjFetchInnerSecondVecNorm(D_003BAB34);
            VU0_STORE_VF(vf10, buffer + 4);
        }
        D_003BAB34 = 0;
        D_003BAB38 = 0;
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

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00123E00);

void fldResetPlayerSceneObjectState(void) {
    if (D_003BAB34 != 0) {
        fldClearSceneControlFlags(0x40);
        dds3InvokeSlot1Handler(D_003BAB34, 0);
    }
    D_0032E498[0] = 4;
}

extern void dds3ClearObjectFlags(u32, s32);
extern void func_00110A00(u64, u32);
extern void func_00111E30(u32, s32, s32);
extern u32 dds3SpawnCameraSlotObj5(void *, f32 *, f32 *);
extern s32 D_0032F1DC[];
extern char D_0039FC50[]; /* "PLAYER_UNIT" */
extern s32 D_003BAB50;
void fldCreatePlayerObject(void) {
    f32 pos[4];
    f32 rot[4];

    memset(pos, 0, sizeof(pos));
    pos[3] = 1.0f;
    memset(rot, 0, sizeof(rot));
    rot[3] = 1.0f;
    if (D_003BAB34 == 0) {
        D_003BAB34 = dds3SpawnCameraSlotObj5(dds3AdvanceWorldCounter(), pos, rot);
        dds3SetWorldEntryCallbackTarget((void *)D_003BAB34, D_0039FC50);
        func_00110A00(dds3GetWorldSecondaryObject(), D_003BAB34);
        if (D_003BAB50 != 0) {
            dds3ClearObjectFlags(D_003BAB34, 0x20);
        }
        fldPrepareResourceBuffer();
        func_00111E30(D_003BAB34, 2, D_0032F1DC[0]);
    }
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00123FB8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001242B8);

u32 func_001243C0(void) {
    return 0;
}

s32 fldSelectSceneCommand(void) {
    s32 mode = D_0032E3B0[4];
    s32 result = 0x64;
    if (mode == 0x15) {
        s32 variant = D_0032E3B0[5];
        result = 0x66;
        if (variant == 2) result = 0x64;
        if (variant == 0x17) result = 0x65;
    }
    if (mode == 0x1a) {
        s32 variant = D_0032E3B0[5];
        result = 0x66;
        if (variant == 0x12) result = 0x65;
        if (variant == 0x13) result = 0x65;
        if (variant == 0x15) result = 0x65;
    }
    if (mode == 0x1d) {
        s32 variant = D_0032E3B0[5];
        result = 0x66;
        if (variant == 1) result = 0x64;
        if (variant == 9) result = 0x65;
        if (variant == 0xa) result = 0x65;
    }
    if (mode == 0x25 && D_0032E3B0[5] == 0xf) {
        result = 0x64;
    }
    return result;
}

void fldConsumeSceneCommandFlag(void) {
    if ((((FldWorkFlags *)D_003BAA00)->fieldFlags & 8) != 0) {
        fldClearSceneCommandFlag();
        D_0032E3B0[3] |= 1;
    }
}

void fldClearSceneCommandFlag(void) {
    ((FldWorkFlags *)D_003BAA00)->fieldFlags &= ~8;
    D_0032E3B0[3] &= ~1;
}


extern void fldPlayFieldSeVolumePan(s32);
extern void func_00133640(s16, s32);
void fldEnterSceneCommand(void) {
    s32 code;

    if (D_0032E3B0[75] == 0) {
        return;
    }
    if ((((FldWorkFlags *)D_003BAA00)->fieldFlags & 8) == 0) {
        fldPlayFieldSeVolumePan(0x29);
    }
    code = fldSelectSceneCommand();
    *(s16 *)((u8 *)D_0032E3B0 + 0x128) = code;
    ((FldWorkFlags *)D_003BAA00)->fieldFlags |= 8;
    D_0032E3B0[3] &= ~1;
    func_00133640(code, 0);
}

s32 fldGetSceneCommandState(void) {
    if ((((FldWorkFlags *)D_003BAA00)->fieldFlags & 8) != 0) {
        return 1;
    }
    if (D_0032E4DC[0] != 0) {
        return 0;
    }
    return -1;
}


typedef struct FldSceneState {
    u8 pad0[0x128];
    s16 sceneCommand; /* 0x128 */
    u8 pad12A[2];
    s32 commandEnabled; /* 0x12C: D_0032E3B0[75] gates scene command updates */
} FldSceneState;
void fldUpdateSceneCommand(void) {
    FldSceneState *state = (FldSceneState *)D_0032E3B0;
    s32 code;

    if (state->commandEnabled == 0) {
        if (state->sceneCommand != 0) {
            state->sceneCommand = 0;
            func_00133640(0, 0);
        }
    } else if ((((FldWorkFlags *)D_003BAA00)->fieldFlags & 8) != 0) {
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

void fldConsumeFieldTransitionFlag(void) {
    if ((((FldWorkFlags *)D_003BAA00)->fieldFlags & 4) != 0) {
        fldClearFieldTransitionFlag();
        D_0032E3B0[3] |= 2;
    }
}

void fldClearFieldTransitionFlag(void) {
    ((FldWorkFlags *)D_003BAA00)->fieldFlags &= ~4;
    D_0032E3B0[3] &= ~2;
}

void fldSetFieldTransitionFlag(void) {
    ((FldWorkFlags *)D_003BAA00)->fieldFlags |= 4;
    D_0032E3B0[3] &= ~2;
}

u8 fldTestFieldTransitionFlag(void) {
    s32 flags = ((FldWorkFlags *)D_003BAA00)->fieldFlags;
    flags &= 4;
    return flags != 0;
}

void func_00124740(void) {
    if ((((FldWorkFlags *)D_003BAA00)->fieldFlags & 2) != 0) {
        func_00124788();
        D_0032E3B0[3] |= 4;
    }
}

void func_00124788(void) {
    ((FldWorkFlags *)D_003BAA00)->fieldFlags &= ~2;
    D_0032E3B0[3] &= ~4;
}

void func_001247B8(void) {
    ((FldWorkFlags *)D_003BAA00)->fieldFlags = (((FldWorkFlags *)D_003BAA00)->fieldFlags | 2) & ~1;
    D_0032E3B0[3] &= ~4;
}

u8 func_001247F0(void) {
    s32 flags = ((FldWorkFlags *)D_003BAA00)->fieldFlags;
    flags &= 2;
    return flags != 0;
}

void func_00124808(void) {
    if ((((FldWorkFlags *)D_003BAA00)->fieldFlags & 1) != 0) {
        func_00124850();
        D_0032E3B0[3] |= 8;
    }
}

void func_00124850(void) {
    ((FldWorkFlags *)D_003BAA00)->fieldFlags &= ~1;
    D_0032E3B0[3] &= ~8;
}

void func_00124880(void) {
    ((FldWorkFlags *)D_003BAA00)->fieldFlags = (((FldWorkFlags *)D_003BAA00)->fieldFlags | 1) & ~2;
    D_0032E3B0[3] &= ~8;
}

u8 fldIsFlagActive(void) {
    s32 flags = ((FldWorkFlags *)D_003BAA00)->fieldFlags;
    flags &= 1;
    if (flags == 0) return 0;
    return 1;
}

void func_001248D0(void) {
    func_002D8C88(D_003257F8, (s32)D_00324F88, (s32)(D_00324F88 + 0xC0), (s32)(D_00324F88 + 0x100), (s32)(D_00324F88 + 0xE0));
}


typedef struct FldEncEntry {
    s16 stage;
    s16 flag;
    s16 chance;
    s16 result;
} FldEncEntry;
extern FldEncEntry *D_003BAA40;
extern s32 mdlFlagTest();
extern s32 effMiscRand();
s16 fldRollEncounter(void) {
    s32 i;
    s32 enabled;

    for (i = 0; i < 16; i++) {
        if (D_003BAA40[i].stage == D_0032E3B0[4]) {
            enabled = 1;
            if (D_003BAA40[i].flag != -1) {
                enabled = mdlFlagTest(D_003BAA40[i].flag) != 0;
            }
            if (enabled != 0) {
                if ((u32)effMiscRand(0) % 100U < (u32)D_003BAA40[i].chance) {
                    return D_003BAA40[i].result;
                }
            }
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001249E0);

s32 fldAdvanceToNextScene(void) {
    u32 scene;
    u32 area;

    fldResetEventSceneState();
    D_003BAC08[0] = 0;
    D_003BAC08[1] = func_0014D100();
    scene = D_0032E3B0[5];
    area = D_0032E3B0[4];
    D_0032E3B0[0x3C] = area;
    D_0032E3B0[0x3D] = scene + 1;
    fldResetTaskSlots();
    fldSetSceneLifecycleFlags(1);
    fldSetSceneLifecycleFlags(2);
    fldResetPlayerSceneObjectState();
    func_00220178();
    return -1;
}

u8 func_00124E90(void) {
    return scrFindNamedProcessNode(D_0039FCA0) != 0;
}

u8 func_00124EB8(void) {
    return scrFindNamedProcessNode(D_0039FCC8) != 0;
}

u8 func_00124EE0(void) {
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

u8 func_00125140(void) {
    if (D_003BABF0 > 0) {
        return 2;
    }
    return fldGetSceneReadyFlag() != 0;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00125170);

extern void evtStartSceneResourceTask(u64, void *);
extern void func_0014C468(void);
/* Dispatch one pending field command, preferring the temporary override
 * over the scene-work buffer and its saved fallback. */
s32 fldDispatchPendingSceneResource(void) {
    u32 *sceneWork = D_0032F1A0;
    u32 flags;

    if (D_0032E3B0[64] == 1) {
        func_0014C468();
    }
    D_0032E3B0[64] = 0;
    if ((D_003BAB3C & 2) && *(s8 *)D_0032C9A0 != 0) {
        evtStartSceneResourceTask(dds3GetWorldSecondaryObject(), D_0032C9A0);
        flags = D_003BAB3C;
        if (!(flags & 1)) {
            D_0032C9A0[0] = 0;
            D_003BAB3C = flags & 0xFB;
        }
        return 1;
    }
    if (sceneWork[13] == 0 && *(s8 *)((u8 *)sceneWork + 0x80) != 0) {
        evtStartSceneResourceTask(dds3GetWorldSecondaryObject(), (u8 *)sceneWork + 0x80);
        return 1;
    }
    if (D_0032E3B0[1] != 0) {
        evtStartSceneResourceTask(dds3GetWorldSecondaryObject(), (void *)D_0032E3B0[1]);
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

/* Independent flag words: D_003BABF8 and D_003BAC00 each have
 * their own set, clear and test operations. */
void fldSetSceneLifecycleFlags(u32 mask) {
    u32 *flags = &D_003BABF8;
    *flags |= mask;
}

void fldClearSceneLifecycleFlags(u32 mask) {
    u32 *flags = &D_003BABF8;
    *flags &= ~mask;
}

u8 fldTestSceneLifecycleFlags(u32 mask) {
    return (D_003BABF8 & mask) != 0;
}

void fldSetSceneControlFlags(u32 mask) {
    D_003BAC00 = D_003BAC00 | mask;
}

void fldClearSceneControlFlags(u32 mask) {
    D_003BAC00 = D_003BAC00 & ~mask;
}

u8 fldTestSceneControlFlags(u32 mask) {
    return (D_003BAC00 & mask) != 0;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00125E08);

extern s32 D_003BABF4, D_003BABE4;
extern char D_0039FD30[], D_0039FD40[];
extern void kwlnFadeResetBackground(void);
extern void fldDestroyPanelTaskIfPresent(void), evtSetSolarOverlayFullyTransparent(void), fldDestroyTask(void);
extern void effBlurReleaseFirstResource(s32), mnuDestroyCampTasks(void), scrDestroyAllNamedProcesses(void);
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
        effBlurReleaseFirstResource(D_003BABE4);
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
    func_0013EC10();
}

void func_00126050(void) {
    fldStopCurrentBgm();
}

void fldProcessDeferredSceneCommand(void) {
    if (D_003BABD8 != 0) {
        fldDispatchDeferredFieldCommand();
        return;
    }
    func_0013EC68();
}

void fldSetDeferredFieldCommand(u32 command, u32 parameter) {
    D_003BABD8 = command;
    D_003BABDC = parameter;
}

void fldDispatchDeferredFieldCommand(void) {
    if (D_003BABD8 == 0) return;
    if (func_00102A60() > 0) return;
    if (func_00102A60() < 0 && (dds3AdminReadPreviousUnsignedSample() & 1) != 0) return;
    func_0013F100(D_003BABD8, D_003BABDC);
    D_003BABD8 = 0;
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
    if (D_0032E3B0[0x3C] == -1 && D_0032E3B0[0x3D] == -1) {
        return 0;
    }
    *outCode = D_0032E3B0[0x3C] + 0xC8;
    *outParameter = D_0032E3B0[0x3D];
    D_0032E3B0[0x3C] = -1;
    D_0032E3B0[0x3D] = -1;
    return 1;
}

void *fldCreateDummyMatter(void) {
    u32 args[8];
    void *matter;
    args[0] = 0;
    args[1] = 0;
    args[2] = 0;
    args[3] = 0;
    args[4] = 0;
    args[5] = 0;
    args[6] = 0;
    args[7] = 0;
    matter = dds3SpawnInnerVecObj6(dds3AdvanceWorldCounter(), args, args + 4);
    dds3SetWorldEntryCallbackTarget(matter, D_0039FD50);
    return matter;
}

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FD30);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FD40);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FD50);

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

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB34);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB38);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB3C);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB40);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB50);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB54);

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

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABD8);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABDC);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABE0);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABE4);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABE8);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABEC);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABF0);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABF4);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABF8);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABFC);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAC00);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAC08);

