#include "common.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

/* Render/history buffer, created by the Magatuhi owner factory (0x38).
 * This is not the callback input: its +8 word is a float, not an effect pointer. */
typedef struct {
    s32 count;
    u16 historyCount;
    u8 pad06[2];
    f32 unk08;
    f32 unk0C;
    u32 unk10;
    f32 (*positions)[4];
    u32 *colorTable;
    f32 *unk1C;
    u32 *values;
    u16 *writeIndices;
    u16 *validCounts;
    f32 (*unk2C)[4];
    u32 texture;
    void *resource;
} EffMagatuhiValueWork;

/* Callback input prefix; type 3 contains an effect-dispatch object at +8. */
typedef struct {
    s32 type;
    u8 pad04[4];
    void *effect;
} EffMagatuhiCallback;

extern void *effGetHandlerArg(void *arg);
extern u32 effBlendColor(u32 colorA, u32 colorB, f32 t);

/* Four rows; the copy updates only xyz and preserves each destination w. */
typedef struct {
    f32 row[4][4];
    f32 extra[4];
} EffMagatuhiRowsSrc;




typedef struct EffMagatuhiOwner EffMagatuhiOwner;

/* Parameter head of the first family: particles of 0x30 bytes precede the work. */
typedef struct {
    f32 unk00, unk04, unk08;
    u8 pad0C[4];
    f32 unk10, unk14, unk18;
    u8 pad1C[0xC];
    s32 spread;            /* 0x28 modulus of the particle delay */
    u8 pad2C[8];
    f32 unk34;
    u8 pad38[0x18];
    f32 unk50;
    u8 pad54[0xC];
    u32 count;             /* 0x60 */
    u8 pad64[0x118];
} EffMagatuhiHeadFirst; /* 0x17C */

/* A 0x30-byte position/direction state, shared by the first and drift families. */
typedef struct {
    f32 pos[3];
    u8 pad0C[4];
    f32 dir[3];
    u8 pad1C[4];
    s32 delay;
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
    void *buffer;          /* 0x190 */
} EffMagatuhiWideFirst;

/* Parameter head of the second family: `count` particles, delay spread at 0x48. */
typedef struct {
    f32 row[4][4];
    u8 respawn;            /* 0x40 restart finished particles */
    u8 pad41[3];
    s32 life;              /* 0x44 frames a particle lives */
    s32 spread;            /* 0x48 modulus of the particle delay */
    s32 fadeIn;            /* 0x4C */
    s32 fadeOut;           /* 0x50 */
    f32 extra[4];          /* 0x54 */
    u32 count;             /* 0x64 */
    u32 duration;          /* 0x68 */
    u8 pad6C[0x114];
} EffMagatuhiHeadSecond; /* 0x180 */

typedef struct {
    EffMagatuhiHeadSecond head;
    s32 *delays;           /* 0x180 */
    void *mathResource;    /* 0x184 */
    EffMagatuhiOwner *managedResource; /* 0x188 */
    void *buffer;          /* 0x18C */
} EffMagatuhiWideSecond;

/* Float source block read by effMagatuhiCopyFloatBlock. */
typedef struct {
    f32 unk00, unk04, unk08;
    u8 pad0C[4];
    f32 unk10, unk14, unk18;
    u8 pad1C[4];
    f32 unk20, unk24;
} EffMagatuhiFloatParams; /* 0x28 */


void effMagatuhiReleaseResource(EffMagatuhiValueWork *work) {
    sdfReleaseResourceAllocation(work->resource);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_001893D8);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00189818);

void effMagatuhiSetValue(EffMagatuhiValueWork *work, s32 index, u32 value) {
    work->values[index] = value;
}

/* Fill the history palette; the final sample is (count - 1) / count, not 1. */
void effMagatuhiFillColorTable(EffMagatuhiValueWork *work, u32 colorA, u32 colorB) {
    f32 t = 0.0f;
    u32 count = work->historyCount;
    u32 *out = work->colorTable;
    f32 step = 1.0f / count;
    u32 i;

    for (i = 0; i < count; i++) {
        *out++ = effBlendColor(colorA, colorB, t);
        t += step;
    }
}

extern f32 effMiscRandUnitFloat(void *state);
extern u8 D_0034DF38[];
extern void *effMathGetSlotAt(void *slots, s32 index);

/* Owner of the value table an effect variant's particles write through. */
struct EffMagatuhiOwner {
    u8 pad00[0x1C];
    EffMagatuhiValueWork *valueWork; /* 0x1C */
};

