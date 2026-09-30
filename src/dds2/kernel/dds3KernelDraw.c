#include "common.h"

extern u32 D_0037F770[4];

extern u32 D_00435CD4;

extern u8 D_00435CD8;

extern u16 D_00435CDA;

extern u16 D_00435CDC;

extern u16 D_00438DB4;

extern u16 D_00438DB6;

extern u16 D_00438DB8;

extern u16 D_00438DBA;

extern u16 D_00438DBC;

extern u16 D_00438DBE;

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

extern DrawBlkE08 D_0043E588;

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

extern DrawBlkC70 D_0043E3F0;

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

extern DrawBlkCD0 D_0043E450;

extern u32 D_00435D04;

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

extern DrawBlkD30 D_0043E4B0;

extern u32 D_00435D08;

extern u16 D_00438E5A;

extern u16 D_00438E58;

extern u16 D_00438E54;

extern u16 D_00438E56;

extern void effCopyCh75Common(void *);

extern void func_00197378(void);

extern void func_00197388(void);

extern u16 D_00438E1C;

extern u16 D_00438E18;

extern u16 D_00438E1E;

extern u16 D_00438E1A;

extern void effCopyCh71Common(void *);

extern void func_00197060(void);

extern void func_00197070(void);

extern u16 D_00438E28;

extern u16 D_00438E24;

extern u16 D_00438E2A;

extern u16 D_00438E26;

extern void effCopyCh72Common(void *);

extern void func_00197118(void);

extern void func_00197128(void);

extern u16 D_00438E36;

extern u16 D_00438E34;

extern u16 D_00438E30;

extern u16 D_00438E32;

extern void func_001971E0(void);

extern void effCopyCh76Common(void *);

extern void func_001971D0(void);

#define DRAW_VIEWPORT_WIDTH 0x200

#define DRAW_VIEWPORT_HEIGHT 0x1C0

typedef struct {
    u32 x;
    u32 y;
    u32 w;
    u32 h;
} DrawRect;

typedef struct {
    union {
        u32 w;
        u8 b[4];
    } u00;
    u32 unk04;
    DrawRect r08;
} DrawBlkDC8;

extern DrawBlkDC8 D_0043E548;

extern u16 D_00438E4C;

extern u16 D_00438E48;

extern u16 D_00438E4E;

extern u16 D_00438E4A;

extern void func_00197328(void *);

extern void func_00197310(void);

extern void func_00197320(void);

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

extern DrawBlkD88 D_0043E508;

extern u16 D_00438E40;

extern u16 D_00438E3C;

extern u16 D_00438E42;

extern u16 D_00438E3E;

extern void func_00196FF0(void *);

extern void func_00196FD8(void);

extern void func_00196FE8(void);

extern u128 D_0037F780;

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", kwlnDrawCopyRow128);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", kwlnDrawCopyWords20);

void dds3DrawSetIndexedWord(u32 value, s32 index) {
    D_0037F770[index] = value;
}

/* Default draw viewport is 512 by 448 pixels. */
void kwlnDrawInitRect(DrawRect *rect) {
    rect->w = DRAW_VIEWPORT_WIDTH;
    rect->h = DRAW_VIEWPORT_HEIGHT;
    rect->y = 0;
    rect->x = 0;
}

void kwlnDrawSetDc8Second(u32 value) {
    D_0043E548.unk04 = value;
}

void kwlnDrawSetDc8First(u32 value) {
    D_0043E548.u00.w = value;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106460);

void kwlnDrawSetupDc8(s32 mode) {
    DrawBlkDC8 *blk = &D_0043E548;
    s32 requestedMode = mode;

    D_00438E4C = 0;
    D_00438E48 = 0;
    D_00438E4E = blk->u00.b[3];
    D_00438E4A = requestedMode;
    if (requestedMode == 0) {
        D_00435CD4 &= ~0x80000;
        kwlnDrawInitRect(&blk->r08);
        func_00197328(blk);
        func_00197310();
    }
    else {
        D_00435CD4 |= 0x80000;
    }
}

void kwlnDrawEnableDc8(s32 enabled) {
    D_00438E4E = 0;
    D_00438E4C = D_0043E548.u00.b[3];
    D_00438E48 = 0;
    D_00438E4A = enabled;
    if (enabled == 0) {
        D_00435CD4 &= ~0x80000;
        D_00435CD4 &= ~0x100000;
        func_00197320();
    }
    else {
        D_00435CD4 |= 0x80000;
    }
}

void kwlnDrawSetE08Fifth(u32 value) {
    D_0043E588.unk10 = value;
}

void kwlnDrawSetE08Fourth(u32 value) {
    D_0043E588.u0C.w = value;
}

