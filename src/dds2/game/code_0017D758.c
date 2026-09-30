#include "common.h"
#include "pcp_vu0.h"

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

typedef struct PcpScatterWork4 PcpScatterWork4;

/* func_001730D0 */
struct PcpScatterWork4 {
    u8 pad00[0x40];
    s128 unk40;
    u8 pad50[0x12C];
    f32 unk17C;
    u32 unk180;
    u32 scatterObject;
    u32 ownedBuffer;
};

void func_0017D758(void *dst, void *src) {
    PCP_COPY_VECTOR((u8 *)dst + 0x40, src);
}

/* Store the float parameter beside the scatter object's trailing control word. */
void func_0017D770(ScatterObject *object, f32 value) {
    object->value12C = value;
}

void func_0017D778(ScatterObject *object, u32 value) {
    object->value130 = value;
}

/* vu0 routine: copy a 4x4 matrix through vf28-vf31 */
void func_0017D780(void *dst, void *src) {
    VU0_COPY_MATRIX(dst, src);
}

INCLUDE_ASM(const s32, "game/code_0017D758", func_0017D7A8);

/* Release the shared scatter resource before the object's private assets. */
void effReleaseScatterObject(ScatterObject *object) {
    if (object->sharedResource != 0) {
        effPcpScatterResRelease(object->sharedResource);
    }
    sdfQueueAssetRelease(object->graphics);
    func_003297C8(object->allocation);
    func_00328E48(object);
}

INCLUDE_ASM(const s32, "game/code_0017D758", func_0017DA28);

/* Give this object its own reference to a newly created scatter resource. */
void effCreateScatterResource(ScatterObject *object, u32 resource) {
    u32 shared;

    shared = effPcpScatterResCreate(resource);
    object->sharedResource = shared;
}

/* Share another object's scatter resource while retaining a separate reference. */
void effShareScatterResource(ScatterObject *object, ScatterObject *source) {
    u32 shared;

    shared = effPcpScatterResAddRef(source->sharedResource);
    object->sharedResource = shared;
}

/* Compute the address of a 16-byte-wide block within the stride. */
s32 effGetScatterWideBlock(ScatterObject *object, s32 index) {
    return object->wideBlocks + index * object->stride * 0x10;
}

/* Compute the address of an 8-byte-wide block within the stride. */
s32 effGetScatterNarrowBlock(ScatterObject *object, s32 index) {
    return object->narrowBlocks + index * object->stride * 8;
}

u32 effGetScatterEntry(ScatterObject *object, s32 index) {
    return object->entries[index];
}

/* vu0 routine: copy a 4x4 matrix into the destination's second slot */
void func_0017DD20(void *dst, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_STORE_MATRIX((u8 *)dst + 0x10);
}

INCLUDE_ASM(const s32, "game/code_0017D758", func_0017DD50);
