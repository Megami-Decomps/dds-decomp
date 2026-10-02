#include "common.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

/* Shared resource handed between scatter effects. effPcpScatterResCreate creates it,
   effPcpScatterResAddRef takes a reference, effPcpScatterResRelease releases it. */
typedef struct PcpScatterRes PcpScatterRes;

/* Ownership handles refer to SDF allocation nodes, not their retained payloads. */
typedef struct SdfMemoryBlock SdfMemoryBlock;


struct PcpScatterRes {
    u32 resourceHandle;
    s32 refCount;
};

extern void *effParamTableGetBlock(void *data, s32 index);

extern void *sdfAllocSizeClassBlock(s32 size);
extern u32 effParamWorkDuplicate(u32 param);
extern SdfMemoryBlock *sdfAllocGeneralBlock(u32 size);
extern void *sdfResourceRetainAddress(SdfMemoryBlock *block);
extern void sdfReleaseChipBlock(void *ptr);

extern void effReleaseScatterObject(u32 res);
extern void sdfQueueAssetRelease(u32 res);
extern void sdfReleaseResourceAllocation(SdfMemoryBlock *block);
extern u32 sdfTexAcquireResourceTexture(u32 resId);
extern void sdfTexReleaseReferenceViaHandler(u32 res);
extern void effPcpScatterResRelease(PcpScatterRes *res);
extern PcpScatterRes *effPcpScatterResAddRef(PcpScatterRes *res);

extern void effDispatchParameterDataAndFreeWork(u32 handle);
extern void sdfComposeVuMatrixFromRegisters(void);

typedef struct PcpScatterRadialWork PcpScatterRadialWork;
typedef struct PcpScatterSpinWork PcpScatterSpinWork;
typedef struct PcpScatterRibbonWork PcpScatterRibbonWork;

/* Constructors also serve the legacy parameter-table dispatch surface. */
extern PcpScatterRadialWork *func_001708A0();
extern PcpScatterSpinWork *func_00171550();
extern PcpScatterRibbonWork *func_00172158();
extern void *effScatterInstanceCreateB();
extern void *effScatterInstanceCreateC();
extern void *effPcpScatterCreatePlainInstance();


/* Serialized parameter heads precede each scatter effect's runtime state. */
typedef struct PcpScatterInstance PcpScatterInstance;
typedef struct PcpScatterInstanceB PcpScatterInstanceB;
typedef struct PcpScatterInstanceC PcpScatterInstanceC;
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
    SdfMemoryBlock *buffer;
    PcpScatterRes *sharedResource;
} PcpScatterPool;
extern void effPcpScatterSharePoolResource(PcpScatterPool *work, PcpScatterPool *src);

extern PcpScatterPool *effPcpScatterPoolCreate(s32 groups);
extern void effPcpScatterCreatePoolResource(PcpScatterPool *work, u32 resId);
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

extern void effPcpScatterReleasePoolResources(PcpScatterPool *work);

typedef struct {
    f32 origin[4];
    f32 width;
    f32 height;
    u32 poolMode;
    u8 respawn;
    u8 pad1D[3];
    u32 particleCount;
    u32 radialSegments;
    s32 delaySpread;
    u32 fadeIn;
    u32 fadeOut;
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
    u8 pad69[3];
    s32 duplicateStartAge;
    u32 particlesPerGroup;
} PcpScatterRadialParams;

typedef struct {
    s32 age;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 radius;
    f32 angle;
    f32 unk18;
} PcpScatterRadialParticle;

struct PcpScatterRadialWork {
    PcpScatterRadialParams params;
    PcpScatterRadialParticle *particles;
    f32 scale;
    u32 color;
    PcpScatterPool *childWork;
    SdfMemoryBlock *ownedResource;
    u32 duplicatedCount;
    u32 *duplicatedHandles;
    SdfMemoryBlock *duplicateAllocation;
};

