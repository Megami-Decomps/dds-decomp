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

s32 AreaBasicConstructor(dds3Basic_t *object) {
    WorldIndexNode **indexNodes;
    u32 i;

    indexNodes = sdfChipAllocDirect(8);
    object->workdata = indexNodes;
    for (i = 0; i < 2; i++) {
        indexNodes[i] = dds3GetBasicArrayHandle(0);
    }
    return 1;
}

void AreaBasicDestructor(dds3Basic_t *object) {
    WorldIndexNode **indexNodes;
    u32 i;

    indexNodes = object->workdata;
    dds3ForEachBasicArray((dds3BasicArrayHandle_t *)indexNodes[0], CB_ReleaseObjectBasic);
    dds3ForEachBasicArray((dds3BasicArrayHandle_t *)indexNodes[1], CB_ReleaseObjectBasic);
    for (i = 0; i < 2; i++) {
        dds3FreeBasicArrayHandle(indexNodes[i]);
    }
    sdfChipFreeDirect(indexNodes);
}

u32 AreaBasicMainprocuder(void) {
    return 1;
}

s32 AreaBasicDrawprocuder(dds3Basic_t *object) {
    WorldIndexNode **indexNodes = object->workdata;

    dds3ForEachBasicArray((dds3BasicArrayHandle_t *)indexNodes[0], CB_BasicDrawProcuder);
    return 1;
}
/* Exchange the area's slot, retaining the native selector pair (0, 5). */
Dds3BasicArrayVisitResult CB_ReleaseObjectBasic(dds3Basic_t *areaObject) {
    dds3ObjectBaseSetControlBasicType(areaObject, 0, DDS3_OBJECT_HANDLER_SLOT);
    return DDS3_BASIC_ARRAY_VISIT_CONTINUE;
}

/* Continue without an area callback; otherwise forward its visitor result. */
Dds3BasicArrayVisitResult CB_BasicDrawProcuder(dds3Basic_t *areaObject) {
    Dds3AreaObjectView *area = (Dds3AreaObjectView *)areaObject;
    Dds3BasicArrayVisitResult (*callback)(Dds3AreaObjectView *);

    callback = area->callbackRecord->callback;
    if (callback == NULL) {
        return DDS3_BASIC_ARRAY_VISIT_CONTINUE;
    }
    return callback(area);
}
s32 dds3SelectOwnerWorldIndexSlot(u32 nodeKind) {
    s32 result = 0;

    if (nodeKind >= 4) {
        if (nodeKind >= 8) {
            result = nodeKind == 8;
        }
    }
    return result;
}

dds3Basic_t *dds3AddAreaBasic(s32 key, const char *objectName) {
    dds3Basic_t *obj = dds3AddSceneBasicData(EFF_WORLD_KIND_WORLD_INDEX_OWNER);

    obj->key = key;
    obj->name = objectName;
    return obj;
}

void dds3AreaBasicRegistItem(dds3Basic_t *ownerObject, dds3Basic_t *node) {
    WorldIndexNode *slot =
        dds3AreaBasicGetWorldIndexNode(ownerObject, dds3SelectOwnerWorldIndexSlot(node->kindTag >> 24));

    dds3SetFirstBasicArray((dds3BasicArrayHandle_t *)slot);
    if (dds3SetSearchNextBasicArray((dds3BasicArrayHandle_t *)slot, node) != DDS3_BASIC_ARRAY_MATCH_FOUND) {
        dds3AddBasicArray((dds3BasicArrayHandle_t *)slot, 1);
        dds3SetBasicArray((dds3BasicArrayHandle_t *)slot, node);
    }
}
s32 dds3SelectOwnerWorldIndexSlot(u32 kind);

/* Remove every entry for the node from the owner's world slot selected by the node's kind. */
void dds3AreaBasicGetItemArray(dds3Basic_t *worldIndexOwner, dds3Basic_t *node) {
    WorldIndexNode *slot =
        dds3AreaBasicGetWorldIndexNode(worldIndexOwner, dds3SelectOwnerWorldIndexSlot(node->kindTag >> 24));

    dds3SearchDeleteBasicArray((dds3BasicArrayHandle_t *)slot, node, 0);
    /* Retail retains a JAL tail. The consumed list cursor is no longer needed
     * after removal. */
    slot = NULL;
}

/* Select one of the kind-2 owner's two world index nodes. */
WorldIndexNode *dds3AreaBasicGetWorldIndexNode(dds3Basic_t *worldIndexOwner, s32 slotIndex) {
    WorldIndexNode **slot = worldIndexOwner->workdata;

    slot += slotIndex;
    return *slot;
}
#include "dds3_world_secondary.h"
/* Copy the slot's world-object words (optionally filtered) into a fresh index node. */
WorldIndexNode *dds3AreaBasicArrangeGetItemArray(dds3Basic_t *worldIndexOwner, s32 slotIndex, s32 (*filter)(dds3Basic_t *)) {
    WorldIndexNode *slot = dds3AreaBasicGetWorldIndexNode(worldIndexOwner, slotIndex);
    WorldIndexNode *result;
    dds3Basic_t *value;

    if (dds3GetBasicArrayCount((dds3BasicArrayHandle_t *)slot) == 0) {
        return NULL;
    }
    result = dds3GetBasicArrayHandle(0);
    dds3SetFirstBasicArray((dds3BasicArrayHandle_t *)slot);
    do {
        value = dds3GetBasicArray((dds3BasicArrayHandle_t *)slot);
        if (filter == NULL || filter(value) != 0) {
            dds3AddBasicArray((dds3BasicArrayHandle_t *)result, 1);
            dds3SetBasicArray((dds3BasicArrayHandle_t *)result, value);
        }
    } while (dds3SetNextBasicArray((dds3BasicArrayHandle_t *)slot) != DDS3_BASIC_ARRAY_CURSOR_EXHAUSTED);
    if (dds3GetBasicArrayCount((dds3BasicArrayHandle_t *)result) == 0) {
        dds3FreeBasicArrayHandle(result);
        return NULL;
    }
    return result;
}

dds3Basic_t *dds3GetFirstWorldObjectNodeOfKind2(void) {
    WorldIndexNode *indexObject;
    dds3Basic_t *node;

    indexObject = dds3SceneBasicGetList(dds3GetControlScene(), 2);
    if (indexObject == NULL) {
        return NULL;
    }
    if (dds3GetBasicArrayCount((dds3BasicArrayHandle_t *)indexObject) == 0) {
        return NULL;
    }
    dds3SetFirstBasicArray((dds3BasicArrayHandle_t *)indexObject);
    node = dds3GetBasicArray((dds3BasicArrayHandle_t *)indexObject);
    dds3FreeBasicArrayHandle(indexObject);
    return node;
}
