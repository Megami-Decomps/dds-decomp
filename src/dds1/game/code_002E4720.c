#include "common.h"

extern char D_003B4690[]; /* "cdrom0:\\IRX\\DEV9.IRX;1 resident fail.\n", followed by padding no C emits */

typedef struct SifCommand {
    s32 source;  /* 0x0 */
    s32 end;     /* 0x4 */
    s32 argument; /* 0x8 */
    u32 command; /* 0xC */
} SifCommand;

typedef struct CmdPkt {
    s32 unk0; /* 0x0 */
    s16 unk4; /* 0x4 */
    u16 unk6; /* 0x6 */
    s16 unk8; /* 0x8 */
    u16 unkA; /* 0xA */
    s32 unkC; /* 0xC */
} CmdPkt;

typedef struct DevRequest {
    s32 handle;
    s16 flags;
    u16 count;
    s16 stride;
    s16 mode;
    s32 buffer;
} DevRequest;

typedef struct DevState {
    struct DevState *next; /* 0x0 */
    struct DevState *previous; /* 0x4 */
    struct DevState *workerNext; /* 0x8 */
    struct DevState *workerPrev; /* 0xC */
    void *resource; /* 0x10 */
    u8 workerIndex; /* 0x14 */
    u8 operation; /* 0x15 */
    s8 state; /* 0x16 */
    u8 pad17; /* 0x17 */
    s32 operationArg; /* 0x18 */
    s32 requestExtra; /* 0x1C */
    void *requestData; /* 0x20 */
    s32 options; /* 0x24 */
    s32 resourceId; /* 0x28 */
    s32 result; /* 0x2C */
    s32 transferred; /* 0x30 */
    u8 pad34[4]; /* 0x34 */
    void (*callback)(struct DevState *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4); /* 0x38 */
    s32 callbackContext; /* 0x3C */
} DevState;

#define SDF_DEV_STATE_ACTIVE 7
#define SDF_DEV_STATE_INACTIVE 9

typedef struct DevWorkerEntry {
    s32 handle; /* 0x00: thread ID at sdfDeviceWorkerEntries, semaphore ID at D_00398864 */
    s32 semaphore; /* 0x04 */
    struct DevState *first; /* 0x08 */
    struct DevState *last; /* 0x0C */
    u8 pad10[8];
} DevWorkerEntry;

extern u8 sdfDevicePriorityOverrideTicks;
extern u8 D_003BD42E;
extern u8 D_003BD3F0;
extern s16 D_003BD420;
extern DevState *D_003BD424;
extern DevState *D_003BD428;
extern s32 sdfDeviceWorkerPriority;

extern u32 sdfDevOperationReplyValue;

extern u32 sdfDevControlReplyValue;

extern s32 sdfDevReplySemaphore;

extern s32 func_002E5158(u32, u8 *, u32);
extern void func_002E5D98(s32 arg0);

extern s32 sdfOpenDiscFileRecord;
extern u32 sdfDiscSemaphore;

extern s32 sdfDiscRequestPending;
extern u32 sdfDiscRequestSemaphore;

extern u32 D_003BDA44;

extern u32 D_003987E0[];
extern char D_00398820[];
extern DevWorkerEntry sdfDeviceWorkerEntries[];
extern DevWorkerEntry D_00398864[];
extern u8 sdfPfsPathPrefix[];
extern f32 sdfNormalizedAsinSamples[];
extern char *func_002E5970(char *path);

extern s32 SignalSema(s32 sema);
extern s32 WaitSema(s32 sema);
extern s32 ChangeThreadPriority(s32 tid, s32 prio);
extern void sdfDevUnlinkAndFreeState(DevState *arg0);
extern void sdfDevRecycleCompletedState(DevState *arg0);
extern DevState *sdfDevCreateCallbackState(s32 arg0,
                                void (*callback)(DevState *, s32, s32, s32, s32), s32 arg2);
