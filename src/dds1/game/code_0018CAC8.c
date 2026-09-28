#include "common.h"

/* Dispatch object: a type-selected handler plus instance words. */
typedef struct EffWork {
    u32 type;       /* 0x0: index into the handler tables below */
    void *unk4;     /* 0x4: argument passed to the handler */
    u32 unk8;       /* 0x8: node list freed by func_0018D3D0 */
    u8 unkC[8];     /* 0xC */
    u32 unk14;      /* 0x14: read by func_0018D988 */
    u8 unk18[8];    /* 0x18 */
    u32 unk20;      /* 0x20: set by func_0018D9E0 */
    u32 unk24;      /* 0x24: set by func_0018D9E8 */
    u8 unk28[0x10]; /* 0x28 */
    void *unk38;    /* 0x38: next link freed by func_0018D3D0 */
    u32 unk3C;      /* 0x3C: sound handle */
} EffWork;

/* 24-byte handler-table entry (stride selected by type). */
typedef struct EffHandler {
    void (*handler)(void *);
    u8 unk4[0x14];
} EffHandler;

/* Message record with length-prefixed strings at +0x34/+0x40. */
typedef struct EffMsg {
    s32 unk0;      /* 0x0: set by func_0018D978 */
    s32 unk4;      /* 0x4: set by func_0018D978 */
    u8 unk8[0x20]; /* 0x8 */
    u32 unk28;     /* 0x28: set by func_0018D9F0 */
    u32 unk2C;     /* 0x2C: set by func_0018D9F0 */
    u8 unk30[4];   /* 0x30 */
    u32 *unk34;    /* 0x34: words with text at +4 (func_0018D998) */
    u8 unk38[8];   /* 0x38 */
    u32 *unk40;    /* 0x40: words with text at +4 (func_0018D998) */
} EffMsg;

/* 0x44-byte init record built by func_0018D428. */
typedef struct EffBig44 {
    u32 unk0;      /* 0x0 */
    u32 unk4;      /* 0x4 */
    u32 unk8;      /* 0x8 */
    u32 unkC;      /* 0xC: parent type */
    u32 unk10;     /* 0x10 */
    u32 unk14;     /* 0x14 */
    u32 unk18;     /* 0x18 */
    u32 unk1C;     /* 0x1C */
    u32 unk20;     /* 0x20 */
    u32 unk24;     /* 0x24 */
    u32 unk28;     /* 0x28 */
    u32 unk2C;     /* 0x2C */
    u32 unk30;     /* 0x30 */
    u32 unk34;     /* 0x34 */
    u32 unk38;     /* 0x38 */
    u32 unk3C;     /* 0x3C */
    void *unk40;   /* 0x40 */
} EffBig44;

/* 0x38-byte slot with effMath-style defaults (0, 0.05f). */
typedef struct EffSlot38 {
    u8 unk0[0x30]; /* 0x0 */
    s32 unk30;     /* 0x30: cleared by func_0018DF00 */
    f32 unk34;     /* 0x34: set to 0.05f by func_0018DF00 */
} EffSlot38;

/* Array header written past the last slot by func_0018DF00. */
typedef struct EffArrHdr {
    void *unk0; /* 0x0: base */
    u32 unk4;   /* 0x4: count */
    void *unk8; /* 0x8: mem handle */
} EffArrHdr;

/* Directory entry filled by func_0018CEF0. */
typedef struct EffDirEnt {
    u32 unk0;      /* 0x0: flags (bit 12 cleared) */
    u8 unk4[0x3C]; /* 0x4 */
    char unk40[1];  /* 0x40: name */
} EffDirEnt;

