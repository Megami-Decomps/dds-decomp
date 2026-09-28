#include "common.h"

typedef struct NodeA {
    u8 pad[0x20];
    struct NodeA *unk20;
    struct NodeA *unk24;
} NodeA;

typedef struct NodeB {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    struct NodeB *unk8;
    struct NodeB *unkC;
} NodeB;

typedef struct {
    s32 unk0;
    s16 unk4;
    s16 unk6;
} Entry;

typedef struct {
    NodeA *unk0;
    NodeA *unk4;
    void *unk8;
    void *unkC;
    u8 pad10[4];
    Entry *unk14;
    u8 pad18[2];
    u16 unk1A;
    s16 unk1C;
    u16 unk1E;
    NodeB *unk20;
    NodeB *unk24;
} WorldInfo;

typedef struct {
    u8 pad[0x18];
    WorldInfo *unk18;
} World;

extern World *D_00435D8C;

void effObjNodeDestroy(void *arg);

void *func_0010F640(s32 arg);

void *func_00328D68(s32 arg);

void func_001104F0(void *arg, s32 arg1);

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    u16 unk6;
} IndexObj;

void func_00110240(IndexObj *arg);

s32 func_00110498(void *arg0, void *arg1);

void *func_00110628(void *arg0, void *arg1, s32 arg2);

void func_001106B8(void *arg);

void *func_00110680(void *arg);

s32 func_001106D8(void *arg);

void func_0010FF58(void) {
    World *world;

    world = D_00435D8C;
    if (world != NULL) {
        effObjNodeDestroy(world);
        D_00435D8C = NULL;
    }
}

void func_0010FF88(void *arg) {
    if (D_00435D8C != NULL) {
        D_00435D8C->unk18->unk8 = arg;
    }
}

void *func_0010FFA8(void) {
    World *world;

    world = D_00435D8C;
    if (world == NULL) {
        return NULL;
    }
    return world->unk18->unk8;
}

void func_0010FFC8(void *arg) {
    if (D_00435D8C != NULL) {
        D_00435D8C->unk18->unkC = arg;
    }
}

void *func_0010FFE8(void) {
    World *world;

    world = D_00435D8C;
    if (world == NULL) {
        return NULL;
    }
    return world->unk18->unkC;
}

void *func_00110008(void) {
    WorldInfo *info;
    NodeA *node;

    if (D_00435D8C == NULL) {
        return NULL;
    }
    info = D_00435D8C->unk18;
    node = func_0010F640(1);
    if (node == NULL) {
        return NULL;
    }
    if (info->unk4 == NULL) {
        info->unk0 = node;
        info->unk4 = node;
    } else {
        info->unk4->unk20 = node;
        node->unk24 = info->unk4;
        info->unk4 = node;
    }
    return node;
}

INCLUDE_ASM(const s32, "basic/dds3WorldBasic", func_00110070);

void *func_001100F0(s32 arg) {
    WorldInfo *info;
    NodeB *node;

    if (D_00435D8C == NULL) {
        return NULL;
    }
    info = D_00435D8C->unk18;
    if (info->unk1E < arg) {
        return NULL;
    }
    node = func_00328D68(0x10);
    if (node == NULL) {
        return NULL;
    }
    node->unkC = NULL;
    node->unk8 = NULL;
    node->unk0 = -1;
    node->unk2 = -1;
    node->unk4 = -1;
    node->unk6 = 0;
    if (info->unk24 == NULL) {
        info->unk20 = node;
        info->unk24 = node;
    } else {
        info->unk24->unk8 = node;
        node->unkC = info->unk24;
        info->unk24 = node;
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
