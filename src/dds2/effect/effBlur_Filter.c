#include "common.h"

extern u32 func_00159BB8(u32);

/* Both blur variants store a setting at 0x2c and an owned resource at 0x30. */
typedef struct EffBlurWork {
    u8 pad0[0x2C];
    u32 setting;
    u32 resource;
} EffBlurWork;

/* Parameter block at the start of a blur work (0x2C bytes). */
typedef struct {
    u32 word[11];
} EffBlurParams;

/* Quad written into each slot at +0x8: two words copied from the work, a float,
   the on-screen position and the four edges around it. */
typedef struct {
    s32 unk0;   /* 0x00 */
    s32 unk4;   /* 0x04 */
    f32 unk8;   /* 0x08 */
    s32 unkC;   /* 0x0C */
    s32 x;      /* 0x10 */
    s32 y;      /* 0x14 */
    s32 left;   /* 0x18 */
    s32 top;    /* 0x1C */
    s32 right;  /* 0x20 */
    s32 bottom; /* 0x24 */
} EffBlurQuad; /* 0x28 */

/* Second blur variant: one 0x30-byte slot per step, led by a float phase. */
typedef struct {
    f32 phase;
    s32 unk4;
    EffBlurQuad quad;
} EffBlurSlot2; /* 0x30 */

typedef struct {
    s32 count;           /* 0x00: number of slots */
    s32 unk4;            /* 0x04 */
    f32 spacing;         /* 0x08: phase step between slots */
    s32 unkC;            /* 0x0C */
    s32 unk10;           /* 0x10 */
    f32 unk14;           /* 0x14 */
    s32 unk18;           /* 0x18 */
    s32 unk1C;           /* 0x1C */
    s32 x;               /* 0x20 */
    s32 y;               /* 0x24 */
    s32 size;            /* 0x28 */
    u32 setting;         /* 0x2C */
    void *resource;      /* 0x30 */
    EffBlurSlot2 *slots; /* 0x34 */
} EffBlurWork2; /* 0x38 */

void func_0018E8F0(void) {
    func_00328E48();
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018E908);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018E980);

void func_0018EA00(EffBlurParams *dst, EffBlurParams *src) {
    *dst = *src;
}

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

/* Copy the parameter block but keep the destination's own first word. */
void func_0018EED8(EffBlurParams *dst, EffBlurParams *src) {
    u32 first = dst->word[0];

    *dst = *src;
    dst->word[0] = first;
}

/* The second blur variant has its own setter for the same work layout. */
void effBlurSetSecondSetting(EffBlurWork *work, u32 setting) {
    work->setting = setting;
}

void effBlurAcquireSecondHandle(EffBlurWork *work) {
    u32 setting;

    setting = func_00159BB8(2);
    work->setting = setting;
}

void func_0018EF78(EffBlurWork2 *work, EffBlurSlot2 *slot) {
    f32 size = (f32)work->size * slot->phase * 16.0f;
    s32 cx = (work->x + 0x100) << 4;
    s32 cy = (work->y + 0xE0) << 3;
    s32 s = (s32)size;

    slot->quad.left = cx - s;
    slot->quad.right = cx + s;
    s >>= 1;
    slot->quad.top = cy - s;
    slot->quad.bottom = cy + s;
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018EFE0);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00414620);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00414628);

