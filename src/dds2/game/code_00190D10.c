#include "common.h"

typedef struct MagatuhiEffectOwner {
    u8 pad00[0x1C];
    u32 resource;
} MagatuhiEffectOwner;

typedef struct MagatuhiEffectData {
    s32 field00;
    s32 field04;
    f32 field08;
    s32 field0C;
    s32 field10;
    s32 field14;
    f32 field18;
    s32 field1C;
} MagatuhiEffectData;

typedef struct {
    u8 pad00[0x20];
    u32 *values;
    u16 *writeIndices;
    u16 *validCounts;
    f32 *angleValues; /* Four floats per indexed row. */
} MagatuhiValueWork;

extern void *sdfAllocSizeClassBlock(s32 size);
extern s32 func_00190E58(s32, s32, s32, f32, f32);
extern void effMagatuhiFillColorTable(s32, s32, s32);

void *effCloneMagatuhiWithColorResource(MagatuhiEffectData *source) {
    MagatuhiEffectData *effect;

    effect = (MagatuhiEffectData *)sdfAllocSizeClassBlock(0x20);
    memcpy(effect, source, 0x1C);
    effect->field1C = func_00190E58(effect->field00, effect->field04, effect->field14, effect->field08, effect->field18);
    effMagatuhiFillColorTable(effect->field1C, effect->field0C, effect->field10);
    return effect;
}

void effReleaseMagatuhiOwner(MagatuhiEffectOwner *effect) {
    effMagatuhiReleaseResource(effect->resource);
    sdfReleaseChipBlock(effect);
}

void func_00190DE0(MagatuhiEffectOwner *effect) {
    func_00191010(effect->resource);
}

void func_00190DF8(MagatuhiValueWork *work, s32 index) {
    f32 *angles;

    work->writeIndices[index] = 0;
    work->validCounts[index] = 0;
    work->values[index] = 0x80808080;
    angles = work->angleValues;
    angles += index * 4;
    angles[0] = 6.2831853f;
    angles[1] = 0.0f;
    angles[2] = 3.1415926f;
    angles[3] = 0.0f;
}

INCLUDE_ASM(const s32, "game/code_00190D10", func_00190E58);
