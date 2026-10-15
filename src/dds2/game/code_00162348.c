#include "common.h"
#include "par_release_record.h"
#include "par_vertex_colors.h"
#include "eff_random.h"
#include "sdf_asset_state.h"
#include "sdf_asset_create.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "sdf_packet_append.h"
#include "bill_object_api.h"
#include "par_cell_api.h"
#include "sdf_resource.h"
#include "par_draw.h"
#include "eff_vertex_draw_request.h"
#include "par_kind_api.h"
#include "par_kind_resource.h"
#include "par_table.h"
#include "par_draw_block.h"
#include "par_emitter_desc.h"
#include "par_object_client_view.h"

#include "eff.h"
#include "eff_track_poly_list.h"

#include "pcp_vu0.h"
#include "ee_mmi.h"

typedef struct ParListNode {
    u8 pad00[0x54];
    struct ParListNode *next;
} ParListNode;





extern f32 D_00451F30[];

extern f32 D_00451F40[];

extern u8 sdfViewEyeVector[];

extern u8 sdfViewTargetVector[];

extern f32 D_003AAEC0[];

extern f32 D_003AAEE0[];

extern f32 D_003AAEF0[];

extern f32 D_003AAF00[];

extern ParListNode *parRecordListHead;

extern ParSystem *D_00436404;

extern void (*D_003AAF10[])(void *, void *, void *);

extern BillDispatch D_003AAB88[];

extern u16 parGetRestartFlag(ParObj *obj);



extern void parUpdateCellVertexTriangle(ParSystem *, s32, const u128 *);

extern u8 parObjGetMode(ParObj *object);


extern void parUpdateBillboardCrossStrip(s32 particle, s32 index, u32 color);

extern void parUpdateBillboardCrossTriangle(s32 particle, s32 index, u32 color);

extern void effBillSetEntryValue(s32, s32, u32);


extern void parFadeTriangleCellAlpha(ParSystem *system, s32 index);


#include "sdf_packet_alloc.h"

struct SdfListHead;





extern void *memcpy(void *dst, void *src, u32 n);
extern void polyInitializeBasicRingCells(void *);

extern void parClearHistorySlotPoints(s32);





void parSetObjectParameterWord(ParObj *work, u32 value) {
    work->parameterWord = value;
}

void parObjSetMode(ParObj *object, u8 mode) {
    switch (object->dispatchIndex) {
    case 1:
    case 5:
    case 11:
        ((ParObjClientView *)object)->mode150 = mode;
        break;
    case 0:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
    case 8:
    case 10:
    case 12:
        ((ParObjClientView *)object)->mode151 = mode;
        break;
    }
    object->restartFlag = 1;
}

u8 parObjGetMode(ParObj *object) {
    switch (object->dispatchIndex) {
    case 1:
    case 5:
    case 11:
        return ((ParObjClientView *)object)->mode150;
    case 0:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
    case 8:
    case 10:
    case 12:
        return ((ParObjClientView *)object)->mode151;
    default:
        return 0;
    }
}

extern ParDispatch parKindConstructorEntries[];








ParObj *parCreateResourceKindObject(s32 kind, ParKindResource *resource) {
    ParObj *object = (ParObj *)((u8 *)resource + resource->offset + 0x10);
    BillObj *billboard;

    if (resource->type != 3 || resource->offset != 0) {
        object->billboardCloneMarker = PAR_BILLBOARD_CLONE_FROM_RESOURCE;
        object = parKindConstructorEntries[kind].func(object);
        billboard = billCreateIndexed(resource->type, (u32)(resource + 1));
        billSetChildScaleComponents(billboard, object->billboardScale, object->billboardScaleY);
        billSetBillboardMode(billboard, object->billboardMode);
        billMarkKindOneFlag(billboard);
        object->billboard = billboard;
    } else {
        object = parKindConstructorEntries[kind].func(object);
    }
    object->dispatchIndex = kind;
    return object;
}

ParObj *parInstantiateKind(ParObj *source) {
    ParObj *particle;
    BillObj *billboard;

    particle = parKindConstructorEntries[source->dispatchIndex].func(source);
    particle->dispatchIndex = source->dispatchIndex;
    if (source->billboardCloneMarker == PAR_BILLBOARD_CLONE_FROM_RESOURCE) {
        billboard = billCloneObjectRetainingSharedData(source->billboard);
        billSetChildScaleComponents(billboard, particle->billboardScale, particle->billboardScaleY);
        billSetBillboardMode(billboard, particle->billboardMode);
        billMarkKindOneFlag(billboard);
        particle->billboard = billboard;
    }
    return particle;
}

void parObjDispatch(ParObj *object) {
    D_003AAB88[object->dispatchIndex].func();
}

extern void billInvokeCallback(BillObj *);

/* Apply queued updates, then draw live records and submit the kind's resource. */
void effParUpdateAndDrawInstance(ParObj *effect) {
    EffectBufferRecord *record;
    BillObj *billboard;
    u32 step;
    s32 particleCount;
    s32 lifetime;
    f32 scaleStep;
    f32 spinStep;
    f32 spinDirection;
    f32 alternate;

    if (effect->restartFlag == 0) {
        return;
    }
    if (effect->pendingRestartSteps == 0) {
        parKindConstructorEntries[effect->dispatchIndex].update(effect);
    } else {
        for (step = 0; step < effect->pendingRestartSteps; step++) {
            parKindConstructorEntries[effect->dispatchIndex].update(effect);
        }
        particleCount = effect->particleCount;
        scaleStep = (effect->scaleEnd - effect->billboardScale) / effect->lifetimeFrames;
        spinStep = effect->spinDegrees * (3.14159265f / 180.0f);
        alternate = effect->alternateSpin ? -1.0f : 1.0f;
        for (step = 0; step < effect->pendingRestartSteps; step++) {
            s32 recordIndex;

            record = effect->buffer->records;
            spinDirection = 1.0f;
            for (recordIndex = 0; recordIndex < particleCount; recordIndex++, record++) {
                if (record->age >= 0) {
                    record->scale += scaleStep;
                    record->spin += spinStep * spinDirection;
                }
                spinDirection *= alternate;
            }
        }
        effect->pendingRestartSteps = 0;
    }
    /* Capture the draw owners before deriving this frame's scale/spin steps. */
    record = effect->buffer->records;
    billboard = effect->billboard;
    particleCount = effect->particleCount;
    scaleStep = (effect->scaleEnd - effect->billboardScale) / effect->lifetimeFrames;
    lifetime = effect->lifetimeFrames;
    spinStep = effect->spinDegrees * (3.14159265f / 180.0f);
    alternate = effect->alternateSpin ? -1.0f : 1.0f;
    spinDirection = 1.0f;
    if (particleCount > 0) {
        step = particleCount;
        do {
            s32 age = record->age;

            if (age < lifetime && age >= 0) {
                PCP_COPY_VECTOR(billboard, record->position);
                billboard->rotationAngle = record->spin;
                billboard->childParam = record->color;
                billboard->childScaleX = billboard->childScaleY = record->scale;
                record->scale += scaleStep;
                if (record->scale < 0) {
                    record->scale = 0;
                }
                record->spin += spinStep * spinDirection;
                billSetAnimationFrameForImmediateAdvance(billboard, age);
                billInvokeCallback(billboard);
            }
            spinDirection *= alternate;
            --step;
            ++record;
        } while (step != 0);
    }
    parSubmitKindDrawing(&effect->kindState);
}

