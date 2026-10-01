#include "common.h"

extern u64 sdfFindThreadNode(u64);

void func_002CFC18(void) {
    u64 thread;

    thread = sdfFindThreadNode(0xffffffffffffffff);
    func_002CFB18(thread);
}

extern s32 D_003BD9A4;
extern s32 D_003BD9A0;
extern u8 D_003BD480;
extern s32 GetThreadId(void);
extern s32 ChangeThreadPriority(s32, s32);
extern s32 iWakeupThread(s32);
extern s32 SleepThread(void);

s32 sdfWakeThreadOnCompletionEvent(s32 event) {
    if (event == 2) {
        iWakeupThread(D_003BD9A0);
        if (D_003BD480 != 0) {
            D_003BD480--;
        }
    }
    return 0;
}

typedef struct ThreadWaiter {
    struct ThreadWaiter *next; /* 0x0 */
    s32 thread;                /* 0x4 */
} ThreadWaiter;

extern s32 D_003BD2D8;
extern s32 D_003BD998;
extern ThreadWaiter *D_003BD99C;
extern void sdfAddHandler(s32, s32, s32 (*)(s32), s32, s32);
extern void func_0030B568(s32);
extern s32 WaitSema(s32);
extern s32 SignalSema(s32);
extern s32 WakeupThread(s32);

/* Completion thread: each wake-up wakes every thread registered on the waiter list. */
void func_002CFC70(void) {
    ThreadWaiter *waiter;

    sdfAddHandler(0, 2, sdfWakeThreadOnCompletionEvent, -1, 0);
    func_0030B568(2);
    for (;;) {
        SleepThread();
        D_003BD2D8++;
        WaitSema(D_003BD998);
        for (waiter = D_003BD99C; waiter != NULL; waiter = waiter->next) {
            WakeupThread(waiter->thread);
        }
        SignalSema(D_003BD998);
    }
}

INCLUDE_ASM(const s32, "game/code_002CFC18", func_002CFCF0);

s32 sdfThreadSleepSelf(void) {
    s32 thread = GetThreadId();

    D_003BD9A4 = thread;
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

INCLUDE_ASM(const s32, "game/code_002CFC18", func_002CFDF0);

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

extern SdfCursorSlot *D_003BD9AC;
extern SdfCursorNode *func_002CFDF0();

SdfCursorNode *sdfCursorSlotAlloc(SdfCursorOwner *owner) {
    SdfCursorSlot *slot = D_003BD9AC;

    D_003BD9AC = slot->next;
    slot->count = 0;
    slot->handler = func_002CFDF0;
    owner->slot = slot;
    slot->owner = owner;
    slot->limit = owner->limit;
    return func_002CFDF0(owner, slot);
}

INCLUDE_ASM(const s32, "game/code_002CFC18", func_002CFEB8);

INCLUDE_SDATA(const s32, "game/code_002CFC18", D_003BD2D8);

