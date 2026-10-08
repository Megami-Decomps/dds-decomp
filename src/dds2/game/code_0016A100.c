#include "common.h"
#include "eff_param.h"
#include "par_cell_api.h"
#include "sdf_resource.h"
#include "btl_state.h"
#include "eff.h"
#include "ee_mmi.h"
#include "evt_unit.h"
#include "pcp_vu0.h"
#include "mdl.h"
#include "bill_object_api.h"

#define EFF_PARAM_WORK_BYTES 8
#define EFF_PARAM_EXTENDED_WORK_BYTES 0xC
#define EFF_PARAM_RECORD_BYTES 16
#define EFF_PARAM_TABLE_HEADER_BYTES 0x10
#define EFF_PARAM_BLOCK_OFFSET 0x14
#define EFF_PARAM_KIND_WORD_OFFSET 0x18
#define EFF_VIEWER_RESOURCE_GROUP 7
#define EFF_VIEWER_LOAD_FLAGS 0x101

/* Native 0x28-byte operation row, indexed by effect kind.
 * The scale operation takes a float in addition to its payload.
 * In the extended table, a duplicate callback selects raw-source creation
 * and cloning; its absence selects the kind/tableIndex fallback table. */
typedef struct EffDispatchEntry {
    void *(*create)(void *);          /* 0x00 */
    void (*dispatch)(void *);         /* 0x04 */
    void (*release)(void *);          /* 0x08 */
    void *(*duplicate)(void *);       /* 0x0C */
    void (*callback0)(void *, void *);       /* 0x10 */
    void (*setScale)(void *, f32);    /* 0x14 */
    void (*callback2)(void *, void *);       /* 0x18 */
    void (*callback3)(void *, u32);       /* 0x1C */
    void *(*callback4)(void *);       /* 0x20 */
    void *(*callback5)(void *);       /* 0x24 */
} EffDispatchEntry; /* 0x28 */


extern EffDispatchEntry effParamWorkFactories[];

extern void *sdfAllocSizeClassBlock(s32 size);


extern u8 D_003B0180[];

extern u8 D_003B0190[];

extern u8 D_003B01A0[];

extern void mdlBroadcastMasked(MdlCtx *, u32);

extern void mdlStorePrimaryVectorVU(MdlCtx *);

extern void mdlUpdateContextRotationBasisFromQuaternion(MdlCtx *);

extern void mdlStoreTertiaryVectorVU(MdlCtx *);

extern void mdlAddEntryFlagged(MdlCtx *, s32, s32);
extern void mdlDestroyContext(MdlCtx *);

extern MdlCtx *func_00232198(s32, s32);

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


extern u8 D_00380828[];

extern void mdlProcessContextNodesAndTransforms(MdlCtx *, s32);

extern void sdfReleaseChipBlock(void *p);

extern void parDispatchSub(void *work, s32 sub, void *a2, void *a3);

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
    void *firstDispatchArg;  /* 0x38 first dispatch argument */
    u32 pad3C;
    void *secondDispatchArg; /* 0x40 second dispatch argument */
    u32 pad44;
    void *thirdDispatchArg;  /* 0x48 third dispatch argument */
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
    ParSystem *system;  /* 0x5C: allocated cell system */
    u32 handle;         /* 0x60 */
} EffThunderWork4C; /* 0x64 */

typedef struct EffBattleUnitColorCommand {
    EffectVectorRequest request;
    u32 firstColor;
    u32 secondColor;
    u32 startFrame;
    s32 startDurationIndex;
    u32 endFrame;
    s32 endDurationIndex;
} EffBattleUnitColorCommand;

extern u32 effBTLFieldColorGetOriginalSelector(void);
extern u32 effBTLFieldColorGetVariantSelector(void);
extern void evtSetUnitStatusFlags(EvtUnit *);
extern void evtInitializeUnitColorTransition(EvtUnit *, s32, u32, u32);
extern void evtSetUnitNormalizedDirection(EvtUnit *, s32);
extern void effBattleMiscDirectionTo(BtlUnit *, EffectVectorRequest *, f32 *);
extern u32 btlCameraVectorHasNaN(void);
extern u32 btlBlendColorVec(f32 *, f32 *, f32);

