#include "common.h"
#include "eff.h"
#include "pcp_vu0.h"

/* Packed effect parameter-set accessor shared with the effect constructors. */
extern void *effParamTableGetBlock(void *data, s32 index);
extern void effCreateThunderCellSystemWork(void *work);

extern void parReleaseCellSystem(u32 handle);
extern void parFillSymmetricCellColors(u32 param0, u32 param1, void *cells, u32 param3);
extern void parDecreaseSymmetricCellAlpha(u32 param0, u32 param1, void *cells, u32 param3);
extern void parIncreaseSymmetricCellAlpha(u32 param0, u32 param1, void *cells, u32 param3);
extern void sdfReleaseResourceAllocation(u32 handle);
extern u32 effMiscRand(void *state);
extern f32 effMiscRandUnitFloat(void *state);
extern u8 D_003AA868[];

/* Keep literal types and arithmetic associations identical to the native code. */
#define EFF_THUNDER_PARAMETER_BLOCK 0
#define EFF_THUNDER_VECTOR_COMPONENTS 4
#define EFF_THUNDER_NEUTRAL_COLOR 0x80808080
#define EFF_THUNDER_ALPHA_MASK 0xFF000000
#define EFF_THUNDER_ALPHA_WRAP_ADD 0xE0000000
#define EFF_THUNDER_ALPHA_FADE_STEP 0x20000000
#define EFF_THUNDER_RANDOM_MIDPOINT 0.5f
#define EFF_THUNDER_RANDOM_SPAN 2.0f
#define EFF_THUNDER_AXIS_JITTER 0.5f
#define EFF_THUNDER_ROTATION_JITTER 0.4f
#define EFF_THUNDER_RADIUS_JITTER 0.3f
#define EFF_THUNDER_HALF_SCALE 0.5f
#define EFF_THUNDER_HALF_TURN 3.14159265f
#define EFF_THUNDER_MIN_DELAY_SPREAD 1
#define EFF_THUNDER_SINGLE_CELL 1
#define EFF_THUNDER_ENDPOINT_VECTOR_BYTES 0x10
#define EFF_THUNDER_MIN_ACTIVE_FRAMES 1

/* Parameter head (0x4C bytes) copied verbatim into the work. */
typedef struct {
    u8 pad00[0x10];
    u16 systemParam;    /* 0x10 */
    u8 pad12[2];
    u32 cellCount;      /* 0x14 number of cells */
    u8 pad18[4];
    f32 radiusScale;    /* 0x1C halves and jitters the initial X/Z radius */
    f32 heightScale;    /* 0x20 scales the separately sampled Y component */
    f32 rotationScale;  /* 0x24 randomized before constructing the rotation */
    u32 startDelayRange; /* 0x28 modulus of delayFrames */
    u32 activeFrameRange; /* 0x2C modulus of activeFrames before adding one */
    u16 perCell;        /* 0x30 */
    u8 pad32[0xE];
    void *dispatchArg;  /* 0x40 */
    u8 pad44[8];
} EffThunderVectorParams;

typedef struct {
    u32 delayFrames;
    u32 activeFrames;
    f32 placementVector[3]; /* 0x08 unit X/Z direction, with a separate Y height */
    f32 rotationAxis[3]; /* 0x14 normalized axis */
    f32 rotationScale;  /* 0x20 scales the per-update random rotation angle */
    f32 radius;         /* 0x24 scales the X/Z direction during placement */
    u32 color;         /* 0x28 */
} EffThunderVectorCell; /* 0x2C */

/* Both vector-based variants allocate this 0x64-byte work followed by cells.
   Their scale, tint and teardown callbacks use the same constructor layout. */
typedef struct {
    EffThunderVectorParams head;
    EffThunderVectorCell *cells; /* 0x4C */
    u32 tintColor;      /* 0x50 multiplies each cell's sampled/faded color */
    f32 baseRadiusScale; /* 0x54 retained for absolute scale callbacks */
    f32 baseHeightScale; /* 0x58 retained for absolute scale callbacks */
    void *cellSystem;   /* 0x5C */
    u32 allocationHandle; /* 0x60 */
} EffThunderVectorWork; /* 0x64 */

