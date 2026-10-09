#include "common.h"
#include "dds_nested_resource.h"
#include "mnu_callback_list.h"
#include "sdf_resource.h"

typedef struct SdfMat4 {
    f32 m[16];
} SdfMat4;

typedef struct SdfVec4 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} SdfVec4;

extern f32 sdfVec3Normalize(f32 *);

extern s32 sdfCreateThread(void *entryAddress, void *workspace, s32 stackBytes, s32 priority);

extern s32 CreateSema(void *);

extern void (*sdfTickCallback)(void);

extern f32 sdfVec3DotNormalized(void *, void *);

extern f32 func_003532B8(f32);

extern f64 cos(f64);

extern f64 sin(f64);

extern f64 func_003532A0(f64);

extern void *memcpy(void *, const void *, u32);

f32 sdfVec3AngleBetween(void *a, void *b) {
    return func_003532B8(sdfVec3DotNormalized(a, b));
}

void sdfCrossNormalizedVectors(float *out, float *left, float *right) {
    SdfVec4 a, b;

    a = *(SdfVec4 *)left;
    b = *(SdfVec4 *)right;
    sdfVec3Normalize(&a.x);
    sdfVec3Normalize(&b.x);
    out[0] = a.y * b.z - a.z * b.y;
    out[1] = a.z * b.x - a.x * b.z;
    out[2] = a.x * b.y - a.y * b.x;
}

void func_00326BC8(SdfMat4 *matrix, f32 angle) {
    SdfMat4 rotated;

    rotated.m[0] = matrix->m[0];
    rotated.m[1] = matrix->m[1];
    rotated.m[2] = matrix->m[2];
    rotated.m[3] = matrix->m[3];
    rotated.m[4] = matrix->m[4] * cos(angle) + matrix->m[8] * sin(angle);
    rotated.m[5] = matrix->m[5] * cos(angle) + matrix->m[9] * sin(angle);
    rotated.m[6] = matrix->m[6] * cos(angle) + matrix->m[10] * sin(angle);
    rotated.m[7] = matrix->m[7] * cos(angle) + matrix->m[11] * sin(angle);
    rotated.m[8] = matrix->m[4] * -sin(angle) + matrix->m[8] * cos(angle);
    rotated.m[9] = matrix->m[5] * -sin(angle) + matrix->m[9] * cos(angle);
    rotated.m[10] = matrix->m[6] * -sin(angle) + matrix->m[10] * cos(angle);
    rotated.m[11] = matrix->m[7] * -sin(angle) + matrix->m[11] * cos(angle);
    rotated.m[12] = matrix->m[12];
    rotated.m[13] = matrix->m[13];
    rotated.m[14] = matrix->m[14];
    rotated.m[15] = matrix->m[15];
    *matrix = rotated;
}

void func_003270C8(SdfMat4 *matrix, f32 angle) {
    SdfMat4 rotated;

    rotated.m[0] = matrix->m[0] * cos(angle) + matrix->m[8] * -sin(angle);
    rotated.m[1] = matrix->m[1] * cos(angle) + matrix->m[9] * -sin(angle);
    rotated.m[2] = matrix->m[2] * cos(angle) + matrix->m[10] * -sin(angle);
    rotated.m[3] = matrix->m[3] * cos(angle) + matrix->m[11] * -sin(angle);
    rotated.m[4] = matrix->m[4];
    rotated.m[5] = matrix->m[5];
    rotated.m[6] = matrix->m[6];
    rotated.m[7] = matrix->m[7];
    rotated.m[8] = matrix->m[0] * sin(angle) + matrix->m[8] * cos(angle);
    rotated.m[9] = matrix->m[1] * sin(angle) + matrix->m[9] * cos(angle);
    rotated.m[10] = matrix->m[2] * sin(angle) + matrix->m[10] * cos(angle);
    rotated.m[11] = matrix->m[3] * sin(angle) + matrix->m[11] * cos(angle);
    rotated.m[12] = matrix->m[12];
    rotated.m[13] = matrix->m[13];
    rotated.m[14] = matrix->m[14];
    rotated.m[15] = matrix->m[15];
    *matrix = rotated;
}

