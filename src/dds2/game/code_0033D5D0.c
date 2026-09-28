#include "common.h"

extern u8 D_00438B1F;

extern u32 D_004391A4;

extern s32 D_00438AC8;

extern u32 D_004391C4;

extern s32 D_004391A8;

extern u32 D_004391B8;

extern s32 func_0033E008(u32, u8 *, u32);

extern u32 D_004391C8;

extern u32 D_00438AE4;

extern u32 D_004391CC;

typedef struct SifCommand {
    s32 unk0; /* 0x0 */
    s32 unk4; /* 0x4 */
    s32 unk8; /* 0x8 */
    u32 unkC; /* 0xC */
} SifCommand;

extern u32 D_0040B990[];

extern char D_0040B9D0[];

typedef struct DevState {
    struct DevState *unk0; /* 0x0 */
    struct DevState *unk4; /* 0x4 */
    u8 pad8[8]; /* 0x8 */
    void *unk10; /* 0x10 */
    u8 unk14; /* 0x14 */
    u8 unk15; /* 0x15 */
    s8 unk16; /* 0x16 */
    u8 pad17; /* 0x17 */
    s32 unk18; /* 0x18 */
    s32 unk1C; /* 0x1C */
    s32 unk20; /* 0x20 */
    s32 unk24; /* 0x24 */
    s32 unk28; /* 0x28 */
    s32 unk2C; /* 0x2C */
    u8 pad30[8]; /* 0x30 */
    void (*unk38)(struct DevState *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4); /* 0x38 */
    s32 unk3C; /* 0x3C */
} DevState;

typedef struct SemaEntry {
    s32 sema; /* 0x0 */
    u8 pad4[20]; /* 0x4 */
} SemaEntry;

extern SemaEntry D_0040BA14[];

extern s32 SignalSema(s32 sema);

extern s32 D_00438B20;

extern SemaEntry D_0040BA10[];

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

extern s32 func_003298F8(s32 arg0);

extern s32 GetThreadId(void);

extern void sceSifSetRpcQueue(void *, s32);

extern void sceSifRegisterRpc(void *, s32, void *, void *, s32, s32, void *);

extern void sceSifRpcLoop(void *);

extern u8 D_00476510[];

extern u32 D_00438AD8;

extern void sleepWithAlarm(s32);

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

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033D5D0);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033D7B8);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033D810);

void sdfPktSetCmd(SifCommand *arg0, s32 arg1) {
    arg0->unkC = D_0040B990[arg1];
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", sdfPktInit);

void *sdfRpcBufHandler(s32 arg0, SifCommand *arg1) {
    s32 size = arg1->unk4 - arg1->unk0;

    if (size > 0) {
        memcpy(arg1, (void *)arg1->unk0, size);
        return arg1;
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
        sleepWithAlarm(100);
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

void func_0033DCA8(void) {
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

u8 func_0033E4C8(u32 arg0) {
    s64 temp_v0;
    u8 temp_v1 [16];

    temp_v0 = func_0033E008(arg0, temp_v1, 0);
    return temp_v0 != 0;
}

s32 sdfPktQuery(u32 arg0) {
    u8 buf[16];
    s32 pkt;

    pkt = func_0033E008(arg0, buf, 0);
    if (pkt != 0) {
        return *(s32 *)(pkt + 8);
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033E520);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033E550);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033E5E0);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033E660);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033E6B0);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033E728);

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

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033E9B8);

INCLUDE_ASM(const s32, "game/code_0033D5D0", sdfDevCreateCommandState);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033EA60);

void func_0033EAE0(u32 arg0) {
    func_0033FCE0();
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
    func_0033FB38();
    WaitSema(D_00438AE4);
    return D_004391CC;
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033EB80);

char *func_0033EC18(void) {
    return D_0040B9D0;
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033EC28);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033EC40);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033EC48);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033ED38);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033EDB0);

void sdfDevRelease(u32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 0x28);
    *(u32 *)((s32)arg0 + 0x28) = 0xffffffff;
    if (-1 < temp_v0) {
        func_00369DF8(temp_v0);
    }
    func_0033EDB0(arg0);
}

