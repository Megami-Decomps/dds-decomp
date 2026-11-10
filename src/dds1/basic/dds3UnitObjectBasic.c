#include "common.h"
#include "dds3obj.h"
#include "dds3_object_control_slot.h"
#include "eff_object.h"
#include "eff_transform.h"
#include "sdk_ps2.h"
#include "mdl.h"
#include "sdf_map_position_api.h"
#include "ps2asm.h"
#include "sdf_vu_matrix.h"
#include "sdf_trig.h"
#include "sdf_model.h"
#include "sdf_chunk.h"
#include "sdf_chip.h"
#include "dds3_basic_array_cursor.h"
#include "dds3_object_flags.h"
#include "eff_dependency.h"
#include "evt_world_source_transform.h"
#include "evt_unit.h"
#include "evt_unit_motion_step_result.h"
#include "eff.h"
#include "eff_node.h"
#include "eff_event.h"
#include "eff_event_sound.h"
#include "btl_sound.h"
#include "bill_object_api.h"
#include "effect_value_object.h"
#include "fld_actor_motion_step_result_api.h"

dds3ObjectBaseControlWorkData_t *dds3UnitObjectBasicGetControlWorkData(dds3Basic_t *object) {
    return ((EffectObjectData *)object->workdata)->modelHolder;
}

extern void dds3SetRadiusPoseSet(dds3PoseSet_t *node);
extern dds3Basic_t *dds3GetFirstWorldObjectNodeOfKind2(void);
extern s32 dds3ObjectBaseRegistArea(dds3Basic_t *object);

dds3Basic_t *dds3AddUnitObjectBasic(s32 worldId, u128 *position, u128 *rotation) {
    dds3Basic_t *obj = dds3AddSceneBasicData(EFF_WORLD_KIND_FOLLOW_MODEL);
    EffectObjectData *data;

    if (obj != NULL) {
        obj->key = worldId;
        dds3ObjectBaseCreateMover(obj);
        dds3SetPosPoseSet(obj, position);
        dds3SetRotPoseSet(obj, rotation);
        dds3SetRadiusPoseSet(obj->poseset);
        data = ((EffectObjectData *)obj->workdata);
        data->handle = (dds3ObjectBaseControlWorkData_t *)-1;
        data->word04 = 0;
        dds3ObjectBaseSetControlBasicType(
            obj, dds3GetFirstWorldObjectNodeOfKind2(), DDS3_OBJECT_HANDLER_SLOT);
        dds3ObjectBaseRegistArea(obj);
        return obj;
    }
    return NULL;
}

void dds3DeleteUnitObjectBasic(dds3Basic_t *unitObject) {
    dds3DeleteSceneBasicData(unitObject);
}

extern void dds3ObjectBaseFreeModelHandle(dds3Basic_t *object);

#include "dds3_world_secondary.h"
extern void dds3FreePoseSet(dds3Basic_t *node);
extern void dds3UnitObjectBasicEL_LinkFree(dds3Basic_t *object);

u32 dds3GetStatusUnitObjectBasic(dds3Basic_t *obj) {
    return (u32)((EffectObjectData *)obj->workdata)->handle;
}

evtUnitHandle_t *dds3GetUnitHandleObjectBasic(dds3Basic_t *object) {
    return ((EffectObjectData *)object->workdata)->transitionWork;
}

void dds3UnitObjectBasicSetOt(dds3Basic_t *obj, s32 value) {
    ((EffectObjectData *)obj->workdata)->followParameterIndex = value;
}

/* Return the signed shortest turn from one degree angle to another.
 * The cast before modulo intentionally discards fractional degrees. */
f32 dds3ShortestAngleDelta(f32 fromDegrees, f32 toDegrees) {
    f32 diff;

    if (fromDegrees < 0.0f || toDegrees < 0.0f) {
        fromDegrees += 360.0f;
        toDegrees += 360.0f;
    }
    fromDegrees = (s32)fromDegrees % 360;
    toDegrees = (s32)toDegrees % 360;
    diff = fromDegrees - toDegrees;
    if (diff > 180.0f || diff < -180.0f) {
        if (fromDegrees < toDegrees) {
            fromDegrees += 360.0f;
        } else {
            toDegrees += 360.0f;
        }
    }
    return toDegrees - fromDegrees;
}

