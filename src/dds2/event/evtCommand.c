#include "common.h"

extern s32 scrGetCommandTimer(void);

/* Script VM helpers (see script/scrCommonCommand.c for the convention). */
s32 func_0010D650(s32 idx);

void *evtFindWorldObjectByIdAndKind(s32 type, s32 id);

void func_00115F40(void *unit);

void func_0010D818(s32 value);

void func_001027D8(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void func_0035B6E0(const char *fmt, ...);

void func_0010BFE0(void);

void evtSubmitEventRequest(s32 arg0, s32 arg1);

void func_00144F60(void);

void func_0012EBF8(u32 arg0, s32 arg1);

extern u32 D_004371E8;

void evtSubmitEventRequestImmediate(s32 arg0);

void func_00144EE0(void);

void func_00144F08(void);

char *func_0010D7D0(s32 idx);

void fldInitializeSequenceAndResetFlags(s32 arg0, s32 arg1, s32 arg2, char *arg3);

extern s32 D_00389780[];

void fldInitializeAlternateSequence(s32 arg0, s32 arg1, s32 arg2, char *arg3);

void func_0033DAD8(const char *msg);

void func_0025D390(s32 arg0, s32 arg1);

s32 dds3GetWorldSecondaryObject(void);

s32 dds3GetWorldObjectValue(s32 arg0);

void func_0023AA30(s32 arg0, s32 arg1);

void func_0010AE38(const char *fmt, ...);

void func_0023A9E0(void);

s32 sdfGraphHasPendingWorkInterruptSafe(void);

void evtDestroyWorldSecondaryNode(void);

extern u32 D_00435CD4;

void func_00110B50(void *unit);

s32 func_001287B8(s32 arg0);

void func_0023AFC8(void *arg0, s32 arg1);

void func_0023B078(void *unit, s32 flag);

void effObjSetFlags(void *unit, s32 flag);

s32 evtGetMirroredSolarPhase(void);

s32 evtGetRawSolarPhase(void);

void evtPolygonMovieClearFlagBits(s32 arg0, u32 bits);

u32 func_00125F38(void);

void func_00110BE0(s32 arg0, u32 arg1);

f32 func_0010D718(s32 idx);

s32 func_0010D8C8(void);

s32 evtFindTaskById(s32 arg0);

s32 evtGetTaskValueWord(s32 arg0);

extern char D_00421ED8[]; /* "BE ok! (%d)\n" */

typedef struct EvtCampTask {
    u8 pad00[0x20];
    s32 resource;
} EvtCampTask;

typedef struct EvtCommandWork {
    u8 pad00[0xE4];
    EvtCampTask *campTask;
} EvtCommandWork;

extern char D_00421F68[];

void evtSetWorldSlotStatusFlag();

void evtClearWorldSlotStatusFlag();

void func_0023AE08(s32 arg0, f32 arg1);

s32 func_00154FA0(char *name);

s32 func_001235E8(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

extern u32 D_00389770[];

void func_0023AE70(void *arg0, s32 arg1);

s32 func_0023AEB0(void *arg0, void *arg1);

extern char D_00421FB0[]; /* "LIGHT_PATH_MOVE error!\n" */

extern char D_00421FC8[]; /* "error: LIGHT_PATH_MOVE.\n" */

INCLUDE_ASM(const s32, "event/evtCommand", func_00240D20);

INCLUDE_ASM(const s32, "event/evtCommand", func_00240DE0);

s32 evtCommandEnablePathUnit(void)
{
    s32 id;
    void *unit;

    id = func_0010D650(0);
    unit = evtFindWorldObjectByIdAndKind(7, id);
    if (unit != NULL) {
        func_0023B078(unit, 1);
        effObjSetFlags(unit, 1);
    }
    return 1;
}

s32 func_00240F58(void) {
    void *path = evtFindWorldObjectByIdAndKind(7, func_0010D650(0));
    s32 found;
    s32 i;

    if (path == NULL) {
        return 1;
    }
    i = 4;
    do {
        found = (s32)evtFindWorldObjectByIdAndKind(i, func_0010D650(1));
        i++;
    } while (i < 10 && found == 0);
    if (found == 0) {
        return 1;
    }
    func_00115E20(path, found);
    return 1;
}

s32 func_00240FE8(void) {
    void *path = evtFindWorldObjectByIdAndKind(7, func_0010D650(0));
    s32 found;
    s32 i;

    if (path == NULL) {
        return 1;
    }
    i = 4;
    do {
        found = (s32)evtFindWorldObjectByIdAndKind(i, func_0010D650(1));
        i++;
    } while (i < 10 && found == 0);
    if (found == 0) {
        return 1;
    }
    func_00115EE8(path, found, func_0010D650(2));
    return 1;
}

s32 func_00241090(void)
{
    s32 id;
    void *unit;

    id = func_0010D650(0);
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

    id = func_0010D650(0);
    unit = evtFindWorldObjectByIdAndKind(7, id);
    if (unit != NULL) {
        func_0023B078(unit, 0);
    }
    return 1;
}

s32 func_00241110(void) {
    void *effect;
    void *path;

    effect = evtFindWorldObjectByIdAndKind(7, func_0010D650(0));
    if (effect == NULL) {
        func_0010AE38("EFFECT_PATH_MOVE not found eff!\n");
        return 1;
    }
    path = evtFindWorldObjectByIdAndKind(0x10, func_0010D650(1));
    if (path == NULL) {
        func_0010AE38("EFFECT_PATH_MOVE not found path!\n", effect);
        return 1;
    }
    func_0023AEB0(path, effect);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_002411A0);

INCLUDE_ASM(const s32, "event/evtCommand", func_00241218);

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

s32 func_00241340(void)
{
    s32 args[2];

    args[0] = func_0010D650(0);
    args[1] = func_0010D650(1);
    func_001027D8(0x15, (s32)args, 8, 0);
    func_0010D818(0);
    return 1;
}

extern void fldSetDeferredFieldCommand(s32 a, s32 b);

s32 func_00241390(void) {
    s32 a = func_0010D650(0);
    s32 b = func_0010D650(1);
    s32 c = func_0010D650(2);
    s32 d;

    func_0035B6E0("CALL_NEXT(%d,%d,%d)\n", a, b, c);
    d = func_0010D650(1);
    fldSetDeferredFieldCommand(d, func_0010D650(2));
    return 1;
}

s32 evtCommandCallEvent(void)
{
    s32 p0;

    p0 = func_0010D650(0);
    func_0035B6E0("call_event:%d\n", p0);
    func_001027D8(6, (s32)&p0, 4, 0);
    func_0010BFE0();
    return 1;
}

u32 func_00241460(void) {
    s64 timer;

    timer = scrGetCommandTimer();
    if (timer == 0) {
        func_00102908();
    }
    return 0;
}

void evtSubmitEventRequest(s32 eventId, s32 requestMode)
{
    s32 args[2];

    func_00144F60();
    func_0012EBF8(2, eventId);
    D_004371E8 = requestMode;
    args[0] = 0;
    args[1] = eventId;
    func_001027D8(0xe, (s32)args, 8, requestMode > 0);
    func_0010BFE0();
}

s32 evtCommandSubmitEvent(void)
{
    s32 p0;

    p0 = func_0010D650(0);
    evtSubmitEventRequest(p0, 0);
    return 1;
}

void evtSubmitEventRequestImmediate(s32 eventId)
{
    s32 args[2];

    func_00144EE0();
    func_0012EBF8(2, eventId);
    D_004371E8 = 0;
    args[0] = 0;
    args[1] = eventId;
    func_001027D8(0xe, (s32)args, 8, 1);
    func_0010BFE0();
}

s32 evtCommandSubmitEventImmediate(void)
{
    s32 p0;

    p0 = func_0010D650(0);
    evtSubmitEventRequestImmediate(p0);
    return 1;
}

s32 evtCommandSubmitEventWithMode(void)
{
    s32 p0;
    s32 p1;

    p0 = func_0010D650(0);
    p1 = func_0010D650(1);
    evtSubmitEventRequest(p0, p1);
    return 1;
}

s32 func_002415E8(void)
{
    s32 p0;
    s32 p1;
    char *q;
    u8 buf[0xa0];

    func_00144F08();
    p0 = func_0010D650(0);
    p1 = func_0010D650(1);
    q = func_0010D7D0(2);
    fldInitializeSequenceAndResetFlags((s32)buf, p0, p1, q);
    func_001027D8(5, (s32)buf, 0xa0, 0);
    func_0010BFE0();
    return 1;
}

s32 func_00241668(void)
{
    s32 p0;
    char *q;
    u8 buf[0xa0];

    p0 = func_0010D650(0);
    q = func_0010D7D0(1);
    fldInitializeSequenceAndResetFlags((s32)buf, D_00389780[0], p0, q);
    func_001027D8(5, (s32)buf, 0xa0, 0);
    func_0010BFE0();
    return 1;
}

s32 func_002416D0(void)
{
    s32 p0;

    p0 = func_0010D650(0);
    func_001027D8(0x1c, (s32)&p0, 4, 0);
    return 1;
}

s32 func_00241708(void)
{
    s32 p0;
    char *q;
    u8 buf[0xa0];

    p0 = func_0010D650(0);
    q = func_0010D7D0(1);
    fldInitializeAlternateSequence((s32)buf, D_00389780[0], p0, q);
    func_001027D8(5, (s32)buf, 0xa0, 0);
    func_0010BFE0();
    return 1;
}

s32 evtCommandDispatchFieldBE(void)
{
    s32 p0;
    s32 p1;

    func_0033DAD8("FIELD_BE start\n");
    p0 = func_0010D650(0);
    p1 = func_0010D650(1);
    func_0025D390(p0, p1);
    func_0033DAD8("FIELD_BE end\n");
    return 1;
}

s32 func_002417C8(void)
{
    s32 p0;
    s32 p1;
    s32 combined;

    p0 = func_0010D650(0);
    p1 = func_0010D650(1);
    combined = (p0 << 16) + p1;
    if (dds3GetWorldObjectValue(dds3GetWorldSecondaryObject()) != combined) {
        p0 = func_0010D650(0);
        p1 = func_0010D650(1);
        func_0023AA30(p0, p1);
    }
    return 1;
}

u32 func_00241838(void) {
    u64 temp_v0;

    temp_v0 = func_0010D650(0);
    func_0023B0D0(0, temp_v0);
    return 1;
}

u32 func_00241868(void) {
    u64 temp_v0;

    temp_v0 = func_0010D650(0);
    func_0023B0D0(1, temp_v0);
    return 1;
}

s32 evtCommandShutdownStage(void)
{
    func_0023A9E0();
    while (sdfGraphHasPendingWorkInterruptSafe()) {
    }
    evtDestroyWorldSecondaryNode();
    while (sdfGraphHasPendingWorkInterruptSafe()) {
    }
    D_00435CD4 |= 0x2000000;
    func_0035B6E0("event stage shutdown !!\n");
    func_0010AE38("field shutdown.\n");
    return 1;
}

s32 evtCommandShutdownStageAlternate(void)
{
    func_0023A9E0();
    while (sdfGraphHasPendingWorkInterruptSafe()) {
    }
    evtDestroyWorldSecondaryNode();
    while (sdfGraphHasPendingWorkInterruptSafe()) {
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

s32 func_002419B0(void)
{
    func_0023A9E0();
    while (sdfGraphHasPendingWorkInterruptSafe()) {
    }
    func_0035B6E0("unit all clear2 !!\n");
    func_0010AE38("unit all clear2.\n");
    return 1;
}

s32 func_002419F8(void) {
    void *unit;

    if (func_0010D650(0) < 0) {
        unit = (void *)func_00125F38();
    } else {
        unit = evtFindWorldObjectByIdAndKind(4, func_0010D650(0));
    }
    if (unit == NULL) {
        return 1;
    }
    func_00110BE0(dds3GetWorldObject(), (u32)unit);
    return 1;
}

s32 func_00241A70(void) {
    void *unit;

    if (func_0010D650(0) < 0) {
        unit = (void *)func_00125F38();
    } else {
        unit = evtFindWorldObjectByIdAndKind(4, func_0010D650(0));
        *(u32 *)(*(u8 **)((u8 *)unit + 0x18) + 0x88) |= 1;
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

s32 func_00241C20(void) {
    f32 pos[4];
    f32 rot[4];
    s32 world;

    memset(pos, 0, 0x10);
    memset(rot, 0, 0x10);
    rot[3] = 1.0f;
    world = dds3AdvanceWorldCounter();
    effObjSetInnerFloat(func_00112E30(world, pos, rot), 1.0f);
    func_0010D818(world);
    return 1;
}

s32 func_00241CB0(void)
{
    s32 id;
    void *unit;

    id = func_0010D650(0);
    unit = evtFindWorldObjectByIdAndKind(4, id);
    if (unit == NULL) {
        return 1;
    }
    func_00110B50(unit);
    return 1;
}

s32 func_00241CF0(void) {
    void *unit;
    f32 vec[4];

    memset(vec, 0, sizeof(vec));
    if (func_0010D650(0) < 0) {
        unit = (void *)func_00125F38();
    } else {
        unit = evtFindWorldObjectByIdAndKind(4, func_0010D650(0));
    }
    if (unit == NULL) {
        return 1;
    }
    vec[0] = func_0010D718(1);
    vec[1] = func_0010D718(2);
    vec[2] = func_0010D718(3);
    effObjSetInnerFirstVec(unit, vec);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_00241D98);

s32 func_00241E70(void) {
    void *unit;
    f32 vec[4];

    if (func_0010D650(0) < 0) {
        unit = (void *)func_00125F38();
    } else {
        unit = evtFindWorldObjectByIdAndKind(4, func_0010D650(0));
    }
    if (unit == NULL) {
        return 1;
    }
    vec[0] = func_0010D718(1);
    vec[1] = func_0010D718(2);
    vec[2] = func_0010D718(3);
    vec[3] = func_0010D718(4);
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
    func_0010D818(evtGetMirroredSolarPhase());
    return 1;
}

s32 evtCommandReadSolarPhase(void)
{
    func_0010D818(evtGetRawSolarPhase());
    return 1;
}

u32 evtCommandSetSolarPhase(void) {
    u64 temp_v0;

    temp_v0 = func_0010D650(0);
    evtSetSolarPhase(temp_v0);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", evtCommandWaitForCampTask);

INCLUDE_RODATA(const s32, "event/evtCommand", D_00421ED8);

extern s32 mnuCampCreateTask(s32 id);

s32 evtCommandStartCampTaskIfAbsent(void) {
    s32 id = func_0010D650(0);
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
    s32 id = func_0010D650(0);
    s32 ok;

    if (func_0010D8C8() != 0 && evtFindTaskById(id) != 0 && evtGetTaskValueWord(id) == 2) {
        func_0010AE38(D_00421ED8, id);
        ok = 1;
    } else {
        ok = 0;
    }
    func_0010D818(ok);
    return 1;
}

extern void mnuCampDestroyTaskById(s32 id);

s32 evtCommandDestroyCampTask(void) {
    s32 id = func_0010D650(0);

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

INCLUDE_RODATA(const s32, "event/evtCommand", D_00421F08);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommandStartPolygonMovie);

s32 evtCommandClearPolygonMovieFlag(void)
{
    s32 p0;

    p0 = func_0010D650(0);
    evtPolygonMovieClearFlagBits(p0, 1);
    func_0010D818(p0);
    return 1;
}

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
    a = func_0010D650(0);
    b = func_0010D650(1);
    result = func_0024FCB8(work->campTask->resource, a, b);
    func_00101968((s32)work->campTask, result);
    func_0010D818(result);
    return 1;
}

s32 func_00242818(void) {
    s32 found;
    s32 i = 4;

    do {
        found = (s32)evtFindWorldObjectByIdAndKind(i, func_0010D650(0));
        i++;
    } while (i < 10 && found == 0);
    if (found == 0) {
        found = func_001287B8(func_0010D650(0));
        if (found == 0) {
            return 1;
        }
    }
    evtSetWorldSlotStatusFlag(found);
    return 1;
}

s32 func_00242898(void) {
    s32 found;
    s32 i = 4;

    do {
        found = (s32)evtFindWorldObjectByIdAndKind(i, func_0010D650(0));
        i++;
    } while (i < 10 && found == 0);
    if (found == 0) {
        found = func_001287B8(func_0010D650(0));
        if (found == 0) {
            return 1;
        }
    }
    evtClearWorldSlotStatusFlag(found);
    return 1;
}

s32 func_00242918(void) {
    u8 *unit;
    s32 count;
    char *owner;
    s32 i = 4;

    do {
        unit = (u8 *)evtFindWorldObjectByIdAndKind(i, func_0010D650(0));
        i++;
    } while (i < 10 && unit == NULL);
    if (unit == NULL) {
        unit = (u8 *)func_001287B8(func_0010D650(0));
        if (unit == NULL) {
            return 1;
        }
    }
    func_0023AE08(unit, func_0010D718(1));
    owner = *(char **)(unit + 8);
    if (owner == 0) {
        return 1;
    }
    count = func_00154FA0(owner);
    if (count > 0) {
        if (func_0010D718(1) > 0.5f) {
            func_001235E8(D_00389770[4], D_00389770[5] + 1, count, 1);
        } else {
            func_001235E8(D_00389770[4], D_00389770[5] + 1, count, 0);
        }
    }
    return 1;
}

s32 func_00242A30(void) {
    u8 *unit;
    s32 count;
    char *owner;
    s32 i = 4;

    do {
        unit = (u8 *)evtFindWorldObjectByIdAndKind(i, func_0010D650(0));
        i++;
    } while (i < 10 && unit == NULL);
    if (unit == NULL) {
        unit = (u8 *)func_001287B8(func_0010D650(0));
        if (unit == NULL) {
            return 1;
        }
    }
    func_0023AE70(unit, func_0010D650(1));
    owner = *(char **)(unit + 8);
    if (owner == 0) {
        return 1;
    }
    count = func_00154FA0(owner);
    if (count > 0) {
        if (func_0010D650(1) == 0) {
            func_001235E8(D_00389770[4], D_00389770[5] + 1, count, 1);
        } else {
            func_001235E8(D_00389770[4], D_00389770[5] + 1, count, 0);
        }
    }
    return 1;
}

s32 func_00242B40(void)
{
    s32 i;
    void *unit;

    i = 4;
    do {
        unit = evtFindWorldObjectByIdAndKind(i, func_0010D650(0));
        i++;
    } while (i < 10 && unit == NULL);
    if (unit == NULL) {
        unit = (void *)func_001287B8(func_0010D650(0));
        if (unit == NULL) {
            return 1;
        }
    }
    func_0023AFC8(unit, 1);
    return 1;
}

s32 func_00242BC0(void)
{
    s32 i;
    void *unit;

    i = 4;
    do {
        unit = evtFindWorldObjectByIdAndKind(i, func_0010D650(0));
        i++;
    } while (i < 10 && unit == NULL);
    if (unit == NULL) {
        unit = (void *)func_001287B8(func_0010D650(0));
        if (unit == NULL) {
            return 1;
        }
    }
    func_0023AFC8(unit, 0);
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtCommand", D_00421F68);

s32 func_00242C40(void) {
    void *path = evtFindWorldObjectByIdAndKind(9, func_0010D650(0));

    if (func_0023AEB0(evtFindWorldObjectByIdAndKind(0x10, func_0010D650(1)), path) != 0) {
        return 1;
    }
    func_0035B6E0(D_00421FB0);
    func_0010AE38(D_00421FC8);
    return 1;
}



