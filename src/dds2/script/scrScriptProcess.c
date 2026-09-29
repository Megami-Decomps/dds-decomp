#include "common.h"

#include "scr.h"

void *func_00343ED0(s32 arg0, u32 *arg1, s32 arg2);

s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

s32 func_0010C2F0();

s32 func_0010C298();

extern ScrProcGlobals *D_00435DD0;

s32 func_0010BC40(s32 arg0, s32 arg1);

s32 func_0010B9E8(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7);

s32 scrOpenProcessFromResource(s32 scriptId, s32 option)
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
    s32 taskId;
    taskId = kwlnTaskCreate(task->unkB4 + (task->unkC8 << 5), priority, 1, 1, func_0010C2F0, func_0010C298, (s32)task);
    task->taskId = taskId;
    return taskId;
}

void scrClearProcessGlobals(void)
{
    s32 i;
    /* Countdown with a forward index; gcc keeps a single pointer (see asm). */
    for (i = 255; i >= 0; i--)
    {
        D_00435DD0->integers[255 - i] = 0;
        D_00435DD0->floatBits[255 - i] = 0;
    }
}

void func_0010BE40(s32 priority, s32 scriptId, s32 option)
{
    scrProcCreateTask(priority, scrOpenProcessFromResource(scriptId, option));
}

void scrCreateProcessWithDefaultOption(u32 scriptId) {
    scrOpenProcessFromResource(scriptId, 0);
}

void func_0010BE90(s32 priority, s32 taskId, s32 option)
{
    scrProcCreateTask(priority, func_0010BC40(taskId, option));
}

void scrCreateTaskWithDefaultOption(u32 processId) {
    func_0010BC40(processId, 0);
}

s32 func_0010BEE0(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8)
{
    return scrProcCreateTask(a0, func_0010B9E8(a1, a2, a3, a4, a5, a6, a7, a8));
}

s32 func_0010BF30(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6)
{
    return func_0010B9E8(arg0, arg1, arg2, arg3, arg4, arg5, arg6, 0);
}

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BF48);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BFE0);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010C058);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010C100);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010C158);

INCLUDE_RODATA(const s32, "script/scrScriptProcess", D_00411408);