/* Apply and restore a selected group's two-color and direction keyframe. */
void func_0016A100(BtlUnit *unit, EffBattleUnitColorCommand *command, u32 frame) {
    BtlUnit *selected[16];
    f32 direction[4];
    u32 packedStart[4];
    u32 packedEnd[4];
    u32 startFrame = command->startFrame;
    u32 endFrame = command->endFrame;
    u32 startDuration = effBattleMiscGetTableEntry(command->startDurationIndex);
    u32 endDuration = effBattleMiscGetTableEntry(command->endDurationIndex);
    u32 restoreFrame;
    u32 mask = 0;
    u32 count;
    u32 index;
    BtlState *battle;
    BtlUnit *current;
    EvtUnit *eventUnit;

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
    battle = (BtlState *)btlGetRuntime();
    if (battle->battleFlags & 0x40000000) {
        return;
    }
    count = 0;
    switch (command->request.kind) {
    case 0:
        selected[0] = unit;
        count = 1;
        break;
    case 1:
        mask = ((BtlUnit *)effBTLFieldColorGetOriginalSelector())->flags & 0xE00;
        break;
    case 2:
        mask = ((BtlUnit *)effBTLFieldColorGetVariantSelector())->flags & 0xE00;
        break;
    case 3:
        mask = 0xE00;
        break;
    }
    if (count == 0) {
        for (current = battle->units; current != NULL; current = current->nextActor) {
            u32 flags = current->flags;
            if ((flags & 2) && (current->stateFlags & 0x10) &&
                current->ext != NULL && (flags & mask)) {
                selected[count++] = current;
            }
        }
    }
    if (frame == startFrame) {
        for (index = 0; index < count; index++) {
            eventUnit = selected[index]->ext;
            evtSetUnitStatusFlags(eventUnit);
            evtInitializeUnitColorTransition(eventUnit, startDuration, command->firstColor, command->secondColor);
            effBattleMiscDirectionTo(selected[index], &command->request, direction);
            VU0_LOAD_VF(vf10, direction);
            evtSetUnitNormalizedDirection(eventUnit, startDuration);
        }
    }
    if (frame == restoreFrame) {
        for (index = 0; index < count; index++) {
            u32 firstColor;
            u32 secondColor;

            eventUnit = selected[index]->ext;

            evtSetUnitStatusFlags(eventUnit);
            if (btlCameraVectorHasNaN()) {
                firstColor = btlBlendColorVec(battle->lightColor, selected[index]->colorStart, 0.3f);
                secondColor = btlBlendColorVec(battle->ambientColor, selected[index]->colorEnd, 0.3f);
            } else {
                VU0_LOAD_VF(vf10, selected[index]->colorStart);
                EE_MMI_RGBA_PACK_UNIT(packedStart[0], 128.0f);
                firstColor = packedStart[0];
                VU0_LOAD_VF(vf10, selected[index]->colorEnd);
                EE_MMI_RGBA_PACK_UNIT(packedEnd[0], 128.0f);
                secondColor = packedEnd[0];
            }
            evtInitializeUnitColorTransition(eventUnit, endDuration, firstColor, secondColor);
            VU0_LOAD_VF(vf10, selected[index]->lightDirection);
            evtSetUnitNormalizedDirection(eventUnit, endDuration);
        }
    }
}



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

