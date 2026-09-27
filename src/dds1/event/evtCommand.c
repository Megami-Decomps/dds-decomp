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

INCLUDE_ASM(const s32, "event/evtCommand", func_00226430);

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

INCLUDE_ASM(const s32, "event/evtCommand", func_002266E0);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226730);

INCLUDE_ASM(const s32, "event/evtCommand", func_002267B0);

s32 func_00226800(void)
{
    if (func_0010D680() == 0) {
        func_00102A18();
    }
    return 0;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_00226830);

s32 func_00226898(void)
{
    s32 p0;

    p0 = func_0010D428(0);
    func_00226830(p0, 0);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_002268C8);

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

INCLUDE_ASM(const s32, "event/evtCommand", func_00226988);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226A08);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226A70);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226AA8);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226B10);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226B68);

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

INCLUDE_ASM(const s32, "event/evtCommand", func_00226C38);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226CA8);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226D18);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226D50);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226D98);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226E10);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226E98);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226F38);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226FC0);

INCLUDE_ASM(const s32, "event/evtCommand", func_00227050);

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

INCLUDE_ASM(const s32, "event/evtCommand", func_00227EE0);

INCLUDE_ASM(const s32, "event/evtCommand", func_00227F60);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003AC9F8);

INCLUDE_ASM(const s32, "event/evtCommand", func_00227FE0);

INCLUDE_RODATA(const s32, "event/evtCommand", D_003ACA78);

