#include "common.h"

extern void func_0024E260(s32, s32, s32, s32, s32, s32);

INCLUDE_ASM(const s32, "game/code_00253E58", func_00253E58);

void func_00254218(s32 a0, s32 a1, s32 a2, s32 a3, s32 t0) {
    s32 arr[4];
    s32 v;

    v = (s32)(((f32)(a3 << 4)) * 0.0078125f);
    v |= 0x0A050700;
    arr[0] = v;
    arr[1] = v;
    arr[2] = v;
    arr[3] = v;
    a1 += 0x32;
    a1 *= 8;
    func_002C0F88(a0 * 16, a1, a2, 0x2000, 0xC70, arr, t0);
}

INCLUDE_ASM(const s32, "game/code_00253E58", func_00254288);

void func_00254680(s32 p0, s32 a1, s32 a2) {
    func_00255E08();
    func_0024E260(0, 0, 0, a1, 0x5A, a2);
    itfDspDrawMarksA(a1, a2);
}

/* Three sprite layers drawn at the origin for one draw context. */
void func_002546D8(s32 scale, s32 context) {
    func_0024E260(0, 0, 0, scale, 0x4F, context);
    func_0024E260(0, 0, 0, scale, 0xC, context);
    func_0024E260(0, 0, 0, scale, 0xD, context);
}

void func_00254758(s32 x, s32 y, s32 z, s32 drawContext, s32 drawArgument) {
    func_0024E260(x, y, z, drawContext, 14, drawArgument);
}

INCLUDE_ASM(const s32, "game/code_00253E58", func_00254778);

INCLUDE_ASM(const s32, "game/code_00253E58", func_00254810);

INCLUDE_SDATA(const s32, "game/code_00253E58", D_003BC440);

