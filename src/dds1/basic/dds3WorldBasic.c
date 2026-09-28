#include "common.h"

typedef struct NodeA {
    u8 pad[0x20];
    struct NodeA *next;
    struct NodeA *previous;
} NodeA;

typedef struct NodeB {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    struct NodeB *next;
    struct NodeB *previous;
} NodeB;

typedef struct {
    s32 unk0;
    s16 unk4;
    s16 unk6;
} Entry;

typedef struct {
    NodeA *firstNode;
    NodeA *lastNode;
    void *primaryObject;
    void *secondaryObject;
    u8 pad10[4];
    Entry *unk14;
    u8 pad18[2];
    u16 unk1A;
    s16 unk1C;
    u16 unk1E;
    NodeB *firstIndex;
    NodeB *lastIndex;
} WorldInfo;

typedef struct {
    u8 pad[0x18];
    WorldInfo *info;
} World;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    u16 unk6;
} IndexObj;

extern World *D_003BA9BC;

void func_002CFF98(void *arg);
void effObjNodeDestroy(void *arg);
void *func_0010F418(s32 arg);
void *func_002CFEB8(s32 arg);
void func_001102C8(void *arg, s32 arg1);
void func_00110120(IndexObj *arg);
void func_00110018(IndexObj *arg);
s32 func_00110270(void *arg0, void *arg1);
void *func_00110400(void *arg0, void *arg1, s32 arg2);
void func_00110490(void *arg);
void *func_00110458(void *arg);
s32 func_001104B0(void *arg);

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

void *func_0010FEC8(s32 index) {
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

void func_0010FF80(NodeB *node) {
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
    func_002CFF98(node);
}

INCLUDE_ASM(const s32, "basic/dds3WorldBasic", func_00110018);

INCLUDE_ASM(const s32, "basic/dds3WorldBasic", func_00110120);

s32 func_001101E0(void *arg0, void *arg1, s32 arg2) {
    s32 ret;

    ret = 0;
    if (func_00110400(arg0, arg1, arg2) != NULL) {
        func_00110490(arg0);
        do {
            if (func_00110270(arg0, arg1) != 1) {
                break;
            }
            func_00110018(arg0);
            ret = 1;
        } while (arg2 != 0);
    }
    return ret;
}

s32 func_00110270(void *arg0, void *arg1) {
    void *res;

    do {
        res = func_00110458(arg0);
        if (res == NULL) {
            return 0;
        }
        if (arg1 == res) {
            return 1;
        }
    } while (func_001104B0(arg0) != 0);
    return 0;
}
