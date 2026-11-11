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

/* Allocate the object's data and 18 empty per-kind lists; return success. */
Dds3BasicCreateResult dds3CreateWorldObjectData(dds3Basic_t *object) {
    evtWorldTable_t *data = sdfChipAllocDirect(sizeof(*data));
    evtWorldSlot_t *lists;
    s32 listIndex;

    if (data != NULL) {
        data->areaRoomKey = DDS3_AREA_ROOM_KEY_UNASSIGNED;
        data->drawEnabled = 1;
        data->resource = 0;
        data->slots = NULL;
        data->cameraObject = 0;
        data->playerObject = 0;
        data->fieldResourceHandle = NULL;
        data->sceneResourceHandle = 0;
        data->sceneScript = NULL;
        data->resource = sdfMemAllocDirect(0xD8);
        if (data->resource == 0) {
            sdfChipFreeDirect(data);
            return 0;
        }
        lists = (evtWorldSlot_t *)sdfMemLock(data->resource);
        data->slots = lists;
        for (listIndex = 0x11; listIndex >= 0; listIndex--) {
            lists->count = 0;
            lists->head = NULL;
            lists->tail = NULL;
            lists++;
        }
        object->workdata = data;
        return 1;
    }
    return 0;
}

extern void dds3SceneBasicReleaseFieldResources(dds3Basic_t *object);

/* Destroy every node of every list, then release the data block and scene state. */
void dds3DestroyWorldObjectData(dds3Basic_t *object) {
    evtWorldTable_t *data = object->workdata;
    s32 listIndex;

    if (data != NULL) {
        for (listIndex = 0; listIndex <= 0x11; listIndex++) {
            while (data->slots[listIndex].head != NULL) {
                dds3DeleteSceneBasicData(data->slots[listIndex].head);
            }
        }
        if (data->resource != 0) {
            sdfMemFreeDirect(data->resource);
        }
        dds3SceneBasicReleaseFieldResources(object);
        dds3SceneBasicReleaseScriptFile(object);
        sdfChipFreeDirect(data);
    }
}

/* Cache the next link before calling an update that may remove the current node. */
u32 scenemainprocuder(dds3Basic_t *object) {
    evtWorldTable_t *data = object->workdata;
    dds3Basic_t *node;
    dds3Basic_t *next;
    s32 listIndex;

    if (data == NULL) {
        return 0;
    }
    for (listIndex = 0; listIndex <= 0x11; listIndex++) {
        if (data->slots[listIndex].count != 0) {
            node = data->slots[listIndex].head;
            if (node->ops != NULL && node->ops->mainprocuder != NULL) {
                do {
                    next = node->next;
                    node->ops->mainprocuder(node);
                    node = next;
                } while (next != NULL);
            }
        }
    }
    return 1;
}

extern void drawSceneBasicList(void);

/* Only kind 2 is drawn here; the remaining kinds participate in update traversal. */
u32 scenedrawprocuder(dds3Basic_t *object) {
    evtWorldTable_t *data = object->workdata;
    dds3Basic_t *node;
    dds3Basic_t *next;

    if (data == NULL) {
        return 0;
    }
    if (data->drawEnabled == 0) {
        return 1;
    }
    if (data->slots[2].count == 0) {
        return 1;
    }
    node = data->slots[2].head;
    if (node->ops == NULL) {
        return 1;
    }
    if (node->ops->drawprocuder == NULL) {
        return 1;
    }
    drawSceneBasicList();
    do {
        next = node->next;
        node->ops->drawprocuder(node);
        node = next;
    } while (next != NULL);
    return 1;
}

void dds3VisibleSceneBasic(dds3Basic_t *object, s8 enabled) {
    if (((evtWorldTable_t *)object->workdata) != NULL) {
        ((evtWorldTable_t *)object->workdata)->drawEnabled = (s32)enabled;
    }
}
#include "dds3_world_secondary.h"
extern dds3Basic_t *dds3AllocBasic(u32 kind);

