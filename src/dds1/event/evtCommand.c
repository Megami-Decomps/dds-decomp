#include "common.h"
#include "dds3_path.h"
#include "dds3obj.h"
#include "eff_dependency.h"
#include "eff_object.h"
#include "pcp_vu0.h"
#include "kwln.h"
#include "scr.h"

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
s32 scrReadIntParameter(s32 idx);

void scrSetIntegerReturnValue(s32 value);

s32 scrGetCommandTimer(void);

void *evtFindWorldObjectByIdAndKind(s32 type, s32 id);

struct EffectObj;
s32 effObjBindValidatedOwner(struct EffectObj *obj, struct EffectObj *owner);
s32 effObjBindOwnerBillEntry(struct EffectObj *obj, struct EffectObj *owner, s32 entryId);

void evtDispatchSupportedNodeOnClear(void *unit, s32 flag);

void effObjSetFlags(void *unit, s32 flag);

void evtApplyIndexValueToWorldNodes(s32 which, s32 value);

void dds3AdminSubmitModeRequest(s32 command, s32 payload, s32 payloadSize, s32 mode);

void dds3AdminSetControlFlag(void);

void evtSubmitEventRequest(s32 eventId, s32 mode);

void evtSubmitEventRequestImmediate(s32 eventId);

void evtEnableSolarPhaseAdvance(void);

void evtDisableSolarPhaseAdvance(void);

s32 evtGetMirroredSolarPhase(void);

s32 evtGetSolarPhase(void);

void evtSetSolarPhase(s32 phase);

void evtPolygonMovieClearFlagBits(s32 movieId, u32 bits);

void evtPrintDeveloperConsoleMessage(const char *fmt, ...);

/* libc printf returns the signed vfprintf character count. */
s32 func_003003F0(const char *fmt, ...);

void scrDestroyAllNamedProcesses(void);

void evtDrainSecondaryWorldNodes(void);


void dds3RemoveWorldObjectNode(void *unit);

void dds3RefreshStoredVec3(void *);

void fldStopCurrentBgm(void);

void fldPlayCurrentBgmSound(void);

void fldReleaseCurrentBgm(void);

char *scrReadStringParameter(s32 idx);

void fldInitializeSequenceAndResetFlags(s32 request, s32 sequence, s32 variant, char *name);

void fldInitializeAlternateSequence(s32 request, s32 sequence, s32 variant, char *name);

s32 sdfCheckPendingWorkWithInterrupts(void);

void *dds3GetWorldSecondaryObject(void);

s32 dds3GetWorldObjectValue(EffWorldNode *world);

void evtCreateWorldObjectForKey(s32 highPart, s32 lowPart);

void evtDestroySecondaryWorldNode(void);

s32 evtStageRelinkOwnedNodeResource(void *target, void *path);

f32 bfWaitReadArgFloat(s32 idx);
extern void func_002E7F20(f32, f32, f32);
extern void effMiscQuatMultiplyVU(void);

void evtScaleSlotByClampedMultiplier(void *unit, f32 value);

s32 fldParseRoomNumberFromName(char *name);

s32 fldSetMapSlotValueFlag(s32 worldKey, s32 roomGroup, s32 roomNumber, s32 enabled);

extern u32 fldAreaState[];

extern u32 kwlnDrawControlFlags;

s32 func_00126200(s32 id);

void evtSetWorldSlotStatusFlag(void *unit);

void evtClearWorldSlotStatusFlag(void *unit);

void evtSetWorldSlotValue(void *unit, u32 value);

void evtToggleWorldSlotScaledValueFlag(void *unit, s32 enabled);

u32 fldGetPlayerSceneState(void);

EffWorldNode *dds3SetWorldCameraObject(EffWorldNode *world, EffWorldNode *unit);


s32 evtFindTaskById(s32 id);

s32 evtGetTaskValueWord(s32 id);

extern char D_003AC968[]; /* "BE ok! (%d)\n" */

extern char D_003AC978[];

extern char D_003AC988[]; /* "del BE (%d)..\n" */

extern char D_003ACA40[]; /* "LIGHT_PATH_MOVE error!\n" */

extern char D_003ACA58[]; /* "error: LIGHT_PATH_MOVE.\n" */

void sdfPrintFormattedDevMessage(const char *msg, ...);

s32 evtTryCreateWorldObjectFromPackResourceSet(s32 eventId, s32 resourceId);

extern s32 D_0032E3C0[];

void fldRequestEncounterWithFade(u32 kind, s32 eventId);

extern u32 evtPendingEventSelection;




/* The room-name lookup and status flag share this world-unit layout. */
typedef struct EvtWorldUnitInner {
    u8 pad00[0x88];
    u32 statusFlags; /* 0x88 */
} EvtWorldUnitInner;

typedef struct EvtWorldUnit {
    u8 pad00[8];
    char *roomName; /* 0x08 */
    u8 pad0C[0xC];
    EvtWorldUnitInner *inner; /* 0x18 */
} EvtWorldUnit;

