#include "common.h"

extern s32 scrGetCommandTimer(void);

/* Script VM helpers (see script/scrCommonCommand.c for the convention). */
s32 scrReadIntParameter(s32 idx);

void *evtFindWorldObjectByIdAndKind(s32 type, s32 id);

void func_00115F40(void *unit);

void scrSetIntegerReturnValue(s32 value);

void func_001027D8(s32 command, s32 payload, s32 payloadSize, s32 mode);

void func_0035B6E0(const char *fmt, ...);

void scrDestroyAllNamedProcesses(void);

void evtSubmitEventRequest(s32 eventId, s32 requestMode);

void fldPlayCurrentBgmSound(void);

void fldRequestEncounterWithFade(u32 kind, s32 eventId);

extern u32 D_004371E8;

void evtSubmitEventRequestImmediate(s32 eventId);

void fldStopCurrentBgm(void);

void fldReleaseCurrentBgm(void);

char *scrReadStringParameter(s32 idx);

void fldInitializeSequenceAndResetFlags(s32 request, s32 sequence, s32 variant, char *name);

extern s32 D_00389780[];

void fldInitializeAlternateSequence(s32 request, s32 sequence, s32 variant, char *name);

void func_0033DAD8(const char *msg, ...);

void func_0025D390(s32 first, s32 second);

s32 dds3GetWorldSecondaryObject(void);

s32 dds3GetWorldObjectValue(s32 world);

void func_0023AA30(s32 highPart, s32 lowPart);

void func_0010AE38(const char *fmt, ...);

void func_0023A9E0(void);

s32 sdfCheckPendingWorkWithInterrupts(void);

void evtDestroySecondaryWorldNode(void);

extern u32 D_00435CD4;

void func_00110B50(void *unit);

s32 func_001287B8(s32 id);

void func_0023AFC8(void *unit, s32 enabled);

void func_0023B078(void *unit, s32 flag);

void effObjSetFlags(void *unit, s32 flag);

s32 evtGetMirroredSolarPhase(void);

s32 evtGetSolarPhase(void);

void evtPolygonMovieClearFlagBits(s32 movieId, u32 bits);

u32 fldGetPlayerSceneState(void);

void func_00110BE0(s32 world, u32 unit);

f32 bfWaitReadArgFloat(s32 idx);

s32 func_0010D8C8(void);

s32 evtFindTaskById(s32 id);

s32 evtGetTaskValueWord(s32 id);

extern s32 mnuCampCreateTask(s32 id);

extern char D_00421ED8[]; /* "BE ok! (%d)\n" */

extern char D_00421F08[];

typedef struct EvtCampTask {
    u8 pad00[0x20];
    s32 resource;
} EvtCampTask;

typedef struct EvtCommandWork {
    u8 pad00[0xE4];
    EvtCampTask *campTask;
} EvtCommandWork;

/* Same room-name and inner-status offsets as the DDS1 event unit. */
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

extern char D_00421F68[];

void evtSetWorldSlotStatusFlag();

void evtClearWorldSlotStatusFlag();

void func_0023AE08(s32 unit, f32 value);

s32 fldParseRoomNumberFromName(char *name);

s32 func_001235E8(s32 worldKey, s32 roomGroup, s32 roomNumber, s32 enabled);

extern u32 D_00389770[];

void func_0023AE70(void *unit, s32 value);

s32 func_0023AEB0(void *target, void *path);

extern char D_00421FB0[]; /* "LIGHT_PATH_MOVE error!\n" */

extern char D_00421FC8[]; /* "error: LIGHT_PATH_MOVE.\n" */

INCLUDE_ASM(const s32, "event/evtCommand", func_00240D20);

INCLUDE_ASM(const s32, "event/evtCommand", func_00240DE0);

s32 evtCommandEnablePathUnit(void)
{
    s32 id;
    void *unit;

    id = scrReadIntParameter(0);
    unit = evtFindWorldObjectByIdAndKind(7, id);
    if (unit != NULL) {
        func_0023B078(unit, 1);
        effObjSetFlags(unit, 1);
    }
    return 1;
}

