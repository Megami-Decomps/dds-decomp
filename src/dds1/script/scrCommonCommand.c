#include "common.h"
#include "scr.h"

typedef struct KwlnTask KwlnTask;
f32 func_0010D4F0(s32 idx);
s32 evtUnk89F8SetState(s32 arg0, f32 arg1, f32 arg2);
s32 func_001082D8(s32 arg0, void *arg1);
s32 kwlnDrawSetC70FloatTriple(s32 arg0, f32 arg1, f32 arg2);
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
    func_0010D5F0(effMiscRandMod(0, func_0010D428(0)) + 1);
    return 1;
}

s32 func_0010D7A0(void)
{
    return scrGetCommandTimer() != 0;
}

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010D7C0);

s32 func_0010D808(void)
{
    func_0010AC10("PUT -> %d\n", func_0010D428(0));
    return 1;
}

s32 func_0010D838(void)
{
    func_0010AC10(D_0039F4D8, func_0010D5A8(0));
    return 1;
}

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010D868);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010D908);

s32 func_0010D9A8(void)
{
    if (scrGetCommandTimer() == 0)
    {
        func_001060C8(func_0010D428(0));
        return 0;
    }
    return 1;
}

s32 func_0010D9E8(void)
{
    if (scrGetCommandTimer() == 0)
    {
        func_00106160(func_0010D428(0));
        return 0;
    }
    return 1;
}

s32 func_0010DA28(void)
{
    s32 argumentIndex;
    s32 label;
    argumentIndex = func_0010D428(0);
    if (argumentIndex < 0)
    {
        return 1;
    }
    label = func_0010D428(argumentIndex + 1);
    if (label < 0)
    {
        return 1;
    }
    scrSetProgramCounter(scrGetLabelAddress(label));
    return 1;
}

s32 func_0010DA80(void)
{
    func_0010D5F0(D_00398628[func_0010D428(0)] < 0);
    return 1;
}

s32 func_0010DAB8(void)
{
    func_0010D5F0(D_00398628[func_0010D428(0)] & 1);
    return 1;
}

s32 func_0010DAF0(void)
{
    s32 p0;
    s32 p1;
    p0 = func_0010D428(0);
    p1 = func_0010D428(1);
    func_00104068(p0, p1 & 0xFF, func_0010D428(2));
    return 1;
}

INCLUDE_RODATA(const s32, "script/scrCommonCommand", D_0039F4D8);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010DB48);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010DBD0);

s32 func_0010DC80(void)
{
    s32 p0;
    p0 = func_0010D428(0);
    if (kwlnTaskIsRegistered(p0) == 0)
    {
        return 1;
    }
    kwlnTaskDestroyWithHierarchy(p0, 1);
    return 1;
}

s32 func_0010DCC8(void)
{
    return kwlnTaskIsRegistered(func_0010D428(0)) == 0;
}

s32 func_0010DCF0(void)
{
    if (kwlnTaskIsRegistered(func_0010D428(0)) != 0)
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
    task = (KwlnTask*)func_0010D428(0);
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
    p0 = func_0010D428(0);
    kwlnFadeSetupFrames(p0, func_0010D428(1));
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
    x = func_0010D4F0(1);
    __asm__ volatile ("qmtc2 %0, $vf02\n\tvaddx.x $vf10, $vf00, $vf02x" :: "r"(x));
    y = func_0010D4F0(2);
    __asm__ volatile ("qmtc2 %0, $vf02\n\tvaddx.y $vf10, $vf00, $vf02x" :: "r"(y));
    z = func_0010D4F0(3);
    __asm__ volatile ("qmtc2 %0, $vf02\n\tvaddx.z $vf10, $vf00, $vf02x" :: "r"(z));
    __asm__ volatile ("vmulx.w $vf10, $vf10, $vf00x\n\tsqc2 $vf10, %0" : "=m"(v));
    func_00107FD8(func_0010D428(0), 0, &v);
    return 1;
}

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010DE70);