extern EffHandler D_00355734[];
extern EffHandler D_00355738[];
extern EffHandler D_0035573C[];
extern EffHandler D_00355740[];
extern EffHandler D_00355744[];
extern u8 D_003BD476;
extern s8 D_003BB04D;
extern u32 D_003BD800;
extern char D_003BB058[];
extern char D_003BB060[];
extern char *D_003557A8[];
extern s32 func_00310320(void);
extern u32 func_002D3288(u32);
extern u64 func_002EB028(u64, u32 *, u64);
extern void func_002CFF98(void *arg0);
extern void func_002D2D00(s32 arg0);
extern void func_002D0918(u64 arg0);
extern void func_001028E8(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void func_003101B8(void);
extern void func_003014F0();
extern s32 sceDopen(void *arg0);
extern void *func_002CFEB8(s32 arg0);
extern void func_002D4010(s32 arg0);
extern void func_002E1370(s32 arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
extern void *func_002E1420(s32 arg0);
extern s32 func_002E1478(s32 arg0);
extern void func_002D4038(s32 arg0, s32 arg1);

/* Voice parameters read via EffWork.unk3C. */
typedef struct SndVoice {
    u8 unk0[0xC]; /* 0x0 */
    s16 unkC;     /* 0xC */
    s16 unkE;     /* 0xE */
} SndVoice;

/* Sound register block built by func_0018DA70. */
typedef struct SndReg {
    s32 unk0;      /* 0x0 */
    s32 unk4;      /* 0x4 */
    s32 unk8;      /* 0x8 */
    s32 unkC;      /* 0xC */
    s32 unk10;     /* 0x10 */
    s32 unk14;     /* 0x14 */
    u8 unk18[8];   /* 0x18 */
    s32 unk20;     /* 0x20 */
    s32 unk24;     /* 0x24 */
    s32 unk28;     /* 0x28 */
    s32 unk2C;     /* 0x2C */
    s32 unk30;     /* 0x30 */
    s32 unk34;     /* 0x34 */
    u8 unk38[8];   /* 0x38 */
    s32 unk40;     /* 0x40 */
    s32 unk44;     /* 0x44 */
    s32 unk48;     /* 0x48 */
    s32 unk4C;     /* 0x4C */
} SndReg;

/* Sound device with method at +0x10. */
typedef struct SndDev {
    u8 unk0[0x10];            /* 0x0 */
    void (*unk10)(void *, s32); /* 0x10 */
} SndDev;

extern SndDev D_003255A8;
typedef struct EffHandler32 {
    s32 (*handler)(s32);
    u8 unk4[0x14];
} EffHandler32;

extern EffHandler32 D_00355730[];

/* 8-byte result record allocated by func_0018CAC8. */
typedef struct EffResult {
    s32 unk0; /* 0x0: input selector */
    s32 unk4; /* 0x4: handler result */
} EffResult;

EffResult *func_0018CAC8(s32 arg0, s32 arg1) {
    EffResult *mem = func_002CFEB8(8);
    s32 ret = D_00355730[arg0].handler(arg1);

    mem->unk0 = arg0;
    mem->unk4 = ret;
    return mem;
}

void func_0018CB38(EffWork *arg0) {
    D_00355734[arg0->type].handler(arg0->unk4);
}

void func_0018CB70(EffWork *arg0) {
    D_00355738[arg0->type].handler(arg0->unk4);
    func_002CFF98(arg0);
}

u32 func_0018CBB8(EffWork *arg0) {
    return (u32)arg0->unk4;
}

u32 func_0018CBC0(u32 *arg0) {
    return *arg0;
}

void func_0018CBC8(void) {
}

u32 func_0018CBD0(void) {
    return 1;
}

void func_0018CBD8(EffWork *arg0) {
    void (*handler)(void *) = D_0035573C[arg0->type].handler;

    if (handler != NULL) {
        handler(arg0->unk4);
    }
}

void func_0018CC18(EffWork *arg0) {
    void (*handler)(void *) = D_00355744[arg0->type].handler;

    if (handler != NULL) {
        handler(arg0->unk4);
    }
}

void func_0018CC58(EffWork *arg0) {
    void (*handler)(void *) = D_00355740[arg0->type].handler;

    if (handler != NULL) {
        handler(arg0->unk4);
    }
}

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018CC98);

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018CCF0);

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018CD50);

void func_0018CDA0(void) {
}

void func_0018CDA8(void) {
    func_001028E8(0, 0, 0, 0);
}

u32 func_0018CDD0(u32 arg0) {
    return arg0;
}

void func_0018CDD8(void) {
}

void func_0018CDE0(void) {
}

void func_0018CDE8(void) {
}

void func_0018CDF0(void) {
}

void func_0018CDF8(void) {
}

void func_0018CE00(void) {
}

void func_0018CE08(void) {
}

void func_0018CE10(void) {
}

void func_0018CE18(void) {
}

void func_0018CE20(void) {
}

void func_0018CE28(void) {
}

u32 func_0018CE30(u32 arg0) {
    return arg0;
}

