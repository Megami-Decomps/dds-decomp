#include "mdl_motion_api.h"
#include "fld_area_work.h"
#include "fld_area_encounter_table.h"
#include "sdf_packet_list.h"
#include "common.h"
#include "sdf_vu_matrix.h"
#include "fld_scene_status_api.h"
#include "fld_scene_ready_api.h"
#include "fld_encounter_runtime_api.h"
#include "sdf_message.h"
#include "eff_random.h"
#include "snd_sequence_volume_pan.h"
#include "evt_world_object_api.h"
#include "sdf_model_scalars.h"
#include "dds3_path.h"
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
#include "evt_world_source_transform.h"
#include "dds3obj.h"
#include "dds3_world_value.h"
#include "dds3_world_selection.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "kwln_fade.h"
#include "dat_state.h"
#include "kwln_task_lifecycle.h"
#include "mdl_flag_api.h"
#include "dds3_world_counter.h"
#include "fld_resource_wait_api.h"
#include "fld_sequence_controller.h"
#include "file_selection_api.h"

/* Field travel and encounter providers. Integer/FPU argument classes have
 * independent register sequences; the field-controller call establishes
 * step-before-index, and all callers are verified. */
extern void evtSubmitEventRequest(s32, s32);
extern void evtSubmitEventRequestImmediate(s32);
#include "dds3_world_primary.h"
extern void fldUpdateCameraHeadingTransition(void);
extern u8 fldTestSceneLifecycleFlags(u32);

extern f32 D_0038A6F0[4];
extern void evtAdvanceSolarClockAndExpireControls(f32);
extern void evtUpdateFlaggedStats(void);
extern s32 evtClearRandomStatusFlags(void);
extern s32 ptyAnyUnitFlagMatch(u32, s32);
extern void ptyApplyFieldHpAttrition(s32);
extern s32 btlAdvanceConditionalEncounterRoll(f32, s32);

extern s32 fldDeferredCommand;






#include "dds3_object_payload.h"

extern u32 D_00435F60;

extern FldSequenceController fldSceneLifecycleFlags;

extern s16 D_00389874[];

extern u32 D_00435CBC;

extern u32 D_004360E8;

extern s32 D_003898AC[];

extern void fldInitializeAreaWork(void);

extern void fldRebuildFieldScene(void);

extern void fldApplySkyLightSet(u32, s32);

extern void fldResetSparkRewardProfileIndex(void);

extern KwlnTask *kwlnTaskFindByPriority(u32);

extern s32 fldCanUseTargetGuideInCurrentScene(s32, s32);

extern void fldResetSecondaryArchiveLoadPhase(void);

extern s32 fldStepSecondaryArchiveLoad(void);

extern s32 fldReportCampVolumeError(void);

extern void fldApplyCurrentAreaActorEntries(void);

extern s32 fldIsTargetWithinInteractionRange(void);

extern void fldUpdateSparkEncounterScene(void);

extern s32 fldGetSparkDialogPhase(void);

extern void fldUpdateDevilizeEventSequence(void);

extern void fldUpdateTargetGuideController(void);

extern void func_00154F18(s32);

extern s32 fldUpdateTravelAndEncounters(void);

extern void fldCreateInputPanelTask(void);

extern void fldEnsureTask(void);

extern void fldRequestMiniTitleDismiss(void);

extern void fldFinishDeferredExit(void);

extern void func_001447A0(void);

/* Providers used by the field lifecycle controller. */

extern void evtEnableSolarPhaseAdvance(void);

extern s32 fileMenuTaskExists(void);

extern void fldBeginSelectedValueTransition(u32);

extern void fldCheckSceneReady(void);

extern void fldFlushQueuedEffectPositions(void);

extern u32 fldGetArchiveLoadPending(void);

extern void fldLoadSceneModelsAndCamera(void);

extern void fldPrepareSceneBgmArchive(void);

extern s32 fldRestartSceneResourceTask(void);

extern s32 fldSetEncounterMode(s32);

extern void fldStartDeferredFieldExit(void);

extern void fldStartSceneBgm(void);

extern void fldStartSceneBgmAlternate(void);

extern s32 fldStartTitleBgmIfSelected(void);

extern s32 fldStepSceneBgmArchive(void);

extern s32 fldUpdateCameraFollow(void);

extern s32 fldUpdateCameraFrame(void);

extern void fldUpdateSwayOffset(void);

extern void fldUpdateSelectedValueTransition(void);

extern void func_0013DDC0(EffWorldNode *);

extern void fldUpdateObjectSlotActivation(void);

#include "mnu_title_stream.h"





extern u32 fldSceneControlFlags;

extern u32 D_00435F70;

extern FieldPlayerSceneWork D_0038A640;

extern u32 fldPlayerModelResource;

extern u32 D_003898B8[];

extern u32 fldPlayerObject;

extern u32 D_00389858[];

void fldClearSceneControlFlags(u32 arg0);

void fldClearSceneCommandFlag(void);

extern s32 D_0038989C[];

void fldClearFieldTransitionFlag(void);

void fldClearSecondarySceneFlag(void);

void fldClearPrimarySceneFlag(void);

extern char D_00412F30[];

extern struct ScrData *scrFindNamedProcessNode(char *arg0);

extern char D_00412F58[];

extern char D_00412F40[];

extern s32 D_00435F7C;

extern u32 mnuAcknowledgeCampState(void);

u8 fldTestSceneControlFlags(u32 arg0);

extern s32 D_00435F80;


extern u32 fldDeferredCommandParameter;

extern void func_001411F8(u32 arg0);

extern void dds3WorkClear(void);

extern char D_00412D50[]; /* "fldProcSequence" */

extern u8 D_00435F24;

extern u8 D_00387D60[];

extern u32 D_00435F38;

extern u32 D_00435F44;

extern u32 D_00435F40;

extern u32 D_00435F3C;

extern void mdlLoadViewerPackage(s32, s32, s32, s32, s32);

void fldLoadPlayerModel(void);

extern u32 D_00389790[];




extern void fldFreeDisplayObjects(void);

extern s32 sdfCheckPendingWorkWithInterrupts(void);

struct EffWorldNode;


extern f32 fldGetDirectionAngleBetweenXZPositions(f32, f32, f32, f32);

extern u8 D_0037F530[];

extern u8 fldGetCampSceneControlMode(void);

extern u32 D_00389988[];

extern u32 D_00389988[];

extern u32 D_00389988[];


extern s32 D_00435EE0;

extern s32 D_00435EE4;

extern s32 D_00435EE8;

extern s32 D_00435EEC;

extern s32 D_00435EF0;

extern s32 D_00435EF4;

extern s32 D_00435EF8;




extern void sdfResetGameRuntime(s32);

extern s32 fileLoadStateChanged(void);

extern void fileCacheSlotFlagsFromState(void);

extern void fileRestoreSlotFlagsToState(void);

void fldSetDeferredFieldCommand(u32, u32);

extern void dds3AdminSubmitModeRequest(s32, void *, u32, s32);

u32 *fldGetPlayerSceneStateAddress(void);

extern void fldResetPlayerSceneTransformState(void);

extern u32 D_00435F78;

extern void fldSetPendingAreaAndFloor(s32, s32);

void fldUnloadPlayerModel(void);

extern void dds3ClearObjectFlags(u32, s32);


extern void dds3RebuildObjectModelContext(EffWorldNode *, s32, s32);

extern EffWorldNode *dds3SpawnCameraSlotObj5(s32, void *, void *);

extern s32 D_0038A67C[];

extern char D_00412EC0[]; /* "PLAYER_UNIT" */

extern s32 D_00435F30;

extern u8 D_0037FF88[];

extern u8 D_003807F8[];


typedef struct FldEncEntry {
    s16 stage;
    s16 flag;
    s16 chance;
    s16 result;
} FldEncEntry;

extern FldEncEntry *fldEncounterRollTable;



/* Mode 14 copies this eight-byte request. The packed selection contains
 * group:8, entry:8, and scene:16; its consumer reads both words as signed. */
#include "fld_encounter_request.h"
extern FieldEncounterRequest D_00435F98;

extern void fldResetEventSceneState(void);

extern s32 fldGetSparkEncounterSelection(void);

extern void fldResetTaskSlots(void);

extern void fldSetSceneLifecycleFlags(u32);

extern void evtKillFieldScriptTasks(void);

extern void func_00150800(void);

extern s32 D_00435F84;
extern EffBlurScatterWork *D_00435F74;

extern char D_00412FD0[], D_00412FE0[];

extern void kwlnFadeResetBackground(void);

extern void fldDestroyPanelTaskIfPresent(void), evtSetSolarOverlayFullyTransparent(void), fldDestroyTask(void);

extern void mnuDestroyCampTasks(void), scrDestroyAllNamedProcesses(void);

extern void fldReleaseMenuSlotsAfterWait(void);

void fldSetSceneControlFlags(u32 mask);

s32 fldPlayerCameraTargetNoPathUpdate(ObjectTransform *relativeTransform, EffWorldNode *target);

extern s32 dds3SetSlot1HandlerCallback(void *object, Dds3MoverUpdate update);

extern void fldUpdateCameraTarget(void);

extern void fldRefreshAreaStateFromPosition(f32 *position);

extern char D_00435EB8[];

extern char D_00435EC0[];

extern char D_00435EC8[];

extern char D_00435ED0[];

extern char D_00435ED8[];

/* Search location records 1..639; a miss returns the reserved first record, * not NULL, so callers can read the fallback record's fields. */ s16 *fldFindLocationCoordinateRecord(s32 x, s32 y);

/* Read the matching location record's fourth signed halfword; zero on miss. */ s32 fldGetLocationCoordinateValue(s32 x, s32 y);

void fldTestDrawCreate(void);