extern f32 EulerAngleAxisY(void);

/* The first conversion follows the original 180 / 3.14 approximation; the
 * quaternion helper uses the more precise radians-to-degrees factor. */
#define EFFECT_HEADING_DEGREES_PER_RADIAN_APPROX 57.32484055f
#define EFFECT_HEADING_RADIANS_TO_DEGREES 57.29577637f

void effObjStepFollowAngleTowardPosition(dds3Basic_t *obj, const f32 *targetPosition) {
    EffectObjectData *data = obj->workdata;
    f32 position[4];
    f32 currentAngle = data->angle;
    f32 heading;
    f32 targetAngle;
    f32 referenceAngle;
    f32 angleDelta;
    f32 step;

    dds3GetPosPoseSet(obj);
    SDF_VU0_STOREVECTOR(position);
    if (position[0] == targetPosition[0] &&
        position[2] == targetPosition[2]) {
        return;
    }

    heading = -(sdfAtan2(position[0] - targetPosition[0],
                         position[2] - targetPosition[2]) *
                EFFECT_HEADING_DEGREES_PER_RADIAN_APPROX);
    dds3GetRotPoseSet(obj);
    referenceAngle = EulerAngleAxisY();
    referenceAngle *= EFFECT_HEADING_RADIANS_TO_DEGREES;
    targetAngle = dds3ShortestAngleDelta(referenceAngle, heading);

    if (targetAngle < data->limitMin2C) {
        targetAngle = data->limitMin2C;
    }
    if (data->limitMax30 < targetAngle) {
        targetAngle = data->limitMax30;
    }

    angleDelta = dds3ShortestAngleDelta(currentAngle, targetAngle);
    step = 3.0f;
    if ((0.0f <= angleDelta && angleDelta <= step) ||
        (angleDelta <= 0.0f && -step <= angleDelta)) {
        angleDelta = targetAngle;
    } else if (angleDelta < 0.0f) {
        angleDelta = currentAngle - step;
    } else {
        angleDelta = currentAngle + step;
    }
    data->angle = angleDelta;
    data->word34 = 0;
}

void effObjStepFollowAngleTowardZero(dds3Basic_t *object) {
    EffectObjectData *data = object->workdata;
    f32 angle;
    f32 value;
    f32 step;

    if (data->angleReturnDelayFrames > 0) {
        data->angleReturnDelayFrames--;
    } else {
        angle = data->angle;
        if (angle < 0.1f && -0.1f < angle) {
            dds3ObjectBaseOffFlag(object, 0x2000);
        } else {
            value = dds3ShortestAngleDelta(angle, 0.0f);
            step = 1.0f;
            if ((0.0f <= value && value <= step) ||
                (value <= 0.0f && -step <= value)) {
                value = 0.0f;
            } else {
                if (value < 0.0f) {
                    value = angle - step;
                } else {
                    value = angle + step;
                }
            }
            data->angle = value;
            data->word34 = 0;
        }
    }
}

void evtArmEffectObjectPendingValue(dds3Basic_t *obj, s32 value) {
    EffectObjectData *data;

    data = obj->workdata;
    dds3ObjectBaseOnFlag(obj, 0x2000);
    data->pendingTargetKey = value;
    data->angleReturnDelayFrames = 0;
}

void evtResetObjectPendingValue(dds3Basic_t *obj) {
    EffectObjectData *data;

    data = obj->workdata;
    data->angleReturnDelayFrames = 0x1e;
    data->pendingTargetKey = 0;
}

s32 effObjInitializeFollowModelData(dds3Basic_t *object) {
    EffectObjectData *data;
    void *work;

    dds3InitPoseSet(object);
    work = sdfChipAllocDirect(sizeof(EffectObjectData));
    object->workdata = work;
    memset(work, 0, sizeof(EffectObjectData));
    data = object->workdata;
    data->modelHolder = dds3ObjectBaseInitControlWorkData(object);
    data->transitionWork = NULL;
    data->activeId = -1;
    data->followParameterIndex = -1;
    data->pendingTargetKey = 0;
    data->angleReturnDelayFrames = 0;
    data->angle = 0.0f;
    data->limitMin2C = -45.0f;
    data->limitMax30 = 45.0f;
    data->word34 = 0;
    data->limitMin38 = -45.0f;
    data->limitMax3C = 45.0f;
    dds3ObjectBaseOnFlag(object, 0x42);
    return 1;
}

