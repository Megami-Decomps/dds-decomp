#include "common.h"
extern f32 D_00433528;

typedef struct EffMathWork {
    u8 pad00[8];
    u32 resource;
} EffMathWork;

typedef struct EffMathSlot {
    u8 pad00[0x30];
    s32 unk30;
    f32 unk34;
} EffMathSlot;

typedef struct EffMathSlotList {
    EffMathSlot *slots;
} EffMathSlotList;

void effMathReleaseWorkResource(EffMathWork *work) {
    func_003297C8(work->resource);
}

INCLUDE_ASM(const s32, "effect/effMath", func_00195BE0);

INCLUDE_ASM(const s32, "effect/effMath", func_00195D00);

void func_00195E10(EffMathSlotList *list, s32 index) {
    EffMathSlot *slot = &list->slots[index];
    slot->unk34 = 0.05f;
    slot->unk30 = 0;
}

s32 effMathGetSlotAt(EffMathSlotList *list, s32 index) {
    return (s32)&list->slots[index];
}
