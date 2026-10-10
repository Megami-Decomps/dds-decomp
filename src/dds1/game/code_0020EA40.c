#include "pcp_vu0.h"
#include "common.h"
#include "btl_state.h"
#include "btl_command.h"
#include "dat_state.h"
#include "ee_mmi.h"
#include "sdf_resource.h"


extern void btlBossDebugPrintf(const char *format, ...);

extern s32 datActionAnimationRecords;

extern s32 btlGetRuntime(void);

extern s32 btlSetLinkedDefeatCameraPresetB();

extern void btlFlagAllUnitDefeatCandidatesTask(void);

extern void btlSetEffectCameraKeys(void *, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

extern void btlClearRuntimeFlag2000(void);

extern void btlSetEffectCameraKeys(void *, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

/* Action-animation records are distinct from the 0x38-byte command metadata. */
typedef struct BtlActionTableRow {
    u8 pad00[3];
    u8 enabled;
    u8 pad04[0x18];
    u16 flags;
    u8 pad1E[2];
} BtlActionTableRow;

extern void btlSelectRandomDefeatCamera(BtlLinkedCommand *);

extern void btlFlagAllUnitDefeatCandidatesTask(void);

void btlRaiseLinkedActionPose(BtlLinkedCommand *command);

/* Dispatch the command's animation-camera flags. Clearing +0x110 resets command
 * state; it does not clear a BtlUnit's active flags. Returns 1 when handled. */
s32 btlDispatchActionAnimation(BtlLinkedCommand *command) {
    u16 flags = ((BtlActionTableRow *)datActionAnimationRecords)[command->actionCode].flags;
    if (flags & 0x4000) {
        btlFlagAllUnitDefeatCandidatesTask();
        if (!(flags & 0x10)) {
            btlSelectRandomDefeatCamera(command);
        } else {
            btlSetEffectCameraKeys(command, -203.0f, -531.1f, -1259.0f,
                           0.124f, -0.07f, -0.021f, 0.981f,
                           -203.0f, -46.1f, -1259.0f, -0.144f,
                           -0.066f, -0.003f, 0.978f, 45.0f, 30.0f);
        }
        command->state = 0;
    } else if (flags & 0x8000) {
        btlFlagAllUnitDefeatCandidatesTask();
        btlSetLinkedDefeatCameraPresetB(command, command, 0);
    } else if (flags & 8) {
        if (btlGetIndexListCount(command->task->indexWork.indices) == 1) {
            btlFlagAllUnitDefeatCandidatesTask();
            btlRaiseLinkedActionPose(command);
            command->state = 0;
        } else {
            btlFlagAllUnitDefeatCandidatesTask();
            btlSelectRandomDefeatCamera(command);
        }
    } else {
        return 0;
    }
    btlClearRuntimeFlag2000();
    return 1;
}

s32 btlGetPhaseCommand(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    switch (battle->phase) {
    case 0: return 0x11c;
    case 1: return 0x11b;
    default: return -1;
    }
}

s32 btlGetAlternatePhaseCommand(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    switch (battle->phase) {
    case 0: return 0x11f;
    case 1: return 0x120;
    default: return -1;
    }
}

/* Only scene-listed special units use these alternate action codes. */
s32 btlRemapListedUnitAction(BtlUnit *unit, s32 action) {
    BtlState *battle;
    u16 *listedMode;
    u32 i;

    if ((unit->status.flags & 0x400) == 0) {
        return action;
    }
    battle = (BtlState *)btlGetRuntime();
    i = 0;
    listedMode = ((DatBattleSceneRecord *)(battle->battleMode * (s32)sizeof(DatBattleSceneRecord) +
                                      (u32)datBattleSceneRecords))->unitModes;
    while (i < 0xB && listedMode[i] != unit->partyRecord.unitId) {
        i++;
    }
    if (i == 0xB) {
        return action;
    }
    switch (action) {
    case 2:
        return 0;
    case 11:
        return 1;
    case 13:
        return -1;
    default:
        return action;
    }
}

/* Whether the unit's mode appears in its battle scene's listed unit modes. */
s32 btlIsSceneUnitModeListed(BtlUnit *unit) {
    BtlState *battle;
    u16 *listedMode;
    u32 i;

    if ((unit->status.flags & 0x400) == 0) {
        return 0;
    }
    battle = (BtlState *)btlGetRuntime();
    i = 0;
    listedMode = ((DatBattleSceneRecord *)(battle->battleMode * (s32)sizeof(DatBattleSceneRecord) +
                                      (u32)datBattleSceneRecords))->unitModes;
    for (; i < 0xB; i++) {
        if (listedMode[i] == unit->partyRecord.unitId) {
            return 1;
        }
    }
    return 0;
}

extern void *memset(void *, s32, u32);

typedef struct BtlScaleStageSource BtlScaleStageSource;

extern s32 btlAdjustDamageKind(BtlUnit *unit, s32 damageKind);
extern s32 btlAdjustSpeciesAnimation(u8 *unit, s32 animation);
extern s32 btlAllowsSpeciesCondition(BtlUnit *unit, BtlUnit *other, s32 condition);
extern s32 btlApplyActionDefeatCamera(BtlLinkedCommand *command);
extern void btlArmEventResourceTrigger(void);
extern s32 btlCheckActiveEffectForSpecialTarget(BtlUnit *actor, BtlUnit *target, s32 command, s32 bits);
extern s32 btlCheckAttachedMember(u8 *entry);
extern s32 btlCheckBossOptionAllowed(u8 *unit);
extern s32 btlCheckEnemyUnitReadiness(void);
extern s32 btlCheckLinkedActionEffectTarget(BtlUnit *actor, BtlUnit *target, s32 command);
extern s32 btlCheckSelectedBossUnitFlag(void);
extern s32 btlChooseDefeatCameraByActionAndTargets(u8 *unit);
extern s32 btlClassifyLinkedSkillRequest(BtlUnit *source, BtlUnit *unit, s32 index);
extern void btlClearEventEffectValue(void);
extern s32 btlClearUnitRestrictionFlag(void);
extern s32 btlConsumeReadyEventScriptResource(void);
extern u64 btlCreateSpecialUnitAndLoadModel(u64 prerequisiteHandle);
extern s32 btlDeactivateOthersOnSpecialUnitDefeat(void);
extern void btlDestroyActiveMemberSlot(void);
extern void btlDestroySpecialUnitSlot(void);
extern s32 btlDisableNonBossUnits(void);
extern s32 btlDisableUnitsIfSpeciesFlagged(void);
extern s32 btlDispatchActionAnimationB(BtlLinkedCommand *command);
extern s32 btlDispatchEligibleBossEvent(BtlLinkedCommand *entry);
extern void btlDispatchSpecialEnemyActionWhenPhaseAllows(BtlUnit *unit, s32 action);
extern s32 btlEffectTaskStartFinale(BtlTask *task);
extern u64 btlEnsureEffectUnitModelLoadTask(u64 prerequisiteHandle);
extern s32 btlFadeOtherEnemyUnitsWhenSpecialModeActive(void);
extern s32 btlFilterBossCommandBySelection(u8 *unit, s32 command);
extern s32 btlFilterRestrictedCommand(s32 battler, s32 command);
extern BtlUnit *btlFindActiveMember(u32 group, u32 type);
extern u8 *btlFindFlaggedSpecialSpeciesUnit(s32 category, s32 species);
extern s32 btlFindSubsequentTurnScript(void);
extern void btlFormatBattleEffectResourceName(s32 record, s32 action, s32 pathBuffer);
extern s32 btlFrameMarkedTargetDefeatCamera(BtlLinkedCommand *command);
extern s32 btlGetAdjustedUnitDisplaySpecies(BtlUnit *unit);
extern s32 btlGetEffectTaskActorMatchCode(u8 *task);
extern s32 btlGetEnabledEnemyActionResponse(BtlUnit *unit, s32 action);
extern u8 *btlGetReadyUnitForSpecies(s32 mode, u32 species);
extern s32 btlGetSoleTargetKind(void);
extern s32 btlHandleLinkedUnitDefeatAction(BtlLinkedCommand *command);
extern s32 btlHandleTargetedDefeatAction(u8 *unit);
extern s32 btlHasDifferentActiveTarget(BtlUnit *target);
extern void btlInitRandomBossSelection(void);
extern s32 btlInitializeResources(s32 unused, s32 resource);
extern void btlInitializeUnitDisplaySpeciesAndFlags(u8 *unit);
extern u8 btlIsCommandCodeF(u32 unused, s32 command);
extern s32 btlIsEffectActor(BtlUnit *actor);
extern s32 btlIsEffectPhaseInRange(s32 unused, s32 value);
extern u32 btlIsInSpecialModeRange(u32 unused0, u32 unused1, s32 action);
extern s32 btlIsSpecialActionKind(BtlLinkedCommand *command);
extern u32 btlMapActionToCode(s32 action);
extern s32 btlMapLinkedCommandResult(BtlUnit *unit, s32 arg1);
extern s32 btlMapSkillRange(u32 skill);
extern s32 btlMapSkillToCode(s32 skill);
extern s32 btlMapSpeciesToActionVariant(u8 *unit, s32 action);
extern void btlMarkSpecialUnit(u8 *unit);
extern s32 btlNormalizeActionForSkill(BtlUnit *unit, s32 action);
extern s32 btlNormalizeActionForStatus(BtlUnit *unit, s32 action);
extern void btlNormalizeDefaultPlayerDisplaySpecies(s32 unit);
extern s32 btlOffsetSpecialTargetPositionForAction(BtlLinkedCommand *command, s32 unused1, s32 unused2);
extern s32 btlPickRandomMarkedTask(void);
extern s32 btlQueueHariFormChangeOrPartyCommand(void);
extern void btlRecenterUnitsOnLead(void);
extern s32 btlRemapBossAction(BtlUnit *unit, s32 action, u8 option);
extern s32 btlRemapSelectedBossCommand(u8 *unit, s32 command, u8 mode);
extern s32 btlRemapSpecialUnitCommandIndex(u8 *unit, s32 index);
extern void btlResetEffectState(void);
extern void btlResetUnitPlacement(void);
extern void btlRestoreHaritiFormation(void);
extern void btlRestoreSlotEntriesFromEffect(void);
extern u64 btlScheduleActionAndSoundSequence(BtlTask *action);
extern s32 btlSelectDisabledCommand(s32 battler);
extern u32 btlSelectResponseCodeForEnemyFlag(u32 unused, s32 unit);
extern void btlSelectSlotEntries(void);
extern s32 btlSelectSpecialActionCameraPose(BtlLinkedCommand *command, s8 a, s8 b);
extern s32 btlSelectSpecialDefeatCameraPose(BtlLinkedCommand *command, s32 any200, s32 any400);
extern s32 btlSetLinkFlagOff(BtlUnit *requestedUnit);
extern s32 btlSetLinkFlagOn(BtlUnit *requestedUnit);
extern s32 btlSetLinkedDefeatCameraPresetA(BtlLinkedCommand *command, BtlCamState *camera, s32 rotate);
extern s32 btlSetLinkedDefeatCameraPresetB(BtlLinkedCommand *command, BtlCamState *camera, s32 rotate);
extern void btlStartPairedActorCommandEffect(BtlTask *task, s32 unusedCommand, BtlUnit *supplied, u64 ownerId, u64 prerequisiteHandle, s32 condition);
extern u64 btlStartSubtaskWithInput(u64 prerequisiteHandle);
extern void btlStepFocusAngle(void);
extern s32 btlSwapRandomBossSelection(void);
extern void btlSyncEffectActorToUnit(BtlUnit *unit, u8 *task);
extern void btlTrackSpecialEnemyCommandRestrictionByTurn(u8 *unit, u8 *command);
extern void btlTriggerSpecialUnitAction(void);
extern void btlTriggerSpecialUnitActionAndResetPose(s32 unit);
extern s32 btlTryScheduleMarkedUnitTask(u8 *unit);
extern s32 btlTryStartTargetFacingActionEffect(u8 *unit);
extern s32 btlUnitStartAimAtTarget(BtlLinkedCommand *command);
extern void btlUpdateReadyUnits(void);
extern s32 btlUpdateSpecialTargetCamera(BtlLinkedCommand *command);
extern void effBTLFieldColorSetFlags(u32 flags);
extern u32 func_00204AC0(void);
extern u32 func_00204B40(void);
extern void func_00205070(void);
extern u32 func_002055F0(void);
extern s32 func_002055F8(void);
extern void func_00205918(BtlUnit *actor, BtlOperandEntry *operand);
extern void func_00205BD8(void);
extern void func_00205EE0(void);
extern s32 func_00205EF8(BtlUnit *unit, s32 code, s32 unused);
extern void func_00206450(void);
extern s32 func_00207718(BtlLinkedCommand *command);
extern void func_00207E68(void);
extern s32 func_00207FF0(BtlUnit *unit, s32 animation);
extern u32 func_00208660(void);
extern s32 func_00208668(s32 unit);
extern s32 func_00208678(BtlUnit *unit, s32 command);
extern void func_00208860(s32 unit, BtlScaleStageSource *source);
extern void func_00208A50(void);
extern void func_00208C20(void);
extern void func_00208E10(BtlUnit *unit);
extern void func_00208EA8(BtlUnit *unit);
extern void func_00209528(void);
extern s32 func_0020B818(BtlLinkedCommand *command, s8 a, s8 b);
extern u32 func_0020CB28(s32 action);
extern void func_0020CCA8(void);
extern void func_0020D168(BtlUnit *unit);
extern u32 func_0020D548(void);
extern s32 func_0020D598(BtlUnit *unit, s32 action);
extern s32 func_0020D690(BtlUnit *unit, s32 action);
extern u8 func_0020D9A8(s32 action);
extern s32 func_0020D9F8(BtlUnit *unit, s32 action);
extern s32 func_0020E170(BtlLinkedCommand *command, s8 firstSide, s8 secondSide);
extern void mdlFlagSet(s32 flag);

/* Native providers whose bodies remain assembly in their own units. */
s32 btlInitResourcesWrap(BtlLinkedCommand *, BtlCamState *, s32);
void func_00204D08(BtlTask *);
s32 func_00206180(BtlUnit *);
void func_00206608(void);
void func_002069C0(BtlTask *, s32, u64, u64, u64);
void func_00209EB8(BtlTask *);
s32 func_0020A860(void);
s32 func_0020AB08(DatPartyRecord *, s32);
s32 func_0020B560(BtlLinkedCommand *, BtlCamState *, s32);
void func_0020CCC0(void);
s32 func_0020D668(BtlLinkedCommand *, s32, s32);
s32 func_0020DE50(BtlLinkedCommand *, BtlCamState *, s32);

extern u32 kwlnDrawControlFlags;

/* Copy the verified four-byte EE callback representation into its actual slot.
 * Some callback slots remain byte storage or carry older shared prototypes. */
#define BTL_INSTALL_CALLBACK(storage, byteOffset, provider) { \
    __typeof__(&(provider)) callback = &(provider); \
    typedef char CallbackWidthIsFour[(sizeof(callback) == 4) ? 1 : -1]; \
    typedef char CallbackFitsSlot[((byteOffset) + sizeof(callback) <= sizeof(storage)) ? 1 : -1]; \
    memcpy((u8 *)&(storage) + (byteOffset), &callback, sizeof(callback)); \
}

/* Install the mode-specific battle providers before starting their setup. */
void func_0020ED90(s32 mode) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    void *allocation;
    void (*setup)(void);

    battle->effect = NULL;
    switch (mode) {
    case 0x101:
        battle->commandRestrictFlags |= 0x2000;
        mdlFlagSet(0x802);
        BTL_INSTALL_CALLBACK(battle->pad598, 0x0, func_00204AC0);
        BTL_INSTALL_CALLBACK(battle->hitResultOverride, 0x0, btlClassifyLinkedSkillRequest);
        break;
    case 0x102:
        mdlFlagSet(0x803);
        battle->commandRestrictFlags |= 5;
        BTL_INSTALL_CALLBACK(battle->pad598, 0x0, func_00204B40);
        break;
    case 0x103:
    case 0x106:
    case 0x109: {
        u32 commandFlags;
        commandFlags = battle->commandRestrictFlags;
        battle->commandRestrictFlags = commandFlags | 1;
        if (mode != 0x109) {
            battle->commandRestrictFlags = commandFlags | 5;
        }
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, btlFilterRestrictedCommand);
        BTL_INSTALL_CALLBACK(battle->pad5A4, 0x4, btlIsCommandCodeF);
        BTL_INSTALL_CALLBACK(battle->pad5A4, 0x0, btlSelectDisabledCommand);
        BTL_INSTALL_CALLBACK(battle->actorParameterDeltaCallback, 0x0, btlTrackSpecialEnemyCommandRestrictionByTurn);
        BTL_INSTALL_CALLBACK(battle->pad5B0, 0x0, btlClearUnitRestrictionFlag);
        BTL_INSTALL_CALLBACK(battle->pad5F4, 0xc, func_00204D08);
        BTL_INSTALL_CALLBACK(battle->updateCallback, 0x0, btlResetUnitPlacement);
        if (mode == 0x106) {
            BTL_INSTALL_CALLBACK(battle->unk5B4, 0x0, btlDisableNonBossUnits);
        }
        allocation = sdfAllocateBlockBySizeThreshold(4);
        battle->effect = allocation;
        memset(allocation, 0, 4);
        break;
    }
    case 0x105:
        battle->commandRestrictFlags = (battle->commandRestrictFlags | 1) & ~2;
        BTL_INSTALL_CALLBACK(battle->postPlacementCallback, 0x0, func_00205070);
        BTL_INSTALL_CALLBACK(battle->unk5B4, 0x0, btlDisableUnitsIfSpeciesFlagged);
        break;
    case 0x107:
    case 0x117:
        battle->commandRestrictFlags = (battle->commandRestrictFlags | 1) & ~2;
        BTL_INSTALL_CALLBACK(battle->pad590, 0x0, btlSelectSlotEntries);
        BTL_INSTALL_CALLBACK(battle->bossCleanup, 0x0, btlRestoreSlotEntriesFromEffect);
        BTL_INSTALL_CALLBACK(battle->pad5C0, 0x0, btlInitializeUnitDisplaySpeciesAndFlags);
        BTL_INSTALL_CALLBACK(battle->finishModelUnit, 0x0, btlNormalizeDefaultPlayerDisplaySpecies);
        BTL_INSTALL_CALLBACK(battle->unk5B4, 0x0, func_002055F8);
        BTL_INSTALL_CALLBACK(battle->cameraArrangementHook, 0x0, btlInitializeResources);
        BTL_INSTALL_CALLBACK(battle->defeatCameraHook, 0x0, btlInitResourcesWrap);
        if (mode == 0x107) {
            BTL_INSTALL_CALLBACK(battle->pad5B0, 0x0, func_002055F0);
        }
        allocation = sdfAllocateBlockBySizeThreshold(0x44);
        battle->effect = allocation;
        memset(allocation, 0, 0x44);
        break;
    case 0x108: {
        u32 commandFlags;
        commandFlags = battle->commandRestrictFlags;
        battle->commandRestrictFlags = commandFlags | 1;
        BTL_INSTALL_CALLBACK(battle->pad590, 0x0, btlResetEffectState);
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, func_00205EF8);
        BTL_INSTALL_CALLBACK(battle->pad5A4, 0x4, btlIsEffectPhaseInRange);
        BTL_INSTALL_CALLBACK(battle->pad5A4, 0x0, func_00206180);
        BTL_INSTALL_CALLBACK(battle->actorParameterDeltaCallback, 0x0, func_00205918);
        BTL_INSTALL_CALLBACK(battle->actionHitOverride, 0x0, btlCheckActiveEffectForSpecialTarget);
        BTL_INSTALL_CALLBACK(battle->postPlacementCallback, 0x0, func_00205BD8);
        BTL_INSTALL_CALLBACK(battle->updateCallback, 0x0, func_00205EE0);
        BTL_INSTALL_CALLBACK(battle->effectParameterCallback, 0x0, btlAdjustDamageKind);
        BTL_INSTALL_CALLBACK(battle->findReusableUnit, 0x0, btlFindFlaggedSpecialSpeciesUnit);
        BTL_INSTALL_CALLBACK(battle->callback5CC, 0x0, btlGetAdjustedUnitDisplaySpecies);
        BTL_INSTALL_CALLBACK(battle->beforeMotionUpdate, 0x0, func_00206450);
        BTL_INSTALL_CALLBACK(battle->pad5F4, 0x4, func_00206608);
        BTL_INSTALL_CALLBACK(battle->pad5F4, 0x8, btlGetEffectTaskActorMatchCode);
        BTL_INSTALL_CALLBACK(battle->pad5F4, 0x10, btlEffectTaskStartFinale);
        BTL_INSTALL_CALLBACK(battle->preActionHook, 0x0, func_002069C0);
        BTL_INSTALL_CALLBACK(battle->pad650, 0x0, btlIsEffectActor);
        BTL_INSTALL_CALLBACK(battle->unk5B4, 0x0, btlGetSoleTargetKind);
        BTL_INSTALL_CALLBACK(battle->pad65C, 0x0, btlHasDifferentActiveTarget);
        BTL_INSTALL_CALLBACK(battle->allowDefeatCandidate, 0x0, btlSetLinkFlagOff);
        BTL_INSTALL_CALLBACK(battle->unk658, 0x0, btlSetLinkFlagOn);
        BTL_INSTALL_CALLBACK(battle->pad65C, 0x4, btlScheduleActionAndSoundSequence);
        BTL_INSTALL_CALLBACK(battle->pad5E4, 0x4, btlTryScheduleMarkedUnitTask);
        BTL_INSTALL_CALLBACK(battle->cameraPoseBlendHook, 0x0, func_00207718);
        BTL_INSTALL_CALLBACK(battle->actionCameraStepHook, 0x0, btlUnitStartAimAtTarget);
        BTL_INSTALL_CALLBACK(battle->handleActorCategoryCamera, 0x0, btlIsSpecialActionKind);
        allocation = sdfAllocateBlockBySizeThreshold(0x18);
        battle->effect = allocation;
        memset(allocation, 0, 0x18);
        break;
    }
    case 0x10A: {
        u32 commandFlags;
        commandFlags = battle->commandRestrictFlags;
        battle->commandRestrictFlags = commandFlags | 1;
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, func_00207FF0);
        BTL_INSTALL_CALLBACK(battle->postPlacementCallback, 0x0, func_00207E68);
        BTL_INSTALL_CALLBACK(battle->effectParameterCallback, 0x0, btlAdjustSpeciesAnimation);
        BTL_INSTALL_CALLBACK(battle->pad5C0, 0x0, btlMarkSpecialUnit);
        BTL_INSTALL_CALLBACK(battle->findReusableUnit, 0x0, btlGetReadyUnitForSpecies);
        BTL_INSTALL_CALLBACK(battle->callback5C8, 0x0, btlCreateSpecialUnitAndLoadModel);
        BTL_INSTALL_CALLBACK(battle->cleanup, 0x0, btlDestroySpecialUnitSlot);
        BTL_INSTALL_CALLBACK(battle->callback5D4, 0x0, btlStartSubtaskWithInput);
        BTL_INSTALL_CALLBACK(battle->beforeMotionUpdate, 0x0, btlUpdateReadyUnits);
        BTL_INSTALL_CALLBACK(battle->pad5E4, 0x4, btlCheckEnemyUnitReadiness);
        BTL_INSTALL_CALLBACK(battle->pad644, 0x0, func_00208660);
        BTL_INSTALL_CALLBACK(battle->pad680, 0xc, func_00208668);
        BTL_INSTALL_CALLBACK(battle->pad680, 0x10, func_00208678);
        allocation = sdfAllocateBlockBySizeThreshold(4);
        battle->effect = allocation;
        memset(allocation, 0, 4);
        break;
    }
    case 0x116:
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, btlRemapSpecialUnitCommandIndex);
        BTL_INSTALL_CALLBACK(battle->actorParameterDeltaCallback, 0x0, func_00208860);
        BTL_INSTALL_CALLBACK(battle->prepareModelUnit, 0x0, btlTriggerSpecialUnitActionAndResetPose);
        BTL_INSTALL_CALLBACK(battle->beforeMotionUpdate, 0x0, func_00208A50);
        BTL_INSTALL_CALLBACK(battle->updateCallback, 0x0, func_00208C20);
        BTL_INSTALL_CALLBACK(battle->postPlacementCallback, 0x0, btlTriggerSpecialUnitAction);
        BTL_INSTALL_CALLBACK(battle->actionPointsOverride, 0x0, btlIsInSpecialModeRange);
        allocation = sdfAllocateBlockBySizeThreshold(8);
        battle->effect = allocation;
        memset(allocation, 0, 8);
        break;
    case 0x10B: {
        u32 commandFlags;
        commandFlags = battle->commandRestrictFlags;
        battle->commandRestrictFlags = commandFlags | 0x51;
        BTL_INSTALL_CALLBACK(battle->pad590, 0x0, btlInitRandomBossSelection);
        BTL_INSTALL_CALLBACK(battle->effectParameterCallback, 0x0, btlFilterBossCommandBySelection);
        BTL_INSTALL_CALLBACK(battle->pad5C0, 0x0, func_00208E10);
        BTL_INSTALL_CALLBACK(battle->findReusableUnit, 0x0, btlFindActiveMember);
        BTL_INSTALL_CALLBACK(battle->callback5C8, 0x0, btlEnsureEffectUnitModelLoadTask);
        BTL_INSTALL_CALLBACK(battle->cleanup, 0x0, btlDestroyActiveMemberSlot);
        BTL_INSTALL_CALLBACK(battle->beforeMotionUpdate, 0x0, btlStepFocusAngle);
        BTL_INSTALL_CALLBACK(battle->finishModelUnit, 0x0, func_00208EA8);
        BTL_INSTALL_CALLBACK(battle->actionHitOverride, 0x0, btlCheckLinkedActionEffectTarget);
        BTL_INSTALL_CALLBACK(battle->reflectedHitOverride, 0x0, btlSelectResponseCodeForEnemyFlag);
        BTL_INSTALL_CALLBACK(battle->pad5B0, 0x0, btlSwapRandomBossSelection);
        BTL_INSTALL_CALLBACK(battle->pad5F4, 0x0, btlFindSubsequentTurnScript);
        BTL_INSTALL_CALLBACK(battle->unk5B4, 0x0, btlCheckSelectedBossUnitFlag);
        BTL_INSTALL_CALLBACK(battle->updateCallback, 0x0, func_00209528);
        BTL_INSTALL_CALLBACK(battle->pad65C, 0x8, btlDispatchEligibleBossEvent);
        BTL_INSTALL_CALLBACK(battle->pad65C, 0xc, btlCheckAttachedMember);
        BTL_INSTALL_CALLBACK(battle->pad644, 0x0, btlCheckBossOptionAllowed);
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, btlRemapSelectedBossCommand);
        BTL_INSTALL_CALLBACK(battle->actorParameterDeltaCallback, 0x0, btlSyncEffectActorToUnit);
        allocation = sdfAllocateBlockBySizeThreshold(0x10);
        battle->effect = allocation;
        memset(allocation, 0, 0x10);
        break;
    }
    case 0x11D: {
        u32 commandFlags;
        commandFlags = battle->commandRestrictFlags;
        battle->commandRestrictFlags = ((commandFlags | 1) & ~2) | 0x80;
        BTL_INSTALL_CALLBACK(battle->pad590, 0x0, btlArmEventResourceTrigger);
        BTL_INSTALL_CALLBACK(battle->pad5B0, 0x0, btlPickRandomMarkedTask);
        BTL_INSTALL_CALLBACK(battle->pad5F4, 0x0, btlConsumeReadyEventScriptResource);
        allocation = sdfAllocateBlockBySizeThreshold(2);
        battle->effect = allocation;
        memset(allocation, 0, 2);
        break;
    }
    case 0x10D: {
        u32 commandFlags;
        commandFlags = battle->commandRestrictFlags;
        battle->commandRestrictFlags = commandFlags | 0x841;
        BTL_INSTALL_CALLBACK(battle->pad590, 0x0, btlClearEventEffectValue);
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, btlRemapBossAction);
        BTL_INSTALL_CALLBACK(battle->pad5B0, 0x0, btlQueueHariFormChangeOrPartyCommand);
        BTL_INSTALL_CALLBACK(battle->actorParameterDeltaCallback, 0x0, btlDispatchSpecialEnemyActionWhenPhaseAllows);
        BTL_INSTALL_CALLBACK(battle->pad65C, 0x4, func_00209EB8);
        allocation = sdfAllocateBlockBySizeThreshold(2);
        battle->effect = allocation;
        memset(allocation, 0, 2);
        break;
    }
    case 0x10E:
        kwlnDrawControlFlags |= 0x20000000;
        battle->commandRestrictFlags |= 0x281;
        effBTLFieldColorSetFlags(1);
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, btlMapLinkedCommandResult);
        BTL_INSTALL_CALLBACK(battle->pad5B0, 0x0, func_0020A860);
        BTL_INSTALL_CALLBACK(battle->pad670, 0x0, func_0020AB08);
        BTL_INSTALL_CALLBACK(battle->unk5B4, 0x0, btlFadeOtherEnemyUnitsWhenSpecialModeActive);
        BTL_INSTALL_CALLBACK(battle->pad670, 0x4, btlMapSkillToCode);
        BTL_INSTALL_CALLBACK(battle->pad670, 0x8, btlMapSkillRange);
        BTL_INSTALL_CALLBACK(battle->postPlacementCallback, 0x0, btlRestoreHaritiFormation);
        BTL_INSTALL_CALLBACK(battle->pad644, 0x0, btlMapSpeciesToActionVariant);
        BTL_INSTALL_CALLBACK(battle->updateCallback, 0x0, func_0020CCA8);
        BTL_INSTALL_CALLBACK(battle->cameraArrangementHook, 0x0, btlSetLinkedDefeatCameraPresetA);
        BTL_INSTALL_CALLBACK(battle->defeatCameraHook, 0x0, func_0020B560);
        BTL_INSTALL_CALLBACK(battle->cameraStateChangePredicate, 0x0, btlTryStartTargetFacingActionEffect);
        BTL_INSTALL_CALLBACK(battle->pad614, 0x0, btlUpdateSpecialTargetCamera);
        BTL_INSTALL_CALLBACK(battle->pad614, 0x4, btlHandleTargetedDefeatAction);
        BTL_INSTALL_CALLBACK(battle->cameraPoseBlendHook, 0x0, func_0020B818);
        BTL_INSTALL_CALLBACK(battle->handleActorCategoryCamera, 0x0, btlSelectSpecialDefeatCameraPose);
        BTL_INSTALL_CALLBACK(battle->actionCameraSetupHook, 0x0, btlChooseDefeatCameraByActionAndTargets);
        BTL_INSTALL_CALLBACK(battle->commandSoundDelay, 0x0, func_0020CB28);
        BTL_INSTALL_CALLBACK(battle->pad620, 0x0, btlDispatchActionAnimationB);
        BTL_INSTALL_CALLBACK(battle->beforeMotionUpdate, 0x0, func_0020CCC0);
        BTL_INSTALL_CALLBACK(battle->finishModelUnit, 0x0, func_0020D168);
        BTL_INSTALL_CALLBACK(battle->postTargetHook, 0x0, btlStartPairedActorCommandEffect);
        BTL_INSTALL_CALLBACK(battle->pad680, 0x0, btlAllowsSpeciesCondition);
        BTL_INSTALL_CALLBACK(battle->pad680, 0x4, btlFormatBattleEffectResourceName);
        BTL_INSTALL_CALLBACK(battle->pad680, 0x8, func_0020D548);
        allocation = sdfAllocateBlockBySizeThreshold(0x24);
        battle->effect = allocation;
        memset(allocation, 0, 0x24);
        break;
    case 0x10C:
        BTL_INSTALL_CALLBACK(battle->pad644, 0x0, btlGetEnabledEnemyActionResponse);
        break;
    case 0x14B:
        BTL_INSTALL_CALLBACK(battle->pad644, 0x0, func_0020D598);
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, btlRemapListedUnitAction);
        BTL_INSTALL_CALLBACK(battle->pad5E4, 0x0, btlIsSceneUnitModeListed);
        BTL_INSTALL_CALLBACK(battle->cameraPoseBlendHook, 0x0, btlOffsetSpecialTargetPositionForAction);
        BTL_INSTALL_CALLBACK(battle->handleActorCategoryCamera, 0x0, func_0020D668);
        break;
    case 0x13E:
        BTL_INSTALL_CALLBACK(battle->pad644, 0x0, func_0020D690);
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, btlRemapListedUnitAction);
        BTL_INSTALL_CALLBACK(battle->pad5E4, 0x0, btlIsSceneUnitModeListed);
        break;
    case 0x138:
        battle->commandRestrictFlags |= 0x400;
        BTL_INSTALL_CALLBACK(battle->unk5B4, 0x0, btlDeactivateOthersOnSpecialUnitDefeat);
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, btlNormalizeActionForSkill);
        break;
    case 0x140:
        BTL_INSTALL_CALLBACK(battle->postPlacementCallback, 0x0, btlRecenterUnitsOnLead);
        BTL_INSTALL_CALLBACK(battle->pad670, 0x8, btlMapActionToCode);
        BTL_INSTALL_CALLBACK(battle->pad634, 0x4, func_0020D9A8);
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, btlRemapListedUnitAction);
        BTL_INSTALL_CALLBACK(battle->pad5E4, 0x0, btlIsSceneUnitModeListed);
        break;
    case 0x13F: {
        u32 commandFlags;
        commandFlags = battle->commandRestrictFlags;
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, btlNormalizeActionForStatus);
        battle->commandRestrictFlags = commandFlags | 0x200;
        effBTLFieldColorSetFlags(1);
        break;
    }
    case 0x139:
    case 0x13A:
    case 0x13D:
    case 0x149:
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, btlRemapListedUnitAction);
        BTL_INSTALL_CALLBACK(battle->pad5E4, 0x0, btlIsSceneUnitModeListed);
        break;
    case 0x14C:
        battle->commandRestrictFlags |= 0x200;
        BTL_INSTALL_CALLBACK(battle->chooseMotion, 0x0, btlRemapListedUnitAction);
        BTL_INSTALL_CALLBACK(battle->pad5E4, 0x0, btlIsSceneUnitModeListed);
        BTL_INSTALL_CALLBACK(battle->pad644, 0x0, func_0020D9F8);
        BTL_INSTALL_CALLBACK(battle->cameraArrangementHook, 0x0, btlSetLinkedDefeatCameraPresetB);
        BTL_INSTALL_CALLBACK(battle->defeatCameraHook, 0x0, func_0020DE50);
        BTL_INSTALL_CALLBACK(battle->cameraStateChangePredicate, 0x0, btlFrameMarkedTargetDefeatCamera);
        BTL_INSTALL_CALLBACK(battle->pad614, 0x4, btlHandleLinkedUnitDefeatAction);
        BTL_INSTALL_CALLBACK(battle->cameraPoseBlendHook, 0x0, btlSelectSpecialActionCameraPose);
        BTL_INSTALL_CALLBACK(battle->handleActorCategoryCamera, 0x0, func_0020E170);
        BTL_INSTALL_CALLBACK(battle->actionCameraSetupHook, 0x0, btlApplyActionDefeatCamera);
        BTL_INSTALL_CALLBACK(battle->pad620, 0x0, btlDispatchActionAnimation);
        break;
    case 0x11C:
        BTL_INSTALL_CALLBACK(battle->serialOverride, 0, btlGetPhaseCommand);
        break;
    case 0x11E:
        BTL_INSTALL_CALLBACK(battle->serialOverride, 0, btlGetAlternatePhaseCommand);
        break;
    default:
        return;
    }
    memcpy(&setup, battle->pad590, sizeof(setup));
    if (setup != NULL) {
        setup();
    }
    battle->battleFlags |= 0x80000;
    btlBossDebugPrintf("btl:boss init\n");
}

#undef BTL_INSTALL_CALLBACK

extern char D_003A66C0[];

void btlRunCleanupAndLog(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    void (*cleanup)(void) = battle->cleanup;
    if (cleanup != 0) {
        cleanup();
    }
    btlBossDebugPrintf(D_003A66C0);
}

extern char D_003A66D8[];

void btlReleaseBossData(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    void (*cleanup)(void);
    if ((battle->battleFlags & 0x80000) == 0) {
        return;
    }
    cleanup = battle->bossCleanup;
    if (cleanup != 0) {
        cleanup();
    }
    btlRunCleanupAndLog();
    if (battle->effect != 0) {
        sdfReleaseChipOrRetainedResource(battle->effect);
        battle->effect = 0;
    }
    battle->battleFlags &= ~0x80000;
    btlBossDebugPrintf(D_003A66D8);
}

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A66C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A66D8);

