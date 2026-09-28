#include "common.h"

typedef struct EffMathWork {
    u8 pad00[8];
    u32 resource;
} EffMathWork;

typedef struct EffMathSlot {
    u8 data[0x38];
} EffMathSlot;

typedef struct EffMathSlotList {
    EffMathSlot *slots;
} EffMathSlotList;

void effMathReleaseWorkResource(EffMathWork *work) {
    func_003297C8(work->resource);
}

INCLUDE_ASM(const s32, "effect/effMath", func_00195BE0);

INCLUDE_ASM(const s32, "effect/effMath", func_00195D00);

INCLUDE_ASM(const s32, "effect/effMath", func_00195E10);

s32 func_00195E38(EffMathSlotList *list, s32 index) {
    return (s32)&list->slots[index];
}
