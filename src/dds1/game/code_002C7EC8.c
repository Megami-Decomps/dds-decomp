#include "common.h"
#include "fpu.h"
#include "pcp_vu0.h"

extern u32 itfCreateConvertedTextGlyph(s32, s32, s32, u32, const u8 *, s32);

typedef struct Vec3 {
    float x; // 0x00
    float y; // 0x04
    float z; // 0x08
} Vec3; // 0x0C

extern float fldNormalizedVectorDot(float *, float *);

extern float fldVectorLength(float *vector);

extern float func_002FA1C0(float);

extern u32 sdfAllocGeneralBlock(s32 size);

extern void *sdfMemoryGetBlockAddress(u32 handle);

void sdfCounterDrawGlyphAtGridCell(s32 gridX, s32 gridY, u32 style, const u8 *text) {
    u32 glyph;

    glyph = itfCreateConvertedTextGlyph(gridX << 4, gridY << 3, 0, style, text, 0);
    frFontDrawGlyphWithSharedFlags(glyph, 1);
    frFontQueueGlyphInSelectedSlot(glyph);
}

extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);

extern f32 sdfSinPoly(f32);

/* Rotate Y/Z in place; preserve the repeated trig-call order and leave X/W untouched. */
void fldRotateVectorAroundX(f32 *vector, f32 angle) {
    f32 rotated[4];
    f32 cosine = sdfEvaluateCosineViaSinePhaseShift(angle);
    f32 sine = sdfSinPoly(angle);

    rotated[1] = vector[1] * cosine + vector[2] * sine;
    sine = sdfSinPoly(angle);
    cosine = sdfEvaluateCosineViaSinePhaseShift(angle);
    rotated[2] = vector[1] * -sine + vector[2] * cosine;
    vector[1] = rotated[1];
    vector[2] = rotated[2];
}

/* Rotate X/Z in place; preserve the repeated trig-call order and leave Y/W untouched. */
void fldRotateVectorAroundY(f32 *vector, f32 angle) {
    f32 rotated[4];
    f32 cosine = sdfEvaluateCosineViaSinePhaseShift(angle);
    f32 sine = sdfSinPoly(angle);

    rotated[0] = vector[0] * cosine - vector[2] * sine;
    sine = sdfSinPoly(angle);
    cosine = sdfEvaluateCosineViaSinePhaseShift(angle);
    rotated[2] = vector[0] * sine + vector[2] * cosine;
    vector[0] = rotated[0];
    vector[2] = rotated[2];
}

/* Rotate X/Y in place; preserve the repeated trig-call order and leave Z/W untouched. */
void fldRotateVectorAroundZ(f32 *vector, f32 angle) {
    f32 rotated[4];
    f32 cosine = sdfEvaluateCosineViaSinePhaseShift(angle);
    f32 sine = sdfSinPoly(angle);

    rotated[0] = vector[0] * cosine + vector[1] * sine;
    sine = sdfSinPoly(angle);
    cosine = sdfEvaluateCosineViaSinePhaseShift(angle);
    rotated[1] = vector[0] * -sine + vector[1] * cosine;
    vector[0] = rotated[0];
    vector[1] = rotated[1];
}

typedef struct Vector4 {
    float x;
    float y;
    float z;
    float w;
} Vector4;

