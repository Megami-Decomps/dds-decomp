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

extern void *func_0035A828(u64);

extern u64 func_00325790(u64, u32);

extern s32 CreateSema(void *);

extern void (*sdfTickCallback)(void);

extern f32 sdfVec3DotNormalized(void *, void *);

extern f32 func_003532B8(f32);

extern f64 cos(f64);

extern f64 sin(f64);

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

typedef struct SdfResourceInfo {
    u32 word[4];
} SdfResourceInfo;

typedef struct SdfResourceRecord {
    u8 unk00[0xE];
    u16 count;
    u32 *items;
    SdfResourceInfo *info;
} SdfResourceRecord;

typedef struct SdfResourceVectorRecord1C {
    u8 data[0x1C];
} SdfResourceVectorRecord1C;

typedef struct SdfResourceVectorRecord30 {
    u8 data[0x2C];
    SdfVec4 *vector;
} SdfResourceVectorRecord30;

extern u32 *func_00324D50(void);
extern void *memcpy(void *, const void *, u32);

s32 dds3RemoveListNodeAndNotify(u32 list, u32 node);

ResourceNode *mnuFindResourceNodeById();

ResourceNode *mnuFindResourceNodeByHandle();

/* Reset the first resource list in a two-list owner. */
void func_00324DF8(u32 *lists, u32 option) {
    func_00320CE0(*lists, 0, option);
}

u32 mnuInsertResourceHandleAfterMatchingId(u32 *pair, u32 key, u32 value) {
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

extern s32 mnuClearResourceList(u32);

s64 mnuClearOwnedResourceListPair(u32 *pair) {
    mnuClearResourceList(pair[0]);
    return mnuClearResourceList(pair[1]);
}

void func_00324F20(ResourceList **list) {
    mnuFindResourceNodeByHandle(*list);
}

void func_00324F38(ResourceList **list) {
    mnuFindResourceNodeById(*list);
}

void *func_00324F50(s32 owner, u64 resource) {
    void *handle;

    handle = func_0035A828(resource);
    func_00320CE0(*(u32 *)(owner + 4), 0, (u32)handle);
    return handle;
}

INCLUDE_ASM(const s32, "game/code_00324DF8", mnuRemoveLinkedResourceByHandle);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00324FD0);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_003251C0);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00325398);

INCLUDE_ASM(const s32, "game/code_00324DF8", func_003255A0);

u32 *sdfCloneOwnedResourceRecords(const SdfResourceRecord *source, s32 count) {
    u32 *owner = func_00324D50();

    if (count != 0) {
        do {
            SdfResourceRecord *copy = (SdfResourceRecord *)func_00324F50((s32)owner, sizeof(*copy));

            *copy = *source;
            copy->items = (u32 *)func_00324F50((s32)owner, copy->count * sizeof(*copy->items));
            memcpy(copy->items, source->items, copy->count * sizeof(*copy->items));
            copy->info = (SdfResourceInfo *)func_00324F50((s32)owner, sizeof(*copy->info));
            *copy->info = *source->info;
            func_00324DF8(owner, (u32)copy);
            source++;
            count--;
        } while (count != 0);
    }
    return owner;
}

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00325790);

u32 *func_00325AB8(const SdfResourceVectorRecord1C *source, s32 count) {
    u32 *owner = func_00324D50();

    while (count != 0) {
        SdfResourceVectorRecord1C *copy =
            (SdfResourceVectorRecord1C *)func_00324F50((s32)owner, sizeof(*copy) + sizeof(SdfVec4));
        SdfVec4 *vector;

        memset(copy, 0, sizeof(*copy) + sizeof(*vector));
        *copy = *source;
        vector = (SdfVec4 *)(copy + 1);
        *(SdfVec4 **)((u8 *)copy + 0x18) = vector;
        *vector = **(SdfVec4 **)((u8 *)source + 0x18);
        func_00324DF8(owner, (u32)copy);
        source++;
        count--;
    }
    return owner;
}

