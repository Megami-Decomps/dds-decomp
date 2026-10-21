#include "sdk_ps2.h"
struct SdfPoolNode;
struct EffNodeDescriptor;

#include "common.h"
#include "sdf_model_scalars.h"
#include "sdf_motion.h"
#include "sdf_chip.h"
#include "dds3_path.h"
#include "evt_unit.h"
#include "evt_unit_motion_setup.h"
#include "evt_scaled_value.h"
#include "evt_unit_motion_slot.h"
#include "evt_unit_color_transition.h"
#include "evt_unit_vertical_move.h"
#include "eff_transform.h"
#include "dds3obj.h"
#include "mdl.h"
#include "sdf_draw.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "sdf_vu_matrix.h"
#include "eff_quaternion_nlerp.h"


extern f32 *D_0037F770[];
extern u8 kwlnDefaultColorVector[];

extern void evtComputePlanarTargetDirectionVu(EvtUnit *unit);
s32 evtRotateUnitTowardTarget(EvtUnit *unit, f32 *dir, f32 angle);

typedef struct PcpScatterWork4 PcpScatterWork4;

/* func_001730D0 */
struct PcpScatterWork4 {
    u8 pad00[0x40];
    s128 unk40;
    u8 pad50[0x12C];
    f32 unk17C;
    u32 unk180;
    u32 scatterObject;
    u32 ownedBuffer;
};

/* Length of the path's vec4 trajectory sampled at 20 steps of the value multiplier. */
f32 evtMeasurePathTrajectoryLength(Dds3PathCurveWork *path) {
    f32 savedScaleFactor;
    f32 length = 0.0f;
    f32 step = 0.05f;
    f32 t = step;
    f32 segmentLength;

    savedScaleFactor = evtGetValueScaleFactor((EvtScaledValue *)path);
    evtScaleValueByMultiplier((EvtScaledValue *)path, 0.0f);
    dds3InterpolatePathVectorVU(path);
    do {
        VU0_MOVE_VF(vf11, vf10);
        evtScaleValueByMultiplier((EvtScaledValue *)path, t);
        dds3InterpolatePathVectorVU(path);
        VU0_MOVE_VF(vf12, vf10);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(segmentLength);
        length += segmentLength;
        VU0_MOVE_VF(vf10, vf12);
        t += step;
    } while (t <= 1.0f);
    evtScaleValueByMultiplier((EvtScaledValue *)path, savedScaleFactor);
    return length;
}

extern f32 D_00438A48, D_00438A4C;
#include "dds3_world_primary.h"
extern s32 dds3ContainsNodeInObjectChain(EffWorldNode *object, s32 slotIndex, EffWorldNode *value);
extern s32 sdfLoadMapRecordPositionVector(SdfModel *model, s32 mapRecordId);
extern void kwlnBuildShortestArcQuaternion(void);
extern s32 evtFindUnitSlotAuxCoordinates(EvtUnit *unit, f32 *outX, f32 *outY);


/* Advance the model, light-colour and directional transitions for one event unit. */
void evtAdvanceUnitVisualTransitions(EvtUnit *unit) {
    f32 identity[4];
    f32 targetDirection[4];
    f32 previousDirection[4];
    f32 position[4];
    f32 blendedDirection[4];
    f32 oldDirection[4];
    u32 rgbTarget[4], rgbSource[4], rgbResult[4];
    u32 alphaTarget[4], alphaSource[4], alphaResult[4];
    u32 firstSource[4], firstTarget[4], firstResult[4];
    u32 secondSource[4], secondTarget[4], secondResult[4];
    u32 defaultFirst[4], defaultSecond[4];
    u32 currentFirst[4], previousFirst[4], finalFirst[4];
    u32 currentSecond[4], previousSecond[4], finalSecond[4];
    f32 auxFirst, auxSecond;
    f32 factor;
    f32 transition;
    f32 currentWeight;
    f32 previousWeight;
    EvtTargetInfo *target;
    EvtTargetInfo *previous;
    s32 ownVector;
    s32 overrideAux = 0;
    u32 flags;
    u32 originalFlags;
    u32 oldColor;

    memset(identity, 0, sizeof(identity));
    identity[3] = 1.0f;
    auxFirst = D_00438A48;
    auxSecond = D_00438A4C;
    previousWeight = 0.0f;
    /* The model RGB and alpha tracks preserve the other packed channel. */
    flags = unit->flags;
    if (flags & EVT_UNIT_FLAG_RGB_TRANSITION) {
        u32 packed;
        factor = unit->rgbDuration ? (f32)unit->rgbElapsed / unit->rgbDuration : 1.0f;
        rgbTarget[0] = unit->color64;
        EE_MMI_RGBA_UNPACK_READONLY(rgbTarget, 0.0078125f);
        VU0_MOVE_VF(vf11, vf10);
        rgbSource[0] = unit->color60;
        EE_MMI_RGBA_UNPACK_READONLY(rgbSource, 0.0078125f);
        VU0_SCALE_VF(vf10, 1.0f - factor);
        VU0_SCALE_VF(vf11, factor);
        VU0_ADD(vf10, vf10, vf11);
        oldColor = mdlGetBroadcastValue((MdlCtx *)unit->owner);
        EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
        rgbResult[0] = packed;
        mdlBroadcastMasked((MdlCtx *)unit->owner, (oldColor & 0xFF000000) | (rgbResult[0] & 0xFFFFFF));
        if (unit->rgbElapsed >= unit->rgbDuration) {
            flags = unit->flags &= ~EVT_UNIT_FLAG_RGB_TRANSITION;
        } else {
            flags = unit->flags;
            unit->rgbElapsed++;
        }
    }
    if (flags & EVT_UNIT_FLAG_ALPHA_TRANSITION) {
        u32 packed;
        factor = unit->alphaDuration ? (f32)unit->alphaElapsed / unit->alphaDuration : 1.0f;
        alphaTarget[0] = unit->color64;
        EE_MMI_RGBA_UNPACK_READONLY(alphaTarget, 0.0078125f);
        VU0_MOVE_VF(vf11, vf10);
        alphaSource[0] = unit->color60;
        EE_MMI_RGBA_UNPACK_READONLY(alphaSource, 0.0078125f);
        VU0_SCALE_VF(vf10, 1.0f - factor);
        VU0_SCALE_VF(vf11, factor);
        VU0_ADD(vf10, vf10, vf11);
        oldColor = mdlGetBroadcastValue((MdlCtx *)unit->owner);
        EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
        alphaResult[0] = packed;
        mdlBroadcastMasked((MdlCtx *)unit->owner, (oldColor & 0xFFFFFF) | (alphaResult[0] & 0xFF000000));
        if (unit->alphaElapsed >= unit->alphaDuration) {
            flags = unit->flags &= ~EVT_UNIT_FLAG_ALPHA_TRANSITION;
        } else {
            flags = unit->flags;
            unit->alphaElapsed++;
        }
    }
    /* Stale world-node references must not reach the target-vector reads. */
    if (flags & EVT_UNIT_FLAG_TARGET_TRANSITION) {
        if (unit->currentTransitionValue == 0) {
            unit->flags = flags & ~EVT_UNIT_FLAG_TARGET_TRANSITION;
        } else if (!dds3ContainsNodeInObjectChain(dds3GetWorldObject(), 9,
                                                 (EffWorldNode *)unit->currentTransitionValue)) {
            unit->currentTransitionValue = 0;
            unit->previousTransitionValue = 0;
            unit->flags &= ~EVT_UNIT_FLAG_TARGET_TRANSITION;
        }
    }
    if (unit->previousTransitionValue &&
        !dds3ContainsNodeInObjectChain(dds3GetWorldObject(), 9,
                                       (EffWorldNode *)unit->previousTransitionValue)) {
        unit->previousTransitionValue = 0;
    }
    transition = 0.0f;
    target = 0;
    previous = 0;
    ownVector = 0;
    currentWeight = 0.0f;
    if (unit->currentTransitionValue) {
        flags = unit->flags;
        if (flags & EVT_UNIT_FLAG_TARGET_TRANSITION) {
            f32 distance;
            target = ((EffWorldNode *)unit->currentTransitionValue)->data;
            if (unit->previousTransitionValue) {
                previous = ((EffWorldNode *)unit->previousTransitionValue)->data;
            }
            if (!sdfLoadMapRecordPositionVector(unit->owner->inner, 0)) {
                mdlLoadPrimaryVectorVU(unit->owner);
            }
            VU0_STORE_VF_UNCLOBBERED(vf10, position);
            VU0_LOAD_VF(vf11, ((EffWorldNode *)unit->currentTransitionValue)->inner->position);
            VU0_SUB(vf10, vf10, vf11);
            VU0_STORE_VF_UNCLOBBERED(vf10, targetDirection);
            VU0_LENGTH_VF10(distance);
            if (distance <= target->nearDistance) {
                currentWeight = 1.0f;
            } else if (distance >= target->farDistance) {
                currentWeight = 0.0f;
            } else {
                currentWeight = 1.0f - (distance - target->nearDistance) /
                    (target->farDistance - target->nearDistance);
            }
            if (previous) {
                f32 previousDistance;
                VU0_LOAD_VF(vf10, position);
                VU0_LOAD_VF(vf11, ((EffWorldNode *)unit->previousTransitionValue)->inner->position);
                VU0_SUB(vf10, vf10, vf11);
                VU0_STORE_VF_UNCLOBBERED(vf10, previousDirection);
                VU0_LENGTH_VF10(previousDistance);
                if (previousDistance <= previous->nearDistance) {
                    previousWeight = 1.0f;
                } else if (previousDistance >= previous->farDistance) {
                    previousWeight = 0.0f;
                } else {
                    previousWeight = 1.0f - (previousDistance - previous->nearDistance) /
                        (previous->farDistance - previous->nearDistance);
                }
            }
            VU0_LOAD_VF(vf10, targetDirection);
            if (target->flags & EVT_TARGET_INFO_FLAG_UNIT_OWNS_VECTOR) {
                VU0_STORE_VF_UNCLOBBERED(vf10, target->direction);
                flags = unit->flags;
                ownVector = 1;
            } else {
                if (unit->flags & EVT_UNIT_FLAG_TARGET_BLEND_PHASES) {
                    if ((u16)unit->transitionFrameCount) {
                        transition = (f32)(u16)unit->transitionElapsed / (u16)unit->transitionFrameCount;
                    } else {
                        transition = 1.0f;
                    }
                    if (unit->flags & EVT_UNIT_FLAG_TARGET_BLEND_OUT) {
                        transition = 1.0f - transition;
                    }
                    if ((u16)unit->transitionFrameCount) {
                        if ((u16)unit->transitionFrameCount <= (u16)++unit->transitionElapsed) {
                            if (unit->flags & EVT_UNIT_FLAG_TARGET_BLEND_OUT) {
                                unit->currentTransitionValue = 0;
                                unit->flags &= ~EVT_UNIT_FLAG_TARGET_TRANSITION;
                            }
                            unit->previousTransitionValue = 0;
                            unit->flags &= ~EVT_UNIT_FLAG_TARGET_BLEND_PHASES;
                        }
                    }
                } else {
                    transition = 1.0f;
                }
                flags = unit->flags;
            }
        }
    } else {
        flags = unit->flags;
    }
    /* Direction consumes the snapshot after the target transition updates. */
    if (flags & 0x6000) {
        if (unit->directionFramesRemaining) {
            factor = 1.0f / unit->directionFramesRemaining;
        } else {
            factor = 1.0f;
        }
        VU0_LOAD_VF(vf10, unit->vec20);
        if (flags & 0x2000) {
            VU0_LOAD_VF(vf11, unit->vec30);
        } else if (ownVector) {
            VU0_LOAD_VF(vf11, target->direction);
        } else {
            VU0_LOAD_VF(vf11, D_0037F770[0] + 4);
        }
        kwlnBuildShortestArcQuaternion();
        VU0_MOVE_VF(vf11, vf10);
        VU0_LOAD_VF(vf10, identity);
        effMiscQuaternionNlerpVU(factor);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, unit->vec20);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf10, unit->vec40);
        VU0_STORE_VF_UNCLOBBERED(vf10, unit->vec20);
        if (unit->directionFramesRemaining == 0) {
            flags = unit->flags;
            if (flags & 0x4000) {
                unit->flags &= ~0x400;
                flags = unit->flags;
            }
            unit->flags = flags & ~0x6000;
        } else {
            unit->directionFramesRemaining--;
        }
    } else if (!(flags & 0x400)) {
        if (ownVector) {
            VU0_LOAD_VF(vf10, target->direction);
        } else {
            VU0_LOAD_VF(vf10, D_0037F770[0] + 4);
        }
        VU0_STORE_VF_UNCLOBBERED(vf10, unit->vec40);
    }
    if (target && !ownVector) {
        factor = transition;
        VU0_LOAD_VF(vf10, unit->vec40);
        VU0_LOAD_VF(vf11, targetDirection);
        kwlnBuildShortestArcQuaternion();
        VU0_MOVE_VF(vf11, vf10);
        VU0_LOAD_VF(vf10, identity);
        effMiscQuaternionNlerpVU(factor);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, unit->vec40);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf10, blendedDirection);
        if (previous) {
            factor = 1.0f;
            VU0_LOAD_VF(vf10, unit->vec40);
            VU0_LOAD_VF(vf11, previousDirection);
            kwlnBuildShortestArcQuaternion();
            VU0_MOVE_VF(vf11, vf10);
            VU0_LOAD_VF(vf10, identity);
            effMiscQuaternionNlerpVU(factor);
            effMiscQuaternionToMatrixVU();
            VU0_LOAD_VF(vf10, unit->vec40);
            VU0_APPLY_MATRIX(vf10, vf10);
            VU0_STORE_VF_UNCLOBBERED(vf10, oldDirection);
            VU0_LOAD_VF(vf11, blendedDirection);
            kwlnBuildShortestArcQuaternion();
            VU0_MOVE_VF(vf11, vf10);
            VU0_LOAD_VF(vf10, identity);
            effMiscQuaternionNlerpVU(transition);
            effMiscQuaternionToMatrixVU();
            VU0_LOAD_VF(vf10, oldDirection);
            VU0_APPLY_MATRIX(vf10, vf10);
        }
        VU0_STORE_VF_UNCLOBBERED(vf10, unit->vec10);
    } else {
        PCP_COPY_VECTOR(unit->vec10, unit->vec40);
    }
    /* Advance the two local colour tracks before applying target influence. */
    originalFlags = unit->flags;
    flags = originalFlags;
    if (originalFlags & 0x1800) {
        u32 firstPacked;
        u32 secondPacked;

        if (unit->colorFramesRemaining) {
            factor = 1.0f / unit->colorFramesRemaining;
        } else {
            factor = 1.0f;
        }
        firstSource[0] = unit->firstCurrent;
        EE_MMI_RGBA_UNPACK_READONLY(firstSource, 0.0078125f);
        VU0_SCALE_VF(vf10, 1.0f - factor);
        VU0_MOVE_VF(vf11, vf10);
        if (unit->flags & 0x800) {
            firstTarget[0] = unit->color08;
            EE_MMI_RGBA_UNPACK_READONLY(firstTarget, 0.0078125f);
        } else if (ownVector) {
            VU0_LOAD_VF(vf10, target);
        } else {
            VU0_LOAD_VF(vf10, D_0037F770[0]);
        }
        VU0_SCALE_VF(vf10, factor);
        VU0_ADD(vf10, vf10, vf11);
        VU0_CLEAR_W(vf10);
        EE_MMI_RGBA_PACK_UNIT(firstPacked, 128.0f);
        firstResult[0] = firstPacked;
        unit->firstCurrent = firstResult[0];
        unit->color0C = unit->firstCurrent;
        secondSource[0] = unit->color54;
        EE_MMI_RGBA_UNPACK_READONLY(secondSource, 0.0078125f);
        VU0_SCALE_VF(vf10, 1.0f - factor);
        VU0_MOVE_VF(vf11, vf10);
        if (unit->flags & 0x800) {
            secondTarget[0] = unit->color58;
            EE_MMI_RGBA_UNPACK_READONLY(secondTarget, 0.0078125f);
        } else if (ownVector) {
            VU0_LOAD_VF(vf10, target->secondColor);
        } else {
            VU0_LOAD_VF(vf10, kwlnDefaultColorVector);
        }
        VU0_SCALE_VF(vf10, factor);
        VU0_ADD(vf10, vf10, vf11);
        VU0_SET_W_ONE(vf10);
        EE_MMI_RGBA_PACK_UNIT(secondPacked, 128.0f);
        secondResult[0] = secondPacked;
        unit->color54 = secondResult[0];
        unit->color5C = unit->color54;
        if (unit->colorFramesRemaining == 0) {
            if (originalFlags & 0x1000) {
                flags = unit->flags = originalFlags & ~EVT_UNIT_FLAG_USE_UNIT_COLOR_CHANNELS;
            }
            unit->flags = flags & ~0x1800;
        } else {
            unit->colorFramesRemaining--;
        }
    } else {
        u32 firstPacked;
        u32 secondPacked;
        if (!(originalFlags & EVT_UNIT_FLAG_USE_UNIT_FIRST_COLOR)) {
            if (ownVector) {
                VU0_LOAD_VF(vf10, target);
            } else {
                VU0_LOAD_VF(vf10, D_0037F770[0]);
            }
            EE_MMI_RGBA_PACK_UNIT(firstPacked, 128.0f);
            defaultFirst[0] = firstPacked;
            unit->color0C = defaultFirst[0];
        }
        if (!(originalFlags & EVT_UNIT_FLAG_USE_UNIT_SECOND_COLOR)) {
            if (ownVector) {
                VU0_LOAD_VF(vf10, target->secondColor);
            } else {
                VU0_LOAD_VF(vf10, kwlnDefaultColorVector);
            }
            EE_MMI_RGBA_PACK_UNIT(secondPacked, 128.0f);
            defaultSecond[0] = secondPacked;
            unit->color5C = defaultSecond[0];
        }
    }
    if (target && !ownVector) {
        u32 packed;
        f32 weightedTransition;
        factor = currentWeight * transition;
        weightedTransition = factor;
        currentFirst[0] = unit->color0C;
        EE_MMI_RGBA_UNPACK_READONLY(currentFirst, 0.0078125f);
        VU0_SCALE_VF(vf10, 1.0f - factor);
        VU0_LOAD_VF(vf11, target);
        VU0_SCALE_VF(vf11, factor);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, blendedDirection);
        if (previous) {
            factor = previousWeight;
            previousFirst[0] = unit->color0C;
            EE_MMI_RGBA_UNPACK_READONLY(previousFirst, 0.0078125f);
            VU0_SCALE_VF(vf10, 1.0f - factor);
            VU0_LOAD_VF(vf11, previous);
            VU0_SCALE_VF(vf11, factor);
            VU0_ADD(vf10, vf10, vf11);
            VU0_SCALE_VF(vf10, 1.0f - transition);
            VU0_LOAD_VF(vf11, blendedDirection);
            VU0_SCALE_VF(vf11, transition);
            VU0_ADD(vf10, vf10, vf11);
        }
        VU0_CLEAR_W(vf10);
        EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
        finalFirst[0] = packed;
        unit->color = finalFirst[0];
        factor = weightedTransition;
        currentSecond[0] = unit->color5C;
        EE_MMI_RGBA_UNPACK_READONLY(currentSecond, 0.0078125f);
        VU0_SCALE_VF(vf10, 1.0f - factor);
        VU0_LOAD_VF(vf11, target->secondColor);
        VU0_SCALE_VF(vf11, factor);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, blendedDirection);
        if (previous) {
            factor = previousWeight;
            previousSecond[0] = unit->color5C;
            EE_MMI_RGBA_UNPACK_READONLY(previousSecond, 0.0078125f);
            VU0_SCALE_VF(vf10, 1.0f - factor);
            VU0_LOAD_VF(vf11, previous->secondColor);
            VU0_SCALE_VF(vf11, factor);
            VU0_ADD(vf10, vf10, vf11);
            VU0_SCALE_VF(vf10, 1.0f - transition);
            VU0_LOAD_VF(vf11, blendedDirection);
            VU0_SCALE_VF(vf11, transition);
            VU0_ADD(vf10, vf10, vf11);
        }
        VU0_CLEAR_W(vf10);
        EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
        finalSecond[0] = packed;
        unit->color50 = finalSecond[0];
    } else {
        unit->color = unit->color0C;
        unit->color50 = unit->color5C;
    }
    if (target && !ownVector) {
        auxFirst = target->auxFirst;
        auxSecond = target->auxSecond;
        if (auxFirst >= 1.0f) {
            factor = currentWeight * transition;
            auxFirst = D_00438A48 * (1.0f - factor) + auxFirst * factor;
            auxSecond = D_00438A4C * (1.0f - factor) + auxSecond * factor;
        }
        if (previous) {
            f32 oldFirst = previous->auxFirst;
            f32 oldSecond = previous->auxSecond;
            if (oldFirst >= 1.0f) {
                factor = previousWeight;
                oldFirst = D_00438A48 * (1.0f - factor) + oldFirst * factor;
                oldSecond = D_00438A4C * (1.0f - factor) + oldSecond * factor;
                auxFirst = auxFirst * transition + oldFirst * (1.0f - transition);
                auxSecond = auxSecond * transition + oldSecond * (1.0f - transition);
            }
        }
        overrideAux = 1;
    }
    if (!target && evtFindUnitSlotAuxCoordinates(unit, &auxFirst, &auxSecond)) {
        overrideAux = 1;
    }
    if (overrideAux) {
        sdfSetModelScalarOverrides(unit->owner->inner, auxFirst, auxSecond);
    } else {
        sdfClearModelScalarOverrides(unit->owner->inner);
    }
}


