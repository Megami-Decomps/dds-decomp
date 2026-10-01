#include "common.h"

#include "fpu.h"

extern u64 func_0019F460(s32, s32, u64, u64, u64, u64);

extern u32 D_004390A4;

extern u32 D_0045C7A0[];

extern u32 D_0045C7B0[];

s32 sdfQueueNonzeroResourceId(u32 sprite);

typedef struct MapResource {
    u32 image;
    u32 handle;
    u32 descriptor;
    u32 unkC;
} MapResource;

extern u32 fldReleaseMapResource(s32 *);

extern f32 sdfQuatDot(f32 *, f32 *);

extern f32 func_003532B8(f32);

extern void sdfQuatMultiply(f32 *, f32 *, f32 *);

extern f32 fldNormalizedVectorDot(f32 *, f32 *);

extern u32 func_00343ED0(const char *, void *, s32);

extern u32 func_0032C138(u32);

struct SdfRing;
extern struct SdfRing *func_0030EE40(s16, s16);

extern void fldSetMapRequestInterval(s32, u16);

extern void func_0030E940(void);

extern void func_0030E958(void);

extern s32 D_004390AC;

extern s32 D_004390B0;

extern u32 D_004390A8;

typedef struct SdfRingNode {
    u8 pad00[0xC];
    s32 f0C;                        /* 0x0C */
    struct SdfRingNode *next;       /* 0x10 */
    struct SdfRingNode *prev;       /* 0x14 */
    u8 pad18[8];
} SdfRingNode;

typedef struct SdfRing {
    s32 allocation;                 /* 0x00 */
    SdfRingNode *head;              /* 0x04 */
    SdfRingNode *cursor;            /* 0x08 */
    SdfRingNode *last;              /* 0x0C */
    s16 count;                      /* 0x10 */
    s16 limit;                      /* 0x12 */
    s16 pad14;                      /* 0x14 */
} SdfRing;

/* The ring of nodes lives inside the same block, 0x2C past the header. */
typedef struct SdfRingBlock {
    SdfRing header;
    u8 pad18[0x2C];
    SdfRingNode nodes[1]; /* 0x44 */
} SdfRingBlock;

extern s32 func_003292A8(s32);

extern void *sdfMemoryGetBlockAddress(u32);

extern void *memset(void *, s32, u32);

typedef struct SdfMat4 {
    f32 m[16];
} SdfMat4;
/* The map-request queue stores its cursor at +8 and two halfword timers at +0x14. */
typedef struct MapRequestQueue {
    u8 pad00[8];
    u32 *cursor;   /* 0x08: five-word request entry */
    u8 pad0C[8];
    s16 interval;  /* 0x14 */
    s16 elapsed;   /* 0x16 */
    s32 callback;  /* 0x18: handler installed after queue creation */
} MapRequestQueue;


INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030E1A0);

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030E390);

void func_0030E878(void) {
}

void func_0030E880(void) {
    s32 handler;
    handler = (s32)func_0030EE40(0x14, 0xC);
    D_004390AC = handler;
    ((MapRequestQueue *)D_004390AC)->callback = (s32)func_0030E940;
    fldSetMapRequestInterval(handler, 0);
    D_004390B0 = (s32)func_0030EE40(0x14, 0x18);
    ((MapRequestQueue *)D_004390B0)->callback = (s32)func_0030E958;
    D_004390A8 = 5;
    D_004390A4 = 0;
}

extern s32 sdfDrawUniformlyScaledSlotImage(s32, s32, s32, s32, s32, s32, s32);

s64 fldReleaseMapRequestQueues(void) {
    func_0030EF18(D_004390AC);
    return func_0030EF18(D_004390B0);
}

s64 func_0030E910(s32 map, s32 request, s32 value) {
    return sdfDrawUniformlyScaledSlotImage(map, request, 0, value, 0x20, 0, 0x54);
}

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030E940);

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030E958);

void sdfCommitPendingVectorAndMarkChanged(void) {
    D_0045C7B0[0] = D_0045C7A0[0];
    D_0045C7B0[1] = D_0045C7A0[1];
    D_0045C7B0[2] = D_0045C7A0[2];
    D_004390A4 = 1;
}

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030EAA8);

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030ECC0);

