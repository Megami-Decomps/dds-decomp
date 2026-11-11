#include "common.h"
#include "sdk_libgcc.h"
#include "kernel.h"
#include "scr.h"
#include "ps2asm.h"
#include "sdfFrame.h"


static inline f32 scrFabsf(f32 x)
{
    f32 r;
    r = sdfFabs(x);
    return r;
}

typedef struct
{
    f32 x;
    f32 y;
    f32 z;
    s32 w;
} scrVecW_t;

typedef struct
{
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} scrVec4_t;

extern s8 dds3PadBitTable[];

#include "sdk_libc.h"

extern void datDebugPrintf(const char *fmt, ...);


extern s32 scrGetIntPara(s32 n);
extern f32 scrGetFloatPara(s32 n);
extern char *scrGetStrPara(s32 n);
extern void scrSetIntReturnValue(s32 i);
extern s32 scrGetCommandTimer(void);
extern s32 scrGetLabelCodeAddress(s32 labelindex);
extern void scrSetCodePointer(s32 cp);
extern scrProcessWork_s *scrGetScriptHandle(void);
extern dds3ProcessID_t *scrRunScriptEx(s32 priority, void *header, void *typeTable, void *procTable,
                          void *labelTable, void *code, void *messageData, void *stringData,
                          s32 startProc);

extern s32 evtBitCheck(s32 no);
extern void evtBitOn(s32 no);
extern void evtBitOff(s32 no);

extern s32 dds3GetRandIntA(s32 aLimit, s32 name);
extern void dds3FadeIn(s32 r, s32 g, s32 b, s32 frame);
extern void dds3FadeOut(s32 r, s32 g, s32 b, s32 frame);
extern void dds3PadVibrationSet(s32 port, s32 motor, s32 level);
extern void dds3SetScreenFovy(f32 fovy);
extern s32 dds3ExistProcess(dds3ProcessID_t *id);
extern s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);
extern u32 dds3GetProcessRunTime(dds3ProcessID_t *id);
extern void dds3SetQuake(s32 frame, s32 range);
extern void dds3StopQuake(void);
extern void dds3SetLightColor(s32 frame, s32 no, void *col);
extern void dds3SetLightDir(s32 frame, s32 no, void *dir);
extern void dds3SetAmbientColor(s32 frame, void *col);
extern void dds3SetFogColor(s32 frame, f32 *col);
extern void dds3SetFogPara(s32 frame, f32 max, f32 min, f32 near, f32 far);
extern void dds3SetSoftenBias(s32 frame, f32 soften, f32 bias);
extern void dds3SetBlur(s32 frame, s32 alpha, s32 scale);

extern void dds3SetMultiBlurPara(f32 rotate, f32 scale, u32 blend);
extern void dds3SetMultiBlurRGBA(s32 rgba);
extern void dds3SetMultiBlurArea(s32 area, s32 posx, s32 posy);
extern void dds3MultiBlurSwap(s32 frame);
extern void dds3MultiBlurOn(s32 frame);
extern void dds3MultiBlurOff(s32 frame);

extern void dds3SetDistBlurPara(s32 number, s32 width, s32 intval, f32 speed, f32 rotate, f32 distor,
                                s32 blend);
extern void dds3SetDistBlurRGBA(s32 rgba);
extern void dds3SetDistBlurArea(s32 area, s32 posx, s32 posy);
extern void dds3DistBlurSwap(s32 frame);
extern void dds3DistBlurOn(s32 frame);
extern void dds3DistBlurOff(s32 frame);

extern void dds3SetRipBlurPara(s32 number, f32 speed, f32 intval, f32 rotate, f32 distor, f32 distor_s,
                               s32 blend);
extern void dds3SetRipBlurRGBA(s32 rgba);
extern void dds3SetRipBlurArea(s32 area, s32 posx, s32 posy);
extern void dds3RipBlurSwap(s32 frame);
extern void dds3RipBlurOn(s32 frame);
extern void dds3RipBlurOff(s32 frame);

extern void dds3SetBlur2Para(f32 rotate, f32 scale, u32 blend);
extern void dds3SetBlur2RGBA(s32 rgba);
extern void dds3SetBlur2Area(s32 posx, s32 posy);
extern void dds3Blur2Swap(s32 frame);
extern void dds3Blur2On(s32 frame);
extern void dds3Blur2Off(s32 frame);

