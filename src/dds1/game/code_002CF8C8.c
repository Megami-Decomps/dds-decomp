#include "common.h"

extern u8 D_003BD39C;

extern void (*D_003BD2D4)(void);

extern void func_002CFAD8(s32);

extern void func_002E3970(void);

extern void func_002E39C8(void);

extern void func_002E70B0(void);

extern s32 CancelWakeupThread(u64);

extern u64 GetThreadId(void);

void func_002CF8C8(u32 arg0, u32 arg1, u32 arg2) {
    iWakeupThread(arg2);
}

void sleepWithAlarm(u32 delay) {
    u64 thread = GetThreadId();
    CancelWakeupThread(thread);
    SetAlarm(delay & 0xFFFF, func_002CF8C8, thread);
    SleepThread();
}

INCLUDE_ASM(const s32, "game/code_002CF8C8", func_002CF930);

u32 func_002CF940(u32 base) {
    u32 now;

    now = *(volatile u32 *)0x10000000;
    return (now - base) & 0xFFFF;
}

void func_002CF958(void) {
    for (;;) {
        func_002CFAD8(1);
        func_002E3970();
        if (!D_003BD39C) {
            func_002E39C8();
        }
        func_002E70B0();
        if (D_003BD2D4 != NULL) {
            D_003BD2D4();
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002CF8C8", func_002CF9A8);

INCLUDE_ASM(const s32, "game/code_002CF8C8", func_002CFA68);

void func_002CFAD8(s32 arg0) {
    u64 temp_v0;
    s32 temp_v1;

    temp_v0 = GetThreadId();
    temp_v1 = CancelWakeupThread(temp_v0);
    arg0 = arg0 - temp_v1;
    do {
        arg0 = arg0 - 1;
        SleepThread();
    } while (0 < arg0);
}

INCLUDE_ASM(const s32, "game/code_002CF8C8", func_002CFB18);
INCLUDE_SDATA(const s32, "game/code_002CF8C8", D_003BD2D0);

INCLUDE_SDATA(const s32, "game/code_002CF8C8", D_003BD2D4);

