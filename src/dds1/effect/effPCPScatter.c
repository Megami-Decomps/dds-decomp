#include "common.h"
#include "pcp_vu0.h"

/* Shared resource handed between scatter effects. effPcpScatterResCreate creates it,
   effPcpScatterResAddRef takes a reference, effPcpScatterResRelease releases it. */
typedef struct PcpScatterRes PcpScatterRes;

struct PcpScatterRes {
    u32 resourceHandle;
    s32 refCount;
};

extern void *effParamTableGetBlock(void *data, s32 index);

extern void *func_002CFEB8(s32 size);
extern u32 effParamWorkDuplicate(u32 param);
extern u32 func_002D03F8(u32 size);
extern u32 *sdfResourceRetainAddress(u32 handle);
extern void func_002CFF98(void *ptr);

extern void effReleaseScatterObject(u32 res);
extern void sdfQueueAssetRelease(u32 res);
extern void func_002D0918(u32 res);
extern u32 func_002D3288(u32 resId);
extern void sdfTexReleaseReferenceViaHandler(u32 res);
extern void effPcpScatterResRelease(PcpScatterRes *res);
extern PcpScatterRes *effPcpScatterResAddRef(PcpScatterRes *res);

extern void func_001629F0(u32 handle);
extern void func_002DDBF8(void);

/* Effect initializers implemented in assembly below (func_001708A0 lives in
   another unit). Each is entered with and without spawn arguments, so they
   are declared unchecked. */
extern void *func_001708A0();
extern void *func_00171550();
extern void *func_00172158();
extern void *effScatterInstanceCreateB();
extern void *effScatterInstanceCreateC();
extern void *func_00175230();


/* Per-effect work areas. Only the fields touched by the matched spawn,
   teardown and scale helpers are known; the update bodies are still assembly.
   Each work area belongs to the effect whose initializer is noted. */
typedef struct PcpScatterWork1 PcpScatterWork1;
typedef struct PcpScatterWork2 PcpScatterWork2;
typedef struct PcpScatterWork3 PcpScatterWork3;
typedef struct PcpScatterWork4 PcpScatterWork4;
typedef struct PcpScatterWork5 PcpScatterWork5;
typedef struct PcpScatterWork6 PcpScatterWork6;
typedef struct PcpScatterWork7 PcpScatterWork7;
typedef struct PcpScatterWork8 PcpScatterWork8;

extern void effPcpScatterSharePoolResource(PcpScatterWork3 *work, PcpScatterWork3 *src);

/* Pool: `first` words of slot data, then `second` words, then the 0x34-byte control block. */
typedef struct {
    u8 pad00[0x10];
    u32 unk10;
    u32 color14;
    s32 secondWordCount;
    f32 unk1C;
    u32 *firstWords;
    u32 *secondWords;
    void *unk28;
    u32 allocation;
    u32 unk30;
} PcpScatterPool;

extern PcpScatterPool *effPcpScatterPoolCreate(s32 groups);
extern void effPcpScatterCreatePoolResource(PcpScatterWork3 *work, u32 resId);
extern u32 effMiscRand(void *table);
extern u32 effParamWorkCreate(s32 kind, void *params);
extern u8 D_0034DF38[];

extern void effPcpScatterReleasePoolResources(PcpScatterWork3 *work);

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

/* func_00171550: the handle at 0x80 is a resource, not a child work area. */
struct PcpScatterWork8 {
    u8 pad00[0x20];
    u32 unk20;
    u8 pad24[0x18];
    f32 unk3C;
    u32 unk40;
    u32 unk44;
    u32 unk48;
    f32 unk4C;
    f32 unk50;
    u8 pad54[4];
    u8 unk58;
    u8 pad59[7];
    u32 unk60;
    u8 pad64[0xC];
    PcpScatterWork3 *childWork;
    u32 ownedResource;
    u32 duplicatedCount;
    u32 *duplicatedHandles;
    u32 duplicateAllocation;
    u32 unk84;
};

