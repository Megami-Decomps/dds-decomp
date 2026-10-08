#include "common.h"

#include "scr.h"
#include "sdf.h"

SdfMemBlock *sdfReadNamedResource(const char *path, u32 *outAddress, u32 *outSize);

KwlnTask *kwlnTaskCreate(const char *name, u32 priority, s32 startDelay,
                         s32 destroyDelay, TaskUpdate update, TaskDestroy destroy,
                         u32 userValue);

extern ScrProcGlobals *datGameState;

ScrData *bfParseFLW0(void *header, s32 procedureIndex);


/* Load a script resource, create its VM process and retain its resource handle. */
ScrData *scrOpenProcessFromResource(const char *path, s32 option)
{
    u32 resourceInfo[4];
    SdfMemBlock *handle;
    u32 resourceAddress;
    ScrData *task;
    handle = sdfReadNamedResource(path, resourceInfo, 0);
    resourceAddress = resourceInfo[0];
    if (resourceAddress == 0)
    {
        return 0;
    }
    task = bfParseFLW0((void *)resourceAddress, option);
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

s32 scrCreateProcessTaskFromResource(s32 priority, const char *path, s32 option)
{
    return (s32)scrProcCreateTask(priority, scrOpenProcessFromResource(path, option));
}

ScrData *scrCreateProcessWithDefaultOption(const char *path) {
    return scrOpenProcessFromResource(path, 0);
}

s32 scrCreateTaskForProcessId(s32 priority, s32 processId, s32 option)
{
    return (s32)scrProcCreateTask(priority, bfParseFLW0((void *)processId, option));
}

ScrData *scrCreateTaskWithDefaultOption(void *header) {
    return bfParseFLW0(header, 0);
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
extern void sdfReleaseResourceAllocation(SdfMemBlock *);
extern void evtUnlinkWorkNode(ScrData *process);

/* Release the process's VM buffers, resource and named-list allocation. */
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

extern u8 scrIsCurrentWorkTask(u32);
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
        if (scrIsCurrentWorkTask((u32)node) == 0) {
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

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010C058);

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

INCLUDE_RODATA(const s32, "script/scrScriptProcess", D_00411408);