void func_00189C80(void *valueWork, s32 index) {
    func_001891C0(valueWork, index);
}

/* Ring state is one 0x18-byte slot; replay visits every third slot. */
typedef struct {
    s32 delay;
    f32 height;
    f32 angle;
    f32 angleStep;
    f32 radius;
    f32 radiusStep;
} EffMagatuhiRingParticle;


extern u32 sdfAllocGeneralBlock(s32 size);
extern u8 *sdfResourceRetainAddress(u32 handle);
extern void *effCloneMagatuhiWithColorResource(void *block);
extern void *effAllocSlotArray(s32 count);
extern u32 effMiscRand(void *state);
extern u8 D_0034DF38[];

/* Clone the first-family parameters; return the work after its state array.
 * Clamp only the copied delay modulus, leaving the caller's parameters intact. */
EffMagatuhiWideFirst *effMagatuhiCreateFirst(EffMagatuhiHeadFirst *src) {
    u32 count = src->count;
    u32 size = count * sizeof(EffMagatuhiDriftParticle);
    u32 handle = sdfAllocGeneralBlock(size + sizeof(EffMagatuhiWideFirst));
    EffMagatuhiDriftParticle *particle = (EffMagatuhiDriftParticle *)sdfResourceRetainAddress(handle);
    EffMagatuhiWideFirst *work = (EffMagatuhiWideFirst *)((u8 *)particle + size);
    s32 spread;
    u32 i;

    work->head = *src;
    work->buffer = (void *)handle;
    work->particles = particle;
    if (work->head.spread <= 0) {
        work->head.spread = 1;
    }
    work->managedResource = effCloneMagatuhiWithColorResource(&work->head.count);
    work->mathResource = effAllocSlotArray(count);
    spread = work->head.spread;
    for (i = 0; i < count; i++) {
        particle->delay = -(effMiscRand(D_0034DF38) % spread);
        particle++;
    }
    return work;
}

void effMagatuhiReleaseMathOwnerAndBuffer(EffMagatuhiWideFirst *work) {
    effMathReleaseWorkResource(work->mathResource);
    effReleaseMagatuhiOwner(work->managedResource);
    sdfReleaseResourceAllocation(work->buffer);
}

void func_00189E98(EffMagatuhiWideFirst *work, s32 index) {
    EffMagatuhiDriftParticle *particle = &work->particles[index];
    f32 direction[4];
    f32 radius;
    f32 random;
    void *slot;

    random = effMiscRandUnitFloat(D_0034DF38) - 0.5f;
    radius = work->head.unk34 * (random + random);
    random = effMiscRandUnitFloat(D_0034DF38) - 0.5f;
    direction[0] = random + random;
    direction[1] = 0.0f;
    random = effMiscRandUnitFloat(D_0034DF38) - 0.5f;
    direction[2] = random + random;
    VU0_LOAD_VF_FROM(vf10, *(u128 *)direction);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF_TO_MEMORY(vf10, *(u128 *)direction);
    particle->pos[0] = radius * direction[0];
    particle->pos[1] = 0.0f;
    particle->pos[2] = radius * direction[2];
    random = effMiscRandUnitFloat(D_0034DF38) - 0.5f;
    particle->dir[0] = random + random;
    particle->dir[1] = 0.0f;
    random = effMiscRandUnitFloat(D_0034DF38) - 0.5f;
    particle->dir[2] = random + random;
    VU0_LOAD_VF(vf10, particle->dir);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, particle->dir);
    particle->scale = *(f32 *)((u8 *)work + 0x44) *
        (effMiscRandUnitFloat(D_0034DF38) * *(f32 *)((u8 *)work + 0x4C) +
         (1.0f - *(f32 *)((u8 *)work + 0x4C)));
    particle->angle = effMiscRandUnitFloat(D_0034DF38) * 6.2831853f;
    particle->liftStep = *(f32 *)((u8 *)work + 0x38) *
        (effMiscRandUnitFloat(D_0034DF38) * *(f32 *)((u8 *)work + 0x3C) +
         (1.0f - *(f32 *)((u8 *)work + 0x3C)));
    slot = effMathGetSlotAt(work->mathResource, index);
    *(s32 *)((u8 *)slot + 0x30) = 0;
    *(s32 *)((u8 *)slot + 0x34) = 0;
    func_00189C80(work->managedResource->valueWork, index);
    effMagatuhiSetValue(work->managedResource->valueWork, index, 0);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018A098);

