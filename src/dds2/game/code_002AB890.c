#include "common.h"

extern s64 func_002ACF38(void);

extern s32 D_00435DD0;

extern s32 func_00101958();

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AB890);

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AB8C0);

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AB8F0);

void func_002ABCD0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xaa48);
    func_002B9520(*(u32 *)(temp_v0 + 8));
    func_002B9520(*(u32 *)(temp_v0 + 0xc));
}

INCLUDE_ASM(const s32, "game/code_002AB890", func_002ABD08);

INCLUDE_ASM(const s32, "game/code_002AB890", func_002ABD60);

void func_002ABEB0(s32 arg0) {
    func_002B9520(*(u32 *)(*(s32 *)(arg0 + 0xaa48) + 0x10));
}

INCLUDE_ASM(const s32, "game/code_002AB890", func_002ABED8);

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AC050);

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AC408);

void func_002AC660(s32 arg0) {
    func_002B9520(*(u32 *)(*(s32 *)(arg0 + 0xaa48) + 0x14));
}

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AC688);

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AC750);

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AC8F0);

void func_002ACA98(s32 arg0) {
    func_002B9520(*(u32 *)(*(s32 *)(arg0 + 0xaa48) + 0x18));
}

INCLUDE_ASM(const s32, "game/code_002AB890", func_002ACAC0);

void func_002ACB18(u32 arg0) {
    func_002A9460(2, arg0);
}

void func_002ACB38(void) {
}

INCLUDE_ASM(const s32, "game/code_002AB890", func_002ACB40);

INCLUDE_ASM(const s32, "game/code_002AB890", func_002ACBF8);

INCLUDE_ASM(const s32, "game/code_002AB890", func_002ACC50);

INCLUDE_ASM(const s32, "game/code_002AB890", func_002ACE58);

INCLUDE_ASM(const s32, "game/code_002AB890", func_002ACF00);

INCLUDE_ASM(const s32, "game/code_002AB890", func_002ACF38);

void func_002AD030(s32 arg0, s32 arg1) {
    s32 temp_v0;
    s64 temp_v1;

    temp_v0 = *(s32 *)(arg1 + 0xaa48);
    temp_v1 = func_002ACF38();
    if (temp_v1 != 0) {
        *(u32 *)(*(s32 *)(*(s32 *)(*(s32 *)(temp_v0 + 8) + 0x18) + 0x1c) + 0x60) =
                  (u32)*(u8 *)(arg0 + D_00435DD0 + 0x1340);
        *(s32 *)(temp_v0 + 0x38) = arg0;
    }
    func_002C1B68(arg1 + 0xaa50, 1);
}

u32 func_002AD0A8(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    func_002BAF50(*(u32 *)(*(s32 *)(temp_v0 + 0xaa48) + 8), temp_v0 + 0xb10c);
    return 1;
}

u32 func_002AD0E8(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    func_002BAF50(*(u32 *)(temp_v0 + 0x108), temp_v0 + 0xb10c);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AD118);

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AD330);
