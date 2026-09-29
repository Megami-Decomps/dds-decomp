#include "common.h"

extern s32 func_00169440(void);

extern s32 func_00169438(void);

extern u32 D_003AB050[];

extern u32 func_00169448(void);

extern void func_001697D0();

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_001696D0);

void effBattleMiscCallByOwnerA(u32 unused, void *arg) {
    func_001697D0(func_00169448(), arg);
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00169740);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00169770);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_001697A0);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_001697D0);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00169A78);

void func_00169C58(u32 unused, u32 value) {
    s32 actor;

    actor = func_00169438();
    func_00169A78(*(u32 *)(actor + 0x110) & 0xe00, value);
}

void func_00169C88(u32 unused, u32 value) {
    s32 actor;

    actor = func_00169440();
    func_00169A78(*(u32 *)(actor + 0x110) & 0xe00, value);
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00169CB8);

void effBattleMiscApplyParamByte(u32 arg0, s32 arg1) {
    func_001E31F0(arg0, *(u8 *)(arg1 + 1));
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00169EA0);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00169F68);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00169FF0);

u32 effBattleMiscGetTableEntry(s32 arg0) {
    return D_003AB050[arg0];
}
