#include "sdk_ps2.h"
#include "scr.h"
#include "mdl_motion_api.h"
#include "fld_area_work.h"
#include "fld_archive_load_state.h"
#include "fld_deferred_transition.h"
#include "fld_runtime_reset_api.h"
#include "fld_area_encounter_table.h"
#include "sdf_packet_list.h"
#include "common.h"
#include "pty_status_query_api.h"
#include "sdf_delayed_resource_check_api.h"
#include "dds3_object_flags.h"
#include "sdf_vu0.h"
#include "sdf_trig.h"
#include "sdf_vu_matrix.h"
#include "fld_scene_status_api.h"
#include "fld_scene_ready_api.h"
#include "fld_area_resource_ready_api.h"
#include "fld_movement_hold_state_api.h"
#include "fld_scene_command_state_api.h"
#include "fld_camp_check_state_api.h"
#include "mnu_camp_state_ack_api.h"
#include "fld_next_scene_request_api.h"
#include "fld_encounter_runtime_api.h"
#include "sdf_message.h"
#include "eff_random.h"
#include "snd_sequence_volume_pan.h"
#include "evt_world_object_api.h"
#include "sdf_model_scalars.h"
#include "dds3_path.h"
#include "dds3_slot1_handler_api.h"
#include "sdf_dev_state.h"
#include "sdf_resource.h"
#include "mdl.h"
#include "field_stage.h"
#include "eff_blur.h"
#include "pcp_vu0.h"
#include "fpu.h"
#include "fld.h"
#include "evt_world.h"
#include "evt_solar.h"
#include "evt_random_status_clear_api.h"
#include "evt_world_source_transform.h"
#include "dds3obj.h"
#include "dds3_world_value.h"
#include "dds3_world_selection.h"
#include "kwln.h"
#include "dds3_admin_control_api.h"
#include "kwln_task_runner.h"
#include "kwln_task_priority_query.h"
#include "kwln_task_create.h"
#include "kwln_fade.h"
#include "dat_state.h"
#include "dat_world_field_flags.h"
#include "kwln_task_lifecycle.h"
#include "mdl_flag_api.h"
#include "dds3_world_counter.h"
#include "fld_resource_wait_api.h"
#include "fld_sequence_controller.h"
#include "file_selection_api.h"
#include "fld_player_scene_state.h"
#include "eff_inner_float_api.h"
#include "btl.h"
/* Declared before first use (were implicit int). */
extern void evtFreeStage(void);
extern f32 fabsf(f32);
extern void fldResetTargetViewAndSound(void);
extern void fldPauseBgm(void);
extern void fldSubmitSelectedActorWaypointTransition(void);

extern dds3Basic_t *dds3AddCameraObjectBasicTypeLookAt(s32 key, u128 *targetPosition, u128 *worldEye, u128 *localUp);

/* Field travel and encounter providers. Integer/FPU argument classes have
 * independent register sequences; the field-controller call establishes
 * step-before-index, and all callers are verified. */
extern void evtSubmitEventRequest(s32 eventId, s32 requestMode);
extern void evtSubmitEventRequestImmediate(s32);
#include "dds3_world_primary.h"
extern void fldUpdateCameraHeadingTransition(void);
extern u8 fldAutoMapSeqTestFlag(u32);

extern f32 D_0038A6F0[4];
extern void evtAdvanceSolarClockAndExpireControls(f32 travelDistance);
extern void evtUpdateFlaggedStats(void);
extern void ptyApplyFieldHpAttrition(s32 attritionMode);
extern s32 btlAdvanceConditionalEncounterRoll(f32 step, s32 encounterIndex);

extern s32 fldNext_fld;






#include "dds3_object_payload.h"

extern dds3Basic_t *D_00435F60;

extern FldSequenceController fldAutoMapSeqFlags;

extern s16 D_00389874[];

extern u32 D_00435CBC;

extern u32 D_004360E8;

extern s32 D_003898AC[];

extern void fldInitializeAreaWork(void);

extern void fldRebuildFieldScene(void);

extern void fldApplySkyLightSet(u32 skyLightSetIndex, s32 durationMode);

extern void fldRunDanteClear(void);


extern s32 fldCanUseTargetGuideInCurrentScene(s32, s32);

extern void fldResetSecondaryArchiveLoadPhase(void);

extern FldArchiveLoadResult fldStepSecondaryArchiveLoad(void);

extern s32 fldReportCampVolumeError(void);

extern void fldApplyCurrentAreaActorEntries(void);

extern s32 fldIsTargetWithinInteractionRange(void);

extern void fldUpdateSparkEncounterScene(void);

extern s32 fldGetSparkDialogPhase(void);

extern void fldUpdateDevilizeEventSequence(void);

extern void fldUpdateTargetGuideController(void);

extern void fldNoopWithMode(s32 mode);

extern s32 fldUpdateTravelAndEncounters(void);

extern void fldInitInputPanel(void);

extern void fldInitAddressProcess(void);

extern void fldSetSnowActive(void);

extern void fldFinishDeferredExit(void);

extern void func_001447A0(void);

/* Providers used by the field lifecycle controller. */

extern void evtEnableMoonCalc(void);

extern s32 fileMenuTaskExists(void);

extern void fldSkySetTableAnime(u32);

extern void fldCheckSceneReady(void);

extern void fldFlushQueuedEffectPositions(void);

extern FldArchivePendingState fldGetArchiveLoadPending(void);

extern void fldLoadSceneModelsAndCamera(void);

extern void fldPrepareSceneBgmArchive(void);

extern s32 fldEveHit_ChkEveHitAfter(void);


extern void fldStartDeferredFieldExit(void);

extern void fldStartSceneBgm(void);

extern void fldStartSceneBgmAlternate(void);

extern s32 fldPlayFieldBgm(void);

extern s32 fldUpdateCameraFollow(void);

extern s32 fldUpdateCameraFrame(void);

extern void fldSkyCalcFilter(void);

extern void fldUpdateSelectedValueTransition(void);

extern void fldUpdateRoomTriggers(dds3Basic_t *);

extern void fldUpdateObjectSlotActivation(void);

#include "mnu_title_stream.h"





extern u32 fldEncountFlags;

extern s32 D_00435F70;

extern FieldPlayerSceneWork fldSceneParam;

extern struct sdfMemHandle_t *fldPlayerModelResource;

extern u32 D_003898B8[];

extern struct dds3Basic_t *fldPlayerObject;

extern u32 D_00389858[];

void fldEncountClearFlag(u32 mask);

void fldLightMaClear(void);

extern s32 D_0038989C[];

void fldRifutoMaClear(void);

void fldRiberaMaClear(void);

void fldEsutoMaClear(void);

extern char D_00412F30[];


extern char D_00412F58[];

extern char D_00412F40[];

extern s32 D_00435F7C;

u8 fldEncountTestFlag(u32 mask);

extern s32 D_00435F80;


extern u32 fldDeferredCommandParameter;

extern void fldDispatchSceneAction(s32 sceneAction);

extern void dds3WorkClear(void);

extern char D_00412D50[]; /* "fldProcSequence" */

extern u8 D_00435F24;

extern u8 D_00387D60[];

extern struct sdfMemHandle_t *D_00435F38;

extern u32 D_00435F44;

extern u32 D_00435F40;

extern u32 D_00435F3C;

extern void mdlPackDecode(s32, s32, s32, const u8 *, s32);

void fldLoadPlayerModel(void);

extern u32 D_00389790[];




extern void fldFreeNextFile(void);
struct dds3Basic_t;


extern f32 fldCalcAngleBetween(f32 fromX, f32 fromZ, f32 toX, f32 toZ);

extern u8 D_0037F530[];

extern u32 fldRunDanteGunFlag[];

extern u32 fldRunDanteGunFlag[];

extern u32 fldRunDanteGunFlag[];


extern s32 gFldElvFloorNow;

extern s32 fldElvFloorFrom;

extern s32 fldElvFloorNum;

extern s32 fldElvFloorTo;

extern s32 fldElvFloorDisp;

extern s32 fldElvFloorTarget;

extern s32 fldElvState;




extern void sdfResetGameRuntime(s32);

extern s32 fileLoadStateChanged(void);

extern void fileCacheSlotFlagsFromState(void);

extern void fileRestoreSlotFlagsToState(void);

void fldSetDeferredFieldCommand(u32, u32);

extern void dds3AdminiCallSequence(s32, const void *, u32, s32);


extern void fldResetPlayerSceneTransformState(void);

extern u32 D_00435F78;

extern void fldLoadNextFile_SetId_BtlRet(s32 area, s32 floor);

void fldUnloadPlayerModel(void);



extern void dds3RebuildObjectModelContext(dds3Basic_t *object, s32 resourceGroup, s32 resourceId);

extern dds3Basic_t *dds3AddUnitObjectBasic(s32, u128 *, u128 *);

extern s32 D_0038A67C[];

extern char D_00412EC0[]; /* "PLAYER_UNIT" */

extern s32 D_00435F30;

extern SdfPoolNode D_0037FF88[];

extern SdfPoolNode *kwlnSceneSurfaceSets[13][4];


#include "fld_encounter_roll_entry.h"

extern FldEncEntry *fldEncounterRollTable;



/* Mode 14 copies this eight-byte request. The packed selection contains
 * group:8, entry:8, and scene:16; its consumer reads both words as signed. */
#include "fld_encounter_request.h"
extern FieldEncounterRequest D_00435F98;

extern void fldResetEventSceneState(void);

extern s32 fldGetSparkEncounterSelection(void);

extern void fldEveHit_End(void);

extern void fldAutoMapSeqSetFlag(u32);

extern void evtKillFieldScript(void);

extern void func_00150800(void);

extern s32 D_00435F84;
extern EffBlurScatterWork *D_00435F74;

extern char D_00412FD0[], D_00412FE0[];

extern void kwlnFadeResetBackground(void);

extern void fldFreeInputPanel(void), evtDisableMoonDraw(void), fldFreeAddressProcess(void);

extern void mnuDestroyCampTasks(void), scrDestroyAllNamedProcesses(void);

extern void fldReleaseMenuSlotsAfterWait(void);

void fldEncountSetFlag(u32 mask);

s32 fldPlayerCameraTargetNoPathUpdate(dds3PoseSet_t *relativeTransform, dds3Basic_t *target);

extern void fldUpdateCameraTarget(void);

extern void fldRefreshAreaStateFromPosition(f32 *position);

extern char D_00435EB8[];

extern char D_00435EC0[];

extern char D_00435EC8[];

extern char D_00435ED0[];

extern char D_00435ED8[];

/* Search location records 1..639; a miss returns the reserved first record, * not NULL, so callers can read the fallback record's fields. */ const s16 *fldFindLocationCoordinateRecord(s32 area, s32 locationIndex);

/* Read the matching location record's signed flags; zero on miss. */ s16 fldGetLocationCoordinateFlags(s32 area, s32 locationIndex);

void InitOoyTest(void);

void KillOoyTest(void);

/* Return to-minus-from after integer-degree reduction and one wrap adjustment. */
f32 fldAngleDifference(f32 fromAngle, f32 toAngle) {
    f32 difference;

    if (fromAngle < 0.0f || toAngle < 0.0f) {
        fromAngle += 360.0f;
        toAngle += 360.0f;
    }
    fromAngle = (s32)fromAngle % 360;
    toAngle = (s32)toAngle % 360;
    difference = fromAngle - toAngle;
    if (difference > 180.0f || difference < -180.0f) {
        if (fromAngle < toAngle) {
            fromAngle += 360.0f;
        } else {
            toAngle += 360.0f;
        }
    }
    return toAngle - fromAngle;
}

