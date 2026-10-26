#include "sdk_ps2.h"
#include "dat_state.h"
#include "common.h"
#include "sdf_chip.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "sdf_resource.h"

#include "scr.h"
#include "scr_instruction_stream_api.h"
#include "scr_work_task_api.h"
#include "bf_flw0_header.h"
#include "sdf.h"
#include "kwln_task_lifecycle.h"




scrProcessWork_s *fldFlowParse(const BfFlw0Header *header, ScrProcedureIndex procedureIndex);


/* Load a script resource, create its VM process and retain its resource handle. */
scrProcessWork_s *scrLoadScriptFile(const char *path, ScrProcedureIndex procedureIndex)
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
    process = fldFlowParse((const BfFlw0Header *)resourceAddress, procedureIndex);
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

void scrClearProcessGlobals(void)
{
    s32 globalClearCountdown;
    /* Countdown with a forward index; gcc keeps a single pointer (see asm). */
    for (globalClearCountdown = 255; globalClearCountdown >= 0; globalClearCountdown--)
    {
        DDS3_GBWK->script.ints[255 - globalClearCountdown] = 0;
        DDS3_GBWK->script.floats[255 - globalClearCountdown] = 0;
    }
}

dds3ProcessID_t *scrStartScriptByName(u32 priority, const char *path, ScrProcedureIndex procedureIndex)
{
    return scrStartScriptProcess(priority, scrLoadScriptFile(path, procedureIndex));
}

scrProcessWork_s *scrCreateProcessWithDefaultOption(const char *path) {
    return scrLoadScriptFile(path, 0);
}

dds3ProcessID_t *scrStartScript2(s32 priority, BfFlw0Header *script, ScrProcedureIndex procedureIndex)
{
    return scrStartScriptProcess(priority, fldFlowParse(script, procedureIndex));
}

scrProcessWork_s *scrCreateTaskWithDefaultOption(BfFlw0Header *header) {
    return fldFlowParse(header, 0);
}

dds3ProcessID_t *scrRunScriptEx(u32 priority, const BfFlw0Header *header,
    const ScrSection *procedureSection, const ScrLabel *procedures, const ScrLabel *labels,
    const ScrInstr *instructions, struct ItfMesSub *auxiliaryData, const char *strings, ScrProcedureIndex procedureIndex)
{
    return scrStartScriptProcess(priority, bfContextCreate(header, procedureSection, procedures, labels, instructions, auxiliaryData, strings, procedureIndex));
}

scrProcessWork_s *scrCreateProcessAtFirstProcedure(BfFlw0Header *header, ScrSection *procedureSection,
    ScrLabel *procedures, ScrLabel *labels, const ScrInstr *instructions,
    struct ItfMesSub *auxiliaryData, const char *strings)
{
    return bfContextCreate(header, procedureSection, procedures, labels, instructions, auxiliaryData, strings, 0);
}

extern void evtPrintDeveloperConsoleMessage(const char *fmt, ...);
extern void itfMesMngRelease(ItfMesWindowIndex window);
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
    if (process->messageWindowIndex >= 0) {
        itfMesMngRelease(process->messageWindowIndex);
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
s32 scrStartProcessProcedure(scrProcessWork_s *process, ScrProcedureIndex procedureIndex)
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
        ScrStackValueType *stackType = process->stackTypes;

        for (cursor = SCR_STACK_RET; cursor >= 0; cursor--) {
            *stackType++ = 0;
            stackValue++->bits = 0;
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
ScrProcedureIndex scrSearchProcedureName(const BfFlw0Header *header, const char *procedureName) {
    const ScrLabel *procedures = NULL;
    const ScrSection *sections;
    s32 sectionIndex;
    ScrProcedureIndex procedureIndex;

    if (header == NULL) {
        return SCR_PROCEDURE_INDEX_NOT_FOUND;
    }
    sections = header->sections;
    for (sectionIndex = 0; sectionIndex < header->sectionCount; sectionIndex++) {
        if (sections[sectionIndex].type == BF_FLW0_SECTION_PROCEDURES) {
            procedures = (const ScrLabel *)((const u8 *)header + sections[sectionIndex].offset);
            break;
        }
    }
    if (procedures == NULL) {
        return SCR_PROCEDURE_INDEX_NOT_FOUND;
    }
    for (procedureIndex = 0; procedureIndex < sections[sectionIndex].count; procedureIndex++) {
        if (strcmp(procedures[procedureIndex].name, procedureName) == 0) {
            return procedureIndex;
        }
    }
    return SCR_PROCEDURE_INDEX_NOT_FOUND;
}

INCLUDE_RODATA(const s32, "script/scrScriptProcess", D_00411408);
