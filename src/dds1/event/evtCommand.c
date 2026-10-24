#include "sdk_ps2.h"
#include "fld_area_work.h"
#include "fld.h"
#include "dds3_admin_control_api.h"
#include "common.h"
#include "sdf_vu0.h"
#include "scr_integer_return_value_api.h"
#include "kwln_task_add_child.h"
#include "sdf_vu_matrix.h"
#include "evt_world_object_api.h"
#include "fld_resource_resolver.h"
#include "dds3_path.h"
#include "dds3obj.h"
#include "dds3_world_selection.h"
#include "eff_dependency.h"
#include "eff_object.h"
#include "fld_player_scene_state.h"
#include "eff_inner_float_api.h"
#include "pcp_vu0.h"
#include "kwln.h"
#include "evt_task.h"
#include "evt_world.h"
#include "evt_event_pack.h"
#include "scr.h"
#include "scr_command_timer_api.h"
#include "scr_string_parameter_api.h"
#include "scr_arg_read.h"
#include "evt_solar.h"
#include "dds3_world_counter.h"

/* Fixed dispatch payload sizes and the native six-kind world-object scan. */
enum {
    EVT_SCALAR_REQUEST_SIZE = 4,
    EVT_PAIRED_REQUEST_SIZE = 8,
    EVT_FIELD_SEQUENCE_REQUEST_SIZE = 0xA0,
    EVT_WORLD_OBJECT_KIND_FIRST = 4,
    EVT_WORLD_OBJECT_KIND_LIMIT = 10
};

/* Script VM helpers (see script/scrCommonCommand.c for the convention). */
s32 scrGetIntPara(s32 idx);


dds3Basic_t *evtSearchBasic(s32 objectKind, s32 worldObjectKey);

s32 dds3EffectObjectBasicLinkObject(dds3Basic_t *obj, dds3Basic_t *owner);
s32 dds3EffectObjectSetMG12Pos(dds3Basic_t *obj, dds3Basic_t *first,
                                     dds3Basic_t *second, dds3Basic_t *third,
                                     dds3Basic_t *fourth);
s32 dds3EffectObjectBasicLinkObjectIndex(dds3Basic_t *obj, dds3Basic_t *owner, s32 entryId);

void evtEffectLoopControl(dds3Basic_t *unit, s32 flag);

void dds3EffectObjectBasicStatOn(dds3Basic_t *unit, s32 flag);

void evtStageAnimeControl(s32 modeSelector, s32 modeValue);

void dds3AdminiCallSequence(s32 command, const void *payload, u32 payloadSize, s32 mode);
void evtSubmitEventRequest(s32 eventId, s32 mode);

void evtSubmitEventRequestImmediate(s32 eventId);

void evtEnableMoonCalc(void);

void evtDisableMoonCalc(void);



void evtSetAgeOfMoon(s32 phase);

void evtPrintDeveloperConsoleMessage(const char *fmt, ...);

#include "sdf_debug_api.h"

void scrDestroyAllNamedProcesses(void);

void evtFreeAllUnit(void);



void dds3RefreshStoredVec3(dds3Basic_t *object);

void fldPauseBgm(void);

void fldBgmStop(void);

void fldBgmFadeOut(void);


void fldSetSceneParam2(fldSceneParam_t *record, s32 stage, s32 kind, const char *name);

void fldSetSceneParam3(fldSceneParam_t *record, s32 stage, s32 kind, const char *name);

s32 sdfDelayedResrcCheck(void);

void evtFreeStage(void);

s32 evtPathLinkAndMake(dds3Basic_t *target, dds3Basic_t *path);

void evtPathRate(dds3Basic_t *unit, f32 value);

s32 fldCommand_GetHojiID(const char *name);

void fldSetMapSlotValueFlag(s32 mapId, u32 slotIndex, s32 bit, s32 enabled);



extern u32 dds3DrawFlag;


void evtPathStop(dds3Basic_t *unit);

void evtPathStart(dds3Basic_t *unit);

void evtPathDir(dds3Basic_t *worldNode, u32 direction);

void evtPathLoop(dds3Basic_t *unit, s32 enabled);


extern char D_003AC968[]; /* "BE ok! (%d)\n" */

extern char D_003AC978[];

extern char D_003AC988[]; /* "del BE (%d)..\n" */

extern char D_003ACA40[]; /* "LIGHT_PATH_MOVE error!\n" */

extern char D_003ACA58[]; /* "error: LIGHT_PATH_MOVE.\n" */

void sdfPrintf(const char *msg, ...);

s32 evtRegistBEField(s32 eventId, s32 resourceId);

extern s32 D_0032E3C0[];

void fldRequestEncounterWithFade(s32 kind, s32 eventId);

extern u32 evtPendingEventSelection;






extern void fldSetDeferredFieldCommand(s32 a, s32 b);



extern dds3Basic_t *dds3AddCameraObjectBasicTypeFree(s32 world, void *pos, void *rot);


