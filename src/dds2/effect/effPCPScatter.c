#include "common.h"

#include "pcp_vu0.h"
#include "ee_mmi.h"

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

extern void *sdfAllocSizeClassBlock(s32 size);

extern u32 sdfTexAcquireResourceTexture(u32 resId);

extern PcpScatterRes *effPcpScatterResCreate(u32 resId);

typedef struct PcpScatterInstance PcpScatterInstance;


extern PcpScatterInstance *effPcpScatterCreateParticleInstance();

extern void effShareScatterResource(u32 param0, u32 param1);

typedef struct PcpScatterInstanceB PcpScatterInstanceB;

typedef struct PcpScatterParticle PcpScatterParticle;
typedef struct PcpScatterDraw PcpScatterDraw;

/* The allocator creates one 0x80-byte drawable plus separate vector, UV and
 * color arrays. Geometry and the per-particle fade pass share this owner. */
struct PcpScatterDraw {
    f32 origin[4];
    f32 matrix[16];
    u32 unk50;
    u32 color;
    u32 particleCount;
    s32 stride; /* Coordinate vectors per particle; two vectors form a pair. */
    f32 scale;
    f32 *points;
    f32 *uv;
    u32 *vertexColors;
    u32 *colors;
    u32 asset;
    u32 allocation;
    PcpScatterRes *sharedResource;
}; /* 0x80 */

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

extern void *effPcpScatterCreatePlainInstance();

typedef struct PcpScatterPlainInstance PcpScatterPlainInstance;


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

/* The linked variant has a 0x84-byte header before its particle array;
 * scaling, duplication and teardown all operate on this owner. */
typedef struct PcpScatterWork8 {
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
    PcpScatterPool *childWork;
    u32 ownedResource;
    u32 duplicatedCount;
    u32 *duplicatedHandles;
    u32 duplicateAllocation;
} PcpScatterWork8;

typedef struct PcpScatterWork2 {
    u8 pad00[0x40];
    f32 unk40;
    f32 unk44;
    u8 pad48[0x24];
    u32 unk6C;
} PcpScatterWork2;

extern u32 effParamWorkDuplicate(u32 param);

extern void effPcpScatterSharePoolResource(PcpScatterPool *dst, PcpScatterPool *src);

extern u32 sdfAllocGeneralBlock(u32 size);

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


typedef struct PcpScatterRadialParticle {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 radius;
    f32 angle;
    f32 unk18;
} PcpScatterRadialParticle;

/* func_001708A0 */
struct PcpScatterWork1 {
    u8 pad00[0x20];
    u32 particleCount;
    u32 radialSegments;
    u8 pad28[0xC];
    s32 duration;
    u8 pad38[4];
    f32 unk3C;
    u32 unk40;
    f32 unk44;
    u32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 radiusJitter;
    f32 targetRadiusJitter;
    f32 speedJitter;
    u8 pad60[8];
    u8 duplicateParticles;
    u8 pad69[7];
    u32 particlesPerGroup;
    PcpScatterRadialParticle *particles;
    u8 pad78[8];
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

/* The table variant's copied header has a byte flag at 0x40. Whether it
 * shares an argument layout with the scaler's float input is unresolved. */
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
        handle = sdfAllocGeneralBlock(count * 4);
        buf = sdfResourceRetainAddress(handle);
        work->duplicateAllocation = handle;
        work->duplicatedHandles = buf;
        for (i = 0; i < count; i++) {
            work->duplicatedHandles[i] = effParamWorkDuplicate(*src->duplicatedHandles);
        }
    }
    return work;
}


extern void effDispatchParameterDataAndFreeWork(u32 particle);


void effPcpScatterReleaseParticleGroup(PcpScatterWork1 *work) {
    if (work->duplicateAllocation != 0) {
        u32 count = work->duplicatedCount;
        u32 i;

        for (i = 0; i < count; i++) {
            effDispatchParameterDataAndFreeWork(work->duplicatedHandles[i]);
        }
        sdfReleaseResourceAllocation(work->duplicateAllocation);
    }
    effPcpScatterReleasePoolResources(work->childWork);
    sdfReleaseResourceAllocation(work->ownedResource);
}