extern EffWorldNode *dds3FindObjectChainNodeByName(EffWorldNode *world, const u8 *id);

extern void fldSetDeferredFieldCommand(s32 a, s32 b);

extern void *memset(void *dst, s32 value, u32 size);

extern u32 dds3AdvanceWorldCounter(void);

extern EffWorldNode *dds3CreateCameraObject(s32 world, void *pos, void *rot);

extern void effObjSetInnerFloat(EffWorldNode *obj, f32 value);

INCLUDE_ASM(const s32, "event/evtCommand", func_002260C0);

extern char D_003AC700[];

extern char D_003AC728[];

extern char D_003AC9F8[];

extern char D_003AC998[];

extern char D_003AC9E0[];

extern char D_003AC958[];

INCLUDE_ASM(const s32, "event/evtCommand", func_00226180);

s32 evtCommandEnablePathUnit(void)
{
    s32 id;
    void *unit;

    id = scrReadIntParameter(0);
    unit = evtFindWorldObjectByIdAndKind(7, id);
    if (unit != NULL) {
        evtDispatchSupportedNodeOnClear(unit, 1);
        effObjSetFlags(unit, 1);
    }
    return 1;
}

/* Bind the selected effect path to the first owner found in kinds 4..9.
 * Missing paths or owners complete the command without binding anything. */
s32 evtCommandAssignEffectObjectOwner(void) {
    struct EffectObj *effectPath = evtFindWorldObjectByIdAndKind(7, scrReadIntParameter(0));
    void *owner;
    s32 objectKind;

    if (effectPath == NULL) {
        return 1;
    }
    objectKind = EVT_WORLD_OBJECT_KIND_FIRST;
    do {
        owner = evtFindWorldObjectByIdAndKind(objectKind, scrReadIntParameter(1));
        objectKind++;
    } while (objectKind < EVT_WORLD_OBJECT_KIND_LIMIT && owner == NULL);
    if (owner == NULL) {
        return 1;
    }
    effObjBindValidatedOwner(effectPath, owner);
    return 1;
}

/* Search the same owner kinds, then bind with the VM's third entry argument. */
s32 evtCommandAssignEffectObjectOwnerWithEntry(void) {
    struct EffectObj *effectPath = evtFindWorldObjectByIdAndKind(7, scrReadIntParameter(0));
    void *owner;
    s32 objectKind;

    if (effectPath == NULL) {
        return 1;
    }
    objectKind = EVT_WORLD_OBJECT_KIND_FIRST;
    do {
        owner = evtFindWorldObjectByIdAndKind(objectKind, scrReadIntParameter(1));
        objectKind++;
    } while (objectKind < EVT_WORLD_OBJECT_KIND_LIMIT && owner == NULL);
    if (owner == NULL) {
        return 1;
    }
    effObjBindOwnerBillEntry(effectPath, owner, scrReadIntParameter(2));
    return 1;
}

s32 evtCmdResetWorldResourceState(void)
{
    s32 id;
    void *unit;

    id = scrReadIntParameter(0);
    unit = evtFindWorldObjectByIdAndKind(7, id);
    if (unit == NULL) {
        return 1;
    }
    dds3ResetWorldResourceState(unit);
    return 1;
}

s32 evtCommandClearSelectedEffectNode(void)
{
    s32 id;
    void *unit;

    id = scrReadIntParameter(0);
    unit = evtFindWorldObjectByIdAndKind(7, id);
    if (unit != NULL) {
        evtDispatchSupportedNodeOnClear(unit, 0);
    }
    return 1;
}

s32 evtCommandAttachLightToUnitPath(void) {
    void *effect;
    void *path;

    effect = evtFindWorldObjectByIdAndKind(7, scrReadIntParameter(0));
    if (effect == NULL) {
        evtPrintDeveloperConsoleMessage(D_003AC700);
        return 1;
    }
    path = evtFindWorldObjectByIdAndKind(0x10, scrReadIntParameter(1));
    if (path == NULL) {
        evtPrintDeveloperConsoleMessage(D_003AC728);
        return 1;
    }
    evtStageRelinkOwnedNodeResource(path, effect);
    return 1;
}

extern char D_003AC750[];

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC700);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC728);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226540);

/* Return the named world-chain node's key to the script VM. */
s32 evtCommandReadSecondaryWorldIdValue(void) {
    EffWorldNode *node;

    node = dds3FindObjectChainNodeByName((EffWorldNode *)dds3GetWorldSecondaryObject(),
                                         (const u8 *)scrReadStringParameter(0));
    if (node == NULL) {
        func_003003F0("ID : id not found!! <%s>\n", scrReadStringParameter(0));
        evtPrintDeveloperConsoleMessage("WARNING: ID not found! <%s>\n", scrReadStringParameter(0));
        scrSetIntegerReturnValue(0);
    } else {
        scrSetIntegerReturnValue(node->key);
    }
    return 1;
}

s32 func_00226650(void)
{
    dds3AdminSubmitModeRequest(2, 0, 0, 0);
    return 1;
}

