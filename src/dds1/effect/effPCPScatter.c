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
extern void sdfReleaseChipBlock(void *ptr);

extern void effReleaseScatterObject(u32 res);
extern void sdfQueueAssetRelease(u32 res);
extern void func_002D0918(u32 res);
extern u32 func_002D3288(u32 resId);
extern void sdfTexReleaseReferenceViaHandler(u32 res);
extern void effPcpScatterResRelease(PcpScatterRes *res);
extern PcpScatterRes *effPcpScatterResAddRef(PcpScatterRes *res);

extern void func_001629F0(u32 handle);
extern void sdfComposeVuMatrixFromRegisters(void);

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
extern f32 effMiscRandUnitFloat(void *state);
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32 angle);
extern f32 sdfSinPoly(f32 angle);
extern void func_002DD608(f32 angle);
extern void func_002DD968(f32 angle);
extern void sdfMultiplyVuMatrixInPlace(void);
extern s32 effGetScatterWideBlock(u32 object, s32 index);
extern s32 effGetScatterNarrowBlock(u32 object, s32 index);
extern void vu0RotMatrixXYZFromVec3(f32 *rot);

extern void effPcpScatterReleasePoolResources(PcpScatterWork3 *work);

/* func_001708A0 */
struct PcpScatterWork1 {
    u8 pad00[0x20];
    u32 particleCount;
    u8 pad24[0x18];
    f32 unk3C;
    u32 unk40;
    u32 unk44;
    u32 unk48;
    f32 unk4C;
    f32 unk50;
    u8 pad54[0x14];
    u8 duplicateParticles;
    u8 pad69[7];
    u32 particlesPerGroup;
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
    u32 particleCount;
    u8 pad24[0x18];
    f32 unk3C;
    u32 unk40;
    u32 unk44;
    u32 unk48;
    f32 unk4C;
    f32 unk50;
    u8 pad54[4];
    u8 duplicateParticles;
    u8 pad59[7];
    u32 particlesPerGroup;
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
    PcpScatterWork3 *childWork;
    u32 ownedResource;
    u32 duplicatedCount;
    u32 *duplicatedHandles;
    u32 duplicateAllocation;
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
    s128 particleParams;
    u8 pad50[0x12C];
    f32 scale;
    u32 color;
    u32 scatterObject;
    u32 ownedBuffer;
};

extern PcpScatterWork4 *effPcpScatterCreateParticleInstance();
extern void effShareScatterResource(u32 param0, u32 param1);

/* effScatterInstanceCreateB */
struct PcpScatterWork5 {
    u8 pad00[0x40];
    s128 particleParams;
    u8 pad50[0x130];
    f32 scale;
    u32 color;
    u32 unk188;
    u32 scatterObject;
    u32 ownedBuffer;
};

/* effScatterInstanceCreateC */
struct PcpScatterWork6 {
    u8 pad00[0x40];
    s128 particleParams;
    u8 pad50[0x138];
    f32 scale;
    u32 color;
    u32 unk190;
    u32 scatterObject;
    u32 ownedBuffer;
};

/* func_00175230 */
struct PcpScatterWork7 {
    u8 pad00[0x40];
    s128 particleParams;
    u8 pad50[0xE4];
    u32 scatterObject;
    u32 ownedBuffer;
};



extern PcpScatterRes *effPcpScatterResCreate(u32 resId);

