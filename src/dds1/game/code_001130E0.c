#include "sdk_ps2.h"
#include "mdl.h"
#include "common.h"
#include "sdf_vu0.h"
#include "sdf_vu_matrix.h"
#include "sdf_trig.h"
#include "sdf_model.h"
#include "sdf_chunk.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "dds3_object_flags.h"
#include "eff_dependency.h"
#include "pcp_vu0.h"
#include "eff_object.h"
#include "evt_world_source_transform.h"
#include "evt_unit.h"
#include "eff.h"
#include "eff_node.h"
#include "eff_event.h"
#include "eff_event_sound.h"
#include "btl_sound.h"
#include "bill_object_api.h"
#include "effect_value_object.h"
#include "fld_actor_motion_step_result_api.h"

extern void dds3ObjectBaseFreeModelHandle(dds3Basic_t *object);

extern u32 effObjOpacityPassEnabled;

/* Mode 0 is the initialized neutral tint; other values describe the native
 * alpha updates performed by the paired transform renderer. */
enum {
    EFFECT_OPACITY_MODE_INITIAL = 0,
    EFFECT_OPACITY_MODE_ALPHA_CLEAR = 2,
    EFFECT_OPACITY_MODE_ALPHA_RISE_8 = 3,
    EFFECT_OPACITY_MODE_ALPHA_FALL_8_TO_32 = 4,
    EFFECT_OPACITY_MODE_ALPHA_RISE_4 = 5,
    EFFECT_OPACITY_MODE_ALPHA_FALL_4 = 6,
    EFFECT_OPACITY_MODE_ALPHA_RISE_2 = 7,
    EFFECT_OPACITY_MODE_ALPHA_FALL_2 = 8,
    EFFECT_OPACITY_MODE_ALPHA_FALL_11 = 9,
    EFFECT_OPACITY_MODE_ALPHA_FADE_START = EFFECT_OPACITY_MODE_ALPHA_RISE_8,
    EFFECT_OPACITY_MODE_ALPHA_FADE_END = EFFECT_OPACITY_MODE_ALPHA_RISE_4,
    EFFECT_OPACITY_ALPHA_MAX = 0x80,
    EFFECT_OPACITY_FALL_8_FLOOR = 0x20,
    EFFECT_OPACITY_COLOR_NEUTRAL = 0x80808080,
    EFFECT_OPACITY_COLOR_TRANSPARENT = 0x00808080
};
#include "dds3_world_secondary.h"
extern void dds3FreePoseSet(dds3Basic_t *node);
extern void dds3UnitObjectBasicEL_LinkFree(dds3Basic_t *object);

u32 dds3GetStatusUnitObjectBasic(dds3Basic_t *obj) {
    return (u32)((EffectObjectData *)obj->data)->handle;
}

evtUnitHandle_t *dds3GetUnitHandleObjectBasic(dds3Basic_t *object) {
    return ((EffectObjectData *)object->data)->transitionWork;
}

void dds3UnitObjectBasicSetOt(dds3Basic_t *obj, s32 value) {
    ((EffectObjectData *)obj->data)->followParameterIndex = value;
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
    EffectObjectData *data = obj->data;
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
    EffectObjectData *data = object->data;
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

    data = obj->data;
    dds3ObjectBaseOnFlag(obj, 0x2000);
    data->pendingTargetKey = value;
    data->angleReturnDelayFrames = 0;
}

void evtResetObjectPendingValue(dds3Basic_t *obj) {
    EffectObjectData *data;

    data = obj->data;
    data->angleReturnDelayFrames = 0x1e;
    data->pendingTargetKey = 0;
}


s32 effObjInitializeFollowModelData(dds3Basic_t *object) {
    EffectObjectData *data;
    void *work;

    dds3InitPoseSet(object);
    work = sdfChipAllocDirect(sizeof(EffectObjectData));
    object->data = work;
    memset(work, 0, sizeof(EffectObjectData));
    data = object->data;
    data->modelHolder = dds3CreateSlotResourceState(object);
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
    data = obj->data;
    if (data->handle != -1) {
        data->handle = -1;
    }
    if (data->transitionWork != 0) {
        evtDeleteUnitHandle(data->transitionWork);
        data->transitionWork = 0;
    }
    dds3ObjectBaseFreeModelHandle(obj);
    dds3ObjectBaseFreeControlWorkData(data->modelHolder);
    sdfChipFreeDirect(obj->data);
}

