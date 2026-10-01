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
    u8 pad68[0x118];
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

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00192A10);

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

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00193280);

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

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00193AD0);

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

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00194428);

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
