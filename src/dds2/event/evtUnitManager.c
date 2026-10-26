#include "sdk_ps2.h"
struct SdfPoolNode;
struct EffNodeDescriptor;

#include "common.h"
#include "dds3_object_flags.h"
#include "sdf_vu0.h"
#include "btl_model_record.h"
#include "sdf_model_scalars.h"
#include "sdf_motion.h"
#include "sdf_chip.h"
#include "dds3_path.h"
#include "evt_unit.h"
#include "evt_unit_motion_callback_result.h"
#include "evt_unit_motion_step_result.h"
#include "evt_unit_move_end_api.h"
#include "evt_unit_path_advance_api.h"
#include "evt_unit_wait_api.h"
#include "evt_unit_facing_result_api.h"
#include "evt_unit_light_animation_status.h"
#include "evt_unit_vector_slot.h"
#include "evt_unit_motion_slot_flags.h"
#include "evt_unit_transition_flags.h"
#include "evt_unit_motion_setup.h"
#include "sdf_float_counter.h"
#include "evt_unit_motion_slot.h"
#include "evt_unit_color_transition.h"
#include "evt_unit_vertical_move.h"
#include "eff_transform.h"
#include "eff_object.h"
#include "dds3obj.h"
#include "mdl.h"
#include "sdf_draw.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "sdf_vu_matrix.h"
#include "eff_quaternion_nlerp.h"


extern f32 *D_0037F770[];
extern u8 kwlnDefaultColorVector[];

extern void evtGetUnitDirVector(evtUnitHandle_t *unit);

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
f32 GetSplineLength(dds3PathBasicControl_t *path) {
    f32 savedScaleFactor;
    f32 length = 0.0f;
    f32 step = 0.05f;
    f32 t = step;
    f32 segmentLength;

    savedScaleFactor = dds3PathBasicControlGetRatio(&path->counter);
    dds3PathBasicControlSetRatio(&path->counter, 0.0f);
    dds3PathBasicControlGetPos(path);
    do {
        SDF_VU0_COPYVECTOR_A_TO_B();
        dds3PathBasicControlSetRatio(&path->counter, t);
        dds3PathBasicControlGetPos(path);
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_SUBVECTOR();
        SDF_VU0_GETLENGTH(segmentLength);
        length += segmentLength;
        SDF_VU0_COPYVECTOR_C_TO_A();
        t += step;
    } while (t <= 1.0f);
    dds3PathBasicControlSetRatio(&path->counter, savedScaleFactor);
    return length;
}

extern f32 sdfFaceDivSoften, sdfFaceDivBias;
#include "dds3_world_primary.h"
#include "dds3_scene_membership.h"
extern s32 sdfModelGetMarkPos(sdfModelRootNode_t *model, s32 mapRecordId);
extern void dds3RotationArc(void);


