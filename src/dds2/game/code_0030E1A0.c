#include "common.h"
#include "fpu.h"

extern u64 func_0019F460(s32, s32, u64, u64, u64, u64);

extern u32 D_004390A4;

extern u32 D_0045C7A0[];

extern u32 D_0045C7B0[];

s32 func_003298C0(u32 sprite);

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

extern s32 func_0030EE40(s32, s32);

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
extern s32 func_003292A8(s32);
extern void *sdfMemoryGetBlockAddress(u32);
extern void *memset(void *, s32, u32);

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030E1A0);

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030E390);

void func_0030E878(void) {
}

void func_0030E880(void) {
    s32 handler;
    handler = func_0030EE40(0x14, 0xC);
    D_004390AC = handler;
    *(s32 *)(D_004390AC + 0x18) = (s32)func_0030E940;
    fldSetMapRequestInterval(handler, 0);
    D_004390B0 = func_0030EE40(0x14, 0x18);
    *(s32 *)(D_004390B0 + 0x18) = (s32)func_0030E958;
    D_004390A8 = 5;
    D_004390A4 = 0;
}

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030E8E8);

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030E910);

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030E940);

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030E958);

void func_0030EA70(void) {
    D_0045C7B0[0] = D_0045C7A0[0];
    D_0045C7B0[1] = D_0045C7A0[1];
    D_0045C7B0[2] = D_0045C7A0[2];
    D_004390A4 = 1;
}

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030EAA8);

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030ECC0);

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030EE40);

s64 func_0030EF18(u32 *sprite) {
    if (sprite != NULL) {
        return func_003298C0(*sprite);
    }
}

void fldAdvanceMapRequest(s32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    u32 *puVar1;

    puVar1 = *(u32 **)(arg0 + 8);
    if (*(s16 *)(arg0 + 0x16) == *(s16 *)(arg0 + 0x14)) {
        if (puVar1[3] == 0) {
            *puVar1 = arg1;
            puVar1[1] = arg2;
            puVar1[2] = arg3;
            *(u32 *)(arg0 + 8) = puVar1[4];
            puVar1[3] = 1;
        }
        *(u16 *)(arg0 + 0x16) = 0;
        return;
    }
    *(s16 *)(arg0 + 0x16) = *(s16 *)(arg0 + 0x16) + 1;
}

void fldSetMapRequestInterval(s32 arg0, u16 arg1) {
    *(u16 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030EF90);

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030F038);

s32 fldLoadMapResource(const char *name, MapResource *record) {
    u32 handle = func_00343ED0(name, &record->descriptor, 0);
    u32 descriptor = record->descriptor;
    record->handle = handle;
    record->image = func_0032C138(descriptor);
    if (record->handle != 0) {
        func_003298C0((void *)record->handle);
        record->handle = 0;
        record->descriptor = 0;
    }
    return 1;
}

u32 fldReleaseMapResource(s32 *arg0) {
    if (*arg0 != 0) {
        func_0032BBB0(*arg0);
        *arg0 = 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030F1A0);

void func_0030F2A8(s32 arg0, s32 arg1, u64 arg2, u64 arg3) {
    u64 temp_v0;

    temp_v0 = func_0019F460(arg0 << 4, arg1 << 3, 0, arg2, arg3, 0);
    func_0019D530(temp_v0, 1);
    func_0019C5B0(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030F2F8);

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030F390);

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030F420);

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

float fldVectorLength(float *v) {
    return fsqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

float fldNormalizedVectorDot(float *left, float *right) {
    struct Vector4 { float x, y, z, w; } a, b;
    a = *(struct Vector4 *)left;
    b = *(struct Vector4 *)right;
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

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030FA28);

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030FAF0);

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030FD50);

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_0030FFB0);

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_00310210);

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_00310320);

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_003103C8);

float func_00310608(float x, float y) {
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

INCLUDE_ASM(const s32, "game/code_0030E1A0", func_00310888);

void sdfVec4Add(float *arg0, float *arg1, float *arg2) {
    *arg0 = *arg1 + *arg2;
    arg0[1] = arg1[1] + arg2[1];
    arg0[2] = arg1[2] + arg2[2];
    arg0[3] = arg1[3] + arg2[3];
}

void sdfQuatMultiply(float *arg0, float *arg1, float *arg2) {
    *arg0 = (arg1[3] * *arg2 + *arg1 * arg2[3] + arg1[1] * arg2[2]) -
                          arg1[2] * arg2[1];
    arg0[1] = (arg1[3] * arg2[1] + arg1[1] * arg2[3] + arg1[2] * *arg2) -
                              *arg1 * arg2[2];
    arg0[2] = (arg1[3] * arg2[2] + arg1[2] * arg2[3] + *arg1 * arg2[1]) -
                              arg1[1] * *arg2;
    arg0[3] = ((arg1[3] * arg2[3] - *arg1 * *arg2) - arg1[1] * arg2[1]) -
                              arg1[2] * arg2[2];
}

float sdfQuatDot(float *arg0, float *arg1) {
    return *arg0 * *arg1 + arg0[1] * arg1[1] + arg0[2] * arg1[2] +
                  arg0[3] * arg1[3];
}

float func_00310B60(float *arg0, float *arg1) {
    return (arg0[1] * arg1[2] - arg0[2] * arg1[1]) +
                  (arg0[2] * *arg1 - *arg0 * arg1[2]) +
                  (*arg0 * arg1[1] - arg0[1] * *arg1);
}

float fldVec4ArcCosDot(float *a, float *b) {
    return func_003532B8(sdfQuatDot(a, b));
}
