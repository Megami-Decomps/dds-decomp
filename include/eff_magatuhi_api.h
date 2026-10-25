#ifndef EFF_MAGATUHI_API_H
#define EFF_MAGATUHI_API_H

#include "eff.h"
#include "eff_node.h"

typedef struct EffMagatuhiPosition {
    f32 x;
    f32 y;
    f32 z;
    u8 opaqueFourthWord[4];
} EffMagatuhiPosition;

/* Ten-word input copied into the first Magatuhi family's parameter head. */
typedef struct EffMagatuhiFloatParams {
    EffMagatuhiPosition origin;
    EffMagatuhiPosition target;
    f32 initialRadius;
    f32 captureDistance;
} EffMagatuhiFloatParams;

/* Four 4-word control points followed by their four jitter scales. */
typedef struct EffMagatuhiRowsSrc {
    f32 controlPoints[4][4];
    f32 jitterScales[4];
} EffMagatuhiRowsSrc;

typedef char EffMagatuhiPositionSizeCheck[(sizeof(EffMagatuhiPosition) == 0x10) ? 1 : -1];
typedef char EffMagatuhiFloatParamsSizeCheck[(sizeof(EffMagatuhiFloatParams) == 0x28) ? 1 : -1];
typedef char EffMagatuhiFloatParamsTargetOffsetCheck[
    ((u32)&((EffMagatuhiFloatParams *)0)->target == 0x10) ? 1 : -1];
typedef char EffMagatuhiRowsSrcSizeCheck[(sizeof(EffMagatuhiRowsSrc) == 0x50) ? 1 : -1];

u32 dds3MagatuhiGetType(EffWork *work);
void effMagaSCSetArea(EffNode *node, const EffMagatuhiFloatParams *src);
void effMagaP4SetPoint(EffNode *node, const EffMagatuhiRowsSrc *src);

#endif /* EFF_MAGATUHI_API_H */
