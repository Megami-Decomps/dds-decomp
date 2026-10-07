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
extern void sdfReleaseResourceAllocation(SdfMemBlock *allocation);
extern u32 effMiscRand(void *state);
extern f32 effMiscRandUnitFloat(void *state);
extern u8 D_0034DF38[];

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
    f32 origin[4];     /* 0x00 complete origin quadword */
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
    u8 pad32[0xA];
    f32 bandWidth;     /* 0x3C */
    void *dispatchArg;  /* 0x40 */
    f32 edgeWidth;     /* 0x44 */
    u8 pad48[4];
} EffThunderVectorParams;
typedef char EffThunderVectorParamsSizeCheck[sizeof(EffThunderVectorParams) == 0x4C ? 1 : -1];

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
typedef struct EffThunderVectorWork {
    EffThunderVectorParams head;
    EffThunderVectorCell *cells; /* 0x4C */
    u32 tintColor;      /* 0x50 multiplies each cell's sampled/faded color */
    f32 baseRadiusScale; /* 0x54 retained for absolute scale callbacks */
    f32 baseHeightScale; /* 0x58 retained for absolute scale callbacks */
    void *cellSystem;   /* 0x5C */
    SdfMemBlock *allocationHandle; /* 0x60: allocation descriptor */
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
void func_001634C0(void *parameters) {
    effCreateThunderCellSystemWork(parameters);
}

/* Release the cell system before releasing the containing work allocation. */
void effThunderReleaseVectorWork(EffThunderVectorWork *work) {
    parReleaseCellSystem((u32)work->cellSystem);
    sdfReleaseResourceAllocation(work->allocationHandle);
}

/* vu0 routine: copy the entire quadword, including its fourth component. */
void func_00163508(void *destination, void *source) {
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
u32 func_00163540(u32 value) {
    return value;
}

extern SdfMemBlock *sdfAllocGeneralBlock(s32 size);
extern u32 sdfResourceRetainAddress(SdfMemBlock *allocation);
extern void *parAllocateCellSystem(s32 count, s32 perCell, s32 groupDivisor, u32 kind);
extern void func_0015D078(void *system, u32 value);
extern void parDispatchSub(void *work, s32 sub, void *a2, void *a3);
extern void parPrependCellNode(void *system);
extern void parCellInit(void *system, s32 index);
extern s32 effMultiplyPackedColors(s32 color, s32 param);


typedef struct {
    u8 pad00[8];
    s32 vertexCount;    /* 0x08 quadword vector capacity per cell */
    u8 pad0C[8];
    ParCell *cells; /* 0x14 */
} EffThunderParSystem;

/* Resample timing, normalize X/Z before assigning Y, then sample the axis.
   Both timing moduli are unchecked; the fourth scratch lane is not initialized. */
void effThunderCellRestart(EffThunderVectorWork *work, s32 index) {
    EffThunderVectorCell *cell = work->cells + index;
    f32 scratchVector[EFF_THUNDER_VECTOR_COMPONENTS];

    cell->delayFrames = effMiscRand(D_0034DF38) % work->head.startDelayRange;
    cell->activeFrames = effMiscRand(D_0034DF38) % work->head.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
    scratchVector[0] = (effMiscRandUnitFloat(D_0034DF38) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN;
    scratchVector[1] = 0;
    scratchVector[2] = (effMiscRandUnitFloat(D_0034DF38) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN;
    VU0_LOAD_VF(vf10, scratchVector);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, scratchVector);
    scratchVector[1] = work->head.heightScale * EFF_THUNDER_HALF_SCALE * ((effMiscRandUnitFloat(D_0034DF38) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN);
    cell->placementVector[0] = scratchVector[0];
    cell->placementVector[1] = scratchVector[1];
    cell->placementVector[2] = scratchVector[2];
    scratchVector[0] = (effMiscRandUnitFloat(D_0034DF38) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_AXIS_JITTER;
    scratchVector[1] = 1.0f;
    scratchVector[2] = (effMiscRandUnitFloat(D_0034DF38) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_AXIS_JITTER;
    VU0_LOAD_VF(vf10, scratchVector);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, scratchVector);
    cell->rotationAxis[0] = scratchVector[0];
    cell->rotationAxis[1] = scratchVector[1];
    cell->rotationAxis[2] = scratchVector[2];
    cell->rotationScale = work->head.rotationScale * ((effMiscRandUnitFloat(D_0034DF38) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_ROTATION_JITTER + 1.0f);
    cell->radius = work->head.radiusScale * EFF_THUNDER_HALF_SCALE * ((effMiscRandUnitFloat(D_0034DF38) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_RADIUS_JITTER + 1.0f);
    cell->color = EFF_THUNDER_NEUTRAL_COLOR;
}

struct RwV3d;
extern f32 sdfViewEyeVector[4], sdfViewTargetVector[4];
extern void vu0RotMatrixXYZFromVec3(const struct RwV3d *);
extern void sdfBuildVuRotationFromAxisAngle(const struct RwV3d *, f32);

/* Sample a width multiplier, then bend the cell's placement vector around
 * a perturbed axis while emitting five-vector rows into its kind-4 history.
 */
void func_00163780(EffThunderVectorWork *work, s32 index) {
    f32 position[4] __attribute__((aligned(16)));
    f32 placement[4] __attribute__((aligned(16)));
    f32 axis[4] __attribute__((aligned(16)));
    f32 originalAxis[4] __attribute__((aligned(16)));
    f32 side[4] __attribute__((aligned(16)));
    f32 cameraDirection[4] __attribute__((aligned(16)));
    f32 origin[4] __attribute__((aligned(16)));
    f32 width[4] __attribute__((aligned(16)));
    f32 outerWidth[4] __attribute__((aligned(16)));
    f32 rotation[4] __attribute__((aligned(16)));
    EffThunderParSystem *system = work->cellSystem;
    ParCell *cell = &system->cells[index];
    EffThunderVectorCell *source;
    u128 *vertices;
    s32 count;
    s32 i;
    f32 widthScale;
    f32 rotationScale;
    f32 radius;

    count = system->vertexCount / 5;
    cell->vertexCount = system->vertexCount;
    source = &work->cells[index];
    vertices = cell->history;
    widthScale = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 0.2f + 1.0f;
    VEC3_SPLAT(width, work->head.bandWidth * widthScale);
    VEC3_SPLAT(outerWidth, width[0] + work->head.edgeWidth * widthScale);
    originalAxis[0] = source->rotationAxis[0];
    originalAxis[1] = source->rotationAxis[1];
    originalAxis[2] = source->rotationAxis[2];
    originalAxis[3] = 0.0f;
    rotationScale = -source->rotationScale;
    radius = source->radius;
    VU0_LOAD_VF(vf10, sdfViewEyeVector);
    VU0_LOAD_VF(vf11, sdfViewTargetVector);
    VU0_SUB_EXTENDED(vf10, vf10, vf11);
    VU0_STORE_VF_UNCLOBBERED(vf10, cameraDirection);
    placement[0] = source->placementVector[0];
    placement[1] = 0.0f;
    placement[2] = source->placementVector[2];
    PCP_COPY_VECTOR(origin, work->head.origin);
    origin[1] += source->placementVector[1];

    for (i = 0; i < count; i++) {
        position[0] = origin[0] + placement[0] * radius;
        position[1] = origin[1] + placement[1] * radius;
        position[2] = origin[2] + placement[2] * radius;
        PCP_COPY_VECTOR(axis, originalAxis);
        rotation[0] = effMiscRandUnitFloat(D_0034DF38) * (75.0f * (EFF_THUNDER_HALF_TURN / 180.0f));
        rotation[1] = effMiscRandUnitFloat(D_0034DF38) * (75.0f * (EFF_THUNDER_HALF_TURN / 180.0f));
        rotation[2] = effMiscRandUnitFloat(D_0034DF38) * (75.0f * (EFF_THUNDER_HALF_TURN / 180.0f));
        vu0RotMatrixXYZFromVec3((const struct RwV3d *)rotation);
        VU0_LOAD_VF(vf10, axis);
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf10, axis);
        sdfBuildVuRotationFromAxisAngle((const struct RwV3d *)axis,
            ((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 0.7f) * rotationScale + rotationScale);
        VU0_LOAD_VF(vf10, placement);
        VU0_MOVE_VF_EXTENDED(vf11, vf10);
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf10, placement);
        VU0_SUB_EXTENDED(vf10, vf10, vf11);
        VU0_LOAD_VF(vf11, cameraDirection);
        VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10_EXTENDED();
        VU0_STORE_VF_UNCLOBBERED(vf10, side);
        VU0_LOAD_VF(vf11, width);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        VU0_LOAD_VF(vf10, position);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices + 2);
        VU0_MOVE_VF_EXTENDED(vf11, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices + 1);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf11, vertices + 3);
        VU0_LOAD_VF(vf10, side);
        VU0_LOAD_VF(vf11, outerWidth);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        VU0_LOAD_VF(vf10, position);
        VU0_MOVE_VF_EXTENDED(vf11, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf11, vertices + 4);
        vertices += 5;
    }
}


/* Rotate the live placement direction and every five-vector history row
 * around the effect origin. The separate placement height is preserved.
 */
void func_00163AF8(EffThunderVectorWork *work, s32 index) {
    f32 axis[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    EffThunderParSystem *system = work->cellSystem;
    EffThunderVectorCell *source = &work->cells[index];
    ParCell *cell = &system->cells[index];
    u128 *vertices = cell->history;
    s32 rows = system->vertexCount / 5;
    s32 i;
    f32 sample;
    f32 angle;

    sample = effMiscRandUnitFloat(D_0034DF38);
    angle = source->rotationScale * sample * 3.0f;
    direction[0] = source->placementVector[0];
    direction[1] = 0.0f;
    direction[2] = source->placementVector[2];
    axis[0] = source->rotationAxis[0];
    axis[1] = source->rotationAxis[1];
    axis[2] = source->rotationAxis[2];
    sdfBuildVuRotationFromAxisAngle((const struct RwV3d *)axis, angle);
    VU0_LOAD_VF(vf10, direction);
    VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
    VU0_STORE_VF_UNCLOBBERED(vf10, direction);
    source->placementVector[0] = direction[0];
    source->placementVector[2] = direction[2];
    VU0_LOAD_VF(vf11, work->head.origin);
    for (i = 0; i < rows; i++) {
        VU0_LOAD_VF(vf10, vertices);
        VU0_SUB_EXTENDED(vf10, vf10, vf11);
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices);
        VU0_LOAD_VF(vf10, vertices + 1);
        VU0_SUB_EXTENDED(vf10, vf10, vf11);
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices + 1);
        VU0_LOAD_VF(vf10, vertices + 2);
        VU0_SUB_EXTENDED(vf10, vf10, vf11);
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices + 2);
        VU0_LOAD_VF(vf10, vertices + 3);
        VU0_SUB_EXTENDED(vf10, vf10, vf11);
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices + 3);
        VU0_LOAD_VF(vf10, vertices + 4);
        VU0_SUB_EXTENDED(vf10, vf10, vf11);
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices + 4);
        vertices += 5;
    }
}


extern void func_00163780(EffThunderVectorWork *, s32);
extern void func_00163AF8(EffThunderVectorWork *, s32);

/* Delay -> active geometry -> alpha fade -> restart; tint each render cell.
   The unsigned wrap-add is not a saturating fade. Submit even for signed count <= 0. */
