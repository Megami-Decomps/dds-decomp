#include "common.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

/* Small work area: type id and count, an id block, a result table plus an
 * object released on cleanup. */
typedef struct {
    s32   type;         /* 0x00 effect type (== 3 in effMagatuhiDispatchByKind) */
    u16   count04;      /* 0x04 loop count */
    u8    pad06[2];     /* 0x06 */
    void *ptr08;        /* 0x08 id block / source block */
    u8    pad0C[0x0C];  /* 0x0C */
    u32  *out18;        /* 0x18 result table */
    u8    pad1C[4];     /* 0x1C */
    u32  *values;       /* 0x20: indexed value table */
    u8    pad24[0x10];  /* 0x24 */
    void *resource;     /* 0x34 released by effMagatuhiReleaseResource */
} EffMagatuhiWork; /* 0x38 */

extern void *effGetHandlerArg(void *arg);
extern u32 effBlendColor(u32 colorA, u32 colorB, f32 t);

/* Four matrix rows (three floats each) plus four floats, copied by effMagatuhiCopyHandlerRows. */
typedef struct {
    f32 row[4][4];
    f32 extra[4];
} EffMagatuhiRowsSrc;

typedef struct {
    f32 row[4][4];
    u8 pad40[0x14];
    f32 extra[4];
} EffMagatuhiRowsDst;


/* The two resource slots select different owners during each variant's teardown. */
typedef struct {
    u8   pad_0x000[0x120]; /* 0x000 */
    void *firstResource;   /* 0x120 */
    void *secondResource;  /* 0x124 */
    void *buffer;          /* 0x128 */
    void *extraBuffer;     /* 0x12C */
} EffMagatuhiMidWork; /* 0x130 */

/* The first and second teardown paths use different offsets for the trio. */
/* Parameter head of the first family: particles of 0x30 bytes precede the work. */
typedef struct {
    u8 pad00[0x28];
    s32 spread;            /* 0x28 modulus of the particle delay */
    u8 pad2C[0x34];
    u32 count;             /* 0x60 */
    u8 pad64[0x118];
} EffMagatuhiHeadFirst; /* 0x17C */

typedef struct {
    u8 pad00[0x20];
    s32 delay;             /* 0x20 */
    u8 pad24[0xC];
} EffMagatuhiParticleFirst; /* 0x30 */

typedef struct {
    EffMagatuhiHeadFirst head;
    EffMagatuhiParticleFirst *particles; /* 0x17C */
    void *mathResource;    /* 0x180 */
    u8 pad184[8];
    void *managedResource; /* 0x18C */
    void *buffer;          /* 0x190 */
} EffMagatuhiWideFirst;

/* Parameter head of the second family: `count` particles, delay spread at 0x48. */
typedef struct {
    u8 pad00[0x40];
    u8 respawn;            /* 0x40 restart finished particles */
    u8 pad41[3];
    s32 life;              /* 0x44 frames a particle lives */
    s32 spread;            /* 0x48 modulus of the particle delay */
    s32 fadeIn;            /* 0x4C */
    s32 fadeOut;           /* 0x50 */
    u8 pad54[0x10];
    u32 count;             /* 0x64 */
    u32 duration;          /* 0x68 */
    u8 pad6C[0x114];
} EffMagatuhiHeadSecond; /* 0x180 */

typedef struct {
    EffMagatuhiHeadSecond head;
    s32 *delays;           /* 0x180 */
    void *mathResource;    /* 0x184 */
    void *managedResource; /* 0x188 */
    void *buffer;          /* 0x18C */
} EffMagatuhiWideSecond;

/* Float source block read by effMagatuhiCopyFloatBlock. */
typedef struct EffMagatuhiSrc {
    f32 f00, f04, f08;
    u8 pad0C[4];
    f32 f10, f14, f18;
    u8 pad1C[4];
    f32 f20, f24;
} EffMagatuhiSrc; /* 0x28 */

/* Float destination block written by effMagatuhiCopyFloatBlock. */
typedef struct EffMagatuhiDst {
    f32 f00, f04, f08;
    u8 pad0C[4];
    f32 f10, f14, f18;
    u8 pad1C[0x18];
    f32 f34;
    u8 pad38[0x18];
    f32 f50;
} EffMagatuhiDst; /* 0x54 */

