#include "common.h"

extern u64 func_00163258(u64, u64);

void func_0016A1A8(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_00163258(arg0, 0);
    func_0016A088(temp_v0);
}

void func_0016A1C8(void) {
    func_0016A088();
}

void func_0016A1E0(s32 arg0) {
    func_00170048(*(u32 *)(arg0 + 0x44));
    func_002D0918(*(u32 *)(arg0 + 0x40));
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016A210);

void func_0016A220(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x38) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016A228);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016A230);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016A2D0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016A450);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016A6C0);

void func_0016A858(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_00163258(arg0, 0);
    func_0016A6C0(temp_v0);
}

void func_0016A878(void) {
    func_0016A6C0();
}

void func_0016A890(s32 arg0) {
    func_0016FC28(*(u32 *)(arg0 + 0x54));
    func_002D0918(*(u32 *)(arg0 + 0x50));
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016A8C0);

void func_0016A8D0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x48) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016A8D8);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016A8E0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016A9D0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016AB48);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016ACD0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016AD58);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016AFF0);

void func_0016B210(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_00163258(arg0, 0);
    func_0016AFF0(temp_v0);
}

void func_0016B230(void) {
    func_0016AFF0();
}

void func_0016B248(s32 arg0) {
    func_0016FC28(*(u32 *)(arg0 + 0x5c));
    func_002D0918(*(u32 *)(arg0 + 0x58));
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016B278);

void func_0016B288(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x50) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016B290);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016B298);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016B388);

void func_0016B570(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x48) + arg1 * 0x14;
    *(float *)(temp_v0 + 0x10) = *(float *)(temp_v0 + 0x10) + *(float *)(arg0 + 0x40);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016B598);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016B800);

void func_0016BA38(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_00163258(arg0, 0);
    func_0016B800(temp_v0);
}

void func_0016BA58(void) {
    func_0016B800();
}

void func_0016BA70(s32 arg0) {
    func_00170350(*(u32 *)(arg0 + 100));
    func_002D0918(*(u32 *)(arg0 + 0x60));
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016BAA0);

void func_0016BAB0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x58) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016BAB8);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016BAC0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016BBB0);

void func_0016BE20(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x50) + arg1 * 0x1c;
    *(float *)(temp_v0 + 0x18) = *(float *)(temp_v0 + 0x18) + *(float *)(arg0 + 0x44);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016BE48);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016C0E8);

void func_0016C338(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_00163258(arg0, 0);
    func_0016C0E8(temp_v0);
}

void func_0016C358(void) {
    func_0016C0E8();
}

void func_0016C370(s32 arg0) {
    func_0016FC28(*(u32 *)(arg0 + 0x7c));
    func_002D0918(*(u32 *)(arg0 + 0x78));
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016C3A0);

void func_0016C3B0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x60) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016C3B8);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016C3C0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016C4B0);

void func_0016C678(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x58) + arg1 * 0x10;
    *(float *)(temp_v0 + 8) = *(float *)(temp_v0 + 8) + *(float *)(arg0 + 0x50);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016C698);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016C9F0);

void func_0016CBB0(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_00163258(arg0, 0);
    func_0016C9F0(temp_v0);
}

void func_0016CBD0(void) {
    func_0016C9F0();
}

void func_0016CBE8(s32 arg0) {
    func_0016FC28(*(u32 *)(arg0 + 0x60));
    func_002D0918(*(u32 *)(arg0 + 0x5c));
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016CC18);

void func_0016CC28(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x54) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016CC30);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016CC38);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016CD28);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016CE00);

void func_0016CFC0(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x4c) + arg1 * 0x20;
    *(float *)(temp_v0 + 0x10) = *(float *)(temp_v0 + 0x10) + *(float *)(temp_v0 + 8);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016CFE0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016D2A8);

void func_0016D3E0(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_00163258(arg0, 0);
    func_0016D2A8(temp_v0);
}

void func_0016D400(void) {
    func_0016D2A8();
}

void func_0016D418(s32 arg0) {
    func_00170048(*(u32 *)(arg0 + 0x50));
    func_002D0918(*(u32 *)(arg0 + 0x4c));
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016D448);

void func_0016D458(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x44) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016D460);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016D468);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016D508);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016D6A0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016D940);

void func_0016DB18(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_00163258(arg0, 0);
    func_0016D940(temp_v0);
}

void func_0016DB38(void) {
    func_0016D940();
}

void func_0016DB50(s32 arg0) {
    func_00170350(*(u32 *)(arg0 + 0xe4));
    func_002D0918(*(u32 *)(arg0 + 0xe0));
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016DB80);

void func_0016DB90(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0xd8) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016DB98);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016DBA0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016DC90);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016DD68);

void func_0016DF90(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xd0) + arg1 * 0x20;
    *(float *)(temp_v0 + 0x18) = *(float *)(temp_v0 + 0x18) + *(float *)(temp_v0 + 8);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016DFB0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016E290);

void func_0016E4C0(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_00163258(arg0, 0);
    func_0016E290(temp_v0);
}

void func_0016E4E0(void) {
    func_0016E290();
}

void func_0016E4F8(s32 arg0) {
    func_0016FC28(*(u32 *)(arg0 + 100));
    func_002D0918(*(u32 *)(arg0 + 0x60));
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016E528);

void func_0016E538(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x58) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016E540);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016E548);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016E638);

void func_0016E820(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x50) + arg1 * 0x14;
    *(float *)(temp_v0 + 0x10) = *(float *)(temp_v0 + 0x10) + *(float *)(arg0 + 0x48);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016E848);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016EB00);

void func_0016EC40(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_00163258(arg0, 0);
    func_0016EB00(temp_v0);
}

void func_0016EC60(void) {
    func_0016EB00();
}

void func_0016EC78(s32 arg0) {
    func_00170048(*(u32 *)(arg0 + 0x54));
    func_002D0918(*(u32 *)(arg0 + 0x50));
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016ECA8);

void func_0016ECB8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x48) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016ECC0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016ECC8);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016ED68);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016EF00);
