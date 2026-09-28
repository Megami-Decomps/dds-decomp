#include "common.h"

extern u32 D_003BA904;
extern u8 D_003BA908;
extern u16 D_003BA90A;
extern u16 D_003BA90C;
extern u16 D_003BD6B4;
extern u16 D_003BD6B6;
extern u16 D_003BD6B8;
extern u16 D_003BD6BA;
extern u16 D_003BD6BC;
extern u16 D_003BD6BE;

extern u32 D_00324770[4];

/* Draw state blocks (D_003C2xxx) with dirty flags in D_003BA904. Each
 * setter writes its block, snapshots a flag byte to D_003BD7xx and
 * sets/clears its dirty bit. Overlapping word/byte views are unions;
 * byte symbols (e.g. D_003C2DCB = block+3) are declared separately
 * because those functions address the byte directly.
 */
typedef struct {
    u32 x;
    u32 y;
    u32 w;
    u32 h;
} DrawRect;

typedef struct {
    u32 w[5];
} DrawWord20;

typedef struct {
    u32 w[4];
} DrawWord16;

typedef struct {
    union {
        u32 w;
        u8 b[4];
    } u00;
    u32 unk04;
    DrawRect r08;
} DrawBlkDC8;

typedef struct {
    u32 unk00;
    u32 unk04;
    u32 unk08;
    union {
        u32 w;
        u8 b[4];
    } u0C;
    u32 unk10;
} DrawBlkE08;

typedef struct {
    union {
        u32 w;
        u8 b[4];
    } u00;
    u32 unk04;
    f32 unk08;
    f32 unk0C;
    u32 unk10;
    u32 unk14;
    DrawRect r18;
} DrawBlkD88;

typedef struct {
    u32 unk00;
    union {
        u32 w;
        u8 b[4];
    } u04;
    u32 unk08;
    f32 unk0C;
    f32 unk10;
    u32 unk14;
    u32 unk18;
} DrawBlkC70;

typedef struct {
    u32 unk00;
    u32 unk04;
    f32 unk08;
    union {
        u32 w;
        u8 b[4];
    } u0C;
    u32 unk10;
    f32 unk14;
    f32 unk18;
    u32 unk1C;
    u32 unk20;
    u32 unk24;
    u32 unk28;
} DrawBlkCD0;

typedef struct {
    u32 unk00;
    f32 unk04;
    f32 unk08;
    union {
        u32 w;
        u8 b[4];
    } u0C;
    u32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u32 unk20;
    u32 unk24;
    u32 unk28;
} DrawBlkD30;

extern DrawBlkDC8 D_003C2DC8;
extern DrawBlkE08 D_003C2E08;
extern u32 D_003C2E14;
extern u32 D_003C2E18;
extern DrawBlkD88 D_003C2D88;
extern DrawBlkC70 D_003C2C70;
extern DrawBlkCD0 D_003C2CD0;
extern DrawBlkD30 D_003C2D30;
extern u32 D_003C2DCC;
extern u8 D_003C2DCB;
extern u8 D_003C2E17;
extern u8 D_003C2D8B;
extern u8 D_003C2CDF;
extern u8 D_003C2D3F;
extern u128 D_00324780;
extern DrawWord20 D_00324790;
extern u16 D_003BD74C;
extern u16 D_003BD748;
extern u16 D_003BD74E;
extern u16 D_003BD74A;
extern u16 D_003BD740;
extern u16 D_003BD73C;
extern u16 D_003BD742;
extern u16 D_003BD73E;
extern u16 D_003BD71C;
extern u16 D_003BD718;
extern u16 D_003BD71E;
extern u16 D_003BD71A;
extern u16 D_003BD728;
extern u16 D_003BD724;
extern u16 D_003BD72A;
extern u16 D_003BD726;
extern u16 D_003BD736;
extern u16 D_003BD734;
extern u16 D_003BD730;
extern u16 D_003BD732;
extern u16 D_003BD75A;
extern u16 D_003BD758;
extern u16 D_003BD754;
extern u16 D_003BD756;
extern u32 D_003BA934;
extern u32 D_003BA938;
extern void func_0018F6F0(void *arg0);
extern void func_0018F6D8(void);
extern void func_0018F3B8(void *arg0);
extern void func_0018F3A0(void);
extern void func_0018F6E8(void);
extern void func_0018F750(void);
extern void func_0018F3B0(void);
extern void effCopyCh71Common(void *arg0);
extern void func_0018F428(void);
extern void effCopyCh72Common(void *arg0);
extern void func_0018F4E0(void);
extern void func_0018F4F0(void);
extern void func_0018F5A8(void);
extern void effCopyCh75Common(void *arg0);
extern void func_0018F740(void);
extern void func_0018F438(void);
extern void effCopyCh76Common(void *arg0);
extern void func_0018F598(void);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", drawCopyRow128);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", drawCopyWords20);

