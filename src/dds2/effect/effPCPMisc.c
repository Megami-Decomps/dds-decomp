#include "sdk_ps2.h"
#include "common.h"
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




extern u8 effExpandingThunderFadeParams[];


extern u8 effGrowingThunderFadeParams[];





extern u8 effShortThunderFadeParams[];

extern u8 effScalingThunderFadeParams[];


extern s8 D_0043643C;


extern u8 effMuzzleThunderRayParams[];

extern u8 effCyanBlueRayPairParams[];

extern u8 effWhiteYellowRayPairParams[];

extern u8 effYellowOrangeRayPairParams[];

extern u8 effBrownRedRayPairParams[];

extern u8 effOrangeVioletRayPairParams[];

extern EffPCPSharedTrail *effPcpSharedTrailWork;

/* Every compact timeline precedes one variant-specific SDK parameter payload.
   Respawn copies that payload only, not its live resource/allocation tail. */
typedef EffBlurTemplateBody EffPCPTexturedBlurParams;



typedef struct {
    EffPCPCompactTimelineParams timeline;
    EffResourceRectParams res;
} EffPCPCompactRectParams;

typedef struct {
    EffPCPCompactTimelineParams timeline;
    EffPCPTexturedBlurParams res;
} EffPCPCompactTexturedBlurParams;

typedef struct {
    EffPCPCompactTimelineParams timeline;
    EffBlurScatterParams res;
} EffPCPCompactScatterParams;

typedef struct {
    EffPCPCompactTimelineParams timeline;
    EffBlurScaleParams res;
} EffPCPCompactScaleParams;


extern void effBlurSecondInitSlots(EffBlurScaleWork *work);
extern void effBlurStepScaleSlotsAndDraw(EffBlurScaleWork *work);

extern u8 effOffsetMuzzleThunderRayParams[];

extern u8 effCyanBlueRaySingleParams[];

extern u8 effWhiteYellowRaySingleParams[];

extern u8 effYellowOrangeRaySingleParams[];

extern u8 effBrownRedRaySingleParams[];

extern u8 effOrangeVioletRaySingleParams[];

struct EffPCPBeamWork;
extern void effPcpBuildConcentricBeamVertices(f32 value, struct EffPCPBeamWork *work);


extern void sdfVuLoadSecondaryRotationMatrixX(f32 angle);

extern void effPcpStaggerRerollSlot(EffPCPStaggered *work, s32 index);

extern void effPcpDelayedPairsRerollSlot(EffPCPDelayedPairs *work, s32 index);



/* Boss effect work: settable param plus two handles released on free. */
extern void effTwinEffectRerollSlot(EffPCPTwinWork *work, s32 slotIndex);



/* Three view-relative offsets and stagger thresholds for seven handles per group. */
typedef struct {
    f32 origin[4];
    f32 offset[3][2];
    s32 groupStartFrame[3];
    s32 handleDelay[7];
} EffPCPTripleParams;

typedef struct EffPCPTripleWork {
    EffPCPTripleParams head;
    s32 frame;
    u32 color;
    struct EffNode *handles[21];
} EffPCPTripleWork;

typedef char EffPCPTripleParamsSizeCheck[sizeof(EffPCPTripleParams) == 0x50 ? 1 : -1];
typedef char EffPCPTripleWorkSizeCheck[sizeof(EffPCPTripleWork) == 0xAC ? 1 : -1];

EffPCPTripleWork *effPcpTripleHandleCreate(EffPCPTripleParams *params, EffNodeDescriptor **descriptors);

extern void effPcpSpanSetHeadVector(void *dst, void *src);






extern EffRandState effDefaultRandomState[];


extern EffRandState effDefaultRandomState[];

typedef struct EffPCPSprayWork {
    f32 pos[4];       /* 0x00 vector loaded by the pulse updater */
    f32 scale;        /* 0x10 */
    u32 color;        /* 0x14 */
    u32 count;        /* 0x18 particle count */
    s32 frame;        /* 0x1C current update frame */
    s32 startFrame[10]; /* 0x20 */
    f32 rotationY[10];  /* 0x48 random start angles */
    EffParamWork *handle[10];   /* 0x70 */
} EffPCPSprayWork; /* 0x98 */

typedef char EffPCPSprayWork_size_must_be_0x98[
    (sizeof(EffPCPSprayWork) == 0x98) ? 1 : -1];



extern s32 sdfLoadMapRecordPositionVector(SdfModel *model, s32 value);

extern struct SdfPoolNode *D_00380828[4];















/* Effect initializers implemented in assembly below (effScatterCreateRadialWork lives in
   another unit). Each is entered with and without spawn arguments, so they
   are declared unchecked. */
extern void *effPcpCreateDelayedEventEntries();

typedef struct EffPCPRingWork {
    f32 pos[4];
    u32 color10;
    u32 color14;
    f32 scale;
    BillObj *handle;
} EffPCPRingWork;
void effPcpDispatchKindAndRelease(EffPCPRingWork *work) {
    billDispatchByKind(work->handle);
    sdfChipFreeDirect(work);
}

extern u8 sdfViewEyeVector[];
extern u8 sdfViewTargetVector[];
extern void billInvokeCallback(BillObj *effect);
extern u32 effMultiplyPackedColors(u32 flags, u32 color);


/* Places a ring of 10 shrinking, brightening copies of the handle along the
 * fixed view direction. */
void effPcpDrawViewAlignedRing(EffPCPRingWork *work) {
    f32 pos[4];
    f32 dir[4];
    f32 size[4];
    BillObj *handle;
    f32 scale;
    u32 color;
    s32 i;

    VU0_LOAD_VF($vf10, sdfViewTargetVector);
    VU0_LOAD_VF($vf11, sdfViewEyeVector);
    VU0_SUB(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF($vf10, dir);
    handle = work->handle;
    size[0] = work->scale;
    size[1] = work->scale;
    size[2] = work->scale;
    VU0_LOAD_VF($vf10, size);
    VU0_LOAD_VF($vf11, dir);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF($vf11, work);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF($vf10, pos);
    effCopyVector(handle, pos);
    scale = work->scale;
    color = 0x10808080;
    for (i = 0; i < 10; i++) {
        billSetChildScaleComponents(handle, scale, scale);
        scale *= 0.975f;
        billSetChildParameter(handle, effMultiplyPackedColors(effMultiplyPackedColors(color, work->color14), work->color10));
        color += 0x05000000;
        billInvokeCallback(handle);
    }
}

void effPcpViewAlignedRingSetPosition(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpViewAlignedRingSetOverlayColor(EffPCPRingWork *work, u32 value) {
    work->color14 = value;
}

void effPcpViewAlignedRingSetScale(EffPCPRingWork *work, f32 val) {
    work->scale = val;
}

/* Re-rolls slot `index`: random-angle rotation matrix pushed to both handles
 * of the pair, then a new random countdown. */
void effTwinEffectRerollSlot(EffPCPTwinWork *work, s32 slotIndex) {
    u128 mtx[4];

    sdfVuLoadRotationMatrixY((sdfGetRandFloat(effDefaultRandomState) - 0.5f) * 2.0f * 6.283185f);
            VU0_STORE_MATRIX_UNCLOBBERED(mtx);

;
    effParamWorkCallback2(work->pair[slotIndex][0], mtx);
    effParamWorkCallback2(work->pair[slotIndex][1], mtx);
    mdlAddEntryPlain(effParamWorkGetData(work->pair[slotIndex][0]), 0, 0);
    mdlAddEntryPlain(effParamWorkGetData(work->pair[slotIndex][1]), 0, 0);
    work->counter[slotIndex] = sdfGetRand((EffRandState *)effDefaultRandomState) % 10;
}

EffPCPTwinWork *effTwinEffectCreateFromTable(void *src) {
    EffPCPTwinWork *work = sdfChipAllocDirect(0xBC);
    s32 slotIndex;

    for (slotIndex = 0; slotIndex < 8; slotIndex++) {
        if (slotIndex == 0) {
            work->pair[0][0] = effParamCreateFromTable(src, 0);
            work->pair[0][1] = effParamCreateFromTable(src, 2);
        } else if (slotIndex == 1) {
            work->pair[1][0] = effParamCreateFromTable(src, 1);
            work->pair[1][1] = effParamWorkDuplicate(work->pair[0][1]);
        } else {
            work->pair[slotIndex][0] = effParamWorkDuplicate(work->pair[slotIndex & 1][0]);
            work->pair[slotIndex][1] = effParamWorkDuplicate(work->pair[slotIndex & 1][1]);
        }
        effTwinEffectRerollSlot(work, slotIndex);
    }
    work->shared[0] = effParamCreateFromTable(src, 3);
    work->state[0] = 0;
    for (slotIndex = 1; slotIndex < 8; slotIndex++) {
        work->shared[slotIndex] = effParamWorkDuplicate(work->shared[0]);
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

void effTwinEffectRelease(EffPCPTwinWork *work) {
    EffParamWork **sharedHandleCursor = work->shared;
    EffParamWork **pairHandleCursor = &work->pair[0][0];
    s32 pairIndex;

    for (pairIndex = 0; pairIndex < 8; pairIndex++) {
        effDispatchParameterDataAndFreeWork(sharedHandleCursor[0]);
        effDispatchParameterDataAndFreeWork(pairHandleCursor[1]);
        effDispatchParameterDataAndFreeWork(pairHandleCursor[0]);
        sharedHandleCursor++;
        pairHandleCursor += 2;
    }
    sdfChipFreeDirect(work);
}

EffPCPTwinWork *effTwinEffectClone(EffPCPTwinWork *src) {
    EffPCPTwinWork *work = sdfChipAllocDirect(0xBC);
    s32 slotIndex;

    for (slotIndex = 0; slotIndex < 8; slotIndex++) {
        work->pair[slotIndex][0] = effParamWorkDuplicate(src->pair[slotIndex & 1][0]);
        work->pair[slotIndex][1] = effParamWorkDuplicate(src->pair[slotIndex & 1][1]);
        work->shared[slotIndex] = effParamWorkDuplicate(src->shared[0]);
        work->state[slotIndex] = 0;
        effTwinEffectRerollSlot(work, slotIndex);
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
void effTwinEffectUpdate(EffPCPTwinWork *work) {
    MdlCtx *obj[2];
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
        VU0_LOAD_VF(vf10, work);
        mdlStorePrimaryVectorVU(obj[0]);
        effParamWorkCallback1(work->pair[slotIndex][0], work->scale * 1.5f);
        effParamWorkCallback1(work->pair[slotIndex][1], 1.75f);
        mdlBroadcastMasked(obj[1], work->color);
        mdlProcessContextNodesAndTransforms(obj[0], D_00380828);
        sdfLoadMapRecordPositionVector(obj[0]->inner, 1);
        posp = &pos;
        VU0_STORE_VF_UNCLOBBERED(vf10, posp);
        mdlStorePrimaryVectorVU(obj[1]);
        mdlProcessContextNodesAndTransforms(obj[1], D_00380828);
        if (work->frame > 0x18) {
            effParamWorkCallback0(work->shared[slotIndex], posp);
            effParamWorkInvokeCallback(work->shared[slotIndex]);
        }
    }
    work->frame++;
}

void effPcpCopyTwinVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetTwinEffectScale(EffPCPTwinWork *work, f32 val) {
    work->scale = val;
}

void effPcpSetTwinEffectColor(EffPCPTwinWork *work, u32 value) {
    work->color = value;
}

/* Re-rolls slot `index` of the staggered effect: random-angle rotation matrix
 * for both handles, new random offset and delay. */
void effPcpStaggerRerollSlot(EffPCPStaggered *work, s32 index) {
    u128 mtx[4];

    sdfVuLoadRotationMatrixY((sdfGetRandFloat(effDefaultRandomState) - 0.5f) * 2.0f * 6.283185f);
            VU0_STORE_MATRIX_UNCLOBBERED(mtx);

;
    effParamWorkCallback2(work->handle[index * 2], mtx);
    effParamWorkCallback2(work->handle[index * 2 + 1], mtx);
    mdlAddEntryPlain(effParamWorkGetData(work->handle[index * 2]), 0, 0);
    mdlAddEntryPlain(effParamWorkGetData(work->handle[index * 2 + 1]), 0, 0);
    work->verticalOffset[index] = sdfGetRandFloat(effDefaultRandomState) * 150.0f;
    work->remainingDelay[index] = sdfGetRand((EffRandState *)effDefaultRandomState) % 10;
}


EffPCPStaggered *effPcpStaggerCreate(void *args) {
    EffPCPStaggered *work = sdfChipAllocDirect(0x98);
    EffParamWork **handle = &work->handle[1];
    s32 i = 0;

    do {
        if (i == 0) {
            work->handle[0] = effParamCreateFromTable(args, 0);
            work->handle[1] = effParamCreateFromTable(args, 1);
        } else {
            handle[-1] = effParamWorkDuplicate(work->handle[0]);
            handle[0] = effParamWorkDuplicate(work->handle[1]);
        }
        handle += 2;
        effPcpStaggerRerollSlot(work, i++);
    } while (i < 8);
    work->color = 0x80808080;
    work->scale = 1.0f;
    work->position[0] = 0;
    work->position[1] = 0;
    work->position[2] = 0;
    return work;
}

void effPcpStaggerRelease(EffPCPStaggered *work) {
    EffParamWork **p = &work->handle[1];
    s32 i;

    for (i = 0; i < 8; i++) {
        effDispatchParameterDataAndFreeWork(p[-1]);
        effDispatchParameterDataAndFreeWork(p[0]);
        p += 2;
    }
    sdfChipFreeDirect(work);
}

EffPCPStaggered *effCreatePairedResourceWork(EffPCPStaggered *source) {
    EffPCPStaggered *work = sdfChipAllocDirect(0x98);
    EffParamWork **handle = &work->handle[1];
    s32 i = 0;

    do {
        handle[-1] = effParamWorkDuplicate(source->handle[0]);
        handle[0] = effParamWorkDuplicate(source->handle[1]);
        handle += 2;
        effPcpStaggerRerollSlot(work, i);
        i++;
    } while (i < 8);
    work->color = 0x80808080;
    work->scale = 1.0f;
    work->position[0] = 0;
    work->position[1] = 0;
    work->position[2] = 0;
    return work;
}

void effPcpStaggerUpdate(EffPCPStaggered *work) {
    MdlCtx *obj[2];
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
                        VU0_LOAD_VF(vf10, pos);
            mdlStorePrimaryVectorVU(obj[0]);
            effParamWorkCallback1(work->handle[i * 2], work->scale * 1.5f);
            effParamWorkCallback1(work->handle[i * 2 + 1], 1.5f);
            mdlBroadcastMasked(obj[1], work->color);
            mdlProcessContextNodesAndTransforms(obj[0], D_00380828);
            sdfLoadMapRecordPositionVector(obj[0]->inner, 1);
            mdlStorePrimaryVectorVU(obj[1]);
            mdlProcessContextNodesAndTransforms(obj[1], D_00380828);
        }
    }
}

void effPcpCopyStaggerVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetStaggerEffectScale(EffPCPStaggered *work, f32 val) {
    work->scale = val;
}

void effPcpSetStaggerEffectColor(EffPCPStaggered *work, u32 value) {
    work->color = value;
}

/* Spawns cross arm (i, j): rotates the model by j sixths of a turn about the axis chosen by i. */
void effCrossArmSpawn(EffPCPCrossWork *work, u32 i, u32 j) {
    u128 mtx[4];
    f32 angle;

    mdlAddEntryPlain(effParamWorkGetData(work->handle[i][j]), 0, 0);
    angle = (f32)j * -1.0471974f;
    if (i == 0) {
        sdfVuLoadRotationMatrixY(angle);
    } else if (i == 1) {
        sdfVuLoadRotationMatrixY(-angle);
    } else if (i == 2) {
        sdfVuLoadRotationMatrixZ(-angle);
    } else {
        sdfVuLoadRotationMatrixZ(angle);
    }
        VU0_STORE_MATRIX(mtx);
;
    effParamWorkCallback2(work->handle[i][j], mtx);
    if ((j + 1) & 1) {
        work->remainingDelay[i][j] = 0;
    } else {
        work->remainingDelay[i][j] = 5;
    }
}

EffPCPCrossWork *effCrossEffectCreateFromTable(void *src) {
    EffPCPCrossWork *work = sdfChipAllocDirect(0xDC);
    s32 i;
    s32 j;

    work->base = effParamCreateFromTable(src, 0);
    mdlAddEntryPlain(effParamWorkGetData(work->base), 0, 0);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            if (j == 0) {
                work->handle[i][j] = effParamCreateFromTable(src, i + 1);
            } else {
                work->handle[i][j] = effParamWorkDuplicate(work->handle[i][0]);
            }
            effCrossArmSpawn(work, i, j);
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
void effCrossEffectRelease(EffPCPCrossWork *work) {
    s32 i;
    s32 j;

    effDispatchParameterDataAndFreeWork(work->base);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            effDispatchParameterDataAndFreeWork(work->handle[i][j]);
        }
    }
    sdfChipFreeDirect(work);
}

EffPCPCrossWork *effCrossEffectClone(EffPCPCrossWork *src) {
    EffPCPCrossWork *work = sdfChipAllocDirect(0xDC);
    s32 i;
    s32 j;

    work->base = effParamWorkDuplicate(src->base);
    mdlAddEntryPlain(effParamWorkGetData(work->base), 0, 0);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            work->handle[i][j] = effParamWorkDuplicate(src->handle[i][0]);
            effCrossArmSpawn(work, i, j);
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
void effCrossEffectUpdate(EffPCPCrossWork *work) {
    MdlCtx *anchor;
    MdlCtx *obj;
    s32 i;
    s32 j;

    anchor = effParamWorkGetData(work->base);
    VU0_LOAD_VF(vf10, work);
    mdlStorePrimaryVectorVU(anchor);
    effParamWorkCallback1(work->base, work->scale);
    mdlProcessContextNodesAndTransforms(anchor, D_00380828);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            if (work->remainingDelay[i][j] != 0) {
                work->remainingDelay[i][j]--;
                continue;
            }
            obj = effParamWorkGetData(work->handle[i][j]);
            mdlBroadcastMasked(obj, work->color);
            sdfLoadMapRecordPositionVector(anchor->inner, i * 4 + j + 1);
            mdlStorePrimaryVectorVU(obj);
            mdlProcessContextNodesAndTransforms(obj, D_00380828);
        }
    }
}

void effPcpCopyCrossVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetCrossEffectScale(EffPCPCrossWork *work, f32 val) {
    work->scale = val;
}

void effPcpSetCrossEffectColor(EffPCPCrossWork *work, u32 value) {
    work->color = value;
}


/* Re-arms slot `index`: registers both handles of the pair and rolls a new
 * random countdown. */
void effPcpDelayedPairsRerollSlot(EffPCPDelayedPairs *work, s32 index) {

    mdlAddEntryPlain(effParamWorkGetData(work->handle[index * 2]), 0, 0);
    mdlAddEntryPlain(effParamWorkGetData(work->handle[index * 2 + 1]), 0, index & 1);
    work->remainingDelay[index] = sdfGetRand((EffRandState *)effDefaultRandomState) % 10;
}


EffPCPDelayedPairs *effPcpDelayedPairsCreate(void *source) {
    EffPCPDelayedPairs *work = sdfChipAllocDirect(0x60);
    EffParamWork **handle = &work->handle[1];
    s32 i;

    for (i = 0; i < 6; i++) {
        if (i == 0) {
            work->handle[1] = effParamCreateFromTable(source, 6);
        }
        handle[-1] = effParamCreateFromTable(source, i);
        handle[0] = effParamWorkDuplicate(work->handle[1]);
        handle += 2;
        effPcpDelayedPairsRerollSlot(work, i);
    }
    work->color = 0x80808080;
    work->scale = 1.0f;
    work->unk00 = 0;
    work->unk04 = 0;
    work->unk08 = 0;
    return work;
}

void effPcpDelayedPairsRelease(EffPCPDelayedPairs *work) {
    EffParamWork **p = &work->handle[1];
    s32 i;

    for (i = 0; i < 6; i++) {
        effDispatchParameterDataAndFreeWork(p[-1]);
        effDispatchParameterDataAndFreeWork(p[0]);
        p += 2;
    }
    sdfChipFreeDirect(work);
}

EffPCPDelayedPairs *effPcpDelayedPairsClone(EffPCPDelayedPairs *source) {
    EffParamWork **sourceHandle;
    EffParamWork **workHandle;
    s32 i;
    EffPCPDelayedPairs *work = sdfChipAllocDirect(0x60);
    sourceHandle = &source->handle[1];
    workHandle = &work->handle[1];

    for (i = 0; i < 6; i++) {
        workHandle[-1] = effParamWorkDuplicate(sourceHandle[-1]);
        workHandle[0] = effParamWorkDuplicate(sourceHandle[0]);
        sourceHandle += 2;
        workHandle += 2;
        effPcpDelayedPairsRerollSlot(work, i);
    }
    work->color = 0x80808080;
    work->scale = 1.0f;
    work->unk00 = 0;
    work->unk04 = 0;
    work->unk08 = 0;
    return work;
}

void effPcpDelayedPairsUpdate(EffPCPDelayedPairs *work) {
    MdlCtx *obj[2];
    u128 vec;
    s32 i;

    for (i = 0; i < 6; i++) {
        if (work->remainingDelay[i] != 0) {
            work->remainingDelay[i]--;
        } else {
            obj[0] = effParamWorkGetData(work->handle[i * 2]);
            obj[1] = effParamWorkGetData(work->handle[i * 2 + 1]);
                        VU0_LOAD_VF_MEMORY(vf10, work);
            mdlStorePrimaryVectorVU(obj[0]);
            mdlBroadcastMasked(obj[1], work->color);
            mdlProcessContextNodesAndTransforms(obj[0], D_00380828);
            sdfLoadMapRecordPositionVector(obj[0]->inner, 1);
                        VU0_STORE_VF_UNCLOBBERED(vf10, &vec);
            mdlStorePrimaryVectorVU(obj[1]);
            mdlProcessContextNodesAndTransforms(obj[1], D_00380828);
        }
    }
}

void effPcpCopyDelayedPairVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetDelayedPairScale(EffPCPDelayedPairs *work, f32 val) {
    work->scale = val;
}

void effPcpSetDelayedPairColor(EffPCPDelayedPairs *work, u32 value) {
    work->color = value;
}

void effPcpChargeInitTail(EffPCPChargeWork *work) {
    work->historyCount = 0;
    work->updateCount = 0;
    work->baseColor = 0x80808080;
}

EffPCPChargeWork *effPcpChargeCreateWork(void *source) {
    SdfMemBlock *resource = sdfMemAllocDirect(0x1354);
    EffPCPChargeWork *work = (void *)sdfMemLock(resource);
    work->allocationHandle = resource;
    work->primaryHandle = effParamCreateFromTable(source, 0);
    work->secondaryHandle = effParamCreateFromTable(source, 1);
    effPcpChargeInitTail(work);
    work->vectorWords[0] = 0;
    work->vectorWords[1] = 0;
    work->vectorWords[2] = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    return work;
}

void effPcpChargeReleaseWork(EffPCPChargeWork *work) {
    effDispatchParameterDataAndFreeWork(work->primaryHandle);
    effDispatchParameterDataAndFreeWork(work->secondaryHandle);
    sdfMemFreeDirect(work->allocationHandle);
}

/* Duplicate the parameter owners into fresh history storage; sampled arrays
 * are not copied from source. */
EffPCPChargeWork *effPcpChargeCloneWork(EffPCPChargeWork *source) {
    SdfMemBlock *resource = sdfMemAllocDirect(0x1354);
    EffPCPChargeWork *work = (void *)sdfMemLock(resource);
    EffParamWork *firstHandle = source->primaryHandle;
    work->allocationHandle = resource;
    work->primaryHandle = effParamWorkDuplicate(firstHandle);
    work->secondaryHandle = effParamWorkDuplicate(source->secondaryHandle);
    effPcpChargeInitTail(work);
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
void effPcpChargeUpdateAndDrawHistory(EffPCPChargeWork *work) {
    f32 position[4] __attribute__((aligned(16)));
    u32 i;
    u32 color;
    BillObj *bill;
    MdlCtx *model;
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
    color = effMultiplyPackedColors(work->color, work->baseColor);
    bill = effParamWorkGetData(work->primaryHandle);
    model = effParamWorkGetData(work->secondaryHandle);
    scale = work->scale * 1.5f;
    effParamWorkCallback1(work->secondaryHandle, scale);
    effParamWorkCallback0(work->secondaryHandle, work->vectorWords);
    mdlProcessContextNodesAndTransforms(model, D_00380828);
    captureRow = work->historyCount;
    if (captureRow < 25) {
        for (point = 0; point < 7; point++) {
            if (captureRow % effChargeSamplePeriods[point] == 0) {
                sdfLoadMapRecordPositionVector(model->inner, point + 1);
                VU0_STORE_VF(vf10, position);
                PCP_COPY_VECTOR(work->samplePositions[captureRow][point], position);
                work->animationFrames[captureRow][point] = sdfGetRand((EffRandState *)effDefaultRandomState);
                work->sampleScales[captureRow][point] = 0;
                work->sampleAges[captureRow][point] = 0;
            }
        }
        work->historyCount++;
    }
    count = work->historyCount;
    billSetChildParameter(bill, color);
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
                billSetChildScaleComponents(bill, size, size);
                effCopyVector(bill, work->samplePositions[i][point]);
                billInvokeCallback(bill);
                billSetAnimationFrameForImmediateAdvance(bill, work->animationFrames[i][point]);
                work->animationFrames[i][point]++;
            }
        }
    }
}

