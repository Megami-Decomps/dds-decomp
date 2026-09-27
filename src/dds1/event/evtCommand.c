#include "common.h"

extern u64 func_002286E8(void);

extern u64 func_002286C0(void);

extern s32 func_0010D680(void);

extern u64 func_0010D428(u64);
extern s64 func_00223AA0(u64, u64);

INCLUDE_ASM(const s32, "event/evtCommand", func_002260C0);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226180);

u32 func_002262A0(void) {
    u64 temp_v0;
    s64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_00223AA0(7, temp_v0);
    if (temp_v1 != 0) {
        func_00220508(temp_v1, 1);
        func_00115B88(temp_v1, 1);
    }
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_002262F8);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226388);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226430);

u32 func_00226470(void) {
    u64 temp_v0;
    s64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_00223AA0(7, temp_v0);
    if (temp_v1 != 0) {
        func_00220508(temp_v1, 0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_002264B0);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226540);

INCLUDE_ASM(const s32, "event/evtCommand", func_002265B8);

u32 func_00226650(void) {
    func_001028E8(2, 0, 0, 0);
    return 1;
}

u32 func_00226680(void) {
    func_001028E8(0xc, 0, 0, 0);
    return 1;
}

u32 func_002266B0(void) {
    func_001028E8(4, 0, 0, 0);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_002266E0);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226730);

INCLUDE_ASM(const s32, "event/evtCommand", func_002267B0);

u32 func_00226800(void) {
    s64 temp_v0;

    temp_v0 = func_0010D680();
    if (temp_v0 == 0) {
        func_00102A18();
    }
    return 0;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_00226830);

u32 func_00226898(void) {
    u64 temp_v0;

    temp_v0 = func_0010D428(0);
    func_00226830(temp_v0, 0);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_002268C8);

u32 func_00226920(void) {
    u64 temp_v0;

    temp_v0 = func_0010D428(0);
    func_002268C8(temp_v0);
    return 1;
}

u32 func_00226948(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    func_00226830(temp_v0, temp_v1);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_00226988);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226A08);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226A70);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226AA8);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226B10);

INCLUDE_ASM(const s32, "event/evtCommand", func_00226B68);

u32 func_00226BD8(void) {
    u64 temp_v0;

    temp_v0 = func_0010D428(0);
    func_00220560(0, temp_v0);
    return 1;
}

u32 func_00226C08(void) {
    u64 temp_v0;

    temp_v0 = func_0010D428(0);
    func_00220560(1, temp_v0);
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

INCLUDE_ASM(const s32, "event/evtCommand", func_002274A0);

INCLUDE_ASM(const s32, "event/evtCommand", func_00227648);

u32 func_00227708(void) {
    func_00228710();
    return 1;
}

u32 func_00227728(void) {
    func_00228728();
    return 1;
}

u32 func_00227748(void) {
    u64 temp_v0;

    temp_v0 = func_002286C0();
    func_0010D5F0(temp_v0);
    return 1;
}

u32 func_00227770(void) {
    u64 temp_v0;

    temp_v0 = func_002286E8();
    func_0010D5F0(temp_v0);
    return 1;
}

u32 func_00227798(void) {
    u64 temp_v0;

    temp_v0 = func_0010D428(0);
    func_002286F8(temp_v0);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_002277C0);

INCLUDE_ASM(const s32, "event/evtCommand", func_002278A0);

INCLUDE_ASM(const s32, "event/evtCommand", func_00227920);

INCLUDE_ASM(const s32, "event/evtCommand", func_002279A0);

INCLUDE_ASM(const s32, "event/evtCommand", func_00227A00);

u32 func_00227AD8(void) {
    u64 temp_v0;

    temp_v0 = func_0010D428(0);
    func_002342C0(temp_v0, 1);
    func_0010D5F0(temp_v0);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_00227B18);

INCLUDE_ASM(const s32, "event/evtCommand", func_00227BB8);

INCLUDE_ASM(const s32, "event/evtCommand", func_00227C38);

INCLUDE_ASM(const s32, "event/evtCommand", func_00227CB8);

INCLUDE_ASM(const s32, "event/evtCommand", func_00227DD0);

INCLUDE_ASM(const s32, "event/evtCommand", func_00227EE0);

INCLUDE_ASM(const s32, "event/evtCommand", func_00227F60);

INCLUDE_ASM(const s32, "event/evtCommand", func_00227FE0);