s32 func_00226680(void)
{
    dds3AdminSubmitModeRequest(0xc, 0, 0, 0);
    return 1;
}

s32 func_002266B0(void)
{
    dds3AdminSubmitModeRequest(4, 0, 0, 0);
    return 1;
}

/* Forward two VM integers as the admin request's two-word payload. */
s32 evtCommandSubmitPairedAdminRequest(void)
{
    s32 payload[2];

    payload[0] = scrReadIntParameter(0);
    payload[1] = scrReadIntParameter(1);
    dds3AdminSubmitModeRequest(0x15, (s32)payload, EVT_PAIRED_REQUEST_SIZE, 0);
    scrSetIntegerReturnValue(0);
    return 1;
}

/* Log all three arguments, then re-read command and parameter for dispatch. */
s32 evtCommandDeferFieldTransition(void) {
    s32 loggedArgument = scrReadIntParameter(0);
    s32 loggedCommand = scrReadIntParameter(1);
    s32 loggedParameter = scrReadIntParameter(2);
    s32 command;

    func_003003F0("CALL_NEXT(%d,%d,%d)\n", loggedArgument, loggedCommand, loggedParameter);
    command = scrReadIntParameter(1);
    fldSetDeferredFieldCommand(command, scrReadIntParameter(2));
    return 1;
}

/* The event ID becomes a four-byte dispatch payload. */
s32 evtCommandCallEvent(void)
{
    s32 eventId;

    eventId = scrReadIntParameter(0);
    func_003003F0("call_event:%d\n", eventId);
    dds3AdminSubmitModeRequest(6, (s32)&eventId, EVT_SCALAR_REQUEST_SIZE, 0);
    scrDestroyAllNamedProcesses();
    return 1;
}

s32 evtCommandSignalAdminAtTimerZero(void)
{
    if (scrGetCommandTimer() == 0) {
        dds3AdminSetControlFlag();
    }
    return 0;
}

/* Queue an event request after preparing BGM/encounter state, then stop scripts.
 * The mode is retained separately; only its positivity controls dispatch mode. */
void evtSubmitEventRequest(s32 eventId, s32 requestMode)
{
    s32 payload[2];

    fldPlayCurrentBgmSound();
    fldRequestEncounterWithFade(2, eventId);
    evtPendingEventSelection = requestMode;
    payload[0] = 0;
    payload[1] = eventId;
    dds3AdminSubmitModeRequest(0xe, (s32)payload, EVT_PAIRED_REQUEST_SIZE, requestMode > 0);
    scrDestroyAllNamedProcesses();
}

s32 evtCommandSubmitEvent(void)
{
    s32 eventId;

    eventId = scrReadIntParameter(0);
    evtSubmitEventRequest(eventId, 0);
    return 1;
}

/* Stop BGM and submit the event payload with immediate dispatch enabled. */
void evtSubmitEventRequestImmediate(s32 eventId)
{
    s32 payload[2];

    fldStopCurrentBgm();
    fldRequestEncounterWithFade(2, eventId);
    evtPendingEventSelection = 0;
    payload[0] = 0;
    payload[1] = eventId;
    dds3AdminSubmitModeRequest(0xe, (s32)payload, EVT_PAIRED_REQUEST_SIZE, 1);
    scrDestroyAllNamedProcesses();
}

s32 evtCommandSubmitEventImmediate(void)
{
    s32 eventId;

    eventId = scrReadIntParameter(0);
    evtSubmitEventRequestImmediate(eventId);
    return 1;
}

s32 evtCommandSubmitEventWithMode(void)
{
    s32 eventId;
    s32 requestMode;

    eventId = scrReadIntParameter(0);
    requestMode = scrReadIntParameter(1);
    evtSubmitEventRequest(eventId, requestMode);
    return 1;
}

/* A field-sequence request is a fixed 0xa0-byte VM message. */
s32 evtCommandRequestFieldSequence(void)
{
    s32 stage;
    s32 kind;
    char *sequenceName;
    u8 requestBuffer[EVT_FIELD_SEQUENCE_REQUEST_SIZE];

    fldReleaseCurrentBgm();
    stage = scrReadIntParameter(0);
    kind = scrReadIntParameter(1);
    sequenceName = scrReadStringParameter(2);
    fldInitializeSequenceAndResetFlags((s32)requestBuffer, stage, kind, sequenceName);
    dds3AdminSubmitModeRequest(5, (s32)requestBuffer, EVT_FIELD_SEQUENCE_REQUEST_SIZE, 0);
    scrDestroyAllNamedProcesses();
    return 1;
}

/* Use the current group's stage with the VM's kind and sequence-name inputs. */
s32 evtCommandRequestCurrentGroupSequence(void)
{
    s32 kind;
    char *sequenceName;
    u8 requestBuffer[EVT_FIELD_SEQUENCE_REQUEST_SIZE];

    kind = scrReadIntParameter(0);
    sequenceName = scrReadStringParameter(1);
    fldInitializeSequenceAndResetFlags((s32)requestBuffer, D_0032E3C0[0], kind, sequenceName);
    dds3AdminSubmitModeRequest(5, (s32)requestBuffer, EVT_FIELD_SEQUENCE_REQUEST_SIZE, 0);
    scrDestroyAllNamedProcesses();
    return 1;
}

