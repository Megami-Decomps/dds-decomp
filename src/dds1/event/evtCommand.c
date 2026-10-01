#include "common.h"

/* Script VM helpers (see script/scrCommonCommand.c for the convention). */
s32 scrReadIntParameter(s32 idx);

void scrSetIntegerReturnValue(s32 value);

s32 scrGetCommandTimer(void);

void *evtFindWorldObjectByIdAndKind(s32 type, s32 id);

void func_00220508(void *unit, s32 flag);

void effObjSetFlags(void *unit, s32 flag);

void evtApplyIndexValueToWorldNodes(s32 which, s32 value);

void func_001028E8(s32 command, s32 payload, s32 payloadSize, s32 mode);

void dds3AdminSetControlFlag(void);

void evtSubmitEventRequest(s32 eventId, s32 mode);

void evtSubmitEventRequestImmediate(s32 eventId);

void evtEnableSolarPhaseAdvance(void);

void evtDisableSolarPhaseAdvance(void);

s32 evtGetMirroredSolarPhase(void);

s32 evtGetSolarPhase(void);

void evtSetSolarPhase(s32 phase);

void evtPolygonMovieClearFlagBits(s32 movieId, u32 bits);

void func_0010AC10(const char *fmt, ...);

void func_003003F0(const char *fmt, ...);

void scrDestroyAllNamedProcesses(void);

void func_0021FE70(void);

void func_00115CD8(void *unit);

void func_00110928(void *unit);

void fldStopCurrentBgm(void);

void fldPlayCurrentBgmSound(void);

void fldReleaseCurrentBgm(void);

char *scrReadStringParameter(s32 idx);

void fldInitializeSequenceAndResetFlags(s32 request, s32 sequence, s32 variant, char *name);

void fldInitializeAlternateSequence(s32 request, s32 sequence, s32 variant, char *name);

s32 sdfCheckPendingWorkWithInterrupts(void);

s32 dds3GetWorldSecondaryObject(void);

s32 dds3GetWorldObjectValue(s32 world);

void func_0021FEC0(s32 highPart, s32 lowPart);

void evtDestroySecondaryWorldNode(void);

s32 func_00220340(void *target, void *path);

f32 bfWaitReadArgFloat(s32 idx);

void func_00220298(s32 unit, f32 value);

s32 fldParseRoomNumberFromName(char *name);

s32 func_00121650(s32 worldKey, s32 roomGroup, s32 roomNumber, s32 enabled);

extern u32 D_0032E3B0[];

extern u32 D_003BA904;

s32 func_00126200(s32 id);

void evtSetWorldSlotStatusFlag();

void evtClearWorldSlotStatusFlag();

void func_00220300(void *unit, s32 value);

void func_00220458(void *unit, s32 enabled);

u32 fldGetPlayerSceneState(void);

void func_001109B8(s32 world, u32 unit);

s32 func_0010D6A0(void);

s32 evtFindTaskById(s32 id);

s32 evtGetTaskValueWord(s32 id);

extern char D_003AC968[]; /* "BE ok! (%d)\n" */

extern char D_003AC978[];

extern char D_003AC988[]; /* "del BE (%d)..\n" */

extern char D_003ACA40[]; /* "LIGHT_PATH_MOVE error!\n" */

extern char D_003ACA58[]; /* "error: LIGHT_PATH_MOVE.\n" */

void sdfPrintFormattedDevMessage(const char *msg, ...);

void func_00241F78(s32 first, s32 second);

extern s32 D_0032E3C0[];

void fldRequestEncounterWithFade(u32 kind, s32 eventId);

extern u32 D_003BBDA8;

typedef struct EvtCampTask {
    u8 pad00[0x20];
    s32 resource;
} EvtCampTask;

typedef struct EvtCommandWork {
    u8 pad00[0xE4];
    EvtCampTask *campTask;
} EvtCommandWork;

typedef struct EvtIdNode {
    u8 pad00[4];
    s32 value; /* 0x04 */
} EvtIdNode;

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

extern EvtIdNode *func_00110F80(s32 world, char *id);

extern void fldSetDeferredFieldCommand(s32 a, s32 b);

extern void *memset(void *dst, s32 value, u32 size);

extern s32 dds3AdvanceWorldCounter();

extern s32 func_00112C08(s32 world, f32 *pos, f32 *rot);

extern void effObjSetInnerFloat(s32 obj, f32 value);

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
        func_00220508(unit, 1);
        effObjSetFlags(unit, 1);
    }
    return 1;
}

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

s32 func_00226430(void)
{
    s32 id;
    void *unit;

    id = scrReadIntParameter(0);
    unit = evtFindWorldObjectByIdAndKind(7, id);
    if (unit == NULL) {
        return 1;
    }
    func_00115CD8(unit);
    return 1;
}

