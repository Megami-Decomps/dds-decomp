#include "common.h"
#include "sdf_vu0.h"
#include "eff_random.h"
#include "eff_param.h"
#include "sdf_heap_api.h"
#include "eff.h"
#include "eff_math.h"
#include "eff_channel.h"
#include "eff_channel_layout.h"
#include "pcp_vu0.h"
#include "fr_font.h"
#include "font_mem_control_api.h"

#define EFF_CURVE_COMPONENT_COUNT 3

#define EFF_CURVE_POINT_BYTES 12

#define EFF_BEZIER_MIN_RECORD_COUNT 4

#define EFF_BEZIER_RECORD_ADVANCE 3

#define EFF_CHANNEL_BYTES 0x18

#define EFF_CHANNEL_DEFAULT_STEP 0.05f

#define EFF_PRIMITIVE_CUBIC_MODE 0

extern void effSplCalc(EffVert *vertex, EffPrimitiveCurve *primitive, s32 recordIndex, f32 t);

extern void effSampleChannelBezier(EffVert *vertex, EffChan *channel, s32 recordIndex, f32 t);

/* List header defined in game/code_00193C08 (unsized: keeps absolute access). */
extern void dds3BuildSplinePoints(f32 *tangents, f32 *keys, u32 recordCount);

extern void SolveTridiagonal(f32 *solution, f32 *rhs, s32 count);

extern void dds3BuildSplineCoefs(EffPrimitiveCurve *primitive, f32 *tangents);

extern void dds3BuildSplineCoefsSkipDup(EffPrimitiveCurve *primitive, f32 *tangents);

extern void dds3AllocSplinePoints(EffPrimitiveCurve *primitive, s32 flattenEqualComponents);

EffPrimitiveCurve *dds3AllocSplineHandle(f32 *keys, u32 recordCount, s32 flattenEqualComponents) {
    sdfMemHandle_t *primaryResource = sdfMemAllocDirect(0x2C);
    f32 step = EFF_CHANNEL_DEFAULT_STEP;
    EffPrimitiveCurve *primitive = (EffPrimitiveCurve *)sdfMemLock(primaryResource);
    sdfMemHandle_t *coefficientResource;
    f32 (*coefficients)[EFF_CURVE_COMPONENT_COUNT];

    primitive->allocation = primaryResource;
    primitive->recordCount = recordCount;
    primitive->keys = keys;
    primitive->cursorIndex = 0;
    primitive->cursorPosition = 0.0f;
    primitive->cursorStep = step;
    if (recordCount < 3) {
        primitive->cubicCoefficients = NULL;
        primitive->interpolationMode = 1;
    } else {
        coefficientResource = sdfMemAllocDirect(recordCount * 36);
        coefficients = (f32 (*)[EFF_CURVE_COMPONENT_COUNT])sdfMemLock(coefficientResource);
        primitive->allocation = primaryResource;
        primitive->coefficientAllocation = coefficientResource;
        primitive->interpolationMode = EFF_PRIMITIVE_CUBIC_MODE;
        primitive->recordCount = recordCount;
        primitive->keys = keys;
        primitive->cubicCoefficients = (f32 *)coefficients;
        primitive->quadraticCoefficients = (f32 *)(coefficients + recordCount);
        primitive->linearCoefficients = (f32 *)(coefficients + recordCount * 2);
        primitive->cursorIndex = 0;
        primitive->cursorPosition = 0.0f;
        primitive->cursorStep = step;
        dds3AllocSplinePoints(primitive, flattenEqualComponents);
    }
    return primitive;
}

/* Release the optional coefficient allocation, then the primary buffer; do not free or clear the object. */
void dds3FreeSplineHandle(EffPrimitiveCurve *primitive) {
    if (primitive != NULL) {
        if (primitive->cubicCoefficients != NULL) {
            sdfMemFreeDirect(primitive->coefficientAllocation);
        }
        sdfMemFreeDirect(primitive->allocation);
    }
}

/* Sample before stepping. Wrap at >1 only, subtract once, and return zero when the record cursor resets. */
s32 effSplRun(void *vertex, EffPrimitiveCurve *primitive) {
    s32 continuing = 1;
    f32 position = primitive->cursorPosition;
    u32 recordIndex = primitive->cursorIndex;

    effSplCalc(vertex, primitive, recordIndex, position);
    position += primitive->cursorStep;
    if (position > 1.0f) {
        position -= 1.0f;
        recordIndex += 1;
    }
    if (recordIndex >= primitive->recordCount - 1) {
        position = 0.0f;
        recordIndex = 0;
        continuing = 0;
    }
    primitive->cursorIndex = recordIndex;
    primitive->cursorPosition = position;
    return continuing;
}