void parRestartInstanceCallback(void) {
    parRestartKind();
}

void effParScaleComponent(float scale, ParObj *work) {
    parScaleAndRestartKind();
    work->scaleEnd = work->scaleEnd * scale;
}

/* Descriptor query: return the complete 16-bit restart state. */
u16 effParGetInstanceRestartFlag(ParObj *obj) {
    return parGetRestartFlag(obj);
}

void parCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void parRebuildInstanceTransforms(void) {
    parComposeEffectTransformMatrices();
}

void effParSetInstanceParameterWord(ParObj *work, u32 value) {
    work->parameterWord = value;
}

void parChangeInstanceMode(ParObj *work, u8 mode) {
    parObjSetMode(work, mode);
}

/* Descriptor query: select the kind's current mode byte. */
u8 effParGetInstanceMode(ParObj *work) {
    return parObjGetMode(work);
}

/* Kinds 2-4 keep the scale at +8 of their own record; copy it into the
 * shared vector and store the (vf10 - vf11) difference. */
void parUpdateSharedScaleAndDelta(ParKindState *obj) {
    f32 scale;

    switch (obj->kind) {
    case 0:
    case PAR_KIND_STATE_HISTORY:
        return;
    case PAR_KIND_STATE_BILLBOARD_CROSS_STRIP:
        scale = obj->payload.crossStrip.scale;
        D_00451F40[0] = D_00451F40[1] = D_00451F40[2] = scale;
        break;
    case PAR_KIND_STATE_BILLBOARD_CROSS_TRIANGLE:
        scale = obj->payload.crossTriangle.scale;
        D_00451F40[0] = D_00451F40[1] = D_00451F40[2] = scale;
        break;
    case PAR_KIND_STATE_TRACK_POLYGON_CROSS_AXES:
        scale = obj->payload.trackPolygon.scale;
        D_00451F40[0] = D_00451F40[1] = D_00451F40[2] = scale;
        break;
    default:
        return;
    }
    VU0_LOAD_VF($vf10, sdfViewEyeVector);
    VU0_LOAD_VF($vf11, sdfViewTargetVector);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF($vf10, D_00451F30);
}

void parDispatchKindUpdate(ParKindState *work, s32 index, u32 color, f32 speed) {
    switch ((u16)work->kind) {
    case 1:
        parPrependHistorySample(work->payload.history.table, index, color, speed);
        return;
    case 2:
        parUpdateBillboardCrossStrip((s32)work->payload.crossStrip.drawSystem, index, color);
        return;
    case 3:
        parUpdateBillboardCrossTriangle((s32)work->payload.crossTriangle.drawSystem, index, color);
        return;
    case 4:
        parUpdateTrackPolygonCrossAxes(work->payload.trackPolygon.modelList, index, color);
        break;
    }
}

void parDispatchKindInit(ParKindState *work, s32 index) {
    switch ((u16)work->kind) {
    case 1:
        parClearHistorySlotPoints((s32)work->payload.history.table);
        return;
    case 2:
        parCellInit(work->payload.crossStrip.drawSystem, index);
        return;
    case 3:
        parCellInit(work->payload.crossTriangle.drawSystem, index);
        return;
    case 4:
        effTrackPolyResetIndexedWork(work->payload.trackPolygon.modelList, index);
        break;
    }
}

void parUpdateBillboardCrossStrip(s32 particle, s32 index, u32 color) {
    u128 axis[2];
    VU0_MOVE_VF(vf11, vf12);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_00451F30);
;
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_LOAD_VF(vf11, D_00451F40);
;
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf2, vf10);
    VU0_MOVE_VF(vf10, vf12);
    VU0_MOVE_VF(vf12, vf2);
    VU0_MOVE_VF(vf11, vf10);
    VU0_ADD(vf10, vf10, vf12);
    VU0_STORE_VF(vf10, &axis[0]);
;
    VU0_MOVE_VF(vf10, vf12);
    VU0_SUB(vf11, vf11, vf10);
    VU0_STORE_VF(vf11, &axis[1]);
;
    parUpdateCellVertexPair((ParSystem *)particle, index, axis);
    parFadeAlphaCell((ParSystem *)particle, index);
    effBillSetEntryValue(particle, index, (color & 0xFF000000) | 0x808080);
}

void parUpdateBillboardCrossTriangle(s32 particle, s32 index, u32 color) {
    u128 axis[3];
    VU0_MOVE_VF(vf11, vf12);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_00451F30);
;
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_LOAD_VF(vf11, D_00451F40);
;
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf2, vf10);
    VU0_MOVE_VF(vf10, vf12);
    VU0_MOVE_VF(vf12, vf2);
    VU0_STORE_VF(vf10, &axis[1]);
;
    VU0_MOVE_VF(vf11, vf10);
    VU0_ADD(vf10, vf10, vf12);
    VU0_STORE_VF(vf10, &axis[0]);
;
    VU0_MOVE_VF(vf10, vf12);
    VU0_SUB(vf11, vf11, vf10);
    VU0_STORE_VF(vf11, &axis[2]);
;
    parUpdateCellVertexTriangle((ParSystem *)particle, index, axis);
    parFadeTriangleCellAlpha((ParSystem *)particle, index);
    effBillSetEntryValue(particle, index, (color & 0xFF000000) | 0x808080);
}

void parUpdateTrackPolygonCrossAxes(EffTrackPolyList *list, s32 index, u32 color) {
    u128 axis[2];
    VU0_MOVE_VF(vf11, vf12);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_00451F30);
;
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_LOAD_VF(vf11, D_00451F40);
;
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf2, vf10);
    VU0_MOVE_VF(vf10, vf12);
    VU0_MOVE_VF(vf12, vf2);
    VU0_MOVE_VF(vf11, vf10);
    VU0_ADD(vf10, vf10, vf12);
    VU0_STORE_VF(vf10, &axis[0]);
;
    VU0_MOVE_VF(vf10, vf12);
    VU0_SUB(vf11, vf11, vf10);
    VU0_STORE_VF(vf11, &axis[1]);
;
    effTrackPolyPushIndexedWorkEndpoints(list, index, axis);
    effTrackPolySetIndexedColor(list, index, (color & 0xFF000000) | 0x808080);
}

u32 parGetEmitterCellGroupDivisor(u16 pointCapacity) {
    return 0;
}

extern u8 D_00451F50[];


void parSysReset(void) {
    parRecordListHead = 0;
    parControlInit();
    effMiscSeedRandomFromClock((EffRandState *)D_00451F50);
}

void parInitializeParticleSubsystem(void) {
}

INCLUDE_ASM(const s32, "game/code_00162348", func_00162E48);

void parReleaseAssetRecord(ParReleaseRecord *record) {
    record->flags = 1;
    sdfQueueAssetRelease(record->asset);
    sdfReleaseResourceAllocation(record->allocation);
}

void parPrependRecordListNode(ParListNode *node) {
    node->next = parRecordListHead;
    parRecordListHead = node;
}

INCLUDE_ASM(const s32, "game/code_00162348", parDrawPendingParticleRecords);

/* Replace the record asset's primary texture and dirty its draw state. */
void effParSetAssetRecordTexture(ParReleaseRecord *record, SdfTex *texture) {
    sdfSetAssetPrimaryTextureAddress(record->asset, texture);
}