/* Copy two source points into the primary effect object. */
s32 evtCommand_EFFMG1_POS(void) {
    dds3Basic_t *primaryEffect;
    dds3Basic_t *sourcePoint0;
    dds3Basic_t *sourcePoint1;

    primaryEffect = evtSearchBasic(7, scrGetIntPara(0));
    if (primaryEffect == NULL) {
        evtPrintDeveloperConsoleMessage("EFFMG1_POS mg1 ID error!\n");
        return 1;
    }

    sourcePoint0 = evtSearchBasic(7, scrGetIntPara(1));
    sourcePoint1 = evtSearchBasic(7, scrGetIntPara(2));
    if (sourcePoint0 == NULL || sourcePoint1 == NULL) {
        evtPrintDeveloperConsoleMessage("EFFMG1_POS point ID not found!\n");
        return 1;
    }
    if (dds3EffectObjectSetMG12Pos(primaryEffect, sourcePoint0, sourcePoint1, NULL, NULL) == 0) {
        evtPrintDeveloperConsoleMessage("EFFMG1_POS set error!\n");
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
s32 evtCommand_EFFMG2_POS(void) {
    dds3Basic_t *primaryEffect;
    dds3Basic_t *sourcePoint0;
    dds3Basic_t *sourcePoint1;
    dds3Basic_t *sourcePoint2;
    dds3Basic_t *sourcePoint3;

    primaryEffect = evtSearchBasic(7, scrGetIntPara(0));
    if (primaryEffect == NULL) {
        evtPrintDeveloperConsoleMessage("EFFMG2_POS mg2 ID error!\n");
        return 1;
    }

    sourcePoint0 = evtSearchBasic(7, scrGetIntPara(1));
    sourcePoint1 = evtSearchBasic(7, scrGetIntPara(2));
    sourcePoint2 = evtSearchBasic(7, scrGetIntPara(3));
    sourcePoint3 = evtSearchBasic(7, scrGetIntPara(4));

    if (sourcePoint0 == NULL || sourcePoint1 == NULL || sourcePoint2 == NULL || sourcePoint3 == NULL) {
        evtPrintDeveloperConsoleMessage("EFFMG2_POS point ID not found!\n");
        return 1;
    }
    if (dds3EffectObjectSetMG12Pos(primaryEffect, sourcePoint0, sourcePoint1, sourcePoint2, sourcePoint3) == 0) {
        evtPrintDeveloperConsoleMessage("EFFMG2_POS set error!\n");
        return 1;
    }
    return 1;
}

s32 evtCommand_EFFECT_ON(void)
{
    s32 effectId;
    dds3Basic_t *effectObject;

    effectId = scrGetIntPara(0);
    effectObject = evtSearchBasic(7, effectId);
    if (effectObject != NULL) {
        evtEffectLoopControl(effectObject, 1);
        dds3EffectObjectBasicStatOn(effectObject, 1);
    }
    return 1;
}

/* Bind the selected effect path to the first owner found in kinds 4..9.
 * Missing paths or owners complete the command without binding anything. */
s32 evtCommand_EFFECT_LINK(void) {
    dds3Basic_t *effectPath = evtSearchBasic(7, scrGetIntPara(0));
    dds3Basic_t *owner;
    s32 objectKind;

    if (effectPath == NULL) {
        return 1;
    }
    objectKind = EVT_WORLD_OBJECT_KIND_FIRST;
    do {
        owner = evtSearchBasic(objectKind, scrGetIntPara(1));
        objectKind++;
    } while (objectKind < EVT_WORLD_OBJECT_KIND_LIMIT && owner == NULL);
    if (owner == NULL) {
        return 1;
    }
    dds3EffectObjectBasicLinkObject(effectPath, owner);
    return 1;
}

/* Search the same owner kinds, then bind with the VM's third entry argument. */
s32 evtCommand_EFFECT_LINK2(void) {
    dds3Basic_t *effectPath = evtSearchBasic(7, scrGetIntPara(0));
    dds3Basic_t *owner;
    s32 objectKind;

    if (effectPath == NULL) {
        return 1;
    }
    objectKind = EVT_WORLD_OBJECT_KIND_FIRST;
    do {
        owner = evtSearchBasic(objectKind, scrGetIntPara(1));
        objectKind++;
    } while (objectKind < EVT_WORLD_OBJECT_KIND_LIMIT && owner == NULL);
    if (owner == NULL) {
        return 1;
    }
    dds3EffectObjectBasicLinkObjectIndex(effectPath, owner, scrGetIntPara(2));
    return 1;
}

s32 evtCommand_EFFECT_UNLINK(void)
{
    s32 effectId;
    dds3Basic_t *effectObject;

    effectId = scrGetIntPara(0);
    effectObject = evtSearchBasic(7, effectId);
    if (effectObject == NULL) {
        return 1;
    }
    dds3EffectObjectBasicUnlinkObject(effectObject);
    return 1;
}

s32 evtCommand_EFFECT_OFF(void)
{
    s32 effectId;
    dds3Basic_t *effectObject;

    effectId = scrGetIntPara(0);
    effectObject = evtSearchBasic(7, effectId);
    if (effectObject != NULL) {
        evtEffectLoopControl(effectObject, 0);
    }
    return 1;
}

s32 evtCommand_EFFECT_PATH_MOVE(void) {
    dds3Basic_t *effect;
    dds3Basic_t *path;

    effect = evtSearchBasic(7, scrGetIntPara(0));
    if (effect == NULL) {
        evtPrintDeveloperConsoleMessage(D_003AC700);
        return 1;
    }
    path = evtSearchBasic(0x10, scrGetIntPara(1));
    if (path == NULL) {
        evtPrintDeveloperConsoleMessage(D_003AC728);
        return 1;
    }
    evtPathLinkAndMake(path, effect);
    return 1;
}

extern char D_003AC750[];

extern EvtPathStatus evtCheckPathStat(dds3Basic_t *object);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC700);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC728);

