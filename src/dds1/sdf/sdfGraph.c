#include "common.h"
#include "sdf.h"

#define SDF_GRAPH_AUXILIARY_BUFFER 2
#define SDF_GRAPH_CPU_ADDRESS_MASK 0x0FFFFFFF
#define SDF_GRAPH_UNCACHED_ALIAS 0x20000000
#define SDF_GS_FRAMEBUFFER_PAGE_SHIFT 11

extern SdfGraphObj D_003980E0;
extern u8 D_003BD2E8;
extern u8 D_003BD9DC;

void func_002D0E30(void *arg0, s32 arg1, s32 arg2);
void sdfUpdateTextureHeadsWithInterruptsMasked(void *arg0);
SdfTexResource *sdfAllocImageBuffer(s32 width, s32 height, s32 format);

void sdfGraphSetDisplayMode(s32 arg0) {
    D_003BD2E8 = (u8)arg0;
    func_002D0E30(&D_003980E0, arg0, 0);
    D_003BD9DC = 1;
}

/* Release buffers in slot order, clearing each slot immediately afterward. */
void sdfGraphReleaseBuffers(SdfGraphObj *graph) {
    sdfUpdateTextureHeadsWithInterruptsMasked(graph->buffers[0]);
    graph->buffers[0] = NULL;
    sdfUpdateTextureHeadsWithInterruptsMasked(graph->buffers[1]);
    graph->buffers[1] = NULL;
    sdfUpdateTextureHeadsWithInterruptsMasked(graph->buffers[SDF_GRAPH_AUXILIARY_BUFFER]);
    graph->buffers[SDF_GRAPH_AUXILIARY_BUFFER] = NULL;
}

/* Rebuild the auxiliary buffer with its own PSM; both others share a PSM. */
void sdfGraphRecreateBuffers(SdfGraphObj *graph) {
    s16 width;
    s16 height;
    u8 bufferFormat;

    sdfGraphReleaseBuffers(graph);
    width = graph->width;
    height = graph->height;
    graph->buffers[SDF_GRAPH_AUXILIARY_BUFFER] = sdfAllocImageBuffer(width, height, graph->auxiliaryFormat);
    bufferFormat = graph->bufferFormat;
    graph->buffers[0] = sdfAllocImageBuffer(width, height, bufferFormat);
    graph->buffers[1] = sdfAllocImageBuffer(width, height, bufferFormat);
}

typedef struct SdfDisplayEnv {
    u8 pad00[0x10];
    u32 fbp : 9;
    u32 : 23;
} SdfDisplayEnv;

extern SdfDisplayEnv D_003EB820;
extern void sceGsSetDefDispEnv(void *, s32, s32, s32, s32, s32);
extern void sceGsPutDispEnv(void *);

/* Select the indexed image buffer for display; the index and pointer are unchecked. */
void sdfGraphSelectDisplayBuffer(s32 bufferIndex) {
    SdfTexResource *displayBuffer = D_003980E0.buffers[bufferIndex];

    /* Initialize the environment through its uncached main-RAM alias. */
    sceGsSetDefDispEnv((void *)(((u32)&D_003EB820 & SDF_GRAPH_CPU_ADDRESS_MASK) | SDF_GRAPH_UNCACHED_ALIAS),
                      0, D_003980E0.width, D_003980E0.height, 0, 0);
    /* VRAM offsets are in 32-bit words; FBP counts 2048-word (8 KiB) pages. */
    D_003EB820.fbp = displayBuffer->word >> SDF_GS_FRAMEBUFFER_PAGE_SHIFT;
    sceGsPutDispEnv(&D_003EB820);
}

INCLUDE_ASM(const s32, "sdf/sdfGraph", func_002D1020);


INCLUDE_ASM(const s32, "sdf/sdfGraph", func_002D1080);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_003BD2E8);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_003BD2E9);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", sdfCurrentBufferIndex);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", sdfGsImageUploadSemaphore);

