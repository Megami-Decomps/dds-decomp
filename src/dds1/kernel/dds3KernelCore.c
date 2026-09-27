#include "common.h"

typedef struct KwlnTask KwlnTask;
typedef s32 (*TaskUpdate)(KwlnTask* task);
typedef void (*TaskDestroy)(KwlnTask* task);
struct KwlnTask {
    u8 unk00[0x18];
    s32 nameSum;
    u32 flags;
    u32 unk20;
    u32 unk24;
    u32 timer;
    s16 unk2C;
    s16 unk2E;
    TaskUpdate unk30;
    TaskDestroy unk34;
    u32 unk38;
    KwlnTask* listNext;
    KwlnTask* listPrev;
    KwlnTask* parent;
    KwlnTask* childList;
    KwlnTask* next;
};

extern void func_001006E0(KwlnTask* task);
extern void func_00100858(KwlnTask* task);
extern void func_00100A98(KwlnTask* task);
extern void func_00100AE0(void);
extern s32 func_00100B40(KwlnTask* task);
extern void func_00100D40(void);
extern void func_00100E68(KwlnTask* task);
extern void func_00100EF0(KwlnTask* task);
extern void func_00100F68(void);
extern void func_00101368(KwlnTask* task, s32 arg1);
extern KwlnTask* kwlnTaskGetTaskByName(const char* name);
extern s32 kwlnTaskDestroyWithHierarchy(KwlnTask* task, s32 arg1);
extern s32 func_001019C8(void* target);
extern void func_00101B08(KwlnTask* task);
extern void* func_002CFEB8(s32 arg0);
extern void func_002CFF98(void* ptr);
extern void func_003003F0();

extern KwlnTask* D_003BA800;
extern void* D_003BA808;
extern KwlnTask* D_003BA80C;
extern void* D_003BA814;
extern KwlnTask* D_003BA818;
extern void* D_003BA820;
extern s32 D_003BA824;
extern u8 D_003BA828[];
extern u8 D_003BA830[];
extern u8 D_003BDC48[];
extern u8 D_003244D0[];
extern u8 D_00324510[];
extern u8 D_0039DE88[];
extern u8 D_0039DEB8[];
extern u8 D_0039DEC8[];
extern u8 D_0039DEF8[];

void func_00100A98(KwlnTask* task)
{
    func_001006E0(task);
    task->flags = (task->flags & ~0xF) | 2;
    func_00100858(task);
    task->unk24 = 0;
    task->timer = 0;
}

void func_00100AE0(void)
{
    KwlnTask* node;
    KwlnTask* curr;

    node = D_003BA800;
    while (node != 0) {
        if (node->unk2C > 0) {
            node->unk2C--;
        }
        curr = node;
        node = node->listNext;
        if (curr->unk2C == 0) {
            func_00100A98(curr);
        }
    }
}


INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100B40);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100D40);

void func_00100E68(KwlnTask* task)
{
    KwlnTask* child;
    KwlnTask* next;

    child = task->childList;
    while (child != 0) {
        next = child->next;
        kwlnTaskDestroyWithHierarchy(child, 0);
        child = next;
    }
    func_001006E0(task);
    if (task->unk34 != 0) {
        task->unk34(task);
    }
    task->flags &= ~0xF;
    func_00101B08(task);
    func_002CFF98(task);
}

void func_00100EF0(KwlnTask* task)
{
    u32 state;

    state = task->flags & 0xF;
    if (state >= 3) {
        return;
    }
    if (state == 0) {
        return;
    }
    func_001006E0(task);
    task->flags = (task->flags & ~0xF) | 3;
    func_00100858(task);
    if (task->unk2E == 0) {
        func_00100E68(task);
    }
}


void func_00100F68(void)
{
    KwlnTask* node;
    KwlnTask* curr;

    node = D_003BA80C;
    while (node != 0) {
        if (node->unk2E > 0) {
            node->unk2E--;
        }
        curr = node;
        node = node->listNext;
        if (curr->unk2E == 0) {
            func_00100E68(curr);
        }
    }
}


INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100FC8);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101060);

void* func_001011C0(u32 arg0)
{
    switch (arg0 & 0xF) {
    case 1:
        return D_003BA808;
    case 2:
        return D_003BA820;
    case 3:
        return D_003BA814;
    default:
        return 0;
    }
}


INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101218);

void func_001012B0(KwlnTask* task, void* arg1)
{
    if (task == 0) {
        return;
    }
    do {
        task = task->listNext;
    } while (task != 0);
}


INCLUDE_RODATA(const s32, "kernel/dds3KernelCore", D_0039DE88);

INCLUDE_RODATA(const s32, "kernel/dds3KernelCore", D_0039DEB8);

INCLUDE_RODATA(const s32, "kernel/dds3KernelCore", D_0039DEC8);

INCLUDE_RODATA(const s32, "kernel/dds3KernelCore", D_0039DEF8);

void func_001012E8(void)
{
    u8* tmp;

    func_001012B0(D_003BA800, D_0039DE88);
    tmp = D_0039DEB8;
    func_003003F0(tmp, D_003BA808);
    func_001012B0(D_003BA818, D_0039DEC8);
    func_003003F0(tmp, D_003BA820);
    func_001012B0(D_003BA80C, D_0039DEF8);
    func_003003F0(tmp, D_003BA814);
    func_003003F0(D_003BA828);
}

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101368);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101440);

s32 func_00101540(void)
{
    func_00100AE0();
    func_00100D40();
    func_00100F68();
    return 1;
}


INCLUDE_ASM(const s32, "kernel/dds3KernelCore", kwlnTaskCreate);

s32 kwlnTaskDestroyWithHierarchyByName(const char* name, s32 arg1)
{
    KwlnTask* task;

    task = kwlnTaskGetTaskByName(name);
    if (task == 0) {
        return 0;
    }
    return kwlnTaskDestroyWithHierarchy(task, arg1);
}

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", kwlnTaskDestroyWithHierarchy);

void func_00101790(KwlnTask* task)
{
    if ((task->flags & 0xF) != 2) {
        return;
    }
    func_001006E0(task);
    task->flags = (task->flags & ~0xF) | 3;
    func_00100858(task);
}


void func_001017F8(KwlnTask* task, s32 arg1)
{
    u32 state;

    state = task->flags & 0xF;
    if (state == 0) {
        return;
    }
    if (state < 4) {
        task->unk2E = arg1;
    }
}


s32 func_00101818(KwlnTask* task)
{
    u32 state;

    if (func_001019C8(task) == 0) {
        return 0;
    }
    state = task->flags & 0xF;
    return (state < 4) ? state : 0;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", kwlnTaskGetTaskByName);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101880);

KwlnTask* func_00101938(u32 prio)
{
    s32 idx;
    KwlnTask* node;
    node = 0;
    idx = 0;
    for (; idx < 3; idx++)
    {
        switch (idx)
        {
        case 0:
            node = D_003BA800;
            break;
        case 1:
            node = D_003BA818;
            break;
        case 2:
            node = D_003BA80C;
            break;
        }
        while (node != 0)
        {
            if (node->unk20 == prio)
            {
                return node;
            }
            node = node->listNext;
        }
    }
    return 0;
}


/* Persona 4 func_00452490 @ 00452490 (src/Kernel/sdkTask.c), recompiled unchanged */
s32 func_001019C8(void* target)
{
    s32 idx;
    void* node;
    node = 0;
    idx = 0;
    for (; idx < 3; idx++)
    {
        switch (idx)
        {
        case 0:
            node = D_003BA800;
            break;
        case 1:
            node = D_003BA818;
            break;
        case 2:
            node = D_003BA80C;
            break;
        }
        while (node != 0)
        {
            if (node == target)
            {
                return 1;
            }
            node = *(void**)((u8*)node + 0x3C);
        }
    }
    return 0;
}

u32 func_00101A58(void* task)
{
    return *(u32*)((u8*)task + 0x24);
}

/* Persona 4 kwlnTaskGetTimer @ 00452540 (src/Kernel/sdkTask.c), recompiled unchanged */
u32 kwlnTaskGetTimer(void* task)
{
    return *(u32*)((u8*)task + 0x28);
}

void func_00101A68(void* task, u32 value)
{
    *(u32*)((u8*)task + 0x38) = value;
}

u32 func_00101A70(void* task)
{
    return *(u32*)((u8*)task + 0x38);
}

void func_00101A78(void) {
}

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101A80);
