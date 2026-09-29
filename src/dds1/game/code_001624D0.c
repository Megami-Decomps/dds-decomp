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

extern void mdlStorePrimaryVectorVU(void *work);

extern void func_00217FB8(void *work);

extern void mdlStoreTertiaryVectorVU(void *work);

extern void mdlAddEntryFlagged(void *work, s32 arg1, s32 arg2);

extern void mdlLoadViewerPackage(s32 arg0, u16 arg1, s32 arg2, void *arg3, u32 arg4);

extern void *func_00217680(void *arg0, void *arg1);

extern void *func_002183D0(void *arg);

extern void *func_002183E0(void *arg);

/* Callback table at D_0037E14C (0x28 bytes per entry). */
typedef struct Cb3714C {
    void (*cb)(void *arg);    /* 0x00 */
    u8 pad4[8];               /* 0x04 */
    void (*cbC)(void *, void *); /* 0x0C */
    void (*cb10)(void *arg);  /* 0x10 */
    void (*cb14)(void *arg);  /* 0x14 */
    void (*cb18)(void *arg, void *extra);  /* 0x18 */
    void (*cb1C)(void *arg);  /* 0x1C */
    void (*cb20)(void *arg);  /* 0x20 */
    u32 unk24;                /* 0x24 */
} Cb3714C;

extern Cb3714C D_00353714[];

INCLUDE_ASM(const s32, "game/code_001624D0", func_001624D0);

INCLUDE_ASM(const s32, "game/code_001624D0", func_00162808);

void func_00162948(void) {
    func_001028E8(0, 0, 0, 0);
}

u32 effParamWorkGetData(EffParamWork *work) {
    return (u32)work->data;
}

u16 effParamWorkGetId(EffParamWork *work) {
    return work->id;
}

EffParamWork *effParamWorkCreate(u16 id, void *data) {
    EffParamWork *work;

    work = func_002CFEB8(8);
    work->id = id;
    work->data = D_00353710[id].func(data);
    return work;
}

INCLUDE_ASM(const s32, "game/code_001624D0", func_001629F0);

INCLUDE_ASM(const s32, "game/code_001624D0", effParamWorkInvokeCallback);

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

void func_00162C60(u32 index) {
    billCreateIndexed(0, index);
}

void func_00162C80(u32 index) {
    billCreateIndexed(1, index);
}

void effParamDispatchFloat(f32 value) {
    func_00152000(value, value);
}

void effParamInitWork(EffInitWork *work) {
    __asm__ volatile ("lqc2 $vf10, 0(%0)" :: "r" (&D_00353850));
    mdlStorePrimaryVectorVU(work);
    __asm__ volatile ("lqc2 $vf10, 0(%0)" :: "r" (&D_00353860));
    func_00217FB8(work);
    __asm__ volatile ("lqc2 $vf10, 0(%0)" :: "r" (&D_00353870));
    mdlStoreTertiaryVectorVU(work);
    mdlBroadcastMasked(work, 0x80808080);
    if (work->param != NULL) {
        mdlAddEntryFlagged(work, 0, 0);
        *(f32 *)((u8 *)work->param + 0x20) = 1.0f;
    }
    work->flags &= ~1u;
}

void *effParamCreateInitWork(void *arg0) {
    void *work;

    mdlLoadViewerPackage(7, D_003BB044, 0x101, (u8 *)arg0 + 0x10, *(u32 *)arg0);
    work = func_00217680((void *)7, (void *)(u32)D_003BB044);
    effParamInitWork(work);
    D_003BB044++;
    return work;
}

void effParamInitFromGlobal(void *work) {
    func_00217878(work, &D_00325828);
}

void func_00162DE0(void) {
    func_002177D0();
}

/* Assemble a parameter work item from the two pieces extracted from source. */
void *effParamAssembleWork(void *source) {
    void *firstPart;
    void *secondPart;
    void *work;

    firstPart = func_002183D0(source);
    secondPart = func_002183E0(source);
    work = func_00217680(firstPart, secondPart);
    effParamInitWork(work);
    return work;
}

void effParamForwardVector(void *work, void *vec) {
    __asm__ volatile (
        ".set noreorder\n\t"
        "lqc2 vf10, 0(%0)\n\t"
        ".set reorder"
        : : "r" (vec) : "memory");
    mdlStorePrimaryVectorVU(work);
}

/* Broadcast one scalar into three components before loading VU0 vf10. */
void effParamBuildVector(void *work, f32 scalar) {
    f32 v[3];

    v[0] = v[1] = v[2] = scalar;
    __asm__ volatile (
        ".set noreorder\n\t"
        "lqc2 vf10, 0(%0)\n\t"
        ".set reorder"
        : : "r" (v) : "memory");
    mdlStoreTertiaryVectorVU(work);
}

/* The scatter work holds its destination vector block at +0x18. */
typedef struct EffScatterWork {
    u8 pad00[0x18];
    u8 *destination;
} EffScatterWork;

void effParamScatterVectors(EffScatterWork *work, void *src) {
    u8 *firstVector;
    u8 *secondVector;
    u8 *thirdVector;

    __asm__ volatile (
        ".set noreorder\n\t"
        "lqc2 vf28, 0(%0)\n\t"
        "lqc2 vf29, 16(%0)\n\t"
        "lqc2 vf30, 32(%0)\n\t"
        "lqc2 vf31, 48(%0)\n\t"
        ".set reorder"
        : : "r" (src) : "memory");
    firstVector = work->destination + 0x20;
    __asm__ volatile (
        ".set noreorder\n\t"
        "sqc2 vf28, 0(%0)\n\t"
        ".set reorder"
        : : "r" (firstVector) : "memory");
    secondVector = work->destination + 0x30;
    __asm__ volatile (
        ".set noreorder\n\t"
        "sqc2 vf29, 0(%0)\n\t"
        ".set reorder"
        : : "r" (secondVector) : "memory");
    thirdVector = work->destination + 0x40;
    __asm__ volatile (
        ".set noreorder\n\t"
        "sqc2 vf30, 0(%0)\n\t"
        ".set reorder"
        : : "r" (thirdVector) : "memory");
}

void func_00162ED8(void) {
    mdlBroadcastMasked();
}

INCLUDE_ASM(const s32, "game/code_001624D0", func_00162EF0);

void func_00162FA0(EffParamWorkEx *work) {
    ((void (*)(void *))D_00353888[work->id].func)(work->data);
    func_002CFF98(work);
}

void func_00162FE8(EffParamWorkEx *work) {
    ((void (*)(void *))D_00353884[work->id].func)(work->data);
}

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

u32 func_00163248(u32 *word) {
    return *word;
}

u32 func_00163250(s32 address) {
    return *(u32 *)(address + 4);
}

/* Table records are 16 bytes apart after a 16-byte header; block offsets
 * are relative to the start of the table, not to each record.
 */
void *effParamTableGetBlock(void *table, s32 index) {
    u8 *data = (u8 *)table;
    u8 *row = data + index * 16;

    return data + *(s32 *)(row + 0x14);
}

u32 effParamTableGetWord(void *table, s32 index) {
    u8 *data = (u8 *)table;

    data += index * 16;
    return *(u32 *)(data + 0x10);
}

u32 effParamTableGetWord2(void *table, s32 index) {
    u8 *data = (u8 *)table;

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

