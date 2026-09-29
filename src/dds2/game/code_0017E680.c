#include "common.h"

typedef struct EffResourceEntry {
    f32 position[3];
    u8 pad0C[4];
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

void effReleaseAttachedResources(u32 address) {
    EffResourceWork *effect = (EffResourceWork *)address;
    sdfQueueAssetRelease(effect->graphics6C);
    effReleaseOptionalResource(address);
    func_003297C8(effect->resource70);
}

INCLUDE_ASM(const s32, "game/code_0017E680", func_0017E7E0);

INCLUDE_ASM(const s32, "game/code_0017E680", func_0017EA78);

void effReleaseOptionalResource(s32 address) {
    EffResourceWork *effect = (EffResourceWork *)address;
    if (effect->resource68 != 0) {
        func_003297C8(effect->resource68);
        return;
    }
}

void effSetResourceEntryPosition(EffResourceWork *effect, s32 index, f32 *vec) {
    f32 *dst = (f32 *)(index * 0x14 + (s32)effect->entries);
    dst[0] = vec[0];
    dst[1] = vec[1];
    dst[2] = vec[2];
}

void effGetResourceEntryPosition(EffResourceWork *effect, s32 index, f32 *vec) {
    f32 *src = (f32 *)(index * 0x14 + (s32)effect->entries);
    vec[0] = src[0];
    vec[1] = src[1];
    vec[2] = src[2];
}

void effSetResourceEntryValue(EffResourceWork *effect, s32 index, u32 value) {
    effect->entries[index].value = value;
}

INCLUDE_ASM(const s32, "game/code_0017E680", func_0017ED50);

INCLUDE_ASM(const s32, "game/code_0017E680", func_0017ED78);