s32 evtCommand_EFFECT_PATH_WAIT(void) {
    s32 effectId = scrGetIntPara(0);
    dds3Basic_t *effectObject = evtSearchBasic(7, effectId);
    EvtPathStatus scaleStatus;

    if (effectObject == NULL) {
        evtPrintDeveloperConsoleMessage("EFFECT_PATH_WAIT not found eff!\n");
        return 1;
    }
    scaleStatus = evtCheckPathStat(effectObject);
    switch (scaleStatus) {
    case EVT_PATH_STATUS_AT_ENDPOINT:
        return 1;
    case EVT_PATH_STATUS_IN_PROGRESS:
        return 0;
    case EVT_PATH_STATUS_INVALID:
        return 1;
    default:
        return 0;
    }
}

/* Return the named world-chain node's key to the script VM. */
s32 evtCommand_SEARCH_ID(void) {
    dds3Basic_t *worldNode;

    worldNode = dds3SceneBasicSearchName((dds3Basic_t *)dds3GetControlScene(),
                                              (const u8 *)scrGetStrPara(0));
    if (worldNode == NULL) {
        printf("ID : id not found!! <%s>\n", scrGetStrPara(0));
        evtPrintDeveloperConsoleMessage("WARNING: ID not found! <%s>\n", scrGetStrPara(0));
        scrSetIntReturnValue(0);
    } else {
        scrSetIntReturnValue(worldNode->key);
    }
    return 1;
}

u32 evtCommand_RTRN_TITLE(void)
{
    dds3AdminiCallSequence(2, 0, 0, 0);
    return 1;
}

u32 evtCommand_CALL_STAFF(void)
{
    dds3AdminiCallSequence(0xc, 0, 0, 0);
    return 1;
}

u32 evtCommand_CALL_ENDSAVE(void)
{
    dds3AdminiCallSequence(4, 0, 0, 0);
    return 1;
}

/* Forward two VM integers as the admin request's two-word payload. */
s32 evtCommand_CALL_SAVE(void)
{
    s32 payload[2];

    payload[0] = scrGetIntPara(0);
    payload[1] = scrGetIntPara(1);
    dds3AdminiCallSequence(0x15, payload, EVT_PAIRED_REQUEST_SIZE, 0);
    scrSetIntReturnValue(0);
    return 1;
}

