#include "common.h"
#include "scr.h"
s32 kwlnDrawSetCd0Clamped(s32 arg0, s32 arg1, s32 arg2, f32 farg0, f32 farg1, f32 farg2, s32 arg3);

typedef struct KwlnTask KwlnTask;
s32 kwlnFadeOutStart(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 kwlnFadeInStart(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

extern char D_00412648[];

extern char D_00412658[];

extern s8 D_0040B7D8[];

f32 bfWaitReadArgFloat(s32 idx);

s32 func_00107EF8(s32 arg0, s32 arg1, void *arg2);

s32 func_00108138(s32 arg0, void *arg1);

s32 func_001081F8(s32 arg0, void *arg1);

s32 evtUnk89F8SetState(s32 arg0, f32 arg1, f32 arg2);

s32 kwlnDrawSetDc8Second(s32 arg0);

extern char D_004126D0[];

s32 kwlnDrawSetE08Fifth(s32 arg0);
/* Declared floats-first: gcc 2.96 emits the outgoing register moves in
 * parameter order and schedules the last one into the jal delay slot, so
 * retail moves the kind argument ($16) last, in the delay slot. */
s32 kwlnDrawSetC70FloatTriple(f32 arg0, f32 arg1, s32 arg2);

extern char D_004126F0[];

extern ScrComGlobals *D_00435DD0;

s32 func_0010D990(void)
{
    func_0010D818(effMiscRandMod(0, func_0010D650(0)) + 1);
    return 1;
}

s32 func_0010D9C8(void)
{
    return scrGetCommandTimer() != 0;
}

/* DDS2 twin of DDS1 func_0010D7C0: BF wait step-ticks-at-least callback. */
s32 func_0010D9E8(void) {
    if (func_0010D650(0) <= 0) {
        return 1;
    }
    if (scrGetCommandTimer() < func_0010D650(0)) {
        return 0;
    }
    return 1;
}

s32 func_0010DA30(void)
{
    func_0010AE38(D_00412648, func_0010D650(0));
    return 1;
}

s32 func_0010DA60(void)
{
    func_0010AE38(D_00412658, func_0010D7D0(0));
    return 1;
}

s32 bfWaitCbScreenFadeA(void)
{
    s32 mode;

    if (scrGetCommandTimer() == 0)
    {
        mode = func_0010D650(0);
        switch (mode)
        {
        case 0:
            kwlnFadeOutStart(0, 0, 0, func_0010D650(1));
            break;
        case 1:
            kwlnFadeOutStart(0xFF, 0xFF, 0xFF, func_0010D650(1));
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
        mode = func_0010D650(0);
        switch (mode)
        {
        case 0:
            kwlnFadeInStart(0, 0, 0, func_0010D650(1));
            break;
        case 1:
            kwlnFadeInStart(0xFF, 0xFF, 0xFF, func_0010D650(1));
            break;
        default:
            return 1;
        }
        return 0;
    }
    return 1;
}

s32 func_0010DBD0(void)
{
    if (scrGetCommandTimer() == 0)
    {
        func_00105FE8(func_0010D650(0));
        return 0;
    }
    return 1;
}

s32 func_0010DC10(void)
{
    if (scrGetCommandTimer() == 0)
    {
        func_00106080(func_0010D650(0));
        return 0;
    }
    return 1;
}

s32 func_0010DC50(void)
{
    s32 argumentIndex;
    s32 label;
    argumentIndex = func_0010D650(0);
    if (argumentIndex < 0)
    {
        return 1;
    }
    label = func_0010D650(argumentIndex + 1);
    if (label < 0)
    {
        return 1;
    }
    scrSetProgramCounter(scrGetLabelAddress(label));
    return 1;
}

s32 func_0010DCA8(void)
{
    func_0010D818(D_0040B7D8[func_0010D650(0)] < 0);
    return 1;
}

s32 func_0010DCE0(void)
{
    func_0010D818(D_0040B7D8[func_0010D650(0)] & 1);
    return 1;
}

s32 func_0010DD18(void)
{
    s32 p0;
    s32 p1;
    p0 = func_0010D650(0);
    p1 = func_0010D650(1);
    func_00103F58(p0, p1 & 0xFF, func_0010D650(2));
    return 1;
}

extern char D_00412668[];
s32 func_001063A8(f32 arg0);

s32 func_0010DD70(void)
{
    f32 fovy;

    fovy = bfWaitReadArgFloat(0) * 0.017453293f;
    if (fovy <= 5.0f || fovy >= 180.0f)
    {
        func_0010AE38(D_00412668, fovy);
        return 1;
    }
    func_001063A8(fovy);
    return 1;
}

typedef struct BfSectionTable {
    u8 unk0[8];
    s32 count; /* 0x8 */
} BfSectionTable;

typedef struct BfTaskRecord {
    u8 unk0[0x20];
    s32 basePriority; /* 0x20 */
} BfTaskRecord;

/* BF wait context: the shared ScrData plus the owning task record at +0xE4. */
typedef struct BfWaitContext {
    ScrData base;         /* 0x00 */
    BfTaskRecord *record; /* 0xE4 */
} BfWaitContext;

s32 func_0010D8C8(void);
s32 func_0010BEE0(s32 a0, void *a1, void *a2, void *a3, void *a4, void *a5, void *a6, void *a7, s32 a8);

s32 bfWaitCbCreateTask(void)
{
    s32 index;
    BfWaitContext *ctx;

    index = func_0010D650(0);
    ctx = (BfWaitContext *)func_0010D8C8();
    if (ctx == NULL)
    {
        return 1;
    }
    if (ctx->record == NULL)
    {
        return 1;
    }
    if (index < 0 || index >= ((BfSectionTable *)ctx->base.unkB0)->count)
    {
        return 1;
    }
    func_0010D818(func_0010BEE0(
        ctx->record->basePriority + func_0010D650(1), ctx->base.unkAC,
        ctx->base.unkB0, ctx->base.procedures, ctx->base.labels,
        ctx->base.instructions, ctx->base.unkC0, ctx->base.strings, index));
    return 1;
}

s32 func_0010DEA8(void)
{
    s32 p0;
    p0 = func_0010D650(0);
    if (kwlnTaskIsRegistered(p0) == 0)
    {
        return 1;
    }
    kwlnTaskDestroyWithHierarchy(p0, 1);
    return 1;
}

s32 func_0010DEF0(void)
{
    return kwlnTaskIsRegistered(func_0010D650(0)) == 0;
}

s32 func_0010DF18(void)
{
    if (kwlnTaskIsRegistered(func_0010D650(0)) != 0)
    {
        func_0010D818(1);
    }
    else
    {
        func_0010D818(0);
    }
    return 1;
}

/* Persona 4 scrCommand_SCR_GET_TIMER @ 00299660 (src/Script/scrCommonCommand.c), recompiled unchanged */
u32 scrCommand_SCR_GET_TIMER()
{
    KwlnTask* task;
    task = (KwlnTask*)func_0010D650(0);
    if (!kwlnTaskIsRegistered(task))
    {
        func_0010D818(0);
    }
    else
    {
        func_0010D818(kwlnTaskGetTimer(task));
    }
    return 1;
}

s32 func_0010DFC0(void)
{
    s32 p0;
    p0 = func_0010D650(0);
    kwlnFadeSetupFrames(p0, func_0010D650(1));
    return 1;
}

s32 func_0010E000(void)
{
    func_001057A8();
    return 1;
}

s32 func_0010E020(void)
{
    ScrVec4 v;
    f32 x;
    f32 y;
    f32 z;
    x = bfWaitReadArgFloat(1);
    __asm__ volatile ("qmtc2 %0, $vf02\n\tvaddx.x $vf10, $vf00, $vf02x" :: "r"(x));
    y = bfWaitReadArgFloat(2);
    __asm__ volatile ("qmtc2 %0, $vf02\n\tvaddx.y $vf10, $vf00, $vf02x" :: "r"(y));
    z = bfWaitReadArgFloat(3);
    __asm__ volatile ("qmtc2 %0, $vf02\n\tvaddx.z $vf10, $vf00, $vf02x" :: "r"(z));
    __asm__ volatile ("vmulx.w $vf10, $vf10, $vf00x\n\tsqc2 $vf10, %0" : "=m"(v));
    func_00107EF8(func_0010D650(0), 0, &v);
    return 1;
}

INCLUDE_RODATA(const s32, "script/scrCommonCommand", D_00412648);

INCLUDE_RODATA(const s32, "script/scrCommonCommand", D_00412658);

INCLUDE_RODATA(const s32, "script/scrCommonCommand", D_00412668);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E098);

s32 func_0010E148(void)
{
    ScrVec4 v;
    f32 x;
    f32 y;
    f32 z;
    x = bfWaitReadArgFloat(1);
    __asm__ volatile ("qmtc2 %0, $vf02\n\tvaddx.x $vf10, $vf00, $vf02x" :: "r"(x));
    y = bfWaitReadArgFloat(2);
    __asm__ volatile ("qmtc2 %0, $vf02\n\tvaddx.y $vf10, $vf00, $vf02x" :: "r"(y));
    z = bfWaitReadArgFloat(3);
    __asm__ volatile ("qmtc2 %0, $vf02\n\tvaddx.z $vf10, $vf00, $vf02x" :: "r"(z));
    __asm__ volatile ("vmove.w $vf10, $vf00\n\tsqc2 $vf10, %0" : "=m"(v));
    func_00108138(func_0010D650(0), &v);
    return 1;
}

s32 func_0010E1B8(void)
{
    ScrVecW v;
    v.x = bfWaitReadArgFloat(1);
    v.y = bfWaitReadArgFloat(2);
    v.z = bfWaitReadArgFloat(3);
    v.w = 0;
    func_001081F8(func_0010D650(0), &v);
    return 1;
}

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E210);

s32 func_0010E348(void)
{
    s32 p0;
    p0 = func_0010D650(0);
    evtUnk89F8SetState(p0, bfWaitReadArgFloat(1), bfWaitReadArgFloat(2));
    return 1;
}

s32 func_0010E3A0(void)
{
    s32 p0;
    s32 p1;
    p0 = func_0010D650(0);
    p1 = func_0010D650(1);
    kwlnDrawSetOffsetTransition(p0, p1, func_0010D650(2));
    return 1;
}

s32 func_0010E3F8(void)
{
    s32 p2;
    s32 mode;
    f32 first;
    f32 second;

    p2 = func_0010D650(2);
    switch (p2)
    {
    case 1:
        mode = 0x48;
        break;
    case 2:
        mode = 0x42;
        break;
    case 0:
    default:
        mode = 0x44;
        break;
    }
    first = bfWaitReadArgFloat(0);
    second = bfWaitReadArgFloat(1);
    kwlnDrawSetC70FloatTriple(first, second, mode);
    return 1;
}

s32 func_0010E480(void)
{
    /* Declared out of order: gcc 2.96 fills $16-$18 in declaration order. */
    s32 p2;
    s32 p0;
    s32 p3;
    s32 p1;
    p0 = func_0010D650(0);
    p3 = func_0010D650(3);
    p2 = func_0010D650(2);
    p1 = func_0010D650(1);
    kwlnDrawSetC70Second(((p0 & 0xFF) | (p3 << 24)) | (((p2 & 0xFF) << 16) | ((p1 & 0xFF) << 8)));
    return 1;
}

s32 func_0010E508(void)
{
    s32 p0;
    s32 p1;
    p0 = func_0010D650(0);
    p1 = func_0010D650(1);
    kwlnDrawSetC70Triple(p0, p1, func_0010D650(2));
    return 1;
}

s32 func_0010E560(void)
{
    func_00106AE8(func_0010D650(0));
    return 1;
}

s32 func_0010E588(void)
{
    kwlnDrawSetupC70(func_0010D650(0));
    return 1;
}

s32 func_0010E5B0(void)
{
    kwlnDrawSetupC70B(func_0010D650(0));
    return 1;
}

s32 func_0010E5D8(void)
{
    s32 p2;
    s32 p1;
    s32 p0;
    s32 sel;
    s32 kind;
    f32 f4;
    f32 f3;
    f32 f5;

    sel = func_0010D650(6);
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
    p0 = func_0010D650(0);
    p1 = func_0010D650(1);
    p2 = func_0010D650(2);
    f3 = bfWaitReadArgFloat(3);
    f4 = bfWaitReadArgFloat(4);
    f5 = bfWaitReadArgFloat(5);
    kwlnDrawSetCd0Clamped(p0, p1, p2, f3, f4, f5, kind);
    return 1;
}

s32 func_0010E6C0(void)
{
    /* Declared out of order: gcc 2.96 fills $16-$18 in declaration order. */
    s32 p2;
    s32 p0;
    s32 p3;
    s32 p1;
    p0 = func_0010D650(0);
    p3 = func_0010D650(3);
    p2 = func_0010D650(2);
    p1 = func_0010D650(1);
    kwlnDrawSetCd0Fourth(((p0 & 0xFF) | (p3 << 24)) | (((p2 & 0xFF) << 16) | ((p1 & 0xFF) << 8)));
    return 1;
}

s32 func_0010E748(void)
{
    s32 p0;
    s32 p1;
    p0 = func_0010D650(0);
    p1 = func_0010D650(1);
    kwlnDrawSetCd0Triple(p0, p1, func_0010D650(2));
    return 1;
}

s32 func_0010E7A0(void)
{
    func_00106D10(func_0010D650(0));
    return 1;
}

s32 func_0010E7C8(void)
{
    kwlnDrawSetupCd0(func_0010D650(0));
    return 1;
}

s32 func_0010E7F0(void)
{
    kwlnDrawEnableCd0(func_0010D650(0));
    return 1;
}

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E818);