typedef struct {
    f32 origin[4];
    f32 width;
    f32 height;
    u32 poolMode;
    u8 respawn;
    u8 pad1D[3];
    u32 particleCount;
    s32 delaySpread;
    u32 fadeIn;
    u32 fadeOut;
    s32 duration;
    s32 fadeDuration;
    f32 angleStep;
    f32 angleDamping;
    f32 startRadius;
    f32 endRadius;
    f32 startRadiusJitter;
    f32 endRadiusJitter;
    u8 adjustAngle;
    u8 pad51[3];
    f32 endAngleStep;
    u8 duplicateParticles;
    u8 pad59[3];
    s32 duplicateStartAge;
    u32 particlesPerGroup;
} PcpScatterSpinParams; /* 0x64 copied by the constructor. */

typedef struct {
    s32 age;
    f32 radiusStep;
    f32 angleStep;
    f32 radius;
    f32 angle;
    f32 dirX;
    f32 dirY;
    f32 dirZ;
} PcpScatterSpinParticle; /* 0x20 */

struct PcpScatterSpinWork {
    PcpScatterSpinParams params;
    PcpScatterSpinParticle *particles;
    f32 scale;
    u32 color;
    PcpScatterPool *childWork;
    SdfMemoryBlock *ownedResource;
    u32 duplicatedCount;
    u32 *duplicatedHandles;
    SdfMemoryBlock *duplicateAllocation;
};

typedef struct {
    f32 origin[4];
    f32 width;
    f32 height;
    u32 poolMode;
    u8 respawn;
    u8 pad1D[3];
    u32 particleCount;
    s32 delaySpread;
    u32 fadeIn;
    u32 fadeOut;
    s32 duration;
    s32 fadeDuration;
    f32 heightStep;
    u8 pad3C[4];
    u8 duplicateParticles;
    u8 pad41[3];
    s32 duplicateStartAge;
    u32 particlesPerGroup;
} PcpScatterRibbonParams; /* 0x4C copied by the constructor. */

typedef struct {
    s32 age;
    f32 height;
    f32 heightStep;
    f32 tiltAngle;
    f32 tiltHalfAngle;
    f32 tiltStep;
    f32 radius;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
} PcpScatterRibbonParticle; /* 0x28 */

struct PcpScatterRibbonWork {
    PcpScatterRibbonParams params;
    PcpScatterRibbonParticle *particles;
    f32 scale;
    u32 color;
    PcpScatterPool *childWork;
    SdfMemoryBlock *ownedResource;
    u32 duplicatedCount;
    u32 *duplicatedHandles;
    SdfMemoryBlock *duplicateAllocation;
};



extern PcpScatterInstance *effPcpScatterCreateParticleInstance();
extern void effShareScatterResource(u32 param0, u32 param1);

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
    SdfMemoryBlock *allocation;
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




extern PcpScatterRes *effPcpScatterResCreate(u32 resId);

void effScatterCreateFromParameterTriplet(void *data)
{
    func_001708A0(effParamTableGetBlock(data, 0), effParamTableGetBlock(data, 1), effParamTableGetBlock(data, 2));
}