/* Update the first family's scattered float parameters without touching gaps. */
void effMagatuhiCopyFloatBlock(EffMagatuhiCallback *work, EffMagatuhiFloatParams *src) {
    EffMagatuhiWideFirst *dst = effGetHandlerArg(work->effect);

    dst->head.unk00 = src->unk00;
    dst->head.unk04 = src->unk04;
    dst->head.unk08 = src->unk08;
    dst->head.unk34 = src->unk20;
    dst->head.unk10 = src->unk10;
    dst->head.unk14 = src->unk14;
    dst->head.unk18 = src->unk18;
    dst->head.unk50 = src->unk24;
}

/* Clone history parameters; signed delays follow the returned work block. */
EffMagatuhiWideSecond *effMagatuhiCreateSecond(EffMagatuhiHeadSecond *src) {
    u32 count = src->count;
    u32 handle = sdfAllocGeneralBlock(count * 4 + sizeof(EffMagatuhiWideSecond));
    EffMagatuhiWideSecond *work = (EffMagatuhiWideSecond *)sdfResourceRetainAddress(handle);
    s32 *delays = (s32 *)(work + 1);
    s32 spread;
    u32 i;

    work->head = *src;
    work->buffer = (void *)handle;
    work->delays = delays;
    if (work->head.spread <= 0) {
        work->head.spread = 1;
    }
    work->managedResource = effCloneMagatuhiWithColorResource(&work->head.count);
    work->mathResource = effAllocSlotArray(count);
    spread = work->head.spread;
    for (i = 0; i < count; i++) {
        *delays++ = -(effMiscRand(D_0034DF38) % spread);
    }
    return work;
}

void effMagatuhiReleaseWideWorkResources(EffMagatuhiWideSecond *work) {
    effMathReleaseWorkResource(work->mathResource);
    effReleaseMagatuhiOwner(work->managedResource);
    sdfReleaseResourceAllocation(work->buffer);
}

extern s32 effMathStepBezierSlot(void *slots, s32 index, void *out);
extern void func_001891A8(void *owner);
extern void func_00189818(void *valueWork, s32 index, void *out);

typedef struct {
    f32 controlPoints[4][3];
    f32 scale; /* 0x30 */
    f32 base;  /* 0x34 */
} EffMagatuhiSlot;

extern f32 sdfViewTargetVector[4];
extern f32 sdfViewEyeVector[4];

/* Jitter control points perpendicular to the path and camera viewing direction. */
void effMagatuhiBuildBezierControlPointsVU(EffMagatuhiWideSecond *work, s32 index) {
    f32 scale[4];
    f32 lastNormal[4];
    f32 viewDirection[4];
    f32 point[4];
    EffMagatuhiSlot *slot;
    f32 step;
    f32 random;

    VU0_LOAD_VF(vf10, sdfViewTargetVector);
    VU0_LOAD_VF(vf11, sdfViewEyeVector);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, viewDirection);
    step = 1.0f / (f32)work->head.life;
    slot = effMathGetSlotAt(work->mathResource, index);
    slot->scale = 0;
    slot->base = step;

    random = effMiscRandUnitFloat(D_0034DF38) - 0.5f;
    scale[0] = work->head.extra[0] * (random + random);
    scale[1] = scale[0];
    scale[2] = scale[0];
    VU0_LOAD_VF(vf10, work->head.row[0]);
    VU0_LOAD_VF(vf11, work->head.row[1]);
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

    random = effMiscRandUnitFloat(D_0034DF38) - 0.5f;
    scale[0] = work->head.extra[1] * (random + random);
    scale[1] = scale[0];
    scale[2] = scale[0];
    VU0_LOAD_VF(vf10, work->head.row[1]);
    VU0_LOAD_VF(vf11, work->head.row[2]);
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

    random = effMiscRandUnitFloat(D_0034DF38) - 0.5f;
    scale[0] = work->head.extra[2] * (random + random);
    scale[1] = scale[0];
    scale[2] = scale[0];
    VU0_LOAD_VF(vf10, work->head.row[2]);
    VU0_LOAD_VF(vf11, work->head.row[3]);
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

    random = effMiscRandUnitFloat(D_0034DF38) - 0.5f;
    scale[0] = work->head.extra[3] * (random + random);
    scale[1] = scale[0];
    scale[2] = scale[0];
    VU0_LOAD_VF(vf10, lastNormal);
    VU0_LOAD_VF(vf11, scale);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, work->head.row[3]);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, point);
    slot->controlPoints[3][0] = point[0];
    slot->controlPoints[3][1] = point[1];
    slot->controlPoints[3][2] = point[2];
    func_00189C80(work->managedResource->valueWork, index);
    effMagatuhiSetValue(work->managedResource->valueWork, index, 0);
}

