/* Keeps its own declaration of the functions below (see sdk_libc.h note). */
#define SDK_SKIP_memcpy
#include "sdk_ps2.h"
#include "common.h"
#include "sdf_vu0.h"
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
#include "eff_packet_params.h"
#include "par_kind_api.h"
#include "par_kind_resource.h"
#include "par_table.h"
#include "par_draw_block.h"
#include "par_emitter_desc.h"
#include "par_object_client_view.h"

#include "eff.h"
#include "par_dispatch_api.h"
#include "eff_track_poly_list.h"
#include "eff_matrix4.h"

#include "pcp_vu0.h"
#include "ee_mmi.h"






extern f32 parLook[];

extern f32 parTrack_w[];

extern u8 aViewTarget[];

extern u8 sdfViewTargetVector[];

extern f32 D_003AAEC0[];

extern f32 D_003AAEE0[];

extern f32 D_003AAEF0[];

extern f32 D_003AAF00[];

extern dds3Polygon_t *hDrawListTop;

extern ParSystem *D_00436404;

extern void (*D_003AAF10[])(ParSystem *, u32, u32);


extern u16 dds3ParticleGetState(ParObj *obj);



extern void dds3TrackPolySet2(ParSystem *, s32, const u128 *);

extern u8 dds3ParticleGetLoop(ParObj *object);


extern void dds3Particle_TrackSet(ParSystem *system, s32 index, u32 color);

extern void dds3Particle_Track2Set(ParSystem *system, s32 index, u32 color);

extern void dds3TrackPolyVCRefresh_ParType2(ParSystem *system, s32 cellIndex);


#include "sdf_packet_alloc.h"

struct sdfDrawTag_t;





extern void *memcpy(void *dst, void *src, u32 n);
extern void dds3SpPoly_Ring_Old_BuffInit(void *);

extern void dds3Particle_BlurClearItem(s32, s32);





void dds3ParticleSetRGBA(ParObj *work, u32 parameterWord) {
    work->parameterWord = parameterWord;
}

void dds3ParticleSetTypeFlag(ParObj *object, u8 mode) {
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

u8 dds3ParticleGetLoop(ParObj *object) {
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









ParObj *dds3ParticleExpCreate(s32 kind, ParKindResource *resource) {
    ParObj *object = (ParObj *)((u8 *)resource + resource->offset + 0x10);
    billHandle_t *billboard;

    if (resource->type != 3 || resource->offset != 0) {
        object->billboardCloneMarker = PAR_BILLBOARD_CLONE_FROM_RESOURCE;
        object = dds3ParticleFuncTable[kind].createOrCopy(object);
        billboard = dds3BillboardCreate(resource->type, (u32)(resource + 1));
        dds3BillboardSetScale(billboard, object->billboardScale, object->billboardScaleY);
        dds3BillboardSetMode(billboard, object->billboardMode);
        dds3BillboardSetAnimColorAdvMode(billboard);
        object->billboard = billboard;
    } else {
        object = dds3ParticleFuncTable[kind].createOrCopy(object);
    }
    object->dispatchIndex = kind;
    return object;
}

ParObj *dds3ParticleExpCopy(ParObj *source) {
    ParObj *particle;
    billHandle_t *billboard;

    particle = dds3ParticleFuncTable[source->dispatchIndex].createOrCopy(source);
    particle->dispatchIndex = source->dispatchIndex;
    if (source->billboardCloneMarker == PAR_BILLBOARD_CLONE_FROM_RESOURCE) {
        billboard = dds3BillboardCopy(source->billboard);
        dds3BillboardSetScale(billboard, particle->billboardScale, particle->billboardScaleY);
        dds3BillboardSetMode(billboard, particle->billboardMode);
        dds3BillboardSetAnimColorAdvMode(billboard);
        particle->billboard = billboard;
    }
    return particle;
}

void dds3ParticleExpDelete(ParObj *object) {
    dds3ParticleFuncTable[object->dispatchIndex].destroy(object);
}

extern void dds3BillboardDraw(billHandle_t *);

/* Apply queued updates, then draw live records and submit the kind's resource. */
void dds3ParticleExpDraw(ParObj *effect) {
    EffectBufferRecord *record;
    billHandle_t *billboard;
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
        dds3ParticleFuncTable[effect->dispatchIndex].update(effect);
    } else {
        for (step = 0; step < effect->pendingRestartSteps; step++) {
            dds3ParticleFuncTable[effect->dispatchIndex].update(effect);
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
                SDF_VU0_COPYVECTOR(billboard, record->position);
                billboard->rotationAngle = record->spin;
                billboard->childParam = record->color;
                billboard->childScaleX = billboard->childScaleY = record->scale;
                record->scale += scaleStep;
                if (record->scale < 0) {
                    record->scale = 0;
                }
                record->spin += spinStep * spinDirection;
                dds3BillboardSetAnimKey(billboard, age);
                dds3BillboardDraw(billboard);
            }
            spinDirection *= alternate;
            --step;
            ++record;
        } while (step != 0);
    }
    parSubmitKindDrawing(&effect->kindState);
}

