#include "sdk_ps2.h"
#include "fld_area_work.h"
#include "evt_encounter_request_api.h"
#include "fld.h"
#include "dds3_admin_control_api.h"
#include "common.h"
#include "scr_integer_parameter_api.h"
#include "dds3_camera_update_flags.h"
#include "sdf_delayed_resource_check_api.h"
#include "ps2asm.h"
#include "scr_integer_return_value_api.h"
#include "dds3_task_add_child.h"
#include "sdf_vu_matrix.h"
#include "evt_world_object_api.h"
#include "evt_polygon_movie_flags_api.h"
#include "fld_resource_resolver.h"
#include "dds3_path.h"
#include "evt_stage_anime_control_api.h"
#include "dds3_object_control_slot.h"
#include "dds3_world_selection.h"
#include "eff_dependency.h"
#include "eff_object.h"
#include "fld_player_scene_state.h"
#include "eff_inner_float_api.h"
#include "evt_task.h"
#include "evt_path_direction.h"
#include "scr.h"
#include "scr_command_timer_api.h"
#include "scr_process_handle_api.h"
#include "scr_string_parameter_api.h"
#include "scr_arg_read.h"
#include "evt_solar.h"
#include "dds3_world_counter.h"
#include "sdf_debug_api.h"
#include "dds3_process_registration.h"
#include "evt_picture.h"
#include "evt_event_bgm_readiness.h"
#include "evt_sound_variation.h"

extern char D_003ACAE0[];

extern void rsetTest(s32 bit0, s32 bit1, s32 bit4, s32 bit12,
                               s32 bit14, s32 bit15, s32 unused, s32 bit17);
extern u32 dds3DrawFlag;
EvtPackedSoundTrackId evtLoadEventBgm(s32 id);
EvtEventBgmReadiness evtCheckEventBgmLoaded(s32 id);

extern void fldSetFadeTarget(s32 area, s32 targetMode, s32 duration);
extern void evtSetSkyAlpha(s32 duration, s32 target);

extern struct dds3Basic_t *evtSearchBasic(s32 worldKind, s32 worldObjectKey);
extern EvtPathStatus evtCheckPathStat(struct dds3Basic_t *object);

/* Fixed dispatch payload sizes used by admin-sequence requests. */
enum {
    EVT_SCALAR_REQUEST_SIZE = 4,
    EVT_PAIRED_REQUEST_SIZE = 8,
    EVT_FIELD_SEQUENCE_REQUEST_SIZE = 0xA0
};

/* Script VM helpers (see script/scrCommonCommand.c for the convention). */

/* Selects both the world-node tag and its per-kind scene slot. */
typedef s32 EffWorldKind;
dds3Basic_t *evtSearchBasic(EffWorldKind worldKind, s32 worldObjectKey);

s32 dds3EffectObjectBasicLinkObject(dds3Basic_t *obj, dds3Basic_t *owner);
s32 dds3EffectObjectSetMG12Pos(dds3Basic_t *obj, dds3Basic_t *first,
                                     dds3Basic_t *second, dds3Basic_t *third,
                                     dds3Basic_t *fourth);
s32 dds3EffectObjectBasicLinkObjectIndex(dds3Basic_t *obj, dds3Basic_t *owner, s32 entryId);

void evtEffectLoopControl(dds3Basic_t *unit, s32 flag);

void dds3EffectObjectBasicStatOn(dds3Basic_t *unit, u32 flag);

void dds3AdminiCallSequence(s32 command, const void *payload, u32 payloadSize, s32 mode);
void evtEnableMoonCalc(void);

void evtDisableMoonCalc(void);

void evtSetAgeOfMoon(s32 phase);

void datDebugPrintf(const char *fmt, ...);

void scrAllReleaseScript(void);

void evtFreeAllUnit(void);

void dds3RefreshStoredVec3(dds3Basic_t *object);

void fldPauseBgm(void);

void fldBgmStop(void);

void fldBgmFadeOut(void);

void fldSetSceneParam2(fldSceneParam_t *record, s32 stage, s32 kind, const char *name);

void fldSetSceneParam3(fldSceneParam_t *record, s32 stage, s32 kind, const char *name);
void evtFreeStage(void);

EvtPathLinkResult evtPathLinkAndMake(dds3Basic_t *target, dds3Basic_t *path);

void evtPathRate(dds3Basic_t *unit, f32 value);

s32 fldCommand_GetHojiID(const char *name);

void fldSetMapSlotValueFlag(s32 mapId, u32 slotIndex, s32 bit, s32 enabled);

extern u32 dds3DrawFlag;

void evtPathStop(dds3Basic_t *unit);

void evtPathStart(dds3Basic_t *unit);

void evtPathDir(dds3Basic_t *worldNode, EvtPathDirection direction);

void evtPathLoop(dds3Basic_t *unit, s32 enabled);

extern char D_003AC968[]; /* "BE ok! (%d)\n" */

extern char D_003AC978[];

extern char D_003AC988[]; /* "del BE (%d)..\n" */

extern char D_003ACA40[]; /* "LIGHT_PATH_MOVE error!\n" */

extern char D_003ACA58[]; /* "error: LIGHT_PATH_MOVE.\n" */

void sdfPrintf(const char *msg, ...);

s32 evtRegistBEField(EvtPackEventId eventId, EvtPackFieldResourceId resourceId);

extern s32 D_0032E3C0[];

extern void fldSetDeferredFieldCommand(s32 a, s32 b);

extern dds3Basic_t *dds3AddCameraObjectBasicTypeFree(s32 world, u128 *pos, u128 *rot);

/* Copy two source points into the primary effect object. */
ScrCommandResult evtCommand_EFFMG1_POS(void) {
    dds3Basic_t *primaryEffect;
    dds3Basic_t *sourcePoint0;
    dds3Basic_t *sourcePoint1;

    primaryEffect = evtSearchBasic(EFF_WORLD_KIND_EFFECT_OBJECT, scrGetIntPara(0));
    if (primaryEffect == NULL) {
        datDebugPrintf("EFFMG1_POS mg1 ID error!\n");
        return 1;
    }

    sourcePoint0 = evtSearchBasic(EFF_WORLD_KIND_EFFECT_OBJECT, scrGetIntPara(1));
    sourcePoint1 = evtSearchBasic(EFF_WORLD_KIND_EFFECT_OBJECT, scrGetIntPara(2));
    if (sourcePoint0 == NULL || sourcePoint1 == NULL) {
        datDebugPrintf("EFFMG1_POS point ID not found!\n");
        return 1;
    }
    if (dds3EffectObjectSetMG12Pos(primaryEffect, sourcePoint0, sourcePoint1, NULL, NULL) == 0) {
        datDebugPrintf("EFFMG1_POS set error!\n");
        return 1;
    }
    return 1;
}

extern char D_003AC700[];

extern char D_003AC728[];

extern char D_003AC9F8[];

extern char D_003AC998[];

extern char D_003AC9E0[];

extern char D_003AC958[];

