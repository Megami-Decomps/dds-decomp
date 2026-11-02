#include "sdk_ps2.h"
#include "common.h"
#include "sdf_timer.h"
#include "sdf_chip.h"
#include "sdf_heap_api.h"
#include "sdf.h"
#include "sdf_pending.h"
#include "sdf_named_resource_request_api.h"
#include "sif_rpc_client_prefix.h"
#include "sdf_mem_block_state.h"
#include "ee_mmi.h"
#include "sdf_graph_display.h"
#include "sdf_graph_buffer_state.h"
#include "eeregs.h"
#include "sdf_texture_queue.h"
#include "sdf_image_upload.h"
#include "sdf_thread.h"
#include "sdf_chip_range_api.h"
#include "sdf_gs_mem_delayed_free_api.h"
#include "sdf_display_thread_state.h"
#include "sdf_draw_frame_execution_state.h"
#include "sdf_semaphore_api.h"
#include "sdf_texture_primary_buffer_api.h"
#include "sdf_texture_submit.h"
#include "sdf_texture_lists.h"
#include "sdf_texture_registry.h"
#include "sdf_texture_release.h"
#include "sdf_image_packets.h"
#include "sdf_tex_copy_image_data_api.h"
#include "sdf_vu_transform_api.h"
#include "sdf_handler_node.h"

#ifdef NON_MATCHING
#include "common.h"
#include "sdf.h"

extern SdfGraphModeDefaults sdfGrFrameModes[];

/* Copy a display mode's defaults into a graph target, optionally forgetting
 * its buffers. Upstream: INCLUDE_ASM in both games; same loads and
 * stores as the retail routine. */
void sdfGrSetFrameSetMode(sdfGrFrameSet_t *graph, s32 mode, s32 initializeBuffers) {
    const SdfGraphModeDefaults *defaults = &sdfGrFrameModes[mode];

    if (initializeBuffers) {
        graph->buffers[0] = NULL;
        graph->buffers[1] = NULL;
        graph->buffers[2] = NULL;
    }
    graph->width = defaults->width;
    graph->unk2 = defaults->unk2;
    graph->height = defaults->height;
    graph->bufferFormat = defaults->bufferFormat;
    graph->auxiliaryFormat = defaults->auxiliaryFormat;
}
#else
INCLUDE_ASM(const s32, "sdf/sdfGraph", sdfGrSetFrameSetMode);
#endif

extern s32 sdfDrawThreadId;

#define SDF_GRAPH_AUXILIARY_BUFFER 2

#define SDF_GRAPH_CPU_ADDRESS_MASK 0x0FFFFFFF

#define SDF_GRAPH_UNCACHED_ALIAS 0x20000000

#define SDF_GS_FRAMEBUFFER_PAGE_SHIFT 11

extern sdfGrFrameSet_t D_003980E0;

extern u8 sdfGrScreenMode;

extern s8 reqMakeFrameBuff;

void sdfGrSetScreen(s32 displayModeIndex) {
    sdfGrScreenMode = (u8)displayModeIndex;
    sdfGrSetFrameSetMode(&D_003980E0, displayModeIndex, 0);
    reqMakeFrameBuff = 1;
}

/* Release buffers in slot order, clearing each slot immediately afterward. */
void sdfGrFreeFrameSet(sdfGrFrameSet_t *graph) {
    sdfGsMemFree(graph->buffers[0]);
    graph->buffers[0] = NULL;
    sdfGsMemFree(graph->buffers[1]);
    graph->buffers[1] = NULL;
    sdfGsMemFree(graph->buffers[SDF_GRAPH_AUXILIARY_BUFFER]);
    graph->buffers[SDF_GRAPH_AUXILIARY_BUFFER] = NULL;
}

/* Rebuild the auxiliary buffer with its own PSM; both others share a PSM. */
void sdfGrMakeFrameSet(sdfGrFrameSet_t *graph) {
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
void sdfGrSetDisp(SdfGraphBufferIndex bufferIndex) {
    sdfGsMemSlice_t *displayBuffer = D_003980E0.buffers[bufferIndex];

    /* Initialize the environment through its uncached main-RAM alias. */
    sceGsSetDefDispEnv((void *)(((u32)&sdfDisplayEnvironment & SDF_GRAPH_CPU_ADDRESS_MASK) | SDF_GRAPH_UNCACHED_ALIAS),
                      0, D_003980E0.width, D_003980E0.height, 0, 0);
    /* VRAM offsets are in 32-bit words; FBP counts 2048-word (8 KiB) pages. */
    sdfDisplayEnvironment.framebufferPage = displayBuffer->word >> SDF_GS_FRAMEBUFFER_PAGE_SHIFT;
    sceGsPutDispEnv(&sdfDisplayEnvironment);
}

extern SdfGraphModeDefaults sdfGrFrameModes[];

extern u8 D_003BD2E9;

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
    sceGsResetGraph(D_003BD2E9, inter, 2, ffmode);
    D_003BD2E9 = 1;
}