/* Forward the first VM integer as an opaque four-byte admin payload. */
s32 func_00226A70(void)
{
    s32 payloadValue;

    payloadValue = scrReadIntParameter(0);
    dds3AdminSubmitModeRequest(0x1c, (s32)&payloadValue, EVT_SCALAR_REQUEST_SIZE, 0);
    return 1;
}

/* Build the current group's field request using the alternate initializer. */
s32 evtCommandRequestAlternateFieldSequence(void)
{
    s32 kind;
    char *sequenceName;
    u8 requestBuffer[EVT_FIELD_SEQUENCE_REQUEST_SIZE];

    kind = scrReadIntParameter(0);
    sequenceName = scrReadStringParameter(1);
    fldInitializeAlternateSequence((s32)requestBuffer, D_0032E3C0[0], kind, sequenceName);
    dds3AdminSubmitModeRequest(5, (s32)requestBuffer, EVT_FIELD_SEQUENCE_REQUEST_SIZE, 0);
    scrDestroyAllNamedProcesses();
    return 1;
}

s32 evtCommandDispatchFieldBE(void)
{
    s32 firstArg;
    s32 secondArg;

    sdfPrintFormattedDevMessage("FIELD_BE start\n");
    firstArg = scrReadIntParameter(0);
    secondArg = scrReadIntParameter(1);
    evtTryCreateWorldObjectFromPackResourceSet(firstArg, secondArg);
    sdfPrintFormattedDevMessage("FIELD_BE end\n");
    return 1;
}

s32 evtCommandHandleChangedSecondaryWorldKey(void)
{
    s32 highPart;
    s32 lowPart;
    s32 targetKey;

    highPart = scrReadIntParameter(0);
    lowPart = scrReadIntParameter(1);
    targetKey = (highPart << 16) + lowPart;
    if (dds3GetWorldObjectValue(dds3GetWorldSecondaryObject()) != targetKey) {
        highPart = scrReadIntParameter(0);
        lowPart = scrReadIntParameter(1);
        evtCreateWorldObjectForKey(highPart, lowPart);
    }
    return 1;
}

s32 evtCommandSetWorldNodeBaseMode(void)
{
    s32 value;

    value = scrReadIntParameter(0);
    evtApplyIndexValueToWorldNodes(0, value);
    return 1;
}

s32 evtCommandSetWorldNodeUpperMode(void)
{
    s32 value;

    value = scrReadIntParameter(0);
    evtApplyIndexValueToWorldNodes(1, value);
    return 1;
}

s32 evtCommandShutdownStage(void)
{
    evtDrainSecondaryWorldNodes();
    while (sdfCheckPendingWorkWithInterrupts()) {
    }
    evtDestroySecondaryWorldNode();
    while (sdfCheckPendingWorkWithInterrupts()) {
    }
    kwlnDrawControlFlags |= 0x2000000;
    func_003003F0("event stage shutdown !!\n");
    evtPrintDeveloperConsoleMessage("field shutdown.\n");
    return 1;
}

s32 evtCommandShutdownStageAlternate(void)
{
    evtDrainSecondaryWorldNodes();
    while (sdfCheckPendingWorkWithInterrupts()) {
    }
    evtDestroySecondaryWorldNode();
    while (sdfCheckPendingWorkWithInterrupts()) {
    }
    kwlnDrawControlFlags |= 0x2000000;
    func_003003F0("event stage shutdown2 !!\n");
    evtPrintDeveloperConsoleMessage("field shutdown2.\n");
    return 1;
}

s32 evtCommandClearAllUnits(void)
{
    evtDrainSecondaryWorldNodes();
    func_003003F0("unit all clear !!\n");
    evtPrintDeveloperConsoleMessage("unit all clear.\n");
    return 1;
}

s32 evtCommandClearAllUnitsAndWait(void)
{
    evtDrainSecondaryWorldNodes();
    while (sdfCheckPendingWorkWithInterrupts()) {
    }
    func_003003F0("unit all clear2 !!\n");
    evtPrintDeveloperConsoleMessage("unit all clear2.\n");
    return 1;
}

s32 evtCommandAddEffectUnitToWorld(void) {
    EffWorldNode *unit;

    if (scrReadIntParameter(0) < 0) {
        unit = (EffWorldNode *)fldGetPlayerSceneState();
    } else {
        unit = evtFindWorldObjectByIdAndKind(4, scrReadIntParameter(0));
    }
    if (unit == NULL) {
        return 1;
    }
    dds3SetWorldCameraObject(dds3GetWorldObject(), unit);
    return 1;
}

