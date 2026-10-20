#include "common.h"
#include "scr_integer_return_value_api.h"
#include "kwln_background_fade_api.h"
#include "eff_random_mod.h"
#include "fpu.h"
#include "kwln.h"
#include "pcp_vu0.h"
#include "scr.h"
#include "scr_string_parameter_api.h"
#include "scr_arg_read.h"
#include "kwln_task_lifecycle.h"
#include "kwln_fade.h"
#include "eff_event_draw.h"

s32 kwlnDrawSetCd0Clamped(s32 filterCount, s32 size, s32 delaySpread, f32 angleStep,
                          f32 uvDisplacementAngleDegrees, f32 uvDisplacementAmplitude,
                          s32 blendControl);
s32 kwlnDrawSetD30Clamped(s32 count, f32 phaseStep, f32 spacing,
                          f32 uvDisplacementAngleDegrees, f32 uvDisplacementAmplitude,
                          f32 angleStep, s32 blendControl);


extern char D_00412648[];

extern char D_00412658[];

extern s8 sdfPadButtonStates[];


s32 kwlnSetLightColorTarget(s32 blendFrames, s32 lightIndex, void *color);

s32 kwlnSetBackgroundColorTarget(s32 blendFrames, void *color);

void kwlnSetDrawColorTarget(s32 blendFrames, f32 *color);

s32 evtToggleSavedDrawVectors(s32 frames, f32 firstTarget, f32 secondTarget);
s32 evtSetDrawVectorTarget(s32 blendFrames, f32 x, f32 y, f32 z, f32 w);

s32 kwlnDrawSetDc8Second(s32 blendControl);

extern char D_004126D0[];

s32 kwlnDrawSetE08Fifth(s32 blendControl);
void kwlnDrawSetC70FloatTriple(f32 rotation, f32 scale, u32 blendControl);
void kwlnDrawSetD88FloatTriple(f32 rotation, f32 scale, u32 blendControl);

extern char D_004126F0[];
extern char D_004126B0[];

extern ScrComGlobals *datGameState;

extern s32 scrReadIntParameter(s32 parameterIndex);
extern u32 scrGetCommandTimer(void);

s32 scrCommandStoreRandomCondition(void)
{
    scrSetIntegerReturnValue(sdfGetRandInt(0, scrReadIntParameter(0)) + 1);
    return 1;
}

s32 scrCommandWaitForTimerStart(void)
{
    return scrGetCommandTimer() != 0;
}

/* DDS2 twin of DDS1 func_0010D7C0: BF wait step-ticks-at-least callback. */
s32 scrCommandWaitForTimerLimit(void) {
    if (scrReadIntParameter(0) <= 0) {
        return 1;
    }
    /* VM timer limits use signed words; retail +0x30 is slt, not sltu. */
    if ((s32)scrGetCommandTimer() < scrReadIntParameter(0)) {
        return 0;
    }
    return 1;
}

extern void evtPrintDeveloperConsoleMessage(const char *format, ...);

s32 scrCommandPrintInteger(void)
{
    evtPrintDeveloperConsoleMessage(D_00412648, scrReadIntParameter(0));
    return 1;
}

s32 scrCommandPrintString(void)
{
    evtPrintDeveloperConsoleMessage(D_00412658, scrReadStringParameter(0));
    return 1;
}

