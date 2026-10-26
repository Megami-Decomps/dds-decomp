#include "sdk_ps2.h"
#include "common.h"
#include "sdf_vu0.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "dds3_path.h"
#include "pcp_vu0.h"
#include "eff_quaternion_nlerp.h"


void dds3FreeSplineHandle(struct EffPrimitiveCurve *primitive);

void dds3DeletePathControl(dds3PathBasicControl_t *path) {
    dds3FreeSplineHandle(path->primitiveCurve);
    sdfChipFreeDirect(path);
}

/* vu0 routine: interpolate the path's XYZ keys into vf10. */
void dds3PathBasicControlGetPos(dds3PathBasicControl_t *path) {
    u32 segmentIndex;
    f32 interpolationWeight;
    Dds3PathKeyframes *keyframes;
    Dds3PathPositionSample *entries;

    if (path->counter.flags & DDS3_PATH_POSITION_CHANNEL) {
        keyframes = path->positionKeys;
        dds3PathEvalSpline(&segmentIndex, &interpolationWeight, keyframes, path->counter.time);
        entries = keyframes->data.position;
        VU0_SET_VF10_COMPONENT(x, entries[segmentIndex + 1][0]);
        VU0_SET_VF10_COMPONENT(y, entries[segmentIndex + 1][1]);
        VU0_SET_VF10_COMPONENT(z, entries[segmentIndex + 1][2]);
        VU0_SCALAR_OP(interpolationWeight, "vmulx.xyzw vf10, vf10, vf2x");
        SDF_VU0_COPYVECTOR_A_TO_B();
        VU0_SET_VF10_COMPONENT(x, entries[segmentIndex][0]);
        VU0_SET_VF10_COMPONENT(y, entries[segmentIndex][1]);
        VU0_SET_VF10_COMPONENT(z, entries[segmentIndex][2]);
        VU0_SCALAR_OP(1.0f - interpolationWeight, "vmulx.xyzw vf10, vf10, vf2x");
        SDF_VU0_ADDVECTOR();
        SDF_VU0_VECTOR_W_SET_ZERO();
    } else {
        SDF_VU0_LOADIDENTITYVECTOR();
    }
}

void dds3PathBasicControlGetRot(dds3PathBasicControl_t *path) {
    u32 segmentIndex;
    f32 interpolationWeight;
    Dds3PathKeyframes *keyframes;
    Dds3PathRotationSample *entries;
    Dds3PathRotationSample *firstKey;
    Dds3PathRotationSample *secondKey;
    if (path->counter.flags & DDS3_PATH_ROTATION_CHANNEL) {
        keyframes = path->rotationKeys;
        dds3PathEvalSpline(&segmentIndex, &interpolationWeight, keyframes, path->counter.time);
        entries = keyframes->data.rotation;
        firstKey = &entries[segmentIndex];
        SDF_VU0_LOADVECTOR_BARRIER(firstKey);
        secondKey = &entries[segmentIndex] + 1;
        SDF_VU0_LOADVECTOR_B_BARRIER(secondKey);
        Lerp(interpolationWeight);
    } else {
        SDF_VU0_LOADIDENTITYVECTOR();
    }
}

/* Interpolate rotation, position, and scale at the current path time, or clear the output. */
void dds3PathBasicControlGetTransform(const dds3PathBasicControl_t *path, WorldTransformParams *transformParams) {
    u32 segmentIndex;
    f32 interpolationWeight;
    const Dds3PathKeyframes *keyframes;
    const WorldTransformParams *entries;

    if (path->counter.flags & DDS3_PATH_WORLD_TRANSFORM_CHANNEL) {
        keyframes = path->transformKeys;
        dds3PathEvalSpline(&segmentIndex, &interpolationWeight, keyframes, path->counter.time);
        entries = keyframes->data.worldTransform;
        VU0_SET_VF10_COMPONENT(x, entries[segmentIndex + 1].rotation[0]);
        VU0_SET_VF10_COMPONENT(y, entries[segmentIndex + 1].rotation[1]);
        VU0_SET_VF10_COMPONENT(z, entries[segmentIndex + 1].rotation[2]);
        VU0_SET_VF10_W(entries[segmentIndex + 1].rotation[3]);
        VU0_SCALAR_OP(interpolationWeight, "vmulx.xyzw vf10, vf10, vf2x");
        SDF_VU0_COPYVECTOR_A_TO_B();
        VU0_SET_VF10_COMPONENT(x, entries[segmentIndex].rotation[0]);
        VU0_SET_VF10_COMPONENT(y, entries[segmentIndex].rotation[1]);
        VU0_SET_VF10_COMPONENT(z, entries[segmentIndex].rotation[2]);
        VU0_SET_VF10_W(entries[segmentIndex].rotation[3]);
        VU0_SCALAR_OP(1.0f - interpolationWeight, "vmulx.xyzw vf10, vf10, vf2x");
        SDF_VU0_ADDVECTOR();
        SDF_VU0_GETVECTOR_X(transformParams->rotation[0]);
        SDF_VU0_GETVECTOR_Y(transformParams->rotation[1]);
        SDF_VU0_GETVECTOR_Z(transformParams->rotation[2]);
        SDF_VU0_GETVECTOR_W(transformParams->rotation[3]);
        VU0_SET_VF10_COMPONENT(x, entries[segmentIndex + 1].position[0]);
        VU0_SET_VF10_COMPONENT(y, entries[segmentIndex + 1].position[1]);
        VU0_SET_VF10_COMPONENT(z, entries[segmentIndex + 1].position[2]);
        VU0_SCALAR_OP(interpolationWeight, "vmulx.xyzw vf10, vf10, vf2x");
        SDF_VU0_COPYVECTOR_A_TO_B();
        VU0_SET_VF10_COMPONENT(x, entries[segmentIndex].position[0]);
        VU0_SET_VF10_COMPONENT(y, entries[segmentIndex].position[1]);
        VU0_SET_VF10_COMPONENT(z, entries[segmentIndex].position[2]);
        VU0_SCALAR_OP(1.0f - interpolationWeight, "vmulx.xyzw vf10, vf10, vf2x");
        SDF_VU0_ADDVECTOR();
        SDF_VU0_GETVECTOR_X(transformParams->position[0]);
        SDF_VU0_GETVECTOR_Y(transformParams->position[1]);
        SDF_VU0_GETVECTOR_Z(transformParams->position[2]);
        VU0_SET_VF10_COMPONENT(x, entries[segmentIndex + 1].scale[0]);
        VU0_SET_VF10_COMPONENT(y, entries[segmentIndex + 1].scale[1]);
        VU0_SET_VF10_COMPONENT(z, entries[segmentIndex + 1].scale[2]);
        VU0_SCALAR_OP(interpolationWeight, "vmulx.xyzw vf10, vf10, vf2x");
        SDF_VU0_COPYVECTOR_A_TO_B();
        VU0_SET_VF10_COMPONENT(x, entries[segmentIndex].scale[0]);
        VU0_SET_VF10_COMPONENT(y, entries[segmentIndex].scale[1]);
        VU0_SET_VF10_COMPONENT(z, entries[segmentIndex].scale[2]);
        VU0_SCALAR_OP(1.0f - interpolationWeight, "vmulx.xyzw vf10, vf10, vf2x");
        SDF_VU0_ADDVECTOR();
        SDF_VU0_GETVECTOR_X(transformParams->scale[0]);
        SDF_VU0_GETVECTOR_Y(transformParams->scale[1]);
        SDF_VU0_GETVECTOR_Z(transformParams->scale[2]);
    } else {
        memset(transformParams, 0, 0x28);
    }
}
