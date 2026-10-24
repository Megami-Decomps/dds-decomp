#include "sdk_ps2.h"
#include "common.h"
#include "sdf_vu0.h"
#include "eff_param.h"
#include "sdf_vu_matrix.h"
#include "btl_effect_position.h"
#include "sdf_trig.h"
#include "eff_random.h"
#include "eff_pcp_event_place.h"
#include "sdf_quaternion.h"
#include "sdf_asset_state.h"
#include "sdf_asset_create.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "sdf_packet_append.h"
#include "eff_thunder_vector.h"
#include "par_cell_api.h"
#include "sdf_resource.h"
#include "btl_sound.h"
#include "eff.h"
#include "eff_thunder_particle_records.h"
#include "eff_pcp_thunder_cell.h"
#include "eff_thunder_draw_params.h"
#include "eff_math.h"
#include "eff_thunder_fragment.h"
#include "eff_event.h"
#include "eff_event_sound.h"
#include "eff_pcp_flash.h"
#include "eff_pcp_thunder_group.h"
#include "pcp_vu0.h"

typedef char EffThunderFragmentParamsSizeCheck[
    sizeof(EffThunderFragmentParams) == 0x54 ? 1 : -1];
typedef char EffThunderFragmentParamsColorOffsetsCheck[
    ((u32)&((EffThunderFragmentParams *)0)->centerColorWord == 0x40 &&
     (u32)&((EffThunderFragmentParams *)0)->middleColorWord == 0x48 &&
     (u32)&((EffThunderFragmentParams *)0)->edgeColorWord == 0x50) ? 1 : -1];

extern EffRandState effDefaultRandomState[];

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

/* Create the first vector variant from the first packed parameter block. */
void *effPCPThunderSpherePolyCreatePCF(const EffParamTable *parameterTable) {
    void *parameters;

    parameters = dds3EffectPCPDataGetData(parameterTable, EFF_THUNDER_PARAMETER_BLOCK);
    return effPCPThunderSpherePolyCreate(parameters);
}
/* Recreate vector cell-system work from its retained parameter prefix. */
void *effPCPThunderSpherePolyCopy(void *source) {
    return effPCPThunderSpherePolyCreate(source);
}

/* Release the cell system before releasing the containing work allocation. */
void effPCPThunderSpherePolyDelete(EffThunderVectorWork *work) {
    dds3TrackPolyDelete(work->cellSystem);
    sdfMemFreeDirect(work->allocationHandle);
}

/* vu0 routine: copy the entire quadword, including its fourth component. */
void effPCPThunderSpherePolySetPosition(void *destination, void *source) {
    SDF_VU0_COPYVECTOR(destination, source);
}

/* Set the packed modulation color applied to every vector cell. */
void effPCPThunderSpherePolySetColor(EffThunderVectorWork *work, u32 tintColor) {
    work->tintColor = tintColor;
}

/* Absolute radius/height scaling from the retained constructor values. */
void effPCPThunderSpherePolySetScale(f32 factor, EffThunderVectorWork *work) {
    work->head.vector.radiusScale = work->baseRadiusScale * factor;
    work->head.vector.heightScale = work->baseHeightScale * factor;
}

/* The copied parameter head starts at the beginning of the full work. */
EffThunderParameterHead *effPCPThunderSpherePolyGetParam(EffThunderVectorWork *work) {
    return &work->head;
}

extern void dds3TrackPolyVCSet(ParSystem *work, s32 sub, void *dispatchArgA, void *dispatchArgB);
extern u32 effMagaCylinderCalcColor(u32 color, u32 param);


/* Resample timing, normalize X/Z before assigning Y, then sample the axis.
   Both timing moduli are unchecked; the fourth scratch lane is not initialized. */
void effPCPThunderSphereInit(EffThunderVectorWork *work, s32 index) {
    EffThunderVectorCell *cell = work->cells + index;
    f32 scratchVector[EFF_THUNDER_VECTOR_COMPONENTS];

    cell->delayFrames = dds3GetRandA((EffRandState *)effDefaultRandomState) % work->head.vector.startDelayRange;
    cell->activeFrames = dds3GetRandA((EffRandState *)effDefaultRandomState) % work->head.vector.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
    scratchVector[0] = (dds3GetRandFloatA(effDefaultRandomState) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN;
    scratchVector[1] = 0;
    scratchVector[2] = (dds3GetRandFloatA(effDefaultRandomState) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN;
    SDF_VU0_LOADVECTOR(scratchVector);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(scratchVector);
    scratchVector[1] = work->head.vector.heightScale * EFF_THUNDER_HALF_SCALE * ((dds3GetRandFloatA(effDefaultRandomState) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN);
    cell->placementVector[0] = scratchVector[0];
    cell->placementVector[1] = scratchVector[1];
    cell->placementVector[2] = scratchVector[2];
    scratchVector[0] = (dds3GetRandFloatA(effDefaultRandomState) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_AXIS_JITTER;
    scratchVector[1] = 1.0f;
    scratchVector[2] = (dds3GetRandFloatA(effDefaultRandomState) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_AXIS_JITTER;
    SDF_VU0_LOADVECTOR(scratchVector);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(scratchVector);
    cell->rotationAxis[0] = scratchVector[0];
    cell->rotationAxis[1] = scratchVector[1];
    cell->rotationAxis[2] = scratchVector[2];
    cell->rotationScale = work->head.vector.rotationScale * ((dds3GetRandFloatA(effDefaultRandomState) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_ROTATION_JITTER + 1.0f);
    cell->radius = work->head.vector.radiusScale * EFF_THUNDER_HALF_SCALE * ((dds3GetRandFloatA(effDefaultRandomState) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_RADIUS_JITTER + 1.0f);
    cell->color = EFF_THUNDER_NEUTRAL_COLOR;
}

extern f32 aViewTarget[4], sdfViewTargetVector[4];
extern void SetRotateMatrixXYZ(const struct RwV3d *);

/* Sample a width multiplier, then bend the cell's placement vector around
 * a perturbed axis while emitting five-vector rows into its kind-4 history.
 */
void effPCPThunderSphereRotate(EffThunderVectorWork *work, s32 index) {
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
    ParSystem *system = work->cellSystem;
    ParCell *cell = &system->cells[index];
    EffThunderVectorCell *source;
    u128 *vertices;
    s32 count;
    s32 i;
    f32 widthScale;
    f32 rotationScale;
    f32 radius;

    count = system->vertexWordCount / 5;
    cell->vertexCount = system->vertexWordCount;
    source = &work->cells[index];
    vertices = cell->history;
    widthScale = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f * 0.2f + 1.0f;
    VEC3_SPLAT(width, work->head.vector.bandWidth * widthScale);
    VEC3_SPLAT(outerWidth, width[0] + work->head.vector.edgeWidth * widthScale);
    originalAxis[0] = source->rotationAxis[0];
    originalAxis[1] = source->rotationAxis[1];
    originalAxis[2] = source->rotationAxis[2];
    originalAxis[3] = 0.0f;
    rotationScale = -source->rotationScale;
    radius = source->radius;
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(sdfViewTargetVector);
    SDF_VU0_SUBVECTOR_EXTENDED();
    SDF_VU0_STOREVECTOR_NOMEM(cameraDirection);
    placement[0] = source->placementVector[0];
    placement[1] = 0.0f;
    placement[2] = source->placementVector[2];
    SDF_VU0_COPYVECTOR(origin, work->head.vector.origin);
    origin[1] += source->placementVector[1];

    for (i = 0; i < count; i++) {
        position[0] = origin[0] + placement[0] * radius;
        position[1] = origin[1] + placement[1] * radius;
        position[2] = origin[2] + placement[2] * radius;
        SDF_VU0_COPYVECTOR(axis, originalAxis);
        rotation[0] = dds3GetRandFloatA(effDefaultRandomState) * (75.0f * (EFF_THUNDER_HALF_TURN / 180.0f));
        rotation[1] = dds3GetRandFloatA(effDefaultRandomState) * (75.0f * (EFF_THUNDER_HALF_TURN / 180.0f));
        rotation[2] = dds3GetRandFloatA(effDefaultRandomState) * (75.0f * (EFF_THUNDER_HALF_TURN / 180.0f));
        SetRotateMatrixXYZ((const struct RwV3d *)rotation);
        SDF_VU0_LOADVECTOR(axis);
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        SDF_VU0_STOREVECTOR_NOMEM(axis);
        SetRotateMatrixAxis(((dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f * 0.7f) * rotationScale + rotationScale, (const struct RwV3d *)axis);
        SDF_VU0_LOADVECTOR(placement);
        SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        SDF_VU0_STOREVECTOR_NOMEM(placement);
        SDF_VU0_SUBVECTOR_EXTENDED();
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10_EXTENDED();
        SDF_VU0_STOREVECTOR_NOMEM(side);
        SDF_VU0_LOADVECTOR_B(width);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 2);
        SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 1);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 3);
        SDF_VU0_LOADVECTOR(side);
        SDF_VU0_LOADVECTOR_B(outerWidth);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 4);
        vertices += 5;
    }
}


/* Rotate the live placement direction and every five-vector history row
 * around the effect origin. The separate placement height is preserved.
 */
void effPCPThunderSphereRotateAxis(EffThunderVectorWork *work, s32 index) {
    f32 axis[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    ParSystem *system = work->cellSystem;
    EffThunderVectorCell *source = &work->cells[index];
    ParCell *cell = &system->cells[index];
    u128 *vertices = cell->history;
    s32 rows = system->vertexWordCount / 5;
    s32 i;
    f32 sample;
    f32 angle;

    sample = dds3GetRandFloatA(effDefaultRandomState);
    angle = source->rotationScale * sample * 3.0f;
    direction[0] = source->placementVector[0];
    direction[1] = 0.0f;
    direction[2] = source->placementVector[2];
    axis[0] = source->rotationAxis[0];
    axis[1] = source->rotationAxis[1];
    axis[2] = source->rotationAxis[2];
    SetRotateMatrixAxis(angle, (const struct RwV3d *)axis);
    SDF_VU0_LOADVECTOR(direction);
    VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
    SDF_VU0_STOREVECTOR_NOMEM(direction);
    source->placementVector[0] = direction[0];
    source->placementVector[2] = direction[2];
    SDF_VU0_LOADVECTOR_B(work->head.vector.origin);
    for (i = 0; i < rows; i++) {
        SDF_VU0_LOADVECTOR(vertices);
        SDF_VU0_SUBVECTOR_EXTENDED();
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf11);
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        SDF_VU0_LOADVECTOR(vertices + 1);
        SDF_VU0_SUBVECTOR_EXTENDED();
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf11);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 1);
        SDF_VU0_LOADVECTOR(vertices + 2);
        SDF_VU0_SUBVECTOR_EXTENDED();
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf11);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 2);
        SDF_VU0_LOADVECTOR(vertices + 3);
        SDF_VU0_SUBVECTOR_EXTENDED();
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf11);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 3);
        SDF_VU0_LOADVECTOR(vertices + 4);
        SDF_VU0_SUBVECTOR_EXTENDED();
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf11);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 4);
        vertices += 5;
    }
}


extern void effPCPThunderSphereRotate(EffThunderVectorWork *work, s32 index);
extern void effPCPThunderSphereRotateAxis(EffThunderVectorWork *work, s32 index);

/* Delay -> active geometry -> alpha fade -> restart; tint each render cell.
   The unsigned wrap-add is not a saturating fade. Submit even for signed count <= 0. */
void effPCPThunderSpherePolyDraw(EffThunderVectorWork *work) {
    s32 i = 0;
    ParSystem *renderSystem = work->cellSystem;
    s32 cellCount = work->head.vector.cellCount;
    u32 tintColor = work->tintColor;
    EffThunderVectorCell *cell = work->cells;
    ParCell *renderCells = renderSystem->cells;
    u32 *renderColor;

    if (cellCount > 0) {
        renderColor = &renderCells->color;
        do {
            if (cell->delayFrames == 0) {
                if (cell->activeFrames != 0) {
                    effPCPThunderSphereRotate(work, i);
                    effPCPThunderSphereRotateAxis(work, i);
                    cell->activeFrames--;
                } else if (cell->color & EFF_THUNDER_ALPHA_MASK) {
                    cell->color += EFF_THUNDER_ALPHA_WRAP_ADD;
                    effPCPThunderSphereRotateAxis(work, i);
                } else {
                    effPCPThunderSphereInit(work, i);
                    dds3TrackPolyListInit(work->cellSystem, i);
                }
            } else {
                cell->delayFrames--;
            }
            *renderColor = effMagaCylinderCalcColor(cell->color, tintColor);
            i++;
            cell++;
            renderColor = (u32 *)((u8 *)renderColor + sizeof(ParCell));
        } while (i < cellCount);
    }
    dds3TrackPolyDraw(work->cellSystem);
}

/* Allocate one work followed by its vector cells and retain unscaled dimensions.
   Only cell countdowns/color are initialized; geometry is sampled on the first restart. */
