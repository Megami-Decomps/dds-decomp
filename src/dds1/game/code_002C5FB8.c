#include "common.h"

#include "fpu.h"

extern u64 func_00197760(s32, s32, u64, u64, u64, u64);

extern u32 sdfCounterAnimationValue;

extern s32 D_003BD97C;

extern u32 D_003BD980;

extern s32 sdfCounterGetDisplayValue(void);

typedef struct Vec3 {
    float x; // 0x00
    float y; // 0x04
    float z; // 0x08
} Vec3; // 0x0C

extern float fldNormalizedVectorDot(float *, float *);

extern float fldVectorLength(float *vector);

typedef struct MapResource {
    u32 image;
    u32 handle;
    u32 descriptor;
    u32 unkC;
} MapResource;

extern MapResource fldLocalMapNameTextures[10];

extern MapResource D_003906F0;

extern MapResource fldLocalMapTextureResource;

extern s32 fldLoadMapResource(const char *, MapResource *);

extern s32 func_003014F0(char *, const char *, ...);

extern u32 fldReleaseMapResource(s32 *);

extern u32 func_002EB028(const char *, void *, s32);

extern u32 func_002D3288(u32);

extern float func_002FA1C0(float);

extern u32 D_003BD984;

typedef struct MapRequestNode {
    u32 value;
    u32 argument1;
    u32 argument2;
    s32 active;
    struct MapRequestNode *next;
    struct MapRequestNode *prev;
    u8 pad18[8];
} MapRequestNode;

typedef struct {
    u32 handle;           /* 0x00 */
    MapRequestNode *first; /* 0x04 */
    MapRequestNode *next; /* 0x08 */
    MapRequestNode *third; /* 0x0C */
    s16 count;            /* 0x10 */
    s16 arg;              /* 0x12 */
    s16 interval;         /* 0x14 */
    s16 elapsed;          /* 0x16 */
    void (*callback)(void); /* 0x18 */
} MapRequestState;

/* The ring of nodes lives inside the same block, 0x2C past the header. */
typedef struct MapRequestRing {
    MapRequestState header;
    u8 pad1C[0x28];
    MapRequestNode nodes[1]; /* 0x44 */
} MapRequestRing;

extern MapRequestState *D_003BD988;

extern MapRequestState *D_003BD98C;

extern u32 D_003DFED0[];

extern u32 D_003DFEE0[];

s32 sdfQueueNonzeroResourceId(u32 sprite);

typedef struct {
    u32 *word;         /* 0x00 */
    u8 *info;          /* 0x04 */
    s16 value;         /* 0x08 */
    u8 pad0A[4];
    s16 flag;          /* 0x0E */
} SdfCounterDisplay;

typedef struct {
    s32 value;            /* 0x00 */
    s16 countdown;        /* 0x04 */
    s16 mode;             /* 0x06 */
    u8 pad08[4];
    s16 mapTimerPrimary;  /* 0x0C */
    s16 mapTimerSecondary; /* 0x0E */
    s32 y;                /* 0x10 */
    s16 startX;           /* 0x14 */
    s16 startY;           /* 0x16 */
    s16 targetX;          /* 0x18 */
    s16 targetY;          /* 0x1A */
    s16 curX;             /* 0x1C */
    s16 curY;             /* 0x1E */
    s32 frames;           /* 0x20 */
} SdfCounterTimer;

typedef struct SdfCounterChannel {
    s32 index;                      /* 0x00 */
    u8 pad04[0x54];
    struct SdfCounterChannel *next; /* 0x58 */
    struct SdfCounterChannel *prev; /* 0x5C */
    u8 pad60[0x10];
    SdfCounterDisplay *display;     /* 0x70 */
} SdfCounterChannel;

struct SdfCounterRuntime;

typedef void (*SdfCounterDrawFn)(s32, s32, s32, struct SdfCounterRuntime *, SdfCounterChannel *, s32);

typedef struct SdfCounterRuntime {
    u8 pad00[0xC];
    s32 base;                       /* 0x0C */
    SdfCounterChannel *first;       /* 0x10 */
    SdfCounterChannel *last;        /* 0x14 */
    SdfCounterChannel *selected;    /* 0x18 */
    SdfCounterChannel *channel;     /* 0x1C */
    s32 active;                     /* 0x20 */
    s32 scroll;                     /* 0x24 */
    s32 posX;                       /* 0x28 */
    SdfCounterDrawFn draw;          /* 0x2C */
    SdfCounterTimer *timer;         /* 0x30 */
} SdfCounterRuntime;

void func_002C5FB8(u32 arg0) {
    func_00195CD8(arg0, 1, 3);
}

