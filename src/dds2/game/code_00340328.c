#include "sdf_request.h"
#include "common.h"
#include "sdf_chip.h"
#include "sdf_resource.h"
#include "sdf_draw.h"
#include "sdf_sif_command.h"
#include "sdf_dev_event.h"
#include "sdf_dev_protocol.h"
#include "sdf_dev_state.h"

#define SDF_BCD_DIGIT_BITS 4

#define SDF_DECIMAL_RADIX 10

#define SDF_TRIG_INVERSE_TAU 0.15915494f

#define SDF_TRIG_HALF_PI 1.5707963f

#define SDF_TRIG_PI 3.1415926f

#define SDF_TRIG_TAU 6.2831852f

#define SDF_SINE_POLYNOMIAL_SCALE 3.9999996f

#define SDF_ASIN_SAMPLE_COUNT 128

extern u8 sdfDevicePriorityOverrideTicks;

extern s32 sdfDiscRequestPending;

extern s32 sdfDeviceWorkerPriority;

extern u8 D_00438B50[];

extern u8 D_00438B58[];

extern u32 strlen(const char *s);

extern f32 sdfNormalizedAsinSamples[];

extern u32 sdfDiscType;

extern u16 sdfDefaultDevRequestOptions;

extern s32 sdfDevReplySemaphore;

extern u8 D_00438B68;

extern f32 sdfSinPoly(f32 arg0);

typedef struct Bytes7 {
    s8 b[7];
} Bytes7;

extern Bytes7 D_00438AF0[];

extern s8 D_00438AB8;

extern char D_00438AC0[];

extern char sdfDiscPathPrefix[];

typedef struct Bytes6 {
    s8 b[6];
} Bytes6;

extern Bytes6 sdfPfsPathPrefix[];

extern char D_00438B00[]; /* cdrom */

extern u8 D_00438AE0;

extern s8 D_00438B1E;

extern DevState *D_00438B08;

extern DevState *D_00438B0C;

extern s16 D_00438B1C;

extern DevState *D_00438B14;

extern DevState *D_00438B18;

extern s16 D_00438B10;

extern char D_00438B28[]; /* cdrom0: */

extern char D_00438B30[]; /* host0: */

extern char D_00438B38[]; /* pfs0: */

extern char D_00438B60[];

INCLUDE_ASM(const s32, "game/code_00340328", sdfClearQuadwords);

char *sdfStrDup(const char *text) {
    u32 length;
    char *copy;

    if (text == NULL) {
        return NULL;
    }
    length = strlen(text);
    copy = sdfAllocSizeClassBlock(length + 1);
    memcpy(copy, text, length);
    copy[length] = 0;
    return copy;
}

s32 sdfBcdStrToInt(s32 packedDigits) {
    s32 place = 1;
    s32 acc = 0;

    while (packedDigits > 0) {
        acc += (packedDigits & 0xf) * place;
        packedDigits >>= 4;
        place *= 10;
    }
    return acc;
}

/* Pack positive decimal digits into nibbles; non-positive input returns zero.
 * The original signed shifts and lack of an overflow check are retained.
 */
s32 sdfDecimalToPackedDigits(s32 number) {
    s32 digitShift = 0;
    s32 packedDigits = 0;

    while (number > 0) {
        s32 quotient = number / SDF_DECIMAL_RADIX;
        packedDigits |= (number - quotient * SDF_DECIMAL_RADIX) << digitShift;
        number = quotient;
        digitShift += SDF_BCD_DIGIT_BITS;
    }
    return packedDigits;
}

/* Allocate element storage with a fixed element stride and growth increment. */
DevRequest *sdfDevCreateBufferedRequest(s32 elementCount, s32 elementStride, s32 growthStep) {
    DevRequest *request = sdfAllocSizeClassBlock(sizeof(*request));

    request->growStep = growthStep;
    request->usedCount = 0;
    request->capacity = elementCount;
    request->stride = elementStride;
    if (elementCount != 0) {
        request->backingAllocation = sdfAllocGeneralBlock(elementStride * elementCount);
        request->buffer = (void *)sdfResourceRetainAddress(request->backingAllocation);
    } else {
        request->backingAllocation = 0;
        request->buffer = 0;
    }
    return request;
}

