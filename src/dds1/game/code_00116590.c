#include "sdk_ps2.h"
#include "common.h"
#include "evt_unit.h"
#include "effect_value_object.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "eff_light.h"
#include "eff_object.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

extern f32 *D_00324770[];
extern u8 kwlnDefaultColorVector[];

struct evtUnitHandle_t;

extern void evtEndUnitValueTransition(struct evtUnitHandle_t *unit, s32 duration);



u32 lightdrawprocedure(void) {
    return 1;
}

dds3ObjectBaseControlWorkData_t *dds3LightObjectBasicGetControlWorkData(dds3Basic_t *object) {
    return ((EffLightData *)object->data)->resourceState;
}



dds3Basic_t *dds3AddLightObjectBasic(s32 key) {
    dds3Basic_t *obj = dds3AddSceneBasicData(EFF_WORLD_KIND_LIGHT);

    obj->key = key;
    dds3ObjectBaseCreateMover(obj);
    return obj;
}

void dds3DeleteLightObjectBasic(dds3Basic_t *object) {
    dds3DeleteSceneBasicData(object);
}

void dds3LinkSceneLight(dds3Basic_t *lightNode, s32 blendFrames) {
    EffLightData *lightData = lightNode->data;
    s32 color1[4];
    s32 color2[4];
    u32 packed1;
    u32 packed2;

    lightData->flags |= EFF_LIGHT_FLAG_UPDATE_COLOR_VECTORS;
    if (blendFrames == 0) {
        lightData->flags &= ~EFF_LIGHT_FLAG_BLEND_COLOR_VECTORS;
    } else {
        lightData->flags |= EFF_LIGHT_FLAG_BLEND_COLOR_VECTORS;
        lightData->blendFrames = blendFrames;
        VU0_LOAD_VF(vf10, D_00324770[0]);
        EE_MMI_RGBA_PACK_F128(packed1);
        color1[0] = packed1;
        lightData->packedColorA = color1[0];
        VU0_LOAD_VF(vf10, kwlnDefaultColorVector);
        EE_MMI_RGBA_PACK(packed2);
        color2[0] = packed2;
        lightData->packedColorB = color2[0];
    }
}

void dds3UnlinkSceneLight(dds3Basic_t *object) {
    EffLightData *data = object->data;

    data->flags &= ~(EFF_LIGHT_FLAG_UPDATE_COLOR_VECTORS | EFF_LIGHT_FLAG_BLEND_COLOR_VECTORS);
}

/* Kind-5 effect payloads retain transition work separately from light data. */
void dds3LinkLightObjectToUnitObject(EffectValueObject *value, dds3Basic_t *effectNode, s32 duration) {
    EffectObjectData *effectData = effectNode->data;

    evtSetUnitValueTransition(effectData->transitionWork, value, duration);
}

void dds3UnlinkLightObjectFromUnitObject(dds3Basic_t *effectNode, s32 duration) {
    EffectObjectData *effectData = effectNode->data;

    evtEndUnitValueTransition((struct evtUnitHandle_t *)effectData->transitionWork, duration);
}
/* Load flags, mode and the transform block from a setup record. */
void dds3SetLightObjectResoruce(dds3Basic_t *worldNode, WorldTransformSetup *setup) {
    WorldTransformData *transformData = worldNode->data;

    transformData->flags = 0;
    if (setup->flags & 1) {
        transformData->flags = 1;
    }
    if (setup->flags & 2) {
        transformData->flags |= 4;
    }
    transformData->rotation[0] = setup->transform.rotation[0];
    transformData->mode = setup->mode;
    transformData->rotation[1] = setup->transform.rotation[1];
    transformData->rotation[2] = setup->transform.rotation[2];
    transformData->rotation[3] = setup->transform.rotation[3];
    transformData->position[0] = setup->transform.position[0];
    transformData->position[1] = setup->transform.position[1];
    transformData->position[2] = setup->transform.position[2];
    transformData->positionW = 0.0f;
    transformData->scale[0] = setup->transform.scale[0];
    transformData->scale[1] = setup->transform.scale[1];
    transformData->scale[2] = setup->transform.scale[2];
    transformData->scaleW = 1.0f;
}

/* Load rotation, position and scale from a parameter block. */
void dds3SetLightObjectPathResoruce(dds3Basic_t *worldNode, const WorldTransformParams *params) {
    WorldTransformData *transformData = worldNode->data;

    transformData->rotation[0] = params->rotation[0];
    transformData->rotation[1] = params->rotation[1];
    transformData->rotation[2] = params->rotation[2];
    transformData->rotation[3] = params->rotation[3];
    transformData->position[0] = params->position[0];
    transformData->position[1] = params->position[1];
    transformData->position[2] = params->position[2];
    transformData->positionW = 0.0f;
    transformData->scale[0] = params->scale[0];
    transformData->scale[1] = params->scale[1];
    transformData->scale[2] = params->scale[2];
    transformData->scaleW = 1.0f;
}


u32 dds3AllocEventBasicWorkData(dds3Basic_t *obj) {
    void *work = sdfChipAllocDirect(0x1C);

    obj->data = work;
    memset(work, 0, 0x1C);
    return 1;
}