void effMagatuhiReleaseResource(EffMagatuhiWork *work) {
    func_003297C8(work->resource);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00191010);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00191450);

void effMagatuhiSetValue(EffMagatuhiWork *work, s32 index, u32 value) {
    work->values[index] = value;
}

/* Fill the result table with `count04` blends from colorA to colorB. */
void effMagatuhiFillColorTable(EffMagatuhiWork *work, u32 colorA, u32 colorB) {
    f32 t = 0.0f;
    u32 count = work->count04;
    u32 *out = work->out18;
    f32 step = 1.0f / count;
    u32 i;

    for (i = 0; i < count; i++) {
        *out++ = effBlendColor(colorA, colorB, t);
        t += step;
    }
}

extern f32 effMiscRandUnitFloat(void *state);
extern u8 D_003AA868[];

/* Owner of the value table an effect variant's particles write through. */
typedef struct {
    u8 pad00[0x1C];
    EffMagatuhiWork *valueWork; /* 0x1C */
} EffMagatuhiOwner;

void func_001918B8(void *valueWork, s32 index) {
    func_00190DF8(valueWork, index);
}

/* Particle records of the first variant family (0x18 bytes each). */
typedef struct {
    u32 unk00;
    f32 f04;
    f32 f08;
    f32 f0C;
    f32 f10;
    f32 f14;
} EffMagatuhiElemA; /* 0x18 */

typedef struct {
    u8 pad00[0x54];
    s32 frames;          /* 0x54 */
    u8 pad58[0xC];
    f32 f64;             /* 0x64 */
    f32 blend68;         /* 0x68 */
    f32 f6C;             /* 0x6C */
    f32 f70;             /* 0x70 */
    f32 f74;             /* 0x74 */
    f32 blend78;         /* 0x78 */
    f32 blend7C;         /* 0x7C */
    u8 pad80[0x9C];
    EffMagatuhiElemA *elems; /* 0x11C */
    EffMagatuhiOwner *owner; /* 0x120 */
} EffMagatuhiFamilyA;

extern u32 func_003292A8(s32 size);
extern u8 *sdfResourceRetainAddress(u32 handle);
extern void *effCloneMagatuhiWithColorResource(void *block);
extern void *effAllocSlotArray(s32 count);
extern u32 effMiscRand(void *state);
extern u8 D_003AA868[];

EffMagatuhiWideFirst *effMagatuhiCreateFirst(EffMagatuhiHeadFirst *src) {
    u32 count = src->count;
    u32 size = count * sizeof(EffMagatuhiParticleFirst);
    u32 handle = func_003292A8(size + sizeof(EffMagatuhiWideFirst));
    EffMagatuhiParticleFirst *particle = (EffMagatuhiParticleFirst *)sdfResourceRetainAddress(handle);
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
        particle->delay = -(effMiscRand(D_003AA868) % spread);
        particle++;
    }
    return work;
}

void effMagatuhiReleaseMathOwnerAndBuffer(EffMagatuhiWideFirst *work) {
    effMathReleaseWorkResource(work->mathResource);
    effReleaseMagatuhiOwner(work->managedResource);
    func_003297C8(work->buffer);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00191AD0);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00191CD0);

void effMagatuhiCopyFloatBlock(EffMagatuhiWork *work, EffMagatuhiSrc *src) {
    EffMagatuhiDst *dst = effGetHandlerArg(work->ptr08);

    dst->f00 = src->f00;
    dst->f04 = src->f04;
    dst->f08 = src->f08;
    dst->f34 = src->f20;
    dst->f10 = src->f10;
    dst->f14 = src->f14;
    dst->f18 = src->f18;
    dst->f50 = src->f24;
}

EffMagatuhiWideSecond *effMagatuhiCreateSecond(EffMagatuhiHeadSecond *src) {
    u32 count = src->count;
    u32 handle = func_003292A8(count * 4 + sizeof(EffMagatuhiWideSecond));
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
        *delays++ = -(effMiscRand(D_003AA868) % spread);
    }
    return work;
}