s32 evtCommandAddFlaggedEffectUnitToWorld(void) {
    EffWorldNode *unit;

    if (scrReadIntParameter(0) < 0) {
        unit = (EffWorldNode *)fldGetPlayerSceneState();
    } else {
        unit = evtFindWorldObjectByIdAndKind(4, scrReadIntParameter(0));
        ((EvtWorldUnit *)unit)->inner->statusFlags |= 1;
    }
    if (unit == NULL) {
        return 1;
    }
    dds3SetWorldCameraObject(dds3GetWorldObject(), unit);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_00226E98);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226F38);

/* Create with a zero position and identity rotation; return the world value to the VM. */
s32 evtCommandCreateWorldEffectObject(void) {
    f32 position[4];
    f32 rotation[4];
    s32 world;

    memset(position, 0, 0x10);
    memset(rotation, 0, 0x10);
    rotation[3] = 1.0f;
    world = dds3AdvanceWorldCounter();
    effObjSetInnerFloat(dds3CreateCameraObject(world, position, rotation), 1.0f);
    scrSetIntegerReturnValue(world);
    return 1;
}

s32 evtCommandDestroyEffectUnitById(void)
{
    s32 id;
    void *unit;

    id = scrReadIntParameter(0);
    unit = evtFindWorldObjectByIdAndKind(4, id);
    if (unit == NULL) {
        return 1;
    }
    dds3RemoveWorldObjectNode(unit);
    return 1;
}

s32 evtCommandSetEffectUnitFirstVector(void) {
    void *unit;
    f32 vec[4];

    memset(vec, 0, sizeof(vec));
    if (scrReadIntParameter(0) < 0) {
        unit = (void *)fldGetPlayerSceneState();
    } else {
        unit = evtFindWorldObjectByIdAndKind(4, scrReadIntParameter(0));
    }
    if (unit == NULL) {
        return 1;
    }
    vec[0] = bfWaitReadArgFloat(1);
    vec[1] = bfWaitReadArgFloat(2);
    vec[2] = bfWaitReadArgFloat(3);
    effObjSetInnerFirstVec(unit, vec);
    return 1;
}

/* Convert the three degree inputs before composing and storing a quaternion. */
s32 evtCommandSetEffectUnitEulerRotation(void) {
    void *unit;
    f32 quaternion[4];
    f32 degreesToRadians;
    f32 pitch;

    if (scrReadIntParameter(0) < 0) {
        unit = (void *)fldGetPlayerSceneState();
    } else {
        unit = evtFindWorldObjectByIdAndKind(4, scrReadIntParameter(0));
    }
    if (unit == NULL) {
        return 1;
    }
    degreesToRadians = 0.017453293f;
    pitch = bfWaitReadArgFloat(1) * degreesToRadians;
    func_002E7F20(pitch, bfWaitReadArgFloat(2) * degreesToRadians, 0.0f);
    VU0_MOVE_VF(vf11, vf10);
    func_002E7F20(0.0f, 0.0f, bfWaitReadArgFloat(3) * degreesToRadians);
    effMiscQuatMultiplyVU();
    VU0_STORE_VF(vf10, quaternion);
    effObjSetInnerSecondVec(unit, quaternion);
    return 1;
}

s32 evtCommandSetEffectUnitSecondVector(void) {
    void *unit;
    f32 vec[4];

    if (scrReadIntParameter(0) < 0) {
        unit = (void *)fldGetPlayerSceneState();
    } else {
        unit = evtFindWorldObjectByIdAndKind(4, scrReadIntParameter(0));
    }
    if (unit == NULL) {
        return 1;
    }
    vec[0] = bfWaitReadArgFloat(1);
    vec[1] = bfWaitReadArgFloat(2);
    vec[2] = bfWaitReadArgFloat(3);
    vec[3] = bfWaitReadArgFloat(4);
    effObjSetInnerSecondVec(unit, vec);
    return 1;
}

/* Slot 1 owns a path node with the same resource record as the slot provider. */
typedef struct ObjectResource {
    u32 owner;
    u32 handle;
    u32 value;
    u32 resourceId;
} ObjectResource;

typedef struct ObjectWithResource {
    u8 pad0[0x18];
    ObjectResource *resource;
} ObjectWithResource;

extern void dds3EnsureSlotData(void *);
extern void dds3SetSlotKey(ObjectWithResource *, u32);
extern void dds3ReplaceObjectResource(ObjectWithResource *);
extern Dds3PathCurveWork *dds3GetObjectResourceHandle(ObjectWithResource *);
extern void sdfSetFloatCounterDirection(u32, s32);
extern char D_003AC8E8[], D_003AC8F8[], D_003AC908[];

