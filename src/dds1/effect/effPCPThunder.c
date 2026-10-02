#include "common.h"
#include "eff.h"
#include "pcp_vu0.h"

/* Packed effect parameter-set accessor (see game/code_001624D0). */
extern void *effParamTableGetBlock(void *data, s32 index);
extern void effCreateThunderCellSystemWork(void *work);

extern void parReleaseCellSystem(u32 handle);
extern void parFillSymmetricCellColors(u32 param0, u32 param1, void *cells, u32 param3);
extern void func_0015CCD0(u32 param0, u32 param1, void *cells, u32 param3);
extern void func_0015CDF0(u32 param0, u32 param1, void *cells, u32 param3);
extern void sdfReleaseResourceAllocation(u32 handle);
extern u32 effMiscRand(void *state);
extern f32 effMiscRandUnitFloat(void *state);
extern u8 D_0034DF38[];

/* Parameter head (0x4C bytes) copied verbatim into the work. */
typedef struct {
    u8 pad00[0x10];
    u16 systemParam;    /* 0x10 */
    u8 pad12[2];
    u32 count;          /* 0x14 number of cells */
    u8 pad18[4];
    f32 scaledFirst;    /* 0x1C */
    f32 scaledSecond;   /* 0x20 */
    f32 rotationScale;  /* 0x24 randomized before constructing the rotation */
    u32 delayRange;     /* 0x28 modulus of delayFrames */
    u32 activeRange;    /* 0x2C modulus of activeFrames */
    u16 perCell;        /* 0x30 */
    u8 pad32[0xE];
    void *dispatchArg;  /* 0x40 */
    u8 pad44[8];
} EffThunderVectorParams;

typedef struct {
    u32 delayFrames;
    u32 activeFrames;
    f32 dirA[3];        /* 0x08 unit X/Z direction, with a separate Y height */
    f32 rotationAxis[3]; /* 0x14 normalized axis */
    f32 rotationScale;  /* 0x20 scales the per-update random rotation angle */
    f32 radius;         /* 0x24 scales the X/Z direction during placement */
    u32 color;         /* 0x28 */
} EffThunderVectorCell; /* 0x2C */

/* Both vector-based variants allocate this 0x64-byte work followed by cells.
   Their scale, color and teardown callbacks use the same constructor layout. */
typedef struct {
    EffThunderVectorParams head;
    EffThunderVectorCell *cells; /* 0x4C */
    u32 color;          /* 0x50 */
    f32 baseFirst;      /* 0x54 */
    f32 baseSecond;     /* 0x58 */
    void *system;       /* 0x5C */
    u32 handle;         /* 0x60 */
} EffThunderVectorWork; /* 0x64 */

/* Delay and active countdowns, followed by an alpha fade of the sampled color. */
typedef struct {
    u32 delayFrames;  /* 0x00: modulo fragmentFirstRange */
    u32 activeFrames; /* 0x04: modulo fragmentSecondRange, plus one */
    u32 color;        /* 0x08: fades after activeFrames reaches zero */
} EffThunderFrag; /* 0x0C */

/* 20-byte thunder element (see effThunderRandomizeCell/effThunderCellUpdate): randomized on
 * setup (direction floats plus moduli), then counted down while active. */
typedef struct {
    f32 directionX; /* 0x00 randomized to [-1, 1] */
    f32 directionY; /* 0x04 randomized to [-1, 1] */
    f32 directionZ; /* 0x08 randomized to [-1, 1] */
    u32 delayFrames;  /* 0x0C countdown before emitting geometry */
    u32 activeFrames; /* 0x10 countdown before the color fade */
} EffThunderCell; /* 0x14 */

/* 0x20-byte sub-element holding a handle released by parReleaseCellSystem. */
typedef struct {
    s32 age;            /* 0x00 negative during the initial delay */
    f32 verticalOffset; /* 0x04 */
    f32 verticalSpeed;  /* 0x08 */
    f32 orbitAngle;     /* 0x0C */
    f32 angularSpeed;   /* 0x10 */
    f32 orbitRadius;    /* 0x14 */
    f32 heightOffset;   /* 0x18 subtracted from the moving Y position */
    u32 systemHandle;   /* 0x1C released by parReleaseCellSystem */
} EffThunderSpark; /* 0x20 */


