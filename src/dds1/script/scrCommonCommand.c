#include "dat_state.h"
#include "common.h"
#include "sdf_vu0.h"
#include "scr_integer_return_value_api.h"
#include "kwln_background_fade_api.h"
#include "eff_random_mod.h"
#include "fpu.h"
#include "kwln.h"
#include "pcp_vu0.h"
#include "scr.h"
#include "scr_command_timer_api.h"
#include "scr_string_parameter_api.h"
#include "scr_arg_read.h"
#include "kwln_task_lifecycle.h"
#include "kwln_fade.h"
#include "eff_event_draw.h"
s32 dds3SetDistBlurPara(s32 filterCount, s32 size, s32 delaySpread, f32 angleStep,
                          f32 uvDisplacementAngleDegrees, f32 uvDisplacementAmplitude,
                          s32 gsAlphaControl);
s32 dds3SetRipBlurPara(s32 count, f32 phaseStep, f32 spacing,
                          f32 uvDisplacementAngleDegrees, f32 uvDisplacementAmplitude,
                          f32 angleStep, s32 gsAlphaControl);

void dds3SetSoftenBias(s32 frames, f32 firstTarget, f32 secondTarget);
void dds3SetFogColor(s32 blendFrames, f32 *color);
void dds3SetMultiBlurPara(f32 rotation, f32 scale, u32 gsAlphaControl);
void dds3SetLightColor(s32 blendFrames, s32 lightIndex, f32 *color);
void dds3SetAmbientColor(s32 blendFrames, f32 *color);
void dds3SetLightDir(s32 blendFrames, s32 lightIndex, f32 *direction);
void dds3SetBlur2Para(f32 rotation, f32 scale, u32 gsAlphaControl);
void dds3SetFilter2Para(u32 gsAlphaControl);
void dds3SetMFilterPara(u32 gsAlphaControl);
void dds3SetScreenFovy(f32 fieldOfView);
void dds3SetFogPara(s32 blendFrames, f32 x, f32 y, f32 z, f32 w);
s32 fptodp(void);
extern s8 dds3PadBitTable[];
extern char D_0039F4D8[];
extern char D_0039F4E8[];
extern const char scrSceneLightDirectionZeroWarning[];
extern char D_0039F530[];
extern char D_0039F550[];
extern char D_0039F570[];

extern s32 scrGetIntPara(s32 parameterIndex);

s32 scrCommand_RND(void)
{
    scrSetIntReturnValue(dds3GetRandIntA(0, scrGetIntPara(0)) + 1);
    return 1;
}

s32 scrCommand_SYNC(void)
{
    return scrGetCommandTimer() != 0;
}

/* Succeed when the command timer reaches the requested tick count; a
 * nonpositive request completes immediately. */
s32 scrCommand_WAIT(void) {
    if (scrGetIntPara(0) <= 0) {
        return 1;
    }
    /* VM timer limits use signed words; retail +0x30 is slt, not sltu. */
    if ((s32)scrGetCommandTimer() < scrGetIntPara(0)) {
        return 0;
    }
    return 1;
}

extern void evtPrintDeveloperConsoleMessage(const char *format, ...);

s32 scrCommandPrintInteger(void)
{
    evtPrintDeveloperConsoleMessage("PUT -> %d\n", scrGetIntPara(0));
    return 1;
}

s32 scrCommandPrintString(void)
{
    evtPrintDeveloperConsoleMessage(D_0039F4D8, scrGetStrPara(0));
    return 1;
}

s32 bfWaitCbScreenFadeA(void)
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

s32 bfWaitCbScreenFadeB(void)
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
        kwlnFadeBackgroundStartOut(scrGetIntPara(0));
        return 0;
    }
    return 1;
}

extern void kwlnFadeBackgroundStartIn(s32 duration);

s32 scrCommandFadeBackgroundIn(void)
{
    if (scrGetCommandTimer() == 0)
    {
        kwlnFadeBackgroundStartIn(scrGetIntPara(0));
        return 0;
    }
    return 1;
}

