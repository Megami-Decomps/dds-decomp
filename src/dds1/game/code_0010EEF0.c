#include "common.h"

extern u64 func_00119AF8(u64);

extern u64 func_0010D428(u64);

u32 func_0010EEF0(void) {
    u64 temp_v0;

    temp_v0 = func_0010D428(0);
    func_00119B08(temp_v0);
    return 1;
}

u32 func_0010EF18(void) {
    func_00119E00();
    return 1;
}

u32 func_0010EF38(void) {
    u64 temp_v0;

    temp_v0 = func_0010D428(0);
    temp_v0 = func_00119AF8(temp_v0);
    func_0010D5F0(temp_v0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0010EEF0", func_0010EF68);

INCLUDE_ASM(const s32, "game/code_0010EEF0", func_0010F268);

INCLUDE_ASM(const s32, "game/code_0010EEF0", func_0010F2F0);

INCLUDE_ASM(const s32, "game/code_0010EEF0", func_0010F418);

INCLUDE_ASM(const s32, "game/code_0010EEF0", func_0010F4F0);

INCLUDE_ASM(const s32, "game/code_0010EEF0", func_0010F570);

void func_0010F5E8(s32 arg0) {
    s32 temp_v0;

    if (arg0 != 0) {
        temp_v0 = *(s32 *)((s32)arg0 + 0x1c);
        if (temp_v0 != 0) {
            func_002CFF98(temp_v0);
            *(u32 *)((s32)arg0 + 0x1c) = 0;
        }
    }
}

void func_0010F628(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0xc0) = *(u32 *)(arg0 + 0xc0) | arg1;
}

void func_0010F638(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0xc0) = *(u32 *)(arg0 + 0xc0) & ~arg1;
}

u8 func_0010F650(s32 arg0, u32 arg1) {
    return (*(u32 *)(arg0 + 0xc0) & arg1) != 0;
}

INCLUDE_ASM(const s32, "game/code_0010EEF0", func_0010F660);

INCLUDE_ASM(const s32, "game/code_0010EEF0", func_0010F688);

INCLUDE_ASM(const s32, "game/code_0010EEF0", func_0010F6C0);

INCLUDE_ASM(const s32, "game/code_0010EEF0", func_0010F6D0);

INCLUDE_ASM(const s32, "game/code_0010EEF0", func_0010F6E0);

INCLUDE_ASM(const s32, "game/code_0010EEF0", func_0010F710);

INCLUDE_ASM(const s32, "game/code_0010EEF0", func_0010F740);

INCLUDE_ASM(const s32, "game/code_0010EEF0", func_0010F770);

INCLUDE_ASM(const s32, "game/code_0010EEF0", func_0010F788);

INCLUDE_ASM(const s32, "game/code_0010EEF0", func_0010F7A8);

INCLUDE_ASM(const s32, "game/code_0010EEF0", func_0010F7C0);

INCLUDE_ASM(const s32, "game/code_0010EEF0", func_0010F7F8);

INCLUDE_ASM(const s32, "game/code_0010EEF0", func_0010F848);
