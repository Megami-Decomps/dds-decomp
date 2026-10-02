#include "common.h"

extern u32 func_00116FA0(u32);

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

extern SlotObjectFull *dds3AppendWorldObjectNode();

extern u32 dds3AdvanceWorldCounter();

extern SlotEntry dds3SlotRingEntries[];

extern s32 dds3SlotRingCursor;

typedef struct DdsSlotResourceState {
    u32 unk00;
    u32 indexNode;
    s32 kind;
    u32 unk0C;
    s32 unk10;
    u8 pad14[0x38];
    s32 slots[24];
    u32 unkAC;
    s32 currentSlot;
} DdsSlotResourceState;

extern void *func_00328D68(s32);
extern void *memset(void *, s32, u32);
extern u32 dds3AppendWorldIndexNode(s32);

SlotObjectFull *dds3SpawnSlotRingObj3(u32 owner) {
    SlotObjectFull *obj = dds3AppendWorldObjectNode(3);
    ObjectResource *resource = obj->resource;
    u32 sequence = dds3AdvanceWorldCounter();
    s32 slot;

    resource->owner = owner;
    slot = dds3SlotRingCursor;
    obj->unk4 = sequence;
    obj->entry = &dds3SlotRingEntries[slot];
    dds3SlotRingCursor = slot + 1;
    dds3SlotRingCursor = dds3SlotRingCursor % 10;
    return obj;
}

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
    dds3UpdateMoverTransform();
}

/* Number of slots a given object kind occupies in the slot ring. */
s32 dds3GetObjectSlotRingOccupancy(u32 kind)
{
    s32 slots = 0;

    switch (kind - 2) {
    case 1:
        slots = 1;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        break;
    case 8:
        slots = 3;
        break;
    case 10:
        slots = 3;
        break;
    case 11:
        slots = 3;
        break;
    case 9:
        slots = 4;
        break;
    case 0:
        slots = 5;
        break;
    }
    return slots;
}

DdsSlotResourceState *dds3CreateSlotResourceState(s32 value) {
    DdsSlotResourceState *state = func_00328D68(sizeof(DdsSlotResourceState));
    s32 i;
    u32 node;

    memset(state, 0, sizeof(DdsSlotResourceState));
    state->kind = 3;
    node = dds3AppendWorldIndexNode(0);
    state->indexNode = node;
    state->unk10 = value;
    for (i = 0; i < 24; i++) {
        state->slots[i] = -1;
    }
    state->currentSlot = -1;
    return state;
}

INCLUDE_SDATA(const s32, "game/code_00111838", dds3SlotRingCursor);