extern s32 scrGetLabelCodeAddress(s32 labelIndex);
extern void scrSetCodePointer(s32 address);

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

extern void dds3PadVibrationSet(u32 motor, u8 level, s32 duration);

s32 scrCommand_PAD_ACT(void)
{
    s32 motor;
    s32 motorLevel;
    motor = scrGetIntPara(0);
    motorLevel = scrGetIntPara(1);
    dds3PadVibrationSet(motor, motorLevel & 0xFF, scrGetIntPara(2));
    return 1;
}

s32 scrCommand_SET_FOVY(void)
{
    f32 fovy;

    fovy = scrGetFloatPara(0) * 0.017453293f;
    if (fovy <= 5.0f || fovy >= 180.0f)
    {
        evtPrintDeveloperConsoleMessage(D_0039F4E8, fovy);
        return 1;
    }
    dds3SetScreenFovy(fovy);
    return 1;
}


s32 bfWaitCbCreateTask(void)
{
    ScrProcedureIndex procedureIndex;
    scrProcessWork_s *ctx;

    procedureIndex = scrGetIntPara(0);
    ctx = scrGetScriptHandle();
    if (ctx == NULL)
    {
        return 1;
    }
    if (ctx->task == NULL)
    {
        return 1;
    }
    if (procedureIndex < 0 || procedureIndex >= ctx->sections->count)
    {
        return 1;
    }
    scrSetIntReturnValue((s32)scrRunScriptEx(
        ctx->task->priority + scrGetIntPara(1), ctx->scriptHeader,
        ctx->sections, ctx->procedures, ctx->labels,
        ctx->instructions, ctx->auxiliaryData, ctx->strings, procedureIndex));
    return 1;
}

s32 scrCommand_SCR_KILL(void)
{
    dds3ProcessID_t *task;
    task = (dds3ProcessID_t *)scrGetIntPara(0);
    if (dds3ExistProcess(task) == 0)
    {
        return 1;
    }
    dds3KillProcess(task, 1);
    return 1;
}

s32 scrCommand_SCR_WAIT(void)
{
    return dds3ExistProcess((dds3ProcessID_t *)scrGetIntPara(0)) == 0;
}

s32 scrCommand_SCR_STAT(void)
{
    if (dds3ExistProcess((dds3ProcessID_t *)scrGetIntPara(0)) != 0)
    {
        scrSetIntReturnValue(1);
    }
    else
    {
        scrSetIntReturnValue(0);
    }
    return 1;
}

/* Persona 4 scrCommand_SCR_TIME @ 00299660 (src/Script/scrCommonCommand.c), recompiled unchanged */
u32 scrCommand_SCR_TIME()
{
    dds3ProcessID_t* task;
    task = (dds3ProcessID_t*)scrGetIntPara(0);
    if (!dds3ExistProcess(task))
    {
        scrSetIntReturnValue(0);
    }
    else
    {
        scrSetIntReturnValue(dds3GetProcessRunTime(task));
    }
    return 1;
}

extern void dds3SetQuake(s32 fadeMode, s32 frames);

s32 scrCommand_QUAKE_ON(void)
{
    s32 fadeMode;
    s32 durationFrames;
    fadeMode = scrGetIntPara(0);
    durationFrames = scrGetIntPara(1);
    dds3SetQuake(fadeMode, durationFrames);
    return 1;
}

extern void dds3StopQuake(void);

s32 scrCommand_QUAKE_OFF(void)
{
    dds3StopQuake();
    return 1;
}

s32 scrCommand_SCENE_LIGHT_RGB(void)
{
    ScrVec4 v;
    f32 x;
    f32 y;
    f32 z;
    x = scrGetFloatPara(1);
    VU0_SET_AXIS_GPR(x, x);
    y = scrGetFloatPara(2);
    VU0_SET_AXIS_GPR(y, y);
    z = scrGetFloatPara(3);
    VU0_SET_AXIS_GPR(z, z);
    SDF_VU0_VECTOR_W_SET_ZERO();
    SDF_VU0_STOREVECTOR_MEMOPS(v);
    dds3SetLightColor(scrGetIntPara(0), 0, &v.x);
    return 1;
}