EffThunderVectorWork *effBTLBindCreateTrack(EffThunderVectorParams *parameters) {
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(parameters->cellCount * sizeof(EffThunderVectorCell) + sizeof(EffThunderVectorWork));
    EffThunderVectorWork *work = (EffThunderVectorWork *)sdfMemLock(allocationHandle);
    u32 cellIndex;

    work->head.vector = *parameters;
    work->cells = (EffThunderVectorCell *)(work + 1);
    work->baseRadiusScale = parameters->radiusScale;
    work->baseHeightScale = parameters->heightScale;
    work->allocationHandle = allocationHandle;
    work->cellSystem = dds3TrackPolyCreate(work->head.vector.cellCount, work->head.vector.perCell, 0, PAR_CELL_TOPOLOGY_PAIR);
    dds3TrackPolyVCSet(work->cellSystem, 2, work->head.vector.dispatchArg, work->head.vector.dispatchArg);
    dds3TrackPolySetMode(work->cellSystem, work->head.vector.systemParam);
    for (cellIndex = 0; cellIndex < work->head.vector.cellCount; cellIndex++) {
        work->cells[cellIndex].delayFrames = 0;
        work->cells[cellIndex].activeFrames = 0;
        work->cells[cellIndex].color = 0;
    }
    work->tintColor = EFF_THUNDER_NEUTRAL_COLOR;
    return work;
}

/* Tear down the indexed vector variant: system first, containing allocation last. */
void effPCPThunderSpherePolyLowDelete(EffThunderVectorWork *work) {
    dds3TrackPolyDelete(work->cellSystem);
    sdfMemFreeDirect(work->allocationHandle);
}

/* Create indexed vector work from the first packed parameter block; ignore its result. */
EffThunderVectorWork *effPCPThunderSpherePolyLowCreate(void *parameterTable) {
    void *parameters;

    parameters = dds3EffectPCPDataGetData(parameterTable, EFF_THUNDER_PARAMETER_BLOCK);
    return effBTLBindCreateTrack(parameters);
}

/* Direct parameter-pointer entry for indexed vector creation; ignore its result. */
EffThunderVectorWork *effThunderSpawnIndexedVector(void *parameters) {
    return effBTLBindCreateTrack(parameters);
}

/* vu0 routine: copy the entire quadword, including its fourth component. */
void effPCPThunderSpherePolyLowSetPosition(void *destination, void *source) {
    SDF_VU0_COPYVECTOR(destination, source);
}

/* Set the shared packed modulation color, not an individual cell's sampled color. */
void effPCPThunderSpherePolyLowSetColor(EffThunderVectorWork *work, u32 tintColor) {
    work->tintColor = tintColor;
}

/* Absolute radius/height scaling for the indexed variant; repeated calls do not compound. */
void effPCPThunderSpherePolyLowSetScale(f32 factor, EffThunderVectorWork *work) {
    work->head.vector.radiusScale = work->baseRadiusScale * factor;
    work->head.vector.heightScale = work->baseHeightScale * factor;
}

/* Preserve the caller's word unchanged; its wider callback role is unknown. */
u32 effPCPThunderSpherePolyLowGetParam(u32 value) {
    return value;
}

/* Indexed-variant resampling follows the same native RNG order as the first variant.
   X/Z are normalized before Y is assigned; the fourth scratch lane is not initialized. */