s32 fldCountMaskBitsBeforeOrdinal(s32 mask, s32 ordinal) {
    s32 bitIndex = 0;
    s32 count = 0;
    s32 nextIndex;

    do {
        nextIndex = bitIndex + 1;
        if (ordinal == nextIndex) {
            break;
        }
        count += (mask >> bitIndex) & 1;
        bitIndex = nextIndex;
    } while (bitIndex < 0x1F);
    return count;
}

void sdfCounterSelectChannelByIndex(SdfCounterRuntime *rt, s32 target) {
    SdfCounterChannel *channel;
    SdfCounterChannel *prev;
    s32 i;

    if (target < rt->active) {
        channel = rt->first;
        rt->scroll = 0;
        rt->channel = channel;
        rt->selected = channel;
        for (i = 0; i <= target; i++) {
            prev = channel->prev;
            if (prev != NULL) {
                if (rt->active - i >= rt->base - 1) {
                    rt->selected = prev;
                    rt->scroll = 1;
                } else {
                    rt->scroll = rt->scroll + 1;
                }
            }
            rt->channel = channel;
            channel = channel->next;
            if (channel == NULL) {
                break;
            }
        }
    }
}

typedef struct {
    u8 pad00[0x0C];
    s32 selectedCount; /* 0x0C */
    u8 pad10[0x10];
    s32 maxCount;      /* 0x20 */
} MapSelection;

void fldSetMapSelectedCount(MapSelection *selection, s32 count) {
    if ((count <= selection->maxCount) && (count != 0)) {
        selection->selectedCount = count;
    }
}

void fldIncreaseMapSelectedCount(MapSelection *selection) {
    if (selection->selectedCount < 10) {
        selection->selectedCount = selection->selectedCount + 1;
    }
}

void fldDecreaseMapSelectedCount(MapSelection *selection) {
    if (1 < selection->selectedCount) {
        selection->selectedCount = selection->selectedCount - 1;
    }
}

typedef struct MapSelectionNode {
    u8 pad00[8];
    s16 ordinal;           /* 0x08: 1-based bit position */
} MapSelectionNode;

typedef struct MapSelectionLink {
    u8 pad00[0x58];
    struct MapSelectionLink *next; /* 0x58 */
    u8 pad5C[0x14];
    MapSelectionNode *selection;   /* 0x70 */
} MapSelectionLink;

typedef struct MapSelectionContext {
    u8 pad00[0x10];
    MapSelectionLink *first;       /* 0x10 */
} MapSelectionContext;

/* Collect the 1-based selection ids in the linked map nodes into a bitmask. */
s32 func_002C60F8(MapSelectionContext *context) {
    MapSelectionLink *node;
    s32 mask;

    node = context->first;
    mask = 0;
    do {
        MapSelectionNode *selection = node->selection;
        node = node->next;
        mask |= 1 << (selection->ordinal - 1);
    } while (node != NULL);
    return mask;
}

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C6130);

/* Load the ten numbered "sname" tiles plus the two fixed local-map images. */
s32 fldLoadLocalMapResources(void) {
    char name[32];
    s32 i;
    MapResource *item = fldLocalMapNameTextures;

    for (i = 0; i < 10; i++) {
        func_003014F0(name, "/lmap/sname_%02d.tmx", i + 1);
        fldLoadMapResource(name, item);
        item++;
    }
    fldLoadMapResource("/lmap/1006.tmx", &D_003906F0);
    fldLoadMapResource("/lmap/l_map00.tmx", &fldLocalMapTextureResource);
    return 1;
}

s32 fldReleaseLocalMapResources(void) {
    s32 i = 9;
    MapResource *item = fldLocalMapNameTextures;
    do {
        fldReleaseMapResource((s32 *)item);
        item++;
        --i;
    } while (i >= 0);
    fldReleaseMapResource((s32 *)&D_003906F0);
    fldReleaseMapResource((s32 *)&fldLocalMapTextureResource);
    return 1;
}

void sdfCounterInitializeDisplayAnimation(void) {
    s32 count;

    sdfCounterAnimationValue = 0;
    count = sdfCounterGetDisplayValue();
    D_003BD97C = count - 1;
    D_003BD980 = 0x3c;
}

void sdfCounterAdvanceBoundedAnimationValue(void) {
    if ((s32)sdfCounterAnimationValue < 0x3C) {
        sdfCounterAnimationValue++;
    }
}

void sdfCounterStepDownAnimationValue(void) {
    if ((s32)sdfCounterAnimationValue > 0) {
        sdfCounterAnimationValue -= 2;
    } else {
        sdfCounterAnimationValue = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C6448);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C6948);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C6EC8);

extern MapRequestState *sdfCreateLinkedRequestRing(s16, s16);

extern void func_002C7180(void);

extern void func_002C7430(void);

void fldSetMapRequestInterval(MapRequestState *state, u16 interval);

