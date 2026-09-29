#include "common.h"

extern u32 func_00101958(void);

extern u32 func_001A0710(u32);

extern u32 func_00328D68(u32);

INCLUDE_ASM(const s32, "game/code_0024F9B8", func_0024F9B8);

INCLUDE_ASM(const s32, "game/code_0024F9B8", func_0024FA48);

INCLUDE_ASM(const s32, "game/code_0024F9B8", func_0024FB48);

INCLUDE_ASM(const s32, "game/code_0024F9B8", func_0024FCB8);

void func_0024FE28(void) {
    u32 *flags;

    flags = (u32 *)func_00101958();
    *flags = *flags | 1;
}

void func_0024FE50(void) {
    u32 *flags;

    flags = (u32 *)func_00101958();
    *flags = *flags & 0xfffffffe;
}

void func_0024FE80(u32 task) {
    kwlnTaskDestroyWithHierarchy(task, 1);
}

s32 *func_0024FE98(s32 *owner) {
    s32 *work = (s32 *)func_00328D68(8);
    work[0] = 0;
    work[1] = 0;
    return work;
}

void func_0024FEC0(s32 object, u32 value) {
    u32 result;

    result = func_001A0710(value);
    *(u32 *)(object + 4) = result;
}

INCLUDE_SDATA(const s32, "game/code_0024F9B8", D_004373B8);