void effPCPThunderSphereLowInit(EffThunderVectorWork *work, s32 index) {
    EffThunderVectorCell *cell = work->cells + index;
    f32 scratchVector[EFF_THUNDER_VECTOR_COMPONENTS];

    cell->delayFrames = dds3GetRandA((EffRandState *)effDefaultRandomState) % work->head.vector.startDelayRange;
    cell->activeFrames = dds3GetRandA((EffRandState *)effDefaultRandomState) % work->head.vector.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
    scratchVector[0] = (dds3GetRandFloatA(effDefaultRandomState) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN;
    scratchVector[1] = 0;
    scratchVector[2] = (dds3GetRandFloatA(effDefaultRandomState) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN;
    SDF_VU0_LOADVECTOR(scratchVector);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(scratchVector);
    scratchVector[1] = work->head.vector.heightScale * EFF_THUNDER_HALF_SCALE * ((dds3GetRandFloatA(effDefaultRandomState) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN);
    cell->placementVector[0] = scratchVector[0];
    cell->placementVector[1] = scratchVector[1];
    cell->placementVector[2] = scratchVector[2];
    scratchVector[0] = (dds3GetRandFloatA(effDefaultRandomState) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_AXIS_JITTER;
    scratchVector[1] = 1.0f;
    scratchVector[2] = (dds3GetRandFloatA(effDefaultRandomState) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_AXIS_JITTER;
    SDF_VU0_LOADVECTOR(scratchVector);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(scratchVector);
    cell->rotationAxis[0] = scratchVector[0];
    cell->rotationAxis[1] = scratchVector[1];
    cell->rotationAxis[2] = scratchVector[2];
    cell->rotationScale = work->head.vector.rotationScale * ((dds3GetRandFloatA(effDefaultRandomState) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_ROTATION_JITTER + 1.0f);
    cell->radius = work->head.vector.radiusScale * EFF_THUNDER_HALF_SCALE * ((dds3GetRandFloatA(effDefaultRandomState) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_RADIUS_JITTER + 1.0f);
    cell->color = EFF_THUNDER_NEUTRAL_COLOR;
}

/* Generate the indexed variant as two-vector rows in its kind-0 history.
 * It samples wider axis perturbations and refreshes the cell angle per row.
 */
void effPCPThunderSphereLowRotate(EffThunderVectorWork *work, s32 index) {
    f32 position[4] __attribute__((aligned(16)));
    f32 placement[4] __attribute__((aligned(16)));
    f32 axis[4] __attribute__((aligned(16)));
    f32 originalAxis[4] __attribute__((aligned(16)));
    f32 cameraDirection[4] __attribute__((aligned(16)));
    f32 origin[4] __attribute__((aligned(16)));
    f32 width[4] __attribute__((aligned(16)));
    f32 rotation[4] __attribute__((aligned(16)));
    ParSystem *system = work->cellSystem;
    ParCell *cell = &system->cells[index];
    EffThunderVectorCell *source;
    u128 *vertices;
    s32 count;
    s32 i;
    f32 widthScale;
    f32 rotationScale;
    f32 angleJitter;
    f32 radius;

    count = system->vertexWordCount >> 1;
    cell->vertexCount = system->vertexWordCount;
    source = &work->cells[index];
    vertices = cell->history;
    widthScale = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f * 0.3f + 1.0f;
    VEC3_SPLAT(width, work->head.vector.bandWidth * widthScale);
    originalAxis[0] = source->rotationAxis[0];
    originalAxis[1] = source->rotationAxis[1];
    originalAxis[2] = source->rotationAxis[2];
    originalAxis[3] = 0.0f;
    radius = source->radius;
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(sdfViewTargetVector);
    SDF_VU0_SUBVECTOR_EXTENDED();
    SDF_VU0_STOREVECTOR_NOMEM(cameraDirection);
    placement[0] = source->placementVector[0];
    placement[1] = 0.0f;
    placement[2] = source->placementVector[2];
    SDF_VU0_COPYVECTOR(origin, work->head.vector.origin);
    origin[1] += source->placementVector[1];

    for (i = 0; i < count; i++) {
        position[0] = origin[0] + placement[0] * radius;
        position[1] = origin[1] + placement[1] * radius;
        position[2] = origin[2] + placement[2] * radius;
        SDF_VU0_COPYVECTOR(axis, originalAxis);
        rotation[0] = dds3GetRandFloatA(effDefaultRandomState) * (EFF_THUNDER_HALF_TURN * 0.5f);
        rotation[1] = dds3GetRandFloatA(effDefaultRandomState) * (EFF_THUNDER_HALF_TURN * 0.5f);
        rotation[2] = dds3GetRandFloatA(effDefaultRandomState) * (EFF_THUNDER_HALF_TURN * 0.5f);
        SetRotateMatrixXYZ((const struct RwV3d *)rotation);
        SDF_VU0_LOADVECTOR(axis);
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        SDF_VU0_STOREVECTOR_NOMEM(axis);
        angleJitter = dds3GetRandFloatA(effDefaultRandomState);
        rotationScale = -source->rotationScale;
        SetRotateMatrixAxis(((angleJitter - 0.5f) * 2.0f * 0.8f) * rotationScale + rotationScale, (const struct RwV3d *)axis);
        SDF_VU0_LOADVECTOR(placement);
        SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        SDF_VU0_STOREVECTOR_NOMEM(placement);
        SDF_VU0_SUBVECTOR_EXTENDED();
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10_EXTENDED();
        SDF_VU0_LOADVECTOR_B(width);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 1);
        vertices += 2;
    }
}


/* Rotate the live placement direction and every two-vector history row
 * around the effect origin. The separate placement height is preserved.
 */
void effPCPThunderSphereLowRotateAxis(EffThunderVectorWork *work, s32 index) {
    f32 axis[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    ParSystem *system = work->cellSystem;
    EffThunderVectorCell *source = &work->cells[index];
    ParCell *cell = &system->cells[index];
    u128 *vertices = cell->history;
    s32 rows = system->vertexWordCount >> 1;
    s32 i;
    f32 sample;
    f32 angle;

    sample = dds3GetRandFloatA(effDefaultRandomState);
    angle = source->rotationScale * sample * 3.0f;
    direction[0] = source->placementVector[0];
    direction[1] = 0.0f;
    direction[2] = source->placementVector[2];
    axis[0] = source->rotationAxis[0];
    axis[1] = source->rotationAxis[1];
    axis[2] = source->rotationAxis[2];
    SetRotateMatrixAxis(angle, (const struct RwV3d *)axis);
    SDF_VU0_LOADVECTOR(direction);
    VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
    SDF_VU0_STOREVECTOR_NOMEM(direction);
    source->placementVector[0] = direction[0];
    source->placementVector[2] = direction[2];
    SDF_VU0_LOADVECTOR_B(work->head.vector.origin);
    for (i = 0; i < rows; i++) {
        SDF_VU0_LOADVECTOR(vertices);
        SDF_VU0_SUBVECTOR_EXTENDED();
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf11);
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        SDF_VU0_LOADVECTOR(vertices + 1);
        SDF_VU0_SUBVECTOR_EXTENDED();
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        VU0_ADD_EXTENDED(vf10, vf10, vf11);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 1);
        vertices += 2;
    }
}


extern void effPCPThunderSphereLowRotate(EffThunderVectorWork *work, s32 index);
extern void effPCPThunderSphereLowRotateAxis(EffThunderVectorWork *work, s32 index);

/* Indexed vector countdown/fade/restart with shared tint and native render-cell stride.
   Fade is unsigned wrap-add; submission is unconditional after the signed-count loop. */
void effPCPThunderSpherePolyLowDraw(EffThunderVectorWork *work) {
    s32 i = 0;
    ParSystem *renderSystem = work->cellSystem;
    s32 cellCount = work->head.vector.cellCount;
    u32 tintColor = work->tintColor;
    EffThunderVectorCell *cell = work->cells;
    ParCell *renderCells = renderSystem->cells;
    u32 *renderColor;

    if (cellCount > 0) {
        renderColor = &renderCells->color;
        do {
            if (cell->delayFrames == 0) {
                if (cell->activeFrames != 0) {
                    effPCPThunderSphereLowRotate(work, i);
                    effPCPThunderSphereLowRotateAxis(work, i);
                    cell->activeFrames--;
                } else if (cell->color & EFF_THUNDER_ALPHA_MASK) {
                    cell->color += EFF_THUNDER_ALPHA_WRAP_ADD;
                    effPCPThunderSphereLowRotateAxis(work, i);
                } else {
                    effPCPThunderSphereLowInit(work, i);
                    dds3TrackPolyListInit(work->cellSystem, i);
                }
            } else {
                cell->delayFrames--;
            }
            *renderColor = effMagaCylinderCalcColor(cell->color, tintColor);
            i++;
            cell++;
            renderColor = (u32 *)((u8 *)renderColor + sizeof(ParCell));
        } while (i < cellCount);
    }
    dds3TrackPolyDraw(work->cellSystem);
}

extern void effThunderSparkInit(EffThunderSparkWork *work, s32 index);

/* Allocate spark state after the work and a separate one-cell system per spark.
   Clamp delay spread only in the copied head; sample motion before initial age. */
EffThunderSparkWork *effThunderSparkCreate(EffThunderSparkParams *parameters) {
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(parameters->sparkCount * sizeof(EffThunderSpark) + sizeof(EffThunderSparkWork));
    EffThunderSparkWork *work = (EffThunderSparkWork *)sdfMemLock(allocationHandle);
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
        work->sparks[sparkIndex].system = dds3TrackPolyCreate(EFF_THUNDER_SINGLE_CELL, work->head.halfLife * 2 - 1, 0, PAR_CELL_TOPOLOGY_PAIR);
        dds3TrackPolyVCSet(work->sparks[sparkIndex].system, 2, work->head.dispatchArg, work->head.dispatchArg);
        dds3TrackPolySetMode(work->sparks[sparkIndex].system, work->head.systemParam);
        effThunderSparkInit(work, sparkIndex);
        work->sparks[sparkIndex].age = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % delaySpread);
    }
    return work;
}

/* Release every spark's system before freeing the owner, using the native signed count. */
void effThunderReleaseSparkWork(EffThunderSparkWork *work) {
    s32 sparkCount = (s32)work->head.sparkCount;
    s32 sparkIndex = 0;

    if (sparkCount > 0) {
        do {
            dds3TrackPolyDelete(work->sparks[sparkIndex].system);
            sparkIndex++;
        } while (sparkIndex < sparkCount);
    }
    sdfMemFreeDirect(work->allocationHandle);
}

/* vu0 routine: copy one whole quadword, including its fourth component. */
void effThunderCopySparkVector(void *destination, void *source) {
    SDF_VU0_COPYVECTOR(destination, source);
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
    spark->verticalSpeed = work->head.verticalSpeed * (dds3GetRandFloatA(effDefaultRandomState) * randomness + (1.0f - randomness));
    spark->heightOffset = work->head.heightOffsetRange * dds3GetRandFloatA(effDefaultRandomState);
    spark->orbitAngle = dds3GetRandFloatA(effDefaultRandomState) * (EFF_THUNDER_HALF_TURN * 2.0f);
    randomness = work->head.angularSpeedRandomness;
    spark->angularSpeed = work->head.angularSpeed * (dds3GetRandFloatA(effDefaultRandomState) * randomness + (1.0f - randomness));
    randomness = work->head.radiusRandomness;
    spark->orbitRadius = work->head.orbitRadius * (dds3GetRandFloatA(effDefaultRandomState) * randomness + (1.0f - randomness));
}

/* Build one spark's kind-0 strip between its lowered and current positions.
 * Every iteration emits two two-vector rows into that spark's own cell.
 */
void effThunderBuildSparkStripCell(EffThunderSparkWork *work, s32 index) {
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
    ParSystem *system;
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
    system = work->sparks[index].system;
    cell = system->cells;
    cell->vertexCount = system->vertexWordCount;
    vertices = cell->history;
    segments = work->head.halfLife;
    SDF_VU0_LOADVECTOR(work->head.position);
    SDF_VU0_LOADVECTOR_B(work->head.loweredPosition);
    SDF_VU0_SUBVECTOR_EXTENDED();
    SDF_VU0_GETLENGTH(distance);
    VU0_NORMALIZE_VF10_EXTENDED();
    SDF_VU0_STOREVECTOR_NOMEM(direction);
    distance /= work->head.halfLife;
    amplitude = work->head.waveAmplitude;
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(sdfViewTargetVector);
    SDF_VU0_SUBVECTOR_EXTENDED();
    SDF_VU0_STOREVECTOR_NOMEM(cameraDirection);
    SDF_VU0_LOADVECTOR_B(direction);
    VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10_EXTENDED();
    SDF_VU0_STOREVECTOR_NOMEM(side);
    phase = dds3GetRandFloatA(effDefaultRandomState) * (EFF_THUNDER_HALF_TURN * 2.0f);
    cycle = phase - (EFF_THUNDER_HALF_TURN * 2.0f);
    SDF_VU0_COPYVECTOR(position, work->head.loweredPosition);
    currentWave = amplitude * sdfSin(phase);
    VEC3_SPLAT(waveScale, currentWave);
    SDF_VU0_LOADVECTOR(side);
    SDF_VU0_LOADVECTOR_B(waveScale);
    VU0_MUL_EXTENDED(vf10, vf10, vf11);
    SDF_VU0_LOADVECTOR_B(position);
    SDF_VU0_ADDVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B_NOMEM(position);
    VEC3_SPLAT(stepScale, distance);
    previousWave = currentWave;
    currentWave = amplitude * sdfSin(phase);
    VEC3_SPLAT(waveScale, currentWave - previousWave);
    SDF_VU0_LOADVECTOR(direction);
    SDF_VU0_LOADVECTOR_B(stepScale);
    VU0_MUL_EXTENDED(vf10, vf10, vf11);
    VU0_MOVE_VF_EXTENDED(vf12, vf10);
    SDF_VU0_LOADVECTOR(side);
    SDF_VU0_LOADVECTOR_B(waveScale);
    VU0_MUL_EXTENDED(vf10, vf10, vf11);
    VU0_ADD_EXTENDED(vf10, vf10, vf12);
    VU0_MOVE_VF_EXTENDED(vf12, vf10);
    SDF_VU0_LOADVECTOR_B(position);
    SDF_VU0_ADDVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B_NOMEM(next);
    VU0_MOVE_VF_EXTENDED(vf10, vf12);
    VU0_NORMALIZE_VF10_EXTENDED();
    SDF_VU0_STOREVECTOR_NOMEM(basis);

    for (i = 0; i < segments; i++) {
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10_EXTENDED();
        SDF_VU0_STOREVECTOR_NOMEM(basis);
        SDF_VU0_LOADVECTOR_B(width);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 1);
        vertices += 2;
        SDF_VU0_COPYVECTOR(previousSide, basis);
        SDF_VU0_COPYVECTOR(position, next);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(width);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 1);
        vertices += 2;
        phaseStep = (EFF_THUNDER_HALF_TURN / segments) * 4.0f;
        cycle += phaseStep;
        phase += phaseStep;
        if (cycle > EFF_THUNDER_HALF_TURN) {
            cycle -= EFF_THUNDER_HALF_TURN;
            amplitude = work->head.waveAmplitude * (1.0f - dds3GetRandFloatA(effDefaultRandomState) * 0.95f);
        }
        previousWave = currentWave;
        currentWave = amplitude * sdfSin(phase);
        VEC3_SPLAT(waveScale, currentWave - previousWave);
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_LOADVECTOR_B(stepScale);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        SDF_VU0_LOADVECTOR(side);
        SDF_VU0_LOADVECTOR_B(waveScale);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        SDF_VU0_STOREVECTOR_NOMEM(delta);
        SetRotateMatrixAxis(((dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f) * (20.0f * (EFF_THUNDER_HALF_TURN / 180.0f)), (const struct RwV3d *)cameraDirection);
        SDF_VU0_LOADVECTOR(delta);
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        SDF_VU0_LOADVECTOR_B(position);
        SDF_VU0_ADDVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(next);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_NORMALIZE_VF10_EXTENDED();
        SDF_VU0_STOREVECTOR_NOMEM(basis);
        SDF_VU0_LOADVECTOR(next);
        SDF_VU0_LOADVECTOR_B(position);
        SDF_VU0_SUBVECTOR_EXTENDED();
        VU0_NORMALIZE_VF10_EXTENDED();
        SDF_VU0_LOADVECTOR_B(previousSide);
        SDF_VU0_INNERPRODUCT(facing);
        /* Choose the join edge from the new direction relative to the prior side. */
        if (facing < 0.0f) {
            join = vertices - 1;
            SDF_VU0_LOADVECTOR_C(width);
        } else {
            join = vertices - 2;
            SDF_VU0_LOADVECTOR_C(negativeWidth);
        }
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10_EXTENDED();
        VU0_MOVE_VF_EXTENDED(vf11, vf12);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        SDF_VU0_LOADVECTOR_B(join);
        VU0_ADD_EXTENDED(vf10, vf10, vf11);
        SDF_VU0_STOREVECTOR_NOMEM(position);
    }
}



/* Advance each spark through delayed initialization, motion and alpha fades.
 * Snapshot the entry parameters and age; initialization and geometry callbacks
 * can update the work while the frame's timing decision remains unchanged.
 */
void effThunderUpdateOrbitingSparks(EffThunderSparkWork *work) {
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
    SDF_VU0_COPYVECTOR(origin, work->head.origin);
    count = work->head.sparkCount;
    spark = work->sparks;
    for (i = 0; i < count; i++, spark++) {
        age = spark->age;
        if (age == 0) {
            effThunderSparkInit(work, i);
            dds3TrackPolyListInit(spark->system, 0);
        } else if (age > 0 && age <= duration) {
            angle = spark->orbitAngle;
            radius = spark->orbitRadius;
            position[0] = origin[0] + sdfCos(angle) * radius;
            position[1] = origin[1] + spark->verticalOffset - spark->heightOffset;
            position[2] = origin[2] + sdfSin(angle) * radius;
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
            color = effMagaCylinderCalcColor(((u32)(alpha * 127.0f) << 24) | 0x808080, tintColor);
            renderCell = spark->system->cells;
            lowerHeight = position[1] - heightOffset;
            work->head.loweredPosition[0] = position[0];
            work->head.loweredPosition[1] = lowerHeight;
            work->head.loweredPosition[2] = position[2];
            work->head.position[0] = position[0];
            work->head.position[1] = position[1];
            work->head.position[2] = position[2];
            effThunderBuildSparkStripCell(work, i);
            renderCell->color = color;
            dds3TrackPolyDraw(spark->system);
        }
        if (age >= duration && loop) {
            spark->age = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % delaySpread);
        } else {
            spark->age++;
        }
    }
}


/* Single- and dual-system variants share this allocation layout, but the
   single-system update counts frames where the dual variant keeps a system. */
struct EffThunderFragmentWork {
    EffThunderFragmentParams head;
    EffThunderFrag *fragments; /* 0x54 */
    u32 color;               /* 0x58 */
    union {
        u32 updateCount;     /* single-system variant */
        ParSystem *secondarySystem; /* dual-system variant */
    } state;                 /* 0x5C */
    ParSystem *system;            /* 0x60 */
    sdfMemHandle_t *allocationHandle; /* 0x64 */
}; /* 0x68 */

extern void effPCP_ThunderPolyDrawOff(EffThunderFragmentWork *work, s32 index);

/* Create the single-system fragment work and seed every fragment's timing/color.
   The native history-count expression and unchecked ranges are retained. */
EffThunderFragmentWork *effPCP_ThunderPolyCreate(EffThunderFragmentParams *parameters) {
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(parameters->fragmentCount * sizeof(EffThunderFrag) + sizeof(EffThunderFragmentWork));
    EffThunderFragmentWork *work = (EffThunderFragmentWork *)sdfMemLock(allocationHandle);
    u32 i;

    work->head = *parameters;
    work->fragments = (EffThunderFrag *)(work + 1);
    work->allocationHandle = allocationHandle;
    work->system = dds3TrackPolyCreate(work->head.fragmentCount, work->head.halfLife * 2 - 1, 0, PAR_CELL_TOPOLOGY_FIVE_VECTOR);
    dds3TrackPolyVC5Set_AcuteW(work->system, work->head.centerColorWord, work->head.middleColorWord, work->head.edgeColorWord);
    dds3TrackPolySetMode(work->system, work->head.systemParam);
    for (i = 0; i < work->head.fragmentCount; i++) {
        effPCP_ThunderPolyDrawOff(work, i);
    }
    work->state.updateCount = 0;
    work->color = EFF_THUNDER_NEUTRAL_COLOR;
    return work;
}

/* Release the single fragment system before its containing work allocation. */
void effPCP_ThunderPolyDelete(EffThunderFragmentWork *work) {
    dds3TrackPolyDelete(work->system);
    sdfMemFreeDirect(work->allocationHandle);
}

/* vu0 routine: replace the second endpoint and translate the first by its delta.
   The native store/load order and all four components, including W, are retained. */
void effPCP_ThunderPolySetPosition(u8 *endpoints, void *anchor) {
        SDF_VU0_LOADVECTOR(endpoints + EFF_THUNDER_ENDPOINT_VECTOR_BYTES);
        SDF_VU0_LOADVECTOR_B(anchor);
        SDF_VU0_STOREVECTOR_B(endpoints + EFF_THUNDER_ENDPOINT_VECTOR_BYTES);
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_LOADVECTOR(endpoints);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(endpoints);
}

/* Set the fragment work's shared packed color word. */
void effPCP_ThunderPolySetColor(EffThunderFragmentWork *work, u32 color) {
    work->color = color;
}

/* Borrow the leading parameters without transferring work ownership. */
EffThunderFragmentParams *effPCP_ThunderPolyGetParam(EffThunderFragmentWork *work) {
    return &work->head;
}

/* Apply decreasing alpha along each fragment cell using the retained color words. */
void effPCP_ThunderPolySetVCAcute(EffThunderFragmentWork *work) {
    dds3TrackPolyVC5Set_Acute(work->system, work->head.centerColorWord, work->head.middleColorWord, work->head.edgeColorWord);
}

/* Apply increasing alpha along each fragment cell using the retained color words. */
void effPCP_ThunderPolySetVCAcute2(EffThunderFragmentWork *work) {
    dds3TrackPolyVC5Set_AcuteR(work->system, work->head.centerColorWord, work->head.middleColorWord, work->head.edgeColorWord);
}

/* Apply symmetric cell-color bands using the fragment configuration's native arguments. */
void effThunderApplyFragmentColorBands(EffThunderFragmentWork *work) {
    dds3TrackPolyVC5Set_Normal(work->system, work->head.centerColorWord, work->head.middleColorWord, work->head.edgeColorWord);
}


/* Resample delay/active countdowns and restore the fixed fragment grey.
   Moduli are unchecked; retain the native address-of-array RNG argument. */
void effPCP_ThunderPolyDrawOff(EffThunderFragmentWork *work, s32 index) {
    EffThunderFrag *fragment = &work->fragments[index];

    fragment->delayFrames = dds3GetRandA((EffRandState *)effDefaultRandomState) % work->head.startDelayRange;
    fragment->activeFrames = dds3GetRandA((EffRandState *)effDefaultRandomState) % work->head.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
    fragment->color = EFF_THUNDER_FRAGMENT_GREY;
}

/* Build the first strip from the work's own start point. Each step emits
 * two five-vector rows; later chain segments reuse the final row as a seed.
 */
void effPCP_ThunderPolyPartRefresh(EffThunderFragmentWork *work, s32 index) {
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
    ParSystem *system = work->system;
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
    cell->vertexCount = system->vertexWordCount;
    vertices = cell->history;
    segments = work->head.halfLife;
    SDF_VU0_LOADVECTOR(work->head.end);
    SDF_VU0_LOADVECTOR_B(work->head.start);
    SDF_VU0_SUBVECTOR_EXTENDED();
    SDF_VU0_GETLENGTH(distance);
    VU0_NORMALIZE_VF10_EXTENDED();
    SDF_VU0_STOREVECTOR_NOMEM(direction);
    distance /= work->head.halfLife;
    amplitude = work->head.waveAmplitude;
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(sdfViewTargetVector);
    SDF_VU0_SUBVECTOR_EXTENDED();
    SDF_VU0_STOREVECTOR_NOMEM(cameraDirection);
    SDF_VU0_LOADVECTOR_B(direction);
    VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10_EXTENDED();
    SDF_VU0_STOREVECTOR_NOMEM(side);
    phase = dds3GetRandFloatA(effDefaultRandomState) * (EFF_THUNDER_HALF_TURN * 2.0f);
    cycle = phase - (EFF_THUNDER_HALF_TURN * 2.0f);
    SDF_VU0_COPYVECTOR(position, work->head.start);
    currentWave = amplitude * sdfSin(phase);
    VEC3_SPLAT(waveScale, currentWave);
    SDF_VU0_LOADVECTOR(side);
    SDF_VU0_LOADVECTOR_B(waveScale);
    VU0_MUL_EXTENDED(vf10, vf10, vf11);
    SDF_VU0_LOADVECTOR_B(position);
    SDF_VU0_ADDVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B_NOMEM(position);
    VEC3_SPLAT(stepScale, distance);
    previousWave = currentWave;
    currentWave = amplitude * sdfSin(phase);
    VEC3_SPLAT(waveScale, currentWave - previousWave);
    SDF_VU0_LOADVECTOR(direction);
    SDF_VU0_LOADVECTOR_B(stepScale);
    VU0_MUL_EXTENDED(vf10, vf10, vf11);
    VU0_MOVE_VF_EXTENDED(vf12, vf10);
    SDF_VU0_LOADVECTOR(side);
    SDF_VU0_LOADVECTOR_B(waveScale);
    VU0_MUL_EXTENDED(vf10, vf10, vf11);
    VU0_ADD_EXTENDED(vf10, vf10, vf12);
    VU0_MOVE_VF_EXTENDED(vf12, vf10);
    SDF_VU0_LOADVECTOR_B(position);
    SDF_VU0_ADDVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B_NOMEM(next);
    VU0_MOVE_VF_EXTENDED(vf10, vf12);
    VU0_NORMALIZE_VF10_EXTENDED();
    SDF_VU0_STOREVECTOR_NOMEM(basis);

    for (i = 0; i < segments; i++) {
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10_EXTENDED();
        SDF_VU0_STOREVECTOR_NOMEM(basis);
        SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 2);
        VU0_MOVE_VF_EXTENDED(vf10, vf11);
        SDF_VU0_LOADVECTOR_B(width);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 1);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 3);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(outerWidth);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 4);
        vertices += 5;
        SDF_VU0_COPYVECTOR(previousSide, basis);
        SDF_VU0_COPYVECTOR(position, next);
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 2);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(width);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 1);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 3);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(outerWidth);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 4);
        vertices += 5;
        phaseStep = (EFF_THUNDER_HALF_TURN / segments) * 4.0f;
        cycle += phaseStep;
        phase += phaseStep;
        if (cycle > EFF_THUNDER_HALF_TURN) {
            cycle -= EFF_THUNDER_HALF_TURN;
            amplitude = work->head.waveAmplitude * (1.0f - dds3GetRandFloatA(effDefaultRandomState) * 0.95f);
        }
        previousWave = currentWave;
        currentWave = amplitude * sdfSin(phase);
        VEC3_SPLAT(waveScale, currentWave - previousWave);
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_LOADVECTOR_B(stepScale);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        SDF_VU0_LOADVECTOR(side);
        SDF_VU0_LOADVECTOR_B(waveScale);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        SDF_VU0_STOREVECTOR_NOMEM(delta);
        SetRotateMatrixAxis(((dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f) * (20.0f * (EFF_THUNDER_HALF_TURN / 180.0f)), (const struct RwV3d *)cameraDirection);
        SDF_VU0_LOADVECTOR(delta);
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        SDF_VU0_LOADVECTOR_B(position);
        SDF_VU0_ADDVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(next);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_NORMALIZE_VF10_EXTENDED();
        SDF_VU0_STOREVECTOR_NOMEM(basis);
        SDF_VU0_LOADVECTOR(next);
        SDF_VU0_LOADVECTOR_B(position);
        SDF_VU0_SUBVECTOR_EXTENDED();
        VU0_NORMALIZE_VF10_EXTENDED();
        SDF_VU0_LOADVECTOR_B(previousSide);
        SDF_VU0_INNERPRODUCT(facing);
        /* Choose the join edge from the new direction relative to the prior side. */
        if (facing < 0.0f) {
            join = vertices - 1;
            SDF_VU0_LOADVECTOR_C(outerWidth);
        } else {
            join = vertices - 5;
            SDF_VU0_LOADVECTOR_C(negativeOuterWidth);
        }
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10_EXTENDED();
        VU0_MOVE_VF_EXTENDED(vf11, vf12);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        SDF_VU0_LOADVECTOR_B(join);
        VU0_ADD_EXTENDED(vf10, vf10, vf11);
        SDF_VU0_STOREVECTOR_NOMEM(position);
    }
}