void kwlnDrawSetE08Triple(u32 first, u32 second, u32 third) {
    D_0043E588.unk00 = first;
    D_0043E588.unk04 = second;
    D_0043E588.unk08 = third;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106658);

void kwlnDrawSetupE08(s32 mode) {
    DrawBlkE08 *blk = &D_0043E588;
    s32 requestedMode = mode;

    D_00438E58 = 0;
    D_00438E54 = 0;
    D_00438E5A = blk->u0C.b[3];
    D_00438E56 = requestedMode;
    if (requestedMode == 0) {
        D_00435CD4 &= ~0x200000;
        effCopyCh75Common(blk);
        func_00197378();
    }
    else {
        D_00435CD4 |= 0x200000;
    }
}

void kwlnDrawEnableE08(s32 mode) {
    D_00438E5A = 0;
    D_00438E58 = D_0043E588.u0C.b[3];
    D_00438E54 = 0;
    D_00438E56 = mode;
    if (mode == 0) {
        D_00435CD4 &= ~0x200000;
        D_00435CD4 &= ~0x400000;
        func_00197388();
    }
    else {
        D_00435CD4 |= 0x200000;
    }
}

/* Apply an offset immediately, or stage an interpolated move from the old offset. */
void kwlnDrawSetOffsetTransition(s32 transition, s32 x, s32 y) {
    if (transition == 0) {
        D_00435CDA = (s16)x;
        D_00435CDC = (s16)y;
        if ((x == 0) && (y == 0)) {
            D_00435CD8 = 0;
        }
        else {
            D_00435CD8 = 1;
        }
        D_00435CD4 = D_00435CD4 & 0xfffff7ff;
        return;
    }
    D_00438DB8 = D_00435CDA;
    D_00438DBA = D_00435CDC;
    D_00438DBC = (s16)x;
    D_00438DBE = (s16)y;
    D_00438DB6 = (s16)transition;
    D_00435CD4 = D_00435CD4 | 0x800;
    D_00438DB4 = 0;
}

void kwlnDrawSetD88FloatTriple(u32 value, f32 first, f32 second) {
    D_0043E508.unk08 = first;
    D_0043E508.unk0C = second;
    D_0043E508.unk04 = value;
}

void kwlnDrawSetD88First(u32 value) {
    D_0043E508.u00.w = value;
}

void kwlnDrawSetD88Pair(u32 first, u32 second) {
    D_0043E508.unk10 = first;
    D_0043E508.unk14 = second;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001068C8);

void kwlnDrawSetupD88(s32 mode) {
    DrawBlkD88 *blk = &D_0043E508;
    s32 requestedMode = mode;

    D_00438E40 = 0;
    D_00438E3C = 0;
    D_00438E42 = blk->u00.b[3];
    D_00438E3E = requestedMode;
    if (requestedMode == 0) {
        D_00435CD4 &= ~0x20000;
        kwlnDrawInitRect(&blk->r18);
        func_00196FF0(blk);
        func_00196FD8();
    }
    else {
        D_00435CD4 |= 0x20000;
    }
}

void kwlnDrawEnableD88(s32 enabled) {
    D_00438E42 = 0;
    D_00438E40 = D_0043E508.u00.b[3];
    D_00438E3C = 0;
    D_00438E3E = enabled;
    if (enabled == 0) {
        D_00435CD4 &= ~0x20000;
        D_00435CD4 &= ~0x40000;
        func_00196FE8();
    }
    else {
        D_00435CD4 |= 0x20000;
    }
}

void kwlnDrawSetC70FloatTriple(u32 value, f32 first, f32 second) {
    D_0043E3F0.unk0C = first;
    D_0043E3F0.unk10 = second;
    D_0043E3F0.unk08 = value;
}

void kwlnDrawSetC70Second(u32 value) {
    D_0043E3F0.u04.w = value;
}

void kwlnDrawSetC70Triple(u32 first, u32 second, u32 third) {
    D_0043E3F0.unk00 = first;
    D_0043E3F0.unk14 = second;
    D_0043E3F0.unk18 = third;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106AE8);

void kwlnDrawSetupC70(s32 mode) {
    DrawBlkC70 *blk = &D_0043E3F0;
    s32 requestedMode = mode;

    D_00438E1C = 0;
    D_00438E18 = 0;
    D_00438E1E = blk->u04.b[3];
    D_00438E1A = requestedMode;
    if (requestedMode == 0) {
        D_00435CD4 &= ~0x1000;
        effCopyCh71Common(blk);
        func_00197060();
    }
    else {
        D_00435CD4 |= 0x1000;
    }
}

