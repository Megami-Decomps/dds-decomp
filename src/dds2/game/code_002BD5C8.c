#include "common.h"

INCLUDE_ASM(const s32, "game/code_002BD5C8", func_002BD5C8);

INCLUDE_ASM(const s32, "game/code_002BD5C8", func_002BD710);

INCLUDE_ASM(const s32, "game/code_002BD5C8", func_002BD9E0);

INCLUDE_ASM(const s32, "game/code_002BD5C8", func_002BDA50);

INCLUDE_ASM(const s32, "game/code_002BD5C8", func_002BDA78);

INCLUDE_ASM(const s32, "game/code_002BD5C8", func_002BDAA8);

INCLUDE_ASM(const s32, "game/code_002BD5C8", func_002BDC38);

INCLUDE_ASM(const s32, "game/code_002BD5C8", func_002BE080);

INCLUDE_ASM(const s32, "game/code_002BD5C8", func_002BE138);

INCLUDE_ASM(const s32, "game/code_002BD5C8", func_002BE240);

INCLUDE_ASM(const s32, "game/code_002BD5C8", func_002BE438);

void mnuBlendPanelSlots(s32 dst, s32 src, u32 amount) {
    s32 i;
    s32 ctx = *(s32 *)(dst + 0x18);

    for (i = 0; i < 4; i++) {
        s32 off = i * 4 + 0x80;
        s32 result = func_00309138(*(s32 *)(ctx + off + 4),
                                   *(s32 *)(*(s32 *)(src + 0x18) + off + 4),
                                   (s32)amount / 2 + 0x80, ctx);
        s32 current = *(s32 *)(dst + 0x18);
        ctx = current;
        *(s32 *)(current + i * 4 + 0x14) = result;
    }
}

INCLUDE_RODATA(const s32, "game/code_002BD5C8", D_0042B028);

INCLUDE_RODATA(const s32, "game/code_002BD5C8", D_0042B048);

INCLUDE_RODATA(const s32, "game/code_002BD5C8", D_0042B098);

INCLUDE_SDATA(const s32, "game/code_002BD5C8", D_00437C38);

INCLUDE_SDATA(const s32, "game/code_002BD5C8", D_00437C40);

INCLUDE_SDATA(const s32, "game/code_002BD5C8", D_00437C48);

INCLUDE_SDATA(const s32, "game/code_002BD5C8", D_00437C50);