/* Draw parameter block filled per strip by parDrawPendingCellSystems. */
extern EffVertexDrawRequest parDrawControl;
extern void *effCreateVertexDrawPacket(EffVertexDrawRequest *);

void parControlInit(void) {
    memset(&parDrawControl, 0, 0x2C);
    parDrawControl.flags = 0x4000;
}

ParSystem *parAllocateCellSystem(s32 count, s32 perCell, s32 groupDivisor, u32 kind) {
    s32 total;
    struct SdfMemBlock *allocation;
    s32 base;
    s32 cellsSize;
    ParSystem *system;
    s32 i;

    if (kind == PAR_CELL_TOPOLOGY_FIVE_VECTOR) {
        perCell = perCell * 5 + 5;
    } else if (kind == PAR_CELL_TOPOLOGY_FOUR_VECTOR) {
        perCell = perCell * 4 + 4;
    } else if (kind == PAR_CELL_TOPOLOGY_SIX_VECTOR) {
        perCell = perCell * 6 + 6;
    } else {
        perCell = perCell * 2;
        if (groupDivisor != 0) {
            if (perCell % groupDivisor != 0) {
                perCell = perCell / groupDivisor + 2;
            } else {
                perCell = perCell / groupDivisor;
            }
        }
        perCell &= ~1;
        perCell += 2;
        if (kind == PAR_CELL_TOPOLOGY_TRIANGLE) {
            perCell += perCell >> 1;
        }
    }
    total = count * perCell;
    cellsSize = (total + count) * 0x14;
    allocation = sdfAllocGeneralBlock(cellsSize + 0x2C);
    base = sdfResourceRetainAddress(allocation);
    system = (ParSystem *)(base + cellsSize);
    memset(system, 0, 0x2C);
    system->vertices = (u128 *)base;
    base += total * 0x10;
    system->colors = (u32 *)base;
    base += total * 4;
    system->cells = (ParCell *)base;
    for (i = 0; i < count; i++) {
        ParCell *cell = (ParCell *)(i * sizeof(ParCell) + (s32)system->cells);
        cell->history = (u128 *)((u8 *)system->vertices + i * perCell * 0x10);
        cell->colors = system->colors + i * perCell;
        parCellInit(system, i);
    }
    system->asset = sdfCreateAssetWithDrawEntries();
    sdfSetPrimaryStateFloat(system->asset, 1.0f);
    system->kind = kind;
    system->cellCount = count;
    system->bucket = 2;
    system->vertexWordCount = perCell;
    system->groupDivisor = groupDivisor;
    system->allocation = allocation;
    system->next = 0;
    system->unk28 = 0;
    return system;
}

void parReleaseCellSystem(ParSystem *system) {
    sdfQueueAssetRelease(system->asset);
    sdfReleaseResourceAllocation(system->allocation);
}

void parCellInit(ParSystem *system, s32 index) {
    ParCell *cell = (ParCell *)(index * sizeof(ParCell) + (s32)system->cells);

    cell->color = 0x80808080;
    cell->historyAdvanceCountdown = 0;
    cell->vertexCount = 0;
}

void parPrependCellNode(ParSystem *node) {
    node->next = D_00436404;
    D_00436404 = node;
}

void parUpdateCellVertexPair(ParSystem *system, s32 index, const u128 *vertices) {
    ParCell *cell = &system->cells[index];
    u128 *vertex;
    s32 shiftCount;
    s32 i;

    if (cell->historyAdvanceCountdown == 0) {
        shiftCount = system->vertexWordCount - 2;
        vertex = cell->history + shiftCount;
        for (i = 0; i < shiftCount; i++) {
            vertex--;
            PCP_COPY_VECTOR(vertex + 2, vertex);
        }
        cell->historyAdvanceCountdown = system->groupDivisor;
        if (cell->vertexCount < shiftCount + 2) {
            cell->vertexCount += 2;
        }
    } else {
        cell->historyAdvanceCountdown--;
        vertex = cell->history;
    }
    PCP_COPY_VECTOR(vertex, vertices);
    PCP_COPY_VECTOR(vertex + 1, vertices + 1);
}

void parTranslateCellVertices(ParSystem *system, s32 index, const u128 *delta) {
    ParCell *cell = system->cells + index;
    s32 count = system->vertexWordCount;
    u128 *vertex = cell->history;
    s32 i;
    VU0_LOAD_VF_MEMORY(vf11, delta);
    if (count > 0) {
        i = count;
        do {
            VU0_LOAD_VF(vf10, vertex);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, vertex);
            i--;
            vertex++;
        } while (i != 0);
    }
}

void parUpdateCellVertexTriangle(ParSystem *system, s32 index, const u128 *vertices) {
    ParCell *cell = &system->cells[index];
    u128 *vertex;
    s32 shiftCount;
    s32 i;

    if (cell->historyAdvanceCountdown == 0) {
        shiftCount = system->vertexWordCount - 3;
        vertex = cell->history + shiftCount;
        for (i = 0; i < shiftCount; i++) {
            vertex--;
            PCP_COPY_VECTOR(vertex + 3, vertex);
        }
        cell->historyAdvanceCountdown = system->groupDivisor;
        if (cell->vertexCount < shiftCount + 3) {
            cell->vertexCount += 3;
        }
    } else {
        cell->historyAdvanceCountdown--;
        vertex = cell->history;
    }
    PCP_COPY_VECTOR(vertex, vertices);
    PCP_COPY_VECTOR(vertex + 1, vertices + 1);
    PCP_COPY_VECTOR(vertex + 2, vertices + 2);
}

void parTranslateCellTriangleVertices(ParSystem *system, s32 index, const u128 *delta) {
    ParCell *cell = system->cells + index;
    s32 count = cell->vertexCount / 3;
    u128 *vertex = cell->history;
    s32 i;
    VU0_LOAD_VF(vf11, delta);
    if (count > 0) {
        i = count;
        do {
            VU0_LOAD_VF(vf10, vertex);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, vertex);
            VU0_LOAD_VF(vf10, vertex + 1);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, vertex + 1);
            VU0_LOAD_VF(vf10, vertex + 2);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, vertex + 2);
            i--;
            vertex += 3;
        } while (i != 0);
    }
}

void parFadeAlphaCell(ParSystem *system, s32 index) {
    u32 count = system->cells[index].vertexCount >> 1;
    u32 *vertex = system->cells[index].colors;
    u32 word = vertex[0];
    u32 i;
    s32 alpha[4];
    s32 start[4];
    s32 out[4];
    u32 packed;
    if (count >= 2) {
        alpha[0] = word & 0xFF000000;
        EE_MMI_RGBA_UNPACK(alpha, 1.0f / 128.0f);
        VU0_MOVE_VF(vf11, vf10);
        VU0_LOAD_VF(vf10, D_003AAEC0);
        VU0_LERP_VF10(1.0f / count);
        VU0_MOVE_VF(vf12, vf10);
        start[0] = word;
        EE_MMI_RGBA_UNPACK(start, 1.0f / 128.0f);
        for (i = 0; i < count; i++) {
            VU0_MOVE_VF(vf11, vf10);
            EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
            out[0] = packed;
            vertex[0] = packed;
            vertex[1] = packed;
            VU0_SUB(vf11, vf11, vf12);
            VU0_MOVE_VF(vf10, vf11);
            vertex += 2;
        }
    }
}

