#include "common.h"
#include "fpu.h"

#include "fld.h"

extern s32 D_00435F68;

extern u64 dds3GetWorldSecondaryObject(void);

extern s64 func_00110628(u64);

extern u64 func_00110680(u64);

extern s64 func_001106D8(u64);

extern u64 func_00110CD8(u64, u64);

extern s64 func_00114058(u64);

extern u64 sdfAllocPacketAligned(u64);

extern s32 D_00435DD0;

extern u32 D_00435F60;

extern u32 D_00435F88;

extern u32 D_00435F90;

extern u32 D_00435F70;

extern s16 D_00389170[];

extern u32 D_0038A640[];

extern u32 D_00435F34;

extern u32 D_003898B8[];

extern void func_003298C0(u32 arg0);

extern u32 D_00435F0C;

extern u32 D_00389858[];

void func_00128390(u32 arg0);

void func_00126730(void);

extern s32 D_0038989C[];

void func_00126958(void);

void func_00126A28(void);

void func_00126AF0(void);

extern char D_00412F30[];

extern s32 func_0010C100(const char *arg0);

extern char D_00412F58[];

extern char D_00412F40[];

extern s32 D_00435F7C;

extern u32 mnuAcknowledgeCampState(void);

u8 func_001283A8(u32 arg0);

extern s32 D_00435F80;

extern u32 func_0014A250(void);

extern u32 D_00435F6C;

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
    u32 unk_90;
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

extern u32 func_003292A8(u32);

extern void *sdfMemoryGetBlockAddress(u32);

extern void mdlLoadViewerPackage(s32, s32, s32, void *, u32);

void func_001258B8(void);

extern u32 D_00389790[];

extern s32 fldIsAreaResourceReady(void);

extern void fldPollAreaResourceLoad(void);

extern s32 fldGetResourceReadyFlag(void);

extern void fldFreeDisplayObjects(void);

extern s32 sdfGraphHasPendingWorkInterruptSafe(void);

extern void *dds3AdvanceWorldCounter(void);

extern void *dds3SpawnInnerVecObj6(void *, u32 *, u32 *);

extern void dds3SetWorldEntryCallbackTarget(void *, const char *);

extern char D_00412FF0[]; /* "FLD_DMY_MATTER" */

extern f32 func_001248E8(f32, f32, f32, f32);

extern void fldStepValueByPad(f32 *value, u8 *padState, f32 min, f32 max, f32 step, f32 bigStep);

extern u8 D_0037F530[];

extern void func_0011F928(void *, s32, s64, s64, s64, s64, s8 *);

extern s32 func_0011FAD8(u32 *, s32, s8 *);

extern s32 fldTestDrawUpdate(void);

extern s32 func_0012EB78(void);

extern u8 func_00127398(void);

extern void fldSubmitFrameQuad(s32, s32, s32, s32, s32, s32, s32, s32);

extern void func_0012BE18(s32);

extern void func_00136EF8(void);

extern void func_00134A18(void);

extern void func_0012D3E0(void);

extern u32 D_00389988[];

extern void func_0012BC38(s32);

extern void func_0012BC38(s32);

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

extern void func_00130C20(void);

extern u32 func_001106B8(u64);

extern s32 dds3TestObjectFlags(u64, s32);

extern void dds3DestroyWorldIndexNode(u64);

extern void func_0012DCC8(void);

extern void func_0012DD48(void);

extern s32 D_00389780[];

extern void mdlFlagSet(s32);

extern void func_00118598(s32);

extern s32 fileLoadStateChanged(void);

extern void func_002D0EB0(void);

extern void func_002D0EC8(void);

void fldSetDeferredFieldCommand(u32, u32);

extern void func_001027D8(s32, void *, s32, s32);

u32 *func_00125F28(void);

extern void func_00124E80(void);

extern u32 D_00435F78;

extern void fldSetPendingAreaAndFloor(s32, s32);

void fldUnloadPlayerModel(void);

extern void dds3ClearObjectFlags(u32, s32);

extern void func_00110C28(u64, u32);

extern void func_00112058(u32, s32, s32);

extern u32 dds3SpawnCameraSlotObj5(void *, f32 *, f32 *);

extern s32 D_0038A67C[];

extern char D_00412EC0[]; /* "PLAYER_UNIT" */

