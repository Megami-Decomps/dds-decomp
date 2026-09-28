#include "common.h"

/* Shared work area for the battle-effect helpers in this TU. */
typedef struct {
    u8  pad_0x00[0x10]; /* 0x00 */
    u32 unk10;          /* 0x10 */
    u32 unk14;          /* 0x14 */
    u32 unk18;          /* 0x18 */
    u16 unk1C;          /* 0x1C */
    u8  pad_0x1E[0xFA]; /* 0x1E */
    u32 unk118;         /* 0x118 */
    u32 unk11C;         /* 0x11C */
    u32 unk120;         /* 0x120 */
} EffBattleWork; /* 0x124 */

INCLUDE_ASM(const s32, "effect/effBattle", func_00160B00);

u16 func_00160B90(EffBattleWork *work) {
    return work->unk1C;
}

u32 func_00160B98(EffBattleWork *work) {
    return work->unk10;
}

u32 func_00160BA0(u32 *value) {
    return *value;
}

void func_00160BA8(EffBattleWork *work, u32 value) {
    work->unk118 = value;
}

void func_00160BB0(EffBattleWork *work, u32 value) {
    work->unk11C = value;
}

INCLUDE_ASM(const s32, "effect/effBattle", func_00160BB8);

INCLUDE_ASM(const s32, "effect/effBattle", func_00160BC8);

u32 func_00160BE0(EffBattleWork *work) {
    return work->unk14;
}

INCLUDE_ASM(const s32, "effect/effBattle", func_00160BE8);

u32 func_00160C18(EffBattleWork *work) {
    return work->unk18;
}

void func_00160C20(EffBattleWork *work, u32 value) {
    work->unk120 = value;
}

u32 func_00160C28(EffBattleWork *work) {
    return work->unk120;
}

INCLUDE_ASM(const s32, "effect/effBattle", func_00160C30);

INCLUDE_RODATA(const s32, "effect/effBattle", D_003A0DE0);

INCLUDE_RODATA(const s32, "effect/effBattle", D_003A0DF0);

INCLUDE_RODATA(const s32, "effect/effBattle", D_003A0E00);

INCLUDE_ASM(const s32, "effect/effBattle", func_00160D88);

INCLUDE_ASM(const s32, "effect/effBattle", func_00161588);

INCLUDE_ASM(const s32, "effect/effBattle", func_00161600);

INCLUDE_ASM(const s32, "effect/effBattle", func_00161650);

INCLUDE_ASM(const s32, "effect/effBattle", func_00161790);

INCLUDE_SDATA(const s32, "effect/effBattle", D_003BB024);