extern void dds3ParticleReset(ParObj *obj);
extern void dds3ParticleSetScale(ParObj *obj, f32 factor);

void dds3ParticleExpReset(ParObj *obj) {
    dds3ParticleReset(obj);
}

void dds3ParticleExpSetScale(float scale, ParObj *work) {
    dds3ParticleSetScale(work, scale);
    work->scaleEnd = work->scaleEnd * scale;
}

/* Descriptor query: return the complete 16-bit restart state. */
u16 dds3ParticleExpGetState(ParObj *obj) {
    return dds3ParticleGetState(obj);
}

void dds3ParticleExpSetPosition(void *dst, void *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

extern void dds3ParticleSetRotateMatrix(ParObj *effect, const Matrix4 *matrix);

void dds3ParticleExpSetRotateMatrix(ParObj *effect, const Matrix4 *matrix) {
    dds3ParticleSetRotateMatrix(effect, matrix);
}

void dds3ParticleExpSetRGBA(ParObj *work, u32 parameterWord) {
    work->parameterWord = parameterWord;
}

void parChangeInstanceMode(ParObj *work, u8 mode) {
    dds3ParticleSetTypeFlag(work, mode);
}

/* Descriptor query: select the kind's current mode byte. */
u8 dds3ParticleExpGetLoop(ParObj *work) {
    return dds3ParticleGetLoop(work);
}

/* Kinds 2-4 keep the scale at +8 of their own record; copy it into the
 * shared vector and store the (vf10 - vf11) difference. */
void dds3Particle_EffectInit(ParKindState *obj) {
    f32 scale;

    switch (obj->kind) {
    case 0:
    case PAR_KIND_STATE_HISTORY:
        return;
    case PAR_KIND_STATE_BILLBOARD_CROSS_STRIP:
        scale = obj->payload.crossStrip.scale;
        parTrack_w[0] = parTrack_w[1] = parTrack_w[2] = scale;
        break;
    case PAR_KIND_STATE_BILLBOARD_CROSS_TRIANGLE:
        scale = obj->payload.crossTriangle.scale;
        parTrack_w[0] = parTrack_w[1] = parTrack_w[2] = scale;
        break;
    case PAR_KIND_STATE_TRACK_POLYGON_CROSS_AXES:
        scale = obj->payload.trackPolygon.scale;
        parTrack_w[0] = parTrack_w[1] = parTrack_w[2] = scale;
        break;
    default:
        return;
    }
    VU0_LOAD_VF($vf10, aViewTarget);
    VU0_LOAD_VF($vf11, sdfViewTargetVector);
    SDF_VU0_SUBVECTOR();
    VU0_STORE_VF($vf10, parLook);
}

void parDispatchKindUpdate(ParKindState *work, s32 index, u32 color, f32 speed) {
    switch ((u16)work->kind) {
    case 1:
        dds3Particle_BlurSet(work->payload.history.table, index, color, speed);
        return;
    case 2:
        dds3Particle_TrackSet(work->payload.crossStrip.drawSystem, index, color);
        return;
    case 3:
        dds3Particle_Track2Set(work->payload.crossTriangle.drawSystem, index, color);
        return;
    case 4:
        dds3Particle_Track3Set(work->payload.trackPolygon.modelList, index, color);
        break;
    }
}

void parDispatchKindInit(ParKindState *work, s32 index) {
    switch ((u16)work->kind) {
    case 1:
        dds3Particle_BlurClearItem((s32)work->payload.history.table, index);
        return;
    case 2:
        dds3TrackPolyListInit(work->payload.crossStrip.drawSystem, index);
        return;
    case 3:
        dds3TrackPolyListInit(work->payload.crossTriangle.drawSystem, index);
        return;
    case 4:
        Eff_ModelTrackPolyListReset(work->payload.trackPolygon.modelList, index);
        break;
    }
}

void dds3Particle_TrackSet(ParSystem *system, s32 index, u32 color) {
    u128 axis[2];
    SDF_VU0_COPYVECTOR_C_TO_B();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(parLook);
;
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(parTrack_w);
;
    SDF_VU0_MULVECTOR();
    VU0_MOVE_VF(vf2, vf10);
    SDF_VU0_COPYVECTOR_C_TO_A();
    VU0_MOVE_VF(vf12, vf2);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(&axis[0]);
;
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_SUBVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B(&axis[1]);
;
    dds3TrackPolySet(system, index, axis);
    dds3TrackPolyVCRefresh_ParType1(system, index);
    dds3TrackPolyColorSet(system, index, (color & 0xFF000000) | 0x808080);
}

void dds3Particle_Track2Set(ParSystem *system, s32 index, u32 color) {
    u128 axis[3];
    SDF_VU0_COPYVECTOR_C_TO_B();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(parLook);
;
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(parTrack_w);
;
    SDF_VU0_MULVECTOR();
    VU0_MOVE_VF(vf2, vf10);
    SDF_VU0_COPYVECTOR_C_TO_A();
    VU0_MOVE_VF(vf12, vf2);
    SDF_VU0_STOREVECTOR(&axis[1]);
;
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(&axis[0]);
;
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_SUBVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B(&axis[2]);
;
    dds3TrackPolySet2(system, index, axis);
    dds3TrackPolyVCRefresh_ParType2(system, index);
    dds3TrackPolyColorSet(system, index, (color & 0xFF000000) | 0x808080);
}

void dds3Particle_Track3Set(effMTPolyListHandle_t *list, s32 index, u32 color) {
    u128 axis[2];
    SDF_VU0_COPYVECTOR_C_TO_B();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(parLook);
;
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(parTrack_w);
;
    SDF_VU0_MULVECTOR();
    VU0_MOVE_VF(vf2, vf10);
    SDF_VU0_COPYVECTOR_C_TO_A();
    VU0_MOVE_VF(vf12, vf2);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(&axis[0]);
;
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_SUBVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B(&axis[1]);
;
    Eff_ModelTrackPolyListVertexSet(list, index, axis);
    effTrackPolySetIndexedColor(list, index, (color & 0xFF000000) | 0x808080);
}

u32 dds3Particle_TrackPolyGetRecTiming(u16 pointCapacity) {
    return 0;
}

extern EffRandState D_00451F50;


void dds3TrackPolyInit(void);

void dds3PolygonManagerInit(void) {
    hDrawListTop = 0;
    dds3TrackPolyInit();
    sdfRandomizeByTime(&D_00451F50);
}

void dds3PolygonManagerExit(void) {
}

dds3Polygon_t *dds3PolygonCreate(s32 count, s16 flags, s16 options) {
    s32 vertexCount;
    s32 bytes;
    struct sdfMemHandle_t *allocation;
    u8 *buffer;
    dds3Polygon_t *record;
    struct sdfMat_t *asset;

    if (flags & 8) vertexCount = count + 2;
    else vertexCount = count * 3;
    bytes = vertexCount * sizeof(u128);
    if (flags & 1) bytes += vertexCount * sizeof(record->texcoords[0]);
    if (flags & 2) bytes += vertexCount * sizeof(u128);
    if (flags & 4) bytes += vertexCount * sizeof(u32);
    allocation = sdfMemAllocDirect(bytes + sizeof(*record));
    buffer = (u8 *)sdfMemLock(allocation);
    record = (dds3Polygon_t *)(buffer + bytes);
    memset(record, 0, sizeof(*record));
    record->positions = (u128 *)buffer;
    /* Allocation and layout reuse one byte extent; qualified cc1probe:
     * sandbox_ClusterX/ctor_reentry_F4_probe/decisions.log:16561-16660.
     * The saved span ties allocation at 1250, below vertex count at 3500;
     * a separate span instead has priority 3846 and takes the count's home. */
    bytes = vertexCount * sizeof(u128);
    buffer += bytes;
    if (flags & 1) {
        record->texcoords = (f32 (*)[2])buffer;
        buffer += vertexCount * sizeof(record->texcoords[0]);
    }
    if (flags & 2) record->normals = (u128 *)buffer;
    if (flags & 4) record->colors = (u32 *)buffer;
    record->flags = flags;
    record->capacity = vertexCount;
    record->allocation = allocation;
    record->drawCount = 0;
    memset(&record->request, 0, sizeof(record->request));
    record->request.color = 0x80808080;
    if (!(options & 1)) record->request.primitive = 0x4000;
    asset = sdfMatNew();
    record->asset = asset;
    if (!(options & 2)) sdfMatSetConstant(asset, 1.0f);
    return record;
}

void dds3PolygonDelete(dds3Polygon_t *record) {
    record->flags = 1;
    sdfMatFreeDelayed(record->asset);
    sdfMemFreeDirect(record->allocation);
}

void dds3PolygonDraw(dds3Polygon_t *node) {
    node->next = hDrawListTop;
    hDrawListTop = node;
}

extern SdfPoolNode sdfGrOtTable[0x62];

static inline void parAppendDrawSurfacePool(SdfPoolNode *pool, sdfDrawTag_t *packet) {
    pool->append(pool, packet);
}

extern void *effPkt3DPrimMakePoly(EffPacketParams *);

void dds3PolygonLinkOt(void) {
    dds3Polygon_t *record;
    sdfDrawTag_t *packet;
    s32 remaining;
    s16 flags;
    SdfPoolNode *priorityPool;
    record = hDrawListTop;
    if (record != NULL) {
        priorityPool = &sdfGrOtTable[51];
        do {
            if (record->drawCount != 0 && record->flags != 1) {
                packet = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
                sdfDrawTagInit(packet);
                sdf3DPrimLinkResetTm(packet, 0);
                sdf3DPrimLinkMat(packet, record->asset, 0);
                flags = record->flags;
                remaining = record->drawCount;
                record->request.positions = record->positions;
                record->request.texcoords = record->texcoords[0];
                record->request.colors = record->colors;
                record->request.normals = record->normals;
                record->request.parameterCount = 16;
                record->request.vertexCount = 48;
                while (remaining >= 48) {
                    sdfDrawTagLink(packet, (u32)effPkt3DPrimMakePoly(&record->request));
                    record->request.positions += 48;
                    if (flags & 1) record->request.texcoords += 96;
                    if (flags & 2) record->request.normals += 48;
                    remaining -= 48;
                    if (flags & 4) record->request.colors += 48;
                }
                if (remaining > 0) {
                    record->request.parameterCount = remaining / 3;
                    record->request.vertexCount = remaining;
                    sdfDrawTagLink(packet, (u32)effPkt3DPrimMakePoly(&record->request));
                }
                record->drawCount = 0;
                if (flags & 0x10) parAppendDrawSurfacePool(priorityPool, packet);
                else if (flags & 0x20) parAppendDrawSurfacePool(&sdfGrOtTable[55], packet);
                else parAppendDrawSurfacePool(&sdfGrOtTable[39], packet);
            }
            record = record->next;
        } while (record != NULL);
    }
    hDrawListTop = NULL;
}

/* Replace the record asset's primary texture and dirty its draw state. */
void dds3PolygonSetTex(dds3Polygon_t *record, sdfTexHandle_t *texture) {
    sdfSetAssetPrimaryTextureAddress(record->asset, texture);
}

/* Draw parameter block filled per strip by dds3TrackPolyLinkOt. */
extern EffPacketParams parDrawControl;

void dds3TrackPolyInit(void) {
    memset(&parDrawControl, 0, 0x2C);
    parDrawControl.primitive = 0x4000;
}

ParSystem *dds3TrackPolyCreate(s32 count, s32 perCell, s32 groupDivisor, u32 kind) {
    s32 total;
    struct sdfMemHandle_t *allocation;
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
    allocation = sdfMemAllocDirect(cellsSize + 0x2C);
    base = sdfMemLock(allocation);
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
        dds3TrackPolyListInit(system, i);
    }
    system->asset = sdfMatNew();
    sdfMatSetConstant(system->asset, 1.0f);
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

void dds3TrackPolyDelete(ParSystem *system) {
    sdfMatFreeDelayed(system->asset);
    sdfMemFreeDirect(system->allocation);
}

void dds3TrackPolyListInit(ParSystem *system, s32 cellIndex) {
    ParCell *cell = (ParCell *)(cellIndex * sizeof(ParCell) + (s32)system->cells);

    cell->color = 0x80808080;
    cell->historyAdvanceCountdown = 0;
    cell->vertexCount = 0;
}

void dds3TrackPolyDraw(ParSystem *node) {
    node->next = D_00436404;
    D_00436404 = node;
}

void dds3TrackPolySet(ParSystem *system, s32 cellIndex, const u128 *vertices) {
    ParCell *cell = &system->cells[cellIndex];
    u128 *vertex;
    s32 shiftCount;
    s32 i;

    if (cell->historyAdvanceCountdown == 0) {
        shiftCount = system->vertexWordCount - 2;
        vertex = cell->history + shiftCount;
        for (i = 0; i < shiftCount; i++) {
            vertex--;
            SDF_VU0_COPYVECTOR(vertex + 2, vertex);
        }
        cell->historyAdvanceCountdown = system->groupDivisor;
        if (cell->vertexCount < shiftCount + 2) {
            cell->vertexCount += 2;
        }
    } else {
        cell->historyAdvanceCountdown--;
        vertex = cell->history;
    }
    SDF_VU0_COPYVECTOR(vertex, vertices);
    SDF_VU0_COPYVECTOR(vertex + 1, vertices + 1);
}

void dds3TrackPolyMove(ParSystem *system, s32 cellIndex, const u128 *delta) {
    ParCell *cell = system->cells + cellIndex;
    s32 count = system->vertexWordCount;
    u128 *vertex = cell->history;
    s32 i;
    SDF_VU0_LOADVECTOR_B_BARRIER(delta);
    if (count > 0) {
        i = count;
        do {
            SDF_VU0_LOADVECTOR(vertex);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(vertex);
            i--;
            vertex++;
        } while (i != 0);
    }
}

void dds3TrackPolySet2(ParSystem *system, s32 cellIndex, const u128 *vertices) {
    ParCell *cell = &system->cells[cellIndex];
    u128 *vertex;
    s32 shiftCount;
    s32 i;

    if (cell->historyAdvanceCountdown == 0) {
        shiftCount = system->vertexWordCount - 3;
        vertex = cell->history + shiftCount;
        for (i = 0; i < shiftCount; i++) {
            vertex--;
            SDF_VU0_COPYVECTOR(vertex + 3, vertex);
        }
        cell->historyAdvanceCountdown = system->groupDivisor;
        if (cell->vertexCount < shiftCount + 3) {
            cell->vertexCount += 3;
        }
    } else {
        cell->historyAdvanceCountdown--;
        vertex = cell->history;
    }
    SDF_VU0_COPYVECTOR(vertex, vertices);
    SDF_VU0_COPYVECTOR(vertex + 1, vertices + 1);
    SDF_VU0_COPYVECTOR(vertex + 2, vertices + 2);
}

void dds3TrackPolyMove2(ParSystem *system, s32 cellIndex, const u128 *delta) {
    ParCell *cell = system->cells + cellIndex;
    s32 count = cell->vertexCount / 3;
    u128 *vertex = cell->history;
    s32 i;
    SDF_VU0_LOADVECTOR_B(delta);
    if (count > 0) {
        i = count;
        do {
            SDF_VU0_LOADVECTOR(vertex);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(vertex);
            SDF_VU0_LOADVECTOR(vertex + 1);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(vertex + 1);
            SDF_VU0_LOADVECTOR(vertex + 2);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(vertex + 2);
            i--;
            vertex += 3;
        } while (i != 0);
    }
}

void dds3TrackPolyVCRefresh_ParType1(ParSystem *system, s32 cellIndex) {
    u32 count = system->cells[cellIndex].vertexCount >> 1;
    u32 *vertex = system->cells[cellIndex].colors;
    u32 word = vertex[0];
    u32 i;
    s32 alpha[4];
    s32 start[4];
    s32 out[4];
    u32 packed;
    if (count >= 2) {
        alpha[0] = word & 0xFF000000;
        EE_MMI_RGBA_UNPACK(alpha, 1.0f / 128.0f);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(D_003AAEC0);
        VU0_LERP_VF10(1.0f / count);
        SDF_VU0_COPYVECTOR_A_TO_C();
        start[0] = word;
        EE_MMI_RGBA_UNPACK(start, 1.0f / 128.0f);
        for (i = 0; i < count; i++) {
            SDF_VU0_COPYVECTOR_A_TO_B();
            EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
            out[0] = packed;
            vertex[0] = packed;
            vertex[1] = packed;
            SDF_VU0_SUBVECTOR_B_C();
            SDF_VU0_COPYVECTOR_B_TO_A();
            vertex += 2;
        }
    }
}

extern f32 D_003AAED0[4];
/* Fade one triangle cell's middle and edge colors toward transparent along its history. */
void dds3TrackPolyVCRefresh_ParType2(ParSystem *system, s32 cellIndex) {
    u32 count = system->cells[cellIndex].vertexCount / 3;
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
        vertex = (ParTriangleVertexColors *)system->cells[cellIndex].colors;
        middleWord = vertex->middle;
        edgeWord = vertex->edge0;
        alpha0[0] = middleWord & 0xFF000000;
        EE_MMI_RGBA_UNPACK(alpha0, 1.0f / 128.0f);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(D_003AAED0);
        VU0_LERP_VF10(1.0f / count);
        SDF_VU0_STOREVECTOR(step0);
        alpha1[0] = edgeWord & 0xFF000000;
        EE_MMI_RGBA_UNPACK(alpha1, 1.0f / 128.0f);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(D_003AAED0);
        VU0_LERP_VF10(1.0f / count);
        SDF_VU0_STOREVECTOR_NOMEM(step1);
        col0[0] = middleWord;
        EE_MMI_RGBA_UNPACK(col0, 1.0f / 128.0f);
        SDF_VU0_STOREVECTOR_NOMEM(cur0);
        col1[0] = edgeWord;
        EE_MMI_RGBA_UNPACK(col1, 1.0f / 128.0f);
        SDF_VU0_STOREVECTOR_NOMEM(cur1);
        for (j = 0; j < count; j++) {
            SDF_VU0_LOADVECTOR(cur0);
            SDF_VU0_COPYVECTOR_A_TO_B();
            EE_MMI_RGBA_PACK_UNIT(packedMiddle, 128.0f);
            out0[0] = packedMiddle;
            vertex->middle = packedMiddle;
            SDF_VU0_LOADVECTOR_C(step0);
            SDF_VU0_SUBVECTOR_B_C();
            SDF_VU0_STOREVECTOR_B(cur0);
            SDF_VU0_LOADVECTOR(cur1);
            SDF_VU0_COPYVECTOR_A_TO_B();
            EE_MMI_RGBA_PACK_UNIT(packedEdge, 128.0f);
            out1[0] = packedEdge;
            vertex->edge0 = packedEdge;
            vertex->edge1 = packedEdge;
            SDF_VU0_LOADVECTOR_C(step1);
            SDF_VU0_SUBVECTOR_B_C();
            SDF_VU0_STOREVECTOR_B(cur1);
            vertex++;
        }
    }
}

