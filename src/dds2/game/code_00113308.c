#include "mdl.h"
#include "common.h"
#include "eff_dependency.h"
#include "pcp_vu0.h"
#include "eff_object.h"
#include "evt_unit.h"
#include "eff.h"
#include "btl_sound.h"

extern void dds3ReleaseObjectBaseResources(EffWorldNode *object);

extern void *dds3GetWorldSecondaryObject(void);

extern EffWorldNode *dds3FindWorldObjectNodeByKey(EffWorldNode *object, u32 key, s32 kind);
extern ObjBase *dds3GetEffectObjectModelHolder(EffWorldNode *object);

extern u32 effObjOpacityPassEnabled;

/* Mode 0 is the initialized neutral tint; other values describe the native
 * alpha updates performed by this title's transform renderer. */
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
    EFFECT_OPACITY_MODE_ALPHA_RISE_12 = 10,
    EFFECT_OPACITY_MODE_ALPHA_FALL_12 = 11,
    EFFECT_OPACITY_MODE_ALPHA_FADE_START = EFFECT_OPACITY_MODE_ALPHA_RISE_8,
    EFFECT_OPACITY_MODE_ALPHA_FADE_END = EFFECT_OPACITY_MODE_ALPHA_RISE_4,
    EFFECT_OPACITY_ALPHA_MAX = 0x80,
    EFFECT_OPACITY_FALL_8_FLOOR = 0x20,
    EFFECT_OPACITY_COLOR_NEUTRAL = 0x80808080,
    EFFECT_OPACITY_COLOR_TRANSPARENT = 0x00808080
};

extern s32 effObjInnerCreate(EffWorldNode *node);
extern void effObjFreeInner(EffWorldNode *node);
extern void evtEndObjectValueTransition(EffWorldNode *object);

extern void *sdfAllocSizeClassBlock(s32 size);
extern void sdfReleaseChipBlock(void *memory);

extern void dds3SetObjectFlags(void *object, u32 mask);

u32 dds3GetEffectDataHandle(EffWorldNode *object) {
    return (u32)((EffectObjectData *)object->data)->handle;
}

EvtUnit *effObjGetTransitionWork(EffWorldNode *object) {
    return ((EffectObjectData *)object->data)->transitionWork;
}