void func_001789C0(PcpScatterWork1 *work, u32 index) {
    u32 segments = work->radialSegments;
    PcpScatterRadialParticle *particle = &work->particles[index];
    f32 angleStep;
    f32 jitter;

    if (segments == 0) {
        segments = 1;
    }
    angleStep = 6.2831852f / segments;
    particle->angle = angleStep * (index % segments) + angleStep * 0.5f * effMiscRandUnitFloat(D_003AA868);
    jitter = work->radiusJitter;
    particle->radius = work->unk4C * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    particle->unk18 = 0.0f;
    jitter = work->targetRadiusJitter;
    particle->unk04 = (work->unk50 * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) - particle->radius) / work->duration;
    jitter = work->speedJitter;
    particle->unk0C = work->unk3C * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    particle->unk00 = 0.0f;
    particle->unk08 = work->unk44;
}

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


PcpScatterWork8 *effPcpScatterLinkedDuplicate(src)
    PcpScatterWork8 *src;
{
    PcpScatterWork8 *work;
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
        handle = sdfAllocGeneralBlock(count * 4);
        buf = sdfResourceRetainAddress(handle);
        work->duplicateAllocation = handle;
        work->duplicatedHandles = buf;
        for (i = 0; i < count; i++) {
            work->duplicatedHandles[i] = effParamWorkDuplicate(*src->duplicatedHandles);
        }
    }
    return work;
}


void effPcpScatterReleaseSharedParticles(PcpScatterWork8 *work) {
    if (work->duplicateAllocation != 0) {
        u32 count = work->duplicatedCount;
        u32 i;

        for (i = 0; i < count; i++) {
            effDispatchParameterDataAndFreeWork(work->duplicatedHandles[i]);
        }
        sdfReleaseResourceAllocation(work->duplicateAllocation);
    }
    effPcpScatterReleasePoolResources(work->childWork);
    sdfReleaseResourceAllocation(work->ownedResource);
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
        handle = sdfAllocGeneralBlock(count * 4);
        buf = sdfResourceRetainAddress(handle);
        work->duplicateAllocation = handle;
        work->duplicatedHandles = buf;
        for (i = 0; i < count; i++) {
            work->duplicatedHandles[i] = effParamWorkDuplicate(*src->duplicatedHandles);
        }
    }
    return work;
}


void effPcpScatterReleaseLinkedParticles(PcpScatterWork2Copy *work) {
    if (work->duplicateAllocation != 0) {
        u32 count = work->duplicatedCount;
        u32 i;

        for (i = 0; i < count; i++) {
            effDispatchParameterDataAndFreeWork(work->duplicatedHandles[i]);
        }
        sdfReleaseResourceAllocation(work->duplicateAllocation);
    }
    effPcpScatterReleasePoolResources(work->childWork);
    sdfReleaseResourceAllocation(work->ownedResource);
}

typedef struct PcpScatterInitEntry {
    /* active, state, initial value, three angles, height, phase, direction X/Z */
    f32 values[10];
} PcpScatterInitEntry;

typedef struct PcpScatterInitWork {
    u8 pad00[0x38];
    f32 initialValue;
    u8 pad3C[0x10];
    PcpScatterInitEntry *entries;
} PcpScatterInitWork;

