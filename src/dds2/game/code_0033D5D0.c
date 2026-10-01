#include "common.h"

extern u8 D_00438B1F;

extern u32 D_004391A4;

extern s32 D_00438AC8;

extern u32 D_004391C4;

extern s32 D_004391A8;

extern u32 D_004391B8;

extern s32 func_0033E008(u32, u8 *, u32);

extern u32 D_004391C8;

extern u32 D_004391CC;

typedef struct SifCommand {
    s32 source;   /* 0x0 */
    s32 end;      /* 0x4 */
    s32 argument; /* 0x8 */
    u32 command;  /* 0xC */
} SifCommand;

extern u32 D_0040B990[];

extern char D_0040B9D0[];

typedef struct DevState {
    struct DevState *unk0; /* 0x0 */
    struct DevState *unk4; /* 0x4 */
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
    u8 pad30[8]; /* 0x30 */
    void (*callback)(struct DevState *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4); /* 0x38 */
    s32 callbackContext; /* 0x3C */
} DevState;

typedef struct SemaEntry {
    s32 sema; /* 0x0 */
    u8 pad4[20]; /* 0x4 */
} SemaEntry;

typedef struct ThreadEntry {
    s32 threadId;
    s32 sema;               /* 0x4 */
    DevState *first;        /* 0x8 */
    DevState *last;         /* 0xC */
    u8 pad10[8];
} ThreadEntry;

extern SemaEntry D_0040BA14[];

extern s32 SignalSema(s32 sema);

extern s32 D_00438B20;

extern ThreadEntry D_0040BA10[];

extern s32 ChangeThreadPriority(s32 tid, s32 prio);

extern s32 WaitSema(s32 sema);

extern void func_0036C330(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

extern void sceCdPowerOff(void *arg0);

extern u8 D_00438B50[];

extern u8 D_00438B58[];

extern void *func_00328D68(s32 size);

extern u32 strlen(const char *s);
extern f32 D_00410C6C[];

typedef struct DevRequest {
    s32 handle;
    s16 flags;
    u16 count;
    s16 stride;
    s16 mode;
    s32 buffer;
} DevRequest;

extern s32 func_003292A8(s32 size);

extern s32 sdfResourceRetainAddress(s32 arg0);

extern s32 GetThreadId(void);

extern void sceSifSetRpcQueue(void *, s32);

extern void sceSifRegisterRpc(void *, s32, void *, void *, s32, s32, void *);

extern void sceSifRpcLoop(void *);

extern u8 D_00476510[];

extern u32 D_00438AD8;

extern void sdfSleepWithAlarm(s32);

void sdfDevWaitForDisc(void);

extern s32 D_00439198;

extern void func_0034CE40(s32);

extern s32 func_0034D100(void);

extern s32 sceCdSearchFile(void *, s32);

extern DevState *sdfDevCreateCallbackState(s32 arg0,
                                void (*callback)(DevState *, s32, s32, s32, s32), s32 arg2);

extern s32 func_0033F898(s32, void **);

extern void func_0033EC48(DevState *);

extern DevState *sdfDevAllocState(void *, s32, s32,
                                void (*)(DevState *, s32, s32, s32, s32), s32);

extern u16 D_00438B24;

extern s32 sdfCreateSemaphore(s32 arg0, s32 arg1, s32 arg2);

extern void func_0034D038(u32 arg0);

extern s32 D_00438AE4;

s32 sdfDevReactivate(DevState *);

extern char D_0042E3A0[]; /* "cdrom0:\\IRX\\DEV9.IRX;1 resident fail.\n", followed by padding no C emits */

extern u8 D_00438B67;

extern u8 D_00438B68;

extern s32 func_0036D880(const char *, s32, void *, s32 *);

extern void func_0035B6E0(const char *);

extern f32 sdfSinPoly(f32 arg0);

extern void func_00328520(const char *arg0, ...) __attribute__((noreturn));

extern char D_0042E288[];

extern u32 D_0043919C;

extern u32 D_004391A0;

extern s32 sceSifAllocIopHeap(s32);

extern void func_0034D400(s32, s32, s32);

extern void func_0033E550(s32);

typedef struct Bytes7 {
    s8 b[7];
} Bytes7;

extern Bytes7 D_00438AF0[];

extern char *strcpy(char *, char *);

extern char *strcat(char *, char *);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033D5D0);

