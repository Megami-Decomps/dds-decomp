#include "common.h"
#include "sdf.h"


extern u8 D_004389D8;

extern u8 D_0043913C;

void func_00329CE0(void *arg0, s32 arg1, s32 arg2);

typedef struct SdfGraphObj {
    s16 width;
    s16 unk2;
    s16 height;
    u8 bufferMode;
    u8 auxiliaryMode;
    SdfTexResource *buffers[3];
} SdfGraphObj;

extern SdfGraphObj D_0040B290;

void sdfUpdateTextureHeadsWithInterruptsMasked(void *arg0);

void *sdfAllocImageBuffer(s32 arg0, s32 arg1, s32 arg2);

void sdfGraphSetDisplayMode(s32 mode) {
    D_004389D8 = (u8)mode;
    func_00329CE0(&D_0040B290, mode, 0);
    D_0043913C = 1;
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

extern SdfDisplayEnv D_004681D0;
extern void sceGsSetDefDispEnv(void *, s32, s32, s32, s32, s32);
extern void sceGsPutDispEnv(void *);

void sdfGraphSelectDisplayBuffer(s32 index) {
    SdfTexResource *buffer = D_0040B290.buffers[index];

    sceGsSetDefDispEnv((void *)(((u32)&D_004681D0 & 0x0FFFFFFF) | 0x20000000),
                      0, D_0040B290.width, D_0040B290.height, 0, 0);
    D_004681D0.fbp = buffer->word >> 11;
    sceGsPutDispEnv(&D_004681D0);
}

INCLUDE_ASM(const s32, "sdf/sdfGraph", func_00329ED0);

INCLUDE_ASM(const s32, "sdf/sdfGraph", func_00329F30);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_004389D8);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_004389D9);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", sdfCurrentBufferIndex);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", sdfGsImageUploadSemaphore);

