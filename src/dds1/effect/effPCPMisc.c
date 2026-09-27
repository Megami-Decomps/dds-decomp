#include "common.h"

extern u8 D_003BB04C;

extern s32 func_0017D7A8(void);

extern u64 func_00163258(u64, u64);

extern s32 D_003BD7FC;

extern u64 func_002CFEB8(u64);

void func_00177190(u32 arg0) {
    func_00151F00(*(u32 *)((s32)arg0 + 0x1c));
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001771C0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001772F8);

void func_00177308(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177310);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177318);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177418);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177590);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177608);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177700);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177850);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177860);

void func_00177868(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177870);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177990);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177A68);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177AC8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177B70);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177CD0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177CE0);

void func_00177CE8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177CF0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177E38);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177F90);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178028);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178130);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178260);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178270);

void func_00178278(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178280);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178320);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001783E8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178448);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178500);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001785E8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001785F8);

void func_00178600(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

void func_00178608(s32 arg0) {
    *(u32 *)(arg0 + 0x133c) = 0;
    *(u32 *)(arg0 + 0x1340) = 0;
    *(u32 *)(arg0 + 0x1344) = 0x80808080;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178620);

void func_001786C0(s32 arg0) {
    func_001629F0(*(u32 *)(arg0 + 0x134c));
    func_001629F0(*(u32 *)(arg0 + 0x1348));
    func_002D0918(*(u32 *)(arg0 + 0x1350));
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001786F8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178790);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178AF0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178B08);

void func_00178B10(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x1334) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178B18);

void func_00178B88(u32 arg0) {
    func_001629F0(*(u32 *)((s32)arg0 + 0x14));
    func_001629F0(*(u32 *)((s32)arg0 + 0x18));
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178BC0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178C28);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178CA8);

void func_00178CB8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178CC0);

u64 func_00178F80(void) {
    u64 temp_v0;

    temp_v0 = func_002CFEB8(0x4c);
    func_00178CC0(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178FB8);

u64 func_00179010(void) {
    u64 temp_v0;

    temp_v0 = func_002CFEB8(0x4c);
    func_00178CC0(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179048);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179138);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179148);

void func_00179150(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179158);

