#include "common.h"

typedef struct KwlnTask KwlnTask;

extern void* D_003BA800;

extern void* D_003BA80C;

extern KwlnTask* D_003BA818;

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100A98);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100AE0);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100B40);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100D0C);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100D40);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100E68);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100EF0);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100F68);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100FC8);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101010);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101060);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_001011C0);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101218);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_001012B0);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_001012E8);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101368);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101440);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101540);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101570);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_001016B0);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_001016F8);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101790);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_001017F8);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101818);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101858);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101880);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101938);

/* Persona 4 func_00452490 @ 00452490 (src/Kernel/sdkTask.c), recompiled unchanged */
s32 func_001019C8(void* target)
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
            node = D_003BA800;
            break;
        case 1:
            node = D_003BA818;
            break;
        case 2:
            node = D_003BA80C;
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

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101A58);

/* Persona 4 kwlnTaskGetTimer @ 00452540 (src/Kernel/sdkTask.c), recompiled unchanged */
u32 kwlnTaskGetTimer(void* task)
{
    return *(u32*)((u8*)task + 0x28);
}

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101A68);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101A70);

void func_00101A78(void) {
}

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101A80);
