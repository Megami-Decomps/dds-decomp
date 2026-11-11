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

/* Allocate and clear the world node's 0x28-byte WorldInfo payload. The native
   callback stores the payload on the node before initializing its fields. */
s32 worldconstructor(dds3Basic_t *object) {
    dds3WorldBasic_t *entry;

    entry = (dds3WorldBasic_t *)sdfChipAllocDirect(0x28);
    if (entry == NULL) {
        return 0;
    }
    object->workdata = entry;
    entry->firstNode = NULL;
    entry->lastNode = NULL;
    entry->activeScene = NULL;
    entry->controlscene = NULL;
    entry->entryAllocation = NULL;
    entry->entries = NULL;
    entry->entryCapacity = 0;
    entry->freeHeadIndex = -1;
    entry->freeEntryCount = 0;
    entry->firstIndex = NULL;
    entry->lastIndex = NULL;
    entry->freeTailIndex = -1;
    return 1;
}

void worlddestructor(dds3Basic_t *object)
{
    dds3WorldBasic_t *entry = object->workdata;

    if (entry == NULL) {
        return;
    }
    while (entry->firstNode != NULL) {
        dds3DeleteScene(entry->firstNode);
    }
    while (entry->firstIndex != NULL) {
        dds3FreeBasicArrayHandle(entry->firstIndex);
    }
    if (entry->entryAllocation != NULL) {
        sdfMemFreeDirect(entry->entryAllocation);
    }
    sdfChipFreeDirect(entry);
}

u32 worldmainprocuder(dds3Basic_t *obj) {
    dds3WorldBasic_t *entry = obj->workdata;
    dds3Basic_t *callbackTarget;

    callbackTarget = ((WorldInfoSceneNodeView *)entry)->activeScene;
    if (callbackTarget != NULL) {
        dds3CallBasicMainProc(callbackTarget);
    }
    return 1;
}

u32 worlddrawprocuder(dds3Basic_t *obj) {
    dds3WorldBasic_t *entry = obj->workdata;
    dds3Basic_t *callbackTarget;

    callbackTarget = ((WorldInfoSceneNodeView *)entry)->activeScene;
    if (callbackTarget != NULL) {
        dds3CallBasicDrawProc(callbackTarget);
    }
    return 1;
}

extern dds3Basic_t *dds3AllocBasic(u32 kind);
extern dds3Basic_t *dds3TheWorld;

dds3Basic_t *dds3AddWorld(s32 capacity) {
    dds3Basic_t *world;
    dds3WorldBasic_t *info;
    struct sdfMemHandle_t *allocation;
    s32 index;

    if (dds3TheWorld != NULL) {
        return NULL;
    }
    if ((u32)capacity - 0x20u >= 0x7FE0u) {
        return NULL;
    }

    world = dds3AllocBasic(0);
    info = world->workdata;
    allocation = sdfMemAllocDirect(capacity * 8);
    info->entryAllocation = allocation;
    info->entries = (WorldValueEntry *)sdfMemLock(allocation);
    info->entryCapacity = capacity;

    for (index = 0; index < capacity; index++) {
        if (index == 0) {
            info->entries[index].previousIndex = -1;
        } else {
            info->entries[index].previousIndex = index - 1;
        }
        if (index == capacity - 1) {
            info->entries[index].nextIndex = -1;
        } else {
            info->entries[index].nextIndex = index + 1;
        }
        info->entries[index].value = 0;
    }
    info->freeHeadIndex = 0;
    info->freeTailIndex = capacity - 1;
    info->freeEntryCount = capacity;
    dds3TheWorld = world;
    return world;
}
extern dds3Basic_t *dds3TheWorld;

void dds3DeleteBasic(dds3Basic_t *node);

dds3Basic_t *dds3AllocBasic(u32 kind);



void dds3DeleteBasicArray(dds3BasicArrayHandle_t *iterator);







void dds3DeleteAllBasicArray(dds3BasicArrayHandle_t *object);