void func_0017A248(PcpScatterInitWork *work, s32 index)
{
    f32 halfTurn = 90.0f * 0.017453292f;
    f32 quarterTurn = 45.0f * 0.017453292f;
    f32 smallAngle = 5.0f * 0.017453292f;
    PcpScatterInitEntry *entry = &work->entries[index];
    f32 direction[4];

    entry->values[1] = 0.0f;
    entry->values[2] = work->initialValue;
    entry->values[4] = halfTurn;
    entry->values[3] = quarterTurn;
    entry->values[5] = smallAngle;
    entry->values[7] = effMiscRandUnitFloat(D_003AA868) * 6.2831850051879883f;
    entry->values[6] = 200.0f;

    direction[0] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    direction[1] = 0.0f;
    direction[2] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    VU0_NORMALIZE_PACKED_VECTOR(direction);

    entry->values[8] = direction[0];
    entry->values[0] = 0.0f;
    entry->values[9] = direction[2];
}

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
    handle = sdfAllocGeneralBlock(size);
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
    sdfReleaseResourceAllocation(pool->buffer);
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

    res = sdfAllocSizeClassBlock(8);
    res->resourceHandle = sdfTexAcquireResourceTexture(resId);
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

/* The first ring variant copies this complete 0x138-byte parameter block.
 * Geometry and lifetime control read the copy embedded at instance +0x40. */
typedef struct PcpScatterParams {
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
    f32 targetRadius;
    f32 targetRadiusJitter;
    s32 colorParam;
    u32 vCount;
    u32 vTail;
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

/* One 0x18C-byte owner shared by creation, geometry, setters and teardown. */
struct PcpScatterInstance {
    f32 matrix[16];
    PcpScatterParams params;
    PcpScatterParticle *particles;
    f32 scale;
    u32 color;
    u32 scatterObject;
    u32 ownedBuffer;
};

extern void *func_0017D7A8();

extern void effCreateScatterResource(void *object, u32 resource);

extern u32 effMiscRand(void *state);

/* Allocate particles after the scatter work, then assign randomized offsets. */
PcpScatterInstance *effPcpScatterCreateParticleInstance(src, resource)
    PcpScatterParams *src;

    u32 resource;

{
    u32 allocation = sdfAllocGeneralBlock(src->particleCount * 0x28 + 0x18C);
    PcpScatterInstance *inst = (PcpScatterInstance *)sdfResourceRetainAddress(allocation);
    PcpScatterParticle *particle;
    u32 mod;
    u32 count;
    u32 i;
    PcpScatterDraw *object;
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
    object->unk50 = unk50;
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
    return inst;
}

PcpScatterInstance *effScatterBlockDuplicate(u64 table) {
    u64 particleParams = effParamTableGetBlock(table, 0);
    u64 resource = effParamTableGetBlock(table, 1);

    return effPcpScatterCreateParticleInstance(particleParams, resource);
}

PcpScatterInstance *effScatterCloneWithSharedObject(PcpScatterInstance *work) {
    PcpScatterInstance *child;

    child = effPcpScatterCreateParticleInstance(&work->params, NULL);
    effShareScatterResource(child->scatterObject, work->scatterObject);
    return child;
}

void effScatterReleaseObjectAndBuffer(PcpScatterInstance *work) {
    effReleaseScatterObject(work->scatterObject);
    sdfReleaseResourceAllocation(work->ownedBuffer);
}




/* Initialise ring `index`: randomised radius/angle/rise parameters, then the first set of vertex pairs. */
void effScatterRingInit(PcpScatterInstance *work, s32 index)
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
    count = ((PcpScatterDraw *)work->scatterObject)->stride >> 1;
    angle = effMiscRandUnitFloat(D_003AA868) * (3.14159265f * 2.0f);
    jitter = work->params.angleStepJitter;
    angleStep = work->params.angleStepBase * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) / (f32)count;
    riseStep = work->params.riseStep;
    jitter = work->params.radiusJitter;
    radius = work->params.radiusBase * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    jitter = work->params.heightOffsetJitter;
    height = work->params.heightOffsetBase * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    jitter = work->params.targetRadiusJitter;
    rise = 0.0f;
    ring->radiusStep = (work->params.targetRadius * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) - radius) / (f32)work->params.duration;
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

