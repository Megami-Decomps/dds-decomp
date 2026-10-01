#include "common.h"

#include "pcp_vu0.h"

extern void sdfComposeVuMatrixFromRegisters(void);

extern u64 effParamTableGetBlock(u64, u64);

extern u32 effPcpScatterResAddRef(u32);

extern u8 D_003AA868[];
extern f32 effMiscRandUnitFloat(void *state);
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32 angle);
extern f32 sdfSinPoly(f32 angle);
extern void func_003364B8(f32 angle);
extern void func_00336818(f32 angle);
extern void sdfMultiplyVuMatrixInPlace(void);
extern void vu0RotMatrixXYZFromVec3(f32 *rot);
extern s32 effGetScatterWideBlock(u32 object, s32 index);
extern s32 effGetScatterNarrowBlock(u32 object, s32 index);

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
    s128 particleParams;
    u8 pad50[0x12C];
    f32 scale;
    u32 color;
    u32 scatterObject;
    u32 ownedBuffer;
};

extern PcpScatterWork4 *effPcpScatterCreateParticleInstance();

extern void effShareScatterResource(u32 param0, u32 param1);

typedef struct PcpScatterInstanceB PcpScatterInstanceB;

typedef struct PcpScatterParticle PcpScatterParticle;
typedef struct PcpScatterDraw PcpScatterDraw;

/* The B constructor copies this 0x13C-byte block to instance +0x40;
   ring setup and the fading update read fields from that same copy. */
typedef struct PcpScatterParamsB {
    f32 vec[4];
    f32 matrix[16];
    u32 unk50;
    u8 loop;
    u8 pad55[3];
    s32 duration;
    u32 particleCount;
    u32 unk60;
    u32 randomDelayRange;
    s32 fadeIn;
    s32 fadeRange;
    f32 angleStepBase;
    f32 angleStepJitter;
    f32 riseStep;
    f32 tiltScale;
    f32 heightOffsetBase;
    f32 heightOffsetJitter;
    f32 initialRise;
    f32 riseDecay;
    u8 pad90[4];
    f32 initialTiltSpeed;
    f32 tiltDamping;
    f32 radiusBase;
    f32 radiusJitter;
    f32 radiusStepBase;
    f32 radiusStepJitter;
    f32 radiusDamping;
    s32 colorParam;
    u32 vCount;
    u32 vTail;
    u8 padBC[0x80];
} PcpScatterParamsB;

/* One 0x194-byte instance shared by creation, ring setup, update and teardown.
   The particles pointer at 0x17C is the ring array, not another allocation. */
struct PcpScatterInstanceB {
    f32 matrix[16];
    PcpScatterParamsB params;
    PcpScatterParticle *particles;
    f32 scale;
    u32 color;
    s32 age;
    u32 scatterObject;
    u32 ownedBuffer;
};

typedef struct PcpScatterInstanceC PcpScatterInstanceC;

/* C adds staggered ring motion and two colour keys to the copied parameters.
   Its 0x144-byte parameter block ends immediately before particles at 0x184. */
typedef struct PcpScatterParamsC {
    f32 vec[4];
    f32 matrix[16];
    u32 unk50;
    u8 loop;
    u8 pad55[3];
    s32 duration;
    u32 particleCount;
    u32 unk60;
    u32 randomDelayRange;
    s32 fadeIn;
    s32 fadeRange;
    f32 angleStepBase;
    f32 angleStepJitter;
    f32 tiltScale;
    f32 riseRange;
    f32 radiusRamp;
    f32 heightOffsetBase;
    f32 heightOffsetJitter;
    f32 initialRise;
    f32 riseDecay;
    u8 pad94[4];
    f32 initialTiltSpeed;
    f32 tiltDamping;
    f32 radiusBase;
    f32 radiusJitter;
    f32 radiusStepBase;
    f32 radiusStepJitter;
    f32 radiusDamping;
    s32 colorA;
    s32 colorB;
    u32 vTail;
    u32 vCount;
    u8 padC4[0x80];
} PcpScatterParamsC;

/* One 0x19C-byte instance; ring setup, motion and colour update share particles. */
struct PcpScatterInstanceC {
    f32 matrix[16];
    PcpScatterParamsC params;
    PcpScatterParticle *particles;
    f32 scale;
    u32 color;
    s32 age;
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
    s128 particleParams;
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
    u32 *duplicatedHandles; /* 0x7C: also used by the linked-work copy constructor */
} PcpScatterWork8;

typedef struct PcpScatterWork2 {
    u8 pad00[0x40];
    f32 unk40;
    f32 unk44;
    u8 pad48[0x24];
    u32 unk6C;
} PcpScatterWork2;

/* Allocated after the two record arrays; resource helpers receive this same
   0x34-byte control block, not a separate effect work area. */