void effScatterCreateFromParameterTriplet(void *data)
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
    if (work->duplicateParticles != 0) {
        work->duplicatedCount = work->particleCount / work->particlesPerGroup;
        if (work->particleCount % work->particlesPerGroup != 0) {
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

void effSetScatterDuplicatedHandles(PcpScatterWork8 *work, u32 *values)
{
    work->duplicatedHandles = values;
}

void effScatterScaleParticleValues(f32 scale, PcpScatterWork8 *work)
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
    if (work->duplicateParticles != 0) {
        work->duplicatedCount = work->particleCount / work->particlesPerGroup;
        if (work->particleCount % work->particlesPerGroup != 0) {
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

typedef struct PcpScatterSprite {
    u32 age;
    f32 radiusStep;
    f32 initialSize;
    f32 startRadius;
    f32 spinAngle;
    f32 dirX;
    f32 dirY;
    f32 dirZ;
} PcpScatterSprite;

typedef struct PcpScatterWork10 {
    u8 pad00[0x30];
    s32 lifetime;
    u8 pad34[4];
    f32 initialSize;
    u8 pad3C[4];
    f32 startRadius;
    f32 endRadius;
    f32 startRadiusJitter;
    f32 endRadiusJitter;
    u8 pad50[0x14];
    PcpScatterSprite *sprites;
} PcpScatterWork10;

/* Init sprite `index`: random spin, jittered start and end distances, and a random unit direction. */
void effScatterSpriteSpawn(PcpScatterWork10 *work, s32 index)
{
    PcpScatterSprite *sprite = &work->sprites[index];
    f32 dir[4];
    f32 jitter;

    sprite->spinAngle = effMiscRandUnitFloat(D_0034DF38) * (3.14159265f * 2.0f);
    jitter = work->startRadiusJitter;
    sprite->startRadius = work->startRadius * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter));
    jitter = work->endRadiusJitter;
    sprite->radiusStep = (work->endRadius * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter)) - sprite->startRadius) / (f32)work->lifetime;
    sprite->initialSize = work->initialSize;
    dir[0] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    dir[1] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    dir[2] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, dir);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, dir);
    sprite->age = 0;
    sprite->dirX = dir[0];
    sprite->dirY = dir[1];
    sprite->dirZ = dir[2];
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00171B28);

