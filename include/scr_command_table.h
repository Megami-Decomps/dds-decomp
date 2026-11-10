#ifndef SCR_COMMAND_TABLE_H
#define SCR_COMMAND_TABLE_H

/* The rows of the script command table that script/scrScriptProcess defines,
 * the one unit including this. The commands are the game's, so the rows live
 * here and not in sdf-decomp's shared unit. */

#if defined(VERSION_DDS1)
s32 itfMesMngMessage(void);
s32 itfMesMngOpenWindow(void);
s32 itfMesMngCloseWindow(void);
s32 itfMesMngSelect(void);
s32 itfMesMngOpenSelectWindow(void);
s32 itfMesMngCloseSelectWindow(void);
s32 itfMesMngMaskSelect(void);
s32 scrCommand_BIT_CHK(void);
s32 scrCommand_BIT_ON(void);
s32 scrCommand_BIT_OFF(void);
s32 scrCommand_RND(void);
s32 sndOpSetTrackDefaultVolumePan(void);
s32 sndOpStartTrackFromScript(void);
s32 scrCommand_SYNC(void);
s32 scrCommand_WAIT(void);
s32 bfWaitCbScreenFadeA(void);
s32 bfWaitCbScreenFadeB(void);
s32 scrCommand_SEL_JMP(void);
s32 evtCommandAddEffectUnitToWorld(void);
s32 evtCommand_CAM_PATH_MOVE(void);
s32 evtCommand_SET_SKY(void);
s32 evtCommand_MODEL_DSP(void);
s32 evtCmdDestroySelectedWorldUnit(void);
s32 evtOpStartUnitTransitionTowardWorldObject(void);
s32 evtOpDeprecatedModelMotion(void);
s32 evtCommand_CAM_SEL_FOVY(void);
s32 scrCommand_PAD_ACT(void);
s32 evtCommand_MODEL_SCL_XYZ(void);
s32 scrCommand_SCR_EXISTS(void);
s32 scrCommandFadeBackgroundOut(void);
s32 scrCommandFadeBackgroundIn(void);
s32 evtCommandStartPolygonMovie(void);
s32 evtCommandClearPolygonMovieFlag(void);
s32 evtCommand_FIELD_CLS_STOP(void);
s32 scrCommandPrintInteger(void);
s32 evtCommand_GET_MOON(void);
s32 evtCommand_UNIT_CLS(void);
s32 evtCommand_GET_MOON16(void);
s32 evtCommand_RESTORE_BATTLE(void);
s32 evtCommand_UNIT_CLS_STOP(void);
s32 evtCommand_TEX_LOAD(void);
s32 evtCommand_TEX_ON(void);
s32 evtCommand_TEX_OFF(void);
s32 evtCommand_TEX_CLS(void);
s32 evtCommandSignalAdminAtTimerZero(void);
s32 btlScriptSetBattleCommand(void);
s32 btlScriptSelectEmptyCommand(void);
s32 btlScriptSelectFixedCommand(void);
s32 btlScriptSelectModeDValueOne(void);
s32 btlScriptSelectActionEntry(void);
s32 btlScriptChooseActionTargets(void);
s32 btlScriptPickLowestHpTarget(void);
s32 btlScriptPickTargetWithStatusMask(void);
s32 btlScriptCmdSelectTargetsByMode(void);
s32 btlCmdReportUnsupportedEvent(void);
s32 btlCmdReturnPartyActionPresence(void);
s32 btlCmdReturnEnemyActionPresence(void);
s32 btlScriptSetBattleWorkParameter(void);
s32 btlScriptChooseActionTargetsDuplicate(void);
s32 btlCmdRunWeightedAiSelection(void);
s32 btlScriptClearSceneTransition(void);
s32 btlScriptBeginSceneTransition(void);
s32 itfMesMng_SEL_SELNO(void);
s32 scrCommand_BLUR2_ON(void);
s32 scrCommand_BLUR2_OFF(void);
s32 scrCommandResetDrawEffects(void);
s32 evtCommand_MODEL_EXWAIT(void);
s32 sndOpSendTitleAudioCommand(void);
s32 evtCommand_RTRN_TITLE(void);
s32 scrCommand_SOFTEN_BIAS(void);
s32 evtCommand_SET_SKY_A(void);
s32 evtCommand_MODEL_WAIT(void);
s32 evtCommand_MODEL_POS(void);
s32 evtCommand_MODEL_WAIT_MOT(void);
s32 evtCommand_MODEL_MOVE_MOT(void);
s32 evtCommand_FIELD_DSP(void);
s32 evtCommand_FIELD_CLS(void);
s32 itfCamp(void);
s32 mnuScriptRequestMovieByIndex(void);
s32 mnuScriptStopMovieAndResetDraw(void);
s32 mnuUpdateMovieDrawShutdownCountdown(void);
s32 scrCommand_PAD_TRIG(void);
s32 scrCommand_PAD_PRESS(void);
s32 evtCommand_CAM_PATH_WAIT(void);
s32 evtCommand_MODEL_ROT(void);
s32 evtOpTurnUnitRelativeToWorldNode(void);
s32 evtOpRestoreUnitReferenceAngle(void);
s32 evtCommand_CALL_SAVE(void);
s32 evtCommand_CALL_FIELD(void);
s32 fldCmdQueryActorEntrySceneStatus(void);
s32 fldCmdTestActorEntryCondition(void);
s32 fldCommand_PAD_DISABLE(void);
s32 fldCommand_PAD_ENABLE(void);
s32 mnuOpenTitleMenuDefaultMode(void);
s32 evtCommand_MOON_START(void);
s32 evtCommand_MOON_STOP(void);
s32 evtCommand_CHG_MOON(void);
s32 evtCommand_CALL_EVENT(void);
s32 evtCommandSubmitEvent(void);
s32 evtCommand_MODEL_PLAYER(void);
s32 evtCommand_MODEL_ON(void);
s32 evtCommand_MODEL_OFF(void);
s32 evtCommand_OBJ_PATH_MOVE(void);
s32 evtCommand_OBJ_PATH_WAIT(void);
s32 scrCommand_SET_FOVY(void);
s32 fldCommand_ENCOUNT_ON(void);
s32 fldCommand_ENCOUNT_OFF(void);
s32 scrCommandAdjustItemQuantity(void);
s32 itfMesMngSetVarScr(void);
s32 scrCommandPrintString(void);
s32 evtOpSetUnitParams5(void);
s32 evtCommand_MODEL_WAIT_HOKAN(void);
s32 evtCommand_CAM_CREATE(void);
s32 evtCommand_CAM_CLS(void);
s32 evtCommand_CAM_POS_XYZ(void);
s32 evtCommand_CAM_ROT_XYZ(void);
s32 evtCommand_MODEL_POS_XYZ(void);
s32 evtCommand_MODEL_ROT_XYZ(void);
s32 btlNbScriptCheckActorFlag(void);
s32 btlCmdStoreBossHealthCheckChoice(void);
s32 btlScriptPickLowestHpRatioTarget(void);
s32 btlCmdStoreEnemyCountCheckChoice(void);
s32 btlCmdStorePartyCountCheckChoice(void);
s32 btlCmdStoreActorActionMaskChoice(void);
s32 btlCmdStoreEnemyActionMaskChoice(void);
s32 btlCmdStorePartyActionMaskChoice(void);
s32 btlCmdRememberAllLowerActionBits(void);
s32 btlCmdRememberLowerUnitMode(void);
s32 btlCmdRememberOtherUpperUnitMode(void);
s32 btlCmdRememberLowerEntryCondition(void);
s32 btlCmdRememberUpperEntryCondition(void);
s32 btlAiCheckBa(void);
s32 btlCmdReturnPartyElementQueryCheck(void);
s32 btlCmdReturnEnemyElementQueryCheck(void);
s32 evtCommand_MODEL_MOVE2(void);
s32 btlCmdRememberActorActionGate(void);
s32 evtCommand_LOAD_SOUND_E(void);
s32 evtCommand_BGM_PLAY_E(void);
s32 evtOpcodeSetBgmVolumePan(void);
s32 evtCommand_FADEOUT_E(void);
s32 evtCommand_BGM_VOL_DOWN(void);
s32 evtCommand_BGM_VOL_UP(void);
s32 itfMesMngIgnoreConfig(void);
s32 evtCommand_SEARCH_ID(void);
s32 evtOpSetUnitParams6(void);
s32 evtCommand_MODEL_WAIT_EXMOT(void);
s32 evtCommand_CALL_NEXT(void);
s32 evtCommand_CALL_FIELD2(void);
s32 scrCommandResetFieldEffects(void);
s32 evtCommand_EFFECT_LOAD(void);
s32 evtCommand_EFFECT_CLS(void);
s32 evtCommand_EFFECT_POS_XYZ(void);
s32 evtCommand_EFFECT_POS(void);
s32 evtCommand_EFFECT_ON(void);
s32 evtCommand_EFFECT_OFF(void);
s32 evtCommand_EFFECT_LINK(void);
s32 evtCommand_EFFECT_LINK2(void);
s32 evtCommand_EFFECT_UNLINK(void);
s32 fldCmdFocusCameraOnObject(void);
s32 fldUpdateLookAtSegment(void);
s32 bfWaitCbCreateTask(void);
s32 scrCommand_SCR_KILL(void);
s32 scrCommand_SCR_WAIT(void);
s32 scrCommand_SCR_STAT(void);
s32 scrCommand_SCR_TIME(void);
s32 evtCommand_PMV_RUN(void);
s32 scrCommand_QUAKE_ON(void);
s32 scrCommand_QUAKE_OFF(void);
s32 evtCommand_MODEL_LIGHT_OFF(void);
s32 evtCommand_MODEL_LIGHT_ON(void);
s32 evtCommand_MODEL_LIGHT_RGB(void);
s32 evtCommand_MODEL_LIGHT_DIR(void);
s32 evtCommand_MODEL_LIGHT_WAIT(void);
s32 evtCommand_MODEL_RGB(void);
s32 evtCommand_MODEL_A(void);
s32 scrCommand_SCENE_LIGHT_RGB(void);
s32 scrCommand_SCENE_LIGHT_DIR(void);
s32 scrCommand_SCENE_AMBIENT(void);
s32 evtCommand_PATH_STOP(void);
s32 evtCommand_PATH_START(void);
s32 evtCommand_PATH_RATE(void);
s32 evtCommand_PATH_DIR(void);
s32 scrCommand_SCENE_FOG_RGB(void);
s32 scrCommand_SCENE_FOG_PARA(void);
s32 scrCommand_BLUR(void);
s32 func_0014DAF8(void);
s32 fldCmdFreeDisplayObjects(void);
s32 fldCmdSetScenePhaseThree(void);
s32 fldCmdSetLookAtHeading(void);
s32 fldCommand_CAM_SEL_PHOKAN(void);
s32 fldCmdSetSolarOverlayMode(void);
s32 evtCommand_MODEL_LIGHT_LINK(void);
s32 evtCommand_MODEL_LIGHT_UNLINK(void);
s32 evtCommand_LIGHT_PATH_MOVE(void);
s32 evtCommand_LIGHT_PATH_WAIT(void);
s32 evtCommand_LOAD_BE(void);
s32 evtCommand_MODEL_BE(void);
s32 evtCommand_FIELD_BE(void);
s32 evtCommand_EFFECT_BE(void);
s32 evtCommand_TEX_BE(void);
s32 evtCommand_EFFBED_LOAD(void);
s32 evtCommand_EFFBED_START(void);
s32 evtCommand_EFFECT_ROT_XYZ(void);
s32 evtCommand_EFFBED_BE(void);
s32 evtCommand_EFFECT_PATH_MOVE(void);
s32 evtCommand_EFFECT_PATH_WAIT(void);
s32 evtCommand_EFFMG1_LOAD(void);
s32 evtCommand_EFFMG1_BE(void);
s32 evtCommand_EFFMG2_LOAD(void);
s32 evtCommand_EFFMG2_BE(void);
s32 evtCommand_EFFMG1_POS(void);
s32 evtCommand_EFFMG2_POS(void);
s32 evtCommandStartBgmWithFade(void);
s32 evtCommand_CAM_ROT_Q(void);
s32 evtCommand_FIELD_ANIME_START(void);
s32 evtCommand_FIELD_ANIME_STOP(void);
s32 evtCommand_MODEL_SHADOW_ON2(void);
s32 btlCmdRememberCounterLimit(void);
s32 btlCmdStoreCounterReachedLimitChoice(void);
s32 btlScriptReturnTurnCount(void);
s32 btlScriptSetCommandValueAndParameter(void);
s32 evtCommand_EFFMG2_PRESET(void);
s32 btlCmdReportUnsupportedTaikyo(void);
s32 btlCmdReportUnsupportedAllTaikyo(void);
s32 btlResetCommandContextAndSetStateFlag(void);
s32 btlScriptPickLowestHpBlockTarget(void);
s32 evtCommand_PATH_LOOP_ON(void);
s32 evtCommand_PATH_LOOP_OFF(void);
s32 scrCommand_MUL_BLUR_PARA(void);
s32 scrCommand_MUL_BLUR_RGBA(void);
s32 scrCommand_MUL_BLUR_AREA(void);
s32 scrCommand_MUL_BLUR_ON(void);
s32 scrCommand_MUL_BLUR_OFF(void);
s32 scrCommand_DIST_BLUR_PARA(void);
s32 scrCommand_DIST_BLUR_RGBA(void);
s32 scrCommand_DIST_BLUR_AREA(void);
s32 scrCommand_DIST_BLUR_ON(void);
s32 scrCommand_DIST_BLUR_OFF(void);
s32 btlCmdCameraMove(void);
s32 btlCmdReportUnsupportedCameraOrigin(void);
s32 btlCmdSetContextFlagOne(void);
s32 evtCommand_READ_BE(void);
s32 evtCommand_CHK_BE(void);
s32 evtCommand_DEL_BE(void);
s32 btlScriptQueueActorCommandSound(void);
s32 btlCmdReportUnsupportedCameraTwoShot(void);
s32 btlCmdReportUnsupportedCameraBougai(void);
s32 fldCommand_FLD_CHK_BTL(void);
s32 fldCommand_FLD_CHK_ANA(void);
s32 fldCommand_FLD_CHK_SISETU(void);
s32 evtCommand_CALL_FIELD3(void);
s32 fldCommand_FLD_KANKYO(void);
s32 fldCommand_FLD_FILTER(void);
s32 fldCommand_FLD_MODEL_ON(void);
s32 fldCmdApplyRoomModeGroupOne(void);
s32 fldCommand_FLD_ANIME_ON(void);
s32 fldCommand_FLD_ANIME_OFF(void);
s32 fldCommand_FLD_ATARI_ON(void);
s32 fldCmdSetSceneBitsBarrierRoom(void);
s32 fldCommand_FLD_MODEL_ON2(void);
s32 fldCommand_FLD_MODEL_OFF2(void);
s32 fldCommand_FLD_ANIME_ON2(void);
s32 fldCommand_FLD_ANIME_OFF2(void);
s32 fldCommand_FLD_ANIME_ID2(void);
s32 fldOpClearRoomStateFlags(void);
s32 fldOpSetRoomStateFlags(void);
s32 fldCmdSetSceneBitsValue(void);
s32 fldCommand_FLD_MAP_ON(void);
s32 fldCommand_FLD_MAP_OFF(void);
s32 fldCommand_SET_CAMTBL(void);
s32 fldCmdPushSceneParam(void);
s32 fldCommand_FLD_SET_TAKARA_OPEN(void);
s32 fldCommand_FLD_CHK_TAKARA_OPEN(void);
s32 itfMesMngSetMesPositionScript(void);
s32 itfMesMngSetMesPosDefault(void);
s32 itfMesMngValidateNext(void);
s32 evtCommandSubmitScalarAdminRequest(void);
s32 movCheckStartupSoundState(void);
s32 sndOpUpdateTitleBgmState(void);
s32 evtOpSetActiveEntryFlag(void);
s32 evtOpReadDisplayValue(void);
s32 mnuResetTitlePlaybackCommands(void);
s32 mnuAdvanceTitleStreamFromScript(void);
s32 mnuIsTitleStreamIdle(void);
s32 evtCommand_MODEL_SHADOW_ON(void);
s32 evtCommand_MODEL_SHADOW_OFF(void);
s32 evtCommand_BOKASI_ON(void);
s32 evtCommand_BOKASI_OFF(void);
s32 scrCommand_MUL_BLUR_SWAP(void);
s32 scrCommand_DIST_BLUR_SWAP(void);
s32 scrCommand_BLUR2_PARA(void);
s32 scrCommand_BLUR2_RGBA(void);
s32 scrCommand_BLUR2_AREA(void);
s32 scrCommand_BLUR2_SWAP(void);
s32 scrCommand_FILTER2_PARA(void);
s32 scrCommand_FILTER2_RGBA(void);
s32 scrCommand_FILTER2_SWAP(void);
s32 scrCommand_FILTER2_ON(void);
s32 scrCommand_FILTER2_OFF(void);
s32 scrCommand_MFILTER_PARA(void);
s32 scrCommand_MFILTER_RGBA(void);
s32 scrCommand_MFILTER_AREA(void);
s32 scrCommand_MFILTER_SWAP(void);
s32 scrCommand_MFILTER_ON(void);
s32 scrCommand_MFILTER_OFF(void);
s32 evtCommand_BGM_TRANS_E(void);
s32 scrCommandAddPartyCurrency(void);
s32 ptyScriptApplyEventPenalty(void);
s32 itfMesMngWindowType(void);
s32 itfMesMngSelectDefKey(void);
s32 ptyScriptRestoreEntireParty(void);
s32 scrCommand_RIP_BLUR_ON(void);
s32 scrCommand_RIP_BLUR_OFF(void);
s32 scrCommand_RIP_BLUR_PARA(void);
s32 scrCommand_RIP_BLUR_RGBA(void);
s32 scrCommand_RIP_BLUR_AREA(void);
s32 scrCommand_RIP_BLUR_SWAP(void);
s32 evtOpcodeRefreshTaskData(void);
s32 evtOpcodeInitializeEffectSoundChannel(void);
s32 evtCommand_EFFECT_PRESET(void);
s32 btlCmdStoreActorMpCheckChoice(void);
s32 btlScriptReturnContextSignedByte(void);
s32 btlAiGetAllKoudo(void);
s32 btlScriptReturnActorLevel(void);
s32 btlScriptFlagAllUnitsDefeatCandidates(void);
s32 btlScriptFlagPlayerDefeatCandidates(void);
s32 btlScriptFlagEnemyDefeatCandidates(void);
s32 btlScriptBuiltin342NoOp(void);
s32 evtCommand_CALL_STAFF(void);
s32 btlScriptSelectModeAValueOne(void);
s32 btlScriptPickLowestLevelTarget(void);
s32 ptyScriptGetRosterStat(void);
s32 btlAiGetFurea(void);
s32 evtCommand_CALL_ENDSAVE(void);
s32 evtCommand_KUTI(void);
s32 itfMesMngWindowTypeAlpha(void);
s32 evtCommand_KUTISET(void);
s32 evtCommand_CALL_BATTLE2(void);
s32 evtCommand_EFFBED_BE_EVO(void);
s32 evtCommand_EFFBED_START_EVO(void);
s32 btlCmdReturnLowHpActionReadiness(void);
s32 evtCommandSetDrawFlagWhileWaiting(void);
s32 mnuCanDrawMovie(void);
s32 evtStoreScriptParameterResult(void);
s32 evtPushFirstRosterLevel(void);
s32 evtPushSecondRosterLevel(void);
s32 evtPushFirstRosterStatEligibility(void);
s32 evtPushSecondRosterStatEligibility(void);
s32 evtPushSelectedStatOrRosterLowValue(void);
s32 evtPushSelectedScaledStat(void);
s32 evtSelectScriptStatValue(void);
s32 evtPushSelectedTotalOrRosterHighValue(void);
s32 evtPushEntryFlagBitInverted(void);
s32 evtPushFirstRosterSelectedStat(void);
s32 evtPushSecondRosterSelectedStat(void);
s32 evtPushFirstRosterOptionStat(void);
s32 evtPushSecondRosterOptionStat(void);
s32 evtRollRandomScale(void);
s32 evtPushScriptContextResult(void);
s32 evtPushSelectedStatMaximum(void);
s32 scrGetBattleAverageActorStatForGroup(void);
s32 evtPushAvailableChoiceRatio(void);
s32 evtPushAvailableFloatValue(void);
s32 evtPushAvailableIntegerValue(void);
s32 evtPushFirstRosterCurrentHp(void);
s32 evtPushSecondRosterCurrentHp(void);
s32 evtPushFirstRosterMaximumHp(void);
s32 evtPushSecondRosterMaximumHp(void);
s32 evtCommandReattachObjectPath(void);
s32 evtPushLevelMaxHpGrowth(void);
s32 evtPushLevelMaxMpGrowth(void);
s32 evtSelectStatGrade(void);
s32 evtPushLevelValueA(void);
s32 evtPushLevelValueB(void);
s32 evtScriptSelectRandomValue(void);
s32 evtPushSelectedStatGrade(void);
s32 evtPushLevelValueCCommand18D(void);
s32 evtPushLevelValueCCommand18E(void);
s32 evtPushRequestedRosterPresence(void);
s32 ptyScriptAddUnitAndReturnResult(void);
s32 ptyScriptRemoveUnitAndReturnResult(void);
s32 ptyScriptSetZeroResultCommand192(void);
s32 ptyScriptSetOneResultCommand193(void);
s32 ptyScriptSetOneResultCommand194(void);
s32 evtPushRosterBaseValue(void);
s32 btlCmdReturnActorEligibility(void);
s32 btlCommandSelectEventAction(void);
s32 btlCommandWaitEventSoundDeadline(void);
s32 btlCmdReportEnemySpawnCapacity(void);
s32 btlCmdRememberLowerElementAccess(void);
s32 btlCmdRememberUpperElementAccess(void);
s32 btlCmdRememberLowerElementExclusion(void);
s32 btlCmdRememberUpperElementExclusion(void);
s32 btlCmdRememberUpperActionAvailability(void);
s32 btlCmdRememberActorEntryCondition(void);
s32 btlCmdRememberNonLowerStatFlag(void);
s32 btlOpStartSelectedUnitModelChange(void);
s32 btlCmdReturnReadyWithoutTurnsCheck(void);
s32 btlCmdRememberContextSecondaryFlag(void);
s32 btlAreModelChangeTasksFinished(void);
s32 evtSelectFineStatGrade(void);
s32 btlCommandReportSpecialModeValue(void);
s32 btlCmdStoreDefaultUnitActionChoice(void);
s32 btlCmdStoreAlternateUnitActionChoice(void);
s32 btlCmdStorePartyEntryCheckChoice(void);
s32 btlCmdStoreInversePartyEntryChoice(void);
s32 btlCmdRememberUpperEntryCodeMatch(void);
s32 btlCmdRememberUpperEntryCodeInverse(void);
s32 btlCmdRememberUpperSelectedElement(void);
s32 btlCmdRememberLowerMissingStatus(void);
s32 btlScriptPickUnlinkedPartyTarget(void);
s32 btlScriptAppendActorToTargets(void);
s32 btlScriptPickTargetExcludingActor(void);
s32 btlCommandStartSlotMotion(void);
s32 btlCommandReportBossEffectOption(void);
s32 btlCmdCheckHpPercent(void);
s32 btlScriptReturnSelectedBossEffectId(void);
s32 btlScriptReturnEffectActive(void);
s32 btlCmdReturnBattlePhase(void);
s32 btlCmdRememberLowerEntryCodeMismatch(void);
s32 btlCmdRememberUpperEntryCodeMismatch(void);
s32 btlScriptPickTargetWithoutStatusMask(void);
s32 btlCmdRememberPartyQueuedActionCategory(void);
s32 btlCmdTestEffectActor(void);
s32 btlScriptReturnEffectValue(void);
s32 btlScriptReturnSceneSlotCount(void);
s32 btlCmdReturnPartyMissingFlagCheck(void);
s32 btlCmdReturnPartyHeldFlagCheck(void);
s32 btlScriptPickLinkedPartyTarget(void);
s32 btlScriptReturnFirstSpecialEnemySpecies(void);
s32 btlCommandReportEventEffectValue(void);
s32 btlScriptAddEffectActorTarget(void);
s32 btlScriptReturnGlobalDebugValue(void);
s32 btlCmdRememberLowerElementBlock(void);
s32 btlScriptPickElementBlockTarget(void);
s32 btlCmdRememberUpperQueuedQuery(void);
s32 btlCmdRememberLowerQueuedQuery(void);
s32 btlScriptSetActorUnitParameter(void);
s32 btlScriptPickHighestMpTarget(void);
s32 btlScriptPickElementCheckedTarget(void);
s32 btlCmdReportPartyEmptyMp(void);
s32 btlCommandSetSequenceVolumePan(void);
s32 btlCommandReportSpecialEnemyLink(void);
s32 evtPushEntryIndexedStatOption(void);
s32 btlCmdClearSpecialEnemyFlags(void);
s32 scrGetBattleAverageMaximumValueForGroup(void);
s32 scrGetBattleAverageCurrentValueForGroup(void);
s32 btlScriptQueueUnboundCommandSound(void);
s32 btlScriptSetCameraBlendStart(void);
s32 btlScriptSetCameraBlendEnd(void);
s32 btlCmdCameraMoveBlend(void);
s32 evtOpQueueWorldObjectPendingValue(void);
s32 evtOpClearWorldObjectPendingValue(void);
s32 evtOpModelLodChg(void);
s32 itfCommandSelectOptionWithFade(void);
s32 itfPanelAcquireHold(void);
s32 itfCommandQueueHeldPanelEntry(void);
s32 itfStepHeldPanelMessages(void);
s32 scrCommandClearProcessControlFlag(void);
s32 scrCommandSetProcessControlFlag(void);
s32 fldCommandStartTitle(void);
s32 fldCommand_FLD_GET_KIRETA(void);
s32 fldCmdRollMirroredSolarThreshold(void);
s32 fldCommand_FLD_MSE_PLAY2(void);
s32 fldCommand_FLD_MSE_STOP2(void);
s32 fldCommand_SET_NPC_CAMERA(void);
s32 fldCmdMarkBitmapRegion(void);
s32 fldCommandSendNamedSound(void);
s32 fldCommandSendSoundControl(void);
s32 fldCommandIsSoundBusy(void);
s32 fldCmdGetActorStat1(void);
s32 fldCommand_FLD_WARP_HASIGO_GETTBL(void);
s32 fldCmdGetCurrentSceneSelectionResource(void);
s32 fldCommandFindEffectByName(void);
s32 fldCommand_FLD_WARP_ELV_GETTBL(void);
s32 fldCommand_FLD_WARP_ELV_EXIT(void);
s32 fldCmdSetCurrentTaskScene(void);
s32 func_0014EF20(void);
s32 fldCommand_FLD_WARP_RUN_START(void);
s32 fldCommand_FLD_INF_NEXT(void);
s32 fldCommand_FLD_INF_GET(void);
s32 fldCommand_FLD_INF_GETPANEL(void);
s32 fldCmdGetActorStat0(void);
s32 fldCommand_FLD_ELV_FLOORSTART(void);
s32 fldCommand_FLD_ELV_FLOOREND(void);
s32 fldCommand_FLD_ELV_ENDCHK(void);
s32 fldCommand_FLD_WARP_GET_DOOR(void);
s32 fldCommand_FLD_EXIT_CAMERA(void);
s32 fldCommand_FLD_INF_GOWAP(void);
s32 fldCmdDispatchSceneAction(void);
s32 fldCmdStartSceneBgm(void);
s32 fldCmdStartSceneBgmAlternate(void);
s32 fldCmdStopCurrentBgm(void);
s32 fldCmdPlayCurrentBgmSound(void);
s32 fldCommandPlaySeVolumePan(void);
s32 fldCommandPlaySe(void);
s32 fldCmdReleaseCurrentBgm(void);
s32 fldCommandLoadArchive(void);
s32 fldCommand_FLD_WARP_GET_WARPEFF(void);
s32 fldCmdQueryAlternateActorEntrySceneStatus(void);
s32 fldCommand_FLD_SET_ENCOUNT(void);
s32 fldCommand_ACT_WIN_DSP3(void);
s32 fldCmdInitializeHuntSparkUnit(void);
s32 fldCmdSetStateFlag(void);