/* Advance ring `index`: rebuild the rotation matrix, then lay the ring's vertex pairs around it. */
void effScatterRingUpdate(PcpScatterInstance *work, s32 index)
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
    count = ((PcpScatterDraw *)work->scatterObject)->stride >> 1;
    rise = ring->rise;
    angle = ring->angle;
    step = ring->angleStep;
    func_003364B8(ring->unk00);
    func_00336818(ring->tiltAngle);
    sdfMultiplyVuMatrixInPlace();
    ring->tiltAngle += ring->tiltSpeed;
    ring->tiltSpeed *= work->params.tiltDamping;
    ring->radius = radius;
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

extern s32 effMultiplyPackedColors(s32 color, s32 param);
extern u32 effBlendColor(u32 colorA, u32 colorB, f32 t);
extern void effScatterStoreSourceTransformMatrix(void *draw, void *work);
extern void func_0017DA28(void *draw);
void effScatterUpdateLoopedParticleRing(PcpScatterInstance *work) {
    s32 loop;
    u32 i;
    u32 count = work->params.particleCount;
    PcpScatterDraw *draw = (PcpScatterDraw *)work->scatterObject;
    PcpScatterParticle *particle = work->particles;
    s32 duration = work->params.duration;
    s32 fadeIn;
    s32 fadeRange;
    s32 color;
    loop = work->params.loop;
    fadeIn = work->params.fadeIn;
    fadeRange = work->params.fadeRange;
    color = effMultiplyPackedColors(work->color, work->params.colorParam);

    for (i = 0; i < count; i++) {
        s32 age = particle->age;
        if (duration < age) {
            draw->colors[i] = 0;
        } else {
            if (age == 0) {
                effScatterRingInit(work, i);
            } else if (age > 0) {
                s32 remaining = duration - age;
                f32 factor;
                if (age < fadeIn && fadeIn != 0) {
                    factor = (f32)age / (f32)fadeIn;
                } else if (fadeRange >= remaining && fadeRange != 0) {
                    factor = (f32)remaining / (f32)fadeRange;
                } else {
                    factor = 1.0f;
                }
                draw->colors[i] = effBlendColor(color & 0xFFFFFF, color, factor);
                effScatterRingUpdate(work, i);
            }
            if (loop != 0 && age >= duration) {
                particle->age = 0;
            } else {
                particle->age++;
            }
        }
        particle++;
    }
    draw->scale = work->scale;
    PCP_COPY_VECTOR(draw->origin, work->params.vec);
    effScatterStoreSourceTransformMatrix(draw, work);
    func_0017DA28(draw);
}

void effScatterCopyParticleParameterVector(PcpScatterInstance *work, void *src) {
    PCP_COPY_VECTOR(&work->params, src);
}

void effScatterSetParticleScale(PcpScatterInstance *work, f32 value)
{
    work->scale = value;
}

void effScatterSetParticleColor(PcpScatterInstance *work, u32 value) {
    work->color = value;
}

/* vu0 routine: matrix = (matrix + 0x50) * src, via the vf28-vf31 by vf24-vf27 product routine */
void effPcpScatterTransformMatrix(PcpScatterInstance *work, void *source) {
    VU0_LOAD_MATRIX(source);
    VU0_LOAD_MATRIX_B(work->params.matrix);
    sdfComposeVuMatrixFromRegisters();
    VU0_STORE_MATRIX(work);
}


/* Same particle layout with a longer parameter block and one extra control word. */
void *effScatterInstanceCreateB(src, resource)
    PcpScatterParamsB *src;

    u32 resource;

{
    u32 allocation = sdfAllocGeneralBlock(src->particleCount * 0x28 + 0x194);
    PcpScatterInstanceB *inst = (PcpScatterInstanceB *)sdfResourceRetainAddress(allocation);
    PcpScatterParticle *particle;
    u32 mod;
    u32 count;
    u32 i;
    PcpScatterDraw *object;
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
    object->unk50 = unk50;
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
    sdfReleaseResourceAllocation(work->ownedBuffer);
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
    count = ((PcpScatterDraw *)work->scatterObject)->stride >> 1;
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
    count = ((PcpScatterDraw *)work->scatterObject)->stride >> 1;
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
    PCP_COPY_VECTOR(draw->origin, work->params.vec);
    effScatterStoreSourceTransformMatrix(draw, work);
    func_0017DA28(draw);
}