/* Evaluate packed XYZ cubic coefficients or adjacent linear keys at t.
 * a/b serve as coefficients or endpoints; only the cubic path writes output W. */
void effSplCalc(EffVert *vertex, EffPrimitiveCurve *primitive, s32 recordIndex, f32 t) {
    f32 *a;
    f32 *b;
    f32 *c;
    f32 *d;

    if (primitive->interpolationMode == EFF_PRIMITIVE_CUBIC_MODE) {
        a = primitive->cubicCoefficients + recordIndex * EFF_CURVE_COMPONENT_COUNT;
        b = primitive->quadraticCoefficients + recordIndex * EFF_CURVE_COMPONENT_COUNT;
        c = primitive->linearCoefficients + recordIndex * EFF_CURVE_COMPONENT_COUNT;
        d = primitive->keys + recordIndex * EFF_CURVE_COMPONENT_COUNT;
        vertex->x = ((a[0] * t + b[0]) * t + c[0]) * t + d[0];
        vertex->y = ((a[1] * t + b[1]) * t + c[1]) * t + d[1];
        vertex->z = ((a[2] * t + b[2]) * t + c[2]) * t + d[2];
        vertex->w = 1.0f;
    } else {
        a = primitive->keys + recordIndex * EFF_CURVE_COMPONENT_COUNT;
        b = a + EFF_CURVE_COMPONENT_COUNT;
        vertex->x = a[0] + (b[0] - a[0]) * t;
        vertex->y = a[1] + (b[1] - a[1]) * t;
        vertex->z = a[2] + (b[2] - a[2]) * t;
    }
}

/* Evaluate packed XYZ cubic coefficients or adjacent linear keys into vf10.
 * The linear path leaves local W untouched before the native full-vector load. */
void effSplCalcRegist(EffPrimitiveCurve *primitive, s32 recordIndex, f32 t)
{
    f32 vertex[4];
    f32 *a;
    f32 *b;
    f32 *c;
    f32 *d;

    if (primitive->interpolationMode == EFF_PRIMITIVE_CUBIC_MODE) {
        a = primitive->cubicCoefficients + recordIndex * EFF_CURVE_COMPONENT_COUNT;
        b = primitive->quadraticCoefficients + recordIndex * EFF_CURVE_COMPONENT_COUNT;
        c = primitive->linearCoefficients + recordIndex * EFF_CURVE_COMPONENT_COUNT;
        d = primitive->keys + recordIndex * EFF_CURVE_COMPONENT_COUNT;
        vertex[0] = ((a[0] * t + b[0]) * t + c[0]) * t + d[0];
        vertex[1] = ((a[1] * t + b[1]) * t + c[1]) * t + d[1];
        vertex[2] = ((a[2] * t + b[2]) * t + c[2]) * t + d[2];
        vertex[3] = 1.0f;
    } else {
        a = primitive->keys + recordIndex * EFF_CURVE_COMPONENT_COUNT;
        b = a + EFF_CURVE_COMPONENT_COUNT;
        vertex[0] = a[0] + (b[0] - a[0]) * t;
        vertex[1] = a[1] + (b[1] - a[1]) * t;
        vertex[2] = a[2] + (b[2] - a[2]) * t;
    }
    SDF_VU0_LOADVECTOR_MEMOPS(*(u128 *)vertex);
}

/* Reset the primitive's record index and within-record position without changing its step. */
void effSplRewind(EffPrimitiveCurve *primitive) {
    primitive->cursorIndex = 0;
    primitive->cursorPosition = 0.0f;
}

/* Set the per-call cursor increment; sampling and wrapping are performed by the advance routine. */
void effSplSetStep(EffPrimitiveCurve *primitive, f32 step) {
    primitive->cursorStep = step;
}

/* Build temporary coordinate-major tangents, select a coefficient policy, then release the temporary buffer. */
void dds3AllocSplinePoints(EffPrimitiveCurve *primitive, s32 flattenEqualComponents) {
    sdfMemHandle_t *allocation = sdfMemAllocDirect(primitive->recordCount * EFF_CURVE_POINT_BYTES);
    f32 *tangentData = (f32 *)sdfMemLock(allocation);

    dds3BuildSplinePoints(tangentData, primitive->keys, primitive->recordCount);
    if (flattenEqualComponents == 0) {
        dds3BuildSplineCoefs(primitive, tangentData);
    } else {
        dds3BuildSplineCoefsSkipDup(primitive, tangentData);
    }
    sdfMemFreeDirect(allocation);
}

