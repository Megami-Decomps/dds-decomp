#include "sdf_vu0.h"
#include "sdk_ps2.h"
#include "eff_light.h"
#include "sdf_draw.h"
#include "sdf_chip.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"
#include "dds3_draw_light_binding.h"
#include "common.h"
#include "dds3_object_flags.h"
#include "dds3obj.h"
#include "eff_event.h"
#include "evt_unit.h"
#include "effect_value_object.h"
#include "eff_object.h"

void dds3FreePoseSet(dds3Basic_t *object);
void dds3ObjectBaseFreeControlWorkData(dds3ObjectBaseControlWorkData_t *object);
extern dds3LightSourceTable_t dds3LightParam;
extern f32 dds3AmbientColor[4];

u32 lightconstructor(dds3Basic_t *object) {
    EffLightData *obj;

    dds3InitPoseSet(object);
    obj = (EffLightData *)sdfChipAllocDirect(0x7C);
    object->workdata = obj;
    obj->baseControlWorkData = dds3ObjectBaseInitControlWorkData(object);
    dds3ObjectBaseOnFlag(object, 0x62);
    obj->flags = 0;
    obj->sourceVectors.named.color[0] = 0.7f;
    obj->sourceVectors.named.color[1] = 0.7f;
    obj->sourceVectors.named.color[2] = 0.7f;
    obj->sourceVectors.named.direction[1] = 1.0f;
    obj->sourceVectors.named.direction[2] = 0.5f;
    obj->ambientColor[0] = 0.2f;
    obj->ambientColor[1] = 0.2f;
    obj->ambientColor[2] = 0.2f;
    obj->ambientColor[3] = 1.0f;
    obj->parameter50 = 100.0f;
    obj->parameter54 = 800.0f;
    obj->parameter58 = 7.0f;
    obj->sourceVectors.named.color[3] = 0.0f;
    obj->sourceVectors.named.direction[0] = 0.0f;
    obj->sourceVectors.named.direction[3] = 0.0f;
    obj->word5C = 0;
    obj->buffer = sdfChipAllocDirect(0xE0);
    return 1;
}

/* Releases the light's buffer and resource before freeing the object itself. */
void lightdestructor(dds3Basic_t *light) {
    EffLightData *data;

    data = light->workdata;
    sdfChipFreeDirect(data->buffer);
    dds3ObjectBaseFreeControlWorkData(data->baseControlWorkData);
    sdfChipFreeDirect(light->workdata);
    light->workdata = NULL;
    dds3FreePoseSet(light);
}

s32 lightmainprocedure(dds3Basic_t *lightObject) {
    sdf3DLight_t *sources[3];
    EffLightData *data;
    u32 flags;

    memset(sources, 0, sizeof(sources));
    data = lightObject->workdata;
    flags = data->flags;
    if (flags & EFF_LIGHT_FLAG_UPDATE_COLOR_VECTORS) {
        if (flags & EFF_LIGHT_FLAG_BLEND_COLOR_VECTORS) {
            f32 factor;
            /* Separate SDK workspaces for the two unpack/pack stages. */
            u32 unpackedA[4];
            u32 packedA[4];
            u32 unpackedB[4];
            u32 packedB[4];
            u32 packedColor;
            u32 secondaryWord;
            if (data->blendFrames == 0) {
                data->flags = flags & ~EFF_LIGHT_FLAG_BLEND_COLOR_VECTORS;
                factor = 1.0f;
            } else {
                factor = 1.0f / (f32)data->blendFrames;
                data->blendFrames--;
            }
            unpackedA[0] = data->packedColorA;
            EE_MMI_RGBA_UNPACK(unpackedA, 1.0f / 128.0f);
            VU0_SCALE_VF(vf10, 1.0f - factor);
            SDF_VU0_LOADVECTOR_B(data->sourceVectors.named.color);
            VU0_SCALE_VF(vf11, factor);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_COPYVECTOR_A_TO_C();
            EE_MMI_RGBA_PACK_UNIT(packedColor, 128.0f);
            /* Capture the second color before publishing the first. */
            secondaryWord = data->packedColorB;
            packedA[0] = packedColor;
            data->packedColorA = packedA[0];

            unpackedB[0] = secondaryWord;
            EE_MMI_RGBA_UNPACK(unpackedB, 1.0f / 128.0f);
            VU0_SCALE_VF(vf10, 1.0f - factor);
            SDF_VU0_LOADVECTOR_B(data->ambientColor);
            VU0_SCALE_VF(vf11, factor);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_COPYVECTOR_A_TO_B();
            EE_MMI_RGBA_PACK_UNIT(packedB[0], 128.0f);
            data->packedColorB = packedB[0];
            SDF_VU0_COPYVECTOR_C_TO_A();
        } else {
            SDF_VU0_LOADVECTOR(data->sourceVectors.named.color);
            SDF_VU0_LOADVECTOR_B(data->ambientColor);
        }
        SDF_VU0_STOREVECTOR(dds3LightParam[0]);
        SDF_VU0_STOREVECTOR_B(dds3AmbientColor);
    }
    sources[0] = (sdf3DLight_t *)data->sourceVectors.vectors;
    sdf3DMakeLightParallelPacket(data->buffer, sources, data->ambientColor);
    return 1;
}

