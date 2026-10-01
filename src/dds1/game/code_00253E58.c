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
    s32 nodePrev;
    s32 nodeNext;
    s32 level;
    s32 scaled;
    s32 raw;
    f32 frac;
    u32 *cost;

    frac = 0.0f;
    mnuGetSelectedNodeValue();
    nodePrev = func_002CB3B8(D_003BC4CC, -1);
    nodeNext = func_002CB3B8(D_003BC4CC, 1);
    func_00255E08(nodePrev, 0x80, 0x52);
    level = *(s32 *)(nodePrev + 0x1C);
    if (level == 1) {
        goto case1;
    }
    if (level >= 2) {
        goto ge2;
    }
    if (level != 0) {
        goto tail;
    }
    goto case0;
ge2:
    if (level != 2) {
        goto tail;
    }
    goto case2;
case0:
    frac = (f32)*(s32 *)(nodePrev + 0x20) / 10.0f;
    scaled = (s32)(frac * 128.0f);
    func_0024E5A0(0, 0, 0, scaled, 0x3A, 0x53, 1.5f, 1.5f);
    mnuDrawPanelWithPackedColorPattern(0, 0, 0, scaled, 0x53);
    func_0025D2F8(0, 0, 1, scaled, nodeNext, 0x53);
    func_00254758(0, 0, 1, 0x80, 0x53);
    func_0024E260(0, 0, 0, 0x80, 0xF, 0x53);
    func_0024E260(0, 0, 0, scaled, 0x5B, 0x53);
    cost = mnuGetSelectedNodeValue();
    mnuDrawMantraCostAfterListAdvance(*cost, nodeNext + 0x590, scaled, 0x53);
    func_0024E260(0x20, 0, 0, scaled, 0xB, 0x53);
    goto tail;
case2:
    frac = (f32)*(s32 *)(nodePrev + 0x20) / 10.0f;
    frac = 1.0f - frac;
    scaled = (s32)(frac * 128.0f);
    func_0024E5A0(0, 0, 0, scaled, 0x3A, 0x53, 1.5f, 1.5f);
    mnuDrawPanelWithPackedColorPattern(0, 0, 0, scaled, 0x53);
    func_0025D2F8(0, 0, 1, scaled, nodeNext, 0x53);
    func_00254758(0, 0, 0, 0x80, 0x53);
    func_0024E260(0, 0, 0, 0x80, 0xF, 0x53);
    func_0024E260(0, 0, 0, scaled, 0x5B, 0x53);
    cost = mnuGetSelectedNodeValue();
    mnuDrawMantraCostAfterListAdvance(*cost, nodeNext + 0x590, scaled, 0x53);
    func_0024E260(0x20, 0, 0, scaled, 0xB, 0x53);
    goto tail;
case1:
    raw = *(s32 *)(nodeNext + 0x550);
    if (raw > 0) {
        frac = (f32)raw / 10.0f;
    } else if (raw < 0) {
        frac = (f32)(-raw) / 10.0f;
    }
    frac = 1.0f - sdfSinPoly(frac * 3.14159265f);
    func_0024E5A0(0, 0, 0, 0x80, 0x3A, 0x53, 1.5f, 1.5f);
    mnuDrawPanelWithPackedColorPattern(0, 0, 0, 0x80, 0x53);
    func_0025D2F8(0, 0, 1, (s32)(frac * 128.0f), nodeNext, 0x53);
    func_00254758(0, 0, 1, 0x80, 0x53);
    func_0024E260(0, 0, 0, 0x80, 0xF, 0x53);
    func_0024E260(0, 0, 0, 0x80, 0x5B, 0x53);
    cost = mnuGetSelectedNodeValue();
    mnuDrawMantraCostAfterListAdvance(*cost, nodeNext + 0x590, 0x80, 0x53);
    func_0024E260(0x20, 0, 0, 0x80, 0xB, 0x53);
tail:
    mnuChooseDisplaySpriteKindFromEntryFlags(nodeNext, 0x80, 0x53);
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

