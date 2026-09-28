#include "common.h"

extern s32 func_002CFEB8(u32);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00254B30);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00254C30);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00254C68);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00254E48);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00254EF0);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255010);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255118);

INCLUDE_ASM(const s32, "game/code_00254B30", func_002551B8);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255368);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255508);

INCLUDE_ASM(const s32, "game/code_00254B30", func_002555E8);

INCLUDE_ASM(const s32, "game/code_00254B30", func_002556C8);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255798);

INCLUDE_ASM(const s32, "game/code_00254B30", func_002557B8);

INCLUDE_ASM(const s32, "game/code_00254B30", func_002557D8);

INCLUDE_ASM(const s32, "game/code_00254B30", func_002557F8);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255818);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255838);

INCLUDE_ASM(const s32, "game/code_00254B30", func_002559F8);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255A98);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255B78);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255D00);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255E08);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255EF8);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255F78);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255FF8);

INCLUDE_ASM(const s32, "game/code_00254B30", func_002560B8);

INCLUDE_ASM(const s32, "game/code_00254B30", func_002561A0);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00256290);

INCLUDE_ASM(const s32, "game/code_00254B30", func_002562E8);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00256400);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00256540);

s32 *func_00256B38(void) {
    s32 *temp_v0 = (s32 *)func_002CFEB8(0x14);

    memset(temp_v0, 0, 0x14);
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_00254B30", func_00256B78);

u32 func_00256C00(s32 arg0) {
    u32 temp_v0;

    temp_v0 = *(u32 *)(arg0 + 0x10);
    func_002CFF98();
    return temp_v0;
}

void func_00256C28(s32 *arg0) {
    s32 temp_v0 = arg0[2];

    while (temp_v0 != NULL) {
        temp_v0 = func_00256C00(temp_v0);
    }
}

INCLUDE_ASM(const s32, "game/code_00254B30", func_00256C58);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00256D10);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00256D90);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00256E10);

void func_00256E88(void) {
}

INCLUDE_ASM(const s32, "game/code_00254B30", func_00256E90);

INCLUDE_ASM(const s32, "game/code_00254B30", func_002570A8);
