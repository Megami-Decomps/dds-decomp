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

extern void sdfSetFloatCounterDirection(u32 *destination, u32 value);
extern void sdfFreezeFloatCounter(struct EvtScaledValue *);
extern void sdfUnfreezeFloatCounter(struct EvtScaledValue *);


extern void dds3ReplaceObjectResource(EffWorldNode *);

extern Dds3PathCurveWork *dds3GetSlot1Data(void *object);

const char D_00421578[0x10] = "%sf%03d_%03d";

const char D_00421588[0x20] = "/event/e%03d/e%03d/scr/e%03d.bf";

void evtDestroySecondaryWorldNode(void) {
    EffWorldNode *secondary;

    secondary = (EffWorldNode *)dds3GetWorldSecondaryObject();
    if (secondary != 0) {
        dds3DestroyWorldNode(secondary);
        return;
    }
}



/* Detach every unit node from the secondary object's per-kind list. */
void evtDrainSecondaryWorldNodes(void) {
    EffWorldNode *object = (EffWorldNode *)dds3GetWorldSecondaryObject();
    EvtWorldTable *table;

    if (object != NULL) {
        table = ((EvtWorldTable *)object->data);
        while (table->slots[EVT_WORLD_SLOT_UNIT].head != NULL) {
            dds3RemoveWorldObjectNode(table->slots[EVT_WORLD_SLOT_UNIT].head);
        }
    }
}

extern char D_003C8BA0[];
extern char D_003C8C60[];
extern char D_003C8C80[];
extern EffWorldNode *dds3AppendWorldNode(void);
extern void dds3AttachResourceHandleToWorldObject(void *, u32);
extern void mdlSpawnViewerWorldObject(void);
extern void sdfSetViewFieldOfView(f32);
extern void dds3DrawSetIndexedWord(u32, s32);
extern void kwlnDrawCopyWords20(void *);
extern void kwlnDrawCopyRow128(void *);
extern void sprintf(char *, const char *, ...);

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
    sprintf(resourcePath, D_00421578, directory, area, room);
    dds3AttachResourceHandleToWorldObject(worldObject, (u32)resourcePath);
    mdlSpawnViewerWorldObject();
    if (area < 10) {
        sdfSetViewFieldOfView(0.9424777031f);
    } else {
        sdfSetViewFieldOfView(0.75398216f);
    }
    dds3DrawSetIndexedWord((u32)D_003C8BA0, 0);
    dds3DrawSetIndexedWord((u32)(D_003C8BA0 + 0x40), 1);
    dds3DrawSetIndexedWord((u32)(D_003C8BA0 + 0x80), 2);
    kwlnDrawCopyWords20(D_003C8C60);
    kwlnDrawCopyRow128(D_003C8C80);
    return 1;
}

extern void dds3AttachConstructedResourceToWorldObject(EffWorldNode *, u32, u32, u32, u32,
                                                       const SdfTextureOffsetListHeader *, u32);
extern s32 sdfCheckPendingWorkWithInterrupts(void);

s32 evtCreateWorldObjectFromResource(s32 area, s32 room, s32 arg2, s32 arg3,
                                     const SdfTextureOffsetListHeader *textureOffsets, s32 arg5)
{
    EffWorldNode *worldObject;
    char directory[0x40];

    evtDestroySecondaryWorldNode();
    worldObject = dds3AppendWorldNode();
    dds3SetWorldSecondaryObject(worldObject);
    dds3SetWorldObject(worldObject);
    dds3SetWorldObjectValue(worldObject, (area << 16) + room);
    fldFormatAreaDirectory(directory, area, room);
    dds3AttachConstructedResourceToWorldObject(worldObject, area, room, arg2, arg3, textureOffsets, arg5);
    mdlSpawnViewerWorldObject();
    sdfSetViewFieldOfView(0.75398216f);
    dds3DrawSetIndexedWord((u32)D_003C8BA0, 0);
    dds3DrawSetIndexedWord((u32)(D_003C8BA0 + 0x40), 1);
    dds3DrawSetIndexedWord((u32)(D_003C8BA0 + 0x80), 2);
    kwlnDrawCopyWords20(D_003C8C60);
    kwlnDrawCopyRow128(D_003C8C80);
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    return 1;
}

extern void sprintf(char *, const char *, ...);
extern char evtScriptResourcePathBuffer[];
extern s32 scrCreateProcessTaskFromResource(s32, const char *, s32);
extern void sdfDebugPrintf(const char *format, ...);

/* DDS2 twin of DDS1 func_00220110: start the event BF script by id. */
void evtCreateEventScriptProcess(s32 eventId) {
    sprintf(evtScriptResourcePathBuffer, D_00421588, eventId - eventId % 10, eventId, eventId);
    scrCreateProcessTaskFromResource(0x3EB, evtScriptResourcePathBuffer, 0);
}

const char D_004215A8[0x28] = "kill field script -> [%s]\n";

void evtKillFieldScriptTasks(void) {
    s32 state;
    s32 taskIndex;
    KwlnTask *task;

    for (state = KWLN_TASK_DELAYED_START; state < KWLN_TASK_DESTROY_PENDING; state++) {
    restartStateScan:
        for (taskIndex = 0; taskIndex < kwlnTaskGetStateList(state); taskIndex++) {
            task = kwlnTaskGetByStateAndIndex(state, taskIndex);
            if (task->priority == 0x3EA) {
                sdfDebugPrintf(D_004215A8, task->name);
                kwlnTaskDestroyWithHierarchy(task, 0);
                /* Destruction changes the queue, so restart the current scan. */
                goto restartStateScan;
            }
        }
    }
}

void evtSetWorldSlotStatusFlag(void *object) {
    Dds3PathCurveWork *slotData;

    slotData = dds3GetSlot1Data(object);
    if (slotData != 0) {
        sdfFreezeFloatCounter((struct EvtScaledValue *)slotData);
        return;
    }
}

void evtClearWorldSlotStatusFlag(void *object) {
    Dds3PathCurveWork *slotData;

    slotData = dds3GetSlot1Data(object);
    if (slotData != 0) {
        sdfUnfreezeFloatCounter((struct EvtScaledValue *)slotData);
        return;
    }
}

extern void evtScaleValueByMultiplier(f32 multiplier, struct EvtScaledValue *slotData);

/* Scale the object's retained path time within the clamped multiplier range. */
void evtScaleSlotByClampedMultiplier(void *object, f32 multiplier) {
    Dds3PathCurveWork *slotData;

    slotData = dds3GetSlot1Data(object);
    if (slotData != 0) {
        if (multiplier < 0.0f) {
            multiplier = 0.0f;
        }
        if (multiplier > 1.0f) {
            multiplier = 1.0f;
        }
        evtScaleValueByMultiplier(multiplier, (struct EvtScaledValue *)slotData);
    }
}

void evtSetWorldSlotValue(void *object, u32 value) {
    Dds3PathCurveWork *slotData;

    slotData = dds3GetSlot1Data(object);
    if (slotData != 0) {
        sdfSetFloatCounterDirection((u32 *)&slotData->direction, value);
    }
}

/* Attach object to the node referenced by owner's owned handle. */
s32 evtStageRelinkOwnedNodeResource(void *object, void *owner) {
    ObjBase *ref;
    void *node;

    if (object == NULL) {
        return 0;
    }
    ref = dds3GetObjectOwnedHandle(owner);
    if (ref == NULL) {
        return 0;
    }
    node = ref->slots[1];
    if (node == NULL) {
        return 0;
    }
    dds3SetSlotKey(node, object);
    dds3ReplaceObjectResource(node);
    return 1;
}