void dds3TrackPolyVCSet_Normal(ParSystem *system, u32 firstColor, u32 secondColor) {
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

void dds3TrackPolyVCSet_Acute(ParSystem *system, u32 color) {
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
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(D_003AAEE0);
    VU0_LERP_VF10(1.0f / perCell);
    SDF_VU0_COPYVECTOR_A_TO_C();
    i = 0;
    if (count != 0) {
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(u32 **)cell;
            start[0] = color;
            EE_MMI_RGBA_UNPACK(start, 1.0f / 128.0f);
            for (j = 0; j < perCell; j++) {
                SDF_VU0_COPYVECTOR_A_TO_B();
                EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
                out[0] = packed;
                vertex[0] = packed;
                vertex[1] = packed;
                SDF_VU0_SUBVECTOR_B_C();
                SDF_VU0_COPYVECTOR_B_TO_A();
                vertex += 2;
            }
            i++;
            cell += 0x14;
        } while (i < count);
    }
}

void dds3TrackPolyVCSet_AcuteW(ParSystem *system, u32 color) {
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
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(D_003AAEF0);
    VU0_LERP_VF10(1.0f / quarter);
    SDF_VU0_COPYVECTOR_A_TO_C();
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
                SDF_VU0_COPYVECTOR_A_TO_B();
                EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
                rise[0] = packed;
                vertex[0] = packed;
                vertex[1] = packed;
                SDF_VU0_ADDVECTOR_B_C();
                SDF_VU0_COPYVECTOR_B_TO_A();
                vertex += 2;
            }
            if (odd) {
                vertex[0] = color;
                vertex[1] = color;
                vertex += 2;
            }
            for (j = 0; j < quarter; j++) {
                SDF_VU0_COPYVECTOR_A_TO_B();
                EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
                fall[0] = packed;
                vertex[0] = packed;
                vertex[1] = packed;
                SDF_VU0_SUBVECTOR_B_C();
                SDF_VU0_COPYVECTOR_B_TO_A();
                vertex += 2;
            }
            i++;
            cell += 0x14;
        } while (i < count);
    }
}