/* vu0 routine: load vf10 with the unit's colour (flag 0x100), its target's vector, or the default */
void evtLoadUnitFirstColorVectorVU(EvtUnit *unit) {
    EvtTargetInfo *info = 0;
    s32 ownVector = 0;
    s32 color;
    f32 scale;

    if (unit->currentTransitionValue != 0 && (unit->flags & EVT_UNIT_FLAG_TARGET_TRANSITION)) {
        info = ((EffWorldNode *)unit->currentTransitionValue)->data;
        if (info->flags & EVT_TARGET_INFO_FLAG_UNIT_OWNS_VECTOR) {
            ownVector = 1;
        }
    }
    if (unit->flags & EVT_UNIT_FLAG_USE_UNIT_FIRST_COLOR) {
        scale = 0.0078125f;
        color = unit->color;
        EE_MMI_RGBA_UNPACK(&color, scale);
    } else if (ownVector) {
        VU0_LOAD_VF(vf10, info);
    } else {
        VU0_LOAD_VF(vf10, D_0037F770[0]);
    }
}

/* vu0 routine: load the target info secondColor vector when unit-owned. */
void evtLoadUnitSecondColorVectorVU(EvtUnit *unit) {
    EvtTargetInfo *info = 0;
    s32 ownVector = 0;
    s32 color;
    f32 scale;

    if (unit->currentTransitionValue != 0 && (unit->flags & EVT_UNIT_FLAG_TARGET_TRANSITION)) {
        info = ((EffWorldNode *)unit->currentTransitionValue)->data;
        if (info->flags & EVT_TARGET_INFO_FLAG_UNIT_OWNS_VECTOR) {
            ownVector = 1;
        }
    }
    if (unit->flags & EVT_UNIT_FLAG_USE_UNIT_SECOND_COLOR) {
        scale = 0.0078125f;
        color = unit->color50;
        EE_MMI_RGBA_UNPACK(&color, scale);
    } else if (ownVector) {
        VU0_LOAD_VF(vf10, info->secondColor);
    } else {
        VU0_LOAD_VF(vf10, kwlnDefaultColorVector);
    }
}

/* vu0 routine: load vf10 with the unit's vector, or the default one */
void evtLoadUnitDirectionVectorVU(EvtUnit *unit) {
    s32 ownVector = 0;

    if (unit->currentTransitionValue != 0 && (unit->flags & EVT_UNIT_FLAG_TARGET_TRANSITION)) {
        if (((EvtTargetInfo *)((EffWorldNode *)unit->currentTransitionValue)->data)->flags & EVT_TARGET_INFO_FLAG_UNIT_OWNS_VECTOR) {
            ownVector = 1;
        }
    }
    if (unit->flags & 0x400) {
        VU0_LOAD_VF(vf10, unit->vec10);
    } else if (ownVector) {
        VU0_LOAD_VF(vf10, unit->vec10);
    } else {
        VU0_LOAD_VF(vf10, D_0037F770[0] + 4);
    }
}


extern void evtApplyMatchingUnitSlotEndpoints(EvtUnit *unit);

/* Refresh endpoint render work after the value-change flag is cleared; defer
 * to the matching-slot selector when no target or color transition is active. */
void evtRefreshUnitEndpointWork(EvtUnit *unit) {
    f32 ends[4][4];
    f32 color[4];
    SdfLightSources desc = { ends, 0, 0 };
    u32 packed0[4];
    u32 packed1[4];
    f32 scale0;
    f32 scale1;
    u32 flags = unit->flags;
    s32 colorChannelIndex;

    if (flags & EVT_UNIT_FLAG_VALUE_CHANGED) {
        return;
    }
    unit->value = 0;
    if (!(flags & EVT_UNIT_FLAG_TARGET_TRANSITION)) {
        if (!(flags & 0x700)) {
            evtApplyMatchingUnitSlotEndpoints(unit);
            return;
        }
    }
    packed0[0] = unit->color;
    scale0 = 0.0078125f;
    EE_MMI_RGBA_UNPACK(packed0, scale0);
    VU0_STORE_VF(vf10, ends[0]);
    VU0_LOAD_VF(vf10, unit->vec10);
    VU0_STORE_VF(vf10, ends[1]);
    packed1[0] = unit->color50;
    scale1 = 0.0078125f;
    EE_MMI_RGBA_UNPACK(packed1, scale1);
    VU0_STORE_VF(vf10, color);
    color[3] = 1.0f;
    for (colorChannelIndex = 0; colorChannelIndex < 3; colorChannelIndex++) {
        if (color[colorChannelIndex] > 1.0f) {
            color[colorChannelIndex] = 1.0f;
        }
    }
    sdfBuildLightingPacket(unit->endpointWork, desc, color);
    unit->value = (u32)unit->endpointWork;
}

void evtSetUnitValueTransition(EvtUnit *unit, EffWorldNode *target, s32 duration) {
    unit->flags |= EVT_UNIT_FLAG_TARGET_TRANSITION;
    unit->previousTransitionValue = unit->currentTransitionValue;
    unit->currentTransitionValue = (s32)target;
    if (duration == 0) {
        unit->previousTransitionValue = 0;
        unit->transitionElapsed = 0;
        unit->transitionDuration = 0;
        unit->flags &= ~EVT_UNIT_FLAG_TARGET_BLEND_PHASES;
    } else {
        unit->flags |= EVT_UNIT_FLAG_TARGET_BLEND_IN;
        unit->flags &= ~EVT_UNIT_FLAG_TARGET_BLEND_OUT;
        unit->transitionDuration = duration;
        unit->transitionElapsed = 0;
    }
}

void evtEndUnitValueTransition(EvtUnit *unit, s32 duration) {
    if (unit->currentTransitionValue != 0) {
        if (duration == 0) {
            unit->flags &= ~EVT_UNIT_FLAG_TARGET_TRANSITION;
            unit->flags &= ~EVT_UNIT_FLAG_TARGET_BLEND_PHASES;
            unit->currentTransitionValue = 0;
        } else {
            unit->flags &= ~EVT_UNIT_FLAG_TARGET_BLEND_IN;
            unit->transitionDuration = duration;
            unit->flags |= EVT_UNIT_FLAG_TARGET_BLEND_OUT;
            unit->transitionElapsed = 0;
        }
    }
}

void evtUnitSetValueAndFlag(EvtUnit *unit, u32 value) {
    unit->value = value;
    unit->flags = unit->flags | EVT_UNIT_FLAG_VALUE_CHANGED;
}

void evtClearUnitValueChangeFlag(EvtUnit *unit) {
    unit->flags = unit->flags & ~EVT_UNIT_FLAG_VALUE_CHANGED;
    evtRefreshUnitEndpointWork(unit);
}

