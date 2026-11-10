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

extern void FromRotation(f32 angle);

extern void dds3MoverBasicMakePath(dds3Basic_t *object);

extern const char evtFieldScriptTerminationFormat[0x28];
#include "sdf_debug_api.h"

extern dds3Basic_t *dds3AddScene(void);
extern void dds3SceneBasicMakeScene_FromMemory(dds3Basic_t *object, u32 area, u32 room,
                                                       u8 *resource0Data, u8 *resource1Data,
                                                       const SdfTextureOffsetListHeader *textureOffsets, u32 flags);
extern void evtSetTestCamera(void);
extern void dds3SetScreenFovy(f32 fieldOfView);
extern void dds3SetFog(const sdf3DFogParam_t *source);
extern void dds3SetAmbient(const u128 *source);
extern const char evtStageResourcePathFormat[0x10];
extern f32 D_00367DF0[];
extern sdf3DFogParam_t D_00367EB0[];
extern char D_00367ED0[];

extern dds3Basic_t *dds3AddCameraObjectBasicTypeFree(s32 counter, u128 *position, u128 *rotation);

extern void dds3ObjectBaseCreateMover(dds3Basic_t *object);

void evtSetTestCamera(void) {
    f32 position[4] = { 0.0f, -100.0f, -600.0f, 0.0f };
    f32 rotation[4];
    dds3Basic_t *object;

    memset(rotation, 0, 0x10);
    rotation[3] = 1.0f;
    object = dds3AddCameraObjectBasicTypeFree(dds3AllocObjectId(), (u128 *)position, (u128 *)rotation);
    dds3GetRadiusPoseSet((dds3Basic_t *)object, 10.0f);
    dds3ObjectBaseCreateMover(object);
    dds3ReplaceActiveCameraSceneBasic(dds3GetControlScene(), (dds3Basic_t *)object);
    dds3ObjectBaseSetPadControl(object, 0);
}

extern void dds3RebuildObjectModelContext(dds3Basic_t *object, s32, s32);

extern dds3Basic_t *dds3AddUnitObjectBasic(s32 counter, u128 *position, u128 *rotation);

s32 evtSetRegistUnit(s32 major, s32 minor) {
    f32 position[4];
    f32 rotation[4];
    s32 counter;
    dds3Basic_t *object;

    memset(position, 0, 0x10);
    position[3] = 1.0f;
    memset(rotation, 0, 0x10);
    rotation[3] = 1.0f;
    counter = dds3AllocObjectId();
    object = dds3AddUnitObjectBasic(counter, (u128 *)position, (u128 *)rotation);
    dds3ObjectBaseOnFlag(object, 0x20);
    dds3GetRadiusPoseSet(object, 10.0f);
    dds3RebuildObjectModelContext(object, major, minor);
    if (((s32 *)dds3ObjectBaseGetModelHandle(object))[7] != 0) {
        mdlLoopMotion((void *)dds3ObjectBaseGetModelHandle(object), 0, 0);
    }
    dds3ObjectBaseSetPadControl(object, 0);
    return counter;
}

extern s32 dds3ObjectBaseRegistArea(dds3Basic_t *object);

EvtUnitObjectKey evtLoadUnit(s32 slotKind, s32 resource) {
    f32 position[4];
    f32 rotation[4];
    EvtUnitObjectKey counter;
    dds3Basic_t *object;

    memset(position, 0, 0x10);
    position[3] = 1.0f;
    memset(rotation, 0, 0x10);
    rotation[3] = 1.0f;
    counter = dds3AllocObjectId();
    object = dds3AddUnitObjectBasic(counter, (u128 *)position, (u128 *)rotation);
    dds3ObjectBaseOnFlag(object, 0x20);
    dds3GetRadiusPoseSet(object, 10.0f);
    dds3RebuildObjectModelContext(object, slotKind, resource);
    mdlLoopMotion((void *)dds3ObjectBaseGetModelHandle(object), 0, 0);
    dds3ObjectBaseSetControlBasic(object, dds3SearchSceneBasic(dds3GetControlScene(), 0x10000, 2));
    dds3ObjectBaseRegistArea(object);
    dds3ObjectBaseSetPadControl(object, 0);
    dds3ObjectBaseOnFlag(object, 0x400);
    return counter;
}