typedef struct EffVec4 {
    f32 v[4];
} EffVec4;

extern EffVec4 D_0039F720;
extern EffVec4 D_0039F730;
extern u8 dds3CheckPoseSetFlag(dds3PoseSet_t *node, u32 flags);
extern void dds3BackupPoseSet(dds3PoseSet_t *transform);
extern s32 evtUpdateUnitMotion(dds3PoseSet_t *, dds3Basic_t *);

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

    data = obj->data;
    flag = 0;
    if (data->transitionWork != 0) {
        dds3BackupPoseSet(&node);
        if (evtUpdateUnitMotion(&node, obj) != 0) {
            dds3GetSclPoseSetMCR(obj, (u128 *)node.scale);
            dds3AddRotPoseSet(obj, (u128 *)node.rotation);
            dds3GetPosPoseSetMCR(obj, (u128 *)node.position);
        }
    }
    model = (void *)data->modelHolder->resourceHandle;
    if (model != NULL && dds3CheckPoseSetFlag(obj->inner, OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) == 1) {
        dds3OffPoseSetFlag(obj->inner, OBJECT_TRANSFORM_FLAG_UPDATE_PENDING);
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
        SDF_VU0_LOADVECTOR(&obj->inner->scale[0]);
        if (data->transitionWork != 0 && flag != 0) {
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_LOADIDENTITYVECTOR();
            SDF_VU0_VECTOR_W_SET_ZERO();
            VU0_SCALAR_OP_CLOBBER(sdfSin(data->transitionWork->wobblePhase) * 0.01f, "vaddx.x vf10, vf0, vf2x");
            VU0_SCALAR_OP_CLOBBER(sdfSin(data->transitionWork->wobblePhase) * 0.004f, "vaddx.y vf10, vf0, vf2x");
            SDF_VU0_MULVECTOR();
            SDF_VU0_ADDVECTOR();
        }
        mdlSetScale(model);
        SDF_VU0_LOADVECTOR(&obj->inner->rotation[0]);
        Normalize();
        if (data->transitionWork != 0 && flag != 0) {
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_LOADVECTOR(&axis);
            FromRotation(sdfSin(data->transitionWork->wobblePhase) * 0.006f);
            effMiscQuatMultiplyVU();
        }
        if (data->word18 & 1) {
            SDF_VU0_LOADVECTOR_B(&D_0039F720);
            effMiscQuatMultiplyVU();
        }
        mdlSetRotate(model);
        if (dds3CheckPoseSetFlag(obj->inner, OBJECT_TRANSFORM_FLAG_USE_SMOOTHED_POSITION)) {
            SDF_VU0_LOADVECTOR(&obj->inner->smoothedPosition[0]);
            SDF_VU0_VECTOR_W_SET_ONE();
        } else {
            SDF_VU0_LOADVECTOR(&obj->inner->position[0]);
            SDF_VU0_VECTOR_W_SET_ONE();
        }
        mdlSetPosition(model);
        dds3SetRadiusPoseSet(obj->inner);
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

extern void effObjApplyTransformParameters(SdfPoolNode **, dds3Basic_t *);
extern void fldDrawEffectObjectShadow(dds3Basic_t *);
extern void fldDrawShadowDisc(f32, f32, f32, f32, s32);
extern SdfPoolNode *kwlnSceneSurfaceSets[13][4];

s32 dds3UpdateEffectObjectFollowParameters(dds3Basic_t *obj) {
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
    if (((EffectObjectData *)obj->data)->followParameterIndex == -1) {
        effObjApplyTransformParameters(kwlnSceneSurfaceSets[0], obj);
    } else {
        effObjApplyTransformParameters(kwlnSceneSurfaceSets[((EffectObjectData *)obj->data)->followParameterIndex], obj);
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
        vec[1] = pickMode == 1 ? target->shadowY : obj->inner->position[1];
        fldDrawShadowDisc(vec[0], vec[1], vec[2], obj->inner->radius, level);
    } else {
        if (dds3CheckPoseSetFlag(obj->inner, OBJECT_TRANSFORM_FLAG_USE_SMOOTHED_POSITION)) {
            vec[0] = obj->inner->smoothedPosition[0];
            vec[1] = obj->inner->smoothedPosition[1];
            vec[2] = obj->inner->smoothedPosition[2];
        } else {
            vec[0] = obj->inner->position[0];
            vec[1] = obj->inner->position[1];
            vec[2] = obj->inner->position[2];
        }
        if (pickMode == 1) {
            vec[1] = target->shadowY;
        }
        fldDrawShadowDisc(vec[0], vec[1], vec[2], obj->inner->radius, level);
    }
    return 1;
}

extern void dds3UnlinkLightObjectFromUnitObject(dds3Basic_t *, s32);
extern void dds3LinkLightObjectToUnitObject(EffectValueObject *value, dds3Basic_t *object, s32 duration);

void dds3UnitObjectBasicEL_LinkFree(dds3Basic_t *object) {
    EffectObjectData *data;

    data = object->data;
    if (data->activeId != -1) {
        dds3UnlinkLightObjectFromUnitObject(object, 10);
        data->activeId = 0xffffffff;
    }
}

struct WorldIndexNode;
extern s32 dds3ComparePosPoseSet2(f32 *, f32 *);
void dds3UnitObjectBasicEL_Link(dds3Basic_t *obj) {
    EffectObjectData *data = obj->data;
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
    if (dds3SetFirstBasicArray((dds3BasicArrayHandle_t *)list) != 0) {
        do {
            other = (EffectValueObject *)dds3GetBasicArray((dds3BasicArrayHandle_t *)list);
            otherData = other->data;
            if (!(otherData->flags & 4) &&
                dds3ComparePosPoseSet2(((f32 *)obj->inner), other->source) == 0 &&
                data->activeId == other->valueId) {
                dds3UnlinkLightObjectFromUnitObject(obj, 10);
                data->activeId = -1;
            }
        } while (dds3SetNextBasicArray((dds3BasicArrayHandle_t *)list) != 0);
    }
    dds3FreeBasicArrayHandle(list);

    if (data->activeId == -1) {
        list = dds3SceneBasicGetList(world, 9);
        if (dds3SetFirstBasicArray((dds3BasicArrayHandle_t *)list) == 0) {
            goto destroy_list;
        }
        do {
            other = (EffectValueObject *)dds3GetBasicArray((dds3BasicArrayHandle_t *)list);
            otherData = other->data;
            if (!(otherData->flags & 4) &&
                dds3ComparePosPoseSet2(((f32 *)obj->inner), other->source) != 0) {
                goto attach_transition;
            }
        } while (dds3SetNextBasicArray((dds3BasicArrayHandle_t *)list) != 0);

destroy_list:
        dds3FreeBasicArrayHandle(list);
        return;

attach_transition:
        dds3LinkLightObjectToUnitObject(other, obj, 10);
        data->activeId = other->valueId;
        dds3FreeBasicArrayHandle(list);
    }
}

dds3ObjectBaseControlWorkData_t *dds3MatterObjectBasicGetControlWorkData(const dds3Basic_t *object) {
    const EffectTransformData *data = object->data;

    return data->resourceState;
}

dds3Basic_t *dds3AddMatterObjectBasic(s32 key, f32 *offset, f32 *rotation) {
    f32 zero[4];
    dds3Basic_t *obj;

    memset(zero, 0, 0x10);
    zero[3] = 1.0f;
    obj = dds3AddSceneBasicData(EFF_WORLD_KIND_EFFECT_TRANSFORM);
    obj->key = (u32)key;
    dds3ObjectBaseCreateMover(obj);
    dds3SetRotPoseSet(obj, (u128 *)rotation);
    dds3SetPosPoseSet(obj, (u128 *)zero);
    dds3SetRadiusPoseSet(obj->inner);
    ((EffectTransformData *)obj->data)->offset[0] = offset[0];
    ((EffectTransformData *)obj->data)->offset[1] = offset[1];
    ((EffectTransformData *)obj->data)->offset[2] = offset[2];
    ((EffectTransformData *)obj->data)->offset[3] = offset[3];
    return obj;
}


void dds3DeleteMatterObjectBasic(dds3Basic_t *node) {
    dds3DeleteSceneBasicData(node);
}

void dds3MatterObjectBasicSetOt(dds3Basic_t *object, s32 activeId) {
    ((EffectTransformData *)object->data)->activeId = activeId;
}

void dds3MatterObjectBasicSetHojiID(dds3Basic_t *obj, u32 roomNumber) {
    ((EffectTransformData *)obj->data)->roomNumber = roomNumber;
}

u32 dds3MatterObjectBasicGetHojiID(u32 id) {
    dds3Basic_t *world;
    dds3Basic_t *node;
    EffectTransformData *data;

    world = dds3GetControlScene();
    node = dds3SearchSceneBasic(world, id, 6);
    data = (EffectTransformData *)node->data;
    return data->roomNumber;
}

#include "dds3_object_payload.h"

/* Payload word 8 is interpreted by object kind, not always as a pointer. */
void dds3MatterObjectBasicSetAlpMode(dds3Basic_t *object, u32 value) {
    u32 *payload = object->data;

    payload[2] = value;
}

u32 dds3MatterObjectBasicGetAlpMode(const dds3Basic_t *object) {
    const u32 *payload = object->data;

    return payload[2];
}

void effObjSetOpacityPassEnabled(u32 value) {
    effObjOpacityPassEnabled = value;
}

s32 dds3MatterObjectBasicConstructor(dds3Basic_t *object) {
    EffectTransformData *data;
    void *work;

    dds3InitPoseSet(object);
    work = sdfChipAllocDirect(sizeof(EffectTransformData));
    object->data = work;
    memset(work, 0, sizeof(EffectTransformData));
    data = object->data;
    data->resourceState = dds3CreateSlotResourceState(object);
    data->roomNumber = 0;
    data->opacityMode = EFFECT_OPACITY_MODE_INITIAL;
    data->activeId = -1;
    data->offset[0] = 0.0f;
    data->offset[1] = 0.0f;
    data->offset[2] = 0.0f;
    data->offset[3] = 1.0f;
    data->position[0] = 0.0f;
    data->position[1] = 0.0f;
    data->position[2] = 0.0f;
    data->position[3] = 1.0f;
    dds3ObjectBaseOnFlag(object, 0xE2);
    return 1;
}

void dds3MatterObjectBasicDestructor(dds3Basic_t *object) {
    EffectTransformData *data;

    dds3FreePoseSet(object);
    data = object->data;
    dds3ObjectBaseFreeControlWorkData(data->resourceState);
    sdfChipFreeDirect(data);
}

extern void dds3GetTMPoseSet(dds3Basic_t *object);
INCLUDE_RODATA(const s32, "game/code_001130E0", D_0039F720);

INCLUDE_RODATA(const s32, "game/code_001130E0", D_0039F730);

s32 effUpdateTransformOpacityMode(dds3Basic_t *object) {
    EffectTransformData *data = object->data;
    sdfModelRootNode_t *model;
    u32 mode = data->opacityMode;
    s32 alpha;
    f32 coordinates[3];
    dds3PoseSet_t *inner;

    switch (mode) {
    case 0:
        object->color = 0x80808080;
        break;
    case 1:
        object->color = 0x80808080;
        break;
    case 2:
        object->color = 0x00808080;
        break;
    case 3:
        alpha = ((u8 *)&object->color)[3];
        if (alpha < 0x80) {
            alpha += 8;
        }
        if (alpha > 0x80) {
            alpha = 0x80;
        }
        object->color = 0x00808080 | ((u32)alpha << 24);
        break;
    case 4:
        alpha = ((u8 *)&object->color)[3];
        if (alpha > 0x20) {
            alpha -= 8;
        }
        if (alpha < 0x20) {
            alpha = 0x20;
        }
        object->color = 0x00808080 | ((u32)alpha << 24);
        break;
    case 5:
        alpha = ((u8 *)&object->color)[3];
        if (alpha < 0x80) {
            alpha += 4;
        }
        if (alpha > 0x80) {
            alpha = 0x80;
        }
        object->color = 0x00808080 | ((u32)alpha << 24);
        break;
    case 6:
        alpha = ((u8 *)&object->color)[3];
        if (alpha != 0) {
            alpha -= 4;
        } else {
            alpha = 0;
        }
        if (alpha < 0) {
            alpha = 0;
        }
        object->color = 0x00808080 | ((u32)alpha << 24);
        break;
    case 7:
        alpha = ((u8 *)&object->color)[3];
        if (alpha < 0x80) {
            alpha += 2;
        }
        if (alpha > 0x80) {
            alpha = 0x80;
        }
        object->color = 0x00808080 | ((u32)alpha << 24);
        break;
    case 8:
        alpha = ((u8 *)&object->color)[3];
        if (alpha != 0) {
            alpha -= 2;
        } else {
            alpha = 0;
        }
        if (alpha <= 0) {
            alpha = 0;
        }
        object->color = 0x00808080 | ((u32)alpha << 24);
        break;
    case 9:
        alpha = ((u8 *)&object->color)[3];
        if (alpha != 0) {
            alpha -= 11;
        } else {
            alpha = 0;
        }
        if (alpha < 0) {
            alpha = 0;
        }
        object->color = 0x00808080 | ((u32)alpha << 24);
        break;
    }

    model = (sdfModelRootNode_t *)dds3ObjectBaseGetModelHandle(object);
    if (model != NULL) {
        dds3OffPoseSetFlag(object->inner, OBJECT_TRANSFORM_FLAG_UPDATE_PENDING);
        dds3GetRotPoseSet(object);
        SDF_VU0_STOREVECTOR(model->rotationQuaternion);
        dds3GetTMPoseSet(object);
        dds3GetPosPoseSet(object);
        if (fldStepActorMotionByKey(object->key, &coordinates[0], &coordinates[1], &coordinates[2]) !=
            FLD_ACTOR_MOTION_STEP_NOT_APPLIED) {
            f32 x = coordinates[0];
            f32 y = coordinates[1];
            inner = object->inner;
            inner->position[0] = data->offset[0] + data->position[0] + x;
            inner->position[1] = data->offset[1] + data->position[1] + y;
            inner->position[2] = data->offset[2] + data->position[2] + coordinates[2];
        } else {
            inner = object->inner;
            inner->position[0] = data->offset[0] + data->position[0];
            inner->position[1] = data->offset[1] + data->position[1];
            inner->position[2] = data->offset[2] + data->position[2];
        }
        SDF_VU0_SET_ROW3();
        VU0_STORE_MATRIX(model->matrix);
        dds3SetRadiusPoseSet(inner);
        sdfModelEvalTm(model);
    }
    return 1;
}

extern void rsetOtOfs(s32 index);
extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern dds3Basic_t *fldPlayerObject;

/* Submit the normal pass and the opacity-mode pass, temporarily neutralizing
 * the tint while the player is hidden. */
s32 effObjSubmitTransformOpacityPasses(dds3Basic_t *object) {
    EffectTransformData *data;
    u32 opacityMode;

    if (effObjOpacityPassEnabled == 0) {
        return 1;
    }
    data = object->data;
    if (data->activeId == -1) {
        rsetOtOfs(0x27);
        rsetTest(1, 5, 0x60, 1, 0, 0, 1, 2);
        effObjApplyTransformParameters(kwlnSceneSurfaceSets[0], object);
        rsetOtOfs(0x27);
        rsetTest(1, 5, 0x80, 1, 0, 0, 1, 2);
    } else {
        rsetOtOfs(0x22);
        rsetTest(1, 5, 0x60, 1, 0, 0, 1, 2);
        opacityMode = data->opacityMode;
        if (opacityMode < EFFECT_OPACITY_MODE_ALPHA_FADE_END) {
            if (opacityMode >= EFFECT_OPACITY_MODE_ALPHA_FADE_START) {
                if (object->color != EFFECT_OPACITY_COLOR_NEUTRAL) {
                    if (dds3ObjectBaseCheckFlag(fldPlayerObject, 1)) {
                        u32 savedColor = object->color;

                        object->color = EFFECT_OPACITY_COLOR_NEUTRAL;
                        effObjApplyTransformParameters(kwlnSceneSurfaceSets[data->activeId], object);
                        object->color = savedColor;
                    } else {
                        effObjApplyTransformParameters(kwlnSceneSurfaceSets[8], object);
                    }
                } else {
                    effObjApplyTransformParameters(kwlnSceneSurfaceSets[data->activeId], object);
                }
            }
        }
        effObjApplyTransformParameters(kwlnSceneSurfaceSets[data->activeId], object);
        rsetOtOfs(0x22);
        rsetTest(1, 5, 0x80, 1, 0, 0, 1, 2);
    }
    return 1;
}

/* Refresh the object's stored xyz from the source vector. */
void dds3RefreshStoredVec3(dds3Basic_t *object) {
    dds3PoseSet_t *source = object->inner;
    EffectTransformData *destination = object->data;

    destination->offset[0] = source->position[0];
    destination->offset[1] = source->position[1];
    destination->offset[2] = source->position[2];
}


/* Release each dependency according to the active state, clearing ownership
 * before releasing the next dependency. State 4 only borrows its handle. */
void FreeEffectData(EffectDependencyState *state) {
    EffectDependencyState *data = state;

    switch (data->state) {
    case EFF_DEPENDENCY_STATE_BOUND_EFFECT_NODE:
        if (data->handle != NULL) {
            dds3EffectDelete(data->handle);
            data->handle = NULL;
        }
        break;
    case EFF_DEPENDENCY_STATE_BILLBOARD_ANIMATION:
    case EFF_DEPENDENCY_STATE_BILLBOARD_NODE:
        if (data->handle != NULL) {
            dds3BillboardDelete(data->handle);
            data->handle = NULL;
        }
        break;
    case EFF_DEPENDENCY_STATE_BORROWED_HANDLE:
        if (data->handle != NULL) {
            data->handle = NULL;
        }
        break;
    case EFF_DEPENDENCY_STATE_EVENT_CUT:
        if (data->node != NULL) {
            effEveBEDCutDelete(data->node);
            data->node = NULL;
        }
        if (data->handle != NULL) {
            effEveBEDDelete(data->handle);
            data->handle = NULL;
        }
        if (data->vector != NULL) {
            sdfChipFreeDirect(data->vector);
            data->vector = NULL;
        }
        break;
    case EFF_DEPENDENCY_STATE_MAGATUHI_TWO_ROWS:
    case EFF_DEPENDENCY_STATE_MAGATUHI_FOUR_ROWS:
        if (data->handle != NULL) {
            dds3EffectDelete(data->handle);
            data->handle = NULL;
        }
        if (data->node != NULL) {
            sdfChipFreeDirect(data->node);
            data->node = NULL;
        }
        if (data->vector != NULL) {
            sdfChipFreeDirect(data->vector);
            data->vector = NULL;
        }
        break;
    }
    data->state = EFF_DEPENDENCY_STATE_EMPTY;
    data->handle = NULL;
}


s32 dds3EffectObjectBasicConstructor(dds3Basic_t *obj) {
    EffectDependencyState *data;

    dds3InitPoseSet(obj);
    obj->data = sdfChipAllocDirect(0x50);
    memset(obj->data, 0, 0x50);
    data = obj->data;
    data->objectHandle = dds3CreateSlotResourceState(obj);
    dds3ObjectBaseOnFlag(obj, 0x60);
    return 1;
}

/* Shadow projection reads this byte from the native unit layout. */
typedef char EvtUnitShadowLevelOffsetCheck[
    ((u32)&((evtUnitHandle_t *)0)->shadowLevel == 0xD3) ? 1 : -1];

INCLUDE_SDATA(const s32, "game/code_001130E0", effObjOpacityPassEnabled);
