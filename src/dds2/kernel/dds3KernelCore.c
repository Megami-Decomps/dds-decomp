#include "common.h"
#include "kwln_task_add_child.h"
#include "sdf_chip.h"

#include "kwln.h"
#include "kwln_task_update_count.h"
#include "kwln_process_registration.h"
#include "kwln_task_queue_query.h"
#include "kwln_task_priority_query.h"
#include "kwln_task_create.h"
#include "kwln_task_flags.h"
#include "kwln_task_state.h"
#include "kwln_task_state_query.h"
#include "kwln_task_lifecycle.h"
#include "kwln_task_queue_mutation.h"
#include "kwln_task_runner.h"
#include "kwln_process_main_result.h"



extern dds3ProcessID_t* runninglistf;

extern void StartProcess(dds3ProcessID_t* task);

extern void StartingProcess(void);

extern dds3ProcessID_t* startinglistf;

extern void kwlnTaskFinalizeDestroy(dds3ProcessID_t* task);

extern void EndProcess(dds3ProcessID_t* task);

extern void dds3EndingProcess(void);

extern dds3ProcessID_t* endinglistf;

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



void StartProcess(dds3ProcessID_t* task)
{
    DeleteProcessList(task);
    task->flags = (task->flags & ~KWLN_TASK_STATE_MASK) | KWLN_TASK_ACTIVE;
    AddProcessList(task);
    task->unk24 = 0;
    task->runUpdateCount = 0;
}

