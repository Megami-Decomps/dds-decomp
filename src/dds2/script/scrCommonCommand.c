#include "common.h"

typedef struct KwlnTask KwlnTask;

extern char D_00412648[];

extern char D_00412658[];

extern s8 D_0040B7D8[];

f32 func_0010D718(s32 idx);

s32 func_00107EF8(s32 arg0, s32 arg1, void *arg2);

typedef struct { f32 x; f32 y; f32 z; f32 w; } ScrVec4;

s32 func_00108138(s32 arg0, void *arg1);

typedef struct { f32 x; f32 y; f32 z; s32 w; } ScrVecW;

s32 func_001081F8(s32 arg0, void *arg1);

s32 evtUnk89F8SetState(s32 arg0, f32 arg1, f32 arg2);

s32 drawSetDc8Second(s32 arg0);

extern char D_004126D0[];

s32 drawSetE08Fifth(s32 arg0);

extern char D_004126F0[];

typedef struct { u8 pad[0x388]; s32 unk388; } ScrComGlobals;

extern ScrComGlobals *D_00435DD0;

s32 func_0010D990(void)
{
    func_0010D818(effMiscRandMod(0, func_0010D650(0)) + 1);
    return 1;
}

s32 func_0010D9C8(void)
{
    return func_0010D8A8() != 0;
}

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010D9E8);

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

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010DA90);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010DB30);

s32 func_0010DBD0(void)
{
    if (func_0010D8A8() == 0)
    {
        func_00105FE8(func_0010D650(0));
        return 0;
    }
    return 1;
}

s32 func_0010DC10(void)
{
    if (func_0010D8A8() == 0)
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
    func_0010D888(func_0010D860(label));
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

INCLUDE_RODATA(const s32, "script/scrCommonCommand", D_00412648);

INCLUDE_RODATA(const s32, "script/scrCommonCommand", D_00412658);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010DD70);

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010DDF8);

s32 func_0010DEA8(void)
{
    s32 p0;
    p0 = func_0010D650(0);
    if (func_001018B0(p0) == 0)
    {
        return 1;
    }
    kwlnTaskDestroyWithHierarchy(p0, 1);
    return 1;
}

s32 func_0010DEF0(void)
{
    return func_001018B0(func_0010D650(0)) == 0;
}

s32 func_0010DF18(void)
{
    if (func_001018B0(func_0010D650(0)) != 0)
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
    if (!func_001018B0(task))
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
    x = func_0010D718(1);
    __asm__ volatile ("qmtc2 %0, $vf02\n\tvaddx.x $vf10, $vf00, $vf02x" :: "r"(x));
    y = func_0010D718(2);
    __asm__ volatile ("qmtc2 %0, $vf02\n\tvaddx.y $vf10, $vf00, $vf02x" :: "r"(y));
    z = func_0010D718(3);
    __asm__ volatile ("qmtc2 %0, $vf02\n\tvaddx.z $vf10, $vf00, $vf02x" :: "r"(z));
    __asm__ volatile ("vmulx.w $vf10, $vf10, $vf00x\n\tsqc2 $vf10, %0" : "=m"(v));
    func_00107EF8(func_0010D650(0), 0, &v);
    return 1;
}

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E098);

s32 func_0010E148(void)
{
    ScrVec4 v;
    f32 x;
    f32 y;
    f32 z;
    x = func_0010D718(1);
    __asm__ volatile ("qmtc2 %0, $vf02\n\tvaddx.x $vf10, $vf00, $vf02x" :: "r"(x));
    y = func_0010D718(2);
    __asm__ volatile ("qmtc2 %0, $vf02\n\tvaddx.y $vf10, $vf00, $vf02x" :: "r"(y));
    z = func_0010D718(3);
    __asm__ volatile ("qmtc2 %0, $vf02\n\tvaddx.z $vf10, $vf00, $vf02x" :: "r"(z));
    __asm__ volatile ("vmove.w $vf10, $vf00\n\tsqc2 $vf10, %0" : "=m"(v));
    func_00108138(func_0010D650(0), &v);
    return 1;
}

s32 func_0010E1B8(void)
{
    ScrVecW v;
    v.x = func_0010D718(1);
    v.y = func_0010D718(2);
    v.z = func_0010D718(3);
    v.w = 0;
    func_001081F8(func_0010D650(0), &v);
    return 1;
}

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E210);

s32 func_0010E348(void)
{
    s32 p0;
    p0 = func_0010D650(0);
    evtUnk89F8SetState(p0, func_0010D718(1), func_0010D718(2));
    return 1;
}

s32 func_0010E3A0(void)
{
    s32 p0;
    s32 p1;
    p0 = func_0010D650(0);
    p1 = func_0010D650(1);
    func_00106810(p0, p1, func_0010D650(2));
    return 1;
}

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E3F8);

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
    drawSetC70Second(((p0 & 0xFF) | (p3 << 24)) | (((p2 & 0xFF) << 16) | ((p1 & 0xFF) << 8)));
    return 1;
}