#define DDS3_WORLD_NODE_KIND 1
#define DDS3_WORLD_INDEX_NODE_BYTES 0x10
#define DDS3_WORLD_INVALID_ENTRY_INDEX (-1)

/* Destroy the active world if present; clear the global only after destruction. */
void dds3DeleteWorld(void) {
    dds3Basic_t *world;

    world = dds3TheWorld;
    if (world != NULL) {
        dds3DeleteBasic(world);
        dds3TheWorld = NULL;
    }
}

/* Set the primary world node without releasing it; no world is a no-op. */
void dds3SetActiveScene(dds3Basic_t *primaryWorldNode) {
    if (dds3TheWorld != NULL) {
        ((WorldInfoSceneNodeView *)dds3TheWorld->workdata)->activeScene = primaryWorldNode;
    }
}

/* Return the selected primary object, or NULL when no world is active. */
dds3Basic_t *dds3GetActiveScene(void) {
    dds3Basic_t *world;

    world = dds3TheWorld;
    if (world == NULL) {
        return NULL;
    }
    return ((WorldInfoSceneNodeView *)world->workdata)->activeScene;
}

/* Set the secondary world node without releasing it; no world is a no-op. */
void dds3SetControlScene(dds3Basic_t *secondaryWorldNode) {
    if (dds3TheWorld != NULL) {
        ((WorldInfoSceneNodeView *)dds3TheWorld->workdata)->controlscene = secondaryWorldNode;
    }
}

/* Return the selected secondary object, or NULL when no world is active. */
dds3Basic_t *dds3GetControlScene(void) {
    dds3Basic_t *world;

    world = dds3TheWorld;
    if (world == NULL) {
        return NULL;
    }
    return ((WorldInfoSceneNodeView *)world->workdata)->controlscene;
}

/* Create a kind-1 world node and append it at the tail.
 * Missing active world or failed creation returns NULL without changing the list. */
dds3Basic_t *dds3AddScene(void) {
    dds3WorldBasic_t *worldInfo;
    dds3Basic_t *worldNode;

    if (dds3TheWorld == NULL) {
        return NULL;
    }
    worldInfo = (dds3WorldBasic_t *)dds3TheWorld->workdata;
    worldNode = dds3AllocBasic(DDS3_WORLD_NODE_KIND);
    if (worldNode == NULL) {
        return NULL;
    }
    if (worldInfo->lastNode == NULL) {
        worldInfo->firstNode = worldNode;
        worldInfo->lastNode = worldNode;
    } else {
        worldInfo->lastNode->next = worldNode;
        worldNode->previous = worldInfo->lastNode;
        worldInfo->lastNode = worldNode;
    }
    return worldNode;
}

/* Clear boundary and selected-object references before generic node destruction.
 * NULL node or missing active world leaves the node untouched. */
void dds3DeleteScene(dds3Basic_t *worldNode) {
    dds3WorldBasic_t *worldInfo;
    WorldInfoSceneNodeView *sceneNodeView;

    if (worldNode == NULL) {
        return;
    }
    if (dds3TheWorld == NULL) {
        return;
    }
    worldInfo = (dds3WorldBasic_t *)dds3TheWorld->workdata;
    sceneNodeView = (WorldInfoSceneNodeView *)worldInfo;
    if (worldInfo->firstNode == worldNode) {
        worldInfo->firstNode = worldNode->next;
    }
    if (worldInfo->lastNode == worldNode) {
        worldInfo->lastNode = worldNode->previous;
    }
    if (sceneNodeView->activeScene == worldNode) {
        sceneNodeView->activeScene = NULL;
    }
    if (sceneNodeView->controlscene == worldNode) {
        sceneNodeView->controlscene = NULL;
    }
    dds3DeleteBasic(worldNode);
}

/* Append a separate index node, then request initialCount entries from the shared
 * value pool. Only the upper-bound check occurs here: negative counts can still
 * produce a linked empty node when the growth helper rejects the request. */
