#include "common.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"

typedef struct {
    f32 unk0[4];
    u8 pad10[0x30];
    f32 unk40[4];
    u8 pad50[0x14];
    s32 unk64;
    s32 unk68;
    s32 unk6C;
    u16 unk70;
    u16 unk72;
    s32 resource;
    void *buffer;
} LightData;

typedef struct {
    u8 pad[0x18];
    LightData *data;
} LightObject;

typedef struct LightSlotDesc {
    void *points;
    s32 unk4;
    s32 unk8;
} LightSlotDesc;

void effObjFreeInner(void *arg);
void func_00111840(s32 arg);
void func_002CFF98(void *arg);
void func_002E1938(void *arg0, LightSlotDesc *desc, f32 *color);
void *memset(void *s, s32 c, u32 n);
extern void *D_00324770[];
extern void *D_00324780[];

/* Releases the light's buffer and resource before freeing the object itself. */
void lightReleaseObject(LightObject *light) {
    LightData *data;

    data = light->data;
    func_002CFF98(data->buffer);
    func_00111840(data->resource);
    func_002CFF98(light->data);
    light->data = NULL;
    effObjFreeInner(light);
}

INCLUDE_ASM(const s32, "basic/dds3LightObjectBasic", func_00116388);