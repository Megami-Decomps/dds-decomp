#include "common.h"
#include "newlib_stat.h"
#include "sdk_libgcc.h"
#include "ps2asm.h"
#include "sif_rpc.h"
#include "ps2asm.h"

/* libkernl's first object, klib.o, has .text aligned to 64, so the retail link
 * pads 16 bytes after libc's last function to reach it. */
TEXT_ALIGN(6);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU000_FullReset);

INCLUDE_ASM(const s32, "sdk/libkernl", ResetEE);

INCLUDE_ASM(const s32, "sdk/libkernl", SetGsCrt);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU003);

INCLUDE_ASM(const s32, "sdk/libkernl", libc_2DBAC0);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU005);

INCLUDE_ASM(const s32, "sdk/libkernl", libc_2DBAE0);

INCLUDE_ASM(const s32, "sdk/libkernl", _ExecPS2);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU008);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU009);

INCLUDE_ASM(const s32, "sdk/libkernl", AddSbusIntcHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", RemoveSbusIntcHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", Interrupt2Iop);

INCLUDE_ASM(const s32, "sdk/libkernl", SetVTLBRefillHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", SetVCommonHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", SetVInterruptHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", AddIntcHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", AddIntcHandler2);

INCLUDE_ASM(const s32, "sdk/libkernl", RemoveIntcHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", AddDmacHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", AddDmacHandler2);

INCLUDE_ASM(const s32, "sdk/libkernl", RemoveDmacHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", _EnableIntc);

INCLUDE_ASM(const s32, "sdk/libkernl", _DisableIntc);

INCLUDE_ASM(const s32, "sdk/libkernl", _EnableDmac);

INCLUDE_ASM(const s32, "sdk/libkernl", _DisableDmac);

INCLUDE_ASM(const s32, "sdk/libkernl", SetAlarm);

INCLUDE_ASM(const s32, "sdk/libkernl", ReleaseAlarm);

INCLUDE_ASM(const s32, "sdk/libkernl", _iEnableIntc);

INCLUDE_ASM(const s32, "sdk/libkernl", _iDisableIntc);

INCLUDE_ASM(const s32, "sdk/libkernl", _iEnableDmac);

INCLUDE_ASM(const s32, "sdk/libkernl", _iDisableDmac);

INCLUDE_ASM(const s32, "sdk/libkernl", iSetAlarm);

INCLUDE_ASM(const s32, "sdk/libkernl", iReleaseAlarm);

INCLUDE_ASM(const s32, "sdk/libkernl", CreateThread);

INCLUDE_ASM(const s32, "sdk/libkernl", DeleteThread);

INCLUDE_ASM(const s32, "sdk/libkernl", _StartThread);

INCLUDE_ASM(const s32, "sdk/libkernl", ExitThread);

INCLUDE_ASM(const s32, "sdk/libkernl", ExitDeleteThread);

INCLUDE_ASM(const s32, "sdk/libkernl", TerminateThread);

INCLUDE_ASM(const s32, "sdk/libkernl", iTerminateThread);

INCLUDE_ASM(const s32, "sdk/libkernl", DisableDispatchThread);

INCLUDE_ASM(const s32, "sdk/libkernl", EnableDispatchThread);

INCLUDE_ASM(const s32, "sdk/libkernl", ChangeThreadPriority);

INCLUDE_ASM(const s32, "sdk/libkernl", iChangeThreadPriority);

INCLUDE_ASM(const s32, "sdk/libkernl", RotateThreadReadyQueue);

INCLUDE_ASM(const s32, "sdk/libkernl", _iRotateThreadReadyQueue);

INCLUDE_ASM(const s32, "sdk/libkernl", ReleaseWaitThread);

INCLUDE_ASM(const s32, "sdk/libkernl", iReleaseWaitThread);

INCLUDE_ASM(const s32, "sdk/libkernl", GetThreadId);

INCLUDE_ASM(const s32, "sdk/libkernl", ReferThreadStatus);

INCLUDE_ASM(const s32, "sdk/libkernl", iReferThreadStatus);

INCLUDE_ASM(const s32, "sdk/libkernl", SleepThread);

INCLUDE_ASM(const s32, "sdk/libkernl", WakeupThread);

INCLUDE_ASM(const s32, "sdk/libkernl", _iWakeupThread);

INCLUDE_ASM(const s32, "sdk/libkernl", CancelWakeupThread);

INCLUDE_ASM(const s32, "sdk/libkernl", iCancelWakeupThread);

INCLUDE_ASM(const s32, "sdk/libkernl", SuspendThread);

INCLUDE_ASM(const s32, "sdk/libkernl", _iSuspendThread);

INCLUDE_ASM(const s32, "sdk/libkernl", ResumeThread);

INCLUDE_ASM(const s32, "sdk/libkernl", iResumeThread);

INCLUDE_ASM(const s32, "sdk/libkernl", JoinThread);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU060);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU061);

INCLUDE_ASM(const s32, "sdk/libkernl", EndOfHeap);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU063);

INCLUDE_ASM(const s32, "sdk/libkernl", CreateSema);

INCLUDE_ASM(const s32, "sdk/libkernl", DeleteSema);

INCLUDE_ASM(const s32, "sdk/libkernl", SignalSema);

INCLUDE_ASM(const s32, "sdk/libkernl", iSignalSema);

INCLUDE_ASM(const s32, "sdk/libkernl", WaitSema);

INCLUDE_ASM(const s32, "sdk/libkernl", PollSema);

INCLUDE_ASM(const s32, "sdk/libkernl", iPollSema);

INCLUDE_ASM(const s32, "sdk/libkernl", ReferSemaStatus);

INCLUDE_ASM(const s32, "sdk/libkernl", iReferSemaStatus);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU073);

INCLUDE_ASM(const s32, "sdk/libkernl", SetOsdConfigParam);

INCLUDE_ASM(const s32, "sdk/libkernl", GetOsdConfigParam);

INCLUDE_ASM(const s32, "sdk/libkernl", GetGsHParam);

INCLUDE_ASM(const s32, "sdk/libkernl", GetGsVParam);

INCLUDE_ASM(const s32, "sdk/libkernl", SetGsHParam);

INCLUDE_ASM(const s32, "sdk/libkernl", SetGsVParam);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU080_CreateEventFlag);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU081_DeleteEventFlag);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU082_SetEventFlag);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU083_iSetEventFlag);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU084_ClearEventFlag);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU085_iClearEventFlag);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU086_WaitEvnetFlag);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU087_PollEvnetFlag);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU088_iPollEvnetFlag);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU089_ReferEventFlagStatus);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU090_iReferEventFlagStatus);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU091);

INCLUDE_ASM(const s32, "sdk/libkernl", EnableIntcHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", iEnableIntcHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", DisableIntcHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", iDisableIntcHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", EnableDmacHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", iEnableDmacHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", DisableDmacHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", iDisableDmacHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", KSeg0);

INCLUDE_ASM(const s32, "sdk/libkernl", EnableCache);

INCLUDE_ASM(const s32, "sdk/libkernl", DisableCache);

INCLUDE_ASM(const s32, "sdk/libkernl", GetCop0);

INCLUDE_ASM(const s32, "sdk/libkernl", FlushCache);

INCLUDE_ASM(const s32, "sdk/libkernl", CpuConfig);

INCLUDE_ASM(const s32, "sdk/libkernl", iGetCop0);

INCLUDE_ASM(const s32, "sdk/libkernl", iFlushCache);

INCLUDE_ASM(const s32, "sdk/libkernl", iCpuConfig);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifStopDma);

INCLUDE_ASM(const s32, "sdk/libkernl", SetCPUTimerHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", SetCPUTimer);

INCLUDE_ASM(const s32, "sdk/libkernl", SetOsdConfigParam2);

INCLUDE_ASM(const s32, "sdk/libkernl", GetOsdConfigParam2);

INCLUDE_ASM(const s32, "sdk/libkernl", GsGetIMR);

INCLUDE_ASM(const s32, "sdk/libkernl", iGsGetIMR);

INCLUDE_ASM(const s32, "sdk/libkernl", GsPutIMR);

INCLUDE_ASM(const s32, "sdk/libkernl", iGsPutIMR);

INCLUDE_ASM(const s32, "sdk/libkernl", SetPgifHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", SetVSyncFlag);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU116);

INCLUDE_ASM(const s32, "sdk/libkernl", _print);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifDmaStat);

INCLUDE_ASM(const s32, "sdk/libkernl", isceSifDmaStat);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifSetDma);

INCLUDE_ASM(const s32, "sdk/libkernl", isceSifSetDma);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifSetDChain);

INCLUDE_ASM(const s32, "sdk/libkernl", isceSifSetDChain);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifSetReg);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifGetReg);

INCLUDE_ASM(const s32, "sdk/libkernl", _ExecOSD);

INCLUDE_ASM(const s32, "sdk/libkernl", Deci2Call);

INCLUDE_ASM(const s32, "sdk/libkernl", PSMode);

INCLUDE_ASM(const s32, "sdk/libkernl", MachineType);

INCLUDE_ASM(const s32, "sdk/libkernl", GetMemorySize);

INCLUDE_ASM(const s32, "sdk/libkernl", _InitTLB);

extern s32 ttyInitDone __attribute__((section(".data")));

void sceResetttyinit(void)
{
    ttyInitDone = 0;
}

extern s32 DIntr(void);
extern s32 EIntr(void);

void VSync(void)
{
    s32 enabled;

    enabled = DIntr();
    *(volatile u32 *)0x1000F000 = 4;
    EE_SYNC();
    if (enabled != 0) {
        EIntr();
    }
    while ((*(volatile u32 *)0x1000F000 & 4) == 0) {
    }
    enabled = DIntr();
    *(volatile u32 *)0x1000F000 = 4;
    EE_SYNC();
    if (enabled != 0) {
        EIntr();
    }
}

extern void SetVSyncFlag(volatile u32 *flag, volatile u64 *result);

u64 VSync2(void)
{
    /* Kernel-published outputs: keep the result read after interrupt restore. */
    volatile u32 flag = 0;
    volatile u64 result;
    s32 enabled;

    SetVSyncFlag(&flag, &result);
    enabled = DIntr();
    *(volatile u32 *)0x1000F000 = 4;
    EE_SYNC();
    if (enabled != 0) {
        EIntr();
    }
    while ((*(volatile u32 *)0x1000F000 & 4) == 0 && flag == 0) {
    }
    enabled = DIntr();
    *(volatile u32 *)0x1000F000 = 4;
    EE_SYNC();
    if (enabled != 0) {
        EIntr();
    }
    return result;
}

extern s32 ttyInitDone __attribute__((section(".data")));
extern s32 sceTtyInit(void);
extern s32 sceTtyRead(char *buffer, s32 length);
extern s32 sceTtyWrite(const char *buffer, s32 length);

s32 write(s32 descriptor, const void *buffer, s32 length)
{
    if ((u32)descriptor - 1 < 2) {
        if (ttyInitDone == 0) {
            if (sceTtyInit() == 0) {
                return -1;
            }
            ttyInitDone = 1;
        }
        return sceTtyWrite(buffer, length);
    }
    return -1;
}

s32 read(s32 descriptor, void *buffer, s32 length)
{
    if (descriptor == 0) {
        if (ttyInitDone == 0) {
            if (sceTtyInit() == 0) {
                return -1;
            }
            ttyInitDone = 1;
        }
        return sceTtyRead(buffer, length);
    }
    return -1;
}

extern s32 *__errno_8(void);

s32 open(const char *path, s32 flags, s32 mode)
{
    *__errno_8() = 5;
    return -1;
}

s32 close(s32 descriptor)
{
    return -1;
}

/* Native reads no input registers; parameter types remain unrecovered. */
s32 ioctl()
{
    return -1;
}

long lseek(s32 descriptor, s32 offset, s32 whence)
{
    return -1;
}

INCLUDE_ASM(const s32, "sdk/libkernl", sbrk);

s32 isatty(s32 descriptor)
{
    return 1;
}

s32 fstat(s32 fd, struct stat *st)
{
    st->st_mode = 0x2000;
    st->st_blksize = 0;
    return 0;
}

s32 libc_2DC670(void)
{
    return 1;
}

extern void Exit(s32 status);

s32 kill(s32 pid, s32 signal)
{
    if (pid == 1) {
        Exit(signal);
    }
    return 0;
}

s32 stat(const char *path, struct stat *result)
{
    *__errno_8() = 5;
    return -1;
}

s32 unlink(const char *path)
{
    *__errno_8() = 5;
    return -1;
}

INCLUDE_ASM(const s32, "sdk/libkernl", _sceSDC);

INCLUDE_ASM(const s32, "sdk/libkernl", SyncDCache);

extern void _sceSDC(void *start, void *end);

void iSyncDCache(void *start, void *end)
{
    _sceSDC(EE_ADDR_PTR(EE_ADDR(start) & ~63U),
             EE_ADDR_PTR(EE_ADDR(end) & ~63U));
}

INCLUDE_ASM(const s32, "sdk/libkernl", _sceIDC);

INCLUDE_ASM(const s32, "sdk/libkernl", InvalidDCache);

extern void _sceIDC(void *start, void *end);

void iInvalidDCache(void *start, void *end)
{
    _sceIDC(EE_ADDR_PTR(EE_ADDR(start) & ~63U),
             EE_ADDR_PTR(EE_ADDR(end) & ~63U));
}

INCLUDE_ASM(const s32, "sdk/libkernl", QueryIntrContext);

INCLUDE_ASM(const s32, "sdk/libkernl", DisableIntc);

INCLUDE_ASM(const s32, "sdk/libkernl", EnableIntc);

INCLUDE_ASM(const s32, "sdk/libkernl", DisableDmac);

INCLUDE_ASM(const s32, "sdk/libkernl", EnableDmac);

INCLUDE_ASM(const s32, "sdk/libkernl", iEnableIntc);

INCLUDE_ASM(const s32, "sdk/libkernl", iDisableIntc);

INCLUDE_ASM(const s32, "sdk/libkernl", iEnableDmac);

INCLUDE_ASM(const s32, "sdk/libkernl", iDisableDmac);

typedef struct threadMgrQueue_t {
    u32 readIndex;
    u32 writeIndex;
    u8 commands[512][2];
} threadMgrQueue_t;

extern s32 threadMgrSema;
extern s32 WaitSema(s32 sema);
extern s32 WakeupThread(s32 threadId);
extern s32 RotateThreadReadyQueue(s32 priority);
extern s32 SuspendThread(s32 threadId);
extern void kprintf(const char *format, ...);
extern const char kernlInternalErrorMsg[];

void topThread(void *argument)
{
    threadMgrQueue_t *queue = argument;
    u32 index;

    for (;;) {
        WaitSema(threadMgrSema);
        index = queue->readIndex & 0x1FF;
        queue->readIndex = index + 1;
        switch (queue->commands[index][0]) {
        case 0:
            WakeupThread(queue->commands[index][1]);
            break;
        case 1:
            RotateThreadReadyQueue(queue->commands[index][1]);
            break;
        case 2:
            SuspendThread(queue->commands[index][1]);
            break;
        default:
            kprintf(kernlInternalErrorMsg);
            break;
        }
    }
}

