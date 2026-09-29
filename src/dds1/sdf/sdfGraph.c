#include "common.h"

typedef struct SdfGraphObj {
    s16 width;
    s16 unk2;
    s16 height;
    u8 bufferMode;
    u8 auxiliaryMode;
    void *firstBuffer;
    void *secondBuffer;
    void *auxBuffer;
} SdfGraphObj;

extern u8 D_003980E0[];
extern u8 D_003BD2E8;
extern u8 D_003BD9DC;

void func_002D0E30(void *arg0, s32 arg1, s32 arg2);
void func_002D1B90(void *arg0);
void *func_002D1C70(s32 arg0, s32 arg1, s32 arg2);

void sdfGraphSetDisplayMode(s32 arg0) {
    D_003BD2E8 = (u8)arg0;
    func_002D0E30(D_003980E0, arg0, 0);
    D_003BD9DC = 1;
}

void sdfGraphReleaseBuffers(SdfGraphObj *graph) {
    func_002D1B90(graph->firstBuffer);
    graph->firstBuffer = NULL;
    func_002D1B90(graph->secondBuffer);
    graph->secondBuffer = NULL;
    func_002D1B90(graph->auxBuffer);
    graph->auxBuffer = NULL;
}

void sdfGraphRecreateBuffers(SdfGraphObj *graph) {
    s16 width;
    s16 height;
    u8 mode;

    sdfGraphReleaseBuffers(graph);
    width = graph->width;
    height = graph->height;
    graph->auxBuffer = func_002D1C70(width, height, graph->auxiliaryMode);
    mode = graph->bufferMode;
    graph->firstBuffer = func_002D1C70(width, height, mode);
    graph->secondBuffer = func_002D1C70(width, height, mode);
}

INCLUDE_ASM(const s32, "sdf/sdfGraph", func_002D0F90);

INCLUDE_ASM(const s32, "sdf/sdfGraph", func_002D1020);


INCLUDE_ASM(const s32, "sdf/sdfGraph", func_002D1080);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_003BD2E8);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_003BD2E9);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_003BD2EA);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_003BD2EC);

