#include "common.h"

extern u32 func_00151FC8(u32);

/* Work area shared by both blur-filter variants in this TU. */
typedef struct {
    u8   pad_0x00[0x2C]; /* 0x00 */
    u32  setting;        /* 0x2C: source handle/setting used by both variants */
    void *resource;      /* 0x30: object released by the free helpers */
} EffBlurWork; /* 0x34 */

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_00186DC8);

/* Supply a source handle for the first blur variant. */
void effBlurSetSetting(EffBlurWork *work, u32 setting) {
    work->setting = setting;
}

/* Acquire the first variant's source handle from the effect manager. */
void effBlurAcquireHandle(EffBlurWork *work) {
    work->setting = func_00151FC8(2);
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_00186E60);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_00186F90);

/* Release the first variant's owned effect resource. */
void effBlurReleaseFirstResource(EffBlurWork *work) {
    func_002D0918(work->resource);
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_00187098);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_001872A0);

/* The second variant uses the same work layout but separate callbacks. */
void effBlurSetSecondSetting(EffBlurWork *work, u32 setting) {
    work->setting = setting;
}

void effBlurAcquireSecondHandle(EffBlurWork *work) {
    work->setting = func_00151FC8(2);
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_00187340);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_001873A8);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_001873E0);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_00187460);

/* Release the second variant's owned effect resource. */
void effBlurReleaseSecondResource(EffBlurWork *work) {
    func_002D0918(work->resource);
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_00187598);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_00187788);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_00187988);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_00187C08);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_003A0F00);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_003A0F08);

