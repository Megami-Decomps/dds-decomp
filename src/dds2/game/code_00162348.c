#include "common.h"

extern s32 D_00436400;

extern s32 D_00436404;

extern void (*D_003AAF10[])(void *, void *, void *);

void func_00162348(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0xf0) = arg1;
}

INCLUDE_ASM(const s32, "game/code_00162348", func_00162350);

INCLUDE_ASM(const s32, "game/code_00162348", func_00162390);

INCLUDE_ASM(const s32, "game/code_00162348", func_001623D0);

INCLUDE_ASM(const s32, "game/code_00162348", func_001624B8);

INCLUDE_ASM(const s32, "game/code_00162348", func_00162558);

INCLUDE_ASM(const s32, "game/code_00162348", func_00162590);

void func_001628E0(void) {
    func_001621F8();
}

void func_001628F8(float arg0, s32 arg1) {
    func_00162248();
    *(float *)(arg1 + 0x8c) = *(float *)(arg1 + 0x8c) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00162348", func_00162938);

INCLUDE_ASM(const s32, "game/code_00162348", func_00162958);

void func_00162968(void) {
    func_001622E8();
}

void func_00162980(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0xf0) = arg1;
}

void func_00162988(u32 arg0, u8 arg1) {
    func_00162350(arg0, arg1);
}

INCLUDE_ASM(const s32, "game/code_00162348", func_001629A0);

INCLUDE_ASM(const s32, "game/code_00162348", func_001629C0);

INCLUDE_ASM(const s32, "game/code_00162348", func_00162A30);

INCLUDE_ASM(const s32, "game/code_00162348", func_00162AC8);

INCLUDE_ASM(const s32, "game/code_00162348", func_00162B60);

INCLUDE_ASM(const s32, "game/code_00162348", func_00162C48);

INCLUDE_ASM(const s32, "game/code_00162348", func_00162D38);

u32 func_00162E10(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00162348", func_00162E18);

void func_00162E40(void) {
}

INCLUDE_ASM(const s32, "game/code_00162348", func_00162E48);

void func_00162FC8(u16 *arg0) {
    *arg0 = 1;
    func_00333918(*(u32 *)(arg0 + 0x20));
    func_003297C8(*(u32 *)(arg0 + 8));
}

void func_00163000(s32 arg0) {
    *(s32 *)(arg0 + 0x54) = D_00436400;
    D_00436400 = arg0;
}

INCLUDE_ASM(const s32, "game/code_00162348", func_00163010);

void func_00163238(s32 arg0) {
    func_003332E8(*(u32 *)(arg0 + 0x40));
}

INCLUDE_ASM(const s32, "game/code_00162348", func_00163250);

INCLUDE_ASM(const s32, "game/code_00162348", func_00163290);

void func_001634A8(s32 arg0) {
    func_00333918(*(u32 *)(arg0 + 0x20));
    func_003297C8(*(u32 *)(arg0 + 0x10));
}

INCLUDE_ASM(const s32, "game/code_00162348", func_001634D8);

void func_00163508(s32 arg0) {
    *(s32 *)(arg0 + 0x24) = D_00436404;
    D_00436404 = arg0;
}

INCLUDE_ASM(const s32, "game/code_00162348", func_00163518);

INCLUDE_ASM(const s32, "game/code_00162348", func_001635D0);

INCLUDE_ASM(const s32, "game/code_00162348", func_00163628);

INCLUDE_ASM(const s32, "game/code_00162348", func_001636F0);

INCLUDE_ASM(const s32, "game/code_00162348", func_00163780);

INCLUDE_ASM(const s32, "game/code_00162348", func_001638D8);

INCLUDE_ASM(const s32, "game/code_00162348", func_00163B68);

INCLUDE_ASM(const s32, "game/code_00162348", func_00163BC8);

INCLUDE_ASM(const s32, "game/code_00162348", func_00163D18);

INCLUDE_ASM(const s32, "game/code_00162348", func_00163EE0);

INCLUDE_ASM(const s32, "game/code_00162348", func_00163F50);

INCLUDE_ASM(const s32, "game/code_00162348", func_00164208);

INCLUDE_ASM(const s32, "game/code_00162348", func_00164318);

INCLUDE_ASM(const s32, "game/code_00162348", func_00164390);

INCLUDE_ASM(const s32, "game/code_00162348", func_001644B0);

INCLUDE_ASM(const s32, "game/code_00162348", func_00164630);

INCLUDE_ASM(const s32, "game/code_00162348", func_00164690);

INCLUDE_ASM(const s32, "game/code_00162348", func_00164748);

INCLUDE_ASM(const s32, "game/code_00162348", func_00164848);

INCLUDE_ASM(const s32, "game/code_00162348", func_001648C0);

INCLUDE_ASM(const s32, "game/code_00162348", func_001649E0);

INCLUDE_ASM(const s32, "game/code_00162348", func_00164AE8);

void func_00164C68(s32 arg0, u16 arg1) {
    *(u16 *)(arg0 + 2) = arg1;
}

void func_00164C70(void *work, s32 sub, void *a2, void *a3) {
    u16 id = *(u16 *)work;

    D_003AAF10[id * 3 + sub](work, a2, a3);
}

INCLUDE_ASM(const s32, "game/code_00162348", func_00164CB0);

INCLUDE_ASM(const s32, "game/code_00162348", func_00165300);

void func_001653A8(s32 arg0) {
    func_00333918(*(u32 *)(arg0 + 0x10));
    func_003297C8(*(u32 *)(arg0 + 0x14));
}

INCLUDE_ASM(const s32, "game/code_00162348", func_001653D8);

INCLUDE_ASM(const s32, "game/code_00162348", func_00165500);


INCLUDE_SDATA(const s32, "game/code_00162348", D_00436400);


INCLUDE_SDATA(const s32, "game/code_00162348", D_00436404);