f32 fldSnapAngleToCompassPoint(f32 angle) {
    f32 compass[16] = {0.0f, 22.5f, 45.0f, 67.5f, 90.0f, 112.5f, 135.0f, 157.5f, 180.0f, 202.5f, 225.0f, 247.5f, 270.0f, 292.5f, 315.0f, 337.5f};
    f32 best;
    s32 index;
    f32 diff;

    while (angle >= 360.0f) {
        angle -= 360.0f;
    }
    while (angle < 0.0f) {
        angle += 360.0f;
    }
    best = 900.0f;
    index = -1;
    diff = fabsf(fldAngleDifference(0.0f, angle));
    if (diff < best) {
        best = diff;
        index = 0;
    }
    diff = fabsf(fldAngleDifference(22.5f, angle));
    if (diff < best) {
        best = diff;
        index = 1;
    }
    diff = fabsf(fldAngleDifference(45.0f, angle));
    if (diff < best) {
        best = diff;
        index = 2;
    }
    diff = fabsf(fldAngleDifference(67.5f, angle));
    if (diff < best) {
        best = diff;
        index = 3;
    }
    diff = fabsf(fldAngleDifference(90.0f, angle));
    if (diff < best) {
        best = diff;
        index = 4;
    }
    diff = fabsf(fldAngleDifference(112.5f, angle));
    if (diff < best) {
        best = diff;
        index = 5;
    }
    diff = fabsf(fldAngleDifference(135.0f, angle));
    if (diff < best) {
        best = diff;
        index = 6;
    }
    diff = fabsf(fldAngleDifference(157.5f, angle));
    if (diff < best) {
        best = diff;
        index = 7;
    }
    diff = fabsf(fldAngleDifference(180.0f, angle));
    if (diff < best) {
        best = diff;
        index = 8;
    }
    diff = fabsf(fldAngleDifference(202.5f, angle));
    if (diff < best) {
        best = diff;
        index = 9;
    }
    diff = fabsf(fldAngleDifference(225.0f, angle));
    if (diff < best) {
        best = diff;
        index = 10;
    }
    diff = fabsf(fldAngleDifference(247.5f, angle));
    if (diff < best) {
        best = diff;
        index = 11;
    }
    diff = fabsf(fldAngleDifference(270.0f, angle));
    if (diff < best) {
        best = diff;
        index = 12;
    }
    diff = fabsf(fldAngleDifference(292.5f, angle));
    if (diff < best) {
        best = diff;
        index = 13;
    }
    diff = fabsf(fldAngleDifference(315.0f, angle));
    if (diff < best) {
        best = diff;
        index = 14;
    }
    diff = fabsf(fldAngleDifference(337.5f, angle));
    if (diff < best) {
        index = 15;
    }
    return compass[index];
}

/* Snap an angle in degrees to the nearest of the eight compass directions. */
f32 fldPlayerCalc360_8(f32 angle) {
    f32 best;
    s32 index;
    f32 diff;
    while (angle >= 360.0f) {
        angle -= 360.0f;
    }
    while (angle < 0.0f) {
        angle += 360.0f;
    }
    best = 900.0f;
    index = -1;
    diff = fabsf(fldAngleDifference(0.0f, angle));
    if (diff < best) {
        best = diff;
        index = 0;
    }
    diff = fabsf(fldAngleDifference(45.0f, angle));
    if (diff < best) {
        best = diff;
        index = 1;
    }
    diff = fabsf(fldAngleDifference(90.0f, angle));
    if (diff < best) {
        best = diff;
        index = 2;
    }
    diff = fabsf(fldAngleDifference(135.0f, angle));
    if (diff < best) {
        best = diff;
        index = 3;
    }
    diff = fabsf(fldAngleDifference(180.0f, angle));
    if (diff < best) {
        best = diff;
        index = 4;
    }
    diff = fabsf(fldAngleDifference(225.0f, angle));
    if (diff < best) {
        best = diff;
        index = 5;
    }
    diff = fabsf(fldAngleDifference(270.0f, angle));
    if (diff < best) {
        best = diff;
        index = 6;
    }
    diff = fabsf(fldAngleDifference(315.0f, angle));
    if (diff < best) {
        index = 7;
    }
    switch (index) {
    case 0:
        angle = 0.0f;
        break;
    case 1:
        angle = 45.0f;
        break;
    case 2:
        angle = 90.0f;
        break;
    case 3:
        angle = 135.0f;
        break;
    case 4:
        angle = 180.0f;
        break;
    case 5:
        angle = 225.0f;
        break;
    case 6:
        angle = 270.0f;
        break;
    case 7:
        angle = 315.0f;
        break;
    }
    return angle;
}

extern f32 fldAngleDifference(f32 fromAngle, f32 toAngle);

f32 fldPlayerCalc360_4(f32 angle) {
    f32 best;
    s32 index;
    f32 diff;
    while (angle >= 360.0f) {
        angle -= 360.0f;
    }
    while (angle < 0.0f) {
        angle += 360.0f;
    }
    best = 900.0f;
    index = -1;
    diff = fabsf(fldAngleDifference(0.0f, angle));
    if (diff < best) {
        best = diff;
        index = 0;
    }
    diff = fabsf(fldAngleDifference(90.0f, angle));
    if (diff < best) {
        best = diff;
        index = 2;
    }
    diff = fabsf(fldAngleDifference(180.0f, angle));
    if (diff < best) {
        best = diff;
        index = 4;
    }
    diff = fabsf(fldAngleDifference(270.0f, angle));
    if (diff < best) {
        index = 6;
    }
    switch (index) {
    case 0:
        angle = 0.0f;
        break;
    case 2:
        angle = 90.0f;
        break;
    case 4:
        angle = 180.0f;
        break;
    case 6:
        angle = 270.0f;
        break;
    }
    return angle;
}


/* Direction in degrees from two X/Z positions, preserving the field's 3.14 PI. */
f32 fldCalcAngleBetween(f32 fromX, f32 fromZ, f32 toX, f32 toZ) {
    f32 dx;
    f32 dz;
    f32 result;
    f32 absZ;
    f32 angle;

    if (fromX == toX && fromZ == toZ) return 0.0f;
    result = fromX - toX;
    dx = result;
    dz = fromZ - toZ;
    absZ = dz;
    if (result < 0.0f) result = -result;
    if (absZ < 0.0f) absZ = -absZ;
    angle = sdfAtan2(absZ, result) * (180.0f / 3.14f);
    if (dx < 0.0f) {
        if (!(dz < 0.0f)) angle = -angle;
    } else {
        if (dz < 0.0f) angle = 180.0f - angle;
        else angle += 180.0f;
    }
    result = angle;
    return result;
}

f32 GetSphericalPosMove(f32 fromX, f32 fromZ, f32 toX, f32 toZ) {
    s32 angle = (s32)(360.0f - fldCalcAngleBetween(fromX, fromZ, toX, toZ) + 90.0f);
    return (f32)(angle % 360);
}

INCLUDE_ASM(const s32, "game/code_00124040", fldCamCalcSlopeOffset);

f32 fldPointDistance(f32 ax, f32 ay, f32 az, f32 bx, f32 by, f32 bz) {
    f32 dx = ax - bx;
    f32 dy = ay - by;
    f32 dz = az - bz;
    return fsqrtf(dx * dx + dy * dy + dz * dz);
}

/* For kind-6 objects in mode 4, set mode 3 or clear it to 0.
 * status covers the count, payload mode word and cursor result. */
void fldToggleWorldNodeState(s64 clearMode) {
    WorldIndexNode *valueChain;
    s32 status;
    dds3Basic_t *worldObject;

    valueChain = dds3GetControlScene();
    valueChain = dds3SceneBasicGetList(valueChain, 6);
    status = dds3GetBasicArrayCount((dds3BasicArrayHandle_t *)valueChain);
    if (status == 0) {
        return;
    }
    dds3SetFirstBasicArray((dds3BasicArrayHandle_t *)valueChain);
    do {
        worldObject = dds3GetBasicArray((dds3BasicArrayHandle_t *)valueChain);
        status = dds3MatterObjectBasicGetAlpMode(worldObject);
        if (status == 4) {
            if (clearMode == 0) {
                dds3MatterObjectBasicSetAlpMode(worldObject, 3);
            }
            else {
                dds3MatterObjectBasicSetAlpMode(worldObject, 0);
            }
        }
        status = dds3SetNextBasicArray((dds3BasicArrayHandle_t *)valueChain);
    } while (status != 0);
    dds3FreeBasicArrayHandle(valueChain);
}

void fldPrepareDeferredSceneTransition(void) {
    if (DDS3_GBWK->header.transition != 0) {
        sdfResetGameRuntime(1);
    } else {
        sdfResetGameRuntime(0);
    }
    if (fileLoadStateChanged() == 0) {
        fileCacheSlotFlagsFromState();
    } else {
        fileRestoreSlotFlagsToState();
    }
    evtBitOn(0xc0f);
    {
        s32 code;
        fldRunDanteGunFlag[0x44 / 4] = 1;
        code = 0x259;
        fldRunDanteGunFlag[0x4c / 4] = 0;
        fldSetDeferredFieldCommand(0x15, 0x259);
        dds3AdminiCallSequence(6, &code, 4, 0);
    }
}

extern char D_00435F48[];

void fldSetSceneParam2(fldSceneParam_t *record, s32 stage, s32 kind, const char *name);

void fldMainLoadStart(void) {
    fldSceneParam_t record;
    u32 mode;

    if (fileGetSelectionPendingFlag() == FILE_SELECTION_FLOW_PENDING) {
        return;
    }
    if (DDS3_GBWK->header.transition != 0) {
        mode = 1;
        dds3AdminiCallSequence(0x1E, &mode, 4, 0);
        return;
    }
    fldResetFieldRuntimeState(FLD_RUNTIME_RESET_PRESERVE_MAP_STATE);
    fldRunDanteGunFlag[0x4C / 4] = 0;
    fldRunDanteGunFlag[0x44 / 4] = 1;
    evtBitOn(0xC0F);
    fldSetDeferredFieldCommand(0, 0);
    fldSetSceneParam2(&record, 1, 1, D_00435F48);
    record.options = 1;
    dds3AdminiCallSequence(5, &record, 0xA0, 0);
}

extern void fldDrawInit();

extern void fldSe_Init();

extern void sdfRandomize(void *state, u32 seed);

extern void fldParseMixLb();

extern void fldLoadBattleSkyAndFilter();

extern void fldResetSceneResourceTables();

extern void fldLoadFieldTablesAndIndexStages();

extern u8 D_0038A6E0[];

void fldFirstInit(void) {
    fldDrawInit();
    fldResetPlayerSceneTransformState();
    fldSe_Init();
    sdfRandomize(D_0038A6E0, 0x1E240);
    fldParseMixLb();
    fldLoadBattleSkyAndFilter();
    fldResetSceneResourceTables();
    fldLoadFieldTablesAndIndexStages();
}

void fldResetPlayerSceneTransformState(void) {
    FieldPlayerSceneWork *sceneWork = &fldSceneParam;
    VU0_STORE_VF(vf0, &sceneWork->position);
    VU0_STORE_VF(vf0, &sceneWork->rotation);
    fldPlayerObject = 0;
    *fldGetPlayerSceneStateAddress() = 0;
}

/* Fill a sequence packet for stage/kind/name, then clear the temporary override.
 * Unspecified packet bytes deliberately retain their previous contents. */
void fldSetSceneParam2(fldSceneParam_t *record, s32 stage, s32 kind, const char *name) {
    if (dds3SearchProcessName(D_00412D50) != NULL) {
        if (fldGlobalBase.area == stage) {
            fldGlobalBase.reuseAreaResources = FLD_AREA_RESOURCE_REUSE_ENABLED;
        } else {
            dds3WorkClear();
        }
    }
    record->unk_30 = 0;
    record->sequenceWord3C = 0;
    record->sequenceWord38 = 0;
    strcpy(record->name, name);
    record->stage = stage;
    record->retainCachedAreaResource = 1;
    record->mode = 2;
    record->kind = kind;
    record->code = 0;
    record->sequenceMode = 0;
    record->link = 0;
    memset(record->detail, 0, sizeof(record->detail));
    memset(record->note, 0, sizeof(record->note));
    record->options = 0;
    D_00435F24 = 0;
    D_00387D60[0] = 0;
}

/* Fill the alternate-mode sequence packet for stage/kind/name.
 * Reuse a running sequence in the same area; otherwise request a work clear. */
void fldSetSceneParam3(fldSceneParam_t *record, s32 stage, s32 kind, const char *name) {
    if (dds3SearchProcessName(D_00412D50) != NULL) {
        if (fldGlobalBase.area == stage) {
            fldGlobalBase.reuseAreaResources = FLD_AREA_RESOURCE_REUSE_ENABLED;
        } else {
            dds3WorkClear();
        }
    }
    record->unk_30 = 0;
    record->sequenceWord3C = 0;
    record->sequenceWord38 = 0;
    strcpy(record->name, name);
    record->stage = stage;
    record->retainCachedAreaResource = 1;
    record->mode = 3;
    record->kind = kind;
    record->code = 0;
    record->sequenceMode = 0;
    record->link = 0;
    memset(record->detail, 0, sizeof(record->detail));
    memset(record->note, 0, sizeof(record->note));
    record->options = 0;
}

/* Fill a field sequence packet, including its narrowed code, link and detail name.
 * A running sequence in the same area requests reuse rather than a work clear. */
void fldSetSceneParam4(fldSceneParam_t *record, s32 stage, s32 kind, const char *name,
                   s32 code, s32 link, const char *subname) {
    if (dds3SearchProcessName(D_00412D50) != NULL) {
        if (fldGlobalBase.area == stage) {
            fldGlobalBase.reuseAreaResources = FLD_AREA_RESOURCE_REUSE_ENABLED;
        } else {
            dds3WorkClear();
        }
    }
    record->unk_30 = 0;
    record->sequenceWord3C = 0;
    record->sequenceWord38 = 0;
    strcpy(record->name, name);
    record->stage = stage;
    record->kind = kind;
    record->retainCachedAreaResource = 1;
    record->mode = 2;
    record->code = code;
    record->sequenceMode = 0;
    record->link = link;
    strcpy(record->detail, subname);
    memset(record->note, 0, sizeof(record->note));
    record->options = 0;
}

