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



extern KwlnTask* runninglistf;

extern void StartProcess(KwlnTask* task);

extern void StartingProcess(void);

extern KwlnTask* startinglistf;

extern void kwlnTaskFinalizeDestroy(KwlnTask* task);

extern void EndProcess(KwlnTask* task);

extern void dds3EndingProcess(void);

extern KwlnTask* endinglistf;

extern s32 startinglistcnt;

extern s32 endinglistcnt;

extern s32 runninglistcnt;

#include "sdf_debug_api.h"

extern u8 D_00435BF8[];

extern u8 D_00411008[];

extern u8 D_00411038[];

extern u8 D_00411048[];

extern u8 D_00411078[];

extern void RunningProcess(void);



void StartProcess(KwlnTask* task)
{
    DeleteProcessList(task);
    task->flags = (task->flags & ~KWLN_TASK_STATE_MASK) | KWLN_TASK_ACTIVE;
    AddProcessList(task);
    task->unk24 = 0;
    task->timer = 0;
}

void StartingProcess(void)
{
    KwlnTask* node;
    KwlnTask* curr;

    node = startinglistf;
    while (node != 0) {
        if (node->startDelayTicks > 0) {
            node->startDelayTicks--;
        }
        curr = node;
        node = node->listNext;
        if (curr->startDelayTicks == 0) {
            StartProcess(curr);
        }
    }
}

extern s8 dds3PadButtonPrev[2][2][16];
extern u8 dds3PadButtonState[2][2][16];
extern u8 D_00435C30[2][2][2];
extern s8 D_00435C38[2][2];
extern u8 D_00435C40[2][2][2];
extern u8 D_00435C48[2][2];
extern KwlnTask *runprocess;

