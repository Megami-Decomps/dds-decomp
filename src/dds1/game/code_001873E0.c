#include "common.h"

/* Work area shared by both blur-filter variants in this TU. */
typedef struct {
    u8   pad_0x00[0x2C]; /* 0x00 */
    u32  setting;        /* 0x2C: source handle/setting used by both variants */
    void *resource;      /* 0x30: object released by the free helpers */
} EffBlurWork; /* 0x34 */

INCLUDE_ASM(const s32, "game/code_001873E0", func_001873E0);

INCLUDE_ASM(const s32, "game/code_001873E0", func_00187460);

/* Release the second variant's owned effect resource. */
void effBlurReleaseSecondResource(EffBlurWork *work) {
    func_002D0918(work->resource);
}

INCLUDE_ASM(const s32, "game/code_001873E0", func_00187598);

INCLUDE_ASM(const s32, "game/code_001873E0", func_00187788);

INCLUDE_ASM(const s32, "game/code_001873E0", func_00187988);

INCLUDE_ASM(const s32, "game/code_001873E0", func_00187C08);

