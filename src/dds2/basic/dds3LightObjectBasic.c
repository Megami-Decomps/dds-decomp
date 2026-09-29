#include "common.h"

typedef struct {
    u8 pad0[0x74];
    u32 resource;
    void *buffer;
} LightData;

typedef struct {
    u8 pad0[0x18];
    LightData *data;
} LightObject;

/* Releases the light's buffer and resource before freeing the object itself. */
void lightReleaseObject(LightObject *light) {
    LightData *data;

    data = light->data;
    func_00328E48(data->buffer);
    func_00111A68(data->resource);
    func_00328E48(light->data);
    light->data = NULL;
    effObjFreeInner(light);
}

INCLUDE_ASM(const s32, "basic/dds3LightObjectBasic", func_001165F0);