extern void func_0033D5D0(void *packet, const char *fmt, void *args);
void sdfPktInit(SifCommand *packet, s32 source, s32 end, s32 argument, s32 index);

void sdfFormatSifPacket(void *packet, const char *fmt, ...) {
    __builtin_va_list args;

    __builtin_stdarg_start(args, fmt);
    func_0033D5D0(packet, fmt, args);
}

void sdfCreateFormattedSifCommand(s32 source, s32 end, s32 argument, s32 index, const char *fmt, ...) {
    SifCommand packet;
    __builtin_va_list args;

    sdfPktInit(&packet, source, end, argument, index);
    __builtin_stdarg_start(args, fmt);
    func_0033D5D0(&packet, fmt, args);
}

void sdfPktSetCmd(SifCommand *packet, s32 index) {
    packet->command = D_0040B990[index];
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
    sceSifRegisterRpc(server, 0x32647270, sdfRpcBufHandler, D_00476510, 0, 0, queue);
    sceSifRpcLoop(queue);
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033D990);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033DA30);

extern s32 func_00360E78(char *dst, const char *fmt, void *args);
extern void func_0033DA30(const char *text);

s32 sdfPrintFormattedDevMessage(const char *fmt, ...) {
    char buffer[0x100];
    __builtin_va_list args;
    s32 length;

    __builtin_stdarg_start(args, fmt);
    length = func_00360E78(buffer, fmt, args);
    func_0033DA30(buffer);
    return length;
}

u32 sdfDevGetLoadedFileAddress(void) {
    return D_004391A4;
}

void sdfDevWaitForDisc(void) {
    u8 file[0x30];
    s32 status;
    WaitSema(D_004391B8);
    for (;;) {
        sdfSleepWithAlarm(100);
        func_0034CE40(0);
        status = func_0034D100();
        if (D_00438AD8 == 2) {
            if (status != 20) {
                continue;
            }
        } else if (status != 18) {
            continue;
        }
        if (sceCdSearchFile(file, D_00439198) != 0) {
            break;
        }
    }
    SignalSema(D_004391B8);
}

extern u8 D_004391C0;
extern s32 D_004391BC;
extern s32 func_0034D430(s32 arg, u8 *options);
extern s32 sceCdStatus(void);

void sdfDevSeekDiscRequest(s32 request) {
    u8 options[4];
    s32 ready;

    options[0] = 0;
    if (D_004391C0 == 1) {
        options[1] = 0;
    } else {
        options[1] = 1;
    }
    options[2] = 0;
    options[3] = 0;
    for (;;) {
        WaitSema(D_004391B8);
        ready = func_0034D430(request, options);
        SignalSema(D_004391B8);
        if (ready != 0) {
            break;
        }
        if (sceCdStatus() == 1) {
            sdfDevWaitForDisc();
        } else {
            sdfSleepWithAlarm(100);
        }
    }
    D_004391BC = request;
}

void sdfDevSignalPendingSemaphore(void) {
    if (D_00438AC8 != 0) {
        SignalSema(D_004391C4);
        D_00438AC8 = 0;
    }
}

void sdfDevSeekDiscWithRequestGate(s32 request) {
    u8 options[4];
    s32 ready;

    options[0] = 0;
    if (D_004391C0 == 1) {
        options[1] = 0;
    } else {
        options[1] = 1;
    }
    options[2] = 0;
    options[3] = 0;
    for (;;) {
        WaitSema(D_004391B8);
        D_00438AC8 = 1;
        WaitSema(D_004391C4);
        ready = func_0034D430(request, options);
        SignalSema(D_004391B8);
        if (ready != 0) {
            break;
        }
        if (sceCdStatus() == 1) {
            sdfDevWaitForDisc();
        } else {
            sdfSleepWithAlarm(100);
        }
    }
    D_004391BC = request;
}