/* The path search covers the six object kinds 4 through 9. */
s32 evtCommandAssignEffectObjectOwner(void) {
    void *path = evtFindWorldObjectByIdAndKind(7, scrReadIntParameter(0));
    s32 found;
    s32 i;

    if (path == NULL) {
        return 1;
    }
    i = 4;
    do {
        found = (s32)evtFindWorldObjectByIdAndKind(i, scrReadIntParameter(1));
        i++;
    } while (i < 10 && found == 0);
    if (found == 0) {
        return 1;
    }
    effObjBindValidatedOwner(path, found);
    return 1;
}

s32 evtCommandAssignEffectObjectOwnerWithEntry(void) {
    void *path = evtFindWorldObjectByIdAndKind(7, scrReadIntParameter(0));
    s32 found;
    s32 i;

    if (path == NULL) {
        return 1;
    }
    i = 4;
    do {
        found = (s32)evtFindWorldObjectByIdAndKind(i, scrReadIntParameter(1));
        i++;
    } while (i < 10 && found == 0);
    if (found == 0) {
        return 1;
    }
    effObjBindOwnerBillEntry(path, found, scrReadIntParameter(2));
    return 1;
}

s32 func_00241090(void)
{
    s32 id;
    void *unit;

    id = scrReadIntParameter(0);
    unit = evtFindWorldObjectByIdAndKind(7, id);
    if (unit == NULL) {
        return 1;
    }
    func_00115F40(unit);
    return 1;
}

s32 func_002410D0(void)
{
    s32 id;
    void *unit;

    id = scrReadIntParameter(0);
    unit = evtFindWorldObjectByIdAndKind(7, id);
    if (unit != NULL) {
        func_0023B078(unit, 0);
    }
    return 1;
}

s32 evtCommandAttachLightToUnitPath(void) {
    void *effect;
    void *path;

    effect = evtFindWorldObjectByIdAndKind(7, scrReadIntParameter(0));
    if (effect == NULL) {
        func_0010AE38("EFFECT_PATH_MOVE not found eff!\n");
        return 1;
    }
    path = evtFindWorldObjectByIdAndKind(0x10, scrReadIntParameter(1));
    if (path == NULL) {
        func_0010AE38("EFFECT_PATH_MOVE not found path!\n", effect);
        return 1;
    }
    func_0023AEB0(path, effect);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_002411A0);

typedef struct EvtIdNode {
    u8 pad00[4];
    s32 value; /* 0x04 */
} EvtIdNode;

extern EvtIdNode *func_001111A8(s32 world, char *id);

s32 evtCommandReadSecondaryWorldIdValue(void) {
    EvtIdNode *node;

    node = func_001111A8(dds3GetWorldSecondaryObject(), scrReadStringParameter(0));
    if (node == NULL) {
        func_0035B6E0("ID : id not found!! <%s>\n", scrReadStringParameter(0));
        func_0010AE38("WARNING: ID not found! <%s>\n", scrReadStringParameter(0));
        scrSetIntegerReturnValue(0);
    } else {
        scrSetIntegerReturnValue(node->value);
    }
    return 1;
}

u32 func_002412B0(void) {
    func_001027D8(2, 0, 0, 0);
    return 1;
}

u32 func_002412E0(void) {
    func_001027D8(0xc, 0, 0, 0);
    return 1;
}

u32 func_00241310(void) {
    func_001027D8(4, 0, 0, 0);
    return 1;
}

s32 evtCommandSubmitPairedAdminRequest(void)
{
    s32 args[2];

    args[0] = scrReadIntParameter(0);
    args[1] = scrReadIntParameter(1);
    func_001027D8(0x15, (s32)args, 8, 0);
    scrSetIntegerReturnValue(0);
    return 1;
}

extern void fldSetDeferredFieldCommand(s32 a, s32 b);

s32 evtCommandDeferFieldTransition(void) {
    s32 a = scrReadIntParameter(0);
    s32 b = scrReadIntParameter(1);
    s32 c = scrReadIntParameter(2);
    s32 d;

    func_0035B6E0("CALL_NEXT(%d,%d,%d)\n", a, b, c);
    d = scrReadIntParameter(1);
    fldSetDeferredFieldCommand(d, scrReadIntParameter(2));
    return 1;
}

/* The event ID becomes a four-byte dispatch payload. */
s32 evtCommandCallEvent(void)
{
    s32 eventId;

    eventId = scrReadIntParameter(0);
    func_0035B6E0("call_event:%d\n", eventId);
    func_001027D8(6, (s32)&eventId, 4, 0);
    scrDestroyAllNamedProcesses();
    return 1;
}