/* Apply four effect parameters after resolving every input object. */
ScrCommandResult evtCommand_EFFMG2_POS(void) {
    dds3Basic_t *primaryEffect;
    dds3Basic_t *sourcePoint0;
    dds3Basic_t *sourcePoint1;
    dds3Basic_t *sourcePoint2;
    dds3Basic_t *sourcePoint3;

    primaryEffect = evtSearchBasic(EFF_WORLD_KIND_EFFECT_OBJECT, scrGetIntPara(0));
    if (primaryEffect == NULL) {
        datDebugPrintf("EFFMG2_POS mg2 ID error!\n");
        return 1;
    }

    sourcePoint0 = evtSearchBasic(EFF_WORLD_KIND_EFFECT_OBJECT, scrGetIntPara(1));
    sourcePoint1 = evtSearchBasic(EFF_WORLD_KIND_EFFECT_OBJECT, scrGetIntPara(2));
    sourcePoint2 = evtSearchBasic(EFF_WORLD_KIND_EFFECT_OBJECT, scrGetIntPara(3));
    sourcePoint3 = evtSearchBasic(EFF_WORLD_KIND_EFFECT_OBJECT, scrGetIntPara(4));

    if (sourcePoint0 == NULL || sourcePoint1 == NULL || sourcePoint2 == NULL || sourcePoint3 == NULL) {
        datDebugPrintf("EFFMG2_POS point ID not found!\n");
        return 1;
    }
    if (dds3EffectObjectSetMG12Pos(primaryEffect, sourcePoint0, sourcePoint1, sourcePoint2, sourcePoint3) == 0) {
        datDebugPrintf("EFFMG2_POS set error!\n");
        return 1;
    }
    return 1;
}

ScrCommandResult evtCommand_EFFECT_ON(void)
{
    s32 effectId;
    dds3Basic_t *effectObject;

    effectId = scrGetIntPara(0);
    effectObject = evtSearchBasic(EFF_WORLD_KIND_EFFECT_OBJECT, effectId);
    if (effectObject != NULL) {
        evtEffectLoopControl(effectObject, 1);
        dds3EffectObjectBasicStatOn(effectObject, 1);
    }
    return 1;
}

/* Bind the selected effect path to the first owner found from camera through light.
 * Missing paths or owners complete the command without binding anything. */
ScrCommandResult evtCommand_EFFECT_LINK(void) {
    dds3Basic_t *effectPath = evtSearchBasic(EFF_WORLD_KIND_EFFECT_OBJECT, scrGetIntPara(0));
    dds3Basic_t *owner;
    EffWorldKind worldKind;

    if (effectPath == NULL) {
        return 1;
    }
    worldKind = EFF_WORLD_KIND_CAMERA;
    do {
        owner = evtSearchBasic(worldKind, scrGetIntPara(1));
        worldKind++;
    } while (worldKind < EFF_WORLD_KIND_SCRIPT_OBJECT && owner == NULL);
    if (owner == NULL) {
        return 1;
    }
    dds3EffectObjectBasicLinkObject(effectPath, owner);
    return 1;
}

/* Search the same owner kinds, then bind with the VM's third entry argument. */
ScrCommandResult evtCommand_EFFECT_LINK2(void) {
    dds3Basic_t *effectPath = evtSearchBasic(EFF_WORLD_KIND_EFFECT_OBJECT, scrGetIntPara(0));
    dds3Basic_t *owner;
    EffWorldKind worldKind;

    if (effectPath == NULL) {
        return 1;
    }
    worldKind = EFF_WORLD_KIND_CAMERA;
    do {
        owner = evtSearchBasic(worldKind, scrGetIntPara(1));
        worldKind++;
    } while (worldKind < EFF_WORLD_KIND_SCRIPT_OBJECT && owner == NULL);
    if (owner == NULL) {
        return 1;
    }
    dds3EffectObjectBasicLinkObjectIndex(effectPath, owner, scrGetIntPara(2));
    return 1;
}

ScrCommandResult evtCommand_EFFECT_UNLINK(void)
{
    s32 effectId;
    dds3Basic_t *effectObject;

    effectId = scrGetIntPara(0);
    effectObject = evtSearchBasic(EFF_WORLD_KIND_EFFECT_OBJECT, effectId);
    if (effectObject == NULL) {
        return 1;
    }
    dds3EffectObjectBasicUnlinkObject(effectObject);
    return 1;
}

ScrCommandResult evtCommand_EFFECT_OFF(void)
{
    s32 effectId;
    dds3Basic_t *effectObject;

    effectId = scrGetIntPara(0);
    effectObject = evtSearchBasic(EFF_WORLD_KIND_EFFECT_OBJECT, effectId);
    if (effectObject != NULL) {
        evtEffectLoopControl(effectObject, 0);
    }
    return 1;
}

ScrCommandResult evtCommand_EFFECT_PATH_MOVE(void) {
    dds3Basic_t *effect;
    dds3Basic_t *path;

    effect = evtSearchBasic(EFF_WORLD_KIND_EFFECT_OBJECT, scrGetIntPara(0));
    if (effect == NULL) {
        datDebugPrintf(D_003AC700);
        return 1;
    }
    path = evtSearchBasic(EFF_WORLD_KIND_ACTION_10, scrGetIntPara(1));
    if (path == NULL) {
        datDebugPrintf(D_003AC728);
        return 1;
    }
    evtPathLinkAndMake(path, effect);
    return 1;
}

extern EvtPathStatus evtCheckPathStat(dds3Basic_t *object);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC700);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC728);

ScrCommandResult evtCommand_EFFECT_PATH_WAIT(void) {
    s32 effectId = scrGetIntPara(0);
    dds3Basic_t *effectObject = evtSearchBasic(EFF_WORLD_KIND_EFFECT_OBJECT, effectId);
    EvtPathStatus scaleStatus;

    if (effectObject == NULL) {
        datDebugPrintf("EFFECT_PATH_WAIT not found eff!\n");
        return 1;
    }
    scaleStatus = evtCheckPathStat(effectObject);
    switch (scaleStatus) {
    case EVT_PATH_STATUS_AT_ENDPOINT:
        return SCR_COMMAND_RESULT_COMPLETE;
    case EVT_PATH_STATUS_IN_PROGRESS:
        return SCR_COMMAND_RESULT_PENDING;
    case EVT_PATH_STATUS_INVALID:
        return SCR_COMMAND_RESULT_COMPLETE;
    default:
        return SCR_COMMAND_RESULT_PENDING;
    }
}

/* Store the key in the VM return slot. The callback result only records
 * command completion. */
ScrCommandResult evtCommand_SEARCH_ID(void) {
    dds3Basic_t *worldNode;

    worldNode = dds3SceneBasicSearchName((dds3Basic_t *)dds3GetControlScene(),
                                              (const u8 *)scrGetStrPara(0));
    if (worldNode == NULL) {
        printf("ID : id not found!! <%s>\n", scrGetStrPara(0));
        datDebugPrintf("WARNING: ID not found! <%s>\n", scrGetStrPara(0));
        scrSetIntReturnValue(0);
    } else {
        scrSetIntReturnValue(worldNode->key);
    }
    return SCR_COMMAND_RESULT_COMPLETE;
}

ScrCommandResult evtCommand_RTRN_TITLE(void)
{
    dds3AdminiCallSequence(2, 0, 0, 0);
    return 1;
}

ScrCommandResult evtCommand_CALL_STAFF(void)
{
    dds3AdminiCallSequence(0xc, 0, 0, 0);
    return 1;
}

