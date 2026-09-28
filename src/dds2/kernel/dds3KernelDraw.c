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

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", drawCopyRow128);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", drawCopyWords20);

void func_00106408(u32 arg0, s32 arg1) {
    *(u32 *)(D_0037F770 + arg1 * 4) = arg0;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", drawInitRect);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", drawSetDc8Second);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", drawSetDc8First);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106460);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", drawSetupDc8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", drawEnableDc8);

void drawSetE08Fifth(u32 arg0) {
    D_0043E588.unk10 = arg0;
}

void drawSetE08Fourth(u32 arg0) {
    D_0043E588.u0C.w = arg0;
}

void drawSetE08Triple(u32 arg0, u32 arg1, u32 arg2) {
    D_0043E588.unk00 = arg0;
    D_0043E588.unk04 = arg1;
    D_0043E588.unk08 = arg2;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106658);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", drawSetupE08);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", drawEnableE08);

void func_00106810(s32 arg0, s32 arg1, s32 arg2) {
    if (arg0 == 0) {
        D_00435CDA = (s16)arg1;
        D_00435CDC = (s16)arg2;
        if ((arg1 == 0) && (arg2 == 0)) {
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
    D_00438DBC = (s16)arg1;
    D_00438DBE = (s16)arg2;
    D_00438DB6 = (s16)arg0;
    D_00435CD4 = D_00435CD4 | 0x800;
    D_00438DB4 = 0;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", drawSetD88FloatTriple);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", drawSetD88First);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", drawSetD88Pair);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001068C8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", drawSetupD88);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", drawEnableD88);

void drawSetC70FloatTriple(u32 arg0, f32 farg0, f32 farg1) {
    D_0043E3F0.unk0C = farg0;
    D_0043E3F0.unk10 = farg1;
    D_0043E3F0.unk08 = arg0;
}

void drawSetC70Second(u32 arg0) {
    D_0043E3F0.u04.w = arg0;
}

void drawSetC70Triple(u32 arg0, u32 arg1, u32 arg2) {
    D_0043E3F0.unk00 = arg0;
    D_0043E3F0.unk14 = arg1;
    D_0043E3F0.unk18 = arg2;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106AE8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", drawSetupC70);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", drawSetupC70B);

void drawSetCd0Clamped(s32 arg0, s32 arg1, s32 arg2, s32 arg3, f32 farg0, f32 farg1, f32 farg2) {
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

void drawSetCd0Fourth(u32 arg0) {
    D_0043E450.u0C.w = arg0;
}

void drawSetCd0Triple(u32 arg0, u32 arg1, u32 arg2) {
    D_0043E450.unk24 = arg0;
    D_0043E450.unk1C = arg1;
    D_0043E450.unk20 = arg2;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106D10);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", drawSetupCd0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", drawEnableCd0);

void drawSetD30Clamped(s32 arg0, s32 arg1, f32 farg0, f32 farg1, f32 farg2, f32 farg3, f32 farg4) {
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

void drawSetD30Fourth(u32 arg0) {
    D_0043E4B0.u0C.w = arg0;
}

void drawSetD30Triple(u32 arg0, u32 arg1, u32 arg2) {
    D_0043E4B0.unk28 = arg0;
    D_0043E4B0.unk20 = arg1;
    D_0043E4B0.unk24 = arg2;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106F38);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", drawSetupD30);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", drawEnableD30);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00107108);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00107D08);