/* func_00172158 */
struct PcpScatterWork2 {
    u8 pad00[0x40];
    f32 unk40;
    f32 unk44;
    u8 pad48[0x10];
    PcpScatterWork3 *unk58;
    u32 unk5C;
    u32 unk60;
    u32 *unk64;
    u32 unk68;
    u32 unk6C;
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

/* effPcpScatterCreateParticleInstance */
struct PcpScatterWork4 {
    u8 pad00[0x40];
    s128 unk40;
    u8 pad50[0x12C];
    f32 unk17C;
    u32 unk180;
    u32 unk184;
    u32 unk188;
};

extern PcpScatterWork4 *effPcpScatterCreateParticleInstance();
extern void effShareScatterResource(u32 param0, u32 param1);

/* effScatterInstanceCreateB */
struct PcpScatterWork5 {
    u8 pad00[0x40];
    s128 unk40;
    u8 pad50[0x130];
    f32 unk180;
    u32 unk184;
    u32 unk188;
    u32 unk18C;
    u32 unk190;
};

/* effScatterInstanceCreateC */
struct PcpScatterWork6 {
    u8 pad00[0x40];
    s128 unk40;
    u8 pad50[0x138];
    f32 unk188;
    u32 unk18C;
    u32 unk190;
    u32 unk194;
    u32 unk198;
};

/* func_00175230 */
struct PcpScatterWork7 {
    u8 pad00[0x40];
    s128 unk40;
    u8 pad50[0xE4];
    u32 unk134;
    u32 unk138;
};



extern PcpScatterRes *effPcpScatterResCreate(u32 resId);

void func_00170B88(void *data)
{
    func_001708A0(effParamTableGetBlock(data, 0), effParamTableGetBlock(data, 1), effParamTableGetBlock(data, 2));
}

PcpScatterWork1 *effPcpScatterSharedDuplicate(src)
    PcpScatterWork1 *src;
{
    PcpScatterWork1 *work;
    u32 count;
    u32 handle;
    u32 *buf;
    u32 i;

    work = func_001708A0(src, 0, 0);
    effPcpScatterSharePoolResource(work->childWork, src->childWork);
    if (work->unk68 != 0) {
        work->duplicatedCount = work->unk20 / work->unk70;
        if (work->unk20 % work->unk70 != 0) {
            work->duplicatedCount = work->duplicatedCount + 1;
        }
        count = work->duplicatedCount;
        handle = func_002D03F8(count * 4);
        buf = sdfResourceRetainAddress(handle);
        work->duplicateAllocation = handle;
        work->duplicatedHandles = buf;
        for (i = 0; i < count; i++) {
            work->duplicatedHandles[i] = effParamWorkDuplicate(*src->duplicatedHandles);
        }
    }
    return work;
}

void effPcpScatterReleaseParticleGroup(PcpScatterWork1 *work)
{
    u32 i;
    u32 count;

    if (work->duplicateAllocation != 0) {
        count = work->duplicatedCount;
        i = 0;
        if (count != 0) {
            do {
                func_001629F0(work->duplicatedHandles[i]);
                i++;
            } while (i < count);
        }
        func_002D0918(work->duplicateAllocation);
    }
    effPcpScatterReleasePoolResources(work->childWork);
    func_002D0918(work->ownedResource);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00170D68);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00170F28);

