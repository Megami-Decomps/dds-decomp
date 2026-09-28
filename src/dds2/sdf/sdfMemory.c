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

INCLUDE_ASM(const s32, "sdf/sdfMemory", sdfMemoryGetBlockSize);

u32 sdfMemoryGetBlockAddress(MemBlock *block) {
    return block->addr;
}

INCLUDE_ASM(const s32, "sdf/sdfMemory", func_003292A8);

INCLUDE_ASM(const s32, "sdf/sdfMemory", func_003293C8);

INCLUDE_ASM(const s32, "sdf/sdfMemory", func_003294F8);
