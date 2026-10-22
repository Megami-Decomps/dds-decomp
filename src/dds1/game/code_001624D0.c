#include "common.h"
#include "sdf_chip.h"
#include "eff_param.h"
#include "eff_param_operations.h"
#include "eff_thunder_vector.h"
#include "par_cell_api.h"
#include "sdf_resource.h"
#include "btl_state.h"
#include "eff.h"
#include "eff_battle_color_keyframe.h"
#include "ee_mmi.h"
#include "evt_unit.h"
#include "evt_unit_color_transition.h"
#include "pcp_vu0.h"
#include "mdl.h"
#include "bill_object_api.h"

#define EFF_VIEWER_RESOURCE_GROUP 7
#define EFF_VIEWER_LOAD_FLAGS 0x101

extern u8 effModelDefaultPosition[];

extern u8 effModelDefaultQuaternion[];

extern u8 effModelDefaultScale[];

extern struct SdfPoolNode *kwlnSceneSurfaceSets[13][4];

extern u16 D_003BB044;










extern void mdlPackDecode(s32 resourceGroup, u16 resourceId, s32 flags, void *packageData, u32 packageSize);

extern u32 effBattleMiscGetTableEntry(s32 index);
extern u32 effBTLFieldColorGetOriginalSelector(void);
extern u32 effBTLFieldColorGetVariantSelector(void);

extern void effBattleMiscDirectionTo(BtlUnit *, EffectVectorRequest *, f32 *);
extern u32 btlCameraVectorHasNaN(void);
extern u32 btlBlendColorVec(f32 *, f32 *, f32);

/* Apply and restore a selected group's two-color and direction keyframe. */
void effBattleApplyUnitColorDirection(BtlUnit *unit, EffBattleUnitColorCommand *command, u32 frame) {
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
    evtUnitHandle_t *eventUnit;

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
        mask = ((BtlUnit *)effBTLFieldColorGetOriginalSelector())->status.flags & 0xE00;
        break;
    case 2:
        mask = ((BtlUnit *)effBTLFieldColorGetVariantSelector())->status.flags & 0xE00;
        break;
    case 3:
        mask = 0xE00;
        break;
    }
    if (count == 0) {
        for (current = battle->units; current != NULL; current = current->next) {
            u32 flags = current->status.flags;
            if ((flags & 2) && (current->status.stateFlags & 0x10) &&
                current->ext != NULL && (flags & mask)) {
                selected[count++] = current;
            }
        }
    }
    if (frame == startFrame) {
        for (index = 0; index < count; index++) {
            eventUnit = selected[index]->ext;
            evtUnitLightOn(eventUnit);
            evtSetUnitLightAndAmbient(eventUnit, startDuration, command->firstColor, command->secondColor);
            effBattleMiscDirectionTo(selected[index], &command->request, direction);
            VU0_LOAD_VF(vf10, direction);
            evtSetUnitLightDir(eventUnit, startDuration);
        }
    }
    if (frame == restoreFrame) {
        for (index = 0; index < count; index++) {
            u32 firstColor;
            u32 secondColor;

            eventUnit = selected[index]->ext;

            evtUnitLightOn(eventUnit);
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
            evtSetUnitLightAndAmbient(eventUnit, endDuration, firstColor, secondColor);
            VU0_LOAD_VF(vf10, selected[index]->lightDirection);
            evtSetUnitLightDir(eventUnit, endDuration);
        }
    }
}


void effBattleApplyUnitRgbKeyframe(BtlUnit *unit, EffBattleUnitRgbCommand *command, s32 frame) {
    u32 startFrame = command->startFrame;
    u32 endFrame = command->endFrame;
    u32 startDuration = effBattleMiscGetTableEntry(command->startDurationIndex);
    u32 endDuration = effBattleMiscGetTableEntry(command->endDurationIndex);
    u32 restoreFrame;

    if ((unit->status.flags & 0xE0) != 0) {
        return;
    }
    if ((unit->status.flags & 2) == 0) {
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
        evtUnitHandle_t *eventUnit = (evtUnitHandle_t *)unit->ext;
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
        evtSetUnitColorRGB(eventUnit, startDuration, blendedColor);
    }
    if (frame == restoreFrame) {
        evtSetUnitColorRGB((evtUnitHandle_t *)unit->ext, endDuration, unit->baseColor);
    }
}

extern void dds3AdminiCallSequence(s32 requestedMode, void *requestData, u32 dataBytes, s32 markHistory);

void dds3AdminSubmitEmptyModeRequest(void) {
    dds3AdminiCallSequence(0, 0, 0, 0);
}

/* Return the compact work's payload address without changing its ownership. */
void *effParamWorkGetData(effPartHandle_t *work) {
    return work->payload;
}