typedef struct PcpScatterPool {
    u8 pad00[0x10];
    u32 unk10;
    u32 color;
    s32 secondWordCount;
    f32 unk1C;
    s32 recordBase;
    s32 auxRecordBase;
    u32 resource;
    u32 buffer;
    PcpScatterRes *sharedResource;
} PcpScatterPool;

extern u32 effParamWorkDuplicate(u32 param);

extern void effPcpScatterSharePoolResource(PcpScatterPool *dst, PcpScatterPool *src);

extern u32 func_003292A8(u32 size);

extern u32 *sdfResourceRetainAddress(u32 handle);

extern void *memset(void *dst, s32 value, u32 size);

extern u8 D_00452020[0x2C];

extern u32 sdfCreateAssetWithDrawEntries(void);

extern void func_003332D0(u32 res, f32 scale);

/* Effect initializers implemented in assembly below (func_001708A0 lives in
   another unit). Each is entered with and without spawn arguments, so they
   are declared unchecked. */
extern void *func_001784F8();

/* Per-effect work areas. Only the fields touched by the matched spawn,
   teardown and scale helpers are known; the update bodies are still assembly.
   Each work area belongs to the effect whose initializer is noted. */
typedef struct PcpScatterWork1 PcpScatterWork1;


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
    PcpScatterPool *childWork;
    u32 ownedResource;
    u32 duplicatedCount;
    u32 *duplicatedHandles;
    u32 duplicateAllocation;
};

/* Parameter-vector setter: unlike the pool control block, its argument has
   a word at 0x54. Keep this input distinct from the 0x34-byte allocation. */
typedef struct PcpScatterPoolParams {
    u8 pad00[0x54];
    u32 unk54;
} PcpScatterPoolParams;

extern void *func_00179DB0();

/* Work2 as seen by the copy constructor: byte 0x40 is a flag here (func_00172138 scales
   the same word as a float). */
typedef struct {
    u8 pad00[0x20];
    u32 particleCount;
    u8 pad24[0x1C];
    u8 duplicateParticles;
    u8 pad41[7];
    u32 particlesPerGroup;
    u8 pad4C[0xC];
    PcpScatterPool *childWork;
    u32 ownedResource;
    u32 duplicatedCount;
    u32 *duplicatedHandles;
    u32 duplicateAllocation;
} PcpScatterWork2Copy;

void effScatterCreateFromParameterTriplet(u64 table) {
    u64 params;
    u64 resource;
    u64 options;

    params = effParamTableGetBlock(table, 0);
    resource = effParamTableGetBlock(table, 1);
    options = effParamTableGetBlock(table, 2);
    func_001784F8(params, resource, options);
}

PcpScatterWork1 *effPcpScatterSharedDuplicate(src)
    PcpScatterWork1 *src;
{
    PcpScatterWork1 *work;
    u32 count;
    u32 handle;
    u32 *buf;
    u32 i;

    work = func_001784F8(src, 0, 0);
    effPcpScatterSharePoolResource(work->childWork, src->childWork);
    if (work->duplicateParticles != 0) {
        work->duplicatedCount = work->particleCount / work->particlesPerGroup;
        if (work->particleCount % work->particlesPerGroup != 0) {
            work->duplicatedCount = work->duplicatedCount + 1;
        }
        count = work->duplicatedCount;
        handle = func_003292A8(count * 4);
        buf = sdfResourceRetainAddress(handle);
        work->duplicateAllocation = handle;
        work->duplicatedHandles = buf;
        for (i = 0; i < count; i++) {
            work->duplicatedHandles[i] = effParamWorkDuplicate(*src->duplicatedHandles);
        }
    }
    return work;
}

typedef struct PcpScatterParticles {
    PcpScatterPool *pool;
    u32 buffer;
    u32 count;
    u32 *items;
    u32 itemBuffer;
} PcpScatterParticles;

extern void effDispatchParameterDataAndFreeWork(u32 particle);

typedef struct PcpScatterGroupWork {
    u8 pad00[0x80];
    PcpScatterParticles particles;
} PcpScatterGroupWork;

