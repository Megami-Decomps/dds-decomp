#include "sdk_ps2.h"
#include "common.h"
#include "eff_packet_params.h"
#include "eff_random.h"
#include "sdf_asset_state.h"
#include "sdf_asset_create.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "sdf_packet_append.h"
#include "par_cell_api.h"
#include "sdf_heap_api.h"
#include "pcp_vu0.h"
#include "sdf_vu_matrix.h"
#include "ee_mmi.h"
#include "eff.h"
#include "eff_scatter_draw.h"
#include "eff_pcp_scatter_rings.h"
#include "sdf_packet_alloc.h"
#include "sdf_vu0.h"
#include "eff_param.h"
#include "btl_effect_position.h"
#include "sdf_trig.h"
#include "sdf_quaternion.h"
#include "sdf_vu_transform_api.h"
#include "eff_matrix4.h"
#include "eff_math.h"
#include "sdf_projection.h"
#include "bill_object_api.h"
#include "eff_billboard_resource_work.h"
#include "eff_resource_render_state.h"

extern EffRandState effDefaultRandomState[];

/* effPCPNeedleSpiralCreate */
EffPCPNeedleWork *effPCPNeedleSpiralCreate(EffPCPNeedleParams *params) {
    sdfMemHandle_t *allocation = sdfMemAllocDirect(params->count * sizeof(EffPCPNeedleSlot) +
                                                  sizeof(EffPCPNeedleWork));
    EffPCPNeedleWork *work = (EffPCPNeedleWork *)sdfMemLock(allocation);
    u32 i;
    EffResourceWork *resource;
    u32 count;
    u32 delayRange;
    EffPCPNeedleSlot *slot;
    f32 radiusBase;
    f32 radiusJitter;

    work->slots = (EffPCPNeedleSlot *)(work + 1);
    work->params = *params;
    work->allocationHandle = allocation;
    work->count = params->count;
    work->color = 0x80808080;
    work->system = 0;
    resource = dds3NeedleModelCreate(params->count);
    resource->mode = params->mode;
    work->resource = resource;
    effBuildRadialFanStreams(resource, params->fanSegments, params->centerColor,
                            params->outerColor, params->radiusScale, params->viewOffset);
    work->system = dds3TrackPolyCreate(work->count, work->params.perCellInput, 0, PAR_CELL_TOPOLOGY_PAIR);
    dds3TrackPolyVCSet(work->system, 1, work->params.unk58, work->params.unk58);
    dds3TrackPolySetMode(work->system, (u16)work->params.mode);

    count = work->count;
    delayRange = params->randomDelayRange;
    slot = work->slots;
    if (delayRange == 0) {
        delayRange = 1;
    }
    radiusBase = work->params.radiusBase;
    radiusJitter = work->params.radiusJitter;
    for (i = 0; i < count; i++, slot++) {
        slot->angle = dds3GetRandFloatA(effDefaultRandomState) * EFFECT_RING_FULL_TURN;
        slot->radius = radiusBase * (dds3GetRandFloatA(effDefaultRandomState) *
                                   radiusJitter + (1.0f - radiusJitter));
        slot->age = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % delayRange);
        dds3NeedleModelSetColor(work->resource, i, 0x808080);
    }
    return work;
}

extern u32 effBTLFieldColorUsesActorSelectors(void);

extern u32 effBTLFieldColorGetOriginalSelector(void);

extern f32 aViewTarget[4];

extern f32 sdfViewTargetVector[4];

extern f32 D_003B1530[4];

extern void dds3NeedleModelSetMatrix(Matrix4 *dst, const Matrix4 *src);

void effPCPNeedleSpiralDelete(EffPCPNeedleWork *work) {
    dds3TrackPolyDelete(work->system);
    dds3NeedleModelDelete(work->resource);
    sdfMemFreeDirect(work->allocationHandle);
}

EffPCPNeedleWork *effPCPNeedleSpiralCreatePCF(const EffParamTable *parameters) {
    EffPCPNeedleParams *work;

    work = dds3EffectPCPDataGetData(parameters, 0);
    return effPCPNeedleSpiralCreate(work);
}

