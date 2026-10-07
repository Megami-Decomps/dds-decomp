#include "eff.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

#define EFF_MAGATUHI_VECTOR_WORD_COUNT 4
#define EFF_MAGATUHI_MATRIX_WORD_COUNT 16
#define EFF_MAGATUHI_CONTROL_POINT_COUNT 4
#define EFF_MAGATUHI_XYZ_COMPONENT_COUNT 3
#define EFF_MAGATUHI_BEZIER_GROUP_STRIDE 4
#define EFF_MAGATUHI_REPLAY_GROUP_STRIDE 3
#define EFF_MAGATUHI_DELAY_WORD_BYTES 4
#define EFF_MAGATUHI_NEUTRAL_COLOR 0x80808080
#define EFF_MAGATUHI_RANDOM_MIDPOINT 0.5f
#define EFF_MAGATUHI_FULL_TURN 6.2831853f
#define EFF_MAGATUHI_HALF_TURN 3.14159265f
#define EFF_MAGATUHI_FADE_ALPHA_SCALE 127.0f
#define EFF_MAGATUHI_ALPHA_SHIFT 24
#define EFF_MAGATUHI_NEUTRAL_RGB 0x808080
#define EFF_MAGATUHI_CALLBACK_TYPE 3
#define EFF_MAGATUHI_REPLAY_NONE 0
#define EFF_MAGATUHI_REPLAY_BEZIER 1
#define EFF_MAGATUHI_REPLAY_RING 2
#define EFF_MAGATUHI_REPLAY_ORBIT 3
#define EFF_MAGATUHI_REPLAY_DRIFT 4


/* Callback input prefix; type 3 contains an effect-dispatch object at +8. */
typedef struct {
    s32 type;
    u8 pad04[4];
    void *effect;
} EffMagatuhiCallback;

extern void *effGetHandlerArg(void *arg);
extern u32 effBlendColor(u32 colorA, u32 colorB, f32 t);
extern u32 effMultiplyPackedColors(u32 colorA, u32 colorB);

/* Four control points and their jitter scales; copy xyz but preserve each w. */
typedef struct {
    f32 controlPoints[EFF_MAGATUHI_CONTROL_POINT_COUNT][EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 jitterScales[EFF_MAGATUHI_CONTROL_POINT_COUNT];
} EffMagatuhiRowsSrc;





/* Parameter head of the first family: particles of 0x30 bytes precede the work. */
typedef struct {
    f32 unk00, unk04, unk08;
    u8 pad0C[4];
    f32 unk10, unk14, unk18;
    u8 pad1C[4];
    u8 respawn;            /* 0x20 */
    u8 pad21[3];
    s32 lifetimeFrames;    /* 0x24 */
    s32 delaySpread;       /* 0x28 modulus of the particle delay */
    s32 fadeInFrames;      /* 0x2C */
    s32 fadeOutFrames;     /* 0x30 */
    f32 initialRadius;
    f32 baseLiftStep;      /* 0x38 */
    f32 liftVariation;
    f32 angleStep;         /* 0x40 */
    f32 baseScale;         /* 0x44 */
    f32 scaleStep;         /* 0x48 */
    f32 scaleVariation;    /* 0x4C */
    f32 captureDistance;   /* 0x50: transition from drift to the path */
    f32 pathJitter;        /* 0x54 */
    f32 pathStep;          /* 0x58 */
    f32 pathAcceleration;  /* 0x5C: multiplier applied to the path step */
    u32 particleCount;     /* 0x60 */
    u8 pad64[0x118];
} EffMagatuhiHeadFirst; /* 0x17C */

/* Shared 0x30-byte state: base position, planar drift, and accumulated lift. */
typedef struct {
    f32 position[EFF_MAGATUHI_XYZ_COMPONENT_COUNT];
    u8 pad0C[4];
    f32 driftState[EFF_MAGATUHI_XYZ_COMPONENT_COUNT]; /* XZ direction, Y accumulated lift */
    u8 pad1C[4];
    s32 age; /* negative: waiting to start; nonnegative: elapsed frames */
    f32 scale;
    f32 angle;
    f32 liftStep;
} EffMagatuhiDriftParticle;

typedef struct {
    EffMagatuhiHeadFirst head;
    EffMagatuhiDriftParticle *particles; /* 0x17C */
    void *mathResource;    /* 0x180 */
    u8 pad184[8];
    EffMagatuhiOwner *managedResource; /* 0x18C */
    void *allocationHandle; /* 0x190 opaque handle, not the particle address */
} EffMagatuhiWideFirst;

/* Bezier parameter head: particle count and delay spread at their native offsets. */
typedef struct {
    f32 controlPoints[EFF_MAGATUHI_CONTROL_POINT_COUNT][EFF_MAGATUHI_VECTOR_WORD_COUNT];
    u8 respawn;            /* 0x40 restart finished particles */
    u8 pad41[3];
    s32 lifetimeFrames;    /* 0x44 frames a particle lives */
    s32 delaySpread;       /* 0x48 modulus of the particle delay */
    s32 fadeIn;            /* 0x4C */
    s32 fadeOut;           /* 0x50 */
    f32 jitterScales[EFF_MAGATUHI_CONTROL_POINT_COUNT]; /* 0x54 */
    u32 particleCount;     /* 0x64 */
    u32 duration;          /* 0x68 */
    u8 pad6C[0x114];
} EffMagatuhiHeadSecond; /* 0x180 */

typedef struct {
    EffMagatuhiHeadSecond head;
    s32 *delays;           /* 0x180 */
    void *mathResource;    /* 0x184 */
    EffMagatuhiOwner *managedResource; /* 0x188 */
    void *allocationHandle; /* 0x18C opaque handle, not the work address */
} EffMagatuhiWideSecond;

/* Float source block read by effMagatuhiCopyFloatBlock. */
typedef struct {
    f32 unk00, unk04, unk08;
    u8 pad0C[4];
    f32 unk10, unk14, unk18;
    u8 pad1C[4];
    f32 initialRadius, unk24;
} EffMagatuhiFloatParams; /* 0x28 */


/* Release the allocation retained by the render/history owner. */
void effMagatuhiReleaseResource(EffMagatuhiValueWork *work) {
    sdfReleaseResourceAllocation(work->allocationHandle);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_001893D8);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00189818);

/* Store a per-slot packed color; the native path does not validate the index. */
void effMagatuhiSetValue(EffMagatuhiValueWork *work, s32 index, u32 color) {
    work->slotColors[index] = color;
}

/* Accumulate 1/historyCount per entry; the nominal last fraction is below 1. */
void effMagatuhiFillColorTable(EffMagatuhiValueWork *work, u32 colorA, u32 colorB) {
    f32 blendFraction = 0.0f;
    u32 historyCount = work->historyCount;
    u32 *paletteEntry = work->colorTable;
    f32 blendStep = 1.0f / historyCount;
    u32 i;

    for (i = 0; i < historyCount; i++) {
        *paletteEntry++ = effBlendColor(colorA, colorB, blendFraction);
        blendFraction += blendStep;
    }
}

extern f32 effMiscRandUnitFloat(void *state);
extern u8 D_0034DF38[];
extern s32 effMathStepBezierSlot(void *slots, s32 index, f32 *out);
extern f32 sdfViewTargetVector[EFF_MAGATUHI_VECTOR_WORD_COUNT];
extern f32 sdfViewEyeVector[EFF_MAGATUHI_VECTOR_WORD_COUNT];
extern f32 sdfSinPoly(f32 angle);
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32 angle);
extern s32 effMathGetSlotAt(void *slots, s32 index);


