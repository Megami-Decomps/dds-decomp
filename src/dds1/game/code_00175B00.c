#include "common.h"
#include "pcp_vu0.h"

typedef struct {
    u8 pad0[0x5C];
    s32 stride;
    u8 pad60[4];
    s32 *block64;
    s32 *block68;
    u8 pad6C[4];
    u32 *entries;
    u32 handle74;
    u32 handle78;
    u32 scatterResource;
    u8 pad80[0xB0];
    u32 value130;
} ScatterObject;

extern u32 effPcpScatterResAddRef(u32);

extern u32 effPcpScatterResCreate(u32);

void func_00175B00(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x40, src);
}

void func_00175B18(u8 *work, f32 value) {
    *(f32 *)(work + 0x12C) = value;
}

void func_00175B20(ScatterObject *object, u32 value) {
    object->value130 = value;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void func_00175B28(void *dst, void *src) {
    VU0_COPY_MATRIX(dst, src);
}

INCLUDE_ASM(const s32, "game/code_00175B00", func_00175B50);

void effReleaseScatterObject(ScatterObject *object) {
    ScatterObject *current;

    current = object;
    if (current->scatterResource != 0) {
        effPcpScatterResRelease(current->scatterResource);
    }
    sdfQueueAssetRelease(current->handle74);
    func_002D0918(current->handle78);
    func_002CFF98(object);
}

INCLUDE_ASM(const s32, "game/code_00175B00", func_00175DD0);

void effCreateScatterResource(ScatterObject *object, u32 source) {
    u32 resource;

    resource = effPcpScatterResCreate(source);
    object->scatterResource = resource;
}

void effShareScatterResource(ScatterObject *object, ScatterObject *source) {
    u32 resource;

    resource = effPcpScatterResAddRef(source->scatterResource);
    object->scatterResource = resource;
}

s32 effGetScatterWideBlock(ScatterObject *object, s32 index) {
    return (s32)object->block64 + index * object->stride * 0x10;
}

s32 effGetScatterNarrowBlock(ScatterObject *object, s32 index) {
    return (s32)object->block68 + index * object->stride * 8;
}

u32 effGetScatterEntry(ScatterObject *object, s32 index) {
    return object->entries[index];
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void func_001760C8(void *work, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_STORE_MATRIX((u8 *)work + 0x10);
}

INCLUDE_ASM(const s32, "game/code_00175B00", func_001760F8);
