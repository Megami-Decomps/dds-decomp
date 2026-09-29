#include "common.h"

extern s32 CancelWakeupThread(u64);

extern u64 GetThreadId(void);

extern u8 D_00438A8C;

extern void (*D_004389C4)(void);

extern void sdfSleepThreadCount(s32);

extern void func_0033C820(void);

extern void func_0033C878(void);

extern void sdfTickThreadPriorityOverride(void);

void sdfWakeAlarmThread(u32 unused0, u32 unused1, u32 threadId) {
    iWakeupThread(threadId);
}

void sdfSleepWithAlarm(u32 delay) {
    u64 thread = GetThreadId();
    CancelWakeupThread(thread);
    SetAlarm(delay & 0xFFFF, sdfWakeAlarmThread, thread);
    SleepThread();
}

INCLUDE_ASM(const s32, "game/code_00328778", func_003287E0);

u32 sdfGetElapsedTimerTicks(u32 base) {
    u32 now;

    now = *(volatile u32 *)0x10000000;
    return (now - base) & 0xFFFF;
}

void sdfRunTickWorkerThread(void) {
    for (;;) {
        sdfSleepThreadCount(1);
        func_0033C820();
        if (!D_00438A8C) {
            func_0033C878();
        }
        sdfTickThreadPriorityOverride();
        if (D_004389C4 != NULL) {
            D_004389C4();
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00328778", func_00328858);

INCLUDE_ASM(const s32, "game/code_00328778", func_00328918);

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

INCLUDE_ASM(const s32, "game/code_00328778", func_003289C8);
