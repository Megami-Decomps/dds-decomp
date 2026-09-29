#include "common.h"
#include "scr.h"
s32 kwlnDrawSetCd0Clamped(s32 arg0, s32 arg1, s32 arg2, f32 farg0, f32 farg1, f32 farg2, s32 arg3);
s32 kwlnDrawSetD30Clamped(s32 arg0, f32 farg0, f32 farg1, f32 farg2, f32 farg3, f32 farg4, s32 arg1);

typedef struct KwlnTask KwlnTask;
f32 bfWaitReadArgFloat(s32 idx);
s32 evtUnk89F8SetState(s32 arg0, f32 arg1, f32 arg2);
s32 func_001082D8(s32 arg0, void *arg1);
/* Declared floats-first: gcc 2.96 emits the outgoing register moves in
 * parameter order and schedules the last one into the jal delay slot, so
 * retail moves the kind argument ($16) last, in the delay slot. */
s32 kwlnDrawSetC70FloatTriple(f32 arg0, f32 arg1, s32 arg2);
s32 func_00107FD8(s32 arg0, s32 arg1, void *arg2);
s32 func_00108218(s32 arg0, void *arg1);
s32 func_001080D8(s32 arg0, s32 arg1, void *arg2);
s32 kwlnDrawSetD88FloatTriple(s32 arg0, f32 arg1, f32 arg2);
s32 kwlnDrawSetDc8Second(s32 arg0);
s32 kwlnDrawSetE08Fifth(s32 arg0);
s32 kwlnFadeOutStart(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 kwlnFadeInStart(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 func_00106488(f32 arg0);
s32 evtUnk8360SetVec(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4);
s32 fptodp(void);
extern ScrComGlobals *D_003BAA00;
extern s8 D_00398628[];
extern char D_0039F4D8[];
extern char D_0039F4E8[];
extern char D_0039F508[];
extern char D_0039F530[];
extern char D_0039F550[];
extern char D_0039F570[];

s32 func_0010D768(void)
{
    func_0010D5F0(effMiscRandMod(0, scrReadIntParameter(0)) + 1);
    return 1;
}

s32 func_0010D7A0(void)
{
    return scrGetCommandTimer() != 0;
}

/* Succeed when the command timer reaches the requested tick count; a
 * nonpositive request completes immediately. */
s32 func_0010D7C0(void) {
    if (scrReadIntParameter(0) <= 0) {
        return 1;
    }
    if (scrGetCommandTimer() < scrReadIntParameter(0)) {
        return 0;
    }
    return 1;
}

s32 func_0010D808(void)
{
    func_0010AC10("PUT -> %d\n", scrReadIntParameter(0));
    return 1;
}

s32 func_0010D838(void)
{
    func_0010AC10(D_0039F4D8, scrReadStringParameter(0));
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

s32 func_0010D9A8(void)
{
    if (scrGetCommandTimer() == 0)
    {
        func_001060C8(scrReadIntParameter(0));
        return 0;
    }
    return 1;
}

s32 func_0010D9E8(void)
{
    if (scrGetCommandTimer() == 0)
    {
        func_00106160(scrReadIntParameter(0));
        return 0;
    }
    return 1;
}

s32 func_0010DA28(void)
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

s32 func_0010DA80(void)
{
    func_0010D5F0(D_00398628[scrReadIntParameter(0)] < 0);
    return 1;
}

s32 func_0010DAB8(void)
{
    func_0010D5F0(D_00398628[scrReadIntParameter(0)] & 1);
    return 1;
}

s32 func_0010DAF0(void)
{
    s32 p0;
    s32 p1;
    p0 = scrReadIntParameter(0);
    p1 = scrReadIntParameter(1);
    func_00104068(p0, p1 & 0xFF, scrReadIntParameter(2));
    return 1;
}

s32 func_0010DB48(void)
{
    f32 fovy;

    fovy = bfWaitReadArgFloat(0) * 0.017453293f;
    if (fovy <= 5.0f || fovy >= 180.0f)
    {
        func_0010AC10(D_0039F4E8, fovy);
        return 1;
    }
    func_00106488(fovy);
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

s32 func_0010D6A0(void);
s32 func_0010BCB8(s32 a0, void *a1, void *a2, void *a3, void *a4, void *a5, void *a6, void *a7, s32 a8);

s32 bfWaitCbCreateTask(void)
{
    s32 index;
    BfWaitContext *ctx;

    index = scrReadIntParameter(0);
    ctx = (BfWaitContext *)func_0010D6A0();
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
    func_0010D5F0(func_0010BCB8(
        ctx->record->basePriority + scrReadIntParameter(1), ctx->base.unkAC,
        ctx->base.unkB0, ctx->base.procedures, ctx->base.labels,
        ctx->base.instructions, ctx->base.unkC0, ctx->base.strings, index));
    return 1;
}

s32 func_0010DC80(void)
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

s32 func_0010DCC8(void)
{
    return kwlnTaskIsRegistered(scrReadIntParameter(0)) == 0;
}

s32 func_0010DCF0(void)
{
    if (kwlnTaskIsRegistered(scrReadIntParameter(0)) != 0)
    {
        func_0010D5F0(1);
    }
    else
    {
        func_0010D5F0(0);
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
        func_0010D5F0(0);
    }
    else
    {
        func_0010D5F0(kwlnTaskGetTimer(task));
    }
    return 1;
}

s32 func_0010DD98(void)
{
    s32 p0;
    p0 = scrReadIntParameter(0);
    kwlnFadeSetupFrames(p0, scrReadIntParameter(1));
    return 1;
}

s32 func_0010DDD8(void)
{
    func_00105888();
    return 1;
}

s32 func_0010DDF8(void)
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
    func_00107FD8(scrReadIntParameter(0), 0, &v);
    return 1;
}

INCLUDE_RODATA(const s32, "script/scrCommonCommand", D_0039F4D8);

INCLUDE_RODATA(const s32, "script/scrCommonCommand", D_0039F4E8);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010DE70);

s32 func_0010DF20(void)
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
    func_00108218(scrReadIntParameter(0), &v);
    return 1;
}

s32 func_0010DF90(void)
{
    ScrVecW v;
    v.x = bfWaitReadArgFloat(1);
    v.y = bfWaitReadArgFloat(2);
    v.z = bfWaitReadArgFloat(3);
    v.w = 0;
    func_001082D8(scrReadIntParameter(0), &v);
    return 1;
}

s32 func_0010DFE8(void)
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
    evtUnk8360SetVec(p0, x, y, z, w);
    return 1;
}

