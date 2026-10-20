#include "common.h"
#include "kwln_task_add_child.h"
#include "sdf_chip.h"

#include "kwln.h"
#include "kwln_task_queue_query.h"
#include "kwln_task_priority_query.h"
#include "kwln_task_create.h"
#include "kwln_task_flags.h"
#include "kwln_task_state.h"
#include "kwln_task_lifecycle.h"
#include "kwln_task_queue_mutation.h"



extern KwlnTask* kwlnActiveTaskHead;

extern void kwlnTaskActivate(KwlnTask* task);

extern void kwlnTaskAdvanceStartDelays(void);

extern KwlnTask* kwlnDelayedStartTaskHead;

extern void kwlnTaskFinalizeDestroy(KwlnTask* task);

extern void kwlnTaskRequestDestroy(KwlnTask* task);

extern void kwlnTaskAdvanceDestroyDelays(void);

extern KwlnTask* kwlnDelayedDestroyTaskHead;

extern s32 kwlnDelayedStartTaskCount;

extern s32 kwlnDelayedDestroyTaskCount;

extern s32 kwlnActiveTaskCount;

#include "sdf_debug_api.h"

extern u8 D_00435BF8[];

extern u8 D_00411008[];

extern u8 D_00411038[];

extern u8 D_00411048[];

extern u8 D_00411078[];

extern void kwlnTaskRunScheduledUpdates(void);



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

extern s8 kwlnPadButtonStates[2][2][16];
extern u8 D_0037F510[2][2][16];
extern u8 D_00435C30[2][2][2];
extern s8 D_00435C38[2][2];
extern u8 D_00435C40[2][2][2];
extern u8 D_00435C48[2][2];
extern KwlnTask *kwlnExecutingTask;

