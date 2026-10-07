#include "eff_light.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"

void effObjFreeInner(EffWorldNode *object);
void dds3DestroyObjectBase(ObjBase *object);
void sdfReleaseChipBlock(void *arg);
void *memset(void *s, s32 c, u32 n);
extern void *D_0037F770[];
extern void *kwlnDefaultColorVector[];

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

INCLUDE_ASM(const s32, "basic/dds3LightObjectBasic", lightBlendAmbientColors);