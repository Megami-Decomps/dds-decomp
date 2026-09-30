#include "common.h"

extern u64 sdfFindThreadNode(u64);

void func_00328AC8(void) {
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

s32 func_00328AE8(s32 event) {
    if (event == 2) {
        iWakeupThread(D_00439100);
        if (D_00438B70 != 0) {
            D_00438B70--;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00328AC8", func_00328B20);

INCLUDE_ASM(const s32, "game/code_00328AC8", func_00328BA0);

extern s32 D_00439104;

s32 sdfThreadSleepSelf(void) {
    s32 thread = GetThreadId();

    D_00439104 = thread;
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

extern SdfCursorSlot *D_0043910C;
extern SdfCursorNode *func_00328CA0();

SdfCursorNode *sdfCursorSlotAlloc(SdfCursorOwner *owner) {
    SdfCursorSlot *slot = D_0043910C;

    D_0043910C = slot->next;
    slot->count = 0;
    slot->handler = func_00328CA0;
    owner->slot = slot;
    slot->owner = owner;
    slot->limit = owner->limit;
    return func_00328CA0(owner, slot);
}

INCLUDE_ASM(const s32, "game/code_00328AC8", func_00328D68);

INCLUDE_SDATA(const s32, "game/code_00328AC8", D_004389C8);

