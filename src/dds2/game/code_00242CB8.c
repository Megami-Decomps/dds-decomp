#include "common.h"

extern s64 func_001018B0(u64);

extern u64 func_0010D650(u64);

extern s32 D_00435DD0;

extern u32 D_004371F8;

extern u32 D_00437200;

extern u64 func_00101958(void);

extern u8 D_00438FA4;

extern u32 D_00438FA8;

extern u32 D_00438FAC;

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00242CB8);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00242D10);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00242DE0);

u32 func_00242E70(void) {
    u64 temp_v0;
    s64 temp_v1;

    temp_v0 = func_0010D650(0);
    temp_v1 = func_001018B0(temp_v0);
    if (temp_v1 != 0) {
        func_0024FE28(temp_v0);
    }
    return 1;
}

u32 func_00242EB8(void) {
    u64 temp_v0;
    s64 temp_v1;

    temp_v0 = func_0010D650(0);
    temp_v1 = func_001018B0(temp_v0);
    if (temp_v1 != 0) {
        func_0024FE50(temp_v0);
    }
    return 1;
}

u32 func_00242F00(void) {
    u64 temp_v0;
    s64 temp_v1;

    temp_v0 = func_0010D650(0);
    temp_v1 = func_001018B0(temp_v0);
    if (temp_v1 != 0) {
        func_0024FE80(temp_v0);
    }
    return 1;
}

u32 func_00242F48(void) {
    u64 temp_v0;
    u64 temp_v1;
    u64 temp_v2;

    func_00135568(0);
    func_00135578(0x80);
    temp_v0 = func_0010D650(0);
    temp_v1 = func_0010D650(1);
    temp_v2 = func_0010D650(2);
    func_001354F0(temp_v0, temp_v1, temp_v2);
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00242CB8", D_00422050);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00242FB0);

u32 func_00243028(void) {
    func_002500E0(1);
    return 1;
}

u32 func_00243048(void) {
    func_002500E0(0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00243068);

u32 func_002430C0(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D650(0);
    temp_v1 = func_0010D650(1);
    func_0025CD50(temp_v0, temp_v1);
    return 1;
}

u32 func_00243100(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D650(0);
    temp_v1 = func_0010D650(1);
    func_0025CDA8(temp_v0, temp_v1);
    return 1;
}

u32 func_00243140(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D650(0);
    temp_v1 = func_0010D650(1);
    func_0025CE68(temp_v0, temp_v1);
    return 1;
}

u32 func_00243180(void) {
    func_003425B0();
    return 1;
}

u32 func_002431A0(void) {
    func_003425D8();
    return 1;
}

u32 func_002431C0(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D650(0);
    temp_v1 = func_0010D650(1);
    func_0025CEB0(temp_v0, temp_v1);
    return 1;
}

u32 func_00243200(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D650(0);
    temp_v1 = func_0010D650(1);
    func_0025CF00(temp_v0, temp_v1);
    return 1;
}

u32 func_00243240(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D650(0);
    temp_v1 = func_0010D650(1);
    func_0025D0B8(temp_v0, temp_v1);
    return 1;
}

u32 func_00243280(void) {
    func_001971A8();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242CB8", func_002432A0);

void func_00243310(void) {
    *(u8 *)(D_00435DD0 + 0xa43) = 0;
}

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00243320);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00243330);

u8 func_00243358(void) {
    return *(u8 *)(D_00435DD0 + 0xa41);
}

void func_00243368(u8 arg0) {
    *(u8 *)(D_00435DD0 + 0xa41) = arg0 & 0xf;
    *(u32 *)(D_00435DD0 + 0xa44) = 0;
}

void func_00243380(void) {
    *(u8 *)(D_00435DD0 + 0xa40) = *(u8 *)(D_00435DD0 + 0xa40) | 1;
}

void func_00243398(void) {
    *(u8 *)(D_00435DD0 + 0xa40) = *(u8 *)(D_00435DD0 + 0xa40) & 0xfe;
}

INCLUDE_ASM(const s32, "game/code_00242CB8", func_002433B0);

void func_002433E8(void) {
    *(u8 *)(D_00435DD0 + 0xa40) = *(u8 *)(D_00435DD0 + 0xa40) & 0xfd;
    D_004371F8 = 0;
}

void func_00243400(void) {
    *(u8 *)(D_00435DD0 + 0xa40) = *(u8 *)(D_00435DD0 + 0xa40) | 2;
}

void func_00243418(void) {
    *(u8 *)(D_00435DD0 + 0xa40) = *(u8 *)(D_00435DD0 + 0xa40) & 0xfd;
}

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00243430);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00243568);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_002436A8);

void func_00243700(void) {
    u64 temp_v0;

    temp_v0 = func_00101958();
    func_00243830(temp_v0);
    func_00328E48(temp_v0);
    D_00437200 = 0;
}

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00243740);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_002437B8);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_002437F0);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00243830);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00243850);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_002438B0);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_002438F0);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00243958);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00243AD8);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00243C68);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00243DB8);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00243EE8);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00243FD8);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_002440C8);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00244178);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_002441B8);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_002441F8);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00244408);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_002446C8);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00244828);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00244988);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_002449E0);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00244A38);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00244AE0);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00244B90);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00244F00);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00245508);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00245590);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00245618);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_002457A8);

void func_002457B8(s32 arg0) {
    if (arg0 == 0) {
        D_00438FA4 = 0;
        D_00438FA8 = 0;
        D_00438FAC = 0;
        return;
    }
    D_00438FAC = (s32)arg0;
    D_00438FA4 = 3;
    D_00438FA8 = 0;
}

INCLUDE_ASM(const s32, "game/code_00242CB8", func_002457E0);

void func_00245810(void) {
}

u32 func_00245818(void) {
    return 0;
}

void func_00245820(void) {
    func_0010BFE0();
}

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00245838);

INCLUDE_RODATA(const s32, "game/code_00242CB8", D_00422168);

INCLUDE_RODATA(const s32, "game/code_00242CB8", D_004221D8);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00245880);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_002458B8);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00245F80);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00245F88);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00246028);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00246078);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00246108);