ScrCommandResult evtCommand_CALL_ENDSAVE(void)
{
    dds3AdminiCallSequence(4, 0, 0, 0);
    return 1;
}

/* Forward two VM integers as the admin request's two-word payload. */
ScrCommandResult evtCommand_CALL_SAVE(void)
{
    s32 payload[2];

    payload[0] = scrGetIntPara(0);
    payload[1] = scrGetIntPara(1);
    dds3AdminiCallSequence(0x15, payload, EVT_PAIRED_REQUEST_SIZE, 0);
    scrSetIntReturnValue(0);
    return 1;
}

/* Log all three arguments, then re-read command and parameter for dispatch. */
ScrCommandResult evtCommand_CALL_NEXT(void) {
    s32 loggedArgument = scrGetIntPara(0);
    s32 loggedCommand = scrGetIntPara(1);
    s32 loggedParameter = scrGetIntPara(2);
    s32 command;

    printf("CALL_NEXT(%d,%d,%d)\n", loggedArgument, loggedCommand, loggedParameter);
    command = scrGetIntPara(1);
    fldSetDeferredFieldCommand(command, scrGetIntPara(2));
    return 1;
}

/* The event ID becomes a four-byte dispatch payload. */
ScrCommandResult evtCommand_CALL_EVENT(void)
{
    s32 eventId;

    eventId = scrGetIntPara(0);
    printf("call_event:%d\n", eventId);
    dds3AdminiCallSequence(6, &eventId, EVT_SCALAR_REQUEST_SIZE, 0);
    scrAllReleaseScript();
    return 1;
}

ScrCommandResult evtCommandSignalAdminAtTimerZero(void)
{
    if (scrGetCommandTimer() == 0) {
        dds3AdminiExitSequence();
    }
    return SCR_COMMAND_RESULT_PENDING;
}

/* Queue the encounter and retain a positive follow-up event ID for dispatch. */
void evtSubmitEventRequest(FldBattleSceneRecordIndex sceneRecordIndex,
                           EvtScriptEventId followupEventId)
{
    s32 payload[2];

    fldBgmStop();
    fldRequestEncounterWithFade(FLD_ENCOUNTER_MODE_SCRIPT_EVENT, sceneRecordIndex);
    evtPendingEventSelection = followupEventId;
    payload[0] = 0;
    payload[1] = sceneRecordIndex;
    dds3AdminiCallSequence(0xe, payload, EVT_PAIRED_REQUEST_SIZE, followupEventId > 0);
    scrAllReleaseScript();
}

ScrCommandResult evtCommandSubmitEvent(void)
{
    FldBattleSceneRecordIndex sceneRecordIndex;

    sceneRecordIndex = scrGetIntPara(0);
    evtSubmitEventRequest(sceneRecordIndex, 0);
    return 1;
}

/* Stop BGM and submit the event payload with immediate dispatch enabled. */
void evtSubmitEventRequestImmediate(FldBattleSceneRecordIndex sceneRecordIndex)
{
    s32 payload[2];

    fldPauseBgm();
    fldRequestEncounterWithFade(FLD_ENCOUNTER_MODE_SCRIPT_EVENT, sceneRecordIndex);
    evtPendingEventSelection = 0;
    payload[0] = 0;
    payload[1] = sceneRecordIndex;
    dds3AdminiCallSequence(0xe, payload, EVT_PAIRED_REQUEST_SIZE, 1);
    scrAllReleaseScript();
}

ScrCommandResult evtCommand_CALL_BATTLE2(void)
{
    FldBattleSceneRecordIndex sceneRecordIndex;

    sceneRecordIndex = scrGetIntPara(0);
    evtSubmitEventRequestImmediate(sceneRecordIndex);
    return 1;
}

ScrCommandResult evtCommand_RESTORE_BATTLE(void)
{
    FldBattleSceneRecordIndex sceneRecordIndex;
    EvtScriptEventId followupEventId;

    sceneRecordIndex = scrGetIntPara(0);
    followupEventId = scrGetIntPara(1);
    evtSubmitEventRequest(sceneRecordIndex, followupEventId);
    return 1;
}

/* A field-sequence request is a fixed 0xa0-byte VM message. */
ScrCommandResult evtCommand_CALL_FIELD(void)
{
    s32 stage;
    s32 kind;
    const char *sequenceName;
    fldSceneParam_t requestBuffer;

    fldBgmFadeOut();
    stage = scrGetIntPara(0);
    kind = scrGetIntPara(1);
    sequenceName = scrGetStrPara(2);
    fldSetSceneParam2(&requestBuffer, stage, kind, sequenceName);
    dds3AdminiCallSequence(5, &requestBuffer, EVT_FIELD_SEQUENCE_REQUEST_SIZE, 0);
    scrAllReleaseScript();
    return 1;
}

/* Use the current group's stage with the VM's kind and sequence-name inputs. */
ScrCommandResult evtCommand_CALL_FIELD2(void)
{
    s32 kind;
    const char *sequenceName;
    fldSceneParam_t requestBuffer;

    kind = scrGetIntPara(0);
    sequenceName = scrGetStrPara(1);
    fldSetSceneParam2(&requestBuffer, D_0032E3C0[0], kind, sequenceName);
    dds3AdminiCallSequence(5, &requestBuffer, EVT_FIELD_SEQUENCE_REQUEST_SIZE, 0);
    scrAllReleaseScript();
    return 1;
}

/* Forward the first VM integer as an opaque four-byte admin payload. */
ScrCommandResult evtCommandSubmitScalarAdminRequest(void)
{
    s32 payloadValue;

    payloadValue = scrGetIntPara(0);
    dds3AdminiCallSequence(0x1c, &payloadValue, EVT_SCALAR_REQUEST_SIZE, 0);
    return 1;
}

/* Build the current group's field request using the alternate initializer. */
ScrCommandResult evtCommand_CALL_FIELD3(void)
{
    s32 kind;
    const char *sequenceName;
    fldSceneParam_t requestBuffer;

    kind = scrGetIntPara(0);
    sequenceName = scrGetStrPara(1);
    fldSetSceneParam3(&requestBuffer, D_0032E3C0[0], kind, sequenceName);
    dds3AdminiCallSequence(5, &requestBuffer, EVT_FIELD_SEQUENCE_REQUEST_SIZE, 0);
    scrAllReleaseScript();
    return 1;
}

ScrCommandResult evtCommand_FIELD_BE(void)
{
    EvtPackEventId eventId;
    EvtPackFieldResourceId resourceId;

    sdfPrintf("FIELD_BE start\n");
    eventId = scrGetIntPara(0);
    resourceId = scrGetIntPara(1);
    evtRegistBEField(eventId, resourceId);
    sdfPrintf("FIELD_BE end\n");
    return 1;
}

ScrCommandResult evtCommand_FIELD_DSP(void)
{
    s32 area;
    s32 room;
    Dds3AreaRoomKey areaRoomKey;

    area = scrGetIntPara(0);
    room = scrGetIntPara(1);
    areaRoomKey = (area << 16) + room;
    if (dds3GetMapNo(dds3GetControlScene()) != areaRoomKey) {
        area = scrGetIntPara(0);
        room = scrGetIntPara(1);
        evtLoadArea(area, room);
    }
    return 1;
}

