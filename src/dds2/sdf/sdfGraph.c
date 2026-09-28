#include "common.h"

extern u8 D_0040B290[];

extern u8 D_004389D8;

extern u8 D_0043913C;

void func_00329CE0(void *arg0, s32 arg1, s32 arg2);

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

void func_0032AA40(void *arg0);

void *func_0032AB20(s32 arg0, s32 arg1, s32 arg2);

void func_00329D38(s32 arg0) {
    D_004389D8 = (u8)arg0;
    func_00329CE0(D_0040B290, arg0, 0);
    D_0043913C = 1;
}

void func_00329D70(SdfGraphObj *arg0) {
    func_0032AA40(arg0->unk8);
    arg0->unk8 = NULL;
    func_0032AA40(arg0->unkC);
    arg0->unkC = NULL;
    func_0032AA40(arg0->unk10);
    arg0->unk10 = NULL;
}

void func_00329DB8(SdfGraphObj *arg0) {
    s16 w;
    s16 h;
    u8 mode;

    func_00329D70(arg0);
    w = arg0->unk0;
    h = arg0->unk4;
    arg0->unk10 = func_0032AB20(w, h, arg0->unk7);
    mode = arg0->unk6;
    arg0->unk8 = func_0032AB20(w, h, mode);
    arg0->unkC = func_0032AB20(w, h, mode);
}

INCLUDE_ASM(const s32, "sdf/sdfGraph", func_00329E40);

INCLUDE_ASM(const s32, "sdf/sdfGraph", func_00329ED0);

INCLUDE_ASM(const s32, "sdf/sdfGraph", func_00329F30);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_004389D8);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_004389D9);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_004389DA);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_004389DC);