typedef struct rpcSemaParam {
    s32 count;
    s32 maxCount;
    s32 initCount;
    s32 waitThreads;
    u32 attr;
    u32 option;
} rpcSemaParam;

extern s32 CreateSema(rpcSemaParam *param);
typedef struct kernelThreadParam_t {
    s32 status;
    void (*entry)(void *);
    void *stack;
    s32 stackSize;
    void *gp;
    s32 priority;
    u8 opaque[0x18];
} kernelThreadParam_t;
extern s32 topThreadId __attribute__((section(".data")));
extern s32 threadMgrSema;
extern u8 topThreadStack[0x400];
extern u8 _gp[];
extern threadMgrQueue_t threadCommandQueue;
extern void topThread(void *queue);
extern s32 CreateThread(kernelThreadParam_t *param);
extern s32 DeleteSema(s32 sema);

extern s32 GetThreadId(void);
extern s32 ChangeThreadPriority(s32 threadId, s32 priority);

s32 InitThread(void)
{
    kernelThreadParam_t thread;
    rpcSemaParam sema;

    if (topThreadId > 0) {
        return -1;
    }
    sema.maxCount = 255;
    sema.initCount = 0;
    threadMgrSema = CreateSema(&sema);
    if (threadMgrSema < 0) {
        return -1;
    }
    thread.entry = topThread;
    thread.stack = topThreadStack;
    thread.stackSize = 0x400;
    thread.gp = _gp;
    thread.priority = 0;
    topThreadId = CreateThread(&thread);
    if (topThreadId < 0) {
        DeleteSema(threadMgrSema);
        return -1;
    }
    threadCommandQueue.readIndex = 0;
    threadCommandQueue.writeIndex = 0;
    _StartThread(topThreadId, &threadCommandQueue);
    ChangeThreadPriority(GetThreadId(), 1);
    return topThreadId;
}

INCLUDE_ASM(const s32, "sdk/libkernl", iWakeupThread);

extern s32 iSignalSema(s32 sema);

s32 iRotateThreadReadyQueue(s32 priority)
{
    u32 index;

    if ((u32)priority >= 0x80 || topThreadId == 0) {
        return -1;
    }

    index = threadCommandQueue.writeIndex & 0x1FF;
    threadCommandQueue.writeIndex = index + 1;
    threadCommandQueue.commands[index][0] = 1;
    threadCommandQueue.commands[index][1] = (u8)priority;
    iSignalSema(threadMgrSema);
    return priority;
}

INCLUDE_ASM(const s32, "sdk/libkernl", iSuspendThread);

typedef struct ttyQueue_t {
    u32 capacity;
    /* The TTY callback produces bytes while sceTtyRead polls occupancy. */
    volatile s32 count;
    u8 *read;
    u8 *write;
    u8 data[256];
} ttyQueue_t;

extern ttyQueue_t ttyQueueStorage;

ttyQueue_t *gcc2_compiled__10(u32 capacity)
{
    ttyQueue_t *queue = &ttyQueueStorage;

    queue->capacity = capacity;
    queue->count = 0;
    queue->write = queue->data;
    queue->read = queue->data;
    return queue;
}

void QueuePeekWriteDone(ttyQueue_t *queue)
{
    queue->count++;
    queue->write++;
    if (queue->write == queue->data + queue->capacity) {
        queue->write = queue->data;
    }
}

void QueuePeekReadDone(ttyQueue_t *queue)
{
    queue->count--;
    queue->read++;
    if (queue->read == queue->data + queue->capacity) {
        queue->read = queue->data;
    }
}

/* Native TTY context prefix, through its receive queue. */
typedef struct ttyState_t {
    s32 handle;
    s32 sendLength;
    s32 receiveLength;
    s32 sendBusy;
    u8 *sendBuffer;
    u8 *receiveBuffer;
    ttyQueue_t *queue;
} ttyState_t;

extern ttyState_t ttyState;

extern s32 sceDeci2ExRecv(s32 handle, void *buffer, u16 length);
extern s32 sceDeci2ExSend(s32 handle, void *buffer, u16 length);
extern void kprintf(const char *format, ...);
extern const char ttyMsgPacketTooLarge[];
extern const char ttyMsgReceiveError[];
extern const char ttyMsgSendError[];
extern const char ttyMsgWlenError[];

/* The asynchronous callback observes progress and publishes completion with
 * volatile word accesses. The writer uses the same shared-word views while
 * polling the transport. */
#define TTY_CALLBACK_WORD(state, member) (*(volatile s32 *)&(state)->member)

void sceTtyHandler(s32 event, s32 byteCount, void *argument)
{
    /* Qualified EE 2.9 global_alloc trace: this shared receive/traversal count
     * has 17 refs over 44 live instructions, ahead of the context's 25/78.
     * It takes r16 and the context takes r17; splitting the count reverses them.
     * Ordinary, dumped and hooked assembly agree; both RTL dump sets agree. */
    ttyState_t *state = argument;
    u8 *packet;

    switch (event) {
    case 1:
    case 2:
        if (byteCount != 0) {
            if ((u32)TTY_CALLBACK_WORD(state, receiveLength) + (u32)byteCount > 320) {
                kprintf(ttyMsgPacketTooLarge);
            }
            {
                s32 receiveOffset = TTY_CALLBACK_WORD(state, receiveLength);
                byteCount = sceDeci2ExRecv(state->handle,
                                          state->receiveBuffer + receiveOffset,
                                          (u16)byteCount);
            }
            if (byteCount < 0) {
                kprintf(ttyMsgReceiveError);
            }
            TTY_CALLBACK_WORD(state, receiveLength) += byteCount;
        } else {
            packet = state->receiveBuffer;
            for (byteCount = 12; byteCount < *(u16 *)packet; byteCount++) {
                *state->queue->write = state->receiveBuffer[byteCount];
                QueuePeekWriteDone(state->queue);
            }
            TTY_CALLBACK_WORD(state, receiveLength) = 0;
        }
        return;
    case 3: {
        s32 sent = sceDeci2ExSend(state->handle, state->sendBuffer,
                                (u16)TTY_CALLBACK_WORD(state, sendLength));
        if (sent < 0) {
            kprintf(ttyMsgSendError, sent);
        } else {
            state->sendBuffer += sent;
            TTY_CALLBACK_WORD(state, sendLength) -= sent;
            return;
        }
        break;
    }
    case 4:
        if (TTY_CALLBACK_WORD(state, sendLength) != 0) {
            kprintf(ttyMsgWlenError, TTY_CALLBACK_WORD(state, sendLength));
        }
        break;
    default:
        return;
    }
    TTY_CALLBACK_WORD(state, sendBusy) = 0;
}

#undef TTY_CALLBACK_WORD
/* The transport header is eight bytes; TTY adds a four-byte reserved word.
 * Both native backing buffers span 320 bytes. */
typedef struct ttyPacket_t {
    u16 length;
    u16 reserved;
    u16 protocol;
    u8 source;
    u8 destination;
    u32 ttyReserved;
    u8 payload[308];
} ttyPacket_t;

extern ttyPacket_t ttySendPacket;
extern ttyPacket_t ttyReceivePacket;
extern s32 sceDeci2ReqSend(s32 handle, s8 destination);
extern s32 sceDeci2Poll(s32 handle);

s32 sceTtyWrite(const char *buffer, s32 length)
{
    s32 consumed = 0;
    s32 outputLength = 0;
    /* Reuse the live transport status for the request and completion tests. */
    s32 status;
    ttyPacket_t *packet;
    u8 *output;

    if ((*(volatile ttyState_t *)&ttyState).sendBusy != 0) {
        return -1;
    }
    DIntr();
    packet = (ttyPacket_t *)(EE_ADDR(&ttySendPacket) | 0x20000000);
    ttyState.sendBusy = 1;
    ttyState.sendBuffer = (u8 *)packet;
    output = packet->payload;
    /* Native decrement wraps even for negative lengths; only zero stops it. */
    while ((length = (s32)((u32)length - 1)) != -1) {
        if (*buffer == '\n') {
            *output++ = '\r';
            if (++outputLength >= 256) {
                break;
            }
        }
        *output++ = *buffer++;
        outputLength++;
        consumed++;
        if (outputLength >= 256) {
            break;
        }
    }
    {
        volatile ttyState_t *state = &ttyState;

        state->sendLength = outputLength + 12;
        packet->length = state->sendLength;
        status = sceDeci2ReqSend((*(volatile ttyState_t *)&ttyState).handle,
                                 (s8)packet->destination);
        if (status < 0) {
            state->sendBusy = 0;
            EIntr();
            return -1;
        }
        while ((status = state->sendBusy) != 0) {
            sceDeci2Poll((*(volatile ttyState_t *)&ttyState).handle);
        }
        EIntr();
        return consumed;
    }
}

s32 sceTtyRead(char *buffer, s32 length)
{
    s32 count;

    for (count = 0; count < length; count++) {
        while (ttyState.queue->count == 0) {
        }
        buffer[count] = *ttyState.queue->read;
        QueuePeekReadDone(ttyState.queue);
        if (buffer[count] == 10 || buffer[count] == 13) {
            return count + 1;
        }
    }
    return count;
}

extern void FlushCache(s32 mode);
extern s32 sceDeci2Open(u16 protocol, void *argument,
                       void (*handler)(s32, s32, void *));

s32 sceTtyInit(void)
{
    ttyPacket_t *packet;

    /* Registration exposes this context to the asynchronous TTY callback.
     * Publish and observe its shared words with native-width accesses. */
    FlushCache(0);
    (*(volatile s32 *)&ttyState.handle) = sceDeci2Open(0x210, &ttyState, sceTtyHandler);
    if ((*(volatile s32 *)&ttyState.handle) < 0) {
        return 0;
    }
    (*(volatile s32 *)&ttyState.sendBusy) = 0;
    (*(volatile s32 *)&ttyState.sendLength) = 0;
    (*(volatile s32 *)&ttyState.receiveLength) = 0;
    ttyState.receiveBuffer = (u8 *)(EE_ADDR(&ttyReceivePacket) | 0x20000000);
    packet = (ttyPacket_t *)(EE_ADDR(&ttySendPacket) | 0x20000000);
    ttyState.sendBuffer = (u8 *)packet;
    packet->protocol = 0x210;
    packet->source = 'E';
    packet->destination = 'H';
    packet->reserved = 0;
    packet->ttyReserved = 0;
    ttyState.queue = gcc2_compiled__10(0x100);
    return 1;
}

s32 kputchar(s32 character)
{
    while (*(volatile u32 *)0x1000F130 & 0x8000) {
    }
    *(volatile u8 *)0x1000F180 = character;
    return character;
}

extern s32 deci2LineIndex __attribute__((section(".data")));
extern char deci2LineBuffer[128];
extern s32 kputs(const char *text);

void deci2Putchar(s32 character)
{
    s32 index;

    index = deci2LineIndex;
    if (index >= 126) {
        deci2LineIndex = 0;
        deci2LineBuffer[127] = 0;
        kputs(deci2LineBuffer);
        index = deci2LineIndex;
    }
    if (character == '\n') {
        deci2LineIndex = 0;
        deci2LineBuffer[index] = character;
        deci2LineBuffer[index + 1] = 0;
        kputs(deci2LineBuffer);
    } else {
        deci2LineIndex = index + 1;
        deci2LineBuffer[index] = character;
    }
}

extern s32 kputchar(s32 character);

void serialPutchar(s32 character)
{
    if (character == '\n') {
        kputchar('\r');
        kputchar('\n');
    } else {
        kputchar(character);
    }
}

INCLUDE_ASM(const s32, "sdk/libkernl", ftoi);

/* ftoi consumes the integer bits directly, not a numeric double conversion. */
extern s32 ftoi(u64 bits);
extern void (*printfPutchar)(s32 character) __attribute__((section(".data")));

void printfloat(f64 value)
{
    s32 exponent = 0;
    s32 digits;

    if (value < 0.0) {
        value = 0.0 - value;
        printfPutchar('-');
    }
    if (value < 0.1) {
        while (value < 0.1) {
            value *= 10.0;
            exponent--;
        }
    } else if (value >= 1.0) {
        while (value >= 1.0) {
            value /= 10.0;
            exponent++;
        }
    }
    digits = ftoi((u64)(value * 1000000.0));
    kprintf("0.%d", digits);
    if (exponent >= 0) {
        kprintf("e+%d", exponent);
    } else {
        kprintf("e%d", exponent);
    }
}

INCLUDE_ASM(const s32, "sdk/libkernl", _printf);
INCLUDE_ASM(const s32, "sdk/libkernl", func_0030C838);

/* In ee-gcc 2.9, next_arg names the first incoming stack argument. These
 * one-fixed-argument wrappers save the seven remaining argument GPRs in
 * contiguous eight-byte slots immediately before it. The formatter walks
 * that register-save area and then continues into the stack arguments. */
typedef char *kernelVaList_t;
extern void _printf(const char *format, kernelVaList_t arguments);
extern void (*printfPutchar)(s32 character) __attribute__((section(".data")));

void kprintf(const char *format, ...)
{
    kernelVaList_t arguments;

    arguments = (kernelVaList_t)__builtin_next_arg(format) - 7 * sizeof(long);
    _printf(format, arguments);
}

void scePrintf(const char *format, ...)
{
    kernelVaList_t arguments;
    void (*previous)(s32);

    previous = printfPutchar;
    printfPutchar = deci2Putchar;
    arguments = (kernelVaList_t)__builtin_next_arg(format) - 7 * sizeof(long);
    _printf(format, arguments);
    printfPutchar = previous;
}

typedef struct {
    void (*handler)(void *packet, void *argument);
    void *argument;
} sifCommandHandler_t;

/* Consumed command-context prefix; later state remains opaque. */
typedef struct {
    u8 prefix[8];
    void *remoteAddress;
    sifCommandHandler_t *systemHandlers;
    s32 systemCount;
    sifCommandHandler_t *handlers;
    s32 handlerCount;
    u32 *softwareRegisters;
} sifCommandContext_t;

extern sifCommandContext_t sifCommandContext;
extern u32 sifSregs[];

typedef struct {
    u8 header[16];
    u32 index;
    u32 value;
} sifRegisterPacket_t;

typedef struct {
    u8 header[16];
    void *address;
} sifAddressPacket_t;

void gcc2_compiled__12(void *packet, void *argument)
{
    const sifRegisterPacket_t *command = packet;
    sifCommandContext_t *context = argument;
    context->softwareRegisters[command->index] = command->value;
}

void _change_addr(void *packet, void *argument)
{
    const sifAddressPacket_t *command = packet;
    sifCommandContext_t *context = argument;
    context->remoteAddress = command->address;
}

u32 sceSifGetSreg(s32 index)
{
    return sifSregs[index];
}

u32 sceSifSetSreg(s32 index, u32 value)
{
    return sifSregs[index] = value;
}

sifCommandContext_t *sceSifGetDataTable(void)
{
    return &sifCommandContext;
}

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifInitCmd);

