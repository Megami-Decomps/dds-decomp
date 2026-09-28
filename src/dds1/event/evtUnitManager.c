#include "common.h"

/* Event unit: flag bits at 0xa8 drive status queries below. */
typedef struct EvtUnit {
    u8 pad[0x6c];      /* 0x0 */
    u32 unk6C;         /* 0x6c */
    u8 pad2[0x38];     /* 0x70 */
    u32 flags;         /* 0xa8 */
    u8 pad3[0x10];     /* 0xac */
    u16 unkBC;         /* 0xbc */
} EvtUnit;

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00220830);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00220910);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00221890);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00221940);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_002219F8);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00221A80);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00221BE0);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00221C50);

void evtSetUnitValueAndFlag(EvtUnit *unit, u32 value)
{
    unit->unk6C = value;
    unit->flags = unit->flags | 0x20000;
}

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00221CD0);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00221D00);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00221D98);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00221E08);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00221EF0);

u8 evtTestUnitStatusFlags(EvtUnit *unit)
{
    return (unit->flags & 0x7800) != 0;
}

void evtSetUnitStatusFlags(EvtUnit *unit)
{
    unit->flags = unit->flags | 0x300;
}

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00221FF8);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00222090);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_002220D8);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_002220F0);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00222200);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00222270);

void func_00222278(EvtUnit *unit, u16 value)
{
    unit->unkBC = value;
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

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_003AC060);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_003AC070);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_003AC080);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_002227C8);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00222A20);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00222AA8);
