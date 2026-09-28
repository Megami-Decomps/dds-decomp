#include "common.h"

extern u32 D_00438FA8;

extern u32 D_00438FAC;

void initializeEventVisualData(s32 arg0);

extern s8 D_00438FA4;

s32 func_00243358(s32 object);

INCLUDE_ASM(const s32, "game/code_00244F00", func_00244F00);

void initializeEventVisualData(s32 object) {
    s32 state = func_00243358(object);
    s16 *values = (s16 *)(object + 0x3C);
    *(u8 *)(object + 0xFC) = state;
    values[0] = 0x39;
    values[1] = 0x33;
    values[2] = 0x1D;
    values[3] = 0x23;
    values[5] = 5;
    values[7] = 10;
    values = (s16 *)(object + 0x9C);
    values[0] = 0x37;
    values[1] = 0x32;
    values[2] = 15;
    values[3] = 15;
    values[5] = 10;
    values[7] = 0;
}

INCLUDE_ASM(const s32, "game/code_00244F00", func_00245590);

INCLUDE_ASM(const s32, "game/code_00244F00", func_00245618);

s32 func_002457A8(void) {
    return D_00438FA4 != 0;
}

void func_002457B8(s32 arg0) {
    if (arg0 == 0) {
        D_00438FA4 = 0;
        D_00438FA8 = 0;
        D_00438FAC = 0;
        return;
    }
    D_00438FAC = (s32)arg0;
    D_00438FA4 = 3;
    D_00438FA8 = 0;
}

void func_002457E0(s32 arg0) {
    if (arg0 == 0) {
        D_00438FA4 = 5;
        D_00438FAC = 1;
        D_00438FA8 = 0;
    } else {
        D_00438FA8 = arg0;
        D_00438FA4 = 5;
        D_00438FAC = arg0;
    }
}

void func_00245810(void) {
}

u32 func_00245818(void) {
    return 0;
}

void func_00245820(void) {
    func_0010BFE0();
}

INCLUDE_ASM(const s32, "game/code_00244F00", startEventTestTask);

INCLUDE_ASM(const s32, "game/code_00244F00", stopEventTestTasks);

INCLUDE_ASM(const s32, "game/code_00244F00", func_002458B8);

INCLUDE_ASM(const s32, "game/code_00244F00", func_00245F80);

INCLUDE_ASM(const s32, "game/code_00244F00", func_00245F88);

void unlinkEventListNode(s32 owner, s32 node) {
    s32 next = *(s32 *)(node + 0x30);
    s32 previous = *(s32 *)(node + 0x34);
    if (previous == 0) {
        *(s32 *)(owner + 0x54) = next;
    } else {
        *(s32 *)(previous + 0x30) = next;
    }
    {
        s32 earlier = *(s32 *)(node + 0x34);
        s32 later = *(s32 *)(node + 0x30);
        if (later == 0) {
            *(s32 *)(owner + 0x58) = earlier;
        } else {
            *(s32 *)(later + 0x34) = earlier;
        }
    }
    {
        s32 count = *(s32 *)(owner + 0x50);
        *(s32 *)(node + 0x34) = 0;
        *(s32 *)(node + 0x30) = 0;
        *(s32 *)(owner + 0x50) = count - 1;
    }
}

void reorderEventListNodes(s32 owner) {
    if (owner != 0) {
        s32 current = *(s32 *)(owner + 0x54);
        while (current != 0) {
            s32 next = *(s32 *)(current + 0x30);
            s32 scan = next;
            while (scan != 0) {
                if (*(u16 *)scan < *(u16 *)current) {
                    unlinkEventListNode(owner, scan);
                    func_00245F88(owner, scan);
                    next = *(s32 *)(scan + 0x30);
                    break;
                }
                scan = *(s32 *)(scan + 0x30);
            }
            current = next;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00244F00", func_00246108);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_0043722C);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437230);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437234);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437238);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437240);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437248);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437250);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437258);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437260);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437268);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437270);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437278);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437280);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437288);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437290);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437298);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_004372A0);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_004372A8);

