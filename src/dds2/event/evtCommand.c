#include "sdk_ps2.h"
#include "fld_area_work.h"
#include "scr_integer_return_value_api.h"
#include "common.h"
#include "dds3_admin_control_api.h"
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
    EVT_WORLD_OBJECT_KIND_LIMIT = 10,
    EVT_CAMP_TASK_READY_VALUE = 2
};

/* Script VM helpers (see script/scrCommonCommand.c for the convention). */
s32 scrGetIntPara(s32 idx);

void *evtSearchBasic(s32 objectKind, s32 worldObjectKey);

s32 dds3EffectObjectBasicLinkObject(EffWorldNode *obj, EffWorldNode *owner);
s32 dds3EffectObjectSetMG12Pos(EffWorldNode *obj, EffWorldNode *first,
                                     EffWorldNode *second, EffWorldNode *third,
                                     EffWorldNode *fourth);
s32 dds3EffectObjectBasicLinkObjectIndex(EffWorldNode *obj, EffWorldNode *owner, s32 entryId);



void dds3AdminiCallSequence(s32 command, s32 payload, s32 payloadSize, s32 mode);

#include "sdf_debug_api.h"

void scrDestroyAllNamedProcesses(void);

void evtSubmitEventRequest(s32 eventId, s32 requestMode);

void fldBgmStop(void);

void fldRequestEncounterWithFade(u32 kind, s32 eventId);

extern u32 evtPendingEventSelection;

void evtSubmitEventRequestImmediate(s32 eventId);

void fldPauseBgm(void);

void fldBgmFadeOut(void);


void fldSetSceneParam2(s32 request, s32 sequence, s32 variant, char *name);

extern s32 D_00389780[];

void fldSetSceneParam3(s32 request, s32 sequence, s32 variant, char *name);

void sdfPrintf(const char *msg, ...);

s32 evtRegistBEField(s32 eventId, s32 resourceId);

void evtPrintDeveloperConsoleMessage(const char *fmt, ...);

void evtFreeAllUnit(void);

s32 sdfDelayedResrcCheck(void);

void evtFreeStage(void);

extern u32 kwlnDrawControlFlags;


void dds3RefreshStoredVec3(EffWorldNode *object);


void evtPathLoop(void *unit, s32 enabled);

void evtEffectLoopControl(void *unit, s32 flag);

void dds3EffectObjectBasicStatOn(void *unit, s32 flag);




extern char D_00421ED8[]; /* "BE ok! (%d)\n" */

extern char D_00421F08[];




extern char D_00421F68[];

void evtPathStop(void *unit);

void evtPathStart(void *unit);

void evtPathRate(void *unit, f32 value);

s32 fldCommand_GetHojiID(char *name);

s32 fldSetMapSlotValueFlag(s32 worldKey, s32 roomGroup, s32 roomNumber, s32 enabled);



void evtPathDir(void *worldNode, u32 direction);

s32 evtPathLinkAndMake(void *target, void *path);

extern char D_00421FB0[]; /* "LIGHT_PATH_MOVE error!\n" */

extern char D_00421FC8[]; /* "error: LIGHT_PATH_MOVE.\n" */