s32 func_002272B0(void) {
    void *unit;
    EvtWorldUnit *target;
    ObjBase *data;
    ObjectWithResource *slot;
    Dds3PathCurveWork *path;
    s32 room;

    unit = evtFindWorldObjectByIdAndKind(6, scrReadIntParameter(0));
    if (unit == NULL) {
        unit = (void *)func_00126200(scrReadIntParameter(0));
        if (unit == NULL) {
            func_003003F0(D_003AC8E8, scrReadIntParameter(0));
            func_003003F0(D_003AC8F8, scrReadIntParameter(1));
            func_003003F0(D_003AC908);
            return 1;
        }
    }
    target = evtFindWorldObjectByIdAndKind(0x10, scrReadIntParameter(1));
    if (target == NULL)
        return 1;
    data = effObjGetDataHandle(unit);
    slot = data->slots[1];
    if (slot == NULL) {
        dds3EnsureSlotData(unit);
        slot = data->slots[1];
    }
    if (slot->resource->resourceId == 0) {
        dds3SetSlotKey(slot, (u32)target);
        dds3ReplaceObjectResource(slot);
    }
    path = dds3GetObjectResourceHandle(slot);
    if (path == 0)
        return 1;
    switch (scrReadIntParameter(2)) {
    case 0:
        sdfSetFloatCounterDirection((u32 *)&path->direction, 0);
        if (target->roomName != NULL) {
            room = fldParseRoomNumberFromName(target->roomName);
            if (room > 0)
                fldSetMapSlotValueFlag(fldAreaState[4], fldAreaState[5] + 1, room, 1);
        }
        break;
    case 1:
        sdfSetFloatCounterDirection((u32 *)&path->direction, 1);
        if (target->roomName != NULL) {
            room = fldParseRoomNumberFromName(target->roomName);
            if (room > 0)
                fldSetMapSlotValueFlag(fldAreaState[4], fldAreaState[5] + 1, room, 0);
        }
        break;
    }
    return 1;
}

extern char D_003AC928[]; /* "re attach...!\n" */

s32 func_002274A0(void) {
    void *unit;
    void *target;
    ObjBase *data;
    ObjectWithResource *slot;
    Dds3PathCurveWork *path;
    s32 targetId;
    s32 mode;

    unit = evtFindWorldObjectByIdAndKind(6, scrReadIntParameter(0));
    if (unit == NULL) {
        unit = (void *)func_00126200(scrReadIntParameter(0));
        if (unit == NULL) {
            func_003003F0(D_003AC8E8, scrReadIntParameter(0));
            targetId = scrReadIntParameter(1);
            func_003003F0(D_003AC8F8, targetId);
            func_003003F0(D_003AC908);
            return 1;
        }
    }
    target = evtFindWorldObjectByIdAndKind(0x10, scrReadIntParameter(1));
    if (target == NULL) {
        return 1;
    }
    data = effObjGetDataHandle(unit);
    slot = data->slots[1];
    if (slot == NULL) {
        dds3EnsureSlotData(unit);
        slot = data->slots[1];
    }
    if (slot->resource->resourceId == 0) {
        dds3SetSlotKey(slot, (u32)target);
        dds3ReplaceObjectResource(slot);
    } else {
        dds3RefreshStoredVec3(unit);
        dds3SetSlotKey(slot, (u32)target);
        dds3ReplaceObjectResource(slot);
        func_003003F0(D_003AC928);
    }
    path = dds3GetObjectResourceHandle(slot);
    if (path == 0) {
        return 1;
    }
    mode = scrReadIntParameter(2);
    switch (mode) {
    case 0:
        sdfSetFloatCounterDirection((u32 *)&path->direction, 0);
        break;
    case 1:
        sdfSetFloatCounterDirection((u32 *)&path->direction, 1);
        break;
    }
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC8E8);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC8F8);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC908);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC928);

INCLUDE_ASM(const s32, "event/evtCommand", func_00227648);

s32 evtCommandEnableSolarAdvance(void)
{
    evtEnableSolarPhaseAdvance();
    return 1;
}

s32 evtCommandDisableSolarAdvance(void)
{
    evtDisableSolarPhaseAdvance();
    return 1;
}

s32 evtCommandReadMirroredSolarPhase(void)
{
    scrSetIntegerReturnValue(evtGetMirroredSolarPhase());
    return 1;
}

s32 evtCommandReadSolarPhase(void)
{
    scrSetIntegerReturnValue(evtGetSolarPhase());
    return 1;
}

s32 evtCommandSetSolarPhase(void)
{
    evtSetSolarPhase(scrReadIntParameter(0));
    return 1;
}

/* Create a missing camp task, then wait for its ready value.
 * No VM context completes immediately; a pending task returns zero. */
s32 evtCommandWaitForCampTask(void) {
    s32 taskId = scrReadIntParameter(0);
    ScrData *commandWork = scrGetCurrentContext();
    char *message;

    if (commandWork == NULL) {
        return 1;
    }
    if (evtFindTaskById(taskId) == 0) {
        message = D_003AC958;
        evtPrintDeveloperConsoleMessage(message, taskId);
        sdfPrintFormattedDevMessage(message, taskId);
        func_00101A80((s32)commandWork->task, mnuCampCreateTask(taskId));
        return 0;
    }
    if (evtGetTaskValueWord(taskId) == EVT_CAMP_TASK_READY_VALUE) {
        message = D_003AC968;
        evtPrintDeveloperConsoleMessage(message, taskId);
        sdfPrintFormattedDevMessage(message, taskId);
        return 1;
    }
    return 0;
}

