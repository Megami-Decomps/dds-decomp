#include "common.h"

extern u32 func_0032C138(u32);

extern u64 func_00343ED0(u64, u32 *, u64);

extern void *func_00328D68(s32 arg0);

typedef struct EffHandler32 {
    s32 (*handler)(s32);
    u8 unk4[0x14];
} EffHandler32;

extern EffHandler32 D_003B2060[];

/* 8-byte result record allocated by func_0018CAC8. */
typedef struct EffResult {
    s32 unk0; /* 0x0: input selector */
    s32 unk4; /* 0x4: handler result */
} EffResult;

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

extern EffHandler D_003B2064[];

extern EffHandler D_003B206C[];

extern EffHandler D_003B2074[];

extern EffHandler D_003B2070[];

/* Sub-object at EffWork.unk4 with per-type byte slots. */
typedef struct EffSub {
    u8 unk0[0x20];  /* 0x0 */
    u8 unk20;       /* 0x20 */
    u8 unk21[0x1F]; /* 0x21 */
    u8 unk40;       /* 0x40 */
    u8 unk41[0xF];  /* 0x41 */
    u8 unk50;       /* 0x50 */
} EffSub;

extern u8 D_00438B66;

extern u32 D_00438F08;

extern char D_00436448[];

extern void func_0035C860();

extern s32 sceDopen(void *arg0);

extern void func_00328E48(void *arg0);

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

extern char D_00436450[];

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

EffResult *func_00194700(s32 arg0, s32 arg1) {
    EffResult *mem = func_00328D68(8);
    s32 ret = D_003B2060[arg0].handler(arg1);

    mem->unk0 = arg0;
    mem->unk4 = ret;
    return mem;
}

void func_00194770(EffWork *arg0) {
    D_003B2064[arg0->type].handler(arg0->unk4);
}

INCLUDE_ASM(const s32, "game/code_00194700", func_001947A8);

u32 func_001947F0(s32 arg0) {
    return *(u32 *)(arg0 + 4);
}

u32 func_001947F8(u32 *arg0) {
    return *arg0;
}

void func_00194800(void) {
}

u32 func_00194808(void) {
    return 1;
}

void func_00194810(EffWork *arg0) {
    void (*handler)(void *) = D_003B206C[arg0->type].handler;

    if (handler != NULL) {
        handler(arg0->unk4);
    }
}

void func_00194850(EffWork *arg0) {
    void (*handler)(void *) = D_003B2074[arg0->type].handler;

    if (handler != NULL) {
        handler(arg0->unk4);
    }
}

void func_00194890(EffWork *arg0) {
    void (*handler)(void *) = D_003B2070[arg0->type].handler;

    if (handler != NULL) {
        handler(arg0->unk4);
    }
}

void func_001948D0(EffWork *arg0, s32 arg1) {
    u8 v = arg1;
    u32 t = arg0->type;

    switch (t) {
    case 0:
        ((EffSub *)arg0->unk4)->unk20 = v;
        return;
    case 1:
        ((EffSub *)arg0->unk4)->unk40 = v;
        return;
    case 2:
        ((EffSub *)arg0->unk4)->unk50 = v;
        return;
    case 3:
        ((EffSub *)arg0->unk4)->unk50 = v;
        return;
    case 4:
        ((EffSub *)arg0->unk4)->unk50 = v;
        return;
    default:
        return;
    }
}

