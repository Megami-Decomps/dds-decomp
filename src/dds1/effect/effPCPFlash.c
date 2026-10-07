#include "common.h"
#include "eff.h"
#include "pcp_vu0.h"

/* Work records keep SDF allocation ownership separate from drawable records.
   tintColor multiplies the age-faded packed color; scaleRampTime governs
   scale growth independently of the color fade. */

extern void *effParamTableGetBlock(void *data, s32 index);

extern void effReleaseRecordPoolResourceAndBuffer(EffRecordPool *pool);
extern void effReleaseRecordGroupResources(EffRecordPool *pool);
extern void effReleaseRecordGroupAssetAndHandle(EffRecordPool *pool);
extern void sdfReleaseResourceAllocation(SdfMemBlock *allocation);
extern void *effGetGroupIndexRecord(EffRecordPool *pool, s32 index);
extern u32 effMultiplyPackedColors(u32 color, u32 param);
extern u8 D_0034DF38[];
extern void *effGetIndexedEffectGroupRecord(EffRecordPool *pool, s32 index);
extern f32 D_00354900[];
extern f32 D_00354910[];
extern f32 D_00354920[];
extern f32 D_00354960[];
extern f32 D_00354930[];
extern f32 D_00354940[];
extern f32 D_00354950[];
extern f32 D_003548F0[];
extern f32 D_00354970[];
extern void *effGetGroupRecordByIndex(EffRecordPool *pool, s32 index);
extern void *effGetRecordGroupElement(EffRecordPool *pool, s32 index);
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32 angle);
extern f32 sdfSinPoly(f32 angle);

extern void sdfBuildVuRotationFromAxisAngle(f32 angle, void *orientation);


/* Initializers are also entered without spawn arguments by effect callbacks. */
extern PcpFlashTrianglePulseWork *effFlashRecordCreate();
extern void *effFlashOrbitArcCreate();


typedef struct PcpFlashColorSlot {
    s32 first;
    s32 second;
    s32 third;
} PcpFlashColorSlot;

typedef struct PcpFlashStreakWork PcpFlashStreakWork;

typedef struct PcpFlashRotatingParticle PcpFlashRotatingParticle;

/* Spawn, rotation and draw passes share this 0x58-byte streak work.
   The position-rotation routine uses the same parts pointer at 0x40. */
struct PcpFlashStreakWork {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    s32 scaleRampTime;
    u32 randomRange;
    u32 colorA;
    u32 colorB;
    f32 upSpan;
    f32 acrossSpan;
    f32 maxScale;
    f32 rotationStepRange;
    u32 unk3C;
    PcpFlashRotatingParticle *parts;
    s32 updateCount;
    u32 tintColor;
    f32 renderScale;
    SdfMemBlock *allocationHandle;
    EffRecordPool *resourceHandle;
};

typedef struct PcpFlashOrbitParticle PcpFlashOrbitParticle;

struct PcpFlashOrbitParticle {
    u32 color;
    s32 age;
    f32 scale;
    f32 initialScale;
    f32 angle;
};

typedef struct PcpFlashScalingOrbitWork PcpFlashScalingOrbitWork;

struct PcpFlashScalingOrbitWork {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    s32 scaleRampTime;
    u32 randomRange;
    u32 colorA;
    u32 colorB;
    f32 upSpan;
    f32 acrossSpan;
    f32 orbitRadius;
    f32 maxScale;
    f32 tilt;
    f32 angularStep;
    u32 unk44;
    PcpFlashOrbitParticle *parts;
    u32 updateCount;
    u32 tintColor;
    f32 renderScale;
    SdfMemBlock *allocationHandle;
    EffRecordPool *resourceHandle;
};

typedef struct PcpFlashAccumulatingParticle PcpFlashAccumulatingParticle;

struct PcpFlashAccumulatingParticle {
    u32 color;
    s32 age;
    f32 unk08;
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 accumulator;
};

typedef struct PcpFlashAccumulatingWork PcpFlashAccumulatingWork;

struct PcpFlashAccumulatingWork {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    u32 randomRange;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 unk30;
    f32 unk34;
    f32 orbitRadius;
    f32 unk3C;
    f32 tilt;
    f32 increment;
    f32 radialStep;
    u32 unk4C;
    PcpFlashAccumulatingParticle *parts;
    u32 unk54;
    u32 tintColor;
    f32 renderScale;
    SdfMemBlock *allocationHandle;
    EffRecordPool *resourceHandle;
};

/* Shared 0x10-byte motion state: accumulator is an angle for orbit arcs and
   a radius for radial triangles; stepSpeed is radial speed or arc height. */
typedef struct PcpFlashMotionParticle PcpFlashMotionParticle;

struct PcpFlashMotionParticle {
    u32 color;
    s32 age;
    f32 accumulator;
    f32 stepSpeed; /* 0x0C: multiplied by decay each step, then added to
                       * accumulator; also read as the particle's height */
};

typedef struct PcpFlashOrbitArcWork PcpFlashOrbitArcWork;

/* The orbit constructor and renderer share this 0x80-byte work record.
   The 0x58-byte copied parameters are followed by the motion-particle array
   pointer and draw/resource state. */
struct PcpFlashOrbitArcWork {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    u32 randomRange;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 angularStep;
    u32 unk54;
    PcpFlashMotionParticle *parts;
    u32 unk5C;
    u32 tintColor;
    f32 renderScale;
    f32 orbitRadius;
    f32 normalSpan;
    f32 upSpan;
    f32 acrossSpan;
    SdfMemBlock *allocationHandle;
    EffRecordPool *resourceHandle;
};

typedef struct PcpFlashRotatingQuadParticle PcpFlashRotatingQuadParticle;

struct PcpFlashRotatingQuadParticle {
    u32 color;
    s32 age;
    f32 angularStep;
    f32 scale;
    f32 angle;
    f32 upSpan;
    f32 acrossSpan;
    f32 initialScale;
};

typedef struct PcpFlashRotatingQuadWork PcpFlashRotatingQuadWork;

struct PcpFlashRotatingQuadWork {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    s32 scaleRampTime;
    u32 randomRange;
    u8 pad24[0x04];
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 upSpan;
    f32 acrossSpan;
    f32 maxScale;
    f32 angularStepRange;
    u32 unk48;
    PcpFlashRotatingQuadParticle *parts;
    u32 updateCount;
    u32 tintColor;
    f32 renderScale;
    SdfMemBlock *allocationHandle;
    EffRecordPool *resourceHandle;
};

typedef struct PcpFlashRadialTriangleWork PcpFlashRadialTriangleWork;

struct PcpFlashRadialTriangleWork {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 initialRadius;
    f32 initialRadialSpeed;
    f32 radialDamping;
    u32 unk38;
    PcpFlashMotionParticle *parts;
    s32 updateCount;
    u32 tintColor;
    f32 renderScale;
    SdfMemBlock *allocationHandle;
    EffRecordPool *resourceHandle;
};

typedef struct PcpFlashRadialStripParticle PcpFlashRadialStripParticle;

struct PcpFlashRadialStripParticle {
    u32 color;
    s32 age;
    f32 angularStep;
    f32 thickness;
    f32 radius;
    f32 radialSpeed;
    f32 angle;
    f32 span;
};

typedef struct PcpFlashRadialStripWork PcpFlashRadialStripWork;

struct PcpFlashRadialStripWork {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    u32 randomRange;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 unk30;
    f32 unk34;
    f32 maxScale;
    f32 unk3C;
    f32 initialRadius;
    f32 initialRadialSpeed;
    f32 radialDamping;
    u32 unk4C;
    u8 pad50[0x80];
    PcpFlashRadialStripParticle *parts;
    u32 updateCount;
    u32 tintColor;
    f32 renderScale;
    SdfMemBlock *allocationHandle;
    EffRecordPool *resourceHandle;
};

typedef struct PcpFlashFadingOrbitWork PcpFlashFadingOrbitWork;

struct PcpFlashFadingOrbitWork {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    s32 scaleRampTime;
    u32 randomRange;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 upSpan;
    f32 acrossSpan;
    f32 orbitRadius;
    f32 maxScale;
    f32 tilt;
    f32 angularStep;
    u32 unk4C;
    PcpFlashOrbitParticle *parts;
    u32 updateCount;
    u32 tintColor;
    f32 renderScale;
    SdfMemBlock *allocationHandle;
    EffRecordPool *resourceHandle;
};

typedef struct PcpFlashOffsetRadialWork PcpFlashOffsetRadialWork;

struct PcpFlashOffsetRadialWork {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 initialRadius;
    f32 initialRadialSpeed;
    f32 radialDamping;
    f32 originOffset;
    u32 unk3C;
    PcpFlashMotionParticle *parts;
    s32 updateCount;
    u32 tintColor;
    f32 renderScale;
    SdfMemBlock *allocationHandle;
    EffRecordPool *resourceHandle;
};

struct PcpFlashRotatingParticle {
    u32 color;
    s32 age;
    f32 rotationStep;
    f32 scale;
    f32 position[3];
    f32 unk1C;
    f32 upSpan;
    f32 acrossSpan;
    f32 initialScale;
};


typedef struct PcpFlashQuadColorSlot {
    s32 color[5];
} PcpFlashQuadColorSlot;



extern u8 sdfViewEyeVector[];

extern u8 sdfViewTargetVector[];

extern s32 effBlendColor(s32, s32, f32);

extern void effFlashTrianglePulseWriteCorners();

extern void effDrawTriangleRecordPool(EffRecordPool *);


extern void effFlashBillboardQuad(PcpFlashStreakWork *, s32, void *);

extern void effDrawScaledRecordPool(EffRecordPool *);

extern void effFlashArcQuadScaling(PcpFlashScalingOrbitWork *, s32);


extern void effFlashRotatedTriangle(PcpFlashRadialTriangleWork *, s32, void *);

extern void effFlashFadingOrbitWriteCorners(PcpFlashFadingOrbitWork *, s32);

extern void effFlashOffsetRadialWriteCorners(PcpFlashOffsetRadialWork *, s32, void *);

void effFlashTrianglePulseSpawnFromTable(void *data)
{
    effFlashRecordCreate(effParamTableGetBlock(data, 0));
}

void func_0016A1C8(void)
{
    effFlashRecordCreate();
}

void effFlashTrianglePulseDestroy(PcpFlashTrianglePulseWork *work)
{
    effReleaseRecordPoolResourceAndBuffer(work->resourceHandle);
    sdfReleaseResourceAllocation(work->allocationHandle);
}

void effFlashTrianglePulseCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effFlashTrianglePulseSetColorParam(PcpFlashTrianglePulseWork *work, u32 value)
{
    work->tintColor = value;
}

void effFlashTrianglePulseSetRenderScale(PcpFlashTrianglePulseWork *work, f32 value)
{
    work->renderScale = value;
}