void func_00171510(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00171520(PcpScatterWork8 *work, u32 *values)
{
    work->duplicatedHandles = values;
}

void func_00171528(f32 scale, PcpScatterWork8 *work)
{
    work->unk3C *= scale;
    work->unk4C *= scale;
    work->unk50 *= scale;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00171550);

void func_001717E0(void *data)
{
    func_00171550(effParamTableGetBlock(data, 0), effParamTableGetBlock(data, 1), effParamTableGetBlock(data, 2));
}

PcpScatterWork8 *effPcpScatterLinkedDuplicate(src)
    PcpScatterWork8 *src;
{
    PcpScatterWork8 *work;
    u32 count;
    u32 handle;
    u32 *buf;
    u32 i;

    work = func_00171550(src, 0, 0);
    effPcpScatterSharePoolResource(work->childWork, src->childWork);
    if (work->unk58 != 0) {
        work->duplicatedCount = work->unk20 / work->unk60;
        if (work->unk20 % work->unk60 != 0) {
            work->duplicatedCount = work->duplicatedCount + 1;
        }
        count = work->duplicatedCount;
        handle = func_002D03F8(count * 4);
        buf = sdfResourceRetainAddress(handle);
        work->duplicateAllocation = handle;
        work->duplicatedHandles = buf;
        for (i = 0; i < count; i++) {
            work->duplicatedHandles[i] = effParamWorkDuplicate(*src->duplicatedHandles);
        }
    }
    return work;
}

void effPcpScatterReleaseSharedParticles(PcpScatterWork8 *work)
{
    u32 i;
    u32 count;

    if (work->duplicateAllocation != 0) {
        count = work->duplicatedCount;
        i = 0;
        if (count != 0) {
            do {
                func_001629F0(work->duplicatedHandles[i]);
                i++;
            } while (i < count);
        }
        func_002D0918(work->duplicateAllocation);
    }
    effPcpScatterReleasePoolResources(work->childWork);
    func_002D0918(work->ownedResource);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001719C0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00171B28);

void func_00172120(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00172130(PcpScatterWork2 *work, u32 value)
{
    work->unk6C = value;
}

void func_00172138(f32 scale, PcpScatterWork2 *work)
{
    work->unk40 *= scale;
    work->unk44 *= scale;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00172158);

void func_00172400(void *data)
{
    func_00172158(effParamTableGetBlock(data, 0), effParamTableGetBlock(data, 1), effParamTableGetBlock(data, 2));
}

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

PcpScatterWork2Copy *effPcpScatterTableDuplicate(src)
    PcpScatterWork2Copy *src;
{
    PcpScatterWork2Copy *work;
    u32 count;
    u32 handle;
    u32 *buf;
    u32 i;

    work = func_00172158(src, 0, 0);
    effPcpScatterSharePoolResource(work->unk58, src->unk58);
    if (work->unk40 != 0) {
        if (work->unk48 == 0) {
            work->unk48 = 1;
        }
        work->unk60 = work->unk20 / work->unk48;
        if (work->unk20 % work->unk48 != 0) {
            work->unk60 = work->unk60 + 1;
        }
        count = work->unk60;
        handle = func_002D03F8(count * 4);
        buf = sdfResourceRetainAddress(handle);
        work->unk68 = handle;
        work->unk64 = buf;
        for (i = 0; i < count; i++) {
            work->unk64[i] = effParamWorkDuplicate(*src->unk64);
        }
    }
    return work;
}

void effPcpScatterReleaseLinkedParticles(PcpScatterWork2 *work)
{
    u32 i;
    u32 count;

    if (work->unk68 != 0) {
        count = work->unk60;
        i = 0;
        if (count != 0) {
            do {
                func_001629F0(work->unk64[i]);
                i++;
            } while (i < count);
        }
        func_002D0918(work->unk68);
    }
    effPcpScatterReleasePoolResources(work->unk58);
    func_002D0918(work->unk5C);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001725F0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001726E8);

void func_00172C48(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00172C58(PcpScatterWork3 *work, u32 value)
{
    work->unk54 = value;
}

void func_00172C60(void)
{
}

extern void *memset(void *dst, s32 value, u32 size);
extern void *func_002DA730(void);
extern void func_002DA420(void *obj, f32 value);
extern u8 D_003D6580[0x2C];

PcpScatterPool *effPcpScatterPoolCreate(s32 groups) {
    PcpScatterPool *pool;
    u32 handle;
    u32 *block;
    s32 slots;
    s32 first;
    s32 second;
    u32 size;

    slots = groups * 3;
    first = slots * 8;
    second = slots * 2;
    size = (first + second) * 4 + 0x34;
    handle = func_002D03F8(size);
    block = sdfResourceRetainAddress(handle);
    memset(block, 0, size);
    pool = (PcpScatterPool *)(block + (first + second));
    pool->firstWords = block;
    pool->unk10 = 1;
    pool->secondWords = block + first;
    pool->secondWordCount = second;
    pool->allocation = handle;
    pool->unk1C = 1.0f;
    pool->color14 = 0x80808080;
    pool->unk30 = 0;
    pool->unk28 = func_002DA730();
    func_002DA420(pool->unk28, 1.0f);
    memset(D_003D6580, 0, 0x2C);
    *(u16 *)(D_003D6580 + 4) = 0x4000;
    return pool;
}

void effPcpScatterReleasePoolResources(PcpScatterWork3 *work)
{
    if (work->res != NULL) {
        effPcpScatterResRelease(work->res);
    }
    sdfQueueAssetRelease(work->unk28);
    func_002D0918(work->unk2C);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00172DB0);

void effPcpScatterCreatePoolResource(PcpScatterWork3 *work, u32 resId)
{
    PcpScatterRes *res;

    res = effPcpScatterResCreate(resId);
    work->res = res;
}

void effPcpScatterSharePoolResource(PcpScatterWork3 *work, PcpScatterWork3 *src)
{
    PcpScatterRes *res;

    res = effPcpScatterResAddRef(src->res);
    work->res = res;
}

s32 effPcpScatterGetRecordAddress(PcpScatterWork3 *work, s32 index)
{
    return work->unk20 + index * 0x60;
}

s32 effPcpScatterGetAuxRecordAddress(PcpScatterWork3 *work, s32 index)
{
    return work->unk24 + index * 0x18;
}

PcpScatterRes *effPcpScatterResCreate(u32 resId)
{
    PcpScatterRes *res;

    res = func_002CFEB8(8);
    res->resourceHandle = func_002D3288(resId);
    res->refCount = 1;
    return res;
}

void effPcpScatterResRelease(PcpScatterRes *res)
{
    if (--res->refCount == 0) {
        sdfTexReleaseReferenceViaHandler(res->resourceHandle);
        func_002CFF98(res);
    }
}

PcpScatterRes *effPcpScatterResAddRef(PcpScatterRes *res)
{
    res->refCount++;
    return res;
}

typedef struct PcpScatterParams {
    u8 pad00[0x10];
    f32 matrix[16];
    u32 unk50;
    u8 pad54[0x08];
    u32 particleCount;
    u32 unk60;
    u32 unk64;
    u8 pad68[0xD0];
} PcpScatterParams;

typedef struct PcpScatterParticle {
    u8 pad00[0x08];
    s32 unk08;
    u8 pad0C[0x1C];
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

extern void *func_00175B50();
extern void effCreateScatterResource(void *object, u32 resource);

/* Allocate particles after the scatter work, then assign randomized offsets. */
PcpScatterWork4 *effPcpScatterCreateParticleInstance(src, resource)
    PcpScatterParams *src;
    u32 resource;
{
    u32 allocation = func_002D03F8(src->particleCount * 0x28 + 0x18C);
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
    object = func_00175B50(src->particleCount, src->unk60);
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
        particle->unk08 = -(effMiscRand(D_0034DF38) % mod);
        particle++;
    }
    return (PcpScatterWork4 *)inst;
}

void effScatterBlockDuplicate(void *data)
{
    effPcpScatterCreateParticleInstance(effParamTableGetBlock(data, 0), effParamTableGetBlock(data, 1));
}

PcpScatterWork4 *func_00173330(PcpScatterWork4 *work) {
    PcpScatterWork4 *child;

    child = effPcpScatterCreateParticleInstance(&work->unk40, NULL);
    effShareScatterResource(child->unk184, work->unk184);
    return child;
}

void func_00173378(PcpScatterWork4 *work)
{
    effReleaseScatterObject(work->unk184);
    func_002D0918(work->unk188);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001733A8);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00173738);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001738C8);

void func_00173AC0(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x40, src);
}

