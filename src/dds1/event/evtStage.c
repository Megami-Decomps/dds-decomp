#include "sdk_ps2.h"
#include "common.h"
#include "sdf_float_counter.h"
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
#include "kwln.h"
#include "sdf_texture_offset_list.h"



extern void dds3MoverBasicMakePath(EffWorldNode *object);

extern const char evtFieldScriptTerminationFormat[0x28];
#include "sdf_debug_api.h"

extern EffWorldNode *dds3AddScene(void);
extern void dds3SceneBasicMakeScene_FromMemory(EffWorldNode *object, u32 area, u32 room,
                                                       u32 resource0Data, u32 resource1Data,
                                                       const SdfTextureOffsetListHeader *textureOffsets, u32 flags);
extern void evtSetTestCamera(void);
extern void dds3SetScreenFovy(f32 fieldOfView);
extern void dds3SetLight(void *lightSource, s32 index);
extern void dds3SetFog(KwlnDrawVectorParams *source);
extern void dds3SetAmbient(u128 *source);
extern const char evtStageResourcePathFormat[0x10];
extern char D_00367DF0[];
extern KwlnDrawVectorParams D_00367EB0[];
extern char D_00367ED0[];

void evtFreeStage(void)
{
    EffWorldNode *node;

    node = (EffWorldNode *)dds3GetControlScene();
    if (node != 0) {
        dds3DeleteScene(node);
    }
}



/* Detach every unit node from the secondary object's per-kind list. */
void evtFreeAllUnit(void) {
    EffWorldNode *secondaryObject = (EffWorldNode *)dds3GetControlScene();
    EvtWorldTable *worldTable;

    if (secondaryObject != NULL) {
        worldTable = ((EvtWorldTable *)secondaryObject->data);
        while (worldTable->slots[EVT_WORLD_SLOT_UNIT].head != NULL) {
            dds3DeleteSceneBasicData(worldTable->slots[EVT_WORLD_SLOT_UNIT].head);
        }
    }
}

const char evtStageResourcePathFormat[0x10] = "%sf%03d_%03d";

/* Build the field world object and load its stage resource. */
s32 evtLoadArea(s32 area, s32 room)
{
    char directory[0x40];
    char resourcePath[0x50];
    EffWorldNode *worldObject;

    evtFreeStage();
    worldObject = dds3AddScene();
    dds3SetControlScene(worldObject);
    dds3SetActiveScene(worldObject);
    dds3SetMapNo(worldObject, (area << 16) + room);
    fldMakeFilePath(directory, area, room);
    sprintf(resourcePath, evtStageResourcePathFormat, directory, area, room);
    dds3SceneBasicMakeScene(worldObject, (u32)resourcePath);
    evtSetTestCamera();
    if (area < 10) {
        dds3SetScreenFovy(0.9424777031f);
    } else {
        dds3SetScreenFovy(0.75398216f);
    }
    dds3SetLight(D_00367DF0, 0);
    dds3SetLight(D_00367DF0 + 0x40, 1);
    dds3SetLight(D_00367DF0 + 0x80, 2);
    dds3SetFog(D_00367EB0);
    dds3SetAmbient((u128 *)D_00367ED0);
    return 1;
}

extern s32 sdfDelayedResrcCheck(void);

/* Build a field world object from the supplied resource parameters. */
s32 evtLoadArea_FromMemory(s32 area, s32 room, s32 resource0Data, s32 resource1Data,
                                     const SdfTextureOffsetListHeader *textureOffsets, s32 flags)
{
    EffWorldNode *worldObject;
    char directory[0x40];

    evtFreeStage();
    worldObject = dds3AddScene();
    dds3SetControlScene(worldObject);
    dds3SetActiveScene(worldObject);
    dds3SetMapNo(worldObject, (area << 16) + room);
    fldMakeFilePath(directory, area, room);
    dds3SceneBasicMakeScene_FromMemory(worldObject, area, room, resource0Data, resource1Data, textureOffsets, flags);
    evtSetTestCamera();
    dds3SetScreenFovy(0.75398216f);
    dds3SetLight(D_00367DF0, 0);
    dds3SetLight(D_00367DF0 + 0x40, 1);
    dds3SetLight(D_00367DF0 + 0x80, 2);
    dds3SetFog(D_00367EB0);
    dds3SetAmbient((u128 *)D_00367ED0);
    while (sdfDelayedResrcCheck() != 0) {
    }
    return 1;
}

extern s32 scrStartScriptByName(s32 priority, const char *path, s32 option);
extern char evtScriptResourcePathBuffer[];

void evtStartEvent(s32 eventId) {
    sprintf(evtScriptResourcePathBuffer, "/event/e%03d/e%03d/scr/e%03d.bf", eventId - eventId % 10, eventId, eventId);
    scrStartScriptByName(0x3EB, evtScriptResourcePathBuffer, 0);
}

const char evtFieldScriptTerminationFormat[0x28] = "kill field script -> [%s]\n";

void evtKillFieldScript(void)
{
    s32 taskState;
    s32 taskIndex;
    KwlnTask *task;

    for (taskState = KWLN_TASK_DELAYED_START; taskState < KWLN_TASK_DESTROY_PENDING; taskState++) {
    restartStateScan:
        for (taskIndex = 0; taskIndex < dds3GetProcessCount(taskState); taskIndex++) {
            task = dds3SearchProcessNumber(taskState, taskIndex);
            if (task->priority == 0x3EA) {
                printf(evtFieldScriptTerminationFormat, task->name);
                dds3KillProcess(task, 0);
                /* Destruction changes the queue, so restart the current scan. */
                goto restartStateScan;
            }
        }
    }
}

void evtPathStop(void *object)
{
    Dds3PathCurveWork *slotData;

    slotData = dds3ObjectBaseGetPathControl(object);
    if (slotData != 0) {
        dds3PathBasicPauseOn(&slotData->counter);
    }
}

void evtPathStart(void *object)
{
    Dds3PathCurveWork *slotData;

    slotData = dds3ObjectBaseGetPathControl(object);
    if (slotData != 0) {
        dds3PathBasicPauseOff(&slotData->counter);
    }
}


/* Scale the object's retained path time within the clamped multiplier range. */
void evtPathRate(void *worldNode, f32 multiplier) {
    Dds3PathCurveWork *slotData;

    slotData = dds3ObjectBaseGetPathControl(worldNode);
    if (slotData != 0) {
        if (multiplier < 0.0f) {
            multiplier = 0.0f;
        }
        if (multiplier > 1.0f) {
            multiplier = 1.0f;
        }
        dds3PathBasicControlSetRatio(&slotData->counter, multiplier);
    }
}

void evtPathDir(void *worldNode, u32 direction) {
    Dds3PathCurveWork *slotData;

    slotData = dds3ObjectBaseGetPathControl(worldNode);
    if (slotData != 0) {
        dds3PathBasicControlAlterMode(&slotData->counter, direction);
    }
}

/* Attach object to the node referenced by owner's owned handle. */
s32 evtPathLinkAndMake(void *object, void *owner) {
    ObjBase *ref;
    void *slotNode;

    if (object == NULL) {
        return 0;
    }
    ref = dds3GetBaseControlWorkData(owner);
    if (ref == NULL) {
        return 0;
    }
    slotNode = ref->slots[1];
    if (slotNode == NULL) {
        return 0;
    }
    dds3MoverBasicSetPath(slotNode, object);
    dds3MoverBasicMakePath(slotNode);
    return 1;
}