WorldIndexNode *dds3GetBasicArrayHandle(s32 initialCount) {
    dds3WorldBasic_t *worldInfo;
    WorldIndexNode *indexNode;

    if (dds3TheWorld == NULL) {
        return NULL;
    }
    worldInfo = (dds3WorldBasic_t *)dds3TheWorld->workdata;
    if (worldInfo->freeEntryCount < initialCount) {
        return NULL;
    }
    indexNode = sdfChipAllocDirect(DDS3_WORLD_INDEX_NODE_BYTES);
    if (indexNode == NULL) {
        return NULL;
    }
    indexNode->previous = NULL;
    indexNode->next = NULL;
    indexNode->headIndex = DDS3_WORLD_INVALID_ENTRY_INDEX;
    indexNode->tailIndex = DDS3_WORLD_INVALID_ENTRY_INDEX;
    indexNode->cursorIndex = DDS3_WORLD_INVALID_ENTRY_INDEX;
    indexNode->entryCount = 0;
    if (worldInfo->lastIndex == NULL) {
        worldInfo->firstIndex = indexNode;
        worldInfo->lastIndex = indexNode;
    } else {
        worldInfo->lastIndex->next = indexNode;
        indexNode->previous = worldInfo->lastIndex;
        worldInfo->lastIndex = indexNode;
    }
    dds3AddBasicArray((dds3BasicArrayHandle_t *)indexNode, initialCount);
    return indexNode;
}

/* Return the node's value entries to the shared pool, unlink the index node,
 * then release its block. NULL node or missing active world is a no-op. */
void dds3FreeBasicArrayHandle(WorldIndexNode *indexNode) {
    dds3WorldBasic_t *worldInfo;

    if (indexNode == NULL) {
        return;
    }
    if (dds3TheWorld == NULL) {
        return;
    }
    worldInfo = (dds3WorldBasic_t *)dds3TheWorld->workdata;
    dds3DeleteAllBasicArray((dds3BasicArrayHandle_t *)indexNode);
    if (indexNode->previous == NULL) {
        worldInfo->firstIndex = indexNode->next;
    } else {
        indexNode->previous->next = indexNode->next;
    }
    if (indexNode->next == NULL) {
        worldInfo->lastIndex = indexNode->previous;
    } else {
        indexNode->next->previous = indexNode->previous;
    }
    sdfChipFreeDirect(indexNode);
}

/* Preserve native-width tail copies; see docs/idioms.md,
 * "Trace-justified world free-list tail publication". */
void dds3DeleteBasicArray(dds3BasicArrayHandle_t *iterator) {
    dds3WorldBasic_t *info;
    WorldValueEntry *entries;
    s16 previous;
    s16 next;

    if (iterator == NULL) {
        return;
    }
    if (iterator->entryCount == 0) {
        return;
    }
    if (iterator->cursorIndex < 0) {
        return;
    }
    if (dds3TheWorld == NULL) {
        return;
    }
    info = (dds3WorldBasic_t *)dds3TheWorld->workdata;
    entries = info->entries;
    previous = entries[iterator->cursorIndex].previousIndex;
    entries[iterator->cursorIndex].value = 0;
    next = entries[iterator->cursorIndex].nextIndex;
    if (previous < 0) {
        iterator->headIndex = next;
    } else {
        entries[previous].nextIndex = next;
    }
    if (next < 0) {
        iterator->tailIndex = previous;
    } else {
        entries[next].previousIndex = previous;
    }
    entries[iterator->cursorIndex].previousIndex = -1;
    entries[iterator->cursorIndex].nextIndex = -1;
    if (info->freeTailIndex < 0) {
        info->freeHeadIndex = iterator->cursorIndex;
        info->freeTailIndex = iterator->cursorIndex;
    } else {
        entries[info->freeTailIndex].nextIndex = iterator->cursorIndex;
        entries[iterator->cursorIndex].previousIndex = info->freeTailIndex;
        info->freeTailIndex = iterator->cursorIndex;
    }
    info->freeEntryCount++;
    iterator->cursorIndex = next;
    iterator->entryCount--;
}

