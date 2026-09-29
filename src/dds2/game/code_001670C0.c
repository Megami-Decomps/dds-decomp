#include "common.h"

#include "eff.h"

extern BillDispatch D_003AAF88[];

extern BillDispatch D_003AAF84[];

typedef struct EffectEntry {
    u8 pad00[0x10];
    u32 value;
} EffectEntry;

typedef struct EffectDispatchState {
    u8 pad00[0x14];
    EffectEntry *entries;
    u8 pad18[4];
    u32 value1C;
    u8 pad20[0x40];
    u32 value60;
    u8 value64;
    u8 pad65[0x4D];
    u16 valueB2;
} EffectDispatchState;

typedef struct BillWork {
    u8 pad00[0x64];
    u8 currentValue;
    u8 pad65[0x4B];
    u16 pendingCount;
    u16 queuedCount;
    u8 padB4[0xC];
    u8 stagedValue;
} BillWork;

INCLUDE_ASM(const s32, "game/code_001670C0", func_001670C0);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167110);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167168);

INCLUDE_ASM(const s32, "game/code_001670C0", func_001671A0);

INCLUDE_ASM(const s32, "game/code_001670C0", func_001671D8);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167220);

u16 effBillGetQueuedCount(EffectDispatchState *effect) {
    return effect->valueB2;
}

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167268);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167278);

void func_001672D8(EffectDispatchState *effect, u32 value) {
    effect->value60 = value;
}

/* A value staged with no pending work is immediately mirrored to the active slot. */
void effBillSetWorkValue(BillWork *work, u8 value) {
    if (work->pendingCount == 0) {
        work->stagedValue = value;
    }
    work->currentValue = value;
}

u8 func_001672F8(EffectDispatchState *effect) {
    return effect->value64;
}

void func_00167300(EffectDispatchState *effect, u32 value) {
    effect->value1C = value;
}

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167308);

void effBillSetEntryValue(EffectDispatchState *effect, s32 index, u32 value) {
    effect->entries[index].value = value;
}

s32 func_00167400(EffectDispatchState *effect) {
    return (s32)effect + 0x20;
}

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167408);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167480);

INCLUDE_ASM(const s32, "game/code_001670C0", func_001675B8);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167778);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167838);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167988);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167A10);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167E00);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167EE8);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00168280);

INCLUDE_ASM(const s32, "game/code_001670C0", func_001682B0);

INCLUDE_ASM(const s32, "game/code_001670C0", func_001683F0);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00168448);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00168478);

INCLUDE_ASM(const s32, "game/code_001670C0", func_001684A8);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00168500);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00168548);

INCLUDE_RODATA(const s32, "game/code_001670C0", D_00414478);

INCLUDE_SDATA(const s32, "game/code_001670C0", D_00436408);

INCLUDE_SDATA(const s32, "game/code_001670C0", D_0043640C);

INCLUDE_SDATA(const s32, "game/code_001670C0", D_00436410);