void effMagatuhiReleaseWideWorkResources(EffMagatuhiWideSecond *work) {
    effMathReleaseWorkResource(work->mathResource);
    effReleaseMagatuhiOwner(work->managedResource);
    func_003297C8(work->buffer);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00192470);

extern void *effMathGetSlotAt(void *slots, s32 index);
extern void func_00195BE0(void *slots, s32 index, void *out);
extern void func_00190DE0(void *owner);
extern void func_00192470(void *work, s32 index);
extern void func_00191450(void *valueWork, s32 index, void *out);

typedef struct {
    u8 pad00[0x30];
    f32 scale; /* 0x30 */
    f32 base;  /* 0x34 */
} EffMagatuhiSlot;

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00192778);

void effMagatuhiCopyHandlerRows(EffMagatuhiWork *work, EffMagatuhiRowsSrc *src) {
    EffMagatuhiRowsDst *dst = effGetHandlerArg(work->ptr08);
    u32 i;

    for (i = 0; i < 4; i++) {
        dst->row[i][0] = src->row[i][0];
        dst->row[i][1] = src->row[i][1];
        dst->row[i][2] = src->row[i][2];
        dst->extra[i] = src->extra[i];
    }
}

void func_00192A10(EffMagatuhiWork *arg) {
    EffMagatuhiWideSecond *work = effGetHandlerArg(arg->ptr08);
    EffMagatuhiWork *valueWork = ((EffMagatuhiOwner *)work->managedResource)->valueWork;
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
        func_00192470(work, i);
        *delays = effMiscRand(D_003AA868) % life;
        slot = effMathGetSlotAt(work->mathResource, i);
        if (duration < *delays) {
            slot->scale = slot->base * (f32)(*delays - duration);
            span = duration;
        } else {
            slot->scale = 0.0f;
            span = duration - *delays;
        }
        t = 0.0f;
        func_00195BE0(work->mathResource, i, from);
        slot->scale = slot->base * (f32)*delays;
        func_00195BE0(work->mathResource, i, to);
        step = 1.0f / (f32)span;
        for (j = 0; j < span; j++) {
            VU0_LOAD_VF(vf10, from);
            VU0_LOAD_VF(vf11, to);
            VU0_LERP_VF10(t);
            VU0_STORE_VF(vf10, out);
            func_00191450(valueWork, i, out);
            t += step;
        }
        (*delays)++;
        delays += 4;
    }
}

/* Parameter head shared by the third and fourth families (copied to 0x40 inside the work). */
typedef struct {
    u8 pad00[0x18];
    s32 spread;            /* 0x18 modulus of the particle delay */
    u8 pad1C[0x24];
    u32 count;             /* 0x40 */
    u8 pad44[0x98];
} EffMagatuhiHeadThird; /* 0xDC */

/* Fourth family: work first, 0x18-byte particles after it. */
typedef struct {
    u8 matrix[0x40];       /* 0x00 identity */
    EffMagatuhiHeadThird head; /* 0x40 */
    EffMagatuhiElemA *particles; /* 0x11C */
    void *managedResource; /* 0x120 */
    u32 color;             /* 0x124 */
    void *buffer;          /* 0x128 */
} EffMagatuhiWideFourth;

EffMagatuhiWideFourth *effMagatuhiCreateFourth(EffMagatuhiHeadThird *src) {
    u32 count = src->count;
    u32 handle = func_003292A8(count * sizeof(EffMagatuhiElemA) + sizeof(EffMagatuhiWideFourth));
    EffMagatuhiWideFourth *work = (EffMagatuhiWideFourth *)sdfResourceRetainAddress(handle);
    EffMagatuhiElemA *particle = (EffMagatuhiElemA *)(work + 1);
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
        particle->unk00 = -(effMiscRand(D_003AA868) % spread);
        particle++;
    }
    return work;
}

void effMagatuhiReleaseOwnerAndBuffer(EffMagatuhiMidWork *work) {
    effReleaseMagatuhiOwner(work->firstResource);
    func_003297C8(work->buffer);
}

