#include "common.h"
#include "sdf.h"

extern u8 D_003BD39C;

extern void func_00100000(void);
s32 D_003BD2D0 __attribute__((section(".sdata"), aligned(8))) = (s32)func_00100000;
void (*sdfTickCallback)(void) __attribute__((section(".sdata"))) = NULL;

extern void sdfSleepThreadCount(s32);

extern void sdfPadUpdatePorts(void);

extern void sdfPadBuildButtonStates(void);

extern void sdfTickThreadPriorityOverride(void);

extern void func_002D00F8(u32 heapSize);
extern void func_002D1590(u32 resourceAddress);
extern void sdfInitGeneralHeap(u32 heapSize);
extern void sdfTexInitializeLists(void);
extern void sdfTexInitializeSemaphore(void);
extern void sdfRegisterTextureReleaseRequestHandler(void);
extern void sdfPadInit(void);
extern s32 sdfStartQueuedThreadWakeWorker(void);
extern void sdfInitializeResourceQueuesAndTextureWords();
extern void sdfConsUploadDmaProgram(s32 size);
extern void effMiscSeedRandomFromClock(void *state);
extern void sdfStartTrackedThread(SdfThreadNode *node, s32 entry, s32 stack, s64 stackSize, s32 priority, s32 arg);
extern void sdfRunTickWorkerThread(void);
extern SdfThreadNode D_003BD990;
extern u8 D_003DFEF0[0x2000];

extern s32 CancelWakeupThread(s32 threadId);

extern s32 GetThreadId(void);

extern s32 SetAlarm(u16 delay, void (*callback)(s32 alarmId, u16 time, void *common), void *common);

extern s32 sdfCreateThread(s32 entry, s32 stack, s32 stackSize, s32 priority);

extern s32 WaitSema(s32 sema);

extern s32 SignalSema(s32 sema);

extern void _StartThread(s32 threadId, s32 arg);

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

void func_002CF9A8(u32 heapSize, u32 initialHeapSize, u32 resourceAddress) {
    *(volatile u32 *)0x10000010 = 0x83;
    *(volatile u32 *)0x10000000 = 0;
    func_002D00F8(initialHeapSize);
    sdfInitGeneralHeap(heapSize);
    sdfTexInitializeLists();
    sdfTexInitializeSemaphore();
    sdfRegisterTextureReleaseRequestHandler();
    sdfPadInit();
    sdfStartQueuedThreadWakeWorker();
    func_002D1590(resourceAddress);
    sdfInitializeResourceQueuesAndTextureWords(0x40);
    sdfConsUploadDmaProgram(D_003BD2D0);
    effMiscSeedRandomFromClock(NULL);
    sdfStartTrackedThread(&D_003BD990, (s32)sdfRunTickWorkerThread, (s32)&D_003DFEF0, 0x2000, 0x4C, 0);
}

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
