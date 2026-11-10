#include "common.h"
#include "sdf_resource.h"
/* DDS's global work keeps the script variables at SMT3's offsets. */
typedef struct
{
    u8 unk0[0x40];
    s32 integers[256];
    f32 floats[256];
} dds3GlobalWork_t;
extern dds3GlobalWork_t *DDS3_GBWK;
#include "scr.h"

#define datDebugPrintf evtPrintDeveloperConsoleMessage
#define scrPrintProcedureStart bfContextCreate
extern s32 scrIsCurrentWorkTask(scrProcessWork_s *pw);

extern s32 scrHandleCount;
extern scrProcessWork_s *scrHandleFirst;

extern s32 strcmp(const char *s1, const char *s2);

extern dds3ProcessID_t *dds3StartProcess(char *v1, s32 dDS3_PRIORITY_DRAW_PB, s32 v2, s32 v3,
                                         s32 (*drawExit)(dds3ProcessID_t *id),
                                         void (*deleteExit)(dds3ProcessID_t *id),
                                         void *p);


extern scrProcessWork_s *fldFlowParse(void *data, s32 messageHandle);
extern scrProcessWork_s *scrPrintProcedureStart(scrHeader_t *header, scrTypeTable_t *typeTable,
                                                scrProcTable_t *procTable,
                                                scrLabelTable_t *labelTable, scrCode_u *code,
                                                u8 *messageData, u8 *stringData, s32 procNo);
extern void scrUnlinkHandle(scrProcessWork_s *pw);
extern void sdfChipFreeDirect(void *aPtr);
extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);
extern void itfMesMngRelease(s32 id);
extern s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);
#include "sdk_libc.h"

extern void datDebugPrintf(const char *fmt, ...);
extern s32 scrTraceCode(scrProcessWork_s *pwork);

extern s32 scrScriptProcess(dds3ProcessID_t *id);
void scrScriptShutdown(dds3ProcessID_t *id);

scrProcessWork_s *scrLoadScriptFile(char *name, s32 messageHandle)
{
    sdfMemHandle_t *mem;
    void *data;
    scrProcessWork_s *pw;

    mem = sdfDevLoadFileEx(name, (u32 *)&data, NULL);
    if (data == NULL) {
        return NULL;
    }

    pw = fldFlowParse(data, messageHandle);
    if (pw != NULL) {
        pw->MemHandle = mem;
    }
    return pw;
}

dds3ProcessID_t *scrStartScriptProcess(s32 priority, scrProcessWork_s *pw)
{
    return pw->ProcessID = dds3StartProcess((char *)pw->ProcTable[pw->StartProc].Name, priority, 1, 1,
                                            scrScriptProcess, scrScriptShutdown, pw);
}

void dds3GlobalWorkClearFlags(void)
{
    s32 countdown;

    for (countdown = 255; countdown >= 0; countdown--) {
        DDS3_GBWK->integers[255 - countdown] = 0;
        DDS3_GBWK->floats[255 - countdown] = 0;
    }
}

dds3ProcessID_t *scrStartScriptByName(s32 priority, char *name, s32 messageHandle)
{
    return scrStartScriptProcess(priority, scrLoadScriptFile(name, messageHandle));
}

scrProcessWork_s *scrCreateProcessWithDefaultOption(char *name)
{
    return scrLoadScriptFile(name, 0);
}

dds3ProcessID_t *scrStartScript2(s32 priority, void *scrdata, s32 startproc)
{
    return scrStartScriptProcess(priority, fldFlowParse(scrdata, startproc));
}

scrProcessWork_s *scrCreateTaskWithDefaultOption(void *data)
{
    return fldFlowParse(data, 0);
}

dds3ProcessID_t *scrRunScriptEx(s32 priority, scrHeader_t *header, scrTypeTable_t *typeTable,
                    scrProcTable_t *procTable, scrLabelTable_t *labelTable, scrCode_u *code,
                    u8 *messageData, u8 *stringData, s32 procNo)
{
    return scrStartScriptProcess(priority, scrPrintProcedureStart(header, typeTable, procTable, labelTable,
                                                           code, messageData, stringData, procNo));
}

scrProcessWork_s *scrCreateProcessAtFirstProcedure(scrHeader_t *header, scrTypeTable_t *typeTable,
                             scrProcTable_t *procTable, scrLabelTable_t *labelTable,
                             scrCode_u *code, u8 *messageData, u8 *stringData)
{
    return scrPrintProcedureStart(header, typeTable, procTable, labelTable, code, messageData,
                                  stringData, 0);
}