#define SCR_COMMAND_TABLE_ENTRIES \
    { itfMesMngMessage, 1 },                          \
    { itfMesMngOpenWindow, 0 },                       \
    { itfMesMngCloseWindow, 0 },                      \
    { itfMesMngSelect, 1 },                           \
    { itfMesMngOpenSelectWindow, 0 },                 \
    { itfMesMngCloseSelectWindow, 0 },                \
    { itfMesMngMaskSelect, 1 },                       \
    { scrCommand_BIT_CHK, 1 },                        \
    { scrCommand_BIT_ON, 1 },                         \
    { scrCommand_BIT_OFF, 1 },                        \
    { scrCommand_RND, 1 },                            \
    { sndOpSetTrackDefaultVolumePan, 1 },             \
    { sndOpStartTrackFromScript, 1 },                 \
    { scrCommand_SYNC, 0 },                           \
    { scrCommand_WAIT, 1 },                           \
    { bfWaitCbScreenFadeA, 2 },                       \
    { bfWaitCbScreenFadeB, 2 },                       \
    { scrCommand_SEL_JMP, 8 },                        \
    { evtCommandAddEffectUnitToWorld, 1 },            \
    { evtCommand_CAM_PATH_MOVE, 2 },                  \
    { evtCommand_SET_SKY, 3 },                        \
    { evtCommand_MODEL_DSP, 2 },                      \
    { evtCmdDestroySelectedWorldUnit, 1 },            \
    { evtOpStartUnitTransitionTowardWorldObject, 5 }, \
    { evtOpDeprecatedModelMotion, 4 },                \
    { evtCommand_CAM_SEL_FOVY, 1 },                   \
    { scrCommand_PAD_ACT, 3 },                        \
    { evtCommand_MODEL_SCL_XYZ, 4 },                  \
    { scrCommand_SCR_EXISTS, 1 },                     \
    { NULL, 0 },                                      \
    { scrCommandFadeBackgroundOut, 1 },               \
    { scrCommandFadeBackgroundIn, 1 },                \
    { NULL, 0 },                                      \
    { evtCommandStartPolygonMovie, 2 },               \
    { evtCommandClearPolygonMovieFlag, 1 },           \
    { evtCommand_FIELD_CLS_STOP, 0 },                 \
    { scrCommandPrintInteger, 1 },                    \
    { evtCommand_GET_MOON, 0 },                       \
    { evtCommand_UNIT_CLS, 0 },                       \
    { evtCommand_GET_MOON16, 0 },                     \
    { evtCommand_RESTORE_BATTLE, 2 },                 \
    { evtCommand_UNIT_CLS_STOP, 0 },                  \
    { evtCommand_TEX_LOAD, 1 },                       \
    { evtCommand_TEX_ON, 1 },                         \
    { evtCommand_TEX_OFF, 1 },                        \
    { evtCommand_TEX_CLS, 1 },                        \
    { evtCommandSignalAdminAtTimerZero, 0 },          \
    { btlScriptSetBattleCommand, 1 },                 \
    { btlScriptSelectEmptyCommand, 0 },               \
    { btlScriptSelectFixedCommand, 0 },               \
    { btlScriptSelectModeDValueOne, 0 },              \
    { btlScriptSelectActionEntry, 1 },                \
    { btlScriptChooseActionTargets, 0 },              \
    { btlScriptPickLowestHpTarget, 0 },               \
    { btlScriptPickTargetWithStatusMask, 1 },         \
    { btlScriptCmdSelectTargetsByMode, 1 },           \
    { btlCmdReportUnsupportedEvent, 1 },              \
    { btlCmdReturnPartyActionPresence, 1 },           \
    { btlCmdReturnEnemyActionPresence, 1 },           \
    { btlScriptSetBattleWorkParameter, 1 },           \
    { btlScriptChooseActionTargetsDuplicate, 0 },     \
    { btlCmdRunWeightedAiSelection, 0 },              \
    { btlScriptClearSceneTransition, 0 },             \
    { btlScriptBeginSceneTransition, 0 },             \
    { itfMesMng_SEL_SELNO, 1 },                       \
    { scrCommand_BLUR2_ON, 1 },                       \
    { scrCommand_BLUR2_OFF, 1 },                      \
    { scrCommandResetDrawEffects, 0 },                \
    { evtCommand_MODEL_EXWAIT, 2 },                   \
    { sndOpSendTitleAudioCommand, 0 },                \
    { evtCommand_RTRN_TITLE, 0 },                     \
    { scrCommand_SOFTEN_BIAS, 3 },                    \
    { evtCommand_SET_SKY_A, 2 },                      \
    { evtCommand_MODEL_WAIT, 1 },                     \
    { evtCommand_MODEL_POS, 2 },                      \
    { evtCommand_MODEL_WAIT_MOT, 2 },                 \
    { evtCommand_MODEL_MOVE_MOT, 2 },                 \
    { evtCommand_FIELD_DSP, 2 },                      \
    { evtCommand_FIELD_CLS, 0 },                      \
    { itfCamp, 0 },                                   \
    { mnuScriptRequestMovieByIndex, 1 },              \
    { mnuScriptStopMovieAndResetDraw, 0 },            \
    { mnuUpdateMovieDrawShutdownCountdown, 0 },       \
    { scrCommand_PAD_TRIG, 1 },                       \
    { scrCommand_PAD_PRESS, 1 },                      \
    { evtCommand_CAM_PATH_WAIT, 1 },                  \
    { evtCommand_MODEL_ROT, 3 },                      \
    { evtOpTurnUnitRelativeToWorldNode, 3 },          \
    { evtOpRestoreUnitReferenceAngle, 2 },            \
    { evtCommand_CALL_SAVE, 2 },                      \
    { NULL, 0 },                                      \
    { NULL, 0 },                                      \
    { NULL, 0 },                                      \
    { evtCommand_CALL_FIELD, 3 },                     \
    { fldCmdQueryActorEntrySceneStatus, 1 },          \
    { fldCmdTestActorEntryCondition, 1 },             \
    { fldCommand_PAD_DISABLE, 0 },                    \
    { fldCommand_PAD_ENABLE, 0 },                     \
    { mnuOpenTitleMenuDefaultMode, 0 },               \
    { evtCommand_MOON_START, 0 },                     \
    { evtCommand_MOON_STOP, 0 },                      \
    { evtCommand_CHG_MOON, 1 },                       \
    { evtCommand_CALL_EVENT, 1 },                     \
    { evtCommandSubmitEvent, 1 },                     \
    { evtCommand_MODEL_PLAYER, 0 },                   \
    { evtCommand_MODEL_ON, 1 },                       \
    { evtCommand_MODEL_OFF, 1 },                      \
    { evtCommand_OBJ_PATH_MOVE, 3 },                  \
    { evtCommand_OBJ_PATH_WAIT, 1 },                  \
    { scrCommand_SET_FOVY, 1 },                       \
    { fldCommand_ENCOUNT_ON, 0 },                     \
    { fldCommand_ENCOUNT_OFF, 0 },                    \
    { scrCommandAdjustItemQuantity, 2 },              \
    { itfMesMngSetVarScr, 3 },                        \
    { scrCommandPrintString, 1 },                     \
    { evtOpSetUnitParams5, 5 },                       \
    { evtCommand_MODEL_WAIT_HOKAN, 3 },               \
    { evtCommand_CAM_CREATE, 0 },                     \
    { evtCommand_CAM_CLS, 1 },                        \
    { evtCommand_CAM_POS_XYZ, 4 },                    \
    { evtCommand_CAM_ROT_XYZ, 4 },                    \
    { evtCommand_MODEL_POS_XYZ, 4 },                  \
    { evtCommand_MODEL_ROT_XYZ, 4 },                  \
    { btlNbScriptCheckActorFlag, 1 },                 \
    { btlCmdStoreBossHealthCheckChoice, 1 },          \
    { btlScriptPickLowestHpRatioTarget, 0 },          \
    { btlCmdStoreEnemyCountCheckChoice, 1 },          \
    { btlCmdStorePartyCountCheckChoice, 1 },          \
    { btlCmdStoreActorActionMaskChoice, 1 },          \
    { btlCmdStoreEnemyActionMaskChoice, 1 },          \
    { btlCmdStorePartyActionMaskChoice, 1 },          \
    { btlCmdRememberAllLowerActionBits, 1 },          \
    { btlCmdRememberLowerUnitMode, 1 },               \
    { btlCmdRememberOtherUpperUnitMode, 1 },          \
    { btlCmdRememberLowerEntryCondition, 1 },         \
    { btlCmdRememberUpperEntryCondition, 1 },         \
    { btlAiCheckBa, 0 },                              \
    { btlCmdReturnPartyElementQueryCheck, 1 },        \
    { btlCmdReturnEnemyElementQueryCheck, 1 },        \
    { evtCommand_MODEL_MOVE2, 7 },                    \
    { btlCmdRememberActorActionGate, 0 },             \
    { evtCommand_LOAD_SOUND_E, 1 },                   \
    { evtCommand_BGM_PLAY_E, 2 },                     \
    { evtOpcodeSetBgmVolumePan, 2 },                  \
    { evtCommand_FADEOUT_E, 2 },                      \
    { evtCommand_BGM_VOL_DOWN, 0 },                   \
    { evtCommand_BGM_VOL_UP, 0 },                     \
    { itfMesMngIgnoreConfig, 1 },                     \
    { evtCommand_SEARCH_ID, 1 },                      \
    { evtOpSetUnitParams6, 6 },                       \
    { evtCommand_MODEL_WAIT_EXMOT, 3 },               \
    { evtCommand_CALL_NEXT, 3 },                      \
    { evtCommand_CALL_FIELD2, 2 },                    \
    { scrCommandResetFieldEffects, 0 },               \
    { evtCommand_EFFECT_LOAD, 1 },                    \
    { evtCommand_EFFECT_CLS, 1 },                     \
    { evtCommand_EFFECT_POS_XYZ, 4 },                 \
    { evtCommand_EFFECT_POS, 2 },                     \
    { evtCommand_EFFECT_ON, 1 },                      \
    { evtCommand_EFFECT_OFF, 1 },                     \
    { evtCommand_EFFECT_LINK, 2 },                    \
    { evtCommand_EFFECT_LINK2, 3 },                   \
    { evtCommand_EFFECT_UNLINK, 1 },                  \
    { fldCmdFocusCameraOnObject, 1 },                 \
    { fldUpdateLookAtSegment, 0 },                    \
    { bfWaitCbCreateTask, 2 },                        \
    { scrCommand_SCR_KILL, 1 },                       \
    { scrCommand_SCR_WAIT, 1 },                       \
    { scrCommand_SCR_STAT, 1 },                       \
    { scrCommand_SCR_TIME, 1 },                       \
    { evtCommand_PMV_RUN, 2 },                        \
    { scrCommand_QUAKE_ON, 2 },                       \
    { scrCommand_QUAKE_OFF, 0 },                      \
    { evtCommand_MODEL_LIGHT_OFF, 2 },                \
    { evtCommand_MODEL_LIGHT_ON, 1 },                 \
    { evtCommand_MODEL_LIGHT_RGB, 8 },                \
    { evtCommand_MODEL_LIGHT_DIR, 5 },                \
    { evtCommand_MODEL_LIGHT_WAIT, 1 },               \
    { evtCommand_MODEL_RGB, 5 },                      \
    { evtCommand_MODEL_A, 3 },                        \
    { scrCommand_SCENE_LIGHT_RGB, 4 },                \
    { scrCommand_SCENE_LIGHT_DIR, 4 },                \
    { scrCommand_SCENE_AMBIENT, 4 },                  \
    { evtCommand_PATH_STOP, 1 },                      \
    { evtCommand_PATH_START, 1 },                     \
    { evtCommand_PATH_RATE, 2 },                      \
    { evtCommand_PATH_DIR, 2 },                       \
    { scrCommand_SCENE_FOG_RGB, 4 },                  \
    { scrCommand_SCENE_FOG_PARA, 5 },                 \
    { scrCommand_BLUR, 3 },                           \
    { func_0014DAF8, 2 },                             \
    { fldCmdFreeDisplayObjects, 0 },                  \
    { fldCmdSetScenePhaseThree, 0 },                  \
    { fldCmdSetLookAtHeading, 1 },                    \
    { fldCommand_CAM_SEL_PHOKAN, 2 },                 \
    { fldCmdSetSolarOverlayMode, 1 },                 \
    { evtCommand_MODEL_LIGHT_LINK, 3 },               \
    { evtCommand_MODEL_LIGHT_UNLINK, 2 },             \
    { evtCommand_LIGHT_PATH_MOVE, 2 },                \
    { evtCommand_LIGHT_PATH_WAIT, 1 },                \
    { evtCommand_LOAD_BE, 1 },                        \
    { evtCommand_MODEL_BE, 2 },                       \
    { evtCommand_FIELD_BE, 2 },                       \
    { evtCommand_EFFECT_BE, 2 },                      \
    { evtCommand_TEX_BE, 2 },                         \
    { evtCommand_EFFBED_LOAD, 1 },                    \
    { evtCommand_EFFBED_START, 2 },                   \
    { evtCommand_EFFECT_ROT_XYZ, 4 },                 \
    { evtCommand_EFFBED_BE, 2 },                      \
    { evtCommand_EFFECT_PATH_MOVE, 2 },               \
    { evtCommand_EFFECT_PATH_WAIT, 1 },               \
    { evtCommand_EFFMG1_LOAD, 1 },                    \
    { evtCommand_EFFMG1_BE, 2 },                      \
    { evtCommand_EFFMG2_LOAD, 1 },                    \
    { evtCommand_EFFMG2_BE, 2 },                      \
    { evtCommand_EFFMG1_POS, 3 },                     \
    { evtCommand_EFFMG2_POS, 5 },                     \
    { evtCommandStartBgmWithFade, 2 },                \
    { evtCommand_CAM_ROT_Q, 5 },                      \
    { evtCommand_FIELD_ANIME_START, 1 },              \
    { evtCommand_FIELD_ANIME_STOP, 1 },               \
    { evtCommand_MODEL_SHADOW_ON2, 1 },               \
    { btlCmdRememberCounterLimit, 1 },                \
    { btlCmdStoreCounterReachedLimitChoice, 1 },      \
    { btlScriptReturnTurnCount, 0 },                  \
    { NULL, 0 },                                      \
    { btlScriptSetCommandValueAndParameter, 2 },      \
    { evtCommand_EFFMG2_PRESET, 1 },                  \
    { btlCmdReportUnsupportedTaikyo, 0 },             \
    { btlCmdReportUnsupportedAllTaikyo, 0 },          \
    { btlResetCommandContextAndSetStateFlag, 0 },     \
    { btlScriptPickLowestHpBlockTarget, 1 },          \
    { evtCommand_PATH_LOOP_ON, 1 },                   \
    { evtCommand_PATH_LOOP_OFF, 1 },                  \
    { scrCommand_MUL_BLUR_PARA, 3 },                  \
    { scrCommand_MUL_BLUR_RGBA, 4 },                  \
    { scrCommand_MUL_BLUR_AREA, 3 },                  \
    { scrCommand_MUL_BLUR_ON, 1 },                    \
    { scrCommand_MUL_BLUR_OFF, 1 },                   \
    { scrCommand_DIST_BLUR_PARA, 7 },                 \
    { scrCommand_DIST_BLUR_RGBA, 4 },                 \
    { scrCommand_DIST_BLUR_AREA, 3 },                 \
    { scrCommand_DIST_BLUR_ON, 1 },                   \
    { scrCommand_DIST_BLUR_OFF, 1 },                  \
    { btlCmdCameraMove, 7 },                          \
    { btlCmdReportUnsupportedCameraOrigin, 0 },       \
    { btlCmdSetContextFlagOne, 0 },                   \
    { evtCommand_READ_BE, 1 },                        \
    { evtCommand_CHK_BE, 1 },                         \
    { evtCommand_DEL_BE, 1 },                         \
    { btlScriptQueueActorCommandSound, 0 },           \
    { btlCmdReportUnsupportedCameraTwoShot, 0 },      \
    { btlCmdReportUnsupportedCameraBougai, 0 },       \
    { fldCommand_FLD_CHK_BTL, 0 },                    \
    { fldCommand_FLD_CHK_ANA, 0 },                    \
    { fldCommand_FLD_CHK_SISETU, 0 },                 \
    { evtCommand_CALL_FIELD3, 2 },                    \
    { fldCommand_FLD_KANKYO, 2 },                     \
    { fldCommand_FLD_FILTER, 4 },                     \
    { fldCommand_FLD_MODEL_ON, 4 },                   \
    { fldCmdApplyRoomModeGroupOne, 4 },               \
    { fldCommand_FLD_ANIME_ON, 4 },                   \
    { fldCommand_FLD_ANIME_OFF, 4 },                  \
    { fldCommand_FLD_ATARI_ON, 3 },                   \
    { fldCmdSetSceneBitsBarrierRoom, 3 },             \
    { fldCommand_FLD_MODEL_ON2, 3 },                  \
    { fldCommand_FLD_MODEL_OFF2, 3 },                 \
    { fldCommand_FLD_ANIME_ON2, 3 },                  \
    { fldCommand_FLD_ANIME_OFF2, 3 },                 \
    { fldCommand_FLD_ANIME_ID2, 4 },                  \
    { fldOpClearRoomStateFlags, 3 },                  \
    { fldOpSetRoomStateFlags, 3 },                    \
    { fldCmdSetSceneBitsValue, 4 },                   \
    { fldCommand_FLD_MAP_ON, 3 },                     \
    { fldCommand_FLD_MAP_OFF, 3 },                    \
    { fldCommand_SET_CAMTBL, 1 },                     \
    { fldCmdPushSceneParam, 1 },                      \
    { fldCommand_FLD_SET_TAKARA_OPEN, 0 },            \
    { fldCommand_FLD_CHK_TAKARA_OPEN, 0 },            \
    { itfMesMngSetMesPositionScript, 2 },             \
    { itfMesMngSetMesPosDefault, 0 },                 \
    { itfMesMngValidateNext, 1 },                     \
    { evtCommandSubmitScalarAdminRequest, 1 },        \
    { movCheckStartupSoundState, 1 },                 \
    { sndOpUpdateTitleBgmState, 0 },                  \
    { evtOpSetActiveEntryFlag, 1 },                   \
    { evtOpReadDisplayValue, 1 },                     \
    { mnuResetTitlePlaybackCommands, 0 },             \
    { mnuAdvanceTitleStreamFromScript, 0 },           \
    { mnuIsTitleStreamIdle, 0 },                      \
    { NULL, 0 },                                      \
    { evtCommand_MODEL_SHADOW_ON, 1 },                \
    { evtCommand_MODEL_SHADOW_OFF, 1 },               \
    { evtCommand_BOKASI_ON, 0 },                      \
    { evtCommand_BOKASI_OFF, 0 },                     \
    { scrCommand_MUL_BLUR_SWAP, 1 },                  \
    { scrCommand_DIST_BLUR_SWAP, 1 },                 \
    { scrCommand_BLUR2_PARA, 3 },                     \
    { scrCommand_BLUR2_RGBA, 4 },                     \
    { scrCommand_BLUR2_AREA, 2 },                     \
    { scrCommand_BLUR2_SWAP, 1 },                     \
    { scrCommand_FILTER2_PARA, 1 },                   \
    { scrCommand_FILTER2_RGBA, 4 },                   \
    { scrCommand_FILTER2_SWAP, 1 },                   \
    { scrCommand_FILTER2_ON, 1 },                     \
    { scrCommand_FILTER2_OFF, 1 },                    \
    { scrCommand_MFILTER_PARA, 1 },                   \
    { scrCommand_MFILTER_RGBA, 4 },                   \
    { scrCommand_MFILTER_AREA, 3 },                   \
    { scrCommand_MFILTER_SWAP, 1 },                   \
    { scrCommand_MFILTER_ON, 1 },                     \
    { scrCommand_MFILTER_OFF, 1 },                    \
    { evtCommand_BGM_TRANS_E, 2 },                    \
    { scrCommandAddPartyCurrency, 1 },                \
    { ptyScriptApplyEventPenalty, 1 },                \
    { itfMesMngWindowType, 1 },                       \
    { itfMesMngSelectDefKey, 2 },                     \
    { ptyScriptRestoreEntireParty, 0 },               \
    { NULL, 0 },                                      \
    { NULL, 0 },                                      \
    { NULL, 0 },                                      \
    { NULL, 0 },                                      \
    { scrCommand_RIP_BLUR_ON, 1 },                    \
    { scrCommand_RIP_BLUR_OFF, 1 },                   \
    { scrCommand_RIP_BLUR_PARA, 7 },                  \
    { scrCommand_RIP_BLUR_RGBA, 4 },                  \
    { scrCommand_RIP_BLUR_AREA, 3 },                  \
    { scrCommand_RIP_BLUR_SWAP, 1 },                  \
    { evtOpcodeRefreshTaskData, 2 },                  \
    { evtOpcodeInitializeEffectSoundChannel, 0 },     \
    { evtCommand_EFFECT_PRESET, 1 },                  \
    { btlCmdStoreActorMpCheckChoice, 1 },             \
    { btlScriptReturnContextSignedByte, 0 },          \
    { btlAiGetAllKoudo, 0 },                          \
    { btlScriptReturnActorLevel, 0 },                 \
    { NULL, 0 },                                      \
    { NULL, 0 },                                      \
    { NULL, 0 },                                      \
    { NULL, 0 },                                      \
    { btlScriptFlagAllUnitsDefeatCandidates, 0 },     \
    { btlScriptFlagPlayerDefeatCandidates, 0 },       \
    { btlScriptFlagEnemyDefeatCandidates, 0 },        \
    { btlScriptBuiltin342NoOp, 0 },                   \
    { evtCommand_CALL_STAFF, 0 },                     \
    { NULL, 0 },                                      \
    { btlScriptSelectModeAValueOne, 0 },              \
    { NULL, 0 },                                      \
    { btlScriptPickLowestLevelTarget, 0 },            \
    { ptyScriptGetRosterStat, 1 },                    \
    { btlAiGetFurea, 0 },                             \
    { evtCommand_CALL_ENDSAVE, 0 },                   \
    { evtCommand_KUTI, 3 },                           \
    { itfMesMngWindowTypeAlpha, 2 },                  \
    { NULL, 0 },                                      \
    { NULL, 0 },                                      \
    { NULL, 0 },                                      \
    { NULL, 0 },                                      \
    { evtCommand_KUTISET, 3 },                        \
    { evtCommand_CALL_BATTLE2, 1 },                   \
    { evtCommand_EFFBED_BE_EVO, 3 },                  \
    { evtCommand_EFFBED_START_EVO, 3 },               \
    { btlCmdReturnLowHpActionReadiness, 0 },          \
    { evtCommandSetDrawFlagWhileWaiting, 1 },         \
    { mnuCanDrawMovie, 0 },                           \
    { evtStoreScriptParameterResult, 1 },             \
    { evtPushFirstRosterLevel, 0 },                   \
    { evtPushSecondRosterLevel, 0 },                  \
    { evtPushFirstRosterStatEligibility, 1 },         \
    { evtPushSecondRosterStatEligibility, 1 },        \
    { evtPushSelectedStatOrRosterLowValue, 0 },       \
    { evtPushSelectedScaledStat, 0 },                 \
    { evtSelectScriptStatValue, 0 },                  \
    { evtPushSelectedTotalOrRosterHighValue, 0 },     \
    { evtPushEntryFlagBitInverted, 0 },               \
    { evtPushFirstRosterSelectedStat, 0 },            \
    { evtPushSecondRosterSelectedStat, 0 },           \
    { evtPushFirstRosterOptionStat, 0 },              \
    { evtPushSecondRosterOptionStat, 0 },             \
    { evtRollRandomScale, 1 },                        \
    { evtPushScriptContextResult, 0 },                \
    { evtPushSelectedStatMaximum, 0 },                \
    { scrGetBattleAverageActorStatForGroup, 1 },      \
    { evtPushAvailableChoiceRatio, 2 },               \
    { evtPushAvailableFloatValue, 0 },                \
    { evtPushAvailableIntegerValue, 0 },              \
    { evtPushFirstRosterCurrentHp, 0 },               \
    { evtPushSecondRosterCurrentHp, 0 },              \
    { evtPushFirstRosterMaximumHp, 0 },               \
    { evtPushSecondRosterMaximumHp, 0 },              \
    { evtCommandReattachObjectPath, 3 },              \
    { evtPushLevelMaxHpGrowth, 0 },                   \
    { evtPushLevelMaxMpGrowth, 0 },                   \
    { evtSelectStatGrade, 0 },                        \
    { evtPushLevelValueA, 0 },                        \
    { evtPushLevelValueB, 0 },                        \
    { evtScriptSelectRandomValue, 0 },                \
    { evtPushSelectedStatGrade, 0 },                  \
    { evtPushLevelValueCCommand18D, 0 },              \
    { evtPushLevelValueCCommand18E, 0 },              \
    { evtPushRequestedRosterPresence, 1 },            \
    { ptyScriptAddUnitAndReturnResult, 1 },           \
    { ptyScriptRemoveUnitAndReturnResult, 1 },        \
    { ptyScriptSetZeroResultCommand192, 2 },          \
    { ptyScriptSetOneResultCommand193, 2 },           \
    { ptyScriptSetOneResultCommand194, 1 },           \
    { evtPushRosterBaseValue, 0 },                    \
    { btlCmdReturnActorEligibility, 0 },              \
    { btlCommandSelectEventAction, 3 },               \
    { btlCommandWaitEventSoundDeadline, 2 },          \
    { btlCmdReportEnemySpawnCapacity, 0 },            \
    { btlCmdRememberLowerElementAccess, 1 },          \
    { btlCmdRememberUpperElementAccess, 1 },          \
    { btlCmdRememberLowerElementExclusion, 1 },       \
    { btlCmdRememberUpperElementExclusion, 1 },       \
    { btlCmdRememberUpperActionAvailability, 1 },     \
    { btlCmdRememberActorEntryCondition, 1 },         \
    { btlCmdRememberNonLowerStatFlag, 0 },            \
    { btlOpStartSelectedUnitModelChange, 4 },         \
    { btlCmdReturnReadyWithoutTurnsCheck, 0 },        \
    { btlCmdRememberContextSecondaryFlag, 0 },        \
    { btlAreModelChangeTasksFinished, 0 },            \
    { evtSelectFineStatGrade, 0 },                    \
    { btlCommandReportSpecialModeValue, 0 },          \
    { btlCmdStoreDefaultUnitActionChoice, 1 },        \
    { btlCmdStoreAlternateUnitActionChoice, 1 },      \
    { btlCmdStorePartyEntryCheckChoice, 1 },          \
    { btlCmdStoreInversePartyEntryChoice, 1 },        \
    { btlCmdRememberUpperEntryCodeMatch, 1 },         \
    { btlCmdRememberUpperEntryCodeInverse, 1 },       \
    { btlCmdRememberUpperSelectedElement, 1 },        \
    { btlCmdRememberLowerMissingStatus, 0 },          \
    { btlScriptPickUnlinkedPartyTarget, 0 },          \
    { btlScriptAppendActorToTargets, 0 },             \
    { btlScriptPickTargetExcludingActor, 0 },         \
    { btlCommandStartSlotMotion, 3 },                 \
    { btlCommandReportBossEffectOption, 0 },          \
    { btlCmdCheckHpPercent, 3 },                      \
    { btlScriptReturnSelectedBossEffectId, 0 },       \
    { btlScriptReturnEffectActive, 0 },               \
    { btlCmdReturnBattlePhase, 0 },                   \
    { btlCmdRememberLowerEntryCodeMismatch, 1 },      \
    { btlCmdRememberUpperEntryCodeMismatch, 1 },      \
    { btlScriptPickTargetWithoutStatusMask, 1 },      \
    { btlCmdRememberPartyQueuedActionCategory, 1 },   \
    { btlCmdTestEffectActor, 0 },                     \
    { btlScriptReturnEffectValue, 0 },                \
    { btlScriptReturnSceneSlotCount, 0 },             \
    { btlCmdReturnPartyMissingFlagCheck, 0 },         \
    { btlCmdReturnPartyHeldFlagCheck, 0 },            \
    { btlScriptPickLinkedPartyTarget, 0 },            \
    { btlScriptReturnFirstSpecialEnemySpecies, 0 },   \
    { btlCommandReportEventEffectValue, 0 },          \
    { btlScriptAddEffectActorTarget, 0 },             \
    { btlScriptReturnGlobalDebugValue, 0 },           \
    { btlCmdRememberLowerElementBlock, 1 },           \
    { btlScriptPickElementBlockTarget, 1 },           \
    { btlCmdRememberUpperQueuedQuery, 1 },            \
    { btlCmdRememberLowerQueuedQuery, 1 },            \
    { btlScriptSetActorUnitParameter, 1 },            \
    { btlScriptPickHighestMpTarget, 0 },              \
    { btlScriptPickElementCheckedTarget, 1 },         \
    { btlCmdReportPartyEmptyMp, 0 },                  \
    { btlCommandSetSequenceVolumePan, 1 },            \
    { btlCommandReportSpecialEnemyLink, 0 },          \
    { evtPushEntryIndexedStatOption, 0 },             \
    { btlCmdClearSpecialEnemyFlags, 0 },              \
    { scrGetBattleAverageMaximumValueForGroup, 1 },   \
    { scrGetBattleAverageCurrentValueForGroup, 1 },   \
    { btlScriptQueueUnboundCommandSound, 0 },         \
    { btlScriptSetCameraBlendStart, 7 },              \
    { btlScriptSetCameraBlendEnd, 7 },                \
    { btlCmdCameraMoveBlend, 2 },                     \
    { NULL, 0 },                                      \
    { NULL, 0 },                                      \
    { NULL, 0 },                                      \
    { NULL, 0 },                                      \
    { NULL, 0 },                                      \
    { NULL, 0 },                                      \
    { NULL, 0 },                                      \
    { NULL, 0 },                                      \
    { evtOpQueueWorldObjectPendingValue, 2 },         \
    { evtOpClearWorldObjectPendingValue, 1 },         \
    { evtOpModelLodChg, 2 },                          \
    { itfCommandSelectOptionWithFade, 1 },            \
    { itfPanelAcquireHold, 0 },                       \
    { itfCommandQueueHeldPanelEntry, 3 },             \
    { itfStepHeldPanelMessages, 0 },                  \
    { scrCommandClearProcessControlFlag, 0 },         \
    { scrCommandSetProcessControlFlag, 0 },           \
    { NULL, 0 },                                      \
    { NULL, 0 },                                      \
    { NULL, 0 },                                      \
    { NULL, 0 },                                      \
    { NULL, 0 },                                      \
    { NULL, 0 },                                      \
    { NULL, 0 },                                      \
    { fldCommandStartTitle, 2 },                      \
    { fldCommand_FLD_GET_KIRETA, 1 },                 \
    { fldCmdRollMirroredSolarThreshold, 0 },          \
    { fldCommand_FLD_MSE_PLAY2, 2 },                  \
    { fldCommand_FLD_MSE_STOP2, 2 },                  \
    { fldCommand_SET_NPC_CAMERA, 1 },                 \
    { fldCmdMarkBitmapRegion, 5 },                    \
    { fldCommandSendNamedSound, 1 },                  \
    { fldCommandSendSoundControl, 0 },                \
    { fldCommandIsSoundBusy, 0 },                     \
    { fldCmdGetActorStat1, 1 },                       \
    { fldCommand_FLD_WARP_HASIGO_GETTBL, 1 },         \
    { NULL, 0 },                                      \
    { NULL, 0 },                                      \
    { fldCmdGetCurrentSceneSelectionResource, 0 },    \
    { fldCommandFindEffectByName, 1 },                \
    { fldCommand_FLD_WARP_ELV_GETTBL, 1 },            \
    { fldCommand_FLD_WARP_ELV_EXIT, 1 },              \
    { fldCmdSetCurrentTaskScene, 0 },                 \
    { func_0014EF20, 0 },                             \
    { fldCommand_FLD_WARP_RUN_START, 0 },             \
    { fldCommand_FLD_INF_NEXT, 2 },                   \
    { fldCommand_FLD_INF_GET, 2 },                    \
    { fldCommand_FLD_INF_GETPANEL, 0 },               \
    { fldCmdGetActorStat0, 1 },                       \
    { fldCommand_FLD_ELV_FLOORSTART, 2 },             \
    { fldCommand_FLD_ELV_FLOOREND, 0 },               \
    { fldCommand_FLD_ELV_ENDCHK, 0 },                 \
    { fldCommand_FLD_WARP_GET_DOOR, 1 },              \
    { fldCommand_FLD_EXIT_CAMERA, 0 },                \
    { fldCommand_FLD_INF_GOWAP, 0 },                  \
    { fldCmdDispatchSceneAction, 1 },                 \
    { fldCmdStartSceneBgm, 1 },                       \
    { fldCmdStartSceneBgmAlternate, 1 },              \
    { fldCmdStopCurrentBgm, 0 },                      \
    { fldCmdPlayCurrentBgmSound, 0 },                 \
    { fldCommandPlaySeVolumePan, 1 },                 \
    { fldCommandPlaySe, 1 },                          \
    { fldCmdReleaseCurrentBgm, 0 },                   \
    { fldCommandLoadArchive, 1 },                     \
    { NULL, 0 },                                      \
    { fldCommand_FLD_WARP_GET_WARPEFF, 0 },           \
    { NULL, 0 },                                      \
    { fldCmdQueryAlternateActorEntrySceneStatus, 1 }, \
    { fldCommand_FLD_SET_ENCOUNT, 1 },                \
    { fldCommand_ACT_WIN_DSP3, 1 },                   \
    { fldCmdInitializeHuntSparkUnit, 1 },             \
    { fldCmdSetStateFlag, 0 },                        \
    { NULL, 0 },
