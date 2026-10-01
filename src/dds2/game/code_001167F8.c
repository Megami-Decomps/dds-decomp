#include "common.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

extern f32 *D_0037F770[];
extern u8 kwlnDefaultColorVector[];

typedef struct WorldUnitState {
    u8 pad00[8];
    u32 unit; /* 0x08: passed to the unit value transition helpers */
    u8 pad0C[0x58];
    u32 flags;
    u32 colorA;          /* 0x68 packed from D_0037F770 */
    u32 colorB;          /* 0x6C packed from kwlnDefaultColorVector */
    s16 unk70;           /* 0x70 */
    u8 pad72[2];
    u32 value74; /* Meaning unknown; exposed by func_00116800. */
} WorldUnitState;

typedef struct WorldUnitOwner {
    u8 pad00[0x18];
    WorldUnitState *state;
} WorldUnitOwner;

typedef struct ActionObj {
    u8 unk0[4];   /* 0x0 */
    s32 unk4;     /* 0x4 */
    u8 unk8[0x14]; /* 0x8 */
    s32 unk1C;    /* 0x1C */
} ActionObj;

extern ActionObj *dds3AppendWorldObjectNode();

extern void dds3EnsureSlotData();

extern void *func_00328D68(s32 size);

u32 func_001167F8(void) {
    return 1;
}

u32 func_00116800(WorldUnitOwner *object) {
    return object->state->value74;
}

ActionObj *evtSpawnActionObj9(s32 value) {
    ActionObj *obj = dds3AppendWorldObjectNode(9);

    obj->unk4 = value;
    dds3EnsureSlotData(obj);
    return obj;
}

void func_00116858(void) {
    dds3RemoveWorldObjectNode();
}

void evtBeginUnitValueColorTransition(WorldUnitOwner *object, s32 value) {
    WorldUnitState *state = object->state;
    s32 color1[4];
    s32 color2[4];
    u32 packed1;
    u32 packed2;

    state->flags |= 1;
    if (value == 0) {
        state->flags &= ~2;
    } else {
        state->flags |= 2;
        state->unk70 = value;
        VU0_LOAD_VF(vf10, D_0037F770[0]);
        EE_MMI_RGBA_PACK_F128(packed1);
        color1[0] = packed1;
        state->colorA = color1[0];
        VU0_LOAD_VF(vf10, kwlnDefaultColorVector);
        EE_MMI_RGBA_PACK(packed2);
        color2[0] = packed2;
        state->colorB = color2[0];
    }
}

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


s32 func_00116A88(WorldUnitOwner *obj) {
    obj->state = func_00328D68(0x1C);
    memset(obj->state, 0, 0x1C);
    return 1;
}
