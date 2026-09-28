#include "common.h"

extern void func_0025DF68(s32, s32);

extern void func_00261760(s32);
extern s32 D_003BAA00;

extern void func_00260530(s32, u32);
extern void func_0025ECD0();
extern u8 D_0036AB64[];

extern void func_0025E108(s32, s32);

extern void func_002605B0(s32, u32);
extern void func_00260570(s32, u32);
extern void func_00260AB0(s32);
extern void func_0025F138();
extern s32 func_00245A40(s32);
extern void func_0024DED8(s32);
extern s32 func_0024DEF8(s32, s32);
extern void func_002E96D8(s32);

extern void func_00260670(s32 arg0);

extern void func_0024DD78(void);
extern void func_002858E8(s32, s32);
extern u8 D_0036AA68[];
extern u8 D_0036AA84[];
extern u8 D_0036AAA0[];
extern u8 D_0036AABC[];

extern s32 kwlnFadeIsActive(void);

extern s64 func_0024DC08(void);

extern s64 func_00285670(s32, s32 *, u64, u64);

extern s32 func_00101A70();

typedef struct EvtDispatchState {
    u8 pad00[0x84];
    s32 mode;
    u8 pad88[0x1C];
    s32 progressTicks; /* 0xA4 */
    u8 padA8[4];
    s32 action;
    s16 substate;
} EvtDispatchState;

s32 evtIsFadeDispatchIdle(void) {
    s32 temp_v0 = kwlnFadeIsActive();

    if (temp_v0 != 0) {
        return 0;
    }
    return func_0024DC08() == 0;
}

void evtInstallStateTable(s32 arg0) {
    if (((EvtDispatchState *)arg0)->mode == 2) {
        *(s32 *)(arg0 + 0x58) = (s32)D_0036AA68;
        func_002858E8(arg0 + 0x54, (s32)D_0036AA68 + 0xC4);
    }
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00245D08);

s32 func_00245DA0(void) {
    s32 *temp_v0 = (s32 *)func_00101A70();

    if (temp_v0[43] == 1) {
        func_002605B0((s32)temp_v0, 4);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00245DE0);

void evtPrimeDispatchStart(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_00260670(temp_v0);
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 1, arg0);
}

void evtSetupDispatchSync(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_0024DD78();
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

void evtInstallStateTableB(s32 arg0) {
    if (((EvtDispatchState *)arg0)->mode == 1) {
        *(s32 *)(arg0 + 0x58) = (s32)D_0036AA84;
        func_002858E8(arg0 + 0x54, (s32)D_0036AA84 + 0xA8);
    }
}

s32 func_00246160(void) {
    s32 *temp_v0 = (s32 *)func_00101A70();

    if (temp_v0[43] == 1) {
        func_002453C8(temp_v0);
    }
    return 1;
}

s32 evtSelectStateAction(void) {
    s32 temp_v0 = func_00101A70();
    s32 temp_v1 = ((EvtDispatchState *)temp_v0)->action;

    if (temp_v1 == 5) {
        func_002605B0(temp_v0, 3);
    } else if (temp_v1 == 7) {
        s32 temp_v2;

        func_002605B0(temp_v0, 9);
        temp_v2 = *(s32 *)(*(s32 *)(temp_v0 + 0x70) + 0x14);
        *(s32 *)(temp_v2 + 0x2C) = (s32)func_0025F138;
        func_00260570(temp_v2, 10);
    }
    ((EvtDispatchState *)temp_v0)->substate = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00246220);

void evtStageDispatchStart(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_00260AB0(temp_v0);
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 1, arg0);
}