/* Build a ring of `count` request nodes (0x20 bytes each) behind a 0x44-byte queue header. */
SdfRing *func_0030EE40(s16 count, s16 limit) {
    s32 size = count * 0x20 + 0x44;
    s32 allocation = func_003292A8(size);
    SdfRingBlock *block = (SdfRingBlock *)sdfMemoryGetBlockAddress(allocation);
    SdfRing *ring = &block->header;
    SdfRingNode *node;
    SdfRingNode *next;
    SdfRingNode *first;
    s32 n;

    memset(ring, 0, size);
    ring->allocation = allocation;
    node = block->nodes;
    ring->head = node;
    ring->last = node;
    ring->cursor = node;
    for (n = count - 2; n != -1; n--) {
        next = node + 1;
        node->next = next;
        next->prev = node;
        node = node->next;
    }
    first = ring->head;
    node->next = first;
    first->prev = node;
    ring->limit = limit;
    ring->count = count;
    ring->pad14 = 0;
    return ring;
}

s64 func_0030EF18(u32 *sprite) {
    if (sprite != NULL) {
        return sdfQueueNonzeroResourceId(*sprite);
    }
}

void fldAdvanceMapRequest(s32 queue, u32 first, u32 second, u32 third) {
    u32 *entry;

    entry = ((MapRequestQueue *)queue)->cursor;
    if (((MapRequestQueue *)queue)->elapsed == ((MapRequestQueue *)queue)->interval) {
        if (entry[3] == 0) {
            *entry = first;
            entry[1] = second;
            entry[2] = third;
            ((MapRequestQueue *)queue)->cursor = (u32 *)entry[4];
            entry[3] = 1;
        }
        ((MapRequestQueue *)queue)->elapsed = 0;
        return;
    }
    ((MapRequestQueue *)queue)->elapsed = ((MapRequestQueue *)queue)->elapsed + 1;
}

void fldSetMapRequestInterval(s32 queue, u16 interval) {
    ((MapRequestQueue *)queue)->interval = interval;
}

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030EF90);

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030F038);

s32 fldLoadMapResource(const char *name, MapResource *record) {
    u32 handle = func_00343ED0(name, &record->descriptor, 0);
    u32 descriptor = record->descriptor;
    record->handle = handle;
    record->image = func_0032C138(descriptor);
    if (record->handle != 0) {
        sdfQueueNonzeroResourceId((void *)record->handle);
        record->handle = 0;
        record->descriptor = 0;
    }
    return 1;
}

u32 fldReleaseMapResource(s32 *image) {
    if (*image != 0) {
        sdfTexReleaseReferenceViaHandler(*image);
        *image = 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030F1A0);

void sdfCounterDrawGlyphAtGridCell(s32 arg0, s32 arg1, u64 arg2, u64 arg3) {
    u64 handle;

    handle = func_0019F460(arg0 << 4, arg1 << 3, 0, arg2, arg3, 0);
    frFontDrawGlyphWithSharedFlags(handle, 1);
    frFontQueueGlyphInSelectedSlot(handle);
}

extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);
extern f32 sdfSinPoly(f32);

/* Rotate the y/z components of `v` by `angle` (about the x axis). */
void fldRotateVectorAroundX(f32 *v, f32 angle) {
    f32 r[4];
    f32 c = sdfEvaluateCosineViaSinePhaseShift(angle);
    f32 s = sdfSinPoly(angle);

    r[1] = v[1] * c + v[2] * s;
    s = sdfSinPoly(angle);
    c = sdfEvaluateCosineViaSinePhaseShift(angle);
    r[2] = v[1] * -s + v[2] * c;
    v[1] = r[1];
    v[2] = r[2];
}

/* Rotate the x/z components of `v` by `angle` (about the y axis). */
void fldRotateVectorAroundY(f32 *v, f32 angle) {
    f32 r[4];
    f32 c = sdfEvaluateCosineViaSinePhaseShift(angle);
    f32 s = sdfSinPoly(angle);

    r[0] = v[0] * c - v[2] * s;
    s = sdfSinPoly(angle);
    c = sdfEvaluateCosineViaSinePhaseShift(angle);
    r[2] = v[0] * s + v[2] * c;
    v[0] = r[0];
    v[2] = r[2];
}

/* Rotate the x/y components of `v` by `angle` (about the z axis). */
void fldRotateVectorAroundZ(f32 *v, f32 angle) {
    f32 r[4];
    f32 c = sdfEvaluateCosineViaSinePhaseShift(angle);
    f32 s = sdfSinPoly(angle);

    r[0] = v[0] * c + v[1] * s;
    s = sdfSinPoly(angle);
    c = sdfEvaluateCosineViaSinePhaseShift(angle);
    r[1] = v[0] * -s + v[1] * c;
    v[0] = r[0];
    v[1] = r[1];
}

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030F4B8);

