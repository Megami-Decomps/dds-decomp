#include "sdk_ps2.h"
#include "common.h"
#include "eff_matrix4.h"
#include "sdf_vu0.h"
#include "sdf_model_packet_callback.h"
#include "sdf_vu_rotation_axes.h"
#include "eff_pcp_paired_event_row.h"
#include "eff_pcp_spawn_range_event_row.h"
#include "eff_pcp_spawn_range_work.h"
#include "eff_pcp_drift_event_row.h"
#include "eff_packet_params.h"
#include "eff_pcp_fade_timer.h"
#include "btl_effect_position.h"
#include "sdf_trig.h"
#include "eff_random.h"
#include "sdf_model.h"
#include "eff_pcp_event_place.h"
#include "sdf_quaternion.h"
#include "fpu.h"
#include "btl.h"
#include "eff_pcp_beam.h"
#include "sdf_asset_state.h"
#include "sdf_asset_create.h"
#include "sdf_motion.h"
#include "sdf_motion_next_result.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "sdf_packet_append.h"
#include "eff_thunder_vector.h"
#include "bill_object_api.h"
#include "sdf_resource.h"
#include "btl_sound.h"
#include "eff_blur.h"
#include "eff_pcp_long_fade_timer.h"
#include "eff_pcp_shared_trail.h"
#include "eff.h"
#include "eff_resource_rect_api.h"
#include "eff_math.h"
#include "eff_node.h"
#include "eff_node_descriptor.h"
#include "eff_thunder_fragment.h"
#include "eff_pcp_burst_work.h"
#include "eff_param.h"
#include "eff_pcp_spawn_once_work.h"
#include "eff_event.h"
#include "eff_event_sound.h"
#include "eff_pcp_group_set.h"
#include "eff_pcp_rotate_block_inputs.h"
#include "eff_pcp_map_events.h"
#include "eff_pcp_block_set.h"
#include "eff_pcp_rotate_work.h"
#include "eff_pcp_staggered.h"
#include "eff_pcp_delayed_pairs.h"
#include "eff_pcp_vector_copy_api.h"
#include "eff_pcp_cross.h"
#include "eff_pcp_charge.h"
#include "eff_pcp_compact_fade.h"
#include "eff_pcp_compact_work.h"
#include "eff_pcp_event_emitter_state.h"
#include "eff_pcp_staggered_pulse_work.h"
#include "eff_pcp_triple_emitter_work.h"
#include "eff_pcp_twin.h"
#include "eff_pcp_spin.h"
#include "eff_pcp_span.h"
#include "mdl.h"
#include "sdf_chunk.h"
#include "pcp_vu0.h"
#include "sdf_vu_transform_api.h"
#include "sdf_vu_matrix.h"
#include "sdf_vu_matrix_multiply_api.h"
#include "bill_object_api.h"
#include "sdf_projection.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"




extern u8 effPCP_ShockWave01_ThunderSpherePoly_Param[];


extern u8 effPCP_Houden01_ThunderSpherePoly_Param[];





extern u8 effPCP_Megidora01_ThunderSpherePoly_Param[];

extern u8 effPCP_Megidoraon01_ThunderSpherePoly_Param[];


extern s8 D_0043643C;


extern u8 effMuzzleThunderRayParams[];

extern u8 effCyanBlueRayPairParams[];

extern u8 effWhiteYellowRayPairParams[];

extern u8 effYellowOrangeRayPairParams[];

extern u8 effBrownRedRayPairParams[];

extern u8 effOrangeVioletRayPairParams[];

extern EffPCPSharedTrail *pBlinkMultiBlurBase;

/* Every compact timeline precedes one variant-specific SDK parameter payload.
   Respawn copies that payload only, not its live resource/allocation tail. */
typedef dds3Effect_PartialMultiBlurParam_t EffPCPTexturedBlurParams;



typedef struct {
    EffPCPCompactTimelineParams timeline;
    dds3Effect_PartialMultiFilterParam_t res;
} EffPCPCompactRectParams;

typedef struct {
    EffPCPCompactTimelineParams timeline;
    EffPCPTexturedBlurParams res;
} EffPCPCompactTexturedBlurParams;

typedef struct {
    EffPCPCompactTimelineParams timeline;
    dds3Effect_DistortionBlurParam_t res;
} EffPCPCompactScatterParams;

typedef struct {
    EffPCPCompactTimelineParams timeline;
    dds3Effect_RippleBlurParam_t res;
} EffPCPCompactScaleParams;


extern void dds3EffectRippleBlurReset(EffBlurScaleWork *work);
extern void effBlurStepScaleSlotsAndDraw(EffBlurScaleWork *work);

extern u8 effOffsetMuzzleThunderRayParams[];

extern u8 effCyanBlueRaySingleParams[];

extern u8 effWhiteYellowRaySingleParams[];

extern u8 effYellowOrangeRaySingleParams[];

extern u8 effBrownRedRaySingleParams[];

extern u8 effOrangeVioletRaySingleParams[];

struct EffPCPBeamWork;
extern void effPCPRingPolyExpSetVertex(f32 value, struct EffPCPBeamWork *work);


extern void SetRotateMatrixX_B(f32 angle);

extern void effPCPHama02PartInit(EffPCPStaggered *work, s32 index);

extern void effPCPMHanma01PartInit(EffPCPDelayedPairs *work, s32 index);



/* Boss effect work: settable param plus two handles released on free. */
extern void effPCPHama01PartInit(EffPCPTwinWork *work, s32 slotIndex);



EffPCPTripleWork *effPCPAgiraoFireCreate(EffPCPTripleParams *params, EffNodeDescriptor **descriptors);

extern void effPCPBreathSetPosition(void *dst, void *src);






extern EffRandState effDefaultRandomState[];


extern EffRandState effDefaultRandomState[];

extern s32 sdfModelGetMarkPos(sdfModelRootNode_t *model, s32 value);

extern struct SdfPoolNode *kwlnSceneSurfaceSets[13][4];















/* Effect initializers implemented in assembly below (effPCPHamaCardSpiralCreate lives in
   another unit). Each is entered with and without spawn arguments, so they
   are declared unchecked. */
extern EffPCPSpawnRangeWork *effPCPSpiralBEDCreate();

typedef struct EffPCPRingWork {
    f32 pos[4];
    u32 color10;
    u32 color14;
    f32 scale;
    billHandle_t *handle;
} EffPCPRingWork;
void effPCPSparkleSphereDelete(EffPCPRingWork *work) {
    dds3BillboardDelete(work->handle);
    sdfChipFreeDirect(work);
}

extern f32 aViewTarget[4];
extern f32 sdfViewTargetVector[4];
extern void dds3BillboardDraw(billHandle_t *effect);
extern u32 effMagaCylinderCalcColor(u32 flags, u32 color);


/* Places a ring of 10 shrinking, brightening copies of the handle along the
 * fixed view direction. */
void effPCPSparkleSphereDraw(EffPCPRingWork *work) {
    f32 pos[4];
    f32 dir[4];
    f32 size[4];
    billHandle_t *handle;
    f32 scale;
    u32 color;
    s32 i;

    VU0_LOAD_VF($vf10, sdfViewTargetVector);
    VU0_LOAD_VF($vf11, aViewTarget);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    VU0_STORE_VF($vf10, dir);
    handle = work->handle;
    size[0] = work->scale;
    size[1] = work->scale;
    size[2] = work->scale;
    VU0_LOAD_VF($vf10, size);
    VU0_LOAD_VF($vf11, dir);
    SDF_VU0_MULVECTOR();
    VU0_LOAD_VF($vf11, work);
    SDF_VU0_ADDVECTOR();
    VU0_STORE_VF($vf10, pos);
    effCopyVector(handle->position, pos);
    scale = work->scale;
    color = 0x10808080;
    for (i = 0; i < 10; i++) {
        dds3BillboardSetScale(handle, scale, scale);
        scale *= 0.975f;
        dds3BillboardSetRGBA(handle, effMagaCylinderCalcColor(effMagaCylinderCalcColor(color, work->color14), work->color10));
        color += 0x05000000;
        dds3BillboardDraw(handle);
    }
}

void effPCPSparkleSphereSetPosition(f32 destinationPosition[4], const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(destinationPosition, sourcePosition);
}

void effPCPSparkleSphereSetColor(EffPCPRingWork *work, u32 value) {
    work->color14 = value;
}

void effPCPSparkleSphereSetArea(EffPCPRingWork *work, f32 val) {
    work->scale = val;
}

/* Re-rolls slot `index`: random-angle rotation matrix pushed to both handles
 * of the pair, then a new random countdown. */
void effPCPHama01PartInit(EffPCPTwinWork *work, s32 slotIndex) {
    u128 mtx[4];

    SetRotateMatrixY((dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f * 6.283185f);
            VU0_STORE_MATRIX_UNCLOBBERED(mtx);

;
    dds3EffectPartSetRotateMatrix(work->pair[slotIndex][0], mtx);
    dds3EffectPartSetRotateMatrix(work->pair[slotIndex][1], mtx);
    mdlSingleMotion(effParamWorkGetData(work->pair[slotIndex][0]), 0, 0);
    mdlSingleMotion(effParamWorkGetData(work->pair[slotIndex][1]), 0, 0);
    work->counter[slotIndex] = dds3GetRandA((EffRandState *)effDefaultRandomState) % 10;
}

EffPCPTwinWork *effPCPHama01Create(void *src) {
    EffPCPTwinWork *work = sdfChipAllocDirect(0xBC);
    s32 slotIndex;

    for (slotIndex = 0; slotIndex < 8; slotIndex++) {
        if (slotIndex == 0) {
            work->pair[0][0] = dds3EffectPCPDataPartCreate(src, 0);
            work->pair[0][1] = dds3EffectPCPDataPartCreate(src, 2);
        } else if (slotIndex == 1) {
            work->pair[1][0] = dds3EffectPCPDataPartCreate(src, 1);
            work->pair[1][1] = dds3EffectPartCopy(work->pair[0][1]);
        } else {
            work->pair[slotIndex][0] = dds3EffectPartCopy(work->pair[slotIndex & 1][0]);
            work->pair[slotIndex][1] = dds3EffectPartCopy(work->pair[slotIndex & 1][1]);
        }
        effPCPHama01PartInit(work, slotIndex);
    }
    work->shared[0] = dds3EffectPCPDataPartCreate(src, 3);
    work->state[0] = 0;
    for (slotIndex = 1; slotIndex < 8; slotIndex++) {
        work->shared[slotIndex] = dds3EffectPartCopy(work->shared[0]);
        work->state[slotIndex] = 0;
    }
    work->unk00 = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    work->unk04 = 0;
    work->unk08 = 0;
    work->frame = 0;
    return work;
}

void effPCPHama01Delete(EffPCPTwinWork *work) {
    effPartHandle_t **sharedHandleCursor = work->shared;
    effPartHandle_t **pairHandleCursor = &work->pair[0][0];
    s32 pairIndex;

    for (pairIndex = 0; pairIndex < 8; pairIndex++) {
        dds3EffectPartDelete(sharedHandleCursor[0]);
        dds3EffectPartDelete(pairHandleCursor[1]);
        dds3EffectPartDelete(pairHandleCursor[0]);
        sharedHandleCursor++;
        pairHandleCursor += 2;
    }
    sdfChipFreeDirect(work);
}

EffPCPTwinWork *effPCPHama01Copy(EffPCPTwinWork *src) {
    EffPCPTwinWork *work = sdfChipAllocDirect(0xBC);
    s32 slotIndex;

    for (slotIndex = 0; slotIndex < 8; slotIndex++) {
        work->pair[slotIndex][0] = dds3EffectPartCopy(src->pair[slotIndex & 1][0]);
        work->pair[slotIndex][1] = dds3EffectPartCopy(src->pair[slotIndex & 1][1]);
        work->shared[slotIndex] = dds3EffectPartCopy(src->shared[0]);
        work->state[slotIndex] = 0;
        effPCPHama01PartInit(work, slotIndex);
    }
    work->unk00 = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    work->unk04 = 0;
    work->unk08 = 0;
    work->frame = 0;
    return work;
}

/* Per-frame update of the 8 twin slots: a slot whose countdown reached zero
 * fires its handle pair; after frame 0x18 its position is pushed to the shared
 * handle. */
void effPCPHama01Draw(EffPCPTwinWork *work) {
    dds3ModelHandle_t *obj[2];
    u128 pos;
    u128 *posp;
    s32 slotIndex;

    for (slotIndex = 0; slotIndex < 8; slotIndex++) {
        if (work->counter[slotIndex] != 0) {
            work->counter[slotIndex]--;
            continue;
        }
        obj[0] = effParamWorkGetData(work->pair[slotIndex][0]);
        obj[1] = effParamWorkGetData(work->pair[slotIndex][1]);
        SDF_VU0_LOADVECTOR(work);
        mdlSetPosition(obj[0]);
        dds3EffectPartSetScale(work->pair[slotIndex][0], work->scale * 1.5f);
        dds3EffectPartSetScale(work->pair[slotIndex][1], 1.75f);
        mdlSetColor(obj[1], work->color);
        mdlLink(obj[0], kwlnSceneSurfaceSets[10]);
        sdfModelGetMarkPos(obj[0]->inner, 1);
        posp = &pos;
        SDF_VU0_STOREVECTOR_NOMEM(posp);
        mdlSetPosition(obj[1]);
        mdlLink(obj[1], kwlnSceneSurfaceSets[10]);
        if (work->frame > 0x18) {
            dds3EffectPartSetPosition(work->shared[slotIndex], posp);
            dds3EffectPartDraw(work->shared[slotIndex]);
        }
    }
    work->frame++;
}

void effPCPHama01SetPosition(f32 destinationPosition[4], const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(destinationPosition, sourcePosition);
}

void effPCPHama01SetScale(EffPCPTwinWork *work, f32 val) {
    work->scale = val;
}

void effPCPHama01SetColor(EffPCPTwinWork *work, u32 value) {
    work->color = value;
}

/* Re-rolls slot `index` of the staggered effect: random-angle rotation matrix
 * for both handles, new random offset and delay. */
void effPCPHama02PartInit(EffPCPStaggered *work, s32 index) {
    u128 mtx[4];

    SetRotateMatrixY((dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f * 6.283185f);
            VU0_STORE_MATRIX_UNCLOBBERED(mtx);

;
    dds3EffectPartSetRotateMatrix(work->handle[index * 2], mtx);
    dds3EffectPartSetRotateMatrix(work->handle[index * 2 + 1], mtx);
    mdlSingleMotion(effParamWorkGetData(work->handle[index * 2]), 0, 0);
    mdlSingleMotion(effParamWorkGetData(work->handle[index * 2 + 1]), 0, 0);
    work->verticalOffset[index] = dds3GetRandFloatA(effDefaultRandomState) * 150.0f;
    work->remainingDelay[index] = dds3GetRandA((EffRandState *)effDefaultRandomState) % 10;
}


EffPCPStaggered *effPCPHama02Create(void *args) {
    EffPCPStaggered *work = sdfChipAllocDirect(0x98);
    effPartHandle_t **handle = &work->handle[1];
    s32 i = 0;

    do {
        if (i == 0) {
            work->handle[0] = dds3EffectPCPDataPartCreate(args, 0);
            work->handle[1] = dds3EffectPCPDataPartCreate(args, 1);
        } else {
            handle[-1] = dds3EffectPartCopy(work->handle[0]);
            handle[0] = dds3EffectPartCopy(work->handle[1]);
        }
        handle += 2;
        effPCPHama02PartInit(work, i++);
    } while (i < 8);
    work->color = 0x80808080;
    work->scale = 1.0f;
    work->position[0] = 0;
    work->position[1] = 0;
    work->position[2] = 0;
    return work;
}

void effPCPHama02Delete(EffPCPStaggered *work) {
    effPartHandle_t **p = &work->handle[1];
    s32 i;

    for (i = 0; i < 8; i++) {
        dds3EffectPartDelete(p[-1]);
        dds3EffectPartDelete(p[0]);
        p += 2;
    }
    sdfChipFreeDirect(work);
}

EffPCPStaggered *effPCPHama02Copy(EffPCPStaggered *source) {
    EffPCPStaggered *work = sdfChipAllocDirect(0x98);
    effPartHandle_t **handle = &work->handle[1];
    s32 i = 0;

    do {
        handle[-1] = dds3EffectPartCopy(source->handle[0]);
        handle[0] = dds3EffectPartCopy(source->handle[1]);
        handle += 2;
        effPCPHama02PartInit(work, i);
        i++;
    } while (i < 8);
    work->color = 0x80808080;
    work->scale = 1.0f;
    work->position[0] = 0;
    work->position[1] = 0;
    work->position[2] = 0;
    return work;
}

void effPCPHama02Draw(EffPCPStaggered *work) {
    dds3ModelHandle_t *obj[2];
    f32 pos[4];
    s32 i;

    for (i = 0; i < 8; i++) {
        if (work->remainingDelay[i] != 0) {
            work->remainingDelay[i]--;
        } else {
            obj[0] = effParamWorkGetData(work->handle[i * 2]);
            obj[1] = effParamWorkGetData(work->handle[i * 2 + 1]);
            pos[0] = work->position[0];
            pos[2] = work->position[2];
            pos[1] = (work->position[1] - work->verticalOffset[i] + 100.0f) * work->scale;
                        SDF_VU0_LOADVECTOR(pos);
            mdlSetPosition(obj[0]);
            dds3EffectPartSetScale(work->handle[i * 2], work->scale * 1.5f);
            dds3EffectPartSetScale(work->handle[i * 2 + 1], 1.5f);
            mdlSetColor(obj[1], work->color);
            mdlLink(obj[0], kwlnSceneSurfaceSets[10]);
            sdfModelGetMarkPos(obj[0]->inner, 1);
            mdlSetPosition(obj[1]);
            mdlLink(obj[1], kwlnSceneSurfaceSets[10]);
        }
    }
}

void effPCPHama02SetPosition(f32 destinationPosition[4], const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(destinationPosition, sourcePosition);
}

void effPCPHama02SetScale(EffPCPStaggered *work, f32 val) {
    work->scale = val;
}

void effPCPHama02SetColor(EffPCPStaggered *work, u32 value) {
    work->color = value;
}

/* Spawns cross arm (i, j): rotates the model by j sixths of a turn about the axis chosen by i. */
void effPCPHamaonPartInit(EffPCPCrossWork *work, u32 i, u32 j) {
    u128 mtx[4];
    f32 angle;

    mdlSingleMotion(effParamWorkGetData(work->handle[i][j]), 0, 0);
    angle = (f32)j * -1.0471974f;
    if (i == 0) {
        SetRotateMatrixY(angle);
    } else if (i == 1) {
        SetRotateMatrixY(-angle);
    } else if (i == 2) {
        SetRotateMatrixZ(-angle);
    } else {
        SetRotateMatrixZ(angle);
    }
        VU0_STORE_MATRIX(mtx);
;
    dds3EffectPartSetRotateMatrix(work->handle[i][j], mtx);
    if ((j + 1) & 1) {
        work->remainingDelay[i][j] = 0;
    } else {
        work->remainingDelay[i][j] = 5;
    }
}

EffPCPCrossWork *effPCPHamaonCreate(void *src) {
    EffPCPCrossWork *work = sdfChipAllocDirect(0xDC);
    s32 i;
    s32 j;

    work->base = dds3EffectPCPDataPartCreate(src, 0);
    mdlSingleMotion(effParamWorkGetData(work->base), 0, 0);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            if (j == 0) {
                work->handle[i][j] = dds3EffectPCPDataPartCreate(src, i + 1);
            } else {
                work->handle[i][j] = dds3EffectPartCopy(work->handle[i][0]);
            }
            effPCPHamaonPartInit(work, i, j);
        }
    }
    work->unk00 = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    work->unk04 = 0;
    work->unk08 = 0;
    return work;
}

/* Destroys the cross effect: releases the main handle and all 12 group handles. */
void effPCPHamaonDelete(EffPCPCrossWork *work) {
    s32 i;
    s32 j;

    dds3EffectPartDelete(work->base);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            dds3EffectPartDelete(work->handle[i][j]);
        }
    }
    sdfChipFreeDirect(work);
}

EffPCPCrossWork *effPCPHamaonCopy(EffPCPCrossWork *src) {
    EffPCPCrossWork *work = sdfChipAllocDirect(0xDC);
    s32 i;
    s32 j;

    work->base = dds3EffectPartCopy(src->base);
    mdlSingleMotion(effParamWorkGetData(work->base), 0, 0);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            work->handle[i][j] = dds3EffectPartCopy(src->handle[i][0]);
            effPCPHamaonPartInit(work, i, j);
        }
    }
    work->unk00 = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    work->unk04 = 0;
    work->unk08 = 0;
    return work;
}

/* Per-frame update of the cross effect: scales the anchor model and fires each
 * of the 12 slots whose countdown has expired. */
void effPCPHamaonDraw(EffPCPCrossWork *work) {
    dds3ModelHandle_t *anchor;
    dds3ModelHandle_t *obj;
    s32 i;
    s32 j;

    anchor = effParamWorkGetData(work->base);
    SDF_VU0_LOADVECTOR(work);
    mdlSetPosition(anchor);
    dds3EffectPartSetScale(work->base, work->scale);
    mdlLink(anchor, kwlnSceneSurfaceSets[10]);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            if (work->remainingDelay[i][j] != 0) {
                work->remainingDelay[i][j]--;
                continue;
            }
            obj = effParamWorkGetData(work->handle[i][j]);
            mdlSetColor(obj, work->color);
            sdfModelGetMarkPos(anchor->inner, i * 4 + j + 1);
            mdlSetPosition(obj);
            mdlLink(obj, kwlnSceneSurfaceSets[10]);
        }
    }
}

void effPCPHamaonSetPosition(f32 destinationPosition[4], const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(destinationPosition, sourcePosition);
}

void effPCPHamaonSetScale(EffPCPCrossWork *work, f32 val) {
    work->scale = val;
}

void effPCPHamaonSetColor(EffPCPCrossWork *work, u32 value) {
    work->color = value;
}


/* Re-arms slot `index`: registers both handles of the pair and rolls a new
 * random countdown. */
void effPCPMHanma01PartInit(EffPCPDelayedPairs *work, s32 index) {

    mdlSingleMotion(effParamWorkGetData(work->handle[index * 2]), 0, 0);
    mdlSingleMotion(effParamWorkGetData(work->handle[index * 2 + 1]), 0, index & 1);
    work->remainingDelay[index] = dds3GetRandA((EffRandState *)effDefaultRandomState) % 10;
}


EffPCPDelayedPairs *effPCPMHanma01Create(void *source) {
    EffPCPDelayedPairs *work = sdfChipAllocDirect(0x60);
    effPartHandle_t **handle = &work->handle[1];
    s32 i;

    for (i = 0; i < 6; i++) {
        if (i == 0) {
            work->handle[1] = dds3EffectPCPDataPartCreate(source, 6);
        }
        handle[-1] = dds3EffectPCPDataPartCreate(source, i);
        handle[0] = dds3EffectPartCopy(work->handle[1]);
        handle += 2;
        effPCPMHanma01PartInit(work, i);
    }
    work->color = 0x80808080;
    work->scale = 1.0f;
    work->unk00 = 0;
    work->unk04 = 0;
    work->unk08 = 0;
    return work;
}

void effPCPMHanma01Delete(EffPCPDelayedPairs *work) {
    effPartHandle_t **p = &work->handle[1];
    s32 i;

    for (i = 0; i < 6; i++) {
        dds3EffectPartDelete(p[-1]);
        dds3EffectPartDelete(p[0]);
        p += 2;
    }
    sdfChipFreeDirect(work);
}

EffPCPDelayedPairs *effPCPMHanma01Copy(EffPCPDelayedPairs *source) {
    effPartHandle_t **sourceHandle;
    effPartHandle_t **workHandle;
    s32 i;
    EffPCPDelayedPairs *work = sdfChipAllocDirect(0x60);
    sourceHandle = &source->handle[1];
    workHandle = &work->handle[1];

    for (i = 0; i < 6; i++) {
        workHandle[-1] = dds3EffectPartCopy(sourceHandle[-1]);
        workHandle[0] = dds3EffectPartCopy(sourceHandle[0]);
        sourceHandle += 2;
        workHandle += 2;
        effPCPMHanma01PartInit(work, i);
    }
    work->color = 0x80808080;
    work->scale = 1.0f;
    work->unk00 = 0;
    work->unk04 = 0;
    work->unk08 = 0;
    return work;
}

void effPCPMHanma01Draw(EffPCPDelayedPairs *work) {
    dds3ModelHandle_t *obj[2];
    u128 vec;
    s32 i;

    for (i = 0; i < 6; i++) {
        if (work->remainingDelay[i] != 0) {
            work->remainingDelay[i]--;
        } else {
            obj[0] = effParamWorkGetData(work->handle[i * 2]);
            obj[1] = effParamWorkGetData(work->handle[i * 2 + 1]);
                        SDF_VU0_LOADVECTOR_BARRIER(work);
            mdlSetPosition(obj[0]);
            mdlSetColor(obj[1], work->color);
            mdlLink(obj[0], kwlnSceneSurfaceSets[10]);
            sdfModelGetMarkPos(obj[0]->inner, 1);
                        SDF_VU0_STOREVECTOR_NOMEM(&vec);
            mdlSetPosition(obj[1]);
            mdlLink(obj[1], kwlnSceneSurfaceSets[10]);
        }
    }
}

void effPCPMHanma01SetPosition(f32 destinationPosition[4], const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(destinationPosition, sourcePosition);
}

void effPCPMHanma01SetScale(EffPCPDelayedPairs *work, f32 val) {
    work->scale = val;
}

void effPCPMHanma01SetColor(EffPCPDelayedPairs *work, u32 value) {
    work->color = value;
}

void effPCPMudoPartInit(EffPCPChargeWork *work) {
    work->historyCount = 0;
    work->updateCount = 0;
    work->baseColor = 0x80808080;
}

