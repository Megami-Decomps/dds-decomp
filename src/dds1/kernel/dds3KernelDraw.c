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

extern s8 D_00324770[13];

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106498);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001064B0);

void func_001064E8(u32 arg0, s32 arg1) {
    *(u32 *)(D_00324770 + arg1 * 4) = arg0;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106500);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106520);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106530);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106540);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106608);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106690);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106700);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106710);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106720);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106738);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106808);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106880);

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

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106968);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106980);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106990);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001069A8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106A90);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106B18);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106B88);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106BA0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106BB0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106BC8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106CA0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106D08);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106D80);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106DC8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106DD8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106DF0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106ED0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106F40);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106FA8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106FF0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00107000);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00107018);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001070F8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00107178);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001071E8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00107DE8);
