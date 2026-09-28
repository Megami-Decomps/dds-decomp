#include "common.h"

typedef struct SifCommand {
    s32 unk0; /* 0x0 */
    s32 unk4; /* 0x4 */
    s32 unk8; /* 0x8 */
    u32 unkC; /* 0xC */
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
    u16 mode;
    s32 buffer;
} DevRequest;

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

extern u8 D_003BD42F;
extern u8 D_003BD42E;
extern u8 D_003BD3F0;
extern u16 D_003BD420;
extern DevState *D_003BD424;
extern DevState *D_003BD428;
extern s32 D_003BD430;

extern u32 D_003BDA6C;

extern u32 D_003BDA68;

extern s32 D_003BD3F4;

extern s32 func_002E5158(u32, u8 *, u32);
extern void func_002E5D98(s32 arg0);

extern s32 D_003BDA48;
extern u32 D_003BDA58;

extern s32 D_003BD3D8;
extern u32 D_003BDA64;

extern u32 D_003BDA44;

extern u32 D_003987E0[];
extern char D_00398820[];
extern SemaEntry D_00398860[];
extern SemaEntry D_00398864[];
extern u8 D_003BD408[];

extern s32 SignalSema(s32 sema);
extern s32 WaitSema(s32 sema);
extern s32 ChangeThreadPriority(s32 tid, s32 prio);
extern void func_002E5E90(DevState *arg0);
extern void func_002E5F08(DevState *arg0);
extern DevState *sdfDevCreateCallbackState(s32 arg0,
                                void (*callback)(DevState *, s32, s32, s32, s32), s32 arg2);
extern void func_002E77F8(f32 arg0);
extern void func_0030EB78(s32 arg0);
extern s32 func_00312C08(DevState *arg0);
extern void func_003110C8(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void EIntr(void);
extern void sceCdPowerOff(void *arg0);
extern s32 createSemaphore(s32 arg0, s32 arg1, s32 arg2);
extern s32 func_002CF670(const char *arg0);
extern void *func_002CFEB8(s32 size);
extern void func_002CFF98(void *ptr);
extern void *func_002CFF68(s32 size);
extern s32 func_002D03F8(s32 size);
extern void func_002D0750(s32 arg0, s32 arg1);
extern s32 func_002D0A48(s32 arg0);
extern s32 func_002D0A60(s32 arg0);
extern u32 strlen(const char *s);
extern void func_002E7730(CmdPkt *arg0, s32 arg1);
extern void func_002F4190(u32 arg0);
extern u32 D_003BD3E8;
extern u8 D_003BD460[];
extern u8 D_003BD468[];
extern s32 GetThreadId(void);
extern void sceSifSetRpcQueue(void *, s32);
extern void sceSifRegisterRpc(void *, s32, void *, void *, s32, s32, void *);
extern void sceSifRpcLoop(void *);
extern u8 D_003F9B90[];
extern void sleepWithAlarm(s32);
void sdfDevWaitForDisc(void);
extern s32 D_003BDA38;
extern void func_002F3F98(s32);
extern s32 func_002F4258(void);
extern s32 sceCdSearchFile(void *, s32);
extern u16 D_003BD434;
extern s32 func_002E69F0(s32, void **);
extern void func_002E5DA0(DevState *);
extern DevState *func_002E6B28(void *, s32, s32,
                                void (*)(DevState *, s32, s32, s32, s32), s32);
extern u8 D_003BD477;
extern u8 D_003BD478;
extern s32 func_00312618(const char *, s32, void *, s32 *);
extern void func_003003F0(const char *);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4720);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4908);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4960);

void sdfPktSetCmd(SifCommand *arg0, s32 arg1) {
    arg0->unkC = D_003987E0[arg1];
}

void sdfPktInit(SifCommand *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    arg0->unk0 = arg1;
    arg0->unk4 = arg2;
    arg0->unk8 = arg3;
    sdfPktSetCmd(arg0, arg4);
}

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
    sceSifRegisterRpc(server, 0x32647270, sdfRpcBufHandler, D_003F9B90, 0, 0, queue);
    sceSifRpcLoop(queue);
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4AE0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4B80);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4C28);

u32 func_002E4CA8(void) {
    return D_003BDA44;
}

void sdfDevWaitForDisc(void) {
    u8 file[0x30];
    s32 status;
    WaitSema(D_003BDA58);
    for (;;) {
        sleepWithAlarm(100);
        func_002F3F98(0);
        status = func_002F4258();
        if (D_003BD3E8 == 2) {
            if (status != 20) {
                continue;
            }
        } else if (status != 18) {
            continue;
        }
        if (sceCdSearchFile(file, D_003BDA38) != 0) {
            break;
        }
    }
    SignalSema(D_003BDA58);
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4D48);

