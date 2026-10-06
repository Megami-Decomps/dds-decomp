#include "common.h"
#include "btl.h"
#include "evt_unit.h"
#include "pcp_vu0.h"
#include "mdl.h"

#define EFF_PARAM_WORK_BYTES 8
#define EFF_PARAM_EXTENDED_WORK_BYTES 0xC
#define EFF_PARAM_RECORD_BYTES 16
#define EFF_PARAM_TABLE_HEADER_BYTES 0x10
#define EFF_PARAM_BLOCK_OFFSET 0x14
#define EFF_PARAM_KIND_WORD_OFFSET 0x18
#define EFF_VIEWER_RESOURCE_GROUP 7
#define EFF_VIEWER_LOAD_FLAGS 0x101
#define EFF_CELL_SYSTEM_KIND 4

/* Native 0x28-byte operation row, indexed by effect kind. The optional
 * callbacks retain their existing one-payload call interface here.
 * In the extended table, a duplicate callback selects raw-source creation
 * and cloning; its absence selects the kind/tableIndex fallback table. */
typedef struct EffDispatchEntry {
    void *(*create)(void *);          /* 0x00 */
    void (*dispatch)(void *);         /* 0x04 */
    void (*release)(void *);          /* 0x08 */
    void *(*duplicate)(void *);       /* 0x0C */
    void *(*callbacks[6])(void *);    /* 0x10 */
} EffDispatchEntry; /* 0x28 */

/* Compact work: a halfword effect kind and an opaque callback payload. */
typedef struct EffParamWork {
    u16 kind;     /* 0x00 effect kind */
    u8 pad02[2];  /* 0x02 */
    void *payload; /* 0x04 callback payload */
} EffParamWork; /* 0x08 */

/* Extended work: a full-word kind, fallback-table index, and payload. */
typedef struct EffParamWorkEx {
    u32 kind;       /* 0x00 effect kind */
    u32 tableIndex; /* 0x04 fallback index, retained for cloning */
    void *payload;  /* 0x08 callback payload */
} EffParamWorkEx; /* 0x0C */


extern EffDispatchEntry effParamWorkFactories[];

extern EffDispatchEntry effParameterWorkOperations[];


extern u8 D_00353850[];

extern u8 D_00353860[];

extern u8 D_00353870[];

extern u8 D_00325828[];

extern u16 D_003BB044;

extern void *sdfAllocSizeClassBlock(s32 size);

extern void sdfReleaseChipBlock(void *p);

extern void mdlBroadcastMasked(MdlCtx *, u32);

extern void billSetChildScaleComponents(f32 arg0, f32 arg1);

extern void mdlProcessContextNodesAndTransforms(MdlCtx *, s32);

extern void mdlStorePrimaryVectorVU(MdlCtx *);

extern void mdlUpdateContextRotationBasisFromQuaternion(MdlCtx *);

extern void mdlStoreTertiaryVectorVU(MdlCtx *);

extern void mdlAddEntryFlagged(MdlCtx *, s32, s32);
extern void mdlDestroyContext(MdlCtx *);

extern void mdlLoadViewerPackage(s32 arg0, u16 arg1, s32 arg2, void *arg3, u32 arg4);

extern MdlCtx *func_00217680(s32, s32);

extern u16 mdlGetContextResourceGroup(MdlCtx *);

extern u16 mdlGetContextResourceId(MdlCtx *);


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

/* Return the compact work's payload address without changing its ownership. */
u32 effParamWorkGetData(EffParamWork *work) {
    return (u32)work->payload;
}

/* Return the compact work's halfword effect kind. */
u16 effParamWorkGetId(EffParamWork *work) {
    return work->kind;
}

/* Allocate compact work and create its payload through the required kind factory. */
EffParamWork *effParamWorkCreate(u16 kind, void *source) {
    EffParamWork *work;

    work = sdfAllocSizeClassBlock(EFF_PARAM_WORK_BYTES);
    work->kind = kind;
    work->payload = effParamWorkFactories[kind].create(source);
    return work;
}

/* Dispatch the payload before freeing its compact owner; this callback is required. */
void effDispatchParameterDataAndFreeWork(EffParamWork *work) {
    effParamWorkFactories[work->kind].release(work->payload);
    sdfReleaseChipBlock(work);
}

