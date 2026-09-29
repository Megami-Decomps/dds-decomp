#include "common.h"

#include "fpu.h"

extern u64 func_00197760(s32, s32, u64, u64, u64, u64);

extern u32 D_003BD978;

extern s32 D_003BD97C;

extern u32 D_003BD980;

extern s32 func_002C4A10(void);

typedef struct Vec3 {
    float x; // 0x00
    float y; // 0x04
    float z; // 0x08
} Vec3; // 0x0C

extern float fldNormalizedVectorDot(float *, float *);

typedef struct MapResource {
    u32 image;
    u32 handle;
    u32 descriptor;
    u32 unkC;
} MapResource;

extern MapResource D_00390710[10];

extern MapResource D_003906F0;

extern MapResource D_00390700;

extern u32 fldReleaseMapResource(s32 *);

extern u32 func_002EB028(const char *, void *, s32);

extern u32 func_002D3288(u32);

extern float func_002FA1C0(float);

extern u32 D_003BD984;

extern u32 D_003DFED0[];

extern u32 D_003DFEE0[];

s32 func_002D0A10(u32 sprite);

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

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C6010);

typedef struct {
    u8 pad00[0x0C];
    s32 selectedCount; /* 0x0C */
    u8 pad10[0x10];
    s32 maxCount;      /* 0x20 */
} MapSelection;

void fldSetSelectionCount(MapSelection *selection, s32 count) {
    if ((count <= selection->maxCount) && (count != 0)) {
        selection->selectedCount = count;
    }
}

void fldIncrementSelectionCount(MapSelection *selection) {
    if (selection->selectedCount < 10) {
        selection->selectedCount = selection->selectedCount + 1;
    }
}

void fldDecrementSelectionCount(MapSelection *selection) {
    if (1 < selection->selectedCount) {
        selection->selectedCount = selection->selectedCount - 1;
    }
}

/* Collect the 1-based selection ids in the linked map nodes into a bitmask. */
s32 func_002C60F8(void *context) {
    void *node;
    s32 mask;

    node = *(void **)((s32)context + 0x10);
    mask = 0;
    do {
        void *selection = *(void **)((s32)node + 0x70);
        node = *(void **)((s32)node + 0x58);
        mask |= 1 << (*(s16 *)((s32)selection + 8) - 1);
    } while (node != NULL);
    return mask;
}

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C6130);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C62D8);

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
    s32 temp_v0;

    D_003BD978 = 0;
    temp_v0 = func_002C4A10();
    D_003BD97C = temp_v0 - 1;
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

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C7000);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C7058);

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
} MapRequestState;

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

u32 fldReleaseMapResource(s32 *arg0) {
    if (*arg0 != 0) {
        func_002D2D00(*arg0);
        *arg0 = 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C7DC0);

void func_002C7EC8(s32 arg0, s32 arg1, u64 arg2, u64 arg3) {
    u64 temp_v0;

    temp_v0 = func_00197760(arg0 << 4, arg1 << 3, 0, arg2, arg3, 0);
    func_00195880(temp_v0, 1);
    func_00194920(temp_v0);
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

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C8E30);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C8F40);

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