void func_002E4DF8(void) {
    if (D_003BD3D8 != 0) {
        SignalSema(D_003BDA64);
        D_003BD3D8 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4E20);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4EE0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4FB0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5158);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5310);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5398);

void func_002E55E0(void) {
    if (D_003BDA48 != 0) {
        WaitSema(D_003BDA58);
        func_002F4620();
        SignalSema(D_003BDA58);
        D_003BDA48 = 0;
    }
}

u8 func_002E5618(u32 arg0) {
    u8 buf[16];

    return func_002E5158(arg0, buf, 0) != 0;
}

s32 sdfPktQuery(u32 arg0) {
    u8 buf[16];
    s32 pkt;

    pkt = func_002E5158(arg0, buf, 0);
    if (pkt != 0) {
        return *(s32 *)(pkt + 8);
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5670);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E56A0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5738);

void func_002E57B8(void) {
    D_003BDA58 = createSemaphore(1, 0xff, 0);
    D_003BDA64 = createSemaphore(0, 0xff, 0);
    D_003BD3D8 = 0;
    func_002F3CB8(0);
    func_002F4190(D_003BD3E8);
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5808);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5880);

void sdfPathPrefixCat(char *arg0, char *arg1) {
    memcpy(arg0, D_003BD408, 6);
    strcat(arg0, arg1);
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

void func_002E5B10(DevState *state, s32 event, s32 unused, s32 value, s32 context) {
    if (event != 3) {
        if (event == 4) {
            D_003BDA68 = value;
        }
    } else {
        D_003BDA6C = value;
    }
    SignalSema(D_003BD3F4);
}
s32 func_002E6E08(DevState *);

DevState *sdfDevCreateCommandState(s32 command) {
    DevState *state;
    if (D_003BD3F4 < 0) {
        D_003BD3F4 = createSemaphore(0, 0x80, 0);
    }
    state = sdfDevCreateCallbackState(command, func_002E5B10, 0);
    WaitSema(D_003BD3F4);
    func_002E6E08(state);
    return state;
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5BB8);

void func_002E5C38(u32 arg0) {
    func_002E6E38();
    WaitSema(D_003BD3F4);
    func_002E6E88(arg0);
}

void func_002E5C68(void) {
    func_002E6D48();
    WaitSema(D_003BD3F4);
}

u32 func_002E5C88(void) {
    func_002E6CF0();
    WaitSema(D_003BD3F4);
    return D_003BDA68;
}

u32 func_002E5CB0(void) {
    func_002E6C90();
    WaitSema(D_003BD3F4);
    return D_003BDA6C;
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5CD8);

char *func_002E5D70(void) {
    return D_00398820;
}

void func_002E5D80(s32 arg0) {
    D_003BD3F0 = arg0;
    func_002E5D98(arg0);
}

void func_002E5D98(s32 arg0) {
    D_003BD42E = arg0;
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5DA0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5E90);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5F08);

void sdfDevRelease(DevState *arg0) {
    s32 id = arg0->unk28;

    arg0->unk28 = -1;
    if (id >= 0) {
        func_0030EB78(id);
    }
    func_002E5F08(arg0);
}

void sdfDevDeactivate(DevState *arg0, s32 arg1) {
    arg0->unk2C = arg1;
    arg0->unk16 = 9;
    sdfDevRelease(arg0);
    if (arg0->unk38 != NULL) {
        arg0->unk38(arg0, 0, 0, 0, arg0->unk3C);
    }
}

INCLUDE_RODATA(const s32, "game/code_002E4720", D_003B4578);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E60B0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E67A8);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E69F0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E6B28);

DevState *sdfDevCreateCallbackState(s32 path, void (*callback)(DevState *, s32, s32, s32, s32),
                        s32 context) {
    void *resource;
    s32 id = func_002E69F0(path, &resource);
    DevState *state;

    if (id < 0) {
        return NULL;
    }
    state = func_002E6B28(resource, id, 1, callback, context);
    func_002E5DA0(state);
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
    state = func_002E6B28(resource, id, 2, callback, context);
    state->unk24 = options != 0 ? options : D_003BD434;
    func_002E5DA0(state);
    return state;
}