void effMagatuhiInitParticleA(EffMagatuhiFamilyA *work, s32 index) {
    EffMagatuhiElemA *elem = &work->elems[index];
    f32 blend;
    f32 t;

    elem->unk00 = 0;
    elem->f04 = -work->f6C * effMiscRandUnitFloat(D_003AA868);
    elem->f08 = effMiscRandUnitFloat(D_003AA868) * (3.14159265f * 2.0f);
    blend = work->blend68;
    elem->f0C = work->f64 * (effMiscRandUnitFloat(D_003AA868) * blend + (1.0f - blend));
    blend = work->blend78;
    elem->f10 = work->f70 * (effMiscRandUnitFloat(D_003AA868) * blend + (1.0f - blend));
    blend = work->blend7C;
    t = effMiscRandUnitFloat(D_003AA868) * blend + (1.0f - blend);
    elem->f14 = (work->f74 * t - elem->f10) / (f32)work->frames;
    func_001918B8(work->owner->valueWork, index);
    effMagatuhiSetValue(work->owner->valueWork, index, 0);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00192F80);

void effMagatuhiCopyWorkVector(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x40, src);
}

void effMagatuhiSetSecondResource(EffMagatuhiMidWork *work, void *value) {
    work->secondResource = value;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effMagatuhiCopyVecs(EffMagatuhiMidWork *dst, EffMagatuhiMidWork *src) {
    VU0_COPY_MATRIX(dst, src);
}

/* One orbiting particle (0x48 bytes): the angle and radius advance by per-frame steps at a fixed height. */
typedef struct {
    s32 delay;      /* 0x00 */
    f32 height;     /* 0x04 */
    f32 angle;      /* 0x08 */
    f32 angleStep;  /* 0x0C */
    f32 radius;     /* 0x10 */
    f32 radiusStep; /* 0x14 */
    u8 pad18[0x30];
} EffMagatuhiRingParticle; /* 0x48 */

typedef struct {
    u8 matrix[0x40];       /* 0x00 */
    f32 origin[4];         /* 0x40 */
    u8 pad50[4];
    s32 spread;            /* 0x54 modulus of the particle delay */
    u8 pad58[0x28];
    u32 count;             /* 0x80 */
    u32 maxSteps;          /* 0x84 */
    u8 pad88[0x94];
    EffMagatuhiRingParticle *particles; /* 0x11C */
    void *managedResource; /* 0x120 */
} EffMagatuhiRingWork;

extern u32 func_001947F8(void *block);
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32 angle);
extern f32 sdfSinPoly(f32 angle);

/* Advance the ring family: each slot is replayed from a random delay, one orbit step at a time, into the value table. */
void func_00193280(EffMagatuhiWork *arg) {
    u32 k;
    EffMagatuhiRingWork *work;
    EffMagatuhiWork *valueWork;
    EffMagatuhiRingParticle *particle;
    u32 count;
    s32 spread;
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
        if (func_001947F8(arg->ptr08) == 2) {
            work = effGetHandlerArg(arg->ptr08);
            count = work->count;
            valueWork = ((EffMagatuhiOwner *)work->managedResource)->valueWork;
            maxSteps = work->maxSteps;
            spread = work->spread;
            particle = work->particles;
            PCP_COPY_VECTOR(origin, work->origin);
            VU0_LOAD_MATRIX(work);
            for (i = 0; i < count; i += 3, particle++) {
                effMagatuhiInitParticleA(work, i);
                delay = effMiscRand(D_003AA868) % spread;
                particle->delay = delay;
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
                    func_00191450(valueWork, i, out);
                    angle += angleStep;
                    radius += radiusStep;
                }
                particle->radius = radius;
                particle->angle = angle;
            }
        }
    }
}

/* Fifth family head: 0xE0 bytes, count at 0x44. */
typedef struct {
    u8 pad00[0x18];
    s32 spread;            /* 0x18 modulus of the particle delay */
    u8 pad1C[0x28];
    u32 count;             /* 0x44 */
    u8 pad48[0x98];
} EffMagatuhiHeadFifth; /* 0xE0 */

typedef struct {
    u32 delay;             /* 0x00 */
    u8 pad04[0x18];
} EffMagatuhiParticleFifth; /* 0x1C */

