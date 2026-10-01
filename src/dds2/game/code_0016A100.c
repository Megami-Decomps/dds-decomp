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
    void *(*altFunc)(void *); /* 0x0C create table only: set means the entry handler takes the raw source data */
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

/* Init work behind effParamInitWork: flag word plus an optional parameter
 * block whose +0x20 word is reset to 1.0f.
 */
typedef struct EffInitWork {
    u32 flags;      /* 0x00 bit0 cleared on init */
    u8 pad04[0x18]; /* 0x04 */
    void *param;    /* 0x1C optional block */
} EffInitWork; /* 0x20 */

typedef struct EffScatterWork {
    u8 pad00[0x18];
    u8 *destination;
} EffScatterWork;

extern u8 D_003B0180[];

extern u8 D_003B0190[];

extern u8 D_003B01A0[];

extern void mdlBroadcastMasked();

extern void mdlStorePrimaryVectorVU(void *work);

extern void mdlUpdateContextRotationBasisFromQuaternion(void *work);

extern void mdlStoreTertiaryVectorVU(void *work);

extern void mdlAddEntryFlagged(void *work, s32 arg1, s32 arg2);

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

/* Callback table at D_003B0044 (0x28 bytes per entry). */
typedef struct Cb3714C {
    void (*cb)(void *arg); /* 0x00 */
    u8 pad4[0x24];         /* 0x04 */
} Cb3714C;

extern Cb3714C D_003B0044[];

extern void billSetChildScaleComponents(f32 arg0, f32 arg1);

extern u8 D_00380828[];

extern void func_00232390(void *arg0, void *arg1);

extern EffDispatchEntry D_003B01B8[];

extern void sdfReleaseChipBlock(void *p);

extern EffDispatchEntry D_003B01B4[];

INCLUDE_ASM(const s32, "game/code_0016A100", func_0016A100);

INCLUDE_ASM(const s32, "game/code_0016A100", func_0016A438);

void func_0016A578(void) {
    dds3AdminSubmitModeRequest(0, 0, 0, 0);
}

u32 effParamWorkGetData(EffParamWork *work) {
    return (u32)work->data;
}

u16 effParamWorkGetId(EffParamWork *work) {
    return work->id;
}

EffParamWork *effParamWorkCreate(u16 id, void *data) {
    EffParamWork *work;

    work = func_00328D68(8);
    work->id = id;
    work->data = D_003B0040[id].func(data);
    return work;
}

INCLUDE_ASM(const s32, "game/code_0016A100", func_0016A620);

/* Invoke the kind-specific callback on this parameter block. */
void effParamWorkInvokeCallback(EffParamWork *work) {
    u16 id = work->id;

    D_003B0044[id].cb(work->data);
}

EffParamWork *effParamWorkDuplicate(EffParamWork *src) {
    EffParamWork *work;

    work = func_00328D68(8);
    work->id = src->id;
    work->data = D_003B004C[src->id].func(src->data);
    return work;
}

void effParamWorkCallback0(EffParamWork *work) {
    if (D_003B0050[work->id].func != NULL) {
        D_003B0050[work->id].func(work->data);
    }
}

void effParamWorkCallback1(EffParamWork *work) {
    if (D_003B0054[work->id].func != NULL) {
        D_003B0054[work->id].func(work->data);
    }
}

void effParamWorkCallback2(EffParamWork *work) {
    if (D_003B0058[work->id].func != NULL) {
        D_003B0058[work->id].func(work->data);
    }
}

void effParamWorkCallback3(EffParamWork *work) {
    if (D_003B005C[work->id].func != NULL) {
        D_003B005C[work->id].func(work->data);
    }
}

void effParamWorkCallback4(EffParamWork *work) {
    if (D_003B0060[work->id].func != NULL) {
        D_003B0060[work->id].func(work->data);
    }
}

void effParamWorkCallback5(EffParamWork *work) {
    if (D_003B0064[work->id].func != NULL) {
        D_003B0064[work->id].func(work->data);
    }
}

void func_0016A890(u32 index) {
    billCreateIndexed(0, index);
}

void func_0016A8B0(u32 index) {
    billCreateIndexed(1, index);
}

void effParamDispatchFloat(f32 value) {
    billSetChildScaleComponents(value, value);
}

void effParamInitWork(EffInitWork *work) {
    __asm__ volatile ("lqc2 $vf10, 0(%0)" :: "r" (&D_003B0180));
    mdlStorePrimaryVectorVU(work);
    __asm__ volatile ("lqc2 $vf10, 0(%0)" :: "r" (&D_003B0190));
    mdlUpdateContextRotationBasisFromQuaternion(work);
    __asm__ volatile ("lqc2 $vf10, 0(%0)" :: "r" (&D_003B01A0));
    mdlStoreTertiaryVectorVU(work);
    mdlBroadcastMasked(work, 0x80808080);
    if (work->param != NULL) {
        mdlAddEntryFlagged(work, 0, 0);
        *(f32 *)((u8 *)work->param + 0x20) = 1.0f;
    }
    work->flags &= ~1u;
}