void func_00189C80(EffMagatuhiValueWork *valueWork, s32 index) {
    effResetMagatuhiValueSlot(valueWork, index);
}

/* Ring state is one 0x18-byte slot; replay visits every third slot. */
typedef struct {
    s32 age; /* negative: waiting to start; nonnegative: elapsed frames */
    f32 height;
    f32 angle;
    f32 angleStep;
    f32 radius;
    f32 radiusStep;
} EffMagatuhiRingParticle;


extern u32 sdfAllocGeneralBlock(s32 size);
extern u8 *sdfResourceRetainAddress(u32 handle);
extern void *effAllocSlotArray(s32 count);
extern u32 effMiscRand(void *state);
extern u8 D_0034DF38[];

/* Clone the first-family parameters; return the work after its state array.
 * Clamp only the copied delay modulus, leaving the caller's parameters intact. */
EffMagatuhiWideFirst *effMagatuhiCreateFirst(EffMagatuhiHeadFirst *src) {
    u32 count = src->particleCount;
    u32 size = count * sizeof(EffMagatuhiDriftParticle);
    u32 handle = sdfAllocGeneralBlock(size + sizeof(EffMagatuhiWideFirst));
    EffMagatuhiDriftParticle *particle = (EffMagatuhiDriftParticle *)sdfResourceRetainAddress(handle);
    EffMagatuhiWideFirst *work = (EffMagatuhiWideFirst *)((u8 *)particle + size);
    s32 spread;
    u32 i;

    work->head = *src;
    work->allocationHandle = (void *)handle;
    work->particles = particle;
    if (work->head.delaySpread <= 0) {
        work->head.delaySpread = 1;
    }
    work->managedResource = effCloneMagatuhiWithColorResource(&work->head.particleCount);
    work->mathResource = effAllocSlotArray(count);
    spread = work->head.delaySpread;
    for (i = 0; i < count; i++) {
        particle->age = -(effMiscRand(D_0034DF38) % spread);
        particle++;
    }
    return work;
}

/* Release math state, the history owner, then the backing allocation. */
void effMagatuhiReleaseMathOwnerAndBuffer(EffMagatuhiWideFirst *work) {
    effMathReleaseWorkResource(work->mathResource);
    effReleaseMagatuhiOwner(work->managedResource);
    sdfReleaseResourceAllocation(work->allocationHandle);
}

typedef struct {
    f32 controlPoints[EFF_MAGATUHI_CONTROL_POINT_COUNT][EFF_MAGATUHI_XYZ_COMPONENT_COUNT];
    f32 t;    /* 0x30 Bezier evaluation parameter, as in effMath's slot */
    f32 step; /* 0x34 per-frame increment of t */
} EffMagatuhiSlot;
/* Seed independent normalized XZ position/drift vectors and clear slot history.
 * The vector scratch w and the raw accesses below retain their native forms. */
void func_00189E98(EffMagatuhiWideFirst *work, s32 index) {
    EffMagatuhiDriftParticle *particle = &work->particles[index];
    f32 direction[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 radius;
    f32 random;
    EffMagatuhiSlot *slot;

    random = effMiscRandUnitFloat(D_0034DF38) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    radius = work->head.initialRadius * (random + random);
    random = effMiscRandUnitFloat(D_0034DF38) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    direction[0] = random + random;
    direction[1] = 0.0f;
    random = effMiscRandUnitFloat(D_0034DF38) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    direction[2] = random + random;
    VU0_LOAD_VF_FROM(vf10, *(u128 *)direction);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF_TO_MEMORY(vf10, *(u128 *)direction);
    particle->position[0] = radius * direction[0];
    particle->position[1] = 0.0f;
    particle->position[2] = radius * direction[2];
    random = effMiscRandUnitFloat(D_0034DF38) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    particle->driftState[0] = random + random;
    particle->driftState[1] = 0.0f;
    random = effMiscRandUnitFloat(D_0034DF38) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    particle->driftState[2] = random + random;
    VU0_LOAD_VF(vf10, particle->driftState);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, particle->driftState);
    particle->scale = work->head.baseScale *
        (effMiscRandUnitFloat(D_0034DF38) * work->head.scaleVariation +
         (1.0f - work->head.scaleVariation));
    particle->angle = effMiscRandUnitFloat(D_0034DF38) * EFF_MAGATUHI_FULL_TURN;
    particle->liftStep = work->head.baseLiftStep *
        (effMiscRandUnitFloat(D_0034DF38) * work->head.liftVariation +
         (1.0f - work->head.liftVariation));
    slot = (EffMagatuhiSlot *)effMathGetSlotAt(work->mathResource, index);
    slot->t = 0.0f;
    slot->step = 0.0f;
    func_00189C80(work->managedResource->valueWork, index);
    effMagatuhiSetValue(work->managedResource->valueWork, index, 0);
}

/* Drift until the particle enters the capture radius, then follow a randomized
 * cubic path to the target. Fade-in takes priority over an overlapping fade-out.
 * Preserve the native unwritten point w and unchecked fade divisors. */