void evtInitializeUnitColorTransition(EvtUnit *unit, s32 duration, u32 firstColor, u32 secondColor) {
    s32 firstColorVector[4];
    s32 secondColorVector[4];
    u32 firstPackedColor;
    u32 secondPackedColor;
    union {
        f32 scaleValue;
        u32 scaleBits;
    } packedScale;

    unit->unused1B0 = duration;
    unit->color08 = firstColor;
    unit->color58 = secondColor;
    packedScale.scaleValue = 128.0f;
    evtLoadUnitFirstColorVectorVU(unit);
    EE_MMI_RGBA_PACK_UNIT(firstPackedColor, packedScale.scaleBits);
    firstColorVector[0] = firstPackedColor;
    unit->firstCurrent = firstPackedColor;
    evtLoadUnitSecondColorVectorVU(unit);
    EE_MMI_RGBA_PACK_UNIT(secondPackedColor, packedScale.scaleBits);
    secondColorVector[0] = secondPackedColor;
    unit->color54 = secondPackedColor;
    unit->flags = (unit->flags | 0x800) & ~0x1000;
}

/* vu0 routine: normalize the direction in vf10, store it to vec30, then vf10 from evtLoadUnitDirectionVectorVU to vec20 */
void evtSetUnitNormalizedDirection(EvtUnit *unit, s32 directionMode) {
    unit->directionMode = directionMode;
    VU0_CLEAR_W(vf10);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF_UNCLOBBERED(vf10, unit->vec30);
    evtLoadUnitDirectionVectorVU(unit);
    VU0_STORE_VF_UNCLOBBERED(vf10, unit->vec20);
    unit->flags = (unit->flags | 0x2400) & ~0x4000;
}



void evtSetUnitRgbTransition(EvtUnit *unit, s32 duration, u32 color) {
    unit->rgbDuration = duration;
    unit->rgbElapsed = 0;
    if (duration == 0) {
        mdlBroadcastMasked((MdlCtx *)unit->owner,
            (mdlGetBroadcastValue((MdlCtx *)unit->owner) & 0xFF000000) | (color & 0xFFFFFF));
        unit->color60 = (unit->color60 & 0xFF000000) | (color & 0xFFFFFF);
        unit->flags &= ~EVT_UNIT_FLAG_RGB_TRANSITION;
    } else {
        u32 currentRgb = mdlGetBroadcastValue((MdlCtx *)unit->owner) & 0xFFFFFF;

        unit->color60 = (unit->color60 & 0xFF000000) | currentRgb;
        unit->color64 = (unit->color64 & 0xFF000000) | (color & 0xFFFFFF);
        unit->flags |= EVT_UNIT_FLAG_RGB_TRANSITION;
    }
}

void evtSetUnitAlphaTransition(EvtUnit *unit, s32 duration, u32 color) {
    unit->alphaDuration = duration;
    unit->alphaElapsed = 0;
    if (duration == 0) {
        mdlBroadcastMasked((MdlCtx *)unit->owner,
            (mdlGetBroadcastValue((MdlCtx *)unit->owner) & 0xFFFFFF) | (color & 0xFF000000));
        unit->color60 = (unit->color60 & 0xFFFFFF) | (color & 0xFF000000);
        unit->flags &= ~EVT_UNIT_FLAG_ALPHA_TRANSITION;
    } else {
        u32 currentAlpha = mdlGetBroadcastValue((MdlCtx *)unit->owner) & 0xFF000000;

        unit->color60 = (unit->color60 & 0xFFFFFF) | currentAlpha;
        unit->color64 = (unit->color64 & 0xFFFFFF) | (color & 0xFF000000);
        unit->flags |= EVT_UNIT_FLAG_ALPHA_TRANSITION;
    }
}

u8 evtTestUnitStatusFlags(EvtUnit *unit) {
    return (unit->flags & 0x7800) != 0;
}

void evtSetUnitStatusFlags(EvtUnit *unit) {
    unit->flags = unit->flags | EVT_UNIT_FLAG_USE_UNIT_COLOR_CHANNELS;
}

void evtConfigureUnitTransition(EvtUnit *unit, s32 duration) {
    if (duration == 0) {
        unit->flags &= ~0x700;
        unit->flags &= ~0x5000;
        unit->flags &= ~0x2800;
    } else {
        evtInitializeUnitColorTransition(unit, duration, 0, 0);
        unit->flags = (unit->flags & ~0x800) | 0x1000;
        VU0_MOVE_VF(vf10, vf0);
        evtSetUnitNormalizedDirection(unit, duration);
        unit->flags = (unit->flags & ~0x2000) | 0x4000;
    }
}

EvtUnit *evtGetWorldUnitNestedValue(s32 worldObjectKey) {
    EffWorldNode *obj = dds3FindWorldObjectNodeByKey(dds3GetWorldObject(), worldObjectKey, EFF_WORLD_KIND_FOLLOW_MODEL);

    if (obj != NULL) {
        return (EvtUnit *)*(s32 *)((u8 *)obj->data + 8);
    }
    return NULL;
}

EvtUnit *evtUnitGetNestedValue(EffWorldNode *unit) {
    if (unit == NULL) {
        return NULL;
    }
    return (EvtUnit *)*(s32 *)((u8 *)unit->data + 8);
}

extern f32 D_004215D0[];


EvtUnit *evtCreateUnitTransitionWork(EffWorldNode *effObj, MdlCtx *owner) {
    EvtUnit *work;
    struct SdfLightingPacketStorage *endpoint;
    f32 defaultVector[4];
    s32 i;

    memcpy(defaultVector, D_004215D0, sizeof(defaultVector));
    work = sdfChipAllocDirect(sizeof(EvtUnit));
    memset(work, 0, sizeof(EvtUnit));
    work->motionState = EVT_UNIT_MOTION_STATE_IDLE;
    work->transitionSourceKind = EVT_UNIT_TRANSITION_SOURCE_WORLD_NODE;
    work->transitionMotionScale = 1.0f;
    work->effObj = effObj;
    work->owner = owner;
    work->unkC0 = 10;
    EVT_UNIT_MOTION_SETUP(work)->endFrame = 10;
    work->unkBE = 0;
    EVT_UNIT_MOTION_SETUP(work)->startFrame = 0;

    VU0_LOAD_VF(vf10, defaultVector);
    VU0_STORE_VF_UNCLOBBERED(vf10, work->vec10);
    VU0_STORE_VF_UNCLOBBERED(vf10, work->vec40);
    work->color0C = 0x00B2B2B2;
    work->color = 0x00B2B2B2;
    work->color5C = 0x80303030;
    work->color50 = 0x80303030;
    endpoint = sdfChipAllocDirect(sizeof(*endpoint));
    work->endpointWork = endpoint;
    memset(endpoint, 0, sizeof(*endpoint));
    work->value = 0;
    work->referenceAngleCacheFlags = 0;
    work->cachedReferenceAngleDegrees = 0;
    {
        f32 *slot = &work->unk138[1];

        i = 10;
        do {
            i--;
            *slot = 1.0f;
            slot++;
        } while (i >= 0);
    }
    return work;
}

s32 evtReleaseUnitTransitionWork(EvtUnit *transitionWork) {
    struct SdfLightingPacketStorage *endpointWork;

    if (transitionWork == NULL) {
        return 1;
    }
    endpointWork = transitionWork->endpointWork;
    transitionWork->owner->inner->lighting = 0;
    if (endpointWork != NULL) {
        sdfChipFreeDirect(endpointWork);
        transitionWork->endpointWork = NULL;
    }
    if (transitionWork->pathHandle != 0) {
        dds3FreePathObject((Dds3PathCurveWork *)transitionWork->pathHandle);
        transitionWork->pathHandle = 0;
    }
    sdfChipFreeDirect(transitionWork);
    return 1;
}

s32 evtGetUnitMotionState(EvtUnit *unit) {
    return unit->motionState;
}

/* Callers pass the promoted full-width value; only the low halfword is stored. */
void evtUnitSetStoredParameter(EvtUnit *unit, s32 storedParameter) {
    unit->unkBC = storedParameter;
}

void evtSetTransitionMotionScale(EvtUnit *unit, f32 transitionMotionScale) {
    unit->transitionMotionScale = transitionMotionScale;
}

void evtStoreUnitMotionShortParameters(EvtUnit *unit, s32 firstParameter, s32 secondParameter) {
    unit->unkBE = firstParameter;
    unit->unkC0 = secondParameter;
}

s32 evtIsUnitMotionIdleOrTimedMode(EvtUnit *unit) {
    s32 state = evtGetUnitMotionState(unit);

    if (state == EVT_UNIT_MOTION_STATE_IDLE) {
        return 1;
    }
    if (state == EVT_UNIT_MOTION_STATE_MOTION && unit->motionTicks > 0 &&
        unit->owner->first->state == SDF_MOTION_STATE_TERMINAL) {
        return 1;
    }
    return 0;
}

void evtStoreUnitMotionSlotSelection(EvtUnit *unit, s32 firstSlot, s32 secondSlot) {
    unit->firstSlot = firstSlot;
    unit->secondSlot = secondSlot;
}

void evtActivateStoredUnitMotionSlot(EvtUnit *unit) {
    evtConfigureUnitMotionSlot(unit, unit->firstSlot, unit->secondSlot, 0, 0, 2);
}

void evtConfigureUnitMotionSlot(EvtUnit *unit, s32 slot, s32 slotAValue, s32 slotBValue,
                                s32 slotCValue, s32 mode) {
    unit->slotFlags[slot] = 3;
    unit->slotA[slot] = slotAValue;
    unit->slotB[slot] = slotBValue;
    unit->slotC[slot] = slotCValue;
    switch (mode) {
    case 0:
        unit->slotFlags[slot] |= 0x4;
        break;
    case 1:
        unit->slotFlags[slot] |= 0x8;
        break;
    case 2:
        unit->slotFlags[slot] |= 0x10;
        break;
    }
}

void evtPrepareUnitMotionState(EvtUnit *unit, s32 motionId, s32 startFrame, s32 endFrame, s32 mode) {
    unit->flags &= ~0x1;
    unit->flags &= ~0x20;
    unit->flags &= ~0x40;
    unit->flags &= ~0x400000;
    unit->flags &= ~0x800000;
    unit->motionState = EVT_UNIT_MOTION_STATE_MOTION;
    EVT_UNIT_MOTION_SETUP(unit)->motionId = motionId;
    EVT_UNIT_MOTION_SETUP(unit)->startFrame = startFrame;
    EVT_UNIT_MOTION_SETUP(unit)->endFrame = endFrame;
    unit->flags |= 0x80;
    unit->motionTicks = 0;
    unit->motionCallback = 0;
    switch (mode) {
    case 0:
        unit->flags |= 0x20;
        break;
    case 1:
        unit->flags |= 0x1;
        break;
    case 2:
        break;
    case 3:
        unit->flags |= 0x40;
        break;
    }
}

extern const s32 D_004215E0[];
extern const s32 D_004215F0[];
extern void evtComputeHorizontalDisplacementVu(f32 *a, f32 *b);
extern void effMiscAxisAngleToQuaternionVU(f32 angle);

s32 evtRotateUnitTowardTarget(EvtUnit *unit, f32 *dir, f32 scale) {
    /* SDK stores end local CSE before the axis and quaternion addresses
     * are rebuilt. One angle carries the cosine through acos and clamping. */
    f32 a[4];
    f32 b[4];
    f32 q[4];
    f32 out[4];
    f32 dot1;
    f32 rotationAngle;
    s32 result = 1;

    memcpy(a, D_004215E0, sizeof(a));
    memcpy(b, D_004215F0, sizeof(b));
    evtComputePlanarTargetDirectionVu(unit);
    VU0_STORE_VF(vf10, a);
    VU0_MOVE_VF(vf12, vf10);
    evtComputeHorizontalDisplacementVu(dir, unit->effObj->inner->position);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF_UNCLOBBERED(vf10, out);
    VU0_LOAD_VF(vf11, b);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf11, vf12);
    VU0_DOT_XYZ(dot1, vf10, vf11);
    VU0_LOAD_VF(vf10, out);
    VU0_DOT_XYZ(rotationAngle, vf10, vf11);
    if (0.9998f <= rotationAngle) {
        return result;
    }
    rotationAngle = acosf(rotationAngle);
    if (dot1 > 0.0f) {
        rotationAngle = -rotationAngle;
    }
    rotationAngle *= scale;
    if (!(unit->flags & 0x200000)) {
        if (rotationAngle < (-3.14159265f / 20.0f)) {
            rotationAngle = (-3.14159265f / 20.0f);
        }
        if ((3.14159265f / 20.0f) < rotationAngle) {
            rotationAngle = (3.14159265f / 20.0f);
        }
    }
    VU0_LOAD_VF(vf10, b);
    effMiscAxisAngleToQuaternionVU(rotationAngle);
    VU0_STORE_VF_UNCLOBBERED(vf10, q);
    effObjMultiplyInnerRotation(unit->effObj, (u128 *)q);
    return 0;
}



/* Flat, negated direction of the rotation in quat, offset by the effect object's point, aimed with evtRotateUnitTowardTarget. */
s32 evtAimUnitFromFlatQuaternion(EvtUnit *unit, f32 *quat, f32 angle) {
    f32 v[4];

    VU0_LOAD_VF(vf10, quat);
    effMiscQuaternionToMatrixVU();
    VU0_MOVE_VF(vf10, vf30);
    VU0_SET_AXIS_CLEAR_W(0.0f, y);
    VU0_NORMALIZE_VF10();
    VU0_SCALAR_OP(-1.0f, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_LOAD_VF(vf11, unit->effObj->inner->position);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, v);
    return evtRotateUnitTowardTarget(unit, v, angle);
}

s32 evtApplyUnitDirectionOffset(EvtUnit *unit) {
    f32 v[4];
    f32 scale;
    EffWorldNode *obj;

    evtRotateUnitTowardTarget(unit, unit->targetVector, unit->directionOffset * 0.01f);
    if (unit->directionOffset != 0) {
        evtComputePlanarTargetDirectionVu(unit);
        obj = unit->effObj;
    } else {
        VU0_LOAD_VF(vf10, unit->targetVector);
        obj = unit->effObj;
        VU0_LOAD_VF(vf11, obj->inner->position);
        VU0_SUB(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10();
    }
    scale = unit->motionParameter * 0.1f;
    VU0_SCALAR_OP(scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_STORE_VF(vf10, v);
    effObjAddInnerPosition(obj, (u128 *)v);
    return 1;
}


extern s32 evtUnitHasReachedMotionEndpoint(EvtUnit *unit);
#include "sdf_debug_api.h"

/* Run a copy of the unit until evtUnitHasReachedMotionEndpoint reports done, and derive its per-step Y speed. */
INCLUDE_RODATA(const s32, "event/evtUnitManager", D_004215D0);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_004215E0);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_004215F0);