/* Per coordinate, estimate the slope at every key (one-sided at the ends, the
 * sum of both neighbour differences inside) and solve those into cubic
 * tangents, writing one coordinate-major run of recordCount values each. */
void dds3BuildSplinePoints(f32 *tangents, f32 *keys, u32 recordCount)
{
    s32 count = recordCount;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(count * sizeof(f32));
    f32 *slopes = (f32 *)sdfMemLock(allocation);
    f32 (*key)[EFF_CURVE_COMPONENT_COUNT] = (f32 (*)[EFF_CURVE_COMPONENT_COUNT])keys;
    s32 component;
    s32 i;

    for (component = 0; component < EFF_CURVE_COMPONENT_COUNT; component++) {
        slopes[0] = (key[1][component] - key[0][component]) * 3.0f;
        slopes[count - 1] = (key[count - 1][component] - key[count - 2][component]) * 3.0f;
        for (i = 1; i <= count - 2; i++) {
            slopes[i] = ((key[i][component] - key[i - 1][component]) +
                         (key[i + 1][component] - key[i][component])) * 3.0f;
        }
        SolveTridiagonal(tangents, slopes, count);
        tangents += count;
    }
    sdfMemFreeDirect(allocation);
}

/* Solve the cubic tangent system: endpoint diagonal 2, interior diagonal 4. */
void SolveTridiagonal(f32 *solution, f32 *rhs, s32 count) {
    s32 byteCount = count * sizeof(f32);
    sdfMemHandle_t *allocation = sdfMemAllocDirect(byteCount);
    f32 *upper;
    s32 i;
    f32 denominator;
    f32 *upperEnd;
    f32 *solutionEnd;
    f32 *rhsEnd;

    count -= 2;
    upper = (f32 *)sdfMemLock(allocation);
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
    sdfMemFreeDirect(allocation);
}

/* Build Hermite power-basis XYZ coefficients from interleaved points and coordinate-major tangents. */
void dds3BuildSplineCoefs(EffPrimitiveCurve *primitive, f32 *tangents) {
    f32 *cubic = primitive->cubicCoefficients;
    f32 *quadratic = primitive->quadraticCoefficients;
    f32 *linear = primitive->linearCoefficients;
    f32 *points = primitive->keys;
    s32 recordCount = primitive->recordCount;
    s32 segment;
    for (segment = 0; segment < recordCount - 1; segment++) {
        s32 component;
        for (component = 0; component < EFF_CURVE_COMPONENT_COUNT; component++) {
            f32 endValue = points[segment * EFF_CURVE_COMPONENT_COUNT + component + EFF_CURVE_COMPONENT_COUNT];
            f32 startValue = points[segment * EFF_CURVE_COMPONENT_COUNT + component];
            f32 startTangent = tangents[component * recordCount + segment];
            f32 endTangent = tangents[component * recordCount + segment + 1];
            cubic[segment * EFF_CURVE_COMPONENT_COUNT + component] = 2.0f * (startValue - endValue) +
                (startTangent + endTangent);
            quadratic[segment * EFF_CURVE_COMPONENT_COUNT + component] = (endValue - startValue) * 3.0f -
                (2.0f * startTangent + endTangent);
            linear[segment * EFF_CURVE_COMPONENT_COUNT + component] = startTangent;
        }
    }
}

/* Build Hermite coefficients, but flatten exactly equal endpoint components even when tangents are nonzero. */
void dds3BuildSplineCoefsSkipDup(EffPrimitiveCurve *primitive, f32 *tangents) {
    f32 *cubic = primitive->cubicCoefficients;
    f32 *quadratic = primitive->quadraticCoefficients;
    f32 *linear = primitive->linearCoefficients;
    f32 *points = primitive->keys;
    s32 recordCount = primitive->recordCount;
    s32 segment;
    s32 component;

    for (segment = 0; segment < recordCount - 1; segment++) {
        for (component = 0; component < EFF_CURVE_COMPONENT_COUNT; component++) {
            f32 startValue = points[segment * EFF_CURVE_COMPONENT_COUNT + component];
            f32 endValue = points[segment * EFF_CURVE_COMPONENT_COUNT + component + EFF_CURVE_COMPONENT_COUNT];

            if (startValue == endValue) {
                cubic[segment * EFF_CURVE_COMPONENT_COUNT + component] = 0.0f;
                quadratic[segment * EFF_CURVE_COMPONENT_COUNT + component] = 0.0f;
                linear[segment * EFF_CURVE_COMPONENT_COUNT + component] = 0.0f;
            } else {
                f32 startTangent = tangents[component * recordCount + segment];
                f32 endTangent = tangents[component * recordCount + segment + 1];

                cubic[segment * EFF_CURVE_COMPONENT_COUNT + component] = 2.0f * (startValue - endValue) + (startTangent + endTangent);
                quadratic[segment * EFF_CURVE_COMPONENT_COUNT + component] = (endValue - startValue) * 3.0f - (2.0f * startTangent + endTangent);
                linear[segment * EFF_CURVE_COMPONENT_COUNT + component] = startTangent;
            }
        }
    }
}

