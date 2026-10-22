#ifndef DDS3_PATH_H
#define DDS3_PATH_H

#include "common.h"
#include "sdf_float_counter.h"

struct EffPrimitiveCurve;

/* Observed serialized path-curve kinds, distinct from runtime flags. */
enum Dds3PathCurveKind {
    DDS3_PATH_CURVE_POSITION = 0,
    DDS3_PATH_CURVE_ROTATION = 2,
    DDS3_PATH_CURVE_SCALAR = 4,
    DDS3_PATH_CURVE_WORLD_TRANSFORM = 5
};

enum Dds3PathChannelFlags {
    DDS3_PATH_POSITION_CHANNEL = 1,
    DDS3_PATH_ROTATION_CHANNEL = 2,
    DDS3_PATH_SCALAR_CHANNEL = 4,
    DDS3_PATH_WORLD_TRANSFORM_CHANNEL = 0x10
};

typedef f32 Dds3PathPositionSample[3];
typedef f32 Dds3PathRotationSample[4];

struct WorldTransformParams;

/* Each channel's sample stride is selected by its enclosing curve kind. */
typedef union Dds3PathSampleData {
    Dds3PathPositionSample *position;
    Dds3PathRotationSample *rotation;
    f32 *scalar;
    struct WorldTransformParams *worldTransform;
} Dds3PathSampleData;

/* A channel descriptor stores kind-specific samples and their frame numbers. */
typedef struct Dds3PathKeyframes {
    u32 count;
    Dds3PathSampleData data; /* Selected by the enclosing Dds3PathCurveEntry.kind. */
    u32 *frames;
} Dds3PathKeyframes;

/* The path object's variable-length curve table starts with this count. */
typedef struct Dds3PathCurveEntry {
    enum Dds3PathCurveKind kind;
    Dds3PathKeyframes *keys;
} Dds3PathCurveEntry;

typedef struct Dds3PathCurveTable {
    u32 count;
    Dds3PathCurveEntry entries[1];
} Dds3PathCurveTable;

/* Complete 0x24-byte state allocated by dds3AllocPathBasicWorkData. */
typedef struct dds3PathBasicControl_t {
    SdfFloatCounter counter; /* Duration is the greatest final channel frame. */
    struct EffPrimitiveCurve *primitiveCurve; /* 0x10: derived from position samples. */
    Dds3PathKeyframes *positionKeys; /* 0x14 */
    Dds3PathKeyframes *rotationKeys; /* 0x18 */
    Dds3PathKeyframes *scalarKeys; /* 0x1C */
    Dds3PathKeyframes *transformKeys; /* 0x20: ten-float world-transform samples. */
} dds3PathBasicControl_t;

typedef char Dds3PathKeyframes_size_must_be_0x0C[
    (sizeof(Dds3PathKeyframes) == 0x0C) ? 1 : -1];
typedef char Dds3PathSampleData_size_must_be_4[
    (sizeof(Dds3PathSampleData) == 0x04) ? 1 : -1];
typedef char Dds3PathKeyframes_data_at_0x04[
    ((u32)&((Dds3PathKeyframes *)0)->data == 0x04) ? 1 : -1];
typedef char Dds3PathCurveEntry_size_must_be_8[
    (sizeof(Dds3PathCurveEntry) == 0x08) ? 1 : -1];
typedef char Dds3PathCurveKind_size_must_be_4[
    (sizeof(enum Dds3PathCurveKind) == 4) ? 1 : -1];
typedef char Dds3PathCurveEntry_kind_at_0[
    ((u32)&((Dds3PathCurveEntry *)0)->kind == 0) ? 1 : -1];
typedef char Dds3PathCurveEntry_keys_at_4[
    ((u32)&((Dds3PathCurveEntry *)0)->keys == 0x04) ? 1 : -1];
typedef char Dds3PathCurveTable_entries_at_4[
    ((u32)&((Dds3PathCurveTable *)0)->entries == 0x04) ? 1 : -1];
typedef char Dds3PathCurveWork_size_must_be_0x24[
    (sizeof(dds3PathBasicControl_t) == 0x24) ? 1 : -1];

struct dds3Basic_t;
struct dds3PoseSet_t;
struct WorldTransformParams;

typedef s32 (*Dds3MoverUpdate)(struct dds3PoseSet_t *, struct dds3Basic_t *);

/* Kind 3 allocates 0x10 bytes in MoverBasicConstructor. */
typedef struct Dds3SlotResource {
    struct dds3Basic_t *target; /* 0x00: object whose transform is updated. */
    dds3PathBasicControl_t *path; /* 0x04: retained curve work. */
    Dds3MoverUpdate update; /* 0x08: used when no curve work is retained. */
    struct dds3Basic_t *sourceObject; /* 0x0C: kind-16 curve source. */
} Dds3SlotResource;

typedef char Dds3SlotResource_size_must_be_0x10[
    (sizeof(Dds3SlotResource) == 0x10) ? 1 : -1];

void dds3PathEvalSpline(u32 *segment, f32 *weight, const Dds3PathKeyframes *keys, f32 frame);
dds3PathBasicControl_t *dds3AllocPathBasicWorkData(struct dds3Basic_t *object);
dds3PathBasicControl_t *dds3ObjectBaseGetPathControl(struct dds3Basic_t *object);
void dds3DeletePathControl(dds3PathBasicControl_t *path);
dds3PathBasicControl_t *dds3MoverBasicGetPathControl(struct dds3Basic_t *object);
void dds3PathBasicControlGetPos(dds3PathBasicControl_t *path);
void dds3PathBasicControlGetRot(dds3PathBasicControl_t *path);
void dds3PathBasicControlNextStep(dds3PathBasicControl_t *path, struct WorldTransformParams *out);
f32 dds3PathBasicControlGetFovy(dds3PathBasicControl_t *path);
f32 GetSplineLength(dds3PathBasicControl_t *path);

#endif