void func_003275C8(SdfMat4 *matrix, f32 angle) {
    SdfMat4 rotated;

    rotated.m[0] = matrix->m[0] * cos(angle) + matrix->m[4] * sin(angle);
    rotated.m[1] = matrix->m[1] * cos(angle) + matrix->m[5] * sin(angle);
    rotated.m[2] = matrix->m[2] * cos(angle) + matrix->m[6] * sin(angle);
    rotated.m[3] = matrix->m[3] * cos(angle) + matrix->m[7] * sin(angle);
    rotated.m[4] = matrix->m[0] * -sin(angle) + matrix->m[4] * cos(angle);
    rotated.m[5] = matrix->m[1] * -sin(angle) + matrix->m[5] * cos(angle);
    rotated.m[6] = matrix->m[2] * -sin(angle) + matrix->m[6] * cos(angle);
    rotated.m[7] = matrix->m[3] * -sin(angle) + matrix->m[7] * cos(angle);
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
void sdfMat4Transpose(SdfMat4 *dst, SdfMat4 *src) {
    SdfMat4 t = *src;

    dst->m[0] = t.m[0];
    dst->m[1] = t.m[4];
    dst->m[2] = t.m[8];
    dst->m[3] = t.m[12];
    dst->m[4] = t.m[1];
    dst->m[5] = t.m[5];
    dst->m[6] = t.m[9];
    dst->m[7] = t.m[13];
    dst->m[8] = t.m[2];
    dst->m[9] = t.m[6];
    dst->m[10] = t.m[10];
    dst->m[11] = t.m[14];
    dst->m[12] = t.m[3];
    dst->m[13] = t.m[7];
    dst->m[14] = t.m[11];
    dst->m[15] = t.m[15];
}

extern void *memcpy(void *, const void *, u32);

f32 *sdfVectorTransformByMatrix(f32 *vec, f32 *mat) {
    f32 out[4];
    f32 x = vec[0];
    f32 y = vec[1];
    f32 z = vec[2];

    out[0] = x * mat[0] + y * mat[4] + z * mat[8];
    out[1] = x * mat[1] + y * mat[5] + z * mat[9];
    out[2] = x * mat[2] + y * mat[6] + z * mat[10];
    memcpy(vec, out, 16);
    return vec;
}

/* Convert a rotation basis to quaternion components, choosing the largest
 * diagonal when the trace branch is ill-conditioned. */
void sdfMatrixToQuaternion(f32 *out, SdfMat4 *matrix) {
    SdfMat4 copy;
    f32 quaternion[4];
    f32 trace;
    f32 scale;
    s32 i;
    u8 j;
    u8 k;

    trace = matrix->m[0] + matrix->m[5] + matrix->m[10] + 1.0f;
    if (trace >= 1.0f) {
        scale = func_003532A0(trace) * 2.0;
        out[3] = scale * 0.25f;
        out[0] = (matrix->m[6] - matrix->m[9]) / scale;
        out[1] = (matrix->m[8] - matrix->m[2]) / scale;
        out[2] = (matrix->m[1] - matrix->m[4]) / scale;
    } else {
        copy = *matrix;
        i = copy.m[0] > copy.m[5] ? 0 : 1;
        if (copy.m[i * 4 + i] < copy.m[10]) {
            i = 2;
        }
        j = (i + 1) % 3;
        k = (j + 1) % 3;
        scale = func_003532A0(copy.m[i * 4 + i] - copy.m[j * 4 + j] -
                             copy.m[k * 4 + k] + 1.0f) * 2.0;
        if (scale != 0.0f) {
            quaternion[i] = scale * 0.25f;
            quaternion[j] = (copy.m[i * 4 + j] + copy.m[j * 4 + i]) / scale;
            quaternion[k] = (copy.m[i * 4 + k] + copy.m[k * 4 + i]) / scale;
            quaternion[3] = (copy.m[j * 4 + k] - copy.m[k * 4 + j]) / scale;
        } else {
            quaternion[i] = 1.0f;
            quaternion[j] = 0.0f;
            quaternion[k] = 0.0f;
            quaternion[3] = 0.0f;
        }
        memcpy(out, quaternion, sizeof(quaternion));
    }
}

void func_00328018(SdfMat4 *out, SdfVec4 *q) {
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

/* Quaternion from Euler angles using negated half angles. */
void func_00328160(f32 *out, f32 x, f32 y, f32 z) {
    f32 half;
    f32 cx;
    f32 sx;
    f32 cy;
    f32 sy;
    f32 cz;
    f32 sz;

    half = -x * 0.5f;
    cx = cos(half);
    sx = sin(half);
    half = -y * 0.5f;
    cy = cos(half);
    sy = sin(half);
    half = -z * 0.5f;
    cz = cos(half);
    sz = sin(half);
    out[0] = sz * sy * cx + cz * cy * sx;
    out[1] = cz * sy * cx - sz * cy * sx;
    out[2] = sz * cy * cx + cz * sy * sx;
    out[3] = cz * cy * cx - sz * sy * sx;
}

void sdfCreateSemaphoreFromOptions(void) {
}

s32 sdfCreateSemaphore(u32 initial, u32 option, u32 maximum) {
    struct {
        u32 attr;
        u32 option;
        u32 initial;
        u32 reserved[2];
        u32 maximum;
    } sema;

    sema.initial = initial;
    sema.option = option;
    sema.maximum = maximum;
    return CreateSema(&sema);
}

INCLUDE_ASM(const s32, "game/code_00326AE0", sdfCreateThread);

s32 sdfCreateThreadWithAllocatedWorkspace(void *entryAddress, s32 stackBytes, s32 priority) {
    void *workspace;

    workspace = sdfAllocateBlockBySizeThreshold(stackBytes);
    return sdfCreateThread(entryAddress, workspace, stackBytes, priority);
}

