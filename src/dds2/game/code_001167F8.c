#include "common.h"

typedef struct WorldUnitState {
    u8 pad00[8];
    u32 unit; /* 0x08: passed to the unit value transition helpers */
    u8 pad0C[0x58];
    u32 flags;
    u8 pad68[0xC];
    u32 value74; /* Meaning unknown; exposed by func_00116800. */
} WorldUnitState;

typedef struct WorldUnitOwner {
    u8 pad00[0x18];
    WorldUnitState *state;
} WorldUnitOwner;

u32 func_001167F8(void) {
    return 1;
}

u32 func_00116800(WorldUnitOwner *object) {
    return object->state->value74;
}

INCLUDE_ASM(const s32, "game/code_001167F8", evtSpawnActionObj9);

void func_00116858(void) {
    func_00110B50();
}

INCLUDE_ASM(const s32, "game/code_001167F8", func_00116870);

void dds3ClearUnitObjectLowFlags(WorldUnitOwner *object) {
    object->state->flags = object->state->flags & 0xfffffffc;
}

void evtSetUnitValueTransitionForObject(u32 value, WorldUnitOwner *object) {
    evtSetUnitValueTransition(object->state->unit, value);
}

void evtEndUnitValueTransitionForObject(WorldUnitOwner *object) {
    evtEndUnitValueTransition(object->state->unit);
}

INCLUDE_ASM(const s32, "game/code_001167F8", func_00116978);

INCLUDE_ASM(const s32, "game/code_001167F8", func_00116A20);

INCLUDE_ASM(const s32, "game/code_001167F8", func_00116A88);