void sdfDevDeactivate(DevState *arg0, s32 arg1) {
    arg0->unk2C = arg1;
    arg0->unk16 = 9;
    sdfDevRelease(arg0);
    if (arg0->unk38 != NULL) {
        arg0->unk38(arg0, 0, 0, 0, arg0->unk3C);
    }
}

INCLUDE_RODATA(const s32, "game/code_0033D5D0", D_0042E288);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033EF58);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033F650);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033F898);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033F9D0);

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
    state->unk24 = options != 0 ? options : D_00438B24;
    func_0033EC48(state);
    return state;
}

s32 func_0033FB38(DevState *arg0, s32 arg1, s32 arg2) {
    if (arg0->unk16 != 7) {
        return -1;
    }
    arg0->unk18 = arg1;
    arg0->unk24 = arg2;
    arg0->unk15 = 3;
    SignalSema(D_0040BA14[arg0->unk14].sema);
    return 0;
}

s32 func_0033FB98(DevState *arg0) {
    if (arg0->unk16 != 7) {
        return -1;
    }
    arg0->unk15 = 4;
    SignalSema(D_0040BA14[arg0->unk14].sema);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033FBF0);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033FC50);

s32 func_0033FCB0(DevState *arg0) {
    if (arg0->unk16 != 9) {
        return -1;
    }
    arg0->unk2C = 0;
    arg0->unk16 = 7;
    return 0;
}

s32 func_0033FCE0(DevState *arg0) {
    s8 state = arg0->unk16;

    if (state != 7) {
        return -1;
    }
    arg0->unk15 = state;
    SignalSema(D_0040BA14[arg0->unk14].sema);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033FD30);

DevState *sdfDevCreateRequest(s32 path, s32 data, s32 extra,
                        void (*context)(DevState *, s32, s32, s32, s32), s32 callback) {
    void *resource;
    s32 id = func_0033F898(path, &resource);
    DevState *state;

    if (id < 0) {
        return NULL;
    }
    state = func_0033F9D0(resource, id, 8, context, callback);
    state->unk1C = extra;
    state->unk20 = data;
    state->unk18 = 0;
    func_0033EC48(state);
    return state;
}

DevState *sdfDevOpenRequest(s32 path, s32 data, s32 extra,
                        void (*context)(DevState *, s32, s32, s32, s32),
                        s32 callback, s32 options) {
    void *resource;
    s32 id = func_0033F898(path, &resource);
    DevState *state;

    if (id < 0) {
        return NULL;
    }
    state = func_0033F9D0(resource, id, 9, context, callback);
    state->unk1C = extra;
    state->unk20 = data;
    state->unk24 = options != 0 ? options : D_00438B24;
    state->unk18 = 0;
    func_0033EC48(state);
    return state;
}

void sdfSetThreadPriorities(s32 arg0) {
    SemaEntry *p;
    u32 i;

    if (D_00438B20 == arg0) {
        return;
    }
    D_00438B20 = arg0;
    p = D_0040BA10;
    i = 0;
    do {
        s32 tid = p->sema;

        p++;
        if (tid >= 0) {
            ChangeThreadPriority(tid, arg0);
        }
        i++;
    } while (i < 4);
}

void func_0033FF20(void) {
    D_00438B1F = 3;
    sdfSetThreadPriorities(0x78);
}

void func_0033FF40(void) {
    sdfSetThreadPriorities(0x48);
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033FF58);

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

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_00340218);

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

s32 devDecimalToPackedDigits(s32 number) {
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

DevRequest *devCreateRequest(s32 count, s32 stride, s32 mode) {
    DevRequest *request = func_00328D68(sizeof(*request));

    request->mode = mode;
    request->flags = 0;
    request->count = count;
    request->stride = stride;
    if (count != 0) {
        request->handle = func_003292A8(stride * count);
        request->buffer = func_003298F8(request->handle);
    } else {
        request->handle = 0;
        request->buffer = 0;
    }
    return request;
}

void func_00340528(u32 arg0) {
    func_003297C8(*(u32 *)arg0);
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_00340558);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_003405D8);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_003406A0);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_003407A0);

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

