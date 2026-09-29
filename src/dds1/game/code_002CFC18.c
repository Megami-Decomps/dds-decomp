#include "common.h"

extern u64 sdfFindThreadById(u64);

void func_002CFC18(void) {
    u64 thread;

    thread = sdfFindThreadById(0xffffffffffffffff);
    func_002CFB18(thread);
}

extern s32 D_003BD9A4;
extern s32 D_003BD9A0;
extern u8 D_003BD480;
extern s32 GetThreadId(void);
extern s32 ChangeThreadPriority(s32, s32);
extern s32 iWakeupThread(s32);
extern s32 SleepThread(void);

s32 func_002CFC38(s32 event) {
    if (event == 2) {
        iWakeupThread(D_003BD9A0);
        if (D_003BD480 != 0) {
            D_003BD480--;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002CFC18", func_002CFC70);

INCLUDE_ASM(const s32, "game/code_002CFC18", func_002CFCF0);

s32 func_002CFD50(void) {
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
    struct SdfCursorNode *link;
} SdfCursorNode;

typedef struct SdfCursorState {
    SdfNodeCursor cursor;
    u8 unk8[2];
    s16 limit;
} SdfCursorState;

typedef struct SdfCursorWalk {
    u8 unk0[0x10];
    SdfCursorNode *node;
    s16 count;
} SdfCursorWalk;

SdfCursorNode *func_002CFDA0(SdfCursorState *state, SdfCursorWalk *walk) {
    SdfCursorNode *node = walk->node;

    walk->count++;
    walk->node = node->link;
    if (walk->count == state->limit) {
        sdfAdvanceNodeCursor(&state->cursor);
    }
    return node;
}

INCLUDE_ASM(const s32, "game/code_002CFC18", func_002CFDF0);

typedef struct SdfCursorSlot {
    struct SdfCursorSlot *next;
    u8 unk4[4];
    void *handler;
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

SdfCursorNode *func_002CFE70(SdfCursorOwner *owner) {
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

