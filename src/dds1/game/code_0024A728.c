#include "common.h"

typedef struct MenuFadeHost {
    u8 pad00[0x84];
    s32 reduced;      /* 0x84 */
    u8 pad88[0xCC];
    s32 fadeColor;    /* 0x154 */
} MenuFadeHost;

extern void sndStartTrackExtended(s32);

extern void func_002E9708(void);

extern void func_002E96D8(s32);

extern void func_002E9730(void);

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
    u32 callback;         /* 0xC4 */
    u32 previousCallback; /* 0xC8 */
} SceneTransition;

void evtRememberDispatchCallback(u32 callback, SceneTransition *transition) {
    u32 previous;

    previous = transition->callback;
    transition->callback = callback;
    transition->previousCallback = previous;
}

INCLUDE_ASM(const s32, "game/code_0024A728", func_0024B2E0);

INCLUDE_ASM(const s32, "game/code_0024A728", func_0024B358);

INCLUDE_RODATA(const s32, "game/code_0024A728", D_003AF6B0);

INCLUDE_RODATA(const s32, "game/code_0024A728", D_003AF6E0);

INCLUDE_RODATA(const s32, "game/code_0024A728", D_003AF6F0);

INCLUDE_RODATA(const s32, "game/code_0024A728", D_003AF700);