INCLUDE_RODATA(const s32, "script/scrCommonCommand", D_0039F4D8);

INCLUDE_RODATA(const s32, "script/scrCommonCommand", D_0039F4E8);

const char scrSceneLightDirectionZeroWarning[] = "WARNING: SCENE_LIGHT_DIR length zero\n";

s32 scrCommand_SCENE_LIGHT_DIR(void) {
    ScrVecW direction;

    direction.x = scrGetFloatPara(1);
    direction.y = scrGetFloatPara(2);
    direction.z = scrGetFloatPara(3);
    direction.w = 0;
    if (ffabsf(direction.x) <= 0.01f &&
        ffabsf(direction.y) <= 0.01f &&
        ffabsf(direction.z) <= 0.01f) {
        evtPrintDeveloperConsoleMessage(scrSceneLightDirectionZeroWarning);
        return 1;
    }
    dds3SetLightDir(scrGetIntPara(0), 0, &direction.x);
    return 1;
}

s32 scrCommand_SCENE_AMBIENT(void)
{
    ScrVec4 v;
    f32 x;
    f32 y;
    f32 z;
    x = scrGetFloatPara(1);
    VU0_SET_AXIS_GPR(x, x);
    y = scrGetFloatPara(2);
    VU0_SET_AXIS_GPR(y, y);
    z = scrGetFloatPara(3);
    VU0_SET_AXIS_GPR(z, z);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_STOREVECTOR_MEMOPS(v);
    dds3SetAmbientColor(scrGetIntPara(0), &v.x);
    return 1;
}

s32 scrCommand_SCENE_FOG_RGB(void)
{
    ScrVecW v;
    v.x = scrGetFloatPara(1);
    v.y = scrGetFloatPara(2);
    v.z = scrGetFloatPara(3);
    v.w = 0;
    dds3SetFogColor(scrGetIntPara(0), &v.x);
    return 1;
}

s32 scrCommand_SCENE_FOG_PARA(void)
{
    s32 blendFrames;
    f32 x;
    f32 y;
    f32 z;
    f32 w;

    blendFrames = scrGetIntPara(0);
    x = (f32)(u32)scrGetIntPara(1);
    y = (f32)(u32)scrGetIntPara(3);
    z = (f32)(u32)scrGetIntPara(2);
    w = (f32)(u32)scrGetIntPara(4);
    dds3SetFogPara(blendFrames, x, y, z, w);
    return 1;
}

s32 scrCommand_SOFTEN_BIAS(void)
{
    s32 frames;
    frames = scrGetIntPara(0);
    dds3SetSoftenBias(frames, scrGetFloatPara(1), scrGetFloatPara(2));
    return 1;
}

extern void dds3SetBlur(s32 duration, s32 alpha, s32 scale);

s32 scrCommand_BLUR(void)
{
    s32 duration;
    s32 alpha;
    duration = scrGetIntPara(0);
    alpha = scrGetIntPara(1);
    dds3SetBlur(duration, alpha, scrGetIntPara(2));
    return 1;
}

s32 scrCommand_MUL_BLUR_PARA(void) {
    u32 gsAlphaControl;
    f32 rotation;
    f32 scale;

    switch (scrGetIntPara(2)) {
    case 1:
        gsAlphaControl = 0x48;
        break;
    case 2:
        gsAlphaControl = 0x42;
        break;
    case 0:
    default:
        gsAlphaControl = 0x44;
        break;
    }
    rotation = scrGetFloatPara(0);
    scale = scrGetFloatPara(1);
    dds3SetMultiBlurPara(rotation, scale, gsAlphaControl);
    return 1;
}


extern void dds3SetMultiBlurRGBA(u32 rgba);

