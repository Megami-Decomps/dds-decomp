#include "common.h"

typedef struct BattleEffect {
    u8 pad0[0x10];
    u32 value10;
    u32 value14;
    u32 value18;
    u16 value1C;
    u8 pad1E[0xFA];
    u32 value118;
    u32 value11C;
    u32 value120;
} BattleEffect;

INCLUDE_ASM(const s32, "effect/effBattle", func_001686F0);

u16 func_00168780(BattleEffect *effect) {
    return effect->value1C;
}

u32 func_00168788(BattleEffect *effect) {
    return effect->value10;
}

u32 func_00168790(u32 *arg0) {
    return *arg0;
}

void func_00168798(BattleEffect *effect, u32 value) {
    effect->value118 = value;
}

void func_001687A0(BattleEffect *effect, u32 value) {
    effect->value11C = value;
}

INCLUDE_ASM(const s32, "effect/effBattle", func_001687A8);

INCLUDE_ASM(const s32, "effect/effBattle", func_001687B8);

u32 func_001687D0(BattleEffect *effect) {
    return effect->value14;
}

INCLUDE_ASM(const s32, "effect/effBattle", func_001687D8);

u32 func_00168808(BattleEffect *effect) {
    return effect->value18;
}

void func_00168810(BattleEffect *effect, u32 value) {
    effect->value120 = value;
}

u32 func_00168818(BattleEffect *effect) {
    return effect->value120;
}

INCLUDE_ASM(const s32, "effect/effBattle", func_00168820);

INCLUDE_RODATA(const s32, "effect/effBattle", D_004144A0);

INCLUDE_RODATA(const s32, "effect/effBattle", D_004144B0);

INCLUDE_RODATA(const s32, "effect/effBattle", D_004144C0);

INCLUDE_ASM(const s32, "effect/effBattle", func_00168978);

INCLUDE_ASM(const s32, "effect/effBattle", func_00169168);

INCLUDE_ASM(const s32, "effect/effBattle", func_001691E0);

INCLUDE_ASM(const s32, "effect/effBattle", func_00169230);

INCLUDE_ASM(const s32, "effect/effBattle", func_00169370);
