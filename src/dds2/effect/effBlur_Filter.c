#include "common.h"

extern u32 func_00159BB8(u32);

typedef struct BlurFilter {
    u8 pad0[0x2C];
    u32 texture;
    u32 resource;
} BlurFilter;

void func_0018E8F0(void) {
    func_00328E48();
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018E908);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018E980);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018EA00);

void func_0018EA60(BlurFilter *blur, u32 texture) {
    blur->texture = texture;
}

void func_0018EA68(BlurFilter *blur) {
    u32 texture;

    texture = func_00159BB8(2);
    blur->texture = texture;
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018EA98);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018EBC8);

void func_0018ECB8(BlurFilter *blur) {
    func_003297C8(blur->resource);
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018ECD0);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018EED8);

void func_0018EF40(BlurFilter *blur, u32 texture) {
    blur->texture = texture;
}

void func_0018EF48(BlurFilter *blur) {
    u32 texture;

    texture = func_00159BB8(2);
    blur->texture = texture;
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018EF78);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018EFE0);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018F018);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018F098);

void func_0018F1B8(BlurFilter *blur) {
    func_003297C8(blur->resource);
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018F1D0);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018F3C0);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018F5C0);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018F840);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00414620);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00414628);

