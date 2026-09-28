#include "common.h"

/* Effect parameter-set dispatch tables. Every effect kind owns one 0x28-byte
 * entry per table; the handler lives at +0x0. Slots are declared as separate
 * arrays (D_00353710/14/18/1C/20/24/28/2C/30/34 and D_00353880/84/88/90/94/
 * 98/9C/A0/A4). The family2 create table (D_00353880) additionally carries a
 * fallback selector at +0x0C: nonzero calls the entry handler directly,
 * zero falls back through D_003536A0.
 */
typedef struct EffDispatchEntry {
    void *(*func)(void *); /* 0x00 handler, may be NULL */
    u8 pad04[0x08];        /* 0x04 */
    u32 unk0C;             /* 0x0C fallback selector (create table only) */
    u8 pad10[0x18];        /* 0x10 */
} EffDispatchEntry; /* 0x28 */

/* 8-byte parameter work (family1): kind id plus one data pointer. */
typedef struct EffParamWork {
    u16 id;       /* 0x00 effect kind */
    u8 pad02[2];  /* 0x02 */
    void *data;   /* 0x04 parameter block */
} EffParamWork; /* 0x08 */

extern EffDispatchEntry D_003B0040[];

extern void *func_00328D68(s32 size);

extern EffDispatchEntry D_003B004C[];

extern EffDispatchEntry D_003B0050[];

extern EffDispatchEntry D_003B0054[];

extern EffDispatchEntry D_003B0058[];

extern EffDispatchEntry D_003B005C[];

extern EffDispatchEntry D_003B0060[];

extern EffDispatchEntry D_003B0064[];

/* Init work behind func_00162CB8: flag word plus an optional parameter
 * block whose +0x20 word is reset to 1.0f.
 */
typedef struct EffInitWork {
    u32 flags;      /* 0x00 bit0 cleared on init */
    u8 pad04[0x18]; /* 0x04 */
    void *param;    /* 0x1C optional block */
} EffInitWork; /* 0x20 */

extern u8 D_003B0180[];

extern u8 D_003B0190[];

extern u8 D_003B01A0[];

extern void func_00232BC0();

extern void func_00232AA0(void *work);

extern void func_00232AD0(void *work);

extern void func_00232B40(void *work);

extern void func_002328B8(void *work, s32 arg1, s32 arg2);

extern void *func_00232198(void *arg0, void *arg1);

extern void *func_00232EE8(void *arg);

extern void *func_00232EF8(void *arg);

/* 12-byte parameter work (family2): full-word id plus two data words. */
typedef struct EffParamWorkEx {
    u32 id;       /* 0x00 effect kind */
    u32 unk04;    /* 0x04 */
    void *data;   /* 0x08 parameter block */
} EffParamWorkEx; /* 0x0C */

extern EffDispatchEntry D_003B01C0[];

extern EffDispatchEntry D_003B01C4[];

extern EffDispatchEntry D_003B01C8[];

extern EffDispatchEntry D_003B01D0[];

extern EffDispatchEntry D_003B01CC[];

extern EffDispatchEntry D_003B01D4[];

INCLUDE_ASM(const s32, "game/code_0016A100", func_0016A100);

INCLUDE_ASM(const s32, "game/code_0016A100", func_0016A438);

void func_0016A578(void) {
    func_001027D8(0, 0, 0, 0);
}

u32 func_0016A5A0(s32 arg0) {
    return *(u32 *)(arg0 + 4);
}

u16 func_0016A5A8(u16 *arg0) {
    return *arg0;
}

EffParamWork *func_0016A5B0(u16 id, void *data) {
    EffParamWork *work;

    work = func_00328D68(8);
    work->id = id;
    work->data = D_003B0040[id].func(data);
    return work;
}

INCLUDE_ASM(const s32, "game/code_0016A100", func_0016A620);

INCLUDE_ASM(const s32, "game/code_0016A100", func_0016A668);

EffParamWork *func_0016A6A0(EffParamWork *src) {
    EffParamWork *work;

    work = func_00328D68(8);
    work->id = src->id;
    work->data = D_003B004C[src->id].func(src->data);
    return work;
}

void func_0016A710(EffParamWork *work) {
    if (D_003B0050[work->id].func != NULL) {
        D_003B0050[work->id].func(work->data);
    }
}

void func_0016A750(EffParamWork *work) {
    if (D_003B0054[work->id].func != NULL) {
        D_003B0054[work->id].func(work->data);
    }
}

void func_0016A790(EffParamWork *work) {
    if (D_003B0058[work->id].func != NULL) {
        D_003B0058[work->id].func(work->data);
    }
}

void func_0016A7D0(EffParamWork *work) {
    if (D_003B005C[work->id].func != NULL) {
        D_003B005C[work->id].func(work->data);
    }
}

void func_0016A810(EffParamWork *work) {
    if (D_003B0060[work->id].func != NULL) {
        D_003B0060[work->id].func(work->data);
    }
}

void func_0016A850(EffParamWork *work) {
    if (D_003B0064[work->id].func != NULL) {
        D_003B0064[work->id].func(work->data);
    }
}

void func_0016A890(u32 arg0) {
    func_00159978(0, arg0);
}

void func_0016A8B0(u32 arg0) {
    func_00159978(1, arg0);
}

INCLUDE_ASM(const s32, "game/code_0016A100", func_0016A8D0);