/* Fill a sequence packet with a narrowed code and note instead of a detail/link.
 * A running sequence in the same area requests reuse rather than a work clear. */
void fldSetSceneParam5(fldSceneParam_t *record, s32 stage, s32 kind, const char *name,
                    s32 code, const char *subname) {
    if (dds3SearchProcessName(D_00412D50) != NULL) {
        if (fldGlobalBase.area == stage) {
            fldGlobalBase.reuseAreaResources = FLD_AREA_RESOURCE_REUSE_ENABLED;
        } else {
            dds3WorkClear();
        }
    }
    record->unk_30 = 0;
    record->sequenceWord3C = 0;
    record->sequenceWord38 = 0;
    strcpy(record->name, name);
    record->stage = stage;
    record->retainCachedAreaResource = 1;
    record->mode = 2;
    record->kind = kind;
    record->code = code;
    record->sequenceMode = 0;
    record->link = 0;
    memset(record->detail, 0, sizeof(record->detail));
    strcpy(record->note, subname);
    record->options = 0;
}

void fldSetSceneParamLoad(fldSceneParam_t *record, s32 stage, s32 kind, const char *name,
                    s32 code, s32 link, const char *subname) {
    record->unk_30 = 0;
    record->sequenceWord3C = 0;
    record->sequenceWord38 = 0;
    strcpy(record->name, name);
    record->retainCachedAreaResource = 1;
    record->stage = stage;
    record->kind = kind;
    record->code = code;
    record->link = link;
    record->mode = 2;
    record->sequenceMode = 0;
    strcpy(record->detail, subname);
    memset(record->note, 0, sizeof(record->note));
    record->options = 0;
}

FldSceneStatus fldGetRestoreMode(void) {
    FieldPlayerSceneWork *sceneWork = &fldSceneParam;

    if (sceneWork->primaryState == 1) {
        return FLD_SCENE_STATUS_BATTLE;
    }
    if (sceneWork->secondaryState == 3) {
        return FLD_SCENE_STATUS_ANA;
    }
    if (sceneWork->secondaryState == 4) {
        return FLD_SCENE_STATUS_SISETU;
    }
    if (sceneWork->secondaryState == 5) {
        return FLD_SCENE_STATUS_MINI_TITLE_UNLOCK;
    }
    if (sceneWork->secondaryState == 6) {
        return FLD_SCENE_STATUS_EVENT;
    }
    return sceneWork->primaryState != 0 ? FLD_SCENE_STATUS_EVENT : FLD_SCENE_STATUS_DEFAULT;
}

extern FieldAreaEncounterTable *D_00435E08;
extern void fldInitializeActorLinkedSequence(s32 mode, s32 index, fldSceneParam_t *sequence);

extern f32 *dds3GetTransformSourceData(dds3Basic_t *node);
extern void fldStartMiniTitleForUnlock(s32 area);
extern void FromRotation(f32 angle);
extern void fldApplySceneLightSet(s32 lightSetIndex, s32 durationMode);
extern void fldSpawnPlayerUnits(f32 *transform);
extern void fldCreateChaseCameraObject(void);
extern void fldPreparePlayerSceneCameraTarget(void);
extern void fldCreateSecondaryWorldCamera(void);
extern void fldUpdateCameraViewFromPlayer(void);
extern void fldInitializeCameraViewFromPlayer(void);
extern void func_0012F400(void);
extern void fldActivateCameraColorSetting(s32 enabled);
extern void mnuInitializeResourceEntries(void);
extern void fldLoadFieldEffectTextureSlots(void);
extern void fldLoadResourceByIndex(s32 index);

INCLUDE_RODATA(const s32, "game/code_00124040", D_00412D50);

void fldRebuildFieldScene(void) {
    f32 fallback[12];
    f32 *transform = NULL;
    FieldPlayerSceneWork *sceneWork;
    dds3Basic_t *node;
    FieldAreaEncounterTable *encounters;
    FieldAreaEncounterRow *room;
    FldAreaResourceReuseState reuseAreaResources;
    s32 areaIndex;
    s32 i;
    s32 selection;
    s32 flag;
    s32 encounter;
    s32 cameraLinkMode;
    dds3PoseSet_t *camera;

    memset(fallback, 0, sizeof(fallback));
    fallback[3] = 1.0f;
    fallback[7] = 1.0f;
    {
        f32 axis[4] = { 0.0f, 1.0f, 0.0f, 1.0f };

        sceneWork = &fldSceneParam;
        if (sceneWork->sequenceOption == 1) {
            /* This provider initializes the real sequence-packet layout in scene storage. */
            fldInitializeActorLinkedSequence(DDS3_GBWK->vr.previewX,
                DDS3_GBWK->vr.previewY, (fldSceneParam_t *)sceneWork);
        }
        reuseAreaResources = fldGlobalBase.reuseAreaResources;
        evtLoadArea(sceneWork->area, sceneWork->room);
        dds3SetActiveScene(NULL);
        if (sceneWork->primaryState == 0) {
            node = dds3SceneBasicSearchNameAndType(dds3GetControlScene(),
                0x11, (const u8 *)sceneWork->entryName);
            if (node != NULL) {
                transform = dds3GetTransformSourceData(node);
            } else {
                transform = fallback;
            }
            if (sceneWork->area < 40) {
                fldGlobalBase.defaultEncounterId = 0;
                encounters = D_00435E08;
                areaIndex = -1;
                for (i = 0; i < 16; i++) {
                    if (sceneWork->area == encounters[i].area) {
                        areaIndex = i;
                    }
                }
                if (areaIndex != -1) {
                    room = &encounters[areaIndex].rooms[sceneWork->room];
                    selection = 0;
                    flag = room->higherPriorityFlagId;
                    if (flag != -1 && flag != 0) {
                        if (evtBitCheck(flag)) {
                            selection = 2;
                        } else {
                            flag = room->lowerPriorityFlagId;
                            if (flag != -1 && flag != 0 && evtBitCheck(flag)) {
                                selection = 1;
                            }
                        }
                    } else {
                        flag = room->lowerPriorityFlagId;
                        if (flag != -1 && flag != 0) {
                            selection = evtBitCheck(flag) != 0;
                        }
                    }
                    switch (selection) {
                        case 1:
                            encounter = room->lowerPriorityEncounterId;
                            break;
                        case 2:
                            encounter = room->higherPriorityEncounterId;
                            break;
                        default:
                            encounter = room->defaultEncounter;
                            break;
                    }
                    fldGlobalBase.defaultEncounterId = encounter;
                }
            }
            if (fldGlobalBase.area < 31 && fldGetRestoreMode() != FLD_SCENE_STATUS_SISETU && reuseAreaResources == 0) {
                if (fldGetRestoreMode() == FLD_SCENE_STATUS_MINI_TITLE_UNLOCK) {
                    if (fldGlobalBase.cachedArea != fldGlobalBase.area) {
                        fldStartMiniTitleForUnlock(fldGlobalBase.area);
                    }
                } else if (fldGetRestoreMode() != FLD_SCENE_STATUS_EVENT) {
                    if (!((fldGlobalBase.cachedArea == 32 &&
                           (fldGlobalBase.area == 36 || fldGlobalBase.area == 37)) ||
                          (fldGlobalBase.cachedArea == 36 &&
                           (fldGlobalBase.area == 32 || fldGlobalBase.area == 37)) ||
                          (fldGlobalBase.cachedArea == 37 &&
                           (fldGlobalBase.area == 32 || fldGlobalBase.area == 36)))) {
                        fldStartMiniTitleForUnlock(fldGlobalBase.area);
                    }
                }
            }
        } else if (sceneWork->primaryState == 1) {
            transform = (f32 *)&sceneWork->position;
            SDF_VU0_LOADVECTOR(axis);
            FromRotation(3.14159265f);
            SDF_VU0_LOADVECTOR_B(transform + 4);
            effMiscQuatMultiplyVU();
            SDF_VU0_STOREVECTOR(transform + 4);
        }
        fldSpawnPlayerUnits(transform);
        if (fldCanUseTargetGuideInCurrentScene(fldGlobalBase.area, fldGlobalBase.floor + 1)) {
            fldCreateChaseCameraObject();
            fldGlobalBase.targetGuideEnabled = 1;
        }
        dds3ReplaceActiveUnitSceneBasic(dds3GetControlScene(), fldPlayerObject);
        fldPreparePlayerSceneCameraTarget();
        fldCreateSecondaryWorldCamera();
        if (sceneWork->primaryState == 0) {
            cameraLinkMode = sceneWork->cameraLinkMode;
            if (cameraLinkMode == 1) {
                node = dds3SceneBasicSearchNameAndType(dds3GetControlScene(),
                    4, (const u8 *)sceneWork->cameraName);
                if (node != NULL) {
                    camera = node->inner;
                    fldGlobalBase.mode = FLD_CAMERA_MODE_OBJECT_FOCUS;
                    fldGlobalBase.focusPos[0] = camera->position[0];
                    fldGlobalBase.focusPos[1] = camera->position[1];
                    fldGlobalBase.focusPos[2] = camera->position[2];
                    fldUpdateCameraViewFromPlayer();
                } else {
                    fldGlobalBase.mode = FLD_CAMERA_MODE_PLAYER_FOLLOW;
                    fldInitializeCameraViewFromPlayer();
                }
            } else {
                if (cameraLinkMode == 2) {
                    fldGlobalBase.mode = FLD_CAMERA_MODE_ALTERNATE_FOCUS;
                } else {
                    fldGlobalBase.mode = FLD_CAMERA_MODE_PLAYER_FOLLOW;
                }
                if (sceneWork->secondaryState == 3) {
                    func_0012F400();
                } else {
                    fldInitializeCameraViewFromPlayer();
                }
            }
        }
        fldGlobalBase.verticalStepDirection = 0;
        if (fldGetLocationCoordinateFlags(fldGlobalBase.area, fldGlobalBase.floor + 1) & 8) {
            fldActivateCameraColorSetting(1);
        } else {
            fldActivateCameraColorSetting(0);
        }
        if (fldGetLocationCoordinateFlags(fldGlobalBase.area, fldGlobalBase.floor + 1) & 0x800) {
            mnuInitializeResourceEntries();
        }
        if (fldGetLocationCoordinateFlags(fldGlobalBase.area, fldGlobalBase.floor + 1) & 0x40) {
            fldLoadFieldEffectTextureSlots();
        }
        if (fldGetLocationCoordinateFlags(fldGlobalBase.area, fldGlobalBase.floor + 1) & 0x200) {
            fldLoadResourceByIndex(0);
        }
        if (fldGetLocationCoordinateFlags(fldGlobalBase.area, fldGlobalBase.floor + 1) & 0x400) {
            fldLoadResourceByIndex(1);
        }
    }
}


struct sdfMemHandle_t;


/* Reload only when the field-selected variant changes or its handle is absent. */


void fldLoadPlayerModel(void) {
    s32 model;

    if (fldGetLocationCoordinateFlags(fldGlobalBase.area, fldGlobalBase.floor + 1) & 0x20) {
        model = 2;
    } else {
        model = fldGlobalBase.playerModelAlternate != 0;
    }
    if (fldGlobalBase.playerModelVariant != model || fldPlayerModelResource == NULL) {
        if (fldPlayerModelResource != NULL) {
            fldUnloadPlayerModel();
        }
        switch (model) {
        case 0:
            if (evtBitCheck(0x31)) {
                fldPlayerModelResource = sdfDevLoadFileEx("/model/field/player_13.PB", &D_00435F3C, &D_00435F44);
            } else if (evtBitCheck(0x25)) {
                fldPlayerModelResource = sdfDevLoadFileEx("/model/field/player_d.PB", &D_00435F3C, &D_00435F44);
            } else if (evtBitCheck(0x1C)) {
                fldPlayerModelResource = sdfDevLoadFileEx("/model/field/player_11.PB", &D_00435F3C, &D_00435F44);
            } else if (evtBitCheck(0x13)) {
                fldPlayerModelResource = sdfDevLoadFileEx("/model/field/player_f.PB", &D_00435F3C, &D_00435F44);
            } else if (evtBitCheck(0x290)) {
                fldPlayerModelResource = sdfDevLoadFileEx("/model/field/player_d.PB", &D_00435F3C, &D_00435F44);
            } else {
                fldPlayerModelResource = sdfDevLoadFileEx("/model/field/player_a.PB", &D_00435F3C, &D_00435F44);
            }
            break;
        case 1:
            fldPlayerModelResource = sdfDevLoadFileEx("/model/field/player_b.PB", &D_00435F3C, &D_00435F44);
            break;
        default:
            if (evtBitCheck(0x31)) {
                fldPlayerModelResource = sdfDevLoadFileEx("/model/field/player_13.PB", &D_00435F3C, &D_00435F44);
            } else if (evtBitCheck(0x25)) {
                fldPlayerModelResource = sdfDevLoadFileEx("/model/field/player_e.PB", &D_00435F3C, &D_00435F44);
            } else if (evtBitCheck(0x1C)) {
                fldPlayerModelResource = sdfDevLoadFileEx("/model/field/player_12.PB", &D_00435F3C, &D_00435F44);
            } else if (evtBitCheck(0x13)) {
                fldPlayerModelResource = sdfDevLoadFileEx("/model/field/player_10.PB", &D_00435F3C, &D_00435F44);
            } else if (evtBitCheck(0x290)) {
                fldPlayerModelResource = sdfDevLoadFileEx("/model/field/player_e.PB", &D_00435F3C, &D_00435F44);
            } else {
                fldPlayerModelResource = sdfDevLoadFileEx("/model/field/player_l.PB", &D_00435F3C, &D_00435F44);
            }
            break;
        }
        fldGlobalBase.playerModelVariant = model;
    }
}

