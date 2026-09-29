#include "common.h"
#include "scr.h"
extern ScrProcGlobals *D_003BAA00;

void *func_002EB028(s32 arg0, u32 *arg1, s32 arg2);
s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
s32 bfTaskUpdate();
s32 func_0010C070();
s32 bfContextCreate(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7);
s32 bfParseFLW0(s32 processId, s32 option);
/* Load a script resource, create its VM process and retain its resource handle. */
s32 scrOpenProcessFromResource(s32 scriptId, s32 option)
{
    u32 resourceInfo[4];
    void *handle;
    s32 processId;
    ScrProcTask *task;
    handle = func_002EB028(scriptId, resourceInfo, 0);
    processId = resourceInfo[0];
    if (processId == 0)
    {
        return 0;
    }
    task = (ScrProcTask *)bfParseFLW0(processId, option);
    if (task != NULL)
    {
        task->scriptHandle = handle;
    }
    return (s32)task;
}

s32 scrProcCreateTask(s32 priority, ScrProcTask *task)
{
    s32 taskId;
    taskId = kwlnTaskCreate(task->nameTableBase + (task->nameIndex << 5), priority, 1, 1, bfTaskUpdate, func_0010C070, (s32)task);
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

void scrCreateTaskForProcessId(s32 priority, s32 processId, s32 option)
{
    scrProcCreateTask(priority, bfParseFLW0(processId, option));
}

s32 scrCreateTaskWithDefaultOption(s32 processId)
{
    return bfParseFLW0(processId, 0);
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
extern void evtUnlinkWorkNode(void *);

/* Log the process name, release its VM buffers and resource, then unlink and free it. */
void scrProcDestroyTask(ScrProcTask *process) {
    func_0010AC10("end <%s>\n", process->nameTableBase + (process->nameIndex << 5));
    if (process->workBuffer != 0) {
        func_002CFF98((void *)process->workBuffer);
    }
    if (process->auxBuffer != 0) {
        func_002CFF98((void *)process->auxBuffer);
    }
    if (process->resourceIndex >= 0) {
        func_0019B9A0(process->resourceIndex);
    }
    if (process->scriptHandle != 0) {
        func_002D0918(process->scriptHandle);
    }
    evtUnlinkWorkNode(process);
    func_002CFF98(process);
}

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BDB8);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BE30);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BED8);

INCLUDE_ASM(const s32, "script/scrScriptProcess", bfFindScriptIndexByName);

INCLUDE_RODATA(const s32, "script/scrScriptProcess", D_0039E288);