#elif defined(VERSION_DDS2)
s32 itfMesMngMessage(void);
s32 itfMesMngOpenWindow(void);
s32 itfMesMngCloseWindow(void);
s32 itfMesMngSelect(void);
s32 itfMesMngOpenSelectWindow(void);
s32 itfMesMngCloseSelectWindow(void);
s32 itfMesMngMaskSelect(void);
s32 scrCommand_BIT_CHK(void);
s32 scrCommand_BIT_ON(void);
s32 scrCommand_BIT_OFF(void);
s32 scrCommand_RND(void);
s32 sndOpSetTrackDefaultVolumePan(void);
s32 sndOpStartTrackFromScript(void);
s32 scrCommand_SYNC(void);
s32 scrCommand_WAIT(void);
s32 bfWaitCbScreenFadeA(void);
s32 bfWaitCbScreenFadeB(void);
s32 scrCommand_SEL_JMP(void);
s32 evtCommandAddEffectUnitToWorld(void);
s32 evtCommand_CAM_PATH_MOVE(void);
s32 evtCommand_SET_SKY(void);
s32 evtCommand_MODEL_DSP(void);
s32 evtCmdDestroySelectedWorldUnit(void);
s32 evtOpStartUnitTransitionTowardWorldObject(void);
s32 evtOpDeprecatedModelMotion(void);
s32 evtCommand_CAM_SEL_FOVY(void);
s32 scrCommand_PAD_ACT(void);
s32 evtCommand_MODEL_SCL_XYZ(void);
s32 scrCommand_SCR_EXISTS(void);
s32 btlScriptSelectWeightedEntry(void);
s32 scrCommandFadeBackgroundOut(void);
s32 scrCommandFadeBackgroundIn(void);
s32 btlScriptSetChoiceFlag(void);
s32 evtCommandStartPolygonMovie(void);
s32 evtCommandClearPolygonMovieFlag(void);
s32 evtCommand_FIELD_CLS_STOP(void);
s32 scrCommandPrintInteger(void);
s32 evtCommand_GET_MOON(void);
s32 evtCommand_UNIT_CLS(void);
s32 evtCommand_GET_MOON16(void);
s32 evtCommand_RESTORE_BATTLE(void);
s32 evtCommand_UNIT_CLS_STOP(void);
s32 evtCommand_TEX_LOAD(void);
s32 evtCommand_TEX_ON(void);
s32 evtCommand_TEX_OFF(void);
s32 evtCommand_TEX_CLS(void);
s32 evtCommandSignalAdminAtTimerZero(void);
s32 btlScriptSetBattleCommand(void);
s32 btlScriptSelectEmptyCommand(void);
s32 btlScriptSelectFixedCommand(void);
s32 btlScriptSelectModeDValueOne(void);
s32 btlScriptSelectActionEntry(void);
s32 btlScriptChooseActionTargets(void);
s32 btlScriptPickLowestHpTarget(void);
s32 btlScriptPickTargetWithStatusMask(void);
s32 btlScriptFilterTargetsByUnitId(void);
s32 btlCmdReportUnsupportedEvent(void);
s32 btlCmdReturnPartyActionPresence(void);
s32 btlCmdReturnEnemyActionPresence(void);
s32 btlScriptSetBattleWorkParameter(void);
s32 btlScriptChooseActionTargetsDuplicate(void);
s32 btlCmdRunWeightedAiSelection(void);
s32 btlScriptClearSceneTransition(void);
s32 btlScriptBeginSceneTransition(void);
s32 itfMesMng_SEL_SELNO(void);
s32 scrCommand_BLUR2_ON(void);
s32 scrCommand_BLUR2_OFF(void);
s32 scrCommandResetDrawEffects(void);
s32 evtCommand_MODEL_EXWAIT(void);
s32 sndOpSendTitleAudioCommand(void);
s32 evtCommand_RTRN_TITLE(void);
s32 scrCommand_SOFTEN_BIAS(void);
s32 evtCommand_SET_SKY_A(void);
s32 evtCommand_MODEL_WAIT(void);
s32 evtCommand_MODEL_POS(void);
s32 evtCommand_MODEL_WAIT_MOT(void);
s32 evtCommand_MODEL_MOVE_MOT(void);
s32 evtCommand_FIELD_DSP(void);
s32 evtCommand_FIELD_CLS(void);
s32 itfCamp(void);
s32 mnuScriptRequestMovieByIndex(void);
s32 mnuScriptStopMovieAndResetDraw(void);
s32 mnuUpdateMovieDrawShutdownCountdown(void);
s32 scrCommand_PAD_TRIG(void);
s32 scrCommand_PAD_PRESS(void);
s32 evtCommand_CAM_PATH_WAIT(void);
s32 evtCommand_MODEL_ROT(void);
s32 evtOpTurnUnitRelativeToWorldNode(void);
s32 evtOpRestoreUnitReferenceAngle(void);
s32 evtCommand_CALL_SAVE(void);
s32 evtPushRosterOrGlobalCounterValue(void);
s32 btlCmdSetContextFlagOne(void);
s32 btlCmdRememberUpperActorTripleCheck(void);
s32 evtCommand_CALL_FIELD(void);
s32 fldCmdQueryActorEntrySceneStatus(void);
s32 fldCmdTestActorEntryCondition(void);
s32 fldCommand_PAD_DISABLE(void);
s32 fldCommand_PAD_ENABLE(void);
s32 mnuOpenTitleMenuDefaultMode(void);
s32 evtCommand_MOON_START(void);
s32 evtCommand_MOON_STOP(void);
s32 evtCommand_CHG_MOON(void);
s32 evtCommand_CALL_EVENT(void);
s32 evtCommandSubmitEvent(void);
s32 evtCommand_MODEL_PLAYER(void);
s32 evtCommand_MODEL_ON(void);
s32 evtCommand_MODEL_OFF(void);
s32 evtCommand_OBJ_PATH_MOVE(void);
s32 evtCommand_OBJ_PATH_WAIT(void);
s32 scrCommand_SET_FOVY(void);
s32 fldCommand_ENCOUNT_ON(void);
s32 fldCommand_ENCOUNT_OFF(void);
s32 scrCommandAdjustItemQuantity(void);
s32 itfMesMngSetVarScr(void);
s32 scrCommandPrintString(void);
s32 evtOpSetUnitParams5(void);
s32 evtCommand_MODEL_WAIT_HOKAN(void);
s32 evtCommand_CAM_CREATE(void);
s32 evtCommand_CAM_CLS(void);
s32 evtCommand_CAM_POS_XYZ(void);
s32 evtCommand_CAM_ROT_XYZ(void);
s32 evtCommand_MODEL_POS_XYZ(void);
s32 evtCommand_MODEL_ROT_XYZ(void);
s32 btlNbScriptCheckActorFlag(void);
s32 btlCmdStoreBossHealthCheckChoice(void);
s32 btlScriptPickLowestHpRatioTarget(void);
s32 btlCmdStoreEnemyCountCheckChoice(void);
s32 btlCmdStorePartyCountCheckChoice(void);
s32 btlCmdStoreActorActionMaskChoice(void);
s32 btlCmdStoreEnemyActionMaskChoice(void);
s32 btlCmdStorePartyActionMaskChoice(void);
s32 btlCmdRememberAllLowerActionBits(void);
s32 btlCmdRememberLowerUnitMode(void);
s32 btlCmdRememberOtherUpperUnitMode(void);
s32 btlCmdRememberLowerEntryCondition(void);
s32 btlCmdRememberUpperEntryCondition(void);
s32 btlAiCheckBa(void);
s32 btlCmdReturnPartyElementQueryCheck(void);
s32 btlCmdReturnEnemyElementQueryCheck(void);
s32 evtCommand_MODEL_MOVE2(void);
s32 btlCmdRememberActorActionGate(void);
s32 evtCommand_LOAD_SOUND_E(void);
s32 evtCommand_BGM_PLAY_E(void);
s32 evtOpcodeSetBgmVolumePan(void);
s32 evtCommand_FADEOUT_E(void);
s32 evtCommand_BGM_VOL_DOWN(void);
s32 evtCommand_BGM_VOL_UP(void);
s32 itfMesMngIgnoreConfig(void);
s32 evtCommand_SEARCH_ID(void);
s32 evtOpSetUnitParams6(void);
s32 evtScriptSetUnitTableValueAndReset(void);
s32 evtCommand_CALL_NEXT(void);
s32 evtCommand_CALL_FIELD2(void);
s32 scrCommandResetFieldEffects(void);
s32 evtCommand_EFFECT_LOAD(void);
s32 evtCommand_EFFECT_CLS(void);
s32 evtCommand_EFFECT_POS_XYZ(void);
s32 evtCommand_EFFECT_POS(void);
s32 evtCommand_EFFECT_ON(void);
s32 evtCommand_EFFECT_OFF(void);
s32 evtCommand_EFFECT_LINK(void);
s32 evtCommand_EFFECT_LINK2(void);
s32 evtCommand_EFFECT_UNLINK(void);
s32 fldCmdFocusCameraOnObject(void);
s32 fldUpdateLookAtSegment(void);
s32 bfWaitCbCreateTask(void);
s32 scrCommand_SCR_KILL(void);
s32 scrCommand_SCR_WAIT(void);
s32 scrCommand_SCR_STAT(void);
s32 scrCommand_SCR_TIME(void);
s32 evtCommand_PMV_RUN(void);
s32 scrCommand_QUAKE_ON(void);
s32 scrCommand_QUAKE_OFF(void);
s32 evtCommand_MODEL_LIGHT_OFF(void);
s32 evtCommand_MODEL_LIGHT_ON(void);
s32 evtCommand_MODEL_LIGHT_RGB(void);
s32 evtCommand_MODEL_LIGHT_DIR(void);
s32 evtCommand_MODEL_LIGHT_WAIT(void);
s32 evtCommand_MODEL_RGB(void);
s32 evtCommand_MODEL_A(void);
s32 scrCommand_SCENE_LIGHT_RGB(void);
s32 scrCommand_SCENE_LIGHT_DIR(void);
s32 scrCommand_SCENE_AMBIENT(void);
s32 evtCommand_PATH_STOP(void);
s32 evtCommand_PATH_START(void);
s32 evtCommand_PATH_RATE(void);
s32 evtCommand_PATH_DIR(void);
s32 scrCommand_SCENE_FOG_RGB(void);
s32 scrCommand_SCENE_FOG_PARA(void);
s32 scrCommand_BLUR(void);
s32 func_00154F20(void);
s32 fldCmdFreeDisplayObjects(void);
s32 fldCmdSetScenePhaseThree(void);
s32 fldCmdSetLookAtHeading(void);
s32 fldCommand_CAM_SEL_PHOKAN(void);
s32 fldCmdSetSolarOverlayMode(void);
s32 evtCommand_MODEL_LIGHT_LINK(void);
s32 evtCommand_MODEL_LIGHT_UNLINK(void);
s32 evtCommand_LIGHT_PATH_MOVE(void);
s32 evtCommand_LIGHT_PATH_WAIT(void);
s32 evtCommand_LOAD_BE(void);
s32 evtCommand_MODEL_BE(void);
s32 evtCommand_FIELD_BE(void);
s32 evtCommand_EFFECT_BE(void);
s32 evtCommand_TEX_BE(void);
s32 evtCommand_EFFBED_LOAD(void);
s32 evtCommand_EFFBED_START(void);
s32 evtCommand_EFFECT_ROT_XYZ(void);
s32 evtCommand_EFFBED_BE(void);
s32 evtCommand_EFFECT_PATH_MOVE(void);
s32 evtCommand_EFFECT_PATH_WAIT(void);
s32 evtCommand_EFFMG1_LOAD(void);
s32 evtCommand_EFFMG1_BE(void);
s32 evtCommand_EFFMG2_LOAD(void);
s32 evtCommand_EFFMG2_BE(void);
s32 evtCommand_EFFMG1_POS(void);
s32 evtCommand_EFFMG2_POS(void);
s32 evtCommandStartBgmWithFade(void);
s32 evtCommand_CAM_ROT_Q(void);
s32 evtCommand_FIELD_ANIME_START(void);
s32 evtCommand_FIELD_ANIME_STOP(void);
s32 evtCommand_MODEL_SHADOW_ON2(void);
s32 btlCmdRememberCounterLimit(void);
s32 btlCmdStoreCounterReachedLimitChoice(void);
s32 btlScriptReturnTurnCount(void);
s32 btlScriptSelectDirect(void);
s32 btlScriptSelectByKind(void);
s32 evtCommand_EFFMG2_PRESET(void);
s32 btlCmdReportUnsupportedTaikyo(void);
s32 btlCmdReportUnsupportedAllTaikyo(void);
s32 btlResetCommandContextAndSetStateFlag(void);
s32 btlScriptPickLowestHpBlockTarget(void);
s32 evtCommand_PATH_LOOP_ON(void);
s32 evtCommand_PATH_LOOP_OFF(void);
s32 scrCommand_MUL_BLUR_PARA(void);
s32 scrCommand_MUL_BLUR_RGBA(void);
s32 scrCommand_MUL_BLUR_AREA(void);
s32 scrCommand_MUL_BLUR_ON(void);
s32 scrCommand_MUL_BLUR_OFF(void);
s32 scrCommand_DIST_BLUR_PARA(void);
s32 scrCommand_DIST_BLUR_RGBA(void);
s32 scrCommand_DIST_BLUR_AREA(void);
s32 scrCommand_DIST_BLUR_ON(void);
s32 scrCommand_DIST_BLUR_OFF(void);
s32 btlCmdCameraMove(void);
s32 btlCmdReportUnsupportedCameraOrigin(void);
s32 btlEnableCommandStateFlag(void);
s32 evtCommand_READ_BE(void);
s32 evtCommand_CHK_BE(void);
s32 evtCommand_DEL_BE(void);
s32 btlScriptQueueActorCommandSound(void);
s32 btlCmdReportUnsupportedCameraTwoShot(void);
s32 btlCmdReportUnsupportedCameraBougai(void);
s32 fldCommand_FLD_CHK_BTL(void);
s32 fldCommand_FLD_CHK_ANA(void);
s32 fldCommand_FLD_CHK_SISETU(void);
s32 evtCommand_CALL_FIELD3(void);
s32 fldCommand_FLD_KANKYO(void);
s32 fldCommand_FLD_FILTER(void);
s32 fldCommand_FLD_MODEL_ON(void);
s32 fldCmdApplyRoomModeGroupOne(void);
s32 fldCommand_FLD_ANIME_ON(void);
s32 fldCommand_FLD_ANIME_OFF(void);
s32 fldCommand_FLD_ATARI_ON(void);
s32 fldCmdSetSceneBitsBarrierRoom(void);
s32 fldCommand_FLD_MODEL_ON2(void);
s32 fldCommand_FLD_MODEL_OFF2(void);
s32 fldCommand_FLD_ANIME_ON2(void);
s32 fldCommand_FLD_ANIME_OFF2(void);
s32 fldCommand_FLD_ANIME_ID2(void);
s32 fldOpClearRoomStateFlags(void);
s32 fldOpSetRoomStateFlags(void);
s32 fldCmdSetSceneBitsValue(void);
s32 fldCommand_FLD_MAP_ON(void);
s32 fldCommand_FLD_MAP_OFF(void);
s32 fldCommand_SET_CAMTBL(void);
s32 fldCmdPushSceneParam(void);
s32 fldCommand_FLD_SET_TAKARA_OPEN(void);
s32 fldCommand_FLD_CHK_TAKARA_OPEN(void);
s32 itfMesMngSetMesPositionScript(void);
s32 itfMesMngSetMesPosDefault(void);
s32 itfMesMngValidateNext(void);
s32 evtCommandSubmitScalarAdminRequest(void);
s32 movCheckStartupSoundState(void);
s32 sndOpResetTitleStreamAfterFileIdle(void);
s32 evtOpSetActiveEntryFlag(void);
s32 evtOpReadDisplayValue(void);
s32 mnuResetTitlePlaybackCommands(void);
s32 mnuAdvanceTitleStreamFromScript(void);
s32 mnuIsTitleStreamIdle(void);
s32 btlCommandReportLinkedActionScene(void);
s32 evtCommand_MODEL_SHADOW_ON(void);
s32 evtCommand_MODEL_SHADOW_OFF(void);
s32 evtCommand_BOKASI_ON(void);
s32 evtCommand_BOKASI_OFF(void);
s32 scrCommand_MUL_BLUR_SWAP(void);
s32 scrCommand_DIST_BLUR_SWAP(void);
s32 scrCommand_BLUR2_PARA(void);
s32 scrCommand_BLUR2_RGBA(void);
s32 scrCommand_BLUR2_AREA(void);
s32 scrCommand_BLUR2_SWAP(void);
s32 scrCommand_FILTER2_PARA(void);
s32 scrCommand_FILTER2_RGBA(void);
s32 scrCommand_FILTER2_SWAP(void);
s32 scrCommand_FILTER2_ON(void);
s32 scrCommand_FILTER2_OFF(void);
s32 scrCommand_MFILTER_PARA(void);
s32 scrCommand_MFILTER_RGBA(void);
s32 scrCommand_MFILTER_AREA(void);
s32 scrCommand_MFILTER_SWAP(void);
s32 scrCommand_MFILTER_ON(void);
s32 scrCommand_MFILTER_OFF(void);
s32 evtCommand_BGM_TRANS_E(void);
s32 scrCommandAddPartyCurrency(void);
s32 ptyScriptApplyEventPenalty(void);
s32 itfMesMngWindowType(void);
s32 itfMesMngSelectDefKey(void);
s32 ptyScriptRestoreEntireParty(void);
s32 btlCmdSelectChoiceSlot2Code0C(void);
s32 btlCmdRememberPartyHighHpChoice(void);
s32 scrCmdSetEntryFlagsInBothStores(void);
s32 btlCmdReturnQueuedSpecialAction(void);
s32 scrCommand_RIP_BLUR_ON(void);
s32 scrCommand_RIP_BLUR_OFF(void);
s32 scrCommand_RIP_BLUR_PARA(void);
s32 scrCommand_RIP_BLUR_RGBA(void);
s32 scrCommand_RIP_BLUR_AREA(void);
s32 scrCommand_RIP_BLUR_SWAP(void);
s32 evtOpcodeRefreshTaskData(void);
s32 evtOpcodeInitializeEffectSoundChannel(void);
s32 evtCommand_EFFECT_PRESET(void);
s32 btlCmdStoreActorMpCheckChoice(void);
s32 btlScriptReturnContextSignedByte(void);
s32 btlAiGetAllKoudo(void);
s32 btlScriptReturnActorLevel(void);
s32 btlCommandGetGroupSlotAuxValue(void);
s32 btlCommandAddOneItem(void);
s32 btlCmdReturnExtendedStatusCheck(void);
s32 btlScriptFlagAllUnitsDefeatCandidates(void);
s32 btlScriptFlagPlayerDefeatCandidates(void);
s32 btlScriptFlagEnemyDefeatCandidates(void);
s32 btlScriptReturnActorUnitParameter(void);
s32 evtCommand_CALL_STAFF(void);
s32 scrCmdHasEnoughOwnedMantras(void);
s32 btlScriptSelectModeAValueOne(void);
s32 btlScriptReturnOneBasedAiBucket(void);
s32 btlScriptPickLowestLevelTarget(void);
s32 ptyScriptGetRosterStat(void);
s32 btlAiGetFurea(void);
s32 evtCommand_CALL_ENDSAVE(void);
s32 evtCommand_KUTI(void);
s32 itfMesMngWindowTypeAlpha(void);
s32 btlCommandGetScriptReturnHookValue(void);
s32 evtPushLevelRelativeStatGradeScale(void);
s32 evtCommand_KUTISET(void);
s32 evtCommand_CALL_BATTLE2(void);
s32 evtCommand_EFFBED_BE_EVO(void);
s32 evtCommand_EFFBED_START_EVO(void);
s32 btlCmdReturnLowHpActionReadiness(void);
s32 evtCommandSetDrawFlagWhileWaiting(void);
s32 mnuCanDrawMovie(void);
s32 evtStoreScriptParameterResult(void);
s32 evtPushFirstRosterLevel(void);
s32 evtPushSecondRosterLevel(void);
s32 evtPushFirstRosterStatEligibility(void);
s32 evtPushSecondRosterStatEligibility(void);
s32 evtPushSelectedStatOrRosterLowValue(void);
s32 evtPushSelectedScaledStat(void);
s32 evtSelectScriptStatValue(void);
s32 evtPushSelectedTotalOrRosterHighValue(void);
s32 evtPushEntryFlagBitInverted(void);
s32 evtPushFirstRosterSelectedStat(void);
s32 evtPushSecondRosterSelectedStat(void);
s32 evtPushFirstRosterOptionStat(void);
s32 evtPushSecondRosterOptionStat(void);
s32 evtRollRandomScale(void);
s32 evtPushScriptContextResult(void);
s32 evtPushSelectedStatMaximum(void);
s32 scrGetBattleAverageActorStatForGroup(void);
s32 evtPushAvailableChoiceRatio(void);
s32 evtPushAvailableFloatValue(void);
s32 evtPushAvailableIntegerValue(void);
s32 evtPushFirstRosterCurrentHp(void);
s32 evtPushSecondRosterCurrentHp(void);
s32 evtPushFirstRosterMaximumHp(void);
s32 evtPushSecondRosterMaximumHp(void);
s32 evtCommandReattachObjectPath(void);
s32 evtPushLevelMaxHpGrowth(void);
s32 evtPushLevelMaxMpGrowth(void);
s32 evtSelectStatGrade(void);
s32 evtPushLevelValueA(void);
s32 evtPushLevelValueB(void);
s32 evtRollFlagDependentResultCode(void);
s32 evtPushSelectedStatGrade(void);
s32 evtPushLevelValueCCommand18D(void);
s32 evtPushLevelValueCCommand18E(void);
s32 ptyScriptCheckOrRebalanceFrontline(void);
s32 ptyScriptAddUnitAndReturnResult(void);
s32 ptyScriptRemoveUnitAndReturnResult(void);
s32 itfMesScriptStartEntryAndWait(void);
s32 ptyScriptAddUnitWithFlagsAndReturnResult(void);
s32 ptyScriptRemoveUnitWithFlagsAndReturnResult(void);
s32 evtPushRosterBaseValue(void);
s32 btlCmdReturnActorEligibility(void);
s32 btlCommandSelectEventAction(void);
s32 btlCommandWaitEventSoundDeadline(void);
s32 btlCmdReportEnemySpawnCapacity(void);
s32 btlCmdRememberLowerElementAccess(void);
s32 btlCmdRememberUpperElementAccess(void);
s32 btlCmdRememberLowerElementExclusion(void);
s32 btlCmdRememberUpperElementExclusion(void);
s32 btlCmdRememberUpperActionAvailability(void);
s32 btlCmdRememberActorEntryCondition(void);
s32 btlCmdRememberNonLowerStatFlag(void);
s32 btlOpStartSelectedUnitModelChange(void);
s32 btlCmdReturnReadyWithoutTurnsCheck(void);
s32 btlCmdRememberContextSecondaryFlag(void);
s32 btlAreModelChangeTasksFinished(void);
s32 evtSelectFineStatGrade(void);
s32 btlScriptBuiltin422NoOp(void);
s32 btlCmdStoreDefaultUnitActionChoice(void);
s32 btlCmdStoreAlternateUnitActionChoice(void);
s32 btlCmdStorePartyEntryCheckChoice(void);
s32 btlCmdStoreInversePartyEntryChoice(void);
s32 btlCmdRememberUpperEntryCodeMatch(void);
s32 btlCmdRememberUpperEntryCodeInverse(void);
s32 btlCmdRememberUpperSelectedElement(void);
s32 btlCmdRememberLowerMissingStatus(void);
s32 btlScriptPickUnlinkedPartyTarget(void);
s32 btlScriptAppendActorToTargets(void);
s32 btlScriptPickTargetExcludingActor(void);
s32 btlCommandPlayUnitMotion(void);
s32 btlScriptBuiltin435NoOp(void);
s32 btlScriptReturnUnitHpRatioPercent(void);
s32 btlScriptBuiltin437NoOp(void);
s32 btlScriptReturnEffectActive(void);
s32 btlCmdReturnBattlePhase(void);
s32 btlCmdRememberLowerEntryCodeMismatch(void);
s32 btlCmdRememberUpperEntryCodeMismatch(void);
s32 btlScriptPickTargetWithoutStatusMask(void);
s32 btlCmdRememberPartyQueuedActionCategory(void);
s32 btlCmdTestEffectActor(void);
s32 btlScriptReturnEffectValue(void);
s32 btlScriptReturnSceneSlotCount(void);
s32 btlCmdReturnPartyMissingFlagCheck(void);
s32 btlCmdReturnPartyHeldFlagCheck(void);
s32 btlScriptPickLinkedPartyTarget(void);
s32 btlScriptBuiltin450NoOp(void);
s32 btlScriptBuiltin451NoOp(void);
s32 btlScriptAddEffectActorTarget(void);
s32 btlScriptReturnGlobalDebugValue(void);
s32 btlCmdRememberLowerElementBlock(void);
s32 btlScriptPickElementBlockTarget(void);
s32 btlCmdRememberUpperQueuedQuery(void);
s32 btlCmdRememberLowerQueuedQuery(void);
s32 btlScriptSetActorUnitParameter(void);
s32 btlScriptPickHighestMpTarget(void);
s32 btlScriptPickElementCheckedTarget(void);
s32 btlCmdReportPartyEmptyMp(void);
s32 btlCommandSetSequenceVolumePan(void);
s32 btlCommandReportSpecialEnemyLink(void);
s32 evtPushEntryIndexedStatOption(void);
s32 btlCmdClearSpecialEnemyFlags(void);
s32 scrGetBattleAverageMaximumValueForGroup(void);
s32 scrGetBattleAverageCurrentValueForGroup(void);
s32 btlScriptQueueUnboundCommandSound(void);
s32 btlScriptSetCameraBlendStart(void);
s32 btlScriptSetCameraBlendEnd(void);
s32 btlCmdCameraMoveBlend(void);
s32 btlScriptReturnActiveSubtaskPresence(void);
s32 btlCmdGetSubtaskTargetMode(void);
s32 btlScriptCmdSelectTargetsByMode(void);
s32 btlScriptReturnMarkedCommandRequestedState(void);
s32 btlScriptAppendSelectedOrCurrentTarget(void);
s32 btlScriptSelectShadowSkill(void);
s32 btlScriptReturnMarkedActionSceneActive(void);
s32 evtCallShooting(void);
s32 evtOpQueueWorldObjectPendingValue(void);
s32 evtOpClearWorldObjectPendingValue(void);
s32 evtOpModelLodChg(void);
s32 itfCommandSelectOptionWithFade(void);
s32 itfPanelAcquireHold(void);
s32 itfCommandQueueHeldPanelEntry(void);
s32 itfStepHeldPanelMessages(void);
s32 scrCommandClearProcessControlFlag(void);
s32 scrCommandSetProcessControlFlag(void);
s32 btlScriptSetBossStatSelector(void);
s32 btlCmdSpawnEffectTaskForSelectedUnit(void);
s32 btlCommandChooseRandomUnflaggedGroupSlot(void);
s32 btlCmdGetIndexedBattleStateValue(void);
s32 btlCommandCountModelFlags(void);
s32 fldCommandStartTitle(void);
s32 fldCommand_FLD_GET_KIRETA(void);
s32 fldCmdRollMirroredSolarThreshold(void);
s32 fldCommand_FLD_MSE_PLAY2(void);
s32 fldCommand_FLD_MSE_STOP2(void);
s32 fldCommand_SET_NPC_CAMERA(void);
s32 fldCmdMarkBitmapRegion(void);
s32 fldCommand_FLD_MODEL_DANTE(void);
s32 fldCmdSetTargetGuideEnabled(void);
s32 fldCmdSetTargetGuideMode(void);
s32 fldCmdGetActorStat1(void);
s32 fldCommand_FLD_WARP_HASIGO_GETTBL(void);
s32 fldCmdApplyNumericRoomModeGroupZero(void);
s32 fldCmdApplyNumericRoomModeGroupOne(void);
s32 fldCmdGetCurrentSceneSelectionResource(void);
s32 fldCommandFindEffectByName(void);
s32 fldCommand_FLD_WARP_ELV_GETTBL(void);
s32 fldCommand_FLD_WARP_ELV_EXIT(void);
s32 fldCmdSetCurrentTaskScene(void);
s32 func_001569E0(void);
s32 fldCommand_FLD_WARP_RUN_START(void);
s32 fldCommand_FLD_INF_NEXT(void);
s32 fldCommand_FLD_INF_GET(void);
s32 fldCommand_FLD_INF_GETPANEL(void);
s32 fldCmdGetActorStat0(void);
s32 fldCommand_FLD_ELV_FLOORSTART(void);
s32 fldCommand_FLD_ELV_FLOOREND(void);
s32 fldCommand_FLD_ELV_ENDCHK(void);
s32 fldCommand_FLD_WARP_GET_DOOR(void);
s32 fldCommand_FLD_EXIT_CAMERA(void);
s32 fldCommand_FLD_INF_GOWAP(void);
s32 fldCmdExecuteSceneAction(void);
s32 fldCmdStartSceneBgm(void);
s32 fldCmdStartSceneBgmAlternate(void);
s32 fldCmdStopCurrentBgm(void);
s32 fldCmdPlayCurrentBgmSound(void);
s32 fldCommandPlaySeVolumePan(void);
s32 fldCommandPlaySe(void);
s32 fldCmdReleaseCurrentBgm(void);
s32 fldCommandLoadArchive(void);
s32 fldCmdForceTargetGuideUpdate(void);
s32 fldCommand_FLD_WARP_GET_WARPEFF(void);
s32 fldCmdResetAfterEvent(void);
s32 fldCmdQueryAlternateActorEntrySceneStatus(void);
s32 fldCommand_FLD_SET_ENCOUNT(void);
s32 fldCommand_ACT_WIN_DSP3(void);
s32 fldCmdInitializeHuntSparkUnit(void);
s32 fldCmdSetStateFlag(void);