void func_001064E8(u32 value, s32 index) {
    D_00324770[index] = value;
}

void drawInitRect(DrawRect *rect) {
    rect->w = 0x200;
    rect->h = 0x1C0;
    rect->y = 0;
    rect->x = 0;
}

void drawSetDc8Second(u32 arg0) {
    D_003C2DC8.unk04 = arg0;
}

void drawSetDc8First(u32 arg0) {
    D_003C2DC8.u00.w = arg0;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106540);

void drawSetupDc8(s32 arg0) {
    DrawBlkDC8 *blk = &D_003C2DC8;
    s32 t = arg0;

    D_003BD74C = 0;
    D_003BD748 = 0;
    D_003BD74E = blk->u00.b[3];
    D_003BD74A = t;
    if (t == 0) {
        D_003BA904 &= ~0x80000;
        drawInitRect(&blk->r08);
        func_0018F6F0(blk);
        func_0018F6D8();
    }
    else {
        D_003BA904 |= 0x80000;
    }
}

void drawEnableDc8(s32 arg0) {
    D_003BD74E = 0;
    D_003BD74C = D_003C2DC8.u00.b[3];
    D_003BD748 = 0;
    D_003BD74A = arg0;
    if (arg0 == 0) {
        D_003BA904 &= ~0x80000;
        D_003BA904 &= ~0x100000;
        func_0018F6E8();
    }
    else {
        D_003BA904 |= 0x80000;
    }
}

void drawSetE08Fifth(u32 arg0) {
    D_003C2E08.unk10 = arg0;
}

void drawSetE08Fourth(u32 arg0) {
    D_003C2E08.u0C.w = arg0;
}

void drawSetE08Triple(u32 arg0, u32 arg1, u32 arg2) {
    D_003C2E08.unk00 = arg0;
    D_003C2E08.unk04 = arg1;
    D_003C2E08.unk08 = arg2;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106738);

void drawSetupE08(s32 arg0) {
    DrawBlkE08 *blk = &D_003C2E08;
    s32 t = arg0;

    D_003BD758 = 0;
    D_003BD754 = 0;
    D_003BD75A = blk->u0C.b[3];
    D_003BD756 = t;
    if (t == 0) {
        D_003BA904 &= ~0x200000;
        effCopyCh75Common(blk);
        func_0018F740();
    }
    else {
        D_003BA904 |= 0x200000;
    }
}

void drawEnableE08(s32 arg0) {
    D_003BD75A = 0;
    D_003BD758 = D_003C2E08.u0C.b[3];
    D_003BD754 = 0;
    D_003BD756 = arg0;
    if (arg0 == 0) {
        D_003BA904 &= ~0x200000;
        D_003BA904 &= ~0x400000;
        func_0018F750();
    }
    else {
        D_003BA904 |= 0x200000;
    }
}