void effPCPThunderCreate(void *data) {
    void *work;

    work = effParamTableGetBlock(data, 0);
    effCreateThunderCellSystemWork(work);
}
void func_001634C0(void *work) {
    effCreateThunderCellSystemWork(work);
}

void effPCPThunderFree(EffThunderVectorWork *work) {
    parReleaseCellSystem((u32)work->system);
    sdfReleaseResourceAllocation(work->handle);
}

void func_00163508(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPCPThunderSetParam50(EffThunderVectorWork *work, u32 value) {
    work->color = value;
}

void effPCPThunderScale(f32 value, EffThunderVectorWork *work) {
    work->head.scaledFirst = work->baseFirst * value;
    work->head.scaledSecond = work->baseSecond * value;
}

u32 func_00163540(u32 arg0) {
    return arg0;
}

extern u32 sdfAllocGeneralBlock(s32 size);
extern u8 *sdfResourceRetainAddress(u32 handle);
extern void *parAllocateCellSystem(s32 count, s32 perCell, s32 groupDivisor, u32 kind);
extern void func_0015D078(void *system, u32 value);
extern void parDispatchSub(void *work, s32 sub, void *a2, void *a3);
extern void parPrependCellNode(void *system);
extern void parCellInit(void *system, s32 index);
extern s32 effMultiplyPackedColors(s32 color, s32 param);


/* Particle system as far as the cell colors are concerned. */
typedef struct {
    u8 *history;        /* 0x00 first of the cell's vertex vectors */
    u8 pad04[0xC];
    u32 color;          /* 0x10 */
} EffThunderParCell; /* 0x14 */

typedef struct {
    u8 pad00[8];
    s32 vertexCount;    /* 0x08 five vertices per group */
    u8 pad0C[8];
    EffThunderParCell *cells; /* 0x14 */
} EffThunderParSystem;

/* Restart a cell: delay/lifetime, radial direction and normalized rotation axis. */
void effThunderCellRestart(EffThunderVectorWork *work, s32 index) {
    EffThunderVectorCell *cell = work->cells + index;
    f32 dir[4];

    cell->delayFrames = effMiscRand(D_0034DF38) % work->head.delayRange;
    cell->activeFrames = effMiscRand(D_0034DF38) % work->head.activeRange + 1;
    dir[0] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    dir[1] = 0;
    dir[2] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, dir);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, dir);
    dir[1] = work->head.scaledSecond * 0.5f * ((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f);
    cell->dirA[0] = dir[0];
    cell->dirA[1] = dir[1];
    cell->dirA[2] = dir[2];
    dir[0] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 0.5f;
    dir[1] = 1.0f;
    dir[2] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 0.5f;
    VU0_LOAD_VF(vf10, dir);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, dir);
    cell->rotationAxis[0] = dir[0];
    cell->rotationAxis[1] = dir[1];
    cell->rotationAxis[2] = dir[2];
    cell->rotationScale = work->head.rotationScale * ((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 0.4f + 1.0f);
    cell->radius = work->head.scaledFirst * 0.5f * ((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 0.3f + 1.0f);
    cell->color = 0x80808080;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00163780);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00163AF8);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00163CD0);

EffThunderVectorWork *effThunderWorkCreate(EffThunderVectorParams *src) {
    u32 handle = sdfAllocGeneralBlock(src->count * sizeof(EffThunderVectorCell) + sizeof(EffThunderVectorWork));
    EffThunderVectorWork *work = (EffThunderVectorWork *)sdfResourceRetainAddress(handle);
    u32 i;

    work->head = *src;
    work->cells = (EffThunderVectorCell *)(work + 1);
    work->baseFirst = src->scaledFirst;
    work->baseSecond = src->scaledSecond;
    work->handle = handle;
    work->system = parAllocateCellSystem(work->head.count, work->head.perCell, 0, 0);
    parDispatchSub(work->system, 2, work->head.dispatchArg, work->head.dispatchArg);
    func_0015D078(work->system, work->head.systemParam);
    for (i = 0; i < work->head.count; i++) {
        work->cells[i].delayFrames = 0;
        work->cells[i].activeFrames = 0;
        work->cells[i].color = 0;
    }
    work->color = 0x80808080;
    return work;
}

