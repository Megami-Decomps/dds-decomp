#include "common.h"

extern u64 sdfFindThreadNode(u64);

void sdfUnlinkAndDeleteCurrentThread(void) {
    u64 node;

    node = sdfFindThreadNode(0xffffffffffffffff);
    func_003289C8(node);
}

extern s32 D_00439100;
extern u8 D_00438B70;
extern s32 GetThreadId(void);
extern s32 ChangeThreadPriority(s32, s32);
extern s32 iWakeupThread(s32);
extern s32 SleepThread(void);

s32 sdfWakeThreadOnCompletionEvent(s32 event) {
    if (event == 2) {
        iWakeupThread(D_00439100);
        if (D_00438B70 != 0) {
            D_00438B70--;
        }
    }
    return 0;
}

typedef struct ThreadWaiter {
    struct ThreadWaiter *next; /* 0x0 */
    s32 thread;                /* 0x4 */
} ThreadWaiter;

extern s32 sdfThreadWakeTick;
extern s32 sdfTrackedThreadSemaphore;
extern ThreadWaiter *sdfTrackedThreadHead;
extern void sdfAddHandler(s32, s32, s32 (*)(s32), s32, s32);
extern void func_003667E8(s32);
extern s32 WaitSema(s32);
extern s32 SignalSema(s32);
extern s32 WakeupThread(s32);

/* Completion thread: each wake-up wakes every thread registered on the waiter list. */
void sdfWakeQueuedThreadWaiters(void) {
    ThreadWaiter *waiter;

    sdfAddHandler(0, 2, sdfWakeThreadOnCompletionEvent, -1, 0);
    func_003667E8(2);
    for (;;) {
        SleepThread();
        sdfThreadWakeTick++;
        WaitSema(sdfTrackedThreadSemaphore);
        for (waiter = sdfTrackedThreadHead; waiter != NULL; waiter = waiter->next) {
            WakeupThread(waiter->thread);
        }
        SignalSema(sdfTrackedThreadSemaphore);
    }
}

extern void func_00366A78(void);
extern s32 sdfCreateSemaphore(s32, s32, s32);
extern s32 sdfCreateThread(void (*)(void), void *, s32, s32);
extern char D_0045E8A0[];
extern s32 _StartThread(s32, s32);

s32 sdfStartQueuedThreadWakeWorker(void) {
    s32 thread;

    func_00366A78();
    sdfThreadWakeTick = 0;
    sdfTrackedThreadHead = NULL;
    sdfTrackedThreadSemaphore = sdfCreateSemaphore(1, 1, 0);
    thread = sdfCreateThread(sdfWakeQueuedThreadWaiters, D_0045E8A0, 0x800, 0x3E);
    D_00439100 = thread;
    return _StartThread(thread, 0);
}

extern s32 sdfSleepThreadId;

s32 sdfThreadSleepSelf(void) {
    s32 thread = GetThreadId();

    sdfSleepThreadId = thread;
    ChangeThreadPriority(thread, 0x7C);
    return SleepThread();
}

typedef struct SdfNode {
    struct SdfNode *next;
} SdfNode;

typedef struct {
    SdfNode *current;
    SdfNode *next;
} SdfNodeCursor;

void sdfAdvanceNodeCursor(SdfNodeCursor *cursor) {
    SdfNode *next;

    next = cursor->next;
    if (next != (SdfNode *)0x0) {
        cursor->next = next->next;
    }
    cursor->current = next;
}

typedef struct SdfCursorNode {
    u8 unk0[8];
    struct SdfCursorNode *next;
} SdfCursorNode;

typedef struct SdfCursorState {
    SdfNodeCursor cursor;
    u8 unk8[2];
    s16 limit;
} SdfCursorState;

typedef struct SdfCursorWalk {
    u8 unk0[0x10];
    SdfCursorNode *node;
    s16 visited;
} SdfCursorWalk;

SdfCursorNode *sdfAdvanceCursorWalk(SdfCursorState *state, SdfCursorWalk *walk) {
    SdfCursorNode *node = walk->node;

    walk->visited++;
    walk->node = node->next;
    if (walk->visited == state->limit) {
        sdfAdvanceNodeCursor(&state->cursor);
    }
    return node;
}

INCLUDE_ASM(const s32, "game/code_00328AC8", func_00328CA0);

typedef struct SdfCursorSlot {
    struct SdfCursorSlot *next;
    u8 unk4[4];
    SdfCursorNode *(*handler)();
    struct SdfCursorOwner *owner;
    SdfCursorNode *node;
    s16 count;
    u16 limit;
} SdfCursorSlot;

typedef struct SdfCursorOwner {
    SdfCursorSlot *slot;
    u8 unk4[6];
    u16 limit;
} SdfCursorOwner;

extern SdfCursorSlot *sdfFreeCursorSlotHead;
extern SdfCursorNode *func_00328CA0();

SdfCursorNode *sdfCursorSlotAlloc(SdfCursorOwner *owner) {
    SdfCursorSlot *slot = sdfFreeCursorSlotHead;

    sdfFreeCursorSlotHead = slot->next;
    slot->count = 0;
    slot->handler = func_00328CA0;
    owner->slot = slot;
    slot->owner = owner;
    slot->limit = owner->limit;
    return func_00328CA0(owner, slot);
}

INCLUDE_ASM(const s32, "game/code_00328AC8", func_00328D68);

INCLUDE_SDATA(const s32, "game/code_00328AC8", sdfThreadWakeTick);

