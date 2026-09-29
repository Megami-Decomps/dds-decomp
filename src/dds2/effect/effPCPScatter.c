#include "common.h"
#include "pcp_vu0.h"
extern void func_00336AA8(void);

/* Load a 4x4 matrix into vf24-vf27 (the second operand of func_00336AA8). */
#define VU0_LOAD_MATRIX_B(src) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "lqc2 vf24, 0(%0)\n\t" \
    "lqc2 vf25, 16(%0)\n\t" \
    "lqc2 vf26, 32(%0)\n\t" \
    "lqc2 vf27, 48(%0)\n\t" \
    ".set reorder" \
    : : "r" (src) : "memory")


extern u64 effParamTableGetBlock(u64, u64);

extern u32 effPcpScatterResAddRef(u32);

/* Shared resource handed between scatter effects. func_00173018 creates it,
   func_001730B8 takes a reference, func_00173068 releases it. */
typedef struct PcpScatterRes PcpScatterRes;

struct PcpScatterRes {
    u32 unk00;
    s32 refCount;
};

extern void *func_00328D68(s32 size);

extern u32 func_0032C138(u32 resId);

extern PcpScatterRes *effPcpScatterResCreate(u32 resId);

typedef struct PcpScatterWork4 PcpScatterWork4;

/* func_001730D0 */
struct PcpScatterWork4 {
    u8 pad00[0x40];
    s128 unk40;
    u8 pad50[0x12C];
    f32 unk17C;
    u32 unk180;
    u32 scatterObject;
    u32 ownedBuffer;
};

extern PcpScatterWork4 *func_0017AD28(u64 param0, u64 param1);

extern void effShareScatterResource(u32 param0, u32 param1);

typedef struct PcpScatterWork5 PcpScatterWork5;

/* func_00173B48 */
struct PcpScatterWork5 {
    u8 pad00[0x40];
    s128 unk40;
    u8 pad50[0x130];
    f32 unk180;
    u32 unk184;
    u32 unk188;
    u32 scatterObject;
    u32 ownedBuffer;
};

typedef struct PcpScatterWork6 PcpScatterWork6;

/* func_00174680 */
struct PcpScatterWork6 {
    u8 pad00[0x40];
    s128 unk40;
    u8 pad50[0x138];
    f32 unk188;
    u32 unk18C;
    u32 unk190;
    u32 scatterObject;
    u32 ownedBuffer;
};

extern void *func_0017B7A0();

extern void *func_0017C2D8();

extern void *func_0017CE88();

typedef struct PcpScatterWork7 PcpScatterWork7;

/* func_00175230 */
struct PcpScatterWork7 {
    u8 pad00[0x40];
    s128 unk40;
    u8 pad50[0xE4];
    u32 scatterObject;
    u32 ownedBuffer;
};

typedef struct PcpScatterWork8 {
    u8 pad00[0x3C];
    f32 unk3C;
    u8 pad40[0x0C];
    f32 unk4C;
    f32 unk50;
    u8 pad54[0x28];
    u32 *unk7C;
} PcpScatterWork8;

typedef struct PcpScatterWork2 {
    u8 pad00[0x40];
    f32 unk40;
    f32 unk44;
    u8 pad48[0x24];
    u32 unk6C;
} PcpScatterWork2;

typedef struct PcpScatterPool {
    u8 pad00[0x10];
    s32 unk10;                /* 0x10: set to 1 on creation */
    u32 color;                /* 0x14: 0x80808080 on creation */
    s32 unk18;                /* 0x18: 6 * record count */
    f32 scale;                /* 0x1C: 1.0f on creation */
    s32 recordBase;           /* 0x20: base of 0x60-byte records */
    s32 auxRecordBase;        /* 0x24: base of 0x18-byte records */
    u32 resource;             /* 0x28: released by func_00333918 */
    u32 buffer;               /* 0x2C: freed by func_003297C8 */
    PcpScatterRes *sharedResource; /* 0x30: reference counted */
} PcpScatterPool;

