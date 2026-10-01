#include "common.h"

typedef struct SdfMat4 {
    f32 m[16];
} SdfMat4;

typedef struct SdfVec4 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} SdfVec4;

extern f32 sdfVec3Normalize();


extern u64 sdfAllocateBlockBySizeThreshold(u64);

extern u64 func_0035A828(u64);

extern u64 func_00325BB0(u64, u32);

extern u64 func_00325AB8(u64, u32);

extern u64 func_00325790(u64, u32);

extern s32 CreateSema(void *);

extern void (*D_004389C4)(void);

extern f32 sdfVec3DotNormalized(void *, void *);

extern f32 func_003532B8(f32);

typedef struct ResourceNode {
    u32 id;
    u32 value;
    struct ResourceNode *next;
    u32 unk_C;
    u32 handle;
} ResourceNode;

typedef struct ResourceList {
    u32 count;
    ResourceNode *first;
} ResourceList;

s32 dds3RemoveListNodeAndNotify(u32 list, u32 node);

ResourceNode *mnuFindResourceNodeById();

ResourceNode *mnuFindResourceNodeByHandle();

/* Reset the first resource list in a two-list owner. */
void func_00324DF8(u32 *lists, u32 option) {
    func_00320CE0(*lists, 0, option);
}

u32 func_00324E18(u32 *pair, u32 key, u32 value) {
    u32 node = mnuFindResourceNodeById(pair[0], key);
    if (node) {
        return func_00320D80(pair[0], node, 0, value);
    }
    return 0;
}

void mnuRemoveMatchedNodesFromLinkedResourceLists(u32 *pair, u32 key) {
    u32 node = mnuFindResourceNodeById(pair[0], key);
    if (node == 0) {
        return;
    }
    dds3RemoveListNodeAndNotify(pair[1], mnuFindResourceNodeByHandle(pair[1], *(u32 *)(node + 0x10)));
    dds3RemoveListNodeAndNotify(pair[0], node);
}

extern s32 func_00321018(u32);

s64 func_00324EF0(u32 *pair) {
    func_00321018(pair[0]);
    return func_00321018(pair[1]);
}

void func_00324F20(ResourceList **list) {
    mnuFindResourceNodeByHandle(*list);
}

void func_00324F38(ResourceList **list) {
    mnuFindResourceNodeById(*list);
}

u64 func_00324F50(s32 owner, u64 resource) {
    u64 handle;

    handle = func_0035A828(resource);
    func_00320CE0(*(u32 *)(owner + 4), 0, handle);
    return handle;
}

s64 func_00324F98(u32 *pair) {
    return dds3RemoveListNodeAndNotify(pair[1], mnuFindResourceNodeByHandle(pair[1]));
}

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00324FD0);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_003251C0);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00325398);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_003255A0);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00325688);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00325790);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00325AB8);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00325BB0);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00325CC8);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00325EC8);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00326018);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00326158);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_003262A8);

void sdfVectorAdd(float *vector, float *delta) {
    *vector = *vector + *delta;
    vector[1] = vector[1] + delta[1];
    vector[2] = vector[2] + delta[2];
}

void sdfVectorSubtract(float *vector, float *delta) {
    *vector = *vector - *delta;
    vector[1] = vector[1] - delta[1];
    vector[2] = vector[2] - delta[2];
}

void func_00326918(float *vector, float x, float y, float z) {
    vector[0] += x;
    vector[1] += y;
    vector[2] += z;
}

void sdfVec3SetComponents(float *vector, float x, float y, float z) {
    vector[0] = x;
    vector[1] = y;
    vector[2] = z;
}

void sdfVectorScale(float factor, float *vector) {
    *vector = *vector * factor;
    vector[1] = vector[1] * factor;
    vector[2] = vector[2] * factor;
}

extern f32 sdfVectorLength();

/* Normalize the first three components; a zero-length vector stays unchanged. */
f32 sdfVec3Normalize(f32 *vector) {
    f32 length = sdfVectorLength();

    if (length == 0.0f) {
        return 0.0f;
    }
    vector[0] = vector[0] / length;
    vector[1] = vector[1] / length;
    vector[2] = vector[2] / length;
    return length;
}

INCLUDE_ASM(const s32, "game/code_00324DF8", sdfVectorLength);

/* Dot product of two normalized 3D directions (w is ignored). */
f32 sdfVec3DotNormalized(void *first, void *second) {
    SdfVec4 firstNormalized = *(SdfVec4 *)first;
    SdfVec4 secondNormalized = *(SdfVec4 *)second;

    sdfVec3Normalize(&firstNormalized);
    sdfVec3Normalize(&secondNormalized);
    return firstNormalized.x * secondNormalized.x + firstNormalized.y * secondNormalized.y + firstNormalized.z * secondNormalized.z;
}

f32 func_00326AE0(void *a, void *b) {
    return func_003532B8(sdfVec3DotNormalized(a, b));
}

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00326B00);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00326BC8);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_003270C8);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_003275C8);

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

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00327C80);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00328018);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00328160);

void func_003282E8(void) {
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

INCLUDE_ASM(const s32, "game/code_00324DF8", sdfCreateThread);

void sdfCreateThreadWithAllocatedWorkspace(u64 destination, u64 source, u64 option) {
    u64 handle;

    handle = sdfAllocateBlockBySizeThreshold(source);
    sdfCreateThread(destination, handle, source, option);
}

INCLUDE_SDATA(const s32, "game/code_00324DF8", D_004389BC);

INCLUDE_SDATA(const s32, "game/code_00324DF8", D_004389C0);

INCLUDE_SDATA(const s32, "game/code_00324DF8", D_004389C4);

