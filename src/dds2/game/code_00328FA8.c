#include "common.h"

typedef struct {
    u8 pad00[0xC];
    u16 state; /* 0x0C: 0 free, 1 used, 2 end marker */
} SdfMemBlockPrefix;

extern u32 D_0045F0FC[];

INCLUDE_ASM(const s32, "game/code_00328FA8", func_00328FA8);

INCLUDE_ASM(const s32, "game/code_00328FA8", func_003290A0);

INCLUDE_ASM(const s32, "game/code_00328FA8", func_00329170);

u16 sdfGetMemoryBlockState(SdfMemBlockPrefix *block) {
    return block->state;
}

u32 func_00329230(void) {
    return D_0045F0FC[0];
}

INCLUDE_SDATA(const s32, "game/code_00328FA8", D_004389CC);

INCLUDE_SDATA(const s32, "game/code_00328FA8", D_004389D0);

INCLUDE_SDATA(const s32, "game/code_00328FA8", D_004389D1);