s32 func_0010DF20(void)
{
    ScrVec4 v;
    f32 x;
    f32 y;
    f32 z;
    x = func_0010D4F0(1);
    __asm__ volatile ("qmtc2 %0, $vf02\n\tvaddx.x $vf10, $vf00, $vf02x" :: "r"(x));
    y = func_0010D4F0(2);
    __asm__ volatile ("qmtc2 %0, $vf02\n\tvaddx.y $vf10, $vf00, $vf02x" :: "r"(y));
    z = func_0010D4F0(3);
    __asm__ volatile ("qmtc2 %0, $vf02\n\tvaddx.z $vf10, $vf00, $vf02x" :: "r"(z));
    __asm__ volatile ("vmove.w $vf10, $vf00\n\tsqc2 $vf10, %0" : "=m"(v));
    func_00108218(func_0010D428(0), &v);
    return 1;
}

s32 func_0010DF90(void)
{
    ScrVecW v;
    v.x = func_0010D4F0(1);
    v.y = func_0010D4F0(2);
    v.z = func_0010D4F0(3);
    v.w = 0;
    func_001082D8(func_0010D428(0), &v);
    return 1;
}

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010DFE8);

s32 func_0010E120(void)
{
    s32 p0;
    p0 = func_0010D428(0);
    evtUnk89F8SetState(p0, func_0010D4F0(1), func_0010D4F0(2));
    return 1;
}

s32 func_0010E178(void)
{
    s32 p0;
    s32 p1;
    p0 = func_0010D428(0);
    p1 = func_0010D428(1);
    kwlnDrawSetOffsetTransition(p0, p1, func_0010D428(2));
    return 1;
}

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E1D0);

s32 func_0010E258(void)
{
    /* Declared out of order: gcc 2.96 fills $16-$18 in declaration order. */
    s32 byte2;
    s32 byte0;
    s32 byte3;
    s32 byte1;
    byte0 = func_0010D428(0);
    byte3 = func_0010D428(3);
    byte2 = func_0010D428(2);
    byte1 = func_0010D428(1);
    kwlnDrawSetC70Second(((byte0 & 0xFF) | (byte3 << 24)) | (((byte2 & 0xFF) << 16) | ((byte1 & 0xFF) << 8)));
    return 1;
}

s32 func_0010E2E0(void)
{
    s32 p0;
    s32 p1;
    p0 = func_0010D428(0);
    p1 = func_0010D428(1);
    kwlnDrawSetC70Triple(p0, p1, func_0010D428(2));
    return 1;
}

s32 func_0010E338(void)
{
    func_00106BC8(func_0010D428(0));
    return 1;
}

s32 func_0010E360(void)
{
    kwlnDrawSetupC70(func_0010D428(0));
    return 1;
}

s32 func_0010E388(void)
{
    kwlnDrawSetupC70B(func_0010D428(0));
    return 1;
}

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E3B0);

s32 func_0010E498(void)
{
    /* Declared out of order: gcc 2.96 fills $16-$18 in declaration order. */
    s32 byte2;
    s32 byte0;
    s32 byte3;
    s32 byte1;
    byte0 = func_0010D428(0);
    byte3 = func_0010D428(3);
    byte2 = func_0010D428(2);
    byte1 = func_0010D428(1);
    kwlnDrawSetCd0Fourth(((byte0 & 0xFF) | (byte3 << 24)) | (((byte2 & 0xFF) << 16) | ((byte1 & 0xFF) << 8)));
    return 1;
}

s32 func_0010E520(void)
{
    s32 p0;
    s32 p1;
    p0 = func_0010D428(0);
    p1 = func_0010D428(1);
    kwlnDrawSetCd0Triple(p0, p1, func_0010D428(2));
    return 1;
}

s32 func_0010E578(void)
{
    func_00106DF0(func_0010D428(0));
    return 1;
}

s32 func_0010E5A0(void)
{
    kwlnDrawSetupCd0(func_0010D428(0));
    return 1;
}

s32 func_0010E5C8(void)
{
    kwlnDrawEnableCd0(func_0010D428(0));
    return 1;
}

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E5F0);

s32 func_0010E6D8(void)
{
    /* Declared out of order: gcc 2.96 fills $16-$18 in declaration order. */
    s32 byte2;
    s32 byte0;
    s32 byte3;
    s32 byte1;
    byte0 = func_0010D428(0);
    byte3 = func_0010D428(3);
    byte2 = func_0010D428(2);
    byte1 = func_0010D428(1);
    kwlnDrawSetD30Fourth(((byte0 & 0xFF) | (byte3 << 24)) | (((byte2 & 0xFF) << 16) | ((byte1 & 0xFF) << 8)));
    return 1;
}