void func_00172120(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00172130(PcpScatterWork2 *work, u32 value)
{
    work->unk6C = value;
}

void effScatterScalePair(f32 scale, PcpScatterWork2 *work)
{
    work->unk40 *= scale;
    work->unk44 *= scale;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00172158);

void func_00172400(void *data)
{
    func_00172158(effParamTableGetBlock(data, 0), effParamTableGetBlock(data, 1), effParamTableGetBlock(data, 2));
}

/* Work2 as seen by the copy constructor: byte 0x40 is a flag here (effScatterScalePair scales
   the same word as a float). */
typedef struct {
    u8 pad00[0x20];
    u32 particleCount;
    u8 pad24[0x1C];
    u8 duplicateParticles;
    u8 pad41[7];
    u32 particlesPerGroup;
    u8 pad4C[0xC];
    PcpScatterWork3 *childWork;
    u32 ownedResource;
    u32 duplicatedCount;
    u32 *duplicatedHandles;
    u32 duplicateAllocation;
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
    effPcpScatterSharePoolResource(work->childWork, src->childWork);
    if (work->duplicateParticles != 0) {
        if (work->particlesPerGroup == 0) {
            work->particlesPerGroup = 1;
        }
        work->duplicatedCount = work->particleCount / work->particlesPerGroup;
        if (work->particleCount % work->particlesPerGroup != 0) {
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

void effPcpScatterReleaseLinkedParticles(PcpScatterWork2 *work)
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
extern void *sdfCreateAssetWithDrawEntries(void);
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
    pool->unk28 = sdfCreateAssetWithDrawEntries();
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
        sdfReleaseChipBlock(res);
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
    u32 randomDelayRange;
    u8 pad68[0xD0];
} PcpScatterParams;

typedef struct PcpScatterParticle {
    u8 pad00[0x08];
    s32 age;
    u8 pad0C[0x1C];
} PcpScatterParticle;

typedef struct PcpScatterInstance {
    f32 matrix[16];
    PcpScatterParams params;
    PcpScatterParticle *particles;
    f32 scale;
    u32 color;
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
    inst->color = 0x80808080;
    inst->scale = 1.0f;
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
    mod = inst->params.randomDelayRange;
    count = inst->params.particleCount;
    if ((s32)mod <= 0) {
        mod = 1;
    }
    for (i = 0; i < count; i++) {
        particle->age = -(effMiscRand(D_0034DF38) % mod);
        particle++;
    }
    return (PcpScatterWork4 *)inst;
}

void effScatterBlockDuplicate(void *data)
{
    effPcpScatterCreateParticleInstance(effParamTableGetBlock(data, 0), effParamTableGetBlock(data, 1));
}

PcpScatterWork4 *effScatterCloneWithSharedObject(PcpScatterWork4 *work) {
    PcpScatterWork4 *child;

    child = effPcpScatterCreateParticleInstance(&work->particleParams, NULL);
    effShareScatterResource(child->scatterObject, work->scatterObject);
    return child;
}

void effScatterReleaseObjectAndBuffer(PcpScatterWork4 *work)
{
    effReleaseScatterObject(work->scatterObject);
    func_002D0918(work->ownedBuffer);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001733A8);

typedef struct PcpScatterRing {
    f32 unk00;
    f32 tiltAngle;
    u8 pad08[4];
    f32 rise;
    f32 angle;
    f32 tiltSpeed;
    f32 angleStep;
    f32 radius;
    f32 radiusStep;
    u8 pad24[4];
} PcpScatterRing;

typedef struct PcpScatterBlockObject {
    u8 pad00[0x5C];
    s32 stride;
} PcpScatterBlockObject;

typedef struct PcpScatterWork11 {
    u8 pad00[0xCC];
    f32 riseDecay;
    u8 padD0[8];
    f32 tiltDamping;
    u8 padDC[0x9C];
    PcpScatterRing *rings;
    u8 pad17C[8];
    u32 scatterObject;
} PcpScatterWork11;

/* Advance ring `index`: rebuild the rotation matrix, then lay the ring's vertex pairs around it. */
void effScatterRingUpdate(PcpScatterWork11 *work, s32 index)
{
    f32 *vertex = (f32 *)effGetScatterWideBlock(work->scatterObject, index);
    PcpScatterRing *ring;
    f32 radius;
    f32 rise;
    f32 angle;
    f32 step;
    u32 count;
    u32 i;

    effGetScatterNarrowBlock(work->scatterObject, index);
    ring = &work->rings[index];
    radius = ring->radius + ring->radiusStep;
    count = ((PcpScatterBlockObject *)work->scatterObject)->stride >> 1;
    rise = ring->rise;
    angle = ring->angle;
    step = ring->angleStep;
    func_002DD608(ring->unk00);
    func_002DD968(ring->tiltAngle);
    sdfMultiplyVuMatrixInPlace();
    ring->tiltAngle += ring->tiltSpeed;
    ring->tiltSpeed *= work->tiltDamping;
    ring->radius = radius;
    ring->rise = rise * work->riseDecay;
    for (i = 0; i < count; i++) {
        f32 s;

        vertex[0] = sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
        vertex[1] += rise;
        s = sdfSinPoly(angle);
        vertex[4] = vertex[0];
        vertex[5] += rise;
        vertex[6] = vertex[2] = s * radius;
        VU0_LOAD_VF(vf10, vertex);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, vertex);
        VU0_LOAD_VF(vf10, vertex + 4);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, vertex + 4);
        angle += step;
        vertex += 8;
    }
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001738C8);

void effScatterCopyParticleParameterVector(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x40, src);
}

void effScatterSetParticleScale(PcpScatterWork4 *work, f32 value)
{
    work->scale = value;
}

void effScatterSetParticleColor(PcpScatterWork4 *work, u32 value)
{
    work->color = value;
}

