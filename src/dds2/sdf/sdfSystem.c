#include "sdk_ps2.h"
#include "sdf_sprite_record.h"
#include "common.h"
#include "sdf_semaphore_api.h"
#include "sdf_kernel_thread_param.h"
#include "sdf_thread_workspace_api.h"
#include "sdf_mat4.h"
#include "dds_nested_resource.h"
#include "mnu_callback_list.h"
#include "sdf_heap_api.h"
#include "sdf_chip.h"
#include "sdf_chip_range_api.h"
#include "sdf_handler_node.h"
#include "sdf_mem_free_pointer_api.h"
#include "eeregs.h"
#include "eff_random.h"
#include "sdf_timer.h"
#include "sdf.h"
#include "sdf_alarm.h"
#include "sdf_thread.h"
#include "ee_mmi.h"
#include "sdf_chip_page_link_view.h"
#include "sdf_thread_sleep.h"

extern s32 sdfVblankFlag;

extern s32 CreateThreadSimple(void (*entryAddress)(), void *workspace, s32 stackBytes, s32 priority);

extern void (*sdfTickCallback)(void);

void sdfDummyFunc(u32 callbackArg) {
}

s32 sdfCreateSema(u32 initial, u32 option, u32 maximum) {
    struct {
        u32 attr;
        u32 option;
        u32 initial;
        u32 reserved[2];
        u32 maximum;
    } sema;

    sema.initial = initial;
    sema.option = option;
    sema.maximum = maximum;
    return CreateSema(&sema);
}

extern void *_gp;

/* Create a kernel thread from a zeroed descriptor that uses the caller's
 * workspace as its stack and the program's GP. */
s32 CreateThreadSimple(void (*entryAddress)(), void *workspace, s32 stackBytes, s32 priority) {
    SdfKernelThreadParam param;

    memset(&param, 0, sizeof(param));
    param.entry = (void (*)(void))entryAddress;
    param.stack = workspace;
    param.stackSize = stackBytes;
    param.priority = priority;
    param.gp = &_gp;
    return CreateThread(&param);
}

s32 sdfCreateThreadWithStack(void (*entryAddress)(), s32 stackBytes, s32 priority) {
    void *workspace;

    workspace = sdfAllocAuto(stackBytes);
    return CreateThreadSimple(entryAddress, workspace, stackBytes, priority);
}

extern SdfHandlerNode *sdfDmacHandlerTable;

extern s32 DisableIntc(s32);

void *sdfAllocAuto(s32 size) {
    void *p;

    if (size > 0x400) {
        p = sdfMemLock(sdfMemAllocDirect(size));
        size = 0;
    } else {
        p = sdfChipAllocDirect(size);
        size = 0;
    }
    return p;
}

s32 sdfFreeAuto(void *allocation) {
    if (allocation != NULL) {
        if (sdfChipCheck(allocation)) {
            sdfChipFreeDirect(allocation);
        } else {
            sdfMemFreePtrDirect(allocation);
        }
    }
}

s32 sdfFreeAutoDelayed(void *allocation) {
    if (allocation != NULL) {
        if (sdfChipCheck(allocation)) {
            sdfChipFreeDelayedDirect(allocation);
        } else {
            sdfMemFreeDelayedDirect(sdfMemGetHandleByPtr(allocation));
        }
    }
}

s32 sdfFreeMemorySlotFromEitherHeap(void **slot) {
    void *allocation = *slot;

    if (allocation != NULL) {
        *slot = NULL;
        if (sdfChipCheck(allocation)) {
            sdfChipFreeDirect(allocation);
        } else {
            sdfMemFreePtrDirect(allocation);
        }
    }
}

void FatalError(const char *format, ...) {
    for (;;) {
    }
}

extern void FatalError(const char *format, ...) __attribute__((noreturn));

extern s32 AddIntcHandler2(s32, s32, s32, s32);

extern s32 AddDmacHandler2(s32, s32, s32, s32);

SdfHandlerNode *DmacAddHandler(SdfHandlerKind kind, s32 channel, SdfHandlerCallback handler, s32 arg, s32 next) {
    SdfHandlerNode *node;
    s32 id;

    switch ((s32)kind) {
    case SDF_HANDLER_KIND_INTERRUPT:
        id = AddIntcHandler2(channel, (s32)handler, arg, next);
        break;
    case SDF_HANDLER_KIND_DMA:
        id = AddDmacHandler2(channel, (s32)handler, arg, next);
        break;
    case SDF_HANDLER_KIND_SBUS:
        id = AddSbusIntcHandler(channel, (s32)handler);
        break;
    default:
        FatalError("sdfAddHandler : unknown type %d\n", kind);
        break;
    }
    node = sdfAllocAuto(sizeof(*node));
    node->handlerKind = kind;
    node->channel = channel;
    node->handlerId = id;
    node->next = sdfDmacHandlerTable;
    sdfDmacHandlerTable = node;
    return node;
}

