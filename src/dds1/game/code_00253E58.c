#include "common.h"

extern void func_0024E260(s32, s32, s32, s32, s32, s32);
extern s32 func_002CB3B8(u32, u32);
extern u32 D_003BC4CC;
extern u32 *mnuGetSelectedNodeValue(void);
extern s32 func_00255E08();
extern void func_0024E5A0(s32, s32, s32, s32, s32, s32, f32, f32);
extern void func_0025D2F8(s32, s32, s32, s32, s32, s32);
extern void mnuDrawMantraCostAfterListAdvance(s32, s32, s32, s32);
extern void mnuChooseDisplaySpriteKindFromEntryFlags(s32, s32, s32);
extern f32 sdfSinPoly(f32 angle);
extern void func_00254758(s32, s32, s32, s32, s32);

INCLUDE_ASM(const s32, "game/code_00253E58", func_00253E58);

void mnuDrawPanelWithPackedColorPattern(s32 a0, s32 a1, s32 a2, s32 a3, s32 t0) {
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

/* Fill-level dispatch: direct, inverted, or sin-pulsed, then the shared layers. */
s32 func_00254288(void) {
    s32 s16;
    s32 s17;
    s32 w;
    s32 i;
    s32 r;
    f32 f;
    u32 *v;

    f = 0.0f;
    mnuGetSelectedNodeValue();
    s16 = func_002CB3B8(D_003BC4CC, -1);
    s17 = func_002CB3B8(D_003BC4CC, 1);
    func_00255E08(s16, 0x80, 0x52);
    w = *(s32 *)(s16 + 0x1C);
    if (w == 1) {
        goto case1;
    }
    if (w >= 2) {
        goto ge2;
    }
    if (w != 0) {
        goto tail;
    }
    goto case0;
ge2:
    if (w != 2) {
        goto tail;
    }
    goto case2;
case0:
    f = (f32)*(s32 *)(s16 + 0x20) / 10.0f;
    i = (s32)(f * 128.0f);
    func_0024E5A0(0, 0, 0, i, 0x3A, 0x53, 1.5f, 1.5f);
    mnuDrawPanelWithPackedColorPattern(0, 0, 0, i, 0x53);
    func_0025D2F8(0, 0, 1, i, s17, 0x53);
    func_00254758(0, 0, 1, 0x80, 0x53);
    func_0024E260(0, 0, 0, 0x80, 0xF, 0x53);
    func_0024E260(0, 0, 0, i, 0x5B, 0x53);
    v = mnuGetSelectedNodeValue();
    mnuDrawMantraCostAfterListAdvance(*v, s17 + 0x590, i, 0x53);
    func_0024E260(0x20, 0, 0, i, 0xB, 0x53);
    goto tail;
case2:
    f = (f32)*(s32 *)(s16 + 0x20) / 10.0f;
    f = 1.0f - f;
    i = (s32)(f * 128.0f);
    func_0024E5A0(0, 0, 0, i, 0x3A, 0x53, 1.5f, 1.5f);
    mnuDrawPanelWithPackedColorPattern(0, 0, 0, i, 0x53);
    func_0025D2F8(0, 0, 1, i, s17, 0x53);
    func_00254758(0, 0, 0, 0x80, 0x53);
    func_0024E260(0, 0, 0, 0x80, 0xF, 0x53);
    func_0024E260(0, 0, 0, i, 0x5B, 0x53);
    v = mnuGetSelectedNodeValue();
    mnuDrawMantraCostAfterListAdvance(*v, s17 + 0x590, i, 0x53);
    func_0024E260(0x20, 0, 0, i, 0xB, 0x53);
    goto tail;
case1:
    r = *(s32 *)(s17 + 0x550);
    if (r > 0) {
        f = (f32)r / 10.0f;
    } else if (r < 0) {
        f = (f32)(-r) / 10.0f;
    }
    f = 1.0f - sdfSinPoly(f * 3.14159265f);
    func_0024E5A0(0, 0, 0, 0x80, 0x3A, 0x53, 1.5f, 1.5f);
    mnuDrawPanelWithPackedColorPattern(0, 0, 0, 0x80, 0x53);
    func_0025D2F8(0, 0, 1, (s32)(f * 128.0f), s17, 0x53);
    func_00254758(0, 0, 1, 0x80, 0x53);
    func_0024E260(0, 0, 0, 0x80, 0xF, 0x53);
    func_0024E260(0, 0, 0, 0x80, 0x5B, 0x53);
    v = mnuGetSelectedNodeValue();
    mnuDrawMantraCostAfterListAdvance(*v, s17 + 0x590, 0x80, 0x53);
    func_0024E260(0x20, 0, 0, 0x80, 0xB, 0x53);
tail:
    mnuChooseDisplaySpriteKindFromEntryFlags(s17, 0x80, 0x53);
    func_0024E260(0, 0, 0, 0x80, 0xD, 0x53);
    return 0;
}

void mnuDrawDisplaySpriteAndPanelMarks(s32 p0, s32 a1, s32 a2) {
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

void func_00254778(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4) {
    func_0024E260(a0, a1, a2, a3, 0x47, a4);
    func_0024E260(a0, a1, a2, a3, 0x48, a4);
    func_0024E260(a0, a1, a2, a3, 0x49, a4);
}

INCLUDE_ASM(const s32, "game/code_00253E58", func_00254810);

INCLUDE_SDATA(const s32, "game/code_00253E58", D_003BC440);

