#include "common.h"
#include "sdf.h"

extern s32 sdfCreateThread(s32 entry, s32 stack, s32 stackSize, s32 priority);
extern s32 WaitSema(s32);
extern s32 SignalSema(s32);
extern void _StartThread(s32, s32);
extern s32 sdfTrackedThreadSemaphore;
extern SdfThreadNode *sdfTrackedThreadHead;

extern s32 CancelWakeupThread(u64);

extern u64 GetThreadId(void);

extern u8 D_00438A8C;

extern void (*D_004389C4)(void);

extern void sdfSleepThreadCount(s32);

extern void sdfPadUpdatePorts(void);

extern void sdfPadBuildButtonStates(void);

extern void sdfTickThreadPriorityOverride(void);

void sdfWakeAlarmThread(u32 unused0, u32 unused1, u32 threadId) {
    iWakeupThread(threadId);
}

/* Clear any pending wakeup before arming a 16-bit delay for this thread. */
void sdfSleepWithAlarm(u32 delay) {
    u64 threadId = GetThreadId();
    CancelWakeupThread(threadId);
    SetAlarm(delay & 0xFFFF, sdfWakeAlarmThread, threadId);
    SleepThread();
}

INCLUDE_ASM(const s32, "game/code_00328778", func_003287E0);

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
        if (!D_00438A8C) {
            sdfPadBuildButtonStates();
        }
        sdfTickThreadPriorityOverride();
        if (D_004389C4 != NULL) {
            D_004389C4();
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00328778", func_00328858);

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
    u64 threadId;
    s32 cancelledWakeup;

    threadId = GetThreadId();
    cancelledWakeup = CancelWakeupThread(threadId);
    count = count - cancelledWakeup;
    do {
        count = count - 1;
        SleepThread();
    } while (0 < count);
}

INCLUDE_ASM(const s32, "game/code_00328778", func_003289C8);
