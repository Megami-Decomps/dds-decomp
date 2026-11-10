#include "common.h"
#include "dds3_task_add_child.h"
#include "sdf_chip.h"
#include "kwln.h"
#include "dds3_task_update_count.h"
#include "dds3_process_registration.h"
#include "dds3_task_queue_query.h"
#include "dds3_task_priority_query.h"
#include "dds3_task_create.h"
#include "dds3_task_flags.h"
#include "dds3_task_state.h"
#include "dds3_task_state_query.h"
#include "dds3_task_lifecycle.h"
#include "dds3_task_queue_mutation.h"
#include "dds3_task_runner.h"
#include "dds3_process_main_result.h"

extern void StartProcess(dds3ProcessID_t* task);

extern void StartingProcess(void);

extern void RunningProcess(void);

extern void kwlnTaskFinalizeDestroy(dds3ProcessID_t* task);

extern void EndProcess(dds3ProcessID_t* task);

extern void dds3EndingProcess(void);

extern void DebugProcessTree(dds3ProcessID_t* task, s32 depth);




#include "sdf_debug_api.h"

extern dds3ProcessID_t* startinglistf;

extern s32 startinglistcnt;

extern dds3ProcessID_t* endinglistf;

extern s32 endinglistcnt;

extern dds3ProcessID_t* runninglistf;

extern s32 runninglistcnt;

extern dds3ProcessID_t *runprocess;

extern u8 D_003BA828[];

extern u8 D_003BA830[];

extern u8 dds3ProcessTreeDepth[];

extern s8 dds3PadButtonPrev[2][2][16];

extern u8 dds3PadButtonState[2][2][16];

extern u8 D_003BA860[2][2][2];
extern s8 D_003BA868[2][2];
extern u8 D_003BA870[2][2][2];
extern u8 D_003BA878[2][2];

extern u8 D_0039DE88[];

extern u8 D_0039DEB8[];

extern u8 D_0039DEC8[];

extern u8 D_0039DEF8[];

void StartProcess(dds3ProcessID_t* task)
{
    DeleteProcessList(task);
    task->flag = (task->flag & ~DDS3_TASK_STATE_MASK) | DDS3_TASK_ACTIVE;
    AddProcessList(task);
    task->starttime = 0;
    task->runtime = 0;
}

void StartingProcess(void)
{
    dds3ProcessID_t* node;
    dds3ProcessID_t* curr;

    node = startinglistf;
    while (node != 0) {
        if (node->starttimer > 0) {
            node->starttimer--;
        }
        curr = node;
        node = node->next;
        if (curr->starttimer == 0) {
            StartProcess(curr);
        }
    }
}

