#include "common.h"
#include "pcp_vu0.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} PathEntry12;

typedef struct {
    u8 data[0x10];
} PathEntry16;

typedef struct {
    s32 unk0;
    PathEntry12 *unk4;
} PathData14;

typedef struct {
    s32 unk0;
    PathEntry16 *unk4;
} PathData18;

typedef struct {
    f32 f[10];
} PathEntry40;

typedef struct {
    s32 unk0;
    PathEntry40 *unk4;
} PathData20;

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

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    f32 unkC;
    s32 unk10;
    PathData14 *unk14;
    PathData18 *unk18;
    s32 unk1C;
    PathData20 *unk20;
} PathObj;

void func_00116DE8(s32 *arg0, f32 *arg1, void *arg2, f32 arg3);

void effMiscQuaternionNlerpVU(void *arg0, f32 arg1);
void *memset(void *s, s32 c, u32 n);

void dds3FreePathObject(PathObj *path) {
    effFreeBuffers(path->unk10);
    sdfReleaseChipBlock(path);
}

INCLUDE_ASM(const s32, "basic/dds3PathBasic", func_001171A0);

void dds3PreparePathVectorPair(PathObj *arg) {
    s32 idx;
    f32 frac;
    PathData18 *data;
    PathEntry16 *base;
    PathEntry16 *p1;
    PathEntry16 *p2;
    if (arg->unk4 & 2) {
        data = arg->unk18;
        func_00116DE8(&idx, &frac, data, arg->unkC);
        base = data->unk4;
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
void func_00117340(PathObj *path, PathOut *out) {
    s32 index;
    f32 fraction;
    PathData20 *data;
    PathEntry40 *entries;

    if (path->unk4 & 0x10) {
        data = path->unk20;
        func_00116DE8(&index, &fraction, data, path->unkC);
        entries = data->unk4;
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
