#include "common.h"
#include "kwln.h"

/* Low four bits encode the scheduler list/state; upper bits are independent flags. */
#define KWLN_TASK_STATE_MASK 0xF

extern void kwlnTaskRemoveFromStateQueue(KwlnTask* task);

extern void kwlnTaskInsertIntoOrderedStateQueue(KwlnTask* task);

extern void kwlnTaskActivate(KwlnTask* task);

extern void kwlnTaskAdvanceStartDelays(void);

extern s32 func_00100B40(KwlnTask* task);

extern void func_00100D40(void);

extern void kwlnTaskFinalizeDestroy(KwlnTask* task);

extern void kwlnTaskRequestDestroy(KwlnTask* task);

extern void kwlnTaskAdvanceDestroyDelays(void);

extern void func_00101368(KwlnTask* task, s32 arg1);

extern KwlnTask* kwlnTaskGetTaskByName(const char* name);

extern s32 kwlnTaskDestroyWithHierarchy(KwlnTask* task, s32 delayTicks);

extern s32 kwlnTaskIsRegistered(KwlnTask* target);

extern void kwlnUnlinkListNode(KwlnTask* task);

extern void* func_002CFEB8(s32 arg0);

extern void sdfReleaseChipBlock(void* ptr);

extern void func_003003F0();

extern KwlnTask* kwlnDelayedStartTaskHead;

extern void* kwlnDelayedStartTaskCount;

extern KwlnTask* kwlnDelayedDestroyTaskHead;

extern void* kwlnDelayedDestroyTaskCount;

extern KwlnTask* kwlnActiveTaskHead;

extern void* kwlnActiveTaskCount;

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

void kwlnTaskActivate(KwlnTask* task)
{
    kwlnTaskRemoveFromStateQueue(task);
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

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100B40);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100D40);

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
    kwlnTaskRemoveFromStateQueue(task);
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
    kwlnTaskRemoveFromStateQueue(task);
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

void kwlnTaskUpdateFlagsRecursive(s32 setFlags, KwlnTask* task, u32 flags)
{
    KwlnTask* child;

    if (setFlags != 0) {
        task->flags |= flags & 0x0FFFFFF0;
    } else {
        task->flags &= ~(flags & 0x0FFFFFF0);
    }
    child = task->childList;
    while (child != 0) {
        kwlnTaskUpdateFlagsRecursive(setFlags, child, flags);
        child = child->next;
    }
}

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101060);

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

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101218);

void kwlnTraverseDiagnosticTaskQueue(KwlnTask* task, void* unused)
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

    kwlnTraverseDiagnosticTaskQueue(kwlnDelayedStartTaskHead, D_0039DE88);
    tmp = D_0039DEB8;
    func_003003F0(tmp, kwlnDelayedStartTaskCount);
    kwlnTraverseDiagnosticTaskQueue(kwlnActiveTaskHead, D_0039DEC8);
    func_003003F0(tmp, kwlnActiveTaskCount);
    kwlnTraverseDiagnosticTaskQueue(kwlnDelayedDestroyTaskHead, D_0039DEF8);
    func_003003F0(tmp, kwlnDelayedDestroyTaskCount);
    func_003003F0(D_003BA828);
}

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101368);

/* Blank the task-name scratch buffer, then run func_00101368 on every parentless task of the three scheduler lists. */
void kwlnVisitTaskForestRoots(void)
{
    KwlnTask* task;
    s32 i;

    for (i = 0; i < 0x40; i++) {
        D_003BDC48[i] = 0x20;
    }
    for (task = kwlnDelayedStartTaskHead; task != NULL; task = task->listNext) {
        if (task->parent == NULL) {
            D_003BDC48[0] = 0;
            func_00101368(task, 0);
        }
    }
    for (task = kwlnActiveTaskHead; task != NULL; task = task->listNext) {
        if (task->parent == NULL) {
            D_003BDC48[0] = 0;
            func_00101368(task, 0);
        }
    }
    for (task = kwlnDelayedDestroyTaskHead; task != NULL; task = task->listNext) {
        if (task->parent == NULL) {
            D_003BDC48[0] = 0;
            func_00101368(task, 0);
        }
    }
}

s32 kwlnTaskTickScheduler(void)
{
    kwlnTaskAdvanceStartDelays();
    func_00100D40();
    kwlnTaskAdvanceDestroyDelays();
    return 1;
}

KwlnTask *kwlnTaskCreate(const char *name, u32 priority, s32 startDelay, s32 destroyDelay,
                         TaskUpdate update, TaskDestroy destroy, u32 userValue) {
    KwlnTask *task = func_002CFEB8(sizeof(KwlnTask));
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

    task = kwlnTaskGetTaskByName(name);
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
    kwlnTaskRemoveFromStateQueue(task);
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

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", kwlnTaskGetTaskByName);

KwlnTask* kwlnTaskFindByPriority(u32 prio)
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

u32 func_00101A58(KwlnTask* task)
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

void func_00101A78(void) {
}

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101A80);

INCLUDE_RODATA(const s32, "kernel/dds3KernelCore", D_0039DE88);

INCLUDE_RODATA(const s32, "kernel/dds3KernelCore", D_0039DEB8);

INCLUDE_RODATA(const s32, "kernel/dds3KernelCore", D_0039DEC8);

INCLUDE_RODATA(const s32, "kernel/dds3KernelCore", D_0039DEF8);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_003BA824);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_003BA828);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_003BA830);

