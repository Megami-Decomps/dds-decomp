#include "sdk_ps2.h"
#include "common.h"
#include "eeregs.h"
#include "eff_random.h"
#include "sdf_timer.h"
#include "sdf.h"
#include "sdf_alarm.h"
#include "sdf_thread.h"

extern s32 CreateThreadSimple(void (*entryAddress)(), void *workspace, s32 stackBytes, s32 priority);




extern u8 D_00438A8C;

extern void (*sdfTickCallback)(void);

extern void sdfWaitVblank(s32 count);

extern void sdfPadManager(void);

extern void sdfPadBuildButtonStates(void);

extern void sdfFileCmdThreadCheckPrio(void);

extern void sdfChipInit(u32 heapSize);
extern void sdfGrInit(u32 resourceAddress);
extern void sdfMemInit(u32 heapSize);
extern void sdfGsMemInit(void);
extern void sdfImageSetInit(void);
extern void sdfTexInit(void);
extern void sdfPadInitManager(void);
extern s32 sdfInitThreadSystem(void);
extern void sdf3DInit();
extern void sdfCoreDrawInit(s32 size);
extern void sdfSystemThreadProc(void);
extern s32 D_004389C0;
extern SdfThreadNode D_004390F0;
extern u8 D_0045C8A0[0x2000];


void sdfAlarmWakeup(s32 unused0, u16 unused1, void *threadId) {
    iWakeupThread((s32)threadId);
}


/* Clear any pending wakeup before arming a 16-bit delay for this thread. */
void sdfDelayThread(u32 delay) {
    s32 threadId = GetThreadId();
    CancelWakeupThread(threadId);
    SetAlarm(delay & 0xFFFF, sdfAlarmWakeup, (void *)threadId);
    SleepThread();
}

/* Read EE timer 0's free-running count register. */
u32 sdfGetRunTimeCountBase(void) {
    return HW_READ(T0_COUNT);
}

/* EE timer 0 count register; subtraction is reduced modulo 2^16 so
 * wraparound does not make short elapsed intervals negative. */
u32 sdfGetRunTimeCount(u32 previousTick) {
    u32 current;

    current = HW_READ(T0_COUNT);
    return (current - previousTick) & 0xFFFF;
}

void sdfSystemThreadProc(void) {
    for (;;) {
        sdfWaitVblank(1);
        sdfPadManager();
        if (!D_00438A8C) {
            sdfPadBuildButtonStates();
        }
        sdfFileCmdThreadCheckPrio();
        if (sdfTickCallback != NULL) {
            sdfTickCallback();
        }
    }
}

/* Initialize heaps, textures, pad, graphics/resource services and the tick worker. */
void dds3InitSubsystems(u32 heapSize, u32 initialHeapSize, u32 packetBufferBytes) {
    HW_WRITE(T0_MODE, 0x83);
    HW_WRITE(T0_COUNT, 0);
    sdfChipInit(initialHeapSize);
    sdfMemInit(heapSize);
    sdfGsMemInit();
    sdfImageSetInit();
    sdfTexInit();
    sdfPadInitManager();
    sdfInitThreadSystem();
    sdfGrInit(packetBufferBytes);
    sdf3DInit(0x40);
    sdfCoreDrawInit(D_004389C0);
    sdfRandomizeByTime(NULL);
    sdfStartThreadAndWait(&D_004390F0, sdfSystemThreadProc, D_0045C8A0, 0x2000, 0x4C, 0);
}

/* Register the thread under the list semaphore before starting it. */
void sdfStartThreadAndWait(SdfThreadNode *node, void (*entry)(void), void *stack, s64 stackSize, s32 priority, s32 threadArgument) {
    node->threadId = CreateThreadSimple(entry, stack, (s32)stackSize, priority);
    WaitSema(sdfThreadSema);
    node->next = sdfVblankThreadId;
    sdfVblankThreadId = node;
    SignalSema(sdfThreadSema);
    _StartThread(node->threadId, threadArgument);
}

/* A previously queued wakeup counts toward the requested sleep count. */
void sdfWaitVblank(s32 requestedSleepCount) {
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


/* Unlink the tracked thread, then delete self or terminate/delete another thread. */
void sdfDeleteTrackedThread(SdfThreadNode *node) {
    SdfThreadNode **link;
    SdfThreadNode *current;
    s32 threadId;

    WaitSema(sdfThreadSema);
    link = &sdfVblankThreadId;
    current = *link;
    while (current != node) {
        link = &current->next;
        current = current->next;
    }
    *link = node->next;
    SignalSema(sdfThreadSema);
    threadId = node->threadId;
    if (threadId == GetThreadId()) {
        ExitDeleteThread();
        return;
    }
    TerminateThread(threadId);
    DeleteThread(threadId);
}
