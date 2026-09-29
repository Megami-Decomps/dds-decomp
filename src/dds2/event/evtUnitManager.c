#include "common.h"

typedef struct EventUnitData {
    u8 pad00[8];
    s32 value08;
} EventUnitData;

typedef struct EventUnit {
    u8 pad0[0x18];
    EventUnitData *data;
    u8 pad1C[0x50];
    u32 value6C;
    u8 pad70[0x38];
    u32 flags;
    s16 valueAC;
    u8 padAE[0xA];
    f32 valueB8;
    u16 valueBC;
    s16 valueBE;
    s16 valueC0;
    u8 padC2[0xE];
    s8 valueD0;
    s8 valueD1;
} EventUnit;

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

typedef struct PcpScatterWork4 PcpScatterWork4;

/* func_001730D0 */
struct PcpScatterWork4 {
    u8 pad00[0x40];
    s128 unk40;
    u8 pad50[0x12C];
    f32 unk17C;
    u32 unk180;
    u32 scatterObject;
    u32 ownedBuffer;
};

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023B3A0);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023B480);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023C400);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023C4B0);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023C568);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023C5F0);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitValueTransition);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtEndUnitValueTransition);

void evtUnitSetValueAndFlag(EventUnit *unit, u32 value) {
    unit->value6C = value;
    unit->flags = unit->flags | 0x20000;
}

void evtClearUnitValueChangeFlag(EvtUnit *unit) {
    unit->flags = unit->flags & ~0x20000;
    func_0023C5F0(unit);
}

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023C870);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023C908);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023C978);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023CA60);

u8 evtUnitHasStateBits(EventUnit *unit) {
    return (unit->flags & 0x7800) != 0;
}

void evtUnitSetStateBits(EventUnit *unit) {
    unit->flags = unit->flags | 0x300;
}

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023CB68);

s32 func_0023CC00(s32 id) {
    u8 *obj = (u8 *)func_00110C70(dds3GetWorldObject(), id, 5);

    if (obj != NULL) {
        return *(s32 *)(*(u8 **)(obj + 0x18) + 8);
    }
    return (s32)obj;
}

s32 evtUnitGetNestedValue(EventUnit *unit) {
    if (unit == NULL) {
        return 0;
    }
    return unit->data->value08;
}

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023CC60);

s32 func_0023CD98(u8 *work) {
    s32 handle;

    if (work == NULL) {
        return 1;
    }
    handle = *(s32 *)(work + 0x68);
    *(s32 *)(*(u8 **)(*(u8 **)(work + 0x8C) + 0x18) + 0x80) = 0;
    if (handle != 0) {
        func_00328E48(handle);
        *(s32 *)(work + 0x68) = 0;
    }
    if (*(s32 *)(work + 0xA0) != 0) {
        dds3FreePathObject(*(s32 *)(work + 0xA0));
        *(s32 *)(work + 0xA0) = 0;
    }
    func_00328E48(work);
    return 1;
}

s32 func_0023CE08(EventUnit *unit) {
    return unit->valueAC;
}

void func_0023CE10(EventUnit *unit, u16 value) {
    unit->valueBC = value;
}

void func_0023CE18(EventUnit *unit, f32 value) {
    unit->valueB8 = value;
}

void func_0023CE20(EventUnit *unit, s32 a, s32 b) {
    unit->valueBE = a;
    unit->valueC0 = b;
}

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023CE30);

void func_0023CE98(EventUnit *unit, s32 first, s32 second) {
    unit->valueD0 = first;
    unit->valueD1 = second;
}

void func_0023CEA8(u8 *work) {
    func_0023CED8(work, *(s8 *)(work + 0xD0), *(s8 *)(work + 0xD1), 0, 0, 2);
}

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
