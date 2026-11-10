#include "fpu.h"
#include "common.h"
#include "eeregs.h"
#include "pipe.h"
#include "ps2asm.h"
#include "basic.h"
#include "motion.h"

#define sdf_2A8258 sdfFreeMemorySlotFromEitherHeap
#define sdf_2A84D8 sdfDrainPendingHandlers
#define func_002A8758 sdfDeleteTrackedThread
#define func_002A87F0 sdfFindThreadNode
#define sdf_2A8858 sdfUnlinkAndDeleteCurrentThread

/* Native 0x30-byte EE thread descriptor passed to CreateThread. */
typedef struct {
    s32 status;
    void (*entry)(void *);
    void *stack;
    s32 stackSize;
    void *gp;
    s32 priority;
    u8 unk18[0x18];
} sdfKernelThreadParam_t;

typedef struct sdfHandlerNode_t {
    struct sdfHandlerNode_t *next;
    s32 handlerKind;
    s32 channel;
    s32 handlerId;
} sdfHandlerNode_t;

struct pipeTextureFile_t;


extern dds3Basic_t *pipe_Camera;

extern s32 _gp, sdfThreadSema, sdfVblankFlag, sdfCoreDrawBuffSize;

extern u8 sdfPadManagerMode, sdfSystemThreadStack[], sdfVblankThreadStack[];

extern void (*sdfTickCallback)(void);

extern sdfThCB_t *sdfVblankThreadId;

extern sdfThCB_t sdfSystemThreadCB;

extern s32 CreateThread(sdfKernelThreadParam_t *parameter);

extern s32 AddIntcHandler2(s32 channel, s32 handler, s32 argument, s32 next);

#define SDK_SKIP_ChangeThreadPriority
#define SDK_SKIP_CreateSema
#define SDK_SKIP_CreateThread
#define SDK_SKIP_DeleteThread
#define SDK_SKIP_RemoveDmacHandler
#define SDK_SKIP_RemoveIntcHandler
#define SDK_SKIP_RemoveSbusIntcHandler
#define SDK_SKIP_SetAlarm
#define SDK_SKIP_TerminateThread
#define SDK_SKIP_WakeupThread
#include "sdk_libkernl.h"

extern s32 AddSbusIntcHandler(s32 channel, s32 handler);

extern void DisableIntc(s32 channel);

extern void RemoveIntcHandler(s32 channel, s32 handlerId);

extern void RemoveDmacHandler(s32 channel, s32 handlerId);

extern void RemoveSbusIntcHandler(s32 channel);

extern s32 SetAlarm(u16 delay, void (*callback)(s32, s32, s32), void *argument);

extern s32 WaitSema(s32 semaphore);

extern s32 SignalSema(s32 semaphore);

extern void ExitDeleteThread(void);

extern void TerminateThread(s32 threadId);

extern void DeleteThread(s32 threadId);

extern void EnableIntc(s32 channel);

extern void WakeupThread(s32 threadId);

extern s32 InitThread(void);

extern void sdfPadManager(void);

extern void sdfPadWork(void);

extern void sdfFileCmdThreadCheckPrio(void);

extern void sdfChipInit(u32 aSize);

extern void sdfMemInit(u32 aSize);

extern void sdfGsMemInit(void);

extern void sdfImageSetInit(void);

extern void sdfTexInit(void);

extern void sdfPadInitManager(void);

extern void sdfGrInit(u32 aPacketBuffSize);

extern void sdf3DInit(s32 aNestLevelLimit);

extern void sdfCoreDrawInit(s32 buffSize);

extern void sdfRandomizeByTime(void *aCB);

extern void *memset(void *a, s32 b, size_t c);

void FatalError(const char *format, ...) __attribute__((noreturn));

s32 CreateThreadSimple(void *entryAddress, void *workspace, s32 stackBytes, s32 priority);

void *sdfAllocAuto(s32 bytes);

s32 sdfFreeAuto(void *allocation);

void sdfAlarmWakeup(s32 unused0, s32 unused1, s32 threadId);

void sdfWaitVblank(s32 count);


s32 sdfInitThreadSystem(void);

s32 VblankIntcHandler(s32 cause);





typedef struct sdfSemaParam_t {
    s32 count;
    s32 maxCount;
    s32 initCount;
    s32 waitThreads;
    u32 attr;
    u32 option;
} sdfSemaParam_t;

typedef struct tagpipeParts {
    s32 offset;
    u8 id;
    u8 unk05;
    u8 mdl;
    s8 resv[5];
} tagpipeParts;

extern tagpipeParts *PartsWork;

extern u16 myStatus;

extern u8 camMode, CameraAction, offsetStep, offsetStepCount;

extern s32 D_00551738, D_0055173C, D_00551744, D_00551748;

extern void *pipeObjList, *pipeObjTail;

extern u8 attackCount;