/* Update each fragment through delay, active geometry and alpha fade. The
 * optional frame limit stops new restarts; active geometry and fades continue.
 */
void effPCP_ThunderPolyDraw(EffThunderFragmentWork *work) {
    s32 i = 0;
    ParSystem *system = work->system;
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
                    effPCP_ThunderPolyPartRefresh(work, i);
                    fragment->activeFrames--;
                } else if (fragment->color & EFF_THUNDER_ALPHA_MASK) {
                    fragment->color += EFF_THUNDER_ALPHA_WRAP_ADD;
                } else if (restartLimit == 0 || canRestart) {
                    effPCP_ThunderPolyDrawOff(work, i);
                    dds3TrackPolyListInit(work->system, i);
                } else {
                    renderCells[i].vertexCount = 0;
                }
            } else {
                fragment->delayFrames--;
            }
            renderCells[i].color = effMagaCylinderCalcColor(fragment->color, tintColor);
            i++;
            fragment++;
        } while (i < count);
        updateCount = work->state.updateCount;
    }
    work->state.updateCount = updateCount + 1;
    dds3TrackPolyDraw(work->system);
}



extern void effPCP_ThunderPolyLowDrawOff(EffThunderFragmentWork *work, s32 index);

/* Allocate dual-system fragment work, creating the secondary system before the primary.
   Both systems share one fragment-state array; keep native dispatch/allocation order. */
EffThunderFragmentWork *effPCP_ThunderPolyLowCreate(EffThunderFragmentParams *parameters) {
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(parameters->fragmentCount * sizeof(EffThunderFrag) + sizeof(EffThunderFragmentWork));
    EffThunderFragmentWork *work = (EffThunderFragmentWork *)sdfMemLock(allocationHandle);
    u32 i;

    work->head = *parameters;
    work->fragments = (EffThunderFrag *)(work + 1);
    work->allocationHandle = allocationHandle;
    work->state.secondarySystem = dds3TrackPolyCreate(work->head.fragmentCount, work->head.halfLife * 2 - 1, 0, PAR_CELL_TOPOLOGY_TRIANGLE);
    dds3TrackPolyVCSet(work->state.secondarySystem, 2, work->head.middleColorWord, work->head.edgeColorWord);
    dds3TrackPolySetMode(work->state.secondarySystem, work->head.systemParam);
    work->system = dds3TrackPolyCreate(work->head.fragmentCount, work->head.halfLife * 2 - 1, 0, PAR_CELL_TOPOLOGY_PAIR);
    dds3TrackPolyVCSet(work->system, 2, work->head.centerColorWord, work->head.centerColorWord);
    dds3TrackPolySetMode(work->system, work->head.systemParam);
    for (i = 0; i < work->head.fragmentCount; i++) {
        effPCP_ThunderPolyLowDrawOff(work, i);
    }
    work->color = EFF_THUNDER_NEUTRAL_COLOR;
    return work;
}

/* Release secondary then primary systems before freeing the shared work allocation. */
void effPCP_ThunderPolyLowDelete(EffThunderFragmentWork *work) {
    dds3TrackPolyDelete(work->state.secondarySystem);
    dds3TrackPolyDelete(work->system);
    sdfMemFreeDirect(work->allocationHandle);
}

/* vu0 routine: replace the second endpoint and translate the first by its delta.
   The native store/load order and all four components, including W, are retained. */
void effPCP_ThunderPolyLowSetPosition(u8 *endpoints, void *anchor) {
        SDF_VU0_LOADVECTOR(endpoints + EFF_THUNDER_ENDPOINT_VECTOR_BYTES);
        SDF_VU0_LOADVECTOR_B(anchor);
        SDF_VU0_STOREVECTOR_B(endpoints + EFF_THUNDER_ENDPOINT_VECTOR_BYTES);
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_LOADVECTOR(endpoints);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(endpoints);
}

/* Set the shared packed modulation color used by both fragment systems. */
void effPCP_ThunderPolyLowSetColor(EffThunderFragmentWork *work, u32 color) {
    work->color = color;
}

/* Seed the dual variant's shared fragment timing/color; both moduli remain unchecked. */
void effPCP_ThunderPolyLowDrawOff(EffThunderFragmentWork *work, s32 index) {
    EffThunderFrag *fragment = &work->fragments[index];

    fragment->delayFrames = dds3GetRandA((EffRandState *)effDefaultRandomState) % work->head.startDelayRange;
    fragment->activeFrames = dds3GetRandA((EffRandState *)effDefaultRandomState) % work->head.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
    fragment->color = EFF_THUNDER_FRAGMENT_GREY;
}

/* Build synchronized kind-0 core and kind-1 outer strips. Each step emits
 * two rows of two core vertices and three outer vertices from the same path.
 */
