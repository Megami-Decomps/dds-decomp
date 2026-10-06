#include "common.h"
#include "scr.h"
extern ScrProcGlobals *datGameState;

void *sdfReadNamedResource(s32 arg0, u32 *arg1, s32 arg2);
KwlnTask *kwlnTaskCreate(const char *name, u32 priority, s32 startDelay,
                         s32 destroyDelay, TaskUpdate update, TaskDestroy destroy,
                         u32 userValue);
ScrData *bfParseFLW0(void *header, s32 procedureIndex);
/* Load a script resource, create its VM process and retain its resource handle. */
ScrData *scrOpenProcessFromResource(s32 scriptId, s32 option)
{
    u32 resourceInfo[4];
    void *handle;
    s32 processId;
    ScrData *task;
    handle = sdfReadNamedResource(scriptId, resourceInfo, 0);
    processId = resourceInfo[0];
    if (processId == 0)
    {
        return 0;
    }
    task = bfParseFLW0((void *)processId, option);
    if (task != NULL)
    {
        task->scriptHandle = handle;
    }
    return task;
}

KwlnTask *scrProcCreateTask(u32 priority, ScrData *process)
{
    KwlnTask *task;
    task = kwlnTaskCreate(process->procedures[process->procedureIndex].name,
                          priority, 1, 1, bfTaskUpdate, scrReplaceCurrentTask,
                          (u32)process);
    process->task = task;
    return task;
}

void scrClearProcessGlobals(void)
{
    s32 i;
    /* Countdown with a forward index; gcc keeps a single pointer (see asm). */
    for (i = 255; i >= 0; i--)
    {
        datGameState->integers[255 - i] = 0;
        datGameState->floatBits[255 - i] = 0;
    }
}

s32 scrCreateProcessTaskFromResource(s32 priority, s32 scriptId, s32 option)
{
    return (s32)scrProcCreateTask(priority, scrOpenProcessFromResource(scriptId, option));
}

ScrData *scrCreateProcessWithDefaultOption(s32 scriptId)
{
    return scrOpenProcessFromResource(scriptId, 0);
}

s32 scrCreateTaskForProcessId(s32 priority, s32 processId, s32 option)
{
    return (s32)scrProcCreateTask(priority, bfParseFLW0((void *)processId, option));
}

ScrData *scrCreateTaskWithDefaultOption(s32 processId)
{
    return bfParseFLW0((void *)processId, 0);
}

KwlnTask *scrCreateTaskFromContextParameters(u32 priority, void *header,
    ScrSection *procedureSection, ScrLabel *procedures, ScrLabel *labels,
    ScrInstr *instructions, void *auxiliaryData, char *strings, s32 procedureIndex)
{
    return scrProcCreateTask(priority, bfContextCreate(header, procedureSection, procedures, labels, instructions, auxiliaryData, strings, procedureIndex));
}

ScrData *scrCreateProcessAtFirstProcedure(void *header, ScrSection *procedureSection,
    ScrLabel *procedures, ScrLabel *labels, ScrInstr *instructions,
    void *auxiliaryData, char *strings)
{
    return bfContextCreate(header, procedureSection, procedures, labels, instructions, auxiliaryData, strings, 0);
}

extern void evtPrintDeveloperConsoleMessage(const char *fmt, ...);
extern void sdfReleaseChipBlock(void *);
extern void itfMesDestroyWindowIfPresent(s32);
extern void sdfReleaseResourceAllocation(void *);
extern void evtUnlinkWorkNode(ScrData *process);

/* Log the process name, release its VM buffers and resource, then unlink and free it. */
void scrProcDestroyTask(ScrData *process) {
    evtPrintDeveloperConsoleMessage("end <%s>\n", process->procedures[process->procedureIndex].name);
    if (process->localInt != NULL) {
        sdfReleaseChipBlock(process->localInt);
    }
    if (process->localFloat != NULL) {
        sdfReleaseChipBlock(process->localFloat);
    }
    if (process->resourceIndex >= 0) {
        itfMesDestroyWindowIfPresent(process->resourceIndex);
    }
    if (process->scriptHandle != 0) {
        sdfReleaseResourceAllocation(process->scriptHandle);
    }
    evtUnlinkWorkNode(process);
    sdfReleaseChipBlock(process);
}

extern s32 strcmp(const char *a, const char *b);

extern s32 scrIsCurrentWorkTask(void *);
extern void kwlnTaskDestroyWithHierarchy(KwlnTask *task, s32 flag);

/* Walk the script-name table, releasing each node's task or process. */
void scrDestroyAllNamedProcesses(void)
{
    ScrData *node;
    ScrData *next;

    node = scrNamedProcessHead;
    if (node == NULL) {
        return;
    }
    while (1) {
        next = node->next;
        if (scrIsCurrentWorkTask(node) == 0) {
            if (node->task != NULL) {
                kwlnTaskDestroyWithHierarchy(node->task, 0);
            } else {
                scrProcDestroyTask(node);
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
ScrData *scrFindNamedProcessNode(char *name) {
    ScrData *node = scrNamedProcessHead;

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

