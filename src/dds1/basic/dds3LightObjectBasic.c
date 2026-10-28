#include "sdf_vu0.h"
#include "sdk_ps2.h"
#include "eff_light.h"
#include "sdf_draw.h"
#include "sdf_chip.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"
#include "kwln_draw_light_binding.h"

void dds3FreePoseSet(dds3Basic_t *object);
void dds3ObjectBaseFreeControlWorkData(dds3ObjectBaseControlWorkData_t *object);
extern KwlnDrawLightSourceTable D_00324770;
extern f32 kwlnDefaultColorVector[4];


/* Releases the light's buffer and resource before freeing the object itself. */
void lightdestructor(dds3Basic_t *light) {
    EffLightData *data;

    data = light->data;
    sdfChipFreeDirect(data->buffer);
    dds3ObjectBaseFreeControlWorkData(data->baseControlWorkData);
    sdfChipFreeDirect(light->data);
    light->data = NULL;
    dds3FreePoseSet(light);
}

s32 lightmainprocedure(dds3Basic_t *lightObject) {
    SdfLightSources sources;
    EffLightData *data;
    u32 flags;

    memset(sources, 0, sizeof(sources));
    data = lightObject->data;
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
        SDF_VU0_STOREVECTOR(D_00324770[0]);
        SDF_VU0_STOREVECTOR_B(kwlnDefaultColorVector);
    }
    sources[0] = data->sourceVectors.vectors;
    sdf3DMakeLightParallelPacket(data->buffer, sources, data->ambientColor);
    return 1;
}
