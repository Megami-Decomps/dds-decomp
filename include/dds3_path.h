#ifndef DDS3_PATH_H
#define DDS3_PATH_H

#include "common.h"

struct EffPrim;

/* A channel descriptor stores payload samples and their frame numbers. */
typedef struct Dds3PathKeyframes {
    u32 count;
    void *data; /* XYZ, quaternion, scalar, or composite samples by channel kind. */
    u32 *frames;
} Dds3PathKeyframes;

/* Complete 0x24-byte state allocated by dds3CreatePathCurveWork. */
typedef struct Dds3PathCurveWork {
    s32 state;
    u32 flags;
    f32 duration; /* Greatest final frame across the retained channels. */
    f32 time;
    struct EffPrim *primitiveCurve; /* 0x10: derived from position samples. */
    Dds3PathKeyframes *positionKeys; /* 0x14 */
    Dds3PathKeyframes *rotationKeys; /* 0x18 */
    Dds3PathKeyframes *scalarKeys; /* 0x1C */
    Dds3PathKeyframes *transformKeys; /* 0x20: ten-float world-transform samples. */
} Dds3PathCurveWork;

typedef char Dds3PathKeyframes_size_must_be_0x0C[
    (sizeof(Dds3PathKeyframes) == 0x0C) ? 1 : -1];
typedef char Dds3PathCurveWork_size_must_be_0x24[
    (sizeof(Dds3PathCurveWork) == 0x24) ? 1 : -1];

#endif
