#include "common.h"

#include "fpu.h"
#include "pcp_vu0.h"

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

extern f32 fldVectorLength(f32 *);

extern u32 sdfReadNamedResource(const char *, void *, s32);

extern u32 sdfTexAcquireResourceTexture(u32);

struct SdfRing;
extern struct SdfRing *sdfCreateLinkedRequestRing(s16, s16);

extern void fldSetMapRequestInterval(s32, u16);

extern void func_0030E940(void);

struct MapRequestQueue;
extern void func_0030E958(s32, s32, s32, struct MapRequestQueue *, s32, f32);

extern s32 D_004390AC;

extern s32 D_004390B0;

extern u32 D_004390A8;

typedef struct SdfRingNode {
    u32 value;                       /* 0x00 */
    u32 argument1;                   /* 0x04 */
    u32 argument2;                   /* 0x08 */
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
    s32 callback;                   /* 0x18 */
} SdfRing;

/* The ring of nodes lives inside the same block, 0x2C past the header. */
typedef struct SdfRingBlock {
    SdfRing header;
    u8 pad1C[0x28];
    SdfRingNode nodes[1]; /* 0x44 */
} SdfRingBlock;

typedef void (*MapRequestCallback)(u32, u32, u32, SdfRing *, SdfRingNode *, f32);

extern s32 sdfAllocGeneralBlock(s32);

extern void *sdfMemoryGetBlockAddress(u32);

extern void *memset(void *, s32, u32);

typedef struct SdfMat4 {
    f32 m[16];
} SdfMat4;
/* The map-request queue stores its cursor at +8 and two halfword timers at +0x14. */
typedef struct MapRequestQueue {
    u8 pad00[8];
    u32 *cursor;   /* 0x08: current request node */
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
    handler = (s32)sdfCreateLinkedRequestRing(0x14, 0xC);
    D_004390AC = handler;
    ((MapRequestQueue *)D_004390AC)->callback = (s32)func_0030E940;
    fldSetMapRequestInterval(handler, 0);
    D_004390B0 = (s32)sdfCreateLinkedRequestRing(0x14, 0x18);
    ((MapRequestQueue *)D_004390B0)->callback = (s32)func_0030E958;
    D_004390A8 = 5;
    D_004390A4 = 0;
}

extern s32 sdfDrawUniformlyScaledSlotImage(s32, s32, s32, s32, s32, s32, s32, f32);
extern void fldProjectPointToGridCell(s32 *, s32 *, f32, f32, f32);

INCLUDE_ASM(const s32, "game/code_0030E1A0", fldReleaseMapRequestQueues);

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030E910);

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030E940);

/* The current request pulses; other requests enlarge and fade in later. */
void func_0030E958(s32 x, s32 y, s32 z, MapRequestQueue *queue, s32 selected, f32 progress) {
    s32 gridX;
    s32 gridY;
    f32 scale = (1.0f - progress) * 3.0f + progress;

    if (queue->cursor[5] != selected) {
        progress -= 0.5f;
        if (progress < 0.0f) {
            progress = 0.0f;
        }
        scale *= 1.5f;
    } else if (progress >= 0.7f) {
        progress = (1.0f - progress) / 0.3f;
    } else {
        progress /= 0.7f;
    }
    fldProjectPointToGridCell(&gridX, &gridY, x, y, z);
    sdfDrawUniformlyScaledSlotImage(gridX, gridY, 0, (s32)(progress * 64.0f), 0x20, 0, 0x54, scale);
}

void sdfCommitPendingVectorAndMarkChanged(void) {
    D_0045C7B0[0] = D_0045C7A0[0];
    D_0045C7B0[1] = D_0045C7A0[1];
    D_0045C7B0[2] = D_0045C7A0[2];
    D_004390A4 = 1;
}

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030EAA8);

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030ECC0);

/* Build a ring of `count` request nodes (0x20 bytes each) behind a 0x44-byte queue header. */
SdfRing *sdfCreateLinkedRequestRing(s16 count, s16 limit) {
    s32 size = count * 0x20 + 0x44;
    s32 allocation = sdfAllocGeneralBlock(size);
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

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030EF18);

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

/* Advance active ring nodes, retiring each node when it reaches the queue limit. */
void func_0030EF90(SdfRing *ring) {
    s32 remaining;
    s32 pending;
    u16 limit;
    SdfRingNode *first;
    SdfRingNode *node;

    remaining = ring->count;
    first = ring->last;
    if (--remaining == -1) {
        goto done;
    }
    if (first->f0C == 0) {
        goto done;
    }
    first->f0C++;
    pending = first->f0C < ring->limit;
    limit = ring->limit;
    if (!pending) {
        node = first->next;
        first->f0C = 0;
        ring->last = node;
        goto loop;
    }
    node = first->next;
    goto loop;

advance:
    node = node->next;
loop:
    if (--remaining == -1) {
        goto done;
    }
    if (node->f0C == 0) {
        goto done;
    }
    node->f0C++;
    if (node->f0C < (s16)limit) {
        goto advance;
    }
    {
        SdfRingNode *nextHead = ring->last->next;
        node->f0C = 0;
        node = node->next;
        ring->last = nextHead;
    }
    goto loop;

done:
    return;
}

