#include "common.h"
#include "sdf.h"

#define SDF_GRAPH_AUXILIARY_BUFFER 2
#define SDF_GRAPH_CPU_ADDRESS_MASK 0x0FFFFFFF
#define SDF_GRAPH_UNCACHED_ALIAS 0x20000000
#define SDF_GS_FRAMEBUFFER_PAGE_SHIFT 11


extern u8 D_004389D8;

extern u8 D_0043913C;

void func_00329CE0(void *arg0, s32 arg1, s32 arg2);

extern SdfGraphObj D_0040B290;

void sdfUpdateTextureHeadsWithInterruptsMasked(void *arg0);

void *sdfAllocImageBuffer(s32 arg0, s32 arg1, s32 arg2);

void sdfGraphSetDisplayMode(s32 mode) {
    D_004389D8 = (u8)mode;
    func_00329CE0(&D_0040B290, mode, 0);
    D_0043913C = 1;
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

extern SdfDisplayEnv D_004681D0;
extern void sceGsSetDefDispEnv(void *, s32, s32, s32, s32, s32);
extern void sceGsPutDispEnv(void *);

/* Select the indexed image buffer for display; the index and pointer are unchecked. */
void sdfGraphSelectDisplayBuffer(s32 bufferIndex) {
    SdfTexResource *displayBuffer = D_0040B290.buffers[bufferIndex];

    /* Initialize the environment through its uncached main-RAM alias. */
    sceGsSetDefDispEnv((void *)(((u32)&D_004681D0 & SDF_GRAPH_CPU_ADDRESS_MASK) | SDF_GRAPH_UNCACHED_ALIAS),
                      0, D_0040B290.width, D_0040B290.height, 0, 0);
    /* VRAM offsets are in 32-bit words; FBP counts 2048-word (8 KiB) pages. */
    D_004681D0.fbp = displayBuffer->word >> SDF_GS_FRAMEBUFFER_PAGE_SHIFT;
    sceGsPutDispEnv(&D_004681D0);
}

INCLUDE_ASM(const s32, "sdf/sdfGraph", func_00329ED0);

INCLUDE_ASM(const s32, "sdf/sdfGraph", func_00329F30);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_004389D8);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", D_004389D9);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", sdfCurrentBufferIndex);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", sdfGsImageUploadSemaphore);