/* Return this index record's values to the active world's free-entry chain. */
void dds3DeleteAllBasicArray(dds3BasicArrayHandle_t *object) {
    dds3WorldBasic_t *world;
    s32 entryIndex;

    if (object == NULL) {
        return;
    }
    if (object->entryCount == 0) {
        return;
    }
    if (dds3TheWorld == NULL) {
        return;
    }
    world = dds3TheWorld->workdata;
    entryIndex = object->headIndex;
    while (entryIndex >= 0) {
        world->entries[entryIndex].value = 0;
        entryIndex = world->entries[entryIndex].nextIndex;
    }
    if (world->freeTailIndex < 0) {
        world->freeHeadIndex = object->headIndex;
        world->freeTailIndex = object->tailIndex;
    } else {
        world->entries[world->freeTailIndex].nextIndex = object->headIndex;
        world->entries[object->headIndex].previousIndex = world->freeTailIndex;
        world->freeTailIndex = object->tailIndex;
    }
    world->freeEntryCount += object->entryCount;
    object->entryCount = 0;
    object->headIndex = DDS3_WORLD_INVALID_ENTRY_INDEX;
    object->tailIndex = DDS3_WORLD_INVALID_ENTRY_INDEX;
    object->cursorIndex = DDS3_WORLD_INVALID_ENTRY_INDEX;
}

/* Reset the cursor and remove this node's matching address once, or all matches
 * when processAllMatches is nonzero. Return whether any match was processed. */
Dds3BasicArrayMatchResult dds3SearchDeleteBasicArray(dds3BasicArrayHandle_t *indexNode, const dds3Basic_t *targetNode, s32 processAllMatches) {
    Dds3BasicArrayMatchResult processedMatch;

    processedMatch = DDS3_BASIC_ARRAY_NO_MATCH;
    if (dds3GetBasicArrayCount(indexNode) != 0) {
        dds3SetFirstBasicArray(indexNode);
        do {
            if (dds3SetSearchNextBasicArray(indexNode, targetNode) != DDS3_BASIC_ARRAY_MATCH_FOUND) {
                break;
            }
            dds3DeleteBasicArray(indexNode);
            processedMatch = DDS3_BASIC_ARRAY_MATCH_FOUND;
        } while (processAllMatches != 0);
    }
    return processedMatch;
}

/* Search from the current cursor without resetting it; leave it on a match.
 * A NULL payload terminates before comparison, even if later entries remain,
 * so a NULL target never matches. Return 1 for a match, otherwise 0. */
Dds3BasicArrayMatchResult dds3SetSearchNextBasicArray(dds3BasicArrayHandle_t *indexNode, const dds3Basic_t *targetNode) {
    dds3Basic_t *candidate;

    do {
        candidate = dds3GetBasicArray(indexNode);
        if (candidate == NULL) {
            return DDS3_BASIC_ARRAY_NO_MATCH;
        }
        if (targetNode == candidate) {
            return DDS3_BASIC_ARRAY_MATCH_FOUND;
        }
    } while (dds3SetNextBasicArray(indexNode) != DDS3_BASIC_ARRAY_CURSOR_EXHAUSTED);
    return DDS3_BASIC_ARRAY_NO_MATCH;
}
struct WorldIndexNode;

extern dds3Basic_t *dds3TheWorld;