void evtSetupDispatchSyncB(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_0024DD78();
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

void evtInstallStateTableC(s32 arg0) {
    if (((EvtDispatchState *)arg0)->mode == 1) {
        *(s32 *)(arg0 + 0x58) = (s32)D_0036AAA0;
        func_002858E8(arg0 + 0x54, (s32)D_0036AAA0 + 0x8C);
    }
}

s32 func_00246538(void) {
    s32 *temp_v0 = (s32 *)func_00101A70();

    if (temp_v0[43] == 1) {
        func_002457E8(temp_v0);
    }
    return 1;
}

s32 evtSelectStateActionB(void) {
    s32 temp_v0 = func_00101A70();
    s32 temp_v1 = ((EvtDispatchState *)temp_v0)->action;

    if (temp_v1 == 5) {
        func_002605B0(temp_v0, 3);
    } else if (temp_v1 == 7) {
        s32 temp_v2;

        func_002605B0(temp_v0, 9);
        temp_v2 = *(s32 *)(*(s32 *)(temp_v0 + 0x70) + 0x14);
        *(s32 *)(temp_v2 + 0x2C) = (s32)func_0025F138;
        func_00260570(temp_v2, 10);
    }
    ((EvtDispatchState *)temp_v0)->substate = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_002465F8);

void evtStageDispatchStartB(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_00260AB0(temp_v0);
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 1, arg0);
}

void evtSetupDispatchSyncC(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_0024DD78();
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

void evtInstallStateTableD(s32 arg0) {
    if (((EvtDispatchState *)arg0)->mode == 2) {
        *(s32 *)(arg0 + 0x58) = (s32)D_0036AABC;
        func_002858E8(arg0 + 0x54, (s32)D_0036AABC + 0x70);
    }
}

s32 evtEnableStateFlag(void) {
    s32 temp_v0 = func_00101A70();

    if ((((EvtDispatchState *)temp_v0)->action == 1) && (func_00245A40(temp_v0) == 0)) {
        ((EvtDispatchState *)temp_v0)->mode = 2;
    }
    return 1;
}

s32 evtSelectStateActionC(void) {
    s32 temp_v0 = func_00101A70();
    s32 temp_v1 = ((EvtDispatchState *)temp_v0)->action;

    if (temp_v1 == 5) {
        func_002605B0(temp_v0, 3);
    } else if (temp_v1 == 7) {
        s32 temp_v2;

        func_002605B0(temp_v0, 9);
        temp_v2 = *(s32 *)(*(s32 *)(temp_v0 + 0x70) + 0x14);
        *(s32 *)(temp_v2 + 0x2C) = (s32)func_0025F138;
        func_00260570(temp_v2, 10);
    }
    ((EvtDispatchState *)temp_v0)->substate = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_002469F0);

void evtStageDispatchStartC(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_00260AB0(temp_v0);
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 1, arg0);
}

void evtSetupDispatchSyncD(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_0024DD78();
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

u32 func_00246C80(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    *(u32 *)(temp_v0 + 0xa4) = 0;
    func_0027BB28(*(u32 *)(*(s32 *)(temp_v0 + 0x6c) + 0x14));
    return 1;
}

s64 evtQueryStateProgress(u64 arg0) {
    s32 temp_v0 = func_00101A70();
    s64 temp_v1 = func_00285670(temp_v0 + 8, temp_v0 + 0x54, 0, arg0);

    if (temp_v1 == 0) {
        if ((*(s32 *)(temp_v0 + 0x54) == 0) && (func_0024DC08() == 0)) {
            s32 temp_v2 = ((EvtDispatchState *)temp_v0)->progressTicks;

            if ((f32)temp_v2 < 20.0f) {
                ((EvtDispatchState *)temp_v0)->progressTicks = temp_v2 + 1;
            } else {
                func_002858E8(temp_v0 + 0x54, (s32)D_0036AB64);
            }
        }
        temp_v1 = 0;
    }
    return temp_v1;
}

void evtFetchDispatchStart(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_0025E108(temp_v0, *(s32 *)(temp_v0 + 0xA4));
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 1, arg0);
}