void effFlashTrianglePulseSetParticleColors(PcpFlashTrianglePulseWork *work, s32 index, s32 param)
{
    PcpFlashColorSlot *slot;
    s32 rgb1;
    s32 rgb2;

    slot = (PcpFlashColorSlot *)effGetGroupIndexRecord(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    slot->first = effMultiplyPackedColors(rgb2, param);
    slot->second = effMultiplyPackedColors(rgb2, param);
    slot->third = effMultiplyPackedColors(rgb1 | 0xFF000000, param);
}

/* vu0 routine: a triangle of corner offsets for a flash particle, two of them turned around the view axis by index * step */
void effFlashTrianglePulseWriteCorners(PcpFlashTrianglePulseWork *work, s32 index, void *view)
{
    PcpFlashPulseParticle *part = &work->parts[index];
    f32 *quad = (f32 *)effGetGroupRecordByIndex(work->resourceHandle, index);
    f32 base[4];
    f32 size[4];
    f32 step;
    f32 angle;
    f32 radius;

    step = 3.14159265f * 2.0f / (f32)(u32)work->particleCount;
    radius = part->scale;
    VEC3_SPLAT(size, radius);
    angle = step * (f32)index;
    base[0] = 0;
    base[1] = 1.0f;
    base[2] = 0;
    VU0_LOAD_VF(vf10, base);
    VU0_LOAD_VF(vf11, view);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, base);
    sdfBuildVuRotationFromAxisAngle(angle, view);
    VU0_LOAD_VF(vf10, base);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_LOAD_VF(vf11, size);
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, D_003548F0);
    VU0_STORE_VF(vf10, quad + 8);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad);
    sdfBuildVuRotationFromAxisAngle(angle + step, view);
    VU0_LOAD_VF(vf10, base);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_LOAD_VF(vf11, size);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_003548F0);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 4);
}
void effFlashTrianglePulseUpdate(PcpFlashTrianglePulseWork *work) {
    f32 axis[4];
    s32 index;
    s32 lifetime;
    s32 count;
    s32 half;
    s32 ramp;
    f32 maxScale;
    s32 restart;
    s32 tintColor;
    PcpFlashPulseParticle *part;
    EffRecordPool *handle;

    VU0_LOAD_VF($vf10, sdfViewEyeVector);
    VU0_LOAD_VF($vf11, sdfViewTargetVector);
        VU0_SUB(vf10, vf10, vf11);;
        VU0_NORMALIZE_VF10();;
    VU0_STORE_VF($vf10, axis);
    lifetime = work->lifetime;
    count = work->particleCount;
    part = work->parts;
    half = lifetime >> 1;
    ramp = work->scaleRampTime;
    maxScale = work->maxScale;
    restart = work->restartRandomly;
    tintColor = work->tintColor;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;
        s32 color;
        f32 blend;

        if (part->age == 0) {
            effFlashTrianglePulseWriteCorners(work, index, axis);
            effFlashTrianglePulseSetParticleColors(work, index, 0);
            if (ramp == 0) {
                part->scale = maxScale;
            } else {
                part->scale = 0.0f;
            }
            part->color = 0x80808080;
        } else {
            if (part->age >= lifetime) {
                if (restart != 0) {
                    part->age = 0;
                }
                color = 0;
                effFlashTrianglePulseSetParticleColors(work, index, color);
            } else if (part->age > 0) {
                if (ramp == 0) {
                    part->scale = maxScale;
                } else {
                    part->scale = (maxScale * (f32)part->age) / (f32)ramp;
                    if (maxScale < part->scale) {
                        part->scale = maxScale;
                    }
                }
                effFlashTrianglePulseWriteCorners(work, index, axis);
                if (part->age < half) {
                    blend = (f32)age / (f32)half;
                } else {
                    blend = (f32)(lifetime - age) / (f32)half;
                }
                color = effMultiplyPackedColors(effBlendColor(0, part->color, blend), tintColor);
                effFlashTrianglePulseSetParticleColors(work, index, color);
            }
        }
        part->age = part->age + 1;
    }
    handle = work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->updateCount = work->updateCount + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->scale = work->renderScale;
    effDrawTriangleRecordPool(handle);
}

extern SdfMemBlock *sdfAllocGeneralBlock(s32 size);
extern u32 sdfResourceRetainAddress(SdfMemBlock *allocation);
extern void *memcpy(void *dst, const void *src, u32 n);
extern u32 effMiscRand(void *state);
extern EffRecordPool *effRecordPoolCreateFiveVertexGroups(u32 cellCount);

PcpFlashStreakWork *effFlashRotatingStreakCreate(PcpFlashStreakWork *src) {
    SdfMemBlock *handle = sdfAllocGeneralBlock(src->particleCount * sizeof(PcpFlashRotatingParticle) + sizeof(PcpFlashStreakWork));
    PcpFlashStreakWork *work = (PcpFlashStreakWork *)sdfResourceRetainAddress(handle);
    EffRecordPool *record;
    u32 range;
    u32 i;

    memcpy(work, src, 0x40);
    work->parts = (PcpFlashRotatingParticle *)(work + 1);
    work->allocationHandle = handle;
    work->tintColor = 0x80808080;
    work->renderScale = 1.0f;
    work->updateCount = 0;
    if (work->randomRange == 0) {
        work->randomRange = 1;
    }
    record = effRecordPoolCreateFiveVertexGroups(work->particleCount);
    work->resourceHandle = record;
    record->drawMode = work->unk3C;
    range = work->randomRange;
    for (i = 0; i < work->particleCount; i++) {
        work->parts[i].age = -(effMiscRand(D_0034DF38) % range);
    }
    return work;
}

void effFlashRotatingStreakSpawnFromTable(void *data)
{
    effFlashRotatingStreakCreate(effParamTableGetBlock(data, 0));
}

void func_0016A878(PcpFlashStreakWork *src)
{
    effFlashRotatingStreakCreate(src);
}

void effFlashRotatingStreakDestroy(PcpFlashStreakWork *work)
{
    effReleaseRecordGroupAssetAndHandle(work->resourceHandle);
    sdfReleaseResourceAllocation(work->allocationHandle);
}

void effFlashRotatingStreakCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effFlashRotatingStreakSetColorParam(PcpFlashStreakWork *work, u32 value)
{
    work->tintColor = value;
}

void effFlashRotatingStreakSetRenderScale(PcpFlashStreakWork *work, f32 value)
{
    work->renderScale = value;
}

extern void *effGetIndexedEffectGroupIndexEntry(EffRecordPool *pool, s32 index);

void effFlashRotatingStreakSetParticleColors(PcpFlashStreakWork *work, s32 index, s32 param)
{
    PcpFlashQuadColorSlot *slot;
    s32 rgb1;
    s32 rgb2;

    slot = (PcpFlashQuadColorSlot *)effGetIndexedEffectGroupIndexEntry(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    slot->color[0] = effMultiplyPackedColors(rgb2, param);
    slot->color[1] = effMultiplyPackedColors(rgb2, param);
    if (index & 1) {
        slot->color[2] = effMultiplyPackedColors(0x80000000, param);
        slot->color[3] = effMultiplyPackedColors(rgb1 | 0xFF000000, param);
        slot->color[4] = effMultiplyPackedColors(0x80000000, param);
    } else {
        slot->color[2] = effMultiplyPackedColors(0xFF000000, param);
        slot->color[3] = effMultiplyPackedColors(rgb1 | 0x40000000, param);
        slot->color[4] = effMultiplyPackedColors(0xFF000000, param);
    }
}

extern f32 effMiscRandUnitFloat(void *state);
extern u8 D_0034DF38[];

void effFlashSpawnRotatingParticle(PcpFlashStreakWork *work, s32 index, void *orientation) {
    PcpFlashRotatingParticle *part = work->parts + index;
    f32 direction[4];
    f32 factor;
    f32 scale;

    direction[0] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    direction[1] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    direction[2] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    /* Two plain quadword loads, no memory clobber: retail keeps `direction`
     * and `orientation` CSE'd across them. */
    VU0_LOAD_VF(vf10, direction);
    VU0_LOAD_VF(vf11, orientation);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10();;
    VU0_STORE_VF(vf10, direction);
    part->position[0] = direction[0];
    part->position[1] = direction[1];
    part->position[2] = direction[2];
    factor = effMiscRandUnitFloat(D_0034DF38) * 0.3f + 0.7f;
    scale = work->maxScale * factor;
    part->initialScale = scale;
    part->scale = scale;
    factor = (effMiscRandUnitFloat(D_0034DF38) * 0.5f + 0.5f) * 0.5f;
    part->upSpan = work->upSpan * factor;
    part->acrossSpan = work->acrossSpan * factor;
    part->rotationStep = work->rotationStepRange * ((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f);
}

/* vu0 routine: the four corner offsets of a rotating particle's billboard around its scaled position */
void effFlashBillboardQuad(PcpFlashStreakWork *work, s32 index, void *view)
{
    PcpFlashRotatingParticle *part = &work->parts[index];
    f32 *quad = (f32 *)effGetIndexedEffectGroupRecord(work->resourceHandle, index);
    f32 center[4];
    f32 scale[4];
    f32 across[4];
    f32 up[4];
    f32 ratio;
    f32 size;
    f32 acrossLen;
    f32 upLen;

    size = part->scale;
    ratio = size / part->initialScale;
    VEC3_SPLAT(scale, size);
    acrossLen = part->acrossSpan * ratio;
    VEC3_SPLAT(across, acrossLen);
    upLen = part->upSpan * ratio;
    VEC3_SPLAT(up, upLen);
    center[0] = part->position[0];
    center[1] = part->position[1];
    center[2] = part->position[2];
    VU0_LOAD_VF(vf10, center);
    VU0_LOAD_VF(vf11, view);
    VU0_MOVE_VF(vf12, vf10);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, across);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, across);
    VU0_LOAD_VF(vf10, up);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, up);
    VU0_MOVE_VF(vf10, vf12);
    VU0_LOAD_VF(vf11, scale);
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf10, D_00354900);
    VU0_LOAD_VF(vf11, up);
    VU0_STORE_VF(vf10, quad + 12);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 8);
    VU0_SUB(vf10, vf10, vf11);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 16);
    VU0_LOAD_VF(vf10, D_00354900);
    VU0_LOAD_VF(vf11, across);
    VU0_ADD(vf10, vf10, vf12);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad);
    VU0_SUB(vf10, vf10, vf11);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 4);
}
void effRotateFlashParticlePosition(PcpFlashStreakWork *work, s32 index, void *orientation)
{
    PcpFlashRotatingParticle *part = &work->parts[index];
    f32 position[4];

    position[0] = part->position[0];
    position[1] = part->position[1];
    position[2] = part->position[2];
    sdfBuildVuRotationFromAxisAngle(part->rotationStep, orientation);
    VU0_LOAD_VF(vf10, position);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, position);
    part->position[0] = position[0];
    part->position[1] = position[1];
    part->position[2] = position[2];
}