extern void dds3SetFilter2Para(s32 blend);
extern void dds3SetFilter2RGBA(s32 rgba);
extern void dds3Filter2Swap(s32 frame);
extern void dds3Filter2On(s32 frame);
extern void dds3Filter2Off(s32 frame);

extern void dds3SetMFilterPara(s32 blend);
extern void dds3SetMFilterRGBA(s32 rgba);
extern void dds3SetMFilterArea(s32 area, s32 posx, s32 posy);
extern void dds3MFilterSwap(s32 frame);
extern void dds3MFilterOn(s32 frame);
extern void dds3MFilterOff(s32 frame);

/* DDS's global work keeps the script variables at SMT3's offsets. */
typedef struct
{
    u8 unk0[0x40];
    s32 integers[256];
    f32 floats[256];
} dds3GlobalWork_t;
extern dds3GlobalWork_t *DDS3_GBWK;

#define SCR_PROCESS_CONTROL_FLAG 210

extern void dds3FadeBackgroundStartOut(s32 duration);
extern void dds3FadeBackgroundStartIn(s32 duration);
extern void effDisableFramebufferQuad(void);
extern void fldSetFilterAnime(u32 mode);
extern void fldSkySetAlpha(u32 alpha);
extern void fldSkySetAlphaOfs(u32 alpha);
extern void fldSetFadeTarget(s32 area, s32 mode, s32 frame);
extern void ptyAdjustItemQuantity(s32 itemId, s32 delta);
extern void datAddCurrencyClamped(s32 delta);
extern u8 datHasEnoughCurrency(s32 amount);

s32 scrCommand_KEY(void)
{
    if (scrGetCommandTimer() == 0)
    {
        return 0;
    }
    if (dds3PadBitTable[1] < 0)
    {
        return 1;
    }
    return 0;
}

s32 scrCommand_BIT_CHK(void)
{
    scrSetIntReturnValue(evtBitCheck(scrGetIntPara(0)));
    return 1;
}

s32 scrCommand_BIT_ON(void)
{
    evtBitOn(scrGetIntPara(0));
    return 1;
}

s32 scrCommand_BIT_OFF(void)
{
    evtBitOff(scrGetIntPara(0));
    return 1;
}

s32 scrCommand_RND(void)
{
    scrSetIntReturnValue(dds3GetRandIntA(0, scrGetIntPara(0)) + 1);
    return 1;
}

s32 scrCommand_SYNC(void)
{
    return scrGetCommandTimer() != 0;
}

s32 scrCommand_WAIT(void)
{
    if (scrGetIntPara(0) <= 0)
    {
        return 1;
    }
    if (scrGetCommandTimer() < SDF_FRAMES(scrGetIntPara(0)))
    {
        return 0;
    }
    return 1;
}

s32 scrCommand_PUT(void)
{
    datDebugPrintf("PUT -> %d\n", scrGetIntPara(0));
    return 1;
}

s32 scrCommand_PUTS(void)
{
    datDebugPrintf("PUTSTR -> %s\n", scrGetStrPara(0));
    return 1;
}

s32 scrCommand_FADE_IN(void)
{
    s32 colorMode;

    if (scrGetCommandTimer() == 0)
    {
        colorMode = scrGetIntPara(0);
        switch (colorMode)
        {
        case 0:
            dds3FadeIn(0, 0, 0, scrGetIntPara(1));
            break;
        case 1:
            dds3FadeIn(0xFF, 0xFF, 0xFF, scrGetIntPara(1));
            break;
        default:
            return 1;
        }
        return 0;
    }
    return 1;
}

s32 scrCommand_FADE_OUT(void)
{
    s32 colorMode;

    if (scrGetCommandTimer() == 0)
    {
        colorMode = scrGetIntPara(0);
        switch (colorMode)
        {
        case 0:
            dds3FadeOut(0, 0, 0, scrGetIntPara(1));
            break;
        case 1:
            dds3FadeOut(0xFF, 0xFF, 0xFF, scrGetIntPara(1));
            break;
        default:
            return 1;
        }
        return 0;
    }
    return 1;
}

