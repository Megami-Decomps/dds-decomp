#include "common.h"
#include "evt_unit.h"
#include "eff_transform.h"
#include "mdl.h"
#include "sdf_draw.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"



typedef struct EvtTarget {
    u8 pad0[0x18];
    EvtTargetInfo *info; /* 0x18 */
    EvtEffData *data;
} EvtTarget;

extern f32 *D_00324770[];
extern u8 kwlnDefaultColorVector[];


extern void sdfStepWrappingFloatCounter(s32 path);
extern void dds3InterpolatePathVectorVU(s32 path);
extern void dds3PreparePathVectorPair(s32 path);
extern void effObjSetInnerFirstVec(void *obj, void *vec);
extern void effObjSetInnerSecondVec(void *obj, void *vec);
extern void func_002E7F20(f32, f32, f32);
extern void effMiscQuatMultiplyVU();
extern void effMiscQuaternionToMatrixVU(void);
extern void effObjAddInnerFirstVec(void *obj, void *vec);
extern void evtComputePlanarTargetDirectionVu(EvtUnit *unit);
s32 func_00222498(EvtUnit *unit, f32 *dir, f32 scale);
extern f32 evtGetValueScaleFactor(s32 path);
extern void evtScaleValueByMultiplier(s32 path, f32 multiplier);

void func_00221D00(EvtUnit *unit, s32 duration, u32 firstColor, u32 secondColor);

/* Length of the path's vec4 trajectory sampled at 20 steps of the value multiplier. */
f32 evtMeasurePathTrajectoryLength(s32 path) {
    f32 saved;
    f32 length = 0.0f;
    f32 step = 0.05f;
    f32 t = step;
    f32 segment;

    saved = evtGetValueScaleFactor(path);
    evtScaleValueByMultiplier(path, 0.0f);
    dds3InterpolatePathVectorVU(path);
    do {
        VU0_MOVE_VF(vf11, vf10);
        evtScaleValueByMultiplier(path, t);
        dds3InterpolatePathVectorVU(path);
        VU0_MOVE_VF(vf12, vf10);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(segment);
        length += segment;
        VU0_MOVE_VF(vf10, vf12);
        t += step;
    } while (t <= 1.0f);
    evtScaleValueByMultiplier(path, saved);
    return length;
}