/* vu0 routine: matrix = (matrix + 0x50) * src, via the vf28-vf31 by vf24-vf27 product routine */
void effPcpScatterTransformMatrix(PcpScatterWork4 *work, void *source)
{
    VU0_LOAD_MATRIX(source);
    VU0_LOAD_MATRIX_B(&work->pad50[0]);
    sdfComposeVuMatrixFromRegisters();
    VU0_STORE_MATRIX(work);
}

typedef struct PcpScatterParamsB {
    u8 pad00[0x10];
    f32 matrix[16];
    u32 unk50;
    u8 pad54[0x08];
    u32 particleCount;
    u32 unk60;
    u32 randomDelayRange;
    u8 pad68[0xD4];
} PcpScatterParamsB;

typedef struct PcpScatterInstanceB {
    f32 matrix[16];
    PcpScatterParamsB params;
    PcpScatterParticle *particles;
    f32 scale;
    u32 color;
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
    inst->color = 0x80808080;
    inst->scale = 1.0f;
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
    limit = inst->params.randomDelayRange;
    if (limit <= 0) {
        inst->params.randomDelayRange = 1;
        limit = 1;
    }
    mod = limit;
    count = inst->params.particleCount;
    for (i = 0; i < count; i++) {
        particle->age = -(effMiscRand(D_0034DF38) % mod);
        particle++;
    }
    return inst;
}

void effScatterSpawnFromParameterPair(void *data)
{
    effScatterInstanceCreateB(effParamTableGetBlock(data, 0), effParamTableGetBlock(data, 1));
}

PcpScatterWork5 *effScatterCloneWithSharedResource(PcpScatterWork5 *work)
{
    PcpScatterWork5 *child;

    child = effScatterInstanceCreateB(&work->particleParams, NULL);
    effShareScatterResource(child->scatterObject, work->scatterObject);
    return child;
}

void effScatterReleaseInstanceResources(PcpScatterWork5 *work)
{
    effReleaseScatterObject(work->scatterObject);
    func_002D0918(work->ownedBuffer);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00173E38);

typedef struct PcpScatterWork12 {
    u8 pad00[0xCC];
    f32 riseDecay;
    u8 padD0[8];
    f32 tiltDamping;
    u8 padDC[0x10];
    f32 radiusDamping;
    u8 padF0[0x8C];
    PcpScatterRing *rings;
    u8 pad180[0xC];
    u32 scatterObject;
} PcpScatterWork12;

/* Same ring update as effScatterRingUpdate, with the ring's 0x20 radius step scaled as well. */
void effScatterRingUpdateScaled(PcpScatterWork12 *work, s32 index)
{
    f32 *vertex = (f32 *)effGetScatterWideBlock(work->scatterObject, index);
    PcpScatterRing *ring;
    f32 radius;
    f32 rise;
    f32 angle;
    f32 step;
    u32 count;
    u32 i;

    effGetScatterNarrowBlock(work->scatterObject, index);
    ring = &work->rings[index];
    radius = ring->radius + ring->radiusStep;
    count = ((PcpScatterBlockObject *)work->scatterObject)->stride >> 1;
    rise = ring->rise;
    angle = ring->angle;
    step = ring->angleStep;
    func_002DD608(ring->unk00);
    func_002DD968(ring->tiltAngle);
    sdfMultiplyVuMatrixInPlace();
    ring->tiltAngle += ring->tiltSpeed;
    ring->tiltSpeed *= work->tiltDamping;
    ring->radius = radius;
    ring->radiusStep *= work->radiusDamping;
    ring->rise = rise * work->riseDecay;
    for (i = 0; i < count; i++) {
        f32 s;

        vertex[0] = sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
        vertex[1] += rise;
        s = sdfSinPoly(angle);
        vertex[4] = vertex[0];
        vertex[5] += rise;
        vertex[6] = vertex[2] = s * radius;
        VU0_LOAD_VF(vf10, vertex);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, vertex);
        VU0_LOAD_VF(vf10, vertex + 4);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, vertex + 4);
        angle += step;
        vertex += 8;
    }
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00174350);

void func_001745F8(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x40, src);
}

