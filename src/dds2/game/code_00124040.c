#include "fld_area_work.h"
#include "sdf_packet_list.h"
#include "common.h"
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
#include "dds3obj.h"
#include "kwln.h"
#include "dat_state.h"
#include "kwln_task_lifecycle.h"

extern s32 fldDeferredCommand;

extern void *dds3GetWorldSecondaryObject(void);

extern u16 dds3GetWorldValueCount(WorldValueIndices *object);

extern u32 dds3ReadIndexedWorldObjectWord(WorldValueIndices *object);

extern u32 dds3AdvanceObjectValueCursor(WorldValueIndices *object);

extern NodeB *dds3CopyWorldListToValueChain(EffWorldNode *object, s32 kind);

extern u32 dds3GetObjectPayloadWord8(EffWorldNode *object);

extern void dds3SetObjectPayloadWord8(EffWorldNode *object, u32 value);

extern u32 D_00435F60;

/* The stage member also has the assembler label D_00435F8C. */
typedef struct FldSequenceController {
    u32 flags;
    s32 stage;
} FldSequenceController;

extern FldSequenceController fldSceneLifecycleFlags;

extern s16 D_00389874[];

extern u32 D_00435CBC;

extern u32 D_004360E8;

extern s32 D_003898AC[];

extern void func_00122E50(void);

extern void func_00125380(void);

extern void func_00135A68(u32, s32);

extern void func_001512D8(void);

extern KwlnTask *func_00101820(u32);

extern s32 func_00153560(s32, s32);

extern void fldResetSecondaryArchiveLoadPhase(void);

extern s32 fldStepSecondaryArchiveLoad(void);

extern s32 fldReportCampVolumeError(void);

extern void fldApplyCurrentAreaActorEntries(void);

extern s32 fldIsTargetWithinInteractionRange(void);

extern void func_00150F20(void);

extern s32 func_00150F10(void);

extern void func_00150A60(void);

extern void func_00153410(void);

extern void func_00154F18(s32);

extern void func_00126C80(void);

extern void fldCreateInputPanelTask(void);

extern void fldEnsureTask(void);

extern void fldRequestMiniTitleDismiss(void);

extern void fldFinishDeferredExit(void);

extern void func_001447A0(void);

/* Providers used by the field lifecycle controller. */
extern void dds3SetWorldObject(void *);

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

extern void func_001355D8(void);

extern void func_0013DDC0(EffWorldNode *);

extern void func_0014D0E8(void);

extern s32 kwlnFadeIsActive(void);

extern void mnuMarkTitleStreamResetPending(void);

extern s32 mnuPollTitleStreamStateLocked(void);

extern void mnuResetTitleStreamLocked(void);

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

extern u32 fldGetSceneReadyFlag(void);

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

extern void mdlLoadViewerPackage(s32, s32, s32, void *, u32);

void func_001258B8(void);

extern u32 D_00389790[];

extern s32 fldIsAreaResourceReady(void);

extern u32 fldPollAreaResourceLoad(void);

extern u32 fldGetResourceReadyFlag(void);

extern void fldFreeDisplayObjects(void);

extern s32 sdfCheckPendingWorkWithInterrupts(void);

extern u32 dds3AdvanceWorldCounter(void);

struct EffWorldNode;

extern void dds3SetWorldNodeValue(struct EffWorldNode *node, u32 value);

extern f32 func_001248E8(f32, f32, f32, f32);

extern u8 D_0037F530[];

extern s32 fldGetEncounterRuntimeResult(void);

extern u8 fldGetCampSceneControlMode(void);

extern u32 D_00389988[];

extern u32 D_00389988[];

extern u32 D_00389988[];

extern KwlnTask *kwlnTaskCreate(const char *name, u32 priority, s32 startDelay, s32 destroyDelay, TaskUpdate update, TaskDestroy destroy, u32 userValue);

extern s32 D_00435EE0;

extern s32 D_00435EE4;

extern s32 D_00435EE8;

