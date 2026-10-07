#include "eff.h"


extern void *sdfAllocSizeClassBlock(s32 size);
extern void *sdfAllocGeneralBlock(s32 size);
extern s32 sdfResourceRetainAddress(void *resource);
extern s32 effGetResourceFirstWord(s32 index);
extern s16 D_00452110[];

EffMagatuhiOwner *effCloneMagatuhiWithColorResource(const u32 *source) {
    EffMagatuhiOwner *effect;

    effect = sdfAllocSizeClassBlock(sizeof(*effect));
    memcpy(&effect->params, source, sizeof(effect->params));
    effect->valueWork = func_00190E58(effect->params.count, effect->params.historyCount, effect->params.unk08, effect->params.unk14, effect->params.unk18);
    effMagatuhiFillColorTable(effect->valueWork, effect->params.colorA, effect->params.colorB);
    return effect;
}

void effReleaseMagatuhiOwner(EffMagatuhiOwner *effect) {
    effMagatuhiReleaseResource(effect->valueWork);
    sdfReleaseChipBlock(effect);
}

void func_00190DE0(EffMagatuhiOwner *effect) {
    func_00191010(effect->valueWork);
}

void func_00190DF8(EffMagatuhiValueWork *work, s32 index) {
    f32 *angles;

    work->writeIndices[index] = 0;
    work->validCounts[index] = 0;
    work->slotColors[index] = 0x80808080;
    angles = work->angleRows[index];
    angles[0] = 6.2831853f;
    angles[1] = 0.0f;
    angles[2] = 3.1415926f;
    angles[3] = 0.0f;
}

EffMagatuhiValueWork *func_00190E58(s32 count, s32 frames, f32 param08, s32 param, f32 param0C) {
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
    allocationSize = (((count << 3) + ((countTerm + frameTerm) << 1) + (count << 1)) << 1) + sizeof(EffMagatuhiValueWork);
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
    resource->slotColors = (u32 *)values;
    resource->writeIndices = (u16 *)writeIndices;
    resource->validCounts = (u16 *)validCounts;
    resource->angleRows = (f32 (*)[4])angleValues;
    resource->allocationHandle = allocation;
    resource->texture = (SdfTex *)effGetResourceFirstWord(0);

    defaults = D_00452110;
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
            func_00190DF8(resource, i);
            i++;
        } while (i < count);
    }
    return resource;
}
