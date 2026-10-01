#include "common.h"

#include "dds3obj.h"

extern World *D_00435D8C;

void effObjNodeDestroy(void *arg);

void *func_0010F640(s32 arg);

void *func_00328D68(s32 arg);

void func_001104F0(void *arg, s32 arg1);

void func_00110240(IndexObj *arg);

s32 dds3SeekWorldNode(void *arg0, void *arg1);

void *func_00110628(void *arg0, void *arg1, s32 arg2);

void dds3ResetObjectValueCursor(void *arg);

void *func_00110680(void *arg);

s32 dds3AdvanceObjectValueCursor(void *arg);

void sdfReleaseChipBlock(void *arg);

void func_00110348(IndexObj *arg);

void dds3DestroyWorld(void) {
    World *world;

    world = D_00435D8C;
    if (world != NULL) {
        effObjNodeDestroy(world);
        D_00435D8C = NULL;
    }
}

void dds3SetWorldObject(void *object) {
    if (D_00435D8C != NULL) {
        D_00435D8C->info->primaryObject = object;
    }
}

void *dds3GetWorldObject(void) {
    World *world;

    world = D_00435D8C;
    if (world == NULL) {
        return NULL;
    }
    return world->info->primaryObject;
}

void dds3SetWorldSecondaryObject(void *object) {
    if (D_00435D8C != NULL) {
        D_00435D8C->info->secondaryObject = object;
    }
}

void *dds3GetWorldSecondaryObject(void) {
    World *world;

    world = D_00435D8C;
    if (world == NULL) {
        return NULL;
    }
    return world->info->secondaryObject;
}

void *dds3AppendWorldNode(void) {
    WorldInfo *info;
    NodeA *node;

    if (D_00435D8C == NULL) {
        return NULL;
    }
    info = D_00435D8C->info;
    node = func_0010F640(1);
    if (node == NULL) {
        return NULL;
    }
    if (info->lastNode == NULL) {
        info->firstNode = node;
        info->lastNode = node;
    } else {
        info->lastNode->next = node;
        node->previous = info->lastNode;
        info->lastNode = node;
    }
    return node;
}

/* Clear world-owned references before destroying a node that may also be selected. */
void dds3DestroyWorldNode(NodeA *node) {
    WorldInfo *info;

    if (node == NULL) {
        return;
    }
    if (D_00435D8C == NULL) {
        return;
    }
    info = D_00435D8C->info;
    if (info->firstNode == node) {
        info->firstNode = node->next;
    }
    if (info->lastNode == node) {
        info->lastNode = node->previous;
    }
    if (info->primaryObject == node) {
        info->primaryObject = NULL;
    }
    if (info->secondaryObject == node) {
        info->secondaryObject = NULL;
    }
    effObjNodeDestroy(node);
}

/* Append a separate index entry to the world's doubly linked index list. */
void *dds3AppendWorldIndexNode(s32 index) {
    WorldInfo *info;
    NodeB *node;

    if (D_00435D8C == NULL) {
        return NULL;
    }
    info = D_00435D8C->info;
    if (info->unk1E < index) {
        return NULL;
    }
    node = func_00328D68(0x10);
    if (node == NULL) {
        return NULL;
    }
    node->previous = NULL;
    node->next = NULL;
    node->unk0 = -1;
    node->unk2 = -1;
    node->unk4 = -1;
    node->unk6 = 0;
    if (info->lastIndex == NULL) {
        info->firstIndex = node;
        info->lastIndex = node;
    } else {
        info->lastIndex->next = node;
        node->previous = info->lastIndex;
        info->lastIndex = node;
    }
    func_001104F0(node, index);
    return node;
}

void dds3DestroyWorldIndexNode(NodeB *node) {
    WorldInfo *info;

    if (node == NULL) {
        return;
    }
    if (D_00435D8C == NULL) {
        return;
    }
    info = D_00435D8C->info;
    func_00110348(node);
    if (node->previous == NULL) {
        info->firstIndex = node->next;
    } else {
        node->previous->next = node->next;
    }
    if (node->next == NULL) {
        info->lastIndex = node->previous;
    } else {
        node->next->previous = node->previous;
    }
    sdfReleaseChipBlock(node);
}

INCLUDE_ASM(const s32, "basic/dds3WorldBasic", func_00110240);

INCLUDE_ASM(const s32, "basic/dds3WorldBasic", func_00110348);

s32 dds3ProcessMatchingWorldNodes(void *iterator, void *target, s32 repeat) {
    s32 found;

    found = 0;
    if (func_00110628(iterator, target, repeat) != NULL) {
        dds3ResetObjectValueCursor(iterator);
        do {
            if (dds3SeekWorldNode(iterator, target) != 1) {
                break;
            }
            func_00110240(iterator);
            found = 1;
        } while (repeat != 0);
    }
    return found;
}

s32 dds3SeekWorldNode(void *iterator, void *target) {
    void *candidate;

    do {
        candidate = func_00110680(iterator);
        if (candidate == NULL) {
            return 0;
        }
        if (target == candidate) {
            return 1;
        }
    } while (dds3AdvanceObjectValueCursor(iterator) != 0);
    return 0;
}
