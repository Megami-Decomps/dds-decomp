#include "fpu.h"
#include "common.h"
#include "libsdf.h"
#include "ps2asm.h"

f32 sdfSin(f32 aRadius);

f32 sdfClipAngleValuePI(f32 aValue);

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

typedef union { f32 quaternion[4]; u16 monthDays[20]; } sdfMathConstants_t;

extern sdfMathConstants_t sdfMathConstants;

extern f32 sdfQuatPoleDown[4];

extern f32 sdfQuatPoleUp[4];

extern f32 sdfUnitXVector[4];

extern s32 (*p2sdSmgBuf)(void);

/* Return the input-signed complement of the sampled angle, not standard acos(x). * Magnitudes at or above one leave the angle magnitude at zero. */ f32 sdfAcos(f32 x);

f32 sdfAtan2(f32 aY, f32 aX);

f32 sdfCos(f32 aRadius);

/* vu0 routine: shared quaternion registers are the input/output interface. */
void Mult(void)
{
    VU0_SEQ_MULT();
}

/* vu0 routine: the conjugate over the squared norm. */
void Inverse(void)
{
    VU0_SEQ_INVERSE();
}

/* vu0 routine: normalize all four components of vf10. The IDB called this
 * SDF_QUAT_INNERPRODUCT, which belongs to the dot product below. */
void Normalize(void)
{
    VU0_SEQ_NORMALIZE();
}


/* Squared length, but retail's vmulay multiplies x*x by y*y instead of adding them. */
f32 LengthSq(void)
{
    f32 componentMetric;
    VU0_QUAT_LENGTHSQ(componentMetric);
    return componentMetric;
}

/* vu0 routine: return the four-lane dot product of vf10 and vf11. */
f32 SDF_QUAT_INNERPRODUCT(void)
{
    f32 dotProduct;
    VU0_QUAT_DOT_VF10_VF11(dotProduct);
    return dotProduct;
}

/* vf10 initially holds the axis; write the sine-scaled axis and cosine W. */
void FromRotation(f32 aAngle)
{
    f32 halfAngle = aAngle * 0.5f;
    f32 trigValue = sdfSin(halfAngle);
    SDF_VU0_SCALEVECTOR_V0(trigValue);
    trigValue = sdfCos(halfAngle);
    VU0_SEQ_FROM_ROTATION(trigValue);
}

/* As above, but write the second VU quaternion register (vf11). */
void FromRotation_B(f32 aAngle)
{
    f32 halfAngle = aAngle * 0.5f;
    f32 trigValue = sdfSin(halfAngle);
    SDF_VU0_SCALEVECTOR_B_V0(trigValue);
    trigValue = sdfCos(halfAngle);
    VU0_SEQ_FROM_ROTATION_B(trigValue);
}

/* vu0 routine: shared quaternion registers are the input/output interface. */
void ToMatrix(void)
{
    VU0_SEQ_TO_MATRIX();
}

/* vu0 routine: the composed quaternion is left in the shared register. */
void FromEulerXYZ(f32 aRotateX, f32 aRotateY, f32 aRotateZ)
{
    /* VU lqc2 reads this scratch through its address; retain retail store order. */
    volatile f32 quaternion[4] __attribute__((aligned(16)));
    f32 half;
    f32 cx;
    f32 sx;
    f32 cy;
    f32 sy;
    f32 cz;
    f32 sz;

    half = sdfClipAngleValuePI(-aRotateX) * 0.5f;
    cx = sdfCos(half);
    sx = sdfSin(half);
    half = sdfClipAngleValuePI(-aRotateY) * 0.5f;
    cy = sdfCos(half);
    sy = sdfSin(half);
    half = sdfClipAngleValuePI(-aRotateZ) * 0.5f;
    cz = sdfCos(half);
    sz = sdfSin(half);
    quaternion[0] = sz * sy * cx + cz * cy * sx;
    quaternion[1] = cz * sy * cx - sz * cy * sx;
    quaternion[2] = sz * cy * cx + cz * sy * sx;
    quaternion[3] = cz * cy * cx - sz * sy * sx;
    SDF_VU0_LOADVECTOR((f32 *)quaternion);
}

/* vu0 routine: the composed quaternion is left in the shared register. */
void FromEuler(f32 aRotateX, f32 aRotateY, f32 aRotateZ)
{
    /* VU lqc2 reads this scratch through its address; retain retail store order. */
    volatile f32 quaternion[4] __attribute__((aligned(16)));
    f32 half;
    f32 cx;
    f32 sx;
    f32 cy;
    f32 sy;
    f32 cz;
    f32 sz;

    half = -aRotateX * 0.5f;
    cx = sdfCos(half);
    sx = sdfSin(half);
    half = -aRotateY * 0.5f;
    cy = sdfCos(half);
    sy = sdfSin(half);
    half = -aRotateZ * 0.5f;
    cz = sdfCos(half);
    sz = sdfSin(half);
    quaternion[0] = sz * sy * cx + cz * cy * sx;
    quaternion[1] = cz * sy * cx - sz * cy * sx;
    quaternion[2] = sz * cy * cx + cz * sy * sx;
    quaternion[3] = cz * cy * cx - sz * sy * sx;
    SDF_VU0_LOADVECTOR((f32 *)quaternion);
}