s32 RunProcess(KwlnTask *task) {
    s32 port;
    s32 inputIndex;
    s32 nextUpdate;

    if (task->flags & 0x20) {
        return 1;
    }
    runprocess = task;

    if (task->flags & 0x10) {
        for (port = 0; port < 2; port++) {
            for (inputIndex = 0; inputIndex < 16; inputIndex++) {
                dds3PadButtonState[0][port][inputIndex] = 0;
                dds3PadButtonState[1][port][inputIndex] = 0;
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
                dds3PadButtonState[0][port][inputIndex] = dds3PadButtonPrev[0][port][inputIndex];
                dds3PadButtonState[1][port][inputIndex] = dds3PadButtonPrev[1][port][inputIndex];
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
            EndProcess(task);
            runprocess = 0;
            return 0;
        }
    }
    task->timer++;
    runprocess = 0;
    return 1;
}

void RunningProcess(void)
{
    KwlnTask* active = runninglistf;
    KwlnTask* delayed = endinglistf;
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
                    if (RunProcess(current) != 0) {
                        active = current->listNext;
                    } else {
                        active = previous != 0 ? previous->listNext : runninglistf;
                    }
                } else {
                    current = delayed;
                    previous = current->listPrev;
                    if (RunProcess(current) != 0) {
                        delayed = current->listNext;
                    } else {
                        delayed = previous != 0 ? previous->listNext : endinglistf;
                    }
                }
            } else {

                do {
                    current = active;
                    drainPrevious = current->listPrev;
                    if (RunProcess(current) != 0) {
                        active = current->listNext;
                    } else {
                        active = drainPrevious != 0 ? drainPrevious->listNext : runninglistf;
                    }
                } while (active != 0);
                return;
            }
        } else {
            if (delayed != 0) {

                do {
                    current = delayed;
                    drainPrevious = current->listPrev;
                    if (RunProcess(current) != 0) {
                        delayed = current->listNext;
                    } else {
                        delayed = drainPrevious != 0 ? drainPrevious->listNext : endinglistf;
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
        dds3KillProcess(child, 0);
        child = next;
    }
    DeleteProcessList(task);
    if (task->destroy != 0) {
        task->destroy(task);
    }
    task->flags &= ~KWLN_TASK_STATE_MASK;
    dds3UnlinkProcess(task);
    sdfChipFreeDirect(task);
}

void EndProcess(KwlnTask* task)
{
    u32 state;

    state = task->flags & KWLN_TASK_STATE_MASK;
    if (state >= KWLN_TASK_DESTROY_PENDING) {
        return;
    }
    if (state == KWLN_TASK_DETACHED) {
        return;
    }
    DeleteProcessList(task);
    task->flags = (task->flags & ~KWLN_TASK_STATE_MASK) | KWLN_TASK_DESTROY_PENDING;
    AddProcessList(task);
    if (task->destroyDelayTicks == 0) {
        kwlnTaskFinalizeDestroy(task);
    }
}

void dds3EndingProcess(void)
{
    KwlnTask* node;
    KwlnTask* curr;

    node = endinglistf;
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

void ChangeProcessFlagChild(s32 setFlags, KwlnTask* task, u32 flags)
{
    KwlnTask* child;

    if (setFlags != 0) {
        task->flags |= flags & KWLN_TASK_MUTABLE_FLAGS_MASK;
    } else {
        task->flags &= ~(flags & KWLN_TASK_MUTABLE_FLAGS_MASK);
    }
    child = task->childList;
    while (child != 0) {
        ChangeProcessFlagChild(setFlags, child, flags);
        child = child->next;
    }
}

void ChangeProcessFlag(s32 setFlags, KwlnTask *task, u32 flags, s32 scope)
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
                node = startinglistf;
                break;
            case 1:
                node = runninglistf;
                break;
            case 2:
                node = endinglistf;
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
        ChangeProcessFlagChild(setFlags, task, flags);
        break;
    }
}

s32 dds3GetProcessCount(u32 state)
{
    switch (state & KWLN_TASK_STATE_MASK) {
    case KWLN_TASK_DELAYED_START:
        return startinglistcnt;
    case KWLN_TASK_ACTIVE:
        return runninglistcnt;
    case KWLN_TASK_DESTROY_PENDING:
        return endinglistcnt;
    default:
        return 0;
    }
}

KwlnTask *dds3SearchProcessNumber(u32 taskState, s32 taskIndex)
{
    KwlnTask *task = startinglistf;
    s32 listPosition;

    switch (taskState & KWLN_TASK_STATE_MASK) {
    case KWLN_TASK_DELAYED_START:
        break;
    case KWLN_TASK_ACTIVE:
        task = runninglistf;
        break;
    case KWLN_TASK_DESTROY_PENDING:
        task = endinglistf;
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

void dds3PrintProcessList(void)
{
    u8* queueCountFormat;

    kwlnTraverseDiagnosticTaskQueue(startinglistf, D_00411008);
    queueCountFormat = D_00411038;
    printf(queueCountFormat, startinglistcnt);
    kwlnTraverseDiagnosticTaskQueue(runninglistf, D_00411048);
    printf(queueCountFormat, runninglistcnt);
    kwlnTraverseDiagnosticTaskQueue(endinglistf, D_00411078);
    printf(queueCountFormat, endinglistcnt);
    printf(D_00435BF8);
}

extern u8 dds3ProcessTreeDepth[];
extern u8 D_00435C00[];

void DebugProcessTree(KwlnTask *task, s32 depth)
{
    printf(D_00435C00, dds3ProcessTreeDepth, task);
    if (task->childList != NULL) {
        if (task->parent == NULL) {
            dds3ProcessTreeDepth[0] = ' ';
        } else {
            if (task->next == NULL) {
                dds3ProcessTreeDepth[depth * 4] = ' ';
            } else {
                dds3ProcessTreeDepth[depth * 4] = '|';
            }
        }
        dds3ProcessTreeDepth[depth * 4 + 4] = 0;
        DebugProcessTree(task->childList, depth + 1);
        dds3ProcessTreeDepth[depth * 4] = 0;
    }
    if (task->next != NULL) {
        DebugProcessTree(task->next, depth);
    }
}




/* Blank the task-name buffer, then print each root in the three scheduler lists. */
void dds3PrintProcessTree(void)
{
    KwlnTask* task;
    s32 prefixByteIndex;

    for (prefixByteIndex = 0; prefixByteIndex < 0x40; prefixByteIndex++) {
        dds3ProcessTreeDepth[prefixByteIndex] = 0x20;
    }
    for (task = startinglistf; task != NULL; task = task->listNext) {
        if (task->parent == NULL) {
            dds3ProcessTreeDepth[0] = 0;
            DebugProcessTree(task, 0);
        }
    }
    for (task = runninglistf; task != NULL; task = task->listNext) {
        if (task->parent == NULL) {
            dds3ProcessTreeDepth[0] = 0;
            DebugProcessTree(task, 0);
        }
    }
    for (task = endinglistf; task != NULL; task = task->listNext) {
        if (task->parent == NULL) {
            dds3ProcessTreeDepth[0] = 0;
            DebugProcessTree(task, 0);
        }
    }
}

s32 dds3ProcessMain(void)
{
    StartingProcess();
    RunningProcess();
    dds3EndingProcess();
    return 1;
}

KwlnTask *dds3StartProcess(const char *name, u32 priority, s32 startDelay, s32 destroyDelay,
                         TaskUpdate update, TaskDestroy destroy, u32 userValue) {
    KwlnTask *task = sdfChipAllocDirect(sizeof(KwlnTask));
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
    AddProcessList(task);
    if (task->startDelayTicks == 0) {
        StartProcess(task);
    }
    return task;
}

s32 dds3KillProcessName(const char* name, s32 delayTicks)
{
    KwlnTask* task;

    task = dds3SearchProcessName(name);
    if (task == 0) {
        return 0;
    }
    return dds3KillProcess(task, delayTicks);
}

/* Destroy a live task: running tasks queue the request, while a task already
 * being destroyed finalizes at once when the new delay is zero. */
s32 dds3KillProcess(KwlnTask* task, s32 delayTicks)
{
    if (task == 0 || task == (KwlnTask*)-1) {
        return 0;
    }
    switch (task->flags & 0xF) {
    case 1:
    case 2:
        task->destroyDelayTicks = delayTicks;
        EndProcess(task);
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

void dds3EndProcess(KwlnTask* task)
{
    if ((task->flags & KWLN_TASK_STATE_MASK) != KWLN_TASK_ACTIVE) {
        return;
    }
    DeleteProcessList(task);
    task->flags = (task->flags & ~KWLN_TASK_STATE_MASK) | KWLN_TASK_DESTROY_PENDING;
    AddProcessList(task);
}

void dds3SetShutdownTimer(KwlnTask* task, s32 delayTicks)
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

s32 dds3GetProcessStat(KwlnTask* task)
{
    u32 state;

    if (dds3ExistProcess(task) == 0) {
        return KWLN_TASK_DETACHED;
    }
    state = task->flags & KWLN_TASK_STATE_MASK;
    return (state < KWLN_TASK_STATE_LIMIT) ? state : KWLN_TASK_DETACHED;
}

KwlnTask* dds3SearchProcessName(const char* name)
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
            task = startinglistf;
            break;
        case 1:
            task = runninglistf;
            break;
        case 2:
            task = endinglistf;
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

KwlnTask* dds3SearchProcessPriority(u32 priority)
{
    s32 taskListIndex;
    KwlnTask* node;

    node = 0;
    taskListIndex = 0;
    for (; taskListIndex < 3; taskListIndex++) {
        switch (taskListIndex) {
        case 0:
            node = startinglistf;
            break;
        case 1:
            node = runninglistf;
            break;
        case 2:
            node = endinglistf;
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
s32 dds3ExistProcess(KwlnTask* target)
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
            node = startinglistf;
            break;
        case 1:
            node = runninglistf;
            break;
        case 2:
            node = endinglistf;
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

u32 dds3GetProcessStartTime(KwlnTask* task)
{
    return task->unk24;
}

/* Persona 4 dds3GetProcessRunTime @ 00452540 (src/Kernel/sdkTask.c), recompiled unchanged */
u32 dds3GetProcessRunTime(KwlnTask* task)
{
    return task->timer;
}

void dds3SetProcessData(KwlnTask* task, u32 userValue)
{
    task->userValue = userValue;
}

u32 dds3GetProcessData(KwlnTask* task)
{
    return task->userValue;
}

void func_00101960(void) {
}

void dds3LinkProcess(KwlnTask* parent, KwlnTask* child)
{
    KwlnTask* sibling;

    if (child->parent != NULL) {
        dds3UnlinkProcess(child);
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

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", runprocess);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_00435BF8);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_00435C00);

