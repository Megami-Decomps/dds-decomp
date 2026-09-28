#include "common.h"

typedef struct {
    u8 pad00[8];
    u32 resourceHandle; /* 0x08 */
} EffMathWork;

void effMathReleaseWorkResource(EffMathWork *work) {
    func_002D0918(work->resourceHandle);
}

INCLUDE_ASM(const s32, "effect/effMath", func_0018DFA8);

INCLUDE_ASM(const s32, "effect/effMath", func_0018E0C8);

typedef struct EffMathSlot {
    u8 unk0[0x30];
    s32 unk30; /* 0x30 */
    f32 unk34; /* 0x34 */
} EffMathSlot; /* 0x38 */

typedef struct EffMathSlots {
    EffMathSlot *slots;
} EffMathSlots;

void func_0018E1D8(EffMathSlots *table, s32 index) {
    EffMathSlot *slot = &table->slots[index];

    slot->unk34 = 0.05f;
    slot->unk30 = 0;
}

s32 func_0018E200(s32 *base, s32 index) {
    return *base + index * 0x38;
}