void fldTestDrawDestroy(void);

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
f32 fldSnapAngleToCompassOctant(f32 angle) {
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

extern f32 fldAngleDifference(f32, f32);

f32 fldSnapAngleToCardinalDirection(f32 angle) {
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

extern f32 sdfAtan2(f32 y, f32 x);

/* Direction in degrees from two X/Z positions, preserving the field's 3.14 PI. */
f32 fldGetDirectionAngleBetweenXZPositions(f32 fromX, f32 fromZ, f32 toX, f32 toZ) {
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

f32 fldGetNormalizedComplementaryAngle(f32 ax, f32 ay, f32 bx, f32 by) {
    s32 angle = (s32)(360.0f - fldGetDirectionAngleBetweenXZPositions(ax, ay, bx, by) + 90.0f);
    return (f32)(angle % 360);
}

INCLUDE_ASM(const s32, "game/code_00124040", fldGetTriangleHeightAtXZ);

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
    u32 status;
    EffWorldNode *worldObject;

    valueChain = dds3GetWorldSecondaryObject();
    valueChain = dds3CopyWorldListToValueChain(valueChain, 6);
    status = dds3GetWorldValueCount((WorldValueIndices *)valueChain);
    if (status == 0) {
        return;
    }
    dds3ResetObjectValueCursor((WorldValueIndices *)valueChain);
    do {
        worldObject = (EffWorldNode *)dds3ReadIndexedWorldObjectWord((WorldValueIndices *)valueChain);
        status = dds3GetObjectPayloadWord8(worldObject);
        if (status == 4) {
            if (clearMode == 0) {
                dds3SetObjectPayloadWord8(worldObject, 3);
            }
            else {
                dds3SetObjectPayloadWord8(worldObject, 0);
            }
        }
        status = dds3AdvanceObjectValueCursor((WorldValueIndices *)valueChain);
    } while (status != 0);
    dds3DestroyWorldIndexNode(valueChain);
}

void fldPrepareDeferredSceneTransition(void) {
    if (datGameState->header.transition != 0) {
        sdfResetGameRuntime(1);
    } else {
        sdfResetGameRuntime(0);
    }
    if (fileLoadStateChanged() == 0) {
        fileCacheSlotFlagsFromState();
    } else {
        fileRestoreSlotFlagsToState();
    }
    mdlFlagSet(0xc0f);
    {
        s32 code;
        D_00389988[0x44 / 4] = 1;
        code = 0x259;
        D_00389988[0x4c / 4] = 0;
        fldSetDeferredFieldCommand(0x15, 0x259);
        dds3AdminSubmitModeRequest(6, &code, 4, 0);
    }
}

extern void fldResetFieldRuntimeState();

extern char D_00435F48[];

void fldStartSequenceRecord(void) {
    FieldSequenceRecord record;
    u32 mode;

    if (fileGetSelectionPendingFlag() == 1) {
        return;
    }
    if (datGameState->header.transition != 0) {
        mode = 1;
        dds3AdminSubmitModeRequest(0x1E, &mode, 4, 0);
        return;
    }
    fldResetFieldRuntimeState(1);
    D_00389988[0x4C / 4] = 0;
    D_00389988[0x44 / 4] = 1;
    mdlFlagSet(0xC0F);
    fldSetDeferredFieldCommand(0, 0);
    fldInitializeSequenceAndResetFlags(&record, 1, 1, D_00435F48);
    record.options = 1;
    dds3AdminSubmitModeRequest(5, &record, 0xA0, 0);
}

extern void fldInitDisplayObjects();

extern void fldResetPendingSounds();

extern void effMiscSeedRandom();

extern void fldParseMixLb();

extern void fldLoadBattleSkyAndFilter();

extern void fldResetSceneResourceTables();

extern void fldLoadFieldTablesAndIndexStages();

extern u8 D_0038A6E0[];

void fldInitializeDisplayAndSceneSound(void) {
    fldInitDisplayObjects();
    fldResetPlayerSceneTransformState();
    fldResetPendingSounds();
    effMiscSeedRandom(D_0038A6E0, 0x1E240);
    fldParseMixLb();
    fldLoadBattleSkyAndFilter();
    fldResetSceneResourceTables();
    fldLoadFieldTablesAndIndexStages();
}

void fldResetPlayerSceneTransformState(void) {
    FieldPlayerSceneWork *sceneWork = &D_0038A640;
    VU0_STORE_VF(vf0, &sceneWork->position);
    VU0_STORE_VF(vf0, &sceneWork->rotation);
    fldPlayerObject = 0;
    *fldGetPlayerSceneStateAddress() = 0;
}

/* Fill a sequence packet for stage/kind/name, then clear the temporary override.
 * Unspecified packet bytes deliberately retain their previous contents. */
void fldInitializeSequenceAndResetFlags(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name) {
    if (kwlnTaskGetTaskByName(D_00412D50) != NULL) {
        if (fldAreaState.area == stage) {
            fldAreaState.reuseAreaResources = 1;
        } else {
            dds3WorkClear();
        }
    }
    record->unk_30 = 0;
    record->unk_3c = 0;
    record->unk_38 = 0;
    strcpy(record->name, name);
    record->stage = stage;
    record->enabled = 1;
    record->mode = 2;
    record->kind = kind;
    record->code = 0;
    record->unk_62 = 0;
    record->link = 0;
    memset(record->detail, 0, sizeof(record->detail));
    memset(record->note, 0, sizeof(record->note));
    record->options = 0;
    D_00435F24 = 0;
    D_00387D60[0] = 0;
}

/* Fill the alternate-mode sequence packet for stage/kind/name.
 * Reuse a running sequence in the same area; otherwise request a work clear. */
void fldInitializeAlternateSequence(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name) {
    if (kwlnTaskGetTaskByName(D_00412D50) != NULL) {
        if (fldAreaState.area == stage) {
            fldAreaState.reuseAreaResources = 1;
        } else {
            dds3WorkClear();
        }
    }
    record->unk_30 = 0;
    record->unk_3c = 0;
    record->unk_38 = 0;
    strcpy(record->name, name);
    record->stage = stage;
    record->enabled = 1;
    record->mode = 3;
    record->kind = kind;
    record->code = 0;
    record->unk_62 = 0;
    record->link = 0;
    memset(record->detail, 0, sizeof(record->detail));
    memset(record->note, 0, sizeof(record->note));
    record->options = 0;
}

/* Fill a field sequence packet, including its narrowed code, link and detail name.
 * A running sequence in the same area requests reuse rather than a work clear. */
void fldInitializeFieldSequenceRecord(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name,
                   s32 code, s32 link, const char *subname) {
    if (kwlnTaskGetTaskByName(D_00412D50) != NULL) {
        if (fldAreaState.area == stage) {
            fldAreaState.reuseAreaResources = 1;
        } else {
            dds3WorkClear();
        }
    }
    record->unk_30 = 0;
    record->unk_3c = 0;
    record->unk_38 = 0;
    strcpy(record->name, name);
    record->stage = stage;
    record->kind = kind;
    record->enabled = 1;
    record->mode = 2;
    record->code = code;
    record->unk_62 = 0;
    record->link = link;
    strcpy(record->detail, subname);
    memset(record->note, 0, sizeof(record->note));
    record->options = 0;
}

/* Fill a sequence packet with a narrowed code and note instead of a detail/link.
 * A running sequence in the same area requests reuse rather than a work clear. */
void fldInitializeSequenceWithNote(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name,
                    s32 code, const char *subname) {
    if (kwlnTaskGetTaskByName(D_00412D50) != NULL) {
        if (fldAreaState.area == stage) {
            fldAreaState.reuseAreaResources = 1;
        } else {
            dds3WorkClear();
        }
    }
    record->unk_30 = 0;
    record->unk_3c = 0;
    record->unk_38 = 0;
    strcpy(record->name, name);
    record->stage = stage;
    record->enabled = 1;
    record->mode = 2;
    record->kind = kind;
    record->code = code;
    record->unk_62 = 0;
    record->link = 0;
    memset(record->detail, 0, sizeof(record->detail));
    strcpy(record->note, subname);
    record->options = 0;
}

void fldInitializeLinkedSequence(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name,
                    s32 code, s32 link, const char *subname) {
    record->unk_30 = 0;
    record->unk_3c = 0;
    record->unk_38 = 0;
    strcpy(record->name, name);
    record->enabled = 1;
    record->stage = stage;
    record->kind = kind;
    record->code = code;
    record->link = link;
    record->mode = 2;
    record->unk_62 = 0;
    strcpy(record->detail, subname);
    memset(record->note, 0, sizeof(record->note));
    record->options = 0;
}

u32 fldGetSceneStatusCode(void) {
    FieldPlayerSceneWork *sceneWork = &D_0038A640;

    if (sceneWork->primaryState == 1) {
        return 1;
    }
    if (sceneWork->secondaryState == 3) {
        return 3;
    }
    if (sceneWork->secondaryState == 4) {
        return 4;
    }
    if (sceneWork->secondaryState == 5) {
        return 5;
    }
    if (sceneWork->secondaryState == 6) {
        return 2;
    }
    return sceneWork->primaryState != 0 ? 2 : 0;
}

extern FieldAreaEncounterTable *D_00435E08;
extern void fldInitializeActorLinkedSequence(s32 mode, s32 index, FieldSequenceRecord *sequence);

extern void *dds3GetTransformSourceData(EffWorldNode *node);
extern void fldStartMiniTitleForUnlock(s32 area);
extern void effMiscAxisAngleToQuaternionVU(f32 angle);
extern void fldApplySceneLightSet(s32, s32);
extern void fldSpawnPlayerUnits(f32 *transform);
extern void fldCreateChaseCameraObject(void);
extern void fldPreparePlayerSceneCameraTarget(void);
extern void fldCreateSecondaryWorldCamera(void);
extern void fldUpdateCameraViewFromPlayer(void);
extern void func_0012F078(void);
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
    EffWorldNode *node;
    FieldAreaEncounterTable *encounters;
    FieldAreaEncounterRow *room;
    s32 reuseAreaResources;
    s32 areaIndex;
    s32 i;
    s32 selection;
    s32 flag;
    s32 encounter;
    s32 cameraLinkMode;
    ObjectTransform *camera;

    memset(fallback, 0, sizeof(fallback));
    fallback[3] = 1.0f;
    fallback[7] = 1.0f;
    {
        f32 axis[4] = { 0.0f, 1.0f, 0.0f, 1.0f };

        sceneWork = &D_0038A640;
        if (sceneWork->sequenceOption == 1) {
            /* This provider initializes the real sequence-packet layout in scene storage. */
            fldInitializeActorLinkedSequence(datGameState->vr.previewX,
                datGameState->vr.previewY, (FieldSequenceRecord *)sceneWork);
        }
        reuseAreaResources = fldAreaState.reuseAreaResources;
        evtCreateWorldObjectForKey(sceneWork->area, sceneWork->room);
        dds3SetWorldObject(NULL);
        if (sceneWork->primaryState == 0) {
            node = dds3FindIndexedObjectChainNodeByName(dds3GetWorldSecondaryObject(),
                0x11, (const u8 *)sceneWork->entryName);
            if (node != NULL) {
                transform = dds3GetTransformSourceData(node);
            } else {
                transform = fallback;
            }
            if (sceneWork->area < 40) {
                fldAreaState.defaultEncounterId = 0;
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
                    flag = room->flagB;
                    if (flag != -1 && flag != 0) {
                        if (mdlFlagTest(flag)) {
                            selection = 2;
                        } else {
                            flag = room->flagA;
                            if (flag != -1 && flag != 0 && mdlFlagTest(flag)) {
                                selection = 1;
                            }
                        }
                    } else {
                        flag = room->flagA;
                        if (flag != -1 && flag != 0) {
                            selection = mdlFlagTest(flag) != 0;
                        }
                    }
                    switch (selection) {
                        case 1:
                            encounter = room->valueA;
                            break;
                        case 2:
                            encounter = room->valueB;
                            break;
                        default:
                            encounter = room->defaultEncounter;
                            break;
                    }
                    fldAreaState.defaultEncounterId = encounter;
                }
            }
            if (fldAreaState.area < 31 && fldGetSceneStatusCode() != 4 && reuseAreaResources == 0) {
                if (fldGetSceneStatusCode() == 5) {
                    if (fldAreaState.cachedArea != fldAreaState.area) {
                        fldStartMiniTitleForUnlock(fldAreaState.area);
                    }
                } else if (fldGetSceneStatusCode() != 2) {
                    if (!((fldAreaState.cachedArea == 32 &&
                           (fldAreaState.area == 36 || fldAreaState.area == 37)) ||
                          (fldAreaState.cachedArea == 36 &&
                           (fldAreaState.area == 32 || fldAreaState.area == 37)) ||
                          (fldAreaState.cachedArea == 37 &&
                           (fldAreaState.area == 32 || fldAreaState.area == 36)))) {
                        fldStartMiniTitleForUnlock(fldAreaState.area);
                    }
                }
            }
        } else if (sceneWork->primaryState == 1) {
            transform = (f32 *)&sceneWork->position;
            VU0_LOAD_VF(vf10, axis);
            effMiscAxisAngleToQuaternionVU(3.14159265f);
            VU0_LOAD_VF(vf11, transform + 4);
            effMiscQuatMultiplyVU();
            VU0_STORE_VF(vf10, transform + 4);
        }
        fldSpawnPlayerUnits(transform);
        if (fldCanUseTargetGuideInCurrentScene(fldAreaState.area, fldAreaState.floor + 1)) {
            fldCreateChaseCameraObject();
            fldAreaState.unk13C = 1;
        }
        dds3SetWorldPlayerObject(dds3GetWorldSecondaryObject(), (EffWorldNode *)fldPlayerObject);
        fldPreparePlayerSceneCameraTarget();
        fldCreateSecondaryWorldCamera();
        if (sceneWork->primaryState == 0) {
            cameraLinkMode = sceneWork->cameraLinkMode;
            if (cameraLinkMode == 1) {
                node = dds3FindIndexedObjectChainNodeByName(dds3GetWorldSecondaryObject(),
                    4, (const u8 *)sceneWork->cameraName);
                if (node != NULL) {
                    camera = node->inner;
                    fldAreaState.mode = 1;
                    fldAreaState.focusPos[0] = camera->position[0];
                    fldAreaState.focusPos[1] = camera->position[1];
                    fldAreaState.focusPos[2] = camera->position[2];
                    fldUpdateCameraViewFromPlayer();
                } else {
                    fldAreaState.mode = 0;
                    func_0012F078();
                }
            } else {
                if (cameraLinkMode == 2) {
                    fldAreaState.mode = 3;
                } else {
                    fldAreaState.mode = 0;
                }
                if (sceneWork->secondaryState == 3) {
                    func_0012F400();
                } else {
                    func_0012F078();
                }
            }
        }
        fldAreaState.verticalStepDirection = 0;
        if (fldGetLocationCoordinateValue(fldAreaState.area, fldAreaState.floor + 1) & 8) {
            fldActivateCameraColorSetting(1);
        } else {
            fldActivateCameraColorSetting(0);
        }
        if (fldGetLocationCoordinateValue(fldAreaState.area, fldAreaState.floor + 1) & 0x800) {
            mnuInitializeResourceEntries();
        }
        if (fldGetLocationCoordinateValue(fldAreaState.area, fldAreaState.floor + 1) & 0x40) {
            fldLoadFieldEffectTextureSlots();
        }
        if (fldGetLocationCoordinateValue(fldAreaState.area, fldAreaState.floor + 1) & 0x200) {
            fldLoadResourceByIndex(0);
        }
        if (fldGetLocationCoordinateValue(fldAreaState.area, fldAreaState.floor + 1) & 0x400) {
            fldLoadResourceByIndex(1);
        }
    }
}