PcpScatterRadialWork *effPcpScatterSharedDuplicate(src)
    PcpScatterRadialWork *src;
{
    PcpScatterRadialWork *work;
    u32 count;
    SdfMemoryBlock *handle;
    u32 *buf;
    u32 i;

    work = func_001708A0(&src->params, 0, 0);
    effPcpScatterSharePoolResource(work->childWork, src->childWork);
    if (work->params.duplicateParticles != 0) {
        work->duplicatedCount = work->params.particleCount / work->params.particlesPerGroup;
        if (work->params.particleCount % work->params.particlesPerGroup != 0) {
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

void effPcpScatterReleaseParticleGroup(PcpScatterRadialWork *work)
{
    u32 i;
    u32 count;

    if (work->duplicateAllocation != 0) {
        count = work->duplicatedCount;
        i = 0;
        if (count != 0) {
            do {
                effDispatchParameterDataAndFreeWork(work->duplicatedHandles[i]);
                i++;
            } while (i < count);
        }
        sdfReleaseResourceAllocation(work->duplicateAllocation);
    }
    effPcpScatterReleasePoolResources(work->childWork);
    sdfReleaseResourceAllocation(work->ownedResource);
}

void func_00170D68(PcpScatterRadialWork *work, u32 index) {
    u32 segments = work->params.radialSegments;
    PcpScatterRadialParticle *particle = &work->particles[index];
    f32 angleStep;
    f32 jitter;

    if (segments == 0) {
        segments = 1;
    }
    angleStep = 6.2831852f / segments;
    particle->angle = angleStep * (index % segments) + angleStep * 0.5f * effMiscRandUnitFloat(D_0034DF38);
    jitter = work->params.radiusJitter;
    particle->radius = work->params.unk4C * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter));
    particle->unk18 = 0.0f;
    jitter = work->params.targetRadiusJitter;
    particle->unk04 = (work->params.unk50 * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter)) - particle->radius) / work->params.duration;
    jitter = work->params.speedJitter;
    particle->unk0C = work->params.unk3C * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter));
    particle->age = 0;
    particle->unk08 = work->params.unk44;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00170F28);

