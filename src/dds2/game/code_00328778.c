#include "common.h"
#include "sdf.h"

extern s32 sdfCreateThread(void *entryAddress, void *workspace, s32 stackBytes, s32 priority);
extern s32 WaitSema(s32);
extern s32 SignalSema(s32);
extern void _StartThread(s32, s32);

extern s32 CancelWakeupThread(s32 threadId);

extern s32 GetThreadId(void);

extern s32 SetAlarm(u16 delay, void (*callback)(s32 alarmId, u16 time, void *common), void *common);

extern u8 D_00438A8C;

extern void (*sdfTickCallback)(void);

extern void sdfSleepThreadCount(s32);

extern void sdfPadUpdatePorts(void);

extern void sdfPadBuildButtonStates(void);

extern void sdfTickThreadPriorityOverride(void);

extern void func_00328FA8(u32 heapSize);
extern void func_0032A440(u32 resourceAddress);
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
extern s32 D_004389C0;
extern SdfThreadNode D_004390F0;
extern u8 D_0045C8A0[0x2000];

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

/* Read EE timer 0's free-running count register. */
u32 func_003287E0(void) {
    return *(volatile u32 *)0x10000000;
}

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
        if (sdfTickCallback != NULL) {
            sdfTickCallback();
        }
    }
}

void func_00328858(u32 heapSize, u32 initialHeapSize, u32 resourceAddress) {
    *(volatile u32 *)0x10000010 = 0x83;
    *(volatile u32 *)0x10000000 = 0;
    func_00328FA8(initialHeapSize);
    sdfInitGeneralHeap(heapSize);
    sdfTexInitializeLists();
    sdfTexInitializeSemaphore();
    sdfRegisterTextureReleaseRequestHandler();
    sdfPadInit();
    sdfStartQueuedThreadWakeWorker();
    func_0032A440(resourceAddress);
    sdfInitializeResourceQueuesAndTextureWords(0x40);
    sdfConsUploadDmaProgram(D_004389C0);
    effMiscSeedRandomFromClock(NULL);
    sdfStartTrackedThread(&D_004390F0, (s32)sdfRunTickWorkerThread, (s32)&D_0045C8A0, 0x2000, 0x4C, 0);
}

/* Register the thread under the list semaphore before starting it. */
void sdfStartTrackedThread(SdfThreadNode *node, s32 entry, s32 stack, s64 stackSize, s32 priority, s32 arg) {
    node->threadId = sdfCreateThread((void *)(u32)entry, (void *)(u32)stack, (s32)stackSize, priority);
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

void func_003289C8(SdfThreadNode *node) {
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