void func_0016A8E8(EffInitWork *work) {
    __asm__ volatile ("lqc2 $vf10, 0(%0)" :: "r" (&D_003B0180));
    func_00232AA0(work);
    __asm__ volatile ("lqc2 $vf10, 0(%0)" :: "r" (&D_003B0190));
    func_00232AD0(work);
    __asm__ volatile ("lqc2 $vf10, 0(%0)" :: "r" (&D_003B01A0));
    func_00232B40(work);
    func_00232BC0(work, 0x80808080);
    if (work->param != NULL) {
        func_002328B8(work, 0, 0);
        *(f32 *)((u8 *)work->param + 0x20) = 1.0f;
    }
    work->flags &= ~1u;
}

INCLUDE_ASM(const s32, "game/code_0016A100", func_0016A990);

INCLUDE_ASM(const s32, "game/code_0016A100", func_0016AA18);

void func_0016AA38(void) {
    func_002322E8();
}

void *func_0016AA50(void *arg0) {
    void *a;
    void *b;
    void *work;

    a = func_00232EE8(arg0);
    b = func_00232EF8(arg0);
    work = func_00232198(a, b);
    func_0016A8E8(work);
    return work;
}

INCLUDE_ASM(const s32, "game/code_0016A100", func_0016AAA8);

void func_0016AAC0(void *arg0, f32 x) {
    f32 v[3];

    v[0] = v[1] = v[2] = x;
    __asm__ volatile (
        ".set noreorder\n\t"
        "lqc2 vf10, 0(%0)\n\t"
        ".set reorder"
        : : "r" (v) : "memory");
    func_00232B40(arg0);
}

void func_0016AAF0(void *work, void *src) {
    u8 *d0;
    u8 *d1;
    u8 *d2;

    __asm__ volatile (
        ".set noreorder\n\t"
        "lqc2 vf28, 0(%0)\n\t"
        "lqc2 vf29, 16(%0)\n\t"
        "lqc2 vf30, 32(%0)\n\t"
        "lqc2 vf31, 48(%0)\n\t"
        ".set reorder"
        : : "r" (src) : "memory");
    d0 = *(u8 **)((u8 *)work + 0x18) + 0x20;
    __asm__ volatile (
        ".set noreorder\n\t"
        "sqc2 vf28, 0(%0)\n\t"
        ".set reorder"
        : : "r" (d0) : "memory");
    d1 = *(u8 **)((u8 *)work + 0x18) + 0x30;
    __asm__ volatile (
        ".set noreorder\n\t"
        "sqc2 vf29, 0(%0)\n\t"
        ".set reorder"
        : : "r" (d1) : "memory");
    d2 = *(u8 **)((u8 *)work + 0x18) + 0x40;
    __asm__ volatile (
        ".set noreorder\n\t"
        "sqc2 vf30, 0(%0)\n\t"
        ".set reorder"
        : : "r" (d2) : "memory");
}

void func_0016AB30(void) {
    func_00232BC0();
}

INCLUDE_ASM(const s32, "game/code_0016A100", func_0016AB48);

INCLUDE_ASM(const s32, "game/code_0016A100", func_0016ABF8);

INCLUDE_ASM(const s32, "game/code_0016A100", func_0016AC40);

INCLUDE_ASM(const s32, "game/code_0016A100", func_0016AC78);

void func_0016AD20(EffParamWorkEx *work) {
    if (D_003B01C0[work->id].func != NULL) {
        D_003B01C0[work->id].func(work->data);
    }
}

void func_0016AD60(EffParamWorkEx *work) {
    if (D_003B01C4[work->id].func != NULL) {
        D_003B01C4[work->id].func(work->data);
    }
}

void func_0016ADA0(EffParamWorkEx *work) {
    if (D_003B01C8[work->id].func != NULL) {
        D_003B01C8[work->id].func(work->data);
    }
}

void func_0016ADE0(EffParamWorkEx *work) {
    if (D_003B01D0[work->id].func != NULL) {
        D_003B01D0[work->id].func(work->data);
    }
}

void func_0016AE20(EffParamWorkEx *work) {
    if (D_003B01CC[work->id].func != NULL) {
        D_003B01CC[work->id].func(work->data);
    }
}

void func_0016AE60(EffParamWorkEx *work) {
    if (D_003B01D4[work->id].func != NULL) {
        D_003B01D4[work->id].func(work->data);
    }
}

u32 func_0016AEA0(u32 *arg0) {
    return *arg0;
}

u32 func_0016AEA8(s32 arg0) {
    return *(u32 *)(arg0 + 4);
}

void *func_0016AEB0(void *arg0, s32 index) {
    u8 *data = (u8 *)arg0;
    u8 *row = data + index * 16;

    return data + *(s32 *)(row + 0x14);
}

u32 func_0016AEC8(void *arg0, s32 index) {
    u8 *data = (u8 *)arg0;

    data += index * 16;
    return *(u32 *)(data + 0x10);
}

u32 func_0016AED8(void *arg0, s32 index) {
    u8 *data = (u8 *)arg0;

    data += index * 16;
    return *(u32 *)(data + 0x18);
}

INCLUDE_ASM(const s32, "game/code_0016A100", func_0016AEE8);

INCLUDE_ASM(const s32, "game/code_0016A100", func_0016AF38);
