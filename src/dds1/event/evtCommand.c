#include "common.h"

/* Script VM helpers (see script/scrCommonCommand.c for the convention). */
s32 func_0010D428(s32 idx);
void func_0010D5F0(s32 value);
s32 scrGetCommandTimer(void);
void *func_00223AA0(s32 type, s32 id);
void func_00220508(void *unit, s32 flag);
void effObjSetFlags(void *unit, s32 flag);
void func_00220560(s32 which, s32 value);
void func_001028E8(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_00102A18(void);
void evtSubmitEventRequest(s32 arg0, s32 arg1);
void evtSubmitEventRequestImmediate(s32 arg0);
void func_00228710(void);
void func_00228728(void);
s32 evtGetMirroredSolarPhase(void);
s32 evtGetSolarPhase(void);
void evtSetSolarPhase(s32 arg0);
void evtPolygonMovieClearFlagBits(s32 arg0, u32 bits);
void func_0010AC10(const char *fmt, ...);
void func_003003F0(const char *fmt, ...);
void func_0010BDB8(void);
void func_0021FE70(void);
void func_00115CD8(void *unit);
void func_00110928(void *unit);
void func_00141D18(void);
void func_00141D98(void);
void func_00141D40(void);
char *func_0010D5A8(s32 idx);
void func_00122F08(s32 arg0, s32 arg1, s32 arg2, char *arg3);
void func_00122FF0(s32 arg0, s32 arg1, s32 arg2, char *arg3);
s32 func_002D3EE8(void);
s32 dds3GetWorldSecondaryObject(void);
s32 func_00110EB8(s32 arg0);
void func_0021FEC0(s32 arg0, s32 arg1);
void func_0021FE38(void);
s32 func_00220340(void *arg0, void *arg1);
f32 func_0010D4F0(s32 idx);
void func_00220298(s32 arg0, f32 arg1);
s32 func_0014DB78(char *name);
s32 func_00121650(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern u32 D_0032E3B0[];
extern u32 D_003BA904;
s32 func_00126200(s32 arg0);
void func_00220228();
void func_00220260();
void func_00220300(void *arg0, s32 arg1);
void func_00220458(void *arg0, s32 arg1);
u32 func_00123DE0(void);
void func_001109B8(s32 arg0, u32 arg1);
s32 func_0010D6A0(void);
s32 evtFindTaskById(s32 arg0);
s32 func_00241B80(s32 arg0);
extern char D_003AC968[]; /* "BE ok! (%d)\n" */
extern char D_003AC978[];
extern char D_003AC988[]; /* "del BE (%d)..\n" */
extern char D_003ACA40[]; /* "LIGHT_PATH_MOVE error!\n" */
extern char D_003ACA58[]; /* "error: LIGHT_PATH_MOVE.\n" */
void func_002E4C28(const char *msg, ...);
void func_00241F78(s32 arg0, s32 arg1);
extern s32 D_0032E3C0[];
void func_0012C6C8(u32 arg0, s32 arg1);
extern u32 D_003BBDA8;

INCLUDE_ASM(const s32, "event/evtCommand", func_002260C0);

extern char D_003AC700[];
extern char D_003AC728[];
extern char D_003AC9F8[];
extern char D_003AC998[];
extern char D_003AC9E0[];
extern char D_003AC958[];
INCLUDE_ASM(const s32, "event/evtCommand", func_00226180);

s32 func_002262A0(void)
{
    s32 id;
    void *unit;

    id = func_0010D428(0);
    unit = func_00223AA0(7, id);
    if (unit != NULL) {
        func_00220508(unit, 1);
        effObjSetFlags(unit, 1);
    }
    return 1;
}

s32 func_002262F8(void) {
    void *path = func_00223AA0(7, func_0010D428(0));
    s32 found;
    s32 i;

    if (path == NULL) {
        return 1;
    }
    i = 4;
    do {
        found = (s32)func_00223AA0(i, func_0010D428(1));
        i++;
    } while (i < 10 && found == 0);
    if (found == 0) {
        return 1;
    }
    func_00115BB8(path, found);
    return 1;
}

s32 func_00226388(void) {
    void *path = func_00223AA0(7, func_0010D428(0));
    s32 found;
    s32 i;

    if (path == NULL) {
        return 1;
    }
    i = 4;
    do {
        found = (s32)func_00223AA0(i, func_0010D428(1));
        i++;
    } while (i < 10 && found == 0);
    if (found == 0) {
        return 1;
    }
    func_00115C80(path, found, func_0010D428(2));
    return 1;
}

s32 func_00226430(void)
{
    s32 id;
    void *unit;

    id = func_0010D428(0);
    unit = func_00223AA0(7, id);
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

    id = func_0010D428(0);
    unit = func_00223AA0(7, id);
    if (unit != NULL) {
        func_00220508(unit, 0);
    }
    return 1;
}

s32 func_002264B0(void) {
    void *path = func_00223AA0(7, func_0010D428(0));
    void *target;

    if (path == NULL) {
        func_0010AC10(D_003AC700);
        return 1;
    }
    target = func_00223AA0(0x10, func_0010D428(1));
    if (target == NULL) {
        func_0010AC10(D_003AC728);
        return 1;
    }
    func_00220340(target, path);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_00226540);

INCLUDE_ASM(const s32, "event/evtCommand", func_002265B8);

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

s32 func_002266E0(void)
{
    s32 args[2];

    args[0] = func_0010D428(0);
    args[1] = func_0010D428(1);
    func_001028E8(0x15, (s32)args, 8, 0);
    func_0010D5F0(0);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_00226730);

s32 evtCommandCallEvent(void)
{
    s32 eventId;

    eventId = func_0010D428(0);
    func_003003F0("call_event:%d\n", eventId);
    func_001028E8(6, (s32)&eventId, 4, 0);
    func_0010BDB8();
    return 1;
}

s32 func_00226800(void)
{
    if (scrGetCommandTimer() == 0) {
        func_00102A18();
    }
    return 0;
}

void evtSubmitEventRequest(s32 eventId, s32 mode)
{
    s32 args[2];

    func_00141D98();
    func_0012C6C8(2, eventId);
    D_003BBDA8 = mode;
    args[0] = 0;
    args[1] = eventId;
    func_001028E8(0xe, (s32)args, 8, mode > 0);
    func_0010BDB8();
}

s32 func_00226898(void)
{
    s32 eventId;

    eventId = func_0010D428(0);
    evtSubmitEventRequest(eventId, 0);
    return 1;
}

void evtSubmitEventRequestImmediate(s32 eventId)
{
    s32 args[2];

    func_00141D18();
    func_0012C6C8(2, eventId);
    D_003BBDA8 = 0;
    args[0] = 0;
    args[1] = eventId;
    func_001028E8(0xe, (s32)args, 8, 1);
    func_0010BDB8();
}

s32 func_00226920(void)
{
    s32 eventId;

    eventId = func_0010D428(0);
    evtSubmitEventRequestImmediate(eventId);
    return 1;
}

s32 func_00226948(void)
{
    s32 eventId;
    s32 mode;

    eventId = func_0010D428(0);
    mode = func_0010D428(1);
    evtSubmitEventRequest(eventId, mode);
    return 1;
}

s32 func_00226988(void)
{
    s32 p0;
    s32 p1;
    char *q;
    u8 buf[0xa0];

    func_00141D40();
    p0 = func_0010D428(0);
    p1 = func_0010D428(1);
    q = func_0010D5A8(2);
    func_00122F08((s32)buf, p0, p1, q);
    func_001028E8(5, (s32)buf, 0xa0, 0);
    func_0010BDB8();
    return 1;
}

s32 func_00226A08(void)
{
    s32 p0;
    char *q;
    u8 buf[0xa0];

    p0 = func_0010D428(0);
    q = func_0010D5A8(1);
    func_00122F08((s32)buf, D_0032E3C0[0], p0, q);
    func_001028E8(5, (s32)buf, 0xa0, 0);
    func_0010BDB8();
    return 1;
}

s32 func_00226A70(void)
{
    s32 p0;

    p0 = func_0010D428(0);
    func_001028E8(0x1c, (s32)&p0, 4, 0);
    return 1;
}

s32 func_00226AA8(void)
{
    s32 p0;
    char *q;
    u8 buf[0xa0];

    p0 = func_0010D428(0);
    q = func_0010D5A8(1);
    func_00122FF0((s32)buf, D_0032E3C0[0], p0, q);
    func_001028E8(5, (s32)buf, 0xa0, 0);
    func_0010BDB8();
    return 1;
}

s32 func_00226B10(void)
{
    s32 p0;
    s32 p1;

    func_002E4C28("FIELD_BE start\n");
    p0 = func_0010D428(0);
    p1 = func_0010D428(1);
    func_00241F78(p0, p1);
    func_002E4C28("FIELD_BE end\n");
    return 1;
}

s32 func_00226B68(void)
{
    s32 highPart;
    s32 lowPart;
    s32 targetKey;

    highPart = func_0010D428(0);
    lowPart = func_0010D428(1);
    targetKey = (highPart << 16) + lowPart;
    if (func_00110EB8(dds3GetWorldSecondaryObject()) != targetKey) {
        highPart = func_0010D428(0);
        lowPart = func_0010D428(1);
        func_0021FEC0(highPart, lowPart);
    }
    return 1;
}

s32 func_00226BD8(void)
{
    s32 p0;

    p0 = func_0010D428(0);
    func_00220560(0, p0);
    return 1;
}

s32 func_00226C08(void)
{
    s32 p0;

    p0 = func_0010D428(0);
    func_00220560(1, p0);
    return 1;
}

s32 evtCommandShutdownStage(void)
{
    func_0021FE70();
    while (func_002D3EE8()) {
    }
    func_0021FE38();
    while (func_002D3EE8()) {
    }
    D_003BA904 |= 0x2000000;
    func_003003F0("event stage shutdown !!\n");
    func_0010AC10("field shutdown.\n");
    return 1;
}

s32 evtCommandShutdownStageAlternate(void)
{
    func_0021FE70();
    while (func_002D3EE8()) {
    }
    func_0021FE38();
    while (func_002D3EE8()) {
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

s32 func_00226D50(void)
{
    func_0021FE70();
    while (func_002D3EE8()) {
    }
    func_003003F0("unit all clear2 !!\n");
    func_0010AC10("unit all clear2.\n");
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_00226D98);

s32 func_00226E10(void) {
    void *unit;

    if (func_0010D428(0) < 0) {
        unit = (void *)func_00123DE0();
    } else {
        unit = func_00223AA0(4, func_0010D428(0));
        *(u32 *)(*(u8 **)((u8 *)unit + 0x18) + 0x88) |= 1;
    }
    if (unit == NULL) {
        return 1;
    }
    func_001109B8(dds3GetWorldObject(), (u32)unit);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_00226E98);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226F38);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226FC0);

s32 func_00227050(void)
{
    s32 id;
    void *unit;

    id = func_0010D428(0);
    unit = func_00223AA0(4, id);
    if (unit == NULL) {
        return 1;
    }
    func_00110928(unit);
    return 1;
}

s32 func_00227090(void) {
    void *unit;
    f32 vec[4];

    memset(vec, 0, sizeof(vec));
    if (func_0010D428(0) < 0) {
        unit = (void *)func_00123DE0();
    } else {
        unit = func_00223AA0(4, func_0010D428(0));
    }
    if (unit == NULL) {
        return 1;
    }
    vec[0] = func_0010D4F0(1);
    vec[1] = func_0010D4F0(2);
    vec[2] = func_0010D4F0(3);
    effObjSetInnerFirstVec(unit, vec);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_00227138);

s32 func_00227210(void) {
    void *unit;
    f32 vec[4];

    if (func_0010D428(0) < 0) {
        unit = (void *)func_00123DE0();
    } else {
        unit = func_00223AA0(4, func_0010D428(0));
    }
    if (unit == NULL) {
        return 1;
    }
    vec[0] = func_0010D4F0(1);
    vec[1] = func_0010D4F0(2);
    vec[2] = func_0010D4F0(3);
    vec[3] = func_0010D4F0(4);
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
    func_00228710();
    return 1;
}

s32 func_00227728(void)
{
    func_00228728();
    return 1;
}

s32 func_00227748(void)
{
    func_0010D5F0(evtGetMirroredSolarPhase());
    return 1;
}

s32 func_00227770(void)
{
    func_0010D5F0(evtGetSolarPhase());
    return 1;
}

s32 func_00227798(void)
{
    evtSetSolarPhase(func_0010D428(0));
    return 1;
}

s32 func_002277C0(void) {
    s32 id = func_0010D428(0);
    u8 *work = (u8 *)func_0010D6A0();
    char *msg;

    if (work == NULL) {
        return 1;
    }
    if (evtFindTaskById(id) == 0) {
        msg = D_003AC958;
        func_0010AC10(msg, id);
        func_002E4C28(msg, id);
        func_00101A80(*(s32 *)(work + 0xE4), mnuCampCreateTask(id));
        return 0;
    }
    if (func_00241B80(id) == 2) {
        msg = D_003AC968;
        func_0010AC10(msg, id);
        func_002E4C28(msg, id);
        return 1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC968);

s32 func_002278A0(void) {
    s32 id = func_0010D428(0);
    u8 *work = (u8 *)func_0010D6A0();

    if (work == NULL) {
        return 1;
    }
    if (evtFindTaskById(id) != 0) {
        return 1;
    }
    func_0010AC10(D_003AC978, id);
    func_00101A80(*(s32 *)(work + 0xE4), mnuCampCreateTask(id));
    return 1;
}

s32 func_00227920(void) {
    s32 id = func_0010D428(0);
    s32 ok;

    if (func_0010D6A0() != 0 && evtFindTaskById(id) != 0 && func_00241B80(id) == 2) {
        func_0010AC10(D_003AC968, id);
        ok = 1;
    } else {
        ok = 0;
    }
    func_0010D5F0(ok);
    return 1;
}

s32 func_002279A0(void) {
    s32 id = func_0010D428(0);

    if (func_0010D6A0() == 0) {
        return 1;
    }
    if (evtFindTaskById(id) == 0) {
        return 1;
    }
    func_0010AC10(D_003AC988, id);
    campDestroyTaskById(id);
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC998);

s32 func_00227A00(void) {
    u8 *work = (u8 *)func_0010D6A0();
    s32 a;
    s32 b;
    s32 result;

    if (work == NULL) {
        return 1;
    }
    if (*(s32 *)(work + 0xE4) == 0) {
        func_003003F0(D_003AC998);
        return 1;
    }
    a = func_0010D428(0);
    b = func_0010D428(1);
    result = func_00234F18(*(s32 *)(*(u8 **)(work + 0xE4) + 0x20), a, b);
    func_0010AC10(D_003AC9E0, func_0010D428(0), func_0010D428(1));
    func_00101A80(*(s32 *)(work + 0xE4), result);
    evtPolygonMovieSetFlagBits(result, 1);
    func_0010D5F0(result);
    return 1;
}

s32 func_00227AD8(void)
{
    s32 movieId;

    movieId = func_0010D428(0);
    evtPolygonMovieClearFlagBits(movieId, 1);
    func_0010D5F0(movieId);
    return 1;
}

s32 func_00227B18(void) {
    u8 *work = (u8 *)func_0010D6A0();
    s32 a;
    s32 b;
    s32 result;

    if (work == NULL) {
        return 1;
    }
    if (*(s32 *)(work + 0xE4) == 0) {
        func_003003F0(D_003AC9F8);
        return 1;
    }
    a = func_0010D428(0);
    b = func_0010D428(1);
    result = func_00234F18(*(s32 *)(*(u8 **)(work + 0xE4) + 0x20), a, b);
    func_00101A80(*(s32 *)(work + 0xE4), result);
    func_0010D5F0(result);
    return 1;
}

s32 func_00227BB8(void) {
    s32 found;
    s32 i = 4;

    do {
        found = (s32)func_00223AA0(i, func_0010D428(0));
        i++;
    } while (i < 10 && found == 0);
    if (found == 0) {
        found = func_00126200(func_0010D428(0));
        if (found == 0) {
            return 1;
        }
    }
    func_00220228(found);
    return 1;
}

s32 func_00227C38(void) {
    s32 found;
    s32 i = 4;

    do {
        found = (s32)func_00223AA0(i, func_0010D428(0));
        i++;
    } while (i < 10 && found == 0);
    if (found == 0) {
        found = func_00126200(func_0010D428(0));
        if (found == 0) {
            return 1;
        }
    }
    func_00220260(found);
    return 1;
}

s32 func_00227CB8(void) {
    u8 *unit;
    s32 count;
    char *owner;
    s32 i = 4;

    do {
        unit = (u8 *)func_00223AA0(i, func_0010D428(0));
        i++;
    } while (i < 10 && unit == NULL);
    if (unit == NULL) {
        unit = (u8 *)func_00126200(func_0010D428(0));
        if (unit == NULL) {
            return 1;
        }
    }
    func_00220298(unit, func_0010D4F0(1));
    owner = *(char **)(unit + 8);
    if (owner == 0) {
        return 1;
    }
    count = func_0014DB78(owner);
    if (count > 0) {
        if (func_0010D4F0(1) > 0.5f) {
            func_00121650(D_0032E3B0[4], D_0032E3B0[5] + 1, count, 1);
        } else {
            func_00121650(D_0032E3B0[4], D_0032E3B0[5] + 1, count, 0);
        }
    }
    return 1;
}

s32 func_00227DD0(void) {
    u8 *unit;
    s32 count;
    char *owner;
    s32 i = 4;

    do {
        unit = (u8 *)func_00223AA0(i, func_0010D428(0));
        i++;
    } while (i < 10 && unit == NULL);
    if (unit == NULL) {
        unit = (u8 *)func_00126200(func_0010D428(0));
        if (unit == NULL) {
            return 1;
        }
    }
    func_00220300(unit, func_0010D428(1));
    owner = *(char **)(unit + 8);
    if (owner == 0) {
        return 1;
    }
    count = func_0014DB78(owner);
    if (count > 0) {
        if (func_0010D428(1) == 0) {
            func_00121650(D_0032E3B0[4], D_0032E3B0[5] + 1, count, 1);
        } else {
            func_00121650(D_0032E3B0[4], D_0032E3B0[5] + 1, count, 0);
        }
    }
    return 1;
}

s32 func_00227EE0(void)
{
    s32 unitType;
    void *unit;

    unitType = 4;
    do {
        unit = func_00223AA0(unitType, func_0010D428(0));
        unitType++;
    } while (unitType < 10 && unit == NULL);
    if (unit == NULL) {
        unit = (void *)func_00126200(func_0010D428(0));
        if (unit == NULL) {
            return 1;
        }
    }
    func_00220458(unit, 1);
    return 1;
}

s32 func_00227F60(void)
{
    s32 unitType;
    void *unit;

    unitType = 4;
    do {
        unit = func_00223AA0(unitType, func_0010D428(0));
        unitType++;
    } while (unitType < 10 && unit == NULL);
    if (unit == NULL) {
        unit = (void *)func_00126200(func_0010D428(0));
        if (unit == NULL) {
            return 1;
        }
    }
    func_00220458(unit, 0);
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC9F8);

s32 func_00227FE0(void) {
    void *path = func_00223AA0(9, func_0010D428(0));

    if (func_00220340(func_00223AA0(0x10, func_0010D428(1)), path) != 0) {
        return 1;
    }
    func_003003F0(D_003ACA40);
    func_0010AC10(D_003ACA58);
    return 1;
}