/* Invoke the kind-specific callback on this parameter block. */
void effParamWorkInvokeCallback(EffParamWork *work) {
    u16 kind = work->kind;

    effParamWorkFactories[kind].dispatch(work->payload);
}

/* Allocate a second compact owner and duplicate the source payload by kind. */
EffParamWork *effParamWorkDuplicate(EffParamWork *source) {
    EffParamWork *work;

    work = sdfAllocSizeClassBlock(EFF_PARAM_WORK_BYTES);
    work->kind = source->kind;
    work->payload = effParamWorkFactories[source->kind].duplicate(source->payload);
    return work;
}

/* Optional compact-work dispatches. Other callers supply additional native
 * arguments; the existing unit-local callback prototypes remain unchanged. */
void effParamWorkCallback0(EffParamWork *work) {
    if (effParamWorkFactories[work->kind].callbacks[0] != NULL) {
        effParamWorkFactories[work->kind].callbacks[0](work->payload);
    }
}

void effParamWorkCallback1(EffParamWork *work) {
    if (effParamWorkFactories[work->kind].callbacks[1] != NULL) {
        effParamWorkFactories[work->kind].callbacks[1](work->payload);
    }
}

void effParamWorkCallback2(EffParamWork *work) {
    if (effParamWorkFactories[work->kind].callbacks[2] != NULL) {
        effParamWorkFactories[work->kind].callbacks[2](work->payload);
    }
}

void effParamWorkCallback3(EffParamWork *work) {
    if (effParamWorkFactories[work->kind].callbacks[3] != NULL) {
        effParamWorkFactories[work->kind].callbacks[3](work->payload);
    }
}

void effParamWorkCallback4(EffParamWork *work) {
    if (effParamWorkFactories[work->kind].callbacks[4] != NULL) {
        effParamWorkFactories[work->kind].callbacks[4](work->payload);
    }
}

void effParamWorkCallback5(EffParamWork *work) {
    if (effParamWorkFactories[work->kind].callbacks[5] != NULL) {
        effParamWorkFactories[work->kind].callbacks[5](work->payload);
    }
}

/* Select billboard kind zero; the index is passed through without validation. */
void func_00162C60(u32 index) {
    billCreateIndexed(0, index);
}

/* Select billboard kind one; the index is passed through without validation. */
void func_00162C80(u32 index) {
    billCreateIndexed(1, index);
}

/* Use the same floating value for both billboard child-scale components. */
void effParamDispatchFloat(f32 value) {
    billSetChildScaleComponents(value, value);
}

/* Load the default primary/rotation/tertiary vectors and packed broadcast value.
 * Reset the first motion node's float to 1.0f, then clear flag bit zero. */
void effParamInitWork(MdlCtx *work) {
    VU0_LOAD_VF(vf10, &D_00353850);
    mdlStorePrimaryVectorVU(work);
    VU0_LOAD_VF(vf10, &D_00353860);
    mdlUpdateContextRotationBasisFromQuaternion(work);
    VU0_LOAD_VF(vf10, &D_00353870);
    mdlStoreTertiaryVectorVU(work);
    mdlBroadcastMasked(work, 0x80808080);
    if (work->first != NULL) {
        mdlAddEntryFlagged(work, 0, 0);
        work->first->frameStep = 1.0f;
    }
    work->flags &= ~1u;
}

/* Create and initialize a viewer-package context in the fixed effect group.
 * DDS1 consumes the current halfword id directly; it does not scan occupied ids. */
void *effParamCreateInitWork(void *package) {
    MdlCtx *work;

    mdlLoadViewerPackage(EFF_VIEWER_RESOURCE_GROUP, D_003BB044, EFF_VIEWER_LOAD_FLAGS, (u8 *)package + 0x10, *(u32 *)package);
    work = func_00217680(EFF_VIEWER_RESOURCE_GROUP, D_003BB044);
    effParamInitWork(work);
    D_003BB044++;
    return work;
}

/* Run the context/node update with this game's fixed global argument. */
void effParamInitFromGlobal(void *work) {
    mdlProcessContextNodesAndTransforms(work, (s32)D_00325828);
}

void func_00162DE0(void *work) {
    mdlDestroyContext(work);
}

/* Resolve the source's resource group/id, obtain its context, and initialize it. */
void *effParamAssembleWork(void *source) {
    s32 resourceGroup;
    s32 resourceId;
    MdlCtx *work;

    resourceGroup = mdlGetContextResourceGroup(source);
    resourceId = mdlGetContextResourceId(source);
    work = func_00217680(resourceGroup, resourceId);
    effParamInitWork(work);
    return work;
}