void effThunderUpdateVectorCells(EffThunderVectorWork *work) {
    s32 i = 0;
    EffThunderParSystem *renderSystem = work->cellSystem;
    s32 cellCount = work->head.cellCount;
    u32 tintColor = work->tintColor;
    EffThunderVectorCell *cell = work->cells;
    ParCell *renderCells = renderSystem->cells;
    u32 *renderColor;

    if (cellCount > 0) {
        renderColor = &renderCells->color;
        do {
            if (cell->delayFrames == 0) {
                if (cell->activeFrames != 0) {
                    func_00163780(work, i);
                    func_00163AF8(work, i);
                    cell->activeFrames--;
                } else if (cell->color & EFF_THUNDER_ALPHA_MASK) {
                    cell->color += EFF_THUNDER_ALPHA_WRAP_ADD;
                    func_00163AF8(work, i);
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
            renderColor = (u32 *)((u8 *)renderColor + sizeof(ParCell));
        } while (i < cellCount);
    }
    parPrependCellNode(work->cellSystem);
}

/* Allocate one work followed by its vector cells and retain unscaled dimensions.
   Only cell countdowns/color are initialized; geometry is sampled on the first restart. */
EffThunderVectorWork *effThunderWorkCreate(EffThunderVectorParams *parameters) {
    SdfMemBlock *allocationHandle = sdfAllocGeneralBlock(parameters->cellCount * sizeof(EffThunderVectorCell) + sizeof(EffThunderVectorWork));
    EffThunderVectorWork *work = (EffThunderVectorWork *)sdfResourceRetainAddress(allocationHandle);
    u32 i;

    work->head = *parameters;
    work->cells = (EffThunderVectorCell *)(work + 1);
    work->baseRadiusScale = parameters->radiusScale;
    work->baseHeightScale = parameters->heightScale;
    work->allocationHandle = allocationHandle;
    work->cellSystem = parAllocateCellSystem(work->head.cellCount, work->head.perCell, 0, 0);
    parDispatchSub(work->cellSystem, 2, work->head.dispatchArg, work->head.dispatchArg);
    func_0015D078(work->cellSystem, work->head.systemParam);
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
void func_00164020(void *parameters) {
    effThunderWorkCreate(parameters);
}

/* vu0 routine: copy the entire quadword, including its fourth component. */
void func_00164038(void *destination, void *source) {
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
u32 func_00164070(u32 value) {
    return value;
}

/* Indexed-variant resampling follows the same native RNG order as the first variant.
   X/Z are normalized before Y is assigned; the fourth scratch lane is not initialized. */
void effThunderRestartIndexedCell(EffThunderVectorWork *work, s32 index) {
    EffThunderVectorCell *cell = work->cells + index;
    f32 scratchVector[EFF_THUNDER_VECTOR_COMPONENTS];

    cell->delayFrames = effMiscRand(D_0034DF38) % work->head.startDelayRange;
    cell->activeFrames = effMiscRand(D_0034DF38) % work->head.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
    scratchVector[0] = (effMiscRandUnitFloat(D_0034DF38) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN;
    scratchVector[1] = 0;
    scratchVector[2] = (effMiscRandUnitFloat(D_0034DF38) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN;
    VU0_LOAD_VF(vf10, scratchVector);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, scratchVector);
    scratchVector[1] = work->head.heightScale * EFF_THUNDER_HALF_SCALE * ((effMiscRandUnitFloat(D_0034DF38) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN);
    cell->placementVector[0] = scratchVector[0];
    cell->placementVector[1] = scratchVector[1];
    cell->placementVector[2] = scratchVector[2];
    scratchVector[0] = (effMiscRandUnitFloat(D_0034DF38) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_AXIS_JITTER;
    scratchVector[1] = 1.0f;
    scratchVector[2] = (effMiscRandUnitFloat(D_0034DF38) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_AXIS_JITTER;
    VU0_LOAD_VF(vf10, scratchVector);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, scratchVector);
    cell->rotationAxis[0] = scratchVector[0];
    cell->rotationAxis[1] = scratchVector[1];
    cell->rotationAxis[2] = scratchVector[2];
    cell->rotationScale = work->head.rotationScale * ((effMiscRandUnitFloat(D_0034DF38) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_ROTATION_JITTER + 1.0f);
    cell->radius = work->head.radiusScale * EFF_THUNDER_HALF_SCALE * ((effMiscRandUnitFloat(D_0034DF38) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_RADIUS_JITTER + 1.0f);
    cell->color = EFF_THUNDER_NEUTRAL_COLOR;
}

/* Generate the indexed variant as two-vector rows in its kind-0 history.
 * It samples wider axis perturbations and refreshes the cell angle per row.
 */
void func_001642B0(EffThunderVectorWork *work, s32 index) {
    f32 position[4] __attribute__((aligned(16)));
    f32 placement[4] __attribute__((aligned(16)));
    f32 axis[4] __attribute__((aligned(16)));
    f32 originalAxis[4] __attribute__((aligned(16)));
    f32 cameraDirection[4] __attribute__((aligned(16)));
    f32 origin[4] __attribute__((aligned(16)));
    f32 width[4] __attribute__((aligned(16)));
    f32 rotation[4] __attribute__((aligned(16)));
    EffThunderParSystem *system = work->cellSystem;
    ParCell *cell = &system->cells[index];
    EffThunderVectorCell *source;
    u128 *vertices;
    s32 count;
    s32 i;
    f32 widthScale;
    f32 rotationScale;
    f32 angleJitter;
    f32 radius;

    count = system->vertexCount >> 1;
    cell->vertexCount = system->vertexCount;
    source = &work->cells[index];
    vertices = cell->history;
    widthScale = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 0.3f + 1.0f;
    VEC3_SPLAT(width, work->head.bandWidth * widthScale);
    originalAxis[0] = source->rotationAxis[0];
    originalAxis[1] = source->rotationAxis[1];
    originalAxis[2] = source->rotationAxis[2];
    originalAxis[3] = 0.0f;
    radius = source->radius;
    VU0_LOAD_VF(vf10, sdfViewEyeVector);
    VU0_LOAD_VF(vf11, sdfViewTargetVector);
    VU0_SUB_EXTENDED(vf10, vf10, vf11);
    VU0_STORE_VF_UNCLOBBERED(vf10, cameraDirection);
    placement[0] = source->placementVector[0];
    placement[1] = 0.0f;
    placement[2] = source->placementVector[2];
    PCP_COPY_VECTOR(origin, work->head.origin);
    origin[1] += source->placementVector[1];

    for (i = 0; i < count; i++) {
        position[0] = origin[0] + placement[0] * radius;
        position[1] = origin[1] + placement[1] * radius;
        position[2] = origin[2] + placement[2] * radius;
        PCP_COPY_VECTOR(axis, originalAxis);
        rotation[0] = effMiscRandUnitFloat(D_0034DF38) * (EFF_THUNDER_HALF_TURN * 0.5f);
        rotation[1] = effMiscRandUnitFloat(D_0034DF38) * (EFF_THUNDER_HALF_TURN * 0.5f);
        rotation[2] = effMiscRandUnitFloat(D_0034DF38) * (EFF_THUNDER_HALF_TURN * 0.5f);
        vu0RotMatrixXYZFromVec3((const struct RwV3d *)rotation);
        VU0_LOAD_VF(vf10, axis);
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf10, axis);
        angleJitter = effMiscRandUnitFloat(D_0034DF38);
        rotationScale = -source->rotationScale;
        sdfBuildVuRotationFromAxisAngle((const struct RwV3d *)axis,
            ((angleJitter - 0.5f) * 2.0f * 0.8f) * rotationScale + rotationScale);
        VU0_LOAD_VF(vf10, placement);
        VU0_MOVE_VF_EXTENDED(vf11, vf10);
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf10, placement);
        VU0_SUB_EXTENDED(vf10, vf10, vf11);
        VU0_LOAD_VF(vf11, cameraDirection);
        VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10_EXTENDED();
        VU0_LOAD_VF(vf11, width);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        VU0_LOAD_VF(vf10, position);
        VU0_MOVE_VF_EXTENDED(vf11, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf11, vertices + 1);
        vertices += 2;
    }
}


/* Rotate the live placement direction and every two-vector history row
 * around the effect origin. The separate placement height is preserved.
 */
void func_001645A0(EffThunderVectorWork *work, s32 index) {
    f32 axis[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    EffThunderParSystem *system = work->cellSystem;
    EffThunderVectorCell *source = &work->cells[index];
    ParCell *cell = &system->cells[index];
    u128 *vertices = cell->history;
    s32 rows = system->vertexCount >> 1;
    s32 i;
    f32 sample;
    f32 angle;

    sample = effMiscRandUnitFloat(D_0034DF38);
    angle = source->rotationScale * sample * 3.0f;
    direction[0] = source->placementVector[0];
    direction[1] = 0.0f;
    direction[2] = source->placementVector[2];
    axis[0] = source->rotationAxis[0];
    axis[1] = source->rotationAxis[1];
    axis[2] = source->rotationAxis[2];
    sdfBuildVuRotationFromAxisAngle((const struct RwV3d *)axis, angle);
    VU0_LOAD_VF(vf10, direction);
    VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
    VU0_STORE_VF_UNCLOBBERED(vf10, direction);
    source->placementVector[0] = direction[0];
    source->placementVector[2] = direction[2];
    VU0_LOAD_VF(vf11, work->head.origin);
    for (i = 0; i < rows; i++) {
        VU0_LOAD_VF(vf10, vertices);
        VU0_SUB_EXTENDED(vf10, vf10, vf11);
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices);
        VU0_LOAD_VF(vf10, vertices + 1);
        VU0_SUB_EXTENDED(vf10, vf10, vf11);
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices + 1);
        vertices += 2;
    }
}


extern void func_001642B0(EffThunderVectorWork *, s32);
extern void func_001645A0(EffThunderVectorWork *, s32);

/* Indexed vector countdown/fade/restart with shared tint and native render-cell stride.
   Fade is unsigned wrap-add; submission is unconditional after the signed-count loop. */
void effThunderUpdateIndexedVectorCells(EffThunderVectorWork *work) {
    s32 i = 0;
    EffThunderParSystem *renderSystem = work->cellSystem;
    s32 cellCount = work->head.cellCount;
    u32 tintColor = work->tintColor;
    EffThunderVectorCell *cell = work->cells;
    ParCell *renderCells = renderSystem->cells;
    u32 *renderColor;

    if (cellCount > 0) {
        renderColor = &renderCells->color;
        do {
            if (cell->delayFrames == 0) {
                if (cell->activeFrames != 0) {
                    func_001642B0(work, i);
                    func_001645A0(work, i);
                    cell->activeFrames--;
                } else if (cell->color & EFF_THUNDER_ALPHA_MASK) {
                    cell->color += EFF_THUNDER_ALPHA_WRAP_ADD;
                    func_001645A0(work, i);
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
            renderColor = (u32 *)((u8 *)renderColor + sizeof(ParCell));
        } while (i < cellCount);
    }
    parPrependCellNode(work->cellSystem);
}

/* Spark parameter prefix (0xA4 bytes); updates also rewrite its position vectors. */
typedef struct {
    f32 origin[4];     /* 0x00 complete orbit origin quadword */
    f32 loweredPosition[4]; /* 0x10: complete endpoint quadword */
    f32 position[4];    /* 0x20: complete endpoint quadword */
    u16 systemParam;    /* 0x30 */
    u8 pad32[0xA];
    f32 waveAmplitude; /* 0x3C */
    u8 pad40[8];
    u16 halfLife;       /* 0x48 */
    u8 pad4A[2];
    f32 width;         /* 0x4C: strip half-width */
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
typedef char EffThunderSparkParamsSizeCheck[sizeof(EffThunderSparkParams) == 0xA4 ? 1 : -1];

/* Each spark owns its own cell system; all subsystems are released before
   the containing work allocation. */
typedef struct {
    EffThunderSparkParams head;
    EffThunderSpark *sparks; /* 0xA4 */
    u32 tintColor;      /* 0xA8 multiplies the faded spark color */
    SdfMemBlock *allocationHandle; /* 0xAC */
} EffThunderSparkWork; /* 0xB0 */

extern void effThunderSparkInit(EffThunderSparkWork *work, s32 index);

/* Allocate spark state after the work and a separate one-cell system per spark.
   Clamp delay spread only in the copied head; sample motion before initial age. */
EffThunderSparkWork *effThunderSparkCreate(EffThunderSparkParams *parameters) {
    SdfMemBlock *allocationHandle = sdfAllocGeneralBlock(parameters->sparkCount * sizeof(EffThunderSpark) + sizeof(EffThunderSparkWork));
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
        func_0015D078((void *)work->sparks[sparkIndex].systemHandle, work->head.systemParam);
        effThunderSparkInit(work, sparkIndex);
        work->sparks[sparkIndex].age = -(effMiscRand(D_0034DF38) % delaySpread);
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
    spark->verticalSpeed = work->head.verticalSpeed * (effMiscRandUnitFloat(D_0034DF38) * randomness + (1.0f - randomness));
    spark->heightOffset = work->head.heightOffsetRange * effMiscRandUnitFloat(D_0034DF38);
    spark->orbitAngle = effMiscRandUnitFloat(D_0034DF38) * (EFF_THUNDER_HALF_TURN * 2.0f);
    randomness = work->head.angularSpeedRandomness;
    spark->angularSpeed = work->head.angularSpeed * (effMiscRandUnitFloat(D_0034DF38) * randomness + (1.0f - randomness));
    randomness = work->head.radiusRandomness;
    spark->orbitRadius = work->head.orbitRadius * (effMiscRandUnitFloat(D_0034DF38) * randomness + (1.0f - randomness));
}

struct RwV3d;
extern f32 sdfViewEyeVector[4], sdfViewTargetVector[4];
extern f32 sdfSinPoly(f32);
extern void sdfBuildVuRotationFromAxisAngle(const struct RwV3d *, f32);

/* Build one spark's kind-0 strip between its lowered and current positions.
 * Every iteration emits two two-vector rows into that spark's own cell.
 */
void func_00164BA8(EffThunderSparkWork *work, s32 index) {
    f32 width[4] __attribute__((aligned(16)));
    f32 negativeWidth[4] __attribute__((aligned(16)));
    f32 position[4] __attribute__((aligned(16)));
    f32 next[4] __attribute__((aligned(16)));
    f32 basis[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    f32 side[4] __attribute__((aligned(16)));
    f32 cameraDirection[4] __attribute__((aligned(16)));
    f32 waveScale[4] __attribute__((aligned(16)));
    f32 stepScale[4] __attribute__((aligned(16)));
    f32 previousSide[4] __attribute__((aligned(16)));
    f32 delta[4] __attribute__((aligned(16)));
    EffThunderParSystem *system;
    ParCell *cell;
    u128 *vertices;
    u128 *join;
    s32 i;
    s32 segments;
    f32 distance;
    f32 amplitude;
    f32 phase;
    f32 cycle;
    f32 currentWave;
    f32 previousWave;
    f32 phaseStep;
    f32 facing;

    VEC3_SPLAT(width, work->head.width);
    VEC3_SPLAT(negativeWidth, -width[0]);
    system = (EffThunderParSystem *)work->sparks[index].systemHandle;
    cell = system->cells;
    cell->vertexCount = system->vertexCount;
    vertices = cell->history;
    segments = work->head.halfLife;
    VU0_LOAD_VF(vf10, work->head.position);
    VU0_LOAD_VF(vf11, work->head.loweredPosition);
    VU0_SUB_EXTENDED(vf10, vf10, vf11);
    VU0_LENGTH_VF10(distance);
    VU0_NORMALIZE_VF10_EXTENDED();
    VU0_STORE_VF_UNCLOBBERED(vf10, direction);
    distance /= work->head.halfLife;
    amplitude = work->head.waveAmplitude;
    VU0_LOAD_VF(vf10, sdfViewEyeVector);
    VU0_LOAD_VF(vf11, sdfViewTargetVector);
    VU0_SUB_EXTENDED(vf10, vf10, vf11);
    VU0_STORE_VF_UNCLOBBERED(vf10, cameraDirection);
    VU0_LOAD_VF(vf11, direction);
    VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10_EXTENDED();
    VU0_STORE_VF_UNCLOBBERED(vf10, side);
    phase = effMiscRandUnitFloat(D_0034DF38) * (EFF_THUNDER_HALF_TURN * 2.0f);
    cycle = phase - (EFF_THUNDER_HALF_TURN * 2.0f);
    PCP_COPY_VECTOR(position, work->head.loweredPosition);
    currentWave = amplitude * sdfSinPoly(phase);
    VEC3_SPLAT(waveScale, currentWave);
    VU0_LOAD_VF(vf10, side);
    VU0_LOAD_VF(vf11, waveScale);
    VU0_MUL_EXTENDED(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, position);
    VU0_ADD_EXTENDED(vf11, vf11, vf10);
    VU0_STORE_VF_UNCLOBBERED(vf11, position);
    VEC3_SPLAT(stepScale, distance);
    previousWave = currentWave;
    currentWave = amplitude * sdfSinPoly(phase);
    VEC3_SPLAT(waveScale, currentWave - previousWave);
    VU0_LOAD_VF(vf10, direction);
    VU0_LOAD_VF(vf11, stepScale);
    VU0_MUL_EXTENDED(vf10, vf10, vf11);
    VU0_MOVE_VF_EXTENDED(vf12, vf10);
    VU0_LOAD_VF(vf10, side);
    VU0_LOAD_VF(vf11, waveScale);
    VU0_MUL_EXTENDED(vf10, vf10, vf11);
    VU0_ADD_EXTENDED(vf10, vf10, vf12);
    VU0_MOVE_VF_EXTENDED(vf12, vf10);
    VU0_LOAD_VF(vf11, position);
    VU0_ADD_EXTENDED(vf11, vf11, vf10);
    VU0_STORE_VF_UNCLOBBERED(vf11, next);
    VU0_MOVE_VF_EXTENDED(vf10, vf12);
    VU0_NORMALIZE_VF10_EXTENDED();
    VU0_STORE_VF_UNCLOBBERED(vf10, basis);

    for (i = 0; i < segments; i++) {
        VU0_LOAD_VF(vf10, basis);
        VU0_LOAD_VF(vf11, cameraDirection);
        VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10_EXTENDED();
        VU0_STORE_VF_UNCLOBBERED(vf10, basis);
        VU0_LOAD_VF(vf11, width);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        VU0_LOAD_VF(vf10, position);
        VU0_MOVE_VF_EXTENDED(vf11, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf11, vertices + 1);
        vertices += 2;
        PCP_COPY_VECTOR(previousSide, basis);
        PCP_COPY_VECTOR(position, next);
        VU0_LOAD_VF(vf10, basis);
        VU0_LOAD_VF(vf11, width);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        VU0_LOAD_VF(vf10, position);
        VU0_MOVE_VF_EXTENDED(vf11, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf11, vertices + 1);
        vertices += 2;
        phaseStep = (EFF_THUNDER_HALF_TURN / segments) * 4.0f;
        cycle += phaseStep;
        phase += phaseStep;
        if (cycle > EFF_THUNDER_HALF_TURN) {
            cycle -= EFF_THUNDER_HALF_TURN;
            amplitude = work->head.waveAmplitude * (1.0f - effMiscRandUnitFloat(D_0034DF38) * 0.95f);
        }
        previousWave = currentWave;
        currentWave = amplitude * sdfSinPoly(phase);
        VEC3_SPLAT(waveScale, currentWave - previousWave);
        VU0_LOAD_VF(vf10, direction);
        VU0_LOAD_VF(vf11, stepScale);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        VU0_LOAD_VF(vf10, side);
        VU0_LOAD_VF(vf11, waveScale);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        VU0_STORE_VF_UNCLOBBERED(vf10, delta);
        sdfBuildVuRotationFromAxisAngle((const struct RwV3d *)cameraDirection,
            ((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f) * (20.0f * (EFF_THUNDER_HALF_TURN / 180.0f)));
        VU0_LOAD_VF(vf10, delta);
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        VU0_LOAD_VF(vf11, position);
        VU0_ADD_EXTENDED(vf11, vf11, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf11, next);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_NORMALIZE_VF10_EXTENDED();
        VU0_STORE_VF_UNCLOBBERED(vf10, basis);
        VU0_LOAD_VF(vf10, next);
        VU0_LOAD_VF(vf11, position);
        VU0_SUB_EXTENDED(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10_EXTENDED();
        VU0_LOAD_VF(vf11, previousSide);
        VU0_DOT_XYZ(facing, vf10, vf11);
        /* Choose the join edge from the new direction relative to the prior side. */
        if (facing < 0.0f) {
            join = vertices - 1;
            VU0_LOAD_VF(vf12, width);
        } else {
            join = vertices - 2;
            VU0_LOAD_VF(vf12, negativeWidth);
        }
        VU0_LOAD_VF(vf10, basis);
        VU0_LOAD_VF(vf11, cameraDirection);
        VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10_EXTENDED();
        VU0_MOVE_VF_EXTENDED(vf11, vf12);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_LOAD_VF(vf11, join);
        VU0_ADD_EXTENDED(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, position);
    }
}


extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);

/* Advance each spark through delayed initialization, motion and alpha fades.
 * Snapshot the entry parameters and age; initialization and geometry callbacks
 * can update the work while the frame's timing decision remains unchanged.
 */
void func_00165110(EffThunderSparkWork *work) {
    f32 position[3];
    f32 origin[4] __attribute__((aligned(16)));
    u32 tintColor;
    s32 count;
    s32 loop = work->head.loop;
    s32 duration = work->head.duration;
    s32 delaySpread = work->head.startDelaySpread;
    s32 fadeInTime = work->head.fadeInTime;
    s32 fadeOutTime = work->head.fadeOutTime;
    f32 heightOffset = work->head.heightOffset;
    f32 verticalDamping = work->head.verticalDamping;
    f32 angularDamping = work->head.angularDamping;
    EffThunderSpark *spark;
    ParCell *renderCell;
    s32 i;
    s32 age;
    f32 angle;
    f32 radius;
    f32 alpha;
    f32 lowerHeight;
    u32 color;

    tintColor = work->tintColor;
    PCP_COPY_VECTOR(origin, work->head.origin);
    count = work->head.sparkCount;
    spark = work->sparks;
    for (i = 0; i < count; i++, spark++) {
        age = spark->age;
        if (age == 0) {
            effThunderSparkInit(work, i);
            parCellInit((void *)spark->systemHandle, 0);
        } else if (age > 0 && age <= duration) {
            angle = spark->orbitAngle;
            radius = spark->orbitRadius;
            position[0] = origin[0] + sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
            position[1] = origin[1] + spark->verticalOffset - spark->heightOffset;
            position[2] = origin[2] + sdfSinPoly(angle) * radius;
            spark->verticalOffset += spark->verticalSpeed;
            spark->orbitAngle += spark->angularSpeed;
            spark->verticalSpeed *= verticalDamping;
            spark->angularSpeed *= angularDamping;
            if (age < fadeInTime) {
                alpha = (f32)age / fadeInTime;
            } else if (duration - age <= fadeOutTime) {
                alpha = (f32)(duration - age) / fadeOutTime;
            } else {
                alpha = 1.0f;
            }
            color = effMultiplyPackedColors(((u32)(alpha * 127.0f) << 24) | 0x808080, tintColor);
            renderCell = ((EffThunderParSystem *)spark->systemHandle)->cells;
            lowerHeight = position[1] - heightOffset;
            work->head.loweredPosition[0] = position[0];
            work->head.loweredPosition[1] = lowerHeight;
            work->head.loweredPosition[2] = position[2];
            work->head.position[0] = position[0];
            work->head.position[1] = position[1];
            work->head.position[2] = position[2];
            func_00164BA8(work, i);
            renderCell->color = color;
            parPrependCellNode((void *)spark->systemHandle);
        }
        if (age >= duration && loop) {
            spark->age = -(effMiscRand(D_0034DF38) % delaySpread);
        } else {
            spark->age++;
        }
    }
}


extern void parRiseFallSymmetricCellAlpha(void *system, u32 a, u32 b, u32 c);


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
    SdfMemBlock *allocationHandle; /* 0x64 */
} EffThunderFragmentWork; /* 0x68 */

extern void effThunderRandomizeFrag(EffThunderFragmentWork *work, s32 index);

/* Create the single-system fragment work and seed every fragment's timing/color.
   The native history-count expression and unchecked ranges are retained. */
EffThunderFragmentWork *effThunderFragCreate(EffThunderFragmentParams *parameters) {
    SdfMemBlock *allocationHandle = sdfAllocGeneralBlock(parameters->fragmentCount * sizeof(EffThunderFrag) + sizeof(EffThunderFragmentWork));
    EffThunderFragmentWork *work = (EffThunderFragmentWork *)sdfResourceRetainAddress(allocationHandle);
    u32 i;

    work->head = *parameters;
    work->fragments = (EffThunderFrag *)(work + 1);
    work->allocationHandle = allocationHandle;
    work->system = parAllocateCellSystem(work->head.fragmentCount, work->head.halfLife * 2 - 1, 0, 4);
    parRiseFallSymmetricCellAlpha(work->system, work->head.arg40, work->head.arg48, work->head.arg50);
    func_0015D078(work->system, work->head.systemParam);
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
u32 func_00165638(u32 value) {
    return value;
}

/* Forward the system and its opaque fragment configuration through the native call. */
void func_00165640(EffThunderFragmentWork *work) {
    parDecreaseSymmetricCellAlpha((u32)work->system, work->head.arg40, (void *)work->head.arg48, work->head.arg50);
}

/* Alternate native operation on the same system and opaque fragment configuration. */
void func_00165668(EffThunderFragmentWork *work) {
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

    fragment->delayFrames = effMiscRand(&D_0034DF38) % work->head.startDelayRange;
    fragment->activeFrames = effMiscRand(&D_0034DF38) % work->head.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
    fragment->color = EFF_THUNDER_FRAGMENT_GREY;
}

struct RwV3d;
extern f32 sdfViewEyeVector[4], sdfViewTargetVector[4];
extern f32 sdfSinPoly(f32);
extern void sdfBuildVuRotationFromAxisAngle(const struct RwV3d *, f32);

/* Build the first strip from the work's own start point. Each step emits
 * two five-vector rows; later chain segments reuse the final row as a seed.
 */
void func_00165758(EffThunderFragmentWork *work, s32 index) {
    f32 width[4] __attribute__((aligned(16)));
    f32 outerWidth[4] __attribute__((aligned(16)));
    f32 negativeOuterWidth[4] __attribute__((aligned(16)));
    f32 position[4] __attribute__((aligned(16)));
    f32 next[4] __attribute__((aligned(16)));
    f32 basis[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    f32 side[4] __attribute__((aligned(16)));
    f32 cameraDirection[4] __attribute__((aligned(16)));
    f32 waveScale[4] __attribute__((aligned(16)));
    f32 stepScale[4] __attribute__((aligned(16)));
    f32 previousSide[4] __attribute__((aligned(16)));
    f32 delta[4] __attribute__((aligned(16)));
    EffThunderParSystem *system = work->system;
    ParCell *cell = &system->cells[index];
    u128 *vertices;
    u128 *join;
    s32 i;
    s32 segments;
    f32 distance;
    f32 amplitude;
    f32 phase;
    f32 cycle;
    f32 currentWave;
    f32 previousWave;
    f32 phaseStep;
    f32 facing;

    VEC3_SPLAT(width, work->head.bandWidth);
    VEC3_SPLAT(outerWidth, width[0] + work->head.edgeWidth);
    VEC3_SPLAT(negativeOuterWidth, -outerWidth[0]);
    cell->vertexCount = system->vertexCount;
    vertices = cell->history;
    segments = work->head.halfLife;
    VU0_LOAD_VF(vf10, work->head.end);
    VU0_LOAD_VF(vf11, work->head.start);
    VU0_SUB_EXTENDED(vf10, vf10, vf11);
    VU0_LENGTH_VF10(distance);
    VU0_NORMALIZE_VF10_EXTENDED();
    VU0_STORE_VF_UNCLOBBERED(vf10, direction);
    distance /= work->head.halfLife;
    amplitude = work->head.waveAmplitude;
    VU0_LOAD_VF(vf10, sdfViewEyeVector);
    VU0_LOAD_VF(vf11, sdfViewTargetVector);
    VU0_SUB_EXTENDED(vf10, vf10, vf11);
    VU0_STORE_VF_UNCLOBBERED(vf10, cameraDirection);
    VU0_LOAD_VF(vf11, direction);
    VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10_EXTENDED();
    VU0_STORE_VF_UNCLOBBERED(vf10, side);
    phase = effMiscRandUnitFloat(D_0034DF38) * (EFF_THUNDER_HALF_TURN * 2.0f);
    cycle = phase - (EFF_THUNDER_HALF_TURN * 2.0f);
    PCP_COPY_VECTOR(position, work->head.start);
    currentWave = amplitude * sdfSinPoly(phase);
    VEC3_SPLAT(waveScale, currentWave);
    VU0_LOAD_VF(vf10, side);
    VU0_LOAD_VF(vf11, waveScale);
    VU0_MUL_EXTENDED(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, position);
    VU0_ADD_EXTENDED(vf11, vf11, vf10);
    VU0_STORE_VF_UNCLOBBERED(vf11, position);
    VEC3_SPLAT(stepScale, distance);
    previousWave = currentWave;
    currentWave = amplitude * sdfSinPoly(phase);
    VEC3_SPLAT(waveScale, currentWave - previousWave);
    VU0_LOAD_VF(vf10, direction);
    VU0_LOAD_VF(vf11, stepScale);
    VU0_MUL_EXTENDED(vf10, vf10, vf11);
    VU0_MOVE_VF_EXTENDED(vf12, vf10);
    VU0_LOAD_VF(vf10, side);
    VU0_LOAD_VF(vf11, waveScale);
    VU0_MUL_EXTENDED(vf10, vf10, vf11);
    VU0_ADD_EXTENDED(vf10, vf10, vf12);
    VU0_MOVE_VF_EXTENDED(vf12, vf10);
    VU0_LOAD_VF(vf11, position);
    VU0_ADD_EXTENDED(vf11, vf11, vf10);
    VU0_STORE_VF_UNCLOBBERED(vf11, next);
    VU0_MOVE_VF_EXTENDED(vf10, vf12);
    VU0_NORMALIZE_VF10_EXTENDED();
    VU0_STORE_VF_UNCLOBBERED(vf10, basis);

    for (i = 0; i < segments; i++) {
        VU0_LOAD_VF(vf10, basis);
        VU0_LOAD_VF(vf11, cameraDirection);
        VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10_EXTENDED();
        VU0_STORE_VF_UNCLOBBERED(vf10, basis);
        VU0_MOVE_VF_EXTENDED(vf11, vf10);
        VU0_LOAD_VF(vf10, position);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices + 2);
        VU0_MOVE_VF_EXTENDED(vf10, vf11);
        VU0_LOAD_VF(vf11, width);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        VU0_LOAD_VF(vf10, position);
        VU0_MOVE_VF_EXTENDED(vf11, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices + 1);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf11, vertices + 3);
        VU0_LOAD_VF(vf10, basis);
        VU0_LOAD_VF(vf11, outerWidth);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        VU0_LOAD_VF(vf10, position);
        VU0_MOVE_VF_EXTENDED(vf11, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf11, vertices + 4);
        vertices += 5;
        PCP_COPY_VECTOR(previousSide, basis);
        PCP_COPY_VECTOR(position, next);
        VU0_LOAD_VF(vf10, position);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices + 2);
        VU0_LOAD_VF(vf10, basis);
        VU0_LOAD_VF(vf11, width);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        VU0_LOAD_VF(vf10, position);
        VU0_MOVE_VF_EXTENDED(vf11, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices + 1);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf11, vertices + 3);
        VU0_LOAD_VF(vf10, basis);
        VU0_LOAD_VF(vf11, outerWidth);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        VU0_LOAD_VF(vf10, position);
        VU0_MOVE_VF_EXTENDED(vf11, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf11, vertices + 4);
        vertices += 5;
        phaseStep = (EFF_THUNDER_HALF_TURN / segments) * 4.0f;
        cycle += phaseStep;
        phase += phaseStep;
        if (cycle > EFF_THUNDER_HALF_TURN) {
            cycle -= EFF_THUNDER_HALF_TURN;
            amplitude = work->head.waveAmplitude * (1.0f - effMiscRandUnitFloat(D_0034DF38) * 0.95f);
        }
        previousWave = currentWave;
        currentWave = amplitude * sdfSinPoly(phase);
        VEC3_SPLAT(waveScale, currentWave - previousWave);
        VU0_LOAD_VF(vf10, direction);
        VU0_LOAD_VF(vf11, stepScale);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        VU0_LOAD_VF(vf10, side);
        VU0_LOAD_VF(vf11, waveScale);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        VU0_STORE_VF_UNCLOBBERED(vf10, delta);
        sdfBuildVuRotationFromAxisAngle((const struct RwV3d *)cameraDirection,
            ((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f) * (20.0f * (EFF_THUNDER_HALF_TURN / 180.0f)));
        VU0_LOAD_VF(vf10, delta);
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        VU0_LOAD_VF(vf11, position);
        VU0_ADD_EXTENDED(vf11, vf11, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf11, next);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_NORMALIZE_VF10_EXTENDED();
        VU0_STORE_VF_UNCLOBBERED(vf10, basis);
        VU0_LOAD_VF(vf10, next);
        VU0_LOAD_VF(vf11, position);
        VU0_SUB_EXTENDED(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10_EXTENDED();
        VU0_LOAD_VF(vf11, previousSide);
        VU0_DOT_XYZ(facing, vf10, vf11);
        /* Choose the join edge from the new direction relative to the prior side. */
        if (facing < 0.0f) {
            join = vertices - 1;
            VU0_LOAD_VF(vf12, outerWidth);
        } else {
            join = vertices - 5;
            VU0_LOAD_VF(vf12, negativeOuterWidth);
        }
        VU0_LOAD_VF(vf10, basis);
        VU0_LOAD_VF(vf11, cameraDirection);
        VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10_EXTENDED();
        VU0_MOVE_VF_EXTENDED(vf11, vf12);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_LOAD_VF(vf11, join);
        VU0_ADD_EXTENDED(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, position);
    }
}


/* Update each fragment through delay, active geometry and alpha fade. The
 * optional frame limit stops new restarts; active geometry and fades continue.
 */
void func_00165D80(EffThunderFragmentWork *work) {
    s32 i = 0;
    EffThunderParSystem *system = work->system;
    s32 count = work->head.fragmentCount;
    u32 restartLimit = work->head.restartFrameLimit;
    u32 tintColor = work->color;
    u32 updateCount = work->state.updateCount;
    EffThunderFrag *fragment = work->fragments;
    ParCell *renderCells = system->cells;
    s32 canRestart;

    if (count > 0) {
        canRestart = updateCount < restartLimit;
        do {
            if (fragment->delayFrames == 0) {
                if (fragment->activeFrames != 0) {
                    func_00165758(work, i);
                    fragment->activeFrames--;
                } else if (fragment->color & EFF_THUNDER_ALPHA_MASK) {
                    fragment->color += EFF_THUNDER_ALPHA_WRAP_ADD;
                } else if (restartLimit == 0 || canRestart) {
                    effThunderRandomizeFrag(work, i);
                    parCellInit(work->system, i);
                } else {
                    renderCells[i].vertexCount = 0;
                }
            } else {
                fragment->delayFrames--;
            }
            renderCells[i].color = effMultiplyPackedColors(fragment->color, tintColor);
            i++;
            fragment++;
        } while (i < count);
        updateCount = work->state.updateCount;
    }
    work->state.updateCount = updateCount + 1;
    parPrependCellNode(work->system);
}



extern void effThunderRandomizeFrag2(EffThunderFragmentWork *work, s32 index);

/* Allocate dual-system fragment work, creating the secondary system before the primary.
   Both systems share one fragment-state array; keep native dispatch/allocation order. */
EffThunderFragmentWork *func_00165ED0(EffThunderFragmentParams *parameters) {
    SdfMemBlock *allocationHandle = sdfAllocGeneralBlock(parameters->fragmentCount * sizeof(EffThunderFrag) + sizeof(EffThunderFragmentWork));
    EffThunderFragmentWork *work = (EffThunderFragmentWork *)sdfResourceRetainAddress(allocationHandle);
    u32 i;

    work->head = *parameters;
    work->fragments = (EffThunderFrag *)(work + 1);
    work->allocationHandle = allocationHandle;
    work->state.secondarySystem = parAllocateCellSystem(work->head.fragmentCount, work->head.halfLife * 2 - 1, 0, 1);
    parDispatchSub(work->state.secondarySystem, 2, work->head.arg48, work->head.arg50);
    func_0015D078(work->state.secondarySystem, work->head.systemParam);
    work->system = parAllocateCellSystem(work->head.fragmentCount, work->head.halfLife * 2 - 1, 0, 0);
    parDispatchSub(work->system, 2, work->head.arg40, work->head.arg40);
    func_0015D078(work->system, work->head.systemParam);
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

    fragment->delayFrames = effMiscRand(&D_0034DF38) % work->head.startDelayRange;
    fragment->activeFrames = effMiscRand(&D_0034DF38) % work->head.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
    fragment->color = EFF_THUNDER_FRAGMENT_GREY;
}

struct RwV3d;
extern f32 sdfViewEyeVector[4], sdfViewTargetVector[4];
extern f32 sdfSinPoly(f32);
extern void sdfBuildVuRotationFromAxisAngle(const struct RwV3d *, f32);

/* Build synchronized kind-0 core and kind-1 outer strips. Each step emits
 * two rows of two core vertices and three outer vertices from the same path.
 */
void func_001661D8(EffThunderFragmentWork *work, s32 index) {
    f32 width[4] __attribute__((aligned(16)));
    f32 outerWidth[4] __attribute__((aligned(16)));
    f32 negativeOuterWidth[4] __attribute__((aligned(16)));
    f32 position[4] __attribute__((aligned(16)));
    f32 next[4] __attribute__((aligned(16)));
    f32 basis[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    f32 side[4] __attribute__((aligned(16)));
    f32 cameraDirection[4] __attribute__((aligned(16)));
    f32 waveScale[4] __attribute__((aligned(16)));
    f32 stepScale[4] __attribute__((aligned(16)));
    f32 previousSide[4] __attribute__((aligned(16)));
    f32 delta[4] __attribute__((aligned(16)));
    EffThunderParSystem *secondarySystem = work->state.secondarySystem;
    EffThunderParSystem *primarySystem = work->system;
    ParCell *secondaryCell = &secondarySystem->cells[index];
    ParCell *primaryCell = &primarySystem->cells[index];
    u128 *vertices;
    u128 *coreVertices;
    u128 *join;
    s32 i;
    s32 segments;
    f32 distance;
    f32 amplitude;
    f32 phase;
    f32 cycle;
    f32 currentWave;
    f32 previousWave;
    f32 phaseStep;
    f32 facing;

    VEC3_SPLAT(width, work->head.coreWidth);
    VEC3_SPLAT(outerWidth, (width[0] + work->head.edgeWidth) + work->head.bandWidth);
    VEC3_SPLAT(negativeOuterWidth, -outerWidth[0]);
    secondaryCell->vertexCount = secondarySystem->vertexCount;
    primaryCell->vertexCount = primarySystem->vertexCount;
    vertices = secondaryCell->history;
    coreVertices = primaryCell->history;
    segments = work->head.halfLife;
    VU0_LOAD_VF(vf10, work->head.end);
    VU0_LOAD_VF(vf11, work->head.start);
    VU0_SUB_EXTENDED(vf10, vf10, vf11);
    VU0_LENGTH_VF10(distance);
    VU0_NORMALIZE_VF10_EXTENDED();
    VU0_STORE_VF_UNCLOBBERED(vf10, direction);
    distance /= work->head.halfLife;
    amplitude = work->head.waveAmplitude;
    VU0_LOAD_VF(vf10, sdfViewEyeVector);
    VU0_LOAD_VF(vf11, sdfViewTargetVector);
    VU0_SUB_EXTENDED(vf10, vf10, vf11);
    VU0_STORE_VF_UNCLOBBERED(vf10, cameraDirection);
    VU0_LOAD_VF(vf11, direction);
    VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10_EXTENDED();
    VU0_STORE_VF_UNCLOBBERED(vf10, side);
    phase = effMiscRandUnitFloat(D_0034DF38) * (EFF_THUNDER_HALF_TURN * 2.0f);
    cycle = phase - (EFF_THUNDER_HALF_TURN * 2.0f);
    PCP_COPY_VECTOR(position, work->head.start);
    currentWave = amplitude * sdfSinPoly(phase);
    VEC3_SPLAT(waveScale, currentWave);
    VU0_LOAD_VF(vf10, side);
    VU0_LOAD_VF(vf11, waveScale);
    VU0_MUL_EXTENDED(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, position);
    VU0_ADD_EXTENDED(vf11, vf11, vf10);
    VU0_STORE_VF_UNCLOBBERED(vf11, position);
    VEC3_SPLAT(stepScale, distance);
    previousWave = currentWave;
    currentWave = amplitude * sdfSinPoly(phase);
    VEC3_SPLAT(waveScale, currentWave - previousWave);
    VU0_LOAD_VF(vf10, direction);
    VU0_LOAD_VF(vf11, stepScale);
    VU0_MUL_EXTENDED(vf10, vf10, vf11);
    VU0_MOVE_VF_EXTENDED(vf12, vf10);
    VU0_LOAD_VF(vf10, side);
    VU0_LOAD_VF(vf11, waveScale);
    VU0_MUL_EXTENDED(vf10, vf10, vf11);
    VU0_ADD_EXTENDED(vf10, vf10, vf12);
    VU0_MOVE_VF_EXTENDED(vf12, vf10);
    VU0_LOAD_VF(vf11, position);
    VU0_ADD_EXTENDED(vf11, vf11, vf10);
    VU0_STORE_VF_UNCLOBBERED(vf11, next);
    VU0_MOVE_VF_EXTENDED(vf10, vf12);
    VU0_NORMALIZE_VF10_EXTENDED();
    VU0_STORE_VF_UNCLOBBERED(vf10, basis);

    for (i = 0; i < segments; i++) {
        VU0_LOAD_VF(vf10, basis);
        VU0_LOAD_VF(vf11, cameraDirection);
        VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10_EXTENDED();
        VU0_STORE_VF_UNCLOBBERED(vf10, basis);
        VU0_LOAD_VF(vf11, width);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        VU0_LOAD_VF(vf10, position);
        VU0_MOVE_VF_EXTENDED(vf11, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        VU0_STORE_VF_UNCLOBBERED(vf10, coreVertices);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf11, coreVertices + 1);
        VU0_LOAD_VF(vf10, basis);
        VU0_LOAD_VF(vf11, outerWidth);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        VU0_LOAD_VF(vf10, position);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices + 1);
        VU0_MOVE_VF_EXTENDED(vf11, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf11, vertices + 2);
        coreVertices += 2;
        vertices += 3;
        PCP_COPY_VECTOR(previousSide, basis);
        PCP_COPY_VECTOR(position, next);
        VU0_LOAD_VF(vf10, basis);
        VU0_LOAD_VF(vf11, width);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        VU0_LOAD_VF(vf10, position);
        VU0_MOVE_VF_EXTENDED(vf11, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        VU0_STORE_VF_UNCLOBBERED(vf10, coreVertices);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf11, coreVertices + 1);
        VU0_LOAD_VF(vf10, basis);
        VU0_LOAD_VF(vf11, outerWidth);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        VU0_LOAD_VF(vf10, position);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices + 1);
        VU0_MOVE_VF_EXTENDED(vf11, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf11, vertices + 2);
        coreVertices += 2;
        vertices += 3;
        phaseStep = (EFF_THUNDER_HALF_TURN / segments) * 4.0f;
        cycle += phaseStep;
        phase += phaseStep;
        if (cycle > EFF_THUNDER_HALF_TURN) {
            cycle -= EFF_THUNDER_HALF_TURN;
            amplitude = work->head.waveAmplitude * (1.0f - effMiscRandUnitFloat(D_0034DF38) * 0.95f);
        }
        previousWave = currentWave;
        currentWave = amplitude * sdfSinPoly(phase);
        VEC3_SPLAT(waveScale, currentWave - previousWave);
        VU0_LOAD_VF(vf10, direction);
        VU0_LOAD_VF(vf11, stepScale);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        VU0_LOAD_VF(vf10, side);
        VU0_LOAD_VF(vf11, waveScale);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        VU0_STORE_VF_UNCLOBBERED(vf10, delta);
        sdfBuildVuRotationFromAxisAngle((const struct RwV3d *)cameraDirection,
            ((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f) * (20.0f * (EFF_THUNDER_HALF_TURN / 180.0f)));
        VU0_LOAD_VF(vf10, delta);
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        VU0_LOAD_VF(vf11, position);
        VU0_ADD_EXTENDED(vf11, vf11, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf11, next);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_NORMALIZE_VF10_EXTENDED();
        VU0_STORE_VF_UNCLOBBERED(vf10, basis);
        VU0_LOAD_VF(vf10, next);
        VU0_LOAD_VF(vf11, position);
        VU0_SUB_EXTENDED(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10_EXTENDED();
        VU0_LOAD_VF(vf11, previousSide);
        VU0_DOT_XYZ(facing, vf10, vf11);
        /* Choose the join edge from the new direction relative to the prior side. */
        if (facing < 0.0f) {
            join = vertices - 1;
            VU0_LOAD_VF(vf12, outerWidth);
        } else {
            join = vertices - 3;
            VU0_LOAD_VF(vf12, negativeOuterWidth);
        }
        VU0_LOAD_VF(vf10, basis);
        VU0_LOAD_VF(vf11, cameraDirection);
        VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10_EXTENDED();
        VU0_MOVE_VF_EXTENDED(vf11, vf12);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_LOAD_VF(vf11, join);
        VU0_ADD_EXTENDED(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, position);
    }
}


extern void func_001661D8(EffThunderFragmentWork *, s32);

/* One countdown/fade/restart state drives both systems; write the same tinted color to each.
   Secondary initialization/submission precedes primary; submission also occurs for count <= 0. */
void effThunderUpdateDualFragments(EffThunderFragmentWork *work) {
    s32 index = 0;
    EffThunderParSystem *secondarySystem = work->state.secondarySystem;
    s32 fragmentCount = work->head.fragmentCount;
    EffThunderParSystem *primarySystem = work->system;
    ParCell *primaryCells = primarySystem->cells;
    u32 tintColor = work->color;
    EffThunderFrag *fragment = work->fragments;
    ParCell *secondaryCells = secondarySystem->cells;
    s32 packedColor;

    if (fragmentCount > 0) {
        do {
            if (fragment->delayFrames == 0) {
                if (fragment->activeFrames != 0) {
                    func_001661D8(work, index);
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
    f32 origin[4];          /* 0x00: complete origin quadword */
    u16 systemParam;         /* 0x10 */
    u8 pad12[2];
    f32 length;             /* 0x14: travel distance scale */
    u32 cellCount;           /* 0x18 */
    u8 pad1C[8];
    u32 startDelayRange;     /* 0x24 modulus of delayFrames */
    u32 activeFrameRange;    /* 0x28 modulus of activeFrames before adding one */
    u16 halfLife;            /* 0x2C */
    u8 pad2E[2];
    f32 coreWidth;          /* 0x30 */
    u32 arg34;               /* 0x34 */
    f32 bandWidth;          /* 0x38 */
    u32 arg3C;               /* 0x3C */
    f32 edgeWidth;          /* 0x40 */
    u32 arg44;               /* 0x44 */
} EffThunderCellParams;
typedef char EffThunderCellParamsSizeCheck[sizeof(EffThunderCellParams) == 0x48 ? 1 : -1];

/* The counted-down cell effect has its own 0x58-byte work and 0x14-byte
   particles, not the fragment or vector variants' resource offsets. */
typedef struct {
    EffThunderCellParams head;
    EffThunderCell *cells;   /* 0x48 */
    u32 unk4C;               /* 0x4C: settable, otherwise unobserved */
    void *system;            /* 0x50 */
    SdfMemBlock *allocationHandle; /* 0x54 */
} EffThunderCellWork; /* 0x58 */

extern void effThunderRandomizeCell(EffThunderCellWork *work, s32 index);

/* Allocate cell states after the work, configure its system and sample directions/timing.
   The opaque settable word is not initialized here; no new default or range checks are added. */
EffThunderCellWork *effThunderCellCreate(EffThunderCellParams *parameters) {
    SdfMemBlock *allocationHandle = sdfAllocGeneralBlock(parameters->cellCount * sizeof(EffThunderCell) + sizeof(EffThunderCellWork));
    EffThunderCellWork *work = (EffThunderCellWork *)sdfResourceRetainAddress(allocationHandle);
    u32 i;

    work->head = *parameters;
    work->cells = (EffThunderCell *)(work + 1);
    work->allocationHandle = allocationHandle;
    work->system = parAllocateCellSystem(work->head.cellCount, work->head.halfLife * 2 - 1, 0, 2);
    parDecreaseStripCellAlpha(work->system, work->head.arg34, work->head.arg3C, work->head.arg44);
    func_0015D078(work->system, work->head.systemParam);
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

    centeredRandom = effMiscRandUnitFloat(&D_0034DF38) - EFF_THUNDER_RANDOM_MIDPOINT;
    cell->directionX = centeredRandom + centeredRandom;
    centeredRandom = effMiscRandUnitFloat(&D_0034DF38) - EFF_THUNDER_RANDOM_MIDPOINT;
    cell->directionY = centeredRandom + centeredRandom;
    centeredRandom = effMiscRandUnitFloat(&D_0034DF38) - EFF_THUNDER_RANDOM_MIDPOINT;
    cell->directionZ = centeredRandom + centeredRandom;
    cell->delayFrames = effMiscRand(&D_0034DF38) % work->head.startDelayRange;
    cell->activeFrames = effMiscRand(&D_0034DF38) % work->head.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
}

/* Build a kind-2 cell with three nested width bands. Each randomized step
 * emits two six-vector rows and then turns the direction around the view axis.
 */
void func_00166C18(EffThunderCellWork *work, s32 index) {
    f32 width[4] __attribute__((aligned(16)));
    f32 bandWidth[4] __attribute__((aligned(16)));
    f32 outerWidth[4] __attribute__((aligned(16)));
    f32 negativeOuterWidth[4] __attribute__((aligned(16)));
    f32 stepScale[4] __attribute__((aligned(16)));
    f32 position[4] __attribute__((aligned(16)));
    f32 basis[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    f32 cameraDirection[4] __attribute__((aligned(16)));
    f32 side[4] __attribute__((aligned(16)));
    EffThunderParSystem *system = work->system;
    EffThunderCell *source = &work->cells[index];
    ParCell *cell = &system->cells[index];
    u128 *vertices;
    u128 *join;
    s32 i;
    s32 segments;
    f32 distance;
    f32 facing;
    f32 stepLength;

    VEC3_SPLAT(width, work->head.coreWidth);
    VEC3_SPLAT(bandWidth, width[0] + work->head.bandWidth);
    VEC3_SPLAT(outerWidth, bandWidth[0] + work->head.edgeWidth);
    VEC3_SPLAT(negativeOuterWidth, -outerWidth[0]);
    cell->vertexCount = system->vertexCount;
    vertices = cell->history;
    segments = work->head.halfLife;
    VU0_LOAD_VF(vf10, sdfViewEyeVector);
    VU0_LOAD_VF(vf11, sdfViewTargetVector);
    VU0_SUB_EXTENDED(vf10, vf10, vf11);
    VU0_STORE_VF_UNCLOBBERED(vf10, cameraDirection);
    direction[0] = source->directionX;
    direction[1] = source->directionY;
    direction[2] = source->directionZ;
    VU0_LOAD_VF(vf11, direction);
    VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10_EXTENDED();
    VU0_STORE_VF_UNCLOBBERED(vf10, side);
    distance = work->head.length / work->head.halfLife;
    PCP_COPY_VECTOR(position, work->head.origin);

    for (i = 0; i < segments; i++) {
        VU0_LOAD_VF(vf10, direction);
        VU0_LOAD_VF(vf11, cameraDirection);
        VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10_EXTENDED();
        VU0_STORE_VF_UNCLOBBERED(vf10, basis);
        VU0_LOAD_VF(vf11, width);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        VU0_LOAD_VF(vf10, position);
        VU0_MOVE_VF_EXTENDED(vf11, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices + 2);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf11, vertices + 3);
        VU0_LOAD_VF(vf10, basis);
        VU0_LOAD_VF(vf11, bandWidth);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        VU0_LOAD_VF(vf10, position);
        VU0_MOVE_VF_EXTENDED(vf11, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices + 1);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf11, vertices + 4);
        VU0_LOAD_VF(vf10, basis);
        VU0_LOAD_VF(vf11, outerWidth);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        VU0_LOAD_VF(vf10, position);
        VU0_MOVE_VF_EXTENDED(vf11, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf11, vertices + 5);
        vertices += 6;
        stepLength = distance * (1.0f - effMiscRandUnitFloat(D_0034DF38) * 0.9f);
        VEC3_SPLAT(stepScale, stepLength);
        VU0_LOAD_VF(vf10, direction);
        VU0_LOAD_VF(vf11, stepScale);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_LOAD_VF(vf11, position);
        VU0_ADD_EXTENDED(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, position);
        VU0_LOAD_VF(vf10, basis);
        VU0_LOAD_VF(vf11, width);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        VU0_LOAD_VF(vf10, position);
        VU0_MOVE_VF_EXTENDED(vf11, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices + 2);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf11, vertices + 3);
        VU0_LOAD_VF(vf10, basis);
        VU0_LOAD_VF(vf11, bandWidth);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        VU0_LOAD_VF(vf10, position);
        VU0_MOVE_VF_EXTENDED(vf11, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices + 1);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf11, vertices + 4);
        VU0_LOAD_VF(vf10, basis);
        VU0_LOAD_VF(vf11, outerWidth);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        VU0_LOAD_VF(vf10, position);
        VU0_MOVE_VF_EXTENDED(vf11, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf11, vertices + 5);
        vertices += 6;
        sdfBuildVuRotationFromAxisAngle((const struct RwV3d *)cameraDirection,
            ((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f) * (70.0f * (EFF_THUNDER_HALF_TURN / 180.0f)));
        VU0_LOAD_VF(vf10, direction);
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        VU0_LOAD_VF(vf11, basis);
        VU0_DOT_XYZ(facing, vf10, vf11);
        if (facing < 0.0f) {
            join = vertices - 1;
            VU0_LOAD_VF(vf12, outerWidth);
        } else {
            join = vertices - 6;
            VU0_LOAD_VF(vf12, negativeOuterWidth);
        }
        VU0_LOAD_VF(vf10, direction);
        VU0_LOAD_VF(vf11, cameraDirection);
        VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10_EXTENDED();
        VU0_STORE_VF_UNCLOBBERED(vf10, basis);
        VU0_MOVE_VF_EXTENDED(vf11, vf12);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_LOAD_VF(vf11, join);
        VU0_ADD_EXTENDED(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, position);
    }
}


extern void func_00166C18(EffThunderCellWork *work, s32 index);

/* Delay -> active geometry -> render-cell alpha fade -> restart.
   Unlike fragment/vector state, color lives in the render cells; submit even for count <= 0. */
void effThunderCellUpdate(EffThunderCellWork *work) {
    s32 i = 0;
    s32 cellCount = work->head.cellCount;
    EffThunderCell *cell = work->cells;
    ParCell *renderCells = ((EffThunderParSystem *)work->system)->cells;

    if (cellCount > 0) {
        do {
            if (cell->delayFrames == 0) {
                if (cell->activeFrames != 0) {
                    func_00166C18(work, i);
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




struct EffThunderGroup {
    EffThunderGroupParams head;
    EffThunderFragmentWork *handles[10];
    u32 color;
};

extern void *sdfAllocSizeClassBlock(s32);
extern void *memset(void *, s32, u32);

EffThunderGroup *effThunderChainGroupCreate(EffThunderGroupParams *src) {
    EffThunderGroup *group = sdfAllocSizeClassBlock(sizeof(EffThunderGroup));
    s32 i;

    memset(group->handles, 0, sizeof(group->handles));
    group->head = *src;
    group->handles[0] = effThunderFragCreate(&group->head.params);
    func_00165668(group->handles[0]);
    for (i = 1; i < src->count - 2; i++) {
        group->handles[i] = effThunderFragCreate(&group->head.params);
        effThunderApplyFragmentColorBands(group->handles[i]);
    }
    group->handles[i] = effThunderFragCreate(&group->head.params);
    func_00165640(group->handles[i]);
    group->color = 0x80808080;
    return group;
}



extern void func_00165758(EffThunderFragmentWork *work, s32 index);
extern void func_001673D0(EffThunderFragmentWork *work, s32 index, const u128 *seed);
extern void parPrependCellNode(void *system);

extern void sdfReleaseChipBlock(void *block);

void effThunderGroupRelease(EffThunderGroup *group) {
    s32 i;

    for (i = 0; i < group->head.count - 1; i++) {
        effThunderReleaseFragmentWork(group->handles[i]);
    }
    sdfReleaseChipBlock(group);
}

u32 func_001673C0(u32 arg0) {
    return arg0;
}

/* Sets the blend colour word of a resource handle (see the blur caller in
   code_00185E18.c). */
void effSetResourceBlendColor(u32 handle, u32 color) {
    *(u32 *)(handle + 0x120) = color;
}

struct RwV3d;
extern f32 sdfViewEyeVector[4], sdfViewTargetVector[4];
extern f32 sdfSinPoly(f32);
extern void sdfBuildVuRotationFromAxisAngle(const struct RwV3d *, f32);

/* Continue a strip from the preceding segment's final five-vector row.
 * Each iteration emits two rows; seed[3] is the connection anchor used by
 * this profile, not its middle vertex. XYZ calculations and drawing ignore W.
 */
void func_001673D0(EffThunderFragmentWork *work, s32 index, const u128 *seed) {
    f32 width[4] __attribute__((aligned(16)));
    f32 outerWidth[4] __attribute__((aligned(16)));
    f32 negativeOuterWidth[4] __attribute__((aligned(16)));
    f32 position[4] __attribute__((aligned(16)));
    f32 next[4] __attribute__((aligned(16)));
    f32 basis[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    f32 side[4] __attribute__((aligned(16)));
    f32 cameraDirection[4] __attribute__((aligned(16)));
    f32 waveScale[4] __attribute__((aligned(16)));
    f32 stepScale[4] __attribute__((aligned(16)));
    f32 previousSide[4] __attribute__((aligned(16)));
    f32 delta[4] __attribute__((aligned(16)));
    EffThunderParSystem *system = work->system;
    ParCell *cell = &system->cells[index];
    u128 *vertices;
    u128 *join;
    s32 i;
    s32 segments;
    f32 distance;
    f32 amplitude;
    f32 phase;
    f32 cycle;
    f32 currentWave;
    f32 previousWave;
    f32 phaseStep;
    f32 facing;

    VEC3_SPLAT(width, work->head.bandWidth);
    VEC3_SPLAT(outerWidth, width[0] + work->head.edgeWidth);
    VEC3_SPLAT(negativeOuterWidth, -outerWidth[0]);
    cell->vertexCount = system->vertexCount;
    vertices = cell->history;
    segments = work->head.halfLife;
    VU0_LOAD_VF(vf10, work->head.end);
    VU0_LOAD_VF(vf11, seed + 3);
    VU0_SUB_EXTENDED(vf10, vf10, vf11);
    VU0_LENGTH_VF10(distance);
    VU0_NORMALIZE_VF10_EXTENDED();
    VU0_STORE_VF_UNCLOBBERED(vf10, direction);
    distance /= work->head.halfLife;
    amplitude = work->head.waveAmplitude;
    VU0_LOAD_VF(vf10, sdfViewEyeVector);
    VU0_LOAD_VF(vf11, sdfViewTargetVector);
    VU0_SUB_EXTENDED(vf10, vf10, vf11);
    VU0_STORE_VF_UNCLOBBERED(vf10, cameraDirection);
    VU0_LOAD_VF(vf11, direction);
    VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10_EXTENDED();
    VU0_STORE_VF_UNCLOBBERED(vf10, side);
    phase = effMiscRandUnitFloat(D_0034DF38) * (EFF_THUNDER_HALF_TURN * 2.0f);
    cycle = phase - (EFF_THUNDER_HALF_TURN * 2.0f);
    PCP_COPY_VECTOR(position, seed + 3);
    currentWave = amplitude * sdfSinPoly(phase);
    VEC3_SPLAT(waveScale, currentWave);
    VU0_LOAD_VF(vf10, side);
    VU0_LOAD_VF(vf11, waveScale);
    VU0_MUL_EXTENDED(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, position);
    VU0_ADD_EXTENDED(vf11, vf11, vf10);
    VU0_STORE_VF_UNCLOBBERED(vf11, position);
    VEC3_SPLAT(stepScale, distance);
    previousWave = currentWave;
    currentWave = amplitude * sdfSinPoly(phase);
    VEC3_SPLAT(waveScale, currentWave - previousWave);
    VU0_LOAD_VF(vf10, direction);
    VU0_LOAD_VF(vf11, stepScale);
    VU0_MUL_EXTENDED(vf10, vf10, vf11);
    VU0_MOVE_VF_EXTENDED(vf12, vf10);
    VU0_LOAD_VF(vf10, side);
    VU0_LOAD_VF(vf11, waveScale);
    VU0_MUL_EXTENDED(vf10, vf10, vf11);
    VU0_ADD_EXTENDED(vf10, vf10, vf12);
    VU0_MOVE_VF_EXTENDED(vf12, vf10);
    VU0_LOAD_VF(vf11, position);
    VU0_ADD_EXTENDED(vf11, vf11, vf10);
    VU0_STORE_VF_UNCLOBBERED(vf11, next);
    VU0_MOVE_VF_EXTENDED(vf10, vf12);
    VU0_NORMALIZE_VF10_EXTENDED();
    VU0_STORE_VF_UNCLOBBERED(vf10, basis);

    for (i = 0; i < segments; i++) {
        VU0_LOAD_VF(vf10, basis);
        VU0_LOAD_VF(vf11, cameraDirection);
        VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10_EXTENDED();
        VU0_STORE_VF_UNCLOBBERED(vf10, basis);
        if (i > 0) {
            VU0_MOVE_VF_EXTENDED(vf11, vf10);
            VU0_LOAD_VF(vf10, position);
            VU0_STORE_VF_UNCLOBBERED(vf10, vertices + 2);
            VU0_MOVE_VF_EXTENDED(vf10, vf11);
            VU0_LOAD_VF(vf11, width);
            VU0_MUL_EXTENDED(vf10, vf10, vf11);
            VU0_MOVE_VF_EXTENDED(vf12, vf10);
            VU0_LOAD_VF(vf10, position);
            VU0_MOVE_VF_EXTENDED(vf11, vf10);
            VU0_ADD_EXTENDED(vf10, vf10, vf12);
            VU0_STORE_VF_UNCLOBBERED(vf10, vertices + 1);
            VU0_MOVE_VF_EXTENDED(vf10, vf12);
            VU0_SUB_EXTENDED(vf11, vf11, vf10);
            VU0_STORE_VF_UNCLOBBERED(vf11, vertices + 3);
            VU0_LOAD_VF(vf10, basis);
            VU0_LOAD_VF(vf11, outerWidth);
            VU0_MUL_EXTENDED(vf10, vf10, vf11);
            VU0_MOVE_VF_EXTENDED(vf12, vf10);
            VU0_LOAD_VF(vf10, position);
            VU0_MOVE_VF_EXTENDED(vf11, vf10);
            VU0_ADD_EXTENDED(vf10, vf10, vf12);
            VU0_STORE_VF_UNCLOBBERED(vf10, vertices);
            VU0_MOVE_VF_EXTENDED(vf10, vf12);
            VU0_SUB_EXTENDED(vf11, vf11, vf10);
            VU0_STORE_VF_UNCLOBBERED(vf11, vertices + 4);
            vertices += 5;
        } else {
            PCP_COPY_VECTOR(vertices, seed);
            PCP_COPY_VECTOR(vertices + 1, seed + 1);
            PCP_COPY_VECTOR(vertices + 2, seed + 2);
            PCP_COPY_VECTOR(vertices + 3, seed + 3);
            PCP_COPY_VECTOR(vertices + 4, seed + 4);
            vertices += 5;
        }
        PCP_COPY_VECTOR(previousSide, basis);
        PCP_COPY_VECTOR(position, next);
        VU0_LOAD_VF(vf10, position);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices + 2);
        VU0_LOAD_VF(vf10, basis);
        VU0_LOAD_VF(vf11, width);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        VU0_LOAD_VF(vf10, position);
        VU0_MOVE_VF_EXTENDED(vf11, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices + 1);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf11, vertices + 3);
        VU0_LOAD_VF(vf10, basis);
        VU0_LOAD_VF(vf11, outerWidth);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        VU0_LOAD_VF(vf10, position);
        VU0_MOVE_VF_EXTENDED(vf11, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        VU0_STORE_VF_UNCLOBBERED(vf10, vertices);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf11, vertices + 4);
        vertices += 5;
        phaseStep = (EFF_THUNDER_HALF_TURN / segments) * 4.0f;
        cycle += phaseStep;
        phase += phaseStep;
        if (cycle > EFF_THUNDER_HALF_TURN) {
            cycle -= EFF_THUNDER_HALF_TURN;
            amplitude = work->head.waveAmplitude * (1.0f - effMiscRandUnitFloat(D_0034DF38) * 0.95f);
        }
        previousWave = currentWave;
        currentWave = amplitude * sdfSinPoly(phase);
        VEC3_SPLAT(waveScale, currentWave - previousWave);
        VU0_LOAD_VF(vf10, direction);
        VU0_LOAD_VF(vf11, stepScale);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        VU0_LOAD_VF(vf10, side);
        VU0_LOAD_VF(vf11, waveScale);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        VU0_STORE_VF_UNCLOBBERED(vf10, delta);
        sdfBuildVuRotationFromAxisAngle((const struct RwV3d *)cameraDirection,
            ((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f) * (20.0f * (EFF_THUNDER_HALF_TURN / 180.0f)));
        VU0_LOAD_VF(vf10, delta);
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        VU0_LOAD_VF(vf11, position);
        VU0_ADD_EXTENDED(vf11, vf11, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf11, next);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_NORMALIZE_VF10_EXTENDED();
        VU0_STORE_VF_UNCLOBBERED(vf10, basis);
        VU0_LOAD_VF(vf10, next);
        VU0_LOAD_VF(vf11, position);
        VU0_SUB_EXTENDED(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10_EXTENDED();
        VU0_LOAD_VF(vf11, previousSide);
        VU0_DOT_XYZ(facing, vf10, vf11);
        /* Choose the join edge from the new direction relative to the prior side. */
        if (facing < 0.0f) {
            join = vertices - 1;
            VU0_LOAD_VF(vf12, outerWidth);
        } else {
            join = vertices - 5;
            VU0_LOAD_VF(vf12, negativeOuterWidth);
        }
        VU0_LOAD_VF(vf10, basis);
        VU0_LOAD_VF(vf11, cameraDirection);
        VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10_EXTENDED();
        VU0_MOVE_VF_EXTENDED(vf11, vf12);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_LOAD_VF(vf11, join);
        VU0_ADD_EXTENDED(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, position);
    }
}


void effThunderUpdateChainSegments(EffThunderGroup *group) {
    s32 i = 0;
    s32 segmentCount = group->head.count - 1;
    s32 j;
    s32 fragmentCount;
    u32 color;
    f32 (*points)[4] = group->head.points;
    EffThunderFragmentWork *work;
    EffThunderFragmentWork *previous;
    ParCell *cell;

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
                func_001673D0(work, j,
                    ((EffThunderParSystem *)previous->system)->cells[j].history + ((EffThunderParSystem *)previous->system)->cells[j].vertexCount - 5);
            } else {
                func_00165758(work, j);
            }
            cell[j].color = color;
        }
        work->state.updateCount++;
        parPrependCellNode(work->system);
    }
}

/* Point history and its resource/allocation ownership share one header. */
typedef struct EffFragmentResources {
    u32 color;
    s32 surfaceIndex;
    s32 count;
    s32 activePointCount;
    s32 position;
    s32 subdivisionCount; /* 0x14: retained subdivision factor */
    u128 *points;
    u32 *colors;
    u32 resourceHandle;
    SdfMemBlock *allocation;
    u128 *endPoints;
    u32 *endColors;
} EffFragmentResources;

/* Two joined cubic segments followed by their stepping state. */
typedef struct EffGroupSlot {
    f32 position[4];
    s32 age;
    f32 scale;
    EffFragmentResources *resources;
    EffSegmentedBezierSlot curve;
    void *node;
} EffGroupSlot;

typedef struct EffGroupParams {
    f32 origin[4];
    u32 mode;
    f32 spreadX, spreadZ, height;
    u32 count;
    s32 curveFrames;
    s32 unk28;
    u32 unk2C;
    s32 fadeFrames;
    f32 width;
    s32 unk38;
    s32 unk3C;
    u32 palette[4];
} EffGroupParams;

/* Kind-two placement is 0x30 bytes; its four palette colors are separate. */
typedef char EffBezierSlotSizeCheck[sizeof(EffSegmentedBezierSlot) == 0x60 ? 1 : -1];
typedef char EffGroupSlotSizeCheck[sizeof(EffGroupSlot) == 0x80 ? 1 : -1];
typedef char EffGroupParamsSizeCheck[sizeof(EffGroupParams) == 0x50 ? 1 : -1];

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
    SdfMemBlock *allocation;
} EffGroup;

extern u32 func_00190100();
extern EffFragmentResources *func_00169940(s32, s32);
extern void func_00169B90(EffFragmentResources *, u32 *);
extern void *effEventCreate(u32, u16, EffPCPEventPlace *);

EffGroup *func_00167BF8(src, eventParams)
EffGroup *src;
void *eventParams;
{
    u32 count = src->params.count;
    SdfMemBlock *allocation = sdfAllocGeneralBlock(count * sizeof(EffGroupSlot) + sizeof(EffGroup));
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
    work->owner = (EffPCPEventOwner *)func_00190100(eventParams);
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
        slot->resources = func_00169940(a, b);
        func_00169B90(slot->resources, palette);
        slot->age = 0;
        slot->scale = 1.0f;
        slot->curve.pointIndex = 0;
        slot->curve.t = 0;
        slot->curve.parameterStep = 0;
        slot->node = effEventCreate((u32)work->owner, 2, &place);
    }
    return work;
}

/* Forward the first two parameter-table blocks as one effect-handler pair. */
void effApplyParamBlockPair(void *paramTable) {
    void *firstBlock;
    void *secondBlock;

    firstBlock = effParamTableGetBlock(paramTable, 0);
    secondBlock = effParamTableGetBlock(paramTable, 1);
    func_00167BF8(firstBlock, secondBlock);
}

EffGroup *func_00167EC0(EffGroup *src) {
    u32 count = src->params.count;
    SdfMemBlock *allocation = sdfAllocGeneralBlock(count * sizeof(EffGroupSlot) + sizeof(EffGroup));
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
        slot->resources = func_00169940(a, b);
        func_00169B90(slot->resources, palette);
        slot->age = 0;
        slot->scale = 1.0f;
        slot->curve.pointIndex = 0;
        slot->curve.t = 0;
        slot->curve.parameterStep = 0;
        slot->node = effEventCreate((u32)work->owner, 2, &place);
    }
    return work;
}


extern void effReleaseEffectResources(EffFragmentResources *work);
extern void effEventReleaseNode(void *node);
extern void func_00190118(u32 handle);

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
        func_00190118((u32)group->owner);
    }
    sdfReleaseResourceAllocation(group->allocation);
}

struct BtlUnit;
struct RwV3d;
struct EffectColorState;
/* The event holder consumes the same packed 0x30-byte record as effEvent.c. */
typedef struct {
    u8 bytes[0x30];
} __attribute__((packed)) FileRecordHeader;

extern u32 func_001619E8(void);
extern u32 effBTLFieldColorGetOriginalSelector(void);
extern u32 effBTLFieldColorGetVariantSelector(void);
extern void btlUnitGetMuzzlePosVU(struct BtlUnit *);
extern f32 sdfViewEyeVector[4], sdfViewTargetVector[4];
extern void sdfBuildVuRotationFromAxisAngle(const struct RwV3d *, f32);
extern void effInitializeColorState(struct EffectColorState *);
extern void func_00169D78(EffFragmentResources *, u128 *);
extern f32 sdfAtan2(f32, f32);
extern void func_002E7F20(f32, f32, f32);
extern void effEventCopyFileRecordHeader(void *, const void *);
extern void func_00190328(void *);
void effThunderDrawHistoryAndEndCap(EffFragmentResources *);

/* Each slot owns two joined cubic segments, a ribbon history and an end cap.
 * VU helpers use the SDF vf10/vf11/vf12 register convention; callees returning
 * a vector leave it in vf10. Keep the SDK-form stores paired with their
 * checked C readback; see docs/thunder-bezier-matching.md for the limits.
 */
typedef f32 EffThunderVector[4] __attribute__((aligned(16)));

void func_001681C0(EffGroup *group) {
    EffPCPEventPlace place __attribute__((aligned(16)));
    EffThunderVector start;
    EffThunderVector direction;
    EffThunderVector offset;
    EffThunderVector end;
    EffThunderVector point;
    EffThunderVector origin;
    EffThunderVector previous;
    EffThunderVector cameraDirection;
    EffThunderVector width;
    EffThunderVector delta;
    EffThunderVector ribbon[3];
    EffThunderVector side;
    EffThunderVector oppositeSide;
    EffThunderVector tangent;
    f32 distance, yOffset, zOffset, xOffset;
    f32 along;
    f32 spread;
    f32 pullback;
    u32 delayRange;
    u32 i, j;
    u32 count;
    f32 curveFrames;
    u32 subdivisions;
    f32 fadeStep;
    u32 mode;
    u32 color;
    EffGroupSlot *slot;
    f32 (*cap)[4];

    count = group->params.count;
    curveFrames = group->params.curveFrames;
    subdivisions = group->params.unk3C - 1;
    fadeStep = 1.0f / group->params.fadeFrames;
    delayRange = group->params.unk28;
    mode = group->params.mode;
    color = group->color;
    slot = group->slots;
    PCP_COPY_VECTOR(origin, group->params.origin);
    place.unk00[4] = 0.0f;
    place.unk00[5] = 0.0f;
    place.unk00[6] = 0.0f;
    place.unk1C = 1.0f;
    place.unk20 = 100.0f;
    place.unk24 = 100.0f;
    place.unk28 = 1.0f;
    if (func_001619E8()) {
        u32 actor;
        if (mode == 3) actor = effBTLFieldColorGetVariantSelector();
        else actor = effBTLFieldColorGetOriginalSelector();
        btlUnitGetMuzzlePosVU((struct BtlUnit *)actor);
        VU0_STORE_VF_UNCLOBBERED(vf10, start);
    } else {
        start[0] = 0.0f;
        start[1] = -80.0f;
        start[2] = -400.0f;
    }
    VU0_LOAD_VF(vf10, sdfViewEyeVector);
    VU0_LOAD_VF(vf11, sdfViewTargetVector);
    VU0_SUB_EXTENDED(vf10, vf10, vf11);
    VU0_STORE_VF_UNCLOBBERED(vf10, cameraDirection);
    VEC3_SPLAT(width, group->params.width * 0.5f);
    for (i = 0; i < count; i++, slot++) {
        if (slot->curve.parameterStep == 0.0f) {
            yOffset = -group->params.height * effMiscRandUnitFloat(D_0034DF38);
            zOffset = group->params.spreadZ * ((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f);
            xOffset = group->params.spreadX * ((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f);
            end[0] = origin[0] + xOffset;
            end[1] = origin[1] - yOffset;
            end[2] = origin[2] + zOffset;
            VU0_LOAD_VF(vf10, end);
            VU0_LOAD_VF(vf11, start);
            VU0_SUB_EXTENDED(vf10, vf10, vf11);
            VU0_LENGTH_VF10(distance);
            VU0_NORMALIZE_VF10_EXTENDED();
            VU0_STORE_VF_UNCLOBBERED(vf10, direction);
            switch (mode) {
            case 0:
                offset[0] = ((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f);
                offset[1] = -effMiscRandUnitFloat(D_0034DF38);
                offset[2] = 0.0f;
                VU0_LOAD_VF(vf10, offset);
                VU0_NORMALIZE_VF10_EXTENDED();
                VU0_STORE_VF_UNCLOBBERED(vf10, offset);
                sdfBuildVuRotationFromAxisAngle((const struct RwV3d *)direction, ((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f) * 0.5235987f);
                spread = 50.0f;
                slot->curve.controlPoints[0].x = offset[0] * spread + start[0];
                slot->curve.controlPoints[0].y = offset[1] * spread + start[1];
                slot->curve.controlPoints[0].z = offset[2] * spread + start[2];
                if (slot->curve.controlPoints[0].y > -50.0f) slot->curve.controlPoints[0].y = -50.0f;
                spread = 200.0f;
                pullback = 150.0f;
                slot->curve.controlPoints[1].x = (offset[0] * spread - direction[0] * pullback) + start[0];
                slot->curve.controlPoints[1].y = (offset[1] * spread - direction[1] * pullback) + start[1];
                slot->curve.controlPoints[1].z = (offset[2] * spread - direction[2] * pullback) + start[2];
                if (slot->curve.controlPoints[1].y > -30.0f) slot->curve.controlPoints[1].y = -30.0f;
                spread = 300.0f;
                slot->curve.controlPoints[2].x = offset[0] * spread + start[0];
                slot->curve.controlPoints[2].y = offset[1] * spread + start[1];
                slot->curve.controlPoints[2].z = offset[2] * spread + start[2];
                if (slot->curve.controlPoints[2].y > -20.0f) slot->curve.controlPoints[2].y = -20.0f;
                along = distance * 0.25f;
                spread = 200.0f;
                VU0_LOAD_VF(vf10, offset);
                VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
                VU0_STORE_VF_UNCLOBBERED(vf10, offset);
                slot->curve.controlPoints[3].x = spread * offset[0] + along * direction[0] + start[0];
                slot->curve.controlPoints[3].y = spread * offset[1] + along * direction[1] + start[1];
                slot->curve.controlPoints[3].z = spread * offset[2] + along * direction[2] + start[2];
                if (slot->curve.controlPoints[3].y > -10.0f) slot->curve.controlPoints[3].y = -10.0f;
                along = distance * 0.5f;
                spread = 150.0f;
                VU0_LOAD_VF(vf10, offset);
                VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
                VU0_STORE_VF_UNCLOBBERED(vf10, offset);
                slot->curve.controlPoints[4].x = spread * offset[0] + along * direction[0] + start[0];
                slot->curve.controlPoints[4].y = spread * offset[1] + along * direction[1] + start[1];
                slot->curve.controlPoints[4].z = spread * offset[2] + along * direction[2] + start[2];
                if (slot->curve.controlPoints[4].y > -10.0f) slot->curve.controlPoints[4].y = -10.0f;
                along = distance * 0.75f;
                spread = 65.0f;
                VU0_LOAD_VF(vf10, offset);
                VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
                VU0_STORE_VF_UNCLOBBERED(vf10, offset);
                slot->curve.controlPoints[5].x = spread * offset[0] + along * direction[0] + start[0];
                slot->curve.controlPoints[5].y = spread * offset[1] + along * direction[1] + start[1];
                slot->curve.controlPoints[5].z = spread * offset[2] + along * direction[2] + start[2];
                if (slot->curve.controlPoints[5].y > -10.0f) slot->curve.controlPoints[5].y = -10.0f;
                along = distance;
                slot->curve.controlPoints[6].x = along * direction[0] + start[0];
                slot->curve.controlPoints[6].y = along * direction[1] + start[1];
                slot->curve.controlPoints[6].z = along * direction[2] + start[2];
                if (slot->curve.controlPoints[6].y > -10.0f) slot->curve.controlPoints[6].y = -10.0f;
                break;
            case 1:
                offset[0] = ((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f) * 0.5f;
                offset[1] = -effMiscRandUnitFloat(D_0034DF38);
                offset[2] = 0.0f;
                VU0_LOAD_VF(vf10, offset);
                VU0_NORMALIZE_VF10_EXTENDED();
                VU0_STORE_VF_UNCLOBBERED(vf10, offset);
                spread = 25.0f;
                slot->curve.controlPoints[0].x = offset[0] * spread + start[0];
                slot->curve.controlPoints[0].y = offset[1] * spread + start[1];
                slot->curve.controlPoints[0].z = offset[2] * spread + start[2];
                spread = 100.0f;
                pullback = 150.0f;
                slot->curve.controlPoints[1].x = (offset[0] * spread - direction[0] * pullback) + start[0];
                slot->curve.controlPoints[1].y = (offset[1] * spread - direction[1] * pullback) + start[1];
                slot->curve.controlPoints[1].z = (offset[2] * spread - direction[2] * pullback) + start[2];
                spread = 400.0f;
                slot->curve.controlPoints[2].x = offset[0] * spread + start[0];
                slot->curve.controlPoints[2].y = offset[1] * spread + start[1];
                slot->curve.controlPoints[2].z = offset[2] * spread + start[2];
                along = distance * 0.25f;
                spread = 450.0f;
                slot->curve.controlPoints[3].x = offset[0] * spread + along * direction[0] + start[0];
                slot->curve.controlPoints[3].y = offset[1] * spread + along * direction[1] + start[1];
                slot->curve.controlPoints[3].z = offset[2] * spread + along * direction[2] + start[2];
                along = distance * 0.5f;
                spread = 500.0f;
                slot->curve.controlPoints[4].x = offset[0] * spread + along * direction[0] + start[0];
                slot->curve.controlPoints[4].y = offset[1] * spread + along * direction[1] + start[1];
                slot->curve.controlPoints[4].z = offset[2] * spread + along * direction[2] + start[2];
                along = distance * 0.75f;
                spread = 300.0f;
                slot->curve.controlPoints[5].x = offset[0] * spread + along * direction[0] + start[0];
                slot->curve.controlPoints[5].y = offset[1] * spread + along * direction[1] + start[1];
                slot->curve.controlPoints[5].z = offset[2] * spread + along * direction[2] + start[2];
                along = distance;
                slot->curve.controlPoints[6].x = along * direction[0] + start[0];
                slot->curve.controlPoints[6].y = along * direction[1] + start[1];
                slot->curve.controlPoints[6].z = along * direction[2] + start[2];
                break;
            case 2:
                offset[0] = ((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f);
                offset[1] = -effMiscRandUnitFloat(D_0034DF38);
                offset[2] = 0.0f;
                VU0_LOAD_VF(vf10, offset);
                VU0_NORMALIZE_VF10_EXTENDED();
                VU0_STORE_VF_UNCLOBBERED(vf10, offset);
                spread = 20.0f;
                slot->curve.controlPoints[0].x = offset[0] * spread + start[0];
                slot->curve.controlPoints[0].y = offset[1] * spread + start[1];
                slot->curve.controlPoints[0].z = offset[2] * spread + start[2];
                along = distance * 0.15f;
                spread = 50.0f;
                slot->curve.controlPoints[1].x = offset[0] * spread + along * direction[0] + start[0];
                slot->curve.controlPoints[1].y = offset[1] * spread + along * direction[1] + start[1];
                slot->curve.controlPoints[1].z = offset[2] * spread + along * direction[2] + start[2];
                along = distance * 0.3f;
                spread = 80.0f;
                slot->curve.controlPoints[2].x = offset[0] * spread + along * direction[0] + start[0];
                slot->curve.controlPoints[2].y = offset[1] * spread + along * direction[1] + start[1];
                slot->curve.controlPoints[2].z = offset[2] * spread + along * direction[2] + start[2];
                along = distance * 0.45f;
                spread = 60.0f;
                slot->curve.controlPoints[3].x = offset[0] * spread + along * direction[0] + start[0];
                slot->curve.controlPoints[3].y = offset[1] * spread + along * direction[1] + start[1];
                slot->curve.controlPoints[3].z = offset[2] * spread + along * direction[2] + start[2];
                along = distance * 0.6f;
                spread = 40.0f;
                slot->curve.controlPoints[4].x = offset[0] * spread + along * direction[0] + start[0];
                slot->curve.controlPoints[4].y = offset[1] * spread + along * direction[1] + start[1];
                slot->curve.controlPoints[4].z = offset[2] * spread + along * direction[2] + start[2];
                along = distance * 0.75f;
                spread = 20.0f;
                slot->curve.controlPoints[5].x = offset[0] * spread + along * direction[0] + start[0];
                slot->curve.controlPoints[5].y = offset[1] * spread + along * direction[1] + start[1];
                slot->curve.controlPoints[5].z = offset[2] * spread + along * direction[2] + start[2];
                along = distance;
                slot->curve.controlPoints[6].x = along * direction[0] + start[0];
                slot->curve.controlPoints[6].y = along * direction[1] + start[1];
                slot->curve.controlPoints[6].z = along * direction[2] + start[2];
                break;
            case 3:
                offset[0] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
                offset[1] = -(effMiscRandUnitFloat(D_0034DF38) * 0.3f + 0.7f);
                offset[2] = 0.0f;
                VU0_LOAD_VF(vf10, offset);
                VU0_NORMALIZE_VF10_EXTENDED();
                VU0_STORE_VF_UNCLOBBERED(vf10, offset);
                spread = 25.0f;
                slot->curve.controlPoints[0].x = offset[0] * spread + start[0];
                slot->curve.controlPoints[0].y = offset[1] * spread + start[1];
                slot->curve.controlPoints[0].z = offset[2] * spread + start[2];
                spread = 100.0f;
                pullback = 150.0f;
                slot->curve.controlPoints[1].x = (offset[0] * spread - direction[0] * pullback) + start[0];
                slot->curve.controlPoints[1].y = (offset[1] * spread - direction[1] * pullback) + start[1];
                slot->curve.controlPoints[1].z = (offset[2] * spread - direction[2] * pullback) + start[2];
                spread = 400.0f;
                slot->curve.controlPoints[2].x = offset[0] * spread + start[0];
                slot->curve.controlPoints[2].y = offset[1] * spread + start[1];
                slot->curve.controlPoints[2].z = offset[2] * spread + start[2];
                along = distance * 0.25f;
                spread = 450.0f;
                slot->curve.controlPoints[3].x = offset[0] * spread + along * direction[0] + start[0];
                slot->curve.controlPoints[3].y = offset[1] * spread + along * direction[1] + start[1];
                slot->curve.controlPoints[3].z = offset[2] * spread + along * direction[2] + start[2];
                along = distance * 0.5f;
                spread = 500.0f;
                slot->curve.controlPoints[4].x = offset[0] * spread + along * direction[0] + start[0];
                slot->curve.controlPoints[4].y = offset[1] * spread + along * direction[1] + start[1];
                slot->curve.controlPoints[4].z = offset[2] * spread + along * direction[2] + start[2];
                along = distance * 0.75f;
                spread = 300.0f;
                slot->curve.controlPoints[5].x = offset[0] * spread + along * direction[0] + start[0];
                slot->curve.controlPoints[5].y = offset[1] * spread + along * direction[1] + start[1];
                slot->curve.controlPoints[5].z = offset[2] * spread + along * direction[2] + start[2];
                along = distance;
                slot->curve.controlPoints[6].x = along * direction[0] + start[0];
                slot->curve.controlPoints[6].y = along * direction[1] + start[1];
                slot->curve.controlPoints[6].z = along * direction[2] + start[2];
                break;
            case 4:
                along = distance * 0.14f;
                slot->curve.controlPoints[0].x = along * direction[0] + start[0];
                slot->curve.controlPoints[0].y = along * direction[1] + start[1];
                slot->curve.controlPoints[0].z = along * direction[2] + start[2];
                along = distance * 0.28f;
                slot->curve.controlPoints[1].x = along * direction[0] + start[0];
                slot->curve.controlPoints[1].y = along * direction[1] + start[1];
                slot->curve.controlPoints[1].z = along * direction[2] + start[2];
                along = distance * 0.42f;
                slot->curve.controlPoints[2].x = along * direction[0] + start[0];
                slot->curve.controlPoints[2].y = along * direction[1] + start[1];
                slot->curve.controlPoints[2].z = along * direction[2] + start[2];
                along = distance * 0.56f;
                slot->curve.controlPoints[3].x = along * direction[0] + start[0];
                slot->curve.controlPoints[3].y = along * direction[1] + start[1];
                slot->curve.controlPoints[3].z = along * direction[2] + start[2];
                along = distance * 0.7f;
                slot->curve.controlPoints[4].x = along * direction[0] + start[0];
                slot->curve.controlPoints[4].y = along * direction[1] + start[1];
                slot->curve.controlPoints[4].z = along * direction[2] + start[2];
                along = distance * 0.84f;
                slot->curve.controlPoints[5].x = along * direction[0] + start[0];
                slot->curve.controlPoints[5].y = along * direction[1] + start[1];
                slot->curve.controlPoints[5].z = along * direction[2] + start[2];
                along = distance;
                slot->curve.controlPoints[6].x = along * direction[0] + start[0];
                slot->curve.controlPoints[6].y = along * direction[1] + start[1];
                slot->curve.controlPoints[6].z = along * direction[2] + start[2];
                break;
            }
            slot->curve.pointIndex = 0;
            slot->curve.t = 0.0f;
            slot->curve.parameterStep = 3.0f / (curveFrames * (subdivisions + 1));
            effStepBezierSlotSegment(&slot->curve, point);
            slot->position[0] = point[0];
            slot->position[1] = point[1];
            slot->position[2] = point[2];
            slot->age = -(effMiscRand(D_0034DF38) % delayRange);
            effInitializeColorState((struct EffectColorState *)slot->resources);
        } else if (slot->age++ >= 0) {
            if (slot->curve.t >= 1.0f && slot->curve.pointIndex >= 3) {
                if (slot->scale > fadeStep) {
                    slot->scale -= fadeStep;
                    slot->resources->color = ((u32)(slot->scale * 128.0f) << 24) | 0x808080;
                    slot->resources->color = effMultiplyPackedColors(color, slot->resources->color);
                } else {
                    effInitializeColorState((struct EffectColorState *)slot->resources);
                }
            } else {
                previous[0] = slot->position[0];
                previous[1] = slot->position[1];
                previous[2] = slot->position[2];
                for (j = 0; j < subdivisions; j++) {
                    effStepBezierSlotSegment(&slot->curve, point);
                    VU0_LOAD_VF(vf10, point);
                    VU0_MOVE_VF_EXTENDED(vf12, vf10);
                    VU0_LOAD_VF(vf11, previous);
                    VU0_SUB_EXTENDED(vf10, vf10, vf11);
                    VU0_LOAD_VF(vf11, cameraDirection);
                    VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
                    VU0_NORMALIZE_VF10_EXTENDED();
                    VU0_LOAD_VF(vf11, width);
                    VU0_MUL_EXTENDED(vf10, vf10, vf11);
                    VU0_MOVE_VF_EXTENDED(vf2, vf10);
                    VU0_MOVE_VF_EXTENDED(vf10, vf12);
                    VU0_MOVE_VF_EXTENDED(vf12, vf2);
                    VU0_STORE_VF_UNCLOBBERED(vf10, ribbon[1]);
                    VU0_MOVE_VF_EXTENDED(vf11, vf10);
                    VU0_ADD_EXTENDED(vf10, vf10, vf12);
                    VU0_STORE_VF_UNCLOBBERED(vf10, ribbon[0]);
                    VU0_MOVE_VF_EXTENDED(vf10, vf12);
                    VU0_SUB_EXTENDED(vf11, vf11, vf10);
                    VU0_STORE_VF_UNCLOBBERED(vf11, ribbon[2]);
                    PCP_COPY_VECTOR(previous, point);
                    func_00169D78(slot->resources, (u128 *)ribbon);
                }
                effStepBezierSlotSegment(&slot->curve, point);
                VU0_LOAD_VF(vf10, point);
                VU0_MOVE_VF_EXTENDED(vf12, vf10);
                VU0_LOAD_VF(vf11, previous);
                VU0_SUB_EXTENDED(vf10, vf10, vf11);
                VU0_NORMALIZE_VF10_EXTENDED();
                VU0_STORE_VF_UNCLOBBERED(vf10, tangent);
                VU0_LOAD_VF(vf11, cameraDirection);
                VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
                VU0_NORMALIZE_VF10_EXTENDED();
                VU0_STORE_VF_UNCLOBBERED(vf10, side);
                VU0_LOAD_VF(vf11, width);
                VU0_MUL_EXTENDED(vf10, vf10, vf11);
                VU0_MOVE_VF_EXTENDED(vf2, vf10);
                VU0_MOVE_VF_EXTENDED(vf10, vf12);
                VU0_MOVE_VF_EXTENDED(vf12, vf2);
                VU0_STORE_VF_UNCLOBBERED(vf10, ribbon[1]);
                VU0_MOVE_VF_EXTENDED(vf11, vf10);
                VU0_ADD_EXTENDED(vf10, vf10, vf12);
                VU0_STORE_VF_UNCLOBBERED(vf10, ribbon[0]);
                VU0_MOVE_VF_EXTENDED(vf10, vf12);
                VU0_SUB_EXTENDED(vf11, vf11, vf10);
                VU0_STORE_VF_UNCLOBBERED(vf11, ribbon[2]);
                func_00169D78(slot->resources, (u128 *)ribbon);
                cap = (f32 (*)[4])slot->resources->endPoints;
                slot->position[0] = point[0];
                slot->position[1] = point[1];
                slot->position[2] = point[2];
                PCP_COPY_VECTOR(cap[0], ribbon[1]);
                PCP_COPY_VECTOR(cap[1], ribbon[0]);
                PCP_COPY_VECTOR(cap[7], ribbon[2]);
                oppositeSide[0] = -side[0];
                oppositeSide[1] = -side[1];
                oppositeSide[2] = -side[2];
                oppositeSide[3] = -side[3];
                VU0_LOAD_VF(vf12, ribbon[1]);
                VU0_LOAD_VF(vf10, side);
                VU0_LOAD_VF(vf11, tangent);
                VU0_LERP_VF10(0.3333333333f);
                VU0_NORMALIZE_VF10_EXTENDED();
                VU0_LOAD_VF(vf11, width);
                VU0_MUL_EXTENDED(vf10, vf10, vf11);
                VU0_ADD_EXTENDED(vf10, vf10, vf12);
                VU0_STORE_VF_UNCLOBBERED(vf10, cap[2]);
                VU0_LOAD_VF(vf10, side);
                VU0_LOAD_VF(vf11, tangent);
                VU0_LERP_VF10(0.6666666667f);
                VU0_NORMALIZE_VF10_EXTENDED();
                VU0_LOAD_VF(vf11, width);
                VU0_MUL_EXTENDED(vf10, vf10, vf11);
                VU0_ADD_EXTENDED(vf10, vf10, vf12);
                VU0_STORE_VF_UNCLOBBERED(vf10, cap[3]);
                VU0_LOAD_VF(vf10, tangent);
                VU0_MUL_EXTENDED(vf10, vf10, vf11);
                VU0_ADD_EXTENDED(vf10, vf10, vf12);
                VU0_STORE_VF_UNCLOBBERED(vf10, cap[4]);
                VU0_LOAD_VF(vf10, oppositeSide);
                VU0_LOAD_VF(vf11, tangent);
                VU0_LERP_VF10(0.6666666667f);
                VU0_NORMALIZE_VF10_EXTENDED();
                VU0_LOAD_VF(vf11, width);
                VU0_MUL_EXTENDED(vf10, vf10, vf11);
                VU0_ADD_EXTENDED(vf10, vf10, vf12);
                VU0_STORE_VF_UNCLOBBERED(vf10, cap[5]);
                VU0_LOAD_VF(vf10, oppositeSide);
                VU0_LOAD_VF(vf11, tangent);
                VU0_LERP_VF10(0.3333333333f);
                VU0_NORMALIZE_VF10_EXTENDED();
                VU0_LOAD_VF(vf11, width);
                VU0_MUL_EXTENDED(vf10, vf10, vf11);
                VU0_ADD_EXTENDED(vf10, vf10, vf12);
                VU0_STORE_VF_UNCLOBBERED(vf10, cap[6]);
            }
            if (slot->curve.t >= 1.0f && slot->curve.pointIndex >= 3) {
                place.unk00[0] = slot->position[0];
                place.unk00[1] = slot->position[1];
                place.unk00[2] = slot->position[2];
                place.unk00[3] = 0.0f;
                place.color = color;
                delta[0] = start[0] - place.unk00[0];
                delta[2] = start[2] - place.unk00[2];
                func_002E7F20(0.0f, sdfAtan2(delta[0], delta[2]), 0.0f);
                VU0_STORE_VF_UNCLOBBERED(vf10, &place.unk00[4]);
                effEventCopyFileRecordHeader((FileRecordHeader *)slot->node, (const FileRecordHeader *)&place);
                func_00190328(slot->node);
            }
            effThunderDrawHistoryAndEndCap(slot->resources);
        }
    }
}


void func_00169928(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00169938(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x54) = arg1;
}

typedef struct EffThunderDrawParams {
    s16 primitiveCount;
    s16 pointCount;
    u16 flags;
    u8 pad06[2];
    u32 color;
    const u32 *primitiveIndices; /* 0x0C: packed triangle-index words */
    u128 *points;
    u8 pad14[0xC];
    u32 *colors;
    u8 pad24[0xC];
} EffThunderDrawParams;

extern EffThunderDrawParams D_003D64F0;
extern EffThunderDrawParams D_003D6520;

struct SdfTextParam;
extern SdfAsset *sdfCreateAssetWithDrawEntries(void);
extern void func_002DA420(struct SdfTextParam *, f32);
extern const f32 D_00354860[8][4];
extern const u32 D_003547D0[28];
extern const u32 D_00354840[8];
extern void *memcpy(void *, const void *, u32);

/* Lay out history vertices/colors, eight cap vertices/colors, then the owner.
 * The two global draw descriptors are reset through their native 0x2C prefix.
 */
EffFragmentResources *func_00169940(s32 historyLength, s32 subdivisions) {
    s32 count = historyLength * subdivisions * 3 + 6;
    u32 colorBytes = count * sizeof(u32);
    u32 bufferBytes = count * (sizeof(u128) + sizeof(u32)) + 8 * (sizeof(u128) + sizeof(u32));
    SdfMemBlock *allocation = sdfAllocGeneralBlock(bufferBytes + sizeof(EffFragmentResources));
    u8 *storage = (u8 *)sdfResourceRetainAddress(allocation);
    EffFragmentResources *history = (EffFragmentResources *)(storage + bufferBytes);
    SdfAsset *asset;

    history->points = (u128 *)storage;
    storage += count * sizeof(u128);
    history->endPoints = (u128 *)storage;
    storage += 8 * sizeof(u128);
    history->colors = (u32 *)storage;
    storage += colorBytes;
    history->endColors = (u32 *)storage;
    history->surfaceIndex = 2;
    history->count = count;
    history->position = 3;
    history->subdivisionCount = subdivisions;
    history->color = 0x80808080;
    history->allocation = allocation;
    history->activePointCount = 0;
    memcpy(history->endPoints, D_00354860, sizeof(D_00354860));
    asset = sdfCreateAssetWithDrawEntries();
    history->resourceHandle = (u32)asset;
    func_002DA420((struct SdfTextParam *)asset, 1.0f);
    memset(&D_003D64F0, 0, 0x2C);
    D_003D64F0.flags = 0x4000;
    D_003D64F0.primitiveIndices = D_003547D0;
    memset(&D_003D6520, 0, 0x2C);
    D_003D6520.flags = 0x4000;
    D_003D6520.primitiveIndices = D_00354840;
    D_003D6520.primitiveCount = 6;
    D_003D6520.pointCount = 8;
    return history;
}

/* The packed color state is identical to the sequel's effect state layout. */
typedef struct EffectColorState {
    u32 color;    /* 0x00 */
    u8 pad04[8];
    u32 valueC;   /* 0x0C */
    u32 mode;     /* 0x10 */
} EffectColorState;

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

void func_00169B90(EffFragmentResources *history, u32 *gradientColors) {
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

void func_00169D78(EffFragmentResources *history, u128 *source) {
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



extern SdfPoolNode *D_003548E0[];
extern s32 sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(SdfListHead *);
extern void sdfConsAppendClearPacket(s32, s32 (*)(s32));
extern void sdfConsAppendAssetPacket(s32, void *, s32 (*)(s32));
extern void sdfAppendPacket(SdfListHead *, u32);
extern s32 func_0015FE20(EffThunderDrawParams *);

/* Render the two runs of a wrapped three-point history and its end cap. */
void effThunderDrawHistoryAndEndCap(EffFragmentResources *history) {
    s32 start[4];
    s32 length[4];
    SdfListHead *list;
    s32 recent;
    s32 wrapped;
    s32 i;
    s32 remaining;
    EffThunderDrawParams *draw;
    u32 *colors;
    u32 edgeColor;
    u32 centerColor;
    SdfPoolNode *surface;

    if (history->activePointCount == 0) {
        return;
    }
    list = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    sdfConsAppendClearPacket((s32)list, 0);
    sdfConsAppendAssetPacket((s32)list, (void *)history->resourceHandle, 0);
    recent = history->activePointCount;
    start[0] = history->position - recent;
    if (start[0] < 3) {
        wrapped = start[0] - 3;
        start[1] = 0;
        start[0] = wrapped + history->count;
        length[0] = -wrapped;
        length[1] = recent - length[0] + 3;
    } else {
        length[0] = recent;
        length[1] = 0;
    }
    D_003D64F0.colors = history->colors;
    D_003D64F0.color = history->color;
    for (i = 0; i < 2; i++) {
        draw = &D_003D64F0;
        remaining = length[i];
        draw->primitiveCount = 16;
        draw->points = history->points + start[i];
        draw->pointCount = 15;
        while (remaining >= 15) {
            remaining -= 12;
            sdfAppendPacket(list, func_0015FE20(&D_003D64F0));
            D_003D64F0.points += 12;
            D_003D64F0.colors += 12;
        }
        if (remaining >= 6) {
            draw->primitiveCount = (remaining / 3) * 4 - 4;
            draw->pointCount = remaining;
            sdfAppendPacket(list, func_0015FE20(draw));
            draw->colors += remaining;
        }
    }
    colors = history->endColors;
    edgeColor = D_003D64F0.colors[-3];
    centerColor = D_003D64F0.colors[-2];
    colors[0] = centerColor;
    colors[7] = colors[6] = colors[5] = colors[4] =
        colors[3] = colors[2] = colors[1] = edgeColor;
    D_003D6520.colors = colors;
    D_003D6520.points = history->endPoints;
    D_003D6520.color = history->color;
    sdfAppendPacket(list, func_0015FE20(&D_003D6520));
    surface = D_003548E0[history->surfaceIndex];
    surface->append((SdfListHead *)surface, list);
}


extern EffRecordPool *effRecordPoolCreateTriple(s32 triangleCount);

/* Clone the 0x30-byte parameter block, create the record pool and clear every particle's age. */
PcpFlashTrianglePulseWork *effFlashRecordCreate(src)
    PcpFlashTrianglePulseWork *src;
{
    SdfMemBlock *allocation = sdfAllocGeneralBlock(src->particleCount * sizeof(PcpFlashPulseParticle) + sizeof(PcpFlashTrianglePulseWork));
    PcpFlashTrianglePulseWork *work = (PcpFlashTrianglePulseWork *)sdfResourceRetainAddress(allocation);
    EffRecordPool *record;
    u32 i;

    memcpy(work, src, 0x30);
    work->parts = (PcpFlashPulseParticle *)(work + 1);
    work->tintColor = 0x80808080;
    work->allocationHandle = allocation;
    work->renderScale = 1.0f;
    work->updateCount = 0;
    record = effRecordPoolCreateTriple(work->particleCount);
    work->resourceHandle = record;
    record->drawMode = work->drawMode;
    for (i = 0; i < work->particleCount; i++) {
        work->parts[i].age = 0;
    }
    return work;
}
