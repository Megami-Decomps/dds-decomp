#include "common.h"
#include "dds3obj.h"

extern World *D_00435D8C;

void effObjNodeDestroy(void *arg);

void *func_0010F640(s32 arg);

void *func_00328D68(s32 arg);

void func_001104F0(void *arg, s32 arg1);

void func_00110240(IndexObj *arg);

s32 func_00110498(void *arg0, void *arg1);

void *func_00110628(void *arg0, void *arg1, s32 arg2);

void func_001106B8(void *arg);

void *func_00110680(void *arg);

s32 func_001106D8(void *arg);

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

INCLUDE_ASM(const s32, "basic/dds3WorldBasic", dds3DestroyWorldNode);

void *func_001100F0(s32 arg) {
    WorldInfo *info;
    NodeB *node;

    if (D_00435D8C == NULL) {
        return NULL;
    }
    info = D_00435D8C->info;
    if (info->unk1E < arg) {
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
    func_001104F0(node, arg);
    return node;
}

INCLUDE_ASM(const s32, "basic/dds3WorldBasic", func_001101A8);

INCLUDE_ASM(const s32, "basic/dds3WorldBasic", func_00110240);

INCLUDE_ASM(const s32, "basic/dds3WorldBasic", func_00110348);

s32 func_00110408(void *arg0, void *arg1, s32 arg2) {
    s32 ret;

    ret = 0;
    if (func_00110628(arg0, arg1, arg2) != NULL) {
        func_001106B8(arg0);
        do {
            if (func_00110498(arg0, arg1) != 1) {
                break;
            }
            func_00110240(arg0);
            ret = 1;
        } while (arg2 != 0);
    }
    return ret;
}

s32 func_00110498(void *arg0, void *arg1) {
    void *res;

    do {
        res = func_00110680(arg0);
        if (res == NULL) {
            return 0;
        }
        if (arg1 == res) {
            return 1;
        }
    } while (func_001106D8(arg0) != 0);
    return 0;
}
