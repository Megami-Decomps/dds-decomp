#include "common.h"
#include "kwln.h"

extern u8 D_003BA709;
extern u32 D_003BA704;
extern u32 D_003BA71C;
extern u32 D_003BA720;
extern u32 D_003BA718;

extern u32 D_003BA70C;
extern s32 D_003BA710;
extern u32 D_003BA714;

extern u32 D_003BD680;

extern u32 D_003BA700;

extern s8 D_003BA708;

extern KwlnTask *kwlnDelayedStartTaskHead;
extern KwlnTask *D_003BA804;
extern s32 kwlnDelayedStartTaskCount;
extern KwlnTask *kwlnDelayedDestroyTaskHead;
extern KwlnTask *D_003BA810;
extern s32 kwlnDelayedDestroyTaskCount;
extern KwlnTask *kwlnActiveTaskHead;
extern KwlnTask *D_003BA81C;
extern s32 kwlnActiveTaskCount;

INCLUDE_ASM(const s32, "game/code_00100000", func_00100000);

INCLUDE_ASM(const s32, "game/code_00100000", _start);

INCLUDE_ASM(const s32, "game/code_00100000", func_001001C8);

INCLUDE_ASM(const s32, "game/code_00100000", func_001001D0);

INCLUDE_ASM(const s32, "game/code_00100000", func_001001D8);

void func_00100500(void) {
    D_003BA708 = 1;
}

u32 func_00100510(void) {
    return D_003BA700;
}

u32 kwlnGetDrawBufferIndex(void) {
    return D_003BD680;
}

void func_00100520(void) {
    D_003BA710 = 0;
    D_003BA70C = 1;
    D_003BA714 = 0;
}

void func_00100538(void) {
    D_003BA70C = 0;
    D_003BA710 = 0;
    D_003BA714 = 0;
}

void func_00100548(u32 value) {
    D_003BA710 = value;
    D_003BA70C = 1;
    D_003BA714 = 0;
}

void func_00100560(u32 first, u32 second) {
    u32 value = D_003BA704;
    D_003BA714 = 1;
    D_003BA718 = first;
    D_003BA71C = value;
    D_003BA720 = second;
    D_003BA70C = 0;
    D_003BA710 = 0;
}

extern s32 D_003BA710;
extern u32 D_003BA714;

void func_00100588(void) {
    D_003BA70C = 0;
    D_003BA718 = 4;
    D_003BA710 = 0;
    D_003BA714 = 0;
}

void func_001005A0(void) {
}

void func_001005A8(void) {
}

void func_001005B0(void) {
    D_003BA709 = 0;
}

void func_001005B8(void) {
    D_003BA709 = 1;
}

extern s16 D_003BA728;
extern u16 D_003BA72A;
extern s8 D_003BA734;
extern u16 D_003BD32E;
extern u16 D_003BD334;
extern u16 mnuMovieTaskState;
extern u32 func_002CF930(void);
extern u32 sdfGetElapsedTimerTicks(u32);
extern void sdfSleepThreadCount(s32);
extern void sdfSetNonnegativePacketIndex(s32);
extern void sdfPadBuildButtonStates(void);
extern void sdfRaiseDeviceThreadPriority(void);
extern void sdfRestoreDeviceThreadPriority(void);
extern void LoadExecPS2(const char *, s32, char **);

void func_001005C8(void) {
    u32 startTick;

    func_001001D8();
    D_003BD680 = 0;
    while (D_003BA708 == 0) {
        sdfSleepThreadCount(mnuMovieTaskState);
        sdfSetNonnegativePacketIndex(mnuMovieTaskState - 1);
        sdfPadBuildButtonStates();
        if (D_003BA734 != 0) {
            sdfRaiseDeviceThreadPriority();
        }
        if (D_003BA70C == 0) {
            startTick = func_002CF930();
            func_00103B10();
            if (kwlnTaskTickScheduler() == 0) {
                break;
            }
            D_003BA700++;
            D_003BA728 = sdfGetElapsedTimerTicks(startTick) - D_003BD334;
            D_003BD680 ^= 1;
            D_003BA72A = D_003BD32E;
        } else if (D_003BA710 > 0) {
            D_003BA710--;
            if (D_003BA710 == 0) {
                D_003BA70C = 0;
            }
        }
        D_003BA704++;
        if (D_003BA734 == 0) {
            sdfRestoreDeviceThreadPriority();
        }
    }
    LoadExecPS2("cdrom0:\\SLPS_999.99;1", 0, NULL);
}

