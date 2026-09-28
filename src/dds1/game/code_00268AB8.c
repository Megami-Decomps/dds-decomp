#include "common.h"

extern char D_003AFD48[]; /* "---------- AT3 --------\n", followed by 8 zero bytes no C function emits */

extern char D_003AFD80[]; /* "titleProc" */

extern u32 D_003DA180[];

extern u32 D_003D9140[];

extern u32 D_003BD8D0;

extern u32 D_003BD8D4;

extern u32 D_003BC5BC;

extern u32 D_003BC5C8;

extern u8 D_003D9178[];

extern s32 func_0026A720(void);

extern u64 func_002E97E0(void);

typedef struct { u64 v; } __attribute__((packed)) u64p;

extern u8 D_003BC590[];

extern u8 D_003BC598[];

extern char *strcat(char *, char *);

extern s32 func_00101A70();

extern s32 D_003BC588;

extern void func_00134CF0(void);

extern void func_002CF430(void);

extern void func_0021FE38(void);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_00268AB8);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_00268D40);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_002692E0);

void func_002693E0(u32 arg0) {
    soundSetSequenceVolumePan(arg0, 0x7f, 0x3f);
}

void func_00269400(void) {
}

void func_00269408(void) {
}

void func_00269410(void) {
}

void func_00269418(void) {
}

char *func_00269420(char *arg0, char *arg1) {
    *(u64p *)arg0 = *(u64p *)D_003BC590;
    return strcat(arg0, arg1);
}

char *func_00269450(char *arg0, char *arg1) {
    *(u64p *)arg0 = *(u64p *)D_003BC598;
    return strcat(arg0, arg1);
}

INCLUDE_ASM(const s32, "game/code_00268AB8", func_00269480);

u32 func_002694F8(void) {
    return 0x599;
}

u32 func_00269500(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    *(s32 *)(temp_v0 + 4) = *(s32 *)(temp_v0 + 4) + 1;
    return 0;
}

void func_00269530(void) {
    func_002CFF98(func_00101A70());
    D_003BC588 = 0;
}

INCLUDE_ASM(const s32, "game/code_00268AB8", func_00269558);

void resetTitleEffectState(s32 effect) {
    s32 context = func_00101A70(D_003BC588);
    if (func_002E97E0() != 0) {
        func_002E97E8();
    }
    sdfSoundSendNamedCommand(effect, 0x7f);
    *(s32 *)(context + 4) = 0;
}

void func_00269628(s32 arg0) {
    *(s32 *)func_00101A70(D_003BC588) = arg0;
}

INCLUDE_RODATA(const s32, "game/code_00268AB8", D_003AFC80);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_00269658);

void func_002696F8(void) {
    func_002E97E8();
}

void func_00269710(void) {
    func_002E97E0();
}

void func_00269728(void) {
}

s32 func_00269730(void) {
    return *(s32 *)(func_00101A70(D_003BC588) + 4);
}

u32 func_00269758(void) {
    u64 temp_v0;

    temp_v0 = func_0010D428(0);
    func_002E8D10(temp_v0);
    return 1;
}

u32 func_00269780(void) {
    u64 temp_v0;

    temp_v0 = func_0010D428(0);
    soundSetSequenceVolumePan(temp_v0, 0x7f, 0x3f);
    return 1;
}

u32 func_002697B0(void) {
    func_002E8E50();
    return 1;
}

s32 initializeTitleEffects(void) {
    s32 value;
    value = func_0010D5A8(0);
    if (func_002E97E0() != 0) {
        func_002E97E8();
    }
    sdfSoundSendNamedCommand(value, 0x7f);
    return 1;
}

u32 func_00269820(void) {
    func_002E97E8();
    return 1;
}

u32 func_00269840(void) {
    u64 temp_v0;

    temp_v0 = func_002E97E0();
    func_0010D5F0(temp_v0);
    return 1;
}