/* Release the backing allocation and the request object. */
void sdfDestroyDevRequest(DevRequest *request) {
    sdfReleaseResourceAllocation(request->backingAllocation);
    sdfReleaseChipBlock(request);
}

extern void func_00329600(struct SdfMemBlock *allocation, s32 size);

/* Grow capacity, preserving the SDK's signed 16-bit allocation-size arithmetic. */
void sdfDevBufferedRequestGrow(DevRequest *request) {
    if (request->backingAllocation == 0) {
        sdfDevResizeBufferedRequest(request, request->growStep);
        return;
    }
    sdfDecrementAllocationReferenceCount(request->backingAllocation);
    request->capacity = request->capacity + request->growStep;
    func_00329600(request->backingAllocation, (s16)request->capacity * request->stride);
    request->buffer = (void *)sdfResourceRetainAddress(request->backingAllocation);
}

/* Resize storage and clamp the live entry count to the new capacity. */
void sdfDevResizeBufferedRequest(DevRequest *request, s32 elementCount) {
    if (request->backingAllocation == 0) {
        if (elementCount > 0) {
            request->capacity = elementCount;
            request->backingAllocation = sdfAllocGeneralBlock(request->stride * elementCount);
            request->buffer = (void *)sdfResourceRetainAddress(request->backingAllocation);
        }
    } else if (elementCount <= 0) {
        sdfReleaseResourceAllocation(request->backingAllocation);
        request->backingAllocation = 0;
        request->usedCount = 0;
        request->capacity = 0;
        request->buffer = 0;
    } else {
        sdfDecrementAllocationReferenceCount(request->backingAllocation);
        request->capacity = elementCount;
        func_00329600(request->backingAllocation, request->stride * elementCount);
        request->buffer = (void *)sdfResourceRetainAddress(request->backingAllocation);
        if (elementCount < request->usedCount) {
            request->usedCount = elementCount;
        }
    }
}

/* Mirror the phase into a quarter turn, then evaluate an odd ninth-degree polynomial. */
f32 sdfSinPoly(f32 angle) {
    f32 phase = angle * SDF_TRIG_INVERSE_TAU;
    f32 polynomialInput;
    f32 inputSquared;
    f32 inputCubed;
    f32 inputFifthPower;
    f32 inputSeventhPower;
    f32 inputNinthPower;

    phase -= (s32)phase;
    if (phase > 0.5f) {
        phase -= 1.0f;
    } else if (phase < -0.5f) {
        phase += 1.0f;
    }
    if (phase > 0.25f) {
        phase = 0.5f - phase;
    } else if (phase < -0.25f) {
        phase = -0.5f - phase;
    }
    polynomialInput = phase * SDF_SINE_POLYNOMIAL_SCALE;
    inputSquared = polynomialInput * polynomialInput;
    inputCubed = inputSquared * polynomialInput;
    inputFifthPower = inputCubed * inputSquared;
    inputSeventhPower = inputFifthPower * inputSquared;
    inputNinthPower = inputSeventhPower * inputSquared;
    return polynomialInput * SDF_TRIG_HALF_PI + inputCubed * -0.64596367f + inputFifthPower * 0.07968968f + inputSeventhPower * -0.0046737656f + inputNinthPower * 0.00015148419f;
}

/* Apply a quarter-turn phase shift to the existing sine approximation. */
f32 sdfEvaluateCosineViaSinePhaseShift(f32 angle) {
    return sdfSinPoly(angle + SDF_TRIG_HALF_PI);
}

/* Binary-search samples, using zero as the implicit lower endpoint.
 * The step is integer 1/sampleCount converted to f32, not a float reciprocal.
 * The caller must provide valid table bounds for the final sample index.
 */