extern void dds3ObjectBaseFreeControlWorkData(dds3ObjectBaseControlWorkData_t *base);

void dds3UnitObjectBasicDestructor(dds3Basic_t *obj) {
    EffectObjectData *data;

    dds3UnitObjectBasicEL_LinkFree(obj);
    dds3FreePoseSet(obj);
    data = obj->workdata;
    if (data->handle != -1) {
        data->handle = -1;
    }
    if (data->transitionWork != 0) {
        evtDeleteUnitHandle(data->transitionWork);
        data->transitionWork = 0;
    }
    dds3ObjectBaseFreeModelHandle(obj);
    dds3ObjectBaseFreeControlWorkData(data->modelHolder);
    sdfChipFreeDirect(obj->workdata);
}

typedef struct EffVec4 {
    f32 v[4];
} EffVec4;

extern EffVec4 D_0039F720;
extern EffVec4 D_0039F730;
extern u8 dds3CheckPoseSetFlag(dds3PoseSet_t *node, u32 flags);
extern void dds3BackupPoseSet(dds3PoseSet_t *transform);
extern EvtUnitMotionStepResult evtUpdateUnitMotion(dds3PoseSet_t *, dds3Basic_t *);

extern void dds3OffPoseSetFlag(dds3PoseSet_t *node, u32 flags);
extern void Normalize(void);
extern void FromRotation(f32);
extern void dds3SetRadiusPoseSet(dds3PoseSet_t *inner);
extern void dds3UnitObjectBasicEL_Link(dds3Basic_t *obj);
extern void effObjStepFollowAngleTowardZero(dds3Basic_t *object);
extern void effObjStepFollowAngleTowardPosition(dds3Basic_t *obj, const f32 *targetPosition);
extern void *dds3GetWorldNodeData(dds3Basic_t *node);

/* Per-frame refresh of a model effect object: rebuild the child transform from the follow record (a tilt that wobbles with its angle), then run the timed callbacks. */
s32 effUpdateFollowModelTransform(dds3Basic_t *obj) {
    EffVec4 axis = D_0039F730;
    dds3PoseSet_t node;
    EffectObjectData *data;
    void *model;
    s32 flag;

    data = obj->workdata;
    flag = 0;
    if (data->transitionWork != 0) {
        dds3BackupPoseSet(&node);
        if (evtUpdateUnitMotion(&node, obj) != EVT_UNIT_MOTION_STEP_NOT_APPLIED) {
            dds3GetSclPoseSetMCR(obj, (u128 *)node.scale);
            dds3AddRotPoseSet(obj, (u128 *)node.rotation);
            dds3GetPosPoseSetMCR(obj, (u128 *)node.pos);
        }
    }
    model = (void *)data->modelHolder->resourceHandle;
    if (model != NULL && dds3CheckPoseSetFlag(obj->poseset, OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) == 1) {
        dds3OffPoseSetFlag(obj->poseset, OBJECT_TRANSFORM_FLAG_UPDATE_PENDING);
        if (data->transitionWork != 0) {
            if (data->transitionWork->motionState == EVT_UNIT_MOTION_STATE_IDLE ||
                data->transitionWork->motionState == EVT_UNIT_MOTION_STATE_VECTOR) {
                if (data->transitionWork->flags & 0x40) {
                    flag = 1;
                }
            }
            if (data->transitionWork->flags & 0x400000) {
                flag = 1;
            }
        }
        SDF_VU0_LOADVECTOR(&obj->poseset->scale[0]);
        if (data->transitionWork != 0 && flag != 0) {
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_LOADIDENTITYVECTOR();
            SDF_VU0_VECTOR_W_SET_ZERO();
            SDF_VU0_SETVECTOR_X(sdfSin(data->transitionWork->wobblePhase) * 0.01f);
            SDF_VU0_SETVECTOR_Y(sdfSin(data->transitionWork->wobblePhase) * 0.004f);
            SDF_VU0_MULVECTOR();
            SDF_VU0_ADDVECTOR();
        }
        mdlSetScale(model);
        SDF_VU0_LOADVECTOR(&obj->poseset->rotation[0]);
        Normalize();
        if (data->transitionWork != 0 && flag != 0) {
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_LOADVECTOR(&axis);
            FromRotation(sdfSin(data->transitionWork->wobblePhase) * 0.006f);
            Mult();
        }
        if (data->word18 & 1) {
            SDF_VU0_LOADVECTOR_B(&D_0039F720);
            Mult();
        }
        mdlSetRotate(model);
        if (dds3CheckPoseSetFlag(obj->poseset, OBJECT_TRANSFORM_FLAG_USE_SMOOTHED_POSITION)) {
            SDF_VU0_LOADVECTOR(&obj->poseset->smoothedPosition[0]);
            SDF_VU0_VECTOR_W_SET_ONE();
        } else {
            SDF_VU0_LOADVECTOR(&obj->poseset->pos[0]);
            SDF_VU0_VECTOR_W_SET_ONE();
        }
        mdlSetPosition(model);
        dds3SetRadiusPoseSet(obj->poseset);
    }
    if (!dds3ObjectBaseCheckFlag(obj, 0x100)) {
        dds3UnitObjectBasicEL_Link(obj);
    }
    if (dds3ObjectBaseCheckFlag(obj, 0x2000)) {
        if (data->pendingTargetKey != 0) {
            effObjStepFollowAngleTowardPosition(
                obj, &((EvtWorldSourceTransformPrefix *)dds3GetWorldNodeData(dds3SearchSceneBasic(
                    dds3GetControlScene(), data->pendingTargetKey, 0x11)))->position.x);
        } else {
            effObjStepFollowAngleTowardZero(obj);
        }
    }
    return 1;
}

