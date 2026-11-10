#include "fpu.h"
#include "common.h"
#include "libsdf.h"
#include "ps2asm.h"

#define SDF_PI_2 1.5707963f

f32 sdfSin(f32 aRadius);

enum SdfFileIOCmdStat {
    SDF_FILE_CMD_STAT_INIT = 0,
    SDF_FILE_CMD_STAT_DORMANT = 1,
    SDF_FILE_CMD_STAT_OPEN = 2,
    SDF_FILE_CMD_STAT_SEEK = 3,
    SDF_FILE_CMD_STAT_READ = 4,
    SDF_FILE_CMD_STAT_WRITE = 5,
    SDF_FILE_CMD_STAT_CLOSE = 6,
    SDF_FILE_CMD_STAT_SUSPEND = 7,
    SDF_FILE_CMD_STAT_DONE = 8,
    SDF_FILE_CMD_STAT_ERROR = 9,
    SDF_FILE_CMD_STAT_CANCEL = 10
};

enum SdfFileIOCmd {
    SDF_FILE_CMD_NONE = 0,
    SDF_FILE_CMD_READOPEN = 1,
    SDF_FILE_CMD_WRITEOPEN = 2,
    SDF_FILE_CMD_SEEK = 3,
    SDF_FILE_CMD_FILESIZE = 4,
    SDF_FILE_CMD_READ = 5,
    SDF_FILE_CMD_WRITE = 6,
    SDF_FILE_CMD_CLOSE = 7,
    SDF_FILE_CMD_READFILE = 8,
    SDF_FILE_CMD_WRITEFILE = 9
};

enum sdf_tex_resource {
    SDF_TEX_RESOURCE_FREE = 0,
    SDF_TEX_RESOURCE_FRAME = 1,
    SDF_TEX_RESOURCE_TEXTURE = 2,
    SDF_TEX_RESOURCE_CLUT = 3,
    SDF_TEX_RESOURCE_KNOWN_MODE_COUNT = 4
};

enum sdf_stream_output {
    SDF_STREAM_OUTPUT_RGBA32 = 0,
    SDF_STREAM_OUTPUT_RGBA16 = 1,
    SDF_STREAM_OUTPUT_RGBA16_DITHERED = 2
};

enum sdf_stream_playback {
    SDF_STREAM_PLAYBACK_INITIAL = 0,
    SDF_STREAM_PLAYBACK_FIRST_COMPLETION = 1,
    SDF_STREAM_PLAYBACK_CADENCED = 2
};

extern f32 sdfAsinTable[];

extern s32 (*p2sdSmgBuf)(void);

/* Mirror the phase into a quarter turn, then evaluate an odd ninth-degree polynomial. */
f32 sdfSin(f32 aRadius) {
    f32 phase = aRadius * 0.15915494f;
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
    polynomialInput = phase * 3.9999996f;
    inputSquared = polynomialInput * polynomialInput;
    inputCubed = inputSquared * polynomialInput;
    inputFifthPower = inputCubed * inputSquared;
    inputSeventhPower = inputFifthPower * inputSquared;
    inputNinthPower = inputSeventhPower * inputSquared;
    return polynomialInput * 1.5707963f + inputCubed * -0.64596367f + inputFifthPower * 0.07968968f + inputSeventhPower * -0.0046737656f + inputNinthPower * 0.00015148419f;
}

f32 sdfCos(f32 aRadius)
{
    return sdfSin(aRadius + SDF_PI_2);
}

/* Binary-search samples, using zero as the implicit lower endpoint.
 * The step is integer 1/sampleCount converted to f32, not a float reciprocal.
 * The caller must provide valid table bounds for the final sample index.
 */
f32 sdfTableInterpolation(f32 aValue, const f32 *aTable, s32 aTableSize) {
    f32 positionStep = 1 / aTableSize;
    s32 lowerIndex = 0;
    s32 upperIndex = aTableSize;
    s32 sampleIndex;
    f32 lowerValue;
    f32 upperValue;
    do {
        sampleIndex = lowerIndex + upperIndex;
        sampleIndex >>= 1;
        upperValue = aTable[sampleIndex];
        if (aValue < upperValue) {
            upperIndex = sampleIndex;
        } else {
            sampleIndex++;
            lowerIndex = sampleIndex;
        }
    } while (lowerIndex < upperIndex);
    upperValue = aTable[sampleIndex];
    lowerValue = 0.0f;
    if (sampleIndex != 0) {
        lowerValue = aTable[sampleIndex - 1];
    }
    return sampleIndex * positionStep + (aValue - lowerValue) * positionStep / (upperValue - lowerValue);
}

f32 sdfAtan(f32 aX)
{
    f32 x2 = aX * aX;
    f32 x3 = x2 * aX;
    f32 x5 = x2 * x3;

    return aX * 0.9999998f + x3 * -0.33325735f + x5 * 0.19388643f;
}

f32 sdfAtan2(f32 aY, f32 aX) {
    s32 xNegative = 0;
    s32 yNegative;
    f32 angle;
    if (aX < 0.0f) {
        aX = -aX;
        xNegative = 1;
    }
    yNegative = 0;
    if (aY < 0.0f) {
        aY = -aY;
        yNegative = 1;
    }
    if (aY < aX) {
        angle = sdfAtan(aY / aX);
    } else {
        angle = 1.5707963f - sdfAtan(aX / aY);
    }
    if (xNegative != 0) {
        angle = 3.1415926f - angle;
    }
    if (yNegative != 0) {
        angle = -angle;
    }
    return angle;
}

/* Restore the input sign after sampling; magnitudes at or above one use half pi. */
f32 sdfAsin(f32 aX) {
    f32 inputSign;
    f32 angleMagnitude;
    if (aX < 0.0f) {
        aX = -aX;
        inputSign = -1.0f;
    } else {
        inputSign = 1.0f;
    }
    angleMagnitude = aX >= 1.0f ? 1.5707963f : sdfTableInterpolation(aX, sdfAsinTable, 128) * 1.5707963f;
    return angleMagnitude * inputSign;
}

/* Return the input-signed complement of the sampled angle, not standard acos(x).
 * Magnitudes at or above one leave the angle magnitude at zero.
 */
f32 sdfAcos(f32 aX) {
    f32 inputSign;
    f32 angleMagnitude;
    if (aX < 0.0f) {
        aX = -aX;
        inputSign = -1.0f;
    } else {
        inputSign = 1.0f;
    }
    angleMagnitude = 0.0f;
    if (!(aX >= 1.0f)) {
        angleMagnitude = (1.0f - sdfTableInterpolation(aX, sdfAsinTable, 128)) * 1.5707963f;
    }
    return angleMagnitude * inputSign;
}

/* Keep the cast-plus/minus-one turn adjustment; this is not general modulo.
 * Inputs already within the pi thresholds are returned unchanged.
 */
f32 sdfClipAngleValuePI(f32 aValue) {
    s32 adjustedTurns;
    if (aValue > 3.1415926f) {
        adjustedTurns = (s32)(aValue / 6.2831852f) + 1;
        return aValue - (f32)adjustedTurns * 6.2831852f;
    }
    if (aValue < -3.1415926f) {
        adjustedTurns = (s32)(aValue / 6.2831852f) - 1;
        return aValue - (f32)adjustedTurns * 6.2831852f;
    }
    return aValue;
}
