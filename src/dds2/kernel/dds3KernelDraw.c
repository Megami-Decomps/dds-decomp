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

void kwlnDrawSetE08Fifth(u32 arg0) {
    D_0043E588.unk10 = arg0;
}

void kwlnDrawSetE08Fourth(u32 arg0) {
    D_0043E588.u0C.w = arg0;
}

void kwlnDrawSetE08Triple(u32 arg0, u32 arg1, u32 arg2) {
    D_0043E588.unk00 = arg0;
    D_0043E588.unk04 = arg1;
    D_0043E588.unk08 = arg2;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106658);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", kwlnDrawSetupE08);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", kwlnDrawEnableE08);

void func_00106810(s32 transition, s32 x, s32 y) {
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

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", kwlnDrawSetupC70);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", kwlnDrawSetupC70B);

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

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", kwlnDrawSetupCd0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", kwlnDrawEnableCd0);

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

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", kwlnDrawSetupD30);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", kwlnDrawEnableD30);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00107108);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00107D08);
