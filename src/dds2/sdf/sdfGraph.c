#include "common.h"
#include "eeregs.h"
#include "libsdf.h"
#include "draw.h"

#include "ps2asm.h"

typedef struct
{
    u64 pmode;
    u64 smode2;
    u32 dispfb;
    u32 dispfbHi;
    u64 display;
    u64 bgcolor;
} sceGsDispEnv;

extern void sceGsResetGraph(s16 aMode, s16 aInter, s16 aOmode, s16 aFfmode);
extern void sceGsSetDefDispEnv(sceGsDispEnv *aEnv, s32 aPsm, s32 aWidth, s32 aHeight, s32 aDx,
                               s32 aDy);
extern void sceGsPutDispEnv(sceGsDispEnv *aEnv);

extern void sdfGsMemFree(sdfGsMemSlice_t *aHandle);
extern sdfGsMemSlice_t *sdfGsAllocFrameBuff(s32 aW, s32 aH, s32 aPixelFormat);

extern sdfGrScreenDef_t sdfGrFrameModes[4];
extern sdfGrFrameSet_t sdfGrFrameSet;
extern sceGsDispEnv sdfGrDispEnv;

extern u8 reqMakeFrameBuff;
extern u8 sdfGrScreenMode;
extern u8 sdfGrGsResetDone;
extern s8 sdfDispFrameId[2];
extern u8 sdfFrameEvenOdd;
extern volatile u8 sdfReqDrawDelayCount;
extern s32 WakeupThread(s32 thread);
extern void sdfWaitVblank(s32 mode);
extern volatile s8 sdfGrDrawBusy;
extern s32 sdfGrCaptureReq;
/* The capture hook, held as an address and called through a cast. */
typedef void (*sdfGrCaptureFunc_t)(u8 *);

extern s32 sdfGrCaptureTarget;
extern s8 sdfGrCaptureBuff;

void sdfGrSetFrameSetMode(sdfGrFrameSet_t *aFrame, s32 aMode, s32 aReqHandleClear);
void sdfGrFreeFrameSet(sdfGrFrameSet_t *aFrame);

/* GS store-image packet and tracked-thread node are only passed to the SDK. */
typedef struct sceGsStoreImage sceGsStoreImage;
extern sceGsStoreImage sdfGrStoreImage;
extern sdfThCB_t sdfDispThreadCB;
extern u8 sdfDrawThreadStack[0x8000];
extern u8 sdfDispThreadStack[0x1000];
extern volatile u8 sdfDispThreadFlag;
extern volatile u8 sdfFrameId;
extern s8 reqGsReset;
extern volatile s32 dds3GsSema;
extern s32 dds3GsThreadId;
extern s32 sdfDrawThreadId;
extern volatile u8 sdfDrawSyncFlag;
extern volatile u8 sdfDrawFrameFlag;
extern u16 sdfDrawFrameCounter;
extern s32 iWakeupThread(s32 thread);
extern s32 DmacAddHandler(s32 type, s32 channel, s32 (*callback)(s32), s32 priority, s32 options);
extern s32 EnableDmac(s32 channel);
extern s32 GetThreadId(void);
extern s32 CancelWakeupThread(s32 thread);
extern s32 SleepThread(void);
extern s32 WaitSema(s32 sema);
extern s32 SignalSema(s32 sema);
extern u32 sdfGetRunTimeCountBase(void);
extern u32 sdfGetRunTimeCount(u32 aBaseTime);
extern sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);
extern void *sdfMemLock(sdfMemHandle_t *aHandle);
extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);
extern s32 sceGsSetDefStoreImage(sceGsStoreImage *, s16, s16, s16, s16, s16, s16, s16);
extern s32 sceGsExecStoreImage(sceGsStoreImage *, void *);
extern s32 sceGsSyncPath(s32 mode, s32 timeout);
extern void sceGsResetPath(void);
extern void *memcpy(void *a, const void *b, size_t c);
extern void sdfGrDrawFrame(void);
extern void sdfGrMakePacketBuff(s32 aPacketBuffSize);
extern void sdfGrInitDraw(void);
extern void sdfTexInit(void);
extern s32 sdfCreateSema(s32 initCount, s32 maxCount, s32 option);
extern s32 CreateThreadSimple(void *entry, void *stack, s32 stackSize, s32 priority);
#include "sdk_libkernl.h"