/* Delay and active countdowns, followed by an alpha fade of the sampled color. */
typedef struct {
    u32 delayFrames;  /* 0x00: modulo startDelayRange */
    u32 activeFrames; /* 0x04: modulo activeFrameRange, plus one */
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


/* Create the first vector variant from the first packed parameter block;
   this callback deliberately ignores the constructor's returned work. */
void effPCPThunderCreate(void *parameterTable) {
    void *parameters;

    parameters = effParamTableGetBlock(parameterTable, EFF_THUNDER_PARAMETER_BLOCK);
    effCreateThunderCellSystemWork(parameters);
}
/* Direct parameter-pointer entry for the first vector variant. */
void func_0016B118(void *parameters) {
    effCreateThunderCellSystemWork(parameters);
}

/* Release the cell system before releasing the containing work allocation. */
void effThunderReleaseVectorWork(EffThunderVectorWork *work) {
    parReleaseCellSystem((u32)work->cellSystem);
    sdfReleaseResourceAllocation(work->allocationHandle);
}

/* vu0 routine: copy the entire quadword, including its fourth component. */
void func_0016B160(void *destination, void *source) {
    PCP_COPY_VECTOR(destination, source);
}

/* Set the packed modulation color applied to every vector cell. */
void effThunderSetVectorTint(EffThunderVectorWork *work, u32 tintColor) {
    work->tintColor = tintColor;
}

/* Absolute radius/height scaling from the retained constructor values. */
void effThunderScaleVectorDimensions(f32 factor, EffThunderVectorWork *work) {
    work->head.radiusScale = work->baseRadiusScale * factor;
    work->head.heightScale = work->baseHeightScale * factor;
}

/* Preserve the caller's word unchanged; its wider callback role is unknown. */
u32 func_0016B198(u32 value) {
    return value;
}

extern u32 sdfAllocGeneralBlock(s32 size);
extern u8 *sdfResourceRetainAddress(u32 handle);
extern void *parAllocateCellSystem(s32 count, s32 perCell, s32 groupDivisor, u32 kind);
extern void func_00164C68(void *system, u32 value);
extern void parDispatchSub(void *work, s32 sub, void *a2, void *a3);
extern void parPrependCellNode(void *system);
extern void parCellInit(void *system, s32 index);
extern u32 effMultiplyPackedColors(u32 colorA, u32 colorB);


/* Particle system as far as the cell colors are concerned. */
typedef struct {
    u128 *history;      /* 0x00 first vertex quadword */
    void *vertices;
    s32 vertexCount;
    s32 unk0C;
    u32 color;          /* 0x10 */
} EffThunderParCell; /* 0x14 */

typedef struct {
    u8 pad00[8];
    s32 vertexCount;    /* 0x08 five vertices per group */
    u8 pad0C[8];
    EffThunderParCell *cells; /* 0x14 */
} EffThunderParSystem;

/* Resample timing, normalize X/Z before assigning Y, then sample the axis.
   Both timing moduli are unchecked; the fourth scratch lane is not initialized. */
void effThunderCellRestart(EffThunderVectorWork *work, s32 index) {
    EffThunderVectorCell *cell = work->cells + index;
    f32 scratchVector[EFF_THUNDER_VECTOR_COMPONENTS];

    cell->delayFrames = effMiscRand(D_003AA868) % work->head.startDelayRange;
    cell->activeFrames = effMiscRand(D_003AA868) % work->head.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
    scratchVector[0] = (effMiscRandUnitFloat(D_003AA868) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN;
    scratchVector[1] = 0;
    scratchVector[2] = (effMiscRandUnitFloat(D_003AA868) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN;
    VU0_LOAD_VF(vf10, scratchVector);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, scratchVector);
    scratchVector[1] = work->head.heightScale * EFF_THUNDER_HALF_SCALE * ((effMiscRandUnitFloat(D_003AA868) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN);
    cell->placementVector[0] = scratchVector[0];
    cell->placementVector[1] = scratchVector[1];
    cell->placementVector[2] = scratchVector[2];
    scratchVector[0] = (effMiscRandUnitFloat(D_003AA868) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_AXIS_JITTER;
    scratchVector[1] = 1.0f;
    scratchVector[2] = (effMiscRandUnitFloat(D_003AA868) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_AXIS_JITTER;
    VU0_LOAD_VF(vf10, scratchVector);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, scratchVector);
    cell->rotationAxis[0] = scratchVector[0];
    cell->rotationAxis[1] = scratchVector[1];
    cell->rotationAxis[2] = scratchVector[2];
    cell->rotationScale = work->head.rotationScale * ((effMiscRandUnitFloat(D_003AA868) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_ROTATION_JITTER + 1.0f);
    cell->radius = work->head.radiusScale * EFF_THUNDER_HALF_SCALE * ((effMiscRandUnitFloat(D_003AA868) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_RADIUS_JITTER + 1.0f);
    cell->color = EFF_THUNDER_NEUTRAL_COLOR;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016B3D8);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016B750);

extern void func_0016B3D8(EffThunderVectorWork *, s32);
extern void func_0016B750(EffThunderVectorWork *, s32);

/* Delay -> active geometry -> alpha fade -> restart; tint each render cell.
   The unsigned wrap-add is not a saturating fade. Submit even for signed count <= 0. */
void effThunderUpdateVectorCells(EffThunderVectorWork *work) {
    s32 i = 0;
    EffThunderParSystem *renderSystem = work->cellSystem;
    s32 cellCount = work->head.cellCount;
    u32 tintColor = work->tintColor;
    EffThunderVectorCell *cell = work->cells;
    EffThunderParCell *renderCells = renderSystem->cells;
    u32 *renderColor;

    if (cellCount > 0) {
        renderColor = &renderCells->color;
        do {
            if (cell->delayFrames == 0) {
                if (cell->activeFrames != 0) {
                    func_0016B3D8(work, i);
                    func_0016B750(work, i);
                    cell->activeFrames--;
                } else if (cell->color & EFF_THUNDER_ALPHA_MASK) {
                    cell->color += EFF_THUNDER_ALPHA_WRAP_ADD;
                    func_0016B750(work, i);
                } else {
                    effThunderCellRestart(work, i);
                    parCellInit(work->cellSystem, i);
                }
            } else {
                cell->delayFrames--;
            }
            *renderColor = effMultiplyPackedColors(cell->color, tintColor);
            i++;
            cell++;
            renderColor = (u32 *)((u8 *)renderColor + sizeof(EffThunderParCell));
        } while (i < cellCount);
    }
    parPrependCellNode(work->cellSystem);
}

/* Allocate one work followed by its vector cells and retain unscaled dimensions.
   Only cell countdowns/color are initialized; geometry is sampled on the first restart. */
EffThunderVectorWork *effThunderWorkCreate(EffThunderVectorParams *parameters) {
    u32 allocationHandle = sdfAllocGeneralBlock(parameters->cellCount * sizeof(EffThunderVectorCell) + sizeof(EffThunderVectorWork));
    EffThunderVectorWork *work = (EffThunderVectorWork *)sdfResourceRetainAddress(allocationHandle);
    u32 i;

    work->head = *parameters;
    work->cells = (EffThunderVectorCell *)(work + 1);
    work->baseRadiusScale = parameters->radiusScale;
    work->baseHeightScale = parameters->heightScale;
    work->allocationHandle = allocationHandle;
    work->cellSystem = parAllocateCellSystem(work->head.cellCount, work->head.perCell, 0, 0);
    parDispatchSub(work->cellSystem, 2, work->head.dispatchArg, work->head.dispatchArg);
    func_00164C68(work->cellSystem, work->head.systemParam);
    for (i = 0; i < work->head.cellCount; i++) {
        work->cells[i].delayFrames = 0;
        work->cells[i].activeFrames = 0;
        work->cells[i].color = 0;
    }
    work->tintColor = EFF_THUNDER_NEUTRAL_COLOR;
    return work;
}

/* Tear down the indexed vector variant: system first, containing allocation last. */
void effThunderReleaseIndexedVectorWork(EffThunderVectorWork *work) {
    parReleaseCellSystem((u32)work->cellSystem);
    sdfReleaseResourceAllocation(work->allocationHandle);
}

/* Create indexed vector work from the first packed parameter block; ignore its result. */
void effThunderCreateWorkFromPackedParams(void *parameterTable) {
    void *parameters;

    parameters = effParamTableGetBlock(parameterTable, EFF_THUNDER_PARAMETER_BLOCK);
    effThunderWorkCreate(parameters);
}

/* Direct parameter-pointer entry for indexed vector creation; ignore its result. */
void func_0016BC78(void *parameters) {
    effThunderWorkCreate(parameters);
}

/* vu0 routine: copy the entire quadword, including its fourth component. */
void func_0016BC90(void *destination, void *source) {
    PCP_COPY_VECTOR(destination, source);
}

/* Set the shared packed modulation color, not an individual cell's sampled color. */
void effThunderSetIndexedVectorTint(EffThunderVectorWork *work, u32 tintColor) {
    work->tintColor = tintColor;
}

/* Absolute radius/height scaling for the indexed variant; repeated calls do not compound. */
void effThunderScaleIndexedVectorDimensions(f32 factor, EffThunderVectorWork *work) {
    work->head.radiusScale = work->baseRadiusScale * factor;
    work->head.heightScale = work->baseHeightScale * factor;
}

/* Preserve the caller's word unchanged; its wider callback role is unknown. */
u32 func_0016BCC8(u32 value) {
    return value;
}

/* Indexed-variant resampling follows the same native RNG order as the first variant.
   X/Z are normalized before Y is assigned; the fourth scratch lane is not initialized. */
void effThunderRestartIndexedCell(EffThunderVectorWork *work, s32 index) {
    EffThunderVectorCell *cell = work->cells + index;
    f32 scratchVector[EFF_THUNDER_VECTOR_COMPONENTS];

    cell->delayFrames = effMiscRand(D_003AA868) % work->head.startDelayRange;
    cell->activeFrames = effMiscRand(D_003AA868) % work->head.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
    scratchVector[0] = (effMiscRandUnitFloat(D_003AA868) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN;
    scratchVector[1] = 0;
    scratchVector[2] = (effMiscRandUnitFloat(D_003AA868) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN;
    VU0_LOAD_VF(vf10, scratchVector);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, scratchVector);
    scratchVector[1] = work->head.heightScale * EFF_THUNDER_HALF_SCALE * ((effMiscRandUnitFloat(D_003AA868) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN);
    cell->placementVector[0] = scratchVector[0];
    cell->placementVector[1] = scratchVector[1];
    cell->placementVector[2] = scratchVector[2];
    scratchVector[0] = (effMiscRandUnitFloat(D_003AA868) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_AXIS_JITTER;
    scratchVector[1] = 1.0f;
    scratchVector[2] = (effMiscRandUnitFloat(D_003AA868) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_AXIS_JITTER;
    VU0_LOAD_VF(vf10, scratchVector);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, scratchVector);
    cell->rotationAxis[0] = scratchVector[0];
    cell->rotationAxis[1] = scratchVector[1];
    cell->rotationAxis[2] = scratchVector[2];
    cell->rotationScale = work->head.rotationScale * ((effMiscRandUnitFloat(D_003AA868) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_ROTATION_JITTER + 1.0f);
    cell->radius = work->head.radiusScale * EFF_THUNDER_HALF_SCALE * ((effMiscRandUnitFloat(D_003AA868) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_RADIUS_JITTER + 1.0f);
    cell->color = EFF_THUNDER_NEUTRAL_COLOR;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016BF08);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016C1F8);

extern void func_0016BF08(EffThunderVectorWork *, s32);
extern void func_0016C1F8(EffThunderVectorWork *, s32);

/* Indexed vector countdown/fade/restart with shared tint and native render-cell stride.
   Fade is unsigned wrap-add; submission is unconditional after the signed-count loop. */
void effThunderUpdateIndexedVectorCells(EffThunderVectorWork *work) {
    s32 i = 0;
    EffThunderParSystem *renderSystem = work->cellSystem;
    s32 cellCount = work->head.cellCount;
    u32 tintColor = work->tintColor;
    EffThunderVectorCell *cell = work->cells;
    EffThunderParCell *renderCells = renderSystem->cells;
    u32 *renderColor;

    if (cellCount > 0) {
        renderColor = &renderCells->color;
        do {
            if (cell->delayFrames == 0) {
                if (cell->activeFrames != 0) {
                    func_0016BF08(work, i);
                    func_0016C1F8(work, i);
                    cell->activeFrames--;
                } else if (cell->color & EFF_THUNDER_ALPHA_MASK) {
                    cell->color += EFF_THUNDER_ALPHA_WRAP_ADD;
                    func_0016C1F8(work, i);
                } else {
                    effThunderRestartIndexedCell(work, i);
                    parCellInit(work->cellSystem, i);
                }
            } else {
                cell->delayFrames--;
            }
            *renderColor = effMultiplyPackedColors(cell->color, tintColor);
            i++;
            cell++;
            renderColor = (u32 *)((u8 *)renderColor + sizeof(EffThunderParCell));
        } while (i < cellCount);
    }
    parPrependCellNode(work->cellSystem);
}

/* Spark parameter prefix (0xA4 bytes); updates also rewrite its position vectors. */
typedef struct {
    u8 pad00[0x10];
    f32 loweredPosition[3]; /* 0x10 spark position with the height offset removed */
    u8 pad1C[4];
    f32 position[3];    /* 0x20 spark position */
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
    s32 startDelaySpread; /* 0x74 modulus of the initial negative age */
    s32 fadeInTime;     /* 0x78 initial-age blend duration */
    s32 fadeOutTime;    /* 0x7C remaining-lifetime blend duration */
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
    EffThunderSpark *sparks; /* 0xA4 */
    u32 tintColor;      /* 0xA8 multiplies the faded spark color */
    u32 allocationHandle; /* 0xAC */
} EffThunderSparkWork; /* 0xB0 */

extern void effThunderSparkInit(EffThunderSparkWork *work, s32 index);

/* Allocate spark state after the work and a separate one-cell system per spark.
   Clamp delay spread only in the copied head; sample motion before initial age. */
EffThunderSparkWork *effThunderSparkCreate(EffThunderSparkParams *parameters) {
    u32 allocationHandle = sdfAllocGeneralBlock(parameters->sparkCount * sizeof(EffThunderSpark) + sizeof(EffThunderSparkWork));
    EffThunderSparkWork *work = (EffThunderSparkWork *)sdfResourceRetainAddress(allocationHandle);
    s32 delaySpread;
    u32 sparkIndex;

    work->head = *parameters;
    work->sparks = (EffThunderSpark *)(work + 1);
    work->tintColor = EFF_THUNDER_NEUTRAL_COLOR;
    work->allocationHandle = allocationHandle;
    if (work->head.startDelaySpread <= 0) {
        work->head.startDelaySpread = EFF_THUNDER_MIN_DELAY_SPREAD;
    }
    delaySpread = work->head.startDelaySpread;
    for (sparkIndex = 0; sparkIndex < work->head.sparkCount; sparkIndex++) {
        work->sparks[sparkIndex].systemHandle = (u32)parAllocateCellSystem(EFF_THUNDER_SINGLE_CELL, work->head.halfLife * 2 - 1, 0, 0);
        parDispatchSub((void *)work->sparks[sparkIndex].systemHandle, 2, work->head.dispatchArg, work->head.dispatchArg);
        func_00164C68((void *)work->sparks[sparkIndex].systemHandle, work->head.systemParam);
        effThunderSparkInit(work, sparkIndex);
        work->sparks[sparkIndex].age = -(effMiscRand(D_003AA868) % delaySpread);
    }
    return work;
}

/* Release every spark's system before freeing the owner, using the native signed count. */
void effThunderReleaseSparkWork(EffThunderSparkWork *work) {
    s32 sparkCount = (s32)work->head.sparkCount;
    s32 sparkIndex = 0;

    if (sparkCount > 0) {
        do {
            parReleaseCellSystem(work->sparks[sparkIndex].systemHandle);
            sparkIndex++;
        } while (sparkIndex < sparkCount);
    }
    sdfReleaseResourceAllocation(work->allocationHandle);
}

/* vu0 routine: copy one whole quadword, including its fourth component. */
void effThunderCopySparkVector(void *destination, void *source) {
    PCP_COPY_VECTOR(destination, source);
}

/* Set the spark work's packed modulation color, separate from the age fade. */
void effThunderSetSparkColor(EffThunderSparkWork *work, u32 tintColor) {
    work->tintColor = tintColor;
}

/* Sample orbit radius, angular/vertical speeds and initial height offset.
   This resets motion only: age and the cell-system handle remain untouched. */
void effThunderSparkInit(EffThunderSparkWork *work, s32 index) {
    EffThunderSpark *spark = work->sparks + index;
    f32 randomness;

    spark->verticalOffset = 0;
    randomness = work->head.verticalSpeedRandomness;
    spark->verticalSpeed = work->head.verticalSpeed * (effMiscRandUnitFloat(D_003AA868) * randomness + (1.0f - randomness));
    spark->heightOffset = work->head.heightOffsetRange * effMiscRandUnitFloat(D_003AA868);
    spark->orbitAngle = effMiscRandUnitFloat(D_003AA868) * (EFF_THUNDER_HALF_TURN * 2.0f);
    randomness = work->head.angularSpeedRandomness;
    spark->angularSpeed = work->head.angularSpeed * (effMiscRandUnitFloat(D_003AA868) * randomness + (1.0f - randomness));
    randomness = work->head.radiusRandomness;
    spark->orbitRadius = work->head.orbitRadius * (effMiscRandUnitFloat(D_003AA868) * randomness + (1.0f - randomness));
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016C800);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016CD68);

extern void parRiseFallSymmetricCellAlpha(void *system, u32 a, u32 b, u32 c);

/* Parameter head (0x54 bytes) of the fragment effect, copied verbatim into the work. */
typedef struct {
    f32 start[4];
    f32 end[4];
    u16 systemParam;         /* 0x20 */
    u8 pad22[2];
    u32 fragmentCount;       /* 0x24 */
    u8 pad28[8];
    u32 startDelayRange;     /* 0x30 modulus of delayFrames */
    u32 activeFrameRange;    /* 0x34 modulus of activeFrames before adding one */
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
    u32 allocationHandle;    /* 0x64 */
} EffThunderFragmentWork; /* 0x68 */

extern void effThunderRandomizeFrag(EffThunderFragmentWork *work, s32 index);

/* Create the single-system fragment work and seed every fragment's timing/color.
   The native history-count expression and unchecked ranges are retained. */
EffThunderFragmentWork *effThunderFragCreate(EffThunderFragmentParams *parameters) {
    u32 allocationHandle = sdfAllocGeneralBlock(parameters->fragmentCount * sizeof(EffThunderFrag) + sizeof(EffThunderFragmentWork));
    EffThunderFragmentWork *work = (EffThunderFragmentWork *)sdfResourceRetainAddress(allocationHandle);
    u32 i;

    work->head = *parameters;
    work->fragments = (EffThunderFrag *)(work + 1);
    work->allocationHandle = allocationHandle;
    work->system = parAllocateCellSystem(work->head.fragmentCount, work->head.halfLife * 2 - 1, 0, 4);
    parRiseFallSymmetricCellAlpha(work->system, work->head.arg40, work->head.arg48, work->head.arg50);
    func_00164C68(work->system, work->head.systemParam);
    for (i = 0; i < work->head.fragmentCount; i++) {
        effThunderRandomizeFrag(work, i);
    }
    work->state.updateCount = 0;
    work->color = EFF_THUNDER_NEUTRAL_COLOR;
    return work;
}

/* Release the single fragment system before its containing work allocation. */
void effThunderReleaseFragmentWork(EffThunderFragmentWork *work) {
    parReleaseCellSystem((u32)work->system);
    sdfReleaseResourceAllocation(work->allocationHandle);
}

/* vu0 routine: replace the second endpoint and translate the first by its delta.
   The native store/load order and all four components, including W, are retained. */
void effThunderShiftOriginByVectorDelta(u8 *endpoints, void *anchor) {
        VU0_LOAD_VF(vf10, endpoints + EFF_THUNDER_ENDPOINT_VECTOR_BYTES);
        VU0_LOAD_VF(vf11, anchor);
        VU0_STORE_VF(vf11, endpoints + EFF_THUNDER_ENDPOINT_VECTOR_BYTES);
        VU0_SUB(vf11, vf11, vf10);
        VU0_LOAD_VF(vf10, endpoints);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, endpoints);
}

/* Set the fragment work's shared packed color word. */
void effThunderSetFragmentColor(EffThunderFragmentWork *work, u32 color) {
    work->color = color;
}

/* Preserve the caller's word unchanged; its wider callback role is unknown. */
u32 func_0016D290(u32 value) {
    return value;
}

/* Forward the system and its opaque fragment configuration through the native call. */
void func_0016D298(EffThunderFragmentWork *work) {
    parDecreaseSymmetricCellAlpha((u32)work->system, work->head.arg40, (void *)work->head.arg48, work->head.arg50);
}

/* Alternate native operation on the same system and opaque fragment configuration. */
void func_0016D2C0(EffThunderFragmentWork *work) {
    parIncreaseSymmetricCellAlpha((u32)work->system, work->head.arg40, (void *)work->head.arg48, work->head.arg50);
}

/* Apply symmetric cell-color bands using the fragment configuration's native arguments. */
void effThunderApplyFragmentColorBands(EffThunderFragmentWork *work) {
    parFillSymmetricCellColors((u32)work->system, work->head.arg40, (void *)work->head.arg48, work->head.arg50);
}

/* Resample delay/active countdowns and restore the fixed fragment grey.
   Moduli are unchecked; retain the native address-of-array RNG argument. */
void effThunderRandomizeFrag(EffThunderFragmentWork *work, s32 index) {
    EffThunderFrag *fragment = &work->fragments[index];

    fragment->delayFrames = effMiscRand(&D_003AA868) % work->head.startDelayRange;
    fragment->activeFrames = effMiscRand(&D_003AA868) % work->head.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
    fragment->color = EFF_THUNDER_FRAGMENT_GREY;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016D3B0);



INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016D9D8);
extern void effThunderRandomizeFrag2(EffThunderFragmentWork *work, s32 index);

/* Allocate dual-system fragment work, creating the secondary system before the primary.
   Both systems share one fragment-state array; keep native dispatch/allocation order. */
EffThunderFragmentWork *func_0016DB28(EffThunderFragmentParams *parameters) {
    u32 allocationHandle = sdfAllocGeneralBlock(parameters->fragmentCount * sizeof(EffThunderFrag) + sizeof(EffThunderFragmentWork));
    EffThunderFragmentWork *work = (EffThunderFragmentWork *)sdfResourceRetainAddress(allocationHandle);
    u32 i;

    work->head = *parameters;
    work->fragments = (EffThunderFrag *)(work + 1);
    work->allocationHandle = allocationHandle;
    work->state.secondarySystem = parAllocateCellSystem(work->head.fragmentCount, work->head.halfLife * 2 - 1, 0, 1);
    parDispatchSub(work->state.secondarySystem, 2, work->head.arg48, work->head.arg50);
    func_00164C68(work->state.secondarySystem, work->head.systemParam);
    work->system = parAllocateCellSystem(work->head.fragmentCount, work->head.halfLife * 2 - 1, 0, 0);
    parDispatchSub(work->system, 2, work->head.arg40, work->head.arg40);
    func_00164C68(work->system, work->head.systemParam);
    for (i = 0; i < work->head.fragmentCount; i++) {
        effThunderRandomizeFrag2(work, i);
    }
    work->color = EFF_THUNDER_NEUTRAL_COLOR;
    return work;
}

/* Release secondary then primary systems before freeing the shared work allocation. */
void effThunderReleaseDualFragmentWork(EffThunderFragmentWork *work) {
    parReleaseCellSystem((u32)work->state.secondarySystem);
    parReleaseCellSystem((u32)work->system);
    sdfReleaseResourceAllocation(work->allocationHandle);
}

/* vu0 routine: replace the second endpoint and translate the first by its delta.
   The native store/load order and all four components, including W, are retained. */
void effThunderShiftEndpointsWithAnchor(u8 *endpoints, void *anchor) {
        VU0_LOAD_VF(vf10, endpoints + EFF_THUNDER_ENDPOINT_VECTOR_BYTES);
        VU0_LOAD_VF(vf11, anchor);
        VU0_STORE_VF(vf11, endpoints + EFF_THUNDER_ENDPOINT_VECTOR_BYTES);
        VU0_SUB(vf11, vf11, vf10);
        VU0_LOAD_VF(vf10, endpoints);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, endpoints);
}

/* Set the shared packed modulation color used by both fragment systems. */
void effThunderSetDualFragmentColor(EffThunderFragmentWork *work, u32 color) {
    work->color = color;
}

/* Seed the dual variant's shared fragment timing/color; both moduli remain unchecked. */
void effThunderRandomizeFrag2(EffThunderFragmentWork *work, s32 index) {
    EffThunderFrag *fragment = &work->fragments[index];

    fragment->delayFrames = effMiscRand(&D_003AA868) % work->head.startDelayRange;
    fragment->activeFrames = effMiscRand(&D_003AA868) % work->head.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
    fragment->color = EFF_THUNDER_FRAGMENT_GREY;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016DE30);

extern void func_0016DE30(EffThunderFragmentWork *, s32);

/* One countdown/fade/restart state drives both systems; write the same tinted color to each.
   Secondary initialization/submission precedes primary; submission also occurs for count <= 0. */
void effThunderUpdateDualFragments(EffThunderFragmentWork *work) {
    s32 index = 0;
    EffThunderParSystem *secondarySystem = work->state.secondarySystem;
    s32 fragmentCount = work->head.fragmentCount;
    EffThunderParSystem *primarySystem = work->system;
    EffThunderParCell *primaryCells = primarySystem->cells;
    u32 tintColor = work->color;
    EffThunderFrag *fragment = work->fragments;
    EffThunderParCell *secondaryCells = secondarySystem->cells;
    s32 packedColor;

    if (fragmentCount > 0) {
        do {
            if (fragment->delayFrames == 0) {
                if (fragment->activeFrames != 0) {
                    func_0016DE30(work, index);
                    fragment->activeFrames--;
                } else if (fragment->color & EFF_THUNDER_ALPHA_MASK) {
                    fragment->color -= EFF_THUNDER_ALPHA_FADE_STEP;
                } else {
                    effThunderRandomizeFrag2(work, index);
                    parCellInit(work->state.secondarySystem, index);
                    parCellInit(work->system, index);
                }
            } else {
                fragment->delayFrames--;
            }
            packedColor = effMultiplyPackedColors(fragment->color, tintColor);
            secondaryCells[index].color = packedColor;
            primaryCells[index].color = packedColor;
            fragment++;
            index++;
        } while (index < fragmentCount);
    }
    parPrependCellNode(work->state.secondarySystem);
    parPrependCellNode(work->system);
}

extern void parDecreaseStripCellAlpha(void *system, u32 a, u32 b, u32 c);

/* Parameter head (0x48 bytes) of the cell effect, copied verbatim into the work. */
typedef struct {
    u8 pad00[0x10];
    u16 systemParam;         /* 0x10 */
    u8 pad12[6];
    u32 cellCount;           /* 0x18 */
    u8 pad1C[8];
    u32 startDelayRange;     /* 0x24 modulus of delayFrames */
    u32 activeFrameRange;    /* 0x28 modulus of activeFrames before adding one */
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
    u32 allocationHandle;    /* 0x54 */
} EffThunderCellWork; /* 0x58 */

extern void effThunderRandomizeCell(EffThunderCellWork *work, s32 index);

/* Allocate cell states after the work, configure its system and sample directions/timing.
   The opaque settable word is not initialized here; no new default or range checks are added. */
EffThunderCellWork *effThunderCellCreate(EffThunderCellParams *parameters) {
    u32 allocationHandle = sdfAllocGeneralBlock(parameters->cellCount * sizeof(EffThunderCell) + sizeof(EffThunderCellWork));
    EffThunderCellWork *work = (EffThunderCellWork *)sdfResourceRetainAddress(allocationHandle);
    u32 i;

    work->head = *parameters;
    work->cells = (EffThunderCell *)(work + 1);
    work->allocationHandle = allocationHandle;
    work->system = parAllocateCellSystem(work->head.cellCount, work->head.halfLife * 2 - 1, 0, 2);
    parDecreaseStripCellAlpha(work->system, work->head.arg34, work->head.arg3C, work->head.arg44);
    func_00164C68(work->system, work->head.systemParam);
    for (i = 0; i < work->head.cellCount; i++) {
        effThunderRandomizeCell(work, i);
    }
    return work;
}

/* Release the cell system before releasing the containing work allocation. */
void effThunderReleaseCellWork(EffThunderCellWork *work) {
    parReleaseCellSystem((u32)work->system);
    sdfReleaseResourceAllocation(work->allocationHandle);
}

/* vu0 routine: copy the entire quadword, not only its XYZ components. */
void effThunderCopyCellVector(void *destination, void *source) {
    PCP_COPY_VECTOR(destination, source);
}

/* Store the opaque work word; no C consumer establishes a color/control interpretation. */
void effThunderSetCellWorkValue(EffThunderCellWork *work, u32 value) {
    work->unk4C = value;
}

/* Sample independent direction components without normalization, then timing.
   Keep the native add-to-double form and address-of-array RNG argument. */
void effThunderRandomizeCell(EffThunderCellWork *work, s32 index) {
    EffThunderCell *cell = work->cells + index;
    f32 centeredRandom;

    centeredRandom = effMiscRandUnitFloat(&D_003AA868) - EFF_THUNDER_RANDOM_MIDPOINT;
    cell->directionX = centeredRandom + centeredRandom;
    centeredRandom = effMiscRandUnitFloat(&D_003AA868) - EFF_THUNDER_RANDOM_MIDPOINT;
    cell->directionY = centeredRandom + centeredRandom;
    centeredRandom = effMiscRandUnitFloat(&D_003AA868) - EFF_THUNDER_RANDOM_MIDPOINT;
    cell->directionZ = centeredRandom + centeredRandom;
    cell->delayFrames = effMiscRand(&D_003AA868) % work->head.startDelayRange;
    cell->activeFrames = effMiscRand(&D_003AA868) % work->head.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016E870);

extern void func_0016E870(EffThunderCellWork *work, s32 index);

/* Delay -> active geometry -> render-cell alpha fade -> restart.
   Unlike fragment/vector state, color lives in the render cells; submit even for count <= 0. */
void effThunderCellUpdate(EffThunderCellWork *work) {
    s32 i = 0;
    s32 cellCount = work->head.cellCount;
    EffThunderCell *cell = work->cells;
    EffThunderParCell *renderCells = ((EffThunderParSystem *)work->system)->cells;

    if (cellCount > 0) {
        do {
            if (cell->delayFrames == 0) {
                if (cell->activeFrames != 0) {
                    func_0016E870(work, i);
                    cell->activeFrames--;
                } else if (renderCells[i].color & EFF_THUNDER_ALPHA_MASK) {
                    renderCells[i].color -= EFF_THUNDER_ALPHA_FADE_STEP;
                } else {
                    effThunderRandomizeCell(work, i);
                    parCellInit(work->system, i);
                }
            } else {
                cell->delayFrames--;
            }
            cell++;
            i++;
        } while (i < cellCount);
    }
    parPrependCellNode(work->system);
}



typedef struct EffectColorState {
    u32 color;
    u8 pad04[8];
    u32 valueC;
    u32 mode;
} EffectColorState;



typedef struct EffThunderGroupParams {
    f32 points[10][4];
    s32 count;
    EffThunderFragmentParams params;
} EffThunderGroupParams;

typedef struct EffThunderGroup {
    EffThunderGroupParams head;
    EffThunderFragmentWork *handles[10];
    u32 color;
} EffThunderGroup;

extern void *sdfAllocSizeClassBlock(s32);
extern void *memset(void *, s32, u32);

EffThunderGroup *effThunderChainGroupCreate(EffThunderGroupParams *src) {
    EffThunderGroup *group = sdfAllocSizeClassBlock(sizeof(EffThunderGroup));
    s32 i;

    memset(group->handles, 0, sizeof(group->handles));
    group->head = *src;
    group->handles[0] = effThunderFragCreate(&group->head.params);
    func_0016D2C0(group->handles[0]);
    for (i = 1; i < src->count - 2; i++) {
        group->handles[i] = effThunderFragCreate(&group->head.params);
        effThunderApplyFragmentColorBands(group->handles[i]);
    }
    group->handles[i] = effThunderFragCreate(&group->head.params);
    func_0016D298(group->handles[i]);
    group->color = 0x80808080;
    return group;
}



extern void func_0016D3B0(void *work, s32 index);
extern void func_0016F028(void *work, s32 index, void *seed);
extern void parPrependCellNode(void *system);

extern void sdfReleaseChipBlock(void *block);

void effThunderGroupRelease(EffThunderGroup *group) {
    s32 i;

    for (i = 0; i < group->head.count - 1; i++) {
        effThunderReleaseFragmentWork(group->handles[i]);
    }
    sdfReleaseChipBlock(group);
}

u32 func_0016F018(u32 arg0) {
    return arg0;
}

void effSetResourceBlendColor(s32 work, u32 value) {
    *(u32 *)(work + 0x120) = value;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016F028);

void effThunderUpdateChainSegments(EffThunderGroup *group) {
    s32 i = 0;
    s32 segmentCount = group->head.count - 1;
    s32 j;
    s32 fragmentCount;
    u32 color;
    f32 (*points)[4] = group->head.points;
    EffThunderFragmentWork *work;
    EffThunderFragmentWork *previous;
    EffThunderParCell *cell;

    for (; i < segmentCount; i++) {
        work = group->handles[i];
        PCP_COPY_VECTOR(work->head.start, points[0]);
        PCP_COPY_VECTOR(work->head.end, points[1]);
        points++;
        fragmentCount = work->head.fragmentCount;
        color = effMultiplyPackedColors(group->color, work->color);
        cell = ((EffThunderParSystem *)work->system)->cells;
        for (j = 0; j < fragmentCount; j++) {
            if (i > 0) {
                previous = group->handles[i - 1];
                func_0016F028(work, j,
                    ((EffThunderParSystem *)previous->system)->cells[j].history + ((EffThunderParSystem *)previous->system)->cells[j].vertexCount - 5);
            } else {
                func_0016D3B0(work, j);
            }
            cell[j].color = color;
        }
        work->state.updateCount++;
        parPrependCellNode(work->system);
    }
}

/* Point history and its resource/allocation ownership share one header. */
typedef struct EffFragmentResources {
    u8 pad00[8];
    s32 count;
    s32 activePointCount;
    s32 position;
    u8 pad14[4];
    u128 *points;
    u32 *colors;
    u32 resourceHandle;
    u32 allocation;
} EffFragmentResources;

/* The native Bezier update advances the two floating states at +0x74/+0x78. */
typedef struct EffGroupSlot {
    u8 pad00[0x10];
    s32 age;
    f32 scale;
    EffFragmentResources *resources;
    u8 pad1C[0x54];
    u32 unk70;
    f32 unk74;
    f32 unk78;
    void *node;
} EffGroupSlot;

typedef struct EffGroupParams {
    u8 pad00[0x20];
    u32 count;
    u8 pad24[4];
    s32 unk28;
    u8 pad2C[0xC];
    s32 unk38;
    s32 unk3C;
    u32 palette[4];
} EffGroupParams;

/* Kind-two placement is 0x30 bytes; its four palette colors are separate. */
typedef struct EffPCPEventPlace {
    f32 unk00[7];
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    u32 color;
} EffPCPEventPlace;

typedef struct EffPCPEventOwner EffPCPEventOwner;

typedef struct EffGroup {
    EffGroupParams params;
    EffGroupSlot *slots;
    u32 color;
    EffPCPEventOwner *owner;
    u8 hasHandle58;
    u8 pad5D[3];
    u32 allocation;
} EffGroup;

extern u32 sdfAllocGeneralBlock(s32 size);
extern u8 *sdfResourceRetainAddress(u32 handle);
extern u32 func_00197D38();
extern EffFragmentResources *func_00171598(s32, s32);
extern void func_001717E8(EffFragmentResources *, u32 *);
extern void *effEventCreate(u32, u16, EffPCPEventPlace *);

EffGroup *func_0016F850(src, eventParams)
EffGroup *src;
void *eventParams;
{
    u32 count = src->params.count;
    u32 allocation = sdfAllocGeneralBlock(count * sizeof(EffGroupSlot) + sizeof(EffGroup));
    EffGroup *work = (EffGroup *)sdfResourceRetainAddress(allocation);
    EffGroupSlot *slot = (EffGroupSlot *)(work + 1);
    EffPCPEventPlace place;
    u32 palette[4];
    s32 a, b;
    u32 i;

    work->params = src->params;
    work->allocation = allocation;
    work->hasHandle58 = 1;
    work->slots = slot;
    work->color = 0x80808080;
    work->owner = (EffPCPEventOwner *)func_00197D38(eventParams);
    a = work->params.unk38;
    if (a == 0) {
        work->params.unk38 = 1;
        a = 1;
    }
    b = work->params.unk3C;
    if (b <= 0) {
        work->params.unk3C = 1;
        b = 1;
    }
    if (work->params.unk28 <= 0) {
        work->params.unk28 = 1;
    }
    palette[0] = work->params.palette[0];
    palette[1] = work->params.palette[1];
    palette[2] = work->params.palette[2];
    palette[3] = work->params.palette[3];
    place.unk00[0] = 0;
    place.unk00[1] = 0;
    place.unk00[2] = 0;
    place.unk00[3] = 0;
    place.unk00[4] = 0;
    place.unk00[5] = 0;
    place.unk00[6] = 0;
    place.unk1C = 1.0f;
    place.unk20 = 100.0f;
    place.unk24 = 100.0f;
    place.unk28 = 1.0f;
    place.color = 0x80808080;
    for (i = 0; i < count; i++, slot++) {
        slot->resources = func_00171598(a, b);
        func_001717E8(slot->resources, palette);
        slot->age = 0;
        slot->scale = 1.0f;
        slot->unk70 = 0;
        slot->unk74 = 0;
        slot->unk78 = 0;
        slot->node = effEventCreate((u32)work->owner, 2, &place);
    }
    return work;
}

/* Forward the first two parameter-table blocks as one effect-handler pair. */
void effApplyParamBlockPair(void *table) {
    void *firstBlock;
    void *secondBlock;

    firstBlock = effParamTableGetBlock(table, 0);
    secondBlock = effParamTableGetBlock(table, 1);
    func_0016F850(firstBlock, secondBlock);
}

EffGroup *func_0016FB18(EffGroup *src) {
    u32 count = src->params.count;
    u32 allocation = sdfAllocGeneralBlock(count * sizeof(EffGroupSlot) + sizeof(EffGroup));
    EffGroup *work = (EffGroup *)sdfResourceRetainAddress(allocation);
    EffGroupSlot *slot = (EffGroupSlot *)(work + 1);
    EffPCPEventPlace place;
    u32 palette[4];
    s32 a, b;
    u32 i;

    work->params = src->params;
    work->hasHandle58 = 0;
    work->allocation = allocation;
    work->owner = src->owner;
    work->slots = slot;
    work->color = 0x80808080;
    a = work->params.unk38;
    if (a == 0) {
        work->params.unk38 = 1;
        a = 1;
    }
    b = work->params.unk3C;
    if (b <= 0) {
        work->params.unk3C = 1;
        b = 1;
    }
    if (work->params.unk28 <= 0) {
        work->params.unk28 = 1;
    }
    palette[0] = work->params.palette[0];
    palette[1] = work->params.palette[1];
    palette[2] = work->params.palette[2];
    palette[3] = work->params.palette[3];
    place.unk00[0] = 0;
    place.unk00[1] = 0;
    place.unk00[2] = 0;
    place.unk00[3] = 0;
    place.unk00[4] = 0;
    place.unk00[5] = 0;
    place.unk00[6] = 0;
    place.unk1C = 1.0f;
    place.unk20 = 100.0f;
    place.unk24 = 100.0f;
    place.unk28 = 1.0f;
    place.color = 0x80808080;
    for (i = 0; i < count; i++, slot++) {
        slot->resources = func_00171598(a, b);
        func_001717E8(slot->resources, palette);
        slot->age = 0;
        slot->scale = 1.0f;
        slot->unk70 = 0;
        slot->unk74 = 0;
        slot->unk78 = 0;
        slot->node = effEventCreate((u32)work->owner, 2, &place);
    }
    return work;
}


extern void effReleaseEffectResources(EffFragmentResources *work);
extern void effEventReleaseNode(void *node);
extern void func_00197D50(u32 handle);
extern void sdfReleaseResourceAllocation(u32 allocation);

/* Release every slot's effect resources and event node, then the optional handle and the group allocation. */
void effReleaseGroupSlotsAndResources(EffGroup *group) {
    u32 i = 0;
    u32 count = group->params.count;
    EffGroupSlot *slot = group->slots;

    if (count != 0) {
        do {
            i++;
            effReleaseEffectResources(slot->resources);
            effEventReleaseNode(slot->node);
            slot++;
        } while (i < count);
    }
    if (group->hasHandle58 != 0) {
        func_00197D50((u32)group->owner);
    }
    sdfReleaseResourceAllocation(group->allocation);
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016FE18);

void effCopyResourceGroupVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00171590(s32 work, u32 value) {
    *(u32 *)(work + 0x54) = value;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00171598);

void effReleaseEffectResources(EffFragmentResources *work) {
    sdfQueueAssetRelease(work->resourceHandle);
    sdfReleaseResourceAllocation(work->allocation);
}

/* Restore the neutral gray color and default effect mode before rendering. */
void effInitializeColorState(EffectColorState *state) {
    state->mode = 3;
    state->color = 0x80808080;
    state->valueC = 0;
}


extern u32 effBlendColor(u32 colorA, u32 colorB, f32 t);

void func_001717E8(EffFragmentResources *history, u32 *gradientColors) {
    f32 t = 0.0f;
    u32 count = history->count / 3;
    u32 alphaCount = count >> 1;
    f32 step = 1.0f / count;
    f32 alpha = 0.0f;
    f32 alphaStep = 1.0f / alphaCount;
    u32 *colors = history->colors;
    u32 innerStart = gradientColors[0] & 0xFFFFFF;
    u32 innerEnd = gradientColors[2] & 0xFFFFFF;
    u32 outerStart = gradientColors[1];
    u32 outerEnd = gradientColors[3];
    u32 i = 0;

    if (count != 0) {
        do {
            u32 alphaMask = (u32)(alpha * 2147483648.0f) & 0xFF000000;

            colors[0] = effBlendColor(outerEnd, outerStart, t);
            colors[1] = effBlendColor(innerEnd, innerStart, t) | alphaMask;
            colors[2] = colors[0];
            t += step;
            if (i < alphaCount) {
                alpha += alphaStep;
            }
            i++;
            colors += 3;
        } while (i < count);
    }
}

void func_001719D0(EffFragmentResources *history, u128 *source) {
    s32 position = history->position;
    u128 *points = history->points;
    s32 count;

    PCP_COPY_VECTOR(&points[position], source);
    PCP_COPY_VECTOR(&points[position + 1], source + 1);
    PCP_COPY_VECTOR(&points[position + 2], source + 2);
    position += 3;
    count = history->count;
    history->position = position;
    if (position == count) {
        PCP_COPY_VECTOR(&points[0], source);
        PCP_COPY_VECTOR(&points[1], source + 1);
        PCP_COPY_VECTOR(&points[2], source + 2);
        history->position = 3;
    }
    if (history->activePointCount < count - 3) {
        history->activePointCount += 3;
    }
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00171A68);

typedef struct EffFlashRecordPart {
    u32 unk00;
    s32 age; /* 0x04 */
    u8 pad08[8];
} EffFlashRecordPart; /* 0x10 */

typedef struct EffFlashRecordHandle {
    u8 pad00[0x50];
    u32 unk50;
} EffFlashRecordHandle;

typedef struct EffFlashRecordWork {
    u8 pad00[0x10];
    u32 particleCount;    /* 0x10 */
    u8 pad14[0x18];
    u32 unk2C;            /* 0x2C */
    EffFlashRecordPart *parts; /* 0x30 */
    u32 updateCount;      /* 0x34 */
    u32 colorParam;       /* 0x38 */
    f32 renderScale;      /* 0x3C */
    u32 ownedBuffer;      /* 0x40 */
    u32 resourceHandle;   /* 0x44 */
} EffFlashRecordWork; /* 0x48 */

extern void *memcpy(void *dst, const void *src, u32 size);
extern struct EffRecordPool *effRecordPoolCreateTriple(s32 count);

/* Clone the 0x30-byte parameter block, create the record pool and clear every particle's age. */
EffFlashRecordWork *effFlashRecordCreate(src)
    EffFlashRecordWork *src;
{
    u32 handle = sdfAllocGeneralBlock(src->particleCount * sizeof(EffFlashRecordPart) + sizeof(EffFlashRecordWork));
    EffFlashRecordWork *work = (EffFlashRecordWork *)sdfResourceRetainAddress(handle);
    EffFlashRecordHandle *record;
    u32 i;

    memcpy(work, src, 0x30);
    work->parts = (EffFlashRecordPart *)(work + 1);
    work->colorParam = 0x80808080;
    work->ownedBuffer = handle;
    work->renderScale = 1.0f;
    work->updateCount = 0;
    record = (EffFlashRecordHandle *)effRecordPoolCreateTriple(work->particleCount);
    work->resourceHandle = (u32)record;
    record->unk50 = work->unk2C;
    for (i = 0; i < work->particleCount; i++) {
        work->parts[i].age = 0;
    }
    return work;
}