extern void effObjApplyTransformParameters(sdfGrOt_t **, dds3Basic_t *);
extern void fldDrawEffectObjectShadow(dds3Basic_t *);
extern void dds3DrawUnitObjectShadow(f32, f32, f32, f32, s32);
extern sdfGrOt_t *sdfModelOtTable[13][4];

s32 dds3UnitObjectBasicDrawProcedure(dds3Basic_t *obj) {
    f32 vec[4];
    evtUnitHandle_t *target;
    dds3ModelHandle_t *config;
    s32 level;
    s32 pickMode;

    memset(vec, 0, 0x10);
    vec[3] = 1.0f;
    if (dds3ObjectBaseCheckFlag(obj, 1)) {
        return 1;
    }
    target = dds3GetUnitHandleObjectBasic(obj);
    if (dds3ObjectBaseCheckFlag(obj, 0x200) && target != NULL && !(target->owner->flags & MDL_SKIP_TRANSFORMS)) {
        fldDrawEffectObjectShadow(obj);
    }
    if (((EffectObjectData *)obj->workdata)->followParameterIndex == -1) {
        effObjApplyTransformParameters(sdfModelOtTable[0], obj);
    } else {
        effObjApplyTransformParameters(sdfModelOtTable[((EffectObjectData *)obj->workdata)->followParameterIndex], obj);
    }
    if (!dds3ObjectBaseCheckFlag(obj, 0x400)) {
        return 1;
    }
    if (dds3ObjectBaseCheckFlag(obj, 0x1000)) {
        return 1;
    }
    if (target == NULL) {
        return 1;
    }
    config = (dds3ModelHandle_t *)dds3UnitObjectBasicGetControlWorkData(obj)->resourceHandle;
    if (dds3ObjectBaseCheckFlag(obj, 0x4000)) {
        level = target->shadowLevel;
    } else {
        level = config->inner->color >> 24;
    }
    pickMode = dds3ObjectBaseCheckFlag(obj, 0x8000) != 0;
    if (sdfModelGetMarkPos(target->owner->inner, 0)) {
        SDF_VU0_STOREVECTOR(vec);
        vec[1] = pickMode == 1 ? target->shadowY : obj->poseset->pos[1];
        dds3DrawUnitObjectShadow(vec[0], vec[1], vec[2], obj->poseset->radius, level);
    } else {
        if (dds3CheckPoseSetFlag(obj->poseset, OBJECT_TRANSFORM_FLAG_USE_SMOOTHED_POSITION)) {
            vec[0] = obj->poseset->smoothedPosition[0];
            vec[1] = obj->poseset->smoothedPosition[1];
            vec[2] = obj->poseset->smoothedPosition[2];
        } else {
            vec[0] = obj->poseset->pos[0];
            vec[1] = obj->poseset->pos[1];
            vec[2] = obj->poseset->pos[2];
        }
        if (pickMode == 1) {
            vec[1] = target->shadowY;
        }
        dds3DrawUnitObjectShadow(vec[0], vec[1], vec[2], obj->poseset->radius, level);
    }
    return 1;
}

