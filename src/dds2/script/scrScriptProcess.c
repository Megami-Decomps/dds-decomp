#include "common.h"

void *func_00343ED0(s32 arg0, u32 *arg1, s32 arg2);

s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

s32 func_0010C2F0();

s32 func_0010C298();

typedef struct { u8 pad0[0x40]; s32 integers[256]; s32 floatBits[256]; } ScrProcGlobals;

typedef struct {
    u8 pad0[0xB4];
    s32 unkB4;
    u8 padB8[0x10];
    s32 unkC8;
    u8 padCC[0xC];
    void *scriptHandle;
    u8 padDC[8];
    s32 taskId;
} ScrProcTask;

extern ScrProcGlobals *D_00435DD0;

s32 func_0010BD50(s32 scriptId, s32 option)
{
    u32 buf[4];
    void *handle;
    s32 id;
    ScrProcTask *task;
    handle = func_00343ED0(scriptId, buf, 0);
    id = buf[0];
    if (id == 0)
    {
        return 0;
    }
    task = (ScrProcTask *)func_0010BC40(id, option);
    if (task != NULL)
    {
        task->scriptHandle = handle;
    }
    return (s32)task;
}

s32 scrProcCreateTask(s32 priority, ScrProcTask *task)
{
    s32 id;
    id = kwlnTaskCreate(task->unkB4 + (task->unkC8 << 5), priority, 1, 1, func_0010C2F0, func_0010C298, (s32)task);
    task->taskId = id;
    return id;
}

void func_0010BE08(void)
{
    s32 i;
    /* Countdown with a forward index; gcc keeps a single pointer (see asm). */
    for (i = 255; i >= 0; i--)
    {
        D_00435DD0->integers[255 - i] = 0;
        D_00435DD0->floatBits[255 - i] = 0;
    }
}

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BE40);

void func_0010BE78(u32 arg0) {
    func_0010BD50(arg0, 0);
}

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BE90);

void func_0010BEC8(u32 arg0) {
    func_0010BC40(arg0, 0);
}

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BEE0);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BF30);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BF48);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BFE0);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010C058);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010C100);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010C158);

INCLUDE_RODATA(const s32, "script/scrScriptProcess", D_00411408);

