#include "common.h"

extern u64 func_00328D68(u64);

extern s32 D_00438F04;

extern s32 func_00185400(void);

extern u8 D_0043643C;

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017EDE8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017EE18);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017EF50);

void func_0017EF60(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017EF68);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017EF70);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F070);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F1E8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F260);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F358);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F4A8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F4B8);

void func_0017F4C0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F4C8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F5E8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F6C0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F720);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F7C8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F928);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F938);

void func_0017F940(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F948);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FA90);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FBE8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FC80);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FD88);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FEB8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FEC8);

void func_0017FED0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FED8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FF78);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180040);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001800A0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180158);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180240);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180250);

void func_00180258(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

void func_00180260(s32 arg0) {
    *(u32 *)(arg0 + 0x133c) = 0;
    *(u32 *)(arg0 + 0x1340) = 0;
    *(u32 *)(arg0 + 0x1344) = 0x80808080;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180278);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180318);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180350);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001803E8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180748);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180760);

void func_00180768(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x1334) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180770);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001807E0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180818);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180880);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180900);

void func_00180910(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180918);

u64 func_00180BD8(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x4c);
    func_00180918(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180C10);

u64 func_00180C68(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x4c);
    func_00180918(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180CA0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180D90);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180DA0);

void func_00180DA8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180DB0);

u64 func_00181050(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x4c);
    func_00180DB0(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181088);

u64 func_001810E0(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x4c);
    func_00180DB0(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181118);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181208);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181218);

void func_00181220(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181228);

u64 func_001814E8(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x94);
    func_00181228(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181520);

u64 func_00181578(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x94);
    func_00181228(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001815B0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001816A0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001816B0);

void func_001816B8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001816C0);

u64 func_00181960(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x4c);
    func_001816C0(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181998);

u64 func_001819F0(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x4c);
    func_001816C0(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181A28);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181B18);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181B28);

void func_00181B30(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181B38);

u64 func_00181CD0(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x4c);
    func_00181B38(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181D08);

u64 func_00181D60(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x4c);
    func_00181B38(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181D98);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181E88);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181E98);

void func_00181EA0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181EA8);

u64 func_00182040(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x3c);
    func_00181EA8(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182078);

u64 func_001820D0(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x3c);
    func_00181EA8(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182108);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001821F8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182208);

void func_00182210(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182218);

u64 func_00182250(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x20);
    func_00182218(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182288);

u64 func_001822B8(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x20);
    func_00182218(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001822F0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182448);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182458);

void func_00182460(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182468);

u64 func_00182728(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x94);
    func_00182468(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182760);

u64 func_001827B8(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x94);
    func_00182468(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001827F0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001828E0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001828F0);

void func_001828F8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182900);

u64 func_00182938(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x20);
    func_00182900(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182970);

u64 func_001829A0(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x20);
    func_00182900(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001829D8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182B28);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182B38);

void func_00182B40(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182B48);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182C10);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182C60);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182DA0);

void func_00182DB8(u32 arg0, u32 arg1) {
    *(u32 *)(D_00438F04 + 0x10) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182DC8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182DD8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182E70);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182EA0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182F80);

void func_00182F90(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182F98);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182FA0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183030);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183050);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001830F0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183120);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001832C8);

void func_001832D8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001832E0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001832E8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183378);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183398);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183448);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183478);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183620);

void func_00183630(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183638);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183640);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001836D0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001836F0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183708);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183720);

void func_00183830(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183838);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001838E8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183908);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183920);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183938);

void func_00183A50(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x34) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183A58);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183AE8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183B08);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183BB8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183BE8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183DB0);

void func_00183DC0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x20) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183DC8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183E58);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183E78);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183F28);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183F58);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184148);

void func_00184158(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x20) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184160);

u64 func_00184198(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x20);
    func_00184160(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001841D0);

u64 func_00184200(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x20);
    func_00184160(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184238);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001843F0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184400);

void func_00184408(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184410);

u64 func_00184448(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x20);
    func_00184410(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184480);

u64 func_001844B0(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x20);
    func_00184410(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001844E8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184698);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001846A8);

void func_001846B0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001846B8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001847E0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184828);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184880);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001848B8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184B10);

void func_00184B28(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 100) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184B30);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184B90);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184CF8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184D70);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184EE0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184F50);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00185100);

void func_00185110(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x54) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00185118);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00185400);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00185500);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001856B0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00185808);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00185950);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186100);

void func_00186118(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0xb8) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186120);

void func_00186148(void) {
    s32 temp_v0;

    temp_v0 = func_00185400();
    *(u32 *)(temp_v0 + 0xbc) = 1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186170);

void func_001862C0(void) {
    s32 temp_v0;

    temp_v0 = func_00185400();
    *(u32 *)(temp_v0 + 0xbc) = 2;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001862E8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186438);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001865D0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186708);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186990);

void func_001869F0(s32 *arg0) {
    s32 temp_v0;
    s32 *piVar2;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = arg0[0x42];
    piVar2 = arg0;
    do {
        if (*piVar2 <= temp_v0) {
            func_00185950(piVar2[0x3f]);
            temp_v0 = arg0[0x42];
        }
        temp_v1 = temp_v1 + 1;
        piVar2 = piVar2 + 1;
    } while (temp_v1 < 3);
    arg0[0x42] = temp_v0 + 1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186A68);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186AC8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186B28);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186B88);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186C60);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186D40);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186D70);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186E18);

void func_00186E28(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x5c) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186E30);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186E38);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186E68);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186F28);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186FE0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187018);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187118);

void func_00187128(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x5c) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187130);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187138);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187168);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187228);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001872E0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187318);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187418);

void func_00187428(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x5c) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187430);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187438);

u32 func_00187468(void) {
    D_0043643C = 1;
    return 0;
}

u32 func_00187478(void) {
    D_0043643C = 1;
    return 0;
}

void func_00187488(void) {
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187490);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001874A8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001874F0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187508);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187558);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187570);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001875B8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001875D0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187620);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187638);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187680);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187698);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001876E8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187700);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187748);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187760);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001877B0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001877C8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187810);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187828);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187878);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187890);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001878D8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001878F0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187940);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187958);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187988);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187A90);

void func_00187AA0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187AA8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187B68);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187B88);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187CA8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187CD8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187E30);

void func_00187E40(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187E48);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187E50);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187F90);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187FC8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188198);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188320);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188350);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001884E8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188508);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188520);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188550);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188778);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188788);

void func_00188798(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x54) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001887A0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001887D0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188828);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188A70);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188AD0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188C78);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188C98);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188CB0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188CE0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188E08);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188E18);

void func_00188E28(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x60) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188E30);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188E60);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001890E8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00189190);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001892A8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00189360);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00189500);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001898B8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001898C8);

void func_001898D0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x170) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001898D8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001899D8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00189AD8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00189B48);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00189BA0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00189DC8);

void func_00189DD8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00189DE0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00189DE8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00189FF0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018A038);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018A238);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018A2B8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018A428);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018A678);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018A688);

void func_0018A690(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x114) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018A698);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018A8E8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018A950);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018AB78);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018AC20);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018AD50);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018B118);

void func_0018B128(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x9c) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018B130);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018B350);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018B398);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018B5A8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018B628);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018B778);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018B9F8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018BA08);

void func_0018BA10(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0xa4) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018BA18);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018BB38);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018BC28);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018BC90);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018BCE8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018BD88);

INCLUDE_RODATA(const s32, "effect/effPCPMisc", D_00414610);

