#include "common.h"
#include "pcp_vu0.h"

extern u32 kwlnDrawControlFlags;
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

/* Draw state blocks (D_003C2xxx) with dirty flags in kwlnDrawControlFlags. Each
 * setter writes its block, snapshots a flag byte to D_003BD7xx and
 * sets/clears its dirty bit. Overlapping word/byte views are unions;
 * byte symbols (e.g. D_003C2DCB = block+3) are declared separately
 * because those functions address the byte directly.
 */
#define DRAW_VIEWPORT_WIDTH 0x200
#define DRAW_VIEWPORT_HEIGHT 0x1C0

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
extern DrawBlkCD0 D_003C2CA0;
extern DrawBlkD30 D_003C2D00;
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
extern u16 D_003BD72C;
extern u16 D_003BD72E;
extern u16 D_003BD726;
extern u16 D_003BD736;
extern u16 D_003BD738;
extern u16 D_003BD73A;
extern u16 D_003BD734;
extern u16 D_003BD730;
extern u16 D_003BD732;
extern u16 D_003BD75A;
extern u16 D_003BD758;
extern u16 D_003BD754;
extern u16 D_003BD756;
extern u32 D_003BA934;
extern u32 D_003BA938;
extern void func_0018F6F0(void *);
extern void func_0018F6D8(void);
extern void func_0018F3B8(void *);
extern void func_0018F3A0(void);
extern void func_0018F6E8(void);
extern void func_0018F750(void);
extern void func_0018F3B0(void);
extern void effCopyCh71Common(void *);
extern u32 effGetCh72Work(void);
extern void func_0018F428(void);
extern void effCopyCh72Common(void *);
extern void func_0018F4E0(void);
extern void func_0018F4F0(void);
extern void func_0018F5A8(void);
extern void effCopyCh75Common(void *);
extern void func_0018F740(void);
extern void func_0018F438(void);
extern void effCopyCh76Common(void *);
extern u32 effGetCh76Work(void);
extern void func_0018F598(void);

void kwlnDrawCopyRow128(void *src) {
    PCP_COPY_VECTOR(&D_00324780, src);
}

void kwlnDrawCopyWords20(DrawWord20 *src) {
    memcpy(&D_00324790, src, 0x14);
}

void dds3DrawSetIndexedWord(u32 value, s32 index) {
    D_00324770[index] = value;
}

/* Default draw viewport is 512 by 448 pixels. */
void kwlnDrawInitRect(DrawRect *rect) {
    rect->w = DRAW_VIEWPORT_WIDTH;
    rect->h = DRAW_VIEWPORT_HEIGHT;
    rect->y = 0;
    rect->x = 0;
}

void kwlnDrawSetDc8Second(u32 value) {
    D_003C2DC8.unk04 = value;
}

void kwlnDrawSetDc8First(u32 value) {
    D_003C2DC8.u00.w = value;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106540);

void kwlnDrawSetupDc8(s32 mode) {
    DrawBlkDC8 *blk = &D_003C2DC8;
    s32 requestedMode = mode;

    D_003BD74C = 0;
    D_003BD748 = 0;
    D_003BD74E = blk->u00.b[3];
    D_003BD74A = requestedMode;
    if (requestedMode == 0) {
        kwlnDrawControlFlags &= ~0x80000;
        kwlnDrawInitRect(&blk->r08);
        func_0018F6F0(blk);
        func_0018F6D8();
    }
    else {
        kwlnDrawControlFlags |= 0x80000;
    }
}

void kwlnDrawEnableDc8(s32 enabled) {
    D_003BD74E = 0;
    D_003BD74C = D_003C2DC8.u00.b[3];
    D_003BD748 = 0;
    D_003BD74A = enabled;
    if (enabled == 0) {
        kwlnDrawControlFlags &= ~0x80000;
        kwlnDrawControlFlags &= ~0x100000;
        func_0018F6E8();
    }
    else {
        kwlnDrawControlFlags |= 0x80000;
    }
}

void kwlnDrawSetE08Fifth(u32 value) {
    D_003C2E08.unk10 = value;
}

void kwlnDrawSetE08Fourth(u32 value) {
    D_003C2E08.u0C.w = value;
}

void kwlnDrawSetE08Triple(u32 first, u32 second, u32 third) {
    D_003C2E08.unk00 = first;
    D_003C2E08.unk04 = second;
    D_003C2E08.unk08 = third;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106738);

void kwlnDrawSetupE08(s32 mode) {
    DrawBlkE08 *blk = &D_003C2E08;
    s32 requestedMode = mode;

    D_003BD758 = 0;
    D_003BD754 = 0;
    D_003BD75A = blk->u0C.b[3];
    D_003BD756 = requestedMode;
    if (requestedMode == 0) {
        kwlnDrawControlFlags &= ~0x200000;
        effCopyCh75Common(blk);
        func_0018F740();
    }
    else {
        kwlnDrawControlFlags |= 0x200000;
    }
}

