#include "common.h"

extern u32 func_00116FA0(u32);

typedef struct ObjectResource {
    u32 owner;      /* 0x00: object that owns this resource */
    u32 handle;     /* 0x04 */
    u32 value;      /* 0x08 */
    u32 resourceId; /* 0x0C */
} ObjectResource;

typedef struct ObjectWithResource {
    u8 pad0[0x18];
    ObjectResource *resource;
} ObjectWithResource;

INCLUDE_ASM(const s32, "game/code_00111838", dds3SpawnSlotRingObj3);

void dds3SetSlotValue(ObjectWithResource *object, u32 value) {
    object->resource->value = value;
}

void dds3SetSlotKey(ObjectWithResource *object, u32 resourceId) {
    object->resource->resourceId = resourceId;
}

/* The resource ID determines the replacement; releasing the old handle
 * does not change the ID stored in the object. */
void dds3ReplaceObjectResource(ObjectWithResource *object) {
    ObjectResource *resource;
    u32 handle;

    resource = object->resource;
    if (resource->handle != 0) {
        dds3FreePathObject(resource->handle);
    }
    handle = func_00116FA0(resource->resourceId);
    resource->handle = handle;
}

void dds3ReleaseObjectResource(ObjectWithResource *object) {
    ObjectResource *resource;
    s32 handle;

    resource = object->resource;
    handle = resource->handle;
    if (handle != 0) {
        dds3FreePathObject(handle);
        resource->handle = 0;
    }
}

u32 dds3GetObjectResourceHandle(ObjectWithResource *object) {
    return object->resource->handle;
}

void func_00111968(void) {
    func_001116A0();
}

INCLUDE_ASM(const s32, "game/code_00111838", func_00111980);

INCLUDE_ASM(const s32, "game/code_00111838", func_001119D0);

INCLUDE_SDATA(const s32, "game/code_00111838", D_00435D90);