s32 scrCommandFadeBackgroundOut(void)
{
    if (scrGetCommandTimer() == 0)
    {
        dds3FadeBackgroundStartOut(scrGetIntPara(0));
        return 0;
    }
    return 1;
}

s32 scrCommandFadeBackgroundIn(void)
{
    if (scrGetCommandTimer() == 0)
    {
        dds3FadeBackgroundStartIn(scrGetIntPara(0));
        return 0;
    }
    return 1;
}

s32 scrCommand_SEL_JMP(void)
{
    s32 argumentIndex;
    s32 labelIndex;

    argumentIndex = scrGetIntPara(0);
    if (argumentIndex < 0)
    {
        return 1;
    }
    labelIndex = scrGetIntPara(argumentIndex + 1);
    if (labelIndex < 0)
    {
        return 1;
    }
    scrSetCodePointer(scrGetLabelCodeAddress(labelIndex));
    return 1;
}

s32 scrCommand_PAD_TRIG(void)
{
    scrSetIntReturnValue(dds3PadBitTable[scrGetIntPara(0)] < 0);
    return 1;
}

s32 scrCommand_PAD_PRESS(void)
{
    scrSetIntReturnValue(dds3PadBitTable[scrGetIntPara(0)] & 1);
    return 1;
}

s32 scrCommand_PAD_ACT(void)
{
    s32 port;
    s32 motor;

    port = scrGetIntPara(0);
    motor = scrGetIntPara(1);
    dds3PadVibrationSet(port, motor & 0xFF, scrGetIntPara(2));
    return 1;
}

s32 scrCommand_SET_FOVY(void)
{
    f32 fovy;

    fovy = scrGetFloatPara(0) * 0.017453293f;
    if (fovy <= 5.0f || fovy >= 180.0f)
    {
        datDebugPrintf("WARNING: SET_FOVY over = %f\n", fovy);
        return 1;
    }
    dds3SetScreenFovy(fovy);
    return 1;
}

s32 scrCommand_SCR_RUN(void)
{
    scrProcessWork_s *work;
    s32 startProc;

    startProc = scrGetIntPara(0);
    work = scrGetScriptHandle();
    if (work == NULL)
    {
        return 1;
    }
    if (work->ProcessID == NULL)
    {
        return 1;
    }
    if (startProc < 0 || startProc >= work->TypeTable->ItemCount)
    {
        return 1;
    }
    scrSetIntReturnValue(EE_ADDR_S(scrRunScriptEx(work->ProcessID->priority + scrGetIntPara(1),
                                        work->Header, work->TypeTable, work->ProcTable,
                                        work->LabelTable, work->Code, work->MessageData,
                                        work->StringData, startProc)));
    return 1;
}

s32 scrCommand_SCR_KILL(void)
{
    s32 taskHandle;

    taskHandle = scrGetIntPara(0);
    if (dds3ExistProcess(EE_ADDR_AS(dds3ProcessID_t *, taskHandle)) == 0)
    {
        return 1;
    }
    dds3KillProcess(EE_ADDR_AS(dds3ProcessID_t *, taskHandle), 1);
    return 1;
}

s32 scrCommand_SCR_WAIT(void)
{
    return dds3ExistProcess(EE_ADDR_AS(dds3ProcessID_t *, scrGetIntPara(0))) == 0;
}

s32 scrCommand_SCR_STAT(void)
{
    if (dds3ExistProcess(EE_ADDR_AS(dds3ProcessID_t *, scrGetIntPara(0))) != 0)
    {
        scrSetIntReturnValue(1);
    }
    else
    {
        scrSetIntReturnValue(0);
    }
    return 1;
}

s32 scrCommand_SCR_TIME(void)
{
    dds3ProcessID_t *id;

    id = EE_ADDR_AS(dds3ProcessID_t *, scrGetIntPara(0));
    if (dds3ExistProcess(id) == 0)
    {
        scrSetIntReturnValue(0);
    }
    else
    {
        scrSetIntReturnValue(SDF_FRAME_TICK(dds3GetProcessRunTime(id)));
    }
    return 1;
}

s32 scrCommand_QUAKE_ON(void)
{
    s32 frames;

    frames = scrGetIntPara(0);
    dds3SetQuake(frames, scrGetIntPara(1));
    return 1;
}