extern s32 DisableDmac(s32 channel);
extern s32 RemoveDmacHandler(s32 channel, s32 handlerId);
extern s32 sifDmacHandlerId;
extern s32 D_0039C460;

void sceSifExitCmd(void)
{
    DisableDmac(5);
    RemoveDmacHandler(5, sifDmacHandlerId);
    D_0039C460 = 0;
}

sifCommandHandler_t *sceSifSetCmdBuffer(sifCommandHandler_t *buffer, s32 count)
{
    sifCommandHandler_t *previous = sifCommandContext.handlers;
    sifCommandContext.handlers = buffer;
    sifCommandContext.handlerCount = count;
    return previous;
}

sifCommandHandler_t *sceSifSetSysCmdBuffer(sifCommandHandler_t *buffer, s32 count)
{
    sifCommandHandler_t *previous = sifCommandContext.systemHandlers;
    sifCommandContext.systemHandlers = buffer;
    sifCommandContext.systemCount = count;
    return previous;
}

/* The sign selects the table. Scaling in 32 bits drops that selector and
 * gives the native eight-byte entry offset. Once tested, command is reused
 * for the selected table's actual address; its old value is no longer live.
 */
void sceSifAddCmdHandler(s32 command, void (*handler)(void *, void *), void *argument)
{
    u32 address = (u32)command << 3;
    sifCommandHandler_t *entry;

    if (command < 0) {
        command = EE_ADDR_S(sifCommandContext.systemHandlers);
    } else {
        command = EE_ADDR_S(sifCommandContext.handlers);
    }
    address += (u32)command;
    entry = (sifCommandHandler_t *)address;
    entry->handler = handler;
    entry->argument = argument;
}

void sceSifRemoveCmdHandler(s32 command)
{
    u32 address = (u32)command << 3;
    sifCommandHandler_t *entry;

    if (command < 0) {
        command = EE_ADDR_S(sifCommandContext.systemHandlers);
    } else {
        command = EE_ADDR_S(sifCommandContext.handlers);
    }
    address += (u32)command;
    entry = (sifCommandHandler_t *)address;
    entry->handler = NULL;
}

/* DMA bus addresses and transfer controls occupy four 32-bit words. */
typedef struct sifDmaTransfer_t {
    u32 source;
    u32 destination;
    s32 size;
    u32 attributes;
} sifDmaTransfer_t;

SIZE_CHECK(sifDmaTransfer_size_must_be_16, sifDmaTransfer_t, 16);

#include "sdk_libkernl.h"
extern s32 isceSifSetDma(sifDmaTransfer_t *transfers, s32 count);
extern void sceSifWriteBackDCache(void *address, s32 size);

typedef struct sifCommandHeader_t {
    u32 packetSize : 8;
    u32 dataSize : 24;
    u32 destination;
    u32 command;
    u32 option;
} sifCommandHeader_t;

SIZE_CHECK(sifCommandHeader_size_must_be_16, sifCommandHeader_t, 16);

s32 _sceSifSendCmd(s32 command, s32 interruptMode, void *packet, s32 packetBytes,
                    void *source, void *destination, s32 dataBytes)
{
    sifDmaTransfer_t transfers[2];
    sifCommandHeader_t *header = packet;
    s32 count;

    if ((u32)packetBytes - 16U > 96U) {
        return 0;
    }
    count = 0;
    if (dataBytes > 0) {
        transfers[count].source = EE_ADDR(source);
        transfers[count].destination = EE_ADDR(destination);
        transfers[count].size = dataBytes;
        transfers[count].attributes = 0;
        header->dataSize = dataBytes;
        header->destination = EE_ADDR(destination);
        count++;
        if (interruptMode & 4) {
            sceSifWriteBackDCache(source, dataBytes);
        }
    } else {
        header->dataSize = 0;
        header->destination = 0;
    }
    transfers[count].source = EE_ADDR(packet);
    transfers[count].destination = EE_ADDR(sifCommandContext.remoteAddress);
    transfers[count].size = packetBytes;
    header->command = command;
    header->packetSize = packetBytes;
    transfers[count].attributes = 0x44;
    sceSifWriteBackDCache(packet, packetBytes);
    count++;
    if (interruptMode & 1) {
        return isceSifSetDma(transfers, count);
    }
    return sceSifSetDma(transfers, count);
}

s32 sceSifSendCmd(s32 command, void *packet, s32 packetBytes,
                  void *source, void *destination, s32 dataBytes)
{
    return _sceSifSendCmd(command, 0, packet, packetBytes, source, destination, dataBytes);
}

s32 isceSifSendCmd(s32 command, void *packet, s32 packetBytes,
                   void *source, void *destination, s32 dataBytes)
{
    return _sceSifSendCmd(command, 1, packet, packetBytes, source, destination, dataBytes);
}

INCLUDE_ASM(const s32, "sdk/libkernl", _sceSifCmdIntrHdlr);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifWriteBackDCache);

INCLUDE_ASM(const s32, "sdk/libkernl", func_0030D084);

typedef struct rpcPacket {
    u32 w[16];
} rpcPacket;

typedef struct rpcQueue {
    u32 seq;
    rpcPacket *base;
    s32 count;
    u32 unk_C; /* Cleared by sceSifInitRpc. */
    u32 unk_10; /* Cleared by sceSifInitRpc. */
    rpcPacket *forwardBase;
    s32 forwardCount;
    rpcPacket *indexedBase;
    s32 indexedCount;
    s32 forwardIndex;
    sifRpcDataQueue_t *queues;
} rpcQueue;

extern rpcQueue sifRpcQueue;

extern rpcPacket rpcPackets[32];
extern rpcPacket rpcForwardPackets[32];
extern rpcPacket rpcIndexedPackets[32];
extern s32 sifRpcInitialized;
extern void sceSifInitCmd(void);
extern u32 sceSifGetReg(u32 registerId);
extern s32 sceSifSetReg(u32 registerId, u32 value);
extern void sceSifAddCmdHandler(s32 command, void (*handler)(void *, void *), void *argument);
extern void _request_end_0(void *packet, void *queue);
extern void _request_bind(void *packet, void *queue);
extern void _request_call(void *packet, void *queue);
extern void _request_rdata(rpcPacket *packet, rpcQueue *queue);

#ifdef NON_MATCHING
/* Native-sized reconstruction; see docs/matching/sdk/libkernl.md. */
void sceSifInitRpc(s32 mode)
{
    rpcPacket *packets;
    rpcPacket *packet;
    rpcQueue *queue;

    DIntr();
    if (sifRpcInitialized != 0) {
        EIntr();
        return;
    }
    sifRpcInitialized = 1;
    EIntr();
    sceSifInitCmd();
    DIntr();
    packets = rpcPackets;
    queue = &sifRpcQueue;
    queue->indexedCount = queue->forwardCount = queue->count = 32;
    queue->seq = 1;
    queue->base = EE_ADDR_AS(rpcPacket *, (EE_ADDR(packets) | 0x20000000));
    queue->unk_C = 0;
    queue->unk_10 = 0;
    queue->forwardBase = (rpcPacket *)(EE_ADDR(rpcForwardPackets) | 0x20000000);
    queue->indexedBase = (rpcPacket *)(EE_ADDR(rpcIndexedPackets) | 0x20000000);
    queue->forwardIndex = 0;
    sceSifAddCmdHandler(0x80000008, _request_end_0, queue);
    sceSifAddCmdHandler(0x80000009, _request_bind, queue);
    sceSifAddCmdHandler(0x8000000a, _request_call, queue);
    sceSifAddCmdHandler(0x8000000c, (void (*)(void *, void *))_request_rdata, queue);
    EIntr();
    if (sceSifGetReg(0x80000002) != 0)
        return;
    packet = packets + 1;
    packet->w[3] = 1;
    sceSifSendCmd(0x80000002, packet, 16, 0, 0, 0);
    while (sceSifGetSreg(0) == 0) {
    }
    sceSifSetReg(0x80000002, 1);
}
#else
INCLUDE_ASM(const s32, "sdk/libkernl", sceSifInitRpc);
#endif

extern s32 sifRpcInitialized;

void sceSifExitRpc(void)
{
    sceSifExitCmd();
    sifRpcInitialized = 0;
}

extern s32 DIntr(void);
extern s32 EIntr(void);

rpcPacket *_sceRpcGetPacket_0(rpcQueue *queue)
{
    rpcPacket *packet;
    s32 i;
    s32 count;

    DIntr();
    count = queue->count;
    packet = queue->base;
    for (i = 0; i < count; i++, packet++) {
        if ((packet->w[4] & 1) == 0) {
            u32 seq;

            packet->w[4] = (i << 16) | 5;
            queue->seq++;
            /* Capture this packet's sequence before advancing the queue again. */
            if (queue->seq == 1) {
                seq = queue->seq++;
            } else {
                seq = queue->seq;
            }
            packet->w[5] = EE_ADDR(packet);
            packet->w[6] = seq;
            EIntr();
            return packet;
        }
    }
    EIntr();
    return NULL;
}

void _sceRpcFreePacket_0(rpcPacket *packet)
{
    u32 flags = packet->w[4];

    packet->w[6] = 0;
    packet->w[4] = flags & ~1;
}

rpcPacket *_sceRpcGetFPacket(rpcQueue *queue)
{
    s32 index = queue->forwardIndex % queue->forwardCount;
    rpcPacket *packet = &queue->forwardBase[index];

    queue->forwardIndex = index + 1;
    return packet;
}

rpcPacket *_sceRpcGetFPacket2(rpcQueue *queue, s32 index)
{
    if (index < 0 || index >= queue->indexedCount) {
        return _sceRpcGetFPacket(queue);
    }
    return &queue->indexedBase[index];
}

/* Shared transaction fields used by RPC completion. */
typedef struct rpcTransactionPrefix {
    void *packet;
    u32 sequence;
    s32 sema;
} rpcTransactionPrefix;

SIZE_CHECK(rpcTransactionPrefix_size_must_be_12, rpcTransactionPrefix, 12);

typedef struct rpcClient {
    rpcTransactionPrefix transaction;
    u32 unk_C;
    u32 serverId;
    void *buffer;
    void *cbuffer;
    void (*endFunction)(void *);
    void *endParam;
    void *server;
} rpcClient;

SIZE_CHECK(rpcClient_size_must_be_40, rpcClient, 40);

/* Observed completion-response prefix, not the complete packet allocation. */
typedef struct rpcResponse {
    u32 commandHeader[4];
    u32 flags;
    void *packetAddress;
    u32 sequence;
    rpcTransactionPrefix *client;
    u32 command;
    void *server;
    void *buffer;
    void *cbuffer;
} rpcResponse;

SIZE_CHECK(rpcResponse_prefix_size_must_be_48, rpcResponse, 48);

void _request_end_0(void *packet, void *queue)
{
    rpcResponse *response = packet;
    rpcClient *client;
    rpcTransactionPrefix *transaction;

    switch (response->command) {
    case 0x8000000A:
        client = (rpcClient *)response->client;
        if (client->endFunction != NULL) {
            client->endFunction(client->endParam);
        }
        break;
    case 0x80000009:
        client = (rpcClient *)response->client;
        client->server = response->server;
        client->buffer = response->buffer;
        client->cbuffer = response->cbuffer;
        break;
    case 0x8000000C:
        break;
    }
    transaction = response->client;
    if (transaction->sema >= 0) {
        iSignalSema(transaction->sema);
    }
    _sceRpcFreePacket_0(transaction->packet);
    transaction->packet = NULL;
}

void _request_rdata(rpcPacket *request, rpcQueue *queue)
{
    rpcPacket *packet = _sceRpcGetFPacket(queue);

    u32 packetAddress = request->w[5];
    u32 client = request->w[7];

    packet->w[5] = packetAddress;
    packet->w[7] = client;
    packet->w[8] = 0x8000000C;
    isceSifSendCmd(0x80000008, packet, 0x40, EE_ADDR_AS(void *, request->w[8]),
                   EE_ADDR_AS(void *, request->w[9]), request->w[10]);
}

extern s32 WaitSema(s32 sema);
extern s32 DeleteSema(s32 sema);

s32 sceSifGetOtherData(rpcTransactionPrefix *client, void *source, void *destination, s32 size, s32 mode)
{
    rpcSemaParam param;
    rpcPacket *packet;

    packet = _sceRpcGetPacket_0(&sifRpcQueue);
    if (packet == NULL) {
        return -1;
    }
    client->packet = packet;
    client->sequence = packet->w[6];
    packet->w[8] = EE_ADDR(source);
    packet->w[9] = EE_ADDR(destination);
    packet->w[10] = size;
    packet->w[5] = EE_ADDR(packet);
    packet->w[7] = EE_ADDR(client);
    if ((mode & 1) == 0) {
        param.maxCount = 1;
        param.initCount = 0;
        client->sema = CreateSema(&param);
        if (client->sema < 0) {
            _sceRpcFreePacket_0(packet);
            return -3;
        }
        if (sceSifSendCmd(0x8000000C, packet, 0x40, NULL, NULL, 0) == 0) {
            _sceRpcFreePacket_0(packet);
            DeleteSema(client->sema);
            return -2;
        }
        WaitSema(client->sema);
        DeleteSema(client->sema);
        return 0;
    }
    client->sema = -1;
    if (sceSifSendCmd(0x8000000C, packet, 0x40, NULL, NULL, 0) == 0) {
        _sceRpcFreePacket_0(packet);
        return -2;
    }
    return 0;
}

sifRpcServerData_t *_search_svdata(s32 serviceId, rpcQueue *queue)
{
    sifRpcDataQueue_t *current;
    sifRpcServerData_t *server;

    for (current = queue->queues; current != NULL; current = current->next) {
        for (server = current->link; server != NULL; server = server->link) {
            if (server->serviceId == serviceId) {
                return server;
            }
        }
    }
    return NULL;
}

INCLUDE_ASM(const s32, "sdk/libkernl", _request_bind);

s32 sceSifBindRpc(rpcClient *client, u32 serverId, s32 mode)
{
    rpcSemaParam param;
    rpcPacket *packet;

    client->serverId = 0;
    client->server = NULL;
    packet = _sceRpcGetPacket_0(&sifRpcQueue);
    if (packet == NULL) {
        return -1;
    }
    client->transaction.packet = packet;
    client->transaction.sequence = packet->w[6];
    packet->w[8] = serverId;
    packet->w[5] = EE_ADDR(packet);
    packet->w[7] = EE_ADDR(client);
    if ((mode & 1) == 0) {
        param.maxCount = 1;
        param.initCount = 0;
        client->transaction.sema = CreateSema(&param);
        if (client->transaction.sema < 0) {
            _sceRpcFreePacket_0(packet);
            return -3;
        }
        if (sceSifSendCmd(0x80000009, packet, 0x40, NULL, NULL, 0) == 0) {
            _sceRpcFreePacket_0(packet);
            DeleteSema(client->transaction.sema);
            return -2;
        }
        WaitSema(client->transaction.sema);
        DeleteSema(client->transaction.sema);
        return 0;
    }
    client->transaction.sema = -1;
    if (sceSifSendCmd(0x80000009, packet, 0x40, NULL, NULL, 0) == 0) {
        _sceRpcFreePacket_0(packet);
        return -2;
    }
    return 0;
}

