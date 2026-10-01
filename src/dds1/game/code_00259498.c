#include "common.h"

extern void sdfReleaseChipBlock(void *);
/* Retail retains a jal and epilogue; default TU -O2 changes the shape. */

extern s32 func_002D03F8(s32);

extern void *sdfMemoryGetBlockAddress(s32);

extern f32 effMiscRandUnitFloat(s32);

extern void *memset(void *, s32, u32);

extern f32 sdfSinPoly(f32);

extern void func_0024E260(s32, s32, s32, s32, s32, s32);

INCLUDE_ASM(const s32, "game/code_00259498", func_00259498);

INCLUDE_ASM(const s32, "game/code_00259498", func_00259890);

INCLUDE_ASM(const s32, "game/code_00259498", func_00259B40);

INCLUDE_ASM(const s32, "game/code_00259498", func_0025A680);

void func_0025AA20(s32 frame, s32 size, s32 param) {
    f32 x = frame;
    f32 scale = size;
    f32 t;

    t = x > 5.0f ? (x - 5.0f) / 40.0f : 0.0f;
    t = sdfSinPoly(t * 3.14159265f) * 0.2f + 0.1f;
    func_0024E260(0, -10, 0, (s32)(scale * t), 0, param);
    t = x < 35.0f ? x / 35.0f : 0.0f;
    t = sdfSinPoly(t * 3.14159265f) * 0.5f + 0.5f;
    func_0024E260(0, 0, 0, (s32)(scale * t), 0, param);
}

void func_0025AB38(s32 frame, s32 size, s32 param) {
    f32 x = frame;
    f32 scale = size;
    f32 t;

    t = x > 5.0f ? (x - 5.0f) / 40.0f : 0.0f;
    t = sdfSinPoly(t * 3.14159265f) * 0.2f + 0.1f;
    func_0024E260(10, 0, 0, (s32)(scale * t), 1, param);
    t = x < 35.0f ? x / 35.0f : 0.0f;
    t = sdfSinPoly(t * 3.14159265f) * 0.5f + 0.5f;
    func_0024E260(0, 0, 0, (s32)(scale * t), 1, param);
}

void func_0025AC50(s32 frame, s32 size, s32 param) {
    f32 x = frame;
    f32 scale = size;
    f32 t;

    t = x > 5.0f ? (x - 5.0f) / 40.0f : 0.0f;
    t = sdfSinPoly(t * 3.14159265f) * 0.2f + 0.1f;
    func_0024E260(0, 10, 0, (s32)(scale * t), 2, param);
    t = x < 35.0f ? x / 35.0f : 0.0f;
    t = sdfSinPoly(t * 3.14159265f) * 0.5f + 0.5f;
    func_0024E260(0, 0, 0, (s32)(scale * t), 2, param);
}

void func_0025AD68(s32 frame, s32 size, s32 param) {
    f32 x = frame;
    f32 scale = size;
    f32 t;

    t = x > 5.0f ? (x - 5.0f) / 40.0f : 0.0f;
    t = sdfSinPoly(t * 3.14159265f) * 0.2f + 0.1f;
    func_0024E260(-10, 0, 0, (s32)(scale * t), 3, param);
    t = x < 35.0f ? x / 35.0f : 0.0f;
    t = sdfSinPoly(t * 3.14159265f) * 0.5f + 0.5f;
    func_0024E260(0, 0, 0, (s32)(scale * t), 3, param);
}

INCLUDE_ASM(const s32, "game/code_00259498", func_0025AE80);

INCLUDE_ASM(const s32, "game/code_00259498", func_0025B0F0);

INCLUDE_ASM(const s32, "game/code_00259498", func_0025B350);

void mnuReleaseOptionalDrawAllocation(void *unused, void *allocation) {
    if (allocation != 0) {
        sdfReleaseChipBlock(allocation);
    }
}


typedef struct {
    s32 allocation;    /* 0x00 */
    u8 pad04[0x2C];
    s32 lifetime;      /* 0x30 */
    s32 owner;         /* 0x34 */
    u8 pad38[0xC];
    u8 variant;       /* 0x44 */
    u8 sprite;        /* 0x45 */
    u8 pad46[2];
} MovieSpriteResource;

void *mnuCreateSpriteResource(s32 owner, u8 sprite, u8 variant) {
    s32 allocation = func_002D03F8(0x48);
    MovieSpriteResource *resource = sdfMemoryGetBlockAddress(allocation);
    memset(resource, 0, 0x48);
    resource->allocation = allocation;
    resource->owner = owner;
    resource->sprite = sprite;
    resource->variant = variant;
    resource->lifetime = (s32)(effMiscRandUnitFloat(0) * 30.0f + 10.0f);
    return resource;
}

INCLUDE_ASM(const s32, "game/code_00259498", func_0025B888);

INCLUDE_RODATA(const s32, "game/code_00259498", D_003AF9F0);

INCLUDE_SDATA(const s32, "game/code_00259498", D_003BC4A0);

INCLUDE_SDATA(const s32, "game/code_00259498", D_003BC4A8);

INCLUDE_SDATA(const s32, "game/code_00259498", D_003BC4B0);

INCLUDE_SDATA(const s32, "game/code_00259498", D_003BC4B8);

INCLUDE_SDATA(const s32, "game/code_00259498", D_003BC4C0);

INCLUDE_SDATA(const s32, "game/code_00259498", D_003BC4C8);

INCLUDE_SDATA(const s32, "game/code_00259498", D_003BC4CC);