f32 sdfTableInterpolate(f32 value, f32 *table, s32 sampleCount) {
    f32 positionStep = 1 / sampleCount;
    s32 lowerIndex = 0;
    s32 upperIndex = sampleCount;
    s32 sampleIndex;
    f32 lowerValue;
    f32 upperValue;

    do {
        sampleIndex = lowerIndex + upperIndex;
        sampleIndex >>= 1;
        upperValue = table[sampleIndex];
        if (value < upperValue) {
            upperIndex = sampleIndex;
        } else {
            sampleIndex++;
            lowerIndex = sampleIndex;
        }
    } while (lowerIndex < upperIndex);
    upperValue = table[sampleIndex];
    lowerValue = 0.0f;
    if (sampleIndex != 0) {
        lowerValue = table[sampleIndex - 1];
    }
    return sampleIndex * positionStep + (value - lowerValue) * positionStep / (upperValue - lowerValue);
}

/* Odd fifth-degree atan approximation; the ratio is not range-checked here. */
f32 sdfAtan2Poly(f32 ratio) {
    f32 ratioSquared = ratio * ratio;
    f32 ratioCubed = ratioSquared * ratio;
    f32 ratioFifthPower = ratioSquared * ratioCubed;

    return ratio * 0.99999977f + ratioCubed * -0.33325735f + ratioFifthPower * 0.19388643f;
}

f32 sdfAtan2(f32 y, f32 x) {
    s32 sx = 0;
    s32 sy;
    f32 r;

    if (x < 0.0f) {
        x = -x;
        sx = 1;
    }
    sy = 0;
    if (y < 0.0f) {
        y = -y;
        sy = 1;
    }
    if (y < x) {
        r = sdfAtan2Poly(y / x);
    } else {
        r = 1.5707963f - sdfAtan2Poly(x / y);
    }
    if (sx != 0) {
        r = 3.1415926f - r;
    }
    if (sy != 0) {
        r = -r;
    }
    return r;
}

/* Restore the input sign after sampling; magnitudes at or above one use half pi. */
f32 sdfAsinTable(f32 x) {
    f32 inputSign;
    f32 angleMagnitude;

    if (x < 0.0f) {
        x = -x;
        inputSign = -1.0f;
    } else {
        inputSign = 1.0f;
    }
    angleMagnitude = x >= 1.0f ? SDF_TRIG_HALF_PI : sdfTableInterpolate(x, sdfNormalizedAsinSamples, SDF_ASIN_SAMPLE_COUNT) * SDF_TRIG_HALF_PI;
    return angleMagnitude * inputSign;
}

/* Return the input-signed complement of the sampled angle, not standard acos(x).
 * Magnitudes at or above one leave the angle magnitude at zero.
 */
f32 sdfAcosTable(f32 x) {
    f32 inputSign;
    f32 angleMagnitude;

    if (x < 0.0f) {
        x = -x;
        inputSign = -1.0f;
    } else {
        inputSign = 1.0f;
    }
    angleMagnitude = 0.0f;
    if (!(x >= 1.0f)) {
        angleMagnitude = (1.0f - sdfTableInterpolate(x, sdfNormalizedAsinSamples, SDF_ASIN_SAMPLE_COUNT)) * SDF_TRIG_HALF_PI;
    }
    return angleMagnitude * inputSign;
}

/* Keep the cast-plus/minus-one turn adjustment; this is not general modulo.
 * Inputs already within the pi thresholds are returned unchanged.
 */
f32 sdfWrapAngle(f32 angle) {
    s32 adjustedTurns;
    if (angle > SDF_TRIG_PI) {
        adjustedTurns = (s32)(angle / SDF_TRIG_TAU) + 1;
        return angle - (f32)adjustedTurns * SDF_TRIG_TAU;
    }
    if (angle < -SDF_TRIG_PI) {
        adjustedTurns = (s32)(angle / SDF_TRIG_TAU) - 1;
        return angle - (f32)adjustedTurns * SDF_TRIG_TAU;
    }
    return angle;
}

