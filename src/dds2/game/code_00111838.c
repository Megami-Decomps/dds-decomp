#include "common.h"
#include "dds3_path.h"
#include "dds3obj.h"

extern Dds3PathCurveWork *dds3CreatePathCurveWork(EffWorldNode *);
extern void dds3FreePathObject(Dds3PathCurveWork *);

typedef struct ObjectResource {
    u32 owner;      /* 0x00: object passed to the slot constructor */
    Dds3PathCurveWork *pathWork;          /* 0x04: constructed curve work released before replacement */
    u32 value;      /* 0x08 */
    u32 sourceObjectAddress; /* 0x0C: object supplying the replacement curve table */
} ObjectResource;

/* Opaque fixed-size ring entry; this unit only selects its address. */
typedef struct {
    u8 unk0[0x10];
} SlotEntry;

/* Shared slot-ring object prefix used by construction and resource accessors. */
typedef struct ObjectWithResource {
    u8 unk0[4];
    u32 unk4;
    SlotEntry *entry;         /* 0x08: selected dds3SlotRingEntries record */
    u8 unkC[0xC];
    ObjectResource *resource; /* 0x18 */
} ObjectWithResource;

#define DDS3_SLOT_RING_ENTRY_COUNT 10

extern ObjectWithResource *dds3AppendWorldObjectNode();

extern u32 dds3AdvanceWorldCounter(void);

extern SlotEntry dds3SlotRingEntries[];

extern s32 dds3SlotRingCursor;

extern s32 dds3UpdateMoverTransform(EffWorldNode *object);

extern void *sdfAllocSizeClassBlock(s32);
extern void *memset(void *, s32, u32);
extern NodeB *dds3AppendWorldIndexNode(s32 initialCount);

/* Bind the owner, select a 16-byte ring entry, and advance the ten-entry cursor. */
ObjectWithResource *dds3SpawnSlotRingObj3(u32 owner) {
    ObjectWithResource *object = dds3AppendWorldObjectNode(3);
    ObjectResource *resource = object->resource;
    u32 sequence = dds3AdvanceWorldCounter();
    s32 slotIndex;

    resource->owner = owner;
    slotIndex = dds3SlotRingCursor;
    object->unk4 = sequence;
    object->entry = &dds3SlotRingEntries[slotIndex];
    dds3SlotRingCursor = slotIndex + 1;
    dds3SlotRingCursor = dds3SlotRingCursor % DDS3_SLOT_RING_ENTRY_COUNT;
    return object;
}

/* Cache the opaque handler value without interpreting its bits. */
void dds3SetSlotValue(ObjectWithResource *object, u32 value) {
    object->resource->value = value;
}

/* Cache the source object address for the next curve-work replacement. */
void dds3SetSlotKey(ObjectWithResource *object, u32 sourceObjectAddress) {
    object->resource->sourceObjectAddress = sourceObjectAddress;
}

/* Free the old curve work before constructing its replacement from the stored
 * source object address. The source is not an integer resource ID. */
void dds3ReplaceObjectResource(ObjectWithResource *object) {
    ObjectResource *resource;
    Dds3PathCurveWork *pathWork;

    resource = object->resource;
    if (resource->pathWork != 0) {
        dds3FreePathObject(resource->pathWork);
    }
    pathWork = dds3CreatePathCurveWork((EffWorldNode *)resource->sourceObjectAddress);
    resource->pathWork = pathWork;
}

/* Release nonzero curve work and clear its handle, retaining the source address. */
void dds3ReleaseObjectResource(ObjectWithResource *object) {
    ObjectResource *resource;
    Dds3PathCurveWork *pathWork;

    resource = object->resource;
    pathWork = resource->pathWork;
    if (pathWork != 0) {
        dds3FreePathObject(pathWork);
        resource->pathWork = 0;
    }
}

/* Return the currently stored curve-work handle. */
Dds3PathCurveWork *dds3GetObjectResourceHandle(ObjectWithResource *object) {
    return object->resource->pathWork;
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
    NodeB *node;

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

