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
    u8 data[0x28];
} PathEntry40;

typedef struct {
    s32 unk0;
    PathEntry40 *unk4;
} PathData20;

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

INCLUDE_ASM(const s32, "basic/dds3PathBasic", func_00117340);
