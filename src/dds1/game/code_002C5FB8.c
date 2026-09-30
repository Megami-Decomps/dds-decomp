#include "common.h"

#include "fpu.h"

extern u64 func_00197760(s32, s32, u64, u64, u64, u64);

extern u32 D_003BD978;

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

extern MapResource D_00390710[10];

extern MapResource D_003906F0;

extern MapResource D_00390700;

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
} MapRequestNode;

typedef struct {
    u8 pad00[8];
    MapRequestNode *next; /* 0x08 */
    u8 pad0C[8];
    s16 interval;         /* 0x14 */
    s16 elapsed;          /* 0x16 */
    void (*callback)(void); /* 0x18 */
} MapRequestState;

extern MapRequestState *D_003BD988;

extern MapRequestState *D_003BD98C;

extern u32 D_003DFED0[];

extern u32 D_003DFEE0[];

s32 func_002D0A10(u32 sprite);

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

void func_002C6010(SdfCounterRuntime *rt, s32 target) {
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
    MapResource *item = D_00390710;

    for (i = 0; i < 10; i++) {
        func_003014F0(name, "/lmap/sname_%02d.tmx", i + 1);
        fldLoadMapResource(name, item);
        item++;
    }
    fldLoadMapResource("/lmap/1006.tmx", &D_003906F0);
    fldLoadMapResource("/lmap/l_map00.tmx", &D_00390700);
    return 1;
}

s32 fldReleaseLocalMapResources(void) {
    s32 i = 9;
    MapResource *item = D_00390710;
    do {
        fldReleaseMapResource((s32 *)item);
        item++;
        --i;
    } while (i >= 0);
    fldReleaseMapResource((s32 *)&D_003906F0);
    fldReleaseMapResource((s32 *)&D_00390700);
    return 1;
}

void func_002C63D8(void) {
    s32 count;

    D_003BD978 = 0;
    count = sdfCounterGetDisplayValue();
    D_003BD97C = count - 1;
    D_003BD980 = 0x3c;
}

void func_002C6408(void) {
    if ((s32)D_003BD978 < 0x3C) {
        D_003BD978++;
    }
}

void func_002C6428(void) {
    if ((s32)D_003BD978 > 0) {
        D_003BD978 -= 2;
    } else {
        D_003BD978 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C6448);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C6948);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C6EC8);

extern MapRequestState *func_002C7A60(s16, s16);

extern void func_002C7180(void);

extern void func_002C7430(void);

void fldSetMapRequestInterval(MapRequestState *state, u16 interval);

/* Allocate the two map request queues and install their dispatch callbacks. */
void fldCreateMapRequestQueues(void) {
    D_003BD988 = func_002C7A60(0x14, 0xC);
    D_003BD988->callback = func_002C7180;
    fldSetMapRequestInterval(D_003BD988, 0);
    D_003BD98C = func_002C7A60(0x14, 0x18);
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

void func_002C7700(void) {
    D_003DFEE0[0] = D_003DFED0[0];
    D_003DFEE0[1] = D_003DFED0[1];
    D_003DFEE0[2] = D_003DFED0[2];
    D_003BD984 = 1;
}

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C7738);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C7950);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C7A60);

s64 func_002C7B38(u32 *sprite) {
    if (sprite != NULL) {
        return func_002D0A10(*sprite);
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
        func_002D0A10((void *)record->handle);
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

void func_002C7EC8(s32 gridX, s32 gridY, u64 style, u64 flags) {
    u64 glyph;

    glyph = func_00197760(gridX << 4, gridY << 3, 0, style, flags, 0);
    frFontDrawGlyphWithSharedFlags(glyph, 1);
    frFontQueueGlyphInSelectedSlot(glyph);
}

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C7F18);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C7FB0);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C8040);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C80D8);

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

void func_002C84B8(Vec3 *v, float x, float y, float z) {
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
    struct Vector4 { float x, y, z, w; } a, b;
    a = *(struct Vector4 *)left;
    b = *(struct Vector4 *)right;
    func_002C84F0(&a.x);
    func_002C84F0(&b.x);
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float fldVec3AngleBetween(float *left, float *right) {
    return func_002FA1C0(fldNormalizedVectorDot(left, right));
}

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C8648);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C8710);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C8970);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C8BD0);

typedef struct SdfMat4 {
    f32 m[16];
} SdfMat4;

/* Transpose through a local copy so source and destination may alias. */
void func_002C8E30(SdfMat4 *dst, SdfMat4 *src) {
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

f32 *func_002C8F40(f32 *vec, f32 *mat) {
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

float func_002C9228(float x, float y) {
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

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C94A8);

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

float func_002C9780(float *arg0, float *arg1) {
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

