#include "sdf_vu0.h"
#include "sdk_ps2.h"
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

struct sdf3DLightParallelPacket_t;

typedef struct dds3LightObjectBasicWorkData_t
{
    sdf3DLight_t Light;
    sdfVector4_t Ambient;
    f32 InsideR;
    f32 OutsideR;
    f32 Soften;
    f32 Bias;
    u32 AnimeType;
    u32 Flag;
    u32 BaseLightRGB;
    u32 BaseAmbientRGB;
    u16 SwitchFrame;
    u16 Reserve1;
    dds3ObjectBaseControlWorkData_t *ObjectBaseControlWorkData;
    struct sdf3DLightParallelPacket_t *LightPacket;
} dds3LightObjectBasicWorkData_t;

void dds3FreePoseSet(dds3Basic_t *object);
void dds3ObjectBaseFreeControlWorkData(dds3ObjectBaseControlWorkData_t *object);
extern dds3LightSourceTable_t dds3LightParam;
extern f32 dds3AmbientColor[4];

u32 lightconstructor(dds3Basic_t *object) {
    dds3LightObjectBasicWorkData_t *obj;

    dds3InitPoseSet(object);
    obj = (dds3LightObjectBasicWorkData_t *)sdfChipAllocDirect(0x7C);
    object->workdata = obj;
    obj->ObjectBaseControlWorkData = dds3ObjectBaseInitControlWorkData(object);
    dds3ObjectBaseOnFlag(object, 0x62);
    obj->Flag = 0;
    obj->Light.color.x = 0.7f;
    obj->Light.color.y = 0.7f;
    obj->Light.color.z = 0.7f;
    obj->Light.v.y = 1.0f;
    obj->Light.v.z = 0.5f;
    obj->Ambient.x = 0.2f;
    obj->Ambient.y = 0.2f;
    obj->Ambient.z = 0.2f;
    obj->Ambient.w = 1.0f;
    obj->InsideR = 100.0f;
    obj->OutsideR = 800.0f;
    obj->Soften = 7.0f;
    obj->Light.color.w = 0.0f;
    obj->Light.v.x = 0.0f;
    obj->Light.v.w = 0.0f;
    obj->Bias = 0.0f;
    obj->LightPacket = sdfChipAllocDirect(0xE0);
    return 1;
}

/* Releases the light's buffer and resource before freeing the object itself. */
void lightdestructor(dds3Basic_t *light) {
    dds3LightObjectBasicWorkData_t *data;

    data = light->workdata;
    sdfChipFreeDirect(data->LightPacket);
    dds3ObjectBaseFreeControlWorkData(data->ObjectBaseControlWorkData);
    sdfChipFreeDirect(light->workdata);
    light->workdata = NULL;
    dds3FreePoseSet(light);
}

s32 lightmainprocedure(dds3Basic_t *lightObject) {
    sdf3DLight_t *sources[3];
    dds3LightObjectBasicWorkData_t *data;
    u32 flags;

    memset(sources, 0, sizeof(sources));
    data = lightObject->workdata;
    flags = data->Flag;
    if (flags & 1) {
        if (flags & 2) {
            f32 factor;
            /* Separate SDK workspaces for the two unpack/pack stages. */
            u32 unpackedA[4];
            u32 packedA[4];
            u32 unpackedB[4];
            u32 packedB[4];
            u32 packedColor;
            u32 secondaryWord;
            if (data->SwitchFrame == 0) {
                data->Flag = flags & ~2;
                factor = 1.0f;
            } else {
                factor = 1.0f / (f32)data->SwitchFrame;
                data->SwitchFrame--;
            }
            unpackedA[0] = data->BaseLightRGB;
            EE_MMI_RGBA_UNPACK(unpackedA, 1.0f / 128.0f);
            VU0_SCALE_VF(vf10, 1.0f - factor);
            SDF_VU0_LOADVECTOR_B(&data->Light.color);
            VU0_SCALE_VF(vf11, factor);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_COPYVECTOR_A_TO_C();
            EE_MMI_RGBA_PACK_UNIT(packedColor, 128.0f);
            /* Capture the second color before publishing the first. */
            secondaryWord = data->BaseAmbientRGB;
            packedA[0] = packedColor;
            data->BaseLightRGB = packedA[0];

            unpackedB[0] = secondaryWord;
            EE_MMI_RGBA_UNPACK(unpackedB, 1.0f / 128.0f);
            VU0_SCALE_VF(vf10, 1.0f - factor);
            SDF_VU0_LOADVECTOR_B(&data->Ambient);
            VU0_SCALE_VF(vf11, factor);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_COPYVECTOR_A_TO_B();
            EE_MMI_RGBA_PACK_UNIT(packedB[0], 128.0f);
            data->BaseAmbientRGB = packedB[0];
            SDF_VU0_COPYVECTOR_C_TO_A();
        } else {
            SDF_VU0_LOADVECTOR(&data->Light.color);
            SDF_VU0_LOADVECTOR_B(&data->Ambient);
        }
        SDF_VU0_STOREVECTOR(dds3LightParam[0]);
        SDF_VU0_STOREVECTOR_B(dds3AmbientColor);
    }
    sources[0] = &data->Light;
    sdf3DMakeLightParallelPacket(data->LightPacket, sources, &data->Ambient);
    return 1;
}