void effFlashUpdateStreak(PcpFlashStreakWork *work) {
    s128 axis;
    s32 index;
    s32 lifetime;
    s32 count;
    s32 half;
    s32 ramp;
    f32 maxScale;
    s32 restart;
    s32 tintColor;
    u32 range;
    PcpFlashRotatingParticle *part;
    EffRecordPool *handle;

    VU0_LOAD_VF($vf10, sdfViewEyeVector);
    VU0_LOAD_VF($vf11, sdfViewTargetVector);
        VU0_SUB(vf10, vf10, vf11);;
    VU0_STORE_VF($vf10, &axis);
    lifetime = work->lifetime;
    count = work->particleCount;
    part = work->parts;
    half = lifetime >> 1;
    ramp = work->scaleRampTime;
    maxScale = work->maxScale;
    restart = work->restartRandomly;
    range = work->randomRange;
    tintColor = work->tintColor;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;
        s32 color;
        f32 blend;

        if (part->age == 0) {
            effFlashSpawnRotatingParticle(work, index, &axis);
            effFlashBillboardQuad(work, index, &axis);
            effFlashRotatingStreakSetParticleColors(work, index, 0);
            if (ramp == 0) {
                part->scale = maxScale;
            } else {
                part->scale = 0.0f;
            }
            part->color = 0x80808080;
        } else {
            if (part->age >= lifetime) {
                if (restart != 0) {
                    part->age = ~(effMiscRand(D_0034DF38) % range);
                }
                color = 0;
                effFlashRotatingStreakSetParticleColors(work, index, color);
            } else if (part->age > 0) {
                if (ramp == 0) {
                    part->scale = maxScale;
                } else {
                    part->scale = (maxScale * (f32)part->age) / (f32)ramp;
                    if (maxScale < part->scale) {
                        part->scale = maxScale;
                    }
                }
                effRotateFlashParticlePosition(work, index, &axis);
                effFlashBillboardQuad(work, index, &axis);
                if (part->age < half) {
                    blend = (f32)age / (f32)half;
                } else {
                    blend = (f32)(lifetime - age) / (f32)half;
                }
                color = effMultiplyPackedColors(effBlendColor(0, part->color, blend), tintColor);
                effFlashRotatingStreakSetParticleColors(work, index, color);
            }
        }
        part->age = part->age + 1;
    }
    handle = work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->updateCount = work->updateCount + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->scale = work->renderScale;
    effDrawScaledRecordPool(handle);
}

PcpFlashScalingOrbitWork *effFlashOrbitScalingCreate(src)
    PcpFlashScalingOrbitWork *src;
{
    SdfMemBlock *handle = sdfAllocGeneralBlock(src->particleCount * sizeof(PcpFlashOrbitParticle) + sizeof(PcpFlashScalingOrbitWork));
    PcpFlashScalingOrbitWork *work = (PcpFlashScalingOrbitWork *)sdfResourceRetainAddress(handle);
    EffRecordPool *record;
    f32 angle;
    f32 step;
    u32 range;
    u32 i;

    memcpy(work, src, 0x48);
    work->parts = (PcpFlashOrbitParticle *)(work + 1);
    work->allocationHandle = handle;
    work->tintColor = 0x80808080;
    work->updateCount = 0;
    work->renderScale = 1.0f;
    if (work->randomRange == 0) {
        work->randomRange = 1;
    }
    angle = -3.14159265f / 2.0f;
    record = effRecordPoolCreateFiveVertexGroups(work->particleCount);
    record->scale = 1.0f;
    record->drawMode = work->unk44;
    work->resourceHandle = record;
    step = 3.14159265f * 2.0f / (f32)(u32)work->particleCount;
    range = work->randomRange;
    for (i = 0; i < (u32)work->particleCount; i++) {
        work->parts[i].age = -(effMiscRand(D_0034DF38) % range);
        work->parts[i].angle = angle;
        angle += step;
    }
    return work;
}

void effFlashOrbitScalingSpawnFromTable(void *data)
{
    effFlashOrbitScalingCreate(effParamTableGetBlock(data, 0));
}

void func_0016B230(void)
{
    effFlashOrbitScalingCreate();
}

void effFlashOrbitScalingDestroy(PcpFlashScalingOrbitWork *work)
{
    effReleaseRecordGroupAssetAndHandle(work->resourceHandle);
    sdfReleaseResourceAllocation(work->allocationHandle);
}

void effFlashOrbitScalingCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effFlashOrbitScalingSetColorParam(PcpFlashScalingOrbitWork *work, u32 value)
{
    work->tintColor = value;
}

void effFlashOrbitScalingSetRenderScale(PcpFlashScalingOrbitWork *work, f32 value)
{
    work->renderScale = value;
}

void effFlashOrbitScalingSetParticleColors(PcpFlashScalingOrbitWork *work, s32 index, s32 param)
{
    PcpFlashQuadColorSlot *slot;
    s32 rgb1;
    s32 rgb2;

    slot = (PcpFlashQuadColorSlot *)effGetIndexedEffectGroupIndexEntry(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    slot->color[0] = effMultiplyPackedColors(rgb2, param);
    slot->color[1] = effMultiplyPackedColors(rgb2, param);
    if (index & 1) {
        slot->color[2] = effMultiplyPackedColors(0x80000000, param);
        slot->color[3] = effMultiplyPackedColors(rgb1 | 0xFF000000, param);
        slot->color[4] = effMultiplyPackedColors(0x80000000, param);
    } else {
        slot->color[2] = effMultiplyPackedColors(0xFF000000, param);
        slot->color[3] = effMultiplyPackedColors(rgb1 | 0x40000000, param);
        slot->color[4] = effMultiplyPackedColors(0xFF000000, param);
    }
}

/* vu0 routine: billboard corner offsets for a scaling particle on an arc, built from a normalised direction and its perpendicular */
void effFlashArcQuadScaling(PcpFlashScalingOrbitWork *work, s32 index)
{
    PcpFlashOrbitParticle *part = &work->parts[index];
    f32 *quad = (f32 *)effGetIndexedEffectGroupRecord(work->resourceHandle, index);
    f32 offset[4];
    f32 unit[4];
    f32 scaleA[4];
    f32 scaleB[4];
    f32 scaleC[4];
    f32 size;
    f32 ratio;
    f32 sinv;
    f32 height;
    f32 widthB;
    f32 widthC;

    size = part->scale;
    ratio = size / part->initialScale;
    VEC3_SPLAT(scaleA, size);
    widthB = work->acrossSpan * ratio;
    widthC = work->upSpan * ratio;
    VEC3_SPLAT(scaleB, widthB);
    VEC3_SPLAT(scaleC, widthC);
    unit[0] = sdfEvaluateCosineViaSinePhaseShift(part->angle);
    unit[1] = 0;
    sinv = sdfSinPoly(part->angle);
    unit[2] = sinv;
    offset[0] = unit[0] * work->orbitRadius;
    offset[1] = 0;
    offset[2] = sinv * work->orbitRadius;
    height = work->tilt;
    D_00354910[0] = unit[0] * height;
    D_00354910[1] = height + -1.0f;
    D_00354910[2] = sinv * height;
    VU0_LOAD_VF(vf10, D_00354910);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf11, vf10);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf10, unit);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, scaleB);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, scaleB);
    VU0_LOAD_VF(vf10, scaleC);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, scaleC);
    VU0_MOVE_VF(vf10, vf12);
    VU0_LOAD_VF(vf11, scaleA);
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf10, offset);
    VU0_LOAD_VF(vf11, scaleC);
    VU0_STORE_VF(vf10, quad + 12);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 8);
    VU0_SUB(vf10, vf10, vf11);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 16);
    VU0_LOAD_VF(vf10, offset);
    VU0_LOAD_VF(vf11, scaleB);
    VU0_ADD(vf10, vf10, vf12);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad);
    VU0_SUB(vf10, vf10, vf11);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 4);
}
void effFlashOrbitScalingAdvanceAngle(PcpFlashScalingOrbitWork *work, s32 index)
{
    PcpFlashOrbitParticle *part;

    part = &work->parts[index];
    part->angle += work->angularStep;
}

void effFlashOrbitScalingUpdate(PcpFlashScalingOrbitWork *work) {
    s32 index;
    s32 lifetime;
    s32 count;
    s32 half;
    s32 ramp;
    f32 maxScale;
    s32 restart;
    s32 tintColor;
    u32 range;
    PcpFlashOrbitParticle *part;
    EffRecordPool *handle;

    lifetime = work->lifetime;
    count = work->particleCount;
    part = work->parts;
    half = lifetime >> 1;
    ramp = work->scaleRampTime;
    maxScale = work->maxScale;
    restart = work->restartRandomly;
    range = work->randomRange;
    tintColor = work->tintColor;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;
        s32 color;
        f32 blend;

        if (part->age == 0) {
            part->initialScale = maxScale;
            if (ramp == 0) {
                part->scale = maxScale;
            } else {
                part->scale = 0.0f;
            }
            effFlashArcQuadScaling(work, index);
            effFlashOrbitScalingSetParticleColors(work, index, 0);
            part->color = 0x80808080;
        } else {
            if (part->age >= lifetime) {
                if (restart != 0) {
                    part->age = ~(effMiscRand(D_0034DF38) % range);
                }
                color = 0;
                effFlashOrbitScalingSetParticleColors(work, index, color);
            } else if (part->age > 0) {
                if (ramp == 0) {
                    part->scale = maxScale;
                } else {
                    part->scale = (maxScale * (f32)part->age) / (f32)ramp;
                    if (maxScale < part->scale) {
                        part->scale = maxScale;
                    }
                }
                effFlashOrbitScalingAdvanceAngle(work, index);
                effFlashArcQuadScaling(work, index);
                if (part->age < half) {
                    blend = (f32)age / (f32)half;
                } else {
                    blend = (f32)(lifetime - age) / (f32)half;
                }
                color = effMultiplyPackedColors(effBlendColor(0, part->color, blend), tintColor);
                effFlashOrbitScalingSetParticleColors(work, index, color);
            }
        }
        part->age = part->age + 1;
    }
    handle = work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->updateCount = work->updateCount + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->scale = work->renderScale;
    effDrawScaledRecordPool(handle);
}

extern EffRecordPool *effRecordPoolCreate(s32 quadCount);

/* Clone the 0x50-byte parameter block, then spread the particles evenly around the orbit from -pi/2 with random negative start ages (two handle slots per particle). */
PcpFlashAccumulatingWork *effFlashAccumulatingCreate(src)
    PcpFlashAccumulatingWork *src;
{
    SdfMemBlock *handle = sdfAllocGeneralBlock(src->particleCount * sizeof(PcpFlashAccumulatingParticle) + sizeof(PcpFlashAccumulatingWork));
    PcpFlashAccumulatingWork *work = (PcpFlashAccumulatingWork *)sdfResourceRetainAddress(handle);
    EffRecordPool *record;
    f32 angle;
    f32 step;
    u32 range;
    u32 i;

    memcpy(work, src, 0x50);
    work->parts = (PcpFlashAccumulatingParticle *)(work + 1);
    work->allocationHandle = handle;
    work->tintColor = 0x80808080;
    work->unk54 = 0;
    work->renderScale = 1.0f;
    if (work->randomRange == 0) {
        work->randomRange = 1;
    }
    angle = -3.14159265f / 2.0f;
    record = effRecordPoolCreate(work->particleCount * 2);
    record->scale = 1.0f;
    record->drawMode = work->unk4C;
    work->resourceHandle = record;
    step = 3.14159265f * 2.0f / (f32)(u32)work->particleCount;
    range = work->randomRange;
    for (i = 0; i < (u32)work->particleCount; i++) {
        work->parts[i].age = -(effMiscRand(D_0034DF38) % range);
        work->parts[i].accumulator = angle;
        angle += step;
    }
    return work;
}