extern s32 func_0034D500(s32 size, s32 buffer, s32 count, s32 *status);

void sdfDevReadDiscUntilComplete(s32 size, s32 buffer) {
    s32 status;
    s32 result;

    for (;;) {
        WaitSema(D_004391B8);
        result = func_0034D500(size, buffer, 1, &status);
        SignalSema(D_004391B8);
        if (status == 0 && result == size) {
            break;
        }
        if (sceCdStatus() == 1) {
            sdfDevWaitForDisc();
            sdfDevSeekDiscRequest(D_004391BC);
        } else {
            WaitSema(D_004391B8);
            func_0034D4C8();
            SignalSema(D_004391B8);
            sdfDevSeekDiscRequest(D_004391BC);
        }
    }
    D_004391BC += size;
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033DE60);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033E008);

extern s32 D_004391AC;
extern s32 D_004391B4;

s32 sdfDevOpenDiscFileAndGetSize(const char *name) {
    s32 result;
    s32 request[4];
    s32 file;

    file = func_0033E008((u32)name, (u8 *)request, (u32)&D_004391C0);
    result = -1;
    if (file != 0) {
        D_004391AC = *(s32 *)(file + 8);
        D_004391A8 = file;
        D_004391B4 = 0;
        if (name[1] == 0x76 || name[1] == 0x56) {
            sdfDevSeekDiscWithRequestGate(request[0]);
        } else {
            sdfDevSeekDiscRequest(request[0]);
        }
        result = *(s32 *)(file + 8);
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033E248);

void func_0033E490(void) {
    if (D_004391A8 != 0) {
        WaitSema(D_004391B8);
        func_0034D4C8();
        SignalSema(D_004391B8);
        D_004391A8 = 0;
    }
}

u32 sdfPacketExists(index)
    u32 index;
{
    s64 packet;
    u8 buffer[16];

    packet = func_0033E008(index, buffer, 0);
    return packet != 0;
}

s32 sdfPktQuery(u32 index) {
    u8 buffer[16];
    SifCommand *packet;

    packet = (SifCommand *)func_0033E008(index, buffer, 0);
    if (packet != 0) {
        return packet->argument;
    }
    return -1;
}

s32 sdfDevGetFileSize(void) {
    if (D_004391A8 == 0) {
        func_00328520("file didn't open.");
    }
    return *(s32 *)(D_004391A8 + 8);
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033E550);

void sdfDevStartLoad(s32 name, s32 mode) {
    u32 file[12];
    s32 heap;

    D_00439198 = name;
    func_0033E550(mode);
    if (sceCdSearchFile(file, name) == 0) {
        func_00328520(D_0042E288, name);
    }
    D_004391A0 = file[0];
    heap = sceSifAllocIopHeap(0x28010);
    D_0043919C = heap;
    func_0034D400(0x50, 5, (heap + 15) & -16);
}

void sdfInitDeviceSemaphores(void) {
    D_004391B8 = sdfCreateSemaphore(1, 0xff, 0);
    D_004391C4 = sdfCreateSemaphore(0, 0xff, 0);
    D_00438AC8 = 0;
    func_0034CB60(0);
    func_0034D038(D_00438AD8);
}

extern char D_00438AE8[];

/* Convert a relative disc path to the drive's uppercase backslash form with ;1 suffix. */
void sdfDevMakeDiscPath(char *dst, char *src) {
    s32 c;

    memcpy(dst, D_00438AE8, 8);
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

char *sdfDevBuildPath(char *dst, char *src) {
    if (*src == 0x2F) {
        strcpy(dst, D_0040B9D0);
        return strcat(dst, src);
    }
    *(Bytes7 *)dst = D_00438AF0[0];
    return strcat(dst, src);
}

typedef struct Bytes6 {
    s8 b[6];
} Bytes6;

extern Bytes6 D_00438AF8[];

void sdfPathPrefixCat(char *dst, char *src) {
    *(Bytes6 *)dst = D_00438AF8[0];
    strcat(dst, src);
}

u32 func_0033E800(void) {
    return 0;
}

u32 func_0033E808(void) {
    return 0;
}

u32 func_0033E810(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033E818);

void sdfDevCommandReplyCallback(DevState *state, s32 event, s32 unused, s32 value, s32 context) {
    if (event != 3) {
        if (event == 4) {
            D_004391C8 = value;
        }
    } else {
        D_004391CC = value;
    }
    SignalSema(D_00438AE4);
}

DevState *sdfDevCreateCommandState(s32 command) {
    DevState *state;
    if (D_00438AE4 < 0) {
        D_00438AE4 = sdfCreateSemaphore(0, 0x80, 0);
    }
    state = sdfDevCreateCallbackState(command, sdfDevCommandReplyCallback, 0);
    WaitSema(D_00438AE4);
    sdfDevReactivate(state);
    return state;
}

extern char *func_0033E818();

extern s32 func_00369B70();

extern void func_00369DF8(s32);

extern void sdfReleaseChipBlock(void *);

s32 sdfPathExists(char *path) {
    char *resolved = func_0033E818(path);
    s32 fd;

    if (*resolved == '/') {
        return sdfPacketExists((u32)resolved);
    }

    fd = func_00369B70(resolved, 1);
    func_00369DF8(fd);
    sdfReleaseChipBlock(resolved);
    return fd >= 0;
}

void sdfDevWaitThenReleaseCommandState(DevState *state) {
    sdfDevQueueActiveOperation();
    WaitSema(D_00438AE4);
    sdfDevQueueReleaseState(state);
}

void sdfDevQueueReadAndWait(void) {
    sdfDevQueueRead();
    WaitSema(D_00438AE4);
}

u32 sdfDevQueueControlAndWait(void) {
    sdfDevQueueControlRequest();
    WaitSema(D_00438AE4);
    return D_004391C8;
}

u32 sdfDevQueueOperationAndWait(void) {
    sdfDevQueueOperation();
    WaitSema(D_00438AE4);
    return D_004391CC;
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033EB80);

char *func_0033EC18(void) {
    return D_0040B9D0;
}

extern s8 D_00438AE0;

void func_0033EC28(s8 value) {
    D_00438AE0 = value;
    func_0033EC40();
}

extern s8 D_00438B1E;

void func_0033EC40(s8 value) {
    D_00438B1E = value;
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033EC48);

extern s64 func_0036DE70(void);
extern void EIntr(void);
extern void sdfReleaseChipBlock(void *ptr);
extern DevState *D_00438B14;
extern DevState *D_00438B18;
extern s16 D_00438B10;

void sdfDevUnlinkAndFreeState(DevState *state) {
    DevState *prev;
    DevState *next;
    s64 interrupts;

    interrupts = func_0036DE70();
    prev = state->unk4;
    next = state->unk0;
    if (prev == NULL) {
        D_00438B14 = next;
    } else {
        prev->unk0 = next;
    }
    if (next == NULL) {
        D_00438B18 = prev;
    } else {
        next->unk4 = prev;
    }
    D_00438B10 -= 1;
    if (interrupts != 0) {
        EIntr();
    }
    sdfReleaseChipBlock(state->resource);
    sdfReleaseChipBlock(state);
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033EDB0);

void sdfDevRelease(DevState *state) {
    s32 resourceId;

    resourceId = state->resourceId;
    state->resourceId = -1;
    if (resourceId >= 0) {
        func_00369DF8(resourceId);
    }
    func_0033EDB0(state);
}

void sdfDevDeactivate(DevState *state, s32 result) {
    state->result = result;
    state->state = 9;
    sdfDevRelease(state);
    if (state->callback != NULL) {
        state->callback(state, 0, 0, 0, state->callbackContext);
    }
}

INCLUDE_RODATA(const s32, "game/code_0033D5D0", D_0042E288);

INCLUDE_ASM(const s32, "game/code_0033D5D0", sdfDevWorkerThread);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033F650);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033F898);

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
    s32 id = func_0033F898(path, &resource);
    DevState *state;

    if (id < 0) {
        return NULL;
    }
    state = sdfDevAllocState(resource, id, 1, callback, context);
    func_0033EC48(state);
    return state;
}

