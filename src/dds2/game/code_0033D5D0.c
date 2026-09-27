#include "common.h"

extern u8 D_00438B1F;

extern u32 D_004391A4;

extern s32 D_00438AC8;

extern u32 D_004391C4;

extern s32 D_004391A8;

extern u32 D_004391B8;

extern s32 func_0033E008(u32, u8 *, u32);

extern u32 D_004391C8;

extern u32 D_00438AE4;

extern u32 D_004391CC;

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033D5D0);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033D7B8);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033D810);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033D898);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033D8B0);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033D8D8);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033D930);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033D990);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033DA30);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033DAD8);

u32 func_0033DB58(void) {
    return D_004391A4;
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033DB60);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033DBF8);

void func_0033DCA8(void) {
    if (D_00438AC8 != 0) {
        SignalSema(D_004391C4);
        D_00438AC8 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033DCD0);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033DD90);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033DE60);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033E008);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033E1C0);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033E248);

void func_0033E490(void) {
    if (D_004391A8 != 0) {
        WaitSema(D_004391B8);
        func_0034D4C8();
        SignalSema(D_004391B8);
        D_004391A8 = 0;
    }
}

u8 func_0033E4C8(u32 arg0) {
    s64 temp_v0;
    u8 temp_v1 [16];

    temp_v0 = func_0033E008(arg0, temp_v1, 0);
    return temp_v0 != 0;
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033E4F0);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033E520);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033E550);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033E5E0);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033E660);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033E6B0);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033E728);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033E7C0);

u32 func_0033E800(void) {
    return 0;
}

u32 func_0033E808(void) {
    return 0;
}

u32 func_0033E810(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033E818);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033E9B8);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033E9F8);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033EA60);

void func_0033EAE0(u32 arg0) {
    func_0033FCE0();
    WaitSema(D_00438AE4);
    func_0033FD30(arg0);
}

void func_0033EB10(void) {
    func_0033FBF0();
    WaitSema(D_00438AE4);
}

u32 func_0033EB30(void) {
    func_0033FB98();
    WaitSema(D_00438AE4);
    return D_004391C8;
}

u32 func_0033EB58(void) {
    func_0033FB38();
    WaitSema(D_00438AE4);
    return D_004391CC;
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033EB80);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033EC18);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033EC28);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033EC40);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033EC48);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033ED38);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033EDB0);

void func_0033EEC8(u32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 0x28);
    *(u32 *)((s32)arg0 + 0x28) = 0xffffffff;
    if (-1 < temp_v0) {
        func_00369DF8(temp_v0);
    }
    func_0033EDB0(arg0);
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033EF08);

INCLUDE_RODATA(const s32, "game/code_0033D5D0", D_0042E288);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033EF58);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033F650);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033F898);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033F9D0);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033FA50);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033FAB8);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033FB38);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033FB98);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033FBF0);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033FC50);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033FCB0);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033FCE0);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033FD30);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033FD70);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033FE00);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033FEA8);

void func_0033FF20(void) {
    D_00438B1F = 3;
    func_0033FEA8(0x78);
}

void func_0033FF40(void) {
    func_0033FEA8(0x48);
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033FF58);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_0033FF98);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_00340048);

void func_003400B8(void) {
    iSignalSema();
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_003400D0);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_00340218);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_00340298);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_00340328);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_003403A0);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_00340410);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_00340448);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_00340498);

void func_00340528(u32 arg0) {
    func_003297C8(*(u32 *)arg0);
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_00340558);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_003405D8);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_003406A0);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_003407A0);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_003407C0);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_00340868);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_00340898);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_00340950);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_003409C8);

INCLUDE_ASM(const s32, "game/code_0033D5D0", func_00340A50);