void effFlashAccumulatingParticleSpawnFromTable(void *data)
{
    effFlashAccumulatingCreate(effParamTableGetBlock(data, 0));
}

void func_0016BA58(void)
{
    effFlashAccumulatingCreate();
}

void effFlashAccumulatingParticleDestroy(PcpFlashAccumulatingWork *work)
{
    effReleaseRecordGroupResources(work->resourceHandle);
    sdfReleaseResourceAllocation(work->allocationHandle);
}

void effFlashAccumulatingParticleCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effFlashAccumulatingParticleSetColorParam(PcpFlashAccumulatingWork *work, u32 value)
{
    work->tintColor = value;
}

void effFlashAccumulatingParticleSetRenderScale(PcpFlashAccumulatingWork *work, f32 value)
{
    work->renderScale = value;
}

typedef struct EffRecordPool EffRecordPool;
extern void *effGetRecordGroupAuxEntry(EffRecordPool *, s32);

void func_0016BAC0(PcpFlashAccumulatingWork *work, s32 index, u32 param) {
    u32 *colors;
    u32 colorA;
    u32 colorB;
    u32 *mirror;

    colors = (u32 *)effGetRecordGroupAuxEntry(work->resourceHandle, index * 2);
    colorA = work->colorA & 0xFFFFFF;
    colorB = work->colorB & 0xFFFFFF;
    colors[0] = effMultiplyPackedColors(colorB | 0x80000000, param);
    colors[1] = effMultiplyPackedColors(colorB, param);
    colors[2] = effMultiplyPackedColors(colorA | 0x80000000, param);
    colors[3] = effMultiplyPackedColors(colorB, param);
    mirror = (u32 *)effGetRecordGroupAuxEntry(work->resourceHandle, index * 2 + 1);
    mirror[0] = colors[0];
    mirror[1] = colors[1];
    mirror[2] = colors[2];
    mirror[3] = colors[3];
}

/* vu0 routine: the orbit offset plus a tilted disc of corner offsets for an
   accumulating particle, whose radius grows by the work record's radial step */
void func_0016BBB0(PcpFlashAccumulatingWork *work, s32 index)
{
    PcpFlashAccumulatingParticle *part = &work->parts[index];
    f32 *quad = effGetRecordGroupElement(work->resourceHandle, index * 2);
    f32 offset[4];
    f32 unit[4];
    f32 middle[4];
    f32 outer[4];
    f32 inner[4];
    f32 size[4];
    f32 center;
    f32 outerEdge;
    f32 innerEdge;
    f32 span;
    f32 sinv;
    f32 height;
    f32 *mirror;

    center = part->unk14;
    VEC3_SPLAT(middle, center);
    outerEdge = center + part->unk08;
    VEC3_SPLAT(outer, outerEdge);
    innerEdge = center - part->unk08;
    VEC3_SPLAT(inner, innerEdge);
    part->unk14 = part->unk14 + work->radialStep;
    span = part->unk0C;
    VEC3_SPLAT(size, span);
    unit[0] = sdfEvaluateCosineViaSinePhaseShift(part->accumulator);
    unit[1] = 0;
    sinv = sdfSinPoly(part->accumulator);
    unit[2] = sinv;
    offset[0] = unit[0] * work->orbitRadius;
    offset[1] = 0;
    offset[2] = sinv * work->orbitRadius;
    height = work->tilt;
    D_00354920[0] = unit[0] * height;
    D_00354920[1] = height + -1.0f;
    D_00354920[2] = sinv * height;
    VU0_LOAD_VF(vf10, D_00354920);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf11, vf10);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf10, unit);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, size);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, size);
    VU0_MOVE_VF(vf10, vf12);
    VU0_LOAD_VF(vf11, outer);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, outer);
    VU0_MOVE_VF(vf10, vf12);
    VU0_LOAD_VF(vf11, inner);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, inner);
    VU0_MOVE_VF(vf10, vf12);
    VU0_LOAD_VF(vf11, middle);
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf10, offset);
    VU0_ADD(vf10, vf10, vf12);
    VU0_LOAD_VF(vf11, size);
    VU0_STORE_VF(vf10, quad + 8);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 4);
    VU0_SUB(vf10, vf10, vf11);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 12);
    VU0_LOAD_VF(vf10, offset);
    VU0_LOAD_VF(vf11, outer);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad);
    mirror = effGetRecordGroupElement(work->resourceHandle, index * 2 + 1);
    PCP_COPY_VECTOR(mirror + 8, quad + 8);
    PCP_COPY_VECTOR(mirror + 4, quad + 4);
    PCP_COPY_VECTOR(mirror + 12, quad + 12);
    VU0_LOAD_VF(vf10, offset);
    VU0_LOAD_VF(vf11, inner);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, mirror);
}

void effFlashAccumulatingParticleAdvance(PcpFlashAccumulatingWork *work, s32 index)
{
    PcpFlashAccumulatingParticle *part;

    part = &work->parts[index];
    part->accumulator += work->increment;
}

extern void func_0016BAC0(PcpFlashAccumulatingWork *, s32, u32);
extern void func_0016BBB0(PcpFlashAccumulatingWork *, s32);

void effFlashAccumulatingParticleUpdate(PcpFlashAccumulatingWork *work)
{
    s32 index;
    s32 count;
    PcpFlashAccumulatingParticle *part;
    s32 lifetime;
    s32 fadeIn;
    s32 fadeOut;
    u32 randomRange;
    s32 restart;
    s32 tintColor;
    EffRecordPool *handle;

    count = work->particleCount;
    part = work->parts;
    lifetime = work->lifetime;
    fadeIn = work->fadeInTime;
    fadeOut = work->fadeOutTime;
    restart = work->restartRandomly;
    randomRange = work->randomRange;
    tintColor = work->tintColor;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;
        s32 color;
        f32 factor;

        if (age == 0) {
            func_0016BBB0(work, index);
            func_0016BAC0(work, index, 0);
            part->color = 0x80808080;
            factor = effMiscRandUnitFloat(D_0034DF38) * 0.5f + 0.5f;
            part->unk08 = work->unk3C * factor;
            factor = effMiscRandUnitFloat(D_0034DF38) * 0.7f + 0.3f;
            part->unk0C = work->unk30 * factor;
            part->unk10 = work->unk34 * factor;
            part->unk14 = 0;
        } else if (age >= lifetime) {
            if (restart != 0) {
                part->age = ~(effMiscRand(D_0034DF38) % randomRange);
            }
            color = 0;
            func_0016BAC0(work, index, color);
        } else if (age > 0) {
            effFlashAccumulatingParticleAdvance(work, index);
            func_0016BBB0(work, index);
            if (part->age < fadeIn && fadeIn != 0) {
                factor = (f32)part->age / (f32)fadeIn;
            } else if (fadeOut >= lifetime - part->age && fadeOut != 0) {
                factor = (f32)(lifetime - part->age) / (f32)fadeOut;
            } else {
                factor = 1.0f;
            }
            color = effMultiplyPackedColors(effBlendColor(0, part->color, factor), tintColor);
            func_0016BAC0(work, index, color);
        }
        part->age = part->age + 1;
    }
    handle = work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->unk54 = work->unk54 + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->scale = work->renderScale;
    effDrawQuadRecordPool(handle);
}

/* The orbit-arc particles follow the same work header used by its draw and
   angle-update passes; this is an allocation container, not a second view. */
typedef struct PcpFlashOrbitArcBlock {
    PcpFlashOrbitArcWork header;
    PcpFlashMotionParticle parts[1];
} PcpFlashOrbitArcBlock;

void *effFlashOrbitArcCreate(source)
PcpFlashOrbitArcWork *source;
{
    SdfMemBlock *handle;
    PcpFlashOrbitArcBlock *block;
    PcpFlashOrbitArcWork *ring;
    EffRecordPool *record;
    f32 angle;
    f32 step;
    u32 spread;
    u32 i;

    handle = sdfAllocGeneralBlock((u32)source->particleCount * 16 + 0x80);
    block = (PcpFlashOrbitArcBlock *)sdfResourceRetainAddress(handle);
    ring = &block->header;
    memcpy(ring, source, 0x58);
    ring->parts = block->parts;
    ring->allocationHandle = handle;
    ring->tintColor = 0x80808080;
    ring->orbitRadius = ring->unk38;
    ring->upSpan = ring->unk30;
    ring->acrossSpan = ring->unk34;
    ring->unk5C = 0;
    ring->renderScale = 1.0f;
    if (ring->randomRange == 0) {
        ring->randomRange = 1;
    }
    angle = EFFECT_RING_START_ANGLE;
    record = effRecordPoolCreateFiveVertexGroups(ring->particleCount);
    record->scale = 1.0f;
    record->drawMode = ring->unk54;
    ring->resourceHandle = record;
    step = EFFECT_RING_FULL_TURN / (u32)ring->particleCount;
    spread = ring->randomRange;
    for (i = 0; i < (u32)ring->particleCount; i++) {
        ring->parts[i].age = -(effMiscRand(D_0034DF38) % spread);
        ring->parts[i].accumulator = angle;
        angle += step;
    }
    return ring;
}

void effFlashOrbitArcSpawnFromTable(void *data)
{
    effFlashOrbitArcCreate(effParamTableGetBlock(data, 0));
}

void func_0016C358(void)
{
    effFlashOrbitArcCreate();
}

void effFlashOrbitArcDestroy(PcpFlashOrbitArcWork *work)
{
    effReleaseRecordGroupAssetAndHandle(work->resourceHandle);
    sdfReleaseResourceAllocation(work->allocationHandle);
}

void effFlashOrbitArcCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effFlashOrbitArcSetColorParam(PcpFlashOrbitArcWork *work, u32 value)
{
    work->tintColor = value;
}

void effFlashOrbitArcSetRenderScale(PcpFlashOrbitArcWork *work, f32 value)
{
    work->renderScale = value;
}