/* Report normalized progress for each active request in the ring. */
void func_0030F038(SdfRing *ring) {
    SdfRingNode *node;
    s32 remaining;
    s32 end;
    f32 one;

    node = ring->last;
    remaining = ring->count;
    end = -1;
    one = 1.0f;
    goto loop;

advance:
    node = node->next;
loop:
    remaining--;
    if (remaining == end) {
        goto done;
    }
    if (node->f0C == 0) {
        goto done;
    }
    {
        f32 progress;
        MapRequestCallback callback = (MapRequestCallback)ring->callback;

        progress = (f32)node->f0C / (f32)ring->limit;
        progress = one - progress;
        if (callback == NULL) {
            goto advance;
        }
        callback(node->value, node->argument1, node->argument2, ring, node, progress);
    }
    node = node->next;
    goto loop;

done:
    return;
}

s32 fldLoadMapResource(const char *name, MapResource *record) {
    u32 handle = sdfReadNamedResource(name, &record->descriptor, 0);
    u32 descriptor = record->descriptor;
    record->handle = handle;
    record->image = sdfTexAcquireResourceTexture(descriptor);
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

extern u8 sdfViewMatrix[];
extern u8 sdfProjectionMatrix[];
extern u8 D_0037F650[];
extern u8 D_0037F660[];
extern void sdfPostmultiplyVuMatrixFromMemory(void *src);

/* vu0 routine: project a world point to the screen and return its GS grid cell. */
void fldProjectPointToGridCell(s32 *gridX, s32 *gridY, f32 x, f32 y, f32 z) {
    f32 point[4];
    f32 screen[4];

    point[0] = x;
    point[1] = y;
    point[2] = z;
    point[3] = 1.0f;
    VU0_LOAD_MATRIX(sdfViewMatrix);
    sdfPostmultiplyVuMatrixFromMemory(sdfProjectionMatrix);
    VU0_MOVE_MATRIX_TO_B();
    VU0_LOAD_VF(vf10, point);
    VU0_TRANSFORM_POINT(vf10, vf10);
    VU0_PERSPECTIVE_DIVIDE_VF10();
    VU0_LOAD_VF(vf11, D_0037F650);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_0037F660);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, screen);
    *gridX = ((s32)(screen[0] * 16.0f) - 0x7000) >> 4;
    *gridY = ((s32)(screen[1] * 16.0f) - 0x7900) >> 3;
}

void sdfCounterDrawGlyphAtGridCell(s32 arg0, s32 arg1, u64 arg2, u64 arg3) {
    u64 handle;

    handle = func_0019F460(arg0 << 4, arg1 << 3, 0, arg2, arg3, 0);
    frFontDrawGlyphWithSharedFlags(handle, 1);
    frFontQueueGlyphInSelectedSlot(handle);
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
    float x, y, z, w;
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
    func_0030F8D0(&normalizedAxis.x);
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

float func_0030F8D0(float *vector) {
    float length = fldVectorLength(vector);

    vector[0] = vector[0] / length;
    vector[1] = vector[1] / length;
    vector[2] = vector[2] / length;
    return length;
}

float fldVectorLength(float *v) {
    return fsqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

/* Normalize copies of the input vectors; callers' vectors stay untouched. */
float fldNormalizedVectorDot(float *left, float *right) {
    Vector4 normalizedLeft, normalizedRight;
    normalizedLeft = *(Vector4 *)left;
    normalizedRight = *(Vector4 *)right;
    func_0030F8D0(&normalizedLeft.x);
    func_0030F8D0(&normalizedRight.x);
    return normalizedLeft.x * normalizedRight.x + normalizedLeft.y * normalizedRight.y + normalizedLeft.z * normalizedRight.z;
}

float fldVec3AngleBetween(a, b)
float *a;
float *b;
{
    return func_003532B8(fldNormalizedVectorDot(a, b));
}

/* Cross the normalized input copies; the result itself is not normalized. */
void fldNormalizedVectorCross(float *output, float *left, float *right) {
    Vector4 normalizedLeft, normalizedRight;
    normalizedLeft = *(Vector4 *)left;
    normalizedRight = *(Vector4 *)right;
    func_0030F8D0(&normalizedLeft.x);
    func_0030F8D0(&normalizedRight.x);
    output[0] = normalizedLeft.y * normalizedRight.z - normalizedLeft.z * normalizedRight.y;
    output[1] = normalizedLeft.z * normalizedRight.x - normalizedLeft.x * normalizedRight.z;
    output[2] = normalizedLeft.x * normalizedRight.y - normalizedLeft.y * normalizedRight.x;
}

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

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_003103C8);

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

void sdfVec4Add(float *out, float *left, float *right) {
    *out = *left + *right;
    out[1] = left[1] + right[1];
    out[2] = left[2] + right[2];
    out[3] = left[3] + right[3];
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

/* Pass the four-component dot directly to the angle evaluator, without input normalization. */
float fldVec4ArcCosDot(float *left, float *right) {
    return func_003532B8(sdfQuatDot(left, right));
}