u32 evtCommandSignalAdminAtTimerZero(void) {
    s64 timer;

    timer = scrGetCommandTimer();
    if (timer == 0) {
        dds3AdminSetControlFlag();
    }
    return 0;
}

void evtSubmitEventRequest(s32 eventId, s32 requestMode)
{
    s32 args[2];

    fldPlayCurrentBgmSound();
    fldRequestEncounterWithFade(2, eventId);
    D_004371E8 = requestMode;
    args[0] = 0;
    args[1] = eventId;
    func_001027D8(0xe, (s32)args, 8, requestMode > 0);
    scrDestroyAllNamedProcesses();
}

s32 evtCommandSubmitEvent(void)
{
    s32 eventId;

    eventId = scrReadIntParameter(0);
    evtSubmitEventRequest(eventId, 0);
    return 1;
}

void evtSubmitEventRequestImmediate(s32 eventId)
{
    s32 args[2];

    fldStopCurrentBgm();
    fldRequestEncounterWithFade(2, eventId);
    D_004371E8 = 0;
    args[0] = 0;
    args[1] = eventId;
    func_001027D8(0xe, (s32)args, 8, 1);
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
    s32 mode;

    eventId = scrReadIntParameter(0);
    mode = scrReadIntParameter(1);
    evtSubmitEventRequest(eventId, mode);
    return 1;
}

/* A field-sequence request is a fixed 0xa0-byte VM message. */
s32 evtCommandRequestFieldSequence(void)
{
    s32 firstArg;
    s32 secondArg;
    char *textArg;
    u8 request[0xa0];

    fldReleaseCurrentBgm();
    firstArg = scrReadIntParameter(0);
    secondArg = scrReadIntParameter(1);
    textArg = scrReadStringParameter(2);
    fldInitializeSequenceAndResetFlags((s32)request, firstArg, secondArg, textArg);
    func_001027D8(5, (s32)request, 0xa0, 0);
    scrDestroyAllNamedProcesses();
    return 1;
}

s32 evtCommandRequestCurrentGroupSequence(void)
{
    s32 firstArg;
    char *textArg;
    u8 request[0xa0];

    firstArg = scrReadIntParameter(0);
    textArg = scrReadStringParameter(1);
    fldInitializeSequenceAndResetFlags((s32)request, D_00389780[0], firstArg, textArg);
    func_001027D8(5, (s32)request, 0xa0, 0);
    scrDestroyAllNamedProcesses();
    return 1;
}

s32 func_002416D0(void)
{
    s32 p0;

    p0 = scrReadIntParameter(0);
    func_001027D8(0x1c, (s32)&p0, 4, 0);
    return 1;
}

s32 evtCommandRequestAlternateFieldSequence(void)
{
    s32 firstArg;
    char *textArg;
    u8 request[0xa0];

    firstArg = scrReadIntParameter(0);
    textArg = scrReadStringParameter(1);
    fldInitializeAlternateSequence((s32)request, D_00389780[0], firstArg, textArg);
    func_001027D8(5, (s32)request, 0xa0, 0);
    scrDestroyAllNamedProcesses();
    return 1;
}

s32 evtCommandDispatchFieldBE(void)
{
    s32 firstArg;
    s32 secondArg;

    func_0033DAD8("FIELD_BE start\n");
    firstArg = scrReadIntParameter(0);
    secondArg = scrReadIntParameter(1);
    func_0025D390(firstArg, secondArg);
    func_0033DAD8("FIELD_BE end\n");
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
        func_0023AA30(highPart, lowPart);
    }
    return 1;
}

u32 func_00241838(void) {
    u64 value;

    value = scrReadIntParameter(0);
    func_0023B0D0(0, value);
    return 1;
}

u32 func_00241868(void) {
    u64 value;

    value = scrReadIntParameter(0);
    func_0023B0D0(1, value);
    return 1;
}

s32 evtCommandShutdownStage(void)
{
    func_0023A9E0();
    while (sdfCheckPendingWorkWithInterrupts()) {
    }
    evtDestroySecondaryWorldNode();
    while (sdfCheckPendingWorkWithInterrupts()) {
    }
    D_00435CD4 |= 0x2000000;
    func_0035B6E0("event stage shutdown !!\n");
    func_0010AE38("field shutdown.\n");
    return 1;
}

