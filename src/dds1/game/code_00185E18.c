#include "common.h"

typedef struct {
    u8 pad0[0x70];
    u32 firstResource;
    u32 secondResource;
    u8 pad78[4];
    u32 value7C;
} EffectPair;

void effFreePairedResources(EffectPair *pair) {
    func_00167350(pair->secondResource);
    func_00167350(pair->firstResource);
    func_002CFF98(pair);
}

INCLUDE_ASM(const s32, "game/code_00185E18", func_00185E50);

INCLUDE_ASM(const s32, "game/code_00185E18", func_00186030);

void func_00186040(EffectPair *pair, u32 value) {
    pair->value7C = value;
}

INCLUDE_ASM(const s32, "game/code_00185E18", func_00186048);

INCLUDE_ASM(const s32, "game/code_00185E18", func_00186398);

INCLUDE_ASM(const s32, "game/code_00185E18", func_00186498);

INCLUDE_ASM(const s32, "game/code_00185E18", func_00186718);

INCLUDE_ASM(const s32, "game/code_00185E18", func_001869F0);

INCLUDE_ASM(const s32, "game/code_00185E18", func_00186A98);

INCLUDE_ASM(const s32, "game/code_00185E18", func_00186B20);

INCLUDE_ASM(const s32, "game/code_00185E18", func_00186BC8);

INCLUDE_ASM(const s32, "game/code_00185E18", func_00186C18);

void func_00186CB8(void) {
    func_002CFF98();
}

INCLUDE_ASM(const s32, "game/code_00185E18", func_00186CD0);

INCLUDE_ASM(const s32, "game/code_00185E18", func_00186D48);