/* Log all three arguments, then re-read command and parameter for dispatch. */
s32 evtCommand_CALL_NEXT(void) {
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
s32 evtCommand_CALL_EVENT(void)
{
    s32 eventId;

    eventId = scrGetIntPara(0);
    printf("call_event:%d\n", eventId);
    dds3AdminiCallSequence(6, &eventId, EVT_SCALAR_REQUEST_SIZE, 0);
    scrDestroyAllNamedProcesses();
    return 1;
}

u32 evtCommandSignalAdminAtTimerZero(void)
{
    if (scrGetCommandTimer() == 0) {
        dds3AdminiExitSequence();
    }
    return 0;
}

/* Queue an event request after preparing BGM/encounter state, then stop scripts.
 * The mode is retained separately; only its positivity controls dispatch mode. */
void evtSubmitEventRequest(s32 eventId, s32 requestMode)
{
    s32 payload[2];

    fldBgmStop();
    fldRequestEncounterWithFade(2, eventId);
    evtPendingEventSelection = requestMode;
    payload[0] = 0;
    payload[1] = eventId;
    dds3AdminiCallSequence(0xe, payload, EVT_PAIRED_REQUEST_SIZE, requestMode > 0);
    scrDestroyAllNamedProcesses();
}

s32 evtCommandSubmitEvent(void)
{
    s32 eventId;

    eventId = scrGetIntPara(0);
    evtSubmitEventRequest(eventId, 0);
    return 1;
}

/* Stop BGM and submit the event payload with immediate dispatch enabled. */
void evtSubmitEventRequestImmediate(s32 eventId)
{
    s32 payload[2];

    fldPauseBgm();
    fldRequestEncounterWithFade(2, eventId);
    evtPendingEventSelection = 0;
    payload[0] = 0;
    payload[1] = eventId;
    dds3AdminiCallSequence(0xe, payload, EVT_PAIRED_REQUEST_SIZE, 1);
    scrDestroyAllNamedProcesses();
}

s32 evtCommand_CALL_BATTLE2(void)
{
    s32 eventId;

    eventId = scrGetIntPara(0);
    evtSubmitEventRequestImmediate(eventId);
    return 1;
}

s32 evtCommand_RESTORE_BATTLE(void)
{
    s32 eventId;
    s32 requestMode;

    eventId = scrGetIntPara(0);
    requestMode = scrGetIntPara(1);
    evtSubmitEventRequest(eventId, requestMode);
    return 1;
}

/* A field-sequence request is a fixed 0xa0-byte VM message. */
s32 evtCommand_CALL_FIELD(void)
{
    s32 stage;
    s32 kind;
    char *sequenceName;
    fldSceneParam_t requestBuffer;

    fldBgmFadeOut();
    stage = scrGetIntPara(0);
    kind = scrGetIntPara(1);
    sequenceName = scrGetStrPara(2);
    fldSetSceneParam2(&requestBuffer, stage, kind, sequenceName);
    dds3AdminiCallSequence(5, &requestBuffer, EVT_FIELD_SEQUENCE_REQUEST_SIZE, 0);
    scrDestroyAllNamedProcesses();
    return 1;
}

/* Use the current group's stage with the VM's kind and sequence-name inputs. */
s32 evtCommand_CALL_FIELD2(void)
{
    s32 kind;
    char *sequenceName;
    fldSceneParam_t requestBuffer;

    kind = scrGetIntPara(0);
    sequenceName = scrGetStrPara(1);
    fldSetSceneParam2(&requestBuffer, D_0032E3C0[0], kind, sequenceName);
    dds3AdminiCallSequence(5, &requestBuffer, EVT_FIELD_SEQUENCE_REQUEST_SIZE, 0);
    scrDestroyAllNamedProcesses();
    return 1;
}

/* Forward the first VM integer as an opaque four-byte admin payload. */
s32 evtCommandSubmitScalarAdminRequest(void)
{
    s32 payloadValue;

    payloadValue = scrGetIntPara(0);
    dds3AdminiCallSequence(0x1c, &payloadValue, EVT_SCALAR_REQUEST_SIZE, 0);
    return 1;
}

/* Build the current group's field request using the alternate initializer. */
s32 evtCommand_CALL_FIELD3(void)
{
    s32 kind;
    char *sequenceName;
    fldSceneParam_t requestBuffer;

    kind = scrGetIntPara(0);
    sequenceName = scrGetStrPara(1);
    fldSetSceneParam3(&requestBuffer, D_0032E3C0[0], kind, sequenceName);
    dds3AdminiCallSequence(5, &requestBuffer, EVT_FIELD_SEQUENCE_REQUEST_SIZE, 0);
    scrDestroyAllNamedProcesses();
    return 1;
}

s32 evtCommand_FIELD_BE(void)
{
    s32 eventId;
    s32 resourceId;

    sdfPrintf("FIELD_BE start\n");
    eventId = scrGetIntPara(0);
    resourceId = scrGetIntPara(1);
    evtRegistBEField(eventId, resourceId);
    sdfPrintf("FIELD_BE end\n");
    return 1;
}

s32 evtCommand_FIELD_DSP(void)
{
    s32 area;
    s32 room;
    s32 worldKey;

    area = scrGetIntPara(0);
    room = scrGetIntPara(1);
    worldKey = (area << 16) + room;
    if (dds3GetMapNo(dds3GetControlScene()) != worldKey) {
        area = scrGetIntPara(0);
        room = scrGetIntPara(1);
        evtLoadArea(area, room);
    }
    return 1;
}

u32 evtCommand_FIELD_ANIME_START(void)
{
    s32 baseMode;

    baseMode = scrGetIntPara(0);
    evtStageAnimeControl(0, baseMode);
    return 1;
}

u32 evtCommand_FIELD_ANIME_STOP(void)
{
    s32 upperMode;

    upperMode = scrGetIntPara(0);
    evtStageAnimeControl(1, upperMode);
    return 1;
}

s32 evtCommand_FIELD_CLS(void)
{
    evtFreeAllUnit();
    while (sdfDelayedResrcCheck()) {
    }
    evtFreeStage();
    while (sdfDelayedResrcCheck()) {
    }
    dds3DrawFlag |= 0x2000000;
    printf("event stage shutdown !!\n");
    evtPrintDeveloperConsoleMessage("field shutdown.\n");
    return 1;
}

s32 evtCommand_FIELD_CLS_STOP(void)
{
    evtFreeAllUnit();
    while (sdfDelayedResrcCheck()) {
    }
    evtFreeStage();
    while (sdfDelayedResrcCheck()) {
    }
    dds3DrawFlag |= 0x2000000;
    printf("event stage shutdown2 !!\n");
    evtPrintDeveloperConsoleMessage("field shutdown2.\n");
    return 1;
}

s32 evtCommand_UNIT_CLS(void)
{
    evtFreeAllUnit();
    printf("unit all clear !!\n");
    evtPrintDeveloperConsoleMessage("unit all clear.\n");
    return 1;
}

s32 evtCommand_UNIT_CLS_STOP(void)
{
    evtFreeAllUnit();
    while (sdfDelayedResrcCheck()) {
    }
    printf("unit all clear2 !!\n");
    evtPrintDeveloperConsoleMessage("unit all clear2.\n");
    return 1;
}

s32 evtCommandAddEffectUnitToWorld(void) {
    dds3Basic_t *cameraOwner;

    if (scrGetIntPara(0) < 0) {
        cameraOwner = fldGetPlayerSceneState();
    } else {
        cameraOwner = evtSearchBasic(4, scrGetIntPara(0));
    }
    if (cameraOwner == NULL) {
        return 1;
    }
    dds3SetActiveCameraSceneBasic(dds3GetActiveScene(), cameraOwner);
    return 1;
}

s32 evtCommand_CAM_SEL_FOVY(void) {
    dds3Basic_t *cameraOwner;
    CameraData *cameraData;

    if (scrGetIntPara(0) < 0) {
        cameraOwner = fldGetPlayerSceneState();
    } else {
        cameraOwner = evtSearchBasic(4, scrGetIntPara(0));
        cameraData = cameraOwner->data;
        cameraData->fovUpdatePending |= 1;
    }
    if (cameraOwner == NULL) {
        return 1;
    }
    dds3SetActiveCameraSceneBasic(dds3GetActiveScene(), cameraOwner);
    return 1;
}

/* Relink the selected camera owner to its path, reporting any failure. */
s32 evtCommand_CAM_PATH_MOVE(void) {
    dds3Basic_t *owner;
    dds3Basic_t *path;

    if (scrGetIntPara(0) < 0) {
        owner = fldGetPlayerSceneState();
    } else {
        owner = evtSearchBasic(4, scrGetIntPara(0));
    }
    if (owner == NULL) {
        printf("CAM_PATH_MOVE error!\n");
        return 1;
    }
    path = evtSearchBasic(0x10, scrGetIntPara(1));
    if (path == NULL) {
        printf("CAM_PATH_MOVE error!\n");
        return 1;
    }
    if (evtPathLinkAndMake(path, owner) == 0) {
        printf("CAM_PATH_MOVE error!\n");
        return 1;
    }
    return 1;
}

s32 evtCommand_CAM_PATH_WAIT(void) {
    dds3Basic_t *cameraOwner;
    EvtPathStatus scaleStatus;

    if (scrGetIntPara(0) < 0) {
        cameraOwner = fldGetPlayerSceneState();
    } else {
        cameraOwner = evtSearchBasic(4, scrGetIntPara(0));
    }
    if (cameraOwner == NULL) {
        return 1;
    }
    scaleStatus = evtCheckPathStat(cameraOwner);
    switch (scaleStatus) {
    case EVT_PATH_STATUS_AT_ENDPOINT:
        return 1;
    case EVT_PATH_STATUS_IN_PROGRESS:
        return 0;
    case EVT_PATH_STATUS_INVALID:
        return 1;
    default:
        return 0;
    }
}

/* Create with a zero position and identity rotation; return the world value to the VM. */
s32 evtCommand_CAM_CREATE(void) {
    f32 position[4];
    f32 rotation[4];
    s32 world;

    memset(position, 0, 0x10);
    memset(rotation, 0, 0x10);
    rotation[3] = 1.0f;
    world = dds3AllocObjectId();
    dds3GetRadiusPoseSet(dds3AddCameraObjectBasicTypeFree(world, position, rotation), 1.0f);
    scrSetIntReturnValue(world);
    return 1;
}

s32 evtCommand_CAM_CLS(void)
{
    s32 effectUnitId;
    void *effectUnit;

    effectUnitId = scrGetIntPara(0);
    effectUnit = evtSearchBasic(4, effectUnitId);
    if (effectUnit == NULL) {
        return 1;
    }
    dds3DeleteSceneBasicData(effectUnit);
    return 1;
}

s32 evtCommand_CAM_POS_XYZ(void) {
    void *effectUnit;
    f32 position[4];

    memset(position, 0, sizeof(position));
    if (scrGetIntPara(0) < 0) {
        effectUnit = fldGetPlayerSceneState();
    } else {
        effectUnit = evtSearchBasic(4, scrGetIntPara(0));
    }
    if (effectUnit == NULL) {
        return 1;
    }
    position[0] = scrGetFloatPara(1);
    position[1] = scrGetFloatPara(2);
    position[2] = scrGetFloatPara(3);
    dds3SetPosPoseSet(effectUnit, (u128 *)position);
    return 1;
}

/* Convert the three degree inputs before composing and storing a quaternion. */
s32 evtCommand_CAM_ROT_XYZ(void) {
    void *effectUnit;
    f32 quaternion[4];
    f32 degreesToRadians;
    f32 pitch;

    if (scrGetIntPara(0) < 0) {
        effectUnit = fldGetPlayerSceneState();
    } else {
        effectUnit = evtSearchBasic(4, scrGetIntPara(0));
    }
    if (effectUnit == NULL) {
        return 1;
    }
    degreesToRadians = 0.017453293f;
    pitch = scrGetFloatPara(1) * degreesToRadians;
    sdfConvertEulerAnglesToQuaternionVU(pitch, scrGetFloatPara(2) * degreesToRadians, 0.0f);
    SDF_VU0_COPYVECTOR_A_TO_B();
    sdfConvertEulerAnglesToQuaternionVU(0.0f, 0.0f, scrGetFloatPara(3) * degreesToRadians);
    effMiscQuatMultiplyVU();
    SDF_VU0_STOREVECTOR(quaternion);
    dds3SetRotPoseSet(effectUnit, (u128 *)quaternion);
    return 1;
}

s32 evtCommand_CAM_ROT_Q(void) {
    void *effectUnit;
    f32 quaternion[4];

    if (scrGetIntPara(0) < 0) {
        effectUnit = fldGetPlayerSceneState();
    } else {
        effectUnit = evtSearchBasic(4, scrGetIntPara(0));
    }
    if (effectUnit == NULL) {
        return 1;
    }
    quaternion[0] = scrGetFloatPara(1);
    quaternion[1] = scrGetFloatPara(2);
    quaternion[2] = scrGetFloatPara(3);
    quaternion[3] = scrGetFloatPara(4);
    dds3SetRotPoseSet(effectUnit, (u128 *)quaternion);
    return 1;
}


extern void dds3MoverBasicMakePath(dds3Basic_t *object);
extern char D_003AC8E8[], D_003AC8F8[], D_003AC908[];

s32 evtCommand_OBJ_PATH_MOVE(void) {
    void *unit;
    dds3Basic_t *target;
    dds3ObjectBaseControlWorkData_t *data;
    dds3Basic_t *slotNode;
    dds3PathBasicControl_t *path;
    Dds3SlotResource *resource;
    s32 roomNumber;

    unit = evtSearchBasic(6, scrGetIntPara(0));
    if (unit == NULL) {
        unit = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (unit == NULL) {
            printf(D_003AC8E8, scrGetIntPara(0));
            printf(D_003AC8F8, scrGetIntPara(1));
            printf(D_003AC908);
            return 1;
        }
    }
    target = evtSearchBasic(0x10, scrGetIntPara(1));
    if (target == NULL)
        return 1;
    data = dds3MatterObjectBasicGetControlWorkData(unit);
    slotNode = data->slots[1];
    if (slotNode == NULL) {
        dds3ObjectBaseCreateMover(unit);
        slotNode = data->slots[1];
    }
    resource = slotNode->data;
    if (resource->sourceObject == NULL) {
        dds3MoverBasicSetPath(slotNode, target);
        dds3MoverBasicMakePath(slotNode);
    }
    path = dds3MoverBasicGetPathControl(slotNode);
    if (path == 0)
        return 1;
    switch (scrGetIntPara(2)) {
    case 0:
        dds3PathBasicControlAlterMode(&path->counter, 0);
        if (target->value != 0) {
            roomNumber = fldCommand_GetHojiID(target->value);
            if (roomNumber > 0)
                fldSetMapSlotValueFlag(fldGlobalBase.area, fldGlobalBase.floor + 1, roomNumber, 1);
        }
        break;
    case 1:
        dds3PathBasicControlAlterMode(&path->counter, 1);
        if (target->value != 0) {
            roomNumber = fldCommand_GetHojiID(target->value);
            if (roomNumber > 0)
                fldSetMapSlotValueFlag(fldGlobalBase.area, fldGlobalBase.floor + 1, roomNumber, 0);
        }
        break;
    }
    return 1;
}

extern char D_003AC928[]; /* "re attach...!\n" */

s32 evtCommandReattachObjectPath(void) {
    void *unit;
    dds3Basic_t *target;
    dds3ObjectBaseControlWorkData_t *data;
    dds3Basic_t *slotNode;
    dds3PathBasicControl_t *path;
    Dds3SlotResource *resource;
    s32 targetId;
    s32 mode;

    unit = evtSearchBasic(6, scrGetIntPara(0));
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
    target = evtSearchBasic(0x10, scrGetIntPara(1));
    if (target == NULL) {
        return 1;
    }
    data = dds3MatterObjectBasicGetControlWorkData(unit);
    slotNode = data->slots[1];
    if (slotNode == NULL) {
        dds3ObjectBaseCreateMover(unit);
        slotNode = data->slots[1];
    }
    resource = slotNode->data;
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
    mode = scrGetIntPara(2);
    switch (mode) {
    case 0:
        dds3PathBasicControlAlterMode(&path->counter, 0);
        break;
    case 1:
        dds3PathBasicControlAlterMode(&path->counter, 1);
        break;
    }
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC8E8);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC8F8);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC908);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC928);