void effFlashOrbitArcSetParticleColors(PcpFlashOrbitArcWork *work, s32 index, s32 param)
{
    PcpFlashQuadColorSlot *slot;
    s32 rgb1;
    s32 rgb2;

    slot = (PcpFlashQuadColorSlot *)effGetIndexedEffectGroupIndexEntry(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    slot->color[0] = effMultiplyPackedColors(rgb2, param);
    slot->color[1] = effMultiplyPackedColors(rgb2, param);
    if (index & 1) {
        slot->color[2] = effMultiplyPackedColors(0x80000000, param);
        slot->color[3] = effMultiplyPackedColors(rgb1 | 0xFF000000, param);
        slot->color[4] = effMultiplyPackedColors(0x80000000, param);
    } else {
        slot->color[2] = effMultiplyPackedColors(0xFF000000, param);
        slot->color[3] = effMultiplyPackedColors(rgb1 | 0x40000000, param);
        slot->color[4] = effMultiplyPackedColors(0xFF000000, param);
    }
}

/* vu0 routine: billboard corner offsets for a particle on an arc, built from a normalised direction and its perpendicular */
void effFlashArcQuad(PcpFlashOrbitArcWork *work, s32 index)
{
    PcpFlashMotionParticle *part = &work->parts[index];
    f32 *quad = (f32 *)effGetIndexedEffectGroupRecord(work->resourceHandle, index);
    f32 offset[4];
    f32 unit[4];
    f32 scaleA[4];
    f32 scaleB[4];
    f32 scaleC[4];
    f32 sinv;
    f32 height;

    VEC3_SPLAT(scaleA, work->normalSpan);
    VEC3_SPLAT(scaleB, work->acrossSpan);
    VEC3_SPLAT(scaleC, work->upSpan);
    unit[0] = sdfEvaluateCosineViaSinePhaseShift(part->accumulator);
    unit[1] = 0;
    sinv = sdfSinPoly(part->accumulator);
    unit[2] = sinv;
    offset[0] = unit[0] * work->orbitRadius;
    offset[1] = 0;
    offset[2] = sinv * work->orbitRadius;
    height = part->stepSpeed;
    D_00354930[0] = unit[0] * height;
    D_00354930[1] = height + -1.0f;
    D_00354930[2] = sinv * height;
    VU0_LOAD_VF(vf10, D_00354930);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf11, vf10);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf10, unit);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, scaleB);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, scaleB);
    VU0_LOAD_VF(vf10, scaleC);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, scaleC);
    VU0_MOVE_VF(vf10, vf12);
    VU0_LOAD_VF(vf11, scaleA);
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf10, offset);
    VU0_LOAD_VF(vf11, scaleC);
    VU0_STORE_VF(vf10, quad + 12);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 8);
    VU0_SUB(vf10, vf10, vf11);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 16);
    VU0_LOAD_VF(vf10, offset);
    VU0_LOAD_VF(vf11, scaleB);
    VU0_ADD(vf10, vf10, vf12);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad);
    VU0_SUB(vf10, vf10, vf11);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 4);
}
void effFlashOrbitArcAdvanceAngle(PcpFlashOrbitArcWork *work, s32 index)
{
    PcpFlashMotionParticle *part;

    part = &work->parts[index];
    part->accumulator += work->angularStep;
}

void func_0016C698(PcpFlashOrbitArcWork *work)
{
    s32 index;
    s32 count;
    PcpFlashMotionParticle *part;
    s32 lifetime;
    s32 fadeIn;
    s32 fadeOut;
    u32 randomRange;
    s32 restart;
    s32 tintColor;
    EffRecordPool *handle;
    f32 heightDelta;
    f32 delta;
    f32 progress;
    f32 factor;
    f32 start;
    f32 radiusEnd;
    f32 upStart;
    f32 acrossStart;
    f32 upEnd;
    f32 acrossEnd;
    f32 normalEnd;

    part = work->parts;
    count = work->particleCount;
    lifetime = work->lifetime;
    fadeIn = work->fadeInTime;
    fadeOut = work->fadeOutTime;
    restart = work->restartRandomly;
    randomRange = work->randomRange;
    tintColor = work->tintColor;
    heightDelta = work->unk4C - work->unk48;
    progress = 1.0f;
    if (lifetime > 0) {
        progress = (f32)work->unk5C / (f32)lifetime;
    }
    start = work->unk38;
    radiusEnd = work->unk3C;
    delta = radiusEnd - start;
    work->orbitRadius = start + delta * progress;
    if (start > 0.0f) {
        factor = radiusEnd / start;
    } else {
        factor = radiusEnd;
    }
    upStart = work->unk30;
    acrossStart = work->unk34;
    upEnd = upStart * factor;
    acrossEnd = acrossStart * factor;
    start = work->unk40;
    normalEnd = work->unk44;
    delta = upEnd - upStart;
    work->upSpan = upStart + delta * progress;
    delta = acrossEnd - acrossStart;
    work->acrossSpan = acrossStart + delta * progress;
    delta = normalEnd - start;
    work->normalSpan = start + delta * progress;

    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;
        s32 color;

        if (age == 0) {
            part->stepSpeed = work->unk48;
            effFlashArcQuad(work, index);
            effFlashOrbitArcSetParticleColors(work, index, 0);
            part->color = 0x80808080;
        } else if (age >= lifetime) {
            if (restart != 0) {
                part->age = ~(effMiscRand(D_0034DF38) % randomRange);
            }
            color = 0;
            effFlashOrbitArcSetParticleColors(work, index, color);
        } else if (age > 0) {
            progress = (f32)age / (f32)lifetime;
            part->stepSpeed = heightDelta * progress + work->unk48;
            effFlashOrbitArcAdvanceAngle(work, index);
            effFlashArcQuad(work, index);
            if (part->age < fadeIn && fadeIn != 0) {
                factor = (f32)part->age / (f32)fadeIn;
            } else if (fadeOut >= lifetime - part->age && fadeOut != 0) {
                factor = (f32)(lifetime - part->age) / (f32)fadeOut;
            } else {
                factor = 1.0f;
            }
            color = effMultiplyPackedColors(effBlendColor(0, part->color, factor), tintColor);
            effFlashOrbitArcSetParticleColors(work, index, color);
        }
        part->age = part->age + 1;
    }
    if (work->unk5C == lifetime) {
        work->unk5C = 0;
    } else {
        work->unk5C++;
    }
    handle = work->resourceHandle;
    handle->origin[0] = work->origin[0];
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->scale = work->renderScale;
    effDrawScaledRecordPool(handle);
}

PcpFlashRotatingQuadWork *effFlashRotatingQuadCreate(src)
    PcpFlashRotatingQuadWork *src;
{
    SdfMemBlock *handle = sdfAllocGeneralBlock(src->particleCount * sizeof(PcpFlashRotatingQuadParticle) + sizeof(PcpFlashRotatingQuadWork));
    PcpFlashRotatingQuadWork *work = (PcpFlashRotatingQuadWork *)sdfResourceRetainAddress(handle);
    EffRecordPool *record;
    u32 range;
    u32 i;

    memcpy(work, src, 0x4C);
    work->parts = (PcpFlashRotatingQuadParticle *)(work + 1);
    work->allocationHandle = handle;
    work->tintColor = 0x80808080;
    work->renderScale = 1.0f;
    work->updateCount = 0;
    if (work->randomRange == 0) {
        work->randomRange = 1;
    }
    record = effRecordPoolCreateFiveVertexGroups(work->particleCount);
    work->resourceHandle = record;
    record->drawMode = work->unk48;
    range = work->randomRange;
    for (i = 0; i < work->particleCount; i++) {
        work->parts[i].age = -(effMiscRand(D_0034DF38) % range);
    }
    return work;
}

void effFlashRotatingQuadSpawnFromTable(void *data)
{
    effFlashRotatingQuadCreate(effParamTableGetBlock(data, 0));
}

void func_0016CBD0(void)
{
    effFlashRotatingQuadCreate();
}

void effFlashRotatingQuadDestroy(PcpFlashRotatingQuadWork *work)
{
    effReleaseRecordGroupAssetAndHandle(work->resourceHandle);
    sdfReleaseResourceAllocation(work->allocationHandle);
}

void effFlashRotatingQuadCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effFlashRotatingQuadSetColorParam(PcpFlashRotatingQuadWork *work, u32 value)
{
    work->tintColor = value;
}

void effFlashRotatingQuadSetRenderScale(PcpFlashRotatingQuadWork *work, f32 value)
{
    work->renderScale = value;
}

void effFlashRotatingQuadSetParticleColors(PcpFlashRotatingQuadWork *work, s32 index, s32 param)
{
    PcpFlashQuadColorSlot *slot;
    s32 rgb1;
    s32 rgb2;

    slot = (PcpFlashQuadColorSlot *)effGetIndexedEffectGroupIndexEntry(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    slot->color[0] = effMultiplyPackedColors(rgb2, param);
    slot->color[1] = effMultiplyPackedColors(rgb2, param);
    if (index & 1) {
        slot->color[2] = effMultiplyPackedColors(0x80000000, param);
        slot->color[3] = effMultiplyPackedColors(rgb1 | 0xFF000000, param);
        slot->color[4] = effMultiplyPackedColors(0x80000000, param);
    } else {
        slot->color[2] = effMultiplyPackedColors(0xFF000000, param);
        slot->color[3] = effMultiplyPackedColors(rgb1 | 0x40000000, param);
        slot->color[4] = effMultiplyPackedColors(0xFF000000, param);
    }
}

extern f32 effMiscRandUnitFloat(void *state);

void effFlashRotatingQuadSpawnParticle(PcpFlashRotatingQuadWork *work, s32 index, void *orientation) {
    PcpFlashRotatingQuadParticle *part = work->parts + index;
    f32 factor;
    f32 scale;

    part->angle = effMiscRandUnitFloat(D_0034DF38) * 6.2831853f;
    factor = effMiscRandUnitFloat(D_0034DF38) * 0.3f + 0.7f;
    scale = work->maxScale * factor;
    part->initialScale = scale;
    part->scale = scale;
    factor = (effMiscRandUnitFloat(D_0034DF38) * 0.5f + 0.5f) * 0.5f;
    part->upSpan = work->upSpan * factor;
    part->acrossSpan = work->acrossSpan * factor;
    part->angularStep = work->angularStepRange * ((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f);
}

/* vu0 routine: corner offsets of a flash particle's billboard, turned around the view axis by the particle's angle */
void effFlashRotatedQuad(PcpFlashRotatingQuadWork *work, s32 index, void *view)
{
    PcpFlashRotatingQuadParticle *part = &work->parts[index];
    f32 *quad = (f32 *)effGetIndexedEffectGroupRecord(work->resourceHandle, index);
    f32 base[4];
    f32 scale[4];
    f32 across[4];
    f32 up[4];
    f32 ratio;
    f32 size;
    f32 acrossLen;
    f32 upLen;

    size = part->scale;
    ratio = size / part->initialScale;
    VEC3_SPLAT(scale, size);
    acrossLen = part->acrossSpan * ratio;
    VEC3_SPLAT(across, acrossLen);
    upLen = part->upSpan * ratio;
    VEC3_SPLAT(up, upLen);
    sdfBuildVuRotationFromAxisAngle(part->angle, view);
    base[0] = 0;
    base[1] = 1.0f;
    base[2] = 0;
    VU0_LOAD_VF(vf10, base);
    VU0_LOAD_VF(vf11, view);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_LOAD_VF(vf11, view);
    VU0_MOVE_VF(vf12, vf10);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, across);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, across);
    VU0_LOAD_VF(vf10, up);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, up);
    VU0_MOVE_VF(vf10, vf12);
    VU0_LOAD_VF(vf11, scale);
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf10, D_00354940);
    VU0_LOAD_VF(vf11, up);
    VU0_STORE_VF(vf10, quad + 12);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 8);
    VU0_SUB(vf10, vf10, vf11);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 16);
    VU0_LOAD_VF(vf10, D_00354940);
    VU0_LOAD_VF(vf11, across);
    VU0_ADD(vf10, vf10, vf12);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad);
    VU0_SUB(vf10, vf10, vf11);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 4);
}
void effFlashAdvanceOrbitPhase(PcpFlashRotatingQuadWork *work, s32 index, void *orientation)
{
    PcpFlashRotatingQuadParticle *part;

    part = &work->parts[index];
    part->angle += part->angularStep;
}