s32 sdfDrawThIntHandler(s32 channel) {
    if (channel == 1) {
        iWakeupThread(sdfDrawThreadId);
    }
    EE_ENABLE_INTERRUPTS_SYNC();
    return 0;
}

u8 sdfGrScreenMode __attribute__((section(".sdata"))) = 0;

u8 D_003BD2E9 __attribute__((section(".sdata"))) = 0;

vu8 sdfFrameId __attribute__((section(".sdata"))) = 0;

volatile s32 dds3GsSema __attribute__((section(".sdata"))) = 0;

/* GS pixel-storage modes, using the same private names as sdfTex.c. */
enum {
    SDF_PSMCT32 = 0x0, SDF_PSMCT24 = 0x1, SDF_PSMCT16 = 0x2,
    SDF_PSMCT16S = 0xA, SDF_PSMT8 = 0x13, SDF_PSMT4 = 0x14,
    SDF_PSMT8H = 0x1B, SDF_PSMT4HL = 0x24, SDF_PSMT4HH = 0x2C,
    SDF_PSMZ32 = 0x30, SDF_PSMZ24 = 0x31, SDF_PSMZ16 = 0x32,
    SDF_PSMZ16S = 0x3A
};

extern SdfGraphBufferState sdfDispFrameId[2];

/* Busy-buffer index is published by the slot setters and polled below;
 * volatile prevents the wait loop from reusing an earlier read. */
extern volatile SdfGraphBufferState sdfGrDrawBusy;

extern u8 sdfGrCaptureBuff;

extern u8 *sdfGrCaptureReq;

extern void (*sdfGrCaptureTarget)(u8 *);

extern void (*sdfGsErrorHandler)(s32 size, s32 allocationMode);

struct sdfGsMemSlice_t *sdfGsMemAllocFromHead(s32 size, s32 allocationMode);

void sdfGrReqCapture(u8 *destination, void (*onComplete)(u8 *)) {
    sdfGrCaptureReq = destination;
    sdfGrCaptureTarget = onComplete;
    sdfGrCaptureBuff = 1;
}

extern sceGsStoreImage sdfGsCaptureStoreImage;

extern volatile SdfDisplayThreadState sdfDispThreadFlag;

/* Interleave rows from both color buffers into the queued destination. */
void sdfGrCapture(void) {
    s32 width;
    s32 height;
    s32 format;
    s32 pixelBytes;
    s32 rowBytes;
    s32 stride;
    sdfMemHandle_t *allocation;
    s32 rows;
    u8 *pixels;
    u8 *source;
    u8 *destination;

    if (sdfGrCaptureBuff) {
        width = D_003980E0.width;
        height = D_003980E0.height;
        format = D_003980E0.bufferFormat;
        pixelBytes = 4;
        if (format == SDF_PSMCT16) {
            pixelBytes = 2;
        }
        rowBytes = pixelBytes * width;
        stride = rowBytes * 2;
        sdfDispThreadFlag = SDF_DISPLAY_THREAD_CAPTURING_FRAME;
        allocation = sdfMemAllocDirect(rowBytes * height);
        pixels = (u8 *)sdfMemLock(allocation);
        sceGsSetDefStoreImage(
            (sceGsStoreImage *)(((u32)&sdfGsCaptureStoreImage & 0x0FFFFFFF) | 0x20000000),
            D_003980E0.buffers[SDF_GRAPH_BUFFER_1]->word >> 6,
            width / 64, format, 0, 0, width, height);
        sceGsExecStoreImage(&sdfGsCaptureStoreImage, pixels);
        sceGsSyncPath(0, 0);
        destination = sdfGrCaptureReq;
        source = pixels;
        rows = height;
        do {
            memcpy(destination, source, rowBytes);
            destination += stride;
            source += rowBytes;
        } while (--rows != 0);
        sceGsSetDefStoreImage(
            (sceGsStoreImage *)(((u32)&sdfGsCaptureStoreImage & 0x0FFFFFFF) | 0x20000000),
            D_003980E0.buffers[SDF_GRAPH_BUFFER_0]->word >> 6,
            width / 64, format, 0, 0, width, height);
        sceGsExecStoreImage(&sdfGsCaptureStoreImage, pixels);
        sceGsSyncPath(0, 0);
        destination = sdfGrCaptureReq + rowBytes;
        source = pixels;
        rows = height;
        do {
            memcpy(destination, source, rowBytes);
            destination += stride;
            source += rowBytes;
        } while (--rows != 0);
        sdfMemFreeDirect(allocation);
        sdfGrCaptureBuff = 0;
        if (sdfGrCaptureTarget) {
            sdfGrCaptureTarget(sdfGrCaptureReq);
        }
    }
}