s32 kwlnTaskStep(KwlnTask *task) {
    s32 port;
    s32 inputIndex;
    s32 nextUpdate;

    if (task->flags & 0x20) {
        return 1;
    }
    kwlnExecutingTask = task;

    if (task->flags & 0x10) {
        for (port = 0; port < 2; port++) {
            for (inputIndex = 0; inputIndex < 16; inputIndex++) {
                D_0037F510[0][port][inputIndex] = 0;
                D_0037F510[1][port][inputIndex] = 0;
            }
            for (inputIndex = 0; inputIndex < 2; inputIndex++) {
                D_00435C40[port][inputIndex][0] = 0;
                D_00435C48[port][inputIndex] = 0;
                D_00435C40[port][inputIndex][1] = 0;
            }
        }
    } else {
        for (port = 0; port < 2; port++) {
            for (inputIndex = 0; inputIndex < 16; inputIndex++) {
                D_0037F510[0][port][inputIndex] = kwlnPadButtonStates[0][port][inputIndex];
                D_0037F510[1][port][inputIndex] = kwlnPadButtonStates[1][port][inputIndex];
            }
            for (inputIndex = 0; inputIndex < 2; inputIndex++) {
                D_00435C40[port][inputIndex][0] = D_00435C30[port][inputIndex][0];
                D_00435C40[port][inputIndex][1] = D_00435C30[port][inputIndex][1];
                D_00435C48[port][inputIndex] = D_00435C38[port][inputIndex];
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

void kwlnTaskRunScheduledUpdates(void)
{
    KwlnTask* active = kwlnActiveTaskHead;
    KwlnTask* delayed = kwlnDelayedDestroyTaskHead;
    KwlnTask* current;
    /* The two single-queue drains share a predecessor snapshot; the merge
     * keeps its own. A step may unlink the selected node. SMT3 RunningProcess
     * retains the selected-node role (dds3KernelCore.c:591-717). */
    KwlnTask* drainPrevious;

    for (;;) {
        if (active != 0) {
            if (delayed != 0) {
                KwlnTask* previous;

                if (active->priority <= delayed->priority) {
                    current = active;
                    previous = current->listPrev;
                    if (kwlnTaskStep(current) != 0) {
                        active = current->listNext;
                    } else {
                        active = previous != 0 ? previous->listNext : kwlnActiveTaskHead;
                    }
                } else {
                    current = delayed;
                    previous = current->listPrev;
                    if (kwlnTaskStep(current) != 0) {
                        delayed = current->listNext;
                    } else {
                        delayed = previous != 0 ? previous->listNext : kwlnDelayedDestroyTaskHead;
                    }
                }
            } else {

                do {
                    current = active;
                    drainPrevious = current->listPrev;
                    if (kwlnTaskStep(current) != 0) {
                        active = current->listNext;
                    } else {
                        active = drainPrevious != 0 ? drainPrevious->listNext : kwlnActiveTaskHead;
                    }
                } while (active != 0);
                return;
            }
        } else {
            if (delayed != 0) {

                do {
                    current = delayed;
                    drainPrevious = current->listPrev;
                    if (kwlnTaskStep(current) != 0) {
                        delayed = current->listNext;
                    } else {
                        delayed = drainPrevious != 0 ? drainPrevious->listNext : kwlnDelayedDestroyTaskHead;
                    }
                } while (delayed != 0);
            }
            return;
        }
    }
}

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

s32 kwlnTaskGetStateList(u32 state)
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

KwlnTask *kwlnTaskGetByStateAndIndex(u32 taskState, s32 taskIndex)
{
    KwlnTask *task = kwlnDelayedStartTaskHead;
    s32 listPosition;

    switch (taskState & KWLN_TASK_STATE_MASK) {
    case KWLN_TASK_DELAYED_START:
        break;
    case KWLN_TASK_ACTIVE:
        task = kwlnActiveTaskHead;
        break;
    case KWLN_TASK_DESTROY_PENDING:
        task = kwlnDelayedDestroyTaskHead;
        break;
    case KWLN_TASK_DETACHED:
    default:
        return NULL;
    }
    for (listPosition = 0; listPosition < taskIndex && task != NULL; listPosition++) {
        task = task->listNext;
    }
    return task;
}

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
    u8* queueCountFormat;

    kwlnTraverseDiagnosticTaskQueue(kwlnDelayedStartTaskHead, D_00411008);
    queueCountFormat = D_00411038;
    sdfDebugPrintf(queueCountFormat, kwlnDelayedStartTaskCount);
    kwlnTraverseDiagnosticTaskQueue(kwlnActiveTaskHead, D_00411048);
    sdfDebugPrintf(queueCountFormat, kwlnActiveTaskCount);
    kwlnTraverseDiagnosticTaskQueue(kwlnDelayedDestroyTaskHead, D_00411078);
    sdfDebugPrintf(queueCountFormat, kwlnDelayedDestroyTaskCount);
    sdfDebugPrintf(D_00435BF8);
}

extern u8 kwlnTaskTreePrefix[];
extern u8 D_00435C00[];

void kwlnPrintTaskTreeRecursive(KwlnTask *task, s32 depth)
{
    sdfDebugPrintf(D_00435C00, kwlnTaskTreePrefix, task);
    if (task->childList != NULL) {
        if (task->parent == NULL) {
            kwlnTaskTreePrefix[0] = ' ';
        } else {
            if (task->next == NULL) {
                kwlnTaskTreePrefix[depth * 4] = ' ';
            } else {
                kwlnTaskTreePrefix[depth * 4] = '|';
            }
        }
        kwlnTaskTreePrefix[depth * 4 + 4] = 0;
        kwlnPrintTaskTreeRecursive(task->childList, depth + 1);
        kwlnTaskTreePrefix[depth * 4] = 0;
    }
    if (task->next != NULL) {
        kwlnPrintTaskTreeRecursive(task->next, depth);
    }
}




/* Blank the task-name buffer, then print each root in the three scheduler lists. */
void kwlnVisitTaskForestRoots(void)
{
    KwlnTask* task;
    s32 prefixByteIndex;

    for (prefixByteIndex = 0; prefixByteIndex < 0x40; prefixByteIndex++) {
        kwlnTaskTreePrefix[prefixByteIndex] = 0x20;
    }
    for (task = kwlnDelayedStartTaskHead; task != NULL; task = task->listNext) {
        if (task->parent == NULL) {
            kwlnTaskTreePrefix[0] = 0;
            kwlnPrintTaskTreeRecursive(task, 0);
        }
    }
    for (task = kwlnActiveTaskHead; task != NULL; task = task->listNext) {
        if (task->parent == NULL) {
            kwlnTaskTreePrefix[0] = 0;
            kwlnPrintTaskTreeRecursive(task, 0);
        }
    }
    for (task = kwlnDelayedDestroyTaskHead; task != NULL; task = task->listNext) {
        if (task->parent == NULL) {
            kwlnTaskTreePrefix[0] = 0;
            kwlnPrintTaskTreeRecursive(task, 0);
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
    s32 taskNameByteIndex = 0;

    if (task == NULL) {
        return NULL;
    }
    task->nameSum = 0;
    while ((task->name[taskNameByteIndex] = name[taskNameByteIndex]) != 0 && taskNameByteIndex < 0x18) {
        task->nameSum += name[taskNameByteIndex];
        taskNameByteIndex++;
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

/* Destroy a live task: running tasks queue the request, while a task already
 * being destroyed finalizes at once when the new delay is zero. */
s32 kwlnTaskDestroyWithHierarchy(KwlnTask* task, s32 delayTicks)
{
    if (task == 0 || task == (KwlnTask*)-1) {
        return 0;
    }
    switch (task->flags & 0xF) {
    case 1:
    case 2:
        task->destroyDelayTicks = delayTicks;
        kwlnTaskRequestDestroy(task);
        return 1;
    case 3:
        task->destroyDelayTicks = delayTicks;
        if (task->destroyDelayTicks == 0) {
            kwlnTaskFinalizeDestroy(task);
            return 1;
        }
        break;
    case 0:
    default:
        return 0;
    }
    return 1;
}

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
    s32 nameCharacterIndex;
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
                for (nameCharacterIndex = nameLength; ; nameCharacterIndex--) {
                    if (name[nameCharacterIndex] != task->name[nameCharacterIndex]) {
                        break;
                    }
                    if (nameCharacterIndex == 0) {
                        return task;
                    }
                }
            }
            task = task->listNext;
        }
    }
    return NULL;
}

KwlnTask* kwlnTaskFindByPriority(u32 priority)
{
    s32 taskListIndex;
    KwlnTask* node;

    node = 0;
    taskListIndex = 0;
    for (; taskListIndex < 3; taskListIndex++) {
        switch (taskListIndex) {
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
            if (node->priority == priority) {
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
    s32 taskListIndex;
    KwlnTask* node;
    node = 0;
    taskListIndex = 0;
    for (; taskListIndex < 3; taskListIndex++)
    {
        switch (taskListIndex)
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

void kwlnTaskSetUserValue(KwlnTask* task, u32 userValue)
{
    task->userValue = userValue;
}

u32 kwlnTaskGetUserValue(KwlnTask* task)
{
    return task->userValue;
}

void func_00101960(void) {
}

void kwlnTaskAddChild(KwlnTask* parent, KwlnTask* child)
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