s32 func_0010E900(void)
{
    /* Declared out of order: gcc 2.96 fills $16-$18 in declaration order. */
    s32 p2;
    s32 p0;
    s32 p3;
    s32 p1;
    p0 = func_0010D650(0);
    p3 = func_0010D650(3);
    p2 = func_0010D650(2);
    p1 = func_0010D650(1);
    kwlnDrawSetD30Fourth(((p0 & 0xFF) | (p3 << 24)) | (((p2 & 0xFF) << 16) | ((p1 & 0xFF) << 8)));
    return 1;
}

s32 func_0010E988(void)
{
    s32 p0;
    s32 p1;
    p0 = func_0010D650(0);
    p1 = func_0010D650(1);
    kwlnDrawSetD30Triple(p0, p1, func_0010D650(2));
    return 1;
}

s32 func_0010E9E0(void)
{
    func_00106F38(func_0010D650(0));
    return 1;
}

s32 func_0010EA08(void)
{
    kwlnDrawSetupD30(func_0010D650(0));
    return 1;
}

s32 func_0010EA30(void)
{
    kwlnDrawEnableD30(func_0010D650(0));
    return 1;
}

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010EA58);

s32 func_0010EAF8(void)
{
    /* Declared out of order: gcc 2.96 fills $16-$18 in declaration order. */
    s32 p2;
    s32 p0;
    s32 p3;
    s32 p1;
    p0 = func_0010D650(0);
    p3 = func_0010D650(3);
    p2 = func_0010D650(2);
    p1 = func_0010D650(1);
    kwlnDrawSetD88First(((p0 & 0xFF) | (p3 << 24)) | (((p2 & 0xFF) << 16) | ((p1 & 0xFF) << 8)));
    return 1;
}

