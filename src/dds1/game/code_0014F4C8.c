#include "common.h"

extern u64 sdfSoundIsCommandBusy(void);

extern u64 func_0014A250(void);

extern s32 D_0032E4E4[];

u32 func_0014F4C8(void) {
    u64 value;

    value = func_0014A250();
    func_0010D5F0(value);
    return 1;
}

extern s32 D_0032E3B0[];

extern s32 func_0010D428(s32);

s32 func_0014F4F0(void) {
    s32 changed = 0;
    switch (func_0010D428(0)) {
    case 0:
        if (D_0032E3B0[3] & 1) {
            D_0032E3B0[3] &= ~1;
            changed = 1;
        }
        break;
    case 1:
        if (D_0032E3B0[3] & 2) {
            D_0032E3B0[3] &= ~2;
            changed = 1;
        }
        break;
    case 2:
        if (D_0032E3B0[3] & 4) {
            D_0032E3B0[3] &= ~4;
            changed = 1;
        }
        break;
    case 3:
        if (D_0032E3B0[3] & 8) {
            D_0032E3B0[3] &= ~8;
            changed = 1;
        }
        break;
    }
    func_0010D5F0(changed);
    return 1;
}

extern s32 effMiscRandMod(s32, s32);

extern s32 evtGetMirroredSolarPhase(void);

extern s32 D_0034DDF0[];

s32 func_0014F5E8(void) {
    s32 threshold = D_0034DDF0[evtGetMirroredSolarPhase()];
    if (threshold >= effMiscRandMod(0, 100)) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

s32 func_0014F658(void) {
    s32 commandName;
    commandName = func_0010D5A8(0);
    if (sdfSoundIsCommandBusy() != 0) {
        func_002E97E8();
    }
    sdfSoundSendNamedCommand(commandName, 0x7f);
    return 1;
}

u32 func_0014F6A8(void) {
    func_002E97E8();
    return 1;
}

u32 func_0014F6C8(void) {
    u64 busy;

    busy = sdfSoundIsCommandBusy();
    func_0010D5F0(busy);
    return 1;
}

extern s32 D_0032E3C0[];

extern s32 func_0010D428(s32);

extern void func_00121DE0(s32, s32, s32, s32, s32, s32);

s32 func_0014F6F0(void) {
    s32 first = func_0010D428(0);
    s32 second = func_0010D428(1);
    s32 third = func_0010D428(2);
    s32 fourth = func_0010D428(3);
    func_00121DE0(D_0032E3C0[0], first, second, third, fourth, func_0010D428(4));
    return 1;
}

s32 func_0014F780(void) {
    D_0032E4E4[0] = 1;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0014F4C8", func_0014F790);

INCLUDE_SDATA(const s32, "game/code_0014F4C8", D_003BB008);