extern void sdfDevSignalPendingSemaphore(void);

extern void sdfDispThread(void);
extern void sdfGrSetupPacketBuff(s32 aFrameId);
extern void sdfCoreDrawReset(void);




void sdfGrSetFrameSetMode(sdfGrFrameSet_t *aFrame, s32 aMode, s32 aReqHandleClear)
{
    /* Target-specific allocation aid: retain the native row, destination and
     * depth-format homes. The read constraints order the live depth-format
     * value without adding accesses or extending it beyond its real store.
     * This is not evidence of the original source spelling. */
    FAKEMATCH_ASM_REG(sdfGrScreenDef_t *, def, "$2") = &sdfGrFrameModes[aMode];
    FAKEMATCH_ASM_REG(sdfGrFrameSet_t *, frame, "$10") = aFrame;
    s16 w;
    s16 h;
    s16 buffH;
    u16 pixelFormat;
    FAKEMATCH_ASM_REG(u16, zbuffFormat, "$9");

    if (aReqHandleClear != 0) {
        frame->frameHandle[0] = NULL;
        frame->frameHandle[1] = NULL;
        frame->zbuffHandle = NULL;
    }
    zbuffFormat = def->zbuffFormat;
    FAKEMATCH_ASM_READ(zbuffFormat);
    w = def->w;
    h = def->h;
    buffH = def->buffH;
    pixelFormat = def->pixelFormat;
    frame->w = w;
    frame->h = h;
    frame->buffH = buffH;
    FAKEMATCH_ASM_READ(zbuffFormat);
    frame->zbuffFormat = zbuffFormat;
    frame->pixelFormat = pixelFormat;
}

void sdfGrSetScreen(s32 aMode)
{
    sdfGrScreenMode = aMode;
    sdfGrSetFrameSetMode(&sdfGrFrameSet, aMode, 0);
    reqMakeFrameBuff = 1;
}

void sdfGrFreeFrameSet(sdfGrFrameSet_t *aFrame)
{
    sdfGsMemFree(aFrame->frameHandle[0]);
    aFrame->frameHandle[0] = NULL;
    sdfGsMemFree(aFrame->frameHandle[1]);
    aFrame->frameHandle[1] = NULL;
    sdfGsMemFree(aFrame->zbuffHandle);
    aFrame->zbuffHandle = NULL;
}

void sdfGrMakeFrameSet(sdfGrFrameSet_t *aFrame)
{
    s16 width;
    s16 height;
    u8 format;

    sdfGrFreeFrameSet(aFrame);
    width = aFrame->w;
    height = aFrame->buffH;
    aFrame->zbuffHandle = sdfGsAllocFrameBuff(width, height, aFrame->zbuffFormat);
    format = aFrame->pixelFormat;
    aFrame->frameHandle[0] = sdfGsAllocFrameBuff(width, height, format);
    aFrame->frameHandle[1] = sdfGsAllocFrameBuff(width, height, format);
}

void sdfGrSetDisp(s32 aEvenOdd)
{
    sdfGsMemSlice_t *buff = sdfGrFrameSet.frameHandle[aEvenOdd];
    u32 fbp;

    sceGsSetDefDispEnv((sceGsDispEnv *)EE_UNCACHED(EE_PHYSICAL(EE_ADDR(&sdfGrDispEnv))), 0,
                       sdfGrFrameSet.w, sdfGrFrameSet.buffH, 0, 0);
    fbp = (buff->addr >> 11) & 0x1FF;
    sdfGrDispEnv.dispfb = (sdfGrDispEnv.dispfb & ~0x1FF) | fbp;
    sceGsPutDispEnv(&sdfGrDispEnv);
}