void StartingProcess(void)
{
    dds3ProcessID_t* node;
    dds3ProcessID_t* curr;

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
extern dds3ProcessID_t *runprocess;

KwlnTaskCursorAction RunProcess(dds3ProcessID_t *task) {
    s32 port;
    s32 inputIndex;
    KwlnTaskUpdateResult nextUpdate;

    if (task->flags & KWLN_TASK_SKIP_UPDATE) {
        return KWLN_TASK_CURSOR_ACTION_ADVANCE;
    }
    runprocess = task;

    if (task->flags & KWLN_TASK_CLEAR_INPUT_STATE) {
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

    if (task->update != NULL && task->update != (KwlnTaskUpdateCallback)KWLN_TASK_UPDATE_FINISH_CALLBACK) {
        nextUpdate = task->update(task);
        if (nextUpdate != KWLN_TASK_UPDATE_RETAIN_CALLBACK) {
            task->update = (KwlnTaskUpdateCallback)nextUpdate;
        }
        if (nextUpdate == KWLN_TASK_UPDATE_FINISH_CALLBACK &&
            (task->flags & KWLN_TASK_STATE_MASK) == KWLN_TASK_ACTIVE) {
            EndProcess(task);
            runprocess = 0;
            return KWLN_TASK_CURSOR_ACTION_RELOAD;
        }
    }
    task->runUpdateCount++;
    runprocess = 0;
    return KWLN_TASK_CURSOR_ACTION_ADVANCE;
}

void RunningProcess(void)
{
    dds3ProcessID_t* active = runninglistf;
    dds3ProcessID_t* destroyPending = endinglistf;
    dds3ProcessID_t* current;
    /* The two single-queue drains share a predecessor snapshot; the merge
     * keeps its own. A step may unlink the selected node. SMT3 RunningProcess
     * retains the selected-node role (dds3KernelCore.c:591-717). */
    dds3ProcessID_t* drainPrevious;

    /* Ending-queue tasks participate in this priority-ordered update pass.
     * dds3EndingProcess later decrements positive delays and finalizes at zero;
     * RunProcess still honors KWLN_TASK_SKIP_UPDATE. */

    for (;;) {
        if (active != 0) {
            if (destroyPending != 0) {
                dds3ProcessID_t* previous;

                if (active->priority <= destroyPending->priority) {
                    current = active;
                    previous = current->listPrev;
                    if (RunProcess(current) != KWLN_TASK_CURSOR_ACTION_RELOAD) {
                        active = current->listNext;
                    } else {
                        active = previous != 0 ? previous->listNext : runninglistf;
                    }
                } else {
                    current = destroyPending;
                    previous = current->listPrev;
                    if (RunProcess(current) != KWLN_TASK_CURSOR_ACTION_RELOAD) {
                        destroyPending = current->listNext;
                    } else {
                        destroyPending = previous != 0 ? previous->listNext : endinglistf;
                    }
                }
            } else {

                do {
                    current = active;
                    drainPrevious = current->listPrev;
                    if (RunProcess(current) != KWLN_TASK_CURSOR_ACTION_RELOAD) {
                        active = current->listNext;
                    } else {
                        active = drainPrevious != 0 ? drainPrevious->listNext : runninglistf;
                    }
                } while (active != 0);
                return;
            }
        } else {
            if (destroyPending != 0) {

                do {
                    current = destroyPending;
                    drainPrevious = current->listPrev;
                    if (RunProcess(current) != KWLN_TASK_CURSOR_ACTION_RELOAD) {
                        destroyPending = current->listNext;
                    } else {
                        destroyPending = drainPrevious != 0 ? drainPrevious->listNext : endinglistf;
                    }
                } while (destroyPending != 0);
            }
            return;
        }
    }
}

void kwlnTaskFinalizeDestroy(dds3ProcessID_t* task)
{
    dds3ProcessID_t* child;
    dds3ProcessID_t* next;

    child = task->childList;
    while (child != 0) {
        next = child->nextSibling;
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

void EndProcess(dds3ProcessID_t* task)
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
    dds3ProcessID_t* node;
    dds3ProcessID_t* curr;

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

void ChangeProcessFlagChild(KwlnTaskFlagUpdateAction action, dds3ProcessID_t* task, u32 flags)
{
    dds3ProcessID_t* child;

    if (action != KWLN_TASK_FLAG_UPDATE_CLEAR) {
        task->flags |= flags & KWLN_TASK_MUTABLE_FLAGS_MASK;
    } else {
        task->flags &= ~(flags & KWLN_TASK_MUTABLE_FLAGS_MASK);
    }
    child = task->childList;
    while (child != 0) {
        ChangeProcessFlagChild(action, child, flags);
        child = child->nextSibling;
    }
}

void ChangeProcessFlag(KwlnTaskFlagUpdateAction action, dds3ProcessID_t *task, u32 flags,
                       KwlnTaskFlagScope scopeArg)
{
    dds3ProcessID_t *node = NULL;
    s32 state;
    s32 scope = (s32)scopeArg; /* Keep the selector in its original signed domain. */

    switch (scope) {
    case KWLN_TASK_FLAG_SCOPE_TARGET:
        if (action != KWLN_TASK_FLAG_UPDATE_CLEAR) {
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
                        if (action != KWLN_TASK_FLAG_UPDATE_CLEAR) {
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
        ChangeProcessFlagChild(action, task, flags);
        break;
    }
}

KwlnTaskQueueCount dds3GetProcessCount(KwlnTaskState state)
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

dds3ProcessID_t *dds3SearchProcessNumber(KwlnTaskState taskState, KwlnTaskQueueIndex queueIndex)
{
    dds3ProcessID_t *task = startinglistf;
    KwlnTaskQueueIndex listPosition;

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
    for (listPosition = 0; listPosition < queueIndex && task != NULL; listPosition++) {
        task = task->listNext;
    }
    return task;
}

void kwlnTraverseDiagnosticTaskQueue(dds3ProcessID_t* task, void* unused)
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

void DebugProcessTree(dds3ProcessID_t *task, s32 depth)
{
    printf(D_00435C00, dds3ProcessTreeDepth, task);
    if (task->childList != NULL) {
        if (task->parent == NULL) {
            dds3ProcessTreeDepth[0] = ' ';
        } else {
            if (task->nextSibling == NULL) {
                dds3ProcessTreeDepth[depth * 4] = ' ';
            } else {
                dds3ProcessTreeDepth[depth * 4] = '|';
            }
        }
        dds3ProcessTreeDepth[depth * 4 + 4] = 0;
        DebugProcessTree(task->childList, depth + 1);
        dds3ProcessTreeDepth[depth * 4] = 0;
    }
    if (task->nextSibling != NULL) {
        DebugProcessTree(task->nextSibling, depth);
    }
}




/* Blank the task-name buffer, then print each root in the three scheduler lists. */
void dds3PrintProcessTree(void)
{
    dds3ProcessID_t* task;
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

KwlnProcessMainResult dds3ProcessMain(void)
{
    StartingProcess();
    RunningProcess();
    dds3EndingProcess();
    return KWLN_PROCESS_MAIN_CONTINUE;
}

dds3ProcessID_t *dds3StartProcess(const char *name, u32 priority, s32 startDelay, s32 destroyDelay,
                         KwlnTaskUpdateCallback update, TaskDestroy destroy, u32 userValue) {
    dds3ProcessID_t *task = sdfChipAllocDirect(sizeof(dds3ProcessID_t));
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
    task->runUpdateCount = 0;
    task->listNext = NULL;
    task->listPrev = NULL;
    task->parent = NULL;
    task->childList = NULL;
    task->nextSibling = NULL;
    AddProcessList(task);
    if (task->startDelayTicks == 0) {
        StartProcess(task);
    }
    return task;
}

KwlnTaskKillRequestResult dds3KillProcessName(const char* name, s32 delayTicks)
{
    dds3ProcessID_t* task;

    task = dds3SearchProcessName(name);
    if (task == 0) {
        return KWLN_TASK_KILL_REQUEST_REJECTED;
    }
    return dds3KillProcess(task, delayTicks);
}

/* Queued and active tasks move to destruction pending; a pending task
 * finalizes immediately when its delay is reset to zero. */
KwlnTaskKillRequestResult dds3KillProcess(dds3ProcessID_t* task, s32 delayTicks)
{
    if (task == 0 || task == KWLN_TASK_HANDLE_INVALID) {
        return KWLN_TASK_KILL_REQUEST_REJECTED;
    }
    switch (task->flags & KWLN_TASK_STATE_MASK) {
    case KWLN_TASK_DELAYED_START:
    case KWLN_TASK_ACTIVE:
        task->destroyDelayTicks = delayTicks;
        EndProcess(task);
        return KWLN_TASK_KILL_REQUEST_ACCEPTED;
    case KWLN_TASK_DESTROY_PENDING:
        task->destroyDelayTicks = delayTicks;
        if (task->destroyDelayTicks == 0) {
            kwlnTaskFinalizeDestroy(task);
            return KWLN_TASK_KILL_REQUEST_ACCEPTED;
        }
        break;
    case KWLN_TASK_DETACHED:
    default:
        return KWLN_TASK_KILL_REQUEST_REJECTED;
    }
    return KWLN_TASK_KILL_REQUEST_ACCEPTED;
}

void dds3EndProcess(dds3ProcessID_t* task)
{
    if ((task->flags & KWLN_TASK_STATE_MASK) != KWLN_TASK_ACTIVE) {
        return;
    }
    DeleteProcessList(task);
    task->flags = (task->flags & ~KWLN_TASK_STATE_MASK) | KWLN_TASK_DESTROY_PENDING;
    AddProcessList(task);
}

void dds3SetShutdownTimer(dds3ProcessID_t* task, s32 delayTicks)
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

KwlnTaskState dds3GetProcessStat(const dds3ProcessID_t* task)
{
    u32 state;

    if (dds3ExistProcess(task) == KWLN_PROCESS_NOT_REGISTERED) {
        return KWLN_TASK_DETACHED;
    }
    state = task->flags & KWLN_TASK_STATE_MASK;
    return (state < KWLN_TASK_STATE_LIMIT) ? state : KWLN_TASK_DETACHED;
}

dds3ProcessID_t* dds3SearchProcessName(const char* name)
{
    s32 nameLength;
    s32 nameSum;
    s32 queueIndex;
    s32 nameCharacterIndex;
    dds3ProcessID_t* task;

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

dds3ProcessID_t* dds3SearchProcessPriority(u32 priority)
{
    s32 taskListIndex;
    dds3ProcessID_t* node;

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
KwlnProcessRegistrationResult dds3ExistProcess(const dds3ProcessID_t* target)
{
    s32 taskListIndex;
    dds3ProcessID_t* node;
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
                return KWLN_PROCESS_REGISTERED;
            }
            node = node->listNext;
        }
    }
    return KWLN_PROCESS_NOT_REGISTERED;
}

u32 dds3GetProcessStartTime(const dds3ProcessID_t* task)
{
    return task->unk24;
}

/* Persona 4 dds3GetProcessRunTime @ 00452540 (src/Kernel/sdkTask.c), recompiled unchanged */
KwlnTaskUpdateCount dds3GetProcessRunTime(const dds3ProcessID_t* task)
{
    return task->runUpdateCount;
}

void dds3SetProcessData(dds3ProcessID_t* task, u32 userValue)
{
    task->userValue = userValue;
}

u32 dds3GetProcessData(const dds3ProcessID_t* task)
{
    return task->userValue;
}

void func_00101960(void) {
}

void dds3LinkProcess(dds3ProcessID_t* parent, dds3ProcessID_t* child)
{
    dds3ProcessID_t* sibling;

    if (child->parent != NULL) {
        dds3UnlinkProcess(child);
    }
    if (parent->childList != NULL) {
        sibling = parent->childList;
        while (sibling->nextSibling != NULL) {
            sibling = sibling->nextSibling;
        }
        sibling->nextSibling = child;
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