s32 scrCommand_QUAKE_OFF(void)
{
    dds3StopQuake();
    return 1;
}

s32 scrCommand_SCENE_LIGHT_RGB(void)
{
    scrVec4_t v;
    f32 x;
    f32 y;
    f32 z;

    x = scrGetFloatPara(1);
    SDF_VU0_SETVECTOR_X_BITS(x);
    y = scrGetFloatPara(2);
    SDF_VU0_SETVECTOR_Y_BITS(y);
    z = scrGetFloatPara(3);
    SDF_VU0_SETVECTOR_Z_BITS(z);
    SDF_VU0_VECTOR_W_SET_ZERO();
    SDF_VU0_STOREVECTOR_MEMOPS(v);
    dds3SetLightColor(scrGetIntPara(0), 0, &v);
    return 1;
}


s32 scrCommand_SCENE_LIGHT_DIR(void)
{
    scrVecW_t dir;

    dir.x = scrGetFloatPara(1);
    dir.y = scrGetFloatPara(2);
    dir.z = scrGetFloatPara(3);
    dir.w = 0;
    if (scrFabsf(dir.x) <= 0.01f && scrFabsf(dir.y) <= 0.01f && scrFabsf(dir.z) <= 0.01f)
    {
        datDebugPrintf("WARNING: SCENE_LIGHT_DIR length zero\n");
        return 1;
    }
    dds3SetLightDir(scrGetIntPara(0), 0, &dir);
    return 1;
}

s32 scrCommand_SCENE_AMBIENT(void)
{
    scrVec4_t v;
    f32 x;
    f32 y;
    f32 z;

    x = scrGetFloatPara(1);
    SDF_VU0_SETVECTOR_X_BITS(x);
    y = scrGetFloatPara(2);
    SDF_VU0_SETVECTOR_Y_BITS(y);
    z = scrGetFloatPara(3);
    SDF_VU0_SETVECTOR_Z_BITS(z);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_STOREVECTOR_MEMOPS(v);
    dds3SetAmbientColor(scrGetIntPara(0), &v);
    return 1;
}

s32 scrCommand_SCENE_FOG_RGB(void)
{
    scrVecW_t v;

    v.x = scrGetFloatPara(1);
    v.y = scrGetFloatPara(2);
    v.z = scrGetFloatPara(3);
    v.w = 0;
    dds3SetFogColor(scrGetIntPara(0), &v.x);
    return 1;
}

s32 scrCommand_SCENE_FOG_PARA(void)
{
    s32 frames;
    f32 x;
    f32 y;
    f32 z;
    f32 w;

    frames = scrGetIntPara(0);
    x = (f32)(u32)scrGetIntPara(1);
    y = (f32)(u32)scrGetIntPara(3);
    z = (f32)(u32)scrGetIntPara(2);
    w = (f32)(u32)scrGetIntPara(4);
    dds3SetFogPara(frames, x, y, z, w);
    return 1;
}

s32 scrCommand_SOFTEN_BIAS(void)
{
    s32 frames;

    frames = scrGetIntPara(0);
    dds3SetSoftenBias(frames, scrGetFloatPara(1), scrGetFloatPara(2));
    return 1;
}

s32 scrCommand_BLUR(void)
{
    s32 frames;
    s32 alpha;

    frames = scrGetIntPara(0);
    alpha = scrGetIntPara(1);
    dds3SetBlur(frames, alpha, scrGetIntPara(2));
    return 1;
}

s32 scrCommand_MUL_BLUR_PARA(void)
{
    u32 blendControl;
    f32 rotation;
    f32 scale;

    switch (scrGetIntPara(2))
    {
    case 1:
        blendControl = 0x48;
        break;
    case 2:
        blendControl = 0x42;
        break;
    case 0:
    default:
        blendControl = 0x44;
        break;
    }
    rotation = scrGetFloatPara(0);
    scale = scrGetFloatPara(1);
    dds3SetMultiBlurPara(rotation, scale, blendControl);
    return 1;
}

