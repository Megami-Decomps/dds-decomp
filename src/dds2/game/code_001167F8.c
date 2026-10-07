#include "common.h"
#include "eff_light.h"
#include "eff_object.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

extern f32 *D_0037F770[];
extern u8 kwlnDefaultColorVector[];

struct EvtUnit;
extern void evtSetUnitValueTransition(struct EvtUnit *unit, s32 value, s32 duration);
extern void evtEndUnitValueTransition(struct EvtUnit *unit, s32 duration);
extern void dds3RemoveWorldObjectNode(EffWorldNode *node);

extern EffWorldNode *dds3AppendWorldObjectNode();

extern void dds3EnsureSlotData();

extern void *sdfAllocSizeClassBlock(s32 size);

u32 func_001167F8(void) {
    return 1;
}

u32 func_00116800(EffWorldNode *object) {
    return (u32)((EffLightData *)object->data)->resourceState;
}

EffWorldNode *evtSpawnActionObj9(s32 value) {
    EffWorldNode *obj = dds3AppendWorldObjectNode(9);

    obj->key = value;
    dds3EnsureSlotData(obj);
    return obj;
}

void evtReleaseActionWorldNode(EffWorldNode *object) {
    dds3RemoveWorldObjectNode(object);
}

void evtBeginUnitValueColorTransition(EffWorldNode *object, s32 value) {
    EffLightData *state = object->data;
    s32 color1[4];
    s32 color2[4];
    u32 packed1;
    u32 packed2;

    state->flags |= 1;
    if (value == 0) {
        state->flags &= ~2;
    } else {
        state->flags |= 2;
        state->blendFrames = value;
        VU0_LOAD_VF(vf10, D_0037F770[0]);
        EE_MMI_RGBA_PACK_F128(packed1);
        color1[0] = packed1;
        state->packedColorA = color1[0];
        VU0_LOAD_VF(vf10, kwlnDefaultColorVector);
        EE_MMI_RGBA_PACK(packed2);
        color2[0] = packed2;
        state->packedColorB = color2[0];
    }
}

void dds3ClearUnitObjectLowFlags(EffWorldNode *object) {
    EffLightData *data = object->data;

    data->flags = data->flags & 0xfffffffc;
}

/* Kind-5 effect payloads retain transition work separately from light data. */
void evtSetUnitValueTransitionForObject(void *value, EffWorldNode *object, s32 duration) {
    EffectObjectData *data = object->data;

    evtSetUnitValueTransition((struct EvtUnit *)data->transitionWork, (s32)value, duration);
}

void evtEndUnitValueTransitionForObject(EffWorldNode *object, s32 duration) {
    EffectObjectData *data = object->data;

    evtEndUnitValueTransition((struct EvtUnit *)data->transitionWork, duration);
}


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


/* Load flags, mode and the transform block from a setup record. */
void dds3LoadWorldTransformSetup(EffWorldNode *object, WorldTransformSetup *setup) {
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
void dds3LoadWorldTransformParams(EffWorldNode *object, WorldTransformParams *params) {
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


s32 dds3AllocateUnitObjectWork(EffWorldNode *obj) {
    obj->data = sdfAllocSizeClassBlock(0x1C);
    memset(obj->data, 0, 0x1C);
    return 1;
}