void func_0018A098(EffMagatuhiWideFirst *work) {
    f32 point[4];
    f32 origin[4];
    f32 basePosition[4];
    f32 target[4];
    f32 viewDirection[4];
    f32 sideAxis[4];
    f32 towardTarget[4];
    f32 startTangent[4];
    void *mathResource;
    EffMagatuhiValueWork *valueWork;
    EffMagatuhiDriftParticle *particle;
    EffMagatuhiSlot *slot;
    u8 respawn;
    u32 count;
    s32 life, spread, fadeIn, fadeOut;
    f32 angleStep, scaleStep;
    f32 captureDistance, pathJitter, pathStep, pathAcceleration;
    f32 radial, distance, segmentDistance, offset, random, fade;
    s32 age;
    u32 i;

    VU0_LOAD_VF(vf10, sdfViewTargetVector);
    VU0_LOAD_VF(vf11, sdfViewEyeVector);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, viewDirection);
    mathResource = work->mathResource;
    valueWork = work->managedResource->valueWork;
    particle = work->particles;
    count = work->head.particleCount;
    respawn = work->head.respawn;
    life = work->head.lifetimeFrames;
    spread = work->head.delaySpread;
    angleStep = work->head.angleStep;
    scaleStep = work->head.scaleStep;
    fadeIn = work->head.fadeInFrames;
    fadeOut = work->head.fadeOutFrames;
    slot = (EffMagatuhiSlot *)effMathGetSlotAt(mathResource, 0);
    PCP_COPY_VECTOR(origin, &work->head.unk00);
    captureDistance = work->head.captureDistance;
    pathJitter = work->head.pathJitter;
    pathStep = work->head.pathStep;
    pathAcceleration = work->head.pathAcceleration;
    PCP_COPY_VECTOR(target, &work->head.unk10);
    for (i = 0; i < count; i++, particle++, slot++) {
        age = particle->age;
        if (age == 0) {
            func_00189E98(work, i);
        }
        if (age > 0 && age <= life) {
            if (slot->step == 0.0f) {
                PCP_COPY_VECTOR(basePosition, particle->position);
                radial = particle->scale * sdfSinPoly(particle->angle);
                particle->driftState[1] += particle->liftStep;
                point[0] = origin[0] + particle->position[0] + particle->driftState[0] * radial;
                point[1] = origin[1] + particle->position[1] + particle->driftState[1];
                point[2] = origin[2] + particle->position[2] + particle->driftState[2] * radial;
                particle->scale += scaleStep;
                particle->angle += angleStep;
                VU0_LOAD_VF(vf10, target);
                VU0_LOAD_VF(vf11, point);
                VU0_SUB(vf10, vf10, vf11);
                VU0_LENGTH_VF10(distance);
                if (distance <= captureDistance) {
                    VU0_NORMALIZE_VF10();
                    VU0_STORE_VF(vf10, towardTarget);
                    VU0_LOAD_VF(vf11, viewDirection);
                    VU0_CROSS_XYZ(vf10, vf10, vf11);
                    VU0_NORMALIZE_VF10();
                    VU0_STORE_VF(vf10, sideAxis);
                    VU0_LOAD_VF(vf10, point);
                    VU0_LOAD_VF(vf11, basePosition);
                    VU0_SUB(vf10, vf10, vf11);
                    VU0_NORMALIZE_VF10();
                    VU0_STORE_VF(vf10, startTangent);
                    slot->controlPoints[0][0] = point[0];
                    slot->controlPoints[0][1] = point[1];
                    slot->controlPoints[0][2] = point[2];
                    segmentDistance = captureDistance * 0.3999999762f;
                    offset = pathJitter * (effMiscRandUnitFloat(D_0034DF38) * 0.5f + 0.5f);
                    slot->controlPoints[1][0] = point[0] + towardTarget[0] * segmentDistance + startTangent[0] * offset;
                    slot->controlPoints[1][1] = point[1] + towardTarget[1] * segmentDistance + startTangent[1] * offset;
                    slot->controlPoints[1][2] = point[2] + towardTarget[2] * segmentDistance + startTangent[2] * offset;
                    segmentDistance = captureDistance * 0.6499999762f;
                    random = effMiscRandUnitFloat(D_0034DF38) - 0.5f;
                    offset = pathJitter * (random + random);
                    slot->controlPoints[2][0] = point[0] + towardTarget[0] * segmentDistance + sideAxis[0] * offset;
                    slot->controlPoints[2][1] = point[1] + towardTarget[1] * segmentDistance + sideAxis[1] * offset;
                    slot->controlPoints[2][2] = point[2] + towardTarget[2] * segmentDistance + sideAxis[2] * offset;
                    slot->controlPoints[3][0] = target[0];
                    slot->controlPoints[3][1] = target[1];
                    slot->controlPoints[3][2] = target[2];
                    slot->step = pathStep;
                }
            } else {
                effMathStepBezierSlot(mathResource, i, point);
                slot->step *= pathAcceleration;
            }
            if (age < fadeIn) {
                fade = (f32)age / (f32)fadeIn;
            } else if (life - age <= fadeOut) {
                fade = (f32)(life - age) / (f32)fadeOut;
            } else {
                fade = 1.0f;
            }
            effMagatuhiSetValue(valueWork, i, ((u32)(fade * 127.0f) << 24) | 0x808080);
            func_00189818(valueWork, i, point);
        }
        if (age >= life && respawn) {
            particle->age = -(effMiscRand(D_0034DF38) % spread);
        } else {
            particle->age++;
        }
    }
    func_001891A8(work->managedResource);
}

/* Update the first family's scattered float parameters without touching gaps. */
void effMagatuhiCopyFloatBlock(EffMagatuhiCallback *work, EffMagatuhiFloatParams *src) {
    EffMagatuhiWideFirst *dst = effGetHandlerArg(work->effect);

    dst->head.unk00 = src->unk00;
    dst->head.unk04 = src->unk04;
    dst->head.unk08 = src->unk08;
    dst->head.initialRadius = src->initialRadius;
    dst->head.unk10 = src->unk10;
    dst->head.unk14 = src->unk14;
    dst->head.unk18 = src->unk18;
    dst->head.captureDistance = src->unk24;
}

/* Clone history parameters; signed delays follow the returned work block. */
EffMagatuhiWideSecond *effMagatuhiCreateBezierHistoryWork(EffMagatuhiHeadSecond *src) {
    u32 count = src->particleCount;
    u32 handle = sdfAllocGeneralBlock(count * EFF_MAGATUHI_DELAY_WORD_BYTES + sizeof(EffMagatuhiWideSecond));
    EffMagatuhiWideSecond *work = (EffMagatuhiWideSecond *)sdfResourceRetainAddress(handle);
    s32 *delays = (s32 *)(work + 1);
    s32 spread;
    u32 i;

    work->head = *src;
    work->allocationHandle = (void *)handle;
    work->delays = delays;
    if (work->head.delaySpread <= 0) {
        work->head.delaySpread = 1;
    }
    work->managedResource = effCloneMagatuhiWithColorResource(&work->head.particleCount);
    work->mathResource = effAllocSlotArray(count);
    spread = work->head.delaySpread;
    for (i = 0; i < count; i++) {
        *delays++ = -(effMiscRand(D_0034DF38) % spread);
    }
    return work;
}

/* Release Bezier slots, their history owner, then the backing allocation. */
void effMagatuhiReleaseWideWorkResources(EffMagatuhiWideSecond *work) {
    effMathReleaseWorkResource(work->mathResource);
    effReleaseMagatuhiOwner(work->managedResource);
    sdfReleaseResourceAllocation(work->allocationHandle);
}




/* Jitter control points perpendicular to the path and camera viewing direction. */
void effMagatuhiBuildBezierControlPointsVU(EffMagatuhiWideSecond *work, s32 index) {
    f32 scale[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 lastNormal[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 viewDirection[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 point[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    EffMagatuhiSlot *slot;
    f32 step;
    f32 random;

    VU0_LOAD_VF(vf10, sdfViewTargetVector);
    VU0_LOAD_VF(vf11, sdfViewEyeVector);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, viewDirection);
    step = 1.0f / (f32)work->head.lifetimeFrames;
    slot = (EffMagatuhiSlot *)effMathGetSlotAt(work->mathResource, index);
    slot->t = 0;
    slot->step = step;

    random = effMiscRandUnitFloat(D_0034DF38) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    scale[0] = work->head.jitterScales[0] * (random + random);
    scale[1] = scale[0];
    scale[2] = scale[0];
    VU0_LOAD_VF(vf10, work->head.controlPoints[0]);
    VU0_LOAD_VF(vf11, work->head.controlPoints[1]);
    VU0_MOVE_VF(vf12, vf10);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, viewDirection);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_LOAD_VF(vf11, scale);
    VU0_MUL(vf10, vf10, vf11);
    VU0_ADD(vf10, vf10, vf12);
    VU0_STORE_VF(vf10, point);
    slot->controlPoints[0][0] = point[0];
    slot->controlPoints[0][1] = point[1];
    slot->controlPoints[0][2] = point[2];

    random = effMiscRandUnitFloat(D_0034DF38) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    scale[0] = work->head.jitterScales[1] * (random + random);
    scale[1] = scale[0];
    scale[2] = scale[0];
    VU0_LOAD_VF(vf10, work->head.controlPoints[1]);
    VU0_LOAD_VF(vf11, work->head.controlPoints[2]);
    VU0_MOVE_VF(vf12, vf10);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, viewDirection);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_LOAD_VF(vf11, scale);
    VU0_MUL(vf10, vf10, vf11);
    VU0_ADD(vf10, vf10, vf12);
    VU0_STORE_VF(vf10, point);
    slot->controlPoints[1][0] = point[0];
    slot->controlPoints[1][1] = point[1];
    slot->controlPoints[1][2] = point[2];

    random = effMiscRandUnitFloat(D_0034DF38) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    scale[0] = work->head.jitterScales[2] * (random + random);
    scale[1] = scale[0];
    scale[2] = scale[0];
    VU0_LOAD_VF(vf10, work->head.controlPoints[2]);
    VU0_LOAD_VF(vf11, work->head.controlPoints[3]);
    VU0_MOVE_VF(vf12, vf10);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, viewDirection);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, lastNormal);
    VU0_LOAD_VF(vf11, scale);
    VU0_MUL(vf10, vf10, vf11);
    VU0_ADD(vf10, vf10, vf12);
    VU0_STORE_VF(vf10, point);
    slot->controlPoints[2][0] = point[0];
    slot->controlPoints[2][1] = point[1];
    slot->controlPoints[2][2] = point[2];

    /* The fourth point reuses the third segment's normalized side direction. */
    random = effMiscRandUnitFloat(D_0034DF38) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    scale[0] = work->head.jitterScales[3] * (random + random);
    scale[1] = scale[0];
    scale[2] = scale[0];
    VU0_LOAD_VF(vf10, lastNormal);
    VU0_LOAD_VF(vf11, scale);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, work->head.controlPoints[3]);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, point);
    slot->controlPoints[3][0] = point[0];
    slot->controlPoints[3][1] = point[1];
    slot->controlPoints[3][2] = point[2];
    func_00189C80(work->managedResource->valueWork, index);
    effMagatuhiSetValue(work->managedResource->valueWork, index, 0);
}

