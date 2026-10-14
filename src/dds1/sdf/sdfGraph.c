#include "common.h"
#include "sdf.h"
#include "ee_mmi.h"
#include "sdf_graph_display.h"

extern s32 D_003BD9D8;
extern s32 iWakeupThread(s32 threadId);

#define SDF_GRAPH_AUXILIARY_BUFFER 2
#define SDF_GRAPH_CPU_ADDRESS_MASK 0x0FFFFFFF
#define SDF_GRAPH_UNCACHED_ALIAS 0x20000000
#define SDF_GS_FRAMEBUFFER_PAGE_SHIFT 11

extern SdfGraphObj D_003980E0;
extern u8 D_003BD2E8;
extern u8 D_003BD9DC;


void sdfGraphSetDisplayMode(s32 arg0) {
    D_003BD2E8 = (u8)arg0;
    sdfGraphApplyModeDefaults(&D_003980E0, arg0, 0);
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

extern SdfDisplayEnvView D_003EB820;
extern void sceGsSetDefDispEnv(void *, s32, s32, s32, s32, s32);
extern void sceGsPutDispEnv(void *);

/* Select the indexed image buffer for display; the index and pointer are unchecked. */
void sdfGraphSelectDisplayBuffer(s32 bufferIndex) {
    SdfTexResource *displayBuffer = D_003980E0.buffers[bufferIndex];

    /* Initialize the environment through its uncached main-RAM alias. */
    sceGsSetDefDispEnv((void *)(((u32)&D_003EB820 & SDF_GRAPH_CPU_ADDRESS_MASK) | SDF_GRAPH_UNCACHED_ALIAS),
                      0, D_003980E0.width, D_003980E0.height, 0, 0);
    /* VRAM offsets are in 32-bit words; FBP counts 2048-word (8 KiB) pages. */
    D_003EB820.framebufferPage = displayBuffer->word >> SDF_GS_FRAMEBUFFER_PAGE_SHIFT;
    sceGsPutDispEnv(&D_003EB820);
}

extern SdfGraphModeDefaults D_003980B0[];
extern u8 D_003BD2E9;
extern void sceGsResetGraph(s32 mode, s32 inter, s32 omode, s32 ffmode);

/* Reset the GS with the interlace setting of the current display mode. */
void sdfGraphResetDeviceForCurrentMode(void) {
    s32 inter;
    s32 ffmode;
    u16 interlace = D_003980B0[D_003BD2E8].interlace;

    switch (interlace) {
    case 0:
        inter = 0;
        ffmode = 1;
        break;
    case 1:
        inter = 1;
        ffmode = 0;
        break;
    default:
        inter = 1;
        ffmode = 1;
        break;
    }
    sceGsResetGraph(D_003BD2E9, inter, 2, ffmode);
    D_003BD2E9 = 1;
}


s32 sdfGraphWakeDisplayThread(s32 channel) {
    if (channel == 1) {
        iWakeupThread(D_003BD9D8);
    }
    EE_ENABLE_INTERRUPTS_SYNC();
    return 0;
}

u8 D_003BD2E8 __attribute__((section(".sdata"))) = 0;

u8 D_003BD2E9 __attribute__((section(".sdata"))) = 0;

vu8 sdfCurrentBufferIndex __attribute__((section(".sdata"))) = 0;

volatile s32 sdfGsImageUploadSemaphore __attribute__((section(".sdata"))) = 0;

