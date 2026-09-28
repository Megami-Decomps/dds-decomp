#include "common.h"

extern s32 kwlnFadeIsActive(void);

extern s32 func_0024A6C0(s32);

extern s8 D_003BC3E0;

extern s64 func_0024DC08(void);

extern s64 func_00285670(s32, s32 *, u64, u64);

extern s32 func_00101A70();

INCLUDE_ASM(const s32, "game/code_00249FA8", func_00249FA8);

INCLUDE_ASM(const s32, "game/code_00249FA8", func_0024A058);

s32 fldPollSceneState(void) {
    s32 state = D_003BC3E0;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        D_003BC3E0 = 0;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00249FA8", func_0024A0D8);

void func_0024A138(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 1, arg0);
}

INCLUDE_ASM(const s32, "game/code_00249FA8", func_0024A170);

s32 func_0024A1A8(void) {
    s32 temp_v0 = kwlnFadeIsActive();

    if (temp_v0 != 0) {
        return 0;
    }
    return func_0024DC08() == 0;
}

INCLUDE_ASM(const s32, "game/code_00249FA8", func_0024A1D8);

typedef struct {
    u8 pad00[6];
    u16 previousA; /* 0x06 */
    u16 currentA;  /* 0x08 */
    u16 previousB; /* 0x0A */
    u16 currentB;  /* 0x0C */
    u16 flags;     /* 0x0E */
} SceneOptionRecord;

void func_0024A2B8(SceneOptionRecord *option) {
    u16 flags = option->flags;
    u16 currentA = option->currentA;
    u16 currentB = option->currentB;
    u16 retainedFlags = flags & 0xfa2f;

    option->previousA = currentA;
    option->previousB = currentB;
    option->flags = retainedFlags;
}

INCLUDE_ASM(const s32, "game/code_00249FA8", func_0024A2D8);

INCLUDE_ASM(const s32, "game/code_00249FA8", func_0024A340);

s32 fldClassifyRemainingFrames(s32 arg0) {
    s32 value = *(s32 *)(arg0 + 0x9C);
    if (value == 0) {
        return 0;
    }
    return value >= 60 ? 2 : 1;
}

INCLUDE_ASM(const s32, "game/code_00249FA8", func_0024A4A0);

INCLUDE_ASM(const s32, "game/code_00249FA8", func_0024A570);

INCLUDE_ASM(const s32, "game/code_00249FA8", func_0024A610);

INCLUDE_ASM(const s32, "game/code_00249FA8", func_0024A6C0);

u8 func_0024A6E8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0024A6C0(arg0);
    return *(u8 *)(temp_v0 * 0xa0 + *(s32 *)(*(s32 *)(arg0 + 100) + 0x18) + 0x14);
}

INCLUDE_RODATA(const s32, "game/code_00249FA8", D_003AF658);

INCLUDE_RODATA(const s32, "game/code_00249FA8", D_003AF668);

INCLUDE_RODATA(const s32, "game/code_00249FA8", D_003AF678);

INCLUDE_RODATA(const s32, "game/code_00249FA8", D_003AF688);

INCLUDE_RODATA(const s32, "game/code_00249FA8", D_003AF6A0);

INCLUDE_SDATA(const s32, "game/code_00249FA8", D_003BC400);

