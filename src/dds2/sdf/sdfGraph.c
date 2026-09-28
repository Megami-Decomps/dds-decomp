#include "common.h"

extern u8 D_0040B290[];

extern u8 D_004389D8;

extern u8 D_0043913C;

void func_00329CE0(void *arg0, s32 arg1, s32 arg2);

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

void func_0032AA40(void *arg0);

void *func_0032AB20(s32 arg0, s32 arg1, s32 arg2);

void func_00329D38(s32 arg0) {
    D_004389D8 = (u8)arg0;
    func_00329CE0(D_0040B290, arg0, 0);
    D_0043913C = 1;
}

void func_00329D70(SdfGraphObj *graph) {
    func_0032AA40(graph->firstBuffer);
    graph->firstBuffer = NULL;
    func_0032AA40(graph->secondBuffer);
    graph->secondBuffer = NULL;
    func_0032AA40(graph->auxBuffer);
    graph->auxBuffer = NULL;
}

void func_00329DB8(SdfGraphObj *graph) {
    s16 width;
    s16 height;
    u8 mode;

    func_00329D70(graph);
    width = graph->width;
    height = graph->height;
    graph->auxBuffer = func_0032AB20(width, height, graph->auxiliaryMode);
    mode = graph->bufferMode;
    graph->firstBuffer = func_0032AB20(width, height, mode);
    graph->secondBuffer = func_0032AB20(width, height, mode);
}

INCLUDE_ASM(const s32, "sdf/sdfGraph", func_00329E40);

INCLUDE_ASM(const s32, "sdf/sdfGraph", func_00329ED0);

INCLUDE_ASM(const s32, "sdf/sdfGraph", func_00329F30);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_004389D8);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_004389D9);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_004389DA);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_004389DC);