void effFlashRotatingQuadUpdate(PcpFlashRotatingQuadWork *work) {
    s128 axis;
    s32 index;
    s32 lifetime;
    s32 count;
    s32 ramp;
    s32 fadeIn;
    s32 fadeOut;
    f32 maxScale;
    s32 restart;
    s32 tintColor;
    u32 range;
    PcpFlashRotatingQuadParticle *part;
    EffRecordPool *handle;

    VU0_LOAD_VF($vf10, sdfViewEyeVector);
    VU0_LOAD_VF($vf11, sdfViewTargetVector);
        VU0_SUB(vf10, vf10, vf11);;
    VU0_STORE_VF($vf10, &axis);
    count = work->particleCount;
    part = work->parts;
    lifetime = work->lifetime;
    ramp = work->scaleRampTime;
    maxScale = work->maxScale;
    fadeIn = work->fadeInTime;
    fadeOut = work->fadeOutTime;
    restart = work->restartRandomly;
    range = work->randomRange;
    tintColor = work->tintColor;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;
        s32 color;
        f32 blend;

        if (part->age == 0) {
            effFlashRotatingQuadSpawnParticle(work, index, &axis);
            effFlashRotatedQuad(work, index, &axis);
            effFlashRotatingQuadSetParticleColors(work, index, 0);
            if (ramp == 0) {
                part->scale = maxScale;
            } else {
                part->scale = 0.0f;
            }
            part->color = 0x80808080;
        } else {
            if (part->age >= lifetime) {
                if (restart != 0) {
                    part->age = ~(effMiscRand(D_0034DF38) % range);
                }
                color = 0;
                effFlashRotatingQuadSetParticleColors(work, index, color);
            } else if (part->age > 0) {
                s32 remain;

                if (ramp == 0) {
                    part->scale = maxScale;
                } else {
                    part->scale = (maxScale * (f32)part->age) / (f32)ramp;
                    if (maxScale < part->scale) {
                        part->scale = maxScale;
                    }
                }
                effFlashAdvanceOrbitPhase(work, index, &axis);
                effFlashRotatedQuad(work, index, &axis);
                age = part->age;
                if (age < fadeIn && fadeIn != 0) {
                    blend = (f32)age / (f32)fadeIn;
                } else {
                    remain = lifetime - age;
                    if (fadeOut >= remain && fadeOut != 0) {
                        blend = (f32)remain / (f32)fadeOut;
                    } else {
                        blend = 1.0f;
                    }
                }
                color = effMultiplyPackedColors(effBlendColor(0, part->color, blend), tintColor);
                effFlashRotatingQuadSetParticleColors(work, index, color);
            }
        }
        part->age = part->age + 1;
    }
    handle = work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->updateCount = work->updateCount + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->scale = work->renderScale;
    effDrawScaledRecordPool(handle);
}

extern EffRecordPool *effRecordPoolCreateTriple(s32 count);

PcpFlashRadialTriangleWork *effFlashRadialTriangleCreate(src)
    PcpFlashRadialTriangleWork *src;
{
    SdfMemBlock *handle = sdfAllocGeneralBlock(src->particleCount * sizeof(PcpFlashMotionParticle) + sizeof(PcpFlashRadialTriangleWork));
    PcpFlashRadialTriangleWork *work = (PcpFlashRadialTriangleWork *)sdfResourceRetainAddress(handle);
    EffRecordPool *record;
    u32 i;

    memcpy(work, src, 0x3C);
    work->parts = (PcpFlashMotionParticle *)(work + 1);
    work->tintColor = 0x80808080;
    work->allocationHandle = handle;
    work->renderScale = 1.0f;
    work->updateCount = 0;
    record = effRecordPoolCreateTriple(work->particleCount);
    work->resourceHandle = record;
    record->drawMode = work->unk38;
    for (i = 0; i < work->particleCount; i++) {
        work->parts[i].age = 0;
    }
    return work;
}

void effFlashRadialTriangleSpawnFromTable(void *data)
{
    effFlashRadialTriangleCreate(effParamTableGetBlock(data, 0));
}

void func_0016D400(void)
{
    effFlashRadialTriangleCreate();
}

void effFlashRadialTriangleDestroy(PcpFlashRadialTriangleWork *work)
{
    effReleaseRecordPoolResourceAndBuffer(work->resourceHandle);
    sdfReleaseResourceAllocation(work->allocationHandle);
}

void effFlashRadialTriangleCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effFlashRadialTriangleSetColorParam(PcpFlashRadialTriangleWork *work, u32 value)
{
    work->tintColor = value;
}

void effFlashRadialTriangleSetRenderScale(PcpFlashRadialTriangleWork *work, f32 value)
{
    work->renderScale = value;
}

void effFlashRadialTriangleSetParticleColors(PcpFlashRadialTriangleWork *work, s32 index, s32 param)
{
    PcpFlashColorSlot *slot;
    s32 rgb1;
    s32 rgb2;

    slot = (PcpFlashColorSlot *)effGetGroupIndexRecord(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    slot->first = effMultiplyPackedColors(rgb2, param);
    slot->second = effMultiplyPackedColors(rgb2, param);
    slot->third = effMultiplyPackedColors(rgb1 | 0xFF000000, param);
}

/* vu0 routine: a triangle of corner offsets for a flash particle, two of them turned around the view axis by index * step */
void effFlashRotatedTriangle(PcpFlashRadialTriangleWork *work, s32 index, void *view)
{
    PcpFlashMotionParticle *part = &work->parts[index];
    f32 *quad = (f32 *)effGetGroupRecordByIndex(work->resourceHandle, index);
    f32 base[4];
    f32 size[4];
    f32 step;
    f32 angle;
    f32 radius;

    step = 3.14159265f * 2.0f / (f32)(u32)work->particleCount;
    radius = part->accumulator;
    VEC3_SPLAT(size, radius);
    angle = step * (f32)index;
    sdfBuildVuRotationFromAxisAngle(angle, view);
    base[0] = 0;
    base[1] = 1.0f;
    base[2] = 0;
    VU0_LOAD_VF(vf10, base);
    VU0_LOAD_VF(vf11, view);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, base);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_LOAD_VF(vf11, size);
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, D_00354950);
    VU0_STORE_VF(vf10, quad + 8);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad);
    sdfBuildVuRotationFromAxisAngle(angle + step, view);
    VU0_LOAD_VF(vf10, base);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_LOAD_VF(vf11, size);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_00354950);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 4);
}
void effFlashRadialTriangleUpdate(PcpFlashRadialTriangleWork *work) {
    s128 axis;
    s32 restart;
    s32 tintColor;
    s32 index;
    s32 active;
    s32 count;
    s32 lifetime;
    s32 fadeIn;
    s32 fadeOut;
    f32 startA;
    f32 startB;
    f32 decay;
    PcpFlashMotionParticle *part;
    EffRecordPool *handle;

    VU0_LOAD_VF($vf10, sdfViewEyeVector);
    VU0_LOAD_VF($vf11, sdfViewTargetVector);
        VU0_SUB(vf10, vf10, vf11);;
        VU0_NORMALIZE_VF10();;
    VU0_STORE_VF($vf10, &axis);
    count = work->particleCount;
    part = work->parts;
    lifetime = work->lifetime;
    startA = work->initialRadius;
    startB = work->initialRadialSpeed;
    decay = work->radialDamping;
    fadeIn = work->fadeInTime;
    fadeOut = work->fadeOutTime;
    restart = work->restartRandomly;
    tintColor = work->tintColor;
    active = 0;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;

        if (lifetime < age) {
            effFlashRadialTriangleSetParticleColors(work, index, 0);
        } else {
            if (age == 0) {
                effFlashRotatedTriangle(work, index, &axis);
                effFlashRadialTriangleSetParticleColors(work, index, 0);
                part->accumulator = startA;
                part->stepSpeed = startB;
                part->color = 0x80808080;
            } else if (age > 0) {
                f32 speed = part->stepSpeed;
                s32 remain;
                f32 blend;

                part->stepSpeed = speed * decay;
                part->accumulator = part->accumulator + speed;
                effFlashRotatedTriangle(work, index, &axis);
                if (age < fadeIn && fadeIn != 0) {
                    blend = (f32)age / (f32)fadeIn;
                } else {
                    remain = lifetime - age;
                    if (fadeOut >= remain && fadeOut != 0) {
                        blend = (f32)remain / (f32)fadeOut;
                    } else {
                        blend = 1.0f;
                    }
                }
                active++;
                effFlashRadialTriangleSetParticleColors(work, index, effMultiplyPackedColors(effBlendColor(0, part->color, blend), tintColor));
            }
            if (age == lifetime && restart != 0) {
                part->age = 0;
            } else {
                part->age = part->age + 1;
            }
        }
    }
    handle = work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->updateCount = work->updateCount + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->scale = work->renderScale;
    if (active != 0) {
        effDrawTriangleRecordPool(handle);
    }
}

/* Clone the 0xD0-byte parameter block, then give every particle a random negative start age (two handle slots per particle). */
PcpFlashRadialStripWork *effFlashRadialStripCreate(src)
    PcpFlashRadialStripWork *src;
{
    SdfMemBlock *handle = sdfAllocGeneralBlock(src->particleCount * sizeof(PcpFlashRadialStripParticle) + sizeof(PcpFlashRadialStripWork));
    PcpFlashRadialStripWork *work = (PcpFlashRadialStripWork *)sdfResourceRetainAddress(handle);
    EffRecordPool *record;
    u32 range;
    u32 i;

    memcpy(work, src, 0xD0);
    work->parts = (PcpFlashRadialStripParticle *)(work + 1);
    work->allocationHandle = handle;
    work->tintColor = 0x80808080;
    work->updateCount = 0;
    work->renderScale = 1.0f;
    if (work->randomRange == 0) {
        work->randomRange = 1;
    }
    record = effRecordPoolCreate(work->particleCount * 2);
    record->scale = 1.0f;
    record->drawMode = work->unk4C;
    work->resourceHandle = record;
    range = work->randomRange;
    for (i = 0; i < work->particleCount; i++) {
        work->parts[i].age = -(effMiscRand(D_0034DF38) % range);
    }
    return work;
}

void effFlashRadialStripSpawnFromTable(void *data)
{
    effFlashRadialStripCreate(effParamTableGetBlock(data, 0));
}

void func_0016DB38(void)
{
    effFlashRadialStripCreate();
}

void effFlashRadialStripDestroy(PcpFlashRadialStripWork *work)
{
    effReleaseRecordGroupResources(work->resourceHandle);
    sdfReleaseResourceAllocation(work->allocationHandle);
}

void effFlashRadialStripCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effFlashRadialStripSetColorParam(PcpFlashRadialStripWork *work, u32 value)
{
    work->tintColor = value;
}

void effFlashRadialStripSetRenderScale(PcpFlashRadialStripWork *work, f32 value)
{
    work->renderScale = value;
}

void func_0016DBA0(PcpFlashRadialStripWork *work, s32 index, u32 param) {
    u32 *colors;
    u32 colorA;
    u32 colorB;
    u32 *mirror;

    colors = (u32 *)effGetRecordGroupAuxEntry(work->resourceHandle, index * 2);
    colorA = work->colorA & 0xFFFFFF;
    colorB = work->colorB & 0xFFFFFF;
    colors[0] = effMultiplyPackedColors(colorB | 0x80000000, param);
    colors[1] = effMultiplyPackedColors(colorB, param);
    colors[2] = effMultiplyPackedColors(colorA | 0x80000000, param);
    colors[3] = effMultiplyPackedColors(colorB, param);
    mirror = (u32 *)effGetRecordGroupAuxEntry(work->resourceHandle, index * 2 + 1);
    mirror[0] = colors[0];
    mirror[1] = colors[1];
    mirror[2] = colors[2];
    mirror[3] = colors[3];
}

void effFlashSpawnStripParticle(PcpFlashRadialStripWork *work, s32 index, void *orientation) {
    PcpFlashRadialStripParticle *part = work->parts + index;
    f32 factor;

    part->angle = effMiscRandUnitFloat(D_0034DF38) * 6.2831853f;
    factor = effMiscRandUnitFloat(D_0034DF38) * 0.3f + 0.7f;
    part->thickness = work->maxScale * factor;
    factor = work->unk34;
    part->span = work->unk30 * (effMiscRandUnitFloat(D_0034DF38) * factor + (1.0f - factor));
    part->angularStep = work->unk3C * ((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f);
}

/* vu0 routine: two quads of corner offsets for a flash particle (a strip and its mirror), turned around the view axis by the particle's angle */
void effFlashRotatedStripPair(PcpFlashRadialStripWork *work, s32 index, void *view)
{
    PcpFlashRadialStripParticle *part = &work->parts[index];
    f32 *quad = (f32 *)effGetRecordGroupElement(work->resourceHandle, index * 2);
    f32 base[4];
    f32 size[4];
    f32 spare[4];
    f32 middle[4];
    f32 outer[4];
    f32 inner[4];
    f32 center;
    f32 outerEdge;
    f32 innerEdge;
    f32 span;
    f32 *mirror;

    center = part->radius;
    VEC3_SPLAT(middle, center);
    outerEdge = center + part->thickness;
    VEC3_SPLAT(outer, outerEdge);
    innerEdge = center - part->thickness;
    VEC3_SPLAT(inner, innerEdge);
    span = part->span;
    VEC3_SPLAT(size, span);
    sdfBuildVuRotationFromAxisAngle(part->angle, view);
    base[0] = 0;
    base[1] = 1.0f;
    base[2] = 0;
    VU0_LOAD_VF(vf10, base);
    VU0_LOAD_VF(vf11, view);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_MOVE_VF(vf12, vf10);
    /* retail multiplies the (never written) `spare` slot here and stores it back */
    VU0_LOAD_VF(vf11, spare);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, spare);
    VU0_MOVE_VF(vf10, vf12);
    VU0_LOAD_VF(vf11, view);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, size);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, size);
    VU0_MOVE_VF(vf10, vf12);
    VU0_LOAD_VF(vf11, outer);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, outer);
    VU0_MOVE_VF(vf10, vf12);
    VU0_LOAD_VF(vf11, inner);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, inner);
    VU0_MOVE_VF(vf10, vf12);
    VU0_LOAD_VF(vf11, middle);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, size);
    VU0_STORE_VF(vf10, quad + 8);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 4);
    VU0_SUB(vf10, vf10, vf11);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 12);
    VU0_LOAD_VF(vf10, outer);
    VU0_STORE_VF(vf10, quad);
    mirror = (f32 *)effGetRecordGroupElement(work->resourceHandle, index * 2 + 1);
    PCP_COPY_VECTOR(mirror + 8, quad + 8);
    PCP_COPY_VECTOR(mirror + 4, quad + 4);
    PCP_COPY_VECTOR(mirror + 12, quad + 12);
    VU0_LOAD_VF(vf10, inner);
    VU0_STORE_VF(vf10, mirror);
}
void effFlashRadialStripAdvanceAngle(PcpFlashRadialStripWork *work, s32 index, void *orientation)
{
    PcpFlashRadialStripParticle *part;

    part = &work->parts[index];
    part->angle += part->angularStep;
}

extern void func_0016DBA0(PcpFlashRadialStripWork *, s32, u32);

void effFlashRadialStripUpdate(PcpFlashRadialStripWork *work) {
    s128 axis;
    s32 index;
    s32 count;
    s32 lifetime;
    f32 maxScale;
    s32 fadeIn;
    s32 fadeOut;
    f32 startA;
    f32 startB;
    f32 decay;
    s32 restart;
    u32 range;
    s32 tintColor;
    PcpFlashRadialStripParticle *part;
    EffRecordPool *handle;

    VU0_LOAD_VF($vf10, sdfViewEyeVector);
    VU0_LOAD_VF($vf11, sdfViewTargetVector);
        VU0_SUB(vf10, vf10, vf11);;
    VU0_STORE_VF($vf10, &axis);
    count = work->particleCount;
    part = work->parts;
    lifetime = work->lifetime;
    maxScale = work->maxScale;
    fadeIn = work->fadeInTime;
    fadeOut = work->fadeOutTime;
    startA = work->initialRadius;
    startB = work->initialRadialSpeed;
    decay = work->radialDamping;
    restart = work->restartRandomly;
    range = work->randomRange;
    tintColor = work->tintColor;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;

        if (lifetime < age) {
            func_0016DBA0(work, index, 0);
        } else {
            if (age == 0) {
                effFlashSpawnStripParticle(work, index, &axis);
                effFlashRotatedStripPair(work, index, &axis);
                func_0016DBA0(work, index, 0);
                part->thickness = maxScale;
                part->radius = startA;
                part->radialSpeed = startB;
                part->color = 0x80808080;
            } else if (age > 0) {
                f32 speed = part->radialSpeed;
                s32 remain;
                f32 blend;

                part->radialSpeed = speed * decay;
                part->radius = part->radius + speed;
                effFlashRadialStripAdvanceAngle(work, index, &axis);
                effFlashRotatedStripPair(work, index, &axis);
                age = part->age;
                if (age < fadeIn && fadeIn != 0) {
                    blend = (f32)age / (f32)fadeIn;
                } else {
                    remain = lifetime - age;
                    if (fadeOut >= remain && fadeOut != 0) {
                        blend = (f32)remain / (f32)fadeOut;
                    } else {
                        blend = 1.0f;
                    }
                }
                func_0016DBA0(work, index, effMultiplyPackedColors(effBlendColor(0, part->color, blend), tintColor));
            }
            if (age == lifetime && restart != 0) {
                part->age = ~(effMiscRand(D_0034DF38) % range);
                func_0016DBA0(work, index, 0);
            } else {
                part->age = part->age + 1;
            }
        }
    }
    handle = work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->updateCount = work->updateCount + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->scale = work->renderScale;
    effDrawQuadRecordPool(handle);
}

/* Clone the 0x50-byte parameter block, then spread the particles evenly around the orbit from -pi/2 with random negative start ages. */
PcpFlashFadingOrbitWork *effFlashFadingOrbitCreate(src)
    PcpFlashFadingOrbitWork *src;
{
    SdfMemBlock *handle = sdfAllocGeneralBlock(src->particleCount * sizeof(PcpFlashOrbitParticle) + sizeof(PcpFlashFadingOrbitWork));
    PcpFlashFadingOrbitWork *work = (PcpFlashFadingOrbitWork *)sdfResourceRetainAddress(handle);
    EffRecordPool *record;
    f32 angle;
    f32 step;
    u32 range;
    u32 i;

    memcpy(work, src, 0x50);
    work->parts = (PcpFlashOrbitParticle *)(work + 1);
    work->allocationHandle = handle;
    work->tintColor = 0x80808080;
    work->updateCount = 0;
    work->renderScale = 1.0f;
    if (work->randomRange == 0) {
        work->randomRange = 1;
    }
    angle = -3.14159265f / 2.0f;
    record = effRecordPoolCreateFiveVertexGroups(work->particleCount);
    record->scale = 1.0f;
    record->drawMode = work->unk4C;
    work->resourceHandle = record;
    step = 3.14159265f * 2.0f / (f32)(u32)work->particleCount;
    range = work->randomRange;
    for (i = 0; i < (u32)work->particleCount; i++) {
        work->parts[i].age = -(effMiscRand(D_0034DF38) % range);
        work->parts[i].angle = angle;
        angle += step;
    }
    return work;
}

void effFlashFadingOrbitSpawnFromTable(void *data)
{
    effFlashFadingOrbitCreate(effParamTableGetBlock(data, 0));
}

void func_0016E4E0(void)
{
    effFlashFadingOrbitCreate();
}

void effFlashFadingOrbitDestroy(PcpFlashFadingOrbitWork *work)
{
    effReleaseRecordGroupAssetAndHandle(work->resourceHandle);
    sdfReleaseResourceAllocation(work->allocationHandle);
}

void effFlashFadingOrbitCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effFlashFadingOrbitSetColorParam(PcpFlashFadingOrbitWork *work, u32 value)
{
    work->tintColor = value;
}

void effFlashFadingOrbitSetRenderScale(PcpFlashFadingOrbitWork *work, f32 value)
{
    work->renderScale = value;
}

