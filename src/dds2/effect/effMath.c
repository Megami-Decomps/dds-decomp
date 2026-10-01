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

/* Cubic Bezier control points (four xyz points) followed by the parameter t and its per-step increment. */
typedef struct EffBezierSlot {
    f32 point[4][3];
    f32 t;    /* 0x30 */
    f32 step; /* 0x34 */
} EffBezierSlot; /* 0x38 */

/* Evaluate the cubic Bezier at t into out[3], advance t, and report whether it is still below 1. */
s32 func_00195BE0(EffMathSlotList *table, s32 index, f32 *out) {
    EffBezierSlot *slot = (EffBezierSlot *)&table->slots[index];
    f32 w[4];
    f32 t = slot->t;
    f32 u = 1.0f - t;

    w[0] = u * u * u;
    w[1] = t * (u * u) * 3.0f;
    w[2] = t * t * u * 3.0f;
    w[3] = t * t * t;
    out[0] = slot->point[0][0] * w[0] + slot->point[1][0] * w[1] + slot->point[2][0] * w[2] + slot->point[3][0] * w[3];
    out[1] = slot->point[0][1] * w[0] + slot->point[1][1] * w[1] + slot->point[2][1] * w[2] + slot->point[3][1] * w[3];
    out[2] = slot->point[0][2] * w[0] + slot->point[1][2] * w[1] + slot->point[2][2] * w[2] + slot->point[3][2] * w[3];
    t += slot->step;
    if (t > 1.0f) {
        slot->t = 1.0f;
        return 0;
    }
    slot->t = t;
    return 1;
}

INCLUDE_ASM(const s32, "effect/effMath", func_00195D00);

void func_00195E10(EffMathSlotList *list, s32 index) {
    EffMathSlot *slot = &list->slots[index];
    slot->unk34 = 0.05f;
    slot->unk30 = 0;
}

s32 effMathGetSlotAt(EffMathSlotList *list, s32 index) {
    return (s32)&list->slots[index];
}
