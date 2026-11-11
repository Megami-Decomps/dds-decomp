#include "common.h"
#include "sdfFrame.h"
#include "kernel.h"
#include "ps2asm.h"
#include "model.h"
#include "motion.h"

/* DDS's names for the same functions. */

/* Cubic spline fitted over a point list; dds3AllocSplineHandle takes 0x2C. */
typedef struct dds3Spline_t {
    void *handle;
    void *coefHandle;
    s32 count;
    s16 single;
    s16 pad0E;
    void *points;
    f32 *coefs;
    f32 *coefB;
    f32 *coefC;
    s32 unk20;
    f32 unk24;
    f32 unk28;
} dds3Spline_t;

/* Walks a point list at a fixed step; effCreateChannel takes 0x18. */
typedef struct dds3SplineWalk_t {
    void *handle;
    s32 count;
    f32 *points;
    s32 index;
    f32 t;
    f32 step;
} dds3SplineWalk_t;

/* 128-byte party records; this unit only reads the formation index. */


extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

extern void *sdfMemLock(sdfMemHandle_t *aHandle);

void dds3AllocSplinePoints(dds3Spline_t *spline, s32 flattenEqualComponents);

void dds3BuildSplinePoints(f32 *tangents, f32 *points, u32 count);

void dds3BuildSplineCoefs(dds3Spline_t *spline, f32 *tangents);

void SolveTridiagonal(f32 *solution, f32 *rhs, s32 count);

void dds3BuildSplineCoefsSkipDup(dds3Spline_t *spline, f32 *tangents);

void effSampleChannelBezier(sdfVector4_t *vertex, dds3SplineWalk_t *channel, s32 index, f32 t);

void effSplCalc(sdfVector4_t *vertex, dds3Spline_t *primitive, s32 recordIndex, f32 t);

dds3Spline_t *dds3AllocSplineHandle(f32 *points, u32 count, s32 flattenEqualComponents) {
    sdfMemHandle_t *primaryResource = sdfMemAllocDirect(0x2C);
    f32 step = 0.05f;
    dds3Spline_t *primitive = (dds3Spline_t *)sdfMemLock(primaryResource);
    sdfMemHandle_t *coefficientResource;
    f32 (*coefficients)[3];

    primitive->handle = primaryResource;
    primitive->count = count;
    primitive->points = points;
    primitive->unk20 = 0;
    primitive->unk24 = 0;
    primitive->unk28 = step;
    if (count < 3) {
        primitive->coefs = NULL;
        primitive->single = 1;
    } else {
        coefficientResource = sdfMemAllocDirect(count * 36);
        coefficients = (f32 (*)[3])sdfMemLock(coefficientResource);
        primitive->handle = primaryResource;
        primitive->coefHandle = coefficientResource;
        primitive->single = 0;
        primitive->count = count;
        primitive->points = points;
        primitive->coefs = (f32 *)coefficients;
        primitive->coefB = (f32 *)(coefficients + count);
        primitive->coefC = (f32 *)(coefficients + count * 2);
        primitive->unk20 = 0;
        primitive->unk24 = 0;
        primitive->unk28 = step;
        dds3AllocSplinePoints(primitive, flattenEqualComponents);
    }
    return primitive;
}

void dds3FreeSplineHandle(dds3Spline_t *pSpline)
{
    if (pSpline == NULL) {
        return;
    }
    if (pSpline->coefs != NULL) {
        sdfMemFreeDirect(pSpline->coefHandle);
    }
    sdfMemFreeDirect(pSpline->handle);
}

s32 effSplRun(sdfVector4_t *pVertex, dds3Spline_t *pSpline)
{
    f32 t = pSpline->unk24;
    s32 index = pSpline->unk20;
    s32 running = 1;

    effSplCalc(pVertex, pSpline, index, t);
    t += SDF_FRAME_STEP(pSpline->unk28);
    if (t > 1.0f) {
        t -= 1.0f;
        index++;
    }
    if ((u32)index >= (u32)(pSpline->count - 1)) {
        t = 0.0f;
        index = 0;
        running = 0;
    }
    pSpline->unk20 = index;
    pSpline->unk24 = t;
    return running;
}

void effSplCalc(sdfVector4_t *vertex, dds3Spline_t *primitive, s32 recordIndex, f32 t) {
    f32 *a;
    f32 *b;
    f32 *c;
    f32 *d;

    if ((u16)primitive->single == 0) {
        a = primitive->coefs + recordIndex * 3;
        b = primitive->coefB + recordIndex * 3;
        c = primitive->coefC + recordIndex * 3;
        d = (f32 *)primitive->points + recordIndex * 3;
        vertex->x = ((a[0] * t + b[0]) * t + c[0]) * t + d[0];
        vertex->y = ((a[1] * t + b[1]) * t + c[1]) * t + d[1];
        vertex->z = ((a[2] * t + b[2]) * t + c[2]) * t + d[2];
        vertex->w = 1.0f;
    } else {
        a = (f32 *)primitive->points + recordIndex * 3;
        b = a + 3;
        vertex->x = a[0] + (b[0] - a[0]) * t;
        vertex->y = a[1] + (b[1] - a[1]) * t;
        vertex->z = a[2] + (b[2] - a[2]) * t;
    }
}

