#ifndef EFF_MATH_H
#define EFF_MATH_H

#include "common.h"

struct EffArrHdr;

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

struct EffArrHdr *effAllocSlotArray(s32 count);
void effMathReleaseWorkResource(struct EffArrHdr *work);
s32 effMathStepBezierSlot(struct EffArrHdr *table, s32 index, f32 *out);
s32 effMathStepBezierSlotDirect(EffCubicBezierSlot *slot, f32 *out);
void effMathResetBezierSlot(struct EffArrHdr *table, s32 index);
EffCubicBezierSlot *effMathGetSlotAt(struct EffArrHdr *table, s32 index);

#endif