void fldUnloadPlayerModel(void) {
    if (fldPlayerModelResource != NULL) {
        sdfMemFreeDelayedDirect(fldPlayerModelResource);
        fldPlayerModelResource = NULL;
        D_003898B8[0] = 0;
    }
}

typedef struct FieldVec4 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} FieldVec4;

extern FieldVec4 D_00412EB0;
extern void FromRotation(f32);
extern s32 D_00389888[];
extern dds3ModelHandle_t *fldPlayerModelContext;

/* Toggle the selected player model, preserving and rebuilding its scene pose.
 * Sable/Garnet/Tidal reconstruction; KiB compiler/ABI qualification.
 * The four legacy SDK stores are audited against this exact VU/callee-read
 * pipeline; they are not general-purpose memory barriers. */

extern void dds3DeleteUnitObjectBasic(dds3Basic_t *);
extern s32 fldSynchronizePlayerPositionAndAreaState(void);

void fldTogglePlayerModel(void) {
    f32 work[4];
    FieldPlayerSceneWork *sceneWork;
    dds3Basic_t *player;
    void *source;
    void *buffer;

    memset(work, 0, sizeof(work));
    work[3] = 1.0f;
    {
        FieldVec4 axis = D_00412EB0;

        player = fldPlayerObject;
        {
            sceneWork = &fldSceneParam;
            dds3GetPosPoseSet(player);
            SDF_VU0_STOREVECTOR_NOMEM(&sceneWork->position);
            dds3GetRotPoseSet(player);
            SDF_VU0_STOREVECTOR_NOMEM(&sceneWork->rotation);
        }
        dds3DeleteUnitObjectBasic(fldPlayerObject);
        fldUnloadPlayerModel();
        /* Rebind the same local for the rebuild phase. Both definitions feed
         * real phase uses; the resulting two-definition pseudo stays global,
         * rather than merging snapshot HIGH/base/+16 in local-alloc. */
        sceneWork = &fldSceneParam;
        fldGlobalBase.playerModelAlternate ^= 1;
        SDF_VU0_LOADVECTOR(&axis);
        FromRotation(3.1415926f);
        SDF_VU0_LOADVECTOR_B(&sceneWork->rotation);
        effMiscQuatMultiplyVU();
        SDF_VU0_STOREVECTOR_NOMEM(&sceneWork->rotation);
        SDF_VU0_LOADVECTOR(&axis);
        FromRotation(3.1415926f);
        SDF_VU0_LOADVECTOR_B(&sceneWork->rotation);
        effMiscQuatMultiplyVU();
        SDF_VU0_STOREVECTOR_NOMEM(work);
    }
    fldPlayerObject = dds3AddUnitObjectBasic(dds3AllocObjectId(), &sceneWork->position, (u128 *)work);
    dds3SetUnitObjectId(fldPlayerObject, D_00412EC0);
    dds3ReplaceActiveUnitSceneBasic(dds3GetControlScene(), fldPlayerObject);
    fldLoadPlayerModel();
    D_00435F40 = D_00435F44;
    D_00435F38 = sdfMemAllocDirect(D_00435F44);
    source = (void *)sdfMemGetPtr(fldPlayerModelResource);
    buffer = (void *)sdfMemGetPtr(D_00435F38);
    memcpy(buffer, source, D_00435F40);
    D_00435F3C = (u32)buffer;
    mdlPackDecode(2, 0, 0x101, buffer, D_00435F40);
    sdfMemFreeDelayedDirect(D_00435F38);
    D_00435F38 = 0;
    dds3RebuildObjectModelContext(fldPlayerObject, 2, fldSceneParam.sequenceWord3C);
    fldPlayerModelContext = (dds3ModelHandle_t *)dds3ObjectBaseGetModelHandle(fldPlayerObject);
    if (D_00389888[0] != 0) {
        mdlLoopMotion((dds3ModelHandle_t *)fldPlayerModelContext, 0, 2);
        mdlSetMotionSpeedEx((dds3ModelHandle_t *)fldPlayerModelContext, 1, 1.0f);
        mdlLoopMotion((dds3ModelHandle_t *)fldPlayerModelContext, 1, 2);
    } else {
        mdlLoopMotion((dds3ModelHandle_t *)fldPlayerModelContext, 0, 3);
        fldUnloadPlayerModel();
    }
    fldApplySceneLightSet(0, 0);
    fldApplySkyLightSet(fldRunDanteGunFlag[0x2C / 4], 0);
    fldSkySetTableAnime(fldRunDanteGunFlag[0x30 / 4]);
    fldSynchronizePlayerPositionAndAreaState();
}


void fldPrepareResourceBuffer(void) {
    void *source;
    void *buffer;
    fldLoadPlayerModel();
    D_00435F40 = D_00435F44;
    D_00435F38 = sdfMemAllocDirect(D_00435F44);
    source = (void *)sdfMemGetPtr(fldPlayerModelResource);
    buffer = (void *)sdfMemGetPtr(D_00435F38);
    memcpy(buffer, source, D_00435F40);
    D_00435F3C = (u32)buffer;
    mdlPackDecode(2, 0, 0x101, buffer, D_00435F40);
    sdfMemFreeDelayedDirect(D_00435F38);
    D_00435F38 = 0;
}

void fldReleaseResources(void) {
    if (D_00435F38 != 0) {
        sdfMemFreeDelayedDirect(D_00435F38);
        D_00435F38 = 0;
    }
    if (D_00389790[0] == 0 && D_00435F78 == 0) {
        fldUnloadPlayerModel();
        fldLoadNextFile_SetId_BtlRet(0, 0);
    }
}

extern dds3Basic_t *fldSecondarySceneObject;

extern dds3ModelHandle_t *fldPlayerModelContext;

extern dds3ModelHandle_t *fldSecondarySceneModelContext;

void fldSnapshotAndReleasePlayerSceneObject(void) {
    FieldPlayerSceneWork *sceneWork;
    if (fldPlayerObject != 0) {
        if (dds3GetControlScene() != 0) {
            sceneWork = &fldSceneParam;
            dds3GetPosPoseSet(fldPlayerObject);
            SDF_VU0_STOREVECTOR(&sceneWork->position);
            dds3GetRotPoseSet(fldPlayerObject);
            SDF_VU0_STOREVECTOR(&sceneWork->rotation);
        }
        fldPlayerObject = 0;
        fldSecondarySceneObject = 0;
        fldPlayerModelContext = 0;
        fldSecondarySceneModelContext = 0;
        *fldGetPlayerSceneStateAddress() = 0;
        fldReleaseResources();
    }
}

void fldReleaseCampSceneTasks(void);

void fldShutdownSceneTasksAndWorld(void) {
    fldEveHit_End();
    KillOoyTest();
    fldSnapshotAndReleasePlayerSceneObject();
    evtFreeStage();
    fldReleaseCampSceneTasks();
    fldResetTargetViewAndSound();
}

dds3Basic_t **fldGetPlayerSceneStateAddress(void) {
    return &D_00435F60;
}

dds3Basic_t *fldGetPlayerSceneState(void) {
    return *fldGetPlayerSceneStateAddress();
}

/* Raise the scene-state minima, clear pending slots and aim ten units above
 * the fetched player position. Existing larger mode/state values are retained. */
void fldPreparePlayerSceneCameraTarget(void) {
    f32 position[4];

    if (fldPlayerObject != 0) {
        fldEncountSetFlag(0x40);
        dds3ObjectBaseSetPrimaryControl(fldPlayerObject, fldPlayerCameraTargetNoPathUpdate);
    }
    fldGlobalBase.cameraTargetGraceTicks = 4;
    if (fldGlobalBase.sceneMode < 4) {
        fldGlobalBase.sceneMode = 4;
    }
    if (fldGlobalBase.sceneState < 5) {
        fldGlobalBase.sceneState = 5;
    }
    fldGlobalBase.primaryMovingMatterIndex = -1;
    fldGlobalBase.secondaryMovingMatterIndex = -1;
    fldUpdateCameraTarget();
    dds3GetPosPoseSet(fldPlayerObject);
    SDF_VU0_STOREVECTOR(position);
    position[1] += 10.0f;
    fldRefreshAreaStateFromPosition(position);
}

void fldResetPlayerSceneObjectState(void) {
    if (fldPlayerObject != 0) {
        fldEncountClearFlag(0x40);
        dds3ObjectBaseSetPrimaryControl(fldPlayerObject, 0);
    }
    D_00389858[0] = 4;
}

/* Initialize the homogeneous vectors and create/register the player if absent. */
void fldCreatePlayerObject(void) {
    f32 position[4];
    f32 rotation[4];

    memset(position, 0, sizeof(position));
    position[3] = 1.0f;
    memset(rotation, 0, sizeof(rotation));
    rotation[3] = 1.0f;
    if (fldPlayerObject == 0) {
        fldPlayerObject = dds3AddUnitObjectBasic(dds3AllocObjectId(), (u128 *)position, (u128 *)rotation);
        dds3SetUnitObjectId(fldPlayerObject, D_00412EC0);
        dds3ReplaceActiveUnitSceneBasic(dds3GetControlScene(), fldPlayerObject);
        if (D_00435F30 != 0) {
            dds3ObjectBaseOffFlag(fldPlayerObject, 0x20);
        }
        fldPrepareResourceBuffer();
        dds3RebuildObjectModelContext(fldPlayerObject, 2, D_0038A67C[0]);
    }
}

extern void FromRotation(f32);


extern void fldResetCameraModelHandles(void);




extern void fldActivatePendingSceneCommand(void);

/* The scene reset ignores the command word supplied by this caller. */
extern void fldClearCameraObjectActiveFlag();




extern char D_00412F00[];

extern f32 D_0038A610[12];

extern char D_00435F50[];

INCLUDE_RODATA(const s32, "game/code_00124040", D_00412EB0);

INCLUDE_RODATA(const s32, "game/code_00124040", D_00412EC0);

