#include "sdk_ps2.h"
#include "dds3_path.h"
#include "common.h"
#include "dds3_object_flags.h"
#include "ps2asm.h"
#include "eff_inner_float_api.h"
#include "mdl.h"
#include "mdl_part_entry.h"
#include "dds3_world_counter.h"
#include "evt_unit_object_key.h"
#include "sdf_vu_matrix.h"
#include "eff_node.h"
#include "evt_world_source_transform.h"
#include "evt_script_event_id.h"
#include "dds3_task_lifecycle.h"
#include "scr.h"
#include "sdf_delayed_resource_check_api.h"
#include "fld_area_path.h"
#include "dds3_object_control_slot.h"
#include "dds3_world_selection.h"
#include "dds3_world_lifecycle.h"
#include "evt_world.h"
#include "evt_path_direction.h"
#include "dds3_task_queue_query.h"
#include "draw3d.h"
#include "dds3_draw_light_binding.h"
#include "sdf_texture_offset_list.h"
#include "eff_dependency.h"
#include "dds3_object_mode.h"
#include "evt_stage_anime_control_api.h"

#include "fpu.h"
#include "evt_task.h"

extern dds3Basic_t *dds3AddUnitObjectBasic(s32 world, u128 *pos, u128 *rot);
extern void dds3RebuildObjectModelContext(dds3Basic_t *obj, s32 a, s32 b);
extern void FromRotation(f32 angle);

extern void dds3MoverBasicMakePath(dds3Basic_t *object);

extern dds3Basic_t *dds3AddCameraObjectBasicTypeFree(s32 world, u128 *pos, u128 *rot);

void evtSetTestCamera(void) {
    f32 pos[4] = {0.0f, -100.0f, -600.0f, 0.0f};
    f32 rot[4];
    dds3Basic_t *obj;

    memset(rot, 0, 0x10);
    rot[3] = 1.0f;
    obj = dds3AddCameraObjectBasicTypeFree(dds3AllocObjectId(), (u128 *)pos, (u128 *)rot);
    dds3GetRadiusPoseSet(obj, 10.0f);
    dds3ObjectBaseCreateMover(obj);
    dds3ReplaceActiveCameraSceneBasic(dds3GetControlScene(), obj);
    dds3ObjectBaseSetPadControl(obj, 0);
}

s32 evtSetRegistUnit(s32 major, s32 minor) {
    f32 pos[4];
    f32 rot[4];
    s32 world;
    dds3Basic_t *obj;

    memset(pos, 0, 0x10);
    pos[3] = 1.0f;
    memset(rot, 0, 0x10);
    rot[3] = 1.0f;
    world = dds3AllocObjectId();
    obj = dds3AddUnitObjectBasic(world, (u128 *)pos, (u128 *)rot);
    dds3ObjectBaseOnFlag(obj, 0x20);
    dds3GetRadiusPoseSet(obj, 10.0f);
    dds3RebuildObjectModelContext(obj, major, minor);
    if (((dds3ModelHandle_t *)dds3ObjectBaseGetModelHandle(obj))->first != NULL) {
        mdlLoopMotion((dds3ModelHandle_t *)dds3ObjectBaseGetModelHandle(obj), 0, 0);
    }
    dds3ObjectBaseSetPadControl(obj, 0);
    return world;
}

extern s32 dds3ObjectBaseRegistArea(dds3Basic_t *obj);

EvtUnitObjectKey evtLoadUnit(s32 slotKind, s32 resource) {
    f32 pos[4];
    f32 rot[4];
    EvtUnitObjectKey world;
    dds3Basic_t *obj;

    memset(pos, 0, 0x10);
    pos[3] = 1.0f;
    memset(rot, 0, 0x10);
    rot[3] = 1.0f;
    world = dds3AllocObjectId();
    obj = dds3AddUnitObjectBasic(world, (u128 *)pos, (u128 *)rot);
    dds3ObjectBaseOnFlag(obj, 0x20);
    dds3GetRadiusPoseSet(obj, 10.0f);
    dds3RebuildObjectModelContext(obj, slotKind, resource);
    mdlLoopMotion((dds3ModelHandle_t *)dds3ObjectBaseGetModelHandle(obj), 0, 0);
    dds3ObjectBaseSetControlBasic(obj, dds3SearchSceneBasic(dds3GetControlScene(), 0x10000, 2));
    dds3ObjectBaseRegistArea(obj);
    dds3ObjectBaseSetPadControl(obj, 0);
    dds3ObjectBaseOnFlag(obj, 0x400);
    return world;
}