/* vu0 routine: compose pitch and offset-adjusted planar direction quaternions. */
void LookAt(f32 aFrontX, f32 aFrontZ) {
    sdfVector4_t input;
    sdfVector4_t quaternion;
    f32 x;
    f32 y;
    f32 z;
    f32 horizontalSquared;
    f32 horizontalLength;
    f32 s;
    f32 t;
    f32 offsetSquared;
    f32 adjustedZ;
    SDF_VU0_STOREVECTOR(&input);
    t = 0.0f;
    x = -input.x;
    y = input.y;
    if (aFrontZ < t) {
        y = -y;
    }
    z = input.z;
    horizontalSquared = x * x + z * z;
    if (horizontalSquared < 0.000001f) {
        if (y < t) {
            SDF_VU0_LOADVECTOR(sdfQuatPoleDown);
        } else {
            SDF_VU0_LOADVECTOR(sdfQuatPoleUp);
        }
        return;
    }
    /* Pitch quaternion from the vertical and horizontal direction lengths. */
    s = y * y;
    horizontalLength = fsqrtf(horizontalSquared);
    quaternion.y = t;
    quaternion.z = t;
    t = fsqrtf(horizontalSquared + s) + horizontalLength;
    s = frsqrtf(1.0f, t * t + s);
    quaternion.x = y * s;
    quaternion.w = t * s;
    SDF_VU0_LOADVECTOR(&quaternion);
    /* Yaw quaternion from the normalized, offset-adjusted XZ direction. */
    s = 1.0f / horizontalLength;
    x *= s;
    z *= s;
    adjustedZ = z + aFrontZ;
    t = x + aFrontX;
    offsetSquared = t * t + adjustedZ * adjustedZ;
    if (offsetSquared < 0.000001f) {
        SDF_VU0_LOADVECTOR_B(sdfMathConstants.quaternion);
    } else {
        s = frsqrtf(1.0f, offsetSquared);
        quaternion.x = 0.0f;
        quaternion.y = t * s;
        quaternion.z = 0.0f;
        quaternion.w = adjustedZ * s;
        SDF_VU0_LOADVECTOR_B(&quaternion);
    }
    Mult();
}

/* vu0 routine: blend vf10/vf11 along the shorter quaternion arc into vf10.
 * blendAmount is used without clamping; the final step normalizes either blend. */
void Slerp(f32 aRatio) {
    f32 dotProduct;
    f32 firstWeight;
    f32 secondWeight;
    f32 sineAngle;
    f32 angleBetween;
    VU0_QUAT_DOT_VF10_VF11(dotProduct);
    if (dotProduct < 0.0f) {
        dotProduct = -dotProduct;
        VU0_NEGATE_VF11_TO_VF12();
    } else {
        SDF_VU0_COPYVECTOR_B_TO_C();
    }
    firstWeight = 1.0f - aRatio;
    secondWeight = aRatio;
    /* Near-identical quaternions use linear weights; normalization still follows. */
    if (dotProduct < 0.99899996f) {
        angleBetween = sdfAcos(dotProduct);
        sineAngle = sdfSin(angleBetween);
        firstWeight = sdfSin(firstWeight * angleBetween) / sineAngle;
        secondWeight = sdfSin(secondWeight * angleBetween) / sineAngle;
    }
    VU0_SET_WEIGHTS_VF2X_VF3X(firstWeight, secondWeight);
    VU0_WEIGHTED_SUM_VF10_VF12();
    Normalize();
}

/* vu0 routine: shared quaternion registers are the input/output interface. */
void Lerp(f32 aRatio)
{
    f32 dotProduct;
    VU0_QUAT_DOT_VF10_VF11(dotProduct);
    if (dotProduct < 0.0f) {
        VU0_NEGATE_VF11_TO_VF12();
    } else {
        SDF_VU0_COPYVECTOR_B_TO_C();
    }
    {
        f32 firstWeight = 1.0f - aRatio;
        VU0_SET_WEIGHTS_VF2X_VF3X(firstWeight, aRatio);
        VU0_WEIGHTED_SUM_VF10_VF12();
    }
    Normalize();
}

/* vu0 routine: shared quaternion registers are the input/output interface. */
f32 EulerAngleAxisY(void)
{
    f32 rotatedZ;
    f32 rotatedX;
    ToMatrix();
    /* PEXEW exchanges words 0 and 2: output 0 is Z, while output 1 is X. */
    SDF_VU0_LOADVECTOR_BARRIER(sdfUnitXVector);
    SDF_VU0_APPLYMATRIX_XYZ();
    VU0_SEQ_EULER_ANGLE_AXIS_Y(rotatedZ, rotatedX);
    return sdfAtan2(rotatedZ, rotatedX);
}
