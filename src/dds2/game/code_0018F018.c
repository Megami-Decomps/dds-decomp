#include "common.h"

/* Both blur variants store a setting at 0x2c and an owned resource at 0x30. */
typedef struct EffBlurWork {
    u8 pad0[0x2C];
    u32 setting;
    u32 resource;
} EffBlurWork;

INCLUDE_ASM(const s32, "game/code_0018F018", func_0018F018);

INCLUDE_ASM(const s32, "game/code_0018F018", func_0018F098);

/* Release the second variant's owned effect resource. */
void effBlurReleaseSecondResource(EffBlurWork *work) {
    func_003297C8(work->resource);
}

INCLUDE_ASM(const s32, "game/code_0018F018", func_0018F1D0);

INCLUDE_ASM(const s32, "game/code_0018F018", func_0018F3C0);

INCLUDE_ASM(const s32, "game/code_0018F018", func_0018F5C0);

INCLUDE_ASM(const s32, "game/code_0018F018", func_0018F840);