void evtSetUnitPos(u32 targetKey, u32 sourceKey) {
    f32 axis[4] = {0.0f, 1.0f, 0.0f, 1.0f};
    dds3Basic_t *obj;
    dds3Basic_t *src;
    EvtWorldSourceTransformPrefix *sourceTransform;

    obj = dds3SearchSceneBasic(dds3GetControlScene(), targetKey, 5);
    if (obj != NULL) {
        src = dds3SearchSceneBasic(dds3GetControlScene(), sourceKey, 0x11);
        if (src != NULL) {
            sourceTransform = src->workdata;
            dds3SetPosPoseSet(obj, (u128 *)&sourceTransform->position);
            SDF_VU0_LOADVECTOR(axis);
            FromRotation(3.14159265f);
            SDF_VU0_LOADVECTOR_B(sourceTransform->rotation);
            Mult();
            SDF_VU0_STOREVECTOR(axis);
            dds3SetRotPoseSet(obj, (u128 *)axis);
            dds3GetPosPoseSet(obj);
            SDF_VU0_STOREVECTOR(obj->poseset->smoothedPosition);
        }
    }
}

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
    evtWorldTable_t *worldTable;

    if (secondaryObject != NULL) {
        worldTable = ((evtWorldTable_t *)secondaryObject->workdata);
        while (worldTable->slots[EVT_WORLD_SLOT_UNIT].head != NULL) {
            dds3DeleteSceneBasicData(worldTable->slots[EVT_WORLD_SLOT_UNIT].head);
        }
    }
}

extern f32 D_003C8BA0[];
extern sdf3DFogParam_t D_003C8C60[];
extern char D_003C8C80[];
extern dds3Basic_t *dds3AddScene(void);
extern void evtSetTestCamera(void);
extern void dds3SetScreenFovy(f32 fieldOfView);
extern void dds3SetFog(const sdf3DFogParam_t *source);
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
    dds3SetLight((f32 (*)[4])D_003C8BA0, DDS3_LIGHT_SOURCE_SLOT_0);
    dds3SetLight((f32 (*)[4])(D_003C8BA0 + 0x10), DDS3_LIGHT_SOURCE_SLOT_1);
    dds3SetLight((f32 (*)[4])(D_003C8BA0 + 0x20), DDS3_LIGHT_SOURCE_SLOT_2);
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
    dds3SetLight((f32 (*)[4])D_003C8BA0, DDS3_LIGHT_SOURCE_SLOT_0);
    dds3SetLight((f32 (*)[4])(D_003C8BA0 + 0x10), DDS3_LIGHT_SOURCE_SLOT_1);
    dds3SetLight((f32 (*)[4])(D_003C8BA0 + 0x20), DDS3_LIGHT_SOURCE_SLOT_2);
    dds3SetFog(D_003C8C60);
    dds3SetAmbient((const u128 *)D_003C8C80);
    while (sdfDelayedResrcCheck() != 0) {
    }
    return 1;
}

extern char evtScriptResourcePathBuffer[];
#include "sdf_debug_api.h"

/* DDS2 twin of DDS1 func_00220110: start the event BF script by id. */
void evtStartEvent(EvtScriptEventId eventId) {
    sprintf(evtScriptResourcePathBuffer, evtScriptResourcePathFormat, eventId - eventId % 10, eventId, eventId);
    scrStartScriptByName(0x3EB, evtScriptResourcePathBuffer, 0);
}

const char evtFieldScriptTerminationFormat[0x28] = "kill field script -> [%s]\n";