ScrCommandResult evtCommand_FIELD_ANIME_START(void)
{
    s32 baseMode;

    baseMode = scrGetIntPara(0);
    evtStageAnimeControl(0, baseMode);
    return 1;
}

ScrCommandResult evtCommand_FIELD_ANIME_STOP(void)
{
    s32 upperMode;

    upperMode = scrGetIntPara(0);
    evtStageAnimeControl(1, upperMode);
    return 1;
}

ScrCommandResult evtCommand_FIELD_CLS(void)
{
    evtFreeAllUnit();
    while (sdfDelayedResrcCheck()) {
    }
    evtFreeStage();
    while (sdfDelayedResrcCheck()) {
    }
    dds3DrawFlag |= 0x2000000;
    printf("event stage shutdown !!\n");
    datDebugPrintf("field shutdown.\n");
    return 1;
}

ScrCommandResult evtCommand_FIELD_CLS_STOP(void)
{
    evtFreeAllUnit();
    while (sdfDelayedResrcCheck()) {
    }
    evtFreeStage();
    while (sdfDelayedResrcCheck()) {
    }
    dds3DrawFlag |= 0x2000000;
    printf("event stage shutdown2 !!\n");
    datDebugPrintf("field shutdown2.\n");
    return 1;
}

ScrCommandResult evtCommand_UNIT_CLS(void)
{
    evtFreeAllUnit();
    printf("unit all clear !!\n");
    datDebugPrintf("unit all clear.\n");
    return 1;
}

ScrCommandResult evtCommand_UNIT_CLS_STOP(void)
{
    evtFreeAllUnit();
    while (sdfDelayedResrcCheck()) {
    }
    printf("unit all clear2 !!\n");
    datDebugPrintf("unit all clear2.\n");
    return 1;
}

ScrCommandResult evtCommandAddEffectUnitToWorld(void) {
    dds3Basic_t *cameraOwner;

    if (scrGetIntPara(0) < 0) {
        cameraOwner = fldGetPlayerSceneState();
    } else {
        cameraOwner = evtSearchBasic(EFF_WORLD_KIND_CAMERA, scrGetIntPara(0));
    }
    if (cameraOwner == NULL) {
        return 1;
    }
    dds3ReplaceActiveCameraSceneBasic(dds3GetActiveScene(), cameraOwner);
    return 1;
}

ScrCommandResult evtCommand_CAM_SEL_FOVY(void) {
    dds3Basic_t *cameraOwner;
    CameraData *cameraData;

    if (scrGetIntPara(0) < 0) {
        cameraOwner = fldGetPlayerSceneState();
    } else {
        cameraOwner = evtSearchBasic(EFF_WORLD_KIND_CAMERA, scrGetIntPara(0));
        cameraData = cameraOwner->workdata;
        cameraData->fovUpdatePending |= DDS3_CAMERA_UPDATE_FOV_BIT;
    }
    if (cameraOwner == NULL) {
        return 1;
    }
    dds3ReplaceActiveCameraSceneBasic(dds3GetActiveScene(), cameraOwner);
    return 1;
}

/* Relink the selected camera owner to its path, reporting any failure. */
ScrCommandResult evtCommand_CAM_PATH_MOVE(void) {
    dds3Basic_t *owner;
    dds3Basic_t *path;

    if (scrGetIntPara(0) < 0) {
        owner = fldGetPlayerSceneState();
    } else {
        owner = evtSearchBasic(EFF_WORLD_KIND_CAMERA, scrGetIntPara(0));
    }
    if (owner == NULL) {
        printf("CAM_PATH_MOVE error!\n");
        return 1;
    }
    path = evtSearchBasic(EFF_WORLD_KIND_ACTION_10, scrGetIntPara(1));
    if (path == NULL) {
        printf("CAM_PATH_MOVE error!\n");
        return 1;
    }
    if (evtPathLinkAndMake(path, owner) == EVT_PATH_LINK_FAILED) {
        printf("CAM_PATH_MOVE error!\n");
        return 1;
    }
    return 1;
}

ScrCommandResult evtCommand_CAM_PATH_WAIT(void) {
    dds3Basic_t *cameraOwner;
    EvtPathStatus scaleStatus;

    if (scrGetIntPara(0) < 0) {
        cameraOwner = fldGetPlayerSceneState();
    } else {
        cameraOwner = evtSearchBasic(EFF_WORLD_KIND_CAMERA, scrGetIntPara(0));
    }
    if (cameraOwner == NULL) {
        return 1;
    }
    scaleStatus = evtCheckPathStat(cameraOwner);
    switch (scaleStatus) {
    case EVT_PATH_STATUS_AT_ENDPOINT:
        return SCR_COMMAND_RESULT_COMPLETE;
    case EVT_PATH_STATUS_IN_PROGRESS:
        return SCR_COMMAND_RESULT_PENDING;
    case EVT_PATH_STATUS_INVALID:
        return SCR_COMMAND_RESULT_COMPLETE;
    default:
        return SCR_COMMAND_RESULT_PENDING;
    }
}

/* Create with a zero position and identity rotation; return the world value to the VM. */
ScrCommandResult evtCommand_CAM_CREATE(void) {
    f32 position[4];
    f32 rotation[4];
    s32 world;

    memset(position, 0, 0x10);
    memset(rotation, 0, 0x10);
    rotation[3] = 1.0f;
    world = dds3AllocObjectId();
    dds3GetRadiusPoseSet(dds3AddCameraObjectBasicTypeFree(world, (u128 *)position, (u128 *)rotation), 1.0f);
    scrSetIntReturnValue(world);
    return 1;
}

ScrCommandResult evtCommand_CAM_CLS(void)
{
    s32 cameraObjectId;
    dds3Basic_t *cameraObject;

    cameraObjectId = scrGetIntPara(0);
    cameraObject = evtSearchBasic(EFF_WORLD_KIND_CAMERA, cameraObjectId);
    if (cameraObject == NULL) {
        return 1;
    }
    dds3DeleteSceneBasicData(cameraObject);
    return 1;
}

ScrCommandResult evtCommand_CAM_POS_XYZ(void) {
    dds3Basic_t *cameraObject;
    f32 position[4];

    memset(position, 0, sizeof(position));
    if (scrGetIntPara(0) < 0) {
        cameraObject = fldGetPlayerSceneState();
    } else {
        cameraObject = evtSearchBasic(EFF_WORLD_KIND_CAMERA, scrGetIntPara(0));
    }
    if (cameraObject == NULL) {
        return 1;
    }
    position[0] = scrGetFloatPara(1);
    position[1] = scrGetFloatPara(2);
    position[2] = scrGetFloatPara(3);
    dds3SetPosPoseSet(cameraObject, (u128 *)position);
    return 1;
}