s32 evtCommand_OBJ_PATH_WAIT(void) {
    dds3Basic_t *worldObject = evtSearchBasic(6, scrGetIntPara(0));
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
        return 1;
    case EVT_PATH_STATUS_IN_PROGRESS:
        return 0;
    case EVT_PATH_STATUS_INVALID:
        return 1;
    default:
        return 0;
    }
}

u32 evtCommand_MOON_START(void)
{
    evtEnableMoonCalc();
    return 1;
}

u32 evtCommand_MOON_STOP(void)
{
    evtDisableMoonCalc();
    return 1;
}

s32 evtCommand_GET_MOON(void)
{
    scrSetIntReturnValue(evtGetMirroredSolarPhase());
    return 1;
}

s32 evtCommand_GET_MOON16(void)
{
    scrSetIntReturnValue(evtGetAgeOfMoon16());
    return 1;
}

u32 evtCommand_CHG_MOON(void)
{
    evtSetAgeOfMoon(scrGetIntPara(0));
    return 1;
}

/* Create a missing camp task, then wait for its ready value.
 * No VM context completes immediately; a pending task returns zero. */
s32 evtCommand_LOAD_BE(void) {
    s32 taskId = scrGetIntPara(0);
    scrProcessWork_s *commandWork = scrGetScriptHandle();
    char *message;

    if (commandWork == NULL) {
        return 1;
    }
    if (evtSearchEventPack(taskId) == 0) {
        message = D_003AC958;
        evtPrintDeveloperConsoleMessage(message, taskId);
        sdfPrintf(message, taskId);
        dds3LinkProcess(commandWork->task, evtLoadEventPack(taskId));
        return 0;
    }
    if (evtGetEventPackStat(taskId) == EVT_PACK_LOAD_COMPLETE) {
        message = D_003AC968;
        evtPrintDeveloperConsoleMessage(message, taskId);
        sdfPrintf(message, taskId);
        return 1;
    }
    return 0;
}

