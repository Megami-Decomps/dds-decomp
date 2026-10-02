#include "common.h"
#include "ee_mmi.h"
#include "sdf.h"

#define SDF_CURRENT_THREAD_SELECTOR 0xffffffffffffffff
#define SDF_THREAD_COMPLETION_EVENT 2
#define SDF_WAKE_WORKER_STACK_BYTES 0x800
#define SDF_WAKE_WORKER_PRIORITY 0x3E
#define SDF_SLEEP_THREAD_PRIORITY 0x7C

s32 sdfThreadWakeTick __attribute__((section(".sdata"), aligned(8))) = 0;

extern u64 sdfFindThreadNode(u64);

/* The negative lookup selector selects the calling thread's tracked entry. */
void sdfUnlinkAndDeleteCurrentThread(void) {
    u64 threadNode;

    threadNode = sdfFindThreadNode(SDF_CURRENT_THREAD_SELECTOR);
    func_003289C8(threadNode);
}

extern s32 sdfThreadWakeWorkerId;
extern u8 D_00438B70;
extern s32 GetThreadId(void);
extern s32 ChangeThreadPriority(s32, s32);
extern s32 iWakeupThread(s32);
extern s32 SleepThread(void);

/* Wake the worker only for its registered event; the byte counter never underflows. */
s32 sdfWakeThreadOnCompletionEvent(s32 eventId) {
    if (eventId == SDF_THREAD_COMPLETION_EVENT) {
        iWakeupThread(sdfThreadWakeWorkerId);
        if (D_00438B70 != 0) {
            D_00438B70--;
        }
    }
    return 0;
}

extern void sdfAddHandler(s32, s32, s32 (*)(s32), s32, s32);
extern void func_003667E8(s32);
extern s32 WaitSema(s32);
extern s32 SignalSema(s32);
extern s32 WakeupThread(s32);

/* Each wake-up broadcasts to every tracked thread under the list semaphore. */
void sdfWakeQueuedThreadWaiters(void) {
    SdfThreadNode *threadEntry;

    sdfAddHandler(0, SDF_THREAD_COMPLETION_EVENT, sdfWakeThreadOnCompletionEvent, -1, 0);
    func_003667E8(SDF_THREAD_COMPLETION_EVENT);
    for (;;) {
        SleepThread();
        sdfThreadWakeTick++;
        WaitSema(sdfTrackedThreadSemaphore);
        for (threadEntry = sdfTrackedThreadHead; threadEntry != NULL; threadEntry = threadEntry->next) {
            WakeupThread(threadEntry->threadId);
        }
        SignalSema(sdfTrackedThreadSemaphore);
    }
}

extern void func_00366A78(void);
extern s32 sdfCreateSemaphore(s32, s32, s32);
extern s32 sdfCreateThread(void (*)(void), void *, s32, s32);
extern char D_0045E8A0[];
extern s32 _StartThread(s32, s32);

/* Initialize the tracked-thread list and its semaphore before starting the wake worker. */
s32 sdfStartQueuedThreadWakeWorker(void) {
    s32 threadId;

    func_00366A78();
    sdfThreadWakeTick = 0;
    sdfTrackedThreadHead = NULL;
    sdfTrackedThreadSemaphore = sdfCreateSemaphore(1, 1, 0);
    threadId = sdfCreateThread(sdfWakeQueuedThreadWaiters, D_0045E8A0, SDF_WAKE_WORKER_STACK_BYTES, SDF_WAKE_WORKER_PRIORITY);
    sdfThreadWakeWorkerId = threadId;
    return _StartThread(threadId, 0);
}

extern s32 sdfSleepThreadId;

/* Record the calling thread, set its sleep priority, then return the SDK sleep result. */
s32 sdfThreadSleepSelf(void) {
    s32 threadId = GetThreadId();

    sdfSleepThreadId = threadId;
    ChangeThreadPriority(threadId, SDF_SLEEP_THREAD_PRIORITY);
    return SleepThread();
}

typedef struct SdfCursorLink {
    struct SdfCursorLink *next;
} SdfCursorLink;

typedef struct {
    SdfCursorLink *current;
    SdfCursorLink *next;
} SdfNodeCursor;

/* Promote the next list node to current; an empty list clears current. */
void sdfAdvanceNodeCursor(SdfNodeCursor *cursor) {
    SdfCursorLink *nextNode;

    nextNode = cursor->next;
    if (nextNode != (SdfCursorLink *)0x0) {
        cursor->next = nextNode->next;
    }
    cursor->current = nextNode;
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

/* Pop a recycled block, count it live, and advance the size-class cursor when this slot fills. */
SdfCursorNode *sdfAdvanceCursorWalk(SdfCursorState *sizeClassState, SdfCursorWalk *slotState) {
    SdfCursorNode *freeBlock = slotState->node;

    slotState->visited++;
    slotState->node = freeBlock->next;
    if (slotState->visited == sizeClassState->limit) {
        sdfAdvanceNodeCursor(&sizeClassState->cursor);
    }
    return freeBlock;
}
typedef struct SdfCursorSlot {
    struct SdfCursorSlot *next;
    SdfCursorNode *base;
    SdfCursorNode *(*handler)();
    struct SdfCursorOwner *owner;
    SdfCursorNode *node;
    u16 count;
    s16 limit;
    s16 visited;
} SdfCursorSlot;

typedef struct SdfCursorOwner {
    SdfCursorSlot *slot;
    SdfCursorSlot *next;
    s16 stride;
    s16 limit;
} SdfCursorOwner;

extern SdfCursorSlot *sdfFreeCursorSlotHead;
SdfCursorNode *func_00328CA0(SdfCursorOwner *owner, SdfCursorSlot *slot) {
    s16 ownerLimit = owner->limit;
    s32 count = slot->count + 1;
    s32 remaining = slot->limit;
    SdfCursorNode *node;

    slot->count = count;
    node = (SdfCursorNode *)((u8 *)slot->base + (ownerLimit - remaining) * owner->stride);
    slot->limit = remaining - 1;
    if (slot->limit == 0) {
        slot->handler = (SdfCursorNode *(*)())sdfAdvanceCursorWalk;
        if ((s16)count == ownerLimit) {
            sdfAdvanceNodeCursor((SdfNodeCursor *)owner);
        }
    }
    return node;
}

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

extern SdfCursorOwner D_0045F0A0[];
extern s32 func_0036DE70(void);
extern void EIntr(void);

/* Allocate a block of `size` bytes from the size class that covers it (classes are powers of two from 16 bytes up); each class hands out from its own slot list. */
SdfCursorNode *sdfAllocSizeClassBlock(s32 size) {
    s32 index = 0;
    SdfCursorOwner *owner;
    SdfCursorNode *result;
    s32 interruptsDisabled;

    if (size > 16) {
        EE_MMI_PLZCW(index, size - 1);
        index = 27 - (index & 0xFF);
    }
    owner = &D_0045F0A0[index];
    interruptsDisabled = func_0036DE70();
    if (owner->slot == NULL) {
        result = sdfCursorSlotAlloc(owner);
    } else {
        result = owner->slot->handler(owner, owner->slot);
    }
    if (interruptsDisabled != 0) {
        EIntr();
    }
    return result;
}