DevState *sdfDevCreateModeState(s32 path, void (*callback)(DevState *, s32, s32, s32, s32),
                        s32 context, s32 options) {
    void *resource;
    s32 id = func_0033F898(path, &resource);
    DevState *state;

    if (id < 0) {
        return NULL;
    }
    state = sdfDevAllocState(resource, id, 2, callback, context);
    state->options = options != 0 ? options : D_00438B24;
    func_0033EC48(state);
    return state;
}

s32 sdfDevQueueOperation(DevState *state, s32 operationArg, s32 options) {
    if (state->state != 7) {
        return -1;
    }
    state->operationArg = operationArg;
    state->options = options;
    state->operation = 3;
    SignalSema(D_0040BA14[state->workerIndex].sema);
    return 0;
}

s32 sdfDevQueueControlRequest(DevState *state) {
    if (state->state != 7) {
        return -1;
    }
    state->operation = 4;
    SignalSema(D_0040BA14[state->workerIndex].sema);
    return 0;
}

s32 sdfDevQueueRead(DevState *state, void *data, s32 extra) {
    if (state->state != 7) {
        return -1;
    }
    state->requestExtra = extra;
    state->operation = 5;
    state->requestData = data;
    SignalSema(D_0040BA14[state->workerIndex].sema);
    return 0;
}