INCLUDE_ASM(const s32, "game/code_0016A100", func_0016A990);

void effParamInitFromGlobal(void *work) {
    func_00232390(work, &D_00380828);
}

void func_0016AA38(void) {
    mdlDestroyContext();
}

/* Assemble a parameter work item from the two pieces extracted from source. */
void *effParamAssembleWork(void *source) {
    void *firstPart;
    void *secondPart;
    void *work;

    firstPart = func_00232EE8(source);
    secondPart = func_00232EF8(source);
    work = func_00232198(firstPart, secondPart);
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

void effParamScatterVectors(EffScatterWork *work, void *src) {
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
    d0 = work->destination + 0x20;
    __asm__ volatile (
        ".set noreorder\n\t"
        "sqc2 vf28, 0(%0)\n\t"
        ".set reorder"
        : : "r" (d0) : "memory");
    d1 = work->destination + 0x30;
    __asm__ volatile (
        ".set noreorder\n\t"
        "sqc2 vf29, 0(%0)\n\t"
        ".set reorder"
        : : "r" (d1) : "memory");
    d2 = work->destination + 0x40;
    __asm__ volatile (
        ".set noreorder\n\t"
        "sqc2 vf30, 0(%0)\n\t"
        ".set reorder"
        : : "r" (d2) : "memory");
}

void func_0016AB30(void) {
    mdlBroadcastMasked();
}

extern EffDispatchEntry D_003B01B0[];
extern void **D_003AFFD0[];
extern u32 func_0016AEA0(u32 *word);
extern u32 func_0016AEA8(s32 address);

EffParamWorkEx *effCreateDispatchedParameterWork(u32 *source) {
    EffParamWorkEx *work;

    work = func_00328D68(0xC);
    work->id = func_0016AEA0(source);
    work->unk04 = func_0016AEA8((s32)source);
    if (D_003B01B0[work->id].altFunc == NULL) {
        work->data = D_003B01B0[work->id].func(D_003AFFD0[work->id][work->unk04]);
    } else {
        work->data = D_003B01B0[work->id].func(source);
    }
    return work;
}

void effReleaseDispatchedParameterWork(EffParamWorkEx *work) {
    ((void (*)(void *))D_003B01B8[work->id].func)(work->data);
    sdfReleaseChipBlock(work);
}

void effInvokeParameterWorkDispatch(EffParamWorkEx *work) {
    ((void (*)(void *))D_003B01B4[work->id].func)(work->data);
}

EffParamWorkEx *effCloneDispatchedParameterWork(EffParamWorkEx *src) {
    EffParamWorkEx *work;

    work = func_00328D68(0xC);
    work->id = src->id;
    work->unk04 = src->unk04;
    if (D_003B01B0[work->id].altFunc == NULL) {
        work->data = D_003B01B0[work->id].func(D_003AFFD0[work->id][work->unk04]);
    } else {
        work->data = D_003B01B0[work->id].altFunc(src->data);
    }
    return work;
}

void effParamWorkExCallback0(EffParamWorkEx *work) {
    if (D_003B01C0[work->id].func != NULL) {
        D_003B01C0[work->id].func(work->data);
    }
}

void effParamWorkExCallback1(EffParamWorkEx *work) {
    if (D_003B01C4[work->id].func != NULL) {
        D_003B01C4[work->id].func(work->data);
    }
}

void effParamWorkExCallback2(EffParamWorkEx *work) {
    if (D_003B01C8[work->id].func != NULL) {
        D_003B01C8[work->id].func(work->data);
    }
}

void effParamWorkExCallback3(EffParamWorkEx *work) {
    if (D_003B01D0[work->id].func != NULL) {
        D_003B01D0[work->id].func(work->data);
    }
}

void effParamWorkExCallback4(EffParamWorkEx *work) {
    if (D_003B01CC[work->id].func != NULL) {
        D_003B01CC[work->id].func(work->data);
    }
}

void effParamWorkExCallback5(EffParamWorkEx *work) {
    if (D_003B01D4[work->id].func != NULL) {
        D_003B01D4[work->id].func(work->data);
    }
}

u32 func_0016AEA0(u32 *word) {
    return *word;
}

u32 func_0016AEA8(s32 address) {
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

INCLUDE_ASM(const s32, "game/code_0016A100", func_0016AF38);

INCLUDE_SDATA(const s32, "game/code_0016A100", D_00436434);

