#include "common.h"

#include "kwln.h"

extern void *func_00328D68(s32);

#define KWLN_TASK_STATE_MASK 0xF

extern KwlnTask* kwlnActiveTaskHead;

extern void func_001005C8(KwlnTask* task);

extern void kwlnTaskInsertIntoOrderedStateQueue(KwlnTask* task);

extern void kwlnTaskActivate(KwlnTask* task);

extern void kwlnTaskAdvanceStartDelays(void);

extern KwlnTask* kwlnDelayedStartTaskHead;

extern void kwlnTaskFinalizeDestroy(KwlnTask* task);

extern void kwlnTaskRequestDestroy(KwlnTask* task);

extern void kwlnTaskAdvanceDestroyDelays(void);

extern KwlnTask* kwlnDelayedDestroyTaskHead;

extern void* kwlnDelayedStartTaskCount;

extern void* kwlnDelayedDestroyTaskCount;

extern void* kwlnActiveTaskCount;

extern void func_0035B6E0(const char *fmt, ...);

extern u8 D_00435BF8[];

extern u8 D_00411008[];

extern u8 D_00411038[];

extern u8 D_00411048[];

extern u8 D_00411078[];

extern void func_00100C28(void);

extern s32 kwlnTaskIsRegistered(KwlnTask* target);

extern s32 kwlnTaskDestroyWithHierarchy(KwlnTask* task, s32 delayTicks);

extern void kwlnUnlinkListNode(KwlnTask* task);

extern void sdfReleaseChipBlock(void* ptr);

extern KwlnTask* func_00101740(const char* name);

void kwlnTaskActivate(KwlnTask* task)
{
    func_001005C8(task);
    task->flags = (task->flags & ~KWLN_TASK_STATE_MASK) | 2;
    kwlnTaskInsertIntoOrderedStateQueue(task);
    task->unk24 = 0;
    task->timer = 0;
}

void kwlnTaskAdvanceStartDelays(void)
{
    KwlnTask* node;
    KwlnTask* curr;

    node = kwlnDelayedStartTaskHead;
    while (node != 0) {
        if (node->unk2C > 0) {
            node->unk2C--;
        }
        curr = node;
        node = node->listNext;
        if (curr->unk2C == 0) {
            kwlnTaskActivate(curr);
        }
    }
}

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100A28);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100C28);

void kwlnTaskFinalizeDestroy(KwlnTask* task)
{
    KwlnTask* child;
    KwlnTask* next;

    child = task->childList;
    while (child != 0) {
        next = child->next;
        kwlnTaskDestroyWithHierarchy(child, 0);
        child = next;
    }
    func_001005C8(task);
    if (task->destroy != 0) {
        task->destroy(task);
    }
    task->flags &= ~KWLN_TASK_STATE_MASK;
    kwlnUnlinkListNode(task);
    sdfReleaseChipBlock(task);
}

void kwlnTaskRequestDestroy(KwlnTask* task)
{
    u32 state;

    state = task->flags & KWLN_TASK_STATE_MASK;
    if (state >= 3) {
        return;
    }
    if (state == 0) {
        return;
    }
    func_001005C8(task);
    task->flags = (task->flags & ~KWLN_TASK_STATE_MASK) | 3;
    kwlnTaskInsertIntoOrderedStateQueue(task);
    if (task->unk2E == 0) {
        kwlnTaskFinalizeDestroy(task);
    }
}

void kwlnTaskAdvanceDestroyDelays(void)
{
    KwlnTask* node;
    KwlnTask* curr;

    node = kwlnDelayedDestroyTaskHead;
    while (node != 0) {
        if (node->unk2E > 0) {
            node->unk2E--;
        }
        curr = node;
        node = node->listNext;
        if (curr->unk2E == 0) {
            kwlnTaskFinalizeDestroy(curr);
        }
    }
}

void func_00100EB0(s32 setFlags, KwlnTask* task, u32 flags)
{
    KwlnTask* child;

    if (setFlags != 0) {
        task->flags |= flags & 0x0FFFFFF0;
    } else {
        task->flags &= ~(flags & 0x0FFFFFF0);
    }
    child = task->childList;
    while (child != 0) {
        func_00100EB0(setFlags, child, flags);
        child = child->next;
    }
}

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100F48);

void* kwlnTaskGetStateList(u32 state)
{
    switch (state & KWLN_TASK_STATE_MASK) {
    case 1:
        return kwlnDelayedStartTaskCount;
    case 2:
        return kwlnActiveTaskCount;
    case 3:
        return kwlnDelayedDestroyTaskCount;
    default:
        return 0;
    }
}

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101100);

void func_00101198(KwlnTask* task, void* unused)
{
    if (task == 0) {
        return;
    }
    do {
        task = task->listNext;
    } while (task != 0);
}

void kwlnPrintTaskQueueDiagnostics(void)
{
    u8* tmp;

    func_00101198(kwlnDelayedStartTaskHead, D_00411008);
    tmp = D_00411038;
    func_0035B6E0(tmp, kwlnDelayedStartTaskCount);
    func_00101198(kwlnActiveTaskHead, D_00411048);
    func_0035B6E0(tmp, kwlnActiveTaskCount);
    func_00101198(kwlnDelayedDestroyTaskHead, D_00411078);
    func_0035B6E0(tmp, kwlnDelayedDestroyTaskCount);
    func_0035B6E0(D_00435BF8);
}

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101250);

extern u8 D_004393C8[];
extern void func_00101250(KwlnTask* task, s32 arg1);