extern s32 D_00435EEC;

extern s32 D_00435EF0;

extern s32 D_00435EF4;

extern s32 D_00435EF8;

extern u32 dds3ResetObjectValueCursor(WorldValueIndices *object);

extern void dds3DestroyWorldIndexNode(NodeB *node);

extern void mdlFlagSet(s32);

extern void sdfResetGameRuntime(s32);

extern s32 fileLoadStateChanged(void);

extern void fileCacheSlotFlagsFromState(void);

extern void fileRestoreSlotFlagsToState(void);

void fldSetDeferredFieldCommand(u32, u32);

extern void dds3AdminSubmitModeRequest(s32, void *, s32, s32);

u32 *fldGetPlayerSceneStateAddress(void);

extern void fldResetPlayerSceneTransformState(void);

extern u32 D_00435F78;

extern void fldSetPendingAreaAndFloor(s32, s32);

void fldUnloadPlayerModel(void);

extern void dds3ClearObjectFlags(u32, s32);

extern EffWorldNode *dds3SetWorldPlayerObject(EffWorldNode *object, EffWorldNode *value);

extern void func_00112058(u32, s32, s32);

extern EffWorldNode *dds3SpawnCameraSlotObj5(s32, void *, void *);

extern s32 D_0038A67C[];

extern char D_00412EC0[]; /* "PLAYER_UNIT" */

extern s32 D_00435F30;

extern u8 D_0037FF88[];

extern u8 D_003807F8[];