void func_0018AB40(EffMagatuhiWideSecond *work) {
    void *slots = work->mathResource;
    EffMagatuhiValueWork *valueWork = work->managedResource->valueWork;
    s32 *delays = work->delays;
    u8 respawn = work->head.respawn;
    u32 count = work->head.count;
    s32 life = work->head.life;
    s32 spread = work->head.spread;
    s32 fadeIn = work->head.fadeIn;
    s32 fadeOut = work->head.fadeOut;
    f32 out[4];
    f32 fade;
    s32 frame;
    u32 i;

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
            effMagatuhiSetValue(valueWork, i, ((u32)(fade * 127.0f) << 24) | 0x808080);
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

void effMagatuhiCopyHandlerRows(EffMagatuhiCallback *work, EffMagatuhiRowsSrc *src) {
    EffMagatuhiWideSecond *dst = effGetHandlerArg(work->effect);
    u32 i;

    for (i = 0; i < 4; i++) {
        dst->head.row[i][0] = src->row[i][0];
        dst->head.row[i][1] = src->row[i][1];
        dst->head.row[i][2] = src->row[i][2];
        dst->head.extra[i] = src->extra[i];
    }
}

void effMagatuhiInitializeInterpolatedHistory(EffMagatuhiCallback *arg) {
    EffMagatuhiWideSecond *work = effGetHandlerArg(arg->effect);
    EffMagatuhiValueWork *valueWork = work->managedResource->valueWork;
    u32 duration = work->head.duration;
    s32 life = work->head.life;
    u32 count = work->head.count;
    s32 *delays = work->delays;
    f32 out[4];
    f32 from[4];
    f32 to[4];
    f32 t;
    f32 step;
    u32 i;
    u32 j;
    s32 span;
    EffMagatuhiSlot *slot;

    for (i = 0; i < count; i += 4) {
        effMagatuhiBuildBezierControlPointsVU(work, i);
        *delays = effMiscRand(D_0034DF38) % life;
        slot = effMathGetSlotAt(work->mathResource, i);
        if (duration < *delays) {
            slot->scale = slot->base * (f32)(*delays - duration);
            span = duration;
        } else {
            slot->scale = 0.0f;
            span = duration - *delays;
        }
        t = 0.0f;
        effMathStepBezierSlot(work->mathResource, i, from);
        slot->scale = slot->base * (f32)*delays;
        effMathStepBezierSlot(work->mathResource, i, to);
        step = 1.0f / (f32)span;
        for (j = 0; j < span; j++) {
            VU0_LOAD_VF(vf10, from);
            VU0_LOAD_VF(vf11, to);
            VU0_LERP_VF10(t);
            VU0_STORE_VF(vf10, out);
            func_00189818(valueWork, i, out);
            t += step;
        }
        (*delays)++;
        delays += 4;
    }
}

/* Drift parameters are not ring parameters despite their equal size (0xDC). */
typedef struct {
    f32 origin[4];
    u8 pad10[4];
    s32 frames;
    s32 spread;
    u8 pad1C[8];
    f32 initialRadius;
    f32 initialLift;
    f32 initialLiftRandomness;
    f32 angleStep;
    f32 initialScale;
    f32 scaleStep;
    f32 initialScaleRandomness;
    u32 count;
    u32 maxSteps;
    u8 pad48[0x94];
} EffMagatuhiDriftParams;

/* Ring parameters copied verbatim into the work at +0x40 (0xDC). */
typedef struct {
    f32 origin[4];
    u8 pad10[4];
    s32 frames;
    s32 spread;
    u8 pad1C[8];
    f32 angleStep;
    f32 angleJitter;
    f32 heightSpread;
    f32 startRadius;
    f32 endRadius;
    f32 startJitter;
    f32 endJitter;
    u32 count;
    u32 maxSteps;
    u8 pad48[0x94];
} EffMagatuhiRingParams;

/* Work precedes its array of 0x18-byte ring states (0x12C). */
typedef struct {
    f32 matrix[16];
    EffMagatuhiRingParams head;
    EffMagatuhiRingParticle *particles;
    EffMagatuhiOwner *managedResource;
    u32 color;
    void *buffer;
} EffMagatuhiRingWork;

/* Clone ring parameters; the returned work precedes its individual slots. */
EffMagatuhiRingWork *effMagatuhiCreateFourth(EffMagatuhiRingParams *src) {
    u32 count = src->count;
    u32 handle = sdfAllocGeneralBlock(count * sizeof(EffMagatuhiRingParticle) + sizeof(EffMagatuhiRingWork));
    EffMagatuhiRingWork *work = (EffMagatuhiRingWork *)sdfResourceRetainAddress(handle);
    EffMagatuhiRingParticle *particle = (EffMagatuhiRingParticle *)(work + 1);
    s32 spread;
    u32 i;

    work->head = *src;
    work->particles = particle;
    work->color = 0x80808080;
    work->buffer = (void *)handle;
    EE_MMI_UNIT_MATRIX(work->matrix);
    if (work->head.spread <= 0) {
        work->head.spread = 1;
    }
    work->managedResource = effCloneMagatuhiWithColorResource(&work->head.count);
    spread = work->head.spread;
    for (i = 0; i < count; i++) {
        particle->delay = -(effMiscRand(D_0034DF38) % spread);
        particle++;
    }
    return work;
}

void effMagatuhiReleaseOwnerAndBuffer(EffMagatuhiRingWork *work) {
    effReleaseMagatuhiOwner(work->managedResource);
    sdfReleaseResourceAllocation(work->buffer);
}

/* Initialize a ring slot and reset the history/value owned by that slot. */
void effMagatuhiInitParticleA(EffMagatuhiRingWork *work, s32 index) {
    EffMagatuhiRingParticle *elem = &work->particles[index];
    f32 blend;
    f32 t;

    elem->delay = 0;
    elem->height = -work->head.heightSpread * effMiscRandUnitFloat(D_0034DF38);
    elem->angle = effMiscRandUnitFloat(D_0034DF38) * (3.14159265f * 2.0f);
    blend = work->head.angleJitter;
    elem->angleStep = work->head.angleStep * (effMiscRandUnitFloat(D_0034DF38) * blend + (1.0f - blend));
    blend = work->head.startJitter;
    elem->radius = work->head.startRadius * (effMiscRandUnitFloat(D_0034DF38) * blend + (1.0f - blend));
    blend = work->head.endJitter;
    t = effMiscRandUnitFloat(D_0034DF38) * blend + (1.0f - blend);
    elem->radiusStep = (work->head.endRadius * t - elem->radius) / (f32)work->head.frames;
    func_00189C80(work->managedResource->valueWork, index);
    effMagatuhiSetValue(work->managedResource->valueWork, index, 0);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018B348);

void effMagatuhiCopyWorkVector(EffMagatuhiRingWork *work, void *src) {
    PCP_COPY_VECTOR(work->head.origin, src);
}

/* The historical symbol names a resource, but this slot is packed color. */
void effMagatuhiSetSecondResource(EffMagatuhiRingWork *work, u32 value) {
    work->color = value;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effMagatuhiCopyVecs(EffMagatuhiRingWork *dst, void *src) {
    VU0_COPY_MATRIX(dst->matrix, src);
}


extern u32 func_0018CBC0(void *block);
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32 angle);
extern f32 sdfSinPoly(f32 angle);

/* Replay the first slot of each three-slot group from a random age. */
void effMagatuhiInitRingParticles(EffMagatuhiCallback *arg) {
    u32 k;
    EffMagatuhiRingWork *work;
    EffMagatuhiValueWork *valueWork;
    EffMagatuhiRingParticle *particle;
    u32 count;
    s32 frames;
    u32 maxSteps;
    u32 i;
    u32 steps;
    s32 delay; /* random start delay; afterwards the part of it that is replayed */
    f32 out[4];
    f32 origin[4];
    f32 height;
    f32 angle;
    f32 radius;
    f32 angleStep;
    f32 radiusStep;

    if (arg->type == 3) {
        if (func_0018CBC0(arg->effect) == 2) {
            work = effGetHandlerArg(arg->effect);
            count = work->head.count;
            valueWork = work->managedResource->valueWork;
            maxSteps = work->head.maxSteps;
            frames = work->head.frames;
            particle = work->particles;
            PCP_COPY_VECTOR(origin, work->head.origin);
            VU0_LOAD_MATRIX(work->matrix);
            for (i = 0; i < count; i += 3, particle += 3) {
                effMagatuhiInitParticleA(work, i);
                delay = effMiscRand(D_0034DF38) % frames;
                particle->delay = delay;
                /* Keep at most maxSteps samples: skip older state only when
                 * the random age exceeds that window; otherwise start at zero. */
                if (maxSteps < delay) {
                    steps = maxSteps;
                    delay -= steps;
                } else {
                    steps = maxSteps - delay;
                    delay = 0;
                }
                angleStep = particle->angleStep;
                radiusStep = particle->radiusStep;
                height = particle->height;
                angle = particle->angle + angleStep * (f32)delay;
                radius = particle->radius + radiusStep * (f32)delay;
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
    f32 origin[4];
    u8 pad10[4];
    s32 frames;
    s32 spread;
    u8 pad1C[8];
    f32 angleStep;
    f32 angleJitter;
    f32 heightStep;
    f32 heightJitter;
    f32 startRadius;
    f32 endRadius;
    f32 startJitter;
    f32 endJitter;
    u32 count;
    u32 maxSteps;
    u8 pad4C[0x94];
} EffMagatuhiOrbitParams;

/* One 0x1C-byte orbit slot; replay visits the first slot in each triplet. */
typedef struct {
    s32 delay;
    f32 height;
    f32 heightStep;
    f32 angle;
    f32 angleStep;
    f32 radius;
    f32 radiusStep;
} EffMagatuhiOrbitParticle;

/* Work precedes the orbit slots (0x130). */
typedef struct {
    f32 matrix[16];
    EffMagatuhiOrbitParams head;
    EffMagatuhiOrbitParticle *particles;
    EffMagatuhiOwner *managedResource;
    u32 color;
    void *buffer;
} EffMagatuhiOrbitWork;

/* Clone orbit parameters; the returned work precedes its individual slots. */
EffMagatuhiOrbitWork *effMagatuhiCreateFifth(EffMagatuhiOrbitParams *src) {
    u32 count = src->count;
    u32 handle = sdfAllocGeneralBlock(count * sizeof(EffMagatuhiOrbitParticle) + sizeof(EffMagatuhiOrbitWork));
    EffMagatuhiOrbitWork *work = (EffMagatuhiOrbitWork *)sdfResourceRetainAddress(handle);
    EffMagatuhiOrbitParticle *particle = (EffMagatuhiOrbitParticle *)(work + 1);
    s32 spread;
    u32 i;

    work->head = *src;
    work->particles = particle;
    work->color = 0x80808080;
    work->buffer = (void *)handle;
    EE_MMI_UNIT_MATRIX(work->matrix);
    if (work->head.spread <= 0) {
        work->head.spread = 1;
    }
    work->managedResource = effCloneMagatuhiWithColorResource(&work->head.count);
    spread = work->head.spread;
    for (i = 0; i < count; i++) {
        particle->delay = -(effMiscRand(D_0034DF38) % spread);
        particle++;
    }
    return work;
}

void effMagatuhiReleaseOwnerAndExtraBuffer(EffMagatuhiOrbitWork *work) {
    effReleaseMagatuhiOwner(work->managedResource);
    sdfReleaseResourceAllocation(work->buffer);
}

void func_0018BA30(EffMagatuhiOrbitWork *work, s32 index) {
    EffMagatuhiOrbitParticle *elem = &work->particles[index];
    f32 blend;
    f32 t;

    elem->delay = 0;
    elem->height = 0;
    blend = work->head.heightJitter;
    elem->heightStep = work->head.heightStep * (effMiscRandUnitFloat(D_0034DF38) * blend + (1.0f - blend));
    elem->angle = effMiscRandUnitFloat(D_0034DF38) * (3.14159265f * 2.0f);
    blend = work->head.angleJitter;
    elem->angleStep = work->head.angleStep * (effMiscRandUnitFloat(D_0034DF38) * blend + (1.0f - blend));
    blend = work->head.startJitter;
    elem->radius = work->head.startRadius * (effMiscRandUnitFloat(D_0034DF38) * blend + (1.0f - blend));
    blend = work->head.endJitter;
    t = effMiscRandUnitFloat(D_0034DF38) * blend + (1.0f - blend);
    elem->radiusStep = (work->head.endRadius * t - elem->radius) / (f32)work->head.frames;
    func_00189C80(work->managedResource->valueWork, index);
    effMagatuhiSetValue(work->managedResource->valueWork, index, 0);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018BB88);

void effMagatuhiSetOrbitOrigin(EffMagatuhiOrbitWork *work, void *src) {
    PCP_COPY_VECTOR(work->head.origin, src);
}

/* This callback sets the packed orbit color, not an allocation handle. */
void effMagatuhiSetWorkBuffer(EffMagatuhiOrbitWork *work, u32 value) {
    work->color = value;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effMagatuhiCopyVecs2(EffMagatuhiOrbitWork *dst, void *src) {
    VU0_COPY_MATRIX(dst->matrix, src);
}


extern u32 func_0018CBC0(void *block);
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32 angle);
extern f32 sdfSinPoly(f32 angle);

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
    s32 delay; /* random start delay; afterwards the part of it that is replayed */
    f32 out[4];
    f32 origin[4];
    f32 height;
    f32 angle;
    f32 radius;
    f32 heightStep;
    f32 angleStep;
    f32 radiusStep;

    if (arg->type == 3) {
        if (func_0018CBC0(arg->effect) == 3) {
            work = effGetHandlerArg(arg->effect);
            count = work->head.count;
            valueWork = work->managedResource->valueWork;
            maxSteps = work->head.maxSteps;
            frames = work->head.frames;
            particle = work->particles;
            PCP_COPY_VECTOR(origin, work->head.origin);
            VU0_LOAD_MATRIX(work->matrix);
            for (i = 0; i < count; i += 3, particle += 3) {
                func_0018BA30(work, i);
                delay = effMiscRand(D_0034DF38) % frames;
                particle->delay = delay;
                /* Keep at most maxSteps samples: skip older state only when
                 * the random age exceeds that window; otherwise start at zero. */
                if (maxSteps < delay) {
                    steps = maxSteps;
                    delay -= steps;
                } else {
                    steps = maxSteps - delay;
                    delay = 0;
                }
                angleStep = particle->angleStep;
                radiusStep = particle->radiusStep;
                heightStep = particle->heightStep;
                angle = particle->angle + angleStep * (f32)delay;
                radius = particle->radius + radiusStep * (f32)delay;
                height = particle->height + heightStep * (f32)delay;
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
    f32 matrix[16];
    EffMagatuhiDriftParams head;
    EffMagatuhiDriftParticle *particles;
    u32 color;
    EffMagatuhiOwner *managedResource;
    void *buffer;
} EffMagatuhiDriftWork;

/* Clone drift parameters; return the work after its 0x30-byte state array. */
EffMagatuhiDriftWork *effMagatuhiCreateThird(EffMagatuhiDriftParams *src) {
    u32 count = src->count;
    u32 size = count * sizeof(EffMagatuhiDriftParticle);
    u32 handle = sdfAllocGeneralBlock(size + sizeof(EffMagatuhiDriftWork));
    EffMagatuhiDriftParticle *particle = (EffMagatuhiDriftParticle *)sdfResourceRetainAddress(handle);
    EffMagatuhiDriftWork *work = (EffMagatuhiDriftWork *)((u8 *)particle + size);
    s32 spread;
    u32 i;

    work->head = *src;
    work->particles = particle;
    work->color = 0x80808080;
    work->buffer = (void *)handle;
    EE_MMI_UNIT_MATRIX(work->matrix);
    if (work->head.spread <= 0) {
        work->head.spread = 1;
    }
    work->managedResource = effCloneMagatuhiWithColorResource(&work->head.count);
    spread = work->head.spread;
    for (i = 0; i < count; i++) {
        particle->delay = -(effMiscRand(D_0034DF38) % spread);
        particle++;
    }
    return work;
}

void effMagatuhiReleaseSecondaryOwnerAndBuffer(EffMagatuhiDriftWork *work) {
    effReleaseMagatuhiOwner(work->managedResource);
    sdfReleaseResourceAllocation(work->buffer);
}

/* vu0 routine: initialize a planar particle with normalized position/drift. */
void effMagatuhiInitializeDriftParticle(EffMagatuhiDriftWork *work, s32 index) {
    EffMagatuhiDriftParticle *particle = &work->particles[index];
    f32 direction[4];
    f32 radius;
    f32 random;

    random = effMiscRandUnitFloat(D_0034DF38) - 0.5f;
    radius = work->head.initialRadius * (random + random);
    random = effMiscRandUnitFloat(D_0034DF38) - 0.5f;
    direction[0] = random + random;
    direction[1] = 0.0f;
    random = effMiscRandUnitFloat(D_0034DF38) - 0.5f;
    direction[2] = random + random;
    VU0_LOAD_VF_FROM(vf10, *(u128 *)direction);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF_TO_MEMORY(vf10, *(u128 *)direction);
    particle->pos[0] = radius * direction[0];
    particle->pos[1] = 0.0f;
    particle->pos[2] = radius * direction[2];
    random = effMiscRandUnitFloat(D_0034DF38) - 0.5f;
    particle->dir[0] = random + random;
    particle->dir[1] = 0.0f;
    random = effMiscRandUnitFloat(D_0034DF38) - 0.5f;
    particle->dir[2] = random + random;
    VU0_LOAD_VF(vf10, particle->dir);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, particle->dir);
    particle->scale = work->head.initialScale *
        (effMiscRandUnitFloat(D_0034DF38) * work->head.initialScaleRandomness +
         (1.0f - work->head.initialScaleRandomness));
    particle->angle = effMiscRandUnitFloat(D_0034DF38) * 6.2831853f;
    particle->liftStep = work->head.initialLift *
        (effMiscRandUnitFloat(D_0034DF38) * work->head.initialLiftRandomness +
         (1.0f - work->head.initialLiftRandomness));
    func_00189C80(work->managedResource->valueWork, index);
    effMagatuhiSetValue(work->managedResource->valueWork, index, 0);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018C4C8);

void effMagatuhiSetDriftOrigin(EffMagatuhiDriftWork *work, void *src) {
    PCP_COPY_VECTOR(work->head.origin, src);
}

/* This callback sets packed drift color; +0x120 is not an owner pointer. */
void effMagatuhiSetFirstResource(EffMagatuhiDriftWork *work, u32 value) {
    work->color = value;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effMagatuhiCopyVecs3(EffMagatuhiDriftWork *dst, void *src) {
    VU0_COPY_MATRIX(dst->matrix, src);
}


/* Replay the first slot of each three-slot drift group from a random age. */
void effMagatuhiInitDriftParticles(EffMagatuhiCallback *arg) {
    u32 k;
    EffMagatuhiDriftWork *work;
    EffMagatuhiValueWork *valueWork;
    EffMagatuhiDriftParticle *particle;
    u32 count;
    s32 frames;
    u32 maxSteps;
    u32 i;
    u32 steps;
    s32 delay; /* random start delay; afterwards the part of it that is replayed */
    f32 out[4];
    f32 origin[4];
    f32 lift;
    f32 angle;
    f32 scale;
    f32 liftStep;
    f32 angleStep;
    f32 scaleStep;

    if (arg->type == 3) {
        if (func_0018CBC0(arg->effect) == 4) {
            work = effGetHandlerArg(arg->effect);
            count = work->head.count;
            valueWork = work->managedResource->valueWork;
            maxSteps = work->head.maxSteps;
            frames = work->head.frames;
            particle = work->particles;
            PCP_COPY_VECTOR(origin, work->head.origin);
            VU0_LOAD_MATRIX(work->matrix);
            for (i = 0; i < count; i += 3, particle += 3) {
                effMagatuhiInitializeDriftParticle(work, i);
                delay = effMiscRand(D_0034DF38) % frames;
                particle->delay = delay;
                /* Keep at most maxSteps samples: skip older state only when
                 * the random age exceeds that window; otherwise start at zero. */
                if (maxSteps < delay) {
                    steps = maxSteps;
                    delay -= steps;
                } else {
                    steps = maxSteps - delay;
                    delay = 0;
                }
                angleStep = work->head.angleStep;
                scaleStep = work->head.scaleStep;
                liftStep = particle->liftStep;
                angle = particle->angle + angleStep * (f32)delay;
                scale = particle->scale + scaleStep * (f32)delay;
                lift = particle->dir[1] + liftStep * (f32)delay;
                out[3] = 0;
                for (k = 0; k < steps; k++) {
                    lift += liftStep;
                    sdfSinPoly(angle);
                    out[0] = particle->pos[0] + particle->dir[0] * scale;
                    out[1] = particle->pos[1] + lift;
                    out[2] = particle->pos[2] + particle->dir[2] * scale;
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
                particle->dir[1] = lift;
            }
        }
    }
}

extern u32 func_0018CBC0(void *block);

s32 effMagatuhiDispatchByKind(EffMagatuhiCallback *work) {
    if (work->type == 3) {
        switch (func_0018CBC0(work->effect)) {
        case 0:
            break;
        case 1:
            effMagatuhiInitializeInterpolatedHistory(work);
            break;
        case 2:
            effMagatuhiInitRingParticles(work);
            break;
        case 3:
            effMagatuhiReplayOrbitStartDelays(work);
            break;
        case 4:
            effMagatuhiInitDriftParticles(work);
            break;
        }
    }
}