/* Return the compact work's halfword effect kind. */
u16 effParamWorkGetId(effPartHandle_t *work) {
    return work->kind;
}

/* Allocate compact work and create its payload through the required kind factory. */
effPartHandle_t *dds3EffectPartCreate(u16 kind, void *source) {
    effPartHandle_t *work;

    work = sdfChipAllocDirect(sizeof(effPartHandle_t));
    work->kind = kind;
    work->payload = dds3EffectFuncTable[kind].create(source);
    return work;
}

/* Dispatch the payload before freeing its compact owner; this callback is required. */
void dds3EffectPartDelete(effPartHandle_t *work) {
    dds3EffectFuncTable[work->kind].release(work->payload);
    sdfChipFreeDirect(work);
}

/* Invoke the kind-specific callback on this parameter block. */
void dds3EffectPartDraw(effPartHandle_t *work) {
    u16 kind = work->kind;

    dds3EffectFuncTable[kind].dispatch(work->payload);
}

/* Allocate a second compact owner and duplicate the source payload by kind. */
effPartHandle_t *dds3EffectPartCopy(effPartHandle_t *source) {
    effPartHandle_t *work;

    work = sdfChipAllocDirect(sizeof(effPartHandle_t));
    work->kind = source->kind;
    work->payload = dds3EffectFuncTable[source->kind].duplicate(source->payload);
    return work;
}

/* Optional compact-work operations, indexed by effect kind. */
void dds3EffectPartSetPosition(effPartHandle_t *work, void *source) {
    if (dds3EffectFuncTable[work->kind].callback0 != NULL) {
        dds3EffectFuncTable[work->kind].callback0(work->payload, source);
    }
}

void dds3EffectPartSetScale(effPartHandle_t *work, f32 scale) {
    if (dds3EffectFuncTable[work->kind].setScale != NULL) {
        dds3EffectFuncTable[work->kind].setScale(work->payload, scale);
    }
}

void dds3EffectPartSetRotateMatrix(effPartHandle_t *work, void *matrix) {
    if (dds3EffectFuncTable[work->kind].callback2 != NULL) {
        dds3EffectFuncTable[work->kind].callback2(work->payload, matrix);
    }
}

void dds3EffectPartSetColor(effPartHandle_t *work, u32 value) {
    if (dds3EffectFuncTable[work->kind].callback3 != NULL) {
        dds3EffectFuncTable[work->kind].callback3(work->payload, value);
    }
}

void dds3EffectPartSetDirection(effPartHandle_t *work, void *matrix) {
    if (dds3EffectFuncTable[work->kind].callback4 != NULL) {
        dds3EffectFuncTable[work->kind].callback4(work->payload, matrix);
    }
}

void dds3EffectPartSetArea(effPartHandle_t *work, f32 sizeInput) {
    if (dds3EffectFuncTable[work->kind].callback5 != NULL) {
        dds3EffectFuncTable[work->kind].callback5(work->payload, sizeInput);
    }
}

/* Create a child billboard from its resource data. */
billHandle_t *dds3EffectPartTMXBillboardCreate(void *resourceData) {
    return dds3BillboardCreate(0, (u32)resourceData);
}

/* Create an animated billboard from its serialized resource data. */
billHandle_t *dds3EffectPartP2ABillboardCreate(void *resourceData) {
    return dds3BillboardCreate(1, (u32)resourceData);
}

/* Use the same floating value for both billboard child-scale components. */
void dds3EffectPartBillboardScale(void *payload, f32 scale) {
    dds3BillboardSetScale((billHandle_t *)payload, scale, scale);
}

/* Load the default primary/rotation/tertiary vectors and packed broadcast value.
 * Reset the first motion node's float to 1.0f, then clear flag bit zero. */
void dds3EffectPartModelInit(dds3ModelHandle_t *work) {
    VU0_LOAD_VF(vf10, &effModelDefaultPosition);
    mdlSetPosition(work);
    VU0_LOAD_VF(vf10, &effModelDefaultQuaternion);
    mdlSetRotate(work);
    VU0_LOAD_VF(vf10, &effModelDefaultScale);
    mdlSetScale(work);
    mdlSetColor(work, 0x80808080);
    if (work->first != NULL) {
        mdlLoopMotion(work, 0, 0);
        work->first->frameStep = 1.0f;
    }
    work->flags &= ~MDL_SKIP_TRANSFORMS;
}

/* Create and initialize a viewer-package context in the fixed effect group.
 * DDS1 consumes the current halfword id directly; it does not scan occupied ids. */