void evtSetUnitPos(u32 targetKey, u32 sourceKey) {
    f32 quaternion[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
    dds3Basic_t *target;
    dds3Basic_t *source;
    EvtWorldSourceTransformPrefix *sourceTransform;

    target = dds3SearchSceneBasic(dds3GetControlScene(), targetKey, 5);
    if (target != NULL) {
        source = dds3SearchSceneBasic(dds3GetControlScene(), sourceKey, 0x11);
        if (source != NULL) {
            sourceTransform = source->workdata;
            dds3SetPosPoseSet(target, (u128 *)&sourceTransform->position);
                        SDF_VU0_LOADVECTOR(quaternion);
            FromRotation(3.14159265f);
                        SDF_VU0_LOADVECTOR_B(sourceTransform->rotation);
            Mult();
                        SDF_VU0_STOREVECTOR(quaternion);
            dds3SetRotPoseSet(target, (u128 *)quaternion);
            dds3GetPosPoseSet(target);
            SDF_VU0_STOREVECTOR(target->poseset->smoothedPosition);
        }
    }
}

void evtFreeStage(void)
{
    dds3Basic_t *node;

    node = (dds3Basic_t *)dds3GetControlScene();
    if (node != 0) {
        dds3DeleteScene(node);
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

const char evtStageResourcePathFormat[0x10] = "%sf%03d_%03d";

/* Build the field world object and load its stage resource. */
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
    dds3SetLight((f32 (*)[4])D_00367DF0, DDS3_LIGHT_SOURCE_SLOT_0);
    dds3SetLight((f32 (*)[4])(D_00367DF0 + 0x10), DDS3_LIGHT_SOURCE_SLOT_1);
    dds3SetLight((f32 (*)[4])(D_00367DF0 + 0x20), DDS3_LIGHT_SOURCE_SLOT_2);
    dds3SetFog(D_00367EB0);
    dds3SetAmbient((const u128 *)D_00367ED0);
    return 1;
}
/* Build a field world object from the supplied resource parameters. */
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
    dds3SetLight((f32 (*)[4])D_00367DF0, DDS3_LIGHT_SOURCE_SLOT_0);
    dds3SetLight((f32 (*)[4])(D_00367DF0 + 0x10), DDS3_LIGHT_SOURCE_SLOT_1);
    dds3SetLight((f32 (*)[4])(D_00367DF0 + 0x20), DDS3_LIGHT_SOURCE_SLOT_2);
    dds3SetFog(D_00367EB0);
    dds3SetAmbient((const u128 *)D_00367ED0);
    while (sdfDelayedResrcCheck() != 0) {
    }
    return 1;
}

extern char evtScriptResourcePathBuffer[];

void evtStartEvent(EvtScriptEventId eventId) {
    sprintf(evtScriptResourcePathBuffer, "/event/e%03d/e%03d/scr/e%03d.bf", eventId - eventId % 10, eventId, eventId);
    scrStartScriptByName(0x3EB, evtScriptResourcePathBuffer, 0);
}

const char evtFieldScriptTerminationFormat[0x28] = "kill field script -> [%s]\n";

void evtKillFieldScript(void)
{
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

void evtPathStop(dds3Basic_t *object)
{
    dds3PathBasicControl_t *slotData;

    slotData = dds3ObjectBaseGetPathControl(object);
    if (slotData != 0) {
        dds3PathBasicPauseOn(&slotData->counter);
    }
}

void evtPathStart(dds3Basic_t *object)
{
    dds3PathBasicControl_t *slotData;

    slotData = dds3ObjectBaseGetPathControl(object);
    if (slotData != 0) {
        dds3PathBasicPauseOff(&slotData->counter);
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
EvtPathStatus evtCheckPathStat(dds3Basic_t *obj) {
    f32 target = 1.0f;
    dds3Basic_t *slot;
    dds3PathBasicControl_t *handle;
    EvtPathDirection direction;

    if (obj == NULL) {
        return EVT_PATH_STATUS_INVALID;
    }
    slot = dds3ObjectBaseGetControlBasic(obj, DDS3_OBJECT_DATA_SLOT);
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

    if (obj == 0) {
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