extern void dds3UnlinkLightObjectFromUnitObject(dds3Basic_t *, s32);
extern void dds3LinkLightObjectToUnitObject(EffectValueObject *value, dds3Basic_t *object, s32 duration);

void dds3UnitObjectBasicEL_LinkFree(dds3Basic_t *object) {
    EffectObjectData *data;

    data = object->workdata;
    if (data->activeId != -1) {
        dds3UnlinkLightObjectFromUnitObject(object, 10);
        data->activeId = 0xffffffff;
    }
}

struct WorldIndexNode;
extern s32 dds3ComparePosPoseSet2(f32 *, f32 *);
void dds3UnitObjectBasicEL_Link(dds3Basic_t *obj) {
    EffectObjectData *data = obj->workdata;
    dds3Basic_t *world = dds3GetControlScene();
    struct WorldIndexNode *list;
    EffectValueObject *other;
    EffectValueData *otherData;

    if (world == 0) {
        return;
    }
    list = dds3SceneBasicGetList(world, 9);
    if (list == NULL) {
        return;
    }
    if (dds3GetBasicArrayCount((dds3BasicArrayHandle_t *)list) == 0) {
        dds3FreeBasicArrayHandle(list);
        return;
    }
    if (dds3SetFirstBasicArray((dds3BasicArrayHandle_t *)list) != DDS3_BASIC_ARRAY_CURSOR_EXHAUSTED) {
        do {
            other = (EffectValueObject *)dds3GetBasicArray((dds3BasicArrayHandle_t *)list);
            otherData = other->data;
            if (!(otherData->flags & 4) &&
                dds3ComparePosPoseSet2(((f32 *)obj->poseset), other->source) == 0 &&
                data->activeId == other->valueId) {
                dds3UnlinkLightObjectFromUnitObject(obj, 10);
                data->activeId = -1;
            }
        } while (dds3SetNextBasicArray((dds3BasicArrayHandle_t *)list) != DDS3_BASIC_ARRAY_CURSOR_EXHAUSTED);
    }
    dds3FreeBasicArrayHandle(list);

    if (data->activeId == -1) {
        list = dds3SceneBasicGetList(world, 9);
        if (dds3SetFirstBasicArray((dds3BasicArrayHandle_t *)list) == DDS3_BASIC_ARRAY_CURSOR_EXHAUSTED) {
            goto destroy_list;
        }
        do {
            other = (EffectValueObject *)dds3GetBasicArray((dds3BasicArrayHandle_t *)list);
            otherData = other->data;
            if (!(otherData->flags & 4) &&
                dds3ComparePosPoseSet2(((f32 *)obj->poseset), other->source) != 0) {
                goto attach_transition;
            }
        } while (dds3SetNextBasicArray((dds3BasicArrayHandle_t *)list) != DDS3_BASIC_ARRAY_CURSOR_EXHAUSTED);

destroy_list:
        dds3FreeBasicArrayHandle(list);
        return;

attach_transition:
        dds3LinkLightObjectToUnitObject(other, obj, 10);
        data->activeId = other->valueId;
        dds3FreeBasicArrayHandle(list);
    }
}

INCLUDE_RODATA(const s32, "basic/dds3UnitObjectBasic", D_0039F720);

INCLUDE_RODATA(const s32, "basic/dds3UnitObjectBasic", D_0039F730);