s32 sdfDevQueueWrite(DevState *state, void *data, s32 extra) {
    if (state->state != 7) {
        return -1;
    }
    state->requestExtra = extra;
    state->operation = 6;
    state->requestData = data;
    SignalSema(D_0040BA14[state->workerIndex].sema);
    return 0;
}

s32 sdfDevReactivate(DevState *state) {
    if (state->state != 9) {
        return -1;
    }
    state->result = 0;
    state->state = 7;
    return 0;
}

s32 sdfDevQueueActiveOperation(DevState *request) {
    s8 state = request->state;

    if (state != 7) {
        return -1;
    }
    request->operation = state;
    SignalSema(D_0040BA14[request->workerIndex].sema);
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

DevState *sdfDevCreateRequest(s32 path, void *data, s32 extra,
                        void (*completion)(DevState *, s32, s32, s32, s32), s32 completionContext) {
    void *resource;
    s32 id = func_0033F898(path, &resource);
    DevState *state;

    if (id < 0) {
        return NULL;
    }
    state = sdfDevAllocState(resource, id, 8, completion, completionContext);
    state->requestExtra = extra;
    state->requestData = data;
    state->operationArg = 0;
    func_0033EC48(state);
    return state;
}

DevState *sdfDevOpenRequest(s32 path, void *data, s32 extra,
                        void (*completion)(DevState *, s32, s32, s32, s32),
                        s32 completionContext, s32 options) {
    void *resource;
    s32 id = func_0033F898(path, &resource);
    DevState *state;

    if (id < 0) {
        return NULL;
    }
    state = sdfDevAllocState(resource, id, 9, completion, completionContext);
    state->requestExtra = extra;
    state->requestData = data;
    state->options = options != 0 ? options : D_00438B24;
    state->operationArg = 0;
    func_0033EC48(state);
    return state;
}

void sdfSetThreadPriorities(s32 priority) {
    ThreadEntry *thread;
    u32 i;

    if (D_00438B20 == priority) {
        return;
    }
    D_00438B20 = priority;
    thread = D_0040BA10;
    i = 0;
    do {
        s32 tid = thread->threadId;

        thread++;
        if (tid >= 0) {
            ChangeThreadPriority(tid, priority);
        }
        i++;
    } while (i < 4);
}

void sdfRaiseDeviceThreadPriority(void) {
    D_00438B1F = 3;
    sdfSetThreadPriorities(0x78);
}

void sdfRestoreDeviceThreadPriority(void) {
    sdfSetThreadPriorities(0x48);
}

void sdfTickThreadPriorityOverride(void) {
    u8 val = D_00438B1F;
    u8 next;

    if (val == 0) {
        return;
    }
    D_00438B1F = val - 1;
    next = val - 1;
    if (next != 0) {
        return;
    }
    sdfRestoreDeviceThreadPriority();
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033FF98);

void sdfPowerOffLoop(s32 semaphore) {
    s32 status;

    for (;;) {
        WaitSema(semaphore);
        func_0036C330(D_00438B50, 0x5003, 0, 0, 0, 0);
        func_0036C330(D_00438B58, 0x4806, 0, 0, 0, 0);
        sceCdPowerOff(&status);
    }
}

void func_003400B8(void) {
    iSignalSema();
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_003400D0);

void sdfLoadDevModule(void) {
    s32 resident;
    if (D_00438B67 == 0) {
        D_00438B68 = 0;
        if (func_0036D880("cdrom0:\\IRX\\DEV9.IRX;1", 0, NULL, &resident) < 0) {
            func_0035B6E0("cdrom0:\\IRX\\DEV9.IRX;1 could't load.\n");
            return;
        }
        if (resident != 0) {
            func_0035B6E0(D_0042E3A0);
            return;
        }
        D_00438B68 = 1;
        D_00438B67 = 1;
    }
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_00340298);

INCLUDE_ASM(const s32, "game/code_0033D5D0", sdfClearQuadwords);

char *sdfStrDup(const char *text) {
    u32 length;
    char *copy;

    if (text == NULL) {
        return NULL;
    }
    length = strlen(text);
    copy = func_00328D68(length + 1);
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
    DevRequest *request = func_00328D68(sizeof(*request));

    request->mode = mode;
    request->flags = 0;
    request->count = count;
    request->stride = stride;
    if (count != 0) {
        request->handle = func_003292A8(stride * count);
        request->buffer = sdfResourceRetainAddress(request->handle);
    } else {
        request->handle = 0;
        request->buffer = 0;
    }
    return request;
}

void sdfDestroyDevRequest(DevRequest *request) {
    func_003297C8(request->handle);
    sdfReleaseChipBlock(request);
}

extern void sdfDecrementAllocationReferenceCount(s32 handle);
extern void func_00329600(s32 handle, s32 size);
void sdfDevResizeBufferedRequest(DevRequest *request, s32 count);

void sdfDevBufferedRequestGrow(DevRequest *request) {
    if (request->handle == 0) {
        sdfDevResizeBufferedRequest(request, request->mode);
        return;
    }
    sdfDecrementAllocationReferenceCount(request->handle);
    request->count = request->count + request->mode;
    func_00329600(request->handle, (s16)request->count * request->stride);
    request->buffer = sdfResourceRetainAddress(request->handle);
}

void sdfDevResizeBufferedRequest(DevRequest *request, s32 count) {
    if (request->handle == 0) {
        if (count > 0) {
            request->count = count;
            request->handle = func_003292A8(request->stride * count);
            request->buffer = sdfResourceRetainAddress(request->handle);
        }
    } else if (count <= 0) {
        func_003297C8(request->handle);
        request->handle = 0;
        request->flags = 0;
        request->count = 0;
        request->buffer = 0;
    } else {
        sdfDecrementAllocationReferenceCount(request->handle);
        request->count = count;
        func_00329600(request->handle, request->stride * count);
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

void func_003407A0(f32 angle) {
    sdfSinPoly(angle + 1.5707963f);
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
    result = x >= 1.0f ? 1.5707963f : sdfTableInterpolate(x, D_00410C6C, 128) * 1.5707963f;
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
        result = (1.0f - sdfTableInterpolate(x, D_00410C6C, 128)) * 1.5707963f;
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

INCLUDE_RODATA(const s32, "game/code_0033D5D0", D_0042E3A0);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438AB8);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438AC0);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438AC8);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438AD0);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438AD8);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438AE0);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438AE4);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438AE8);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438AF0);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438AF8);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B00);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B08);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B0C);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B10);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B14);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B18);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B1C);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B1E);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B1F);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B20);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B24);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B28);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B30);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B38);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B40);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B48);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B50);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B58);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B60);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B68);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B70);