INCLUDE_ASM(const s32, "sdk/libkernl", _request_call);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifCallRpc);

typedef struct rpcClientPrefix {
    void *packet;
    u32 sequence;
} rpcClientPrefix;

/* NON_MATCHING: 44 of 60 native bytes, 5 of 11 compared words match;
 * the final active-bit test becomes a direct boolean return. */
#ifdef NON_MATCHING
s32 sceSifCheckStatRpc(rpcClientPrefix *client)
{
    rpcPacket *packet = client->packet;

    if (packet != NULL) {
        if (client->sequence == packet->w[6]) {
            if ((packet->w[4] & 1) != 0) {
                return 1;
            }
        }
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "sdk/libkernl", sceSifCheckStatRpc);
#endif

void sceSifSetRpcQueue(sifRpcDataQueue_t *queue, s32 threadId)
{
    sifRpcDataQueue_t *current;
    sifRpcDataQueue_t *head;

    DIntr();
    queue->threadId = threadId;
    queue->active = 0;
    queue->link = NULL;
    queue->start = NULL;
    queue->end = NULL;
    queue->next = NULL;
    head = sifRpcQueue.queues;
    if (head == NULL) {
        sifRpcQueue.queues = queue;
    } else {
        current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = queue;
    }
    EIntr();
}

void sceSifRegisterRpc(sifRpcServerData_t *server, u32 serviceId,
                        sifRpcFunc_t handler, void *buffer,
                        sifRpcFunc_t callback, void *callbackBuffer,
                        sifRpcDataQueue_t *queue)
{
    sifRpcServerData_t *current;
    sifRpcServerData_t *head;

    DIntr();
    server->next = NULL;
    server->link = NULL;
    server->serviceId = serviceId;
    server->handler = handler;
    server->buffer = buffer;
    server->callback = callback;
    server->callbackBuffer = callbackBuffer;
    server->queue = queue;
    head = queue->link;
    if (head == NULL) {
        queue->link = server;
    } else {
        current = head;
        while (current->link != NULL) {
            current = current->link;
        }
        current->link = server;
    }
    EIntr();
}

sifRpcServerData_t *sceSifRemoveRpc(sifRpcServerData_t *server, sifRpcDataQueue_t *queue)
{
    sifRpcServerData_t *current;

    DIntr();
    current = queue->link;
    if (current == server) {
        queue->link = current->link;
    } else {
        while (current != NULL) {
            if (current->link == server) {
                current->link = server->link;
                break;
            }
            current = current->link;
        }
    }
    EIntr();
    return current;
}

sifRpcDataQueue_t *sceSifRemoveRpcQueue(sifRpcDataQueue_t *queue)
{
    sifRpcDataQueue_t *current;

    DIntr();
    current = sifRpcQueue.queues;
    if (current == queue) {
        sifRpcQueue.queues = current->next;
    } else {
        while (current != NULL) {
            if (current->next == queue) {
                current->next = queue->next;
                break;
            }
            current = current->next;
        }
    }
    EIntr();
    return current;
}

sifRpcServerData_t *sceSifGetNextRequest(sifRpcDataQueue_t *queue)
{
    sifRpcServerData_t *request;

    DIntr();
    request = queue->start;
    if (request == NULL) {
        queue->active = 0;
    } else {
        queue->active = 1;
        queue->start = request->next;
    }
    EIntr();
    return request;
}

void sceSifExecRequest(sifRpcServerData_t *server)
{
    sifDmaTransfer_t transfers[2];
    rpcPacket *packet;
    void *reply;
    s32 replySize = 0;
    s32 count;
    s32 status;
    s32 delay;

    reply = server->handler(server->rpcNumber, server->buffer, server->size);
    if (reply != NULL) {
        replySize = server->receiveSize;
    }
    if (server->size > 0) {
        sceSifWriteBackDCache(server->buffer, server->size);
    }
    if (replySize > 0) {
        sceSifWriteBackDCache(reply, replySize);
    }
    DIntr();
    if (server->requestId & 4) {
        packet = _sceRpcGetFPacket2(&sifRpcQueue, (u32)server->requestId >> 16);
    } else {
        packet = _sceRpcGetFPacket(&sifRpcQueue);
    }
    EIntr();
    packet->w[8] = 0x8000000A;
    packet->w[7] = EE_ADDR(server->client);
    if (server->mode != 0) {
        while (sceSifSendCmd(0x80000008, packet, 0x40, reply, server->receiveBuffer, replySize) == 0) {
        }
        return;
    }
    packet->w[6] = 0;
    count = 0;
    packet->w[4] = 0;
    if (replySize > 0) {
        transfers[0].source = EE_ADDR(reply);
        transfers[0].destination = EE_ADDR(server->receiveBuffer);
        transfers[0].size = replySize;
        transfers[0].attributes = 0;
        count++;
    }
    transfers[count].source = EE_ADDR(packet);
    transfers[count].destination = EE_ADDR(server->packetAddress);
    transfers[count].size = 0x40;
    transfers[count].attributes = 0;
    count++;
    do {
        status = sceSifSetDma(transfers, count);
        if (status == 0) {
            for (delay = 0x100000; delay != -1; delay--) {
            }
        }
    } while (status == 0);
}

extern void sceSifExecRequest(sifRpcServerData_t *request);
extern s32 SleepThread(void);

void sceSifRpcLoop(sifRpcDataQueue_t *queue)
{
    sifRpcServerData_t *request;

    for (;;) {
        while ((request = sceSifGetNextRequest(queue)) != NULL) {
            sceSifExecRequest(request);
        }
        SleepThread();
    }
}

extern s32 fsIobSema __attribute__((section(".data")));
extern s32 fsIoctlSema __attribute__((section(".data")));

void gcc2_compiled__13(void)
{
    rpcSemaParam param;
    if (fsIobSema == -1) {
        param.initCount = 1;
        param.maxCount = 1;
        param.option = 0;
        fsIobSema = CreateSema(&param);
        fsIoctlSema = CreateSema(&param);
    }
}

typedef struct {
    u32 word0;
    s32 active;
    u8 opaque[8];
} fsRequestEntry_t;
extern fsRequestEntry_t fsIobTable[32];
extern s32 fsIobSema;
extern void gcc2_compiled__13(void);
extern s32 SignalSema(s32 sema);

fsRequestEntry_t *new_iob(void)
{
    fsRequestEntry_t *entry;

    gcc2_compiled__13();
    WaitSema(fsIobSema);
    for (entry = fsIobTable; entry < &fsIobTable[32]; entry++) {
        if (entry->active == 0) {
            entry->active = 0x10000000;
            SignalSema(fsIobSema);
            return entry;
        }
    }
    SignalSema(fsIobSema);
    return NULL;
}

fsRequestEntry_t *get_iob(u32 index)
{
    fsRequestEntry_t *entry;

    gcc2_compiled__13();
    WaitSema(fsIobSema);
    if (index >= 32) {
        SignalSema(fsIobSema);
        return NULL;
    }
    entry = &fsIobTable[index];
    SignalSema(fsIobSema);
    return entry;
}

typedef struct fsStat_t {
    u32 mode;
    u32 attributes;
    u32 size;
    u8 creationTime[8];
    u8 accessTime[8];
    u8 modificationTime[8];
    u32 highSize;
    u32 privateData[6];
} fsStat_t;

typedef struct fsCompletion_t {
    s32 sema;
    s32 command;
    void *result;
    u32 resultSize;
} fsCompletion_t;
typedef struct fsReadFragments_t {
    s32 headSize;
    s32 tailSize;
    u8 *headDestination;
    u8 *tailDestination;
    u8 head[0x40];
    u8 tail[0x40];
} fsReadFragments_t;
typedef struct fsDirectoryEntry_t {
    fsStat_t stat;
    char name[0x100];
    u32 privateData;
} fsDirectoryEntry_t;
typedef struct fsOutputCompletion_t {
    void *destination;
    u32 size;
} fsOutputCompletion_t;
extern u8 fsCompletionBuffer[];
extern u8 fsReadResultBuffer[];
extern s32 fsRequestSemas[32];
extern void *memcpy(void *destination, const void *source, size_t size);
extern s32 iSignalSema(s32 sema);

/* NON_MATCHING: 948/960 bytes; the final pending-semaphore search differs
 * in key lifetime and early-return scheduling. Copy paths and jump table match. */
#ifdef NON_MATCHING
void _sceFs_Rcv_Intr(void *packet, void *argument)
{
    fsCompletion_t completion;
    fsOutputCompletion_t output;
    s32 index;

    memcpy(&completion.sema, (void *)(EE_ADDR(fsCompletionBuffer) | 0x20000000), 4);
    memcpy(&completion.command, (void *)(EE_ADDR((fsCompletionBuffer + 4)) | 0x20000000), 4);
    memcpy(&completion.result, (void *)(EE_ADDR((fsCompletionBuffer + 8)) | 0x20000000), 4);
    memcpy(&completion.resultSize, (void *)(EE_ADDR((fsCompletionBuffer + 12)) | 0x20000000), 4);
    if (completion.sema >= 0) {
        memcpy(completion.result, (void *)(EE_ADDR((fsCompletionBuffer + 16)) | 0x20000000),
               completion.resultSize);
    }
    switch (completion.command) {
    case 2: {
        fsReadFragments_t *fragments = (void *)(EE_ADDR(fsReadResultBuffer) | 0x20000000);
        u8 *destination;
        s32 i;

        if (fragments->headSize > 0) {
            destination = fragments->headDestination;
            for (i = 0; i < fragments->headSize; i++) {
                destination[i] = fragments->head[i];
            }
        }
        if (fragments->tailSize > 0) {
            destination = fragments->tailDestination;
            for (i = 0; i < fragments->tailSize; i++) {
                destination[i] = fragments->tail[i];
            }
        }
        break;
    }
    case 11: {
        memcpy(&output.destination, (void *)(EE_ADDR(fsReadResultBuffer) | 0x20000000), 4);
        memcpy(output.destination, (void *)(EE_ADDR((fsReadResultBuffer + 4)) | 0x20000000),
               sizeof(fsDirectoryEntry_t));
        break;
    }
    case 12: {
        memcpy(&output.destination, (void *)(EE_ADDR(fsReadResultBuffer) | 0x20000000), 4);
        memcpy(output.destination, (void *)(EE_ADDR((fsReadResultBuffer + 4)) | 0x20000000),
               sizeof(fsStat_t));
        break;
    }
    case 23:
    case 25:
    case 26: {
        const void *destinationSource = (void *)(EE_ADDR(fsReadResultBuffer) | 0x20000000);
        const void *sizeSource = (void *)(EE_ADDR((fsReadResultBuffer + 4)) | 0x20000000);
        u32 size;

        memcpy(&output.destination, destinationSource, 4);
        memcpy(&output.size, sizeSource, 4);
        size = output.size;
        if (size > 0x400) {
            output.size = 0x400;
            size = 0x400;
        }
        memcpy(output.destination, (void *)(EE_ADDR((fsReadResultBuffer + 8)) | 0x20000000),
               size);
        break;
    }
    }
    if (completion.sema < 0) {
        s32 sema;

        completion.sema = -completion.sema;
        sema = completion.sema;
        for (index = 0; index < 32; index++) {
            if (fsRequestSemas[index] == sema) {
                fsRequestSemas[index] = -1;
                break;
            }
        }
        return;
    }
    iSignalSema(completion.sema);
}
#else
INCLUDE_ASM(const s32, "sdk/libkernl", _sceFs_Rcv_Intr);
#endif

extern s32 fsCommandSema __attribute__((section(".data")));

void _sceFsSemInit(void)
{
    rpcSemaParam param;
    if (fsCommandSema == -1) {
        /* Only these descriptor fields are initialized by the native helper. */
        param.initCount = 1;
        param.maxCount = 1;
        param.option = 0;
        fsCommandSema = CreateSema(&param);
    }
}

s32 _sceFsWaitS(s32 command)
{
    _sceFsSemInit();
    WaitSema(fsCommandSema);
    return 0;
}

extern s32 SignalSema(s32 sema);

/* Internal notification: callers discard the SignalSema status. */
void _sceFsSigSema(void)
{
    SignalSema(fsCommandSema);
}

typedef void (*fsPowerOffCallback_t)(void *argument);
typedef struct fsPowerOffState_t {
    fsPowerOffCallback_t callback;
    void *argument;
} fsPowerOffState_t;
extern fsPowerOffState_t fsPowerOffState;
extern s32 fsInitialized;
extern s32 sceFsInit(void);

fsPowerOffCallback_t scePowerOffHandler(fsPowerOffCallback_t callback, void *argument)
{
    fsPowerOffState_t *state = &fsPowerOffState;
    fsPowerOffCallback_t previous;
    s32 interrupts;

    _sceFsWaitS(0x1B);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    interrupts = DIntr();
    previous = fsPowerOffState.callback;
    state->argument = argument;
    fsPowerOffState.callback = callback;
    if (interrupts != 0) {
        EIntr();
    }
    _sceFsSigSema();
    return previous;
}

void _sceFs_Poff_Intr(void *packet, fsPowerOffState_t *state)
{
    if (state->callback != NULL) {
        state->callback(state->argument);
    }
    EE_SYNC_EI();
}

typedef struct { u8 bytes[4]; } fsVersionTag_t;
typedef struct { void *receiveBuffer; } fsRegistrationPacket_t;

extern u8 fsCmdHandlerContext[];
extern rpcClient fsRpcClient;
extern u8 fsCompletionBuffer[];
extern u8 fsVersionRpcBuffer[];
extern fsRegistrationPacket_t fsRegistration;
extern void sceSifInitRpc(s32 mode);
extern void sceSifAddCmdHandler(s32 command, void (*handler)(void *, void *), void *argument);
extern void _sceFs_Rcv_Intr(void *packet, void *argument);
extern s32 sceSifCallRpc(void *client, s32 function, s32 mode, void *send,
                         s32 sendBytes, void *receive, s32 receiveBytes,
                         void *endFunction, void *endParameter);
extern u8 fsDetectedVersion[0x18];
extern s32 fsInitialized __attribute__((section(".data")));
extern s32 SignalSema(s32 sema);

INCLUDE_ASM(const s32, "sdk/libkernl", sceFsInit);

extern u8 fsDetectedVersion[0x18];
extern u8 kernelBuiltinVersion[4] __attribute__((section(".data")));
extern const void *fsFallbackVersion __attribute__((section(".data")));
extern s32 memcmp(const void *left, const void *right, size_t count);

s32 _fs_version(void)
{
    const void *expected = kernelBuiltinVersion;
    const void *version = fsDetectedVersion;
    s32 mismatch = 0;
    if (memcmp(version, expected, 4) != 0) {
        if (memcmp(version, fsFallbackVersion, 4) != 0) {
            mismatch = memcmp(expected, fsFallbackVersion, 4) != 0;
        }
    }
    return mismatch;
}

extern s32 fsInitialized __attribute__((section(".data")));
extern void *memset(void *destination, s32 value, size_t count);

s32 sceFsReset(void)
{
    fsInitialized = 0;
    memset(fsDetectedVersion, 0, 4);
    return 0;
}

typedef struct fsCloseRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    u32 descriptor;
    s32 slot;
} fsCloseRequest_t;
typedef struct fsPathRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    char name[0x400];
} fsPathRequest_t;
typedef struct fsMkdirRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    s32 mode;
    char name[0x400];
} fsMkdirRequest_t;
typedef struct fsGetstatRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    void *stat;
    u8 name[0x400];
} fsGetstatRequest_t;
typedef struct fsSyncRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    u32 descriptor; /* Shared word, left untouched by sync. */
    s32 mode;
    char name[0x400];
} fsSyncRequest_t;
typedef struct fsRenameRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    char oldName[0x400];
    char newName[0x400];
} fsRenameRequest_t;
typedef struct fsChstatRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    u32 mask;
    fsStat_t stat;
    u8 name[0x400];
} fsChstatRequest_t;
typedef struct fsReadlinkRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    u32 count;
    char *buffer;
    char name[0x400];
} fsReadlinkRequest_t;
typedef struct fsFormatRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    char device[0x400];
    char blockDevice[0x400];
    u8 args[0x400];
    s32 argSize;
} fsFormatRequest_t;
typedef struct fsMountRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    char device[0x400];
    char blockDevice[0x400];
    u8 args[0x400];
    s32 mode;
    s32 argSize;
} fsMountRequest_t;
typedef struct fsDreadRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    u32 descriptor;
    void *entry;
    u8 opaque14[0xC];
} fsDreadRequest_t;
typedef struct fsDevctlRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    char name[0x400];
    u8 args[0x400];
    s32 command;
    u32 argSize;
    void *output;
    u32 outputSize;
} fsDevctlRequest_t;
typedef struct fsIoctl2Request_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    u32 descriptor;
    s32 command;
    u8 args[0x400];
    void *output;
    u32 outputSize;
    u32 argSize;
} fsIoctl2Request_t;
typedef struct fsOpenRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    u32 flags;
    s32 mode;
    char name[0x400];
    s32 slot;
} fsOpenRequest_t;
typedef union fsRequest_t {
    fsOpenRequest_t open;
    fsCloseRequest_t close;
    fsPathRequest_t path;
    fsMkdirRequest_t mkdir;
    fsGetstatRequest_t getstat;
    fsSyncRequest_t sync;
    fsRenameRequest_t rename;
    fsChstatRequest_t chstat;
    fsReadlinkRequest_t readlink;
    fsFormatRequest_t format;
    fsDevctlRequest_t devctl;
    fsIoctl2Request_t ioctl2;
    fsMountRequest_t mount;
    fsDreadRequest_t dread;
} fsRequest_t;
extern fsRequest_t fsRequest;

