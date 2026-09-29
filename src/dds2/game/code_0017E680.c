#include "common.h"

typedef struct EffResourceEntry {
    u8 pad00[0x10];
    u32 value;
} EffResourceEntry;

typedef struct EffResourceWork {
    u8 pad00[0x40];
    EffResourceEntry *entries;
    u8 pad44[0x1C];
    u32 value60;
    u8 pad64[4];
    u32 resource68;
    u32 graphics6C;
    u32 resource70;
} EffResourceWork;

void func_0017E680(EffResourceWork *effect, u32 value) {
    effect->value60 = value;
}

INCLUDE_ASM(const s32, "game/code_0017E680", func_0017E688);

void func_0017E7A8(u32 address) {
    EffResourceWork *effect = (EffResourceWork *)address;
    func_00333918(effect->graphics6C);
    func_0017ECA0(address);
    func_003297C8(effect->resource70);
}

INCLUDE_ASM(const s32, "game/code_0017E680", func_0017E7E0);

INCLUDE_ASM(const s32, "game/code_0017E680", func_0017EA78);

void func_0017ECA0(s32 address) {
    EffResourceWork *effect = (EffResourceWork *)address;
    if (effect->resource68 != 0) {
        func_003297C8(effect->resource68);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0017E680", func_0017ECD0);

INCLUDE_ASM(const s32, "game/code_0017E680", func_0017ED00);

void func_0017ED30(EffResourceWork *effect, s32 index, u32 value) {
    effect->entries[index].value = value;
}

INCLUDE_ASM(const s32, "game/code_0017E680", func_0017ED50);

INCLUDE_ASM(const s32, "game/code_0017E680", func_0017ED78);
