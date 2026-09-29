#include "common.h"

extern u32 D_003E274C[];

INCLUDE_ASM(const s32, "game/code_002D00F8", func_002D00F8);

INCLUDE_ASM(const s32, "game/code_002D00F8", func_002D01F0);

INCLUDE_ASM(const s32, "game/code_002D00F8", func_002D02C0);

typedef struct {
    u8 pad00[0xC];
    u16 state; /* 0x0C: 0 free, 1 used, 2 end marker */
} SdfMemBlockPrefix;

u16 func_002D0378(SdfMemBlockPrefix *block) {
    return block->state;
}

u32 func_002D0380(void) {
    return D_003E274C[0];
}

INCLUDE_SDATA(const s32, "game/code_002D00F8", D_003BD2DC);

INCLUDE_SDATA(const s32, "game/code_002D00F8", D_003BD2E0);

INCLUDE_SDATA(const s32, "game/code_002D00F8", D_003BD2E1);