void dds3TrackPolyVCSet2_Normal(ParSystem *system, s32 middleWord, s32 edgeWord) {
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

void dds3TrackPolyVCSet2_Acute(ParSystem *system, u32 middleWord, u32 edgeWord) {
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
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(D_003AAF00);
    VU0_LERP_VF10(1.0f / perCell);
    SDF_VU0_STOREVECTOR(step0);
    alpha1[0] = edgeWord & 0xFF000000;
    EE_MMI_RGBA_UNPACK(alpha1, 1.0f / 128.0f);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(D_003AAF00);
    VU0_LERP_VF10(1.0f / perCell);
    SDF_VU0_STOREVECTOR(step1);
    i = 0;
    if (count != 0) {
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(u32 **)cell;
            col0[0] = middleWord;
            EE_MMI_RGBA_UNPACK(col0, 1.0f / 128.0f);
            SDF_VU0_STOREVECTOR(cur0);
            col1[0] = edgeWord;
            EE_MMI_RGBA_UNPACK(col1, 1.0f / 128.0f);
            SDF_VU0_STOREVECTOR(cur1);
            for (j = 0; j < perCell; j++) {
                SDF_VU0_LOADVECTOR(cur0);
                SDF_VU0_COPYVECTOR_A_TO_B();
                EE_MMI_RGBA_PACK_UNIT(packedMiddle, 128.0f);
                out0[0] = packedMiddle;
                vertex[1] = packedMiddle;
                SDF_VU0_LOADVECTOR_C(step0);
                SDF_VU0_SUBVECTOR_B_C();
                SDF_VU0_STOREVECTOR_B(cur0);
                SDF_VU0_LOADVECTOR(cur1);
                SDF_VU0_COPYVECTOR_A_TO_B();
                EE_MMI_RGBA_PACK_UNIT(packedEdge, 128.0f);
                out1[0] = packedEdge;
                vertex[0] = packedEdge;
                vertex[2] = packedEdge;
                SDF_VU0_LOADVECTOR_C(step1);
                SDF_VU0_SUBVECTOR_B_C();
                SDF_VU0_STOREVECTOR_B(cur1);
                vertex += 3;
            }
            i++;
            cell += 0x14;
        } while (i < count);
    }
}

