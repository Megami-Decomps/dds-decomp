#include "common.h"

typedef struct KwlnTask KwlnTask;

extern void* D_003BA800;

extern void* D_003BA80C;

extern KwlnTask* D_003BA818;

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

INCLUDE_ASM(const s32, "game/sdkTask", func_00101A58);

/* Persona 4 kwlnTaskGetTimer @ 00452540 (src/Kernel/sdkTask.c), recompiled unchanged */
u32 kwlnTaskGetTimer(void* task)
{
    return *(u32*)((u8*)task + 0x28);
}