s32 evtUnitPrepareVerticalMoveSteps(EvtUnit *unit) {
    EvtUnit copy;
    f32 delta[4];
    f32 savedA[4];
    f32 savedB[4];
    s32 simulatedStepCount = 0;
    s32 vectorComponentIndex;
    for (vectorComponentIndex = 0; vectorComponentIndex < 4; vectorComponentIndex++) {
        savedA[vectorComponentIndex] = unit->effObj->inner->position[vectorComponentIndex];
        savedB[vectorComponentIndex] = unit->effObj->inner->rotation[vectorComponentIndex];
    }
    copy = *unit;
    while (evtUnitHasReachedMotionEndpoint(&copy) == 0) {
        simulatedStepCount++;
        evtApplyUnitDirectionOffset(&copy);
    }
    for (vectorComponentIndex = 0; vectorComponentIndex < 4; vectorComponentIndex++) {
        unit->effObj->inner->position[vectorComponentIndex] = savedA[vectorComponentIndex];
        unit->effObj->inner->rotation[vectorComponentIndex] = savedB[vectorComponentIndex];
    }
    if (simulatedStepCount == 0) {
        sdfDebugPrintf("ymove frameno = 0\n");
        unit->stepCount = 0;
        unit->speedY = 0;
        return 0;
    }
    VU0_LOAD_VF(vf10, unit->targetVector);
    VU0_LOAD_VF(vf11, unit->effObj->inner->position);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, delta);
    unit->stepCount = simulatedStepCount;
    unit->speedY = delta[1] / (f32)simulatedStepCount;
    sdfDebugPrintf("ymove frameno = %d addvalue = %f total yzahyo=%f\n", simulatedStepCount, unit->speedY, delta[1]);
    return simulatedStepCount;
}

s32 evtUnitApplyPathVectors(EvtUnit *unit) {
    f32 v[4];

    sdfStepWrappingFloatCounter((Dds3PathCurveWork *)unit->pathHandle);
    dds3InterpolatePathVectorVU((Dds3PathCurveWork *)unit->pathHandle);
    VU0_STORE_VF($vf10, v);
    dds3SetPosPoseSet(unit->effObj, (u128 *)v);
    if (unit->flags & 0x10) {
        dds3InterpolatePathQuaternionVU((Dds3PathCurveWork *)unit->pathHandle);
        VU0_MOVE_VF(vf11, vf10);
        sdfConvertEulerAnglesToQuaternionVU(0.0f, 3.14159265f, 0.0f);
        effMiscQuatMultiplyVU();
        VU0_STORE_VF($vf10, v);
        dds3SetRotPoseSet(unit->effObj, (u128 *)v);
    }
    return 1;
}

void evtCopyUnitTargetVector(EvtUnit *work, void *src) {
    PCP_COPY_VECTOR(work->targetVector, src);
}

#include "sdf_trig.h"
#include "scr_integer_return_value_api.h"
#include "sdf_unique_chunk_value_api.h"
#include "kwln_task_add_child.h"
#include "fpu.h"
#include "mdl_motion_api.h"
#include "mdl_world_object_source_vector_api.h"
#include "itf_mes_window.h"
#include "evt_world.h"
#include "evt_world_source_transform.h"
#include "evt_model_header.h"
#include "evt_lips_link.h"
#include "evt_reset_unit_vector_slots_api.h"
#include "evt_task.h"
#include "evt_event_pack.h"
#include "eff_object.h"
#include "eff_object_dispatch_api.h"
#include "eff_event.h"
#include "eff_node_descriptor.h"
#include "scr.h"
#include "scr_string_parameter_api.h"
#include "scr_arg_read.h"

extern EvtUnit *evtWindowMotionUnit;
extern s32 D_004371F0;

extern u32 sdfGetLodChunkValue(SdfModel *model);


extern EvtUnitVectorSlot evtUnitVectorSlots[10];



typedef struct {
    u8 pad00[0x10];     /* 0x00 */
    f32 scaleFactor;    /* 0x10: uniform model scale, also used by stage tests. */
    u8 pad14[0x04];     /* 0x14 */
    f32 unk18;          /* 0x18 */
    f32 unk1C;          /* 0x1C */
    u8 pad20[0x250];    /* 0x20 */
} Entry270;

typedef char Entry270_scaleFactor_offset_check[
    ((u32)&((Entry270 *)0)->scaleFactor == 0x10) ? 1 : -1];
typedef char Entry270_size_check[(sizeof(Entry270) == 0x270) ? 1 : -1];

extern Entry270 *D_00435DF0;
extern void mnuInitializeCampPanelVisualDefaults(f32 *firstVector, f32 *secondVector, f32 *thirdVector, f32 *scalarA, f32 *scalarB);
#include "dds3_world_secondary.h"
extern void sdfSetFloatCounterDirection(s32 destination, s32 direction);

extern void evtBeginUnitVectorTransition(EvtUnit *work, s32 motionSubmode, s128 *vector, s32 unused, s32 frames, s32 valueB6, s32 (*callback)(EvtUnit *, s32), s32 unusedLast);




extern s32 scrReadIntParameter(s32 idx);

extern void dds3ObjectBaseOnFlag(void *object, s32 flags);

extern void dds3ObjectBaseOffFlag(void *object, s32 flags);

extern void evtArmEffectObjectPendingValue(EffWorldNode *object, s32 value);

extern void evtResetObjectPendingValue(EffWorldNode *object);



/* World object views used by the model-parameter opcodes. */
extern EvtUnit *evtGetWorldUnitNestedValue(s32 idx);


extern s32 evtIsUnitMotionIdleOrTimedMode(EvtUnit *unit);



extern void evtPrepareUnitMotionState(EvtUnit *unit, s32 motionId, s32 startFrame, s32 endFrame, s32 mode);


extern u8 D_00421AC0[];


extern void effObjSetFlags(void *object, s32 flags);

extern EffWorldNode *effObjSpawnDescriptorBoundEffect(struct EffNodeDescriptor *descriptor, void *firstVector, void *secondVector);
extern void *effObjCreateEventNode(void *resource, void *position, void *scale);
extern EffWorldNode *effForwardMagatuhiDescriptor(s32 kind, struct EffNodeDescriptor *descriptor);
extern char D_00421A78[];

extern void *effObjSpawnLoadedResourceEffect(const char *resourceName, void *firstVector, void *secondVector);

extern u8 D_00421A90[];

extern void *effObjCreateFromResolvedResource(void *resource, void *firstVector, void *secondVector);

extern u8 D_00421B20[];

extern void *effObjCreateKindFromResource(s32 kind, void *resource);

extern u8 D_00421B70[];

extern s32 mdlSpawnLinkedCameraSlotViewerObject(s32 slotKind, s32 resource);

extern void evtConfigureUnitTransition(EvtUnit *unit, s32 duration);

extern u8 evtTestUnitStatusFlags(EvtUnit *unit);

extern void evtEndUnitValueTransition(EvtUnit *unit, s32 duration);

extern void evtPrintDeveloperConsoleMessage(const char *fmt, ...);

extern void effObjReplaceActiveEventNode(void *effectObject, u32 entryId);

extern char D_00421AF8[];

extern s32 evtCreateModelFromPackResource(s32 eventId, s32 resourceId);
extern s32 scrGetWindow(void);


extern void evtStoreUnitMotionSlotSelection(EvtUnit *unit, s32 firstSlot, s32 secondSlot);


extern f32 evtComputeClampedModelScale(s32);


/* Start a bounded vector transition; detach any previous secondary-world source. */
void evtBeginVectorTransition(EvtUnit *work, s128 *vector, s32 frames) {
    f32 *destination = work->targetVector;

    if ((u32)(frames - 1) < 100) {
        work->linkedUnit = NULL;
        work->motionState = EVT_UNIT_MOTION_STATE_VECTOR;
        PCP_COPY_VECTOR(destination, vector);
        work->motionParameter = frames;
        work->directionOffset = 0;
        work->motionCallback = 0;
        work->motionTicks = 0;
    }
}

/* Track a secondary-world unit and copy the vector in its subobject at +0x10. */
void evtAttachSecondaryWorldUnit(EvtUnit *work, s32 worldObjectKey, s32 frames) {
    EffWorldNode *worldUnit;
    EvtWorldSourceTransformPrefix *source;

    worldUnit = dds3FindWorldObjectNodeByKey(dds3GetWorldSecondaryObject(), worldObjectKey, 0x11);
    if (worldUnit != NULL) {
        source = worldUnit->data;
        evtBeginVectorTransition(work, (s128 *)&source->rotation, frames);
        work->linkedUnit = worldUnit;
    }
}

/* Configure a mode-one vector transition, without retaining a world source. */
void evtBeginUnitVectorTransition(EvtUnit *work, s32 motionSubmode, s128 *vector, s32 unused, s32 frames, s32 valueB6, s32 (*callback)(EvtUnit *, s32), s32 unusedLast) {
    f32 *destination = work->targetVector;

    work->motionSubmode = motionSubmode;
    work->motionState = EVT_UNIT_MOTION_STATE_SOURCE;
    work->transitionSourceKind = EVT_UNIT_TRANSITION_SOURCE_COPIED_VECTOR;
    work->linkedUnit = NULL;
    PCP_COPY_VECTOR(destination, vector);
    work->motionParameter = frames;
    work->directionOffset = valueB6;
    work->motionCallback = callback;
    work->motionTicks = 0;
}

/* Configure the same transition from a secondary-world object's vector. */
void evtBeginUnitTransitionTowardWorldObject(EvtUnit *work, s32 motionSubmode, s32 worldObjectKey, s32 unused, s32 frames, s32 valueB6, s32 (*callback)(EvtUnit *, s32), s32 unusedLast) {
    EffWorldNode *worldUnit;
    EvtWorldSourceTransformPrefix *source;

    worldUnit = dds3FindWorldObjectNodeByKey(dds3GetWorldSecondaryObject(), worldObjectKey, 0x11);
    if (worldUnit != NULL) {
        source = worldUnit->data;
        evtBeginUnitVectorTransition(work, motionSubmode, (s128 *)&source->position, unused, frames, valueB6, callback, unusedLast);
        work->transitionSourceKind = EVT_UNIT_TRANSITION_SOURCE_WORLD_NODE;
        work->linkedUnit = worldUnit;
    }
}

void evtSetUnitPathFollow(EvtUnit *work, s32 pathObjectKey, s32 frames, s32 valueB6, s32 pathFollowMode, s32 dirFlag, s32 sideMode) {
    EffWorldNode *pathSource;
    Dds3PathCurveWork *path;

    pathSource = dds3FindWorldObjectNodeByKey(dds3GetWorldSecondaryObject(), pathObjectKey, 0x10);
    if (pathSource == NULL) {
        return;
    }
    if (work->pathHandle != 0) {
        dds3FreePathObject((Dds3PathCurveWork *)work->pathHandle);
    }
    path = dds3CreatePathCurveWork(pathSource);
    work->pathHandle = (s32)path;
    work->pathSpeed = 40.0f / evtMeasurePathTrajectoryLength(path);
    if (dirFlag == 0) {
        evtScaleValueByMultiplier((EvtScaledValue *)path, 0.0f);
        sdfSetFloatCounterDirection((s32)path, 0);
    } else {
        evtScaleValueByMultiplier((EvtScaledValue *)path, 1.0f);
        sdfSetFloatCounterDirection((s32)path, 1);
        work->pathSpeed = -work->pathSpeed;
    }
    switch (pathFollowMode) {
    case 0:
        work->motionSubmode = 0;
        work->flags &= ~2;
        break;
    case 1:
        work->motionSubmode = 3;
        work->flags |= 2;
        break;
    }
    switch (dirFlag) {
    case 0:
        work->flags &= ~EVT_UNIT_FLAG_PATH_REVERSE;
        break;
    case 1:
        work->flags |= EVT_UNIT_FLAG_PATH_REVERSE;
        break;
    }
    switch (sideMode) {
    case 0:
        work->flags &= ~8;
        work->flags &= ~0x10;
        break;
    case 1:
        work->flags |= 8;
        work->flags &= ~0x10;
        break;
    case 2:
        work->flags &= ~8;
        work->flags |= 0x10;
        break;
    }
    work->motionState = EVT_UNIT_MOTION_STATE_SOURCE;
    work->transitionSourceKind = EVT_UNIT_TRANSITION_SOURCE_PATH_CURVE;
    work->linkedUnit = pathSource;
    dds3InterpolatePathVectorVU(path);
    VU0_STORE_VF($vf10, work->targetVector);
    work->motionParameter = frames;
    work->directionOffset = valueB6;
    work->motionCallback = 0;
    work->motionTicks = 0;
}

s32 evtStartUnitModeWithValue(EvtUnit *eventUnit, s32 (*callback)(EvtUnit *, s32)) {
    s32 result = 0;

    if (callback != 0) {
        eventUnit->motionCallback = callback;
        eventUnit->motionTicks = 0;
        eventUnit->motionState = EVT_UNIT_MOTION_STATE_VALUE;
        result = 1;
    }
    return result;
}

