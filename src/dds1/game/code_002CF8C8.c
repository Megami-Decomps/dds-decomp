#include "common.h"
#include "eeregs.h"
#include "eff_random.h"
#include "sdf_timer.h"
#include "sdf.h"
#include "sdf_alarm.h"
#include "sdf_thread.h"

extern u8 D_003BD39C;

/* Workspace bytes passed to the console DMA setup allocation. */
s32 D_003BD2D0 __attribute__((section(".sdata"), aligned(8))) = 0x100000;
void (*sdfTickCallback)(void) __attribute__((section(".sdata"))) = NULL;

extern void sdfSleepThreadCount(s32 count);

extern void sdfPadUpdatePorts(void);

extern void sdfPadBuildButtonStates(void);

extern void sdfTickThreadPriorityOverride(void);

extern void sdfInitChipHeap(u32 heapSize);
extern void sdfInitializeGraphicsWorkers(u32 packetBufferBytes);
extern void sdfInitGeneralHeap(u32 heapSize);
extern void sdfTexInitializeLists(void);
extern void sdfTexInitializeSemaphore(void);
extern void sdfRegisterTextureReleaseRequestHandler(void);
extern void sdfPadInit(void);
extern s32 sdfStartQueuedThreadWakeWorker(void);
extern void sdfInitializeResourceQueuesAndTextureWords();
extern void sdfConsUploadDmaProgram(s32 size);
extern void sdfRunTickWorkerThread(void);
extern SdfThreadNode D_003BD990;
extern u8 D_003DFEF0[0x2000];

extern s32 CancelWakeupThread(s32 threadId);

extern s32 GetThreadId(void);

extern s32 SetAlarm(u16 delay, void (*callback)(s32 alarmId, u16 time, void *common), void *common);

extern s32 sdfCreateThread(void *entryAddress, void *workspace, s32 stackBytes, s32 priority);

extern s32 WaitSema(s32 sema);

extern s32 SignalSema(s32 sema);

extern void _StartThread(s32 threadId, s32 arg);

extern int iWakeupThread(int);

void sdfWakeAlarmThread(s32 unused0, u16 unused1, void *threadId) {
    iWakeupThread((s32)threadId);
}

extern int SleepThread(void);

/* Clear any pending wakeup before arming a 16-bit delay for this thread. */
void sdfSleepWithAlarm(u32 delay) {
    s32 threadId = GetThreadId();
    CancelWakeupThread(threadId);
    SetAlarm(delay & 0xFFFF, sdfWakeAlarmThread, (void *)threadId);
    SleepThread();
}

/* Read EE timer 0's free-running count register. */
u32 sdfReadTimerCount(void) {
    return HW_READ(T0_COUNT);
}

/* EE timer 0 count register; subtraction is reduced modulo 2^16 so
 * wraparound does not make short elapsed intervals negative. */
u32 sdfGetElapsedTimerTicks(u32 previousTick) {
    u32 current;

    current = HW_READ(T0_COUNT);
    return (current - previousTick) & 0xFFFF;
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

/* Initialize heaps, textures, pad, graphics/resource services and the tick worker. */
void sdfInitializeRuntimeServices(u32 heapSize, u32 initialHeapSize, u32 packetBufferBytes) {
    HW_WRITE(T0_MODE, 0x83);
    HW_WRITE(T0_COUNT, 0);
    sdfInitChipHeap(initialHeapSize);
    sdfInitGeneralHeap(heapSize);
    sdfTexInitializeLists();
    sdfTexInitializeSemaphore();
    sdfRegisterTextureReleaseRequestHandler();
    sdfPadInit();
    sdfStartQueuedThreadWakeWorker();
    sdfInitializeGraphicsWorkers(packetBufferBytes);
    sdfInitializeResourceQueuesAndTextureWords(0x40);
    sdfConsUploadDmaProgram(D_003BD2D0);
    effMiscSeedRandomFromClock(NULL);
    sdfStartTrackedThread(&D_003BD990, sdfRunTickWorkerThread, D_003DFEF0, 0x2000, 0x4C, 0);
}

/* Register the thread under the list semaphore before starting it. */
void sdfStartTrackedThread(SdfThreadNode *node, void (*entry)(void), void *stack, s64 stackSize, s32 priority, s32 threadArgument) {
    node->threadId = sdfCreateThread((void *)entry, stack, (s32)stackSize, priority);
    WaitSema(sdfTrackedThreadSemaphore);
    node->next = sdfTrackedThreadHead;
    sdfTrackedThreadHead = node;
    SignalSema(sdfTrackedThreadSemaphore);
    _StartThread(node->threadId, threadArgument);
}

/* A previously queued wakeup counts toward the requested sleep count. */
void sdfSleepThreadCount(s32 requestedSleepCount) {
    s32 threadId;
    s32 cancelledWakeup;

    threadId = GetThreadId();
    cancelledWakeup = CancelWakeupThread(threadId);
    requestedSleepCount = requestedSleepCount - cancelledWakeup;
    do {
        requestedSleepCount = requestedSleepCount - 1;
        SleepThread();
    } while (0 < requestedSleepCount);
}

extern void ExitDeleteThread(void);
extern s32 TerminateThread(s32 threadId);
extern s32 DeleteThread(s32 threadId);

/* Unlink the tracked thread, then delete self or terminate/delete another thread. */
void sdfDeleteTrackedThread(SdfThreadNode *node) {
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