void kwlnDrawEnableE08(s32 enabled) {
    D_003BD75A = 0;
    D_003BD758 = D_003C2E08.u0C.b[3];
    D_003BD754 = 0;
    D_003BD756 = enabled;
    if (enabled == 0) {
        kwlnDrawControlFlags &= ~0x200000;
        kwlnDrawControlFlags &= ~0x400000;
        func_0018F750();
    }
    else {
        kwlnDrawControlFlags |= 0x200000;
    }
}

/* Apply an offset immediately, or stage an interpolated move from the old offset. */
void kwlnDrawSetOffsetTransition(s32 transition, s32 x, s32 y) {
    if (transition == 0) {
        D_003BA90A = (s16)x;
        D_003BA90C = (s16)y;
        if ((x == 0) && (y == 0)) {
            D_003BA908 = 0;
        }
        else {
            D_003BA908 = 1;
        }
        kwlnDrawControlFlags = kwlnDrawControlFlags & 0xfffff7ff;
        return;
    }
    D_003BD6B8 = D_003BA90A;
    D_003BD6BA = D_003BA90C;
    D_003BD6BC = (s16)x;
    D_003BD6BE = (s16)y;
    D_003BD6B6 = (s16)transition;
    kwlnDrawControlFlags = kwlnDrawControlFlags | 0x800;
    D_003BD6B4 = 0;
}

void kwlnDrawSetD88FloatTriple(u32 value, f32 first, f32 second) {
    D_003C2D88.unk08 = first;
    D_003C2D88.unk0C = second;
    D_003C2D88.unk04 = value;
}

void kwlnDrawSetD88First(u32 value) {
    D_003C2D88.u00.w = value;
}

void kwlnDrawSetD88Pair(u32 first, u32 second) {
    D_003C2D88.unk10 = first;
    D_003C2D88.unk14 = second;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001069A8);

void kwlnDrawSetupD88(s32 mode) {
    DrawBlkD88 *blk = &D_003C2D88;
    s32 requestedMode = mode;

    D_003BD740 = 0;
    D_003BD73C = 0;
    D_003BD742 = blk->u00.b[3];
    D_003BD73E = requestedMode;
    if (requestedMode == 0) {
        kwlnDrawControlFlags &= ~0x20000;
        kwlnDrawInitRect(&blk->r18);
        func_0018F3B8(blk);
        func_0018F3A0();
    }
    else {
        kwlnDrawControlFlags |= 0x20000;
    }
}

void kwlnDrawEnableD88(s32 enabled) {
    D_003BD742 = 0;
    D_003BD740 = D_003C2D88.u00.b[3];
    D_003BD73C = 0;
    D_003BD73E = enabled;
    if (enabled == 0) {
        kwlnDrawControlFlags &= ~0x20000;
        kwlnDrawControlFlags &= ~0x40000;
        func_0018F3B0();
    }
    else {
        kwlnDrawControlFlags |= 0x20000;
    }
}

void kwlnDrawSetC70FloatTriple(u32 value, f32 first, f32 second) {
    D_003C2C70.unk0C = first;
    D_003C2C70.unk10 = second;
    D_003C2C70.unk08 = value;
}

void kwlnDrawSetC70Second(u32 value) {
    D_003C2C70.u04.w = value;
}

void kwlnDrawSetC70Triple(u32 first, u32 second, u32 third) {
    D_003C2C70.unk00 = first;
    D_003C2C70.unk14 = second;
    D_003C2C70.unk18 = third;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106BC8);

void kwlnDrawSetupC70(s32 mode) {
    DrawBlkC70 *blk = &D_003C2C70;
    s32 requestedMode = mode;

    D_003BD71C = 0;
    D_003BD718 = 0;
    D_003BD71E = blk->u04.b[3];
    D_003BD71A = requestedMode;
    if (requestedMode == 0) {
        kwlnDrawControlFlags &= ~0x1000;
        effCopyCh71Common(blk);
        func_0018F428();
    }
    else {
        kwlnDrawControlFlags |= 0x1000;
    }
}

void kwlnDrawSetupC70B(s32 mode) {
    DrawBlkC70 *blk = &D_003C2C70;
    s32 requestedMode = mode;

    D_003BD71E = 0;
    D_003BD718 = 0;
    D_003BD71C = blk->u04.b[3];
    D_003BD71A = requestedMode;
    if (requestedMode == 0) {
        kwlnDrawControlFlags &= ~0x1000;
        kwlnDrawControlFlags &= ~0x8000;
        effCopyCh71Common(blk);
        func_0018F438();
    }
    else {
        kwlnDrawControlFlags |= 0x1000;
    }
}