void effPCPThunderFree2(EffThunderVectorWork *work) {
    parReleaseCellSystem((u32)work->system);
    sdfReleaseResourceAllocation(work->handle);
}

void effThunderCreateWorkFromPackedParams(void *data) {
    void *work;

    work = effParamTableGetBlock(data, 0);
    effThunderWorkCreate(work);
}

void func_00164020(void *work) {
    effThunderWorkCreate(work);
}

void func_00164038(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00164048(EffThunderVectorWork *work, u32 value) {
    work->color = value;
}

void effPCPThunderScale2(f32 value, EffThunderVectorWork *work) {
    work->head.scaledFirst = work->baseFirst * value;
    work->head.scaledSecond = work->baseSecond * value;
}

u32 func_00164070(u32 arg0) {
    return arg0;
}

/* Restart a cell (same routine as effThunderCellRestart, for the second effect). */
void effThunderRestartIndexedCell(EffThunderVectorWork *work, s32 index) {
    EffThunderVectorCell *cell = work->cells + index;
    f32 dir[4];

    cell->delayFrames = effMiscRand(D_0034DF38) % work->head.delayRange;
    cell->activeFrames = effMiscRand(D_0034DF38) % work->head.activeRange + 1;
    dir[0] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    dir[1] = 0;
    dir[2] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, dir);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, dir);
    dir[1] = work->head.scaledSecond * 0.5f * ((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f);
    cell->dirA[0] = dir[0];
    cell->dirA[1] = dir[1];
    cell->dirA[2] = dir[2];
    dir[0] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 0.5f;
    dir[1] = 1.0f;
    dir[2] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 0.5f;
    VU0_LOAD_VF(vf10, dir);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, dir);
    cell->rotationAxis[0] = dir[0];
    cell->rotationAxis[1] = dir[1];
    cell->rotationAxis[2] = dir[2];
    cell->rotationScale = work->head.rotationScale * ((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 0.4f + 1.0f);
    cell->radius = work->head.scaledFirst * 0.5f * ((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 0.3f + 1.0f);
    cell->color = 0x80808080;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_001642B0);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_001645A0);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_001646F8);

/* Parameter head (0xA4 bytes) of the spark effect, copied verbatim into the work. */
typedef struct {
    u8 pad00[0x10];
    f32 lowPos[3];      /* 0x10 spark position with the height offset removed */
    u8 pad1C[4];
    f32 pos[3];         /* 0x20 spark position */
    u8 pad2C[4];
    u16 systemParam;    /* 0x30 */
    u8 pad32[0x16];
    u16 halfLife;       /* 0x48 */
    u8 pad4A[6];
    void *dispatchArg;  /* 0x50 */
    u8 pad54[0x10];
    f32 heightOffset;   /* 0x64 */
    u32 sparkCount;     /* 0x68 */
    u8 loop;            /* 0x6C restart finished sparks */
    u8 pad6D[3];
    s32 duration;       /* 0x70 */
    s32 spread;         /* 0x74 modulus of the initial negative age */
    s32 fadeIn;         /* 0x78 */
    s32 fadeRange;      /* 0x7C */
    f32 orbitRadius;            /* 0x80 */
    f32 radiusRandomness;       /* 0x84 */
    f32 heightOffsetRange;      /* 0x88 */
    f32 angularSpeed;           /* 0x8C */
    f32 angularSpeedRandomness; /* 0x90 */
    f32 angularDamping;         /* 0x94 */
    f32 verticalSpeed;          /* 0x98 */
    f32 verticalSpeedRandomness; /* 0x9C */
    f32 verticalDamping;        /* 0xA0 */
} EffThunderSparkParams;