extern u32 mdlGetBroadcastValue(MdlCtx *);
extern void mdlBroadcastMasked(MdlCtx *, u32);
extern f32 D_003BD358, D_003BD35C;
extern void *dds3GetWorldObject(void);
extern s32 dds3ContainsNodeInObjectChain(EffWorldNode *, s32, EffWorldNode *);
extern EffWorldNode *dds3FindWorldObjectNodeByKey(EffWorldNode *, u32, s32);
extern s32 sdfLoadMapRecordPositionVector(void *, s32);
extern void mdlLoadPrimaryVectorVU(MdlCtx *);
extern void func_00107DE8(void);
extern void effMiscQuaternionNlerpVU(f32);
extern s32 evtFindUnitSlotAuxCoordinates(EvtUnit *, f32 *, f32 *);
extern void sdfSetTextFloatPairOverride(void *, f32, f32);
extern void sdfClearTextFloatPairOverride(void *);

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
    auxFirst = D_003BD358;
    auxSecond = D_003BD35C;
    previousWeight = 0.0f;
    /* The model RGB and alpha tracks preserve the other packed channel. */
    flags = unit->flags;
    if (flags & 0x8000) {
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
            flags = unit->flags &= ~0x8000;
        } else {
            flags = unit->flags;
            unit->rgbElapsed++;
        }
    }
    if (flags & 0x10000) {
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
            flags = unit->flags &= ~0x10000;
        } else {
            flags = unit->flags;
            unit->alphaElapsed++;
        }
    }
    /* Stale world-node references must not reach the target-vector reads. */
    if (flags & 0x40000) {
        if (unit->currentTransitionValue == 0) {
            unit->flags = flags & ~0x40000;
        } else if (!dds3ContainsNodeInObjectChain(dds3GetWorldObject(), 9,
                                                 (EffWorldNode *)unit->currentTransitionValue)) {
            unit->currentTransitionValue = 0;
            unit->previousTransitionValue = 0;
            unit->flags &= ~0x40000;
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
        if (flags & 0x40000) {
            f32 distance;
            target = ((EvtTarget *)unit->currentTransitionValue)->info;
            if (unit->previousTransitionValue) {
                previous = ((EvtTarget *)unit->previousTransitionValue)->info;
            }
            if (!sdfLoadMapRecordPositionVector(unit->owner->inner, 0)) {
                mdlLoadPrimaryVectorVU(unit->owner);
            }
            VU0_STORE_VF_UNCLOBBERED(vf10, position);
            VU0_LOAD_VF(vf11, ((EvtTarget *)unit->currentTransitionValue)->data->position);
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
                VU0_LOAD_VF(vf11, ((EvtTarget *)unit->previousTransitionValue)->data->position);
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
            if (target->flags & 8) {
                VU0_STORE_VF_UNCLOBBERED(vf10, target->direction);
                flags = unit->flags;
                ownVector = 1;
            } else {
                if (unit->flags & 0x180000) {
                    if ((u16)unit->transitionFrameCount) {
                        transition = (f32)(u16)unit->transitionElapsed / (u16)unit->transitionFrameCount;
                    } else {
                        transition = 1.0f;
                    }
                    if (unit->flags & 0x100000) {
                        transition = 1.0f - transition;
                    }
                    if ((u16)unit->transitionFrameCount) {
                        if ((u16)unit->transitionFrameCount <= (u16)++unit->transitionElapsed) {
                            if (unit->flags & 0x100000) {
                                unit->currentTransitionValue = 0;
                                unit->flags &= ~0x40000;
                            }
                            unit->previousTransitionValue = 0;
                            unit->flags &= ~0x180000;
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
            VU0_LOAD_VF(vf11, D_00324770[0] + 4);
        }
        func_00107DE8();
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
            VU0_LOAD_VF(vf10, D_00324770[0] + 4);
        }
        VU0_STORE_VF_UNCLOBBERED(vf10, unit->vec40);
    }
    if (target && !ownVector) {
        factor = transition;
        VU0_LOAD_VF(vf10, unit->vec40);
        VU0_LOAD_VF(vf11, targetDirection);
        func_00107DE8();
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
            func_00107DE8();
            VU0_MOVE_VF(vf11, vf10);
            VU0_LOAD_VF(vf10, identity);
            effMiscQuaternionNlerpVU(factor);
            effMiscQuaternionToMatrixVU();
            VU0_LOAD_VF(vf10, unit->vec40);
            VU0_APPLY_MATRIX(vf10, vf10);
            VU0_STORE_VF_UNCLOBBERED(vf10, oldDirection);
            VU0_LOAD_VF(vf11, blendedDirection);
            func_00107DE8();
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
            VU0_LOAD_VF(vf10, D_00324770[0]);
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
                flags = unit->flags = originalFlags & ~0x300;
            }
            unit->flags = flags & ~0x1800;
        } else {
            unit->colorFramesRemaining--;
        }
    } else {
        u32 firstPacked;
        u32 secondPacked;
        if (!(originalFlags & 0x100)) {
            if (ownVector) {
                VU0_LOAD_VF(vf10, target);
            } else {
                VU0_LOAD_VF(vf10, D_00324770[0]);
            }
            EE_MMI_RGBA_PACK_UNIT(firstPacked, 128.0f);
            defaultFirst[0] = firstPacked;
            unit->color0C = defaultFirst[0];
        }
        if (!(originalFlags & 0x200)) {
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
            auxFirst = D_003BD358 * (1.0f - factor) + auxFirst * factor;
            auxSecond = D_003BD35C * (1.0f - factor) + auxSecond * factor;
        }
        if (previous) {
            f32 oldFirst = previous->auxFirst;
            f32 oldSecond = previous->auxSecond;
            if (oldFirst >= 1.0f) {
                factor = previousWeight;
                oldFirst = D_003BD358 * (1.0f - factor) + oldFirst * factor;
                oldSecond = D_003BD35C * (1.0f - factor) + oldSecond * factor;
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
        sdfSetTextFloatPairOverride(unit->owner->inner, auxFirst, auxSecond);
    } else {
        sdfClearTextFloatPairOverride(unit->owner->inner);
    }
}


/* vu0 routine: load vf10 with the unit's colour (flag 0x100), its target's vector, or the default */
void evtLoadUnitFirstColorVectorVU(EvtUnit *unit) {
    EvtTargetInfo *info = 0;
    s32 ownVector = 0;
    s32 color;
    f32 scale;

    if (unit->currentTransitionValue != 0 && (unit->flags & 0x40000)) {
        info = ((EvtTarget *)unit->currentTransitionValue)->info;
        if (info->flags & 0x8) {
            ownVector = 1;
        }
    }
    if (unit->flags & 0x100) {
        scale = 0.0078125f;
        color = unit->color;
        EE_MMI_RGBA_UNPACK(&color, scale);
    } else if (ownVector) {
                VU0_LOAD_VF(vf10, info);
    } else {
                VU0_LOAD_VF(vf10, D_00324770[0]);
    }
}

/* vu0 routine: as above for the second colour (flag 0x200) and the vectors at +0x40 */
void evtLoadUnitSecondColorVectorVU(EvtUnit *unit) {
    EvtTargetInfo *info = 0;
    s32 ownVector = 0;
    s32 color;
    f32 scale;

    if (unit->currentTransitionValue != 0 && (unit->flags & 0x40000)) {
        info = ((EvtTarget *)unit->currentTransitionValue)->info;
        if (info->flags & 0x8) {
            ownVector = 1;
        }
    }
    if (unit->flags & 0x200) {
        scale = 0.0078125f;
        color = unit->color50;
        EE_MMI_RGBA_UNPACK(&color, scale);
    } else if (ownVector) {
                VU0_LOAD_VF(vf10, (u8 *)info + 0x40);
    } else {
                VU0_LOAD_VF(vf10, kwlnDefaultColorVector);
    }
}

/* vu0 routine: load vf10 with the unit's vector, or the default one */
void evtLoadUnitDirectionVectorVU(EvtUnit *unit) {
    s32 ownVector = 0;

    if (unit->currentTransitionValue != 0 && (unit->flags & 0x40000)) {
        if (((EvtTarget *)unit->currentTransitionValue)->info->flags & 0x8) {
            ownVector = 1;
        }
    }
    if (unit->flags & 0x400) {
                VU0_LOAD_VF(vf10, unit->vec10);
    } else if (ownVector) {
                VU0_LOAD_VF(vf10, unit->vec10);
    } else {
                VU0_LOAD_VF(vf10, D_00324770[0] + 4);
    }
}


extern void func_002E1938(void *, SdfLightSources, f32 *);

/* Rebuild the unit's endpoint render work from its own colour/vector when the
 * value-change flag is set; otherwise defer to the matching-slot selector. */
void func_00221A80(EvtUnit *unit) {
    f32 ends[4][4];
    f32 color[4];
    SdfLightSources desc = { ends, 0, 0 };
    u32 packed0[4];
    u32 packed1[4];
    f32 scale0;
    f32 scale1;
    u32 flags = unit->flags;
    s32 i;

    if (flags & 0x20000) {
        return;
    }
    unit->value = 0;
    if (!(flags & 0x40000)) {
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
    for (i = 0; i < 3; i++) {
        if (color[i] > 1.0f) {
            color[i] = 1.0f;
        }
    }
    func_002E1938(unit->endpointWork, desc, color);
    unit->value = (u32)unit->endpointWork;
}

void evtSetUnitValueTransition(EvtUnit *unit, s32 value, s32 duration) {
    unit->flags |= 0x40000;
    unit->previousTransitionValue = unit->currentTransitionValue;
    unit->currentTransitionValue = value;
    if (duration == 0) {
        unit->previousTransitionValue = 0;
        unit->transitionElapsed = 0;
        unit->transitionDuration = 0;
        unit->flags &= ~0x180000;
    } else {
        unit->flags |= 0x80000;
        unit->flags &= ~0x100000;
        unit->transitionDuration = duration;
        unit->transitionElapsed = 0;
    }
}

void evtEndUnitValueTransition(EvtUnit *unit, s32 duration) {
    if (unit->currentTransitionValue != 0) {
        if (duration == 0) {
            unit->flags &= ~0x40000;
            unit->flags &= ~0x180000;
            unit->currentTransitionValue = 0;
        } else {
            unit->flags &= ~0x80000;
            unit->transitionDuration = duration;
            unit->flags |= 0x100000;
            unit->transitionElapsed = 0;
        }
    }
}

void evtUnitSetValueAndFlag(EvtUnit *unit, u32 value)
{
    unit->value = value;
    unit->flags = unit->flags | 0x20000;
}

void evtClearUnitValueChangeFlag(EvtUnit *unit) {
    unit->flags = unit->flags & ~0x20000;
    func_00221A80(unit);
}

typedef struct EvtUnitColorEndpoints {
    u8 pad00[4];
    u32 firstCurrent; /* 0x04 */
    u32 firstTarget;  /* 0x08 */
    u8 pad0C[0x48];
    u32 secondCurrent; /* 0x54 */
    u32 secondTarget;  /* 0x58 */
} EvtUnitColorEndpoints;

void func_00221D00(EvtUnit *unit, s32 duration, u32 firstColor, u32 secondColor) {
    s32 color1[4];
    s32 color2[4];
    u32 packed1;
    u32 packed2;
    union {
        f32 value;
        u32 bits;
    } scale;

    unit->unused150 = duration;
    ((EvtUnitColorEndpoints *)unit)->firstTarget = firstColor;
    ((EvtUnitColorEndpoints *)unit)->secondTarget = secondColor;
    scale.value = 128.0f;
    evtLoadUnitFirstColorVectorVU(unit);
    EE_MMI_RGBA_PACK_UNIT(packed1, scale.bits);
    color1[0] = packed1;
    ((EvtUnitColorEndpoints *)unit)->firstCurrent = packed1;
    evtLoadUnitSecondColorVectorVU(unit);
    EE_MMI_RGBA_PACK_UNIT(packed2, scale.bits);
    color2[0] = packed2;
    ((EvtUnitColorEndpoints *)unit)->secondCurrent = packed2;
    unit->flags = (unit->flags | 0x800) & ~0x1000;
}

/* vu0 routine: normalize the direction in vf10, store it to vec30, then vf10 from evtLoadUnitDirectionVectorVU to vec20 */
void evtSetUnitNormalizedDirection(EvtUnit *unit, s32 arg) {
    unit->directionMode = arg;
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
        unit->flags &= ~0x8000;
    } else {
        u32 currentRgb = mdlGetBroadcastValue((MdlCtx *)unit->owner) & 0xFFFFFF;

        unit->color60 = (unit->color60 & 0xFF000000) | currentRgb;
        unit->color64 = (unit->color64 & 0xFF000000) | (color & 0xFFFFFF);
        unit->flags |= 0x8000;
    }
}

void evtSetUnitAlphaTransition(EvtUnit *unit, s32 duration, u32 color) {
    unit->alphaDuration = duration;
    unit->alphaElapsed = 0;
    if (duration == 0) {
        mdlBroadcastMasked((MdlCtx *)unit->owner,
            (mdlGetBroadcastValue((MdlCtx *)unit->owner) & 0xFFFFFF) | (color & 0xFF000000));
        unit->color60 = (unit->color60 & 0xFFFFFF) | (color & 0xFF000000);
        unit->flags &= ~0x10000;
    } else {
        u32 currentAlpha = mdlGetBroadcastValue((MdlCtx *)unit->owner) & 0xFF000000;

        unit->color60 = (unit->color60 & 0xFFFFFF) | currentAlpha;
        unit->color64 = (unit->color64 & 0xFFFFFF) | (color & 0xFF000000);
        unit->flags |= 0x10000;
    }
}

u8 evtTestUnitStatusFlags(EvtUnit *unit)
{
    return (unit->flags & 0x7800) != 0;
}

void evtSetUnitStatusFlags(EvtUnit *unit)
{
    unit->flags = unit->flags | 0x300;
}

void evtConfigureUnitTransition(EvtUnit *unit, s32 arg) {
    if (arg == 0) {
        unit->flags &= ~0x700;
        unit->flags &= ~0x5000;
        unit->flags &= ~0x2800;
    } else {
        func_00221D00(unit, arg, 0, 0);
        unit->flags = (unit->flags & ~0x800) | 0x1000;
                VU0_MOVE_VF(vf10, vf0);
        evtSetUnitNormalizedDirection(unit, arg);
        unit->flags = (unit->flags & ~0x2000) | 0x4000;
    }
}

EvtUnit *evtGetWorldUnitNestedValue(s32 id) {
    EffWorldNode *obj = dds3FindWorldObjectNodeByKey(dds3GetWorldObject(), id, 5);

    if (obj != NULL) {
        return (EvtUnit *)*(s32 *)((u8 *)obj->data + 8);
    }
    return NULL;
}

EvtUnit *evtUnitGetNestedValue(EffWorldNode *obj) {
    if (obj == NULL) {
        return NULL;
    }
    return (EvtUnit *)*(s32 *)((u8 *)obj->data + 8);
}

extern void *sdfAllocSizeClassBlock(s32 size);
extern void sdfReleaseChipBlock(void *block);
extern const s32 D_003AC060[];

EvtUnit *evtCreateUnitTransitionWork(EvtEffObj *effObj, MdlCtx *owner) {
    EvtUnit *work;
    void *endpoint;
    f32 defaultVector[4];

    memcpy(defaultVector, (const f32 *)D_003AC060, sizeof(defaultVector));
    work = sdfAllocSizeClassBlock(sizeof(EvtUnit));
    memset(work, 0, sizeof(EvtUnit));
    work->motionState = EVT_UNIT_MOTION_STATE_IDLE;
    work->transitionSourceKind = 1;
    work->unkB8 = 1.0f;
    work->effObj = effObj;
    work->owner = owner;
    work->unkC0 = 10;
    work->unkC8 = 10;
    work->unkBE = 0;
    work->unkC6 = 0;

    VU0_LOAD_VF(vf10, defaultVector);
    VU0_STORE_VF_UNCLOBBERED(vf10, work->vec10);
    VU0_STORE_VF_UNCLOBBERED(vf10, work->vec40);
    work->color0C = 0x00B2B2B2;
    work->color = 0x00B2B2B2;
    work->color5C = 0x80303030;
    work->color50 = 0x80303030;
    endpoint = sdfAllocSizeClassBlock(0xE0);
    work->endpointWork = endpoint;
    memset(endpoint, 0, 0xE0);
    work->value = 0;
    *(u32 *)((u8 *)work + 0xD8) = 0;
    *(u32 *)((u8 *)work + 0xDC) = 0;
    return work;
}

s32 evtReleaseUnitTransitionWork(EvtUnit *work) {
    void *endpointWork;

    if (work == NULL) {
        return 1;
    }
    endpointWork = work->endpointWork;
    work->owner->inner->lighting = 0;
    if (endpointWork != NULL) {
        sdfReleaseChipBlock(endpointWork);
        work->endpointWork = NULL;
    }
    if (work->pathHandle != 0) {
        dds3FreePathObject(work->pathHandle);
        work->pathHandle = 0;
    }
    sdfReleaseChipBlock(work);
    return 1;
}

s32 evtGetUnitMotionState(EvtUnit *unit) {
    return unit->motionState;
}

/* Callers pass the promoted full-width value (one reads a byte table field);
 * only the low halfword is stored. */
void evtUnitSetStoredParameter(EvtUnit *unit, s32 value) {
    unit->unkBC = value;
}

void evtSetTransitionMotionScale(EvtUnit *work, f32 value) {
    work->unkB8 = value;
}

void evtStoreUnitMotionShortParameters(EvtUnit *work, s32 a, s32 b) {
    work->unkBE = a;
    work->unkC0 = b;
}

s32 evtIsUnitMotionIdleOrTimedMode(EvtUnit *unit) {
    s32 state = evtGetUnitMotionState(unit);

    if (state == EVT_UNIT_MOTION_STATE_IDLE) {
        return 1;
    }
    if (state == EVT_UNIT_MOTION_STATE_MOTION && unit->motionTicks > 0 &&
        unit->owner->first->state == 5) {
        return 1;
    }
    return 0;
}

void evtStoreUnitMotionSlotSelection(EvtUnit *work, s32 a, s32 b) {
    work->firstSlot = a;
    work->secondSlot = b;
}

void evtActivateStoredUnitMotionSlot(EvtUnit *work) {
    evtConfigureUnitMotionSlot(work, work->firstSlot, work->secondSlot, 0, 0, 2);
}

void evtConfigureUnitMotionSlot(EvtUnit *unit, s32 slot, s32 a, s32 b, s32 c, s32 mode) {
    unit->slotFlags[slot] = 3;
    unit->slotA[slot] = a;
    unit->slotB[slot] = b;
    unit->slotC[slot] = c;
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

void evtPrepareUnitMotionState(EvtUnit *unit, s32 a, s32 b, s32 c, s32 mode) {
    unit->flags &= ~0x1;
    unit->flags &= ~0x20;
    unit->flags &= ~0x40;
    unit->flags &= ~0x400000;
    unit->flags &= ~0x800000;
    unit->motionState = EVT_UNIT_MOTION_STATE_MOTION;
    unit->unkC4 = a;
    unit->unkC6 = b;
    unit->unkC8 = c;
    unit->flags |= 0x80;
    unit->motionTicks = 0;
    unit->unk94 = 0;
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

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00222498);

/* Flat, negated direction of the rotation in quat, offset by the effect object's point, aimed with func_00222498. */
s32 evtAimUnitFromFlatQuaternion(EvtUnit *unit, f32 *quat, f32 angle) {
    f32 v[4];

    VU0_LOAD_VF(vf10, quat);
    effMiscQuaternionToMatrixVU();
    VU0_MOVE_VF(vf10, vf30);
    VU0_SET_AXIS_CLEAR_W(0.0f, y);
    VU0_NORMALIZE_VF10();
    VU0_SCALAR_OP(-1.0f, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_LOAD_VF(vf11, unit->effObj->data->position);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, v);
    return func_00222498(unit, v, angle);
}

s32 evtApplyUnitDirectionOffset(EvtUnit *unit) {
    f32 v[4];
    f32 scale;
    EvtEffObj *obj;

    func_00222498(unit, unit->targetVector, unit->directionOffset * 0.01f);
    if (unit->directionOffset != 0) {
        evtComputePlanarTargetDirectionVu(unit);
        obj = unit->effObj;
    } else {
        VU0_LOAD_VF(vf10, unit->targetVector);
        obj = unit->effObj;
        VU0_LOAD_VF(vf11, obj->data->position);
        VU0_SUB(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10();
    }
    scale = unit->motionParameter * 0.1f;
    VU0_SCALAR_OP(scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_STORE_VF(vf10, v);
    effObjAddInnerFirstVec(obj, v);
    return 1;
}


extern s32 func_00220678(EvtUnit *unit);
extern void func_003003F0(const char *fmt, ...);

/* Run a copy of the unit until func_00220678 reports done, and derive its per-step Y speed. */
INCLUDE_RODATA(const s32, "event/evtUnitManager", D_003AC060);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_003AC070);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_003AC080);

s32 evtUnitPrepareVerticalMoveSteps(EvtUnit *unit) {
    EvtUnit copy;
    f32 delta[4];
    f32 savedA[4];
    f32 savedB[4];
    s32 count = 0;
    s32 i;
    for (i = 0; i < 4; i++) {
        savedA[i] = unit->effObj->data->position[i];
        savedB[i] = unit->effObj->data->orientation[i];
    }
    copy = *unit;
    while (func_00220678(&copy) == 0) {
        count++;
        evtApplyUnitDirectionOffset(&copy);
    }
    for (i = 0; i < 4; i++) {
        unit->effObj->data->position[i] = savedA[i];
        unit->effObj->data->orientation[i] = savedB[i];
    }
    if (count == 0) {
        func_003003F0("ymove frameno = 0\n");
        unit->stepCount = 0;
        unit->speedY = 0;
        return 0;
    }
    VU0_LOAD_VF(vf10, unit->targetVector);
    VU0_LOAD_VF(vf11, unit->effObj->data->position);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, delta);
    unit->stepCount = count;
    unit->speedY = delta[1] / (f32)count;
    func_003003F0("ymove frameno = %d addvalue = %f total yzahyo=%f\n", count, unit->speedY, delta[1]);
    return count;
}

s32 evtUnitApplyPathVectors(EvtUnit *unit) {
    f32 v[4];

    sdfStepWrappingFloatCounter(unit->pathHandle);
    dds3InterpolatePathVectorVU(unit->pathHandle);
    VU0_STORE_VF($vf10, v);
    effObjSetInnerFirstVec(unit->effObj, v);
    if (unit->flags & 0x10) {
        dds3PreparePathVectorPair(unit->pathHandle);
        VU0_MOVE_VF(vf11, vf10);
        func_002E7F20(0.0f, 3.14159265f, 0.0f);
        effMiscQuatMultiplyVU();
        VU0_STORE_VF($vf10, v);
        effObjSetInnerSecondVec(unit->effObj, v);
    }
    return 1;
}

void evtCopyUnitTargetVector(EvtUnit *work, void *src) {
    PCP_COPY_VECTOR(work->targetVector, src);
}
