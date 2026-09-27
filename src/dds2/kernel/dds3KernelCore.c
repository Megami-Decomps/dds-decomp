#include "common.h"

typedef struct KwlnTask KwlnTask;

extern void* D_00435BD0;

extern void* D_00435BDC;

extern KwlnTask* D_00435BE8;

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100980);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_001009C8);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100A28);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100C28);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100D0C);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100D50);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100DD8);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100E50);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100EB0);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100F48);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101010);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_001010A8);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101100);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101198);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_001011D0);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101250);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101328);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101428);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101458);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101598);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_001015E0);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101678);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_001016E0);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101700);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101740);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101820);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101880);

/* Persona 4 func_00452490 @ 00452490 (src/Kernel/sdkTask.c), recompiled unchanged */
s32 func_001018B0(void* target)
{
    s32 idx;
    void* node;
    node = 0;
    idx = 0;
    for (; idx < 3; idx++)
    {
        switch (idx)
        {
        case 0:
            node = D_00435BD0;
            break;
        case 1:
            node = D_00435BE8;
            break;
        case 2:
            node = D_00435BDC;
            break;
        }
        while (node != 0)
        {
            if (node == target)
            {
                return 1;
            }
            node = *(void**)((u8*)node + 0x3C);
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101940);

/* Persona 4 kwlnTaskGetTimer @ 00452540 (src/Kernel/sdkTask.c), recompiled unchanged */
u32 kwlnTaskGetTimer(void* task)
{
    return *(u32*)((u8*)task + 0x28);
}

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101950);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101958);

void func_00101960(void) {
}

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101968);
