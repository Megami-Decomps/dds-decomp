#include "common.h"

extern u32 D_003BB164;

extern u32 D_003BB178;

extern u32 func_00195C50(void);

INCLUDE_ASM(const s32, "interface/frFont", func_00194618);

INCLUDE_ASM(const s32, "interface/frFont", func_00194668);

INCLUDE_ASM(const s32, "interface/frFont", func_001946C8);

INCLUDE_ASM(const s32, "interface/frFont", func_00194788);

INCLUDE_ASM(const s32, "interface/frFont", func_00194800);

INCLUDE_ASM(const s32, "interface/frFont", func_00194840);

INCLUDE_ASM(const s32, "interface/frFont", func_00194920);

INCLUDE_ASM(const s32, "interface/frFont", func_00194978);

INCLUDE_ASM(const s32, "interface/frFont", func_00194988);

INCLUDE_ASM(const s32, "interface/frFont", func_00194998);

u32 func_001949A8(void) {
    return 0;
}

INCLUDE_ASM(const s32, "interface/frFont", func_001949B0);

INCLUDE_ASM(const s32, "interface/frFont", func_00194BA0);

INCLUDE_ASM(const s32, "interface/frFont", func_00194CD0);

INCLUDE_ASM(const s32, "interface/frFont", func_00194D20);

INCLUDE_ASM(const s32, "interface/frFont", func_00194E00);

INCLUDE_ASM(const s32, "interface/frFont", func_00194E80);

INCLUDE_ASM(const s32, "interface/frFont", func_00194F78);

INCLUDE_ASM(const s32, "interface/frFont", func_00194FC0);

INCLUDE_ASM(const s32, "interface/frFont", func_00195010);

INCLUDE_ASM(const s32, "interface/frFont", func_00195160);

INCLUDE_ASM(const s32, "interface/frFont", func_001951C8);

INCLUDE_ASM(const s32, "interface/frFont", func_00195360);

void func_00195388(s32 arg0) {
    *(u32 *)(arg0 + 0x40) = 1;
    func_00195360(arg0, 0x80);
}

void func_001953A8(s32 arg0, u8 arg1) {
    u32 temp_v0;

    *(u8 *)(arg0 + 1) = arg1;
    temp_v0 = func_00195C50();
    *(u32 *)(arg0 + 0xc) = temp_v0;
}

INCLUDE_ASM(const s32, "interface/frFont", func_001953D8);

INCLUDE_ASM(const s32, "interface/frFont", func_00195450);

void func_00195460(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1 >> 4;
}

INCLUDE_ASM(const s32, "interface/frFont", func_00195470);

INCLUDE_ASM(const s32, "interface/frFont", func_001954C8);

INCLUDE_ASM(const s32, "interface/frFont", func_00195520);

INCLUDE_ASM(const s32, "interface/frFont", func_00195530);

void func_00195548(u32 arg0) {
    D_003BB178 = arg0;
}

INCLUDE_ASM(const s32, "interface/frFont", func_00195550);

INCLUDE_ASM(const s32, "interface/frFont", func_001955D8);

INCLUDE_ASM(const s32, "interface/frFont", func_00195868);

INCLUDE_ASM(const s32, "interface/frFont", func_00195880);

INCLUDE_ASM(const s32, "interface/frFont", func_001958A0);

INCLUDE_ASM(const s32, "interface/frFont", func_00195B10);

INCLUDE_ASM(const s32, "interface/frFont", func_00195B60);

INCLUDE_ASM(const s32, "interface/frFont", func_00195B78);

void func_00195BD8(u32 arg0) {
    func_001944A0(8, arg0, 0);
}

void func_00195BF8(void) {
    func_001945A8(8);
}

INCLUDE_ASM(const s32, "interface/frFont", func_00195C10);

INCLUDE_ASM(const s32, "interface/frFont", func_00195C50);

INCLUDE_ASM(const s32, "interface/frFont", func_00195C88);

INCLUDE_ASM(const s32, "interface/frFont", func_00195CD8);

INCLUDE_ASM(const s32, "interface/frFont", func_00195DC8);

INCLUDE_ASM(const s32, "interface/frFont", func_00195E08);

void func_00195E48(void) {
    D_003BB164 = 0x15;
}

void func_00195E58(u32 arg0) {
    D_003BB164 = arg0;
}

INCLUDE_ASM(const s32, "interface/frFont", func_00195E60);

INCLUDE_ASM(const s32, "interface/frFont", func_00195ED8);

INCLUDE_ASM(const s32, "interface/frFont", func_00195FA8);

INCLUDE_ASM(const s32, "interface/frFont", func_00196038);

INCLUDE_ASM(const s32, "interface/frFont", func_00196088);

INCLUDE_ASM(const s32, "interface/frFont", func_001961B0);

INCLUDE_ASM(const s32, "interface/frFont", func_00196220);

INCLUDE_ASM(const s32, "interface/frFont", func_00196390);

void func_001963E0(u32 *arg0) {
    s8 temp_v0;

    if (arg0[5] == 0) {
        temp_v0 = *(s8 *)(arg0 + 7);
    }
    else {
        if (*(s32 *)(arg0[5] + 0x1c) == 0) {
            *(u8 *)(arg0 + 7) = 0;
        }
        temp_v0 = *(s8 *)(arg0 + 7);
    }
    if (temp_v0 == '\0') {
        temp_v0 = *(s8 *)((s32)arg0 + 0x1d);
    }
    else {
        func_00196390();
        temp_v0 = *(s8 *)((s32)arg0 + 0x1d);
    }
    if (temp_v0 != '\0') {
        func_00195450(arg0[5], *arg0, arg0[1]);
        *(u8 *)((s32)arg0 + 0x1d) = 0;
    }
}

INCLUDE_ASM(const s32, "interface/frFont", func_00196450);
