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

extern SdfCursorSlot *D_003BD9AC;
extern s32 D_003BD9A8;
extern s32 D_003BD9B0;
extern s32 D_003BD9B4;
extern u8 D_003BD9C0;

void *func_002CFEB8(s32 arg0);
void *sdfClearQuadwords(void *arg0, s32 arg1);
void func_002D3C30(void *arg0, s32 arg1);
s32 func_00312C08(void);
s32 EIntr(void);
void sdfAdvanceNodeCursor(SdfChipOwner *owner);

void func_002CFF68(s32 size) {
    return sdfClearQuadwords(func_002CFEB8(size), (size + 15) >> 4);
}

void func_002CFF98(void *arg0) {
    SdfCursorSlot *slot;
    SdfChipOwner *owner;
    SdfChipBlock *block = arg0;
    SdfCursorSlot **link;
    s32 interrupts;
    s16 count;
    s32 index;

    if (arg0 == NULL) {
        return;
    }
    index = (s32)arg0 - D_003BD9B0;
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
        slot->next = D_003BD9AC;
        D_003BD9AC = slot;
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

void func_002D00B8(s32 value) {
    func_002D3C30(&D_003BD9C0, value);
}

s32 sdfChipIsInRange(s32 address) {
    s32 inRange;

    inRange = 0;
    if (address >= D_003BD9B0) {
        inRange = address < D_003BD9B4;
    }
    return inRange;
}