struct evtUnitHandle_t;

extern void evtUnlinkLightObjectBasic(struct evtUnitHandle_t *unit, s32 duration);

u32 lightdrawprocedure(void) {
    return 1;
}

dds3ObjectBaseControlWorkData_t *dds3LightObjectBasicGetControlWorkData(dds3Basic_t *object) {
    return ((dds3LightObjectBasicWorkData_t *)object->workdata)->ObjectBaseControlWorkData;
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
    dds3LightObjectBasicWorkData_t *lightData = lightNode->workdata;
    s32 color1[4];
    s32 color2[4];
    u32 packed1;
    u32 packed2;

    lightData->Flag |= 1;
    if (blendFrames == 0) {
        lightData->Flag &= ~2;
    } else {
        lightData->Flag |= 2;
        lightData->SwitchFrame = blendFrames;
        SDF_VU0_LOADVECTOR(dds3LightParam[0]);
        EE_MMI_RGBA_PACK_F128(packed1);
        color1[0] = packed1;
        lightData->BaseLightRGB = color1[0];
        SDF_VU0_LOADVECTOR(dds3AmbientColor);
        EE_MMI_RGBA_PACK(packed2);
        color2[0] = packed2;
        lightData->BaseAmbientRGB = color2[0];
    }
}

void dds3UnlinkSceneLight(dds3Basic_t *object) {
    dds3LightObjectBasicWorkData_t *data = object->workdata;

    data->Flag &= ~(3);
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
    dds3LightObjectBasicWorkData_t *work = worldNode->workdata;

    work->Flag = 0;
    if (resource->Flag & 1) {
        work->Flag = 1;
    }
    if (resource->Flag & 2) {
        work->Flag |= 4;
    }
    work->InsideR = resource->InsideR;
    work->AnimeType = resource->AnimeType;
    work->OutsideR = resource->OutsideR;
    work->Soften = resource->Soften;
    work->Bias = resource->Bias;
    work->Light.color.x = resource->DiffuseR;
    work->Light.color.y = resource->DiffuseG;
    work->Light.color.z = resource->DiffuseB;
    work->Light.color.w = 0.0f;
    work->Ambient.x = resource->AmbientR;
    work->Ambient.y = resource->AmbientG;
    work->Ambient.z = resource->AmbientB;
    work->Ambient.w = 1.0f;
}

/* Load rotation, position and scale from a parameter block. */
void dds3SetLightObjectPathResoruce(dds3Basic_t *worldNode, const WorldTransformParams *params) {
    dds3LightObjectBasicWorkData_t *work = worldNode->workdata;

    work->InsideR = params->rotation[0];
    work->OutsideR = params->rotation[1];
    work->Soften = params->rotation[2];
    work->Bias = params->rotation[3];
    work->Light.color.x = params->position[0];
    work->Light.color.y = params->position[1];
    work->Light.color.z = params->position[2];
    work->Light.color.w = 0.0f;
    work->Ambient.x = params->scale[0];
    work->Ambient.y = params->scale[1];
    work->Ambient.z = params->scale[2];
    work->Ambient.w = 1.0f;
}

s32 dds3AllocEventBasicWorkData(dds3Basic_t *obj) {
    obj->workdata = sdfChipAllocDirect(0x1C);
    memset(obj->workdata, 0, 0x1C);
    return 1;
}
