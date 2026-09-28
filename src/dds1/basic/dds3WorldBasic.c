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

void func_0010FD30(void) {
    World *world;

    world = D_003BA9BC;
    if (world != NULL) {
        effObjNodeDestroy(world);
        D_003BA9BC = NULL;
    }
}

void func_0010FD60(void *arg) {
    if (D_003BA9BC != NULL) {
        D_003BA9BC->unk18->unk8 = arg;
    }
}

void *func_0010FD80(void) {
    World *world;

    world = D_003BA9BC;
    if (world == NULL) {
        return NULL;
    }
    return world->unk18->unk8;
}

void func_0010FDA0(void *arg) {
    if (D_003BA9BC != NULL) {
        D_003BA9BC->unk18->unkC = arg;
    }
}

void *func_0010FDC0(void) {
    World *world;

    world = D_003BA9BC;
    if (world == NULL) {
        return NULL;
    }
    return world->unk18->unkC;
}

void *func_0010FDE0(void) {
    WorldInfo *info;
    NodeA *node;

    if (D_003BA9BC == NULL) {
        return NULL;
    }
    info = D_003BA9BC->unk18;
    node = func_0010F418(1);
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

void func_0010FE48(NodeA *arg) {
    WorldInfo *info;

    if (arg == NULL) {
        return;
    }
    if (D_003BA9BC == NULL) {
        return;
    }
    info = D_003BA9BC->unk18;
    if (info->unk0 == arg) {
        info->unk0 = arg->unk20;
    }
    if (info->unk4 == arg) {
        info->unk4 = arg->unk24;
    }
    if (info->unk8 == arg) {
        info->unk8 = NULL;
    }
    if (info->unkC == arg) {
        info->unkC = NULL;
    }
    effObjNodeDestroy(arg);
}

void *func_0010FEC8(s32 arg) {
    WorldInfo *info;
    NodeB *node;

    if (D_003BA9BC == NULL) {
        return NULL;
    }
    info = D_003BA9BC->unk18;
    if (info->unk1E < arg) {
        return NULL;
    }
    node = func_002CFEB8(0x10);
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
    func_001102C8(node, arg);
    return node;
}

void func_0010FF80(NodeB *arg) {
    WorldInfo *info;

    if (arg == NULL) {
        return;
    }
    if (D_003BA9BC == NULL) {
        return;
    }
    info = D_003BA9BC->unk18;
    func_00110120(arg);
    if (arg->unkC == NULL) {
        info->unk20 = arg->unk8;
    } else {
        arg->unkC->unk8 = arg->unk8;
    }
    if (arg->unk8 == NULL) {
        info->unk24 = arg->unkC;
    } else {
        arg->unk8->unkC = arg->unkC;
    }
    func_002CFF98(arg);
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