s32 evtCommandShutdownStageAlternate(void)
{
    func_0023A9E0();
    while (sdfCheckPendingWorkWithInterrupts()) {
    }
    evtDestroySecondaryWorldNode();
    while (sdfCheckPendingWorkWithInterrupts()) {
    }
    D_00435CD4 |= 0x2000000;
    func_0035B6E0("event stage shutdown2 !!\n");
    func_0010AE38("field shutdown2.\n");
    return 1;
}

s32 evtCommandClearAllUnits(void)
{
    func_0023A9E0();
    func_0035B6E0("unit all clear !!\n");
    func_0010AE38("unit all clear.\n");
    return 1;
}

s32 evtCommandClearAllUnitsAndWait(void)
{
    func_0023A9E0();
    while (sdfCheckPendingWorkWithInterrupts()) {
    }
    func_0035B6E0("unit all clear2 !!\n");
    func_0010AE38("unit all clear2.\n");
    return 1;
}

s32 evtCommandAddEffectUnitToWorld(void) {
    void *unit;

    if (scrReadIntParameter(0) < 0) {
        unit = (void *)fldGetPlayerSceneState();
    } else {
        unit = evtFindWorldObjectByIdAndKind(4, scrReadIntParameter(0));
    }
    if (unit == NULL) {
        return 1;
    }
    func_00110BE0(dds3GetWorldObject(), (u32)unit);
    return 1;
}

s32 evtCommandAddFlaggedEffectUnitToWorld(void) {
    void *unit;

    if (scrReadIntParameter(0) < 0) {
        unit = (void *)fldGetPlayerSceneState();
    } else {
        unit = evtFindWorldObjectByIdAndKind(4, scrReadIntParameter(0));
        ((EvtWorldUnit *)unit)->inner->statusFlags |= 1;
    }
    if (unit == NULL) {
        return 1;
    }
    func_00110BE0(dds3GetWorldObject(), (u32)unit);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_00241AF8);

INCLUDE_ASM(const s32, "event/evtCommand", func_00241B98);

extern void *memset(void *dst, s32 value, u32 size);
extern s32 dds3AdvanceWorldCounter();
extern s32 func_00112E30(s32 world, f32 *pos, f32 *rot);
extern void effObjSetInnerFloat(s32 obj, f32 value);

s32 evtCommandCreateWorldEffectObject(void) {
    f32 pos[4];
    f32 rot[4];
    s32 world;

    memset(pos, 0, 0x10);
    memset(rot, 0, 0x10);
    rot[3] = 1.0f;
    world = dds3AdvanceWorldCounter();
    effObjSetInnerFloat(func_00112E30(world, pos, rot), 1.0f);
    scrSetIntegerReturnValue(world);
    return 1;
}

