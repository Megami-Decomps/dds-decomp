#include "common.h"
#include "kwln.h"

extern u8 D_00435B88;
extern u32 D_00435B84;
extern u32 D_00435B9C;
extern u32 D_00435BA0;

extern u32 D_00435B80;

extern u32 D_00438D80;

extern u32 D_00435B8C;

extern u32 D_00435B90;

extern u32 D_00435B94;
extern u32 D_00435B98;

extern u8 D_00435B89;

extern KwlnTask *kwlnDelayedStartTaskHead;
extern KwlnTask *D_00435BD4;
extern s32 kwlnDelayedStartTaskCount;
extern KwlnTask *kwlnDelayedDestroyTaskHead;
extern KwlnTask *D_00435BE0;
extern s32 kwlnDelayedDestroyTaskCount;
extern KwlnTask *kwlnActiveTaskHead;
extern KwlnTask *D_00435BEC;
extern s32 kwlnActiveTaskCount;

INCLUDE_ASM(const s32, "game/code_00100000", func_00100000);

INCLUDE_ASM(const s32, "game/code_00100000", _start);

INCLUDE_ASM(const s32, "game/code_00100000", func_001001C8);

INCLUDE_ASM(const s32, "game/code_00100000", func_001001D0);

INCLUDE_ASM(const s32, "game/code_00100000", func_001001D8);

void func_001003E8(void) {
    D_00435B88 = 1;
}

u32 func_001003F8(void) {
    return D_00435B80;
}

u32 kwlnGetDrawBufferIndex(void) {
    return D_00438D80;
}

void func_00100408(void) {
    D_00435B90 = 0;
    D_00435B8C = 1;
    D_00435B94 = 0;
}

void func_00100420(void) {
    D_00435B8C = 0;
    D_00435B90 = 0;
    D_00435B94 = 0;
}

void func_00100430(u32 value) {
    D_00435B90 = value;
    D_00435B8C = 1;
    D_00435B94 = 0;
}

void func_00100448(u32 first, u32 second) {
    u32 value = D_00435B84;
    D_00435B94 = 1;
    D_00435B98 = first;
    D_00435B9C = value;
    D_00435BA0 = second;
    D_00435B8C = 0;
    D_00435B90 = 0;
}

void func_00100470(void) {
    D_00435B8C = 0;
    D_00435B98 = 4;
    D_00435B90 = 0;
    D_00435B94 = 0;
}

void func_00100488(void) {
}

void func_00100490(void) {
}

void func_00100498(void) {
    D_00435B89 = 0;
}

void func_001004A0(void) {
    D_00435B89 = 1;
}

INCLUDE_ASM(const s32, "game/code_00100000", func_001004B0);

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
            D_00435BD4 = task->listPrev;
            break;
        case 2:
            D_00435BEC = task->listPrev;
            break;
        case 3:
            D_00435BE0 = task->listPrev;
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
            D_00435BD4 = task;
            break;
        case 2:
            kwlnActiveTaskHead = task;
            D_00435BEC = task;
            break;
        case 3:
            kwlnDelayedDestroyTaskHead = task;
            D_00435BE0 = task;
            break;
        }
        task->listPrev = NULL;
        task->listNext = NULL;
    } else {
        while (cur != NULL) {
            if (task->unk20 < cur->unk20) {
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
                prev = D_00435BD4;
                prev->listNext = task;
                task->listPrev = prev;
                D_00435BD4 = task;
                break;
            case 2:
                prev = D_00435BEC;
                prev->listNext = task;
                task->listPrev = prev;
                D_00435BEC = task;
                break;
            case 3:
                prev = D_00435BE0;
                prev->listNext = task;
                task->listPrev = prev;
                D_00435BE0 = task;
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

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435B80);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435B84);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435B88);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435B89);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435B8C);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435B90);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435B94);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435B98);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435B9C);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BA0);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BA4);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BA8);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BAA);

INCLUDE_SDATA(const s32, "game/code_00100000", mnuMovieTaskState);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BB0);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BB4);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BB8);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BC0);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BC8);

INCLUDE_SDATA(const s32, "game/code_00100000", kwlnDelayedStartTaskHead);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BD4);

INCLUDE_SDATA(const s32, "game/code_00100000", kwlnDelayedStartTaskCount);

INCLUDE_SDATA(const s32, "game/code_00100000", kwlnDelayedDestroyTaskHead);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BE0);

INCLUDE_SDATA(const s32, "game/code_00100000", kwlnDelayedDestroyTaskCount);

INCLUDE_SDATA(const s32, "game/code_00100000", kwlnActiveTaskHead);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BEC);

INCLUDE_SDATA(const s32, "game/code_00100000", kwlnActiveTaskCount);

