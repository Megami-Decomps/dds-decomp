#include "common.h"
#include "dds3obj.h"

extern World *D_003BA9BC;

void sdfReleaseChipBlock(void *arg);
void effObjNodeDestroy(void *arg);
void *func_0010F418(s32 arg);
void *func_002CFEB8(s32 arg);
void func_001102C8(void *arg, s32 arg1);
void func_00110120(IndexObj *arg);
void func_00110018(IndexObj *arg);
s32 dds3SeekWorldNode(void *arg0, void *arg1);
void *func_00110400(void *arg0, void *arg1, s32 arg2);
void dds3ResetObjectValueCursor(void *arg);
void *dds3ReadIndexedWorldObjectWord(void *arg);
s32 dds3AdvanceObjectValueCursor(void *arg);

void dds3DestroyWorld(void) {
    World *world;

    world = D_003BA9BC;
    if (world != NULL) {
        effObjNodeDestroy(world);
        D_003BA9BC = NULL;
    }
}

void dds3SetWorldObject(void *object) {
    if (D_003BA9BC != NULL) {
        D_003BA9BC->info->primaryObject = object;
    }
}

void *dds3GetWorldObject(void) {
    World *world;

    world = D_003BA9BC;
    if (world == NULL) {
        return NULL;
    }
    return world->info->primaryObject;
}

void dds3SetWorldSecondaryObject(void *object) {
    if (D_003BA9BC != NULL) {
        D_003BA9BC->info->secondaryObject = object;
    }
}

void *dds3GetWorldSecondaryObject(void) {
    World *world;

    world = D_003BA9BC;
    if (world == NULL) {
        return NULL;
    }
    return world->info->secondaryObject;
}

void *dds3AppendWorldNode(void) {
    WorldInfo *info;
    NodeA *node;

    if (D_003BA9BC == NULL) {
        return NULL;
    }
    info = D_003BA9BC->info;
    node = func_0010F418(1);
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
    if (D_003BA9BC == NULL) {
        return;
    }
    info = D_003BA9BC->info;
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

void *dds3AppendWorldIndexNode(s32 index) {
    WorldInfo *info;
    NodeB *node;

    if (D_003BA9BC == NULL) {
        return NULL;
    }
    info = D_003BA9BC->info;
    if (info->unk1E < index) {
        return NULL;
    }
    node = func_002CFEB8(0x10);
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
    func_001102C8(node, index);
    return node;
}

void dds3DestroyWorldIndexNode(NodeB *node) {
    WorldInfo *info;

    if (node == NULL) {
        return;
    }
    if (D_003BA9BC == NULL) {
        return;
    }
    info = D_003BA9BC->info;
    func_00110120(node);
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

INCLUDE_ASM(const s32, "basic/dds3WorldBasic", func_00110018);

INCLUDE_ASM(const s32, "basic/dds3WorldBasic", func_00110120);

s32 dds3ProcessMatchingWorldNodes(void *iterator, void *target, s32 repeat) {
    s32 found;

    found = 0;
    if (func_00110400(iterator, target, repeat) != NULL) {
        dds3ResetObjectValueCursor(iterator);
        do {
            if (dds3SeekWorldNode(iterator, target) != 1) {
                break;
            }
            func_00110018(iterator);
            found = 1;
        } while (repeat != 0);
    }
    return found;
}

s32 dds3SeekWorldNode(void *iterator, void *target) {
    void *candidate;

    do {
        candidate = dds3ReadIndexedWorldObjectWord(iterator);
        if (candidate == NULL) {
            return 0;
        }
        if (target == candidate) {
            return 1;
        }
    } while (dds3AdvanceObjectValueCursor(iterator) != 0);
    return 0;
}