s32 func_00241CB0(void)
{
    s32 id;
    void *unit;

    id = scrReadIntParameter(0);
    unit = evtFindWorldObjectByIdAndKind(4, id);
    if (unit == NULL) {
        return 1;
    }
    func_00110B50(unit);
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

INCLUDE_ASM(const s32, "event/evtCommand", func_00241D98);

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

INCLUDE_ASM(const s32, "event/evtCommand", func_00241F10);

INCLUDE_RODATA(const s32, "event/evtCommand", D_00421E58);

INCLUDE_RODATA(const s32, "event/evtCommand", D_00421E68);

INCLUDE_RODATA(const s32, "event/evtCommand", D_00421E78);

INCLUDE_ASM(const s32, "event/evtCommand", func_00242100);

INCLUDE_ASM(const s32, "event/evtCommand", func_002422A8);

u32 func_00242368(void) {
    evtEnableSolarPhaseAdvance();
    return 1;
}

u32 func_00242388(void) {
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

u32 evtCommandSetSolarPhase(void) {
    u64 phase;

    phase = scrReadIntParameter(0);
    evtSetSolarPhase(phase);
    return 1;
}

s32 evtCommandWaitForCampTask(void) {
    s32 id = scrReadIntParameter(0);
    EvtCommandWork *work = (EvtCommandWork *)func_0010D8C8();
    char *msg;

    if (work == NULL) {
        return 1;
    }
    if (evtFindTaskById(id) == 0) {
        msg = "load BE (%d)..\n";
        func_0010AE38(msg, id);
        func_0033DAD8(msg, id);
        func_00101968((s32)work->campTask, mnuCampCreateTask(id));
        return 0;
    }
    if (evtGetTaskValueWord(id) == 2) {
        msg = D_00421ED8;
        func_0010AE38(msg, id);
        func_0033DAD8(msg, id);
        return 1;
    }
    return 0;
}

/* Start the camp task only if no task with this ID exists yet. */
INCLUDE_RODATA(const s32, "event/evtCommand", D_00421ED8);

s32 evtCommandStartCampTaskIfAbsent(void) {
    s32 id = scrReadIntParameter(0);
    EvtCommandWork *work = (EvtCommandWork *)func_0010D8C8();

    if (work == NULL) {
        return 1;
    }
    if (evtFindTaskById(id) != 0) {
        return 1;
    }
    func_0010AE38("read BE (%d)..\n", id);
    func_00101968((s32)work->campTask, mnuCampCreateTask(id));
    return 1;
}

s32 evtCommandTestCampTaskReady(void) {
    s32 id = scrReadIntParameter(0);
    s32 ok;

    if (func_0010D8C8() != 0 && evtFindTaskById(id) != 0 && evtGetTaskValueWord(id) == 2) {
        func_0010AE38(D_00421ED8, id);
        ok = 1;
    } else {
        ok = 0;
    }
    scrSetIntegerReturnValue(ok);
    return 1;
}

extern void mnuCampDestroyTaskById(s32 id);

s32 evtCommandDestroyCampTask(void) {
    s32 id = scrReadIntParameter(0);

    if (func_0010D8C8() == 0) {
        return 1;
    }
    if (evtFindTaskById(id) == 0) {
        return 1;
    }
    func_0010AE38("del BE (%d)..\n", id);
    mnuCampDestroyTaskById(id);
    return 1;
}

/* Start a polygon movie on the active camp task's resource. */
INCLUDE_RODATA(const s32, "event/evtCommand", D_00421F08);

s32 evtCommandStartPolygonMovie(void) {
    EvtCommandWork *work = (EvtCommandWork *)func_0010D8C8();
    s32 a;
    s32 b;
    s32 result;

    if (work == NULL) {
        return 1;
    }
    if (work->campTask == 0) {
        func_0035B6E0(D_00421F08);
        return 1;
    }
    a = scrReadIntParameter(0);
    b = scrReadIntParameter(1);
    result = func_0024FCB8(work->campTask->resource, a, b);
    func_0010AE38("load PMV (%03d_%03d)..\n", scrReadIntParameter(0), scrReadIntParameter(1));
    func_00101968((s32)work->campTask, result);
    evtPolygonMovieSetFlagBits(result, 1);
    scrSetIntegerReturnValue(result);
    return 1;
}

s32 evtCommandClearPolygonMovieFlag(void)
{
    s32 p0;

    p0 = scrReadIntParameter(0);
    evtPolygonMovieClearFlagBits(p0, 1);
    scrSetIntegerReturnValue(p0);
    return 1;
}

/* Polygon movies use the active camp task's resource as their owner. */
s32 evtCommandCreatePolygonMovie(void) {
    EvtCommandWork *work = (EvtCommandWork *)func_0010D8C8();
    s32 a;
    s32 b;
    s32 result;

    if (work == NULL) {
        return 1;
    }
    if (work->campTask == 0) {
        func_0035B6E0(D_00421F68);
        return 1;
    }
    a = scrReadIntParameter(0);
    b = scrReadIntParameter(1);
    result = func_0024FCB8(work->campTask->resource, a, b);
    func_00101968((s32)work->campTask, result);
    scrSetIntegerReturnValue(result);
    return 1;
}

/* Look up a world unit across the six kinds before trying the fallback slot. */
s32 evtCommandSetWorldSlotStatusFlag(void) {
    s32 found;
    s32 i = 4;

    do {
        found = (s32)evtFindWorldObjectByIdAndKind(i, scrReadIntParameter(0));
        i++;
    } while (i < 10 && found == 0);
    if (found == 0) {
        found = func_001287B8(scrReadIntParameter(0));
        if (found == 0) {
            return 1;
        }
    }
    evtSetWorldSlotStatusFlag(found);
    return 1;
}

s32 evtCommandClearWorldSlotStatusFlag(void) {
    s32 found;
    s32 i = 4;

    do {
        found = (s32)evtFindWorldObjectByIdAndKind(i, scrReadIntParameter(0));
        i++;
    } while (i < 10 && found == 0);
    if (found == 0) {
        found = func_001287B8(scrReadIntParameter(0));
        if (found == 0) {
            return 1;
        }
    }
    evtClearWorldSlotStatusFlag(found);
    return 1;
}

s32 evtCommandSetUnitRoomFloatState(void) {
    u8 *unit;
    s32 count;
    char *owner;
    s32 i = 4;

    do {
        unit = (u8 *)evtFindWorldObjectByIdAndKind(i, scrReadIntParameter(0));
        i++;
    } while (i < 10 && unit == NULL);
    if (unit == NULL) {
        unit = (u8 *)func_001287B8(scrReadIntParameter(0));
        if (unit == NULL) {
            return 1;
        }
    }
    func_0023AE08(unit, bfWaitReadArgFloat(1));
    owner = ((EvtWorldUnit *)unit)->roomName;
    if (owner == 0) {
        return 1;
    }
    count = fldParseRoomNumberFromName(owner);
    if (count > 0) {
        if (bfWaitReadArgFloat(1) > 0.5f) {
            func_001235E8(D_00389770[4], D_00389770[5] + 1, count, 1);
        } else {
            func_001235E8(D_00389770[4], D_00389770[5] + 1, count, 0);
        }
    }
    return 1;
}

s32 evtCommandSetUnitRoomIntegerState(void) {
    u8 *unit;
    s32 count;
    char *owner;
    s32 i = 4;

    do {
        unit = (u8 *)evtFindWorldObjectByIdAndKind(i, scrReadIntParameter(0));
        i++;
    } while (i < 10 && unit == NULL);
    if (unit == NULL) {
        unit = (u8 *)func_001287B8(scrReadIntParameter(0));
        if (unit == NULL) {
            return 1;
        }
    }
    func_0023AE70(unit, scrReadIntParameter(1));
    owner = ((EvtWorldUnit *)unit)->roomName;
    if (owner == 0) {
        return 1;
    }
    count = fldParseRoomNumberFromName(owner);
    if (count > 0) {
        if (scrReadIntParameter(1) == 0) {
            func_001235E8(D_00389770[4], D_00389770[5] + 1, count, 1);
        } else {
            func_001235E8(D_00389770[4], D_00389770[5] + 1, count, 0);
        }
    }
    return 1;
}

s32 evtCommandSetUnitScaledValueFlag(void)
{
    s32 i;
    void *unit;

    i = 4;
    do {
        unit = evtFindWorldObjectByIdAndKind(i, scrReadIntParameter(0));
        i++;
    } while (i < 10 && unit == NULL);
    if (unit == NULL) {
        unit = (void *)func_001287B8(scrReadIntParameter(0));
        if (unit == NULL) {
            return 1;
        }
    }
    func_0023AFC8(unit, 1);
    return 1;
}

s32 evtCommandClearUnitScaledValueFlag(void)
{
    s32 i;
    void *unit;

    i = 4;
    do {
        unit = evtFindWorldObjectByIdAndKind(i, scrReadIntParameter(0));
        i++;
    } while (i < 10 && unit == NULL);
    if (unit == NULL) {
        unit = (void *)func_001287B8(scrReadIntParameter(0));
        if (unit == NULL) {
            return 1;
        }
    }
    func_0023AFC8(unit, 0);
    return 1;
}

s32 evtCommandMoveLightAlongPathOrWarn(void) {
    void *path = evtFindWorldObjectByIdAndKind(9, scrReadIntParameter(0));

    if (func_0023AEB0(evtFindWorldObjectByIdAndKind(0x10, scrReadIntParameter(1)), path) != 0) {
        return 1;
    }
    func_0035B6E0(D_00421FB0);
    func_0010AE38(D_00421FC8);
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtCommand", D_00421F68);

INCLUDE_RODATA(const s32, "event/evtCommand", D_00421FB0);

INCLUDE_RODATA(const s32, "event/evtCommand", D_00421FC8);