struct SdfMemBlock;


/* Reload only when the field-selected variant changes or its handle is absent. */


void fldLoadPlayerModel(void) {
    s32 model;

    if (fldGetLocationCoordinateValue(fldAreaState.area, fldAreaState.floor + 1) & 0x20) {
        model = 2;
    } else {
        model = fldAreaState.unk118 != 0;
    }
    if (fldAreaState.playerModelVariant != model || fldPlayerModelResource == 0) {
        if (fldPlayerModelResource != 0) {
            fldUnloadPlayerModel();
        }
        switch (model) {
        case 0:
            if (mdlFlagTest(0x31)) {
                fldPlayerModelResource = (u32)sdfReadNamedResource("/model/field/player_13.PB", &D_00435F3C, &D_00435F44);
            } else if (mdlFlagTest(0x25)) {
                fldPlayerModelResource = (u32)sdfReadNamedResource("/model/field/player_d.PB", &D_00435F3C, &D_00435F44);
            } else if (mdlFlagTest(0x1C)) {
                fldPlayerModelResource = (u32)sdfReadNamedResource("/model/field/player_11.PB", &D_00435F3C, &D_00435F44);
            } else if (mdlFlagTest(0x13)) {
                fldPlayerModelResource = (u32)sdfReadNamedResource("/model/field/player_f.PB", &D_00435F3C, &D_00435F44);
            } else if (mdlFlagTest(0x290)) {
                fldPlayerModelResource = (u32)sdfReadNamedResource("/model/field/player_d.PB", &D_00435F3C, &D_00435F44);
            } else {
                fldPlayerModelResource = (u32)sdfReadNamedResource("/model/field/player_a.PB", &D_00435F3C, &D_00435F44);
            }
            break;
        case 1:
            fldPlayerModelResource = (u32)sdfReadNamedResource("/model/field/player_b.PB", &D_00435F3C, &D_00435F44);
            break;
        default:
            if (mdlFlagTest(0x31)) {
                fldPlayerModelResource = (u32)sdfReadNamedResource("/model/field/player_13.PB", &D_00435F3C, &D_00435F44);
            } else if (mdlFlagTest(0x25)) {
                fldPlayerModelResource = (u32)sdfReadNamedResource("/model/field/player_e.PB", &D_00435F3C, &D_00435F44);
            } else if (mdlFlagTest(0x1C)) {
                fldPlayerModelResource = (u32)sdfReadNamedResource("/model/field/player_12.PB", &D_00435F3C, &D_00435F44);
            } else if (mdlFlagTest(0x13)) {
                fldPlayerModelResource = (u32)sdfReadNamedResource("/model/field/player_10.PB", &D_00435F3C, &D_00435F44);
            } else if (mdlFlagTest(0x290)) {
                fldPlayerModelResource = (u32)sdfReadNamedResource("/model/field/player_e.PB", &D_00435F3C, &D_00435F44);
            } else {
                fldPlayerModelResource = (u32)sdfReadNamedResource("/model/field/player_l.PB", &D_00435F3C, &D_00435F44);
            }
            break;
        }
        fldAreaState.playerModelVariant = model;
    }
}

void fldUnloadPlayerModel(void) {
    if (fldPlayerModelResource != 0) {
        sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)fldPlayerModelResource);
        fldPlayerModelResource = 0;
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
extern void effMiscAxisAngleToQuaternionVU(f32);
extern s32 D_00389888[];
extern MdlCtx *fldPlayerModelContext;

/* Toggle the selected player model, preserving and rebuilding its scene pose.
 * Sable/Garnet/Tidal reconstruction; KiB compiler/ABI qualification.
 * The four legacy SDK stores are audited against this exact VU/callee-read
 * pipeline; they are not general-purpose memory barriers. */

extern void ddsReleaseUnitObject(EffWorldNode *);
extern s32 fldSynchronizePlayerPositionAndAreaState(void);