/* Create a member in the current secondary world's list for this kind. */
dds3Basic_t *dds3AddSceneBasicData(s32 worldKind) {
    dds3Basic_t *world;
    dds3Basic_t *node;
    evtWorldSlot_t *list;

    if ((u32)(worldKind - 2) >= 16) {
        return NULL;
    }
    world = dds3GetControlScene();
    if (world == NULL) {
        return NULL;
    }
    list = &((evtWorldTable_t *)world->workdata)->slots[worldKind];
    node = dds3AllocBasic(worldKind);
    if (node == NULL) {
        return NULL;
    }
    node->owner = world;
    if (list->tail == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        node->previous = list->tail;
        list->tail = node;
    }
    list->count++;
    return node;
}

extern void dds3DeleteBasic(dds3Basic_t *node);

/* Unlink a world list node from its kind's list in the owner's data and destroy it. */
void dds3DeleteSceneBasicData(dds3Basic_t *node) {
    evtWorldSlot_t *list;

    if (node != NULL && (u32)((node->kindTag >> 24) - 2) < 0x10) {
        list = &((evtWorldTable_t *)node->owner->workdata)->slots[(node->kindTag >> 24)];
        if (list->head == node) {
            list->head = node->next;
        }
        if (list->tail == node) {
            list->tail = node->previous;
        }
        dds3DeleteBasic(node);
        list->count--;
    }
}


dds3Basic_t *dds3ReplaceActiveCameraSceneBasic(dds3Basic_t *object, dds3Basic_t *cameraObject) {
    evtWorldTable_t *data;
    dds3Basic_t *previous;

    data = ((evtWorldTable_t *)object->workdata);
    previous = dds3GetActiveCameraSceneBasic(object);
    data->cameraObject = cameraObject;
    return previous;
}

dds3Basic_t *dds3GetActiveCameraSceneBasic(dds3Basic_t *object) {
    return ((evtWorldTable_t *)object->workdata)->cameraObject;
}


dds3Basic_t *dds3ReplaceActiveUnitSceneBasic(dds3Basic_t *object, dds3Basic_t *playerObject) {
    evtWorldTable_t *data;
    dds3Basic_t *previous;

    data = ((evtWorldTable_t *)object->workdata);
    previous = dds3GetActiveUnitSceneBasic(object);
    data->playerObject = playerObject;
    return previous;
}

dds3Basic_t *dds3GetActiveUnitSceneBasic(dds3Basic_t *object) {
    return ((evtWorldTable_t *)object->workdata)->playerObject;
}

/* Find the node with `key` in the selected world-kind slot, or NULL. */
dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *object, u32 key, s32 worldKind) {
    dds3Basic_t *node;

    if (((evtWorldTable_t *)object->workdata)->slots[worldKind].count == 0) {
        return NULL;
    }
    node = ((evtWorldTable_t *)object->workdata)->slots[worldKind].head;
    do {
        if (key == node->key) {
            return node;
        }
        node = node->next;
    } while (node != NULL);
    return NULL;
}


struct WorldIndexNode *dds3SceneBasicGetList(dds3Basic_t *object, s32 kind) {
    evtWorldTable_t *data = object->workdata;
    struct WorldIndexNode *indexObject;
    dds3Basic_t *node;

    if (data->slots[kind].count == 0) {
        return NULL;
    }
    indexObject = dds3GetBasicArrayHandle(0);
    node = data->slots[kind].head;
    do {
        dds3AddBasicArray((dds3BasicArrayHandle_t *)indexObject, 1);
        dds3SetBasicArray((dds3BasicArrayHandle_t *)indexObject, node);
        node = node->next;
    } while (node != NULL);
    return indexObject;
}

void dds3SceneBasicMakeScene(dds3Basic_t *object, const char *resourcePath) {
    evtWorldTable_t *data;
    const struct sdfMemHandle_t *resourceHandle;

    data = ((evtWorldTable_t *)object->workdata);
    resourceHandle = fldLoadAreaRoomResources(resourcePath);
    data->fieldResourceHandle = resourceHandle;
}

void dds3SceneBasicMakeScene_FromMemory(dds3Basic_t *object, u32 area, u32 room,
                                    u8 *resource0Data, u8 *resource1Data,
                                    const SdfTextureOffsetListHeader *textureOffsets, u32 flags) {
    evtWorldTable_t *data;
    const struct sdfMemHandle_t *handle;

    data = ((evtWorldTable_t *)object->workdata);
    handle = fldLoadAreaRoomResourcesFromRequest(area, room, resource0Data, resource1Data,
                                                 textureOffsets, flags);
    data->fieldResourceHandle = handle;
}
struct sdfMemHandle_t;