s32 func_0010EB80(void)
{
    s32 p0;
    p0 = func_0010D650(0);
    kwlnDrawSetD88Pair(p0, func_0010D650(1));
    return 1;
}

s32 func_0010EBC0(void)
{
    func_001068C8(func_0010D650(0));
    return 1;
}

s32 func_0010EBE8(void)
{
    kwlnDrawSetupD88(func_0010D650(0));
    return 1;
}

s32 func_0010EC10(void)
{
    kwlnDrawEnableD88(func_0010D650(0));
    return 1;
}

s32 func_0010EC38(void)
{
    s32 p0;
    s32 mode;
    p0 = func_0010D650(0);
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
        func_0010AE38(D_004126D0);
        mode = 0x44;
        break;
    }
    kwlnDrawSetDc8Second(mode);
    return 1;
}

s32 func_0010ECB8(void)
{
    /* Declared out of order: gcc 2.96 fills $16-$18 in declaration order. */
    s32 p2;
    s32 p0;
    s32 p3;
    s32 p1;
    p0 = func_0010D650(0);
    p3 = func_0010D650(3);
    p2 = func_0010D650(2);
    p1 = func_0010D650(1);
    kwlnDrawSetDc8First(((p0 & 0xFF) | (p3 << 24)) | (((p2 & 0xFF) << 16) | ((p1 & 0xFF) << 8)));
    return 1;
}

