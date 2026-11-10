#include "sdk_ps2.h"
#include "dat_state.h"
#include "common.h"
#include "sdf_chip.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "sdf_heap_api.h"
#include "scr.h"
#include "scr_process_task_owner_api.h"
#include "scr_procedure_start_api.h"
#include "scr_instruction_stream_api.h"
#include "scr_work_task_api.h"
#include "bf_flw0_header.h"
#include "sdf.h"
#include "kwln_task_lifecycle.h"
#include "scr_trace_result.h"
#include "kwln_task_runner.h"

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

dds3ProcessID_t *scrStartScriptProcess(u32 priority, ScrProcessTaskOwner process)
{
    dds3ProcessID_t *task;
    task = dds3StartProcess(process->procedures[process->procedureIndex].name,
                          priority, 1, 1, scrScriptProcess, scrScriptShutdown,
                          scrProcessTaskOwnerToUserValue(process));
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

scrProcessWork_s *scrCreateProcessWithDefaultOption(const char *path)
{
    return scrLoadScriptFile(path, 0);
}

dds3ProcessID_t *scrStartScript2(u32 priority, const BfFlw0Header *script, ScrProcedureIndex procedureIndex)
{
    return scrStartScriptProcess(priority, fldFlowParse(script, procedureIndex));
}

scrProcessWork_s *scrCreateTaskWithDefaultOption(const BfFlw0Header *header)
{
    return fldFlowParse(header, 0);
}

dds3ProcessID_t *scrRunScriptEx(u32 priority, const BfFlw0Header *header,
    const ScrSection *procedureSection, const ScrProcedure *procedures, const ScrLabel *labels,
    const scrCode_u *instructions, struct ItfMesSub *auxiliaryData, const char *strings, ScrProcedureIndex procedureIndex)
{
    return scrStartScriptProcess(priority, bfContextCreate(header, procedureSection, procedures, labels, instructions, auxiliaryData, strings, procedureIndex));
}

scrProcessWork_s *scrCreateProcessAtFirstProcedure(const BfFlw0Header *header, const ScrSection *procedureSection,
    const ScrProcedure *procedures, const ScrLabel *labels, const scrCode_u *instructions,
    struct ItfMesSub *auxiliaryData, const char *strings)
{
    return bfContextCreate(header, procedureSection, procedures, labels, instructions, auxiliaryData, strings, 0);
}

extern void evtPrintDeveloperConsoleMessage(const char *fmt, ...);

extern void itfMesMngRelease(ItfMesWindowIndex window);

extern void scrUnlinkHandle(scrProcessWork_s *process);

/* Log the process name, release its VM buffers and resource, then unlink and free it. */
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
        if (scrIsCurrentWorkTask(processNode) == SCR_WORK_TASK_NOT_CURRENT) {
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
ScrProcedureStartResult scrStartProcessProcedure(scrProcessWork_s *process,
    ScrProcedureIndex procedureIndex)
{
    s32 cursor;

    if (procedureIndex < 0 || procedureIndex >= process->procedureSection->count) {
        return SCR_PROCEDURE_START_INVALID_INDEX;
    }
    for (cursor = 0; (process->name[cursor] = process->procedures[procedureIndex].name[cursor]) != 0; cursor++) {
    }
    process->pc = process->procedures[procedureIndex].addr;
    process->sp = 0;
    {
        scrStackData_u *stackValue = process->stackValues;
        ScrStackValueType *stackType = process->stackTypes;

        for (cursor = SCR_STACK_RETURN_VALUE_SLOT; cursor >= 0; cursor--) {
            *stackType++ = 0;
            stackValue++->i = 0;
        }
    }
    process->procedureIndex = procedureIndex;
    process->timer = 0;
    process->cmdTimer = 0;
    return SCR_PROCEDURE_START_INITIALIZED;
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
    const ScrProcedure *procedures = NULL;
    const ScrSection *sections;
    s32 sectionIndex;
    ScrProcedureIndex procedureIndex;

    if (header == NULL) {
        return SCR_PROCEDURE_INDEX_NOT_FOUND;
    }
    sections = header->sections;
    for (sectionIndex = 0; sectionIndex < header->sectionCount; sectionIndex++) {
        if (sections[sectionIndex].type == BF_FLW0_SECTION_PROCEDURES) {
            procedures = (const ScrProcedure *)((const u8 *)header + sections[sectionIndex].offset);
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

void scrSetUserData(dds3ProcessID_t *task, void *userData) {
    scrProcessWork_s *context;

    context = (scrProcessWork_s *)dds3GetProcessData(task);
    context->userData = userData;
}

void *scrGetUserData(dds3ProcessID_t *task) {
    scrProcessWork_s *context;

    context = (scrProcessWork_s *)dds3GetProcessData(task);
    return context->userData;
}

void scrScriptShutdown(dds3ProcessID_t *task) {
    ScrProcessTaskOwner previousContext;

    previousContext = scrProcessTaskOwnerFromUserValue(dds3GetProcessData(task));
    if (previousContext != NULL) {
        scrProcDestroyTask(previousContext);
    }
    dds3SetProcessData(task, 0);
}

void scrRunScript(scrProcessWork_s *context) {
    scrTraceCode(context);
}

KwlnTaskUpdateResult scrScriptProcess(dds3ProcessID_t *task) {
    ScrProcessTaskOwner context = scrProcessTaskOwnerFromUserValue(dds3GetProcessData(task));

    switch (scrTraceCode(context)) {
    case 0:
        return KWLN_TASK_UPDATE_FINISH_CALLBACK;
    case SCR_TRACE_RESULT_FINISHED:
        return KWLN_TASK_UPDATE_FINISH_CALLBACK;
    case SCR_TRACE_RESULT_YIELDED:
    default:
        return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
    }
}

ScrCommandResult itfMesMngMessage(void);
ScrCommandResult itfMesMngOpenWindow(void);
ScrCommandResult itfMesMngCloseWindow(void);
ScrCommandResult itfMesMngSelect(void);
ScrCommandResult itfMesMngOpenSelectWindow(void);
ScrCommandResult itfMesMngCloseSelectWindow(void);
ScrCommandResult itfMesMngMaskSelect(void);
ScrCommandResult scrCommand_BIT_CHK(void);
ScrCommandResult scrCommand_BIT_ON(void);
ScrCommandResult scrCommand_BIT_OFF(void);
ScrCommandResult scrCommand_RND(void);
ScrCommandResult sndOpSetTrackDefaultVolumePan(void);
ScrCommandResult sndOpStartTrackFromScript(void);
ScrCommandResult scrCommand_SYNC(void);
ScrCommandResult scrCommand_WAIT(void);
ScrCommandResult bfWaitCbScreenFadeA(void);
ScrCommandResult bfWaitCbScreenFadeB(void);
ScrCommandResult scrCommand_SEL_JMP(void);
ScrCommandResult evtCommandAddEffectUnitToWorld(void);
ScrCommandResult evtCommand_CAM_PATH_MOVE(void);
ScrCommandResult evtCommand_SET_SKY(void);
ScrCommandResult evtCommand_MODEL_DSP(void);
ScrCommandResult evtCmdDestroySelectedWorldUnit(void);
ScrCommandResult evtOpStartUnitTransitionTowardWorldObject(void);
ScrCommandResult evtOpDeprecatedModelMotion(void);
ScrCommandResult evtCommand_CAM_SEL_FOVY(void);
ScrCommandResult scrCommand_PAD_ACT(void);
ScrCommandResult evtCommand_MODEL_SCL_XYZ(void);
ScrCommandResult scrCommand_SCR_EXISTS(void);
ScrCommandResult scrCommandFadeBackgroundOut(void);
ScrCommandResult scrCommandFadeBackgroundIn(void);
ScrCommandResult evtCommandStartPolygonMovie(void);
ScrCommandResult evtCommandClearPolygonMovieFlag(void);
ScrCommandResult evtCommand_FIELD_CLS_STOP(void);
ScrCommandResult scrCommandPrintInteger(void);
ScrCommandResult evtCommand_GET_MOON(void);
ScrCommandResult evtCommand_UNIT_CLS(void);
ScrCommandResult evtCommand_GET_MOON16(void);
ScrCommandResult evtCommand_RESTORE_BATTLE(void);
ScrCommandResult evtCommand_UNIT_CLS_STOP(void);
ScrCommandResult evtCommand_TEX_LOAD(void);
ScrCommandResult evtCommand_TEX_ON(void);
ScrCommandResult evtCommand_TEX_OFF(void);
ScrCommandResult evtCommand_TEX_CLS(void);
ScrCommandResult evtCommandSignalAdminAtTimerZero(void);
ScrCommandResult btlScriptSetBattleCommand(void);
ScrCommandResult btlScriptSelectEmptyCommand(void);
ScrCommandResult btlScriptSelectFixedCommand(void);
ScrCommandResult btlScriptSelectModeDValueOne(void);
ScrCommandResult btlScriptSelectActionEntry(void);
ScrCommandResult btlScriptChooseActionTargets(void);
ScrCommandResult btlScriptPickLowestHpTarget(void);
ScrCommandResult btlScriptPickTargetWithStatusMask(void);
ScrCommandResult btlScriptCmdSelectTargetsByMode(void);
ScrCommandResult btlCmdReportUnsupportedEvent(void);
ScrCommandResult btlCmdReturnPartyActionPresence(void);
ScrCommandResult btlCmdReturnEnemyActionPresence(void);
ScrCommandResult btlScriptSetBattleWorkParameter(void);
ScrCommandResult btlScriptChooseActionTargetsDuplicate(void);
ScrCommandResult btlCmdRunWeightedAiSelection(void);
ScrCommandResult btlScriptClearSceneTransition(void);
ScrCommandResult btlScriptBeginSceneTransition(void);
ScrCommandResult itfMesMng_SEL_SELNO(void);
ScrCommandResult scrCommand_BLUR2_ON(void);
ScrCommandResult scrCommand_BLUR2_OFF(void);
ScrCommandResult scrCommandResetDrawEffects(void);
ScrCommandResult evtCommand_MODEL_EXWAIT(void);
ScrCommandResult sndOpSendTitleAudioCommand(void);
ScrCommandResult evtCommand_RTRN_TITLE(void);
ScrCommandResult scrCommand_SOFTEN_BIAS(void);
ScrCommandResult evtCommand_SET_SKY_A(void);
ScrCommandResult evtCommand_MODEL_WAIT(void);
ScrCommandResult evtCommand_MODEL_POS(void);
ScrCommandResult evtCommand_MODEL_WAIT_MOT(void);
ScrCommandResult evtCommand_MODEL_MOVE_MOT(void);
ScrCommandResult evtCommand_FIELD_DSP(void);
ScrCommandResult evtCommand_FIELD_CLS(void);
ScrCommandResult itfCamp(void);
ScrCommandResult mnuScriptRequestMovieByIndex(void);
ScrCommandResult mnuScriptStopMovieAndResetDraw(void);
ScrCommandResult mnuUpdateMovieDrawShutdownCountdown(void);
ScrCommandResult scrCommand_PAD_TRIG(void);
ScrCommandResult scrCommand_PAD_PRESS(void);
ScrCommandResult evtCommand_CAM_PATH_WAIT(void);
ScrCommandResult evtCommand_MODEL_ROT(void);
ScrCommandResult evtOpTurnUnitRelativeToWorldNode(void);
ScrCommandResult evtOpRestoreUnitReferenceAngle(void);
ScrCommandResult evtCommand_CALL_SAVE(void);
ScrCommandResult evtCommand_CALL_FIELD(void);
ScrCommandResult fldCmdQueryActorEntrySceneStatus(void);
ScrCommandResult fldCmdTestActorEntryCondition(void);
ScrCommandResult fldCommand_PAD_DISABLE(void);
ScrCommandResult fldCommand_PAD_ENABLE(void);
ScrCommandResult mnuOpenTitleMenuDefaultMode(void);
ScrCommandResult evtCommand_MOON_START(void);
ScrCommandResult evtCommand_MOON_STOP(void);
ScrCommandResult evtCommand_CHG_MOON(void);
ScrCommandResult evtCommand_CALL_EVENT(void);
ScrCommandResult evtCommandSubmitEvent(void);
ScrCommandResult evtCommand_MODEL_PLAYER(void);
ScrCommandResult evtCommand_MODEL_ON(void);
ScrCommandResult evtCommand_MODEL_OFF(void);
ScrCommandResult evtCommand_OBJ_PATH_MOVE(void);
ScrCommandResult evtCommand_OBJ_PATH_WAIT(void);
ScrCommandResult scrCommand_SET_FOVY(void);
ScrCommandResult fldCommand_ENCOUNT_ON(void);
ScrCommandResult fldCommand_ENCOUNT_OFF(void);
ScrCommandResult scrCommandAdjustItemQuantity(void);
ScrCommandResult itfMesMngSetVarScr(void);
ScrCommandResult scrCommandPrintString(void);
ScrCommandResult evtOpSetUnitParams5(void);
ScrCommandResult evtCommand_MODEL_WAIT_HOKAN(void);
ScrCommandResult evtCommand_CAM_CREATE(void);
ScrCommandResult evtCommand_CAM_CLS(void);
ScrCommandResult evtCommand_CAM_POS_XYZ(void);
ScrCommandResult evtCommand_CAM_ROT_XYZ(void);
ScrCommandResult evtCommand_MODEL_POS_XYZ(void);
ScrCommandResult evtCommand_MODEL_ROT_XYZ(void);
ScrCommandResult btlNbScriptCheckActorFlag(void);
ScrCommandResult btlCmdStoreBossHealthCheckChoice(void);
ScrCommandResult btlScriptPickLowestHpRatioTarget(void);
ScrCommandResult btlCmdStoreEnemyCountCheckChoice(void);
ScrCommandResult btlCmdStorePartyCountCheckChoice(void);
ScrCommandResult btlCmdStoreActorActionMaskChoice(void);
ScrCommandResult btlCmdStoreEnemyActionMaskChoice(void);
ScrCommandResult btlCmdStorePartyActionMaskChoice(void);
ScrCommandResult btlCmdRememberAllLowerActionBits(void);
ScrCommandResult btlCmdRememberLowerUnitMode(void);
ScrCommandResult btlCmdRememberOtherUpperUnitMode(void);
ScrCommandResult btlCmdRememberLowerEntryCondition(void);
ScrCommandResult btlCmdRememberUpperEntryCondition(void);
ScrCommandResult btlAiCheckBa(void);
ScrCommandResult btlCmdReturnPartyElementQueryCheck(void);
ScrCommandResult btlCmdReturnEnemyElementQueryCheck(void);
ScrCommandResult evtCommand_MODEL_MOVE2(void);
ScrCommandResult btlCmdRememberActorActionGate(void);
ScrCommandResult evtCommand_LOAD_SOUND_E(void);
ScrCommandResult evtCommand_BGM_PLAY_E(void);
ScrCommandResult evtOpcodeSetBgmVolumePan(void);
ScrCommandResult evtCommand_FADEOUT_E(void);
ScrCommandResult evtCommand_BGM_VOL_DOWN(void);
ScrCommandResult evtCommand_BGM_VOL_UP(void);
ScrCommandResult itfMesMngIgnoreConfig(void);
ScrCommandResult evtCommand_SEARCH_ID(void);
ScrCommandResult evtOpSetUnitParams6(void);
ScrCommandResult evtCommand_MODEL_WAIT_EXMOT(void);
ScrCommandResult evtCommand_CALL_NEXT(void);
ScrCommandResult evtCommand_CALL_FIELD2(void);
ScrCommandResult scrCommandResetFieldEffects(void);
ScrCommandResult evtCommand_EFFECT_LOAD(void);
ScrCommandResult evtCommand_EFFECT_CLS(void);
ScrCommandResult evtCommand_EFFECT_POS_XYZ(void);
ScrCommandResult evtCommand_EFFECT_POS(void);
ScrCommandResult evtCommand_EFFECT_ON(void);
ScrCommandResult evtCommand_EFFECT_OFF(void);
ScrCommandResult evtCommand_EFFECT_LINK(void);
ScrCommandResult evtCommand_EFFECT_LINK2(void);
ScrCommandResult evtCommand_EFFECT_UNLINK(void);
ScrCommandResult fldCmdFocusCameraOnObject(void);
ScrCommandResult fldUpdateLookAtSegment(void);
ScrCommandResult bfWaitCbCreateTask(void);
ScrCommandResult scrCommand_SCR_KILL(void);
ScrCommandResult scrCommand_SCR_WAIT(void);
ScrCommandResult scrCommand_SCR_STAT(void);
ScrCommandResult scrCommand_SCR_TIME(void);
ScrCommandResult evtCommand_PMV_RUN(void);
ScrCommandResult scrCommand_QUAKE_ON(void);
ScrCommandResult scrCommand_QUAKE_OFF(void);
ScrCommandResult evtCommand_MODEL_LIGHT_OFF(void);
ScrCommandResult evtCommand_MODEL_LIGHT_ON(void);
ScrCommandResult evtCommand_MODEL_LIGHT_RGB(void);
ScrCommandResult evtCommand_MODEL_LIGHT_DIR(void);
ScrCommandResult evtCommand_MODEL_LIGHT_WAIT(void);
ScrCommandResult evtCommand_MODEL_RGB(void);
ScrCommandResult evtCommand_MODEL_A(void);
ScrCommandResult scrCommand_SCENE_LIGHT_RGB(void);
ScrCommandResult scrCommand_SCENE_LIGHT_DIR(void);
ScrCommandResult scrCommand_SCENE_AMBIENT(void);
ScrCommandResult evtCommand_PATH_STOP(void);
ScrCommandResult evtCommand_PATH_START(void);
ScrCommandResult evtCommand_PATH_RATE(void);
ScrCommandResult evtCommand_PATH_DIR(void);
ScrCommandResult scrCommand_SCENE_FOG_RGB(void);
ScrCommandResult scrCommand_SCENE_FOG_PARA(void);
ScrCommandResult scrCommand_BLUR(void);
ScrCommandResult func_0014DAF8(void);
ScrCommandResult fldCmdFreeDisplayObjects(void);
ScrCommandResult fldCmdSetScenePhaseThree(void);
ScrCommandResult fldCmdSetLookAtHeading(void);
ScrCommandResult fldCommand_CAM_SEL_PHOKAN(void);
ScrCommandResult fldCmdSetSolarOverlayMode(void);
ScrCommandResult evtCommand_MODEL_LIGHT_LINK(void);
ScrCommandResult evtCommand_MODEL_LIGHT_UNLINK(void);
ScrCommandResult evtCommand_LIGHT_PATH_MOVE(void);
ScrCommandResult evtCommand_LIGHT_PATH_WAIT(void);
ScrCommandResult evtCommand_LOAD_BE(void);
ScrCommandResult evtCommand_MODEL_BE(void);
ScrCommandResult evtCommand_FIELD_BE(void);
ScrCommandResult evtCommand_EFFECT_BE(void);
ScrCommandResult evtCommand_TEX_BE(void);
ScrCommandResult evtCommand_EFFBED_LOAD(void);
ScrCommandResult evtCommand_EFFBED_START(void);
ScrCommandResult evtCommand_EFFECT_ROT_XYZ(void);
ScrCommandResult evtCommand_EFFBED_BE(void);
ScrCommandResult evtCommand_EFFECT_PATH_MOVE(void);
ScrCommandResult evtCommand_EFFECT_PATH_WAIT(void);
ScrCommandResult evtCommand_EFFMG1_LOAD(void);
ScrCommandResult evtCommand_EFFMG1_BE(void);
ScrCommandResult evtCommand_EFFMG2_LOAD(void);
ScrCommandResult evtCommand_EFFMG2_BE(void);
ScrCommandResult evtCommand_EFFMG1_POS(void);
ScrCommandResult evtCommand_EFFMG2_POS(void);
ScrCommandResult evtCommandStartBgmWithFade(void);
ScrCommandResult evtCommand_CAM_ROT_Q(void);
ScrCommandResult evtCommand_FIELD_ANIME_START(void);
ScrCommandResult evtCommand_FIELD_ANIME_STOP(void);
ScrCommandResult evtCommand_MODEL_SHADOW_ON2(void);
ScrCommandResult btlCmdRememberCounterLimit(void);
ScrCommandResult btlCmdStoreCounterReachedLimitChoice(void);
ScrCommandResult btlScriptReturnTurnCount(void);
ScrCommandResult btlScriptSetCommandValueAndParameter(void);
ScrCommandResult evtCommand_EFFMG2_PRESET(void);
ScrCommandResult btlCmdReportUnsupportedTaikyo(void);
ScrCommandResult btlCmdReportUnsupportedAllTaikyo(void);
ScrCommandResult btlResetCommandContextAndSetStateFlag(void);
ScrCommandResult btlScriptPickLowestHpBlockTarget(void);
ScrCommandResult evtCommand_PATH_LOOP_ON(void);
ScrCommandResult evtCommand_PATH_LOOP_OFF(void);
ScrCommandResult scrCommand_MUL_BLUR_PARA(void);
ScrCommandResult scrCommand_MUL_BLUR_RGBA(void);
ScrCommandResult scrCommand_MUL_BLUR_AREA(void);
ScrCommandResult scrCommand_MUL_BLUR_ON(void);
ScrCommandResult scrCommand_MUL_BLUR_OFF(void);
ScrCommandResult scrCommand_DIST_BLUR_PARA(void);
ScrCommandResult scrCommand_DIST_BLUR_RGBA(void);
ScrCommandResult scrCommand_DIST_BLUR_AREA(void);
ScrCommandResult scrCommand_DIST_BLUR_ON(void);
ScrCommandResult scrCommand_DIST_BLUR_OFF(void);
ScrCommandResult btlCmdCameraMove(void);
ScrCommandResult btlCmdReportUnsupportedCameraOrigin(void);
ScrCommandResult btlCmdSetContextFlagOne(void);
ScrCommandResult evtCommand_READ_BE(void);
ScrCommandResult evtCommand_CHK_BE(void);
ScrCommandResult evtCommand_DEL_BE(void);
ScrCommandResult btlScriptQueueActorCommandSound(void);
ScrCommandResult btlCmdReportUnsupportedCameraTwoShot(void);
ScrCommandResult btlCmdReportUnsupportedCameraBougai(void);
ScrCommandResult fldCommand_FLD_CHK_BTL(void);
ScrCommandResult fldCommand_FLD_CHK_ANA(void);
ScrCommandResult fldCommand_FLD_CHK_SISETU(void);
ScrCommandResult evtCommand_CALL_FIELD3(void);
ScrCommandResult fldCommand_FLD_KANKYO(void);
ScrCommandResult fldCommand_FLD_FILTER(void);
ScrCommandResult fldCommand_FLD_MODEL_ON(void);
ScrCommandResult fldCmdApplyRoomModeGroupOne(void);
ScrCommandResult fldCommand_FLD_ANIME_ON(void);
ScrCommandResult fldCommand_FLD_ANIME_OFF(void);
ScrCommandResult fldCommand_FLD_ATARI_ON(void);
ScrCommandResult fldCmdSetSceneBitsBarrierRoom(void);
ScrCommandResult fldCommand_FLD_MODEL_ON2(void);
ScrCommandResult fldCommand_FLD_MODEL_OFF2(void);
ScrCommandResult fldCommand_FLD_ANIME_ON2(void);
ScrCommandResult fldCommand_FLD_ANIME_OFF2(void);
ScrCommandResult fldCommand_FLD_ANIME_ID2(void);
ScrCommandResult fldOpClearRoomStateFlags(void);
ScrCommandResult fldOpSetRoomStateFlags(void);
ScrCommandResult fldCmdSetSceneBitsValue(void);
ScrCommandResult fldCommand_FLD_MAP_ON(void);
ScrCommandResult fldCommand_FLD_MAP_OFF(void);
ScrCommandResult fldCommand_SET_CAMTBL(void);
ScrCommandResult fldCmdPushSceneParam(void);
ScrCommandResult fldCommand_FLD_SET_TAKARA_OPEN(void);
ScrCommandResult fldCommand_FLD_CHK_TAKARA_OPEN(void);
ScrCommandResult itfMesMngSetMesPositionScript(void);
ScrCommandResult itfMesMngSetMesPosDefault(void);
ScrCommandResult itfMesMngValidateNext(void);
ScrCommandResult evtCommandSubmitScalarAdminRequest(void);
ScrCommandResult movCheckStartupSoundState(void);
ScrCommandResult sndOpUpdateTitleBgmState(void);
ScrCommandResult evtOpSetActiveEntryFlag(void);
ScrCommandResult evtOpReadDisplayValue(void);
ScrCommandResult mnuResetTitlePlaybackCommands(void);
ScrCommandResult mnuAdvanceTitleStreamFromScript(void);
ScrCommandResult mnuIsTitleStreamIdle(void);
ScrCommandResult evtCommand_MODEL_SHADOW_ON(void);
ScrCommandResult evtCommand_MODEL_SHADOW_OFF(void);
ScrCommandResult evtCommand_BOKASI_ON(void);
ScrCommandResult evtCommand_BOKASI_OFF(void);
ScrCommandResult scrCommand_MUL_BLUR_SWAP(void);
ScrCommandResult scrCommand_DIST_BLUR_SWAP(void);
ScrCommandResult scrCommand_BLUR2_PARA(void);
ScrCommandResult scrCommand_BLUR2_RGBA(void);
ScrCommandResult scrCommand_BLUR2_AREA(void);
ScrCommandResult scrCommand_BLUR2_SWAP(void);
ScrCommandResult scrCommand_FILTER2_PARA(void);
ScrCommandResult scrCommand_FILTER2_RGBA(void);
ScrCommandResult scrCommand_FILTER2_SWAP(void);
ScrCommandResult scrCommand_FILTER2_ON(void);
ScrCommandResult scrCommand_FILTER2_OFF(void);
ScrCommandResult scrCommand_MFILTER_PARA(void);
ScrCommandResult scrCommand_MFILTER_RGBA(void);
ScrCommandResult scrCommand_MFILTER_AREA(void);
ScrCommandResult scrCommand_MFILTER_SWAP(void);
ScrCommandResult scrCommand_MFILTER_ON(void);
ScrCommandResult scrCommand_MFILTER_OFF(void);
ScrCommandResult evtCommand_BGM_TRANS_E(void);
ScrCommandResult scrCommandAddPartyCurrency(void);
ScrCommandResult ptyScriptApplyEventPenalty(void);
ScrCommandResult itfMesMngWindowType(void);
ScrCommandResult itfMesMngSelectDefKey(void);
ScrCommandResult ptyScriptRestoreEntireParty(void);
ScrCommandResult scrCommand_RIP_BLUR_ON(void);
ScrCommandResult scrCommand_RIP_BLUR_OFF(void);
ScrCommandResult scrCommand_RIP_BLUR_PARA(void);
ScrCommandResult scrCommand_RIP_BLUR_RGBA(void);
ScrCommandResult scrCommand_RIP_BLUR_AREA(void);
ScrCommandResult scrCommand_RIP_BLUR_SWAP(void);
ScrCommandResult evtOpcodeRefreshTaskData(void);
ScrCommandResult evtOpcodeInitializeEffectSoundChannel(void);
ScrCommandResult evtCommand_EFFECT_PRESET(void);
ScrCommandResult btlCmdStoreActorMpCheckChoice(void);
ScrCommandResult btlScriptReturnContextSignedByte(void);
ScrCommandResult btlAiGetAllKoudo(void);
ScrCommandResult btlScriptReturnActorLevel(void);
ScrCommandResult btlScriptFlagAllUnitsDefeatCandidates(void);
ScrCommandResult btlScriptFlagPlayerDefeatCandidates(void);
ScrCommandResult btlScriptFlagEnemyDefeatCandidates(void);
ScrCommandResult btlScriptBuiltin342NoOp(void);
ScrCommandResult evtCommand_CALL_STAFF(void);
ScrCommandResult btlScriptSelectModeAValueOne(void);
ScrCommandResult btlScriptPickLowestLevelTarget(void);
ScrCommandResult ptyScriptGetRosterStat(void);
ScrCommandResult btlAiGetFurea(void);
ScrCommandResult evtCommand_CALL_ENDSAVE(void);
ScrCommandResult evtCommand_KUTI(void);
ScrCommandResult itfMesMngWindowTypeAlpha(void);
ScrCommandResult evtCommand_KUTISET(void);
ScrCommandResult evtCommand_CALL_BATTLE2(void);
ScrCommandResult evtCommand_EFFBED_BE_EVO(void);
ScrCommandResult evtCommand_EFFBED_START_EVO(void);
ScrCommandResult btlCmdReturnLowHpActionReadiness(void);
ScrCommandResult evtCommandSetDrawFlagWhileWaiting(void);
ScrCommandResult mnuCanDrawMovie(void);
ScrCommandResult evtStoreScriptParameterResult(void);
ScrCommandResult evtPushFirstRosterLevel(void);
ScrCommandResult evtPushSecondRosterLevel(void);
ScrCommandResult evtPushFirstRosterStatEligibility(void);
ScrCommandResult evtPushSecondRosterStatEligibility(void);
ScrCommandResult evtPushSelectedStatOrRosterLowValue(void);
ScrCommandResult evtPushSelectedScaledStat(void);
ScrCommandResult evtSelectScriptStatValue(void);
ScrCommandResult evtPushSelectedTotalOrRosterHighValue(void);
ScrCommandResult evtPushEntryFlagBitInverted(void);
ScrCommandResult evtPushFirstRosterSelectedStat(void);
ScrCommandResult evtPushSecondRosterSelectedStat(void);
ScrCommandResult evtPushFirstRosterOptionStat(void);
ScrCommandResult evtPushSecondRosterOptionStat(void);
ScrCommandResult evtRollRandomScale(void);
ScrCommandResult evtPushScriptContextResult(void);
ScrCommandResult evtPushSelectedStatMaximum(void);
ScrCommandResult scrGetBattleAverageActorStatForGroup(void);
ScrCommandResult evtPushAvailableChoiceRatio(void);
ScrCommandResult evtPushAvailableFloatValue(void);
ScrCommandResult evtPushAvailableIntegerValue(void);
ScrCommandResult evtPushFirstRosterCurrentHp(void);
ScrCommandResult evtPushSecondRosterCurrentHp(void);
ScrCommandResult evtPushFirstRosterMaximumHp(void);
ScrCommandResult evtPushSecondRosterMaximumHp(void);
ScrCommandResult evtCommandReattachObjectPath(void);
ScrCommandResult evtPushLevelMaxHpGrowth(void);
ScrCommandResult evtPushLevelMaxMpGrowth(void);
ScrCommandResult evtSelectStatGrade(void);
ScrCommandResult evtPushLevelValueA(void);
ScrCommandResult evtPushLevelValueB(void);
ScrCommandResult evtScriptSelectRandomValue(void);
ScrCommandResult evtPushSelectedStatGrade(void);
ScrCommandResult evtPushLevelValueCCommand18D(void);
ScrCommandResult evtPushLevelValueCCommand18E(void);
ScrCommandResult evtPushRequestedRosterPresence(void);
ScrCommandResult ptyScriptAddUnitAndReturnResult(void);
ScrCommandResult ptyScriptRemoveUnitAndReturnResult(void);
ScrCommandResult ptyScriptSetZeroResultCommand192(void);
ScrCommandResult ptyScriptSetOneResultCommand193(void);
ScrCommandResult ptyScriptSetOneResultCommand194(void);
ScrCommandResult evtPushRosterBaseValue(void);
ScrCommandResult btlCmdReturnActorEligibility(void);
ScrCommandResult btlCommandSelectEventAction(void);
ScrCommandResult btlCommandWaitEventSoundDeadline(void);
ScrCommandResult btlCmdReportEnemySpawnCapacity(void);
ScrCommandResult btlCmdRememberLowerElementAccess(void);
ScrCommandResult btlCmdRememberUpperElementAccess(void);
ScrCommandResult btlCmdRememberLowerElementExclusion(void);
ScrCommandResult btlCmdRememberUpperElementExclusion(void);
ScrCommandResult btlCmdRememberUpperActionAvailability(void);
ScrCommandResult btlCmdRememberActorEntryCondition(void);
ScrCommandResult btlCmdRememberNonLowerStatFlag(void);
ScrCommandResult btlOpStartSelectedUnitModelChange(void);
ScrCommandResult btlCmdReturnReadyWithoutTurnsCheck(void);
ScrCommandResult btlCmdRememberContextSecondaryFlag(void);
ScrCommandResult btlAreModelChangeTasksFinished(void);
ScrCommandResult evtSelectFineStatGrade(void);
ScrCommandResult btlCommandReportSpecialModeValue(void);
ScrCommandResult btlCmdStoreDefaultUnitActionChoice(void);
ScrCommandResult btlCmdStoreAlternateUnitActionChoice(void);
ScrCommandResult btlCmdStorePartyEntryCheckChoice(void);
ScrCommandResult btlCmdStoreInversePartyEntryChoice(void);
ScrCommandResult btlCmdRememberUpperEntryCodeMatch(void);
ScrCommandResult btlCmdRememberUpperEntryCodeInverse(void);
ScrCommandResult btlCmdRememberUpperSelectedElement(void);
ScrCommandResult btlCmdRememberLowerMissingStatus(void);
ScrCommandResult btlScriptPickUnlinkedPartyTarget(void);
ScrCommandResult btlScriptAppendActorToTargets(void);
ScrCommandResult btlScriptPickTargetExcludingActor(void);
ScrCommandResult btlCommandStartSlotMotion(void);
ScrCommandResult btlCommandReportBossEffectOption(void);
ScrCommandResult btlCmdCheckHpPercent(void);
ScrCommandResult btlScriptReturnSelectedBossEffectId(void);
ScrCommandResult btlScriptReturnEffectActive(void);
ScrCommandResult btlCmdReturnBattlePhase(void);
ScrCommandResult btlCmdRememberLowerEntryCodeMismatch(void);
ScrCommandResult btlCmdRememberUpperEntryCodeMismatch(void);
ScrCommandResult btlScriptPickTargetWithoutStatusMask(void);
ScrCommandResult btlCmdRememberPartyQueuedActionCategory(void);
ScrCommandResult btlCmdTestEffectActor(void);
ScrCommandResult btlScriptReturnEffectValue(void);
ScrCommandResult btlScriptReturnSceneSlotCount(void);
ScrCommandResult btlCmdReturnPartyMissingFlagCheck(void);
ScrCommandResult btlCmdReturnPartyHeldFlagCheck(void);
ScrCommandResult btlScriptPickLinkedPartyTarget(void);
ScrCommandResult btlScriptReturnFirstSpecialEnemySpecies(void);
ScrCommandResult btlCommandReportEventEffectValue(void);
ScrCommandResult btlScriptAddEffectActorTarget(void);
ScrCommandResult btlScriptReturnGlobalDebugValue(void);
ScrCommandResult btlCmdRememberLowerElementBlock(void);
ScrCommandResult btlScriptPickElementBlockTarget(void);
ScrCommandResult btlCmdRememberUpperQueuedQuery(void);
ScrCommandResult btlCmdRememberLowerQueuedQuery(void);
ScrCommandResult btlScriptSetActorUnitParameter(void);
ScrCommandResult btlScriptPickHighestMpTarget(void);
ScrCommandResult btlScriptPickElementCheckedTarget(void);
ScrCommandResult btlCmdReportPartyEmptyMp(void);
ScrCommandResult btlCommandSetSequenceVolumePan(void);
ScrCommandResult btlCommandReportSpecialEnemyLink(void);
ScrCommandResult evtPushEntryIndexedStatOption(void);
ScrCommandResult btlCmdClearSpecialEnemyFlags(void);
ScrCommandResult scrGetBattleAverageMaximumValueForGroup(void);
ScrCommandResult scrGetBattleAverageCurrentValueForGroup(void);
ScrCommandResult btlScriptQueueUnboundCommandSound(void);
ScrCommandResult btlScriptSetCameraBlendStart(void);
ScrCommandResult btlScriptSetCameraBlendEnd(void);
ScrCommandResult btlCmdCameraMoveBlend(void);
ScrCommandResult evtOpQueueWorldObjectPendingValue(void);
ScrCommandResult evtOpClearWorldObjectPendingValue(void);
ScrCommandResult evtOpModelLodChg(void);
ScrCommandResult itfCommandSelectOptionWithFade(void);
ScrCommandResult itfPanelAcquireHold(void);
ScrCommandResult itfCommandQueueHeldPanelEntry(void);
ScrCommandResult itfStepHeldPanelMessages(void);
ScrCommandResult scrCommandClearProcessControlFlag(void);
ScrCommandResult scrCommandSetProcessControlFlag(void);
ScrCommandResult fldCommandStartTitle(void);
ScrCommandResult fldCommand_FLD_GET_KIRETA(void);
ScrCommandResult fldCmdRollMirroredSolarThreshold(void);
ScrCommandResult fldCommand_FLD_MSE_PLAY2(void);
ScrCommandResult fldCommand_FLD_MSE_STOP2(void);
ScrCommandResult fldCommand_SET_NPC_CAMERA(void);
ScrCommandResult fldCmdMarkBitmapRegion(void);
ScrCommandResult fldCommandSendNamedSound(void);
ScrCommandResult fldCommandSendSoundControl(void);
ScrCommandResult fldCommandIsSoundBusy(void);
ScrCommandResult fldCmdGetActorStat1(void);
ScrCommandResult fldCommand_FLD_WARP_HASIGO_GETTBL(void);
ScrCommandResult fldCmdGetCurrentSceneSelectionResource(void);
ScrCommandResult fldCommandFindEffectByName(void);
ScrCommandResult fldCommand_FLD_WARP_ELV_GETTBL(void);
ScrCommandResult fldCommand_FLD_WARP_ELV_EXIT(void);
ScrCommandResult fldCmdSetCurrentTaskScene(void);
ScrCommandResult func_0014EF20(void);
ScrCommandResult fldCommand_FLD_WARP_RUN_START(void);
ScrCommandResult fldCommand_FLD_INF_NEXT(void);
ScrCommandResult fldCommand_FLD_INF_GET(void);
ScrCommandResult fldCommand_FLD_INF_GETPANEL(void);
ScrCommandResult fldCmdGetActorStat0(void);
ScrCommandResult fldCommand_FLD_ELV_FLOORSTART(void);
ScrCommandResult fldCommand_FLD_ELV_FLOOREND(void);
ScrCommandResult fldCommand_FLD_ELV_ENDCHK(void);
ScrCommandResult fldCommand_FLD_WARP_GET_DOOR(void);
ScrCommandResult fldCommand_FLD_EXIT_CAMERA(void);
ScrCommandResult fldCommand_FLD_INF_GOWAP(void);
ScrCommandResult fldCmdDispatchSceneAction(void);
ScrCommandResult fldCmdStartSceneBgm(void);
ScrCommandResult fldCmdStartSceneBgmAlternate(void);
ScrCommandResult fldCmdStopCurrentBgm(void);
ScrCommandResult fldCmdPlayCurrentBgmSound(void);
ScrCommandResult fldCommandPlaySeVolumePan(void);
ScrCommandResult fldCommandPlaySe(void);
ScrCommandResult fldCmdReleaseCurrentBgm(void);
ScrCommandResult fldCommandLoadArchive(void);
ScrCommandResult fldCommand_FLD_WARP_GET_WARPEFF(void);
ScrCommandResult fldCmdQueryAlternateActorEntrySceneStatus(void);
ScrCommandResult fldCommand_FLD_SET_ENCOUNT(void);
ScrCommandResult fldCommand_ACT_WIN_DSP3(void);
ScrCommandResult fldCmdInitializeHuntSparkUnit(void);
ScrCommandResult fldCmdSetStateFlag(void);

const ScrCommand D_0039E288[] = {
    { itfMesMngMessage, 1 },
    { itfMesMngOpenWindow, 0 },
    { itfMesMngCloseWindow, 0 },
    { itfMesMngSelect, 1 },
    { itfMesMngOpenSelectWindow, 0 },
    { itfMesMngCloseSelectWindow, 0 },
    { itfMesMngMaskSelect, 1 },
    { scrCommand_BIT_CHK, 1 },
    { scrCommand_BIT_ON, 1 },
    { scrCommand_BIT_OFF, 1 },
    { scrCommand_RND, 1 },
    { sndOpSetTrackDefaultVolumePan, 1 },
    { sndOpStartTrackFromScript, 1 },
    { scrCommand_SYNC, 0 },
    { scrCommand_WAIT, 1 },
    { bfWaitCbScreenFadeA, 2 },
    { bfWaitCbScreenFadeB, 2 },
    { scrCommand_SEL_JMP, 8 },
    { evtCommandAddEffectUnitToWorld, 1 },
    { evtCommand_CAM_PATH_MOVE, 2 },
    { evtCommand_SET_SKY, 3 },
    { evtCommand_MODEL_DSP, 2 },
    { evtCmdDestroySelectedWorldUnit, 1 },
    { evtOpStartUnitTransitionTowardWorldObject, 5 },
    { evtOpDeprecatedModelMotion, 4 },
    { evtCommand_CAM_SEL_FOVY, 1 },
    { scrCommand_PAD_ACT, 3 },
    { evtCommand_MODEL_SCL_XYZ, 4 },
    { scrCommand_SCR_EXISTS, 1 },
    { NULL, 0 },
    { scrCommandFadeBackgroundOut, 1 },
    { scrCommandFadeBackgroundIn, 1 },
    { NULL, 0 },
    { evtCommandStartPolygonMovie, 2 },
    { evtCommandClearPolygonMovieFlag, 1 },
    { evtCommand_FIELD_CLS_STOP, 0 },
    { scrCommandPrintInteger, 1 },
    { evtCommand_GET_MOON, 0 },
    { evtCommand_UNIT_CLS, 0 },
    { evtCommand_GET_MOON16, 0 },
    { evtCommand_RESTORE_BATTLE, 2 },
    { evtCommand_UNIT_CLS_STOP, 0 },
    { evtCommand_TEX_LOAD, 1 },
    { evtCommand_TEX_ON, 1 },
    { evtCommand_TEX_OFF, 1 },
    { evtCommand_TEX_CLS, 1 },
    { evtCommandSignalAdminAtTimerZero, 0 },
    { btlScriptSetBattleCommand, 1 },
    { btlScriptSelectEmptyCommand, 0 },
    { btlScriptSelectFixedCommand, 0 },
    { btlScriptSelectModeDValueOne, 0 },
    { btlScriptSelectActionEntry, 1 },
    { btlScriptChooseActionTargets, 0 },
    { btlScriptPickLowestHpTarget, 0 },
    { btlScriptPickTargetWithStatusMask, 1 },
    { btlScriptCmdSelectTargetsByMode, 1 },
    { btlCmdReportUnsupportedEvent, 1 },
    { btlCmdReturnPartyActionPresence, 1 },
    { btlCmdReturnEnemyActionPresence, 1 },
    { btlScriptSetBattleWorkParameter, 1 },
    { btlScriptChooseActionTargetsDuplicate, 0 },
    { btlCmdRunWeightedAiSelection, 0 },
    { btlScriptClearSceneTransition, 0 },
    { btlScriptBeginSceneTransition, 0 },
    { itfMesMng_SEL_SELNO, 1 },
    { scrCommand_BLUR2_ON, 1 },
    { scrCommand_BLUR2_OFF, 1 },
    { scrCommandResetDrawEffects, 0 },
    { evtCommand_MODEL_EXWAIT, 2 },
    { sndOpSendTitleAudioCommand, 0 },
    { evtCommand_RTRN_TITLE, 0 },
    { scrCommand_SOFTEN_BIAS, 3 },
    { evtCommand_SET_SKY_A, 2 },
    { evtCommand_MODEL_WAIT, 1 },
    { evtCommand_MODEL_POS, 2 },
    { evtCommand_MODEL_WAIT_MOT, 2 },
    { evtCommand_MODEL_MOVE_MOT, 2 },
    { evtCommand_FIELD_DSP, 2 },
    { evtCommand_FIELD_CLS, 0 },
    { itfCamp, 0 },
    { mnuScriptRequestMovieByIndex, 1 },
    { mnuScriptStopMovieAndResetDraw, 0 },
    { mnuUpdateMovieDrawShutdownCountdown, 0 },
    { scrCommand_PAD_TRIG, 1 },
    { scrCommand_PAD_PRESS, 1 },
    { evtCommand_CAM_PATH_WAIT, 1 },
    { evtCommand_MODEL_ROT, 3 },
    { evtOpTurnUnitRelativeToWorldNode, 3 },
    { evtOpRestoreUnitReferenceAngle, 2 },
    { evtCommand_CALL_SAVE, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { evtCommand_CALL_FIELD, 3 },
    { fldCmdQueryActorEntrySceneStatus, 1 },
    { fldCmdTestActorEntryCondition, 1 },
    { fldCommand_PAD_DISABLE, 0 },
    { fldCommand_PAD_ENABLE, 0 },
    { mnuOpenTitleMenuDefaultMode, 0 },
    { evtCommand_MOON_START, 0 },
    { evtCommand_MOON_STOP, 0 },
    { evtCommand_CHG_MOON, 1 },
    { evtCommand_CALL_EVENT, 1 },
    { evtCommandSubmitEvent, 1 },
    { evtCommand_MODEL_PLAYER, 0 },
    { evtCommand_MODEL_ON, 1 },
    { evtCommand_MODEL_OFF, 1 },
    { evtCommand_OBJ_PATH_MOVE, 3 },
    { evtCommand_OBJ_PATH_WAIT, 1 },
    { scrCommand_SET_FOVY, 1 },
    { fldCommand_ENCOUNT_ON, 0 },
    { fldCommand_ENCOUNT_OFF, 0 },
    { scrCommandAdjustItemQuantity, 2 },
    { itfMesMngSetVarScr, 3 },
    { scrCommandPrintString, 1 },
    { evtOpSetUnitParams5, 5 },
    { evtCommand_MODEL_WAIT_HOKAN, 3 },
    { evtCommand_CAM_CREATE, 0 },
    { evtCommand_CAM_CLS, 1 },
    { evtCommand_CAM_POS_XYZ, 4 },
    { evtCommand_CAM_ROT_XYZ, 4 },
    { evtCommand_MODEL_POS_XYZ, 4 },
    { evtCommand_MODEL_ROT_XYZ, 4 },
    { btlNbScriptCheckActorFlag, 1 },
    { btlCmdStoreBossHealthCheckChoice, 1 },
    { btlScriptPickLowestHpRatioTarget, 0 },
    { btlCmdStoreEnemyCountCheckChoice, 1 },
    { btlCmdStorePartyCountCheckChoice, 1 },
    { btlCmdStoreActorActionMaskChoice, 1 },
    { btlCmdStoreEnemyActionMaskChoice, 1 },
    { btlCmdStorePartyActionMaskChoice, 1 },
    { btlCmdRememberAllLowerActionBits, 1 },
    { btlCmdRememberLowerUnitMode, 1 },
    { btlCmdRememberOtherUpperUnitMode, 1 },
    { btlCmdRememberLowerEntryCondition, 1 },
    { btlCmdRememberUpperEntryCondition, 1 },
    { btlAiCheckBa, 0 },
    { btlCmdReturnPartyElementQueryCheck, 1 },
    { btlCmdReturnEnemyElementQueryCheck, 1 },
    { evtCommand_MODEL_MOVE2, 7 },
    { btlCmdRememberActorActionGate, 0 },
    { evtCommand_LOAD_SOUND_E, 1 },
    { evtCommand_BGM_PLAY_E, 2 },
    { evtOpcodeSetBgmVolumePan, 2 },
    { evtCommand_FADEOUT_E, 2 },
    { evtCommand_BGM_VOL_DOWN, 0 },
    { evtCommand_BGM_VOL_UP, 0 },
    { itfMesMngIgnoreConfig, 1 },
    { evtCommand_SEARCH_ID, 1 },
    { evtOpSetUnitParams6, 6 },
    { evtCommand_MODEL_WAIT_EXMOT, 3 },
    { evtCommand_CALL_NEXT, 3 },
    { evtCommand_CALL_FIELD2, 2 },
    { scrCommandResetFieldEffects, 0 },
    { evtCommand_EFFECT_LOAD, 1 },
    { evtCommand_EFFECT_CLS, 1 },
    { evtCommand_EFFECT_POS_XYZ, 4 },
    { evtCommand_EFFECT_POS, 2 },
    { evtCommand_EFFECT_ON, 1 },
    { evtCommand_EFFECT_OFF, 1 },
    { evtCommand_EFFECT_LINK, 2 },
    { evtCommand_EFFECT_LINK2, 3 },
    { evtCommand_EFFECT_UNLINK, 1 },
    { fldCmdFocusCameraOnObject, 1 },
    { fldUpdateLookAtSegment, 0 },
    { bfWaitCbCreateTask, 2 },
    { scrCommand_SCR_KILL, 1 },
    { scrCommand_SCR_WAIT, 1 },
    { scrCommand_SCR_STAT, 1 },
    { scrCommand_SCR_TIME, 1 },
    { evtCommand_PMV_RUN, 2 },
    { scrCommand_QUAKE_ON, 2 },
    { scrCommand_QUAKE_OFF, 0 },
    { evtCommand_MODEL_LIGHT_OFF, 2 },
    { evtCommand_MODEL_LIGHT_ON, 1 },
    { evtCommand_MODEL_LIGHT_RGB, 8 },
    { evtCommand_MODEL_LIGHT_DIR, 5 },
    { evtCommand_MODEL_LIGHT_WAIT, 1 },
    { evtCommand_MODEL_RGB, 5 },
    { evtCommand_MODEL_A, 3 },
    { scrCommand_SCENE_LIGHT_RGB, 4 },
    { scrCommand_SCENE_LIGHT_DIR, 4 },
    { scrCommand_SCENE_AMBIENT, 4 },
    { evtCommand_PATH_STOP, 1 },
    { evtCommand_PATH_START, 1 },
    { evtCommand_PATH_RATE, 2 },
    { evtCommand_PATH_DIR, 2 },
    { scrCommand_SCENE_FOG_RGB, 4 },
    { scrCommand_SCENE_FOG_PARA, 5 },
    { scrCommand_BLUR, 3 },
    { func_0014DAF8, 2 },
    { fldCmdFreeDisplayObjects, 0 },
    { fldCmdSetScenePhaseThree, 0 },
    { fldCmdSetLookAtHeading, 1 },
    { fldCommand_CAM_SEL_PHOKAN, 2 },
    { fldCmdSetSolarOverlayMode, 1 },
    { evtCommand_MODEL_LIGHT_LINK, 3 },
    { evtCommand_MODEL_LIGHT_UNLINK, 2 },
    { evtCommand_LIGHT_PATH_MOVE, 2 },
    { evtCommand_LIGHT_PATH_WAIT, 1 },
    { evtCommand_LOAD_BE, 1 },
    { evtCommand_MODEL_BE, 2 },
    { evtCommand_FIELD_BE, 2 },
    { evtCommand_EFFECT_BE, 2 },
    { evtCommand_TEX_BE, 2 },
    { evtCommand_EFFBED_LOAD, 1 },
    { evtCommand_EFFBED_START, 2 },
    { evtCommand_EFFECT_ROT_XYZ, 4 },
    { evtCommand_EFFBED_BE, 2 },
    { evtCommand_EFFECT_PATH_MOVE, 2 },
    { evtCommand_EFFECT_PATH_WAIT, 1 },
    { evtCommand_EFFMG1_LOAD, 1 },
    { evtCommand_EFFMG1_BE, 2 },
    { evtCommand_EFFMG2_LOAD, 1 },
    { evtCommand_EFFMG2_BE, 2 },
    { evtCommand_EFFMG1_POS, 3 },
    { evtCommand_EFFMG2_POS, 5 },
    { evtCommandStartBgmWithFade, 2 },
    { evtCommand_CAM_ROT_Q, 5 },
    { evtCommand_FIELD_ANIME_START, 1 },
    { evtCommand_FIELD_ANIME_STOP, 1 },
    { evtCommand_MODEL_SHADOW_ON2, 1 },
    { btlCmdRememberCounterLimit, 1 },
    { btlCmdStoreCounterReachedLimitChoice, 1 },
    { btlScriptReturnTurnCount, 0 },
    { NULL, 0 },
    { btlScriptSetCommandValueAndParameter, 2 },
    { evtCommand_EFFMG2_PRESET, 1 },
    { btlCmdReportUnsupportedTaikyo, 0 },
    { btlCmdReportUnsupportedAllTaikyo, 0 },
    { btlResetCommandContextAndSetStateFlag, 0 },
    { btlScriptPickLowestHpBlockTarget, 1 },
    { evtCommand_PATH_LOOP_ON, 1 },
    { evtCommand_PATH_LOOP_OFF, 1 },
    { scrCommand_MUL_BLUR_PARA, 3 },
    { scrCommand_MUL_BLUR_RGBA, 4 },
    { scrCommand_MUL_BLUR_AREA, 3 },
    { scrCommand_MUL_BLUR_ON, 1 },
    { scrCommand_MUL_BLUR_OFF, 1 },
    { scrCommand_DIST_BLUR_PARA, 7 },
    { scrCommand_DIST_BLUR_RGBA, 4 },
    { scrCommand_DIST_BLUR_AREA, 3 },
    { scrCommand_DIST_BLUR_ON, 1 },
    { scrCommand_DIST_BLUR_OFF, 1 },
    { btlCmdCameraMove, 7 },
    { btlCmdReportUnsupportedCameraOrigin, 0 },
    { btlCmdSetContextFlagOne, 0 },
    { evtCommand_READ_BE, 1 },
    { evtCommand_CHK_BE, 1 },
    { evtCommand_DEL_BE, 1 },
    { btlScriptQueueActorCommandSound, 0 },
    { btlCmdReportUnsupportedCameraTwoShot, 0 },
    { btlCmdReportUnsupportedCameraBougai, 0 },
    { fldCommand_FLD_CHK_BTL, 0 },
    { fldCommand_FLD_CHK_ANA, 0 },
    { fldCommand_FLD_CHK_SISETU, 0 },
    { evtCommand_CALL_FIELD3, 2 },
    { fldCommand_FLD_KANKYO, 2 },
    { fldCommand_FLD_FILTER, 4 },
    { fldCommand_FLD_MODEL_ON, 4 },
    { fldCmdApplyRoomModeGroupOne, 4 },
    { fldCommand_FLD_ANIME_ON, 4 },
    { fldCommand_FLD_ANIME_OFF, 4 },
    { fldCommand_FLD_ATARI_ON, 3 },
    { fldCmdSetSceneBitsBarrierRoom, 3 },
    { fldCommand_FLD_MODEL_ON2, 3 },
    { fldCommand_FLD_MODEL_OFF2, 3 },
    { fldCommand_FLD_ANIME_ON2, 3 },
    { fldCommand_FLD_ANIME_OFF2, 3 },
    { fldCommand_FLD_ANIME_ID2, 4 },
    { fldOpClearRoomStateFlags, 3 },
    { fldOpSetRoomStateFlags, 3 },
    { fldCmdSetSceneBitsValue, 4 },
    { fldCommand_FLD_MAP_ON, 3 },
    { fldCommand_FLD_MAP_OFF, 3 },
    { fldCommand_SET_CAMTBL, 1 },
    { fldCmdPushSceneParam, 1 },
    { fldCommand_FLD_SET_TAKARA_OPEN, 0 },
    { fldCommand_FLD_CHK_TAKARA_OPEN, 0 },
    { itfMesMngSetMesPositionScript, 2 },
    { itfMesMngSetMesPosDefault, 0 },
    { itfMesMngValidateNext, 1 },
    { evtCommandSubmitScalarAdminRequest, 1 },
    { movCheckStartupSoundState, 1 },
    { sndOpUpdateTitleBgmState, 0 },
    { evtOpSetActiveEntryFlag, 1 },
    { evtOpReadDisplayValue, 1 },
    { mnuResetTitlePlaybackCommands, 0 },
    { mnuAdvanceTitleStreamFromScript, 0 },
    { mnuIsTitleStreamIdle, 0 },
    { NULL, 0 },
    { evtCommand_MODEL_SHADOW_ON, 1 },
    { evtCommand_MODEL_SHADOW_OFF, 1 },
    { evtCommand_BOKASI_ON, 0 },
    { evtCommand_BOKASI_OFF, 0 },
    { scrCommand_MUL_BLUR_SWAP, 1 },
    { scrCommand_DIST_BLUR_SWAP, 1 },
    { scrCommand_BLUR2_PARA, 3 },
    { scrCommand_BLUR2_RGBA, 4 },
    { scrCommand_BLUR2_AREA, 2 },
    { scrCommand_BLUR2_SWAP, 1 },
    { scrCommand_FILTER2_PARA, 1 },
    { scrCommand_FILTER2_RGBA, 4 },
    { scrCommand_FILTER2_SWAP, 1 },
    { scrCommand_FILTER2_ON, 1 },
    { scrCommand_FILTER2_OFF, 1 },
    { scrCommand_MFILTER_PARA, 1 },
    { scrCommand_MFILTER_RGBA, 4 },
    { scrCommand_MFILTER_AREA, 3 },
    { scrCommand_MFILTER_SWAP, 1 },
    { scrCommand_MFILTER_ON, 1 },
    { scrCommand_MFILTER_OFF, 1 },
    { evtCommand_BGM_TRANS_E, 2 },
    { scrCommandAddPartyCurrency, 1 },
    { ptyScriptApplyEventPenalty, 1 },
    { itfMesMngWindowType, 1 },
    { itfMesMngSelectDefKey, 2 },
    { ptyScriptRestoreEntireParty, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { scrCommand_RIP_BLUR_ON, 1 },
    { scrCommand_RIP_BLUR_OFF, 1 },
    { scrCommand_RIP_BLUR_PARA, 7 },
    { scrCommand_RIP_BLUR_RGBA, 4 },
    { scrCommand_RIP_BLUR_AREA, 3 },
    { scrCommand_RIP_BLUR_SWAP, 1 },
    { evtOpcodeRefreshTaskData, 2 },
    { evtOpcodeInitializeEffectSoundChannel, 0 },
    { evtCommand_EFFECT_PRESET, 1 },
    { btlCmdStoreActorMpCheckChoice, 1 },
    { btlScriptReturnContextSignedByte, 0 },
    { btlAiGetAllKoudo, 0 },
    { btlScriptReturnActorLevel, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { btlScriptFlagAllUnitsDefeatCandidates, 0 },
    { btlScriptFlagPlayerDefeatCandidates, 0 },
    { btlScriptFlagEnemyDefeatCandidates, 0 },
    { btlScriptBuiltin342NoOp, 0 },
    { evtCommand_CALL_STAFF, 0 },
    { NULL, 0 },
    { btlScriptSelectModeAValueOne, 0 },
    { NULL, 0 },
    { btlScriptPickLowestLevelTarget, 0 },
    { ptyScriptGetRosterStat, 1 },
    { btlAiGetFurea, 0 },
    { evtCommand_CALL_ENDSAVE, 0 },
    { evtCommand_KUTI, 3 },
    { itfMesMngWindowTypeAlpha, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { evtCommand_KUTISET, 3 },
    { evtCommand_CALL_BATTLE2, 1 },
    { evtCommand_EFFBED_BE_EVO, 3 },
    { evtCommand_EFFBED_START_EVO, 3 },
    { btlCmdReturnLowHpActionReadiness, 0 },
    { evtCommandSetDrawFlagWhileWaiting, 1 },
    { mnuCanDrawMovie, 0 },
    { evtStoreScriptParameterResult, 1 },
    { evtPushFirstRosterLevel, 0 },
    { evtPushSecondRosterLevel, 0 },
    { evtPushFirstRosterStatEligibility, 1 },
    { evtPushSecondRosterStatEligibility, 1 },
    { evtPushSelectedStatOrRosterLowValue, 0 },
    { evtPushSelectedScaledStat, 0 },
    { evtSelectScriptStatValue, 0 },
    { evtPushSelectedTotalOrRosterHighValue, 0 },
    { evtPushEntryFlagBitInverted, 0 },
    { evtPushFirstRosterSelectedStat, 0 },
    { evtPushSecondRosterSelectedStat, 0 },
    { evtPushFirstRosterOptionStat, 0 },
    { evtPushSecondRosterOptionStat, 0 },
    { evtRollRandomScale, 1 },
    { evtPushScriptContextResult, 0 },
    { evtPushSelectedStatMaximum, 0 },
    { scrGetBattleAverageActorStatForGroup, 1 },
    { evtPushAvailableChoiceRatio, 2 },
    { evtPushAvailableFloatValue, 0 },
    { evtPushAvailableIntegerValue, 0 },
    { evtPushFirstRosterCurrentHp, 0 },
    { evtPushSecondRosterCurrentHp, 0 },
    { evtPushFirstRosterMaximumHp, 0 },
    { evtPushSecondRosterMaximumHp, 0 },
    { evtCommandReattachObjectPath, 3 },
    { evtPushLevelMaxHpGrowth, 0 },
    { evtPushLevelMaxMpGrowth, 0 },
    { evtSelectStatGrade, 0 },
    { evtPushLevelValueA, 0 },
    { evtPushLevelValueB, 0 },
    { evtScriptSelectRandomValue, 0 },
    { evtPushSelectedStatGrade, 0 },
    { evtPushLevelValueCCommand18D, 0 },
    { evtPushLevelValueCCommand18E, 0 },
    { evtPushRequestedRosterPresence, 1 },
    { ptyScriptAddUnitAndReturnResult, 1 },
    { ptyScriptRemoveUnitAndReturnResult, 1 },
    { ptyScriptSetZeroResultCommand192, 2 },
    { ptyScriptSetOneResultCommand193, 2 },
    { ptyScriptSetOneResultCommand194, 1 },
    { evtPushRosterBaseValue, 0 },
    { btlCmdReturnActorEligibility, 0 },
    { btlCommandSelectEventAction, 3 },
    { btlCommandWaitEventSoundDeadline, 2 },
    { btlCmdReportEnemySpawnCapacity, 0 },
    { btlCmdRememberLowerElementAccess, 1 },
    { btlCmdRememberUpperElementAccess, 1 },
    { btlCmdRememberLowerElementExclusion, 1 },
    { btlCmdRememberUpperElementExclusion, 1 },
    { btlCmdRememberUpperActionAvailability, 1 },
    { btlCmdRememberActorEntryCondition, 1 },
    { btlCmdRememberNonLowerStatFlag, 0 },
    { btlOpStartSelectedUnitModelChange, 4 },
    { btlCmdReturnReadyWithoutTurnsCheck, 0 },
    { btlCmdRememberContextSecondaryFlag, 0 },
    { btlAreModelChangeTasksFinished, 0 },
    { evtSelectFineStatGrade, 0 },
    { btlCommandReportSpecialModeValue, 0 },
    { btlCmdStoreDefaultUnitActionChoice, 1 },
    { btlCmdStoreAlternateUnitActionChoice, 1 },
    { btlCmdStorePartyEntryCheckChoice, 1 },
    { btlCmdStoreInversePartyEntryChoice, 1 },
    { btlCmdRememberUpperEntryCodeMatch, 1 },
    { btlCmdRememberUpperEntryCodeInverse, 1 },
    { btlCmdRememberUpperSelectedElement, 1 },
    { btlCmdRememberLowerMissingStatus, 0 },
    { btlScriptPickUnlinkedPartyTarget, 0 },
    { btlScriptAppendActorToTargets, 0 },
    { btlScriptPickTargetExcludingActor, 0 },
    { btlCommandStartSlotMotion, 3 },
    { btlCommandReportBossEffectOption, 0 },
    { btlCmdCheckHpPercent, 3 },
    { btlScriptReturnSelectedBossEffectId, 0 },
    { btlScriptReturnEffectActive, 0 },
    { btlCmdReturnBattlePhase, 0 },
    { btlCmdRememberLowerEntryCodeMismatch, 1 },
    { btlCmdRememberUpperEntryCodeMismatch, 1 },
    { btlScriptPickTargetWithoutStatusMask, 1 },
    { btlCmdRememberPartyQueuedActionCategory, 1 },
    { btlCmdTestEffectActor, 0 },
    { btlScriptReturnEffectValue, 0 },
    { btlScriptReturnSceneSlotCount, 0 },
    { btlCmdReturnPartyMissingFlagCheck, 0 },
    { btlCmdReturnPartyHeldFlagCheck, 0 },
    { btlScriptPickLinkedPartyTarget, 0 },
    { btlScriptReturnFirstSpecialEnemySpecies, 0 },
    { btlCommandReportEventEffectValue, 0 },
    { btlScriptAddEffectActorTarget, 0 },
    { btlScriptReturnGlobalDebugValue, 0 },
    { btlCmdRememberLowerElementBlock, 1 },
    { btlScriptPickElementBlockTarget, 1 },
    { btlCmdRememberUpperQueuedQuery, 1 },
    { btlCmdRememberLowerQueuedQuery, 1 },
    { btlScriptSetActorUnitParameter, 1 },
    { btlScriptPickHighestMpTarget, 0 },
    { btlScriptPickElementCheckedTarget, 1 },
    { btlCmdReportPartyEmptyMp, 0 },
    { btlCommandSetSequenceVolumePan, 1 },
    { btlCommandReportSpecialEnemyLink, 0 },
    { evtPushEntryIndexedStatOption, 0 },
    { btlCmdClearSpecialEnemyFlags, 0 },
    { scrGetBattleAverageMaximumValueForGroup, 1 },
    { scrGetBattleAverageCurrentValueForGroup, 1 },
    { btlScriptQueueUnboundCommandSound, 0 },
    { btlScriptSetCameraBlendStart, 7 },
    { btlScriptSetCameraBlendEnd, 7 },
    { btlCmdCameraMoveBlend, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { evtOpQueueWorldObjectPendingValue, 2 },
    { evtOpClearWorldObjectPendingValue, 1 },
    { evtOpModelLodChg, 2 },
    { itfCommandSelectOptionWithFade, 1 },
    { itfPanelAcquireHold, 0 },
    { itfCommandQueueHeldPanelEntry, 3 },
    { itfStepHeldPanelMessages, 0 },
    { scrCommandClearProcessControlFlag, 0 },
    { scrCommandSetProcessControlFlag, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { fldCommandStartTitle, 2 },
    { fldCommand_FLD_GET_KIRETA, 1 },
    { fldCmdRollMirroredSolarThreshold, 0 },
    { fldCommand_FLD_MSE_PLAY2, 2 },
    { fldCommand_FLD_MSE_STOP2, 2 },
    { fldCommand_SET_NPC_CAMERA, 1 },
    { fldCmdMarkBitmapRegion, 5 },
    { fldCommandSendNamedSound, 1 },
    { fldCommandSendSoundControl, 0 },
    { fldCommandIsSoundBusy, 0 },
    { fldCmdGetActorStat1, 1 },
    { fldCommand_FLD_WARP_HASIGO_GETTBL, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { fldCmdGetCurrentSceneSelectionResource, 0 },
    { fldCommandFindEffectByName, 1 },
    { fldCommand_FLD_WARP_ELV_GETTBL, 1 },
    { fldCommand_FLD_WARP_ELV_EXIT, 1 },
    { fldCmdSetCurrentTaskScene, 0 },
    { func_0014EF20, 0 },
    { fldCommand_FLD_WARP_RUN_START, 0 },
    { fldCommand_FLD_INF_NEXT, 2 },
    { fldCommand_FLD_INF_GET, 2 },
    { fldCommand_FLD_INF_GETPANEL, 0 },
    { fldCmdGetActorStat0, 1 },
    { fldCommand_FLD_ELV_FLOORSTART, 2 },
    { fldCommand_FLD_ELV_FLOOREND, 0 },
    { fldCommand_FLD_ELV_ENDCHK, 0 },
    { fldCommand_FLD_WARP_GET_DOOR, 1 },
    { fldCommand_FLD_EXIT_CAMERA, 0 },
    { fldCommand_FLD_INF_GOWAP, 0 },
    { fldCmdDispatchSceneAction, 1 },
    { fldCmdStartSceneBgm, 1 },
    { fldCmdStartSceneBgmAlternate, 1 },
    { fldCmdStopCurrentBgm, 0 },
    { fldCmdPlayCurrentBgmSound, 0 },
    { fldCommandPlaySeVolumePan, 1 },
    { fldCommandPlaySe, 1 },
    { fldCmdReleaseCurrentBgm, 0 },
    { fldCommandLoadArchive, 1 },
    { NULL, 0 },
    { fldCommand_FLD_WARP_GET_WARPEFF, 0 },
    { NULL, 0 },
    { fldCmdQueryAlternateActorEntrySceneStatus, 1 },
    { fldCommand_FLD_SET_ENCOUNT, 1 },
    { fldCommand_ACT_WIN_DSP3, 1 },
    { fldCmdInitializeHuntSparkUnit, 1 },
    { fldCmdSetStateFlag, 0 },
    { NULL, 0 },
};

