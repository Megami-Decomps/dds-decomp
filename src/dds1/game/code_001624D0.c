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

/* 12-byte parameter work (family2): full-word id plus two data words. */
typedef struct EffParamWorkEx {
    u32 id;       /* 0x00 effect kind */
    u32 unk04;    /* 0x04 */
    void *data;   /* 0x08 parameter block */
} EffParamWorkEx; /* 0x0C */

/* Init work behind effParamInitWork: flag word plus an optional parameter
 * block whose +0x20 word is reset to 1.0f.
 */
typedef struct EffInitWork {
    u32 flags;      /* 0x00 bit0 cleared on init */
    u8 pad04[0x18]; /* 0x04 */
    void *param;    /* 0x1C optional block */
} EffInitWork; /* 0x20 */

extern EffDispatchEntry D_00353710[];
extern EffDispatchEntry D_00353714[];
extern EffDispatchEntry D_00353718[];
extern EffDispatchEntry D_0035371C[];
extern EffDispatchEntry D_00353720[];
extern EffDispatchEntry D_00353724[];
extern EffDispatchEntry D_00353728[];
extern EffDispatchEntry D_0035372C[];
extern EffDispatchEntry D_00353730[];
extern EffDispatchEntry D_00353734[];
extern EffDispatchEntry D_00353880[];
extern EffDispatchEntry D_00353884[];
extern EffDispatchEntry D_00353888[];
extern EffDispatchEntry D_00353890[];
extern EffDispatchEntry D_00353894[];
extern EffDispatchEntry D_00353898[];
extern EffDispatchEntry D_0035389C[];
extern EffDispatchEntry D_003538A0[];
extern EffDispatchEntry D_003538A4[];
extern u8 D_00353850[];
extern u8 D_00353860[];
extern u8 D_00353870[];
extern u8 D_00325828[];
extern u16 D_003BB044;

extern void *func_002CFEB8(s32 size);
extern void func_002CFF98(void *p);
extern void mdlBroadcastMasked();
extern void func_00152000(f32 arg0, f32 arg1);
extern void func_00217878(void *arg0, void *arg1);
extern void func_00217F88(void *work);
extern void func_00217FB8(void *work);
extern void func_00218028(void *work);
extern void mdlAddEntryFlagged(void *work, s32 arg1, s32 arg2);
extern void loadModelViewerPackage(s32 arg0, u16 arg1, s32 arg2, void *arg3, u32 arg4);
extern void *func_00217680(void *arg0, void *arg1);
extern void *func_002183D0(void *arg);
extern void *func_002183E0(void *arg);

INCLUDE_ASM(const s32, "game/code_001624D0", func_001624D0);

INCLUDE_ASM(const s32, "game/code_001624D0", func_00162808);

void func_00162948(void) {
    func_001028E8(0, 0, 0, 0);
}

u32 func_00162970(s32 arg0) {
    return *(u32 *)(arg0 + 4);
}

u16 func_00162978(u16 *arg0) {
    return *arg0;
}

EffParamWork *effParamWorkCreate(u16 id, void *data) {
    EffParamWork *work;

    work = func_002CFEB8(8);
    work->id = id;
    work->data = D_00353710[id].func(data);
    return work;
}

INCLUDE_ASM(const s32, "game/code_001624D0", func_001629F0);

INCLUDE_ASM(const s32, "game/code_001624D0", func_00162A38);

EffParamWork *effParamWorkDuplicate(EffParamWork *src) {
    EffParamWork *work;

    work = func_002CFEB8(8);
    work->id = src->id;
    work->data = D_0035371C[src->id].func(src->data);
    return work;
}

void effParamWorkCallback0(EffParamWork *work) {
    if (D_00353720[work->id].func != NULL) {
        D_00353720[work->id].func(work->data);
    }
}

void effParamWorkCallback1(EffParamWork *work) {
    if (D_00353724[work->id].func != NULL) {
        D_00353724[work->id].func(work->data);
    }
}

void effParamWorkCallback2(EffParamWork *work) {
    if (D_00353728[work->id].func != NULL) {
        D_00353728[work->id].func(work->data);
    }
}

void effParamWorkCallback3(EffParamWork *work) {
    if (D_0035372C[work->id].func != NULL) {
        D_0035372C[work->id].func(work->data);
    }
}

void effParamWorkCallback4(EffParamWork *work) {
    if (D_00353730[work->id].func != NULL) {
        D_00353730[work->id].func(work->data);
    }
}

void effParamWorkCallback5(EffParamWork *work) {
    if (D_00353734[work->id].func != NULL) {
        D_00353734[work->id].func(work->data);
    }
}

void func_00162C60(u32 arg0) {
    billCreateIndexed(0, arg0);
}

void func_00162C80(u32 arg0) {
    billCreateIndexed(1, arg0);
}

void effParamDispatchFloat(f32 arg0) {
    func_00152000(arg0, arg0);
}

