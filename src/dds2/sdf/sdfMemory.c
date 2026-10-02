#include "common.h"

typedef struct MemBlock {
    /* 0x0 */ struct MemBlock *prev;
    /* 0x4 */ struct MemBlock *next;
    /* 0x8 */ u32 addr;
    /* 0xC */ u16 state; /* 0 free, 1 used, 2 end marker */
    /* 0xE */ u16 unkE;
} MemBlock;

MemBlock *sdfMemoryNextBlock(MemBlock *block) {
    MemBlock *next = block->next;
    if (next->state == 2) {
        return NULL;
    }
    return next;
}

extern s32 func_0036DE70(void);

extern void EIntr(void);

s32 sdfMemoryGetBlockSize(MemBlock *block) {
    s32 size;
    s32 interruptsDisabled;

    interruptsDisabled = func_0036DE70();
    size = block->next->addr - block->addr;
    if (interruptsDisabled) {
        EIntr();
    }
    return size;
}

u32 sdfMemoryGetBlockAddress(MemBlock *block) {
    return block->addr;
}

typedef struct MemHeap {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ MemBlock *head;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ MemBlock *tail;
} MemHeap;

extern MemHeap D_0045F0F8;
extern void (*D_004389CC)(s32);
extern MemBlock *sdfAllocSizeClassBlock(s32 size);

/* First-fit allocation of `size` bytes (rounded up to 128) from the general heap: the first free block that is large enough is split if it is bigger than needed and marked used. Running into the end marker calls the out-of-memory hook. */
MemBlock *sdfAllocGeneralBlock(s32 size) {
    MemHeap *heap = &D_0045F0F8;
    s32 alignedSize = (size + 0x7F) & ~0x7F;
    s32 interruptsDisabled;
    MemBlock *block;

    interruptsDisabled = func_0036DE70();
    for (block = heap->head;; block = block->next) {
        if (block->state != 0) {
            if (block->state == 2) {
                if (interruptsDisabled != 0) {
                    EIntr();
                }
                if (D_004389CC != NULL) {
                    D_004389CC(alignedSize);
                }
            }
        } else {
            s32 available = block->next->addr - block->addr;

            if (available >= alignedSize) {
                if (alignedSize < available) {
                    MemBlock *rest = sdfAllocSizeClassBlock(0x10);

                    rest->prev = block;
                    rest->addr = block->addr + alignedSize;
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
MemBlock *sdfAllocGeneralBlockHigh(s32 size) {
    MemHeap *heap = &D_0045F0F8;
    s32 alignedSize = (size + 0x7F) & ~0x7F;
    s32 interruptsDisabled;
    MemBlock *block;

    interruptsDisabled = func_0036DE70();
    for (block = heap->tail;; block = block->prev) {
        if (block->state != 0) {
            if (block->state == 2) {
                if (interruptsDisabled != 0) {
                    EIntr();
                }
            }
        } else {
            s32 available = block->next->addr - block->addr;

            if (available >= alignedSize) {
                if (alignedSize < available) {
                    MemBlock *rest = sdfAllocSizeClassBlock(0x10);

                    rest->prev = block;
                    rest->addr = block->addr + (available - alignedSize);
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

/* Same first-fit allocation as sdfAllocGeneralBlock but without the out-of-memory hook: returns NULL when the end marker is reached. */
MemBlock *sdfTryAllocGeneralBlock(s32 size) {
    MemHeap *heap = &D_0045F0F8;
    s32 alignedSize = (size + 0x7F) & ~0x7F;
    s32 interruptsDisabled;
    MemBlock *block;

    interruptsDisabled = func_0036DE70();
    for (block = heap->head;; block = block->next) {
        if (block->state != 0) {
            if (block->state == 2) {
                if (interruptsDisabled != 0) {
                    EIntr();
                }
                return NULL;
            }
        } else {
            s32 available = block->next->addr - block->addr;

            if (available >= alignedSize) {
                if (alignedSize < available) {
                    MemBlock *rest = sdfAllocSizeClassBlock(0x10);

                    rest->prev = block;
                    rest->addr = block->addr + alignedSize;
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
