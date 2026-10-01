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

extern char D_00414478[];
extern void func_0016A620(s32);
extern void func_0035B6E0(char *, u16, void *, s32);
extern void sndUnlinkVoice(void *);
extern void sdfReleaseChipBlock(void *);

/* Only the value read through the effect's source pointer is known. */
typedef struct BattleEffectValueSource {
    u8 pad00[0x48];
    u32 value;
} BattleEffectValueSource;

typedef struct BattleEffect {
    u8 pad0[0x10];
    u32 value10;
    u32 inputValue;    /* 0x14 */
    u32 selectedValue; /* 0x18 */
    u16 mode;          /* 0x1C: 1 keeps the smaller selected value */
    u8 pad1E[6];
    BattleEffectValueSource *valueSource; /* 0x24 */
    u8 pad28[0xF0];
    u32 value118;
    u32 value11C;
    u32 value120;
} BattleEffect;

void func_001686F0(EffBattleVoiceOwner *owner) {
    s32 i;

    for (i = 0; i < 0x3C; i++) {
        if (owner->voices[i] != 0) {
            func_0016A620(owner->voices[i]);
        }
    }
    sndUnlinkVoice(owner);
    owner->state->activeCount--;
    func_0035B6E0(D_00414478, owner->voiceKind, owner->state,
                  owner->state->activeCount);
    sdfReleaseChipBlock(owner);
}

u16 effBattleGetMode(BattleEffect *effect) {
    return effect->mode;
}

u32 func_00168788(BattleEffect *effect) {
    return effect->value10;
}

u32 func_00168790(u32 *value) {
    return *value;
}

void func_00168798(BattleEffect *effect, u32 value) {
    effect->value118 = value;
}

void func_001687A0(BattleEffect *effect, u32 value) {
    effect->value11C = value;
}

u32 func_001687A8(BattleEffect *effect) {
    return effect->valueSource->value;
}

void effBattleSetInputValue(BattleEffect *effect, s32 value) {
    effect->inputValue = value;
    if (effect->mode == 0) {
        effect->selectedValue = value;
    }
}

u32 effBattleGetInputValue(BattleEffect *effect) {
    return effect->inputValue;
}

/* Mode 1 keeps the lowest value seen; other modes replace it outright. */
void effBattleUpdateSelectedValue(BattleEffect *effect, u32 value) {
    if (effect->mode == 1) {
        if (value < effect->selectedValue) {
            effect->selectedValue = value;
        }
        return;
    }
    effect->selectedValue = value;
}

u32 effBattleGetSelectedValue(BattleEffect *effect) {
    return effect->selectedValue;
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
