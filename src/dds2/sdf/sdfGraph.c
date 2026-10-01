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

void sdfUpdateTextureHeadsWithInterruptsMasked(void *arg0);

void *sdfAllocImageBuffer(s32 arg0, s32 arg1, s32 arg2);

void sdfGraphSetDisplayMode(s32 mode) {
    D_004389D8 = (u8)mode;
    func_00329CE0(D_0040B290, mode, 0);
    D_0043913C = 1;
}

/* Release all three independently allocated image buffers before rebuilding. */
void sdfGraphReleaseBuffers(SdfGraphObj *graph) {
    sdfUpdateTextureHeadsWithInterruptsMasked(graph->firstBuffer);
    graph->firstBuffer = NULL;
    sdfUpdateTextureHeadsWithInterruptsMasked(graph->secondBuffer);
    graph->secondBuffer = NULL;
    sdfUpdateTextureHeadsWithInterruptsMasked(graph->auxBuffer);
    graph->auxBuffer = NULL;
}

/* The auxiliary buffer uses its own mode; both primary buffers share one mode. */
void sdfGraphRecreateBuffers(SdfGraphObj *graph) {
    s16 width;
    s16 height;
    u8 mode;

    sdfGraphReleaseBuffers(graph);
    width = graph->width;
    height = graph->height;
    graph->auxBuffer = sdfAllocImageBuffer(width, height, graph->auxiliaryMode);
    mode = graph->bufferMode;
    graph->firstBuffer = sdfAllocImageBuffer(width, height, mode);
    graph->secondBuffer = sdfAllocImageBuffer(width, height, mode);
}

INCLUDE_ASM(const s32, "sdf/sdfGraph", func_00329E40);

INCLUDE_ASM(const s32, "sdf/sdfGraph", func_00329ED0);

INCLUDE_ASM(const s32, "sdf/sdfGraph", func_00329F30);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_004389D8);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_004389D9);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_004389DA);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_004389DC);