/* Start the camp task only if no task with this ID exists yet.
 * A missing VM context completes without creating a task. */
s32 evtCommand_READ_BE(void) {
    s32 taskId = scrGetIntPara(0);
    scrProcessWork_s *commandWork = scrGetScriptHandle();

    if (commandWork == NULL) {
        return 1;
    }
    if (evtSearchEventPack(taskId) != 0) {
        return 1;
    }
    evtPrintDeveloperConsoleMessage(D_003AC978, taskId);
    dds3LinkProcess(commandWork->task, evtLoadEventPack(taskId));
    return 1;
}

/* Store readiness in the VM result; the command itself always completes. */
s32 evtCommand_CHK_BE(void) {
    s32 taskId = scrGetIntPara(0);
    s32 isReady;

    if (scrGetScriptHandle() != 0 && evtSearchEventPack(taskId) != 0 && evtGetEventPackStat(taskId) == EVT_PACK_LOAD_COMPLETE) {
        evtPrintDeveloperConsoleMessage(D_003AC968, taskId);
        isReady = 1;
    } else {
        isReady = 0;
    }
    scrSetIntReturnValue(isReady);
    return 1;
}

/* Destroy the requested camp task; missing context or task is a completed no-op. */
s32 evtCommand_DEL_BE(void) {
    s32 taskId = scrGetIntPara(0);

    if (scrGetScriptHandle() == 0) {
        return 1;
    }
    if (evtSearchEventPack(taskId) == 0) {
        return 1;
    }
    evtPrintDeveloperConsoleMessage(D_003AC988, taskId);
    evtKillEventPack(taskId);
    return 1;
}