void func_00173AD8(PcpScatterWork4 *work, f32 value)
{
    work->unk17C = value;
}

void func_00173AE0(PcpScatterWork4 *work, u32 value)
{
    work->unk180 = value;
}

/* vu0 routine: matrix = (matrix + 0x50) * src, via the vf28-vf31 by vf24-vf27 product routine */
void effPcpScatterTransformMatrix(PcpScatterWork4 *work, void *source)
{
    VU0_LOAD_MATRIX(source);
    VU0_LOAD_MATRIX_B(&work->pad50[0]);
    func_002DDBF8();
    VU0_STORE_MATRIX(work);
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
    u32 allocation = func_002D03F8(src->particleCount * 0x28 + 0x194);
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
    object = func_00175B50(src->particleCount, src->unk60);
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
        particle->unk08 = -(effMiscRand(D_0034DF38) % mod);
        particle++;
    }
    return inst;
}

void func_00173D78(void *data)
{
    effScatterInstanceCreateB(effParamTableGetBlock(data, 0), effParamTableGetBlock(data, 1));
}

PcpScatterWork5 *func_00173DC0(PcpScatterWork5 *work)
{
    PcpScatterWork5 *child;

    child = effScatterInstanceCreateB(&work->unk40, NULL);
    effShareScatterResource(child->unk18C, work->unk18C);
    return child;
}