/* Return the compact work's payload address without changing its ownership. */
void *effParamWorkGetData(EffParamWork *work) {
    return work->payload;
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

/* Optional compact-work operations, indexed by effect kind. */
void effParamWorkCallback0(EffParamWork *work, void *source) {
    if (effParamWorkFactories[work->kind].callback0 != NULL) {
        effParamWorkFactories[work->kind].callback0(work->payload, source);
    }
}

void effParamWorkCallback1(EffParamWork *work, f32 scale) {
    if (effParamWorkFactories[work->kind].setScale != NULL) {
        effParamWorkFactories[work->kind].setScale(work->payload, scale);
    }
}

void effParamWorkCallback2(EffParamWork *work, void *matrix) {
    if (effParamWorkFactories[work->kind].callback2 != NULL) {
        effParamWorkFactories[work->kind].callback2(work->payload, matrix);
    }
}

void effParamWorkCallback3(EffParamWork *work, u32 value) {
    if (effParamWorkFactories[work->kind].callback3 != NULL) {
        effParamWorkFactories[work->kind].callback3(work->payload, value);
    }
}

void effParamWorkCallback4(EffParamWork *work) {
    if (effParamWorkFactories[work->kind].callback4 != NULL) {
        effParamWorkFactories[work->kind].callback4(work->payload);
    }
}

void effParamWorkCallback5(EffParamWork *work) {
    if (effParamWorkFactories[work->kind].callback5 != NULL) {
        effParamWorkFactories[work->kind].callback5(work->payload);
    }
}

/* Create a child billboard from its resource data. */
BillObj *effParamCreateChildBillboard(void *resourceData) {
    return billCreateIndexed(0, (u32)resourceData);
}

/* Create an animated billboard from its serialized resource data. */
BillObj *effParamCreateAnimatedBillboard(void *resourceData) {
    return billCreateIndexed(1, (u32)resourceData);
}

/* Use the same floating value for both billboard child-scale components. */
void effParamDispatchFloat(void *payload, f32 value) {
    billSetChildScaleComponents((BillObj *)payload, value, value);
}

/* Load the default primary/rotation/tertiary vectors and packed broadcast value.
 * Reset the first motion node's float to 1.0f, then clear flag bit zero. */
void effParamInitWork(MdlCtx *work) {
    VU0_LOAD_VF(vf10, &D_003B0180);
    mdlStorePrimaryVectorVU(work);
    VU0_LOAD_VF(vf10, &D_003B0190);
    mdlUpdateContextRotationBasisFromQuaternion(work);
    VU0_LOAD_VF(vf10, &D_003B01A0);
    mdlStoreTertiaryVectorVU(work);
    mdlBroadcastMasked(work, 0x80808080);
    if (work->first != NULL) {
        mdlAddEntryFlagged(work, 0, 0);
        work->first->frameStep = 1.0f;
    }
    work->flags &= ~MDL_SKIP_TRANSFORMS;
}

extern u16 D_00436434;
extern BattleGroupNode *btlFindGroupedEntity(s32, s32);
extern void mdlLoadViewerPackage(s32, u16, s32, void *, u32);

/* Create and initialize a viewer-package context in the fixed effect group.
 * DDS2 skips occupied halfword ids before loading; DDS1 does not scan them. */
void *effParamCreateViewerWork(s32 *package) {
    MdlCtx *work;

    while (btlFindGroupedEntity(EFF_VIEWER_RESOURCE_GROUP, D_00436434) != 0) {
        D_00436434++;
    }
    mdlLoadViewerPackage(EFF_VIEWER_RESOURCE_GROUP, D_00436434, EFF_VIEWER_LOAD_FLAGS, package + 4, package[0]);
    work = func_00232198(EFF_VIEWER_RESOURCE_GROUP, D_00436434);
    effParamInitWork(work);
    D_00436434++;
    return work;
}

/* Run the context/node update with this game's fixed global argument. */
void effParamInitFromGlobal(void *work) {
    mdlProcessContextNodesAndTransforms(work, (s32)D_00380828);
}

void func_0016AA38(void *work) {
    mdlDestroyContext(work);
}

/* Resolve the source's resource group/id, obtain its context, and initialize it. */
void *effParamAssembleWork(void *source) {
    s32 resourceGroup;
    s32 resourceId;
    MdlCtx *work;

    resourceGroup = mdlGetContextResourceGroup(source);
    resourceId = mdlGetContextResourceId(source);
    work = func_00232198(resourceGroup, resourceId);
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

void func_0016AB30(void *work, u32 color) {
    mdlBroadcastMasked(work, color);
}

extern EffDispatchEntry effParameterWorkOperations[];
extern void **D_003AFFD0[];
extern u32 func_0016AEA0(u32 *word);
extern u32 func_0016AEA8(s32 address);

/* Create extended work from a kind/index descriptor. Kinds with a duplicate
 * callback consume the raw descriptor; other kinds use the fallback table. */
EffParamWorkEx *effCreateDispatchedParameterWork(u32 *source) {
    EffParamWorkEx *work;

    work = sdfAllocSizeClassBlock(EFF_PARAM_EXTENDED_WORK_BYTES);
    work->kind = func_0016AEA0(source);
    work->tableIndex = func_0016AEA8((s32)source);
    if (effParameterWorkOperations[work->kind].duplicate == NULL) {
        work->payload = effParameterWorkOperations[work->kind].create(D_003AFFD0[work->kind][work->tableIndex]);
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
        work->payload = effParameterWorkOperations[work->kind].create(D_003AFFD0[work->kind][work->tableIndex]);
    } else {
        work->payload = effParameterWorkOperations[work->kind].duplicate(source->payload);
    }
    return work;
}

/* Optional extended-work dispatches; preserve the native column order,
 * including the reversed fourth/fifth column addresses. */
void effParamWorkExCallback0(EffParamWorkEx *work, void *source) {
    if (effParameterWorkOperations[work->kind].callback0 != NULL) {
        effParameterWorkOperations[work->kind].callback0(work->payload, source);
    }
}

void effParamWorkExCallback1(EffParamWorkEx *work, f32 scale) {
    if (effParameterWorkOperations[work->kind].setScale != NULL) {
        effParameterWorkOperations[work->kind].setScale(work->payload, scale);
    }
}

void effParamWorkExCallback2(EffParamWorkEx *work, void *matrix) {
    if (effParameterWorkOperations[work->kind].callback2 != NULL) {
        effParameterWorkOperations[work->kind].callback2(work->payload, matrix);
    }
}

void effParamWorkExCallback3(EffParamWorkEx *work) {
    if (effParameterWorkOperations[work->kind].callback4 != NULL) {
        effParameterWorkOperations[work->kind].callback4(work->payload);
    }
}

void effParamWorkExCallback4(EffParamWorkEx *work, u32 value) {
    if (effParameterWorkOperations[work->kind].callback3 != NULL) {
        effParameterWorkOperations[work->kind].callback3(work->payload, value);
    }
}

void effParamWorkExCallback5(EffParamWorkEx *work) {
    if (effParameterWorkOperations[work->kind].callback5 != NULL) {
        effParameterWorkOperations[work->kind].callback5(work->payload);
    }
}

/* Read the descriptor's full-word effect kind. */
u32 func_0016AEA0(u32 *word) {
    return *word;
}

/* Read the descriptor's full-word fallback-table index at +4. */
u32 func_0016AEA8(s32 address) {
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

/* Create compact work from a serialized table record. */
EffParamWork *effParamCreateFromTable(void *table, s32 index) {
    u16 kind;
    void *source;

    kind = (u16)effParamTableGetWord2(table, index);
    source = effParamTableGetBlock(table, index);
    return effParamWorkCreate(kind, source);
}

/* Allocate the copied head and its trailing cells as one block, then create
 * the cell system with native arguments groupDivisor=0 and kind=4.
 * Only three words per cell are zeroed here; vector/range storage is untouched. */
EffThunderWork4C *effCreateThunderCellSystemWork(EffThunderHead4C *source) {
    u32 allocationHandle = (u32)sdfAllocGeneralBlock(source->count * sizeof(EffThunderCell2C) + sizeof(EffThunderWork4C));
    EffThunderWork4C *work = (EffThunderWork4C *)sdfResourceRetainAddress((struct SdfMemBlock *)(allocationHandle));
    u32 cellIndex;

    work->head = *source;
    work->cells = (EffThunderCell2C *)(work + 1);
    work->baseFirst = source->scaledFirst;
    work->baseSecond = source->scaledSecond;
    work->handle = allocationHandle;
    work->system = parAllocateCellSystem(work->head.count, work->head.perCell, 0, PAR_CELL_TOPOLOGY_FIVE_VECTOR);
    parRiseFallSymmetricCellAlpha(work->system, work->head.firstDispatchArg, work->head.secondDispatchArg, work->head.thirdDispatchArg);
    parSetCellDrawBucket(work->system, work->head.systemParam);
    for (cellIndex = 0; cellIndex < work->head.count; cellIndex++) {
        work->cells[cellIndex].unk00 = 0;
        work->cells[cellIndex].unk04 = 0;
        work->cells[cellIndex].unk28 = 0;
    }
    work->color = 0x80808080;
    return work;
}

INCLUDE_SDATA(const s32, "game/code_0016A100", D_00436434);