s32 bfWaitCbScreenFadeA(void)
{
    s32 colorMode;

    if (scrGetCommandTimer() == 0)
    {
        colorMode = scrReadIntParameter(0);
        switch (colorMode)
        {
        case 0:
            kwlnFadeOutStart(0, 0, 0, scrReadIntParameter(1));
            break;
        case 1:
            kwlnFadeOutStart(0xFF, 0xFF, 0xFF, scrReadIntParameter(1));
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
        colorMode = scrReadIntParameter(0);
        switch (colorMode)
        {
        case 0:
            kwlnFadeInStart(0, 0, 0, scrReadIntParameter(1));
            break;
        case 1:
            kwlnFadeInStart(0xFF, 0xFF, 0xFF, scrReadIntParameter(1));
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
        kwlnFadeBackgroundStartOut(scrReadIntParameter(0));
        return 0;
    }
    return 1;
}

extern void kwlnFadeBackgroundStartIn(s32 duration);

s32 scrCommandFadeBackgroundIn(void)
{
    if (scrGetCommandTimer() == 0)
    {
        kwlnFadeBackgroundStartIn(scrReadIntParameter(0));
        return 0;
    }
    return 1;
}

extern u32 scrGetLabelAddress(s32 labelIndex);
extern void scrSetProgramCounter(u32 address);

s32 scrCommandJumpToIndexedLabel(void)
{
    s32 argumentIndex;
    s32 labelIndex;
    argumentIndex = scrReadIntParameter(0);
    if (argumentIndex < 0)
    {
        return 1;
    }
    labelIndex = scrReadIntParameter(argumentIndex + 1);
    if (labelIndex < 0)
    {
        return 1;
    }
    scrSetProgramCounter(scrGetLabelAddress(labelIndex));
    return 1;
}

s32 scrCommandTestIndexedCodeNegative(void)
{
    scrSetIntegerReturnValue(sdfPadButtonStates[scrReadIntParameter(0)] < 0);
    return 1;
}

s32 scrCommandTestIndexedCodeBit(void)
{
    scrSetIntegerReturnValue(sdfPadButtonStates[scrReadIntParameter(0)] & 1);
    return 1;
}

extern void kwlnPadStartMotor(u32 motor, u8 level, s32 duration);

s32 scrCommandStartPadMotor(void)
{
    s32 motor;
    s32 motorLevel;
    motor = scrReadIntParameter(0);
    motorLevel = scrReadIntParameter(1);
    kwlnPadStartMotor(motor, motorLevel & 0xFF, scrReadIntParameter(2));
    return 1;
}

extern char D_00412668[];
void sdfSetViewFieldOfView(f32 fieldOfView);

s32 scrCommandSetCameraFov(void)
{
    f32 fovy;

    fovy = bfWaitReadArgFloat(0) * 0.017453293f;
    if (fovy <= 5.0f || fovy >= 180.0f)
    {
        evtPrintDeveloperConsoleMessage(D_00412668, fovy);
        return 1;
    }
    sdfSetViewFieldOfView(fovy);
    return 1;
}


s32 bfWaitCbCreateTask(void)
{
    s32 procedureIndex;
    ScrData *ctx;

    procedureIndex = scrReadIntParameter(0);
    ctx = scrGetCurrentContext();
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
    scrSetIntegerReturnValue((s32)scrCreateTaskFromContextParameters(
        ctx->task->priority + scrReadIntParameter(1), ctx->scriptHeader,
        ctx->sections, ctx->procedures, ctx->labels,
        ctx->instructions, ctx->auxiliaryData, ctx->strings, procedureIndex));
    return 1;
}

s32 scrCommandDestroyRegisteredTask(void)
{
    s32 taskHandle;
    taskHandle = scrReadIntParameter(0);
    if (dds3ExistProcess((KwlnTask *)taskHandle) == 0)
    {
        return 1;
    }
    dds3KillProcess((KwlnTask *)taskHandle, 1);
    return 1;
}

s32 scrCommandWaitForTaskRemoval(void)
{
    return dds3ExistProcess((KwlnTask *)scrReadIntParameter(0)) == 0;
}

s32 scrCommandStoreTaskPresence(void)
{
    if (dds3ExistProcess((KwlnTask *)scrReadIntParameter(0)) != 0)
    {
        scrSetIntegerReturnValue(1);
    }
    else
    {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

/* Persona 4 scrCommand_SCR_GET_TIMER @ 00299660 (src/Script/scrCommonCommand.c), recompiled unchanged */
u32 scrCommand_SCR_GET_TIMER()
{
    KwlnTask* task;
    task = (KwlnTask*)scrReadIntParameter(0);
    if (!dds3ExistProcess(task))
    {
        scrSetIntegerReturnValue(0);
    }
    else
    {
        scrSetIntegerReturnValue(dds3GetProcessRunTime(task));
    }
    return 1;
}

extern void kwlnFadeSetupFrames(s32 fadeMode, s32 frames);

s32 scrCommandSetupFadeFrames(void)
{
    s32 fadeMode;
    s32 durationFrames;
    fadeMode = scrReadIntParameter(0);
    durationFrames = scrReadIntParameter(1);
    kwlnFadeSetupFrames(fadeMode, durationFrames);
    return 1;
}

extern void kwlnCancelConfiguredFadeFrames(void);

s32 scrCommandCancelConfiguredFrameFade(void)
{
    kwlnCancelConfiguredFadeFrames();
    return 1;
}

s32 scrCmdStoreDirectionVector(void)
{
    ScrVec4 v;
    f32 x;
    f32 y;
    f32 z;
    x = bfWaitReadArgFloat(1);
    VU0_SET_AXIS_GPR(x, x);
    y = bfWaitReadArgFloat(2);
    VU0_SET_AXIS_GPR(y, y);
    z = bfWaitReadArgFloat(3);
    VU0_SET_AXIS_GPR(z, z);
    VU0_CLEAR_W(vf10);
    VU0_STORE_VF_TO(vf10, v);
    kwlnSetLightColorTarget(scrReadIntParameter(0), 0, &v);
    return 1;
}

INCLUDE_RODATA(const s32, "script/scrCommonCommand", D_00412648);

INCLUDE_RODATA(const s32, "script/scrCommonCommand", D_00412658);

INCLUDE_RODATA(const s32, "script/scrCommonCommand", D_00412668);

const char scrSceneLightDirectionZeroWarning[] = "WARNING: SCENE_LIGHT_DIR length zero\n";
extern s32 kwlnSetLightDirectionTarget(s32 blendFrames, s32 lightIndex, void *direction);

s32 scrCommandSetSceneLightDirection(void) {
    ScrVecW direction;

    direction.x = bfWaitReadArgFloat(1);
    direction.y = bfWaitReadArgFloat(2);
    direction.z = bfWaitReadArgFloat(3);
    direction.w = 0;
    if (ffabsf(direction.x) <= 0.01f &&
        ffabsf(direction.y) <= 0.01f &&
        ffabsf(direction.z) <= 0.01f) {
        evtPrintDeveloperConsoleMessage(scrSceneLightDirectionZeroWarning);
        return 1;
    }
    kwlnSetLightDirectionTarget(scrReadIntParameter(0), 0, &direction);
    return 1;
}

s32 scrCmdStorePositionVector(void)
{
    ScrVec4 v;
    f32 x;
    f32 y;
    f32 z;
    x = bfWaitReadArgFloat(1);
    VU0_SET_AXIS_GPR(x, x);
    y = bfWaitReadArgFloat(2);
    VU0_SET_AXIS_GPR(y, y);
    z = bfWaitReadArgFloat(3);
    VU0_SET_AXIS_GPR(z, z);
    VU0_SET_W_ONE(vf10);
    VU0_STORE_VF_TO(vf10, v);
    kwlnSetBackgroundColorTarget(scrReadIntParameter(0), &v);
    return 1;
}

s32 scrCmdSetPackedRgbFromFloatArgs(void)
{
    ScrVecW v;
    v.x = bfWaitReadArgFloat(1);
    v.y = bfWaitReadArgFloat(2);
    v.z = bfWaitReadArgFloat(3);
    v.w = 0;
    kwlnSetDrawColorTarget(scrReadIntParameter(0), &v.x);
    return 1;
}

s32 scrCommandSetDrawVectorTarget(void)
{
    s32 blendFrames;
    f32 x;
    f32 y;
    f32 z;
    f32 w;

    blendFrames = scrReadIntParameter(0);
    x = (f32)(u32)scrReadIntParameter(1);
    y = (f32)(u32)scrReadIntParameter(3);
    z = (f32)(u32)scrReadIntParameter(2);
    w = (f32)(u32)scrReadIntParameter(4);
    evtSetDrawVectorTarget(blendFrames, x, y, z, w);
    return 1;
}

s32 scrCommandToggleSavedDrawVectors(void)
{
    s32 frames;
    frames = scrReadIntParameter(0);
    evtToggleSavedDrawVectors(frames, bfWaitReadArgFloat(1), bfWaitReadArgFloat(2));
    return 1;
}

extern void kwlnDrawSetOverlayTransition(s32 duration, s32 alpha, s32 scale);

s32 scrCommandSetDrawOffsetTransition(void)
{
    s32 duration;
    s32 alpha;
    duration = scrReadIntParameter(0);
    alpha = scrReadIntParameter(1);
    kwlnDrawSetOverlayTransition(duration, alpha, scrReadIntParameter(2));
    return 1;
}

s32 scrCmdSetDrawFloatPairByMode(void) {
    u32 blendControl;
    f32 rotation;
    f32 scale;

    switch (scrReadIntParameter(2)) {
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
    rotation = bfWaitReadArgFloat(0);
    scale = bfWaitReadArgFloat(1);
    kwlnDrawSetC70FloatTriple(rotation, scale, blendControl);
    return 1;
}

extern void kwlnDrawSetC70Second(u32 rgba);

s32 scrCmdSetPackedDrawComponentBytes(void)
{
    /* Declared out of order: gcc 2.96 fills $16-$18 in declaration order. */
    s32 byte2;
    s32 byte0;
    s32 byte3;
    s32 byte1;
    byte0 = scrReadIntParameter(0);
    byte3 = scrReadIntParameter(3);
    byte2 = scrReadIntParameter(2);
    byte1 = scrReadIntParameter(1);
    kwlnDrawSetC70Second(((byte0 & 0xFF) | (byte3 << 24)) | (((byte2 & 0xFF) << 16) | ((byte1 & 0xFF) << 8)));
    return 1;
}

extern void kwlnDrawSetC70Triple(u32 extent, u32 centerX, u32 centerY);

s32 scrCmdSetTexturedBlurIntegerParameters(void)
{
    s32 extent;
    s32 centerX;
    s32 centerY;
    extent = scrReadIntParameter(0);
    centerX = scrReadIntParameter(1);
    centerY = scrReadIntParameter(2);
    kwlnDrawSetC70Triple(extent, centerX, centerY);
    return 1;
}

extern void kwlnDrawSetupC70FromCh71(s32 duration);

s32 scrCmdSetTexturedBlurTransitionMode(void)
{
    kwlnDrawSetupC70FromCh71(scrReadIntParameter(0));
    return 1;
}

extern void kwlnDrawSetupC70(s32 duration);

s32 scrCmdBeginTexturedBlurActivation(void)
{
    kwlnDrawSetupC70(scrReadIntParameter(0));
    return 1;
}

extern void kwlnDrawSetupC70B(s32 duration);

s32 scrCmdBeginTexturedBlurDeactivation(void)
{
    kwlnDrawSetupC70B(scrReadIntParameter(0));
    return 1;
}

s32 scrCmdConfigureFilterBlur(void)
{
    s32 delaySpread;
    s32 size;
    s32 filterCount;
    s32 blendMode;
    s32 blendControl;
    f32 uvDisplacementAngleDegrees;
    f32 angleStep;
    f32 uvDisplacementAmplitude;

    blendMode = scrReadIntParameter(6);
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
    filterCount = scrReadIntParameter(0);
    size = scrReadIntParameter(1);
    delaySpread = scrReadIntParameter(2);
    angleStep = bfWaitReadArgFloat(3);
    uvDisplacementAngleDegrees = bfWaitReadArgFloat(4);
    uvDisplacementAmplitude = bfWaitReadArgFloat(5);
    kwlnDrawSetCd0Clamped(filterCount, size, delaySpread, angleStep,
                           uvDisplacementAngleDegrees, uvDisplacementAmplitude,
                           blendControl);
    return 1;
}

extern void kwlnDrawSetCd0Fourth(u32 rgba);

s32 scrCmdSetFilterBlurPackedColor(void)
{
    /* Declared out of order: gcc 2.96 fills $16-$18 in declaration order. */
    s32 byte2;
    s32 byte0;
    s32 byte3;
    s32 byte1;
    byte0 = scrReadIntParameter(0);
    byte3 = scrReadIntParameter(3);
    byte2 = scrReadIntParameter(2);
    byte1 = scrReadIntParameter(1);
    kwlnDrawSetCd0Fourth(((byte0 & 0xFF) | (byte3 << 24)) | (((byte2 & 0xFF) << 16) | ((byte1 & 0xFF) << 8)));
    return 1;
}

extern void kwlnDrawSetCd0Triple(u32 positionSpread, u32 x, u32 y);

s32 scrCmdSetFilterBlurIntegerParameters(void)
{
    s32 positionSpread;
    s32 x;
    positionSpread = scrReadIntParameter(0);
    x = scrReadIntParameter(1);
    kwlnDrawSetCd0Triple(positionSpread, x, scrReadIntParameter(2));
    return 1;
}

extern void kwlnSetFilterBlurParameterTransition(s32 duration);

s32 scrCmdSetFilterBlurTransitionMode(void)
{
    kwlnSetFilterBlurParameterTransition(scrReadIntParameter(0));
    return 1;
}

extern void kwlnDrawSetupCd0(s32 duration);

s32 scrCmdBeginFilterBlurActivation(void)
{
    kwlnDrawSetupCd0(scrReadIntParameter(0));
    return 1;
}

extern void kwlnDrawEnableCd0(s32 duration);

s32 scrCmdBeginFilterBlurDeactivation(void)
{
    kwlnDrawEnableCd0(scrReadIntParameter(0));
    return 1;
}

s32 scrCmdConfigureStaggeredBlur(void)
{
    s32 count;
    s32 blendMode;
    s32 blendControl;
    f32 phaseStep;
    f32 spacing;
    f32 uvDisplacementAngleDegrees;
    f32 uvDisplacementAmplitude;
    f32 angleStep;

    blendMode = scrReadIntParameter(6);
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
    count = scrReadIntParameter(0);
    phaseStep = bfWaitReadArgFloat(1);
    spacing = bfWaitReadArgFloat(2);
    uvDisplacementAngleDegrees = bfWaitReadArgFloat(3);
    uvDisplacementAmplitude = bfWaitReadArgFloat(4);
    angleStep = bfWaitReadArgFloat(5);
    kwlnDrawSetD30Clamped(count, phaseStep, spacing, uvDisplacementAngleDegrees,
                          uvDisplacementAmplitude, angleStep, blendControl);
    return 1;
}

extern void kwlnDrawSetD30Fourth(u32 rgba);

s32 scrCmdSetStaggeredBlurPackedColor(void)
{
    /* Declared out of order: gcc 2.96 fills $16-$18 in declaration order. */
    s32 byte2;
    s32 byte0;
    s32 byte3;
    s32 byte1;
    byte0 = scrReadIntParameter(0);
    byte3 = scrReadIntParameter(3);
    byte2 = scrReadIntParameter(2);
    byte1 = scrReadIntParameter(1);
    kwlnDrawSetD30Fourth(((byte0 & 0xFF) | (byte3 << 24)) | (((byte2 & 0xFF) << 16) | ((byte1 & 0xFF) << 8)));
    return 1;
}

extern void kwlnDrawSetD30Triple(u32 size, u32 x, u32 y);

s32 scrCmdSetStaggeredBlurIntegerParameters(void)
{
    s32 size;
    s32 x;
    size = scrReadIntParameter(0);
    x = scrReadIntParameter(1);
    kwlnDrawSetD30Triple(size, x, scrReadIntParameter(2));
    return 1;
}

extern void kwlnSetStaggeredBlurParameterTransition(s32 duration);

s32 scrCmdSetStaggeredBlurTransitionMode(void)
{
    kwlnSetStaggeredBlurParameterTransition(scrReadIntParameter(0));
    return 1;
}

extern void kwlnDrawSetupD30(s32 duration);

s32 scrCmdBeginStaggeredBlurActivation(void)
{
    kwlnDrawSetupD30(scrReadIntParameter(0));
    return 1;
}

extern void kwlnDrawEnableD30(s32 duration);

s32 scrCmdBeginStaggeredBlurDeactivation(void)
{
    kwlnDrawEnableD30(scrReadIntParameter(0));
    return 1;
}

s32 scrCmdConfigureRectangleBlur(void)
{
    s32 blendControl;
    s32 blendMode;
    f32 rotation;
    f32 scale;

    blendMode = scrReadIntParameter(2);
    switch (blendMode) {
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
        evtPrintDeveloperConsoleMessage("error: BLUR2_PARA blend\n");
        blendControl = 0x44;
        break;
    }

    rotation = bfWaitReadArgFloat(0);
    scale = bfWaitReadArgFloat(1);
    kwlnDrawSetD88FloatTriple(rotation, scale, blendControl);
    return 1;
}


extern void kwlnDrawSetD88First(u32 rgba);

s32 scrCmdSetRectangleBlurPackedColor(void)
{
    /* Declared out of order: gcc 2.96 fills $16-$18 in declaration order. */
    s32 byte2;
    s32 byte0;
    s32 byte3;
    s32 byte1;
    byte0 = scrReadIntParameter(0);
    byte3 = scrReadIntParameter(3);
    byte2 = scrReadIntParameter(2);
    byte1 = scrReadIntParameter(1);
    kwlnDrawSetD88First(((byte0 & 0xFF) | (byte3 << 24)) | (((byte2 & 0xFF) << 16) | ((byte1 & 0xFF) << 8)));
    return 1;
}

extern void kwlnDrawSetD88Pair(u32 centerX, u32 centerY);

s32 scrCmdSetRectangleBlurIntegerPair(void)
{
    s32 centerX;
    centerX = scrReadIntParameter(0);
    kwlnDrawSetD88Pair(centerX, scrReadIntParameter(1));
    return 1;
}

extern void kwlnSetRectangleBlurParameterTransition(s32 duration);

s32 scrCmdSetRectangleBlurTransitionMode(void)
{
    kwlnSetRectangleBlurParameterTransition(scrReadIntParameter(0));
    return 1;
}

extern void kwlnDrawSetupD88(s32 duration);

s32 scrCmdBeginRectangleBlurActivation(void)
{
    kwlnDrawSetupD88(scrReadIntParameter(0));
    return 1;
}

extern void kwlnDrawEnableD88(s32 duration);

s32 scrCmdBeginRectangleBlurDeactivation(void)
{
    kwlnDrawEnableD88(scrReadIntParameter(0));
    return 1;
}

s32 scrCmdSetIndexedDrawMode(void)
{
    s32 blendMode;
    s32 blendControl;
    blendMode = scrReadIntParameter(0);
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
        evtPrintDeveloperConsoleMessage(D_004126D0);
        blendControl = 0x44;
        break;
    }
    kwlnDrawSetDc8Second(blendControl);
    return 1;
}

extern void kwlnDrawSetDc8First(u32 rgba);

s32 scrCmdSetColorRectanglePackedColor(void)
{
    /* Declared out of order: gcc 2.96 fills $16-$18 in declaration order. */
    s32 byte2;
    s32 byte0;
    s32 byte3;
    s32 byte1;
    byte0 = scrReadIntParameter(0);
    byte3 = scrReadIntParameter(3);
    byte2 = scrReadIntParameter(2);
    byte1 = scrReadIntParameter(1);
    kwlnDrawSetDc8First(((byte0 & 0xFF) | (byte3 << 24)) | (((byte2 & 0xFF) << 16) | ((byte1 & 0xFF) << 8)));
    return 1;
}

extern void kwlnDrawSnapshotSolidRect(s32 duration);

s32 scrCmdSetColorRectangleTransitionMode(void)
{
    kwlnDrawSnapshotSolidRect(scrReadIntParameter(0));
    return 1;
}

extern void kwlnDrawSetupDc8(s32 duration);

s32 scrCmdBeginColorRectangleActivation(void)
{
    kwlnDrawSetupDc8(scrReadIntParameter(0));
    return 1;
}

extern void kwlnDrawEnableDc8(s32 duration);

s32 scrCmdBeginColorRectangleDeactivation(void)
{
    kwlnDrawEnableDc8(scrReadIntParameter(0));
    return 1;
}

s32 scrCmdSetTexturedSquareDrawMode(void)
{
    s32 blendMode;
    s32 blendControl;
    blendMode = scrReadIntParameter(0);
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
        evtPrintDeveloperConsoleMessage(D_004126F0);
        blendControl = 0x44;
        break;
    }
    kwlnDrawSetE08Fifth(blendControl);
    return 1;
}

extern void kwlnDrawSetE08Fourth(u32 rgba);

s32 scrCmdSetTexturedSquarePackedColor(void)
{
    /* Declared out of order: gcc 2.96 fills $16-$18 in declaration order. */
    s32 byte2;
    s32 byte0;
    s32 byte3;
    s32 byte1;
    byte0 = scrReadIntParameter(0);
    byte3 = scrReadIntParameter(3);
    byte2 = scrReadIntParameter(2);
    byte1 = scrReadIntParameter(1);
    kwlnDrawSetE08Fourth(((byte0 & 0xFF) | (byte3 << 24)) | (((byte2 & 0xFF) << 16) | ((byte1 & 0xFF) << 8)));
    return 1;
}

extern void kwlnDrawSetE08Triple(u32 extent, u32 centerX, u32 centerY);

s32 scrCmdSetTexturedSquareIntegerParameters(void)
{
    s32 extent;
    s32 centerX;
    extent = scrReadIntParameter(0);
    centerX = scrReadIntParameter(1);
    kwlnDrawSetE08Triple(extent, centerX, scrReadIntParameter(2));
    return 1;
}

extern void kwlnDrawSetupE08FromCh75(s32 duration);

s32 scrCmdSetTexturedSquareTransitionMode(void)
{
    kwlnDrawSetupE08FromCh75(scrReadIntParameter(0));
    return 1;
}

extern void kwlnDrawSetupE08(s32 duration);

s32 scrCmdBeginTexturedSquareActivation(void)
{
    kwlnDrawSetupE08(scrReadIntParameter(0));
    return 1;
}

extern void kwlnDrawEnableE08(s32 duration);

s32 scrCmdBeginTexturedSquareDeactivation(void)
{
    kwlnDrawEnableE08(scrReadIntParameter(0));
    return 1;
}

s32 scrCommandResetDrawEffects(void)
{
    kwlnDrawSetOverlayTransition(0, 0, 0);
    kwlnDrawEnableD88(0);
    kwlnDrawSetupC70B(0);
    kwlnDrawEnableCd0(0);
    kwlnDrawEnableD30(0);
    effDisableFramebufferQuad();
    return 1;
}

extern void fldSetFadeTarget(s32 area, s32 targetMode, s32 duration);
extern void fldSetSkyAlpha(u32 alpha);
extern void fldSetSkyDrawState(u32 drawState);
extern void fldSetSwayMode(u32 mode);

s32 scrCommandResetFieldEffects(void)
{
    kwlnDrawEnableDc8(0);
    kwlnDrawEnableE08(0);
    fldSetSwayMode(0);
    fldSetSkyDrawState(0x80);
    fldSetSkyAlpha(0);
    fldSetFadeTarget(0, 1, 0);
    return 1;
}

s32 scrCommandClearProcessControlFlag(void)
{
    datGameState->processControlFlag = 0;
    return 1;
}

s32 scrCommandSetProcessControlFlag(void)
{
    datGameState->processControlFlag = 1;
    return 1;
}

s32 scrCommandIsProcessControlFlagClear(void)
{
    return datGameState->processControlFlag == 0;
}

extern void ptyAdjustItemQuantity(s32 itemId, s32 quantityDelta);

s32 scrCommandAdjustItemQuantity(void)
{
    s32 itemId;
    itemId = scrReadIntParameter(0);
    ptyAdjustItemQuantity(itemId, scrReadIntParameter(1));
    return 1;
}

extern void datAddCurrencyClamped(s32 delta);

s32 scrCommandAddPartyCurrency(void)
{
    datAddCurrencyClamped(scrReadIntParameter(0));
    return 1;
}

extern s32 datHasEnoughCurrency(s32 requiredAmount);

/* Persona 4 scrCommand_SCR_EXISTS @ 00299600 (src/Script/scrCommonCommand.c), recompiled unchanged */
u32 scrCommand_SCR_EXISTS()
{
    KwlnTask* task;
    task = (KwlnTask*)scrReadIntParameter(0);
    if (datHasEnoughCurrency(task))
    {
        scrSetIntegerReturnValue(1);
    }
    else
    {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "script/scrCommonCommand", D_004126D0);

INCLUDE_RODATA(const s32, "script/scrCommonCommand", D_004126F0);

