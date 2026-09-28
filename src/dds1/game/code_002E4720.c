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
extern void func_002E6BA8(s32 arg0, void *callback, s32 arg2);
extern void func_002E77F8(f32 arg0);
extern void func_0030EB78(s32 arg0);
extern s32 func_00312C08(DevState *arg0);
extern void func_003110C8(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void EIntr(void);
extern void sceCdPowerOff(void *arg0);
extern s32 func_002CF440(s32 arg0, s32 arg1, s32 arg2);
extern s32 func_002CF670(const char *arg0);
extern void *func_002CFEB8(s32 size);
extern void func_002CFF98(void *ptr);
extern s32 func_002D03F8(void);
extern void func_002D0750(s32 arg0, s32 arg1);
extern s32 func_002D0A48(s32 arg0);
extern s32 func_002D0A60(s32 arg0);
extern u32 strlen(const char *s);
extern void func_002E7730(CmdPkt *arg0, s32 arg1);
extern void func_002F4190(u32 arg0);
extern u32 D_003BD3E8;
extern u8 D_003BD460[];
extern u8 D_003BD468[];

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4720);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4908);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4960);

void func_002E49E8(SifCommand *arg0, s32 arg1) {
    arg0->unkC = D_003987E0[arg1];
}

void func_002E4A00(SifCommand *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    arg0->unk0 = arg1;
    arg0->unk4 = arg2;
    arg0->unk8 = arg3;
    func_002E49E8(arg0, arg4);
}

void *func_002E4A28(s32 arg0, SifCommand *arg1) {
    s32 size = arg1->unk4 - arg1->unk0;

    if (size > 0) {
        memcpy(arg1, (void *)arg1->unk0, size);
        return arg1;
    }
    return NULL;
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4A80);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4AE0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4B80);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4C28);

u32 func_002E4CA8(void) {
    return D_003BDA44;
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4CB0);

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

s32 func_002E5640(u32 arg0) {
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
    D_003BDA58 = func_002CF440(1, 0xff, 0);
    D_003BDA64 = func_002CF440(0, 0xff, 0);
    D_003BD3D8 = 0;
    func_002F3CB8(0);
    func_002F4190(D_003BD3E8);
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5808);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5880);

void func_002E5918(char *arg0, char *arg1) {
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

void func_002E5B10(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (arg1 != 3) {
        if (arg1 == 4) {
            D_003BDA68 = arg3;
        }
    } else {
        D_003BDA6C = arg3;
    }
    SignalSema(D_003BD3F4);
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5B50);

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

void func_002E6020(DevState *arg0) {
    s32 id = arg0->unk28;

    arg0->unk28 = -1;
    if (id >= 0) {
        func_0030EB78(id);
    }
    func_002E5F08(arg0);
}

void func_002E6060(DevState *arg0, s32 arg1) {
    arg0->unk2C = arg1;
    arg0->unk16 = 9;
    func_002E6020(arg0);
    if (arg0->unk38 != NULL) {
        arg0->unk38(arg0, 0, 0, 0, arg0->unk3C);
    }
}

INCLUDE_RODATA(const s32, "game/code_002E4720", D_003B4578);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E60B0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E67A8);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E69F0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E6B28);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E6BA8);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E6C10);

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

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E6EC8);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E6F58);

void func_002E7000(s32 arg0) {
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
    func_002E7000(0x78);
}

void func_002E7098(void) {
    func_002E7000(0x48);
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

void func_002E71A0(s32 arg0) {
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

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E7370);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E73F0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E7480);

char *func_002E74F8(const char *arg0) {
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

s32 func_002E7568(s32 arg0) {
    s32 place = 1;
    s32 acc = 0;

    while (arg0 > 0) {
        acc += (arg0 & 0xf) * place;
        arg0 >>= 4;
        place *= 10;
    }
    return acc;
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E75A0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E75F0);

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

f32 func_002E79C0(f32 arg0) {
    f32 x2 = arg0 * arg0;
    f32 x3 = x2 * arg0;
    f32 x5 = x2 * x3;

    return arg0 * 0.99999977f + x3 * -0.33325735f + x5 * 0.19388643f;
}

f32 func_002E79F0(f32 arg0, f32 arg1) {
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
        r = func_002E79C0(arg0 / arg1);
    } else {
        r = 1.5707963f - func_002E79C0(arg1 / arg0);
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

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E7BA8);