extern f32 D_003AAED0[4];
/* Fade one triangle cell's middle and edge colors toward transparent along its history. */
void parFadeTriangleCellAlpha(ParSystem *system, s32 index) {
    u32 count = system->cells[index].vertexCount / 3;
    ParTriangleVertexColors *vertex;
    u32 middleWord;
    u32 edgeWord;
    f32 cur0[4];
    f32 cur1[4];
    f32 step0[4];
    f32 step1[4];
    s32 alpha0[4];
    s32 alpha1[4];
    s32 col0[4];
    s32 col1[4];
    s32 out0[4];
    s32 out1[4];
    u32 j;
    u32 packedMiddle;
    u32 packedEdge;

    if (count >= 2) {
        vertex = (ParTriangleVertexColors *)system->cells[index].colors;
        middleWord = vertex->middle;
        edgeWord = vertex->edge0;
        alpha0[0] = middleWord & 0xFF000000;
        EE_MMI_RGBA_UNPACK(alpha0, 1.0f / 128.0f);
        VU0_MOVE_VF(vf11, vf10);
        VU0_LOAD_VF(vf10, D_003AAED0);
        VU0_LERP_VF10(1.0f / count);
        VU0_STORE_VF(vf10, step0);
        alpha1[0] = edgeWord & 0xFF000000;
        EE_MMI_RGBA_UNPACK(alpha1, 1.0f / 128.0f);
        VU0_MOVE_VF(vf11, vf10);
        VU0_LOAD_VF(vf10, D_003AAED0);
        VU0_LERP_VF10(1.0f / count);
        VU0_STORE_VF_UNCLOBBERED(vf10, step1);
        col0[0] = middleWord;
        EE_MMI_RGBA_UNPACK(col0, 1.0f / 128.0f);
        VU0_STORE_VF_UNCLOBBERED(vf10, cur0);
        col1[0] = edgeWord;
        EE_MMI_RGBA_UNPACK(col1, 1.0f / 128.0f);
        VU0_STORE_VF_UNCLOBBERED(vf10, cur1);
        for (j = 0; j < count; j++) {
            VU0_LOAD_VF(vf10, cur0);
            VU0_MOVE_VF(vf11, vf10);
            EE_MMI_RGBA_PACK_UNIT(packedMiddle, 128.0f);
            out0[0] = packedMiddle;
            vertex->middle = packedMiddle;
            VU0_LOAD_VF(vf12, step0);
            VU0_SUB(vf11, vf11, vf12);
            VU0_STORE_VF(vf11, cur0);
            VU0_LOAD_VF(vf10, cur1);
            VU0_MOVE_VF(vf11, vf10);
            EE_MMI_RGBA_PACK_UNIT(packedEdge, 128.0f);
            out1[0] = packedEdge;
            vertex->edge0 = packedEdge;
            vertex->edge1 = packedEdge;
            VU0_LOAD_VF(vf12, step1);
            VU0_SUB(vf11, vf11, vf12);
            VU0_STORE_VF(vf11, cur1);
            vertex++;
        }
    }
}

void parFillVertexPairs(ParSystem *system, u32 firstColor, u32 secondColor) {
    s32 perCell = system->vertexWordCount >> 1;
    s32 cellCount = system->cellCount;
    s32 cellIndex;

    for (cellIndex = 0; cellIndex < cellCount; cellIndex++) {
        u32 *vertices = system->cells[cellIndex].colors;
        s32 i;

        for (i = 0; i < perCell; i++) {
            vertices[0] = firstColor;
            vertices[1] = secondColor;
            vertices += 2;
        }
    }
}

void parFadeAlphaAllCells(ParSystem *system, u32 color) {
    u32 perCell = system->vertexWordCount >> 1;
    u32 count = system->cellCount;
    u32 i;
    u32 j;
    u8 *cell;
    u32 *vertex;
    s32 alpha[4];
    s32 start[4];
    s32 out[4];
    u32 packed;
    alpha[0] = color & 0xFF000000;
    EE_MMI_RGBA_UNPACK(alpha, 1.0f / 128.0f);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, D_003AAEE0);
    VU0_LERP_VF10(1.0f / perCell);
    VU0_MOVE_VF(vf12, vf10);
    i = 0;
    if (count != 0) {
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(u32 **)cell;
            start[0] = color;
            EE_MMI_RGBA_UNPACK(start, 1.0f / 128.0f);
            for (j = 0; j < perCell; j++) {
                VU0_MOVE_VF(vf11, vf10);
                EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
                out[0] = packed;
                vertex[0] = packed;
                vertex[1] = packed;
                VU0_SUB(vf11, vf11, vf12);
                VU0_MOVE_VF(vf10, vf11);
                vertex += 2;
            }
            i++;
            cell += 0x14;
        } while (i < count);
    }
}

void parFadeAlphaUpDownAllCells(ParSystem *system, u32 color) {
    u32 words = system->vertexWordCount;
    u32 count = system->cellCount;
    u32 quarter = words >> 2;
    u32 i;
    u32 j;
    u8 *cell;
    u32 *vertex;
    s32 alpha[4];
    s32 start[4];
    s32 rise[4];
    s32 fall[4];
    u32 base;
    u32 odd;
    u32 packed;
    alpha[0] = color & 0xFF000000;
    EE_MMI_RGBA_UNPACK(alpha, 1.0f / 128.0f);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, D_003AAEF0);
    VU0_LERP_VF10(1.0f / quarter);
    VU0_MOVE_VF(vf12, vf10);
    i = 0;
    if (count != 0) {
        base = color & 0xFFFFFF;
        odd = (words >> 1) & 1;
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(u32 **)cell;
            start[0] = base;
            EE_MMI_RGBA_UNPACK(start, 1.0f / 128.0f);
            for (j = 0; j < quarter; j++) {
                VU0_MOVE_VF(vf11, vf10);
                EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
                rise[0] = packed;
                vertex[0] = packed;
                vertex[1] = packed;
                VU0_ADD(vf11, vf11, vf12);
                VU0_MOVE_VF(vf10, vf11);
                vertex += 2;
            }
            if (odd) {
                vertex[0] = color;
                vertex[1] = color;
                vertex += 2;
            }
            for (j = 0; j < quarter; j++) {
                VU0_MOVE_VF(vf11, vf10);
                EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
                fall[0] = packed;
                vertex[0] = packed;
                vertex[1] = packed;
                VU0_SUB(vf11, vf11, vf12);
                VU0_MOVE_VF(vf10, vf11);
                vertex += 2;
            }
            i++;
            cell += 0x14;
        } while (i < count);
    }
}

void parFillTriangleCellColors(ParSystem *system, s32 middleWord, s32 edgeWord) {
    s32 words = system->vertexWordCount;
    s32 count = system->cellCount;
    s32 perCell = words / 3;
    s32 i;
    s32 j;
    u8 *cell;
    u8 *vertex;
    if (count > 0) {
        i = count;
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(u8 **)cell;
            if (perCell > 0) {
                j = perCell;
                do {
                    j--;
                    *(s32 *)(vertex + 4) = middleWord;
                    *(s32 *)(vertex + 8) = edgeWord;
                    *(s32 *)(vertex + 0) = edgeWord;
                    vertex += 0xC;
                } while (j != 0);
            }
            i--;
            cell += 0x14;
        } while (i != 0);
    }
}