u64 func_001793F8(void) {
    u64 temp_v0;

    temp_v0 = func_002CFEB8(0x4c);
    func_00179158(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179430);

u64 func_00179488(void) {
    u64 temp_v0;

    temp_v0 = func_002CFEB8(0x4c);
    func_00179158(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001794C0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001795B0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001795C0);

void func_001795C8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001795D0);

u64 func_00179890(void) {
    u64 temp_v0;

    temp_v0 = func_002CFEB8(0x94);
    func_001795D0(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001798C8);

u64 func_00179920(void) {
    u64 temp_v0;

    temp_v0 = func_002CFEB8(0x94);
    func_001795D0(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179958);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179A48);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179A58);

void func_00179A60(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179A68);

u64 func_00179D08(void) {
    u64 temp_v0;

    temp_v0 = func_002CFEB8(0x4c);
    func_00179A68(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179D40);

u64 func_00179D98(void) {
    u64 temp_v0;

    temp_v0 = func_002CFEB8(0x4c);
    func_00179A68(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179DD0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179EC0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179ED0);

void func_00179ED8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179EE0);

u64 func_0017A078(void) {
    u64 temp_v0;

    temp_v0 = func_002CFEB8(0x4c);
    func_00179EE0(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A0B0);

u64 func_0017A108(void) {
    u64 temp_v0;

    temp_v0 = func_002CFEB8(0x4c);
    func_00179EE0(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A140);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A230);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A240);

void func_0017A248(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A250);

u64 func_0017A3E8(void) {
    u64 temp_v0;

    temp_v0 = func_002CFEB8(0x3c);
    func_0017A250(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A420);

u64 func_0017A478(void) {
    u64 temp_v0;

    temp_v0 = func_002CFEB8(0x3c);
    func_0017A250(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A4B0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A5A0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A5B0);

void func_0017A5B8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A5C0);

u64 func_0017A5F8(void) {
    u64 temp_v0;

    temp_v0 = func_002CFEB8(0x20);
    func_0017A5C0(temp_v0);
    return temp_v0;
}

void func_0017A630(u32 arg0) {
    func_001634D8(*(u32 *)((s32)arg0 + 0x1c));
    func_002CFF98(arg0);
}

u64 func_0017A660(void) {
    u64 temp_v0;

    temp_v0 = func_002CFEB8(0x20);
    func_0017A5C0(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A698);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A7F0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A800);

void func_0017A808(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A810);

u64 func_0017AAD0(void) {
    u64 temp_v0;

    temp_v0 = func_002CFEB8(0x94);
    func_0017A810(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017AB08);

u64 func_0017AB60(void) {
    u64 temp_v0;

    temp_v0 = func_002CFEB8(0x94);
    func_0017A810(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017AB98);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017AC88);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017AC98);

void func_0017ACA0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017ACA8);

u64 func_0017ACE0(void) {
    u64 temp_v0;

    temp_v0 = func_002CFEB8(0x20);
    func_0017ACA8(temp_v0);
    return temp_v0;
}

void func_0017AD18(u32 arg0) {
    func_001634D8(*(u32 *)((s32)arg0 + 0x1c));
    func_002CFF98(arg0);
}

u64 func_0017AD48(void) {
    u64 temp_v0;

    temp_v0 = func_002CFEB8(0x20);
    func_0017ACA8(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017AD80);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017AED0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017AEE0);

void func_0017AEE8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017AEF0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017AFB8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B008);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B148);

void func_0017B160(u32 arg0, u32 arg1) {
    *(u32 *)(D_003BD7FC + 0x10) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B170);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B180);

void func_0017B218(u32 arg0) {
    func_00186CB8(*(u32 *)((s32)arg0 + 0x34));
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B248);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B328);

void func_0017B338(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B340);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B348);

void func_0017B3D8(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_00163258(arg0, 0);
    func_0017B348(temp_v0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B3F8);

void func_0017B498(u32 arg0) {
    func_00188050(*(u32 *)((s32)arg0 + 0x38));
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B4C8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B670);

void func_0017B680(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B688);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B690);

void func_0017B720(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_00163258(arg0, 0);
    func_0017B690(temp_v0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B740);

void func_0017B7F0(u32 arg0) {
    func_00186CB8(*(u32 *)((s32)arg0 + 0x38));
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B820);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B9C8);

void func_0017B9D8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B9E0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B9E8);

void func_0017BA78(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_00163258(arg0, 0);
    func_0017B9E8(temp_v0);
}

void func_0017BA98(void) {
    func_0017B9E8();
}

void func_0017BAB0(void) {
    func_002CFF98();
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017BAC8);

void func_0017BBD8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017BBE0);

void func_0017BC90(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_00163258(arg0, 0);
    func_0017BBE0(temp_v0);
}

void func_0017BCB0(void) {
    func_0017BBE0();
}

void func_0017BCC8(void) {
    func_002CFF98();
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017BCE0);

void func_0017BDF8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x34) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017BE00);

void func_0017BE90(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_00163258(arg0, 0);
    func_0017BE00(temp_v0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017BEB0);

void func_0017BF60(u32 arg0) {
    func_00187080(*(u32 *)((s32)arg0 + 0x34));
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017BF90);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C158);

void func_0017C168(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x20) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C170);

void func_0017C200(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_00163258(arg0, 0);
    func_0017C170(temp_v0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C220);

void func_0017C2D0(u32 arg0) {
    func_00187580(*(u32 *)((s32)arg0 + 0x34));
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C300);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C4F0);

void func_0017C500(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x20) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C508);