u32 *func_00325BB0(const SdfResourceVectorRecord30 *source, s32 count) {
    u32 *owner = func_00324D50();

    while (count != 0) {
        SdfResourceVectorRecord30 *copy =
            (SdfResourceVectorRecord30 *)func_00324F50((s32)owner, sizeof(*copy) + sizeof(SdfVec4));
        SdfVec4 *vector;

        memset(copy, 0, sizeof(*copy) + sizeof(*vector));
        *copy = *source;
        vector = (SdfVec4 *)(copy + 1);
        copy->vector = vector;
        *vector = *source->vector;
        func_00324DF8(owner, (u32)copy);
        source++;
        count--;
    }
    return owner;
}

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00325CC8);

void func_00325EC8(f32 *vector, f32 angle) {
    f32 rotated[4];

    rotated[1] = vector[1] * cos(angle) + vector[2] * sin(angle);
    rotated[2] = vector[1] * -sin(angle) + vector[2] * cos(angle);
    vector[1] = rotated[1];
    vector[2] = rotated[2];
}

void func_00326018(f32 *vector, f32 angle) {
    f32 rotated[4];

    rotated[0] = vector[0] * cos(angle) - vector[2] * sin(angle);
    rotated[2] = vector[0] * sin(angle) + vector[2] * cos(angle);
    vector[0] = rotated[0];
    vector[2] = rotated[2];
}

void func_00326158(f32 *vector, f32 angle) {
    f32 rotated[4];

    rotated[0] = vector[0] * cos(angle) + vector[1] * sin(angle);
    rotated[1] = vector[0] * -sin(angle) + vector[1] * cos(angle);
    vector[0] = rotated[0];
    vector[1] = rotated[1];
}

/* Rotate a vector about a normalized axis using an axis-angle matrix. */
void func_003262A8(f32 *vector, f32 *axis, f32 angle) {
    SdfVec4 normalized;
    SdfVec4 source;
    SdfVec4 temporary;
    f32 matrix[9];

    memset(&source, 0, sizeof(source));
    source.x = axis[0];
    source.y = axis[1];
    source.z = axis[2];
    normalized = source;
    memset(&temporary, 0, sizeof(temporary));
    temporary.x = vector[0];
    temporary.y = vector[1];
    temporary.z = vector[2];
    source = temporary;
    sdfVec3Normalize(&normalized.x);
    matrix[0] = normalized.x * normalized.x * (1.0f - cos(angle)) + cos(angle);
    matrix[1] = normalized.x * normalized.y * (1.0f - cos(angle)) - normalized.z * sin(angle);
    matrix[2] = normalized.x * normalized.z * (1.0f - cos(angle)) + normalized.y * sin(angle);
    matrix[3] = normalized.y * normalized.x * (1.0f - cos(angle)) + normalized.z * sin(angle);
    matrix[4] = normalized.y * normalized.y * (1.0f - cos(angle)) + cos(angle);
    matrix[5] = normalized.y * normalized.z * (1.0f - cos(angle)) - normalized.x * sin(angle);
    matrix[6] = normalized.z * normalized.x * (1.0f - cos(angle)) - normalized.y * sin(angle);
    matrix[7] = normalized.z * normalized.y * (1.0f - cos(angle)) + normalized.x * sin(angle);
    matrix[8] = normalized.z * normalized.z * (1.0f - cos(angle)) + cos(angle);
    vector[0] = source.x * matrix[0] + source.y * matrix[3] + source.z * matrix[6];
    vector[1] = source.x * matrix[1] + source.y * matrix[4] + source.z * matrix[7];
    vector[2] = source.x * matrix[2] + source.y * matrix[5] + source.z * matrix[8];
}

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

void sdfVectorAddComponents(float *vector, float x, float y, float z) {
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

INCLUDE_ASM(const s32, "game/code_00324DF8", func_00327C80);

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

INCLUDE_ASM(const s32, "game/code_00324DF8", sdfCreateThread);

void sdfCreateThreadWithAllocatedWorkspace(u64 destination, u64 source, u64 option) {
    u64 handle;

    handle = sdfAllocateBlockBySizeThreshold(source);
    sdfCreateThread(destination, handle, source, option);
}

INCLUDE_SDATA(const s32, "game/code_00324DF8", D_004389BC);

INCLUDE_SDATA(const s32, "game/code_00324DF8", D_004389C0);

INCLUDE_SDATA(const s32, "game/code_00324DF8", sdfTickCallback);