/* Switch both references off the finished double-buffer slot before
 * publishing the slot currently in use. */
void sdfDeliverDrawStart(SdfGraphBufferIndex oldBuffer, SdfGraphBufferIndex nextBuffer) {
    if (sdfDispFrameId[0] == oldBuffer) {
        sdfDispFrameId[0] = oldBuffer ^ SDF_GRAPH_BUFFER_1;
    }
    if (sdfDispFrameId[1] == oldBuffer) {
        sdfDispFrameId[1] = oldBuffer ^ SDF_GRAPH_BUFFER_1;
    }
    sdfGrDrawBusy = nextBuffer;
}

/* Update one slot or both slots, then clear the busy-buffer status. */
void sdfDeliverDrawEnd(s32 updateSingleSlot, SdfGraphBufferIndex bufferIndex, s32 slotIndex) {
    if (updateSingleSlot == 0) {
        sdfDispFrameId[0] = bufferIndex;
        sdfDispFrameId[1] = bufferIndex;
    } else {
        sdfDispFrameId[slotIndex] = bufferIndex;
    }
    sdfGrDrawBusy = SDF_GRAPH_BUFFER_NONE;
}

extern s8 reqGsReset;

extern volatile s32 dds3GsSema;

extern volatile SdfDrawFrameExecutionState sdfDrawSyncFlag;

extern volatile u8 sdfDrawFrameFlag;

extern u16 D_003BD32E;

extern SdfHandlerNode *DmacAddHandler(SdfHandlerKind, s32, SdfHandlerCallback, s32, s32);

extern void sdfGrMakeFrameSet(sdfGrFrameSet_t *);

extern void sdfGrGsReset(void);

extern void sdfGrDrawFrame(void);

extern s32 sdfDrawThIntHandler(s32);

void sdfDrawThread(void) {
    u32 startTicks;

    sdfDispThreadFlag = SDF_DISPLAY_THREAD_STARTUP;
    DmacAddHandler(SDF_HANDLER_KIND_DMA, 1, sdfDrawThIntHandler, -1, 0);
    EnableDmac(1);
    for (;;) {
        CancelWakeupThread(GetThreadId());
        sdfDispThreadFlag = SDF_DISPLAY_THREAD_WAITING_FOR_VBLANK;
        SleepThread();
        startTicks = sdfGetRunTimeCountBase();
        WaitSema(dds3GsSema);
        if (reqMakeFrameBuff != 0) {
            reqMakeFrameBuff = 0;
            sdfGrMakeFrameSet(&D_003980E0);
        }
        if (reqGsReset != 0) {
            reqGsReset = 0;
            sdfGrGsReset();
        }
        sdfDrawSyncFlag = SDF_DRAW_FRAME_IN_PROGRESS;
        sdfGrDrawFrame();
        sdfDrawSyncFlag = SDF_DRAW_FRAME_IDLE;
        if (sdfDrawFrameFlag != 0) {
            /* Preserve the controller enable state while stopping VIF1. */
            u32 enabledChannels = HW_READ(D_ENABLER);
            HW_WRITE(D_ENABLEW, 0x10000);
            HW_WRITE(D1_CHCR, 0);
            HW_WRITE(D_ENABLEW, enabledChannels);
            sceGsResetPath();
            HW_WRITE(GS_BUSDIR, 0);
            sdfDrawFrameFlag = 1;
        } else {
            sdfGrCapture();
        }
        SignalSema(dds3GsSema);
        D_003BD32E = sdfGetRunTimeCount(startTicks);
    }
}

