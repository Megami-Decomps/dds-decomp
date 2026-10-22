#include "common.h"
#include "eff_param.h"
#include "btl_effect_position.h"
#include "sdf_trig.h"
#include "sdf_quaternion.h"
#include "par_cell_api.h"
#include "sdf_resource.h"
#include "pcp_vu0.h"
#include "sdf_vu_transform_api.h"
#include "eff.h"
#include "eff_matrix4.h"
#include "eff_math.h"
#include "sdf_projection.h"

extern u32 effBTLFieldColorUsesActorSelectors(void);
extern u32 effBTLFieldColorGetOriginalSelector(void);
extern void dds3TrackPolyColorSet(u32 system, s32 index, u32 value);

extern f32 aViewTarget[4];
extern f32 sdfViewTargetVector[4];
extern f32 D_003B1530[4];


extern void dds3NeedleModelSetMatrix(Matrix4 *dst, const Matrix4 *src);

void effPCPNeedleSpiralDelete(EffPCPNeedleWork *work) {
    dds3TrackPolyDelete(work->system);
    dds3NeedleModelDelete(work->resource);
    sdfMemFreeDirect(work->allocationHandle);
}

EffPCPNeedleWork *effPCPNeedleSpiralCreatePCF(void *parameters) {
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
    VU0_LOAD_VF(vf10, aViewTarget);
    VU0_LOAD_VF(vf11, sdfViewTargetVector);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF_UNCLOBBERED(vf10, viewDirection);
    value = -work->params.viewOffset * 0.75f;
    viewOffset[0] = value;
    viewOffset[1] = value;
    viewOffset[2] = value;

    if (effBTLFieldColorUsesActorSelectors() != 0) {
        btlUnitGetMuzzlePosVU((struct BtlUnit *)effBTLFieldColorGetOriginalSelector());
        VU0_STORE_VF_UNCLOBBERED(vf10, position);
        LookAtMatrix(work->params.position, position, D_003B1530);
        ReverseMatrix();
        VU0_STORE_MATRIX_UNCLOBBERED(work->resource->matrix);
    } else {
        VU0_LOAD_MATRIX(work->resource->matrix);
    }

    axis[0] = 0.0f;
    axis[2] = 1.0f;
    axis[1] = 0.0f;
    VU0_LOAD_VF(vf10, axis);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF_UNCLOBBERED(vf10, axis);
    VU0_LOAD_VF(vf11, viewOffset);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF_UNCLOBBERED(vf10, viewOffset);

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
                VU0_LOAD_VF(vf10, position);
                VU0_ROTATE_VEC(vf10, vf10);
                VU0_STORE_VF_UNCLOBBERED(vf10, position);
                slot->direction[0] = position[0];
                slot->direction[1] = position[1];
                slot->direction[2] = position[2];
                dds3NeedleModelSetPosition(work->resource, i, position);
            }

            if (age >= 0 && age <= duration) {
                dds3NeedleModelGetPosition(work->resource, i, resourcePosition);
                VU0_LOAD_VF(vf10, viewOffset);
                VU0_LOAD_VF(vf11, resourcePosition);
                VU0_ADD(vf10, vf10, vf11);
                VU0_STORE_VF_UNCLOBBERED(vf10, resourcePosition);

                position[0] = slot->direction[0];
                position[1] = slot->direction[1];
                position[2] = slot->direction[2];
                SetRotateMatrixAxis(slot->angle, (const RwV3d *)axis);
                slot->angle += angleStep;
                VU0_LOAD_VF(vf10, position);
                VU0_ROTATE_VEC(vf10, vf10);
                VU0_STORE_VF_UNCLOBBERED(vf10, position);

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
                VU0_LOAD_VF(vf10, position);
                VU0_LOAD_VF(vf11, work->params.position);
                VU0_ADD(vf10, vf10, vf11);
                VU0_STORE_VF_UNCLOBBERED(vf10, position);
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
                    VU0_LOAD_VF(vf10, viewOffset);
                    VU0_LOAD_VF(vf11, position);
                    VU0_ADD(vf10, vf10, vf11);
                    VU0_MOVE_VF(vf12, vf10);
                    VU0_LOAD_VF(vf10, resourcePosition);
                    VU0_MOVE_VF(vf11, vf12);
                    VU0_SUB(vf10, vf10, vf11);
                    VU0_LOAD_VF(vf11, viewDirection);
                    VU0_CROSS_XYZ(vf10, vf10, vf11);
                    VU0_NORMALIZE_VF10();
                    VU0_LOAD_VF(vf11, width);
                    VU0_MUL(vf10, vf10, vf11);
                    VU0_MOVE_VF(vf2, vf10);
                    VU0_MOVE_VF(vf10, vf12);
                    VU0_MOVE_VF(vf12, vf2);
                    VU0_MOVE_VF(vf11, vf10);
                    VU0_ADD(vf10, vf10, vf12);
                    VU0_STORE_VF_UNCLOBBERED(vf10, vertices[0]);
                    VU0_MOVE_VF(vf10, vf12);
                    VU0_SUB(vf11, vf11, vf10);
                    VU0_STORE_VF_UNCLOBBERED(vf11, vertices[1]);
                    dds3TrackPolySet(work->system, i, (const u128 *)vertices);
                    dds3TrackPolyVCRefresh_ParType1(work->system, i);
                    dds3TrackPolyColorSet((u32)work->system, i, (color & 0xFF000000) | 0x808080);
                }
            } else {
                dds3NeedleModelSetColor(work->resource, i, 0);
                dds3TrackPolyColorSet((u32)work->system, i, 0);
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


void effPCPNeedleSpiralSetPosition(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

/* Copy the supplied transform into the needle's resource matrix. */
void effNeedleSetResourceMatrix(EffPCPNeedleWork *work, void *matrix) {
    dds3NeedleModelSetMatrix((Matrix4 *)work->resource->matrix, matrix);
}