extern s32 D_00435F30;

extern u8 D_0037FF88[];

extern u8 D_003807F8[];

extern void func_00331B38(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

extern s32 D_00389770[];

typedef struct FldEncEntry {
    s16 stage;
    s16 flag;
    s16 chance;
    s16 result;
} FldEncEntry;

extern FldEncEntry *D_00435E10;

extern s32 mdlFlagTest();

extern s32 effMiscRand();

extern u32 D_00435F98[2];

extern void func_001512E8(void);

extern u32 func_00151498(void);

extern void fldResetTaskSlots(void);

extern void func_00128340(u32);

extern void func_0023ACE8(void);

extern void func_00110F28(u64, void *);

extern void func_00150800(void);

extern s32 D_00435F84, D_00435F74;

extern char D_00412FD0[], D_00412FE0[];

extern void kwlnFadeResetBackground(void);

extern void func_00144238(void), evtSetSolarOverlayFullyTransparent(void), fldDestroyTask(void);

extern void func_0018ECB8(s32), mnuDestroyCampTasks(void), func_0010BFE0(void);

extern void func_0014A228(void);

void func_0011F208(u32 *state, u32 firstValue, u32 secondValue) {
    state[4] = firstValue;
    state[5] = secondValue;
}

u64 sdfCreateResetPacketList(void) {
    u64 packet;

    packet = sdfAllocPacketAligned(0x20);
    sdfResetPacketList(packet);
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

void func_0011F928(void *ptr, s32 type, s64 min, s64 max, s64 small, s64 big, s8 *pad) {
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


void func_0011FAB8(void *ptr, s32 type, s64 min, s64 max, s64 step, s64 bigStep) {
    func_0011F928(ptr, type, min, max, step, bigStep, (s8 *)D_0037F530);
}

s32 func_0011FAD8(u32 *color, s32 channel, s8 *pad) {
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


void func_0011FC68(u32 *arg0, s32 arg1) {
    func_0011FAD8(arg0, arg1, (s8 *)D_0037F530);
}

void func_0011FC88(f32 value, char *out) {
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
    if (func_0012EB78() != 0) {
        return 0;
    }
    if (func_00127398() == 1) {
        return 0;
    }
    func_0012BC38(0x53);
    fldSubmitFrameQuad(1, 0, 0x81, 3, 0, 0, 1, 1);
    func_0012BE18(0);
    func_00136EF8();
    func_00134A18();
    if (D_00389988[9] != 0) {
        if (func_0014A250() == 0) {
            func_0012BC38(0x53);
        } else {
            func_0012BC38(0x5E);
        }
        func_0012D3E0();
    }
    func_0012BC38(0x27);
    fldSubmitFrameQuad(1, 5, 0x80, 1, 0, 0, 1, 2);
    func_0012BE18(0);
    func_0012BC38(0x39);
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

void func_00120820(void) {
    D_00435EF8 = 0;
    D_00435EE0 = -999;
    D_00435EE4 = -999;
    D_00435EE8 = -999;
    D_00435EEC = -999;
    D_00435EF0 = -999;
    D_00435EF4 = -999;
    func_00130C20();
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_00120858);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412B90);

INCLUDE_ASM(const s32, "game/code_0011F208", func_001208D0);

void func_00120A88(void) {
    s32 found = 0;
    u64 list;
    u64 item;

    list = func_00110CD8(dds3GetWorldSecondaryObject(), 5);
    if (list != 0) {
        if (func_001106B8(list) != 0) {
            do {
                item = func_00110680(list);
                if (dds3TestObjectFlags(item, 0x200) != 0) {
                    if (dds3TestObjectFlags(item, 1) == 0) {
                        found = 1;
                    }
                }
            } while (func_001106D8(list) != 0);
        }
        dds3DestroyWorldIndexNode(list);
    }
    if (D_00389780[0] == 1) {
        found = 0;
    }
    if (found != 0) {
        func_0012BC38(0x24);
        func_0012DCC8();
        func_0012BC38(0x26);
        func_0012DD48();
    }
}

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412BB8);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00120B88);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00122828);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00122A38);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00122B58);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00122E50);

