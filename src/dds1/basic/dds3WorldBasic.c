#include "common.h"
#include "dds3obj.h"

extern World *dds3ActiveWorld;

void sdfReleaseChipBlock(void *arg);
void effObjNodeDestroy(void *arg);
void *dds3CreateWorldNodeForKind(s32 arg);
void *sdfAllocSizeClassBlock(s32 arg);
void dds3GrowWorldValueChain(void *arg, s32 arg1);
void func_00110120(IndexObj *arg);
void func_00110018(IndexObj *arg);
s32 dds3SeekWorldNode(void *arg0, void *arg1);
void *dds3GetWorldValueCount(void *arg0, void *arg1, s32 arg2);
void dds3ResetObjectValueCursor(void *arg);
void *dds3ReadIndexedWorldObjectWord(void *arg);
s32 dds3AdvanceObjectValueCursor(void *arg);

#define DDS3_WORLD_NODE_KIND 1
#define DDS3_WORLD_INDEX_NODE_BYTES 0x10
#define DDS3_WORLD_INVALID_ENTRY_INDEX (-1)

/* Destroy the active world if present; clear the global only after destruction. */
void dds3DestroyWorld(void) {
    World *world;

    world = dds3ActiveWorld;
    if (world != NULL) {
        effObjNodeDestroy(world);
        dds3ActiveWorld = NULL;
    }
}

/* Set the primary selection without releasing its old pointer; no world is a no-op. */
void dds3SetWorldObject(void *primaryObject) {
    if (dds3ActiveWorld != NULL) {
        dds3ActiveWorld->info->primaryObject = primaryObject;
    }
}

/* Return the selected primary object, or NULL when no world is active. */
void *dds3GetWorldObject(void) {
    World *world;

    world = dds3ActiveWorld;
    if (world == NULL) {
        return NULL;
    }
    return world->info->primaryObject;
}

/* Set the secondary selection without releasing its old pointer; no world is a no-op. */
void dds3SetWorldSecondaryObject(void *secondaryObject) {
    if (dds3ActiveWorld != NULL) {
        dds3ActiveWorld->info->secondaryObject = secondaryObject;
    }
}

/* Return the selected secondary object, or NULL when no world is active. */
void *dds3GetWorldSecondaryObject(void) {
    World *world;

    world = dds3ActiveWorld;
    if (world == NULL) {
        return NULL;
    }
    return world->info->secondaryObject;
}

/* Create a kind-1 world node and append it at the tail.
 * Missing active world or failed creation returns NULL without changing the list. */
EffWorldNode *dds3AppendWorldNode(void) {
    WorldInfo *worldInfo;
    EffWorldNode *worldNode;

    if (dds3ActiveWorld == NULL) {
        return NULL;
    }
    worldInfo = dds3ActiveWorld->info;
    worldNode = dds3CreateWorldNodeForKind(DDS3_WORLD_NODE_KIND);
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
void dds3DestroyWorldNode(EffWorldNode *worldNode) {
    WorldInfo *worldInfo;

    if (worldNode == NULL) {
        return;
    }
    if (dds3ActiveWorld == NULL) {
        return;
    }
    worldInfo = dds3ActiveWorld->info;
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
    effObjNodeDestroy(worldNode);
}

/* Append a separate index node, then request initialCount entries from the shared
 * value pool. Only the upper-bound check occurs here: negative counts can still
 * produce a linked empty node when the growth helper rejects the request. */
void *dds3AppendWorldIndexNode(s32 initialCount) {
    WorldInfo *worldInfo;
    NodeB *indexNode;

    if (dds3ActiveWorld == NULL) {
        return NULL;
    }
    worldInfo = dds3ActiveWorld->info;
    if (worldInfo->unk1E < initialCount) {
        return NULL;
    }
    indexNode = sdfAllocSizeClassBlock(DDS3_WORLD_INDEX_NODE_BYTES);
    if (indexNode == NULL) {
        return NULL;
    }
    indexNode->previous = NULL;
    indexNode->next = NULL;
    indexNode->unk0 = DDS3_WORLD_INVALID_ENTRY_INDEX;
    indexNode->unk2 = DDS3_WORLD_INVALID_ENTRY_INDEX;
    indexNode->unk4 = DDS3_WORLD_INVALID_ENTRY_INDEX;
    indexNode->unk6 = 0;
    if (worldInfo->lastIndex == NULL) {
        worldInfo->firstIndex = indexNode;
        worldInfo->lastIndex = indexNode;
    } else {
        worldInfo->lastIndex->next = indexNode;
        indexNode->previous = worldInfo->lastIndex;
        worldInfo->lastIndex = indexNode;
    }
    dds3GrowWorldValueChain(indexNode, initialCount);
    return indexNode;
}

/* Return the node's value entries to the shared pool, unlink the index node,
 * then release its block. NULL node or missing active world is a no-op. */
void dds3DestroyWorldIndexNode(NodeB *indexNode) {
    WorldInfo *worldInfo;

    if (indexNode == NULL) {
        return;
    }
    if (dds3ActiveWorld == NULL) {
        return;
    }
    worldInfo = dds3ActiveWorld->info;
    func_00110120(indexNode);
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
    sdfReleaseChipBlock(indexNode);
}

INCLUDE_ASM(const s32, "basic/dds3WorldBasic", func_00110018);

INCLUDE_ASM(const s32, "basic/dds3WorldBasic", func_00110120);

/* Reset the cursor and remove the first matching value, or all matches when
 * processAllMatches is nonzero. Return whether any match was processed.
 * Retain the existing count prototype and three-argument call convention. */
s32 dds3ProcessMatchingWorldNodes(void *indexNode, void *targetWord, s32 processAllMatches) {
    s32 processedMatch;

    processedMatch = 0;
    if (dds3GetWorldValueCount(indexNode, targetWord, processAllMatches) != NULL) {
        dds3ResetObjectValueCursor(indexNode);
        do {
            if (dds3SeekWorldNode(indexNode, targetWord) != 1) {
                break;
            }
            func_00110018(indexNode);
            processedMatch = 1;
        } while (processAllMatches != 0);
    }
    return processedMatch;
}

/* Search from the current cursor without resetting it; leave it on a match.
 * A NULL payload terminates before comparison, even if later entries remain,
 * so a NULL target never matches. Return 1 for a match, otherwise 0. */
s32 dds3SeekWorldNode(void *indexNode, void *targetWord) {
    void *candidateWord;

    do {
        candidateWord = dds3ReadIndexedWorldObjectWord(indexNode);
        if (candidateWord == NULL) {
            return 0;
        }
        if (targetWord == candidateWord) {
            return 1;
        }
    } while (dds3AdvanceObjectValueCursor(indexNode) != 0);
    return 0;
}
