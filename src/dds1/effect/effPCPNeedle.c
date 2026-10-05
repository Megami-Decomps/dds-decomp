#include "common.h"
#include "pcp_vu0.h"
#include "eff.h"

/* Packed effect parameter-set accessor (see game/code_001624D0). */
extern void *effParamTableGetBlock(void *data, s32 index);

extern u32 func_001619E8(void);
extern u32 effBTLFieldColorGetOriginalSelector(void);
extern void btlUnitGetMuzzlePosVU(u32 unit);
extern void sdfVuBuildLookAtBasis(void *origin, void *target, void *up);
extern void sdfInvertRigidVuTransform(void);
extern void sdfBuildVuRotationFromAxisAngle(f32 *axis, f32 angle);
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32 angle);
extern f32 sdfSinPoly(f32 angle);
extern u32 effBlendColor(u32 colorA, u32 colorB, f32 blend);
extern void parUpdateCellVertexPair(u32 system, s32 index, f32 vertices[2][4]);
extern void parFadeAlphaCell(u32 system, s32 index);
extern void effBillSetEntryValue(u32 system, s32 index, u32 value);
extern void parCellInit(u32 system, s32 index);
extern void parPrependCellNode(u32 system);

extern f32 sdfViewEyeVector[4];
extern f32 sdfViewTargetVector[4];
extern f32 D_00354C00[4];

extern void parReleaseCellSystem(u32 handle);
extern void func_001770F8(void *dst, void *src);
extern void sdfReleaseResourceAllocation(u32 handle);


void effPCPNeedleFree(EffPCPNeedleWork *work) {
    parReleaseCellSystem(work->system);
    effReleaseAttachedResources(work->resource);
    sdfReleaseResourceAllocation(work->allocationHandle);
}

/* The first parameter block supplies the effect's runtime work. */
void effPCPNeedleCreate(void *data) {
    EffPCPNeedleParams *work;

    work = effParamTableGetBlock(data, 0);
    func_001760F8(work);
}

void func_00176410(EffPCPNeedleParams *work) {
    func_001760F8(work);
}

/* Advance the orbiting needle entries and rebuild their camera-facing trails. */
void func_00176428(EffPCPNeedleWork *work) {
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
    VU0_LOAD_VF(vf10, sdfViewEyeVector);
    VU0_LOAD_VF(vf11, sdfViewTargetVector);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF_UNCLOBBERED(vf10, viewDirection);
    value = -work->params.viewOffset * 0.75f;
    viewOffset[0] = value;
    viewOffset[1] = value;
    viewOffset[2] = value;

    if (func_001619E8() != 0) {
        btlUnitGetMuzzlePosVU(effBTLFieldColorGetOriginalSelector());
        VU0_STORE_VF_UNCLOBBERED(vf10, position);
        sdfVuBuildLookAtBasis(work->params.position, position, D_00354C00);
        sdfInvertRigidVuTransform();
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
                position[0] = sdfEvaluateCosineViaSinePhaseShift(slot->angle) * radius;
                position[1] = sdfSinPoly(slot->angle) * radius;
                position[2] = 0.0f;
                VU0_LOAD_VF(vf10, position);
                VU0_ROTATE_VEC(vf10, vf10);
                VU0_STORE_VF_UNCLOBBERED(vf10, position);
                slot->direction[0] = position[0];
                slot->direction[1] = position[1];
                slot->direction[2] = position[2];
                effSetResourceEntryPosition(work->resource, i, position);
            }

            if (age >= 0 && age <= duration) {
                effGetResourceEntryPosition(work->resource, i, resourcePosition);
                VU0_LOAD_VF(vf10, viewOffset);
                VU0_LOAD_VF(vf11, resourcePosition);
                VU0_ADD(vf10, vf10, vf11);
                VU0_STORE_VF_UNCLOBBERED(vf10, resourcePosition);

                position[0] = slot->direction[0];
                position[1] = slot->direction[1];
                position[2] = slot->direction[2];
                sdfBuildVuRotationFromAxisAngle(axis, slot->angle);
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
                effSetResourceEntryPosition(work->resource, i, position);

                if (age < fadeIn && fadeIn != 0) {
                    t = (f32)age / (f32)fadeIn;
                } else if (duration == age && fadeOut != 0) {
                    t = 0.0f;
                } else {
                    t = fadeOut < duration - age || fadeOut == 0
                        ? 1.0f : (f32)(duration - age) / (f32)fadeOut;
                }
                color = effBlendColor(baseColor & 0xFFFFFF, baseColor, t);
                effSetResourceEntryValue(work->resource, i, color);

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
                    parUpdateCellVertexPair(work->system, i, vertices);
                    parFadeAlphaCell(work->system, i);
                    effBillSetEntryValue(work->system, i, (color & 0xFF000000) | 0x808080);
                }
            } else {
                effSetResourceEntryValue(work->resource, i, 0);
                effBillSetEntryValue(work->system, i, 0);
                parCellInit(work->system, i);
            }
            slot->age++;
            i++;
            slot++;
        } while (i < count);
    }
    parPrependCellNode(work->system);
    effDrawInstancedResourceTrianglesVU(work->resource);
}

void effPCPNeedleCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00176A10(EffPCPNeedleWork *work, void *matrix) {
    func_001770F8(work->resource->matrix, matrix);
}