s32 func_0010ED40(void)
{
    func_00106460(func_0010D650(0));
    return 1;
}

s32 func_0010ED68(void)
{
    kwlnDrawSetupDc8(func_0010D650(0));
    return 1;
}

s32 func_0010ED90(void)
{
    kwlnDrawEnableDc8(func_0010D650(0));
    return 1;
}

s32 func_0010EDB8(void)
{
    s32 p0;
    s32 mode;
    p0 = func_0010D650(0);
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
        func_0010AE38(D_004126F0);
        mode = 0x44;
        break;
    }
    kwlnDrawSetE08Fifth(mode);
    return 1;
}

s32 func_0010EE38(void)
{
    /* Declared out of order: gcc 2.96 fills $16-$18 in declaration order. */
    s32 p2;
    s32 p0;
    s32 p3;
    s32 p1;
    p0 = func_0010D650(0);
    p3 = func_0010D650(3);
    p2 = func_0010D650(2);
    p1 = func_0010D650(1);
    kwlnDrawSetE08Fourth(((p0 & 0xFF) | (p3 << 24)) | (((p2 & 0xFF) << 16) | ((p1 & 0xFF) << 8)));
    return 1;
}

s32 func_0010EEC0(void)
{
    s32 p0;
    s32 p1;
    p0 = func_0010D650(0);
    p1 = func_0010D650(1);
    kwlnDrawSetE08Triple(p0, p1, func_0010D650(2));
    return 1;
}