/* Load a full source quadword into vf10 and store it as the primary vector. */
void effParamForwardVector(void *work, void *vector) {
    VU0_LOAD_VF_MEMORY(vf10, vector);
    mdlStorePrimaryVectorVU(work);
}

/* Initialize only three components before the quadword VU load.
 * The fourth component is not initialized here; retain the native array size. */
void effParamBuildVector(void *work, f32 scalar) {
    f32 components[3];

    components[0] = components[1] = components[2] = scalar;
    VU0_LOAD_VF_MEMORY(vf10, components);
    mdlStoreTertiaryVectorVU(work);
}

/* Load four source quadwords, then scatter only vf28-vf30 to the destination block. */
void effParamScatterVectors(MdlCtx *work, void *matrix) {
    void *firstVector;
    void *secondVector;
    void *thirdVector;

    VU0_LOAD_MATRIX(matrix);
    firstVector = work->inner->matrix[0];
    VU0_STORE_VF(vf28, firstVector);
    secondVector = work->inner->matrix[1];
    VU0_STORE_VF(vf29, secondVector);
    thirdVector = work->inner->matrix[2];
    VU0_STORE_VF(vf30, thirdVector);
}

void func_00162ED8(void *work, u32 color) {
    mdlBroadcastMasked(work, color);
}

extern void **D_003536A0[];
extern u32 func_00163248(u32 *word);
extern u32 func_00163250(s32 address);

/* Create extended work from a kind/index descriptor. Kinds with a duplicate
 * callback consume the raw descriptor; other kinds use the fallback table. */
EffParamWorkEx *effCreateDispatchedParameterWork(u32 *source) {
    EffParamWorkEx *work;

    work = sdfAllocSizeClassBlock(EFF_PARAM_EXTENDED_WORK_BYTES);
    work->kind = func_00163248(source);
    work->tableIndex = func_00163250((s32)source);
    if (effParameterWorkOperations[work->kind].duplicate == NULL) {
        work->payload = effParameterWorkOperations[work->kind].create(D_003536A0[work->kind][work->tableIndex]);
    } else {
        work->payload = effParameterWorkOperations[work->kind].create(source);
    }
    return work;
}

/* Release the extended payload through its required kind callback, then its owner. */
void effReleaseDispatchedParameterWork(EffParamWorkEx *work) {
    effParameterWorkOperations[work->kind].release(work->payload);
    sdfReleaseChipBlock(work);
}

/* Invoke the extended work's required dispatch callback. */
void effInvokeParameterWorkDispatch(EffParamWorkEx *work) {
    effParameterWorkOperations[work->kind].dispatch(work->payload);
}

/* Recreate table-backed payloads; duplicate payloads only for raw-source kinds. */
EffParamWorkEx *effCloneDispatchedParameterWork(EffParamWorkEx *source) {
    EffParamWorkEx *work;

    work = sdfAllocSizeClassBlock(EFF_PARAM_EXTENDED_WORK_BYTES);
    work->kind = source->kind;
    work->tableIndex = source->tableIndex;
    if (effParameterWorkOperations[work->kind].duplicate == NULL) {
        work->payload = effParameterWorkOperations[work->kind].create(D_003536A0[work->kind][work->tableIndex]);
    } else {
        work->payload = effParameterWorkOperations[work->kind].duplicate(source->payload);
    }
    return work;
}

/* Optional extended-work dispatches; preserve the native column order,
 * including the reversed fourth/fifth column addresses. */
void effParamWorkExCallback0(EffParamWorkEx *work) {
    if (effParameterWorkOperations[work->kind].callbacks[0] != NULL) {
        effParameterWorkOperations[work->kind].callbacks[0](work->payload);
    }
}

void effParamWorkExCallback1(EffParamWorkEx *work) {
    if (effParameterWorkOperations[work->kind].callbacks[1] != NULL) {
        effParameterWorkOperations[work->kind].callbacks[1](work->payload);
    }
}

void effParamWorkExCallback2(EffParamWorkEx *work) {
    if (effParameterWorkOperations[work->kind].callbacks[2] != NULL) {
        effParameterWorkOperations[work->kind].callbacks[2](work->payload);
    }
}