typedef struct PIPE_EFF_t {
    sdfMemHandle_t *mhFile;
    void *ptr;
    s32 size;
    void *d3p;
    f32 x;
    f32 y;
    f32 z;
    f32 pos2[2];
    s32 offset;
    s32 count;
} PIPE_EFF_t;

extern s32 SectionIndex, mapIndex, mapPrev;

extern u8 debugMode, pipeInitDone;

extern s32 offsetCount;

extern tagpipeBasic *basicMy;

extern tagpipeBasic *PipeLine, *PipeCur, *PipeCur2, *PipeRBOX, *PipeEMS, *PipeEML;

extern tagpipeBasic *PipeMaka1, *PipeZMaka, *PipeDMaka, *PipeDRBOX;

extern PIPE_EFF_t *pipe_CELL01_D3P, *pipe_BREAK01_D3P, *pipe_BREAK02_D3P;

extern PIPE_EFF_t *pipe_DAMAGE01_D3P, *pipe_DAMAGE03_D3P, *pipe_ATTACK01_D3P;


extern s32 sdfChipCheck(void *aPtr);

extern void sdfChipFreeDirect(void *aPtr);

extern void sdfChipFreeDelayedDirect(void *aPtr);

extern void sdfMemFreePtrDirect(void *aPtr);

extern void *sdfMemGetHandleByPtr(void *aPtr);

extern tagPipeObject *ObjWork;

extern tagPipeTexture *MakaTex;

extern s32 ZmakaCount;

extern void *hWorkArea;

extern sdfHandlerNode_t *sdfDmacHandlerTable;

extern void *slLoadProcId;

extern s32 sdfMainThreadId;

extern char D_00551700[];

extern char D_00551808[];

extern s32 dds3ConfigValue[2];

extern s32 playerDAMAGE;

extern s32 myMaka;

extern s32 pipeMyMakaCount;

extern s8 rBoxNum;

extern u8 sdfCdClock;

extern s32 sdfVblankSema;

extern s8 slLoadKillFlag;

extern s32 pipeBulrLimitValue;

extern void *pipeMessData;

extern void *makaWork;

extern s32 CreateSema(sdfSemaParam_t *param);

extern s32 iWakeupThread(s32 thid);

extern void sdfMemFreeDelayedDirect(void *aHandle);

extern sdfThCB_t *func_002A87F0(s32 threadId);

extern s32 GetThreadId(void);

extern void ChangeThreadPriority(s32 thid, s32 prio);

extern s32 SleepThread(void);

extern void sdfIpuRemoveHandler(sdfHandlerNode_t *handler);

extern void func_002A8758(sdfThCB_t *node);

extern void *sdfMemAllocDirect(s32 aSize);

extern void *sdfMemLock(void *aHandle);

extern void *sdfChipAllocDirect(s32 aSize);

extern s32 CancelWakeupThread(s32 thid);


void sdfDummyFunc(void) {
}

s32 sdfCreateSema(s32 initCount, s32 maxCount, u32 option)
{
    sdfSemaParam_t param;

    param.initCount = initCount;
    param.maxCount = maxCount;
    param.option = option;
    return CreateSema(&param);
}

s32 CreateThreadSimple(void *entryAddress, void *workspace, s32 stackBytes, s32 priority) {
    sdfKernelThreadParam_t param;

    memset(&param, 0, sizeof(param));
    param.entry = entryAddress;
    param.stack = workspace;
    param.stackSize = stackBytes;
    param.priority = priority;
    param.gp = &_gp;
    return CreateThread(&param);
}

s32 sdfCreateThreadWithStack(void (*entryAddress)(), s32 stackBytes, s32 priority) {
    void *workspace;

    workspace = sdfAllocAuto(stackBytes);
    return CreateThreadSimple((void *)entryAddress, workspace, stackBytes, priority);
}