s32 scrCommand_MUL_BLUR_RGBA(void)
{
    /* Declared out of order: gcc 2.96 fills $16-$18 in declaration order. */
    s32 byte2;
    s32 byte0;
    s32 byte3;
    s32 byte1;
    byte0 = scrGetIntPara(0);
    byte3 = scrGetIntPara(3);
    byte2 = scrGetIntPara(2);
    byte1 = scrGetIntPara(1);
    dds3SetMultiBlurRGBA(((byte0 & 0xFF) | (byte3 << 24)) | (((byte2 & 0xFF) << 16) | ((byte1 & 0xFF) << 8)));
    return 1;
}

extern void dds3SetMultiBlurArea(u32 extent, u32 centerX, u32 centerY);

s32 scrCommand_MUL_BLUR_AREA(void)
{
    s32 extent;
    s32 centerX;
    s32 centerY;
    extent = scrGetIntPara(0);
    centerX = scrGetIntPara(1);
    centerY = scrGetIntPara(2);
    dds3SetMultiBlurArea(extent, centerX, centerY);
    return 1;
}

extern void dds3MultiBlurSwap(s32 duration);

s32 scrCommand_MUL_BLUR_SWAP(void)
{
    dds3MultiBlurSwap(scrGetIntPara(0));
    return 1;
}

extern void dds3MultiBlurOn(s32 duration);

s32 scrCommand_MUL_BLUR_ON(void)
{
    dds3MultiBlurOn(scrGetIntPara(0));
    return 1;
}

extern void dds3MultiBlurOff(s32 duration);

s32 scrCommand_MUL_BLUR_OFF(void)
{
    dds3MultiBlurOff(scrGetIntPara(0));
    return 1;
}

s32 scrCommand_DIST_BLUR_PARA(void)
{
    s32 delaySpread;
    s32 size;
    s32 filterCount;
    s32 scriptBlendSelector;
    s32 gsAlphaControl;
    f32 f4;
    f32 f3;
    f32 f5;

    scriptBlendSelector = scrGetIntPara(6);
    switch (scriptBlendSelector)
    {
    case 1:
        gsAlphaControl = 0x48;
        break;
    case 2:
        gsAlphaControl = 0x42;
        break;
    case 0:
    default:
        gsAlphaControl = 0x44;
        break;
    }
    filterCount = scrGetIntPara(0);
    size = scrGetIntPara(1);
    delaySpread = scrGetIntPara(2);
    f3 = scrGetFloatPara(3);
    f4 = scrGetFloatPara(4);
    f5 = scrGetFloatPara(5);
    dds3SetDistBlurPara(filterCount, size, delaySpread, f3, f4, f5, gsAlphaControl);
    return 1;
}

extern void dds3SetDistBlurRGBA(u32 rgba);

s32 scrCommand_DIST_BLUR_RGBA(void)
{
    /* Declared out of order: gcc 2.96 fills $16-$18 in declaration order. */
    s32 byte2;
    s32 byte0;
    s32 byte3;
    s32 byte1;
    byte0 = scrGetIntPara(0);
    byte3 = scrGetIntPara(3);
    byte2 = scrGetIntPara(2);
    byte1 = scrGetIntPara(1);
    dds3SetDistBlurRGBA(((byte0 & 0xFF) | (byte3 << 24)) | (((byte2 & 0xFF) << 16) | ((byte1 & 0xFF) << 8)));
    return 1;
}

extern void dds3SetDistBlurArea(u32 positionSpread, u32 x, u32 y);

s32 scrCommand_DIST_BLUR_AREA(void)
{
    s32 positionSpread;
    s32 x;
    positionSpread = scrGetIntPara(0);
    x = scrGetIntPara(1);
    dds3SetDistBlurArea(positionSpread, x, scrGetIntPara(2));
    return 1;
}

extern void dds3DistBlurSwap(s32 duration);

s32 scrCommand_DIST_BLUR_SWAP(void)
{
    dds3DistBlurSwap(scrGetIntPara(0));
    return 1;
}

extern void dds3DistBlurOn(s32 duration);

s32 scrCommand_DIST_BLUR_ON(void)
{
    dds3DistBlurOn(scrGetIntPara(0));
    return 1;
}

