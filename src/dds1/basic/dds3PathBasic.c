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
    PathEntry16 *entries;
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
    s32 flags;
    s32 unk8;
    f32 time;
    s32 bufferHandle;
    PathData14 *unk14;
    PathData18 *vectorData;
    s32 unk1C;
    PathData20 *unk20;
} PathObj;

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

void func_00116B80(s32 *arg0, f32 *arg1, f32 arg2, void *arg3);
void effMiscQuaternionNlerpVU(void *arg0, f32 arg1);
void *memset(void *s, s32 c, u32 n);

void effFreeBuffers(s32 arg);
void sdfReleaseChipBlock(void *arg);

void dds3FreePathObject(PathObj *path) {
    effFreeBuffers(path->bufferHandle);
    sdfReleaseChipBlock(path);
}

INCLUDE_ASM(const s32, "basic/dds3PathBasic", func_00116F38);

void dds3PreparePathVectorPair(PathObj *path) {
    s32 index;
    f32 fraction;
    PathData18 *vectorData;
    PathEntry16 *entries;
    PathEntry16 *first;
    PathEntry16 *second;
    if (path->flags & 2) {
        vectorData = path->vectorData;
        func_00116B80(&index, &fraction, path->time, vectorData);
        entries = vectorData->entries;
        first = &entries[index];
        VU0_LOAD_VF_MEMORY(vf10, first);
        second = &entries[index] + 1;
        VU0_LOAD_VF_MEMORY(vf11, second);
        effMiscQuaternionNlerpVU(second, fraction);
    } else {
        VU0_MOVE_VF(vf10, vf0);
    }
}

INCLUDE_ASM(const s32, "basic/dds3PathBasic", func_001170D8);