extern void evtAdvanceUnitVisualTransitions(EvtUnit *);
extern s32 evtAimUnitFromFlatQuaternion(EvtUnit *, f32 *, f32);
extern s32 evtUnitHasReachedMotionEndpoint(EvtUnit *);
extern s32 evtUnitStepScaledValue(EvtUnit *);
extern s32 evtApplyUnitDirectionOffset(EvtUnit *);
extern s32 evtUnitApplyPathVectors(EvtUnit *);
extern void evtRefreshUnitEndpointWork(EvtUnit *);
s32 func_0023DA70(ObjectTransform *unused, EffWorldNode *object) {
    f32 delta[4];
    s32 moving = 0;
    s32 wobble = 0;
    EffectObjectData *data = object->data;
    EvtUnit *unit = data->transitionWork;
    MdlCtx *model;
    s32 i;
    u8 enabled;

    if (unit == NULL) {
        return 0;
    }
    model = unit->owner;
    evtAdvanceUnitVisualTransitions(unit);
    switch (unit->motionState) {
    case EVT_UNIT_MOTION_STATE_IDLE:
    case EVT_UNIT_MOTION_STATE_VECTOR:
        if (unit->flags & 0x80) {
            if (unit->flags & 1) {
                mdlAddEntryFlaggedEx(model, 0, unit->motionSetupId, (f32)unit->motionSetupStartFrame, (f32)unit->motionSetupEndFrame);
            } else {
                mdlAddEntryPlainEx(model, 0, unit->motionSetupId, (f32)unit->motionSetupStartFrame, (f32)unit->motionSetupEndFrame);
            }
            unit->flags &= ~0x80;
        } else if (unit->flags & 0x20) {
            /* The selected-ID restart stays distinct in source: retail 0x0023DB84
             * and Nocturne 0x001E6864 retain the current-index load after tail merging. */
            s32 currentMotion = model->current.h.motionIndex;

            if (unit->unkBC != currentMotion) {
                if (model->first->state == 5) {
                    mdlAddEntryFlaggedEx(model, 0, unit->unkBC, (f32)unit->unkBE, (f32)unit->unkC0);
                    model->first->frameStep = unit->transitionMotionScale;
                    unit->motionTicks = 0;
                }
            } else {
                if (model->first->state == 5) {
                    mdlAddEntryFlaggedEx(model, 0, currentMotion, (f32)unit->unkBE, (f32)unit->unkC0);
                    model->first->frameStep = unit->transitionMotionScale;
                    unit->motionTicks = 0;
                }
            }
        }
        if (unit->flags & 0x40) {
            wobble = 1;
        } else {
            wobble = 0;
        }
        if (unit->motionState == EVT_UNIT_MOTION_STATE_VECTOR) {
            if (evtAimUnitFromFlatQuaternion(unit, unit->targetVector, (f32)unit->motionParameter / 100.0f) != 0) {
                unit->motionState = EVT_UNIT_MOTION_STATE_IDLE;
            }
        }
        break;
    case EVT_UNIT_MOTION_STATE_SOURCE:
        if (evtUnitHasReachedMotionEndpoint(unit) != 1 || evtUnitStepScaledValue(unit) != 0) {
            if (unit->motionSetupId != model->current.h.motionIndex) {
                if (unit->flags & 1) {
                    mdlAddEntryFlaggedEx(model, 0, unit->motionSetupId, (f32)unit->motionSetupStartFrame, (f32)unit->motionSetupEndFrame);
                } else {
                    mdlAddEntryPlainEx(model, 0, unit->motionSetupId, (f32)unit->motionSetupStartFrame, (f32)unit->motionSetupEndFrame);
                }
            }
            switch (unit->motionSubmode) {
            case 0:
            case 1:
                moving = evtApplyUnitDirectionOffset(unit);
                if (unit->motionSubmode == 1) {
                    delta[0] = 0.0f;
                    delta[1] = unit->speedY;
                    delta[2] = 0.0f;
                    delta[3] = 0.0f;
                    effObjAddInnerPosition(unit->effObj, (u128 *)delta);
                }
                break;
            case 2:
                break;
            case 3:
                moving = evtUnitApplyPathVectors(unit);
                break;
            default:
                unit->motionState = EVT_UNIT_MOTION_STATE_IDLE;
                break;
            }
            if (unit->motionCallback != NULL) {
                if (unit->motionCallback(unit, 0) == 1) {
                    unit->motionState = EVT_UNIT_MOTION_STATE_IDLE;
                }
            }
        } else {
            unit->motionState = EVT_UNIT_MOTION_STATE_IDLE;
        }
        break;
    case EVT_UNIT_MOTION_STATE_MOTION:
        if (unit->motionTicks == 0) {
            if (unit->flags & 1) {
                mdlAddEntryFlaggedEx(model, 0, unit->motionSetupId, (f32)unit->motionSetupStartFrame, (f32)unit->motionSetupEndFrame);
            } else {
                mdlAddEntryPlainEx(model, 0, unit->motionSetupId, (f32)unit->motionSetupStartFrame, (f32)unit->motionSetupEndFrame);
            }
            unit->flags &= ~0x80;
        } else if (model->first->state == 5) {
            unit->motionState = EVT_UNIT_MOTION_STATE_IDLE;
        } else if ((unit->flags & 0x800000) && (f32)unit->motionEndFrame <= model->first->currentFrame) {
            s32 currentMotion;

            unit->motionState = EVT_UNIT_MOTION_STATE_VALUE;
            currentMotion = model->current.h.motionIndex;
            unit->flags &= ~0x800000;
            if (unit->unkBC != currentMotion) {
                mdlAddEntryFlaggedEx(model, 0, unit->unkBC, (f32)unit->unkBE, (f32)unit->unkC0);
                model->first->frameStep = unit->transitionMotionScale;
                unit->motionTicks = 0;
            }
        }
        break;
    case EVT_UNIT_MOTION_STATE_VALUE:
        if (unit->motionCallback != NULL) {
            if (unit->motionCallback(unit, 0) == 1) {
                unit->motionState = EVT_UNIT_MOTION_STATE_IDLE;
            }
        } else {
            unit->motionState = EVT_UNIT_MOTION_STATE_IDLE;
        }
        break;
    default:
        return 0;
    }
    if (unit->flags & 0x400000) {
        wobble = 1;
    }
    if (wobble != 0) {
        unit->wobblePhase += 0.104719755f;
        if (6.2831853f <= unit->wobblePhase) {
            unit->wobblePhase -= 6.2831853f;
        }
        unit->effObj->inner->flags = (unit->effObj->inner->flags | 1) & ~2;
    }
    for (i = 1; i < 12; i++) {
        enabled = unit->slotFlags[i] & 1;
        if (enabled != 0) {
            if (unit->slotFlags[i] & 2) {
                unit->slotFlags[i] &= ~2;
                sdfDebugPrintf("set exmot %d_%d\n", i, unit->slotA[i]);
                if (unit->slotFlags[i] & 8) {
                    mdlAddEntryFlaggedEx(model, i, unit->slotA[i], (f32)unit->slotB[i], (f32)unit->slotC[i]);
                } else {
                    mdlAddEntryPlainEx(model, i, unit->slotA[i], (f32)unit->slotB[i], (f32)unit->slotC[i]);
                }
            }
            if (mdlGetNodeMotionTerminalStatus(model, i) != 0) {
                if (unit->slotFlags[i] & 4) {
                    if ((unit->flags & 0x1000000) == 0) {
                        mdlAddEntryFlaggedEx(model, i, unit->tableValues[i], 0.0f, 0.0f);
                    } else {
                        mdlAddEntryFlaggedEx(model, i, unit->tableValues[i], (f32)unit->unk108[i], (f32)unit->unk120[i]);
                        mdlSetNodeFrameStep(model, i, unit->unk138[i]);
                    }
                } else if ((unit->flags & 0x1000000) && (unit->slotFlags[i] & 0x10)) {
                    mdlAddEntryPlainEx(model, i, unit->slotA[i], 0.0f, 1.0f);
                    mdlSetNodeFrameStep(model, i, 0.0f);
                }
            }
        }
    }
    evtRefreshUnitEndpointWork(unit);
    if (unit->motionTicks < 0x7FFF) {
        unit->motionTicks++;
    }
    if (moving != 0) {
        if (unit->movingTicks < 0x7FFF) {
            unit->movingTicks++;
        }
        unit->idleTicks = 0;
    } else {
        if (unit->idleTicks < 0x7FFF) {
            unit->idleTicks++;
        }
        if (unit->idleTicks >= 6) {
            unit->movingTicks = 0;
        }
    }
    return moving;
}

void evtResetUnitVectorSlots(void) {
    s32 i;

    for (i = 0; i < 10; i++) {
        mnuInitializeCampPanelVisualDefaults(&evtUnitVectorSlots[i].vec[0], &evtUnitVectorSlots[i].vec[4], &evtUnitVectorSlots[i].vec[8], &evtUnitVectorSlots[i].auxX, &evtUnitVectorSlots[i].auxY);
        evtUnitVectorSlots[i].state = EVT_UNIT_VECTOR_SLOT_EMPTY;
        evtUnitVectorSlots[i].id = 0;
    }
}

void evtSetSlotVectors(s32 slotIndex, s32 slotState, s32 unitId, f32 *firstEndpoint, f32 *secondEndpoint, f32 *color) {
    if (slotIndex < 10) {
        evtUnitVectorSlots[slotIndex].vec[0] = firstEndpoint[0];
        evtUnitVectorSlots[slotIndex].state = slotState;
        evtUnitVectorSlots[slotIndex].vec[1] = firstEndpoint[1];
        evtUnitVectorSlots[slotIndex].vec[2] = firstEndpoint[2];
        evtUnitVectorSlots[slotIndex].vec[3] = 0;
        evtUnitVectorSlots[slotIndex].vec[4] = secondEndpoint[0];
        evtUnitVectorSlots[slotIndex].vec[5] = secondEndpoint[1];
        evtUnitVectorSlots[slotIndex].vec[6] = secondEndpoint[2];
        evtUnitVectorSlots[slotIndex].vec[7] = secondEndpoint[3];
        evtUnitVectorSlots[slotIndex].vec[8] = color[0];
        evtUnitVectorSlots[slotIndex].vec[9] = color[1];
        evtUnitVectorSlots[slotIndex].vec[10] = color[2];
        evtUnitVectorSlots[slotIndex].vec[11] = 1.0f;
        if (slotState == EVT_UNIT_VECTOR_SLOT_UNIT_BOUND) {
            evtUnitVectorSlots[slotIndex].id = unitId;
        } else {
            evtUnitVectorSlots[slotIndex].id = 0;
        }
    }
}

void evtSetSlotVector(s32 slotIndex, f32 x, f32 y) {
    if (slotIndex < 10) {
        evtUnitVectorSlots[slotIndex].auxX = x;
        evtUnitVectorSlots[slotIndex].auxY = y;
    }
}

/* Copy the second vector of the slot bound to `id` (else the first state-2 slot). */
s32 evtCopyUnitVectorSlotEndpoint(s32 id, f32 *out) {
    s32 found = -1;
    s32 i;

    for (i = 0; i < 10; i++) {
        if (evtUnitVectorSlots[i].state == EVT_UNIT_VECTOR_SLOT_UNIT_BOUND && evtUnitVectorSlots[i].id == id) {
            found = i;
            break;
        }
    }
    if (found == -1) {
        for (i = 0; i < 10; i++) {
            if (evtUnitVectorSlots[i].state == EVT_UNIT_VECTOR_SLOT_SHARED_FALLBACK) {
                found = i;
                break;
            }
        }
        if (found == -1) {
            return 0;
        }
    }
    out[0] = evtUnitVectorSlots[found].vec[4];
    out[1] = evtUnitVectorSlots[found].vec[5];
    out[2] = evtUnitVectorSlots[found].vec[6];
    return 1;
}

/* Select a matching slot and write its endpoint render work. */
void evtApplyMatchingUnitSlotEndpoints(EvtUnit *unit) {
    f32 ends[4][4];
    f32 color[4];
    SdfLightSources desc = { ends, 0, 0 };
    s32 found = -1;
    s32 i;

    for (i = 0; i < 10; i++) {
        if (evtUnitVectorSlots[i].state == EVT_UNIT_VECTOR_SLOT_UNIT_BOUND && evtUnitVectorSlots[i].id == (s32)unit) {
            found = i;
            break;
        }
    }
    if (found == -1) {
        for (i = 0; i < 10; i++) {
            if (evtUnitVectorSlots[i].state == EVT_UNIT_VECTOR_SLOT_SHARED_FALLBACK) {
                found = i;
                break;
            }
        }
        if (found == -1) {
            return;
        }
    }
    ends[0][0] = evtUnitVectorSlots[found].vec[0];
    ends[0][1] = evtUnitVectorSlots[found].vec[1];
    ends[0][2] = evtUnitVectorSlots[found].vec[2];
    ends[0][3] = 0;
    ends[1][0] = evtUnitVectorSlots[found].vec[4];
    ends[1][1] = evtUnitVectorSlots[found].vec[5];
    ends[1][2] = evtUnitVectorSlots[found].vec[6];
    ends[1][3] = 0;
    color[0] = evtUnitVectorSlots[found].vec[8];
    color[1] = evtUnitVectorSlots[found].vec[9];
    color[2] = evtUnitVectorSlots[found].vec[10];
    color[3] = 1.0f;
    for (i = 0; i < 3; i++) {
        if (color[i] > 1.0f) {
            color[i] = 1.0f;
        }
    }
    sdfBuildLightingPacket(unit->endpointWork, desc, color);
    unit->value = (u32)unit->endpointWork;
}

/* Find auxiliary coordinates for the unit-bound slot, else the shared fallback. */
s32 evtFindUnitSlotAuxCoordinates(EvtUnit *unit, f32 *outX, f32 *outY) {
    s32 i;

    for (i = 0; i < 10; i++) {
        if (evtUnitVectorSlots[i].state == EVT_UNIT_VECTOR_SLOT_UNIT_BOUND && evtUnitVectorSlots[i].id == (s32)unit) {
            *outX = evtUnitVectorSlots[i].auxX;
            *outY = evtUnitVectorSlots[i].auxY;
            return 1;
        }
    }
    for (i = 0; i < 10; i++) {
        if (evtUnitVectorSlots[i].state == EVT_UNIT_VECTOR_SLOT_SHARED_FALLBACK) {
            *outX = evtUnitVectorSlots[i].auxX;
            *outY = evtUnitVectorSlots[i].auxY;
            return 1;
        }
    }
    return 0;
}

void *evtFindWorldObjectByIdAndKind(s32 objectKind, s32 worldObjectKey) {
    void *world;

    world = dds3GetWorldObject();
    return dds3FindWorldObjectNodeByKey(world, worldObjectKey, objectKind);
}

u32 evtGetWorldObjectId(void) {
    void *world;
    EffWorldNode *object;
    s32 playerObjectKey;

    world = dds3GetWorldObject();
    object = dds3GetWorldPlayerObject(world);
    playerObjectKey = -1;
    if (object != NULL) {
        playerObjectKey = object->key;
    }
    scrSetIntegerReturnValue(playerObjectKey);
    return 1;
}

u32 evtOpCreateLinkedCameraViewer(void) {
    s32 slotKind;
    s32 resource;
    s32 viewerObjectKey;

    slotKind = scrReadIntParameter(0);
    resource = scrReadIntParameter(1);
    viewerObjectKey = mdlSpawnLinkedCameraSlotViewerObject(slotKind, resource);
    scrSetIntegerReturnValue(viewerObjectKey);
    return 1;
}

