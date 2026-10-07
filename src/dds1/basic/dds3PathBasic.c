#include "common.h"
#include "dds3obj.h"
#include "dds3_path.h"
#include "pcp_vu0.h"

typedef f32 PathPositionKey[3];
typedef f32 PathQuaternionKey[4];

void dds3SamplePathKeyframeInterval(u32 *index, f32 *fraction, Dds3PathKeyframes *keys, f32 time);
void effMiscQuaternionNlerpVU(void *arg0, f32 arg1);
void *memset(void *s, s32 c, u32 n);

void effFreeBuffers(s32 arg);
void sdfReleaseChipBlock(void *arg);

void dds3FreePathObject(Dds3PathCurveWork *path) {
    effFreeBuffers((s32)path->primitiveCurve);
    sdfReleaseChipBlock(path);
}

/* vu0 routine: interpolate the path's XYZ keys into vf10. */
void dds3InterpolatePathVectorVU(Dds3PathCurveWork *path) {
    u32 index;
    f32 fraction;
    Dds3PathKeyframes *data;
    PathPositionKey *entries;

    if (path->flags & 1) {
        data = path->positionKeys;
        dds3SamplePathKeyframeInterval(&index, &fraction, data, path->time);
        entries = (PathPositionKey *)data->data;
        VU0_SET_VF10_COMPONENT(x, entries[index + 1][0]);
        VU0_SET_VF10_COMPONENT(y, entries[index + 1][1]);
        VU0_SET_VF10_COMPONENT(z, entries[index + 1][2]);
        VU0_SCALAR_OP(fraction, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_MOVE_VF(vf11, vf10);
        VU0_SET_VF10_COMPONENT(x, entries[index][0]);
        VU0_SET_VF10_COMPONENT(y, entries[index][1]);
        VU0_SET_VF10_COMPONENT(z, entries[index][2]);
        VU0_SCALAR_OP(1.0f - fraction, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_ADD(vf10, vf10, vf11);
        VU0_CLEAR_W(vf10);
    } else {
        VU0_MOVE_VF(vf10, vf0);
    }
}

void dds3PreparePathVectorPair(Dds3PathCurveWork *path) {
    u32 index;
    f32 fraction;
    Dds3PathKeyframes *vectorData;
    PathQuaternionKey *entries;
    PathQuaternionKey *first;
    PathQuaternionKey *second;
    if (path->flags & 2) {
        vectorData = path->rotationKeys;
        dds3SamplePathKeyframeInterval(&index, &fraction, vectorData, path->time);
        entries = (PathQuaternionKey *)vectorData->data;
        first = &entries[index];
        VU0_LOAD_VF_MEMORY(vf10, first);
        second = &entries[index] + 1;
        VU0_LOAD_VF_MEMORY(vf11, second);
        effMiscQuaternionNlerpVU(second, fraction);
    } else {
        VU0_MOVE_VF(vf10, vf0);
    }
}

/* vu0 routine: lerp the three key vectors (xyzw, xyz, xyz) of path entries `index` and `index + 1` at the sampled fraction into out, or clear out */
void dds3InterpolatePathOutput(Dds3PathCurveWork *path, WorldTransformParams *out) {
    u32 index;
    f32 fraction;
    Dds3PathKeyframes *data;
    WorldTransformParams *entries;

    if (path->flags & 0x10) {
        data = path->transformKeys;
        dds3SamplePathKeyframeInterval(&index, &fraction, data, path->time);
        entries = (WorldTransformParams *)data->data;
        VU0_SET_VF10_COMPONENT(x, entries[index + 1].rotation[0]);
        VU0_SET_VF10_COMPONENT(y, entries[index + 1].rotation[1]);
        VU0_SET_VF10_COMPONENT(z, entries[index + 1].rotation[2]);
        VU0_SET_VF10_W(entries[index + 1].rotation[3]);
        VU0_SCALAR_OP(fraction, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_MOVE_VF(vf11, vf10);
        VU0_SET_VF10_COMPONENT(x, entries[index].rotation[0]);
        VU0_SET_VF10_COMPONENT(y, entries[index].rotation[1]);
        VU0_SET_VF10_COMPONENT(z, entries[index].rotation[2]);
        VU0_SET_VF10_W(entries[index].rotation[3]);
        VU0_SCALAR_OP(1.0f - fraction, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_ADD(vf10, vf10, vf11);
        VU0_GET_VF10_X(out->rotation[0]);
        VU0_GET_VF10_Y(out->rotation[1]);
        VU0_GET_VF10_Z(out->rotation[2]);
        VU0_GET_VF10_W(out->rotation[3]);
        VU0_SET_VF10_COMPONENT(x, entries[index + 1].position[0]);
        VU0_SET_VF10_COMPONENT(y, entries[index + 1].position[1]);
        VU0_SET_VF10_COMPONENT(z, entries[index + 1].position[2]);
        VU0_SCALAR_OP(fraction, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_MOVE_VF(vf11, vf10);
        VU0_SET_VF10_COMPONENT(x, entries[index].position[0]);
        VU0_SET_VF10_COMPONENT(y, entries[index].position[1]);
        VU0_SET_VF10_COMPONENT(z, entries[index].position[2]);
        VU0_SCALAR_OP(1.0f - fraction, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_ADD(vf10, vf10, vf11);
        VU0_GET_VF10_X(out->position[0]);
        VU0_GET_VF10_Y(out->position[1]);
        VU0_GET_VF10_Z(out->position[2]);
        VU0_SET_VF10_COMPONENT(x, entries[index + 1].scale[0]);
        VU0_SET_VF10_COMPONENT(y, entries[index + 1].scale[1]);
        VU0_SET_VF10_COMPONENT(z, entries[index + 1].scale[2]);
        VU0_SCALAR_OP(fraction, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_MOVE_VF(vf11, vf10);
        VU0_SET_VF10_COMPONENT(x, entries[index].scale[0]);
        VU0_SET_VF10_COMPONENT(y, entries[index].scale[1]);
        VU0_SET_VF10_COMPONENT(z, entries[index].scale[2]);
        VU0_SCALAR_OP(1.0f - fraction, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_ADD(vf10, vf10, vf11);
        VU0_GET_VF10_X(out->scale[0]);
        VU0_GET_VF10_Y(out->scale[1]);
        VU0_GET_VF10_Z(out->scale[2]);
    } else {
        memset(out, 0, 0x28);
    }
}
