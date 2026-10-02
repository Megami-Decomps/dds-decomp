#include "common.h"

typedef struct SdfCursorSlot {
    struct SdfCursorSlot *next;
    u8 unk4[4];
    void *(*handler)();
    struct SdfChipOwner *owner;
    void *node;
    s16 count;
    u16 limit;
} SdfCursorSlot;

typedef struct SdfChipOwner {
    SdfCursorSlot *slot;
    SdfCursorSlot *next;
    u8 unk8[2];
    s16 limit;
} SdfChipOwner;

typedef struct SdfChipBlock {
    u8 pad0[8];
    void *next;
} SdfChipBlock;

extern SdfCursorSlot *sdfFreeCursorSlotHead;
extern s32 D_003BD9A8;
extern s32 sdfChipHeapStart;
extern s32 D_003BD9B4;
extern u8 D_003BD9C0;

void *sdfAllocSizeClassBlock(s32 arg0);
void *sdfClearQuadwords(void *arg0, s32 arg1);
void sdfPendingQueuePush(void *arg0, s32 arg1);
s32 func_00312C08(void);
s32 EIntr(void);
void sdfAdvanceNodeCursor(SdfChipOwner *owner);

void sdfAllocAndClearQuadwords(s32 size) {
    return sdfClearQuadwords(sdfAllocSizeClassBlock(size), (size + 15) >> 4);
}

void sdfReleaseChipBlock(void *memory) {
    SdfCursorSlot *slot;
    SdfChipOwner *owner;
    SdfChipBlock *block = memory;
    SdfCursorSlot **link;
    s32 interrupts;
    s16 count;
    s32 index;

    if (memory == NULL) {
        return;
    }
    index = (s32)memory - sdfChipHeapStart;
    if (index < 0) {
        index += 0xFFF;
    }
    slot = (SdfCursorSlot *)(D_003BD9A8 + (index >> 12) * 24);
    owner = slot->owner;
    interrupts = func_00312C08();
    count = slot->count;
    slot->count = count - 1;
    if (slot->count == 0) {
        link = &owner->next;
        if (owner->slot == slot) {
            sdfAdvanceNodeCursor(owner);
        } else {
            while (*link != slot) {
                link = &(*link)->next;
            }
            *link = slot->next;
        }
        slot->owner = NULL;
        slot->next = sdfFreeCursorSlotHead;
        sdfFreeCursorSlotHead = slot;
    } else {
        if (count == owner->limit) {
            slot->next = owner->next;
            owner->next = slot;
        }
        block->next = slot->node;
        slot->node = block;
    }
    if (interrupts != 0) {
        EIntr();
    }
}

void sdfQueuePendingChipValue(s32 value) {
    sdfPendingQueuePush(&D_003BD9C0, value);
}

s32 sdfChipIsInRange(s32 address) {
    s32 inRange;

    inRange = 0;
    if (address >= sdfChipHeapStart) {
        inRange = address < D_003BD9B4;
    }
    return inRange;
}