dds3TaskCursorAction_t RunProcess(dds3ProcessID_t *task) {
    s32 port;
    s32 inputIndex;
    dds3TaskUpdateResult_t nextUpdate;

    if (task->flag & DDS3_TASK_SKIP_UPDATE) {
        return DDS3_TASK_CURSOR_ACTION_ADVANCE;
    }
    runprocess = task;

    if (task->flag & DDS3_TASK_CLEAR_INPUT_STATE) {
        for (port = 0; port < 2; port++) {
            for (inputIndex = 0; inputIndex < 16; inputIndex++) {
                dds3PadButtonState[0][port][inputIndex] = 0;
                dds3PadButtonState[1][port][inputIndex] = 0;
            }
            for (inputIndex = 0; inputIndex < 2; inputIndex++) {
                D_003BA870[port][inputIndex][0] = 0;
                D_003BA878[port][inputIndex] = 0;
                D_003BA870[port][inputIndex][1] = 0;
            }
        }
    } else {
        for (port = 0; port < 2; port++) {
            for (inputIndex = 0; inputIndex < 16; inputIndex++) {
                dds3PadButtonState[0][port][inputIndex] = dds3PadButtonPrev[0][port][inputIndex];
                dds3PadButtonState[1][port][inputIndex] = dds3PadButtonPrev[1][port][inputIndex];
            }
            for (inputIndex = 0; inputIndex < 2; inputIndex++) {
                D_003BA870[port][inputIndex][0] = D_003BA860[port][inputIndex][0];
                D_003BA870[port][inputIndex][1] = D_003BA860[port][inputIndex][1];
                D_003BA878[port][inputIndex] = D_003BA868[port][inputIndex];
            }
        }
    }

    if (task->processfunc != NULL && task->processfunc != (dds3ProcessFunc_t)DDS3_TASK_UPDATE_FINISH_CALLBACK) {
        nextUpdate = task->processfunc(task);
        if (nextUpdate != DDS3_TASK_UPDATE_RETAIN_CALLBACK) {
            task->processfunc = (dds3ProcessFunc_t)nextUpdate;
        }
        if (nextUpdate == DDS3_TASK_UPDATE_FINISH_CALLBACK &&
            (task->flag & DDS3_TASK_STATE_MASK) == DDS3_TASK_ACTIVE) {
            EndProcess(task);
            runprocess = 0;
            return DDS3_TASK_CURSOR_ACTION_RELOAD;
        }
    }
    task->runtime++;
    runprocess = 0;
    return DDS3_TASK_CURSOR_ACTION_ADVANCE;
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
     * RunProcess still honors DDS3_TASK_SKIP_UPDATE. */

    for (;;) {
        if (active != 0) {
            if (destroyPending != 0) {
                dds3ProcessID_t* previous;

                if (active->priority <= destroyPending->priority) {
                    current = active;
                    previous = current->prev;
                    if (RunProcess(current) != DDS3_TASK_CURSOR_ACTION_RELOAD) {
                        active = current->next;
                    } else {
                        active = previous != 0 ? previous->next : runninglistf;
                    }
                } else {
                    current = destroyPending;
                    previous = current->prev;
                    if (RunProcess(current) != DDS3_TASK_CURSOR_ACTION_RELOAD) {
                        destroyPending = current->next;
                    } else {
                        destroyPending = previous != 0 ? previous->next : endinglistf;
                    }
                }
            } else {

                do {
                    current = active;
                    drainPrevious = current->prev;
                    if (RunProcess(current) != DDS3_TASK_CURSOR_ACTION_RELOAD) {
                        active = current->next;
                    } else {
                        active = drainPrevious != 0 ? drainPrevious->next : runninglistf;
                    }
                } while (active != 0);
                return;
            }
        } else {
            if (destroyPending != 0) {

                do {
                    current = destroyPending;
                    drainPrevious = current->prev;
                    if (RunProcess(current) != DDS3_TASK_CURSOR_ACTION_RELOAD) {
                        destroyPending = current->next;
                    } else {
                        destroyPending = drainPrevious != 0 ? drainPrevious->next : endinglistf;
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

    child = task->child;
    while (child != 0) {
        next = child->sibling;
        dds3KillProcess(child, 0);
        child = next;
    }
    DeleteProcessList(task);
    if (task->shutdownfunc != 0) {
        task->shutdownfunc(task);
    }
    task->flag &= ~DDS3_TASK_STATE_MASK;
    dds3UnlinkProcess(task);
    sdfChipFreeDirect(task);
}

void EndProcess(dds3ProcessID_t* task)
{
    u32 state;

    state = task->flag & DDS3_TASK_STATE_MASK;
    if (state >= DDS3_TASK_DESTROY_PENDING) {
        return;
    }
    if (state == DDS3_TASK_DETACHED) {
        return;
    }
    DeleteProcessList(task);
    task->flag = (task->flag & ~DDS3_TASK_STATE_MASK) | DDS3_TASK_DESTROY_PENDING;
    AddProcessList(task);
    if (task->shutdowntimer == 0) {
        kwlnTaskFinalizeDestroy(task);
    }
}

void dds3EndingProcess(void)
{
    dds3ProcessID_t* node;
    dds3ProcessID_t* curr;

    node = endinglistf;
    while (node != 0) {
        if (node->shutdowntimer > 0) {
            node->shutdowntimer--;
        }
        curr = node;
        node = node->next;
        if (curr->shutdowntimer == 0) {
            kwlnTaskFinalizeDestroy(curr);
        }
    }
}

void ChangeProcessFlagChild(dds3TaskFlagUpdateAction_t action, dds3ProcessID_t* task, u32 flags)
{
    dds3ProcessID_t* child;

    if (action != DDS3_TASK_FLAG_UPDATE_CLEAR) {
        task->flag |= flags & DDS3_TASK_MUTABLE_FLAGS_MASK;
    } else {
        task->flag &= ~(flags & DDS3_TASK_MUTABLE_FLAGS_MASK);
    }
    child = task->child;
    while (child != 0) {
        ChangeProcessFlagChild(action, child, flags);
        child = child->sibling;
    }
}

void ChangeProcessFlag(dds3TaskFlagUpdateAction_t action, dds3ProcessID_t *task, u32 flags,
                       dds3TaskFlagScope_t scopeArg)
{
    dds3ProcessID_t *node = NULL;
    s32 state;
    s32 scope = (s32)scopeArg; /* Keep the selector in its original signed domain. */

    switch (scope) {
    case DDS3_TASK_FLAG_SCOPE_TARGET:
        if (action != DDS3_TASK_FLAG_UPDATE_CLEAR) {
            task->flag |= flags & DDS3_TASK_MUTABLE_FLAGS_MASK;
        } else {
            task->flag &= ~(flags & DDS3_TASK_MUTABLE_FLAGS_MASK);
        }
        return;
    case DDS3_TASK_FLAG_SCOPE_OTHER_QUEUED_TASKS:
    case DDS3_TASK_FLAG_SCOPE_ALL_QUEUED_TASKS:
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
                u32 setMask = flags & DDS3_TASK_MUTABLE_FLAGS_MASK;

                while (node != NULL) {
                    if ((node != task && scope == DDS3_TASK_FLAG_SCOPE_OTHER_QUEUED_TASKS) ||
                        scope == DDS3_TASK_FLAG_SCOPE_ALL_QUEUED_TASKS) {
                        if (action != DDS3_TASK_FLAG_UPDATE_CLEAR) {
                            node->flag |= setMask;
                        } else {
                            node->flag &= ~setMask;
                        }
                    }
                    node = node->next;
                }
            }
        }
        return;
    case DDS3_TASK_FLAG_SCOPE_SUBTREE:
        ChangeProcessFlagChild(action, task, flags);
        break;
    }
}

dds3TaskQueueCount_t dds3GetProcessCount(dds3TaskState_t state)
{
    switch (state & DDS3_TASK_STATE_MASK) {
    case DDS3_TASK_DELAYED_START:
        return startinglistcnt;
    case DDS3_TASK_ACTIVE:
        return runninglistcnt;
    case DDS3_TASK_DESTROY_PENDING:
        return endinglistcnt;
    default:
        return 0;
    }
}

dds3ProcessID_t *dds3SearchProcessNumber(dds3TaskState_t taskState, dds3TaskQueueIndex_t queueIndex)
{
    dds3ProcessID_t *task = startinglistf;
    dds3TaskQueueIndex_t listPosition;

    switch (taskState & DDS3_TASK_STATE_MASK) {
    case DDS3_TASK_DELAYED_START:
        break;
    case DDS3_TASK_ACTIVE:
        task = runninglistf;
        break;
    case DDS3_TASK_DESTROY_PENDING:
        task = endinglistf;
        break;
    case DDS3_TASK_DETACHED:
    default:
        return NULL;
    }
    for (listPosition = 0; listPosition < queueIndex && task != NULL; listPosition++) {
        task = task->next;
    }
    return task;
}


void kwlnTraverseDiagnosticTaskQueue(dds3ProcessID_t* task, void* unused)
{
    if (task == 0) {
        return;
    }
    do {
        task = task->next;
    } while (task != 0);
}

void dds3PrintProcessList(void)
{
    u8* queueCountFormat;

    kwlnTraverseDiagnosticTaskQueue(startinglistf, D_0039DE88);
    queueCountFormat = D_0039DEB8;
    printf(queueCountFormat, startinglistcnt);
    kwlnTraverseDiagnosticTaskQueue(runninglistf, D_0039DEC8);
    printf(queueCountFormat, runninglistcnt);
    kwlnTraverseDiagnosticTaskQueue(endinglistf, D_0039DEF8);
    printf(queueCountFormat, endinglistcnt);
    printf(D_003BA828);
}

void DebugProcessTree(dds3ProcessID_t *task, s32 depth)
{
    printf(D_003BA830, dds3ProcessTreeDepth, task);
    if (task->child != NULL) {
        if (task->parent == NULL) {
            dds3ProcessTreeDepth[0] = ' ';
        } else {
            if (task->sibling == NULL) {
                dds3ProcessTreeDepth[depth * 4] = ' ';
            } else {
                dds3ProcessTreeDepth[depth * 4] = '|';
            }
        }
        dds3ProcessTreeDepth[depth * 4 + 4] = 0;
        DebugProcessTree(task->child, depth + 1);
        dds3ProcessTreeDepth[depth * 4] = 0;
    }
    if (task->sibling != NULL) {
        DebugProcessTree(task->sibling, depth);
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
    for (task = startinglistf; task != NULL; task = task->next) {
        if (task->parent == NULL) {
            dds3ProcessTreeDepth[0] = 0;
            DebugProcessTree(task, 0);
        }
    }
    for (task = runninglistf; task != NULL; task = task->next) {
        if (task->parent == NULL) {
            dds3ProcessTreeDepth[0] = 0;
            DebugProcessTree(task, 0);
        }
    }
    for (task = endinglistf; task != NULL; task = task->next) {
        if (task->parent == NULL) {
            dds3ProcessTreeDepth[0] = 0;
            DebugProcessTree(task, 0);
        }
    }
}

dds3ProcessMainResult_t dds3ProcessMain(void)
{
    StartingProcess();
    RunningProcess();
    dds3EndingProcess();
    return DDS3_PROCESS_MAIN_CONTINUE;
}

dds3ProcessID_t *dds3StartProcess(const char *name, u32 priority, s32 startDelay, s32 destroyDelay,
                         dds3ProcessFunc_t update, dds3ProcessShutdownFunc_t destroy, u32 userValue) {
    dds3ProcessID_t *task = sdfChipAllocDirect(sizeof(dds3ProcessID_t));
    s32 taskNameByteIndex = 0;

    if (task == NULL) {
        return NULL;
    }
    task->namehash = 0;
    while ((task->name[taskNameByteIndex] = name[taskNameByteIndex]) != 0 && taskNameByteIndex < 0x18) {
        task->namehash += name[taskNameByteIndex];
        taskNameByteIndex++;
    }
    task->priority = priority;
    task->flag = DDS3_TASK_DELAYED_START;
    task->starttimer = startDelay;
    task->shutdowntimer = destroyDelay;
    task->processfunc = update;
    task->shutdownfunc = destroy;
    task->workdata = userValue;
    task->name[0x17] = 0;
    task->starttime = 0;
    task->runtime = 0;
    task->next = NULL;
    task->prev = NULL;
    task->parent = NULL;
    task->child = NULL;
    task->sibling = NULL;
    AddProcessList(task);
    if (task->starttimer == 0) {
        StartProcess(task);
    }
    return task;
}

dds3TaskKillRequestResult_t dds3KillProcessName(const char* name, s32 delayTicks)
{
    dds3ProcessID_t* task;

    task = dds3SearchProcessName(name);
    if (task == 0) {
        return DDS3_TASK_KILL_REQUEST_REJECTED;
    }
    return dds3KillProcess(task, delayTicks);
}

/* Queued and active tasks move to destruction pending; a pending task
 * finalizes immediately when its delay is reset to zero. */
dds3TaskKillRequestResult_t dds3KillProcess(dds3ProcessID_t* task, s32 delayTicks)
{
    if (task == 0 || task == DDS3_TASK_HANDLE_INVALID) {
        return DDS3_TASK_KILL_REQUEST_REJECTED;
    }
    switch (task->flag & DDS3_TASK_STATE_MASK) {
    case DDS3_TASK_DELAYED_START:
    case DDS3_TASK_ACTIVE:
        task->shutdowntimer = delayTicks;
        EndProcess(task);
        return DDS3_TASK_KILL_REQUEST_ACCEPTED;
    case DDS3_TASK_DESTROY_PENDING:
        task->shutdowntimer = delayTicks;
        if (task->shutdowntimer == 0) {
            kwlnTaskFinalizeDestroy(task);
            return DDS3_TASK_KILL_REQUEST_ACCEPTED;
        }
        break;
    case DDS3_TASK_DETACHED:
    default:
        return DDS3_TASK_KILL_REQUEST_REJECTED;
    }
    return DDS3_TASK_KILL_REQUEST_ACCEPTED;
}

void dds3EndProcess(dds3ProcessID_t* task)
{
    if ((task->flag & DDS3_TASK_STATE_MASK) != DDS3_TASK_ACTIVE) {
        return;
    }
    DeleteProcessList(task);
    task->flag = (task->flag & ~DDS3_TASK_STATE_MASK) | DDS3_TASK_DESTROY_PENDING;
    AddProcessList(task);
}

void dds3SetShutdownTimer(dds3ProcessID_t* task, s32 delayTicks)
{
    u32 state;

    state = task->flag & DDS3_TASK_STATE_MASK;
    if (state == DDS3_TASK_DETACHED) {
        return;
    }
    if (state < DDS3_TASK_STATE_LIMIT) {
        task->shutdowntimer = delayTicks;
    }
}

dds3TaskState_t dds3GetProcessStat(const dds3ProcessID_t* task)
{
    u32 state;

    if (dds3ExistProcess(task) == DDS3_PROCESS_NOT_REGISTERED) {
        return DDS3_TASK_DETACHED;
    }
    state = task->flag & DDS3_TASK_STATE_MASK;
    return (state < DDS3_TASK_STATE_LIMIT) ? state : DDS3_TASK_DETACHED;
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
            if (task->namehash == nameSum) {
                for (nameCharacterIndex = nameLength; ; nameCharacterIndex--) {
                    if (name[nameCharacterIndex] != task->name[nameCharacterIndex]) {
                        break;
                    }
                    if (nameCharacterIndex == 0) {
                        return task;
                    }
                }
            }
            task = task->next;
        }
    }
    return NULL;
}

dds3ProcessID_t* dds3SearchProcessPriority(u32 prio)
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
            if (node->priority == prio)
            {
                return node;
            }
            node = node->next;
        }
    }
    return 0;
}