extern u8 sdfFrameEvenOdd;

extern s32 dds3GsThreadId;

extern volatile u8 sdfReqDrawDelayCount;

extern void sdfWaitVblank(s32);

extern void sdfDevSignalPendingSemaphore(void);

extern void sdfGrSetDisp(s32);

/* GS CSR bit 13 supplies the double-buffer field selector. */

#define SDF_GS_CSR_FIELD_SHIFT 13

/* Service device requests, select the GS field's buffer, and age packet slots.
 * Capture state 1 wakes its worker; state 2 precedes the semaphore wait. */
void sdfServiceGraphicsBuffers(void) {
    SdfGraphBufferState bufferIndex;
    s32 invertedField;

    for (;;) {
        sdfWaitVblank(0);
        if ((sdfFrameEvenOdd & 1) == 0) {
            sdfDevSignalPendingSemaphore();
        }
        invertedField = (~HW_READ(GS_CSR) >> SDF_GS_CSR_FIELD_SHIFT) & 1;
        sdfFrameEvenOdd = invertedField ^ 1;
        if (sdfDispThreadFlag == SDF_DISPLAY_THREAD_WAITING_FOR_VBLANK) {
            sdfDispThreadFlag = SDF_DISPLAY_THREAD_WAKE_REQUESTED;
            WakeupThread(sdfDrawThreadId);
            WaitSema(dds3GsThreadId);
        }
        bufferIndex = sdfDispFrameId[sdfFrameEvenOdd];
        if (bufferIndex >= 0) {
            sdfGrSetDisp(bufferIndex);
        }
        if (sdfReqDrawDelayCount != 0) {
            sdfReqDrawDelayCount--;
        }
    }
}

extern s32 CreateThreadSimple(void (*)(), void *, s32, s32);

extern SdfThreadNode sdfGraphicsBufferThread;

extern u8 sdfGraphicsTransferThreadStack[0x8000];

extern u8 sdfGraphicsBufferThreadStack[0x1000];

extern void sdfGrMakePacketBuff(s32);

extern void sdfGrInitDraw(void);

extern void sdfTexInit(void);

/* Reset GS/buffer state and start graphics workers; packetBufferBytes sizes each packet-buffer half. */
void sdfGrInit(s32 packetBufferBytes) {
    s32 thread;
    sdfGrFrameSet_t *graph = &D_003980E0;

    sceGsResetPath();
    sdfGrScreenMode = 1;
    sdfGrSetFrameSetMode(graph, 1, 1);
    sdfGrMakeFrameSet(graph);
    D_003BD2E9 = 0;
    sdfDispFrameId[0] = sdfDispFrameId[1] = SDF_GRAPH_BUFFER_NONE;
    sdfGrDrawBusy = SDF_GRAPH_BUFFER_NONE;
    reqGsReset = 1;
    sdfGrMakePacketBuff(packetBufferBytes);
    sdfGrInitDraw();
    sdfTexInit();
    dds3GsSema = sdfCreateSema(1, 0x20, 0);
    dds3GsThreadId = sdfCreateSema(0, 0x20, 0);
    thread = CreateThreadSimple(sdfDrawThread, sdfGraphicsTransferThreadStack, 0x8000, 0x44);
    sdfDrawThreadId = thread;
    _StartThread(thread, 0);
    sdfStartThreadAndWait(&sdfGraphicsBufferThread, sdfServiceGraphicsBuffers, sdfGraphicsBufferThreadStack,
                          0x1000, 0x40, 0);
}

extern vu8 sdfFrameId;

extern void sdfGrSetupPacketBuff(s32 bufferIndex);

/* Wait until the other buffer is no longer busy before selecting it. */
void sdfGrInitNewFrame(void) {
    s8 nextBuffer = sdfFrameId ^ 1;

    /* Do not select a buffer while its index is the busy-buffer status. */
    while (sdfGrDrawBusy == nextBuffer) {
    }
    sdfFrameId = nextBuffer;
    sdfGrSetupPacketBuff((s8)sdfFrameId);
    sdfCoreDrawReset();
}

INCLUDE_SDATA(const s32, "sdf/sdfGraph", sdfGrCaptureBuff);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", sdfDispThreadFlag);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", sdfGrCaptureReq);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", sdfGrCaptureTarget);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", dds3GsThreadId);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", sdfDispFrameId);

INCLUDE_SDATA(const s32, "sdf/sdfGraph", sdfGrDrawBusy);