u64 func_0017C540(void) {
    u64 temp_v0;

    temp_v0 = func_002CFEB8(0x20);
    func_0017C508(temp_v0);
    return temp_v0;
}

void func_0017C578(u32 arg0) {
    func_001634D8(*(u32 *)((s32)arg0 + 0x1c));
    func_002CFF98(arg0);
}

u64 func_0017C5A8(void) {
    u64 temp_v0;

    temp_v0 = func_002CFEB8(0x20);
    func_0017C508(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C5E0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C798);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C7A8);

void func_0017C7B0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C7B8);

u64 func_0017C7F0(void) {
    u64 temp_v0;

    temp_v0 = func_002CFEB8(0x20);
    func_0017C7B8(temp_v0);
    return temp_v0;
}

void func_0017C828(u32 arg0) {
    func_001634D8(*(u32 *)((s32)arg0 + 0x1c));
    func_002CFF98(arg0);
}

u64 func_0017C858(void) {
    u64 temp_v0;

    temp_v0 = func_002CFEB8(0x20);
    func_0017C7B8(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C890);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017CA40);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017CA50);

void func_0017CA58(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017CA60);

void func_0017CB88(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_00163258(arg0, 0);
    temp_v1 = func_00163258(arg0, 1);
    func_0017CA60(temp_v0, temp_v1);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017CBD0);

void func_0017CC28(u32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 0x74);
    if (temp_v0 != 0) {
        func_0014FAB8(temp_v0);
    }
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017CC60);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017CEB8);

void func_0017CED0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 100) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017CED8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017CF38);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017D0A0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017D118);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017D288);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017D2F8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017D4A8);

void func_0017D4B8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x54) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017D4C0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017D7A8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017D8A8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017DA58);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017DBB0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017DCF8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017E4A8);

void func_0017E4C0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0xb8) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017E4C8);

void func_0017E4F0(void) {
    s32 temp_v0;

    temp_v0 = func_0017D7A8();
    *(u32 *)(temp_v0 + 0xbc) = 1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017E518);

void func_0017E668(void) {
    s32 temp_v0;

    temp_v0 = func_0017D7A8();
    *(u32 *)(temp_v0 + 0xbc) = 2;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017E690);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017E7E0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017E978);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017EAB0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017ED38);

void func_0017ED98(s32 *arg0) {
    s32 temp_v0;
    s32 *piVar2;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = arg0[0x42];
    piVar2 = arg0;
    do {
        if (*piVar2 <= temp_v0) {
            func_0017DCF8(piVar2[0x3f]);
            temp_v0 = arg0[0x42];
        }
        temp_v1 = temp_v1 + 1;
        piVar2 = piVar2 + 1;
    } while (temp_v1 < 3);
    arg0[0x42] = temp_v0 + 1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017EE10);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017EE70);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017EED0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017EF30);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F008);

void func_0017F0E8(u32 arg0) {
    func_001629F0(*(u32 *)((s32)arg0 + 0x60));
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F118);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F1C0);

void func_0017F1D0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x5c) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F1D8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F1E0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F210);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F2D0);

void func_0017F388(u32 arg0) {
    func_001629F0(*(u32 *)((s32)arg0 + 100));
    func_001629F0(*(u32 *)((s32)arg0 + 0x60));
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F3C0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F4C0);

void func_0017F4D0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x5c) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F4D8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F4E0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F510);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F5D0);

void func_0017F688(u32 arg0) {
    func_001629F0(*(u32 *)((s32)arg0 + 100));
    func_001629F0(*(u32 *)((s32)arg0 + 0x60));
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F6C0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F7C0);

void func_0017F7D0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x5c) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F7D8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F7E0);

u32 func_0017F810(void) {
    D_003BB04C = 1;
    return 0;
}

u32 func_0017F820(void) {
    D_003BB04C = 1;
    return 0;
}

void func_0017F830(void) {
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F838);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F850);