/* Start the camp task only if no task with this ID exists yet.
 * A missing VM context completes without creating a task. */
s32 evtCommandStartCampTaskIfAbsent(void) {
    s32 taskId = scrReadIntParameter(0);
    ScrData *commandWork = scrGetCurrentContext();

    if (commandWork == NULL) {
        return 1;
    }
    if (evtFindTaskById(taskId) != 0) {
        return 1;
    }
    evtPrintDeveloperConsoleMessage(D_003AC978, taskId);
    func_00101A80((s32)commandWork->task, mnuCampCreateTask(taskId));
    return 1;
}

/* Store readiness in the VM result; the command itself always completes. */
s32 evtCommandTestCampTaskReady(void) {
    s32 taskId = scrReadIntParameter(0);
    s32 isReady;

    if (scrGetCurrentContext() != 0 && evtFindTaskById(taskId) != 0 && evtGetTaskValueWord(taskId) == EVT_CAMP_TASK_READY_VALUE) {
        evtPrintDeveloperConsoleMessage(D_003AC968, taskId);
        isReady = 1;
    } else {
        isReady = 0;
    }
    scrSetIntegerReturnValue(isReady);
    return 1;
}

/* Destroy the requested camp task; missing context or task is a completed no-op. */
s32 evtCommandDestroyCampTask(void) {
    s32 taskId = scrReadIntParameter(0);

    if (scrGetCurrentContext() == 0) {
        return 1;
    }
    if (evtFindTaskById(taskId) == 0) {
        return 1;
    }
    evtPrintDeveloperConsoleMessage(D_003AC988, taskId);
    mnuCampDestroyTaskById(taskId);
    return 1;
}

/* Create an event/scene movie task at the owning task's priority, set bit 0,
 * and return its handle through the VM result. Missing work skips creation. */
s32 evtCommandStartPolygonMovie(void) {
    ScrData *commandWork = scrGetCurrentContext();
    s32 eventId;
    s32 sceneId;
    s32 movieTask;

    if (commandWork == NULL) {
        return 1;
    }
    if (commandWork->task == 0) {
        func_003003F0(D_003AC998);
        return 1;
    }
    eventId = scrReadIntParameter(0);
    sceneId = scrReadIntParameter(1);
    movieTask = evtViewerCreateTask(commandWork->task->priority, eventId, sceneId);
    evtPrintDeveloperConsoleMessage(D_003AC9E0, scrReadIntParameter(0), scrReadIntParameter(1));
    func_00101A80((s32)commandWork->task, movieTask);
    evtPolygonMovieSetFlagBits(movieTask, 1);
    scrSetIntegerReturnValue(movieTask);
    return 1;
}

/* Clear bit 0 and echo the movie ID through the VM result. */
s32 evtCommandClearPolygonMovieFlag(void)
{
    s32 movieId;

    movieId = scrReadIntParameter(0);
    evtPolygonMovieClearFlagBits(movieId, 1);
    scrSetIntegerReturnValue(movieId);
    return 1;
}

/* Create and return an event/scene movie task without setting bit 0.
 * The owning scheduler task supplies the viewer task's priority. */
s32 evtCommandCreatePolygonMovie(void) {
    ScrData *commandWork = scrGetCurrentContext();
    s32 eventId;
    s32 sceneId;
    s32 movieTask;

    if (commandWork == NULL) {
        return 1;
    }
    if (commandWork->task == 0) {
        func_003003F0(D_003AC9F8);
        return 1;
    }
    eventId = scrReadIntParameter(0);
    sceneId = scrReadIntParameter(1);
    movieTask = evtViewerCreateTask(commandWork->task->priority, eventId, sceneId);
    func_00101A80((s32)commandWork->task, movieTask);
    scrSetIntegerReturnValue(movieTask);
    return 1;
}

/* Look up a world unit across the six kinds before trying the fallback slot. */
s32 evtCommandSetWorldSlotStatusFlag(void) {
    s32 unit;
    s32 objectKind = EVT_WORLD_OBJECT_KIND_FIRST;

    do {
        unit = (s32)evtFindWorldObjectByIdAndKind(objectKind, scrReadIntParameter(0));
        objectKind++;
    } while (objectKind < EVT_WORLD_OBJECT_KIND_LIMIT && unit == 0);
    if (unit == 0) {
        unit = func_00126200(scrReadIntParameter(0));
        if (unit == 0) {
            return 1;
        }
    }
    evtSetWorldSlotStatusFlag(unit);
    return 1;
}

/* Clear the first matching unit's status flag, using the same fallback search. */
s32 evtCommandClearWorldSlotStatusFlag(void) {
    s32 unit;
    s32 objectKind = EVT_WORLD_OBJECT_KIND_FIRST;

    do {
        unit = (s32)evtFindWorldObjectByIdAndKind(objectKind, scrReadIntParameter(0));
        objectKind++;
    } while (objectKind < EVT_WORLD_OBJECT_KIND_LIMIT && unit == 0);
    if (unit == 0) {
        unit = func_00126200(scrReadIntParameter(0));
        if (unit == 0) {
            return 1;
        }
    }
    evtClearWorldSlotStatusFlag(unit);
    return 1;
}