void kwlnDrawSetCd0Clamped(s32 boundedValue, s32 lastWord, s32 secondWord, s32 fourthWord,
                           f32 firstFloat, f32 secondFloat, f32 thirdFloat) {
    if (boundedValue >= 0x65) {
        D_003BA934++;
        boundedValue = 0x64;
    }
    D_003C2CD0.unk00 = boundedValue;
    D_003C2CD0.unk28 = lastWord;
    D_003C2CD0.unk04 = secondWord;
    D_003C2CD0.unk08 = firstFloat;
    D_003C2CD0.unk14 = secondFloat;
    D_003C2CD0.unk18 = thirdFloat;
    D_003C2CD0.unk10 = fourthWord;
}

void kwlnDrawSetCd0Fourth(u32 value) {
    D_003C2CD0.u0C.w = value;
}

void kwlnDrawSetCd0Triple(u32 first, u32 second, u32 third) {
    D_003C2CD0.unk24 = first;
    D_003C2CD0.unk1C = second;
    D_003C2CD0.unk20 = third;
}

void func_00106DF0(s32 mode) {
    D_003C2CA0 = *(DrawBlkCD0 *)effGetCh72Work();
    D_003BD72C = 0;
    D_003BD72E = mode;
    if (mode == 0) {
        kwlnDrawControlFlags &= 0xFFFEFFFF;
        effCopyCh72Common(&D_003C2CD0);
    } else {
        kwlnDrawControlFlags |= 0x10000;
    }
}

void kwlnDrawSetupCd0(s32 mode) {
    DrawBlkCD0 *blk = &D_003C2CD0;
    s32 requestedMode = mode;

    D_003BD728 = 0;
    D_003BD724 = 0;
    D_003BD72A = blk->u0C.b[3];
    D_003BD726 = requestedMode;
    if (requestedMode == 0) {
        kwlnDrawControlFlags &= ~0x2000;
        effCopyCh72Common(blk);
        func_0018F4E0();
    }
    else {
        kwlnDrawControlFlags |= 0x2000;
        effCopyCh72Common(blk);
        func_0018F4F0();
    }
}

void kwlnDrawEnableCd0(s32 enabled) {
    D_003BD72A = 0;
    D_003BD728 = D_003C2CD0.u0C.b[3];
    D_003BD724 = 0;
    D_003BD726 = enabled;
    if (enabled == 0) {
        kwlnDrawControlFlags &= ~0x2000;
        kwlnDrawControlFlags &= ~0x10000;
        func_0018F4F0();
    }
    else {
        kwlnDrawControlFlags |= 0x2000;
    }
}

void kwlnDrawSetD30Clamped(s32 boundedValue, s32 fourthWord, f32 firstFloat, f32 secondFloat,
                           f32 thirdFloat, f32 fourthFloat, f32 fifthFloat) {
    if (boundedValue >= 0x29) {
        D_003BA938++;
        boundedValue = 0x28;
    }
    D_003C2D30.unk00 = boundedValue;
    D_003C2D30.unk04 = firstFloat;
    D_003C2D30.unk08 = secondFloat;
    D_003C2D30.unk14 = thirdFloat;
    D_003C2D30.unk18 = fourthFloat;
    D_003C2D30.unk1C = fifthFloat;
    D_003C2D30.unk10 = fourthWord;
}

void kwlnDrawSetD30Fourth(u32 value) {
    D_003C2D30.u0C.w = value;
}

void kwlnDrawSetD30Triple(u32 first, u32 second, u32 third) {
    D_003C2D30.unk28 = first;
    D_003C2D30.unk20 = second;
    D_003C2D30.unk24 = third;
}

void func_00107018(s32 mode) {
    D_003C2D00 = *(DrawBlkD30 *)effGetCh76Work();
    D_003BD738 = 0;
    D_003BD73A = mode;
    if (mode == 0) {
        kwlnDrawControlFlags &= 0xFEFFFFFF;
        effCopyCh76Common(&D_003C2D30);
    } else {
        kwlnDrawControlFlags |= 0x1000000;
    }
}

void kwlnDrawSetupD30(s32 mode) {
    DrawBlkD30 *blk = &D_003C2D30;
    s32 requestedMode = mode;

    D_003BD734 = 0;
    D_003BD730 = 0;
    D_003BD736 = blk->u0C.b[3];
    D_003BD732 = requestedMode;
    if (requestedMode == 0) {
        kwlnDrawControlFlags &= ~0x800000;
        effCopyCh76Common(blk);
        func_0018F598();
    }
    else {
        kwlnDrawControlFlags |= 0x800000;
        effCopyCh76Common(blk);
        func_0018F5A8();
    }
}

void kwlnDrawEnableD30(s32 enabled) {
    D_003BD736 = 0;
    D_003BD734 = D_003C2D30.u0C.b[3];
    D_003BD730 = 0;
    D_003BD732 = enabled;
    if (enabled == 0) {
        kwlnDrawControlFlags &= ~0x800000;
        kwlnDrawControlFlags &= ~0x1000000;
        func_0018F5A8();
    }
    else {
        kwlnDrawControlFlags |= 0x800000;
    }
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001071E8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00107DE8);
