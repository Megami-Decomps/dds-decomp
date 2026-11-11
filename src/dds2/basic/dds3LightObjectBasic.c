#include "common.h"
#include "dds3obj.h"
#include "dds3_object_flags.h"
#include "eff_object.h"
#include "evt_unit.h"
#include "libsdf.h"
#include "ps2asm.h"
#include "sdf_chip.h"
#include "sdfFrame.h"

typedef struct
{
    sdfVector4_t *sources[4];
    sdfVector4_t ambient;
} dds3SceneLightParam_t;

extern dds3SceneLightParam_t dds3LightParam;
SYMBOL_AT(sdfVector4_t, dds3LightObjectAmbient, dds3LightParam, 0x10);


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

typedef struct dds3LightColorWork_t
{
    u32 word;
    u8 pad04[12];
} dds3LightColorWork_t;



s32 lightconstructor(dds3Basic_t *basic)
{
    dds3LightObjectBasicWorkData_t *work;

    dds3InitPoseSet(basic);
    work = sdfChipAllocDirect(sizeof(dds3LightObjectBasicWorkData_t));
    basic->workdata = work;
    work->ObjectBaseControlWorkData = dds3ObjectBaseInitControlWorkData(basic);
    dds3ObjectBaseOnFlag(basic, 0x62);
    work->Flag = 0;

    work->Light.color.x = 0.7f;
    work->Light.color.y = 0.7f;
    work->Light.color.z = 0.7f;
    work->Light.color.w = 0.0f;
    work->Light.v.x = 0.0f;
    work->Light.v.y = 1.0f;
    work->Light.v.z = 0.5f;
    work->Light.v.w = 0.0f;
    work->Ambient.x = 0.2f;
    work->Ambient.y = 0.2f;
    work->Ambient.z = 0.2f;
    work->Ambient.w = 1.0f;
    work->InsideR = 100.0f;
    work->OutsideR = 800.0f;
    work->Soften = 7.0f;
    work->Bias = 0.0f;

    work->LightPacket = sdfChipAllocDirect(224);
    return 1;
}

void lightdestructor(dds3Basic_t *basic)
{
    dds3LightObjectBasicWorkData_t *work = basic->workdata;

    sdfChipFreeDirect(work->LightPacket);
    dds3ObjectBaseFreeControlWorkData(work->ObjectBaseControlWorkData);
    sdfChipFreeDirect(basic->workdata);
    basic->workdata = NULL;
    dds3FreePoseSet(basic);
}


/* vu0 routine: blend the stored light and ambient colours toward the target */
s32 lightmainprocedure(dds3Basic_t *basic)
{
    sdf3DLight_t *lights[3];
    dds3LightColorWork_t work[4];
    dds3LightObjectBasicWorkData_t *data;
    u32 flag;
    f32 ratio;
    u32 lightColor;
    u32 ambientColor;
    u32 nextColor;

    memset(lights, 0, sizeof(lights));
    data = basic->workdata;
    flag = data->Flag;
    if ((flag & 1) != 0) {
        if ((flag & 2) != 0) {
            if (data->SwitchFrame == 0) {
                data->Flag = flag & ~2;
                ratio = 1.0f;
            } else {
                u16 frames = data->SwitchFrame;

                ratio = 1.0f / (f32)frames;
                data->SwitchFrame = frames - 1;
            }
            work[0].word = data->BaseLightRGB;
            MMI_RGBA_UNPACK(&work[0], 0.0078125f);
            SDF_VU0_SCALEVECTOR_BITS(1.0f - ratio);
            SDF_VU0_LOADVECTOR_B(&data->Light.color);
            SDF_VU0_SCALEVECTOR_B_BITS(ratio);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_COPYVECTOR_A_TO_C();
            MMI_RGBA_PACK_UNIT(lightColor, 128.0f);
            nextColor = data->BaseAmbientRGB;
            work[1].word = lightColor;
            data->BaseLightRGB = lightColor;
            work[2].word = nextColor;
            MMI_RGBA_UNPACK(&work[2], 0.0078125f);
            SDF_VU0_SCALEVECTOR_BITS(1.0f - ratio);
            SDF_VU0_LOADVECTOR_B(&data->Ambient);
            SDF_VU0_SCALEVECTOR_B_BITS(ratio);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_COPYVECTOR_A_TO_B();
            MMI_RGBA_PACK_UNIT(ambientColor, 128.0f);
            work[3].word = ambientColor;
            data->BaseAmbientRGB = ambientColor;
            SDF_VU0_COPYVECTOR_C_TO_A();
        } else {
            SDF_VU0_LOADVECTOR(&data->Light.color);
            SDF_VU0_LOADVECTOR_B(&data->Ambient);
        }
        SDF_VU0_STOREVECTOR(dds3LightParam.sources[0]);
        SDF_VU0_STOREVECTOR_B(&dds3LightObjectAmbient);
    }
    lights[0] = &data->Light;
    sdf3DMakeLightParallelPacket(data->LightPacket, lights, &data->Ambient);
    return 1;
}