/* Rotate XYZ about a normalized copy of axis; copy inputs before writing XYZ and leave W untouched. */
void fldRotateVectorAroundAxis(float *vector, float *axis, float angle) {
    Vector4 normalizedAxis;
    Vector4 scratch;
    Vector4 inputVector;
    float rotationMatrix[9];

    memset(&scratch, 0, sizeof(scratch));
    scratch.x = axis[0];
    scratch.y = axis[1];
    scratch.z = axis[2];
    normalizedAxis = scratch;
    memset(&inputVector, 0, sizeof(inputVector));
    inputVector.x = vector[0];
    inputVector.y = vector[1];
    inputVector.z = vector[2];
    scratch = inputVector;
    func_002C84F0(&normalizedAxis.x);
    rotationMatrix[0] = normalizedAxis.x * normalizedAxis.x * (1.0f - sdfEvaluateCosineViaSinePhaseShift(angle)) + sdfEvaluateCosineViaSinePhaseShift(angle);
    rotationMatrix[1] = normalizedAxis.x * normalizedAxis.y * (1.0f - sdfEvaluateCosineViaSinePhaseShift(angle)) - normalizedAxis.z * sdfSinPoly(angle);
    rotationMatrix[2] = normalizedAxis.x * normalizedAxis.z * (1.0f - sdfEvaluateCosineViaSinePhaseShift(angle)) + normalizedAxis.y * sdfSinPoly(angle);
    rotationMatrix[3] = normalizedAxis.y * normalizedAxis.x * (1.0f - sdfEvaluateCosineViaSinePhaseShift(angle)) + normalizedAxis.z * sdfSinPoly(angle);
    rotationMatrix[4] = normalizedAxis.y * normalizedAxis.y * (1.0f - sdfEvaluateCosineViaSinePhaseShift(angle)) + sdfEvaluateCosineViaSinePhaseShift(angle);
    rotationMatrix[5] = normalizedAxis.y * normalizedAxis.z * (1.0f - sdfEvaluateCosineViaSinePhaseShift(angle)) - normalizedAxis.x * sdfSinPoly(angle);
    rotationMatrix[6] = normalizedAxis.z * normalizedAxis.x * (1.0f - sdfEvaluateCosineViaSinePhaseShift(angle)) - normalizedAxis.y * sdfSinPoly(angle);
    rotationMatrix[7] = normalizedAxis.z * normalizedAxis.y * (1.0f - sdfEvaluateCosineViaSinePhaseShift(angle)) + normalizedAxis.x * sdfSinPoly(angle);
    rotationMatrix[8] = normalizedAxis.z * normalizedAxis.z * (1.0f - sdfEvaluateCosineViaSinePhaseShift(angle)) + sdfEvaluateCosineViaSinePhaseShift(angle);
    vector[0] = scratch.x * rotationMatrix[0] + scratch.y * rotationMatrix[3] + scratch.z * rotationMatrix[6];
    vector[1] = scratch.x * rotationMatrix[1] + scratch.y * rotationMatrix[4] + scratch.z * rotationMatrix[7];
    vector[2] = scratch.x * rotationMatrix[2] + scratch.y * rotationMatrix[5] + scratch.z * rotationMatrix[8];
}

void sdfVec3AddInPlace(float *dst, float *src) {
    *dst = *dst + *src;
    dst[1] = dst[1] + src[1];
    dst[2] = dst[2] + src[2];
}

void sdfVec3SubtractInPlace(float *dst, float *src) {
    *dst = *dst - *src;
    dst[1] = dst[1] - src[1];
    dst[2] = dst[2] - src[2];
}

void sdfVec3AddComponents(float x, float y, float z, float *dst) {
    *dst = *dst + x;
    dst[1] = dst[1] + y;
    dst[2] = dst[2] + z;
}

