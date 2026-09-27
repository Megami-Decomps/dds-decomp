#include "common.h"

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
    s32 unk74;
    void *unk78;
} LightData;

typedef struct {
    u8 pad[0x18];
    LightData *unk18;
} LightObj;

void func_0010F5E8(void *arg);
void func_00111840(s32 arg);
void func_002CFF98(void *arg);
void func_002E1938(void *arg0, void *arg1);
void *memset(void *s, s32 c, u32 n);
extern void *D_00324770[];
extern void *D_00324780[];

void func_00116338(LightObj *arg) {
    LightData *data;

    data = arg->unk18;
    func_002CFF98(data->unk78);
    func_00111840(data->unk74);
    func_002CFF98(arg->unk18);
    arg->unk18 = NULL;
    func_0010F5E8(arg);
}

INCLUDE_ASM(const s32, "basic/dds3LightObjectBasic", func_00116388);
