#include "common.h"
#include "sdf_resource.h"
#include "eff.h"
#include "eff_math.h"
extern f32 D_00433528;


void effMathReleaseWorkResource(EffArrHdr *work) {
    sdfMemFreeDirect(work->allocation);
}


/* Evaluate the cubic Bezier at t into out[3], advance t, and report whether it is still below 1. */
s32 effMathStepBezierSlot(EffArrHdr *table, s32 slotIndex, f32 *out) {
    EffCubicBezierSlot *slot = &((EffCubicBezierSlot *)table->slots)[slotIndex];
    f32 w[4];
    f32 t = slot->t;
    f32 u = 1.0f - t;

    w[0] = u * u * u;
    w[1] = t * (u * u) * 3.0f;
    w[2] = t * t * u * 3.0f;
    w[3] = t * t * t;
    out[0] = slot->controlPoints[0][0] * w[0] + slot->controlPoints[1][0] * w[1] + slot->controlPoints[2][0] * w[2] + slot->controlPoints[3][0] * w[3];
    out[1] = slot->controlPoints[0][1] * w[0] + slot->controlPoints[1][1] * w[1] + slot->controlPoints[2][1] * w[2] + slot->controlPoints[3][1] * w[3];
    out[2] = slot->controlPoints[0][2] * w[0] + slot->controlPoints[1][2] * w[1] + slot->controlPoints[2][2] * w[2] + slot->controlPoints[3][2] * w[3];
    t += slot->step;
    if (t > 1.0f) {
        slot->t = 1.0f;
        return 0;
    }
    slot->t = t;
    return 1;
}

/* Direct-slot variant of effMathStepBezierSlot. */
s32 effMathStepBezierSlotDirect(EffCubicBezierSlot *slot, f32 *out) {
    f32 w[4];
    f32 t = slot->t;
    f32 u = 1.0f - t;

    w[0] = u * u * u;
    w[1] = t * (u * u) * 3.0f;
    w[2] = t * t * u * 3.0f;
    w[3] = t * t * t;
    out[0] = slot->controlPoints[0][0] * w[0] + slot->controlPoints[1][0] * w[1] + slot->controlPoints[2][0] * w[2] + slot->controlPoints[3][0] * w[3];
    out[1] = slot->controlPoints[0][1] * w[0] + slot->controlPoints[1][1] * w[1] + slot->controlPoints[2][1] * w[2] + slot->controlPoints[3][1] * w[3];
    out[2] = slot->controlPoints[0][2] * w[0] + slot->controlPoints[1][2] * w[1] + slot->controlPoints[2][2] * w[2] + slot->controlPoints[3][2] * w[3];
    out[3] = 1.0f;
    t += slot->step;
    if (t > 1.0f) {
        slot->t = 1.0f;
        return 0;
    }
    slot->t = t;
    return 1;
}

void effMathResetBezierSlot(EffArrHdr *table, s32 slotIndex) {
    EffCubicBezierSlot *slot = &((EffCubicBezierSlot *)table->slots)[slotIndex];
    slot->step = 0.05f;
    slot->t = 0;
}

EffCubicBezierSlot *effMathGetSlotAt(EffArrHdr *table, s32 slotIndex) {
    return &((EffCubicBezierSlot *)table->slots)[slotIndex];
}
