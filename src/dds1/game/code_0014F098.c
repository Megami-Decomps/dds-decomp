#include "common.h"

extern u64 func_00222090(u64);

extern u64 func_0013DB28(void);

extern s32 func_0010D6A0(void);
extern s32 func_0013BEE8(u32);

extern s64 func_00142160(u64);

extern u64 func_0013CBA8(u64);

extern u64 func_0013D410(u64, u64);

extern u64 func_0010D428(u64);
extern u64 func_0013CEB0(u64, u64);

/* Persona 4 func_002993c0 @ 002993C0 (src/Script/scrCommonCommand.c), recompiled unchanged */
u32 func_0014F098(void)
{
    func_0010D5F0(func_0013DB58(func_0010D428(0)));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0014F098", func_0014F0C8);

u32 func_0014F110(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    temp_v0 = func_0013CEB0(temp_v0, temp_v1);
    func_0010D5F0(temp_v0);
    return 1;
}

u32 func_0014F158(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    temp_v0 = func_0013D410(temp_v0, temp_v1);
    func_0010D5F0(temp_v0);
    return 1;
}

void func_0014F1A0(void) {
    u64 temp_v0;

    temp_v0 = func_0013CBA8(1);
    func_0010D5F0(temp_v0);
}

u32 func_0014F1C0(void) {
    func_0013D650();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0014F098", func_0014F1E0);

INCLUDE_ASM(const s32, "game/code_0014F098", func_0014F210);

u32 func_0014F240(void) {
    func_00141D18();
    return 1;
}

u32 func_0014F260(void) {
    func_00141D98();
    return 1;
}

u32 func_0014F280(void) {
    func_00141D40();
    return 1;
}

u32 func_0014F2A0(void) {
    u64 temp_v0;

    temp_v0 = func_0010D428(0);
    func_00141DC0(temp_v0);
    return 1;
}

u32 func_0014F2C8(void) {
    u64 temp_v0;

    temp_v0 = func_0010D428(0);
    func_00141E88(temp_v0);
    return 1;
}

u8 func_0014F2F0(void) {
    u64 temp_v0;
    s64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_00142160(temp_v0);
    return temp_v1 != 0;
}

u32 func_0014F318(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    func_001421D0(temp_v0, temp_v1);
    return 1;
}

u32 func_0014F358(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    func_00142200(temp_v0, temp_v1);
    return 1;
}

u32 func_0014F398(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    func_0014A960(temp_v0, temp_v1, 0x3c);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0014F098", func_0014F3E0);

u32 func_0014F408(void) {
    s32 temp_v0;
    s64 temp_v1;

    temp_v0 = func_0010D6A0();
    temp_v1 = func_0013BEE8(*(u32 *)(temp_v0 + 0xe4));
    if (temp_v1 != 0) {
        func_0013DDF0(temp_v1);
    }
    return 1;
}

u32 func_0014F440(void) {
    u64 temp_v0;

    temp_v0 = func_0013DB28();
    func_0010D5F0(temp_v0);
    return 1;
}

u32 func_0014F468(void) {
    u64 temp_v0;

    temp_v0 = func_0010D428(0);
    temp_v0 = func_00222090(temp_v0);
    func_00147DB0(temp_v0);
    return 1;
}

/* Persona 4 func_001eb2a0 @ 001EB2A0 (src/promoted/code1_001e.c), recompiled unchanged */
s32 func_0014F498(void) {
    s32 temp;
    temp = func_0010D5A8(0);
    temp = func_0014A1A0(temp);
    func_0010D5F0(temp);
    return 1;
}