void func_00171510(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effSetScatterDuplicatedHandles(PcpScatterRadialWork *work, u32 value)
{
    work->color = value;
}

void effScatterScaleParticleValues(f32 scale, PcpScatterRadialWork *work)
{
    work->params.unk3C *= scale;
    work->params.unk4C *= scale;
    work->params.unk50 *= scale;
}

PcpScatterSpinWork *func_00171550(params, resource, particleParams)
    const PcpScatterSpinParams *params;
    u32 resource;
    void *particleParams;
{
    PcpScatterSpinWork *work;
    PcpScatterSpinParticle *particle;
    SdfMemoryBlock *handle;
    u32 *handles;
    u32 count;
    u32 i;
    s32 delaySpread;

    handle = sdfAllocGeneralBlock(sizeof(PcpScatterSpinWork) + params->particleCount * sizeof(PcpScatterSpinParticle));
    work = sdfResourceRetainAddress(handle);
    work->particles = (PcpScatterSpinParticle *)(work + 1);
    work->params = *params;
    work->color = 0x80808080;
    work->ownedResource = handle;
    work->scale = 1.0f;
    work->duplicatedHandles = NULL;
    work->duplicateAllocation = NULL;
    work->childWork = effPcpScatterPoolCreate(params->particleCount);
    work->childWork->unk10 = params->poolMode;
    if (resource != 0) {
        effPcpScatterCreatePoolResource(work->childWork, resource);
    }
    if (particleParams != NULL && work->params.duplicateParticles != 0) {
        if (work->params.particlesPerGroup == 0) {
            work->params.particlesPerGroup = 1;
        }
        work->duplicatedCount = work->params.particleCount / work->params.particlesPerGroup;
        if (work->params.particleCount % work->params.particlesPerGroup != 0) {
            work->duplicatedCount++;
        }
        count = work->duplicatedCount;
        handle = sdfAllocGeneralBlock(count * sizeof(u32));
        handles = sdfResourceRetainAddress(handle);
        work->duplicateAllocation = handle;
        work->duplicatedHandles = handles;
        work->duplicatedHandles[0] = effParamWorkCreate(6, particleParams);
        for (i = 1; i < count; i++) {
            work->duplicatedHandles[i] = effParamWorkDuplicate(work->duplicatedHandles[0]);
        }
    }
    delaySpread = work->params.delaySpread;
    count = work->params.particleCount;
    particle = work->particles;
    if (delaySpread <= 0) {
        delaySpread = 1;
    }
    for (i = 0; i < count; i++, particle++) {
        particle->age = -(effMiscRand(D_0034DF38) % delaySpread);
    }
    return work;
}

void func_001717E0(void *data)
{
    func_00171550(effParamTableGetBlock(data, 0), effParamTableGetBlock(data, 1), effParamTableGetBlock(data, 2));
}

PcpScatterSpinWork *effPcpScatterLinkedDuplicate(src)
    PcpScatterSpinWork *src;
{
    PcpScatterSpinWork *work;
    u32 count;
    SdfMemoryBlock *handle;
    u32 *buf;
    u32 i;

    work = func_00171550(&src->params, 0, 0);
    effPcpScatterSharePoolResource(work->childWork, src->childWork);
    if (work->params.duplicateParticles != 0) {
        work->duplicatedCount = work->params.particleCount / work->params.particlesPerGroup;
        if (work->params.particleCount % work->params.particlesPerGroup != 0) {
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

void effPcpScatterReleaseSharedParticles(PcpScatterSpinWork *work)
{
    u32 i;
    u32 count;

    if (work->duplicateAllocation != 0) {
        count = work->duplicatedCount;
        i = 0;
        if (count != 0) {
            do {
                effDispatchParameterDataAndFreeWork(work->duplicatedHandles[i]);
                i++;
            } while (i < count);
        }
        sdfReleaseResourceAllocation(work->duplicateAllocation);
    }
    effPcpScatterReleasePoolResources(work->childWork);
    sdfReleaseResourceAllocation(work->ownedResource);
}


/* Init sprite `index`: random spin, jittered start and end distances, and a random unit direction. */
void effScatterSpriteSpawn(PcpScatterSpinWork *work, s32 index)
{
    PcpScatterSpinParticle *sprite = &work->particles[index];
    f32 dir[4];
    f32 jitter;

    sprite->angle = effMiscRandUnitFloat(D_0034DF38) * (3.14159265f * 2.0f);
    jitter = work->params.startRadiusJitter;
    sprite->radius = work->params.startRadius * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter));
    jitter = work->params.endRadiusJitter;
    sprite->radiusStep = (work->params.endRadius * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter)) - sprite->radius) / (f32)work->params.duration;
    sprite->angleStep = work->params.angleStep;
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

void func_00172130(PcpScatterSpinWork *work, u32 value)
{
    work->color = value;
}

void effScatterScalePair(f32 scale, PcpScatterSpinWork *work)
{
    work->params.startRadius *= scale;
    work->params.endRadius *= scale;
}

PcpScatterRibbonWork *func_00172158(params, resource, particleParams)
    const PcpScatterRibbonParams *params;
    u32 resource;
    void *particleParams;
{
    PcpScatterRibbonWork *work;
    PcpScatterRibbonParticle *particle;
    SdfMemoryBlock *handle;
    u32 *handles;
    u32 count;
    u32 i;
    s32 delaySpread;

    handle = sdfAllocGeneralBlock(sizeof(PcpScatterRibbonWork) + params->particleCount * sizeof(PcpScatterRibbonParticle));
    work = sdfResourceRetainAddress(handle);
    work->particles = (PcpScatterRibbonParticle *)(work + 1);
    work->params = *params;
    work->color = 0x80808080;
    work->ownedResource = handle;
    work->scale = 1.0f;
    work->duplicatedHandles = NULL;
    work->duplicateAllocation = NULL;
    work->childWork = effPcpScatterPoolCreate(params->particleCount);
    work->childWork->unk10 = params->poolMode;
    if (resource != 0) {
        effPcpScatterCreatePoolResource(work->childWork, resource);
    }
    if (particleParams != NULL && work->params.duplicateParticles != 0) {
        if (work->params.particlesPerGroup == 0) {
            work->params.particlesPerGroup = 1;
        }
        work->duplicatedCount = work->params.particleCount / work->params.particlesPerGroup;
        if (work->params.particleCount % work->params.particlesPerGroup != 0) {
            work->duplicatedCount++;
        }
        count = work->duplicatedCount;
        handle = sdfAllocGeneralBlock(count * sizeof(u32));
        handles = sdfResourceRetainAddress(handle);
        work->duplicateAllocation = handle;
        work->duplicatedHandles = handles;
        work->duplicatedHandles[0] = effParamWorkCreate(6, particleParams);
        for (i = 1; i < count; i++) {
            work->duplicatedHandles[i] = effParamWorkDuplicate(work->duplicatedHandles[0]);
        }
    }
    delaySpread = work->params.delaySpread;
    count = work->params.particleCount;
    particle = work->particles;
    if (delaySpread <= 0) {
        delaySpread = 1;
    }
    for (i = 0; i < count; i++, particle++) {
        particle->age = -(effMiscRand(D_0034DF38) % delaySpread);
    }
    return work;
}

void func_00172400(void *data)
{
    func_00172158(effParamTableGetBlock(data, 0), effParamTableGetBlock(data, 1), effParamTableGetBlock(data, 2));
}


PcpScatterRibbonWork *effPcpScatterTableDuplicate(src)
    PcpScatterRibbonWork *src;
{
    PcpScatterRibbonWork *work;
    u32 count;
    SdfMemoryBlock *handle;
    u32 *buf;
    u32 i;

    work = func_00172158(&src->params, 0, 0);
    effPcpScatterSharePoolResource(work->childWork, src->childWork);
    if (work->params.duplicateParticles != 0) {
        if (work->params.particlesPerGroup == 0) {
            work->params.particlesPerGroup = 1;
        }
        work->duplicatedCount = work->params.particleCount / work->params.particlesPerGroup;
        if (work->params.particleCount % work->params.particlesPerGroup != 0) {
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

void effPcpScatterReleaseLinkedParticles(PcpScatterRibbonWork *work)
{
    u32 i;
    u32 count;

    if (work->duplicateAllocation != 0) {
        count = work->duplicatedCount;
        i = 0;
        if (count != 0) {
            do {
                effDispatchParameterDataAndFreeWork(work->duplicatedHandles[i]);
                i++;
            } while (i < count);
        }
        sdfReleaseResourceAllocation(work->duplicateAllocation);
    }
    effPcpScatterReleasePoolResources(work->childWork);
    sdfReleaseResourceAllocation(work->ownedResource);
}


void func_001725F0(PcpScatterRibbonWork *work, s32 index)
{
    f32 halfTurn = 90.0f * 0.017453292f;
    f32 quarterTurn = 45.0f * 0.017453292f;
    f32 smallAngle = 5.0f * 0.017453292f;
    PcpScatterRibbonParticle *particle = &work->particles[index];
    f32 direction[4];

    particle->height = 0.0f;
    particle->heightStep = work->params.heightStep;
    particle->tiltHalfAngle = halfTurn;
    particle->tiltAngle = quarterTurn;
    particle->tiltStep = smallAngle;
    particle->unk1C = effMiscRandUnitFloat(D_0034DF38) * 6.2831850051879883f;
    particle->radius = 200.0f;

    direction[0] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    direction[1] = 0.0f;
    direction[2] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    VU0_NORMALIZE_PACKED_VECTOR(direction);

    particle->unk20 = direction[0];
    particle->age = 0;
    particle->unk24 = direction[2];
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001726E8);

void func_00172C48(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00172C58(PcpScatterRibbonWork *work, u32 value)
{
    work->color = value;
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
    SdfMemoryBlock *handle;
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
    pool->resource = (u32)sdfCreateAssetWithDrawEntries();
    func_002DA420((void *)pool->resource, 1.0f);
    memset(D_003D6580, 0, 0x2C);
    *(u16 *)(D_003D6580 + 4) = 0x4000;
    return pool;
}

void effPcpScatterReleasePoolResources(PcpScatterPool *work)
{
    if (work->sharedResource != NULL) {
        effPcpScatterResRelease(work->sharedResource);
    }
    sdfQueueAssetRelease(work->resource);
    sdfReleaseResourceAllocation(work->buffer);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00172DB0);

void effPcpScatterCreatePoolResource(PcpScatterPool *work, u32 resId)
{
    PcpScatterRes *res;

    res = effPcpScatterResCreate(resId);
    work->sharedResource = res;
}

void effPcpScatterSharePoolResource(PcpScatterPool *work, PcpScatterPool *src)
{
    PcpScatterRes *res;

    res = effPcpScatterResAddRef(src->sharedResource);
    work->sharedResource = res;
}

s32 effPcpScatterGetRecordAddress(PcpScatterPool *work, s32 index)
{
    return work->recordBase + index * 0x60;
}

s32 effPcpScatterGetAuxRecordAddress(PcpScatterPool *work, s32 index)
{
    return work->auxRecordBase + index * 0x18;
}

PcpScatterRes *effPcpScatterResCreate(u32 resId)
{
    PcpScatterRes *res;

    res = sdfAllocSizeClassBlock(8);
    res->resourceHandle = sdfTexAcquireResourceTexture(resId);
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

extern void *func_00175B50();
extern void effCreateScatterResource(void *object, u32 resource);

/* Allocate particles after the scatter work, then assign randomized offsets. */
PcpScatterInstance *effPcpScatterCreateParticleInstance(src, resource)
    PcpScatterParams *src;
    u32 resource;
{
    SdfMemoryBlock *allocation = sdfAllocGeneralBlock(src->particleCount * 0x28 + 0x18C);
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
    inst->ownedBuffer = (u32)allocation;
    inst->particles = particle;
    VU0_COPY_MATRIX(inst->matrix, src->matrix);
    object = func_00175B50(src->particleCount, src->unk60);
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
        particle->age = -(effMiscRand(D_0034DF38) % mod);
        particle++;
    }
    return inst;
}

void effScatterBlockDuplicate(void *data)
{
    effPcpScatterCreateParticleInstance(effParamTableGetBlock(data, 0), effParamTableGetBlock(data, 1));
}

PcpScatterInstance *effScatterCloneWithSharedObject(PcpScatterInstance *work) {
    PcpScatterInstance *child;

    child = effPcpScatterCreateParticleInstance(&work->params, NULL);
    effShareScatterResource(child->scatterObject, work->scatterObject);
    return child;
}

void effScatterReleaseObjectAndBuffer(PcpScatterInstance *work)
{
    effReleaseScatterObject(work->scatterObject);
    sdfReleaseResourceAllocation((SdfMemoryBlock *)work->ownedBuffer);
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
    angle = effMiscRandUnitFloat(D_0034DF38) * (3.14159265f * 2.0f);
    jitter = work->params.angleStepJitter;
    angleStep = work->params.angleStepBase * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter)) / (f32)count;
    riseStep = work->params.riseStep;
    jitter = work->params.radiusJitter;
    radius = work->params.radiusBase * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter));
    jitter = work->params.heightOffsetJitter;
    height = work->params.heightOffsetBase * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter));
    jitter = work->params.targetRadiusJitter;
    rise = 0.0f;
    ring->radiusStep = (work->params.targetRadius * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter)) - radius) / (f32)work->params.duration;
    ring->unk00 = work->params.tiltScale * effMiscRandUnitFloat(D_0034DF38);
    ring->tiltAngle = rise;
    ring->angle = angle;
    ring->radius = radius;
    ring->heightOffset = height;
    ring->angleStep = angleStep;
    ring->tiltSpeed = work->params.initialTiltSpeed;
    ring->rise = work->params.initialRise;
    func_002DD608(ring->unk00);
    func_002DD968(ring->tiltAngle);
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
    func_002DD608(ring->unk00);
    func_002DD968(ring->tiltAngle);
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
extern void func_00175DD0(void *draw);
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
    func_00175DD0(draw);
}

