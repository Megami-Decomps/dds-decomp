#include "common.h"
#include "btl.h"
#include "evt_unit.h"
#include "pcp_vu0.h"

/* Effect parameter-set dispatch tables. Every effect kind owns one 0x28-byte
 * entry per table; the handler lives at +0x0. Slots are declared as separate
 * arrays (effParamWorkFactories/14/18/1C/20/24/28/2C/30/34 and effParameterWorkOperations/84/88/90/94/
 * 98/9C/A0/A4). The family2 create table (effParameterWorkOperations) additionally carries a
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

extern EffDispatchEntry effParamWorkFactories[];

extern EffDispatchEntry D_00353718[];

extern EffDispatchEntry effParamWorkDuplicators[];

extern EffDispatchEntry D_00353720[];

extern EffDispatchEntry D_00353724[];

extern EffDispatchEntry D_00353728[];

extern EffDispatchEntry D_0035372C[];

extern EffDispatchEntry D_00353730[];

extern EffDispatchEntry D_00353734[];

extern EffDispatchEntry effParameterWorkOperations[];

extern EffDispatchEntry D_00353884[];

extern EffDispatchEntry effParamWorkReleaseCallbacks[];

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

extern void sdfReleaseChipBlock(void *p);

extern void mdlBroadcastMasked();

extern void billSetChildScaleComponents(f32 arg0, f32 arg1);

extern void mdlProcessContextNodesAndTransforms(void *arg0, void *arg1);

extern void mdlStorePrimaryVectorVU(void *work);

extern void mdlUpdateContextRotationBasisFromQuaternion(void *work);

extern void mdlStoreTertiaryVectorVU(void *work);

extern void mdlAddEntryFlagged(void *work, s32 arg1, s32 arg2);

extern void mdlLoadViewerPackage(s32 arg0, u16 arg1, s32 arg2, void *arg3, u32 arg4);

extern void *func_00217680(void *arg0, void *arg1);

extern void *func_002183D0(void *arg);

extern void *func_002183E0(void *arg);

/* Kind-specific callback slot, 0x28 bytes per entry. */
typedef struct EffParamCallbackEntry {
    void (*cb)(void *arg); /* 0x00 */
    u8 pad4[0x24];         /* 0x04 */
} EffParamCallbackEntry;

extern EffParamCallbackEntry D_00353714[];

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

INCLUDE_ASM(const s32, "game/code_001624D0", func_001624D0);

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

void func_00162948(void) {
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

    work = func_002CFEB8(8);
    work->id = id;
    work->data = effParamWorkFactories[id].func(data);
    return work;
}

void effDispatchParameterDataAndFreeWork(EffParamWork *work) {
    ((void (*)(void *))D_00353718[work->id].func)(work->data);
    sdfReleaseChipBlock(work);
}

/* Invoke the kind-specific callback on this parameter block. */
void effParamWorkInvokeCallback(EffParamWork *work) {
    u16 id = work->id;

    D_00353714[id].cb(work->data);
}

EffParamWork *effParamWorkDuplicate(EffParamWork *src) {
    EffParamWork *work;

    work = func_002CFEB8(8);
    work->id = src->id;
    work->data = effParamWorkDuplicators[src->id].func(src->data);
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
    billSetChildScaleComponents(value, value);
}

void effParamInitWork(EffInitWork *work) {
    VU0_LOAD_VF(vf10, &D_00353850);
    mdlStorePrimaryVectorVU(work);
    VU0_LOAD_VF(vf10, &D_00353860);
    mdlUpdateContextRotationBasisFromQuaternion(work);
    VU0_LOAD_VF(vf10, &D_00353870);
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
    mdlProcessContextNodesAndTransforms(work, &D_00325828);
}