struct evtUnitHandle_t;

extern void evtUnlinkLightObjectBasic(struct evtUnitHandle_t *unit, s32 duration);

u32 lightdrawprocedure(void) {
    return 1;
}

dds3ObjectBaseControlWorkData_t *dds3LightObjectBasicGetControlWorkData(dds3Basic_t *object) {
    return ((EffLightData *)object->workdata)->baseControlWorkData;
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
    EffLightData *lightData = lightNode->workdata;
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
        SDF_VU0_LOADVECTOR(dds3LightParam[0]);
        EE_MMI_RGBA_PACK_F128(packed1);
        color1[0] = packed1;
        lightData->packedColorA = color1[0];
        SDF_VU0_LOADVECTOR(dds3AmbientColor);
        EE_MMI_RGBA_PACK(packed2);
        color2[0] = packed2;
        lightData->packedColorB = color2[0];
    }
}

void dds3UnlinkSceneLight(dds3Basic_t *object) {
    EffLightData *data = object->workdata;

    data->flags &= ~(EFF_LIGHT_FLAG_UPDATE_COLOR_VECTORS | EFF_LIGHT_FLAG_BLEND_COLOR_VECTORS);
}

/* Kind-5 effect payloads retain transition work separately from light data. */
void dds3LinkLightObjectToUnitObject(EffectValueObject *value, dds3Basic_t *effectNode, s32 duration) {
    EffectObjectData *effectData = effectNode->workdata;

    evtLinkLightObjectBasic(effectData->transitionWork, (dds3Basic_t *)value, duration);
}

void dds3UnlinkLightObjectFromUnitObject(dds3Basic_t *effectNode, s32 duration) {
    EffectObjectData *effectData = effectNode->workdata;

    evtUnlinkLightObjectBasic((struct evtUnitHandle_t *)effectData->transitionWork, duration);
}
/* Load flags, mode and the transform block from a setup record. */
void dds3SetLightObjectResoruce(dds3Basic_t *worldNode, fldResourceLight_t *resource) {
    WorldTransformData *transformData = worldNode->workdata;

    transformData->flags = 0;
    if (resource->Flag & 1) {
        transformData->flags = 1;
    }
    if (resource->Flag & 2) {
        transformData->flags |= 4;
    }
    transformData->rotation[0] = resource->InsideR;
    transformData->mode = resource->AnimeType;
    transformData->rotation[1] = resource->OutsideR;
    transformData->rotation[2] = resource->Soften;
    transformData->rotation[3] = resource->Bias;
    transformData->position[0] = resource->DiffuseR;
    transformData->position[1] = resource->DiffuseG;
    transformData->position[2] = resource->DiffuseB;
    transformData->positionW = 0.0f;
    transformData->scale[0] = resource->AmbientR;
    transformData->scale[1] = resource->AmbientG;
    transformData->scale[2] = resource->AmbientB;
    transformData->scaleW = 1.0f;
}

/* Load rotation, position and scale from a parameter block. */
void dds3SetLightObjectPathResoruce(dds3Basic_t *worldNode, const WorldTransformParams *params) {
    WorldTransformData *transformData = worldNode->workdata;

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

s32 dds3AllocEventBasicWorkData(dds3Basic_t *obj) {
    obj->workdata = sdfChipAllocDirect(0x1C);
    memset(obj->workdata, 0, 0x1C);
    return 1;
}