void effPCP_ThunderPolyLowPartRefresh(EffThunderFragmentWork *work, s32 index) {
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
    ParSystem *secondarySystem = work->state.secondarySystem;
    ParSystem *primarySystem = work->system;
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
    secondaryCell->vertexCount = secondarySystem->vertexWordCount;
    primaryCell->vertexCount = primarySystem->vertexWordCount;
    vertices = secondaryCell->history;
    coreVertices = primaryCell->history;
    segments = work->head.halfLife;
    SDF_VU0_LOADVECTOR(work->head.end);
    SDF_VU0_LOADVECTOR_B(work->head.start);
    SDF_VU0_SUBVECTOR_EXTENDED();
    SDF_VU0_GETLENGTH(distance);
    VU0_NORMALIZE_VF10_EXTENDED();
    SDF_VU0_STOREVECTOR_NOMEM(direction);
    distance /= work->head.halfLife;
    amplitude = work->head.waveAmplitude;
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(sdfViewTargetVector);
    SDF_VU0_SUBVECTOR_EXTENDED();
    SDF_VU0_STOREVECTOR_NOMEM(cameraDirection);
    SDF_VU0_LOADVECTOR_B(direction);
    VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10_EXTENDED();
    SDF_VU0_STOREVECTOR_NOMEM(side);
    phase = dds3GetRandFloatA(effDefaultRandomState) * (EFF_THUNDER_HALF_TURN * 2.0f);
    cycle = phase - (EFF_THUNDER_HALF_TURN * 2.0f);
    SDF_VU0_COPYVECTOR(position, work->head.start);
    currentWave = amplitude * sdfSin(phase);
    VEC3_SPLAT(waveScale, currentWave);
    SDF_VU0_LOADVECTOR(side);
    SDF_VU0_LOADVECTOR_B(waveScale);
    VU0_MUL_EXTENDED(vf10, vf10, vf11);
    SDF_VU0_LOADVECTOR_B(position);
    SDF_VU0_ADDVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B_NOMEM(position);
    VEC3_SPLAT(stepScale, distance);
    previousWave = currentWave;
    currentWave = amplitude * sdfSin(phase);
    VEC3_SPLAT(waveScale, currentWave - previousWave);
    SDF_VU0_LOADVECTOR(direction);
    SDF_VU0_LOADVECTOR_B(stepScale);
    VU0_MUL_EXTENDED(vf10, vf10, vf11);
    VU0_MOVE_VF_EXTENDED(vf12, vf10);
    SDF_VU0_LOADVECTOR(side);
    SDF_VU0_LOADVECTOR_B(waveScale);
    VU0_MUL_EXTENDED(vf10, vf10, vf11);
    VU0_ADD_EXTENDED(vf10, vf10, vf12);
    VU0_MOVE_VF_EXTENDED(vf12, vf10);
    SDF_VU0_LOADVECTOR_B(position);
    SDF_VU0_ADDVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B_NOMEM(next);
    VU0_MOVE_VF_EXTENDED(vf10, vf12);
    VU0_NORMALIZE_VF10_EXTENDED();
    SDF_VU0_STOREVECTOR_NOMEM(basis);

    for (i = 0; i < segments; i++) {
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10_EXTENDED();
        SDF_VU0_STOREVECTOR_NOMEM(basis);
        SDF_VU0_LOADVECTOR_B(width);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        SDF_VU0_STOREVECTOR_NOMEM(coreVertices);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        SDF_VU0_STOREVECTOR_B_NOMEM(coreVertices + 1);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(outerWidth);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 1);
        SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 2);
        coreVertices += 2;
        vertices += 3;
        SDF_VU0_COPYVECTOR(previousSide, basis);
        SDF_VU0_COPYVECTOR(position, next);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(width);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        SDF_VU0_STOREVECTOR_NOMEM(coreVertices);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        SDF_VU0_STOREVECTOR_B_NOMEM(coreVertices + 1);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(outerWidth);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 1);
        SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 2);
        coreVertices += 2;
        vertices += 3;
        phaseStep = (EFF_THUNDER_HALF_TURN / segments) * 4.0f;
        cycle += phaseStep;
        phase += phaseStep;
        if (cycle > EFF_THUNDER_HALF_TURN) {
            cycle -= EFF_THUNDER_HALF_TURN;
            amplitude = work->head.waveAmplitude * (1.0f - dds3GetRandFloatA(effDefaultRandomState) * 0.95f);
        }
        previousWave = currentWave;
        currentWave = amplitude * sdfSin(phase);
        VEC3_SPLAT(waveScale, currentWave - previousWave);
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_LOADVECTOR_B(stepScale);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        SDF_VU0_LOADVECTOR(side);
        SDF_VU0_LOADVECTOR_B(waveScale);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        SDF_VU0_STOREVECTOR_NOMEM(delta);
        SetRotateMatrixAxis(((dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f) * (20.0f * (EFF_THUNDER_HALF_TURN / 180.0f)), (const struct RwV3d *)cameraDirection);
        SDF_VU0_LOADVECTOR(delta);
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        SDF_VU0_LOADVECTOR_B(position);
        SDF_VU0_ADDVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(next);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_NORMALIZE_VF10_EXTENDED();
        SDF_VU0_STOREVECTOR_NOMEM(basis);
        SDF_VU0_LOADVECTOR(next);
        SDF_VU0_LOADVECTOR_B(position);
        SDF_VU0_SUBVECTOR_EXTENDED();
        VU0_NORMALIZE_VF10_EXTENDED();
        SDF_VU0_LOADVECTOR_B(previousSide);
        SDF_VU0_INNERPRODUCT(facing);
        /* Choose the join edge from the new direction relative to the prior side. */
        if (facing < 0.0f) {
            join = vertices - 1;
            SDF_VU0_LOADVECTOR_C(outerWidth);
        } else {
            join = vertices - 3;
            SDF_VU0_LOADVECTOR_C(negativeOuterWidth);
        }
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10_EXTENDED();
        VU0_MOVE_VF_EXTENDED(vf11, vf12);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        SDF_VU0_LOADVECTOR_B(join);
        VU0_ADD_EXTENDED(vf10, vf10, vf11);
        SDF_VU0_STOREVECTOR_NOMEM(position);
    }
}


extern void effPCP_ThunderPolyLowPartRefresh(EffThunderFragmentWork *work, s32 fragmentIndex);

/* One countdown/fade/restart state drives both systems; write the same tinted color to each.
   Secondary initialization/submission precedes primary; submission also occurs for count <= 0. */
void effPCP_ThunderPolyLowDraw(EffThunderFragmentWork *work) {
    s32 index = 0;
    ParSystem *secondarySystem = work->state.secondarySystem;
    s32 fragmentCount = work->head.fragmentCount;
    ParSystem *primarySystem = work->system;
    ParCell *primaryCells = primarySystem->cells;
    u32 tintColor = work->color;
    EffThunderFrag *fragment = work->fragments;
    ParCell *secondaryCells = secondarySystem->cells;
    u32 packedColor;

    if (fragmentCount > 0) {
        do {
            if (fragment->delayFrames == 0) {
                if (fragment->activeFrames != 0) {
                    effPCP_ThunderPolyLowPartRefresh(work, index);
                    fragment->activeFrames--;
                } else if (fragment->color & EFF_THUNDER_ALPHA_MASK) {
                    fragment->color -= EFF_THUNDER_ALPHA_FADE_STEP;
                } else {
                    effPCP_ThunderPolyLowDrawOff(work, index);
                    dds3TrackPolyListInit(work->state.secondarySystem, index);
                    dds3TrackPolyListInit(work->system, index);
                }
            } else {
                fragment->delayFrames--;
            }
            packedColor = effMagaCylinderCalcColor(fragment->color, tintColor);
            secondaryCells[index].color = packedColor;
            primaryCells[index].color = packedColor;
            fragment++;
            index++;
        } while (index < fragmentCount);
    }
    dds3TrackPolyDraw(work->state.secondarySystem);
    dds3TrackPolyDraw(work->system);
}

extern void effPCP_ThunderRadiatePolyDrawOff(EffThunderCellWork *work, s32 index);

/* Allocate cell states after the work, configure its system and sample directions/timing.
   The opaque settable word is not initialized here; no new default or range checks are added. */
EffThunderCellWork *effPCP_ThunderRadiatePolyCreate(EffThunderCellParams *parameters) {
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(parameters->cellCount * sizeof(EffThunderCell) + sizeof(EffThunderCellWork));
    EffThunderCellWork *work = (EffThunderCellWork *)sdfMemLock(allocationHandle);
    u32 i;

    work->head = *parameters;
    work->cells = (EffThunderCell *)(work + 1);
    work->allocationHandle = allocationHandle;
    work->system = dds3TrackPolyCreate(work->head.cellCount, work->head.halfLife * 2 - 1, 0, PAR_CELL_TOPOLOGY_SIX_VECTOR);
    dds3TrackPolyVC3Set_Acute(work->system, work->head.centerColor, work->head.middleColor, work->head.edgeColor);
    dds3TrackPolySetMode(work->system, work->head.systemParam);
    for (i = 0; i < work->head.cellCount; i++) {
        effPCP_ThunderRadiatePolyDrawOff(work, i);
    }
    return work;
}

/* Release the cell system before releasing the containing work allocation. */
void effPCP_ThunderRadiatePolyDelete(EffThunderCellWork *work) {
    dds3TrackPolyDelete(work->system);
    sdfMemFreeDirect(work->allocationHandle);
}

/* vu0 routine: copy the entire quadword, not only its XYZ components. */
void effPCP_ThunderRadiatePolySetPosition(void *destination, void *source) {
    SDF_VU0_COPYVECTOR(destination, source);
}

/* Store the opaque work word; no C consumer establishes a color/control interpretation. */
void effPCP_ThunderRadiatePolySetColor(EffThunderCellWork *work, u32 value) {
    work->unk4C = value;
}

/* Sample independent direction components without normalization, then timing.
   Keep the native add-to-double form and address-of-array RNG argument. */
void effPCP_ThunderRadiatePolyDrawOff(EffThunderCellWork *work, s32 index) {
    EffThunderCell *cell = work->cells + index;
    f32 centeredRandom;

    centeredRandom = dds3GetRandFloatA(&effDefaultRandomState) - EFF_THUNDER_RANDOM_MIDPOINT;
    cell->directionX = centeredRandom + centeredRandom;
    centeredRandom = dds3GetRandFloatA(&effDefaultRandomState) - EFF_THUNDER_RANDOM_MIDPOINT;
    cell->directionY = centeredRandom + centeredRandom;
    centeredRandom = dds3GetRandFloatA(&effDefaultRandomState) - EFF_THUNDER_RANDOM_MIDPOINT;
    cell->directionZ = centeredRandom + centeredRandom;
    cell->delayFrames = dds3GetRandA((EffRandState *)effDefaultRandomState) % work->head.startDelayRange;
    cell->activeFrames = dds3GetRandA((EffRandState *)effDefaultRandomState) % work->head.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
}

/* Build a kind-2 cell with three nested width bands. Each randomized step
 * emits two six-vector rows and then turns the direction around the view axis.
 */
void effPCP_ThunderRadiatePolyPartRefresh(EffThunderCellWork *work, s32 index) {
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
    ParSystem *system = work->system;
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
    cell->vertexCount = system->vertexWordCount;
    vertices = cell->history;
    segments = work->head.halfLife;
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(sdfViewTargetVector);
    SDF_VU0_SUBVECTOR_EXTENDED();
    SDF_VU0_STOREVECTOR_NOMEM(cameraDirection);
    direction[0] = source->directionX;
    direction[1] = source->directionY;
    direction[2] = source->directionZ;
    SDF_VU0_LOADVECTOR_B(direction);
    VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10_EXTENDED();
    SDF_VU0_STOREVECTOR_NOMEM(side);
    distance = work->head.length / work->head.halfLife;
    SDF_VU0_COPYVECTOR(position, work->head.origin);

    for (i = 0; i < segments; i++) {
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10_EXTENDED();
        SDF_VU0_STOREVECTOR_NOMEM(basis);
        SDF_VU0_LOADVECTOR_B(width);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 2);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 3);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(bandWidth);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 1);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 4);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(outerWidth);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 5);
        vertices += 6;
        stepLength = distance * (1.0f - dds3GetRandFloatA(effDefaultRandomState) * 0.9f);
        VEC3_SPLAT(stepScale, stepLength);
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_LOADVECTOR_B(stepScale);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        SDF_VU0_LOADVECTOR_B(position);
        VU0_ADD_EXTENDED(vf10, vf10, vf11);
        SDF_VU0_STOREVECTOR_NOMEM(position);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(width);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 2);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 3);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(bandWidth);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 1);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 4);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(outerWidth);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 5);
        vertices += 6;
        SetRotateMatrixAxis(((dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f) * (70.0f * (EFF_THUNDER_HALF_TURN / 180.0f)), (const struct RwV3d *)cameraDirection);
        SDF_VU0_LOADVECTOR(direction);
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        SDF_VU0_STOREVECTOR_NOMEM(direction);
        SDF_VU0_LOADVECTOR_B(basis);
        SDF_VU0_INNERPRODUCT(facing);
        if (facing < 0.0f) {
            join = vertices - 1;
            SDF_VU0_LOADVECTOR_C(outerWidth);
        } else {
            join = vertices - 6;
            SDF_VU0_LOADVECTOR_C(negativeOuterWidth);
        }
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10_EXTENDED();
        SDF_VU0_STOREVECTOR_NOMEM(basis);
        VU0_MOVE_VF_EXTENDED(vf11, vf12);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        SDF_VU0_LOADVECTOR_B(join);
        VU0_ADD_EXTENDED(vf10, vf10, vf11);
        SDF_VU0_STOREVECTOR_NOMEM(position);
    }
}


extern void effPCP_ThunderRadiatePolyPartRefresh(EffThunderCellWork *work, s32 index);

/* Delay -> active geometry -> render-cell alpha fade -> restart.
   Unlike fragment/vector state, color lives in the render cells; submit even for count <= 0. */
void effPCP_ThunderRadiatePolyDraw(EffThunderCellWork *work) {
    s32 i = 0;
    s32 cellCount = work->head.cellCount;
    EffThunderCell *cell = work->cells;
    ParCell *renderCells = work->system->cells;

    if (cellCount > 0) {
        do {
            if (cell->delayFrames == 0) {
                if (cell->activeFrames != 0) {
                    effPCP_ThunderRadiatePolyPartRefresh(work, i);
                    cell->activeFrames--;
                } else if (renderCells[i].color & EFF_THUNDER_ALPHA_MASK) {
                    renderCells[i].color -= EFF_THUNDER_ALPHA_FADE_STEP;
                } else {
                    effPCP_ThunderRadiatePolyDrawOff(work, i);
                    dds3TrackPolyListInit(work->system, i);
                }
            } else {
                cell->delayFrames--;
            }
            cell++;
            i++;
        } while (i < cellCount);
    }
    dds3TrackPolyDraw(work->system);
}




