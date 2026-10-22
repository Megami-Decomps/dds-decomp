#include "common.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "dds3_world_selection.h"
#include "dds3_world_lifecycle.h"

extern dds3Basic_t *dds3TheWorld;

void dds3DeleteBasic(dds3Basic_t *node);
dds3Basic_t *dds3AllocBasic(u32 kind);
void dds3DeleteAllBasicArray(dds3BasicArrayHandle_t *object);
void dds3DeleteBasicArray(dds3BasicArrayHandle_t *iterator);

#define DDS3_WORLD_NODE_KIND 1
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
        ((WorldInfo *)dds3TheWorld->data)->primaryObject = primaryWorldNode;
    }
}

/* Return the selected primary object, or NULL when no world is active. */
dds3Basic_t *dds3GetActiveScene(void) {
    dds3Basic_t *world;

    world = dds3TheWorld;
    if (world == NULL) {
        return NULL;
    }
    return ((WorldInfo *)world->data)->primaryObject;
}

/* Set the secondary world node without releasing it; no world is a no-op. */
void dds3SetControlScene(dds3Basic_t *secondaryWorldNode) {
    if (dds3TheWorld != NULL) {
        ((WorldInfo *)dds3TheWorld->data)->secondaryObject = secondaryWorldNode;
    }
}

/* Return the selected secondary object, or NULL when no world is active. */
dds3Basic_t *dds3GetControlScene(void) {
    dds3Basic_t *world;

    world = dds3TheWorld;
    if (world == NULL) {
        return NULL;
    }
    return ((WorldInfo *)world->data)->secondaryObject;
}

/* Create a kind-1 world node and append it at the tail.
 * Missing active world or failed creation returns NULL without changing the list. */
dds3Basic_t *dds3AddScene(void) {
    WorldInfo *worldInfo;
    dds3Basic_t *worldNode;

    if (dds3TheWorld == NULL) {
        return NULL;
    }
    worldInfo = (WorldInfo *)dds3TheWorld->data;
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
    WorldInfo *worldInfo;

    if (worldNode == NULL) {
        return;
    }
    if (dds3TheWorld == NULL) {
        return;
    }
    worldInfo = (WorldInfo *)dds3TheWorld->data;
    if (worldInfo->firstNode == worldNode) {
        worldInfo->firstNode = worldNode->next;
    }
    if (worldInfo->lastNode == worldNode) {
        worldInfo->lastNode = worldNode->previous;
    }
    if (worldInfo->primaryObject == worldNode) {
        worldInfo->primaryObject = NULL;
    }
    if (worldInfo->secondaryObject == worldNode) {
        worldInfo->secondaryObject = NULL;
    }
    dds3DeleteBasic(worldNode);
}

/* Append a separate index node, then request initialCount entries from the shared
 * value pool. Only the upper-bound check occurs here: negative counts can still
 * produce a linked empty node when the growth helper rejects the request. */
WorldIndexNode *dds3GetBasicArrayHandle(s32 initialCount) {
    WorldInfo *worldInfo;
    WorldIndexNode *indexNode;

    if (dds3TheWorld == NULL) {
        return NULL;
    }
    worldInfo = (WorldInfo *)dds3TheWorld->data;
    if (worldInfo->freeEntryCount < initialCount) {
        return NULL;
    }
    indexNode = sdfChipAllocDirect(sizeof(WorldIndexNode));
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
    WorldInfo *worldInfo;

    if (indexNode == NULL) {
        return;
    }
    if (dds3TheWorld == NULL) {
        return;
    }
    worldInfo = (WorldInfo *)dds3TheWorld->data;
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
    WorldInfo *info;
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
    info = (WorldInfo *)dds3TheWorld->data;
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
    WorldInfo *world;
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
    world = dds3TheWorld->data;
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
s32 dds3SearchDeleteBasicArray(dds3BasicArrayHandle_t *indexNode, dds3Basic_t *targetNode, s32 processAllMatches) {
    s32 processedMatch;

    processedMatch = 0;
    if (dds3GetBasicArrayCount(indexNode) != 0) {
        dds3SetFirstBasicArray(indexNode);
        do {
            if (dds3SetSearchNextBasicArray(indexNode, targetNode) != 1) {
                break;
            }
            dds3DeleteBasicArray(indexNode);
            processedMatch = 1;
        } while (processAllMatches != 0);
    }
    return processedMatch;
}

/* Search from the current cursor without resetting it; leave it on a match.
 * A NULL payload terminates before comparison, even if later entries remain,
 * so a NULL target never matches. Return 1 for a match, otherwise 0. */
s32 dds3SetSearchNextBasicArray(dds3BasicArrayHandle_t *indexNode, const dds3Basic_t *targetNode) {
    u32 candidateWord;

    do {
        candidateWord = dds3GetBasicArray(indexNode);
        if (candidateWord == 0) {
            return 0;
        }
        if ((u32)targetNode == candidateWord) {
            return 1;
        }
    } while (dds3SetNextBasicArray(indexNode) != 0);
    return 0;
}
