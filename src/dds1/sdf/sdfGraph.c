#include "common.h"
#include "sdf.h"

typedef struct SdfGraphObj {
    s16 width;
    s16 unk2;
    s16 height;
    u8 bufferMode;
    u8 auxiliaryMode;
    SdfTexResource *buffers[3];
} SdfGraphObj;

extern SdfGraphObj D_003980E0;
extern u8 D_003BD2E8;
extern u8 D_003BD9DC;

void func_002D0E30(void *arg0, s32 arg1, s32 arg2);
void sdfUpdateTextureHeadsWithInterruptsMasked(void *arg0);
void *sdfAllocImageBuffer(s32 arg0, s32 arg1, s32 arg2);

void sdfGraphSetDisplayMode(s32 arg0) {
    D_003BD2E8 = (u8)arg0;
    func_002D0E30(&D_003980E0, arg0, 0);
    D_003BD9DC = 1;
}

/* Release all three independently allocated image buffers before rebuilding. */
void sdfGraphReleaseBuffers(SdfGraphObj *graph) {
    sdfUpdateTextureHeadsWithInterruptsMasked(graph->buffers[0]);
    graph->buffers[0] = NULL;
    sdfUpdateTextureHeadsWithInterruptsMasked(graph->buffers[1]);
    graph->buffers[1] = NULL;
    sdfUpdateTextureHeadsWithInterruptsMasked(graph->buffers[2]);
    graph->buffers[2] = NULL;
}

/* The auxiliary buffer uses its own mode; both primary buffers share one mode. */
void sdfGraphRecreateBuffers(SdfGraphObj *graph) {
    s16 width;
    s16 height;
    u8 mode;

    sdfGraphReleaseBuffers(graph);
    width = graph->width;
    height = graph->height;
    graph->buffers[2] = sdfAllocImageBuffer(width, height, graph->auxiliaryMode);
    mode = graph->bufferMode;
    graph->buffers[0] = sdfAllocImageBuffer(width, height, mode);
    graph->buffers[1] = sdfAllocImageBuffer(width, height, mode);
}

typedef struct SdfDisplayEnv {
    u8 pad00[0x10];
    u32 fbp : 9;
    u32 : 23;
} SdfDisplayEnv;

extern SdfDisplayEnv D_003EB820;
extern void sceGsSetDefDispEnv(void *, s32, s32, s32, s32, s32);
extern void sceGsPutDispEnv(void *);

void sdfGraphSelectDisplayBuffer(s32 index) {
    SdfTexResource *buffer = D_003980E0.buffers[index];

    sceGsSetDefDispEnv((void *)(((u32)&D_003EB820 & 0x0FFFFFFF) | 0x20000000),
                      0, D_003980E0.width, D_003980E0.height, 0, 0);
    D_003EB820.fbp = buffer->word >> 11;
    sceGsPutDispEnv(&D_003EB820);
}

INCLUDE_ASM(const s32, "sdf/sdfGraph", func_002D1020);


INCLUDE_ASM(const s32, "sdf/sdfGraph", func_002D1080);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_003BD2E8);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_003BD2E9);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", sdfCurrentBufferIndex);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", sdfGsImageUploadSemaphore);

