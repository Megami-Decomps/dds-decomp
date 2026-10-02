#include "common.h"

typedef struct {
    u8 pad00[8];
    u32 resourceHandle; /* 0x08 */
} EffMathWork;

void effMathReleaseWorkResource(EffMathWork *work) {
    sdfReleaseResourceAllocation(work->resourceHandle);
}

typedef struct EffMathSlot {
    u8 unk0[0x30];
    s32 unk30; /* 0x30 */
    f32 unk34; /* 0x34 */
} EffMathSlot; /* 0x38 */

typedef struct EffMathSlots {
    EffMathSlot *slots;
} EffMathSlots;

/* Cubic Bezier control points (four xyz points) followed by the parameter t and its per-step increment. */
typedef struct EffBezierSlot {
    f32 point[4][3];
    f32 t;    /* 0x30 */
    f32 step; /* 0x34 */
} EffBezierSlot; /* 0x38 */

/* Evaluate the cubic Bezier at t into out[3], advance t, and report whether it is still below 1. */
s32 effMathStepBezierSlot(EffMathSlots *table, s32 index, f32 *out) {
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

/* Direct-slot variant of effMathStepBezierSlot. */
s32 func_0018E0C8(EffBezierSlot *slot, f32 *out) {
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
    out[3] = 1.0f;
    t += slot->step;
    if (t > 1.0f) {
        slot->t = 1.0f;
        return 0;
    }
    slot->t = t;
    return 1;
}

void effMathResetBezierSlot(EffMathSlots *table, s32 index) {
    EffMathSlot *slot = &table->slots[index];

    slot->unk34 = 0.05f;
    slot->unk30 = 0;
}

s32 effMathGetSlotAt(s32 *base, s32 index) {
    return *base + index * 0x38;
}
