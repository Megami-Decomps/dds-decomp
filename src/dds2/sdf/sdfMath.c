#include "sdk_ps2.h"
#include "sdf_request.h"
#include "common.h"
#include "sdf_vu0.h"
#include "ee_mmi.h"
#include "sdf_trig.h"
#include "sdf_chip.h"
#include "sdf_heap_api.h"
#include "sdf_draw.h"
#include "sdf_sif_command.h"
#include "sdf_dev_event.h"
#include "sdf_dev_protocol.h"
#include "sdf_dev_state.h"
#include "sdf_memory_api.h"

#define SDF_TRIG_INVERSE_TAU 0.15915494f

#define SDF_TRIG_HALF_PI 1.5707963f

#define SDF_TRIG_PI 3.1415926f

#define SDF_TRIG_TAU 6.2831852f

#define SDF_SINE_POLYNOMIAL_SCALE 3.9999996f

#define SDF_ASIN_SAMPLE_COUNT 128

extern f32 sdfNormalizedAsinSamples[];

/* Mirror the phase into a quarter turn, then evaluate an odd ninth-degree polynomial. */
f32 sdfSin(f32 angle) {
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
f32 sdfCos(f32 angle) {
    return sdfSin(angle + SDF_TRIG_HALF_PI);
}

/* Binary-search samples, using zero as the implicit lower endpoint.
 * The step is integer 1/sampleCount converted to f32, not a float reciprocal.
 * The caller must provide valid table bounds for the final sample index.
 */
f32 sdfTableInterpolation(f32 lookupValue, const f32 *table, s32 sampleCount) {
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
        if (lookupValue < upperValue) {
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
    return sampleIndex * positionStep + (lookupValue - lowerValue) * positionStep / (upperValue - lowerValue);
}

/* Odd fifth-degree atan approximation; the ratio is not range-checked here. */
f32 sdfAtan(f32 ratio) {
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
        r = sdfAtan(y / x);
    } else {
        r = 1.5707963f - sdfAtan(x / y);
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
f32 sdfAsin(f32 x) {
    f32 inputSign;
    f32 angleMagnitude;

    if (x < 0.0f) {
        x = -x;
        inputSign = -1.0f;
    } else {
        inputSign = 1.0f;
    }
    angleMagnitude = x >= 1.0f ? SDF_TRIG_HALF_PI : sdfTableInterpolation(x, sdfNormalizedAsinSamples, SDF_ASIN_SAMPLE_COUNT) * SDF_TRIG_HALF_PI;
    return angleMagnitude * inputSign;
}

/* Return the input-signed complement of the sampled angle, not standard acos(x).
 * Magnitudes at or above one leave the angle magnitude at zero.
 */
f32 sdfAcos(f32 x) {
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
        angleMagnitude = (1.0f - sdfTableInterpolation(x, sdfNormalizedAsinSamples, SDF_ASIN_SAMPLE_COUNT)) * SDF_TRIG_HALF_PI;
    }
    return angleMagnitude * inputSign;
}

/* Keep the cast-plus/minus-one turn adjustment; this is not general modulo.
 * Inputs already within the pi thresholds are returned unchanged.
 */
f32 sdfClipAngleValuePI(f32 angle) {
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