s32 func_00226470(void)
{
    s32 id;
    void *unit;

    id = scrReadIntParameter(0);
    unit = evtFindWorldObjectByIdAndKind(7, id);
    if (unit != NULL) {
        func_00220508(unit, 0);
    }
    return 1;
}

s32 evtCommandAttachLightToUnitPath(void) {
    void *path = evtFindWorldObjectByIdAndKind(7, scrReadIntParameter(0));
    void *target;

    if (path == NULL) {
        func_0010AC10(D_003AC700);
        return 1;
    }
    target = evtFindWorldObjectByIdAndKind(0x10, scrReadIntParameter(1));
    if (target == NULL) {
        func_0010AC10(D_003AC728);
        return 1;
    }
    func_00220340(target, path);
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC700);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC728);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226540);

s32 evtCommandReadSecondaryWorldIdValue(void) {
    EvtIdNode *node;

    node = func_00110F80(dds3GetWorldSecondaryObject(), scrReadStringParameter(0));
    if (node == NULL) {
        func_003003F0("ID : id not found!! <%s>\n", scrReadStringParameter(0));
        func_0010AC10("WARNING: ID not found! <%s>\n", scrReadStringParameter(0));
        scrSetIntegerReturnValue(0);
    } else {
        scrSetIntegerReturnValue(node->value);
    }
    return 1;
}

s32 func_00226650(void)
{
    func_001028E8(2, 0, 0, 0);
    return 1;
}

s32 func_00226680(void)
{
    func_001028E8(0xc, 0, 0, 0);
    return 1;
}

s32 func_002266B0(void)
{
    func_001028E8(4, 0, 0, 0);
    return 1;
}

s32 evtCommandSubmitPairedAdminRequest(void)
{
    s32 args[2];

    args[0] = scrReadIntParameter(0);
    args[1] = scrReadIntParameter(1);
    func_001028E8(0x15, (s32)args, 8, 0);
    scrSetIntegerReturnValue(0);
    return 1;
}

s32 evtCommandDeferFieldTransition(void) {
    s32 a = scrReadIntParameter(0);
    s32 b = scrReadIntParameter(1);
    s32 c = scrReadIntParameter(2);
    s32 d;

    func_003003F0("CALL_NEXT(%d,%d,%d)\n", a, b, c);
    d = scrReadIntParameter(1);
    fldSetDeferredFieldCommand(d, scrReadIntParameter(2));
    return 1;
}

s32 evtCommandCallEvent(void)
{
    s32 eventId;

    eventId = scrReadIntParameter(0);
    func_003003F0("call_event:%d\n", eventId);
    func_001028E8(6, (s32)&eventId, 4, 0);
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

void evtSubmitEventRequest(s32 eventId, s32 mode)
{
    s32 args[2];

    fldPlayCurrentBgmSound();
    fldRequestEncounterWithFade(2, eventId);
    D_003BBDA8 = mode;
    args[0] = 0;
    args[1] = eventId;
    func_001028E8(0xe, (s32)args, 8, mode > 0);
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
    D_003BBDA8 = 0;
    args[0] = 0;
    args[1] = eventId;
    func_001028E8(0xe, (s32)args, 8, 1);
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
    func_001028E8(5, (s32)request, 0xa0, 0);
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
    fldInitializeSequenceAndResetFlags((s32)request, D_0032E3C0[0], firstArg, textArg);
    func_001028E8(5, (s32)request, 0xa0, 0);
    scrDestroyAllNamedProcesses();
    return 1;
}

s32 func_00226A70(void)
{
    s32 p0;

    p0 = scrReadIntParameter(0);
    func_001028E8(0x1c, (s32)&p0, 4, 0);
    return 1;
}

s32 evtCommandRequestAlternateFieldSequence(void)
{
    s32 firstArg;
    char *textArg;
    u8 request[0xa0];

    firstArg = scrReadIntParameter(0);
    textArg = scrReadStringParameter(1);
    fldInitializeAlternateSequence((s32)request, D_0032E3C0[0], firstArg, textArg);
    func_001028E8(5, (s32)request, 0xa0, 0);
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
    func_00241F78(firstArg, secondArg);
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
        func_0021FEC0(highPart, lowPart);
    }
    return 1;
}

s32 func_00226BD8(void)
{
    s32 value;

    value = scrReadIntParameter(0);
    evtApplyIndexValueToWorldNodes(0, value);
    return 1;
}

s32 func_00226C08(void)
{
    s32 value;

    value = scrReadIntParameter(0);
    evtApplyIndexValueToWorldNodes(1, value);
    return 1;
}

