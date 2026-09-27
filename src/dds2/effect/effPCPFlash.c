#include "common.h"

extern u64 func_0016AEB0(u64, u64);

void func_00171E00(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_0016AEB0(arg0, 0);
    func_00171CE0(temp_v0);
}

void func_00171E20(void) {
    func_00171CE0();
}

void func_00171E38(s32 arg0) {
    func_00177CA0(*(u32 *)(arg0 + 0x44));
    func_003297C8(*(u32 *)(arg0 + 0x40));
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00171E68);

void func_00171E78(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x38) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00171E80);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00171E88);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00171F28);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001720A8);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00172318);

void func_001724B0(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_0016AEB0(arg0, 0);
    func_00172318(temp_v0);
}

void func_001724D0(void) {
    func_00172318();
}

void func_001724E8(s32 arg0) {
    func_00177880(*(u32 *)(arg0 + 0x54));
    func_003297C8(*(u32 *)(arg0 + 0x50));
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00172518);

void func_00172528(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x48) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00172530);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00172538);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00172628);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001727A0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00172928);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001729B0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00172C48);

void func_00172E68(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_0016AEB0(arg0, 0);
    func_00172C48(temp_v0);
}

void func_00172E88(void) {
    func_00172C48();
}

void func_00172EA0(s32 arg0) {
    func_00177880(*(u32 *)(arg0 + 0x5c));
    func_003297C8(*(u32 *)(arg0 + 0x58));
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00172ED0);

void func_00172EE0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x50) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00172EE8);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00172EF0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00172FE0);

void func_001731C8(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x48) + arg1 * 0x14;
    *(float *)(temp_v0 + 0x10) = *(float *)(temp_v0 + 0x10) + *(float *)(arg0 + 0x40);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001731F0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00173458);

void func_00173690(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_0016AEB0(arg0, 0);
    func_00173458(temp_v0);
}

void func_001736B0(void) {
    func_00173458();
}

void func_001736C8(s32 arg0) {
    func_00177FA8(*(u32 *)(arg0 + 100));
    func_003297C8(*(u32 *)(arg0 + 0x60));
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001736F8);

void func_00173708(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x58) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00173710);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00173718);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00173808);

void func_00173A78(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x50) + arg1 * 0x1c;
    *(float *)(temp_v0 + 0x18) = *(float *)(temp_v0 + 0x18) + *(float *)(arg0 + 0x44);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00173AA0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00173D40);

void func_00173F90(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_0016AEB0(arg0, 0);
    func_00173D40(temp_v0);
}

void func_00173FB0(void) {
    func_00173D40();
}

void func_00173FC8(s32 arg0) {
    func_00177880(*(u32 *)(arg0 + 0x7c));
    func_003297C8(*(u32 *)(arg0 + 0x78));
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00173FF8);

void func_00174008(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x60) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00174010);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00174018);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00174108);

void func_001742D0(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x58) + arg1 * 0x10;
    *(float *)(temp_v0 + 8) = *(float *)(temp_v0 + 8) + *(float *)(arg0 + 0x50);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001742F0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00174648);

void func_00174808(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_0016AEB0(arg0, 0);
    func_00174648(temp_v0);
}

void func_00174828(void) {
    func_00174648();
}

void func_00174840(s32 arg0) {
    func_00177880(*(u32 *)(arg0 + 0x60));
    func_003297C8(*(u32 *)(arg0 + 0x5c));
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00174870);

void func_00174880(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x54) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00174888);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00174890);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00174980);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00174A58);

void func_00174C18(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x4c) + arg1 * 0x20;
    *(float *)(temp_v0 + 0x10) = *(float *)(temp_v0 + 0x10) + *(float *)(temp_v0 + 8);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00174C38);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00174F00);

void func_00175038(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_0016AEB0(arg0, 0);
    func_00174F00(temp_v0);
}

void func_00175058(void) {
    func_00174F00();
}

void func_00175070(s32 arg0) {
    func_00177CA0(*(u32 *)(arg0 + 0x50));
    func_003297C8(*(u32 *)(arg0 + 0x4c));
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001750A0);

void func_001750B0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x44) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001750B8);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001750C0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00175160);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001752F8);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00175598);

void func_00175770(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_0016AEB0(arg0, 0);
    func_00175598(temp_v0);
}

void func_00175790(void) {
    func_00175598();
}

void func_001757A8(s32 arg0) {
    func_00177FA8(*(u32 *)(arg0 + 0xe4));
    func_003297C8(*(u32 *)(arg0 + 0xe0));
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001757D8);

void func_001757E8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0xd8) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001757F0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001757F8);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001758E8);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001759C0);

void func_00175BE8(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xd0) + arg1 * 0x20;
    *(float *)(temp_v0 + 0x18) = *(float *)(temp_v0 + 0x18) + *(float *)(temp_v0 + 8);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00175C08);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00175EE8);

void func_00176118(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_0016AEB0(arg0, 0);
    func_00175EE8(temp_v0);
}

void func_00176138(void) {
    func_00175EE8();
}

void func_00176150(s32 arg0) {
    func_00177880(*(u32 *)(arg0 + 100));
    func_003297C8(*(u32 *)(arg0 + 0x60));
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00176180);

void func_00176190(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x58) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00176198);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001761A0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00176290);

void func_00176478(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x50) + arg1 * 0x14;
    *(float *)(temp_v0 + 0x10) = *(float *)(temp_v0 + 0x10) + *(float *)(arg0 + 0x48);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001764A0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00176758);

void func_00176898(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_0016AEB0(arg0, 0);
    func_00176758(temp_v0);
}

void func_001768B8(void) {
    func_00176758();
}

void func_001768D0(s32 arg0) {
    func_00177CA0(*(u32 *)(arg0 + 0x54));
    func_003297C8(*(u32 *)(arg0 + 0x50));
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00176900);

void func_00176910(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x48) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00176918);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00176920);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001769C0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00176B58);