u32 func_00269868(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00268AB8", func_00269870);

u32 func_002698C0(void) {
    func_0026A778();
    return 1;
}

u32 func_002698E0(void) {
    func_0026A808();
    func_0026A950();
    return 1;
}

u32 func_00269908(void) {
    advanceTitleStateUnderSemaphore();
    return 1;
}

u8 func_00269928(void) {
    s64 temp_v0;

    temp_v0 = func_0026A720();
    return temp_v0 == 0;
}

INCLUDE_ASM(const s32, "game/code_00268AB8", func_00269948);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_002699A0);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_00269A68);

void func_00269B50(u32 arg0) {
    sceSdRemoteInit();
    func_002699A0(arg0);
    D_003BC5BC = 0;
}

INCLUDE_ASM(const s32, "game/code_00268AB8", func_00269B80);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_00269C10);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_00269CA0);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_00269D18);

void func_0026A1C8(void) {
    for (;;) {
        func_002CFAD8(1);
        WaitSema(D_003BD8D0);
        func_00269D18();
        SignalSema(D_003BD8D0);
    }
}

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026A1F8);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026A248);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026A340);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026A390);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026A3E0);

void menuStoreTaskResult(void) {
    D_003BD8D4 = func_00288B48();
    D_003D9140[9] = 1;
}

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026A490);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026A588);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026A5F0);

u32 updateTitleTransition(void) {
    if (D_003D9140[9] == 1) {
        func_0026A490(D_003D9140);
    }
    return D_003D9140[9];
}

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026A720);

INCLUDE_RODATA(const s32, "game/code_00268AB8", D_003AFCF0);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026A778);

void func_0026A808(void) {
    WaitSema(D_003BD8D0);
    D_003D9140[1] = 0;
    D_003D9140[4] = 2;
    SignalSema(D_003BD8D0);
}

void advanceTitleStateUnderSemaphore(void) {
    WaitSema(D_003BD8D0);
    if (D_003D9140[4] == 1 && D_003D9140[9] == 3) {
        D_003D9140[9] = 4;
        *(u32 *)D_003D9178 = 0;
        D_003BC5C8 = 6;
    }
    SignalSema(D_003BD8D0);
}

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026A8A0);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026A900);

void func_0026A950(void) {
    WaitSema(D_003BD8D0);
    func_0026A900();
    SignalSema(D_003BD8D0);
}

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026A980);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026AA28);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026ABA8);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026AD28);

void printTitleDebugBanner(void) {
    WaitSema(D_003BD8D0);
    if (D_003DA180[4] != 1) {
        D_003DA180[4] = 0;
    }
    func_003003F0(D_003AFD48);
    SignalSema(D_003BD8D0);
}

void func_0026ADE8(void) {
    D_003DA180[1] = 0;
    D_003DA180[4] = 2;
    func_0026AE10();
}

void func_0026AE10(void) {
    u32 *temp_v0 = D_003DA180;
    u32 temp_v1 = temp_v0[8];

    if (temp_v1 == 0) {
        return;
    }
    func_002D0918(temp_v1);
    temp_v0[8] = 0;
}

void func_0026AE50(void) {
    WaitSema(D_003BD8D0);
    func_0026ADE8();
    SignalSema(D_003BD8D0);
}

void func_0026AE80(void) {
    WaitSema(D_003BD8D0);
    func_0026AE10();
    SignalSema(D_003BD8D0);
}

void func_0026AEB0(void) {
    func_0026BFC8();
}

INCLUDE_RODATA(const s32, "game/code_00268AB8", D_003AFD48);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026AEC8);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026AF30);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026AF78);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026B020);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026B050);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026B160);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026B1C0);

INCLUDE_RODATA(const s32, "game/code_00268AB8", D_003AFD80);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026B1F0);

u32 func_0026BCE8(void) {
    func_0026B050();
    return 0xffffffff;
}

s32 func_0026BD08(void) {
    func_0026B160();
    kwlnTaskDestroyWithHierarchyByName(D_003AFD80, 1);
    return 0;
}

u32 func_0026BD38(void) {
    func_0026B050(0);
    return 0;
}

void func_0026BD58(void) {
    func_0021FE38();
    func_00117730();
    func_001176A0();
}

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC588);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC58C);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC590);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC598);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5A0);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5A8);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5AC);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5B0);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5B8);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5BC);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5C0);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5C4);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5C8);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5CC);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5D0);