void *sdfAllocAuto(s32 size)
{
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

s32 sdfFreeAuto(void *allocation)
{
    if (allocation != NULL) {
        if (sdfChipCheck(allocation)) {
            sdfChipFreeDirect(allocation);
        } else {
            sdfMemFreePtrDirect(allocation);
        }
    }
}

s32 sdfFreeAutoDelayed(void *allocation)
{
    if (allocation != NULL) {
        if (sdfChipCheck(allocation)) {
            sdfChipFreeDelayedDirect(allocation);
        } else {
            sdfMemFreeDelayedDirect(sdfMemGetHandleByPtr(allocation));
        }
    }
}

s32 sdf_2A8258(void **slot)
{
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

sdfHandlerNode_t *DmacAddHandler(s32 kind, s32 channel, saddr_t handler, s32 arg, s32 next) {
    sdfHandlerNode_t *node;
    s32 id;

    switch (kind) {
    case 0:
        id = AddIntcHandler2(channel, handler, arg, next);
        break;
    case 1:
        id = AddDmacHandler2(channel, (s32 (*)(s32, void *, void *))handler, arg, EE_ADDR_AS(void *, next));
        break;
    case 2:
        id = AddSbusIntcHandler(channel, handler);
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

void sdfIpuRemoveHandler(sdfHandlerNode_t *target) {
    sdfHandlerNode_t **link = &sdfDmacHandlerTable;
    sdfHandlerNode_t *node = *link;

    while (node != NULL) {
        if (node == target) {
            *link = node->next;
            switch (node->handlerKind) {
            case 0:
                DisableIntc(node->channel);
                RemoveIntcHandler(node->channel, node->handlerId);
                break;
            case 1:
                RemoveDmacHandler(node->channel, node->handlerId);
                break;
            case 2:
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

void sdf_2A84D8(void)
{
    void *handler;

    while ((handler = sdfDmacHandlerTable) != NULL) {
        sdfIpuRemoveHandler(handler);
    }
}

void sdfAlarmWakeup(s32 arg0, s32 arg1, s32 thid)
{
    iWakeupThread(thid);
}

void sdfDelayThread(u32 delay) {
    s32 threadId = GetThreadId();
    CancelWakeupThread(threadId);
    SetAlarm(delay & 0xFFFF, sdfAlarmWakeup, EE_ADDR_AS(void *, threadId));
    SleepThread();
}

s32 sdfGetRunTimeCountBase(void)
{
    return HW_READ(T0_COUNT);
}

s32 sdfGetRunTimeCount(s32 aBaseTime)
{
    return (HW_READ(T0_COUNT) - aBaseTime) & 0xFFFF;
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
    sdfCoreDrawInit(sdfCoreDrawBuffSize);
    sdfRandomizeByTime(NULL);
    sdfStartThreadAndWait(&sdfSystemThreadCB, sdfSystemThreadProc, sdfSystemThreadStack, 0x2000, 0x4C, 0);
}

void sdfStartThreadAndWait(sdfThCB_t *node, void (*entry)(void), void *stack, s64 stackSize, s32 priority, s32 threadArgument) {
    node->thId = CreateThreadSimple((void *)entry, stack, (s32)stackSize, priority);
    WaitSema(sdfThreadSema);
    node->next = sdfVblankThreadId;
    sdfVblankThreadId = node;
    SignalSema(sdfThreadSema);
    _StartThread(node->thId, EE_ADDR_AS(void *, threadArgument));
}

void sdfWaitVblank(s32 count)
{
    s32 rest = count - CancelWakeupThread(GetThreadId());

    do {
        SleepThread();
        rest--;
    } while (rest > 0);
}

void func_002A8758(sdfThCB_t *node) {
    sdfThCB_t **link;
    sdfThCB_t *current;
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
    threadId = node->thId;
    if (threadId == GetThreadId()) {
        ExitDeleteThread();
        return;
    }
    TerminateThread(threadId);
    DeleteThread(threadId);
}

s32 sdfThreadSema __attribute__((section(".sbss")));
sdfThCB_t *sdfVblankThreadId __attribute__((section(".sbss")));

sdfThCB_t *func_002A87F0(s32 threadId) {
    sdfThCB_t *node;

    if (threadId < 0) {
        threadId = GetThreadId();
    }
    WaitSema(sdfThreadSema);
    node = sdfVblankThreadId;
    while (node != NULL) {
        if (node->thId == threadId) {
            break;
        }
        node = node->next;
    }
    SignalSema(sdfThreadSema);
    return node;
}

void sdf_2A8858(void)
{
    func_002A8758(func_002A87F0(-1));
}

s32 VblankIntcHandler(s32 cause)
{
    u8 clock;

    if (cause == 2) {
        iWakeupThread(sdfVblankSema);
        clock = sdfCdClock;
        if (clock != 0) {
            sdfCdClock = clock - 1;
        }
    }
    return 0;
}

void VblankThreadProc(void) {
    sdfThCB_t *threadEntry;

    DmacAddHandler(0, 2, EE_ADDR_S(VblankIntcHandler), -1, 0);
    EnableIntc(2);
    for (;;) {
        SleepThread();
        sdfVblankFlag++;
        WaitSema(sdfThreadSema);
        for (threadEntry = sdfVblankThreadId; threadEntry != NULL; threadEntry = threadEntry->next) {
            WakeupThread(threadEntry->thId);
        }
        SignalSema(sdfThreadSema);
    }
}

s32 sdfInitThreadSystem(void) {
    s32 threadId;

    InitThread();
    sdfVblankFlag = 0;
    sdfVblankThreadId = NULL;
    sdfThreadSema = sdfCreateSema(1, 1, 0);
    threadId = CreateThreadSimple((void *)VblankThreadProc, sdfVblankThreadStack, 0x800, 0x3E);
    sdfVblankSema = threadId;
    return _StartThread(threadId, 0);
}

void sdfSetMainThreadPriority(void)
{
    s32 thid = GetThreadId();

    sdfMainThreadId = thid;
    ChangeThreadPriority(thid, 0x7C);
    SleepThread();
}

INCLUDE_SDATA(const s32, "sdf/sdfSystem", sdfVblankFlag);