/* Recreate needle work directly from the parameter prefix used by duplication. */
EffPCPNeedleWork *effPCPNeedleSpiralCopy(EffPCPNeedleParams *parameters) {
    return effPCPNeedleSpiralCreate(parameters);
}

/* Advance the orbiting needle entries and rebuild their camera-facing trails. */
void effPCPNeedleSpiralDraw(EffPCPNeedleWork *work) {
    f32 position[4];
    f32 axis[4];
    f32 vertices[2][4];
    f32 resourcePosition[4];
    f32 viewDirection[4];
    f32 width[4];
    f32 viewOffset[4];
    EffPCPNeedleSlot *slot;
    s32 fadeIn;
    u32 count;
    u32 i;
    s32 age;
    u32 baseColor;
    u32 color;
    f32 t;
    f32 speed;
    f32 acceleration;
    f32 angleStep;
    f32 value;
    u32 radius;
    s32 duration;
    s32 fadeOut;

    value = work->params.width;
    width[0] = value;
    width[1] = value;
    width[2] = value;
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(sdfViewTargetVector);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(viewDirection);
    value = -work->params.viewOffset * 0.75f;
    viewOffset[0] = value;
    viewOffset[1] = value;
    viewOffset[2] = value;

    if (effBTLFieldColorUsesActorSelectors() != 0) {
        btlUnitGetMuzzlePosVU((struct BtlUnit *)effBTLFieldColorGetOriginalSelector());
        SDF_VU0_STOREVECTOR_NOMEM(position);
        LookAtMatrix(work->params.position, position, D_003B1530);
        ReverseMatrix();
        VU0_STORE_MATRIX_UNCLOBBERED(work->resource->matrix);
    } else {
        VU0_LOAD_MATRIX(work->resource->matrix);
    }

    axis[0] = 0.0f;
    axis[2] = 1.0f;
    axis[1] = 0.0f;
    SDF_VU0_LOADVECTOR(axis);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR_NOMEM(axis);
    SDF_VU0_LOADVECTOR_B(viewOffset);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(viewOffset);

    count = work->count;
    i = 0;
    slot = work->slots;
    speed = work->params.speed;
    acceleration = work->params.acceleration;
    duration = work->params.duration;
    fadeIn = work->params.fadeIn;
    fadeOut = work->params.fadeOut;
    angleStep = work->params.angleStep;
    baseColor = work->color;
    if (count != 0) {
        do {
            age = slot->age;
            if (age == 0) {
                VU0_LOAD_MATRIX(work->resource->matrix);
                radius = (u32)slot->radius;
                position[0] = sdfCos(slot->angle) * radius;
                position[1] = sdfSin(slot->angle) * radius;
                position[2] = 0.0f;
                SDF_VU0_LOADVECTOR(position);
                SDF_VU0_APPLYMATRIX_XYZ();
                SDF_VU0_STOREVECTOR_NOMEM(position);
                slot->direction[0] = position[0];
                slot->direction[1] = position[1];
                slot->direction[2] = position[2];
                dds3NeedleModelSetPosition(work->resource, i, position);
            }

            if (age >= 0 && age <= duration) {
                dds3NeedleModelGetPosition(work->resource, i, resourcePosition);
                SDF_VU0_LOADVECTOR(viewOffset);
                SDF_VU0_LOADVECTOR_B(resourcePosition);
                SDF_VU0_ADDVECTOR();
                SDF_VU0_STOREVECTOR_NOMEM(resourcePosition);

                position[0] = slot->direction[0];
                position[1] = slot->direction[1];
                position[2] = slot->direction[2];
                SetRotateMatrixAxis(slot->angle, (const RwV3d *)axis);
                slot->angle += angleStep;
                SDF_VU0_LOADVECTOR(position);
                SDF_VU0_APPLYMATRIX_XYZ();
                SDF_VU0_STOREVECTOR_NOMEM(position);

                {
                    f32 elapsed = (f32)slot->age;
                    f32 halfAcceleration = acceleration * 0.5f;
                    f32 distance = speed * elapsed + halfAcceleration * elapsed * elapsed;

                    elapsed = (f32)duration;
                    distance -= speed * elapsed + halfAcceleration * elapsed * elapsed;
                    position[0] += axis[0] * distance;
                    position[1] += axis[1] * distance;
                    position[2] += axis[2] * distance;
                }
                SDF_VU0_LOADVECTOR(position);
                SDF_VU0_LOADVECTOR_B(work->params.position);
                SDF_VU0_ADDVECTOR();
                SDF_VU0_STOREVECTOR_NOMEM(position);
                dds3NeedleModelSetPosition(work->resource, i, position);

                if (age < fadeIn && fadeIn != 0) {
                    t = (f32)age / (f32)fadeIn;
                } else if (duration == age && fadeOut != 0) {
                    t = 0.0f;
                } else {
                    t = fadeOut < duration - age || fadeOut == 0
                        ? 1.0f : (f32)(duration - age) / (f32)fadeOut;
                }
                color = effColorBlend(baseColor & 0xFFFFFF, baseColor, t);
                dds3NeedleModelSetColor(work->resource, i, color);

                if (age > 0) {
                    SDF_VU0_LOADVECTOR(viewOffset);
                    SDF_VU0_LOADVECTOR_B(position);
                    SDF_VU0_ADDVECTOR();
                    SDF_VU0_COPYVECTOR_A_TO_C();
                    SDF_VU0_LOADVECTOR(resourcePosition);
                    SDF_VU0_COPYVECTOR_C_TO_B();
                    SDF_VU0_SUBVECTOR();
                    SDF_VU0_LOADVECTOR_B(viewDirection);
                    SDF_VU0_OUTERPRODUCT();
                    SDF_VU0_NORMALIZE();
                    SDF_VU0_LOADVECTOR_B(width);
                    SDF_VU0_MULVECTOR();
                    VU0_MOVE_VF(vf2, vf10);
                    SDF_VU0_COPYVECTOR_C_TO_A();
                    VU0_MOVE_VF(vf12, vf2);
                    SDF_VU0_COPYVECTOR_A_TO_B();
                    SDF_VU0_ADDVECTOR_A_C();
                    SDF_VU0_STOREVECTOR_NOMEM(vertices[0]);
                    SDF_VU0_COPYVECTOR_C_TO_A();
                    SDF_VU0_SUBVECTOR_B_A();
                    SDF_VU0_STOREVECTOR_B_NOMEM(vertices[1]);
                    dds3TrackPolySet(work->system, i, (const u128 *)vertices);
                    dds3TrackPolyVCRefresh_ParType1(work->system, i);
                    dds3TrackPolyColorSet(work->system, i, (color & 0xFF000000) | 0x808080);
                }
            } else {
                dds3NeedleModelSetColor(work->resource, i, 0);
                dds3TrackPolyColorSet(work->system, i, 0);
                dds3TrackPolyListInit(work->system, i);
            }
            slot->age++;
            i++;
            slot++;
        } while (i < count);
    }
    dds3TrackPolyDraw(work->system);
    dds3NeedleModelDraw(work->resource);
}

