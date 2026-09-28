#include "common.h"

extern u32 func_002D3288(u32);

extern u64 func_002EB028(u64, u32 *, u64);

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018CAC8);

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018CB38);

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018CB70);

u32 func_0018CBB8(s32 arg0) {
    return *(u32 *)(arg0 + 4);
}

u32 func_0018CBC0(u32 *arg0) {
    return *arg0;
}

void func_0018CBC8(void) {
}

u32 func_0018CBD0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018CBD8);

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018CC18);

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018CC58);

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018CC98);

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018CCF0);

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018CD50);

void func_0018CDA0(void) {
}

void func_0018CDA8(void) {
    func_001028E8(0, 0, 0, 0);
}

u32 func_0018CDD0(u32 arg0) {
    return arg0;
}

void func_0018CDD8(void) {
}

void func_0018CDE0(void) {
}

void func_0018CDE8(void) {
}

void func_0018CDF0(void) {
}

void func_0018CDF8(void) {
}

void func_0018CE00(void) {
}

void func_0018CE08(void) {
}

void func_0018CE10(void) {
}

void func_0018CE18(void) {
}

void func_0018CE20(void) {
}

void func_0018CE28(void) {
}

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018CE30);

void func_0018CE38(void) {
}

u32 func_0018CE40(void) {
    return 0;
}

u32 func_0018CE48(void) {
    return 0;
}

void func_0018CE50(void) {
}

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018CE58);

void func_0018CE60(void) {
}

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018CE68);

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018CE70);

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018CEC0);

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018CEF0);

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018CF98);

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018D3D0);

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018D428);

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018D4B8);

void func_0018D938(u32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 0x3c);
    if (temp_v0 != 0) {
        func_002D2D00(temp_v0);
        *(u32 *)((s32)arg0 + 0x3c) = 0;
    }
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018D978);

u32 func_0018D988(s32 arg0) {
    return *(u32 *)(arg0 + 0x14);
}

u32 func_0018D990(s32 arg0) {
    return *(u32 *)(arg0 + 8);
}

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018D998);

void func_0018D9E0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x20) = arg1;
}

void func_0018D9E8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018D9F0);

void func_0018DA00(s32 arg0, u64 arg1) {
    u32 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    if (*(s32 *)(arg0 + 0x3c) != 0) {
        func_002D2D00(*(s32 *)(arg0 + 0x3c));
        *(u32 *)(arg0 + 0x3c) = 0;
    }
    temp_v1 = func_002EB028(arg1, temp_v2, 0);
    temp_v0 = func_002D3288(temp_v2[0]);
    *(u32 *)(arg0 + 0x3c) = temp_v0;
    func_002D0918(temp_v1);
}

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018DA70);

void func_0018DB88(void) {
    func_001028E8(0, 0, 0, 0);
}

void func_0018DBB0(void) {
    func_001028E8(0, 0, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018DBD8);

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018DC58);

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018DD40);

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018DDF8);

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018DE78);

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018DF00);






INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0F88);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0F98);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0FA8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0FB8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0FC8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0FD8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0FE8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0FF8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1008);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1018);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1028);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1038);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1048);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1058);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1068);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1078);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1088);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1098);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A10A8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A10B8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A10C8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A10D8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A10E8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A10F8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1108);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1118);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1128);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1138);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1148);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1158);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1168);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1178);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1188);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1198);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A11A8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A11B8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A11C8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A11D8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A11E8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A11F8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1208);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1218);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1228);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1238);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1248);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1258);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1270);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1280);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1290);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A12A0);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A12B0);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A12C0);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A12D0);


INCLUDE_SDATA(const s32, "game/code_0018CAC8", D_003BB058);


INCLUDE_SDATA(const s32, "game/code_0018CAC8", D_003BB060);