void parFadeAlphaTriangleAllCells(ParSystem *system, u32 middleWord, u32 edgeWord) {
    f32 cur0[4];
    f32 cur1[4];
    f32 step0[4];
    f32 step1[4];
    s32 alpha0[4];
    s32 alpha1[4];
    s32 col0[4];
    s32 col1[4];
    s32 out0[4];
    s32 out1[4];
    s32 groups = 3;
    u32 perCell = system->vertexWordCount / groups;
    u32 count = system->cellCount;
    u32 i;
    u32 j;
    u8 *cell;
    u32 *vertex;
    u32 packedMiddle;
    u32 packedEdge;
    alpha0[0] = middleWord & 0xFF000000;
    EE_MMI_RGBA_UNPACK(alpha0, 1.0f / 128.0f);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, D_003AAF00);
    VU0_LERP_VF10(1.0f / perCell);
    VU0_STORE_VF(vf10, step0);
    alpha1[0] = edgeWord & 0xFF000000;
    EE_MMI_RGBA_UNPACK(alpha1, 1.0f / 128.0f);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, D_003AAF00);
    VU0_LERP_VF10(1.0f / perCell);
    VU0_STORE_VF(vf10, step1);
    i = 0;
    if (count != 0) {
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(u32 **)cell;
            col0[0] = middleWord;
            EE_MMI_RGBA_UNPACK(col0, 1.0f / 128.0f);
            VU0_STORE_VF(vf10, cur0);
            col1[0] = edgeWord;
            EE_MMI_RGBA_UNPACK(col1, 1.0f / 128.0f);
            VU0_STORE_VF(vf10, cur1);
            for (j = 0; j < perCell; j++) {
                VU0_LOAD_VF(vf10, cur0);
                VU0_MOVE_VF(vf11, vf10);
                EE_MMI_RGBA_PACK_UNIT(packedMiddle, 128.0f);
                out0[0] = packedMiddle;
                vertex[1] = packedMiddle;
                VU0_LOAD_VF(vf12, step0);
                VU0_SUB(vf11, vf11, vf12);
                VU0_STORE_VF(vf11, cur0);
                VU0_LOAD_VF(vf10, cur1);
                VU0_MOVE_VF(vf11, vf10);
                EE_MMI_RGBA_PACK_UNIT(packedEdge, 128.0f);
                out1[0] = packedEdge;
                vertex[0] = packedEdge;
                vertex[2] = packedEdge;
                VU0_LOAD_VF(vf12, step1);
                VU0_SUB(vf11, vf11, vf12);
                VU0_STORE_VF(vf11, cur1);
                vertex += 3;
            }
            i++;
            cell += 0x14;
        } while (i < count);
    }
}

void parFadeTriangleCellAlphaUpDown(ParSystem *system, u32 middleWord, u32 edgeWord) {
    s32 words = system->vertexWordCount;
    s32 count = system->cellCount;
    u32 middleAlpha = middleWord & 0xFF000000;
    u32 edgeAlpha = edgeWord & 0xFF000000;
    s32 halfCount = (words / 3) >> 1;
    u32 middleStep = middleAlpha / halfCount;
    u32 edgeStep = edgeAlpha / halfCount;
    u32 middle;
    u32 edge;
    s32 i;
    s32 j;
    u8 *cell;
    ParTriangleVertexColors *vertex;

    middleWord &= 0xFFFFFF;
    edgeWord &= 0xFFFFFF;
    if (count > 0) {
        i = count;
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(ParTriangleVertexColors **)cell;
            middle = 0;
            edge = 0;
            for (j = 0; j < halfCount; j++, vertex++) {
                vertex->middle = middleWord | (middle & 0xFF000000);
                vertex->edge0 = vertex->edge1 = edgeWord | (edge & 0xFF000000);
                middle += middleStep;
                edge += edgeStep;
            }
            middle = middleAlpha;
            edge = edgeAlpha;
            for (j = 0; j < halfCount; j++, vertex++) {
                middle -= middleStep;
                edge -= edgeStep;
                vertex->middle = middleWord | (middle & 0xFF000000);
                vertex->edge0 = vertex->edge1 = edgeWord | (edge & 0xFF000000);
            }
            i--;
            cell += 0x14;
        } while (i != 0);
    }
}

void parFillStripCellColors(ParSystem *system, s32 centerWord, s32 middleWord, s32 edgeWord) {
    s32 words = system->vertexWordCount;
    s32 count = system->cellCount;
    s32 perCell = words / 6;
    s32 i;
    s32 j;
    u8 *cell;
    u8 *vertex;
    if (count > 0) {
        i = count;
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(u8 **)cell;
            if (perCell > 0) {
                j = perCell;
                do {
                    j--;
                    ((ParStripVertexColors *)vertex)->edge0 = edgeWord;
                    ((ParStripVertexColors *)vertex)->middle0 = middleWord;
                    ((ParStripVertexColors *)vertex)->center0 = centerWord;
                    ((ParStripVertexColors *)vertex)->center1 = centerWord;
                    ((ParStripVertexColors *)vertex)->middle1 = middleWord;
                    ((ParStripVertexColors *)vertex)->edge1 = edgeWord;
                    vertex += 0x18;
                } while (j != 0);
            }
            i--;
            cell += 0x14;
        } while (i != 0);
    }
}

/* Fade the three strip colors toward transparent across each cell. */
void parDecreaseStripCellAlpha(ParSystem *system, u32 centerWord, u32 middleWord, u32 edgeWord)
{
    s32 words = system->vertexWordCount;
    s32 count = system->cellCount;
    u32 centerAlpha = centerWord & 0xFF000000;
    u32 middleAlpha = middleWord & 0xFF000000;
    u32 edgeAlpha = edgeWord & 0xFF000000;
    s32 perCell = words / 6;
    u32 centerStep = centerAlpha / perCell;
    u32 middleStep = middleAlpha / perCell;
    u32 edgeStep = edgeAlpha / perCell;
    u32 center;
    u32 middle;
    u32 edge;
    s32 i;
    s32 j;
    u8 *cell;
    ParStripVertexColors *vertex;

    centerWord &= 0xFFFFFF;
    middleWord &= 0xFFFFFF;
    edgeWord &= 0xFFFFFF;
    if (count > 0) {
        i = count;
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(ParStripVertexColors **)cell;
            center = centerAlpha;
            middle = middleAlpha;
            edge = edgeAlpha;
            for (j = 0; j < perCell; j++, vertex++) {
                center -= centerStep;
                middle -= middleStep;
                edge -= edgeStep;
                vertex->edge0 = edgeWord | (edge & 0xFF000000);
                vertex->middle0 = middleWord | (middle & 0xFF000000);
                vertex->center0 = centerWord | (center & 0xFF000000);
                vertex->center1 = centerWord | (center & 0xFF000000);
                vertex->middle1 = middleWord | (middle & 0xFF000000);
                vertex->edge1 = edgeWord | (edge & 0xFF000000);
            }
            i--;
            cell += 0x14;
        } while (i != 0);
    }
}