s32 func_002E6C90(DevState *arg0, s32 arg1, s32 arg2) {
    if (arg0->unk16 != 7) {
        return -1;
    }
    arg0->unk18 = arg1;
    arg0->unk24 = arg2;
    arg0->unk15 = 3;
    SignalSema(D_00398864[arg0->unk14].sema);
    return 0;
}

s32 func_002E6CF0(DevState *arg0) {
    if (arg0->unk16 != 7) {
        return -1;
    }
    arg0->unk15 = 4;
    SignalSema(D_00398864[arg0->unk14].sema);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E6D48);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E6DA8);

s32 func_002E6E08(DevState *arg0) {
    if (arg0->unk16 != 9) {
        return -1;
    }
    arg0->unk2C = 0;
    arg0->unk16 = 7;
    return 0;
}

s32 func_002E6E38(DevState *arg0) {
    s8 state = arg0->unk16;

    if (state != 7) {
        return -1;
    }
    arg0->unk15 = state;
    SignalSema(D_00398864[arg0->unk14].sema);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E6E88);

DevState *sdfDevCreateRequest(s32 path, s32 data, s32 extra,
                        void (*context)(DevState *, s32, s32, s32, s32), s32 callback) {
    void *resource;
    s32 id = func_002E69F0(path, &resource);
    DevState *state;

    if (id < 0) {
        return NULL;
    }
    state = func_002E6B28(resource, id, 8, context, callback);
    state->unk1C = extra;
    state->unk20 = data;
    state->unk18 = 0;
    func_002E5DA0(state);
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
    state = func_002E6B28(resource, id, 9, context, callback);
    state->unk1C = extra;
    state->unk20 = data;
    state->unk24 = options != 0 ? options : D_003BD434;
    state->unk18 = 0;
    func_002E5DA0(state);
    return state;
}

void sdfSetThreadPriorities(s32 arg0) {
    SemaEntry *p;
    u32 i;

    if (D_003BD430 == arg0) {
        return;
    }
    D_003BD430 = arg0;
    p = D_00398860;
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

void func_002E7078(void) {
    D_003BD42F = 3;
    sdfSetThreadPriorities(0x78);
}

void func_002E7098(void) {
    sdfSetThreadPriorities(0x48);
}

void func_002E70B0(void) {
    u8 val = D_003BD42F;
    u8 next;

    if (val == 0) {
        return;
    }
    D_003BD42F = val - 1;
    next = val - 1;
    if (next != 0) {
        return;
    }
    func_002E7098();
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E70F0);

void sdfPowerOffLoop(s32 arg0) {
    s32 status;

    for (;;) {
        WaitSema(arg0);
        func_003110C8(D_003BD460, 0x5003, 0, 0, 0, 0);
        func_003110C8(D_003BD468, 0x4806, 0, 0, 0, 0);
        sceCdPowerOff(&status);
    }
}

void func_002E7210(void) {
    iSignalSema();
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E7228);

void func_002E7370(void) {
    s32 resident;
    if (D_003BD477 == 0) {
        D_003BD478 = 0;
        if (func_00312618("cdrom0:\\IRX\\DEV9.IRX;1", 0, NULL, &resident) < 0) {
            func_003003F0("cdrom0:\\IRX\\DEV9.IRX;1 could't load.\n");
            return;
        }
        if (resident != 0) {
            func_003003F0("cdrom0:\\IRX\\DEV9.IRX;1 resident fail.\n");
            return;
        }
        D_003BD478 = 1;
        D_003BD477 = 1;
    }
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E73F0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E7480);

char *sdfStrDup(const char *arg0) {
    u32 len;
    char *buf;

    if (arg0 == NULL) {
        return NULL;
    }
    len = strlen(arg0);
    buf = func_002CFEB8(len + 1);
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
    DevRequest *request = func_002CFEB8(sizeof(*request));

    request->mode = mode;
    request->flags = 0;
    request->count = count;
    request->stride = stride;
    if (count != 0) {
        request->handle = func_002D03F8(stride * count);
        request->buffer = func_002D0A48(request->handle);
    } else {
        request->handle = 0;
        request->buffer = 0;
    }
    return request;
}

void func_002E7680(s32 *arg0) {
    func_002D0918(*arg0);
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E76B0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E7730);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E77F8);

void func_002E78F8(f32 arg0) {
    func_002E77F8(arg0 + 1.5707963f);
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E7918);

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

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E7AA8);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E7B20);

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

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD3C8);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD3D0);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD3D8);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD3E0);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD3E8);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD3F0);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD3F4);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD3F8);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD400);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD408);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD410);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD418);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD41C);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD420);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD424);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD428);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD42C);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD42E);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD42F);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD430);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD434);

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

