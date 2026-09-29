#include "common.h"
#include "pcp_vu0.h"

typedef struct {
    u8 pad0[0x5C];
    s32 stride;
    u8 pad60[4];
    s32 *wideBlocks;
    s32 *narrowBlocks;
    u8 pad6C[4];
    u32 *entries;
    u32 graphics;
    u32 allocation;
    u32 sharedResource;
    u8 pad80[0xAC];
    f32 value12C;
    u32 value130;
} ScatterObject;

extern u32 effPcpScatterResAddRef(u32);

extern u32 effPcpScatterResCreate(u32);

void func_00175B00(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x40, src);
}

/* Set the float parameter stored just before the scatter object's final word. */
void func_00175B18(ScatterObject *object, f32 value) {
    object->value12C = value;
}

void func_00175B20(ScatterObject *object, u32 value) {
    object->value130 = value;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void func_00175B28(void *dst, void *src) {
    VU0_COPY_MATRIX(dst, src);
}

INCLUDE_ASM(const s32, "game/code_00175B00", func_00175B50);

/* Release the shared scatter resource before the object's private assets. */
void effReleaseScatterObject(ScatterObject *object) {
    ScatterObject *current;

    current = object;
    if (current->sharedResource != 0) {
        effPcpScatterResRelease(current->sharedResource);
    }
    sdfQueueAssetRelease(current->graphics);
    func_002D0918(current->allocation);
    func_002CFF98(object);
}

INCLUDE_ASM(const s32, "game/code_00175B00", func_00175DD0);

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
    return (s32)object->wideBlocks + index * object->stride * 0x10;
}

/* Compute the address of an 8-byte-wide block within the stride. */
s32 effGetScatterNarrowBlock(ScatterObject *object, s32 index) {
    return (s32)object->narrowBlocks + index * object->stride * 8;
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