s32 scrCommand_MUL_BLUR_RGBA(void)
{
    s32 byte2;
    s32 byte0;
    s32 byte3;
    s32 byte1;

    byte0 = scrGetIntPara(0);
    byte3 = scrGetIntPara(3);
    byte2 = scrGetIntPara(2);
    byte1 = scrGetIntPara(1);
    dds3SetMultiBlurRGBA(((byte0 & 0xFF) | (byte3 << 24)) |
                         (((byte2 & 0xFF) << 16) | ((byte1 & 0xFF) << 8)));
    return 1;
}

s32 scrCommand_MUL_BLUR_AREA(void)
{
    s32 size;
    s32 x;

    size = scrGetIntPara(0);
    x = scrGetIntPara(1);
    dds3SetMultiBlurArea(size, x, scrGetIntPara(2));
    return 1;
}

s32 scrCommand_MUL_BLUR_SWAP(void)
{
    dds3MultiBlurSwap(scrGetIntPara(0));
    return 1;
}

s32 scrCommand_MUL_BLUR_ON(void)
{
    dds3MultiBlurOn(scrGetIntPara(0));
    return 1;
}

s32 scrCommand_MUL_BLUR_OFF(void)
{
    dds3MultiBlurOff(scrGetIntPara(0));
    return 1;
}

s32 scrCommand_DIST_BLUR_PARA(void)
{
    s32 spread;
    s32 size;
    s32 count;
    s32 blendMode;
    s32 blendControl;
    f32 f4;
    f32 f3;
    f32 f5;

    blendMode = scrGetIntPara(6);
    switch (blendMode)
    {
    case 1:
        blendControl = 0x48;
        break;
    case 2:
        blendControl = 0x42;
        break;
    case 0:
    default:
        blendControl = 0x44;
        break;
    }
    count = scrGetIntPara(0);
    size = scrGetIntPara(1);
    spread = scrGetIntPara(2);
    f3 = scrGetFloatPara(3);
    f4 = scrGetFloatPara(4);
    f5 = scrGetFloatPara(5);
    dds3SetDistBlurPara(count, size, spread, f3, f4, f5, blendControl);
    return 1;
}

s32 scrCommand_DIST_BLUR_RGBA(void)
{
    s32 byte2;
    s32 byte0;
    s32 byte3;
    s32 byte1;

    byte0 = scrGetIntPara(0);
    byte3 = scrGetIntPara(3);
    byte2 = scrGetIntPara(2);
    byte1 = scrGetIntPara(1);
    dds3SetDistBlurRGBA(((byte0 & 0xFF) | (byte3 << 24)) |
                        (((byte2 & 0xFF) << 16) | ((byte1 & 0xFF) << 8)));
    return 1;
}

s32 scrCommand_DIST_BLUR_AREA(void)
{
    s32 size;
    s32 x;

    size = scrGetIntPara(0);
    x = scrGetIntPara(1);
    dds3SetDistBlurArea(size, x, scrGetIntPara(2));
    return 1;
}

s32 scrCommand_DIST_BLUR_SWAP(void)
{
    dds3DistBlurSwap(scrGetIntPara(0));
    return 1;
}

s32 scrCommand_DIST_BLUR_ON(void)
{
    dds3DistBlurOn(scrGetIntPara(0));
    return 1;
}

s32 scrCommand_DIST_BLUR_OFF(void)
{
    dds3DistBlurOff(scrGetIntPara(0));
    return 1;
}

s32 scrCommand_RIP_BLUR_PARA(void)
{
    s32 count;
    s32 blendMode;
    s32 blendControl;
    f32 f1;
    f32 f2;
    f32 f3;
    f32 f4;
    f32 f5;

    blendMode = scrGetIntPara(6);
    switch (blendMode)
    {
    case 1:
        blendControl = 0x48;
        break;
    case 2:
        blendControl = 0x42;
        break;
    case 0:
    default:
        blendControl = 0x44;
        break;
    }
    count = scrGetIntPara(0);
    f1 = scrGetFloatPara(1);
    f2 = scrGetFloatPara(2);
    f3 = scrGetFloatPara(3);
    f4 = scrGetFloatPara(4);
    f5 = scrGetFloatPara(5);
    dds3SetRipBlurPara(count, f1, f2, f3, f4, f5, blendControl);
    return 1;
}