void func_001068F0(s32 arg0, s32 arg1, s32 arg2) {
    if (arg0 == 0) {
        D_003BA90A = (s16)arg1;
        D_003BA90C = (s16)arg2;
        if ((arg1 == 0) && (arg2 == 0)) {
            D_003BA908 = 0;
        }
        else {
            D_003BA908 = 1;
        }
        D_003BA904 = D_003BA904 & 0xfffff7ff;
        return;
    }
    D_003BD6B8 = D_003BA90A;
    D_003BD6BA = D_003BA90C;
    D_003BD6BC = (s16)arg1;
    D_003BD6BE = (s16)arg2;
    D_003BD6B6 = (s16)arg0;
    D_003BA904 = D_003BA904 | 0x800;
    D_003BD6B4 = 0;
}

void drawSetD88FloatTriple(u32 arg0, f32 farg0, f32 farg1) {
    D_003C2D88.unk08 = farg0;
    D_003C2D88.unk0C = farg1;
    D_003C2D88.unk04 = arg0;
}

void drawSetD88First(u32 arg0) {
    D_003C2D88.u00.w = arg0;
}

void drawSetD88Pair(u32 arg0, u32 arg1) {
    D_003C2D88.unk10 = arg0;
    D_003C2D88.unk14 = arg1;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001069A8);

void drawSetupD88(s32 arg0) {
    DrawBlkD88 *blk = &D_003C2D88;
    s32 t = arg0;

    D_003BD740 = 0;
    D_003BD73C = 0;
    D_003BD742 = blk->u00.b[3];
    D_003BD73E = t;
    if (t == 0) {
        D_003BA904 &= ~0x20000;
        drawInitRect(&blk->r18);
        func_0018F3B8(blk);
        func_0018F3A0();
    }
    else {
        D_003BA904 |= 0x20000;
    }
}

void drawEnableD88(s32 arg0) {
    D_003BD742 = 0;
    D_003BD740 = D_003C2D88.u00.b[3];
    D_003BD73C = 0;
    D_003BD73E = arg0;
    if (arg0 == 0) {
        D_003BA904 &= ~0x20000;
        D_003BA904 &= ~0x40000;
        func_0018F3B0();
    }
    else {
        D_003BA904 |= 0x20000;
    }
}

void drawSetC70FloatTriple(u32 arg0, f32 farg0, f32 farg1) {
    D_003C2C70.unk0C = farg0;
    D_003C2C70.unk10 = farg1;
    D_003C2C70.unk08 = arg0;
}

void drawSetC70Second(u32 arg0) {
    D_003C2C70.u04.w = arg0;
}

void drawSetC70Triple(u32 arg0, u32 arg1, u32 arg2) {
    D_003C2C70.unk00 = arg0;
    D_003C2C70.unk14 = arg1;
    D_003C2C70.unk18 = arg2;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106BC8);

void drawSetupC70(s32 arg0) {
    DrawBlkC70 *blk = &D_003C2C70;
    s32 t = arg0;

    D_003BD71C = 0;
    D_003BD718 = 0;
    D_003BD71E = blk->u04.b[3];
    D_003BD71A = t;
    if (t == 0) {
        D_003BA904 &= ~0x1000;
        effCopyCh71Common(blk);
        func_0018F428();
    }
    else {
        D_003BA904 |= 0x1000;
    }
}

void drawSetupC70B(s32 arg0) {
    DrawBlkC70 *blk = &D_003C2C70;
    s32 t = arg0;

    D_003BD71E = 0;
    D_003BD718 = 0;
    D_003BD71C = blk->u04.b[3];
    D_003BD71A = t;
    if (t == 0) {
        D_003BA904 &= ~0x1000;
        D_003BA904 &= ~0x8000;
        effCopyCh71Common(blk);
        func_0018F438();
    }
    else {
        D_003BA904 |= 0x1000;
    }
}

