#include "common.h"

extern s8 D_0037F770[13];

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

extern DrawBlkC70 D_0043E508;

extern u128 D_0037F780;

extern u16 D_00438E5A;

extern u16 D_00438E58;

extern u16 D_00438E54;

extern u16 D_00438E56;

extern void effCopyCh75Common(void *arg0);

extern void func_00197378(void);

extern void func_00197388(void);

extern u16 D_00438E1C;

extern u16 D_00438E18;

extern u16 D_00438E1E;

extern u16 D_00438E1A;

extern void effCopyCh71Common(void *arg0);

extern void func_00197060(void);

extern void func_00197070(void);

extern u16 D_00438E28;

extern u16 D_00438E24;

extern u16 D_00438E2A;

extern u16 D_00438E26;

extern void effCopyCh72Common(void *arg0);

extern void func_00197118(void);

extern void func_00197128(void);

extern u16 D_00438E36;

extern u16 D_00438E34;

extern u16 D_00438E30;

extern u16 D_00438E32;

extern void func_001971E0(void);

extern void effCopyCh76Common(void *arg0);

extern void func_001971D0(void);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", kwlnDrawCopyRow128);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", kwlnDrawCopyWords20);

void dds3DrawSetIndexedWord(u32 value, s32 index) {
    *(u32 *)(D_0037F770 + index * 4) = value;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", kwlnDrawInitRect);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", kwlnDrawSetDc8Second);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", kwlnDrawSetDc8First);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106460);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", kwlnDrawSetupDc8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", kwlnDrawEnableDc8);

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
    s32 t = mode;

    D_00438E58 = 0;
    D_00438E54 = 0;
    D_00438E5A = blk->u0C.b[3];
    D_00438E56 = t;
    if (t == 0) {
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

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", kwlnDrawSetD88FloatTriple);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", kwlnDrawSetD88First);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", kwlnDrawSetD88Pair);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001068C8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", kwlnDrawSetupD88);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", kwlnDrawEnableD88);

void kwlnDrawSetC70FloatTriple(u32 arg0, f32 farg0, f32 farg1) {
    D_0043E3F0.unk0C = farg0;
    D_0043E3F0.unk10 = farg1;
    D_0043E3F0.unk08 = arg0;
}

void kwlnDrawSetC70Second(u32 arg0) {
    D_0043E3F0.u04.w = arg0;
}

void kwlnDrawSetC70Triple(u32 arg0, u32 arg1, u32 arg2) {
    D_0043E3F0.unk00 = arg0;
    D_0043E3F0.unk14 = arg1;
    D_0043E3F0.unk18 = arg2;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106AE8);

void kwlnDrawSetupC70(s32 mode) {
    DrawBlkC70 *blk = &D_0043E3F0;
    s32 t = mode;

    D_00438E1C = 0;
    D_00438E18 = 0;
    D_00438E1E = blk->u04.b[3];
    D_00438E1A = t;
    if (t == 0) {
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
    s32 t = mode;

    D_00438E1E = 0;
    D_00438E18 = 0;
    D_00438E1C = blk->u04.b[3];
    D_00438E1A = t;
    if (t == 0) {
        D_00435CD4 &= ~0x1000;
        D_00435CD4 &= ~0x8000;
        effCopyCh71Common(blk);
        func_00197070();
    }
    else {
        D_00435CD4 |= 0x1000;
    }
}

void kwlnDrawSetCd0Clamped(s32 arg0, s32 arg1, s32 arg2, s32 arg3, f32 farg0, f32 farg1, f32 farg2) {
    if (arg0 >= 0x65) {
        D_00435D04++;
        arg0 = 0x64;
    }
    D_0043E450.unk00 = arg0;
    D_0043E450.unk28 = arg1;
    D_0043E450.unk04 = arg2;
    D_0043E450.unk08 = farg0;
    D_0043E450.unk14 = farg1;
    D_0043E450.unk18 = farg2;
    D_0043E450.unk10 = arg3;
}

void kwlnDrawSetCd0Fourth(u32 arg0) {
    D_0043E450.u0C.w = arg0;
}

void kwlnDrawSetCd0Triple(u32 arg0, u32 arg1, u32 arg2) {
    D_0043E450.unk24 = arg0;
    D_0043E450.unk1C = arg1;
    D_0043E450.unk20 = arg2;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106D10);

void kwlnDrawSetupCd0(s32 mode) {
    DrawBlkCD0 *blk = &D_0043E450;
    s32 t = mode;

    D_00438E28 = 0;
    D_00438E24 = 0;
    D_00438E2A = blk->u0C.b[3];
    D_00438E26 = t;
    if (t == 0) {
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

void kwlnDrawSetD30Clamped(s32 arg0, s32 arg1, f32 farg0, f32 farg1, f32 farg2, f32 farg3, f32 farg4) {
    if (arg0 >= 0x29) {
        D_00435D08++;
        arg0 = 0x28;
    }
    D_0043E4B0.unk00 = arg0;
    D_0043E4B0.unk04 = farg0;
    D_0043E4B0.unk08 = farg1;
    D_0043E4B0.unk14 = farg2;
    D_0043E4B0.unk18 = farg3;
    D_0043E4B0.unk1C = farg4;
    D_0043E4B0.unk10 = arg1;
}

void kwlnDrawSetD30Fourth(u32 arg0) {
    D_0043E4B0.u0C.w = arg0;
}

void kwlnDrawSetD30Triple(u32 arg0, u32 arg1, u32 arg2) {
    D_0043E4B0.unk28 = arg0;
    D_0043E4B0.unk20 = arg1;
    D_0043E4B0.unk24 = arg2;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106F38);

void kwlnDrawSetupD30(s32 mode) {
    DrawBlkD30 *blk = &D_0043E4B0;
    s32 t = mode;

    D_00438E34 = 0;
    D_00438E30 = 0;
    D_00438E36 = blk->u0C.b[3];
    D_00438E32 = t;
    if (t == 0) {
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