extern s32 func_0030EB78(s32 arg0);
extern s32 func_00312C08(DevState *arg0);
extern void func_003110C8(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void EIntr(void);
extern void sceCdPowerOff(void *arg0);
extern s32 sdfCreateSemaphore(s32 arg0, s32 arg1, s32 arg2);
extern void sdfPanicHaltPrintf(const char *arg0, ...) __attribute__((noreturn));
extern void *sdfAllocSizeClassBlock(s32 size);
extern void sdfReleaseChipBlock(void *ptr);
extern void *sdfAllocAndClearQuadwords(s32 size);
extern s32 sdfAllocGeneralBlock(s32 size);
extern void func_002D0750(s32 arg0, s32 arg1);
extern s32 sdfResourceRetainAddress(s32 arg0);
extern void sdfDecrementAllocationReferenceCount(s32 arg0);
extern u32 strlen(const char *s);
extern void func_002F4190(u32 arg0);
extern u32 sdfDiscType;
extern u8 D_003BD460[];
extern u8 D_003BD468[];
extern s32 GetThreadId(void);
extern void sceSifSetRpcQueue(void *, s32);
extern void sceSifRegisterRpc(void *, s32, void *, void *, s32, s32, void *);
extern void sceSifRpcLoop(void *);
extern u8 sdfDevRpcBuffer[];
extern void sdfSleepWithAlarm(s32);
void sdfDevWaitForDisc(void);
extern s32 sdfDiscLoadFilename;
extern void func_002F3F98(s32);
extern s32 func_002F4258(void);
extern s32 sceCdSearchFile(void *, s32);
extern u16 sdfDefaultDevRequestOptions;
extern s32 func_002E69F0(s32, void **);
extern void sdfDevEnqueueStateAndWakeWorker(DevState *);
extern DevState *sdfDevAllocState(void *, s32, s32,
                                void (*)(DevState *, s32, s32, s32, s32), s32);
extern u8 sdfDevModuleLoaded;
extern u8 D_003BD478;
extern s32 func_00312618(const char *, s32, void *, s32 *);
extern void func_003003F0(const char *);

extern void func_002E4720(s32 arg0, s32 arg1, void *args);

extern s32 func_00305B08(char *dst, const char *fmt, void *args);

extern void func_002E4B80(const char *text);

extern u8 sdfDiscReadMode;

extern s32 sdfDiscReadPosition;

extern s32 func_002F4588(s32 arg, u8 *options);

extern s32 sceCdStatus(void);

extern s32 func_002F4658(s32 size, s32 buffer, s32 arg2, s32 *status);

extern s32 func_002F4620(void);

extern s32 D_003BDA4C;

extern s32 D_003BDA54;

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4720);

void sdfFormatSifPacket(s32 packet, s32 format, ...) {
    __builtin_va_list args;

    __builtin_stdarg_start(args, format);
    func_002E4720(packet, format, args);
}

void sdfCreateFormattedSifCommand(s32 source, s32 end, s32 argument, s32 index, const char *fmt, ...) {
    SifCommand packet;
    __builtin_va_list args;

    sdfPktInit(&packet, source, end, argument, index);
    __builtin_stdarg_start(args, fmt);
    func_002E4720(&packet, fmt, args);
}

void sdfPktSetCmd(SifCommand *packet, s32 index) {
    packet->command = D_003987E0[index];
}

void sdfPktInit(SifCommand *packet, s32 source, s32 end, s32 argument, s32 index) {
    packet->source = source;
    packet->end = end;
    packet->argument = argument;
    sdfPktSetCmd(packet, index);
}

void *sdfRpcBufHandler(s32 unused, SifCommand *packet) {
    s32 size = packet->end - packet->source;

    if (size > 0) {
        memcpy(packet, (void *)packet->source, size);
        return packet;
    }
    return NULL;
}

void sdfDevStartRpcServer(void) {
    u8 queue[0x20];
    u8 server[0x50];
    sceSifSetRpcQueue(queue, GetThreadId());
    sceSifRegisterRpc(server, 0x32647270, sdfRpcBufHandler, sdfDevRpcBuffer, 0, 0, queue);
    sceSifRpcLoop(queue);
}

typedef struct SifClient {
    u8 pad00[0x24];
    void *server; /* 0x24: non-NULL once the bind succeeded */
    u8 pad28[8];
} SifClient;

extern SifClient D_003F9B50;
extern u8 D_003BD3C8;
extern s32 sdfCreateThreadWithAllocatedWorkspace();
extern s32 func_002CF930(void);
extern s32 sdfGetElapsedTimerTicks(s32);
extern s32 sceSifMBindRpc(void *, s32, s32);
extern void _StartThread(s32, s32);