void sdfVec3AddInPlace(float *vector, float *delta) {
    *vector = *vector + *delta;
    vector[1] = vector[1] + delta[1];
    vector[2] = vector[2] + delta[2];
}

void sdfVec3SubtractInPlace(float *vector, float *delta) {
    *vector = *vector - *delta;
    vector[1] = vector[1] - delta[1];
    vector[2] = vector[2] - delta[2];
}

void sdfVec3AddComponents(float x, float y, float z, float *vector) {
    *vector = *vector + x;
    vector[1] = vector[1] + y;
    vector[2] = vector[2] + z;
}

void func_0030F898(f32 x, f32 y, f32 z, f32 *out) {
    out[0] = x;
    out[1] = y;
    out[2] = z;
}

void sdfVec3ScaleInPlace(float factor, float *vector) {
    *vector = *vector * factor;
    vector[1] = vector[1] * factor;
    vector[2] = vector[2] * factor;
}

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030F8D0);

typedef struct Vector4 {
    float x, y, z, w;
} Vector4;

float fldVectorLength(float *v) {
    return fsqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

float fldNormalizedVectorDot(float *left, float *right) {
    Vector4 a, b;
    a = *(Vector4 *)left;
    b = *(Vector4 *)right;
    func_0030F8D0(&a.x);
    func_0030F8D0(&b.x);
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float fldVec3AngleBetween(a, b)
float *a;
float *b;
{
    return func_003532B8(fldNormalizedVectorDot(a, b));
}

void func_0030FA28(float *out, float *left, float *right) {
    Vector4 a, b;
    a = *(Vector4 *)left;
    b = *(Vector4 *)right;
    func_0030F8D0(&a.x);
    func_0030F8D0(&b.x);
    out[0] = a.y * b.z - a.z * b.y;
    out[1] = a.z * b.x - a.x * b.z;
    out[2] = a.x * b.y - a.y * b.x;
}

/* Rotate rows 1 and 2 of the matrix about the X axis by `angle`. */
void func_0030FAF0(SdfMat4 *mat, f32 angle) {
    SdfMat4 r;

    r.m[0] = mat->m[0];
    r.m[1] = mat->m[1];
    r.m[2] = mat->m[2];
    r.m[3] = mat->m[3];
    r.m[4] = mat->m[4] * sdfEvaluateCosineViaSinePhaseShift(angle) + mat->m[8] * sdfSinPoly(angle);
    r.m[5] = mat->m[5] * sdfEvaluateCosineViaSinePhaseShift(angle) + mat->m[9] * sdfSinPoly(angle);
    r.m[6] = mat->m[6] * sdfEvaluateCosineViaSinePhaseShift(angle) + mat->m[10] * sdfSinPoly(angle);
    r.m[7] = mat->m[7] * sdfEvaluateCosineViaSinePhaseShift(angle) + mat->m[11] * sdfSinPoly(angle);
    r.m[8] = mat->m[4] * -sdfSinPoly(angle) + mat->m[8] * sdfEvaluateCosineViaSinePhaseShift(angle);
    r.m[9] = mat->m[5] * -sdfSinPoly(angle) + mat->m[9] * sdfEvaluateCosineViaSinePhaseShift(angle);
    r.m[10] = mat->m[6] * -sdfSinPoly(angle) + mat->m[10] * sdfEvaluateCosineViaSinePhaseShift(angle);
    r.m[11] = mat->m[7] * -sdfSinPoly(angle) + mat->m[11] * sdfEvaluateCosineViaSinePhaseShift(angle);
    r.m[12] = mat->m[12];
    r.m[13] = mat->m[13];
    r.m[14] = mat->m[14];
    r.m[15] = mat->m[15];
    *mat = r;
}

/* Rotate rows 0 and 2 of the matrix about the Y axis by `angle`. */
void func_0030FD50(SdfMat4 *mat, f32 angle) {
    SdfMat4 r;

    r.m[0] = mat->m[0] * sdfEvaluateCosineViaSinePhaseShift(angle) + mat->m[8] * -sdfSinPoly(angle);
    r.m[1] = mat->m[1] * sdfEvaluateCosineViaSinePhaseShift(angle) + mat->m[9] * -sdfSinPoly(angle);
    r.m[2] = mat->m[2] * sdfEvaluateCosineViaSinePhaseShift(angle) + mat->m[10] * -sdfSinPoly(angle);
    r.m[3] = mat->m[3] * sdfEvaluateCosineViaSinePhaseShift(angle) + mat->m[11] * -sdfSinPoly(angle);
    r.m[4] = mat->m[4];
    r.m[5] = mat->m[5];
    r.m[6] = mat->m[6];
    r.m[7] = mat->m[7];
    r.m[8] = mat->m[0] * sdfSinPoly(angle) + mat->m[8] * sdfEvaluateCosineViaSinePhaseShift(angle);
    r.m[9] = mat->m[1] * sdfSinPoly(angle) + mat->m[9] * sdfEvaluateCosineViaSinePhaseShift(angle);
    r.m[10] = mat->m[2] * sdfSinPoly(angle) + mat->m[10] * sdfEvaluateCosineViaSinePhaseShift(angle);
    r.m[11] = mat->m[3] * sdfSinPoly(angle) + mat->m[11] * sdfEvaluateCosineViaSinePhaseShift(angle);
    r.m[12] = mat->m[12];
    r.m[13] = mat->m[13];
    r.m[14] = mat->m[14];
    r.m[15] = mat->m[15];
    *mat = r;
}

/* Rotate rows 0 and 1 of the matrix about the Z axis by `angle`. */
void func_0030FFB0(SdfMat4 *mat, f32 angle) {
    SdfMat4 r;

    r.m[0] = mat->m[0] * sdfEvaluateCosineViaSinePhaseShift(angle) + mat->m[4] * sdfSinPoly(angle);
    r.m[1] = mat->m[1] * sdfEvaluateCosineViaSinePhaseShift(angle) + mat->m[5] * sdfSinPoly(angle);
    r.m[2] = mat->m[2] * sdfEvaluateCosineViaSinePhaseShift(angle) + mat->m[6] * sdfSinPoly(angle);
    r.m[3] = mat->m[3] * sdfEvaluateCosineViaSinePhaseShift(angle) + mat->m[7] * sdfSinPoly(angle);
    r.m[4] = mat->m[0] * -sdfSinPoly(angle) + mat->m[4] * sdfEvaluateCosineViaSinePhaseShift(angle);
    r.m[5] = mat->m[1] * -sdfSinPoly(angle) + mat->m[5] * sdfEvaluateCosineViaSinePhaseShift(angle);
    r.m[6] = mat->m[2] * -sdfSinPoly(angle) + mat->m[6] * sdfEvaluateCosineViaSinePhaseShift(angle);
    r.m[7] = mat->m[3] * -sdfSinPoly(angle) + mat->m[7] * sdfEvaluateCosineViaSinePhaseShift(angle);
    r.m[8] = mat->m[8];
    r.m[9] = mat->m[9];
    r.m[10] = mat->m[10];
    r.m[11] = mat->m[11];
    r.m[12] = mat->m[12];
    r.m[13] = mat->m[13];
    r.m[14] = mat->m[14];
    r.m[15] = mat->m[15];
    *mat = r;
}

/* Transpose through a local copy so source and destination may alias. */
void sdfTransposeMatrix(SdfMat4 *dst, SdfMat4 *src) {
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

f32 *sdfTransformDirectionByMatrix(f32 *vec, f32 *mat) {
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

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_003103C8);

float sdfPowFloatByTruncatedExponent(float x, float y) {
    float p = 1.0f;
    s32 i = 1;

    if (y >= 1.0f) {
        do {
            i++;
            p *= x;
        } while ((float)i <= y);
    }
    return p;
}

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_00310648);

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
void func_00310888(Mat4F *out, QuatF *q) {
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

void sdfVec4Add(float *out, float *left, float *right) {
    *out = *left + *right;
    out[1] = left[1] + right[1];
    out[2] = left[2] + right[2];
    out[3] = left[3] + right[3];
}

void sdfQuatMultiply(float *out, float *left, float *right) {
    *out = (left[3] * *right + *left * right[3] + left[1] * right[2]) -
                          left[2] * right[1];
    out[1] = (left[3] * right[1] + left[1] * right[3] + left[2] * *right) -
                              *left * right[2];
    out[2] = (left[3] * right[2] + left[2] * right[3] + *left * right[1]) -
                              left[1] * *right;
    out[3] = ((left[3] * right[3] - *left * *right) - left[1] * right[1]) -
                              left[2] * right[2];
}

float sdfQuatDot(float *left, float *right) {
    return *left * *right + left[1] * right[1] + left[2] * right[2] +
                  left[3] * right[3];
}

/* Sum the three components of the cross product. */
float sdfSumCrossProductComponents(float *left, float *right) {
    return (left[1] * right[2] - left[2] * right[1]) +
                  (left[2] * *right - *left * right[2]) +
                  (*left * right[1] - left[1] * *right);
}

float fldVec4ArcCosDot(float *a, float *b) {
    return func_003532B8(sdfQuatDot(a, b));
}