void effScatterCopyParticleParameterVector(PcpScatterInstance *work, void *src) {
    PCP_COPY_VECTOR(&work->params, src);
}

void effScatterSetParticleScale(PcpScatterInstance *work, f32 value)
{
    work->scale = value;
}

void effScatterSetParticleColor(PcpScatterInstance *work, u32 value)
{
    work->color = value;
}

/* vu0 routine: matrix = (matrix + 0x50) * src, via the vf28-vf31 by vf24-vf27 product routine */
void effPcpScatterTransformMatrix(PcpScatterInstance *work, void *source)
{
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
    SdfMemoryBlock *allocation = sdfAllocGeneralBlock(src->particleCount * 0x28 + 0x194);
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
    inst->ownedBuffer = (u32)allocation;
    inst->particles = particle;
    inst->age = 0;
    VU0_COPY_MATRIX(inst->matrix, src->matrix);
    object = func_00175B50(src->particleCount, src->unk60);
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
        particle->age = -(effMiscRand(D_0034DF38) % mod);
        particle++;
    }
    return inst;
}

void effScatterSpawnFromParameterPair(void *data)
{
    effScatterInstanceCreateB(effParamTableGetBlock(data, 0), effParamTableGetBlock(data, 1));
}

PcpScatterInstanceB *effScatterCloneWithSharedResource(PcpScatterInstanceB *work)
{
    PcpScatterInstanceB *child;

    child = effScatterInstanceCreateB(&work->params, NULL);
    effShareScatterResource(child->scatterObject, work->scatterObject);
    return child;
}