s32 lightdrawprocedure(dds3Basic_t *basic)
{
    return 1;
}

dds3ObjectBaseControlWorkData_t *dds3LightObjectBasicGetControlWorkData(dds3Basic_t *basic)
{
    dds3LightObjectBasicWorkData_t *work = basic->workdata;

    return work->ObjectBaseControlWorkData;
}

dds3Basic_t *dds3AddLightObjectBasic(u32 serial)
{
    dds3Basic_t *basic = dds3AddSceneBasicData(EFF_WORLD_KIND_LIGHT);

    basic->key = serial;
    dds3ObjectBaseCreateMover(basic);
    return basic;
}

void dds3DeleteLightObjectBasic(dds3Basic_t *basic)
{
    dds3DeleteSceneBasicData(basic);
}

/* vu0 routine: pack the live scene light and ambient colours */
void dds3LinkSceneLight(dds3Basic_t *basic, s32 frame)
{
    dds3LightColorWork_t work[2];
    dds3LightObjectBasicWorkData_t *data = basic->workdata;
    u32 lightColor;
    u32 ambientColor;
    u32 nextColor;

    data->Flag |= 1;
    if (frame == 0) {
        data->Flag &= ~2;
    } else {
        data->Flag |= 2;
        data->SwitchFrame = SDF_FRAMES(frame);
        SDF_VU0_LOADVECTOR(dds3LightParam.sources[0]);
        MMI_RGBA_PACK_UNIT(lightColor, 128.0f);
        work[0].word = lightColor;
        data->BaseLightRGB = lightColor;
        SDF_VU0_LOADVECTOR(&dds3LightParam.ambient);
        MMI_RGBA_PACK_UNIT(ambientColor, 128.0f);
        work[1].word = ambientColor;
        data->BaseAmbientRGB = ambientColor;
    }
}

void dds3UnlinkSceneLight(dds3Basic_t *basic)
{
    dds3LightObjectBasicWorkData_t *work = basic->workdata;

    work->Flag &= ~3;
}

void dds3LinkLightObjectToUnitObject(dds3Basic_t *lightbasic, dds3Basic_t *unitbasic, s32 frame)
{
    EffectObjectData *work = unitbasic->workdata;

    evtLinkLightObjectBasic(work->transitionWork, lightbasic, frame);
}

void dds3UnlinkLightObjectFromUnitObject(dds3Basic_t *unitbasic, s32 frame)
{
    EffectObjectData *work = unitbasic->workdata;

    evtUnlinkLightObjectBasic(work->transitionWork, frame);
}

void dds3SetLightObjectResoruce(dds3Basic_t *basic, fldResourceLight_t *resource)
{
    dds3LightObjectBasicWorkData_t *work = basic->workdata;

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

void dds3SetLightObjectPathResoruce(dds3Basic_t *basic, const WorldTransformParams *resource)
{
    dds3LightObjectBasicWorkData_t *work = basic->workdata;

    work->InsideR = resource->rotation[0];
    work->OutsideR = resource->rotation[1];
    work->Soften = resource->rotation[2];
    work->Bias = resource->rotation[3];
    work->Light.color.x = resource->position[0];
    work->Light.color.y = resource->position[1];
    work->Light.color.z = resource->position[2];
    work->Light.color.w = 0.0f;
    work->Ambient.x = resource->scale[0];
    work->Ambient.y = resource->scale[1];
    work->Ambient.z = resource->scale[2];
    work->Ambient.w = 1.0f;
}

s32 dds3AllocEventBasicWorkData(dds3Basic_t *basic)
{
    basic->workdata = sdfChipAllocDirect(28);
    memset(basic->workdata, 0, 28);
    return 1;
}
