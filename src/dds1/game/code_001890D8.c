#include "common.h"

typedef struct {
    u8 pad0[0x1C];
    u32 handle;
} SceneResource;

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

extern void *sdfAllocSizeClassBlock(s32 size);
extern s32 func_00189220(s32, s32, s32, f32, f32);
extern void effMagatuhiFillColorTable(s32, s32, s32);

void *effCloneMagatuhiWithColorResource(MagatuhiEffectData *source) {
    MagatuhiEffectData *effect;

    effect = (MagatuhiEffectData *)sdfAllocSizeClassBlock(0x20);
    memcpy(effect, source, 0x1C);
    effect->field1C = func_00189220(effect->field00, effect->field04, effect->field14, effect->field08, effect->field18);
    effMagatuhiFillColorTable(effect->field1C, effect->field0C, effect->field10);
    return effect;
}

void effReleaseMagatuhiOwner(SceneResource *resource) {
    effMagatuhiReleaseResource(resource->handle);
    sdfReleaseChipBlock(resource);
}

void func_001891A8(SceneResource *resource) {
    func_001893D8(resource->handle);
}

INCLUDE_ASM(const s32, "game/code_001890D8", func_001891C0);

INCLUDE_ASM(const s32, "game/code_001890D8", func_00189220);