void effPcpCopyVectorAF0(void *work, void *src) {
    PCP_COPY_VECTOR(((EffPCPChargeWork *)work)->vectorWords, src);
}

void effPcpChargeSetScale(EffPCPChargeWork *work, f32 val) {
    work->scale = val;
}

void effPcpChargeSetColor(EffPCPChargeWork *work, u32 value) {
    work->color = value;
}

EffPCPSpawnOnceWork *effPcpSpawnOnceCreate(void *src) {
    EffPCPSpawnOnceWork *dst;
    EffParamWork *handle;

    dst = sdfChipAllocDirect(0x1C);
    dst->primaryHandle = effParamCreateFromTable(src, 0);
    handle = effParamCreateFromTable(src, 1);
    dst->vector[0] = 0;
    dst->secondaryHandle = handle;
    dst->vector[1] = 0;
    dst->color = 0x80808080;
    dst->vector[2] = 0;
    return dst;
}

void effPcpSpawnOnceRelease(EffPCPSpawnOnceWork *work) {
    effDispatchParameterDataAndFreeWork(work->primaryHandle);
    effDispatchParameterDataAndFreeWork(work->secondaryHandle);
    sdfChipFreeDirect(work);
}

EffPCPSpawnOnceWork *effPcpSpawnOnceClone(EffPCPSpawnOnceWork *src) {
    EffPCPSpawnOnceWork *dst;
    EffParamWork *handle;

    dst = sdfChipAllocDirect(0x1C);
    dst->primaryHandle = effParamWorkDuplicate(src->primaryHandle);
    handle = effParamWorkDuplicate(src->secondaryHandle);
    dst->vector[0] = 0;
    dst->secondaryHandle = handle;
    dst->vector[1] = 0;
    dst->color = 0x80808080;
    dst->vector[2] = 0;
    return dst;
}

void effPcpSpawnOnce(EffPCPSpawnOnceWork *work) {
    MdlCtx *obj;
    u128 vec;

    obj = effParamWorkGetData(work->primaryHandle);
        VU0_LOAD_VF_MEMORY(vf10, work->vector);
    mdlStorePrimaryVectorVU(obj);
    effParamWorkCallback3(work->secondaryHandle, work->color);
    mdlProcessContextNodesAndTransforms(obj, D_00380828);
    sdfLoadMapRecordPositionVector(obj->inner, 1);
        VU0_STORE_VF_TO_MEMORY(vf10, vec);
    effParamWorkCallback0(work->secondaryHandle, &vec);
    effParamWorkInvokeCallback(work->secondaryHandle);
}

void effPcpCopySpawnOnceVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSpawnOnceSetColor(EffPCPSpawnOnceWork *work, u32 value) {
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
void effPcpInitTwelveRadialParticles(EffPCPThunderGroup *group) {
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
            cosv = sdfSinPoly(angle);
            angle += 60.0f * EFF_DEG2RAD;
        }
        spread = sdfGetRandFloat(effDefaultRandomState) * 0.5f + 0.5f;
        effSharedSpawnParams->bandWidth = spread * 1.25f;
        effSharedSpawnParams->edgeWidth = spread * 12.5f;
        radius = (sdfGetRandFloat(effDefaultRandomState) * 0.25f + 0.75f) * 100.0f;
        effSharedSpawnParams->end[1] = 0;
        effSharedSpawnParams->end[0] = sinv * radius;
        effSharedSpawnParams->end[2] = cosv * radius;
        dir[0] = sinv;
        dir[1] = (sdfGetRandFloat(effDefaultRandomState) - 0.5f) * 2.0f + 2.0f;
        dir[2] = cosv;
        VU0_LOAD_VF($vf10, dir);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF($vf10, dir);
        reach = (sdfGetRandFloat(effDefaultRandomState) * 0.65f + (1.0f - 0.65f)) * 350.0f;
        scale[2] = reach;
        scale[0] = reach;
        scale[1] = -reach;
        VU0_LOAD_VF($vf10, dir);
        VU0_LOAD_VF($vf11, scale);
        VU0_MUL(vf10, vf10, vf11);
        VU0_LOAD_VF($vf11, D_003B1680);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF($vf10, effSharedSpawnParams);
        if (scale[0] < 250.0f) {
            speed = (sdfGetRandFloat(effDefaultRandomState) - 0.5f) * 2.0f * 2.5f + 5.0f;
            life = sdfGetRand((EffRandState *)effDefaultRandomState) % 5 + 5;
        } else {
            speed = (sdfGetRandFloat(effDefaultRandomState) - 0.5f) * 2.0f * 5.0f + 10.0f;
            life = sdfGetRand((EffRandState *)effDefaultRandomState) % 6 + 15;
        }
        effSharedSpawnParams->halfLife = life;
        effSharedSpawnParams->waveAmplitude = speed;
        *out = effThunderFragCreate(effSharedSpawnParams);
        out++;
        i++;
    } while (i < 12);
}

void *effPcpCreateTwelveRadialParticles(void) {
    void *work;

    work = sdfChipAllocDirect(0x4c);
    effPcpInitTwelveRadialParticles(work);
    return work;
}

typedef struct MenuPanelChildren1C {
    u8 pad00[0x1C];
    void *children[30];
} MenuPanelChildren1C;

void effPcpReleaseTwelveRadialParticles(EffPCPThunderGroup *group) {
    s32 i;
    for (i = 0; i < 12; i++) {
        effThunderReleaseFragmentWork(group->handles[i]);
    }
    sdfChipFreeDirect(group);
}

void *effPcpAllocateRadialParticleGroup(void) {
    void *work;

    work = sdfChipAllocDirect(0x4c);
    effPcpInitTwelveRadialParticles(work);
    return work;
}


/* Shifts the first `count` handles' objects by the work position for the
 * duration of their update, then puts the original vectors back. */
static inline void effPcpShiftThunderHandles(EffPCPThunderGroup *work, s32 count) {
    u128 saved[2];
    u8 *obj;
    s32 i;

    for (i = 0; i < count; i++) {
        obj = (u8 *)effThunderGetFragmentParameters(work->handles[i]);
        VU0_LOAD_VF($vf10, obj + 0x10);
        VU0_STORE_VF($vf10, &saved[1]);
        VU0_LOAD_VF($vf11, work);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF($vf10, obj + 0x10);
        VU0_LOAD_VF($vf10, obj);
        VU0_STORE_VF($vf10, &saved[0]);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF($vf10, obj);
        effThunderUpdateFragments(work->handles[i]);
        PCP_COPY_VECTOR(obj, &saved[0]);
        PCP_COPY_VECTOR(obj + 0x10, &saved[1]);
    }
}

void effPcpThunderShiftUpdateA(EffPCPThunderGroup *work) {
    effPcpShiftThunderHandles(work, 12);
}

void effPcpCopyRadialParticleVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetRadialParticleScale(EffPCPThunderGroup *work, f32 val) {
    work->unk14 = val;
}

void effPcpSetRadialParticleColor(EffPCPThunderGroup *work, u32 value) {
    work->unk18 = value;
}

extern EffThunderFragmentParams effNarrowConeSpawnParams[];

/* 12-piece spread on a cone around a random axis (rotation matrix built by
 * sdfBuildVuRotationFromAxisAngle into the VU0 matrix registers). */
void effPcpSpawnConeTwelve(EffPCPThunderGroup *group) {
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
            cosv = sdfSinPoly(angle);
            angle += 60.0f * EFF_DEG2RAD;
        }
        spread = sdfGetRandFloat(effDefaultRandomState) * 0.5f + 0.5f;
        effNarrowConeSpawnParams->bandWidth = spread * 1.25f;
        effNarrowConeSpawnParams->edgeWidth = spread * 12.5f;
        radius = (sdfGetRandFloat(effDefaultRandomState) * 0.25f + 0.75f) * 100.0f;
        axis[0] = cosv;
        axis[1] = 0;
        axis[2] = -sinv;
        effNarrowConeSpawnParams->end[1] = 0;
        effNarrowConeSpawnParams->end[0] = sinv * radius;
        effNarrowConeSpawnParams->end[2] = cosv * radius;
        sdfBuildVuRotationFromAxisAngle(-(sdfGetRandFloat(effDefaultRandomState) * (50.0f * EFF_DEG2RAD) + 5.0f * EFF_DEG2RAD), (const struct RwV3d *)axis);
        height = (sdfGetRandFloat(effDefaultRandomState) * 0.65f + (1.0f - 0.65f)) * 500.0f;
        effNarrowConeSpawnParams->start[0] = 0;
        effNarrowConeSpawnParams->start[2] = 0;
        effNarrowConeSpawnParams->start[1] = -height;
        VU0_LOAD_VF($vf10, effNarrowConeSpawnParams->start);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF($vf10, effNarrowConeSpawnParams->start);
        effNarrowConeSpawnParams->start[0] += effNarrowConeSpawnParams->end[0];
        effNarrowConeSpawnParams->start[2] += effNarrowConeSpawnParams->end[2];
        if (height < 300.0f) {
            speed = (sdfGetRandFloat(effDefaultRandomState) - 0.5f) * 2.0f * 2.5f + 5.0f;
            life = sdfGetRand((EffRandState *)effDefaultRandomState) % 5 + 5;
        } else {
            speed = (sdfGetRandFloat(effDefaultRandomState) - 0.5f) * 2.0f * 5.0f + 10.0f;
            life = sdfGetRand((EffRandState *)effDefaultRandomState) % 6 + 15;
        }
        effNarrowConeSpawnParams->halfLife = life;
        effNarrowConeSpawnParams->waveAmplitude = speed;
        group->handles[i] = effThunderFragCreate(effNarrowConeSpawnParams);
        i++;
    } while (i < 12);
}

void *effPcpAllocateNarrowConeGroup(void) {
    void *work;

    work = sdfChipAllocDirect(0x4c);
    effPcpSpawnConeTwelve(work);
    return work;
}

void effPcpNarrowConeGroupRelease(EffPCPThunderGroup *group) {
    s32 i;
    for (i = 0; i < 12; i++) {
        effThunderReleaseFragmentWork(group->handles[i]);
    }
    sdfChipFreeDirect(group);
}

void *effPcpCreateNarrowConeParticles(void) {
    void *work;

    work = sdfChipAllocDirect(0x4c);
    effPcpSpawnConeTwelve(work);
    return work;
}

void effPcpThunderShiftUpdateB(EffPCPThunderGroup *work) {
    effPcpShiftThunderHandles(work, 12);
}

void effPcpCopyNarrowConeVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetNarrowConeScale(EffPCPThunderGroup *work, f32 val) {
    work->unk14 = val;
}

void effPcpSetNarrowConeColor(EffPCPThunderGroup *work, u32 value) {
    work->unk18 = value;
}

extern EffThunderFragmentParams effVariableHeightSpawnParams[];

/* 30-piece spread: pieces are placed on a cone around a random axis (rotation
 * matrix built by sdfBuildVuRotationFromAxisAngle into the VU0 matrix registers). */
void effPcpSpawnVariableHeightParticles(EffPCPThunderGroup *group) {
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
            cosv = sdfSinPoly(angle);
            angle += 24.0f * EFF_DEG2RAD;
        }
        spread = sdfGetRandFloat(effDefaultRandomState) * 0.5f + 0.5f;
        effVariableHeightSpawnParams->bandWidth = spread * 1.25f;
        effVariableHeightSpawnParams->edgeWidth = spread * 12.5f;
        effVariableHeightSpawnParams->end[1] = 0;
        effVariableHeightSpawnParams->end[0] = sinv * ((sdfGetRandFloat(effDefaultRandomState) * 0.25f + 0.75f) * 500.0f);
        effVariableHeightSpawnParams->end[2] = cosv * ((sdfGetRandFloat(effDefaultRandomState) * 0.25f + 0.75f) * 250.0f);
        axis[0] = cosv;
        axis[1] = 0;
        axis[2] = -sinv;
        sdfBuildVuRotationFromAxisAngle(-(sdfGetRandFloat(effDefaultRandomState) * (55.0f * EFF_DEG2RAD) + 5.0f * EFF_DEG2RAD), (const struct RwV3d *)axis);
        height = (sdfGetRandFloat(effDefaultRandomState) * 0.65f + (1.0f - 0.65f)) * 500.0f;
        effVariableHeightSpawnParams->start[0] = 0;
        effVariableHeightSpawnParams->start[2] = 0;
        effVariableHeightSpawnParams->start[1] = -height;
        VU0_LOAD_VF($vf10, effVariableHeightSpawnParams->start);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF($vf10, effVariableHeightSpawnParams->start);
        effVariableHeightSpawnParams->start[0] += effVariableHeightSpawnParams->end[0];
        effVariableHeightSpawnParams->start[2] += effVariableHeightSpawnParams->end[2];
        if (height < 300.0f) {
            speed = (sdfGetRandFloat(effDefaultRandomState) - 0.5f) * 2.0f * 2.5f + 5.0f;
            life = sdfGetRand((EffRandState *)effDefaultRandomState) % 5 + 5;
        } else {
            speed = (sdfGetRandFloat(effDefaultRandomState) - 0.5f) * 2.0f * 5.0f + 10.0f;
            life = sdfGetRand((EffRandState *)effDefaultRandomState) % 6 + 15;
        }
        effVariableHeightSpawnParams->halfLife = life;
        effVariableHeightSpawnParams->waveAmplitude = speed;
        group->handles[i] = effThunderFragCreate(effVariableHeightSpawnParams);
        i++;
    } while (i < 30);
}

void *effPcpAllocateVariableHeightParticleGroup(void) {
    void *work;

    work = sdfChipAllocDirect(0x94);
    effPcpSpawnVariableHeightParticles(work);
    return work;
}

void effPcpWideConeGroupRelease(EffPCPThunderGroup *group) {
    s32 i;
    for (i = 0; i < 30; i++) {
        effThunderReleaseFragmentWork(group->handles[i]);
    }
    sdfChipFreeDirect(group);
}

void *effPcpCreateVariableHeightParticles(void) {
    void *work;

    work = sdfChipAllocDirect(0x94);
    effPcpSpawnVariableHeightParticles(work);
    return work;
}

void effPcpThunderShiftUpdateC(EffPCPThunderGroup *work) {
    effPcpShiftThunderHandles(work, 30);
}

void effPcpCopyVariableHeightVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetVariableHeightParticleScale(EffPCPThunderGroup *work, f32 val) {
    work->unk14 = val;
}

void effPcpSetVariableHeightParticleColor(EffPCPThunderGroup *work, u32 value) {
    work->unk18 = value;
}

extern EffThunderFragmentParams effWideConeSpawnParams[];

/* Same spread as effPcpSpawnConeTwelve with a wider cone and its own parameter block. */
void effPcpSpawnWideConeTwelve(EffPCPThunderGroup *group) {
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
            cosv = sdfSinPoly(angle);
            angle += 60.0f * EFF_DEG2RAD;
        }
        spread = sdfGetRandFloat(effDefaultRandomState) * 0.5f + 0.5f;
        effWideConeSpawnParams->bandWidth = spread * 1.25f;
        effWideConeSpawnParams->edgeWidth = spread * 12.5f;
        radius = (sdfGetRandFloat(effDefaultRandomState) * 0.25f + 0.75f) * 100.0f;
        axis[0] = cosv;
        axis[1] = 0;
        axis[2] = -sinv;
        effWideConeSpawnParams->end[1] = 0;
        effWideConeSpawnParams->end[0] = sinv * radius;
        effWideConeSpawnParams->end[2] = cosv * radius;
        sdfBuildVuRotationFromAxisAngle(-(sdfGetRandFloat(effDefaultRandomState) * (55.0f * EFF_DEG2RAD) + 5.0f * EFF_DEG2RAD), (const struct RwV3d *)axis);
        height = (sdfGetRandFloat(effDefaultRandomState) * 0.65f + (1.0f - 0.65f)) * 500.0f;
        effWideConeSpawnParams->start[0] = 0;
        effWideConeSpawnParams->start[2] = 0;
        effWideConeSpawnParams->start[1] = -height;
        VU0_LOAD_VF($vf10, effWideConeSpawnParams->start);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF($vf10, effWideConeSpawnParams->start);
        effWideConeSpawnParams->start[0] += effWideConeSpawnParams->end[0];
        effWideConeSpawnParams->start[2] += effWideConeSpawnParams->end[2];
        if (height < 300.0f) {
            speed = (sdfGetRandFloat(effDefaultRandomState) - 0.5f) * 2.0f * 2.5f + 5.0f;
            life = sdfGetRand((EffRandState *)effDefaultRandomState) % 5 + 5;
        } else {
            speed = (sdfGetRandFloat(effDefaultRandomState) - 0.5f) * 2.0f * 5.0f + 10.0f;
            life = sdfGetRand((EffRandState *)effDefaultRandomState) % 6 + 15;
        }
        effWideConeSpawnParams->halfLife = life;
        effWideConeSpawnParams->waveAmplitude = speed;
        group->handles[i] = effThunderFragCreate(effWideConeSpawnParams);
        i++;
    } while (i < 12);
}

