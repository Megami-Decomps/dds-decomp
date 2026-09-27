#include "common.h"

extern s64 func_00221FD8(u64);

extern u64 func_00222090(u64);

extern u32 D_003BBDAC;

extern u64 func_0010D428(u64);
extern u64 func_0021FC30(u64, u64);

extern u64 func_0010FD80(void);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00222AC0);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00222B00);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00222B70);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00222BA8);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00222C68);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00222EB0);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00222ED8);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00223540);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_002235E8);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_002236E8);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00223718);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00223828);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00223A10);

void func_00223AA0(u64 arg0, u64 arg1) {
    u64 temp_v0;

    temp_v0 = func_0010FD80();
    func_00110A48(temp_v0, arg1, arg0);
}

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00223AE0);

u32 func_00223B20(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    temp_v0 = func_0021FC30(temp_v0, temp_v1);
    func_0010D5F0(temp_v0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00223B68);

u32 func_00223C60(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    func_0021FD50(temp_v0, temp_v1);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00223CA0);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00223D10);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00223D80);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00223DF0);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00223E58);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00223EB0);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00223FB0);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00224048);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00224130);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_002241D0);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00224268);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00224308);

void func_002243C0(void) {
    func_00222310(D_003BBDAC);
}

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC2A0);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_002243D8);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00224530);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00224638);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_002246D0);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00224770);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_002247B0);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00224828);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00224880);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00224948);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00224A18);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00224A80);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00224AD0);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00224B28);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00224B98);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00224C08);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00224CD8);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00224F48);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_002250D8);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225120);

u32 func_00225160(void) {
    u64 temp_v0;

    temp_v0 = func_0010D428(0);
    temp_v0 = func_00222090(temp_v0);
    func_00221FE8(temp_v0);
    return 1;
}

u32 func_00225190(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v0 = func_00222090(temp_v0);
    temp_v1 = func_0010D428(1);
    func_00221FF8(temp_v0, temp_v1);
    return 1;
}

u8 func_002251D8(void) {
    u64 temp_v0;
    s64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v0 = func_00222090(temp_v0);
    temp_v1 = func_00221FD8(temp_v0);
    return temp_v1 == 0;
}

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225208);

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC480);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225330);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225408);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_002254C8);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225560);

u32 func_002255D8(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v0 = func_00222090(temp_v0);
    temp_v1 = func_0010D428(1);
    func_00221C50(temp_v0, temp_v1);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225620);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225708);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_002257C0);

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC508);

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC520);

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC550);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225880);

u32 func_00225958(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225960);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_002259E0);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225A60);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225B10);

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC588);

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC5B0);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225BA0);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225C48);

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC600);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225CD8);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225D80);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225DC0);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225E00);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225E40);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225F08);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225FE8);