s32 evtCommandShutdownStage(void)
{
    func_0021FE70();
    while (sdfCheckPendingWorkWithInterrupts()) {
    }
    evtDestroySecondaryWorldNode();
    while (sdfCheckPendingWorkWithInterrupts()) {
    }
    D_003BA904 |= 0x2000000;
    func_003003F0("event stage shutdown !!\n");
    func_0010AC10("field shutdown.\n");
    return 1;
}

s32 evtCommandShutdownStageAlternate(void)
{
    func_0021FE70();
    while (sdfCheckPendingWorkWithInterrupts()) {
    }
    evtDestroySecondaryWorldNode();
    while (sdfCheckPendingWorkWithInterrupts()) {
    }
    D_003BA904 |= 0x2000000;
    func_003003F0("event stage shutdown2 !!\n");
    func_0010AC10("field shutdown2.\n");
    return 1;
}

s32 evtCommandClearAllUnits(void)
{
    func_0021FE70();
    func_003003F0("unit all clear !!\n");
    func_0010AC10("unit all clear.\n");
    return 1;
}

s32 evtCommandClearAllUnitsAndWait(void)
{
    func_0021FE70();
    while (sdfCheckPendingWorkWithInterrupts()) {
    }
    func_003003F0("unit all clear2 !!\n");
    func_0010AC10("unit all clear2.\n");
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
    func_001109B8(dds3GetWorldObject(), (u32)unit);
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
    func_001109B8(dds3GetWorldObject(), (u32)unit);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_00226E98);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226F38);

s32 evtCommandCreateWorldEffectObject(void) {
    f32 pos[4];
    f32 rot[4];
    s32 world;

    memset(pos, 0, 0x10);
    memset(rot, 0, 0x10);
    rot[3] = 1.0f;
    world = dds3AdvanceWorldCounter();
    effObjSetInnerFloat(func_00112C08(world, pos, rot), 1.0f);
    scrSetIntegerReturnValue(world);
    return 1;
}

s32 func_00227050(void)
{
    s32 id;
    void *unit;

    id = scrReadIntParameter(0);
    unit = evtFindWorldObjectByIdAndKind(4, id);
    if (unit == NULL) {
        return 1;
    }
    func_00110928(unit);
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

INCLUDE_ASM(const s32, "event/evtCommand", func_00227138);

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

INCLUDE_ASM(const s32, "event/evtCommand", func_002272B0);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC8E8);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC8F8);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC908);

INCLUDE_ASM(const s32, "event/evtCommand", func_002274A0);

INCLUDE_ASM(const s32, "event/evtCommand", func_00227648);

s32 func_00227708(void)
{
    evtEnableSolarPhaseAdvance();
    return 1;
}

s32 func_00227728(void)
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

s32 evtCommandWaitForCampTask(void) {
    s32 id = scrReadIntParameter(0);
    EvtCommandWork *work = (EvtCommandWork *)func_0010D6A0();
    char *msg;

    if (work == NULL) {
        return 1;
    }
    if (evtFindTaskById(id) == 0) {
        msg = D_003AC958;
        func_0010AC10(msg, id);
        sdfPrintFormattedDevMessage(msg, id);
        func_00101A80((s32)work->campTask, mnuCampCreateTask(id));
        return 0;
    }
    if (evtGetTaskValueWord(id) == 2) {
        msg = D_003AC968;
        func_0010AC10(msg, id);
        sdfPrintFormattedDevMessage(msg, id);
        return 1;
    }
    return 0;
}

s32 evtCommandStartCampTaskIfAbsent(void) {
    s32 id = scrReadIntParameter(0);
    EvtCommandWork *work = (EvtCommandWork *)func_0010D6A0();

    if (work == NULL) {
        return 1;
    }
    if (evtFindTaskById(id) != 0) {
        return 1;
    }
    func_0010AC10(D_003AC978, id);
    func_00101A80((s32)work->campTask, mnuCampCreateTask(id));
    return 1;
}

s32 evtCommandTestCampTaskReady(void) {
    s32 id = scrReadIntParameter(0);
    s32 ok;

    if (func_0010D6A0() != 0 && evtFindTaskById(id) != 0 && evtGetTaskValueWord(id) == 2) {
        func_0010AC10(D_003AC968, id);
        ok = 1;
    } else {
        ok = 0;
    }
    scrSetIntegerReturnValue(ok);
    return 1;
}

s32 evtCommandDestroyCampTask(void) {
    s32 id = scrReadIntParameter(0);

    if (func_0010D6A0() == 0) {
        return 1;
    }
    if (evtFindTaskById(id) == 0) {
        return 1;
    }
    func_0010AC10(D_003AC988, id);
    mnuCampDestroyTaskById(id);
    return 1;
}