void dds3TrackPolyVCSet2_AcuteW(ParSystem *system, u32 middleWord, u32 edgeWord) {
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

void dds3TrackPolyVC3Set_Normal(ParSystem *system, s32 centerWord, s32 middleWord, s32 edgeWord) {
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
void dds3TrackPolyVC3Set_Acute(ParSystem *system, u32 centerWord, u32 middleWord, u32 edgeWord)
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
void dds3TrackPolyVC3Set_AcuteW(ParSystem *system, u32 centerWord, u32 middleWord, u32 edgeWord)
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

void dds3TrackPolyVCSet4_Normal(ParSystem *system, u32 firstColor, u32 secondColor) {
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
void dds3TrackPolyVCSet4_Acute(ParSystem *system, u32 middleWord, u32 edgeWord) {
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
void dds3TrackPolyVCSet4_AcuteW(ParSystem *system, u32 middleWord, u32 edgeWord) {
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

void dds3TrackPolyVC5Set_Normal(ParSystem *system, s32 centerWord, s32 middleWord, s32 edgeWord) {
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
void dds3TrackPolyVC5Set_Acute(ParSystem *system, u32 centerWord, u32 middleWord, u32 edgeWord)
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
void dds3TrackPolyVC5Set_AcuteR(ParSystem *system, u32 centerWord, u32 middleWord, u32 edgeWord)
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
void dds3TrackPolyVC5Set_AcuteW(ParSystem *system, u32 centerWord, u32 middleWord, u32 edgeWord)
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

void dds3TrackPolySetMode(ParSystem *system, u16 bucket) {
    system->bucket = bucket;
}

void dds3TrackPolyVCSet(ParSystem *system, s32 sub, u32 colorWordA, u32 colorWordB) {
    u16 id = system->kind;

    D_003AAF10[id * 3 + sub](system, colorWordA, colorWordB);
}

extern const u32 D_003AAC90[];
extern const u32 D_003AAD10[];
extern const u32 D_003AAE50[];
extern const u32 D_003AAD80[];
extern const u32 D_003AAE00[];
extern SdfPoolNode *D_003AAF50[];


/* Batch pending cell systems by topology, then submit the five draw buckets. */
void dds3TrackPolyLinkOt(void) {
    sdfDrawTag_t *lists[5];
    sdfDrawTag_t **slot;
    sdfDrawTag_t *list;
    sdfDrawTag_t *specialList;
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
            *slot = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
            sdfDrawTagInit(*slot);
            sdf3DPrimLinkResetTm(*slot, NULL);
            list = *slot;
        }
        sdf3DPrimLinkMat(list, (sdfMat_t *)system->asset, NULL);
        count = system->cellCount;
        if (system->kind == PAR_CELL_TOPOLOGY_PAIR) {
            parDrawControl.parameters = D_003AAC90;
            for (i = 0; i < count; i++) {
                cell = &system->cells[i];
                parDrawControl.parameterCount = 16;
                parDrawControl.vertexCount = 18;
                remaining = cell->vertexCount;
                parDrawControl.positions = cell->history;
                parDrawControl.colors = cell->colors;
                parDrawControl.color = cell->color;
                while (remaining >= 18) {
                    remaining -= 16;
                    sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(&parDrawControl));
                    parDrawControl.positions += 16;
                    parDrawControl.colors += 16;
                }
                if (remaining >= 4) {
                    parDrawControl.parameterCount = remaining - 2;
                    parDrawControl.vertexCount = remaining;
                    sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(&parDrawControl));
                }
            }
        } else if (system->kind == PAR_CELL_TOPOLOGY_TRIANGLE) {
            parDrawControl.parameters = D_003AAD10;
            for (i = 0; i < count; i++) {
                cell = &system->cells[i];
                parDrawControl.parameterCount = 16;
                parDrawControl.vertexCount = 15;
                remaining = cell->vertexCount;
                parDrawControl.positions = cell->history;
                parDrawControl.colors = cell->colors;
                parDrawControl.color = cell->color;
                while (remaining >= 15) {
                    remaining -= 12;
                    sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(&parDrawControl));
                    parDrawControl.positions += 12;
                    parDrawControl.colors += 12;
                }
                if (remaining >= 6) {
                    parDrawControl.parameterCount = (remaining / 3) * 4 - 4;
                    parDrawControl.vertexCount = remaining;
                    sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(&parDrawControl));
                }
            }
        } else if (system->kind == PAR_CELL_TOPOLOGY_FIVE_VECTOR) {
            parDrawControl.parameters = D_003AAE50;
            for (i = 0; i < count; i++) {
                cell = &system->cells[i];
                parDrawControl.parameterCount = 8;
                parDrawControl.vertexCount = 10;
                remaining = cell->vertexCount;
                parDrawControl.positions = cell->history;
                parDrawControl.colors = cell->colors;
                parDrawControl.color = cell->color;
                while (remaining >= 10) {
                    remaining -= 5;
                    sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(&parDrawControl));
                    parDrawControl.positions += 5;
                    parDrawControl.colors += 5;
                }
            }
        } else if (system->kind == PAR_CELL_TOPOLOGY_SIX_VECTOR) {
            parDrawControl.parameters = D_003AAD80;
            for (i = 0; i < count; i++) {
                cell = &system->cells[i];
                parDrawControl.parameterCount = 10;
                parDrawControl.vertexCount = 12;
                remaining = cell->vertexCount;
                parDrawControl.positions = cell->history;
                parDrawControl.colors = cell->colors;
                parDrawControl.color = cell->color;
                while (remaining >= 12) {
                    remaining -= 6;
                    sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(&parDrawControl));
                    parDrawControl.positions += 6;
                    parDrawControl.colors += 6;
                }
            }
        } else {
            parDrawControl.parameters = D_003AAE00;
            for (i = 0; i < count; i++) {
                cell = &system->cells[i];
                parDrawControl.parameterCount = 12;
                parDrawControl.vertexCount = 16;
                remaining = cell->vertexCount;
                parDrawControl.positions = cell->history;
                parDrawControl.colors = cell->colors;
                parDrawControl.color = cell->color;
                while (remaining >= 16) {
                    remaining -= 12;
                    sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(&parDrawControl));
                    parDrawControl.positions += 12;
                    parDrawControl.colors += 12;
                }
            }
        }
    }
    for (i = 0; i < 4; i++) {
        if (lists[i] != 0) {
            parAppendDrawSurfacePool(D_003AAF50[i], lists[i]);
        }
    }
    if (lists[4] != 0) {
        specialList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(specialList);
        packet = (u64 *)sdfGrAllocPacketBuff(0x30);
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[4] = 0x8000000026ULL;
        packet[5] = 0x42;
        sdfDrawTagLink(specialList, (u32)packet);
        parAppendDrawSurfacePool(&sdfGrOtTable[56], specialList);
        packet = (u64 *)sdfGrAllocPacketBuff(0x30);
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[4] = 0x42;
        packet[5] = 0x42;
        sdfDrawTagLink(lists[4], (u32)packet);
        parAppendDrawSurfacePool(&sdfGrOtTable[56], lists[4]);
    }
    D_00436404 = NULL;
}

