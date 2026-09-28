#include "common.h"

extern s32 scrGetCommandTimer(void);

/* Script VM helpers (see script/scrCommonCommand.c for the convention). */
s32 func_0010D650(s32 idx);

void *func_0023E6D8(s32 type, s32 id);

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

void func_00124EB8(s32 arg0, s32 arg1, s32 arg2, char *arg3);

extern s32 D_00389780[];

void func_00124FA0(s32 arg0, s32 arg1, s32 arg2, char *arg3);

void func_0033DAD8(const char *msg);

void func_0025D390(s32 arg0, s32 arg1);

s32 dds3GetWorldSecondaryObject(void);

s32 func_001110E0(s32 arg0);

void func_0023AA30(s32 arg0, s32 arg1);

void func_0010AE38(const char *fmt, ...);

void func_0023A9E0(void);

s32 func_0032CD98(void);

void func_0023A9A8(void);

extern u32 D_00435CD4;

void func_00110B50(void *unit);

s32 func_001287B8(s32 arg0);

void func_0023AFC8(void *arg0, s32 arg1);

void func_0023B078(void *unit, s32 flag);

void effObjSetFlags(void *unit, s32 flag);

s32 evtGetMirroredSolarPhase(void);

s32 evtGetRawSolarPhase(void);

void evtPolygonMovieClearFlagBits(s32 arg0, u32 bits);

INCLUDE_ASM(const s32, "event/evtCommand", func_00240D20);

INCLUDE_ASM(const s32, "event/evtCommand", func_00240DE0);

s32 func_00240F00(void)
{
    s32 id;
    void *unit;

    id = func_0010D650(0);
    unit = func_0023E6D8(7, id);
    if (unit != NULL) {
        func_0023B078(unit, 1);
        effObjSetFlags(unit, 1);
    }
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_00240F58);

INCLUDE_ASM(const s32, "event/evtCommand", func_00240FE8);

s32 func_00241090(void)
{
    s32 id;
    void *unit;

    id = func_0010D650(0);
    unit = func_0023E6D8(7, id);
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
    unit = func_0023E6D8(7, id);
    if (unit != NULL) {
        func_0023B078(unit, 0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_00241110);

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

INCLUDE_ASM(const s32, "event/evtCommand", func_00241390);

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

s32 func_002414F8(void)
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

s32 func_00241580(void)
{
    s32 p0;

    p0 = func_0010D650(0);
    evtSubmitEventRequestImmediate(p0);
    return 1;
}

s32 func_002415A8(void)
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
    func_00124EB8((s32)buf, p0, p1, q);
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
    func_00124EB8((s32)buf, D_00389780[0], p0, q);
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
    func_00124FA0((s32)buf, D_00389780[0], p0, q);
    func_001027D8(5, (s32)buf, 0xa0, 0);
    func_0010BFE0();
    return 1;
}

s32 func_00241770(void)
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
    if (func_001110E0(dds3GetWorldSecondaryObject()) != combined) {
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
    while (func_0032CD98()) {
    }
    func_0023A9A8();
    while (func_0032CD98()) {
    }
    D_00435CD4 |= 0x2000000;
    func_0035B6E0("event stage shutdown !!\n");
    func_0010AE38("field shutdown.\n");
    return 1;
}

s32 evtCommandShutdownStageAlternate(void)
{
    func_0023A9E0();
    while (func_0032CD98()) {
    }
    func_0023A9A8();
    while (func_0032CD98()) {
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
    while (func_0032CD98()) {
    }
    func_0035B6E0("unit all clear2 !!\n");
    func_0010AE38("unit all clear2.\n");
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_002419F8);

INCLUDE_ASM(const s32, "event/evtCommand", func_00241A70);

INCLUDE_ASM(const s32, "event/evtCommand", func_00241AF8);

INCLUDE_ASM(const s32, "event/evtCommand", func_00241B98);

INCLUDE_ASM(const s32, "event/evtCommand", func_00241C20);

s32 func_00241CB0(void)
{
    s32 id;
    void *unit;

    id = func_0010D650(0);
    unit = func_0023E6D8(4, id);
    if (unit == NULL) {
        return 1;
    }
    func_00110B50(unit);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_00241CF0);

INCLUDE_ASM(const s32, "event/evtCommand", func_00241D98);

INCLUDE_ASM(const s32, "event/evtCommand", func_00241E70);

INCLUDE_ASM(const s32, "event/evtCommand", func_00241F10);

INCLUDE_RODATA(const s32, "event/evtCommand", D_00421E58);

INCLUDE_RODATA(const s32, "event/evtCommand", D_00421E68);

INCLUDE_RODATA(const s32, "event/evtCommand", D_00421E78);

INCLUDE_ASM(const s32, "event/evtCommand", func_00242100);

INCLUDE_ASM(const s32, "event/evtCommand", func_002422A8);

u32 func_00242368(void) {
    func_00243380();
    return 1;
}

u32 func_00242388(void) {
    func_00243398();
    return 1;
}

s32 func_002423A8(void)
{
    func_0010D818(evtGetMirroredSolarPhase());
    return 1;
}

s32 func_002423D0(void)
{
    func_0010D818(evtGetRawSolarPhase());
    return 1;
}

u32 func_002423F8(void) {
    u64 temp_v0;

    temp_v0 = func_0010D650(0);
    evtSetSolarPhase(temp_v0);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_00242420);

INCLUDE_RODATA(const s32, "event/evtCommand", D_00421ED8);

INCLUDE_ASM(const s32, "event/evtCommand", func_00242500);

INCLUDE_ASM(const s32, "event/evtCommand", func_00242580);

INCLUDE_ASM(const s32, "event/evtCommand", func_00242600);

INCLUDE_RODATA(const s32, "event/evtCommand", D_00421F08);

INCLUDE_ASM(const s32, "event/evtCommand", func_00242660);

s32 func_00242738(void)
{
    s32 p0;

    p0 = func_0010D650(0);
    evtPolygonMovieClearFlagBits(p0, 1);
    func_0010D818(p0);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_00242778);

INCLUDE_ASM(const s32, "event/evtCommand", func_00242818);

INCLUDE_ASM(const s32, "event/evtCommand", func_00242898);

INCLUDE_ASM(const s32, "event/evtCommand", func_00242918);

INCLUDE_ASM(const s32, "event/evtCommand", func_00242A30);

s32 func_00242B40(void)
{
    s32 i;
    void *unit;

    i = 4;
    do {
        unit = func_0023E6D8(i, func_0010D650(0));
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
        unit = func_0023E6D8(i, func_0010D650(0));
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

INCLUDE_ASM(const s32, "event/evtCommand", func_00242C40);