void effParamInitWork(EffInitWork *work) {
    __asm__ volatile ("lqc2 $vf10, 0(%0)" :: "r" (&D_00353850));
    func_00217F88(work);
    __asm__ volatile ("lqc2 $vf10, 0(%0)" :: "r" (&D_00353860));
    func_00217FB8(work);
    __asm__ volatile ("lqc2 $vf10, 0(%0)" :: "r" (&D_00353870));
    func_00218028(work);
    mdlBroadcastMasked(work, 0x80808080);
    if (work->param != NULL) {
        mdlAddEntryFlagged(work, 0, 0);
        *(f32 *)((u8 *)work->param + 0x20) = 1.0f;
    }
    work->flags &= ~1u;
}

void *effParamCreateInitWork(void *arg0) {
    void *work;

    loadModelViewerPackage(7, D_003BB044, 0x101, (u8 *)arg0 + 0x10, *(u32 *)arg0);
    work = func_00217680((void *)7, (void *)(u32)D_003BB044);
    effParamInitWork(work);
    D_003BB044++;
    return work;
}

void effParamInitFromGlobal(void *arg0) {
    func_00217878(arg0, &D_00325828);
}

void func_00162DE0(void) {
    func_002177D0();
}

void *effParamAssembleWork(void *arg0) {
    void *a;
    void *b;
    void *work;

    a = func_002183D0(arg0);
    b = func_002183E0(arg0);
    work = func_00217680(a, b);
    effParamInitWork(work);
    return work;
}

void effParamForwardVector(void *arg0, void *vec) {
    __asm__ volatile (
        ".set noreorder\n\t"
        "lqc2 vf10, 0(%0)\n\t"
        ".set reorder"
        : : "r" (vec) : "memory");
    func_00217F88(arg0);
}

void effParamBuildVector(void *arg0, f32 x) {
    f32 v[3];

    v[0] = v[1] = v[2] = x;
    __asm__ volatile (
        ".set noreorder\n\t"
        "lqc2 vf10, 0(%0)\n\t"
        ".set reorder"
        : : "r" (v) : "memory");
    func_00218028(arg0);
}

void effParamScatterVectors(void *work, void *src) {
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

void func_00162ED8(void) {
    mdlBroadcastMasked();
}

INCLUDE_ASM(const s32, "game/code_001624D0", func_00162EF0);

INCLUDE_ASM(const s32, "game/code_001624D0", func_00162FA0);

INCLUDE_ASM(const s32, "game/code_001624D0", func_00162FE8);

INCLUDE_ASM(const s32, "game/code_001624D0", func_00163020);

void effParamWorkExCallback0(EffParamWorkEx *work) {
    if (D_00353890[work->id].func != NULL) {
        D_00353890[work->id].func(work->data);
    }
}

void effParamWorkExCallback1(EffParamWorkEx *work) {
    if (D_00353894[work->id].func != NULL) {
        D_00353894[work->id].func(work->data);
    }
}

void effParamWorkExCallback2(EffParamWorkEx *work) {
    if (D_00353898[work->id].func != NULL) {
        D_00353898[work->id].func(work->data);
    }
}

void effParamWorkExCallback3(EffParamWorkEx *work) {
    if (D_003538A0[work->id].func != NULL) {
        D_003538A0[work->id].func(work->data);
    }
}

void effParamWorkExCallback4(EffParamWorkEx *work) {
    if (D_0035389C[work->id].func != NULL) {
        D_0035389C[work->id].func(work->data);
    }
}

void effParamWorkExCallback5(EffParamWorkEx *work) {
    if (D_003538A4[work->id].func != NULL) {
        D_003538A4[work->id].func(work->data);
    }
}

u32 func_00163248(u32 *arg0) {
    return *arg0;
}

u32 func_00163250(s32 arg0) {
    return *(u32 *)(arg0 + 4);
}

void *effParamTableGetBlock(void *arg0, s32 index) {
    u8 *data = (u8 *)arg0;
    u8 *row = data + index * 16;

    return data + *(s32 *)(row + 0x14);
}

u32 effParamTableGetWord(void *arg0, s32 index) {
    u8 *data = (u8 *)arg0;

    data += index * 16;
    return *(u32 *)(data + 0x10);
}

u32 effParamTableGetWord2(void *arg0, s32 index) {
    u8 *data = (u8 *)arg0;

    data += index * 16;
    return *(u32 *)(data + 0x18);
}

EffParamWork *effParamCreateFromTable(EffParamWork *work, s32 index) {
    u16 id;
    void *data;

    id = (u16)effParamTableGetWord2(work, index);
    data = effParamTableGetBlock(work, index);
    return effParamWorkCreate(id, data);
}

INCLUDE_ASM(const s32, "game/code_001624D0", func_001632E0);

INCLUDE_SDATA(const s32, "game/code_001624D0", D_003BB044);

