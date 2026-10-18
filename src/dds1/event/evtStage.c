#include "common.h"
#include "evt_scaled_value.h"
#include "fld_area_path.h"
#include "dds3_path.h"
#include "dds3obj.h"
#include "dds3_world_selection.h"
#include "dds3_world_lifecycle.h"
#include "evt_world.h"
#include "evt_world_object_api.h"
#include "kwln_task_lifecycle.h"
#include "kwln_task_queue_query.h"
#include "kwln_task_state.h"
#include "sdf_texture_offset_list.h"

extern void sdfSetFloatCounterDirection(u32 *destination, u32 direction);


extern void dds3ReplaceObjectResource(EffWorldNode *object);

Dds3PathCurveWork *dds3GetSlot1Data(void *object);
extern const char evtFieldScriptTerminationFormat[0x28];
#include "sdf_debug_api.h"

extern EffWorldNode *dds3AppendWorldNode(void);
extern void dds3AttachConstructedResourceToWorldObject(EffWorldNode *object, u32 area, u32 room,
                                                       u32 resource0Data, u32 resource1Data,
                                                       const SdfTextureOffsetListHeader *textureOffsets, u32 flags);
extern void mdlSpawnViewerWorldObject(void);
extern void sdfSetViewFieldOfView(f32 fieldOfView);
extern void dds3DrawSetLightSourcePointer(void *lightSource, s32 index);
extern void kwlnDrawCopyWords20(void *source);
extern void kwlnDrawCopyRow128(u128 *source);
extern const char evtStageResourcePathFormat[0x10];
extern char D_00367DF0[];
extern char D_00367EB0[];
extern char D_00367ED0[];
extern void sprintf(char *, const char *, ...);

void evtDestroySecondaryWorldNode(void)
{
    EffWorldNode *node;

    node = (EffWorldNode *)dds3GetWorldSecondaryObject();
    if (node != 0) {
        dds3DestroyWorldNode(node);
    }
}



/* Detach every unit node from the secondary object's per-kind list. */
void evtDrainSecondaryWorldNodes(void) {
    EffWorldNode *secondaryObject = (EffWorldNode *)dds3GetWorldSecondaryObject();
    EvtWorldTable *worldTable;

    if (secondaryObject != NULL) {
        worldTable = ((EvtWorldTable *)secondaryObject->data);
        while (worldTable->slots[EVT_WORLD_SLOT_UNIT].head != NULL) {
            dds3RemoveWorldObjectNode(worldTable->slots[EVT_WORLD_SLOT_UNIT].head);
        }
    }
}

const char evtStageResourcePathFormat[0x10] = "%sf%03d_%03d";

/* Build the field world object and load its stage resource. */
s32 evtCreateWorldObjectForKey(s32 area, s32 room)
{
    char directory[0x40];
    char resourcePath[0x50];
    EffWorldNode *worldObject;

    evtDestroySecondaryWorldNode();
    worldObject = dds3AppendWorldNode();
    dds3SetWorldSecondaryObject(worldObject);
    dds3SetWorldObject(worldObject);
    dds3SetWorldObjectValue(worldObject, (area << 16) + room);
    fldFormatAreaDirectory(directory, area, room);
    sprintf(resourcePath, evtStageResourcePathFormat, directory, area, room);
    dds3AttachResourceHandleToWorldObject(worldObject, (u32)resourcePath);
    mdlSpawnViewerWorldObject();
    if (area < 10) {
        sdfSetViewFieldOfView(0.9424777031f);
    } else {
        sdfSetViewFieldOfView(0.75398216f);
    }
    dds3DrawSetLightSourcePointer(D_00367DF0, 0);
    dds3DrawSetLightSourcePointer(D_00367DF0 + 0x40, 1);
    dds3DrawSetLightSourcePointer(D_00367DF0 + 0x80, 2);
    kwlnDrawCopyWords20(D_00367EB0);
    kwlnDrawCopyRow128((u128 *)D_00367ED0);
    return 1;
}

