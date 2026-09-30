#include "common.h"

#include "pcp_vu0.h"

extern void func_00336AA8(void);

extern u64 effParamTableGetBlock(u64, u64);

extern u32 effPcpScatterResAddRef(u32);

/* Shared resource handed between scatter effects. func_00173018 creates it,
   func_001730B8 takes a reference, func_00173068 releases it. */
typedef struct PcpScatterRes PcpScatterRes;

struct PcpScatterRes {
    u32 resourceHandle;
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

extern PcpScatterWork4 *effPcpScatterCreateParticleInstance();

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

extern void *effScatterInstanceCreateB();

extern void *effScatterInstanceCreateC();

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
    u32 resource;             /* 0x28: released by sdfQueueAssetRelease */
    u32 buffer;               /* 0x2C: freed by func_003297C8 */
    PcpScatterRes *sharedResource; /* 0x30: reference counted */
} PcpScatterPool;

/* Spawn a scatter variant from three parameter-table blocks. */
/* Shared resource handed between scatter effects. effPcpScatterResCreate creates it,
   effPcpScatterResAddRef takes a reference, effPcpScatterResRelease releases it. */
typedef struct PcpScatterRes PcpScatterRes;

extern u32 effParamWorkDuplicate(u32 param);

extern u32 func_003292A8(u32 size);

extern u32 *sdfResourceRetainAddress(u32 handle);

/* Effect initializers implemented in assembly below (func_001708A0 lives in
   another unit). Each is entered with and without spawn arguments, so they
   are declared unchecked. */
extern void *func_001784F8();

/* Per-effect work areas. Only the fields touched by the matched spawn,
   teardown and scale helpers are known; the update bodies are still assembly.
   Each work area belongs to the effect whose initializer is noted. */
typedef struct PcpScatterWork1 PcpScatterWork1;

typedef struct PcpScatterWork3 PcpScatterWork3;

/* func_001708A0 */
struct PcpScatterWork1 {
    u8 pad00[0x20];
    u32 unk20;
    u8 pad24[0x18];
    f32 unk3C;
    u32 unk40;
    u32 unk44;
    u32 unk48;
    f32 unk4C;
    f32 unk50;
    u8 pad54[0x14];
    u8 unk68;
    u8 pad69[7];
    u32 unk70;
    u8 pad74[0xC];
    PcpScatterWork3 *childWork;
    u32 ownedResource;
    u32 duplicatedCount;
    u32 *duplicatedHandles;
    u32 duplicateAllocation;
};

/* Resource-holding effect around effPcpScatterCreatePoolResource */
struct PcpScatterWork3 {
    u8 pad00[0x20];
    s32 unk20;
    s32 unk24;
    u32 unk28;
    u32 unk2C;
    PcpScatterRes *res;
    u8 pad34[0x20];
    u32 unk54;
};

extern void *func_00179DB0();

/* Work2 as seen by the copy constructor: byte 0x40 is a flag here (func_00172138 scales
   the same word as a float). */
typedef struct {
    u8 pad00[0x20];
    u32 unk20;
    u8 pad24[0x1C];
    u8 unk40;
    u8 pad41[7];
    u32 unk48;
    u8 pad4C[0xC];
    PcpScatterWork3 *unk58;
    u32 unk5C;
    u32 unk60;
    u32 *unk64;
    u32 unk68;
} PcpScatterWork2Copy;

