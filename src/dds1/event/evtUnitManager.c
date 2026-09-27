#include "common.h"

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00220830);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00220910);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00221890);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00221940);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_002219F8);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00221A80);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00221BE0);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00221C50);

void func_00221CB8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x6c) = arg1;
    *(u32 *)(arg0 + 0xa8) = *(u32 *)(arg0 + 0xa8) | 0x20000;
}

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00221CD0);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00221D00);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00221D98);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00221E08);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00221EF0);

u8 func_00221FD8(s32 arg0) {
    return (*(u32 *)(arg0 + 0xa8) & 0x7800) != 0;
}

void func_00221FE8(s32 arg0) {
    *(u32 *)(arg0 + 0xa8) = *(u32 *)(arg0 + 0xa8) | 0x300;
}

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00221FF8);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00222090);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_002220D8);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_002220F0);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00222200);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00222270);

void func_00222278(s32 arg0, u16 arg1) {
    *(u16 *)(arg0 + 0xbc) = arg1;
}

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00222280);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00222288);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00222298);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00222300);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00222310);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00222340);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_002223D8);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00222498);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00222658);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00222700);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_002227C8);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00222A20);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00222AA8);
