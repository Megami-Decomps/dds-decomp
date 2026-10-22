#include "sdk_ps2.h"
#include "common.h"
#include "sdf_chip.h"
#include "dds3_path.h"
#include "dds3obj.h"
#include "dds3_world_counter.h"

/* Opaque fixed-size ring entry; this unit only selects its address. */
typedef struct {
    u8 unk0[0x10];
} SlotEntry;

#define DDS3_SLOT_RING_ENTRY_COUNT 10


extern SlotEntry dds3SlotRingEntries[];
extern s32 dds3MoverCount;

extern s32 MoverBasicMainprocuder(dds3Basic_t *object);


/* Bind the owner, select a 16-byte ring entry, and advance the ten-entry cursor. */
dds3Basic_t *dds3AddMoverBasic(dds3Basic_t *owner) {
    dds3Basic_t *object = dds3AddSceneBasicData(EFF_WORLD_KIND_SLOT_RING);
    Dds3SlotResource *resource = object->data;
    u32 sequence = dds3AllocObjectId();
    s32 slotIndex;

    resource->target = owner;
    slotIndex = dds3MoverCount;
    object->key = sequence;
    object->value = (const char *)&dds3SlotRingEntries[slotIndex];
    dds3MoverCount = slotIndex + 1;
    dds3MoverCount = dds3MoverCount % DDS3_SLOT_RING_ENTRY_COUNT;
    return object;
}

/* Install the relative-transform callback used when no path is retained. */
void dds3MoverBasicSetCallBackPrimaryControl(dds3Basic_t *slotObject, Dds3MoverUpdate update) {
    Dds3SlotResource *resource = slotObject->data;

    resource->update = update;
}

/* Cache the source object address for the next curve-work replacement. */
/* Cache the source address consumed by the slot's later path replacement. */
void dds3MoverBasicSetPath(dds3Basic_t *slotObject, dds3Basic_t *sourceObject) {
    Dds3SlotResource *resource = slotObject->data;

    resource->sourceObject = sourceObject;
}

/* Free the old curve work before constructing its replacement from the stored
 * source object address. The source is not an integer resource ID. */
void dds3MoverBasicMakePath(dds3Basic_t *slotObject) {
    Dds3SlotResource *resource;
    dds3PathBasicControl_t *pathWork;

    resource = slotObject->data;
    if (resource->path != 0) {
        dds3DeletePathControl(resource->path);
    }
    pathWork = dds3AllocPathBasicWorkData(resource->sourceObject);
    resource->path = pathWork;
}

/* Release nonzero curve work and clear its handle, retaining the source address. */
void dds3MoverBasicFreePath(dds3Basic_t *slotObject) {
    Dds3SlotResource *resource;
    dds3PathBasicControl_t *pathWork;

    resource = slotObject->data;
    pathWork = resource->path;
    if (pathWork != 0) {
        dds3DeletePathControl(pathWork);
        resource->path = 0;
    }
}

/* Return the currently stored curve-work handle. */
dds3PathBasicControl_t *dds3MoverBasicGetPathControl(dds3Basic_t *slotObject) {
    Dds3SlotResource *resource = slotObject->data;

    return resource->path;
}

void dds3MoverBasicUpdate(dds3Basic_t *slotObject) {
    MoverBasicMainprocuder(slotObject);
}

/* Number of slots a given object kind occupies in the slot ring. */
s32 BasicTypeToItemType(u32 kind)
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
dds3ObjectBaseControlWorkData_t *dds3CreateSlotResourceState(void *owner) {
    dds3ObjectBaseControlWorkData_t *state = sdfChipAllocDirect(sizeof(dds3ObjectBaseControlWorkData_t));
    s32 i;
    WorldIndexNode *node;

    memset(state, 0, sizeof(dds3ObjectBaseControlWorkData_t));
    state->resourceState = 3;
    node = dds3GetBasicArrayHandle(0);
    state->worldIndexNode = node;
    state->slots[0] = owner;
    for (i = 0; i < DDS3_OBJECT_RESOURCE_SLOT_COUNT; i++) {
        state->resourceSlots[i] = -1;
    }
    state->currentSlot = -1;
    return state;
}

INCLUDE_SDATA(const s32, "game/code_00111610", dds3MoverCount);