void func_0018CE38(void) {
}

u32 func_0018CE40(void) {
    return 0;
}

u32 func_0018CE48(void) {
    return 0;
}

void func_0018CE50(void) {
}

u32 func_0018CE58(void) {
    return 0;
}

void func_0018CE60(void) {
}

s32 func_0018CE68(void) {
    return D_003BB04D;
}

s32 func_0018CE70(void *arg0) {
    u8 buf[0x70];

    if (D_003BD476 != 0) {
        func_003014F0(buf, D_003BB058, arg0);
        return sceDopen(buf);
    } else {
        D_003BD800 = 0;
        return 0;
    }
}


void func_0018CEC0(void) {
    if (D_003BD476 != 0) {
        func_003101B8();
    }
}

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018CEF0);


INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018CF98);

void func_0018D3D0(EffWork *arg0) {
    EffWork *p = (EffWork *)arg0->unk8;

    if (p != NULL) {
        do {
            EffWork *next = p->unk38;
            func_002CFF98(p);
            p = next;
        } while (p != NULL);
    }
    func_002CFF98(arg0->unk4);
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018D428);

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018D4B8);

void func_0018D938(EffWork *arg0) {
    if (arg0->unk3C != 0) {
        func_002D2D00(arg0->unk3C);
        arg0->unk3C = 0;
    }
    func_002CFF98(arg0);
}

void func_0018D978(EffMsg *arg0, s32 arg1, s32 arg2) {
    arg0->unk0 = arg1;
    arg0->unk4 = arg2;
}


u32 func_0018D988(EffWork *arg0) {
    return arg0->unk14;
}

u32 func_0018D990(EffWork *arg0) {
    return arg0->unk8;
}

u32 func_0018D998(EffMsg *arg0, void *arg1) {
    func_003014F0(arg1, D_003BB060, arg0->unk40[1], arg0->unk34 + 1);
    return *arg0->unk34;
}

void func_0018D9E0(EffWork *arg0, u32 arg1) {
    arg0->unk20 = arg1;
}

void func_0018D9E8(EffWork *arg0, u32 arg1) {
    arg0->unk24 = arg1;
}

void func_0018D9F0(EffMsg *arg0, u32 arg1, u32 arg2) {
    arg0->unk28 = arg1;
    arg0->unk2C = arg2;
}

void func_0018DA00(EffWork *arg0, u64 arg1) {
    u32 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    if (arg0->unk3C != 0) {
        func_002D2D00(arg0->unk3C);
        arg0->unk3C = 0;
    }
    temp_v1 = func_002EB028(arg1, temp_v2, 0);
    temp_v0 = func_002D3288(temp_v2[0]);
    arg0->unk3C = temp_v0;
    func_002D0918(temp_v1);
}

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018DA70);

void func_0018DB88(void) {
    func_001028E8(0, 0, 0, 0);
}

void func_0018DBB0(void) {
    func_001028E8(0, 0, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018DBD8);

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018DC58);

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018DD40);

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018DDF8);

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018DE78);

void *func_0018DF00(s32 n) {
    void *mem1 = func_002D03F8(n * 0x38 + 0xC);
    void *mem2 = func_002D0A48(mem1);
    u32 i = 0;
    EffSlot38 *r = mem2;
    u8 *end = (u8 *)r + n * 0x38;

    ((EffArrHdr *)end)->unk8 = mem1;
    ((EffArrHdr *)end)->unk0 = mem2;
    ((EffArrHdr *)end)->unk4 = n;
    if (n != 0) {
        do {
            i++;
            r->unk30 = 0;
            r->unk34 = 0.05f;
            r = (EffSlot38 *)((u8 *)r + 0x38);
        } while (i < n);
    }
    return end;
}


INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0F88);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0F98);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0FA8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0FB8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0FC8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0FD8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0FE8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0FF8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1008);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1018);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1028);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1038);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1048);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1058);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1068);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1078);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1088);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1098);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A10A8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A10B8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A10C8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A10D8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A10E8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A10F8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1108);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1118);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1128);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1138);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1148);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1158);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1168);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1178);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1188);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1198);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A11A8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A11B8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A11C8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A11D8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A11E8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A11F8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1208);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1218);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1228);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1238);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1248);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1258);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1270);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1280);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1290);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A12A0);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A12B0);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A12C0);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A12D0);