extern dds3ProcessID_t * evtViewerCreateTask(s32 taskId, s32 event, s32 id);
extern void evtPolygonMovieSetFlagBits(dds3ProcessID_t *task, u32 bits);

/* Create an event/scene movie task at the owning task's priority, set bit 0,
 * and return its handle through the VM result. Missing work skips creation. */
s32 evtCommandStartPolygonMovie(void) {
    scrProcessWork_s *commandWork = scrGetScriptHandle();
    s32 eventId;
    s32 sceneId;
    dds3ProcessID_t *movieTask;

    if (commandWork == NULL) {
        return 1;
    }
    if (commandWork->task == 0) {
        printf(D_003AC998);
        return 1;
    }
    eventId = scrGetIntPara(0);
    sceneId = scrGetIntPara(1);
    movieTask = evtViewerCreateTask(commandWork->task->priority, eventId, sceneId);
    evtPrintDeveloperConsoleMessage(D_003AC9E0, scrGetIntPara(0), scrGetIntPara(1));
    dds3LinkProcess(commandWork->task, movieTask);
    evtPolygonMovieSetFlagBits(movieTask, 1);
    scrSetIntReturnValue((s32)(u32)movieTask);
    return 1;
}

extern void evtPolygonMovieClearFlagBits(dds3ProcessID_t *task, u32 bits);

/* Clear bit 0 and echo the movie ID through the VM result. */
s32 evtCommandClearPolygonMovieFlag(void)
{
    s32 movieId;

    movieId = scrGetIntPara(0);
    evtPolygonMovieClearFlagBits((dds3ProcessID_t *)(u32)movieId, 1);
    scrSetIntReturnValue(movieId);
    return 1;
}

/* Create and return an event/scene movie task without setting bit 0.
 * The owning scheduler task supplies the viewer task's priority. */