/* Persona 4 func_00452490 @ 00452490 (src/Kernel/sdkTask.c), recompiled unchanged */
dds3ProcessRegistrationResult_t dds3ExistProcess(const dds3ProcessID_t* target)
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
                return DDS3_PROCESS_REGISTERED;
            }
            node = node->next;
        }
    }
    return DDS3_PROCESS_NOT_REGISTERED;
}

u32 dds3GetProcessStartTime(const dds3ProcessID_t* task)
{
    return task->starttime;
}

/* Persona 4 dds3GetProcessRunTime @ 00452540 (src/Kernel/sdkTask.c), recompiled unchanged */
dds3TaskUpdateCount_t dds3GetProcessRunTime(const dds3ProcessID_t* task)
{
    return task->runtime;
}

void dds3SetProcessData(dds3ProcessID_t* task, u32 userValue)
{
    task->workdata = userValue;
}

u32 dds3GetProcessData(const dds3ProcessID_t* task)
{
    return task->workdata;
}

void func_00101A78(void) {
}

void dds3LinkProcess(dds3ProcessID_t* parent, dds3ProcessID_t* child)
{
    dds3ProcessID_t* tail;

    if (child->parent != NULL) {
        dds3UnlinkProcess(child);
    }

    if (parent->child != NULL) {
        tail = parent->child;
        while (tail->sibling != NULL) {
            tail = tail->sibling;
        }
        tail->sibling = child;
    } else {
        parent->child = child;
    }
    child->parent = parent;
}

INCLUDE_RODATA(const s32, "kernel/dds3KernelCore", D_0039DE88);

INCLUDE_RODATA(const s32, "kernel/dds3KernelCore", D_0039DEB8);

INCLUDE_RODATA(const s32, "kernel/dds3KernelCore", D_0039DEC8);

INCLUDE_RODATA(const s32, "kernel/dds3KernelCore", D_0039DEF8);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", runprocess);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_003BA828);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_003BA830);