void sdfGrGsReset(void)
{
    s32 inter;
    s32 ffmode;

    switch (sdfGrFrameModes[sdfGrScreenMode].fieldFrameMode) {
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
    sceGsResetGraph(sdfGrGsResetDone, inter, 2, ffmode);
    sdfGrGsResetDone = 1;
}

s32 sdfDrawThIntHandler(s32 channel) {
    if (channel == 1) {
        iWakeupThread(sdfDrawThreadId);
    }
    EE_SYNC_EI_BARRIER();
    return 0;
}

/* DDS defines the first four .sdata words here. */
u8 sdfGrScreenMode __attribute__((section(".sdata"))) = 0;
u8 sdfGrGsResetDone __attribute__((section(".sdata"))) = 0;
vu8 sdfFrameId __attribute__((section(".sdata"))) = 0;
volatile s32 dds3GsSema __attribute__((section(".sdata"))) = 0;

void sdfGrReqCapture(s32 aBuff, s32 aCallback)
{
    sdfGrCaptureReq = aBuff;
    sdfGrCaptureTarget = aCallback;
    sdfGrCaptureBuff = 1;
}

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

    if (*(u8 *)&sdfGrCaptureBuff) {
        width = sdfGrFrameSet.w;
        height = sdfGrFrameSet.buffH;
        format = sdfGrFrameSet.pixelFormat;
        pixelBytes = 4;
        if (format == 2) {
            pixelBytes = 2;
        }
        rowBytes = pixelBytes * width;
        stride = rowBytes * 2;
        sdfDispThreadFlag = 5;
        allocation = sdfMemAllocDirect(rowBytes * height);
        pixels = (u8 *)sdfMemLock(allocation);
        sceGsSetDefStoreImage(
            (sceGsStoreImage *)EE_UNCACHED(EE_PHYSICAL(EE_ADDR(&sdfGrStoreImage))),
            sdfGrFrameSet.frameHandle[1]->addr >> 6,
            width / 64, format, 0, 0, width, height);
        sceGsExecStoreImage(&sdfGrStoreImage, pixels);
        sceGsSyncPath(0, 0);
        destination = EE_ADDR_AS(u8 *, sdfGrCaptureReq);
        source = pixels;
        rows = height;
        do {
            memcpy(destination, source, rowBytes);
            destination += stride;
            source += rowBytes;
        } while (--rows != 0);
        sceGsSetDefStoreImage(
            (sceGsStoreImage *)EE_UNCACHED(EE_PHYSICAL(EE_ADDR(&sdfGrStoreImage))),
            sdfGrFrameSet.frameHandle[0]->addr >> 6,
            width / 64, format, 0, 0, width, height);
        sceGsExecStoreImage(&sdfGrStoreImage, pixels);
        sceGsSyncPath(0, 0);
        destination = EE_ADDR_AS(u8 *, sdfGrCaptureReq) + rowBytes;
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
            EE_ADDR_AS(sdfGrCaptureFunc_t, sdfGrCaptureTarget)(EE_ADDR_AS(u8 *, sdfGrCaptureReq));
        }
    }
}

void sdfDeliverDrawStart(s32 frameId, s32 buffId)
{
    if (sdfDispFrameId[0] == frameId) {
        sdfDispFrameId[0] = frameId ^ 1;
    }
    if (sdfDispFrameId[1] == frameId) {
        sdfDispFrameId[1] = frameId ^ 1;
    }
    SDF_FLAG_SET(&sdfGrDrawBusy, sdfGrDrawBusy, buffId);
}