void effPcpScatterReleaseParticleGroup(PcpScatterGroupWork *work) {
    if (work->particles.itemBuffer != 0) {
        u32 count = work->particles.count;
        u32 i;

        for (i = 0; i < count; i++) {
            effDispatchParameterDataAndFreeWork(work->particles.items[i]);
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

void effSetScatterDuplicatedHandles(PcpScatterWork8 *work, u32 *handles) {
    work->duplicatedHandles = handles;
}

void effScatterScaleParticleValues(float scale, PcpScatterWork8 *work) {
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

/* Work8 as seen by its copy constructor. The float parameters also occur
   in PcpScatterWork8 and are scaled by effScatterScaleParticleValues. */
typedef struct {
    u8 pad00[0x20];
    u32 particleCount;
    u8 pad24[0x18];
    /* These floats are scaled together; their individual roles are not yet known. */
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
    PcpScatterPool *childWork;
    u32 ownedResource;
    u32 duplicatedCount;
    u32 *duplicatedHandles;
    u32 duplicateAllocation;
} PcpScatterWork8Copy;

PcpScatterWork8Copy *effPcpScatterLinkedDuplicate(src)
    PcpScatterWork8Copy *src;
{
    PcpScatterWork8Copy *work;
    u32 count;
    u32 handle;
    u32 *buf;
    u32 i;

    work = func_001791A8(src, 0, 0);
    effPcpScatterSharePoolResource(work->childWork, src->childWork);
    if (work->duplicateParticles != 0) {
        work->duplicatedCount = work->particleCount / work->particlesPerGroup;
        if (work->particleCount % work->particlesPerGroup != 0) {
            work->duplicatedCount = work->duplicatedCount + 1;
        }
        count = work->duplicatedCount;
        handle = func_003292A8(count * 4);
        buf = sdfResourceRetainAddress(handle);
        work->duplicateAllocation = handle;
        work->duplicatedHandles = buf;
        for (i = 0; i < count; i++) {
            work->duplicatedHandles[i] = effParamWorkDuplicate(*src->duplicatedHandles);
        }
    }
    return work;
}

typedef struct PcpScatterSharedWork {
    u8 pad00[0x70];
    PcpScatterParticles particles;
} PcpScatterSharedWork;

void effPcpScatterReleaseSharedParticles(PcpScatterSharedWork *work) {
    if (work->particles.itemBuffer != 0) {
        u32 count = work->particles.count;
        u32 i;

        for (i = 0; i < count; i++) {
            effDispatchParameterDataAndFreeWork(work->particles.items[i]);
        }
        func_003297C8(work->particles.itemBuffer);
    }
    effPcpScatterReleasePoolResources(work->particles.pool);
    func_003297C8(work->particles.buffer);
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

    sprite->spinAngle = effMiscRandUnitFloat(D_003AA868) * (3.14159265f * 2.0f);
    jitter = work->startRadiusJitter;
    sprite->startRadius = work->startRadius * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    jitter = work->endRadiusJitter;
    sprite->radiusStep = (work->endRadius * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) - sprite->startRadius) / (f32)work->lifetime;
    sprite->initialSize = work->initialSize;
    dir[0] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    dir[1] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    dir[2] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, dir);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, dir);
    sprite->age = 0;
    sprite->dirX = dir[0];
    sprite->dirY = dir[1];
    sprite->dirZ = dir[2];
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00179780);

void func_00179D78(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00179D88(PcpScatterWork2 *work, u32 value) {
    work->unk6C = value;
}

void effScatterScalePair(float scale, PcpScatterWork2 *work) {
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


PcpScatterWork2Copy *effPcpScatterTableDuplicate(src)
    PcpScatterWork2Copy *src;
{
    PcpScatterWork2Copy *work;
    u32 count;
    u32 handle;
    u32 *buf;
    u32 i;

    work = func_00179DB0(src, 0, 0);
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
        handle = func_003292A8(count * 4);
        buf = sdfResourceRetainAddress(handle);
        work->duplicateAllocation = handle;
        work->duplicatedHandles = buf;
        for (i = 0; i < count; i++) {
            work->duplicatedHandles[i] = effParamWorkDuplicate(*src->duplicatedHandles);
        }
    }
    return work;
}

typedef struct PcpScatterLinkedWork {
    u8 pad00[0x58];
    PcpScatterParticles particles;
} PcpScatterLinkedWork;

void effPcpScatterReleaseLinkedParticles(PcpScatterLinkedWork *work) {
    if (work->particles.itemBuffer != 0) {
        u32 count = work->particles.count;
        u32 i;

        for (i = 0; i < count; i++) {
            effDispatchParameterDataAndFreeWork(work->particles.items[i]);
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

void func_0017A8B0(PcpScatterPoolParams *work, u32 value) {
    work->unk54 = value;
}

void func_0017A8B8(void) {
}

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
    handle = func_003292A8(size);
    block = sdfResourceRetainAddress(handle);
    memset(block, 0, size);
    pool = (PcpScatterPool *)(block + (first + second));
    pool->recordBase = (s32)block;
    pool->unk10 = 1;
    pool->auxRecordBase = (s32)(block + first);
    pool->secondWordCount = second;
    pool->buffer = handle;
    pool->unk1C = 1.0f;
    pool->color = 0x80808080;
    pool->sharedResource = 0;
    pool->resource = sdfCreateAssetWithDrawEntries();
    func_003332D0(pool->resource, 1.0f);
    memset(D_00452020, 0, 0x2C);
    *(u16 *)(D_00452020 + 4) = 0x4000;
    return pool;
}

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

extern void sdfReleaseChipBlock(void *);

void effPcpScatterResRelease(PcpScatterRes *res) {
    res->refCount--;
    if (res->refCount == 0) {
        sdfTexReleaseReferenceViaHandler(res->resourceHandle);
        sdfReleaseChipBlock(res);
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
    u32 randomDelayRange;
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

/* The 0x28-byte particle's age sits between its two orientation values and
   ring motion state; both the lifecycle and vertex passes use this record. */
struct PcpScatterParticle {
    f32 unk00;
    f32 tiltAngle;
    s32 age;
    f32 rise;
    f32 angle;
    f32 tiltSpeed;
    f32 angleStep;
    f32 radius;
    f32 radiusStep;
    f32 heightOffset;
};

typedef struct PcpScatterInstance {
    f32 matrix[16];
    PcpScatterParams params;
    PcpScatterParticle *particles;
    f32 scale;
    u32 color;
    u32 scatterObject;
    u32 ownedBuffer;
} PcpScatterInstance;

extern void *func_0017D7A8();

extern void effCreateScatterResource(void *object, u32 resource);

extern u32 effMiscRand(void *state);

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
    inst->color = 0x80808080;
    inst->scale = 1.0f;
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
    mod = inst->params.randomDelayRange;
    count = inst->params.particleCount;
    if ((s32)mod <= 0) {
        mod = 1;
    }
    for (i = 0; i < count; i++) {
        particle->age = -(effMiscRand(D_003AA868) % mod);
        particle++;
    }
    return (PcpScatterWork4 *)inst;
}

PcpScatterWork4 *effScatterBlockDuplicate(u64 table) {
    u64 particleParams = effParamTableGetBlock(table, 0);
    u64 resource = effParamTableGetBlock(table, 1);

    return effPcpScatterCreateParticleInstance(particleParams, resource);
}

PcpScatterWork4 *effScatterCloneWithSharedObject(PcpScatterWork4 *work) {
    PcpScatterWork4 *child;

    child = effPcpScatterCreateParticleInstance(&work->particleParams, NULL);
    effShareScatterResource(child->scatterObject, work->scatterObject);
    return child;
}

void effScatterReleaseObjectAndBuffer(PcpScatterWork4 *work) {
    effReleaseScatterObject(work->scatterObject);
    func_003297C8(work->ownedBuffer);
}


typedef struct PcpScatterBlockObject {
    u8 pad00[0x5C];
    s32 stride;
} PcpScatterBlockObject;

typedef struct PcpScatterWork11 {
    u8 pad00[0x98];
    s32 lifetime;           /* 0x98 */
    u8 pad9C[0x14];
    f32 angleStepBase;      /* 0xB0 */
    f32 angleStepJitter;    /* 0xB4 */
    f32 riseStep;           /* 0xB8 */
    f32 tiltScale;          /* 0xBC */
    f32 heightOffsetBase;   /* 0xC0 */
    f32 heightOffsetJitter; /* 0xC4 */
    f32 initialRise;        /* 0xC8 */
    f32 riseDecay;          /* 0xCC */
    u8 padD0[4];
    f32 initialTiltSpeed;   /* 0xD4 */
    f32 tiltDamping;        /* 0xD8 */
    f32 radiusBase;         /* 0xDC */
    f32 radiusJitter;       /* 0xE0 */
    f32 radiusEndBase;      /* 0xE4 */
    f32 radiusEndJitter;    /* 0xE8 */
    u8 padEC[4];
    u32 vCount;             /* 0xF0 */
    u32 vTail;              /* 0xF4 */
    u8 padF8[0x80];
    PcpScatterParticle *rings;  /* 0x178 */
    u8 pad17C[8];
    u32 scatterObject;      /* 0x184 */
} PcpScatterWork11;

/* Initialise ring `index`: randomised radius/angle/rise parameters, then the first set of vertex pairs. */
void effScatterRingInit(PcpScatterWork11 *work, s32 index)
{
    f32 *vertex = (f32 *)effGetScatterWideBlock(work->scatterObject, index);
    f32 *uv = (f32 *)effGetScatterNarrowBlock(work->scatterObject, index);
    PcpScatterParticle *ring;
    f32 angle;
    f32 angleStep;
    f32 radius;
    f32 height;
    f32 rise;
    f32 riseStep;
    f32 u;
    f32 du;
    f32 v;
    f32 jitter;
    f32 s;
    u32 count;
    u32 i;

    ring = &work->rings[index];
    count = ((PcpScatterBlockObject *)work->scatterObject)->stride >> 1;
    angle = effMiscRandUnitFloat(D_003AA868) * (3.14159265f * 2.0f);
    jitter = work->angleStepJitter;
    angleStep = work->angleStepBase * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) / (f32)count;
    riseStep = work->riseStep;
    jitter = work->radiusJitter;
    radius = work->radiusBase * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    jitter = work->heightOffsetJitter;
    height = work->heightOffsetBase * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    jitter = work->radiusEndJitter;
    rise = 0.0f;
    ring->radiusStep = (work->radiusEndBase * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) - radius) / (f32)work->lifetime;
    ring->unk00 = work->tiltScale * effMiscRandUnitFloat(D_003AA868);
    ring->tiltAngle = rise;
    ring->angle = angle;
    ring->radius = radius;
    ring->heightOffset = height;
    ring->angleStep = angleStep;
    ring->tiltSpeed = work->initialTiltSpeed;
    ring->rise = work->initialRise;
    func_003364B8(ring->unk00);
    func_00336818(ring->tiltAngle);
    sdfMultiplyVuMatrixInPlace();
    v = (f32)work->vTail;
    u = 0.0f;
    du = (f32)work->vCount / (f32)count;
    for (i = 0; i < count; i++) {
        vertex[0] = sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
        vertex[1] = rise;
        s = sdfSinPoly(angle);
        vertex[4] = vertex[0];
        vertex[5] = vertex[1] - height;
        vertex[6] = vertex[2] = s * radius;
        VU0_LOAD_VF(vf10, vertex);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, vertex);
        VU0_LOAD_VF(vf10, vertex + 4);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, vertex + 4);
        uv[0] = u;
        uv[2] = u;
        uv[1] = 0;
        uv[3] = v;
        uv += 4;
        rise += riseStep;
        u += du;
        angle += angleStep;
        vertex += 8;
    }
}

/* Advance ring `index`: rebuild the rotation matrix, then lay the ring's vertex pairs around it. */
void effScatterRingUpdate(PcpScatterWork11 *work, s32 index)
{
    f32 *vertex = (f32 *)effGetScatterWideBlock(work->scatterObject, index);
    PcpScatterParticle *ring;
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
    func_003364B8(ring->unk00);
    func_00336818(ring->tiltAngle);
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

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017B520);

void effScatterCopyParticleParameterVector(PcpScatterWork4 *work, void *src) {
    PCP_COPY_VECTOR(&work->particleParams, src);
}

void effScatterSetParticleScale(PcpScatterWork4 *work, f32 value)
{
    work->scale = value;
}

void effScatterSetParticleColor(PcpScatterWork4 *work, u32 value) {
    work->color = value;
}

/* vu0 routine: matrix = (matrix + 0x50) * src, via the vf28-vf31 by vf24-vf27 product routine */
void effPcpScatterTransformMatrix(u8 *matrix, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_LOAD_MATRIX_B(matrix + 0x50);
    sdfComposeVuMatrixFromRegisters();
    VU0_STORE_MATRIX(matrix);
}


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
    inst->color = 0x80808080;
    inst->scale = 1.0f;
    inst->ownedBuffer = allocation;
    inst->particles = particle;
    inst->age = 0;
    VU0_COPY_MATRIX(inst->matrix, src->matrix);
    object = func_0017D7A8(src->particleCount, src->unk60);
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
        particle->age = -(effMiscRand(D_003AA868) % mod);
        particle++;
    }
    return inst;
}