extern void sdfStoreMessageWordsAndNotifyConsumer(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

typedef struct FldEncEntry {
    s16 stage;
    s16 flag;
    s16 chance;
    s16 result;
} FldEncEntry;

extern FldEncEntry *fldEncounterRollTable;

extern s32 mdlFlagTest(s32 flag);

extern s32 effMiscRand();

extern u32 D_00435F98[2];

extern void fldResetEventSceneState(void);

extern u32 func_00151498(void);

extern void fldResetTaskSlots(void);

extern void fldSetSceneLifecycleFlags(u32);

extern void func_0023ACE8(void);

extern void evtStartSceneResourceTask(u64, void *);

extern void func_00150800(void);

extern s32 D_00435F84, D_00435F74;

extern char D_00412FD0[], D_00412FE0[];

extern void kwlnFadeResetBackground(void);

extern void fldDestroyPanelTaskIfPresent(void), evtSetSolarOverlayFullyTransparent(void), fldDestroyTask(void);

extern void mnuDestroyCampTasks(void), scrDestroyAllNamedProcesses(void);

extern void fldReleaseMenuSlotsAfterWait(void);

void fldSetSceneControlFlags(u32 mask);

s32 func_001266D8(ObjectTransform *relativeTransform, EffWorldNode *target);

extern s32 dds3InvokeSlot1Handler(void *object, Dds3MoverUpdate update);

extern void fldUpdateCameraTarget(void);

extern void func_00139EC0(f32 *position);

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

INCLUDE_ASM(const s32, "game/code_00124040", func_001248E8);

f32 fldGetNormalizedComplementaryAngle(f32 ax, f32 ay, f32 bx, f32 by) {
    s32 angle = (s32)(360.0f - func_001248E8(ax, ay, bx, by) + 90.0f);
    return (f32)(angle % 360);
}

INCLUDE_ASM(const s32, "game/code_00124040", func_00124A10);

f32 fldPointDistance(f32 ax, f32 ay, f32 az, f32 bx, f32 by, f32 bz) {
    f32 dx = ax - bx;
    f32 dy = ay - by;
    f32 dz = az - bz;
    return fsqrtf(dx * dx + dy * dy + dz * dz);
}

/* For kind-6 objects in mode 4, set mode 3 or clear it to 0.
 * status covers the count, payload mode word and cursor result. */
void fldToggleWorldNodeState(s64 clearMode) {
    NodeB *valueChain;
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

extern u32 fileGetSelectionPendingFlag(void);

extern void func_00122B58();

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
    func_00122B58(1);
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

extern void func_00145818();

extern void fldLoadFieldTablesAndIndexStages();

extern u8 D_0038A6E0[];

void fldInitializeDisplayAndSceneSound(void) {
    fldInitDisplayObjects();
    fldResetPlayerSceneTransformState();
    fldResetPendingSounds();
    effMiscSeedRandom(D_0038A6E0, 0x1E240);
    fldParseMixLb();
    fldLoadBattleSkyAndFilter();
    func_00145818();
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
            fldAreaState.unk20 = 1;
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
            fldAreaState.unk20 = 1;
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
            fldAreaState.unk20 = 1;
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
            fldAreaState.unk20 = 1;
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

INCLUDE_ASM(const s32, "game/code_00124040", func_00125380);

struct SdfMemBlock;

extern struct SdfMemBlock *sdfReadNamedResource(const char *name, u32 *outAddress, u32 *outSize);

/* Reload only when the field-selected variant changes or its handle is absent. */
INCLUDE_RODATA(const s32, "game/code_00124040", D_00412D50);

INCLUDE_RODATA(const s32, "game/code_00124040", D_00412D60);

void func_001258B8(void) {
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

INCLUDE_ASM(const s32, "game/code_00124040", func_00125B10);

void fldPrepareResourceBuffer(void) {
    void *source;
    void *buffer;
    func_001258B8();
    D_00435F40 = D_00435F44;
    D_00435F38 = (u32)sdfAllocGeneralBlock(D_00435F44);
    source = (void *)sdfMemoryGetBlockAddress((struct SdfMemBlock *)(u32)fldPlayerModelResource);
    buffer = (void *)sdfMemoryGetBlockAddress((struct SdfMemBlock *)(u32)D_00435F38);
    memcpy(buffer, source, D_00435F40);
    D_00435F3C = (u32)buffer;
    mdlLoadViewerPackage(2, 0, 0x101, buffer, D_00435F40);
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

extern u32 fldCameraModelObject;

extern u32 fldSecondarySceneModelHandle;

extern void effObjFetchInnerFirstVec(u32);

extern void effObjFetchInnerSecondVecNorm(u32);

void fldSnapshotAndReleasePlayerSceneObject(void) {
    FieldPlayerSceneWork *sceneWork;
    if (fldPlayerObject != 0) {
        if (dds3GetWorldSecondaryObject() != 0) {
            sceneWork = &D_0038A640;
            effObjFetchInnerFirstVec(fldPlayerObject);
            VU0_STORE_VF(vf10, &sceneWork->position);
            effObjFetchInnerSecondVecNorm(fldPlayerObject);
            VU0_STORE_VF(vf10, &sceneWork->rotation);
        }
        fldPlayerObject = 0;
        fldSecondarySceneObject = 0;
        fldCameraModelObject = 0;
        fldSecondarySceneModelHandle = 0;
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
        dds3InvokeSlot1Handler((void *)fldPlayerObject, func_001266D8);
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
    effObjFetchInnerFirstVec(fldPlayerObject);
    VU0_STORE_VF(vf10, position);
    position[1] += 10.0f;
    func_00139EC0(position);
}

void fldResetPlayerSceneObjectState(void) {
    if (fldPlayerObject != 0) {
        fldClearSceneControlFlags(0x40);
        dds3InvokeSlot1Handler((void *)fldPlayerObject, 0);
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
        func_00112058(fldPlayerObject, 2, D_0038A67C[0]);
    }
}

extern void effObjSetInnerFloat(u32, f32);

extern void effObjSetInnerFirstVec(EffWorldNode *, u128 *);

extern void effObjSetInnerSecondVec(EffWorldNode *, u128 *);

extern void effObjSetInnerThirdVec(EffWorldNode *, u128 *);

extern void effMiscAxisAngleToQuaternionVU(f32);

extern void effMiscQuatMultiplyVU(void);

extern u32 dds3GetObjectBaseResourceHandle(void *);

extern void dds3SetObjectFlags(void *, u32);

extern void fldResetCameraModelHandles(void);

extern void mdlAddEntryFlagged(MdlCtx *, s32, s32);

extern void mdlFlagClear(s32);

extern void sdfSetTextFloatPairOverride(void *, f32, f32);

extern void fldActivatePendingSceneCommand(void);

extern s32 fldGetSceneCommandState(void);

/* The scene reset ignores the command word supplied by this caller. */
extern void func_00136098();

extern void func_001360B8(s16, s32);

extern s32 func_00133B10(void);

extern EffWorldNode *evtSpawnActionObj11(s32, void *, s32);

extern char D_00412F00[];

extern f32 D_0038A610[12];

extern char D_00435F50[];

INCLUDE_RODATA(const s32, "game/code_00124040", D_00412EB0);

INCLUDE_RODATA(const s32, "game/code_00124040", D_00412EC0);

void func_00126110(f32 *input) {
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
            func_00112058(fldPlayerObject, 2, D_0038A67C[0]);
            player = (EffWorldNode *)fldPlayerObject;
        } else {
            ObjectTransform *inner = player->inner;
            PCP_COPY_VECTOR_F32(inner->position, input);
            PCP_COPY_VECTOR_F32(inner->smoothedPosition, input);
            PCP_COPY_VECTOR_F32(inner->rotation, input + 4);
        }

        effObjSetInnerFirstVec(player, (u128 *)input);
        effObjSetInnerSecondVec((EffWorldNode *)fldPlayerObject, (u128 *)work);
        effObjSetInnerThirdVec((EffWorldNode *)fldPlayerObject, (u128 *)unitScale);
        fldCameraModelObject = dds3GetObjectBaseResourceHandle((EffWorldNode *)fldPlayerObject);
        effObjSetInnerFloat(fldPlayerObject, 90.0f);

        if (fldAreaState.unk118 == 0) {
            mdlAddEntryFlagged((MdlCtx *)fldCameraModelObject, 0, 2);
        } else {
            mdlAddEntryFlagged((MdlCtx *)fldCameraModelObject, 0, 2);
            mdlAddEntryFlagged((MdlCtx *)fldCameraModelObject, 1, 2);
            fldResetCameraModelHandles();
        }
        sdfSetTextFloatPairOverride(((MdlCtx *)fldCameraModelObject)->inner, 15.0f, 0.0f);
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
            func_00112058(fldSecondarySceneObject, 2, D_0038A67C[0]);
            effObjSetInnerFirstVec((EffWorldNode *)fldSecondarySceneObject, (u128 *)input);
            effObjSetInnerSecondVec((EffWorldNode *)fldSecondarySceneObject, (u128 *)work);
            effObjSetInnerThirdVec((EffWorldNode *)fldSecondarySceneObject, (u128 *)secondaryScale);
            fldSecondarySceneModelHandle = dds3GetObjectBaseResourceHandle((EffWorldNode *)fldSecondarySceneObject);
            effObjSetInnerFloat(fldSecondarySceneObject, 90.0f);
            mdlAddEntryFlagged((MdlCtx *)fldSecondarySceneModelHandle, 0, 0);
            sdfSetTextFloatPairOverride(((MdlCtx *)fldSecondarySceneModelHandle)->inner, 15.0f, 0.0f);
            ((MdlCtx *)fldSecondarySceneModelHandle)->inner->flags |= 8;
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
            func_001360B8(0, 0);
            func_00135A68(D_00389988[0x2C / 4], 0);
            fldBeginSelectedValueTransition(D_00389988[0x30 / 4]);
            if (fldGetSceneCommandState() == 1) {
                if (fldAreaState.commandEnabled != 0) {
                    fldActivatePendingSceneCommand();
                } else {
                    mdlFlagClear(0x818);
                    fldAreaState.sceneCommand = 0;
                    func_001360B8(0, 0);
                    func_00135A68(D_00389988[0x2C / 4], 0);
                    fldBeginSelectedValueTransition(D_00389988[0x30 / 4]);
                }
            } else if (fldAreaState.commandEnabled == 0) {
                mdlFlagClear(0x818);
                fldAreaState.sceneCommand = 0;
            }
            func_00136098(fldAreaState.commandEnabled);
        }
        func_00133B10();
        evtSpawnActionObj11(dds3AdvanceWorldCounter(), D_0038A610, (s32)(u32)D_00435F50);
    }
}

typedef struct FieldVec4 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} FieldVec4;

extern FieldVec4 D_00412F10;

extern FieldVec4 D_00412F20;

extern char D_00435F58[];

extern u32 dds3CreateConfiguredCameraObject(s32, FieldVec4 *, FieldVec4 *, FieldVec4 *);

extern void dds3SetCameraVector(struct EffWorldNode *camera, u128 *worldEye);

extern void effObjSetInnerFloat(u32, f32);

extern EffWorldNode *dds3SetWorldCameraObject(EffWorldNode *, EffWorldNode *);

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

s32 func_001266D8(ObjectTransform *relativeTransform, EffWorldNode *target) {
    return 0;
}

u32 func_001266E0(void) {
    return 100;
}

void fldConsumeSceneCommandFlag(void) {
    if ((datGameState->world.fieldFlags & 8) != 0) {
        fldClearSceneCommandFlag();
        fldAreaState.consumedFlags |= 1;
    }
}

extern void mdlFlagClear(s32);

extern void evtClearSolarOverlayControl(void);

void fldClearSceneCommandFlag(void) {
    datGameState->world.fieldFlags &= ~8;
    fldAreaState.consumedFlags &= ~1;
    mdlFlagClear(0x818);
    evtClearSolarOverlayControl();
}

extern void fldPlayMenuSound(s32);

extern void func_001360B8(s16, s32);

void fldActivatePendingSceneCommand(void) {
    u32 value;
    if (fldAreaState.commandEnabled != 0) {
        if ((datGameState->world.fieldFlags & 8) == 0) {
            fldPlayMenuSound(0x29);
        }
        mdlFlagSet(0x818);
        value = func_001266E0();
        fldAreaState.sceneCommand = value;
        datGameState->world.fieldFlags |= 8;
        fldAreaState.consumedFlags &= ~1;
        func_001360B8(value, 0);
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
            func_001360B8(0, 0);
        }
    } else if ((datGameState->world.fieldFlags & 8) != 0) {
        if (scene->sceneCommand == 0) {
            value = func_001266E0();
            scene->sceneCommand = value;
            func_001360B8(value, 0x14);
        }
    } else if (scene->sceneCommand != 0) {
        scene->sceneCommand = 0;
        func_001360B8(0, 0x14);
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

extern void func_00243320(void);

/* Clear the transition work flag and its area-state consumption marker. */
void fldClearFieldTransitionFlag(void) {
    datGameState->world.fieldFlags &= ~FIELD_TRANSITION_WORK_FLAG;
    fldAreaState.consumedFlags &= ~FIELD_TRANSITION_CONSUMED_FLAG;
    func_00243320();
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

void func_00126B70(void) {
    sdfStoreMessageWordsAndNotifyConsumer(D_003807F8, (s32)D_0037FF88, (s32)(D_0037FF88 + 0xC0), (s32)(D_0037FF88 + 0x100), (s32)(D_0037FF88 + 0xE0));
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

INCLUDE_ASM(const s32, "game/code_00124040", func_00126C80);

/* Queue the current area and next floor, reset the scene lifecycle, and return -1.
 * The pending pair is consumed later with a 200-entry offset on the area value. */
s32 fldAdvanceToNextScene(void) {
    u32 scene;
    u32 area;

    fldResetEventSceneState();
    D_00435F98[0] = 0;
    D_00435F98[1] = func_00151498();
    scene = fldAreaState.floor;
    area = fldAreaState.area;
    fldAreaState.nextArea = area;
    fldAreaState.nextFloor = scene + 1;
    fldResetTaskSlots();
    fldSetSceneLifecycleFlags(1);
    fldSetSceneLifecycleFlags(2);
    fldResetPlayerSceneObjectState();
    func_0023ACE8();
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

extern void *dds3GetWorldObject(void);

extern void dds3SetWorldObjectDataValue(EffWorldNode *, s8);

extern void kwlnFadeStartIn(s32);

extern void kwlnFadeInStart(s32, s32, s32, s32);

extern void func_00149A00(void);

extern void func_00133F08(void);

extern u8 fldHasPendingSceneFlags(void);

extern void fldSetCameraNodeModeWithTen(void);

extern void func_00123B88(s32, s32, f32, f32, f32);

extern void kwlnFadeStartOut(s32 duration);

extern void mnuCreateCampTasks(void);

extern void evtSetSolarOverlayFullyVisible(void);

extern void fldApplySkyLightSetToPlayerVU(void);

extern void sndSetSequenceVolumePan(s32 sequence, s32 volume, s32 pan);

extern void fldClearSceneLifecycleFlags(u32 mask);

extern u8 fldTestSceneLifecycleFlags(u32 mask);

extern s16 D_00389898[];

/* Complete a delayed camp handoff or advance the player scene transition. */
s32 func_001273E8(void) {
    FldAreaWork *scene;
    s32 control;

    if (D_00435F7C > 0) {
        D_00435F7C--;
        if (D_00435F7C == 0) {
            dds3SetWorldObjectDataValue(dds3GetWorldObject(), 0);
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
        dds3SetWorldObjectDataValue(dds3GetWorldObject(), 1);
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
        func_00133F08();
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
                evtStartSceneResourceTask((u64)dds3GetWorldObject(), D_00412F58);
            }
        }
    } else {
        if (D_00435F80 > 0) {
            D_00435F80--;
            if (D_00435F80 == 0) {
                dds3SetWorldObjectDataValue(dds3GetWorldObject(), 0);
                kwlnFadeStartIn(4);
                func_00149A00();
            }
            return 0;
        }
        if (fldGetSceneReadyOrPendingState() != 0) {
            return 0;
        }
        func_00123B88(scene->floor, scene->unkC0, scene->x, scene->z, 50.0f);
        if (scene->sceneMode == 0) {
            if (fldHasPendingSceneFlags() != 0) {
                return 0;
            }
            if (fldTestSceneControlFlags(0x40) != 0 && transitionInput != 0 && D_00389988[0x48 / 4] == 0) {
                scene->sceneMode = 4;
                scene->sceneState = 5;
                fldResetPlayerSceneObjectState();
                func_00133F08();
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

s32 func_001278D0(void) {
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
            func_001273E8();
            if (controller->stage == 4 && D_003898AC[0] != 0 && D_00389988[0x5C / 4] != 0) {
                fldReportCampVolumeError();
            }
            return 0;
        }
    }
    if (fldTestSceneControlFlags(0x40) == 0) func_00133F08();
    switch (controller->stage) {
    case 0:
        D_00435F78 = 0;
        fldAreaState.unk130 = 1;
        func_00122E50();
        D_00435CBC = 0x80000000;
        evtEnableSolarPhaseAdvance();
        D_004360E8 = 0;
        func_00125380();
        dds3SetWorldObject(dds3GetWorldSecondaryObject());
        fldCreateInputPanelTask();
        if (fldAreaState.transitionMode == 0) evtSetSolarOverlayFullyVisible();
        fldEnsureTask();
        if (fldAreaState.area >= 200) {
            D_00389988[0x2C / 4] = 0;
            D_00389988[0x30 / 4] = 0;
            func_00135A68(0, 0);
            fldBeginSelectedValueTransition(D_00389988[0x30 / 4]);
        }
        if (fldAreaState.transitionMode == 0) func_001512D8();
        coordinateFlags = fldGetLocationCoordinateValue(fldAreaState.area, fldAreaState.floor + 1);
        if (coordinateFlags & 0x40) {
            blur.count = 15;
            blur.delaySpread = 15;
            blur.angleStep = 0.1999999881f;
            blur.color = 0x3C989884;
            blur.blendControl = 0x44;
            blur.uvDisplacementAngleDegrees = 0.005f;
            blur.uvDisplacementAmplitude = 0.13f;
            blur.x = 0;
            blur.y = 0;
            blur.positionSpread = 0x100;
            blur.size = 200;
            D_00435F74 = (s32)effBlurCreateScatterWork(&blur);
        } else if (coordinateFlags & 0x80) {
            blur.count = 60;
            blur.delaySpread = 15;
            blur.angleStep = 0.221999988f;
            blur.color = 0x28848484;
            blur.blendControl = 0x44;
            blur.uvDisplacementAngleDegrees = 0.005f;
            blur.uvDisplacementAmplitude = 0.13f;
            blur.x = 0;
            blur.y = 0;
            blur.positionSpread = 0x100;
            blur.size = 136;
            D_00435F74 = (s32)effBlurCreateScatterWork(&blur);
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
            if (func_00153560(fldAreaState.area, fldAreaState.floor + 1) != 0) {
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
        if (func_00101820(0x3EA) == NULL && func_00150F10() == 0) {
            fldPreparePlayerSceneCameraTarget();
        }
        dds3ClearObjectFlags(fldPlayerObject, 0x40);
        fldSetSceneControlFlags(0x10);
        fldSetSceneControlFlags(0x20);
        fldRequestMiniTitleDismiss();
        if (fldAreaState.deferredExit == 1) fldStartDeferredFieldExit();
        break;
    case 4:
        if (fldAreaState.unk13C != 0 && D_00389988[0x5C / 4] != 0) func_00153410();
        D_004360E8 = 1;
        if (fldTestSceneControlFlags(0x40) != 0 && fldRestartSceneResourceTask() != 0) return 0;
        if (fldTestSceneControlFlags(0x40) != 0 && fldAreaState.titleFade != 0) {
            func_0013DDC0((EffWorldNode *)fldPlayerObject);
            break;
        }
        fldFinishDeferredExit();
        if (fldAreaState.transitionMode != 0) {
            func_00150F20();
            func_00123B88(fldAreaState.floor, fldAreaState.unkC0,
                          fldAreaState.x, fldAreaState.z, 50.0f);
        } else if (D_00389988[0] == 0) {
            func_001273E8();
            if (fldTestSceneControlFlags(0x20) == 0) break;
            if (D_00389988[0] == 0) fldUpdateNextFloorTransition();
        }
        if (func_00150F10() != 0) func_00150A60();
        if (fldAreaState.pendingSceneRequest == 1) fldAdvanceToNextScene();
        if (fldIsTargetWithinInteractionRange() != 0 && fldAreaState.targetGuideActive == 1) {
            func_00154F18(1);
            func_00126C80();
        } else if (fldTestSceneControlFlags(0x40) != 0) {
            func_00154F18(1);
            func_00126C80();
        }
        if (fldTestSceneLifecycleFlags(2) == 1) {
            controller->stage++;
            break;
        }
        fldUpdateCameraFrame();
        if (fldTestSceneControlFlags(0x40) != 0) func_0013DDC0((EffWorldNode *)fldPlayerObject);
        break;
    case 5:
        dds3SetWorldObjectDataValue(dds3GetWorldObject(), 0);
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
        dds3AdminSubmitModeRequest(14, D_00435F98, 8, 1);
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
        func_001355D8();
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
            func_0014D0E8();
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

INCLUDE_ASM(const s32, "game/code_00124040", func_001283B8);

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
        effBlurReleaseFirstResource((EffBlurScatterWork *)D_00435F74);
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

void func_001285E8(void) {
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

extern s32 func_00141CF0(s32, u32);

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
    if (func_00141CF0(fldDeferredCommand, fldDeferredCommandParameter) == 0) {
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