void fldTogglePlayerModel(void) {
    f32 work[4];
    FieldPlayerSceneWork *sceneWork;
    EffWorldNode *player;
    void *source;
    void *buffer;

    memset(work, 0, sizeof(work));
    work[3] = 1.0f;
    {
        FieldVec4 axis = D_00412EB0;

        player = (EffWorldNode *)fldPlayerObject;
        {
            sceneWork = &D_0038A640;
            effObjFetchInnerPosition(player);
            VU0_STORE_VF_UNCLOBBERED(vf10, &sceneWork->position);
            effObjFetchInnerRotationNormalized(player);
            VU0_STORE_VF_UNCLOBBERED(vf10, &sceneWork->rotation);
        }
        ddsReleaseUnitObject((EffWorldNode *)fldPlayerObject);
        fldUnloadPlayerModel();
        /* Rebind the same local for the rebuild phase. Both definitions feed
         * real phase uses; the resulting two-definition pseudo stays global,
         * rather than merging snapshot HIGH/base/+16 in local-alloc. */
        sceneWork = &D_0038A640;
        fldAreaState.unk118 ^= 1;
        VU0_LOAD_VF(vf10, &axis);
        effMiscAxisAngleToQuaternionVU(3.1415926f);
        VU0_LOAD_VF(vf11, &sceneWork->rotation);
        effMiscQuatMultiplyVU();
        VU0_STORE_VF_UNCLOBBERED(vf10, &sceneWork->rotation);
        VU0_LOAD_VF(vf10, &axis);
        effMiscAxisAngleToQuaternionVU(3.1415926f);
        VU0_LOAD_VF(vf11, &sceneWork->rotation);
        effMiscQuatMultiplyVU();
        VU0_STORE_VF_UNCLOBBERED(vf10, work);
    }
    fldPlayerObject = (u32)dds3SpawnCameraSlotObj5(dds3AdvanceWorldCounter(), sceneWork, work);
    dds3SetWorldNodeValue((struct EffWorldNode *)fldPlayerObject, (u32)D_00412EC0);
    dds3SetWorldPlayerObject(dds3GetWorldSecondaryObject(), (EffWorldNode *)fldPlayerObject);
    fldLoadPlayerModel();
    D_00435F40 = D_00435F44;
    D_00435F38 = (u32)sdfAllocGeneralBlock(D_00435F44);
    source = (void *)sdfMemoryGetBlockAddress((struct SdfMemBlock *)(u32)fldPlayerModelResource);
    buffer = (void *)sdfMemoryGetBlockAddress((struct SdfMemBlock *)(u32)D_00435F38);
    memcpy(buffer, source, D_00435F40);
    D_00435F3C = (u32)buffer;
    mdlLoadViewerPackage(2, 0, 0x101, (s32)buffer, D_00435F40);
    sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)D_00435F38);
    D_00435F38 = 0;
    dds3RebuildObjectModelContext((EffWorldNode *)fldPlayerObject, 2, D_0038A640.sequenceWord3C);
    fldPlayerModelContext = (MdlCtx *)dds3GetObjectBaseResourceHandle((EffWorldNode *)fldPlayerObject);
    if (D_00389888[0] != 0) {
        mdlAddEntryFlagged((MdlCtx *)fldPlayerModelContext, 0, 2);
        mdlSetNodeFrameStep((MdlCtx *)fldPlayerModelContext, 1, 1.0f);
        mdlAddEntryFlagged((MdlCtx *)fldPlayerModelContext, 1, 2);
    } else {
        mdlAddEntryFlagged((MdlCtx *)fldPlayerModelContext, 0, 3);
        fldUnloadPlayerModel();
    }
    fldApplySceneLightSet(0, 0);
    fldApplySkyLightSet(D_00389988[0x2C / 4], 0);
    fldBeginSelectedValueTransition(D_00389988[0x30 / 4]);
    fldSynchronizePlayerPositionAndAreaState();
}


void fldPrepareResourceBuffer(void) {
    void *source;
    void *buffer;
    fldLoadPlayerModel();
    D_00435F40 = D_00435F44;
    D_00435F38 = (u32)sdfAllocGeneralBlock(D_00435F44);
    source = (void *)sdfMemoryGetBlockAddress((struct SdfMemBlock *)(u32)fldPlayerModelResource);
    buffer = (void *)sdfMemoryGetBlockAddress((struct SdfMemBlock *)(u32)D_00435F38);
    memcpy(buffer, source, D_00435F40);
    D_00435F3C = (u32)buffer;
    mdlLoadViewerPackage(2, 0, 0x101, (s32)buffer, D_00435F40);
    sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)D_00435F38);
    D_00435F38 = 0;
}

void fldReleaseResources(void) {
    if (D_00435F38 != 0) {
        sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)D_00435F38);
        D_00435F38 = 0;
    }
    if (D_00389790[0] == 0 && D_00435F78 == 0) {
        fldUnloadPlayerModel();
        fldSetPendingAreaAndFloor(0, 0);
    }
}

extern u32 fldSecondarySceneObject;

extern MdlCtx *fldPlayerModelContext;

extern MdlCtx *fldSecondarySceneModelContext;

void fldSnapshotAndReleasePlayerSceneObject(void) {
    FieldPlayerSceneWork *sceneWork;
    if (fldPlayerObject != 0) {
        if (dds3GetWorldSecondaryObject() != 0) {
            sceneWork = &D_0038A640;
            effObjFetchInnerPosition((EffWorldNode *)fldPlayerObject);
            VU0_STORE_VF(vf10, &sceneWork->position);
            effObjFetchInnerRotationNormalized((EffWorldNode *)fldPlayerObject);
            VU0_STORE_VF(vf10, &sceneWork->rotation);
        }
        fldPlayerObject = 0;
        fldSecondarySceneObject = 0;
        fldPlayerModelContext = 0;
        fldSecondarySceneModelContext = 0;
        *fldGetPlayerSceneStateAddress() = 0;
        fldReleaseResources();
    }
}

void fldShutdownSceneTasksAndWorld(void) {
    fldResetTaskSlots();
    fldTestDrawDestroy();
    fldSnapshotAndReleasePlayerSceneObject();
    evtDestroySecondaryWorldNode();
    fldReleaseCampSceneTasks();
    fldResetTargetViewAndSound();
}

u32 * fldGetPlayerSceneStateAddress(void) {
    return &D_00435F60;
}

u32 fldGetPlayerSceneState(void) {
    return *fldGetPlayerSceneStateAddress();
}

/* Raise the scene-state minima, clear pending slots and aim ten units above
 * the fetched player position. Existing larger mode/state values are retained. */
void fldPreparePlayerSceneCameraTarget(void) {
    f32 position[4];

    if (fldPlayerObject != 0) {
        fldSetSceneControlFlags(0x40);
        dds3SetSlot1HandlerCallback((void *)fldPlayerObject, fldPlayerCameraTargetNoPathUpdate);
    }
    fldAreaState.unkE8 = 4;
    if (fldAreaState.sceneMode < 4) {
        fldAreaState.sceneMode = 4;
    }
    if (fldAreaState.sceneState < 5) {
        fldAreaState.sceneState = 5;
    }
    fldAreaState.unk90 = -1;
    fldAreaState.unk94 = -1;
    fldUpdateCameraTarget();
    effObjFetchInnerPosition((EffWorldNode *)fldPlayerObject);
    VU0_STORE_VF(vf10, position);
    position[1] += 10.0f;
    fldRefreshAreaStateFromPosition(position);
}

void fldResetPlayerSceneObjectState(void) {
    if (fldPlayerObject != 0) {
        fldClearSceneControlFlags(0x40);
        dds3SetSlot1HandlerCallback((void *)fldPlayerObject, 0);
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
        fldPlayerObject = (u32)dds3SpawnCameraSlotObj5(dds3AdvanceWorldCounter(), position, rotation);
        dds3SetWorldNodeValue((struct EffWorldNode *)fldPlayerObject, (u32)D_00412EC0);
        dds3SetWorldPlayerObject(dds3GetWorldSecondaryObject(), (EffWorldNode *)fldPlayerObject);
        if (D_00435F30 != 0) {
            dds3ClearObjectFlags(fldPlayerObject, 0x20);
        }
        fldPrepareResourceBuffer();
        dds3RebuildObjectModelContext((EffWorldNode *)fldPlayerObject, 2, D_0038A67C[0]);
    }
}

extern void effObjSetInnerFloat(u32, f32);

extern void effMiscAxisAngleToQuaternionVU(f32);

extern void dds3SetObjectFlags(void *, u32);

extern void fldResetCameraModelHandles(void);




extern void fldActivatePendingSceneCommand(void);

extern s32 fldGetSceneCommandState(void);

/* The scene reset ignores the command word supplied by this caller. */
extern void fldClearCameraObjectActiveFlag();




extern char D_00412F00[];

extern f32 D_0038A610[12];

extern char D_00435F50[];

INCLUDE_RODATA(const s32, "game/code_00124040", D_00412EB0);

INCLUDE_RODATA(const s32, "game/code_00124040", D_00412EC0);

