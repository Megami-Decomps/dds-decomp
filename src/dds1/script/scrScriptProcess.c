#include "common.h"
#include "scr.h"
extern ScrProcGlobals *D_003BAA00;

void *func_002EB028(s32 arg0, u32 *arg1, s32 arg2);
s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
s32 bfTaskUpdate();
s32 scrReplaceCurrentTask();
s32 bfContextCreate(s32 header, s32 procedureSection, s32 procedures, s32 labels, s32 instructions, s32 auxiliaryData, s32 strings, s32 procedureIndex);
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

s32 scrCreateTaskFromContextParameters(s32 priority, s32 header, s32 procedureSection, s32 procedures, s32 labels, s32 instructions, s32 auxiliaryData, s32 strings, s32 procedureIndex)
{
    return scrProcCreateTask(priority, bfContextCreate(header, procedureSection, procedures, labels, instructions, auxiliaryData, strings, procedureIndex));
}

s32 func_0010BD08(s32 header, s32 procedureSection, s32 procedures, s32 labels, s32 instructions, s32 auxiliaryData, s32 strings)
{
    return bfContextCreate(header, procedureSection, procedures, labels, instructions, auxiliaryData, strings, 0);
}

extern void evtPrintDeveloperConsoleMessage(char *, u32);
extern void sdfReleaseChipBlock(void *);
extern void itfMesDestroyWindowIfPresent(s32);
extern void func_002D0918(void *);
extern void evtUnlinkWorkNode(void *);

/* Log the process name, release its VM buffers and resource, then unlink and free it. */
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
        func_002D0918(process->scriptHandle);
    }
    evtUnlinkWorkNode(process);
    sdfReleaseChipBlock(process);
}

/* Node of the script-name table walked by scrDestroyAllNamedProcesses. */
typedef struct ScriptNameNode {
    char name[1];                 /* 0x0: the name text is stored in place */
    u8 pad01[0xE3];
    s32 taskId;                    /* 0xE4: task id, or 0 for a plain process */
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
            if (node->taskId != 0) {
                kwlnTaskDestroyWithHierarchy(node->taskId, 0);
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

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BE30);

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

INCLUDE_RODATA(const s32, "script/scrScriptProcess", D_0039E288);

