#include "common.h"

/* Script VM helpers (see script/scrCommonCommand.c for the convention). */
s32 func_0010D428(s32 idx);
void func_0010D5F0(s32 value);
s32 func_0010D680(void);
void *func_00223AA0(s32 type, s32 id);
void func_00220508(void *unit, s32 flag);
void func_00115B88(void *unit, s32 flag);
void func_00220560(s32 which, s32 value);
void func_001028E8(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_00102A18(void);
void func_00226830(s32 arg0, s32 arg1);
void func_002268C8(s32 arg0);
void func_00228710(void);
void func_00228728(void);
s32 func_002286C0(void);
s32 func_002286E8(void);
void func_002286F8(s32 arg0);
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
s32 func_0010FDC0(void);
s32 func_00110EB8(s32 arg0);
void func_0021FEC0(s32 arg0, s32 arg1);
void func_0021FE38(void);
s32 func_00220340(void *arg0, void *arg1);
f32 func_0010D4F0(s32 idx);
void func_00220298(s32 arg0, f32 arg1);
s32 func_0014DB78(void);
s32 func_00121650(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern u32 D_0032E3B0[];
extern u32 D_003BA904;
s32 func_00126200(s32 arg0);
void func_00220228(void);
void func_00220260(void);
void func_00220300(void *arg0, s32 arg1);
void func_00220458(void *arg0, s32 arg1);
u32 func_00123DE0(void);
void func_001109B8(s32 arg0, u32 arg1);
s32 func_0010D6A0(s32 arg0);
s32 func_00241B58(s32 arg0);
s32 func_00241B80(s32 arg0);
extern char D_003AC968[]; /* "BE ok! (%d)\n" */
void func_002E4C28(const char *msg);
void func_00241F78(s32 arg0, s32 arg1);
extern s32 D_0032E3C0[];
void func_0012C6C8(u32 arg0, s32 arg1);
extern u32 D_003BBDA8;

INCLUDE_ASM(const s32, "event/evtCommand", func_002260C0);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226180);

s32 func_002262A0(void)
{
    s32 id;
    void *unit;

    id = func_0010D428(0);
    unit = func_00223AA0(7, id);
    if (unit != NULL) {
        func_00220508(unit, 1);
        func_00115B88(unit, 1);
    }
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_002262F8);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226388);

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

INCLUDE_ASM(const s32, "event/evtCommand", func_002264B0);

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

s32 func_002267B0(void)
{
    s32 p0;

    p0 = func_0010D428(0);
    func_003003F0("call_event:%d\n", p0);
    func_001028E8(6, (s32)&p0, 4, 0);
    func_0010BDB8();
    return 1;
}

s32 func_00226800(void)
{
    if (func_0010D680() == 0) {
        func_00102A18();
    }
    return 0;
}

void func_00226830(s32 arg0, s32 arg1)
{
    s32 args[2];

    func_00141D98();
    func_0012C6C8(2, arg0);
    D_003BBDA8 = arg1;
    args[0] = 0;
    args[1] = arg0;
    func_001028E8(0xe, (s32)args, 8, arg1 > 0);
    func_0010BDB8();
}

s32 func_00226898(void)
{
    s32 p0;

    p0 = func_0010D428(0);
    func_00226830(p0, 0);
    return 1;
}

void func_002268C8(s32 arg0)
{
    s32 args[2];

    func_00141D18();
    func_0012C6C8(2, arg0);
    D_003BBDA8 = 0;
    args[0] = 0;
    args[1] = arg0;
    func_001028E8(0xe, (s32)args, 8, 1);
    func_0010BDB8();
}

s32 func_00226920(void)
{
    s32 p0;

    p0 = func_0010D428(0);
    func_002268C8(p0);
    return 1;
}

s32 func_00226948(void)
{
    s32 p0;
    s32 p1;

    p0 = func_0010D428(0);
    p1 = func_0010D428(1);
    func_00226830(p0, p1);
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
    s32 p0;
    s32 p1;
    s32 combined;

    p0 = func_0010D428(0);
    p1 = func_0010D428(1);
    combined = (p0 << 16) + p1;
    if (func_00110EB8(func_0010FDC0()) != combined) {
        p0 = func_0010D428(0);
        p1 = func_0010D428(1);
        func_0021FEC0(p0, p1);
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

s32 func_00226C38(void)
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

s32 func_00226CA8(void)
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

s32 func_00226D18(void)
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

INCLUDE_ASM(const s32, "event/evtCommand", func_00226E10);

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

INCLUDE_ASM(const s32, "event/evtCommand", func_00227090);

INCLUDE_ASM(const s32, "event/evtCommand", func_00227138);

INCLUDE_ASM(const s32, "event/evtCommand", func_00227210);

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
    func_0010D5F0(func_002286C0());
    return 1;
}

s32 func_00227770(void)
{
    func_0010D5F0(func_002286E8());
    return 1;
}

s32 func_00227798(void)
{
    func_002286F8(func_0010D428(0));
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_002277C0);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC968);

INCLUDE_ASM(const s32, "event/evtCommand", func_002278A0);

INCLUDE_ASM(const s32, "event/evtCommand", func_00227920);

INCLUDE_ASM(const s32, "event/evtCommand", func_002279A0);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC998);

INCLUDE_ASM(const s32, "event/evtCommand", func_00227A00);

s32 func_00227AD8(void)
{
    s32 p0;

    p0 = func_0010D428(0);
    evtPolygonMovieClearFlagBits(p0, 1);
    func_0010D5F0(p0);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_00227B18);

INCLUDE_ASM(const s32, "event/evtCommand", func_00227BB8);

INCLUDE_ASM(const s32, "event/evtCommand", func_00227C38);

INCLUDE_ASM(const s32, "event/evtCommand", func_00227CB8);

INCLUDE_ASM(const s32, "event/evtCommand", func_00227DD0);

s32 func_00227EE0(void)
{
    s32 i;
    void *unit;

    i = 4;
    do {
        unit = func_00223AA0(i, func_0010D428(0));
        i++;
    } while (i < 10 && unit == NULL);
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
    s32 i;
    void *unit;

    i = 4;
    do {
        unit = func_00223AA0(i, func_0010D428(0));
        i++;
    } while (i < 10 && unit == NULL);
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

INCLUDE_ASM(const s32, "event/evtCommand", func_00227FE0);





INCLUDE_RODATA(const s32, "event/evtCommand", D_003ACA78);