void effScatterSpawnFromParameterPair(u64 table) {
    u64 particleParams;
    u64 resource;

    particleParams = effParamTableGetBlock(table, 0);
    resource = effParamTableGetBlock(table, 1);
    effScatterInstanceCreateB(particleParams, resource);
}

PcpScatterInstanceB *effScatterCloneWithSharedResource(PcpScatterInstanceB *work)
{
    PcpScatterInstanceB *child;

    child = effScatterInstanceCreateB(&work->params, NULL);
    effShareScatterResource(child->scatterObject, work->scatterObject);
    return child;
}

void effScatterReleaseInstanceResources(PcpScatterInstanceB *work) {
    effReleaseScatterObject(work->scatterObject);
    func_003297C8(work->ownedBuffer);
}


/* Initialise ring `index`: randomised radius/angle/rise parameters, then the first set of vertex pairs. */
void effScatterRingInitScaled(PcpScatterInstanceB *work, s32 index)
{
    f32 *vertex = (f32 *)effGetScatterWideBlock(work->scatterObject, index);
    f32 *uv = (f32 *)effGetScatterNarrowBlock(work->scatterObject, index);
    PcpScatterParticle *ring;
    f32 angle;
    f32 angleStep;
    f32 radius;
    f32 height;
    f32 rise;
    f32 riseStep;
    f32 u;
    f32 du;
    f32 v;
    f32 jitter;
    f32 s;
    u32 count;
    u32 i;

    ring = &work->particles[index];
    count = ((PcpScatterBlockObject *)work->scatterObject)->stride >> 1;
    angle = effMiscRandUnitFloat(D_003AA868) * (3.14159265f * 2.0f);
    jitter = work->params.angleStepJitter;
    angleStep = work->params.angleStepBase * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) / (f32)count;
    riseStep = work->params.riseStep;
    jitter = work->params.radiusJitter;
    radius = work->params.radiusBase * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    jitter = work->params.heightOffsetJitter;
    height = work->params.heightOffsetBase * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    jitter = work->params.radiusStepJitter;
    rise = 0.0f;
    ring->radiusStep = work->params.radiusStepBase * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    ring->unk00 = work->params.tiltScale * effMiscRandUnitFloat(D_003AA868);
    ring->tiltAngle = rise;
    ring->angle = angle;
    ring->radius = radius;
    ring->heightOffset = height;
    ring->angleStep = angleStep;
    ring->tiltSpeed = work->params.initialTiltSpeed;
    ring->rise = work->params.initialRise;
    func_003364B8(ring->unk00);
    func_00336818(ring->tiltAngle);
    sdfMultiplyVuMatrixInPlace();
    v = (f32)work->params.vTail;
    u = 0.0f;
    du = (f32)work->params.vCount / (f32)count;
    for (i = 0; i < count; i++) {
        vertex[0] = sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
        vertex[1] = rise;
        s = sdfSinPoly(angle);
        vertex[4] = vertex[0];
        vertex[5] = vertex[1] - height;
        vertex[6] = vertex[2] = s * radius;
        VU0_LOAD_VF(vf10, vertex);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, vertex);
        VU0_LOAD_VF(vf10, vertex + 4);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, vertex + 4);
        uv[0] = u;
        uv[2] = u;
        uv[1] = 0;
        uv[3] = v;
        uv += 4;
        rise += riseStep;
        u += du;
        angle += angleStep;
        vertex += 8;
    }
}

