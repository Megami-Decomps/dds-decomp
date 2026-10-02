#include "common.h"
#include "btl.h"
#include "evt_unit.h"
#include "pcp_vu0.h"

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

extern EffDispatchEntry effParamWorkFactories[];

extern void *func_00328D68(s32 size);

extern EffDispatchEntry effParamWorkDuplicators[];

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

/* Kind-specific callback slot, 0x28 bytes per entry. */
typedef struct EffParamCallbackEntry {
    void (*cb)(void *arg); /* 0x00 */
    u8 pad4[0x24];         /* 0x04 */
} EffParamCallbackEntry;

extern EffParamCallbackEntry D_003B0044[];

extern EffParamCallbackEntry D_003B0048[];

extern u32 effBattleMiscGetTableEntry(s32 index);
extern s32 btlGetRuntime(void);
extern void evtSetUnitRgbTransition(EvtUnit *unit, s32 duration, u32 color);

typedef struct EffBattleUnitRgbCommand {
    u32 color;
    u32 startFrame;
    s32 startDurationIndex;
    u32 endFrame;
    s32 endDurationIndex;
} EffBattleUnitRgbCommand;

extern void billSetChildScaleComponents(f32 arg0, f32 arg1);

extern u8 D_00380828[];

extern void mdlProcessContextNodesAndTransforms(void *arg0, void *arg1);

extern EffDispatchEntry effParamWorkReleaseCallbacks[];

extern void sdfReleaseChipBlock(void *p);

extern EffDispatchEntry D_003B01B4[];

extern u32 sdfAllocGeneralBlock(s32 size);
extern u8 *sdfResourceRetainAddress(u32 handle);
extern void *parAllocateCellSystem(s32 count, s32 perCell, s32 groupDivisor, u32 kind);
extern void parDispatchSub(void *work, s32 sub, void *a2, void *a3);
extern void func_00164C68(void *system, u32 value);
extern void func_00164AE8(void *system, void *a, void *b, void *c);

/* Parameter head (0x4C bytes) copied verbatim into the work. */
typedef struct {
    u8 pad00[0x10];
    u16 systemParam;    /* 0x10 */
    u8 pad12[2];
    u32 count;          /* 0x14 number of cells */
    u8 pad18[4];
    f32 scaledFirst;    /* 0x1C */
    f32 scaledSecond;   /* 0x20 */
    f32 rangeF24;       /* 0x24 */
    u32 spreadA;        /* 0x28 modulus of the first cell counter */
    u32 spreadB;        /* 0x2C modulus of the second cell counter */
    u16 perCell;        /* 0x30 */
    u8 pad32[0x04];
    void *unk38;        /* 0x38 first dispatch argument */
    u32 pad3C;
    void *dispatchArg;  /* 0x40 second dispatch argument */
    u32 pad44;
    void *unk48;        /* 0x48 third dispatch argument */
} EffThunderHead4C;

typedef struct {
    u32 unk00;
    u32 unk04;
    f32 dirA[3];        /* 0x08 */
    f32 dirB[3];        /* 0x14 */
    f32 f20;            /* 0x20 */
    f32 f24;            /* 0x24 */
    u32 unk28;
} EffThunderCell2C; /* 0x2C */

typedef struct {
    EffThunderHead4C head;
    EffThunderCell2C *cells; /* 0x4C */
    u32 color;          /* 0x50 */
    f32 baseFirst;      /* 0x54 */
    f32 baseSecond;     /* 0x58 */
    void *system;       /* 0x5C */
    u32 handle;         /* 0x60 */
} EffThunderWork4C; /* 0x64 */

INCLUDE_ASM(const s32, "game/code_0016A100", func_0016A100);

void effBattleApplyUnitRgbKeyframe(BtlUnit *unit, EffBattleUnitRgbCommand *command, s32 frame) {
    u32 startFrame = command->startFrame;
    u32 endFrame = command->endFrame;
    u32 startDuration = effBattleMiscGetTableEntry(command->startDurationIndex);
    u32 endDuration = effBattleMiscGetTableEntry(command->endDurationIndex);
    u32 restoreFrame;

    if ((unit->flags & 0xE0) != 0) {
        return;
    }
    if ((unit->flags & 2) == 0) {
        return;
    }
    if (startFrame >= endFrame) {
        return;
    }
    if (endFrame < endDuration) {
        return;
    }
    restoreFrame = endFrame - endDuration;
    if (startFrame >= restoreFrame) {
        return;
    }
    if (frame != startFrame && frame != restoreFrame) {
        return;
    }
    /* Preserve the runtime touch before the event-unit color is changed. */
    btlGetRuntime();
    if (frame == startFrame) {
        EvtUnit *eventUnit = (EvtUnit *)unit->ext;
        u32 baseColor = unit->baseColor;
        u32 blendedColor;

        if ((baseColor & 0xFFFFFF) != 0x808080) {
            u32 color = command->color;
            u32 differentBits = color ^ baseColor;
            u32 sharedBits = color & baseColor;
            blendedColor = sharedBits + ((differentBits & 0xFEFEFEFE) >> 1);
        } else {
            blendedColor = command->color;
        }
        evtSetUnitRgbTransition(eventUnit, startDuration, blendedColor);
    }
    if (frame == restoreFrame) {
        evtSetUnitRgbTransition((EvtUnit *)unit->ext, endDuration, unit->baseColor);
    }
}

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
    work->data = effParamWorkFactories[id].func(data);
    return work;
}