void effScatterReleaseInstanceResources(PcpScatterInstanceB *work)
{
    effReleaseScatterObject(work->scatterObject);
    sdfReleaseResourceAllocation((SdfMemoryBlock *)work->ownedBuffer);
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
    angle = effMiscRandUnitFloat(D_0034DF38) * (3.14159265f * 2.0f);
    jitter = work->params.angleStepJitter;
    angleStep = work->params.angleStepBase * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter)) / (f32)count;
    riseStep = work->params.riseStep;
    jitter = work->params.radiusJitter;
    radius = work->params.radiusBase * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter));
    jitter = work->params.heightOffsetJitter;
    height = work->params.heightOffsetBase * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter));
    jitter = work->params.radiusStepJitter;
    rise = 0.0f;
    ring->radiusStep = work->params.radiusStepBase * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter));
    ring->unk00 = work->params.tiltScale * effMiscRandUnitFloat(D_0034DF38);
    ring->tiltAngle = rise;
    ring->angle = angle;
    ring->radius = radius;
    ring->heightOffset = height;
    ring->angleStep = angleStep;
    ring->tiltSpeed = work->params.initialTiltSpeed;
    ring->rise = work->params.initialRise;
    func_002DD608(ring->unk00);
    func_002DD968(ring->tiltAngle);
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
    func_002DD608(ring->unk00);
    func_002DD968(ring->tiltAngle);
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
                particle->age = -(effMiscRand(D_0034DF38) % delay);
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
    func_00175DD0(draw);
}