void func_001787E0(u64 table) {
    u64 params;
    u64 resource;
    u64 options;

    params = effParamTableGetBlock(table, 0);
    resource = effParamTableGetBlock(table, 1);
    options = effParamTableGetBlock(table, 2);
    func_001784F8(params, resource, options);
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
    effPcpScatterReleasePoolResources(work->particles.pool);
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

/* Spawn the linked variant from its three parameter-table blocks. */
void func_00179438(u64 table) {
    u64 params;
    u64 resource;
    u64 options;

    params = effParamTableGetBlock(table, 0);
    resource = effParamTableGetBlock(table, 1);
    options = effParamTableGetBlock(table, 2);
    func_001791A8(params, resource, options);
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
    effPcpScatterReleasePoolResources(work->particles.pool);
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

/* Spawn the table variant from its three parameter-table blocks. */
void func_0017A058(u64 table) {
    u64 params;
    u64 resource;
    u64 options;

    params = effParamTableGetBlock(table, 0);
    resource = effParamTableGetBlock(table, 1);
    options = effParamTableGetBlock(table, 2);
    func_00179DB0(params, resource, options);
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
    effPcpScatterReleasePoolResources(work->particles.pool);
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

void effPcpScatterReleasePoolResources(PcpScatterPool *pool) {
    if (pool->sharedResource != NULL) {
        effPcpScatterResRelease(pool->sharedResource);
    }
    sdfQueueAssetRelease(pool->resource);
    func_003297C8(pool->buffer);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017AA08);

void effPcpScatterCreatePoolResource(PcpScatterPool *pool, u32 resId) {
    u32 resource;

    resource = (u32)effPcpScatterResCreate(resId);
    pool->sharedResource = (PcpScatterRes *)resource;
}

void effPcpScatterSharePoolResource(PcpScatterPool *dst, PcpScatterPool *src) {
    u32 resource;

    resource = effPcpScatterResAddRef((u32)src->sharedResource);
    dst->sharedResource = (PcpScatterRes *)resource;
}

s32 effPcpScatterGetRecordAddress(PcpScatterPool *pool, s32 index) {
    return pool->recordBase + index * 0x60;
}

s32 effPcpScatterGetAuxRecordAddress(PcpScatterPool *pool, s32 index) {
    return pool->auxRecordBase + index * 0x18;
}

PcpScatterRes *effPcpScatterResCreate(u32 resId)
{
    PcpScatterRes *res;

    res = func_00328D68(8);
    res->resourceHandle = func_0032C138(resId);
    res->refCount = 1;
    return res;
}

extern void sdfTexReleaseReferenceViaHandler(u32);

extern void func_00328E48(void *);

void effPcpScatterResRelease(PcpScatterRes *res) {
    res->refCount--;
    if (res->refCount == 0) {
        sdfTexReleaseReferenceViaHandler(res->resourceHandle);
        func_00328E48(res);
    }
}

u32 effPcpScatterResAddRef(u32 handle) {
    PcpScatterRes *resource = (PcpScatterRes *)handle;
    resource->refCount = resource->refCount + 1;
    return handle;
}

typedef struct PcpScatterParams {
    u8 pad00[0x10];
    f32 matrix[16];
    u32 unk50;
    u8 pad54[0x04];
    s32 unk58;
    u32 particleCount;
    u32 unk60;
    u32 unk64;
    u8 pad68[0x08];
    f32 unk70;
    f32 unk74;
    f32 unk78;
    f32 unk7C;
    f32 unk80;
    f32 unk84;
    f32 unk88;
    u8 pad8C[0x08];
    f32 unk94;
    u8 pad98[0x04];
    f32 unk9C;
    f32 unkA0;
    f32 unkA4;
    f32 unkA8;
    u8 padAC[0x04];
    u32 unkB0;
    u32 unkB4;
    u8 padB8[0x80];
} PcpScatterParams;

typedef struct PcpScatterParticle {
    f32 unk00;
    f32 unk04;
    s32 unk08;
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
} PcpScatterParticle;

typedef struct PcpScatterInstance {
    f32 matrix[16];
    PcpScatterParams params;
    PcpScatterParticle *particles;
    f32 unk17C;
    u32 unk180;
    u32 scatterObject;
    u32 ownedBuffer;
} PcpScatterInstance;

extern void *func_0017D7A8();

extern void effCreateScatterResource(void *object, u32 resource);

extern u32 effMiscRand(void *state);

extern u8 D_003AA868[];

/* Allocate particles after the scatter work, then assign randomized offsets. */
PcpScatterWork4 *effPcpScatterCreateParticleInstance(src, resource)
    PcpScatterParams *src;

    u32 resource;

{
    u32 allocation = func_003292A8(src->particleCount * 0x28 + 0x18C);
    PcpScatterInstance *inst = (PcpScatterInstance *)sdfResourceRetainAddress(allocation);
    PcpScatterParticle *particle;
    u32 mod;
    u32 count;
    u32 i;
    void *object;
    u32 unk50;

    particle = (PcpScatterParticle *)((u8 *)inst + 0x18C);
    inst->params = *src;
    inst->unk180 = 0x80808080;
    inst->unk17C = 1.0f;
    inst->ownedBuffer = allocation;
    inst->particles = particle;
    VU0_COPY_MATRIX(inst->matrix, src->matrix);
    object = func_0017D7A8(src->particleCount, src->unk60);
    unk50 = src->unk50;
    inst->scatterObject = (u32)object;
    *(u32 *)((u8 *)object + 0x50) = unk50;
    if (resource != 0) {
        effCreateScatterResource(object, resource);
    }
    mod = inst->params.unk64;
    count = inst->params.particleCount;
    if ((s32)mod <= 0) {
        mod = 1;
    }
    for (i = 0; i < count; i++) {
        particle->unk08 = -(effMiscRand(D_003AA868) % mod);
        particle++;
    }
    return (PcpScatterWork4 *)inst;
}

PcpScatterWork4 *effScatterBlockDuplicate(u64 arg0) {
    u64 first = effParamTableGetBlock(arg0, 0);
    u64 second = effParamTableGetBlock(arg0, 1);

    return effPcpScatterCreateParticleInstance(first, second);
}

PcpScatterWork4 *func_0017AF88(PcpScatterWork4 *work) {
    PcpScatterWork4 *child;

    child = effPcpScatterCreateParticleInstance(&work->unk40, NULL);
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

/* vu0 routine: matrix = (matrix + 0x50) * src, via the vf28-vf31 by vf24-vf27 product routine */
void effPcpScatterTransformMatrix(u8 *matrix, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_LOAD_MATRIX_B(matrix + 0x50);
    func_00336AA8();
    VU0_STORE_MATRIX(matrix);
}

typedef struct PcpScatterParamsB {
    u8 pad00[0x10];
    f32 matrix[16];
    u32 unk50;
    u8 pad54[0x08];
    u32 particleCount;
    u32 unk60;
    u32 unk64;
    u8 pad68[0xD4];
} PcpScatterParamsB;

typedef struct PcpScatterInstanceB {
    f32 matrix[16];
    PcpScatterParamsB params;
    PcpScatterParticle *particles;
    f32 unk180;
    u32 unk184;
    u32 unk188;
    u32 scatterObject;
    u32 ownedBuffer;
} PcpScatterInstanceB;

/* Same particle layout with a longer parameter block and one extra control word. */
void *effScatterInstanceCreateB(src, resource)
    PcpScatterParamsB *src;

    u32 resource;

{
    u32 allocation = func_003292A8(src->particleCount * 0x28 + 0x194);
    PcpScatterInstanceB *inst = (PcpScatterInstanceB *)sdfResourceRetainAddress(allocation);
    PcpScatterParticle *particle;
    u32 mod;
    u32 count;
    u32 i;
    void *object;
    u32 unk50;
    s32 limit;

    particle = (PcpScatterParticle *)((u8 *)inst + 0x194);
    inst->params = *src;
    inst->unk184 = 0x80808080;
    inst->unk180 = 1.0f;
    inst->ownedBuffer = allocation;
    inst->particles = particle;
    inst->unk188 = 0;
    VU0_COPY_MATRIX(inst->matrix, src->matrix);
    object = func_0017D7A8(src->particleCount, src->unk60);
    unk50 = src->unk50;
    inst->scatterObject = (u32)object;
    *(u32 *)((u8 *)object + 0x50) = unk50;
    if (resource != 0) {
        effCreateScatterResource(object, resource);
    }
    limit = inst->params.unk64;
    if (limit <= 0) {
        inst->params.unk64 = 1;
        limit = 1;
    }
    mod = limit;
    count = inst->params.particleCount;
    for (i = 0; i < count; i++) {
        particle->unk08 = -(effMiscRand(D_003AA868) % mod);
        particle++;
    }
    return inst;
}

void func_0017B9D0(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    temp_v1 = effParamTableGetBlock(arg0, 1);
    effScatterInstanceCreateB(temp_v0, temp_v1);
}

PcpScatterWork5 *func_0017BA18(PcpScatterWork5 *work)
{
    PcpScatterWork5 *child;

    child = effScatterInstanceCreateB(&work->unk40, NULL);
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

/* vu0 routine: matrix = (matrix + 0x50) * src, via the vf28-vf31 by vf24-vf27 product routine */
void func_0017C278(u8 *matrix, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_LOAD_MATRIX_B(matrix + 0x50);
    func_00336AA8();
    VU0_STORE_MATRIX(matrix);
}

typedef struct PcpScatterParamsC {
    u8 pad00[0x10];
    f32 matrix[16];
    u32 unk50;
    u8 pad54[0x08];
    u32 particleCount;
    u32 unk60;
    u32 unk64;
    u8 pad68[0xDC];
} PcpScatterParamsC;

typedef struct PcpScatterInstanceC {
    f32 matrix[16];
    PcpScatterParamsC params;
    PcpScatterParticle *particles;
    f32 unk188;
    u32 unk18C;
    u32 unk190;
    u32 scatterObject;
    u32 ownedBuffer;
} PcpScatterInstanceC;

/* Third particle variant has another eight bytes of per-instance state. */
void *effScatterInstanceCreateC(src, resource)
    PcpScatterParamsC *src;

    u32 resource;

{
    u32 allocation = func_003292A8(src->particleCount * 0x28 + 0x19C);
    PcpScatterInstanceC *inst = (PcpScatterInstanceC *)sdfResourceRetainAddress(allocation);
    PcpScatterParticle *particle;
    u32 mod;
    u32 count;
    u32 i;
    void *object;
    u32 unk50;
    s32 limit;

    particle = (PcpScatterParticle *)((u8 *)inst + 0x19C);
    inst->params = *src;
    inst->unk18C = 0x80808080;
    inst->unk188 = 1.0f;
    inst->ownedBuffer = allocation;
    inst->particles = particle;
    inst->unk190 = 0;
    VU0_COPY_MATRIX(inst->matrix, src->matrix);
    object = func_0017D7A8(src->particleCount, src->unk60);
    unk50 = src->unk50;
    inst->scatterObject = (u32)object;
    *(u32 *)((u8 *)object + 0x50) = unk50;
    if (resource != 0) {
        effCreateScatterResource(object, resource);
    }
    limit = inst->params.unk64;
    if (limit <= 0) {
        inst->params.unk64 = 1;
        limit = 1;
    }
    mod = limit;
    count = inst->params.particleCount;
    for (i = 0; i < count; i++) {
        particle->unk08 = -(effMiscRand(D_003AA868) % mod);
        particle++;
    }
    return inst;
}

void func_0017C4D8(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    temp_v1 = effParamTableGetBlock(arg0, 1);
    effScatterInstanceCreateC(temp_v0, temp_v1);
}

PcpScatterWork6 *func_0017C520(PcpScatterWork6 *work)
{
    PcpScatterWork6 *child;

    child = effScatterInstanceCreateC(&work->unk40, NULL);
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

/* vu0 routine: matrix = (matrix + 0x50) * src, via the vf28-vf31 by vf24-vf27 product routine */
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