/* Same ring update as effScatterRingUpdate, with the ring's 0x20 radius step scaled as well. */
void effScatterRingUpdateScaled(PcpScatterInstanceB *work, s32 index)
{
    f32 *vertex = (f32 *)effGetScatterWideBlock(work->scatterObject, index);
    PcpScatterParticle *ring;
    f32 radius;
    f32 rise;
    f32 angle;
    f32 step;
    u32 count;
    u32 i;

    effGetScatterNarrowBlock(work->scatterObject, index);
    ring = &work->particles[index];
    radius = ring->radius + ring->radiusStep;
    count = ((PcpScatterBlockObject *)work->scatterObject)->stride >> 1;
    rise = ring->rise;
    angle = ring->angle;
    step = ring->angleStep;
    func_003364B8(ring->unk00);
    func_00336818(ring->tiltAngle);
    sdfMultiplyVuMatrixInPlace();
    ring->tiltAngle += ring->tiltSpeed;
    ring->tiltSpeed *= work->params.tiltDamping;
    ring->radius = radius;
    ring->radiusStep *= work->params.radiusDamping;
    ring->rise = rise * work->params.riseDecay;
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

/* Draw record behind the instance's scatterObject: colour per particle at +0x70, scale at +0x60. */
struct PcpScatterDraw {
    u8 pad00[0x60];
    f32 scale;           /* 0x60 */
    u8 pad64[0xC];
    u32 *colors;         /* 0x70 */
};


extern s32 effMultiplyPackedColors(s32 color, s32 param);
extern u32 effBlendColor(u32 colorA, u32 colorB, f32 t);
extern void effScatterStoreSourceTransformMatrix(void *draw, void *work);
extern void func_0017DA28(void *draw);

/* Per-frame update of a fading, optionally looping scatter instance. */
void effScatterUpdateLoopedScaledRing(PcpScatterInstanceB *work) {
    s32 loop;
    s32 duration = work->params.duration;
    PcpScatterDraw *draw = (PcpScatterDraw *)work->scatterObject;
    PcpScatterParticle *particle = work->particles;
    u32 count = work->params.particleCount;
    s32 fadeIn;
    s32 fadeRange;
    u32 delay;
    s32 age;
    s32 color;
    s32 remaining;
    f32 total;
    f32 t;
    u32 i;

    loop = work->params.loop;
    fadeIn = work->params.fadeIn;
    fadeRange = work->params.fadeRange;
    delay = work->params.randomDelayRange;
    color = effMultiplyPackedColors(work->color, work->params.colorParam);
    age = work->age;
    if (duration < age) {
        return;
    }
    remaining = duration - age;
    if (fadeRange >= remaining && fadeRange != 0) {
        total = (f32)remaining / (f32)fadeRange;
    } else {
        total = 1.0f;
    }
    for (i = 0; i < count; i++) {
        s32 particleAge = particle->age;

        if (duration < particleAge) {
            draw->colors[i] = 0;
        } else {
            if (particleAge == 0) {
                effScatterRingInitScaled(work, i);
            } else if (particleAge > 0) {
                if (particleAge < fadeIn && fadeIn != 0) {
                    t = (f32)particleAge / (f32)fadeIn;
                } else {
                    t = 1.0f;
                }
                draw->colors[i] = effBlendColor(color & 0xFFFFFF, color, t * total);
                effScatterRingUpdateScaled(work, i);
            }
            if (loop != 0 && !(age < duration)) {
                particle->age = -(effMiscRand(D_003AA868) % delay);
            } else {
                particle->age++;
            }
        }
        particle++;
    }
    if (loop != 0 && age >= duration) {
        work->age = 0;
    } else {
        work->age++;
    }
    draw->scale = work->scale;
    PCP_COPY_VECTOR(draw, work->params.vec);
    effScatterStoreSourceTransformMatrix(draw, work);
    func_0017DA28(draw);
}

void func_0017C250(PcpScatterInstanceB *work, void *src) {
    PCP_COPY_VECTOR(&work->params, src);
}

void effScatterSetInstanceScale(PcpScatterInstanceB *work, f32 value)
{
    work->scale = value;
}

void effScatterSetInstanceColor(PcpScatterInstanceB *work, u32 value) {
    work->color = value;
}

/* vu0 routine: matrix = (matrix + 0x50) * src, via the vf28-vf31 by vf24-vf27 product routine */
void effScatterComposeWorkMatrix(PcpScatterInstanceB *work, void *source) {
    VU0_LOAD_MATRIX(source);
    VU0_LOAD_MATRIX_B(work->params.matrix);
    sdfComposeVuMatrixFromRegisters();
    VU0_STORE_MATRIX(work);
}


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
    inst->color = 0x80808080;
    inst->scale = 1.0f;
    inst->ownedBuffer = allocation;
    inst->particles = particle;
    inst->age = 0;
    VU0_COPY_MATRIX(inst->matrix, src->matrix);
    object = func_0017D7A8(src->particleCount, src->unk60);
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
        particle->age = -(effMiscRand(D_003AA868) % mod);
        particle++;
    }
    return inst;
}