void drawSetCd0Clamped(s32 arg0, s32 arg1, s32 arg2, s32 arg3, f32 farg0, f32 farg1, f32 farg2) {
    if (arg0 >= 0x65) {
        D_003BA934++;
        arg0 = 0x64;
    }
    D_003C2CD0.unk00 = arg0;
    D_003C2CD0.unk28 = arg1;
    D_003C2CD0.unk04 = arg2;
    D_003C2CD0.unk08 = farg0;
    D_003C2CD0.unk14 = farg1;
    D_003C2CD0.unk18 = farg2;
    D_003C2CD0.unk10 = arg3;
}

void drawSetCd0Fourth(u32 arg0) {
    D_003C2CD0.u0C.w = arg0;
}

void drawSetCd0Triple(u32 arg0, u32 arg1, u32 arg2) {
    D_003C2CD0.unk24 = arg0;
    D_003C2CD0.unk1C = arg1;
    D_003C2CD0.unk20 = arg2;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106DF0);

void drawSetupCd0(s32 arg0) {
    DrawBlkCD0 *blk = &D_003C2CD0;
    s32 t = arg0;

    D_003BD728 = 0;
    D_003BD724 = 0;
    D_003BD72A = blk->u0C.b[3];
    D_003BD726 = t;
    if (t == 0) {
        D_003BA904 &= ~0x2000;
        effCopyCh72Common(blk);
        func_0018F4E0();
    }
    else {
        D_003BA904 |= 0x2000;
        effCopyCh72Common(blk);
        func_0018F4F0();
    }
}

void drawEnableCd0(s32 arg0) {
    D_003BD72A = 0;
    D_003BD728 = D_003C2CD0.u0C.b[3];
    D_003BD724 = 0;
    D_003BD726 = arg0;
    if (arg0 == 0) {
        D_003BA904 &= ~0x2000;
        D_003BA904 &= ~0x10000;
        func_0018F4F0();
    }
    else {
        D_003BA904 |= 0x2000;
    }
}

void drawSetD30Clamped(s32 arg0, s32 arg1, f32 farg0, f32 farg1, f32 farg2, f32 farg3, f32 farg4) {
    if (arg0 >= 0x29) {
        D_003BA938++;
        arg0 = 0x28;
    }
    D_003C2D30.unk00 = arg0;
    D_003C2D30.unk04 = farg0;
    D_003C2D30.unk08 = farg1;
    D_003C2D30.unk14 = farg2;
    D_003C2D30.unk18 = farg3;
    D_003C2D30.unk1C = farg4;
    D_003C2D30.unk10 = arg1;
}

void drawSetD30Fourth(u32 arg0) {
    D_003C2D30.u0C.w = arg0;
}

void drawSetD30Triple(u32 arg0, u32 arg1, u32 arg2) {
    D_003C2D30.unk28 = arg0;
    D_003C2D30.unk20 = arg1;
    D_003C2D30.unk24 = arg2;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00107018);

void drawSetupD30(s32 arg0) {
    DrawBlkD30 *blk = &D_003C2D30;
    s32 t = arg0;

    D_003BD734 = 0;
    D_003BD730 = 0;
    D_003BD736 = blk->u0C.b[3];
    D_003BD732 = t;
    if (t == 0) {
        D_003BA904 &= ~0x800000;
        effCopyCh76Common(blk);
        func_0018F598();
    }
    else {
        D_003BA904 |= 0x800000;
        effCopyCh76Common(blk);
        func_0018F5A8();
    }
}

void drawEnableD30(s32 arg0) {
    D_003BD736 = 0;
    D_003BD734 = D_003C2D30.u0C.b[3];
    D_003BD730 = 0;
    D_003BD732 = arg0;
    if (arg0 == 0) {
        D_003BA904 &= ~0x800000;
        D_003BA904 &= ~0x1000000;
        func_0018F5A8();
    }
    else {
        D_003BA904 |= 0x800000;
    }
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001071E8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00107DE8);
