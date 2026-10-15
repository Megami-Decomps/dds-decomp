#include "eff_light.h"
#include "sdf_draw.h"
#include "sdf_chip.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"

void effObjFreeInner(EffWorldNode *object);
void dds3DestroyObjectBase(ObjBase *object);
void *memset(void *s, s32 c, u32 n);
extern f32 (*D_00324770[4])[4];
extern f32 kwlnDefaultColorVector[4];


/* Releases the light's buffer and resource before freeing the object itself. */
void lightReleaseObject(EffWorldNode *light) {
    EffLightData *data;

    data = light->data;
    sdfReleaseChipBlock(data->buffer);
    dds3DestroyObjectBase(data->resourceState);
    sdfReleaseChipBlock(light->data);
    light->data = NULL;
    effObjFreeInner(light);
}

s32 lightBlendAmbientColors(EffWorldNode *object) {
    SdfLightSources sources;
    EffLightData *data;
    u32 flags;

    memset(sources, 0, sizeof(sources));
    data = object->data;
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
            VU0_LOAD_VF(vf11, data->ambientColor);
            VU0_SCALE_VF(vf11, factor);
            VU0_ADD(vf10, vf10, vf11);
            VU0_MOVE_VF(vf12, vf10);
            EE_MMI_RGBA_PACK_UNIT(packedColor, 128.0f);
            /* Capture the second color before publishing the first. */
            secondaryWord = data->packedColorB;
            packedA[0] = packedColor;
            data->packedColorA = packedA[0];

            unpackedB[0] = secondaryWord;
            EE_MMI_RGBA_UNPACK(unpackedB, 1.0f / 128.0f);
            VU0_SCALE_VF(vf10, 1.0f - factor);
            VU0_LOAD_VF(vf11, data->secondaryColor);
            VU0_SCALE_VF(vf11, factor);
            VU0_ADD(vf10, vf10, vf11);
            VU0_MOVE_VF(vf11, vf10);
            EE_MMI_RGBA_PACK_UNIT(packedB[0], 128.0f);
            data->packedColorB = packedB[0];
            VU0_MOVE_VF(vf10, vf12);
        } else {
            VU0_LOAD_VF(vf10, data->ambientColor);
            VU0_LOAD_VF(vf11, data->secondaryColor);
        }
        VU0_STORE_VF(vf10, D_00324770[0]);
        VU0_STORE_VF(vf11, kwlnDefaultColorVector);
    }
    sources[0] = (f32 (*)[4])data;
    sdfBuildLightingPacket(data->buffer, sources, data->secondaryColor);
    return 1;
}