void *effParamCreateInitWork(void *package) {
    dds3ModelHandle_t *work;

    mdlPackDecode(EFF_VIEWER_RESOURCE_GROUP, D_003BB044, EFF_VIEWER_LOAD_FLAGS, (u8 *)package + 0x10, *(u32 *)package);
    work = mdlLoad(EFF_VIEWER_RESOURCE_GROUP, D_003BB044);
    dds3EffectPartModelInit(work);
    D_003BB044++;
    return work;
}

/* Run the context/node update with this game's fixed global argument. */
void dds3EffectPartModelDraw(void *work) {
    mdlLink(work, kwlnSceneSurfaceSets[10]);
}

/* Release the model context owned by parameter work. */
void dds3EffectPartModelDelete(void *work) {
    mdlFree(work);
}

/* Resolve the source's resource group/id, obtain its context, and initialize it. */
void *dds3EffectPartModelCopy(void *source) {
    s32 resourceGroup;
    s32 resourceId;
    dds3ModelHandle_t *work;

    resourceGroup = mdlGetMajorNum(source);
    resourceId = mdlGetMinorNum(source);
    work = mdlLoad(resourceGroup, resourceId);
    dds3EffectPartModelInit(work);
    return work;
}

/* Load a full source quadword into vf10 and store it as the primary vector. */
void dds3EffectPartModelSetPosition(void *work, void *vector) {
    VU0_LOAD_VF_MEMORY(vf10, vector);
    mdlSetPosition(work);
}

/* Initialize only three components before the quadword VU load.
 * The fourth component is not initialized here; retain the native array size. */
void dds3EffectPartModelSetScale(void *work, f32 scalar) {
    f32 components[4];

    components[0] = components[1] = components[2] = scalar;
    VU0_LOAD_VF_MEMORY(vf10, components);
    mdlSetScale(work);
}