void effScatterCreateFromParameterTable(u64 table) {
    u64 particleParams;
    u64 resource;

    particleParams = effParamTableGetBlock(table, 0);
    resource = effParamTableGetBlock(table, 1);
    effScatterInstanceCreateC(particleParams, resource);
}

PcpScatterInstanceC *effCreateScatterChildSharingParentResource(PcpScatterInstanceC *work)
{
    PcpScatterInstanceC *child;

    child = effScatterInstanceCreateC(&work->params, NULL);
    effShareScatterResource(child->scatterObject, work->scatterObject);
    return child;
}

void effReleaseScatterObjectAndOwnedBuffer(PcpScatterInstanceC *work) {
    effReleaseScatterObject(work->scatterObject);
    func_003297C8(work->ownedBuffer);
}


/* Initialise ring `index`, staggering its rise and radius by index over the lifetime, then its first vertex pairs. */
void effScatterInitStaggeredRing(PcpScatterInstanceC *work, s32 index)
{
    f32 *vertex = (f32 *)effGetScatterWideBlock(work->scatterObject, index);
    f32 *uv = (f32 *)effGetScatterNarrowBlock(work->scatterObject, index);
    PcpScatterParticle *ring;
    f32 angle;
    f32 angleStep;
    f32 radius;
    f32 height;
    f32 rise;
    f32 u;
    f32 du;
    f32 v;
    f32 jitter;
    f32 s;
    u32 count;
    u32 i;

    ring = &work->particles[index];
    count = ((PcpScatterBlockObject *)work->scatterObject)->stride >> 1;
    angle = effMiscRandUnitFloat(D_003AA868) * (3.14159265f * 2.0f);
    jitter = work->params.angleStepJitter;
    angleStep = work->params.angleStepBase * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) / (f32)count;
    jitter = work->params.radiusJitter;
    radius = work->params.radiusBase * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    jitter = work->params.heightOffsetJitter;
    height = work->params.heightOffsetBase * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    jitter = work->params.radiusStepJitter;
    ring->radiusStep = work->params.radiusStepBase * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    ring->unk00 = work->params.tiltScale * effMiscRandUnitFloat(D_003AA868);
    ring->tiltAngle = 0;
    rise = work->params.riseRange / (f32)work->params.particleCount * (f32)index;
    radius = radius + work->params.radiusRamp * (f32)(work->params.particleCount - index);
    ring->angle = angle;
    ring->heightOffset = height;
    ring->angleStep = angleStep;
    ring->radius = radius;
    ring->tiltSpeed = work->params.initialTiltSpeed;
    ring->rise = work->params.initialRise;
    func_003364B8(ring->unk00);
    func_00336818(ring->tiltAngle);
    sdfMultiplyVuMatrixInPlace();
    v = (f32)work->params.vCount;
    u = 0.0f;
    du = (f32)work->params.vTail / (f32)count;
    for (i = 0; i < count; i++) {
        vertex[0] = sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
        vertex[1] = rise;
        s = sdfSinPoly(angle) * radius;
        vertex[4] = vertex[0];
        vertex[6] = vertex[2] = s;
        vertex[5] = vertex[1] - height;
        VU0_LOAD_VF(vf10, vertex);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, vertex);
        VU0_LOAD_VF(vf10, vertex + 4);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, vertex + 4);
        uv[0] = u;
        uv[2] = u;
        uv[1] = 0;
        uv[3] = v;
        uv += 4;
        angle += angleStep;
        u += du;
        vertex += 8;
    }
}


