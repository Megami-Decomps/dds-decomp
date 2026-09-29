#include "common.h"

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

void func_00116B80(s32 *arg0, f32 *arg1, void *arg2, f32 arg3);
void func_002E8278(void *arg0, f32 arg1);
void *memset(void *s, s32 c, u32 n);

void effFreeBuffers(s32 arg);
void func_002CFF98(void *arg);

void dds3FreePathObject(PathObj *path) {
    effFreeBuffers(path->bufferHandle);
    func_002CFF98(path);
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
        func_00116B80(&index, &fraction, vectorData, path->time);
        entries = vectorData->entries;
        first = &entries[index];
        __asm__ volatile (
            ".set noreorder\n"
            "lqc2 vf10, 0(%0)\n"
            ".set reorder"
            :
            : "r"(first)
            : "memory"
        );
        second = &entries[index] + 1;
        __asm__ volatile (
            ".set noreorder\n"
            "lqc2 vf11, 0(%0)\n"
            ".set reorder"
            :
            : "r"(second)
            : "memory"
        );
        func_002E8278(second, fraction);
    } else {
        __asm__ volatile (
            ".set noreorder\n"
            "vmove.xyzw vf10, vf0\n"
            ".set reorder"
            :
            :
            : "memory"
        );
    }
}

INCLUDE_ASM(const s32, "basic/dds3PathBasic", func_001170D8);