/* Each spark owns its own cell system; all subsystems are released before
   the containing work allocation. */
typedef struct {
    EffThunderSparkParams head;
    EffThunderSpark *subs; /* 0xA4 */
    u32 color;          /* 0xA8 */
    u32 handle;         /* 0xAC */
} EffThunderSparkWork; /* 0xB0 */

extern void effThunderSparkInit(EffThunderSparkWork *work, s32 index);

EffThunderSparkWork *effThunderSparkCreate(EffThunderSparkParams *src) {
    u32 handle = sdfAllocGeneralBlock(src->sparkCount * sizeof(EffThunderSpark) + sizeof(EffThunderSparkWork));
    EffThunderSparkWork *work = (EffThunderSparkWork *)sdfResourceRetainAddress(handle);
    s32 spread;
    u32 i;

    work->head = *src;
    work->subs = (EffThunderSpark *)(work + 1);
    work->color = 0x80808080;
    work->handle = handle;
    if (work->head.spread <= 0) {
        work->head.spread = 1;
    }
    spread = work->head.spread;
    for (i = 0; i < work->head.sparkCount; i++) {
        work->subs[i].systemHandle = (u32)parAllocateCellSystem(1, work->head.halfLife * 2 - 1, 0, 0);
        parDispatchSub((void *)work->subs[i].systemHandle, 2, work->head.dispatchArg, work->head.dispatchArg);
        func_0015D078((void *)work->subs[i].systemHandle, work->head.systemParam);
        effThunderSparkInit(work, i);
        work->subs[i].age = -(effMiscRand(D_0034DF38) % spread);
    }
    return work;
}

void effThunderDestroySubs(EffThunderSparkWork *work) {
    s32 count = (s32)work->head.sparkCount;
    s32 i = 0;

    if (count > 0) {
        do {
            parReleaseCellSystem(work->subs[i].systemHandle);
            i++;
        } while (i < count);
    }
    sdfReleaseResourceAllocation(work->handle);
}

void func_00164A98(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPCPThunderSetParamA8(EffThunderSparkWork *work, u32 value) {
    work->color = value;
}

/* Sample an orbit radius, angular/vertical speeds and initial height offset.
   The native update advances age and damps the two speeds independently. */
void effThunderSparkInit(EffThunderSparkWork *work, s32 index) {
    EffThunderSpark *spark = work->subs + index;
    f32 blend;

    spark->verticalOffset = 0;
    blend = work->head.verticalSpeedRandomness;
    spark->verticalSpeed = work->head.verticalSpeed * (effMiscRandUnitFloat(D_0034DF38) * blend + (1.0f - blend));
    spark->heightOffset = work->head.heightOffsetRange * effMiscRandUnitFloat(D_0034DF38);
    spark->orbitAngle = effMiscRandUnitFloat(D_0034DF38) * (3.14159265f * 2.0f);
    blend = work->head.angularSpeedRandomness;
    spark->angularSpeed = work->head.angularSpeed * (effMiscRandUnitFloat(D_0034DF38) * blend + (1.0f - blend));
    blend = work->head.radiusRandomness;
    spark->orbitRadius = work->head.orbitRadius * (effMiscRandUnitFloat(D_0034DF38) * blend + (1.0f - blend));
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00164BA8);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00165110);

extern void func_0015CEF8(void *system, u32 a, u32 b, u32 c);

/* Parameter head (0x54 bytes) of the fragment effect, copied verbatim into the work. */
typedef struct {
    u8 pad00[0x20];
    u16 systemParam;         /* 0x20 */
    u8 pad22[2];
    u32 fragmentCount;       /* 0x24 */
    u8 pad28[8];
    u32 fragmentFirstRange;  /* 0x30 modulus of delayFrames */
    u32 fragmentSecondRange; /* 0x34 modulus of activeFrames */
    u16 halfLife;            /* 0x38 */
    u8 pad3A[6];
    u32 arg40;               /* 0x40 */
    u8 pad44[4];
    u32 arg48;               /* 0x48 */
    u8 pad4C[4];
    u32 arg50;               /* 0x50 */
} EffThunderFragmentParams;