/* Convert the three degree inputs before composing and storing a quaternion. */
ScrCommandResult evtCommand_CAM_ROT_XYZ(void) {
    dds3Basic_t *cameraObject;
    f32 quaternion[4];
    f32 degreesToRadians;
    f32 pitch;

    if (scrGetIntPara(0) < 0) {
        cameraObject = fldGetPlayerSceneState();
    } else {
        cameraObject = evtSearchBasic(EFF_WORLD_KIND_CAMERA, scrGetIntPara(0));
    }
    if (cameraObject == NULL) {
        return 1;
    }
    degreesToRadians = 0.017453293f;
    pitch = scrGetFloatPara(1) * degreesToRadians;
    FromEuler(pitch, scrGetFloatPara(2) * degreesToRadians, 0.0f);
    SDF_VU0_COPYVECTOR_A_TO_B();
    FromEuler(0.0f, 0.0f, scrGetFloatPara(3) * degreesToRadians);
    Mult();
    SDF_VU0_STOREVECTOR(quaternion);
    dds3SetRotPoseSet(cameraObject, (u128 *)quaternion);
    return 1;
}

ScrCommandResult evtCommand_CAM_ROT_Q(void) {
    dds3Basic_t *cameraObject;
    f32 quaternion[4];

    if (scrGetIntPara(0) < 0) {
        cameraObject = fldGetPlayerSceneState();
    } else {
        cameraObject = evtSearchBasic(EFF_WORLD_KIND_CAMERA, scrGetIntPara(0));
    }
    if (cameraObject == NULL) {
        return 1;
    }
    quaternion[0] = scrGetFloatPara(1);
    quaternion[1] = scrGetFloatPara(2);
    quaternion[2] = scrGetFloatPara(3);
    quaternion[3] = scrGetFloatPara(4);
    dds3SetRotPoseSet(cameraObject, (u128 *)quaternion);
    return 1;
}

extern void dds3MoverBasicMakePath(dds3Basic_t *object);
extern char D_003AC8E8[], D_003AC8F8[], D_003AC908[];

ScrCommandResult evtCommand_OBJ_PATH_MOVE(void) {
    dds3Basic_t *unit;
    dds3Basic_t *target;
    dds3ObjectBaseControlWorkData_t *data;
    dds3Basic_t *slotNode;
    dds3PathBasicControl_t *path;
    Dds3SlotResource *resource;
    s32 roomNumber;
    EvtPathDirection direction;

    unit = evtSearchBasic(EFF_WORLD_KIND_EFFECT_TRANSFORM, scrGetIntPara(0));
    if (unit == NULL) {
        unit = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (unit == NULL) {
            printf(D_003AC8E8, scrGetIntPara(0));
            printf(D_003AC8F8, scrGetIntPara(1));
            printf(D_003AC908);
            return 1;
        }
    }
    target = evtSearchBasic(EFF_WORLD_KIND_ACTION_10, scrGetIntPara(1));
    if (target == NULL)
        return 1;
    data = dds3MatterObjectBasicGetControlWorkData(unit);
    slotNode = data->slots[DDS3_OBJECT_DATA_SLOT];
    if (slotNode == NULL) {
        dds3ObjectBaseCreateMover(unit);
        slotNode = data->slots[DDS3_OBJECT_DATA_SLOT];
    }
    resource = slotNode->workdata;
    if (resource->sourceObject == NULL) {
        dds3MoverBasicSetPath(slotNode, target);
        dds3MoverBasicMakePath(slotNode);
    }
    path = dds3MoverBasicGetPathControl(slotNode);
    if (path == 0)
        return 1;
    direction = scrGetIntPara(2);
    switch (direction) {
    case EVT_PATH_DIRECTION_FORWARD:
        dds3PathBasicControlAlterMode(&path->counter, EVT_PATH_DIRECTION_FORWARD);
        if (target->name != 0) {
            roomNumber = fldCommand_GetHojiID(target->name);
            if (roomNumber > 0)
                fldSetMapSlotValueFlag(fldGlobalBase.area, fldGlobalBase.floor + 1, roomNumber, 1);
        }
        break;
    case EVT_PATH_DIRECTION_REVERSE:
        dds3PathBasicControlAlterMode(&path->counter, EVT_PATH_DIRECTION_REVERSE);
        if (target->name != 0) {
            roomNumber = fldCommand_GetHojiID(target->name);
            if (roomNumber > 0)
                fldSetMapSlotValueFlag(fldGlobalBase.area, fldGlobalBase.floor + 1, roomNumber, 0);
        }
        break;
    }
    return 1;
}

extern char D_003AC928[]; /* "re attach...!\n" */

ScrCommandResult evtCommandReattachObjectPath(void) {
    dds3Basic_t *unit;
    dds3Basic_t *target;
    dds3ObjectBaseControlWorkData_t *data;
    dds3Basic_t *slotNode;
    dds3PathBasicControl_t *path;
    Dds3SlotResource *resource;
    s32 targetId;
    EvtPathDirection direction;

    unit = evtSearchBasic(EFF_WORLD_KIND_EFFECT_TRANSFORM, scrGetIntPara(0));
    if (unit == NULL) {
        unit = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (unit == NULL) {
            printf(D_003AC8E8, scrGetIntPara(0));
            targetId = scrGetIntPara(1);
            printf(D_003AC8F8, targetId);
            printf(D_003AC908);
            return 1;
        }
    }
    target = evtSearchBasic(EFF_WORLD_KIND_ACTION_10, scrGetIntPara(1));
    if (target == NULL) {
        return 1;
    }
    data = dds3MatterObjectBasicGetControlWorkData(unit);
    slotNode = data->slots[DDS3_OBJECT_DATA_SLOT];
    if (slotNode == NULL) {
        dds3ObjectBaseCreateMover(unit);
        slotNode = data->slots[DDS3_OBJECT_DATA_SLOT];
    }
    resource = slotNode->workdata;
    if (resource->sourceObject == NULL) {
        dds3MoverBasicSetPath(slotNode, target);
        dds3MoverBasicMakePath(slotNode);
    } else {
        dds3RefreshStoredVec3(unit);
        dds3MoverBasicSetPath(slotNode, target);
        dds3MoverBasicMakePath(slotNode);
        printf(D_003AC928);
    }
    path = dds3MoverBasicGetPathControl(slotNode);
    if (path == 0) {
        return 1;
    }
    direction = scrGetIntPara(2);
    switch (direction) {
    case EVT_PATH_DIRECTION_FORWARD:
        dds3PathBasicControlAlterMode(&path->counter, EVT_PATH_DIRECTION_FORWARD);
        break;
    case EVT_PATH_DIRECTION_REVERSE:
        dds3PathBasicControlAlterMode(&path->counter, EVT_PATH_DIRECTION_REVERSE);
        break;
    }
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC8E8);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC8F8);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC908);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC928);

ScrCommandResult evtCommand_OBJ_PATH_WAIT(void) {
    dds3Basic_t *worldObject = evtSearchBasic(EFF_WORLD_KIND_EFFECT_TRANSFORM, scrGetIntPara(0));
    EvtPathStatus scaleStatus;

    if (worldObject == NULL) {
        worldObject = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (worldObject == NULL) {
            printf(D_003AC8E8, scrGetIntPara(0));
            printf(D_003AC8F8, scrGetIntPara(1));
            printf("e OBJ_PATH_WAIT : obj==NULL\n");
            return 1;
        }
    }
    scaleStatus = evtCheckPathStat(worldObject);
    switch (scaleStatus) {
    case EVT_PATH_STATUS_AT_ENDPOINT:
        return SCR_COMMAND_RESULT_COMPLETE;
    case EVT_PATH_STATUS_IN_PROGRESS:
        return SCR_COMMAND_RESULT_PENDING;
    case EVT_PATH_STATUS_INVALID:
        return SCR_COMMAND_RESULT_COMPLETE;
    default:
        return SCR_COMMAND_RESULT_PENDING;
    }
}