void sdfDeliverDrawEnd(s32 mode, s32 frameId, s32 halfOffset) {
    if (mode == 0) {
        sdfDispFrameId[0] = frameId;
        sdfDispFrameId[1] = frameId;
    } else {
        sdfDispFrameId[halfOffset] = frameId;
    }
    SDF_FLAG_SET(&sdfGrDrawBusy, sdfGrDrawBusy, -1);
}

void sdfDrawThread(void) {
    u32 startTicks;

    sdfDispThreadFlag = 0;
    DmacAddHandler(1, 1, sdfDrawThIntHandler, -1, 0);
    EnableDmac(1);
    for (;;) {
        CancelWakeupThread(GetThreadId());
        sdfDispThreadFlag = 1;
        SleepThread();
        startTicks = sdfGetRunTimeCountBase();
        WaitSema(dds3GsSema);
        if (*(s8 *)&reqMakeFrameBuff != 0) {
            reqMakeFrameBuff = 0;
            sdfGrMakeFrameSet(&sdfGrFrameSet);
        }
        if (reqGsReset != 0) {
            reqGsReset = 0;
            sdfGrGsReset();
        }
        SDF_FLAG_SET(&sdfDrawSyncFlag, sdfDrawSyncFlag, 1);
        sdfGrDrawFrame();
        SDF_FLAG_SET(&sdfDrawSyncFlag, sdfDrawSyncFlag, 0);
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
        sdfDrawFrameCounter = sdfGetRunTimeCount(startTicks);
    }
}

void sdfDispThread(void) {
    u32 field;
    s32 bufferIndex;

    for (;;) {
        sdfWaitVblank(0);
        if ((sdfFrameEvenOdd & 1) == 0) {
            sdfDevSignalPendingSemaphore();
        }
        field = (~HW_READ(GS_CSR) >> 13) & 1;
        sdfFrameEvenOdd = field ^ 1;
        if (sdfDispThreadFlag == 1) {
            sdfDispThreadFlag = 2;
            WakeupThread(sdfDrawThreadId);
            WaitSema(dds3GsThreadId);
        }
        bufferIndex = sdfDispFrameId[sdfFrameEvenOdd];
        if (bufferIndex >= 0) {
            sdfGrSetDisp(bufferIndex);
        }
        if (sdfReqDrawDelayCount != 0) {
            SDF_FLAG_SET(&sdfReqDrawDelayCount, sdfReqDrawDelayCount,
                         sdfReqDrawDelayCount - 1);
        }
    }
}

void sdfGrInit(s32 aPacketBuffSize) {
    s32 thread;
    sdfGrFrameSet_t *graph = &sdfGrFrameSet;

    sceGsResetPath();
    sdfGrScreenMode = 1;
    sdfGrSetFrameSetMode(graph, 1, 1);
    sdfGrMakeFrameSet(graph);
    sdfGrGsResetDone = 0;
    sdfDispFrameId[0] = sdfDispFrameId[1] = -1;
    /* Plain: runs before the draw thread exists, so no waiter. */
    sdfGrDrawBusy = -1;
    reqGsReset = 1;
    sdfGrMakePacketBuff(aPacketBuffSize);
    sdfGrInitDraw();
    sdfTexInit();
    dds3GsSema = sdfCreateSema(1, 0x20, 0);
    dds3GsThreadId = sdfCreateSema(0, 0x20, 0);
    thread = CreateThreadSimple((void *)sdfDrawThread, sdfDrawThreadStack, 0x8000, 0x44);
    sdfDrawThreadId = thread;
    _StartThread(thread, 0);
    sdfStartThreadAndWait(&sdfDispThreadCB, sdfDispThread, sdfDispThreadStack,
                          0x1000, 0x40, 0);
}

void sdfGrInitNewFrame(void) {
    s8 nextBuffer = sdfFrameId ^ 1;

    /* Do not select a buffer while its index is the busy-buffer status. */
    SDF_FLAG_WAIT(&sdfGrDrawBusy, sdfGrDrawBusy == nextBuffer);
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