void kwlnDrawSetupC70B(s32 mode) {
    DrawBlkC70 *blk = &D_0043E3F0;
    s32 requestedMode = mode;

    D_00438E1E = 0;
    D_00438E18 = 0;
    D_00438E1C = blk->u04.b[3];
    D_00438E1A = requestedMode;
    if (requestedMode == 0) {
        D_00435CD4 &= ~0x1000;
        D_00435CD4 &= ~0x8000;
        effCopyCh71Common(blk);
        func_00197070();
    }
    else {
        D_00435CD4 |= 0x1000;
    }
}

void kwlnDrawSetCd0Clamped(s32 boundedValue, s32 lastWord, s32 secondWord, s32 fourthWord,
                           f32 firstFloat, f32 secondFloat, f32 thirdFloat) {
    if (boundedValue >= 0x65) {
        D_00435D04++;
        boundedValue = 0x64;
    }
    D_0043E450.unk00 = boundedValue;
    D_0043E450.unk28 = lastWord;
    D_0043E450.unk04 = secondWord;
    D_0043E450.unk08 = firstFloat;
    D_0043E450.unk14 = secondFloat;
    D_0043E450.unk18 = thirdFloat;
    D_0043E450.unk10 = fourthWord;
}

void kwlnDrawSetCd0Fourth(u32 value) {
    D_0043E450.u0C.w = value;
}

void kwlnDrawSetCd0Triple(u32 first, u32 second, u32 third) {
    D_0043E450.unk24 = first;
    D_0043E450.unk1C = second;
    D_0043E450.unk20 = third;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106D10);

void kwlnDrawSetupCd0(s32 mode) {
    DrawBlkCD0 *blk = &D_0043E450;
    s32 requestedMode = mode;

    D_00438E28 = 0;
    D_00438E24 = 0;
    D_00438E2A = blk->u0C.b[3];
    D_00438E26 = requestedMode;
    if (requestedMode == 0) {
        D_00435CD4 &= ~0x2000;
        effCopyCh72Common(blk);
        func_00197118();
    }
    else {
        D_00435CD4 |= 0x2000;
        effCopyCh72Common(blk);
        func_00197128();
    }
}

void kwlnDrawEnableCd0(s32 mode) {
    D_00438E2A = 0;
    D_00438E28 = D_0043E450.u0C.b[3];
    D_00438E24 = 0;
    D_00438E26 = mode;
    if (mode == 0) {
        D_00435CD4 &= ~0x2000;
        D_00435CD4 &= ~0x10000;
        func_00197128();
    }
    else {
        D_00435CD4 |= 0x2000;
    }
}

void kwlnDrawSetD30Clamped(s32 boundedValue, s32 fourthWord, f32 firstFloat, f32 secondFloat,
                           f32 thirdFloat, f32 fourthFloat, f32 fifthFloat) {
    if (boundedValue >= 0x29) {
        D_00435D08++;
        boundedValue = 0x28;
    }
    D_0043E4B0.unk00 = boundedValue;
    D_0043E4B0.unk04 = firstFloat;
    D_0043E4B0.unk08 = secondFloat;
    D_0043E4B0.unk14 = thirdFloat;
    D_0043E4B0.unk18 = fourthFloat;
    D_0043E4B0.unk1C = fifthFloat;
    D_0043E4B0.unk10 = fourthWord;
}

void kwlnDrawSetD30Fourth(u32 value) {
    D_0043E4B0.u0C.w = value;
}

void kwlnDrawSetD30Triple(u32 first, u32 second, u32 third) {
    D_0043E4B0.unk28 = first;
    D_0043E4B0.unk20 = second;
    D_0043E4B0.unk24 = third;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106F38);

void kwlnDrawSetupD30(s32 mode) {
    DrawBlkD30 *blk = &D_0043E4B0;
    s32 requestedMode = mode;

    D_00438E34 = 0;
    D_00438E30 = 0;
    D_00438E36 = blk->u0C.b[3];
    D_00438E32 = requestedMode;
    if (requestedMode == 0) {
        D_00435CD4 &= ~0x800000;
        effCopyCh76Common(blk);
        func_001971D0();
    }
    else {
        D_00435CD4 |= 0x800000;
        effCopyCh76Common(blk);
        func_001971E0();
    }
}

void kwlnDrawEnableD30(s32 mode) {
    D_00438E36 = 0;
    D_00438E34 = D_0043E4B0.u0C.b[3];
    D_00438E30 = 0;
    D_00438E32 = mode;
    if (mode == 0) {
        D_00435CD4 &= ~0x800000;
        D_00435CD4 &= ~0x1000000;
        func_001971E0();
    }
    else {
        D_00435CD4 |= 0x800000;
    }
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00107108);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00107D08);