ScrCommandResult evtCommand_MOON_START(void)
{
    evtEnableMoonCalc();
    return SCR_COMMAND_RESULT_COMPLETE;
}

ScrCommandResult evtCommand_MOON_STOP(void)
{
    evtDisableMoonCalc();
    return SCR_COMMAND_RESULT_COMPLETE;
}

ScrCommandResult evtCommand_GET_MOON(void)
{
    scrSetIntReturnValue(evtGetAgeOfMoon());
    return SCR_COMMAND_RESULT_COMPLETE;
}

ScrCommandResult evtCommand_GET_MOON16(void)
{
    scrSetIntReturnValue(evtGetAgeOfMoon16());
    return SCR_COMMAND_RESULT_COMPLETE;
}

ScrCommandResult evtCommand_CHG_MOON(void)
{
    evtSetAgeOfMoon(scrGetIntPara(0));
    return SCR_COMMAND_RESULT_COMPLETE;
}

/* Create a missing camp task, then wait for its ready value.
 * No VM context completes immediately; a pending task returns zero. */
ScrCommandResult evtCommand_LOAD_BE(void) {
    EvtPackEventId eventId = scrGetIntPara(0);
    scrProcessWork_s *commandWork = scrGetScriptHandle();
    char *message;

    if (commandWork == NULL) {
        return SCR_COMMAND_RESULT_COMPLETE;
    }
    if (evtSearchEventPack(eventId) == 0) {
        message = D_003AC958;
        datDebugPrintf(message, eventId);
        sdfPrintf(message, eventId);
        dds3LinkProcess(commandWork->ProcessID, evtLoadEventPack(eventId));
        return SCR_COMMAND_RESULT_PENDING;
    }
    if (evtGetEventPackStat(eventId) == EVT_PACK_LOAD_COMPLETE) {
        message = D_003AC968;
        datDebugPrintf(message, eventId);
        sdfPrintf(message, eventId);
        return SCR_COMMAND_RESULT_COMPLETE;
    }
    return SCR_COMMAND_RESULT_PENDING;
}

/* Start the camp task only if no task with this ID exists yet.
 * A missing VM context completes without creating a task. */
ScrCommandResult evtCommand_READ_BE(void) {
    EvtPackEventId eventId = scrGetIntPara(0);
    scrProcessWork_s *commandWork = scrGetScriptHandle();

    if (commandWork == NULL) {
        return SCR_COMMAND_RESULT_COMPLETE;
    }
    if (evtSearchEventPack(eventId) != 0) {
        return SCR_COMMAND_RESULT_COMPLETE;
    }
    datDebugPrintf(D_003AC978, eventId);
    dds3LinkProcess(commandWork->ProcessID, evtLoadEventPack(eventId));
    return SCR_COMMAND_RESULT_COMPLETE;
}

/* Store readiness in the VM result; the command itself always completes. */
ScrCommandResult evtCommand_CHK_BE(void) {
    EvtPackEventId eventId = scrGetIntPara(0);
    EvtPackLoadQueryResult result;

    if (scrGetScriptHandle() != 0 && evtSearchEventPack(eventId) != 0 && evtGetEventPackStat(eventId) == EVT_PACK_LOAD_COMPLETE) {
        datDebugPrintf(D_003AC968, eventId);
        result = EVT_PACK_LOAD_QUERY_READY;
    } else {
        result = EVT_PACK_LOAD_QUERY_NOT_READY;
    }
    scrSetIntReturnValue(result);
    return SCR_COMMAND_RESULT_COMPLETE;
}

/* Destroy the requested camp task; missing context or task is a completed no-op. */
ScrCommandResult evtCommand_DEL_BE(void) {
    EvtPackEventId eventId = scrGetIntPara(0);

    if (scrGetScriptHandle() == 0) {
        return SCR_COMMAND_RESULT_COMPLETE;
    }
    if (evtSearchEventPack(eventId) == 0) {
        return SCR_COMMAND_RESULT_COMPLETE;
    }
    datDebugPrintf(D_003AC988, eventId);
    evtKillEventPack(eventId);
    return SCR_COMMAND_RESULT_COMPLETE;
}

extern dds3ProcessID_t * evtViewerCreateTask(u32 priority, s32 eventId, s32 sceneId);

/* Create an event/scene movie task at the owning task's priority, hold its
 * viewer setup until the script releases it,
 * and return its handle through the VM result. Missing work skips creation. */
ScrCommandResult evtCommandStartPolygonMovie(void) {
    scrProcessWork_s *commandWork = scrGetScriptHandle();
    s32 eventId;
    s32 sceneId;
    dds3ProcessID_t *movieTask;

    if (commandWork == NULL) {
        return 1;
    }
    if (commandWork->ProcessID == 0) {
        printf(D_003AC998);
        return 1;
    }
    eventId = scrGetIntPara(0);
    sceneId = scrGetIntPara(1);
    movieTask = evtViewerCreateTask(commandWork->ProcessID->priority, eventId, sceneId);
    datDebugPrintf(D_003AC9E0, scrGetIntPara(0), scrGetIntPara(1));
    dds3LinkProcess(commandWork->ProcessID, movieTask);
    evtPolygonMovieSetFlagBits(movieTask, EVT_POLY_MOVIE_SCRIPT_STARTUP_HOLD);
    scrSetIntReturnValue(scrProcessHandleToVmInteger(movieTask));
    return 1;
}

/* Release viewer setup and echo the movie ID through the VM result. */
ScrCommandResult evtCommandClearPolygonMovieFlag(void)
{
    dds3ProcessID_t *movieTask;

    movieTask = scrVmIntegerToProcessHandle(scrGetIntPara(0));
    evtPolygonMovieClearFlagBits(movieTask, EVT_POLY_MOVIE_SCRIPT_STARTUP_HOLD);
    scrSetIntReturnValue(scrProcessHandleToVmInteger(movieTask));
    return 1;
}

/* Create and return an event/scene movie task without holding viewer setup.
 * The owning scheduler task supplies the viewer task's priority. */
ScrCommandResult evtCommand_PMV_RUN(void) {
    scrProcessWork_s *commandWork = scrGetScriptHandle();
    s32 eventId;
    s32 sceneId;
    dds3ProcessID_t *movieTask;

    if (commandWork == NULL) {
        return 1;
    }
    if (commandWork->ProcessID == 0) {
        printf(D_003AC9F8);
        return 1;
    }
    eventId = scrGetIntPara(0);
    sceneId = scrGetIntPara(1);
    movieTask = evtViewerCreateTask(commandWork->ProcessID->priority, eventId, sceneId);
    dds3LinkProcess(commandWork->ProcessID, movieTask);
    scrSetIntReturnValue(scrProcessHandleToVmInteger(movieTask));
    return 1;
}