void effScatterSetScaledRingOrigin(PcpScatterInstanceB *work, void *src) {
    PCP_COPY_VECTOR(&work->params, src);
}

void effScatterSetInstanceScale(PcpScatterInstanceB *work, f32 value)
{
    work->scale = value;
}

void effScatterSetInstanceColor(PcpScatterInstanceB *work, u32 value)
{
    work->color = value;
}

/* vu0 routine: matrix = (matrix + 0x50) * src, via the vf28-vf31 by vf24-vf27 product routine */
void effScatterComposeWorkMatrix(PcpScatterInstanceB *work, void *source)
{
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
    SdfMemoryBlock *allocation = sdfAllocGeneralBlock(src->particleCount * 0x28 + 0x19C);
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
    inst->ownedBuffer = (u32)allocation;
    inst->particles = particle;
    inst->age = 0;
    VU0_COPY_MATRIX(inst->matrix, src->matrix);
    object = func_00175B50(src->particleCount, src->unk60);
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
        particle->age = -(effMiscRand(D_0034DF38) % mod);
        particle++;
    }
    return inst;
}

void effScatterCreateFromParameterTable(void *data)
{
    effScatterInstanceCreateC(effParamTableGetBlock(data, 0), effParamTableGetBlock(data, 1));
}

PcpScatterInstanceC *effCreateScatterChildSharingParentResource(PcpScatterInstanceC *work)
{
    PcpScatterInstanceC *child;

    child = effScatterInstanceCreateC(&work->params, NULL);
    effShareScatterResource(child->scatterObject, work->scatterObject);
    return child;
}