void func_00122F38(s32 map, s32 slot, s32 bit, s32 enabled) {
    if (map < 40) {
        s32 index = map % 100;
        if (enabled != 0) {
            s32 byteOffset = slot * 30 + index * 1920 + 0x1450;
            u16 *flags = (u16 *)(D_00435DD0 + byteOffset);
            *flags |= 1 << bit;
        } else {
            s32 byteOffset = slot * 30 + index * 1920 + 0x1450;
            u16 *flags = (u16 *)(D_00435DD0 + byteOffset);
            *flags &= ~(1 << bit);
        }
    }
}

u8 func_00122FE0(s32 map, u32 slot, u32 bit) {
    s32 index;
    u8 *entry;
    u32 flags;

    if (map < 0x28) {
        index = map % 100;
        entry = (u8 *)(slot * 30 + index * 1920);
        entry += D_00435DD0;
        flags = *(u16 *)(entry + 0x1450);
        return (flags >> bit) & 1;
    }
    return 0;
}

void func_00123038(s32 map, s32 slot, s32 bit, s32 enabled) {
    if (map < 40) {
        s32 index = map % 100;
        if (enabled != 0) {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_00435DD0 + 0x1452);
            *flags |= 1 << bit;
        } else {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_00435DD0 + 0x1452);
            *flags &= ~(1 << bit);
        }
    }
}

u8 func_001230E0(s32 map, u32 slot, u32 bit) {
    s32 index;
    u8 *entry;
    u32 flags;

    if (map < 0x28) {
        index = map % 100;
        entry = (u8 *)(slot * 30 + index * 1920);
        entry += D_00435DD0;
        flags = *(u16 *)(entry + 0x1452);
        return (flags >> bit) & 1;
    }
    return 0;
}

void func_00123138(s32 map, s32 slot, s32 bit, s32 enabled) {
    if (map < 40) {
        s32 index = map % 100;
        if (enabled != 0) {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_00435DD0 + 0x1454);
            *flags |= 1 << bit;
        } else {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_00435DD0 + 0x1454);
            *flags &= ~(1 << bit);
        }
    }
}

u8 func_001231E0(s32 map, u32 slot, u32 bit) {
    s32 index;
    u8 *entry;
    u32 flags;

    if (map < 0x28) {
        index = map % 100;
        entry = (u8 *)(slot * 30 + index * 1920);
        entry += D_00435DD0;
        flags = *(u16 *)(entry + 0x1454);
        return (flags >> bit) & 1;
    }
    return 0;
}

void func_00123238(s32 map, s32 slot, s32 bit, s32 enabled) {
    if (map < 40) {
        s32 index = map % 100;
        if (enabled != 0) {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_00435DD0 + 0x1456);
            *flags |= 1 << bit;
        } else {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_00435DD0 + 0x1456);
            *flags &= ~(1 << bit);
        }
    }
}

u8 func_001232E0(s32 map, u32 slot, u32 bit) {
    s32 index;
    u8 *entry;
    u32 flags;

    if (map < 0x28) {
        index = map % 100;
        entry = (u8 *)(slot * 30 + index * 1920);
        entry += D_00435DD0;
        flags = *(u16 *)(entry + 0x1456);
        return (flags >> bit) & 1;
    }
    return 0;
}

void func_00123338(s32 map, s32 slot, s32 bit, s32 enabled) {
    if (map < 40) {
        s32 index = map % 100;
        if (enabled != 0) {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_00435DD0 + 0x1458);
            *flags |= 1 << bit;
        } else {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_00435DD0 + 0x1458);
            *flags &= ~(1 << bit);
        }
    }
}

u8 func_001233E0(s32 map, u32 slot, u32 bit) {
    s32 index;
    u8 *entry;
    u32 flags;

    if (map < 0x28) {
        index = map % 100;
        entry = (u8 *)(slot * 30 + index * 1920);
        entry += D_00435DD0;
        flags = *(u16 *)(entry + 0x1458);
        return (flags >> bit) & 1;
    }
    return 0;
}

void fldSetMapSlotByte(s32 map, u32 slot, s32 offset, s32 value) {
    if (map < 40) {
        s32 index = map % 100;
        s32 displacement = offset + index * 1920;
        u8 *entry = (u8 *)(slot * 30 + displacement);
        entry += D_00435DD0;
        entry[0x145A] = value;
    }
}