s32 func_0010E760(void)
{
    s32 p0;
    s32 p1;
    p0 = func_0010D428(0);
    p1 = func_0010D428(1);
    kwlnDrawSetD30Triple(p0, p1, func_0010D428(2));
    return 1;
}

s32 func_0010E7B8(void)
{
    func_00107018(func_0010D428(0));
    return 1;
}

s32 func_0010E7E0(void)
{
    kwlnDrawSetupD30(func_0010D428(0));
    return 1;
}

s32 func_0010E808(void)
{
    kwlnDrawEnableD30(func_0010D428(0));
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
    byte0 = func_0010D428(0);
    byte3 = func_0010D428(3);
    byte2 = func_0010D428(2);
    byte1 = func_0010D428(1);
    kwlnDrawSetD88First(((byte0 & 0xFF) | (byte3 << 24)) | (((byte2 & 0xFF) << 16) | ((byte1 & 0xFF) << 8)));
    return 1;
}

s32 func_0010E958(void)
{
    s32 p0;
    p0 = func_0010D428(0);
    kwlnDrawSetD88Pair(p0, func_0010D428(1));
    return 1;
}

s32 func_0010E998(void)
{
    func_001069A8(func_0010D428(0));
    return 1;
}

s32 func_0010E9C0(void)
{
    kwlnDrawSetupD88(func_0010D428(0));
    return 1;
}

s32 func_0010E9E8(void)
{
    kwlnDrawEnableD88(func_0010D428(0));
    return 1;
}

s32 func_0010EA10(void)
{
    s32 p0;
    s32 mode;
    p0 = func_0010D428(0);
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
    byte0 = func_0010D428(0);
    byte3 = func_0010D428(3);
    byte2 = func_0010D428(2);
    byte1 = func_0010D428(1);
    kwlnDrawSetDc8First(((byte0 & 0xFF) | (byte3 << 24)) | (((byte2 & 0xFF) << 16) | ((byte1 & 0xFF) << 8)));
    return 1;
}

s32 func_0010EB18(void)
{
    func_00106540(func_0010D428(0));
    return 1;
}

s32 func_0010EB40(void)
{
    kwlnDrawSetupDc8(func_0010D428(0));
    return 1;
}

s32 func_0010EB68(void)
{
    kwlnDrawEnableDc8(func_0010D428(0));
    return 1;
}

s32 func_0010EB90(void)
{
    s32 p0;
    s32 mode;
    p0 = func_0010D428(0);
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
    byte0 = func_0010D428(0);
    byte3 = func_0010D428(3);
    byte2 = func_0010D428(2);
    byte1 = func_0010D428(1);
    kwlnDrawSetE08Fourth(((byte0 & 0xFF) | (byte3 << 24)) | (((byte2 & 0xFF) << 16) | ((byte1 & 0xFF) << 8)));
    return 1;
}

s32 func_0010EC98(void)
{
    s32 p0;
    s32 p1;
    p0 = func_0010D428(0);
    p1 = func_0010D428(1);
    kwlnDrawSetE08Triple(p0, p1, func_0010D428(2));
    return 1;
}

s32 func_0010ECF0(void)
{
    func_00106738(func_0010D428(0));
    return 1;
}

s32 func_0010ED18(void)
{
    kwlnDrawSetupE08(func_0010D428(0));
    return 1;
}

s32 func_0010ED40(void)
{
    kwlnDrawEnableE08(func_0010D428(0));
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
    func_00132B60(0);
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
    p0 = func_0010D428(0);
    func_00119900(p0, func_0010D428(1));
    return 1;
}

s32 func_0010EE80(void)
{
    func_001198B8(func_0010D428(0));
    return 1;
}

/* Persona 4 scrCommand_SCR_EXISTS @ 00299600 (src/Script/scrCommonCommand.c), recompiled unchanged */
u32 scrCommand_SCR_EXISTS()
{
    KwlnTask* task;
    task = (KwlnTask*)func_0010D428(0);
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