/* Blank the task-name scratch buffer, then run func_00101250 on every parentless task of the three scheduler lists. */
void func_00101328(void)
{
    KwlnTask* task;
    s32 i;

    for (i = 0; i < 0x40; i++) {
        D_004393C8[i] = 0x20;
    }
    for (task = kwlnDelayedStartTaskHead; task != NULL; task = task->listNext) {
        if (task->parent == NULL) {
            D_004393C8[0] = 0;
            func_00101250(task, 0);
        }
    }
    for (task = kwlnActiveTaskHead; task != NULL; task = task->listNext) {
        if (task->parent == NULL) {
            D_004393C8[0] = 0;
            func_00101250(task, 0);
        }
    }
    for (task = kwlnDelayedDestroyTaskHead; task != NULL; task = task->listNext) {
        if (task->parent == NULL) {
            D_004393C8[0] = 0;
            func_00101250(task, 0);
        }
    }
}

s32 kwlnTaskTickScheduler(void)
{
    kwlnTaskAdvanceStartDelays();
    func_00100C28();
    kwlnTaskAdvanceDestroyDelays();
    return 1;
}

KwlnTask *kwlnTaskCreate(const char *name, u32 priority, s32 startDelay, s32 destroyDelay,
                         TaskUpdate update, TaskDestroy destroy, u32 userValue) {
    KwlnTask *task = func_00328D68(sizeof(KwlnTask));
    s32 i = 0;

    if (task == NULL) {
        return NULL;
    }
    task->nameSum = 0;
    while ((task->name[i] = name[i]) != 0 && i < 0x18) {
        task->nameSum += name[i];
        i++;
    }
    task->unk20 = priority;
    task->flags = 1;
    task->unk2C = startDelay;
    task->unk2E = destroyDelay;
    task->update = update;
    task->destroy = destroy;
    task->unk38 = userValue;
    task->name[0x17] = 0;
    task->unk24 = 0;
    task->timer = 0;
    task->listNext = NULL;
    task->listPrev = NULL;
    task->parent = NULL;
    task->childList = NULL;
    task->next = NULL;
    kwlnTaskInsertIntoOrderedStateQueue(task);
    if (task->unk2C == 0) {
        kwlnTaskActivate(task);
    }
    return task;
}

s32 kwlnTaskDestroyWithHierarchyByName(const char* name, s32 delayTicks)
{
    KwlnTask* task;

    task = func_00101740(name);
    if (task == 0) {
        return 0;
    }
    return kwlnTaskDestroyWithHierarchy(task, delayTicks);
}

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", kwlnTaskDestroyWithHierarchy);

void kwlnTaskMarkDestroyPending(KwlnTask* task)
{
    if ((task->flags & KWLN_TASK_STATE_MASK) != 2) {
        return;
    }
    func_001005C8(task);
    task->flags = (task->flags & ~KWLN_TASK_STATE_MASK) | 3;
    kwlnTaskInsertIntoOrderedStateQueue(task);
}

void kwlnTaskSetDestroyDelay(KwlnTask* task, s32 delayTicks)
{
    u32 state;

    state = task->flags & KWLN_TASK_STATE_MASK;
    if (state == 0) {
        return;
    }
    if (state < 4) {
        task->unk2E = delayTicks;
    }
}

s32 kwlnTaskGetRegisteredState(KwlnTask* task)
{
    u32 state;

    if (kwlnTaskIsRegistered(task) == 0) {
        return 0;
    }
    state = task->flags & KWLN_TASK_STATE_MASK;
    return (state < 4) ? state : 0;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101740);

KwlnTask* func_00101820(u32 value)
{
    s32 state;
    KwlnTask* node;

    node = 0;
    state = 0;
    for (; state < 3; state++) {
        switch (state) {
        case 0:
            node = kwlnDelayedStartTaskHead;
            break;
        case 1:
            node = kwlnActiveTaskHead;
            break;
        case 2:
            node = kwlnDelayedDestroyTaskHead;
            break;
        }
        while (node != 0) {
            if (node->unk20 == value) {
                return node;
            }
            node = node->listNext;
        }
    }
    return 0;
}

/* Persona 4 func_00452490 @ 00452490 (src/Kernel/sdkTask.c), recompiled unchanged */
s32 kwlnTaskIsRegistered(KwlnTask* target)
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
            node = kwlnDelayedStartTaskHead;
            break;
        case 1:
            node = kwlnActiveTaskHead;
            break;
        case 2:
            node = kwlnDelayedDestroyTaskHead;
            break;
        }
        while (node != 0)
        {
            if (node == target)
            {
                return 1;
            }
            node = node->listNext;
        }
    }
    return 0;
}

u32 func_00101940(KwlnTask* task)
{
    return task->unk24;
}

/* Persona 4 kwlnTaskGetTimer @ 00452540 (src/Kernel/sdkTask.c), recompiled unchanged */
u32 kwlnTaskGetTimer(KwlnTask* task)
{
    return task->timer;
}

void kwlnTaskSetUserValue(KwlnTask* task, u32 value)
{
    task->unk38 = value;
}

u32 kwlnTaskGetUserValue(KwlnTask* task)
{
    return task->unk38;
}

void func_00101960(void) {
}

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101968);

INCLUDE_RODATA(const s32, "kernel/dds3KernelCore", D_00411008);

INCLUDE_RODATA(const s32, "kernel/dds3KernelCore", D_00411038);

INCLUDE_RODATA(const s32, "kernel/dds3KernelCore", D_00411048);

INCLUDE_RODATA(const s32, "kernel/dds3KernelCore", D_00411078);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_00435BF4);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_00435BF8);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_00435C00);