void scrPrintProcedureEnd(scrProcessWork_s *pw)
{
    datDebugPrintf("end <%s>\n", pw->ProcTable[pw->StartProc].Name);

    if (pw->LocalInt != NULL) {
        sdfChipFreeDirect(pw->LocalInt);
    }
    if (pw->LocalFloat != NULL) {
        sdfChipFreeDirect(pw->LocalFloat);
    }
    if (pw->MesHandle >= 0) {
        itfMesMngRelease(pw->MesHandle);
    }
    if (pw->MemHandle != NULL) {
        sdfMemFreeDirect(pw->MemHandle);
    }

    scrUnlinkHandle(pw);
    sdfChipFreeDirect(pw);
}

/* DDS skips the running script's own process and does not print. */
void scrAllReleaseScript(void)
{
    scrProcessWork_s *pw;
    scrProcessWork_s *next;

    pw = scrHandleFirst;
    if (pw == NULL) {
        return;
    }
    while (1) {
        next = pw->next;
        if (scrIsCurrentWorkTask(pw) == 0) {
            if (pw->ProcessID != NULL) {
                dds3KillProcess(pw->ProcessID, 0);
            } else {
                scrPrintProcedureEnd(pw);
            }
        }
        pw = next;
        if (pw == NULL) {
            break;
        }
    }
}

/* Restart a procedure with an empty operand stack and fresh timers. */
s32 scrStartProcessProcedure(scrProcessWork_s *pw, s32 procNo)
{
    s32 cursor;

    if (procNo < 0 || procNo >= pw->TypeTable->ItemCount) {
        return 0;
    }
    for (cursor = 0; (((char *)pw->Name)[cursor] = pw->ProcTable[procNo].Name[cursor]) != 0; cursor++) {
    }
    pw->CP = pw->ProcTable[procNo].CodeAddress;
    pw->SP = 0;
    {
        scrStackData_u *stackValue = pw->StackData;
        u8 *stackType = pw->StackType;

        for (cursor = 27; cursor >= 0; cursor--) {
            *stackType++ = 0;
            stackValue++->i = 0;
        }
    }
    pw->StartProc = procNo;
    pw->ScriptTimer = 0;
    pw->CommandTimer = 0;
    return 1;
}

scrProcessWork_s *scrSearchScriptName(char *scriptname)
{
    scrProcessWork_s *pw;

    for (pw = scrHandleFirst; pw != NULL; pw = pw->next) {
        if (strcmp(scriptname, (char *)pw->Name) == 0) {
            return pw;
        }
    }
    return NULL;
}

s32 scrSearchProcedureName(void *scrdata, char *procname)
{
    scrHeader_t *header = scrdata;
    scrTypeTable_t *typeTable;
    scrProcTable_t *procTable = NULL;
    s32 i;
    s32 no;

    if (header == NULL) {
        return -1;
    }

    typeTable = (scrTypeTable_t *)(header + 1);
    for (i = 0; i < header->TypeTableCount; i++) {
        if (typeTable[i].Type == SCR_DATATYPE_PROCTABLE) {
            procTable = (scrProcTable_t *)((u8 *)header + typeTable[i].ItemAddress);
            break;
        }
    }

    if (procTable == NULL) {
        return -1;
    }

    for (no = 0; no < typeTable[i].ItemCount; no++) {
        if (strcmp((char *)procTable[no].Name, procname) == 0) {
            return no;
        }
    }
    return -1;
}

void scrSetUserData(dds3ProcessID_t *id, void *p)
{
    scrProcessWork_s *pw = dds3GetProcessData(id);

    pw->data = p;
}

void *scrGetUserData(dds3ProcessID_t *id)
{
    scrProcessWork_s *pw = dds3GetProcessData(id);

    return pw->data;
}

void scrScriptShutdown(dds3ProcessID_t *id)
{
    scrProcessWork_s *pw = dds3GetProcessData(id);

    if (pw != NULL) {
        scrPrintProcedureEnd(pw);
    }
    dds3SetProcessData(id, NULL);
}

s32 scrRunScript(scrProcessWork_s *h)
{
    return scrTraceCode(h);
}

s32 scrScriptProcess(dds3ProcessID_t *id)
{
    switch (scrTraceCode(dds3GetProcessData(id))) {
    case SCR_RESULT_ERROR:
        return -1;
    case SCR_RESULT_END:
        return -1;
    case SCR_RESULT_CONTINUE:
    default:
        return 0;
    }
}

/* DDS defines the command table in C; SMT3 includes it with scrTraceCode. */
#include "scr_command_table.h"

const scrCommandTable_t scrCommandTable[] = { SCR_COMMAND_TABLE_ENTRIES };