extern void dds3DistBlurOff(s32 duration);

s32 scrCommand_DIST_BLUR_OFF(void)
{
    dds3DistBlurOff(scrGetIntPara(0));
    return 1;
}

s32 scrCommand_RIP_BLUR_PARA(void)
{
    s32 count;
    s32 scriptBlendSelector;
    s32 gsAlphaControl;
    f32 phaseStep;
    f32 spacing;
    f32 uvDisplacementAngleDegrees;
    f32 uvDisplacementAmplitude;
    f32 angleStep;

    scriptBlendSelector = scrGetIntPara(6);
    switch (scriptBlendSelector)
    {
    case 1:
        gsAlphaControl = 0x48;
        break;
    case 2:
        gsAlphaControl = 0x42;
        break;
    case 0:
    default:
        gsAlphaControl = 0x44;
        break;
    }
    count = scrGetIntPara(0);
    phaseStep = scrGetFloatPara(1);
    spacing = scrGetFloatPara(2);
    uvDisplacementAngleDegrees = scrGetFloatPara(3);
    uvDisplacementAmplitude = scrGetFloatPara(4);
    angleStep = scrGetFloatPara(5);
    dds3SetRipBlurPara(count, phaseStep, spacing, uvDisplacementAngleDegrees,
                          uvDisplacementAmplitude, angleStep, gsAlphaControl);
    return 1;
}

extern void dds3SetRipBlurRGBA(u32 rgba);

s32 scrCommand_RIP_BLUR_RGBA(void)
{
    /* Declared out of order: gcc 2.96 fills $16-$18 in declaration order. */
    s32 byte2;
    s32 byte0;
    s32 byte3;
    s32 byte1;
    byte0 = scrGetIntPara(0);
    byte3 = scrGetIntPara(3);
    byte2 = scrGetIntPara(2);
    byte1 = scrGetIntPara(1);
    dds3SetRipBlurRGBA(((byte0 & 0xFF) | (byte3 << 24)) | (((byte2 & 0xFF) << 16) | ((byte1 & 0xFF) << 8)));
    return 1;
}

extern void dds3SetRipBlurArea(u32 size, u32 x, u32 y);

s32 scrCommand_RIP_BLUR_AREA(void)
{
    s32 size;
    s32 x;
    size = scrGetIntPara(0);
    x = scrGetIntPara(1);
    dds3SetRipBlurArea(size, x, scrGetIntPara(2));
    return 1;
}

extern void dds3RipBlurSwap(s32 duration);

s32 scrCommand_RIP_BLUR_SWAP(void)
{
    dds3RipBlurSwap(scrGetIntPara(0));
    return 1;
}

extern void dds3RipBlurOn(s32 duration);

s32 scrCommand_RIP_BLUR_ON(void)
{
    dds3RipBlurOn(scrGetIntPara(0));
    return 1;
}

extern void dds3RipBlurOff(s32 duration);

s32 scrCommand_RIP_BLUR_OFF(void)
{
    dds3RipBlurOff(scrGetIntPara(0));
    return 1;
}

s32 scrCommand_BLUR2_PARA(void)
{
    s32 gsAlphaControl;
    s32 scriptBlendSelector;
    f32 rotation;
    f32 scale;

    scriptBlendSelector = scrGetIntPara(2);
    switch (scriptBlendSelector) {
    case 0:
        gsAlphaControl = 0x44;
        break;
    case 1:
        gsAlphaControl = 0x48;
        break;
    case 2:
        gsAlphaControl = 0x42;
        break;
    default:
        evtPrintDeveloperConsoleMessage("error: BLUR2_PARA blend\n");
        gsAlphaControl = 0x44;
        break;
    }

    rotation = scrGetFloatPara(0);
    scale = scrGetFloatPara(1);
    dds3SetBlur2Para(rotation, scale, gsAlphaControl);
    return 1;
}


extern void dds3SetBlur2RGBA(u32 rgba);