void evtKillFieldScript(void) {
    dds3TaskState_t taskState;
    dds3TaskQueueIndex_t taskIndex;
    dds3ProcessID_t *task;

    for (taskState = DDS3_TASK_DELAYED_START; (s32)taskState < DDS3_TASK_DESTROY_PENDING; taskState++) {
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
EvtPathStatus evtCheckPathStat(dds3Basic_t *object) {
    f32 target = 1.0f;
    dds3Basic_t *slot;
    dds3PathBasicControl_t *handle;
    EvtPathDirection direction;

    if (object == NULL) {
        return EVT_PATH_STATUS_INVALID;
    }
    slot = dds3ObjectBaseGetControlBasic(object, DDS3_OBJECT_DATA_SLOT);
    if (slot == NULL) {
        return EVT_PATH_STATUS_INVALID;
    }
    handle = dds3MoverBasicGetPathControl(slot);
    if (handle == NULL) {
        return EVT_PATH_STATUS_INVALID;
    }
    direction = dds3PathBasicControlGetMode(&handle->counter);
    if (direction != EVT_PATH_DIRECTION_FORWARD) {
        if (direction != EVT_PATH_DIRECTION_REVERSE) {
            return EVT_PATH_STATUS_INVALID;
        }
        target = 0.0f;
    }
    if (dds3PathBasicControlGetRatio(&handle->counter) == target) {
        return EVT_PATH_STATUS_AT_ENDPOINT;
    }
    return EVT_PATH_STATUS_IN_PROGRESS;
}

void evtPathLoop(dds3Basic_t *worldObject, s32 enabled) {
    dds3PathBasicControl_t *data = dds3ObjectBaseGetPathControl(worldObject);

    if (data == NULL) {
        return;
    }
    if (enabled != 0) {
        dds3PathBasicLoopOn(&data->counter);
    } else {
        dds3PathBasicLoopOff(&data->counter);
    }
}

extern dds3Basic_t *dds3AddCameraObjectBasicTypeFree(s32 world, u128 *pos, u128 *rot);
extern void dds3GetRadiusPoseSet(dds3Basic_t *obj, f32 value);

dds3Basic_t *evtAddCamera(f32 *pos, f32 *rot) {
    dds3Basic_t *obj = dds3AddCameraObjectBasicTypeFree(dds3AllocObjectId(), (u128 *)pos, (u128 *)rot);

    if (obj == NULL) {
        return obj;
    }
    dds3GetRadiusPoseSet(obj, 1.0f);
    return obj;
}

void evtEffectLoopControl(dds3Basic_t *owner, s32 nodeFlag) {
    EffectDependencyState *state = owner->workdata;
    s32 stateKind = state->state;
    EffNode *node;

    switch (stateKind) {
    case EFF_DEPENDENCY_STATE_BOUND_EFFECT_NODE:
    case EFF_DEPENDENCY_STATE_MAGATUHI_TWO_ROWS:
    case EFF_DEPENDENCY_STATE_MAGATUHI_FOUR_ROWS:
        break;
    default:
        return;
    }
    node = (EffNode *)state->handle;
    if (nodeFlag == 0) {
        effDispatchOptionalNodeFlag(node, nodeFlag);
    }
}

EvtStageAnimeControlResult evtStageAnimeControl(s32 modeSelector, s32 modeValue) {
    dds3Basic_t *world = dds3GetActiveScene();
    dds3Basic_t *node;
    Dds3ObjectMatterAnimeMode requestedMode;

    if (world == NULL) {
        return EVT_STAGE_ANIME_CONTROL_NO_ACTIVE_WORLD;
    }
    requestedMode = modeSelector * 3 + modeValue;
    for (node = ((evtWorldTable_t *)world->workdata)->slots[6].head; node != NULL; node = node->next) {
        dds3ObjectBaseMatterAnimeSpeed(node, requestedMode);
    }
    return EVT_STAGE_ANIME_CONTROL_ACTIVE_WORLD;
}
