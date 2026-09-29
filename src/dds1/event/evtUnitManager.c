#include "common.h"
#include "pcp_vu0.h"

/* Event unit: flag bits at 0xa8 drive status queries below. */
typedef struct EvtUnit {
    u8 pad[0x6c];      /* 0x0 */
    u32 value;         /* 0x6c: changed by evtSetUnitValueAndFlag */
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
    unit->value = value;
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

s32 func_002220D8(u8 *obj) {
    if (obj == NULL) {
        return 0;
    }
    return *(s32 *)(*(u8 **)(obj + 0x18) + 8);
}

INCLUDE_ASM(const s32, "event/evtUnitManager", func_002220F0);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00222200);

s32 func_00222270(u8 *work) {
    return *(s16 *)(work + 0xAC);
}

void func_00222278(EvtUnit *unit, u16 value)
{
    unit->unkBC = value;
}

void func_00222280(u8 *work, f32 value) {
    *(f32 *)(work + 0xB8) = value;
}

void func_00222288(u8 *work, s32 a, s32 b) {
    *(s16 *)(work + 0xBE) = a;
    *(s16 *)(work + 0xC0) = b;
}

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00222298);

void func_00222300(u8 *work, s32 a, s32 b) {
    *(s8 *)(work + 0xD0) = a;
    *(s8 *)(work + 0xD1) = b;
}

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

void func_00222AA8(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x70, src);
}