/* Same ring update as effScatterRingUpdateScaled on a work area with a longer header. */
void effScatterRingUpdateScaledLong(PcpScatterInstanceC *work, s32 index)
{
    f32 *vertex = (f32 *)effGetScatterWideBlock(work->scatterObject, index);
    PcpScatterParticle *ring;
    f32 radius;
    f32 rise;
    f32 angle;
    f32 step;
    u32 count;
    u32 i;

    effGetScatterNarrowBlock(work->scatterObject, index);
    ring = &work->particles[index];
    radius = ring->radius + ring->radiusStep;
    count = ((PcpScatterBlockObject *)work->scatterObject)->stride >> 1;
    rise = ring->rise;
    angle = ring->angle;
    step = ring->angleStep;
    func_003364B8(ring->unk00);
    func_00336818(ring->tiltAngle);
    sdfMultiplyVuMatrixInPlace();
    ring->tiltAngle += ring->tiltSpeed;
    ring->tiltSpeed *= work->params.tiltDamping;
    ring->radius = radius;
    ring->radiusStep *= work->params.radiusDamping;
    ring->rise = rise * work->params.riseDecay;
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


/* Per-frame update of a fading, optionally looping scatter instance whose colour blends between two keys over its lifetime. */
void effScatterUpdateDualColor(PcpScatterInstanceC *work) {
    s32 loop;
    s32 duration = work->params.duration;
    PcpScatterDraw *draw = (PcpScatterDraw *)work->scatterObject;
    PcpScatterParticle *particle = work->particles;
    u32 count = work->params.particleCount;
    s32 fadeIn;
    s32 fadeRange;
    u32 delay;
    s32 age;
    s32 remaining;
    f32 total;
    f32 t;
    f32 u;
    s32 colorMul;
    s32 colorA;
    s32 colorB;
    s32 color;
    u32 i;

    age = work->age;
    loop = work->params.loop;
    fadeIn = work->params.fadeIn;
    fadeRange = work->params.fadeRange;
    delay = work->params.randomDelayRange;
    colorA = work->params.colorA;
    colorB = work->params.colorB;
    colorMul = work->color;
    if (duration < age) {
        return;
    }
    remaining = duration - age;
    if (fadeRange >= remaining && fadeRange != 0) {
        total = (f32)remaining / (f32)fadeRange;
    } else {
        total = 1.0f;
    }
    for (i = 0; i < count; i++) {
        s32 particleAge = particle->age;

        if (duration < particleAge) {
            draw->colors[i] = 0;
        } else {
            if (particleAge == 0) {
                effScatterInitStaggeredRing(work, i);
            } else if (particleAge > 0) {
                u = (f32)particleAge;
                color = effBlendColor(colorA, colorB, u / (f32)duration);
                if (particleAge < fadeIn && fadeIn != 0) {
                    t = u / (f32)fadeIn;
                } else {
                    t = 1.0f;
                }
                color = effMultiplyPackedColors(colorMul, color);
                draw->colors[i] = effBlendColor(color & 0xFFFFFF, color, t * total);
                effScatterRingUpdateScaledLong(work, i);
            }
            if (loop != 0 && !(age < duration)) {
                particle->age = -(effMiscRand(D_003AA868) % delay);
            } else {
                particle->age++;
            }
        }
        particle++;
    }
    if (loop != 0 && age >= duration) {
        work->age = 0;
    } else {
        work->age++;
    }
    draw->scale = work->scale;
    PCP_COPY_VECTOR(draw, work->params.vec);
    effScatterStoreSourceTransformMatrix(draw, work);
    func_0017DA28(draw);
}

void func_0017CE00(PcpScatterInstanceC *work, void *src) {
    PCP_COPY_VECTOR(&work->params, src);
}

void effSetScatterWorkScale(PcpScatterInstanceC *work, f32 value)
{
    work->scale = value;
}

void effSetScatterWorkColor(PcpScatterInstanceC *work, u32 value) {
    work->color = value;
}

/* vu0 routine: matrix = (matrix + 0x50) * src, via the vf28-vf31 by vf24-vf27 product routine */
void func_0017CE28(PcpScatterInstanceC *work, void *source) {
    VU0_LOAD_MATRIX(source);
    VU0_LOAD_MATRIX_B(work->params.matrix);
    sdfComposeVuMatrixFromRegisters();
    VU0_STORE_MATRIX(work);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017CE88);

void func_0017D078(u64 table) {
    u64 shared;
    u64 local;

    shared = effParamTableGetBlock(table, 0);
    local = effParamTableGetBlock(table, 1);
    func_0017CE88(shared, local);
}

PcpScatterWork7 *effCloneScatterWithSharedResource(PcpScatterWork7 *work)
{
    PcpScatterWork7 *child;

    child = func_0017CE88(&work->particleParams, NULL);
    effShareScatterResource(child->scatterObject, work->scatterObject);
    return child;
}

void effReleaseScatterWorkResources(PcpScatterWork7 *work) {
    effReleaseScatterObject(work->scatterObject);
    func_003297C8(work->ownedBuffer);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017D138);

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

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017D560);