u32 dds3AddBasicArray(dds3BasicArrayHandle_t *object, s32 count) {
    dds3WorldBasic_t *world;
    s16 firstIndex;
    s16 lastIndex;
    s32 i;

    if (count == 0) {
        return 1;
    }
    if (dds3TheWorld == NULL) {
        return 0;
    }
    world = dds3TheWorld->workdata;
    if (world->freeEntryCount < count || count < 0) {
        return 0;
    }
    if (object == NULL) {
        return 0;
    }
    firstIndex = world->freeHeadIndex;
    for (i = 0; i < count - 1; i++) {
        world->freeHeadIndex = world->entries[world->freeHeadIndex].nextIndex;
    }
    lastIndex = world->freeHeadIndex;
    world->freeHeadIndex = world->entries[lastIndex].nextIndex;
    if (world->freeHeadIndex == -1) {
        world->freeTailIndex = -1;
    } else {
        world->entries[lastIndex].nextIndex = -1;
        world->entries[world->freeHeadIndex].previousIndex = -1;
    }
    world->freeEntryCount -= count;
    if (object->tailIndex < 0) {
        object->tailIndex = lastIndex;
        object->headIndex = firstIndex;
    } else {
        world->entries[object->tailIndex].nextIndex = firstIndex;
        world->entries[firstIndex].previousIndex = object->tailIndex;
        object->tailIndex = lastIndex;
    }
    object->cursorIndex = firstIndex;
    object->entryCount += count;
    return 1;
}

u16 dds3GetBasicArrayCount(const dds3BasicArrayHandle_t *object) {
    u16 value;

    value = 0;
    if (object != 0) {
        value = object->entryCount;
    }
    return value;
}

u32 dds3SetBasicArray(dds3BasicArrayHandle_t *object, dds3Basic_t *node) {
    if (object->entryCount == 0) {
        return 0;
    }
    if (object->cursorIndex < 0) {
        return 0;
    }
    ((dds3WorldBasic_t *)dds3TheWorld->workdata)->entries[object->cursorIndex].value = (s32)(u32)node;
    return 1;
}

dds3Basic_t *dds3GetBasicArray(const dds3BasicArrayHandle_t *object) {
    if (object->entryCount == 0) {
        return 0;
    }
    if (object->cursorIndex < 0) {
        return 0;
    }
    return (dds3Basic_t *)(u32)((dds3WorldBasic_t *)dds3TheWorld->workdata)->entries[object->cursorIndex].value;
}

/* Signed comparison via complement-and-shift: zero counts as nonnegative. */
Dds3BasicArrayCursorResult dds3SetFirstBasicArray(dds3BasicArrayHandle_t *object) {
    object->cursorIndex = object->headIndex;
    return (u32)~(s32)object->headIndex >> 0x1f;
}

Dds3BasicArrayCursorResult dds3SetNextBasicArray(dds3BasicArrayHandle_t *object) {
    if (object->cursorIndex < 0) {
        return DDS3_BASIC_ARRAY_CURSOR_EXHAUSTED;
    }
    object->cursorIndex = ((dds3WorldBasic_t *)dds3TheWorld->workdata)->entries[object->cursorIndex].nextIndex;
    return (u32)~(s32)object->cursorIndex >> 0x1f;
}

/* Exact zero stops traversal; any nonzero callback result continues. */
Dds3BasicArrayVisitResult dds3ForEachBasicArray(
    dds3BasicArrayHandle_t *object, Dds3BasicArrayVisitCallback callback) {
    Dds3BasicArrayCursorResult more;

    if (callback == NULL) {
        return DDS3_BASIC_ARRAY_VISIT_CONTINUE;
    }
    if (dds3GetBasicArrayCount(object) == 0) {
        return DDS3_BASIC_ARRAY_VISIT_CONTINUE;
    }
    for (more = dds3SetFirstBasicArray(object); more != DDS3_BASIC_ARRAY_CURSOR_EXHAUSTED;
         more = dds3SetNextBasicArray(object)) {
        if (callback(dds3GetBasicArray(object)) == DDS3_BASIC_ARRAY_VISIT_STOP) {
            return DDS3_BASIC_ARRAY_VISIT_STOP;
        }
    }
    return DDS3_BASIC_ARRAY_VISIT_CONTINUE;
}