void func_00173E08(PcpScatterWork5 *work)
{
    effReleaseScatterObject(work->unk18C);
    func_002D0918(work->unk190);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00173E38);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001741B0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00174350);

void func_001745F8(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x40, src);
}

void func_00174610(PcpScatterWork5 *work, f32 value)
{
    work->unk180 = value;
}

void func_00174618(PcpScatterWork5 *work, u32 value)
{
    work->unk184 = value;
}

/* vu0 routine: matrix = (matrix + 0x50) * src, via the vf28-vf31 by vf24-vf27 product routine */
void func_00174620(PcpScatterWork5 *work, void *source)
{
    VU0_LOAD_MATRIX(source);
    VU0_LOAD_MATRIX_B(&work->pad50[0]);
    func_002DDBF8();
    VU0_STORE_MATRIX(work);
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
    u32 allocation = func_002D03F8(src->particleCount * 0x28 + 0x19C);
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
    object = func_00175B50(src->particleCount, src->unk60);
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
        particle->unk08 = -(effMiscRand(D_0034DF38) % mod);
        particle++;
    }
    return inst;
}

void func_00174880(void *data)
{
    effScatterInstanceCreateC(effParamTableGetBlock(data, 0), effParamTableGetBlock(data, 1));
}

PcpScatterWork6 *func_001748C8(PcpScatterWork6 *work)
{
    PcpScatterWork6 *child;

    child = effScatterInstanceCreateC(&work->unk40, NULL);
    effShareScatterResource(child->unk194, work->unk194);
    return child;
}

void func_00174910(PcpScatterWork6 *work)
{
    effReleaseScatterObject(work->unk194);
    func_002D0918(work->unk198);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00174940);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00174D30);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00174ED0);

void func_001751A8(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x40, src);
}

void func_001751C0(PcpScatterWork6 *work, f32 value)
{
    work->unk188 = value;
}

void func_001751C8(PcpScatterWork6 *work, u32 value)
{
    work->unk18C = value;
}

/* vu0 routine: matrix = (matrix + 0x50) * src, via the vf28-vf31 by vf24-vf27 product routine */
void func_001751D0(PcpScatterWork6 *work, void *source)
{
    VU0_LOAD_MATRIX(source);
    VU0_LOAD_MATRIX_B(&work->pad50[0]);
    func_002DDBF8();
    VU0_STORE_MATRIX(work);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00175230);

void func_00175420(void *data)
{
    func_00175230(effParamTableGetBlock(data, 0), effParamTableGetBlock(data, 1));
}

PcpScatterWork7 *func_00175468(PcpScatterWork7 *work)
{
    PcpScatterWork7 *child;

    child = func_00175230(&work->unk40, NULL);
    effShareScatterResource(child->unk134, work->unk134);
    return child;
}

void func_001754B0(PcpScatterWork7 *work)
{
    effReleaseScatterObject(work->unk134);
    func_002D0918(work->unk138);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001754E0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00175780);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00175908);