s32 func_0010E508(void)
{
    s32 p0;
    s32 p1;
    p0 = func_0010D650(0);
    p1 = func_0010D650(1);
    drawSetC70Triple(p0, p1, func_0010D650(2));
    return 1;
}

s32 func_0010E560(void)
{
    func_00106AE8(func_0010D650(0));
    return 1;
}

s32 func_0010E588(void)
{
    drawSetupC70(func_0010D650(0));
    return 1;
}

s32 func_0010E5B0(void)
{
    drawSetupC70B(func_0010D650(0));
    return 1;
}

INCLUDE_ASM(const s32, "script/scrCommonCommand", func_0010E5D8);

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
    drawSetCd0Fourth(((p0 & 0xFF) | (p3 << 24)) | (((p2 & 0xFF) << 16) | ((p1 & 0xFF) << 8)));
    return 1;
}

s32 func_0010E748(void)
{
    s32 p0;
    s32 p1;
    p0 = func_0010D650(0);
    p1 = func_0010D650(1);
    drawSetCd0Triple(p0, p1, func_0010D650(2));
    return 1;
}

s32 func_0010E7A0(void)
{
    func_00106D10(func_0010D650(0));
    return 1;
}

s32 func_0010E7C8(void)
{
    drawSetupCd0(func_0010D650(0));
    return 1;
}

s32 func_0010E7F0(void)
{
    drawEnableCd0(func_0010D650(0));
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
    drawSetD30Fourth(((p0 & 0xFF) | (p3 << 24)) | (((p2 & 0xFF) << 16) | ((p1 & 0xFF) << 8)));
    return 1;
}

s32 func_0010E988(void)
{
    s32 p0;
    s32 p1;
    p0 = func_0010D650(0);
    p1 = func_0010D650(1);
    drawSetD30Triple(p0, p1, func_0010D650(2));
    return 1;
}

s32 func_0010E9E0(void)
{
    func_00106F38(func_0010D650(0));
    return 1;
}

s32 func_0010EA08(void)
{
    drawSetupD30(func_0010D650(0));
    return 1;
}

s32 func_0010EA30(void)
{
    drawEnableD30(func_0010D650(0));
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
    drawSetD88First(((p0 & 0xFF) | (p3 << 24)) | (((p2 & 0xFF) << 16) | ((p1 & 0xFF) << 8)));
    return 1;
}

s32 func_0010EB80(void)
{
    s32 p0;
    p0 = func_0010D650(0);
    drawSetD88Pair(p0, func_0010D650(1));
    return 1;
}

s32 func_0010EBC0(void)
{
    func_001068C8(func_0010D650(0));
    return 1;
}

s32 func_0010EBE8(void)
{
    drawSetupD88(func_0010D650(0));
    return 1;
}

s32 func_0010EC10(void)
{
    drawEnableD88(func_0010D650(0));
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
    drawSetDc8Second(mode);
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
    drawSetDc8First(((p0 & 0xFF) | (p3 << 24)) | (((p2 & 0xFF) << 16) | ((p1 & 0xFF) << 8)));
    return 1;
}

s32 func_0010ED40(void)
{
    func_00106460(func_0010D650(0));
    return 1;
}

s32 func_0010ED68(void)
{
    drawSetupDc8(func_0010D650(0));
    return 1;
}

s32 func_0010ED90(void)
{
    drawEnableDc8(func_0010D650(0));
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
    drawSetE08Fifth(mode);
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
    drawSetE08Fourth(((p0 & 0xFF) | (p3 << 24)) | (((p2 & 0xFF) << 16) | ((p1 & 0xFF) << 8)));
    return 1;
}

s32 func_0010EEC0(void)
{
    s32 p0;
    s32 p1;
    p0 = func_0010D650(0);
    p1 = func_0010D650(1);
    drawSetE08Triple(p0, p1, func_0010D650(2));
    return 1;
}

s32 func_0010EF18(void)
{
    func_00106658(func_0010D650(0));
    return 1;
}

s32 func_0010EF40(void)
{
    drawSetupE08(func_0010D650(0));
    return 1;
}

s32 func_0010EF68(void)
{
    drawEnableE08(func_0010D650(0));
    return 1;
}

s32 func_0010EF90(void)
{
    func_00106810(0, 0, 0);
    drawEnableD88(0);
    drawSetupC70B(0);
    drawEnableCd0(0);
    drawEnableD30(0);
    func_00197298();
    return 1;
}

s32 func_0010EFE0(void)
{
    drawEnableDc8(0);
    drawEnableE08(0);
    func_00135568(0);
    func_00135578(0x80);
    func_00135588(0);
    func_001354F0(0, 1, 0);
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

