#ifndef DDS3_PATH_H
#define DDS3_PATH_H

#include "common.h"

struct EffPrimitiveCurve;

enum Dds3PathChannelFlags {
    DDS3_PATH_POSITION_CHANNEL = 1,
    DDS3_PATH_ROTATION_CHANNEL = 2,
    DDS3_PATH_SCALAR_CHANNEL = 4,
    DDS3_PATH_WORLD_TRANSFORM_CHANNEL = 0x10
};

/* A channel descriptor stores payload samples and their frame numbers. */
typedef struct Dds3PathKeyframes {
    u32 count;
    void *data; /* XYZ, quaternion, scalar, or composite samples by channel kind. */
    u32 *frames;
} Dds3PathKeyframes;

/* Complete 0x24-byte state allocated by dds3CreatePathCurveWork. */
typedef struct Dds3PathCurveWork {
    s32 direction; /* 0 counts up, 1 counts down; other values do not step. */
    u32 flags;
    f32 duration; /* Greatest final frame across the retained channels. */
    f32 time;
    struct EffPrimitiveCurve *primitiveCurve; /* 0x10: derived from position samples. */
    Dds3PathKeyframes *positionKeys; /* 0x14 */
    Dds3PathKeyframes *rotationKeys; /* 0x18 */
    Dds3PathKeyframes *scalarKeys; /* 0x1C */
    Dds3PathKeyframes *transformKeys; /* 0x20: ten-float world-transform samples. */
} Dds3PathCurveWork;

typedef char Dds3PathKeyframes_size_must_be_0x0C[
    (sizeof(Dds3PathKeyframes) == 0x0C) ? 1 : -1];
typedef char Dds3PathCurveWork_size_must_be_0x24[
    (sizeof(Dds3PathCurveWork) == 0x24) ? 1 : -1];

struct EffWorldNode;
struct ObjectTransform;
struct WorldTransformParams;

typedef s32 (*Dds3MoverUpdate)(struct ObjectTransform *, struct EffWorldNode *);

/* Kind 3 allocates 0x10 bytes in dds3AllocateClearedObjectWork. */
typedef struct Dds3SlotResource {
    struct EffWorldNode *target; /* 0x00: object whose transform is updated. */
    Dds3PathCurveWork *path; /* 0x04: retained curve work. */
    Dds3MoverUpdate update; /* 0x08: used when no curve work is retained. */
    struct EffWorldNode *sourceObject; /* 0x0C: kind-16 curve source. */
} Dds3SlotResource;

typedef char Dds3SlotResource_size_must_be_0x10[
    (sizeof(Dds3SlotResource) == 0x10) ? 1 : -1];

void dds3SamplePathKeyframeInterval(u32 *segment, f32 *weight, Dds3PathKeyframes *keys, f32 frame);
Dds3PathCurveWork *dds3CreatePathCurveWork(struct EffWorldNode *object);
void dds3FreePathObject(Dds3PathCurveWork *path);
Dds3PathCurveWork *dds3GetObjectResourceHandle(struct EffWorldNode *object);
void dds3InterpolatePathVectorVU(Dds3PathCurveWork *path);
void dds3InterpolatePathQuaternionVU(Dds3PathCurveWork *path);
void dds3InterpolatePathOutput(Dds3PathCurveWork *path, struct WorldTransformParams *out);
f32 sdfSampleActiveLinearCurve(Dds3PathCurveWork *path);
s32 sdfStepWrappingFloatCounter(Dds3PathCurveWork *path);
f32 evtMeasurePathTrajectoryLength(Dds3PathCurveWork *path);

#endif