/* Apply the float state, then update a room flag when its parsed number is positive.
 * Values above 0.5 set that flag; the VM float parameter is deliberately re-read. */
s32 evtCommandSetUnitRoomFloatState(void) {
    u8 *unit;
    s32 roomNumber;
    char *roomName;
    s32 objectKind = EVT_WORLD_OBJECT_KIND_FIRST;

    do {
        unit = (u8 *)evtFindWorldObjectByIdAndKind(objectKind, scrReadIntParameter(0));
        objectKind++;
    } while (objectKind < EVT_WORLD_OBJECT_KIND_LIMIT && unit == NULL);
    if (unit == NULL) {
        unit = (u8 *)func_00126200(scrReadIntParameter(0));
        if (unit == NULL) {
            return 1;
        }
    }
    evtScaleSlotByClampedMultiplier(unit, bfWaitReadArgFloat(1));
    roomName = ((EvtWorldUnit *)unit)->roomName;
    if (roomName == 0) {
        return 1;
    }
    roomNumber = fldParseRoomNumberFromName(roomName);
    if (roomNumber > 0) {
        if (bfWaitReadArgFloat(1) > 0.5f) {
            fldSetMapSlotValueFlag(fldAreaState[4], fldAreaState[5] + 1, roomNumber, 1);
        } else {
            fldSetMapSlotValueFlag(fldAreaState[4], fldAreaState[5] + 1, roomNumber, 0);
        }
    }
    return 1;
}

/* Apply the integer state and update a positive parsed room number's flag.
 * Unlike the float command, an integer value of zero sets the room flag. */
s32 evtCommandSetUnitRoomIntegerState(void) {
    u8 *unit;
    s32 roomNumber;
    char *roomName;
    s32 objectKind = EVT_WORLD_OBJECT_KIND_FIRST;

    do {
        unit = (u8 *)evtFindWorldObjectByIdAndKind(objectKind, scrReadIntParameter(0));
        objectKind++;
    } while (objectKind < EVT_WORLD_OBJECT_KIND_LIMIT && unit == NULL);
    if (unit == NULL) {
        unit = (u8 *)func_00126200(scrReadIntParameter(0));
        if (unit == NULL) {
            return 1;
        }
    }
    evtSetWorldSlotValue(unit, scrReadIntParameter(1));
    roomName = ((EvtWorldUnit *)unit)->roomName;
    if (roomName == 0) {
        return 1;
    }
    roomNumber = fldParseRoomNumberFromName(roomName);
    if (roomNumber > 0) {
        if (scrReadIntParameter(1) == 0) {
            fldSetMapSlotValueFlag(fldAreaState[4], fldAreaState[5] + 1, roomNumber, 1);
        } else {
            fldSetMapSlotValueFlag(fldAreaState[4], fldAreaState[5] + 1, roomNumber, 0);
        }
    }
    return 1;
}

/* Enable the scaled-value flag on the first unit found, including the fallback. */
s32 evtCommandSetUnitScaledValueFlag(void)
{
    s32 objectKind;
    void *unit;

    objectKind = EVT_WORLD_OBJECT_KIND_FIRST;
    do {
        unit = evtFindWorldObjectByIdAndKind(objectKind, scrReadIntParameter(0));
        objectKind++;
    } while (objectKind < EVT_WORLD_OBJECT_KIND_LIMIT && unit == NULL);
    if (unit == NULL) {
        unit = (void *)func_00126200(scrReadIntParameter(0));
        if (unit == NULL) {
            return 1;
        }
    }
    evtToggleWorldSlotScaledValueFlag(unit, 1);
    return 1;
}

/* Disable the scaled-value flag using the same six-kind and fallback search. */
s32 evtCommandClearUnitScaledValueFlag(void)
{
    s32 objectKind;
    void *unit;

    objectKind = EVT_WORLD_OBJECT_KIND_FIRST;
    do {
        unit = evtFindWorldObjectByIdAndKind(objectKind, scrReadIntParameter(0));
        objectKind++;
    } while (objectKind < EVT_WORLD_OBJECT_KIND_LIMIT && unit == NULL);
    if (unit == NULL) {
        unit = (void *)func_00126200(scrReadIntParameter(0));
        if (unit == NULL) {
            return 1;
        }
    }
    evtToggleWorldSlotScaledValueFlag(unit, 0);
    return 1;
}

s32 evtCommandMoveLightAlongPathOrWarn(void) {
    void *path = evtFindWorldObjectByIdAndKind(9, scrReadIntParameter(0));

    if (evtStageRelinkOwnedNodeResource(evtFindWorldObjectByIdAndKind(0x10, scrReadIntParameter(1)), path) != 0) {
        return 1;
    }
    func_003003F0(D_003ACA40);
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

