#include "sdk_ps2.h"
#include "common.h"
#include "sdf_chip.h"
#include "dds3_path.h"
#include "dds3obj.h"
#include "dds3_object_control_slot.h"
#include "dds3_world_counter.h"


/* Opaque fixed-size ring entry; this unit only selects its address. */
typedef struct {
    u8 unk0[0x10];
} SlotEntry;

#define DDS3_SLOT_RING_ENTRY_COUNT 10



extern SlotEntry dds3SlotRingEntries[];

extern s32 dds3MoverCount;

extern s32 MoverBasicMainprocuder(dds3Basic_t *slotObject);


/* Bind the owner, select a 16-byte ring entry, and advance the ten-entry cursor. */
dds3Basic_t *dds3AddMoverBasic(dds3Basic_t *owner) {
    dds3Basic_t *slotObject = dds3AddSceneBasicData(EFF_WORLD_KIND_SLOT_RING);
    Dds3SlotResource *resource = slotObject->workdata;
    u32 sequence = dds3AllocObjectId();
    s32 slotIndex;

    resource->target = owner;
    slotIndex = dds3MoverCount;
    slotObject->key = sequence;
    slotObject->name = (const char *)&dds3SlotRingEntries[slotIndex];
    dds3MoverCount = slotIndex + 1;
    dds3MoverCount = dds3MoverCount % DDS3_SLOT_RING_ENTRY_COUNT;
    return slotObject;
}

/* Install the relative-transform callback used when no path is retained. */
void dds3MoverBasicSetCallBackPrimaryControl(dds3Basic_t *slotObject, Dds3MoverUpdate update) {
    Dds3SlotResource *resource = slotObject->workdata;

    resource->update = update;
}

/* Cache the source object address for the next curve-work replacement. */
/* Cache the source address consumed by the slot's later path replacement. */
void dds3MoverBasicSetPath(dds3Basic_t *slotObject, dds3Basic_t *sourceObject) {
    Dds3SlotResource *resource = slotObject->workdata;

    resource->sourceObject = sourceObject;
}

/* Free the old curve work before constructing its replacement from the stored
 * source object address. The source is not an integer resource ID. */
void dds3MoverBasicMakePath(dds3Basic_t *slotObject) {
    Dds3SlotResource *resource;
    dds3PathBasicControl_t *pathWork;

    resource = slotObject->workdata;
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

    resource = slotObject->workdata;
    pathWork = resource->path;
    if (pathWork != 0) {
        dds3DeletePathControl(pathWork);
        resource->path = 0;
    }
}

/* Return the currently stored curve-work handle. */
dds3PathBasicControl_t *dds3MoverBasicGetPathControl(dds3Basic_t *slotObject) {
    Dds3SlotResource *resource = slotObject->workdata;

    return resource->path;
}

void dds3MoverBasicUpdate(dds3Basic_t *slotObject) {
    MoverBasicMainprocuder(slotObject);
}

/* Select the owner-control slot used by a world-node kind. */
Dds3ObjectControlSlotIndex BasicTypeToItemType(u32 kind)
{
    Dds3ObjectControlSlotIndex slotIndex = 0;

    switch (kind - 2) {
    case 1:
        slotIndex = DDS3_OBJECT_DATA_SLOT;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        break;
    case 8:
        slotIndex = DDS3_OBJECT_SCRIPT_MODEL_SLOT;
        break;
    case 10:
        slotIndex = DDS3_OBJECT_SCRIPT_MODEL_SLOT;
        break;
    case 11:
        slotIndex = DDS3_OBJECT_SCRIPT_MODEL_SLOT;
        break;
    case 9:
        slotIndex = DDS3_OBJECT_ACTION_B_SLOT;
        break;
    case 0:
        slotIndex = DDS3_OBJECT_HANDLER_SLOT;
        break;
    }
    return slotIndex;
}

/* Initialize a kind-3 slot state and its index node.
 * Every slot and currentSlot start unused (-1). */
dds3ObjectBaseControlWorkData_t *dds3CreateSlotResourceState(dds3Basic_t *owner) {
    dds3ObjectBaseControlWorkData_t *state = sdfChipAllocDirect(sizeof(dds3ObjectBaseControlWorkData_t));
    s32 i;
    WorldIndexNode *node;

    memset(state, 0, sizeof(dds3ObjectBaseControlWorkData_t));
    state->resourceState = DDS3_OBJECT_RESOURCE_RELEASED;
    node = dds3GetBasicArrayHandle(0);
    state->worldIndexNode = node;
    state->slots[DDS3_OBJECT_OWNER_SLOT] = owner;
    for (i = 0; i < DDS3_OBJECT_RESOURCE_SLOT_COUNT; i++) {
        state->resourceSlots[i] = -1;
    }
    state->currentSlot = -1;
    return state;
}

INCLUDE_SDATA(const s32, "game/code_00111838", dds3MoverCount);