void sdfIpuRemoveHandler(SdfHandlerNode *target) {
    SdfHandlerNode **link = &sdfDmacHandlerTable;
    SdfHandlerNode *node = *link;

    while (node != NULL) {
        if (node == target) {
            *link = node->next;
            switch ((s32)node->handlerKind) {
            case SDF_HANDLER_KIND_INTERRUPT:
                DisableIntc(node->channel);
                RemoveIntcHandler(node->channel, node->handlerId);
                break;
            case SDF_HANDLER_KIND_DMA:
                RemoveDmacHandler(node->channel, node->handlerId);
                break;
            case SDF_HANDLER_KIND_SBUS:
                RemoveSbusIntcHandler(node->channel);
                break;
            }
            sdfFreeAuto(target);
            return;
        }
        /* Non-head handles are followed through their own link. */
        link = &target->next;
        node = *link;
    }
}

void sdfDrainPendingHandlers(void) {
    SdfHandlerNode *current;
    while ((current = sdfDmacHandlerTable) != 0) {
        sdfIpuRemoveHandler(current);
    }
}

extern u8 sdfPadManagerMode;

extern void sdfWaitVblank(s32 count);

extern void sdfPadManager(void);

extern void sdfPadWork(void);

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
        if (!sdfPadManagerMode) {
            sdfPadWork();
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

s32 sdfThreadSema __attribute__((section(".sbss")));

SdfThreadNode *sdfVblankThreadId __attribute__((section(".sbss")));

/* A negative ID means the calling thread; search under the list semaphore. */
SdfThreadNode *sdfFindThreadNode(s32 threadId) {
    s32 requestedId;
    SdfThreadNode *node;

    requestedId = threadId;
    if (requestedId < 0) {
        requestedId = GetThreadId();
    }
    WaitSema(sdfThreadSema);
    node = sdfVblankThreadId;
    while (node != NULL) {
        if (node->threadId == requestedId) {
            break;
        }
        node = node->next;
    }
    SignalSema(sdfThreadSema);
    return node;
}

#define SDF_CURRENT_THREAD_SELECTOR 0xffffffffffffffff

#define SDF_THREAD_COMPLETION_EVENT 2

#define SDF_WAKE_WORKER_STACK_BYTES 0x800

#define SDF_WAKE_WORKER_PRIORITY 0x3E

#define SDF_SLEEP_THREAD_PRIORITY 0x7C

extern void sdfDeleteTrackedThread(SdfThreadNode *node);

/* The negative lookup selector selects the calling thread's tracked entry. */
void sdfUnlinkAndDeleteCurrentThread(void) {
    SdfThreadNode *threadNode;

    threadNode = sdfFindThreadNode(SDF_CURRENT_THREAD_SELECTOR);
    sdfDeleteTrackedThread(threadNode);
}

extern s32 sdfVblankSema;

extern u8 sdfCdClock;

/* Wake the worker only for its registered event; the byte counter never underflows. */
s32 VblankIntcHandler(s32 eventId) {
    if (eventId == SDF_THREAD_COMPLETION_EVENT) {
        iWakeupThread(sdfVblankSema);
        if (sdfCdClock != 0) {
            sdfCdClock--;
        }
    }
    return 0;
}

extern SdfHandlerNode *DmacAddHandler(SdfHandlerKind, s32, SdfHandlerCallback, s32, s32);

/* Each wake-up broadcasts to every tracked thread under the list semaphore. */
void VblankThreadProc(void) {
    SdfThreadNode *threadEntry;

    DmacAddHandler(SDF_HANDLER_KIND_INTERRUPT, SDF_THREAD_COMPLETION_EVENT, VblankIntcHandler, -1, 0);
    EnableIntc(SDF_THREAD_COMPLETION_EVENT);
    for (;;) {
        SleepThread();
        sdfVblankFlag++;
        WaitSema(sdfThreadSema);
        for (threadEntry = sdfVblankThreadId; threadEntry != NULL; threadEntry = threadEntry->next) {
            WakeupThread(threadEntry->threadId);
        }
        SignalSema(sdfThreadSema);
    }
}

extern void InitThread(void);

extern char D_0045E8A0[];

/* Initialize the tracked-thread list and its semaphore before starting the wake worker. */
s32 sdfInitThreadSystem(void) {
    s32 threadId;

    InitThread();
    sdfVblankFlag = 0;
    sdfVblankThreadId = NULL;
    sdfThreadSema = sdfCreateSema(1, 1, 0);
    threadId = CreateThreadSimple(VblankThreadProc, D_0045E8A0, SDF_WAKE_WORKER_STACK_BYTES, SDF_WAKE_WORKER_PRIORITY);
    sdfVblankSema = threadId;
    return _StartThread(threadId, 0);
}

extern s32 sdfSleepThreadId;

/* Record the calling thread, set its sleep priority, then return the SDK sleep result. */
s32 sdfSetMainThreadPriority(void) {
    s32 threadId = GetThreadId();

    sdfSleepThreadId = threadId;
    ChangeThreadPriority(threadId, SDF_SLEEP_THREAD_PRIORITY);
    return SleepThread();
}

INCLUDE_SDATA(const s32, "sdf/sdfSystem", sdfVblankFlag);