void effObjSetFollowParameterIndex(EffWorldNode *object, u32 value) {
    ((EffectObjectData *)object->data)->followParameterIndex = value;
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

extern void effObjFetchInnerFirstVec(EffWorldNode *);
extern void effObjFetchInnerSecondVecNorm(EffWorldNode *);
extern f32 effMiscComputeQuaternionRotatedReferenceAngle(void);
extern f32 sdfAtan2(f32, f32);

/* The first conversion follows the original 180 / 3.14 approximation; the
 * quaternion helper uses the more precise radians-to-degrees factor. */
#define EFFECT_HEADING_DEGREES_PER_RADIAN_APPROX 57.32484055f
#define EFFECT_HEADING_RADIANS_TO_DEGREES 57.29577637f

void effObjStepFollowAngleTowardPosition(EffWorldNode *obj, const f32 *targetPosition) {
    EffectObjectData *data = obj->data;
    f32 position[4];
    f32 currentAngle = data->angle;
    f32 heading;
    f32 targetAngle;
    f32 referenceAngle;
    f32 angleDelta;
    f32 step;

    effObjFetchInnerFirstVec(obj);
    VU0_STORE_VF(vf10, position);
    if (position[0] == targetPosition[0] &&
        position[2] == targetPosition[2]) {
        return;
    }

    heading = -(sdfAtan2(position[0] - targetPosition[0],
                         position[2] - targetPosition[2]) *
                EFFECT_HEADING_DEGREES_PER_RADIAN_APPROX);
    effObjFetchInnerSecondVecNorm(obj);
    referenceAngle = effMiscComputeQuaternionRotatedReferenceAngle();
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

extern void dds3ClearObjectFlags(void *object, u32 mask);

void effObjStepFollowAngleTowardZero(EffWorldNode *object) {
    EffectObjectData *data = object->data;
    f32 angle;
    f32 value;
    f32 step;

    if (data->angleReturnDelayFrames > 0) {
        data->angleReturnDelayFrames--;
    } else {
        angle = data->angle;
        if (angle < 0.1f && -0.1f < angle) {
            dds3ClearObjectFlags(object, 0x2000);
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

void evtArmEffectObjectPendingValue(EffWorldNode *object, s32 value) {
    EffectObjectData *data;

    data = object->data;
    dds3SetObjectFlags(object, 0x2000);
    data->pendingTargetKey = value;
    data->angleReturnDelayFrames = 0;
}

void evtResetObjectPendingValue(EffWorldNode *object) {
    EffectObjectData *data;

    data = object->data;
    data->angleReturnDelayFrames = 0x1e;
    data->pendingTargetKey = 0;
}

s32 effObjInitializeFollowModelData(EffWorldNode *object) {
    EffectObjectData *data;
    void *work;

    effObjInnerCreate(object);
    work = sdfAllocSizeClassBlock(sizeof(EffectObjectData));
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
    dds3SetObjectFlags(object, 0x42);
    return 1;
}

void evtDestroyEffectObjectData(EffWorldNode *object) {
    EffectObjectData *data;

    evtEndObjectValueTransition(object);
    effObjFreeInner(object);
    data = object->data;
    if (data->handle != -1) {
        data->handle = -1;
    }
    if (data->transitionWork != 0) {
        evtReleaseUnitTransitionWork(data->transitionWork);
        data->transitionWork = 0;
    }
    dds3ReleaseObjectBaseResources(object);
    dds3DestroyObjectBase(data->modelHolder);
    sdfReleaseChipBlock(object->data);
}

typedef struct EffLocalNode {
    u8 pad00[0x40];
    u128 vec40; /* 0x40 */
    u128 vec50; /* 0x50 */
    u128 vec60; /* 0x60 */
    u8 pad70[0x60];
} EffLocalNode; /* 0xD0, a transform node built on the stack */

typedef struct EffVec4 {
    f32 v[4];
} EffVec4;

extern EffVec4 D_004128A0;
extern EffVec4 D_004128B0;
extern u8 dds3TestObjectFlags(void *object, u32 mask);
extern u8 effObjTestNodeFlags(ObjectTransform *, u32);
extern void effObjInnerVecInit(EffLocalNode *);
extern s32 func_0023DA70(EffLocalNode *, EffWorldNode *);
extern void effObjMulInnerThirdVec(EffWorldNode *, u128 *);
extern void effObjQuatMulInnerSecondVec(EffWorldNode *, u128 *);
extern void effObjAddInnerFirstVec(EffWorldNode *, u128 *);
extern void effObjClearNodeFlags(ObjectTransform *, u32);
extern f32 sdfSinPoly(f32);
extern void mdlStoreTertiaryVectorVU(void *);
extern void mdlStorePrimaryVectorVU(void *);
extern void mdlUpdateContextRotationBasisFromQuaternion(void *);
extern void effMiscNormalizeVU(void);
extern void effMiscAxisAngleToQuaternionVU(f32);
extern void effMiscQuatMultiplyVU(void);
extern void effObjInnerVecBackup(ObjectTransform *);
extern void func_00113D18(EffWorldNode *);
extern void effObjStepFollowAngleTowardZero(EffWorldNode *);
extern void effObjStepFollowAngleTowardPosition(EffWorldNode *, const f32 *);
extern void *dds3GetWorldNodeData(EffWorldNode *node);

/* Per-frame refresh of a model effect object: rebuild the child transform from the follow record (a tilt that wobbles with its angle), then run the timed callbacks. */
s32 effUpdateFollowModelTransform(EffWorldNode *obj) {
    EffVec4 axis = D_004128B0;
    EffLocalNode node;
    EffectObjectData *data;
    void *model;
    s32 flag;

    data = obj->data;
    flag = 0;
    if (data->transitionWork != 0) {
        effObjInnerVecInit(&node);
        if (func_0023DA70(&node, obj) != 0) {
            effObjMulInnerThirdVec(obj, &node.vec60);
            effObjQuatMulInnerSecondVec(obj, &node.vec50);
            effObjAddInnerFirstVec(obj, &node.vec40);
        }
    }
    model = (void *)data->modelHolder->resourceHandle;
    if (model != NULL && effObjTestNodeFlags(obj->inner, OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) == 1) {
        effObjClearNodeFlags(obj->inner, OBJECT_TRANSFORM_FLAG_UPDATE_PENDING);
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
        VU0_LOAD_VF(vf10, &obj->inner->scale[0]);
        if (data->transitionWork != 0 && flag != 0) {
            VU0_MOVE_VF(vf11, vf10);
            VU0_MOVE_VF(vf10, vf0);
            VU0_CLEAR_W(vf10);
            VU0_SCALAR_OP_CLOBBER(sdfSinPoly(data->transitionWork->wobblePhase) * 0.01f, "vaddx.x vf10, vf0, vf2x");
            VU0_SCALAR_OP_CLOBBER(sdfSinPoly(data->transitionWork->wobblePhase) * 0.004f, "vaddx.y vf10, vf0, vf2x");
            VU0_MUL(vf10, vf10, vf11);
            VU0_ADD(vf10, vf10, vf11);
        }
        mdlStoreTertiaryVectorVU(model);
        VU0_LOAD_VF(vf10, &obj->inner->rotation[0]);
        effMiscNormalizeVU();
        if (data->transitionWork != 0 && flag != 0) {
            VU0_MOVE_VF(vf11, vf10);
            VU0_LOAD_VF(vf10, &axis);
            effMiscAxisAngleToQuaternionVU(sdfSinPoly(data->transitionWork->wobblePhase) * 0.006f);
            effMiscQuatMultiplyVU();
        }
        if (data->word18 & 1) {
            VU0_LOAD_VF(vf11, &D_004128A0);
            effMiscQuatMultiplyVU();
        }
        mdlUpdateContextRotationBasisFromQuaternion(model);
        if (effObjTestNodeFlags(obj->inner, OBJECT_TRANSFORM_FLAG_USE_SMOOTHED_POSITION)) {
            VU0_LOAD_VF(vf10, &obj->inner->smoothedPosition[0]);
            VU0_SET_W_ONE(vf10);
        } else {
            VU0_LOAD_VF(vf10, &obj->inner->position[0]);
            VU0_SET_W_ONE(vf10);
        }
        mdlStorePrimaryVectorVU(model);
        effObjInnerVecBackup(obj->inner);
    }
    if (!dds3TestObjectFlags(obj, 0x100)) {
        func_00113D18(obj);
    }
    if (dds3TestObjectFlags(obj, 0x2000)) {
        if (data->pendingTargetKey != 0) {
            effObjStepFollowAngleTowardPosition(
                obj, dds3GetWorldNodeData(dds3FindWorldObjectNodeByKey(
                    dds3GetWorldSecondaryObject(), data->pendingTargetKey, 0x11)));
        } else {
            effObjStepFollowAngleTowardZero(obj);
        }
    }
    return 1;
}

extern void func_00112518(void *, EffWorldNode *);
extern void func_00120B88(EffWorldNode *);
extern s32 sdfLoadMapRecordPositionVector(SdfModel *model, s32 id);
extern void func_001200E8(s32, f32, f32, f32, f32);
extern u8 D_00380788[];

s32 dds3UpdateEffectObjectFollowParameters(EffWorldNode *obj) {
    f32 vec[4];
    EvtUnit *target;
    MdlCtx *config;
    s32 level;
    s32 pickMode;

    memset(vec, 0, 0x10);
    vec[3] = 1.0f;
    if (dds3TestObjectFlags(obj, 1)) {
        return 1;
    }
    target = effObjGetTransitionWork(obj);
    if (dds3TestObjectFlags(obj, 0x200) && target != NULL && !(target->owner->flags & MDL_SKIP_TRANSFORMS)) {
        func_00120B88(obj);
    }
    if ((s32)((EffectObjectData *)obj->data)->followParameterIndex == -1) {
        func_00112518(D_00380788, obj);
    } else {
        func_00112518(D_00380788 + (s32)((EffectObjectData *)obj->data)->followParameterIndex * 0x10, obj);
    }
    if (!dds3TestObjectFlags(obj, 0x400)) {
        return 1;
    }
    if (dds3TestObjectFlags(obj, 0x1000)) {
        return 1;
    }
    if (target == NULL) {
        return 1;
    }
    config = (MdlCtx *)dds3GetEffectObjectModelHolder(obj)->resourceHandle;
    if (dds3TestObjectFlags(obj, 0x4000)) {
        level = target->unkD3;
    } else {
        level = config->inner->color >> 24;
    }
    pickMode = dds3TestObjectFlags(obj, 0x8000) != 0;
    if (sdfLoadMapRecordPositionVector(target->owner->inner, 0)) {
        VU0_STORE_VF(vf10, vec);
        vec[1] = pickMode == 1 ? target->unkD4 : obj->inner->position[1];
        func_001200E8(level, vec[0], vec[1], vec[2], obj->inner->radius);
    } else {
        if (effObjTestNodeFlags(obj->inner, OBJECT_TRANSFORM_FLAG_USE_SMOOTHED_POSITION)) {
            vec[0] = obj->inner->smoothedPosition[0];
            vec[1] = obj->inner->smoothedPosition[1];
            vec[2] = obj->inner->smoothedPosition[2];
        } else {
            vec[0] = obj->inner->position[0];
            vec[1] = obj->inner->position[1];
            vec[2] = obj->inner->position[2];
        }
        if (pickMode == 1) {
            vec[1] = target->unkD4;
        }
        func_001200E8(level, vec[0], vec[1], vec[2], obj->inner->radius);
    }
    return 1;
}

extern void evtEndUnitValueTransitionForObject(EffWorldNode *, s32);
extern void evtSetUnitValueTransitionForObject(void *, EffWorldNode *, s32);

void evtEndObjectValueTransition(EffWorldNode *object) {
    EffectObjectData *data;

    data = object->data;
    if (data->activeId != -1) {
        evtEndUnitValueTransitionForObject(object, 10);
        data->activeId = 0xffffffff;
    }
}

typedef struct EffectValueData {
    u8 pad00[0x64];
    u32 flags;
} EffectValueData;

typedef struct EffectValueObject {
    u8 pad00[4];
    s32 valueId;
    u8 pad08[0x10];
    EffectValueData *data;
    f32 *source;
} EffectValueObject;

struct NodeB;
extern struct NodeB *dds3CopyWorldListToValueChain(EffWorldNode *object, s32 kind);
extern u16 dds3GetWorldValueCount(WorldValueIndices *object);
extern u32 dds3ResetObjectValueCursor(WorldValueIndices *object);
extern u32 dds3ReadIndexedWorldObjectWord(WorldValueIndices *object);
extern u32 dds3AdvanceObjectValueCursor(WorldValueIndices *object);
extern void dds3DestroyWorldIndexNode(struct NodeB *node);
extern s32 func_0010FBD0(f32 *, f32 *);
void func_00113D18(EffWorldNode *object) {
    EffectObjectData *data = object->data;
    EffWorldNode *world = dds3GetWorldSecondaryObject();
    struct NodeB *list;
    EffectValueObject *other;
    EffectValueData *otherData;

    if (world == 0) {
        return;
    }
    list = dds3CopyWorldListToValueChain(world, 9);
    if (list == NULL) {
        return;
    }
    if (dds3GetWorldValueCount((WorldValueIndices *)list) == 0) {
        dds3DestroyWorldIndexNode(list);
        return;
    }
    if (dds3ResetObjectValueCursor((WorldValueIndices *)list) != 0) {
        do {
            other = (EffectValueObject *)dds3ReadIndexedWorldObjectWord((WorldValueIndices *)list);
            otherData = other->data;
            if (!(otherData->flags & 4) &&
                func_0010FBD0(((f32 *)object->inner), other->source) == 0 &&
                data->activeId == other->valueId) {
                evtEndUnitValueTransitionForObject(object, 10);
                data->activeId = -1;
            }
        } while (dds3AdvanceObjectValueCursor((WorldValueIndices *)list) != 0);
    }
    dds3DestroyWorldIndexNode(list);

    if (data->activeId == -1) {
        list = dds3CopyWorldListToValueChain(world, 9);
        if (dds3ResetObjectValueCursor((WorldValueIndices *)list) == 0) {
            goto destroy_list;
        }
        do {
            other = (EffectValueObject *)dds3ReadIndexedWorldObjectWord((WorldValueIndices *)list);
            otherData = other->data;
            if (!(otherData->flags & 4) &&
                func_0010FBD0(((f32 *)object->inner), other->source) != 0) {
                goto attach_transition;
            }
        } while (dds3AdvanceObjectValueCursor((WorldValueIndices *)list) != 0);

destroy_list:
        dds3DestroyWorldIndexNode(list);
        return;

attach_transition:
        evtSetUnitValueTransitionForObject(other, object, 10);
        data->activeId = other->valueId;
        dds3DestroyWorldIndexNode(list);
    }
}

ObjBase *effObjGetDataHandle(EffWorldNode *object) {
    EffectTransformData *data = object->data;

    return data->resourceState;
}

extern EffWorldNode *dds3AppendWorldObjectNode(s32 kind);

extern void dds3EnsureSlotData();

extern void effObjSetInnerFirstVec();

extern void effObjSetInnerSecondVec();

/* Spawn a world object of kind 6 and seed its stored vector. */
EffWorldNode *dds3SpawnInnerVecObj6(s32 a, f32 *vec, void *second) {
    f32 zero[4];
    EffWorldNode *obj;

    memset(zero, 0, 0x10);
    zero[3] = 1.0f;
    obj = dds3AppendWorldObjectNode(6);
    obj->key = (u32)a;
    dds3EnsureSlotData(obj);
    effObjSetInnerSecondVec(obj, second);
    effObjSetInnerFirstVec(obj, zero);
    effObjInnerVecBackup(obj->inner);
    ((EffectTransformData *)obj->data)->offset[0] = vec[0];
    ((EffectTransformData *)obj->data)->offset[1] = vec[1];
    ((EffectTransformData *)obj->data)->offset[2] = vec[2];
    ((EffectTransformData *)obj->data)->offset[3] = vec[3];
    return obj;
}

extern void dds3RemoveWorldObjectNode(EffWorldNode *node);

void func_00113FD0(EffWorldNode *node) {
    dds3RemoveWorldObjectNode(node);
}

void effObjSetActiveId(EffWorldNode *object, s32 activeId) {
    ((EffectTransformData *)object->data)->activeId = activeId;
}

void effObjSetRoomNumber(EffWorldNode *object, u32 roomNumber) {
    ((EffectTransformData *)object->data)->roomNumber = roomNumber;
}

u32 effObjGetRoomNumberById(u32 id) {
    EffWorldNode *world;
    EffWorldNode *node;
    EffectTransformData *data;

    world = dds3GetWorldSecondaryObject();
    node = dds3FindWorldObjectNodeByKey(world, id, 6);
    data = (EffectTransformData *)node->data;
    return data->roomNumber;
}

/* Payload word 8 is interpreted by object kind, not always as a pointer. */
void dds3SetObjectPayloadWord8(EffWorldNode *object, u32 value) {
    u32 *payload = object->data;

    payload[2] = value;
}

u32 dds3GetObjectPayloadWord8(EffWorldNode *object) {
    u32 *payload = object->data;

    return payload[2];
}

void effObjSetOpacityPassEnabled(u32 value) {
    effObjOpacityPassEnabled = value;
}

s32 effObjInitializeTransformData(EffWorldNode *object) {
    EffectTransformData *data;
    void *work;

    effObjInnerCreate(object);
    work = sdfAllocSizeClassBlock(sizeof(EffectTransformData));
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
    dds3SetObjectFlags(object, 0xE2);
    return 1;
}

void evtReleaseEffectObjectHandleAndData(EffWorldNode *object) {
    EffectTransformData *data;

    effObjFreeInner(object);
    data = object->data;
    dds3DestroyObjectBase(data->resourceState);
    sdfReleaseChipBlock(data);
}

extern u32 dds3GetObjectBaseResourceHandle(EffWorldNode *);
extern void dds3LoadOrBuildObjectMatrix(EffWorldNode *object);
extern s32 func_00143910(u32 key, f32 *x, f32 *y, f32 *z);
extern void sdfModelUpdateCurrentFrameTransforms(SdfModel *model);

INCLUDE_RODATA(const s32, "game/code_00113308", D_004128A0);

INCLUDE_RODATA(const s32, "game/code_00113308", D_004128B0);

s32 func_00114150(EffWorldNode *object) {
    EffectTransformData *data = object->data;
    SdfModel *model;
    u32 mode = data->opacityMode;
    s32 alpha;
    f32 coordinates[3];
    ObjectTransform *inner;

    switch (mode) {
    case EFFECT_OPACITY_MODE_INITIAL:
        object->color = EFFECT_OPACITY_COLOR_NEUTRAL;
        break;
    case 1:
        object->color = EFFECT_OPACITY_COLOR_NEUTRAL;
        break;
    case EFFECT_OPACITY_MODE_ALPHA_CLEAR:
        object->color = EFFECT_OPACITY_COLOR_TRANSPARENT;
        break;
    case EFFECT_OPACITY_MODE_ALPHA_RISE_8:
        alpha = ((u8 *)&object->color)[3];
        if (alpha < EFFECT_OPACITY_ALPHA_MAX) {
            alpha += 8;
        }
        if (alpha > EFFECT_OPACITY_ALPHA_MAX) {
            alpha = EFFECT_OPACITY_ALPHA_MAX;
        }
        object->color = EFFECT_OPACITY_COLOR_TRANSPARENT | ((u32)alpha << 24);
        break;
    case EFFECT_OPACITY_MODE_ALPHA_FALL_8_TO_32:
        alpha = ((u8 *)&object->color)[3];
        if (alpha > EFFECT_OPACITY_FALL_8_FLOOR) {
            alpha -= 8;
        }
        if (alpha < EFFECT_OPACITY_FALL_8_FLOOR) {
            alpha = EFFECT_OPACITY_FALL_8_FLOOR;
        }
        object->color = EFFECT_OPACITY_COLOR_TRANSPARENT | ((u32)alpha << 24);
        break;
    case EFFECT_OPACITY_MODE_ALPHA_RISE_4:
        alpha = ((u8 *)&object->color)[3];
        if (alpha < EFFECT_OPACITY_ALPHA_MAX) {
            alpha += 4;
        }
        if (alpha > EFFECT_OPACITY_ALPHA_MAX) {
            alpha = EFFECT_OPACITY_ALPHA_MAX;
        }
        object->color = EFFECT_OPACITY_COLOR_TRANSPARENT | ((u32)alpha << 24);
        break;
    case EFFECT_OPACITY_MODE_ALPHA_FALL_4:
        alpha = ((u8 *)&object->color)[3];
        if (alpha != 0) {
            alpha -= 4;
        } else {
            alpha = 0;
        }
        if (alpha < 0) {
            alpha = 0;
        }
        object->color = EFFECT_OPACITY_COLOR_TRANSPARENT | ((u32)alpha << 24);
        break;
    case EFFECT_OPACITY_MODE_ALPHA_RISE_2:
        alpha = ((u8 *)&object->color)[3];
        if (alpha < EFFECT_OPACITY_ALPHA_MAX) {
            alpha += 2;
        }
        if (alpha > EFFECT_OPACITY_ALPHA_MAX) {
            alpha = EFFECT_OPACITY_ALPHA_MAX;
        }
        object->color = EFFECT_OPACITY_COLOR_TRANSPARENT | ((u32)alpha << 24);
        break;
    case EFFECT_OPACITY_MODE_ALPHA_FALL_2:
        alpha = ((u8 *)&object->color)[3];
        if (alpha != 0) {
            alpha -= 2;
        } else {
            alpha = 0;
        }
        if (alpha <= 0) {
            alpha = 0;
        }
        object->color = EFFECT_OPACITY_COLOR_TRANSPARENT | ((u32)alpha << 24);
        break;
    case EFFECT_OPACITY_MODE_ALPHA_FALL_11:
        alpha = ((u8 *)&object->color)[3];
        if (alpha != 0) {
            alpha -= 11;
        } else {
            alpha = 0;
        }
        if (alpha < 0) {
            alpha = 0;
        }
        object->color = EFFECT_OPACITY_COLOR_TRANSPARENT | ((u32)alpha << 24);
        break;
    case EFFECT_OPACITY_MODE_ALPHA_RISE_12:
        alpha = ((u8 *)&object->color)[3];
        if (alpha < EFFECT_OPACITY_ALPHA_MAX) {
            alpha += 12;
        }
        if (alpha > EFFECT_OPACITY_ALPHA_MAX) {
            alpha = EFFECT_OPACITY_ALPHA_MAX;
        }
        object->color = EFFECT_OPACITY_COLOR_TRANSPARENT | ((u32)alpha << 24);
        break;
    case EFFECT_OPACITY_MODE_ALPHA_FALL_12:
        alpha = ((u8 *)&object->color)[3];
        if (alpha != 0) {
            alpha -= 12;
        } else {
            alpha = 0;
        }
        if (alpha < 0) {
            alpha = 0;
        }
        object->color = EFFECT_OPACITY_COLOR_TRANSPARENT | ((u32)alpha << 24);
        break;
    }

    model = (SdfModel *)dds3GetObjectBaseResourceHandle(object);
    if (model != NULL) {
        effObjClearNodeFlags(object->inner, OBJECT_TRANSFORM_FLAG_UPDATE_PENDING);
        effObjFetchInnerSecondVecNorm(object);
        VU0_STORE_VF(vf10, model->unk60);
        dds3LoadOrBuildObjectMatrix(object);
        effObjFetchInnerFirstVec(object);
        if (func_00143910(object->key, &coordinates[0], &coordinates[1], &coordinates[2]) != 0) {
            f32 x = coordinates[0];
            f32 y = coordinates[1];
            f32 z = coordinates[2];
            inner = object->inner;
            inner->position[0] = data->offset[0] + data->position[0] + x;
            inner->position[1] = data->offset[1] + data->position[1] + y;
            inner->position[2] = data->offset[2] + data->position[2] + z;
        } else {
            inner = object->inner;
            inner->position[0] = data->offset[0] + data->position[0];
            inner->position[1] = data->offset[1] + data->position[1];
            inner->position[2] = data->offset[2] + data->position[2];
        }
        VU0_MOVE_VF(vf31, vf10);
        VU0_STORE_MATRIX(model->matrix);
        effObjInnerVecBackup(inner);
        sdfModelUpdateCurrentFrameTransforms(model);
    }
    return 1;
}

extern void fldSelectDisplayBuffer(u32);
extern void fldSubmitFrameQuad(s32, s32, s32, s32, s32, s32, s32, s32);
extern u32 fldPlayerObject;
extern u8 D_00380808[];

/* Submit the normal pass and the opacity-mode pass, temporarily neutralizing
 * the tint while the player is hidden. */
s32 effObjSubmitTransformOpacityPasses(EffWorldNode *object) {
    EffectTransformData *data;
    u32 opacityMode;

    if (effObjOpacityPassEnabled == 0) {
        return 1;
    }
    data = object->data;
    if (data->activeId == -1) {
        fldSelectDisplayBuffer(0x27);
        fldSubmitFrameQuad(1, 5, 0x60, 1, 0, 0, 1, 2);
        func_00112518(D_00380788, object);
        fldSelectDisplayBuffer(0x27);
        fldSubmitFrameQuad(1, 5, 0x80, 1, 0, 0, 1, 2);
    } else {
        fldSelectDisplayBuffer(0x22);
        fldSubmitFrameQuad(1, 5, 0x60, 1, 0, 0, 1, 2);
        opacityMode = data->opacityMode;
        if (opacityMode < EFFECT_OPACITY_MODE_ALPHA_FADE_END) {
            if (opacityMode >= EFFECT_OPACITY_MODE_ALPHA_FADE_START) {
                if (object->color != EFFECT_OPACITY_COLOR_NEUTRAL) {
                    if (dds3TestObjectFlags((EffWorldNode *)fldPlayerObject, 1)) {
                        u32 savedColor = object->color;

                        object->color = EFFECT_OPACITY_COLOR_NEUTRAL;
                        func_00112518(D_00380788 + data->activeId * 0x10, object);
                        object->color = savedColor;
                    } else {
                        func_00112518(D_00380808, object);
                    }
                } else {
                    func_00112518(D_00380788 + data->activeId * 0x10, object);
                }
            }
        }
        func_00112518(D_00380788 + data->activeId * 0x10, object);
        fldSelectDisplayBuffer(0x22);
        fldSubmitFrameQuad(1, 5, 0x80, 1, 0, 0, 1, 2);
    }
    return 1;
}

/* Refresh the object's stored xyz from the source vector. */
void dds3RefreshStoredVec3(EffWorldNode *object) {
    ObjectTransform *source = object->inner;
    EffectTransformData *destination = object->data;

    destination->offset[0] = source->position[0];
    destination->offset[1] = source->position[1];
    destination->offset[2] = source->position[2];
}

struct EffEventWork;
extern void effDestroyNode(struct EffNode *);
extern void billDispatchByKind(BillObj *);
extern void effEventReleaseNode(struct EffEventWork *);
extern void func_00197D50(SoundMixer *);

/* Release each dependency according to the active state, clearing ownership
 * before releasing the next dependency. State 4 only borrows its handle. */
void effObjReleaseStateDependencies(EffectDependencyState *state) {
    EffectDependencyState *data = state;

    switch (data->state) {
    case 1:
        if (data->handle != NULL) {
            effDestroyNode(data->handle);
            data->handle = NULL;
        }
        break;
    case 2:
    case 3:
        if (data->handle != NULL) {
            billDispatchByKind(data->handle);
            data->handle = NULL;
        }
        break;
    case 4:
        if (data->handle != NULL) {
            data->handle = NULL;
        }
        break;
    case 5:
        if (data->node != NULL) {
            effEventReleaseNode(data->node);
            data->node = NULL;
        }
        if (data->handle != NULL) {
            func_00197D50(data->handle);
            data->handle = NULL;
        }
        if (data->vector != NULL) {
            sdfReleaseChipBlock(data->vector);
            data->vector = NULL;
        }
        break;
    case 7:
    case 8:
        if (data->handle != NULL) {
            effDestroyNode(data->handle);
            data->handle = NULL;
        }
        if (data->node != NULL) {
            sdfReleaseChipBlock(data->node);
            data->node = NULL;
        }
        if (data->vector != NULL) {
            sdfReleaseChipBlock(data->vector);
            data->vector = NULL;
        }
        break;
    }
    data->state = 0;
    data->handle = NULL;
}


s32 evtInitializeEffectObjectData(EffWorldNode *obj) {
    EffectDependencyState *data;

    effObjInnerCreate(obj);
    obj->data = sdfAllocSizeClassBlock(0x50);
    memset(obj->data, 0, 0x50);
    data = obj->data;
    data->objectHandle = dds3CreateSlotResourceState(obj);
    dds3SetObjectFlags(obj, 0x60);
    return 1;
}

INCLUDE_SDATA(const s32, "game/code_00113308", effObjOpacityPassEnabled);

