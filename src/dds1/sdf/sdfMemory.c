#include "common.h"

typedef struct MemBlock {
    /* 0x0 */ struct MemBlock *prev;
    /* 0x4 */ struct MemBlock *next;
    /* 0x8 */ u32 addr;
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

MemBlock *func_002D0390(MemBlock *block) {
    MemBlock *next = block->next;
    if (next->state == 2) {
        return NULL;
    }
    return next;
}

s32 func_002D03A8(MemBlock *block) {
    s32 size;
    s32 intr;

    intr = func_00312C08();
    size = block->next->addr - block->addr;
    if (intr) {
        EIntr();
    }
    return size;
}

u32 func_002D03F0(MemBlock *block) {
    return block->addr;
}

INCLUDE_ASM(const s32, "sdf/sdfMemory", func_002D03F8);

INCLUDE_ASM(const s32, "sdf/sdfMemory", func_002D0518);

INCLUDE_ASM(const s32, "sdf/sdfMemory", func_002D0648);
