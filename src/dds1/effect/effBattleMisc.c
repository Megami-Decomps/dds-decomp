#include "common.h"

extern s32 func_00161860(void);

extern s32 func_00161858(void);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00161AA0);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00161AE0);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00161B10);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00161B40);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00161B70);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00161BA0);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00161E48);

void func_00162028(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = func_00161858();
    func_00161E48(*(u32 *)(temp_v0 + 0x110) & 0xe00, arg1);
}

void func_00162058(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = func_00161860();
    func_00161E48(*(u32 *)(temp_v0 + 0x110) & 0xe00, arg1);
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00162088);

void func_00162258(u32 arg0, s32 arg1) {
    func_001D63E8(arg0, *(u8 *)(arg1 + 1));
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00162270);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00162338);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_001623C0);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_001624B8);