s32 fldGetMapSlotByte(s32 map, u32 slot, s32 offset) {
    u8 value = 0;
    if (map < 40) {
        s32 index = map % 100;
        s32 displacement = offset + index * 1920;
        u8 *entry = (u8 *)(slot * 30 + displacement);
        entry += D_00435DD0;
        value = entry[0x145A];
    }
    return value == 0xff ? -1 : value;
}

void func_001234E8(s32 map, s32 slot, s32 bit, s32 enabled) {
    if (map < 40) {
        s32 index = map % 100;
        if (enabled != 0) {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_00435DD0 + 0x146A);
            *flags |= 1 << bit;
        } else {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_00435DD0 + 0x146A);
            *flags &= ~(1 << bit);
        }
    }
}

u8 func_00123590(s32 map, u32 slot, u32 bit) {
    s32 index;
    u8 *entry;
    u32 flags;

    if (map < 0x28) {
        index = map % 100;
        entry = (u8 *)(slot * 30 + index * 1920);
        entry += D_00435DD0;
        flags = *(u16 *)(entry + 0x146A);
        return (flags >> bit) & 1;
    }
    return 0;
}

void func_001235E8(s32 map, s32 slot, s32 bit, s32 enabled) {
    if (map < 40) {
        s32 index = map % 100;
        if (enabled != 0) {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_00435DD0 + 0x146C);
            *flags |= 1 << bit;
        } else {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_00435DD0 + 0x146C);
            *flags &= ~(1 << bit);
        }
    }
}

u8 func_00123690(s32 map, u32 slot, u32 bit) {
    s32 mapIndex;
    u8 *flagsAddress;
    u32 flags;

    if (map < 0x28) {
        mapIndex = map % 100;
        flagsAddress = (u8 *)(slot * 30 + mapIndex * 1920);
        flagsAddress += D_00435DD0;
        flags = *(u16 *)(flagsAddress + 0x146C);
        return (flags >> bit) & 1;
    }
    return 0;
}

typedef struct FieldActivationRecord {
    s32 kind;
    s16 parameter;
    u8 pad06[0xA];
} FieldActivationRecord;

extern FieldActivationRecord D_003A8EB0[];
extern void fldActivateObjectById(s32);
extern void func_0011C6A0(s32);

#define FIELD_ACTIVATION_FLAGS_OFFSET 0x110D0

void fldActivateFlaggedObject(s32 flagIndex) {
    s32 byteOffset = (flagIndex >> 3) + FIELD_ACTIVATION_FLAGS_OFFSET;
    u8 *byte = (u8 *)(D_00435DD0 + byteOffset);
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
    return (*(u8 *)(((s32)arg0 >> 3) + D_00435DD0 + 0x110d0) >> (arg0 & 7)) & 1;
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

extern u32 D_003899F0[];

u32 func_001239A8(s32 x, s32 y) {
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

INCLUDE_ASM(const s32, "game/code_0011F208", func_00124110);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412CA8);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412CF0);

INCLUDE_ASM(const s32, "game/code_0011F208", func_001244A8);

extern f32 fldAngleDifference(f32, f32);