/* Single- and dual-system variants share this allocation layout, but the
   single-system update counts frames where the dual variant keeps a system. */
typedef struct {
    EffThunderFragmentParams head;
    EffThunderFrag *fragments; /* 0x54 */
    u32 color;               /* 0x58 */
    union {
        u32 updateCount;     /* single-system variant */
        void *secondarySystem; /* dual-system variant */
    } state;                 /* 0x5C */
    void *system;            /* 0x60 */
    u32 handle;              /* 0x64 */
} EffThunderFragmentWork; /* 0x68 */

extern void effThunderRandomizeFrag(EffThunderFragmentWork *work, s32 index);

EffThunderFragmentWork *effThunderFragCreate(EffThunderFragmentParams *src) {
    u32 handle = sdfAllocGeneralBlock(src->fragmentCount * sizeof(EffThunderFrag) + sizeof(EffThunderFragmentWork));
    EffThunderFragmentWork *work = (EffThunderFragmentWork *)sdfResourceRetainAddress(handle);
    u32 i;

    work->head = *src;
    work->fragments = (EffThunderFrag *)(work + 1);
    work->handle = handle;
    work->system = parAllocateCellSystem(work->head.fragmentCount, work->head.halfLife * 2 - 1, 0, 4);
    func_0015CEF8(work->system, work->head.arg40, work->head.arg48, work->head.arg50);
    func_0015D078(work->system, work->head.systemParam);
    for (i = 0; i < work->head.fragmentCount; i++) {
        effThunderRandomizeFrag(work, i);
    }
    work->state.updateCount = 0;
    work->color = 0x80808080;
    return work;
}

void effPCPThunderFree3(EffThunderFragmentWork *work) {
    parReleaseCellSystem((u32)work->system);
    sdfReleaseResourceAllocation(work->handle);
}

void effThunderShiftOriginByVectorDelta(u8 *p, void *src) {
        VU0_LOAD_VF(vf10, p + 0x10);
        VU0_LOAD_VF(vf11, src);
        VU0_STORE_VF(vf11, p + 0x10);
        VU0_SUB(vf11, vf11, vf10);
        VU0_LOAD_VF(vf10, p);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, p);
}

void effPCPThunderSetParam58(EffThunderFragmentWork *work, u32 value) {
    work->color = value;
}

u32 func_00165638(u32 arg0) {
    return arg0;
}

void func_00165640(EffThunderFragmentWork *work) {
    func_0015CCD0((u32)work->system, work->head.arg40, (void *)work->head.arg48, work->head.arg50);
}

void func_00165668(EffThunderFragmentWork *work) {
    func_0015CDF0((u32)work->system, work->head.arg40, (void *)work->head.arg48, work->head.arg50);
}

void func_00165690(EffThunderFragmentWork *work) {
    parFillSymmetricCellColors((u32)work->system, work->head.arg40, (void *)work->head.arg48, work->head.arg50);
}


/* Sample per-fragment timing values; the color is a fixed neutral grey. */
void effThunderRandomizeFrag(EffThunderFragmentWork *work, s32 index) {
    EffThunderFrag *frag = &work->fragments[index];

    frag->delayFrames = effMiscRand(&D_0034DF38) % work->head.fragmentFirstRange;
    frag->activeFrames = effMiscRand(&D_0034DF38) % work->head.fragmentSecondRange + 1;
    frag->color = EFF_THUNDER_FRAGMENT_GREY;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00165758);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00165D80);


extern void effThunderRandomizeFrag2(EffThunderFragmentWork *work, s32 index);

