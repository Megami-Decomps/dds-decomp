#include "common.h"

INCLUDE_ASM(const s32, "game/code_002BD5C8", func_002BD5C8);

INCLUDE_ASM(const s32, "game/code_002BD5C8", func_002BD710);

s32 func_002BD9E0(s32 arg0, s32 arg1, s32 arg2, u32 *window, s32 arg4) {
    s32 result = func_002BD5C8(window, arg4);

    switch (result) {
    case 1:
        *window &= ~2;
        return 1;
    case 2:
        *window &= ~2;
        return 1;
    default:
        return 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002BD5C8", func_002BDA50);

INCLUDE_ASM(const s32, "game/code_002BD5C8", func_002BDA78);

INCLUDE_ASM(const s32, "game/code_002BD5C8", func_002BDAA8);

INCLUDE_ASM(const s32, "game/code_002BD5C8", func_002BDC38);

INCLUDE_ASM(const s32, "game/code_002BD5C8", func_002BE080);

INCLUDE_ASM(const s32, "game/code_002BD5C8", func_002BE138);

INCLUDE_ASM(const s32, "game/code_002BD5C8", func_002BE240);

INCLUDE_ASM(const s32, "game/code_002BD5C8", func_002BE438);

typedef struct MenuBlendContext {
    u8 pad00[0x14];
    s32 result[4];
    u8 pad24[0x60];
    s32 source[4];
} MenuBlendContext;

typedef struct MenuBlendObject {
    u8 pad00[0x18];
    MenuBlendContext *context;
} MenuBlendObject;

void mnuBlendPanelSlots(MenuBlendObject *dst, MenuBlendObject *src, u32 amount) {
    s32 i;
    s32 ctx = (s32)dst->context;

    for (i = 0; i < 4; i++) {
        s32 result = uiBlendColors(((MenuBlendContext *)ctx)->source[i],
                                   src->context->source[i],
                                   (s32)amount / 2 + 0x80, ctx);
        s32 current = (s32)dst->context;
        ctx = current;
        ((MenuBlendContext *)current)->result[i] = result;
    }
}
INCLUDE_RODATA(const s32, "game/code_002BD5C8", D_0042B028);

INCLUDE_RODATA(const s32, "game/code_002BD5C8", D_0042B048);

INCLUDE_RODATA(const s32, "game/code_002BD5C8", D_0042B098);

INCLUDE_SDATA(const s32, "game/code_002BD5C8", D_00437C38);

INCLUDE_SDATA(const s32, "game/code_002BD5C8", D_00437C40);

INCLUDE_SDATA(const s32, "game/code_002BD5C8", D_00437C48);

INCLUDE_SDATA(const s32, "game/code_002BD5C8", D_00437C50);