struct EffThunderGroup {
    EffThunderGroupParams head;
    EffThunderFragmentWork *handles[10];
    u32 color;
};
typedef char EffThunderGroupHeadOffsetCheck[
    ((u32)&((EffThunderGroup *)0)->head == 0x00) ? 1 : -1];
typedef char EffThunderGroupColorOffsetCheck[
    ((u32)&((EffThunderGroup *)0)->color == 0x120) ? 1 : -1];


EffThunderGroup *effPCP_ThunderPolyListCreate(EffThunderGroupParams *src) {
    EffThunderGroup *group = sdfChipAllocDirect(sizeof(EffThunderGroup));
    s32 i;

    memset(group->handles, 0, sizeof(group->handles));
    group->head = *src;
    group->handles[0] = effPCP_ThunderPolyCreate(&group->head.params);
    effPCP_ThunderPolySetVCAcute2(group->handles[0]);
    for (i = 1; i < src->count - 2; i++) {
        group->handles[i] = effPCP_ThunderPolyCreate(&group->head.params);
        effThunderApplyFragmentColorBands(group->handles[i]);
    }
    group->handles[i] = effPCP_ThunderPolyCreate(&group->head.params);
    effPCP_ThunderPolySetVCAcute(group->handles[i]);
    group->color = 0x80808080;
    return group;
}



extern void effPCP_ThunderPolyPartRefresh(EffThunderFragmentWork *work, s32 index);
extern void effPCP_ThunderPolyListPartRefresh(EffThunderFragmentWork *work, s32 index, const u128 *seed);


void effPCP_ThunderPolyListDelete(EffThunderGroup *group) {
    s32 fragmentIndex;

    for (fragmentIndex = 0; fragmentIndex < group->head.count - 1; fragmentIndex++) {
        effPCP_ThunderPolyDelete(group->handles[fragmentIndex]);
    }
    sdfChipFreeDirect(group);
}

/* Expose the group parameters; the paired blur caller fills the leading points. */
EffThunderGroupParams *effPCP_ThunderPolyListGetParam(EffThunderGroup *resource) {
    return &resource->head;
}

/* Sets the blend colour word of a resource handle (see the blur caller in
   code_00185E18.c). */
void effPCP_ThunderPolyListSetColor(EffThunderGroup *resource, u32 color) {
    resource->color = color;
}

/* Continue a strip from the preceding segment's final five-vector row.
 * Each iteration emits two rows; seed[3] is the connection anchor used by
 * this profile, not its middle vertex. XYZ calculations and drawing ignore W.
 */
void effPCP_ThunderPolyListPartRefresh(EffThunderFragmentWork *work, s32 index, const u128 *seed) {
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
    ParSystem *system = work->system;
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
    cell->vertexCount = system->vertexWordCount;
    vertices = cell->history;
    segments = work->head.halfLife;
    SDF_VU0_LOADVECTOR(work->head.end);
    SDF_VU0_LOADVECTOR_B(seed + 3);
    SDF_VU0_SUBVECTOR_EXTENDED();
    SDF_VU0_GETLENGTH(distance);
    VU0_NORMALIZE_VF10_EXTENDED();
    SDF_VU0_STOREVECTOR_NOMEM(direction);
    distance /= work->head.halfLife;
    amplitude = work->head.waveAmplitude;
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(sdfViewTargetVector);
    SDF_VU0_SUBVECTOR_EXTENDED();
    SDF_VU0_STOREVECTOR_NOMEM(cameraDirection);
    SDF_VU0_LOADVECTOR_B(direction);
    VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10_EXTENDED();
    SDF_VU0_STOREVECTOR_NOMEM(side);
    phase = dds3GetRandFloatA(effDefaultRandomState) * (EFF_THUNDER_HALF_TURN * 2.0f);
    cycle = phase - (EFF_THUNDER_HALF_TURN * 2.0f);
    SDF_VU0_COPYVECTOR(position, seed + 3);
    currentWave = amplitude * sdfSin(phase);
    VEC3_SPLAT(waveScale, currentWave);
    SDF_VU0_LOADVECTOR(side);
    SDF_VU0_LOADVECTOR_B(waveScale);
    VU0_MUL_EXTENDED(vf10, vf10, vf11);
    SDF_VU0_LOADVECTOR_B(position);
    SDF_VU0_ADDVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B_NOMEM(position);
    VEC3_SPLAT(stepScale, distance);
    previousWave = currentWave;
    currentWave = amplitude * sdfSin(phase);
    VEC3_SPLAT(waveScale, currentWave - previousWave);
    SDF_VU0_LOADVECTOR(direction);
    SDF_VU0_LOADVECTOR_B(stepScale);
    VU0_MUL_EXTENDED(vf10, vf10, vf11);
    VU0_MOVE_VF_EXTENDED(vf12, vf10);
    SDF_VU0_LOADVECTOR(side);
    SDF_VU0_LOADVECTOR_B(waveScale);
    VU0_MUL_EXTENDED(vf10, vf10, vf11);
    VU0_ADD_EXTENDED(vf10, vf10, vf12);
    VU0_MOVE_VF_EXTENDED(vf12, vf10);
    SDF_VU0_LOADVECTOR_B(position);
    SDF_VU0_ADDVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B_NOMEM(next);
    VU0_MOVE_VF_EXTENDED(vf10, vf12);
    VU0_NORMALIZE_VF10_EXTENDED();
    SDF_VU0_STOREVECTOR_NOMEM(basis);

    for (i = 0; i < segments; i++) {
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10_EXTENDED();
        SDF_VU0_STOREVECTOR_NOMEM(basis);
        if (i > 0) {
            SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
            SDF_VU0_LOADVECTOR(position);
            SDF_VU0_STOREVECTOR_NOMEM(vertices + 2);
            VU0_MOVE_VF_EXTENDED(vf10, vf11);
            SDF_VU0_LOADVECTOR_B(width);
            VU0_MUL_EXTENDED(vf10, vf10, vf11);
            VU0_MOVE_VF_EXTENDED(vf12, vf10);
            SDF_VU0_LOADVECTOR(position);
            SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
            VU0_ADD_EXTENDED(vf10, vf10, vf12);
            SDF_VU0_STOREVECTOR_NOMEM(vertices + 1);
            VU0_MOVE_VF_EXTENDED(vf10, vf12);
            VU0_SUB_EXTENDED(vf11, vf11, vf10);
            SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 3);
            SDF_VU0_LOADVECTOR(basis);
            SDF_VU0_LOADVECTOR_B(outerWidth);
            VU0_MUL_EXTENDED(vf10, vf10, vf11);
            VU0_MOVE_VF_EXTENDED(vf12, vf10);
            SDF_VU0_LOADVECTOR(position);
            SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
            VU0_ADD_EXTENDED(vf10, vf10, vf12);
            SDF_VU0_STOREVECTOR_NOMEM(vertices);
            VU0_MOVE_VF_EXTENDED(vf10, vf12);
            VU0_SUB_EXTENDED(vf11, vf11, vf10);
            SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 4);
            vertices += 5;
        } else {
            SDF_VU0_COPYVECTOR(vertices, seed);
            SDF_VU0_COPYVECTOR(vertices + 1, seed + 1);
            SDF_VU0_COPYVECTOR(vertices + 2, seed + 2);
            SDF_VU0_COPYVECTOR(vertices + 3, seed + 3);
            SDF_VU0_COPYVECTOR(vertices + 4, seed + 4);
            vertices += 5;
        }
        SDF_VU0_COPYVECTOR(previousSide, basis);
        SDF_VU0_COPYVECTOR(position, next);
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 2);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(width);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 1);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 3);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(outerWidth);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_SUB_EXTENDED(vf11, vf11, vf10);
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 4);
        vertices += 5;
        phaseStep = (EFF_THUNDER_HALF_TURN / segments) * 4.0f;
        cycle += phaseStep;
        phase += phaseStep;
        if (cycle > EFF_THUNDER_HALF_TURN) {
            cycle -= EFF_THUNDER_HALF_TURN;
            amplitude = work->head.waveAmplitude * (1.0f - dds3GetRandFloatA(effDefaultRandomState) * 0.95f);
        }
        previousWave = currentWave;
        currentWave = amplitude * sdfSin(phase);
        VEC3_SPLAT(waveScale, currentWave - previousWave);
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_LOADVECTOR_B(stepScale);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        SDF_VU0_LOADVECTOR(side);
        SDF_VU0_LOADVECTOR_B(waveScale);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        VU0_ADD_EXTENDED(vf10, vf10, vf12);
        SDF_VU0_STOREVECTOR_NOMEM(delta);
        SetRotateMatrixAxis(((dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f) * (20.0f * (EFF_THUNDER_HALF_TURN / 180.0f)), (const struct RwV3d *)cameraDirection);
        SDF_VU0_LOADVECTOR(delta);
        VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
        VU0_MOVE_VF_EXTENDED(vf12, vf10);
        SDF_VU0_LOADVECTOR_B(position);
        SDF_VU0_ADDVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(next);
        VU0_MOVE_VF_EXTENDED(vf10, vf12);
        VU0_NORMALIZE_VF10_EXTENDED();
        SDF_VU0_STOREVECTOR_NOMEM(basis);
        SDF_VU0_LOADVECTOR(next);
        SDF_VU0_LOADVECTOR_B(position);
        SDF_VU0_SUBVECTOR_EXTENDED();
        VU0_NORMALIZE_VF10_EXTENDED();
        SDF_VU0_LOADVECTOR_B(previousSide);
        SDF_VU0_INNERPRODUCT(facing);
        /* Choose the join edge from the new direction relative to the prior side. */
        if (facing < 0.0f) {
            join = vertices - 1;
            SDF_VU0_LOADVECTOR_C(outerWidth);
        } else {
            join = vertices - 5;
            SDF_VU0_LOADVECTOR_C(negativeOuterWidth);
        }
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10_EXTENDED();
        VU0_MOVE_VF_EXTENDED(vf11, vf12);
        VU0_MUL_EXTENDED(vf10, vf10, vf11);
        SDF_VU0_LOADVECTOR_B(join);
        VU0_ADD_EXTENDED(vf10, vf10, vf11);
        SDF_VU0_STOREVECTOR_NOMEM(position);
    }
}


void effPCP_ThunderPolyListDraw(EffThunderGroup *group) {
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
        SDF_VU0_COPYVECTOR(work->head.start, points[0]);
        SDF_VU0_COPYVECTOR(work->head.end, points[1]);
        points++;
        fragmentCount = work->head.fragmentCount;
        color = effMagaCylinderCalcColor(group->color, work->color);
        cell = work->system->cells;
        for (j = 0; j < fragmentCount; j++) {
            if (i > 0) {
                previous = group->handles[i - 1];
                effPCP_ThunderPolyListPartRefresh(work, j,
                    previous->system->cells[j].history + previous->system->cells[j].vertexCount - 5);
            } else {
                effPCP_ThunderPolyPartRefresh(work, j);
            }
            cell[j].color = color;
        }
        work->state.updateCount++;
        dds3TrackPolyDraw(work->system);
    }
}

extern EffPcpThunderFragmentResources *dds3BezierTrackPolySetVertex(s32 historyLength, s32 subdivisions);
extern void dds3BezierTrackPolyCreate(EffPcpThunderFragmentResources *history, u32 *gradientColors);

EffPcpThunderGroup *effPCPBezierBillCreate(const EffPcpThunderGroupParams *src, SoundMixer *eventParams)
{
    u32 count = src->groupCount;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(count * sizeof(EffPcpThunderGroupSlot) + sizeof(EffPcpThunderGroup));
    EffPcpThunderGroup *work = (EffPcpThunderGroup *)sdfMemLock(allocation);
    EffPcpThunderGroupSlot *slot = (EffPcpThunderGroupSlot *)(work + 1);
    EffPCPEventPlace place;
    u32 palette[4];
    s32 historyLength, subdivisions;
    u32 i;

    work->params = *src;
    work->allocation = allocation;
    work->ownsMixerVoices = 1;
    work->slots = slot;
    work->tintColor = 0x80808080;
    work->soundMixer = effEveBEDCreate(eventParams);
    historyLength = work->params.historyLength;
    if (historyLength == 0) {
        work->params.historyLength = 1;
        historyLength = 1;
    }
    subdivisions = work->params.subdivisions;
    if (subdivisions <= 0) {
        work->params.subdivisions = 1;
        subdivisions = 1;
    }
    if (work->params.startDelayRange <= 0) {
        work->params.startDelayRange = 1;
    }
    palette[0] = work->params.palette[0];
    palette[1] = work->params.palette[1];
    palette[2] = work->params.palette[2];
    palette[3] = work->params.palette[3];
    place.payload[0] = 0;
    place.payload[1] = 0;
    place.payload[2] = 0;
    place.payload[3] = 0;
    place.payload[4] = 0;
    place.payload[5] = 0;
    place.payload[6] = 0;
    place.payload[7] = 1.0f;
    place.payload[8] = 100.0f;
    place.payload[9] = 100.0f;
    place.payload[10] = 1.0f;
    place.color = 0x80808080;
    for (i = 0; i < count; i++, slot++) {
        slot->fragment = dds3BezierTrackPolySetVertex(historyLength, subdivisions);
        dds3BezierTrackPolyCreate(slot->fragment, palette);
        slot->age = 0;
        slot->fadeScale = 1.0f;
        slot->curve.pointIndex = 0;
        slot->curve.t = 0;
        slot->curve.parameterStep = 0;
        slot->eventNode = effEveBEDCutCreate(work->soundMixer, 2, &place);
    }
    return work;
}