s32 evtCommandStartPolygonMovie(void) {
    EvtCommandWork *work = (EvtCommandWork *)func_0010D6A0();
    s32 a;
    s32 b;
    s32 result;

    if (work == NULL) {
        return 1;
    }
    if (work->campTask == 0) {
        func_003003F0(D_003AC998);
        return 1;
    }
    a = scrReadIntParameter(0);
    b = scrReadIntParameter(1);
    result = func_00234F18(work->campTask->resource, a, b);
    func_0010AC10(D_003AC9E0, scrReadIntParameter(0), scrReadIntParameter(1));
    func_00101A80((s32)work->campTask, result);
    evtPolygonMovieSetFlagBits(result, 1);
    scrSetIntegerReturnValue(result);
    return 1;
}

s32 evtCommandClearPolygonMovieFlag(void)
{
    s32 movieId;

    movieId = scrReadIntParameter(0);
    evtPolygonMovieClearFlagBits(movieId, 1);
    scrSetIntegerReturnValue(movieId);
    return 1;
}

s32 evtCommandCreatePolygonMovie(void) {
    EvtCommandWork *work = (EvtCommandWork *)func_0010D6A0();
    s32 a;
    s32 b;
    s32 result;

    if (work == NULL) {
        return 1;
    }
    if (work->campTask == 0) {
        func_003003F0(D_003AC9F8);
        return 1;
    }
    a = scrReadIntParameter(0);
    b = scrReadIntParameter(1);
    result = func_00234F18(work->campTask->resource, a, b);
    func_00101A80((s32)work->campTask, result);
    scrSetIntegerReturnValue(result);
    return 1;
}

s32 evtCommandSetWorldSlotStatusFlag(void) {
    s32 found;
    s32 i = 4;

    do {
        found = (s32)evtFindWorldObjectByIdAndKind(i, scrReadIntParameter(0));
        i++;
    } while (i < 10 && found == 0);
    if (found == 0) {
        found = func_00126200(scrReadIntParameter(0));
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
        found = func_00126200(scrReadIntParameter(0));
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
        unit = (u8 *)func_00126200(scrReadIntParameter(0));
        if (unit == NULL) {
            return 1;
        }
    }
    func_00220298(unit, bfWaitReadArgFloat(1));
    owner = ((EvtWorldUnit *)unit)->roomName;
    if (owner == 0) {
        return 1;
    }
    count = fldParseRoomNumberFromName(owner);
    if (count > 0) {
        if (bfWaitReadArgFloat(1) > 0.5f) {
            func_00121650(D_0032E3B0[4], D_0032E3B0[5] + 1, count, 1);
        } else {
            func_00121650(D_0032E3B0[4], D_0032E3B0[5] + 1, count, 0);
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
        unit = (u8 *)func_00126200(scrReadIntParameter(0));
        if (unit == NULL) {
            return 1;
        }
    }
    func_00220300(unit, scrReadIntParameter(1));
    owner = ((EvtWorldUnit *)unit)->roomName;
    if (owner == 0) {
        return 1;
    }
    count = fldParseRoomNumberFromName(owner);
    if (count > 0) {
        if (scrReadIntParameter(1) == 0) {
            func_00121650(D_0032E3B0[4], D_0032E3B0[5] + 1, count, 1);
        } else {
            func_00121650(D_0032E3B0[4], D_0032E3B0[5] + 1, count, 0);
        }
    }
    return 1;
}

s32 evtCommandSetUnitScaledValueFlag(void)
{
    s32 unitType;
    void *unit;

    unitType = 4;
    do {
        unit = evtFindWorldObjectByIdAndKind(unitType, scrReadIntParameter(0));
        unitType++;
    } while (unitType < 10 && unit == NULL);
    if (unit == NULL) {
        unit = (void *)func_00126200(scrReadIntParameter(0));
        if (unit == NULL) {
            return 1;
        }
    }
    func_00220458(unit, 1);
    return 1;
}

s32 evtCommandClearUnitScaledValueFlag(void)
{
    s32 unitType;
    void *unit;

    unitType = 4;
    do {
        unit = evtFindWorldObjectByIdAndKind(unitType, scrReadIntParameter(0));
        unitType++;
    } while (unitType < 10 && unit == NULL);
    if (unit == NULL) {
        unit = (void *)func_00126200(scrReadIntParameter(0));
        if (unit == NULL) {
            return 1;
        }
    }
    func_00220458(unit, 0);
    return 1;
}

s32 evtCommandMoveLightAlongPathOrWarn(void) {
    void *path = evtFindWorldObjectByIdAndKind(9, scrReadIntParameter(0));

    if (func_00220340(evtFindWorldObjectByIdAndKind(0x10, scrReadIntParameter(1)), path) != 0) {
        return 1;
    }
    func_003003F0(D_003ACA40);
    func_0010AC10(D_003ACA58);
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