s32 scrCommand_RIP_BLUR_RGBA(void)
{
    s32 byte2;
    s32 byte0;
    s32 byte3;
    s32 byte1;

    byte0 = scrGetIntPara(0);
    byte3 = scrGetIntPara(3);
    byte2 = scrGetIntPara(2);
    byte1 = scrGetIntPara(1);
    dds3SetRipBlurRGBA(((byte0 & 0xFF) | (byte3 << 24)) |
                       (((byte2 & 0xFF) << 16) | ((byte1 & 0xFF) << 8)));
    return 1;
}

s32 scrCommand_RIP_BLUR_AREA(void)
{
    s32 size;
    s32 x;

    size = scrGetIntPara(0);
    x = scrGetIntPara(1);
    dds3SetRipBlurArea(size, x, scrGetIntPara(2));
    return 1;
}

s32 scrCommand_RIP_BLUR_SWAP(void)
{
    dds3RipBlurSwap(scrGetIntPara(0));
    return 1;
}

s32 scrCommand_RIP_BLUR_ON(void)
{
    dds3RipBlurOn(scrGetIntPara(0));
    return 1;
}

s32 scrCommand_RIP_BLUR_OFF(void)
{
    dds3RipBlurOff(scrGetIntPara(0));
    return 1;
}

s32 scrCommand_BLUR2_PARA(void)
{
    s32 blendControl;
    s32 blendMode;
    f32 rotation;
    f32 scale;

    blendMode = scrGetIntPara(2);
    switch (blendMode)
    {
    case 0:
        blendControl = 0x44;
        break;
    case 1:
        blendControl = 0x48;
        break;
    case 2:
        blendControl = 0x42;
        break;
    default:
        datDebugPrintf("error: BLUR2_PARA blend\n");
        blendControl = 0x44;
        break;
    }
    rotation = scrGetFloatPara(0);
    scale = scrGetFloatPara(1);
    dds3SetBlur2Para(rotation, scale, blendControl);
    return 1;
}

s32 scrCommand_BLUR2_RGBA(void)
{
    s32 byte2;
    s32 byte0;
    s32 byte3;
    s32 byte1;

    byte0 = scrGetIntPara(0);
    byte3 = scrGetIntPara(3);
    byte2 = scrGetIntPara(2);
    byte1 = scrGetIntPara(1);
    dds3SetBlur2RGBA(((byte0 & 0xFF) | (byte3 << 24)) |
                     (((byte2 & 0xFF) << 16) | ((byte1 & 0xFF) << 8)));
    return 1;
}

s32 scrCommand_BLUR2_AREA(void)
{
    s32 size;

    size = scrGetIntPara(0);
    dds3SetBlur2Area(size, scrGetIntPara(1));
    return 1;
}

s32 scrCommand_BLUR2_SWAP(void)
{
    dds3Blur2Swap(scrGetIntPara(0));
    return 1;
}

s32 scrCommand_BLUR2_ON(void)
{
    dds3Blur2On(scrGetIntPara(0));
    return 1;
}

s32 scrCommand_BLUR2_OFF(void)
{
    dds3Blur2Off(scrGetIntPara(0));
    return 1;
}

s32 scrCommand_FILTER2_PARA(void)
{
    s32 blendMode;
    s32 blendControl;

    blendMode = scrGetIntPara(0);
    switch (blendMode)
    {
    case 1:
        blendControl = 0x48;
        break;
    case 0:
        blendControl = 0x44;
        break;
    case 2:
        blendControl = 0x42;
        break;
    case 3:
        blendControl = 6;
        break;
    default:
        datDebugPrintf("error: FILTER2_PARA blend\n");
        blendControl = 0x44;
        break;
    }
    dds3SetFilter2Para(blendControl);
    return 1;
}

s32 scrCommand_FILTER2_RGBA(void)
{
    s32 byte2;
    s32 byte0;
    s32 byte3;
    s32 byte1;

    byte0 = scrGetIntPara(0);
    byte3 = scrGetIntPara(3);
    byte2 = scrGetIntPara(2);
    byte1 = scrGetIntPara(1);
    dds3SetFilter2RGBA(((byte0 & 0xFF) | (byte3 << 24)) |
                       (((byte2 & 0xFF) << 16) | ((byte1 & 0xFF) << 8)));
    return 1;
}