void effReleaseScatterObjectAndOwnedBuffer(PcpScatterInstanceC *work)
{
    effReleaseScatterObject(work->scatterObject);
    sdfReleaseResourceAllocation((SdfMemoryBlock *)work->ownedBuffer);
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
    angle = effMiscRandUnitFloat(D_0034DF38) * (3.14159265f * 2.0f);
    jitter = work->params.angleStepJitter;
    angleStep = work->params.angleStepBase * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter)) / (f32)count;
    jitter = work->params.radiusJitter;
    radius = work->params.radiusBase * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter));
    jitter = work->params.heightOffsetJitter;
    height = work->params.heightOffsetBase * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter));
    jitter = work->params.radiusStepJitter;
    ring->radiusStep = work->params.radiusStepBase * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter));
    ring->unk00 = work->params.tiltScale * effMiscRandUnitFloat(D_0034DF38);
    ring->tiltAngle = 0;
    rise = work->params.riseRange / (f32)work->params.particleCount * (f32)index;
    radius = radius + work->params.radiusRamp * (f32)(work->params.particleCount - index);
    ring->angle = angle;
    ring->heightOffset = height;
    ring->angleStep = angleStep;
    ring->radius = radius;
    ring->tiltSpeed = work->params.initialTiltSpeed;
    ring->rise = work->params.initialRise;
    func_002DD608(ring->unk00);
    func_002DD968(ring->tiltAngle);
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
    func_002DD608(ring->unk00);
    func_002DD968(ring->tiltAngle);
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
                particle->age = -(effMiscRand(D_0034DF38) % delay);
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
    func_00175DD0(draw);
}

void effScatterSetDualColorOrigin(PcpScatterInstanceC *work, void *src) {
    PCP_COPY_VECTOR(&work->params, src);
}

void effSetScatterWorkScale(PcpScatterInstanceC *work, f32 value)
{
    work->scale = value;
}

void effSetScatterWorkColor(PcpScatterInstanceC *work, u32 value)
{
    work->color = value;
}

/* vu0 routine: matrix = (matrix + 0x50) * src, via the vf28-vf31 by vf24-vf27 product routine */
void effScatterComposeParticleMatrix(PcpScatterInstanceC *work, void *source)
{
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
    SdfMemoryBlock *allocation = sdfAllocGeneralBlock(src->particleCount * 0x28 + 0x13C);
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
    inst->ownedBuffer = (u32)allocation;
    inst->particles = particle;
    EE_MMI_UNIT_MATRIX(inst->matrix);
    object = func_00175B50(src->particleCount, src->unk20);
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
        particle->age = -(effMiscRand(D_0034DF38) % mod);
        particle++;
    }
    return inst;
}

void func_00175420(void *data)
{
    effPcpScatterCreatePlainInstance(effParamTableGetBlock(data, 0), effParamTableGetBlock(data, 1));
}

PcpScatterPlainInstance *effCloneScatterWithSharedResource(PcpScatterPlainInstance *work)
{
    PcpScatterPlainInstance *child;

    child = effPcpScatterCreatePlainInstance(&work->params, NULL);
    effShareScatterResource(child->scatterObject, work->scatterObject);
    return child;
}

void effReleaseScatterWorkResources(PcpScatterPlainInstance *work)
{
    effReleaseScatterObject(work->scatterObject);
    sdfReleaseResourceAllocation((SdfMemoryBlock *)work->ownedBuffer);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001754E0);


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

extern void func_001754E0(PcpScatterPlainInstance *work, s32 index);

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
                func_001754E0(work, i);
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
    func_00175DD0(draw);
}
