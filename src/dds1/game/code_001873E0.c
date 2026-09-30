#include "common.h"

/* Work area shared by both blur-filter variants in this TU. */
typedef struct {
    u8   pad_0x00[0x2C]; /* 0x00 */
    u32  setting;        /* 0x2C: source handle/setting used by both variants */
    void *resource;      /* 0x30: object released by the free helpers */
} EffBlurWork; /* 0x34 */

/* Quad written into each slot at +0x8. */
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

extern void *func_002D03F8(s32 size);
extern void *sdfResourceRetainAddress(void *allocation);
extern u32 func_00151FC8(s32 index);

extern void func_001873A8(EffBlurWork2 *work, EffBlurSlot2 *slot);

void effBlurSecondInitSlots(EffBlurWork2 *work) {
    EffBlurSlot2 *slot = work->slots;
    s32 count = work->count;
    s32 i;

    for (i = 0; i < count; i++, slot++) {
        func_001873A8(work, slot);
        slot->phase = -(work->spacing * (f32)i);
    }
}

/* Allocate a slot array for the second variant and seed every slot. */
EffBlurWork2 *func_00187460(EffBlurWork2 *src) {
    s32 count = src->count;
    void *allocation = func_002D03F8(count * 0x30 + 0x38);
    EffBlurWork2 *work = (EffBlurWork2 *)sdfResourceRetainAddress(allocation);
    EffBlurSlot2 *slot;
    s32 i = 0;

    memcpy(work, src, 0x2C);
    work->resource = allocation;
    work->slots = (EffBlurSlot2 *)((u8 *)work + 0x38);
    work->setting = func_00151FC8(3);
    slot = work->slots;
    while (i < count) {
        func_001873A8(work, slot);
        slot->phase = -(work->spacing * (f32)i);
        i++;
        slot = (EffBlurSlot2 *)((u8 *)slot + 0x30);
    }
    return work;
}

/* Release the second variant's owned effect resource. */
void effBlurReleaseSecondResource(EffBlurWork *work) {
    func_002D0918(work->resource);
}

INCLUDE_ASM(const s32, "game/code_001873E0", func_00187598);

INCLUDE_ASM(const s32, "game/code_001873E0", func_00187788);

INCLUDE_ASM(const s32, "game/code_001873E0", func_00187988);

INCLUDE_ASM(const s32, "game/code_001873E0", func_00187C08);

