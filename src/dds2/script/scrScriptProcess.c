#include "common.h"

#include "scr.h"

void *func_00343ED0(s32 arg0, u32 *arg1, s32 arg2);

s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

s32 bfTaskUpdate();

s32 scrReplaceCurrentTask();

extern ScrProcGlobals *D_00435DD0;

s32 bfParseFLW0(s32 arg0, s32 arg1);

s32 bfContextCreate(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7);

/* Load a script resource, create its VM process and retain its resource handle. */
s32 scrOpenProcessFromResource(s32 scriptId, s32 option)
{
    u32 resourceInfo[4];
    void *handle;
    s32 processId;
    ScrProcTask *task;
    handle = func_00343ED0(scriptId, resourceInfo, 0);
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
    taskId = kwlnTaskCreate(task->nameTableBase + (task->nameIndex << 5), priority, 1, 1, bfTaskUpdate, scrReplaceCurrentTask, (s32)task);
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

void scrCreateTaskForProcessId(s32 priority, s32 processId, s32 option)
{
    scrProcCreateTask(priority, bfParseFLW0(processId, option));
}

void scrCreateTaskWithDefaultOption(u32 processId) {
    bfParseFLW0(processId, 0);
}

s32 scrCreateTaskFromContextParameters(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8)
{
    return scrProcCreateTask(a0, bfContextCreate(a1, a2, a3, a4, a5, a6, a7, a8));
}

s32 func_0010BF30(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6)
{
    return bfContextCreate(arg0, arg1, arg2, arg3, arg4, arg5, arg6, 0);
}

extern void evtPrintDeveloperConsoleMessage(char *, u32);
extern void sdfReleaseChipBlock(void *);
extern void itfMesDestroyWindowIfPresent(s32);
extern void func_003297C8(void *);
extern void evtUnlinkWorkNode(void *);

/* DDS2 process teardown follows the same layout as DDS1's ScrProcTask. */
void scrProcDestroyTask(ScrProcTask *process) {
    evtPrintDeveloperConsoleMessage("end <%s>\n", process->nameTableBase + (process->nameIndex << 5));
    if (process->workBuffer != 0) {
        sdfReleaseChipBlock((void *)process->workBuffer);
    }
    if (process->auxBuffer != 0) {
        sdfReleaseChipBlock((void *)process->auxBuffer);
    }
    if (process->resourceIndex >= 0) {
        itfMesDestroyWindowIfPresent(process->resourceIndex);
    }
    if (process->scriptHandle != 0) {
        func_003297C8(process->scriptHandle);
    }
    evtUnlinkWorkNode(process);
    sdfReleaseChipBlock(process);
}

/* Node of the script-name table walked by scrFindNamedProcessNode. */
typedef struct ScriptNameNode {
    char name[1];                 /* 0x0: the name text is stored in place */
    u8 pad01[0xE3];
    s32 unkE4;                    /* 0xE4: task id, or 0 for a plain process */
    u8 padE8[4];
    struct ScriptNameNode *next;   /* 0xEC */
} ScriptNameNode;

extern ScriptNameNode *scrNamedProcessHead;
extern s32 strcmp(const char *a, const char *b);

extern s32 scrIsCurrentWorkTask(void *);
extern void kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);


/* Walk the script-name table, releasing each node's task or process. */
void scrDestroyAllNamedProcesses(void)
{
    ScriptNameNode *node;
    ScriptNameNode *next;

    node = scrNamedProcessHead;
    if (node == NULL) {
        return;
    }
    while (1) {
        next = node->next;
        if (scrIsCurrentWorkTask(node) == 0) {
            if (node->unkE4 != 0) {
                kwlnTaskDestroyWithHierarchy(node->unkE4, 0);
            } else {
                scrProcDestroyTask((ScrProcTask *)node);
            }
        }
        node = next;
        if (node == NULL) {
            break;
        }
    }
}

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010C058);

/* Find the node whose name matches, or NULL when the table is exhausted. */
ScriptNameNode *scrFindNamedProcessNode(char *name) {
    ScriptNameNode *node = scrNamedProcessHead;

    while (node != NULL) {
        if (strcmp(name, node->name) == 0) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

INCLUDE_ASM(const s32, "script/scrScriptProcess", bfFindScriptIndexByName);

INCLUDE_RODATA(const s32, "script/scrScriptProcess", D_00411408);

