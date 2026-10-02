#include "common.h"

typedef struct MemBlock {
    /* 0x0 */ struct MemBlock *prev;
    /* 0x4 */ struct MemBlock *next;
    /* 0x8 */ u32 address;
    /* 0xC */ u16 state; /* 0 free, 1 used, 2 end marker */
    /* 0xE */ u16 unkE;
} MemBlock;

typedef struct MemHeap {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ MemBlock *head;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ MemBlock *tail;
} MemHeap;

extern MemHeap D_003E2748;
extern void (*D_003BD2DC)(s32);

s32 func_00312C08(void);
s32 EIntr(void);
MemBlock *func_002CFEB8(s32 size);

MemBlock *sdfMemoryNextBlock(MemBlock *block) {
    MemBlock *next = block->next;
    if (next->state == 2) {
        return NULL;
    }
    return next;
}

s32 sdfMemoryGetBlockSize(MemBlock *block) {
    s32 size;
    s32 interruptsDisabled;

    interruptsDisabled = func_00312C08();
    size = block->next->address - block->address;
    if (interruptsDisabled) {
        EIntr();
    }
    return size;
}

u32 sdfMemoryGetBlockAddress(MemBlock *block) {
    return block->address;
}

/* First-fit allocation of `size` bytes (rounded up to 128) from the general heap: the first free block that is large enough is split if it is bigger than needed and marked used. Running into the end marker calls the out-of-memory hook. */
MemBlock *func_002D03F8(s32 size) {
    MemHeap *heap = &D_003E2748;
    s32 alignedSize = (size + 0x7F) & ~0x7F;
    s32 interruptsDisabled;
    MemBlock *block;

    interruptsDisabled = func_00312C08();
    for (block = heap->head;; block = block->next) {
        if (block->state != 0) {
            if (block->state == 2) {
                if (interruptsDisabled != 0) {
                    EIntr();
                }
                if (D_003BD2DC != NULL) {
                    D_003BD2DC(alignedSize);
                }
            }
        } else {
            s32 available = block->next->address - block->address;

            if (available >= alignedSize) {
                if (alignedSize < available) {
                    MemBlock *rest = func_002CFEB8(0x10);

                    rest->prev = block;
                    rest->address = block->address + alignedSize;
                    rest->state = 0;
                    rest->next = block->next;
                    rest->unkE = 0;
                    block->next->prev = rest;
                    block->next = rest;
                }
                block->state = 1;
                if (interruptsDisabled != 0) {
                    EIntr();
                }
                return block;
            }
        }
    }
}

/* Allocation of `size` bytes (rounded up to 128) from the high end of the general heap: walk back from the tail to the first free block that is large enough and carve the request off its upper end. */
MemBlock *func_002D0518(s32 size) {
    MemHeap *heap = &D_003E2748;
    s32 alignedSize = (size + 0x7F) & ~0x7F;
    s32 interruptsDisabled;
    MemBlock *block;

    interruptsDisabled = func_00312C08();
    for (block = heap->tail;; block = block->prev) {
        if (block->state != 0) {
            if (block->state == 2) {
                if (interruptsDisabled != 0) {
                    EIntr();
                }
            }
        } else {
            s32 available = block->next->address - block->address;

            if (available >= alignedSize) {
                if (alignedSize < available) {
                    MemBlock *rest = func_002CFEB8(0x10);

                    rest->prev = block;
                    rest->address = block->address + (available - alignedSize);
                    rest->state = 0;
                    rest->next = block->next;
                    rest->unkE = 0;
                    block->next->prev = rest;
                    block->next = rest;
                    block = rest;
                }
                block->state = 1;
                if (interruptsDisabled != 0) {
                    EIntr();
                }
                return block;
            }
        }
    }
}

/* Same first-fit allocation as func_002D03F8 but without the out-of-memory hook: returns NULL when the end marker is reached. */
MemBlock *func_002D0648(s32 size) {
    MemHeap *heap = &D_003E2748;
    s32 alignedSize = (size + 0x7F) & ~0x7F;
    s32 interruptsDisabled;
    MemBlock *block;

    interruptsDisabled = func_00312C08();
    for (block = heap->head;; block = block->next) {
        if (block->state != 0) {
            if (block->state == 2) {
                if (interruptsDisabled != 0) {
                    EIntr();
                }
                return NULL;
            }
        } else {
            s32 available = block->next->address - block->address;

            if (available >= alignedSize) {
                if (alignedSize < available) {
                    MemBlock *rest = func_002CFEB8(0x10);

                    rest->prev = block;
                    rest->address = block->address + alignedSize;
                    rest->state = 0;
                    rest->next = block->next;
                    rest->unkE = 0;
                    block->next->prev = rest;
                    block->next = rest;
                }
                block->state = 1;
                if (interruptsDisabled != 0) {
                    EIntr();
                }
                return block;
            }
        }
    }
}