u32 func_00194928(EffWork *arg0, s32 arg1) {
    u8 v = arg1;
    u32 t = arg0->type;

    switch (t) {
    case 0:
        return ((EffSub *)arg0->unk4)->unk20;
    case 1:
        return ((EffSub *)arg0->unk4)->unk40;
    case 2:
        return ((EffSub *)arg0->unk4)->unk50;
    case 3:
        return ((EffSub *)arg0->unk4)->unk50;
    case 4:
        return ((EffSub *)arg0->unk4)->unk50;
    default:
        break;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00194700", func_00194988);

void func_001949D8(void) {
}

void func_001949E0(void) {
    func_001027D8(0, 0, 0, 0);
}

u32 func_00194A08(u32 arg0) {
    return arg0;
}

void func_00194A10(void) {
}

void func_00194A18(void) {
}

void func_00194A20(void) {
}

void func_00194A28(void) {
}

void func_00194A30(void) {
}

void func_00194A38(void) {
}

void func_00194A40(void) {
}

void func_00194A48(void) {
}

void func_00194A50(void) {
}

void func_00194A58(void) {
}

void func_00194A60(void) {
}

INCLUDE_ASM(const s32, "game/code_00194700", func_00194A68);

void func_00194A70(void) {
}

u32 func_00194A78(void) {
    return 0;
}

u32 func_00194A80(void) {
    return 0;
}

void func_00194A88(void) {
}

INCLUDE_ASM(const s32, "game/code_00194700", func_00194A90);

void func_00194A98(void) {
}

INCLUDE_ASM(const s32, "game/code_00194700", func_00194AA0);

s32 func_00194AA8(void *arg0) {
    u8 buf[0x70];

    if (D_00438B66 != 0) {
        func_0035C860(buf, D_00436448, arg0);
        return sceDopen(buf);
    } else {
        D_00438F08 = 0;
        return 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00194700", func_00194AF8);

INCLUDE_ASM(const s32, "game/code_00194700", func_00194B28);

INCLUDE_ASM(const s32, "game/code_00194700", func_00194BD0);

void func_00195008(EffWork *arg0) {
    EffWork *p = (EffWork *)arg0->unk8;

    if (p != NULL) {
        do {
            EffWork *next = p->unk38;
            func_00328E48(p);
            p = next;
        } while (p != NULL);
    }
    func_00328E48(arg0->unk4);
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_00194700", func_00195060);

INCLUDE_ASM(const s32, "game/code_00194700", func_001950F0);

void func_00195570(u32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 0x3c);
    if (temp_v0 != 0) {
        func_0032BBB0(temp_v0);
        *(u32 *)((s32)arg0 + 0x3c) = 0;
    }
    func_00328E48(arg0);
}

void func_001955B0(EffMsg *arg0, s32 arg1, s32 arg2) {
    arg0->unk0 = arg1;
    arg0->unk4 = arg2;
}

u32 func_001955C0(s32 arg0) {
    return *(u32 *)(arg0 + 0x14);
}

u32 func_001955C8(s32 arg0) {
    return *(u32 *)(arg0 + 8);
}

u32 func_001955D0(EffMsg *arg0, void *arg1) {
    func_0035C860(arg1, D_00436450, arg0->unk40[1], arg0->unk34 + 1);
    return *arg0->unk34;
}

void func_00195618(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x20) = arg1;
}

void func_00195620(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_00195628(EffMsg *arg0, u32 arg1, u32 arg2) {
    arg0->unk28 = arg1;
    arg0->unk2C = arg2;
}

void func_00195638(s32 arg0, u64 arg1) {
    u32 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    if (*(s32 *)(arg0 + 0x3c) != 0) {
        func_0032BBB0(*(s32 *)(arg0 + 0x3c));
        *(u32 *)(arg0 + 0x3c) = 0;
    }
    temp_v1 = func_00343ED0(arg1, temp_v2, 0);
    temp_v0 = func_0032C138(temp_v2[0]);
    *(u32 *)(arg0 + 0x3c) = temp_v0;
    func_003297C8(temp_v1);
}

INCLUDE_ASM(const s32, "game/code_00194700", func_001956A8);

void func_001957C0(void) {
    func_001027D8(0, 0, 0, 0);
}

void func_001957E8(void) {
    func_001027D8(0, 0, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_00194700", func_00195810);

INCLUDE_ASM(const s32, "game/code_00194700", func_00195890);

INCLUDE_ASM(const s32, "game/code_00194700", func_00195978);

INCLUDE_ASM(const s32, "game/code_00194700", func_00195A30);

INCLUDE_ASM(const s32, "game/code_00194700", func_00195AB0);

void *func_00195B38(s32 n) {
    void *mem1 = func_003292A8(n * 0x38 + 0xC);
    void *mem2 = func_003298F8(mem1);
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

INCLUDE_RODATA(const s32, "game/code_00194700", D_004146A8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004146B8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004146C8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004146D8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004146E8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004146F8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414708);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414718);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414728);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414738);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414748);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414758);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414768);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414778);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414788);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414798);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004147A8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004147B8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004147C8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004147D8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004147E8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004147F8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414808);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414818);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414828);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414838);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414848);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414858);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414868);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414878);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414888);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414898);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004148A8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004148B8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004148C8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004148D8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004148E8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004148F8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414908);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414918);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414928);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414938);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414948);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414958);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414968);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414978);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414990);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004149A0);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004149B0);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004149C0);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004149D0);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004149E0);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004149F0);

INCLUDE_SDATA(const s32, "game/code_00194700", D_00436440);

INCLUDE_SDATA(const s32, "game/code_00194700", D_00436448);

INCLUDE_SDATA(const s32, "game/code_00194700", D_00436450);

