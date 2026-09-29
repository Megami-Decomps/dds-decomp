#include "common.h"

extern u64 effParamTableGetBlock(u64, u64);
typedef struct EffectResourceWork {
    u8 pad00[0x20];
    u32 resourceHandle;
    u32 allocation;
} EffectResourceWork;

typedef struct EffectColorState {
    u32 color;
    u8 pad04[8];
    u32 valueC;
    u32 mode;
} EffectColorState;


INCLUDE_ASM(const s32, "game/code_0016EDD0", func_0016EDD0);

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_0016EFA8);

u32 func_0016F018(u32 arg0) {
    return arg0;
}

void func_0016F020(s32 work, u32 value) {
    *(u32 *)(work + 0x120) = value;
}

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_0016F028);

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_0016F6D0);

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_0016F850);

void func_0016FAD0(u64 table) {
    u64 firstBlock;
    u64 secondBlock;

    firstBlock = effParamTableGetBlock(table, 0);
    secondBlock = effParamTableGetBlock(table, 1);
    func_0016F850(firstBlock, secondBlock);
}

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_0016FB18);

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_0016FD90);

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_0016FE18);

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_00171580);

void func_00171590(s32 work, u32 value) {
    *(u32 *)(work + 0x54) = value;
}

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_00171598);

void func_00171798(EffectResourceWork *work) {
    func_00333918(work->resourceHandle);
    func_003297C8(work->allocation);
}

void func_001717C8(EffectColorState *state) {
    state->mode = 3;
    state->color = 0x80808080;
    state->valueC = 0;
}

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_001717E8);

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_001719D0);

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_00171A68);

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_00171CE0);
