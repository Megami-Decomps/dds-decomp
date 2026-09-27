#include "common.h"

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023B3A0);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023B480);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023C400);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023C4B0);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023C568);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023C5F0);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023C750);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023C7C0);

void func_0023C828(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x6c) = arg1;
    *(u32 *)(arg0 + 0xa8) = *(u32 *)(arg0 + 0xa8) | 0x20000;
}

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023C840);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023C870);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023C908);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023C978);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023CA60);

u8 func_0023CB48(s32 arg0) {
    return (*(u32 *)(arg0 + 0xa8) & 0x7800) != 0;
}

void func_0023CB58(s32 arg0) {
    *(u32 *)(arg0 + 0xa8) = *(u32 *)(arg0 + 0xa8) | 0x300;
}

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023CB68);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023CC00);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023CC48);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023CC60);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023CD98);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023CE08);

void func_0023CE10(s32 arg0, u16 arg1) {
    *(u16 *)(arg0 + 0xbc) = arg1;
}

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023CE18);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023CE20);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023CE30);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023CE98);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023CEA8);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023CED8);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023CF70);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023D030);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023D1F0);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023D298);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_004215D0);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_004215E0);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_004215F0);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023D360);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023D5B8);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023D640);