s32 scrCommand_BLUR2_RGBA(void)
{
    /* Declared out of order: gcc 2.96 fills $16-$18 in declaration order. */
    s32 byte2;
    s32 byte0;
    s32 byte3;
    s32 byte1;
    byte0 = scrGetIntPara(0);
    byte3 = scrGetIntPara(3);
    byte2 = scrGetIntPara(2);
    byte1 = scrGetIntPara(1);
    dds3SetBlur2RGBA(((byte0 & 0xFF) | (byte3 << 24)) | (((byte2 & 0xFF) << 16) | ((byte1 & 0xFF) << 8)));
    return 1;
}

extern void dds3SetBlur2Area(u32 centerX, u32 centerY);

s32 scrCommand_BLUR2_AREA(void)
{
    s32 centerX;
    centerX = scrGetIntPara(0);
    dds3SetBlur2Area(centerX, scrGetIntPara(1));
    return 1;
}

extern void dds3Blur2Swap(s32 duration);

s32 scrCommand_BLUR2_SWAP(void)
{
    dds3Blur2Swap(scrGetIntPara(0));
    return 1;
}

extern void dds3Blur2On(s32 duration);

s32 scrCommand_BLUR2_ON(void)
{
    dds3Blur2On(scrGetIntPara(0));
    return 1;
}

extern void dds3Blur2Off(s32 duration);

s32 scrCommand_BLUR2_OFF(void)
{
    dds3Blur2Off(scrGetIntPara(0));
    return 1;
}

s32 scrCommand_FILTER2_PARA(void)
{
    s32 scriptBlendSelector;
    u32 gsAlphaControl;
    scriptBlendSelector = scrGetIntPara(0);
    switch (scriptBlendSelector)
    {
    case 1:
        gsAlphaControl = 0x48;
        break;
    case 0:
        gsAlphaControl = 0x44;
        break;
    case 2:
        gsAlphaControl = 0x42;
        break;
    case 3:
        gsAlphaControl = 6;
        break;
    default:
        evtPrintDeveloperConsoleMessage(D_0039F550);
        gsAlphaControl = 0x44;
        break;
    }
    dds3SetFilter2Para(gsAlphaControl);
    return 1;
}

extern void dds3SetFilter2RGBA(u32 rgba);

s32 scrCommand_FILTER2_RGBA(void)
{
    /* Declared out of order: gcc 2.96 fills $16-$18 in declaration order. */
    s32 byte2;
    s32 byte0;
    s32 byte3;
    s32 byte1;
    byte0 = scrGetIntPara(0);
    byte3 = scrGetIntPara(3);
    byte2 = scrGetIntPara(2);
    byte1 = scrGetIntPara(1);
    dds3SetFilter2RGBA(((byte0 & 0xFF) | (byte3 << 24)) | (((byte2 & 0xFF) << 16) | ((byte1 & 0xFF) << 8)));
    return 1;
}

extern void dds3Filter2Swap(s32 duration);

s32 scrCommand_FILTER2_SWAP(void)
{
    dds3Filter2Swap(scrGetIntPara(0));
    return 1;
}

extern void dds3Filter2On(s32 duration);

s32 scrCommand_FILTER2_ON(void)
{
    dds3Filter2On(scrGetIntPara(0));
    return 1;
}

extern void dds3Filter2Off(s32 duration);

s32 scrCommand_FILTER2_OFF(void)
{
    dds3Filter2Off(scrGetIntPara(0));
    return 1;
}

s32 scrCommand_MFILTER_PARA(void)
{
    s32 scriptBlendSelector;
    u32 gsAlphaControl;
    scriptBlendSelector = scrGetIntPara(0);
    switch (scriptBlendSelector)
    {
    case 1:
        gsAlphaControl = 0x48;
        break;
    case 0:
        gsAlphaControl = 0x44;
        break;
    case 2:
        gsAlphaControl = 0x42;
        break;
    case 3:
        gsAlphaControl = 6;
        break;
    default:
        evtPrintDeveloperConsoleMessage(D_0039F570);
        gsAlphaControl = 0x44;
        break;
    }
    dds3SetMFilterPara(gsAlphaControl);
    return 1;
}

