#include "sdk_ps2.h"
#include "dat_state.h"
#include "common.h"
#include "sdf_chip.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "sdf_resource.h"

#include "scr.h"
#include "scr_work_task_api.h"
#include "bf_flw0_header.h"
#include "sdf.h"
#include "kwln_task_lifecycle.h"




scrProcessWork_s *fldFlowParse(BfFlw0Header *header, s32 procedureIndex);


/* Load a script resource, create its VM process and retain its resource handle. */
scrProcessWork_s *scrLoadScriptFile(const char *path, s32 option)
{
    u32 resourceInfo[4];
    sdfMemHandle_t *handle;
    u32 resourceAddress;
    scrProcessWork_s *process;
    handle = sdfDevLoadFileEx(path, resourceInfo, 0);
    resourceAddress = resourceInfo[0];
    if (resourceAddress == 0)
    {
        return 0;
    }
    process = fldFlowParse((BfFlw0Header *)resourceAddress, option);
    if (process != NULL)
    {
        process->scriptHandle = handle;
    }
    return process;
}

dds3ProcessID_t *scrStartScriptProcess(u32 priority, scrProcessWork_s *process)
{
    dds3ProcessID_t *task;
    task = dds3StartProcess(process->procedures[process->procedureIndex].name,
                          priority, 1, 1, scrScriptProcess, scrScriptShutdown,
                          (u32)process);
    process->task = task;
    return task;
}

void dds3GlobalWorkClearFlags(void)
{
    s32 globalClearCountdown;
    /* Countdown with a forward index; gcc keeps a single pointer (see asm). */
    for (globalClearCountdown = 255; globalClearCountdown >= 0; globalClearCountdown--)
    {
        DDS3_GBWK->script.ints[255 - globalClearCountdown] = 0;
        DDS3_GBWK->script.floats[255 - globalClearCountdown] = 0;
    }
}

dds3ProcessID_t *scrStartScriptByName(s32 priority, const char *path, s32 option)
{
    return scrStartScriptProcess(priority, scrLoadScriptFile(path, option));
}

scrProcessWork_s *scrCreateProcessWithDefaultOption(const char *path) {
    return scrLoadScriptFile(path, 0);
}

dds3ProcessID_t *scrStartScript2(s32 priority, BfFlw0Header *script, s32 option)
{
    return scrStartScriptProcess(priority, fldFlowParse(script, option));
}

scrProcessWork_s *scrCreateTaskWithDefaultOption(BfFlw0Header *header) {
    return fldFlowParse(header, 0);
}

dds3ProcessID_t *scrRunScriptEx(u32 priority, BfFlw0Header *header,
    ScrSection *procedureSection, ScrLabel *procedures, ScrLabel *labels,
    ScrInstr *instructions, void *auxiliaryData, char *strings, s32 procedureIndex)
{
    return scrStartScriptProcess(priority, bfContextCreate(header, procedureSection, procedures, labels, instructions, auxiliaryData, strings, procedureIndex));
}

scrProcessWork_s *scrCreateProcessAtFirstProcedure(BfFlw0Header *header, ScrSection *procedureSection,
    ScrLabel *procedures, ScrLabel *labels, ScrInstr *instructions,
    void *auxiliaryData, char *strings)
{
    return bfContextCreate(header, procedureSection, procedures, labels, instructions, auxiliaryData, strings, 0);
}

extern void evtPrintDeveloperConsoleMessage(const char *fmt, ...);
extern void itfMesMngRelease(s32);
extern void scrUnlinkHandle(scrProcessWork_s *process);

/* Release the process's VM buffers, resource and named-list allocation. */
void scrProcDestroyTask(scrProcessWork_s *process) {
    evtPrintDeveloperConsoleMessage("end <%s>\n", process->procedures[process->procedureIndex].name);
    if (process->localInt != NULL) {
        sdfChipFreeDirect(process->localInt);
    }
    if (process->localFloat != NULL) {
        sdfChipFreeDirect(process->localFloat);
    }
    if (process->resourceIndex >= 0) {
        itfMesMngRelease(process->resourceIndex);
    }
    if (process->scriptHandle != 0) {
        sdfMemFreeDirect(process->scriptHandle);
    }
    scrUnlinkHandle(process);
    sdfChipFreeDirect(process);
}



/* Walk the script-name table, releasing each node's task or process. */
void scrDestroyAllNamedProcesses(void)
{
    scrProcessWork_s *processNode;
    scrProcessWork_s *nextProcessNode;

    processNode = scrHandleFirst;
    if (processNode == NULL) {
        return;
    }
    while (1) {
        nextProcessNode = processNode->next;
        if (scrIsCurrentWorkTask(processNode) == 0) {
            if (processNode->task != NULL) {
                dds3KillProcess(processNode->task, 0);
            } else {
                scrProcDestroyTask(processNode);
            }
        }
        processNode = nextProcessNode;
        if (processNode == NULL) {
            break;
        }
    }
}

/* Start a procedure on a script process: copy its name, set pc, clear the stack and timers. */
s32 scrStartProcessProcedure(scrProcessWork_s *process, s32 procedureIndex)
{
    s32 cursor;

    if (procedureIndex < 0 || procedureIndex >= process->sections->count) {
        return 0;
    }
    for (cursor = 0; (process->name[cursor] = process->procedures[procedureIndex].name[cursor]) != 0; cursor++) {
    }
    process->pc = process->procedures[procedureIndex].addr;
    process->sp = 0;
    {
        ScrStackValue *stackValue = process->stackValues;
        s8 *stackType = process->stackTypes;

        for (cursor = SCR_STACK_RET; cursor >= 0; cursor--) {
            *stackType++ = 0;
            stackValue++->i = 0;
        }
    }
    process->procedureIndex = procedureIndex;
    process->timer = 0;
    process->cmdTimer = 0;
    return 1;
}

/* Find the node whose name matches, or NULL when the table is exhausted. */
scrProcessWork_s *scrSearchScriptName(const char *name) {
    scrProcessWork_s *processNode = scrHandleFirst;

    while (processNode != NULL) {
        if (strcmp(name, processNode->name) == 0) {
            return processNode;
        }
        processNode = processNode->next;
    }
    return NULL;
}

/* Index of the named procedure in the FLW0 procedure section, or -1. */
s32 scrSearchProcedureName(const BfFlw0Header *header, const char *procedureName) {
    const ScrLabel *procedures = NULL;
    const ScrSection *sections;
    s32 sectionIndex;
    s32 procedureIndex;

    if (header == NULL) {
        return -1;
    }
    sections = header->sections;
    for (sectionIndex = 0; sectionIndex < header->sectionCount; sectionIndex++) {
        if (sections[sectionIndex].type == 0) {
            procedures = (const ScrLabel *)((const u8 *)header + sections[sectionIndex].offset);
            break;
        }
    }
    if (procedures == NULL) {
        return -1;
    }
    for (procedureIndex = 0; procedureIndex < sections[sectionIndex].count; procedureIndex++) {
        if (strcmp(procedures[procedureIndex].name, procedureName) == 0) {
            return procedureIndex;
        }
    }
    return -1;
}

INCLUDE_RODATA(const s32, "script/scrScriptProcess", D_00411408);