u32 evtOpBindMotionSoundToModel(void) {
    s32 eventId;
    s32 resourceId;
    s32 modelKey;
    KwlnTask *motionSeTask;

    if (scrGetCurrentContext() == 0) {
        return 1;
    }
    eventId = scrReadIntParameter(0);
    resourceId = scrReadIntParameter(1);
    modelKey = evtCreateModelFromPackResource(eventId, resourceId);
    if (modelKey < 0) {
        evtPrintDeveloperConsoleMessage("MODEL_BE not fount RID = %d!\n", scrReadIntParameter(1));
        return 1;
    }
    eventId = scrReadIntParameter(0);
    resourceId = scrReadIntParameter(1);
    motionSeTask = evtCreateMotionSeTask(modelKey, eventId, resourceId);
    if (motionSeTask != 0) {
        dds3LinkProcess(evtFindTaskById(scrReadIntParameter(0)), motionSeTask);
    }
    /* The original success path ends in this void call, without a return. */
    scrSetIntegerReturnValue(modelKey);
}

u32 evtOpUseSourceVectorForWorldObject(void) {
    u32 targetKey;
    u32 sourceKey;

    targetKey = scrReadIntParameter(0);
    sourceKey = scrReadIntParameter(1);
    mdlAttachWorldObjectToSourceVector(targetKey, sourceKey);
    return 1;
}