typedef struct {
    u8 matrix[0x40];       /* 0x00 identity */
    EffMagatuhiHeadFifth head; /* 0x40 */
    EffMagatuhiParticleFifth *particles; /* 0x120 */
    void *managedResource; /* 0x124 */
    u32 color;             /* 0x128 */
    void *buffer;          /* 0x12C */
} EffMagatuhiWideFifth;

EffMagatuhiWideFifth *effMagatuhiCreateFifth(EffMagatuhiHeadFifth *src) {
    u32 count = src->count;
    u32 handle = func_003292A8(count * sizeof(EffMagatuhiParticleFifth) + sizeof(EffMagatuhiWideFifth));
    EffMagatuhiWideFifth *work = (EffMagatuhiWideFifth *)sdfResourceRetainAddress(handle);
    EffMagatuhiParticleFifth *particle = (EffMagatuhiParticleFifth *)(work + 1);
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
        particle->delay = -(effMiscRand(D_003AA868) % spread);
        particle++;
    }
    return work;
}

void effMagatuhiReleaseOwnerAndExtraBuffer(EffMagatuhiMidWork *work) {
    effReleaseMagatuhiOwner(work->secondResource);
    func_003297C8(work->extraBuffer);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00193668);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_001937C0);

void func_00193A88(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x40, src);
}

void effMagatuhiSetWorkBuffer(EffMagatuhiMidWork *work, void *value) {
    work->buffer = value;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effMagatuhiCopyVecs2(EffMagatuhiMidWork *dst, EffMagatuhiMidWork *src) {
    VU0_COPY_MATRIX(dst, src);
}

/* One orbiting particle (0x54 bytes): height, angle and radius each advance by a per-frame step. */
typedef struct {
    s32 delay;      /* 0x00 */
    f32 height;     /* 0x04 */
    f32 heightStep; /* 0x08 */
    f32 angle;      /* 0x0C */
    f32 angleStep;  /* 0x10 */
    f32 radius;     /* 0x14 */
    f32 radiusStep; /* 0x18 */
    u8 pad1C[0x38];
} EffMagatuhiOrbitParticle; /* 0x54 */

typedef struct {
    u8 matrix[0x40];       /* 0x00 */
    f32 origin[4];         /* 0x40 */
    u8 pad50[4];
    s32 spread;            /* 0x54 modulus of the particle delay */
    u8 pad58[0x2C];
    u32 count;             /* 0x84 */
    u32 maxSteps;          /* 0x88 */
    u8 pad8C[0x94];
    EffMagatuhiOrbitParticle *particles; /* 0x120 */
    void *managedResource; /* 0x124 */
} EffMagatuhiOrbitWork;

extern u32 func_001947F8(void *block);
extern void func_00193668(void *work, u32 index);
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32 angle);
extern f32 sdfSinPoly(f32 angle);

/* Advance the orbiting-particle family: each group of three slots is replayed from a random delay, one orbit step at a time, into the value table. */
void func_00193AD0(EffMagatuhiWork *arg) {
    u32 k;
    EffMagatuhiOrbitWork *work;
    EffMagatuhiWork *valueWork;
    EffMagatuhiOrbitParticle *particle;
    u32 count;
    s32 spread;
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
        if (func_001947F8(arg->ptr08) == 3) {
            work = effGetHandlerArg(arg->ptr08);
            count = work->count;
            valueWork = ((EffMagatuhiOwner *)work->managedResource)->valueWork;
            maxSteps = work->maxSteps;
            spread = work->spread;
            particle = work->particles;
            PCP_COPY_VECTOR(origin, work->origin);
            VU0_LOAD_MATRIX(work);
            for (i = 0; i < count; i += 3, particle++) {
                func_00193668(work, i);
                delay = effMiscRand(D_003AA868) % spread;
                particle->delay = delay;
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
                    func_00191450(valueWork, i, out);
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

typedef struct {
    u8 matrix[0x40];       /* 0x00 identity */
    EffMagatuhiHeadThird head; /* 0x40 */
    EffMagatuhiParticleFirst *particles; /* 0x11C */
    u32 color;             /* 0x120 */
    void *managedResource; /* 0x124 */
    void *buffer;          /* 0x128 */
} EffMagatuhiWideThird;

EffMagatuhiWideThird *effMagatuhiCreateThird(EffMagatuhiHeadThird *src) {
    u32 count = src->count;
    u32 size = count * sizeof(EffMagatuhiParticleFirst);
    u32 handle = func_003292A8(size + sizeof(EffMagatuhiWideThird));
    EffMagatuhiParticleFirst *particle = (EffMagatuhiParticleFirst *)sdfResourceRetainAddress(handle);
    EffMagatuhiWideThird *work = (EffMagatuhiWideThird *)((u8 *)particle + size);
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
        particle->delay = -(effMiscRand(D_003AA868) % spread);
        particle++;
    }
    return work;
}

void effMagatuhiReleaseSecondaryOwnerAndBuffer(EffMagatuhiMidWork *work) {
    effReleaseMagatuhiOwner(work->secondResource);
    func_003297C8(work->buffer);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00193F10);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00194100);

void func_001943E0(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x40, src);
}

