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

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106620);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106630);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106640);

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

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106AA8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106AC0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106AD0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106AE8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106BC0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106C28);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106CA0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106CE8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106CF8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106D10);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106DF0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106E60);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106EC8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106F10);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106F20);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106F38);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00107018);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00107098);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00107108);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00107D08);