void fldSpawnPlayerUnits(f32 *input) {
    f32 work[4];
    EffWorldNode *player;
    s32 locationFlags;

    memset(work, 0, sizeof(work));
    work[3] = 1.0f;
    {
        f32 axis[4] = {0, 1.0f, 0, 1.0f};
        f32 unitScale[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        f32 secondaryScale[4] = {1.0f, -1.0f, 1.0f, 1.0f};

        VU0_LOAD_VF(vf10, axis);
        effMiscAxisAngleToQuaternionVU(3.14159265f);
        VU0_LOAD_VF(vf11, input + 4);
        effMiscQuatMultiplyVU();
        VU0_STORE_VF(vf10, work);

        player = (EffWorldNode *)fldPlayerObject;
        if (player == NULL) {
            player = dds3SpawnCameraSlotObj5(dds3AdvanceWorldCounter(), input, work);
            fldPlayerObject = (u32)player;
            dds3SetWorldNodeValue(player, (u32)D_00412EC0);
            dds3SetWorldPlayerObject(dds3GetWorldSecondaryObject(), (EffWorldNode *)fldPlayerObject);
            if (D_00435F30 != 0) {
                dds3ClearObjectFlags(fldPlayerObject, 0x20);
            }
            fldPrepareResourceBuffer();
            dds3RebuildObjectModelContext((EffWorldNode *)fldPlayerObject, 2, D_0038A67C[0]);
            player = (EffWorldNode *)fldPlayerObject;
        } else {
            ObjectTransform *inner = player->inner;
            PCP_COPY_VECTOR_F32(inner->position, input);
            PCP_COPY_VECTOR_F32(inner->smoothedPosition, input);
            PCP_COPY_VECTOR_F32(inner->rotation, input + 4);
        }

        effObjSetInnerPosition(player, (u128 *)input);
        effObjSetInnerRotation((EffWorldNode *)fldPlayerObject, (u128 *)work);
        effObjSetInnerScale((EffWorldNode *)fldPlayerObject, (u128 *)unitScale);
        fldPlayerModelContext = (MdlCtx *)dds3GetObjectBaseResourceHandle((EffWorldNode *)(u32)fldPlayerObject);
        effObjSetInnerFloat(fldPlayerObject, 90.0f);

        if (fldAreaState.unk118 == 0) {
            mdlAddEntryFlagged(fldPlayerModelContext, 0, 2);
        } else {
            mdlAddEntryFlagged(fldPlayerModelContext, 0, 2);
            mdlAddEntryFlagged(fldPlayerModelContext, 1, 2);
            fldResetCameraModelHandles();
        }
        sdfSetModelScalarOverrides(fldPlayerModelContext->inner, 15.0f, 0.0f);
        fldSecondarySceneObject = 0;

        locationFlags = fldGetLocationCoordinateValue(fldAreaState.area, fldAreaState.floor + 1);
        if ((locationFlags & 4) != 0) {
            VU0_LOAD_VF(vf10, axis);
            effMiscAxisAngleToQuaternionVU(3.14159265f);
            VU0_LOAD_VF(vf11, input + 4);
            effMiscQuatMultiplyVU();
            VU0_STORE_VF(vf10, work);
            fldSecondarySceneObject = (u32)dds3SpawnCameraSlotObj5(dds3AdvanceWorldCounter(), input, work);
            dds3SetWorldNodeValue((EffWorldNode *)fldSecondarySceneObject, (u32)D_00412F00);
            dds3RebuildObjectModelContext((EffWorldNode *)fldSecondarySceneObject, 2, D_0038A67C[0]);
            effObjSetInnerPosition((EffWorldNode *)fldSecondarySceneObject, (u128 *)input);
            effObjSetInnerRotation((EffWorldNode *)fldSecondarySceneObject, (u128 *)work);
            effObjSetInnerScale((EffWorldNode *)fldSecondarySceneObject, (u128 *)secondaryScale);
            fldSecondarySceneModelContext = (MdlCtx *)dds3GetObjectBaseResourceHandle((EffWorldNode *)(u32)fldSecondarySceneObject);
            effObjSetInnerFloat(fldSecondarySceneObject, 90.0f);
            mdlAddEntryFlagged(fldSecondarySceneModelContext, 0, 0);
            sdfSetModelScalarOverrides(fldSecondarySceneModelContext->inner, 15.0f, 0.0f);
            fldSecondarySceneModelContext->inner->flags |= 8;
        }

        if (fldSecondarySceneObject != 0) {
            dds3SetObjectFlags((void *)fldPlayerObject, 0x400);
        } else {
            dds3SetObjectFlags((void *)fldPlayerObject, 0x200);
        }

        if (fldAreaState.area < 0x14) {
            fldClearSceneCommandFlag();
        }
        locationFlags = fldGetLocationCoordinateValue(fldAreaState.area, fldAreaState.floor + 1);
        if ((locationFlags & 1) != 0) {
            fldAreaState.commandEnabled = 1;
        } else {
            fldAreaState.commandEnabled = 0;
            mdlFlagClear(0x818);
        }

        if (fldAreaState.area < 0xC8) {
            fldApplySceneLightSet(0, 0);
            fldApplySkyLightSet(D_00389988[0x2C / 4], 0);
            fldBeginSelectedValueTransition(D_00389988[0x30 / 4]);
            if (fldGetSceneCommandState() == 1) {
                if (fldAreaState.commandEnabled != 0) {
                    fldActivatePendingSceneCommand();
                } else {
                    mdlFlagClear(0x818);
                    fldAreaState.sceneCommand = 0;
                    fldApplySceneLightSet(0, 0);
                    fldApplySkyLightSet(D_00389988[0x2C / 4], 0);
                    fldBeginSelectedValueTransition(D_00389988[0x30 / 4]);
                }
            } else if (fldAreaState.commandEnabled == 0) {
                mdlFlagClear(0x818);
                fldAreaState.sceneCommand = 0;
            }
            fldClearCameraObjectActiveFlag(fldAreaState.commandEnabled);
        }
        fldSynchronizePlayerPositionAndAreaState();
        evtSpawnActionObj11(dds3AdvanceWorldCounter(), (EvtWorldSourceTransformPrefix *)D_0038A610, (s32)(u32)D_00435F50);
    }
}


extern FieldVec4 D_00412F10;

extern FieldVec4 D_00412F20;

extern char D_00435F58[];

extern u32 dds3CreateConfiguredCameraObject(s32, FieldVec4 *, FieldVec4 *, FieldVec4 *);

extern void dds3SetCameraVector(struct EffWorldNode *camera, u128 *worldEye);

extern void effObjSetInnerFloat(u32, f32);


/* Create the secondary camera at target origin with the stored eye/up vectors. */
void fldCreateSecondaryWorldCamera(void) {
    FieldVec4 localUp = D_00412F10;
    FieldVec4 targetPosition;
    FieldVec4 worldEye;
    u32 *cameraObjectSlot = &D_00435F60;
    u32 cameraObject;
    memset(&targetPosition, 0, sizeof(targetPosition));
    targetPosition.w = 1.0f;
    worldEye = D_00412F20;
    cameraObject = dds3CreateConfiguredCameraObject(dds3AdvanceWorldCounter(), &targetPosition, &worldEye, &localUp);
    *cameraObjectSlot = cameraObject;
    dds3SetWorldNodeValue((struct EffWorldNode *)cameraObject, (u32)D_00435F58);
    dds3SetCameraVector((struct EffWorldNode *)*cameraObjectSlot, (u128 *)&worldEye);
    effObjSetInnerFloat(*cameraObjectSlot, 2.0f);
    dds3SetWorldCameraObject(dds3GetWorldSecondaryObject(), (EffWorldNode *)*cameraObjectSlot);
}

s32 fldPlayerCameraTargetNoPathUpdate(ObjectTransform *relativeTransform, EffWorldNode *target) {
    return 0;
}

u32 fldGetDefaultSceneCommandValue(void) {
    return 100;
}

void fldConsumeSceneCommandFlag(void) {
    if ((datGameState->world.fieldFlags & 8) != 0) {
        fldClearSceneCommandFlag();
        fldAreaState.consumedFlags |= 1;
    }
}


extern void evtClearSolarOverlayControl(void);

void fldClearSceneCommandFlag(void) {
    datGameState->world.fieldFlags &= ~8;
    fldAreaState.consumedFlags &= ~1;
    mdlFlagClear(0x818);
    evtClearSolarOverlayControl();
}

extern void fldPlayMenuSound(s32);


void fldActivatePendingSceneCommand(void) {
    u32 value;
    if (fldAreaState.commandEnabled != 0) {
        if ((datGameState->world.fieldFlags & 8) == 0) {
            fldPlayMenuSound(0x29);
        }
        mdlFlagSet(0x818);
        value = fldGetDefaultSceneCommandValue();
        fldAreaState.sceneCommand = value;
        datGameState->world.fieldFlags |= 8;
        fldAreaState.consumedFlags &= ~1;
        fldApplySceneLightSet((s16)value, 0);
    }
}

s32 fldGetSceneCommandState(void) {
    if ((datGameState->world.fieldFlags & 8) != 0) {
        return 1;
    }
    if (D_0038989C[0] != 0) {
        return 0;
    }
    return -1;
}

/* Latch the scene command while enabled and flagged; stop it when either gate
 * clears. Disable passes transition parameter 0; flag removal passes 20. */
void fldUpdateSceneCommandSpeed(void) {
    FldAreaWork *scene = &fldAreaState;
    u32 value;
    if (scene->commandEnabled == 0) {
        if (scene->sceneCommand != 0) {
            scene->sceneCommand = 0;
            fldApplySceneLightSet(0, 0);
        }
    } else if ((datGameState->world.fieldFlags & 8) != 0) {
        if (scene->sceneCommand == 0) {
            value = fldGetDefaultSceneCommandValue();
            scene->sceneCommand = value;
            fldApplySceneLightSet((s16)value, 0x14);
        }
    } else if (scene->sceneCommand != 0) {
        scene->sceneCommand = 0;
        fldApplySceneLightSet(0, 0x14);
        fldPlayMenuSound(0x2A);
        mdlFlagClear(0x818);
        evtClearSolarOverlayControl();
    }
}

/* Game-state work flags and area-state consumption markers use different bits. */
enum {
    FIELD_TRANSITION_WORK_FLAG = 4,
    FIELD_SECONDARY_SCENE_WORK_FLAG = 2,
    FIELD_PRIMARY_SCENE_WORK_FLAG = 1,
    FIELD_TRANSITION_CONSUMED_FLAG = 2,
    FIELD_SECONDARY_SCENE_CONSUMED_FLAG = 4,
    FIELD_PRIMARY_SCENE_CONSUMED_FLAG = 8
};

/* Clear a set transition flag and mark its consumption in area state. */
void fldConsumeFieldTransitionFlag(void) {
    if ((datGameState->world.fieldFlags & FIELD_TRANSITION_WORK_FLAG) != 0) {
        fldClearFieldTransitionFlag();
        fldAreaState.consumedFlags |= FIELD_TRANSITION_CONSUMED_FLAG;
    }
}

extern void evtResetFieldTransitionCounter(void);

/* Clear the transition work flag and its area-state consumption marker. */
void fldClearFieldTransitionFlag(void) {
    datGameState->world.fieldFlags &= ~FIELD_TRANSITION_WORK_FLAG;
    fldAreaState.consumedFlags &= ~FIELD_TRANSITION_CONSUMED_FLAG;
    evtResetFieldTransitionCounter();
}

/* Set the transition work flag and reset its area-state consumption marker. */
void fldSetFieldTransitionFlag(void) {
    datGameState->world.fieldFlags |= FIELD_TRANSITION_WORK_FLAG;
    fldAreaState.consumedFlags &= ~FIELD_TRANSITION_CONSUMED_FLAG;
}

/* Return whether the transition work flag is set. */
u8 fldTestFieldTransitionFlag(void) {
    s32 fieldFlags = datGameState->world.fieldFlags;
    fieldFlags &= FIELD_TRANSITION_WORK_FLAG;
    return fieldFlags != 0;
}

/* Clear a set secondary-scene flag and mark its consumption in area state. */
void fldConsumeSecondarySceneFlag(void) {
    if ((datGameState->world.fieldFlags & FIELD_SECONDARY_SCENE_WORK_FLAG) != 0) {
        fldClearSecondarySceneFlag();
        fldAreaState.consumedFlags |= FIELD_SECONDARY_SCENE_CONSUMED_FLAG;
    }
}

/* Clear the secondary-scene work flag and its consumption marker. */
void fldClearSecondarySceneFlag(void) {
    datGameState->world.fieldFlags &= ~FIELD_SECONDARY_SCENE_WORK_FLAG;
    fldAreaState.consumedFlags &= ~FIELD_SECONDARY_SCENE_CONSUMED_FLAG;
}

/* Select secondary rather than primary, resetting secondary's consume marker. */
void fldSetSecondarySceneFlag(void) {
    datGameState->world.fieldFlags = (datGameState->world.fieldFlags | FIELD_SECONDARY_SCENE_WORK_FLAG) & ~FIELD_PRIMARY_SCENE_WORK_FLAG;
    fldAreaState.consumedFlags &= ~FIELD_SECONDARY_SCENE_CONSUMED_FLAG;
}

/* Return whether the secondary-scene work flag is set. */
u8 fldTestSecondarySceneFlag(void) {
    s32 fieldFlags = datGameState->world.fieldFlags;
    fieldFlags &= FIELD_SECONDARY_SCENE_WORK_FLAG;
    return fieldFlags != 0;
}

/* Clear a set primary-scene flag and mark its consumption in area state. */
void fldConsumePrimarySceneFlag(void) {
    if ((datGameState->world.fieldFlags & FIELD_PRIMARY_SCENE_WORK_FLAG) != 0) {
        fldClearPrimarySceneFlag();
        fldAreaState.consumedFlags |= FIELD_PRIMARY_SCENE_CONSUMED_FLAG;
    }
}

/* Clear the primary-scene work flag and its area-state consumption marker. */
void fldClearPrimarySceneFlag(void) {
    datGameState->world.fieldFlags &= ~FIELD_PRIMARY_SCENE_WORK_FLAG;
    fldAreaState.consumedFlags &= ~FIELD_PRIMARY_SCENE_CONSUMED_FLAG;
}

/* Select primary rather than secondary, resetting primary's consume marker. */
void fldSetPrimarySceneFlag(void) {
    datGameState->world.fieldFlags = (datGameState->world.fieldFlags | FIELD_PRIMARY_SCENE_WORK_FLAG) & ~FIELD_SECONDARY_SCENE_WORK_FLAG;
    fldAreaState.consumedFlags &= ~FIELD_PRIMARY_SCENE_CONSUMED_FLAG;
}

/* Return whether the primary-scene work flag is set. */
u8 fldIsFlagActive(void) {
    s32 fieldFlags = datGameState->world.fieldFlags;
    fieldFlags &= FIELD_PRIMARY_SCENE_WORK_FLAG;
    if (fieldFlags == 0) return 0;
    return 1;
}

void fldConfigureRenderCleanupMessage(void) {
    sdfStoreMessageWordsAndNotifyConsumer((struct SdfMsg *)D_003807F8, (s32)D_0037FF88, (u32 *)(D_0037FF88 + 0xC0), (s32)(D_0037FF88 + 0x100), (s32)(D_0037FF88 + 0xE0));
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
        if (fldEncounterRollTable[encounterIndex].stage == fldAreaState.area) {
            eligible = 1;
            if (fldEncounterRollTable[encounterIndex].flag != -1) {
                eligible = mdlFlagTest(fldEncounterRollTable[encounterIndex].flag) != 0;
            }
            if (eligible != 0) {
                if ((u32)effMiscRand(0) % FIELD_ENCOUNTER_PERCENT_RANGE < (u32)fldEncounterRollTable[encounterIndex].chance) {
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
    EffWorldNode *player;
    f32 distance;
    f32 interval;
    s32 previousPhase;
    s32 phase;
    s32 encounter;
    s32 *selection;
    s32 floor;

    if (fldAreaState.sceneMode > 0) fldAreaState.sceneMode--;
    if (fldAreaState.sceneState > 0) fldAreaState.sceneState--;
    if (fldAreaState.unkE8 > 0) fldAreaState.unkE8--;
    if (!fldIsTargetWithinInteractionRange() && !fldAreaState.targetGuideActive &&
        !fldTestSceneControlFlags(0x40)) return 0;
    if (fldTestSceneLifecycleFlags(1) == 1) return 0;
    if (!dds3GetWorldObject()) return 0;
    player = (EffWorldNode *)fldPlayerObject;
    if (!player) return 0;

    effObjFetchInnerPosition(player);
    VU0_LOAD_VF(vf11, previousPosition);
    VU0_STORE_VF(vf10, previousPosition);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(distance);
    if (distance < 0.2f) distance = 0.0f;
    fldAreaState.frameTravelDistance = distance;
    previousPhase = evtGetSolarPhase();
    if (fldAreaState.unkE8 > 0) {
        if (fldAreaState.unk11C && D_00389988[0x60 / 4]) {
            fldAreaState.attritionDistance += distance;
        }
        effObjFetchInnerPosition(player);
        VU0_STORE_VF(vf10, previousPosition);
        fldAreaState.frameTravelDistance = 0.0f;
        return 0;
    }
    if (fldAreaState.transitionMode != 0) return 0;
    evtAdvanceSolarClockAndExpireControls(distance);
    phase = evtGetSolarPhase();
    if (fldAreaState.consumedFlags & 15) {
        fldResetPlayerSceneObjectState();
        evtStartSceneResourceTask(dds3GetWorldObject(), D_00412F30);
        fldUpdateCameraHeadingTransition();
        return 0;
    }
    if (!(fldGetLocationCoordinateValue(fldAreaState.area, fldAreaState.floor + 1) & 0x40) &&
        previousPhase != phase) {
        evtUpdateFlaggedStats();
        if (phase == 0 && evtClearRandomStatusFlags()) {
            fldAreaState.sceneMode = 4;
            fldAreaState.sceneState = 5;
            fldResetPlayerSceneObjectState();
            evtStartSceneResourceTask(dds3GetWorldObject(), D_00412F40);
            return 0;
        }
    }
    if (fldAreaState.unk11C || ptyAnyUnitFlagMatch(0x80, 1)) {
        if (D_00389988[0x60 / 4]) fldAreaState.attritionDistance += distance;
        interval = 450.0f;
        if (!fldAreaState.unk11C && ptyAnyUnitFlagMatch(0x80, 1)) interval = 1000.0f;
        if (fldAreaState.attritionDistance > interval) {
            if (fldAreaState.unk11C) {
                if (fldAreaState.unk11C != 3 && (datGameState->world.fieldFlags & 4)) {
                    fldAreaState.unk190 = 0;
                    fldAreaState.attritionDistance = 0.0f;
                } else {
                    fldAreaState.unk190 = 60;
                    fldAreaState.attritionDistance = 0.0f;
                    if (fldAreaState.unk11C == 3) ptyApplyFieldHpAttrition(3);
                    else if (fldAreaState.unk11C == 2) ptyApplyFieldHpAttrition(4);
                    else ptyApplyFieldHpAttrition(2);
                }
            } else {
                fldAreaState.attritionDistance = 0.0f;
                fldAreaState.unk190 = 60;
                ptyApplyFieldHpAttrition(1);
            }
        }
    } else {
        fldAreaState.attritionDistance = 450.0f;
    }
    if (fldGetResourceReadyFlag() == 1) return 0;
    if (fldAreaState.unk13C && D_00389988[0x5C / 4] &&
        D_00389988[0x44 / 4] && fldIsTargetWithinInteractionRange()) {
        fldResetTaskSlots();
        fldSetSceneLifecycleFlags(1);
        fldSetSceneLifecycleFlags(2);
        fldResetPlayerSceneObjectState();
        evtKillFieldScriptTasks();
        if (mdlFlagTest(0x4D0)) {
            D_00435F98.tableIndex = 0;
            D_00435F98.encounterSelection = 0x32A;
            floor = fldAreaState.floor + 1;
            switch (floor) {
            case 11:
                mdlFlagSet(0x4C3);
                mdlFlagSet(0x80B);
                fldSetDeferredFieldCommand(23, 201);
                evtSubmitEventRequest(0x31E, 0);
                break;
            case 14:
                mdlFlagSet(0x4C2);
                evtSubmitEventRequestImmediate(0x32B);
                break;
            default:
                mdlFlagSet(0x4C1);
                evtSubmitEventRequestImmediate(0x32A);
                break;
            }
        } else {
            D_00435F98.tableIndex = 0;
            D_00435F98.encounterSelection = 0x302;
            fldSetDeferredFieldCommand(23, 200);
            floor = fldAreaState.floor + 1;
            if (floor == 11 || floor == 14) evtSubmitEventRequest(0x302, 0);
            else evtSubmitEventRequest(0x302, 0);
        }
        return -1;
    }
    if (!fldAreaState.transitionMode && !D_00389988[0x44 / 4]) return 0;
    if (!fldTestSceneControlFlags(0x10)) return 0;
    fldAreaState.encounterMode = 0;
    encounter = fldAreaState.unkE0;
    if (encounter == 0) encounter = fldAreaState.defaultEncounterId;
    D_00435F98.tableIndex = encounter;
    selection = &D_00435F98.encounterSelection;
    if ((*selection = btlAdvanceConditionalEncounterRoll(distance, encounter)) != 0) {
        encounter = fldRollEncounter();
        if (encounter != -1) {
            D_00435F98.tableIndex = encounter;
            *selection = btlAdvanceConditionalEncounterRoll(99999.0f, encounter);
            fldAreaState.encounterMode = 1;
        }
        if (*selection != 0) {
            if (D_00389988[0x44 / 4] == 1) {
                fldAreaState.nextFloor = fldAreaState.floor + 1;
                fldAreaState.nextArea = fldAreaState.area;
                fldResetTaskSlots();
                fldSetSceneLifecycleFlags(1);
                fldSetSceneLifecycleFlags(2);
                fldResetPlayerSceneObjectState();
                evtKillFieldScriptTasks();
                return -1;
            }
            dds3WorkClear();
            fldAreaState.transitionCount++;
            sndSetSequenceVolumePan(15, 127, 63);
            kwlnFadeOutStart(128, 128, 128, 6);
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
    scene = fldAreaState.floor;
    area = fldAreaState.area;
    fldAreaState.nextArea = area;
    fldAreaState.nextFloor = scene + 1;
    fldResetTaskSlots();
    fldSetSceneLifecycleFlags(1);
    fldSetSceneLifecycleFlags(2);
    fldResetPlayerSceneObjectState();
    evtKillFieldScriptTasks();
    return -1;
}

u8 fldHasKiretaLabelProcess(void) {
    return scrFindNamedProcessNode(D_00412F30) != 0;
}

u8 fldHasHirakenaiLabelProcess(void) {
    return scrFindNamedProcessNode(D_00412F58) != 0;
}

u8 fldHasBadkaifukuLabelProcess(void) {
    return scrFindNamedProcessNode(D_00412F40) != 0;
}

u8 fldGetCampSceneControlMode(void) {
    if (D_00435F7C > 0) {
        return 2;
    }
    if (mnuAcknowledgeCampState() != 0) {
        return 1;
    }
    return fldTestSceneControlFlags(0x20) != 0 ? 0 : 3;
}





extern void func_00149A00(void);

extern void fldUpdateCameraHeadingTransition(void);

extern u8 fldHasPendingSceneFlags(void);

extern void fldSetCameraNodeModeWithTen(void);

extern void fldMarkStageExplorationCellsAroundPosition(s32, s32, f32, f32, f32);


extern void mnuCreateCampTasks(void);

extern void evtSetSolarOverlayFullyVisible(void);

extern void fldApplySkyLightSetToPlayerVU(void);
extern void fldClearSceneLifecycleFlags(u32 mask);

extern u8 fldTestSceneLifecycleFlags(u32 mask);

extern s16 D_00389898[];

/* Complete a delayed camp handoff or advance the player scene transition. */
s32 fldUpdateCampMenuTransition(void) {
    FldAreaWork *scene;
    s32 control;

    if (D_00435F7C > 0) {
        D_00435F7C--;
        if (D_00435F7C == 0) {
            dds3SetWorldObjectDrawEnabled(dds3GetWorldObject(), 0);
            kwlnFadeStartIn(4);
            mnuCreateCampTasks();
        }
        return 0;
    }
    if (fldTestSceneControlFlags(0x20) == 0 && fldGetCampSceneControlMode() == 3) {
        fldClearSceneLifecycleFlags(1);
        fldPreparePlayerSceneCameraTarget();
        evtSetSolarOverlayFullyVisible();
        /* Retail retains both branches of this shared scene-state gate. */
        if (D_00389898[0] != 0) {
            fldApplySkyLightSetToPlayerVU();
        } else {
            fldApplySkyLightSetToPlayerVU();
        }
        dds3SetWorldObjectDrawEnabled(dds3GetWorldObject(), 1);
        fldSetSceneControlFlags(0x20);
        kwlnFadeStartOut(0);
        kwlnFadeStartIn(8);
        return 0;
    }
    control = fldTestSceneControlFlags(0x20);
    if (control == 0) {
        return control;
    }
    control = fldTestSceneControlFlags(0x40);
    if (control == 0) {
        return control;
    }
    if (fldTestSceneLifecycleFlags(1) == 1 || fldHasPendingSceneFlags() != 0 || fldGetSceneReadyFlag() != 0) {
        return 0;
    }
    scene = &fldAreaState;
    if (scene->sceneMode == 0 && (s8)D_0037F530[0] < 0) {
        sndSetSequenceVolumePan(0xE, 0x7F, 0x3F);
        fldSetSceneLifecycleFlags(1);
        fldResetPlayerSceneObjectState();
        fldUpdateCameraHeadingTransition();
        evtSetSolarOverlayFullyTransparent();
        scene->sceneMode = 4;
        scene->sceneState = 5;
        fldClearSceneControlFlags(0x20);
        kwlnFadeInStart(0, 0, 0, 4);
        D_00435F7C = 5;
        fldSetCameraNodeModeWithTen();
    }
    return 0;
}

u8 fldGetSceneReadyOrPendingState(void) {
    if (D_00435F80 > 0) {
        return 2;
    }
    return fldGetSceneReadyFlag() != 0;
}

/* Gate next-floor input on camp/readiness flags, then start the native scene
 * transition or its delayed fade path. All return paths retain zero. */
s32 fldUpdateNextFloorTransition(void) {
    s32 transitionInput = 0;
    FldAreaWork *scene;

    if (fldGetCampSceneControlMode() != 0) {
        return 0;
    }
    if (D_00389988[0x58 / 4] & fldTestSceneControlFlags(0x40)) {
        if ((s8)D_0037F530[2] != 0) {
            transitionInput = 1;
        }
    } else if ((s8)D_0037F530[2] < 0) {
        transitionInput = 1;
    }
    scene = &fldAreaState;
    if (fldFindLocationCoordinateRecord(scene->area, scene->floor + 1)[2] <= 0) {
        if (scene->sceneMode == 0) {
            if (fldTestSceneControlFlags(0x40) != 0 && transitionInput != 0) {
                scene->sceneMode = 4;
                scene->sceneState = 5;
                fldResetPlayerSceneObjectState();
                evtStartSceneResourceTask(dds3GetWorldObject(), D_00412F58);
            }
        }
    } else {
        if (D_00435F80 > 0) {
            D_00435F80--;
            if (D_00435F80 == 0) {
                dds3SetWorldObjectDrawEnabled(dds3GetWorldObject(), 0);
                kwlnFadeStartIn(4);
                func_00149A00();
            }
            return 0;
        }
        if (fldGetSceneReadyOrPendingState() != 0) {
            return 0;
        }
        fldMarkStageExplorationCellsAroundPosition(scene->floor, scene->unkC0, scene->x, scene->z, 50.0f);
        if (scene->sceneMode == 0) {
            if (fldHasPendingSceneFlags() != 0) {
                return 0;
            }
            if (fldTestSceneControlFlags(0x40) != 0 && transitionInput != 0 && D_00389988[0x48 / 4] == 0) {
                scene->sceneMode = 4;
                scene->sceneState = 5;
                fldResetPlayerSceneObjectState();
                fldUpdateCameraHeadingTransition();
                kwlnFadeInStart(0, 0, 0, 4);
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
    FieldPlayerSceneWork *sceneWork = &D_0038A640;
    u32 overrideFlags;

    if (fldAreaState.unk100 == 1) {
        func_00150800();
    }
    fldAreaState.unk100 = 0;
    if ((D_00435F24 & 2) && *(s8 *)D_00387D60 != 0) {
        evtStartSceneResourceTask(dds3GetWorldSecondaryObject(), D_00387D60);
        overrideFlags = D_00435F24;
        if (!(overrideFlags & 1)) {
            D_00387D60[0] = 0;
            D_00435F24 = overrideFlags & 0xFB;
        }
        return 1;
    }
    if (sceneWork->primaryState == 0 && sceneWork->resourceName[0] != 0) {
        evtStartSceneResourceTask(dds3GetWorldSecondaryObject(), sceneWork->resourceName);
        return 1;
    }
    if (fldAreaState.fallbackResourceName != 0) {
        evtStartSceneResourceTask(dds3GetWorldSecondaryObject(), fldAreaState.fallbackResourceName);
        return 1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_00124040", D_00412F00);

INCLUDE_RODATA(const s32, "game/code_00124040", D_00412F10);

INCLUDE_RODATA(const s32, "game/code_00124040", D_00412F20);

INCLUDE_RODATA(const s32, "game/code_00124040", D_00412F30);

INCLUDE_RODATA(const s32, "game/code_00124040", D_00412F40);

INCLUDE_RODATA(const s32, "game/code_00124040", D_00412F58);

s32 fldUpdateFieldSceneTask(KwlnTask *task) {
    EffBlurScatterParams blur;
    FieldPlayerSceneWork *playerWork = &D_0038A640;
    FldSequenceController *controller = &fldSceneLifecycleFlags;
    s32 mode;
    s32 coordinateFlags;

    if (dds3AdminGetRequestedMode() != -1) return 0;
    if (fileMenuTaskExists() != 0) return 0;
    fldPollAreaResourceLoad();
    if (controller->stage == 0) {
        if (fldGetResourceReadyFlag() == 1) return 0;
        if (D_00389874[0] == 0 && mnuPollTitleStreamStateLocked() != 0) {
            mnuMarkTitleStreamResetPending();
            mnuResetTitleStreamLocked();
            return 0;
        }
    }
    mode = fldGetCampSceneControlMode();
    if (mode < 3) {
        if (mode > 0) {
            fldUpdateCampMenuTransition();
            if (controller->stage == 4 && D_003898AC[0] != 0 && D_00389988[0x5C / 4] != 0) {
                fldReportCampVolumeError();
            }
            return 0;
        }
    }
    if (fldTestSceneControlFlags(0x40) == 0) fldUpdateCameraHeadingTransition();
    switch (controller->stage) {
    case 0:
        D_00435F78 = 0;
        fldAreaState.unk130 = 1;
        fldInitializeAreaWork();
        D_00435CBC = 0x80000000;
        evtEnableSolarPhaseAdvance();
        D_004360E8 = 0;
        fldRebuildFieldScene();
        dds3SetWorldObject(dds3GetWorldSecondaryObject());
        fldCreateInputPanelTask();
        if (fldAreaState.transitionMode == 0) evtSetSolarOverlayFullyVisible();
        fldEnsureTask();
        if (fldAreaState.area >= 200) {
            D_00389988[0x2C / 4] = 0;
            D_00389988[0x30 / 4] = 0;
            fldApplySkyLightSet(0, 0);
            fldBeginSelectedValueTransition(D_00389988[0x30 / 4]);
        }
        if (fldAreaState.transitionMode == 0) fldResetSparkRewardProfileIndex();
        coordinateFlags = fldGetLocationCoordinateValue(fldAreaState.area, fldAreaState.floor + 1);
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
            D_00435F74 = effBlurCreateScatterWork(&blur);
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
            D_00435F74 = effBlurCreateScatterWork(&blur);
        }
        kwlnFadeStartOut(0);
        fldPrepareSceneBgmArchive();
        fldLoadSceneModelsAndCamera();
        fldClearSceneControlFlags(0x40);
        if (fldAreaState.area >= 0x1D && fldAreaState.area <= 0x1E) {
            fldApplyCurrentAreaActorEntries();
        }
        fldDispatchPendingSceneResource();
        fldUpdateCameraFollow();
        controller->stage++;
        break;
    case 1:
        if (fldStepSceneBgmArchive() != 0) {
            fldAreaState.unk130 = 0;
            if (fldAreaState.transitionMode == 0) {
                if (fldGetSceneStatusCode() == 1 || fldGetSceneStatusCode() == 4 || fldGetSceneStatusCode() == 6) {
                    fldStartSceneBgmAlternate();
                } else if (playerWork->sequenceMode == 0) {
                    fldStartSceneBgm();
                } else {
                    fldStartTitleBgmIfSelected();
                }
            }
            if (fldCanUseTargetGuideInCurrentScene(fldAreaState.area, fldAreaState.floor + 1) != 0) {
                fldResetSecondaryArchiveLoadPhase();
                controller->stage = 8;
            } else {
                controller->stage++;
            }
        }
        break;
    case 8:
        if (fldStepSecondaryArchiveLoad() != 0) controller->stage = 2;
        break;
    case 2:
        controller->stage++;
        break;
    case 3:
        if (fldGetArchiveLoadPending() != 0) return 0;
        controller->stage++;
        if (fldAreaState.unk188 == 0 && fldAreaState.skipFade == 0 &&
            fldAreaState.titleFade == 0 && kwlnFadeIsActive() == 0) {
            kwlnFadeStartIn(8);
        }
        if (kwlnTaskFindByPriority(0x3EA) == NULL && fldGetSparkDialogPhase() == 0) {
            fldPreparePlayerSceneCameraTarget();
        }
        dds3ClearObjectFlags(fldPlayerObject, 0x40);
        fldSetSceneControlFlags(0x10);
        fldSetSceneControlFlags(0x20);
        fldRequestMiniTitleDismiss();
        if (fldAreaState.deferredExit == 1) fldStartDeferredFieldExit();
        break;
    case 4:
        if (fldAreaState.unk13C != 0 && D_00389988[0x5C / 4] != 0) fldUpdateTargetGuideController();
        D_004360E8 = 1;
        if (fldTestSceneControlFlags(0x40) != 0 && fldRestartSceneResourceTask() != 0) return 0;
        if (fldTestSceneControlFlags(0x40) != 0 && fldAreaState.titleFade != 0) {
            func_0013DDC0((EffWorldNode *)fldPlayerObject);
            break;
        }
        fldFinishDeferredExit();
        if (fldAreaState.transitionMode != 0) {
            fldUpdateSparkEncounterScene();
            fldMarkStageExplorationCellsAroundPosition(fldAreaState.floor, fldAreaState.unkC0,
                          fldAreaState.x, fldAreaState.z, 50.0f);
        } else if (D_00389988[0] == 0) {
            fldUpdateCampMenuTransition();
            if (fldTestSceneControlFlags(0x20) == 0) break;
            if (D_00389988[0] == 0) fldUpdateNextFloorTransition();
        }
        if (fldGetSparkDialogPhase() != 0) fldUpdateDevilizeEventSequence();
        if (fldAreaState.pendingSceneRequest == 1) fldAdvanceToNextScene();
        if (fldIsTargetWithinInteractionRange() != 0 && fldAreaState.targetGuideActive == 1) {
            func_00154F18(1);
            fldUpdateTravelAndEncounters();
        } else if (fldTestSceneControlFlags(0x40) != 0) {
            func_00154F18(1);
            fldUpdateTravelAndEncounters();
        }
        if (fldTestSceneLifecycleFlags(2) == 1) {
            controller->stage++;
            break;
        }
        fldUpdateCameraFrame();
        if (fldTestSceneControlFlags(0x40) != 0) func_0013DDC0((EffWorldNode *)fldPlayerObject);
        break;
    case 5:
        dds3SetWorldObjectDrawEnabled(dds3GetWorldObject(), 0);
        fldSetEncounterMode(fldAreaState.encounterMode);
        func_00154F18(0);
        fldStopCurrentBgm();
        sndSetSequenceVolumePan(15, 127, 63);
        /* The ordinary and deferred exit cases share this release path. */
    case 7:
        fldFreeDisplayObjects();
        fldSetPendingAreaAndFloor(fldAreaState.area, fldAreaState.floor + 1);
        controller->stage = 0;
        fldAreaState.transitionCount++;
        if (fldAreaState.pendingSceneRequest == 0) D_00435F78 = 1;
        else fldAreaState.pendingSceneRequest = 0;
        dds3AdminSubmitModeRequest(14, &D_00435F98, sizeof(D_00435F98), 1);
        return -1;
    case 6:
        break;
    default:
        break;
    }
    switch (fldAreaState.overlayMode) {
    case 2:
        if (fldAreaState.overlayCounter > 0) fldAreaState.overlayCounter--;
        break;
    case 6:
        if (fldAreaState.overlayCounter > 0) fldAreaState.overlayCounter--;
        break;
    case 3:
    case 7:
        if (fldAreaState.overlayCounter < 15) fldAreaState.overlayCounter++;
        break;
    case 0:
    case 1:
    case 4:
    case 5:
    default:
        break;
    }
    if (fldGetCampSceneControlMode() != 1) {
        fldUpdateSceneCommandSpeed();
        fldUpdateSelectedValueTransition();
        fldUpdateSwayOffset();
    }
    if (controller->stage >= 4) {
        func_001447A0();
        if (D_00389988[0] == 0) {
            fldCheckSceneReady();
            fldGetSceneReadyFlag();
        }
        if (fldGetSceneReadyFlag() == 0 && fldGetCampSceneControlMode() == 0 &&
            fldHasKiretaLabelProcess() == 0 && fldHasHirakenaiLabelProcess() == 0 &&
            fldHasBadkaifukuLabelProcess() == 0) {
            fldFlushQueuedEffectPositions();
            fldUpdateObjectSlotActivation();
        }
    }
    if (fldGetCampSceneControlMode() != 1) fldUpdateCameraFollow();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00124040", fldProcDraw);

void fldSetSceneLifecycleFlags(u32 mask) {
    u32 *flags = &fldSceneLifecycleFlags.flags;
    *flags |= mask;
}

void fldClearSceneLifecycleFlags(u32 mask) {
    u32 *flags = &fldSceneLifecycleFlags.flags;
    *flags &= ~mask;
}

u8 fldTestSceneLifecycleFlags(u32 mask) {
    return (fldSceneLifecycleFlags.flags & mask) != 0;
}

void fldSetSceneControlFlags(u32 mask) {
    fldSceneControlFlags = fldSceneControlFlags | mask;
}

void fldClearSceneControlFlags(u32 mask) {
    fldSceneControlFlags = fldSceneControlFlags & ~mask;
}

u8 fldTestSceneControlFlags(u32 mask) {
    return (fldSceneControlFlags & mask) != 0;
}

extern s32 fldProcDraw(KwlnTask *task);

extern s32 D_00435F84;
extern char D_00412FD0[], D_00412FE0[];
extern s32 fldProcDraw(KwlnTask *task);

/* Start the field scene tasks; a zero mode copies the caller's sequence record into the scene work. */
void fldStartFieldSceneTasks(FieldSequenceRecord *record, u32 mode) {
    FieldPlayerSceneWork *scene = &D_0038A640;
    FldSequenceController *controller = &fldSceneLifecycleFlags;

    controller->stage = 0;
    controller->flags = 0;
    scene->primaryState = mode;
    if (mode == 0) {
        scene->sequenceWord3C = record->unk_3c;
        scene->sequenceWord38 = record->unk_38;
        strcpy(scene->entryName, record->name);
        scene->area = record->stage;
        scene->sequenceCode = record->code;
        scene->room = record->kind;
        scene->sequenceMode = record->unk_62;
        scene->unk58 = record->enabled;
        scene->secondaryState = record->mode;
        scene->cameraLinkMode = record->link;
        strcpy(scene->cameraName, record->detail);
        strcpy(scene->resourceName, record->note);
        scene->sequenceOption = record->options;
    }
    kwlnTaskCreate(D_00412FD0, 0x3F7, 0, 0, fldUpdateFieldSceneTask, 0, 0);
    kwlnTaskCreate(D_00412FE0, 0x2B0A, 0, 0, fldProcDraw, 0, 0);
    D_00435F84 = 1;
    fldTestDrawCreate();
}

void fldReleaseCampSceneTasks(void) {
    if (D_00435F84 == 0) return;
    D_00435F84 = 0;
    kwlnFadeResetBackground();
    fldClearSceneControlFlags(0x10);
    fldClearSceneControlFlags(0x20);
    fldSetSceneLifecycleFlags(1);
    fldSetSceneLifecycleFlags(2);
    kwlnTaskDestroyWithHierarchyByName(D_00412FD0, 1);
    kwlnTaskDestroyWithHierarchyByName(D_00412FE0, 1);
    fldDestroyPanelTaskIfPresent();
    evtSetSolarOverlayFullyTransparent();
    fldDestroyTask();
    if (D_00435F74 != 0) {
        effBlurReleaseFirstResource(D_00435F74);
        D_00435F74 = 0;
    }
    mnuDestroyCampTasks();
    scrDestroyAllNamedProcesses();
    fldReleaseMenuSlotsAfterWait();
}

u8 fldIsFieldResourceWaitFinished(void) {
    if (D_00389790[0] == 0 && fldIsAreaResourceReady() != 0) {
        fldPollAreaResourceLoad();
        if (fldGetResourceReadyFlag() == 1) return 0;
        fldFreeDisplayObjects();
        return 0;
    }
    return sdfCheckPendingWorkWithInterrupts() == 0;
}

void fldApplyTerminalActorSelection(void) {
    fldSelectActorFromSceneIndexTables();
}

extern void fldDispatchDeferredFieldCommand(void);

void fldStopSceneBgm(void) {
    fldStopCurrentBgm();
}

void fldProcessDeferredSceneCommand(void) {
    if (fldDeferredCommand != 0) {
        fldDispatchDeferredFieldCommand();
        return;
    }
    func_00141898();
}

void fldSetDeferredFieldCommand(u32 command, u32 parameter) {
    fldDeferredCommand = command;
    fldDeferredCommandParameter = parameter;
}

extern s8 dds3AdminGetRequestedMode(void);

extern s32 dds3AdminReadPreviousUnsignedSample(void);

extern s32 fldStartDeferredActorTransition(s32, u32);

void fldDispatchDeferredFieldCommand(void) {
    if (fldDeferredCommand == 0) {
        return;
    }
    if (dds3AdminGetRequestedMode() > 0) {
        return;
    }
    if (dds3AdminGetRequestedMode() < 0 && (dds3AdminReadPreviousUnsignedSample() & 1) != 0) {
        return;
    }
    if (fldStartDeferredActorTransition(fldDeferredCommand, fldDeferredCommandParameter) == 0) {
        fldDeferredCommand = 0;
    }
}

void fldSetPendingSceneAction(u32 argument) {
    D_00435F70 = argument;
}

void fldRunPendingSceneAction(void) {
    u32 argument;

    argument = D_00435F70;
    if (argument != 0) {
        func_001411F8(argument);
        D_00435F70 = 0;
    }
}

/* Consume the pending pair of field-script values. -1 in both slots means
 * no request; the first value is returned with its 200-entry base offset. */
s32 fldConsumeNextSceneRequest(s32 *outCode, s32 *outParameter) {
    if (fldAreaState.nextArea == -1 && fldAreaState.nextFloor == fldAreaState.nextArea) {
        return 0;
    }
    *outCode = fldAreaState.nextArea + 0xC8;
    *outParameter = fldAreaState.nextFloor;
    fldAreaState.nextArea = -1;
    fldAreaState.nextFloor = -1;
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00124040", D_00412FD0);

INCLUDE_RODATA(const s32, "game/code_00124040", D_00412FE0);

