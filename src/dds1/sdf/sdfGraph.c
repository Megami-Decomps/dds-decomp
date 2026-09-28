#include "common.h"

typedef struct SdfGraphObj {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    u8 unk6;
    u8 unk7;
    void *unk8;
    void *unkC;
    void *unk10;
} SdfGraphObj;

extern u8 D_003980E0[];
extern u8 D_003BD2E8;
extern u8 D_003BD9DC;

void func_002D0E30(void *arg0, s32 arg1, s32 arg2);
void func_002D1B90(void *arg0);
void *func_002D1C70(s32 arg0, s32 arg1, s32 arg2);

void func_002D0E88(s32 arg0) {
    D_003BD2E8 = (u8)arg0;
    func_002D0E30(D_003980E0, arg0, 0);
    D_003BD9DC = 1;
}

void func_002D0EC0(SdfGraphObj *arg0) {
    func_002D1B90(arg0->unk8);
    arg0->unk8 = NULL;
    func_002D1B90(arg0->unkC);
    arg0->unkC = NULL;
    func_002D1B90(arg0->unk10);
    arg0->unk10 = NULL;
}

void func_002D0F08(SdfGraphObj *arg0) {
    s16 w;
    s16 h;
    u8 mode;

    func_002D0EC0(arg0);
    w = arg0->unk0;
    h = arg0->unk4;
    arg0->unk10 = func_002D1C70(w, h, arg0->unk7);
    mode = arg0->unk6;
    arg0->unk8 = func_002D1C70(w, h, mode);
    arg0->unkC = func_002D1C70(w, h, mode);
}

INCLUDE_ASM(const s32, "sdf/sdfGraph", func_002D0F90);

INCLUDE_ASM(const s32, "sdf/sdfGraph", func_002D1020);


INCLUDE_ASM(const s32, "sdf/sdfGraph", func_002D1080);



INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_003BD2E8);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_003BD2E9);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_003BD2EA);


INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_003BD2EC);