void *effPcpAllocateWideConeGroup(void) {
    void *work;

    work = sdfChipAllocDirect(0x4c);
    effPcpSpawnWideConeTwelve(work);
    return work;
}

void effPcpReleaseWideConeParticles(EffPCPThunderGroup *group) {
    s32 i;
    for (i = 0; i < 12; i++) {
        effThunderReleaseFragmentWork(group->handles[i]);
    }
    sdfChipFreeDirect(group);
}

void *effPcpCreateWideConeParticles(void) {
    void *work;

    work = sdfChipAllocDirect(0x4c);
    effPcpSpawnWideConeTwelve(work);
    return work;
}

void effPcpThunderShiftUpdateD(EffPCPThunderGroup *work) {
    effPcpShiftThunderHandles(work, 12);
}

void effPcpCopyWideConeVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetWideConeScale(EffPCPThunderGroup *work, f32 val) {
    work->unk14 = val;
}

void effPcpSetWideConeColor(EffPCPThunderGroup *work, u32 value) {
    work->unk18 = value;
}

extern EffThunderFragmentParams effFixedOriginSpawnParams[];

/* 12 pieces thrown from a fixed origin along a random angle; the param block
 * (of two) is picked at random. */
void effPcpSpawnFixedOriginTwelve(EffPCPThunderGroup *group) {
    s32 i;
    EffThunderFragmentParams *params;
    f32 angle;
    f32 dist;
    s32 life;

    for (i = 0; i < 12; i++) {
        params = &effFixedOriginSpawnParams[sdfGetRand((EffRandState *)effDefaultRandomState) & 1];
        params->waveAmplitude = (sdfGetRandFloat(effDefaultRandomState) - 0.5f) * 2.0f * 5.0f + 20.0f;
        life = sdfGetRand((EffRandState *)effDefaultRandomState) % 6 + 15;
        params->start[0] = -150.0f;
        params->start[1] = -500.0f;
        params->start[2] = 0;
        params->halfLife = life;
        angle = sdfGetRandFloat(effDefaultRandomState) * (3.14159265f / 2.0f) + 3.14159265f / 8.0f;
        dist = (sdfGetRandFloat(effDefaultRandomState) * 0.25f + 0.75f) * 600.0f;
        params->end[0] = params->start[0] + sdfCos(angle) * dist;
        params->end[1] = params->start[1] + sdfSinPoly(angle) * dist;
        params->end[2] = 0;
        group->handles[i] = effThunderFragCreate(params);
    }
}

void *effPcpAllocateFixedOriginGroup(void) {
    void *work;

    work = sdfChipAllocDirect(0x4c);
    effPcpSpawnFixedOriginTwelve(work);
    return work;
}

void effPcpAngledBurstGroupRelease(EffPCPThunderGroup *group) {
    s32 i;
    for (i = 0; i < 12; i++) {
        effThunderReleaseFragmentWork(group->handles[i]);
    }
    sdfChipFreeDirect(group);
}

void *effPcpCreateFixedOriginParticles(void) {
    void *work;

    work = sdfChipAllocDirect(0x4c);
    effPcpSpawnFixedOriginTwelve(work);
    return work;
}

void effPcpThunderShiftUpdateE(EffPCPThunderGroup *work) {
    effPcpShiftThunderHandles(work, 12);
}

void effPcpCopyFixedOriginVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetFixedOriginScale(EffPCPThunderGroup *work, f32 val) {
    work->unk14 = val;
}

void effPcpSetFixedOriginParticleColor(EffPCPThunderGroup *work, u32 value) {
    work->unk18 = value;
}

extern void sdfVuLoadSecondaryRotationMatrixZ(f32 angle);
extern EffThunderFragmentParams effSideBurstSpawnParams[];

/* 8 pieces flung sideways from below; three rotation-matrix loads compose the
 * VU0 matrix before the particle positions are transformed. */
void effPcpSpawnSidewaysEight(EffPCPThunderGroup *group) {
    s32 i;
    EffThunderFragmentParams *params;

    for (i = 0; i < 8; i++) {
        params = &effSideBurstSpawnParams[sdfGetRand((EffRandState *)effDefaultRandomState) & 1];
        if (i & 1) {
            params->end[0] = sdfGetRandFloat(effDefaultRandomState) * 500.0f;
        } else {
            params->end[0] = sdfGetRandFloat(effDefaultRandomState) * -500.0f;
        }
        params->end[1] = 0;
        params->end[2] = (sdfGetRandFloat(effDefaultRandomState) - 0.5f) * 2.0f * 250.0f;
        params->start[0] = 0;
        params->start[1] = -600.0f;
        params->start[2] = 0;
        sdfVuLoadRotationMatrixX((sdfGetRandFloat(effDefaultRandomState) - 0.5f) * 2.0f * (30.0f * EFF_DEG2RAD));
        sdfVuLoadSecondaryRotationMatrixZ((sdfGetRandFloat(effDefaultRandomState) - 0.5f) * 2.0f * (30.0f * EFF_DEG2RAD));
        sdfMultiplyVuMatrixInPlace();
        VU0_LOAD_VF($vf10, params->start);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF($vf10, params->start);
        params->start[0] += params->end[0];
        params->start[2] += params->end[2];
        group->handles[i] = effThunderFragCreate(params);
    }
}

void *effPcpAllocateSideBurstGroup(void) {
    void *work;

    work = sdfChipAllocDirect(0x3c);
    effPcpSpawnSidewaysEight(work);
    return work;
}

void effPcpSideBurstGroupRelease(EffPCPThunderGroup *group) {
    s32 i;
    for (i = 0; i < 8; i++) {
        effThunderReleaseFragmentWork(group->handles[i]);
    }
    sdfChipFreeDirect(group);
}

void *effPcpCreateSideBurstParticles(void) {
    void *work;

    work = sdfChipAllocDirect(0x3c);
    effPcpSpawnSidewaysEight(work);
    return work;
}

void effPcpThunderShiftUpdateF(EffPCPThunderGroup *work) {
    effPcpShiftThunderHandles(work, 8);
}

void effPcpCopySideBurstVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetSideBurstScale(EffPCPThunderGroup *work, f32 val) {
    work->unk14 = val;
}

void effPcpSetSideBurstColor(EffPCPThunderGroup *work, u32 value) {
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

void effPcpInitializeThunderHandleWork(EffPCPFadeWork *work) {
    EffThunderVectorWork *handle;

    handle = effCreateThunderCellSystemWork((EffThunderAlphaParams *)effExpandingThunderFadeParams);
    work->frame = 0;
    work->handle = handle;
}

void *effPcpCreateThunderHandleWork(void) {
    void *work;

    work = sdfChipAllocDirect(0x20);
    effPcpInitializeThunderHandleWork(work);
    return work;
}

void effPcpReleaseThunderHandleWork(EffPCPFadeWork *work) {
    effThunderReleaseVectorWork(work->handle);
    sdfChipFreeDirect(work);
}

void *effAllocateThunderHandleWork(void) {
    void *work;

    work = sdfChipAllocDirect(0x20);
    effPcpInitializeThunderHandleWork(work);
    return work;
}

extern void effThunderCopyVectorOrigin(void *destination, void *source);

/* Scales the target over the first 22 frames, fades the colour out from frame 45. */
void effPcpThunderExpandThenFadeUpdate(EffPCPFadeWork *work) {
    EffThunderParameterHead *parameterHead;
    f32 phase;
    u32 fadedColor;
    u32 frame;

    frame = work->frame;
    if (frame < 0x17) {
        parameterHead = effThunderGetParameterHead(work->handle);
        frame = work->frame;
        phase = (f32)frame / 22.0f;
        parameterHead->vector.radiusScale = phase * 600.0f + 200.0f;
        parameterHead->vector.heightScale = phase * 300.0f + 100.0f;
    }
    if (frame >= 0x2D) {
        phase = (f32)(frame - 0x2D) / 30.0f;
        fadedColor = effBlendColor(work->color, 0, phase);
    } else {
        fadedColor = work->color;
    }
    effThunderSetVectorTint(work->handle, fadedColor);
    effThunderCopyVectorOrigin((void *)work->handle, work);
    effThunderUpdateVectorCells(work->handle);
    work->frame++;
}

void effPcpCopyThunderHandleVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

/* Retain this fade variant's float input; its observed updater does not read it. */
void effPcpSetExpandingThunderFadeInput(EffPCPFadeWork *work, f32 input) {
    work->unk10 = input;
}

void effPcpSetExpandingThunderFadeColor(EffPCPFadeWork *work, u32 value) {
    work->color = value;
}

extern EffThunderFragmentParams effConeBurstSpawnParams[];

/* Same spread as effPcpSpawnVariableHeightParticles with its own parameter block. */
void effPcpSpawnConeThirty(EffPCPThunderGroup *group) {
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
            cosv = sdfSinPoly(angle);
            angle += 24.0f * EFF_DEG2RAD;
        }
        spread = sdfGetRandFloat(effDefaultRandomState) * 0.5f + 0.5f;
        effConeBurstSpawnParams->bandWidth = spread * 1.25f;
        effConeBurstSpawnParams->edgeWidth = spread * 12.5f;
        effConeBurstSpawnParams->end[1] = 0;
        effConeBurstSpawnParams->end[0] = sinv * ((sdfGetRandFloat(effDefaultRandomState) * 0.25f + 0.75f) * 500.0f);
        effConeBurstSpawnParams->end[2] = cosv * ((sdfGetRandFloat(effDefaultRandomState) * 0.25f + 0.75f) * 250.0f);
        axis[0] = cosv;
        axis[1] = 0;
        axis[2] = -sinv;
        sdfBuildVuRotationFromAxisAngle(-(sdfGetRandFloat(effDefaultRandomState) * (55.0f * EFF_DEG2RAD) + 5.0f * EFF_DEG2RAD), (const struct RwV3d *)axis);
        height = (sdfGetRandFloat(effDefaultRandomState) * 0.65f + (1.0f - 0.65f)) * 500.0f;
        effConeBurstSpawnParams->start[0] = 0;
        effConeBurstSpawnParams->start[2] = 0;
        effConeBurstSpawnParams->start[1] = -height;
        VU0_LOAD_VF($vf10, effConeBurstSpawnParams->start);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF($vf10, effConeBurstSpawnParams->start);
        effConeBurstSpawnParams->start[0] += effConeBurstSpawnParams->end[0];
        effConeBurstSpawnParams->start[2] += effConeBurstSpawnParams->end[2];
        if (height < 300.0f) {
            speed = (sdfGetRandFloat(effDefaultRandomState) - 0.5f) * 2.0f * 2.5f + 5.0f;
            life = sdfGetRand((EffRandState *)effDefaultRandomState) % 5 + 5;
        } else {
            speed = (sdfGetRandFloat(effDefaultRandomState) - 0.5f) * 2.0f * 5.0f + 10.0f;
            life = sdfGetRand((EffRandState *)effDefaultRandomState) % 6 + 15;
        }
        effConeBurstSpawnParams->halfLife = life;
        effConeBurstSpawnParams->waveAmplitude = speed;
        group->handles[i] = effThunderFragCreate(effConeBurstSpawnParams);
        i++;
    } while (i < 30);
}

void *effPcpAllocateDenseConeGroup(void) {
    void *work;

    work = sdfChipAllocDirect(0x94);
    effPcpSpawnConeThirty(work);
    return work;
}

void effPcpReleaseConeParticleGroup(EffPCPThunderGroup *group) {
    s32 i;
    for (i = 0; i < 30; i++) {
        effThunderReleaseFragmentWork(group->handles[i]);
    }
    sdfChipFreeDirect(group);
}

void *effPcpCreateDenseConeParticles(void) {
    void *work;

    work = sdfChipAllocDirect(0x94);
    effPcpSpawnConeThirty(work);
    return work;
}

void effPcpThunderShiftUpdateG(EffPCPThunderGroup *work) {
    effPcpShiftThunderHandles(work, 30);
}

void effPcpCopyDenseConeVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetDenseConeScale(EffPCPThunderGroup *work, f32 val) {
    work->unk14 = val;
}

void effPcpSetDenseConeColor(EffPCPThunderGroup *work, u32 value) {
    work->unk18 = value;
}

void effPcpInitGrowingThunderFadeWork(EffPCPFadeWork *work) {
    EffThunderVectorWork *handle;

    handle = effCreateThunderCellSystemWork((EffThunderAlphaParams *)effGrowingThunderFadeParams);
    work->frame = 0;
    work->handle = handle;
}

void *effPcpCreateGrowingThunderFadeWork(void) {
    void *work;

    work = sdfChipAllocDirect(0x20);
    effPcpInitGrowingThunderFadeWork(work);
    return work;
}

void effPcpReleaseGrowingThunderFadeWork(EffPCPFadeWork *work) {
    effThunderReleaseVectorWork(work->handle);
    sdfChipFreeDirect(work);
}

void *effAllocateGrowingThunderFadeWork(void) {
    void *work;

    work = sdfChipAllocDirect(0x20);
    effPcpInitGrowingThunderFadeWork(work);
    return work;
}

/* Same fade with a longer scale-up; stops updating once frame 65 is reached. */
void effPcpThunderUniformGrowFadeUpdate(EffPCPFadeWork *work) {
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
        parameterHead = effThunderGetParameterHead(work->handle);
        phase = (f32)frame / 30.0f;
        value = phase * 200.0f + 150.0f;
        parameterHead->vector.radiusScale = value;
        parameterHead->vector.heightScale = value;
    }
    if (frame >= 0x39) {
        phase = ((f32)frame - 57.0f) * 0.125f;
        fadedColor = effBlendColor(work->color, 0, phase);
        effThunderSetVectorTint(work->handle, fadedColor);
    } else {
        effThunderSetVectorTint(work->handle, work->color);
    }
    effThunderCopyVectorOrigin((void *)work->handle, work);
    effThunderUpdateVectorCells(work->handle);
    work->frame++;
}

void effPcpCopyGrowingThunderFadeVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

/* Retain this fade variant's float input; its observed updater does not read it. */
void effPcpSetGrowingThunderFadeInput(EffPCPFadeWork *work, f32 input) {
    work->unk10 = input;
}

void effPcpSetGrowingThunderFadeColor(EffPCPFadeWork *work, u32 value) {
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

EffPCPSharedTrailRef *effPcpCreateSharedTrailRef(EffPCPTrailParams *params) {
    EffPCPSharedTrailRef *ref = sdfChipAllocDirect(sizeof(*ref));

    ref->frame = 0;
    if (D_00436438 == 0) {
        effPcpSharedTrailWork = sdfChipAllocDirect(sizeof(EffPCPSharedTrail));
        effPcpSharedTrailWork->obj = effCloneBlurTemplate(&params->res);
        effPcpSharedTrailWork->color = params->res.source.color;
        effPcpSharedTrailWork->frame = 0;
        effPcpSharedTrailWork->flags = 0x80808080;
        effPcpSharedTrailWork->pos[0] = 0;
        effPcpSharedTrailWork->pos[1] = 0;
        effPcpSharedTrailWork->pos[2] = 0;
        effPcpSharedTrailWork->sizeInput = 0;
        effPcpSharedTrailWork->limit = params->count;
        effPcpSharedTrailWork->baseLimit = params->count;
        effPcpSharedTrailWork->colors[0] = params->colors[0];
        effPcpSharedTrailWork->colors[1] = params->colors[1];
        effPcpSharedTrailWork->scale = params->scale;
        effPcpSharedTrailWork->minSize = params->minSize;
    }
    D_00436438++;
    return ref;
}

void effPcpSharedWorkRelease(EffPCPSharedTrailRef *work)
{
    sdfChipFreeDirect(work);
    if (--D_00436438 != 0) {
        return;
    }
    effReleaseBlurTemplate(effPcpSharedTrailWork->obj);
    sdfChipFreeDirect(effPcpSharedTrailWork);
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

extern s32 effMeasureCameraRightScreenOffsetVU(f32 value);
extern u32 effMultiplyPackedColors(u32 flags, u32 color);
#define EFF_SHARED_TRAIL ((EffPCPSharedTrail *)effPcpSharedTrailWork)

void effPcpUpdateSharedTrail(ref)
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
        obj->body.extent = (s32)((f32)effMeasureCameraRightScreenOffsetVU(EFF_SHARED_TRAIL->sizeInput) * EFF_SHARED_TRAIL->scale);
        VU0_STORE_VF($vf10, pos);
        obj->body.source.position[0] = (s32)pos[0] - 0x800;
        obj->body.source.position[1] = ((s32)pos[1] - 0x800) << 1;
        if (obj->body.extent < EFF_SHARED_TRAIL->minSize) {
            obj->body.extent = EFF_SHARED_TRAIL->minSize;
        }
        obj->body.source.color = effMultiplyPackedColors(EFF_SHARED_TRAIL->flags, EFF_SHARED_TRAIL->color);
        effDrawBlurPixelRectWithResource(obj);
        EFF_SHARED_TRAIL->frame++;
    }
}

void effPcpSharedTrailSetPosition(void *work, void *src) {
    PCP_COPY_VECTOR(effPcpSharedTrailWork, src);
}

void effPcpSharedTrailSetFlags(u32 unused, u32 val) {
    effPcpSharedTrailWork->flags = val;
}

void effSetSharedScale(u32 unused, f32 value) {
    effPcpSharedTrailWork->sizeInput = value;
}


