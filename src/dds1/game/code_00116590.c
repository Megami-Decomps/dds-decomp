#include "common.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

extern f32 *D_00324770[];
extern u8 kwlnDefaultColorVector[];

typedef struct WorldUnitState {
    u8 pad00[8];
    u32 unit; /* 0x08: passed to the unit value transition helpers */
    u8 pad0C[0x58];
    u32 flags;           /* 0x64 */
    u32 colorA;          /* 0x68 packed from D_00324770 */
    u32 colorB;          /* 0x6C packed from kwlnDefaultColorVector */
    s16 unk70;           /* 0x70 */
    u8 pad72[2];
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

extern ActionObj *dds3AppendWorldObjectNode();

extern void dds3EnsureSlotData();

ActionObj *evtSpawnActionObj9(s32 value) {
    ActionObj *obj = dds3AppendWorldObjectNode(9);

    obj->unk4 = value;
    dds3EnsureSlotData(obj);
    return obj;
}

void evtReleaseActionWorldNode(void) {
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
        VU0_LOAD_VF(vf10, D_00324770[0]);
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

typedef struct WorldTransformParams {
    f32 rotation[4];    /* 0x00 */
    f32 position[3];    /* 0x10 */
    f32 scale[3];       /* 0x1C */
} WorldTransformParams;

typedef struct WorldTransformData {
    f32 position[3];    /* 0x00 */
    f32 positionW;      /* 0x0C */
    u8 pad10[0x30];
    f32 scale[3];       /* 0x40 */
    f32 scaleW;         /* 0x4C */
    f32 rotation[4];    /* 0x50 */
    u32 mode;           /* 0x60 */
    u32 flags;          /* 0x64 */
} WorldTransformData;

typedef struct WorldTransformSetup {
    u8 pad00[4];
    u32 flags;                      /* 0x04: bit 0 -> 1, bit 1 -> 4 in the object's flags */
    u32 mode;                       /* 0x08 */
    WorldTransformParams transform; /* 0x0C */
} WorldTransformSetup;

typedef struct WorldTransformOwner {
    u8 pad00[0x18];
    WorldTransformData *data;
} WorldTransformOwner;

/* Load flags, mode and the transform block from a setup record. */
void dds3LoadWorldTransformSetup(WorldTransformOwner *object, WorldTransformSetup *setup) {
    WorldTransformData *data = object->data;

    data->flags = 0;
    if (setup->flags & 1) {
        data->flags = 1;
    }
    if (setup->flags & 2) {
        data->flags |= 4;
    }
    data->rotation[0] = setup->transform.rotation[0];
    data->mode = setup->mode;
    data->rotation[1] = setup->transform.rotation[1];
    data->rotation[2] = setup->transform.rotation[2];
    data->rotation[3] = setup->transform.rotation[3];
    data->position[0] = setup->transform.position[0];
    data->position[1] = setup->transform.position[1];
    data->position[2] = setup->transform.position[2];
    data->positionW = 0.0f;
    data->scale[0] = setup->transform.scale[0];
    data->scale[1] = setup->transform.scale[1];
    data->scale[2] = setup->transform.scale[2];
    data->scaleW = 1.0f;
}

/* Load rotation, position and scale from a parameter block. */
void dds3LoadWorldTransformParams(WorldTransformOwner *object, WorldTransformParams *params) {
    WorldTransformData *data = object->data;

    data->rotation[0] = params->rotation[0];
    data->rotation[1] = params->rotation[1];
    data->rotation[2] = params->rotation[2];
    data->rotation[3] = params->rotation[3];
    data->position[0] = params->position[0];
    data->position[1] = params->position[1];
    data->position[2] = params->position[2];
    data->positionW = 0.0f;
    data->scale[0] = params->scale[0];
    data->scale[1] = params->scale[1];
    data->scale[2] = params->scale[2];
    data->scaleW = 1.0f;
}

u32 dds3AllocateUnitObjectWork(ObjWithWork *obj) {
    void *work = func_002CFEB8(0x1C);

    obj->work = work;
    memset(work, 0, 0x1C);
    return 1;
}
