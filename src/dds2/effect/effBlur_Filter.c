#include "common.h"

extern u32 func_00159BB8(u32);

/* Both blur variants store a setting at 0x2c and an owned resource at 0x30. */
typedef struct EffBlurWork {
    u8 pad0[0x2C];
    u32 setting;
    u32 resource;
} EffBlurWork;

void func_0018E8F0(void) {
    func_00328E48();
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018E908);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018E980);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018EA00);

/* Select the source handle used by the first blur variant. */
void effBlurSetHandle(EffBlurWork *work, u32 setting) {
    work->setting = setting;
}

/* Acquire the same handle through the effect resource manager. */
void effBlurAcquireHandle(EffBlurWork *work) {
    u32 setting;

    setting = func_00159BB8(2);
    work->setting = setting;
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018EA98);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018EBC8);

/* Release the first variant's owned effect resource. */
void effBlurReleaseFirstResource(EffBlurWork *work) {
    func_003297C8(work->resource);
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018ECD0);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018EED8);

/* The second blur variant has its own setter for the same work layout. */
void effBlurSetSecondSetting(EffBlurWork *work, u32 setting) {
    work->setting = setting;
}

void effBlurAcquireSecondHandle(EffBlurWork *work) {
    u32 setting;

    setting = func_00159BB8(2);
    work->setting = setting;
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018EF78);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018EFE0);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018F018);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018F098);

/* Release the second variant's owned effect resource. */
void effBlurReleaseSecondResource(EffBlurWork *work) {
    func_003297C8(work->resource);
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018F1D0);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018F3C0);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018F5C0);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018F840);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00414620);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00414628);