/* vu0 routine: sample the curve into vf10. */
void effSplCalcRegist(dds3Spline_t *primitive, s32 recordIndex, f32 t)
{
    sdfVector4_t vertex;
    f32 *a;
    f32 *b;
    f32 *c;
    f32 *d;

    if ((u16)primitive->single == 0) {
        a = primitive->coefs + recordIndex * 3;
        b = primitive->coefB + recordIndex * 3;
        c = primitive->coefC + recordIndex * 3;
        d = (f32 *)primitive->points + recordIndex * 3;
        vertex.x = ((a[0] * t + b[0]) * t + c[0]) * t + d[0];
        vertex.y = ((a[1] * t + b[1]) * t + c[1]) * t + d[1];
        vertex.z = ((a[2] * t + b[2]) * t + c[2]) * t + d[2];
        vertex.w = 1.0f;
    } else {
        a = (f32 *)primitive->points + recordIndex * 3;
        b = a + 3;
        vertex.x = a[0] + (b[0] - a[0]) * t;
        vertex.y = a[1] + (b[1] - a[1]) * t;
        vertex.z = a[2] + (b[2] - a[2]) * t;
    }
    SDF_VU0_LOADVECTOR_MEMOPS(*(u128 *)&vertex);
}

void effSplRewind(dds3Spline_t *pSpline)
{
    pSpline->unk20 = 0;
    pSpline->unk24 = 0;
}

void effSplSetStep(dds3Spline_t *pSpline, f32 value)
{
    pSpline->unk28 = value;
}

void dds3AllocSplinePoints(dds3Spline_t *primitive, s32 flattenEqualComponents) {
    sdfMemHandle_t *handle = sdfMemAllocDirect(primitive->count * 12);
    f32 *tangentData = (f32 *)sdfMemLock(handle);

    dds3BuildSplinePoints(tangentData, primitive->points, primitive->count);
    if (flattenEqualComponents == 0) {
        dds3BuildSplineCoefs(primitive, tangentData);
    } else {
        dds3BuildSplineCoefsSkipDup(primitive, tangentData);
    }
    sdfMemFreeDirect(handle);
}

void dds3BuildSplinePoints(f32 *tangents, f32 *points, u32 count) {
    sdfMemHandle_t *handle = sdfMemAllocDirect(count * sizeof(f32));
    f32 *rhs = (f32 *)sdfMemLock(handle);
    f32 (*pts)[3] = (f32 (*)[3])points;
    s32 component;
    s32 i;

    for (component = 0; component < 3; component++) {
        rhs[0] = (pts[1][component] - pts[0][component]) * 3.0f;
        rhs[count - 1] = (pts[count - 1][component] - pts[count - 2][component]) * 3.0f;
        for (i = 1; i <= (s32)count - 2; i++) {
            rhs[i] = ((pts[i][component] - pts[i - 1][component]) +
                      (pts[i + 1][component] - pts[i][component])) * 3.0f;
        }
        SolveTridiagonal(tangents, rhs, count);
        tangents += count;
    }
    sdfMemFreeDirect(handle);
}

void SolveTridiagonal(f32 *solution, f32 *rhs, s32 count) {
    s32 byteCount = count * sizeof(f32);
    void *handle = sdfMemAllocDirect(byteCount);
    f32 *upper;
    s32 i;
    f32 denominator;
    f32 *upperEnd;
    f32 *solutionEnd;
    f32 *rhsEnd;

    count -= 2;
    upper = (f32 *)sdfMemLock(handle);
    denominator = 2.0f;

    solution[0] = rhs[0] / denominator;
    for (i = 1; i <= count; i++) {
        upper[i] = 1.0f / denominator;
        denominator = 4.0f - upper[i];
        solution[i] = (rhs[i] - solution[i - 1]) / denominator;
    }
    upperEnd = (f32 *)((u8 *)upper + byteCount);
    solutionEnd = (f32 *)((u8 *)solution + byteCount);
    rhsEnd = (f32 *)((u8 *)rhs + byteCount);
    upperEnd[-1] = 1.0f / denominator;
    denominator = 2.0f - upperEnd[-1];
    solutionEnd[-1] = (rhsEnd[-1] - solutionEnd[-2]) / denominator;
    for (i = count; i >= 0; i--) {
        solution[i] -= upper[i + 1] * solution[i + 1];
    }
    sdfMemFreeDirect(handle);
}