void sdfSetVectorComponents(Vec3 *v, float x, float y, float z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

void sdfVec3ScaleInPlace(float scale, float *dst) {
    *dst = *dst * scale;
    dst[1] = dst[1] * scale;
    dst[2] = dst[2] * scale;
}

float func_002C84F0(float *vector) {
    float length = fldVectorLength(vector);

    vector[0] = vector[0] / length;
    vector[1] = vector[1] / length;
    vector[2] = vector[2] / length;
    return length;
}

float fldVectorLength(float *vector) {
    return fsqrtf(vector[0] * vector[0] + vector[1] * vector[1] +
                  vector[2] * vector[2]);
}

/* Normalize copies of the input vectors; callers' vectors stay untouched. */
float fldNormalizedVectorDot(float *left, float *right) {
    Vector4 normalizedLeft;
    Vector4 normalizedRight;

    normalizedLeft = *(Vector4 *)left;
    normalizedRight = *(Vector4 *)right;
    func_002C84F0(&normalizedLeft.x);
    func_002C84F0(&normalizedRight.x);
    return normalizedLeft.x * normalizedRight.x + normalizedLeft.y * normalizedRight.y + normalizedLeft.z * normalizedRight.z;
}

float fldVec3AngleBetween(float *left, float *right) {
    return func_002FA1C0(fldNormalizedVectorDot(left, right));
}

/* Cross the normalized input copies; the result itself is not normalized. */
void fldNormalizedVectorCross(float *output, float *left, float *right) {
    Vector4 normalizedLeft;
    Vector4 normalizedRight;

    normalizedLeft = *(Vector4 *)left;
    normalizedRight = *(Vector4 *)right;
    func_002C84F0(&normalizedLeft.x);
    func_002C84F0(&normalizedRight.x);
    output[0] = normalizedLeft.y * normalizedRight.z - normalizedLeft.z * normalizedRight.y;
    output[1] = normalizedLeft.z * normalizedRight.x - normalizedLeft.x * normalizedRight.z;
    output[2] = normalizedLeft.x * normalizedRight.y - normalizedLeft.y * normalizedRight.x;
}

typedef struct SdfMat4 {
    f32 m[16];
} SdfMat4;

/* Rotate rows 1 and 2 of the matrix about the X axis by `angle`. */
void sdfRotateMatrixBasisAboutX(SdfMat4 *matrix, f32 angle) {
    SdfMat4 rotated;

    rotated.m[0] = matrix->m[0];
    rotated.m[1] = matrix->m[1];
    rotated.m[2] = matrix->m[2];
    rotated.m[3] = matrix->m[3];
    rotated.m[4] = matrix->m[4] * sdfEvaluateCosineViaSinePhaseShift(angle) + matrix->m[8] * sdfSinPoly(angle);
    rotated.m[5] = matrix->m[5] * sdfEvaluateCosineViaSinePhaseShift(angle) + matrix->m[9] * sdfSinPoly(angle);
    rotated.m[6] = matrix->m[6] * sdfEvaluateCosineViaSinePhaseShift(angle) + matrix->m[10] * sdfSinPoly(angle);
    rotated.m[7] = matrix->m[7] * sdfEvaluateCosineViaSinePhaseShift(angle) + matrix->m[11] * sdfSinPoly(angle);
    rotated.m[8] = matrix->m[4] * -sdfSinPoly(angle) + matrix->m[8] * sdfEvaluateCosineViaSinePhaseShift(angle);
    rotated.m[9] = matrix->m[5] * -sdfSinPoly(angle) + matrix->m[9] * sdfEvaluateCosineViaSinePhaseShift(angle);
    rotated.m[10] = matrix->m[6] * -sdfSinPoly(angle) + matrix->m[10] * sdfEvaluateCosineViaSinePhaseShift(angle);
    rotated.m[11] = matrix->m[7] * -sdfSinPoly(angle) + matrix->m[11] * sdfEvaluateCosineViaSinePhaseShift(angle);
    rotated.m[12] = matrix->m[12];
    rotated.m[13] = matrix->m[13];
    rotated.m[14] = matrix->m[14];
    rotated.m[15] = matrix->m[15];
    *matrix = rotated;
}

/* Rotate rows 0 and 2 of the matrix about the Y axis by `angle`. */
void sdfRotateMatrixBasisAboutY(SdfMat4 *matrix, f32 angle) {
    SdfMat4 rotated;

    rotated.m[0] = matrix->m[0] * sdfEvaluateCosineViaSinePhaseShift(angle) + matrix->m[8] * -sdfSinPoly(angle);
    rotated.m[1] = matrix->m[1] * sdfEvaluateCosineViaSinePhaseShift(angle) + matrix->m[9] * -sdfSinPoly(angle);
    rotated.m[2] = matrix->m[2] * sdfEvaluateCosineViaSinePhaseShift(angle) + matrix->m[10] * -sdfSinPoly(angle);
    rotated.m[3] = matrix->m[3] * sdfEvaluateCosineViaSinePhaseShift(angle) + matrix->m[11] * -sdfSinPoly(angle);
    rotated.m[4] = matrix->m[4];
    rotated.m[5] = matrix->m[5];
    rotated.m[6] = matrix->m[6];
    rotated.m[7] = matrix->m[7];
    rotated.m[8] = matrix->m[0] * sdfSinPoly(angle) + matrix->m[8] * sdfEvaluateCosineViaSinePhaseShift(angle);
    rotated.m[9] = matrix->m[1] * sdfSinPoly(angle) + matrix->m[9] * sdfEvaluateCosineViaSinePhaseShift(angle);
    rotated.m[10] = matrix->m[2] * sdfSinPoly(angle) + matrix->m[10] * sdfEvaluateCosineViaSinePhaseShift(angle);
    rotated.m[11] = matrix->m[3] * sdfSinPoly(angle) + matrix->m[11] * sdfEvaluateCosineViaSinePhaseShift(angle);
    rotated.m[12] = matrix->m[12];
    rotated.m[13] = matrix->m[13];
    rotated.m[14] = matrix->m[14];
    rotated.m[15] = matrix->m[15];
    *matrix = rotated;
}

/* Rotate rows 0 and 1 of the matrix about the Z axis by `angle`. */
void sdfRotateMatrixBasisAboutZ(SdfMat4 *matrix, f32 angle) {
    SdfMat4 rotated;

    rotated.m[0] = matrix->m[0] * sdfEvaluateCosineViaSinePhaseShift(angle) + matrix->m[4] * sdfSinPoly(angle);
    rotated.m[1] = matrix->m[1] * sdfEvaluateCosineViaSinePhaseShift(angle) + matrix->m[5] * sdfSinPoly(angle);
    rotated.m[2] = matrix->m[2] * sdfEvaluateCosineViaSinePhaseShift(angle) + matrix->m[6] * sdfSinPoly(angle);
    rotated.m[3] = matrix->m[3] * sdfEvaluateCosineViaSinePhaseShift(angle) + matrix->m[7] * sdfSinPoly(angle);
    rotated.m[4] = matrix->m[0] * -sdfSinPoly(angle) + matrix->m[4] * sdfEvaluateCosineViaSinePhaseShift(angle);
    rotated.m[5] = matrix->m[1] * -sdfSinPoly(angle) + matrix->m[5] * sdfEvaluateCosineViaSinePhaseShift(angle);
    rotated.m[6] = matrix->m[2] * -sdfSinPoly(angle) + matrix->m[6] * sdfEvaluateCosineViaSinePhaseShift(angle);
    rotated.m[7] = matrix->m[3] * -sdfSinPoly(angle) + matrix->m[7] * sdfEvaluateCosineViaSinePhaseShift(angle);
    rotated.m[8] = matrix->m[8];
    rotated.m[9] = matrix->m[9];
    rotated.m[10] = matrix->m[10];
    rotated.m[11] = matrix->m[11];
    rotated.m[12] = matrix->m[12];
    rotated.m[13] = matrix->m[13];
    rotated.m[14] = matrix->m[14];
    rotated.m[15] = matrix->m[15];
    *matrix = rotated;
}

/* Transpose through a local copy so source and destination may alias. */
void sdfTransposeMatrix(SdfMat4 *destination, SdfMat4 *source) {
    SdfMat4 sourceCopy = *source;

    destination->m[0] = sourceCopy.m[0];
    destination->m[1] = sourceCopy.m[4];
    destination->m[2] = sourceCopy.m[8];
    destination->m[3] = sourceCopy.m[12];
    destination->m[4] = sourceCopy.m[1];
    destination->m[5] = sourceCopy.m[5];
    destination->m[6] = sourceCopy.m[9];
    destination->m[7] = sourceCopy.m[13];
    destination->m[8] = sourceCopy.m[2];
    destination->m[9] = sourceCopy.m[6];
    destination->m[10] = sourceCopy.m[10];
    destination->m[11] = sourceCopy.m[14];
    destination->m[12] = sourceCopy.m[3];
    destination->m[13] = sourceCopy.m[7];
    destination->m[14] = sourceCopy.m[11];
    destination->m[15] = sourceCopy.m[15];
}

extern void *memcpy(void *, const void *, u32);

/* Transform XYZ without translation. Matching quirk: the fourth copied output word is uninitialized. */
f32 *sdfTransformDirectionByMatrix(f32 *direction, f32 *matrix) {
    f32 transformed[4];
    f32 x = direction[0];
    f32 y = direction[1];
    f32 z = direction[2];

    transformed[0] = x * matrix[0] + y * matrix[4] + z * matrix[8];
    transformed[1] = x * matrix[1] + y * matrix[5] + z * matrix[9];
    transformed[2] = x * matrix[2] + y * matrix[6] + z * matrix[10];
    memcpy(direction, transformed, 16);
    return direction;
}

INCLUDE_ASM(const s32, "game/code_002C7EC8", func_002C8FE8);

/* Compare integer steps as floats; exponents below one leave the result at 1.0f. */
float sdfPowFloatByTruncatedExponent(float base, float exponent) {
    float power = 1.0f;
    s32 step = 1;

    if (exponent >= 1.0f) {
        do {
            step++;
            power *= base;
        } while ((float)step <= exponent);
    }
    return power;
}

void func_002C9268(f32 quaternion[4], f32 matrix[4][4]) {
    f32 trace;
    f32 scale;
    s32 i;
    u8 j;
    u8 k;
    f32 xx = matrix[0][0];
    f32 yy = matrix[1][1];
    f32 zz = matrix[2][2];

    trace = xx + yy + zz + 1.0f;
    if (trace >= 1.0f) {
        scale = fsqrtf(trace) * 2.0f;
        quaternion[3] = scale * 0.25f;
        quaternion[0] = (matrix[1][2] - matrix[2][1]) / scale;
        quaternion[1] = (matrix[2][0] - matrix[0][2]) / scale;
        quaternion[2] = (matrix[0][1] - matrix[1][0]) / scale;
    } else {
        i = xx > yy ? 0 : 1;
        if (zz > matrix[i][i]) {
            i = 2;
        }
        j = (i + 1) % 3;
        k = (j + 1) % 3;
        scale = fsqrtf(matrix[i][i] - matrix[j][j] - matrix[k][k] + 1.0f) * 2.0f;
        if (scale != 0.0f) {
            quaternion[i] = scale * 0.25f;
            quaternion[j] = (matrix[i][j] + matrix[j][i]) / scale;
            quaternion[k] = (matrix[i][k] + matrix[k][i]) / scale;
            quaternion[3] = (matrix[j][k] - matrix[k][j]) / scale;
        } else {
            quaternion[i] = 1.0f;
            quaternion[j] = 0.0f;
            quaternion[k] = 0.0f;
            quaternion[3] = 0.0f;
        }
    }
}

typedef struct QuatF {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} QuatF;

typedef struct Mat4F {
    f32 m[16];
} Mat4F;

/* Convert a unit quaternion into a 4x4 rotation matrix (translation zero). */
void sdfConvertQuaternionRotationMatrix(Mat4F *out, QuatF *q) {
    f32 xx = q->x * q->x;
    f32 yy = q->y * q->y;
    f32 zz = q->z * q->z;
    f32 a = 1.0f - (yy + zz) * 2.0f;
    f32 b = 1.0f - (xx + zz) * 2.0f;
    f32 c = 1.0f - (xx + yy) * 2.0f;

    out->m[0] = a;
    out->m[1] = (q->x * q->y - q->w * q->z) * 2.0f;
    out->m[2] = (q->w * q->y + q->x * q->z) * 2.0f;
    out->m[3] = 0;
    out->m[4] = (q->x * q->y + q->w * q->z) * 2.0f;
    out->m[5] = b;
    out->m[6] = (q->y * q->z - q->w * q->x) * 2.0f;
    out->m[7] = 0;
    out->m[8] = (q->x * q->z - q->w * q->y) * 2.0f;
    out->m[9] = (q->y * q->z + q->w * q->x) * 2.0f;
    out->m[10] = c;
    out->m[11] = 0;
    out->m[12] = 0;
    out->m[13] = 0;
    out->m[14] = 0;
    out->m[15] = 1.0f;
}

void sdfVec4Add(float *dst, float *lhs, float *rhs) {
    *dst = *lhs + *rhs;
    dst[1] = lhs[1] + rhs[1];
    dst[2] = lhs[2] + rhs[2];
    dst[3] = lhs[3] + rhs[3];
}

/* Hamilton product in XYZW order; output must not alias either input. */
void sdfQuatMultiply(float *output, float *left, float *right) {
    *output = (left[3] * *right + *left * right[3] + left[1] * right[2]) -
                          left[2] * right[1];
    output[1] = (left[3] * right[1] + left[1] * right[3] + left[2] * *right) -
                              *left * right[2];
    output[2] = (left[3] * right[2] + left[2] * right[3] + *left * right[1]) -
                              left[1] * *right;
    output[3] = ((left[3] * right[3] - *left * *right) - left[1] * right[1]) -
                              left[2] * right[2];
}

float sdfQuatDot(float *lhs, float *rhs) {
    return *lhs * *rhs + lhs[1] * rhs[1] + lhs[2] * rhs[2] +
                  lhs[3] * rhs[3];
}

/* Sum the three components of the cross product. */
float sdfSumCrossProductComponents(float *left, float *right) {
    return (left[1] * right[2] - left[2] * right[1]) +
                  (left[2] * *right - *left * right[2]) +
                  (*left * right[1] - left[1] * *right);
}

/* Pass the four-component dot directly to the angle evaluator, without input normalization. */
float fldVec4ArcCosDot(float *left, float *right) {
    return func_002FA1C0(sdfQuatDot(left, right));
}