EffPCPTrailWork *effPcpTrailCreate(EffPCPTrailParams *params) {
    EffPCPTrailWork *work = sdfChipAllocDirect(sizeof(EffPCPTrailWork));

    work->obj = effCloneBlurTemplate(&params->res);
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

void effPcpTrailRelease(EffPCPTrailWork *work) {
    effReleaseBlurTemplate(work->obj);
    sdfChipFreeDirect(work);
}

/* Per-frame update: places the trail object from the work position and
 * alternates its colour between two entries until the count runs out. */
void effPcpTrailUpdate(EffPCPTrailWork *work) {
    EffBlurTemplate *obj;
    f32 pos[4];

    obj = work->obj;
    VU0_LOAD_VF($vf10, work->pos);
    obj->body.extent = (s32)((f32)effMeasureCameraRightScreenOffsetVU(work->sizeInput) * work->scale);
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
    obj->body.source.color = effMultiplyPackedColors(work->flags, work->color);
    effDrawBlurPixelRectWithResource(obj);
}

void effPcpCopySharedTrailVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetTrailFlags(EffPCPTrailWork *work, u32 value) {
    work->flags = value;
}

void effPcpTrailSetSizeInput(EffPCPTrailWork *work, f32 val) {
    work->sizeInput = val;
}

EffPCPCompactFadeWork *effPcpCompactEffectCreate(EffPCPCompactRectParams *params) {
    EffPCPCompactFadeWork *work;

    work = sdfChipAllocDirect(0x3C);
    work->resource.rectangle = effCloneResourceTemplate(&params->res);
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

EffPCPCompactFadeWork *effPcpCompactEffectCreateFromTable(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    return effPcpCompactEffectCreate(param0);
}

EffPCPCompactFadeWork *effPcpCompactRespawn(EffPCPCompactFadeWork *work) {
    EffPCPCompactRectParams params;

    params.timeline.flags = work->flags;
    params.timeline.duration = work->duration;
    params.timeline.fadeIn = work->fadeIn;
    params.timeline.fadeOut = work->fadeOut;
    params.timeline.startExtent = work->startExtent;
    params.timeline.endExtent = work->endExtent;
    memcpy(&params.res, &work->resource.rectangle->params, sizeof(params.res));
    return effPcpCompactEffectCreate(&params);
}

void effPcpCompactEffectRelease(EffPCPCompactFadeWork *work) {
    effReleaseResourceTemplate(work->resource.rectangle);
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
void effPcpCompactEffectUpdate(EffPCPCompactFadeWork *work) {
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
            VU0_LOAD_VF(vf10, work->position);
            sdfProjectVuVectorToScreen();
            VU0_STORE_VF(vf10, projected);
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
        *(u32 *)rect->params.draw.color = effMultiplyPackedColors(
            effBlendColor(color & 0xFFFFFF, color, opacity), work->baseColor);
        effResourceRectDrawPixels(rect);
        work->frame++;
    }
}

void effPcpCopyCompactResourceRectVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetCompactColor(EffPCPCompactFadeWork *work, u32 value) {
    work->color = value;
}

/* Retain the compact fade input without claiming a scale or timing interpretation. */
void effPcpSetCompactFadeInput(EffPCPCompactFadeWork *work, f32 input) {
    work->unk34 = input;
}

EffPCPCompactFadeWork *effPcpCompactLongCreate(EffPCPCompactTexturedBlurParams *params) {
    EffPCPCompactFadeWork *work;

    work = sdfChipAllocDirect(0x3C);
    work->resource.blur = effCloneBlurTemplate(&params->res);
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

EffPCPCompactFadeWork *effPcpCompactLongCreateFromTable(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    return effPcpCompactLongCreate(param0);
}

EffPCPCompactFadeWork *effPcpCompactLongRespawn(EffPCPCompactFadeWork *work) {
    EffPCPCompactTexturedBlurParams params;

    params.timeline.flags = work->flags;
    params.timeline.duration = work->duration;
    params.timeline.fadeIn = work->fadeIn;
    params.timeline.fadeOut = work->fadeOut;
    params.timeline.startExtent = work->startExtent;
    params.timeline.endExtent = work->endExtent;
    params.res = work->resource.blur->body;
    return effPcpCompactLongCreate(&params);
}

void effPcpCompactLongRelease(EffPCPCompactFadeWork *work) {
    effReleaseBlurTemplate(work->resource.blur);
    sdfChipFreeDirect(work);
}

/* vu0 routine: project the compact textured rectangle's world-space center. */
void effPcpCompactLongUpdate(EffPCPCompactFadeWork *work) {
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
            VU0_LOAD_VF(vf10, work->position);
            sdfProjectVuVectorToScreen();
            VU0_STORE_VF(vf10, projected);
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
        rect->body.source.color = effMultiplyPackedColors(
            effBlendColor(color & 0xFFFFFF, color, opacity), work->baseColor);
        effDrawBlurPixelRectWithResource(rect);
        work->frame++;
    }
}

void effPcpCopyCompactTexturedBlurVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetLongCompactColor(EffPCPCompactFadeWork *work, u32 value) {
    work->color = value;
}

/* Retain the long compact fade input; leave the write-only field's meaning unknown. */
void effPcpSetLongCompactFadeInput(EffPCPCompactFadeWork *work, f32 input) {
    work->unk34 = input;
}

void *effPcpCopyWork(src)
    EffPCPFadeTimerParams *src;
{
    EffPCPFadeTimer *dst;

    dst = sdfChipAllocDirect(0x30);
    dst->params = *src;
    dst->colorFrom = 0x80808080;
    dst->frame = 0;
    dst->colorTo = src->color;
    return dst;
}

void *effPcpCreateFadeTimerFromTable(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    return effPcpCopyWork(param0);
}

void *effPcpRecreateFadeTimer(EffPCPFadeTimerParams *src) {
    return effPcpCopyWork(src);
}

void effPcpReleaseFadeTimerWork(EffPCPFadeTimer *work) {
    sdfChipFreeDirect(work);
}


extern void effDrawSolidRectangle(u32 *color);

/* Timeline update: blend factor ramps up over fadeIn frames, holds at 1.0, then
 * ramps down over the last fadeOut frames. */
void effPcpFadeTimerUpdate(EffPCPFadeTimer *work) {
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
    work->params.color = effMultiplyPackedColors(effBlendColor(work->colorFrom & 0xFFFFFF, work->colorFrom, t), work->colorTo);
    effDrawSolidRectangle(&work->params.color);
    work->frame++;
}

void effPcpFadeTimerSetSourceColor(EffPCPFadeTimer *work, u32 value) {
    work->colorFrom = value;
}

void *effPcpCopyWorkLong(src)
    EffPCPFadeTimerLongParams *src;
{
    EffPCPFadeTimerLong *dst;

    dst = sdfChipAllocDirect(0x40);
    dst->params = *src;
    dst->colorFrom = 0x80808080;
    dst->frame = 0;
    dst->colorTo = src->source.color;
    return dst;
}

void *effPcpCreateLongFadeTimerFromTable(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    return effPcpCopyWorkLong(param0);
}

void *effPcpRecreateLongFadeTimer(EffPCPFadeTimerLongParams *src) {
    return effPcpCopyWorkLong(src);
}

void effPcpReleaseLongFadeTimerWork(EffPCPFadeTimerLong *work) {
    sdfChipFreeDirect(work);
}


/* Same timeline as effPcpFadeTimerUpdate on the longer work layout. */
void effPcpFadeTimerLongUpdate(EffPCPFadeTimerLong *work) {
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
    work->params.source.color = effMultiplyPackedColors(effBlendColor(work->colorFrom & 0xFFFFFF, work->colorFrom, t), work->colorTo);
    effDrawBlurRectangle(&work->params.source);
    work->frame++;
}

void effPcpLongFadeTimerSetSourceColor(EffPCPFadeTimerLong *work, u32 value) {
    work->colorFrom = value;
}

EffPCPCompactWork *effPcpCreateCompactWorkFromParams(EffPCPCompactScatterParams *params) {
    EffPCPCompactWork *work;

    work = sdfChipAllocDirect(0x38);
    work->resource.scatter = effBlurCreateScatterWork(&params->res);
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

EffPCPCompactWork *effPcpCreateChargeWorkFromTable(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    return effPcpCreateCompactWorkFromParams(param0);
}


EffPCPCompactWork *effPcpChargeRespawn(EffPCPCompactWork *work) {
    EffPCPCompactScatterParams params;

    params.timeline.flags = work->flags;
    params.timeline.duration = work->duration;
    params.timeline.fadeIn = work->fadeIn;
    params.timeline.fadeOut = work->fadeOut;
    params.timeline.startExtent = work->startExtent;
    params.timeline.endExtent = work->endExtent;
    params.res = work->resource.scatter->params;
    return effPcpCreateCompactWorkFromParams(&params);
}

void effPcpReleaseCompactBlurWork(EffPCPCompactWork *work) {
    effBlurReleaseFirstResource(work->resource.scatter);
    sdfChipFreeDirect(work);
}

/* vu0 routine: grow the scatter region and project its optional world center. */
void effPcpUpdateCompactScatterWork(EffPCPCompactWork *work) {
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
        VU0_LOAD_VF(vf10, work->position);
        sdfProjectVuVectorToScreen();
        VU0_STORE_VF(vf10, projected);
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
    color = effMultiplyPackedColors(
        effBlendColor(color & 0xFFFFFF, color, opacity), work->baseColor);
    resource = work->resource.scatter;
    resource->params.color = color;
    effBlurStepScatterSlotsAndDraw(resource);
    work->frame++;
}

void effPcpCopyCompactBlurVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpCompactSetColor(EffPCPCompactWork *work, u32 color) {
    work->color = color;
}

EffPCPCompactWork *effPcpCreateCompactResourceWork(EffPCPCompactScaleParams *params) {
    EffPCPCompactWork *work;

    work = sdfChipAllocDirect(0x38);
    work->resource.scale = effCloneBlurWorkWithSlots(&params->res);
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

EffPCPCompactWork *effPcpCreateChargeResourceWorkFromTable(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    return effPcpCreateCompactResourceWork(param0);
}

EffPCPCompactWork *effPcpChargeLongRespawn(EffPCPCompactWork *work) {
    EffPCPCompactScaleParams params;

    params.timeline.flags = work->flags;
    params.timeline.duration = work->duration;
    params.timeline.fadeIn = work->fadeIn;
    params.timeline.fadeOut = work->fadeOut;
    params.timeline.startExtent = work->startExtent;
    params.timeline.endExtent = work->endExtent;
    params.res = work->resource.scale->params;
    return effPcpCreateCompactResourceWork(&params);
}

void effPcpReleaseSecondaryBlurWork(EffPCPCompactWork *work) {
    effBlurReleaseSecondResource(work->resource.scale);
    sdfChipFreeDirect(work);
}

/* vu0 routine: grow the scale slots and restart looping world-space bursts. */
void effPcpUpdateCompactScaleWork(EffPCPCompactWork *work) {
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
        effBlurSecondInitSlots(work->resource.scale);
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
        VU0_LOAD_VF(vf10, work->position);
        sdfProjectVuVectorToScreen();
        VU0_STORE_VF(vf10, projected);
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
    color = effMultiplyPackedColors(
        effBlendColor(color & 0xFFFFFF, color, opacity), work->baseColor);
    resource = work->resource.scale;
    resource->params.color = color;
    effBlurStepScaleSlotsAndDraw(resource);
    work->frame++;
}

void effPcpCopyShortThunderFadeVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetChargeResourceColor(EffPCPCompactWork *work, u32 color) {
    work->color = color;
}

void effPcpInitShortThunderFadeWork(EffPCPFadeWork *work) {
    EffThunderVectorWork *handle;

    handle = effCreateThunderCellSystemWork((EffThunderAlphaParams *)effShortThunderFadeParams);
    work->frame = 0;
    work->handle = handle;
}

void *effPcpCreateShortThunderFadeWork(void) {
    void *work;

    work = sdfChipAllocDirect(0x20);
    effPcpInitShortThunderFadeWork(work);
    return work;
}

void effPcpReleaseShortThunderFadeWork(EffPCPFadeWork *work) {
    effThunderReleaseVectorWork(work->handle);
    sdfChipFreeDirect(work);
}

void *effAllocateInitializedThunderFadeWork(void) {
    void *work;

    work = sdfChipAllocDirect(0x20);
    effPcpInitShortThunderFadeWork(work);
    return work;
}

void effPcpFadeUpdateShort(EffPCPFadeWork *work) {
    EffThunderParameterHead *parameterHead;
    f32 phase;
    f32 value;
    u32 fadedColor;
    u32 frame;

    frame = work->frame;
    if (frame < 0x1F) {
        parameterHead = effThunderGetParameterHead(work->handle);
        frame = work->frame;
        phase = (f32)frame / 30.0f;
        value = phase * 150.0f + 100.0f;
        parameterHead->vector.radiusScale = value;
        parameterHead->vector.heightScale = value;
    } else if (frame - 0x2D < 0x10) {
        parameterHead = effThunderGetParameterHead(work->handle);
        frame = work->frame;
        phase = (f32)(frame - 0x2D) / 15.0f;
        value = phase * 150.0f + 250.0f;
        parameterHead->vector.radiusScale = value;
        parameterHead->vector.heightScale = value;
    }
    if (frame >= 0x2D) {
        phase = (f32)(frame - 0x2D) / 35.0f;
        fadedColor = effBlendColor(work->color, 0, phase);
    } else {
        fadedColor = work->color;
    }
    effThunderSetVectorTint(work->handle, fadedColor);
    effThunderCopyVectorOrigin((void *)work->handle, work);
    effThunderUpdateVectorCells(work->handle);
    work->frame++;
}

void effPcpCopyScalingThunderFadeVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

/* Retain this fade variant's float input; its observed updater does not read it. */
void effPcpSetScalingThunderFadeInput(EffPCPFadeWork *work, f32 input) {
    work->unk10 = input;
}

void effPcpSetScalingThunderFadeColor(EffPCPFadeWork *work, u32 value) {
    work->color = value;
}

void effPcpInitScalingThunderFadeWork(EffPCPFadeWork *work) {
    EffThunderVectorWork *handle;

    handle = effCreateThunderCellSystemWork((EffThunderAlphaParams *)effScalingThunderFadeParams);
    work->frame = 0;
    work->handle = handle;
}

void *effPcpCreateScalingThunderFadeWork(void) {
    void *work;

    work = sdfChipAllocDirect(0x20);
    effPcpInitScalingThunderFadeWork(work);
    return work;
}

void effPcpReleaseScalingThunderFadeWork(EffPCPFadeWork *work) {
    effThunderReleaseVectorWork(work->handle);
    sdfChipFreeDirect(work);
}

void *effAllocateScalingThunderFadeWork(void) {
    void *work;

    work = sdfChipAllocDirect(0x20);
    effPcpInitScalingThunderFadeWork(work);
    return work;
}

/* Scales the target through two frame windows and fades the colour out from
 * frame 60. */
void effPcpThunderScaleFadeUpdate(EffPCPFadeWork *work) {
    EffThunderParameterHead *parameterHead;
    f32 phase;
    u32 fadedColor;
    u32 frame;

    frame = work->frame;
    if (frame < 0x2E) {
        parameterHead = effThunderGetParameterHead(work->handle);
        frame = work->frame;
        phase = (f32)frame / 45.0f;
        parameterHead->vector.heightScale = parameterHead->vector.radiusScale = phase * 950.0f + 200.0f;
    } else if (frame >= 0x3C && frame < 0x4C) {
        parameterHead = effThunderGetParameterHead(work->handle);
        frame = work->frame;
        phase = (f32)(frame - 0x3C) / 15.0f;
        parameterHead->vector.heightScale = parameterHead->vector.radiusScale = phase * 150.0f + 1050.0f;
    }
    if (frame >= 0x3C) {
        phase = (f32)(frame - 0x3C) / 15.0f;
        fadedColor = effBlendColor(work->color, 0, phase);
    } else {
        fadedColor = work->color;
    }
    effThunderSetVectorTint(work->handle, fadedColor);
    effThunderCopyVectorOrigin((void *)work->handle, work);
    effThunderUpdateVectorCells(work->handle);
    work->frame++;
}

void effPcpCopyThunderScaleFadeVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

/* Retain this fade variant's float input; its observed updater does not read it. */
void effPcpSetThunderScaleFadeInput(EffPCPFadeWork *work, f32 input) {
    work->unk10 = input;
}

void effPcpSetThunderScaleFadeColor(EffPCPFadeWork *work, u32 value) {
    work->color = value;
}

/* The update uses unsigned frame bounds; the initial sweep interval is
 * computed as a signed difference of the parameter words. */

EffPCPSpanWork *effPcpSpanCreate(void *params, EffNodeDescriptor *handleParams) {
    EffPCPSpanParams *src = params;
    EffPCPSpanWork *work;
    f32 size;

    work = sdfChipAllocDirect(0x78);
    work->params = *src;
    work->color = 0x80808080;
    work->frame = 0;
    size = work->params.sweepDegrees * 0.017453292f;
    work->angle = -(size * 0.5f);
    work->spin = size / (f32)((s32)work->params.activeFrames - ((work->params.fadeFrames >> 1) + (s32)work->params.holdFrames));
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->optionalHandle = 0;
    if (handleParams != NULL) {
        work->optionalHandle = effCreateNodeFromDescriptor(handleParams);
    }
    return work;
}

EffPCPSpanWork *effPcpSpanCreateFromTable(void *args) {
    void *param0;
    void *param1;

    param0 = effParamTableGetBlock(args, 0);
    param1 = effParamTableGetBlock(args, 1);
    return effPcpSpanCreate(param0, param1);
}

EffPCPSpanWork *effPcpCloneWithOptionalHandle(EffPCPSpanWork *work) {
    EffPCPSpanWork *child;

    child = effPcpSpanCreate(&work->params, NULL);
    if (work->optionalHandle != NULL) {
        child->optionalHandle = effCloneSourceWithTypeHandler(work->optionalHandle);
    }
    return child;
}

void effPcpReleaseOptionalHandle(EffPCPSpanWork *work) {
    if (work->optionalHandle != NULL) {
        effDestroyNode(work->optionalHandle);
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
        VU0_STORE_VF(vf10, center);
        btlUnitGetMuzzlePosVU((BtlUnit *)effBTLFieldColorGetOriginalSelector());
        VU0_STORE_VF(vf10, muzzle);
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
        VU0_LOAD_VF(vf10, aim);
        VU0_ROTATE_VEC(vf10, vf10);
        VEC3_SPLAT(muzzle, params->radius);
        VU0_LOAD_VF(vf11, muzzle);
        VU0_MUL(vf10, vf10, vf11);
        VU0_LOAD_VF(vf11, params->anchor);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, muzzle);
        effCopyVectorToNodeInstance(node, muzzle);
        if (work->frame > params->holdFrames) {
            work->angle += work->spin;
        }
        sdfVuLoadRotationMatrixY(work->angle);
        VU0_STORE_MATRIX(mtx);
        effApplyNodeTransformMatrix(node, mtx);
        if (fadeFrames >= remaining && fadeFrames != 0) {
            t = (f32)remaining / (f32)fadeFrames;
        } else {
            t = 1.0f;
        }
        effSetNodeParameterValue((struct EffNode *)node, effBlendColor(work->color & 0xFFFFFF, work->color, t));
        effUpdateNode((struct EffNode *)node);
    }
    work->frame++;
}

void effPcpSpanSetHeadVector(void *work, void *src) {
    PCP_COPY_VECTOR(((EffPCPSpanWork *)work)->params.anchor, src);
}

void effPcpSpanSetColor(EffPCPSpanWork *work, u32 value) {
    work->color = value;
}

void effPcpCopyHalfTurnMatrix(void *dst, void *src) {
    sdfVuLoadRotationMatrixY(3.1415927f);
        VU0_LOAD_MATRIX_B(src);
;
    sdfComposeVuMatrixFromRegisters();
        VU0_STORE_MATRIX(dst);
;
}

EffPCPTripleWork *effPcpTripleHandleCreate(EffPCPTripleParams *params, EffNodeDescriptor **descriptors) {
    EffPCPTripleWork *work;
    struct EffNode **handle;
    u32 i;

    work = sdfChipAllocDirect(0xAC);
    work->head = *params;
    work->frame = 0;
    work->color = 0x80808080;
    handle = work->handles;
    for (i = 0; i < 7; i++) {
        handle[0] = effCreateNodeFromDescriptor(descriptors[i]);
        handle[7] = effCloneSourceWithTypeHandler(handle[0]);
        handle[14] = effCloneSourceWithTypeHandler(handle[0]);
        handle++;
    }
    return work;
}

EffPCPTripleWork *effPcpTripleHandleCreateFromTable(void *data) {
    EffPCPTripleParams *params;
    EffNodeDescriptor *descriptors[7];
    EffNodeDescriptor **dst;
    u32 i;

    params = effParamTableGetBlock(data, 0);
    dst = descriptors;
    i = 0;
    do {
        i++;
        *dst = effParamTableGetBlock(data, i);
        dst++;
    } while (i < 7);
    return effPcpTripleHandleCreate(params, descriptors);
}

EffPCPTripleWork *effPcpTripleHandleDuplicate(EffPCPTripleWork *src) {
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
        destinationHandleCursor[-14] = effCloneSourceWithTypeHandler(sourceHandleCursor[-14]);
        destinationHandleCursor[-7] = effCloneSourceWithTypeHandler(sourceHandleCursor[-7]);
        destinationHandleCursor[0] = effCloneSourceWithTypeHandler(sourceHandleCursor[0]);
        sourceHandleCursor++;
        destinationHandleCursor++;
    }
    return work;
}

void effPcpTripleHandleRelease(EffPCPTripleWork *work) {
    struct EffNode **handleCursor = work->handles;
    u32 slotIndex;

    for (slotIndex = 0; slotIndex < 7; slotIndex++) {
        effDestroyNode(handleCursor[14]);
        effDestroyNode(handleCursor[7]);
        effDestroyNode(handleCursor[0]);
        handleCursor++;
    }
    sdfChipFreeDirect(work);
}

extern u8 sdfViewUpVector[];

/* vu0 routine: place three staggered groups in the camera-relative basis. */
void effPcpUpdateCameraRelativeTripleGroups(EffPCPTripleWork *work) {
    f32 position[4] __attribute__((aligned(16)));
    f32 basis[4][4] __attribute__((aligned(16)));
    s32 frame = work->frame;
    u32 color = work->color;
    u32 group;
    u32 handleIndex;

    sdfVuBuildLookAtBasis(sdfViewEyeVector, sdfViewTargetVector, sdfViewUpVector);
    VU0_STORE_MATRIX(basis);
    for (group = 0; group < 3; group++) {
        s32 start = work->head.groupStartFrame[group];
        if (frame >= start) {
            VU0_LOAD_VF(vf11, work->head.origin);
            VU0_LOAD_MATRIX(basis);
            position[0] = work->head.offset[group][0];
            position[1] = work->head.offset[group][1];
            position[2] = 0;
            VU0_LOAD_VF_MEMORY(vf10, position);
            VU0_CLEAR_W(vf10);
            VU0_APPLY_MATRIX(vf10, vf10);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, position);
            for (handleIndex = 0; handleIndex < 7; handleIndex++) {
                if (frame >= (s32)((u32)work->head.handleDelay[handleIndex] + (u32)start)) {
                    effSetNodeParameterValue((struct EffNode *)work->handles[group * 7 + handleIndex], color);
                    effCopyVectorToNodeInstance((struct EffNode *)work->handles[group * 7 + handleIndex], position);
                    effUpdateNode((struct EffNode *)work->handles[group * 7 + handleIndex]);
                }
            }
        }
    }
    work->frame = (s32)((u32)work->frame + 1);
}

void effPcpCopySpanVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpTripleHandleSetColor(EffPCPTripleWork *work, u32 value) {
    work->color = value;
}



/* Build a block-set work: copy the parameter block, then create one parameter handle per input block. */
EffPCPBlockSetWork *effPcpCreateBlockSetWork(const EffPCPBlockSetParams *params, void **blocks) {
    EffPCPBlockSetWork *work;
    MdlCtx *model;
    u32 i;
    u32 j;
    u32 n;

    work = sdfChipAllocDirect(0x10C);
    memset(work, 0, 0x10C);
    work->params = *params;
    work->frame = 0;
    work->color = 0x80808080;
    work->mode = 0;
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->headHandle = effParamWorkCreate(EFF_PARAM_WORK_KIND_VIEWER_CONTEXT, blocks[0]);
    work->handleA[0] = effParamWorkCreate(EFF_PARAM_WORK_KIND_EFFECT_NODE, blocks[1]);
    work->handleA[1] = effParamWorkCreate(EFF_PARAM_WORK_KIND_EFFECT_NODE, blocks[2]);
    work->handleA[2] = effParamWorkCreate(EFF_PARAM_WORK_KIND_EFFECT_NODE, blocks[3]);
    work->handleA[3] = effParamWorkCreate(EFF_PARAM_WORK_KIND_EFFECT_NODE, blocks[4]);
    work->handleA[4] = effParamWorkCreate(EFF_PARAM_WORK_KIND_VIEWER_CONTEXT, blocks[5]);
    model = effParamWorkGetData(work->headHandle);
    work->count = model->first->frameCount;
    for (i = 0; i < 3; i++) {
        if (work->params.groupSize[i] > 0) {
            n = work->count * work->params.groupSize[i];
            work->alloc[i] = sdfMemAllocDirect(n * 4);
            work->list[i] = (EffParamWork **)sdfMemLock(work->alloc[i]);
            work->list[i][0] = effParamWorkCreate(EFF_PARAM_WORK_KIND_EFFECT_NODE, blocks[6 + i]);
            for (j = 1; j < n; j++) {
                work->list[i][j] = 0;
            }
        } else {
            work->alloc[i] = NULL;
        }
    }
    work->handleB[0] = effParamWorkCreate(EFF_PARAM_WORK_KIND_EFFECT_NODE, blocks[9]);
    work->handleB[1] = effParamWorkCreate(EFF_PARAM_WORK_KIND_EFFECT_NODE, blocks[10]);
    work->handleB[2] = effParamWorkCreate(EFF_PARAM_WORK_KIND_EFFECT_NODE, blocks[11]);
    work->handleB[3] = effParamWorkCreate(EFF_PARAM_WORK_KIND_EFFECT_NODE, blocks[12]);
    work->handleB[4] = effParamWorkCreate(EFF_PARAM_WORK_KIND_EXTENDED_WORK_WITH_MATRIX_CALLBACK, blocks[13]);
    work->tailHandle = effParamWorkCreate(EFF_PARAM_WORK_KIND_EFFECT_NODE, blocks[14]);
    return work;
}

EffPCPBlockSetWork *effPcpBuildBlockSet(void *parameterTable) {
    EffPCPBlockSetInputs set;
    EffPCPBlockSetParams *first;
    u32 i;

    first = effParamTableGetBlock(parameterTable, 0);
    set.named.block1 = effParamTableGetBlock(parameterTable, 1);
    for (i = 0; i < 5; i++) {
        set.named.group[i] = effParamTableGetBlock(parameterTable, 2 + i);
    }
    set.named.block7 = effParamTableGetBlock(parameterTable, 7);
    set.named.block8 = effParamTableGetBlock(parameterTable, 8);
    set.named.block9 = effParamTableGetBlock(parameterTable, 9);
    for (i = 0; i < 5; i++) {
        set.named.tail[i] = effParamTableGetBlock(parameterTable, 10 + i);
    }
    set.named.block15 = effParamTableGetBlock(parameterTable, 15);
    return effPcpCreateBlockSetWork(first, set.blocks);
}

/* Duplicate the block-set handles and allocate its three optional instance lists. */
void effPcpDuplicateBlockSetHandles(EffPCPBlockSetWork *work, EffPCPBlockSetWork *src) {
    MdlCtx *headModel;
    u32 handleIndex;
    u32 groupIndex;
    u32 groupHandleCount;

    work->headHandle = effParamWorkDuplicate(src->headHandle);
    for (handleIndex = 0; handleIndex < ARRAY_COUNT(work->handleA); handleIndex++) {
        work->handleA[handleIndex] = effParamWorkDuplicate(src->handleA[handleIndex]);
    }
    headModel = effParamWorkGetData(work->headHandle);
    work->count = headModel->first->frameCount;
    for (groupIndex = 0; groupIndex < ARRAY_COUNT(work->list); groupIndex++) {
        if (work->params.groupSize[groupIndex] > 0) {
            groupHandleCount = work->count * work->params.groupSize[groupIndex];
            work->alloc[groupIndex] = sdfMemAllocDirect(groupHandleCount * 4);
            work->list[groupIndex] = (EffParamWork **)sdfMemLock(work->alloc[groupIndex]);
            work->list[groupIndex][0] = effParamWorkDuplicate(src->list[groupIndex][0]);
            for (handleIndex = 1; handleIndex < groupHandleCount; handleIndex++) {
                work->list[groupIndex][handleIndex] = 0;
            }
        } else {
            work->alloc[groupIndex] = NULL;
        }
    }
    for (handleIndex = 0; handleIndex < ARRAY_COUNT(work->handleB); handleIndex++) {
        work->handleB[handleIndex] = effParamWorkDuplicate(src->handleB[handleIndex]);
    }
    work->tailHandle = effParamWorkDuplicate(src->tailHandle);
}




/* Clone of a block-set work that shares the source's blocks (no back pointer). */
EffPCPBlockSetWork *effPcpBlockSetCloneShared(EffPCPBlockSetWork *src) {
    EffPCPBlockSetWork *work;

    work = sdfChipAllocDirect(0x10C);
    memset(work, 0, 0x10C);
    work->params = src->params;
    work->frame = 0;
    work->color = 0x80808080;
    work->mode = 0;
    EE_MMI_UNIT_MATRIX(work->matrix);
    effPcpDuplicateBlockSetHandles(work, src);
    work->source = NULL;
    return work;
}

void effPcpBlockSetWorkRelease(EffPCPBlockSetWork *work) {
    u32 groupIndex;
    u32 handleIndex;
    u32 groupHandleCount;

    if (work->source == NULL) {
        effDispatchParameterDataAndFreeWork(work->headHandle);
        for (handleIndex = 0; handleIndex < 5; handleIndex++) {
            effDispatchParameterDataAndFreeWork(work->handleA[handleIndex]);
        }
        for (groupIndex = 0; groupIndex < 3; groupIndex++) {
            if (work->alloc[groupIndex] != 0) {
                groupHandleCount = work->count * work->params.groupSize[groupIndex];
                for (handleIndex = 0; handleIndex < groupHandleCount; handleIndex++) {
                    if (work->list[groupIndex][handleIndex] != 0) {
                        effDispatchParameterDataAndFreeWork(work->list[groupIndex][handleIndex]);
                    }
                }
                sdfMemFreeDirect(work->alloc[groupIndex]);
            }
        }
        for (handleIndex = 0; handleIndex < 5; handleIndex++) {
            effDispatchParameterDataAndFreeWork(work->handleB[handleIndex]);
        }
        effDispatchParameterDataAndFreeWork(work->tailHandle);
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
    MdlCtx *model;
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
        effPcpDuplicateBlockSetHandles(work, source);
        work->source = NULL;
    }
    if (frame == 0) {
        for (i = 0; i < 5; i++) {
            effParamWorkCallback1(work->handleA[i], work->params.scale);
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
    color = effBlendColor(color & 0xFFFFFF, color, fade);
    model = effParamWorkGetData(work->headHandle);
    if (work->mode == 0) {
        effParamWorkCallback0(work->headHandle, work->params.position);
        if (effBTLFieldColorUsesActorSelectors()) {
            btlUnitGetMuzzlePosVU((void *)effBTLFieldColorGetOriginalSelector());
            VU0_STORE_VF_UNCLOBBERED(vf10, muzzle);
            sdfVuBuildLookAtBasis(work->params.position, muzzle, D_003B1AE0);
            sdfInvertRigidVuTransform();
            VU0_MOVE_VF(vf31, vf0);
            VU0_STORE_MATRIX(matrix);
        } else {
            /* Keep the VU source load and destination store as separate stages. */
            VU0_LOAD_MATRIX_MEMORY(work->matrix);
            VU0_STORE_MATRIX(matrix);
        }
    }
    effParamWorkCallback2(work->headHandle, matrix);
    effParamWorkCallback3(work->headHandle, color);
    mdlProcessContextNodesAndTransforms(model, D_00380828);
    PCP_COPY_VECTOR(work->previousPosition, work->currentPosition);
    sdfLoadMapRecordPositionVector(model->inner, 1);
    VU0_STORE_VF(vf10, work->currentPosition);
    if (frame < count && frame > 0) {
        for (i = 0; i < 5; i++) {
            if (i == 4) {
                if (work->mode == 1 || work->mode == 2) {
                    effParamWorkCallback2(work->handleA[i], matrix);
                }
                effParamWorkCallback0(work->handleA[i], work->previousPosition);
            } else {
                effParamWorkCallback0(work->handleA[i], work->currentPosition);
            }
            effParamWorkCallback3(work->handleA[i], color);
            effParamWorkInvokeCallback(work->handleA[i]);
        }
    }
    if (frame >= 0) {
        if (frame == 0) {
            for (i = 0; i < 5; i++) {
                effParamWorkCallback0(work->handleB[i], work->currentPosition);
                effParamWorkCallback2(work->handleB[i], work->matrix);
            }
        }
        for (i = 0; i < 5; i++) {
            effParamWorkCallback3(work->handleB[i], work->color);
            effParamWorkInvokeCallback(work->handleB[i]);
        }
    }
    if (frame >= work->params.tailStartFrame) {
        if (frame == work->params.tailStartFrame) {
            effParamWorkCallback0(work->tailHandle, work->currentPosition);
            effParamWorkCallback2(work->tailHandle, matrix);
        }
        effParamWorkCallback3(work->tailHandle, work->color);
        effParamWorkInvokeCallback(work->tailHandle);
    }
    if (frame > 0) {
        for (j = 0; j < 3; j++) {
            if (work->params.groupSize[j] > 0) {
                if (frame < count) {
                    base = (work->frame - 1) * work->params.groupSize[j];
                    VU0_LOAD_VF(vf10, work->currentPosition);
                    VU0_LOAD_VF(vf11, work->previousPosition);
                    VU0_SUB(vf10, vf10, vf11);
                    VU0_MOVE_VF(vf12, vf10);
                    VEC3_SPLAT(step, 1.0f / work->params.groupSize[j]);
                    VU0_LOAD_VF(vf11, step);
                    VU0_MUL(vf10, vf10, vf11);
                    VU0_STORE_VF(vf10, step);
                    PCP_COPY_VECTOR(position, work->previousPosition);
                    VU0_LOAD_VF(vf10, sdfViewTargetVector);
                    VU0_LOAD_VF(vf11, sdfViewEyeVector);
                    VU0_SUB(vf10, vf10, vf11);
                    VU0_MOVE_VF(vf11, vf12);
                    VU0_CROSS_XYZ(vf10, vf10, vf11);
                    VU0_NORMALIZE_VF10();
                    VU0_STORE_VF(vf10, side);
                    for (i = 0; i < work->params.groupSize[j]; i++) {
                        if (work->list[j][base + i] == 0) {
                            work->list[j][base + i] = effParamWorkDuplicate(work->list[j][0]);
                            scale = work->params.scale * work->params.groupScale[j];
                            randomScale = work->params.groupRandomScale[j];
                            effParamWorkCallback1(work->list[j][base + i], scale *
                                (sdfGetRandFloat(effDefaultRandomState) * randomScale + (1.0f - randomScale)));
                        }
                        VU0_LOAD_VF(vf10, position);
                        VU0_LOAD_VF(vf11, step);
                        VU0_ADD(vf10, vf10, vf11);
                        VU0_MOVE_VF(vf12, vf10);
                        randomOffset = sdfGetRandFloat(effDefaultRandomState) - 0.5f;
                        randomOffset = randomOffset + randomOffset;
                        VEC3_SPLAT(scatter, work->params.groupScatter[j] * randomOffset);
                        VU0_LOAD_VF(vf10, side);
                        VU0_LOAD_VF(vf11, scatter);
                        VU0_MUL(vf10, vf10, vf11);
                        VU0_ADD(vf10, vf10, vf12);
                        /* Fully written here, then consumed by VU loads and callbacks. */
                        VU0_STORE_VF_UNCLOBBERED(vf10, position);
                        effParamWorkCallback0(work->list[j][base + i], position);
                        effParamWorkCallback3(work->list[j][base + i], work->color);
                    }
                } else {
                    base = (count - 1) * work->params.groupSize[j];
                }
                for (i = 0; i < base; i++) {
                    if (work->list[j][i] == 0) {
                        work->list[j][i] = effParamWorkDuplicate(work->list[j][0]);
                        scale = work->params.scale * work->params.groupScale[j];
                        randomScale = work->params.groupRandomScale[j];
                        effParamWorkCallback1(work->list[j][i], scale *
                            (sdfGetRandFloat(effDefaultRandomState) * randomScale + (1.0f - randomScale)));
                    }
                    effParamWorkCallback2(work->list[j][i], work->matrix);
                    effParamWorkInvokeCallback(work->list[j][i]);
                }
            }
        }
    }
    work->frame++;
}

void effPcpCopyVector60(EffPCPBlockSetWork *work, const void *src) {
    PCP_COPY_VECTOR(work->params.position, src);
}

void effPcpBlockSetSetColor(EffPCPBlockSetWork *work, u32 value) {
    work->color = value;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effPcpCopyBlockMatrix(EffPCPBlockSetWork *work, const void *src) {
    VU0_COPY_MATRIX(work->matrix, src);
}

/* Construct the mode-one block set; the native updater treats modes one and two alike. */
EffPCPBlockSetWork *effPcpCreateBlockSetMode1(void *parameterTable) {
    EffPCPBlockSetWork *work;

    work = effPcpBuildBlockSet(parameterTable);
    work->mode = 1;
    return work;
}

EffPCPBlockSetWork *effPcpCloneBlockWithUnitMatrix(EffPCPBlockSetWork *src) {
    EffPCPBlockSetWork *work;

    work = sdfChipAllocDirect(0x10C);
    memset(work, 0, 0x10C);
    work->params = src->params;
    work->color = 0x80808080;
    work->mode = 1;
    work->frame = 0;
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->source = src;
    return work;
}

/* Construct the mode-two block set; no further semantic distinction is established. */
EffPCPBlockSetWork *effPcpCreateBlockSetMode2(void *parameterTable) {
    EffPCPBlockSetWork *work;

    work = effPcpBuildBlockSet(parameterTable);
    work->mode = 2;
    return work;
}

EffPCPBlockSetWork *effCloneBlockWorkFromSource(EffPCPBlockSetWork *src) {
    EffPCPBlockSetWork *work;

    work = sdfChipAllocDirect(0x10C);
    memset(work, 0, 0x10C);
    work->params = src->params;
    work->color = 0x80808080;
    work->mode = 2;
    work->frame = 0;
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->source = src;
    return work;
}

EffPCPRotateWork *effPcpRotateCreate(EffPCPRotateParams *src, EffPCPRotateBlockInputs *blocks) {
    EffPCPRotateWork *work;
    EffPCPBlockSetParams *sub;
    u32 i;

    work = sdfChipAllocDirect(0x10C);
    work->params = *src;
    sub = src->group;
    work->frame = 0;
    for (i = 0; i < 3; i++) {
        blocks->selectedHead = blocks->head[i];
        work->blockSets[i] = effPcpCreateBlockSetWork(sub, &blocks->selectedHead);
        sub++;
    }
    return work;
}

/* Gathers 17 parameter blocks from the effect table (the first goes to the
 * rotate effect as its source, the rest into an on-stack block set). */
void effPcpRotateCreateFromTable(args)
    void *args;
{
    EffPCPRotateBlockInputs set;
    void **group;
    void *first;
    s32 idx;
    u32 i;
    u32 j;

    idx = 1;
    first = effParamTableGetBlock(args, 0);
    for (i = 0; i < 3; i++) {
        set.head[i] = effParamTableGetBlock(args, idx++);
    }
    group = set.group;
    for (j = 0; j < 5; j++) {
        group[j] = effParamTableGetBlock(args, idx++);
    }
    for (j = 0; j < 3; j++) {
        set.mid[j] = effParamTableGetBlock(args, idx++);
    }
    for (j = 0; j < 5; j++) {
        group[j + 8] = effParamTableGetBlock(args, idx++);
    }
    set.last = effParamTableGetBlock(args, idx);
    effPcpRotateCreate(first, &set);
}

/* Clones the rotate work: block copy of the head plus one fresh block-clone work
 * per id, each initialised from the source's sub-work. */
EffPCPRotateWork *effPcpRotateClone(EffPCPRotateWork *src) {
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
        sub->mode = 0;
        EE_MMI_UNIT_MATRIX(sub->matrix);
        sub->source = src->blockSets[i];
    }
    return work;
}

void effPcpRotateRelease(EffPCPRotateWork *work) {
    u32 i;
    EffPCPBlockSetWork **blockSets;

    blockSets = work->blockSets;
    for (i = 0; i < 3; i++) {
        effPcpBlockSetWorkRelease(blockSets[i]);
    }
    sdfChipFreeDirect(work);
}

/* Dispatch children whose start threshold has been reached. Reload the shared
   frame after each callback before checking the next child. */
void effPcpRotateFireIds(EffPCPRotateWork *work) {
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

void effPcpRotateSetChildVectors(EffPCPRotateWork *work, const void *src) {
    u32 i;
    EffPCPBlockSetWork **blockSets;

    blockSets = work->blockSets;
    for (i = 0; i < 3; i++) {
        effPcpCopyVector60(blockSets[i], src);
    }
}

void effPcpRotateSetChildValues(EffPCPRotateWork *work, u32 val) {
    u32 i;
    EffPCPBlockSetWork **blockSets;

    blockSets = work->blockSets;
    for (i = 0; i < 3; i++) {
        effPcpBlockSetSetColor(blockSets[i], val);
    }
}

void effPcpRotateSetChildMatrices(EffPCPRotateWork *work, const void *src) {
    u32 i;
    EffPCPBlockSetWork **blockSets;

    blockSets = work->blockSets;
    for (i = 0; i < 3; i++) {
        effPcpCopyBlockMatrix(blockSets[i], src);
    }
}

EffPCPSpinWork *effSpinSingleCreateFromTable(void *src) {
    EffPCPSpinWork *work = sdfChipAllocDirect(0x64);

    work->frame = 0;
    work->scale = 1.0f;
    work->color = 0x80808080;
    work->handle0 = effParamCreateFromTable(src, 0);
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->angle = (sdfGetRandFloat(effDefaultRandomState) - 0.5f) * 2.0f * 0.87266457f;
    if (sdfGetRand((EffRandState *)effDefaultRandomState) & 1) {
        work->angle += 3.14159265f;
    }
    return work;
}

/* Single-handle variant: allocation stops before handle1 (0x64 bytes). */
EffPCPSpinWork *effSpinSingleClone(EffPCPSpinWork *src) {
    EffPCPSpinWork *work = sdfChipAllocDirect(0x64);

    work->frame = 0;
    work->scale = 1.0f;
    work->color = 0x80808080;
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->angle = (sdfGetRandFloat(effDefaultRandomState) - 0.5f) * 2.0f * 0.87266457f;
    if (sdfGetRand((EffRandState *)effDefaultRandomState) & 1) {
        work->angle += 3.14159265f;
    }
    work->handle0 = effParamWorkDuplicate(src->handle0);
    return work;
}

void effSpinSingleRelease(EffPCPSpinWork *work) {
    effDispatchParameterDataAndFreeWork(work->handle0);
    sdfChipFreeDirect(work);
}

void effSpinEffectUpdate(EffPCPSpinWork *work) {
    EffParamWork *handle = work->handle0;
    u128 mtx[4];

    effParamWorkCallback0(handle, work);
    effParamWorkCallback1(handle, work->scale);
    effParamWorkCallback3(handle, work->color);
    sdfVuLoadRotationMatrixY(work->angle);
        VU0_LOAD_MATRIX_B(work->matrix);
;
    sdfComposeVuMatrixFromRegisters();
        VU0_STORE_MATRIX(mtx);
;
    effParamWorkCallback2(handle, mtx);
    effParamWorkInvokeCallback(handle);
    work->frame++;
}

void effPcpCopySpinSingleVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effSpinSingleSetColor(EffPCPSpinWork *work, u32 color) {
    work->color = color;
}

void effSpinSingleSetScale(EffPCPSpinWork *work, f32 val) {
    work->scale = val;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effCopyMatrix(EffPCPSpinWork *work, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_STORE_MATRIX(work->matrix);
}

EffPCPSpinWork *effSpinEffectCreateFromTable(void *table) {
    EffPCPSpinWork *work = sdfChipAllocDirect(0x68);

    work->frame = 0;
    work->scale = 1.0f;
    work->color = 0x80808080;
    work->handle0 = effParamCreateFromTable(table, 0);
    work->handle1 = effParamCreateFromTable(table, 1);
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->angle = 2.0f * (sdfGetRandFloat(effDefaultRandomState) - 0.5f) * 0.87266457f;
    return work;
}

EffPCPSpinWork *effSpinEffectClone(EffPCPSpinWork *src) {
    EffPCPSpinWork *work = sdfChipAllocDirect(0x68);

    work->scale = 1.0f;
    work->frame = 0;
    work->color = 0x80808080;
    work->handle0 = effParamWorkDuplicate(src->handle0);
    work->handle1 = effParamWorkDuplicate(src->handle1);
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->angle = 2.0f * (sdfGetRandFloat(effDefaultRandomState) - 0.5f) * 0.87266457f;
    return work;
}

void effSpinPairRelease(EffPCPSpinWork *work) {
    effDispatchParameterDataAndFreeWork(work->handle1);
    effDispatchParameterDataAndFreeWork(work->handle0);
    sdfChipFreeDirect(work);
}

/* Per-frame update of a two-handle spin effect: pushes the work state to both
 * handles, rebuilds the rotation matrix from the angle and hands it over. The
 * VU0 blocks are bare asm (no "memory" clobber), as the original macros were. */
void effSpinPairUpdateDelayed(EffPCPSpinWork *work) {
    EffParamWork *handle[2];
    u128 mtx[4];

    handle[0] = work->handle0;
    handle[1] = work->handle1;
    effParamWorkCallback0(handle[0], work);
    effParamWorkCallback0(handle[1], work);
    effParamWorkCallback1(handle[0], work->scale);
    effParamWorkCallback1(handle[1], work->scale);
    effParamWorkCallback3(handle[0], work->color);
    effParamWorkCallback3(handle[1], work->color);
    sdfVuLoadRotationMatrixY(work->angle);
            VU0_LOAD_MATRIX_B(work->matrix);

    sdfComposeVuMatrixFromRegisters();
            VU0_STORE_MATRIX_UNCLOBBERED(mtx);

    effParamWorkCallback2(handle[0], mtx);
    effParamWorkCallback2(handle[1], mtx);
    effParamWorkInvokeCallback(handle[0]);
    if (work->frame >= 0x1F) {
        effParamWorkInvokeCallback(handle[1]);
    }
    work->frame++;
}

void effPcpCopySpinPairVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effSpinPairSetColor(EffPCPSpinWork *work, u32 color) {
    work->color = color;
}

void effSpinPairSetScale(EffPCPSpinWork *work, f32 val) {
    work->scale = val;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effSpinPairSetMatrix(EffPCPSpinWork *work, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_STORE_MATRIX(work->matrix);
}

EffPCPSpinWork *effPcpCreateSpinningPair(void *table) {
    EffPCPSpinWork *work = sdfChipAllocDirect(0x68);

    work->frame = 0;
    work->scale = 1.0f;
    work->color = 0x80808080;
    work->handle0 = effParamCreateFromTable(table, 0);
    work->handle1 = effParamCreateFromTable(table, 1);
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->angle = 2.0f * (sdfGetRandFloat(effDefaultRandomState) - 0.5f) * 0.87266457f;
    return work;
}

EffPCPSpinWork *effPcpCloneSpinningPair(EffPCPSpinWork *src) {
    EffPCPSpinWork *work = sdfChipAllocDirect(0x68);

    work->scale = 1.0f;
    work->frame = 0;
    work->color = 0x80808080;
    work->handle0 = effParamWorkDuplicate(src->handle0);
    work->handle1 = effParamWorkDuplicate(src->handle1);
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->angle = 2.0f * (sdfGetRandFloat(effDefaultRandomState) - 0.5f) * 0.87266457f;
    return work;
}

void effPcpReleaseSpinningPair(EffPCPSpinWork *work) {
    effDispatchParameterDataAndFreeWork(work->handle1);
    effDispatchParameterDataAndFreeWork(work->handle0);
    sdfChipFreeDirect(work);
}

/* Identical update for the second spin effect. */
void effPcpUpdateSpinningPair(EffPCPSpinWork *work) {
    EffParamWork *handle[2];
    u128 mtx[4];

    handle[0] = work->handle0;
    handle[1] = work->handle1;
    effParamWorkCallback0(handle[0], work);
    effParamWorkCallback0(handle[1], work);
    effParamWorkCallback1(handle[0], work->scale);
    effParamWorkCallback1(handle[1], work->scale);
    effParamWorkCallback3(handle[0], work->color);
    effParamWorkCallback3(handle[1], work->color);
    sdfVuLoadRotationMatrixY(work->angle);
            VU0_LOAD_MATRIX_B(work->matrix);

    sdfComposeVuMatrixFromRegisters();
            VU0_STORE_MATRIX_UNCLOBBERED(mtx);

    effParamWorkCallback2(handle[0], mtx);
    effParamWorkCallback2(handle[1], mtx);
    effParamWorkInvokeCallback(handle[0]);
    if (work->frame >= 0x1F) {
        effParamWorkInvokeCallback(handle[1]);
    }
    work->frame++;
}

void effPcpCopySpinningPairVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetSpinColor(EffPCPSpinWork *work, u32 color) {
    work->color = color;
}

void effPcpSetSpinScale(EffPCPSpinWork *work, f32 val) {
    work->scale = val;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effPcpSetSpinMatrix(EffPCPSpinWork *work, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_STORE_MATRIX(work->matrix);
}

/* Creation-slot callback: enable the global effect flag and return a NULL payload. */
u32 effPcpCreateGlobalEffectFlag(void) {
    D_0043643C = 1;
    return 0;
}

/* Clone-slot callback: enable the same flag and return a NULL payload, without copying work. */
u32 effPcpCloneGlobalEffectFlag(void) {
    D_0043643C = 1;
    return 0;
}

/* Release-slot callback is intentionally empty; the update callback clears the global flag. */
void effPcpGlobalEffectFlagReleaseNoOp(void) {
}

void effPcpClearGlobalEffectFlag(void) {
    if (D_0043643C != 0) {
        D_0043643C = 0;
    }
}

EffPCPBurstWork *effAllocateThunderFragmentWork(u32 unused) {
    EffPCPBurstWork *work;

    work = sdfChipAllocDirect(0x24);
    work->startDistance = 0;
    work->length = 375.0f;
    work->fragmentWork = effThunderFragCreate((EffThunderFragmentParams *)effMuzzleThunderRayParams);
    return work;
}

EffPCPBurstWork *effPcpSpawnMuzzleThunderRay(void) {
    return effAllocateThunderFragmentWork(0);
}

EffPCPBurstWork *effAllocateThunderFragmentBurstWork(u32 unused) {
    EffPCPBurstWork *work;

    work = sdfChipAllocDirect(0x24);
    work->startDistance = 325.0f;
    work->length = 225.0f;
    work->fragmentWork = effThunderFragCreate((EffThunderFragmentParams *)effOffsetMuzzleThunderRayParams);
    return work;
}

EffPCPBurstWork *effPcpSpawnOffsetMuzzleThunderRay(void) {
    return effAllocateThunderFragmentBurstWork(0);
}

EffPCPBurstWork *effPcpCreateCyanBlueFragmentPair(u32 unused) {
    EffPCPBurstWork *work;

    work = sdfChipAllocDirect(0x24);
    work->startDistance = 0;
    work->length = 375.0f;
    work->fragmentWork = effThunderFragCreate((EffThunderFragmentParams *)effCyanBlueRayPairParams);
    return work;
}

EffPCPBurstWork *effPcpRecreateCyanBlueFragmentPair(void) {
    return effPcpCreateCyanBlueFragmentPair(0);
}

EffPCPBurstWork *effPcpCreateCyanBlueFragmentSingle(u32 unused) {
    EffPCPBurstWork *work;

    work = sdfChipAllocDirect(0x24);
    work->startDistance = 325.0f;
    work->length = 225.0f;
    work->fragmentWork = effThunderFragCreate((EffThunderFragmentParams *)effCyanBlueRaySingleParams);
    return work;
}

EffPCPBurstWork *effPcpRecreateCyanBlueFragmentSingle(void) {
    return effPcpCreateCyanBlueFragmentSingle(0);
}

EffPCPBurstWork *effPcpCreateWhiteYellowFragmentPair(u32 unused) {
    EffPCPBurstWork *work;

    work = sdfChipAllocDirect(0x24);
    work->startDistance = 0;
    work->length = 375.0f;
    work->fragmentWork = effThunderFragCreate((EffThunderFragmentParams *)effWhiteYellowRayPairParams);
    return work;
}

EffPCPBurstWork *effPcpRecreateWhiteYellowFragmentPair(void) {
    return effPcpCreateWhiteYellowFragmentPair(0);
}

EffPCPBurstWork *effPcpCreateWhiteYellowFragmentSingle(u32 unused) {
    EffPCPBurstWork *work;

    work = sdfChipAllocDirect(0x24);
    work->startDistance = 325.0f;
    work->length = 225.0f;
    work->fragmentWork = effThunderFragCreate((EffThunderFragmentParams *)effWhiteYellowRaySingleParams);
    return work;
}

EffPCPBurstWork *effPcpRecreateWhiteYellowFragmentSingle(void) {
    return effPcpCreateWhiteYellowFragmentSingle(0);
}

EffPCPBurstWork *effPcpCreateYellowOrangeFragmentPair(u32 unused) {
    EffPCPBurstWork *work;

    work = sdfChipAllocDirect(0x24);
    work->startDistance = 0;
    work->length = 375.0f;
    work->fragmentWork = effThunderFragCreate((EffThunderFragmentParams *)effYellowOrangeRayPairParams);
    return work;
}

EffPCPBurstWork *effPcpRecreateYellowOrangeFragmentPair(void) {
    return effPcpCreateYellowOrangeFragmentPair(0);
}

EffPCPBurstWork *effPcpCreateYellowOrangeFragmentSingle(u32 unused) {
    EffPCPBurstWork *work;

    work = sdfChipAllocDirect(0x24);
    work->startDistance = 325.0f;
    work->length = 225.0f;
    work->fragmentWork = effThunderFragCreate((EffThunderFragmentParams *)effYellowOrangeRaySingleParams);
    return work;
}

EffPCPBurstWork *effPcpRecreateYellowOrangeFragmentSingle(void) {
    return effPcpCreateYellowOrangeFragmentSingle(0);
}

EffPCPBurstWork *effPcpCreateBrownRedFragmentPair(u32 unused) {
    EffPCPBurstWork *work;

    work = sdfChipAllocDirect(0x24);
    work->startDistance = 0;
    work->length = 375.0f;
    work->fragmentWork = effThunderFragCreate((EffThunderFragmentParams *)effBrownRedRayPairParams);
    return work;
}

EffPCPBurstWork *effPcpRecreateBrownRedFragmentPair(void) {
    return effPcpCreateBrownRedFragmentPair(0);
}

EffPCPBurstWork *effPcpCreateBrownRedFragmentSingle(u32 unused) {
    EffPCPBurstWork *work;

    work = sdfChipAllocDirect(0x24);
    work->startDistance = 325.0f;
    work->length = 225.0f;
    work->fragmentWork = effThunderFragCreate((EffThunderFragmentParams *)effBrownRedRaySingleParams);
    return work;
}

EffPCPBurstWork *effPcpRecreateBrownRedFragmentSingle(void) {
    return effPcpCreateBrownRedFragmentSingle(0);
}

EffPCPBurstWork *effPcpCreateOrangeVioletFragmentPair(u32 unused) {
    EffPCPBurstWork *work;

    work = sdfChipAllocDirect(0x24);
    work->startDistance = 0;
    work->length = 375.0f;
    work->fragmentWork = effThunderFragCreate((EffThunderFragmentParams *)effOrangeVioletRayPairParams);
    return work;
}

EffPCPBurstWork *effPcpRecreateOrangeVioletFragmentPair(void) {
    return effPcpCreateOrangeVioletFragmentPair(0);
}

EffPCPBurstWork *effPcpCreateOrangeVioletFragmentSingle(u32 unused) {
    EffPCPBurstWork *work;

    work = sdfChipAllocDirect(0x24);
    work->startDistance = 325.0f;
    work->length = 225.0f;
    work->fragmentWork = effThunderFragCreate((EffThunderFragmentParams *)effOrangeVioletRaySingleParams);
    return work;
}

EffPCPBurstWork *effPcpRecreateOrangeVioletFragmentSingle(void) {
    return effPcpCreateOrangeVioletFragmentSingle(0);
}

void effPcpThunderBurstRelease(EffPCPBurstWork *work) {
    effThunderReleaseFragmentWork(work->fragmentWork);
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
    u8 *params;

    if (effBTLFieldColorUsesActorSelectors() != 0) {
        btlUnitGetMuzzlePosVU((BtlUnit *)effBTLFieldColorGetVariantSelector());
        VU0_LOAD_VF(vf11, work->position);
        VU0_SUB(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF(vf10, direction);
        params = (u8 *)effThunderGetFragmentParameters(work->fragmentWork);
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
        PCP_COPY_VECTOR(params, point);
        distance = work->length;
        point[0] = x + dirX * distance;
        point[1] = y + dirY * distance;
        point[2] = z + dirZ * distance;
        PCP_COPY_VECTOR(params + 0x10, point);
        effThunderSetFragmentColor(work->fragmentWork, work->tintColor);
        effThunderUpdateFragments(work->fragmentWork);
    }
}

void effPcpCopyThunderBurstVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetThunderBurstColor(EffPCPBurstWork *work, u32 value) {
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

EffPCPGrowWork *effPcpCreateScaledThunder(EffPCPScaledThunderParams *params) {
    EffPCPGrowWork *work = sdfChipAllocDirect(sizeof(EffPCPGrowWork));
    EffThunderVectorWork *handle;

    params->res.radiusScale = (f32)params->base;
    params->res.heightScale = (f32)params->base;
    handle = effCreateThunderCellSystemWork(&params->res);
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

EffPCPGrowWork *effPcpScaledEffectCreateFromTable(void *data) {
    void *work;

    work = effParamTableGetBlock(data, 0);
    return effPcpCreateScaledThunder(work);
}


EffPCPGrowWork *effPcpCreateScaledEffectFromResource(EffPCPGrowWork *work) {
    EffPCPScaledThunderParams params;
    EffThunderParameterHead *res;

    res = effThunderGetParameterHead(work->handle);
    params.duration = work->duration;
    params.fadeIn = work->fadeIn;
    params.fadeOut = work->fadeOut;
    params.base = work->base;
    params.target = work->target;
    params.res = res->alpha;
    return effPcpCreateScaledThunder(&params);
}

void effPcpScaledEffectRelease(EffPCPGrowWork *work) {
    effThunderReleaseVectorWork(work->handle);
    sdfChipFreeDirect(work);
}

/* Preserve the resource query before applying its growth and fade timeline. */
void effPcpStepScaledThunderTimeline(EffPCPGrowWork *work) {
    s32 frame;
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    f32 base;
    f32 extent;
    f32 scale;
    f32 opacity;
    u32 color;

    effThunderGetParameterHead(work->handle);
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
    effThunderSetVectorTint(work->handle, effBlendColor(color & 0xFFFFFF, color, opacity));
    effThunderCopyVectorOrigin(work->handle, work->position);
    effThunderScaleVectorDimensions(work->scale * scale, work->handle);
    effThunderUpdateVectorCells(work->handle);
    work->frame = (s32)((u32)work->frame + 1);
}

void effPcpCopyScaledThunderVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetScaledThunderColor(EffPCPGrowWork *work, u32 value) {
    work->color = value;
}

void effPcpSetScaledThunderScale(EffPCPGrowWork *work, f32 val) {
    work->scale = val;
}

/* Shared draw-request parameters are cleared before creating a beam. */
extern EffPacketParams effBeamDrawParams;

/* vu0 routine: initialize both transforms with the libvu0 identity primitive. */
EffPCPBeamNode *effPcpBeamNodeCreate(u32 segments) {
    s32 count = segments * 4 + 4;
    EffPCPBeamNode *node = sdfChipAllocDirect(sizeof(EffPCPBeamNode));
    SdfMemBlock *allocation = sdfMemAllocDirect(count * 20);
    f32 *points = (void *)sdfMemLock(allocation);

    memset(points, 0, count * 20);
    node->points = points;
    node->drawKind = 2;
    node->colors = (u32 *)(points + count * 4);
    node->vertexCount = count;
    node->color = 0x80808080;
    node->allocationHandle = allocation;
    node->scale = 1.0f;
    node->assetHandle = sdfCreateAssetWithDrawEntries();
    sdfSetPrimaryStateFloat((struct SdfAsset *)node->assetHandle, 1.0f);
    EE_MMI_UNIT_MATRIX(node->localMatrix);
    EE_MMI_UNIT_MATRIX(node->matrix);
    memset(&effBeamDrawParams, 0, sizeof(effBeamDrawParams));
    effBeamDrawParams.primitive = 0x4000;
    return node;
}

void effPcpReleaseNestedWork(EffPCPBeamNode *work) {
    sdfQueueAssetRelease(work->assetHandle);
    sdfMemFreeDirect(work->allocationHandle);
    sdfChipFreeDirect(work);
}


extern u8 effBeamTriangleIndices[];
extern SdfPoolNode *effBeamDrawSurfaces[];
#include "sdf_packet_alloc.h"
extern void *effCreateVertexDrawPacket(EffPacketParams *params);

/* vu0 routine: compose the beam transform and submit batches of its vertices. */
void effPcpDrawBeamGeometryNode(EffPCPBeamNode *node) {
    f32 scaleMatrix[4][4];
    SdfListHead *list;
    s32 count;
    SdfPoolNode *entry;

    if (node->vertexCount == 0) {
        return;
    }
    list = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    EE_MMI_UNIT_MATRIX(scaleMatrix);
    scaleMatrix[2][2] = scaleMatrix[1][1] = scaleMatrix[0][0] = node->scale;
    VU0_LOAD_MATRIX(node->localMatrix);
    VU0_LOAD_MATRIX_B(node->matrix);
    sdfComposeVuMatrixFromRegisters();
    VU0_LOAD_VF(vf10, &node->position);
    VU0_ADD_XYZ(vf31, vf31, vf10);
    VU0_LOAD_MATRIX_B(scaleMatrix);
    sdfComposeVuMatrixFromRegisters();
    sdfConsAppendVuPacket(list, 0);
    sdfConsAppendAssetPacket(list, node->assetHandle, 0);
    count = node->vertexCount;
    effBeamDrawParams.colors = node->colors;
    effBeamDrawParams.positions = (u128 *)node->points;
    effBeamDrawParams.color = node->color;
    effBeamDrawParams.vertexCount = effBeamDrawParams.parameterCount = 12;
    effBeamDrawParams.parameters = (u32 *)effBeamTriangleIndices;
    while (count >= 12) {
        count -= 8;
        sdfAppendPacket(list, (u32)effCreateVertexDrawPacket(&effBeamDrawParams));
        effBeamDrawParams.positions += 8;
        effBeamDrawParams.colors += 8;
    }
    if (count >= 8) {
        effBeamDrawParams.parameterCount = 6;
        effBeamDrawParams.vertexCount = 8;
        sdfAppendPacket(list, (u32)effCreateVertexDrawPacket(&effBeamDrawParams));
    }
    entry = effBeamDrawSurfaces[node->drawKind];
    entry->append(entry, list);
}

void effPcpBuildConcentricBeamVertices(f32 radius, EffPCPBeamWork *work) {
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
        direction[2] = sdfSinPoly(angle);
        VU0_LOAD_VF(vf11, direction);
        VU0_LOAD_VF(vf10, inner);
        VU0_MUL(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, out);
        VU0_LOAD_VF(vf10, first);
        VU0_MUL(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, out + 4);
        VU0_LOAD_VF(vf10, middle);
        VU0_MUL(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, out + 8);
        VU0_LOAD_VF(vf10, outer);
        VU0_MUL(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, out + 12);
        angle += step;
    }
}

void effResetChild(EffPCPBeamWork *work) {
    effPcpBuildConcentricBeamVertices(work->params.startRadius, work);
    work->frame = 0;
}

u8 *effBeamEffectClone(src)
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
    node = effPcpBeamNodeCreate(segments);
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
    effResetChild(work);
    work->node->drawKind = src->drawKind;
    return (u8 *)work;
}

u8 *effPcpBeamCreateFromTable(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    return effBeamEffectClone(param0);
}

u8 *effPcpRecreateBeam(EffPCPBeamParams *src) {
    return effBeamEffectClone(src);
}

void effPcpReleaseBeamClone(EffPCPBeamWork *work) {
    effPcpReleaseNestedWork(work->node);
    sdfChipFreeDirect(work);
}

/* Grow the beam's visible geometry and fade it over its configured lifetime. */
void effPcpUpdateBeamGrowthAndFade(EffPCPBeamWork *work) {
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
        effPcpBuildConcentricBeamVertices(
            (work->params.endRadius - work->params.startRadius) *
                t + work->params.startRadius,
            work);
    } else {
        effPcpBuildConcentricBeamVertices(work->params.endRadius, work);
    }
    if (frame < fadeIn && fadeIn != 0) {
        t = (f32)frame / (f32)fadeIn;
    } else if (endFrame - frame <= fadeOut && fadeOut != 0) {
        t = (f32)(endFrame - frame) / (f32)fadeOut;
    } else {
        t = 1.0f;
    }
    color = effBlendColor(work->color & 0xFFFFFF, work->color, t);
    beam = work->node;
    beam->color = color;
    PCP_COPY_VECTOR(&beam->position, work->params.position);
    effPcpDrawBeamGeometryNode(beam);
    work->frame++;
}


void effPcpCopyBeamVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpNestedWorkSetFloat(EffPCPBeamWork *work, f32 value) {
    work->node->scale = value;
}

void effPcpSetBeamColor(EffPCPBeamWork *work, u32 value) {
    work->color = value;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effPcpCopyNestedMatrix(EffPCPBeamWork *work, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_STORE_MATRIX(work->node->localMatrix);
}

void effRotateNested(EffPCPBeamWork *work, void *src) {
    VU0_LOAD_MATRIX(src);
    sdfVuLoadSecondaryRotationMatrixX(1.5707963f);
    sdfComposeVuMatrixFromRegisters();
    VU0_STORE_MATRIX(work->node);
}



/* Fill the node's point buffer with four concentric rings (radius, +stepA, +stepB, +stepC) of unit directions, rotated by the VU matrix. */
void effPcpBuildConcentricRingPoints(EffPCPBeamLargeWork *work, f32 radius) {
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
        sdfVuBuildLookAtBasis(sdfViewEyeVector, sdfViewTargetVector, sdfViewUpVector);
        sdfInvertRigidVuTransform();
    } else {
        sdfVuLoadRotationMatrixZ(work->rotationA);
        sdfVuLoadSecondaryRotationMatrixY(work->rotationB);
        sdfMultiplyVuMatrixInPlace();
        work->rotationB += work->rotationStep;
        work->rotationStep *= work->head.rotationDecay;
    }
    for (i = 0; i < n; i++) {
        if (work->head.mode == 0) {
            dir[0] = sdfCos(angle);
            dir[1] = sdfSinPoly(angle);
            dir[2] = 0;
        } else {
            dir[0] = sdfCos(angle);
            dir[1] = 0;
            dir[2] = sdfSinPoly(angle);
        }
        dir[3] = 0;
        VU0_LOAD_VF(vf10, dir);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_MOVE_VF(vf11, vf10);
        VU0_LOAD_VF(vf10, rowA);
        VU0_MUL(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, points);
        VU0_LOAD_VF(vf10, rowB);
        VU0_MUL(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, points + 4);
        VU0_LOAD_VF(vf10, rowC);
        VU0_MUL(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, points + 8);
        VU0_LOAD_VF(vf10, rowD);
        VU0_MUL(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, points + 12);
        angle += step;
        points += 16;
    }
}

void effPrepareAngles(EffPCPBeamLargeWork *work) {
    work->rotationA = work->head.degreesA * 0.017453291f;
    work->rotationB = work->head.degreesB * 0.017453291f;
    work->rotationStep = work->head.stepDegrees * 0.017453291f;
    work->radius = work->head.initialRadius;
    work->radiusStep = work->head.initialRadiusStep;
    effPcpBuildConcentricRingPoints(work, work->head.initialRadius);
    work->frame = 0;
}

/* The ring geometry, angle preparation and timeline use the same 0x80-byte
   clone, with separate radius and rotation decay inputs. */
u8 *effBeamEffectCloneLarge(src)
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
    node = effPcpBeamNodeCreate(segments);
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
    effPrepareAngles(work);
    work->node->drawKind = src->drawKind;
    return (u8 *)work;
}

u8 *effPcpBeamLargeCreateFromTable(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    return effBeamEffectCloneLarge(param0);
}

u8 *effPcpRecreateLinkedBeam(EffPCPBeamLargeHead *src) {
    return effBeamEffectCloneLarge(src);
}

void effPcpReleaseLinkedBeamClone(EffPCPBeamLargeWork *work) {
    effPcpReleaseNestedWork(work->node);
    sdfChipFreeDirect(work);
}



/* Advance the ring radius, fade the color and mirror the position/color
   into the clone's beam node. */
void effPcpUpdateBeamTimeline(EffPCPBeamLargeWork *work) {
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
    effPcpBuildConcentricRingPoints(work, work->radius);
    work->radiusStep *= work->head.radiusDecay;
    t = 1.0f;
    if (frame < fadeIn && fadeIn != 0) {
        t = (f32)frame / (f32)fadeIn;
    } else if (duration - frame <= fadeOut && fadeOut != 0) {
        t = (f32)(duration - frame) / (f32)fadeOut;
    }
    color = effBlendColor(work->color & 0xFFFFFF, work->color, t);
    beam = work->node;
    beam->color = color;
    PCP_COPY_VECTOR(&beam->position, work);
    effPcpDrawBeamGeometryNode(beam);
    work->frame++;
}

void effPcpCopyLinkedBeamVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpLinkedWorkSetFloat(EffPCPBeamLargeWork *work, f32 value) {
    work->node->scale = value;
}

void effPcpSetLinkedBeamColor(EffPCPBeamLargeWork *work, u32 value) {
    work->color = value;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effPcpCopyLinkedBeamMatrix(EffPCPBeamLargeWork *work, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_STORE_MATRIX(work->node->localMatrix);
}

EffPCPGroupSet *effPcpGroupSetCreate(EffPCPGroupHead *header, void **sourceResources) {
    u32 entryCount = header->count;
    struct SdfMemBlock *workResource = sdfMemAllocDirect(entryCount * 0x18 + 0x180);
    EffPCPGroupSet *groupSet = (void *)sdfMemLock(workResource);
    EffPCPGroupEntry *groupEntryCursor;
    u32 index;
    u32 duplicateIndex;

    groupSet->head = *header;
    groupSet->scale = 1.0f;
    groupSet->color = 0x80808080;
    groupSet->workHandle = workResource;
    groupSet->fragmentLength = header->spawnParams.start[1];
    groupEntryCursor = (EffPCPGroupEntry *)(groupSet + 1);
    groupSet->entries = groupEntryCursor;
    groupSet->duplicates = 0;
    if (sourceResources != NULL) {
        u32 allocationBytes = entryCount * 16;
        void **sourceResourceCursor = sourceResources;
        u32 groupByteOffset;
        u32 groupByteWidth;
        u8 *activeGroupFlags;

        index = 0;
        groupSet->duplicateHandle = sdfMemAllocDirect(allocationBytes);
        activeGroupFlags = header->activeGroups;
        groupByteOffset = 0;
        groupByteWidth = entryCount * 4;
        groupSet->duplicates = (EffParamWork **)sdfMemLock(groupSet->duplicateHandle);
        memset(groupSet->duplicates, 0, allocationBytes);
        for (; index < 4; index++) {
            EffParamWork **destinationHandles = (EffParamWork **)((u8 *)groupSet->duplicates + groupByteOffset);

            if (*activeGroupFlags != 0) {
                EffParamWork *firstCreatedHandle;

                if (index < 2) {
                    *destinationHandles = effParamWorkCreate(EFF_PARAM_WORK_KIND_EFFECT_NODE, *sourceResourceCursor);
                } else {
                    *destinationHandles = effParamWorkCreate(EFF_PARAM_WORK_KIND_EXTENDED_WORK_WITH_MATRIX_CALLBACK, *sourceResourceCursor);
                }
                firstCreatedHandle = *destinationHandles;
                for (duplicateIndex = 1; duplicateIndex < entryCount; duplicateIndex++) {
                    destinationHandles++;
                    *destinationHandles = effParamWorkDuplicate(firstCreatedHandle);
                }
            }
            sourceResourceCursor++;
            activeGroupFlags++;
            groupByteOffset += groupByteWidth;
        }
    }
    for (index = 0; index < entryCount; index++) {
        groupEntryCursor->handle = effThunderFragCreate(&header->spawnParams);
        groupEntryCursor->frame = 0;
        groupEntryCursor++;
    }
    return groupSet;
}

EffPCPGroupSet *effPcpGroupSetCreateFromTable(void *args) {
    EffPCPGroupHead *work;
    void *resources[4];
    u32 i;

    work = effParamTableGetBlock(args, 0);
    for (i = 0; i < 4; i++) {
        if (work->activeGroups[i] != 0) {
            resources[i] = effParamTableGetBlock(args, i + 1);
        } else {
            resources[i] = NULL;
        }
    }
    return effPcpGroupSetCreate(work, resources);
}

EffPCPGroupSet *effPcpGroupSetCloneWithDuplicates(EffPCPGroupSet *work) {
    EffPCPGroupSet *groupSet = effPcpGroupSetCreate(&work->head, 0);
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
        groupSet->duplicates = (EffParamWork **)sdfMemLock(groupSet->duplicateHandle);
        memset(groupSet->duplicates, 0, allocationBytes);
        for (; groupIndex < 4; groupIndex++) {
            EffParamWork **destinationHandles = (EffParamWork **)((u8 *)groupSet->duplicates + groupByteOffset);

            if (*activeGroupFlags != 0) {
                EffParamWork *sourceHandle = *(EffParamWork **)(groupByteOffset + (u32)work->duplicates);
                for (entryIndex = 0; entryIndex < entryCount; entryIndex++) {
                    *destinationHandles++ = effParamWorkDuplicate(sourceHandle);
                }
            }
            activeGroupFlags++;
            groupByteOffset += groupByteWidth;
        }
    }
    return groupSet;
}

void effPcpGroupSetRelease(EffPCPGroupSet *work) {
    u32 releaseIndex = 0;
    u32 releaseCount = work->head.count;
    EffPCPGroupEntry *fragmentEntryCursor = work->entries;
    EffParamWork **duplicateHandleBase;
    EffParamWork **duplicate;

    if (releaseCount != 0) {
        do {
            EffThunderFragmentWork *currentHandle = fragmentEntryCursor->handle;
            fragmentEntryCursor++;
            releaseIndex++;
            effThunderReleaseFragmentWork(currentHandle);
        } while (releaseIndex < releaseCount);
    }
    duplicateHandleBase = work->duplicates;
    releaseCount = releaseCount * 4;
    if (duplicateHandleBase != NULL) {
        duplicate = duplicateHandleBase;
        releaseIndex = 0;
        if (releaseCount != 0) {
            do {
                EffParamWork *currentHandle = *duplicate++;
                if (currentHandle != 0) {
                    effDispatchParameterDataAndFreeWork(currentHandle);
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
void effPcpGroupSetInitEntry(EffPCPGroupSet *work, s32 index) {
    EffPCPGroupEntry *entry = work->entries + index;
    s32 spread = work->head.delaySpread;
    f32 scale = work->scale;
    f32 ratio;

    if (spread > 0) {
        entry->frame = -(sdfGetRand((EffRandState *)effDefaultRandomState) % spread);
    }
    ratio = work->head.startJitter;
    entry->position = work->head.startPosition * (sdfGetRandFloat(effDefaultRandomState) * ratio + (1.0f - ratio)) * scale;
    ratio = work->head.endJitter;
    entry->positionStep = (work->head.endPosition * (sdfGetRandFloat(effDefaultRandomState) * ratio + (1.0f - ratio)) * scale - entry->position) / (f32)work->head.framesPerEntry;
    entry->angle = 3.14159265f * 2.0f / (f32)work->head.count * (f32)index;
    ratio = work->head.angularStepJitter;
    entry->angularStep = work->head.angularStep * (sdfGetRandFloat(effDefaultRandomState) * ratio + (1.0f - ratio));
}

void effPcpUpdateRadialFragmentGroupSet(EffPCPGroupSet *work) {
    f32 point[3];
    f32 origin[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    EffParamWork **followers = NULL;
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
    PCP_COPY_VECTOR(origin, work->head.origin);
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
                effPcpGroupSetInitEntry(work, i);
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
                sine = sdfSinPoly(angle);
                point[0] = origin[0] + cosine * radius;
                point[1] = origin[1];
                point[2] = origin[2] + sine * radius;
                entry->position += entry->positionStep;
                entry->angle += entry->angularStep;
                entry->angularStep *= angularDamping;
                fragment = effThunderGetFragmentParameters(entry->handle);
                blend = startRadialBlend + (endRadialBlend - startRadialBlend) * ((f32)frame / (f32)duration);
                direction[0] = cosine * blend;
                direction[1] = 1.0f - ffabsf(blend);
                direction[2] = sine * blend;
                VU0_LOAD_VF(vf10, direction);
                VU0_NORMALIZE_VF10();
                VU0_STORE_VF(vf10, direction);
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
                fadedColor = effBlendColor(color & 0xFFFFFF, color, fade);
                effThunderSetFragmentColor(entry->handle, fadedColor);
                effThunderUpdateFragments(entry->handle);
                if (work->duplicates != NULL) {
                    u32 followerIndex;
                    for (followerIndex = 0; followerIndex < ARRAY_COUNT(work->head.activeGroups); followerIndex++) {
                        if (*followers != 0) {
                            effParamWorkCallback0(*followers, fragment->end);
                            effParamWorkCallback3(*followers, fadedColor);
                            effParamWorkInvokeCallback(*followers);
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

void effPcpCopyGroupSetVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetGroupScale(EffPCPGroupSet *work, f32 val) {
    work->scale = val;
}

void effPcpSetGroupColor(EffPCPGroupSet *work, u32 value) {
    work->color = value;
}

EffPCPSprayWork *effSprayEffectCreateFromTable(void *src) {
    EffPCPSprayWork *work = sdfChipAllocDirect(0x98);
    u32 i;

    work->scale = 1.0f;
    work->color = 0x80808080;
    work->count = 10;
    work->frame = 0;
    for (i = 0; i < work->count; i++) {
        work->handle[i] = 0;
        work->startFrame[i] = i;
        work->rotationY[i] = (sdfGetRandFloat(effDefaultRandomState) - 0.5f) * 2.0f * 0.87266457f + 3.14159265f;
    }
    work->handle[0] = effParamCreateFromTable(src, 0);
    return work;
}

EffPCPSprayWork *effSprayEffectClone(EffPCPSprayWork *src) {
    EffPCPSprayWork *work = sdfChipAllocDirect(0x98);
    u32 i;

    work->frame = 0;
    work->scale = src->scale;
    work->count = src->count;
    work->color = 0x80808080;
    for (i = 0; i < work->count; i++) {
        work->handle[i] = 0;
        work->startFrame[i] = i;
        work->rotationY[i] = (sdfGetRandFloat(effDefaultRandomState) - 0.5f) * 2.0f * 0.87266457f + 3.14159265f;
    }
    work->handle[0] = effParamWorkDuplicate(src->handle[0]);
    return work;
}

void effDestroyIndexedResources(EffPCPSprayWork *work) {
    u32 i;

    for (i = 0; i < work->count; i++) {
        if (work->handle[i] != 0) {
            effDispatchParameterDataAndFreeWork(work->handle[i]);
        }
    }
    sdfChipFreeDirect(work);
}

void effPcpCaptureNodeVectors(SdfDrawNode *node) {
    SdfDrawNode *child;

    VU0_STORE_VF(vf10, node->scale);
    child = node->children;
    if (child != NULL) {
        do {
            effPcpCaptureNodeVectors(child);
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
    MdlCtx *data;
    SdfModel *model;
    Motion *child;
    SdfDrawNode *root;
    f32 vec[4];
    s32 j;

    effBTLFieldColorGetVariantSelector();
    count = work->count;
    for (; i < count; i++) {
        if (work->startFrame[i] == work->frame && work->handle[i] == 0) {
            work->handle[i] = effParamWorkDuplicate(work->handle[0]);
        }
        if (work->handle[i] == 0) {
            continue;
        }
        data = effParamWorkGetData(work->handle[i]);
        sdfConvertEulerAnglesToQuaternionVU(0.0f, work->rotationY[i], 0.0f);
        if (effBTLFieldColorUsesActorSelectors() && (((BtlUnit *)effBTLFieldColorGetVariantSelector())->status.flags & 0x400)) {
            VU0_LOAD_VF(vf11, D_00414610);
            effMiscQuatMultiplyVU();
        }
        mdlUpdateContextRotationBasisFromQuaternion(data);
        vec[3] = 0;
        VEC3_SPLAT(vec, work->scale * work->scale);
        VU0_LOAD_VF(vf10, vec);
        mdlStoreTertiaryVectorVU(data);
        mdlBroadcastMasked(data, work->color);
        VU0_LOAD_VF(vf10, work->pos);
        mdlStorePrimaryVectorVU(data);
        for (j = 0; j != 4; j++) {
            child = data->slots[j];
            if (child != NULL && child->state) {
                sdfMotionUpdate(child);
            }
        }
        if (!(data->flags & MDL_SKIP_TRANSFORMS)) {
            model = data->inner;
            root = ((SdfDrawNode **)model->list->buffer)[0];
            VEC3_SPLAT(vec, 1.0f / work->scale);
            VU0_LOAD_VF(vf10, vec);
            effPcpCaptureNodeVectors(root);
            sdfModelUpdateCurrentFrameTransforms(model);
            sdfBuildModelPacketsVia(sdfModelBuildNodeResourcePackets, D_00380828, model);
        }
    }
    work->frame++;
}

void effPcpCopyCaptureNodeVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetSprayColor(EffPCPSprayWork *work, u32 value) {
    work->color = value;
}

void effPcpSetSprayScale(EffPCPSprayWork *work, f32 val) {
    work->scale = val;
}

/* Placement block handed to every spawned event entry. */
/* Serialized drift ranges followed by the retained event array. */
typedef struct EffPCPDriftEventParams {
    f32 position[4];
    u8 pad10[0x44];
    s32 duration;
    u32 count;
    s32 delaySpread;
    s32 fadeIn;
    s32 fadeOut;
    f32 startPosition, endPosition, startJitter, endJitter;
    f32 angleStep;
    f32 angleDecay;
    f32 angleJitter;
    u8 pad84[0x80];
} EffPCPDriftEventParams;

typedef struct EffPCPDriftEventWork {
    EffPCPDriftEventParams params;
    EffPCPDriftEventRow *entries;
    SoundMixer *owner;
    u8 unk10C;
    u8 pad10D[3];
    f32 scale;
    u32 color;
    struct SdfMemBlock *handle;
} EffPCPDriftEventWork;

/* Clone the source header (a longer one with 0x18-byte entries), then give every entry one event and a random negative start delay. */
EffPCPDriftEventWork *effPcpEntryWorkBCreate(EffPCPDriftEventParams *src, void *params) {
    u32 count = src->count;
    struct SdfMemBlock *handle = sdfMemAllocDirect(count * 24 + 0x11C);
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
    work->owner = effEventCloneSoundMixer(params);
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
        entry->event = effEventCreate(work->owner, 2, &place);
        if (life > 0) {
            entry->frame = -(sdfGetRand((EffRandState *)effDefaultRandomState) % life);
        } else {
            entry->frame = 0;
        }
        entry++;
    }
    return work;
}

EffPCPDriftEventWork *effPcpCreateDriftEventRingFromTable(void *data) {
    void *work0;
    void *work1;

    work0 = effParamTableGetBlock(data, 0);
    work1 = effParamTableGetBlock(data, 1);
    return effPcpEntryWorkBCreate(work0, work1);
}

EffPCPDriftEventWork *effPcpCloneDriftEventWork(EffPCPDriftEventWork *src) {
    u32 count = src->params.count;
    struct SdfMemBlock *handle = sdfMemAllocDirect(count * 24 + 0x11C);
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
        entry->event = effEventCreate(src->owner, 2, &place);
        if (life > 0) {
            entry->frame = -(sdfGetRand((EffRandState *)effDefaultRandomState) % life);
        } else {
            entry->frame = 0;
        }
        entry++;
    }
    return work;
}

void effPcpEventGroupRelease(EffPCPDriftEventWork *work) {
    u32 i = 0;
    u32 count = work->params.count;
    EffPCPDriftEventRow *entry = work->entries;

    if (count != 0) {
        do {
            effEventReleaseNode(entry->event);
            entry++;
            i++;
        } while (i < count);
    }
    if (work->owner->active == 0) {
        effEventReleaseSoundMixerVoices(work->owner);
    }
    sdfMemFreeDirect(work->handle);
}


void effPcpDriftRandomizeSlot(EffPCPDriftEventWork *work, s32 index) {
    EffPCPDriftEventRow *slot;
    f32 spread;
    f32 scale;

    slot = &work->entries[index];
    scale = work->scale;
    spread = work->params.startJitter;
    slot->position = work->params.startPosition * (sdfGetRandFloat(effDefaultRandomState) * spread + (1.0f - spread)) * scale;
    spread = work->params.endJitter;
    slot->positionStep = (work->params.endPosition * (sdfGetRandFloat(effDefaultRandomState) * spread + (1.0f - spread)) * scale - slot->position) / (f32)work->params.duration;
    slot->angle = 6.2831852f / (f32)work->params.count * (f32)index;
    spread = work->params.angleJitter;
    slot->angleStep = work->params.angleStep * (sdfGetRandFloat(effDefaultRandomState) * spread + (1.0f - spread));
}


/* vu0 routine: place delayed drift events along expanding rotating rings. */
void effPcpUpdateDriftEventRing(EffPCPDriftEventWork *work) {
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

    PCP_COPY_VECTOR(base, work->params.position);
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
                effPcpDriftRandomizeSlot(work, i);
                frame = entry->frame;
            }
            if (frame >= 0) {
                f32 angle = entry->angle;
                f32 radius = entry->position;
                f32 cosine;
                f32 sine;
                f32 t;

                cosine = sdfCos(angle);
                sine = sdfSinPoly(angle);
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
                place.color = effBlendColor(color & 0xFFFFFF, color, t);
                effEventCopyFileRecordHeader(entry->event, &place);
                effEventUpdateEffectParameters(entry->event);
                frame = entry->frame;
            }
            entry->frame = frame + 1;
        }
    }
}

void effPcpCopyDriftVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpDriftSetScale(EffPCPDriftEventWork *work, f32 val) {
    work->scale = val;
}

void effPcpSetDriftEventColor(EffPCPDriftEventWork *work, u32 value) {
    work->color = value;
}

typedef struct EffPCPPairedEventParams {
    f32 origin[4];
    u8 repeat;
    u8 pad11[3];
    s32 duration;
    u32 count;
    s32 delaySpread;
    s32 fadeIn;
    s32 fadeOut;
    f32 baseRadius, radiusJitter, baseTiltStep, tiltJitter;
    EffThunderFragmentParams fragmentParams;
} EffPCPPairedEventParams;

typedef struct EffPCPPairedEventWork {
    EffPCPPairedEventParams params;
    EffPCPPairedEventRow *entries;
    SoundMixer *ownerA, *ownerB;
    u8 unk98;
    u8 pad99[3];
    u32 color;
    struct SdfMemBlock *handle;
} EffPCPPairedEventWork;

/* Clone the source effect header, then give every entry two events (placed at the unit scale) and a random negative start frame. */
EffPCPPairedEventWork *effPcpCreateDelayedDriftEntries(EffPCPPairedEventParams *src, void *paramsA, void *paramsB) {
    u32 count = src->count;
    struct SdfMemBlock *handle = sdfMemAllocDirect(count * 32 + 0xA4);
    EffPCPPairedEventWork *work = (void *)sdfMemLock(handle);
    EffPCPEventPlace place;
    EffPCPPairedEventRow *entry;
    s32 life;
    u32 i;

    work->params = *src;
    entry = (EffPCPPairedEventRow *)(work + 1);
    work->handle = handle;
    work->entries = entry;
    work->color = 0x80808080;
    if (work->params.delaySpread <= 0) {
        work->params.delaySpread = 1;
    }
    work->unk98 = 1;
    work->ownerA = effEventCloneSoundMixer(paramsA);
    work->ownerB = effEventCloneSoundMixer(paramsB);
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
        entry->fragment = effThunderFragCreate(&src->fragmentParams);
        entry->eventA = effEventCreate(work->ownerA, 2, &place);
        entry->eventB = effEventCreate(work->ownerB, 2, &place);
        entry->frame = -(sdfGetRand((EffRandState *)effDefaultRandomState) % life);
        entry++;
    }
    return work;
}

EffPCPPairedEventWork *effPcpDriftCreateFromTable(void *data) {
    void *work0;
    void *work1;
    void *work2;

    work0 = effParamTableGetBlock(data, 0);
    work1 = effParamTableGetBlock(data, 1);
    work2 = effParamTableGetBlock(data, 2);
    return effPcpCreateDelayedDriftEntries(work0, work1, work2);
}

EffPCPPairedEventWork *effPcpClonePairedDriftEvents(EffPCPPairedEventWork *src) {
    u32 count = src->params.count;
    struct SdfMemBlock *handle = sdfMemAllocDirect(count * 32 + 0xA4);
    EffPCPPairedEventWork *work = (void *)sdfMemLock(handle);
    EffPCPEventPlace place;
    EffPCPPairedEventRow *entry;
    s32 life;
    u32 i;

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
        entry->fragment = effThunderFragCreate(&src->params.fragmentParams);
        entry->eventA = effEventCreate(src->ownerA, 2, &place);
        entry->eventB = effEventCreate(src->ownerB, 2, &place);
        entry->frame = -(sdfGetRand((EffRandState *)effDefaultRandomState) % life);
        entry++;
    }
    return work;
}

void effPcpPairedEventGroupRelease(EffPCPPairedEventWork *work) {
    u32 i = 0;
    u32 count = work->params.count;
    EffPCPPairedEventRow *entry = work->entries;

    if (count != 0) {
        do {
            effEventReleaseNode(entry->eventA);
            effEventReleaseNode(entry->eventB);
            effThunderReleaseFragmentWork(entry->fragment);
            entry++;
            i++;
        } while (i < count);
    }
    if (work->ownerA->active == 0) {
        effEventReleaseSoundMixerVoices(work->ownerA);
    }
    if (work->ownerB->active == 0) {
        effEventReleaseSoundMixerVoices(work->ownerB);
    }
    sdfMemFreeDirect(work->handle);
}


/* Randomises slot `index`: phase spread evenly around a full turn, jittered
 * radius, and a tiltStep whose sign is picked at random. */
void effPcpRandomizeSlot(EffPCPPairedEventWork *work, s32 index) {
    EffPCPPairedEventRow *slot;
    f32 spread;

    slot = &work->entries[index];
    slot->phase = (3.14159265f * 2.0f) / work->params.count * index;
    spread = work->params.radiusJitter;
    slot->radius = work->params.baseRadius * (sdfGetRandFloat(effDefaultRandomState) * spread + (1.0f - spread));
    slot->tilt = 0;
    spread = work->params.tiltJitter;
    if (sdfGetRand((EffRandState *)effDefaultRandomState) & 1) {
        slot->tiltStep = work->params.baseTiltStep * (sdfGetRandFloat(effDefaultRandomState) * spread + (1.0f - spread));
    } else {
        slot->tiltStep = -work->params.baseTiltStep * (sdfGetRandFloat(effDefaultRandomState) * spread + (1.0f - spread));
    }
}

extern const f32 effPairedDriftReferenceAxis[4] __attribute__((aligned(16)));

/* vu0 routine: rotate a fragment around the view and place events at both ends. */
void effPcpUpdatePairedDriftEvents(EffPCPPairedEventWork *work) {
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
    PCP_COPY_VECTOR(origin, work->params.origin);
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
    VU0_LOAD_VF(vf10, sdfViewEyeVector);
    VU0_LOAD_VF(vf11, sdfViewTargetVector);
    VU0_SUB(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, viewAxis);
    for (i = 0; i < count; i++, entry++) {
        frame = entry->frame;
        if (frame <= duration) {
            if (frame == 0) {
                effPcpRandomizeSlot(work, i);
                frame = entry->frame;
            }
            if (frame >= 0) {
                s32 elapsedFrames = frame;
                EffThunderFragmentParams *fragment;
                f32 radius;
                f32 fade;
                u32 fadedColor;

                fragment = effThunderGetFragmentParameters(entry->fragment);
                sdfBuildVuRotationFromAxisAngle(entry->phase, (const struct RwV3d *)viewAxis);
                VU0_LOAD_VF(vf10, effPairedDriftReferenceAxis);
                VU0_LOAD_VF(vf11, viewAxis);
                VU0_CROSS_XYZ(vf10, vf10, vf11);
                VU0_NORMALIZE_VF10();
                VU0_ROTATE_VEC(vf10, vf10);
                VU0_STORE_VF(vf10, direction);
                VU0_LOAD_VF(vf11, effPairedDriftReferenceAxis);
                VU0_CROSS_XYZ(vf10, vf10, vf11);
                VU0_NORMALIZE_VF10();
                /* This SDK-produced axis is consumed only by the external builder. */
                VU0_STORE_VF_UNCLOBBERED(vf10, tiltAxis);
                radius = entry->radius;
                sdfBuildVuRotationFromAxisAngle(entry->tilt, (const struct RwV3d *)tiltAxis);
                VU0_LOAD_VF(vf10, direction);
                VU0_ROTATE_VEC(vf10, vf10);
                VU0_STORE_VF(vf10, direction);
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
                fadedColor = effBlendColor(color & 0xFFFFFF, color, fade);
                effThunderSetFragmentColor(entry->fragment, fadedColor);
                effThunderUpdateFragments(entry->fragment);
                place.color = fadedColor;
                PCP_COPY_VECTOR(place.payload, fragment->end);
                effEventCopyFileRecordHeader(entry->eventA, &place);
                effEventUpdateEffectParameters(entry->eventA);
                PCP_COPY_VECTOR(place.payload, fragment->start);
                effEventCopyFileRecordHeader(entry->eventB, &place);
                effEventUpdateEffectParameters(entry->eventB);
                frame = entry->frame;
            }
            if (repeat != 0 && frame >= duration) {
                entry->frame = -(sdfGetRand((EffRandState *)effDefaultRandomState) % delaySpread);
            } else {
                entry->frame = frame + 1;
            }
        }
    }
}

void effPcpCopyRandomizedVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetPairedEventColor(EffPCPPairedEventWork *work, u32 value) {
    work->color = value;
}

/* Clone the source header, then give every entry one event (placed at the unit scale) and a random negative start frame. */
void *effPcpCreateDelayedEventEntries(EffPCPSpawnRangeParams *src, void *params) {
    u32 count = src->count;
    struct SdfMemBlock *handle = sdfMemAllocDirect(count * 32 + 0xAC);
    EffPCPSpawnRangeWork *work = (void *)sdfMemLock(handle);
    EffPCPEventPlace place;
    EffPCPSpawnRangeEventRow *entry;
    s32 life;
    u32 i;

    work->params = *src;
    entry = (EffPCPSpawnRangeEventRow *)(work + 1);
    work->handle = handle;
    work->color = 0x80808080;
    work->unk9C = 1;
    work->entries = entry;
    work->scale = 1.0f;
    work->owner = effEventCloneSoundMixer(params);
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
        entry->event = effEventCreate(work->owner, 2, &place);
        if (life > 0) {
            entry->frame = -(sdfGetRand((EffRandState *)effDefaultRandomState) % life);
        } else {
            entry->frame = 0;
        }
        entry++;
    }
    return work;
}

void *effPcpSlotEffectCreateFromTable(void *data) {
    void *work0;
    void *work1;

    work0 = effParamTableGetBlock(data, 0);
    work1 = effParamTableGetBlock(data, 1);
    return effPcpCreateDelayedEventEntries(work0, work1);
}

EffPCPSpawnRangeWork *effPcpCloneSpawnRangeEvents(EffPCPSpawnRangeWork *src) {
    u32 count = src->params.count;
    struct SdfMemBlock *handle = sdfMemAllocDirect(count * 32 + 0xAC);
    EffPCPSpawnRangeWork *work = (void *)sdfMemLock(handle);
    EffPCPEventPlace place;
    EffPCPSpawnRangeEventRow *entry;
    s32 life;
    u32 i;

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
        entry->event = effEventCreate(work->owner, 2, &place);
        if (life > 0) {
            entry->frame = -(sdfGetRand((EffRandState *)effDefaultRandomState) % life);
        } else {
            entry->frame = 0;
        }
        entry++;
    }
    return work;
}


void effPcpEventBatchRelease(EffPCPSpawnRangeWork *work) {
    u32 i = 0;
    u32 count = work->params.count;
    EffPCPSpawnRangeEventRow *entry = work->entries;

    if (count != 0) {
        do {
            effEventReleaseNode(entry->event);
            entry++;
            i++;
        } while (i < count);
    }
    if (work->owner->active == 0) {
        effEventReleaseSoundMixerVoices(work->owner);
    }
    sdfMemFreeDirect(work->handle);
}


/* Randomises spawn slot `index` inside the ranges held by the work. */
void effPcpRandomizeSpawnSlot(EffPCPSpawnRangeWork *work, s32 index) {
    EffPCPSpawnRangeEventRow *slot;
    f32 spread;
    f32 scale;

    slot = &work->entries[index];
    scale = work->scale;
    slot->frame = 0;
    slot->height = -work->params.startHeightRange * sdfGetRandFloat(effDefaultRandomState);
    spread = work->params.heightJitter;
    slot->heightStep = work->params.initialHeightStep * (sdfGetRandFloat(effDefaultRandomState) * spread + (1.0f - spread));
    slot->angle = sdfGetRandFloat(effDefaultRandomState) * (3.14159265f * 2.0f);
    spread = work->params.angularJitter;
    slot->angularStep = work->params.initialAngularStep * (sdfGetRandFloat(effDefaultRandomState) * spread + (1.0f - spread));
    spread = work->params.startJitter;
    slot->position = work->params.startPosition * (sdfGetRandFloat(effDefaultRandomState) * spread + (1.0f - spread)) * scale;
    spread = work->params.endJitter;
    slot->positionStep = (work->params.endPosition * (sdfGetRandFloat(effDefaultRandomState) * spread + (1.0f - spread)) * scale - slot->position) / (f32)work->params.duration;
}



/* vu0 routine: move delayed radial events, apply damping, then fade and draw. */
void effPcpUpdateSpawnRangeEvents(EffPCPSpawnRangeWork *work) {
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

    PCP_COPY_VECTOR(origin, work->params.origin);
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
                effPcpRandomizeSpawnSlot(work, i);
                effEventSetScale(entry->event, work->scale);
                frame = entry->frame;
            }
            if (frame >= 0) {
                f32 angle = entry->angle;
                f32 radius = entry->position;
                f32 cosine;
                f32 sine;
                f32 fade;

                cosine = sdfCos(angle);
                sine = sdfSinPoly(angle);
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
                place.color = effBlendColor(color & 0xFFFFFF, color, fade);
                effEventCopyFileRecordHeader(entry->event, &place);
                effEventUpdateEffectParameters(entry->event);
                frame = entry->frame;
            }
            entry->frame = frame + 1;
        }
    }
}

void effPcpCopySpawnRangeVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSpawnRangeSetScale(EffPCPSpawnRangeWork *work, f32 val) {
    work->scale = val;
}

void effPcpSetSpawnRangeColor(EffPCPSpawnRangeWork *work, u32 value) {
    work->color = value;
}



void effPcpEventWorkInitEntries(EffPCPMapEventWork *work) {
    EffPCPEventPlace place;
    u32 i = 0;
    u32 count;
    MdlCtx *model;
    EffPCPMapEventEntry *entry;

    model = effParamWorkGetData(work->modelResource);
    model->first->frameStep = work->params.frameStep;
    mdlAddEntryPlain(model, 0, 0);
    work->frameLimit = model->first->frameCount;
    count = sdfCountMapPositionRecords(model->inner);
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
        entry->event = effEventCreate(work->owner, 2, &place);
        entry++;
    }
}



/* Allocate an event work: copy the parameter head, clear the links, then create the resource and owner from the optional parameters. */
EffPCPMapEventWork *effPcpEventWorkCreate(EffPCPEventParamHead *head, void *resourceParams, void *ownerParams) {
    EffPCPMapEventWork *work = sdfChipAllocDirect(0x40);

    work->params = *head;
    work->color = 0x80808080;
    work->scale = 1.0f;
    work->owner = 0;
    work->entries = 0;
    work->modelResource = 0;
    work->frame = 0;
    if (resourceParams != 0) {
        work->modelResource = effParamWorkCreate(EFF_PARAM_WORK_KIND_VIEWER_CONTEXT, resourceParams);
    }
    if (ownerParams != 0) {
        work->owner = effEventCloneSoundMixer(ownerParams);
    }
    if (work->owner != 0) {
        effPcpEventWorkInitEntries(work);
    }
    return work;
}

EffPCPMapEventWork *effPcpEventWorkCreateFromTable(void *data) {
    void *work0;
    void *work1;
    void *work2;

    work0 = effParamTableGetBlock(data, 0);
    work1 = effParamTableGetBlock(data, 1);
    work2 = effParamTableGetBlock(data, 2);
    return effPcpEventWorkCreate(work0, work1, work2);
}


EffPCPMapEventWork *effEventWorkClone(EffPCPMapEventWork *src) {
    EffPCPMapEventWork *work;

    work = effPcpEventWorkCreate(&src->params, 0, 0);
    work->modelResource = effParamWorkDuplicate(src->modelResource);
    work->owner = src->owner;
    effPcpEventWorkInitEntries(work);
    return work;
}


void effDestroyParticleEvents(EffPCPMapEventWork *work) {
    u32 i = 0;
    u32 count;
    EffPCPMapEventEntry *entry;

    effDispatchParameterDataAndFreeWork(work->modelResource);
    count = work->count;
    entry = work->entries;
    if (count != 0) {
        do {
            u32 event = (u32)entry->event;
            entry++;
            if (event != 0) {
                effEventReleaseNode((void *)event);
            }
            i++;
        } while (i < count);
    }
    if (work->owner->active == 0) {
        effEventReleaseSoundMixerVoices(work->owner);
    }
    sdfMemFreeDirect(work->entriesHandle);
    sdfChipFreeDirect(work);
}


/* vu0 routine: latch map-node motion, then place and fade each event. */
void effPcpUpdateMapMotionEvents(EffPCPMapEventWork *work) {
    EffPCPEventPlace place;
    f32 position[4] __attribute__((aligned(16)));
    f32 initialPosition[4] __attribute__((aligned(16)));
    f32 scale[4] __attribute__((aligned(16)));
    s32 frame = work->frame;
    s32 frameLimit = work->frameLimit;
    SdfModel *mapModel;
    s32 fadeIn;
    s32 fadeOut;
    u32 color;
    u32 count;
    u32 i;
    MdlCtx *model;
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
        VU0_LOAD_VF(vf10, work->params.position);
        mdlStorePrimaryVectorVU(model);
        VU0_LOAD_VF(vf10, scale);
        mdlStoreTertiaryVectorVU(model);
        mdlProcessContextNodesAndTransforms(model, D_00380828);
        count = work->count;
        fadeIn = work->params.fadeIn;
        fadeOut = work->params.fadeOut;
        color = work->color;
        entry = work->entries;
        for (i = 0; i < count; i++, entry++) {
            if (frame == 0) {
                effEventSetScale(entry->event, work->scale);
            }
            if (frame >= 0) {
                sdfLoadMapRecordPositionVector(mapModel, i);
                VU0_STORE_VF(vf10, position);
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
                        VU0_LOAD_VF(vf10, position);
                        VU0_LOAD_VF(vf11, initialPosition);
                        VU0_SUB(vf10, vf10, vf11);
                        VU0_LENGTH_VF10(distance);
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
                        place.color = effBlendColor(color & 0xFFFFFF, color, t);
                        place.payload[0] = position[0];
                        place.payload[1] = position[1];
                        place.payload[2] = position[2];
                        effEventCopyFileRecordHeader(entry->event, &place);
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