/* Load four source quadwords, then scatter only vf28-vf30 to the destination block. */
void dds3EffectPartModelSetRotateMatrix(dds3ModelHandle_t *work, void *matrix) {
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

/* Set model color and broadcast its alpha with neutral RGB to the resources. */
void dds3EffectPartModelSetColor(void *work, u32 color) {
    mdlSetColor(work, color);
}

extern void **effParameterPresetBanks[];
extern u32 dds3EffectPCPDataGetKateg(const u32 *descriptor);
extern u32 dds3EffectPCPDataGetParam(const u32 *descriptor);

/* Create extended work from a kind/index descriptor. Kinds with a duplicate
 * callback consume the raw descriptor; other kinds use the fallback table. */
EffParamWorkEx *dds3EffectPCPCreate(u32 *source) {
    EffParamWorkEx *work;

    work = sdfChipAllocDirect(sizeof(EffParamWorkEx));
    work->kind = dds3EffectPCPDataGetKateg(source);
    work->tableIndex = dds3EffectPCPDataGetParam(source);
    if (EffectPCPFuncTable[work->kind].duplicate == NULL) {
        work->payload = EffectPCPFuncTable[work->kind].create(effParameterPresetBanks[work->kind][work->tableIndex]);
    } else {
        work->payload = EffectPCPFuncTable[work->kind].create(source);
    }
    return work;
}

/* Release the extended payload through its required kind callback, then its owner. */
void dds3EffectPCPDelete(EffParamWorkEx *work) {
    EffectPCPFuncTable[work->kind].release(work->payload);
    sdfChipFreeDirect(work);
}

/* Invoke the extended work's required dispatch callback. */
void dds3EffectPCPDraw(EffParamWorkEx *work) {
    EffectPCPFuncTable[work->kind].dispatch(work->payload);
}

/* Recreate table-backed payloads; duplicate payloads only for raw-source kinds. */
EffParamWorkEx *dds3EffectPCPCopy(EffParamWorkEx *source) {
    EffParamWorkEx *work;

    work = sdfChipAllocDirect(sizeof(EffParamWorkEx));
    work->kind = source->kind;
    work->tableIndex = source->tableIndex;
    if (EffectPCPFuncTable[work->kind].duplicate == NULL) {
        work->payload = EffectPCPFuncTable[work->kind].create(effParameterPresetBanks[work->kind][work->tableIndex]);
    } else {
        work->payload = EffectPCPFuncTable[work->kind].duplicate(source->payload);
    }
    return work;
}

/* Optional extended-work dispatches; preserve the native column order,
 * including the reversed fourth/fifth column addresses. */
void dds3EffectPCPSetPosition(EffParamWorkEx *work, void *source) {
    if (EffectPCPFuncTable[work->kind].callback0 != NULL) {
        EffectPCPFuncTable[work->kind].callback0(work->payload, source);
    }
}

void dds3EffectPCPSetScale(EffParamWorkEx *work, f32 scale) {
    if (EffectPCPFuncTable[work->kind].setScale != NULL) {
        EffectPCPFuncTable[work->kind].setScale(work->payload, scale);
    }
}

void dds3EffectPCPSetRotateMatrix(EffParamWorkEx *work, void *matrix) {
    if (EffectPCPFuncTable[work->kind].callback2 != NULL) {
        EffectPCPFuncTable[work->kind].callback2(work->payload, matrix);
    }
}

void dds3EffectPCPSetDirection(EffParamWorkEx *work, void *matrix) {
    if (EffectPCPFuncTable[work->kind].callback4 != NULL) {
        EffectPCPFuncTable[work->kind].callback4(work->payload, matrix);
    }
}

void dds3EffectPCPSetColor(EffParamWorkEx *work, u32 value) {
    if (EffectPCPFuncTable[work->kind].callback3 != NULL) {
        EffectPCPFuncTable[work->kind].callback3(work->payload, value);
    }
}

void dds3EffectPCPSetArea(EffParamWorkEx *work, f32 sizeInput) {
    if (EffectPCPFuncTable[work->kind].callback5 != NULL) {
        EffectPCPFuncTable[work->kind].callback5(work->payload, sizeInput);
    }
}

/* Read the descriptor's first word as the extended-work kind. */
u32 dds3EffectPCPDataGetKateg(const u32 *descriptor) {
    return descriptor[0];
}

/* Read the descriptor's second word as its fallback table index. */
u32 dds3EffectPCPDataGetParam(const u32 *descriptor) {
    return descriptor[1];
}

/* Records follow a 16-byte header and have a 16-byte stride. Block offsets
 * are signed and relative to the whole table, not to individual records.
 * Neither the index nor the resulting target address is validated here. */
void *dds3EffectPCPDataGetData(const EffParamTable *table, s32 recordIndex) {
    const EffParamTableRecord *record = &table->records[recordIndex];

    return (u8 *)table + record->blockOffset;
}

/* Return the record's first word without interpreting its meaning. */
u32 dds3EffectPCPDataGetSize(const EffParamTable *table, s32 recordIndex) {
    const u8 *bytes = (const u8 *)table;

    bytes += recordIndex * sizeof(EffParamTableRecord);
    return *(const u32 *)(bytes + sizeof(table->header));
}

/* Return the full record word whose low halfword selects a compact-work factory. */
u32 dds3EffectPCPDataGetType(const EffParamTable *table, s32 recordIndex) {
    const u8 *bytes = (const u8 *)table;

    bytes += recordIndex * sizeof(EffParamTableRecord);
    return *(const u32 *)(bytes + sizeof(table->header) +
                          sizeof(table->records[0].word0) +
                          sizeof(table->records[0].blockOffset));
}

/* Create compact work from a serialized table record. */
effPartHandle_t *dds3EffectPCPDataPartCreate(const EffParamTable *table, s32 recordIndex) {
    u16 kind;
    void *source;

    kind = (u16)dds3EffectPCPDataGetType(table, recordIndex);
    source = dds3EffectPCPDataGetData(table, recordIndex);
    return dds3EffectPartCreate(kind, source);
}




/* Allocate the copied head and its trailing cells as one block, then create
 * the cell system with native arguments groupDivisor=0 and kind=4.
 * Only three words per cell are zeroed here; vector/range storage is untouched. */
EffThunderVectorWork *effPCPThunderSpherePolyCreate(EffThunderAlphaParams *source) {
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(source->cellCount * sizeof(EffThunderVectorCell) + sizeof(EffThunderVectorWork));
    EffThunderVectorWork *work = (EffThunderVectorWork *)sdfMemLock(allocation);
    u32 cellIndex;

    work->head.alpha = *source;
    work->cells = (EffThunderVectorCell *)(work + 1);
    work->baseRadiusScale = source->radiusScale;
    work->baseHeightScale = source->heightScale;
    work->allocationHandle = allocation;
    work->cellSystem = dds3TrackPolyCreate(work->head.alpha.cellCount, work->head.alpha.perCell, 0, PAR_CELL_TOPOLOGY_FIVE_VECTOR);
    dds3TrackPolyVC5Set_AcuteW(work->cellSystem, work->head.alpha.centerAlphaWord, work->head.alpha.middleAlphaWord, work->head.alpha.edgeAlphaWord);
    dds3TrackPolySetMode(work->cellSystem, work->head.alpha.systemParam);
    for (cellIndex = 0; cellIndex < work->head.alpha.cellCount; cellIndex++) {
        work->cells[cellIndex].delayFrames = 0;
        work->cells[cellIndex].activeFrames = 0;
        work->cells[cellIndex].color = 0;
    }
    work->tintColor = 0x80808080;
    return work;
}

INCLUDE_SDATA(const s32, "game/code_001624D0", D_003BB044);