extern void fldReleaseFieldResources(void);

/* Release globally owned field resources before clearing their borrowed alias.
 * The scene resource handle/address are released separately. */
void dds3SceneBasicReleaseFieldResources(dds3Basic_t *worldNode) {
    evtWorldTable_t *worldData;

    worldData = (evtWorldTable_t *)worldNode->workdata;
    fldReleaseFieldResources();
    worldData->fieldResourceHandle = NULL;
}

/* NULL preserves the current resource; a non-NULL replacement releases it
 * before loading, so failure leaves no scene script attached. */
Dds3SceneScriptAttachResult evtLoadSceneResourceFrom(
    dds3Basic_t *worldNode, const char *resourceName) {
    evtWorldTable_t *worldData;
    struct sdfMemHandle_t *resourceHandle;
    u32 resolvedAddress;
    void *resourceAddress;

    worldData = (evtWorldTable_t *)worldNode->workdata;
    if (resourceName == NULL) {
        return DDS3_SCENE_SCRIPT_ATTACH_FAILED;
    }
    if (worldData->sceneResourceHandle != 0) {
        dds3SceneBasicReleaseScriptFile(worldNode);
    }
    resourceHandle = sdfDevLoadFileEx(resourceName, &resolvedAddress, 0);
    resourceAddress = (void *)resolvedAddress;
    if (resourceAddress == NULL) {
        return DDS3_SCENE_SCRIPT_ATTACH_FAILED;
    }
    worldData->sceneResourceHandle = resourceHandle;
    worldData->sceneScript = (const struct scrHeader_t *)resourceAddress;
    return DDS3_SCENE_SCRIPT_ATTACH_SUCCEEDED;
}

/* Attach a memory-backed script. NULL input preserves the current resource. A
 * non-NULL handle releases the old resource before locking; success transfers
 * handle ownership to the scene, while lock failure leaves the new handle at
 * the caller and does not restore the previous resource. */
Dds3SceneScriptAttachResult dds3SceneBasicLoadScriptFile_FromMem(
    dds3Basic_t *worldNode, struct sdfMemHandle_t *resourceHandle) {
    Dds3SceneScriptAttachResult attachResult = DDS3_SCENE_SCRIPT_ATTACH_FAILED;
    void *resourceAddress;
    evtWorldTable_t *worldData = (evtWorldTable_t *)worldNode->workdata;

    if (resourceHandle == NULL) {
        return attachResult;
    }
    if (worldData->sceneResourceHandle != 0) {
        dds3SceneBasicReleaseScriptFile(worldNode);
    }
    resourceAddress = sdfMemLock(resourceHandle);
    if (resourceAddress != 0) {
        worldData->sceneResourceHandle = resourceHandle;
        worldData->sceneScript = (const struct scrHeader_t *)resourceAddress;
        return DDS3_SCENE_SCRIPT_ATTACH_SUCCEEDED;
    }
    return attachResult;
}

/* Release a stored allocation handle when present, then always clear both
 * handle and resolved address. Safe for an already-cleared resource state. */
void dds3SceneBasicReleaseScriptFile(dds3Basic_t *worldNode) {
    evtWorldTable_t *worldData = (evtWorldTable_t *)worldNode->workdata;

    if (worldData->sceneResourceHandle != 0) {
        sdfMemFreeDirect(worldData->sceneResourceHandle);
    }
    worldData->sceneResourceHandle = 0;
    worldData->sceneScript = NULL;
}

/* Borrow the scene's retained script view to start its named task; do not
 * mutate the scene or release its resource. */
dds3ProcessID_t *evtStartSceneResourceTask(const dds3Basic_t *worldNode, const char *taskName) {
    const evtWorldTable_t *worldData = worldNode->workdata;
    const struct scrHeader_t *sceneScript = worldData->sceneScript;
    ScrProcedureIndex scriptIndex;

    if (sceneScript == NULL) {
        return 0;
    }
    if (taskName == NULL) {
        return 0;
    }
    scriptIndex = scrSearchProcedureName(sceneScript, taskName);
    if (scriptIndex < 0) {
        return 0;
    }
    if (dds3SearchProcessName(taskName) != NULL) {
        return 0;
    }
    /* The nesting keeps ee-gcc from turning this into a sibling call (calls.c
     * tries none inside a loop). */
    do {
        return scrStartScript2(0x3EA, sceneScript, scriptIndex);
    } while (0);
}