s32 func_0010EF18(void)
{
    func_00106658(func_0010D650(0));
    return 1;
}

s32 func_0010EF40(void)
{
    kwlnDrawSetupE08(func_0010D650(0));
    return 1;
}

s32 func_0010EF68(void)
{
    kwlnDrawEnableE08(func_0010D650(0));
    return 1;
}

s32 func_0010EF90(void)
{
    kwlnDrawSetOffsetTransition(0, 0, 0);
    kwlnDrawEnableD88(0);
    kwlnDrawSetupC70B(0);
    kwlnDrawEnableCd0(0);
    kwlnDrawEnableD30(0);
    func_00197298();
    return 1;
}

s32 func_0010EFE0(void)
{
    kwlnDrawEnableDc8(0);
    kwlnDrawEnableE08(0);
    func_00135568(0);
    func_00135578(0x80);
    func_00135588(0);
    fldSetFadeTarget(0, 1, 0);
    return 1;
}

s32 func_0010F030(void)
{
    D_00435DD0->unk388 = 0;
    return 1;
}

s32 func_0010F040(void)
{
    D_00435DD0->unk388 = 1;
    return 1;
}

s32 func_0010F058(void)
{
    return D_00435DD0->unk388 == 0;
}

s32 func_0010F068(void)
{
    s32 p0;
    p0 = func_0010D650(0);
    func_0011A118(p0, func_0010D650(1));
    return 1;
}

s32 func_0010F0A8(void)
{
    func_0011A0D0(func_0010D650(0));
    return 1;
}

/* Persona 4 scrCommand_SCR_EXISTS @ 00299600 (src/Script/scrCommonCommand.c), recompiled unchanged */
u32 scrCommand_SCR_EXISTS()
{
    KwlnTask* task;
    task = (KwlnTask*)func_0010D650(0);
    if (func_0011A100(task))
    {
        func_0010D818(1);
    }
    else
    {
        func_0010D818(0);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "script/scrCommonCommand", D_004126D0);

INCLUDE_RODATA(const s32, "script/scrCommonCommand", D_004126F0);

