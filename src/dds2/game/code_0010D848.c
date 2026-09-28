#include "common.h"

extern s32 D_00438E8C;

extern u64 func_0010D650(u64);

extern u64 mdlFlagTest(u64);

u32 scrGetProcedureAddress(s32 arg0) {
    return *(u32 *)(arg0 * 0x20 + *(s32 *)(D_00438E8C + 0xb4) + 0x18);
}

u32 scrGetLabelAddress(s32 arg0) {
    return *(u32 *)(arg0 * 0x20 + *(s32 *)(D_00438E8C + 0xb8) + 0x18);
}

u32 scrGetProgramCounter(void) {
    return *(u32 *)(D_00438E8C + 0x18);
}

void scrSetProgramCounter(u32 arg0) {
    *(u32 *)(D_00438E8C + 0x18) = arg0;
}

u32 scrGetTimer(void) {
    return *(u32 *)(D_00438E8C + 0xd0);
}

u32 scrGetCommandTimer(void) {
    return *(u32 *)(D_00438E8C + 0xd4);
}

u32 scrGetWindow(void) {
    return *(u32 *)(D_00438E8C + 0xcc);
}

INCLUDE_ASM(const s32, "game/code_0010D848", func_0010D8C8);

u32 func_0010D8D0(void) {
    return *(u32 *)(D_00438E8C + 0xf0);
}

INCLUDE_ASM(const s32, "game/code_0010D848", func_0010D8E0);

u32 func_0010D910(void) {
    u64 flag;

    flag = func_0010D650(0);
    flag = mdlFlagTest(flag);
    func_0010D818(flag);
    return 1;
}

u32 func_0010D940(void) {
    u64 flag;

    flag = func_0010D650(0);
    mdlFlagSet(flag);
    return 1;
}

u32 func_0010D968(void) {
    u64 flag;

    flag = func_0010D650(0);
    mdlFlagClear(flag);
    return 1;
}