s32 scrCommand_FILTER2_SWAP(void)
{
    dds3Filter2Swap(scrGetIntPara(0));
    return 1;
}

s32 scrCommand_FILTER2_ON(void)
{
    dds3Filter2On(scrGetIntPara(0));
    return 1;
}

s32 scrCommand_FILTER2_OFF(void)
{
    dds3Filter2Off(scrGetIntPara(0));
    return 1;
}

s32 scrCommand_MFILTER_PARA(void)
{
    s32 blendMode;
    s32 blendControl;

    blendMode = scrGetIntPara(0);
    switch (blendMode)
    {
    case 1:
        blendControl = 0x48;
        break;
    case 0:
        blendControl = 0x44;
        break;
    case 2:
        blendControl = 0x42;
        break;
    case 3:
        blendControl = 6;
        break;
    default:
        datDebugPrintf("error: MFILTER_PARA blend\n");
        blendControl = 0x44;
        break;
    }
    dds3SetMFilterPara(blendControl);
    return 1;
}

s32 scrCommand_MFILTER_RGBA(void)
{
    s32 byte2;
    s32 byte0;
    s32 byte3;
    s32 byte1;

    byte0 = scrGetIntPara(0);
    byte3 = scrGetIntPara(3);
    byte2 = scrGetIntPara(2);
    byte1 = scrGetIntPara(1);
    dds3SetMFilterRGBA(((byte0 & 0xFF) | (byte3 << 24)) |
                       (((byte2 & 0xFF) << 16) | ((byte1 & 0xFF) << 8)));
    return 1;
}

s32 scrCommand_MFILTER_AREA(void)
{
    s32 size;
    s32 x;

    size = scrGetIntPara(0);
    x = scrGetIntPara(1);
    dds3SetMFilterArea(size, x, scrGetIntPara(2));
    return 1;
}

s32 scrCommand_MFILTER_SWAP(void)
{
    dds3MFilterSwap(scrGetIntPara(0));
    return 1;
}

s32 scrCommand_MFILTER_ON(void)
{
    dds3MFilterOn(scrGetIntPara(0));
    return 1;
}

s32 scrCommand_MFILTER_OFF(void)
{
    dds3MFilterOff(scrGetIntPara(0));
    return 1;
}

s32 scrCommand_BLUR_ALLOFF(void)
{
    dds3SetBlur(0, 0, 0);
    dds3Blur2Off(0);
    dds3MultiBlurOff(0);
    dds3DistBlurOff(0);
    dds3RipBlurOff(0);
    effDisableFramebufferQuad();
    return 1;
}

s32 scrCommand_FILTER_ALLOFF(void)
{
    dds3Filter2Off(0);
    dds3MFilterOff(0);
    fldSetFilterAnime(0);
    fldSkySetAlpha(0x80);
    fldSkySetAlphaOfs(0);
    fldSetFadeTarget(0, 1, 0);
    return 1;
}

s32 scrCommandClearProcessControlFlag(void)
{
    DDS3_GBWK->integers[SCR_PROCESS_CONTROL_FLAG] = 0;
    return 1;
}

s32 scrCommandSetProcessControlFlag(void)
{
    DDS3_GBWK->integers[SCR_PROCESS_CONTROL_FLAG] = 1;
    return 1;
}

s32 scrCommandIsProcessControlFlagClear(void)
{
    return DDS3_GBWK->integers[SCR_PROCESS_CONTROL_FLAG] == 0;
}

s32 scrCommandAdjustItemQuantity(void)
{
    s32 itemId;

    itemId = scrGetIntPara(0);
    ptyAdjustItemQuantity(itemId, scrGetIntPara(1));
    return 1;
}

s32 scrCommandAddPartyCurrency(void)
{
    datAddCurrencyClamped(scrGetIntPara(0));
    return 1;
}

s32 scrCommand_SCR_EXISTS(void)
{
    s32 requiredAmount;

    requiredAmount = scrGetIntPara(0);
    if (datHasEnoughCurrency(requiredAmount) != 0)
    {
        scrSetIntReturnValue(1);
    }
    else
    {
        scrSetIntReturnValue(0);
    }
    return 1;
}
