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

typedef struct {
    s32 count;
    u16 historyCount;
    u8 pad06[2];
    f32 unk08;
    f32 unk0C;
    u32 unk10;
    f32 (*positions)[4];
    u32 *colorTable;
    f32 *unk1C;
    u32 *values;
    u16 *writeIndices;
    u16 *validCounts;
    f32 *angleValues; /* Four floats per indexed row. */
    u32 texture;
    void *resource;
} EffMagatuhiValueWork;

extern void *sdfAllocSizeClassBlock(s32 size);
extern void *sdfAllocGeneralBlock(s32 size);
extern s32 sdfResourceRetainAddress(void *resource);
extern u32 effGetResourceFirstWord(s32 index);
extern s32 func_00189220(s32, s32, f32, s32, f32);
extern void effMagatuhiFillColorTable(s32, s32, s32);
extern s16 D_003D6670[];

void *effCloneMagatuhiWithColorResource(MagatuhiEffectData *source) {
    MagatuhiEffectData *effect;

    effect = (MagatuhiEffectData *)sdfAllocSizeClassBlock(0x20);
    memcpy(effect, source, 0x1C);
    effect->field1C = func_00189220(effect->field00, effect->field04, effect->field08, effect->field14, effect->field18);
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

void func_001891C0(EffMagatuhiValueWork *work, s32 index) {
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

s32 func_00189220(s32 count, s32 frames, f32 param08, s32 param, f32 param0C) {
    s32 countFrames;
    s32 frameTerm;
    s32 countTerm;
    s32 allocationSize;
    void *allocation;
    s32 positions;
    s32 colorTable;
    s32 unknownValues;
    s32 values;
    s32 writeIndices;
    s32 validCounts;
    s32 angleValues;
    EffMagatuhiValueWork *resource;
    s16 *defaults;
    u32 i;

    countFrames = count * frames;
    frameTerm = countFrames + frames;
    countTerm = (countFrames << 2) + count;
    allocationSize = (((count << 3) + ((countTerm + frameTerm) << 1) + (count << 1)) << 1) + 0x38;
    allocation = sdfAllocGeneralBlock(allocationSize);
    positions = sdfResourceRetainAddress(allocation);
    colorTable = positions + (countFrames << 4);
    unknownValues = colorTable + (frames << 2);
    values = unknownValues + (countFrames << 2);
    writeIndices = values + (count << 2);
    validCounts = writeIndices + (count << 1);
    angleValues = validCounts + (count << 1);
    resource = (EffMagatuhiValueWork *)(angleValues + (count << 4));

    resource->count = count;
    resource->historyCount = frames;
    resource->unk0C = param0C;
    resource->unk08 = param08;
    resource->unk10 = param;
    resource->positions = (f32 (*)[4])positions;
    resource->colorTable = (u32 *)colorTable;
    resource->unk1C = (f32 *)unknownValues;
    resource->values = (u32 *)values;
    resource->writeIndices = (u16 *)writeIndices;
    resource->validCounts = (u16 *)validCounts;
    resource->angleValues = (f32 *)angleValues;
    resource->resource = allocation;
    resource->texture = effGetResourceFirstWord(0);

    defaults = D_003D6670;
    i = 0;
    do {
        i++;
        defaults[0] = 0;
        defaults[1] = 0;
        defaults[2] = 0x400;
        defaults[3] = 0;
        defaults[4] = 0x400;
        defaults[5] = 0x400;
        defaults[6] = 0;
        defaults[7] = 0x400;
        defaults += 8;
    } while (i < 0xF);

    if (count != 0) {
        i = 0;
        do {
            func_001891C0(resource, i);
            i++;
        } while (i < count);
    }
    return (s32)resource;
}