void effScatterSetScaledRingOrigin(PcpScatterInstanceB *work, void *src) {
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
    u32 allocation = sdfAllocGeneralBlock(src->particleCount * 0x28 + 0x19C);
    PcpScatterInstanceC *inst = (PcpScatterInstanceC *)sdfResourceRetainAddress(allocation);
    PcpScatterParticle *particle;
    u32 mod;
    u32 count;
    u32 i;
    PcpScatterDraw *object;
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
    object->unk50 = unk50;
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
    sdfReleaseResourceAllocation(work->ownedBuffer);
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
    count = ((PcpScatterDraw *)work->scatterObject)->stride >> 1;
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
    count = ((PcpScatterDraw *)work->scatterObject)->stride >> 1;
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
    PCP_COPY_VECTOR(draw->origin, work->params.vec);
    effScatterStoreSourceTransformMatrix(draw, work);
    func_0017DA28(draw);
}

void effScatterSetDualColorOrigin(PcpScatterInstanceC *work, void *src) {
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
void effScatterComposeParticleMatrix(PcpScatterInstanceC *work, void *source) {
    VU0_LOAD_MATRIX(source);
    VU0_LOAD_MATRIX_B(work->params.matrix);
    sdfComposeVuMatrixFromRegisters();
    VU0_STORE_MATRIX(work);
}

/* The flat-ring variant copies 0xE8 bytes; its motion fields and lifetime
 * controls remain in the same parameter block used by the initializer. */
typedef struct PcpScatterPlainParams {
    f32 vec[4];
    u32 unk10;
    u8 loop;
    u8 pad15[3];
    s32 duration;
    u32 particleCount;
    u32 unk20;
    u32 randomDelayRange;
    s32 fadeIn;
    s32 fadeRange;
    f32 angleStepBase;
    f32 angleStepJitter;
    f32 heightBase;
    f32 heightJitter;
    f32 angularSpeed;
    f32 angularDamping;
    f32 radiusBase;
    f32 radiusJitter;
    f32 radialSpeed;
    f32 radialDamping;
    s32 radialDecayStart;
    s32 colorParam;
    u32 vCount;
    u32 vTail;
    u8 pad68[0x80];
} PcpScatterPlainParams;

/* The flat particle has three rotation angles before its signed age;
 * it is not interchangeable with the other variants' 0x28-byte particle. */
typedef struct PcpScatterPlainParticle {
    f32 rot[3];
    s32 age;
    f32 angle;
    f32 angularSpeed;
    f32 angleStep;
    f32 radius;
    f32 radialSpeed;
    f32 height;
} PcpScatterPlainParticle;

/* One 0x13C-byte owner shared by creation, flat-ring update and teardown. */
struct PcpScatterPlainInstance {
    f32 matrix[16];
    PcpScatterPlainParams params;
    PcpScatterPlainParticle *particles;
    f32 scale;
    u32 color;
    u32 scatterObject;
    u32 ownedBuffer;
};

/* Allocate particles after the scatter work (identity matrix), then assign randomized start delays. */
void *effPcpScatterCreatePlainInstance(src, resource)
    PcpScatterPlainParams *src;
    u32 resource;
{
    u32 allocation = sdfAllocGeneralBlock(src->particleCount * 0x28 + 0x13C);
    PcpScatterPlainInstance *inst = (PcpScatterPlainInstance *)sdfResourceRetainAddress(allocation);
    PcpScatterPlainParticle *particle;
    u32 mod;
    u32 count;
    u32 i;
    PcpScatterDraw *object;
    u32 unk50;

    particle = (PcpScatterPlainParticle *)((u8 *)inst + 0x13C);
    inst->params = *src;
    inst->color = 0x80808080;
    inst->scale = 1.0f;
    inst->ownedBuffer = allocation;
    inst->particles = particle;
    EE_MMI_UNIT_MATRIX(inst->matrix);
    object = func_0017D7A8(src->particleCount, src->unk20);
    unk50 = src->unk10;
    inst->scatterObject = (u32)object;
    object->unk50 = unk50;
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
    return inst;
}

void func_0017D078(u64 table) {
    u64 shared;
    u64 local;

    shared = effParamTableGetBlock(table, 0);
    local = effParamTableGetBlock(table, 1);
    effPcpScatterCreatePlainInstance(shared, local);
}

PcpScatterPlainInstance *effCloneScatterWithSharedResource(PcpScatterPlainInstance *work)
{
    PcpScatterPlainInstance *child;

    child = effPcpScatterCreatePlainInstance(&work->params, NULL);
    effShareScatterResource(child->scatterObject, work->scatterObject);
    return child;
}

void effReleaseScatterWorkResources(PcpScatterPlainInstance *work) {
    effReleaseScatterObject(work->scatterObject);
    sdfReleaseResourceAllocation(work->ownedBuffer);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017D138);


/* Advance ring `index` and lay its vertex pairs around a flat circle rotated by the ring's own angles. */
void effScatterFlatRingUpdate(PcpScatterPlainInstance *work, s32 index)
{
    f32 *vertex = (f32 *)effGetScatterWideBlock(work->scatterObject, index);
    PcpScatterPlainParticle *ring;
    f32 angle;
    f32 radius;
    f32 step;
    f32 height;
    f32 s;
    u32 count;
    u32 i;

    effGetScatterNarrowBlock(work->scatterObject, index);
    ring = &work->particles[index];
    count = ((PcpScatterDraw *)work->scatterObject)->stride >> 1;
    angle = ring->angle + ring->angularSpeed;
    radius = ring->radius;
    step = ring->angleStep;
    radius += ring->radialSpeed;
    if (ring->age >= work->params.radialDecayStart) {
        ring->radialSpeed *= work->params.radialDamping;
    }
    vu0RotMatrixXYZFromVec3(ring->rot);
    ring->radius = radius;
    ring->angle = angle;
    ring->angularSpeed *= work->params.angularDamping;
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

extern void func_0017D138(PcpScatterPlainInstance *work, s32 index);

void effScatterUpdatePlainParticleRing(PcpScatterPlainInstance *work) {
    s32 loop;
    u32 i;
    u32 count = work->params.particleCount;
    PcpScatterDraw *draw = (PcpScatterDraw *)work->scatterObject;
    PcpScatterPlainParticle *particle = work->particles;
    s32 duration = work->params.duration;
    s32 fadeIn;
    s32 fadeRange;
    s32 color;
    loop = work->params.loop;
    fadeIn = work->params.fadeIn;
    fadeRange = work->params.fadeRange;
    color = effMultiplyPackedColors(work->color, work->params.colorParam);

    for (i = 0; i < count; i++) {
        s32 age = particle->age;
        if (duration < age) {
            draw->colors[i] = 0;
        } else {
            if (age == 0) {
                func_0017D138(work, i);
            } else if (age > 0) {
                s32 remaining = duration - age;
                f32 factor;
                if (age < fadeIn && fadeIn != 0) {
                    factor = (f32)age / (f32)fadeIn;
                } else if (fadeRange >= remaining && fadeRange != 0) {
                    factor = (f32)remaining / (f32)fadeRange;
                } else {
                    factor = 1.0f;
                }
                draw->colors[i] = effBlendColor(color & 0xFFFFFF, color, factor);
                effScatterFlatRingUpdate(work, i);
            }
            if (loop != 0 && age >= duration) {
                particle->age = 0;
            } else {
                particle->age++;
            }
        }
        particle++;
    }
    draw->scale = work->scale;
    PCP_COPY_VECTOR(draw->origin, work->params.vec);
    effScatterStoreSourceTransformMatrix(draw, work);
    func_0017DA28(draw);
}
