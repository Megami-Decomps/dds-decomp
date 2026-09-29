#include "common.h"

typedef struct WorldUnitState {
    u8 pad00[8];
    u32 unit; /* 0x08: passed to the unit value transition helpers */
    u8 pad0C[0x58];
    u32 flags;
    u8 pad68[0xC];
    u32 value74; /* Meaning unknown; exposed by func_00116598. */
} WorldUnitState;

typedef struct WorldUnitOwner {
    u8 pad00[0x18];
    WorldUnitState *state;
} WorldUnitOwner;

extern void *func_002CFEB8(s32 size);

typedef struct ObjWithWork {
    u8 unk0[0x18];
    u32 *work;
} ObjWithWork;

u32 func_00116590(void) {
    return 1;
}

u32 func_00116598(WorldUnitOwner *object) {
    return object->state->value74;
}

typedef struct ActionObj {
    u8 unk0[4];   /* 0x0 */
    s32 unk4;     /* 0x4 */
    u8 unk8[0x14]; /* 0x8 */
    s32 unk1C;    /* 0x1C */
} ActionObj;

extern ActionObj *func_00110880();

extern void dds3EnsureSlotData();

ActionObj *evtSpawnActionObj9(s32 value) {
    ActionObj *obj = func_00110880(9);

    obj->unk4 = value;
    dds3EnsureSlotData(obj);
    return obj;
}

void func_001165F0(void) {
    func_00110928();
}

INCLUDE_ASM(const s32, "game/code_00116590", func_00116608);

void dds3ClearUnitObjectLowFlags(WorldUnitOwner *object) {
    object->state->flags = object->state->flags & 0xfffffffc;
}

void evtSetUnitValueTransitionForObject(u32 value, WorldUnitOwner *object) {
    evtSetUnitValueTransition(object->state->unit, value);
}

void evtEndUnitValueTransitionForObject(WorldUnitOwner *object) {
    evtEndUnitValueTransition(object->state->unit);
}

INCLUDE_ASM(const s32, "game/code_00116590", func_00116710);

INCLUDE_ASM(const s32, "game/code_00116590", func_001167B8);

INCLUDE_ASM(const s32, "game/code_00116590", func_00116820);