void effMagatuhiSetFirstResource(EffMagatuhiMidWork *work, void *value) {
    work->firstResource = value;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effMagatuhiCopyVecs3(EffMagatuhiMidWork *dst, EffMagatuhiMidWork *src) {
    VU0_COPY_MATRIX(dst, src);
}

/* One drifting particle (0x90 bytes): a base point plus a direction scaled by an advancing amount. */
typedef struct {
    f32 pos[3];        /* 0x00 */
    u8 padC[4];
    f32 dir[3];        /* 0x10; dir[1] is the one that advances each step */
    u8 pad1C[4];
    s32 delay;         /* 0x20 */
    f32 scale;         /* 0x24 */
    f32 angle;         /* 0x28 */
    f32 liftStep;      /* 0x2C */
    u8 pad30[0x60];
} EffMagatuhiDriftParticle; /* 0x90 */

typedef struct {
    u8 matrix[0x40];       /* 0x00 */
    f32 origin[4];         /* 0x40 */
    u8 pad50[4];
    s32 spread;            /* 0x54 modulus of the particle delay */
    u8 pad58[0x18];
    f32 angleStep;         /* 0x70 */
    u8 pad74[4];
    f32 scaleStep;         /* 0x78 */
    u8 pad7C[4];
    u32 count;             /* 0x80 */
    u32 maxSteps;          /* 0x84 */
    u8 pad88[0x94];
    EffMagatuhiDriftParticle *particles; /* 0x11C */
    void *pad120;
    void *managedResource; /* 0x124 */
} EffMagatuhiDriftWork;

/* Advance the drift family: each slot is replayed from a random delay, one step at a time, into the value table. */
void func_00194428(EffMagatuhiWork *arg) {
    u32 k;
    EffMagatuhiDriftWork *work;
    EffMagatuhiWork *valueWork;
    EffMagatuhiDriftParticle *particle;
    u32 count;
    s32 spread;
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
        if (func_001947F8(arg->ptr08) == 4) {
            work = effGetHandlerArg(arg->ptr08);
            count = work->count;
            valueWork = ((EffMagatuhiOwner *)work->managedResource)->valueWork;
            maxSteps = work->maxSteps;
            spread = work->spread;
            particle = work->particles;
            PCP_COPY_VECTOR(origin, work->origin);
            VU0_LOAD_MATRIX(work);
            for (i = 0; i < count; i += 3, particle++) {
                func_00193F10(work, i);
                delay = effMiscRand(D_003AA868) % spread;
                particle->delay = delay;
                if (maxSteps < delay) {
                    steps = maxSteps;
                    delay -= steps;
                } else {
                    steps = maxSteps - delay;
                    delay = 0;
                }
                angleStep = work->angleStep;
                scaleStep = work->scaleStep;
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
                    func_00191450(valueWork, i, out);
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

extern u32 func_001947F8(void *block);

s32 effMagatuhiDispatchByKind(EffMagatuhiWork *work) {
    if (work->type == 3) {
        switch (func_001947F8(work->ptr08)) {
        case 0:
            break;
        case 1:
            func_00192A10(work);
            break;
        case 2:
            func_00193280(work);
            break;
        case 3:
            func_00193AD0(work);
            break;
        case 4:
            func_00194428(work);
            break;
        }
    }
}
