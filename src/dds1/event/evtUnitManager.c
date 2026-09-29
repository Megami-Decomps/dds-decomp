#include "common.h"
#include "pcp_vu0.h"

/* Event unit: flag bits at 0xa8 drive status queries below. */
typedef struct EvtUnit {
    u8 pad[0x6c];      /* 0x0 */
    u32 value;         /* 0x6c: changed by evtSetUnitValueAndFlag */
    u8 pad70[0x14];   /* 0x70 */
    s32 currentTransitionValue; /* 0x84 */
    s32 previousTransitionValue; /* 0x88 */
    u8 pad8C[0x1C];   /* 0x8c */
    u32 flags;         /* 0xa8 */
    u8 padAC[0x10];   /* 0xac */
    u16 unkBC;         /* 0xbc */
    u8 padBE[0x9E];
    s16 transitionElapsed; /* 0x15c */
    s16 transitionDuration; /* 0x15e */
} EvtUnit;

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00220830);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00220910);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00221890);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00221940);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_002219F8);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00221A80);

void evtSetUnitValueTransition(EvtUnit *unit, s32 value, s32 duration) {
    unit->flags |= 0x40000;
    unit->previousTransitionValue = unit->currentTransitionValue;
    unit->currentTransitionValue = value;
    if (duration == 0) {
        unit->previousTransitionValue = 0;
        unit->transitionElapsed = 0;
        unit->transitionDuration = 0;
        unit->flags &= ~0x180000;
    } else {
        unit->flags |= 0x80000;
        unit->flags &= ~0x100000;
        unit->transitionDuration = duration;
        unit->transitionElapsed = 0;
    }
}

void evtEndUnitValueTransition(EvtUnit *unit, s32 duration) {
    if (unit->currentTransitionValue != 0) {
        if (duration == 0) {
            unit->flags &= ~0x40000;
            unit->flags &= ~0x180000;
            unit->currentTransitionValue = 0;
        } else {
            unit->flags &= ~0x80000;
            unit->transitionDuration = duration;
            unit->flags |= 0x100000;
            unit->transitionElapsed = 0;
        }
    }
}

void evtSetUnitValueAndFlag(EvtUnit *unit, u32 value)
{
    unit->value = value;
    unit->flags = unit->flags | 0x20000;
}

void evtClearUnitValueChangeFlag(EvtUnit *unit) {
    unit->flags = unit->flags & ~0x20000;
    func_00221A80(unit);
}

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

s32 func_00222090(s32 id) {
    u8 *obj = (u8 *)func_00110A48(dds3GetWorldObject(), id, 5);

    if (obj != NULL) {
        return *(s32 *)(*(u8 **)(obj + 0x18) + 8);
    }
    return (s32)obj;
}

s32 evtUnitGetNestedValue(u8 *obj) {
    if (obj == NULL) {
        return 0;
    }
    return *(s32 *)(*(u8 **)(obj + 0x18) + 8);
}

INCLUDE_ASM(const s32, "event/evtUnitManager", func_002220F0);

s32 func_00222200(u8 *work) {
    s32 handle;

    if (work == NULL) {
        return 1;
    }
    handle = *(s32 *)(work + 0x68);
    *(s32 *)(*(u8 **)(*(u8 **)(work + 0x8C) + 0x18) + 0x80) = 0;
    if (handle != 0) {
        func_002CFF98(handle);
        *(s32 *)(work + 0x68) = 0;
    }
    if (*(s32 *)(work + 0xA0) != 0) {
        dds3FreePathObject(*(s32 *)(work + 0xA0));
        *(s32 *)(work + 0xA0) = 0;
    }
    func_002CFF98(work);
    return 1;
}

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

void func_00222310(u8 *work) {
    func_00222340(work, *(s8 *)(work + 0xD0), *(s8 *)(work + 0xD1), 0, 0, 2);
}

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
