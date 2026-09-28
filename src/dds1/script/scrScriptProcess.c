#include "common.h"
#include "scr.h"
extern ScrProcGlobals *D_003BAA00;

void *func_002EB028(s32 arg0, u32 *arg1, s32 arg2);
s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
s32 func_0010C0C8();
s32 func_0010C070();
s32 func_0010B7C0(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7);
s32 func_0010BA18(s32 arg0, s32 arg1);
s32 func_0010BB28(s32 scriptId, s32 option)
{
    u32 buf[4];
    void *handle;
    s32 id;
    ScrProcTask *task;
    handle = func_002EB028(scriptId, buf, 0);
    id = buf[0];
    if (id == 0)
    {
        return 0;
    }
    task = (ScrProcTask *)func_0010BA18(id, option);
    if (task != NULL)
    {
        task->scriptHandle = handle;
    }
    return (s32)task;
}

s32 scrProcCreateTask(s32 priority, ScrProcTask *task)
{
    s32 id;
    id = kwlnTaskCreate(task->unkB4 + (task->unkC8 << 5), priority, 1, 1, func_0010C0C8, func_0010C070, (s32)task);
    task->taskId = id;
    return id;
}

void scrClearProcessGlobals(void)
{
    s32 i;
    /* Countdown with a forward index; gcc keeps a single pointer (see asm). */
    for (i = 255; i >= 0; i--)
    {
        D_003BAA00->integers[255 - i] = 0;
        D_003BAA00->floatBits[255 - i] = 0;
    }
}

void func_0010BC18(s32 priority, s32 scriptId, s32 option)
{
    scrProcCreateTask(priority, func_0010BB28(scriptId, option));
}

s32 scrCreateProcessWithDefaultOption(s32 scriptId)
{
    return func_0010BB28(scriptId, 0);
}

void func_0010BC68(s32 priority, s32 taskId, s32 option)
{
    scrProcCreateTask(priority, func_0010BA18(taskId, option));
}

s32 scrCreateTaskWithDefaultOption(s32 taskId)
{
    return func_0010BA18(taskId, 0);
}

s32 func_0010BCB8(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8)
{
    return scrProcCreateTask(a0, func_0010B7C0(a1, a2, a3, a4, a5, a6, a7, a8));
}

s32 func_0010BD08(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6)
{
    return func_0010B7C0(arg0, arg1, arg2, arg3, arg4, arg5, arg6, 0);
}

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BD20);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BDB8);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BE30);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BED8);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BF30);

INCLUDE_RODATA(const s32, "script/scrScriptProcess", D_0039E288);