extern void dds3SetMFilterRGBA(u32 rgba);

s32 scrCommand_MFILTER_RGBA(void)
{
    /* Declared out of order: gcc 2.96 fills $16-$18 in declaration order. */
    s32 byte2;
    s32 byte0;
    s32 byte3;
    s32 byte1;
    byte0 = scrGetIntPara(0);
    byte3 = scrGetIntPara(3);
    byte2 = scrGetIntPara(2);
    byte1 = scrGetIntPara(1);
    dds3SetMFilterRGBA(((byte0 & 0xFF) | (byte3 << 24)) | (((byte2 & 0xFF) << 16) | ((byte1 & 0xFF) << 8)));
    return 1;
}

extern void dds3SetMFilterArea(u32 extent, u32 centerX, u32 centerY);

s32 scrCommand_MFILTER_AREA(void)
{
    s32 extent;
    s32 centerX;
    extent = scrGetIntPara(0);
    centerX = scrGetIntPara(1);
    dds3SetMFilterArea(extent, centerX, scrGetIntPara(2));
    return 1;
}

extern void dds3MFilterSwap(s32 duration);

s32 scrCommand_MFILTER_SWAP(void)
{
    dds3MFilterSwap(scrGetIntPara(0));
    return 1;
}

extern void dds3MFilterOn(s32 duration);

s32 scrCommand_MFILTER_ON(void)
{
    dds3MFilterOn(scrGetIntPara(0));
    return 1;
}

extern void dds3MFilterOff(s32 duration);

s32 scrCommand_MFILTER_OFF(void)
{
    dds3MFilterOff(scrGetIntPara(0));
    return 1;
}

s32 scrCommandResetDrawEffects(void)
{
    dds3SetBlur(0, 0, 0);
    dds3Blur2Off(0);
    dds3MultiBlurOff(0);
    dds3DistBlurOff(0);
    dds3RipBlurOff(0);
    effDisableFramebufferQuad();
    return 1;
}

extern void fldSetFadeTarget(s32 area, s32 targetMode, s32 duration);
extern void fldSkySetAlphaOfs(u32 alpha);
extern void fldSkySetAlpha(u32 drawState);
extern void fldSetFilterAnime(u32 mode);

s32 scrCommandResetFieldEffects(void)
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
    DDS3_GBWK->script.ints[SCR_PROCESS_CONTROL_FLAG_REGISTER] = 0;
    return 1;
}

s32 scrCommandSetProcessControlFlag(void)
{
    DDS3_GBWK->script.ints[SCR_PROCESS_CONTROL_FLAG_REGISTER] = 1;
    return 1;
}

s32 scrCommandIsProcessControlFlagClear(void)
{
    return DDS3_GBWK->script.ints[SCR_PROCESS_CONTROL_FLAG_REGISTER] == 0;
}

extern void ptyAdjustItemQuantity(s32 itemId, s32 quantityDelta);

s32 scrCommandAdjustItemQuantity(void)
{
    s32 itemId;
    itemId = scrGetIntPara(0);
    ptyAdjustItemQuantity(itemId, scrGetIntPara(1));
    return 1;
}

extern void datAddCurrencyClamped(s32 delta);

s32 scrCommandAddPartyCurrency(void)
{
    datAddCurrencyClamped(scrGetIntPara(0));
    return 1;
}

extern u8 datHasEnoughCurrency(s32 requiredAmount);

/* Persona 4 scrCommand_SCR_EXISTS @ 00299600 (src/Script/scrCommonCommand.c), recompiled unchanged */
u32 scrCommand_SCR_EXISTS()
{
    s32 requiredAmount;
    requiredAmount = scrGetIntPara(0);
    if (datHasEnoughCurrency(requiredAmount))
    {
        scrSetIntReturnValue(1);
    }
    else
    {
        scrSetIntReturnValue(0);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "script/scrCommonCommand", D_0039F550);

INCLUDE_RODATA(const s32, "script/scrCommonCommand", D_0039F570);
