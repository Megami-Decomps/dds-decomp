#include "common.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "dds3_path.h"
#include "pcp_vu0.h"
#include "eff_quaternion_nlerp.h"

void *memset(void *s, s32 c, u32 n);
void effFreeBuffers(struct EffPrimitiveCurve *primitive);

void dds3FreePathObject(Dds3PathCurveWork *path) {
    effFreeBuffers(path->primitiveCurve);
    sdfReleaseChipBlock(path);
}

/* vu0 routine: interpolate the path's XYZ keys into vf10. */
void dds3InterpolatePathVectorVU(Dds3PathCurveWork *path) {
    u32 segmentIndex;
    f32 interpolationWeight;
    Dds3PathKeyframes *keyframes;
    Dds3PathPositionSample *entries;

    if (path->flags & DDS3_PATH_POSITION_CHANNEL) {
        keyframes = path->positionKeys;
        dds3SamplePathKeyframeInterval(&segmentIndex, &interpolationWeight, keyframes, path->time);
        entries = keyframes->data.position;
        VU0_SET_VF10_COMPONENT(x, entries[segmentIndex + 1][0]);
        VU0_SET_VF10_COMPONENT(y, entries[segmentIndex + 1][1]);
        VU0_SET_VF10_COMPONENT(z, entries[segmentIndex + 1][2]);
        VU0_SCALAR_OP(interpolationWeight, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_MOVE_VF(vf11, vf10);
        VU0_SET_VF10_COMPONENT(x, entries[segmentIndex][0]);
        VU0_SET_VF10_COMPONENT(y, entries[segmentIndex][1]);
        VU0_SET_VF10_COMPONENT(z, entries[segmentIndex][2]);
        VU0_SCALAR_OP(1.0f - interpolationWeight, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_ADD(vf10, vf10, vf11);
        VU0_CLEAR_W(vf10);
    } else {
        VU0_MOVE_VF(vf10, vf0);
    }
}

void dds3InterpolatePathQuaternionVU(Dds3PathCurveWork *path) {
    u32 segmentIndex;
    f32 interpolationWeight;
    Dds3PathKeyframes *keyframes;
    Dds3PathRotationSample *entries;
    Dds3PathRotationSample *firstKey;
    Dds3PathRotationSample *secondKey;
    if (path->flags & DDS3_PATH_ROTATION_CHANNEL) {
        keyframes = path->rotationKeys;
        dds3SamplePathKeyframeInterval(&segmentIndex, &interpolationWeight, keyframes, path->time);
        entries = keyframes->data.rotation;
        firstKey = &entries[segmentIndex];
        VU0_LOAD_VF_MEMORY(vf10, firstKey);
        secondKey = &entries[segmentIndex] + 1;
        VU0_LOAD_VF_MEMORY(vf11, secondKey);
        effMiscQuaternionNlerpVU(interpolationWeight);
    } else {
        VU0_MOVE_VF(vf10, vf0);
    }
}

/* vu0 routine: lerp the three key vectors (xyzw, xyz, xyz) of path entries `segmentIndex` and `segmentIndex + 1` at the sampled weight into out, or clear out */
void dds3InterpolatePathOutput(Dds3PathCurveWork *path, WorldTransformParams *out) {
    u32 segmentIndex;
    f32 interpolationWeight;
    Dds3PathKeyframes *keyframes;
    struct WorldTransformParams *entries;

    if (path->flags & DDS3_PATH_WORLD_TRANSFORM_CHANNEL) {
        keyframes = path->transformKeys;
        dds3SamplePathKeyframeInterval(&segmentIndex, &interpolationWeight, keyframes, path->time);
        entries = keyframes->data.worldTransform;
        VU0_SET_VF10_COMPONENT(x, entries[segmentIndex + 1].rotation[0]);
        VU0_SET_VF10_COMPONENT(y, entries[segmentIndex + 1].rotation[1]);
        VU0_SET_VF10_COMPONENT(z, entries[segmentIndex + 1].rotation[2]);
        VU0_SET_VF10_W(entries[segmentIndex + 1].rotation[3]);
        VU0_SCALAR_OP(interpolationWeight, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_MOVE_VF(vf11, vf10);
        VU0_SET_VF10_COMPONENT(x, entries[segmentIndex].rotation[0]);
        VU0_SET_VF10_COMPONENT(y, entries[segmentIndex].rotation[1]);
        VU0_SET_VF10_COMPONENT(z, entries[segmentIndex].rotation[2]);
        VU0_SET_VF10_W(entries[segmentIndex].rotation[3]);
        VU0_SCALAR_OP(1.0f - interpolationWeight, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_ADD(vf10, vf10, vf11);
        VU0_GET_VF10_X(out->rotation[0]);
        VU0_GET_VF10_Y(out->rotation[1]);
        VU0_GET_VF10_Z(out->rotation[2]);
        VU0_GET_VF10_W(out->rotation[3]);
        VU0_SET_VF10_COMPONENT(x, entries[segmentIndex + 1].position[0]);
        VU0_SET_VF10_COMPONENT(y, entries[segmentIndex + 1].position[1]);
        VU0_SET_VF10_COMPONENT(z, entries[segmentIndex + 1].position[2]);
        VU0_SCALAR_OP(interpolationWeight, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_MOVE_VF(vf11, vf10);
        VU0_SET_VF10_COMPONENT(x, entries[segmentIndex].position[0]);
        VU0_SET_VF10_COMPONENT(y, entries[segmentIndex].position[1]);
        VU0_SET_VF10_COMPONENT(z, entries[segmentIndex].position[2]);
        VU0_SCALAR_OP(1.0f - interpolationWeight, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_ADD(vf10, vf10, vf11);
        VU0_GET_VF10_X(out->position[0]);
        VU0_GET_VF10_Y(out->position[1]);
        VU0_GET_VF10_Z(out->position[2]);
        VU0_SET_VF10_COMPONENT(x, entries[segmentIndex + 1].scale[0]);
        VU0_SET_VF10_COMPONENT(y, entries[segmentIndex + 1].scale[1]);
        VU0_SET_VF10_COMPONENT(z, entries[segmentIndex + 1].scale[2]);
        VU0_SCALAR_OP(interpolationWeight, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_MOVE_VF(vf11, vf10);
        VU0_SET_VF10_COMPONENT(x, entries[segmentIndex].scale[0]);
        VU0_SET_VF10_COMPONENT(y, entries[segmentIndex].scale[1]);
        VU0_SET_VF10_COMPONENT(z, entries[segmentIndex].scale[2]);
        VU0_SCALAR_OP(1.0f - interpolationWeight, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_ADD(vf10, vf10, vf11);
        VU0_GET_VF10_X(out->scale[0]);
        VU0_GET_VF10_Y(out->scale[1]);
        VU0_GET_VF10_Z(out->scale[2]);
    } else {
        memset(out, 0, 0x28);
    }
}