/* Raise then lower the three alphas across the two halves of each strip cell. */
void parRiseFallStripCellAlpha(ParSystem *system, u32 centerWord, u32 middleWord, u32 edgeWord)
{
    s32 words = system->vertexWordCount;
    s32 count = system->cellCount;
    u32 centerAlpha = centerWord & 0xFF000000;
    u32 middleAlpha = middleWord & 0xFF000000;
    u32 edgeAlpha = edgeWord & 0xFF000000;
    s32 half = (words / 6) >> 1;
    u32 centerStep = centerAlpha / half;
    u32 middleStep = middleAlpha / half;
    u32 edgeStep = edgeAlpha / half;
    u32 center;
    u32 middle;
    u32 edge;
    s32 i;
    s32 j;
    u8 *cell;
    ParStripVertexColors *vertex;

    centerWord &= 0xFFFFFF;
    middleWord &= 0xFFFFFF;
    edgeWord &= 0xFFFFFF;
    if (count > 0) {
        i = count;
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(ParStripVertexColors **)cell;
            center = 0;
            middle = 0;
            edge = 0;
            for (j = 0; j < half; j++, vertex++) {
                vertex->edge0 = edgeWord | (edge & 0xFF000000);
                vertex->middle0 = middleWord | (middle & 0xFF000000);
                vertex->center0 = centerWord | (center & 0xFF000000);
                vertex->center1 = centerWord | (center & 0xFF000000);
                vertex->middle1 = middleWord | (middle & 0xFF000000);
                vertex->edge1 = edgeWord | (edge & 0xFF000000);
                center += centerStep;
                middle += middleStep;
                edge += edgeStep;
            }
            center = centerAlpha;
            middle = middleAlpha;
            edge = edgeAlpha;
            for (j = 0; j < half; j++, vertex++) {
                center -= centerStep;
                middle -= middleStep;
                edge -= edgeStep;
                vertex->edge0 = edgeWord | (edge & 0xFF000000);
                vertex->middle0 = middleWord | (middle & 0xFF000000);
                vertex->center0 = centerWord | (center & 0xFF000000);
                vertex->center1 = centerWord | (center & 0xFF000000);
                vertex->middle1 = middleWord | (middle & 0xFF000000);
                vertex->edge1 = edgeWord | (edge & 0xFF000000);
            }
            i--;
            cell += 0x14;
        } while (i != 0);
    }
}

void parFillCellVertexQuads(ParSystem *system, u32 firstColor, u32 secondColor) {
    s32 perCell = system->vertexWordCount >> 2;
    s32 cellCount = system->cellCount;

    s32 cellIndex;

    for (cellIndex = 0; cellIndex < cellCount; cellIndex++) {
        u32 *vertices = system->cells[cellIndex].colors;
        s32 i;

        for (i = 0; i < perCell; i++) {
            vertices[2] = firstColor;
            vertices[1] = firstColor;
            vertices[3] = secondColor;
            vertices[0] = secondColor;
            vertices += 4;
        }
    }
}

/* Keep middle/edge RGB while ramping vertex alpha down to zero in each cell. */
void effParSetCellColorsWithAlphaFade(ParSystem *system, u32 middleWord, u32 edgeWord) {
    s32 perCell = system->vertexWordCount >> 2;
    s32 count = system->cellCount;
    u32 middleAlpha = middleWord & 0xFF000000;
    u32 edgeAlpha = edgeWord & 0xFF000000;
    u32 middleStep = middleAlpha / perCell;
    u32 edgeStep = edgeAlpha / perCell;
    u32 middle;
    u32 edge;
    s32 i;
    s32 j;
    u8 *cell;
    ParQuadVertexColors *vertex;

    middleWord &= 0xFFFFFF;
    edgeWord &= 0xFFFFFF;
    if (count > 0) {
        i = count;
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(ParQuadVertexColors **)cell;
            middle = middleAlpha;
            edge = edgeAlpha;
            for (j = 0; j < perCell; j++, vertex++) {
                middle -= middleStep;
                edge -= edgeStep;
                vertex->middle1 = middleWord | (middle & 0xFF000000);
                vertex->edge1 = edgeWord | (edge & 0xFF000000);
                vertex->middle0 = middleWord | (middle & 0xFF000000);
                vertex->edge0 = edgeWord | (edge & 0xFF000000);
            }
            i--;
            cell += 0x14;
        } while (i != 0);
    }
}

/* Keep middle/edge RGB while ramping alpha up and down around the cell midpoint. */
void effParSetCellColorsWithAlphaPeak(ParSystem *system, u32 middleWord, u32 edgeWord) {
    s32 words = system->vertexWordCount;
    s32 count = system->cellCount;
    u32 middleAlpha = middleWord & 0xFF000000;
    u32 edgeAlpha = edgeWord & 0xFF000000;
    s32 halfCount = (words >> 2) >> 1;
    u32 middleStep = middleAlpha / halfCount;
    u32 edgeStep = edgeAlpha / halfCount;
    u32 middle;
    u32 edge;
    s32 i;
    s32 j;
    u8 *cell;
    ParQuadVertexColors *vertex;

    middleWord &= 0xFFFFFF;
    edgeWord &= 0xFFFFFF;
    if (count > 0) {
        i = count;
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(ParQuadVertexColors **)cell;
            middle = 0;
            edge = 0;
            for (j = 0; j < halfCount; j++, vertex++) {
                vertex->middle1 = middleWord | (middle & 0xFF000000);
                vertex->edge1 = edgeWord | (edge & 0xFF000000);
                vertex->middle0 = middleWord | (middle & 0xFF000000);
                vertex->edge0 = edgeWord | (edge & 0xFF000000);
                middle += middleStep;
                edge += edgeStep;
            }
            middle = middleAlpha;
            edge = edgeAlpha;
            for (j = 0; j < halfCount; j++, vertex++) {
                middle -= middleStep;
                edge -= edgeStep;
                vertex->middle1 = middleWord | (middle & 0xFF000000);
                vertex->edge1 = edgeWord | (edge & 0xFF000000);
                vertex->middle0 = middleWord | (middle & 0xFF000000);
                vertex->edge0 = edgeWord | (edge & 0xFF000000);
            }
            i--;
            cell += 0x14;
        } while (i != 0);
    }
}

void parFillSymmetricCellColors(ParSystem *system, s32 centerWord, s32 middleWord, s32 edgeWord) {
    s32 words = system->vertexWordCount;
    s32 count = system->cellCount;
    s32 perCell = words / 5;
    s32 i;
    s32 j;
    u8 *cell;
    u8 *vertex;
    if (count > 0) {
        i = count;
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(u8 **)cell;
            if (perCell > 0) {
                j = perCell;
                do {
                    j--;
                    ((ParSymmetricVertexColors *)vertex)->edge0 = edgeWord;
                    ((ParSymmetricVertexColors *)vertex)->middle0 = middleWord;
                    ((ParSymmetricVertexColors *)vertex)->center = centerWord;
                    ((ParSymmetricVertexColors *)vertex)->middle1 = middleWord;
                    ((ParSymmetricVertexColors *)vertex)->edge1 = edgeWord;
                    vertex += 0x14;
                } while (j != 0);
            }
            i--;
            cell += 0x14;
        } while (i != 0);
    }
}