void fldSpawnPlayerUnits(f32 *input) {
    f32 work[4];
    dds3Basic_t *player;
    s32 locationFlags;

    memset(work, 0, sizeof(work));
    work[3] = 1.0f;
    {
        f32 axis[4] = {0, 1.0f, 0, 1.0f};
        f32 unitScale[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        f32 secondaryScale[4] = {1.0f, -1.0f, 1.0f, 1.0f};

        SDF_VU0_LOADVECTOR(axis);
        FromRotation(3.14159265f);
        SDF_VU0_LOADVECTOR_B(input + 4);
        effMiscQuatMultiplyVU();
        SDF_VU0_STOREVECTOR(work);

        player = fldPlayerObject;
        if (player == NULL) {
            player = dds3AddUnitObjectBasic(dds3AllocObjectId(), (u128 *)input, (u128 *)work);
            fldPlayerObject = player;
            dds3SetUnitObjectId(player, D_00412EC0);
            dds3ReplaceActiveUnitSceneBasic(dds3GetControlScene(), fldPlayerObject);
            if (D_00435F30 != 0) {
                dds3ObjectBaseOffFlag(fldPlayerObject, 0x20);
            }
            fldPrepareResourceBuffer();
            dds3RebuildObjectModelContext(fldPlayerObject, 2, D_0038A67C[0]);
            player = fldPlayerObject;
        } else {
            dds3PoseSet_t *inner = player->inner;
            PCP_COPY_VECTOR_F32(inner->position, input);
            PCP_COPY_VECTOR_F32(inner->smoothedPosition, input);
            PCP_COPY_VECTOR_F32(inner->rotation, input + 4);
        }

        dds3SetPosPoseSet(player, (u128 *)input);
        dds3SetRotPoseSet(fldPlayerObject, (u128 *)work);
        dds3SetSclPoseSet(fldPlayerObject, (u128 *)unitScale);
        fldPlayerModelContext = (dds3ModelHandle_t *)dds3ObjectBaseGetModelHandle(fldPlayerObject);
        dds3GetRadiusPoseSet(fldPlayerObject, 90.0f);

        if (fldGlobalBase.playerModelAlternate == 0) {
            mdlLoopMotion(fldPlayerModelContext, 0, 2);
        } else {
            mdlLoopMotion(fldPlayerModelContext, 0, 2);
            mdlLoopMotion(fldPlayerModelContext, 1, 2);
            fldResetCameraModelHandles();
        }
        sdfModelSetFaceDivParm(fldPlayerModelContext->inner, 15.0f, 0.0f);
        fldSecondarySceneObject = 0;

        locationFlags = fldGetLocationCoordinateFlags(fldGlobalBase.area, fldGlobalBase.floor + 1);
        if ((locationFlags & 4) != 0) {
            SDF_VU0_LOADVECTOR(axis);
            FromRotation(3.14159265f);
            SDF_VU0_LOADVECTOR_B(input + 4);
            effMiscQuatMultiplyVU();
            SDF_VU0_STOREVECTOR(work);
            fldSecondarySceneObject = dds3AddUnitObjectBasic(dds3AllocObjectId(), (u128 *)input, (u128 *)work);
            dds3SetUnitObjectId(fldSecondarySceneObject, D_00412F00);
            dds3RebuildObjectModelContext(fldSecondarySceneObject, 2, D_0038A67C[0]);
            dds3SetPosPoseSet(fldSecondarySceneObject, (u128 *)input);
            dds3SetRotPoseSet(fldSecondarySceneObject, (u128 *)work);
            dds3SetSclPoseSet(fldSecondarySceneObject, (u128 *)secondaryScale);
            fldSecondarySceneModelContext = (dds3ModelHandle_t *)dds3ObjectBaseGetModelHandle(fldSecondarySceneObject);
            dds3GetRadiusPoseSet(fldSecondarySceneObject, 90.0f);
            mdlLoopMotion(fldSecondarySceneModelContext, 0, 0);
            sdfModelSetFaceDivParm(fldSecondarySceneModelContext->inner, 15.0f, 0.0f);
            fldSecondarySceneModelContext->inner->flags |= 8;
        }

        if (fldSecondarySceneObject != 0) {
            dds3ObjectBaseOnFlag(fldPlayerObject, 0x400);
        } else {
            dds3ObjectBaseOnFlag(fldPlayerObject, 0x200);
        }

        if (fldGlobalBase.area < 0x14) {
            fldLightMaClear();
        }
        locationFlags = fldGetLocationCoordinateFlags(fldGlobalBase.area, fldGlobalBase.floor + 1);
        if ((locationFlags & 1) != 0) {
            fldGlobalBase.commandEnabled = 1;
        } else {
            fldGlobalBase.commandEnabled = 0;
            evtBitOff(0x818);
        }

        if (fldGlobalBase.area < 0xC8) {
            fldApplySceneLightSet(0, 0);
            fldApplySkyLightSet(fldRunDanteGunFlag[0x2C / 4], 0);
            fldSkySetTableAnime(fldRunDanteGunFlag[0x30 / 4]);
            if (fldGetSceneCommandState() == FLD_SCENE_COMMAND_STATE_PENDING) {
                if (fldGlobalBase.commandEnabled != 0) {
                    fldActivatePendingSceneCommand();
                } else {
                    evtBitOff(0x818);
                    fldGlobalBase.sceneLightSetOverrideIndex = 0;
                    fldApplySceneLightSet(0, 0);
                    fldApplySkyLightSet(fldRunDanteGunFlag[0x2C / 4], 0);
                    fldSkySetTableAnime(fldRunDanteGunFlag[0x30 / 4]);
                }
            } else if (fldGlobalBase.commandEnabled == 0) {
                evtBitOff(0x818);
                fldGlobalBase.sceneLightSetOverrideIndex = 0;
            }
            fldClearCameraObjectActiveFlag(fldGlobalBase.commandEnabled);
        }
        fldSynchronizePlayerPositionAndAreaState();
        dds3AddPointBasic(dds3AllocObjectId(), (EvtWorldSourceTransformPrefix *)D_0038A610, D_00435F50);
    }
}


extern FieldVec4 D_00412F10;

extern FieldVec4 D_00412F20;

extern char D_00435F58[];

extern void dds3CameraObjectBasicSetWorldEye(struct dds3Basic_t *camera, const u128 *worldEye);


/* Create the secondary camera at target origin with the stored eye/up vectors. */
void fldCreateSecondaryWorldCamera(void) {
    FieldVec4 localUp = D_00412F10;
    FieldVec4 targetPosition;
    FieldVec4 worldEye;
    dds3Basic_t **cameraObjectSlot = &D_00435F60;
    dds3Basic_t *cameraObject;
    memset(&targetPosition, 0, sizeof(targetPosition));
    targetPosition.w = 1.0f;
    worldEye = D_00412F20;
    cameraObject = dds3AddCameraObjectBasicTypeLookAt(dds3AllocObjectId(), (u128 *)&targetPosition, (u128 *)&worldEye, (u128 *)&localUp);
    *cameraObjectSlot = cameraObject;
    dds3SetUnitObjectId(cameraObject, D_00435F58);
    dds3CameraObjectBasicSetWorldEye(*cameraObjectSlot, (u128 *)&worldEye);
    dds3GetRadiusPoseSet(*cameraObjectSlot, 2.0f);
    dds3ReplaceActiveCameraSceneBasic(dds3GetControlScene(), *cameraObjectSlot);
}

s32 fldPlayerCameraTargetNoPathUpdate(dds3PoseSet_t *relativeTransform, dds3Basic_t *target) {
    return 0;
}

u32 fldGetDefaultSceneCommandValue(void) {
    return 100;
}

void fldLightMaClearMsg(void) {
    if ((DDS3_GBWK->world.fieldFlags & DAT_WORLD_FIELD_LIGHT_COMMAND_PENDING) != 0) {
        fldLightMaClear();
        fldGlobalBase.consumedFlags |= 1;
    }
}


extern void evtClearSolarOverlayControl(void);

void fldLightMaClear(void) {
    DDS3_GBWK->world.fieldFlags &= ~DAT_WORLD_FIELD_LIGHT_COMMAND_PENDING;
    fldGlobalBase.consumedFlags &= ~1;
    evtBitOff(0x818);
    evtClearSolarOverlayControl();
}

extern void fldPlayMenuSound(s32);


void fldActivatePendingSceneCommand(void) {
    s32 value;
    if (fldGlobalBase.commandEnabled != 0) {
        if ((DDS3_GBWK->world.fieldFlags & DAT_WORLD_FIELD_LIGHT_COMMAND_PENDING) == 0) {
            fldPlayMenuSound(0x29);
        }
        evtBitOn(0x818);
        value = fldGetDefaultSceneCommandValue();
        fldGlobalBase.sceneLightSetOverrideIndex = value;
        DDS3_GBWK->world.fieldFlags |= DAT_WORLD_FIELD_LIGHT_COMMAND_PENDING;
        fldGlobalBase.consumedFlags &= ~1;
        fldApplySceneLightSet((s16)value, 0);
    }
}

FldSceneCommandState fldGetSceneCommandState(void) {
    if ((DDS3_GBWK->world.fieldFlags & DAT_WORLD_FIELD_LIGHT_COMMAND_PENDING) != 0) {
        return FLD_SCENE_COMMAND_STATE_PENDING;
    }
    if (D_0038989C[0] != 0) {
        return FLD_SCENE_COMMAND_STATE_AVAILABLE;
    }
    return FLD_SCENE_COMMAND_STATE_UNAVAILABLE;
}

/* Latch the scene command while enabled and flagged; stop it when either gate
 * clears. Disable passes transition parameter 0; flag removal passes 20. */
void fldUpdateSceneCommandSpeed(void) {
    FldAreaWork *scene = &fldGlobalBase;
    s32 value;
    if (scene->commandEnabled == 0) {
        if (scene->sceneLightSetOverrideIndex != 0) {
            scene->sceneLightSetOverrideIndex = 0;
            fldApplySceneLightSet(0, 0);
        }
    } else if ((DDS3_GBWK->world.fieldFlags & DAT_WORLD_FIELD_LIGHT_COMMAND_PENDING) != 0) {
        if (scene->sceneLightSetOverrideIndex == 0) {
            value = fldGetDefaultSceneCommandValue();
            scene->sceneLightSetOverrideIndex = value;
            fldApplySceneLightSet((s16)value, 0x14);
        }
    } else if (scene->sceneLightSetOverrideIndex != 0) {
        scene->sceneLightSetOverrideIndex = 0;
        fldApplySceneLightSet(0, 0x14);
        fldPlayMenuSound(0x2A);
        evtBitOff(0x818);
        evtClearSolarOverlayControl();
    }
}

/* Area-state consumption markers are separate from shared game-state flags. */
enum {
    FIELD_TRANSITION_CONSUMED_FLAG = 2,
    FIELD_SECONDARY_SCENE_CONSUMED_FLAG = 4,
    FIELD_PRIMARY_SCENE_CONSUMED_FLAG = 8
};

/* Clear a set transition flag and mark its consumption in area state. */
void fldRifutoMaClearMsg(void) {
    if ((DDS3_GBWK->world.fieldFlags & DAT_WORLD_FIELD_TRANSITION_WORK) != 0) {
        fldRifutoMaClear();
        fldGlobalBase.consumedFlags |= FIELD_TRANSITION_CONSUMED_FLAG;
    }
}

extern void evtResetFieldTransitionCounter(void);

/* Clear the transition work flag and its area-state consumption marker. */
void fldRifutoMaClear(void) {
    DDS3_GBWK->world.fieldFlags &= ~DAT_WORLD_FIELD_TRANSITION_WORK;
    fldGlobalBase.consumedFlags &= ~FIELD_TRANSITION_CONSUMED_FLAG;
    evtResetFieldTransitionCounter();
}

/* Set the transition work flag and reset its area-state consumption marker. */
void fldRifutoMaStart(void) {
    DDS3_GBWK->world.fieldFlags |= DAT_WORLD_FIELD_TRANSITION_WORK;
    fldGlobalBase.consumedFlags &= ~FIELD_TRANSITION_CONSUMED_FLAG;
}

/* Return whether the transition work flag is set. */
u8 fldRifutoMaChk(void) {
    s32 fieldFlags = DDS3_GBWK->world.fieldFlags;
    fieldFlags &= DAT_WORLD_FIELD_TRANSITION_WORK;
    return fieldFlags != 0;
}

/* Clear a set secondary-scene flag and mark its consumption in area state. */
void fldRiberaMaClearMsg(void) {
    if ((DDS3_GBWK->world.fieldFlags & DAT_WORLD_FIELD_SECONDARY_SCENE_WORK) != 0) {
        fldRiberaMaClear();
        fldGlobalBase.consumedFlags |= FIELD_SECONDARY_SCENE_CONSUMED_FLAG;
    }
}

/* Clear the secondary-scene work flag and its consumption marker. */
void fldRiberaMaClear(void) {
    DDS3_GBWK->world.fieldFlags &= ~DAT_WORLD_FIELD_SECONDARY_SCENE_WORK;
    fldGlobalBase.consumedFlags &= ~FIELD_SECONDARY_SCENE_CONSUMED_FLAG;
}

/* Select secondary rather than primary, resetting secondary's consume marker. */
void fldRiberaMaStart(void) {
    DDS3_GBWK->world.fieldFlags = (DDS3_GBWK->world.fieldFlags | DAT_WORLD_FIELD_SECONDARY_SCENE_WORK) & ~DAT_WORLD_FIELD_PRIMARY_SCENE_WORK;
    fldGlobalBase.consumedFlags &= ~FIELD_SECONDARY_SCENE_CONSUMED_FLAG;
}

/* Return whether the secondary-scene work flag is set. */
u8 fldRiberaMaChk(void) {
    s32 fieldFlags = DDS3_GBWK->world.fieldFlags;
    fieldFlags &= DAT_WORLD_FIELD_SECONDARY_SCENE_WORK;
    return fieldFlags != 0;
}

/* Clear a set primary-scene flag and mark its consumption in area state. */
void fldEsutoMaClearMsg(void) {
    if ((DDS3_GBWK->world.fieldFlags & DAT_WORLD_FIELD_PRIMARY_SCENE_WORK) != 0) {
        fldEsutoMaClear();
        fldGlobalBase.consumedFlags |= FIELD_PRIMARY_SCENE_CONSUMED_FLAG;
    }
}

/* Clear the primary-scene work flag and its area-state consumption marker. */
void fldEsutoMaClear(void) {
    DDS3_GBWK->world.fieldFlags &= ~DAT_WORLD_FIELD_PRIMARY_SCENE_WORK;
    fldGlobalBase.consumedFlags &= ~FIELD_PRIMARY_SCENE_CONSUMED_FLAG;
}

/* Select primary rather than secondary, resetting primary's consume marker. */
void fldEsutoMaStart(void) {
    DDS3_GBWK->world.fieldFlags = (DDS3_GBWK->world.fieldFlags | DAT_WORLD_FIELD_PRIMARY_SCENE_WORK) & ~DAT_WORLD_FIELD_SECONDARY_SCENE_WORK;
    fldGlobalBase.consumedFlags &= ~FIELD_PRIMARY_SCENE_CONSUMED_FLAG;
}

/* Return whether the primary-scene work flag is set. */
u8 fldEsutoMaChk(void) {
    s32 fieldFlags = DDS3_GBWK->world.fieldFlags;
    fieldFlags &= DAT_WORLD_FIELD_PRIMARY_SCENE_WORK;
    if (fieldFlags == 0) return 0;
    return 1;
}

void fldConfigureRenderCleanupMessage(void) {
    sdfStoreMessageWordsAndNotifyConsumer(kwlnSceneSurfaceSets[7], &D_0037FF88[0], &D_0037FF88[6], &D_0037FF88[8], &D_0037FF88[7]);
}

enum {
    FIELD_ENCOUNTER_ROLL_ENTRY_COUNT = 16
};

#define FIELD_ENCOUNTER_PERCENT_RANGE 100U

/* Return the first successful stage/flag-gated roll, or -1 when none succeed.
 * Each eligible row rolls independently; chance retains its unsigned cast. */
s16 fldRollEncounter(void) {
    s32 encounterIndex;
    s32 eligible;

    for (encounterIndex = 0; encounterIndex < FIELD_ENCOUNTER_ROLL_ENTRY_COUNT; encounterIndex++) {
        if (fldEncounterRollTable[encounterIndex].stage == fldGlobalBase.area) {
            eligible = 1;
            if (fldEncounterRollTable[encounterIndex].flag != -1) {
                eligible = evtBitCheck(fldEncounterRollTable[encounterIndex].flag) != 0;
            }
            if (eligible != 0) {
                if ((u32)dds3GetRandA(0) % FIELD_ENCOUNTER_PERCENT_RANGE < (u32)fldEncounterRollTable[encounterIndex].chance) {
                    return fldEncounterRollTable[encounterIndex].result;
                }
            }
        }
    }
    return -1;
}

/* Advance movement-driven solar/attrition state and queue field encounters. */
s32 fldUpdateTravelAndEncounters(void) {
    f32 *previousPosition = D_0038A6F0;
    dds3Basic_t *player;
    f32 distance;
    f32 interval;
    s32 previousPhase;
    s32 phase;
    s32 encounter;
    s32 *selection;
    s32 floor;

    if (fldGlobalBase.sceneMode > 0) fldGlobalBase.sceneMode--;
    if (fldGlobalBase.sceneState > 0) fldGlobalBase.sceneState--;
    if (fldGlobalBase.cameraTargetGraceTicks > 0) fldGlobalBase.cameraTargetGraceTicks--;
    if (!fldIsTargetWithinInteractionRange() && !fldGlobalBase.targetGuideActive &&
        !fldEncountTestFlag(0x40)) return 0;
    if (fldAutoMapSeqTestFlag(1) == 1) return 0;
    if (!dds3GetActiveScene()) return 0;
    player = fldPlayerObject;
    if (!player) return 0;

    dds3GetPosPoseSet(player);
    SDF_VU0_LOADVECTOR_B(previousPosition);
    SDF_VU0_STOREVECTOR(previousPosition);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(distance);
    if (distance < 0.2f) distance = 0.0f;
    fldGlobalBase.frameTravelDistance = distance;
    previousPhase = evtGetAgeOfMoon16();
    if (fldGlobalBase.cameraTargetGraceTicks > 0) {
        if (fldGlobalBase.terrainAttritionKind != FLD_TERRAIN_ATTRITION_NONE && fldRunDanteGunFlag[0x60 / 4]) {
            fldGlobalBase.attritionDistance += distance;
        }
        dds3GetPosPoseSet(player);
        SDF_VU0_STOREVECTOR(previousPosition);
        fldGlobalBase.frameTravelDistance = 0.0f;
        return 0;
    }
    if (fldGlobalBase.specialEventActive != 0) return 0;
    evtAdvanceSolarClockAndExpireControls(distance);
    phase = evtGetAgeOfMoon16();
    if (fldGlobalBase.consumedFlags & 15) {
        fldResetPlayerSceneObjectState();
        evtStartSceneResourceTask(dds3GetActiveScene(), D_00412F30);
        fldUpdateCameraHeadingTransition();
        return 0;
    }
    if (!(fldGetLocationCoordinateFlags(fldGlobalBase.area, fldGlobalBase.floor + 1) & 0x40) &&
        previousPhase != phase) {
        evtUpdateFlaggedStats();
        if (phase == 0 && evtClearRandomStatusFlags() != EVT_RANDOM_STATUS_CLEAR_NO_FLAGS) {
            fldGlobalBase.sceneMode = 4;
            fldGlobalBase.sceneState = 5;
            fldResetPlayerSceneObjectState();
            evtStartSceneResourceTask(dds3GetActiveScene(), D_00412F40);
            return 0;
        }
    }
    if (fldGlobalBase.terrainAttritionKind != FLD_TERRAIN_ATTRITION_NONE ||
        ptyAnyLivingUnitHasStatus(0x80, PTY_PARTY_STATUS_SCOPE_FRONTLINE_ONLY)) {
        if (fldRunDanteGunFlag[0x60 / 4]) fldGlobalBase.attritionDistance += distance;
        interval = 450.0f;
        if (fldGlobalBase.terrainAttritionKind == FLD_TERRAIN_ATTRITION_NONE &&
            ptyAnyLivingUnitHasStatus(0x80, PTY_PARTY_STATUS_SCOPE_FRONTLINE_ONLY)) interval = 1000.0f;
        if (fldGlobalBase.attritionDistance > interval) {
            if (fldGlobalBase.terrainAttritionKind != FLD_TERRAIN_ATTRITION_NONE) {
                if (fldGlobalBase.terrainAttritionKind != FLD_TERRAIN_ATTRITION_SOUND_CLASS_20 && (DDS3_GBWK->world.fieldFlags & DAT_WORLD_FIELD_TRANSITION_WORK)) {
                    fldGlobalBase.areaDamageReactionMovementGate = 0;
                    fldGlobalBase.attritionDistance = 0.0f;
                } else {
                    fldGlobalBase.areaDamageReactionMovementGate = 60;
                    fldGlobalBase.attritionDistance = 0.0f;
                    if (fldGlobalBase.terrainAttritionKind == FLD_TERRAIN_ATTRITION_SOUND_CLASS_20) ptyApplyFieldHpAttrition(3);
                    else if (fldGlobalBase.terrainAttritionKind == FLD_TERRAIN_ATTRITION_SOUND_CLASS_30) ptyApplyFieldHpAttrition(4);
                    else ptyApplyFieldHpAttrition(2);
                }
            } else {
                fldGlobalBase.attritionDistance = 0.0f;
                fldGlobalBase.areaDamageReactionMovementGate = 60;
                ptyApplyFieldHpAttrition(1);
            }
        }
    } else {
        fldGlobalBase.attritionDistance = 450.0f;
    }
    if (fldGetNextFileLoadState() == FLD_NEXT_FILE_LOAD_PENDING) return 0;
    if (fldGlobalBase.targetGuideEnabled && fldRunDanteGunFlag[0x5C / 4] &&
        fldRunDanteGunFlag[0x44 / 4] && fldIsTargetWithinInteractionRange()) {
        fldEveHit_End();
        fldAutoMapSeqSetFlag(1);
        fldAutoMapSeqSetFlag(2);
        fldResetPlayerSceneObjectState();
        evtKillFieldScript();
        if (evtBitCheck(0x4D0)) {
            D_00435F98.tableIndex = 0;
            D_00435F98.encounterSelection = 0x32A;
            floor = fldGlobalBase.floor + 1;
            switch (floor) {
            case 11:
                evtBitOn(0x4C3);
                evtBitOn(0x80B);
                fldSetDeferredFieldCommand(23, 201);
                evtSubmitEventRequest(0x31E, 0);
                break;
            case 14:
                evtBitOn(0x4C2);
                evtSubmitEventRequestImmediate(0x32B);
                break;
            default:
                evtBitOn(0x4C1);
                evtSubmitEventRequestImmediate(0x32A);
                break;
            }
        } else {
            D_00435F98.tableIndex = 0;
            D_00435F98.encounterSelection = 0x302;
            fldSetDeferredFieldCommand(23, 200);
            floor = fldGlobalBase.floor + 1;
            if (floor == 11 || floor == 14) evtSubmitEventRequest(0x302, 0);
            else evtSubmitEventRequest(0x302, 0);
        }
        return -1;
    }
    if (!fldGlobalBase.specialEventActive && !fldRunDanteGunFlag[0x44 / 4]) return 0;
    if (!fldEncountTestFlag(0x10)) return 0;
    fldGlobalBase.encounterMode = FLD_ENCOUNTER_MODE_STANDARD;
    encounter = fldGlobalBase.encounterOverrideId;
    if (encounter == 0) encounter = fldGlobalBase.defaultEncounterId;
    D_00435F98.tableIndex = encounter;
    selection = &D_00435F98.encounterSelection;
    if ((*selection = btlAdvanceConditionalEncounterRoll(distance, encounter)) != 0) {
        encounter = fldRollEncounter();
        if (encounter != -1) {
            D_00435F98.tableIndex = encounter;
            *selection = btlAdvanceConditionalEncounterRoll(99999.0f, encounter);
            fldGlobalBase.encounterMode = FLD_ENCOUNTER_MODE_ALTERNATE;
        }
        if (*selection != 0) {
            if (fldRunDanteGunFlag[0x44 / 4] == 1) {
                fldGlobalBase.nextFloor = fldGlobalBase.floor + 1;
                fldGlobalBase.nextArea = fldGlobalBase.area;
                fldEveHit_End();
                fldAutoMapSeqSetFlag(1);
                fldAutoMapSeqSetFlag(2);
                fldResetPlayerSceneObjectState();
                evtKillFieldScript();
                return -1;
            }
            dds3WorkClear();
            fldGlobalBase.transitionCount++;
            p2sdPlaySE(15, 127, 63);
            dds3FadeIn(128, 128, 128, 6);
        }
    }
    return 0;
}


/* Queue the current area and next floor, reset the scene lifecycle, and return -1.
 * The pending pair is consumed later with a 200-entry offset on the area value. */
s32 fldAdvanceToNextScene(void) {
    u32 scene;
    u32 area;

    fldResetEventSceneState();
    D_00435F98.tableIndex = 0;
    D_00435F98.encounterSelection = fldGetSparkEncounterSelection();
    scene = fldGlobalBase.floor;
    area = fldGlobalBase.area;
    fldGlobalBase.nextArea = area;
    fldGlobalBase.nextFloor = scene + 1;
    fldEveHit_End();
    fldAutoMapSeqSetFlag(1);
    fldAutoMapSeqSetFlag(2);
    fldResetPlayerSceneObjectState();
    evtKillFieldScript();
    return -1;
}

u8 fldHasKiretaLabelProcess(void) {
    return scrSearchScriptName(D_00412F30) != 0;
}

u8 fldProcKiretaChk(void) {
    return scrSearchScriptName(D_00412F58) != 0;
}

u8 fldProcHirakenaiChk(void) {
    return scrSearchScriptName(D_00412F40) != 0;
}

FldCampCheckState fldProcCampChk(void) {
    if (D_00435F7C > 0) {
        return FLD_CAMP_CHECK_STATE_HANDOFF_DELAY;
    }
    if (mnuAcknowledgeCampState() != MNU_CAMP_STATE_ACK_NOT_ACTIVE) {
        return FLD_CAMP_CHECK_STATE_MENU_ACTIVE;
    }
    return fldEncountTestFlag(0x20) != 0
        ? FLD_CAMP_CHECK_STATE_ENCOUNTER_FLAG_SET
        : FLD_CAMP_CHECK_STATE_IDLE;
}





extern void func_00149A00(void);

extern void fldUpdateCameraHeadingTransition(void);

extern void fldSetCameraNodeModeWithTen(void);

extern void fldMarkStageExplorationCellsAroundPosition(s32, s32, f32, f32, f32);


extern void mnuCreateCampTasks(void);


extern void fldApplySkyLightSetToPlayerVU(void);
extern void fldAutoMapSeqClearFlag(u32 mask);

extern u8 fldAutoMapSeqTestFlag(u32 mask);

extern s16 D_00389898[];

/* Complete a delayed camp handoff or advance the player scene transition. */
s32 fldUpdateCampMenuTransition(void) {
    FldAreaWork *scene;
    s32 control;

    if (D_00435F7C > 0) {
        D_00435F7C--;
        if (D_00435F7C == 0) {
            dds3VisibleSceneBasic(dds3GetActiveScene(), 0);
            dds3FadeIn2(4);
            mnuCreateCampTasks();
        }
        return 0;
    }
    if (fldEncountTestFlag(0x20) == 0 && fldProcCampChk() == FLD_CAMP_CHECK_STATE_IDLE) {
        fldAutoMapSeqClearFlag(1);
        fldPreparePlayerSceneCameraTarget();
        evtSetSolarOverlayFullyVisible();
        /* Retail retains both branches of this shared scene-state gate. */
        if (D_00389898[0] != 0) {
            fldApplySkyLightSetToPlayerVU();
        } else {
            fldApplySkyLightSetToPlayerVU();
        }
        dds3VisibleSceneBasic(dds3GetActiveScene(), 1);
        fldEncountSetFlag(0x20);
        dds3FadeOut2(0);
        dds3FadeIn2(8);
        return 0;
    }
    control = fldEncountTestFlag(0x20);
    if (control == 0) {
        return control;
    }
    control = fldEncountTestFlag(0x40);
    if (control == 0) {
        return control;
    }
    if (fldAutoMapSeqTestFlag(1) == 1 || fldGetMovementHoldState() != FLD_MOVEMENT_HOLD_CLEAR ||
        fldGetSceneReadyFlag() != FLD_SCENE_NOT_READY) {
        return 0;
    }
    scene = &fldGlobalBase;
    if (scene->sceneMode == 0 && (s8)D_0037F530[0] < 0) {
        p2sdPlaySE(0xE, 0x7F, 0x3F);
        fldAutoMapSeqSetFlag(1);
        fldResetPlayerSceneObjectState();
        fldUpdateCameraHeadingTransition();
        evtDisableMoonDraw();
        scene->sceneMode = 4;
        scene->sceneState = 5;
        fldEncountClearFlag(0x20);
        dds3FadeOut(0, 0, 0, 4);
        D_00435F7C = 5;
        fldSetCameraNodeModeWithTen();
    }
    return 0;
}

u8 fldProcAmChk(void) {
    if (D_00435F80 > 0) {
        return 2;
    }
    return fldGetSceneReadyFlag() != FLD_SCENE_NOT_READY;
}

/* Gate next-floor input on camp/readiness flags, then start the native scene
 * transition or its delayed fade path. All return paths retain zero. */
s32 fldUpdateNextFloorTransition(void) {
    s32 transitionInput = 0;
    FldAreaWork *scene;

    if (fldProcCampChk() != FLD_CAMP_CHECK_STATE_ENCOUNTER_FLAG_SET) {
        return 0;
    }
    if (fldRunDanteGunFlag[0x58 / 4] & fldEncountTestFlag(0x40)) {
        if ((s8)D_0037F530[2] != 0) {
            transitionInput = 1;
        }
    } else if ((s8)D_0037F530[2] < 0) {
        transitionInput = 1;
    }
    scene = &fldGlobalBase;
    if (fldFindLocationCoordinateRecord(scene->area, scene->floor + 1)[2] <= 0) {
        if (scene->sceneMode == 0) {
            if (fldEncountTestFlag(0x40) != 0 && transitionInput != 0) {
                scene->sceneMode = 4;
                scene->sceneState = 5;
                fldResetPlayerSceneObjectState();
                evtStartSceneResourceTask(dds3GetActiveScene(), D_00412F58);
            }
        }
    } else {
        if (D_00435F80 > 0) {
            D_00435F80--;
            if (D_00435F80 == 0) {
                dds3VisibleSceneBasic(dds3GetActiveScene(), 0);
                dds3FadeIn2(4);
                func_00149A00();
            }
            return 0;
        }
        if (fldProcAmChk() != 0) {
            return 0;
        }
        fldMarkStageExplorationCellsAroundPosition(scene->floor, scene->flagStage, scene->x, scene->z, 50.0f);
        if (scene->sceneMode == 0) {
            if (fldGetMovementHoldState() != FLD_MOVEMENT_HOLD_CLEAR) {
                return 0;
            }
            if (fldEncountTestFlag(0x40) != 0 && transitionInput != 0 && fldRunDanteGunFlag[0x48 / 4] == 0) {
                scene->sceneMode = 4;
                scene->sceneState = 5;
                fldResetPlayerSceneObjectState();
                fldUpdateCameraHeadingTransition();
                dds3FadeOut(0, 0, 0, 4);
                D_00435F80 = 5;
                fldSetCameraNodeModeWithTen();
            }
        }
    }
    return 0;
}

/* Dispatch one pending field command, preferring the temporary override
 * over the scene-work buffer and its saved fallback. */
s32 fldDispatchPendingSceneResource(void) {
    FieldPlayerSceneWork *sceneWork = &fldSceneParam;
    u32 overrideFlags;

    if (fldGlobalBase.pendingEntryDialogDisposition == FLD_ENTRY_DIALOG_REQUEST) {
        func_00150800();
    }
    fldGlobalBase.pendingEntryDialogDisposition = FLD_ENTRY_DIALOG_NONE;
    if ((D_00435F24 & 2) && *(s8 *)D_00387D60 != 0) {
        evtStartSceneResourceTask(dds3GetControlScene(), D_00387D60);
        overrideFlags = D_00435F24;
        if (!(overrideFlags & 1)) {
            D_00387D60[0] = 0;
            D_00435F24 = overrideFlags & 0xFB;
        }
        return 1;
    }
    if (sceneWork->primaryState == 0 && sceneWork->resourceName[0] != 0) {
        evtStartSceneResourceTask(dds3GetControlScene(), sceneWork->resourceName);
        return 1;
    }
    if (fldGlobalBase.fallbackResourceTaskName != 0) {
        evtStartSceneResourceTask(dds3GetControlScene(), fldGlobalBase.fallbackResourceTaskName);
        return 1;
    }
    return 0;
}

extern s8 dds3AdminiGetNextSequence(void);

INCLUDE_RODATA(const s32, "game/code_00124040", D_00412F00);

INCLUDE_RODATA(const s32, "game/code_00124040", D_00412F10);

INCLUDE_RODATA(const s32, "game/code_00124040", D_00412F20);

INCLUDE_RODATA(const s32, "game/code_00124040", D_00412F30);

INCLUDE_RODATA(const s32, "game/code_00124040", D_00412F40);

INCLUDE_RODATA(const s32, "game/code_00124040", D_00412F58);

KwlnTaskUpdateResult fldUpdateFieldSceneTask(dds3ProcessID_t *task) {
    dds3Effect_DistortionBlurParam_t blur;
    FieldPlayerSceneWork *playerWork = &fldSceneParam;
    FldSequenceController *controller = &fldAutoMapSeqFlags;
    s32 mode;
    s32 coordinateFlags;

    if (dds3AdminiGetNextSequence() != -1) return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
    if (fileMenuTaskExists() != 0) return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
    fldPollAreaResourceLoad();
    if (controller->phase == 0) {
        if (fldGetNextFileLoadState() == FLD_NEXT_FILE_LOAD_PENDING) return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
        if (D_00389874[0] == 0 && mnuPollTitleStreamStateLocked() != 0) {
            mnuMarkTitleStreamResetPending();
            mnuResetTitleStreamLocked();
            return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
        }
    }
    mode = fldProcCampChk();
    if (mode < FLD_CAMP_CHECK_STATE_IDLE) {
        if (mode > FLD_CAMP_CHECK_STATE_ENCOUNTER_FLAG_SET) {
            fldUpdateCampMenuTransition();
            if (controller->phase == 4 && D_003898AC[0] != 0 && fldRunDanteGunFlag[0x5C / 4] != 0) {
                fldReportCampVolumeError();
            }
            return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
        }
    }
    if (fldEncountTestFlag(0x40) == 0) fldUpdateCameraHeadingTransition();
    switch (controller->phase) {
    case 0:
        D_00435F78 = 0;
        fldGlobalBase.sceneBgmSuppressed = 1;
        fldInitializeAreaWork();
        D_00435CBC = 0x80000000;
        evtEnableMoonCalc();
        D_004360E8 = 0;
        fldRebuildFieldScene();
        dds3SetActiveScene(dds3GetControlScene());
        fldInitInputPanel();
        if (fldGlobalBase.specialEventActive == 0) evtSetSolarOverlayFullyVisible();
        fldInitAddressProcess();
        if (fldGlobalBase.area >= 200) {
            fldRunDanteGunFlag[0x2C / 4] = 0;
            fldRunDanteGunFlag[0x30 / 4] = 0;
            fldApplySkyLightSet(0, 0);
            fldSkySetTableAnime(fldRunDanteGunFlag[0x30 / 4]);
        }
        if (fldGlobalBase.specialEventActive == 0) fldRunDanteClear();
        coordinateFlags = fldGetLocationCoordinateFlags(fldGlobalBase.area, fldGlobalBase.floor + 1);
        if (coordinateFlags & 0x40) {
            blur.count = 15;
            blur.delaySpread = 15;
            blur.angleStep = 0.1999999881f;
            blur.color = 0x3C989884;
            blur.blendControl = 0x44;
            blur.uvDisplacementAngleDegrees = 0.005f;
            blur.uvDisplacementAmplitude = 0.13f;
            blur.position[0] = 0;
            blur.position[1] = 0;
            blur.positionSpread = 0x100;
            blur.size = 200;
            D_00435F74 = dds3EffectDistortionBlurCreate(&blur);
        } else if (coordinateFlags & 0x80) {
            blur.count = 60;
            blur.delaySpread = 15;
            blur.angleStep = 0.221999988f;
            blur.color = 0x28848484;
            blur.blendControl = 0x44;
            blur.uvDisplacementAngleDegrees = 0.005f;
            blur.uvDisplacementAmplitude = 0.13f;
            blur.position[0] = 0;
            blur.position[1] = 0;
            blur.positionSpread = 0x100;
            blur.size = 136;
            D_00435F74 = dds3EffectDistortionBlurCreate(&blur);
        }
        dds3FadeOut2(0);
        fldPrepareSceneBgmArchive();
        fldLoadSceneModelsAndCamera();
        fldEncountClearFlag(0x40);
        if (fldGlobalBase.area >= 0x1D && fldGlobalBase.area <= 0x1E) {
            fldApplyCurrentAreaActorEntries();
        }
        fldDispatchPendingSceneResource();
        fldUpdateCameraFollow();
        controller->phase++;
        break;
    case 1:
        if (fldStepSceneBgmArchive() != FLD_ARCHIVE_LOAD_PENDING) {
            fldGlobalBase.sceneBgmSuppressed = 0;
            if (fldGlobalBase.specialEventActive == 0) {
                /* Raw sequence mode 6 maps to EVENT, so this legacy status check is unreachable. */
                if (fldGetRestoreMode() == FLD_SCENE_STATUS_BATTLE || fldGetRestoreMode() == FLD_SCENE_STATUS_SISETU || fldGetRestoreMode() == 6) {
                    fldStartSceneBgmAlternate();
                } else if (playerWork->sequenceMode == 0) {
                    fldStartSceneBgm();
                } else {
                    fldPlayFieldBgm();
                }
            }
            if (fldCanUseTargetGuideInCurrentScene(fldGlobalBase.area, fldGlobalBase.floor + 1) != 0) {
                fldResetSecondaryArchiveLoadPhase();
                controller->phase = 8;
            } else {
                controller->phase++;
            }
        }
        break;
    case 8:
        if (fldStepSecondaryArchiveLoad() != 0) controller->phase = 2;
        break;
    case 2:
        controller->phase++;
        break;
    case 3:
        if (fldGetArchiveLoadPending() != 0) return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
        controller->phase++;
        if (fldGlobalBase.fieldTransitionMovementGate == 0 && fldGlobalBase.skipFade == 0 &&
            fldGlobalBase.titleFade == 0 && dds3FadeCheck() == 0) {
            dds3FadeIn2(8);
        }
        if (dds3SearchProcessPriority(0x3EA) == NULL && fldGetSparkDialogPhase() == 0) {
            fldPreparePlayerSceneCameraTarget();
        }
        dds3ObjectBaseOffFlag(fldPlayerObject, 0x40);
        fldEncountSetFlag(0x10);
        fldEncountSetFlag(0x20);
        fldSetSnowActive();
        if (fldGlobalBase.deferredExit == FLD_DEFERRED_EXIT_REQUESTED) fldStartDeferredFieldExit();
        break;
    case 4:
        if (fldGlobalBase.targetGuideEnabled != 0 && fldRunDanteGunFlag[0x5C / 4] != 0) fldUpdateTargetGuideController();
        D_004360E8 = 1;
        if (fldEncountTestFlag(0x40) != 0 && fldEveHit_ChkEveHitAfter() != 0) return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
        if (fldEncountTestFlag(0x40) != 0 && fldGlobalBase.titleFade != 0) {
            fldUpdateRoomTriggers(fldPlayerObject);
            break;
        }
        fldFinishDeferredExit();
        if (fldGlobalBase.specialEventActive != 0) {
            fldUpdateSparkEncounterScene();
            fldMarkStageExplorationCellsAroundPosition(fldGlobalBase.floor, fldGlobalBase.flagStage,
                          fldGlobalBase.x, fldGlobalBase.z, 50.0f);
        } else if (fldRunDanteGunFlag[0] == 0) {
            fldUpdateCampMenuTransition();
            if (fldEncountTestFlag(0x20) == 0) break;
            if (fldRunDanteGunFlag[0] == 0) fldUpdateNextFloorTransition();
        }
        if (fldGetSparkDialogPhase() != 0) fldUpdateDevilizeEventSequence();
        if (fldGlobalBase.pendingSceneRequest == FLD_SCENE_ADVANCE_REQUEST_PENDING) fldAdvanceToNextScene();
        if (fldIsTargetWithinInteractionRange() != 0 && fldGlobalBase.targetGuideActive == 1) {
            fldNoopWithMode(1);
            fldUpdateTravelAndEncounters();
        } else if (fldEncountTestFlag(0x40) != 0) {
            fldNoopWithMode(1);
            fldUpdateTravelAndEncounters();
        }
        if (fldAutoMapSeqTestFlag(2) == 1) {
            controller->phase++;
            break;
        }
        fldUpdateCameraFrame();
        if (fldEncountTestFlag(0x40) != 0) fldUpdateRoomTriggers(fldPlayerObject);
        break;
    case 5:
        dds3VisibleSceneBasic(dds3GetActiveScene(), 0);
        fldSetEncounterMode(fldGlobalBase.encounterMode);
        fldNoopWithMode(0);
        fldPauseBgm();
        p2sdPlaySE(15, 127, 63);
        /* The ordinary and deferred exit cases share this release path. */
    case 7:
        fldFreeNextFile();
        fldLoadNextFile_SetId_BtlRet(fldGlobalBase.area, fldGlobalBase.floor + 1);
        controller->phase = 0;
        fldGlobalBase.transitionCount++;
        if (fldGlobalBase.pendingSceneRequest == FLD_SCENE_ADVANCE_REQUEST_NONE) D_00435F78 = 1;
        else fldGlobalBase.pendingSceneRequest = FLD_SCENE_ADVANCE_REQUEST_NONE;
        dds3AdminiCallSequence(14, &D_00435F98, sizeof(D_00435F98), 1);
        return KWLN_TASK_UPDATE_FINISH_CALLBACK;
    case 6:
        break;
    default:
        break;
    }
    switch (fldGlobalBase.overlayMode) {
    case 2:
        if (fldGlobalBase.overlayCounter > 0) fldGlobalBase.overlayCounter--;
        break;
    case 6:
        if (fldGlobalBase.overlayCounter > 0) fldGlobalBase.overlayCounter--;
        break;
    case 3:
    case 7:
        if (fldGlobalBase.overlayCounter < 15) fldGlobalBase.overlayCounter++;
        break;
    case 0:
    case 1:
    case 4:
    case 5:
    default:
        break;
    }
    if (fldProcCampChk() != FLD_CAMP_CHECK_STATE_MENU_ACTIVE) {
        fldUpdateSceneCommandSpeed();
        fldUpdateSelectedValueTransition();
        fldSkyCalcFilter();
    }
    if (controller->phase >= 4) {
        func_001447A0();
        if (fldRunDanteGunFlag[0] == 0) {
            fldCheckSceneReady();
            fldGetSceneReadyFlag();
        }
        if (fldGetSceneReadyFlag() == FLD_SCENE_NOT_READY && fldProcCampChk() == FLD_CAMP_CHECK_STATE_ENCOUNTER_FLAG_SET &&
            fldHasKiretaLabelProcess() == 0 && fldProcKiretaChk() == 0 &&
            fldProcHirakenaiChk() == 0) {
            fldFlushQueuedEffectPositions();
            fldUpdateObjectSlotActivation();
        }
    }
    if (fldProcCampChk() != FLD_CAMP_CHECK_STATE_MENU_ACTIVE) fldUpdateCameraFollow();
    return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
}

#include "fld_encounter_runtime_api.h"
extern void fldDbgPrint2D(s32, s32, const char *);
extern u8 fldIsEventPhaseAtLeastTwo(void);
extern void func_0014DEA8(void);
extern void fldDrawAutomapWhenReady(void);
extern void fldElvDrawFloorNum(void);
extern void fldTickWeatherEffectNodes(void);
extern s32 D_003899A8[];
extern s32 D_003899B0[];
extern s32 D_00435F8C;
KwlnTaskUpdateResult fldProcDraw(dds3ProcessID_t *task) {
    s32 flags;
    FldSequenceController *lifecycle = &fldAutoMapSeqFlags;

    if (dds3AdminiGetNextSequence() != -1) {
        return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
    }
    if (fldGetNextFileLoadState() == FLD_NEXT_FILE_LOAD_PENDING) {
        if (D_003899A8[0] == 1) {
            fldDbgPrint2D(0x40, 0xC0, "NowLoading...");
        }
        if (!lifecycle->phase) {
            return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
        }
    } else {
        if (!lifecycle->phase) {
            return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
        }
    }
    if (fldGetEncounterRuntimeResult() != 0) {
        return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
    }
    if (fileMenuTaskExists() != 0) {
        return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
    }
    if (fldProcCampChk() == FLD_CAMP_CHECK_STATE_MENU_ACTIVE) {
        return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
    }
    if (fldGetSceneReadyFlag() == FLD_SCENE_NOT_READY && fldProcCampChk() == FLD_CAMP_CHECK_STATE_ENCOUNTER_FLAG_SET &&
        fldIsEventPhaseAtLeastTwo() == 0) {
        func_0014DEA8();
    }
    fldDrawAutomapWhenReady();
    fldElvDrawFloorNum();
    if (fldGlobalBase.specialEventActive != 0) {
        fldTickWeatherEffectNodes();
    }
    if (lifecycle->phase >= 2 && D_003899B0[0] != 0 &&
        fldGetEncounterRuntimeResult() == 0) {
    flags = fldGetLocationCoordinateFlags(fldGlobalBase.area, fldGlobalBase.floor + 1);
    if (flags & 0x40) {
        effBlurStepScatterSlotsAndDraw(D_00435F74);
        return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
    }
    if (flags & 0x80) {
        effBlurStepScatterSlotsAndDraw(D_00435F74);
    }
    }
    return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
}

void fldAutoMapSeqSetFlag(u32 mask) {
    u32 *flags = &fldAutoMapSeqFlags.flags;
    *flags |= mask;
}

void fldAutoMapSeqClearFlag(u32 mask) {
    u32 *flags = &fldAutoMapSeqFlags.flags;
    *flags &= ~mask;
}

u8 fldAutoMapSeqTestFlag(u32 mask) {
    return (fldAutoMapSeqFlags.flags & mask) != 0;
}

void fldEncountSetFlag(u32 mask) {
    fldEncountFlags = fldEncountFlags | mask;
}

void fldEncountClearFlag(u32 mask) {
    fldEncountFlags = fldEncountFlags & ~mask;
}

u8 fldEncountTestFlag(u32 mask) {
    return (fldEncountFlags & mask) != 0;
}

extern KwlnTaskUpdateResult fldProcDraw(dds3ProcessID_t *task);

extern s32 D_00435F84;
extern char D_00412FD0[], D_00412FE0[];
extern KwlnTaskUpdateResult fldProcDraw(dds3ProcessID_t *task);

/* Start the field scene tasks; a zero mode copies the caller's sequence record into the scene work. */
void fldProcTrigSequence(fldSceneParam_t *record, u32 mode) {
    FieldPlayerSceneWork *scene = &fldSceneParam;
    FldSequenceController *controller = &fldAutoMapSeqFlags;

    controller->phase = 0;
    controller->flags = 0;
    scene->primaryState = mode;
    if (mode == 0) {
        scene->sequenceWord3C = record->sequenceWord3C;
        scene->sequenceWord38 = record->sequenceWord38;
        strcpy(scene->entryName, record->name);
        scene->area = record->stage;
        scene->sequenceCode = record->code;
        scene->room = record->kind;
        scene->sequenceMode = record->sequenceMode;
        scene->retainCachedAreaResource = record->retainCachedAreaResource;
        scene->secondaryState = record->mode;
        scene->cameraLinkMode = record->link;
        strcpy(scene->cameraName, record->detail);
        strcpy(scene->resourceName, record->note);
        scene->sequenceOption = record->options;
    }
    dds3StartProcess(D_00412FD0, 0x3F7, 0, 0, fldUpdateFieldSceneTask, 0, 0);
    dds3StartProcess(D_00412FE0, 0x2B0A, 0, 0, fldProcDraw, 0, 0);
    D_00435F84 = 1;
    InitOoyTest();
}

void fldReleaseCampSceneTasks(void) {
    if (D_00435F84 == 0) return;
    D_00435F84 = 0;
    kwlnFadeResetBackground();
    fldEncountClearFlag(0x10);
    fldEncountClearFlag(0x20);
    fldAutoMapSeqSetFlag(1);
    fldAutoMapSeqSetFlag(2);
    dds3KillProcessName(D_00412FD0, 1);
    dds3KillProcessName(D_00412FE0, 1);
    fldFreeInputPanel();
    evtDisableMoonDraw();
    fldFreeAddressProcess();
    if (D_00435F74 != 0) {
        dds3EffectDistortionBlurDelete(D_00435F74);
        D_00435F74 = 0;
    }
    mnuDestroyCampTasks();
    scrDestroyAllNamedProcesses();
    fldReleaseMenuSlotsAfterWait();
}

FldResourceWaitResult fldIsFieldResourceWaitFinished(void) {
    if (D_00389790[0] == 0 && fldIsAreaResourceReady() != FLD_AREA_RESOURCE_NOT_READY) {
        fldPollAreaResourceLoad();
        if (fldGetNextFileLoadState() == FLD_NEXT_FILE_LOAD_PENDING) return FLD_RESOURCE_WAIT_PENDING;
        fldFreeNextFile();
        return FLD_RESOURCE_WAIT_PENDING;
    }
    return sdfDelayedResrcCheck() == 0 ? FLD_RESOURCE_WAIT_FINISHED : FLD_RESOURCE_WAIT_PENDING;
}

extern void fldSelectActorFromSceneIndexTables(s32 mode, s32 index);

void fldApplyTerminalActorSelection(s32 mode, s32 index) {
    fldSelectActorFromSceneIndexTables(mode, index);
}

extern void fldProcNextJump(void);

void fldProc_SisetuReturnPoint(void) {
    fldPauseBgm();
}

void fldProc_CallSisetuReturn(void) {
    if (fldNext_fld != 0) {
        fldProcNextJump();
        return;
    }
    fldSubmitSelectedActorWaypointTransition();
}

void fldSetDeferredFieldCommand(u32 command, u32 parameter) {
    fldNext_fld = command;
    fldDeferredCommandParameter = parameter;
}

void fldProcNextJump(void) {
    if (fldNext_fld == 0) {
        return;
    }
    if (dds3AdminiGetNextSequence() > 0) {
        return;
    }
    if (dds3AdminiGetNextSequence() < 0 &&
        (dds3AdminiGetPrevHistoryFlag() & DDS3_ADMIN_HISTORY_FLAG_RESTORE_PREVIOUS_MODE) != 0) {
        return;
    }
    if (fldStartDeferredActorTransition(fldNext_fld, fldDeferredCommandParameter) == FLD_DEFERRED_COMMAND_CLEAR_PENDING) {
        fldNext_fld = 0;
    }
}

void fldSetPendingSceneAction(s32 sceneAction) {
    D_00435F70 = sceneAction;
}

void fldRunPendingSceneAction(void) {
    s32 sceneAction;

    sceneAction = D_00435F70;
    if (sceneAction != 0) {
        fldDispatchSceneAction(sceneAction);
        D_00435F70 = 0;
    }
}

/* Consume the pending pair; write outputs only when a request is consumed. */
FldNextSceneRequestResult fldConsumeNextSceneRequest(s32 *outCode, s32 *outParameter) {
    if (fldGlobalBase.nextArea == -1 && fldGlobalBase.nextFloor == fldGlobalBase.nextArea) {
        return FLD_NEXT_SCENE_REQUEST_NO_REQUEST;
    }
    *outCode = fldGlobalBase.nextArea + 0xC8;
    *outParameter = fldGlobalBase.nextFloor;
    fldGlobalBase.nextArea = -1;
    fldGlobalBase.nextFloor = -1;
    return FLD_NEXT_SCENE_REQUEST_CONSUMED;
}

/* Validate the packet where its native initialization is reconstructed. */
typedef char FieldSequenceRecord_size_check[
    (sizeof(fldSceneParam_t) == 0xA0) ? 1 : -1];
typedef char FieldSequenceRecord_retainCachedAreaResource_offset_check[
    ((u32)&((fldSceneParam_t *)0)->retainCachedAreaResource == 0x58) ? 1 : -1];

INCLUDE_RODATA(const s32, "game/code_00124040", D_00412FD0);

INCLUDE_RODATA(const s32, "game/code_00124040", D_00412FE0);