ParBlock *dds3TrianglePolyCreate(s32 count) {
    s32 points = count * 3;
    s32 colorBytes = points * 4;
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect((colorBytes + points) * 4 + 0x18);
    u8 *base = (u8 *)sdfMemLock(allocation);
    u8 *vertices = base + points * 16;
    ParBlock *block = (ParBlock *)(vertices + colorBytes);
    block->color = 0x80808080;
    block->count = count;
    block->colors = (u32 *)vertices;
    block->allocation = allocation;
    block->positions = (u128 *)base;
    block->asset = sdfMatNew();
    sdfMatSetConstant(block->asset, 1.0f);
    return block;
}

void dds3TrianglePolyDelete(ParBlock *block) {
    sdfMatFreeDelayed(block->asset);
    sdfMemFreeDirect(block->allocation);
}

void dds3TrianglePolyLinkOt(SdfPoolNode *emitter, ParBlock *cmd) {
    sdfDrawTag_t *list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    EffPacketParams state;
    s32 remaining;
    sdfDrawTagInit(list);
    sdf3DPrimLinkResetTm(list, 0);
    remaining = cmd->count * 3;
    memset(&state, 0, sizeof(state));
    state.parameterCount = 0x10;
    state.vertexCount = 0x30;
    state.primitive = 0x4000;
    state.positions = cmd->positions;
    state.colors = cmd->colors;
    state.color = cmd->color;
    while (remaining >= 0x30) {
        remaining -= 0x30;
        sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(&state));
    }
    if (remaining > 0) {
        parDrawControl.parameterCount = remaining / 3;
        parDrawControl.vertexCount = remaining;
        sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(&state));
    }
    parAppendDrawSurfacePool(emitter, list);
}

ParEmitDesc *dds3SpPoly_Ring_Old_Create(ParEmitDesc *src) {
    ParEmitDesc *desc = sdfChipAllocDirect(src->count * 4 + 0xF0);

    memset(desc, 0, 0xF0);
    memcpy(desc, src, src->headerSize);
    memcpy(&desc->cells, (u8 *)src + src->headerSize, sizeof(desc->cells));
    desc->headerSize = 0xC0;
    desc->cells.cellWords = (u32 *)(desc + 1);
    if (desc->cells.verticesPerCell < 3) {
        desc->cells.verticesPerCell = 3;
    }
    desc->cells.cellSystem = dds3TrackPolyCreate(desc->count, desc->cells.verticesPerCell, 1, PAR_CELL_TOPOLOGY_PAIR);
    dds3TrackPolyVCSet(desc->cells.cellSystem, 0,
                   *(u32 *)((u8 *)&desc->cells + 0x14),
                   *(u32 *)((u8 *)&desc->cells + 0x18));
    dds3SpPoly_Ring_Old_BuffInit(desc);
    return desc;
}

INCLUDE_SDATA(const s32, "game/code_00162348", hDrawListTop);

INCLUDE_SDATA(const s32, "game/code_00162348", D_00436404);
