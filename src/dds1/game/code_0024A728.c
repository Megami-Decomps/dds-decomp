#include "common.h"

INCLUDE_ASM(const s32, "game/code_0024A728", func_0024A728);

INCLUDE_ASM(const s32, "game/code_0024A728", func_0024A930);

INCLUDE_ASM(const s32, "game/code_0024A728", func_0024AB28);

INCLUDE_ASM(const s32, "game/code_0024A728", func_0024AB70);

INCLUDE_ASM(const s32, "game/code_0024A728", func_0024ACD8);

INCLUDE_ASM(const s32, "game/code_0024A728", func_0024AE18);

INCLUDE_ASM(const s32, "game/code_0024A728", func_0024AF58);

INCLUDE_ASM(const s32, "game/code_0024A728", func_0024B090);

INCLUDE_ASM(const s32, "game/code_0024A728", func_0024B168);

typedef struct {
    u8 pad00[0xC4];
    u32 currentValue;  /* 0xC4 */
    u32 previousValue; /* 0xC8 */
} SceneTransition;

void func_0024B2D0(u32 value, SceneTransition *transition) {
    u32 previous;

    previous = transition->currentValue;
    transition->currentValue = value;
    transition->previousValue = previous;
}

INCLUDE_ASM(const s32, "game/code_0024A728", func_0024B2E0);

INCLUDE_ASM(const s32, "game/code_0024A728", func_0024B358);

INCLUDE_RODATA(const s32, "game/code_0024A728", D_003AF6B0);

INCLUDE_RODATA(const s32, "game/code_0024A728", D_003AF6E0);

INCLUDE_RODATA(const s32, "game/code_0024A728", D_003AF6F0);

INCLUDE_RODATA(const s32, "game/code_0024A728", D_003AF700);