/* Look up a world unit across the six kinds before trying the fallback slot. */
ScrCommandResult evtCommand_PATH_STOP(void) {
    dds3Basic_t *slotObject;
    EffWorldKind worldKind = EFF_WORLD_KIND_CAMERA;

    do {
        slotObject = evtSearchBasic(worldKind, scrGetIntPara(0));
        worldKind++;
    } while (worldKind < EFF_WORLD_KIND_SCRIPT_OBJECT && slotObject == 0);
    if (slotObject == 0) {
        slotObject = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (slotObject == 0) {
            return 1;
        }
    }
    evtPathStop(slotObject);
    return 1;
}

/* Resume the first matching path controller, using the same fallback search. */
ScrCommandResult evtCommand_PATH_START(void) {
    dds3Basic_t *slotObject;
    EffWorldKind worldKind = EFF_WORLD_KIND_CAMERA;

    do {
        slotObject = evtSearchBasic(worldKind, scrGetIntPara(0));
        worldKind++;
    } while (worldKind < EFF_WORLD_KIND_SCRIPT_OBJECT && slotObject == 0);
    if (slotObject == 0) {
        slotObject = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (slotObject == 0) {
            return 1;
        }
    }
    evtPathStart(slotObject);
    return 1;
}

/* Apply the float state, then update a room flag when its parsed number is positive.
 * Values above 0.5 set that flag; the VM float parameter is deliberately re-read. */
ScrCommandResult evtCommand_PATH_RATE(void) {
    dds3Basic_t *worldNode;
    s32 roomNumber;
    const char *roomName;
    EffWorldKind worldKind = EFF_WORLD_KIND_CAMERA;

    do {
        worldNode = evtSearchBasic(worldKind, scrGetIntPara(0));
        worldKind++;
    } while (worldKind < EFF_WORLD_KIND_SCRIPT_OBJECT && worldNode == NULL);
    if (worldNode == NULL) {
        worldNode = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (worldNode == NULL) {
            return 1;
        }
    }
    evtPathRate(worldNode, scrGetFloatPara(1));
    roomName = worldNode->name;
    if (roomName == 0) {
        return 1;
    }
    roomNumber = fldCommand_GetHojiID(roomName);
    if (roomNumber > 0) {
        if (scrGetFloatPara(1) > 0.5f) {
            fldSetMapSlotValueFlag(fldGlobalBase.area, fldGlobalBase.floor + 1, roomNumber, 1);
        } else {
            fldSetMapSlotValueFlag(fldGlobalBase.area, fldGlobalBase.floor + 1, roomNumber, 0);
        }
    }
    return 1;
}

/* Apply the path direction and update the positive room's traversal flag. */
ScrCommandResult evtCommand_PATH_DIR(void) {
    dds3Basic_t *worldNode;
    s32 roomNumber;
    EvtPathDirection direction;
    const char *roomName;
    EffWorldKind worldKind = EFF_WORLD_KIND_CAMERA;

    do {
        worldNode = evtSearchBasic(worldKind, scrGetIntPara(0));
        worldKind++;
    } while (worldKind < EFF_WORLD_KIND_SCRIPT_OBJECT && worldNode == NULL);
    if (worldNode == NULL) {
        worldNode = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (worldNode == NULL) {
            return 1;
        }
    }
    direction = scrGetIntPara(1);
    evtPathDir(worldNode, direction);
    roomName = worldNode->name;
    if (roomName == 0) {
        return 1;
    }
    roomNumber = fldCommand_GetHojiID(roomName);
    if (roomNumber > 0) {
        if (scrGetIntPara(1) == EVT_PATH_DIRECTION_FORWARD) {
            fldSetMapSlotValueFlag(fldGlobalBase.area, fldGlobalBase.floor + 1, roomNumber, 1);
        } else {
            fldSetMapSlotValueFlag(fldGlobalBase.area, fldGlobalBase.floor + 1, roomNumber, 0);
        }
    }
    return 1;
}

/* Enable the scaled-value flag on the first unit found, including the fallback. */
ScrCommandResult evtCommand_PATH_LOOP_ON(void)
{
    EffWorldKind worldKind;
    dds3Basic_t *slotObject;

    worldKind = EFF_WORLD_KIND_CAMERA;
    do {
        slotObject = evtSearchBasic(worldKind, scrGetIntPara(0));
        worldKind++;
    } while (worldKind < EFF_WORLD_KIND_SCRIPT_OBJECT && slotObject == NULL);
    if (slotObject == NULL) {
        slotObject = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (slotObject == NULL) {
            return 1;
        }
    }
    evtPathLoop(slotObject, 1);
    return 1;
}

/* Disable the scaled-value flag using the same six-kind and fallback search. */
ScrCommandResult evtCommand_PATH_LOOP_OFF(void)
{
    EffWorldKind worldKind;
    dds3Basic_t *slotObject;

    worldKind = EFF_WORLD_KIND_CAMERA;
    do {
        slotObject = evtSearchBasic(worldKind, scrGetIntPara(0));
        worldKind++;
    } while (worldKind < EFF_WORLD_KIND_SCRIPT_OBJECT && slotObject == NULL);
    if (slotObject == NULL) {
        slotObject = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (slotObject == NULL) {
            return 1;
        }
    }
    evtPathLoop(slotObject, 0);
    return 1;
}

ScrCommandResult evtCommand_LIGHT_PATH_MOVE(void) {
    dds3Basic_t *pathOwner = evtSearchBasic(EFF_WORLD_KIND_LIGHT, scrGetIntPara(0));

    if (evtPathLinkAndMake(evtSearchBasic(EFF_WORLD_KIND_ACTION_10, scrGetIntPara(1)), pathOwner) != EVT_PATH_LINK_FAILED) {
        return 1;
    }
    printf(D_003ACA40);
    datDebugPrintf(D_003ACA58);
    return 1;
}

ScrCommandResult evtCommand_LIGHT_PATH_WAIT(void) {
    EvtPathStatus status = evtCheckPathStat(
        evtSearchBasic(EFF_WORLD_KIND_LIGHT, scrGetIntPara(0)));

    switch (status) {
    case EVT_PATH_STATUS_AT_ENDPOINT:
        return SCR_COMMAND_RESULT_COMPLETE;
    case EVT_PATH_STATUS_IN_PROGRESS:
        return SCR_COMMAND_RESULT_PENDING;
    case EVT_PATH_STATUS_INVALID:
        return SCR_COMMAND_RESULT_COMPLETE;
    default:
        return SCR_COMMAND_RESULT_PENDING;
    }
}

extern char D_003ACA78[];
void datDebugPrintf(const char *fmt, ...);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC958);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC968);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC978);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC988);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC998);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC9E0);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC9F8);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003ACA40);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003ACA58);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003ACA78);

u32 evtCommand_TEX_BE(void) {
    scrProcessWork_s *work;
    EvtPackEventId eventId;
    struct sdfTexHandle_t *entry;
    dds3ProcessID_t *task;

    work = scrGetScriptHandle();
    if (work == NULL) {
        return 1;
    }
    if (work->ProcessID == NULL) {
        printf(D_003ACA78);
        return 1;
    }
    eventId = scrGetIntPara(0);
    entry = evtGetResorceData(eventId, scrGetIntPara(1));
    if (entry == 0) {
        datDebugPrintf("TEX_BE not fount RID = %d\n", scrGetIntPara(1));
        return 1;
    }
    task = evtStartPictureProcess(0x2AFE, entry);
    dds3LinkProcess(work->ProcessID, task);
    scrSetIntReturnValue(scrProcessHandleToVmInteger(task));
    return 1;
}