/* Fade the three symmetric colors toward transparent across each cell. */
void parDecreaseSymmetricCellAlpha(ParSystem *system, u32 centerWord, u32 middleWord, u32 edgeWord)
{
    s32 words = system->vertexWordCount;
    s32 count = system->cellCount;
    u32 centerAlpha = centerWord & 0xFF000000;
    u32 middleAlpha = middleWord & 0xFF000000;
    u32 edgeAlpha = edgeWord & 0xFF000000;
    s32 perCell = words / 5;
    u32 centerStep = centerAlpha / perCell;
    u32 middleStep = middleAlpha / perCell;
    u32 edgeStep = edgeAlpha / perCell;
    u32 center;
    u32 middle;
    u32 edge;
    s32 i;
    s32 j;
    u8 *cell;
    ParSymmetricVertexColors *vertex;

    centerWord &= 0xFFFFFF;
    middleWord &= 0xFFFFFF;
    edgeWord &= 0xFFFFFF;
    if (count > 0) {
        i = count;
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(ParSymmetricVertexColors **)cell;
            center = centerAlpha;
            middle = middleAlpha;
            edge = edgeAlpha;
            for (j = 0; j < perCell; j++, vertex++) {
                center -= centerStep;
                middle -= middleStep;
                edge -= edgeStep;
                vertex->edge0 = edgeWord | (edge & 0xFF000000);
                vertex->middle0 = middleWord | (middle & 0xFF000000);
                vertex->center = centerWord | (center & 0xFF000000);
                vertex->middle1 = middleWord | (middle & 0xFF000000);
                vertex->edge1 = edgeWord | (edge & 0xFF000000);
            }
            i--;
            cell += 0x14;
        } while (i != 0);
    }
}

/* Increase each symmetric color's alpha from zero across the cell. */
void parIncreaseSymmetricCellAlpha(ParSystem *system, u32 centerWord, u32 middleWord, u32 edgeWord)
{
    s32 words = system->vertexWordCount;
    s32 count = system->cellCount;
    u32 centerAlpha = centerWord & 0xFF000000;
    u32 middleAlpha = middleWord & 0xFF000000;
    u32 edgeAlpha = edgeWord & 0xFF000000;
    s32 perCell = words / 5;
    u32 centerStep = centerAlpha / perCell;
    u32 middleStep = middleAlpha / perCell;
    u32 edgeStep = edgeAlpha / perCell;
    u32 center;
    u32 middle;
    u32 edge;
    s32 i;
    s32 j;
    u8 *cell;
    ParSymmetricVertexColors *vertex;

    centerWord &= 0xFFFFFF;
    middleWord &= 0xFFFFFF;
    edgeWord &= 0xFFFFFF;
    if (count > 0) {
        i = count;
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(ParSymmetricVertexColors **)cell;
            center = 0;
            middle = 0;
            edge = 0;
            for (j = 0; j < perCell; j++, vertex++) {
                center += centerStep;
                middle += middleStep;
                edge += edgeStep;
                vertex->edge0 = edgeWord | (edge & 0xFF000000);
                vertex->middle0 = middleWord | (middle & 0xFF000000);
                vertex->center = centerWord | (center & 0xFF000000);
                vertex->middle1 = middleWord | (middle & 0xFF000000);
                vertex->edge1 = edgeWord | (edge & 0xFF000000);
            }
            i--;
            cell += 0x14;
        } while (i != 0);
    }
}

/* Raise then lower the three alphas across the two halves of each cell. */
void parRiseFallSymmetricCellAlpha(ParSystem *system, u32 centerWord, u32 middleWord, u32 edgeWord)
{
    s32 words = system->vertexWordCount;
    s32 count = system->cellCount;
    u32 centerAlpha = centerWord & 0xFF000000;
    u32 middleAlpha = middleWord & 0xFF000000;
    u32 edgeAlpha = edgeWord & 0xFF000000;
    s32 half = (words / 5) >> 1;
    u32 centerStep = centerAlpha / half;
    u32 middleStep = middleAlpha / half;
    u32 edgeStep = edgeAlpha / half;
    u32 center;
    u32 middle;
    u32 edge;
    s32 i;
    s32 j;
    u8 *cell;
    ParSymmetricVertexColors *vertex;

    centerWord &= 0xFFFFFF;
    middleWord &= 0xFFFFFF;
    edgeWord &= 0xFFFFFF;
    if (count > 0) {
        i = count;
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(ParSymmetricVertexColors **)cell;
            center = 0;
            middle = 0;
            edge = 0;
            for (j = 0; j < half; j++, vertex++) {
                vertex->edge0 = edgeWord | (edge & 0xFF000000);
                vertex->middle0 = middleWord | (middle & 0xFF000000);
                vertex->center = centerWord | (center & 0xFF000000);
                vertex->middle1 = middleWord | (middle & 0xFF000000);
                vertex->edge1 = edgeWord | (edge & 0xFF000000);
                center += centerStep;
                middle += middleStep;
                edge += edgeStep;
            }
            center = centerAlpha;
            middle = middleAlpha;
            edge = edgeAlpha;
            for (j = 0; j < half; j++, vertex++) {
                center -= centerStep;
                middle -= middleStep;
                edge -= edgeStep;
                vertex->edge0 = edgeWord | (edge & 0xFF000000);
                vertex->middle0 = middleWord | (middle & 0xFF000000);
                vertex->center = centerWord | (center & 0xFF000000);
                vertex->middle1 = middleWord | (middle & 0xFF000000);
                vertex->edge1 = edgeWord | (edge & 0xFF000000);
            }
            i--;
            cell += 0x14;
        } while (i != 0);
    }
}

void parSetCellDrawBucket(ParSystem *system, u16 value) {
    system->bucket = value;
}

void parDispatchSub(void *work, s32 sub, void *a2, void *a3) {
    u16 id = *(u16 *)work;

    D_003AAF10[id * 3 + sub](work, a2, a3);
}

extern const u32 D_003AAC90[];
extern const u32 D_003AAD10[];
extern const u32 D_003AAE50[];
extern const u32 D_003AAD80[];
extern const u32 D_003AAE00[];
extern SdfPoolNode *D_003AAF50[];
extern SdfPoolNode D_00380248;