EffThunderFragmentWork *func_00165ED0(EffThunderFragmentParams *src) {
    u32 handle = sdfAllocGeneralBlock(src->fragmentCount * sizeof(EffThunderFrag) + sizeof(EffThunderFragmentWork));
    EffThunderFragmentWork *work = (EffThunderFragmentWork *)sdfResourceRetainAddress(handle);
    u32 i;

    work->head = *src;
    work->fragments = (EffThunderFrag *)(work + 1);
    work->handle = handle;
    work->state.secondarySystem = parAllocateCellSystem(work->head.fragmentCount, work->head.halfLife * 2 - 1, 0, 1);
    parDispatchSub(work->state.secondarySystem, 2, work->head.arg48, work->head.arg50);
    func_0015D078(work->state.secondarySystem, work->head.systemParam);
    work->system = parAllocateCellSystem(work->head.fragmentCount, work->head.halfLife * 2 - 1, 0, 0);
    parDispatchSub(work->system, 2, work->head.arg40, work->head.arg40);
    func_0015D078(work->system, work->head.systemParam);
    for (i = 0; i < work->head.fragmentCount; i++) {
        effThunderRandomizeFrag2(work, i);
    }
    work->color = 0x80808080;
    return work;
}

void effPCPThunderFree4(EffThunderFragmentWork *work) {
    parReleaseCellSystem((u32)work->state.secondarySystem);
    parReleaseCellSystem((u32)work->system);
    sdfReleaseResourceAllocation(work->handle);
}

void effThunderShiftEndpointsWithAnchor(u8 *p, void *src) {
        VU0_LOAD_VF(vf10, p + 0x10);
        VU0_LOAD_VF(vf11, src);
        VU0_STORE_VF(vf11, p + 0x10);
        VU0_SUB(vf11, vf11, vf10);
        VU0_LOAD_VF(vf10, p);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, p);
}

void func_00166130(EffThunderFragmentWork *work, u32 value) {
    work->color = value;
}

void effThunderRandomizeFrag2(EffThunderFragmentWork *work, s32 index) {
    EffThunderFrag *frag = &work->fragments[index];

    frag->delayFrames = effMiscRand(&D_0034DF38) % work->head.fragmentFirstRange;
    frag->activeFrames = effMiscRand(&D_0034DF38) % work->head.fragmentSecondRange + 1;
    frag->color = EFF_THUNDER_FRAGMENT_GREY;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_001661D8);

extern void func_001661D8(EffThunderFragmentWork *, s32);

void effThunderUpdateDualFragments(EffThunderFragmentWork *work) {
    s32 index = 0;
    EffThunderParSystem *firstSystem = work->state.secondarySystem;
    s32 count = work->head.fragmentCount;
    EffThunderParSystem *secondSystem = work->system;
    EffThunderParCell *secondCells = secondSystem->cells;
    u32 color = work->color;
    EffThunderFrag *frag = work->fragments;
    EffThunderParCell *firstCells = firstSystem->cells;
    s32 packedColor;

    if (count > 0) {
        do {
            if (frag->delayFrames == 0) {
                if (frag->activeFrames != 0) {
                    func_001661D8(work, index);
                    frag->activeFrames--;
                } else if (frag->color & 0xFF000000) {
                    frag->color -= 0x20000000;
                } else {
                    effThunderRandomizeFrag2(work, index);
                    parCellInit(work->state.secondarySystem, index);
                    parCellInit(work->system, index);
                }
            } else {
                frag->delayFrames--;
            }
            packedColor = effMultiplyPackedColors(frag->color, color);
            firstCells[index].color = packedColor;
            secondCells[index].color = packedColor;
            frag++;
            index++;
        } while (index < count);
    }
    parPrependCellNode(work->state.secondarySystem);
    parPrependCellNode(work->system);
}

extern void func_0015C7A0(void *system, u32 a, u32 b, u32 c);

