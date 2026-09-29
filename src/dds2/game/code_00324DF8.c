#include "common.h"

extern u64 func_003283E0(u64);

extern u64 func_0035A828(u64);

extern u64 func_00325BB0(u64, u32);

extern u64 func_00325AB8(u64, u32);

extern u64 func_00325790(u64, u32);

extern s32 CancelWakeupThread(u64);

extern u64 GetThreadId(void);

extern s32 CreateSema(void *);

extern u32 D_004389BC;

extern u8 D_00438A8C;

extern void (*D_004389C4)(void);

extern void sdfSleepThreadCount(s32);

extern void func_0033C820(void);

extern void func_0033C878(void);

extern void sdfTickThreadPriorityOverride(void);

extern f32 func_00326A40(u32);

extern f32 func_003532B8(f32);

typedef struct ResourceNode {
    u32 id;
    u32 value;
    struct ResourceNode *next;
    u32 unk_C;
    u32 handle;
} ResourceNode;

typedef struct ResourceList {
    u32 count;
    ResourceNode *first;
} ResourceList;

u32 func_00320F68(u32 list, u32 node);

ResourceNode *func_00321170();

ResourceNode *func_003211B0();

void func_00324DF8(u32 *arg0, u32 arg1) {
    func_00320CE0(*arg0, 0, arg1);
}

u32 func_00324E18(u32 *pair, u32 key, u32 value) {
    u32 node = func_00321170(pair[0], key);
    if (node) {
        return func_00320D80(pair[0], node, 0, value);
    }
    return 0;
}

void func_00324E80(u32 *pair, u32 key) {
    u32 node = func_00321170(pair[0], key);
    if (node == 0) {
        return;
    }
    func_00320F68(pair[1], func_003211B0(pair[1], *(u32 *)(node + 0x10)));
    func_00320F68(pair[0], node);
}

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00324EF0);

void func_00324F20(ResourceList **list) {
    func_003211B0(*list);
}

void func_00324F38(ResourceList **list) {
    func_00321170(*list);
}

u64 func_00324F50(s32 arg0, u64 arg1) {
    u64 temp_v0;

    temp_v0 = func_0035A828(arg1);
    func_00320CE0(*(u32 *)(arg0 + 4), 0, temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00324F98);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00324FD0);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_003251C0);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00325398);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_003255A0);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00325688);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00325790);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00325AB8);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00325BB0);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00325CC8);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00325EC8);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00326018);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00326158);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_003262A8);

void func_003268A8(float *vector, float *delta) {
    *vector = *vector + *delta;
    vector[1] = vector[1] + delta[1];
    vector[2] = vector[2] + delta[2];
}

void func_003268E0(float *vector, float *delta) {
    *vector = *vector - *delta;
    vector[1] = vector[1] - delta[1];
    vector[2] = vector[2] - delta[2];
}

void func_00326918(float *vec, float x, float y, float z) {
    vec[0] += x;
    vec[1] += y;
    vec[2] += z;
}

void func_00326940(float *vec, float x, float y, float z) {
    vec[0] = x;
    vec[1] = y;
    vec[2] = z;
}

void func_00326950(float factor, float *vector) {
    *vector = *vector * factor;
    vector[1] = vector[1] * factor;
    vector[2] = vector[2] * factor;
}

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00326978);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_003269F0);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00326A40);

f32 func_00326AE0(u32 entry) {
    return func_003532B8(func_00326A40(entry));
}

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00326B00);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00326BC8);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_003270C8);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_003275C8);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00327AC8);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00327BD8);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00327C80);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00328018);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00328160);

void func_003282E8(void) {
}

s32 sdfCreateSemaphore(u32 initial, u32 option, u32 maximum) {
    struct {
        u32 attr;
        u32 option;
        u32 initial;
        u32 reserved[2];
        u32 maximum;
    } sema;

    sema.initial = initial;
    sema.option = option;
    sema.maximum = maximum;
    return CreateSema(&sema);
}

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00328318);

void func_00328390(u64 arg0, u64 arg1, u64 arg2) {
    u64 temp_v0;

    temp_v0 = func_003283E0(arg1);
    func_00328318(arg0, temp_v0, arg1, arg2);
}

INCLUDE_ASM(const s32, "game/code_00324DF8", func_003283E0);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00328420);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00328470);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_003284C8);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00328520);

INCLUDE_ASM(const s32, "game/code_00324DF8", sdfAddHandler);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00328668);

void sdfDrainPendingHandlers(void) {
    u32 current;
    while ((current = D_004389BC) != 0) {
        func_00328668(current);
    }
}

void sdfWakeAlarmThread(u32 unused0, u32 unused1, u32 threadId) {
    iWakeupThread(threadId);
}

INCLUDE_ASM(const s32, "game/code_00324DF8", sdfSleepWithAlarm);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_003287E0);

u32 sdfGetElapsedTimerTicks(u32 base) {
    u32 now;

    now = *(volatile u32 *)0x10000000;
    return (now - base) & 0xFFFF;
}

void func_00328808(void) {
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

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00328858);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00328918);

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

INCLUDE_ASM(const s32, "game/code_00324DF8", func_003289C8);
INCLUDE_SDATA(const s32, "game/code_00324DF8", D_004389BC);

INCLUDE_SDATA(const s32, "game/code_00324DF8", D_004389C0);

INCLUDE_SDATA(const s32, "game/code_00324DF8", D_004389C4);