/* Forward the first two parameter-table blocks as one effect-handler pair. */
EffPcpThunderGroup *effPCPBezierBillCreatePCF(const EffParamTable *paramTable) {
    void *firstBlock;
    void *secondBlock;

    firstBlock = dds3EffectPCPDataGetData(paramTable, 0);
    secondBlock = dds3EffectPCPDataGetData(paramTable, 1);
    return effPCPBezierBillCreate(firstBlock, secondBlock);
}

EffPcpThunderGroup *effPCPBezierBillCopy(EffPcpThunderGroup *src) {
    u32 count = src->params.groupCount;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(count * sizeof(EffPcpThunderGroupSlot) + sizeof(EffPcpThunderGroup));
    EffPcpThunderGroup *work = (EffPcpThunderGroup *)sdfMemLock(allocation);
    EffPcpThunderGroupSlot *slot = (EffPcpThunderGroupSlot *)(work + 1);
    EffPCPEventPlace place;
    u32 palette[4];
    s32 historyLength, subdivisions;
    u32 i;

    work->params = src->params;
    work->ownsMixerVoices = 0;
    work->allocation = allocation;
    work->soundMixer = src->soundMixer;
    work->slots = slot;
    work->tintColor = 0x80808080;
    historyLength = work->params.historyLength;
    if (historyLength == 0) {
        work->params.historyLength = 1;
        historyLength = 1;
    }
    subdivisions = work->params.subdivisions;
    if (subdivisions <= 0) {
        work->params.subdivisions = 1;
        subdivisions = 1;
    }
    if (work->params.startDelayRange <= 0) {
        work->params.startDelayRange = 1;
    }
    palette[0] = work->params.palette[0];
    palette[1] = work->params.palette[1];
    palette[2] = work->params.palette[2];
    palette[3] = work->params.palette[3];
    place.payload[0] = 0;
    place.payload[1] = 0;
    place.payload[2] = 0;
    place.payload[3] = 0;
    place.payload[4] = 0;
    place.payload[5] = 0;
    place.payload[6] = 0;
    place.payload[7] = 1.0f;
    place.payload[8] = 100.0f;
    place.payload[9] = 100.0f;
    place.payload[10] = 1.0f;
    place.color = 0x80808080;
    for (i = 0; i < count; i++, slot++) {
        slot->fragment = dds3BezierTrackPolySetVertex(historyLength, subdivisions);
        dds3BezierTrackPolyCreate(slot->fragment, palette);
        slot->age = 0;
        slot->fadeScale = 1.0f;
        slot->curve.pointIndex = 0;
        slot->curve.t = 0;
        slot->curve.parameterStep = 0;
        slot->eventNode = effEveBEDCutCreate(work->soundMixer, 2, &place);
    }
    return work;
}


extern void dds3BezierTrackPolyDelete(EffPcpThunderFragmentResources *work);

/* Release every slot's effect resources and event node, then the optional handle and the group allocation. */
void effPCPBezierBillDelete(EffPcpThunderGroup *group) {
    u32 i = 0;
    u32 count = group->params.groupCount;
    EffPcpThunderGroupSlot *slot = group->slots;

    if (count != 0) {
        do {
            i++;
            dds3BezierTrackPolyDelete(slot->fragment);
            effEveBEDCutDelete(slot->eventNode);
            slot++;
        } while (i < count);
    }
    if (group->ownsMixerVoices != 0) {
        effEveBEDDelete(group->soundMixer);
    }
    sdfMemFreeDirect(group->allocation);
}

/* The event holder consumes the same packed 0x30-byte record as effEvent.c. */
typedef struct {
    u8 bytes[0x30];
} __attribute__((packed)) FileRecordHeader;

extern u32 effBTLFieldColorUsesActorSelectors(void);
extern u32 effBTLFieldColorGetOriginalSelector(void);
extern u32 effBTLFieldColorGetVariantSelector(void);
extern void dds3BezierTrackPolyReset(EffPcpThunderFragmentResources *history);
extern void dds3BezierTrackPolySetVC(EffPcpThunderFragmentResources *history, const u128 *source);
void dds3BezierTrackPolyDraw(EffPcpThunderFragmentResources *);

/* Each slot owns two joined cubic segments, a ribbon history and an end cap.
 * VU helpers use the SDF vf10/vf11/vf12 register convention; callees returning
 * a vector leave it in vf10. Keep the SDK-form stores paired with their
 * checked C readback; see docs/thunder-bezier-matching.md for the limits.
 */
typedef f32 EffThunderVector[4] __attribute__((aligned(16)));