void func_001787E0(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;
    u64 temp_v2;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    temp_v1 = effParamTableGetBlock(arg0, 1);
    temp_v2 = effParamTableGetBlock(arg0, 2);
    func_001784F8(temp_v0, temp_v1, temp_v2);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", effPcpScatterSharedDuplicate);

typedef struct PcpScatterParticles {
    PcpScatterPool *pool;
    u32 buffer;
    u32 count;
    u32 *items;
    u32 itemBuffer;
} PcpScatterParticles;

extern void func_0016A620(u32 particle);

typedef struct PcpScatterGroupWork {
    u8 pad00[0x80];
    PcpScatterParticles particles;
} PcpScatterGroupWork;

void effPcpScatterReleaseParticleGroup(PcpScatterGroupWork *work) {
    if (work->particles.itemBuffer != 0) {
        u32 count = work->particles.count;
        u32 i;

        for (i = 0; i < count; i++) {
            func_0016A620(work->particles.items[i]);
        }
        func_003297C8(work->particles.itemBuffer);
    }
    func_0017A9C8(work->particles.pool);
    func_003297C8(work->particles.buffer);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001789C0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00178B80);

void func_00179168(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00179178(PcpScatterWork8 *work, u32 *values) {
    work->unk7C = values;
}

void func_00179180(float scale, PcpScatterWork8 *work) {
    work->unk3C *= scale;
    work->unk4C *= scale;
    work->unk50 *= scale;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001791A8);

void func_00179438(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;
    u64 temp_v2;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    temp_v1 = effParamTableGetBlock(arg0, 1);
    temp_v2 = effParamTableGetBlock(arg0, 2);
    func_001791A8(temp_v0, temp_v1, temp_v2);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", effPcpScatterLinkedDuplicate);

typedef struct PcpScatterSharedWork {
    u8 pad00[0x70];
    PcpScatterParticles particles;
} PcpScatterSharedWork;

void effPcpScatterReleaseSharedParticles(PcpScatterSharedWork *work) {
    if (work->particles.itemBuffer != 0) {
        u32 count = work->particles.count;
        u32 i;

        for (i = 0; i < count; i++) {
            func_0016A620(work->particles.items[i]);
        }
        func_003297C8(work->particles.itemBuffer);
    }
    func_0017A9C8(work->particles.pool);
    func_003297C8(work->particles.buffer);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00179618);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00179780);

void func_00179D78(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00179D88(PcpScatterWork2 *work, u32 value) {
    work->unk6C = value;
}

void func_00179D90(float scale, PcpScatterWork2 *work) {
    work->unk40 *= scale;
    work->unk44 *= scale;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00179DB0);

void func_0017A058(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;
    u64 temp_v2;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    temp_v1 = effParamTableGetBlock(arg0, 1);
    temp_v2 = effParamTableGetBlock(arg0, 2);
    func_00179DB0(temp_v0, temp_v1, temp_v2);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", effPcpScatterTableDuplicate);

typedef struct PcpScatterLinkedWork {
    u8 pad00[0x58];
    PcpScatterParticles particles;
} PcpScatterLinkedWork;

void effPcpScatterReleaseLinkedParticles(PcpScatterLinkedWork *work) {
    if (work->particles.itemBuffer != 0) {
        u32 count = work->particles.count;
        u32 i;

        for (i = 0; i < count; i++) {
            func_0016A620(work->particles.items[i]);
        }
        func_003297C8(work->particles.itemBuffer);
    }
    func_0017A9C8(work->particles.pool);
    func_003297C8(work->particles.buffer);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017A248);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017A340);

void func_0017A8A0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0017A8B0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x54) = arg1;
}

void func_0017A8B8(void) {
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", effPcpScatterPoolCreate);

void func_0017A9C8(PcpScatterPool *pool) {
    if (pool->sharedResource != NULL) {
        effPcpScatterResRelease(pool->sharedResource);
    }
    func_00333918(pool->resource);
    func_003297C8(pool->buffer);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017AA08);

void func_0017ABE0(PcpScatterPool *pool, u32 resId) {
    u32 resource;

    resource = (u32)effPcpScatterResCreate(resId);
    pool->sharedResource = (PcpScatterRes *)resource;
}

void func_0017AC10(PcpScatterPool *dst, PcpScatterPool *src) {
    u32 resource;

    resource = effPcpScatterResAddRef((u32)src->sharedResource);
    dst->sharedResource = (PcpScatterRes *)resource;
}

s32 func_0017AC40(PcpScatterPool *pool, s32 index) {
    return pool->recordBase + index * 0x60;
}

s32 func_0017AC58(PcpScatterPool *pool, s32 index) {
    return pool->auxRecordBase + index * 0x18;
}

PcpScatterRes *effPcpScatterResCreate(u32 resId)
{
    PcpScatterRes *res;

    res = func_00328D68(8);
    res->unk00 = func_0032C138(resId);
    res->refCount = 1;
    return res;
}

extern void func_0032BBB0(u32);
extern void func_00328E48(void *);

void effPcpScatterResRelease(PcpScatterRes *res) {
    res->refCount--;
    if (res->refCount == 0) {
        func_0032BBB0(res->unk00);
        func_00328E48(res);
    }
}

u32 effPcpScatterResAddRef(u32 handle) {
    PcpScatterRes *resource = (PcpScatterRes *)handle;
    resource->refCount = resource->refCount + 1;
    return handle;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017AD28);

PcpScatterWork4 *func_0017AF40(u64 arg0) {
    u64 first = effParamTableGetBlock(arg0, 0);
    u64 second = effParamTableGetBlock(arg0, 1);

    return func_0017AD28(first, second);
}

PcpScatterWork4 *func_0017AF88(PcpScatterWork4 *work) {
    PcpScatterWork4 *child;

    child = func_0017AD28(&work->unk40, NULL);
    effShareScatterResource(child->scatterObject, work->scatterObject);
    return child;
}

void func_0017AFD0(PcpScatterWork4 *work) {
    effReleaseScatterObject(work->scatterObject);
    func_003297C8(work->ownedBuffer);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017B000);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017B390);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017B520);

