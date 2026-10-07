#include "common.h"
#include "pcp_vu0.h"
#include "scr.h"
s32 kwlnDrawSetCd0Clamped(s32 arg0, s32 arg1, s32 arg2, f32 farg0, f32 farg1, f32 farg2, s32 arg3);
s32 kwlnDrawSetD30Clamped(s32 arg0, f32 farg0, f32 farg1, f32 farg2, f32 farg3, f32 farg4, s32 arg1);

f32 bfWaitReadArgFloat(s32 idx);
s32 evtToggleSavedDrawVectors(s32 arg0, f32 arg1, f32 arg2);
s32 kwlnSetDrawColorTarget(s32 arg0, void *arg1);
void kwlnDrawSetC70FloatTriple(u32 blendControl, f32 rotation, f32 scale);
s32 kwlnSetLightColorTarget(s32 arg0, s32 arg1, void *arg2);
s32 kwlnSetBackgroundColorTarget(s32 arg0, void *arg1);
s32 kwlnSetLightDirectionTarget(s32 arg0, s32 arg1, void *arg2);
void kwlnDrawSetD88FloatTriple(u32 blendControl, f32 rotation, f32 scale);
s32 kwlnDrawSetDc8Second(s32 arg0);
s32 kwlnDrawSetE08Fifth(s32 arg0);
s32 kwlnFadeOutStart(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 kwlnFadeInStart(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void sdfSetViewFieldOfView(f32 arg0);
s32 evtSetDrawVectorTarget(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4);
s32 fptodp(void);
extern ScrComGlobals *datGameState;
extern s8 sdfPadButtonStates[];
extern char D_0039F4D8[];
extern char D_0039F4E8[];
extern char D_0039F508[];
extern char D_0039F530[];
extern char D_0039F550[];
extern char D_0039F570[];

s32 scrCommandStoreRandomCondition(void)
{
    scrSetIntegerReturnValue(effMiscRandMod(0, scrReadIntParameter(0)) + 1);
    return 1;
}

s32 scrCommandWaitForTimerStart(void)
{
    return scrGetCommandTimer() != 0;
}

/* Succeed when the command timer reaches the requested tick count; a
 * nonpositive request completes immediately. */
s32 scrCommandWaitForTimerLimit(void) {
    if (scrReadIntParameter(0) <= 0) {
        return 1;
    }
    if (scrGetCommandTimer() < scrReadIntParameter(0)) {
        return 0;
    }
    return 1;
}

s32 scrCommandPrintInteger(void)
{
    evtPrintDeveloperConsoleMessage("PUT -> %d\n", scrReadIntParameter(0));
    return 1;
}

s32 scrCommandPrintString(void)
{
    evtPrintDeveloperConsoleMessage(D_0039F4D8, scrReadStringParameter(0));
    return 1;
}

s32 bfWaitCbScreenFadeA(void)
{
    s32 mode;

    if (scrGetCommandTimer() == 0)
    {
        mode = scrReadIntParameter(0);
        switch (mode)
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
    s32 mode;

    if (scrGetCommandTimer() == 0)
    {
        mode = scrReadIntParameter(0);
        switch (mode)
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

s32 scrCommandFadeBackgroundIn(void)
{
    if (scrGetCommandTimer() == 0)
    {
        kwlnFadeBackgroundStartIn(scrReadIntParameter(0));
        return 0;
    }
    return 1;
}

s32 scrCommandJumpToIndexedLabel(void)
{
    s32 argumentIndex;
    s32 label;
    argumentIndex = scrReadIntParameter(0);
    if (argumentIndex < 0)
    {
        return 1;
    }
    label = scrReadIntParameter(argumentIndex + 1);
    if (label < 0)
    {
        return 1;
    }
    scrSetProgramCounter(scrGetLabelAddress(label));
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

s32 scrCommandStartPadMotor(void)
{
    s32 p0;
    s32 p1;
    p0 = scrReadIntParameter(0);
    p1 = scrReadIntParameter(1);
    kwlnPadStartMotor(p0, p1 & 0xFF, scrReadIntParameter(2));
    return 1;
}

s32 scrCommandSetCameraFov(void)
{
    f32 fovy;

    fovy = bfWaitReadArgFloat(0) * 0.017453293f;
    if (fovy <= 5.0f || fovy >= 180.0f)
    {
        evtPrintDeveloperConsoleMessage(D_0039F4E8, fovy);
        return 1;
    }
    sdfSetViewFieldOfView(fovy);
    return 1;
}


s32 bfWaitCbCreateTask(void)
{
    s32 index;
    ScrData *ctx;

    index = scrReadIntParameter(0);
    ctx = scrGetCurrentContext();
    if (ctx == NULL)
    {
        return 1;
    }
    if (ctx->task == NULL)
    {
        return 1;
    }
    if (index < 0 || index >= ctx->sections->count)
    {
        return 1;
    }
    scrSetIntegerReturnValue((s32)scrCreateTaskFromContextParameters(
        ctx->task->priority + scrReadIntParameter(1), ctx->scriptHeader,
        ctx->sections, ctx->procedures, ctx->labels,
        ctx->instructions, ctx->auxiliaryData, ctx->strings, index));
    return 1;
}

s32 scrCommandDestroyRegisteredTask(void)
{
    s32 p0;
    p0 = scrReadIntParameter(0);
    if (kwlnTaskIsRegistered(p0) == 0)
    {
        return 1;
    }
    kwlnTaskDestroyWithHierarchy(p0, 1);
    return 1;
}

s32 scrCommandWaitForTaskRemoval(void)
{
    return kwlnTaskIsRegistered(scrReadIntParameter(0)) == 0;
}

s32 scrCommandStoreTaskPresence(void)
{
    if (kwlnTaskIsRegistered(scrReadIntParameter(0)) != 0)
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
    if (!kwlnTaskIsRegistered(task))
    {
        scrSetIntegerReturnValue(0);
    }
    else
    {
        scrSetIntegerReturnValue(kwlnTaskGetTimer(task));
    }
    return 1;
}

s32 scrCommandSetupFadeFrames(void)
{
    s32 p0;
    p0 = scrReadIntParameter(0);
    kwlnFadeSetupFrames(p0, scrReadIntParameter(1));
    return 1;
}

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

INCLUDE_RODATA(const s32, "script/scrCommonCommand", D_0039F4D8);

INCLUDE_RODATA(const s32, "script/scrCommonCommand", D_0039F4E8);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010DE70);

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
    kwlnSetDrawColorTarget(scrReadIntParameter(0), &v);
    return 1;
}

s32 scrCommandSetDrawVectorTarget(void)
{
    s32 p0;
    f32 x;
    f32 y;
    f32 z;
    f32 w;

    p0 = scrReadIntParameter(0);
    x = (f32)(u32)scrReadIntParameter(1);
    y = (f32)(u32)scrReadIntParameter(3);
    z = (f32)(u32)scrReadIntParameter(2);
    w = (f32)(u32)scrReadIntParameter(4);
    evtSetDrawVectorTarget(p0, x, y, z, w);
    return 1;
}

s32 scrCommandToggleSavedDrawVectors(void)
{
    s32 p0;
    p0 = scrReadIntParameter(0);
    evtToggleSavedDrawVectors(p0, bfWaitReadArgFloat(1), bfWaitReadArgFloat(2));
    return 1;
}

s32 scrCommandSetDrawOffsetTransition(void)
{
    s32 p0;
    s32 p1;
    p0 = scrReadIntParameter(0);
    p1 = scrReadIntParameter(1);
    kwlnDrawSetOverlayTransition(p0, p1, scrReadIntParameter(2));
    return 1;
}

extern s32 scrCmdSetDrawFloatPairByMode(void);
INCLUDE_ASM(const s32, "script/scrCommonCommand", scrCmdSetDrawFloatPairByMode);

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

s32 scrCmdSetTexturedBlurIntegerParameters(void)
{
    s32 p0;
    s32 p1;
    p0 = scrReadIntParameter(0);
    p1 = scrReadIntParameter(1);
    kwlnDrawSetC70Triple(p0, p1, scrReadIntParameter(2));
    return 1;
}

s32 scrCmdSetTexturedBlurTransitionMode(void)
{
    kwlnDrawSetupC70FromCh71(scrReadIntParameter(0));
    return 1;
}

s32 scrCmdBeginTexturedBlurActivation(void)
{
    kwlnDrawSetupC70(scrReadIntParameter(0));
    return 1;
}

s32 scrCmdBeginTexturedBlurDeactivation(void)
{
    kwlnDrawSetupC70B(scrReadIntParameter(0));
    return 1;
}

s32 scrCmdConfigureFilterBlur(void)
{
    s32 p2;
    s32 p1;
    s32 p0;
    s32 sel;
    s32 kind;
    f32 f4;
    f32 f3;
    f32 f5;

    sel = scrReadIntParameter(6);
    switch (sel)
    {
    case 1:
        kind = 0x48;
        break;
    case 2:
        kind = 0x42;
        break;
    case 0:
    default:
        kind = 0x44;
        break;
    }
    p0 = scrReadIntParameter(0);
    p1 = scrReadIntParameter(1);
    p2 = scrReadIntParameter(2);
    f3 = bfWaitReadArgFloat(3);
    f4 = bfWaitReadArgFloat(4);
    f5 = bfWaitReadArgFloat(5);
    kwlnDrawSetCd0Clamped(p0, p1, p2, f3, f4, f5, kind);
    return 1;
}

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

s32 scrCmdSetFilterBlurIntegerParameters(void)
{
    s32 p0;
    s32 p1;
    p0 = scrReadIntParameter(0);
    p1 = scrReadIntParameter(1);
    kwlnDrawSetCd0Triple(p0, p1, scrReadIntParameter(2));
    return 1;
}

s32 scrCmdSetFilterBlurTransitionMode(void)
{
    kwlnSetFilterBlurParameterTransition(scrReadIntParameter(0));
    return 1;
}

s32 scrCmdBeginFilterBlurActivation(void)
{
    kwlnDrawSetupCd0(scrReadIntParameter(0));
    return 1;
}

s32 scrCmdBeginFilterBlurDeactivation(void)
{
    kwlnDrawEnableCd0(scrReadIntParameter(0));
    return 1;
}

s32 scrCmdConfigureStaggeredBlur(void)
{
    s32 p0;
    s32 mode;
    s32 sel;
    f32 f1;
    f32 f2;
    f32 f3;
    f32 f4;
    f32 f5;

    mode = scrReadIntParameter(6);
    switch (mode)
    {
    case 1:
        sel = 0x48;
        break;
    case 2:
        sel = 0x42;
        break;
    case 0:
    default:
        sel = 0x44;
        break;
    }
    p0 = scrReadIntParameter(0);
    f1 = bfWaitReadArgFloat(1);
    f2 = bfWaitReadArgFloat(2);
    f3 = bfWaitReadArgFloat(3);
    f4 = bfWaitReadArgFloat(4);
    f5 = bfWaitReadArgFloat(5);
    kwlnDrawSetD30Clamped(p0, f1, f2, f3, f4, f5, sel);
    return 1;
}

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

s32 scrCmdSetStaggeredBlurIntegerParameters(void)
{
    s32 p0;
    s32 p1;
    p0 = scrReadIntParameter(0);
    p1 = scrReadIntParameter(1);
    kwlnDrawSetD30Triple(p0, p1, scrReadIntParameter(2));
    return 1;
}

s32 scrCmdSetStaggeredBlurTransitionMode(void)
{
    kwlnSetStaggeredBlurParameterTransition(scrReadIntParameter(0));
    return 1;
}

s32 scrCmdBeginStaggeredBlurActivation(void)
{
    kwlnDrawSetupD30(scrReadIntParameter(0));
    return 1;
}

s32 scrCmdBeginStaggeredBlurDeactivation(void)
{
    kwlnDrawEnableD30(scrReadIntParameter(0));
    return 1;
}

extern s32 scrCmdConfigureRectangleBlur(void);
INCLUDE_ASM(const s32, "script/scrCommonCommand", scrCmdConfigureRectangleBlur);

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

s32 scrCmdSetRectangleBlurIntegerPair(void)
{
    s32 p0;
    p0 = scrReadIntParameter(0);
    kwlnDrawSetD88Pair(p0, scrReadIntParameter(1));
    return 1;
}

s32 scrCmdSetRectangleBlurTransitionMode(void)
{
    kwlnSetRectangleBlurParameterTransition(scrReadIntParameter(0));
    return 1;
}

s32 scrCmdBeginRectangleBlurActivation(void)
{
    kwlnDrawSetupD88(scrReadIntParameter(0));
    return 1;
}

s32 scrCmdBeginRectangleBlurDeactivation(void)
{
    kwlnDrawEnableD88(scrReadIntParameter(0));
    return 1;
}

s32 scrCmdSetIndexedDrawMode(void)
{
    s32 p0;
    s32 mode;
    p0 = scrReadIntParameter(0);
    switch (p0)
    {
    case 1:
        mode = 0x48;
        break;
    case 0:
        mode = 0x44;
        break;
    case 2:
        mode = 0x42;
        break;
    case 3:
        mode = 6;
        break;
    default:
        evtPrintDeveloperConsoleMessage(D_0039F550);
        mode = 0x44;
        break;
    }
    kwlnDrawSetDc8Second(mode);
    return 1;
}

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

s32 scrCmdSetColorRectangleTransitionMode(void)
{
    kwlnDrawSnapshotSolidRect(scrReadIntParameter(0));
    return 1;
}

s32 scrCmdBeginColorRectangleActivation(void)
{
    kwlnDrawSetupDc8(scrReadIntParameter(0));
    return 1;
}

s32 scrCmdBeginColorRectangleDeactivation(void)
{
    kwlnDrawEnableDc8(scrReadIntParameter(0));
    return 1;
}

s32 scrCmdSetTexturedSquareDrawMode(void)
{
    s32 p0;
    s32 mode;
    p0 = scrReadIntParameter(0);
    switch (p0)
    {
    case 1:
        mode = 0x48;
        break;
    case 0:
        mode = 0x44;
        break;
    case 2:
        mode = 0x42;
        break;
    case 3:
        mode = 6;
        break;
    default:
        evtPrintDeveloperConsoleMessage(D_0039F570);
        mode = 0x44;
        break;
    }
    kwlnDrawSetE08Fifth(mode);
    return 1;
}

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

s32 scrCmdSetTexturedSquareIntegerParameters(void)
{
    s32 p0;
    s32 p1;
    p0 = scrReadIntParameter(0);
    p1 = scrReadIntParameter(1);
    kwlnDrawSetE08Triple(p0, p1, scrReadIntParameter(2));
    return 1;
}

s32 scrCmdSetTexturedSquareTransitionMode(void)
{
    kwlnDrawSetupE08FromCh75(scrReadIntParameter(0));
    return 1;
}

s32 scrCmdBeginTexturedSquareActivation(void)
{
    kwlnDrawSetupE08(scrReadIntParameter(0));
    return 1;
}

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

s32 scrCommandResetFieldEffects(void)
{
    kwlnDrawEnableDc8(0);
    kwlnDrawEnableE08(0);
    fldSetSwayMode(0);
    fldSetSkyDrawState(0x80);
    func_00132B80(0);
    fldSetFadeTarget(0, 1, 0);
    return 1;
}

s32 scrCommandClearProcessControlFlag(void)
{
    datGameState->unk388 = 0;
    return 1;
}

s32 scrCommandSetProcessControlFlag(void)
{
    datGameState->unk388 = 1;
    return 1;
}

s32 scrCommandIsProcessControlFlagClear(void)
{
    return datGameState->unk388 == 0;
}

s32 func_0010EE40(void)
{
    s32 p0;
    p0 = scrReadIntParameter(0);
    ptyAdjustItemQuantity(p0, scrReadIntParameter(1));
    return 1;
}

s32 scrCommandAddPartyCurrency(void)
{
    datAddCurrencyClamped(scrReadIntParameter(0));
    return 1;
}

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

INCLUDE_RODATA(const s32, "script/scrCommonCommand", D_0039F550);

INCLUDE_RODATA(const s32, "script/scrCommonCommand", D_0039F570);