void effThunderUpdateCurveGroup(EffPcpThunderGroup *group) {
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
    EffPcpThunderGroupSlot *slot;
    f32 (*cap)[4];

    count = group->params.groupCount;
    curveFrames = group->params.curveFrames;
    subdivisions = group->params.subdivisions - 1;
    fadeStep = 1.0f / group->params.fadeFrames;
    delayRange = group->params.startDelayRange;
    mode = group->params.mode;
    color = group->tintColor;
    slot = group->slots;
    SDF_VU0_COPYVECTOR(origin, group->params.origin);
    place.payload[4] = 0.0f;
    place.payload[5] = 0.0f;
    place.payload[6] = 0.0f;
    place.payload[7] = 1.0f;
    place.payload[8] = 100.0f;
    place.payload[9] = 100.0f;
    place.payload[10] = 1.0f;
    if (effBTLFieldColorUsesActorSelectors()) {
        u32 actor;
        if (mode == 3) actor = effBTLFieldColorGetVariantSelector();
        else actor = effBTLFieldColorGetOriginalSelector();
        btlUnitGetMuzzlePosVU((struct BtlUnit *)actor);
        SDF_VU0_STOREVECTOR_NOMEM(start);
    } else {
        start[0] = 0.0f;
        start[1] = -80.0f;
        start[2] = -400.0f;
    }
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(sdfViewTargetVector);
    SDF_VU0_SUBVECTOR_EXTENDED();
    SDF_VU0_STOREVECTOR_NOMEM(cameraDirection);
    VEC3_SPLAT(width, group->params.width * 0.5f);
    for (i = 0; i < count; i++, slot++) {
        if (slot->curve.parameterStep == 0.0f) {
            yOffset = -group->params.height * dds3GetRandFloatA(effDefaultRandomState);
            zOffset = group->params.spreadZ * ((dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f);
            xOffset = group->params.spreadX * ((dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f);
            end[0] = origin[0] + xOffset;
            end[1] = origin[1] - yOffset;
            end[2] = origin[2] + zOffset;
            SDF_VU0_LOADVECTOR(end);
            SDF_VU0_LOADVECTOR_B(start);
            SDF_VU0_SUBVECTOR_EXTENDED();
            SDF_VU0_GETLENGTH(distance);
            VU0_NORMALIZE_VF10_EXTENDED();
            SDF_VU0_STOREVECTOR_NOMEM(direction);
            switch (mode) {
            case 0:
                offset[0] = ((dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f);
                offset[1] = -dds3GetRandFloatA(effDefaultRandomState);
                offset[2] = 0.0f;
                SDF_VU0_LOADVECTOR(offset);
                VU0_NORMALIZE_VF10_EXTENDED();
                SDF_VU0_STOREVECTOR_NOMEM(offset);
                SetRotateMatrixAxis(((dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f) * 0.5235987f, (const struct RwV3d *)direction);
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
                SDF_VU0_LOADVECTOR(offset);
                VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
                SDF_VU0_STOREVECTOR_NOMEM(offset);
                slot->curve.controlPoints[3].x = spread * offset[0] + along * direction[0] + start[0];
                slot->curve.controlPoints[3].y = spread * offset[1] + along * direction[1] + start[1];
                slot->curve.controlPoints[3].z = spread * offset[2] + along * direction[2] + start[2];
                if (slot->curve.controlPoints[3].y > -10.0f) slot->curve.controlPoints[3].y = -10.0f;
                along = distance * 0.5f;
                spread = 150.0f;
                SDF_VU0_LOADVECTOR(offset);
                VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
                SDF_VU0_STOREVECTOR_NOMEM(offset);
                slot->curve.controlPoints[4].x = spread * offset[0] + along * direction[0] + start[0];
                slot->curve.controlPoints[4].y = spread * offset[1] + along * direction[1] + start[1];
                slot->curve.controlPoints[4].z = spread * offset[2] + along * direction[2] + start[2];
                if (slot->curve.controlPoints[4].y > -10.0f) slot->curve.controlPoints[4].y = -10.0f;
                along = distance * 0.75f;
                spread = 65.0f;
                SDF_VU0_LOADVECTOR(offset);
                VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
                SDF_VU0_STOREVECTOR_NOMEM(offset);
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
                offset[0] = ((dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f) * 0.5f;
                offset[1] = -dds3GetRandFloatA(effDefaultRandomState);
                offset[2] = 0.0f;
                SDF_VU0_LOADVECTOR(offset);
                VU0_NORMALIZE_VF10_EXTENDED();
                SDF_VU0_STOREVECTOR_NOMEM(offset);
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
                offset[0] = ((dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f);
                offset[1] = -dds3GetRandFloatA(effDefaultRandomState);
                offset[2] = 0.0f;
                SDF_VU0_LOADVECTOR(offset);
                VU0_NORMALIZE_VF10_EXTENDED();
                SDF_VU0_STOREVECTOR_NOMEM(offset);
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
                offset[0] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
                offset[1] = -(dds3GetRandFloatA(effDefaultRandomState) * 0.3f + 0.7f);
                offset[2] = 0.0f;
                SDF_VU0_LOADVECTOR(offset);
                VU0_NORMALIZE_VF10_EXTENDED();
                SDF_VU0_STOREVECTOR_NOMEM(offset);
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
            effPCPBezierEval(&slot->curve, point);
            slot->position[0] = point[0];
            slot->position[1] = point[1];
            slot->position[2] = point[2];
            slot->age = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % delayRange);
            dds3BezierTrackPolyReset(slot->fragment);
        } else if (slot->age++ >= 0) {
            if (slot->curve.t >= 1.0f && slot->curve.pointIndex >= 3) {
                if (slot->fadeScale > fadeStep) {
                    slot->fadeScale -= fadeStep;
                    slot->fragment->color = ((u32)(slot->fadeScale * 128.0f) << 24) | 0x808080;
                    slot->fragment->color = effMagaCylinderCalcColor(color, slot->fragment->color);
                } else {
                    dds3BezierTrackPolyReset(slot->fragment);
                }
            } else {
                previous[0] = slot->position[0];
                previous[1] = slot->position[1];
                previous[2] = slot->position[2];
                for (j = 0; j < subdivisions; j++) {
                    effPCPBezierEval(&slot->curve, point);
                    SDF_VU0_LOADVECTOR(point);
                    VU0_MOVE_VF_EXTENDED(vf12, vf10);
                    SDF_VU0_LOADVECTOR_B(previous);
                    SDF_VU0_SUBVECTOR_EXTENDED();
                    SDF_VU0_LOADVECTOR_B(cameraDirection);
                    VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
                    VU0_NORMALIZE_VF10_EXTENDED();
                    SDF_VU0_LOADVECTOR_B(width);
                    VU0_MUL_EXTENDED(vf10, vf10, vf11);
                    VU0_MOVE_VF_EXTENDED(vf2, vf10);
                    VU0_MOVE_VF_EXTENDED(vf10, vf12);
                    VU0_MOVE_VF_EXTENDED(vf12, vf2);
                    SDF_VU0_STOREVECTOR_NOMEM(ribbon[1]);
                    SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
                    VU0_ADD_EXTENDED(vf10, vf10, vf12);
                    SDF_VU0_STOREVECTOR_NOMEM(ribbon[0]);
                    VU0_MOVE_VF_EXTENDED(vf10, vf12);
                    VU0_SUB_EXTENDED(vf11, vf11, vf10);
                    SDF_VU0_STOREVECTOR_B_NOMEM(ribbon[2]);
                    SDF_VU0_COPYVECTOR(previous, point);
                    dds3BezierTrackPolySetVC(slot->fragment, (u128 *)ribbon);
                }
                effPCPBezierEval(&slot->curve, point);
                SDF_VU0_LOADVECTOR(point);
                VU0_MOVE_VF_EXTENDED(vf12, vf10);
                SDF_VU0_LOADVECTOR_B(previous);
                SDF_VU0_SUBVECTOR_EXTENDED();
                VU0_NORMALIZE_VF10_EXTENDED();
                SDF_VU0_STOREVECTOR_NOMEM(tangent);
                SDF_VU0_LOADVECTOR_B(cameraDirection);
                VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
                VU0_NORMALIZE_VF10_EXTENDED();
                SDF_VU0_STOREVECTOR_NOMEM(side);
                SDF_VU0_LOADVECTOR_B(width);
                VU0_MUL_EXTENDED(vf10, vf10, vf11);
                VU0_MOVE_VF_EXTENDED(vf2, vf10);
                VU0_MOVE_VF_EXTENDED(vf10, vf12);
                VU0_MOVE_VF_EXTENDED(vf12, vf2);
                SDF_VU0_STOREVECTOR_NOMEM(ribbon[1]);
                SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
                VU0_ADD_EXTENDED(vf10, vf10, vf12);
                SDF_VU0_STOREVECTOR_NOMEM(ribbon[0]);
                VU0_MOVE_VF_EXTENDED(vf10, vf12);
                VU0_SUB_EXTENDED(vf11, vf11, vf10);
                SDF_VU0_STOREVECTOR_B_NOMEM(ribbon[2]);
                dds3BezierTrackPolySetVC(slot->fragment, (u128 *)ribbon);
                cap = (f32 (*)[4])slot->fragment->endPoints;
                slot->position[0] = point[0];
                slot->position[1] = point[1];
                slot->position[2] = point[2];
                SDF_VU0_COPYVECTOR(cap[0], ribbon[1]);
                SDF_VU0_COPYVECTOR(cap[1], ribbon[0]);
                SDF_VU0_COPYVECTOR(cap[7], ribbon[2]);
                oppositeSide[0] = -side[0];
                oppositeSide[1] = -side[1];
                oppositeSide[2] = -side[2];
                oppositeSide[3] = -side[3];
                SDF_VU0_LOADVECTOR_C(ribbon[1]);
                SDF_VU0_LOADVECTOR(side);
                SDF_VU0_LOADVECTOR_B(tangent);
                VU0_LERP_VF10(0.3333333333f);
                VU0_NORMALIZE_VF10_EXTENDED();
                SDF_VU0_LOADVECTOR_B(width);
                VU0_MUL_EXTENDED(vf10, vf10, vf11);
                VU0_ADD_EXTENDED(vf10, vf10, vf12);
                SDF_VU0_STOREVECTOR_NOMEM(cap[2]);
                SDF_VU0_LOADVECTOR(side);
                SDF_VU0_LOADVECTOR_B(tangent);
                VU0_LERP_VF10(0.6666666667f);
                VU0_NORMALIZE_VF10_EXTENDED();
                SDF_VU0_LOADVECTOR_B(width);
                VU0_MUL_EXTENDED(vf10, vf10, vf11);
                VU0_ADD_EXTENDED(vf10, vf10, vf12);
                SDF_VU0_STOREVECTOR_NOMEM(cap[3]);
                SDF_VU0_LOADVECTOR(tangent);
                VU0_MUL_EXTENDED(vf10, vf10, vf11);
                VU0_ADD_EXTENDED(vf10, vf10, vf12);
                SDF_VU0_STOREVECTOR_NOMEM(cap[4]);
                SDF_VU0_LOADVECTOR(oppositeSide);
                SDF_VU0_LOADVECTOR_B(tangent);
                VU0_LERP_VF10(0.6666666667f);
                VU0_NORMALIZE_VF10_EXTENDED();
                SDF_VU0_LOADVECTOR_B(width);
                VU0_MUL_EXTENDED(vf10, vf10, vf11);
                VU0_ADD_EXTENDED(vf10, vf10, vf12);
                SDF_VU0_STOREVECTOR_NOMEM(cap[5]);
                SDF_VU0_LOADVECTOR(oppositeSide);
                SDF_VU0_LOADVECTOR_B(tangent);
                VU0_LERP_VF10(0.3333333333f);
                VU0_NORMALIZE_VF10_EXTENDED();
                SDF_VU0_LOADVECTOR_B(width);
                VU0_MUL_EXTENDED(vf10, vf10, vf11);
                VU0_ADD_EXTENDED(vf10, vf10, vf12);
                SDF_VU0_STOREVECTOR_NOMEM(cap[6]);
            }
            if (slot->curve.t >= 1.0f && slot->curve.pointIndex >= 3) {
                place.payload[0] = slot->position[0];
                place.payload[1] = slot->position[1];
                place.payload[2] = slot->position[2];
                place.payload[3] = 0.0f;
                place.color = color;
                delta[0] = start[0] - place.payload[0];
                delta[2] = start[2] - place.payload[2];
                sdfConvertEulerAnglesToQuaternionVU(0.0f, sdfAtan2(delta[0], delta[2]), 0.0f);
                SDF_VU0_STOREVECTOR_NOMEM(&place.payload[4]);
                effEveBEDCutSetParams(slot->eventNode, &place);
                effEventUpdateEffectParameters(slot->eventNode);
            }
            dds3BezierTrackPolyDraw(slot->fragment);
        }
    }
}


void effCopyResourceGroupVector(void *dst, void *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

void effThunderSetCurveGroupTint(EffPcpThunderGroup *group, u32 value) {
    group->tintColor = value;
}

extern EffThunderDrawParams dds3BezierVertexCount;
extern EffThunderDrawParams dds3BezierTrackPolyVertex;

extern const f32 effThunderEndCapPoints[8][4];
extern const u32 effThunderHistoryIndices[28];
extern const u32 effThunderEndCapIndices[8];

/* Lay out history vertices/colors, eight cap vertices/colors, then the owner.
 * The two global draw descriptors are reset through their native 0x2C prefix.
 */
EffPcpThunderFragmentResources *dds3BezierTrackPolySetVertex(s32 historyLength, s32 subdivisions) {
    s32 count = historyLength * subdivisions * 3 + 6;
    u32 colorBytes = count * sizeof(u32);
    u32 bufferBytes = count * (sizeof(u128) + sizeof(u32)) + 8 * (sizeof(u128) + sizeof(u32));
    sdfMemHandle_t *allocation = sdfMemAllocDirect(bufferBytes + sizeof(EffPcpThunderFragmentResources));
    u8 *storage = (u8 *)sdfMemLock(allocation);
    EffPcpThunderFragmentResources *history = (EffPcpThunderFragmentResources *)(storage + bufferBytes);
    sdfMat_t *asset;

    history->points = (u128 *)storage;
    storage += count * sizeof(u128);
    history->endPoints = (u128 *)storage;
    storage += 8 * sizeof(u128);
    history->colors = (u32 *)storage;
    storage += colorBytes;
    history->endColors = (u32 *)storage;
    history->surfaceIndex = 2;
    history->pointCapacity = count;
    history->nextPointWriteIndex = 3;
    history->subdivisions = subdivisions;
    history->color = 0x80808080;
    history->allocation = allocation;
    history->activePointCount = 0;
    memcpy(history->endPoints, effThunderEndCapPoints, sizeof(effThunderEndCapPoints));
    asset = sdfMatNew();
    history->resourceHandle = asset;
    sdfMatSetConstant(asset, 1.0f);
    memset(&dds3BezierVertexCount, 0, 0x2C);
    dds3BezierVertexCount.flags = 0x4000;
    dds3BezierVertexCount.primitiveIndices = effThunderHistoryIndices;
    memset(&dds3BezierTrackPolyVertex, 0, 0x2C);
    dds3BezierTrackPolyVertex.flags = 0x4000;
    dds3BezierTrackPolyVertex.primitiveIndices = effThunderEndCapIndices;
    dds3BezierTrackPolyVertex.primitiveCount = 6;
    dds3BezierTrackPolyVertex.pointCount = 8;
    return history;
}

void dds3BezierTrackPolyDelete(EffPcpThunderFragmentResources *work) {
    sdfMatFreeDelayed(work->resourceHandle);
    sdfMemFreeDirect(work->allocation);
}

/* Reset the point ring and restore the fragment's neutral packed color. */
void dds3BezierTrackPolyReset(EffPcpThunderFragmentResources *history) {
    history->nextPointWriteIndex = 3;
    history->color = 0x80808080;
    history->activePointCount = 0;
}

void dds3BezierTrackPolyCreate(EffPcpThunderFragmentResources *history, u32 *gradientColors) {
    f32 t = 0.0f;
    u32 count = history->pointCapacity / 3;
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

            colors[0] = effColorBlend(outerEnd, outerStart, t);
            colors[1] = effColorBlend(innerEnd, innerStart, t) | alphaMask;
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

void dds3BezierTrackPolySetVC(EffPcpThunderFragmentResources *history, const u128 *source) {
    s32 position = history->nextPointWriteIndex;
    u128 *points = history->points;
    s32 count;

    SDF_VU0_COPYVECTOR(&points[position], source);
    SDF_VU0_COPYVECTOR(&points[position + 1], source + 1);
    SDF_VU0_COPYVECTOR(&points[position + 2], source + 2);
    position += 3;
    count = history->pointCapacity;
    history->nextPointWriteIndex = position;
    if (position == count) {
        SDF_VU0_COPYVECTOR(&points[0], source);
        SDF_VU0_COPYVECTOR(&points[1], source + 1);
        SDF_VU0_COPYVECTOR(&points[2], source + 2);
        history->nextPointWriteIndex = 3;
    }
    if (history->activePointCount < count - 3) {
        history->activePointCount += 3;
    }
}



extern SdfPoolNode *dds3BezierTrackPolyParam[];
#include "sdf_packet_alloc.h"
extern void *effPkt3DPrimMakePoly(EffThunderDrawParams *);

/* Render the two runs of a wrapped three-point history and its end cap. */
void dds3BezierTrackPolyDraw(EffPcpThunderFragmentResources *history) {
    s32 start[4];
    s32 length[4];
    sdfDrawTag_t *list;
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
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    sdf3DPrimLinkResetTm(list, 0);
    sdf3DPrimLinkMat(list, history->resourceHandle, 0);
    recent = history->activePointCount;
    start[0] = history->nextPointWriteIndex - recent;
    if (start[0] < 3) {
        wrapped = start[0] - 3;
        start[1] = 0;
        start[0] = wrapped + history->pointCapacity;
        length[0] = -wrapped;
        length[1] = recent - length[0] + 3;
    } else {
        length[0] = recent;
        length[1] = 0;
    }
    dds3BezierVertexCount.colors = history->colors;
    dds3BezierVertexCount.color = history->color;
    for (i = 0; i < 2; i++) {
        draw = &dds3BezierVertexCount;
        remaining = length[i];
        draw->primitiveCount = 16;
        draw->points = history->points + start[i];
        draw->pointCount = 15;
        while (remaining >= 15) {
            remaining -= 12;
            sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(&dds3BezierVertexCount));
            dds3BezierVertexCount.points += 12;
            dds3BezierVertexCount.colors += 12;
        }
        if (remaining >= 6) {
            draw->primitiveCount = (remaining / 3) * 4 - 4;
            draw->pointCount = remaining;
            sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(draw));
            draw->colors += remaining;
        }
    }
    colors = history->endColors;
    edgeColor = dds3BezierVertexCount.colors[-3];
    centerColor = dds3BezierVertexCount.colors[-2];
    colors[0] = centerColor;
    colors[7] = colors[6] = colors[5] = colors[4] =
        colors[3] = colors[2] = colors[1] = edgeColor;
    dds3BezierTrackPolyVertex.colors = colors;
    dds3BezierTrackPolyVertex.points = history->endPoints;
    dds3BezierTrackPolyVertex.color = history->color;
    sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(&dds3BezierTrackPolyVertex));
    surface = dds3BezierTrackPolyParam[history->surfaceIndex];
    surface->append(surface, list);
}


extern dds3FlashPolyList_t *dds3FlashPolyList2Create(s32 triangleCount);

/* Clone the 0x30-byte parameter block, create the record pool and clear every particle's age. */
PcpFlashTrianglePulseWork *effPCPFlashPolySphere2Create(PcpFlashTrianglePulseParams *params)
{
    sdfMemHandle_t *allocation = sdfMemAllocDirect(params->particleCount * sizeof(PcpFlashPulseParticle) + sizeof(PcpFlashTrianglePulseWork));
    PcpFlashTrianglePulseWork *work = (PcpFlashTrianglePulseWork *)sdfMemLock(allocation);
    dds3FlashPolyList_t *record;
    u32 i;

    memcpy(work, params, 0x30);
    work->parts = (PcpFlashPulseParticle *)(work + 1);
    work->tintColor = 0x80808080;
    work->allocationHandle = allocation;
    work->renderScale = 1.0f;
    work->updateCount = 0;
    record = dds3FlashPolyList2Create(work->particleCount);
    work->resourceHandle = record;
    record->drawMode = work->drawMode;
    for (i = 0; i < work->particleCount; i++) {
        work->parts[i].age = 0;
    }
    return work;
}
