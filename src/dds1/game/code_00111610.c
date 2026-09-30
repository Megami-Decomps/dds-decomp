#include "common.h"

typedef struct ObjectResource {
    u32 owner;      /* 0x00: object passed to the slot constructor */
    u32 handle;     /* 0x04: resource released before replacement */
    u32 value;      /* 0x08 */
    u32 resourceId; /* 0x0C: used to construct the replacement */
} ObjectResource;

typedef struct ObjectWithResource {
    u8 pad0[0x18];
    ObjectResource *resource;
} ObjectWithResource;

extern u32 func_00116D38(u32);

typedef struct {
    u8 unk0[0x10];
} SlotEntry;

typedef struct {
    u8 unk0[4];        /* 0x0 */
    u32 unk4;          /* 0x4 */
    SlotEntry *entry;     /* 0x8: slot from D_00329A68 */
    u8 unkC[0xC];      /* 0xC */
    ObjectResource *resource; /* 0x18 */
} SlotObjectFull;

extern SlotObjectFull *func_00110880();
extern u32 dds3AdvanceWorldCounter();
extern SlotEntry D_00329A68[];
extern s32 D_003BA9C0;

SlotObjectFull *dds3SpawnSlotRingObj3(u32 owner) {
    SlotObjectFull *obj = func_00110880(3);
    ObjectResource *resource = obj->resource;
    u32 sequence = dds3AdvanceWorldCounter();
    s32 slot;

    resource->owner = owner;
    slot = D_003BA9C0;
    obj->unk4 = sequence;
    obj->entry = &D_00329A68[slot];
    D_003BA9C0 = slot + 1;
    D_003BA9C0 = D_003BA9C0 % 10;
    return obj;
}

void dds3SetSlotValue(ObjectWithResource *object, u32 value) {
    object->resource->value = value;
}

void dds3SetSlotKey(ObjectWithResource *object, u32 resourceId) {
    object->resource->resourceId = resourceId;
}

/* Keep the old handle until it has been freed; the new resource is keyed
 * by resourceId rather than by the previous handle. */
void dds3ReplaceObjectResource(ObjectWithResource *object) {
    ObjectResource *resource;
    u32 handle;

    resource = object->resource;
    if (resource->handle != 0) {
        dds3FreePathObject(resource->handle);
    }
    handle = func_00116D38(resource->resourceId);
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

void func_00111740(void) {
    func_00111478();
}

INCLUDE_ASM(const s32, "game/code_00111610", func_00111758);

INCLUDE_ASM(const s32, "game/code_00111610", func_001117A8);

INCLUDE_SDATA(const s32, "game/code_00111610", D_003BA9C0);