EffPCPChargeWork *effPCPMudoCreate(void *source) {
    sdfMemHandle_t *resource;
    EffPCPChargeWork *work;

    resource = sdfMemAllocDirect(sizeof(*work));
    work = (void *)sdfMemLock(resource);
    work->allocationHandle = resource;
    work->primaryHandle = dds3EffectPCPDataPartCreate(source, 0);
    work->secondaryHandle = dds3EffectPCPDataPartCreate(source, 1);
    effPCPMudoPartInit(work);
    work->vectorWords[0] = 0;
    work->vectorWords[1] = 0;
    work->vectorWords[2] = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    return work;
}

void effPCPMudoDelete(EffPCPChargeWork *work) {
    dds3EffectPartDelete(work->primaryHandle);
    dds3EffectPartDelete(work->secondaryHandle);
    sdfMemFreeDirect(work->allocationHandle);
}

/* Duplicate the parameter owners into fresh history storage; sampled arrays
 * are not copied from source. */
EffPCPChargeWork *effPCPMudoCopy(EffPCPChargeWork *source) {
    sdfMemHandle_t *resource;
    EffPCPChargeWork *work;
    effPartHandle_t *firstHandle;

    resource = sdfMemAllocDirect(sizeof(*work));
    work = (void *)sdfMemLock(resource);
    firstHandle = source->primaryHandle;
    work->allocationHandle = resource;
    work->primaryHandle = dds3EffectPartCopy(firstHandle);
    work->secondaryHandle = dds3EffectPartCopy(source->secondaryHandle);
    effPCPMudoPartInit(work);
    work->vectorWords[0] = 0;
    work->vectorWords[1] = 0;
    work->vectorWords[2] = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    return work;
}

extern u16 effChargeSamplePeriods[8];
extern f32 effChargePointScaleFactors[8];

/* vu0 routine: capture staggered model points, then draw their growing history. */
void effPCPMudoDraw(EffPCPChargeWork *work) {
    f32 position[4] __attribute__((aligned(16)));
    u32 i;
    u32 color;
    billHandle_t *bill;
    dds3ModelHandle_t *model;
    f32 scale;
    u32 count;
    u32 captureRow;
    u32 point;

    if (work->updateCount > 50) {
        if ((work->baseColor & 0xFF000000) >= 0x08000000) {
            work->baseColor -= 0x08000000;
        }
    }
    work->updateCount++;
    color = effMagaCylinderCalcColor(work->color, work->baseColor);
    bill = effParamWorkGetData(work->primaryHandle);
    model = effParamWorkGetData(work->secondaryHandle);
    scale = work->scale * 1.5f;
    dds3EffectPartSetScale(work->secondaryHandle, scale);
    dds3EffectPartSetPosition(work->secondaryHandle, work->vectorWords);
    mdlLink(model, kwlnSceneSurfaceSets[10]);
    captureRow = work->historyCount;
    if (captureRow < 25) {
        for (point = 0; point < 7; point++) {
            if (captureRow % effChargeSamplePeriods[point] == 0) {
                sdfModelGetMarkPos(model->inner, point + 1);
                SDF_VU0_STOREVECTOR(position);
                SDF_VU0_COPYVECTOR(work->samplePositions[captureRow][point], position);
                work->animationFrames[captureRow][point] = dds3GetRandA((EffRandState *)effDefaultRandomState);
                work->sampleScales[captureRow][point] = 0;
                work->sampleAges[captureRow][point] = 0;
            }
        }
        work->historyCount++;
    }
    count = work->historyCount;
    dds3BillboardSetRGBA(bill, color);
    for (i = 0; i < count; i++) {
        for (point = 0; point < 7; point++) {
            if (i % effChargeSamplePeriods[point] == 0) {
                f32 growth = (f32)work->sampleAges[i][point] / 12.0f;
                f32 size;

                if (growth > 1.0f) {
                    growth = 1.0f;
                }
                size = effChargePointScaleFactors[point] * growth * scale;
                work->sampleAges[i][point]++;
                work->sampleScales[i][point] = size;
                dds3BillboardSetScale(bill, size, size);
                effCopyVector(bill->position, work->samplePositions[i][point]);
                dds3BillboardDraw(bill);
                dds3BillboardSetAnimKey(bill, work->animationFrames[i][point]);
                work->animationFrames[i][point]++;
            }
        }
    }
}

void effPCPMudoSetPosition(EffPCPChargeWork *work, const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(work->vectorWords, sourcePosition);
}

void effPCPMudoSetScale(EffPCPChargeWork *work, f32 val) {
    work->scale = val;
}

void effPCPMudoSetColor(EffPCPChargeWork *work, u32 value) {
    work->color = value;
}

EffPCPSpawnOnceWork *effPCPHwave01Create(void *src) {
    EffPCPSpawnOnceWork *dst;
    effPartHandle_t *handle;

    dst = sdfChipAllocDirect(0x1C);
    dst->primaryHandle = dds3EffectPCPDataPartCreate(src, 0);
    handle = dds3EffectPCPDataPartCreate(src, 1);
    dst->vector[0] = 0;
    dst->secondaryHandle = handle;
    dst->vector[1] = 0;
    dst->color = 0x80808080;
    dst->vector[2] = 0;
    return dst;
}

void effPCPHwave01Delete(EffPCPSpawnOnceWork *work) {
    dds3EffectPartDelete(work->primaryHandle);
    dds3EffectPartDelete(work->secondaryHandle);
    sdfChipFreeDirect(work);
}

EffPCPSpawnOnceWork *effPCPHwave01Copy(EffPCPSpawnOnceWork *src) {
    EffPCPSpawnOnceWork *dst;
    effPartHandle_t *handle;

    dst = sdfChipAllocDirect(0x1C);
    dst->primaryHandle = dds3EffectPartCopy(src->primaryHandle);
    handle = dds3EffectPartCopy(src->secondaryHandle);
    dst->vector[0] = 0;
    dst->secondaryHandle = handle;
    dst->vector[1] = 0;
    dst->color = 0x80808080;
    dst->vector[2] = 0;
    return dst;
}

void effPCPHwave01Draw(EffPCPSpawnOnceWork *work) {
    dds3ModelHandle_t *obj;
    u128 vec;

    obj = effParamWorkGetData(work->primaryHandle);
        SDF_VU0_LOADVECTOR_BARRIER(work->vector);
    mdlSetPosition(obj);
    dds3EffectPartSetColor(work->secondaryHandle, work->color);
    mdlLink(obj, kwlnSceneSurfaceSets[10]);
    sdfModelGetMarkPos(obj->inner, 1);
        VU0_STORE_VF_TO_MEMORY(vf10, vec);
    dds3EffectPartSetPosition(work->secondaryHandle, &vec);
    dds3EffectPartDraw(work->secondaryHandle);
}

void effPCPHwave01SetPosition(f32 destinationPosition[4], const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(destinationPosition, sourcePosition);
}

void effPCPHwave01SetColor(EffPCPSpawnOnceWork *work, u32 value) {
    work->color = value;
}


/* The 8-, 12- and 30-particle variants allocate a common prefix followed
   by their actual handle count. Spawn, shift and release use the same tail. */
typedef struct EffPCPThunderGroup {
    f32 pos[4];
    u32 unk10;
    f32 unk14; /* Settable, but not read by the observed group updates. */
    u32 unk18;
    EffThunderFragmentWork *handles[1];
} EffPCPThunderGroup;

#define EFF_DEG2RAD 0.017453292f

extern EffThunderFragmentParams effSharedSpawnParams[];
extern u8 D_003B1680[];

/* Spawns 12 particles in a ring: every second particle advances the ring angle
 * (60 degrees). Direction is normalised on the VU, scaled per axis and offset
 * by the origin vector; short-reach particles get a shorter life. */
void effPCPZiodin01Init(EffPCPThunderGroup *group) {
    s32 i;
    EffThunderFragmentWork **out;
    f32 angle = 0.0f;
    f32 cosv = 0.0f;
    f32 sinv = 0.0f;
    f32 dir[4];
    f32 scale[4];
    f32 spread;
    f32 radius;
    f32 reach;
    f32 speed;
    s16 life;

    i = 0;
    out = group->handles;
    do {
        if ((i & 1) == 0) {
            sinv = sdfCos(angle);
            cosv = sdfSin(angle);
            angle += 60.0f * EFF_DEG2RAD;
        }
        spread = dds3GetRandFloatA(effDefaultRandomState) * 0.5f + 0.5f;
        effSharedSpawnParams->bandWidth = spread * 1.25f;
        effSharedSpawnParams->edgeWidth = spread * 12.5f;
        radius = (dds3GetRandFloatA(effDefaultRandomState) * 0.25f + 0.75f) * 100.0f;
        effSharedSpawnParams->end[1] = 0;
        effSharedSpawnParams->end[0] = sinv * radius;
        effSharedSpawnParams->end[2] = cosv * radius;
        dir[0] = sinv;
        dir[1] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f + 2.0f;
        dir[2] = cosv;
        VU0_LOAD_VF($vf10, dir);
        SDF_VU0_NORMALIZE();
        VU0_STORE_VF($vf10, dir);
        reach = (dds3GetRandFloatA(effDefaultRandomState) * 0.65f + (1.0f - 0.65f)) * 350.0f;
        scale[2] = reach;
        scale[0] = reach;
        scale[1] = -reach;
        VU0_LOAD_VF($vf10, dir);
        VU0_LOAD_VF($vf11, scale);
        SDF_VU0_MULVECTOR();
        VU0_LOAD_VF($vf11, D_003B1680);
        SDF_VU0_ADDVECTOR();
        VU0_STORE_VF($vf10, effSharedSpawnParams);
        if (scale[0] < 250.0f) {
            speed = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f * 2.5f + 5.0f;
            life = dds3GetRandA((EffRandState *)effDefaultRandomState) % 5 + 5;
        } else {
            speed = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f * 5.0f + 10.0f;
            life = dds3GetRandA((EffRandState *)effDefaultRandomState) % 6 + 15;
        }
        effSharedSpawnParams->halfLife = life;
        effSharedSpawnParams->waveAmplitude = speed;
        *out = effPCP_ThunderPolyCreate(effSharedSpawnParams);
        out++;
        i++;
    } while (i < 12);
}

void *effPCPZiodin01Create(void) {
    void *work;

    work = sdfChipAllocDirect(0x4c);
    effPCPZiodin01Init(work);
    return work;
}

typedef struct MenuPanelChildren1C {
    u8 pad00[0x1C];
    void *children[30];
} MenuPanelChildren1C;

void effPCPZiodin01Delete(EffPCPThunderGroup *group) {
    s32 i;
    for (i = 0; i < 12; i++) {
        effPCP_ThunderPolyDelete(group->handles[i]);
    }
    sdfChipFreeDirect(group);
}

void *effPCPZiodin01Copy(void) {
    void *work;

    work = sdfChipAllocDirect(0x4c);
    effPCPZiodin01Init(work);
    return work;
}


/* Shifts the first `count` handles' objects by the work position for the
 * duration of their update, then puts the original vectors back. */
static inline void effPcpShiftThunderHandles(EffPCPThunderGroup *work, s32 count) {
    u128 saved[2];
    EffThunderFragmentParams *obj;
    s32 i;

    for (i = 0; i < count; i++) {
        obj = effPCP_ThunderPolyGetParam(work->handles[i]);
        VU0_LOAD_VF($vf10, obj->end);
        VU0_STORE_VF($vf10, &saved[1]);
        VU0_LOAD_VF($vf11, work);
        SDF_VU0_ADDVECTOR();
        VU0_STORE_VF($vf10, obj->end);
        VU0_LOAD_VF($vf10, obj->start);
        VU0_STORE_VF($vf10, &saved[0]);
        SDF_VU0_ADDVECTOR();
        VU0_STORE_VF($vf10, obj->start);
        effPCP_ThunderPolyDraw(work->handles[i]);
        SDF_VU0_COPYVECTOR(obj->start, &saved[0]);
        SDF_VU0_COPYVECTOR(obj->end, &saved[1]);
    }
}

void effPCPZiodin01Draw(EffPCPThunderGroup *work) {
    effPcpShiftThunderHandles(work, 12);
}

void effPCPZiodin01SetPosition(f32 destinationPosition[4], const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(destinationPosition, sourcePosition);
}

void effPCPZiodin01SetScale(EffPCPThunderGroup *work, f32 val) {
    work->unk14 = val;
}

void effPCPZiodin01SetColor(EffPCPThunderGroup *work, u32 value) {
    work->unk18 = value;
}

extern EffThunderFragmentParams effPCPZiodin02Param[];

/* 12-piece spread on a cone around a random axis (rotation matrix built by
 * SetRotateMatrixAxis into the VU0 matrix registers). */
void effPCPZiodin02Init(EffPCPThunderGroup *group) {
    s32 i;
    f32 angle = 0.0f;
    f32 sinv = 0.0f;
    f32 cosv = 0.0f;
    f32 axis[4];
    f32 spread;
    f32 radius;
    f32 height;
    f32 speed;
    s32 life;

    i = 0;
    do {
        if ((i & 1) == 0) {
            sinv = sdfCos(angle);
            cosv = sdfSin(angle);
            angle += 60.0f * EFF_DEG2RAD;
        }
        spread = dds3GetRandFloatA(effDefaultRandomState) * 0.5f + 0.5f;
        effPCPZiodin02Param->bandWidth = spread * 1.25f;
        effPCPZiodin02Param->edgeWidth = spread * 12.5f;
        radius = (dds3GetRandFloatA(effDefaultRandomState) * 0.25f + 0.75f) * 100.0f;
        axis[0] = cosv;
        axis[1] = 0;
        axis[2] = -sinv;
        effPCPZiodin02Param->end[1] = 0;
        effPCPZiodin02Param->end[0] = sinv * radius;
        effPCPZiodin02Param->end[2] = cosv * radius;
        SetRotateMatrixAxis(-(dds3GetRandFloatA(effDefaultRandomState) * (50.0f * EFF_DEG2RAD) + 5.0f * EFF_DEG2RAD), (const struct RwV3d *)axis);
        height = (dds3GetRandFloatA(effDefaultRandomState) * 0.65f + (1.0f - 0.65f)) * 500.0f;
        effPCPZiodin02Param->start[0] = 0;
        effPCPZiodin02Param->start[2] = 0;
        effPCPZiodin02Param->start[1] = -height;
        VU0_LOAD_VF($vf10, effPCPZiodin02Param->start);
        SDF_VU0_APPLYMATRIX_XYZ();
        VU0_STORE_VF($vf10, effPCPZiodin02Param->start);
        effPCPZiodin02Param->start[0] += effPCPZiodin02Param->end[0];
        effPCPZiodin02Param->start[2] += effPCPZiodin02Param->end[2];
        if (height < 300.0f) {
            speed = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f * 2.5f + 5.0f;
            life = dds3GetRandA((EffRandState *)effDefaultRandomState) % 5 + 5;
        } else {
            speed = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f * 5.0f + 10.0f;
            life = dds3GetRandA((EffRandState *)effDefaultRandomState) % 6 + 15;
        }
        effPCPZiodin02Param->halfLife = life;
        effPCPZiodin02Param->waveAmplitude = speed;
        group->handles[i] = effPCP_ThunderPolyCreate(effPCPZiodin02Param);
        i++;
    } while (i < 12);
}

void *effPCPZiodin02Create(void) {
    void *work;

    work = sdfChipAllocDirect(0x4c);
    effPCPZiodin02Init(work);
    return work;
}

void effPCPZiodin02Delete(EffPCPThunderGroup *group) {
    s32 i;
    for (i = 0; i < 12; i++) {
        effPCP_ThunderPolyDelete(group->handles[i]);
    }
    sdfChipFreeDirect(group);
}

void *effPCPZiodin02Copy(void) {
    void *work;

    work = sdfChipAllocDirect(0x4c);
    effPCPZiodin02Init(work);
    return work;
}

void effPCPZiodin02Draw(EffPCPThunderGroup *work) {
    effPcpShiftThunderHandles(work, 12);
}

void effPCPZiodin02SetPosition(f32 destinationPosition[4], const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(destinationPosition, sourcePosition);
}

void effPCPZiodin02SetScale(EffPCPThunderGroup *work, f32 val) {
    work->unk14 = val;
}

void effPCPZiodin02SetColor(EffPCPThunderGroup *work, u32 value) {
    work->unk18 = value;
}

extern EffThunderFragmentParams effPCPZiodin03Param[];

/* 30-piece spread: pieces are placed on a cone around a random axis (rotation
 * matrix built by SetRotateMatrixAxis into the VU0 matrix registers). */
void effPCPZiodin03Init(EffPCPThunderGroup *group) {
    s32 i;
    f32 angle = 0.0f;
    f32 sinv = 0.0f;
    f32 cosv = 0.0f;
    f32 axis[4];
    f32 spread;
    f32 height;
    f32 speed;
    s16 life;

    i = 0;
    do {
        if ((i & 1) == 0) {
            sinv = sdfCos(angle);
            cosv = sdfSin(angle);
            angle += 24.0f * EFF_DEG2RAD;
        }
        spread = dds3GetRandFloatA(effDefaultRandomState) * 0.5f + 0.5f;
        effPCPZiodin03Param->bandWidth = spread * 1.25f;
        effPCPZiodin03Param->edgeWidth = spread * 12.5f;
        effPCPZiodin03Param->end[1] = 0;
        effPCPZiodin03Param->end[0] = sinv * ((dds3GetRandFloatA(effDefaultRandomState) * 0.25f + 0.75f) * 500.0f);
        effPCPZiodin03Param->end[2] = cosv * ((dds3GetRandFloatA(effDefaultRandomState) * 0.25f + 0.75f) * 250.0f);
        axis[0] = cosv;
        axis[1] = 0;
        axis[2] = -sinv;
        SetRotateMatrixAxis(-(dds3GetRandFloatA(effDefaultRandomState) * (55.0f * EFF_DEG2RAD) + 5.0f * EFF_DEG2RAD), (const struct RwV3d *)axis);
        height = (dds3GetRandFloatA(effDefaultRandomState) * 0.65f + (1.0f - 0.65f)) * 500.0f;
        effPCPZiodin03Param->start[0] = 0;
        effPCPZiodin03Param->start[2] = 0;
        effPCPZiodin03Param->start[1] = -height;
        VU0_LOAD_VF($vf10, effPCPZiodin03Param->start);
        SDF_VU0_APPLYMATRIX_XYZ();
        VU0_STORE_VF($vf10, effPCPZiodin03Param->start);
        effPCPZiodin03Param->start[0] += effPCPZiodin03Param->end[0];
        effPCPZiodin03Param->start[2] += effPCPZiodin03Param->end[2];
        if (height < 300.0f) {
            speed = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f * 2.5f + 5.0f;
            life = dds3GetRandA((EffRandState *)effDefaultRandomState) % 5 + 5;
        } else {
            speed = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f * 5.0f + 10.0f;
            life = dds3GetRandA((EffRandState *)effDefaultRandomState) % 6 + 15;
        }
        effPCPZiodin03Param->halfLife = life;
        effPCPZiodin03Param->waveAmplitude = speed;
        group->handles[i] = effPCP_ThunderPolyCreate(effPCPZiodin03Param);
        i++;
    } while (i < 30);
}

void *effPCPZiodin03Create(void) {
    void *work;

    work = sdfChipAllocDirect(0x94);
    effPCPZiodin03Init(work);
    return work;
}

void effPCPZiodin03Delete(EffPCPThunderGroup *group) {
    s32 i;
    for (i = 0; i < 30; i++) {
        effPCP_ThunderPolyDelete(group->handles[i]);
    }
    sdfChipFreeDirect(group);
}

void *effPCPZiodin03Copy(void) {
    void *work;

    work = sdfChipAllocDirect(0x94);
    effPCPZiodin03Init(work);
    return work;
}

void effPCPZiodin03Draw(EffPCPThunderGroup *work) {
    effPcpShiftThunderHandles(work, 30);
}

void effPCPZiodin03SetPosition(f32 destinationPosition[4], const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(destinationPosition, sourcePosition);
}

void effPCPZiodin03SetScale(EffPCPThunderGroup *work, f32 val) {
    work->unk14 = val;
}

void effPCPZiodin03SetColor(EffPCPThunderGroup *work, u32 value) {
    work->unk18 = value;
}

extern EffThunderFragmentParams effPCPZiodin04Param[];

/* Same spread as effPCPZiodin02Init with a wider cone and its own parameter block. */
void effPCPZiodin04Init(EffPCPThunderGroup *group) {
    s32 i;
    f32 angle = 0.0f;
    f32 sinv = 0.0f;
    f32 cosv = 0.0f;
    f32 axis[4];
    f32 spread;
    f32 radius;
    f32 height;
    f32 speed;
    s32 life;

    i = 0;
    do {
        if ((i & 1) == 0) {
            sinv = sdfCos(angle);
            cosv = sdfSin(angle);
            angle += 60.0f * EFF_DEG2RAD;
        }
        spread = dds3GetRandFloatA(effDefaultRandomState) * 0.5f + 0.5f;
        effPCPZiodin04Param->bandWidth = spread * 1.25f;
        effPCPZiodin04Param->edgeWidth = spread * 12.5f;
        radius = (dds3GetRandFloatA(effDefaultRandomState) * 0.25f + 0.75f) * 100.0f;
        axis[0] = cosv;
        axis[1] = 0;
        axis[2] = -sinv;
        effPCPZiodin04Param->end[1] = 0;
        effPCPZiodin04Param->end[0] = sinv * radius;
        effPCPZiodin04Param->end[2] = cosv * radius;
        SetRotateMatrixAxis(-(dds3GetRandFloatA(effDefaultRandomState) * (55.0f * EFF_DEG2RAD) + 5.0f * EFF_DEG2RAD), (const struct RwV3d *)axis);
        height = (dds3GetRandFloatA(effDefaultRandomState) * 0.65f + (1.0f - 0.65f)) * 500.0f;
        effPCPZiodin04Param->start[0] = 0;
        effPCPZiodin04Param->start[2] = 0;
        effPCPZiodin04Param->start[1] = -height;
        VU0_LOAD_VF($vf10, effPCPZiodin04Param->start);
        SDF_VU0_APPLYMATRIX_XYZ();
        VU0_STORE_VF($vf10, effPCPZiodin04Param->start);
        effPCPZiodin04Param->start[0] += effPCPZiodin04Param->end[0];
        effPCPZiodin04Param->start[2] += effPCPZiodin04Param->end[2];
        if (height < 300.0f) {
            speed = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f * 2.5f + 5.0f;
            life = dds3GetRandA((EffRandState *)effDefaultRandomState) % 5 + 5;
        } else {
            speed = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f * 5.0f + 10.0f;
            life = dds3GetRandA((EffRandState *)effDefaultRandomState) % 6 + 15;
        }
        effPCPZiodin04Param->halfLife = life;
        effPCPZiodin04Param->waveAmplitude = speed;
        group->handles[i] = effPCP_ThunderPolyCreate(effPCPZiodin04Param);
        i++;
    } while (i < 12);
}

void *effPCPZiodin04Create(void) {
    void *work;

    work = sdfChipAllocDirect(0x4c);
    effPCPZiodin04Init(work);
    return work;
}

void effPCPZiodin04Delete(EffPCPThunderGroup *group) {
    s32 i;
    for (i = 0; i < 12; i++) {
        effPCP_ThunderPolyDelete(group->handles[i]);
    }
    sdfChipFreeDirect(group);
}

void *effPCPZiodin04Copy(void) {
    void *work;

    work = sdfChipAllocDirect(0x4c);
    effPCPZiodin04Init(work);
    return work;
}

void effPCPZiodin04Draw(EffPCPThunderGroup *work) {
    effPcpShiftThunderHandles(work, 12);
}

void effPCPZiodin04SetPosition(f32 destinationPosition[4], const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(destinationPosition, sourcePosition);
}

void effPCPZiodin04SetScale(EffPCPThunderGroup *work, f32 val) {
    work->unk14 = val;
}

void effPCPZiodin04SetColor(EffPCPThunderGroup *work, u32 value) {
    work->unk18 = value;
}

extern EffThunderFragmentParams effFixedOriginSpawnParams[];

/* 12 pieces thrown from a fixed origin along a random angle; the param block
 * (of two) is picked at random. */
void effPCPMZiodin01Init(EffPCPThunderGroup *group) {
    s32 i;
    EffThunderFragmentParams *params;
    f32 angle;
    f32 dist;
    s32 life;

    for (i = 0; i < 12; i++) {
        params = &effFixedOriginSpawnParams[dds3GetRandA((EffRandState *)effDefaultRandomState) & 1];
        params->waveAmplitude = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f * 5.0f + 20.0f;
        life = dds3GetRandA((EffRandState *)effDefaultRandomState) % 6 + 15;
        params->start[0] = -150.0f;
        params->start[1] = -500.0f;
        params->start[2] = 0;
        params->halfLife = life;
        angle = dds3GetRandFloatA(effDefaultRandomState) * (3.14159265f / 2.0f) + 3.14159265f / 8.0f;
        dist = (dds3GetRandFloatA(effDefaultRandomState) * 0.25f + 0.75f) * 600.0f;
        params->end[0] = params->start[0] + sdfCos(angle) * dist;
        params->end[1] = params->start[1] + sdfSin(angle) * dist;
        params->end[2] = 0;
        group->handles[i] = effPCP_ThunderPolyCreate(params);
    }
}

void *effPCPMZiodin01Create(void) {
    void *work;

    work = sdfChipAllocDirect(0x4c);
    effPCPMZiodin01Init(work);
    return work;
}

void effPCPMZiodin01Delete(EffPCPThunderGroup *group) {
    s32 i;
    for (i = 0; i < 12; i++) {
        effPCP_ThunderPolyDelete(group->handles[i]);
    }
    sdfChipFreeDirect(group);
}

void *effPCPMZiodin01Copy(void) {
    void *work;

    work = sdfChipAllocDirect(0x4c);
    effPCPMZiodin01Init(work);
    return work;
}

void effPCPMZiodin01Draw(EffPCPThunderGroup *work) {
    effPcpShiftThunderHandles(work, 12);
}

void effPCPMZiodin01SetPosition(f32 destinationPosition[4], const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(destinationPosition, sourcePosition);
}

void effPCPMZiodin01SetScale(EffPCPThunderGroup *work, f32 val) {
    work->unk14 = val;
}