/* Advance delayed Bezier particles, pack their fade, and submit the owner.
 * Fade-in wins when its interval overlaps fade-out; divisors are unchecked. */
void func_0018AB40(EffMagatuhiWideSecond *work) {
    void *slots = work->mathResource;
    EffMagatuhiValueWork *valueWork = work->managedResource->valueWork;
    s32 *delays = work->delays;
    u8 respawn = work->head.respawn;
    u32 count = work->head.particleCount;
    s32 life = work->head.lifetimeFrames;
    s32 spread = work->head.delaySpread;
    s32 fadeIn = work->head.fadeIn;
    s32 fadeOut = work->head.fadeOut;
    f32 out[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 fade;
    s32 frame;
    u32 i;

    /* Retain the native slot lookup even though its returned address is unused. */
    effMathGetSlotAt(slots, 0);
    for (i = 0; i < count; i++) {
        frame = *delays;
        if (frame == 0) {
            effMagatuhiBuildBezierControlPointsVU(work, i);
        }
        if (frame > 0 && frame <= life) {
            effMathStepBezierSlot(slots, i, out);
            if (fadeIn > frame) {
                fade = (f32)frame / (f32)fadeIn;
            } else if (life - frame <= fadeOut) {
                fade = (f32)(life - frame) / (f32)fadeOut;
            } else {
                fade = 1.0f;
            }
            effMagatuhiSetValue(valueWork, i, ((u32)(fade * EFF_MAGATUHI_FADE_ALPHA_SCALE) << EFF_MAGATUHI_ALPHA_SHIFT) | EFF_MAGATUHI_NEUTRAL_RGB);
            func_00189818(valueWork, i, out);
        }
        if (frame >= life && respawn) {
            *delays = -(effMiscRand(D_0034DF38) % spread);
        } else {
            (*delays)++;
        }
        delays++;
    }
    func_001891A8(work->managedResource);
}

/* Replace control-point xyz and jitter amplitudes without changing point w. */
void effMagatuhiSetControlPointParams(EffMagatuhiCallback *work, EffMagatuhiRowsSrc *src) {
    EffMagatuhiWideSecond *dst = effGetHandlerArg(work->effect);
    u32 i;

    for (i = 0; i < EFF_MAGATUHI_CONTROL_POINT_COUNT; i++) {
        dst->head.controlPoints[i][0] = src->controlPoints[i][0];
        dst->head.controlPoints[i][1] = src->controlPoints[i][1];
        dst->head.controlPoints[i][2] = src->controlPoints[i][2];
        dst->head.jitterScales[i] = src->jitterScales[i];
    }
}

/* Seed every fourth slot with interpolated samples between two curve positions.
 * Keep the asymmetric duration/age subtraction and unchecked span divisor. */
void effMagatuhiInitializeInterpolatedHistory(EffMagatuhiCallback *arg) {
    EffMagatuhiWideSecond *work = effGetHandlerArg(arg->effect);
    EffMagatuhiValueWork *valueWork = work->managedResource->valueWork;
    u32 historyFrames = work->head.duration;
    s32 lifetimeFrames = work->head.lifetimeFrames;
    u32 count = work->head.particleCount;
    s32 *ages = work->delays;
    f32 samplePosition[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 startPosition[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 endPosition[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 blendFraction;
    f32 blendStep;
    u32 i;
    u32 j;
    s32 sampleSpan;
    EffMagatuhiSlot *slot;

    for (i = 0; i < count; i += EFF_MAGATUHI_BEZIER_GROUP_STRIDE) {
        effMagatuhiBuildBezierControlPointsVU(work, i);
        *ages = effMiscRand(D_0034DF38) % lifetimeFrames;
        slot = (EffMagatuhiSlot *)effMathGetSlotAt(work->mathResource, i);
        if (historyFrames < *ages) {
            slot->t = slot->step * (f32)(*ages - historyFrames);
            sampleSpan = historyFrames;
        } else {
            slot->t = 0.0f;
            sampleSpan = historyFrames - *ages;
        }
        blendFraction = 0.0f;
        effMathStepBezierSlot(work->mathResource, i, startPosition);
        slot->t = slot->step * (f32)*ages;
        effMathStepBezierSlot(work->mathResource, i, endPosition);
        blendStep = 1.0f / (f32)sampleSpan;
        for (j = 0; j < sampleSpan; j++) {
            VU0_LOAD_VF(vf10, startPosition);
            VU0_LOAD_VF(vf11, endPosition);
            VU0_LERP_VF10(blendFraction);
            VU0_STORE_VF(vf10, samplePosition);
            func_00189818(valueWork, i, samplePosition);
            blendFraction += blendStep;
        }
        (*ages)++;
        ages += EFF_MAGATUHI_BEZIER_GROUP_STRIDE;
    }
}

/* Drift parameters are not ring parameters despite their equal size (0xDC). */
typedef struct {
    f32 origin[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    u8 respawn;
    u8 pad11[3];
    s32 lifetimeFrames;
    s32 delaySpread;
    s32 fadeIn;
    s32 fadeOut;
    f32 initialRadius;
    f32 initialLift;
    f32 initialLiftRandomness;
    f32 angleStep;
    f32 initialScale;
    f32 scaleStep;
    f32 initialScaleRandomness;
    u32 particleCount;
    u32 maxSteps;
    u8 pad48[0x94];
} EffMagatuhiDriftParams;

/* Ring parameters copied verbatim into the work at +0x40 (0xDC). */
typedef struct {
    f32 origin[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    u8 respawn;
    u8 pad11[3];
    s32 lifetimeFrames;
    s32 delaySpread;
    s32 fadeIn;
    s32 fadeOut;
    f32 angleStep;
    f32 angleJitter;
    f32 heightSpread;
    f32 startRadius;
    f32 endRadius;
    f32 startJitter;
    f32 endJitter;
    u32 particleCount;
    u32 maxSteps;
    u8 pad48[0x94];
} EffMagatuhiRingParams;

/* Work precedes its array of 0x18-byte ring states (0x12C). */
typedef struct {
    f32 matrix[EFF_MAGATUHI_MATRIX_WORD_COUNT];
    EffMagatuhiRingParams head;
    EffMagatuhiRingParticle *particles;
    EffMagatuhiOwner *managedResource;
    u32 tintColor;
    void *allocationHandle;
} EffMagatuhiRingWork;

/* Clone ring parameters; the returned work precedes its individual slots. */
EffMagatuhiRingWork *effMagatuhiCreateRingWork(EffMagatuhiRingParams *src) {
    u32 count = src->particleCount;
    u32 handle = sdfAllocGeneralBlock(count * sizeof(EffMagatuhiRingParticle) + sizeof(EffMagatuhiRingWork));
    EffMagatuhiRingWork *work = (EffMagatuhiRingWork *)sdfResourceRetainAddress(handle);
    EffMagatuhiRingParticle *particle = (EffMagatuhiRingParticle *)(work + 1);
    s32 spread;
    u32 i;

    work->head = *src;
    work->particles = particle;
    work->tintColor = EFF_MAGATUHI_NEUTRAL_COLOR;
    work->allocationHandle = (void *)handle;
    EE_MMI_UNIT_MATRIX(work->matrix);
    if (work->head.delaySpread <= 0) {
        work->head.delaySpread = 1;
    }
    work->managedResource = effCloneMagatuhiWithColorResource(&work->head.particleCount);
    spread = work->head.delaySpread;
    for (i = 0; i < count; i++) {
        particle->age = -(effMiscRand(D_0034DF38) % spread);
        particle++;
    }
    return work;
}

/* Release the ring history owner before the work/state allocation. */
void effMagatuhiReleaseOwnerAndBuffer(EffMagatuhiRingWork *work) {
    effReleaseMagatuhiOwner(work->managedResource);
    sdfReleaseResourceAllocation(work->allocationHandle);
}

/* Initialize a ring slot and reset the history/value owned by that slot. */
void effMagatuhiInitParticleA(EffMagatuhiRingWork *work, s32 index) {
    EffMagatuhiRingParticle *elem = &work->particles[index];
    f32 blend;
    f32 t;

    elem->age = 0;
    elem->height = -work->head.heightSpread * effMiscRandUnitFloat(D_0034DF38);
    elem->angle = effMiscRandUnitFloat(D_0034DF38) * (EFF_MAGATUHI_HALF_TURN * 2.0f);
    blend = work->head.angleJitter;
    elem->angleStep = work->head.angleStep * (effMiscRandUnitFloat(D_0034DF38) * blend + (1.0f - blend));
    blend = work->head.startJitter;
    elem->radius = work->head.startRadius * (effMiscRandUnitFloat(D_0034DF38) * blend + (1.0f - blend));
    blend = work->head.endJitter;
    t = effMiscRandUnitFloat(D_0034DF38) * blend + (1.0f - blend);
    elem->radiusStep = (work->head.endRadius * t - elem->radius) / (f32)work->head.lifetimeFrames;
    func_00189C80(work->managedResource->valueWork, index);
    effMagatuhiSetValue(work->managedResource->valueWork, index, 0);
}

/* Sample the current fixed-height ring before advancing angle and radius.
 * Keep the cached age across initialization and preserve fade-in priority. */
void func_0018B348(EffMagatuhiRingWork *work) {
    f32 out[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 origin[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    EffMagatuhiValueWork *valueWork;
    EffMagatuhiRingParticle *particle;
    u8 respawn;
    u32 count;
    s32 life;
    s32 spread;
    s32 fadeIn;
    s32 fadeOut;
    u32 tintColor;
    f32 angle, radius, fade;
    s32 age;
    u32 i;

    particle = work->particles;
    count = work->head.particleCount;
    respawn = work->head.respawn;
    life = work->head.lifetimeFrames;
    spread = work->head.delaySpread;
    fadeIn = work->head.fadeIn;
    fadeOut = work->head.fadeOut;
    tintColor = work->tintColor;
    valueWork = work->managedResource->valueWork;
    PCP_COPY_VECTOR_F32(origin, work->head.origin);
    VU0_LOAD_MATRIX(work->matrix);
    for (i = 0; i < count; i++, particle++) {
        age = particle->age;
        if (age == 0) {
            effMagatuhiInitParticleA(work, i);
        }
        if (age > 0 && age <= life) {
            angle = particle->angle;
            radius = particle->radius;
            out[0] = sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
            out[1] = particle->height;
            out[2] = sdfSinPoly(angle) * radius;
            VU0_LOAD_VF(vf11, origin);
            VU0_LOAD_VF(vf10, out);
            VU0_APPLY_MATRIX(vf10, vf10);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, out);
            particle->angle += particle->angleStep;
            particle->radius += particle->radiusStep;
            if (age < fadeIn) {
                fade = (f32)age / (f32)fadeIn;
            } else if (life - age <= fadeOut) {
                fade = (f32)(life - age) / (f32)fadeOut;
            } else {
                fade = 1.0f;
            }
            effMagatuhiSetValue(valueWork, i, effMultiplyPackedColors(
                ((u32)(fade * EFF_MAGATUHI_FADE_ALPHA_SCALE) << EFF_MAGATUHI_ALPHA_SHIFT) | EFF_MAGATUHI_NEUTRAL_RGB, tintColor));
            func_00189818(valueWork, i, out);
        }
        if (age >= life && respawn) {
            particle->age = -(effMiscRand(D_0034DF38) % spread);
        } else {
            particle->age++;
        }
    }
    func_001891A8(work->managedResource);
}

/* Copy the ring origin as one quadword, including its fourth component. */
void effMagatuhiSetRingOrigin(EffMagatuhiRingWork *work, void *origin) {
    PCP_COPY_VECTOR(work->head.origin, origin);
}

/* The historical symbol names a resource, but this slot is packed color. */
void effMagatuhiSetRingColor(EffMagatuhiRingWork *work, u32 color) {
    work->tintColor = color;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effMagatuhiCopyRingMatrix(EffMagatuhiRingWork *work, void *matrix) {
    VU0_COPY_MATRIX(work->matrix, matrix);
}


extern u32 func_0018CBC0(void *block);

/* Replay the first slot of each three-slot group from a random age. */
void effMagatuhiUpdateRingFamily(EffMagatuhiCallback *arg) {
    u32 k;
    EffMagatuhiRingWork *work;
    EffMagatuhiValueWork *valueWork;
    EffMagatuhiRingParticle *particle;
    u32 count;
    s32 frames;
    u32 maxSteps;
    u32 i;
    u32 steps;
    s32 frameOffset; /* random age, then the initial state offset skipped before replay */
    f32 out[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 origin[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 height;
    f32 angle;
    f32 radius;
    f32 angleStep;
    f32 radiusStep;

    if (arg->type == EFF_MAGATUHI_CALLBACK_TYPE) {
        if (func_0018CBC0(arg->effect) == EFF_MAGATUHI_REPLAY_RING) {
            work = effGetHandlerArg(arg->effect);
            count = work->head.particleCount;
            valueWork = work->managedResource->valueWork;
            maxSteps = work->head.maxSteps;
            frames = work->head.lifetimeFrames;
            particle = work->particles;
            PCP_COPY_VECTOR(origin, work->head.origin);
            VU0_LOAD_MATRIX(work->matrix);
            for (i = 0; i < count; i += EFF_MAGATUHI_REPLAY_GROUP_STRIDE, particle += EFF_MAGATUHI_REPLAY_GROUP_STRIDE) {
                effMagatuhiInitParticleA(work, i);
                frameOffset = effMiscRand(D_0034DF38) % frames;
                particle->age = frameOffset;
                /* Keep at most maxSteps samples: skip older state only when
                 * the random age exceeds that window; otherwise start at zero. */
                if (maxSteps < frameOffset) {
                    steps = maxSteps;
                    frameOffset -= steps;
                } else {
                    steps = maxSteps - frameOffset;
                    frameOffset = 0;
                }
                angleStep = particle->angleStep;
                radiusStep = particle->radiusStep;
                height = particle->height;
                angle = particle->angle + angleStep * (f32)frameOffset;
                radius = particle->radius + radiusStep * (f32)frameOffset;
                out[3] = 0;
                for (k = 0; k < steps; k++) {
                    out[0] = sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
                    out[1] = height;
                    out[2] = sdfSinPoly(angle) * radius;
                    VU0_LOAD_VF(vf11, origin);
                    VU0_LOAD_VF(vf10, out);
                    VU0_APPLY_MATRIX(vf10, vf10);
                    VU0_ADD(vf10, vf10, vf11);
                    VU0_STORE_VF(vf10, out);
                    func_00189818(valueWork, i, out);
                    angle += angleStep;
                    radius += radiusStep;
                }
                particle->radius = radius;
                particle->angle = angle;
            }
        }
    }
}

/* Orbit parameters have an independent height step (0xE0). */
typedef struct {
    f32 origin[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    u8 respawn;
    u8 pad11[3];
    s32 lifetimeFrames;
    s32 delaySpread;
    s32 fadeIn;
    s32 fadeOut;
    f32 angleStep;
    f32 angleJitter;
    f32 heightStep;
    f32 heightJitter;
    f32 startRadius;
    f32 endRadius;
    f32 startJitter;
    f32 endJitter;
    u32 particleCount;
    u32 maxSteps;
    u8 pad4C[0x94];
} EffMagatuhiOrbitParams;

/* One 0x1C-byte orbit slot; replay visits the first slot in each triplet. */
typedef struct {
    s32 age; /* negative: waiting to start; nonnegative: elapsed frames */
    f32 height;
    f32 heightStep;
    f32 angle;
    f32 angleStep;
    f32 radius;
    f32 radiusStep;
} EffMagatuhiOrbitParticle;

/* Work precedes the orbit slots (0x130). */
typedef struct {
    f32 matrix[EFF_MAGATUHI_MATRIX_WORD_COUNT];
    EffMagatuhiOrbitParams head;
    EffMagatuhiOrbitParticle *particles;
    EffMagatuhiOwner *managedResource;
    u32 tintColor;
    void *allocationHandle;
} EffMagatuhiOrbitWork;

/* Clone orbit parameters; the returned work precedes its individual slots. */
EffMagatuhiOrbitWork *effMagatuhiCreateOrbitWork(EffMagatuhiOrbitParams *src) {
    u32 count = src->particleCount;
    u32 handle = sdfAllocGeneralBlock(count * sizeof(EffMagatuhiOrbitParticle) + sizeof(EffMagatuhiOrbitWork));
    EffMagatuhiOrbitWork *work = (EffMagatuhiOrbitWork *)sdfResourceRetainAddress(handle);
    EffMagatuhiOrbitParticle *particle = (EffMagatuhiOrbitParticle *)(work + 1);
    s32 spread;
    u32 i;

    work->head = *src;
    work->particles = particle;
    work->tintColor = EFF_MAGATUHI_NEUTRAL_COLOR;
    work->allocationHandle = (void *)handle;
    EE_MMI_UNIT_MATRIX(work->matrix);
    if (work->head.delaySpread <= 0) {
        work->head.delaySpread = 1;
    }
    work->managedResource = effCloneMagatuhiWithColorResource(&work->head.particleCount);
    spread = work->head.delaySpread;
    for (i = 0; i < count; i++) {
        particle->age = -(effMiscRand(D_0034DF38) % spread);
        particle++;
    }
    return work;
}

/* Release the orbit history owner before the work/state allocation. */
void effMagatuhiReleaseOwnerAndExtraBuffer(EffMagatuhiOrbitWork *work) {
    effReleaseMagatuhiOwner(work->managedResource);
    sdfReleaseResourceAllocation(work->allocationHandle);
}

/* Seed randomized height/angular/radial increments and clear the slot history. */
void effMagatuhiInitOrbitParticle(EffMagatuhiOrbitWork *work, s32 index) {
    EffMagatuhiOrbitParticle *elem = &work->particles[index];
    f32 blend;
    f32 t;

    elem->age = 0;
    elem->height = 0;
    blend = work->head.heightJitter;
    elem->heightStep = work->head.heightStep * (effMiscRandUnitFloat(D_0034DF38) * blend + (1.0f - blend));
    elem->angle = effMiscRandUnitFloat(D_0034DF38) * (EFF_MAGATUHI_HALF_TURN * 2.0f);
    blend = work->head.angleJitter;
    elem->angleStep = work->head.angleStep * (effMiscRandUnitFloat(D_0034DF38) * blend + (1.0f - blend));
    blend = work->head.startJitter;
    elem->radius = work->head.startRadius * (effMiscRandUnitFloat(D_0034DF38) * blend + (1.0f - blend));
    blend = work->head.endJitter;
    t = effMiscRandUnitFloat(D_0034DF38) * blend + (1.0f - blend);
    elem->radiusStep = (work->head.endRadius * t - elem->radius) / (f32)work->head.lifetimeFrames;
    func_00189C80(work->managedResource->valueWork, index);
    effMagatuhiSetValue(work->managedResource->valueWork, index, 0);
}

/* Sample and transform the current orbit before advancing its three rates.
 * Keep the cached age across initialization and preserve fade-in priority. */
void func_0018BB88(EffMagatuhiOrbitWork *work) {
    f32 out[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 origin[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    EffMagatuhiValueWork *valueWork;
    EffMagatuhiOrbitParticle *particle;
    u8 respawn;
    u32 count;
    s32 life;
    s32 spread;
    s32 fadeIn;
    s32 fadeOut;
    u32 tintColor;
    f32 angle, radius, fade;
    s32 age;
    u32 i;

    particle = work->particles;
    count = work->head.particleCount;
    respawn = work->head.respawn;
    life = work->head.lifetimeFrames;
    spread = work->head.delaySpread;
    fadeIn = work->head.fadeIn;
    fadeOut = work->head.fadeOut;
    tintColor = work->tintColor;
    valueWork = work->managedResource->valueWork;
    PCP_COPY_VECTOR_F32(origin, work->head.origin);
    VU0_LOAD_MATRIX(work->matrix);
    for (i = 0; i < count; i++, particle++) {
        age = particle->age;
        if (age == 0) {
            effMagatuhiInitOrbitParticle(work, i);
        }
        if (age > 0 && age <= life) {
            angle = particle->angle;
            radius = particle->radius;
            out[0] = sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
            out[1] = particle->height;
            out[2] = sdfSinPoly(angle) * radius;
            VU0_LOAD_VF(vf11, origin);
            VU0_LOAD_VF(vf10, out);
            VU0_APPLY_MATRIX(vf10, vf10);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, out);
            particle->height += particle->heightStep;
            particle->angle += particle->angleStep;
            particle->radius += particle->radiusStep;
            if (age < fadeIn) {
                fade = (f32)age / (f32)fadeIn;
            } else if (life - age <= fadeOut) {
                fade = (f32)(life - age) / (f32)fadeOut;
            } else {
                fade = 1.0f;
            }
            effMagatuhiSetValue(valueWork, i, effMultiplyPackedColors(
                ((u32)(fade * EFF_MAGATUHI_FADE_ALPHA_SCALE) << EFF_MAGATUHI_ALPHA_SHIFT) | EFF_MAGATUHI_NEUTRAL_RGB, tintColor));
            func_00189818(valueWork, i, out);
        }
        if (age >= life && respawn) {
            particle->age = -(effMiscRand(D_0034DF38) % spread);
        } else {
            particle->age++;
        }
    }
    func_001891A8(work->managedResource);
}

/* Copy the orbit origin as one quadword, preserving the source w. */
void effMagatuhiSetOrbitOrigin(EffMagatuhiOrbitWork *work, void *origin) {
    PCP_COPY_VECTOR(work->head.origin, origin);
}

/* This callback sets the packed orbit color, not an allocation handle. */
void effMagatuhiSetOrbitColor(EffMagatuhiOrbitWork *work, u32 color) {
    work->tintColor = color;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effMagatuhiCopyOrbitMatrix(EffMagatuhiOrbitWork *work, void *matrix) {
    VU0_COPY_MATRIX(work->matrix, matrix);
}


extern u32 func_0018CBC0(void *block);

/* Advance the orbiting-particle family: each group of three slots is replayed from a random delay, one orbit step at a time, into the value table. */
void effMagatuhiReplayOrbitStartDelays(EffMagatuhiCallback *arg) {
    u32 k;
    EffMagatuhiOrbitWork *work;
    EffMagatuhiValueWork *valueWork;
    EffMagatuhiOrbitParticle *particle;
    u32 count;
    s32 frames;
    u32 maxSteps;
    u32 i;
    u32 steps;
    s32 frameOffset; /* random age, then the initial state offset skipped before replay */
    f32 out[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 origin[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 height;
    f32 angle;
    f32 radius;
    f32 heightStep;
    f32 angleStep;
    f32 radiusStep;

    if (arg->type == EFF_MAGATUHI_CALLBACK_TYPE) {
        if (func_0018CBC0(arg->effect) == EFF_MAGATUHI_REPLAY_ORBIT) {
            work = effGetHandlerArg(arg->effect);
            count = work->head.particleCount;
            valueWork = work->managedResource->valueWork;
            maxSteps = work->head.maxSteps;
            frames = work->head.lifetimeFrames;
            particle = work->particles;
            PCP_COPY_VECTOR(origin, work->head.origin);
            VU0_LOAD_MATRIX(work->matrix);
            for (i = 0; i < count; i += EFF_MAGATUHI_REPLAY_GROUP_STRIDE, particle += EFF_MAGATUHI_REPLAY_GROUP_STRIDE) {
                effMagatuhiInitOrbitParticle(work, i);
                frameOffset = effMiscRand(D_0034DF38) % frames;
                particle->age = frameOffset;
                /* Keep at most maxSteps samples: skip older state only when
                 * the random age exceeds that window; otherwise start at zero. */
                if (maxSteps < frameOffset) {
                    steps = maxSteps;
                    frameOffset -= steps;
                } else {
                    steps = maxSteps - frameOffset;
                    frameOffset = 0;
                }
                angleStep = particle->angleStep;
                radiusStep = particle->radiusStep;
                heightStep = particle->heightStep;
                angle = particle->angle + angleStep * (f32)frameOffset;
                radius = particle->radius + radiusStep * (f32)frameOffset;
                height = particle->height + heightStep * (f32)frameOffset;
                out[3] = 0;
                for (k = 0; k < steps; k++) {
                    out[0] = sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
                    out[1] = height;
                    out[2] = sdfSinPoly(angle) * radius;
                    VU0_LOAD_VF(vf11, origin);
                    VU0_LOAD_VF(vf10, out);
                    VU0_APPLY_MATRIX(vf10, vf10);
                    VU0_ADD(vf10, vf10, vf11);
                    VU0_STORE_VF(vf10, out);
                    func_00189818(valueWork, i, out);
                    height += heightStep;
                    angle += angleStep;
                    radius += radiusStep;
                }
                particle->height = height;
                particle->radius = radius;
                particle->angle = angle;
            }
        }
    }
}

/* The 0x30-byte states precede this 0x12C-byte work allocation. */
typedef struct {
    f32 matrix[EFF_MAGATUHI_MATRIX_WORD_COUNT];
    EffMagatuhiDriftParams head;
    EffMagatuhiDriftParticle *particles;
    u32 tintColor;
    EffMagatuhiOwner *managedResource;
    void *allocationHandle;
} EffMagatuhiDriftWork;

/* Clone drift parameters; return the work after its 0x30-byte state array. */
EffMagatuhiDriftWork *effMagatuhiCreateDriftWork(EffMagatuhiDriftParams *src) {
    u32 count = src->particleCount;
    u32 size = count * sizeof(EffMagatuhiDriftParticle);
    u32 handle = sdfAllocGeneralBlock(size + sizeof(EffMagatuhiDriftWork));
    EffMagatuhiDriftParticle *particle = (EffMagatuhiDriftParticle *)sdfResourceRetainAddress(handle);
    EffMagatuhiDriftWork *work = (EffMagatuhiDriftWork *)((u8 *)particle + size);
    s32 spread;
    u32 i;

    work->head = *src;
    work->particles = particle;
    work->tintColor = EFF_MAGATUHI_NEUTRAL_COLOR;
    work->allocationHandle = (void *)handle;
    EE_MMI_UNIT_MATRIX(work->matrix);
    if (work->head.delaySpread <= 0) {
        work->head.delaySpread = 1;
    }
    work->managedResource = effCloneMagatuhiWithColorResource(&work->head.particleCount);
    spread = work->head.delaySpread;
    for (i = 0; i < count; i++) {
        particle->age = -(effMiscRand(D_0034DF38) % spread);
        particle++;
    }
    return work;
}

/* Release the drift history owner before its states-plus-work allocation. */
void effMagatuhiReleaseSecondaryOwnerAndBuffer(EffMagatuhiDriftWork *work) {
    effReleaseMagatuhiOwner(work->managedResource);
    sdfReleaseResourceAllocation(work->allocationHandle);
}

/* vu0 routine: initialize a planar particle with normalized position/drift. */
void effMagatuhiInitializeDriftParticle(EffMagatuhiDriftWork *work, s32 index) {
    EffMagatuhiDriftParticle *particle = &work->particles[index];
    f32 direction[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 radius;
    f32 random;

    random = effMiscRandUnitFloat(D_0034DF38) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    radius = work->head.initialRadius * (random + random);
    random = effMiscRandUnitFloat(D_0034DF38) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    direction[0] = random + random;
    direction[1] = 0.0f;
    random = effMiscRandUnitFloat(D_0034DF38) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    direction[2] = random + random;
    VU0_LOAD_VF_FROM(vf10, *(u128 *)direction);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF_TO_MEMORY(vf10, *(u128 *)direction);
    particle->position[0] = radius * direction[0];
    particle->position[1] = 0.0f;
    particle->position[2] = radius * direction[2];
    random = effMiscRandUnitFloat(D_0034DF38) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    particle->driftState[0] = random + random;
    particle->driftState[1] = 0.0f;
    random = effMiscRandUnitFloat(D_0034DF38) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    particle->driftState[2] = random + random;
    VU0_LOAD_VF(vf10, particle->driftState);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, particle->driftState);
    particle->scale = work->head.initialScale *
        (effMiscRandUnitFloat(D_0034DF38) * work->head.initialScaleRandomness +
         (1.0f - work->head.initialScaleRandomness));
    particle->angle = effMiscRandUnitFloat(D_0034DF38) * EFF_MAGATUHI_FULL_TURN;
    particle->liftStep = work->head.initialLift *
        (effMiscRandUnitFloat(D_0034DF38) * work->head.initialLiftRandomness +
         (1.0f - work->head.initialLiftRandomness));
    func_00189C80(work->managedResource->valueWork, index);
    effMagatuhiSetValue(work->managedResource->valueWork, index, 0);
}

/* Accumulate lift, transform the drift sample, then advance scale and angle.
 * Keep the cached age across initialization and preserve fade-in priority. */
void func_0018C4C8(EffMagatuhiDriftWork *work) {
    f32 out[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 origin[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    EffMagatuhiValueWork *valueWork;
    EffMagatuhiDriftParticle *particle;
    u8 respawn;
    u32 count;
    s32 life;
    s32 spread;
    s32 fadeIn;
    s32 fadeOut;
    u32 tintColor;
    f32 radial, fade;
    f32 angleStep, scaleStep;
    s32 age;
    u32 i;

    particle = work->particles;
    tintColor = work->tintColor;
    count = work->head.particleCount;
    respawn = work->head.respawn;
    life = work->head.lifetimeFrames;
    spread = work->head.delaySpread;
    angleStep = work->head.angleStep;
    scaleStep = work->head.scaleStep;
    fadeIn = work->head.fadeIn;
    fadeOut = work->head.fadeOut;
    valueWork = work->managedResource->valueWork;
    PCP_COPY_VECTOR_F32(origin, work->head.origin);
    VU0_LOAD_MATRIX(work->matrix);
    for (i = 0; i < count; i++, particle++) {
        age = particle->age;
        if (age == 0) {
            effMagatuhiInitializeDriftParticle(work, i);
        }
        if (age > 0 && age <= life) {
            radial = particle->scale * sdfSinPoly(particle->angle);
            particle->driftState[1] += particle->liftStep;
            out[0] = particle->position[0] + particle->driftState[0] * radial;
            out[1] = particle->position[1] + particle->driftState[1];
            out[2] = particle->position[2] + particle->driftState[2] * radial;
            VU0_LOAD_VF(vf11, origin);
            VU0_LOAD_VF(vf10, out);
            VU0_APPLY_MATRIX(vf10, vf10);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, out);
            particle->scale += scaleStep;
            particle->angle += angleStep;
            if (age < fadeIn) {
                fade = (f32)age / (f32)fadeIn;
            } else if (life - age <= fadeOut) {
                fade = (f32)(life - age) / (f32)fadeOut;
            } else {
                fade = 1.0f;
            }
            effMagatuhiSetValue(valueWork, i, effMultiplyPackedColors(
                ((u32)(fade * EFF_MAGATUHI_FADE_ALPHA_SCALE) << EFF_MAGATUHI_ALPHA_SHIFT) | EFF_MAGATUHI_NEUTRAL_RGB, tintColor));
            func_00189818(valueWork, i, out);
        }
        if (age >= life && respawn) {
            particle->age = -(effMiscRand(D_0034DF38) % spread);
        } else {
            particle->age++;
        }
    }
    func_001891A8(work->managedResource);
}

/* Copy the drift origin as one quadword, preserving the source w. */
void effMagatuhiSetDriftOrigin(EffMagatuhiDriftWork *work, void *origin) {
    PCP_COPY_VECTOR(work->head.origin, origin);
}

/* This callback sets packed drift color; +0x120 is not an owner pointer. */
void effMagatuhiSetDriftColor(EffMagatuhiDriftWork *work, u32 color) {
    work->tintColor = color;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effMagatuhiCopyDriftMatrix(EffMagatuhiDriftWork *work, void *matrix) {
    VU0_COPY_MATRIX(work->matrix, matrix);
}


/* Replay the first slot of each three-slot drift group from a random age. */
void effMagatuhiUpdateDriftFamily(EffMagatuhiCallback *arg) {
    u32 k;
    EffMagatuhiDriftWork *work;
    EffMagatuhiValueWork *valueWork;
    EffMagatuhiDriftParticle *particle;
    u32 count;
    s32 frames;
    u32 maxSteps;
    u32 i;
    u32 steps;
    s32 frameOffset; /* random age, then the initial state offset skipped before replay */
    f32 out[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 origin[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 lift;
    f32 angle;
    f32 scale;
    f32 liftStep;
    f32 angleStep;
    f32 scaleStep;

    if (arg->type == EFF_MAGATUHI_CALLBACK_TYPE) {
        if (func_0018CBC0(arg->effect) == EFF_MAGATUHI_REPLAY_DRIFT) {
            work = effGetHandlerArg(arg->effect);
            count = work->head.particleCount;
            valueWork = work->managedResource->valueWork;
            maxSteps = work->head.maxSteps;
            frames = work->head.lifetimeFrames;
            particle = work->particles;
            PCP_COPY_VECTOR(origin, work->head.origin);
            VU0_LOAD_MATRIX(work->matrix);
            for (i = 0; i < count; i += EFF_MAGATUHI_REPLAY_GROUP_STRIDE, particle += EFF_MAGATUHI_REPLAY_GROUP_STRIDE) {
                effMagatuhiInitializeDriftParticle(work, i);
                frameOffset = effMiscRand(D_0034DF38) % frames;
                particle->age = frameOffset;
                /* Keep at most maxSteps samples: skip older state only when
                 * the random age exceeds that window; otherwise start at zero. */
                if (maxSteps < frameOffset) {
                    steps = maxSteps;
                    frameOffset -= steps;
                } else {
                    steps = maxSteps - frameOffset;
                    frameOffset = 0;
                }
                angleStep = work->head.angleStep;
                scaleStep = work->head.scaleStep;
                liftStep = particle->liftStep;
                angle = particle->angle + angleStep * (f32)frameOffset;
                scale = particle->scale + scaleStep * (f32)frameOffset;
                lift = particle->driftState[1] + liftStep * (f32)frameOffset;
                out[3] = 0;
                for (k = 0; k < steps; k++) {
                    /* Native ordering: lift advances before sampling, scale/angle after. */
                    lift += liftStep;
                    /* Keep the sine evaluation even though its result is unused here. */
                    sdfSinPoly(angle);
                    out[0] = particle->position[0] + particle->driftState[0] * scale;
                    out[1] = particle->position[1] + lift;
                    out[2] = particle->position[2] + particle->driftState[2] * scale;
                    VU0_LOAD_VF(vf11, origin);
                    VU0_LOAD_VF(vf10, out);
                    VU0_APPLY_MATRIX(vf10, vf10);
                    VU0_ADD(vf10, vf10, vf11);
                    VU0_STORE_VF(vf10, out);
                    func_00189818(valueWork, i, out);
                    scale += scaleStep;
                    angle += angleStep;
                }
                particle->scale = scale;
                particle->angle = angle;
                particle->driftState[1] = lift;
            }
        }
    }
}

extern u32 func_0018CBC0(void *block);

/* Dispatch replay side effects; retain the native s32 definition's fall-through. */
s32 effMagatuhiDispatchByKind(EffMagatuhiCallback *work) {
    if (work->type == EFF_MAGATUHI_CALLBACK_TYPE) {
        switch (func_0018CBC0(work->effect)) {
        case EFF_MAGATUHI_REPLAY_NONE:
            break;
        case EFF_MAGATUHI_REPLAY_BEZIER:
            effMagatuhiInitializeInterpolatedHistory(work);
            break;
        case EFF_MAGATUHI_REPLAY_RING:
            effMagatuhiUpdateRingFamily(work);
            break;
        case EFF_MAGATUHI_REPLAY_ORBIT:
            effMagatuhiReplayOrbitStartDelays(work);
            break;
        case EFF_MAGATUHI_REPLAY_DRIFT:
            effMagatuhiUpdateDriftFamily(work);
            break;
        }
    }
}