s32 evtCommand_PMV_RUN(void) {
    scrProcessWork_s *commandWork = scrGetScriptHandle();
    s32 eventId;
    s32 sceneId;
    dds3ProcessID_t *movieTask;

    if (commandWork == NULL) {
        return 1;
    }
    if (commandWork->task == 0) {
        printf(D_003AC9F8);
        return 1;
    }
    eventId = scrGetIntPara(0);
    sceneId = scrGetIntPara(1);
    movieTask = evtViewerCreateTask(commandWork->task->priority, eventId, sceneId);
    dds3LinkProcess(commandWork->task, movieTask);
    scrSetIntReturnValue((s32)(u32)movieTask);
    return 1;
}

/* Look up a world unit across the six kinds before trying the fallback slot. */
s32 evtCommand_PATH_STOP(void) {
    dds3Basic_t *slotObject;
    s32 objectKind = EVT_WORLD_OBJECT_KIND_FIRST;

    do {
        slotObject = evtSearchBasic(objectKind, scrGetIntPara(0));
        objectKind++;
    } while (objectKind < EVT_WORLD_OBJECT_KIND_LIMIT && slotObject == 0);
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
s32 evtCommand_PATH_START(void) {
    dds3Basic_t *slotObject;
    s32 objectKind = EVT_WORLD_OBJECT_KIND_FIRST;

    do {
        slotObject = evtSearchBasic(objectKind, scrGetIntPara(0));
        objectKind++;
    } while (objectKind < EVT_WORLD_OBJECT_KIND_LIMIT && slotObject == 0);
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
s32 evtCommand_PATH_RATE(void) {
    dds3Basic_t *worldNode;
    s32 roomNumber;
    const char *roomName;
    s32 objectKind = EVT_WORLD_OBJECT_KIND_FIRST;

    do {
        worldNode = evtSearchBasic(objectKind, scrGetIntPara(0));
        objectKind++;
    } while (objectKind < EVT_WORLD_OBJECT_KIND_LIMIT && worldNode == NULL);
    if (worldNode == NULL) {
        worldNode = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (worldNode == NULL) {
            return 1;
        }
    }
    evtPathRate(worldNode, scrGetFloatPara(1));
    roomName = worldNode->value;
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

/* Apply the integer state and update a positive parsed room number's flag.
 * Unlike the float command, an integer value of zero sets the room flag. */
s32 evtCommand_PATH_DIR(void) {
    dds3Basic_t *worldNode;
    s32 roomNumber;
    const char *roomName;
    s32 objectKind = EVT_WORLD_OBJECT_KIND_FIRST;

    do {
        worldNode = evtSearchBasic(objectKind, scrGetIntPara(0));
        objectKind++;
    } while (objectKind < EVT_WORLD_OBJECT_KIND_LIMIT && worldNode == NULL);
    if (worldNode == NULL) {
        worldNode = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (worldNode == NULL) {
            return 1;
        }
    }
    evtPathDir(worldNode, scrGetIntPara(1));
    roomName = worldNode->value;
    if (roomName == 0) {
        return 1;
    }
    roomNumber = fldCommand_GetHojiID(roomName);
    if (roomNumber > 0) {
        if (scrGetIntPara(1) == 0) {
            fldSetMapSlotValueFlag(fldGlobalBase.area, fldGlobalBase.floor + 1, roomNumber, 1);
        } else {
            fldSetMapSlotValueFlag(fldGlobalBase.area, fldGlobalBase.floor + 1, roomNumber, 0);
        }
    }
    return 1;
}

/* Enable the scaled-value flag on the first unit found, including the fallback. */
s32 evtCommand_PATH_LOOP_ON(void)
{
    s32 objectKind;
    dds3Basic_t *slotObject;

    objectKind = EVT_WORLD_OBJECT_KIND_FIRST;
    do {
        slotObject = evtSearchBasic(objectKind, scrGetIntPara(0));
        objectKind++;
    } while (objectKind < EVT_WORLD_OBJECT_KIND_LIMIT && slotObject == NULL);
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
s32 evtCommand_PATH_LOOP_OFF(void)
{
    s32 objectKind;
    dds3Basic_t *slotObject;

    objectKind = EVT_WORLD_OBJECT_KIND_FIRST;
    do {
        slotObject = evtSearchBasic(objectKind, scrGetIntPara(0));
        objectKind++;
    } while (objectKind < EVT_WORLD_OBJECT_KIND_LIMIT && slotObject == NULL);
    if (slotObject == NULL) {
        slotObject = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (slotObject == NULL) {
            return 1;
        }
    }
    evtPathLoop(slotObject, 0);
    return 1;
}

s32 evtCommand_LIGHT_PATH_MOVE(void) {
    void *pathOwner = evtSearchBasic(9, scrGetIntPara(0));

    if (evtPathLinkAndMake(evtSearchBasic(0x10, scrGetIntPara(1)), pathOwner) != 0) {
        return 1;
    }
    printf(D_003ACA40);
    evtPrintDeveloperConsoleMessage(D_003ACA58);
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC958);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC968);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC978);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC988);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC998);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC9E0);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC9F8);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003ACA40);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003ACA58);
