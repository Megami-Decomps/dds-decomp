#ifndef EFF_MATH_H
#define EFF_MATH_H

#include "common.h"

struct EffCubicBezierArray;

/* One cubic segment: four XYZ controls, evaluation position and frame step.
 * The allocation header follows the slots; it is a distinct pointer. */
typedef struct EffCubicBezierSlot {
    f32 controlPoints[4][3];
    f32 t;
    f32 step;
} EffCubicBezierSlot;

typedef char EffCubicBezierSlotLayoutAssert[
    (sizeof(EffCubicBezierSlot) == 0x38 &&
     (u32)&((EffCubicBezierSlot *)0)->t == 0x30 &&
     (u32)&((EffCubicBezierSlot *)0)->step == 0x34) ? 1 : -1];

typedef s32 EffCubicBezierSlotIndex;

typedef s32 EffCubicBezierStepResult;
enum {
    EFF_CUBIC_BEZIER_STEP_CLAMPED_TO_END = 0,
    EFF_CUBIC_BEZIER_STEP_WITHIN_RANGE = 1
};

struct EffCubicBezierArray *dds3Bezier4PCreate(s32 count);
void dds3Bezier4PDelete(struct EffCubicBezierArray *work);
EffCubicBezierStepResult dds3Bezier4PPartRun(
    struct EffCubicBezierArray *table, EffCubicBezierSlotIndex slotIndex, f32 *out);
EffCubicBezierStepResult nbCameraLerpVec(EffCubicBezierSlot *slot, f32 *out);
void effMathResetBezierSlot(
    struct EffCubicBezierArray *table, EffCubicBezierSlotIndex slotIndex);
EffCubicBezierSlot *dds3Bezier4PGetPart(
    struct EffCubicBezierArray *table, EffCubicBezierSlotIndex slotIndex);
/* Blend packed RGBA8888 colors; t >= 1 returns colorB unchanged. */
u32 effColorBlend(u32 colorA, u32 colorB, f32 t);

#endif