void func_00162DE0(void) {
    mdlDestroyContext();
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

/* The scatter work holds its destination vector block at +0x18. */
typedef struct EffScatterWork {
    u8 pad00[0x18];
    u8 *destination;
} EffScatterWork;

void effParamScatterVectors(EffScatterWork *work, void *src) {
    u8 *firstVector;
    u8 *secondVector;
    u8 *thirdVector;

    VU0_LOAD_MATRIX(src);
    firstVector = work->destination + 0x20;
    VU0_STORE_VF(vf28, firstVector);
    secondVector = work->destination + 0x30;
    VU0_STORE_VF(vf29, secondVector);
    thirdVector = work->destination + 0x40;
    VU0_STORE_VF(vf30, thirdVector);
}

void func_00162ED8(void) {
    mdlBroadcastMasked();
}

extern void **D_003536A0[];
extern u32 func_00163248(u32 *word);
extern u32 func_00163250(s32 address);

EffParamWorkEx *effCreateDispatchedParameterWork(u32 *source) {
    EffParamWorkEx *work;

    work = func_002CFEB8(0xC);
    work->id = func_00163248(source);
    work->unk04 = func_00163250((s32)source);
    if (effParameterWorkOperations[work->id].altFunc == NULL) {
        work->data = effParameterWorkOperations[work->id].func(D_003536A0[work->id][work->unk04]);
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
    ((void (*)(void *))D_00353884[work->id].func)(work->data);
}

EffParamWorkEx *effCloneDispatchedParameterWork(EffParamWorkEx *src) {
    EffParamWorkEx *work;

    work = func_002CFEB8(0xC);
    work->id = src->id;
    work->unk04 = src->unk04;
    if (effParameterWorkOperations[work->id].altFunc == NULL) {
        work->data = effParameterWorkOperations[work->id].func(D_003536A0[work->id][work->unk04]);
    } else {
        work->data = effParameterWorkOperations[work->id].altFunc(src->data);
    }
    return work;
}

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

typedef struct {
    u8 pad00[0x10];
    u16 systemParam;
    u8 pad12[2];
    u32 count;
    u8 pad18[4];
    f32 scaledFirst;
    f32 scaledSecond;
    f32 rangeF24;
    u32 spreadA;
    u32 spreadB;
    u16 perCell;
    u8 pad32[6];
    u32 field_0x38;
    u32 pad3C;
    u32 field_0x40;
    u32 pad44;
    u32 field_0x48;
} ParamThunderHead;

typedef struct {
    u32 unk00;
    u32 unk04;
    f32 dirA[3];
    f32 dirB[3];
    f32 f20;
    f32 f24;
    u32 unk28;
} ParamThunderCell;

typedef struct {
    ParamThunderHead head;
    ParamThunderCell *cells;
    u32 color;
    f32 baseFirst;
    f32 baseSecond;
    void *system;
    u32 handle;
} ParamThunderWork;

extern u32 sdfAllocGeneralBlock(s32 size);
extern u8 *sdfResourceRetainAddress(u32 handle);
extern void *parAllocateCellSystem(s32 count, s32 perCell, s32 groupDivisor, u32 kind);
extern void func_0015CEF8(void *system, u32 arg1, u32 arg2, u32 arg3);
extern void func_0015D078(void *system, u32 value);

ParamThunderWork *effCreateThunderCellSystemWork(ParamThunderHead *src) {
    u32 handle = sdfAllocGeneralBlock(src->count * sizeof(ParamThunderCell) + sizeof(ParamThunderWork));
    ParamThunderWork *work = (ParamThunderWork *)sdfResourceRetainAddress(handle);
    u32 i;

    work->head = *src;
    work->cells = (ParamThunderCell *)(work + 1);
    work->baseFirst = src->scaledFirst;
    work->baseSecond = src->scaledSecond;
    work->handle = handle;
    work->system = parAllocateCellSystem(work->head.count, work->head.perCell, 0, 4);
    func_0015CEF8(work->system, work->head.field_0x38, work->head.field_0x40, work->head.field_0x48);
    func_0015D078(work->system, work->head.systemParam);
    for (i = 0; i < work->head.count; i++) {
        work->cells[i].unk00 = 0;
        work->cells[i].unk04 = 0;
        work->cells[i].unk28 = 0;
    }
    work->color = 0x80808080;
    return work;
}

INCLUDE_SDATA(const s32, "game/code_001624D0", D_003BB044);