void effScatterSetInstanceScale(PcpScatterWork5 *work, f32 value)
{
    work->scale = value;
}

void effScatterSetInstanceColor(PcpScatterWork5 *work, u32 value)
{
    work->color = value;
}

/* vu0 routine: matrix = (matrix + 0x50) * src, via the vf28-vf31 by vf24-vf27 product routine */
void effScatterComposeWorkMatrix(PcpScatterWork5 *work, void *source)
{
    VU0_LOAD_MATRIX(source);
    VU0_LOAD_MATRIX_B(&work->pad50[0]);
    sdfComposeVuMatrixFromRegisters();
    VU0_STORE_MATRIX(work);
}

typedef struct PcpScatterParamsC {
    u8 pad00[0x10];
    f32 matrix[16];
    u32 unk50;
    u8 pad54[0x08];
    u32 particleCount;
    u32 unk60;
    u32 randomDelayRange;
    u8 pad68[0xDC];
} PcpScatterParamsC;

typedef struct PcpScatterInstanceC {
    f32 matrix[16];
    PcpScatterParamsC params;
    PcpScatterParticle *particles;
    f32 scale;
    u32 color;
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
    inst->color = 0x80808080;
    inst->scale = 1.0f;
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
    limit = inst->params.randomDelayRange;
    if (limit <= 0) {
        inst->params.randomDelayRange = 1;
        limit = 1;
    }
    mod = limit;
    count = inst->params.particleCount;
    for (i = 0; i < count; i++) {
        particle->age = -(effMiscRand(D_0034DF38) % mod);
        particle++;
    }
    return inst;
}

void func_00174880(void *data)
{
    effScatterInstanceCreateC(effParamTableGetBlock(data, 0), effParamTableGetBlock(data, 1));
}

PcpScatterWork6 *effCreateScatterChildSharingParentResource(PcpScatterWork6 *work)
{
    PcpScatterWork6 *child;

    child = effScatterInstanceCreateC(&work->particleParams, NULL);
    effShareScatterResource(child->scatterObject, work->scatterObject);
    return child;
}

void effReleaseScatterObjectAndOwnedBuffer(PcpScatterWork6 *work)
{
    effReleaseScatterObject(work->scatterObject);
    func_002D0918(work->ownedBuffer);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00174940);

typedef struct PcpScatterWork13 {
    u8 pad00[0xD0];
    f32 riseDecay;
    u8 padD4[8];
    f32 tiltDamping;
    u8 padE0[0x10];
    f32 radiusDamping;
    u8 padF4[0x90];
    PcpScatterRing *rings;
    u8 pad188[0xC];
    u32 scatterObject;
} PcpScatterWork13;

/* Same ring update as effScatterRingUpdateScaled on a work area with a longer header. */
void effScatterRingUpdateScaledLong(PcpScatterWork13 *work, s32 index)
{
    f32 *vertex = (f32 *)effGetScatterWideBlock(work->scatterObject, index);
    PcpScatterRing *ring;
    f32 radius;
    f32 rise;
    f32 angle;
    f32 step;
    u32 count;
    u32 i;

    effGetScatterNarrowBlock(work->scatterObject, index);
    ring = &work->rings[index];
    radius = ring->radius + ring->radiusStep;
    count = ((PcpScatterBlockObject *)work->scatterObject)->stride >> 1;
    rise = ring->rise;
    angle = ring->angle;
    step = ring->angleStep;
    func_002DD608(ring->unk00);
    func_002DD968(ring->tiltAngle);
    sdfMultiplyVuMatrixInPlace();
    ring->tiltAngle += ring->tiltSpeed;
    ring->tiltSpeed *= work->tiltDamping;
    ring->radius = radius;
    ring->radiusStep *= work->radiusDamping;
    ring->rise = rise * work->riseDecay;
    for (i = 0; i < count; i++) {
        f32 s;

        vertex[0] = sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
        vertex[1] += rise;
        s = sdfSinPoly(angle);
        vertex[4] = vertex[0];
        vertex[5] += rise;
        vertex[6] = vertex[2] = s * radius;
        VU0_LOAD_VF(vf10, vertex);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, vertex);
        VU0_LOAD_VF(vf10, vertex + 4);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, vertex + 4);
        angle += step;
        vertex += 8;
    }
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00174ED0);

