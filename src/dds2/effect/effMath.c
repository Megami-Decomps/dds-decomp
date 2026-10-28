#include "common.h"
#include "sdf_resource.h"
#include "eff.h"
#include "eff_math.h"
#include "eff_cubic_bezier_array.h"
extern f32 D_00433528;


void dds3Bezier4PDelete(EffCubicBezierArray *work) {
    sdfMemFreeDirect(work->allocation);
}


/* Evaluate the curve at the current t, advance t, and report whether the
 * stored next position was clamped to the endpoint. */
EffCubicBezierStepResult dds3Bezier4PPartRun(
    EffCubicBezierArray *table, EffCubicBezierSlotIndex slotIndex, f32 *out) {
    EffCubicBezierSlot *slot = &table->slots[slotIndex];
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
        return EFF_CUBIC_BEZIER_STEP_CLAMPED_TO_END;
    }
    slot->t = t;
    return EFF_CUBIC_BEZIER_STEP_WITHIN_RANGE;
}

/* Direct-slot variant of dds3Bezier4PPartRun. */
EffCubicBezierStepResult nbCameraLerpVec(EffCubicBezierSlot *slot, f32 *out) {
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
        return EFF_CUBIC_BEZIER_STEP_CLAMPED_TO_END;
    }
    slot->t = t;
    return EFF_CUBIC_BEZIER_STEP_WITHIN_RANGE;
}

void effMathResetBezierSlot(EffCubicBezierArray *table, EffCubicBezierSlotIndex slotIndex) {
    EffCubicBezierSlot *slot = &table->slots[slotIndex];
    slot->step = 0.05f;
    slot->t = 0;
}

EffCubicBezierSlot *dds3Bezier4PGetPart(
    EffCubicBezierArray *table, EffCubicBezierSlotIndex slotIndex) {
    return &table->slots[slotIndex];
}