/* Allocate the two map request queues and install their dispatch callbacks. */
void fldCreateMapRequestQueues(void) {
    D_003BD988 = sdfCreateLinkedRequestRing(0x14, 0xC);
    D_003BD988->callback = func_002C7180;
    fldSetMapRequestInterval(D_003BD988, 0);
    D_003BD98C = sdfCreateLinkedRequestRing(0x14, 0x18);
    D_003BD98C->callback = func_002C7430;
}

/* Release both map request queues. */
s64 fldReleaseMapRequestQueues(void) {
    func_002C7B38(D_003BD988);
    return func_002C7B38(D_003BD98C);
}

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C7080);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C7180);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C7430);

void sdfCommitPendingVectorAndMarkChanged(void) {
    D_003DFEE0[0] = D_003DFED0[0];
    D_003DFEE0[1] = D_003DFED0[1];
    D_003DFEE0[2] = D_003DFED0[2];
    D_003BD984 = 1;
}

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C7738);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C7950);

extern u32 func_002D03F8(s32 size);
extern void *sdfMemoryGetBlockAddress(u32 handle);

/* Build a ring of `count` request nodes (0x20 bytes each) behind a 0x44-byte queue header. */
MapRequestState *sdfCreateLinkedRequestRing(s16 count, s16 arg) {
    s32 size = count * 0x20 + 0x44;
    u32 handle = func_002D03F8(size);
    MapRequestRing *pool = (MapRequestRing *)sdfMemoryGetBlockAddress(handle);
    MapRequestState *state = &pool->header;
    MapRequestNode *node;
    MapRequestNode *next;
    MapRequestNode *first;
    s32 n;

    memset(state, 0, size);
    state->handle = handle;
    node = pool->nodes;
    state->first = node;
    state->third = node;
    state->next = node;
    for (n = count - 2; n != -1; n--) {
        next = node + 1;
        node->next = next;
        next->prev = node;
        node = node->next;
    }
    first = state->first;
    node->next = first;
    first->prev = node;
    state->arg = arg;
    state->count = count;
    state->interval = 0;
    return state;
}

s64 func_002C7B38(u32 *sprite) {
    if (sprite != NULL) {
        return sdfQueueNonzeroResourceId(*sprite);
    }
}

/* Dispatch one queued request per interval; inactive nodes are reused. */
void fldAdvanceMapRequest(MapRequestState *state, u32 value, u32 argument1, u32 argument2) {
    MapRequestNode *node;

    node = state->next;
    if (state->elapsed == state->interval) {
        if (node->active == 0) {
            node->value = value;
            node->argument1 = argument1;
            node->argument2 = argument2;
            state->next = node->next;
            node->active = 1;
        }
        state->elapsed = 0;
        return;
    }
    state->elapsed = state->elapsed + 1;
}

void fldSetMapRequestInterval(MapRequestState *state, u16 interval) {
    state->interval = interval;
}

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C7BB0);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C7C58);

s32 fldLoadMapResource(const char *name, MapResource *record) {
    u32 handle = func_002EB028(name, &record->descriptor, 0);
    u32 descriptor = record->descriptor;
    record->handle = handle;
    record->image = func_002D3288(descriptor);
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

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C7DC0);

