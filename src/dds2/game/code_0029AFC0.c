#include "common.h"

extern s32 func_00101958();

extern void func_0026C900(void);

extern s64 func_002C4038(s32, s32 *, u64, u64);

INCLUDE_ASM(const s32, "game/code_0029AFC0", mnuCheckTableSums);

INCLUDE_ASM(const s32, "game/code_0029AFC0", func_0029B008);

INCLUDE_ASM(const s32, "game/code_0029AFC0", func_0029B320);

void func_0029B378(s32 arg0) {
    s32 temp_v0 = func_00101958();

    func_0026C900();
    func_002C4038(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

u32 func_0029B3C0(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    func_002C0CF8(*(u32 *)(temp_v0 + 0xad34), 0xffffffffffffffff);
    func_0026C5B8(0x17);
    func_0026C648(0);
    func_0026C618(0xa3);
    return 1;
}

u32 func_0029B410(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0029AFC0", func_0029B418);

INCLUDE_ASM(const s32, "game/code_0029AFC0", func_0029B600);

void func_0029B658(s32 arg0) {
    s32 temp_v0 = func_00101958();

    func_0026C900();
    func_002C4038(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

INCLUDE_ASM(const s32, "game/code_0029AFC0", func_0029B6A0);

u32 func_0029B778(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0029AFC0", func_0029B780);

INCLUDE_ASM(const s32, "game/code_0029AFC0", func_0029B810);

void func_0029B868(s32 arg0) {
    s32 temp_v0 = func_00101958();

    func_0026C900();
    func_002C4038(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

INCLUDE_ASM(const s32, "game/code_0029AFC0", func_0029B8B0);

INCLUDE_ASM(const s32, "game/code_0029AFC0", func_0029B950);

INCLUDE_ASM(const s32, "game/code_0029AFC0", func_0029BB28);

u32 func_0029BBC0(void) {
    func_0026C710();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0029AFC0", func_0029BBE0);

INCLUDE_SDATA(const s32, "game/code_0029AFC0", D_004379B8);