/* Remove a task from the doubly linked queue of its state (1 delayed start, 2 active, 3 delayed destroy). */
void kwlnTaskRemoveFromStateQueue(KwlnTask *task) {
    switch (task->flags & 0xF) {
    case 0:
        return;
    case 1:
    case 2:
    case 3:
        break;
    default:
        return;
    }
    if (task->listPrev != NULL) {
        task->listPrev->listNext = task->listNext;
    } else {
        switch (task->flags & 0xF) {
        case 1:
            kwlnDelayedStartTaskHead = task->listNext;
            break;
        case 2:
            kwlnActiveTaskHead = task->listNext;
            break;
        case 3:
            kwlnDelayedDestroyTaskHead = task->listNext;
            break;
        }
    }
    if (task->listNext != NULL) {
        task->listNext->listPrev = task->listPrev;
    } else {
        switch (task->flags & 0xF) {
        case 1:
            D_003BA804 = task->listPrev;
            break;
        case 2:
            D_003BA81C = task->listPrev;
            break;
        case 3:
            D_003BA810 = task->listPrev;
            break;
        }
    }
    task->listNext = NULL;
    task->listPrev = NULL;
    switch (task->flags & 0xF) {
    case 1:
        kwlnDelayedStartTaskCount--;
        break;
    case 2:
        kwlnActiveTaskCount--;
        break;
    case 3:
        kwlnDelayedDestroyTaskCount--;
        break;
    }
}

void kwlnTaskInsertIntoOrderedStateQueue(KwlnTask *task) {
    KwlnTask *cur;
    KwlnTask *prev;

    switch (task->flags & 0xF) {
    case 1:
        cur = kwlnDelayedStartTaskHead;
        break;
    case 0:
        return;
    case 2:
        cur = kwlnActiveTaskHead;
        break;
    case 3:
        cur = kwlnDelayedDestroyTaskHead;
        break;
    default:
        return;
    }
    if (cur == NULL) {
        switch (task->flags & 0xF) {
        case 1:
            kwlnDelayedStartTaskHead = task;
            D_003BA804 = task;
            break;
        case 2:
            kwlnActiveTaskHead = task;
            D_003BA81C = task;
            break;
        case 3:
            kwlnDelayedDestroyTaskHead = task;
            D_003BA810 = task;
            break;
        }
        task->listPrev = NULL;
        task->listNext = NULL;
    } else {
        while (cur != NULL) {
            if (task->priority < cur->priority) {
                if (cur->listPrev != NULL) {
                    cur->listPrev->listNext = task;
                    task->listPrev = cur->listPrev;
                    task->listNext = cur;
                    cur->listPrev = task;
                } else {
                    switch (task->flags & 0xF) {
                    case 1:
                        kwlnDelayedStartTaskHead = task;
                        break;
                    case 2:
                        kwlnActiveTaskHead = task;
                        break;
                    case 3:
                        kwlnDelayedDestroyTaskHead = task;
                        break;
                    }
                    task->listPrev = NULL;
                    task->listNext = cur;
                    cur->listPrev = task;
                }
                break;
            }
            cur = cur->listNext;
        }
        if (cur == NULL) {
            switch (task->flags & 0xF) {
            case 1:
                prev = D_003BA804;
                prev->listNext = task;
                task->listPrev = prev;
                D_003BA804 = task;
                break;
            case 2:
                prev = D_003BA81C;
                prev->listNext = task;
                task->listPrev = prev;
                D_003BA81C = task;
                break;
            case 3:
                prev = D_003BA810;
                prev->listNext = task;
                task->listPrev = prev;
                D_003BA810 = task;
                break;
            }
            task->listNext = NULL;
        }
    }
    switch (task->flags & 0xF) {
    case 1:
        kwlnDelayedStartTaskCount++;
        break;
    case 2:
        kwlnActiveTaskCount++;
        break;
    case 3:
        kwlnDelayedDestroyTaskCount++;
        break;
    }
}

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA700);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA704);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA708);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA709);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA70C);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA710);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA714);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA718);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA71C);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA720);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA724);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA728);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA72A);

INCLUDE_SDATA(const s32, "game/code_00100000", mnuMovieTaskState);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA730);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA734);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA738);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA740);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA748);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA750);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA758);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA760);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA768);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA770);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA778);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA780);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA788);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA790);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA798);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7A0);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7A8);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7B0);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7B8);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7C0);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7C8);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7D0);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7D8);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7E0);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7E8);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7F0);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7F8);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7FC);

INCLUDE_SDATA(const s32, "game/code_00100000", kwlnDelayedStartTaskHead);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA804);

INCLUDE_SDATA(const s32, "game/code_00100000", kwlnDelayedStartTaskCount);

INCLUDE_SDATA(const s32, "game/code_00100000", kwlnDelayedDestroyTaskHead);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA810);

INCLUDE_SDATA(const s32, "game/code_00100000", kwlnDelayedDestroyTaskCount);

INCLUDE_SDATA(const s32, "game/code_00100000", kwlnActiveTaskHead);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA81C);

INCLUDE_SDATA(const s32, "game/code_00100000", kwlnActiveTaskCount);