s32 func_0010E120(void)
{
    s32 p0;
    p0 = scrReadIntParameter(0);
    evtUnk89F8SetState(p0, bfWaitReadArgFloat(1), bfWaitReadArgFloat(2));
    return 1;
}

s32 func_0010E178(void)
{
    s32 p0;
    s32 p1;
    p0 = scrReadIntParameter(0);
    p1 = scrReadIntParameter(1);
    kwlnDrawSetOffsetTransition(p0, p1, scrReadIntParameter(2));
    return 1;
}

s32 func_0010E1D0(void)
{
    s32 p2;
    s32 mode;
    f32 first;
    f32 second;

    p2 = scrReadIntParameter(2);
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

s32 func_0010E258(void)
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

s32 func_0010E2E0(void)
{
    s32 p0;
    s32 p1;
    p0 = scrReadIntParameter(0);
    p1 = scrReadIntParameter(1);
    kwlnDrawSetC70Triple(p0, p1, scrReadIntParameter(2));
    return 1;
}

s32 func_0010E338(void)
{
    func_00106BC8(scrReadIntParameter(0));
    return 1;
}

s32 func_0010E360(void)
{
    kwlnDrawSetupC70(scrReadIntParameter(0));
    return 1;
}

s32 func_0010E388(void)
{
    kwlnDrawSetupC70B(scrReadIntParameter(0));
    return 1;
}

s32 func_0010E3B0(void)
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

s32 func_0010E498(void)
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

s32 func_0010E520(void)
{
    s32 p0;
    s32 p1;
    p0 = scrReadIntParameter(0);
    p1 = scrReadIntParameter(1);
    kwlnDrawSetCd0Triple(p0, p1, scrReadIntParameter(2));
    return 1;
}

s32 func_0010E578(void)
{
    func_00106DF0(scrReadIntParameter(0));
    return 1;
}

s32 func_0010E5A0(void)
{
    kwlnDrawSetupCd0(scrReadIntParameter(0));
    return 1;
}

s32 func_0010E5C8(void)
{
    kwlnDrawEnableCd0(scrReadIntParameter(0));
    return 1;
}

s32 func_0010E5F0(void)
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

s32 func_0010E6D8(void)
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

s32 func_0010E760(void)
{
    s32 p0;
    s32 p1;
    p0 = scrReadIntParameter(0);
    p1 = scrReadIntParameter(1);
    kwlnDrawSetD30Triple(p0, p1, scrReadIntParameter(2));
    return 1;
}

s32 func_0010E7B8(void)
{
    func_00107018(scrReadIntParameter(0));
    return 1;
}

s32 func_0010E7E0(void)
{
    kwlnDrawSetupD30(scrReadIntParameter(0));
    return 1;
}

s32 func_0010E808(void)
{
    kwlnDrawEnableD30(scrReadIntParameter(0));
    return 1;
}

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E830);