void func_0017B718(PcpScatterWork4 *work, void *src) {
    PCP_COPY_VECTOR(&work->unk40, src);
}

void func_0017B730(PcpScatterWork4 *work, f32 value)
{
    work->unk17C = value;
}

void func_0017B738(PcpScatterWork4 *work, u32 value) {
    work->unk180 = value;
}

void effPcpScatterTransformMatrix(u8 *matrix, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_LOAD_MATRIX_B(matrix + 0x50);
    func_00336AA8();
    VU0_STORE_MATRIX(matrix);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017B7A0);

void func_0017B9D0(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    temp_v1 = effParamTableGetBlock(arg0, 1);
    func_0017B7A0(temp_v0, temp_v1);
}

PcpScatterWork5 *func_0017BA18(PcpScatterWork5 *work)
{
    PcpScatterWork5 *child;

    child = func_0017B7A0(&work->unk40, NULL);
    effShareScatterResource(child->scatterObject, work->scatterObject);
    return child;
}

void func_0017BA60(PcpScatterWork5 *work) {
    effReleaseScatterObject(work->scatterObject);
    func_003297C8(work->ownedBuffer);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017BA90);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017BE08);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017BFA8);

void func_0017C250(PcpScatterWork5 *work, void *src) {
    PCP_COPY_VECTOR(&work->unk40, src);
}

void func_0017C268(PcpScatterWork5 *work, f32 value)
{
    work->unk180 = value;
}

void func_0017C270(PcpScatterWork5 *work, u32 value) {
    work->unk184 = value;
}

void func_0017C278(u8 *matrix, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_LOAD_MATRIX_B(matrix + 0x50);
    func_00336AA8();
    VU0_STORE_MATRIX(matrix);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017C2D8);

void func_0017C4D8(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    temp_v1 = effParamTableGetBlock(arg0, 1);
    func_0017C2D8(temp_v0, temp_v1);
}

PcpScatterWork6 *func_0017C520(PcpScatterWork6 *work)
{
    PcpScatterWork6 *child;

    child = func_0017C2D8(&work->unk40, NULL);
    effShareScatterResource(child->scatterObject, work->scatterObject);
    return child;
}

void func_0017C568(PcpScatterWork6 *work) {
    effReleaseScatterObject(work->scatterObject);
    func_003297C8(work->ownedBuffer);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017C598);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017C988);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017CB28);

void func_0017CE00(PcpScatterWork6 *work, void *src) {
    PCP_COPY_VECTOR(&work->unk40, src);
}

void func_0017CE18(PcpScatterWork6 *work, f32 value)
{
    work->unk188 = value;
}

void func_0017CE20(PcpScatterWork6 *work, u32 value) {
    work->unk18C = value;
}

void func_0017CE28(u8 *matrix, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_LOAD_MATRIX_B(matrix + 0x50);
    func_00336AA8();
    VU0_STORE_MATRIX(matrix);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017CE88);

void func_0017D078(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    temp_v1 = effParamTableGetBlock(arg0, 1);
    func_0017CE88(temp_v0, temp_v1);
}

PcpScatterWork7 *func_0017D0C0(PcpScatterWork7 *work)
{
    PcpScatterWork7 *child;

    child = func_0017CE88(&work->unk40, NULL);
    effShareScatterResource(child->scatterObject, work->scatterObject);
    return child;
}

void func_0017D108(PcpScatterWork7 *work) {
    effReleaseScatterObject(work->scatterObject);
    func_003297C8(work->ownedBuffer);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017D138);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017D3D8);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017D560);
