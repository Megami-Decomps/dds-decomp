#include "common.h"

extern u32 func_00151FC8(u32);

/* Work area shared by both blur-filter variants in this TU. */
typedef struct {
    u8   pad_0x00[0x2C]; /* 0x00 */
    u32  unk2C;          /* 0x2C: blur source/mode set by the setters below */
    void *unk30;         /* 0x30: object released by the free helpers */
} EffBlurWork; /* 0x34 */

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_00186DC8);

void func_00186E28(EffBlurWork *work, u32 value) {
    work->unk2C = value;
}

void func_00186E30(EffBlurWork *work) {
    work->unk2C = func_00151FC8(2);
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_00186E60);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_00186F90);

void func_00187080(EffBlurWork *work) {
    func_002D0918(work->unk30);
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_00187098);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_001872A0);

void func_00187308(EffBlurWork *work, u32 value) {
    work->unk2C = value;
}

void func_00187310(EffBlurWork *work) {
    work->unk2C = func_00151FC8(2);
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_00187340);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_001873A8);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_001873E0);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_00187460);

void func_00187580(EffBlurWork *work) {
    func_002D0918(work->unk30);
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_00187598);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_00187788);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_00187988);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_00187C08);







INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_003A0F00);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_003A0F08);