/* Batch pending cell systems by topology, then submit the five draw buckets. */
void parDrawPendingCellSystems(void) {
    SdfListHead *lists[5];
    SdfListHead **slot;
    SdfListHead *list;
    SdfListHead *specialList;
    s32 count;
    s32 i;
    s32 remaining;
    ParSystem *system;
    ParCell *cell;
    u64 *packet;

    if (D_00436404 == NULL) {
        return;
    }
    memset(lists, 0, sizeof(lists));
    for (system = D_00436404; system != NULL; system = system->next) {
        slot = &lists[system->bucket];
        list = *slot;
        if (list == 0) {
            *slot = (SdfListHead *)sdfAllocPacketAligned(0x20);
            sdfInitPacketList(*slot);
            sdfConsAppendClearPacket(*slot, NULL);
            list = *slot;
        }
        sdfConsAppendAssetPacket(list, (SdfAsset *)system->asset, NULL);
        count = system->cellCount;
        if (system->kind == PAR_CELL_TOPOLOGY_PAIR) {
            parDrawControl.indices = D_003AAC90;
            for (i = 0; i < count; i++) {
                cell = &system->cells[i];
                parDrawControl.width = 16;
                parDrawControl.height = 18;
                remaining = cell->vertexCount;
                parDrawControl.positions = cell->history;
                parDrawControl.colors = cell->colors;
                parDrawControl.color = cell->color;
                while (remaining >= 18) {
                    remaining -= 16;
                    sdfAppendPacket(list, (u32)effCreateVertexDrawPacket(&parDrawControl));
                    parDrawControl.positions += 16;
                    parDrawControl.colors += 16;
                }
                if (remaining >= 4) {
                    parDrawControl.width = remaining - 2;
                    parDrawControl.height = remaining;
                    sdfAppendPacket(list, (u32)effCreateVertexDrawPacket(&parDrawControl));
                }
            }
        } else if (system->kind == PAR_CELL_TOPOLOGY_TRIANGLE) {
            parDrawControl.indices = D_003AAD10;
            for (i = 0; i < count; i++) {
                cell = &system->cells[i];
                parDrawControl.width = 16;
                parDrawControl.height = 15;
                remaining = cell->vertexCount;
                parDrawControl.positions = cell->history;
                parDrawControl.colors = cell->colors;
                parDrawControl.color = cell->color;
                while (remaining >= 15) {
                    remaining -= 12;
                    sdfAppendPacket(list, (u32)effCreateVertexDrawPacket(&parDrawControl));
                    parDrawControl.positions += 12;
                    parDrawControl.colors += 12;
                }
                if (remaining >= 6) {
                    parDrawControl.width = (remaining / 3) * 4 - 4;
                    parDrawControl.height = remaining;
                    sdfAppendPacket(list, (u32)effCreateVertexDrawPacket(&parDrawControl));
                }
            }
        } else if (system->kind == PAR_CELL_TOPOLOGY_FIVE_VECTOR) {
            parDrawControl.indices = D_003AAE50;
            for (i = 0; i < count; i++) {
                cell = &system->cells[i];
                parDrawControl.width = 8;
                parDrawControl.height = 10;
                remaining = cell->vertexCount;
                parDrawControl.positions = cell->history;
                parDrawControl.colors = cell->colors;
                parDrawControl.color = cell->color;
                while (remaining >= 10) {
                    remaining -= 5;
                    sdfAppendPacket(list, (u32)effCreateVertexDrawPacket(&parDrawControl));
                    parDrawControl.positions += 5;
                    parDrawControl.colors += 5;
                }
            }
        } else if (system->kind == PAR_CELL_TOPOLOGY_SIX_VECTOR) {
            parDrawControl.indices = D_003AAD80;
            for (i = 0; i < count; i++) {
                cell = &system->cells[i];
                parDrawControl.width = 10;
                parDrawControl.height = 12;
                remaining = cell->vertexCount;
                parDrawControl.positions = cell->history;
                parDrawControl.colors = cell->colors;
                parDrawControl.color = cell->color;
                while (remaining >= 12) {
                    remaining -= 6;
                    sdfAppendPacket(list, (u32)effCreateVertexDrawPacket(&parDrawControl));
                    parDrawControl.positions += 6;
                    parDrawControl.colors += 6;
                }
            }
        } else {
            parDrawControl.indices = D_003AAE00;
            for (i = 0; i < count; i++) {
                cell = &system->cells[i];
                parDrawControl.width = 12;
                parDrawControl.height = 16;
                remaining = cell->vertexCount;
                parDrawControl.positions = cell->history;
                parDrawControl.colors = cell->colors;
                parDrawControl.color = cell->color;
                while (remaining >= 16) {
                    remaining -= 12;
                    sdfAppendPacket(list, (u32)effCreateVertexDrawPacket(&parDrawControl));
                    parDrawControl.positions += 12;
                    parDrawControl.colors += 12;
                }
            }
        }
    }
    for (i = 0; i < 4; i++) {
        if (lists[i] != 0) {
            D_003AAF50[i]->append(D_003AAF50[i], lists[i]);
        }
    }
    if (lists[4] != 0) {
        specialList = (SdfListHead *)sdfAllocPacketAligned(0x20);
        sdfInitPacketList(specialList);
        packet = (u64 *)sdfAllocPacketAligned(0x30);
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[4] = 0x8000000026ULL;
        packet[5] = 0x42;
        sdfAppendPacket(specialList, (u32)packet);
        D_00380248.append(&D_00380248, specialList);
        packet = (u64 *)sdfAllocPacketAligned(0x30);
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[4] = 0x42;
        packet[5] = 0x42;
        sdfAppendPacket(lists[4], (u32)packet);
        D_00380248.append(&D_00380248, lists[4]);
    }
    D_00436404 = NULL;
}

ParBlock *parAllocateDrawBlock(s32 count) {
    s32 points = count * 3;
    s32 colorBytes = points * 4;
    struct SdfMemBlock *allocation = sdfAllocGeneralBlock((colorBytes + points) * 4 + 0x18);
    s32 base = sdfResourceRetainAddress(allocation);
    u8 *vertices = (u8 *)base + points * 16;
    ParBlock *block = (ParBlock *)(vertices + colorBytes);
    block->color = 0x80808080;
    block->count = count;
    block->colors = (u32 *)vertices;
    block->allocation = allocation;
    block->positions = (u128 *)base;
    block->asset = sdfCreateAssetWithDrawEntries();
    sdfSetPrimaryStateFloat(block->asset, 1.0f);
    return block;
}

void parReleaseDrawBlock(ParBlock *block) {
    sdfQueueAssetRelease(block->asset);
    sdfReleaseResourceAllocation(block->allocation);
}

void parSubmitCellDrawPackets(SdfPoolNode *emitter, ParBlock *cmd) {
    SdfListHead *list = (SdfListHead *)sdfAllocPacketAligned(0x20);
    EffVertexDrawRequest state;
    s32 remaining;
    sdfInitPacketList(list);
    sdfConsAppendClearPacket(list, 0);
    remaining = cmd->count * 3;
    memset(&state, 0, sizeof(state));
    state.width = 0x10;
    state.height = 0x30;
    state.flags = 0x4000;
    state.positions = cmd->positions;
    state.colors = cmd->colors;
    state.color = cmd->color;
    while (remaining >= 0x30) {
        remaining -= 0x30;
        sdfAppendPacket(list, (u32)effCreateVertexDrawPacket(&state));
    }
    if (remaining > 0) {
        u16 *counts = (u16 *)&parDrawControl;
        counts[0] = remaining / 3;
        counts[1] = remaining;
        sdfAppendPacket(list, (u32)effCreateVertexDrawPacket(&state));
    }
    emitter->append(emitter, list);
}

ParEmitDesc *parCloneEmitterAndInitCells(ParEmitDesc *src) {
    ParEmitDesc *desc = sdfAllocSizeClassBlock(src->count * 4 + 0xF0);

    memset(desc, 0, 0xF0);
    memcpy(desc, src, src->headerSize);
    memcpy(&desc->cells, (u8 *)src + src->headerSize, sizeof(desc->cells));
    desc->headerSize = 0xC0;
    desc->cells.cellWords = (u32 *)(desc + 1);
    if (desc->cells.verticesPerCell < 3) {
        desc->cells.verticesPerCell = 3;
    }
    desc->cells.cellSystem = parAllocateCellSystem(desc->count, desc->cells.verticesPerCell, 1, PAR_CELL_TOPOLOGY_PAIR);
    parDispatchSub(desc->cells.cellSystem, 0,
                   *(void **)((u8 *)&desc->cells + 0x14),
                   *(void **)((u8 *)&desc->cells + 0x18));
    polyInitializeBasicRingCells(desc);
    return desc;
}

INCLUDE_SDATA(const s32, "game/code_00162348", parRecordListHead);

INCLUDE_SDATA(const s32, "game/code_00162348", D_00436404);