s32 func_0010E8D0(void)
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

s32 func_0010E958(void)
{
    s32 p0;
    p0 = scrReadIntParameter(0);
    kwlnDrawSetD88Pair(p0, scrReadIntParameter(1));
    return 1;
}

s32 func_0010E998(void)
{
    func_001069A8(scrReadIntParameter(0));
    return 1;
}

s32 func_0010E9C0(void)
{
    kwlnDrawSetupD88(scrReadIntParameter(0));
    return 1;
}

s32 func_0010E9E8(void)
{
    kwlnDrawEnableD88(scrReadIntParameter(0));
    return 1;
}

s32 func_0010EA10(void)
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
        func_0010AC10(D_0039F550);
        mode = 0x44;
        break;
    }
    kwlnDrawSetDc8Second(mode);
    return 1;
}

s32 func_0010EA90(void)
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

s32 func_0010EB18(void)
{
    func_00106540(scrReadIntParameter(0));
    return 1;
}

s32 func_0010EB40(void)
{
    kwlnDrawSetupDc8(scrReadIntParameter(0));
    return 1;
}

s32 func_0010EB68(void)
{
    kwlnDrawEnableDc8(scrReadIntParameter(0));
    return 1;
}

s32 func_0010EB90(void)
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
        func_0010AC10(D_0039F570);
        mode = 0x44;
        break;
    }
    kwlnDrawSetE08Fifth(mode);
    return 1;
}

s32 func_0010EC10(void)
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

s32 func_0010EC98(void)
{
    s32 p0;
    s32 p1;
    p0 = scrReadIntParameter(0);
    p1 = scrReadIntParameter(1);
    kwlnDrawSetE08Triple(p0, p1, scrReadIntParameter(2));
    return 1;
}

s32 func_0010ECF0(void)
{
    func_00106738(scrReadIntParameter(0));
    return 1;
}

s32 func_0010ED18(void)
{
    kwlnDrawSetupE08(scrReadIntParameter(0));
    return 1;
}

s32 func_0010ED40(void)
{
    kwlnDrawEnableE08(scrReadIntParameter(0));
    return 1;
}

s32 func_0010ED68(void)
{
    kwlnDrawSetOffsetTransition(0, 0, 0);
    kwlnDrawEnableD88(0);
    kwlnDrawSetupC70B(0);
    kwlnDrawEnableCd0(0);
    kwlnDrawEnableD30(0);
    func_0018F660();
    return 1;
}

s32 func_0010EDB8(void)
{
    kwlnDrawEnableDc8(0);
    kwlnDrawEnableE08(0);
    fldSetSwayMode(0);
    func_00132B70(0x80);
    func_00132B80(0);
    fldSetFadeTarget(0, 1, 0);
    return 1;
}

s32 func_0010EE08(void)
{
    D_003BAA00->unk388 = 0;
    return 1;
}

s32 func_0010EE18(void)
{
    D_003BAA00->unk388 = 1;
    return 1;
}

s32 func_0010EE30(void)
{
    return D_003BAA00->unk388 == 0;
}

s32 func_0010EE40(void)
{
    s32 p0;
    p0 = scrReadIntParameter(0);
    func_00119900(p0, scrReadIntParameter(1));
    return 1;
}

s32 func_0010EE80(void)
{
    func_001198B8(scrReadIntParameter(0));
    return 1;
}

/* Persona 4 scrCommand_SCR_EXISTS @ 00299600 (src/Script/scrCommonCommand.c), recompiled unchanged */
u32 scrCommand_SCR_EXISTS()
{
    KwlnTask* task;
    task = (KwlnTask*)scrReadIntParameter(0);
    if (func_001198E8(task))
    {
        func_0010D5F0(1);
    }
    else
    {
        func_0010D5F0(0);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "script/scrCommonCommand", D_0039F550);

INCLUDE_RODATA(const s32, "script/scrCommonCommand", D_0039F570);

