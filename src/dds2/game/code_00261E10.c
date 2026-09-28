#include "common.h"

extern u8 func_00264B58(void);

extern s64 func_0026C768(void);

extern s64 func_002C4038(s32, s32 *, u64, u64);

extern s32 kwlnFadeIsActive(void);

extern s32 func_00101958();

extern void func_002C42B0(s32, s32);

extern u8 D_003CE498[];

extern void func_00297240(s32, u32);

extern void func_0026C900(void);

extern u8 D_003CE4B4[];

extern void func_00297200(s32, u32);

extern void func_00295D38();

extern u8 D_003CE4D0[];

extern u8 D_003CE508[];

extern s32 func_00261B98(s32);

extern u8 D_003CE690[];

extern void func_002971C0(s32, u32);

extern void func_002958B0();

s32 func_00261E10(void) {
    s32 temp_v0 = kwlnFadeIsActive();

    if (temp_v0 != 0) {
        return 0;
    }
    return func_0026C768() == 0;
}

INCLUDE_ASM(const s32, "game/code_00261E10", evtInstallStateTable);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00261E80);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00261F08);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00261F48);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00262190);

INCLUDE_ASM(const s32, "game/code_00261E10", evtSetupDispatchSync);

INCLUDE_ASM(const s32, "game/code_00261E10", evtInstallStateTableB);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00262270);

INCLUDE_ASM(const s32, "game/code_00261E10", evtSelectStateAction);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00262330);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00262598);

INCLUDE_ASM(const s32, "game/code_00261E10", evtSetupDispatchSyncB);

INCLUDE_ASM(const s32, "game/code_00261E10", evtInstallStateTableC);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00262678);

INCLUDE_ASM(const s32, "game/code_00261E10", evtSelectStateActionB);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00262740);

INCLUDE_ASM(const s32, "game/code_00261E10", func_002629A8);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00262A00);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00262A48);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00262A88);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00262AC8);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00262B50);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00262DB8);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00262E10);

INCLUDE_ASM(const s32, "game/code_00261E10", evtInstallStateTableD);

INCLUDE_ASM(const s32, "game/code_00261E10", evtEnableStateFlag);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00262EF0);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00262F78);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263180);

INCLUDE_ASM(const s32, "game/code_00261E10", func_002631D8);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263220);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263270);

u32 func_00263378(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263380);

INCLUDE_ASM(const s32, "game/code_00261E10", func_002633F0);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263448);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263490);

u32 func_002635E0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_002635E8);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263658);

INCLUDE_ASM(const s32, "game/code_00261E10", func_002636B0);

u32 func_002636F8(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    *(u32 *)(temp_v0 + 0xb8) = 0;
    func_002B8988(*(u32 *)(*(s32 *)(temp_v0 + 0x7c) + 0x18));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", evtQueryStateProgress);

INCLUDE_ASM(const s32, "game/code_00261E10", func_002637E0);

INCLUDE_ASM(const s32, "game/code_00261E10", evtSetupDispatchSyncE);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263880);

INCLUDE_ASM(const s32, "game/code_00261E10", evtAdvanceStateStage);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263920);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263B98);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263BF0);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263C38);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263D40);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263DD0);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263E60);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263EB0);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263F10);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263F50);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263FB0);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00264120);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00264240);

INCLUDE_ASM(const s32, "game/code_00261E10", func_002642B8);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00264300);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00264378);

s64 func_002643F8(u64 arg0) {
    s32 temp_v0;
    s64 temp_v1;
    s32 *piVar3;

    temp_v0 = func_00101958();
    piVar3 = (s32 *)(temp_v0 + 0x58);
    temp_v1 = func_002C4038(temp_v0 + 0xc, piVar3, 0, arg0);
    if (temp_v1 == 0) {
        if ((*piVar3 == 0) && (temp_v1 = func_0026C768(), temp_v1 == 0)) {
            func_002C42C0(piVar3, *(u32 *)(temp_v0 + 0x5c));
        }
        temp_v1 = 0;
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00264480);

INCLUDE_ASM(const s32, "game/code_00261E10", func_002646C8);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00264710);

u32 func_00264848(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00264850);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00264AB8);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00264B10);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00264B58);

u32 func_00264C00(void) {
    u8 temp_v0;
    s32 temp_v1;

    temp_v1 = func_00101958();
    temp_v0 = func_00264B58();
    *(u8 *)(temp_v1 + 0xcc) = temp_v0;
    if ((*(s32 *)(temp_v1 + 200) == 0) && (*(s8 *)(temp_v1 + 0xcd) == '\x01')) {
        func_0026C5B8(0x22);
    }
    return 1;
}

u32 func_00264C58(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00264C60);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00264CE0);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00264D38);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00264D80);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00264E18);

u32 func_00264EF8(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00264F00);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00264F98);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00264FF0);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00265038);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00265130);

INCLUDE_RODATA(const s32, "game/code_00261E10", D_00424D08);

INCLUDE_SDATA(const s32, "game/code_00261E10", D_00437840);

INCLUDE_SDATA(const s32, "game/code_00261E10", D_00437848);

INCLUDE_SDATA(const s32, "game/code_00261E10", D_00437850);