u32 evtScriptSetWorldUnitFlagMask(void) {
    void *ctx;
    s32 worldObjectKey;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    worldObjectKey = scrReadIntParameter(0);
    unit = dds3FindWorldObjectNodeByKey(ctx, worldObjectKey, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3ObjectBaseOnFlag(unit, 0x400);
    dds3ObjectBaseOffFlag(unit, 0x200);
    return 1;
}

u32 evtScriptClearWorldUnitFlagMask(void) {
    void *ctx;
    s32 worldObjectKey;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    worldObjectKey = scrReadIntParameter(0);
    unit = dds3FindWorldObjectNodeByKey(ctx, worldObjectKey, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3ObjectBaseOffFlag(unit, 0x400);
    dds3ObjectBaseOnFlag(unit, 0x200);
    return 1;
}

u32 evtOpClearWorldObjectStateFlags(void) {
    void *ctx;
    s32 worldObjectKey;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    worldObjectKey = scrReadIntParameter(0);
    unit = dds3FindWorldObjectNodeByKey(ctx, worldObjectKey, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3ObjectBaseOffFlag(unit, 0x400);
    dds3ObjectBaseOffFlag(unit, 0x200);
    return 1;
}

u32 evtOpQueueWorldObjectPendingValue(void) {
    void *ctx;
    s32 worldObjectKey;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    worldObjectKey = scrReadIntParameter(0);
    unit = dds3FindWorldObjectNodeByKey(ctx, worldObjectKey, 5);
    if (unit == NULL) {
        return ret;
    }
    evtArmEffectObjectPendingValue(unit, scrReadIntParameter(1));
    return 1;
}

u32 evtOpClearWorldObjectPendingValue(void) {
    void *ctx;
    s32 worldObjectKey;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    worldObjectKey = scrReadIntParameter(0);
    unit = dds3FindWorldObjectNodeByKey(ctx, worldObjectKey, 5);
    if (unit == NULL) {
        return ret;
    }
    evtResetObjectPendingValue(unit);
    return 1;
}

u32 evtOpModelLodChg(void) {
    s32 lod;
    void *world;
    EffWorldNode *unit;
    EffectObjectData *data;
    ObjBase *modelHolder;
    MdlCtx *context;
    SdfModel *model;
    s32 max;

    lod = scrReadIntParameter(1);
    sdfDebugPrintf("call: MODEL_LOD_CHG(int,int)\n");
    world = dds3GetWorldObject();
    unit = dds3FindWorldObjectNodeByKey(world, scrReadIntParameter(0), 5);
    if (unit == NULL) {
        sdfDebugPrintf("warning!! MODEL_LOD_CHG(int,int) unit pointer null\n");
        return 1;
    }
    data = unit->data;
    if (data->modelHolder == NULL) {
        sdfDebugPrintf("warning!! MODEL_LOD_CHG(int,int) workbase pointer null\n");
        return 1;
    }
    modelHolder = data->modelHolder;
    if (modelHolder->resourceHandle == 0) {
        sdfDebugPrintf("warning!! MODEL_LOD_CHG(int,int) mh pointer null\n");
        return 1;
    }
    context = (MdlCtx *)modelHolder->resourceHandle;
    model = context->inner;
    if (model == NULL) {
        sdfDebugPrintf("warning!! MODEL_LOD_CHG(int,int) root pointer null\n");
        return 1;
    }
    max = (s32)sdfGetLodChunkValue(model);
    if (max < lod) {
        sdfDebugPrintf("warning!! MODEL_LOD_CHG(int,int) lodno over!! max=%d setval=%d\n", max, lod);
        return 1;
    }
    model->lodIndex = lod;
    sdfDebugPrintf("success: MODEL_LOD_CHG(int,int)\n");
    return 1;
}

u32 evtSetWorldUnitPosition(void) {
    f32 vector[4];
    void *world;
    s32 worldObjectKey;
    void *unit;

    memset(vector, 0, 0x10);
    world = dds3GetWorldObject();
    worldObjectKey = scrReadIntParameter(0);
    unit = dds3FindWorldObjectNodeByKey(world, worldObjectKey, 5);
    if (unit == NULL) {
        return 1;
    }
    vector[0] = bfWaitReadArgFloat(1);
    vector[1] = bfWaitReadArgFloat(2);
    vector[2] = bfWaitReadArgFloat(3);
    dds3SetPosPoseSet(unit, (u128 *)vector);
    return 1;
}

u32 evtOpSetWorldUnitRotationFromAngles(void) {
    f32 v[4];
    void *ctx;
    s32 worldObjectKey;
    void *unit;
    f32 toRad;
    f32 x;
    f32 y;

    memset(v, 0, 0x10);
    v[3] = 1.0f;
    ctx = dds3GetWorldObject();
    worldObjectKey = scrReadIntParameter(0);
    unit = dds3FindWorldObjectNodeByKey(ctx, worldObjectKey, 5);
    if (unit == NULL) {
        return 1;
    }
    toRad = 0.017453293f;
    x = bfWaitReadArgFloat(1) * toRad;
    y = bfWaitReadArgFloat(2) * toRad;
    sdfConvertEulerAnglesToQuaternionVU(x, y, 0.0f);
    VU0_MOVE_VF(vf11, vf10);
    sdfConvertEulerAnglesToQuaternionVU(0.0f, 0.0f, bfWaitReadArgFloat(3) * toRad);
    effMiscQuatMultiplyVU();
    VU0_STORE_VF($vf10, v);
    dds3SetRotPoseSet(unit, (u128 *)v);
    return 1;
}

u32 evtSetWorldUnitScale(void) {
    f32 vector[4];
    void *world;
    s32 worldObjectKey;
    void *unit;

    memset(vector, 0, 0x10);
    vector[3] = 1.0f;
    world = dds3GetWorldObject();
    worldObjectKey = scrReadIntParameter(0);
    unit = dds3FindWorldObjectNodeByKey(world, worldObjectKey, 5);
    if (unit == NULL) {
        return 1;
    }
    vector[0] = bfWaitReadArgFloat(1);
    vector[1] = bfWaitReadArgFloat(2);
    vector[2] = bfWaitReadArgFloat(3);
    dds3SetSclPoseSet(unit, (u128 *)vector);
    return 1;
}

extern char D_00421810[];

u32 evtOpDeprecatedModelMotion(void) {
    EvtUnit *unit;

    unit = evtGetWorldUnitNestedValue(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    evtPrepareUnitMotionState(unit, scrReadIntParameter(1), scrReadIntParameter(2), scrReadIntParameter(3), 0);
    sdfDebugPrintf(D_00421810);
    return 1;
}

u32 evtOpSetUnitParams5(void) {
    EvtUnit *unit;

    unit = evtGetWorldUnitNestedValue(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 motionId = scrReadIntParameter(1);
        s32 startFrame = scrReadIntParameter(2);
        s32 endFrame = scrReadIntParameter(3);
        s32 mode = scrReadIntParameter(4);
        evtPrepareUnitMotionState(unit, motionId, startFrame, endFrame, mode);
    }
    return 1;
}

u32 evtOpSetUnitParams6(void) {
    EvtUnit *unit;

    unit = evtGetWorldUnitNestedValue(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 scriptParam1 = scrReadIntParameter(1);
        s32 scriptParam2 = scrReadIntParameter(2);
        s32 scriptParam3 = scrReadIntParameter(3);
        s32 scriptParam4 = scrReadIntParameter(4);
        s32 scriptParam5 = scrReadIntParameter(5);
        evtConfigureUnitMotionSlot(unit, scriptParam1, scriptParam2, scriptParam3, scriptParam4, scriptParam5);
    }
    return 1;
}

void evtInvokeStoredWindowMotion(void) {
    evtActivateStoredUnitMotionSlot(evtWindowMotionUnit);
}

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_00421810);

void evtLipsExecFunction(s32 unitUniqueId, s32 motionId) {
    void *unit = NULL;
    MdlCtx *model = NULL;
    EffWorldNode *node;

    if (unitUniqueId == 0) {
        return;
    }
    for (node = ((EvtWorldTable *)((EffWorldNode *)dds3GetWorldObject())->data)->slots[EVT_WORLD_SLOT_UNIT].head; node != NULL; node = node->next) {
        model = ((EvtLipsLink *)node->data)->mh->model;
        if (sdfGetUniqueChunkValue(model->inner) == unitUniqueId) {
            unit = ((EvtLipsLink *)node->data)->unit;
            break;
        }
    }
    if (unit == NULL) {
        sdfDebugPrintf("warning: call evtLipsExecFunction() but not find now reegisted unit same UnitUniqID\n");
        return;
    }
    if (motionId >= mdlGetNodeRefHalf(model, 2)) {
        sdfDebugPrintf("warning: call evtLipsExecFunction() but over have motionno fpr user specified motion no.\n");
        return;
    }
    evtConfigureUnitMotionSlot(unit, 2, motionId, 0, 5, 1);
    D_004371F0 = unitUniqueId;
    sdfDebugPrintf("<lips %d %d> \n", unitUniqueId, motionId);
}

void evtLipsStopFunction(void) {
    void *unit = NULL;
    MdlCtx *model = NULL;
    EffWorldNode *node;

    if (D_004371F0 == 0) {
        return;
    }
    for (node = ((EvtWorldTable *)((EffWorldNode *)dds3GetWorldObject())->data)->slots[EVT_WORLD_SLOT_UNIT].head; node != NULL; node = node->next) {
        model = ((EvtLipsLink *)node->data)->mh->model;
        if (sdfGetUniqueChunkValue(model->inner) == D_004371F0) {
            unit = ((EvtLipsLink *)node->data)->unit;
            break;
        }
    }
    if (unit == NULL) {
        sdfDebugPrintf("warning: call evtLipsStopFunction() but not find now reegisted unit same UnitUniqID\n");
        return;
    }
    if (mdlGetNodeRefHalf(model, 2) == 0) {
        sdfDebugPrintf("warning: call evtLipsStopFunction() but over have motionno fpr user specified motion no.\n");
        return;
    }
    evtConfigureUnitMotionSlot(unit, 2, 0, 0, 3, 2);
    sdfDebugPrintf("<lips_stop> stopunitid = %d\n", D_004371F0);
    D_004371F0 = 0;
}

u32 evtOpBeginWindowCallback(void) {
    EvtUnit *unit;

    unit = evtGetWorldUnitNestedValue(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 firstSlot = scrReadIntParameter(1);
        s32 secondSlot = scrReadIntParameter(2);
        evtStoreUnitMotionSlotSelection(unit, firstSlot, secondSlot);
        evtWindowMotionUnit = unit;
    }
    {
        s32 window = scrGetWindow();
        if (window < 0) {
            return 1;
        }
        itfMesSetWindowCallbackAddress(window, evtInvokeStoredWindowMotion);
    }
    return 1;
}

u32 evtOpActivateUnitMotionOnWindowEvent(void) {
    EvtUnit *unit;

    unit = evtGetWorldUnitNestedValue(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 firstSlot = scrReadIntParameter(1);
        s32 secondSlot = scrReadIntParameter(2);
        evtStoreUnitMotionSlotSelection(unit, firstSlot, secondSlot);
        evtActivateStoredUnitMotionSlot(unit);
        evtWindowMotionUnit = unit;
    }
    {
        s32 window = scrGetWindow();
        if (window < 0) {
            return 1;
        }
        itfMesSetWindowCallbackAddress(window, evtInvokeStoredWindowMotion);
    }
    return 1;
}

u32 evtOpIsUnitMotionIdleOrTimed(void) {
    s32 worldObjectKey;
    EvtUnit *unit;
    u32 ret = 1;

    worldObjectKey = scrReadIntParameter(0);
    unit = evtGetWorldUnitNestedValue(worldObjectKey);
    if (unit == NULL) {
        return ret;
    }
    return evtIsUnitMotionIdleOrTimedMode(unit) != 0;
}

u32 evtOpTestUnitMotionNodeFlag(void) {
    s32 worldObjectKey;
    EvtUnit *unit;
    s32 motionNodeIndex;
    u32 ret = 1;

    worldObjectKey = scrReadIntParameter(0);
    unit = evtGetWorldUnitNestedValue(worldObjectKey);
    if (unit == NULL) {
        return ret;
    }
    motionNodeIndex = scrReadIntParameter(1);
    if (((unit->slotFlags[motionNodeIndex] & 1) & 0xFF) == 0) {
        return ret;
    }
    return mdlGetNodeMotionTerminalStatus(unit->owner, scrReadIntParameter(1)) != 0;
}

u32 evtCmdDestroySelectedWorldUnit(void) {
    void *ctx;
    s32 worldObjectKey;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    worldObjectKey = scrReadIntParameter(0);
    unit = dds3FindWorldObjectNodeByKey(ctx, worldObjectKey, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3RemoveWorldObjectNode(unit);
    return 1;
}

u32 evtOpStartUnitTransitionTowardWorldObject(void) {
    EvtUnit *unit;

    unit = evtGetWorldUnitNestedValue(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    unit->motionTicks = 0;
    {
        s32 motionSubmode = scrReadIntParameter(2);
        s32 worldObjectKey = scrReadIntParameter(1);
        s32 frames = scrReadIntParameter(3);
        s32 valueB6 = scrReadIntParameter(4);
        evtBeginUnitTransitionTowardWorldObject(unit, motionSubmode, worldObjectKey, -1, frames, valueB6, 0, 0);
    }
    if (scrReadIntParameter(2) == 1) {
        evtUnitPrepareVerticalMoveSteps(unit);
    }
    return 1;
}

u32 evtOpStartUnitPathFollow(void) {
    EvtUnit *unit;

    unit = evtGetWorldUnitNestedValue(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    unit->motionTicks = 0;
    {
        s32 pathObjectKey = scrReadIntParameter(1);
        s32 frames = scrReadIntParameter(5);
        s32 valueB6 = scrReadIntParameter(6);
        s32 pathFollowMode = scrReadIntParameter(2);
        s32 dirFlag = scrReadIntParameter(4);
        s32 sideMode = scrReadIntParameter(3);
        evtSetUnitPathFollow(unit, pathObjectKey, frames, valueB6, pathFollowMode, dirFlag, sideMode);
    }
    return 1;
}

u32 evtScriptSetUnitTableValueAndReset(void) {
    EvtUnit *unit;

    unit = evtGetWorldUnitNestedValue(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 tableValueIndex = scrReadIntParameter(1);
        s32 value = scrReadIntParameter(2);
        unit->tableValues[tableValueIndex] = value;
        unit->unk108[tableValueIndex] = 0;
        unit->unk120[tableValueIndex] = 0;
        unit->unk138[tableValueIndex] = 1.0f;
    }
    return 1;
}

/* Store the script-supplied value for the selected event unit. */
u32 evtCommandSetUnitValue(void) {
    s32 worldObjectKey;
    EvtUnit *unit;

    worldObjectKey = scrReadIntParameter(0);
    unit = evtGetWorldUnitNestedValue(worldObjectKey);
    if (unit == NULL) {
        return 1;
    }
    unit->unkBC = scrReadIntParameter(1);
    return 1;
}

/* Store the two script-supplied world-unit state values. */
u32 evtCmdSetWorldUnitStatePair(void) {
    s32 worldObjectKey;
    EvtUnit *unit;

    worldObjectKey = scrReadIntParameter(0);
    unit = evtGetWorldUnitNestedValue(worldObjectKey);
    if (unit == NULL) {
        return 1;
    }
    unit->unkBE = scrReadIntParameter(1);
    unit->unkC0 = scrReadIntParameter(2);
    return 1;
}

u32 evtCmdPrepareUnitMotionAndLogState(void) {
    s32 worldObjectKey;
    EvtUnit *unit;

    worldObjectKey = scrReadIntParameter(0);
    unit = evtGetWorldUnitNestedValue(worldObjectKey);
    if (unit == NULL) {
        return 1;
    }
    evtPrepareUnitMotionState(unit, scrReadIntParameter(1), 5, 7, 1);
    sdfDebugPrintf("\xA2\xA8\xC3\xED\xB0\xD5:MODEL_MOVE_MOT()\xA4\xB3\xA4\xCE\xA5\xB3\xA5\xDE\xA5\xF3\xA5\xC9"
                  "\xA4\xCF\xBA\xA3\xB8\xE5\xBB\xC8\xCD\xD1\xB6\xD8\xBB\xDF\xA4\xCB\xA4\xCA\xA4\xEA\xA4\xDE\xA4\xB9\xA1\xA3\n");
    return 1;
}

u32 evtOpAttachUnitToWorldObject(void) {
    s32 unitObjectKey;
    EvtUnit *unit;
    s32 worldObjectKey;
    s32 frames;

    unitObjectKey = scrReadIntParameter(0);
    unit = evtGetWorldUnitNestedValue(unitObjectKey);
    if (unit == NULL) {
        return 1;
    }
    unit->motionTicks = 0;
    worldObjectKey = scrReadIntParameter(1);
    frames = scrReadIntParameter(2);
    evtAttachSecondaryWorldUnit(unit, worldObjectKey, frames);
    return 1;
}

f32 evtGetShortestAngleDelta(f32 a, f32 b) {
    f32 diff;

    if (a < 0.0f || b < 0.0f) {
        a += 360.0f;
        b += 360.0f;
    }
    a = (s32)a % 360;
    b = (s32)b % 360;
    diff = a - b;
    if (diff > 180.0f || diff < -180.0f) {
        if (a < b) {
            a += 360.0f;
        } else {
            b += 360.0f;
        }
    }
    return b - a;
}


extern f32 effMiscComputeQuaternionRotatedReferenceAngle(void);

u32 evtOpTurnUnitRelativeToWorldNode(void) {
    EvtUnit *unit;
    EffWorldNode *actor;
    EffWorldNode *source;
    EvtWorldSourceTransformPrefix *sourceTransform;
    f32 rotation[4] __attribute__((aligned(16)));
    f32 actorPosition[4] __attribute__((aligned(16)));
    f32 sourcePosition[4] __attribute__((aligned(16)));
    f32 referenceAngle;
    f32 targetAngle;
    f32 angleDelta;
    s32 frames;

    unit = evtGetWorldUnitNestedValue(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    unit->motionTicks = 0;

    actor = (EffWorldNode *)dds3FindWorldObjectNodeByKey(
        dds3GetWorldObject(), scrReadIntParameter(0), 5);
    if (actor == NULL) {
        return 1;
    }
    source = (EffWorldNode *)dds3FindWorldObjectNodeByKey(
        dds3GetWorldObject(), scrReadIntParameter(1), 0x11);
    if (source == NULL) {
        return 1;
    }

    sourceTransform = source->data;
    dds3GetPosPoseSet(actor);
    VU0_STORE_VF(vf10, actorPosition);
    PCP_COPY_VECTOR(sourcePosition, &sourceTransform->position);

    if ((unit->referenceAngleCacheFlags & EVT_UNIT_REFERENCE_ANGLE_CACHE_VALID) == 0) {
        dds3GetRotPoseSet(actor);
        referenceAngle = effMiscComputeQuaternionRotatedReferenceAngle();
        unit->referenceAngleCacheFlags |= EVT_UNIT_REFERENCE_ANGLE_CACHE_VALID;
        unit->cachedReferenceAngleDegrees = -(referenceAngle * 57.29577637f);
    }

    targetAngle = sdfAtan2(actorPosition[0] - sourcePosition[0],
                           actorPosition[2] - sourcePosition[2]) * 57.32484055f;
    angleDelta = evtGetShortestAngleDelta(unit->cachedReferenceAngleDegrees, targetAngle);
    if (angleDelta < -135.0f) {
        targetAngle -= angleDelta + 135.0f;
        angleDelta = -135.0f;
    } else if (angleDelta > 135.0f) {
        targetAngle -= angleDelta - 135.0f;
        angleDelta = 135.0f;
    }
    targetAngle -= (angleDelta + angleDelta) / 3.0f;

    sdfConvertEulerAnglesToQuaternionVU(0.0f, targetAngle * 0.017453293f, 0.0f);
    /* First write to this output vector; the SDK store touches only it. */
    VU0_STORE_VF_UNCLOBBERED(vf10, rotation);

    frames = scrReadIntParameter(2);
    if (frames >= 101) {
        frames = 100;
    }
    angleDelta = evtGetShortestAngleDelta(unit->cachedReferenceAngleDegrees, targetAngle);
    if (angleDelta < 0.0f) {
        angleDelta = -angleDelta;
    }
    if (angleDelta > 90.0f) {
        angleDelta = 90.0f;
    }
    frames = (frames * (s32)angleDelta) / 90;
    if (frames <= 0) {
        frames = 1;
    }
    if (frames > 100) {
        frames = 100;
    }

    evtBeginVectorTransition(unit, (s128 *)rotation, frames);
    evtArmEffectObjectPendingValue(actor, source->key);
    return 1;
}

u32 evtOpRestoreUnitReferenceAngle(void) {
    EvtUnit *unit;
    EffWorldNode *actor;
    f32 rotation[4] __attribute__((aligned(16)));
    f32 referenceAngle;
    f32 angleDelta;
    s32 frames;

    unit = evtGetWorldUnitNestedValue(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    unit->motionTicks = 0;
    if ((unit->referenceAngleCacheFlags & EVT_UNIT_REFERENCE_ANGLE_CACHE_VALID) == 0) {
        return 1;
    }
    actor = dds3FindWorldObjectNodeByKey(
        dds3GetWorldObject(), scrReadIntParameter(0), 5);
    if (actor == NULL) {
        return 1;
    }
    dds3GetRotPoseSet(actor);
    referenceAngle = -(effMiscComputeQuaternionRotatedReferenceAngle() * 57.29577637f);
    sdfConvertEulerAnglesToQuaternionVU(0.0f, unit->cachedReferenceAngleDegrees * 0.017453293f, 0.0f);
    /* This is the first write to the vector supplied to the transition. */
    VU0_STORE_VF_UNCLOBBERED(vf10, rotation);
    frames = scrReadIntParameter(1);
    if (frames >= 101) {
        frames = 100;
    }
    angleDelta = evtGetShortestAngleDelta(referenceAngle, unit->cachedReferenceAngleDegrees);
    if (angleDelta < 0.0f) {
        angleDelta = -angleDelta;
    }
    if (angleDelta > 90.0f) {
        angleDelta = 90.0f;
    }
    frames = (frames * (s32)angleDelta) / 90;
    frames = (frames * 40) / 100;
    if (frames <= 0) {
        frames = 1;
    }
    if (frames > 100) {
        frames = 100;
    }
    evtBeginVectorTransition(unit, (s128 *)rotation, frames);
    evtResetObjectPendingValue(actor);
    return 1;
}

u32 evtUnitClearFlagBit(void) {
    s32 worldObjectKey;
    EvtUnit *unit;

    worldObjectKey = scrReadIntParameter(0);
    unit = evtGetWorldUnitNestedValue(worldObjectKey);
    if (unit != NULL) {
        unit->owner->flags &= ~MDL_SKIP_TRANSFORMS;
    }
    return 1;
}

u32 evtUnitSetFlagBit(void) {
    s32 worldObjectKey;
    EvtUnit *unit;

    worldObjectKey = scrReadIntParameter(0);
    unit = evtGetWorldUnitNestedValue(worldObjectKey);
    if (unit != NULL) {
        unit->owner->flags |= MDL_SKIP_TRANSFORMS;
    }
    return 1;
}

u32 evtCommandFlagSelectedUnitStatus(void) {
    s32 worldObjectKey;
    EvtUnit *unit;

    worldObjectKey = scrReadIntParameter(0);
    unit = evtGetWorldUnitNestedValue(worldObjectKey);
    evtSetUnitStatusFlags(unit);
    return 1;
}

u32 evtCommandBeginSelectedUnitTransition(void) {
    s32 worldObjectKey;
    EvtUnit *unit;
    s32 duration;

    worldObjectKey = scrReadIntParameter(0);
    unit = evtGetWorldUnitNestedValue(worldObjectKey);
    duration = scrReadIntParameter(1);
    evtConfigureUnitTransition(unit, duration);
    return 1;
}

u8 evtUnitHasNoStatusFlags(void) {
    s32 worldObjectKey;
    EvtUnit *unit;
    u8 active;

    worldObjectKey = scrReadIntParameter(0);
    unit = evtGetWorldUnitNestedValue(worldObjectKey);
    active = evtTestUnitStatusFlags(unit);
    return active == 0;
}

u32 evtOpSetUnitGradientColors(void) {
    EvtUnit *unit;
    s32 color1[4];
    s32 color2[4];
    u32 packed1;
    u32 packed2;
    u32 scale;

    unit = evtGetWorldUnitNestedValue(scrReadIntParameter(0));
    VU0_SCALAR_OP(bfWaitReadArgFloat(2), "vaddx.x vf10, vf0, vf2x");
    VU0_SCALAR_OP(bfWaitReadArgFloat(3), "vaddx.y vf10, vf0, vf2x");
    VU0_SCALAR_OP(bfWaitReadArgFloat(4), "vaddx.z vf10, vf0, vf2x");
    VU0_CLEAR_W(vf10);
    scale = 0x43000000;
    EE_MMI_RGBA_PACK_UNIT(packed1, scale);
    color1[0] = packed1;
    VU0_SCALAR_OP(bfWaitReadArgFloat(5), "vaddx.x vf10, vf0, vf2x");
    VU0_SCALAR_OP(bfWaitReadArgFloat(6), "vaddx.y vf10, vf0, vf2x");
    VU0_SCALAR_OP(bfWaitReadArgFloat(7), "vaddx.z vf10, vf0, vf2x");
    VU0_SET_W_ONE(vf10);
    EE_MMI_RGBA_PACK_UNIT(packed2, scale);
    color2[0] = packed2;
    evtInitializeUnitColorTransition(unit, scrReadIntParameter(1), packed1, packed2);
    return 1;
}

const char D_00421A30[] = "WARNING: MODEL_LIGHT_DIR length zero\n";

u32 evtOpSetModelLightDirection(void) {
    f32 direction[4];
    EvtUnit *unit;

    memset(direction, 0, sizeof(direction));
    unit = evtGetWorldUnitNestedValue(scrReadIntParameter(0));
    direction[0] = bfWaitReadArgFloat(2);
    direction[1] = bfWaitReadArgFloat(3);
    direction[2] = bfWaitReadArgFloat(4);
    if (ffabsf(direction[0]) <= 0.01f && ffabsf(direction[1]) <= 0.01f &&
        ffabsf(direction[2]) <= 0.01f) {
        evtPrintDeveloperConsoleMessage(D_00421A30);
        return 1;
    }
    VU0_LOAD_VF(vf10, direction);
    evtSetUnitNormalizedDirection(unit, scrReadIntParameter(1));
    return 1;
}


u32 evtOpSetUnitPackedRgbColor(void) {
    EvtUnit *unit;
    s32 color[4];
    u32 packed;

    unit = evtGetWorldUnitNestedValue(scrReadIntParameter(0));
    VU0_SCALAR_OP(bfWaitReadArgFloat(2), "vaddx.x vf10, vf0, vf2x");
    VU0_SCALAR_OP(bfWaitReadArgFloat(3), "vaddx.y vf10, vf0, vf2x");
    VU0_SET_AXIS_CLEAR_W(bfWaitReadArgFloat(4), z);
    EE_MMI_RGBA_PACK_F128(packed);
    color[0] = packed;
    evtSetUnitRgbTransition(unit, scrReadIntParameter(1), packed);
    return 1;
}

u32 evtOpSetUnitPackedAlpha(void) {
    EvtUnit *unit;
    s32 color[4];
    u32 packed;

    unit = evtGetWorldUnitNestedValue(scrReadIntParameter(0));
    VU0_MOVE_VF(vf10, vf0);
    VU0_SCALAR_OP(bfWaitReadArgFloat(2), "vmulx.w vf10, vf0, vf2x");
    EE_MMI_RGBA_PACK_F128(packed);
    color[0] = packed;
    evtSetUnitAlphaTransition(unit, scrReadIntParameter(1), packed);
    return 1;
}

u32 evtOpSetUnitValueTransitionTarget(void) {
    s32 worldObjectKey;
    EvtUnit *unit;
    EffWorldNode *target;

    worldObjectKey = scrReadIntParameter(0);
    unit = evtGetWorldUnitNestedValue(worldObjectKey);
    target = evtFindWorldObjectByIdAndKind(9, scrReadIntParameter(2));
    if (target == NULL) {
        return 1;
    }
    evtSetUnitValueTransition(unit, target, scrReadIntParameter(1));
    return 1;
}

u32 evtOpEndUnitValueTransition(void) {
    s32 worldObjectKey;
    EvtUnit *unit;
    s32 duration;

    worldObjectKey = scrReadIntParameter(0);
    unit = evtGetWorldUnitNestedValue(worldObjectKey);
    duration = scrReadIntParameter(1);
    evtEndUnitValueTransition(unit, duration);
    return 1;
}

const char D_00421A58[] = "EFFECT_BE not fount RID = %d\n";

u32 evtOpSpawnBoundEffectFromTaskResource(void) {
    u8 firstVector[16];
    u8 secondVector[16];
    s32 taskId;
    s32 key;
    struct EffNodeDescriptor *descriptor;
    EffWorldNode *object;

    memset(firstVector, 0, sizeof(firstVector));
    memset(secondVector, 0, sizeof(secondVector));
    taskId = scrReadIntParameter(0);
    key = scrReadIntParameter(1);
    descriptor = evtFindTaskResourceEntryByKey(taskId, key);
    if (descriptor == NULL) {
        key = scrReadIntParameter(1);
        evtPrintDeveloperConsoleMessage(D_00421A58, key);
        scrSetIntegerReturnValue(0);
        return 1;
    }
    object = effObjSpawnDescriptorBoundEffect(descriptor, firstVector, secondVector);
    if (object == NULL) {
        sdfDebugPrintf(D_00421A78, 1);
        scrSetIntegerReturnValue(0);
    } else {
        effObjSetFlags(object, 1);
        scrSetIntegerReturnValue(object->key);
    }
    return 1;
}

u32 evtOpResolveAndFlagObjectFromName(void) {
    u8 buf1[16];
    u8 buf2[16];
    char *resourceName;
    EvtUnit *unit;

    memset(buf1, 0, 0x10);
    memset(buf2, 0, 0x10);
    resourceName = scrReadStringParameter(0);
    unit = effObjSpawnLoadedResourceEffect((const char *)resourceName, buf1, buf2);
    if (unit == NULL) {
        sdfDebugPrintf(D_00421A90, 1);
        sdfDebugPrintf(D_00421AC0, scrReadStringParameter(0));
        scrSetIntegerReturnValue(0);
    } else {
        effObjSetFlags(unit, 1);
        scrSetIntegerReturnValue(unit->objectId);
    }
    return 1;
}

u32 evtOpCreateAndFlagObjectFromResourceName(void) {
    f32 buf1[4];
    f32 buf2[4];
    char *param0;
    EvtUnit *unit;

    memset(buf1, 0, 0x10);
    memset(buf2, 0, 0x10);
    buf2[3] = 1.0f;
    param0 = scrReadStringParameter(0);
    unit = effObjCreateFromResolvedResource(param0, buf1, buf2);
    if (unit == NULL) {
        sdfDebugPrintf(D_00421A90, 1);
        sdfDebugPrintf(D_00421AC0, scrReadStringParameter(0));
        scrSetIntegerReturnValue(0);
    } else {
        effObjSetFlags(unit, 1);
        scrSetIntegerReturnValue(unit->objectId);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_00421A78);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_00421A90);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_00421AC0);

const char D_00421AD8[] = "EFFBED_BE not fount RID = %d\n";

u32 evtOpCreateEffectNodeFromTaskResource(void) {
    u8 position[16];
    u8 scale[16];
    s32 taskId;
    s32 key;
    struct EffNodeDescriptor *descriptor;
    EffWorldNode *object;

    memset(position, 0, sizeof(position));
    memset(scale, 0, sizeof(scale));
    taskId = scrReadIntParameter(0);
    key = scrReadIntParameter(1);
    descriptor = evtFindTaskResourceEntryByKey(taskId, key);
    if (descriptor == NULL) {
        key = scrReadIntParameter(1);
        evtPrintDeveloperConsoleMessage(D_00421AD8, key);
        scrSetIntegerReturnValue(0);
        return 1;
    }
    object = (EffWorldNode *)effObjCreateEventNode(descriptor, position, scale);
    if (object == NULL) {
        scrSetIntegerReturnValue(0);
    } else {
        effObjSetFlags(object, 1);
        scrSetIntegerReturnValue(object->key);
    }
    return 1;
}

u32 evtOpBuiltin359NoOp(void) {
    return 1;
}

u32 evtUnitCheckModelCut(void) {
    s32 worldObjectKey;
    void *unit;
    u32 entryId;

    worldObjectKey = scrReadIntParameter(0);
    unit = evtFindWorldObjectByIdAndKind(7, worldObjectKey);
    if (unit != NULL) {
        entryId = scrReadIntParameter(1);
        if (entryId >= 3U) {
            evtPrintDeveloperConsoleMessage(D_00421AF8, entryId);
            return 1;
        } else {
            effObjReplaceActiveEventNode(unit, entryId);
        }
    }
    return 1;
}

f32 evtComputeClampedModelScale(s32 modelRecordIndex) {
    f32 scale = (D_00435DF0[modelRecordIndex].unk18 + D_00435DF0[modelRecordIndex].unk1C * 0.5f) * 0.5f * D_00435DF0[modelRecordIndex].scaleFactor * (1.0f / 70.0f);

    if (scale > 2.0f) {
        scale = 2.0f;
    } else if (scale < 0.8f) {
        scale = 0.8f;
    }
    return scale;
}

u32 evtOpSetModelCutAndScale(void) {
    EffWorldNode *unit;

    unit = evtFindWorldObjectByIdAndKind(7, scrReadIntParameter(0));
    if (unit != NULL) {
        s32 entryId = scrReadIntParameter(1);
        s32 modelRecordIndex;
        if ((u32)entryId >= 3) {
            evtPrintDeveloperConsoleMessage(D_00421AF8, entryId);
            return 1;
        }
        effObjReplaceActiveEventNode(unit, entryId);
        modelRecordIndex = scrReadIntParameter(2);
        if (modelRecordIndex >= 0) {
            f32 value = evtComputeClampedModelScale(modelRecordIndex);
            void *target = ((EvtModelHeader *)unit->data)->target2C;
            if (target != NULL) {
                effEventSetScale(target, value);
            }
        }
    }
    return 1;
}

u32 evtCmdCreateEffectObjectFromResource(void) {
    char *param0;
    EvtUnit *unit;

    param0 = scrReadStringParameter(0);
    unit = effObjCreateKindFromResource(1, param0);
    if (unit == NULL) {
        sdfDebugPrintf(D_00421B20, 1);
        sdfDebugPrintf(D_00421AC0, scrReadStringParameter(0));
        scrSetIntegerReturnValue(0);
    } else {
        effObjSetFlags(unit, 1);
        scrSetIntegerReturnValue(unit->objectId);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_00421AF8);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_00421B20);

const char D_00421B50[] = "EFFMG1_BE not fount RID = %d\n";

u32 evtOpCreateMagatuhiEffect1FromTaskResource(void) {
    s32 taskId;
    s32 key;
    struct EffNodeDescriptor *descriptor;
    EffWorldNode *object;

    taskId = scrReadIntParameter(0);
    key = scrReadIntParameter(1);
    descriptor = evtFindTaskResourceEntryByKey(taskId, key);
    if (descriptor == NULL) {
        key = scrReadIntParameter(1);
        evtPrintDeveloperConsoleMessage(D_00421B50, key);
        scrSetIntegerReturnValue(0);
        return 1;
    }
    object = effForwardMagatuhiDescriptor(1, descriptor);
    if (object == NULL) {
        scrSetIntegerReturnValue(0);
    } else {
        effObjSetFlags(object, 1);
        scrSetIntegerReturnValue(object->key);
    }
    return 1;
}

u32 evtScriptCreateEffectObjectFromResource(void) {
    char *param0;
    EvtUnit *unit;

    param0 = scrReadStringParameter(0);
    unit = effObjCreateKindFromResource(2, param0);
    if (unit == NULL) {
        sdfDebugPrintf(D_00421B70, 1);
        sdfDebugPrintf(D_00421AC0, scrReadStringParameter(0));
        scrSetIntegerReturnValue(0);
    } else {
        effObjSetFlags(unit, 1);
        scrSetIntegerReturnValue(unit->objectId);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_00421B70);

const char D_00421BA0[] = "EFFMG2_BE not fount RID = %d\n";

u32 evtOpCreateMagatuhiEffect2FromTaskResource(void) {
    s32 taskId;
    s32 key;
    struct EffNodeDescriptor *descriptor;
    EffWorldNode *object;

    taskId = scrReadIntParameter(0);
    key = scrReadIntParameter(1);
    descriptor = evtFindTaskResourceEntryByKey(taskId, key);
    if (descriptor == NULL) {
        key = scrReadIntParameter(1);
        evtPrintDeveloperConsoleMessage(D_00421BA0, key);
        scrSetIntegerReturnValue(0);
        return 1;
    }
    object = effForwardMagatuhiDescriptor(2, descriptor);
    if (object == NULL) {
        scrSetIntegerReturnValue(0);
    } else {
        effObjSetFlags(object, 1);
        scrSetIntegerReturnValue(object->key);
    }
    return 1;
}

u32 evtOpRunWorldEffectReadyAction(void) {
    s32 worldObjectKey;
    void *obj;

    worldObjectKey = scrReadIntParameter(0);
    obj = evtFindWorldObjectByIdAndKind(7, worldObjectKey);
    if (obj != NULL) {
        effObjDispatchReadyState((EffWorldNode *)obj);
    }
    return 1;
}

u32 evtOpRunWorldEffectReadyActionDuplicate(void) {
    s32 worldObjectKey;
    void *obj;

    worldObjectKey = scrReadIntParameter(0);
    obj = evtFindWorldObjectByIdAndKind(7, worldObjectKey);
    if (obj != NULL) {
        effObjDispatchReadyState((EffWorldNode *)obj);
    }
    return 1;
}

u32 evtScriptDestroyWorldEffectObject(void) {
    s32 worldObjectKey;
    void *obj;

    worldObjectKey = scrReadIntParameter(0);
    obj = evtFindWorldObjectByIdAndKind(7, worldObjectKey);
    if (obj != NULL) {
        dds3RemoveWorldObjectNode(obj);
    }
    return 1;
}

u32 evtOpSetModelObjectPosition(void) {
    EffWorldNode *obj;
    EvtModelHeader *header;

    obj = evtFindWorldObjectByIdAndKind(7, scrReadIntParameter(0));
    header = (EvtModelHeader *)obj->data;
    if (header->flags & 4) {
        header->positionX = bfWaitReadArgFloat(1);
        header->positionY = bfWaitReadArgFloat(2);
        header->positionZ = bfWaitReadArgFloat(3);
    } else {
        obj->inner->position[0] = bfWaitReadArgFloat(1);
        obj->inner->position[1] = bfWaitReadArgFloat(2);
        obj->inner->position[2] = bfWaitReadArgFloat(3);
        obj->inner->flags = (obj->inner->flags | OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) & ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
    }
    return 1;
}

u32 evtOpSetModelObjectRotationFromAngles(void) {
    f32 v[4];
    EffWorldNode *obj;
    f32 toRad;
    f32 x;
    f32 y;

    memset(v, 0, 0x10);
    v[3] = 1.0f;
    obj = evtFindWorldObjectByIdAndKind(7, scrReadIntParameter(0));
    if (!(((EvtModelHeader *)obj->data)->flags & 4)) {
        toRad = 0.017453293f;
        x = bfWaitReadArgFloat(1) * toRad;
        y = bfWaitReadArgFloat(2) * toRad;
        sdfConvertEulerAnglesToQuaternionVU(x, y, 0.0f);
        VU0_MOVE_VF(vf11, vf10);
        sdfConvertEulerAnglesToQuaternionVU(0.0f, 0.0f, bfWaitReadArgFloat(3) * toRad);
        effMiscQuatMultiplyVU();
        VU0_STORE_VF($vf10, v);
        dds3SetRotPoseSet(obj, (u128 *)v);
    }
    return 1;
}

u32 evtOpCopyModelTransformFromSource(void) {
    EffWorldNode *obj;
    EffWorldNode *source;
    EvtWorldSourceTransformPrefix *vec;
    ObjectTransform *params;

    obj = evtFindWorldObjectByIdAndKind(7, scrReadIntParameter(0));
    if (obj == NULL) {
        return 1;
    }
    source = evtFindWorldObjectByIdAndKind(0x11, scrReadIntParameter(1));
    if (source == NULL) {
        return 1;
    }
    vec = source->data;
    if (!(((EvtModelHeader *)obj->data)->flags & 4)) {
        params = obj->inner;
        params->position[0] = vec->position.x;
        params->position[1] = vec->position.y;
        params->position[2] = vec->position.z;
        params->rotation[0] = vec->rotation[0];
        params->rotation[1] = vec->rotation[1];
        params->rotation[2] = vec->rotation[2];
        params->rotation[3] = vec->rotation[3];
    }
    obj->inner->flags = (obj->inner->flags | OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) & ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
    return 1;
}

INCLUDE_SDATA(const s32, "event/evtUnitManager", evtWindowMotionUnit);

INCLUDE_SDATA(const s32, "event/evtUnitManager", D_004371F0);