void effParamWorkExCallback3(EffParamWorkEx *work) {
    if (effParameterWorkOperations[work->kind].callbacks[4] != NULL) {
        effParameterWorkOperations[work->kind].callbacks[4](work->payload);
    }
}

void effParamWorkExCallback4(EffParamWorkEx *work) {
    if (effParameterWorkOperations[work->kind].callbacks[3] != NULL) {
        effParameterWorkOperations[work->kind].callbacks[3](work->payload);
    }
}

void effParamWorkExCallback5(EffParamWorkEx *work) {
    if (effParameterWorkOperations[work->kind].callbacks[5] != NULL) {
        effParameterWorkOperations[work->kind].callbacks[5](work->payload);
    }
}

/* Read the descriptor's full-word effect kind. */
u32 func_00163248(u32 *word) {
    return *word;
}

/* Read the descriptor's full-word fallback-table index at +4. */
u32 func_00163250(s32 address) {
    return *(u32 *)(address + 4);
}

/* Records follow a 16-byte header and have a 16-byte stride. Block offsets
 * are signed and relative to the whole table, not to individual records.
 * Neither the index nor the resulting target address is validated here. */
void *effParamTableGetBlock(void *table, s32 index) {
    u8 *base = (u8 *)table;
    u8 *indexedBase = base + index * EFF_PARAM_RECORD_BYTES;

    return base + *(s32 *)(indexedBase + EFF_PARAM_BLOCK_OFFSET);
}

/* Return the record's first word without interpreting its meaning. */
u32 effParamTableGetWord(void *table, s32 index) {
    u8 *bytes = (u8 *)table;

    bytes += index * EFF_PARAM_RECORD_BYTES;
    return *(u32 *)(bytes + EFF_PARAM_TABLE_HEADER_BYTES);
}

/* Return the full record word whose low halfword selects a compact-work factory. */
u32 effParamTableGetWord2(void *table, s32 index) {
    u8 *bytes = (u8 *)table;

    bytes += index * EFF_PARAM_RECORD_BYTES;
    return *(u32 *)(bytes + EFF_PARAM_KIND_WORD_OFFSET);
}

/* Create compact work from a table record; retain the legacy parameter type
 * even though this argument is used as a raw table rather than a work owner. */
EffParamWork *effParamCreateFromTable(EffParamWork *table, s32 index) {
    u16 kind;
    void *source;

    kind = (u16)effParamTableGetWord2(table, index);
    source = effParamTableGetBlock(table, index);
    return effParamWorkCreate(kind, source);
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
    u32 firstDispatchArg;
    u32 pad3C;
    u32 secondDispatchArg;
    u32 pad44;
    u32 thirdDispatchArg;
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
extern void parRiseFallSymmetricCellAlpha(void *system, u32 arg1, u32 arg2, u32 arg3);
extern void func_0015D078(void *system, u32 value);

/* Allocate the copied head and its trailing cells as one block, then create
 * the cell system with native arguments groupDivisor=0 and kind=4.
 * Only three words per cell are zeroed here; vector/range storage is untouched. */
ParamThunderWork *effCreateThunderCellSystemWork(ParamThunderHead *source) {
    u32 allocationHandle = sdfAllocGeneralBlock(source->count * sizeof(ParamThunderCell) + sizeof(ParamThunderWork));
    ParamThunderWork *work = (ParamThunderWork *)sdfResourceRetainAddress(allocationHandle);
    u32 cellIndex;

    work->head = *source;
    work->cells = (ParamThunderCell *)(work + 1);
    work->baseFirst = source->scaledFirst;
    work->baseSecond = source->scaledSecond;
    work->handle = allocationHandle;
    work->system = parAllocateCellSystem(work->head.count, work->head.perCell, 0, EFF_CELL_SYSTEM_KIND);
    parRiseFallSymmetricCellAlpha(work->system, work->head.firstDispatchArg, work->head.secondDispatchArg, work->head.thirdDispatchArg);
    func_0015D078(work->system, work->head.systemParam);
    for (cellIndex = 0; cellIndex < work->head.count; cellIndex++) {
        work->cells[cellIndex].unk00 = 0;
        work->cells[cellIndex].unk04 = 0;
        work->cells[cellIndex].unk28 = 0;
    }
    work->color = 0x80808080;
    return work;
}

INCLUDE_SDATA(const s32, "game/code_001624D0", D_003BB044);

