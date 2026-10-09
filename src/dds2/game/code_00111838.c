#include "common.h"
#include "sdf_chip.h"
#include "dds3_path.h"
#include "dds3obj.h"


/* Opaque fixed-size ring entry; this unit only selects its address. */
typedef struct {
    u8 unk0[0x10];
} SlotEntry;

#define DDS3_SLOT_RING_ENTRY_COUNT 10


extern u32 dds3AdvanceWorldCounter(void);

extern SlotEntry dds3SlotRingEntries[];

extern s32 dds3SlotRingCursor;

extern s32 dds3UpdateMoverTransform(EffWorldNode *object);

extern void *memset(void *, s32, u32);

/* Bind the owner, select a 16-byte ring entry, and advance the ten-entry cursor. */
EffWorldNode *dds3SpawnSlotRingObj3(EffWorldNode *owner) {
    EffWorldNode *object = dds3AppendWorldObjectNode(EFF_WORLD_KIND_SLOT_RING);
    Dds3SlotResource *resource = object->data;
    u32 sequence = dds3AdvanceWorldCounter();
    s32 slotIndex;

    resource->target = owner;
    slotIndex = dds3SlotRingCursor;
    object->key = sequence;
    object->value = (u32)&dds3SlotRingEntries[slotIndex];
    dds3SlotRingCursor = slotIndex + 1;
    dds3SlotRingCursor = dds3SlotRingCursor % DDS3_SLOT_RING_ENTRY_COUNT;
    return object;
}

/* Install the relative-transform callback used when no path is retained. */
void dds3SetSlotValue(EffWorldNode *object, Dds3MoverUpdate update) {
    Dds3SlotResource *resource = object->data;

    resource->update = update;
}

/* Cache the source object address for the next curve-work replacement. */
void dds3SetSlotKey(EffWorldNode *object, EffWorldNode *sourceObject) {
    Dds3SlotResource *resource = object->data;

    resource->sourceObject = sourceObject;
}

/* Free the old curve work before constructing its replacement from the stored
 * source object address. The source is not an integer resource ID. */
void dds3ReplaceObjectResource(EffWorldNode *object) {
    Dds3SlotResource *resource;
    Dds3PathCurveWork *pathWork;

    resource = object->data;
    if (resource->path != 0) {
        dds3FreePathObject(resource->path);
    }
    pathWork = dds3CreatePathCurveWork(resource->sourceObject);
    resource->path = pathWork;
}

/* Release nonzero curve work and clear its handle, retaining the source address. */
void dds3ReleaseObjectResource(EffWorldNode *object) {
    Dds3SlotResource *resource;
    Dds3PathCurveWork *pathWork;

    resource = object->data;
    pathWork = resource->path;
    if (pathWork != 0) {
        dds3FreePathObject(pathWork);
        resource->path = 0;
    }
}

/* Return the currently stored curve-work handle. */
Dds3PathCurveWork *dds3GetObjectResourceHandle(EffWorldNode *object) {
    Dds3SlotResource *resource = object->data;

    return resource->path;
}

void dds3InvokeMoverUpdate(EffWorldNode *object) {
    dds3UpdateMoverTransform(object);
}

/* Number of slots a given object kind occupies in the slot ring. */
s32 dds3SelectSlotForObjectKind(u32 kind)
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

/* Initialize a kind-3 slot state and its index node.
 * Every slot and currentSlot start unused (-1). */
ObjBase *dds3CreateSlotResourceState(void *owner) {
    ObjBase *state = sdfAllocSizeClassBlock(sizeof(ObjBase));
    s32 i;
    WorldIndexNode *node;

    memset(state, 0, sizeof(ObjBase));
    state->resourceState = 3;
    node = dds3AppendWorldIndexNode(0);
    state->worldIndexNode = (u32)node;
    state->slots[0] = owner;
    for (i = 0; i < DDS3_OBJECT_RESOURCE_SLOT_COUNT; i++) {
        state->resourceSlots[i] = -1;
    }
    state->currentSlot = -1;
    return state;
}

INCLUDE_SDATA(const s32, "game/code_00111838", dds3SlotRingCursor);

