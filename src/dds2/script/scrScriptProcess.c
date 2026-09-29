#include "common.h"

#include "scr.h"

void *func_00343ED0(s32 arg0, u32 *arg1, s32 arg2);

s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

s32 bfTaskUpdate();

s32 func_0010C298();

extern ScrProcGlobals *D_00435DD0;

s32 bfParseFLW0(s32 arg0, s32 arg1);

s32 bfContextCreate(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7);

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
    taskId = kwlnTaskCreate(task->unkB4 + (task->unkC8 << 5), priority, 1, 1, bfTaskUpdate, func_0010C298, (s32)task);
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

void scrCreateProcessTaskFromResource(s32 priority, s32 scriptId, s32 option)
{
    scrProcCreateTask(priority, scrOpenProcessFromResource(scriptId, option));
}

void scrCreateProcessWithDefaultOption(u32 scriptId) {
    scrOpenProcessFromResource(scriptId, 0);
}

void scrCreateTaskForProcessId(s32 priority, s32 taskId, s32 option)
{
    scrProcCreateTask(priority, bfParseFLW0(taskId, option));
}

void scrCreateTaskWithDefaultOption(u32 processId) {
    bfParseFLW0(processId, 0);
}

s32 func_0010BEE0(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8)
{
    return scrProcCreateTask(a0, bfContextCreate(a1, a2, a3, a4, a5, a6, a7, a8));
}

s32 func_0010BF30(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6)
{
    return bfContextCreate(arg0, arg1, arg2, arg3, arg4, arg5, arg6, 0);
}

extern void func_0010AE38(char *, u32);
extern void func_00328E48(void *);
extern void func_001A39D0(s32);
extern void func_003297C8(void *);
extern void evtUnlinkWorkNode(void *);

void func_0010BF48(u8 *ctx) {
    func_0010AE38("end <%s>\n", *(u32 *)(ctx + 0xB4) + (*(u32 *)(ctx + 0xC8) << 5));
    if (*(u32 *)(ctx + 0xDC) != 0) {
        func_00328E48((void *)*(u32 *)(ctx + 0xDC));
    }
    if (*(u32 *)(ctx + 0xE0) != 0) {
        func_00328E48((void *)*(u32 *)(ctx + 0xE0));
    }
    if (*(s32 *)(ctx + 0xCC) >= 0) {
        func_001A39D0(*(s32 *)(ctx + 0xCC));
    }
    if (*(u32 *)(ctx + 0xD8) != 0) {
        func_003297C8((void *)*(u32 *)(ctx + 0xD8));
    }
    evtUnlinkWorkNode(ctx);
    func_00328E48(ctx);
}

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BFE0);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010C058);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010C100);

INCLUDE_ASM(const s32, "script/scrScriptProcess", bfFindScriptIndexByName);

INCLUDE_RODATA(const s32, "script/scrScriptProcess", D_00411408);