void effPCPNeedleSpiralSetPosition(u128 *dst, const u128 *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

/* Copy the supplied transform into the needle's resource matrix. */
void effNeedleSetResourceMatrix(EffPCPNeedleWork *work, const Matrix4 *matrix) {
    dds3NeedleModelSetMatrix((Matrix4 *)work->resource->matrix, matrix);
}

extern EffResourceRenderState effResourceDrawParams;

void effPCPNeedleSpiralSetColor(EffResourceWork *work, f32 (*normals)[4]) {
    work->normals = normals;
}

extern void *effPkt3DPrimMakePoly(EffResourceRenderState *);

extern f32 effResourceDefaultTrianglePoints[][4];

extern u32 effResourceDefaultTriangleColors[];

extern sdfGrOt_t *effResourceDrawSurfaces[];

/* Create an effect resource work with count entries inline after its header. */
EffResourceWork *dds3NeedleModelCreate(u32 count) {
    struct sdfMemHandle_t *allocation;
    EffResourceWork *work;
    EffResourceEntry *entries;
    sdfMat_t *asset;
    u32 i;

    allocation = sdfMemAllocDirect(count * sizeof(EffResourceEntry) + sizeof(EffResourceWork));
    work = (EffResourceWork *)sdfMemLock(allocation);
    work->mode = 2;
    work->entries = (EffResourceEntry *)(work + 1);
    work->backingAllocation = allocation;
    work->entryCount = count;
    work->scale[0] = 1.0f;
    work->scale[1] = 1.0f;
    work->scale[2] = 1.0f;
    work->streamAllocation = NULL;
    EE_MMI_UNIT_MATRIX(work);
    asset = sdfMatNew();
    work->drawAsset = asset;
    sdfMatSetConstant(asset, 1.0f);
    entries = work->entries;
    i = 0;
    if (count != 0) {
        do {
            i++;
            entries->color = 0x80808080;
            entries++;
        } while (i < count);
    }
    memset(&effResourceDrawParams, 0, sizeof(effResourceDrawParams));
    effResourceDrawParams.primitive = 0x4000;
    return work;
}

void dds3NeedleModelDelete(EffResourceWork *effect) {
    sdfMatFreeDelayed(effect->drawAsset);
    effReleaseOptionalResource(effect);
    sdfMemFreeDirect(effect->backingAllocation);
}

/* Emit scaled, translated triangle batches with separate vector and packed-color streams. */
void dds3NeedleModelDraw(EffResourceWork *work) {
    f32 matrix[16] __attribute__((aligned(16)));
    sdfDrawTag_t *packet;
    EffResourceEntry *entry;
    u32 i;
    u32 remaining;
    u32 triangleSize;

    packet = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(packet);
    sdf3DPrimLinkMat(packet, work->drawAsset, 0);
    EE_MMI_UNIT_MATRIX(matrix);
    matrix[0] = work->scale[0];
    matrix[5] = work->scale[1];
    matrix[10] = work->scale[2];
    VU0_LOAD_MATRIX(matrix);
    SDF_VU0_LOADMATRIX_B(work->matrix);
    MultMatrixNoArg();
    VU0_STORE_MATRIX(matrix);
    effResourceDrawParams.parameters = NULL;
    entry = work->entries;
    for (i = 0; i < work->entryCount; i++, entry++) {
        triangleSize = 3;
        matrix[12] = entry->position[0];
        matrix[13] = entry->position[1];
        matrix[14] = entry->position[2];
        VU0_LOAD_MATRIX(matrix);
        sdf3DPrimLinkTm(packet, 0);
        if (work->streamAllocation == NULL) {
            remaining = 12;
            effResourceDrawParams.colors = effResourceDefaultTriangleColors;
            effResourceDrawParams.positions = effResourceDefaultTrianglePoints;
            effResourceDrawParams.normals = NULL;
        } else {
            effResourceDrawParams.positions = work->positions;
            effResourceDrawParams.colors = work->colors;
            effResourceDrawParams.normals = work->normals;
            remaining = work->vertexCount;
        }
        effResourceDrawParams.color = entry->color;
        effResourceDrawParams.parameterCount = 16;
        effResourceDrawParams.vertexCount = 48;
        while (remaining >= 48) {
            sdfDrawTagLink(packet, (u32)effPkt3DPrimMakePoly(&effResourceDrawParams));
            if (work->streamAllocation == NULL) {
                effResourceDrawParams.colors += 48;
                effResourceDrawParams.positions += 48;
            } else {
                effResourceDrawParams.colors += 48;
                effResourceDrawParams.positions += 48;
                effResourceDrawParams.normals += 48;
            }
            remaining -= 48;
        }
        if (remaining >= 3) {
            effResourceDrawParams.parameterCount = remaining / triangleSize;
            effResourceDrawParams.vertexCount = remaining;
            sdfDrawTagLink(packet, (u32)effPkt3DPrimMakePoly(&effResourceDrawParams));
        }
    }
    effResourceDrawSurfaces[work->mode]->append(effResourceDrawSurfaces[work->mode], packet);
}

void effBuildRadialFanStreams(EffResourceWork *work, u32 count, u32 centerColor, u32 outerColor,
                   f32 radiusScale, f32 height) {
    struct sdfMemHandle_t *oldResource = work->streamAllocation;
    u32 recordCount;
    struct sdfMemHandle_t *allocation;
    f32 (*positions)[4];
    f32 (*unitVectors)[4];
    u32 *colors;
    u32 i;
    f32 angle;
    f32 step;
    f32 halfStep;
    f32 middleAngle;
    f32 negativeHeight;

    if (count < 3) {
        count = 3;
    }
    if (oldResource != NULL) {
        effReleaseOptionalResource(work);
    }
    angle = 0.0f;
    recordCount = count * 3;
    allocation = sdfMemAllocDirect(recordCount * 0x24);
    work->streamAllocation = allocation;
    positions = (f32 (*)[4])sdfMemLock(allocation);
    unitVectors = positions + recordCount;
    colors = (u32 *)(unitVectors + recordCount);
    work->vertexCount = recordCount;
    work->positions = positions;
    work->normals = unitVectors;
    work->colors = colors;
    step = 6.2831852f / (f32)count;
    for (i = 0; i < count; i++) {
        f32 sine;

        negativeHeight = -height;
        unitVectors[0][0] = sdfCos(angle);
        unitVectors[0][1] = sdfSin(angle);
        unitVectors[0][2] = 0.0f;
        halfStep = step * 0.5f;
        middleAngle = angle + halfStep;
        angle += step;
        positions[0][0] = unitVectors[0][0] * radiusScale;
        sine = unitVectors[0][1];
        unitVectors++;
        positions[0][2] = negativeHeight;
        positions[0][1] = sine * radiusScale;
        positions++;
        unitVectors[0][0] =
            sdfCos(middleAngle);
        unitVectors[0][1] = sdfSin(middleAngle);
        unitVectors[0][2] = 0.0f;
        positions[0][0] = 0.0f;
        positions[0][1] = 0.0f;
        positions[0][2] = 0.0f;
        unitVectors++;
        positions++;
        unitVectors[0][0] = sdfCos(angle);
        unitVectors[0][1] = sdfSin(angle);
        unitVectors[0][2] = 0.0f;
        positions[0][0] = unitVectors[0][0] * radiusScale;
        positions[0][1] = unitVectors[0][1] * radiusScale;
        positions[0][2] = negativeHeight;
        colors[0] = outerColor;
        colors[1] = centerColor;
        colors[2] = outerColor;
        colors += 3;
        positions++;
        unitVectors++;
    }
}

void effReleaseOptionalResource(EffResourceWork *effect) {
    if (effect->streamAllocation != NULL) {
        sdfMemFreeDirect(effect->streamAllocation);
        return;
    }
}

void dds3NeedleModelSetPosition(EffResourceWork *effect, s32 index, f32 *vec) {
    f32 *dst = (f32 *)(index * sizeof(EffResourceEntry) + (s32)effect->entries);
    dst[0] = vec[0];
    dst[1] = vec[1];
    dst[2] = vec[2];
}

void dds3NeedleModelGetPosition(EffResourceWork *effect, s32 index, f32 *vec) {
    f32 *src = (f32 *)(index * sizeof(EffResourceEntry) + (s32)effect->entries);
    vec[0] = src[0];
    vec[1] = src[1];
    vec[2] = src[2];
}

void dds3NeedleModelSetColor(EffResourceWork *effect, s32 index, u32 color) {
    effect->entries[index].color = color;
}

/* vu0 routine: copy a 0x40-byte matrix from src to dst through vf28-vf31 */
void dds3NeedleModelSetMatrix(Matrix4 *dst, const Matrix4 *src) {
    VU0_LOAD_MATRIX(src);
    VU0_STORE_MATRIX(dst);
}
