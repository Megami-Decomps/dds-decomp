#include "common.h"

extern u64 sdfSoundIsCommandBusy(void);

extern u64 func_0014A250(void);

u32 func_0014F4C8(void) {
    u64 value;

    value = func_0014A250();
    func_0010D5F0(value);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0014F4C8", func_0014F4F0);

INCLUDE_ASM(const s32, "game/code_0014F4C8", func_0014F5E8);

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

INCLUDE_ASM(const s32, "game/code_0014F4C8", func_0014F6F0);

INCLUDE_ASM(const s32, "game/code_0014F4C8", func_0014F780);

INCLUDE_ASM(const s32, "game/code_0014F4C8", func_0014F790);

INCLUDE_SDATA(const s32, "game/code_0014F4C8", D_003BB008);

