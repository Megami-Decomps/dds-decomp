#include "common.h"


typedef struct EffBattleVoiceState {
    u8 pad00[0xC3C];
    s32 activeCount;
} EffBattleVoiceState;

typedef struct EffBattleVoiceOwner {
    u32 unk00;
    EffBattleVoiceState *state;
    u8 pad08[0x14];
    u16 voiceKind;
    u8 pad1E[0xA];
    s32 voices[0x3C];
} EffBattleVoiceOwner;

extern char D_003A0DB8[];
extern void func_001629F0(s32);
extern void func_003003F0(char *, u16, void *, s32);
extern void sndUnlinkVoice(void *);
extern void sdfReleaseChipBlock(void *);


/* Shared work area for the battle-effect helpers in this TU. */
typedef struct BattleEffect {
    u8  pad_0x00[0x10]; /* 0x00 */
    u32 unk10;          /* 0x10 */
    u32 inputValue;     /* 0x14 */
    u32 selectedValue;  /* 0x18 */
    u16 mode;           /* 0x1C: 1 keeps the smaller selected value */
    u8  pad_0x1E[0xFA]; /* 0x1E */
    u32 unk118;         /* 0x118 */
    u32 unk11C;         /* 0x11C */
    u32 unk120;         /* 0x120 */
} BattleEffect; /* 0x124 */

void func_00160B00(EffBattleVoiceOwner *owner) {
    s32 i;

    for (i = 0; i < 0x3C; i++) {
        if (owner->voices[i] != 0) {
            func_001629F0(owner->voices[i]);
        }
    }
    sndUnlinkVoice(owner);
    owner->state->activeCount--;
    func_003003F0(D_003A0DB8, owner->voiceKind, owner->state,
                  owner->state->activeCount);
    sdfReleaseChipBlock(owner);
}

u16 effBattleGetMode(BattleEffect *work) {
    return work->mode;
}

u32 func_00160B98(BattleEffect *work) {
    return work->unk10;
}

u32 func_00160BA0(u32 *value) {
    return *value;
}

void func_00160BA8(BattleEffect *work, u32 value) {
    work->unk118 = value;
}

void func_00160BB0(BattleEffect *work, u32 value) {
    work->unk11C = value;
}

s32 func_00160BB8(u8 *obj) {
    return *(s32 *)(*(u8 **)(obj + 0x24) + 0x48);
}

void effBattleSetInputValue(BattleEffect *work, s32 value) {
    work->inputValue = value;
    if (work->mode == 0) {
        work->selectedValue = value;
    }
}

u32 effBattleGetInputValue(BattleEffect *work) {
    return work->inputValue;
}

/* In mode 1 keep the lowest value seen; other modes replace it outright. */
void effBattleUpdateSelectedValue(BattleEffect *work, u32 value) {
    if (work->mode == 1) {
        if (value < work->selectedValue) {
            work->selectedValue = value;
        }
        return;
    }
    work->selectedValue = value;
}

u32 effBattleGetSelectedValue(BattleEffect *work) {
    return work->selectedValue;
}

void func_00160C20(BattleEffect *work, u32 value) {
    work->unk120 = value;
}

u32 func_00160C28(BattleEffect *work) {
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