void func_0017F898(void) {
    func_0017F850(0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F8B0);

void func_0017F900(void) {
    func_0017F8B0(0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F918);

void func_0017F960(void) {
    func_0017F918(0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F978);

void func_0017F9C8(void) {
    func_0017F978(0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F9E0);

void func_0017FA28(void) {
    func_0017F9E0(0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FA40);

void func_0017FA90(void) {
    func_0017FA40(0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FAA8);

void func_0017FAF0(void) {
    func_0017FAA8(0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FB08);

void func_0017FB58(void) {
    func_0017FB08(0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FB70);

void func_0017FBB8(void) {
    func_0017FB70(0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FBD0);

void func_0017FC20(void) {
    func_0017FBD0(0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FC38);

void func_0017FC80(void) {
    func_0017FC38(0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FC98);

void func_0017FCE8(void) {
    func_0017FC98(0);
}

void func_0017FD00(u32 arg0) {
    func_001655D0(*(u32 *)((s32)arg0 + 0x20));
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FD30);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FE38);

void func_0017FE48(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FE50);

void func_0017FF10(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_00163258(arg0, 0);
    func_0017FE50(temp_v0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FF30);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180000);

void func_00180050(u32 arg0) {
    func_001634D8(*(u32 *)((s32)arg0 + 0x30));
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180080);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001801D8);

void func_001801E8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001801F0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001801F8);

void func_00180338(u32 arg0) {
    func_002DAA68(*(u32 *)((s32)arg0 + 0xa8));
    func_002D0918(*(u32 *)((s32)arg0 + 0xac));
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180370);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180540);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001806C8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001806F8);

void func_00180890(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_00163258(arg0, 0);
    func_001806F8(temp_v0);
}

void func_001808B0(void) {
    func_001806F8();
}

void func_001808C8(u32 arg0) {
    func_00180338(*(u32 *)((s32)arg0 + 0x5c));
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001808F8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180B20);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180B30);

void func_00180B40(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x54) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180B48);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180B78);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180BD0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180E18);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180E78);

void func_00181020(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_00163258(arg0, 0);
    func_00180E78(temp_v0);
}

void func_00181040(void) {
    func_00180E78();
}

void func_00181058(u32 arg0) {
    func_00180338(*(u32 *)((s32)arg0 + 0x7c));
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181088);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001811B0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001811C0);

void func_001811D0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x60) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001811D8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181208);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181490);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181538);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181650);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181708);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001818A8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181C60);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181C70);

void func_00181C78(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x170) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181C80);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181D80);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181E80);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181EF0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181F48);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182170);

void func_00182180(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182188);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182190);

void func_00182398(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_00163258(arg0, 0);
    temp_v1 = func_00163258(arg0, 1);
    func_00182190(temp_v0, temp_v1);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001823E0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001825E0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182660);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001827D0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182A20);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182A30);

void func_00182A38(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x114) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182A40);

void func_00182C90(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;
    u64 temp_v2;

    temp_v0 = func_00163258(arg0, 0);
    temp_v1 = func_00163258(arg0, 1);
    temp_v2 = func_00163258(arg0, 2);
    func_00182A40(temp_v0, temp_v1, temp_v2);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182CF8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182F20);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182FC8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001830F8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001834C0);

void func_001834D0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x9c) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001834D8);

void func_001836F8(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_00163258(arg0, 0);
    temp_v1 = func_00163258(arg0, 1);
    func_001834D8(temp_v0, temp_v1);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183740);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183950);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001839D0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183B20);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183DA0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183DB0);

void func_00183DB8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0xa4) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183DC0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183EE0);

void func_00183FD0(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;
    u64 temp_v2;

    temp_v0 = func_00163258(arg0, 0);
    temp_v1 = func_00163258(arg0, 1);
    temp_v2 = func_00163258(arg0, 2);
    func_00183EE0(temp_v0, temp_v1, temp_v2);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184038);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184090);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184130);