void sdfCounterDrawGlyphAtGridCell(s32 gridX, s32 gridY, u64 style, u64 flags) {
    u64 glyph;

    glyph = func_00197760(gridX << 4, gridY << 3, 0, style, flags, 0);
    frFontDrawGlyphWithSharedFlags(glyph, 1);
    frFontQueueGlyphInSelectedSlot(glyph);
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

typedef struct Vector4 {
    float x;
    float y;
    float z;
    float w;
} Vector4;

/* Rotate `v` about `axis` (normalized first) by `angle`, using the axis-angle rotation matrix. */
void fldRotateVectorAroundAxis(float *v, float *axis, float angle) {
    Vector4 a;
    Vector4 u;
    Vector4 w;
    float m[9];

    memset(&u, 0, sizeof(u));
    u.x = axis[0];
    u.y = axis[1];
    u.z = axis[2];
    a = u;
    memset(&w, 0, sizeof(w));
    w.x = v[0];
    w.y = v[1];
    w.z = v[2];
    u = w;
    func_002C84F0(&a.x);
    m[0] = a.x * a.x * (1.0f - sdfEvaluateCosineViaSinePhaseShift(angle)) + sdfEvaluateCosineViaSinePhaseShift(angle);
    m[1] = a.x * a.y * (1.0f - sdfEvaluateCosineViaSinePhaseShift(angle)) - a.z * sdfSinPoly(angle);
    m[2] = a.x * a.z * (1.0f - sdfEvaluateCosineViaSinePhaseShift(angle)) + a.y * sdfSinPoly(angle);
    m[3] = a.y * a.x * (1.0f - sdfEvaluateCosineViaSinePhaseShift(angle)) + a.z * sdfSinPoly(angle);
    m[4] = a.y * a.y * (1.0f - sdfEvaluateCosineViaSinePhaseShift(angle)) + sdfEvaluateCosineViaSinePhaseShift(angle);
    m[5] = a.y * a.z * (1.0f - sdfEvaluateCosineViaSinePhaseShift(angle)) - a.x * sdfSinPoly(angle);
    m[6] = a.z * a.x * (1.0f - sdfEvaluateCosineViaSinePhaseShift(angle)) - a.y * sdfSinPoly(angle);
    m[7] = a.z * a.y * (1.0f - sdfEvaluateCosineViaSinePhaseShift(angle)) + a.x * sdfSinPoly(angle);
    m[8] = a.z * a.z * (1.0f - sdfEvaluateCosineViaSinePhaseShift(angle)) + sdfEvaluateCosineViaSinePhaseShift(angle);
    v[0] = u.x * m[0] + u.y * m[3] + u.z * m[6];
    v[1] = u.x * m[1] + u.y * m[4] + u.z * m[7];
    v[2] = u.x * m[2] + u.y * m[5] + u.z * m[8];
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

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C84F0);

float fldVectorLength(float *vector) {
    return fsqrtf(vector[0] * vector[0] + vector[1] * vector[1] +
                  vector[2] * vector[2]);
}

/* Normalize copies of the input vectors; callers' vectors stay untouched. */
float fldNormalizedVectorDot(float *left, float *right) {
    Vector4 a;
    Vector4 b;

    a = *(Vector4 *)left;
    b = *(Vector4 *)right;
    func_002C84F0(&a.x);
    func_002C84F0(&b.x);
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float fldVec3AngleBetween(float *left, float *right) {
    return func_002FA1C0(fldNormalizedVectorDot(left, right));
}

void sdfCrossNormalizedVectors(float *out, float *left, float *right) {
    Vector4 a;
    Vector4 b;

    a = *(Vector4 *)left;
    b = *(Vector4 *)right;
    func_002C84F0(&a.x);
    func_002C84F0(&b.x);
    out[0] = a.y * b.z - a.z * b.y;
    out[1] = a.z * b.x - a.x * b.z;
    out[2] = a.x * b.y - a.y * b.x;
}

typedef struct SdfMat4 {
    f32 m[16];
} SdfMat4;

/* Rotate rows 1 and 2 of the matrix about the X axis by `angle`. */
void sdfRotateMatrixBasisAboutX(SdfMat4 *mat, f32 angle) {
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
void sdfRotateMatrixBasisAboutY(SdfMat4 *mat, f32 angle) {
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
void sdfRotateMatrixBasisAboutZ(SdfMat4 *mat, f32 angle) {
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

extern void *memcpy(void *, const void *, u32);

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

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C8FE8);

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

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C9268);

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

void sdfQuatMultiply(float *dst, float *lhs, float *rhs) {
    *dst = (lhs[3] * *rhs + *lhs * rhs[3] + lhs[1] * rhs[2]) -
                          lhs[2] * rhs[1];
    dst[1] = (lhs[3] * rhs[1] + lhs[1] * rhs[3] + lhs[2] * *rhs) -
                              *lhs * rhs[2];
    dst[2] = (lhs[3] * rhs[2] + lhs[2] * rhs[3] + *lhs * rhs[1]) -
                              lhs[1] * *rhs;
    dst[3] = ((lhs[3] * rhs[3] - *lhs * *rhs) - lhs[1] * rhs[1]) -
                              lhs[2] * rhs[2];
}

float sdfQuatDot(float *lhs, float *rhs) {
    return *lhs * *rhs + lhs[1] * rhs[1] + lhs[2] * rhs[2] +
                  lhs[3] * rhs[3];
}

float sdfSumCrossProductComponents(float *arg0, float *arg1) {
    return (arg0[1] * arg1[2] - arg0[2] * arg1[1]) +
                  (arg0[2] * *arg1 - *arg0 * arg1[2]) +
                  (*arg0 * arg1[1] - arg0[1] * *arg1);
}

float fldVec4ArcCosDot(float *left, float *right) {
    return func_002FA1C0(sdfQuatDot(left, right));
}

INCLUDE_RODATA(const s32, "game/code_002C5FB8", D_003B3DC0);

INCLUDE_RODATA(const s32, "game/code_002C5FB8", D_003B3E00);

INCLUDE_RODATA(const s32, "game/code_002C5FB8", D_003B3E40);

INCLUDE_SDATA(const s32, "game/code_002C5FB8", D_003BD281);