/* Start the RPC server thread and bind to the remote service, polling every 4 timer ticks. */
void sdfStartDevRpcServerAndBindClient(void) {
    s32 start;

    _StartThread(sdfCreateThreadWithAllocatedWorkspace(sdfDevStartRpcServer, 0x1000, 0x4C), 0);
    while (sceSifMBindRpc(&D_003F9B50, 0x646E7270, 0) >= 0) {
        if (D_003F9B50.server != NULL) {
            D_003BD3C8 = 1;
            break;
        }
        start = func_002CF930();
        while (sdfGetElapsedTimerTicks(start) < 4) {
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4B80);

s32 sdfPrintFormattedDevMessage(const char *fmt, ...) {
    char buffer[0x100];
    __builtin_va_list args;
    s32 length;

    __builtin_stdarg_start(args, fmt);
    length = func_00305B08(buffer, fmt, args);
    func_002E4B80(buffer);
    return length;
}

u32 sdfDevGetLoadedFileAddress(void) {
    return D_003BDA44;
}

void sdfDevWaitForDisc(void) {
    u8 file[0x30];
    s32 status;
    WaitSema(sdfDiscSemaphore);
    for (;;) {
        sdfSleepWithAlarm(100);
        func_002F3F98(0);
        status = func_002F4258();
        if (sdfDiscType == 2) {
            if (status != 20) {
                continue;
            }
        } else if (status != 18) {
            continue;
        }
        if (sceCdSearchFile(file, sdfDiscLoadFilename) != 0) {
            break;
        }
    }
    SignalSema(sdfDiscSemaphore);
}

void sdfDevSeekDiscRequest(s32 request) {
    u8 options[4];
    s32 ready;

    options[0] = 0;
    if (sdfDiscReadMode == 1) {
        options[1] = 0;
    } else {
        options[1] = 1;
    }
    options[2] = 0;
    options[3] = 0;
    for (;;) {
        WaitSema(sdfDiscSemaphore);
        ready = func_002F4588(request, options);
        SignalSema(sdfDiscSemaphore);
        if (ready != 0) {
            break;
        }
        if (sceCdStatus() == 1) {
            sdfDevWaitForDisc();
        } else {
            sdfSleepWithAlarm(100);
        }
    }
    sdfDiscReadPosition = request;
}

void sdfDevSignalPendingSemaphore(void) {
    if (sdfDiscRequestPending != 0) {
        SignalSema(sdfDiscRequestSemaphore);
        sdfDiscRequestPending = 0;
    }
}

void sdfDevSeekDiscWithRequestGate(s32 request) {
    u8 options[4];
    s32 ready;

    options[0] = 0;
    if (sdfDiscReadMode == 1) {
        options[1] = 0;
    } else {
        options[1] = 1;
    }
    options[2] = 0;
    options[3] = 0;
    for (;;) {
        WaitSema(sdfDiscSemaphore);
        sdfDiscRequestPending = 1;
        WaitSema(sdfDiscRequestSemaphore);
        ready = func_002F4588(request, options);
        SignalSema(sdfDiscSemaphore);
        if (ready != 0) {
            break;
        }
        if (sceCdStatus() == 1) {
            sdfDevWaitForDisc();
        } else {
            sdfSleepWithAlarm(100);
        }
    }
    sdfDiscReadPosition = request;
}

void sdfDevReadDiscUntilComplete(s32 size, s32 buffer) {
    s32 status;
    s32 result;

    for (;;) {
        WaitSema(sdfDiscSemaphore);
        result = func_002F4658(size, buffer, 1, &status);
        SignalSema(sdfDiscSemaphore);
        if (status == 0 && result == size) {
            break;
        }
        if (sceCdStatus() == 1) {
            sdfDevWaitForDisc();
            sdfDevSeekDiscRequest(sdfDiscReadPosition);
        } else {
            WaitSema(sdfDiscSemaphore);
            func_002F4620();
            SignalSema(sdfDiscSemaphore);
            sdfDevSeekDiscRequest(sdfDiscReadPosition);
        }
    }
    sdfDiscReadPosition += size;
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4FB0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5158);

s32 sdfDevOpenDiscFileAndGetSize(const char *name) {
    s32 result;
    s32 request[4];
    s32 file;

    file = func_002E5158((u32)name, (u8 *)request, (u32)&sdfDiscReadMode);
    result = -1;
    if (file != 0) {
        D_003BDA4C = *(s32 *)(file + 8);
        sdfOpenDiscFileRecord = file;
        D_003BDA54 = 0;
        if (name[1] == 0x76 || name[1] == 0x56) {
            sdfDevSeekDiscWithRequestGate(request[0]);
        } else {
            sdfDevSeekDiscRequest(request[0]);
        }
        result = *(s32 *)(file + 8);
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5398);

void sdfServicePendingOperationUnderSemaphore(void) {
    if (sdfOpenDiscFileRecord != 0) {
        WaitSema(sdfDiscSemaphore);
        func_002F4620();
        SignalSema(sdfDiscSemaphore);
        sdfOpenDiscFileRecord = 0;
    }
}

s32 sdfPacketExists(u32 request) {
    u8 buffer[16];

    return func_002E5158(request, buffer, 0) != 0;
}

s32 sdfPktQuery(u32 request) {
    u8 buffer[16];
    s32 packet;

    packet = func_002E5158(request, buffer, 0);
    if (packet != 0) {
        return *(s32 *)(packet + 8);
    }
    return -1;
}

s32 sdfDevGetFileSize(void) {
    if (sdfOpenDiscFileRecord == 0) {
        sdfPanicHaltPrintf("file didn't open.");
    }
    return *(s32 *)(sdfOpenDiscFileRecord + 8);
}

extern s32 func_0030E8F0();
extern s32 func_0030ECF8(s32, s32, s32);
extern s32 func_002FF538(s32);
extern s32 func_0030EF30(s32, s32, s32);
extern s32 func_0030F190(s32, s32, s32);
extern void FlushCache(s32);
extern char D_003B4578[];

void sdfDevLoadWholeFile(s32 name) {
    s32 fd = func_0030E8F0(name, 1);
    s32 size;
    s32 buffer;

    if (fd < 0) {
        sdfPanicHaltPrintf(D_003B4578, name);
    }
    size = func_0030ECF8(fd, 0, 2);
    func_0030ECF8(fd, 0, 0);
    buffer = func_002FF538(size);
    D_003BDA44 = buffer;
    func_0030EF30(fd, buffer, size);
    func_0030EB78(fd);
    FlushCache(0);
}

extern u32 D_003BDA3C;
extern u32 D_003BDA40;
extern s32 sceSifAllocIopHeap(s32);
extern void func_002F4558(s32, s32, s32);
extern void sdfDevLoadWholeFile(s32);
extern char D_003B4578[];

void sdfDevStartLoad(s32 name, s32 mode) {
    u32 file[12];
    s32 heap;

    sdfDiscLoadFilename = name;
    sdfDevLoadWholeFile(mode);
    if (sceCdSearchFile(file, name) == 0) {
        sdfPanicHaltPrintf(D_003B4578, name);
    }
    D_003BDA40 = file[0];
    heap = sceSifAllocIopHeap(0x28010);
    D_003BDA3C = heap;
    func_002F4558(0x50, 5, (heap + 15) & -16);
}

void sdfInitDeviceSemaphores(void) {
    sdfDiscSemaphore = sdfCreateSemaphore(1, 0xff, 0);
    sdfDiscRequestSemaphore = sdfCreateSemaphore(0, 0xff, 0);
    sdfDiscRequestPending = 0;
    func_002F3CB8(0);
    func_002F4190(sdfDiscType);
}

extern char sdfDiscPathPrefix[];

/* Convert a relative disc path to the drive's uppercase backslash form with ;1 suffix. */
void sdfDevMakeDiscPath(char *dst, char *src) {
    s32 c;
    memcpy(dst, sdfDiscPathPrefix, 8);
    dst += 7;
    c = *src++;
    while (c != 0) {
        if (c == '/') {
            c = '\\';
        }
        if (c >= 'a' && c <= 'z') {
            c -= 0x20;
        }
        *dst++ = c;
        c = *src++;
    }
    dst[0] = ';';
    dst[1] = '1';
    dst[2] = 0;
}


typedef struct Bytes7 {
    s8 b[7];
} Bytes7;

extern Bytes7 D_003BD400[];
extern char *strcpy(char *, char *);
extern char *strcat(char *, char *);

char *sdfDevBuildPath(char *dst, char *src) {
    if (*src == 0x2F) {
        strcpy(dst, D_00398820);
        return strcat(dst, src);
    }
    *(Bytes7 *)dst = D_003BD400[0];
    return strcat(dst, src);
}

void sdfPathPrefixCat(char *destination, char *path) {
    memcpy(destination, sdfPfsPathPrefix, 6);
    strcat(destination, path);
}

u32 func_002E5958(void) {
    return 0;
}

u32 func_002E5960(void) {
    return 0;
}

u32 func_002E5968(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5970);

void sdfDevCommandReplyCallback(DevState *state, s32 event, s32 unused, s32 value, s32 context) {
    if (event != 3) {
        if (event == 4) {
            sdfDevControlReplyValue = value;
        }
    } else {
        sdfDevOperationReplyValue = value;
    }
    SignalSema(sdfDevReplySemaphore);
}
s32 sdfDevReactivate(DevState *);

DevState *sdfDevCreateCommandState(s32 command) {
    DevState *state;
    if (sdfDevReplySemaphore < 0) {
        sdfDevReplySemaphore = sdfCreateSemaphore(0, 0x80, 0);
    }
    state = sdfDevCreateCallbackState(command, sdfDevCommandReplyCallback, 0);
    WaitSema(sdfDevReplySemaphore);
    sdfDevReactivate(state);
    return state;
}

s32 sdfPathExists(char *path) {
    char *resolved = func_002E5970(path);
    s32 fd;

    if (*resolved == '/') {
        return sdfPacketExists((u32)resolved);
    }
    fd = func_0030E8F0(resolved, 1);
    func_0030EB78(fd);
    sdfReleaseChipBlock(resolved);
    return fd >= 0;
}

void sdfDevWaitThenReleaseCommandState(DevState *state) {
    sdfDevQueueActiveOperation();
    WaitSema(sdfDevReplySemaphore);
    sdfDevQueueReleaseState(state);
}

void sdfDevQueueReadAndWait(void) {
    sdfDevQueueRead();
    WaitSema(sdfDevReplySemaphore);
}

u32 sdfDevQueueControlAndWait(void) {
    sdfDevQueueControlRequest();
    WaitSema(sdfDevReplySemaphore);
    return sdfDevControlReplyValue;
}

u32 sdfDevQueueOperationAndWait(void) {
    sdfDevQueueOperation();
    WaitSema(sdfDevReplySemaphore);
    return sdfDevOperationReplyValue;
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5CD8);

char *sdfDevGetPathBuffer(void) {
    return D_00398820;
}

void func_002E5D80(s32 value) {
    D_003BD3F0 = value;
    func_002E5D98(value);
}

void func_002E5D98(s32 value) {
    D_003BD42E = value;
}

extern DevState *D_003BD418;
extern DevState *D_003BD41C;
extern s16 D_003BD42C;

/* Queue a request on the global list and on its worker's list; wake the worker when its list was empty. */
void sdfDevEnqueueStateAndWakeWorker(DevState *state) {
    DevWorkerEntry *worker;
    s32 interrupts;
    s32 wake = 0;
    DevState *last;

    interrupts = func_00312C08(state);
    worker = &sdfDeviceWorkerEntries[state->workerIndex];
    if (worker->handle < 0) {
        sdfEnsureDeviceWorkerThreadStarted(state->workerIndex);
    }
    state->previous = D_003BD41C;
    if (D_003BD41C != NULL) {
        D_003BD41C->next = state;
    } else {
        D_003BD418 = state;
    }
    last = worker->last;
    D_003BD41C = state;
    state->workerPrev = last;
    if (last != NULL) {
        last->workerNext = state;
    } else {
        worker->first = state;
        wake = 1;
    }
    worker->last = state;
    D_003BD42C += 1;
    if (interrupts != 0) {
        EIntr();
    }
    if (wake != 0) {
        SignalSema(worker->semaphore);
    }
}

void sdfDevUnlinkAndFreeState(DevState *state) {
    DevState *prev;
    DevState *next;
    s64 interrupts;

    interrupts = func_00312C08(state);
    prev = state->previous;
    next = state->next;
    if (prev == NULL) {
        D_003BD424 = next;
    } else {
        prev->next = next;
    }
    if (next == NULL) {
        D_003BD428 = prev;
    } else {
        next->previous = prev;
    }
    D_003BD420 -= 1;
    if (interrupts != 0) {
        EIntr();
    }
    sdfReleaseChipBlock(state->resource);
    sdfReleaseChipBlock(state);
}


void sdfDevRecycleCompletedState(DevState *state) {
    DevWorkerEntry *worker;
    DevState *prev;
    DevState *next;
    s64 interrupts;

    interrupts = func_00312C08(state);
    prev = state->previous;
    next = state->next;
    if (prev == NULL) {
        D_003BD418 = next;
    } else {
        prev->next = next;
    }
    if (next == NULL) {
        D_003BD41C = prev;
    } else {
        next->previous = prev;
    }
    worker = &sdfDeviceWorkerEntries[state->workerIndex];
    prev = state->workerPrev;
    next = state->workerNext;
    if (prev == NULL) {
        worker->first = next;
    } else {
        prev->workerNext = next;
    }
    if (next == NULL) {
        worker->last = prev;
    } else {
        next->workerPrev = prev;
    }
    state->previous = D_003BD428;
    if (D_003BD428 == NULL) {
        D_003BD424 = state;
    } else {
        D_003BD428->next = state;
    }
    D_003BD428 = state;
    state->next = NULL;
    D_003BD420++;
    D_003BD42C--;
    if (interrupts != 0) {
        EIntr();
    }
    if (D_003BD420 >= 9) {
        sdfDevUnlinkAndFreeState(D_003BD424);
    }
    if (worker->first != NULL) {
        SignalSema(worker->semaphore);
    }
}

void sdfDevRelease(DevState *state) {
    s32 id = state->resourceId;

    state->resourceId = -1;
    if (id >= 0) {
        func_0030EB78(id);
    }
    sdfDevRecycleCompletedState(state);
}

void sdfDevDeactivate(DevState *state, s32 result) {
    state->result = result;
    state->state = SDF_DEV_STATE_INACTIVE;
    sdfDevRelease(state);
    if (state->callback != NULL) {
        state->callback(state, 0, 0, 0, state->callbackContext);
    }
}

INCLUDE_RODATA(const s32, "game/code_002E4720", D_003B4578);

INCLUDE_ASM(const s32, "game/code_002E4720", sdfDevWorkerThread);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E67A8);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E69F0);

DevState *sdfDevAllocState(void *resource, s32 workerIndex, s32 operation,
                        void (*callback)(DevState *, s32, s32, s32, s32), s32 context) {
    DevState *state = (DevState *)sdfAllocAndClearQuadwords(0x40);
    state->resource = resource;
    state->workerIndex = workerIndex;
    state->operation = operation;
    state->state = 0;
    state->resourceId = -1;
    state->callback = callback;
    state->callbackContext = context;
    return state;
}


DevState *sdfDevCreateCallbackState(s32 path, void (*callback)(DevState *, s32, s32, s32, s32),
                        s32 context) {
    void *resource;
    s32 id = func_002E69F0(path, &resource);
    DevState *state;

    if (id < 0) {
        return NULL;
    }
    state = sdfDevAllocState(resource, id, 1, callback, context);
    sdfDevEnqueueStateAndWakeWorker(state);
    return state;
}

DevState *sdfDevCreateModeState(s32 path, void (*callback)(DevState *, s32, s32, s32, s32),
                        s32 context, s32 options) {
    void *resource;
    s32 id = func_002E69F0(path, &resource);
    DevState *state;

    if (id < 0) {
        return NULL;
    }
    state = sdfDevAllocState(resource, id, 2, callback, context);
    state->options = options != 0 ? options : sdfDefaultDevRequestOptions;
    sdfDevEnqueueStateAndWakeWorker(state);
    return state;
}

s32 sdfDevQueueOperation(DevState *state, s32 operationArg, s32 options) {
    if (state->state != SDF_DEV_STATE_ACTIVE) {
        return -1;
    }
    state->operationArg = operationArg;
    state->options = options;
    state->operation = 3;
    SignalSema(D_00398864[state->workerIndex].handle);
    return 0;
}

s32 sdfDevQueueControlRequest(DevState *state) {
    if (state->state != SDF_DEV_STATE_ACTIVE) {
        return -1;
    }
    state->operation = 4;
    SignalSema(D_00398864[state->workerIndex].handle);
    return 0;
}

s32 sdfDevQueueRead(DevState *state, void *data, s32 extra) {
    if (state->state != SDF_DEV_STATE_ACTIVE) {
        return -1;
    }
    state->requestExtra = extra;
    state->operation = 5;
    state->requestData = data;
    SignalSema(D_00398864[state->workerIndex].handle);
    return 0;
}


s32 sdfDevQueueWrite(DevState *state, void *data, s32 extra) {
    if (state->state != SDF_DEV_STATE_ACTIVE) {
        return -1;
    }
    state->requestExtra = extra;
    state->operation = 6;
    state->requestData = data;
    SignalSema(D_00398864[state->workerIndex].handle);
    return 0;
}


s32 sdfDevReactivate(DevState *state) {
    if (state->state != SDF_DEV_STATE_INACTIVE) {
        return -1;
    }
    state->result = 0;
    state->state = SDF_DEV_STATE_ACTIVE;
    return 0;
}

s32 sdfDevQueueActiveOperation(DevState *request) {
    s8 state = request->state;

    if (state != SDF_DEV_STATE_ACTIVE) {
        return -1;
    }
    request->operation = state;
    SignalSema(D_00398864[request->workerIndex].handle);
    return 0;
}

s32 sdfDevQueueReleaseState(DevState *state) {
    if (state->state < 9) {
        if (state->state >= 7) {
            sdfDevUnlinkAndFreeState(state);
            return 0;
        }
    }
    return -1;
}


DevState *sdfDevCreateRequest(s32 path, s32 data, s32 extra,
                        void (*context)(DevState *, s32, s32, s32, s32), s32 callback) {
    void *resource;
    s32 id = func_002E69F0(path, &resource);
    DevState *state;

    if (id < 0) {
        return NULL;
    }
    state = sdfDevAllocState(resource, id, 8, context, callback);
    state->requestExtra = extra;
    state->requestData = (void *)data;
    state->operationArg = 0;
    sdfDevEnqueueStateAndWakeWorker(state);
    return state;
}

DevState *sdfDevOpenRequest(s32 path, s32 data, s32 extra,
                        void (*context)(DevState *, s32, s32, s32, s32),
                        s32 callback, s32 options) {
    void *resource;
    s32 id = func_002E69F0(path, &resource);
    DevState *state;

    if (id < 0) {
        return NULL;
    }
    state = sdfDevAllocState(resource, id, 9, context, callback);
    state->requestExtra = extra;
    state->requestData = (void *)data;
    state->options = options != 0 ? options : sdfDefaultDevRequestOptions;
    state->operationArg = 0;
    sdfDevEnqueueStateAndWakeWorker(state);
    return state;
}

void sdfSetThreadPriorities(s32 priority) {
    DevWorkerEntry *worker;
    u32 index;

    if (sdfDeviceWorkerPriority == priority) {
        return;
    }
    sdfDeviceWorkerPriority = priority;
    worker = sdfDeviceWorkerEntries;
    index = 0;
    do {
        s32 threadId = worker->handle;

        worker++;
        if (threadId >= 0) {
            ChangeThreadPriority(threadId, priority);
        }
        index++;
    } while (index < 4);
}

void sdfRaiseDeviceThreadPriority(void) {
    sdfDevicePriorityOverrideTicks = 3;
    sdfSetThreadPriorities(0x78);
}

void sdfRestoreDeviceThreadPriority(void) {
    sdfSetThreadPriorities(0x48);
}

void sdfTickThreadPriorityOverride(void) {
    u8 val = sdfDevicePriorityOverrideTicks;
    u8 next;

    if (val == 0) {
        return;
    }
    sdfDevicePriorityOverrideTicks = val - 1;
    next = val - 1;
    if (next != 0) {
        return;
    }
    sdfRestoreDeviceThreadPriority();
}

extern void D_002E6538();
extern void sdfDevWorkerThread();

/* Start worker thread `index` if it isn't running; slot 3 runs the alternate entry point. */
void sdfEnsureDeviceWorkerThreadStarted(s32 index) {
    DevWorkerEntry *worker = &sdfDeviceWorkerEntries[index];
    void (*entry)();
    s32 thread;

    thread = worker->handle;
    if (thread < 0) {
        worker->semaphore = sdfCreateSemaphore(0, 0xFF, 0);
        entry = D_002E6538;
        if (index != 3) {
            entry = sdfDevWorkerThread;
        }
        sdfDeviceWorkerPriority = 0x48;
        thread = sdfCreateThreadWithAllocatedWorkspace(entry, 0x4000, 0x48);
        worker->handle = thread;
        _StartThread(thread, (s32)worker);
    }
}

void sdfPowerOffLoop(s32 semaphore) {
    s32 status;

    for (;;) {
        WaitSema(semaphore);
        func_003110C8(D_003BD460, 0x5003, 0, 0, 0, 0);
        func_003110C8(D_003BD468, 0x4806, 0, 0, 0, 0);
        sceCdPowerOff(&status);
    }
}

void sdfPowerOffInterruptCallback(void) {
    iSignalSema();
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E7228);

void sdfLoadDevModule(void) {
    s32 resident;
    if (sdfDevModuleLoaded == 0) {
        D_003BD478 = 0;
        if (func_00312618("cdrom0:\\IRX\\DEV9.IRX;1", 0, NULL, &resident) < 0) {
            func_003003F0("cdrom0:\\IRX\\DEV9.IRX;1 could't load.\n");
            return;
        }
        if (resident != 0) {
            func_003003F0(D_003B4690);
            return;
        }
        D_003BD478 = 1;
        sdfDevModuleLoaded = 1;
    }
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E73F0);

INCLUDE_ASM(const s32, "game/code_002E4720", sdfClearQuadwords);

char *sdfStrDup(const char *text) {
    u32 length;
    char *copy;

    if (text == NULL) {
        return NULL;
    }
    length = strlen(text);
    copy = sdfAllocSizeClassBlock(length + 1);
    memcpy(copy, text, length);
    copy[length] = 0;
    return copy;
}

s32 sdfBcdStrToInt(s32 packedDigits) {
    s32 place = 1;
    s32 acc = 0;

    while (packedDigits > 0) {
        acc += (packedDigits & 0xf) * place;
        packedDigits >>= 4;
        place *= 10;
    }
    return acc;
}

s32 sdfDecimalToPackedDigits(s32 number) {
    s32 shift = 0;
    s32 bcd = 0;

    while (number > 0) {
        s32 quotient = number / 10;
        bcd |= (number - quotient * 10) << shift;
        number = quotient;
        shift += 4;
    }
    return bcd;
}

DevRequest *sdfDevCreateBufferedRequest(s32 count, s32 stride, s32 mode) {
    DevRequest *request = sdfAllocSizeClassBlock(sizeof(*request));

    request->mode = mode;
    request->flags = 0;
    request->count = count;
    request->stride = stride;
    if (count != 0) {
        request->handle = sdfAllocGeneralBlock(stride * count);
        request->buffer = sdfResourceRetainAddress(request->handle);
    } else {
        request->handle = 0;
        request->buffer = 0;
    }
    return request;
}

void sdfDestroyDevRequest(DevRequest *request) {
    sdfReleaseResourceAllocation(request->handle);
    sdfReleaseChipBlock(request);
}

void sdfDevResizeBufferedRequest(DevRequest *request, s32 count);

void sdfDevBufferedRequestGrow(DevRequest *request) {
    if (request->handle == 0) {
        sdfDevResizeBufferedRequest(request, request->mode);
        return;
    }
    sdfDecrementAllocationReferenceCount(request->handle);
    request->count = request->count + request->mode;
    func_002D0750(request->handle, (s16)request->count * request->stride);
    request->buffer = sdfResourceRetainAddress(request->handle);
}

void sdfDevResizeBufferedRequest(DevRequest *request, s32 count) {
    if (request->handle == 0) {
        if (count > 0) {
            request->count = count;
            request->handle = sdfAllocGeneralBlock(request->stride * count);
            request->buffer = sdfResourceRetainAddress(request->handle);
        }
    } else if (count <= 0) {
        sdfReleaseResourceAllocation(request->handle);
        request->handle = 0;
        request->flags = 0;
        request->count = 0;
        request->buffer = 0;
    } else {
        sdfDecrementAllocationReferenceCount(request->handle);
        request->count = count;
        func_002D0750(request->handle, request->stride * count);
        request->buffer = sdfResourceRetainAddress(request->handle);
        if (count < request->flags) {
            request->flags = count;
        }
    }
}


f32 sdfSinPoly(f32 angle) {
    f32 x = angle * 0.15915494f;
    f32 t;
    f32 t2;
    f32 t3;
    f32 t5;
    f32 t7;
    f32 t9;

    x -= (s32)x;
    if (x > 0.5f) {
        x -= 1.0f;
    } else if (x < -0.5f) {
        x += 1.0f;
    }
    if (x > 0.25f) {
        x = 0.5f - x;
    } else if (x < -0.25f) {
        x = -0.5f - x;
    }
    t = x * 3.9999996f;
    t2 = t * t;
    t3 = t2 * t;
    t5 = t3 * t2;
    t7 = t5 * t2;
    t9 = t7 * t2;
    return t * 1.5707963f + t3 * -0.64596367f + t5 * 0.07968968f + t7 * -0.0046737656f + t9 * 0.00015148419f;
}


f32 sdfEvaluateCosineViaSinePhaseShift(f32 angle) {
    return sdfSinPoly(angle + 1.5707963f);
}

/* Binary search for value in a sorted table; returns the interpolated position in 0..1. */
f32 sdfTableInterpolate(f32 value, f32 *table, s32 count) {
    f32 unit = 1 / count;
    s32 lo = 0;
    s32 hi = count;
    s32 mid;
    f32 lower;
    f32 upper;

    do {
        mid = lo + hi;
        mid >>= 1;
        upper = table[mid];
        if (value < upper) {
            hi = mid;
        } else {
            mid++;
            lo = mid;
        }
    } while (lo < hi);
    upper = table[mid];
    lower = 0.0f;
    if (mid != 0) {
        lower = table[mid - 1];
    }
    return mid * unit + (value - lower) * unit / (upper - lower);
}

f32 sdfAtan2Poly(f32 ratio) {
    f32 x2 = ratio * ratio;
    f32 x3 = x2 * ratio;
    f32 x5 = x2 * x3;

    return ratio * 0.99999977f + x3 * -0.33325735f + x5 * 0.19388643f;
}

f32 sdfAtan2(f32 y, f32 x) {
    s32 sx = 0;
    s32 sy;
    f32 r;

    if (x < 0.0f) {
        x = -x;
        sx = 1;
    }
    sy = 0;
    if (y < 0.0f) {
        y = -y;
        sy = 1;
    }
    if (y < x) {
        r = sdfAtan2Poly(y / x);
    } else {
        r = 1.5707963f - sdfAtan2Poly(x / y);
    }
    if (sx != 0) {
        r = 3.1415926f - r;
    }
    if (sy != 0) {
        r = -r;
    }
    return r;
}

f32 sdfAsinTable(f32 x) {
    f32 sign;
    f32 result;

    if (x < 0.0f) {
        x = -x;
        sign = -1.0f;
    } else {
        sign = 1.0f;
    }
    result = x >= 1.0f ? 1.5707963f : sdfTableInterpolate(x, sdfNormalizedAsinSamples, 128) * 1.5707963f;
    return result * sign;
}

f32 sdfAcosTable(f32 x) {
    f32 sign;
    f32 result;

    if (x < 0.0f) {
        x = -x;
        sign = -1.0f;
    } else {
        sign = 1.0f;
    }
    result = 0.0f;
    if (!(x >= 1.0f)) {
        result = (1.0f - sdfTableInterpolate(x, sdfNormalizedAsinSamples, 128)) * 1.5707963f;
    }
    return result * sign;
}

f32 sdfWrapAngle(f32 angle) {
    s32 turns;
    if (angle > 3.1415926f) {
        turns = (s32)(angle / 6.2831852f) + 1;
        return angle - (f32)turns * 6.2831852f;
    }
    if (angle < -3.1415926f) {
        turns = (s32)(angle / 6.2831852f) - 1;
        return angle - (f32)turns * 6.2831852f;
    }
    return angle;
}

INCLUDE_RODATA(const s32, "game/code_002E4720", D_003B4690);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD3C8);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD3D0);

INCLUDE_SDATA(const s32, "game/code_002E4720", sdfDiscRequestPending);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD3E0);

INCLUDE_SDATA(const s32, "game/code_002E4720", sdfDiscType);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD3F0);

INCLUDE_SDATA(const s32, "game/code_002E4720", sdfDevReplySemaphore);

INCLUDE_SDATA(const s32, "game/code_002E4720", sdfDiscPathPrefix);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD400);

INCLUDE_SDATA(const s32, "game/code_002E4720", sdfPfsPathPrefix);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD410);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD418);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD41C);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD420);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD424);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD428);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD42C);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD42E);

INCLUDE_SDATA(const s32, "game/code_002E4720", sdfDevicePriorityOverrideTicks);

INCLUDE_SDATA(const s32, "game/code_002E4720", sdfDeviceWorkerPriority);

INCLUDE_SDATA(const s32, "game/code_002E4720", sdfDefaultDevRequestOptions);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD438);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD440);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD448);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD450);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD458);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD460);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD468);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD470);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD478);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD480);

