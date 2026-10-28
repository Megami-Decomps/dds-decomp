#include "sdk_ps2.h"
#include "scr.h"
#include "common.h"
#include "sdf_delayed_resource_check_api.h"
#include "sdf_float_counter.h"
#include "fld_area_path.h"
#include "dds3_path.h"
#include "dds3obj.h"
#include "dds3_object_control_slot.h"
#include "dds3_world_selection.h"
#include "dds3_world_lifecycle.h"
#include "evt_world.h"
#include "evt_path_direction.h"
#include "evt_world_object_api.h"
#include "kwln_task_lifecycle.h"
#include "kwln_task_queue_query.h"
#include "kwln_task_state.h"
#include "kwln.h"
#include "kwln_draw_light_binding.h"
#include "sdf_texture_offset_list.h"



extern void dds3MoverBasicMakePath(dds3Basic_t *object);

const char evtStageResourcePathFormat[0x10] = "%sf%03d_%03d";

const char evtScriptResourcePathFormat[0x20] = "/event/e%03d/e%03d/scr/e%03d.bf";

void evtFreeStage(void) {
    dds3Basic_t *secondary;

    secondary = (dds3Basic_t *)dds3GetControlScene();
    if (secondary != 0) {
        dds3DeleteScene(secondary);
        return;
    }
}



/* Detach every unit node from the secondary object's per-kind list. */
void evtFreeAllUnit(void) {
    dds3Basic_t *secondaryObject = (dds3Basic_t *)dds3GetControlScene();
    EvtWorldTable *worldTable;

    if (secondaryObject != NULL) {
        worldTable = ((EvtWorldTable *)secondaryObject->data);
        while (worldTable->slots[EVT_WORLD_SLOT_UNIT].head != NULL) {
            dds3DeleteSceneBasicData(worldTable->slots[EVT_WORLD_SLOT_UNIT].head);
        }
    }
}

extern f32 D_003C8BA0[];
extern KwlnDrawVectorParams D_003C8C60[];
extern char D_003C8C80[];
extern dds3Basic_t *dds3AddScene(void);
extern void evtSetTestCamera(void);
extern void dds3SetScreenFovy(f32 fieldOfView);
extern void dds3SetFog(const KwlnDrawVectorParams *source);
extern void dds3SetAmbient(const u128 *source);

s32 evtLoadArea(s32 area, s32 room)
{
    char directory[0x40];
    char resourcePath[0x50];
    dds3Basic_t *worldObject;

    evtFreeStage();
    worldObject = dds3AddScene();
    dds3SetControlScene(worldObject);
    dds3SetActiveScene(worldObject);
    dds3SetMapNo(worldObject, (area << 16) + room);
    fldMakeFilePath(directory, area, room);
    sprintf(resourcePath, evtStageResourcePathFormat, directory, area, room);
    dds3SceneBasicMakeScene(worldObject, resourcePath);
    evtSetTestCamera();
    if (area < 10) {
        dds3SetScreenFovy(0.9424777031f);
    } else {
        dds3SetScreenFovy(0.75398216f);
    }
    dds3SetLight((f32 (*)[4])D_003C8BA0, KWLN_LIGHT_SOURCE_SLOT_0);
    dds3SetLight((f32 (*)[4])(D_003C8BA0 + 0x10), KWLN_LIGHT_SOURCE_SLOT_1);
    dds3SetLight((f32 (*)[4])(D_003C8BA0 + 0x20), KWLN_LIGHT_SOURCE_SLOT_2);
    dds3SetFog(D_003C8C60);
    dds3SetAmbient((const u128 *)D_003C8C80);
    return 1;
}

extern void dds3SceneBasicMakeScene_FromMemory(dds3Basic_t *object, u32 area, u32 room,
                                                       u8 *resource0Data, u8 *resource1Data,
                                                       const SdfTextureOffsetListHeader *textureOffsets, u32 flags);
