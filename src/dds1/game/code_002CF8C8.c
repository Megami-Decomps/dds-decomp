#include "common.h"

extern u8 D_003BD39C;

extern void (*D_003BD2D4)(void);

extern void sdfSleepThreadCount(s32);

extern void func_002E3970(void);

extern void func_002E39C8(void);

extern void sdfTickThreadPriorityOverride(void);

extern s32 CancelWakeupThread(u64);

extern u64 GetThreadId(void);

void sdfWakeAlarmThread(u32 unused0, u32 unused1, u32 threadId) {
    iWakeupThread(threadId);
}

/* Clear any pending wakeup before arming a 16-bit delay for this thread. */
void sdfSleepWithAlarm(u32 delay) {
    u64 threadId = GetThreadId();
    CancelWakeupThread(threadId);
    SetAlarm(delay & 0xFFFF, sdfWakeAlarmThread, threadId);
    SleepThread();
}

INCLUDE_ASM(const s32, "game/code_002CF8C8", func_002CF930);

/* EE timer 0 count register; subtraction is reduced modulo 2^16 so
 * wraparound does not make short elapsed intervals negative. */
u32 sdfGetElapsedTimerTicks(u32 previous) {
    u32 current;

    current = *(volatile u32 *)0x10000000;
    return (current - previous) & 0xFFFF;
}

void sdfRunTickWorkerThread(void) {
    for (;;) {
        sdfSleepThreadCount(1);
        func_002E3970();
        if (!D_003BD39C) {
            func_002E39C8();
        }
        sdfTickThreadPriorityOverride();
        if (D_003BD2D4 != NULL) {
            D_003BD2D4();
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002CF8C8", func_002CF9A8);

INCLUDE_ASM(const s32, "game/code_002CF8C8", func_002CFA68);

/* A previously queued wakeup counts toward the requested sleep count. */
void sdfSleepThreadCount(s32 count) {
    u64 threadId;
    s32 cancelledWakeup;

    threadId = GetThreadId();
    cancelledWakeup = CancelWakeupThread(threadId);
    count = count - cancelledWakeup;
    do {
        count = count - 1;
        SleepThread();
    } while (0 < count);
}

INCLUDE_ASM(const s32, "game/code_002CF8C8", func_002CFB18);
INCLUDE_SDATA(const s32, "game/code_002CF8C8", D_003BD2D0);

INCLUDE_SDATA(const s32, "game/code_002CF8C8", D_003BD2D4);

