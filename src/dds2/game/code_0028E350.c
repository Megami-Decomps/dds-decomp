#include "common.h"

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028E350);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028E568);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028E638);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427218);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427258);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028E858);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028EB38);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427278);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_004272C8);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028EF50);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028F128);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_004272F8);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427330);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427340);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427360);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427380);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028F380);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028F570);

s32 func_0028F770(s32 object, s32 state) {
    s32 current = *(s32 *)(*(s32 *)(object + 4) + 0x10);
    s32 index = 0;
    while (current != 0) {
        if (*(u16 *)(*(s32 *)(current + 0x70) + 4) == *(s16 *)(state + 6)) {
            *(s8 *)(state + 5) = index;
            return 0;
        }
        current = *(s32 *)(current + 0x58);
        index++;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427428);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028F7B8);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028F8A8);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427488);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_004274B0);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028F9A0);

u32 func_0028FD08(void) {
    return 0;
}

void func_0028FD10(u32 arg0) {
    func_0028FD30(arg0, 0xffffffffffffffff, 0xffffffffffffffff);
}

