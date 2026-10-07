#include "common.h"
#include "dds3_path.h"
#include "pcp_vu0.h"

typedef f32 PathEntry12[3];

typedef struct {
    u8 data[0x10];
} PathEntry16;

typedef struct {
    f32 f[10];
} PathEntry40;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
} PathOut;

void dds3SamplePathKeyframeInterval(u32 *index, f32 *fraction, Dds3PathKeyframes *keys, f32 time);

void effMiscQuaternionNlerpVU(void *arg0, f32 arg1);
void *memset(void *s, s32 c, u32 n);

void dds3FreePathObject(Dds3PathCurveWork *path) {
    effFreeBuffers((s32)path->primitiveCurve);
    sdfReleaseChipBlock(path);
}

/* vu0 routine: interpolate the path's XYZ keys into vf10. */
void dds3InterpolatePathVectorVU(Dds3PathCurveWork *path) {
    u32 index;
    f32 fraction;
    Dds3PathKeyframes *data;
    PathEntry12 *entries;

    if (path->flags & 1) {
        data = path->positionKeys;
        dds3SamplePathKeyframeInterval(&index, &fraction, data, path->time);
        entries = (PathEntry12 *)data->data;
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

void dds3PreparePathVectorPair(Dds3PathCurveWork *arg) {
    u32 idx;
    f32 frac;
    Dds3PathKeyframes *data;
    PathEntry16 *base;
    PathEntry16 *p1;
    PathEntry16 *p2;
    if (arg->flags & 2) {
        data = arg->rotationKeys;
        dds3SamplePathKeyframeInterval(&idx, &frac, data, arg->time);
        base = (PathEntry16 *)data->data;
        p1 = &base[idx];
        VU0_LOAD_VF_MEMORY(vf10, p1);
        p2 = &base[idx] + 1;
        VU0_LOAD_VF_MEMORY(vf11, p2);
        effMiscQuaternionNlerpVU(p2, frac);
    } else {
        VU0_MOVE_VF(vf10, vf0);
    }
}

/* vu0 routine: lerp the three key vectors (xyzw, xyz, xyz) of path entries `index` and `index + 1` at the sampled fraction into out, or clear out */
void dds3InterpolatePathOutput(Dds3PathCurveWork *path, PathOut *out) {
    u32 index;
    f32 fraction;
    Dds3PathKeyframes *data;
    PathEntry40 *entries;

    if (path->flags & 0x10) {
        data = path->transformKeys;
        dds3SamplePathKeyframeInterval(&index, &fraction, data, path->time);
        entries = (PathEntry40 *)data->data;
        VU0_SET_VF10_COMPONENT(x, entries[index + 1].f[0]);
        VU0_SET_VF10_COMPONENT(y, entries[index + 1].f[1]);
        VU0_SET_VF10_COMPONENT(z, entries[index + 1].f[2]);
        VU0_SET_VF10_W(entries[index + 1].f[3]);
        VU0_SCALAR_OP(fraction, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_MOVE_VF(vf11, vf10);
        VU0_SET_VF10_COMPONENT(x, entries[index].f[0]);
        VU0_SET_VF10_COMPONENT(y, entries[index].f[1]);
        VU0_SET_VF10_COMPONENT(z, entries[index].f[2]);
        VU0_SET_VF10_W(entries[index].f[3]);
        VU0_SCALAR_OP(1.0f - fraction, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_ADD(vf10, vf10, vf11);
        VU0_GET_VF10_X(out->unk0);
        VU0_GET_VF10_Y(out->unk4);
        VU0_GET_VF10_Z(out->unk8);
        VU0_GET_VF10_W(out->unkC);
        VU0_SET_VF10_COMPONENT(x, entries[index + 1].f[4]);
        VU0_SET_VF10_COMPONENT(y, entries[index + 1].f[5]);
        VU0_SET_VF10_COMPONENT(z, entries[index + 1].f[6]);
        VU0_SCALAR_OP(fraction, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_MOVE_VF(vf11, vf10);
        VU0_SET_VF10_COMPONENT(x, entries[index].f[4]);
        VU0_SET_VF10_COMPONENT(y, entries[index].f[5]);
        VU0_SET_VF10_COMPONENT(z, entries[index].f[6]);
        VU0_SCALAR_OP(1.0f - fraction, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_ADD(vf10, vf10, vf11);
        VU0_GET_VF10_X(out->unk10);
        VU0_GET_VF10_Y(out->unk14);
        VU0_GET_VF10_Z(out->unk18);
        VU0_SET_VF10_COMPONENT(x, entries[index + 1].f[7]);
        VU0_SET_VF10_COMPONENT(y, entries[index + 1].f[8]);
        VU0_SET_VF10_COMPONENT(z, entries[index + 1].f[9]);
        VU0_SCALAR_OP(fraction, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_MOVE_VF(vf11, vf10);
        VU0_SET_VF10_COMPONENT(x, entries[index].f[7]);
        VU0_SET_VF10_COMPONENT(y, entries[index].f[8]);
        VU0_SET_VF10_COMPONENT(z, entries[index].f[9]);
        VU0_SCALAR_OP(1.0f - fraction, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_ADD(vf10, vf10, vf11);
        VU0_GET_VF10_X(out->unk1C);
        VU0_GET_VF10_Y(out->unk20);
        VU0_GET_VF10_Z(out->unk24);
    } else {
        memset(out, 0, 0x28);
    }
}