/* Advance the model, light-colour and directional transitions for one event unit. */
void evtAdvanceUnitVisualTransitions(evtUnitHandle_t *unit) {
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
    auxFirst = sdfFaceDivSoften;
    auxSecond = sdfFaceDivBias;
    previousWeight = 0.0f;
    /* The model RGB and alpha tracks preserve the other packed channel. */
    flags = unit->flags;
    if (flags & EVT_UNIT_FLAG_RGB_TRANSITION) {
        u32 packed;
        factor = unit->rgbDuration ? (f32)unit->rgbElapsed / unit->rgbDuration : 1.0f;
        rgbTarget[0] = unit->color64;
        EE_MMI_RGBA_UNPACK_READONLY(rgbTarget, 0.0078125f);
        SDF_VU0_COPYVECTOR_A_TO_B();
        rgbSource[0] = unit->color60;
        EE_MMI_RGBA_UNPACK_READONLY(rgbSource, 0.0078125f);
        VU0_SCALE_VF(vf10, 1.0f - factor);
        VU0_SCALE_VF(vf11, factor);
        SDF_VU0_ADDVECTOR();
        oldColor = mdlGetColor((dds3ModelHandle_t *)unit->owner);
        EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
        rgbResult[0] = packed;
        mdlSetColor((dds3ModelHandle_t *)unit->owner, (oldColor & 0xFF000000) | (rgbResult[0] & 0xFFFFFF));
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
        SDF_VU0_COPYVECTOR_A_TO_B();
        alphaSource[0] = unit->color60;
        EE_MMI_RGBA_UNPACK_READONLY(alphaSource, 0.0078125f);
        VU0_SCALE_VF(vf10, 1.0f - factor);
        VU0_SCALE_VF(vf11, factor);
        SDF_VU0_ADDVECTOR();
        oldColor = mdlGetColor((dds3ModelHandle_t *)unit->owner);
        EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
        alphaResult[0] = packed;
        mdlSetColor((dds3ModelHandle_t *)unit->owner, (oldColor & 0xFFFFFF) | (alphaResult[0] & 0xFF000000));
        if (unit->alphaElapsed >= unit->alphaDuration) {
            flags = unit->flags &= ~EVT_UNIT_FLAG_ALPHA_TRANSITION;
        } else {
            flags = unit->flags;
            unit->alphaElapsed++;
        }
    }
    /* Stale world-node references must not reach the target-vector reads. */
    if (flags & EVT_UNIT_FLAG_TARGET_TRANSITION) {
        if (unit->currentTransitionTarget == 0) {
            unit->flags = flags & ~EVT_UNIT_FLAG_TARGET_TRANSITION;
        } else if (dds3SceneBasicExistBasicFromType(dds3GetActiveScene(), 9,
                                                   unit->currentTransitionTarget) == DDS3_SCENE_MEMBERSHIP_ABSENT) {
            unit->currentTransitionTarget = 0;
            unit->previousTransitionTarget = 0;
            unit->flags &= ~EVT_UNIT_FLAG_TARGET_TRANSITION;
        }
    }
    if (unit->previousTransitionTarget &&
        dds3SceneBasicExistBasicFromType(dds3GetActiveScene(), 9,
                                         unit->previousTransitionTarget) == DDS3_SCENE_MEMBERSHIP_ABSENT) {
        unit->previousTransitionTarget = 0;
    }
    transition = 0.0f;
    target = 0;
    previous = 0;
    ownVector = 0;
    currentWeight = 0.0f;
    if (unit->currentTransitionTarget) {
        flags = unit->flags;
        if (flags & EVT_UNIT_FLAG_TARGET_TRANSITION) {
            f32 distance;
            target = unit->currentTransitionTarget->data;
            if (unit->previousTransitionTarget) {
                previous = unit->previousTransitionTarget->data;
            }
            if (!sdfModelGetMarkPos(unit->owner->inner, 0)) {
                mdlGetPosition(unit->owner);
            }
            SDF_VU0_STOREVECTOR_NOMEM(position);
            SDF_VU0_LOADVECTOR_B(unit->currentTransitionTarget->inner->position);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_STOREVECTOR_NOMEM(targetDirection);
            SDF_VU0_GETLENGTH(distance);
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
                SDF_VU0_LOADVECTOR(position);
                SDF_VU0_LOADVECTOR_B(unit->previousTransitionTarget->inner->position);
                SDF_VU0_SUBVECTOR();
                SDF_VU0_STOREVECTOR_NOMEM(previousDirection);
                SDF_VU0_GETLENGTH(previousDistance);
                if (previousDistance <= previous->nearDistance) {
                    previousWeight = 1.0f;
                } else if (previousDistance >= previous->farDistance) {
                    previousWeight = 0.0f;
                } else {
                    previousWeight = 1.0f - (previousDistance - previous->nearDistance) /
                        (previous->farDistance - previous->nearDistance);
                }
            }
            SDF_VU0_LOADVECTOR(targetDirection);
            if (target->flags & EVT_TARGET_INFO_FLAG_UNIT_OWNS_VECTOR) {
                SDF_VU0_STOREVECTOR_NOMEM(target->direction);
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
                                unit->currentTransitionTarget = 0;
                                unit->flags &= ~EVT_UNIT_FLAG_TARGET_TRANSITION;
                            }
                            unit->previousTransitionTarget = 0;
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
    if (flags & EVT_UNIT_FLAG_DIRECTION_TRANSITION) {
        if (unit->directionFramesRemaining) {
            factor = 1.0f / unit->directionFramesRemaining;
        } else {
            factor = 1.0f;
        }
        SDF_VU0_LOADVECTOR(unit->vec20);
        if (flags & EVT_UNIT_FLAG_USE_STORED_DIRECTION_SOURCE) {
            SDF_VU0_LOADVECTOR_B(unit->vec30);
        } else if (ownVector) {
            SDF_VU0_LOADVECTOR_B(target->direction);
        } else {
            SDF_VU0_LOADVECTOR_B(D_0037F770[0] + 4);
        }
        dds3RotationArc();
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(identity);
        Lerp(factor);
        effMiscQuaternionToMatrixVU();
        SDF_VU0_LOADVECTOR(unit->vec20);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR_NOMEM(unit->vec40);
        SDF_VU0_STOREVECTOR_NOMEM(unit->vec20);
        if (unit->directionFramesRemaining == 0) {
            flags = unit->flags;
            if (flags & EVT_UNIT_FLAG_DISABLE_DIRECTION_HOLD_ON_TRANSITION_END) {
                unit->flags &= ~EVT_UNIT_FLAG_HOLD_TARGET_DIRECTION;
                flags = unit->flags;
            }
            unit->flags = flags & ~EVT_UNIT_FLAG_DIRECTION_TRANSITION;
        } else {
            unit->directionFramesRemaining--;
        }
    } else if (!(flags & EVT_UNIT_FLAG_HOLD_TARGET_DIRECTION)) {
        if (ownVector) {
            SDF_VU0_LOADVECTOR(target->direction);
        } else {
            SDF_VU0_LOADVECTOR(D_0037F770[0] + 4);
        }
        SDF_VU0_STOREVECTOR_NOMEM(unit->vec40);
    }
    if (target && !ownVector) {
        factor = transition;
        SDF_VU0_LOADVECTOR(unit->vec40);
        SDF_VU0_LOADVECTOR_B(targetDirection);
        dds3RotationArc();
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(identity);
        Lerp(factor);
        effMiscQuaternionToMatrixVU();
        SDF_VU0_LOADVECTOR(unit->vec40);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR_NOMEM(blendedDirection);
        if (previous) {
            factor = 1.0f;
            SDF_VU0_LOADVECTOR(unit->vec40);
            SDF_VU0_LOADVECTOR_B(previousDirection);
            dds3RotationArc();
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_LOADVECTOR(identity);
            Lerp(factor);
            effMiscQuaternionToMatrixVU();
            SDF_VU0_LOADVECTOR(unit->vec40);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_STOREVECTOR_NOMEM(oldDirection);
            SDF_VU0_LOADVECTOR_B(blendedDirection);
            dds3RotationArc();
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_LOADVECTOR(identity);
            Lerp(transition);
            effMiscQuaternionToMatrixVU();
            SDF_VU0_LOADVECTOR(oldDirection);
            SDF_VU0_APPLYMATRIX();
        }
        SDF_VU0_STOREVECTOR_NOMEM(unit->vec10);
    } else {
        SDF_VU0_COPYVECTOR(unit->vec10, unit->vec40);
    }
    /* Advance the two local colour tracks before applying target influence. */
    originalFlags = unit->flags;
    flags = originalFlags;
    if (originalFlags & EVT_UNIT_FLAG_COLOR_TRANSITION) {
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
        SDF_VU0_COPYVECTOR_A_TO_B();
        if (unit->flags & EVT_UNIT_FLAG_USE_LOCAL_COLOR_TARGETS) {
            firstTarget[0] = unit->color08;
            EE_MMI_RGBA_UNPACK_READONLY(firstTarget, 0.0078125f);
        } else if (ownVector) {
            SDF_VU0_LOADVECTOR(target);
        } else {
            SDF_VU0_LOADVECTOR(D_0037F770[0]);
        }
        VU0_SCALE_VF(vf10, factor);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_VECTOR_W_SET_ZERO();
        EE_MMI_RGBA_PACK_UNIT(firstPacked, 128.0f);
        firstResult[0] = firstPacked;
        unit->firstCurrent = firstResult[0];
        unit->color0C = unit->firstCurrent;
        secondSource[0] = unit->color54;
        EE_MMI_RGBA_UNPACK_READONLY(secondSource, 0.0078125f);
        VU0_SCALE_VF(vf10, 1.0f - factor);
        SDF_VU0_COPYVECTOR_A_TO_B();
        if (unit->flags & EVT_UNIT_FLAG_USE_LOCAL_COLOR_TARGETS) {
            secondTarget[0] = unit->color58;
            EE_MMI_RGBA_UNPACK_READONLY(secondTarget, 0.0078125f);
        } else if (ownVector) {
            SDF_VU0_LOADVECTOR(target->secondColor);
        } else {
            SDF_VU0_LOADVECTOR(kwlnDefaultColorVector);
        }
        VU0_SCALE_VF(vf10, factor);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_VECTOR_W_SET_ONE();
        EE_MMI_RGBA_PACK_UNIT(secondPacked, 128.0f);
        secondResult[0] = secondPacked;
        unit->color54 = secondResult[0];
        unit->color5C = unit->color54;
        if (unit->colorFramesRemaining == 0) {
            if (originalFlags & EVT_UNIT_FLAG_DISABLE_UNIT_COLORS_ON_TRANSITION_END) {
                flags = unit->flags = originalFlags & ~EVT_UNIT_FLAG_USE_UNIT_COLOR_CHANNELS;
            }
            unit->flags = flags & ~EVT_UNIT_FLAG_COLOR_TRANSITION;
        } else {
            unit->colorFramesRemaining--;
        }
    } else {
        u32 firstPacked;
        u32 secondPacked;
        if (!(originalFlags & EVT_UNIT_FLAG_USE_UNIT_FIRST_COLOR)) {
            if (ownVector) {
                SDF_VU0_LOADVECTOR(target);
            } else {
                SDF_VU0_LOADVECTOR(D_0037F770[0]);
            }
            EE_MMI_RGBA_PACK_UNIT(firstPacked, 128.0f);
            defaultFirst[0] = firstPacked;
            unit->color0C = defaultFirst[0];
        }
        if (!(originalFlags & EVT_UNIT_FLAG_USE_UNIT_SECOND_COLOR)) {
            if (ownVector) {
                SDF_VU0_LOADVECTOR(target->secondColor);
            } else {
                SDF_VU0_LOADVECTOR(kwlnDefaultColorVector);
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
        SDF_VU0_LOADVECTOR_B(target);
        VU0_SCALE_VF(vf11, factor);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR_NOMEM(blendedDirection);
        if (previous) {
            factor = previousWeight;
            previousFirst[0] = unit->color0C;
            EE_MMI_RGBA_UNPACK_READONLY(previousFirst, 0.0078125f);
            VU0_SCALE_VF(vf10, 1.0f - factor);
            SDF_VU0_LOADVECTOR_B(previous);
            VU0_SCALE_VF(vf11, factor);
            SDF_VU0_ADDVECTOR();
            VU0_SCALE_VF(vf10, 1.0f - transition);
            SDF_VU0_LOADVECTOR_B(blendedDirection);
            VU0_SCALE_VF(vf11, transition);
            SDF_VU0_ADDVECTOR();
        }
        SDF_VU0_VECTOR_W_SET_ZERO();
        EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
        finalFirst[0] = packed;
        unit->color = finalFirst[0];
        factor = weightedTransition;
        currentSecond[0] = unit->color5C;
        EE_MMI_RGBA_UNPACK_READONLY(currentSecond, 0.0078125f);
        VU0_SCALE_VF(vf10, 1.0f - factor);
        SDF_VU0_LOADVECTOR_B(target->secondColor);
        VU0_SCALE_VF(vf11, factor);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR_NOMEM(blendedDirection);
        if (previous) {
            factor = previousWeight;
            previousSecond[0] = unit->color5C;
            EE_MMI_RGBA_UNPACK_READONLY(previousSecond, 0.0078125f);
            VU0_SCALE_VF(vf10, 1.0f - factor);
            SDF_VU0_LOADVECTOR_B(previous->secondColor);
            VU0_SCALE_VF(vf11, factor);
            SDF_VU0_ADDVECTOR();
            VU0_SCALE_VF(vf10, 1.0f - transition);
            SDF_VU0_LOADVECTOR_B(blendedDirection);
            VU0_SCALE_VF(vf11, transition);
            SDF_VU0_ADDVECTOR();
        }
        SDF_VU0_VECTOR_W_SET_ZERO();
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
            auxFirst = sdfFaceDivSoften * (1.0f - factor) + auxFirst * factor;
            auxSecond = sdfFaceDivBias * (1.0f - factor) + auxSecond * factor;
        }
        if (previous) {
            f32 oldFirst = previous->auxFirst;
            f32 oldSecond = previous->auxSecond;
            if (oldFirst >= 1.0f) {
                factor = previousWeight;
                oldFirst = sdfFaceDivSoften * (1.0f - factor) + oldFirst * factor;
                oldSecond = sdfFaceDivBias * (1.0f - factor) + oldSecond * factor;
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
        sdfModelSetFaceDivParm(unit->owner->inner, auxFirst, auxSecond);
    } else {
        sdfModelResetFaceDivParm(unit->owner->inner);
    }
}


/* vu0 routine: load vf10 with the unit's colour (flag 0x100), its target's vector, or the default */
void GetBaseLightRGB(evtUnitHandle_t *unit) {
    EvtTargetInfo *info = 0;
    s32 ownVector = 0;
    s32 color;
    f32 scale;

    if (unit->currentTransitionTarget != 0 && (unit->flags & EVT_UNIT_FLAG_TARGET_TRANSITION)) {
        info = unit->currentTransitionTarget->data;
        if (info->flags & EVT_TARGET_INFO_FLAG_UNIT_OWNS_VECTOR) {
            ownVector = 1;
        }
    }
    if (unit->flags & EVT_UNIT_FLAG_USE_UNIT_FIRST_COLOR) {
        scale = 0.0078125f;
        color = unit->color;
        EE_MMI_RGBA_UNPACK(&color, scale);
    } else if (ownVector) {
        SDF_VU0_LOADVECTOR(info);
    } else {
        SDF_VU0_LOADVECTOR(D_0037F770[0]);
    }
}

/* vu0 routine: load the target info secondColor vector when unit-owned. */
void GetBaseAmbientRGB(evtUnitHandle_t *unit) {
    EvtTargetInfo *info = 0;
    s32 ownVector = 0;
    s32 color;
    f32 scale;

    if (unit->currentTransitionTarget != 0 && (unit->flags & EVT_UNIT_FLAG_TARGET_TRANSITION)) {
        info = unit->currentTransitionTarget->data;
        if (info->flags & EVT_TARGET_INFO_FLAG_UNIT_OWNS_VECTOR) {
            ownVector = 1;
        }
    }
    if (unit->flags & EVT_UNIT_FLAG_USE_UNIT_SECOND_COLOR) {
        scale = 0.0078125f;
        color = unit->color50;
        EE_MMI_RGBA_UNPACK(&color, scale);
    } else if (ownVector) {
        SDF_VU0_LOADVECTOR(info->secondColor);
    } else {
        SDF_VU0_LOADVECTOR(kwlnDefaultColorVector);
    }
}

/* vu0 routine: load vf10 with the unit's vector, or the default one */
void GetBaseLightDir(evtUnitHandle_t *unit) {
    s32 ownVector = 0;

    if (unit->currentTransitionTarget != 0 && (unit->flags & EVT_UNIT_FLAG_TARGET_TRANSITION)) {
        if (((EvtTargetInfo *)unit->currentTransitionTarget->data)->flags & EVT_TARGET_INFO_FLAG_UNIT_OWNS_VECTOR) {
            ownVector = 1;
        }
    }
    if (unit->flags & EVT_UNIT_FLAG_HOLD_TARGET_DIRECTION) {
        SDF_VU0_LOADVECTOR(unit->vec10);
    } else if (ownVector) {
        SDF_VU0_LOADVECTOR(unit->vec10);
    } else {
        SDF_VU0_LOADVECTOR(D_0037F770[0] + 4);
    }
}


extern void evtApplyMatchingUnitSlotEndpoints(evtUnitHandle_t *unit);

/* Refresh endpoint render work after the value-change flag is cleared; defer
 * to the matching-slot selector when no target or color transition is active. */
void evtRefreshUnitEndpointWork(evtUnitHandle_t *unit) {
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
    SDF_VU0_STOREVECTOR(ends[0]);
    SDF_VU0_LOADVECTOR(unit->vec10);
    SDF_VU0_STOREVECTOR(ends[1]);
    packed1[0] = unit->color50;
    scale1 = 0.0078125f;
    EE_MMI_RGBA_UNPACK(packed1, scale1);
    SDF_VU0_STOREVECTOR(color);
    color[3] = 1.0f;
    for (colorChannelIndex = 0; colorChannelIndex < 3; colorChannelIndex++) {
        if (color[colorChannelIndex] > 1.0f) {
            color[colorChannelIndex] = 1.0f;
        }
    }
    sdf3DMakeLightParallelPacket(unit->endpointWork, desc, color);
    unit->value = (u32)unit->endpointWork;
}

void evtSetUnitValueTransition(evtUnitHandle_t *unit, dds3Basic_t *target, s32 duration) {
    unit->flags |= EVT_UNIT_FLAG_TARGET_TRANSITION;
    unit->previousTransitionTarget = unit->currentTransitionTarget;
    unit->currentTransitionTarget = target;
    if (duration == 0) {
        unit->previousTransitionTarget = 0;
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

void evtEndUnitValueTransition(evtUnitHandle_t *unit, s32 duration) {
    if (unit->currentTransitionTarget != 0) {
        if (duration == 0) {
            unit->flags &= ~EVT_UNIT_FLAG_TARGET_TRANSITION;
            unit->flags &= ~EVT_UNIT_FLAG_TARGET_BLEND_PHASES;
            unit->currentTransitionTarget = 0;
        } else {
            unit->flags &= ~EVT_UNIT_FLAG_TARGET_BLEND_IN;
            unit->transitionDuration = duration;
            unit->flags |= EVT_UNIT_FLAG_TARGET_BLEND_OUT;
            unit->transitionElapsed = 0;
        }
    }
}

void evtUnitSetValueAndFlag(evtUnitHandle_t *unit, u32 value) {
    unit->value = value;
    unit->flags = unit->flags | EVT_UNIT_FLAG_VALUE_CHANGED;
}

void evtUnlinkExternalLightPacket(evtUnitHandle_t *unit) {
    unit->flags = unit->flags & ~EVT_UNIT_FLAG_VALUE_CHANGED;
    evtRefreshUnitEndpointWork(unit);
}

void evtSetUnitLightAndAmbient(evtUnitHandle_t *unit, s32 duration, u32 firstColor, u32 secondColor) {
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
    GetBaseLightRGB(unit);
    EE_MMI_RGBA_PACK_UNIT(firstPackedColor, packedScale.scaleBits);
    firstColorVector[0] = firstPackedColor;
    unit->firstCurrent = firstPackedColor;
    GetBaseAmbientRGB(unit);
    EE_MMI_RGBA_PACK_UNIT(secondPackedColor, packedScale.scaleBits);
    secondColorVector[0] = secondPackedColor;
    unit->color54 = secondPackedColor;
    unit->flags = (unit->flags | EVT_UNIT_FLAG_USE_LOCAL_COLOR_TARGETS) &
                  ~EVT_UNIT_FLAG_DISABLE_UNIT_COLORS_ON_TRANSITION_END;
}

/* vu0 routine: normalize the direction in vf10, store it to vec30, then vf10 from GetBaseLightDir to vec20 */
void evtSetUnitLightDir(evtUnitHandle_t *unit, s32 directionMode) {
    unit->directionMode = directionMode;
    SDF_VU0_VECTOR_W_SET_ZERO();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(unit->vec30);
    GetBaseLightDir(unit);
    SDF_VU0_STOREVECTOR_NOMEM(unit->vec20);
    unit->flags = (unit->flags | EVT_UNIT_FLAG_HOLD_TARGET_DIRECTION |
                   EVT_UNIT_FLAG_USE_STORED_DIRECTION_SOURCE) &
                  ~EVT_UNIT_FLAG_DISABLE_DIRECTION_HOLD_ON_TRANSITION_END;
}



void evtSetUnitColorRGB(evtUnitHandle_t *unit, s32 duration, u32 color) {
    unit->rgbDuration = duration;
    unit->rgbElapsed = 0;
    if (duration == 0) {
        mdlSetColor((dds3ModelHandle_t *)unit->owner,
            (mdlGetColor((dds3ModelHandle_t *)unit->owner) & 0xFF000000) | (color & 0xFFFFFF));
        unit->color60 = (unit->color60 & 0xFF000000) | (color & 0xFFFFFF);
        unit->flags &= ~EVT_UNIT_FLAG_RGB_TRANSITION;
    } else {
        u32 currentRgb = mdlGetColor((dds3ModelHandle_t *)unit->owner) & 0xFFFFFF;

        unit->color60 = (unit->color60 & 0xFF000000) | currentRgb;
        unit->color64 = (unit->color64 & 0xFF000000) | (color & 0xFFFFFF);
        unit->flags |= EVT_UNIT_FLAG_RGB_TRANSITION;
    }
}

void evtSetUnitColorA(evtUnitHandle_t *unit, s32 duration, u32 color) {
    unit->alphaDuration = duration;
    unit->alphaElapsed = 0;
    if (duration == 0) {
        mdlSetColor((dds3ModelHandle_t *)unit->owner,
            (mdlGetColor((dds3ModelHandle_t *)unit->owner) & 0xFFFFFF) | (color & 0xFF000000));
        unit->color60 = (unit->color60 & 0xFFFFFF) | (color & 0xFF000000);
        unit->flags &= ~EVT_UNIT_FLAG_ALPHA_TRANSITION;
    } else {
        u32 currentAlpha = mdlGetColor((dds3ModelHandle_t *)unit->owner) & 0xFF000000;

        unit->color60 = (unit->color60 & 0xFFFFFF) | currentAlpha;
        unit->color64 = (unit->color64 & 0xFFFFFF) | (color & 0xFF000000);
        unit->flags |= EVT_UNIT_FLAG_ALPHA_TRANSITION;
    }
}

EvtUnitLightAnimationStatus evtUnitLightAnimeStat(evtUnitHandle_t *unit) {
    return (unit->flags & 0x7800) != 0 ? EVT_UNIT_LIGHT_ANIMATION_ACTIVE
                                      : EVT_UNIT_LIGHT_ANIMATION_INACTIVE;
}

void evtUnitLightOn(evtUnitHandle_t *unit) {
    unit->flags = unit->flags | EVT_UNIT_FLAG_USE_UNIT_COLOR_CHANNELS;
}

void evtUnitLightOff(evtUnitHandle_t *unit, s32 duration) {
    if (duration == 0) {
        unit->flags &= ~0x700;
        unit->flags &= ~0x5000;
        unit->flags &= ~0x2800;
    } else {
        evtSetUnitLightAndAmbient(unit, duration, 0, 0);
        unit->flags = (unit->flags & ~EVT_UNIT_FLAG_USE_LOCAL_COLOR_TARGETS) |
                      EVT_UNIT_FLAG_DISABLE_UNIT_COLORS_ON_TRANSITION_END;
        SDF_VU0_LOADIDENTITYVECTOR();
        evtSetUnitLightDir(unit, duration);
        unit->flags = (unit->flags & ~EVT_UNIT_FLAG_USE_STORED_DIRECTION_SOURCE) |
                      EVT_UNIT_FLAG_DISABLE_DIRECTION_HOLD_ON_TRANSITION_END;
    }
}

evtUnitHandle_t *evtGetUnitHandle(s32 worldObjectKey) {
    dds3Basic_t *obj = dds3SearchSceneBasic(dds3GetActiveScene(), worldObjectKey, EFF_WORLD_KIND_FOLLOW_MODEL);

    if (obj != NULL) {
        return ((EffectObjectData *)obj->data)->transitionWork;
    }
    return NULL;
}

evtUnitHandle_t *evtGetUnitHandle2(dds3Basic_t *unit) {
    if (unit == NULL) {
        return NULL;
    }
    return ((EffectObjectData *)unit->data)->transitionWork;
}

extern f32 D_004215D0[];


evtUnitHandle_t *evtCreateUnitTransitionWork(dds3Basic_t *effObj, dds3ModelHandle_t *owner) {
    evtUnitHandle_t *work;
    struct SdfLightingPacketStorage *endpoint;
    f32 defaultVector[4];
    s32 i;

    memcpy(defaultVector, D_004215D0, sizeof(defaultVector));
    work = sdfChipAllocDirect(sizeof(evtUnitHandle_t));
    memset(work, 0, sizeof(evtUnitHandle_t));
    work->motionState = EVT_UNIT_MOTION_STATE_IDLE;
    work->transitionSourceKind = EVT_UNIT_TRANSITION_SOURCE_WORLD_NODE;
    work->transitionMotionScale = 1.0f;
    work->effObj = effObj;
    work->owner = owner;
    work->motionBlendDurationFrames = 10;
    EVT_UNIT_MOTION_SETUP(work)->endFrame = 10;
    work->motionBlendLeadFrames = 0;
    EVT_UNIT_MOTION_SETUP(work)->startFrame = 0;

    SDF_VU0_LOADVECTOR(defaultVector);
    SDF_VU0_STOREVECTOR_NOMEM(work->vec10);
    SDF_VU0_STOREVECTOR_NOMEM(work->vec40);
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
        f32 *slot = &work->tableMotionFrameStep[1];

        i = 10;
        do {
            i--;
            *slot = 1.0f;
            slot++;
        } while (i >= 0);
    }
    return work;
}

s32 evtDeleteUnitHandle(evtUnitHandle_t *transitionWork) {
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
    if (transitionWork->pathControl != NULL) {
        dds3DeletePathControl(transitionWork->pathControl);
        transitionWork->pathControl = NULL;
    }
    sdfChipFreeDirect(transitionWork);
    return 1;
}

EvtUnitMotionState evtGetUnitStat(evtUnitHandle_t *unit) {
    return unit->motionState;
}

/* Callers pass the promoted full-width value; only the low halfword is stored. */
void evtSetWaitMotion(evtUnitHandle_t *unit, s32 storedParameter) {
    unit->pendingMotionIndex = storedParameter;
}

void evtSetWaitMotionSpeed(evtUnitHandle_t *unit, f32 transitionMotionScale) {
    unit->transitionMotionScale = transitionMotionScale;
}

void evtSetWaitHokanFrame(evtUnitHandle_t *unit, s32 firstParameter, s32 secondParameter) {
    unit->motionBlendLeadFrames = firstParameter;
    unit->motionBlendDurationFrames = secondParameter;
}

EvtUnitWaitResult evtCheckUnitWait(evtUnitHandle_t *unit) {
    EvtUnitMotionState state = evtGetUnitStat(unit);

    if (state == EVT_UNIT_MOTION_STATE_IDLE) {
        return EVT_UNIT_WAIT_COMPLETE;
    }
    if (state == EVT_UNIT_MOTION_STATE_MOTION && unit->motionTicks > 0 &&
        unit->owner->first->state == SDF_MOTION_STATE_TERMINAL) {
        return EVT_UNIT_WAIT_COMPLETE;
    }
    return EVT_UNIT_WAIT_IN_PROGRESS;
}

void evtSetUnitKutiMotion(evtUnitHandle_t *unit, s32 firstSlot, s32 secondSlot) {
    unit->firstSlot = firstSlot;
    unit->secondSlot = secondSlot;
}

void evtActivateStoredUnitMotionSlot(evtUnitHandle_t *unit) {
    evtConfigureUnitMotionSlot(unit, unit->firstSlot, unit->secondSlot, 0, 0, 2);
}

void evtConfigureUnitMotionSlot(evtUnitHandle_t *unit, s32 slot, s32 slotAValue, s32 slotBValue,
                                s32 slotCValue, EvtUnitMotionSlotMode mode) {
    unit->slotFlags[slot] = EVT_UNIT_MOTION_SLOT_ENABLED |
                            EVT_UNIT_MOTION_SLOT_START_PENDING;
    unit->slotA[slot] = slotAValue;
    unit->slotB[slot] = slotBValue;
    unit->slotC[slot] = slotCValue;
    switch (mode) {
    case EVT_UNIT_MOTION_SLOT_MODE_LOOP_ON_MOTION_END:
        unit->slotFlags[slot] |= EVT_UNIT_MOTION_SLOT_LOOP_ON_MOTION_END;
        break;
    case EVT_UNIT_MOTION_SLOT_MODE_LOOP_EXTERNAL_MOTION:
        unit->slotFlags[slot] |= EVT_UNIT_MOTION_SLOT_LOOP_EXTERNAL_MOTION;
        break;
    case EVT_UNIT_MOTION_SLOT_MODE_STOP_ON_BATTLE_ACTION:
        unit->slotFlags[slot] |= EVT_UNIT_MOTION_SLOT_STOP_ON_BATTLE_ACTION;
        break;
    }
}

#define EVT_UNIT_FLAG_MOTION_START_PENDING 0x80
#define EVT_UNIT_FLAG_MOTION_WOBBLE 0x40

enum {
    EVT_UNIT_MOTION_MODE_RESTART_PENDING_MOTION = 0,
    EVT_UNIT_MOTION_MODE_LOOP = 1,
    EVT_UNIT_MOTION_MODE_SINGLE = 2,
    EVT_UNIT_MOTION_MODE_WOBBLE = 3,
};

void evtSetUnitMotion(evtUnitHandle_t *unit, s32 motionId, s32 startFrame, s32 endFrame, s32 mode) {
    unit->flags &= ~0x1;
    unit->flags &= ~0x20;
    unit->flags &= ~EVT_UNIT_FLAG_MOTION_WOBBLE;
    unit->flags &= ~0x400000;
    unit->flags &= ~0x800000;
    unit->motionState = EVT_UNIT_MOTION_STATE_MOTION;
    EVT_UNIT_MOTION_SETUP(unit)->motionId = motionId;
    EVT_UNIT_MOTION_SETUP(unit)->startFrame = startFrame;
    EVT_UNIT_MOTION_SETUP(unit)->endFrame = endFrame;
    unit->flags |= EVT_UNIT_FLAG_MOTION_START_PENDING;
    unit->motionTicks = 0;
    unit->motionCallback = 0;
    switch (mode) {
    case EVT_UNIT_MOTION_MODE_RESTART_PENDING_MOTION:
        unit->flags |= 0x20;
        break;
    case EVT_UNIT_MOTION_MODE_LOOP:
        unit->flags |= 0x1;
        break;
    case EVT_UNIT_MOTION_MODE_SINGLE:
        break;
    case EVT_UNIT_MOTION_MODE_WOBBLE:
        unit->flags |= EVT_UNIT_FLAG_MOTION_WOBBLE;
        break;
    }
}

extern const s32 D_004215E0[];
extern const s32 D_004215F0[];
extern void evtVu0SubVectorXZAxis(f32 *a, f32 *b);
extern void FromRotation(f32 angle);

EvtUnitFacingTargetResult evtRotateUnitTowardTarget(evtUnitHandle_t *unit, f32 *dir, f32 scale) {
    /* SDK stores end local CSE before the axis and quaternion addresses
     * are rebuilt. One angle carries the cosine through acos and clamping. */
    f32 a[4];
    f32 b[4];
    f32 q[4];
    f32 out[4];
    f32 dot1;
    f32 rotationAngle;
    EvtUnitFacingTargetResult result = EVT_UNIT_ALREADY_FACING_TARGET;

    memcpy(a, D_004215E0, sizeof(a));
    memcpy(b, D_004215F0, sizeof(b));
    evtGetUnitDirVector(unit);
    SDF_VU0_STOREVECTOR(a);
    SDF_VU0_COPYVECTOR_A_TO_C();
    evtVu0SubVectorXZAxis(dir, unit->effObj->inner->position);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(out);
    SDF_VU0_LOADVECTOR_B(b);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_C_TO_B();
    SDF_VU0_INNERPRODUCT(dot1);
    SDF_VU0_LOADVECTOR(out);
    SDF_VU0_INNERPRODUCT(rotationAngle);
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
    SDF_VU0_LOADVECTOR(b);
    FromRotation(rotationAngle);
    SDF_VU0_STOREVECTOR_NOMEM(q);
    dds3AddRotPoseSet(unit->effObj, (u128 *)q);
    return EVT_UNIT_ROTATION_STEP_APPLIED;
}



/* Flat, negated direction of the rotation in quat, offset by the effect object's point, aimed with evtRotateUnitTowardTarget. */
EvtUnitFacingTargetResult evtUnitRotationQuat(evtUnitHandle_t *unit, f32 *quat, f32 angle) {
    f32 v[4];

    SDF_VU0_LOADVECTOR(quat);
    effMiscQuaternionToMatrixVU();
    SDF_VU0_GET_ROW2();
    VU0_SET_VF10_Y_CLEAR_W(0.0f);
    SDF_VU0_NORMALIZE();
    VU0_SCALAR_OP(-1.0f, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_LOADVECTOR_B(unit->effObj->inner->position);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(v);
    return evtRotateUnitTowardTarget(unit, v, angle);
}

EvtUnitMotionStepResult evtUnitMoveType0(evtUnitHandle_t *unit) {
    f32 v[4];
    f32 scale;
    dds3Basic_t *obj;

    evtRotateUnitTowardTarget(unit, unit->targetVector, unit->directionOffset * 0.01f);
    if (unit->directionOffset != 0) {
        evtGetUnitDirVector(unit);
        obj = unit->effObj;
    } else {
        SDF_VU0_LOADVECTOR(unit->targetVector);
        obj = unit->effObj;
        SDF_VU0_LOADVECTOR_B(obj->inner->position);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_NORMALIZE();
    }
    scale = unit->motionParameter * 0.1f;
    VU0_SCALAR_OP(scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_STOREVECTOR(v);
    dds3GetPosPoseSetMCR(obj, (u128 *)v);
    return EVT_UNIT_MOTION_STEP_APPLIED;
}


#include "sdf_debug_api.h"

/* Run a copy of the unit until evtCheckMoveEnd reports done, and derive its per-step Y speed. */
INCLUDE_RODATA(const s32, "event/evtUnitManager", D_004215D0);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_004215E0);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_004215F0);

s32 evtUnitPrepareVerticalMoveSteps(evtUnitHandle_t *unit) {
    evtUnitHandle_t copy;
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
    while (evtCheckMoveEnd(&copy) == EVT_UNIT_MOVE_END_NOT_REACHED) {
        simulatedStepCount++;
        evtUnitMoveType0(&copy);
    }
    for (vectorComponentIndex = 0; vectorComponentIndex < 4; vectorComponentIndex++) {
        unit->effObj->inner->position[vectorComponentIndex] = savedA[vectorComponentIndex];
        unit->effObj->inner->rotation[vectorComponentIndex] = savedB[vectorComponentIndex];
    }
    if (simulatedStepCount == 0) {
        printf("ymove frameno = 0\n");
        unit->stepCount = 0;
        unit->speedY = 0;
        return 0;
    }
    SDF_VU0_LOADVECTOR(unit->targetVector);
    SDF_VU0_LOADVECTOR_B(unit->effObj->inner->position);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(delta);
    unit->stepCount = simulatedStepCount;
    unit->speedY = delta[1] / (f32)simulatedStepCount;
    printf("ymove frameno = %d addvalue = %f total yzahyo=%f\n", simulatedStepCount, unit->speedY, delta[1]);
    return simulatedStepCount;
}

EvtUnitMotionStepResult evtUnitMoveTypePathTrace(evtUnitHandle_t *unit) {
    f32 v[4];

    dds3PathBasicControlStep(&unit->pathControl->counter);
    dds3PathBasicControlGetPos(unit->pathControl);
    VU0_STORE_VF($vf10, v);
    dds3SetPosPoseSet(unit->effObj, (u128 *)v);
    if (unit->flags & 0x10) {
        dds3PathBasicControlGetRot(unit->pathControl);
        SDF_VU0_COPYVECTOR_A_TO_B();
        sdfConvertEulerAnglesToQuaternionVU(0.0f, 3.14159265f, 0.0f);
        effMiscQuatMultiplyVU();
        VU0_STORE_VF($vf10, v);
        dds3SetRotPoseSet(unit->effObj, (u128 *)v);
    }
    return EVT_UNIT_MOTION_STEP_APPLIED;
}

void evtCopyUnitTargetVector(evtUnitHandle_t *work, const f32 src[4]) {
    SDF_VU0_COPYVECTOR(work->targetVector, src);
}

#include "sdf_trig.h"
#include "scr_integer_return_value_api.h"
#include "sdf_unique_chunk_value_api.h"
#include "kwln_task_add_child.h"
#include "fpu.h"
#include "mdl_motion_api.h"
#include "mdl_motion_end_status.h"
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

extern evtUnitHandle_t *evtWindowMotionUnit;
extern s32 D_004371F0;

extern u32 sdfGetLodChunkValue(sdfModelRootNode_t *model);


extern EvtUnitVectorSlot evtUnitVectorSlots[10];



extern BtlActorStatusRecord *D_00435DF0;
extern void mnuInitializeCampPanelVisualDefaults(f32 *firstVector, f32 *secondVector, f32 *thirdVector, f32 *scalarA, f32 *scalarB);
#include "dds3_world_secondary.h"

extern void evtSetUnitMove1(evtUnitHandle_t *work, s32 motionSubmode, const f32 vector[4], s32 unused, s32 frames, s32 valueB6, EvtUnitMotionCallback callback, s32 unusedLast);




extern s32 scrGetIntPara(s32 idx);



extern void evtArmEffectObjectPendingValue(dds3Basic_t *object, s32 value);

extern void evtResetObjectPendingValue(dds3Basic_t *object);



/* World object views used by the model-parameter opcodes. */
extern evtUnitHandle_t *evtGetUnitHandle(s32 idx);





extern void evtSetUnitMotion(evtUnitHandle_t *unit, s32 motionId, s32 startFrame, s32 endFrame, s32 mode);


extern u8 D_00421AC0[];


extern void dds3EffectObjectBasicStatOn(dds3Basic_t *object, u32 flags);

extern dds3Basic_t *effObjSpawnDescriptorBoundEffect(struct EffNodeDescriptor *descriptor, void *firstVector, void *secondVector);
struct SoundMixer;
extern dds3Basic_t *dds3AddBEDEffectObjectBasic2(struct SoundMixer *resource, const void *position, const void *scale);
extern dds3Basic_t *dds3AddMG12EffectObjectBasic2(s32 kind, struct EffNodeDescriptor *descriptor);
extern char D_00421A78[];

extern dds3Basic_t *effObjSpawnLoadedResourceEffect(const char *resourceName, void *firstVector, void *secondVector);

extern u8 D_00421A90[];

extern dds3Basic_t *dds3AddBEDEffectObjectBasic(const char *resourcePath, void *firstVector, void *secondVector);

extern u8 D_00421B20[];

extern dds3Basic_t *dds3AddMG12EffectObjectBasic(s32 kind, const char *resourcePath);

extern u8 D_00421B70[];

extern s32 evtLoadUnit(s32 slotKind, s32 resource);

extern void evtUnitLightOff(evtUnitHandle_t *unit, s32 duration);

extern void evtEndUnitValueTransition(evtUnitHandle_t *unit, s32 duration);

extern void evtPrintDeveloperConsoleMessage(const char *fmt, ...);

extern void dds3StartBEDEffectObjectBasic(dds3Basic_t *effectObject, u32 entryId);

extern char D_00421AF8[];

extern s32 evtInitMotionSeProcess(s32 eventId, s32 resourceId);
extern s32 scrGetMessageHandle(void);


extern void evtSetUnitKutiMotion(evtUnitHandle_t *unit, s32 firstSlot, s32 secondSlot);


extern f32 evtComputeClampedModelScale(s32);


/* Start a bounded vector transition; detach any previous secondary-world source. */
void evtSetUnitRot1(evtUnitHandle_t *work, const f32 vector[4], s32 frames) {
    f32 *destination = work->targetVector;

    if ((u32)(frames - 1) < 100) {
        work->linkedUnit = NULL;
        work->motionState = EVT_UNIT_MOTION_STATE_VECTOR;
        SDF_VU0_COPYVECTOR(destination, vector);
        work->motionParameter = frames;
        work->directionOffset = 0;
        work->motionCallback = 0;
        work->motionTicks = 0;
    }
}

/* Track a secondary-world unit and copy the vector in its subobject at +0x10. */
void evtSetUnitRot2(evtUnitHandle_t *work, s32 worldObjectKey, s32 frames) {
    dds3Basic_t *worldUnit;
    EvtWorldSourceTransformPrefix *source;

    worldUnit = dds3SearchSceneBasic(dds3GetControlScene(), worldObjectKey, 0x11);
    if (worldUnit != NULL) {
        source = worldUnit->data;
        evtSetUnitRot1(work, source->rotation, frames);
        work->linkedUnit = worldUnit;
    }
}

/* Configure a mode-one vector transition, without retaining a world source. */
void evtSetUnitMove1(evtUnitHandle_t *work, s32 motionSubmode, const f32 vector[4], s32 unused, s32 frames, s32 valueB6, EvtUnitMotionCallback callback, s32 unusedLast) {
    f32 *destination = work->targetVector;

    work->motionSubmode = motionSubmode;
    work->motionState = EVT_UNIT_MOTION_STATE_SOURCE;
    work->transitionSourceKind = EVT_UNIT_TRANSITION_SOURCE_COPIED_VECTOR;
    work->linkedUnit = NULL;
    SDF_VU0_COPYVECTOR(destination, vector);
    work->motionParameter = frames;
    work->directionOffset = valueB6;
    work->motionCallback = callback;
    work->motionTicks = 0;
}

/* Configure the same transition from a secondary-world object's vector. */
void evtSetUnitMove2(evtUnitHandle_t *work, s32 motionSubmode, s32 worldObjectKey, s32 unused, s32 frames, s32 valueB6, EvtUnitMotionCallback callback, s32 unusedLast) {
    dds3Basic_t *worldUnit;
    EvtWorldSourceTransformPrefix *source;

    worldUnit = dds3SearchSceneBasic(dds3GetControlScene(), worldObjectKey, 0x11);
    if (worldUnit != NULL) {
        source = worldUnit->data;
        evtSetUnitMove1(work, motionSubmode, &source->position.x, unused, frames, valueB6, callback, unusedLast);
        work->transitionSourceKind = EVT_UNIT_TRANSITION_SOURCE_WORLD_NODE;
        work->linkedUnit = worldUnit;
    }
}

void evtSetUnitMove3(evtUnitHandle_t *work, s32 pathObjectKey, s32 frames, s32 valueB6, s32 pathFollowMode, s32 dirFlag, s32 sideMode) {
    dds3Basic_t *pathSource;
    dds3PathBasicControl_t *path;

    pathSource = dds3SearchSceneBasic(dds3GetControlScene(), pathObjectKey, 0x10);
    if (pathSource == NULL) {
        return;
    }
    if (work->pathControl != NULL) {
        dds3DeletePathControl(work->pathControl);
    }
    path = dds3AllocPathBasicWorkData(pathSource);
    work->pathControl = path;
    work->pathSpeed = 40.0f / GetSplineLength(path);
    if (dirFlag == 0) {
        dds3PathBasicControlSetRatio(&path->counter, 0.0f);
        dds3PathBasicControlAlterMode(&path->counter, 0);
    } else {
        dds3PathBasicControlSetRatio(&path->counter, 1.0f);
        dds3PathBasicControlAlterMode(&path->counter, 1);
        work->pathSpeed = -work->pathSpeed;
    }
    switch (pathFollowMode) {
    case 0:
        work->motionSubmode = EVT_UNIT_MOTION_SUBMODE_TARGET_XZ;
        work->flags &= ~2;
        break;
    case 1:
        work->motionSubmode = EVT_UNIT_MOTION_SUBMODE_PATH_ENDPOINT;
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
    dds3PathBasicControlGetPos(path);
    VU0_STORE_VF($vf10, work->targetVector);
    work->motionParameter = frames;
    work->directionOffset = valueB6;
    work->motionCallback = 0;
    work->motionTicks = 0;
}

s32 evtStartUnitModeWithValue(evtUnitHandle_t *eventUnit, EvtUnitMotionCallback callback) {
    s32 result = 0;

    if (callback != 0) {
        eventUnit->motionCallback = callback;
        eventUnit->motionTicks = 0;
        eventUnit->motionState = EVT_UNIT_MOTION_STATE_VALUE;
        result = 1;
    }
    return result;
}

extern void evtAdvanceUnitVisualTransitions(evtUnitHandle_t *);
extern EvtUnitMotionStepResult evtUnitMoveType0(evtUnitHandle_t *);
extern EvtUnitMotionStepResult evtUnitMoveTypePathTrace(evtUnitHandle_t *);
extern void evtRefreshUnitEndpointWork(evtUnitHandle_t *);
EvtUnitMotionStepResult evtUpdateUnitMotion(dds3PoseSet_t *unused, dds3Basic_t *object) {
    f32 delta[4];
    EvtUnitMotionStepResult moving = EVT_UNIT_MOTION_STEP_NOT_APPLIED;
    s32 wobble = 0;
    EffectObjectData *data = object->data;
    evtUnitHandle_t *unit = data->transitionWork;
    dds3ModelHandle_t *model;
    s32 i;
    u8 enabled;

    if (unit == NULL) {
        return EVT_UNIT_MOTION_STEP_NOT_APPLIED;
    }
    model = unit->owner;
    evtAdvanceUnitVisualTransitions(unit);
    switch (unit->motionState) {
    case EVT_UNIT_MOTION_STATE_IDLE:
    case EVT_UNIT_MOTION_STATE_VECTOR:
        if (unit->flags & EVT_UNIT_FLAG_MOTION_START_PENDING) {
            if (unit->flags & 1) {
                mdlSmoothLoopMotion(model, 0, unit->motionSetupId, (f32)unit->motionSetupStartFrame, (f32)unit->motionSetupEndFrame);
            } else {
                mdlSmoothSingleMotion(model, 0, unit->motionSetupId, (f32)unit->motionSetupStartFrame, (f32)unit->motionSetupEndFrame);
            }
            unit->flags &= ~EVT_UNIT_FLAG_MOTION_START_PENDING;
        } else if (unit->flags & 0x20) {
            /* The selected-ID restart stays distinct in source: retail 0x0023DB84
             * and Nocturne 0x001E6864 retain the current-index load after tail merging. */
            s32 currentMotion = model->current.h.motionIndex;

            if (unit->pendingMotionIndex != currentMotion) {
                if (model->first->state == 5) {
                    mdlSmoothLoopMotion(model, 0, unit->pendingMotionIndex,
                                        (f32)unit->motionBlendLeadFrames,
                                        (f32)unit->motionBlendDurationFrames);
                    model->first->frameStep = unit->transitionMotionScale;
                    unit->motionTicks = 0;
                }
            } else {
                if (model->first->state == 5) {
                    mdlSmoothLoopMotion(model, 0, currentMotion,
                                        (f32)unit->motionBlendLeadFrames,
                                        (f32)unit->motionBlendDurationFrames);
                    model->first->frameStep = unit->transitionMotionScale;
                    unit->motionTicks = 0;
                }
            }
        }
        if (unit->flags & EVT_UNIT_FLAG_MOTION_WOBBLE) {
            wobble = 1;
        } else {
            wobble = 0;
        }
        if (unit->motionState == EVT_UNIT_MOTION_STATE_VECTOR) {
            if (evtUnitRotationQuat(unit, unit->targetVector, (f32)unit->motionParameter / 100.0f) !=
                EVT_UNIT_ROTATION_STEP_APPLIED) {
                unit->motionState = EVT_UNIT_MOTION_STATE_IDLE;
            }
        }
        break;
    case EVT_UNIT_MOTION_STATE_SOURCE:
        if (evtCheckMoveEnd(unit) != EVT_UNIT_MOVE_END_REACHED ||
            evtGetNextMovePos(unit) != EVT_UNIT_PATH_NOT_ADVANCED) {
            if (unit->motionSetupId != model->current.h.motionIndex) {
                if (unit->flags & 1) {
                    mdlSmoothLoopMotion(model, 0, unit->motionSetupId, (f32)unit->motionSetupStartFrame, (f32)unit->motionSetupEndFrame);
                } else {
                    mdlSmoothSingleMotion(model, 0, unit->motionSetupId, (f32)unit->motionSetupStartFrame, (f32)unit->motionSetupEndFrame);
                }
            }
            switch (unit->motionSubmode) {
            case EVT_UNIT_MOTION_SUBMODE_TARGET_XZ:
            case EVT_UNIT_MOTION_SUBMODE_TARGET_XZ_WITH_VERTICAL_STEP:
                moving = evtUnitMoveType0(unit);
                if (unit->motionSubmode == EVT_UNIT_MOTION_SUBMODE_TARGET_XZ_WITH_VERTICAL_STEP) {
                    delta[0] = 0.0f;
                    delta[1] = unit->speedY;
                    delta[2] = 0.0f;
                    delta[3] = 0.0f;
                    dds3GetPosPoseSetMCR(unit->effObj, (u128 *)delta);
                }
                break;
            case 2:
                break;
            case EVT_UNIT_MOTION_SUBMODE_PATH_ENDPOINT:
                moving = evtUnitMoveTypePathTrace(unit);
                break;
            default:
                unit->motionState = EVT_UNIT_MOTION_STATE_IDLE;
                break;
            }
            if (unit->motionCallback != NULL) {
                if (unit->motionCallback(unit, 0) == EVT_UNIT_MOTION_CALLBACK_COMPLETE) {
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
                mdlSmoothLoopMotion(model, 0, unit->motionSetupId, (f32)unit->motionSetupStartFrame, (f32)unit->motionSetupEndFrame);
            } else {
                mdlSmoothSingleMotion(model, 0, unit->motionSetupId, (f32)unit->motionSetupStartFrame, (f32)unit->motionSetupEndFrame);
            }
            unit->flags &= ~EVT_UNIT_FLAG_MOTION_START_PENDING;
        } else if (model->first->state == 5) {
            unit->motionState = EVT_UNIT_MOTION_STATE_IDLE;
        } else if ((unit->flags & 0x800000) && (f32)unit->motionEndFrame <= model->first->currentFrame) {
            s32 currentMotion;

            unit->motionState = EVT_UNIT_MOTION_STATE_VALUE;
            currentMotion = model->current.h.motionIndex;
            unit->flags &= ~0x800000;
            if (unit->pendingMotionIndex != currentMotion) {
                mdlSmoothLoopMotion(model, 0, unit->pendingMotionIndex,
                                    (f32)unit->motionBlendLeadFrames,
                                    (f32)unit->motionBlendDurationFrames);
                model->first->frameStep = unit->transitionMotionScale;
                unit->motionTicks = 0;
            }
        }
        break;
    case EVT_UNIT_MOTION_STATE_VALUE:
        if (unit->motionCallback != NULL) {
            if (unit->motionCallback(unit, 0) == EVT_UNIT_MOTION_CALLBACK_COMPLETE) {
                unit->motionState = EVT_UNIT_MOTION_STATE_IDLE;
            }
        } else {
            unit->motionState = EVT_UNIT_MOTION_STATE_IDLE;
        }
        break;
    default:
        return EVT_UNIT_MOTION_STEP_NOT_APPLIED;
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
        enabled = unit->slotFlags[i] & EVT_UNIT_MOTION_SLOT_ENABLED;
        if (enabled != 0) {
            if (unit->slotFlags[i] & EVT_UNIT_MOTION_SLOT_START_PENDING) {
                unit->slotFlags[i] &= ~EVT_UNIT_MOTION_SLOT_START_PENDING;
                printf("set exmot %d_%d\n", i, unit->slotA[i]);
                if (unit->slotFlags[i] & EVT_UNIT_MOTION_SLOT_LOOP_EXTERNAL_MOTION) {
                    mdlSmoothLoopMotion(model, i, unit->slotA[i], (f32)unit->slotB[i], (f32)unit->slotC[i]);
                } else {
                    mdlSmoothSingleMotion(model, i, unit->slotA[i], (f32)unit->slotB[i], (f32)unit->slotC[i]);
                }
            }
            if (mdlChkMotionEndEx(model, i) != MDL_MOTION_END_NOT_TERMINAL) {
                if (unit->slotFlags[i] & EVT_UNIT_MOTION_SLOT_LOOP_ON_MOTION_END) {
                    if ((unit->flags & 0x1000000) == 0) {
                        mdlSmoothLoopMotion(model, i, unit->tableValues[i], 0.0f, 0.0f);
                    } else {
                        mdlSmoothLoopMotion(model, i, unit->tableValues[i], (f32)unit->tableMotionBlendLeadFrames[i], (f32)unit->tableMotionBlendDurationFrames[i]);
                        mdlSetMotionSpeedEx(model, i, unit->tableMotionFrameStep[i]);
                    }
                } else if ((unit->flags & 0x1000000) &&
                           (unit->slotFlags[i] & EVT_UNIT_MOTION_SLOT_STOP_ON_BATTLE_ACTION)) {
                    mdlSmoothSingleMotion(model, i, unit->slotA[i], 0.0f, 1.0f);
                    mdlSetMotionSpeedEx(model, i, 0.0f);
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
        evtUnitVectorSlots[i].unitHandle = NULL;
    }
}

void evtSetSlotVectors(s32 slotIndex, EvtUnitVectorSlotState slotState, evtUnitHandle_t *unitHandle,
                       const f32 *firstEndpoint, const f32 *secondEndpoint, const f32 *color) {
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
            evtUnitVectorSlots[slotIndex].unitHandle = unitHandle;
        } else {
            evtUnitVectorSlots[slotIndex].unitHandle = NULL;
        }
    }
}

void evtSetSlotVector(s32 slotIndex, f32 x, f32 y) {
    if (slotIndex < 10) {
        evtUnitVectorSlots[slotIndex].auxX = x;
        evtUnitVectorSlots[slotIndex].auxY = y;
    }
}

/* Copy the second vector of the slot bound to `unitHandle` (else the first state-2 slot). */
s32 evtCopyUnitVectorSlotEndpoint(evtUnitHandle_t *unitHandle, f32 *out) {
    s32 found = -1;
    s32 i;

    for (i = 0; i < 10; i++) {
        if (evtUnitVectorSlots[i].state == EVT_UNIT_VECTOR_SLOT_UNIT_BOUND && evtUnitVectorSlots[i].unitHandle == unitHandle) {
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
void evtApplyMatchingUnitSlotEndpoints(evtUnitHandle_t *unit) {
    f32 ends[4][4];
    f32 color[4];
    SdfLightSources desc = { ends, 0, 0 };
    s32 found = -1;
    s32 i;

    for (i = 0; i < 10; i++) {
        if (evtUnitVectorSlots[i].state == EVT_UNIT_VECTOR_SLOT_UNIT_BOUND && evtUnitVectorSlots[i].unitHandle == unit) {
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
    sdf3DMakeLightParallelPacket(unit->endpointWork, desc, color);
    unit->value = (u32)unit->endpointWork;
}

/* Find auxiliary coordinates for the unit-bound slot, else the shared fallback. */
EvtUnitVectorSlotAuxCoordinatesResult evtFindUnitSlotAuxCoordinates(
    evtUnitHandle_t *unit, f32 *outX, f32 *outY) {
    s32 i;

    for (i = 0; i < 10; i++) {
        if (evtUnitVectorSlots[i].state == EVT_UNIT_VECTOR_SLOT_UNIT_BOUND && evtUnitVectorSlots[i].unitHandle == unit) {
            *outX = evtUnitVectorSlots[i].auxX;
            *outY = evtUnitVectorSlots[i].auxY;
            return EVT_UNIT_VECTOR_SLOT_AUX_COORDINATES_FOUND;
        }
    }
    for (i = 0; i < 10; i++) {
        if (evtUnitVectorSlots[i].state == EVT_UNIT_VECTOR_SLOT_SHARED_FALLBACK) {
            *outX = evtUnitVectorSlots[i].auxX;
            *outY = evtUnitVectorSlots[i].auxY;
            return EVT_UNIT_VECTOR_SLOT_AUX_COORDINATES_FOUND;
        }
    }
    return EVT_UNIT_VECTOR_SLOT_AUX_COORDINATES_NOT_FOUND;
}

dds3Basic_t *evtSearchBasic(s32 objectKind, s32 worldObjectKey) {
    dds3Basic_t *world;

    world = dds3GetActiveScene();
    return dds3SearchSceneBasic(world, worldObjectKey, objectKind);
}

u32 evtCommand_MODEL_PLAYER(void) {
    dds3Basic_t *world;
    dds3Basic_t *object;
    s32 playerObjectKey;

    world = dds3GetActiveScene();
    object = dds3GetActiveUnitSceneBasic(world);
    playerObjectKey = -1;
    if (object != NULL) {
        playerObjectKey = object->key;
    }
    scrSetIntReturnValue(playerObjectKey);
    return 1;
}

u32 evtCommand_MODEL_DSP(void) {
    s32 slotKind;
    s32 resource;
    s32 viewerObjectKey;

    slotKind = scrGetIntPara(0);
    resource = scrGetIntPara(1);
    viewerObjectKey = evtLoadUnit(slotKind, resource);
    scrSetIntReturnValue(viewerObjectKey);
    return 1;
}

u32 evtCommand_MODEL_BE(void) {
    s32 eventId;
    s32 resourceId;
    s32 modelKey;
    dds3ProcessID_t *motionSeTask;

    if (scrGetScriptHandle() == 0) {
        return 1;
    }
    eventId = scrGetIntPara(0);
    resourceId = scrGetIntPara(1);
    modelKey = evtInitMotionSeProcess(eventId, resourceId);
    if (modelKey < 0) {
        evtPrintDeveloperConsoleMessage("MODEL_BE not fount RID = %d!\n", scrGetIntPara(1));
        return 1;
    }
    eventId = scrGetIntPara(0);
    resourceId = scrGetIntPara(1);
    motionSeTask = evtLoadModelSe(modelKey, eventId, resourceId);
    if (motionSeTask != 0) {
        dds3LinkProcess(evtSearchEventPack(scrGetIntPara(0)), motionSeTask);
    }
    /* The original success path ends in this void call, without a return. */
    scrSetIntReturnValue(modelKey);
}

u32 evtCommand_MODEL_POS(void) {
    u32 targetKey;
    u32 sourceKey;

    targetKey = scrGetIntPara(0);
    sourceKey = scrGetIntPara(1);
    evtSetUnitPos(targetKey, sourceKey);
    return 1;
}

u32 evtCommand_MODEL_SHADOW_ON(void) {
    dds3Basic_t *ctx;
    s32 worldObjectKey;
    dds3Basic_t *unit;
    u32 ret = 1;

    ctx = dds3GetActiveScene();
    worldObjectKey = scrGetIntPara(0);
    unit = dds3SearchSceneBasic(ctx, worldObjectKey, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3ObjectBaseOnFlag(unit, 0x400);
    dds3ObjectBaseOffFlag(unit, 0x200);
    return 1;
}

u32 evtCommand_MODEL_SHADOW_ON2(void) {
    dds3Basic_t *ctx;
    s32 worldObjectKey;
    dds3Basic_t *unit;
    u32 ret = 1;

    ctx = dds3GetActiveScene();
    worldObjectKey = scrGetIntPara(0);
    unit = dds3SearchSceneBasic(ctx, worldObjectKey, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3ObjectBaseOffFlag(unit, 0x400);
    dds3ObjectBaseOnFlag(unit, 0x200);
    return 1;
}

u32 evtCommand_MODEL_SHADOW_OFF(void) {
    dds3Basic_t *ctx;
    s32 worldObjectKey;
    dds3Basic_t *unit;
    u32 ret = 1;

    ctx = dds3GetActiveScene();
    worldObjectKey = scrGetIntPara(0);
    unit = dds3SearchSceneBasic(ctx, worldObjectKey, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3ObjectBaseOffFlag(unit, 0x400);
    dds3ObjectBaseOffFlag(unit, 0x200);
    return 1;
}

u32 evtOpQueueWorldObjectPendingValue(void) {
    dds3Basic_t *ctx;
    s32 worldObjectKey;
    dds3Basic_t *unit;
    u32 ret = 1;

    ctx = dds3GetActiveScene();
    worldObjectKey = scrGetIntPara(0);
    unit = dds3SearchSceneBasic(ctx, worldObjectKey, 5);
    if (unit == NULL) {
        return ret;
    }
    evtArmEffectObjectPendingValue(unit, scrGetIntPara(1));
    return 1;
}

u32 evtOpClearWorldObjectPendingValue(void) {
    dds3Basic_t *ctx;
    s32 worldObjectKey;
    dds3Basic_t *unit;
    u32 ret = 1;

    ctx = dds3GetActiveScene();
    worldObjectKey = scrGetIntPara(0);
    unit = dds3SearchSceneBasic(ctx, worldObjectKey, 5);
    if (unit == NULL) {
        return ret;
    }
    evtResetObjectPendingValue(unit);
    return 1;
}

u32 evtOpModelLodChg(void) {
    s32 lod;
    dds3Basic_t *world;
    dds3Basic_t *unit;
    EffectObjectData *data;
    dds3ObjectBaseControlWorkData_t *modelHolder;
    dds3ModelHandle_t *context;
    sdfModelRootNode_t *model;
    s32 max;

    lod = scrGetIntPara(1);
    printf("call: MODEL_LOD_CHG(int,int)\n");
    world = dds3GetActiveScene();
    unit = dds3SearchSceneBasic(world, scrGetIntPara(0), 5);
    if (unit == NULL) {
        printf("warning!! MODEL_LOD_CHG(int,int) unit pointer null\n");
        return 1;
    }
    data = unit->data;
    if (data->modelHolder == NULL) {
        printf("warning!! MODEL_LOD_CHG(int,int) workbase pointer null\n");
        return 1;
    }
    modelHolder = data->modelHolder;
    if (modelHolder->resourceHandle == 0) {
        printf("warning!! MODEL_LOD_CHG(int,int) mh pointer null\n");
        return 1;
    }
    context = (dds3ModelHandle_t *)modelHolder->resourceHandle;
    model = context->inner;
    if (model == NULL) {
        printf("warning!! MODEL_LOD_CHG(int,int) root pointer null\n");
        return 1;
    }
    max = (s32)sdfGetLodChunkValue(model);
    if (max < lod) {
        printf("warning!! MODEL_LOD_CHG(int,int) lodno over!! max=%d setval=%d\n", max, lod);
        return 1;
    }
    model->lodIndex = lod;
    printf("success: MODEL_LOD_CHG(int,int)\n");
    return 1;
}

u32 evtCommand_MODEL_POS_XYZ(void) {
    f32 vector[4];
    dds3Basic_t *world;
    s32 worldObjectKey;
    dds3Basic_t *unit;

    memset(vector, 0, 0x10);
    world = dds3GetActiveScene();
    worldObjectKey = scrGetIntPara(0);
    unit = dds3SearchSceneBasic(world, worldObjectKey, 5);
    if (unit == NULL) {
        return 1;
    }
    vector[0] = scrGetFloatPara(1);
    vector[1] = scrGetFloatPara(2);
    vector[2] = scrGetFloatPara(3);
    dds3SetPosPoseSet(unit, (u128 *)vector);
    return 1;
}

u32 evtCommand_MODEL_ROT_XYZ(void) {
    f32 v[4];
    dds3Basic_t *ctx;
    s32 worldObjectKey;
    dds3Basic_t *unit;
    f32 toRad;
    f32 x;
    f32 y;

    memset(v, 0, 0x10);
    v[3] = 1.0f;
    ctx = dds3GetActiveScene();
    worldObjectKey = scrGetIntPara(0);
    unit = dds3SearchSceneBasic(ctx, worldObjectKey, 5);
    if (unit == NULL) {
        return 1;
    }
    toRad = 0.017453293f;
    x = scrGetFloatPara(1) * toRad;
    y = scrGetFloatPara(2) * toRad;
    sdfConvertEulerAnglesToQuaternionVU(x, y, 0.0f);
    SDF_VU0_COPYVECTOR_A_TO_B();
    sdfConvertEulerAnglesToQuaternionVU(0.0f, 0.0f, scrGetFloatPara(3) * toRad);
    effMiscQuatMultiplyVU();
    VU0_STORE_VF($vf10, v);
    dds3SetRotPoseSet(unit, (u128 *)v);
    return 1;
}

u32 evtCommand_MODEL_SCL_XYZ(void) {
    f32 vector[4];
    dds3Basic_t *world;
    s32 worldObjectKey;
    dds3Basic_t *unit;

    memset(vector, 0, 0x10);
    vector[3] = 1.0f;
    world = dds3GetActiveScene();
    worldObjectKey = scrGetIntPara(0);
    unit = dds3SearchSceneBasic(world, worldObjectKey, 5);
    if (unit == NULL) {
        return 1;
    }
    vector[0] = scrGetFloatPara(1);
    vector[1] = scrGetFloatPara(2);
    vector[2] = scrGetFloatPara(3);
    dds3SetSclPoseSet(unit, (u128 *)vector);
    return 1;
}

extern char D_00421810[];

u32 evtOpDeprecatedModelMotion(void) {
    evtUnitHandle_t *unit;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    if (unit == NULL) {
        return 1;
    }
    evtSetUnitMotion(unit, scrGetIntPara(1), scrGetIntPara(2), scrGetIntPara(3), 0);
    printf(D_00421810);
    return 1;
}

u32 evtOpSetUnitParams5(void) {
    evtUnitHandle_t *unit;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 motionId = scrGetIntPara(1);
        s32 startFrame = scrGetIntPara(2);
        s32 endFrame = scrGetIntPara(3);
        s32 mode = scrGetIntPara(4);
        evtSetUnitMotion(unit, motionId, startFrame, endFrame, mode);
    }
    return 1;
}

u32 evtOpSetUnitParams6(void) {
    evtUnitHandle_t *unit;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 scriptParam1 = scrGetIntPara(1);
        s32 scriptParam2 = scrGetIntPara(2);
        s32 scriptParam3 = scrGetIntPara(3);
        s32 scriptParam4 = scrGetIntPara(4);
        s32 scriptParam5 = scrGetIntPara(5);
        evtConfigureUnitMotionSlot(unit, scriptParam1, scriptParam2, scriptParam3, scriptParam4,
                                   (EvtUnitMotionSlotMode)scriptParam5);
    }
    return 1;
}

void evtRunKutiMotion(void) {
    evtActivateStoredUnitMotionSlot(evtWindowMotionUnit);
}

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_00421810);

void evtLipsExecFunction(s32 unitUniqueId, s32 motionId) {
    evtUnitHandle_t *unit = NULL;
    dds3ModelHandle_t *model = NULL;
    dds3Basic_t *node;

    if (unitUniqueId == 0) {
        return;
    }
    for (node = ((EvtWorldTable *)((dds3Basic_t *)dds3GetActiveScene())->data)->slots[EVT_WORLD_SLOT_UNIT].head; node != NULL; node = node->next) {
        model = ((EvtLipsLink *)node->data)->mh->model;
        if (sdfGetUniqueChunkValue(model->inner) == unitUniqueId) {
            unit = ((EvtLipsLink *)node->data)->unit;
            break;
        }
    }
    if (unit == NULL) {
        printf("warning: call evtLipsExecFunction() but not find now reegisted unit same UnitUniqID\n");
        return;
    }
    if (motionId >= mdlGetNumOfCut(model, 2)) {
        printf("warning: call evtLipsExecFunction() but over have motionno fpr user specified motion no.\n");
        return;
    }
    evtConfigureUnitMotionSlot(unit, 2, motionId, 0, 5,
                               EVT_UNIT_MOTION_SLOT_MODE_LOOP_EXTERNAL_MOTION);
    D_004371F0 = unitUniqueId;
    printf("<lips %d %d> \n", unitUniqueId, motionId);
}

void evtLipsStopFunction(void) {
    evtUnitHandle_t *unit = NULL;
    dds3ModelHandle_t *model = NULL;
    dds3Basic_t *node;

    if (D_004371F0 == 0) {
        return;
    }
    for (node = ((EvtWorldTable *)((dds3Basic_t *)dds3GetActiveScene())->data)->slots[EVT_WORLD_SLOT_UNIT].head; node != NULL; node = node->next) {
        model = ((EvtLipsLink *)node->data)->mh->model;
        if (sdfGetUniqueChunkValue(model->inner) == D_004371F0) {
            unit = ((EvtLipsLink *)node->data)->unit;
            break;
        }
    }
    if (unit == NULL) {
        printf("warning: call evtLipsStopFunction() but not find now reegisted unit same UnitUniqID\n");
        return;
    }
    if (mdlGetNumOfCut(model, 2) == 0) {
        printf("warning: call evtLipsStopFunction() but over have motionno fpr user specified motion no.\n");
        return;
    }
    evtConfigureUnitMotionSlot(unit, 2, 0, 0, 3,
                               EVT_UNIT_MOTION_SLOT_MODE_STOP_ON_BATTLE_ACTION);
    printf("<lips_stop> stopunitid = %d\n", D_004371F0);
    D_004371F0 = 0;
}

u32 evtCommand_KUTISET(void) {
    evtUnitHandle_t *unit;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 firstSlot = scrGetIntPara(1);
        s32 secondSlot = scrGetIntPara(2);
        evtSetUnitKutiMotion(unit, firstSlot, secondSlot);
        evtWindowMotionUnit = unit;
    }
    {
        s32 window = scrGetMessageHandle();
        if (window < 0) {
            return 1;
        }
        itfMesMngSetKutiCallback(window, evtRunKutiMotion);
    }
    return 1;
}

u32 evtCommand_KUTI(void) {
    evtUnitHandle_t *unit;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 firstSlot = scrGetIntPara(1);
        s32 secondSlot = scrGetIntPara(2);
        evtSetUnitKutiMotion(unit, firstSlot, secondSlot);
        evtActivateStoredUnitMotionSlot(unit);
        evtWindowMotionUnit = unit;
    }
    {
        s32 window = scrGetMessageHandle();
        if (window < 0) {
            return 1;
        }
        itfMesMngSetKutiCallback(window, evtRunKutiMotion);
    }
    return 1;
}

u32 evtCommand_MODEL_WAIT(void) {
    s32 worldObjectKey;
    evtUnitHandle_t *unit;
    u32 ret = 1;

    worldObjectKey = scrGetIntPara(0);
    unit = evtGetUnitHandle(worldObjectKey);
    if (unit == NULL) {
        return ret;
    }
    return evtCheckUnitWait(unit) != EVT_UNIT_WAIT_IN_PROGRESS;
}

u32 evtCommand_MODEL_EXWAIT(void) {
    s32 worldObjectKey;
    evtUnitHandle_t *unit;
    s32 motionNodeIndex;
    u32 ret = 1;

    worldObjectKey = scrGetIntPara(0);
    unit = evtGetUnitHandle(worldObjectKey);
    if (unit == NULL) {
        return ret;
    }
    motionNodeIndex = scrGetIntPara(1);
    if (((unit->slotFlags[motionNodeIndex] & EVT_UNIT_MOTION_SLOT_ENABLED) & 0xFF) == 0) {
        return ret;
    }
    return mdlChkMotionEndEx(unit->owner, scrGetIntPara(1)) != MDL_MOTION_END_NOT_TERMINAL;
}

u32 evtCmdDestroySelectedWorldUnit(void) {
    dds3Basic_t *ctx;
    s32 worldObjectKey;
    dds3Basic_t *unit;
    u32 ret = 1;

    ctx = dds3GetActiveScene();
    worldObjectKey = scrGetIntPara(0);
    unit = dds3SearchSceneBasic(ctx, worldObjectKey, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3DeleteSceneBasicData(unit);
    return 1;
}

u32 evtOpStartUnitTransitionTowardWorldObject(void) {
    evtUnitHandle_t *unit;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    if (unit == NULL) {
        return 1;
    }
    unit->motionTicks = 0;
    {
        s32 motionSubmode = scrGetIntPara(2);
        s32 worldObjectKey = scrGetIntPara(1);
        s32 frames = scrGetIntPara(3);
        s32 valueB6 = scrGetIntPara(4);
        evtSetUnitMove2(unit, motionSubmode, worldObjectKey, -1, frames, valueB6, 0, 0);
    }
    if (scrGetIntPara(2) == EVT_UNIT_MOTION_SUBMODE_TARGET_XZ_WITH_VERTICAL_STEP) {
        evtUnitPrepareVerticalMoveSteps(unit);
    }
    return 1;
}

u32 evtCommand_MODEL_MOVE2(void) {
    evtUnitHandle_t *unit;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    if (unit == NULL) {
        return 1;
    }
    unit->motionTicks = 0;
    {
        s32 pathObjectKey = scrGetIntPara(1);
        s32 frames = scrGetIntPara(5);
        s32 valueB6 = scrGetIntPara(6);
        s32 pathFollowMode = scrGetIntPara(2);
        s32 dirFlag = scrGetIntPara(4);
        s32 sideMode = scrGetIntPara(3);
        evtSetUnitMove3(unit, pathObjectKey, frames, valueB6, pathFollowMode, dirFlag, sideMode);
    }
    return 1;
}

u32 evtScriptSetUnitTableValueAndReset(void) {
    evtUnitHandle_t *unit;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 tableValueIndex = scrGetIntPara(1);
        s32 value = scrGetIntPara(2);
        unit->tableValues[tableValueIndex] = value;
        unit->tableMotionBlendLeadFrames[tableValueIndex] = 0;
        unit->tableMotionBlendDurationFrames[tableValueIndex] = 0;
        unit->tableMotionFrameStep[tableValueIndex] = 1.0f;
    }
    return 1;
}

/* Store the script-supplied value for the selected event unit. */
u32 evtCommand_MODEL_WAIT_MOT(void) {
    s32 worldObjectKey;
    evtUnitHandle_t *unit;

    worldObjectKey = scrGetIntPara(0);
    unit = evtGetUnitHandle(worldObjectKey);
    if (unit == NULL) {
        return 1;
    }
    unit->pendingMotionIndex = scrGetIntPara(1);
    return 1;
}

/* Store the two script-supplied world-unit state values. */
u32 evtCommand_MODEL_WAIT_HOKAN(void) {
    s32 worldObjectKey;
    evtUnitHandle_t *unit;

    worldObjectKey = scrGetIntPara(0);
    unit = evtGetUnitHandle(worldObjectKey);
    if (unit == NULL) {
        return 1;
    }
    unit->motionBlendLeadFrames = scrGetIntPara(1);
    unit->motionBlendDurationFrames = scrGetIntPara(2);
    return 1;
}

u32 evtCommand_MODEL_MOVE_MOT(void) {
    s32 worldObjectKey;
    evtUnitHandle_t *unit;

    worldObjectKey = scrGetIntPara(0);
    unit = evtGetUnitHandle(worldObjectKey);
    if (unit == NULL) {
        return 1;
    }
    evtSetUnitMotion(unit, scrGetIntPara(1), 5, 7, 1);
    printf("\xA2\xA8\xC3\xED\xB0\xD5:MODEL_MOVE_MOT()\xA4\xB3\xA4\xCE\xA5\xB3\xA5\xDE\xA5\xF3\xA5\xC9"
                  "\xA4\xCF\xBA\xA3\xB8\xE5\xBB\xC8\xCD\xD1\xB6\xD8\xBB\xDF\xA4\xCB\xA4\xCA\xA4\xEA\xA4\xDE\xA4\xB9\xA1\xA3\n");
    return 1;
}

u32 evtCommand_MODEL_ROT(void) {
    s32 unitObjectKey;
    evtUnitHandle_t *unit;
    s32 worldObjectKey;
    s32 frames;

    unitObjectKey = scrGetIntPara(0);
    unit = evtGetUnitHandle(unitObjectKey);
    if (unit == NULL) {
        return 1;
    }
    unit->motionTicks = 0;
    worldObjectKey = scrGetIntPara(1);
    frames = scrGetIntPara(2);
    evtSetUnitRot2(unit, worldObjectKey, frames);
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


extern f32 EulerAngleAxisY(void);

u32 evtOpTurnUnitRelativeToWorldNode(void) {
    evtUnitHandle_t *unit;
    dds3Basic_t *actor;
    dds3Basic_t *source;
    EvtWorldSourceTransformPrefix *sourceTransform;
    f32 rotation[4] __attribute__((aligned(16)));
    f32 actorPosition[4] __attribute__((aligned(16)));
    f32 sourcePosition[4] __attribute__((aligned(16)));
    f32 referenceAngle;
    f32 targetAngle;
    f32 angleDelta;
    s32 frames;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    if (unit == NULL) {
        return 1;
    }
    unit->motionTicks = 0;

    actor = (dds3Basic_t *)dds3SearchSceneBasic(
        dds3GetActiveScene(), scrGetIntPara(0), 5);
    if (actor == NULL) {
        return 1;
    }
    source = (dds3Basic_t *)dds3SearchSceneBasic(
        dds3GetActiveScene(), scrGetIntPara(1), 0x11);
    if (source == NULL) {
        return 1;
    }

    sourceTransform = source->data;
    dds3GetPosPoseSet(actor);
    SDF_VU0_STOREVECTOR(actorPosition);
    SDF_VU0_COPYVECTOR(sourcePosition, &sourceTransform->position);

    if ((unit->referenceAngleCacheFlags & EVT_UNIT_REFERENCE_ANGLE_CACHE_VALID) == 0) {
        dds3GetRotPoseSet(actor);
        referenceAngle = EulerAngleAxisY();
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
    SDF_VU0_STOREVECTOR_NOMEM(rotation);

    frames = scrGetIntPara(2);
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

    evtSetUnitRot1(unit, rotation, frames);
    evtArmEffectObjectPendingValue(actor, source->key);
    return 1;
}

u32 evtOpRestoreUnitReferenceAngle(void) {
    evtUnitHandle_t *unit;
    dds3Basic_t *actor;
    f32 rotation[4] __attribute__((aligned(16)));
    f32 referenceAngle;
    f32 angleDelta;
    s32 frames;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    if (unit == NULL) {
        return 1;
    }
    unit->motionTicks = 0;
    if ((unit->referenceAngleCacheFlags & EVT_UNIT_REFERENCE_ANGLE_CACHE_VALID) == 0) {
        return 1;
    }
    actor = dds3SearchSceneBasic(
        dds3GetActiveScene(), scrGetIntPara(0), 5);
    if (actor == NULL) {
        return 1;
    }
    dds3GetRotPoseSet(actor);
    referenceAngle = -(EulerAngleAxisY() * 57.29577637f);
    sdfConvertEulerAnglesToQuaternionVU(0.0f, unit->cachedReferenceAngleDegrees * 0.017453293f, 0.0f);
    /* This is the first write to the vector supplied to the transition. */
    SDF_VU0_STOREVECTOR_NOMEM(rotation);
    frames = scrGetIntPara(1);
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
    evtSetUnitRot1(unit, rotation, frames);
    evtResetObjectPendingValue(actor);
    return 1;
}

u32 evtCommand_MODEL_ON(void) {
    s32 worldObjectKey;
    evtUnitHandle_t *unit;

    worldObjectKey = scrGetIntPara(0);
    unit = evtGetUnitHandle(worldObjectKey);
    if (unit != NULL) {
        unit->owner->flags &= ~MDL_SKIP_TRANSFORMS;
    }
    return 1;
}

u32 evtCommand_MODEL_OFF(void) {
    s32 worldObjectKey;
    evtUnitHandle_t *unit;

    worldObjectKey = scrGetIntPara(0);
    unit = evtGetUnitHandle(worldObjectKey);
    if (unit != NULL) {
        unit->owner->flags |= MDL_SKIP_TRANSFORMS;
    }
    return 1;
}

u32 evtCommand_MODEL_LIGHT_ON(void) {
    s32 worldObjectKey;
    evtUnitHandle_t *unit;

    worldObjectKey = scrGetIntPara(0);
    unit = evtGetUnitHandle(worldObjectKey);
    evtUnitLightOn(unit);
    return 1;
}

u32 evtCommand_MODEL_LIGHT_OFF(void) {
    s32 worldObjectKey;
    evtUnitHandle_t *unit;
    s32 duration;

    worldObjectKey = scrGetIntPara(0);
    unit = evtGetUnitHandle(worldObjectKey);
    duration = scrGetIntPara(1);
    evtUnitLightOff(unit, duration);
    return 1;
}

u32 evtCommand_MODEL_LIGHT_WAIT(void) {
    s32 worldObjectKey;
    evtUnitHandle_t *unit;
    EvtUnitLightAnimationStatus active;

    worldObjectKey = scrGetIntPara(0);
    unit = evtGetUnitHandle(worldObjectKey);
    active = evtUnitLightAnimeStat(unit);
    return active == EVT_UNIT_LIGHT_ANIMATION_INACTIVE;
}

u32 evtCommand_MODEL_LIGHT_RGB(void) {
    evtUnitHandle_t *unit;
    s32 color1[4];
    s32 color2[4];
    u32 packed1;
    u32 packed2;
    u32 scale;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    VU0_SCALAR_OP(scrGetFloatPara(2), "vaddx.x vf10, vf0, vf2x");
    VU0_SCALAR_OP(scrGetFloatPara(3), "vaddx.y vf10, vf0, vf2x");
    VU0_SCALAR_OP(scrGetFloatPara(4), "vaddx.z vf10, vf0, vf2x");
    SDF_VU0_VECTOR_W_SET_ZERO();
    scale = 0x43000000;
    EE_MMI_RGBA_PACK_UNIT(packed1, scale);
    color1[0] = packed1;
    VU0_SCALAR_OP(scrGetFloatPara(5), "vaddx.x vf10, vf0, vf2x");
    VU0_SCALAR_OP(scrGetFloatPara(6), "vaddx.y vf10, vf0, vf2x");
    VU0_SCALAR_OP(scrGetFloatPara(7), "vaddx.z vf10, vf0, vf2x");
    SDF_VU0_VECTOR_W_SET_ONE();
    EE_MMI_RGBA_PACK_UNIT(packed2, scale);
    color2[0] = packed2;
    evtSetUnitLightAndAmbient(unit, scrGetIntPara(1), packed1, packed2);
    return 1;
}

const char D_00421A30[] = "WARNING: MODEL_LIGHT_DIR length zero\n";

u32 evtCommand_MODEL_LIGHT_DIR(void) {
    f32 direction[4];
    evtUnitHandle_t *unit;

    memset(direction, 0, sizeof(direction));
    unit = evtGetUnitHandle(scrGetIntPara(0));
    direction[0] = scrGetFloatPara(2);
    direction[1] = scrGetFloatPara(3);
    direction[2] = scrGetFloatPara(4);
    if (ffabsf(direction[0]) <= 0.01f && ffabsf(direction[1]) <= 0.01f &&
        ffabsf(direction[2]) <= 0.01f) {
        evtPrintDeveloperConsoleMessage(D_00421A30);
        return 1;
    }
    SDF_VU0_LOADVECTOR(direction);
    evtSetUnitLightDir(unit, scrGetIntPara(1));
    return 1;
}


u32 evtCommand_MODEL_RGB(void) {
    evtUnitHandle_t *unit;
    s32 color[4];
    u32 packed;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    VU0_SCALAR_OP(scrGetFloatPara(2), "vaddx.x vf10, vf0, vf2x");
    VU0_SCALAR_OP(scrGetFloatPara(3), "vaddx.y vf10, vf0, vf2x");
    VU0_SET_AXIS_CLEAR_W(scrGetFloatPara(4), z);
    EE_MMI_RGBA_PACK_F128(packed);
    color[0] = packed;
    evtSetUnitColorRGB(unit, scrGetIntPara(1), packed);
    return 1;
}

u32 evtCommand_MODEL_A(void) {
    evtUnitHandle_t *unit;
    s32 color[4];
    u32 packed;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    SDF_VU0_LOADIDENTITYVECTOR();
    VU0_SCALAR_OP(scrGetFloatPara(2), "vmulx.w vf10, vf0, vf2x");
    EE_MMI_RGBA_PACK_F128(packed);
    color[0] = packed;
    evtSetUnitColorA(unit, scrGetIntPara(1), packed);
    return 1;
}

u32 evtCommand_MODEL_LIGHT_LINK(void) {
    s32 worldObjectKey;
    evtUnitHandle_t *unit;
    dds3Basic_t *target;

    worldObjectKey = scrGetIntPara(0);
    unit = evtGetUnitHandle(worldObjectKey);
    target = evtSearchBasic(9, scrGetIntPara(2));
    if (target == NULL) {
        return 1;
    }
    evtSetUnitValueTransition(unit, target, scrGetIntPara(1));
    return 1;
}

u32 evtCommand_MODEL_LIGHT_UNLINK(void) {
    s32 worldObjectKey;
    evtUnitHandle_t *unit;
    s32 duration;

    worldObjectKey = scrGetIntPara(0);
    unit = evtGetUnitHandle(worldObjectKey);
    duration = scrGetIntPara(1);
    evtEndUnitValueTransition(unit, duration);
    return 1;
}

const char D_00421A58[] = "EFFECT_BE not fount RID = %d\n";

u32 evtCommand_EFFECT_BE(void) {
    u8 firstVector[16];
    u8 secondVector[16];
    s32 taskId;
    s32 key;
    struct EffNodeDescriptor *descriptor;
    dds3Basic_t *object;

    memset(firstVector, 0, sizeof(firstVector));
    memset(secondVector, 0, sizeof(secondVector));
    taskId = scrGetIntPara(0);
    key = scrGetIntPara(1);
    descriptor = evtGetEventPackResourceAddress(taskId, key);
    if (descriptor == NULL) {
        key = scrGetIntPara(1);
        evtPrintDeveloperConsoleMessage(D_00421A58, key);
        scrSetIntReturnValue(0);
        return 1;
    }
    object = effObjSpawnDescriptorBoundEffect(descriptor, firstVector, secondVector);
    if (object == NULL) {
        printf(D_00421A78, 1);
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(object, 1);
        scrSetIntReturnValue(object->key);
    }
    return 1;
}

u32 evtCommand_EFFECT_LOAD(void) {
    u8 buf1[16];
    u8 buf2[16];
    const char *resourceName;
    dds3Basic_t *unit;

    memset(buf1, 0, 0x10);
    memset(buf2, 0, 0x10);
    resourceName = scrGetStrPara(0);
    unit = effObjSpawnLoadedResourceEffect(resourceName, buf1, buf2);
    if (unit == NULL) {
        printf(D_00421A90, 1);
        printf(D_00421AC0, scrGetStrPara(0));
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(unit, 1);
        scrSetIntReturnValue(unit->key);
    }
    return 1;
}

u32 evtCommand_EFFBED_LOAD(void) {
    f32 buf1[4];
    f32 buf2[4];
    const char *param0;
    dds3Basic_t *unit;

    memset(buf1, 0, 0x10);
    memset(buf2, 0, 0x10);
    buf2[3] = 1.0f;
    param0 = scrGetStrPara(0);
    unit = dds3AddBEDEffectObjectBasic(param0, buf1, buf2);
    if (unit == NULL) {
        printf(D_00421A90, 1);
        printf(D_00421AC0, scrGetStrPara(0));
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(unit, 1);
        scrSetIntReturnValue(unit->key);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_00421A78);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_00421A90);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_00421AC0);

const char D_00421AD8[] = "EFFBED_BE not fount RID = %d\n";

u32 evtCommand_EFFBED_BE(void) {
    u8 position[16];
    u8 scale[16];
    s32 taskId;
    s32 key;
    struct SoundMixer *resource;
    dds3Basic_t *object;

    memset(position, 0, sizeof(position));
    memset(scale, 0, sizeof(scale));
    taskId = scrGetIntPara(0);
    key = scrGetIntPara(1);
    resource = evtGetEventPackResourceAddress(taskId, key);
    if (resource == NULL) {
        key = scrGetIntPara(1);
        evtPrintDeveloperConsoleMessage(D_00421AD8, key);
        scrSetIntReturnValue(0);
        return 1;
    }
    object = dds3AddBEDEffectObjectBasic2(resource, position, scale);
    if (object == NULL) {
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(object, 1);
        scrSetIntReturnValue(object->key);
    }
    return 1;
}

u32 evtCommand_EFFBED_BE_EVO(void) {
    return 1;
}

u32 evtCommand_EFFBED_START(void) {
    s32 worldObjectKey;
    dds3Basic_t *unit;
    u32 entryId;

    worldObjectKey = scrGetIntPara(0);
    unit = evtSearchBasic(7, worldObjectKey);
    if (unit != NULL) {
        entryId = scrGetIntPara(1);
        if (entryId >= 3U) {
            evtPrintDeveloperConsoleMessage(D_00421AF8, entryId);
            return 1;
        } else {
            dds3StartBEDEffectObjectBasic(unit, entryId);
        }
    }
    return 1;
}

f32 evtComputeClampedModelScale(s32 modelRecordIndex) {
    f32 scale = (D_00435DF0[modelRecordIndex].reach + D_00435DF0[modelRecordIndex].height * 0.5f) * 0.5f * D_00435DF0[modelRecordIndex].scale * (1.0f / 70.0f);

    if (scale > 2.0f) {
        scale = 2.0f;
    } else if (scale < 0.8f) {
        scale = 0.8f;
    }
    return scale;
}

u32 evtCommand_EFFBED_START_EVO(void) {
    dds3Basic_t *unit;

    unit = evtSearchBasic(7, scrGetIntPara(0));
    if (unit != NULL) {
        s32 entryId = scrGetIntPara(1);
        s32 modelRecordIndex;
        if ((u32)entryId >= 3) {
            evtPrintDeveloperConsoleMessage(D_00421AF8, entryId);
            return 1;
        }
        dds3StartBEDEffectObjectBasic(unit, entryId);
        modelRecordIndex = scrGetIntPara(2);
        if (modelRecordIndex >= 0) {
            f32 value = evtComputeClampedModelScale(modelRecordIndex);
            struct effEveBEDCutHandle_t *target = ((EvtModelHeader *)unit->data)->target2C;
            if (target != NULL) {
                effEveBEDCutSetScale(target, value);
            }
        }
    }
    return 1;
}

u32 evtCommand_EFFMG1_LOAD(void) {
    const char *param0;
    dds3Basic_t *unit;

    param0 = scrGetStrPara(0);
    unit = dds3AddMG12EffectObjectBasic(1, param0);
    if (unit == NULL) {
        printf(D_00421B20, 1);
        printf(D_00421AC0, scrGetStrPara(0));
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(unit, 1);
        scrSetIntReturnValue(unit->key);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_00421AF8);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_00421B20);

const char D_00421B50[] = "EFFMG1_BE not fount RID = %d\n";

u32 evtCommand_EFFMG1_BE(void) {
    s32 taskId;
    s32 key;
    struct EffNodeDescriptor *descriptor;
    dds3Basic_t *object;

    taskId = scrGetIntPara(0);
    key = scrGetIntPara(1);
    descriptor = evtGetEventPackResourceAddress(taskId, key);
    if (descriptor == NULL) {
        key = scrGetIntPara(1);
        evtPrintDeveloperConsoleMessage(D_00421B50, key);
        scrSetIntReturnValue(0);
        return 1;
    }
    object = dds3AddMG12EffectObjectBasic2(1, descriptor);
    if (object == NULL) {
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(object, 1);
        scrSetIntReturnValue(object->key);
    }
    return 1;
}

u32 evtCommand_EFFMG2_LOAD(void) {
    const char *param0;
    dds3Basic_t *unit;

    param0 = scrGetStrPara(0);
    unit = dds3AddMG12EffectObjectBasic(2, param0);
    if (unit == NULL) {
        printf(D_00421B70, 1);
        printf(D_00421AC0, scrGetStrPara(0));
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(unit, 1);
        scrSetIntReturnValue(unit->key);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_00421B70);

const char D_00421BA0[] = "EFFMG2_BE not fount RID = %d\n";

u32 evtCommand_EFFMG2_BE(void) {
    s32 taskId;
    s32 key;
    struct EffNodeDescriptor *descriptor;
    dds3Basic_t *object;

    taskId = scrGetIntPara(0);
    key = scrGetIntPara(1);
    descriptor = evtGetEventPackResourceAddress(taskId, key);
    if (descriptor == NULL) {
        key = scrGetIntPara(1);
        evtPrintDeveloperConsoleMessage(D_00421BA0, key);
        scrSetIntReturnValue(0);
        return 1;
    }
    object = dds3AddMG12EffectObjectBasic2(2, descriptor);
    if (object == NULL) {
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(object, 1);
        scrSetIntReturnValue(object->key);
    }
    return 1;
}

u32 evtCommand_EFFMG2_PRESET(void) {
    s32 worldObjectKey;
    dds3Basic_t *obj;

    worldObjectKey = scrGetIntPara(0);
    obj = evtSearchBasic(7, worldObjectKey);
    if (obj != NULL) {
        dds3EffectObjectPresetMG12(obj);
    }
    return 1;
}

u32 evtCommand_EFFECT_PRESET(void) {
    s32 worldObjectKey;
    dds3Basic_t *obj;

    worldObjectKey = scrGetIntPara(0);
    obj = evtSearchBasic(7, worldObjectKey);
    if (obj != NULL) {
        dds3EffectObjectPresetMG12(obj);
    }
    return 1;
}

u32 evtCommand_EFFECT_CLS(void) {
    s32 worldObjectKey;
    dds3Basic_t *obj;

    worldObjectKey = scrGetIntPara(0);
    obj = evtSearchBasic(7, worldObjectKey);
    if (obj != NULL) {
        dds3DeleteSceneBasicData(obj);
    }
    return 1;
}

u32 evtCommand_EFFECT_POS_XYZ(void) {
    dds3Basic_t *obj;
    EvtModelHeader *header;

    obj = evtSearchBasic(7, scrGetIntPara(0));
    header = (EvtModelHeader *)obj->data;
    if (header->flags & 4) {
        header->positionX = scrGetFloatPara(1);
        header->positionY = scrGetFloatPara(2);
        header->positionZ = scrGetFloatPara(3);
    } else {
        obj->inner->position[0] = scrGetFloatPara(1);
        obj->inner->position[1] = scrGetFloatPara(2);
        obj->inner->position[2] = scrGetFloatPara(3);
        obj->inner->flags = (obj->inner->flags | OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) & ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
    }
    return 1;
}

u32 evtCommand_EFFECT_ROT_XYZ(void) {
    f32 v[4];
    dds3Basic_t *obj;
    f32 toRad;
    f32 x;
    f32 y;

    memset(v, 0, 0x10);
    v[3] = 1.0f;
    obj = evtSearchBasic(7, scrGetIntPara(0));
    if (!(((EvtModelHeader *)obj->data)->flags & 4)) {
        toRad = 0.017453293f;
        x = scrGetFloatPara(1) * toRad;
        y = scrGetFloatPara(2) * toRad;
        sdfConvertEulerAnglesToQuaternionVU(x, y, 0.0f);
        SDF_VU0_COPYVECTOR_A_TO_B();
        sdfConvertEulerAnglesToQuaternionVU(0.0f, 0.0f, scrGetFloatPara(3) * toRad);
        effMiscQuatMultiplyVU();
        VU0_STORE_VF($vf10, v);
        dds3SetRotPoseSet(obj, (u128 *)v);
    }
    return 1;
}

u32 evtCommand_EFFECT_POS(void) {
    dds3Basic_t *obj;
    dds3Basic_t *source;
    EvtWorldSourceTransformPrefix *vec;
    dds3PoseSet_t *params;

    obj = evtSearchBasic(7, scrGetIntPara(0));
    if (obj == NULL) {
        return 1;
    }
    source = evtSearchBasic(0x11, scrGetIntPara(1));
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
