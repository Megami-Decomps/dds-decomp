#include "common.h"
#include "scr_integer_parameter_api.h"
#include "scr_command_parameter_index.h"
#include "dds3_basic_lifecycle.h"
#include "ps2asm.h"
#include "eff_inner_float_api.h"
#include "sdf_sif_command.h"
#include "scr_integer_return_value_api.h"
#include "sdf_vu_matrix.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "kernel.h"
#include "eff_transform.h"
#include "dds3obj.h"
#include "sdf.h"
#include "pty_read_roster_stat_api.h"
#include "pty_event_penalty_api.h"
#include "dds3_vu_transform.h"
#include "sdf_heap_api.h"
#include "basic.h"
#include "dds3_world_scene_node_view.h"
#include "dds3_world_value.h"
#include "fpu.h"
#include "dds3_world_counter.h"
#include "dds3_basic_array_cursor.h"
#include "dds3_basic_array_search.h"
#include "dds3_world_selection.h"
#include "dds3_world_lifecycle.h"
#include "fld_area_room_resource_load_api.h"
#include "evt_world_object_api.h"
#include "evt_world.h"
#include "sdf_texture_offset_list.h"
#include "dds3_basic_array_visit.h"
#include "scr.h"
#include "dds3_task_lifecycle.h"
#include "dds3_scene_membership.h"
#include "dds3_area_callback.h"
#include "dds3_object_control_slot.h"
#include "dds3_path.h"
#include "evt_action_object.h"
#include "sdk_ps2.h"
#include "dds3_camera_update_flags.h"
#include "eff_object.h"
#include "sdf_model_packet_callback.h"
#include "dds3_object_flags.h"
#include "sdf_model.h"
#include "sdf_model_node_index_api.h"
#include "sdf_model_eval_api.h"
#include "sdf_model_release_api.h"
#include "evt_gradient_rect.h"
#include "sdf_motion.h"
#include "sdf_motion_next_result.h"
#include "dds3_slot1_handler_api.h"
#include "dds3_object_resource_state.h"
#include "dds3_object_base_resource_view.h"
#include "dds3_object_mode.h"
#include "dds3Admin.h"
#include "mdl.h"
#include "evt_unit.h"
#include "evt_script_model_api.h"

s32 MoverBasicConstructor(dds3Basic_t *obj) {
    obj->workdata = sdfChipAllocDirect(0x10);
    memset(obj->workdata, 0, 0x10);
    return 1;
}
extern void dds3MoverBasicFreePath(dds3Basic_t *slotObject);

void MoverBasicDestructor(dds3Basic_t *slotObject) {
    Dds3SlotResource *resource;

    resource = slotObject->workdata;
    dds3MoverBasicFreePath(slotObject);
    dds3ObjectBaseSetControlBasicType(resource->target, 0, DDS3_OBJECT_DATA_SLOT);
    sdfChipFreeDirect(resource);
}
extern void dds3BackupPoseSet(dds3PoseSet_t *transform);


/* Apply enabled absolute path channels and step the path counter once, or apply
 * a callback-built relative transform when its return value is exactly 1.
 * Always return 1, even when no channel or callback transform was applied. */
s32 MoverBasicMainprocuder(dds3Basic_t *moverNode)
{
    f32 pathVector[4];
    WorldTransformParams transformParams;
    dds3PoseSet_t relativeTransform;
    Dds3SlotResource *slotResource = moverNode->workdata;
    dds3Basic_t *targetObject = slotResource->target;
    dds3PoseSet_t *inner = targetObject->poseset;
    Dds3MoverUpdate updateCallback;
    CameraData *cameraData;
    f32 fieldOfView;

    if (slotResource->path != NULL) {
        if (slotResource->path->counter.flags & DDS3_PATH_POSITION_CHANNEL) {
            dds3PathBasicControlGetPos(slotResource->path);
            SDF_VU0_STOREVECTOR(pathVector);
            dds3SetPosPoseSet(targetObject, (u128 *)pathVector);
            if ((targetObject->kindTag >> 24) == EFF_WORLD_KIND_EFFECT_TRANSFORM) {
                SDF_VU0_COPYVECTOR(&((EffectTransformData *)targetObject->workdata)->position, pathVector);
            }
        }
        if (slotResource->path->counter.flags & DDS3_PATH_ROTATION_CHANNEL) {
            dds3PathBasicControlGetRot(slotResource->path);
            SDF_VU0_STOREVECTOR(pathVector);
            dds3SetRotPoseSet(targetObject, (u128 *)pathVector);
        }
        if (slotResource->path->counter.flags & DDS3_PATH_SCALAR_CHANNEL) {
            if ((targetObject->kindTag >> 24) == EFF_WORLD_KIND_CAMERA) {
                fieldOfView = dds3PathBasicControlGetFovy(slotResource->path);
                cameraData = targetObject->workdata;
                cameraData->fieldOfView = fieldOfView;
                cameraData->fovUpdatePending |= DDS3_CAMERA_UPDATE_FOV_BIT;
            }
        }
        if (slotResource->path->counter.flags & DDS3_PATH_WORLD_TRANSFORM_CHANNEL) {
            if ((targetObject->kindTag >> 24) == EFF_WORLD_KIND_LIGHT) {
                dds3PathBasicControlGetTransform(slotResource->path, &transformParams);
                dds3SetLightObjectPathResoruce(targetObject, &transformParams);
            }
        }
        /* Advance even when no applicable channel was enabled for this target. */
        dds3PathBasicControlStep(&slotResource->path->counter);
    } else {
        updateCallback = slotResource->update;
        /* This unidentified word is cleared even when no callback exists. */
        inner->unkC8 = 0;
        if (updateCallback != NULL) {
            dds3BackupPoseSet(&relativeTransform);
            if (updateCallback(&relativeTransform, targetObject) == DDS3_MOVER_UPDATE_APPLY_RELATIVE_TRANSFORM) {
                dds3GetSclPoseSetMCR(targetObject, (u128 *)relativeTransform.scale);
                dds3AddRotPoseSet(targetObject, (u128 *)&relativeTransform.rotation);
                dds3GetPosPoseSetMCR(targetObject, (u128 *)relativeTransform.pos);
            }
        }
    }
    return 1;
}
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

INCLUDE_SDATA(const s32, "basic/dds3MoverBasic", dds3MoverCount);