/* Destroy the task selected by taskName; NULL or an absent task is a no-op.
 * unusedContext is not read. The nested early return retains the matched
 * jal/epilogue rather than turning the destruction into a sibling call. */
void dds3SceneBasicEndScript(void *unusedContext, const char *taskName) {
    dds3ProcessID_t *task;

    if (taskName != NULL) {
        task = dds3SearchProcessName(taskName);
        if (task == NULL) {
            return;
        }
        dds3KillProcess(task, 0);
    }
}
Dds3SceneMembershipResult dds3SceneBasicExistBasicFromType(
    const dds3Basic_t *object, s32 slotIndex, const dds3Basic_t *targetNode) {
    const dds3Basic_t *node;

    if (object == NULL || targetNode == 0) {
        return DDS3_SCENE_MEMBERSHIP_ABSENT;
    }
    node = ((const evtWorldTable_t *)object->workdata)->slots[slotIndex].head;
    while (node != NULL) {
        if (node == targetNode) {
            return DDS3_SCENE_MEMBERSHIP_PRESENT;
        }
        node = node->next;
    }
    return DDS3_SCENE_MEMBERSHIP_ABSENT;
}

Dds3SceneMembershipResult dds3SceneBasicExistBasic(
    const dds3Basic_t *object, const dds3Basic_t *targetNode) {
    const evtWorldSlot_t *slots;
    s32 i;

    if (object == NULL || targetNode == 0) {
        return DDS3_SCENE_MEMBERSHIP_ABSENT;
    }
    slots = ((const evtWorldTable_t *)object->workdata)->slots;
    for (i = 0; i < 0x12; i++) {
        const dds3Basic_t *node = slots[i].head;

        if (node == NULL) {
            continue;
        }
        do {
            if (node == targetNode) {
                return DDS3_SCENE_MEMBERSHIP_PRESENT;
            }
            node = node->next;
        } while (node != 0);
    }
    return DDS3_SCENE_MEMBERSHIP_ABSENT;
}

void dds3SetMapNo(dds3Basic_t *worldNode, Dds3AreaRoomKey areaRoomKey) {
    if (worldNode != NULL) {
        ((evtWorldTable_t *)worldNode->workdata)->areaRoomKey = areaRoomKey;
    }
}

Dds3AreaRoomKey dds3GetMapNo(const dds3Basic_t *worldNode) {
    if (worldNode == NULL) {
        return DDS3_AREA_ROOM_KEY_UNASSIGNED;
    }
    return ((evtWorldTable_t *)worldNode->workdata)->areaRoomKey;
}

dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *object, s32 slotIndex, const char *name) {
    dds3Basic_t *node;
    u8 *s;
    const u8 *nameBytes = (const u8 *)name;
    s32 i;

    if (object == NULL || name == NULL) {
        return NULL;
    }
    node = ((evtWorldTable_t *)object->workdata)->slots[slotIndex].head;
    while (node != NULL) {
        s = ((u8 *)node->name);
        if (s == NULL) {
            node = node->next;
            continue;
        }
        if (s[0] == '\0') {
            node = node->next;
            continue;
        }
        i = 0;
        if (nameBytes[0] == s[0]) {
            do {
                if (nameBytes[i] == '\0') {
                    return node;
                }
                i++;
            } while (nameBytes[i] == ((u8 *)node->name)[i]);
        }
        node = node->next;
    }
    return NULL;
}

dds3Basic_t *dds3SceneBasicSearchName(dds3Basic_t *object, const char *name) {
    dds3Basic_t *node;
    s32 i;

    if (object == NULL || name == NULL) {
        return NULL;
    }
    for (i = 0; i < 0x12; i++) {
        node = dds3SceneBasicSearchNameAndType(object, i, name);
        if (node != NULL) {
            return node;
        }
    }
    return NULL;
}
