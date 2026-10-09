#include "common.h"
#include "sdf_chip.h"
#include "kwln.h"
#include "sdf_resource.h"

#include "scr.h"
#include "sdf.h"
#include "kwln_task_lifecycle.h"


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
extern void itfMesDestroyWindowIfPresent(s32);
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

/* Start procedure `index` on a script process: copy its name, set pc, clear the stack and timers. Returns 0 for an out-of-range index. */
s32 func_0010C058(ScrData *process, s32 index)
{
    s32 i;

    if (index < 0 || index >= process->sections->count) {
        return 0;
    }
    for (i = 0; (process->name[i] = process->procedures[index].name[i]) != 0; i++) {
    }
    process->pc = process->procedures[index].addr;
    process->sp = 0;
    {
        ScrStackValue *value = process->stackValues;
        s8 *type = process->stackTypes;

        for (i = SCR_STACK_RET; i >= 0; i--) {
            *type++ = 0;
            value++->i = 0;
        }
    }
    process->procedureIndex = index;
    process->timer = 0;
    process->cmdTimer = 0;
    return 1;
}

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

typedef struct BfFlw0Lookup {
    u8 pad00[0x10];
    s32 sectionCount;
    u8 pad14[0xC];
    ScrSection sections[1];
} BfFlw0Lookup;

/* Index of the named procedure in the FLW0 procedure section, or -1. */
s32 bfFindScriptIndexByName(BfFlw0Lookup *header, const char *name) {
    ScrLabel *procedures = NULL;
    ScrSection *sections;
    s32 i;
    s32 k;

    if (header == NULL) {
        return -1;
    }
    sections = header->sections;
    for (i = 0; i < header->sectionCount; i++) {
        if (sections[i].type == 0) {
            procedures = (ScrLabel *)((u8 *)header + sections[i].offset);
            break;
        }
    }
    if (procedures == NULL) {
        return -1;
    }
    for (k = 0; k < sections[i].count; k++) {
        if (strcmp(procedures[k].name, name) == 0) {
            return k;
        }
    }
    return -1;
}

INCLUDE_RODATA(const s32, "script/scrScriptProcess", D_00411408);