/* Parameter head (0x48 bytes) of the cell effect, copied verbatim into the work. */
typedef struct {
    u8 pad00[0x10];
    u16 systemParam;         /* 0x10 */
    u8 pad12[6];
    u32 cellCount;           /* 0x18 */
    u8 pad1C[8];
    u32 cellFirstRange;      /* 0x24 modulus of delayFrames */
    u32 cellSecondRange;     /* 0x28 modulus of activeFrames */
    u16 halfLife;            /* 0x2C */
    u8 pad2E[6];
    u32 arg34;               /* 0x34 */
    u8 pad38[4];
    u32 arg3C;               /* 0x3C */
    u8 pad40[4];
    u32 arg44;               /* 0x44 */
} EffThunderCellParams;

/* The counted-down cell effect has its own 0x58-byte work and 0x14-byte
   particles, not the fragment or vector variants' resource offsets. */
typedef struct {
    EffThunderCellParams head;
    EffThunderCell *cells;   /* 0x48 */
    u32 unk4C;               /* 0x4C: settable, otherwise unobserved */
    void *system;            /* 0x50 */
    u32 handle;              /* 0x54 */
} EffThunderCellWork; /* 0x58 */

extern void effThunderRandomizeCell(EffThunderCellWork *work, s32 index);

EffThunderCellWork *effThunderCellCreate(EffThunderCellParams *src) {
    u32 handle = sdfAllocGeneralBlock(src->cellCount * sizeof(EffThunderCell) + sizeof(EffThunderCellWork));
    EffThunderCellWork *work = (EffThunderCellWork *)sdfResourceRetainAddress(handle);
    u32 i;

    work->head = *src;
    work->cells = (EffThunderCell *)(work + 1);
    work->handle = handle;
    work->system = parAllocateCellSystem(work->head.cellCount, work->head.halfLife * 2 - 1, 0, 2);
    func_0015C7A0(work->system, work->head.arg34, work->head.arg3C, work->head.arg44);
    func_0015D078(work->system, work->head.systemParam);
    for (i = 0; i < work->head.cellCount; i++) {
        effThunderRandomizeCell(work, i);
    }
    return work;
}

void effPCPThunderFree5(EffThunderCellWork *work) {
    parReleaseCellSystem((u32)work->system);
    sdfReleaseResourceAllocation(work->handle);
}

void func_00166B20(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPCPThunderSetParam4C(EffThunderCellWork *work, u32 value) {
    work->unk4C = value;
}

/* Spread all three direction components over [-1, 1] before sampling lifetimes. */
void effThunderRandomizeCell(EffThunderCellWork *work, s32 index) {
    EffThunderCell *cell = work->cells + index;
    f32 v;

    v = effMiscRandUnitFloat(&D_0034DF38) - 0.5f;
    cell->directionX = v + v;
    v = effMiscRandUnitFloat(&D_0034DF38) - 0.5f;
    cell->directionY = v + v;
    v = effMiscRandUnitFloat(&D_0034DF38) - 0.5f;
    cell->directionZ = v + v;
    cell->delayFrames = effMiscRand(&D_0034DF38) % work->head.cellFirstRange;
    cell->activeFrames = effMiscRand(&D_0034DF38) % work->head.cellSecondRange + 1;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00166C18);

extern void func_00166C18(EffThunderCellWork *work, s32 index);

/* Per-frame update of the cell effect: count down, fade, re-randomize, then flush the system. */
void effThunderCellUpdate(EffThunderCellWork *work) {
    s32 i = 0;
    s32 count = work->head.cellCount;
    EffThunderCell *cell = work->cells;
    EffThunderParCell *parCells = ((EffThunderParSystem *)work->system)->cells;

    if (count > 0) {
        do {
            if (cell->delayFrames == 0) {
                if (cell->activeFrames != 0) {
                    func_00166C18(work, i);
                    cell->activeFrames--;
                } else if (parCells[i].color & 0xFF000000) {
                    parCells[i].color -= 0x20000000;
                } else {
                    effThunderRandomizeCell(work, i);
                    parCellInit(work->system, i);
                }
            } else {
                cell->delayFrames--;
            }
            cell++;
            i++;
        } while (i < count);
    }
    parPrependCellNode(work->system);
}
