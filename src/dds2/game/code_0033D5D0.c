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
    u8 pad8[8]; /* 0x8 */
    void *resource; /* 0x10 */
    u8 workerIndex; /* 0x14 */
    u8 operation; /* 0x15 */
    s8 state; /* 0x16 */
    u8 pad17; /* 0x17 */
    s32 unk18; /* 0x18 */
    s32 requestExtra; /* 0x1C */
    s32 requestData; /* 0x20 */
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
    u8 pad4[20];
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

typedef struct DevRequest {
    s32 handle;
    s16 flags;
    u16 count;
    s16 stride;
    u16 mode;
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

extern DevState *func_0033F9D0(void *, s32, s32,
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

extern void func_003406A0(f32 arg0);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033D5D0);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033D7B8);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033D810);

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

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033DAD8);

u32 func_0033DB58(void) {
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

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033DBF8);

void sdfDevSignalPendingSemaphore(void) {
    if (D_00438AC8 != 0) {
        SignalSema(D_004391C4);
        D_00438AC8 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033DCD0);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033DD90);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033DE60);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033E008);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033E1C0);

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

INCLUDE_ASM(const s32, "game/code_0033D5D0", sdfDevGetFileSize);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033E550);

INCLUDE_ASM(const s32, "game/code_0033D5D0", sdfDevStartLoad);

void sdfInitDeviceSemaphores(void) {
    D_004391B8 = sdfCreateSemaphore(1, 0xff, 0);
    D_004391C4 = sdfCreateSemaphore(0, 0xff, 0);
    D_00438AC8 = 0;
    func_0034CB60(0);
    func_0034D038(D_00438AD8);
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033E6B0);

INCLUDE_ASM(const s32, "game/code_0033D5D0", sdfDevBuildPath);

INCLUDE_ASM(const s32, "game/code_0033D5D0", sdfPathPrefixCat);

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

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033EA60);

void func_0033EAE0(u32 arg0) {
    sdfDevQueueActiveOperation();
    WaitSema(D_00438AE4);
    func_0033FD30(arg0);
}

void func_0033EB10(void) {
    func_0033FBF0();
    WaitSema(D_00438AE4);
}

u32 func_0033EB30(void) {
    func_0033FB98();
    WaitSema(D_00438AE4);
    return D_004391C8;
}

u32 func_0033EB58(void) {
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

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033ED38);

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

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033EF58);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033F650);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033F898);

DevState *func_0033F9D0(void *resource, s32 workerIndex, s32 operation,
                        void (*callback)(DevState *, s32, s32, s32, s32), s32 context) {
    DevState *state = (DevState *)func_00328E18(0x40);
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
    state = func_0033F9D0(resource, id, 1, callback, context);
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
    state = func_0033F9D0(resource, id, 2, callback, context);
    state->options = options != 0 ? options : D_00438B24;
    func_0033EC48(state);
    return state;
}

s32 sdfDevQueueOperation(DevState *arg0, s32 arg1, s32 arg2) {
    if (arg0->state != 7) {
        return -1;
    }
    arg0->unk18 = arg1;
    arg0->options = arg2;
    arg0->operation = 3;
    SignalSema(D_0040BA14[arg0->workerIndex].sema);
    return 0;
}

s32 func_0033FB98(DevState *arg0) {
    if (arg0->state != 7) {
        return -1;
    }
    arg0->operation = 4;
    SignalSema(D_0040BA14[arg0->workerIndex].sema);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033FBF0);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033FC50);

s32 sdfDevReactivate(DevState *arg0) {
    if (arg0->state != 9) {
        return -1;
    }
    arg0->result = 0;
    arg0->state = 7;
    return 0;
}

s32 sdfDevQueueActiveOperation(DevState *arg0) {
    s8 state = arg0->state;

    if (state != 7) {
        return -1;
    }
    arg0->operation = state;
    SignalSema(D_0040BA14[arg0->workerIndex].sema);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033FD30);

DevState *sdfDevCreateRequest(s32 path, s32 data, s32 extra,
                        void (*completion)(DevState *, s32, s32, s32, s32), s32 completionContext) {
    void *resource;
    s32 id = func_0033F898(path, &resource);
    DevState *state;

    if (id < 0) {
        return NULL;
    }
    state = func_0033F9D0(resource, id, 8, completion, completionContext);
    state->requestExtra = extra;
    state->requestData = data;
    state->unk18 = 0;
    func_0033EC48(state);
    return state;
}

DevState *sdfDevOpenRequest(s32 path, s32 data, s32 extra,
                        void (*completion)(DevState *, s32, s32, s32, s32),
                        s32 completionContext, s32 options) {
    void *resource;
    s32 id = func_0033F898(path, &resource);
    DevState *state;

    if (id < 0) {
        return NULL;
    }
    state = func_0033F9D0(resource, id, 9, completion, completionContext);
    state->requestExtra = extra;
    state->requestData = data;
    state->options = options != 0 ? options : D_00438B24;
    state->unk18 = 0;
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

void sdfPowerOffLoop(s32 arg0) {
    s32 status;

    for (;;) {
        WaitSema(arg0);
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

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_00340328);

char *sdfStrDup(const char *arg0) {
    u32 len;
    char *buf;

    if (arg0 == NULL) {
        return NULL;
    }
    len = strlen(arg0);
    buf = func_00328D68(len + 1);
    memcpy(buf, arg0, len);
    buf[len] = 0;
    return buf;
}

s32 sdfBcdStrToInt(s32 arg0) {
    s32 place = 1;
    s32 acc = 0;

    while (arg0 > 0) {
        acc += (arg0 & 0xf) * place;
        arg0 >>= 4;
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
    func_00328E48(request);
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_00340558);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_003405D8);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_003406A0);

void func_003407A0(f32 arg0) {
    func_003406A0(arg0 + 1.5707963f);
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_003407C0);

f32 sdfAtan2Poly(f32 arg0) {
    f32 x2 = arg0 * arg0;
    f32 x3 = x2 * arg0;
    f32 x5 = x2 * x3;

    return arg0 * 0.99999977f + x3 * -0.33325735f + x5 * 0.19388643f;
}

f32 sdfAtan2(f32 arg0, f32 arg1) {
    s32 sx = 0;
    s32 sy;
    f32 r;

    if (arg1 < 0.0f) {
        arg1 = -arg1;
        sx = 1;
    }
    sy = 0;
    if (arg0 < 0.0f) {
        arg0 = -arg0;
        sy = 1;
    }
    if (arg0 < arg1) {
        r = sdfAtan2Poly(arg0 / arg1);
    } else {
        r = 1.5707963f - sdfAtan2Poly(arg1 / arg0);
    }
    if (sx != 0) {
        r = 3.1415926f - r;
    }
    if (sy != 0) {
        r = -r;
    }
    return r;
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_00340950);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_003409C8);

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