void effPCPMZiodin01SetColor(EffPCPThunderGroup *work, u32 value) {
    work->unk18 = value;
}

extern void SetRotateMatrixZ_B(f32 angle);
extern EffThunderFragmentParams effSideBurstSpawnParams[];

/* 8 pieces flung sideways from below; three rotation-matrix loads compose the
 * VU0 matrix before the particle positions are transformed. */
void effPCPMZiodin02Init(EffPCPThunderGroup *group) {
    s32 i;
    EffThunderFragmentParams *params;

    for (i = 0; i < 8; i++) {
        params = &effSideBurstSpawnParams[dds3GetRandA((EffRandState *)effDefaultRandomState) & 1];
        if (i & 1) {
            params->end[0] = dds3GetRandFloatA(effDefaultRandomState) * 500.0f;
        } else {
            params->end[0] = dds3GetRandFloatA(effDefaultRandomState) * -500.0f;
        }
        params->end[1] = 0;
        params->end[2] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f * 250.0f;
        params->start[0] = 0;
        params->start[1] = -600.0f;
        params->start[2] = 0;
        SetRotateMatrixX((dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f * (30.0f * EFF_DEG2RAD));
        SetRotateMatrixZ_B((dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f * (30.0f * EFF_DEG2RAD));
        MultMatrixRevNoArg();
        VU0_LOAD_VF($vf10, params->start);
        SDF_VU0_APPLYMATRIX_XYZ();
        VU0_STORE_VF($vf10, params->start);
        params->start[0] += params->end[0];
        params->start[2] += params->end[2];
        group->handles[i] = effPCP_ThunderPolyCreate(params);
    }
}

void *effPCPMZiodin02Create(void) {
    void *work;

    work = sdfChipAllocDirect(0x3c);
    effPCPMZiodin02Init(work);
    return work;
}

void effPCPMZiodin02Delete(EffPCPThunderGroup *group) {
    s32 i;
    for (i = 0; i < 8; i++) {
        effPCP_ThunderPolyDelete(group->handles[i]);
    }
    sdfChipFreeDirect(group);
}

void *effPCPMZiodin02Copy(void) {
    void *work;

    work = sdfChipAllocDirect(0x3c);
    effPCPMZiodin02Init(work);
    return work;
}

void effPCPMZiodin02Draw(EffPCPThunderGroup *work) {
    effPcpShiftThunderHandles(work, 8);
}

void effPCPMZiodin02SetPosition(f32 destinationPosition[4], const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(destinationPosition, sourcePosition);
}

void effPCPMZiodin02SetScale(EffPCPThunderGroup *work, f32 val) {
    work->unk14 = val;
}

void effPCPMZiodin02SetColor(EffPCPThunderGroup *work, u32 value) {
    work->unk18 = value;
}

/* All four thunder fade variants allocate this 0x20-byte record.
   Their constructors, color setters, frame updates and release share it. */
typedef struct EffPCPFadeWork {
    u8 pad00[0x10];
    f32 unk10; /* Settable input; not read by the observed fade updates. */
    u32 color;
    u32 frame;
    EffThunderVectorWork *handle; /* 0x1C complete vector work owner */
} EffPCPFadeWork;

void effPCPShockWave01Init(EffPCPFadeWork *work) {
    EffThunderVectorWork *handle;

    handle = effPCPThunderSpherePolyCreate((EffThunderAlphaParams *)effPCP_ShockWave01_ThunderSpherePoly_Param);
    work->frame = 0;
    work->handle = handle;
}

void *effPCPShockWave01Create(void) {
    void *work;

    work = sdfChipAllocDirect(0x20);
    effPCPShockWave01Init(work);
    return work;
}

void effPCPShockWave01Delete(EffPCPFadeWork *work) {
    effPCPThunderSpherePolyDelete(work->handle);
    sdfChipFreeDirect(work);
}

void *effPCPShockWave01Copy(void) {
    void *work;

    work = sdfChipAllocDirect(0x20);
    effPCPShockWave01Init(work);
    return work;
}

extern void effPCPThunderSpherePolySetPosition(void *destination, void *source);

/* Scales the target over the first 22 frames, fades the colour out from frame 45. */
void effPCPShockWave01Draw(EffPCPFadeWork *work) {
    EffThunderParameterHead *parameterHead;
    f32 phase;
    u32 fadedColor;
    u32 frame;

    frame = work->frame;
    if (frame < 0x17) {
        parameterHead = effPCPThunderSpherePolyGetParam(work->handle);
        frame = work->frame;
        phase = (f32)frame / 22.0f;
        parameterHead->vector.radiusScale = phase * 600.0f + 200.0f;
        parameterHead->vector.heightScale = phase * 300.0f + 100.0f;
    }
    if (frame >= 0x2D) {
        phase = (f32)(frame - 0x2D) / 30.0f;
        fadedColor = effColorBlend(work->color, 0, phase);
    } else {
        fadedColor = work->color;
    }
    effPCPThunderSpherePolySetColor(work->handle, fadedColor);
    effPCPThunderSpherePolySetPosition((void *)work->handle, work);
    effPCPThunderSpherePolyDraw(work->handle);
    work->frame++;
}

void effPCPShockWave01SetPosition(f32 destinationPosition[4], const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(destinationPosition, sourcePosition);
}

/* Retain this fade variant's float input; its observed updater does not read it. */
void effPCPShockWave01SetScale(EffPCPFadeWork *work, f32 input) {
    work->unk10 = input;
}

void effPCPShockWave01SetColor(EffPCPFadeWork *work, u32 value) {
    work->color = value;
}

extern EffThunderFragmentParams effPCPShockWave02Param[];

/* Same spread as effPCPZiodin03Init with its own parameter block. */
void effPCPShockWave02Init(EffPCPThunderGroup *group) {
    s32 i;
    f32 angle = 0.0f;
    f32 sinv = 0.0f;
    f32 cosv = 0.0f;
    f32 axis[4];
    f32 spread;
    f32 height;
    f32 speed;
    s16 life;

    i = 0;
    do {
        if ((i & 1) == 0) {
            sinv = sdfCos(angle);
            cosv = sdfSin(angle);
            angle += 24.0f * EFF_DEG2RAD;
        }
        spread = dds3GetRandFloatA(effDefaultRandomState) * 0.5f + 0.5f;
        effPCPShockWave02Param->bandWidth = spread * 1.25f;
        effPCPShockWave02Param->edgeWidth = spread * 12.5f;
        effPCPShockWave02Param->end[1] = 0;
        effPCPShockWave02Param->end[0] = sinv * ((dds3GetRandFloatA(effDefaultRandomState) * 0.25f + 0.75f) * 500.0f);
        effPCPShockWave02Param->end[2] = cosv * ((dds3GetRandFloatA(effDefaultRandomState) * 0.25f + 0.75f) * 250.0f);
        axis[0] = cosv;
        axis[1] = 0;
        axis[2] = -sinv;
        SetRotateMatrixAxis(-(dds3GetRandFloatA(effDefaultRandomState) * (55.0f * EFF_DEG2RAD) + 5.0f * EFF_DEG2RAD), (const struct RwV3d *)axis);
        height = (dds3GetRandFloatA(effDefaultRandomState) * 0.65f + (1.0f - 0.65f)) * 500.0f;
        effPCPShockWave02Param->start[0] = 0;
        effPCPShockWave02Param->start[2] = 0;
        effPCPShockWave02Param->start[1] = -height;
        VU0_LOAD_VF($vf10, effPCPShockWave02Param->start);
        SDF_VU0_APPLYMATRIX_XYZ();
        VU0_STORE_VF($vf10, effPCPShockWave02Param->start);
        effPCPShockWave02Param->start[0] += effPCPShockWave02Param->end[0];
        effPCPShockWave02Param->start[2] += effPCPShockWave02Param->end[2];
        if (height < 300.0f) {
            speed = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f * 2.5f + 5.0f;
            life = dds3GetRandA((EffRandState *)effDefaultRandomState) % 5 + 5;
        } else {
            speed = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f * 5.0f + 10.0f;
            life = dds3GetRandA((EffRandState *)effDefaultRandomState) % 6 + 15;
        }
        effPCPShockWave02Param->halfLife = life;
        effPCPShockWave02Param->waveAmplitude = speed;
        group->handles[i] = effPCP_ThunderPolyCreate(effPCPShockWave02Param);
        i++;
    } while (i < 30);
}

void *effPCPShockWave02Create(void) {
    void *work;

    work = sdfChipAllocDirect(0x94);
    effPCPShockWave02Init(work);
    return work;
}

void effPCPShockWave02Delete(EffPCPThunderGroup *group) {
    s32 i;
    for (i = 0; i < 30; i++) {
        effPCP_ThunderPolyDelete(group->handles[i]);
    }
    sdfChipFreeDirect(group);
}

void *effPCPShockWave02Copy(void) {
    void *work;

    work = sdfChipAllocDirect(0x94);
    effPCPShockWave02Init(work);
    return work;
}

void effPCPShockWave02Draw(EffPCPThunderGroup *work) {
    effPcpShiftThunderHandles(work, 30);
}

void effPCPShockWave02SetPosition(f32 destinationPosition[4], const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(destinationPosition, sourcePosition);
}

void effPCPShockWave02SetScale(EffPCPThunderGroup *work, f32 val) {
    work->unk14 = val;
}

void effPCPShockWave02SetColor(EffPCPThunderGroup *work, u32 value) {
    work->unk18 = value;
}

void effPCPHouden01Init(EffPCPFadeWork *work) {
    EffThunderVectorWork *handle;

    handle = effPCPThunderSpherePolyCreate((EffThunderAlphaParams *)effPCP_Houden01_ThunderSpherePoly_Param);
    work->frame = 0;
    work->handle = handle;
}

void *effPCPHouden01Create(void) {
    void *work;

    work = sdfChipAllocDirect(0x20);
    effPCPHouden01Init(work);
    return work;
}

void effPCPHouden01Delete(EffPCPFadeWork *work) {
    effPCPThunderSpherePolyDelete(work->handle);
    sdfChipFreeDirect(work);
}

void *effPCPHouden01Copy(void) {
    void *work;

    work = sdfChipAllocDirect(0x20);
    effPCPHouden01Init(work);
    return work;
}

/* Same fade with a longer scale-up; stops updating once frame 65 is reached. */
void effPCPHouden01Draw(EffPCPFadeWork *work) {
    EffThunderParameterHead *parameterHead;
    f32 phase;
    f32 value;
    u32 fadedColor;
    u32 frame;

    frame = work->frame;
    if (frame == 0x41) {
        return;
    }
    if (frame < 0x1E) {
        parameterHead = effPCPThunderSpherePolyGetParam(work->handle);
        phase = (f32)frame / 30.0f;
        value = phase * 200.0f + 150.0f;
        parameterHead->vector.radiusScale = value;
        parameterHead->vector.heightScale = value;
    }
    if (frame >= 0x39) {
        phase = ((f32)frame - 57.0f) * 0.125f;
        fadedColor = effColorBlend(work->color, 0, phase);
        effPCPThunderSpherePolySetColor(work->handle, fadedColor);
    } else {
        effPCPThunderSpherePolySetColor(work->handle, work->color);
    }
    effPCPThunderSpherePolySetPosition((void *)work->handle, work);
    effPCPThunderSpherePolyDraw(work->handle);
    work->frame++;
}

void effPCPHouden01SetPosition(f32 destinationPosition[4], const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(destinationPosition, sourcePosition);
}

/* Retain this fade variant's float input; its observed updater does not read it. */
void effPCPHouden01SetScale(EffPCPFadeWork *work, f32 input) {
    work->unk10 = input;
}

void effPCPHouden01SetColor(EffPCPFadeWork *work, u32 value) {
    work->color = value;
}

extern s32 D_00436438;

typedef struct EffPCPTrailParams {
    u32 count;
    u32 colors[2];
    f32 scale;
    s32 minSize;
    EffPCPTexturedBlurParams res;
} EffPCPTrailParams;

typedef struct {
    u32 frame;
} EffPCPSharedTrailRef;

EffPCPSharedTrailRef *effPCPBlinkMultiBlurCreate(EffPCPTrailParams *params) {
    EffPCPSharedTrailRef *ref = sdfChipAllocDirect(sizeof(*ref));

    ref->frame = 0;
    if (D_00436438 == 0) {
        pBlinkMultiBlurBase = sdfChipAllocDirect(sizeof(EffPCPSharedTrail));
        pBlinkMultiBlurBase->obj = dds3EffectPartialMultiBlurCreate(&params->res);
        pBlinkMultiBlurBase->color = params->res.source.color;
        pBlinkMultiBlurBase->frame = 0;
        pBlinkMultiBlurBase->flags = 0x80808080;
        pBlinkMultiBlurBase->pos[0] = 0;
        pBlinkMultiBlurBase->pos[1] = 0;
        pBlinkMultiBlurBase->pos[2] = 0;
        pBlinkMultiBlurBase->sizeInput = 0;
        pBlinkMultiBlurBase->limit = params->count;
        pBlinkMultiBlurBase->baseLimit = params->count;
        pBlinkMultiBlurBase->colors[0] = params->colors[0];
        pBlinkMultiBlurBase->colors[1] = params->colors[1];
        pBlinkMultiBlurBase->scale = params->scale;
        pBlinkMultiBlurBase->minSize = params->minSize;
    }
    D_00436438++;
    return ref;
}

void effPCPBlinkMultiBlurDelete(EffPCPSharedTrailRef *work)
{
    sdfChipFreeDirect(work);
    if (--D_00436438 != 0) {
        return;
    }
    dds3EffectPartialMultiBlurDelete(pBlinkMultiBlurBase->obj);
    sdfChipFreeDirect(pBlinkMultiBlurBase);
}

typedef struct EffPCPTrailWork {
    f32 pos[4];
    u32 flags;
    u32 color;
    u32 frame;
    f32 sizeInput;
    u32 count;
    u32 colors[2];
    f32 scale; /* multiplies the computed trail size */
    s32 minSize;
    EffBlurTemplate *obj;
} EffPCPTrailWork;

typedef char EffPCPTrailWork_size_must_be_0x38[
    (sizeof(EffPCPTrailWork) == 0x38) ? 1 : -1];
typedef char EffPCPTrailWork_sizeInput_offset_must_be_0x1C[
    ((u32)&((EffPCPTrailWork *)0)->sizeInput == 0x1C) ? 1 : -1];

extern s32 effPCPBlurScreenOffset(f32 value);
extern u32 effMagaCylinderCalcColor(u32 flags, u32 color);
#define EFF_SHARED_TRAIL ((EffPCPSharedTrail *)pBlinkMultiBlurBase)

void effPCPBlinkMultiBlurDraw(ref)
    EffPCPSharedTrailRef *ref;
{
    EffPCPSharedTrail *work;
    EffBlurTemplate *obj;
    f32 pos[4];

    if (ref->frame == 0) {
        EFF_SHARED_TRAIL->frame &= 1;
        if (EFF_SHARED_TRAIL->frame != 0) {
            EFF_SHARED_TRAIL->limit = EFF_SHARED_TRAIL->baseLimit + 1;
        }
        ref->frame = EFF_SHARED_TRAIL->frame;
    }
    work = EFF_SHARED_TRAIL;
    if (ref->frame != work->frame) {
        ref->frame = work->frame;
        return;
    }
    obj = work->obj;
    ref->frame = ref->frame + 1;
    if (work->frame < work->limit) {
        work->color = work->colors[work->frame & 1];
        VU0_LOAD_VF($vf10, work);
        obj->body.extent = (s32)((f32)effPCPBlurScreenOffset(EFF_SHARED_TRAIL->sizeInput) * EFF_SHARED_TRAIL->scale);
        VU0_STORE_VF($vf10, pos);
        obj->body.source.position[0] = (s32)pos[0] - 0x800;
        obj->body.source.position[1] = ((s32)pos[1] - 0x800) << 1;
        if (obj->body.extent < EFF_SHARED_TRAIL->minSize) {
            obj->body.extent = EFF_SHARED_TRAIL->minSize;
        }
        obj->body.source.color = effMagaCylinderCalcColor(EFF_SHARED_TRAIL->flags, EFF_SHARED_TRAIL->color);
        effDrawBlurPixelRectWithResource(obj);
        EFF_SHARED_TRAIL->frame++;
    }
}

void effPCPBlinkMultiBlurSetPosition(void *work, void *src) {
    SDF_VU0_COPYVECTOR(pBlinkMultiBlurBase, src);
}

void effPCPBlinkMultiBlurSetColor(u32 unused, u32 val) {
    pBlinkMultiBlurBase->flags = val;
}

void effPCPBlinkMultiBlurSetArea(u32 unused, f32 value) {
    pBlinkMultiBlurBase->sizeInput = value;
}


EffPCPTrailWork *effPCPMultiBlurExpCreate(EffPCPTrailParams *params) {
    EffPCPTrailWork *work = sdfChipAllocDirect(sizeof(EffPCPTrailWork));

    work->obj = dds3EffectPartialMultiBlurCreate(&params->res);
    work->color = params->res.source.color;
    work->frame = 0;
    work->flags = 0x80808080;
    work->pos[0] = 0.0f;
    work->pos[1] = 0.0f;
    work->pos[2] = 0.0f;
    work->sizeInput = 0.0f;
    work->count = params->count;
    work->colors[0] = params->colors[0];
    work->colors[1] = params->colors[1];
    work->scale = params->scale;
    work->minSize = params->minSize;
    return work;
}

void effPCPMultiBlurExpDelete(EffPCPTrailWork *work) {
    dds3EffectPartialMultiBlurDelete(work->obj);
    sdfChipFreeDirect(work);
}

/* Per-frame update: places the trail object from the work position and
 * alternates its colour between two entries until the count runs out. */
void effPCPMultiBlurExpDraw(EffPCPTrailWork *work) {
    EffBlurTemplate *obj;
    f32 pos[4];

    obj = work->obj;
    VU0_LOAD_VF($vf10, work->pos);
    obj->body.extent = (s32)((f32)effPCPBlurScreenOffset(work->sizeInput) * work->scale);
    VU0_STORE_VF($vf10, pos);
    obj->body.source.position[0] = (s32)pos[0] - 0x800;
    obj->body.source.position[1] = ((s32)pos[1] - 0x800) << 1;
    if (obj->body.extent < work->minSize) {
        obj->body.extent = work->minSize;
    }
    if (work->frame < work->count) {
        work->color = work->colors[work->frame & 1];
    } else {
        work->color = work->colors[0];
    }
    work->frame++;
    obj->body.source.color = effMagaCylinderCalcColor(work->flags, work->color);
    effDrawBlurPixelRectWithResource(obj);
}

void effPCPMultiBlurExpSetPosition(void *dst, const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(dst, sourcePosition);
}

void effPCPMultiBlurExpSetColor(EffPCPTrailWork *work, u32 value) {
    work->flags = value;
}

void effPCPMultiBlurExpSetArea(EffPCPTrailWork *work, f32 val) {
    work->sizeInput = val;
}

EffPCPCompactFadeWork *effPCPMultiFilterExpCreate(EffPCPCompactRectParams *params) {
    EffPCPCompactFadeWork *work;

    work = sdfChipAllocDirect(0x3C);
    work->resource.rectangle = dds3EffectPartialMultiFilterCreate(&params->res);
    work->frame = 0;
    work->color = 0x80808080;
    work->flags = params->timeline.flags;
    work->duration = params->timeline.duration;
    work->fadeIn = params->timeline.fadeIn;
    work->fadeOut = params->timeline.fadeOut;
    work->startExtent = params->timeline.startExtent;
    work->endExtent = params->timeline.endExtent;
    work->baseColor = params->res.draw.rgba;
    return work;
}

EffPCPCompactFadeWork *effPCPMultiFilterExpCreatePCF(void *args) {
    void *param0;

    param0 = dds3EffectPCPDataGetData(args, 0);
    return effPCPMultiFilterExpCreate(param0);
}

EffPCPCompactFadeWork *effPCPMultiFilterExpCopy(EffPCPCompactFadeWork *work) {
    EffPCPCompactRectParams params;

    params.timeline.flags = work->flags;
    params.timeline.duration = work->duration;
    params.timeline.fadeIn = work->fadeIn;
    params.timeline.fadeOut = work->fadeOut;
    params.timeline.startExtent = work->startExtent;
    params.timeline.endExtent = work->endExtent;
    memcpy(&params.res, &work->resource.rectangle->params, sizeof(params.res));
    return effPCPMultiFilterExpCreate(&params);
}

void effPCPMultiFilterExpDelete(EffPCPCompactFadeWork *work) {
    dds3EffectPartialMultiFilterDelete(work->resource.rectangle);
    sdfChipFreeDirect(work);
}

/* Both compact fade renderers interpolate their extent with integer truncation. */
static inline s32 effPcpInterpolateCompactExtent(s32 from, s32 to,
    s32 frame, s32 duration) {
    to -= from;
    to = (s32)((f32)to * (f32)frame / (f32)duration);
    return from + to;
}

/* vu0 routine: project the compact rectangle's world-space center to screen. */
void effPCPMultiFilterExpDraw(EffPCPCompactFadeWork *work) {
    f32 projected[4];
    s32 frame = work->frame;
    s32 duration = work->duration;
    EffResourceRectWork *rect = work->resource.rectangle;
    s32 fadeIn;
    s32 fadeOut;
    f32 opacity;
    u32 color;

    if (duration >= frame) {
        fadeIn = work->fadeIn;
        fadeOut = work->fadeOut;
        if (work->flags == 0) {
            SDF_VU0_LOADVECTOR(work->position);
            effScreenOffset();
            SDF_VU0_STOREVECTOR(projected);
            rect->params.center[0] = (s32)projected[0] - 2048;
            rect->params.center[1] = ((s32)projected[1] - 2048) << 1;
        } else {
            rect->params.center[0] = 0;
            rect->params.center[1] = 0;
        }
        rect->params.extent = effPcpInterpolateCompactExtent(
            work->startExtent, work->endExtent, frame, duration);
        if (frame < fadeIn && fadeIn != 0) {
            opacity = (f32)frame / (f32)fadeIn;
        } else if (duration - frame <= fadeOut && fadeOut != 0) {
            opacity = (f32)(duration - frame) / (f32)fadeOut;
        } else {
            opacity = 1.0f;
        }
        color = work->color;
        rect->params.draw.rgba = effMagaCylinderCalcColor(
            effColorBlend(color & 0xFFFFFF, color, opacity), work->baseColor);
        effResourceRectDrawPixels(rect);
        work->frame++;
    }
}

void effPCPMultiFilterExpSetPosition(void *dst, const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(dst, sourcePosition);
}

void effPCPMultiFilterExpSetColor(EffPCPCompactFadeWork *work, u32 value) {
    work->color = value;
}

/* Retain the compact fade input without claiming a scale or timing interpretation. */
void effPCPMultiFilterExpSetArea(EffPCPCompactFadeWork *work, f32 input) {
    work->unk34 = input;
}

EffPCPCompactFadeWork *effPCPMultiBlurExp2Create(EffPCPCompactTexturedBlurParams *params) {
    EffPCPCompactFadeWork *work;

    work = sdfChipAllocDirect(0x3C);
    work->resource.blur = dds3EffectPartialMultiBlurCreate(&params->res);
    work->frame = 0;
    work->color = 0x80808080;
    work->flags = params->timeline.flags;
    work->duration = params->timeline.duration;
    work->fadeIn = params->timeline.fadeIn;
    work->fadeOut = params->timeline.fadeOut;
    work->startExtent = params->timeline.startExtent;
    work->endExtent = params->timeline.endExtent;
    work->baseColor = params->res.source.color;
    return work;
}

EffPCPCompactFadeWork *effPCPMultiBlurExp2CreatePCF(void *args) {
    void *param0;

    param0 = dds3EffectPCPDataGetData(args, 0);
    return effPCPMultiBlurExp2Create(param0);
}

EffPCPCompactFadeWork *effPCPMultiBlurExp2Copy(EffPCPCompactFadeWork *work) {
    EffPCPCompactTexturedBlurParams params;

    params.timeline.flags = work->flags;
    params.timeline.duration = work->duration;
    params.timeline.fadeIn = work->fadeIn;
    params.timeline.fadeOut = work->fadeOut;
    params.timeline.startExtent = work->startExtent;
    params.timeline.endExtent = work->endExtent;
    params.res = work->resource.blur->body;
    return effPCPMultiBlurExp2Create(&params);
}

void effPCPMultiBlurExp2Delete(EffPCPCompactFadeWork *work) {
    dds3EffectPartialMultiBlurDelete(work->resource.blur);
    sdfChipFreeDirect(work);
}

/* vu0 routine: project the compact textured rectangle's world-space center. */
void effPCPMultiBlurExp2Draw(EffPCPCompactFadeWork *work) {
    f32 projected[4];
    s32 frame = work->frame;
    s32 duration = work->duration;
    EffBlurTemplate *rect = work->resource.blur;
    s32 fadeIn;
    s32 fadeOut;
    f32 opacity;
    u32 color;

    if (duration >= frame) {
        fadeIn = work->fadeIn;
        fadeOut = work->fadeOut;
        if (work->flags == 0) {
            SDF_VU0_LOADVECTOR(work->position);
            effScreenOffset();
            SDF_VU0_STOREVECTOR(projected);
            rect->body.source.position[0] = (s32)projected[0] - 2048;
            rect->body.source.position[1] = ((s32)projected[1] - 2048) << 1;
        } else {
            rect->body.source.position[0] = 0;
            rect->body.source.position[1] = 0;
        }
        rect->body.extent = effPcpInterpolateCompactExtent(
            work->startExtent, work->endExtent, frame, duration);
        if (frame < fadeIn && fadeIn != 0) {
            opacity = (f32)frame / (f32)fadeIn;
        } else if (duration - frame <= fadeOut && fadeOut != 0) {
            opacity = (f32)(duration - frame) / (f32)fadeOut;
        } else {
            opacity = 1.0f;
        }
        color = work->color;
        rect->body.source.color = effMagaCylinderCalcColor(
            effColorBlend(color & 0xFFFFFF, color, opacity), work->baseColor);
        effDrawBlurPixelRectWithResource(rect);
        work->frame++;
    }
}

void effPCPMultiBlurExp2SetPosition(void *dst, const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(dst, sourcePosition);
}

void effPCPMultiBlurExp2SetColor(EffPCPCompactFadeWork *work, u32 value) {
    work->color = value;
}

/* Retain the long compact fade input; leave the write-only field's meaning unknown. */
void effPCPMultiBlurExp2SetArea(EffPCPCompactFadeWork *work, f32 input) {
    work->unk34 = input;
}

EffPCPFadeTimer *effPCPFilterExpCreate(const EffPCPFadeTimerParams *src) {
    EffPCPFadeTimer *dst;

    dst = sdfChipAllocDirect(0x30);
    dst->params = *src;
    dst->colorFrom = 0x80808080;
    dst->frame = 0;
    dst->colorTo = src->color;
    return dst;
}

void *effPCPFilterExpCreatePCF(void *args) {
    void *param0;

    param0 = dds3EffectPCPDataGetData(args, 0);
    return effPCPFilterExpCreate(param0);
}

EffPCPFadeTimer *effPCPFilterExpCopy(EffPCPFadeTimerParams *src) {
    return effPCPFilterExpCreate(src);
}

void effPCPFilterExpDelete(EffPCPFadeTimer *work) {
    sdfChipFreeDirect(work);
}


extern void dds3EffectFilter(u32 *color);

/* Timeline update: blend factor ramps up over fadeIn frames, holds at 1.0, then
 * ramps down over the last fadeOut frames. */
void effPCPFilterExpDraw(EffPCPFadeTimer *work) {
    s32 frame = work->frame;
    s32 duration = work->params.duration;
    s32 fadeIn;
    s32 fadeOut;
    f32 t;

    if (duration < frame) {
        return;
    }
    fadeIn = work->params.fadeIn;
    work->params.unk14 = 0;
    work->params.unk18 = 0;
    work->params.width = 0x200;
    work->params.height = 0x1C0;
    fadeOut = work->params.fadeOut;
    if (frame < fadeIn && fadeIn != 0) {
        t = (f32)frame / (f32)fadeIn;
    } else if (duration - frame <= fadeOut && fadeOut != 0) {
        t = (f32)(duration - frame) / (f32)fadeOut;
    } else {
        t = 1.0f;
    }
    work->params.color = effMagaCylinderCalcColor(effColorBlend(work->colorFrom & 0xFFFFFF, work->colorFrom, t), work->colorTo);
    dds3EffectFilter(&work->params.color);
    work->frame++;
}

void effPCPFilterExpSetColor(EffPCPFadeTimer *work, u32 value) {
    work->colorFrom = value;
}

EffPCPFadeTimerLong *effPCPBlurExpCreate(const EffPCPFadeTimerLongParams *src) {
    EffPCPFadeTimerLong *dst;

    dst = sdfChipAllocDirect(0x40);
    dst->params = *src;
    dst->colorFrom = 0x80808080;
    dst->frame = 0;
    dst->colorTo = src->source.color;
    return dst;
}

void *effPCPBlurExpCreatePCF(void *args) {
    void *param0;

    param0 = dds3EffectPCPDataGetData(args, 0);
    return effPCPBlurExpCreate(param0);
}

void *effPCPBlurExpCopy(EffPCPFadeTimerLongParams *src) {
    return effPCPBlurExpCreate(src);
}

void effPCPBlurExpDelete(EffPCPFadeTimerLong *work) {
    sdfChipFreeDirect(work);
}


/* Same timeline as effPCPFilterExpDraw on the longer work layout. */
void effPCPBlurExpDraw(EffPCPFadeTimerLong *work) {
    s32 frame = work->frame;
    s32 duration = work->params.duration;
    s32 fadeIn;
    s32 fadeOut;
    f32 t;

    if (duration < frame) {
        return;
    }
    fadeIn = work->params.fadeIn;
    work->params.source.position[0] = 0;
    work->params.source.position[1] = 0;
    work->params.source.corners[0][0] = 0;
    work->params.source.corners[0][1] = 0;
    work->params.source.corners[1][0] = 0x200;
    work->params.source.corners[1][1] = 0x1C0;
    fadeOut = work->params.fadeOut;
    if (frame < fadeIn && fadeIn != 0) {
        t = (f32)frame / (f32)fadeIn;
    } else if (duration - frame <= fadeOut && fadeOut != 0) {
        t = (f32)(duration - frame) / (f32)fadeOut;
    } else {
        t = 1.0f;
    }
    work->params.source.color = effMagaCylinderCalcColor(effColorBlend(work->colorFrom & 0xFFFFFF, work->colorFrom, t), work->colorTo);
    effDrawBlurRectangle(&work->params.source);
    work->frame++;
}

void effPCPBlurExpSetColor(EffPCPFadeTimerLong *work, u32 value) {
    work->colorFrom = value;
}

EffPCPCompactWork *effPCPDistortionBlurExpCreate(EffPCPCompactScatterParams *params) {
    EffPCPCompactWork *work;

    work = sdfChipAllocDirect(0x38);
    work->resource.scatter = dds3EffectDistortionBlurCreate(&params->res);
    work->flags = params->timeline.flags;
    work->duration = params->timeline.duration;
    work->fadeIn = params->timeline.fadeIn;
    work->fadeOut = params->timeline.fadeOut;
    work->startExtent = params->timeline.startExtent;
    work->endExtent = params->timeline.endExtent;
    work->color = 0x80808080;
    work->frame = 0;
    work->baseColor = params->res.color;
    return work;
}

EffPCPCompactWork *effPCPDistortionBlurExpCreatePCF(void *args) {
    void *param0;

    param0 = dds3EffectPCPDataGetData(args, 0);
    return effPCPDistortionBlurExpCreate(param0);
}


EffPCPCompactWork *effPCPDistortionBlurExpCopy(EffPCPCompactWork *work) {
    EffPCPCompactScatterParams params;

    params.timeline.flags = work->flags;
    params.timeline.duration = work->duration;
    params.timeline.fadeIn = work->fadeIn;
    params.timeline.fadeOut = work->fadeOut;
    params.timeline.startExtent = work->startExtent;
    params.timeline.endExtent = work->endExtent;
    params.res = work->resource.scatter->params;
    return effPCPDistortionBlurExpCreate(&params);
}

void effPCPDistortionBlurExpDelete(EffPCPCompactWork *work) {
    dds3EffectDistortionBlurDelete(work->resource.scatter);
    sdfChipFreeDirect(work);
}

/* vu0 routine: grow the scatter region and project its optional world center. */
void effPCPDistortionBlurExpDraw(EffPCPCompactWork *work) {
    EffBlurScatterWork *resource;
    f32 projected[4] __attribute__((aligned(16)));
    s32 frame = work->frame;
    s32 duration = work->duration;
    s32 fadeIn;
    s32 fadeOut;
    f32 opacity;
    f32 displacement;
    u32 color;

    if (duration < frame) {
        return;
    }
    fadeIn = work->fadeIn;
    fadeOut = work->fadeOut;
    displacement = (f32)(work->endExtent - work->startExtent) *
                   (f32)frame / (f32)duration;
    displacement = (s32)displacement;
    resource = work->resource.scatter;
    resource->params.positionSpread =
        (f32)work->startExtent + displacement;
    if (work->flags == 0) {
        SDF_VU0_LOADVECTOR(work->position);
        effScreenOffset();
        SDF_VU0_STOREVECTOR(projected);
        resource = work->resource.scatter;
        resource->params.position[0] = (s32)projected[0] - 2048;
        resource->params.position[1] = (u32)((s32)projected[1] - 2048) << 1;
    } else {
        resource->params.position[0] = 0;
        resource->params.position[1] = 0;
    }
    if (frame < fadeIn && fadeIn != 0) {
        opacity = (f32)frame / (f32)fadeIn;
    } else if (duration - frame <= fadeOut && fadeOut != 0) {
        opacity = (f32)(duration - frame) / (f32)fadeOut;
    } else {
        opacity = 1.0f;
    }
    color = work->color;
    color = effMagaCylinderCalcColor(
        effColorBlend(color & 0xFFFFFF, color, opacity), work->baseColor);
    resource = work->resource.scatter;
    resource->params.color = color;
    effBlurStepScatterSlotsAndDraw(resource);
    work->frame++;
}

void effPCPDistortionBlurExpSetPosition(void *dst, const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(dst, sourcePosition);
}

void effPCPDistortionBlurExpSetColor(EffPCPCompactWork *work, u32 color) {
    work->color = color;
}

EffPCPCompactWork *effPCPRippleBlurExpCreate(EffPCPCompactScaleParams *params) {
    EffPCPCompactWork *work;

    work = sdfChipAllocDirect(0x38);
    work->resource.scale = dds3EffectRippleBlurCreate(&params->res);
    work->flags = params->timeline.flags;
    work->duration = params->timeline.duration;
    work->fadeIn = params->timeline.fadeIn;
    work->fadeOut = params->timeline.fadeOut;
    work->startExtent = params->timeline.startExtent;
    work->endExtent = params->timeline.endExtent;
    work->color = 0x80808080;
    work->frame = 0;
    work->baseColor = params->res.color;
    return work;
}

EffPCPCompactWork *effPCPRippleBlurExpCreatePCF(void *args) {
    void *param0;

    param0 = dds3EffectPCPDataGetData(args, 0);
    return effPCPRippleBlurExpCreate(param0);
}

EffPCPCompactWork *effPCPRippleBlurExpCopy(EffPCPCompactWork *work) {
    EffPCPCompactScaleParams params;

    params.timeline.flags = work->flags;
    params.timeline.duration = work->duration;
    params.timeline.fadeIn = work->fadeIn;
    params.timeline.fadeOut = work->fadeOut;
    params.timeline.startExtent = work->startExtent;
    params.timeline.endExtent = work->endExtent;
    params.res = work->resource.scale->params;
    return effPCPRippleBlurExpCreate(&params);
}

void effPCPRippleBlurExpDelete(EffPCPCompactWork *work) {
    dds3EffectRippleBlurDelete(work->resource.scale);
    sdfChipFreeDirect(work);
}

/* vu0 routine: grow the scale slots and restart looping world-space bursts. */
void effPCPRippleBlurExpDraw(EffPCPCompactWork *work) {
    EffBlurScaleWork *resource;
    f32 projected[4] __attribute__((aligned(16)));
    s32 frame = work->frame;
    s32 duration = work->duration;
    u8 flags = work->flags;
    s32 fadeIn;
    s32 fadeOut;
    f32 opacity;
    f32 displacement;
    u32 color;

    if (duration < frame) {
        if (flags != 2) {
            return;
        }
        work->frame = 0;
        frame = 0;
        dds3EffectRippleBlurReset(work->resource.scale);
    }
    fadeIn = work->fadeIn;
    fadeOut = work->fadeOut;
    displacement = (f32)(work->endExtent - work->startExtent) *
                   (f32)frame / (f32)duration;
    displacement = (s32)displacement;
    resource = work->resource.scale;
    resource->params.size =
        (f32)work->startExtent + displacement;
    if (flags == 0 || flags == 2) {
        SDF_VU0_LOADVECTOR(work->position);
        effScreenOffset();
        SDF_VU0_STOREVECTOR(projected);
        resource = work->resource.scale;
        resource->params.position[0] = (s32)projected[0] - 2048;
        resource->params.position[1] = (u32)((s32)projected[1] - 2048) << 1;
    } else {
        resource->params.position[0] = 0;
        resource->params.position[1] = 0;
    }
    if (frame < fadeIn && fadeIn != 0) {
        opacity = (f32)frame / (f32)fadeIn;
    } else if (duration - frame <= fadeOut && fadeOut != 0) {
        opacity = (f32)(duration - frame) / (f32)fadeOut;
    } else {
        opacity = 1.0f;
    }
    color = work->color;
    color = effMagaCylinderCalcColor(
        effColorBlend(color & 0xFFFFFF, color, opacity), work->baseColor);
    resource = work->resource.scale;
    resource->params.color = color;
    effBlurStepScaleSlotsAndDraw(resource);
    work->frame++;
}

void effPCPRippleBlurExpSetPosition(void *dst, const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(dst, sourcePosition);
}

void effPCPRippleBlurExpSetColor(EffPCPCompactWork *work, u32 color) {
    work->color = color;
}

void effPCPMegidora01Init(EffPCPFadeWork *work) {
    EffThunderVectorWork *handle;

    handle = effPCPThunderSpherePolyCreate((EffThunderAlphaParams *)effPCP_Megidora01_ThunderSpherePoly_Param);
    work->frame = 0;
    work->handle = handle;
}

void *effPCPMegidora01Create(void) {
    void *work;

    work = sdfChipAllocDirect(0x20);
    effPCPMegidora01Init(work);
    return work;
}

void effPCPMegidora01Delete(EffPCPFadeWork *work) {
    effPCPThunderSpherePolyDelete(work->handle);
    sdfChipFreeDirect(work);
}

void *effPCPMegidora01Copy(void) {
    void *work;

    work = sdfChipAllocDirect(0x20);
    effPCPMegidora01Init(work);
    return work;
}

void effPCPMegidora01Draw(EffPCPFadeWork *work) {
    EffThunderParameterHead *parameterHead;
    f32 phase;
    f32 value;
    u32 fadedColor;
    u32 frame;

    frame = work->frame;
    if (frame < 0x1F) {
        parameterHead = effPCPThunderSpherePolyGetParam(work->handle);
        frame = work->frame;
        phase = (f32)frame / 30.0f;
        value = phase * 150.0f + 100.0f;
        parameterHead->vector.radiusScale = value;
        parameterHead->vector.heightScale = value;
    } else if (frame - 0x2D < 0x10) {
        parameterHead = effPCPThunderSpherePolyGetParam(work->handle);
        frame = work->frame;
        phase = (f32)(frame - 0x2D) / 15.0f;
        value = phase * 150.0f + 250.0f;
        parameterHead->vector.radiusScale = value;
        parameterHead->vector.heightScale = value;
    }
    if (frame >= 0x2D) {
        phase = (f32)(frame - 0x2D) / 35.0f;
        fadedColor = effColorBlend(work->color, 0, phase);
    } else {
        fadedColor = work->color;
    }
    effPCPThunderSpherePolySetColor(work->handle, fadedColor);
    effPCPThunderSpherePolySetPosition((void *)work->handle, work);
    effPCPThunderSpherePolyDraw(work->handle);
    work->frame++;
}

void effPCPMegidora01SetPosition(void *dst, const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(dst, sourcePosition);
}

/* Retain this fade variant's float input; its observed updater does not read it. */
void effPCPMegidora01SetScale(EffPCPFadeWork *work, f32 input) {
    work->unk10 = input;
}

void effPCPMegidora01SetColor(EffPCPFadeWork *work, u32 value) {
    work->color = value;
}

void effPCPMegidoraon01Init(EffPCPFadeWork *work) {
    EffThunderVectorWork *handle;

    handle = effPCPThunderSpherePolyCreate((EffThunderAlphaParams *)effPCP_Megidoraon01_ThunderSpherePoly_Param);
    work->frame = 0;
    work->handle = handle;
}

void *effPCPMegidoraon01Create(void) {
    void *work;

    work = sdfChipAllocDirect(0x20);
    effPCPMegidoraon01Init(work);
    return work;
}

void effPCPMegidoraon01Delete(EffPCPFadeWork *work) {
    effPCPThunderSpherePolyDelete(work->handle);
    sdfChipFreeDirect(work);
}

void *effPCPMegidoraon01Copy(void) {
    void *work;

    work = sdfChipAllocDirect(0x20);
    effPCPMegidoraon01Init(work);
    return work;
}

/* Scales the target through two frame windows and fades the colour out from
 * frame 60. */
void effPCPMegidoraon01Draw(EffPCPFadeWork *work) {
    EffThunderParameterHead *parameterHead;
    f32 phase;
    u32 fadedColor;
    u32 frame;

    frame = work->frame;
    if (frame < 0x2E) {
        parameterHead = effPCPThunderSpherePolyGetParam(work->handle);
        frame = work->frame;
        phase = (f32)frame / 45.0f;
        parameterHead->vector.heightScale = parameterHead->vector.radiusScale = phase * 950.0f + 200.0f;
    } else if (frame >= 0x3C && frame < 0x4C) {
        parameterHead = effPCPThunderSpherePolyGetParam(work->handle);
        frame = work->frame;
        phase = (f32)(frame - 0x3C) / 15.0f;
        parameterHead->vector.heightScale = parameterHead->vector.radiusScale = phase * 150.0f + 1050.0f;
    }
    if (frame >= 0x3C) {
        phase = (f32)(frame - 0x3C) / 15.0f;
        fadedColor = effColorBlend(work->color, 0, phase);
    } else {
        fadedColor = work->color;
    }
    effPCPThunderSpherePolySetColor(work->handle, fadedColor);
    effPCPThunderSpherePolySetPosition((void *)work->handle, work);
    effPCPThunderSpherePolyDraw(work->handle);
    work->frame++;
}

void effPCPMegidoraon01SetPosition(void *dst, const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(dst, sourcePosition);
}

/* Retain this fade variant's float input; its observed updater does not read it. */
void effPCPMegidoraon01SetScale(EffPCPFadeWork *work, f32 input) {
    work->unk10 = input;
}

void effPCPMegidoraon01SetColor(EffPCPFadeWork *work, u32 value) {
    work->color = value;
}

/* The update uses unsigned frame bounds; the initial sweep interval is
 * computed as a signed difference of the parameter words. */

EffPCPSpanWork *effPCPBreathCreate(const EffPCPSpanParams *params,
                                   EffNodeDescriptor *handleParams) {
    EffPCPSpanWork *work;
    f32 size;

    work = sdfChipAllocDirect(0x78);
    work->params = *params;
    work->color = 0x80808080;
    work->frame = 0;
    size = work->params.sweepDegrees * 0.017453292f;
    work->angle = -(size * 0.5f);
    work->spin = size / (f32)((s32)work->params.activeFrames - ((work->params.fadeFrames >> 1) + (s32)work->params.holdFrames));
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->optionalHandle = 0;
    if (handleParams != NULL) {
        work->optionalHandle = dds3EffectLoad(handleParams);
    }
    return work;
}

EffPCPSpanWork *effPCPBreathCreatePCF(void *args) {
    const EffPCPSpanParams *param0;
    EffNodeDescriptor *param1;

    param0 = (const EffPCPSpanParams *)dds3EffectPCPDataGetData(args, 0);
    param1 = (EffNodeDescriptor *)dds3EffectPCPDataGetData(args, 1);
    return effPCPBreathCreate(param0, param1);
}

EffPCPSpanWork *effPCPBreathCopy(EffPCPSpanWork *work) {
    EffPCPSpanWork *child;

    child = effPCPBreathCreate(&work->params, NULL);
    if (work->optionalHandle != NULL) {
        child->optionalHandle = dds3EffectCopy(work->optionalHandle);
    }
    return child;
}

void effPCPBreathDelete(EffPCPSpanWork *work) {
    if (work->optionalHandle != NULL) {
        dds3EffectDelete(work->optionalHandle);
    }
    sdfChipFreeDirect(work);
}



extern u32 effBTLFieldColorGetVariantSelector(void);
extern u32 effBTLFieldColorUsesActorSelectors(void);
extern f32 btlMeasureFilteredUnitGroupBounds(s32 mask, f32 *maxTop, f32 *minTop);
extern u32 effBTLFieldColorGetOriginalSelector(void);

/* On the first frame, center the sweep on the battle group's direction.
   Place the optional node around the anchor, then fade its final frames. */
void effPcpUpdateOrbitingAimNode(EffPCPSpanWork *work) {
    EffPCPSpanParams *params = &work->params;
    struct EffNode *node = work->optionalHandle;
    s32 fadeFrames = params->fadeFrames;
    u32 end = fadeFrames + params->activeFrames;
    u32 frame = work->frame;
    s32 remaining = end - frame;
    f32 mtx[16];
    f32 muzzle[4];
    f32 aim[4];
    f32 center[4];
    f32 look[4];
    f32 t;

    if (end < frame) {
        return;
    }
    if (effBTLFieldColorUsesActorSelectors() && work->frame == 0) {
        btlMeasureFilteredUnitGroupBounds(((BtlUnit *)effBTLFieldColorGetVariantSelector())->status.flags & 0x600, NULL, NULL);
        SDF_VU0_STOREVECTOR(center);
        btlUnitGetMuzzlePosVU((BtlUnit *)effBTLFieldColorGetOriginalSelector());
        SDF_VU0_STOREVECTOR(muzzle);
        look[0] = center[0] - muzzle[0];
        look[2] = center[2] - muzzle[2];
        work->angle = sdfAtan2(look[0], look[2]) - params->sweepDegrees * EFF_DEG2RAD * 0.5f;
    }
    if (node != 0) {
        VU0_LOAD_MATRIX(work);
        sdfVuLoadSecondaryRotationMatrixY(work->angle);
        sdfComposeVuMatrixFromRegisters();
        aim[0] = aim[1] = 0.0f;
        aim[2] = 1.0f;
        SDF_VU0_LOADVECTOR(aim);
        SDF_VU0_APPLYMATRIX_XYZ();
        VEC3_SPLAT(muzzle, params->radius);
        SDF_VU0_LOADVECTOR_B(muzzle);
        SDF_VU0_MULVECTOR();
        SDF_VU0_LOADVECTOR_B(params->anchor);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR_NOMEM(muzzle);
        effCopyVectorToNodeInstance(node, muzzle);
        if (work->frame > params->holdFrames) {
            work->angle += work->spin;
        }
        SetRotateMatrixY(work->angle);
        VU0_STORE_MATRIX(mtx);
        effApplyNodeTransformMatrix(node, mtx);
        if (fadeFrames >= remaining && fadeFrames != 0) {
            t = (f32)remaining / (f32)fadeFrames;
        } else {
            t = 1.0f;
        }
        effSetNodeParameterValue((struct EffNode *)node, effColorBlend(work->color & 0xFFFFFF, work->color, t));
        dds3EffectDraw((struct EffNode *)node);
    }
    work->frame++;
}

void effPCPBreathSetPosition(void *work, void *src) {
    SDF_VU0_COPYVECTOR(((EffPCPSpanWork *)work)->params.anchor, src);
}

void effPCPBreathSetColor(EffPCPSpanWork *work, u32 value) {
    work->color = value;
}

void effPCPBreathSetRotateMatrix(void *dst, void *src) {
    SetRotateMatrixY(3.1415927f);
        SDF_VU0_LOADMATRIX_B(src);
;
    sdfComposeVuMatrixFromRegisters();
        VU0_STORE_MATRIX(dst);
;
}

EffPCPTripleWork *effPCPAgiraoFireCreate(EffPCPTripleParams *params, EffNodeDescriptor **descriptors) {
    EffPCPTripleWork *work;
    struct EffNode **handle;
    u32 i;

    work = sdfChipAllocDirect(sizeof(EffPCPTripleWork));
    work->head = *params;
    work->frame = 0;
    work->color = 0x80808080;
    handle = work->handles;
    for (i = 0; i < 7; i++) {
        handle[0] = dds3EffectLoad(descriptors[i]);
        handle[7] = dds3EffectCopy(handle[0]);
        handle[14] = dds3EffectCopy(handle[0]);
        handle++;
    }
    return work;
}

EffPCPTripleWork *effPCPAgiraoFireCreatePCF(void *data) {
    EffPCPTripleParams *params;
    EffNodeDescriptor *descriptors[7];
    EffNodeDescriptor **dst;
    u32 i;

    params = dds3EffectPCPDataGetData(data, 0);
    dst = descriptors;
    i = 0;
    do {
        i++;
        *dst = dds3EffectPCPDataGetData(data, i);
        dst++;
    } while (i < 7);
    return effPCPAgiraoFireCreate(params, descriptors);
}

EffPCPTripleWork *effPCPAgiraoFireCopy(EffPCPTripleWork *src) {
    EffPCPTripleWork *work;
    struct EffNode **sourceHandleCursor;
    struct EffNode **destinationHandleCursor;
    u32 slotIndex;

    work = sdfChipAllocDirect(0xAC);
    work->head = src->head;
    work->frame = 0;
    work->color = 0x80808080;
    sourceHandleCursor = (src->handles + 14);
    destinationHandleCursor = (work->handles + 14);
    for (slotIndex = 0; slotIndex < 7; slotIndex++) {
        destinationHandleCursor[-14] = dds3EffectCopy(sourceHandleCursor[-14]);
        destinationHandleCursor[-7] = dds3EffectCopy(sourceHandleCursor[-7]);
        destinationHandleCursor[0] = dds3EffectCopy(sourceHandleCursor[0]);
        sourceHandleCursor++;
        destinationHandleCursor++;
    }
    return work;
}

void effPCPAgiraoFireDelete(EffPCPTripleWork *work) {
    struct EffNode **handleCursor = work->handles;
    u32 slotIndex;

    for (slotIndex = 0; slotIndex < 7; slotIndex++) {
        dds3EffectDelete(handleCursor[14]);
        dds3EffectDelete(handleCursor[7]);
        dds3EffectDelete(handleCursor[0]);
        handleCursor++;
    }
    sdfChipFreeDirect(work);
}

extern f32 aViewDown[4];

/* vu0 routine: place three staggered groups in the camera-relative basis. */
void effPCPAgiraoFireDraw(EffPCPTripleWork *work) {
    f32 position[4] __attribute__((aligned(16)));
    f32 basis[4][4] __attribute__((aligned(16)));
    s32 frame = work->frame;
    u32 color = work->color;
    u32 group;
    u32 handleIndex;

    LookAtMatrix(aViewTarget, sdfViewTargetVector, aViewDown);
    VU0_STORE_MATRIX(basis);
    for (group = 0; group < 3; group++) {
        s32 start = work->head.groupStartFrame[group];
        if (frame >= start) {
            SDF_VU0_LOADVECTOR_B(work->head.origin);
            VU0_LOAD_MATRIX(basis);
            position[0] = work->head.offset[group][0];
            position[1] = work->head.offset[group][1];
            position[2] = 0;
            SDF_VU0_LOADVECTOR_BARRIER(position);
            SDF_VU0_VECTOR_W_SET_ZERO();
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(position);
            for (handleIndex = 0; handleIndex < 7; handleIndex++) {
                if (frame >= (s32)((u32)work->head.handleDelay[handleIndex] + (u32)start)) {
                    effSetNodeParameterValue((struct EffNode *)work->handles[group * 7 + handleIndex], color);
                    effCopyVectorToNodeInstance((struct EffNode *)work->handles[group * 7 + handleIndex], position);
                    dds3EffectDraw((struct EffNode *)work->handles[group * 7 + handleIndex]);
                }
            }
        }
    }
    work->frame = (s32)((u32)work->frame + 1);
}

void effPCPAgiraoFireSetPosition(void *dst, const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(dst, sourcePosition);
}

void effPCPAgiraoFireSetColor(EffPCPTripleWork *work, u32 value) {
    work->color = value;
}



/* Build a block-set work: copy the parameter block, then create one parameter handle per input block. */
EffPCPBlockSetWork *effPCPAgiFireballCreate(const EffPCPBlockSetParams *params, void **blocks) {
    EffPCPBlockSetWork *work;
    dds3ModelHandle_t *model;
    u32 i;
    u32 j;
    u32 n;

    work = sdfChipAllocDirect(0x10C);
    memset(work, 0, 0x10C);
    work->params = *params;
    work->frame = 0;
    work->color = 0x80808080;
    work->mode = EFF_PCP_BLOCK_SET_MODE_DEFAULT;
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->headHandle = dds3EffectPartCreate(EFF_PARAM_WORK_KIND_VIEWER_CONTEXT, blocks[0]);
    work->handleA[0] = dds3EffectPartCreate(EFF_PARAM_WORK_KIND_EFFECT_NODE, blocks[1]);
    work->handleA[1] = dds3EffectPartCreate(EFF_PARAM_WORK_KIND_EFFECT_NODE, blocks[2]);
    work->handleA[2] = dds3EffectPartCreate(EFF_PARAM_WORK_KIND_EFFECT_NODE, blocks[3]);
    work->handleA[3] = dds3EffectPartCreate(EFF_PARAM_WORK_KIND_EFFECT_NODE, blocks[4]);
    work->handleA[4] = dds3EffectPartCreate(EFF_PARAM_WORK_KIND_VIEWER_CONTEXT, blocks[5]);
    model = effParamWorkGetData(work->headHandle);
    work->count = model->first->frameCount;
    for (i = 0; i < 3; i++) {
        if (work->params.groupSize[i] > 0) {
            n = work->count * work->params.groupSize[i];
            work->alloc[i] = sdfMemAllocDirect(n * 4);
            work->list[i] = (effPartHandle_t **)sdfMemLock(work->alloc[i]);
            work->list[i][0] = dds3EffectPartCreate(EFF_PARAM_WORK_KIND_EFFECT_NODE, blocks[6 + i]);
            for (j = 1; j < n; j++) {
                work->list[i][j] = 0;
            }
        } else {
            work->alloc[i] = NULL;
        }
    }
    work->handleB[0] = dds3EffectPartCreate(EFF_PARAM_WORK_KIND_EFFECT_NODE, blocks[9]);
    work->handleB[1] = dds3EffectPartCreate(EFF_PARAM_WORK_KIND_EFFECT_NODE, blocks[10]);
    work->handleB[2] = dds3EffectPartCreate(EFF_PARAM_WORK_KIND_EFFECT_NODE, blocks[11]);
    work->handleB[3] = dds3EffectPartCreate(EFF_PARAM_WORK_KIND_EFFECT_NODE, blocks[12]);
    work->handleB[4] = dds3EffectPartCreate(EFF_PARAM_WORK_KIND_EXTENDED_WORK_WITH_MATRIX_CALLBACK, blocks[13]);
    work->tailHandle = dds3EffectPartCreate(EFF_PARAM_WORK_KIND_EFFECT_NODE, blocks[14]);
    return work;
}

EffPCPBlockSetWork *effPCPAgiFireballCreatePCF(void *parameterTable) {
    EffPCPBlockSetInputs set;
    EffPCPBlockSetParams *first;
    u32 i;

    first = dds3EffectPCPDataGetData(parameterTable, 0);
    set.named.block1 = dds3EffectPCPDataGetData(parameterTable, 1);
    for (i = 0; i < 5; i++) {
        set.named.group[i] = dds3EffectPCPDataGetData(parameterTable, 2 + i);
    }
    set.named.block7 = dds3EffectPCPDataGetData(parameterTable, 7);
    set.named.block8 = dds3EffectPCPDataGetData(parameterTable, 8);
    set.named.block9 = dds3EffectPCPDataGetData(parameterTable, 9);
    for (i = 0; i < 5; i++) {
        set.named.tail[i] = dds3EffectPCPDataGetData(parameterTable, 10 + i);
    }
    set.named.block15 = dds3EffectPCPDataGetData(parameterTable, 15);
    return effPCPAgiFireballCreate(first, set.blocks);
}

/* Duplicate the block-set handles and allocate its three optional instance lists. */
void effPCPAgiFireballCopySub(EffPCPBlockSetWork *work, EffPCPBlockSetWork *src) {
    dds3ModelHandle_t *headModel;
    u32 handleIndex;
    u32 groupIndex;
    u32 groupHandleCount;

    work->headHandle = dds3EffectPartCopy(src->headHandle);
    for (handleIndex = 0; handleIndex < ARRAY_COUNT(work->handleA); handleIndex++) {
        work->handleA[handleIndex] = dds3EffectPartCopy(src->handleA[handleIndex]);
    }
    headModel = effParamWorkGetData(work->headHandle);
    work->count = headModel->first->frameCount;
    for (groupIndex = 0; groupIndex < ARRAY_COUNT(work->list); groupIndex++) {
        if (work->params.groupSize[groupIndex] > 0) {
            groupHandleCount = work->count * work->params.groupSize[groupIndex];
            work->alloc[groupIndex] = sdfMemAllocDirect(groupHandleCount * 4);
            work->list[groupIndex] = (effPartHandle_t **)sdfMemLock(work->alloc[groupIndex]);
            work->list[groupIndex][0] = dds3EffectPartCopy(src->list[groupIndex][0]);
            for (handleIndex = 1; handleIndex < groupHandleCount; handleIndex++) {
                work->list[groupIndex][handleIndex] = 0;
            }
        } else {
            work->alloc[groupIndex] = NULL;
        }
    }
    for (handleIndex = 0; handleIndex < ARRAY_COUNT(work->handleB); handleIndex++) {
        work->handleB[handleIndex] = dds3EffectPartCopy(src->handleB[handleIndex]);
    }
    work->tailHandle = dds3EffectPartCopy(src->tailHandle);
}




/* Clone of a block-set work that shares the source's blocks (no back pointer). */
EffPCPBlockSetWork *effPCPAgiFireballCopy(EffPCPBlockSetWork *src) {
    EffPCPBlockSetWork *work;

    work = sdfChipAllocDirect(0x10C);
    memset(work, 0, 0x10C);
    work->params = src->params;
    work->frame = 0;
    work->color = 0x80808080;
    work->mode = EFF_PCP_BLOCK_SET_MODE_DEFAULT;
    EE_MMI_UNIT_MATRIX(work->matrix);
    effPCPAgiFireballCopySub(work, src);
    work->source = NULL;
    return work;
}

void effPCPAgiFireballDelete(EffPCPBlockSetWork *work) {
    u32 groupIndex;
    u32 handleIndex;
    u32 groupHandleCount;

    if (work->source == NULL) {
        dds3EffectPartDelete(work->headHandle);
        for (handleIndex = 0; handleIndex < 5; handleIndex++) {
            dds3EffectPartDelete(work->handleA[handleIndex]);
        }
        for (groupIndex = 0; groupIndex < 3; groupIndex++) {
            if (work->alloc[groupIndex] != 0) {
                groupHandleCount = work->count * work->params.groupSize[groupIndex];
                for (handleIndex = 0; handleIndex < groupHandleCount; handleIndex++) {
                    if (work->list[groupIndex][handleIndex] != 0) {
                        dds3EffectPartDelete(work->list[groupIndex][handleIndex]);
                    }
                }
                sdfMemFreeDirect(work->alloc[groupIndex]);
            }
        }
        for (handleIndex = 0; handleIndex < 5; handleIndex++) {
            dds3EffectPartDelete(work->handleB[handleIndex]);
        }
        dds3EffectPartDelete(work->tailHandle);
    }
    sdfChipFreeDirect(work);
}

extern f32 D_003B1AE0[];

void effPcpBlockSetUpdate(EffPCPBlockSetWork *work) {
    f32 matrix[16];
    f32 step[4];
    f32 side[4];
    f32 scatter[4];
    f32 position[4];
    f32 muzzle[4];
    EffPCPBlockSetWork *source = work->source;
    s32 count;
    s32 frame = work->frame;
    u32 color;
    dds3ModelHandle_t *model;
    u32 i;
    u32 j;
    u32 base;
    s32 fadeIn;
    s32 fadeOut;
    f32 fade;
    f32 scale;
    f32 randomScale;
    f32 randomOffset;

    /* Duplication may change live count; retain both entry snapshots. */
    count = work->count;
    if (source != NULL) {
        effPCPAgiFireballCopySub(work, source);
        work->source = NULL;
    }
    if (frame == 0) {
        for (i = 0; i < 5; i++) {
            dds3EffectPartSetScale(work->handleA[i], work->params.scale);
        }
    }
    fadeIn = work->params.fadeInFrames;
    fadeOut = work->params.fadeOutFrames;
    if (frame < fadeIn && fadeIn != 0) {
        fade = (f32)frame / fadeIn;
    } else if (count - frame <= fadeOut && fadeOut != 0) {
        fade = (f32)(count - frame) / fadeOut;
    } else {
        fade = 1.0f;
    }
    color = work->color;
    color = effColorBlend(color & 0xFFFFFF, color, fade);
    model = effParamWorkGetData(work->headHandle);
    if (work->mode == EFF_PCP_BLOCK_SET_MODE_DEFAULT) {
        dds3EffectPartSetPosition(work->headHandle, work->params.position);
        if (effBTLFieldColorUsesActorSelectors()) {
            btlUnitGetMuzzlePosVU((void *)effBTLFieldColorGetOriginalSelector());
            SDF_VU0_STOREVECTOR_NOMEM(muzzle);
            LookAtMatrix(work->params.position, muzzle, D_003B1AE0);
            ReverseMatrix();
            SDF_VU0_RESET_ROW3();
            VU0_STORE_MATRIX(matrix);
        } else {
            /* Keep the VU source load and destination store as separate stages. */
            VU0_LOAD_MATRIX_MEMORY(work->matrix);
            VU0_STORE_MATRIX(matrix);
        }
    }
    dds3EffectPartSetRotateMatrix(work->headHandle, matrix);
    dds3EffectPartSetColor(work->headHandle, color);
    mdlLink(model, kwlnSceneSurfaceSets[10]);
    SDF_VU0_COPYVECTOR(work->previousPosition, work->currentPosition);
    sdfModelGetMarkPos(model->inner, 1);
    SDF_VU0_STOREVECTOR(work->currentPosition);
    if (frame < count && frame > 0) {
        for (i = 0; i < 5; i++) {
            if (i == 4) {
                if (work->mode == EFF_PCP_BLOCK_SET_MODE_SP ||
                    work->mode == EFF_PCP_BLOCK_SET_MODE_SP2) {
                    dds3EffectPartSetRotateMatrix(work->handleA[i], matrix);
                }
                dds3EffectPartSetPosition(work->handleA[i], work->previousPosition);
            } else {
                dds3EffectPartSetPosition(work->handleA[i], work->currentPosition);
            }
            dds3EffectPartSetColor(work->handleA[i], color);
            dds3EffectPartDraw(work->handleA[i]);
        }
    }
    if (frame >= 0) {
        if (frame == 0) {
            for (i = 0; i < 5; i++) {
                dds3EffectPartSetPosition(work->handleB[i], work->currentPosition);
                dds3EffectPartSetRotateMatrix(work->handleB[i], work->matrix);
            }
        }
        for (i = 0; i < 5; i++) {
            dds3EffectPartSetColor(work->handleB[i], work->color);
            dds3EffectPartDraw(work->handleB[i]);
        }
    }
    if (frame >= work->params.tailStartFrame) {
        if (frame == work->params.tailStartFrame) {
            dds3EffectPartSetPosition(work->tailHandle, work->currentPosition);
            dds3EffectPartSetRotateMatrix(work->tailHandle, matrix);
        }
        dds3EffectPartSetColor(work->tailHandle, work->color);
        dds3EffectPartDraw(work->tailHandle);
    }
    if (frame > 0) {
        for (j = 0; j < 3; j++) {
            if (work->params.groupSize[j] > 0) {
                if (frame < count) {
                    base = (work->frame - 1) * work->params.groupSize[j];
                    SDF_VU0_LOADVECTOR(work->currentPosition);
                    SDF_VU0_LOADVECTOR_B(work->previousPosition);
                    SDF_VU0_SUBVECTOR();
                    SDF_VU0_COPYVECTOR_A_TO_C();
                    VEC3_SPLAT(step, 1.0f / work->params.groupSize[j]);
                    SDF_VU0_LOADVECTOR_B(step);
                    SDF_VU0_MULVECTOR();
                    SDF_VU0_STOREVECTOR(step);
                    SDF_VU0_COPYVECTOR(position, work->previousPosition);
                    SDF_VU0_LOADVECTOR(sdfViewTargetVector);
                    SDF_VU0_LOADVECTOR_B(aViewTarget);
                    SDF_VU0_SUBVECTOR();
                    SDF_VU0_COPYVECTOR_C_TO_B();
                    SDF_VU0_OUTERPRODUCT();
                    SDF_VU0_NORMALIZE();
                    SDF_VU0_STOREVECTOR(side);
                    for (i = 0; i < work->params.groupSize[j]; i++) {
                        if (work->list[j][base + i] == 0) {
                            work->list[j][base + i] = dds3EffectPartCopy(work->list[j][0]);
                            scale = work->params.scale * work->params.groupScale[j];
                            randomScale = work->params.groupRandomScale[j];
                            dds3EffectPartSetScale(work->list[j][base + i], scale *
                                (dds3GetRandFloatA(effDefaultRandomState) * randomScale + (1.0f - randomScale)));
                        }
                        SDF_VU0_LOADVECTOR(position);
                        SDF_VU0_LOADVECTOR_B(step);
                        SDF_VU0_ADDVECTOR();
                        SDF_VU0_COPYVECTOR_A_TO_C();
                        randomOffset = dds3GetRandFloatA(effDefaultRandomState) - 0.5f;
                        randomOffset = randomOffset + randomOffset;
                        VEC3_SPLAT(scatter, work->params.groupScatter[j] * randomOffset);
                        SDF_VU0_LOADVECTOR(side);
                        SDF_VU0_LOADVECTOR_B(scatter);
                        SDF_VU0_MULVECTOR();
                        SDF_VU0_ADDVECTOR_A_C();
                        /* Fully written here, then consumed by VU loads and callbacks. */
                        SDF_VU0_STOREVECTOR_NOMEM(position);
                        dds3EffectPartSetPosition(work->list[j][base + i], position);
                        dds3EffectPartSetColor(work->list[j][base + i], work->color);
                    }
                } else {
                    base = (count - 1) * work->params.groupSize[j];
                }
                for (i = 0; i < base; i++) {
                    if (work->list[j][i] == 0) {
                        work->list[j][i] = dds3EffectPartCopy(work->list[j][0]);
                        scale = work->params.scale * work->params.groupScale[j];
                        randomScale = work->params.groupRandomScale[j];
                        dds3EffectPartSetScale(work->list[j][i], scale *
                            (dds3GetRandFloatA(effDefaultRandomState) * randomScale + (1.0f - randomScale)));
                    }
                    dds3EffectPartSetRotateMatrix(work->list[j][i], work->matrix);
                    dds3EffectPartDraw(work->list[j][i]);
                }
            }
        }
    }
    work->frame++;
}

void effPCPAgiFireballSetPosition(EffPCPBlockSetWork *work, const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(work->params.position, sourcePosition);
}

void effPCPAgiFireballSetColor(EffPCPBlockSetWork *work, u32 value) {
    work->color = value;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effPCPAgiFireballSetRotateMatrix(EffPCPBlockSetWork *work, const Matrix4 *src) {
    VU0_COPY_MATRIX(work->matrix, src);
}

/* Construct the mode-one block set; the native updater treats modes one and two alike. */
EffPCPBlockSetWork *effPCPAgiFireballSpCreatePCF(void *parameterTable) {
    EffPCPBlockSetWork *work;

    work = effPCPAgiFireballCreatePCF(parameterTable);
    work->mode = EFF_PCP_BLOCK_SET_MODE_SP;
    return work;
}

EffPCPBlockSetWork *effPCPAgiFireballSpCopy(EffPCPBlockSetWork *src) {
    EffPCPBlockSetWork *work;

    work = sdfChipAllocDirect(0x10C);
    memset(work, 0, 0x10C);
    work->params = src->params;
    work->color = 0x80808080;
    work->mode = EFF_PCP_BLOCK_SET_MODE_SP;
    work->frame = 0;
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->source = src;
    return work;
}

/* Construct the mode-two block set; no further semantic distinction is established. */
EffPCPBlockSetWork *effPCPAgiFireballSp2CreatePCF(void *parameterTable) {
    EffPCPBlockSetWork *work;

    work = effPCPAgiFireballCreatePCF(parameterTable);
    work->mode = EFF_PCP_BLOCK_SET_MODE_SP2;
    return work;
}

EffPCPBlockSetWork *effPCPAgiFireballSp2Copy(EffPCPBlockSetWork *src) {
    EffPCPBlockSetWork *work;

    work = sdfChipAllocDirect(0x10C);
    memset(work, 0, 0x10C);
    work->params = src->params;
    work->color = 0x80808080;
    work->mode = EFF_PCP_BLOCK_SET_MODE_SP2;
    work->frame = 0;
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->source = src;
    return work;
}

EffPCPRotateWork *effPCPAgiFireball2Create(EffPCPRotateParams *src, EffPCPRotateBlockInputs *blocks) {
    EffPCPRotateWork *work;
    EffPCPBlockSetParams *sub;
    u32 i;

    work = sdfChipAllocDirect(0x10C);
    work->params = *src;
    sub = src->group;
    work->frame = 0;
    for (i = 0; i < 3; i++) {
        blocks->selectedHead = blocks->head[i];
        work->blockSets[i] = effPCPAgiFireballCreate(sub, &blocks->selectedHead);
        sub++;
    }
    return work;
}

/* Gathers 17 parameter blocks from the effect table (the first goes to the
 * rotate effect as its source, the rest into an on-stack block set). */
void effPCPAgiFireball2CreatePCF(args)
    void *args;
{
    EffPCPRotateBlockInputs set;
    void **group;
    void *first;
    s32 idx;
    u32 i;
    u32 j;

    idx = 1;
    first = dds3EffectPCPDataGetData(args, 0);
    for (i = 0; i < 3; i++) {
        set.head[i] = dds3EffectPCPDataGetData(args, idx++);
    }
    group = set.group;
    for (j = 0; j < 5; j++) {
        group[j] = dds3EffectPCPDataGetData(args, idx++);
    }
    for (j = 0; j < 3; j++) {
        set.mid[j] = dds3EffectPCPDataGetData(args, idx++);
    }
    for (j = 0; j < 5; j++) {
        group[j + 8] = dds3EffectPCPDataGetData(args, idx++);
    }
    set.last = dds3EffectPCPDataGetData(args, idx);
    effPCPAgiFireball2Create(first, &set);
}

/* Clones the rotate work: block copy of the head plus one fresh block-clone work
 * per id, each initialised from the source's sub-work. */
EffPCPRotateWork *effPCPAgiFireball2Copy(EffPCPRotateWork *src) {
    EffPCPRotateWork *work;
    EffPCPBlockSetWork *sub;
    u32 i;

    work = sdfChipAllocDirect(0x10C);
    work->params = src->params;
    work->frame = 0;
    for (i = 0; i < 3; i++) {
        work->blockSets[i] = sdfChipAllocDirect(0x10C);
        memset(work->blockSets[i], 0, 0x10C);
        memcpy(&work->blockSets[i]->params, &src->blockSets[i]->params,
               sizeof(work->blockSets[i]->params));
        sub = work->blockSets[i];
        sub->frame = 0;
        sub->color = 0x80808080;
        sub->mode = EFF_PCP_BLOCK_SET_MODE_DEFAULT;
        EE_MMI_UNIT_MATRIX(sub->matrix);
        sub->source = src->blockSets[i];
    }
    return work;
}

void effPCPAgiFireball2Delete(EffPCPRotateWork *work) {
    u32 i;
    EffPCPBlockSetWork **blockSets;

    blockSets = work->blockSets;
    for (i = 0; i < 3; i++) {
        effPCPAgiFireballDelete(blockSets[i]);
    }
    sdfChipFreeDirect(work);
}

/* Dispatch children whose start threshold has been reached. Reload the shared
   frame after each callback before checking the next child. */
void effPCPAgiFireball2Draw(EffPCPRotateWork *work) {
    s32 frame;
    u32 i;

    i = 0;
    frame = work->frame;
    do {
        if (work->params.startFrame[i] <= frame) {
            effPcpBlockSetUpdate(work->blockSets[i]);
            frame = work->frame;
        }
        i = i + 1;
    } while (i < 3);
    work->frame = frame + 1;
}

void effPCPAgiFireball2SetPosition(EffPCPRotateWork *work, const f32 sourcePosition[4]) {
    u32 i;
    EffPCPBlockSetWork **blockSets;

    blockSets = work->blockSets;
    for (i = 0; i < 3; i++) {
        effPCPAgiFireballSetPosition(blockSets[i], sourcePosition);
    }
}

void effPCPAgiFireball2SetColor(EffPCPRotateWork *work, u32 val) {
    u32 i;
    EffPCPBlockSetWork **blockSets;

    blockSets = work->blockSets;
    for (i = 0; i < 3; i++) {
        effPCPAgiFireballSetColor(blockSets[i], val);
    }
}

void effPCPAgiFireball2SetRotateMatrix(EffPCPRotateWork *work, const Matrix4 *src) {
    u32 i;
    EffPCPBlockSetWork **blockSets;

    blockSets = work->blockSets;
    for (i = 0; i < 3; i++) {
        effPCPAgiFireballSetRotateMatrix(blockSets[i], src);
    }
}

EffPCPSpinWork *effPCPBufuIceCreate(void *src) {
    EffPCPSpinWork *work = sdfChipAllocDirect(0x64);

    work->frame = 0;
    work->scale = 1.0f;
    work->color = 0x80808080;
    work->handle0 = dds3EffectPCPDataPartCreate(src, 0);
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->angle = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f * 0.87266457f;
    if (dds3GetRandA((EffRandState *)effDefaultRandomState) & 1) {
        work->angle += 3.14159265f;
    }
    return work;
}

/* Single-handle variant: allocation stops before handle1 (0x64 bytes). */
EffPCPSpinWork *effPCPBufuIceCopy(EffPCPSpinWork *src) {
    EffPCPSpinWork *work = sdfChipAllocDirect(0x64);

    work->frame = 0;
    work->scale = 1.0f;
    work->color = 0x80808080;
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->angle = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f * 0.87266457f;
    if (dds3GetRandA((EffRandState *)effDefaultRandomState) & 1) {
        work->angle += 3.14159265f;
    }
    work->handle0 = dds3EffectPartCopy(src->handle0);
    return work;
}

void effPCPBufuIceDelete(EffPCPSpinWork *work) {
    dds3EffectPartDelete(work->handle0);
    sdfChipFreeDirect(work);
}

void effPCPBufuIceDraw(EffPCPSpinWork *work) {
    effPartHandle_t *handle = work->handle0;
    u128 mtx[4];

    dds3EffectPartSetPosition(handle, work);
    dds3EffectPartSetScale(handle, work->scale);
    dds3EffectPartSetColor(handle, work->color);
    SetRotateMatrixY(work->angle);
        SDF_VU0_LOADMATRIX_B(work->matrix);
;
    sdfComposeVuMatrixFromRegisters();
        VU0_STORE_MATRIX(mtx);
;
    dds3EffectPartSetRotateMatrix(handle, mtx);
    dds3EffectPartDraw(handle);
    work->frame++;
}

void effPCPBufuIceSetPosition(void *dst, const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(dst, sourcePosition);
}

void effPCPBufuIceSetColor(EffPCPSpinWork *work, u32 color) {
    work->color = color;
}

void effPCPBufuIceSetScale(EffPCPSpinWork *work, f32 val) {
    work->scale = val;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effPCPBufuIceSetRotateMatrix(EffPCPSpinWork *work, const Matrix4 *sourceMatrix) {
    VU0_LOAD_MATRIX(sourceMatrix);
    VU0_STORE_MATRIX(work->matrix);
}

EffPCPSpinWork *effPCPBufuraIceCreate(void *table) {
    EffPCPSpinWork *work = sdfChipAllocDirect(0x68);

    work->frame = 0;
    work->scale = 1.0f;
    work->color = 0x80808080;
    work->handle0 = dds3EffectPCPDataPartCreate(table, 0);
    work->handle1 = dds3EffectPCPDataPartCreate(table, 1);
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->angle = 2.0f * (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 0.87266457f;
    return work;
}

EffPCPSpinWork *effPCPBufuraIceCopy(EffPCPSpinWork *src) {
    EffPCPSpinWork *work = sdfChipAllocDirect(0x68);

    work->scale = 1.0f;
    work->frame = 0;
    work->color = 0x80808080;
    work->handle0 = dds3EffectPartCopy(src->handle0);
    work->handle1 = dds3EffectPartCopy(src->handle1);
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->angle = 2.0f * (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 0.87266457f;
    return work;
}

void effPCPBufuraIceDelete(EffPCPSpinWork *work) {
    dds3EffectPartDelete(work->handle1);
    dds3EffectPartDelete(work->handle0);
    sdfChipFreeDirect(work);
}

/* Per-frame update of a two-handle spin effect: pushes the work state to both
 * handles, rebuilds the rotation matrix from the angle and hands it over. The
 * VU0 blocks are bare asm (no "memory" clobber), as the original macros were. */
void effPCPBufuraIceDraw(EffPCPSpinWork *work) {
    effPartHandle_t *handle[2];
    u128 mtx[4];

    handle[0] = work->handle0;
    handle[1] = work->handle1;
    dds3EffectPartSetPosition(handle[0], work);
    dds3EffectPartSetPosition(handle[1], work);
    dds3EffectPartSetScale(handle[0], work->scale);
    dds3EffectPartSetScale(handle[1], work->scale);
    dds3EffectPartSetColor(handle[0], work->color);
    dds3EffectPartSetColor(handle[1], work->color);
    SetRotateMatrixY(work->angle);
            SDF_VU0_LOADMATRIX_B(work->matrix);

    sdfComposeVuMatrixFromRegisters();
            VU0_STORE_MATRIX_UNCLOBBERED(mtx);

    dds3EffectPartSetRotateMatrix(handle[0], mtx);
    dds3EffectPartSetRotateMatrix(handle[1], mtx);
    dds3EffectPartDraw(handle[0]);
    if (work->frame >= 0x1F) {
        dds3EffectPartDraw(handle[1]);
    }
    work->frame++;
}

void effPCPBufuraIceSetPosition(void *dst, const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(dst, sourcePosition);
}

void effPCPBufuraIceSetColor(EffPCPSpinWork *work, u32 color) {
    work->color = color;
}

void effPCPBufuraIceSetScale(EffPCPSpinWork *work, f32 val) {
    work->scale = val;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effPCPBufuraIceSetRotateMatrix(EffPCPSpinWork *work, const Matrix4 *sourceMatrix) {
    VU0_LOAD_MATRIX(sourceMatrix);
    VU0_STORE_MATRIX(work->matrix);
}

EffPCPSpinWork *effPCPBufudnIceCreate(void *table) {
    EffPCPSpinWork *work = sdfChipAllocDirect(0x68);

    work->frame = 0;
    work->scale = 1.0f;
    work->color = 0x80808080;
    work->handle0 = dds3EffectPCPDataPartCreate(table, 0);
    work->handle1 = dds3EffectPCPDataPartCreate(table, 1);
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->angle = 2.0f * (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 0.87266457f;
    return work;
}

EffPCPSpinWork *effPCPBufudnIceCopy(EffPCPSpinWork *src) {
    EffPCPSpinWork *work = sdfChipAllocDirect(0x68);

    work->scale = 1.0f;
    work->frame = 0;
    work->color = 0x80808080;
    work->handle0 = dds3EffectPartCopy(src->handle0);
    work->handle1 = dds3EffectPartCopy(src->handle1);
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->angle = 2.0f * (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 0.87266457f;
    return work;
}

void effPCPBufudnIceDelete(EffPCPSpinWork *work) {
    dds3EffectPartDelete(work->handle1);
    dds3EffectPartDelete(work->handle0);
    sdfChipFreeDirect(work);
}

/* Identical update for the second spin effect. */
void effPCPBufudnIceDraw(EffPCPSpinWork *work) {
    effPartHandle_t *handle[2];
    u128 mtx[4];

    handle[0] = work->handle0;
    handle[1] = work->handle1;
    dds3EffectPartSetPosition(handle[0], work);
    dds3EffectPartSetPosition(handle[1], work);
    dds3EffectPartSetScale(handle[0], work->scale);
    dds3EffectPartSetScale(handle[1], work->scale);
    dds3EffectPartSetColor(handle[0], work->color);
    dds3EffectPartSetColor(handle[1], work->color);
    SetRotateMatrixY(work->angle);
            SDF_VU0_LOADMATRIX_B(work->matrix);

    sdfComposeVuMatrixFromRegisters();
            VU0_STORE_MATRIX_UNCLOBBERED(mtx);

    dds3EffectPartSetRotateMatrix(handle[0], mtx);
    dds3EffectPartSetRotateMatrix(handle[1], mtx);
    dds3EffectPartDraw(handle[0]);
    if (work->frame >= 0x1F) {
        dds3EffectPartDraw(handle[1]);
    }
    work->frame++;
}

void effPCPBufudnIceSetPosition(void *dst, const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(dst, sourcePosition);
}

void effPCPBufudnIceSetColor(EffPCPSpinWork *work, u32 color) {
    work->color = color;
}

void effPCPBufudnIceSetScale(EffPCPSpinWork *work, f32 val) {
    work->scale = val;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effPCPBufudnIceSetRotateMatrix(EffPCPSpinWork *work, const Matrix4 *sourceMatrix) {
    VU0_LOAD_MATRIX(sourceMatrix);
    VU0_STORE_MATRIX(work->matrix);
}

/* Creation-slot callback: enable the global effect flag and return a NULL payload. */
u32 effPCPBufuLightCreate(void) {
    D_0043643C = 1;
    return 0;
}

/* Clone-slot callback: enable the same flag and return a NULL payload, without copying work. */
u32 effPCPBufuLightCopy(void) {
    D_0043643C = 1;
    return 0;
}

/* Release-slot callback is intentionally empty; the update callback clears the global flag. */
void effPCPBufuLightDelete(void) {
}

void effPcpClearGlobalEffectFlag(void) {
    if (D_0043643C != 0) {
        D_0043643C = 0;
    }
}

EffPCPBurstWork *effPCPEyeThunderCreate00_0(u32 unused) {
    EffPCPBurstWork *work;

    work = sdfChipAllocDirect(0x24);
    work->startDistance = 0;
    work->length = 375.0f;
    work->fragmentWork = effPCP_ThunderPolyCreate((EffThunderFragmentParams *)effMuzzleThunderRayParams);
    return work;
}

EffPCPBurstWork *effPCPEyeThunderCopy00_0(void) {
    return effPCPEyeThunderCreate00_0(0);
}

EffPCPBurstWork *effPCPEyeThunderCreate00_1(u32 unused) {
    EffPCPBurstWork *work;

    work = sdfChipAllocDirect(0x24);
    work->startDistance = 325.0f;
    work->length = 225.0f;
    work->fragmentWork = effPCP_ThunderPolyCreate((EffThunderFragmentParams *)effOffsetMuzzleThunderRayParams);
    return work;
}

EffPCPBurstWork *effPCPEyeThunderCopy00_1(void) {
    return effPCPEyeThunderCreate00_1(0);
}

EffPCPBurstWork *effPCPEyeThunderCreate01_0(u32 unused) {
    EffPCPBurstWork *work;

    work = sdfChipAllocDirect(0x24);
    work->startDistance = 0;
    work->length = 375.0f;
    work->fragmentWork = effPCP_ThunderPolyCreate((EffThunderFragmentParams *)effCyanBlueRayPairParams);
    return work;
}

EffPCPBurstWork *effPCPEyeThunderCopy01_0(void) {
    return effPCPEyeThunderCreate01_0(0);
}

EffPCPBurstWork *effPCPEyeThunderCreate01_1(u32 unused) {
    EffPCPBurstWork *work;

    work = sdfChipAllocDirect(0x24);
    work->startDistance = 325.0f;
    work->length = 225.0f;
    work->fragmentWork = effPCP_ThunderPolyCreate((EffThunderFragmentParams *)effCyanBlueRaySingleParams);
    return work;
}

EffPCPBurstWork *effPCPEyeThunderCopy01_1(void) {
    return effPCPEyeThunderCreate01_1(0);
}

EffPCPBurstWork *effPCPEyeThunderCreate02_0(u32 unused) {
    EffPCPBurstWork *work;

    work = sdfChipAllocDirect(0x24);
    work->startDistance = 0;
    work->length = 375.0f;
    work->fragmentWork = effPCP_ThunderPolyCreate((EffThunderFragmentParams *)effWhiteYellowRayPairParams);
    return work;
}

EffPCPBurstWork *effPCPEyeThunderCopy02_0(void) {
    return effPCPEyeThunderCreate02_0(0);
}

EffPCPBurstWork *effPCPEyeThunderCreate02_1(u32 unused) {
    EffPCPBurstWork *work;

    work = sdfChipAllocDirect(0x24);
    work->startDistance = 325.0f;
    work->length = 225.0f;
    work->fragmentWork = effPCP_ThunderPolyCreate((EffThunderFragmentParams *)effWhiteYellowRaySingleParams);
    return work;
}

EffPCPBurstWork *effPCPEyeThunderCopy02_1(void) {
    return effPCPEyeThunderCreate02_1(0);
}

EffPCPBurstWork *effPCPEyeThunderCreate03_0(u32 unused) {
    EffPCPBurstWork *work;

    work = sdfChipAllocDirect(0x24);
    work->startDistance = 0;
    work->length = 375.0f;
    work->fragmentWork = effPCP_ThunderPolyCreate((EffThunderFragmentParams *)effYellowOrangeRayPairParams);
    return work;
}

EffPCPBurstWork *effPCPEyeThunderCopy03_0(void) {
    return effPCPEyeThunderCreate03_0(0);
}

EffPCPBurstWork *effPCPEyeThunderCreate03_1(u32 unused) {
    EffPCPBurstWork *work;

    work = sdfChipAllocDirect(0x24);
    work->startDistance = 325.0f;
    work->length = 225.0f;
    work->fragmentWork = effPCP_ThunderPolyCreate((EffThunderFragmentParams *)effYellowOrangeRaySingleParams);
    return work;
}

EffPCPBurstWork *effPCPEyeThunderCopy03_1(void) {
    return effPCPEyeThunderCreate03_1(0);
}

EffPCPBurstWork *effPCPEyeThunderCreate04_0(u32 unused) {
    EffPCPBurstWork *work;

    work = sdfChipAllocDirect(0x24);
    work->startDistance = 0;
    work->length = 375.0f;
    work->fragmentWork = effPCP_ThunderPolyCreate((EffThunderFragmentParams *)effBrownRedRayPairParams);
    return work;
}

EffPCPBurstWork *effPCPEyeThunderCopy04_0(void) {
    return effPCPEyeThunderCreate04_0(0);
}

EffPCPBurstWork *effPCPEyeThunderCreate04_1(u32 unused) {
    EffPCPBurstWork *work;

    work = sdfChipAllocDirect(0x24);
    work->startDistance = 325.0f;
    work->length = 225.0f;
    work->fragmentWork = effPCP_ThunderPolyCreate((EffThunderFragmentParams *)effBrownRedRaySingleParams);
    return work;
}

EffPCPBurstWork *effPCPEyeThunderCopy04_1(void) {
    return effPCPEyeThunderCreate04_1(0);
}

EffPCPBurstWork *effPCPEyeThunderCreate05_0(u32 unused) {
    EffPCPBurstWork *work;

    work = sdfChipAllocDirect(0x24);
    work->startDistance = 0;
    work->length = 375.0f;
    work->fragmentWork = effPCP_ThunderPolyCreate((EffThunderFragmentParams *)effOrangeVioletRayPairParams);
    return work;
}

EffPCPBurstWork *effPCPEyeThunderCopy05_0(void) {
    return effPCPEyeThunderCreate05_0(0);
}

EffPCPBurstWork *effPCPEyeThunderCreate05_1(u32 unused) {
    EffPCPBurstWork *work;

    work = sdfChipAllocDirect(0x24);
    work->startDistance = 325.0f;
    work->length = 225.0f;
    work->fragmentWork = effPCP_ThunderPolyCreate((EffThunderFragmentParams *)effOrangeVioletRaySingleParams);
    return work;
}

EffPCPBurstWork *effPCPEyeThunderCopy05_1(void) {
    return effPCPEyeThunderCreate05_1(0);
}

void effPCPEyeThunderDelete(EffPCPBurstWork *work) {
    effPCP_ThunderPolyDelete(work->fragmentWork);
    sdfChipFreeDirect(work);
}

/* vu0 routine: normalize the muzzle direction for the thunder ray. */
void effPcpUpdateMuzzleThunderRay(EffPCPBurstWork *work) {
    f32 point[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    f32 dirX;
    f32 dirY;
    f32 dirZ;
    f32 x;
    f32 y;
    f32 z;
    f32 distance;
    EffThunderFragmentParams *params;

    if (effBTLFieldColorUsesActorSelectors() != 0) {
        btlUnitGetMuzzlePosVU((BtlUnit *)effBTLFieldColorGetVariantSelector());
        SDF_VU0_LOADVECTOR_B(work->position);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR(direction);
        params = effPCP_ThunderPolyGetParam(work->fragmentWork);
        distance = work->startDistance;
        dirX = direction[0];
        dirY = direction[1];
        dirZ = direction[2];
        x = work->position[0] + dirX * distance;
        y = work->position[1] + dirY * distance;
        z = work->position[2] + dirZ * distance;
        point[0] = x;
        point[1] = y;
        point[2] = z;
        SDF_VU0_COPYVECTOR(params->start, point);
        distance = work->length;
        point[0] = x + dirX * distance;
        point[1] = y + dirY * distance;
        point[2] = z + dirZ * distance;
        SDF_VU0_COPYVECTOR(params->end, point);
        effPCP_ThunderPolySetColor(work->fragmentWork, work->tintColor);
        effPCP_ThunderPolyDraw(work->fragmentWork);
    }
}

void effPCPEyeThunderSetPosition(void *dst, const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(dst, sourcePosition);
}

void effPCPEyeThunderSetColor(EffPCPBurstWork *work, u32 value) {
    work->tintColor = value;
}


/* Scaled-effect source uses the game creator's alpha parameter variant. */
typedef struct {
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    s32 base;
    s32 target;
    EffThunderAlphaParams res;
} EffPCPScaledThunderParams;

typedef struct EffPCPGrowWork {
    f32 position[4];
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    s32 base;
    s32 target;
    u32 color;
    s32 frame;
    f32 scale;
    EffThunderVectorWork *handle;
} EffPCPGrowWork;

EffPCPGrowWork *effPCPThunderSphereExpCreate(EffPCPScaledThunderParams *params) {
    EffPCPGrowWork *work = sdfChipAllocDirect(sizeof(EffPCPGrowWork));
    EffThunderVectorWork *handle;

    params->res.radiusScale = (f32)params->base;
    params->res.heightScale = (f32)params->base;
    handle = effPCPThunderSpherePolyCreate(&params->res);
    work->duration = params->duration;
    work->fadeIn = params->fadeIn;
    work->fadeOut = params->fadeOut;
    work->base = params->base;
    work->target = params->target;
    work->color = 0x80808080;
    work->frame = 0;
    work->scale = 1.0f;
    work->position[0] = params->res.origin[0];
    work->position[1] = params->res.origin[1];
    work->position[2] = params->res.origin[2];
    work->handle = handle;
    return work;
}

EffPCPGrowWork *effPCPThunderSphereExpCreatePCF(void *data) {
    void *work;

    work = dds3EffectPCPDataGetData(data, 0);
    return effPCPThunderSphereExpCreate(work);
}


EffPCPGrowWork *effPCPThunderSphereExpCopy(EffPCPGrowWork *work) {
    EffPCPScaledThunderParams params;
    EffThunderParameterHead *res;

    res = effPCPThunderSpherePolyGetParam(work->handle);
    params.duration = work->duration;
    params.fadeIn = work->fadeIn;
    params.fadeOut = work->fadeOut;
    params.base = work->base;
    params.target = work->target;
    params.res = res->alpha;
    return effPCPThunderSphereExpCreate(&params);
}

void effPCPThunderSphereExpDelete(EffPCPGrowWork *work) {
    effPCPThunderSpherePolyDelete(work->handle);
    sdfChipFreeDirect(work);
}

/* Preserve the resource query before applying its growth and fade timeline. */
void effPCPThunderSphereExpDraw(EffPCPGrowWork *work) {
    s32 frame;
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    f32 base;
    f32 extent;
    f32 scale;
    f32 opacity;
    u32 color;

    effPCPThunderSpherePolyGetParam(work->handle);
    frame = work->frame;
    duration = work->duration;
    if (duration < frame) {
        return;
    }
    base = (f32)work->base;
    extent = (f32)work->target;
    extent -= base;
    extent = base + extent * ((f32)frame / (f32)duration);
    scale = extent / base;
    fadeIn = work->fadeIn;
    fadeOut = work->fadeOut;
    if (frame < fadeIn && fadeIn != 0) {
        opacity = (f32)frame / (f32)fadeIn;
    } else if ((s32)((u32)duration - (u32)frame) <= fadeOut && fadeOut != 0) {
        opacity = (f32)(s32)((u32)duration - (u32)frame) / (f32)fadeOut;
    } else {
        opacity = 1.0f;
    }
    color = work->color;
    effPCPThunderSpherePolySetColor(work->handle, effColorBlend(color & 0xFFFFFF, color, opacity));
    effPCPThunderSpherePolySetPosition(work->handle, work->position);
    effPCPThunderSpherePolySetScale(work->scale * scale, work->handle);
    effPCPThunderSpherePolyDraw(work->handle);
    work->frame = (s32)((u32)work->frame + 1);
}

void effPCPThunderSphereExpSetPosition(void *dst, const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(dst, sourcePosition);
}

void effPCPThunderSphereExpSetColor(EffPCPGrowWork *work, u32 value) {
    work->color = value;
}

void effPCPThunderSphereExpSetScale(EffPCPGrowWork *work, f32 val) {
    work->scale = val;
}

/* Shared draw-request parameters are cleared before creating a beam. */
extern EffPacketParams effBeamDrawParams;

/* vu0 routine: initialize both transforms with the libvu0 identity primitive. */
EffPCPBeamNode *dds3RingPolyExpListCreate(u32 segments) {
    s32 count = segments * 4 + 4;
    EffPCPBeamNode *node = sdfChipAllocDirect(sizeof(EffPCPBeamNode));
    sdfMemHandle_t *allocation = sdfMemAllocDirect(count * 20);
    f32 *points = (void *)sdfMemLock(allocation);

    memset(points, 0, count * 20);
    node->points = points;
    node->drawKind = 2;
    node->colors = (u32 *)(points + count * 4);
    node->vertexCount = count;
    node->color = 0x80808080;
    node->allocationHandle = allocation;
    node->scale = 1.0f;
    node->assetHandle = sdfMatNew();
    sdfMatSetConstant((struct sdfMat_t *)node->assetHandle, 1.0f);
    EE_MMI_UNIT_MATRIX(node->localMatrix);
    EE_MMI_UNIT_MATRIX(node->matrix);
    memset(&effBeamDrawParams, 0, sizeof(effBeamDrawParams));
    effBeamDrawParams.primitive = 0x4000;
    return node;
}

void dds3RingPolyExpListDelete(EffPCPBeamNode *work) {
    sdfMatFreeDelayed(work->assetHandle);
    sdfMemFreeDirect(work->allocationHandle);
    sdfChipFreeDirect(work);
}


extern u8 effBeamTriangleIndices[];
extern SdfPoolNode *effBeamDrawSurfaces[];
#include "sdf_packet_alloc.h"
extern void *effPkt3DPrimMakePoly(EffPacketParams *params);

/* vu0 routine: compose the beam transform and submit batches of its vertices. */
void dds3RingPolyExpListDraw(EffPCPBeamNode *node) {
    f32 scaleMatrix[4][4];
    sdfDrawTag_t *list;
    s32 count;
    SdfPoolNode *entry;

    if (node->vertexCount == 0) {
        return;
    }
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    EE_MMI_UNIT_MATRIX(scaleMatrix);
    scaleMatrix[2][2] = scaleMatrix[1][1] = scaleMatrix[0][0] = node->scale;
    VU0_LOAD_MATRIX(node->localMatrix);
    SDF_VU0_LOADMATRIX_B(node->matrix);
    sdfComposeVuMatrixFromRegisters();
    SDF_VU0_LOADVECTOR(&node->position);
    VU0_ADD_VF31_VF10_XYZ();
    SDF_VU0_LOADMATRIX_B(scaleMatrix);
    sdfComposeVuMatrixFromRegisters();
    sdf3DPrimLinkTm(list, 0);
    sdf3DPrimLinkMat(list, node->assetHandle, 0);
    count = node->vertexCount;
    effBeamDrawParams.colors = node->colors;
    effBeamDrawParams.positions = (u128 *)node->points;
    effBeamDrawParams.color = node->color;
    effBeamDrawParams.vertexCount = effBeamDrawParams.parameterCount = 12;
    effBeamDrawParams.parameters = (u32 *)effBeamTriangleIndices;
    while (count >= 12) {
        count -= 8;
        sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(&effBeamDrawParams));
        effBeamDrawParams.positions += 8;
        effBeamDrawParams.colors += 8;
    }
    if (count >= 8) {
        effBeamDrawParams.parameterCount = 6;
        effBeamDrawParams.vertexCount = 8;
        sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(&effBeamDrawParams));
    }
    entry = effBeamDrawSurfaces[node->drawKind];
    entry->append(entry, list);
}

void effPCPRingPolyExpSetVertex(f32 radius, EffPCPBeamWork *work) {
    f32 direction[4];
    f32 inner[4];
    f32 first[4];
    f32 middle[4];
    f32 outer[4];
    f32 angle = 0.0f;
    f32 step;
    f32 *out;
    u32 count;
    u32 i;

    VEC3_SPLAT(inner, radius);
    VEC3_SPLAT(first, radius + work->params.firstWidth);
    VEC3_SPLAT(middle, first[0] + work->params.middleWidth);
    VEC3_SPLAT(outer, middle[0] + work->params.lastWidth);
    count = (s32)work->node->vertexCount >> 2;
    out = work->node->points;
    step = 3.14159265f * 2.0f / (f32)work->params.segments;
    direction[1] = 0.0f;
    for (i = 0; i < count; i++, out += 16) {
        direction[0] = sdfCos(angle);
        direction[2] = sdfSin(angle);
        SDF_VU0_LOADVECTOR_B(direction);
        SDF_VU0_LOADVECTOR(inner);
        SDF_VU0_MULVECTOR();
        SDF_VU0_STOREVECTOR(out);
        SDF_VU0_LOADVECTOR(first);
        SDF_VU0_MULVECTOR();
        SDF_VU0_STOREVECTOR(out + 4);
        SDF_VU0_LOADVECTOR(middle);
        SDF_VU0_MULVECTOR();
        SDF_VU0_STOREVECTOR(out + 8);
        SDF_VU0_LOADVECTOR(outer);
        SDF_VU0_MULVECTOR();
        SDF_VU0_STOREVECTOR(out + 12);
        angle += step;
    }
}

void effPCPRingPolyExpInit(EffPCPBeamWork *work) {
    effPCPRingPolyExpSetVertex(work->params.startRadius, work);
    work->frame = 0;
}

u8 *effPCPRingPolyExpCreate(src)
    EffPCPBeamParams *src;
{
    EffPCPBeamWork *work = sdfChipAllocDirect(0x60);
    u32 segments;
    EffPCPBeamNode *node;
    u32 *entry;
    s32 groups;
    u32 i;

    work->params = *src;
    work->color = 0x80808080;
    work->frame = 0;
    segments = src->segments;
    if (segments < 3) {
        src->segments = 3;
        segments = 3;
    }
    node = dds3RingPolyExpListCreate(segments);
    i = 0;
    work->node = node;
    work->vertexCount = node->vertexCount;
    groups = (s32)node->vertexCount >> 2;
    entry = node->colors;
    for (i = 0; i < groups; i++) {
        u32 second;

        entry[0] = src->firstColor;
        second = src->middleColor;
        entry[1] = entry[2] = second;
        entry[3] = src->lastColor;
        entry += 4;
    }
    effPCPRingPolyExpInit(work);
    work->node->drawKind = src->drawKind;
    return (u8 *)work;
}

u8 *effPCPRingPolyExpCreatePCF(void *args) {
    void *param0;

    param0 = dds3EffectPCPDataGetData(args, 0);
    return effPCPRingPolyExpCreate(param0);
}

u8 *effPCPRingPolyExpCopy(EffPCPBeamParams *src) {
    return effPCPRingPolyExpCreate(src);
}

void effPCPRingPolyExpDelete(EffPCPBeamWork *work) {
    dds3RingPolyExpListDelete(work->node);
    sdfChipFreeDirect(work);
}

/* Grow the beam's visible geometry and fade it over its configured lifetime. */
void effPCPRingPolyExpDraw(EffPCPBeamWork *work) {
    s32 vertexFrames = work->params.vertexGrowthFrames;
    s32 endFrame = vertexFrames;
    s32 frame = work->frame;
    s32 fadeIn = work->params.fadeInFrames;
    s32 fadeOut = work->params.fadeOutFrames;
    f32 t;
    u32 color;
    EffPCPBeamNode *beam;

    if (endFrame < work->params.radiusGrowthFrames) {
        endFrame = work->params.radiusGrowthFrames;
    }
    endFrame += work->params.holdFrames;
    if (frame > endFrame) return;
    if (frame < vertexFrames) {
        u32 groups;
        t = (f32)frame / (f32)vertexFrames;
        groups = (u32)((f32)(work->vertexCount >> 2) * t);
        work->node->vertexCount = groups << 2;
    } else {
        work->node->vertexCount = work->vertexCount;
    }
    if (frame < work->params.radiusGrowthFrames) {
        t = (f32)work->frame / (f32)work->params.radiusGrowthFrames;
        effPCPRingPolyExpSetVertex(
            (work->params.endRadius - work->params.startRadius) *
                t + work->params.startRadius,
            work);
    } else {
        effPCPRingPolyExpSetVertex(work->params.endRadius, work);
    }
    if (frame < fadeIn && fadeIn != 0) {
        t = (f32)frame / (f32)fadeIn;
    } else if (endFrame - frame <= fadeOut && fadeOut != 0) {
        t = (f32)(endFrame - frame) / (f32)fadeOut;
    } else {
        t = 1.0f;
    }
    color = effColorBlend(work->color & 0xFFFFFF, work->color, t);
    beam = work->node;
    beam->color = color;
    SDF_VU0_COPYVECTOR(&beam->position, work->params.position);
    dds3RingPolyExpListDraw(beam);
    work->frame++;
}


void effPCPRingPolyExpSetPosition(void *dst, const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(dst, sourcePosition);
}

void effPCPRingPolyExpSetScale(EffPCPBeamWork *work, f32 value) {
    work->node->scale = value;
}

void effPCPRingPolyExpSetColor(EffPCPBeamWork *work, u32 value) {
    work->color = value;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effPCPRingPolyExpSetRotateMatrix(EffPCPBeamWork *work, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_STORE_MATRIX(work->node->localMatrix);
}

void effPCPRingPolyExpSetDirection(EffPCPBeamWork *work, void *src) {
    VU0_LOAD_MATRIX(src);
    SetRotateMatrixX_B(1.5707963f);
    sdfComposeVuMatrixFromRegisters();
    VU0_STORE_MATRIX(work->node);
}



/* Fill the node's point buffer with four concentric rings (radius, +stepA, +stepB, +stepC) of unit directions, rotated by the VU matrix. */
void effPCPRingPolyExp2SetVertex(EffPCPBeamLargeWork *work, f32 radius) {
    f32 dir[4];
    f32 rowA[4];
    f32 rowB[4];
    f32 rowC[4];
    f32 rowD[4];
    f32 *points;
    f32 angle = 0.0f;
    f32 step;
    f32 r1;
    f32 r2;
    f32 r3;
    s32 n;
    u32 i;

    VEC3_SPLAT(rowA, radius);
    r1 = radius + work->head.radiusStepA;
    VEC3_SPLAT(rowB, r1);
    r2 = r1 + work->head.radiusStepB;
    VEC3_SPLAT(rowC, r2);
    r3 = r2 + work->head.radiusStepC;
    VEC3_SPLAT(rowD, r3);
    n = (s32)work->node->vertexCount >> 2;
    points = work->node->points;
    step = 6.2831851f / (f32)work->head.segments;
    if (work->head.mode == 0) {
        LookAtMatrix(aViewTarget, sdfViewTargetVector, aViewDown);
        ReverseMatrix();
    } else {
        SetRotateMatrixZ(work->rotationA);
        sdfVuLoadSecondaryRotationMatrixY(work->rotationB);
        MultMatrixRevNoArg();
        work->rotationB += work->rotationStep;
        work->rotationStep *= work->head.rotationDecay;
    }
    for (i = 0; i < n; i++) {
        if (work->head.mode == 0) {
            dir[0] = sdfCos(angle);
            dir[1] = sdfSin(angle);
            dir[2] = 0;
        } else {
            dir[0] = sdfCos(angle);
            dir[1] = 0;
            dir[2] = sdfSin(angle);
        }
        dir[3] = 0;
        SDF_VU0_LOADVECTOR(dir);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(rowA);
        SDF_VU0_MULVECTOR();
        SDF_VU0_STOREVECTOR(points);
        SDF_VU0_LOADVECTOR(rowB);
        SDF_VU0_MULVECTOR();
        SDF_VU0_STOREVECTOR(points + 4);
        SDF_VU0_LOADVECTOR(rowC);
        SDF_VU0_MULVECTOR();
        SDF_VU0_STOREVECTOR(points + 8);
        SDF_VU0_LOADVECTOR(rowD);
        SDF_VU0_MULVECTOR();
        SDF_VU0_STOREVECTOR(points + 12);
        angle += step;
        points += 16;
    }
}

void effPCPRingPolyExp2Init(EffPCPBeamLargeWork *work) {
    work->rotationA = work->head.degreesA * 0.017453291f;
    work->rotationB = work->head.degreesB * 0.017453291f;
    work->rotationStep = work->head.stepDegrees * 0.017453291f;
    work->radius = work->head.initialRadius;
    work->radiusStep = work->head.initialRadiusStep;
    effPCPRingPolyExp2SetVertex(work, work->head.initialRadius);
    work->frame = 0;
}

/* The ring geometry, angle preparation and timeline use the same 0x80-byte
   clone, with separate radius and rotation decay inputs. */
u8 *effPCPRingPolyExp2Create(src)
    EffPCPBeamLargeHead *src;
{
    EffPCPBeamLargeWork *work = sdfChipAllocDirect(0x80);
    u32 segments;
    EffPCPBeamNode *node;
    u32 *entry;
    s32 groups;
    u32 i;

    work->head = *src;
    work->color = 0x80808080;
    work->frame = 0;
    segments = src->segments;
    if (segments < 3) {
        src->segments = 3;
        segments = 3;
    }
    node = dds3RingPolyExpListCreate(segments);
    i = 0;
    work->node = node;
    groups = (s32)node->vertexCount >> 2;
    entry = node->colors;
    for (i = 0; i < groups; i++) {
        u32 second;

        entry[0] = src->firstColor;
        second = src->middleColor;
        entry[1] = entry[2] = second;
        entry[3] = src->lastColor;
        entry += 4;
    }
    effPCPRingPolyExp2Init(work);
    work->node->drawKind = src->drawKind;
    return (u8 *)work;
}

u8 *effPCPRingPolyExp2CreatePCF(void *args) {
    void *param0;

    param0 = dds3EffectPCPDataGetData(args, 0);
    return effPCPRingPolyExp2Create(param0);
}

u8 *effPCPRingPolyExp2Copy(EffPCPBeamLargeHead *src) {
    return effPCPRingPolyExp2Create(src);
}

void effPCPRingPolyExp2Delete(EffPCPBeamLargeWork *work) {
    dds3RingPolyExpListDelete(work->node);
    sdfChipFreeDirect(work);
}



/* Advance the ring radius, fade the color and mirror the position/color
   into the clone's beam node. */
void effPCPRingPolyExp2Draw(EffPCPBeamLargeWork *work) {
    s32 frame = work->frame;
    s32 duration = work->head.duration;
    s32 fadeIn = work->head.fadeIn;
    s32 fadeOut = work->head.fadeOut;
    f32 t;
    u32 color;
    EffPCPBeamNode *beam;

    if (duration < frame) {
        return;
    }
    work->radius += work->radiusStep;
    effPCPRingPolyExp2SetVertex(work, work->radius);
    work->radiusStep *= work->head.radiusDecay;
    t = 1.0f;
    if (frame < fadeIn && fadeIn != 0) {
        t = (f32)frame / (f32)fadeIn;
    } else if (duration - frame <= fadeOut && fadeOut != 0) {
        t = (f32)(duration - frame) / (f32)fadeOut;
    }
    color = effColorBlend(work->color & 0xFFFFFF, work->color, t);
    beam = work->node;
    beam->color = color;
    SDF_VU0_COPYVECTOR(&beam->position, work);
    dds3RingPolyExpListDraw(beam);
    work->frame++;
}

void effPCPRingPolyExp2SetPosition(void *dst, const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(dst, sourcePosition);
}

void effPCPRingPolyExp2SetScale(EffPCPBeamLargeWork *work, f32 value) {
    work->node->scale = value;
}

void effPCPRingPolyExp2SetColor(EffPCPBeamLargeWork *work, u32 value) {
    work->color = value;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effPCPRingPolyExp2SetRotateMatrix(EffPCPBeamLargeWork *work, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_STORE_MATRIX(work->node->localMatrix);
}

EffPCPGroupSet *effPCPRadiateThunderCreate(EffPCPGroupHead *header, void **sourceResources) {
    u32 entryCount = header->count;
    struct sdfMemHandle_t *workResource;
    EffPCPGroupSet *groupSet;
    EffPCPGroupEntry *groupEntryCursor;
    u32 index;
    u32 duplicateIndex;

    workResource = sdfMemAllocDirect(sizeof(*groupSet) + entryCount * sizeof(*groupEntryCursor));
    groupSet = (void *)sdfMemLock(workResource);
    groupSet->head = *header;
    groupSet->scale = 1.0f;
    groupSet->color = 0x80808080;
    groupSet->workHandle = workResource;
    groupSet->fragmentLength = header->spawnParams.start[1];
    groupEntryCursor = (EffPCPGroupEntry *)(groupSet + 1);
    groupSet->entries = groupEntryCursor;
    groupSet->duplicates = 0;
    if (sourceResources != NULL) {
        u32 allocationBytes = 4 * entryCount * sizeof(*groupSet->duplicates);
        void **sourceResourceCursor = sourceResources;
        u32 groupByteOffset;
        u32 groupByteWidth;
        u8 *activeGroupFlags;

        index = 0;
        groupSet->duplicateHandle = sdfMemAllocDirect(allocationBytes);
        activeGroupFlags = header->activeGroups;
        groupByteOffset = 0;
        groupByteWidth = entryCount * 4;
        groupSet->duplicates = (effPartHandle_t **)sdfMemLock(groupSet->duplicateHandle);
        memset(groupSet->duplicates, 0, allocationBytes);
        for (; index < 4; index++) {
            effPartHandle_t **destinationHandles = (effPartHandle_t **)((u8 *)groupSet->duplicates + groupByteOffset);

            if (*activeGroupFlags != 0) {
                effPartHandle_t *firstCreatedHandle;

                if (index < 2) {
                    *destinationHandles = dds3EffectPartCreate(EFF_PARAM_WORK_KIND_EFFECT_NODE, *sourceResourceCursor);
                } else {
                    *destinationHandles = dds3EffectPartCreate(EFF_PARAM_WORK_KIND_EXTENDED_WORK_WITH_MATRIX_CALLBACK, *sourceResourceCursor);
                }
                firstCreatedHandle = *destinationHandles;
                for (duplicateIndex = 1; duplicateIndex < entryCount; duplicateIndex++) {
                    destinationHandles++;
                    *destinationHandles = dds3EffectPartCopy(firstCreatedHandle);
                }
            }
            sourceResourceCursor++;
            activeGroupFlags++;
            groupByteOffset += groupByteWidth;
        }
    }
    for (index = 0; index < entryCount; index++) {
        groupEntryCursor->handle = effPCP_ThunderPolyCreate(&header->spawnParams);
        groupEntryCursor->frame = 0;
        groupEntryCursor++;
    }
    return groupSet;
}

EffPCPGroupSet *effPCPRadiateThunderCreatePCF(void *args) {
    EffPCPGroupHead *work;
    void *resources[4];
    u32 i;

    work = dds3EffectPCPDataGetData(args, 0);
    for (i = 0; i < 4; i++) {
        if (work->activeGroups[i] != 0) {
            resources[i] = dds3EffectPCPDataGetData(args, i + 1);
        } else {
            resources[i] = NULL;
        }
    }
    return effPCPRadiateThunderCreate(work, resources);
}

EffPCPGroupSet *effPCPRadiateThunderCopy(EffPCPGroupSet *work) {
    EffPCPGroupSet *groupSet = effPCPRadiateThunderCreate(&work->head, 0);
    u32 groupIndex;
    u32 groupByteOffset;
    u32 entryCount;
    u32 allocationBytes;
    u32 groupByteWidth;
    u8 *activeGroupFlags;
    u32 entryIndex;

    if (work->duplicates != 0) {
        entryCount = work->head.count;
        allocationBytes = entryCount * 16;
        groupByteWidth = entryCount * 4;
        groupIndex = 0;
        activeGroupFlags = work->head.activeGroups;
        groupByteOffset = 0;
        groupSet->duplicateHandle = sdfMemAllocDirect(allocationBytes);
        groupSet->duplicates = (effPartHandle_t **)sdfMemLock(groupSet->duplicateHandle);
        memset(groupSet->duplicates, 0, allocationBytes);
        for (; groupIndex < 4; groupIndex++) {
            effPartHandle_t **destinationHandles = (effPartHandle_t **)((u8 *)groupSet->duplicates + groupByteOffset);

            if (*activeGroupFlags != 0) {
                effPartHandle_t *sourceHandle = *(effPartHandle_t **)(groupByteOffset + (u32)work->duplicates);
                for (entryIndex = 0; entryIndex < entryCount; entryIndex++) {
                    *destinationHandles++ = dds3EffectPartCopy(sourceHandle);
                }
            }
            activeGroupFlags++;
            groupByteOffset += groupByteWidth;
        }
    }
    return groupSet;
}

void effPCPRadiateThunderDelete(EffPCPGroupSet *work) {
    u32 releaseIndex = 0;
    u32 releaseCount = work->head.count;
    EffPCPGroupEntry *fragmentEntryCursor = work->entries;
    effPartHandle_t **duplicateHandleBase;
    effPartHandle_t **duplicate;

    if (releaseCount != 0) {
        do {
            EffThunderFragmentWork *currentHandle = fragmentEntryCursor->handle;
            fragmentEntryCursor++;
            releaseIndex++;
            effPCP_ThunderPolyDelete(currentHandle);
        } while (releaseIndex < releaseCount);
    }
    duplicateHandleBase = work->duplicates;
    releaseCount = releaseCount * 4;
    if (duplicateHandleBase != NULL) {
        duplicate = duplicateHandleBase;
        releaseIndex = 0;
        if (releaseCount != 0) {
            do {
                effPartHandle_t *currentHandle = *duplicate++;
                if (currentHandle != 0) {
                    dds3EffectPartDelete(currentHandle);
                }
                releaseIndex++;
            } while (releaseIndex < releaseCount);
        }
        sdfMemFreeDirect(work->duplicateHandle);
    }
    sdfMemFreeDirect(work->workHandle);
}

/* Randomises entry `index` of a group set: a start delay, a jittered
 * position step per frame and a spread angle step. */
void effPCPRadiateThunderPartInit(EffPCPGroupSet *work, s32 index) {
    EffPCPGroupEntry *entry = work->entries + index;
    s32 spread = work->head.delaySpread;
    f32 scale = work->scale;
    f32 ratio;

    if (spread > 0) {
        entry->frame = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % spread);
    }
    ratio = work->head.startJitter;
    entry->position = work->head.startPosition * (dds3GetRandFloatA(effDefaultRandomState) * ratio + (1.0f - ratio)) * scale;
    ratio = work->head.endJitter;
    entry->positionStep = (work->head.endPosition * (dds3GetRandFloatA(effDefaultRandomState) * ratio + (1.0f - ratio)) * scale - entry->position) / (f32)work->head.framesPerEntry;
    entry->angle = 3.14159265f * 2.0f / (f32)work->head.count * (f32)index;
    ratio = work->head.angularStepJitter;
    entry->angularStep = work->head.angularStep * (dds3GetRandFloatA(effDefaultRandomState) * ratio + (1.0f - ratio));
}

void effPCPRadiateThunderDraw(EffPCPGroupSet *work) {
    f32 point[3];
    f32 origin[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    effPartHandle_t **followers = NULL;
    u32 repeat;
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    u32 count;
    u32 color;
    f32 angularDamping;
    f32 startRadialBlend;
    f32 endRadialBlend;
    EffPCPGroupEntry *entry;
    f32 length;
    u32 i;
    s32 frame;

    count = work->head.count;
    repeat = work->head.repeat;
    duration = work->head.framesPerEntry;
    fadeIn = work->head.fadeIn;
    fadeOut = work->head.fadeOut;
    angularDamping = work->head.angularDamping;
    startRadialBlend = work->head.startRadialBlend;
    endRadialBlend = work->head.endRadialBlend;
    entry = work->entries;
    color = work->color;
    length = work->fragmentLength;
    SDF_VU0_COPYVECTOR(origin, work->head.origin);
    if (duration == 0) {
        return;
    }
    for (i = 0; i < count; i++, entry++) {
        frame = entry->frame;
        if (frame <= duration) {
            if (work->duplicates != NULL) {
                followers = work->duplicates + i;
            }
            if (frame == 0) {
                effPCPRadiateThunderPartInit(work, i);
                frame = entry->frame;
            }
            if (frame >= 0) {
                s32 elapsedFrames = entry->frame;
                f32 angle = entry->angle;
                f32 radius = entry->position;
                f32 cosine;
                f32 sine;
                f32 blend;
                f32 fade;
                u32 fadedColor;
                EffThunderFragmentParams *fragment;
                cosine = sdfCos(angle);
                sine = sdfSin(angle);
                point[0] = origin[0] + cosine * radius;
                point[1] = origin[1];
                point[2] = origin[2] + sine * radius;
                entry->position += entry->positionStep;
                entry->angle += entry->angularStep;
                entry->angularStep *= angularDamping;
                fragment = effPCP_ThunderPolyGetParam(entry->handle);
                blend = startRadialBlend + (endRadialBlend - startRadialBlend) * ((f32)frame / (f32)duration);
                direction[0] = cosine * blend;
                direction[1] = 1.0f - ffabsf(blend);
                direction[2] = sine * blend;
                SDF_VU0_LOADVECTOR(direction);
                SDF_VU0_NORMALIZE();
                SDF_VU0_STOREVECTOR(direction);
                fragment->start[0] = point[0] + direction[0] * length;
                fragment->start[1] = point[1] + direction[1] * length;
                fragment->start[2] = point[2] + direction[2] * length;
                fragment->end[0] = point[0];
                fragment->end[1] = point[1];
                fragment->end[2] = point[2];
                if (elapsedFrames < fadeIn && fadeIn != 0) {
                    fade = (f32)elapsedFrames / (f32)fadeIn;
                } else {
                    s32 remainingFrames = duration - frame;
                    if (remainingFrames <= fadeOut && fadeOut != 0) {
                        fade = (f32)remainingFrames / (f32)fadeOut;
                    } else {
                        fade = 1.0f;
                    }
                }
                fadedColor = effColorBlend(color & 0xFFFFFF, color, fade);
                effPCP_ThunderPolySetColor(entry->handle, fadedColor);
                effPCP_ThunderPolyDraw(entry->handle);
                if (work->duplicates != NULL) {
                    u32 followerIndex;
                    for (followerIndex = 0; followerIndex < ARRAY_COUNT(work->head.activeGroups); followerIndex++) {
                        if (*followers != 0) {
                            dds3EffectPartSetPosition(*followers, fragment->end);
                            dds3EffectPartSetColor(*followers, fadedColor);
                            dds3EffectPartDraw(*followers);
                        }
                        followers += count;
                    }
                }
                frame = entry->frame;
            }
            if (repeat != 0 && frame >= duration) {
                entry->frame = 0;
            } else {
                entry->frame = frame + 1;
            }
        }
    }
}

void effPCPRadiateThunderSetPosition(void *dst, const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(dst, sourcePosition);
}

void effPCPRadiateThunderSetScale(EffPCPGroupSet *work, f32 val) {
    work->scale = val;
}

void effPCPRadiateThunderSetColor(EffPCPGroupSet *work, u32 value) {
    work->color = value;
}

EffPCPSprayWork *effPCPMakkaCreat(void *src) {
    EffPCPSprayWork *work = sdfChipAllocDirect(0x98);
    u32 i;

    work->scale = 1.0f;
    work->color = 0x80808080;
    work->count = 10;
    work->frame = 0;
    for (i = 0; i < work->count; i++) {
        work->handle[i] = 0;
        work->startFrame[i] = i;
        work->rotationY[i] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f * 0.87266457f + 3.14159265f;
    }
    work->handle[0] = dds3EffectPCPDataPartCreate(src, 0);
    return work;
}

EffPCPSprayWork *effPCPMakkaCopy(EffPCPSprayWork *src) {
    EffPCPSprayWork *work = sdfChipAllocDirect(0x98);
    u32 i;

    work->frame = 0;
    work->scale = src->scale;
    work->count = src->count;
    work->color = 0x80808080;
    for (i = 0; i < work->count; i++) {
        work->handle[i] = 0;
        work->startFrame[i] = i;
        work->rotationY[i] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f * 0.87266457f + 3.14159265f;
    }
    work->handle[0] = dds3EffectPartCopy(src->handle[0]);
    return work;
}

void effPCPMakkaDelete(EffPCPSprayWork *work) {
    u32 i;

    for (i = 0; i < work->count; i++) {
        if (work->handle[i] != 0) {
            dds3EffectPartDelete(work->handle[i]);
        }
    }
    sdfChipFreeDirect(work);
}

void effPCPMakkaUpdateNode(sdfModelNode_t *node) {
    sdfModelNode_t *child;

    SDF_VU0_STOREVECTOR(node->scale);
    child = node->children;
    if (child != NULL) {
        do {
            effPCPMakkaUpdateNode(child);
            child = child->next;
        } while (child != node->children);
    }
}


/* Unit quaternion for a half turn around Y; consumed by a quadword VU load. */
extern const f32 D_00414610[4] __attribute__((aligned(16)));

/* Per-frame update: for each of `count` slots, spawn its model on its start frame, orient/scale it, refresh its children and capture the node vectors. */
void effPcpUpdateStaggeredPulseModels(EffPCPSprayWork *work) {
    u32 i = 0;
    u32 count;
    dds3ModelHandle_t *data;
    sdfModelRootNode_t *model;
    sdfMotionCtrlr_t *child;
    sdfModelNode_t *root;
    f32 vec[4];
    s32 j;

    effBTLFieldColorGetVariantSelector();
    count = work->count;
    for (; i < count; i++) {
        if (work->startFrame[i] == work->frame && work->handle[i] == 0) {
            work->handle[i] = dds3EffectPartCopy(work->handle[0]);
        }
        if (work->handle[i] == 0) {
            continue;
        }
        data = effParamWorkGetData(work->handle[i]);
        sdfConvertEulerAnglesToQuaternionVU(0.0f, work->rotationY[i], 0.0f);
        if (effBTLFieldColorUsesActorSelectors() && (((BtlUnit *)effBTLFieldColorGetVariantSelector())->status.flags & 0x400)) {
            SDF_VU0_LOADVECTOR_B(D_00414610);
            effMiscQuatMultiplyVU();
        }
        mdlSetRotate(data);
        vec[3] = 0;
        VEC3_SPLAT(vec, work->scale * work->scale);
        SDF_VU0_LOADVECTOR(vec);
        mdlSetScale(data);
        mdlSetColor(data, work->color);
        SDF_VU0_LOADVECTOR(work->pos);
        mdlSetPosition(data);
        for (j = 0; j != 4; j++) {
            child = data->slots[j];
            if (child != NULL && child->state) {
                sdfMotionNext(child);
            }
        }
        if (!(data->flags & MDL_SKIP_TRANSFORMS)) {
            model = data->inner;
            root = ((sdfModelNode_t **)model->list->buffer)[0];
            VEC3_SPLAT(vec, 1.0f / work->scale);
            SDF_VU0_LOADVECTOR(vec);
            effPCPMakkaUpdateNode(root);
            sdfModelEvalTm(model);
            sdfBuildModelPacketsVia(sdfModelBuildNodeResourcePackets, kwlnSceneSurfaceSets[10], model);
        }
    }
    work->frame++;
}

void effPCPMakkaSetPosition(void *dst, const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(dst, sourcePosition);
}

void effPCPMakkaSetColor(EffPCPSprayWork *work, u32 value) {
    work->color = value;
}

void effPCPMakkaSetScale(EffPCPSprayWork *work, f32 val) {
    work->scale = val;
}

/* Clone the source header (a longer one with 0x18-byte entries), then give every entry one event and a random negative start delay. */
EffPCPDriftEventWork *effPCPRadiateBEDCreate(EffPCPDriftEventParams *src, void *params) {
    u32 count = src->count;
    struct sdfMemHandle_t *handle = sdfMemAllocDirect(
        count * sizeof(EffPCPDriftEventRow) + sizeof(EffPCPDriftEventWork));
    EffPCPDriftEventWork *work = (void *)sdfMemLock(handle);
    EffPCPEventPlace place;
    EffPCPDriftEventRow *entry;
    s32 life;
    u32 i;

    work->params = *src;
    entry = (EffPCPDriftEventRow *)(work + 1);
    work->handle = handle;
    work->color = 0x80808080;
    work->unk10C = 1;
    work->entries = entry;
    work->scale = 1.0f;
    work->owner = effEveBEDCreate(params);
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
    life = work->params.delaySpread;
    for (i = 0; i < count; i++) {
        entry->event = effEveBEDCutCreate(work->owner, 2, &place);
        if (life > 0) {
            entry->frame = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % life);
        } else {
            entry->frame = 0;
        }
        entry++;
    }
    return work;
}

EffPCPDriftEventWork *effPCPRadiateBEDCreatePCF(void *data) {
    void *work0;
    void *work1;

    work0 = dds3EffectPCPDataGetData(data, 0);
    work1 = dds3EffectPCPDataGetData(data, 1);
    return effPCPRadiateBEDCreate(work0, work1);
}

EffPCPDriftEventWork *effPCPRadiateBEDCopy(EffPCPDriftEventWork *src) {
    u32 count = src->params.count;
    struct sdfMemHandle_t *handle = sdfMemAllocDirect(
        count * sizeof(EffPCPDriftEventRow) + sizeof(EffPCPDriftEventWork));
    EffPCPDriftEventWork *work = (void *)sdfMemLock(handle);
    EffPCPEventPlace place;
    EffPCPDriftEventRow *entry;
    s32 life;
    u32 i;

    work->params = src->params;
    entry = (EffPCPDriftEventRow *)(work + 1);
    work->handle = handle;
    work->color = 0x80808080;
    work->unk10C = 0;
    work->entries = entry;
    work->scale = 1.0f;
    work->owner = src->owner;
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
    life = work->params.delaySpread;
    for (i = 0; i < count; i++) {
        entry->event = effEveBEDCutCreate(src->owner, 2, &place);
        if (life > 0) {
            entry->frame = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % life);
        } else {
            entry->frame = 0;
        }
        entry++;
    }
    return work;
}

void effPCPRadiateBEDDelete(EffPCPDriftEventWork *work) {
    u32 i = 0;
    u32 count = work->params.count;
    EffPCPDriftEventRow *entry = work->entries;

    if (count != 0) {
        do {
            effEveBEDCutDelete(entry->event);
            entry++;
            i++;
        } while (i < count);
    }
    if (work->owner->active == 0) {
        effEveBEDDelete(work->owner);
    }
    sdfMemFreeDirect(work->handle);
}


void effPCPRadiateBEDPartInit(EffPCPDriftEventWork *work, s32 index) {
    EffPCPDriftEventRow *slot;
    f32 spread;
    f32 scale;

    slot = &work->entries[index];
    scale = work->scale;
    spread = work->params.startJitter;
    slot->position = work->params.startPosition * (dds3GetRandFloatA(effDefaultRandomState) * spread + (1.0f - spread)) * scale;
    spread = work->params.endJitter;
    slot->positionStep = (work->params.endPosition * (dds3GetRandFloatA(effDefaultRandomState) * spread + (1.0f - spread)) * scale - slot->position) / (f32)work->params.duration;
    slot->angle = 6.2831852f / (f32)work->params.count * (f32)index;
    spread = work->params.angleJitter;
    slot->angleStep = work->params.angleStep * (dds3GetRandFloatA(effDefaultRandomState) * spread + (1.0f - spread));
}


/* vu0 routine: place delayed drift events along expanding rotating rings. */
void effPCPRadiateBEDDraw(EffPCPDriftEventWork *work) {
    EffPCPEventPlace place;
    f32 base[4] __attribute__((aligned(16)));
    u32 count = work->params.count;
    s32 duration = work->params.duration;
    s32 fadeIn = work->params.fadeIn;
    s32 fadeOut = work->params.fadeOut;
    f32 angleDecay = work->params.angleDecay;
    EffPCPDriftEventRow *entry = work->entries;
    u32 color = work->color;
    u32 i;
    s32 frame;

    SDF_VU0_COPYVECTOR(base, work->params.position);
    if (duration == 0) {
        return;
    }
    place.payload[4] = 0;
    place.payload[5] = 0;
    place.payload[6] = 0;
    place.payload[7] = 1.0f;
    place.payload[8] = 100.0f;
    place.payload[9] = 100.0f;
    place.payload[10] = 1.0f;
    for (i = 0; i < count; i++, entry++) {
        frame = entry->frame;

        if (frame <= duration) {
            if (frame == 0) {
                effPCPRadiateBEDPartInit(work, i);
                frame = entry->frame;
            }
            if (frame >= 0) {
                f32 angle = entry->angle;
                f32 radius = entry->position;
                f32 cosine;
                f32 sine;
                f32 t;

                cosine = sdfCos(angle);
                sine = sdfSin(angle);
                place.payload[0] = base[0] + cosine * radius;
                place.payload[1] = base[1];
                place.payload[2] = base[2] + sine * radius;
                entry->position += entry->positionStep;
                entry->angle += entry->angleStep;
                entry->angleStep *= angleDecay;
                if (frame < fadeIn && fadeIn != 0) {
                    t = (f32)frame / (f32)fadeIn;
                } else if (duration - frame <= fadeOut && fadeOut != 0) {
                    t = (f32)(duration - frame) / (f32)fadeOut;
                } else {
                    t = 1.0f;
                }
                place.color = effColorBlend(color & 0xFFFFFF, color, t);
                effEveBEDCutSetParams(entry->event, &place);
                effEventUpdateEffectParameters(entry->event);
                frame = entry->frame;
            }
            entry->frame = frame + 1;
        }
    }
}

void effPCPRadiateBEDSetPosition(void *dst, const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(dst, sourcePosition);
}

void effPCPRadiateBEDSetScale(EffPCPDriftEventWork *work, f32 val) {
    work->scale = val;
}

void effPCPRadiateBEDSetColor(EffPCPDriftEventWork *work, u32 value) {
    work->color = value;
}

/* Clone the source effect header, then give every entry two events (placed at the unit scale) and a random negative start frame. */
EffPCPPairedEventWork *effPCPRadiateTBCreate(EffPCPPairedEventParams *src, void *paramsA, void *paramsB) {
    u32 count = src->count;
    struct sdfMemHandle_t *handle;
    EffPCPPairedEventWork *work;
    EffPCPEventPlace place;
    EffPCPPairedEventRow *entry;
    s32 life;
    u32 i;

    handle = sdfMemAllocDirect(sizeof(*work) + count * sizeof(*entry));
    work = (void *)sdfMemLock(handle);
    work->params = *src;
    entry = (EffPCPPairedEventRow *)(work + 1);
    work->handle = handle;
    work->entries = entry;
    work->color = 0x80808080;
    if (work->params.delaySpread <= 0) {
        work->params.delaySpread = 1;
    }
    work->unk98 = 1;
    work->ownerA = effEveBEDCreate(paramsA);
    work->ownerB = effEveBEDCreate(paramsB);
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
    life = work->params.delaySpread;
    for (i = 0; i < count; i++) {
        entry->fragment = effPCP_ThunderPolyCreate(&src->fragmentParams);
        entry->eventA = effEveBEDCutCreate(work->ownerA, 2, &place);
        entry->eventB = effEveBEDCutCreate(work->ownerB, 2, &place);
        entry->frame = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % life);
        entry++;
    }
    return work;
}

EffPCPPairedEventWork *effPCPRadiateTBCreatePCF(void *data) {
    void *work0;
    void *work1;
    void *work2;

    work0 = dds3EffectPCPDataGetData(data, 0);
    work1 = dds3EffectPCPDataGetData(data, 1);
    work2 = dds3EffectPCPDataGetData(data, 2);
    return effPCPRadiateTBCreate(work0, work1, work2);
}

EffPCPPairedEventWork *effPCPRadiateTBCopy(EffPCPPairedEventWork *src) {
    u32 count = src->params.count;
    struct sdfMemHandle_t *handle;
    EffPCPPairedEventWork *work;
    EffPCPEventPlace place;
    EffPCPPairedEventRow *entry;
    s32 life;
    u32 i;

    handle = sdfMemAllocDirect(sizeof(*work) + count * sizeof(*entry));
    work = (void *)sdfMemLock(handle);
    work->params = src->params;
    entry = (EffPCPPairedEventRow *)(work + 1);
    work->handle = handle;
    work->entries = entry;
    work->color = 0x80808080;
    work->unk98 = 0;
    work->ownerA = src->ownerA;
    work->ownerB = src->ownerB;
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
    life = work->params.delaySpread;
    for (i = 0; i < count; i++) {
        entry->fragment = effPCP_ThunderPolyCreate(&src->params.fragmentParams);
        entry->eventA = effEveBEDCutCreate(src->ownerA, 2, &place);
        entry->eventB = effEveBEDCutCreate(src->ownerB, 2, &place);
        entry->frame = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % life);
        entry++;
    }
    return work;
}

void effPCPRadiateTBDelete(EffPCPPairedEventWork *work) {
    u32 i = 0;
    u32 count = work->params.count;
    EffPCPPairedEventRow *entry = work->entries;

    if (count != 0) {
        do {
            effEveBEDCutDelete(entry->eventA);
            effEveBEDCutDelete(entry->eventB);
            effPCP_ThunderPolyDelete(entry->fragment);
            entry++;
            i++;
        } while (i < count);
    }
    if (work->ownerA->active == 0) {
        effEveBEDDelete(work->ownerA);
    }
    if (work->ownerB->active == 0) {
        effEveBEDDelete(work->ownerB);
    }
    sdfMemFreeDirect(work->handle);
}


/* Randomises slot `index`: phase spread evenly around a full turn, jittered
 * radius, and a tiltStep whose sign is picked at random. */
void effPCPRadiateTBPartInit(EffPCPPairedEventWork *work, s32 index) {
    EffPCPPairedEventRow *slot;
    f32 spread;

    slot = &work->entries[index];
    slot->phase = (3.14159265f * 2.0f) / work->params.count * index;
    spread = work->params.radiusJitter;
    slot->radius = work->params.baseRadius * (dds3GetRandFloatA(effDefaultRandomState) * spread + (1.0f - spread));
    slot->tilt = 0;
    spread = work->params.tiltJitter;
    if (dds3GetRandA((EffRandState *)effDefaultRandomState) & 1) {
        slot->tiltStep = work->params.baseTiltStep * (dds3GetRandFloatA(effDefaultRandomState) * spread + (1.0f - spread));
    } else {
        slot->tiltStep = -work->params.baseTiltStep * (dds3GetRandFloatA(effDefaultRandomState) * spread + (1.0f - spread));
    }
}

extern const f32 effPCPRadiateTBParam[4] __attribute__((aligned(16)));

/* vu0 routine: rotate a fragment around the view and place events at both ends. */
void effPCPRadiateTBDraw(EffPCPPairedEventWork *work) {
    EffPCPEventPlace place;
    f32 origin[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    f32 tiltAxis[4] __attribute__((aligned(16)));
    f32 viewAxis[4] __attribute__((aligned(16)));
    u32 repeat;
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    s32 delaySpread;
    u32 count;
    u32 color;
    EffPCPPairedEventRow *entry;
    u32 i;
    s32 frame;

    count = work->params.count;
    repeat = work->params.repeat;
    duration = work->params.duration;
    fadeIn = work->params.fadeIn;
    fadeOut = work->params.fadeOut;
    delaySpread = work->params.delaySpread;
    entry = work->entries;
    color = work->color;
    SDF_VU0_COPYVECTOR(origin, work->params.origin);
    if (duration == 0) {
        return;
    }
    place.payload[4] = 0;
    place.payload[5] = 0;
    place.payload[6] = 0;
    place.payload[7] = 1.0f;
    place.payload[8] = 100.0f;
    place.payload[9] = 100.0f;
    place.payload[10] = 1.0f;
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(sdfViewTargetVector);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(viewAxis);
    for (i = 0; i < count; i++, entry++) {
        frame = entry->frame;
        if (frame <= duration) {
            if (frame == 0) {
                effPCPRadiateTBPartInit(work, i);
                frame = entry->frame;
            }
            if (frame >= 0) {
                s32 elapsedFrames = frame;
                EffThunderFragmentParams *fragment;
                f32 radius;
                f32 fade;
                u32 fadedColor;

                fragment = effPCP_ThunderPolyGetParam(entry->fragment);
                SetRotateMatrixAxis(entry->phase, (const struct RwV3d *)viewAxis);
                SDF_VU0_LOADVECTOR(effPCPRadiateTBParam);
                SDF_VU0_LOADVECTOR_B(viewAxis);
                SDF_VU0_OUTERPRODUCT();
                SDF_VU0_NORMALIZE();
                SDF_VU0_APPLYMATRIX_XYZ();
                SDF_VU0_STOREVECTOR(direction);
                SDF_VU0_LOADVECTOR_B(effPCPRadiateTBParam);
                SDF_VU0_OUTERPRODUCT();
                SDF_VU0_NORMALIZE();
                /* This SDK-produced axis is consumed only by the external builder. */
                SDF_VU0_STOREVECTOR_NOMEM(tiltAxis);
                radius = entry->radius;
                SetRotateMatrixAxis(entry->tilt, (const struct RwV3d *)tiltAxis);
                SDF_VU0_LOADVECTOR(direction);
                SDF_VU0_APPLYMATRIX_XYZ();
                SDF_VU0_STOREVECTOR(direction);
                fragment->start[0] = origin[0] + direction[0] * radius;
                fragment->start[1] = origin[1] + direction[1] * radius;
                fragment->start[2] = origin[2] + direction[2] * radius;
                fragment->end[0] = origin[0];
                fragment->end[1] = origin[1];
                fragment->end[2] = origin[2];
                entry->tilt += entry->tiltStep;
                if (elapsedFrames < fadeIn && fadeIn != 0) {
                    fade = (f32)elapsedFrames / (f32)fadeIn;
                } else {
                    s32 remainingFrames = duration - frame;
                    if (remainingFrames <= fadeOut && fadeOut != 0) {
                        fade = (f32)remainingFrames / (f32)fadeOut;
                    } else {
                        fade = 1.0f;
                    }
                }
                fadedColor = effColorBlend(color & 0xFFFFFF, color, fade);
                effPCP_ThunderPolySetColor(entry->fragment, fadedColor);
                effPCP_ThunderPolyDraw(entry->fragment);
                place.color = fadedColor;
                SDF_VU0_COPYVECTOR(place.payload, fragment->end);
                effEveBEDCutSetParams(entry->eventA, &place);
                effEventUpdateEffectParameters(entry->eventA);
                SDF_VU0_COPYVECTOR(place.payload, fragment->start);
                effEveBEDCutSetParams(entry->eventB, &place);
                effEventUpdateEffectParameters(entry->eventB);
                frame = entry->frame;
            }
            if (repeat != 0 && frame >= duration) {
                entry->frame = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % delaySpread);
            } else {
                entry->frame = frame + 1;
            }
        }
    }
}

void effPCPRadiateTBSetPosition(void *dst, const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(dst, sourcePosition);
}

void effPCPRadiateTBSetColor(EffPCPPairedEventWork *work, u32 value) {
    work->color = value;
}

/* Clone the source header, then give every entry one event (placed at the unit scale) and a random negative start frame. */
EffPCPSpawnRangeWork *effPCPSpiralBEDCreate(EffPCPSpawnRangeParams *src, void *params) {
    u32 count = src->count;
    struct sdfMemHandle_t *handle;
    EffPCPSpawnRangeWork *work;
    EffPCPEventPlace place;
    EffPCPSpawnRangeEventRow *entry;
    s32 life;
    u32 i;

    handle = sdfMemAllocDirect(sizeof(*work) + count * sizeof(*entry));
    work = (void *)sdfMemLock(handle);
    work->params = *src;
    entry = (EffPCPSpawnRangeEventRow *)(work + 1);
    work->handle = handle;
    work->color = 0x80808080;
    work->unk9C = 1;
    work->entries = entry;
    work->scale = 1.0f;
    work->owner = effEveBEDCreate(params);
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
    life = work->params.delaySpread;
    for (i = 0; i < count; i++) {
        entry->event = effEveBEDCutCreate(work->owner, 2, &place);
        if (life > 0) {
            entry->frame = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % life);
        } else {
            entry->frame = 0;
        }
        entry++;
    }
    return work;
}

EffPCPSpawnRangeWork *effPCPSpiralBEDCreatePCF(void *data) {
    void *work0;
    void *work1;

    work0 = dds3EffectPCPDataGetData(data, 0);
    work1 = dds3EffectPCPDataGetData(data, 1);
    return effPCPSpiralBEDCreate(work0, work1);
}

EffPCPSpawnRangeWork *effPCPSpiralBEDCopy(EffPCPSpawnRangeWork *src) {
    u32 count = src->params.count;
    struct sdfMemHandle_t *handle;
    EffPCPSpawnRangeWork *work;
    EffPCPEventPlace place;
    EffPCPSpawnRangeEventRow *entry;
    s32 life;
    u32 i;

    handle = sdfMemAllocDirect(sizeof(*work) + count * sizeof(*entry));
    work = (void *)sdfMemLock(handle);
    work->params = src->params;
    entry = (EffPCPSpawnRangeEventRow *)(work + 1);
    work->handle = handle;
    work->color = 0x80808080;
    work->unk9C = 0;
    work->entries = entry;
    work->scale = 1.0f;
    work->owner = src->owner;
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
    life = work->params.delaySpread;
    for (i = 0; i < count; i++) {
        entry->event = effEveBEDCutCreate(work->owner, 2, &place);
        if (life > 0) {
            entry->frame = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % life);
        } else {
            entry->frame = 0;
        }
        entry++;
    }
    return work;
}


void effPCPSpiralBEDDelete(EffPCPSpawnRangeWork *work) {
    u32 i = 0;
    u32 count = work->params.count;
    EffPCPSpawnRangeEventRow *entry = work->entries;

    if (count != 0) {
        do {
            effEveBEDCutDelete(entry->event);
            entry++;
            i++;
        } while (i < count);
    }
    if (work->owner->active == 0) {
        effEveBEDDelete(work->owner);
    }
    sdfMemFreeDirect(work->handle);
}


/* Randomises spawn slot `index` inside the ranges held by the work. */
void effPCPSpiralBEDPartInit(EffPCPSpawnRangeWork *work, s32 index) {
    EffPCPSpawnRangeEventRow *slot;
    f32 spread;
    f32 scale;

    slot = &work->entries[index];
    scale = work->scale;
    slot->frame = 0;
    slot->height = -work->params.startHeightRange * dds3GetRandFloatA(effDefaultRandomState);
    spread = work->params.heightJitter;
    slot->heightStep = work->params.initialHeightStep * (dds3GetRandFloatA(effDefaultRandomState) * spread + (1.0f - spread));
    slot->angle = dds3GetRandFloatA(effDefaultRandomState) * (3.14159265f * 2.0f);
    spread = work->params.angularJitter;
    slot->angularStep = work->params.initialAngularStep * (dds3GetRandFloatA(effDefaultRandomState) * spread + (1.0f - spread));
    spread = work->params.startJitter;
    slot->position = work->params.startPosition * (dds3GetRandFloatA(effDefaultRandomState) * spread + (1.0f - spread)) * scale;
    spread = work->params.endJitter;
    slot->positionStep = (work->params.endPosition * (dds3GetRandFloatA(effDefaultRandomState) * spread + (1.0f - spread)) * scale - slot->position) / (f32)work->params.duration;
}



/* vu0 routine: move delayed radial events, apply damping, then fade and draw. */
void effPCPSpiralBEDDraw(EffPCPSpawnRangeWork *work) {
    EffPCPEventPlace place;
    f32 origin[4] __attribute__((aligned(16)));
    u32 count = work->params.count;
    s32 duration = work->params.duration;
    s32 fadeIn = work->params.fadeIn;
    s32 fadeOut = work->params.fadeOut;
    f32 angularDamping = work->params.angularDamping;
    f32 heightDamping = work->params.heightDamping;
    EffPCPSpawnRangeEventRow *entry = work->entries;
    u32 color = work->color;
    u32 i;
    s32 frame;

    SDF_VU0_COPYVECTOR(origin, work->params.origin);
    if (duration == 0) {
        return;
    }
    place.payload[4] = 0;
    place.payload[5] = 0;
    place.payload[6] = 0;
    place.payload[7] = 1.0f;
    place.payload[8] = 100.0f;
    place.payload[9] = 100.0f;
    place.payload[10] = 1.0f;
    for (i = 0; i < count; i++, entry++) {
        frame = entry->frame;
        if (frame <= duration) {
            if (frame == 0) {
                effPCPSpiralBEDPartInit(work, i);
                effEveBEDCutSetScale(entry->event, work->scale);
                frame = entry->frame;
            }
            if (frame >= 0) {
                f32 angle = entry->angle;
                f32 radius = entry->position;
                f32 cosine;
                f32 sine;
                f32 fade;

                cosine = sdfCos(angle);
                sine = sdfSin(angle);
                place.payload[0] = origin[0] + cosine * radius;
                place.payload[1] = origin[1] + entry->height;
                place.payload[2] = origin[2] + sine * radius;
                entry->height += entry->heightStep;
                entry->position += entry->positionStep;
                entry->angle += entry->angularStep;
                entry->heightStep *= heightDamping;
                entry->angularStep *= angularDamping;
                if (frame < fadeIn && fadeIn != 0) {
                    fade = (f32)frame / (f32)fadeIn;
                } else if (duration - frame <= fadeOut && fadeOut != 0) {
                    fade = (f32)(duration - frame) / (f32)fadeOut;
                } else {
                    fade = 1.0f;
                }
                place.color = effColorBlend(color & 0xFFFFFF, color, fade);
                effEveBEDCutSetParams(entry->event, &place);
                effEventUpdateEffectParameters(entry->event);
                frame = entry->frame;
            }
            entry->frame = frame + 1;
        }
    }
}

void effPCPSpiralBEDSetPosition(void *dst, const f32 sourcePosition[4]) {
    SDF_VU0_COPYVECTOR(dst, sourcePosition);
}

void effPCPSpiralBEDSetScale(EffPCPSpawnRangeWork *work, f32 val) {
    work->scale = val;
}

void effPCPSpiralBEDSetColor(EffPCPSpawnRangeWork *work, u32 value) {
    work->color = value;
}



void effPCPHelperInit(EffPCPMapEventWork *work) {
    EffPCPEventPlace place;
    u32 i = 0;
    u32 count;
    dds3ModelHandle_t *model;
    EffPCPMapEventEntry *entry;

    model = effParamWorkGetData(work->modelResource);
    model->first->frameStep = work->params.frameStep;
    mdlSingleMotion(model, 0, 0);
    work->frameLimit = model->first->frameCount;
    count = sdfModelGetMarkPosCount(model->inner);
    work->count = count;
    work->entriesHandle = sdfMemAllocDirect(count << 5);
    entry = (void *)sdfMemLock(work->entriesHandle);
    work->entries = entry;
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
    for (; i < count; i++) {
        entry->hasMoved = 0;
        entry->age = 0;
        entry->frameLimit = 0;
        entry->event = effEveBEDCutCreate(work->owner, 2, &place);
        entry++;
    }
}



/* Allocate an event work: copy the parameter head, clear the links, then create the resource and owner from the optional parameters. */
EffPCPMapEventWork *effPCPHelperBEDCreate(EffPCPEventParamHead *head, void *resourceParams, void *ownerParams) {
    EffPCPMapEventWork *work = sdfChipAllocDirect(0x40);

    work->params = *head;
    work->color = 0x80808080;
    work->scale = 1.0f;
    work->owner = 0;
    work->entries = 0;
    work->modelResource = 0;
    work->frame = 0;
    if (resourceParams != 0) {
        work->modelResource = dds3EffectPartCreate(EFF_PARAM_WORK_KIND_VIEWER_CONTEXT, resourceParams);
    }
    if (ownerParams != 0) {
        work->owner = effEveBEDCreate(ownerParams);
    }
    if (work->owner != 0) {
        effPCPHelperInit(work);
    }
    return work;
}

EffPCPMapEventWork *effPCPHelperBEDCreatePCF(void *data) {
    void *work0;
    void *work1;
    void *work2;

    work0 = dds3EffectPCPDataGetData(data, 0);
    work1 = dds3EffectPCPDataGetData(data, 1);
    work2 = dds3EffectPCPDataGetData(data, 2);
    return effPCPHelperBEDCreate(work0, work1, work2);
}


EffPCPMapEventWork *effPCPHelperBEDCopy(EffPCPMapEventWork *src) {
    EffPCPMapEventWork *work;

    work = effPCPHelperBEDCreate(&src->params, 0, 0);
    work->modelResource = dds3EffectPartCopy(src->modelResource);
    work->owner = src->owner;
    effPCPHelperInit(work);
    return work;
}


void effPCPHelperBEDDelete(EffPCPMapEventWork *work) {
    u32 i = 0;
    u32 count;
    EffPCPMapEventEntry *entry;

    dds3EffectPartDelete(work->modelResource);
    count = work->count;
    entry = work->entries;
    if (count != 0) {
        do {
            u32 event = (u32)entry->event;
            entry++;
            if (event != 0) {
                effEveBEDCutDelete((void *)event);
            }
            i++;
        } while (i < count);
    }
    if (work->owner->active == 0) {
        effEveBEDDelete(work->owner);
    }
    sdfMemFreeDirect(work->entriesHandle);
    sdfChipFreeDirect(work);
}


/* vu0 routine: latch map-node motion, then place and fade each event. */
void effPCPHelperBEDDraw(EffPCPMapEventWork *work) {
    EffPCPEventPlace place;
    f32 position[4] __attribute__((aligned(16)));
    f32 initialPosition[4] __attribute__((aligned(16)));
    f32 scale[4] __attribute__((aligned(16)));
    s32 frame = work->frame;
    s32 frameLimit = work->frameLimit;
    sdfModelRootNode_t *mapModel;
    s32 fadeIn;
    s32 fadeOut;
    u32 color;
    u32 count;
    u32 i;
    dds3ModelHandle_t *model;
    EffPCPMapEventEntry *entry;
    f32 distance;
    f32 t;

    VEC3_SPLAT(scale, work->scale);
    scale[3] = 0;
    place.payload[4] = 0;
    place.payload[5] = 0;
    place.payload[6] = 0;
    place.payload[7] = 1.0f;
    place.payload[3] = 0;
    place.payload[8] = 100.0f;
    place.payload[9] = 100.0f;
    place.payload[10] = 1.0f;
    if (frameLimit >= frame) {
        model = effParamWorkGetData(work->modelResource);
        mapModel = model->inner;
        SDF_VU0_LOADVECTOR(work->params.position);
        mdlSetPosition(model);
        SDF_VU0_LOADVECTOR(scale);
        mdlSetScale(model);
        mdlLink(model, kwlnSceneSurfaceSets[10]);
        count = work->count;
        fadeIn = work->params.fadeIn;
        fadeOut = work->params.fadeOut;
        color = work->color;
        entry = work->entries;
        for (i = 0; i < count; i++, entry++) {
            if (frame == 0) {
                effEveBEDCutSetScale(entry->event, work->scale);
            }
            if (frame >= 0) {
                sdfModelGetMarkPos(mapModel, i);
                SDF_VU0_STOREVECTOR(position);
                if (frame == 0) {
                    entry->initialPosition[0] = position[0];
                    entry->initialPosition[1] = position[1];
                    entry->initialPosition[2] = position[2];
                    entry->initialPosition[3] = 0;
                } else {
                    s32 age;
                    s32 ageLimit;

                    if (entry->hasMoved == 0) {
                        initialPosition[0] = entry->initialPosition[0];
                        initialPosition[1] = entry->initialPosition[1];
                        initialPosition[2] = entry->initialPosition[2];
                        initialPosition[3] = 0;
                        SDF_VU0_LOADVECTOR(position);
                        SDF_VU0_LOADVECTOR_B(initialPosition);
                        SDF_VU0_SUBVECTOR();
                        SDF_VU0_GETLENGTH(distance);
                        if (distance > 1.0f) {
                            entry->frameLimit = frameLimit - frame;
                            entry->hasMoved = 1;
                        }
                    }
                    age = entry->age;
                    ageLimit = entry->frameLimit;
                    if (entry->hasMoved && age < ageLimit) {
                        if (age < fadeIn && fadeIn != 0) {
                            t = (f32)age / (f32)fadeIn;
                        } else if (frameLimit - frame <= fadeOut && fadeOut != 0) {
                            t = (f32)(ageLimit - age) / (f32)fadeOut;
                        } else {
                            t = 1.0f;
                        }
                        place.color = effColorBlend(color & 0xFFFFFF, color, t);
                        place.payload[0] = position[0];
                        place.payload[1] = position[1];
                        place.payload[2] = position[2];
                        effEveBEDCutSetParams(entry->event, &place);
                        effEventUpdateEffectParameters(entry->event);
                        entry->age++;
                    }
                }
            }
        }
        work->frame++;
    }
}

const f32 D_00414610[4] __attribute__((aligned(16))) = {0.0f, 1.0f, 0.0f, 0.0f};

INCLUDE_SDATA(const s32, "effect/effPCPMisc", D_00436438);

INCLUDE_SDATA(const s32, "effect/effPCPMisc", D_0043643C);

INCLUDE_SDATA(const s32, "effect/effPCPMisc", D_0043643D);