#define SCR_COMMAND_TABLE_ENTRIES \
    { itfMesMngMessage, 1 },                            \
    { itfMesMngOpenWindow, 0 },                         \
    { itfMesMngCloseWindow, 0 },                        \
    { itfMesMngSelect, 1 },                             \
    { itfMesMngOpenSelectWindow, 0 },                   \
    { itfMesMngCloseSelectWindow, 0 },                  \
    { itfMesMngMaskSelect, 1 },                         \
    { scrCommand_BIT_CHK, 1 },                          \
    { scrCommand_BIT_ON, 1 },                           \
    { scrCommand_BIT_OFF, 1 },                          \
    { scrCommand_RND, 1 },                              \
    { sndOpSetTrackDefaultVolumePan, 1 },               \
    { sndOpStartTrackFromScript, 1 },                   \
    { scrCommand_SYNC, 0 },                             \
    { scrCommand_WAIT, 1 },                             \
    { bfWaitCbScreenFadeA, 2 },                         \
    { bfWaitCbScreenFadeB, 2 },                         \
    { scrCommand_SEL_JMP, 8 },                          \
    { evtCommandAddEffectUnitToWorld, 1 },              \
    { evtCommand_CAM_PATH_MOVE, 2 },                    \
    { evtCommand_SET_SKY, 3 },                          \
    { evtCommand_MODEL_DSP, 2 },                        \
    { evtCmdDestroySelectedWorldUnit, 1 },              \
    { evtOpStartUnitTransitionTowardWorldObject, 5 },   \
    { evtOpDeprecatedModelMotion, 4 },                  \
    { evtCommand_CAM_SEL_FOVY, 1 },                     \
    { scrCommand_PAD_ACT, 3 },                          \
    { evtCommand_MODEL_SCL_XYZ, 4 },                    \
    { scrCommand_SCR_EXISTS, 1 },                       \
    { btlScriptSelectWeightedEntry, 1 },                \
    { scrCommandFadeBackgroundOut, 1 },                 \
    { scrCommandFadeBackgroundIn, 1 },                  \
    { btlScriptSetChoiceFlag, 2 },                      \
    { evtCommandStartPolygonMovie, 2 },                 \
    { evtCommandClearPolygonMovieFlag, 1 },             \
    { evtCommand_FIELD_CLS_STOP, 0 },                   \
    { scrCommandPrintInteger, 1 },                      \
    { evtCommand_GET_MOON, 0 },                         \
    { evtCommand_UNIT_CLS, 0 },                         \
    { evtCommand_GET_MOON16, 0 },                       \
    { evtCommand_RESTORE_BATTLE, 2 },                   \
    { evtCommand_UNIT_CLS_STOP, 0 },                    \
    { evtCommand_TEX_LOAD, 1 },                         \
    { evtCommand_TEX_ON, 1 },                           \
    { evtCommand_TEX_OFF, 1 },                          \
    { evtCommand_TEX_CLS, 1 },                          \
    { evtCommandSignalAdminAtTimerZero, 0 },            \
    { btlScriptSetBattleCommand, 1 },                   \
    { btlScriptSelectEmptyCommand, 0 },                 \
    { btlScriptSelectFixedCommand, 0 },                 \
    { btlScriptSelectModeDValueOne, 0 },                \
    { btlScriptSelectActionEntry, 1 },                  \
    { btlScriptChooseActionTargets, 0 },                \
    { btlScriptPickLowestHpTarget, 0 },                 \
    { btlScriptPickTargetWithStatusMask, 1 },           \
    { btlScriptFilterTargetsByUnitId, 1 },              \
    { btlCmdReportUnsupportedEvent, 1 },                \
    { btlCmdReturnPartyActionPresence, 1 },             \
    { btlCmdReturnEnemyActionPresence, 1 },             \
    { btlScriptSetBattleWorkParameter, 1 },             \
    { btlScriptChooseActionTargetsDuplicate, 0 },       \
    { btlCmdRunWeightedAiSelection, 0 },                \
    { btlScriptClearSceneTransition, 0 },               \
    { btlScriptBeginSceneTransition, 0 },               \
    { itfMesMng_SEL_SELNO, 1 },                         \
    { scrCommand_BLUR2_ON, 1 },                         \
    { scrCommand_BLUR2_OFF, 1 },                        \
    { scrCommandResetDrawEffects, 0 },                  \
    { evtCommand_MODEL_EXWAIT, 2 },                     \
    { sndOpSendTitleAudioCommand, 0 },                  \
    { evtCommand_RTRN_TITLE, 0 },                       \
    { scrCommand_SOFTEN_BIAS, 3 },                      \
    { evtCommand_SET_SKY_A, 2 },                        \
    { evtCommand_MODEL_WAIT, 1 },                       \
    { evtCommand_MODEL_POS, 2 },                        \
    { evtCommand_MODEL_WAIT_MOT, 2 },                   \
    { evtCommand_MODEL_MOVE_MOT, 2 },                   \
    { evtCommand_FIELD_DSP, 2 },                        \
    { evtCommand_FIELD_CLS, 0 },                        \
    { itfCamp, 0 },                                     \
    { mnuScriptRequestMovieByIndex, 1 },                \
    { mnuScriptStopMovieAndResetDraw, 0 },              \
    { mnuUpdateMovieDrawShutdownCountdown, 0 },         \
    { scrCommand_PAD_TRIG, 1 },                         \
    { scrCommand_PAD_PRESS, 1 },                        \
    { evtCommand_CAM_PATH_WAIT, 1 },                    \
    { evtCommand_MODEL_ROT, 3 },                        \
    { evtOpTurnUnitRelativeToWorldNode, 3 },            \
    { evtOpRestoreUnitReferenceAngle, 2 },              \
    { evtCommand_CALL_SAVE, 2 },                        \
    { evtPushRosterOrGlobalCounterValue, 0 },           \
    { btlCmdSetContextFlagOne, 0 },                     \
    { btlCmdRememberUpperActorTripleCheck, 1 },         \
    { evtCommand_CALL_FIELD, 3 },                       \
    { fldCmdQueryActorEntrySceneStatus, 1 },            \
    { fldCmdTestActorEntryCondition, 1 },               \
    { fldCommand_PAD_DISABLE, 0 },                      \
    { fldCommand_PAD_ENABLE, 0 },                       \
    { mnuOpenTitleMenuDefaultMode, 0 },                 \
    { evtCommand_MOON_START, 0 },                       \
    { evtCommand_MOON_STOP, 0 },                        \
    { evtCommand_CHG_MOON, 1 },                         \
    { evtCommand_CALL_EVENT, 1 },                       \
    { evtCommandSubmitEvent, 1 },                       \
    { evtCommand_MODEL_PLAYER, 0 },                     \
    { evtCommand_MODEL_ON, 1 },                         \
    { evtCommand_MODEL_OFF, 1 },                        \
    { evtCommand_OBJ_PATH_MOVE, 3 },                    \
    { evtCommand_OBJ_PATH_WAIT, 1 },                    \
    { scrCommand_SET_FOVY, 1 },                         \
    { fldCommand_ENCOUNT_ON, 0 },                       \
    { fldCommand_ENCOUNT_OFF, 0 },                      \
    { scrCommandAdjustItemQuantity, 2 },                \
    { itfMesMngSetVarScr, 3 },                          \
    { scrCommandPrintString, 1 },                       \
    { evtOpSetUnitParams5, 5 },                         \
    { evtCommand_MODEL_WAIT_HOKAN, 3 },                 \
    { evtCommand_CAM_CREATE, 0 },                       \
    { evtCommand_CAM_CLS, 1 },                          \
    { evtCommand_CAM_POS_XYZ, 4 },                      \
    { evtCommand_CAM_ROT_XYZ, 4 },                      \
    { evtCommand_MODEL_POS_XYZ, 4 },                    \
    { evtCommand_MODEL_ROT_XYZ, 4 },                    \
    { btlNbScriptCheckActorFlag, 1 },                   \
    { btlCmdStoreBossHealthCheckChoice, 1 },            \
    { btlScriptPickLowestHpRatioTarget, 0 },            \
    { btlCmdStoreEnemyCountCheckChoice, 1 },            \
    { btlCmdStorePartyCountCheckChoice, 1 },            \
    { btlCmdStoreActorActionMaskChoice, 1 },            \
    { btlCmdStoreEnemyActionMaskChoice, 1 },            \
    { btlCmdStorePartyActionMaskChoice, 1 },            \
    { btlCmdRememberAllLowerActionBits, 1 },            \
    { btlCmdRememberLowerUnitMode, 1 },                 \
    { btlCmdRememberOtherUpperUnitMode, 1 },            \
    { btlCmdRememberLowerEntryCondition, 1 },           \
    { btlCmdRememberUpperEntryCondition, 1 },           \
    { btlAiCheckBa, 0 },                                \
    { btlCmdReturnPartyElementQueryCheck, 1 },          \
    { btlCmdReturnEnemyElementQueryCheck, 1 },          \
    { evtCommand_MODEL_MOVE2, 7 },                      \
    { btlCmdRememberActorActionGate, 0 },               \
    { evtCommand_LOAD_SOUND_E, 1 },                     \
    { evtCommand_BGM_PLAY_E, 2 },                       \
    { evtOpcodeSetBgmVolumePan, 2 },                    \
    { evtCommand_FADEOUT_E, 2 },                        \
    { evtCommand_BGM_VOL_DOWN, 0 },                     \
    { evtCommand_BGM_VOL_UP, 0 },                       \
    { itfMesMngIgnoreConfig, 1 },                       \
    { evtCommand_SEARCH_ID, 1 },                        \
    { evtOpSetUnitParams6, 6 },                         \
    { evtScriptSetUnitTableValueAndReset, 3 },          \
    { evtCommand_CALL_NEXT, 3 },                        \
    { evtCommand_CALL_FIELD2, 2 },                      \
    { scrCommandResetFieldEffects, 0 },                 \
    { evtCommand_EFFECT_LOAD, 1 },                      \
    { evtCommand_EFFECT_CLS, 1 },                       \
    { evtCommand_EFFECT_POS_XYZ, 4 },                   \
    { evtCommand_EFFECT_POS, 2 },                       \
    { evtCommand_EFFECT_ON, 1 },                        \
    { evtCommand_EFFECT_OFF, 1 },                       \
    { evtCommand_EFFECT_LINK, 2 },                      \
    { evtCommand_EFFECT_LINK2, 3 },                     \
    { evtCommand_EFFECT_UNLINK, 1 },                    \
    { fldCmdFocusCameraOnObject, 1 },                   \
    { fldUpdateLookAtSegment, 0 },                      \
    { bfWaitCbCreateTask, 2 },                          \
    { scrCommand_SCR_KILL, 1 },                         \
    { scrCommand_SCR_WAIT, 1 },                         \
    { scrCommand_SCR_STAT, 1 },                         \
    { scrCommand_SCR_TIME, 1 },                         \
    { evtCommand_PMV_RUN, 2 },                          \
    { scrCommand_QUAKE_ON, 2 },                         \
    { scrCommand_QUAKE_OFF, 0 },                        \
    { evtCommand_MODEL_LIGHT_OFF, 2 },                  \
    { evtCommand_MODEL_LIGHT_ON, 1 },                   \
    { evtCommand_MODEL_LIGHT_RGB, 8 },                  \
    { evtCommand_MODEL_LIGHT_DIR, 5 },                  \
    { evtCommand_MODEL_LIGHT_WAIT, 1 },                 \
    { evtCommand_MODEL_RGB, 5 },                        \
    { evtCommand_MODEL_A, 3 },                          \
    { scrCommand_SCENE_LIGHT_RGB, 4 },                  \
    { scrCommand_SCENE_LIGHT_DIR, 4 },                  \
    { scrCommand_SCENE_AMBIENT, 4 },                    \
    { evtCommand_PATH_STOP, 1 },                        \
    { evtCommand_PATH_START, 1 },                       \
    { evtCommand_PATH_RATE, 2 },                        \
    { evtCommand_PATH_DIR, 2 },                         \
    { scrCommand_SCENE_FOG_RGB, 4 },                    \
    { scrCommand_SCENE_FOG_PARA, 5 },                   \
    { scrCommand_BLUR, 3 },                             \
    { func_00154F20, 2 },                               \
    { fldCmdFreeDisplayObjects, 0 },                    \
    { fldCmdSetScenePhaseThree, 0 },                    \
    { fldCmdSetLookAtHeading, 1 },                      \
    { fldCommand_CAM_SEL_PHOKAN, 2 },                   \
    { fldCmdSetSolarOverlayMode, 1 },                   \
    { evtCommand_MODEL_LIGHT_LINK, 3 },                 \
    { evtCommand_MODEL_LIGHT_UNLINK, 2 },               \
    { evtCommand_LIGHT_PATH_MOVE, 2 },                  \
    { evtCommand_LIGHT_PATH_WAIT, 1 },                  \
    { evtCommand_LOAD_BE, 1 },                          \
    { evtCommand_MODEL_BE, 2 },                         \
    { evtCommand_FIELD_BE, 2 },                         \
    { evtCommand_EFFECT_BE, 2 },                        \
    { evtCommand_TEX_BE, 2 },                           \
    { evtCommand_EFFBED_LOAD, 1 },                      \
    { evtCommand_EFFBED_START, 2 },                     \
    { evtCommand_EFFECT_ROT_XYZ, 4 },                   \
    { evtCommand_EFFBED_BE, 2 },                        \
    { evtCommand_EFFECT_PATH_MOVE, 2 },                 \
    { evtCommand_EFFECT_PATH_WAIT, 1 },                 \
    { evtCommand_EFFMG1_LOAD, 1 },                      \
    { evtCommand_EFFMG1_BE, 2 },                        \
    { evtCommand_EFFMG2_LOAD, 1 },                      \
    { evtCommand_EFFMG2_BE, 2 },                        \
    { evtCommand_EFFMG1_POS, 3 },                       \
    { evtCommand_EFFMG2_POS, 5 },                       \
    { evtCommandStartBgmWithFade, 2 },                  \
    { evtCommand_CAM_ROT_Q, 5 },                        \
    { evtCommand_FIELD_ANIME_START, 1 },                \
    { evtCommand_FIELD_ANIME_STOP, 1 },                 \
    { evtCommand_MODEL_SHADOW_ON2, 1 },                 \
    { btlCmdRememberCounterLimit, 1 },                  \
    { btlCmdStoreCounterReachedLimitChoice, 1 },        \
    { btlScriptReturnTurnCount, 0 },                    \
    { btlScriptSelectDirect, 1 },                       \
    { btlScriptSelectByKind, 2 },                       \
    { evtCommand_EFFMG2_PRESET, 1 },                    \
    { btlCmdReportUnsupportedTaikyo, 0 },               \
    { btlCmdReportUnsupportedAllTaikyo, 0 },            \
    { btlResetCommandContextAndSetStateFlag, 0 },       \
    { btlScriptPickLowestHpBlockTarget, 1 },            \
    { evtCommand_PATH_LOOP_ON, 1 },                     \
    { evtCommand_PATH_LOOP_OFF, 1 },                    \
    { scrCommand_MUL_BLUR_PARA, 3 },                    \
    { scrCommand_MUL_BLUR_RGBA, 4 },                    \
    { scrCommand_MUL_BLUR_AREA, 3 },                    \
    { scrCommand_MUL_BLUR_ON, 1 },                      \
    { scrCommand_MUL_BLUR_OFF, 1 },                     \
    { scrCommand_DIST_BLUR_PARA, 7 },                   \
    { scrCommand_DIST_BLUR_RGBA, 4 },                   \
    { scrCommand_DIST_BLUR_AREA, 3 },                   \
    { scrCommand_DIST_BLUR_ON, 1 },                     \
    { scrCommand_DIST_BLUR_OFF, 1 },                    \
    { btlCmdCameraMove, 7 },                            \
    { btlCmdReportUnsupportedCameraOrigin, 0 },         \
    { btlEnableCommandStateFlag, 0 },                   \
    { evtCommand_READ_BE, 1 },                          \
    { evtCommand_CHK_BE, 1 },                           \
    { evtCommand_DEL_BE, 1 },                           \
    { btlScriptQueueActorCommandSound, 0 },             \
    { btlCmdReportUnsupportedCameraTwoShot, 0 },        \
    { btlCmdReportUnsupportedCameraBougai, 0 },         \
    { fldCommand_FLD_CHK_BTL, 0 },                      \
    { fldCommand_FLD_CHK_ANA, 0 },                      \
    { fldCommand_FLD_CHK_SISETU, 0 },                   \
    { evtCommand_CALL_FIELD3, 2 },                      \
    { fldCommand_FLD_KANKYO, 2 },                       \
    { fldCommand_FLD_FILTER, 4 },                       \
    { fldCommand_FLD_MODEL_ON, 4 },                     \
    { fldCmdApplyRoomModeGroupOne, 4 },                 \
    { fldCommand_FLD_ANIME_ON, 4 },                     \
    { fldCommand_FLD_ANIME_OFF, 4 },                    \
    { fldCommand_FLD_ATARI_ON, 3 },                     \
    { fldCmdSetSceneBitsBarrierRoom, 3 },               \
    { fldCommand_FLD_MODEL_ON2, 3 },                    \
    { fldCommand_FLD_MODEL_OFF2, 3 },                   \
    { fldCommand_FLD_ANIME_ON2, 3 },                    \
    { fldCommand_FLD_ANIME_OFF2, 3 },                   \
    { fldCommand_FLD_ANIME_ID2, 4 },                    \
    { fldOpClearRoomStateFlags, 3 },                    \
    { fldOpSetRoomStateFlags, 3 },                      \
    { fldCmdSetSceneBitsValue, 4 },                     \
    { fldCommand_FLD_MAP_ON, 3 },                       \
    { fldCommand_FLD_MAP_OFF, 3 },                      \
    { fldCommand_SET_CAMTBL, 1 },                       \
    { fldCmdPushSceneParam, 1 },                        \
    { fldCommand_FLD_SET_TAKARA_OPEN, 0 },              \
    { fldCommand_FLD_CHK_TAKARA_OPEN, 0 },              \
    { itfMesMngSetMesPositionScript, 2 },               \
    { itfMesMngSetMesPosDefault, 0 },                   \
    { itfMesMngValidateNext, 1 },                       \
    { evtCommandSubmitScalarAdminRequest, 1 },          \
    { movCheckStartupSoundState, 1 },                   \
    { sndOpResetTitleStreamAfterFileIdle, 0 },          \
    { evtOpSetActiveEntryFlag, 1 },                     \
    { evtOpReadDisplayValue, 1 },                       \
    { mnuResetTitlePlaybackCommands, 0 },               \
    { mnuAdvanceTitleStreamFromScript, 0 },             \
    { mnuIsTitleStreamIdle, 0 },                        \
    { btlCommandReportLinkedActionScene, 0 },           \
    { evtCommand_MODEL_SHADOW_ON, 1 },                  \
    { evtCommand_MODEL_SHADOW_OFF, 1 },                 \
    { evtCommand_BOKASI_ON, 0 },                        \
    { evtCommand_BOKASI_OFF, 0 },                       \
    { scrCommand_MUL_BLUR_SWAP, 1 },                    \
    { scrCommand_DIST_BLUR_SWAP, 1 },                   \
    { scrCommand_BLUR2_PARA, 3 },                       \
    { scrCommand_BLUR2_RGBA, 4 },                       \
    { scrCommand_BLUR2_AREA, 2 },                       \
    { scrCommand_BLUR2_SWAP, 1 },                       \
    { scrCommand_FILTER2_PARA, 1 },                     \
    { scrCommand_FILTER2_RGBA, 4 },                     \
    { scrCommand_FILTER2_SWAP, 1 },                     \
    { scrCommand_FILTER2_ON, 1 },                       \
    { scrCommand_FILTER2_OFF, 1 },                      \
    { scrCommand_MFILTER_PARA, 1 },                     \
    { scrCommand_MFILTER_RGBA, 4 },                     \
    { scrCommand_MFILTER_AREA, 3 },                     \
    { scrCommand_MFILTER_SWAP, 1 },                     \
    { scrCommand_MFILTER_ON, 1 },                       \
    { scrCommand_MFILTER_OFF, 1 },                      \
    { evtCommand_BGM_TRANS_E, 2 },                      \
    { scrCommandAddPartyCurrency, 1 },                  \
    { ptyScriptApplyEventPenalty, 1 },                  \
    { itfMesMngWindowType, 1 },                         \
    { itfMesMngSelectDefKey, 2 },                       \
    { ptyScriptRestoreEntireParty, 0 },                 \
    { btlCmdSelectChoiceSlot2Code0C, 1 },               \
    { btlCmdRememberPartyHighHpChoice, 1 },             \
    { scrCmdSetEntryFlagsInBothStores, 2 },             \
    { btlCmdReturnQueuedSpecialAction, 0 },             \
    { scrCommand_RIP_BLUR_ON, 1 },                      \
    { scrCommand_RIP_BLUR_OFF, 1 },                     \
    { scrCommand_RIP_BLUR_PARA, 7 },                    \
    { scrCommand_RIP_BLUR_RGBA, 4 },                    \
    { scrCommand_RIP_BLUR_AREA, 3 },                    \
    { scrCommand_RIP_BLUR_SWAP, 1 },                    \
    { evtOpcodeRefreshTaskData, 2 },                    \
    { evtOpcodeInitializeEffectSoundChannel, 0 },       \
    { evtCommand_EFFECT_PRESET, 1 },                    \
    { btlCmdStoreActorMpCheckChoice, 1 },               \
    { btlScriptReturnContextSignedByte, 0 },            \
    { btlAiGetAllKoudo, 0 },                            \
    { btlScriptReturnActorLevel, 0 },                   \
    { btlCommandGetGroupSlotAuxValue, 1 },              \
    { btlCommandAddOneItem, 1 },                        \
    { btlCmdReturnExtendedStatusCheck, 1 },             \
    { NULL, 0 },                                        \
    { btlScriptFlagAllUnitsDefeatCandidates, 0 },       \
    { btlScriptFlagPlayerDefeatCandidates, 0 },         \
    { btlScriptFlagEnemyDefeatCandidates, 0 },          \
    { btlScriptReturnActorUnitParameter, 0 },           \
    { evtCommand_CALL_STAFF, 0 },                       \
    { scrCmdHasEnoughOwnedMantras, 0 },                 \
    { btlScriptSelectModeAValueOne, 0 },                \
    { btlScriptReturnOneBasedAiBucket, 0 },             \
    { btlScriptPickLowestLevelTarget, 0 },              \
    { ptyScriptGetRosterStat, 1 },                      \
    { btlAiGetFurea, 0 },                               \
    { evtCommand_CALL_ENDSAVE, 0 },                     \
    { evtCommand_KUTI, 3 },                             \
    { itfMesMngWindowTypeAlpha, 2 },                    \
    { btlCommandGetScriptReturnHookValue, 0 },          \
    { evtPushLevelRelativeStatGradeScale, 0 },          \
    { NULL, 0 },                                        \
    { NULL, 0 },                                        \
    { evtCommand_KUTISET, 3 },                          \
    { evtCommand_CALL_BATTLE2, 1 },                     \
    { evtCommand_EFFBED_BE_EVO, 3 },                    \
    { evtCommand_EFFBED_START_EVO, 3 },                 \
    { btlCmdReturnLowHpActionReadiness, 0 },            \
    { evtCommandSetDrawFlagWhileWaiting, 1 },           \
    { mnuCanDrawMovie, 0 },                             \
    { evtStoreScriptParameterResult, 1 },               \
    { evtPushFirstRosterLevel, 0 },                     \
    { evtPushSecondRosterLevel, 0 },                    \
    { evtPushFirstRosterStatEligibility, 1 },           \
    { evtPushSecondRosterStatEligibility, 1 },          \
    { evtPushSelectedStatOrRosterLowValue, 0 },         \
    { evtPushSelectedScaledStat, 0 },                   \
    { evtSelectScriptStatValue, 0 },                    \
    { evtPushSelectedTotalOrRosterHighValue, 0 },       \
    { evtPushEntryFlagBitInverted, 0 },                 \
    { evtPushFirstRosterSelectedStat, 0 },              \
    { evtPushSecondRosterSelectedStat, 0 },             \
    { evtPushFirstRosterOptionStat, 0 },                \
    { evtPushSecondRosterOptionStat, 0 },               \
    { evtRollRandomScale, 1 },                          \
    { evtPushScriptContextResult, 0 },                  \
    { evtPushSelectedStatMaximum, 0 },                  \
    { scrGetBattleAverageActorStatForGroup, 1 },        \
    { evtPushAvailableChoiceRatio, 2 },                 \
    { evtPushAvailableFloatValue, 0 },                  \
    { evtPushAvailableIntegerValue, 0 },                \
    { evtPushFirstRosterCurrentHp, 0 },                 \
    { evtPushSecondRosterCurrentHp, 0 },                \
    { evtPushFirstRosterMaximumHp, 0 },                 \
    { evtPushSecondRosterMaximumHp, 0 },                \
    { evtCommandReattachObjectPath, 3 },                \
    { evtPushLevelMaxHpGrowth, 0 },                     \
    { evtPushLevelMaxMpGrowth, 0 },                     \
    { evtSelectStatGrade, 0 },                          \
    { evtPushLevelValueA, 0 },                          \
    { evtPushLevelValueB, 0 },                          \
    { evtRollFlagDependentResultCode, 0 },              \
    { evtPushSelectedStatGrade, 0 },                    \
    { evtPushLevelValueCCommand18D, 0 },                \
    { evtPushLevelValueCCommand18E, 0 },                \
    { ptyScriptCheckOrRebalanceFrontline, 2 },          \
    { ptyScriptAddUnitAndReturnResult, 1 },             \
    { ptyScriptRemoveUnitAndReturnResult, 1 },          \
    { itfMesScriptStartEntryAndWait, 1 },               \
    { ptyScriptAddUnitWithFlagsAndReturnResult, 2 },    \
    { ptyScriptRemoveUnitWithFlagsAndReturnResult, 2 }, \
    { evtPushRosterBaseValue, 0 },                      \
    { btlCmdReturnActorEligibility, 0 },                \
    { btlCommandSelectEventAction, 3 },                 \
    { btlCommandWaitEventSoundDeadline, 2 },            \
    { btlCmdReportEnemySpawnCapacity, 0 },              \
    { btlCmdRememberLowerElementAccess, 1 },            \
    { btlCmdRememberUpperElementAccess, 1 },            \
    { btlCmdRememberLowerElementExclusion, 1 },         \
    { btlCmdRememberUpperElementExclusion, 1 },         \
    { btlCmdRememberUpperActionAvailability, 1 },       \
    { btlCmdRememberActorEntryCondition, 1 },           \
    { btlCmdRememberNonLowerStatFlag, 0 },              \
    { btlOpStartSelectedUnitModelChange, 4 },           \
    { btlCmdReturnReadyWithoutTurnsCheck, 0 },          \
    { btlCmdRememberContextSecondaryFlag, 0 },          \
    { btlAreModelChangeTasksFinished, 0 },              \
    { evtSelectFineStatGrade, 0 },                      \
    { btlScriptBuiltin422NoOp, 0 },                     \
    { btlCmdStoreDefaultUnitActionChoice, 1 },          \
    { btlCmdStoreAlternateUnitActionChoice, 1 },        \
    { btlCmdStorePartyEntryCheckChoice, 1 },            \
    { btlCmdStoreInversePartyEntryChoice, 1 },          \
    { btlCmdRememberUpperEntryCodeMatch, 1 },           \
    { btlCmdRememberUpperEntryCodeInverse, 1 },         \
    { btlCmdRememberUpperSelectedElement, 1 },          \
    { btlCmdRememberLowerMissingStatus, 0 },            \
    { btlScriptPickUnlinkedPartyTarget, 0 },            \
    { btlScriptAppendActorToTargets, 0 },               \
    { btlScriptPickTargetExcludingActor, 0 },           \
    { btlCommandPlayUnitMotion, 3 },                    \
    { btlScriptBuiltin435NoOp, 0 },                     \
    { btlScriptReturnUnitHpRatioPercent, 3 },           \
    { btlScriptBuiltin437NoOp, 0 },                     \
    { btlScriptReturnEffectActive, 0 },                 \
    { btlCmdReturnBattlePhase, 0 },                     \
    { btlCmdRememberLowerEntryCodeMismatch, 1 },        \
    { btlCmdRememberUpperEntryCodeMismatch, 1 },        \
    { btlScriptPickTargetWithoutStatusMask, 1 },        \
    { btlCmdRememberPartyQueuedActionCategory, 1 },     \
    { btlCmdTestEffectActor, 0 },                       \
    { btlScriptReturnEffectValue, 0 },                  \
    { btlScriptReturnSceneSlotCount, 0 },               \
    { btlCmdReturnPartyMissingFlagCheck, 0 },           \
    { btlCmdReturnPartyHeldFlagCheck, 0 },              \
    { btlScriptPickLinkedPartyTarget, 0 },              \
    { btlScriptBuiltin450NoOp, 0 },                     \
    { btlScriptBuiltin451NoOp, 0 },                     \
    { btlScriptAddEffectActorTarget, 0 },               \
    { btlScriptReturnGlobalDebugValue, 0 },             \
    { btlCmdRememberLowerElementBlock, 1 },             \
    { btlScriptPickElementBlockTarget, 1 },             \
    { btlCmdRememberUpperQueuedQuery, 1 },              \
    { btlCmdRememberLowerQueuedQuery, 1 },              \
    { btlScriptSetActorUnitParameter, 1 },              \
    { btlScriptPickHighestMpTarget, 0 },                \
    { btlScriptPickElementCheckedTarget, 1 },           \
    { btlCmdReportPartyEmptyMp, 0 },                    \
    { btlCommandSetSequenceVolumePan, 1 },              \
    { btlCommandReportSpecialEnemyLink, 0 },            \
    { evtPushEntryIndexedStatOption, 0 },               \
    { btlCmdClearSpecialEnemyFlags, 0 },                \
    { scrGetBattleAverageMaximumValueForGroup, 1 },     \
    { scrGetBattleAverageCurrentValueForGroup, 1 },     \
    { btlScriptQueueUnboundCommandSound, 0 },           \
    { btlScriptSetCameraBlendStart, 7 },                \
    { btlScriptSetCameraBlendEnd, 7 },                  \
    { btlCmdCameraMoveBlend, 2 },                       \
    { btlScriptReturnActiveSubtaskPresence, 0 },        \
    { btlCmdGetSubtaskTargetMode, 0 },                  \
    { btlScriptCmdSelectTargetsByMode, 1 },             \
    { btlScriptReturnMarkedCommandRequestedState, 0 },  \
    { btlScriptAppendSelectedOrCurrentTarget, 0 },      \
    { btlScriptSelectShadowSkill, 0 },                  \
    { btlScriptReturnMarkedActionSceneActive, 0 },      \
    { evtCallShooting, 0 },                             \
    { evtOpQueueWorldObjectPendingValue, 2 },           \
    { evtOpClearWorldObjectPendingValue, 1 },           \
    { evtOpModelLodChg, 2 },                            \
    { itfCommandSelectOptionWithFade, 1 },              \
    { itfPanelAcquireHold, 0 },                         \
    { itfCommandQueueHeldPanelEntry, 3 },               \
    { itfStepHeldPanelMessages, 0 },                    \
    { scrCommandClearProcessControlFlag, 0 },           \
    { scrCommandSetProcessControlFlag, 0 },             \
    { btlScriptSetBossStatSelector, 1 },                \
    { btlCmdSpawnEffectTaskForSelectedUnit, 3 },        \
    { btlCommandChooseRandomUnflaggedGroupSlot, 1 },    \
    { btlCmdGetIndexedBattleStateValue, 1 },            \
    { btlCommandCountModelFlags, 0 },                   \
    { NULL, 0 },                                        \
    { NULL, 0 },                                        \
    { fldCommandStartTitle, 2 },                        \
    { fldCommand_FLD_GET_KIRETA, 1 },                   \
    { fldCmdRollMirroredSolarThreshold, 0 },            \
    { fldCommand_FLD_MSE_PLAY2, 2 },                    \
    { fldCommand_FLD_MSE_STOP2, 2 },                    \
    { fldCommand_SET_NPC_CAMERA, 1 },                   \
    { fldCmdMarkBitmapRegion, 5 },                      \
    { fldCommand_FLD_MODEL_DANTE, 0 },                  \
    { fldCmdSetTargetGuideEnabled, 1 },                 \
    { fldCmdSetTargetGuideMode, 1 },                    \
    { fldCmdGetActorStat1, 1 },                         \
    { fldCommand_FLD_WARP_HASIGO_GETTBL, 1 },           \
    { fldCmdApplyNumericRoomModeGroupZero, 4 },         \
    { fldCmdApplyNumericRoomModeGroupOne, 4 },          \
    { fldCmdGetCurrentSceneSelectionResource, 0 },      \
    { fldCommandFindEffectByName, 1 },                  \
    { fldCommand_FLD_WARP_ELV_GETTBL, 1 },              \
    { fldCommand_FLD_WARP_ELV_EXIT, 1 },                \
    { fldCmdSetCurrentTaskScene, 0 },                   \
    { func_001569E0, 0 },                               \
    { fldCommand_FLD_WARP_RUN_START, 0 },               \
    { fldCommand_FLD_INF_NEXT, 2 },                     \
    { fldCommand_FLD_INF_GET, 2 },                      \
    { fldCommand_FLD_INF_GETPANEL, 0 },                 \
    { fldCmdGetActorStat0, 1 },                         \
    { fldCommand_FLD_ELV_FLOORSTART, 2 },               \
    { fldCommand_FLD_ELV_FLOOREND, 0 },                 \
    { fldCommand_FLD_ELV_ENDCHK, 0 },                   \
    { fldCommand_FLD_WARP_GET_DOOR, 1 },                \
    { fldCommand_FLD_EXIT_CAMERA, 0 },                  \
    { fldCommand_FLD_INF_GOWAP, 0 },                    \
    { fldCmdExecuteSceneAction, 1 },                    \
    { fldCmdStartSceneBgm, 1 },                         \
    { fldCmdStartSceneBgmAlternate, 1 },                \
    { fldCmdStopCurrentBgm, 0 },                        \
    { fldCmdPlayCurrentBgmSound, 0 },                   \
    { fldCommandPlaySeVolumePan, 1 },                   \
    { fldCommandPlaySe, 1 },                            \
    { fldCmdReleaseCurrentBgm, 0 },                     \
    { fldCommandLoadArchive, 1 },                       \
    { fldCmdForceTargetGuideUpdate, 0 },                \
    { fldCommand_FLD_WARP_GET_WARPEFF, 0 },             \
    { fldCmdResetAfterEvent, 0 },                       \
    { fldCmdQueryAlternateActorEntrySceneStatus, 1 },   \
    { fldCommand_FLD_SET_ENCOUNT, 1 },                  \
    { fldCommand_ACT_WIN_DSP3, 1 },                     \
    { fldCmdInitializeHuntSparkUnit, 1 },               \
    { fldCmdSetStateFlag, 0 },                          \
    { NULL, 0 },
#endif

#endif /* SCR_COMMAND_TABLE_H */
