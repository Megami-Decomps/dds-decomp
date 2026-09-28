#include "common.h"

extern u64 effParamTableGetBlock(u64, u64);

INCLUDE_ASM(const s32, "game/code_00176E28", func_00176E28);

void func_00177078(u64 arg0) {
    u64 temp_v0;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    func_00176E28(temp_v0);
}

void func_00177098(void) {
    func_00176E28();
}

void func_001770B0(s32 arg0) {
    func_001781F8(*(u32 *)(arg0 + 0x7c));
    func_003297C8(*(u32 *)(arg0 + 0x78));
}

INCLUDE_ASM(const s32, "game/code_00176E28", func_001770E0);

void func_001770F0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x60) = arg1;
}

INCLUDE_ASM(const s32, "game/code_00176E28", func_001770F8);

INCLUDE_ASM(const s32, "game/code_00176E28", func_00177100);

INCLUDE_ASM(const s32, "game/code_00176E28", func_00177130);

INCLUDE_ASM(const s32, "game/code_00176E28", func_00177220);

void func_001773E8(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x58) + arg1 * 0x10;
    *(float *)(temp_v0 + 8) = *(float *)(temp_v0 + 8) + *(float *)(arg0 + 0x50);
}

INCLUDE_ASM(const s32, "game/code_00176E28", func_00177408);

INCLUDE_ASM(const s32, "game/code_00176E28", func_00177760);

void func_00177880(s32 arg0) {
    func_00333918(*(u32 *)(arg0 + 0x68));
    func_003297C8(*(u32 *)(arg0 + 0x6c));
}

INCLUDE_ASM(const s32, "game/code_00176E28", func_001778B0);

s32 func_00177B60(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 0x60) + arg1 * 0x50;
}

s32 func_00177B78(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 100) + arg1 * 0x14;
}

void func_00177B90(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x50) = arg1;
}

void func_00177B98(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x54) = arg1;
}

INCLUDE_ASM(const s32, "game/code_00176E28", func_00177BA0);

INCLUDE_ASM(const s32, "game/code_00176E28", func_00177BA8);

void func_00177CA0(s32 arg0) {
    func_00333918(*(u32 *)(arg0 + 0x68));
    func_003297C8(*(u32 *)(arg0 + 0x6c));
}

INCLUDE_ASM(const s32, "game/code_00176E28", func_00177CD0);

s32 func_00177E78(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 0x60) + arg1 * 0x30;
}

s32 func_00177E90(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 100) + arg1 * 0xc;
}

INCLUDE_ASM(const s32, "game/code_00176E28", func_00177EA8);

void func_00177FA8(s32 arg0) {
    func_00333918(*(u32 *)(arg0 + 0x68));
    func_003297C8(*(u32 *)(arg0 + 0x6c));
}

INCLUDE_ASM(const s32, "game/code_00176E28", func_00177FD8);

s32 func_00178190(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 0x60) + arg1 * 0x40;
}

s32 func_001781A0(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 100) + arg1 * 0x10;
}

INCLUDE_ASM(const s32, "game/code_00176E28", func_001781B0);

INCLUDE_ASM(const s32, "game/code_00176E28", func_001781F8);

INCLUDE_ASM(const s32, "game/code_00176E28", func_00178210);

s32 func_001784B0(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 0x60) + arg1 * 0x50;
}

s32 func_001784C8(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 100) + arg1 * 0x14;
}

void func_001784E0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x50) = arg1;
}

void func_001784E8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x54) = arg1;
}

INCLUDE_ASM(const s32, "game/code_00176E28", func_001784F0);

INCLUDE_ASM(const s32, "game/code_00176E28", func_001784F8);