void effDispatchParameterDataAndFreeWork(EffParamWork *work) {
    D_003B0048[work->id].cb(work->data);
    sdfReleaseChipBlock(work);
}

/* Invoke the kind-specific callback on this parameter block. */
void effParamWorkInvokeCallback(EffParamWork *work) {
    u16 id = work->id;

    D_003B0044[id].cb(work->data);
}

EffParamWork *effParamWorkDuplicate(EffParamWork *src) {
    EffParamWork *work;

    work = func_00328D68(8);
    work->id = src->id;
    work->data = effParamWorkDuplicators[src->id].func(src->data);
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
    VU0_LOAD_VF(vf10, &D_003B0180);
    mdlStorePrimaryVectorVU(work);
    VU0_LOAD_VF(vf10, &D_003B0190);
    mdlUpdateContextRotationBasisFromQuaternion(work);
    VU0_LOAD_VF(vf10, &D_003B01A0);
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
    mdlProcessContextNodesAndTransforms(work, &D_00380828);
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
    VU0_LOAD_VF_MEMORY(vf10, vec);
    mdlStorePrimaryVectorVU(work);
}

/* Broadcast one scalar into three components before loading VU0 vf10. */
void effParamBuildVector(void *work, f32 scalar) {
    f32 v[3];

    v[0] = v[1] = v[2] = scalar;
    VU0_LOAD_VF_MEMORY(vf10, v);
    mdlStoreTertiaryVectorVU(work);
}

void effParamScatterVectors(EffScatterWork *work, void *src) {
    u8 *d0;
    u8 *d1;
    u8 *d2;

    VU0_LOAD_MATRIX(src);
    d0 = work->destination + 0x20;
    VU0_STORE_VF(vf28, d0);
    d1 = work->destination + 0x30;
    VU0_STORE_VF(vf29, d1);
    d2 = work->destination + 0x40;
    VU0_STORE_VF(vf30, d2);
}

void func_0016AB30(void) {
    mdlBroadcastMasked();
}

extern EffDispatchEntry effParameterWorkOperations[];
extern void **D_003AFFD0[];
extern u32 func_0016AEA0(u32 *word);
extern u32 func_0016AEA8(s32 address);

EffParamWorkEx *effCreateDispatchedParameterWork(u32 *source) {
    EffParamWorkEx *work;

    work = func_00328D68(0xC);
    work->id = func_0016AEA0(source);
    work->unk04 = func_0016AEA8((s32)source);
    if (effParameterWorkOperations[work->id].altFunc == NULL) {
        work->data = effParameterWorkOperations[work->id].func(D_003AFFD0[work->id][work->unk04]);
    } else {
        work->data = effParameterWorkOperations[work->id].func(source);
    }
    return work;
}

void effReleaseDispatchedParameterWork(EffParamWorkEx *work) {
    ((void (*)(void *))effParamWorkReleaseCallbacks[work->id].func)(work->data);
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
    if (effParameterWorkOperations[work->id].altFunc == NULL) {
        work->data = effParameterWorkOperations[work->id].func(D_003AFFD0[work->id][work->unk04]);
    } else {
        work->data = effParameterWorkOperations[work->id].altFunc(src->data);
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

/* Second thunder effect: the cell sub-system is dispatched with three head
 * pointers and a perCell group divisor of four. */
EffThunderWork4C *effCreateThunderCellSystemWork(EffThunderHead4C *src) {
    u32 handle = sdfAllocGeneralBlock(src->count * sizeof(EffThunderCell2C) + sizeof(EffThunderWork4C));
    EffThunderWork4C *work = (EffThunderWork4C *)sdfResourceRetainAddress(handle);
    u32 i;

    work->head = *src;
    work->cells = (EffThunderCell2C *)(work + 1);
    work->baseFirst = src->scaledFirst;
    work->baseSecond = src->scaledSecond;
    work->handle = handle;
    work->system = parAllocateCellSystem(work->head.count, work->head.perCell, 0, 4);
    func_00164AE8(work->system, work->head.unk38, work->head.dispatchArg, work->head.unk48);
    func_00164C68(work->system, work->head.systemParam);
    for (i = 0; i < work->head.count; i++) {
        work->cells[i].unk00 = 0;
        work->cells[i].unk04 = 0;
        work->cells[i].unk28 = 0;
    }
    work->color = 0x80808080;
    return work;
}

INCLUDE_SDATA(const s32, "game/code_0016A100", D_00436434);