void evtSetupDispatchSyncE(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_0024DD78();
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

s32 func_00246E00(void) {
    s32 *temp_v0 = (s32 *)func_00101A70();

    temp_v0[32] = 1;
    mnuCampClampSceneCounter(-1, temp_v0);
    return 1;
}

s32 evtAdvanceStateStage(void) {
    s32 temp_v0 = func_00101A70();

    if (((EvtDispatchState *)temp_v0)->action == 0xA) {
        s32 temp_v1;

        ((EvtDispatchState *)temp_v0)->substate = 0xA;
        func_002605B0(temp_v0, 6);
        temp_v1 = *(s32 *)(*(s32 *)(temp_v0 + 0x70) + 0x14);
        *(s32 *)(temp_v1 + 0x2C) = (s32)func_0025ECD0;
        func_00260530(temp_v1, 0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00246EA0);

void evtAlignDispatchStart(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_00261760(temp_v0);
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 1, arg0);
}

void evtSetupDispatchSyncF(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_0024DD78();
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247190);

INCLUDE_ASM(const s32, "game/code_00245C98", func_002472D8);

void evtAccumulateStateScore(s32 arg0) {
    s32 temp_v0 = *(s32 *)(*(s32 *)(*(s32 *)(arg0 + 0x70) + 0x14) + 0x1C) + 0x60;

    if ((u32)(*(s32 *)(temp_v0 + 4) - 0x60) < 0x20) {
        *(s32 *)(D_003BAA00 + 0xA50) += *(s32 *)temp_v0 * *(s32 *)(arg0 + 0x80);
    }
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247420);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247588);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247728);

void evtSetupDispatchSyncG(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_0024DD78();
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_002478F8);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247970);

s64 func_002479F0(u64 arg0) {
    s32 temp_v0;
    s64 temp_v1;
    s32 *piVar3;

    temp_v0 = func_00101A70();
    piVar3 = (s32 *)(temp_v0 + 0x54);
    temp_v1 = func_00285670(temp_v0 + 8, piVar3, 0, arg0);
    if (temp_v1 == 0) {
        if ((*piVar3 == 0) && (temp_v1 = func_0024DC08(), temp_v1 == 0)) {
            func_002858F8(piVar3, *(u32 *)(temp_v0 + 0x58));
        }
        temp_v1 = 0;
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247A78);

void evtSetupDispatchSyncH(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_0024DD78();
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

u32 func_00247CF8(void) {
    func_00220110(0x323);
    kwlnFadeOutStart(0, 0, 0, 0xf);
    func_0024DED8(0);
    func_0024DEF8(0, 0);
    func_0024DEF8(1, 1);
    return 1;
}

u32 func_00247D50(void) {
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00245C98", D_003AF528);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247D58);

void evtRefreshDispatchStart(s32 arg0) {
    s32 temp_v0 = func_00101A70();
    s32 temp_v1 = *(s32 *)(temp_v0 + 0xA4);

    if (temp_v1 != 0) {
        func_0025DF68(temp_v0, temp_v1);
    }
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 1, arg0);
}

void evtSetupDispatchSyncI(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_0024DD78();
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

u32 evtResetStateFlags(void) {
    func_0024DED8(0);
    func_0024DEF8(0, 1);
    func_0024DEF8(1, 0);
    func_002E96D8(0x300000);
    return 1;
}

u32 func_00248170(void) {
    kwlnFadeOutStart(0, 0, 0, 0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_002481A0);

void evtDispatchStart(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 1, arg0);
}

void evtDispatchSync(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

u32 func_002482B0(void) {
    return 1;
}

u32 func_002482B8(void) {
    return 1;
}

u32 func_002482C0(void) {
    return 0;
}

u32 func_002482C8(void) {
    return 0;
}

u32 func_002482D0(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_002482D8);

INCLUDE_ASM(const s32, "game/code_00245C98", func_002483C0);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00248468);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00248508);

INCLUDE_RODATA(const s32, "game/code_00245C98", D_003AF570);

INCLUDE_RODATA(const s32, "game/code_00245C98", D_003AF580);

INCLUDE_RODATA(const s32, "game/code_00245C98", D_003AF590);

INCLUDE_SDATA(const s32, "game/code_00245C98", D_003BC3B0);

INCLUDE_SDATA(const s32, "game/code_00245C98", D_003BC3B8);

INCLUDE_SDATA(const s32, "game/code_00245C98", D_003BC3C0);

INCLUDE_SDATA(const s32, "game/code_00245C98", D_003BC3C8);

INCLUDE_SDATA(const s32, "game/code_00245C98", D_003BC3D0);

INCLUDE_SDATA(const s32, "game/code_00245C98", D_003BC3D8);

