#include "common.h"
#include "kwln.h"

extern u8 D_003BA709;

extern u32 D_003BA70C;
extern u32 D_003BA710;
extern u32 D_003BA714;

extern u32 D_003BD680;

extern u32 D_003BA700;

extern u8 D_003BA708;

extern KwlnTask *kwlnDelayedStartTaskHead;
extern KwlnTask *D_003BA804;
extern s32 D_003BA808;
extern KwlnTask *kwlnDelayedDestroyTaskHead;
extern KwlnTask *D_003BA810;
extern s32 D_003BA814;
extern KwlnTask *D_003BA818;
extern KwlnTask *D_003BA81C;
extern s32 D_003BA820;

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

u32 func_00100518(void) {
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

INCLUDE_ASM(const s32, "game/code_00100000", func_00100560);

extern u32 D_003BA710;
extern u32 D_003BA714;
extern u32 D_003BA718;

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

INCLUDE_ASM(const s32, "game/code_00100000", func_001005C8);

INCLUDE_ASM(const s32, "game/code_00100000", func_001006E0);

void func_00100858(KwlnTask *task) {
    KwlnTask *cur;
    KwlnTask *prev;

    switch (task->flags & 0xF) {
    case 1:
        cur = kwlnDelayedStartTaskHead;
        break;
    case 0:
        return;
    case 2:
        cur = D_003BA818;
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
            D_003BA818 = task;
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
                        D_003BA818 = task;
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
        D_003BA808++;
        break;
    case 2:
        D_003BA820++;
        break;
    case 3:
        D_003BA814++;
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

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA72C);

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

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA808);

INCLUDE_SDATA(const s32, "game/code_00100000", kwlnDelayedDestroyTaskHead);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA810);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA814);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA818);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA81C);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA820);

