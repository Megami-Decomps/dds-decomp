#include "common.h"
#include "sdf.h"

extern u8 D_003BD39C;

extern void (*sdfTickCallback)(void);

extern void sdfSleepThreadCount(s32);

extern void sdfPadUpdatePorts(void);

extern void sdfPadBuildButtonStates(void);

extern void sdfTickThreadPriorityOverride(void);

extern s32 CancelWakeupThread(s32 threadId);

extern s32 GetThreadId(void);

extern s32 SetAlarm(u16 delay, void (*callback)(s32 alarmId, u16 time, void *common), void *common);

extern s32 sdfCreateThread(s32 entry, s32 stack, s32 stackSize, s32 priority);

extern s32 WaitSema(s32 sema);

extern s32 SignalSema(s32 sema);

extern void _StartThread(s32 threadId, s32 arg);

extern u32 sdfTrackedThreadSemaphore;

extern SdfThreadNode *sdfTrackedThreadHead;

void sdfWakeAlarmThread(s32 unused0, u16 unused1, void *threadId) {
    iWakeupThread((s32)threadId);
}

/* Clear any pending wakeup before arming a 16-bit delay for this thread. */
void sdfSleepWithAlarm(u32 delay) {
    s32 threadId = GetThreadId();
    CancelWakeupThread(threadId);
    SetAlarm(delay & 0xFFFF, sdfWakeAlarmThread, (void *)threadId);
    SleepThread();
}

INCLUDE_ASM(const s32, "game/code_002CF8C8", func_002CF930);

/* EE timer 0 count register; subtraction is reduced modulo 2^16 so
 * wraparound does not make short elapsed intervals negative. */
u32 sdfGetElapsedTimerTicks(u32 previous) {
    u32 current;

    current = *(volatile u32 *)0x10000000;
    return (current - previous) & 0xFFFF;
}

void sdfRunTickWorkerThread(void) {
    for (;;) {
        sdfSleepThreadCount(1);
        sdfPadUpdatePorts();
        if (!D_003BD39C) {
            sdfPadBuildButtonStates();
        }
        sdfTickThreadPriorityOverride();
        if (sdfTickCallback != NULL) {
            sdfTickCallback();
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002CF8C8", func_002CF9A8);

/* Register the thread under the list semaphore before starting it. */
void sdfStartTrackedThread(SdfThreadNode *node, s32 entry, s32 stack, s64 stackSize, s32 priority, s32 arg) {
    node->threadId = sdfCreateThread(entry, stack, stackSize, priority);
    WaitSema(sdfTrackedThreadSemaphore);
    node->next = sdfTrackedThreadHead;
    sdfTrackedThreadHead = node;
    SignalSema(sdfTrackedThreadSemaphore);
    _StartThread(node->threadId, arg);
}

/* A previously queued wakeup counts toward the requested sleep count. */
void sdfSleepThreadCount(s32 count) {
    s32 threadId;
    s32 cancelledWakeup;

    threadId = GetThreadId();
    cancelledWakeup = CancelWakeupThread(threadId);
    count = count - cancelledWakeup;
    do {
        count = count - 1;
        SleepThread();
    } while (0 < count);
}

extern void ExitDeleteThread(void);
extern s32 TerminateThread(s32 threadId);
extern s32 DeleteThread(s32 threadId);

void func_002CFB18(SdfThreadNode *node) {
    SdfThreadNode **link;
    SdfThreadNode *current;
    s32 threadId;

    WaitSema(sdfTrackedThreadSemaphore);
    link = &sdfTrackedThreadHead;
    current = *link;
    while (current != node) {
        link = &current->next;
        current = current->next;
    }
    *link = node->next;
    SignalSema(sdfTrackedThreadSemaphore);
    threadId = node->threadId;
    if (threadId == GetThreadId()) {
        ExitDeleteThread();
        return;
    }
    TerminateThread(threadId);
    DeleteThread(threadId);
}
INCLUDE_SDATA(const s32, "game/code_002CF8C8", D_003BD2D0);

INCLUDE_SDATA(const s32, "game/code_002CF8C8", sdfTickCallback);