/* Build a field world object from the supplied resource parameters. */
s32 evtCreateWorldObjectFromResource(s32 area, s32 room, s32 resource0Data, s32 resource1Data,
                                     const SdfTextureOffsetListHeader *textureOffsets, s32 flags)
{
    EffWorldNode *worldObject;
    char directory[0x40];

    evtDestroySecondaryWorldNode();
    worldObject = dds3AppendWorldNode();
    dds3SetWorldSecondaryObject(worldObject);
    dds3SetWorldObject(worldObject);
    dds3SetWorldObjectValue(worldObject, (area << 16) + room);
    fldFormatAreaDirectory(directory, area, room);
    dds3AttachConstructedResourceToWorldObject(worldObject, area, room, resource0Data, resource1Data, textureOffsets, flags);
    mdlSpawnViewerWorldObject();
    sdfSetViewFieldOfView(0.75398216f);
    dds3DrawSetLightSourcePointer(D_00367DF0, 0);
    dds3DrawSetLightSourcePointer(D_00367DF0 + 0x40, 1);
    dds3DrawSetLightSourcePointer(D_00367DF0 + 0x80, 2);
    kwlnDrawCopyWords20(D_00367EB0);
    kwlnDrawCopyRow128((u128 *)D_00367ED0);
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    return 1;
}

extern s32 scrCreateProcessTaskFromResource(s32 priority, const char *path, s32 option);
extern char evtScriptResourcePathBuffer[];

void evtCreateEventScriptProcess(s32 eventId) {
    sprintf(evtScriptResourcePathBuffer, "/event/e%03d/e%03d/scr/e%03d.bf", eventId - eventId % 10, eventId, eventId);
    scrCreateProcessTaskFromResource(0x3EB, evtScriptResourcePathBuffer, 0);
}

const char evtFieldScriptTerminationFormat[0x28] = "kill field script -> [%s]\n";

void evtKillFieldScriptTasks(void)
{
    s32 taskState;
    s32 taskIndex;
    KwlnTask *task;

    for (taskState = KWLN_TASK_DELAYED_START; taskState < KWLN_TASK_DESTROY_PENDING; taskState++) {
    restartStateScan:
        for (taskIndex = 0; taskIndex < kwlnTaskGetStateList(taskState); taskIndex++) {
            task = kwlnTaskGetByStateAndIndex(taskState, taskIndex);
            if (task->priority == 0x3EA) {
                sdfDebugPrintf(evtFieldScriptTerminationFormat, task->name);
                kwlnTaskDestroyWithHierarchy(task, 0);
                /* Destruction changes the queue, so restart the current scan. */
                goto restartStateScan;
            }
        }
    }
}

void evtSetWorldSlotStatusFlag(void *object)
{
    Dds3PathCurveWork *slotData;

    slotData = dds3GetSlot1Data(object);
    if (slotData != 0) {
        sdfFreezeFloatCounter((struct EvtScaledValue *)slotData);
    }
}

void evtClearWorldSlotStatusFlag(void *object)
{
    Dds3PathCurveWork *slotData;

    slotData = dds3GetSlot1Data(object);
    if (slotData != 0) {
        sdfUnfreezeFloatCounter((struct EvtScaledValue *)slotData);
    }
}


/* Scale the object's retained path time within the clamped multiplier range. */
void evtScaleSlotByClampedMultiplier(void *worldNode, f32 multiplier) {
    Dds3PathCurveWork *slotData;

    slotData = dds3GetSlot1Data(worldNode);
    if (slotData != 0) {
        if (multiplier < 0.0f) {
            multiplier = 0.0f;
        }
        if (multiplier > 1.0f) {
            multiplier = 1.0f;
        }
        evtScaleValueByMultiplier((EvtScaledValue *)slotData, multiplier);
    }
}

void evtSetWorldSlotValue(void *worldNode, u32 direction) {
    Dds3PathCurveWork *slotData;

    slotData = dds3GetSlot1Data(worldNode);
    if (slotData != 0) {
        sdfSetFloatCounterDirection((u32 *)&slotData->direction, direction);
    }
}

/* Attach object to the node referenced by owner's owned handle. */
s32 evtStageRelinkOwnedNodeResource(void *object, void *owner) {
    ObjBase *ref;
    void *slotNode;

    if (object == NULL) {
        return 0;
    }
    ref = dds3GetObjectOwnedHandle(owner);
    if (ref == NULL) {
        return 0;
    }
    slotNode = ref->slots[1];
    if (slotNode == NULL) {
        return 0;
    }
    dds3SetSlotKey(slotNode, object);
    dds3ReplaceObjectResource(slotNode);
    return 1;
}