void dds3BuildSplineCoefs(dds3Spline_t *primitive, f32 *tangents) {
    f32 *cubic = primitive->coefs;
    f32 *quadratic = primitive->coefB;
    f32 *linear = primitive->coefC;
    f32 *points = primitive->points;
    s32 count = primitive->count;
    s32 segment;
    for (segment = 0; segment < count - 1; segment++) {
        s32 component;
        for (component = 0; component < 3; component++) {
            f32 endValue = points[segment * 3 + component + 3];
            f32 startValue = points[segment * 3 + component];
            f32 startTangent = tangents[component * count + segment];
            f32 endTangent = tangents[component * count + segment + 1];
            cubic[segment * 3 + component] = 2.0f * (startValue - endValue) +
                (startTangent + endTangent);
            quadratic[segment * 3 + component] = (endValue - startValue) * 3.0f -
                (2.0f * startTangent + endTangent);
            linear[segment * 3 + component] = startTangent;
        }
    }
}

void dds3BuildSplineCoefsSkipDup(dds3Spline_t *primitive, f32 *tangents) {
    f32 *cubic = primitive->coefs;
    f32 *quadratic = primitive->coefB;
    f32 *linear = primitive->coefC;
    f32 *points = primitive->points;
    s32 count = primitive->count;
    s32 segment;
    s32 component;

    for (segment = 0; segment < count - 1; segment++) {
        for (component = 0; component < 3; component++) {
            f32 startValue = points[segment * 3 + component];
            f32 endValue = points[segment * 3 + component + 3];

            if (startValue == endValue) {
                cubic[segment * 3 + component] = 0.0f;
                quadratic[segment * 3 + component] = 0.0f;
                linear[segment * 3 + component] = 0.0f;
            } else {
                f32 startTangent = tangents[component * count + segment];
                f32 endTangent = tangents[component * count + segment + 1];

                cubic[segment * 3 + component] = 2.0f * (startValue - endValue) + (startTangent + endTangent);
                quadratic[segment * 3 + component] = (endValue - startValue) * 3.0f - (2.0f * startTangent + endTangent);
                linear[segment * 3 + component] = startTangent;
            }
        }
    }
}

void *effCreateChannel(void *rows, u32 count) {
    void *channel = NULL;
    sdfMemHandle_t *handle;
    dds3SplineWalk_t *channelObject;

    if (count < 4) {
        return channel;
    }
    handle = sdfMemAllocDirect(0x18);
    channel = (void *)sdfMemLock(handle);
    channelObject = channel;
    channelObject->handle = handle;
    channelObject->step = 0.05f;
    channelObject->count = count;
    channelObject->points = rows;
    channelObject->t = 0;
    channelObject->index = 0;
    return channel;
}

s32 effReleaseInterpolationChannel(dds3SplineWalk_t *pWalk)
{
    if (pWalk != NULL) {
        sdfMemFreeDirect(pWalk->handle);
    }
}

s32 effAdvanceChanCursor(void *vertex, dds3SplineWalk_t *channel) {
    s32 continuing = 1;
    f32 position = channel->t;
    u32 recordIndex = channel->index;

    effSampleChannelBezier(vertex, channel, recordIndex, position);
    position += SDF_FRAME_STEP(channel->step);
    if (position > 1.0f) {
        position -= 1.0f;
        recordIndex += 3;
    }
    if (recordIndex >= channel->count - 1) {
        position = 0.0f;
        recordIndex = 0;
        continuing = 0;
    }
    channel->index = recordIndex;
    channel->t = position;
    return continuing;
}

void effSampleChannelBezier(sdfVector4_t *vertex, dds3SplineWalk_t *channel, s32 recordIndex, f32 t) {
    sdfVector4_t weights;
    f32 oneMinusT = 1.0f - t;
    f32 *p0 = channel->points + recordIndex * 3;
    f32 *p1 = p0 + 3;
    f32 *p2 = p0 + 6;
    f32 *p3 = p0 + 9;

    weights.x = oneMinusT * oneMinusT * oneMinusT;
    weights.y = t * (oneMinusT * oneMinusT) * 3.0f;
    weights.z = t * t * oneMinusT * 3.0f;
    weights.w = t * t * t;
    vertex->x = p0[0] * weights.x + p1[0] * weights.y + p2[0] * weights.z + p3[0] * weights.w;
    vertex->y = p0[1] * weights.x + p1[1] * weights.y + p2[1] * weights.z + p3[1] * weights.w;
    vertex->z = p0[2] * weights.x + p1[2] * weights.y + p2[2] * weights.z + p3[2] * weights.w;
    vertex->w = 1.0f;
}

void effClearChanCursor(dds3SplineWalk_t *pWalk)
{
    pWalk->index = 0;
    pWalk->t = 0.0f;
}

void effSetChanStep(dds3SplineWalk_t *pWalk, f32 step)
{
    pWalk->step = step;
}
/* SMT3's sdata in this span is the battle camera's; DDS has none here. */