s32 evtLoadArea_FromMemory(s32 area, s32 room, u8 *resource0Data, u8 *resource1Data,
                                     const SdfTextureOffsetListHeader *textureOffsets, s32 flags)
{
    dds3Basic_t *worldObject;
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
    dds3SetLight((f32 (*)[4])D_003C8BA0, KWLN_LIGHT_SOURCE_SLOT_0);
    dds3SetLight((f32 (*)[4])(D_003C8BA0 + 0x10), KWLN_LIGHT_SOURCE_SLOT_1);
    dds3SetLight((f32 (*)[4])(D_003C8BA0 + 0x20), KWLN_LIGHT_SOURCE_SLOT_2);
    dds3SetFog(D_003C8C60);
    dds3SetAmbient((const u128 *)D_003C8C80);
    while (sdfDelayedResrcCheck() != 0) {
    }
    return 1;
}

extern char evtScriptResourcePathBuffer[];
extern dds3ProcessID_t *scrStartScriptByName(u32 priority, const char *path, ScrProcedureIndex procedureIndex);
#include "sdf_debug_api.h"

/* DDS2 twin of DDS1 func_00220110: start the event BF script by id. */
void evtStartEvent(s32 eventId) {
    sprintf(evtScriptResourcePathBuffer, evtScriptResourcePathFormat, eventId - eventId % 10, eventId, eventId);
    scrStartScriptByName(0x3EB, evtScriptResourcePathBuffer, 0);
}

const char evtFieldScriptTerminationFormat[0x28] = "kill field script -> [%s]\n";

void evtKillFieldScript(void) {
    KwlnTaskState taskState;
    KwlnTaskQueueIndex taskIndex;
    dds3ProcessID_t *task;

    for (taskState = KWLN_TASK_DELAYED_START; (s32)taskState < KWLN_TASK_DESTROY_PENDING; taskState++) {
    restartStateScan:
        for (taskIndex = 0; taskIndex < dds3GetProcessCount(taskState); taskIndex++) {
            task = dds3SearchProcessNumber(taskState, taskIndex);
            if (task->priority == 0x3EA) {
                printf(evtFieldScriptTerminationFormat, task->name);
                dds3KillProcess(task, 0);
                /* Removal shifts queue indexes, so restart from its head. */
                goto restartStateScan;
            }
        }
    }
}

void evtPathStop(dds3Basic_t *object) {
    dds3PathBasicControl_t *slotData;

    slotData = dds3ObjectBaseGetPathControl(object);
    if (slotData != 0) {
        dds3PathBasicPauseOn(&slotData->counter);
        return;
    }
}

void evtPathStart(dds3Basic_t *object) {
    dds3PathBasicControl_t *slotData;

    slotData = dds3ObjectBaseGetPathControl(object);
    if (slotData != 0) {
        dds3PathBasicPauseOff(&slotData->counter);
        return;
    }
}


/* Scale the object's retained path time within the clamped multiplier range. */
void evtPathRate(dds3Basic_t *worldNode, f32 multiplier) {
    dds3PathBasicControl_t *slotData;

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

void evtPathDir(dds3Basic_t *worldNode, EvtPathDirection direction) {
    dds3PathBasicControl_t *slotData;

    slotData = dds3ObjectBaseGetPathControl(worldNode);
    if (slotData != 0) {
        dds3PathBasicControlAlterMode(&slotData->counter, direction);
    }
}

/* Attach object to the node referenced by owner's owned handle. */
EvtPathLinkResult evtPathLinkAndMake(dds3Basic_t *object, dds3Basic_t *owner) {
    dds3ObjectBaseControlWorkData_t *ref;
    dds3Basic_t *slotNode;

    if (object == NULL) {
        return EVT_PATH_LINK_FAILED;
    }
    ref = dds3GetBaseControlWorkData(owner);
    if (ref == NULL) {
        return EVT_PATH_LINK_FAILED;
    }
    slotNode = ref->slots[DDS3_OBJECT_DATA_SLOT];
    if (slotNode == NULL) {
        return EVT_PATH_LINK_FAILED;
    }
    dds3MoverBasicSetPath(slotNode, object);
    dds3MoverBasicMakePath(slotNode);
    return EVT_PATH_LINK_SUCCEEDED;
}
