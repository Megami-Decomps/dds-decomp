#include "common.h"

extern u32 effPcpScatterResCreate(u32);

extern u32 effPcpScatterResAddRef(u32);

typedef struct ScatterObject {
    u8 pad00[0x5C];
    s32 stride;
    u8 pad60[4];
    s32 wideBlocks;
    s32 narrowBlocks;
    u8 pad6C[4];
    u32 *entries;
    u32 graphics;
    u32 allocation;
    u32 sharedResource;
    u8 pad80[0xAC];
    f32 value12C;
    u32 value130;
} ScatterObject;

INCLUDE_ASM(const s32, "game/code_0017D758", func_0017D758);

void func_0017D770(ScatterObject *object, f32 value) {
    object->value12C = value;
}

void func_0017D778(ScatterObject *object, u32 value) {
    object->value130 = value;
}

INCLUDE_ASM(const s32, "game/code_0017D758", func_0017D780);

INCLUDE_ASM(const s32, "game/code_0017D758", func_0017D7A8);

void effReleaseScatterObject(ScatterObject *object) {
    if (object->sharedResource != 0) {
        effPcpScatterResRelease(object->sharedResource);
    }
    sdfQueueAssetRelease(object->graphics);
    func_003297C8(object->allocation);
    func_00328E48(object);
}

INCLUDE_ASM(const s32, "game/code_0017D758", func_0017DA28);

void effCreateScatterResource(ScatterObject *object, u32 resource) {
    u32 shared;

    shared = effPcpScatterResCreate(resource);
    object->sharedResource = shared;
}

void effShareScatterResource(ScatterObject *object, ScatterObject *source) {
    u32 shared;

    shared = effPcpScatterResAddRef(source->sharedResource);
    object->sharedResource = shared;
}

s32 effGetScatterWideBlock(ScatterObject *object, s32 index) {
    return object->wideBlocks + index * object->stride * 0x10;
}

s32 effGetScatterNarrowBlock(ScatterObject *object, s32 index) {
    return object->narrowBlocks + index * object->stride * 8;
}

u32 effGetScatterEntry(ScatterObject *object, s32 index) {
    return object->entries[index];
}

INCLUDE_ASM(const s32, "game/code_0017D758", func_0017DD20);

INCLUDE_ASM(const s32, "game/code_0017D758", func_0017DD50);