/* Allocate the small channel when at least four records exist; borrow the supplied rows and set its default step. */
EffChan *effCreateChannel(EffFloatRows *rows, u32 recordCount) {
    EffChan *channel = NULL;
    sdfMemHandle_t *allocation;
    EffChan *channelObject;

    if (recordCount < EFF_BEZIER_MIN_RECORD_COUNT) {
        return channel;
    }
    allocation = sdfMemAllocDirect(EFF_CHANNEL_BYTES);
    channel = (void *)sdfMemLock(allocation);
    channelObject = channel;
    channelObject->allocation = allocation;
    channelObject->cursorStep = EFF_CHANNEL_DEFAULT_STEP;
    channelObject->recordCount = recordCount;
    channelObject->rows = rows;
    channelObject->cursorPosition = 0;
    channelObject->cursorIndex = 0;
    return channel;
}

/* Release the handle at a nonnull address without clearing it.
 * The native s32 signature has no explicit return; retain that contract. */
s32 effReleaseInterpolationChannel(void **handleAddress) {
    if (handleAddress != NULL) {
        sdfMemFreeDirect(*handleAddress);
    }
}

/* Sample before stepping, then advance three records only when position exceeds one.
 * Subtract once; return zero when the record cursor resets at the unsigned count-minus-one boundary. */
s32 effAdvanceChanCursor(void *vertex, EffChan *channel) {
    s32 continuing = 1;
    f32 position = channel->cursorPosition;
    u32 recordIndex = channel->cursorIndex;

    effSampleChannelBezier(vertex, channel, recordIndex, position);
    position += channel->cursorStep;
    if (position > 1.0f) {
        position -= 1.0f;
        recordIndex += EFF_BEZIER_RECORD_ADVANCE;
    }
    if (recordIndex >= channel->recordCount - 1) {
        position = 0.0f;
        recordIndex = 0;
        continuing = 0;
    }
    channel->cursorIndex = recordIndex;
    channel->cursorPosition = position;
    return continuing;
}

/* Evaluate four packed XYZ Bezier controls at t and write homogeneous W=1.
 * Preserve the native weight multiplication and sum association. */
void effSampleChannelBezier(EffVert *vertex, EffChan *channel, s32 recordIndex, f32 t) {
    f32 weights[4];
    f32 oneMinusT = 1.0f - t;
    f32 *p0 = channel->rows->primary + recordIndex * EFF_CURVE_COMPONENT_COUNT;
    f32 *p1 = p0 + EFF_CURVE_COMPONENT_COUNT;
    f32 *p2 = p0 + 6;
    f32 *p3 = p0 + 9;

    weights[0] = oneMinusT * oneMinusT * oneMinusT;
    weights[1] = t * (oneMinusT * oneMinusT) * 3.0f;
    weights[2] = t * t * oneMinusT * 3.0f;
    weights[3] = t * t * t;
    vertex->x = p0[0] * weights[0] + p1[0] * weights[1] + p2[0] * weights[2] + p3[0] * weights[3];
    vertex->y = p0[1] * weights[0] + p1[1] * weights[1] + p2[1] * weights[2] + p3[1] * weights[3];
    vertex->z = p0[2] * weights[0] + p1[2] * weights[1] + p2[2] * weights[2] + p3[2] * weights[3];
    vertex->w = 1.0f;
}

/* Reset the channel's record index and within-record position without changing its step. */
void effClearChanCursor(EffChan *channel) {
    channel->cursorIndex = 0;
    channel->cursorPosition = 0;
}

/* Set the per-call channel cursor increment without advancing it. */
void effSetChanStep(EffChan *channel, f32 step) {
    channel->cursorStep = step;
}
