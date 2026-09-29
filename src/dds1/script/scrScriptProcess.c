#include "common.h"
#include "scr.h"
extern ScrProcGlobals *D_003BAA00;

void *func_002EB028(s32 arg0, u32 *arg1, s32 arg2);
s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
s32 bfTaskUpdate();
s32 func_0010C070();
s32 bfContextCreate(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7);
s32 bfParseFLW0(s32 arg0, s32 arg1);
s32 scrOpenProcessFromResource(s32 scriptId, s32 option)
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
    task = (ScrProcTask *)bfParseFLW0(id, option);
    if (task != NULL)
    {
        task->scriptHandle = handle;
    }
    return (s32)task;
}

s32 scrProcCreateTask(s32 priority, ScrProcTask *task)
{
    s32 taskId;
    taskId = kwlnTaskCreate(task->unkB4 + (task->unkC8 << 5), priority, 1, 1, bfTaskUpdate, func_0010C070, (s32)task);
    task->taskId = taskId;
    return taskId;
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

void scrCreateProcessTaskFromResource(s32 priority, s32 scriptId, s32 option)
{
    scrProcCreateTask(priority, scrOpenProcessFromResource(scriptId, option));
}

s32 scrCreateProcessWithDefaultOption(s32 scriptId)
{
    return scrOpenProcessFromResource(scriptId, 0);
}

void scrCreateTaskForProcessId(s32 priority, s32 taskId, s32 option)
{
    scrProcCreateTask(priority, bfParseFLW0(taskId, option));
}

s32 scrCreateTaskWithDefaultOption(s32 taskId)
{
    return bfParseFLW0(taskId, 0);
}

s32 func_0010BCB8(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8)
{
    return scrProcCreateTask(a0, bfContextCreate(a1, a2, a3, a4, a5, a6, a7, a8));
}

s32 func_0010BD08(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6)
{
    return bfContextCreate(arg0, arg1, arg2, arg3, arg4, arg5, arg6, 0);
}

extern void func_0010AC10(char *, u32);
extern void func_002CFF98(void *);
extern void func_0019B9A0(s32);
extern void func_002D0918(void *);
extern void evtUnkB768Unlink(void *);

void func_0010BD20(u8 *ctx) {
    func_0010AC10("end <%s>\n", *(u32 *)(ctx + 0xB4) + (*(u32 *)(ctx + 0xC8) << 5));
    if (*(u32 *)(ctx + 0xDC) != 0) {
        func_002CFF98((void *)*(u32 *)(ctx + 0xDC));
    }
    if (*(u32 *)(ctx + 0xE0) != 0) {
        func_002CFF98((void *)*(u32 *)(ctx + 0xE0));
    }
    if (*(s32 *)(ctx + 0xCC) >= 0) {
        func_0019B9A0(*(s32 *)(ctx + 0xCC));
    }
    if (*(u32 *)(ctx + 0xD8) != 0) {
        func_002D0918((void *)*(u32 *)(ctx + 0xD8));
    }
    evtUnkB768Unlink(ctx);
    func_002CFF98(ctx);
}

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BDB8);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BE30);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BED8);

INCLUDE_ASM(const s32, "script/scrScriptProcess", bfFindScriptIndexByName);

INCLUDE_RODATA(const s32, "script/scrScriptProcess", D_0039E288);

