#include "common.h"

extern s32 D_00438E8C;

extern u64 func_0010D650(u64);

extern u64 mdlFlagTest(u64);

u32 func_0010D848(s32 arg0) {
    return *(u32 *)(arg0 * 0x20 + *(s32 *)(D_00438E8C + 0xb4) + 0x18);
}

u32 func_0010D860(s32 arg0) {
    return *(u32 *)(arg0 * 0x20 + *(s32 *)(D_00438E8C + 0xb8) + 0x18);
}

u32 func_0010D878(void) {
    return *(u32 *)(D_00438E8C + 0x18);
}

void func_0010D888(u32 arg0) {
    *(u32 *)(D_00438E8C + 0x18) = arg0;
}

u32 func_0010D898(void) {
    return *(u32 *)(D_00438E8C + 0xd0);
}

u32 func_0010D8A8(void) {
    return *(u32 *)(D_00438E8C + 0xd4);
}

u32 func_0010D8B8(void) {
    return *(u32 *)(D_00438E8C + 0xcc);
}

INCLUDE_ASM(const s32, "game/code_0010D848", func_0010D8C8);

u32 func_0010D8D0(void) {
    return *(u32 *)(D_00438E8C + 0xf0);
}

INCLUDE_ASM(const s32, "game/code_0010D848", func_0010D8E0);

u32 func_0010D910(void) {
    u64 temp_v0;

    temp_v0 = func_0010D650(0);
    temp_v0 = mdlFlagTest(temp_v0);
    func_0010D818(temp_v0);
    return 1;
}

u32 func_0010D940(void) {
    u64 temp_v0;

    temp_v0 = func_0010D650(0);
    mdlFlagSet(temp_v0);
    return 1;
}

u32 func_0010D968(void) {
    u64 temp_v0;

    temp_v0 = func_0010D650(0);
    mdlFlagClear(temp_v0);
    return 1;
}
