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

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001063B8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001063D0);

void func_00106408(u32 arg0, s32 arg1) {
    *(u32 *)(D_0037F770 + arg1 * 4) = arg0;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106420);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106440);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106450);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106460);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106528);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001065B0);

void func_00106620(u32 arg0) {
    D_0043E588.unk10 = arg0;
}

void func_00106630(u32 arg0) {
    D_0043E588.u0C.w = arg0;
}

void func_00106640(u32 arg0, u32 arg1, u32 arg2) {
    D_0043E588.unk00 = arg0;
    D_0043E588.unk04 = arg1;
    D_0043E588.unk08 = arg2;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106658);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106728);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001067A0);

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

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106888);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001068A0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001068B0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001068C8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001069B0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106A38);

void func_00106AA8(u32 arg0, f32 farg0, f32 farg1) {
    D_0043E3F0.unk0C = farg0;
    D_0043E3F0.unk10 = farg1;
    D_0043E3F0.unk08 = arg0;
}

void func_00106AC0(u32 arg0) {
    D_0043E3F0.u04.w = arg0;
}

void func_00106AD0(u32 arg0, u32 arg1, u32 arg2) {
    D_0043E3F0.unk00 = arg0;
    D_0043E3F0.unk14 = arg1;
    D_0043E3F0.unk18 = arg2;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106AE8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106BC0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106C28);

void func_00106CA0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, f32 farg0, f32 farg1, f32 farg2) {
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

void func_00106CE8(u32 arg0) {
    D_0043E450.u0C.w = arg0;
}

void func_00106CF8(u32 arg0, u32 arg1, u32 arg2) {
    D_0043E450.unk24 = arg0;
    D_0043E450.unk1C = arg1;
    D_0043E450.unk20 = arg2;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106D10);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106DF0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106E60);

void func_00106EC8(s32 arg0, s32 arg1, f32 farg0, f32 farg1, f32 farg2, f32 farg3, f32 farg4) {
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

void func_00106F10(u32 arg0) {
    D_0043E4B0.u0C.w = arg0;
}

void func_00106F20(u32 arg0, u32 arg1, u32 arg2) {
    D_0043E4B0.unk28 = arg0;
    D_0043E4B0.unk20 = arg1;
    D_0043E4B0.unk24 = arg2;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106F38);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00107018);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00107098);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00107108);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00107D08);