u32 evtCommand_TEX_LOAD(void) {
    scrProcessWork_s *work;
    dds3ProcessID_t *task;

    work = scrGetScriptHandle();
    if (work == NULL) {
        return 1;
    }
    if (work->ProcessID == NULL) {
        printf(D_003ACAE0);
        return 1;
    }
    task = evtStartPictureProcess2(0x2afe, scrGetStrPara(0));
    dds3LinkProcess(work->ProcessID, task);
    scrSetIntReturnValue(scrProcessHandleToVmInteger(task));
    return 1;
}

u32 evtCommand_TEX_ON(void) {
    dds3ProcessID_t *task;
    dds3ProcessRegistrationResult_t registered;

    task = scrVmIntegerToProcessHandle(scrGetIntPara(0));
    registered = dds3ExistProcess(task);
    if (registered != DDS3_PROCESS_NOT_REGISTERED) {
        evtPictureDrawOn(task);
    }
    return 1;
}

u32 evtCommand_TEX_OFF(void) {
    dds3ProcessID_t *task;
    dds3ProcessRegistrationResult_t registered;

    task = scrVmIntegerToProcessHandle(scrGetIntPara(0));
    registered = dds3ExistProcess(task);
    if (registered != DDS3_PROCESS_NOT_REGISTERED) {
        evtPictureDrawOff(task);
    }
    return 1;
}

extern void evtKillPictureProcess(dds3ProcessID_t *task);

u32 evtCommand_TEX_CLS(void) {
    dds3ProcessID_t *task;
    dds3ProcessRegistrationResult_t registered;

    task = scrVmIntegerToProcessHandle(scrGetIntPara(0));
    registered = dds3ExistProcess(task);
    if (registered != DDS3_PROCESS_NOT_REGISTERED) {
        evtKillPictureProcess(task);
    }
    return 1;
}

extern void fldSkySetAlpha(u32 drawState);
extern void fldSetFilterAnime(u32 mode);

u32 evtCommand_SET_SKY(void) {
    s32 area;
    s32 target;
    s32 duration;

    fldSetFilterAnime(0);
    fldSkySetAlpha(0x80);
    area = scrGetIntPara(0);
    target = scrGetIntPara(1);
    duration = scrGetIntPara(2);
    fldSetFadeTarget(area, target, duration);
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtCommand", D_003ACAE0);

u32 evtCommand_SET_SKY_A(void) {
    s32 alpha;

    alpha = scrGetIntPara(1);
    if (alpha < -255) {
        datDebugPrintf("warning : SET_SKY_A alpha < -255\n");
        alpha = -255;
    }
    if (alpha > 255) {
        datDebugPrintf("warning : SET_SKY_A alpha > 255\n");
        alpha = 255;
    }
    evtSetSkyAlpha(scrGetIntPara(0), alpha);
    return 1;
}

extern void evtSetBokasi(u32 enabled);

u32 evtCommand_BOKASI_ON(void) {
    evtSetBokasi(1);
    return 1;
}

u32 evtCommand_BOKASI_OFF(void) {
    evtSetBokasi(0);
    return 1;
}

u32 evtCommand_LOAD_SOUND_E(void) {
    s32 eventId;

    /* One parameter call per arm; retail has two call sites and sharing one changes the code. */
    if (scrGetCommandTimer() == 0) {
        evtLoadEventBgm(scrGetIntPara(0));
    } else {
        eventId = scrGetIntPara(0);
        if (evtCheckEventBgmLoaded(eventId) != EVT_EVENT_BGM_NOT_READY) {
            return 1;
        }
    }
    return 0;
}

extern EvtPackedSoundTrackId evtPlayEventBgm(s32 eventId, EvtSoundVariation variation);

u32 evtCommand_BGM_PLAY_E(void) {
    s32 eventId;
    EvtSoundVariation variation;

    eventId = scrGetIntPara(0);
    variation = scrGetIntPara(1);
    evtPlayEventBgm(eventId, variation);
    return 1;
}

extern void evtTransEventBgm(s32 eventId, EvtSoundVariation variation);

u32 evtCommand_BGM_TRANS_E(void) {
    s32 eventId;
    EvtSoundVariation variation;

    eventId = scrGetIntPara(0);
    variation = scrGetIntPara(1);
    evtTransEventBgm(eventId, variation);
    return 1;
}

extern EvtPackedSoundTrackId evtStopEventBgm(s32 eventId, EvtSoundVariation variation);

u32 evtCommand_FADEOUT_E(void) {
    s32 eventId;
    EvtSoundVariation variation;

    eventId = scrGetIntPara(0);
    variation = scrGetIntPara(1);
    evtStopEventBgm(eventId, variation);
    return 1;
}

extern void btlAdvanceTitleStateWithAudioCleanupTask(void);
extern void p2sdBgmVolDown(void);

u32 evtCommand_BGM_VOL_DOWN(void) {
    p2sdBgmVolDown();
    btlAdvanceTitleStateWithAudioCleanupTask();
    return 1;
}

extern void p2sdBgmVolUp(void);

u32 evtCommand_BGM_VOL_UP(void) {
    p2sdBgmVolUp();
    btlAdvanceTitleStateWithAudioCleanupTask();
    return 1;
}

extern EvtPackedSoundTrackId evtPlaySe(s32 eventId, EvtSoundVariation variation);

u32 evtCommand_SE_PLAY_E(void) {
    s32 id;
    EvtSoundVariation variation;

    id = scrGetIntPara(0);
    variation = scrGetIntPara(1);
    evtPlaySe(id, variation);
    return 1;
}

extern EvtPackedSoundTrackId evtStartBgmBySoundIdAndFade(
    s32 eventId, EvtSoundVariation variation);

u32 evtCommandStartBgmWithFade(void) {
    s32 eventId;
    EvtSoundVariation variation;

    eventId = scrGetIntPara(0);
    variation = scrGetIntPara(1);
    evtStartBgmBySoundIdAndFade(eventId, variation);
    return 1;
}

extern void evtRefreshTaskEffectTexture(EvtPackEventId eventId, EvtPackSecondaryResourceId key);

u32 evtOpcodeRefreshTaskData(void) {
    EvtPackEventId eventId;
    EvtPackSecondaryResourceId key;

    eventId = scrGetIntPara(0);
    key = scrGetIntPara(1);
    evtRefreshTaskEffectTexture(eventId, key);
    return 1;
}

extern void Eve_DistortionBlurSetDefTex(void);

u32 evtOpcodeInitializeEffectSoundChannel(void) {
    Eve_DistortionBlurSetDefTex();
    return 1;
}

u32 evtCommandSetDrawFlagWhileWaiting(void) {
    if (scrGetIntPara(0) <= 0) {
        dds3DrawFlag |= 0x2000000;
        return 1;
    }
    if ((s32)scrGetCommandTimer() < scrGetIntPara(0)) {
        dds3DrawFlag |= 0x2000000;
        return 0;
    }
    return 1;
}

