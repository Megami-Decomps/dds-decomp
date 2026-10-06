#include "common.h"

#include "kwln.h"

extern void *sdfAllocSizeClassBlock(s32);

#define KWLN_TASK_STATE_MASK 0xF

extern KwlnTask* kwlnActiveTaskHead;

extern void kwlnTaskRemoveFromStateQueue(KwlnTask* task);

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

extern KwlnTask* kwlnTaskGetTaskByName(const char* name);

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

extern u8 D_0037F4D0[2][2][16];
extern u8 D_0037F510[2][2][16];
extern u8 D_00435C30[2][2][2];
extern u8 D_00435C38[2][2];
extern u8 D_00435C40[2][2][2];
extern u8 D_00435C48[2][2];
extern s32 D_00435BF4;

s32 func_00100A28(KwlnTask *task) {
    s32 port;
    s32 i;
    s32 nextUpdate;

    if (task->flags & 0x20) {
        return 1;
    }
    D_00435BF4 = (s32)task;

    if (task->flags & 0x10) {
        for (port = 0; port < 2; port++) {
            for (i = 0; i < 16; i++) {
                D_0037F510[0][port][i] = 0;
                D_0037F510[1][port][i] = 0;
            }
            for (i = 0; i < 2; i++) {
                D_00435C40[port][i][0] = 0;
                D_00435C48[port][i] = 0;
                D_00435C40[port][i][1] = 0;
            }
        }
    } else {
        for (port = 0; port < 2; port++) {
            for (i = 0; i < 16; i++) {
                D_0037F510[0][port][i] = D_0037F4D0[0][port][i];
                D_0037F510[1][port][i] = D_0037F4D0[1][port][i];
            }
            for (i = 0; i < 2; i++) {
                D_00435C40[port][i][0] = D_00435C30[port][i][0];
                D_00435C40[port][i][1] = D_00435C30[port][i][1];
                D_00435C48[port][i] = D_00435C38[port][i];
            }
        }
    }

    if (task->update != NULL && task->update != (TaskUpdate)-1) {
        nextUpdate = task->update(task);
        if (nextUpdate != 0) {
            task->update = (TaskUpdate)nextUpdate;
        }
        if (nextUpdate == -1 && (task->flags & KWLN_TASK_STATE_MASK) == 2) {
            kwlnTaskRequestDestroy(task);
            D_00435BF4 = 0;
            return 0;
        }
    }
    task->timer++;
    D_00435BF4 = 0;
    return 1;
}

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

    kwlnTraverseDiagnosticTaskQueue(kwlnDelayedStartTaskHead, D_00411008);
    tmp = D_00411038;
    func_0035B6E0(tmp, kwlnDelayedStartTaskCount);
    kwlnTraverseDiagnosticTaskQueue(kwlnActiveTaskHead, D_00411048);
    func_0035B6E0(tmp, kwlnActiveTaskCount);
    kwlnTraverseDiagnosticTaskQueue(kwlnDelayedDestroyTaskHead, D_00411078);
    func_0035B6E0(tmp, kwlnDelayedDestroyTaskCount);
    func_0035B6E0(D_00435BF8);
}

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101250);

extern u8 D_004393C8[];
extern void func_00101250(KwlnTask* task, s32 arg1);



/* Blank the task-name scratch buffer, then run func_00101250 on every parentless task of the three scheduler lists. */
void kwlnVisitTaskForestRoots(void)
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
    KwlnTask *task = sdfAllocSizeClassBlock(sizeof(KwlnTask));
    s32 i = 0;

    if (task == NULL) {
        return NULL;
    }
    task->nameSum = 0;
    while ((task->name[i] = name[i]) != 0 && i < 0x18) {
        task->nameSum += name[i];
        i++;
    }
    task->priority = priority;
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

KwlnTask* kwlnTaskGetTaskByName(const char* name)
{
    s32 nameLength;
    s32 nameSum;
    s32 queueIndex;
    s32 i;
    KwlnTask* task;

    task = NULL;
    nameSum = 0;
    nameLength = 0;
    while (name[nameLength] != '\0') {
        nameSum += name[nameLength];
        nameLength++;
    }

    queueIndex = 0;
    for (; queueIndex < 3; queueIndex++) {
        switch (queueIndex) {
        case 0:
            task = kwlnDelayedStartTaskHead;
            break;
        case 1:
            task = kwlnActiveTaskHead;
            break;
        case 2:
            task = kwlnDelayedDestroyTaskHead;
            break;
        }

        while (task != NULL) {
            if (task->nameSum == nameSum) {
                for (i = nameLength; ; i--) {
                    if (name[i] != task->name[i]) {
                        break;
                    }
                    if (i == 0) {
                        return task;
                    }
                }
            }
            task = task->listNext;
        }
    }
    return NULL;
}

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
            if (node->priority == value) {
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

void func_00101968(KwlnTask* parent, KwlnTask* child)
{
    KwlnTask* sibling;

    if (child->parent != NULL) {
        kwlnUnlinkListNode(child);
    }
    if (parent->childList != NULL) {
        sibling = parent->childList;
        while (sibling->next != NULL) {
            sibling = sibling->next;
        }
        sibling->next = child;
    } else {
        parent->childList = child;
    }
    child->parent = parent;
}

INCLUDE_RODATA(const s32, "kernel/dds3KernelCore", D_00411008);

INCLUDE_RODATA(const s32, "kernel/dds3KernelCore", D_00411038);

INCLUDE_RODATA(const s32, "kernel/dds3KernelCore", D_00411048);

INCLUDE_RODATA(const s32, "kernel/dds3KernelCore", D_00411078);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_00435BF4);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_00435BF8);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_00435C00);