void effFlashFadingOrbitSetParticleColors(PcpFlashFadingOrbitWork *work, s32 index, s32 param)
{
    PcpFlashQuadColorSlot *slot;
    s32 rgb1;
    s32 rgb2;

    slot = (PcpFlashQuadColorSlot *)effGetIndexedEffectGroupIndexEntry(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    slot->color[0] = effMultiplyPackedColors(rgb2, param);
    slot->color[1] = effMultiplyPackedColors(rgb2, param);
    if (index & 1) {
        slot->color[2] = effMultiplyPackedColors(0x80000000, param);
        slot->color[3] = effMultiplyPackedColors(rgb1 | 0xFF000000, param);
        slot->color[4] = effMultiplyPackedColors(0x80000000, param);
    } else {
        slot->color[2] = effMultiplyPackedColors(0xFF000000, param);
        slot->color[3] = effMultiplyPackedColors(rgb1 | 0x40000000, param);
        slot->color[4] = effMultiplyPackedColors(0xFF000000, param);
    }
}

/* vu0 routine: billboard corner offsets for a scaling particle on an arc, built from a normalised direction and its perpendicular */
void effFlashFadingOrbitWriteCorners(PcpFlashFadingOrbitWork *work, s32 index)
{
    PcpFlashOrbitParticle *part = &work->parts[index];
    f32 *quad = (f32 *)effGetIndexedEffectGroupRecord(work->resourceHandle, index);
    f32 offset[4];
    f32 unit[4];
    f32 scaleA[4];
    f32 scaleB[4];
    f32 scaleC[4];
    f32 size;
    f32 ratio;
    f32 sinv;
    f32 height;
    f32 widthB;
    f32 widthC;

    size = part->scale;
    ratio = size / part->initialScale;
    VEC3_SPLAT(scaleA, size);
    widthB = work->acrossSpan * ratio;
    widthC = work->upSpan * ratio;
    VEC3_SPLAT(scaleB, widthB);
    VEC3_SPLAT(scaleC, widthC);
    unit[0] = sdfEvaluateCosineViaSinePhaseShift(part->angle);
    unit[1] = 0;
    sinv = sdfSinPoly(part->angle);
    unit[2] = sinv;
    offset[0] = unit[0] * work->orbitRadius;
    offset[1] = 0;
    offset[2] = sinv * work->orbitRadius;
    height = work->tilt;
    D_00354960[0] = unit[0] * height;
    D_00354960[1] = height + -1.0f;
    D_00354960[2] = sinv * height;
    VU0_LOAD_VF(vf10, D_00354960);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf11, vf10);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf10, unit);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, scaleB);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, scaleB);
    VU0_LOAD_VF(vf10, scaleC);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, scaleC);
    VU0_MOVE_VF(vf10, vf12);
    VU0_LOAD_VF(vf11, scaleA);
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf10, offset);
    VU0_LOAD_VF(vf11, scaleC);
    VU0_STORE_VF(vf10, quad + 12);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 8);
    VU0_SUB(vf10, vf10, vf11);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 16);
    VU0_LOAD_VF(vf10, offset);
    VU0_LOAD_VF(vf11, scaleB);
    VU0_ADD(vf10, vf10, vf12);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad);
    VU0_SUB(vf10, vf10, vf11);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 4);
}
void effFlashFadingOrbitAdvanceAngle(PcpFlashFadingOrbitWork *work, s32 index)
{
    PcpFlashOrbitParticle *part;

    part = &work->parts[index];
    part->angle += work->angularStep;
}

void effFlashFadingOrbitUpdate(PcpFlashFadingOrbitWork *work) {
    s32 restart;
    s32 tintColor;
    s32 count;
    s32 index;
    s32 lifetime;
    s32 fadeIn;
    s32 fadeOut;
    s32 ramp;
    f32 maxScale;
    u32 range;
    PcpFlashOrbitParticle *part;
    EffRecordPool *handle;

    count = work->particleCount;
    part = work->parts;
    lifetime = work->lifetime;
    fadeIn = work->fadeInTime;
    fadeOut = work->fadeOutTime;
    ramp = work->scaleRampTime;
    maxScale = work->maxScale;
    restart = work->restartRandomly;
    range = work->randomRange;
    tintColor = work->tintColor;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;

        if (lifetime < age) {
            effFlashFadingOrbitSetParticleColors(work, index, 0);
        } else {
            if (age == 0) {
                part->initialScale = maxScale;
                if (ramp == 0) {
                    part->scale = maxScale;
                } else {
                    part->scale = 0.0f;
                }
                effFlashFadingOrbitWriteCorners(work, index);
                effFlashFadingOrbitSetParticleColors(work, index, 0);
                part->color = 0x80808080;
            } else if (age > 0) {
                s32 remain;
                f32 blend;

                if (ramp == 0) {
                    part->scale = maxScale;
                } else {
                    part->scale = (maxScale * (f32)age) / (f32)ramp;
                    if (maxScale < part->scale) {
                        part->scale = maxScale;
                    }
                }
                effFlashFadingOrbitAdvanceAngle(work, index);
                effFlashFadingOrbitWriteCorners(work, index);
                if (age < fadeIn && fadeIn != 0) {
                    blend = (f32)age / (f32)fadeIn;
                } else {
                    remain = lifetime - age;
                    if (fadeOut >= remain && fadeOut != 0) {
                        blend = (f32)remain / (f32)fadeOut;
                    } else {
                        blend = 1.0f;
                    }
                }
                effFlashFadingOrbitSetParticleColors(work, index, effMultiplyPackedColors(effBlendColor(0, part->color, blend), tintColor));
            }
            if (age == lifetime && restart != 0) {
                part->age = ~(effMiscRand(D_0034DF38) % range);
                effFlashFadingOrbitSetParticleColors(work, index, 0);
            } else {
                part->age = part->age + 1;
            }
        }
    }
    handle = work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->updateCount = work->updateCount + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->scale = work->renderScale;
    effDrawScaledRecordPool(handle);
}

PcpFlashOffsetRadialWork *effFlashOffsetRadialTriangleCreate(src)
    PcpFlashOffsetRadialWork *src;
{
    SdfMemBlock *handle = sdfAllocGeneralBlock(src->particleCount * sizeof(PcpFlashMotionParticle) + sizeof(PcpFlashOffsetRadialWork));
    PcpFlashOffsetRadialWork *work = (PcpFlashOffsetRadialWork *)sdfResourceRetainAddress(handle);
    EffRecordPool *record;
    u32 i;

    memcpy(work, src, 0x40);
    work->parts = (PcpFlashMotionParticle *)(work + 1);
    work->tintColor = 0x80808080;
    work->allocationHandle = handle;
    work->renderScale = 1.0f;
    work->updateCount = 0;
    record = effRecordPoolCreateTriple(work->particleCount);
    work->resourceHandle = record;
    record->drawMode = work->unk3C;
    for (i = 0; i < work->particleCount; i++) {
        work->parts[i].age = 0;
    }
    return work;
}

void effFlashOffsetRadialTriangleSpawnFromTable(void *data)
{
    effFlashOffsetRadialTriangleCreate(effParamTableGetBlock(data, 0));
}

void func_0016EC60(void)
{
    effFlashOffsetRadialTriangleCreate();
}

void effFlashOffsetRadialTriangleDestroy(PcpFlashOffsetRadialWork *work)
{
    effReleaseRecordPoolResourceAndBuffer(work->resourceHandle);
    sdfReleaseResourceAllocation(work->allocationHandle);
}

void effFlashOffsetRadialTriangleCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effFlashOffsetRadialTriangleSetColorParam(PcpFlashOffsetRadialWork *work, u32 value)
{
    work->tintColor = value;
}

void effFlashOffsetRadialTriangleSetRenderScale(PcpFlashOffsetRadialWork *work, f32 value)
{
    work->renderScale = value;
}

void effFlashOffsetRadialTriangleSetParticleColors(PcpFlashOffsetRadialWork *work, s32 index, s32 param)
{
    PcpFlashColorSlot *slot;
    s32 rgb1;
    s32 rgb2;

    slot = (PcpFlashColorSlot *)effGetGroupIndexRecord(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    slot->first = effMultiplyPackedColors(rgb2, param);
    slot->second = effMultiplyPackedColors(rgb2, param);
    slot->third = effMultiplyPackedColors(rgb1 | 0xFF000000, param);
}

/* vu0 routine: a triangle of corner offsets for a flash particle, two of them turned around the view axis by index * step */
void effFlashOffsetRadialWriteCorners(PcpFlashOffsetRadialWork *work, s32 index, void *view)
{
    PcpFlashMotionParticle *part = &work->parts[index];
    f32 *quad = (f32 *)effGetGroupRecordByIndex(work->resourceHandle, index);
    f32 base[4];
    f32 size[4];
    f32 step;
    f32 angle;
    f32 radius;

    step = 3.14159265f * 2.0f / (f32)(u32)work->particleCount;
    radius = part->accumulator;
    VEC3_SPLAT(size, radius);
    angle = step * (f32)index;
    sdfBuildVuRotationFromAxisAngle(angle, view);
    base[0] = 0;
    base[1] = 1.0f;
    base[2] = 0;
    VU0_LOAD_VF(vf10, base);
    VU0_LOAD_VF(vf11, view);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, base);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_LOAD_VF(vf11, size);
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, D_00354970);
    VU0_STORE_VF(vf10, quad + 8);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad);
    sdfBuildVuRotationFromAxisAngle(angle + step, view);
    VU0_LOAD_VF(vf10, base);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_LOAD_VF(vf11, size);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_00354970);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 4);
}
void effFlashOffsetRadialTriangleUpdate(PcpFlashOffsetRadialWork *work) {
    f32 axis[4];
    s32 restart;
    s32 tintColor;
    s32 index;
    s32 active;
    s32 count;
    s32 lifetime;
    s32 fadeIn;
    s32 fadeOut;
    f32 startA;
    f32 startB;
    f32 decay;
    f32 scale;
    PcpFlashMotionParticle *part;
    EffRecordPool *handle;

    VU0_LOAD_VF($vf10, sdfViewEyeVector);
    VU0_LOAD_VF($vf11, sdfViewTargetVector);
        VU0_SUB(vf10, vf10, vf11);;
        VU0_NORMALIZE_VF10();;
    VU0_STORE_VF($vf10, axis);
    count = work->particleCount;
    part = work->parts;
    lifetime = work->lifetime;
    startA = work->initialRadius;
    startB = work->initialRadialSpeed;
    decay = work->radialDamping;
    fadeIn = work->fadeInTime;
    fadeOut = work->fadeOutTime;
    restart = work->restartRandomly;
    tintColor = work->tintColor;
    active = 0;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;

        if (lifetime < age) {
            effFlashOffsetRadialTriangleSetParticleColors(work, index, 0);
        } else {
            if (age == 0) {
                effFlashOffsetRadialWriteCorners(work, index, axis);
                effFlashOffsetRadialTriangleSetParticleColors(work, index, 0);
                part->accumulator = startA;
                part->stepSpeed = startB;
                part->color = 0x80808080;
            } else if (age > 0) {
                f32 speed = part->stepSpeed;
                s32 remain;
                f32 blend;

                part->stepSpeed = speed * decay;
                part->accumulator = part->accumulator + speed;
                effFlashOffsetRadialWriteCorners(work, index, axis);
                if (age < fadeIn && fadeIn != 0) {
                    blend = (f32)age / (f32)fadeIn;
                } else {
                    remain = lifetime - age;
                    if (fadeOut >= remain && fadeOut != 0) {
                        blend = (f32)remain / (f32)fadeOut;
                    } else {
                        blend = 1.0f;
                    }
                }
                active++;
                effFlashOffsetRadialTriangleSetParticleColors(work, index, effMultiplyPackedColors(effBlendColor(0, part->color, blend), tintColor));
            }
            if (age == lifetime && restart != 0) {
                part->age = 0;
            } else {
                part->age = part->age + 1;
            }
        }
    }
    scale = work->originOffset * work->renderScale;
    handle = work->resourceHandle;
    work->updateCount = work->updateCount + 1;
    handle->origin[0] = work->origin[0] + axis[0] * scale;
    handle->origin[1] = work->origin[1] + axis[1] * scale;
    handle->origin[2] = work->origin[2] + axis[2] * scale;
    handle->scale = work->renderScale;
    if (active != 0) {
        effDrawTriangleRecordPool(handle);
    }
}
