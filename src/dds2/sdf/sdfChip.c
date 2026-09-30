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

extern SdfCursorSlot *D_0043910C;
extern s32 D_00439108;
extern s32 D_00439110;
extern s32 D_00439114;
extern u8 D_00439120;

extern u64 func_00328D68(void);

void func_0032CAE0(void *arg0, s32 arg1);

s32 func_0036DE70(void);
s32 EIntr(void);
void sdfAdvanceNodeCursor(SdfChipOwner *owner);

void func_00328E18(s32 size) {
    u64 allocation;

    allocation = func_00328D68();
    sdfClearQuadwords(allocation, (size + 0xf) >> 4);
}

void func_00328E48(void *arg0) {
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
    index = (s32)arg0 - D_00439110;
    if (index < 0) {
        index += 0xFFF;
    }
    slot = (SdfCursorSlot *)(D_00439108 + (index >> 12) * 24);
    owner = slot->owner;
    interrupts = func_0036DE70();
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
        slot->next = D_0043910C;
        D_0043910C = slot;
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

void func_00328F68(s32 value) {
    func_0032CAE0(&D_00439120, value);
}

s32 sdfChipIsInRange(s32 address) {
    s32 withinRange;

    withinRange = 0;
    if (address >= D_00439110) {
        withinRange = address < D_00439114;
    }
    return withinRange;
}