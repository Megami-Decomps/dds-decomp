#include "sdk_ps2.h"
#include "common.h"
#include "sdf.h"
#include "ee_mmi.h"
#include "sdf_graph_display.h"

extern s32 sdfDrawThreadId;

#define SDF_GRAPH_AUXILIARY_BUFFER 2
#define SDF_GRAPH_CPU_ADDRESS_MASK 0x0FFFFFFF
#define SDF_GRAPH_UNCACHED_ALIAS 0x20000000
#define SDF_GS_FRAMEBUFFER_PAGE_SHIFT 11


extern u8 sdfGrScreenMode;

extern u8 reqMakeFrameBuff;

extern SdfGraphObj D_0040B290;



void sdfGrSetScreen(s32 displayModeIndex) {
    sdfGrScreenMode = (u8)displayModeIndex;
    sdfGrSetFrameSetMode(&D_0040B290, displayModeIndex, 0);
    reqMakeFrameBuff = 1;
}

/* Release buffers in slot order, clearing each slot immediately afterward. */
void sdfGrFreeFrameSet(SdfGraphObj *graph) {
    sdfGsMemFree(graph->buffers[0]);
    graph->buffers[0] = NULL;
    sdfGsMemFree(graph->buffers[1]);
    graph->buffers[1] = NULL;
    sdfGsMemFree(graph->buffers[SDF_GRAPH_AUXILIARY_BUFFER]);
    graph->buffers[SDF_GRAPH_AUXILIARY_BUFFER] = NULL;
}

/* Rebuild the auxiliary buffer with its own PSM; both others share a PSM. */
void sdfGrMakeFrameSet(SdfGraphObj *graph) {
    s16 width;
    s16 height;
    u8 bufferFormat;

    sdfGrFreeFrameSet(graph);
    width = graph->width;
    height = graph->height;
    graph->buffers[SDF_GRAPH_AUXILIARY_BUFFER] = sdfGsAllocFrameBuff(width, height, graph->auxiliaryFormat);
    bufferFormat = graph->bufferFormat;
    graph->buffers[0] = sdfGsAllocFrameBuff(width, height, bufferFormat);
    graph->buffers[1] = sdfGsAllocFrameBuff(width, height, bufferFormat);
}

extern SdfDisplayEnvView sdfDisplayEnvironment;

/* Select the indexed image buffer for display; the index and pointer are unchecked. */
void sdfGrSetDisp(s32 bufferIndex) {
    SdfTexResource *displayBuffer = D_0040B290.buffers[bufferIndex];

    /* Initialize the environment through its uncached main-RAM alias. */
    sceGsSetDefDispEnv((void *)(((u32)&sdfDisplayEnvironment & SDF_GRAPH_CPU_ADDRESS_MASK) | SDF_GRAPH_UNCACHED_ALIAS),
                      0, D_0040B290.width, D_0040B290.height, 0, 0);
    /* VRAM offsets are in 32-bit words; FBP counts 2048-word (8 KiB) pages. */
    sdfDisplayEnvironment.framebufferPage = displayBuffer->word >> SDF_GS_FRAMEBUFFER_PAGE_SHIFT;
    sceGsPutDispEnv(&sdfDisplayEnvironment);
}

extern SdfGraphModeDefaults sdfGrFrameModes[];
extern u8 D_004389D9;

/* Reset the GS with the interlace setting of the current display mode. */
void sdfGrGsReset(void) {
    s32 inter;
    s32 ffmode;
    u16 interlace = sdfGrFrameModes[sdfGrScreenMode].interlace;

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
    sceGsResetGraph(D_004389D9, inter, 2, ffmode);
    D_004389D9 = 1;
}

s32 sdfDrawThIntHandler(s32 channel) {
    if (channel == 1) {
        iWakeupThread(sdfDrawThreadId);
    }
    EE_ENABLE_INTERRUPTS_SYNC();
    return 0;
}

u8 sdfGrScreenMode __attribute__((section(".sdata"))) = 0;

u8 D_004389D9 __attribute__((section(".sdata"))) = 0;

vu8 sdfFrameId __attribute__((section(".sdata"))) = 0;

volatile s32 dds3GsSema __attribute__((section(".sdata"))) = 0;