f32 func_00124720(f32 angle) {
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

f32 func_001249B8(f32 ax, f32 ay, f32 bx, f32 by) {
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
    iterator = func_00110CD8(iterator, 6);
    hasEntry = func_00110628(iterator);
    if (hasEntry == 0) {
        return;
    }
    func_001106B8(iterator);
    do {
        node = func_00110680(iterator);
        hasEntry = func_00114058(node);
        if (hasEntry == 4) {
            if (mode == 0) {
                func_00114048(node, 3);
            }
            else {
                func_00114048(node, 0);
            }
        }
        hasEntry = func_001106D8(iterator);
    } while (hasEntry != 0);
    dds3DestroyWorldIndexNode(iterator);
}

void func_00124CC8(void) {
    if (*(s16 *)(D_00435DD0 + 0xe) != 0) {
        func_00118598(1);
    } else {
        func_00118598(0);
    }
    if (fileLoadStateChanged() == 0) {
        func_002D0EB0();
    } else {
        func_002D0EC8();
    }
    mdlFlagSet(0xc0f);
    {
        s32 code;
        D_00389988[0x44 / 4] = 1;
        code = 0x259;
        D_00389988[0x4c / 4] = 0;
        fldSetDeferredFieldCommand(0x15, 0x259);
        func_001027D8(6, &code, 4, 0);
    }
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_00124D70);

extern void fldInitDisplayObjects();
extern void func_00144598();
extern void func_003412D8();
extern void func_0014BA60();
extern void func_001343E8();
extern void func_00145818();
extern void func_00122828();
extern u8 D_0038A6E0[];

void func_00124E28(void) {
    fldInitDisplayObjects();
    func_00124E80();
    func_00144598();
    func_003412D8(D_0038A6E0, 0x1E240);
    func_0014BA60();
    func_001343E8();
    func_00145818();
    func_00122828();
}

void func_00124E80(void) {
    u32 *buffer = D_0038A640;
    __asm__ volatile(
        ".set noreorder\n"
        "sqc2 vf0, 0(%0)\n"
        ".set reorder"
        : : "r"(buffer) : "memory");
    buffer += 4;
    __asm__ volatile(
        ".set noreorder\n"
        "sqc2 vf0, 0(%0)\n"
        ".set reorder"
        : : "r"(buffer) : "memory");
    D_00435F0C = 0;
    *func_00125F28() = 0;
}

void fldInitializeSequenceAndResetFlags(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name) {
    if (func_00101740(D_00412D50) != NULL) {
        if (D_00389770[4] == stage) {
            D_00389770[8] = 1;
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
    record->unk_90 = 0;
    D_00435F24 = 0;
    D_00387D60[0] = 0;
}

void fldInitializeAlternateSequence(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name) {
    if (func_00101740(D_00412D50) != NULL) {
        if (D_00389770[4] == stage) {
            D_00389770[8] = 1;
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
    record->unk_90 = 0;
}

void func_00125080(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name,
                   s32 code, s32 link, const char *subname) {
    if (func_00101740(D_00412D50) != NULL) {
        if (D_00389770[4] == stage) {
            D_00389770[8] = 1;
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
    record->unk_90 = 0;
}


void fldInitializeSequenceWithNote(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name,
                    s32 code, const char *subname) {
    if (func_00101740(D_00412D50) != NULL) {
        if (D_00389770[4] == stage) {
            D_00389770[8] = 1;
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
    record->unk_90 = 0;
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
    record->unk_90 = 0;
}

u32 func_00125328(void) {
    u32 *state = D_0038A640;

    if (state[0xD] == 1) {
        return 1;
    }
    if (state[0x17] == 3) {
        return 3;
    }
    if (state[0x17] == 4) {
        return 4;
    }
    if (state[0x17] == 5) {
        return 5;
    }
    if (state[0x17] == 6) {
        return 2;
    }
    return state[0xD] != 0 ? 2 : 0;
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_00125380);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412D50);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412D60);

INCLUDE_ASM(const s32, "game/code_0011F208", func_001258B8);

void fldUnloadPlayerModel(void) {
    if (D_00435F34 != 0) {
        func_003298C0(D_00435F34);
        D_00435F34 = 0;
        D_003898B8[0] = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_00125B10);

void fldPrepareResourceBuffer(void) {
    void *source;
    void *buffer;
    func_001258B8();
    D_00435F40 = D_00435F44;
    D_00435F38 = func_003292A8(D_00435F44);
    source = sdfMemoryGetBlockAddress(D_00435F34);
    buffer = sdfMemoryGetBlockAddress(D_00435F38);
    memcpy(buffer, source, D_00435F40);
    D_00435F3C = (u32)buffer;
    mdlLoadViewerPackage(2, 0, 0x101, buffer, D_00435F40);
    func_003298C0(D_00435F38);
    D_00435F38 = 0;
}

void fldReleaseResources(void) {
    if (D_00435F38 != 0) {
        func_003298C0(D_00435F38);
        D_00435F38 = 0;
    }
    if (D_00389790[0] == 0 && D_00435F78 == 0) {
        fldUnloadPlayerModel();
        fldSetPendingAreaAndFloor(0, 0);
    }
}

extern u32 D_00435F10;
extern u32 D_00435F14;
extern u32 D_00435F18;
extern void effObjFetchInnerFirstVec(u32);
extern void effObjFetchInnerSecondVecNorm(u32);

void func_00125E68(void) {
    u32 *buffer;
    if (D_00435F0C != 0) {
        if (dds3GetWorldSecondaryObject() != 0) {
            buffer = D_0038A640;
            effObjFetchInnerFirstVec(D_00435F0C);
            __asm__ volatile(
                ".set noreorder\n"
                "sqc2 vf10, 0(%0)\n"
                ".set reorder"
                : : "r"(buffer) : "memory");
            effObjFetchInnerSecondVecNorm(D_00435F0C);
            __asm__ volatile(
                ".set noreorder\n"
                "sqc2 vf10, 0(%0)\n"
                ".set reorder"
                : : "r"(buffer + 4) : "memory");
        }
        D_00435F0C = 0;
        D_00435F10 = 0;
        D_00435F14 = 0;
        D_00435F18 = 0;
        *func_00125F28() = 0;
        fldReleaseResources();
    }
}


void func_00125EE8(void) {
    fldResetTaskSlots();
    fldTestDrawDestroy();
    func_00125E68();
    evtDestroyWorldSecondaryNode();
    func_001284C8();
    func_00152C18();
}

u32 * func_00125F28(void) {
    return &D_00435F60;
}

u32 func_00125F38(void) {
    return *func_00125F28();
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_00125F58);

void func_00126000(void) {
    if (D_00435F0C != 0) {
        func_00128390(0x40);
        dds3InvokeSlot1Handler(D_00435F0C, 0);
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
    if (D_00435F0C == 0) {
        D_00435F0C = dds3SpawnCameraSlotObj5(dds3AdvanceWorldCounter(), pos, rot);
        dds3SetWorldEntryCallbackTarget((void *)D_00435F0C, D_00412EC0);
        func_00110C28(dds3GetWorldSecondaryObject(), D_00435F0C);
        if (D_00435F30 != 0) {
            dds3ClearObjectFlags(D_00435F0C, 0x20);
        }
        fldPrepareResourceBuffer();
        func_00112058(D_00435F0C, 2, D_0038A67C[0]);
    }
}

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412EB0);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412EC0);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412ED0);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412EE0);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412EF0);

INCLUDE_ASM(const s32, "game/code_0011F208", func_00126110);

INCLUDE_ASM(const s32, "game/code_0011F208", func_001265D0);

u32 func_001266D8(void) {
    return 0;
}

u32 func_001266E0(void) {
    return 100;
}

void func_001266E8(void) {
    if ((((FldWorkFlags *)D_00435DD0)->fieldFlags & 8) != 0) {
        func_00126730();
        D_00389770[3] |= 1;
    }
}

extern void mdlFlagClear(s32);
extern void evtClearSolarOverlayControl(void);

void func_00126730(void) {
    ((FldWorkFlags *)D_00435DD0)->fieldFlags &= ~8;
    D_00389770[3] &= ~1;
    mdlFlagClear(0x818);
    evtClearSolarOverlayControl();
}


extern void fldPlayMenuSound(s32);
extern void func_001360B8(s16, s32);

void func_00126778(void) {
    u32 value;
    if (D_00389770[0x4B] != 0) {
        if ((((FldWorkFlags *)D_00435DD0)->fieldFlags & 8) == 0) {
            fldPlayMenuSound(0x29);
        }
        mdlFlagSet(0x818);
        value = func_001266E0();
        ((s16 *)D_00389770)[0x94] = value;
        ((FldWorkFlags *)D_00435DD0)->fieldFlags |= 8;
        D_00389770[3] &= ~1;
        func_001360B8(value, 0);
    }
}


s32 fldGetSceneCommandState(void) {
    if ((((FldWorkFlags *)D_00435DD0)->fieldFlags & 8) != 0) {
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
    u8 pad10[0x118];
    s16 speed;               /* 0x128 */
    u8 pad12A[2];
    s32 commandActive;       /* 0x12C */
} FldSceneState;

void func_00126840(void) {
    FldSceneState *scene = (FldSceneState *)D_00389770;
    u32 value;
    if (scene->commandActive == 0) {
        if (scene->speed != 0) {
            scene->speed = 0;
            func_001360B8(0, 0);
        }
    } else if ((((FldWorkFlags *)D_00435DD0)->fieldFlags & 8) != 0) {
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


void func_00126910(void) {
    if ((((FldWorkFlags *)D_00435DD0)->fieldFlags & 4) != 0) {
        func_00126958();
        D_00389770[3] |= 2;
    }
}

extern void func_00243320(void);

void func_00126958(void) {
    ((FldWorkFlags *)D_00435DD0)->fieldFlags &= ~4;
    D_00389770[3] &= ~2;
    func_00243320();
}


void func_00126998(void) {
    ((FldWorkFlags *)D_00435DD0)->fieldFlags |= 4;
    D_00389770[3] &= ~2;
}

u8 func_001269C8(void) {
    s32 flags = ((FldWorkFlags *)D_00435DD0)->fieldFlags;
    flags &= 4;
    return flags != 0;
}

void func_001269E0(void) {
    if ((((FldWorkFlags *)D_00435DD0)->fieldFlags & 2) != 0) {
        func_00126A28();
        D_00389770[3] |= 4;
    }
}

void func_00126A28(void) {
    ((FldWorkFlags *)D_00435DD0)->fieldFlags &= ~2;
    D_00389770[3] &= ~4;
}

void func_00126A58(void) {
    ((FldWorkFlags *)D_00435DD0)->fieldFlags = (((FldWorkFlags *)D_00435DD0)->fieldFlags | 2) & ~1;
    D_00389770[3] &= ~4;
}

u8 func_00126A90(void) {
    s32 flags = ((FldWorkFlags *)D_00435DD0)->fieldFlags;
    flags &= 2;
    return flags != 0;
}

void func_00126AA8(void) {
    if ((((FldWorkFlags *)D_00435DD0)->fieldFlags & 1) != 0) {
        func_00126AF0();
        D_00389770[3] |= 8;
    }
}

void func_00126AF0(void) {
    ((FldWorkFlags *)D_00435DD0)->fieldFlags &= ~1;
    D_00389770[3] &= ~8;
}

void func_00126B20(void) {
    ((FldWorkFlags *)D_00435DD0)->fieldFlags = (((FldWorkFlags *)D_00435DD0)->fieldFlags | 1) & ~2;
    D_00389770[3] &= ~8;
}

u8 fldIsFlagActive(void) {
    s32 flags = ((FldWorkFlags *)D_00435DD0)->fieldFlags;
    flags &= 1;
    if (flags == 0) return 0;
    return 1;
}

void func_00126B70(void) {
    func_00331B38(D_003807F8, (s32)D_0037FF88, (s32)(D_0037FF88 + 0xC0), (s32)(D_0037FF88 + 0x100), (s32)(D_0037FF88 + 0xE0));
}

s16 fldRollEncounter(void) {
    s32 i;
    s32 enabled;

    for (i = 0; i < 16; i++) {
        if (D_00435E10[i].stage == D_00389770[4]) {
            enabled = 1;
            if (D_00435E10[i].flag != -1) {
                enabled = mdlFlagTest(D_00435E10[i].flag) != 0;
            }
            if (enabled != 0) {
                if ((u32)effMiscRand(0) % 100U < (u32)D_00435E10[i].chance) {
                    return D_00435E10[i].result;
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

    func_001512E8();
    D_00435F98[0] = 0;
    D_00435F98[1] = func_00151498();
    scene = D_00389770[5];
    area = D_00389770[4];
    D_00389770[0x3C] = area;
    D_00389770[0x3D] = scene + 1;
    fldResetTaskSlots();
    func_00128340(1);
    func_00128340(2);
    func_00126000();
    func_0023ACE8();
    return -1;
}

u8 func_00127320(void) {
    return func_0010C100(D_00412F30) != 0;
}

u8 func_00127348(void) {
    return func_0010C100(D_00412F58) != 0;
}

u8 func_00127370(void) {
    return func_0010C100(D_00412F40) != 0;
}

u8 func_00127398(void) {
    if (D_00435F7C > 0) {
        return 2;
    }
    if (mnuAcknowledgeCampState() != 0) {
        return 1;
    }
    return func_001283A8(0x20) != 0 ? 0 : 3;
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_001273E8);

u8 func_001275D0(void) {
    if (D_00435F80 > 0) {
        return 2;
    }
    return func_0014A250() != 0;
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_00127600);

s32 func_001277D8(void) {
    u32 *buffer = D_0038A640;
    u32 flags;

    if (D_00389770[64] == 1) {
        func_00150800();
    }
    D_00389770[64] = 0;
    if ((D_00435F24 & 2) && *(s8 *)D_00387D60 != 0) {
        func_00110F28(dds3GetWorldSecondaryObject(), D_00387D60);
        flags = D_00435F24;
        if (!(flags & 1)) {
            D_00387D60[0] = 0;
            D_00435F24 = flags & 0xFB;
        }
        return 1;
    }
    if (buffer[13] == 0 && *(s8 *)((u8 *)buffer + 0x80) != 0) {
        func_00110F28(dds3GetWorldSecondaryObject(), (u8 *)buffer + 0x80);
        return 1;
    }
    if (D_00389770[1] != 0) {
        func_00110F28(dds3GetWorldSecondaryObject(), (void *)D_00389770[1]);
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

void func_00128340(u32 mask) {
    u32 *flags = &D_00435F88;
    *flags |= mask;
}

void func_00128358(u32 mask) {
    u32 *flags = &D_00435F88;
    *flags &= ~mask;
}

u8 func_00128370(u32 mask) {
    return (D_00435F88 & mask) != 0;
}

void func_00128380(u32 mask) {
    D_00435F90 = D_00435F90 | mask;
}

void func_00128390(u32 mask) {
    D_00435F90 = D_00435F90 & ~mask;
}

u8 func_001283A8(u32 mask) {
    return (D_00435F90 & mask) != 0;
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_001283B8);

void func_001284C8(void) {
    if (D_00435F84 == 0) return;
    D_00435F84 = 0;
    kwlnFadeResetBackground();
    func_00128390(0x10);
    func_00128390(0x20);
    func_00128340(1);
    func_00128340(2);
    kwlnTaskDestroyWithHierarchyByName(D_00412FD0, 1);
    kwlnTaskDestroyWithHierarchyByName(D_00412FE0, 1);
    func_00144238();
    evtSetSolarOverlayFullyTransparent();
    fldDestroyTask();
    if (D_00435F74 != 0) {
        func_0018ECB8(D_00435F74);
        D_00435F74 = 0;
    }
    mnuDestroyCampTasks();
    func_0010BFE0();
    func_0014A228();
}

u8 func_00128580(void) {
    if (D_00389790[0] == 0 && fldIsAreaResourceReady() != 0) {
        fldPollAreaResourceLoad();
        if (fldGetResourceReadyFlag() == 1) return 0;
        fldFreeDisplayObjects();
        return 0;
    }
    return sdfGraphHasPendingWorkInterruptSafe() == 0;
}

void func_001285E8(void) {
    func_00141840();
}

void func_00128600(void) {
    func_00144EE0();
}

void func_00128618(void) {
    if (D_00435F68 != 0) {
        func_00128658();
        return;
    }
    func_00141898();
}

void fldSetDeferredFieldCommand(u32 arg0, u32 arg1) {
    D_00435F68 = arg0;
    D_00435F6C = arg1;
}

INCLUDE_ASM(const s32, "game/code_0011F208", func_00128658);

void func_001286C0(u32 command) {
    D_00435F70 = command;
}

void func_001286C8(void) {
    u32 command;

    command = D_00435F70;
    if (command != 0) {
        func_001411F8(command);
        D_00435F70 = 0;
    }
}

s32 func_001286F8(s32 *out0, s32 *out1) {
    if (D_00389770[0x3C] == -1 && D_00389770[0x3D] == D_00389770[0x3C]) {
        return 0;
    }
    *out0 = D_00389770[0x3C] + 0xC8;
    *out1 = D_00389770[0x3D];
    D_00389770[0x3C] = -1;
    D_00389770[0x3D] = -1;
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
    dds3SetWorldEntryCallbackTarget(matter, D_00412FF0);
    return matter;
}

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412FD0);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412FE0);

INCLUDE_RODATA(const s32, "game/code_0011F208", D_00412FF0);

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

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F0C);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F10);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F14);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F18);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F1C);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F20);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F24);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F28);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F30);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F34);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F38);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F3C);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F40);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F44);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F48);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F50);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F58);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F60);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F68);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F6C);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F70);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F74);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F78);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F7C);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F80);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F84);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F88);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F8C);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F90);

INCLUDE_SDATA(const s32, "game/code_0011F208", D_00435F98);