/* Copy two source points into the primary effect object. */
s32 evtCommand_EFFMG1_POS(void) {
    EffWorldNode *primaryEffect;
    EffWorldNode *sourcePoint0;
    EffWorldNode *sourcePoint1;

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

/* Apply four effect parameters after resolving every input object. */
s32 evtCommand_EFFMG2_POS(void) {
    EffWorldNode *primaryEffect;
    EffWorldNode *sourcePoint0;
    EffWorldNode *sourcePoint1;
    EffWorldNode *sourcePoint2;
    EffWorldNode *sourcePoint3;

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
    void *effectObject;

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
    EffWorldNode *effectPath = evtSearchBasic(7, scrGetIntPara(0));
    void *owner;
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
    EffWorldNode *effectPath = evtSearchBasic(7, scrGetIntPara(0));
    void *owner;
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
    void *effectObject;

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
    void *effectObject;

    effectId = scrGetIntPara(0);
    effectObject = evtSearchBasic(7, effectId);
    if (effectObject != NULL) {
        evtEffectLoopControl(effectObject, 0);
    }
    return 1;
}

s32 evtCommand_EFFECT_PATH_MOVE(void) {
    void *effect;
    void *path;

    effect = evtSearchBasic(7, scrGetIntPara(0));
    if (effect == NULL) {
        evtPrintDeveloperConsoleMessage("EFFECT_PATH_MOVE not found eff!\n");
        return 1;
    }
    path = evtSearchBasic(0x10, scrGetIntPara(1));
    if (path == NULL) {
        evtPrintDeveloperConsoleMessage("EFFECT_PATH_MOVE not found path!\n", effect);
        return 1;
    }
    evtPathLinkAndMake(path, effect);
    return 1;
}

extern s32 evtCheckPathStat(void *object);

s32 evtCommand_EFFECT_PATH_WAIT(void) {
    s32 effectId = scrGetIntPara(0);
    void *effectObject = evtSearchBasic(7, effectId);
    s32 scaleStatus;

    if (effectObject == NULL) {
        evtPrintDeveloperConsoleMessage("EFFECT_PATH_WAIT not found eff!\n");
        return 1;
    }
    scaleStatus = evtCheckPathStat(effectObject);
    switch (scaleStatus) {
    case 0:
        return 1;
    case 1:
        return 0;
    case -1:
        return 1;
    default:
        return 0;
    }
}



/* Return the named world-chain node's key to the script VM. */
s32 evtCommand_SEARCH_ID(void) {
    EffWorldNode *worldNode;

    worldNode = dds3SceneBasicSearchName((EffWorldNode *)dds3GetControlScene(),
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

u32 evtCommand_RTRN_TITLE(void) {
    dds3AdminiCallSequence(2, 0, 0, 0);
    return 1;
}

u32 evtCommand_CALL_STAFF(void) {
    dds3AdminiCallSequence(0xc, 0, 0, 0);
    return 1;
}

u32 evtCommand_CALL_ENDSAVE(void) {
    dds3AdminiCallSequence(4, 0, 0, 0);
    return 1;
}

/* Forward two VM integers as the admin request's two-word payload. */
s32 evtCommand_CALL_SAVE(void)
{
    s32 payload[2];

    payload[0] = scrGetIntPara(0);
    payload[1] = scrGetIntPara(1);
    dds3AdminiCallSequence(0x15, (s32)payload, EVT_PAIRED_REQUEST_SIZE, 0);
    scrSetIntReturnValue(0);
    return 1;
}

extern void fldSetDeferredFieldCommand(s32 a, s32 b);

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
    dds3AdminiCallSequence(6, (s32)&eventId, EVT_SCALAR_REQUEST_SIZE, 0);
    scrDestroyAllNamedProcesses();
    return 1;
}

u32 evtCommandSignalAdminAtTimerZero(void) {
    s32 timer;

    timer = scrGetCommandTimer();
    if (timer == 0) {
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
    dds3AdminiCallSequence(0xe, (s32)payload, EVT_PAIRED_REQUEST_SIZE, requestMode > 0);
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
    dds3AdminiCallSequence(0xe, (s32)payload, EVT_PAIRED_REQUEST_SIZE, 1);
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
    u8 requestBuffer[EVT_FIELD_SEQUENCE_REQUEST_SIZE];

    fldBgmFadeOut();
    stage = scrGetIntPara(0);
    kind = scrGetIntPara(1);
    sequenceName = scrGetStrPara(2);
    fldSetSceneParam2((s32)requestBuffer, stage, kind, sequenceName);
    dds3AdminiCallSequence(5, (s32)requestBuffer, EVT_FIELD_SEQUENCE_REQUEST_SIZE, 0);
    scrDestroyAllNamedProcesses();
    return 1;
}

/* Use the current group's stage with the VM's kind and sequence-name inputs. */
s32 evtCommand_CALL_FIELD2(void)
{
    s32 kind;
    char *sequenceName;
    u8 requestBuffer[EVT_FIELD_SEQUENCE_REQUEST_SIZE];

    kind = scrGetIntPara(0);
    sequenceName = scrGetStrPara(1);
    fldSetSceneParam2((s32)requestBuffer, D_00389780[0], kind, sequenceName);
    dds3AdminiCallSequence(5, (s32)requestBuffer, EVT_FIELD_SEQUENCE_REQUEST_SIZE, 0);
    scrDestroyAllNamedProcesses();
    return 1;
}

/* Forward the first VM integer as an opaque four-byte admin payload. */
s32 evtCommandSubmitScalarAdminRequest(void)
{
    s32 payloadValue;

    payloadValue = scrGetIntPara(0);
    dds3AdminiCallSequence(0x1c, (s32)&payloadValue, EVT_SCALAR_REQUEST_SIZE, 0);
    return 1;
}

/* Build the current group's field request using the alternate initializer. */
s32 evtCommand_CALL_FIELD3(void)
{
    s32 kind;
    char *sequenceName;
    u8 requestBuffer[EVT_FIELD_SEQUENCE_REQUEST_SIZE];

    kind = scrGetIntPara(0);
    sequenceName = scrGetStrPara(1);
    fldSetSceneParam3((s32)requestBuffer, D_00389780[0], kind, sequenceName);
    dds3AdminiCallSequence(5, (s32)requestBuffer, EVT_FIELD_SEQUENCE_REQUEST_SIZE, 0);
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

extern s32 evtStageAnimeControl(s32 modeSelector, s32 modeValue);

u32 evtCommand_FIELD_ANIME_START(void) {
    s32 baseMode;

    baseMode = scrGetIntPara(0);
    evtStageAnimeControl(0, baseMode);
    return 1;
}

u32 evtCommand_FIELD_ANIME_STOP(void) {
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
    kwlnDrawControlFlags |= 0x2000000;
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
    kwlnDrawControlFlags |= 0x2000000;
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
    EffWorldNode *cameraOwner;

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
    EffWorldNode *cameraOwner;
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
    void *owner;
    void *path;

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
    void *cameraOwner;
    s32 scaleStatus;

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
    case 0:
        return 1;
    case 1:
        return 0;
    case -1:
        return 1;
    default:
        return 0;
    }
}

extern EffWorldNode *dds3AddCameraObjectBasicTypeFree(s32 world, void *pos, void *rot);

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
    VU0_MOVE_VF(vf11, vf10);
    sdfConvertEulerAnglesToQuaternionVU(0.0f, 0.0f, scrGetFloatPara(3) * degreesToRadians);
    effMiscQuatMultiplyVU();
    VU0_STORE_VF(vf10, quaternion);
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


extern void dds3MoverBasicMakePath(EffWorldNode *object);
extern char D_00421E58[], D_00421E68[], D_00421E78[];

s32 evtCommand_OBJ_PATH_MOVE(void) {
    void *unit;
    EffWorldNode *target;
    ObjBase *objectData;
    EffWorldNode *slotNode;
    Dds3PathCurveWork *path;
    Dds3SlotResource *resource;
    s32 roomNumber;

    unit = evtSearchBasic(6, scrGetIntPara(0));
    if (unit == NULL) {
        unit = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (unit == NULL) {
            printf(D_00421E58, scrGetIntPara(0));
            printf(D_00421E68, scrGetIntPara(1));
            printf(D_00421E78);
            return 1;
        }
    }
    target = evtSearchBasic(0x10, scrGetIntPara(1));
    if (target == NULL)
        return 1;
    objectData = dds3MatterObjectBasicGetControlWorkData(unit);
    slotNode = objectData->slots[1];
    if (slotNode == NULL) {
        dds3ObjectBaseCreateMover(unit);
        slotNode = objectData->slots[1];
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
            roomNumber = fldCommand_GetHojiID((char *)target->value);
            if (roomNumber > 0)
                fldSetMapSlotValueFlag(fldAreaState.area, fldAreaState.floor + 1, roomNumber, 1);
        }
        break;
    case 1:
        dds3PathBasicControlAlterMode(&path->counter, 1);
        if (target->value != 0) {
            roomNumber = fldCommand_GetHojiID((char *)target->value);
            if (roomNumber > 0)
                fldSetMapSlotValueFlag(fldAreaState.area, fldAreaState.floor + 1, roomNumber, 0);
        }
        break;
    }
    return 1;
}

extern char D_00421E98[]; /* "re attach...!\n" */

s32 evtCommandReattachObjectPath(void) {
    void *unit;
    EffWorldNode *target;
    ObjBase *objectData;
    EffWorldNode *slotNode;
    Dds3PathCurveWork *path;
    Dds3SlotResource *resource;
    s32 targetId;
    s32 mode;

    unit = evtSearchBasic(6, scrGetIntPara(0));
    if (unit == NULL) {
        unit = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (unit == NULL) {
            printf(D_00421E58, scrGetIntPara(0));
            targetId = scrGetIntPara(1);
            printf(D_00421E68, targetId);
            printf(D_00421E78);
            return 1;
        }
    }
    target = evtSearchBasic(0x10, scrGetIntPara(1));
    if (target == NULL) {
        return 1;
    }
    objectData = dds3MatterObjectBasicGetControlWorkData(unit);
    slotNode = objectData->slots[1];
    if (slotNode == NULL) {
        dds3ObjectBaseCreateMover(unit);
        slotNode = objectData->slots[1];
    }
    resource = slotNode->data;
    if (resource->sourceObject == NULL) {
        dds3MoverBasicSetPath(slotNode, target);
        dds3MoverBasicMakePath(slotNode);
    } else {
        dds3RefreshStoredVec3(unit);
        dds3MoverBasicSetPath(slotNode, target);
        dds3MoverBasicMakePath(slotNode);
        printf(D_00421E98);
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

INCLUDE_RODATA(const s32, "event/evtCommand", D_00421E58);

INCLUDE_RODATA(const s32, "event/evtCommand", D_00421E68);

INCLUDE_RODATA(const s32, "event/evtCommand", D_00421E78);

INCLUDE_RODATA(const s32, "event/evtCommand", D_00421E98);

s32 evtCommand_OBJ_PATH_WAIT(void) {
    s32 objectId = scrGetIntPara(0);
    void *worldObject = evtSearchBasic(6, objectId);
    s32 scaleStatus;

    if (worldObject == NULL) {
        objectId = scrGetIntPara(0);
        worldObject = fldGetMatter_From_ModelRangeID((u32)objectId);
        if (worldObject == NULL) {
            printf(D_00421E58, scrGetIntPara(0));
            printf(D_00421E68, scrGetIntPara(1));
            printf("e OBJ_PATH_WAIT : obj==NULL\n");
            return 1;
        }
    }
    scaleStatus = evtCheckPathStat(worldObject);
    switch (scaleStatus) {
    case 0:
        return 1;
    case 1:
        return 0;
    case -1:
        return 1;
    default:
        return 0;
    }
}

extern void evtEnableMoonCalc(void);

u32 evtCommand_MOON_START(void) {
    evtEnableMoonCalc();
    return 1;
}

extern void evtDisableMoonCalc(void);

u32 evtCommand_MOON_STOP(void) {
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

extern void evtSetAgeOfMoon(s32 phase);

u32 evtCommand_CHG_MOON(void) {
    s32 phase;

    phase = scrGetIntPara(0);
    evtSetAgeOfMoon(phase);
    return 1;
}

/* Create a missing camp task, then wait for its ready value.
 * No VM context completes immediately; a pending task returns zero. */
s32 evtCommand_LOAD_BE(void) {
    s32 taskId = scrGetIntPara(0);
    ScrData *commandWork = scrGetScriptHandle();
    char *message;

    if (commandWork == NULL) {
        return 1;
    }
    if (evtSearchEventPack(taskId) == 0) {
        message = "load BE (%d)..\n";
        evtPrintDeveloperConsoleMessage(message, taskId);
        sdfPrintf(message, taskId);
        dds3LinkProcess(commandWork->task, evtLoadEventPack(taskId));
        return 0;
    }
    if (evtGetEventPackStat(taskId) == EVT_CAMP_TASK_READY_VALUE) {
        message = D_00421ED8;
        evtPrintDeveloperConsoleMessage(message, taskId);
        sdfPrintf(message, taskId);
        return 1;
    }
    return 0;
}

/* Start the camp task only if no task with this ID exists yet.
 * A missing VM context completes without creating a task. */
INCLUDE_RODATA(const s32, "event/evtCommand", D_00421ED8);

s32 evtCommand_READ_BE(void) {
    s32 taskId = scrGetIntPara(0);
    ScrData *commandWork = scrGetScriptHandle();

    if (commandWork == NULL) {
        return 1;
    }
    if (evtSearchEventPack(taskId) != 0) {
        return 1;
    }
    evtPrintDeveloperConsoleMessage("read BE (%d)..\n", taskId);
    dds3LinkProcess(commandWork->task, evtLoadEventPack(taskId));
    return 1;
}

/* Store readiness in the VM result; the command itself always completes. */
s32 evtCommand_CHK_BE(void) {
    s32 taskId = scrGetIntPara(0);
    s32 isReady;

    if (scrGetScriptHandle() != 0 && evtSearchEventPack(taskId) != 0 && evtGetEventPackStat(taskId) == EVT_CAMP_TASK_READY_VALUE) {
        evtPrintDeveloperConsoleMessage(D_00421ED8, taskId);
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
    evtPrintDeveloperConsoleMessage("del BE (%d)..\n", taskId);
    evtKillEventPack(taskId);
    return 1;
}

/* Create an event/scene movie task using the active camp resource, set bit 0,
 * and return its handle through the VM result. Missing work skips creation. */
extern KwlnTask * evtViewerCreateTask(s32 taskId, s32 event, s32 id);
extern void evtPolygonMovieSetFlagBits(KwlnTask *task, u32 bits);

INCLUDE_RODATA(const s32, "event/evtCommand", D_00421F08);

s32 evtCommandStartPolygonMovie(void) {
    ScrData *commandWork = scrGetScriptHandle();
    s32 eventId;
    s32 sceneId;
    KwlnTask *movieTask;

    if (commandWork == NULL) {
        return 1;
    }
    if (commandWork->task == 0) {
        printf(D_00421F08);
        return 1;
    }
    eventId = scrGetIntPara(0);
    sceneId = scrGetIntPara(1);
    movieTask = evtViewerCreateTask(commandWork->task->priority, eventId, sceneId);
    evtPrintDeveloperConsoleMessage("load PMV (%03d_%03d)..\n", scrGetIntPara(0), scrGetIntPara(1));
    dds3LinkProcess(commandWork->task, movieTask);
    evtPolygonMovieSetFlagBits(movieTask, 1);
    scrSetIntReturnValue((s32)(u32)movieTask);
    return 1;
}

extern void evtPolygonMovieClearFlagBits(KwlnTask *task, u32 bits);

/* Clear bit 0 and echo the movie ID through the VM result. */
s32 evtCommandClearPolygonMovieFlag(void)
{
    s32 movieId;

    movieId = scrGetIntPara(0);
    evtPolygonMovieClearFlagBits((KwlnTask *)(u32)movieId, 1);
    scrSetIntReturnValue(movieId);
    return 1;
}

/* Create and return an event/scene movie task without setting bit 0.
 * The owning scheduler task supplies the viewer task's priority. */
s32 evtCommand_PMV_RUN(void) {
    ScrData *commandWork = scrGetScriptHandle();
    s32 eventId;
    s32 sceneId;
    KwlnTask *movieTask;

    if (commandWork == NULL) {
        return 1;
    }
    if (commandWork->task == 0) {
        printf(D_00421F68);
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
    void *slotObject;
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

/* Clear the first matching unit's status flag, using the same fallback search. */
s32 evtCommand_PATH_START(void) {
    void *slotObject;
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
    EffWorldNode *worldNode;
    s32 roomNumber;
    char *roomName;
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
    roomName = (char *)worldNode->value;
    if (roomName == 0) {
        return 1;
    }
    roomNumber = fldCommand_GetHojiID(roomName);
    if (roomNumber > 0) {
        if (scrGetFloatPara(1) > 0.5f) {
            fldSetMapSlotValueFlag(fldAreaState.area, fldAreaState.floor + 1, roomNumber, 1);
        } else {
            fldSetMapSlotValueFlag(fldAreaState.area, fldAreaState.floor + 1, roomNumber, 0);
        }
    }
    return 1;
}

/* Apply the integer state and update a positive parsed room number's flag.
 * Unlike the float command, an integer value of zero sets the room flag. */
s32 evtCommand_PATH_DIR(void) {
    EffWorldNode *worldNode;
    s32 roomNumber;
    char *roomName;
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
    roomName = (char *)worldNode->value;
    if (roomName == 0) {
        return 1;
    }
    roomNumber = fldCommand_GetHojiID(roomName);
    if (roomNumber > 0) {
        if (scrGetIntPara(1) == 0) {
            fldSetMapSlotValueFlag(fldAreaState.area, fldAreaState.floor + 1, roomNumber, 1);
        } else {
            fldSetMapSlotValueFlag(fldAreaState.area, fldAreaState.floor + 1, roomNumber, 0);
        }
    }
    return 1;
}

/* Enable the scaled-value flag on the first unit found, including the fallback. */
s32 evtCommand_PATH_LOOP_ON(void)
{
    s32 objectKind;
    void *slotObject;

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
    void *slotObject;

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
    printf(D_00421FB0);
    evtPrintDeveloperConsoleMessage(D_00421FC8);
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtCommand", D_00421F68);

INCLUDE_RODATA(const s32, "event/evtCommand", D_00421FB0);

INCLUDE_RODATA(const s32, "event/evtCommand", D_00421FC8);

