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

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023B3A0);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023B480);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023C400);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023C4B0);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023C568);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023C5F0);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023C750);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023C7C0);

void evtUnitSetValueAndFlag(EventUnit *unit, u32 value) {
    unit->value6C = value;
    unit->flags = unit->flags | 0x20000;
}

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023C840);

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

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023CC00);

s32 func_0023CC48(EventUnit *unit) {
    if (unit == NULL) {
        return 0;
    }
    return unit->data->value08;
}

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023CC60);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023CD98);

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