/* NON_MATCHING: 640/644 bytes; opposite saved-register assignments
 * and final slot-return scheduling remain unresolved. */
#ifdef NON_MATCHING
s32 sceOpen(const char *name, s32 flags, ...)
{
    rpcSemaParam param;
    s32 result;
    fsOpenRequest_t *request = &fsRequest.open;
    fsRequestEntry_t *entry;
    kernelVaList_t args;
    s32 mode;
    s32 length;
    s32 slot;
    s32 sema;
    s32 accepted;

    _sceFsWaitS(0);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    if (_fs_version() != 0) {
        _sceFsSigSema();
        return (s32)0xFFFEFFFC;
    }
    entry = new_iob();
    if (entry == NULL) {
        _sceFsSigSema();
        return -19;
    }
    args = (kernelVaList_t)__builtin_next_arg(flags) - 6 * sizeof(long);
    mode = *(s32 *)args;
    for (length = 0; length < 0x400; length++) {
        if ((request->name[length] = name[length]) == 0) {
            break;
        }
    }
    if (length == 0x400) {
        request->name[0x3FF] = 0;
    }
    slot = entry - fsIobTable;
    request->flags = flags & 0x6FFFFFFF;
    request->mode = mode;
    request->slot = slot;
    param.maxCount = 1;
    param.initCount = 0;
    param.option = 0;
    sema = CreateSema(&param);
    request->result = &result;
    request->sema = sema;
    request->resultSize = 4;
    if (sceSifCallRpc(&fsRpcClient, 0, 0, &fsRequest, 0x418,
                      fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    if (result < 0) {
        WaitSema(fsIobSema);
        entry->active = 0;
        SignalSema(fsIobSema);
        return result;
    }
    WaitSema(fsIobSema);
    entry->active |= flags;
    entry->word0 = result;
    SignalSema(fsIobSema);
    return slot;
}

#else
INCLUDE_ASM(const s32, "sdk/libkernl", sceOpen);
#endif

s32 sceClose(s32 descriptor)
{
    rpcSemaParam param;
    s32 result;
    fsCloseRequest_t *request = &fsRequest.close;
    fsRequestEntry_t *entry;
    s32 sema;
    s32 accepted;
    s32 status;
    s32 threshold;

    entry = get_iob(descriptor);
    _sceFsWaitS(1);
    if (fsInitialized == 0) {
        _sceFsSigSema();
        return -1;
    }
    if (entry == NULL || entry->active == 0) {
        _sceFsSigSema();
        return -9;
    }
    request->descriptor = entry->word0;
    request->slot = entry - fsIobTable;
    param.maxCount = 1;
    param.initCount = 0;
    param.option = 0;
    sema = CreateSema(&param);
    fsRequest.close.sema = sema;
    request->result = &result;
    request->resultSize = 4;
    if (sceSifCallRpc(&fsRpcClient, 1, 0, request, 0x14,
                      fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    entry->active = 0;
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    status = result;
    threshold = -1;
    /* Keep the real native signed threshold in a general register for its
     * comparison. No instruction, fixed register or memory effect is added. */
    FAKEMATCH_ASM_LIVE(threshold);
    if (threshold < status) {
        status = 0;
    }
    return status;
}

INCLUDE_ASM(const s32, "sdk/libkernl", sceLseek);

INCLUDE_ASM(const s32, "sdk/libkernl", sceRead);

INCLUDE_ASM(const s32, "sdk/libkernl", sceWrite);

typedef union fsIoctlStatus_t {
    s32 word;
    s64 doubleword;
} fsIoctlStatus_t;
extern fsIoctlStatus_t fsIoctlStatus;
extern void *fsIoctlArgument;
extern s32 fsRequestSemas[32];
extern void *memcpy(void *destination, const void *source, size_t size);

/* NON_MATCHING: native 844-byte size; early switch allocation and
 * pending-table setup differ in 46 instruction words. */
#ifdef NON_MATCHING
s32 sceIoctl(s32 descriptor, s32 command, void *argument)
{
    rpcSemaParam param;
    s32 result;
    fsIoctl2Request_t *request = &fsRequest.ioctl2;
    fsRequestEntry_t *entry;
    s32 index;
    s32 sema;
    s32 accepted;
    fsRequest_t *packet;

    entry = get_iob(descriptor);
    _sceFsWaitS(5);
    fsIoctlArgument = argument;
    if (fsInitialized == 0) {
        sceFsInit();
    }
    if (entry == NULL || entry->active == 0) {
        _sceFsSigSema();
        return -9;
    }
    request->output = NULL;
    request->outputSize = 0;
    switch (command) {
    case 1:
        WaitSema(fsIoctlSema);
        for (index = 0; index < 32 && fsRequestSemas[index] == -1; index++) {
        }
        if (index == 32) {
            *(s32 *)fsIoctlArgument = 0;
        } else {
            *(s32 *)fsIoctlArgument = 1;
        }
        SignalSema(fsIoctlSema);
        _sceFsSigSema();
        return 0;
    case 2:
        *(s32 *)argument = ((fsIoctlStatus_t *)(EE_ADDR(&fsIoctlStatus) | 0x20000000))->word;
        _sceFsSigSema();
        return 0;
    case 3:
        *(s64 *)argument = ((fsIoctlStatus_t *)(EE_ADDR(&fsIoctlStatus) | 0x20000000))->doubleword;
        _sceFsSigSema();
        return 0;
    }
    request->descriptor = entry->word0;
    request->command = command;
    if (argument == NULL) {
        request->argSize = 0;
    } else {
        request->argSize = 0x400;
        memcpy(request->args, argument, 0x400);
    }
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    sema = CreateSema(&param);
    packet = &fsRequest;
    request->sema = sema;
    request->result = &result;
    request->resultSize = 4;
    sceSifWriteBackDCache(packet, 0x420);
    if (sceSifCallRpc(&fsRpcClient, 5, 0, packet,
                      0x420, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}
#else
INCLUDE_ASM(const s32, "sdk/libkernl", sceIoctl);
#endif

s32 sceIoctl2(s32 descriptor, s32 command, const void *args, u32 argSize,
              void *output, u32 outputSize)
{
    rpcSemaParam param;
    s32 result;
    fsIoctl2Request_t *request = &fsRequest.ioctl2;
    fsRequestEntry_t *entry;
    s32 sema;
    s32 accepted;
    fsRequest_t *packet;

    entry = get_iob(descriptor);
    _sceFsWaitS(26);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    if (entry == NULL || entry->active == 0) {
        _sceFsSigSema();
        return -9;
    }
    if (argSize > 0x400 || outputSize > 0x400) {
        _sceFsSigSema();
        return -22;
    }
    if (args == NULL) {
        request->argSize = 0;
    } else {
        memcpy(request->args, args, argSize);
    }
    request->descriptor = entry->word0;
    request->command = command;
    request->argSize = argSize;
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    packet = &fsRequest;
    sema = CreateSema(&param);
    request->outputSize = outputSize;
    request->result = &result;
    request->resultSize = 4;
    request->output = output;
    request->sema = sema;
    sceSifWriteBackDCache(packet, 0x420);
    if (sceSifCallRpc(&fsRpcClient, 26, 0, packet,
                      0x420, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 _sceCallCode(const char *path, s32 command)
{
    rpcSemaParam param;
    s32 result;
    fsPathRequest_t *request = &fsRequest.path;
    s32 length;
    s32 sema;
    s32 accepted;

    _sceFsWaitS(command);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    for (length = 0; length < 0x400 &&
         (request->name[length] = path[length]) != 0; length++) {
    }
    if (length == 0x400) {
        request->name[0x3FF] = 0;
        length = 0x3FF;
    }
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    sema = CreateSema(&param);
    request->result = &result;
    request->sema = sema;
    request->resultSize = 4;
    if (sceSifCallRpc(&fsRpcClient, command, 0, &fsRequest,
                      length + 0xD, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 sceRemove(const char *path)
{
    return _sceCallCode(path, 6);
}

s32 sceMkdir(const char *path, s32 mode)
{
    rpcSemaParam param;
    s32 result;
    fsMkdirRequest_t *request = &fsRequest.mkdir;
    s32 length;
    s32 sema;
    s32 accepted;

    _sceFsWaitS(7);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    for (length = 0; length < 0x400 &&
         (request->name[length] = path[length]) != 0; length++) {
    }
    if (length == 0x400) {
        request->name[0x3FF] = 0;
        length = 0x3FF;
    }
    request->mode = mode;
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    sema = CreateSema(&param);
    request->result = &result;
    request->sema = sema;
    request->resultSize = 4;
    if (sceSifCallRpc(&fsRpcClient, 7, 0, &fsRequest,
                      length + 0x11, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 sceRmdir(const char *path)
{
    return _sceCallCode(path, 8);
}

s32 sceFormat(const char *device, const char *blockDevice, const void *args, s32 argSize)
{
    rpcSemaParam param;
    s32 result;
    fsFormatRequest_t *request = &fsRequest.format;
    s32 deviceLength;
    s32 blockLength;
    s32 argIndex;
    s32 sema;
    s32 accepted;
    fsRequest_t *packet;

    _sceFsWaitS(14);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    for (deviceLength = 0; deviceLength < 0x400 &&
         (request->device[deviceLength] = device[deviceLength]) != 0; deviceLength++) {
    }
    if (deviceLength == 0x400) {
        request->device[0x3FF] = 0;
    }
    if (blockDevice == NULL) {
        request->blockDevice[0] = 0;
    } else {
        for (blockLength = 0; blockLength < 0x400 &&
             (request->blockDevice[blockLength] = blockDevice[blockLength]) != 0; blockLength++) {
        }
        if (blockLength == 0x400) {
            request->blockDevice[0x3FF] = 0;
        }
    }
    if (argSize > 0x400) {
        _sceFsSigSema();
        return -7;
    }
    for (argIndex = 0; argIndex < argSize; argIndex++) {
        request->args[argIndex] = ((const u8 *)args)[argIndex];
    }
    request->argSize = argSize;
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    packet = &fsRequest;
    sema = CreateSema(&param);
    request->sema = sema;
    request->result = &result;
    request->resultSize = 4;
    sceSifWriteBackDCache(packet, 0xC10);
    if (sceSifCallRpc(&fsRpcClient, 14, 0, packet,
                      0xC10, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

INCLUDE_ASM(const s32, "sdk/libkernl", sceAddDrv);
INCLUDE_ASM(const s32, "sdk/libkernl", func_00310000);

s32 sceDelDrv(const char *path)
{
    return _sceCallCode(path, 16);
}

s32 sceDopen(const char *path)
{
    fsRequestEntry_t *entry;
    s32 result;
    s32 sema;

    _sceFsWaitS(9);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    _sceFsSigSema();
    entry = new_iob();
    if (entry == NULL) {
        return -19;
    }
    result = _sceCallCode(path, 9);
    if (result < 0) {
        WaitSema(fsIobSema);
        entry->active = 0;
        sema = fsIobSema;
    } else {
        WaitSema(fsIobSema);
        entry->word0 = result;
        sema = fsIobSema;
        result = entry - fsIobTable;
    }
    SignalSema(sema);
    return result;
}

s32 sceDclose(s32 descriptor)
{
    rpcSemaParam param;
    s32 result;
    fsCloseRequest_t *request = &fsRequest.close;
    fsRequestEntry_t *entry;
    s32 sema;
    s32 accepted;
    s32 status;
    s32 threshold;

    entry = get_iob(descriptor);
    _sceFsWaitS(10);
    if (fsInitialized == 0) {
        _sceFsSigSema();
        return -1;
    }
    if (entry == NULL || entry->active == 0) {
        _sceFsSigSema();
        return -9;
    }
    request->descriptor = entry->word0;
    param.maxCount = 1;
    param.initCount = 0;
    param.option = 0;
    sema = CreateSema(&param);
    fsRequest.close.sema = sema;
    request->result = &result;
    request->resultSize = 4;
    if (sceSifCallRpc(&fsRpcClient, 10, 0, request, 0x14,
                      fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    entry->active = 0;
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    status = result;
    threshold = -1;
    /* Keep the real native signed threshold in a general register for its
     * comparison. No instruction, fixed register or memory effect is added. */
    FAKEMATCH_ASM_LIVE(threshold);
    if (threshold < status) {
        status = 0;
    }
    return status;
}

s32 sceDread(s32 descriptor, void *directoryEntry)
{
    rpcSemaParam param;
    s32 result;
    fsDreadRequest_t *request = &fsRequest.dread;
    fsRequestEntry_t *entry;
    s32 sema;
    s32 accepted;

    entry = get_iob(descriptor);
    _sceFsWaitS(11);
    if (fsInitialized == 0) {
        _sceFsSigSema();
        return -1;
    }
    if (entry == NULL || entry->active == 0) {
        _sceFsSigSema();
        return -9;
    }
    request->entry = directoryEntry;
    request->descriptor = entry->word0;
    param.maxCount = 1;
    param.initCount = 0;
    param.option = 0;
    sema = CreateSema(&param);
    fsRequest.dread.sema = sema;
    request->result = &result;
    request->resultSize = 4;
    if (sceSifCallRpc(&fsRpcClient, 11, 0, request, 0x20,
                      fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        /* The failure path waits before releasing the filesystem lock. */
        WaitSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 sceGetstat(const char *path, void *stat)
{
    rpcSemaParam param;
    s32 result;
    fsGetstatRequest_t *request = &fsRequest.getstat;
    s32 length;
    s32 sema;
    s32 accepted;

    _sceFsWaitS(12);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    for (length = 0; length < 0x400 &&
         (request->name[length] = path[length]) != 0; length++) {
    }
    if (length == 0x400) {
        request->name[0x3FF] = 0;
        length = 0x3FF;
    }
    request->stat = stat;
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    sema = CreateSema(&param);
    request->result = &result;
    request->sema = sema;
    request->resultSize = 4;
    if (sceSifCallRpc(&fsRpcClient, 12, 0, &fsRequest,
                      length + 0x11, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 sceChstat(const char *path, const fsStat_t *stat, u32 mask)
{
    rpcSemaParam param;
    s32 result;
    fsChstatRequest_t *request = &fsRequest.chstat;
    s32 length;
    s32 sema;
    s32 accepted;
    fsRequest_t *packet;

    _sceFsWaitS(13);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    for (length = 0; length < 0x400 &&
         (request->name[length] = path[length]) != 0; length++) {
    }
    if (length == 0x400) {
        request->name[0x3FF] = 0;
        length = 0x3FF;
    }
    request->stat = *stat;
    request->mask = mask;
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    packet = &fsRequest;
    sema = CreateSema(&param);
    request->sema = sema;
    request->result = &result;
    request->resultSize = 4;
    sceSifWriteBackDCache(packet, 0x450);
    if (sceSifCallRpc(&fsRpcClient, 13, 0, packet,
                      length + 0x51, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 sceRename(const char *oldPath, const char *newPath)
{
    rpcSemaParam param;
    s32 result;
    fsRenameRequest_t *request = &fsRequest.rename;
    s32 oldLength;
    s32 newLength;
    s32 sema;
    s32 accepted;
    fsRequest_t *packet;

    _sceFsWaitS(17);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    for (oldLength = 0; oldLength < 0x400 &&
         (request->oldName[oldLength] = oldPath[oldLength]) != 0; oldLength++) {
    }
    if (oldLength == 0x400) {
        request->oldName[0x3FF] = 0;
    }
    for (newLength = 0; newLength < 0x400 &&
         (request->newName[newLength] = newPath[newLength]) != 0; newLength++) {
    }
    if (newLength == 0x400) {
        request->newName[0x3FF] = 0;
    }
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    packet = &fsRequest;
    sema = CreateSema(&param);
    request->sema = sema;
    request->result = &result;
    request->resultSize = 4;
    sceSifWriteBackDCache(packet, 0x80C);
    if (sceSifCallRpc(&fsRpcClient, 17, 0, packet,
                      0x80C, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 sceChdir(const char *path)
{
    return _sceCallCode(path, 18);
}

s32 sceSync(const char *path, s32 mode)
{
    rpcSemaParam param;
    s32 result;
    fsSyncRequest_t *request = &fsRequest.sync;
    s32 length;
    s32 sema;
    s32 accepted;

    _sceFsWaitS(19);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    for (length = 0; length < 0x400 &&
         (request->name[length] = path[length]) != 0; length++) {
    }
    if (length == 0x400) {
        request->name[0x3FF] = 0;
    }
    request->mode = mode;
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    sema = CreateSema(&param);
    request->result = &result;
    request->sema = sema;
    request->resultSize = 4;
    if (sceSifCallRpc(&fsRpcClient, 19, 0, &fsRequest,
                      0x414, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 sceMount(const char *device, const char *blockDevice, s32 mode, const void *args, s32 argSize)
{
    rpcSemaParam param;
    s32 result;
    fsMountRequest_t *request = &fsRequest.mount;
    s32 deviceLength;
    s32 blockLength;
    s32 argIndex;
    s32 sema;
    s32 accepted;
    fsRequest_t *packet;

    _sceFsWaitS(20);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    for (deviceLength = 0; deviceLength < 0x400 &&
         (request->device[deviceLength] = device[deviceLength]) != 0; deviceLength++) {
    }
    if (deviceLength == 0x400) {
        request->device[0x3FF] = 0;
    }
    for (blockLength = 0; blockLength < 0x400 &&
         (request->blockDevice[blockLength] = blockDevice[blockLength]) != 0; blockLength++) {
    }
    if (blockLength == 0x400) {
        request->blockDevice[0x3FF] = 0;
    }
    if (argSize > 0x400) {
        _sceFsSigSema();
        return -7;
    }
    for (argIndex = 0; argIndex < argSize; argIndex++) {
        request->args[argIndex] = ((const u8 *)args)[argIndex];
    }
    request->argSize = argSize;
    request->mode = mode;
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    packet = &fsRequest;
    sema = CreateSema(&param);
    request->sema = sema;
    request->result = &result;
    request->resultSize = 4;
    sceSifWriteBackDCache(packet, 0xC14);
    if (sceSifCallRpc(&fsRpcClient, 20, 0, packet,
                      0xC14, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 sceUmount(const char *path)
{
    return _sceCallCode(path, 21);
}

INCLUDE_ASM(const s32, "sdk/libkernl", sceLseek64);

s32 sceDevctl(const char *device, s32 command, const void *args, u32 argSize,
              void *output, u32 outputSize)
{
    rpcSemaParam param;
    s32 result;
    fsDevctlRequest_t *request;
    s32 length;
    u32 argIndex;
    s32 sema;
    s32 accepted;
    fsRequest_t *packet;

    _sceFsWaitS(23);
    request = &fsRequest.devctl;
    if (fsInitialized == 0) {
        sceFsInit();
    }
    for (length = 0; length < 0x400 &&
         (request->name[length] = device[length]) != 0; length++) {
    }
    if (length == 0x400) {
        request->name[0x3FF] = 0;
    }
    if (argSize > 0x400 || outputSize > 0x400) {
        _sceFsSigSema();
        return -22;
    }
    for (argIndex = 0; argIndex < argSize; argIndex++) {
        request->args[argIndex] = ((const u8 *)args)[argIndex];
    }
    request->argSize = argSize;
    request->command = command;
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    packet = &fsRequest;
    sema = CreateSema(&param);
    request->outputSize = outputSize;
    request->result = &result;
    request->resultSize = 4;
    request->output = output;
    request->sema = sema;
    sceSifWriteBackDCache(packet, 0x81C);
    if (sceSifCallRpc(&fsRpcClient, 23, 0, packet,
                      0x81C, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 sceSymlink(const char *oldPath, const char *newPath)
{
    rpcSemaParam param;
    s32 result;
    fsRenameRequest_t *request = &fsRequest.rename;
    s32 oldLength;
    s32 newLength;
    s32 sema;
    s32 accepted;

    _sceFsWaitS(17);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    for (oldLength = 0; oldLength < 0x400 &&
         (request->oldName[oldLength] = oldPath[oldLength]) != 0; oldLength++) {
    }
    if (oldLength == 0x400) {
        request->oldName[0x3FF] = 0;
    }
    for (newLength = 0; newLength < 0x400 &&
         (request->newName[newLength] = newPath[newLength]) != 0; newLength++) {
    }
    if (newLength == 0x400) {
        request->newName[0x3FF] = 0;
    }
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    sema = CreateSema(&param);
    request->result = &result;
    request->sema = sema;
    request->resultSize = 4;
    if (sceSifCallRpc(&fsRpcClient, 24, 0, &fsRequest,
                      0x80C, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 sceReadlink(const char *path, char *buffer, u32 count)
{
    rpcSemaParam param;
    s32 result;
    fsReadlinkRequest_t *request = &fsRequest.readlink;
    s32 length;
    s32 sema;
    s32 accepted;

    _sceFsWaitS(17);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    for (length = 0; length < 0x400 &&
         (request->name[length] = path[length]) != 0; length++) {
    }
    if (length == 0x400) {
        request->name[0x3FF] = 0;
    }
    if (count >= 0x400) {
        count = 0x3FF;
    }
    request->buffer = buffer;
    request->count = count;
    sceSifWriteBackDCache(buffer, count);
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    sema = CreateSema(&param);
    request->result = &result;
    request->sema = sema;
    request->resultSize = 4;
    /* The fixed transfer also includes the shared buffer's untouched tail. */
    if (sceSifCallRpc(&fsRpcClient, 25, 0, &fsRequest,
                      0x80C, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

extern s32 iopHeapStatus;
/* The BSS map bounds this opaque RPC client to 0x40 bytes. */
extern u8 iopHeapRpcClient[0x40];
extern s32 iopHeapResult;
/* Shared heap request storage is 0x40 bytes in the BSS map. */
extern addr_t iopHeapArgs[16];
extern s32 sceSifCallRpc(void *client, s32 function, s32 mode, void *send,
                         s32 sendBytes, void *receive, s32 receiveBytes,
                         void *endFunction, void *endParameter);

s32 sceSifInitIopHeap(void)
{
    s32 delay;

    for (;;) {
        if (sceSifBindRpc((rpcClient *)iopHeapRpcClient, 0x80000003, 0) < 0) {
            return -1;
        }
        if (((rpcClient *)iopHeapRpcClient)->server != NULL) {
            break;
        }
        for (delay = 0x100000; delay != -1; delay--) {
        }
    }
    {
        /* SDK 2.9 otherwise separates the flag-address and return phases.
         * Keep this real address/store/result lifetime in the return register;
         * the binding is a reconstruction aid, not original-source evidence.
         * This initialized lifetime starts after the final call.
         */
        FAKEMATCH_ASM_REG(saddr_t, result, "v0") = EE_ADDR_S(&iopHeapStatus);
        *EE_ADDR_AS(s32 *, result) = 0;
        result = 0;
        return result;
    }
}

s32 sceSifAllocIopHeap(s32 size)
{
    if (iopHeapStatus < 0) {
        return 0;
    }
    iopHeapArgs[0] = size;
    if (sceSifCallRpc(iopHeapRpcClient, 1, 0, iopHeapArgs, 4,
                      &iopHeapResult, 4, NULL, NULL) >= 0) {
        return iopHeapResult;
    }
    return 0;
}

void *sceSifAllocSysMemory(s32 mode, s32 size, void *address)
{
    if (iopHeapStatus < 0) {
        return NULL;
    }
    iopHeapArgs[0] = size;
    iopHeapArgs[1] = mode;
    iopHeapArgs[2] = EE_ADDR(address);
    if (sceSifCallRpc(iopHeapRpcClient, 4, 0, iopHeapArgs, 12,
                      &iopHeapResult, 4, NULL, NULL) >= 0) {
        return EE_ADDR_AS(void *, iopHeapResult);
    }
    return NULL;
}

s32 sceSifFreeSysMemory(void *address)
{
    if (iopHeapStatus < 0) {
        return 0;
    }
    iopHeapArgs[0] = EE_ADDR(address);
    if (sceSifCallRpc(iopHeapRpcClient, 2, 0, iopHeapArgs, 4,
                      &iopHeapResult, 4, NULL, NULL) >= 0) {
        return iopHeapResult;
    }
    return -1;
}

s32 sceSifFreeIopHeap(void *address)
{
    return sceSifFreeSysMemory(address);
}

typedef struct heapLoadRequest_t {
    u32 address;
    char name[0xFC];
} heapLoadRequest_t;
extern heapLoadRequest_t iopHeapLoadRequest;

s32 sceSifLoadIopHeap(const char *path, u32 address)
{
    s32 length;

    if (iopHeapStatus < 0) {
        return 0;
    }
    for (length = 0; length < 0xFC &&
         (iopHeapLoadRequest.name[length] = path[length]) != 0; length++) {
    }
    if (length == 0xFC) {
        iopHeapLoadRequest.name[length - 1] = 0;
        length--;
    }
    iopHeapLoadRequest.address = address;
    iopHeapLoadRequest.name[0xFB] = 0;
    if (sceSifCallRpc(iopHeapRpcClient, 3, 0, &iopHeapLoadRequest, length + 5,
                      &iopHeapResult, 4, NULL, NULL) >= 0) {
        return iopHeapResult;
    }
    return -1;
}

u32 sceSifQueryMemSize(void)
{
    if (iopHeapStatus < 0) {
        return 0;
    }
    if (sceSifCallRpc(iopHeapRpcClient, 5, 0, NULL, 0,
                      &iopHeapResult, 4, NULL, NULL) < 0) {
        return 0xFFFFFFFF;
    }
    return iopHeapResult;
}

u32 sceSifQueryMaxFreeMemSize(void)
{
    if (iopHeapStatus < 0) {
        return 0;
    }
    if (sceSifCallRpc(iopHeapRpcClient, 6, 0, NULL, 0,
                      &iopHeapResult, 4, NULL, NULL) < 0) {
        return 0xFFFFFFFF;
    }
    return iopHeapResult;
}

u32 sceSifQueryTotalFreeMemSize(void)
{
    if (iopHeapStatus < 0) {
        return 0;
    }
    if (sceSifCallRpc(iopHeapRpcClient, 7, 0, NULL, 0,
                      &iopHeapResult, 4, NULL, NULL) < 0) {
        return 0xFFFFFFFF;
    }
    return iopHeapResult;
}

/* The IOP block key and query results travel as 32-bit words. */
u32 sceSifQueryBlockTopAddress(u32 block)
{
    if (iopHeapStatus < 0) {
        return 0;
    }
    iopHeapArgs[0] = block;
    if (sceSifCallRpc(iopHeapRpcClient, 8, 0, iopHeapArgs, 4,
                      &iopHeapResult, 4, NULL, NULL) < 0) {
        return 0xFFFFFFFF;
    }
    return iopHeapResult;
}

u32 sceSifQueryBlockSize(u32 block)
{
    if (iopHeapStatus < 0) {
        return 0;
    }
    iopHeapArgs[0] = block;
    if (sceSifCallRpc(iopHeapRpcClient, 9, 0, iopHeapArgs, 4,
                      &iopHeapResult, 4, NULL, NULL) < 0) {
        return 0xFFFFFFFF;
    }
    return iopHeapResult;
}

/* The load-file RPC sends this complete 0x200-byte shared request region. */
typedef struct lfRequest_t {
    s32 value;
    s32 moduleResult; /* Argument size on buffer-load/stop requests; result on replies. */
    char name[0xFC];
    u8 arguments[0xFC];
} lfRequest_t;

extern lfRequest_t lfRequest;
extern rpcClient lfRpcClient;
extern s32 _lf_bind(void);
extern s32 _lf_version(void);
extern char *strncpy(char *destination, const char *source, size_t count);

extern s32 lfBindStatus;
extern u8 lfDetectedVersion[0x18];
extern u8 kernelBuiltinVersion[4];
extern void *lfFallbackVersion;
extern void *memcpy(void *destination, const void *source, size_t size);
extern s32 memcmp(const void *a, const void *b, size_t size);

s32 _lf_bind(void)
{
    s32 delay;

    if (lfBindStatus < 0) {
        for (;;) {
            if (sceSifBindRpc(&lfRpcClient, 0x80000006, 0) < 0) {
                return -1;
            }
            if (lfRpcClient.server != NULL) {
                lfBindStatus = 0;
                if (sceSifCallRpc(&lfRpcClient, 0xFF, 0, NULL, 0,
                                  &lfRequest, 4, NULL, NULL) < 0) {
                    return -65537;
                }
                memcpy(lfDetectedVersion, &lfRequest, 4);
                return 0;
            }
            for (delay = 0x100000; delay != -1; delay--) {
            }
        }
    }
    return 0;
}

s32 _lf_version(void)
{
    const void *builtin = kernelBuiltinVersion;
    const void *detected = lfDetectedVersion;
    s32 mismatch = 0;

    if (memcmp(detected, builtin, 4) != 0) {
        if (memcmp(detected, lfFallbackVersion, 4) != 0) {
            mismatch = memcmp(builtin, lfFallbackVersion, 4) != 0;
        }
    }
    return mismatch;
}

extern void *memset(void *destination, s32 value, size_t size);

s32 sceSifLoadFileReset(void)
{
    lfBindStatus = -1;
    memset(lfDetectedVersion, 0, 4);
    return 0;
}

s32 _sceSifLoadModuleBuffer(void *buffer, s32 argumentSize, const void *arguments, s32 *moduleResult)
{
    s32 result;

    if (_lf_bind() < 0) {
        return -65536;
    }
    if (_lf_version() != 0) {
        return -65540;
    }
    lfRequest.value = EE_ADDR_S(buffer);
    if (arguments != NULL) {
        if (argumentSize > 0xFC) {
            memcpy(lfRequest.arguments, arguments, 0xFC);
            lfRequest.moduleResult = 0xFC;
        } else {
            memcpy(lfRequest.arguments, arguments, argumentSize);
            lfRequest.moduleResult = argumentSize;
        }
    } else {
        lfRequest.moduleResult = 0;
    }
    if (sceSifCallRpc(&lfRpcClient, 6, 0, &lfRequest, 0x200,
                      &lfRequest, 8, NULL, NULL) < 0) {
        return -65537;
    }
    result = lfRequest.value;
    *moduleResult = lfRequest.moduleResult;
    return result;
}

s32 sceSifStopModule(s32 moduleId, s32 argumentSize, const void *arguments, s32 *moduleResult)
{
    s32 result;

    if (_lf_bind() < 0) {
        return -65536;
    }
    if (_lf_version() != 0) {
        return -65540;
    }
    lfRequest.value = moduleId;
    if (arguments != NULL) {
        if (argumentSize > 0xFC) {
            memcpy(lfRequest.arguments, arguments, 0xFC);
            lfRequest.moduleResult = 0xFC;
        } else {
            memcpy(lfRequest.arguments, arguments, argumentSize);
            lfRequest.moduleResult = argumentSize;
        }
    } else {
        lfRequest.moduleResult = 0;
    }
    if (sceSifCallRpc(&lfRpcClient, 7, 0, &lfRequest, 0x200,
                      &lfRequest, 8, NULL, NULL) < 0) {
        return -65537;
    }
    result = lfRequest.value;
    *moduleResult = lfRequest.moduleResult;
    return result;
}

s32 sceSifUnloadModule(s32 moduleId)
{
    if (_lf_bind() < 0) {
        return -65536;
    }
    if (_lf_version() != 0) {
        return -65540;
    }
    lfRequest.value = moduleId;
    if (sceSifCallRpc(&lfRpcClient, 8, 0, &lfRequest, 4,
                      &lfRequest, 4, NULL, NULL) < 0) {
        return -65537;
    }
    return lfRequest.value;
}

s32 sceSifSearchModuleByName(const char *name)
{
    if (_lf_bind() < 0) {
        return -65536;
    }
    if (_lf_version() != 0) {
        return -65540;
    }
    strncpy(lfRequest.name, name, 0xFC);
    lfRequest.name[0xFB] = 0;
    if (sceSifCallRpc(&lfRpcClient, 9, 0, &lfRequest, 512,
                      &lfRequest, 4, NULL, NULL) < 0) {
        return -65537;
    }
    return lfRequest.value;
}

s32 sceSifSearchModuleByAddress(void *address)
{
    if (_lf_bind() < 0) {
        return -65536;
    }
    if (_lf_version() != 0) {
        return -65540;
    }
    lfRequest.value = EE_ADDR_S(address);
    if (sceSifCallRpc(&lfRpcClient, 10, 0, &lfRequest, 4,
                      &lfRequest, 4, NULL, NULL) < 0) {
        return -65537;
    }
    return lfRequest.value;
}

s32 sceSifLoadModuleBuffer(void *buffer, s32 argumentSize, const void *arguments)
{
    s32 moduleResult;

    return _sceSifLoadModuleBuffer(buffer, argumentSize, arguments, &moduleResult);
}

s32 sceSifLoadStartModuleBuffer(void *buffer, s32 argumentSize, const void *arguments,
                                s32 *moduleResult)
{
    return _sceSifLoadModuleBuffer(buffer, argumentSize, arguments, moduleResult);
}

s32 _sceSifLoadModule(const char *path, s32 argumentSize, const void *arguments,
                      s32 *moduleResult, s32 command)
{
    s32 result;

    if (_lf_bind() < 0) {
        return -65536;
    }
    if (_lf_version() != 0) {
        return -65540;
    }
    strncpy(lfRequest.name, path, 0xFC);
    lfRequest.name[0xFB] = 0;
    if (arguments != NULL) {
        if (argumentSize > 0xFC) {
            memcpy(lfRequest.arguments, arguments, 0xFC);
            lfRequest.value = 0xFC;
        } else {
            memcpy(lfRequest.arguments, arguments, argumentSize);
            lfRequest.value = argumentSize;
        }
    } else {
        lfRequest.arguments[0] = 0;
        lfRequest.value = 0;
    }
    if (sceSifCallRpc(&lfRpcClient, command, 0, &lfRequest, 0x200,
                      &lfRequest, 8, NULL, NULL) < 0) {
        return -65537;
    }
    result = lfRequest.value;
    *moduleResult = lfRequest.moduleResult;
    return result;
}

extern s32 _sceSifLoadModule(const char *path, s32 argumentSize, const void *arguments,
                              s32 *moduleResult, s32 command);

s32 sceSifLoadModule(const char *path, s32 argumentSize, const void *arguments)
{
    s32 moduleResult;

    return _sceSifLoadModule(path, argumentSize, arguments, &moduleResult, 0);
}

s32 sceSifLoadStartModule(const char *path, s32 argumentSize, const void *arguments,
                          s32 *moduleResult)
{
    return _sceSifLoadModule(path, argumentSize, arguments, moduleResult, 0);
}

s32 _sceSifLoadElfPart(const char *path, const char *section, u32 *result, s32 command)
{
    if (_lf_bind() < 0) {
        return -65536;
    }
    if (_lf_version() != 0) {
        return -65540;
    }
    strncpy(lfRequest.name, path, 0xFC);
    lfRequest.name[0xFB] = 0;
    strncpy((char *)lfRequest.arguments, section, 0xFC);
    lfRequest.arguments[0xFB] = 0;
    if (sceSifCallRpc(&lfRpcClient, command, 0, &lfRequest, 0x200,
                      &lfRequest, 16, NULL, NULL) < 0) {
        return -65537;
    }
    if (lfRequest.value == 0) {
        return -65539;
    }
    result[0] = lfRequest.value;
    result[1] = lfRequest.moduleResult;
    return 0;
}

s32 sceSifLoadElfPart(const char *path, const char *section, u32 *result)
{
    return _sceSifLoadElfPart(path, section, result, 1);
}

const char D_003B8828[8] = "....";

const char D_003B8830[8] = "....";

s32 sceSifLoadElf(const char *path, u32 *result)
{
    return _sceSifLoadElfPart(path, "all", result, 1);
}

s32 sceSifGetIopAddr(void *address, void *output, u32 kind)
{
    if (_lf_bind() < 0) {
        return -65536;
    }
    if (kind >= 3) {
        return -65538;
    }
    lfRequest.value = EE_ADDR_S(address);
    lfRequest.moduleResult = kind;
    if (sceSifCallRpc(&lfRpcClient, 3, 0, &lfRequest, 32,
                      &lfRequest, 32, NULL, NULL) < 0) {
        return -65537;
    }
    if (kind == 0) {
        *(u8 *)output = *(u8 *)&lfRequest.value;
    } else if (kind == 1) {
        *(u16 *)output = *(u16 *)&lfRequest.value;
    } else if (kind == 2) {
        *(u32 *)output = lfRequest.value;
    } else {
        return -65538;
    }
    return 0;
}

s32 sceSifSetIopAddr(void *address, const void *input, u32 kind)
{
    if (_lf_bind() < 0) {
        return -65536;
    }
    lfRequest.value = EE_ADDR_S(address);
    lfRequest.moduleResult = kind;
    if (kind == 0) {
        *(u8 *)lfRequest.name = *(const u8 *)input;
    } else if (kind == 1) {
        *(u16 *)lfRequest.name = *(const u16 *)input;
    } else if (kind == 2) {
        *(u32 *)lfRequest.name = *(const u32 *)input;
    } else {
        return -65538;
    }
    {
        s32 status = sceSifCallRpc(&lfRpcClient, 2, 0, &lfRequest, 32,
                               &lfRequest, 16, NULL, NULL);
        /* Keep the selected post-RPC return value in its ABI register.
         * This is a target-specific matching aid, not an original-source claim. */
        FAKEMATCH_ASM_REG(s32, result, "v0") = status < 0 ? -65537 : 0;
        return result;
    }
}

typedef struct sifResetPacket_t {
    u64 packetSize : 8;
    u64 dataSize : 24;
    void *destination;
    u32 command;
    u32 option;
    s32 argumentSize;
    s32 mode;
    char arguments[80];
} sifResetPacket_t;

SIZE_CHECK(sifResetPacket_size_must_be_0x68, sifResetPacket_t, 0x68);

extern sifResetPacket_t sifResetPacket;
extern void sceSifStopDma(void);
extern u32 sceSifGetReg(u32 registerId);
extern s32 sceSifSetReg(u32 registerId, u32 value);
extern void sceSifInitRpc(s32 mode);
extern void sceSifExitRpc(void);

s32 sceSifResetIop(const char *arguments, s32 mode)
{
    sifDmaTransfer_t transfer;
    u32 destination;
    s32 length;

    sceSifStopDma();
    destination = sceSifGetReg(0x80000000);
    sifResetPacket.mode = mode;
    for (length = 0; arguments[length] != 0; length++) {
        sifResetPacket.arguments[length] = arguments[length];
    }
    sifResetPacket.destination = NULL;
    sifResetPacket.argumentSize = length;
    sifResetPacket.command = 0x80000003;
    sifResetPacket.dataSize = 0;
    sifResetPacket.packetSize = 104;
    transfer.source = EE_ADDR(&sifResetPacket);
    transfer.destination = destination;
    transfer.size = 104;
    transfer.attributes = 0x44;
    sceSifWriteBackDCache(&sifResetPacket, 104);
    sceSifSetReg(4, 0x40000);
    if (sceSifSetDma(&transfer, 1) != 0) {
        sceSifSetReg(4, 0x10000);
        sceSifSetReg(4, 0x20000);
        sceSifSetReg(0x80000002, 0);
        sceSifSetReg(0x80000000, 0);
        return 1;
    }
    return 0;
}

extern void sceResetttyinit(void);

s32 sceSifIsAliveIop(void)
{
    s32 alive = 0;

    if (sceSifGetReg(4) & 0x10000) {
        alive = 1;
    }
    return alive;
}

s32 sceSifSyncIop(void)
{
    if (sceSifGetReg(4) & 0x40000) {
        sceResetttyinit();
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifRebootIop);

INCLUDE_ASM(const s32, "sdk/libkernl", DIntr);

INCLUDE_ASM(const s32, "sdk/libkernl", EIntr);

extern s32 mallocSema;
extern s32 frameObjectSema;

void supplement_crt0(void)
{
    rpcSemaParam first;
    rpcSemaParam second;

    first.maxCount = 1;
    first.initCount = 1;
    second.maxCount = 1;
    second.initCount = 1;
    mallocSema = CreateSema(&first);
    frameObjectSema = CreateSema(&second);
}

INCLUDE_ASM(const s32, "sdk/libkernl", Copy);

s32 kCopy(u32 *dst, const u32 *src, u32 count)
{
    u32 i;

    count >>= 2;
    for (i = 0; i < count; i++) {
        *dst++ = *src++;
    }
    return 0;
}

u32 *kFindAddress(u32 *start, const u32 *end, u32 instruction)
{
    while (*start != instruction && start < end) {
        start++;
    }
    if (start >= end) {
        start = NULL;
    }
    return start;
}

INCLUDE_ASM(const s32, "sdk/libkernl", FindAddress);

extern u32 *systemCallTable __attribute__((section(".data")));
extern s32 Copy(void *dst, const void *src, u32 count);

u32 GetSystemCallEntry(s32 index)
{
    u32 entry;

    Copy(&entry, &systemCallTable[index], 4);
    return entry;
}

typedef struct {
    u32 index;
    void (*entry)(void);
} kernelEntryPair_t;

extern kernelEntryPair_t systemCallEntryPairs[2];
extern void setup(u32 index, void *entry);
extern u32 *FindAddress(u32 *start, const u32 *end, u32 instruction);

void GetSystemCallTableEntry(void)
{
    u32 *searchEntry;
    u32 *copyEntry;
    u32 *searchBase;
    u32 *copyBase;

    setup(systemCallEntryPairs[0].index, systemCallEntryPairs[0].entry);
    setup(systemCallEntryPairs[1].index, systemCallEntryPairs[1].entry);
    searchEntry = FindAddress((u32 *)0x80000000, (u32 *)0x80080000,
                              EE_ADDR(kFindAddress));
    copyEntry = FindAddress((u32 *)0x80000000, (u32 *)0x80080000,
                            EE_ADDR(kCopy));
    searchBase = searchEntry - 0x83;
    copyBase = copyEntry - 0x5A;
    while (searchBase != copyBase) {
        if (searchBase < copyBase) {
            searchEntry = FindAddress(searchEntry + 1, (u32 *)0x80080000,
                                      EE_ADDR(kFindAddress));
            searchBase = searchEntry - 0x83;
        } else {
            copyEntry = FindAddress(copyEntry + 1, (u32 *)0x80080000,
                                    EE_ADDR(kCopy));
            copyBase = copyEntry - 0x5A;
        }
    }
    systemCallTable = searchBase;
}

INCLUDE_ASM(const s32, "sdk/libkernl", setup);

extern void supplement_crt0(void);
extern void GetSystemCallTableEntry(void);
extern void InitAlarm(void);
extern s32 InitThread(void);
extern void InitExecPS2(void);
extern void InitTLBFunctions(void);

void _InitSys(void)
{
    supplement_crt0();
    GetSystemCallTableEntry();
    InitAlarm();
    InitThread();
    InitExecPS2();
    InitTLBFunctions();
}

INCLUDE_ASM(const s32, "sdk/libkernl", gcc2_compiled__15);

INCLUDE_ASM(const s32, "sdk/libkernl", Copy_0);

s32 kCopy_0(u32 *dst, const u32 *src, u32 count)
{
    u32 i;

    count >>= 2;
    for (i = 0; i < count; i++) {
        dst[i] = src[i];
    }
    return 0;
}

INCLUDE_ASM(const s32, "sdk/libkernl", GetEntryAddress);

extern void SetOsdConfigParam(const u32 *param);
extern void GetOsdConfigParam(u32 *param);

s32 PatchIsNeeded_0(void)
{
    u32 original;
    u32 changed;

    GetOsdConfigParam(&original);
    changed = (original & 0xFFFF1FFF) | 0x2000;
    SetOsdConfigParam(&changed);
    GetOsdConfigParam(&changed);
    SetOsdConfigParam(&original);
    return ((changed >> 13) & 7) == 0;
}

extern void FlushCache(s32 mode);

extern kernelEntryPair_t execEntryPairs[3];
extern const u8 execPs2Payload[0x7A8];
extern s32 PatchIsNeeded_0(void);
extern void gcc2_compiled__15(u32 index, void *entry);
extern s32 Copy_0(void *destination, const void *source, u32 size);
extern void *GetEntryAddress(u32 index);

void InitExecPS2(void)
{
    u32 index;
    u32 i;
    void *entry;

    if (PatchIsNeeded_0() == 0) {
        return;
    }
    gcc2_compiled__15(execEntryPairs[0].index, execEntryPairs[0].entry);
    Copy_0((void *)0x80074000, execPs2Payload, 0x7A8);
    FlushCache(0);
    FlushCache(2);
    gcc2_compiled__15(execEntryPairs[1].index, execEntryPairs[1].entry);
    for (i = 2; i < 3; i++) {
        index = execEntryPairs[i].index;
        entry = GetEntryAddress(index);
        gcc2_compiled__15(execEntryPairs[i].index, entry);
    }
}

INCLUDE_ASM(const s32, "sdk/libkernl", gcc2_compiled__16);

s32 kCopy_1(u8 *dst, const u8 *src, u32 count)
{
    u32 i;

    i = 0;
    if (count != 0) {
        do {
            *dst++ = *src++;
            i++;
        } while (i < count);
    }
    return 0;
}

INCLUDE_ASM(const s32, "sdk/libkernl", PatchIsNeeded);

typedef struct execArgumentBlock_t {
    char *arguments[16];
    char strings[1];
} execArgumentBlock_t;
extern void *execArgumentBlock;
extern kernelEntryPair_t setArgEntryPairs[1];
extern void PatchIsNeeded(u32 index, void *entry);
extern s32 gcc2_compiled__16(void *destination, const void *source, u32 size);
extern u32 strlen(const char *text);

char *SetArg(const char *path, s32 count, const char *const *arguments)
{
    char **table = ((execArgumentBlock_t *)execArgumentBlock)->arguments;
    char *cursor = (char *)&table[16];
    char *start = cursor;
    u32 length;

    PatchIsNeeded(setArgEntryPairs[0].index, setArgEntryPairs[0].entry);
    if (count >= 16) {
        count = 15;
    }
    gcc2_compiled__16(table, &cursor, 4);
    table++;
    length = strlen(path) + 1;
    gcc2_compiled__16(cursor, path, length);
    cursor += length;
    if (count > 0) {
        const char *const *argument = arguments;

        do {
            gcc2_compiled__16(table++, &cursor, 4);
            count--;
            length = strlen(*argument) + 1;
            gcc2_compiled__16(cursor, *argument++, length);
            cursor += length;
        } while (count != 0);
    }
    return start;
}

extern void InitTLB(void);

void TerminateLibrary(void)
{
    InitTLB();
}

extern const char execDefaultPath[];
extern s32 _ExecPS2(void *entry, void *gp, s32 count, char **arguments);
extern void libc_2DBAC0(s32 status);
extern void _ExecOSD(s32 count, char **arguments);

s32 ExecPS2(void *entry, void *gp, s32 count, const char *const *arguments)
{
    SetArg(execDefaultPath, count, arguments);
    TerminateLibrary();
    return _ExecPS2(entry, gp, count,
                    &((execArgumentBlock_t *)execArgumentBlock)->arguments[1]);
}

extern void libc_2DBAE0(const char *path, s32 count, char **arguments);

void LoadExecPS2(const char *path, s32 count, char **arguments)
{
    char *packedPath = SetArg(path, count, (const char *const *)arguments);
    TerminateLibrary();
    libc_2DBAE0(packedPath, count,
                  &((execArgumentBlock_t *)execArgumentBlock)->arguments[1]);
}

void Exit(s32 status)
{
    TerminateLibrary();
    libc_2DBAC0(status);
}

void ExecOSD(s32 count, const char *const *arguments)
{
    SetArg(execDefaultPath, count, arguments);
    TerminateLibrary();
    _ExecOSD(count, &((execArgumentBlock_t *)execArgumentBlock)->arguments[1]);
}

INCLUDE_ASM(const s32, "sdk/libkernl", gcc2_compiled__17);

INCLUDE_ASM(const s32, "sdk/libkernl", Copy_1);

s32 kCopy_2(u32 *dst, const u32 *src, u32 count)
{
    u32 i;

    count >>= 2;
    for (i = 0; i < count; i++) {
        dst[i] = src[i];
    }
    return 0;
}

INCLUDE_ASM(const s32, "sdk/libkernl", GetEntryAddress_0);

extern kernelEntryPair_t alarmEntryPairs[8];
extern kernelEntryPair_t tlbEntryPairs[8];
extern void *execArgumentBlock;
extern const u8 alarmPayload[0x740];
extern const u8 alarmSmallPayload[0x28];
extern const u8 TLBFunctionsImage[0x330];
extern void gcc2_compiled__17(u32 index, void *entry);
extern s32 Copy_1(void *destination, const void *source, u32 size);
extern s32 Copy_2(void *destination, const void *source, u32 size);
extern void *GetEntryAddress_0(u32 index);
extern void *GetEntryAddress_1(u32 index);
extern void setup_0(u32 index, void *entry);

void InitAlarm(void)
{
    u32 index;
    u32 i;
    void *entry;

    if ((*(volatile u32 *)0x10001810 & 0x100) != 0) {
        return;
    }
    gcc2_compiled__17(alarmEntryPairs[0].index, alarmEntryPairs[0].entry);
    Copy_1((void *)0x80076000, alarmPayload, 0x740);
    Copy_1((void *)0x82000, alarmSmallPayload, 0x28);
    FlushCache(0);
    FlushCache(2);
    gcc2_compiled__17(alarmEntryPairs[1].index, alarmEntryPairs[1].entry);
    for (i = 2; i < 8; i++) {
        index = alarmEntryPairs[i].index;
        entry = GetEntryAddress_0(index);
        gcc2_compiled__17(alarmEntryPairs[i].index, entry);
    }
}

extern s32 Deci2Call(s32 command, void *packet);
extern u8 deci2PacketStorage[];

typedef void (*deci2Handler_t)(s32 event, s32 parameter, void *argument);

typedef struct deci2Registration_t {
    u32 protocol;
    void *argument;
    deci2Handler_t handler;
    void *storage;
} deci2Registration_t;

s32 sceDeci2Open(u16 protocol, void *argument, deci2Handler_t handler)
{
    deci2Registration_t packet;

    packet.protocol = protocol;
    packet.argument = argument;
    packet.handler = handler;
    packet.storage = (void *)(EE_ADDR(deci2PacketStorage) | 0x20000000);
    return Deci2Call(1, &packet);
}

s32 sceDeci2Close(s32 handle)
{
    s32 packet[1];

    packet[0] = handle;
    return Deci2Call(2, packet);
}

s32 sceDeci2ReqSend(s32 handle, s8 destination)
{
    s32 packet[2];

    packet[0] = handle;
    packet[1] = destination;
    return Deci2Call(3, packet);
}

s32 sceDeci2Poll(s32 handle)
{
    s32 packet[1];

    packet[0] = handle;
    return Deci2Call(4, packet);
}

typedef struct deci2Transfer_t {
    s32 handle;
    void *buffer;
    u32 length;
} deci2Transfer_t;

s32 sceDeci2ExRecv(s32 handle, void *buffer, u16 length)
{
    deci2Transfer_t packet;

    packet.handle = handle;
    packet.buffer = buffer;
    packet.length = length;
    return Deci2Call(-5, &packet);
}

s32 sceDeci2ExSend(s32 handle, void *buffer, u16 length)
{
    deci2Transfer_t packet;

    packet.handle = handle;
    packet.buffer = buffer;
    packet.length = length;
    return Deci2Call(-6, &packet);
}

s32 sceDeci2ExReqSend(s32 handle, s8 destination)
{
    s32 packet[2];

    packet[0] = handle;
    packet[1] = destination;
    return Deci2Call(-7, packet);
}

s32 sceDeci2ExLock(s32 handle)
{
    s32 packet[1];

    packet[0] = handle;
    return Deci2Call(-8, packet);
}

s32 sceDeci2ExUnLock(s32 handle)
{
    s32 packet[1];

    packet[0] = handle;
    return Deci2Call(-9, packet);
}

s32 kputs(const char *text)
{
    const char *packet[1];

    packet[0] = text;
    return Deci2Call(16, packet);
}

/* Handler entries stay opaque: callers supply code addresses, and this unit
 * does not establish a specific handler function signature. */
extern void *tlbHandler;
extern void *debugHandlers[16];
extern void _kTLBException();
extern void _kDebugException();
extern void *SetVTLBRefillHandler(s32 index, void *handler);
extern void *SetVCommonHandler(s32 index, void *handler);

INCLUDE_ASM(const s32, "sdk/libkernl", SetTLBHandler);
INCLUDE_ASM(const s32, "sdk/libkernl", func_00313630);

void *SetDebugHandler(s32 index, void *handler)
{
    void *previous;

    if (((u32)index - 1) >= 13) {
        return (void *)-1;
    }

    previous = debugHandlers[index];
    debugHandlers[index] = handler;
    if (((u32)index - 1) < 3) {
        SetVTLBRefillHandler(index, _kDebugException);
    } else {
        SetVCommonHandler(index, _kDebugException);
    }
    return previous;
}

INCLUDE_ASM(const s32, "sdk/libkernl", Copy_2);

INCLUDE_ASM(const s32, "sdk/libkernl", kCopy_3);
INCLUDE_ASM(const s32, "sdk/libkernl", func_00313730);

INCLUDE_ASM(const s32, "sdk/libkernl", GetEntryAddress_1);

INCLUDE_ASM(const s32, "sdk/libkernl", setup_0);

void InitTLBFunctions(void)
{
    u32 index;
    u32 i;
    void *entry;

    setup_0(tlbEntryPairs[0].index, tlbEntryPairs[0].entry);
    Copy_2((void *)0x80075000, TLBFunctionsImage, 0x330);
    FlushCache(0);
    FlushCache(2);
    setup_0(tlbEntryPairs[1].index, tlbEntryPairs[1].entry);
    setup_0(tlbEntryPairs[2].index, tlbEntryPairs[2].entry);
    for (i = 3; i < 8; i++) {
        index = tlbEntryPairs[i].index;
        entry = GetEntryAddress_1(index);
        setup_0(tlbEntryPairs[i].index, entry);
    }
    execArgumentBlock = GetEntryAddress_1(3);
}

INCLUDE_ASM(const s32, "sdk/libkernl", PutTLBEntry);

INCLUDE_ASM(const s32, "sdk/libkernl", iPutTLBEntry_0);

INCLUDE_ASM(const s32, "sdk/libkernl", _SetTLBEntry);

extern s32 _SetTLBEntry(u32 index, u32 pageMask, u32 entryHi, u32 entryLo0, u32 entryLo1);

/* Existing retail symbol is retained; this is the checked indexed setter,
 * not the separate interrupt-context PutTLBEntry syscall stub. */
s32 iPutTLBEntry(u32 index, u32 pageMask, u32 entryHi, u32 entryLo0, u32 entryLo1)
{
    if (index - 13U >= 35U) {
        return -1;
    }
    return _SetTLBEntry(index, pageMask, entryHi, entryLo0, entryLo1);
}

INCLUDE_ASM(const s32, "sdk/libkernl", iSetTLBEntry);

INCLUDE_ASM(const s32, "sdk/libkernl", GetTLBEntry);

INCLUDE_ASM(const s32, "sdk/libkernl", iGetTLBEntry);

INCLUDE_ASM(const s32, "sdk/libkernl", ProbeTLBEntry);

INCLUDE_ASM(const s32, "sdk/libkernl", iProbeTLBEntry);

INCLUDE_ASM(const s32, "sdk/libkernl", ExpandScratchPad);

INCLUDE_ASM(const s32, "sdk/libkernl", InitTLB);

const char execDefaultPath[8] = "";

typedef struct {
    u32 pageMask;
    u32 entryHi;
    u32 entryLo0;
    u32 entryLo1;
} kernelTlbEntry_t;

/* InitTLB32MB loads the kernel and default entries, stores how many it wrote in
 * loadedCount, then loads the extended ones. */
typedef struct {
    s32 kernelCount;
    s32 defaultCount;
    s32 extendedCount;
    s32 loadedCount;
    kernelTlbEntry_t *kernelEntries;
    kernelTlbEntry_t *defaultEntries;
    kernelTlbEntry_t *extendedEntries;
} kernelTlbTable_t;

extern kernelTlbEntry_t tlbKernelEntries[13];
extern kernelTlbEntry_t tlbDefaultEntries[18];
extern kernelTlbEntry_t tlbExtendedEntries[8];
extern kernelTlbTable_t tlbTable;

INCLUDE_ASM(const s32, "sdk/libkernl", InitTLB32MB);

INCLUDE_ASM(const s32, "sdk/libkernl", _kTLBException);
INCLUDE_ASM(const s32, "sdk/libkernl", _kExitTLBHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", _kDebugException);