void func_001751A8(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x40, src);
}

void effSetScatterWorkScale(PcpScatterWork6 *work, f32 value)
{
    work->scale = value;
}

void effSetScatterWorkColor(PcpScatterWork6 *work, u32 value)
{
    work->color = value;
}

/* vu0 routine: matrix = (matrix + 0x50) * src, via the vf28-vf31 by vf24-vf27 product routine */
void func_001751D0(PcpScatterWork6 *work, void *source)
{
    VU0_LOAD_MATRIX(source);
    VU0_LOAD_MATRIX_B(&work->pad50[0]);
    sdfComposeVuMatrixFromRegisters();
    VU0_STORE_MATRIX(work);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00175230);

void func_00175420(void *data)
{
    func_00175230(effParamTableGetBlock(data, 0), effParamTableGetBlock(data, 1));
}

PcpScatterWork7 *effCloneScatterWithSharedResource(PcpScatterWork7 *work)
{
    PcpScatterWork7 *child;

    child = func_00175230(&work->particleParams, NULL);
    effShareScatterResource(child->scatterObject, work->scatterObject);
    return child;
}

void effReleaseScatterWorkResources(PcpScatterWork7 *work)
{
    effReleaseScatterObject(work->scatterObject);
    func_002D0918(work->ownedBuffer);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001754E0);

typedef struct PcpScatterRingB {
    f32 rot[3];
    s32 age;
    f32 angle;
    f32 angularSpeed;
    f32 angleStep;
    f32 radius;
    f32 radialSpeed;
    f32 height;
} PcpScatterRingB;

typedef struct PcpScatterWork14 {
    u8 pad00[0x84];
    f32 angularDamping;
    u8 pad88[0xC];
    f32 radialDamping;
    s32 radialDecayStart;
    u8 pad9C[0x8C];
    PcpScatterRingB *rings;
    u8 pad12C[8];
    u32 scatterObject;
} PcpScatterWork14;

/* Advance ring `index` and lay its vertex pairs around a flat circle rotated by the ring's own angles. */
void effScatterFlatRingUpdate(PcpScatterWork14 *work, s32 index)
{
    f32 *vertex = (f32 *)effGetScatterWideBlock(work->scatterObject, index);
    PcpScatterRingB *ring;
    f32 angle;
    f32 radius;
    f32 step;
    f32 height;
    f32 s;
    u32 count;
    u32 i;

    effGetScatterNarrowBlock(work->scatterObject, index);
    ring = &work->rings[index];
    count = ((PcpScatterBlockObject *)work->scatterObject)->stride >> 1;
    angle = ring->angle + ring->angularSpeed;
    radius = ring->radius;
    step = ring->angleStep;
    radius += ring->radialSpeed;
    if (ring->age >= work->radialDecayStart) {
        ring->radialSpeed *= work->radialDamping;
    }
    vu0RotMatrixXYZFromVec3(ring);
    ring->radius = radius;
    ring->angle = angle;
    ring->angularSpeed *= work->angularDamping;
    height = ring->height;
    for (i = 0; i < count; i++) {
        vertex[0] = sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
        vertex[1] = 0;
        s = sdfSinPoly(angle);
        vertex[4] = vertex[0];
        vertex[5] = -height;
        vertex[6] = vertex[2] = s * radius;
        VU0_LOAD_VF(vf10, vertex);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, vertex);
        VU0_LOAD_VF(vf10, vertex + 4);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, vertex + 4);
        angle += step;
        vertex += 8;
    }
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00175908);
