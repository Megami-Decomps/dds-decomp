#include "common.h"
#include "sdf_chip.h"

#include "kwln.h"
#include "kwln_task_flags.h"
#include "kwln_task_state.h"
#include "kwln_task_lifecycle.h"



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

extern void kwlnTaskRunScheduledUpdates(void);



extern void kwlnUnlinkListNode(KwlnTask* task);



void kwlnTaskActivate(KwlnTask* task)
{
    kwlnTaskRemoveFromStateQueue(task);
    task->flags = (task->flags & ~KWLN_TASK_STATE_MASK) | KWLN_TASK_ACTIVE;
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
        if (node->startDelayTicks > 0) {
            node->startDelayTicks--;
        }
        curr = node;
        node = node->listNext;
        if (curr->startDelayTicks == 0) {
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
extern KwlnTask *kwlnExecutingTask;

s32 kwlnTaskStep(KwlnTask *task) {
    s32 port;
    s32 i;
    s32 nextUpdate;

    if (task->flags & 0x20) {
        return 1;
    }
    kwlnExecutingTask = task;

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
        if (nextUpdate == -1 && (task->flags & KWLN_TASK_STATE_MASK) == KWLN_TASK_ACTIVE) {
            kwlnTaskRequestDestroy(task);
            kwlnExecutingTask = 0;
            return 0;
        }
    }
    task->timer++;
    kwlnExecutingTask = 0;
    return 1;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", kwlnTaskRunScheduledUpdates);

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
    if (state >= KWLN_TASK_DESTROY_PENDING) {
        return;
    }
    if (state == KWLN_TASK_DETACHED) {
        return;
    }
    kwlnTaskRemoveFromStateQueue(task);
    task->flags = (task->flags & ~KWLN_TASK_STATE_MASK) | KWLN_TASK_DESTROY_PENDING;
    kwlnTaskInsertIntoOrderedStateQueue(task);
    if (task->destroyDelayTicks == 0) {
        kwlnTaskFinalizeDestroy(task);
    }
}

void kwlnTaskAdvanceDestroyDelays(void)
{
    KwlnTask* node;
    KwlnTask* curr;

    node = kwlnDelayedDestroyTaskHead;
    while (node != 0) {
        if (node->destroyDelayTicks > 0) {
            node->destroyDelayTicks--;
        }
        curr = node;
        node = node->listNext;
        if (curr->destroyDelayTicks == 0) {
            kwlnTaskFinalizeDestroy(curr);
        }
    }
}

void kwlnTaskUpdateFlagsRecursive(s32 setFlags, KwlnTask* task, u32 flags)
{
    KwlnTask* child;

    if (setFlags != 0) {
        task->flags |= flags & KWLN_TASK_MUTABLE_FLAGS_MASK;
    } else {
        task->flags &= ~(flags & KWLN_TASK_MUTABLE_FLAGS_MASK);
    }
    child = task->childList;
    while (child != 0) {
        kwlnTaskUpdateFlagsRecursive(setFlags, child, flags);
        child = child->next;
    }
}

void kwlnTaskUpdateFlagsScoped(s32 setFlags, KwlnTask *task, u32 flags, s32 scope)
{
    KwlnTask *node = NULL;
    s32 state;

    switch (scope) {
    case KWLN_TASK_FLAG_SCOPE_TARGET:
        if (setFlags != 0) {
            task->flags |= flags & KWLN_TASK_MUTABLE_FLAGS_MASK;
        } else {
            task->flags &= ~(flags & KWLN_TASK_MUTABLE_FLAGS_MASK);
        }
        return;
    case KWLN_TASK_FLAG_SCOPE_OTHER_QUEUED_TASKS:
    case KWLN_TASK_FLAG_SCOPE_ALL_QUEUED_TASKS:
        for (state = 0; state < 3; state++) {
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
            if (node != NULL) {
                u32 setMask = flags & KWLN_TASK_MUTABLE_FLAGS_MASK;

                while (node != NULL) {
                    if ((node != task && scope == KWLN_TASK_FLAG_SCOPE_OTHER_QUEUED_TASKS) ||
                        scope == KWLN_TASK_FLAG_SCOPE_ALL_QUEUED_TASKS) {
                        if (setFlags != 0) {
                            node->flags |= setMask;
                        } else {
                            node->flags &= ~setMask;
                        }
                    }
                    node = node->listNext;
                }
            }
        }
        return;
    case KWLN_TASK_FLAG_SCOPE_SUBTREE:
        kwlnTaskUpdateFlagsRecursive(setFlags, task, flags);
        break;
    }
}

void* kwlnTaskGetStateList(u32 state)
{
    switch (state & KWLN_TASK_STATE_MASK) {
    case KWLN_TASK_DELAYED_START:
        return kwlnDelayedStartTaskCount;
    case KWLN_TASK_ACTIVE:
        return kwlnActiveTaskCount;
    case KWLN_TASK_DESTROY_PENDING:
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
    kwlnTaskRunScheduledUpdates();
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
    task->flags = KWLN_TASK_DELAYED_START;
    task->startDelayTicks = startDelay;
    task->destroyDelayTicks = destroyDelay;
    task->update = update;
    task->destroy = destroy;
    task->userValue = userValue;
    task->name[0x17] = 0;
    task->unk24 = 0;
    task->timer = 0;
    task->listNext = NULL;
    task->listPrev = NULL;
    task->parent = NULL;
    task->childList = NULL;
    task->next = NULL;
    kwlnTaskInsertIntoOrderedStateQueue(task);
    if (task->startDelayTicks == 0) {
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
    if ((task->flags & KWLN_TASK_STATE_MASK) != KWLN_TASK_ACTIVE) {
        return;
    }
    kwlnTaskRemoveFromStateQueue(task);
    task->flags = (task->flags & ~KWLN_TASK_STATE_MASK) | KWLN_TASK_DESTROY_PENDING;
    kwlnTaskInsertIntoOrderedStateQueue(task);
}

void kwlnTaskSetDestroyDelay(KwlnTask* task, s32 delayTicks)
{
    u32 state;

    state = task->flags & KWLN_TASK_STATE_MASK;
    if (state == KWLN_TASK_DETACHED) {
        return;
    }
    if (state < KWLN_TASK_STATE_LIMIT) {
        task->destroyDelayTicks = delayTicks;
    }
}

s32 kwlnTaskGetRegisteredState(KwlnTask* task)
{
    u32 state;

    if (kwlnTaskIsRegistered(task) == 0) {
        return KWLN_TASK_DETACHED;
    }
    state = task->flags & KWLN_TASK_STATE_MASK;
    return (state < KWLN_TASK_STATE_LIMIT) ? state : KWLN_TASK_DETACHED;
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
    task->userValue = value;
}

u32 kwlnTaskGetUserValue(KwlnTask* task)
{
    return task->userValue;
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

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", kwlnExecutingTask);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_00435BF8);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_00435C00);

