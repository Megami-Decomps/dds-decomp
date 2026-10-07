#include "mdl.h"
#include "common.h"
#include "eff_dependency.h"
#include "pcp_vu0.h"
#include "eff_object.h"
#include "eff.h"
#include "btl_sound.h"

/* Follow record the object tracks (angle, flags and kind at the end of a longer record). */
typedef struct EffFollowRec {
    u8 pad00[0x98];
    f32 angle;   /* 0x98 */
    u8 pad9C[0xC];
    u32 flags;   /* 0xA8 */
    s16 kind;    /* 0xAC */
} EffFollowRec;




typedef struct EffectObject {
    u8 pad00[0x18];
    EffectObjectData *data;
    f32 *source; /* 0x1C */
    u8 pad20[0x14];
    u32 color; /* 0x34: transform-node draw tint */
} EffectObject;

extern u32 D_003BA9D0;

extern u64 dds3GetWorldSecondaryObject(void);

extern s32 dds3FindWorldObjectNodeByKey(u64, u64, u64);
extern ObjBase *dds3GetEffectObjectModelHolder(EffWorldNode *object);
extern s32 effObjInnerCreate(EffWorldNode *node);
extern void effObjFreeInner(EffWorldNode *node);
extern void evtEndObjectValueTransition(EffWorldNode *object);
extern void *sdfAllocSizeClassBlock(s32 size);
extern void dds3SetObjectFlags(void *, s32);

u32 dds3GetEffectDataHandle(EffectObject *obj) {
    return (u32)obj->data->handle;
}

u32 effObjGetTransitionWork(EffectObject *obj) {
    return (u32)obj->data->transitionWork;
}

void func_00113100(EffectObject *obj, u32 value) {
    obj->data->word14 = value;
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

extern void effObjFetchInnerFirstVec(EffectObject *);
extern void effObjFetchInnerSecondVecNorm(EffectObject *);
extern f32 effMiscComputeQuaternionRotatedReferenceAngle(void);
extern f32 sdfAtan2(f32, f32);

/* The first conversion follows the original 180 / 3.14 approximation; the
 * quaternion helper uses the more precise radians-to-degrees factor. */
#define EFFECT_HEADING_DEGREES_PER_RADIAN_APPROX 57.32484055f
#define EFFECT_HEADING_RADIANS_TO_DEGREES 57.29577637f

void func_001131E0(EffectObject *obj, const f32 *targetPosition) {
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

extern void dds3ClearObjectFlags(EffectObject *, s32);

void func_00113338(EffectObject *object) {
    EffectObjectData *data = object->data;
    f32 angle;
    f32 value;
    f32 step;

    if (data->timer > 0) {
        data->timer--;
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

void evtArmEffectObjectPendingValue(EffectObject *obj, u32 value) {
    EffectObjectData *data;

    data = obj->data;
    dds3SetObjectFlags(obj, 0x2000);
    data->pendingValue = value;
    data->timer = 0;
}

void evtResetObjectPendingValue(EffectObject *obj) {
    EffectObjectData *data;

    data = obj->data;
    data->timer = 0x1e;
    data->pendingValue = 0;
}

s32 effObjInitializeFollowModelData(EffectObject *object) {
    EffectObjectData *data;
    void *work;

    effObjInnerCreate((EffWorldNode *)object);
    work = sdfAllocSizeClassBlock(sizeof(EffectObjectData));
    object->data = work;
    memset(work, 0, sizeof(EffectObjectData));
    data = object->data;
    data->modelHolder = dds3CreateSlotResourceState(object);
    data->transitionWork = NULL;
    data->activeId = -1;
    data->word14 = -1;
    data->pendingValue = 0;
    data->timer = 0;
    data->angle = 0.0f;
    data->limitMin2C = -45.0f;
    data->limitMax30 = 45.0f;
    data->word34 = 0;
    data->limitMin38 = -45.0f;
    data->limitMax3C = 45.0f;
    dds3SetObjectFlags(object, 0x42);
    return 1;
}

void evtDestroyEffectObjectData(EffectObject *obj) {
    EffectObjectData *data;

    evtEndObjectValueTransition((EffWorldNode *)obj);
    effObjFreeInner((EffWorldNode *)obj);
    data = obj->data;
    if (data->handle != -1) {
        data->handle = -1;
    }
    if (data->transitionWork != 0) {
        evtReleaseUnitTransitionWork(data->transitionWork);
        data->transitionWork = 0;
    }
    dds3ReleaseObjectBaseResources(obj);
    dds3DestroyObjectBase(data->modelHolder);
    sdfReleaseChipBlock(obj->data);
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

extern EffVec4 D_0039F720;
extern EffVec4 D_0039F730;
extern u8 dds3TestObjectFlags(EffectObject *, s32);
extern s32 effObjTestNodeFlags(f32 *, s32);
extern void effObjInnerVecInit(EffLocalNode *);
extern s32 func_00222ED8(EffLocalNode *, EffectObject *);
extern void effObjMulInnerThirdVec(EffectObject *, u128 *);
extern void effObjQuatMulInnerSecondVec(EffectObject *, u128 *);
extern void effObjAddInnerFirstVec(EffectObject *, u128 *);
extern void effObjClearNodeFlags(f32 *, s32);
extern f32 sdfSinPoly(f32);
extern void mdlStoreTertiaryVectorVU(void *);
extern void mdlStorePrimaryVectorVU(void *);
extern void mdlUpdateContextRotationBasisFromQuaternion(void *);
extern void effMiscNormalizeVU(void);
extern void effMiscAxisAngleToQuaternionVU(f32);
extern void effMiscQuatMultiplyVU(void);
extern void effObjInnerVecBackup(f32 *);
extern void func_00113AF0(EffectObject *);
extern void func_00113338(EffectObject *);
extern void func_001131E0(EffectObject *, const f32 *);
extern f32 *func_00117650(s32);

/* Per-frame refresh of a model effect object: rebuild the child transform from the follow record (a tilt that wobbles with its angle), then run the timed callbacks. */
s32 effUpdateFollowModelTransform(EffectObject *obj) {
    EffVec4 axis = D_0039F730;
    EffLocalNode node;
    EffectObjectData *data;
    void *model;
    s32 flag;

    data = obj->data;
    flag = 0;
    if (data->transitionWork != 0) {
        effObjInnerVecInit(&node);
        if (func_00222ED8(&node, obj) != 0) {
            effObjMulInnerThirdVec(obj, &node.vec60);
            effObjQuatMulInnerSecondVec(obj, &node.vec50);
            effObjAddInnerFirstVec(obj, &node.vec40);
        }
    }
    model = (void *)data->modelHolder->resourceHandle;
    if (model != NULL && effObjTestNodeFlags(obj->source, 1) == 1) {
        effObjClearNodeFlags(obj->source, 1);
        if (data->transitionWork != 0) {
            if (data->transitionWork->kind == 0 || data->transitionWork->kind == 3) {
                if (data->transitionWork->flags & 0x40) {
                    flag = 1;
                }
            }
            if (data->transitionWork->flags & 0x400000) {
                flag = 1;
            }
        }
        VU0_LOAD_VF(vf10, &obj->source[0x18]);
        if (data->transitionWork != 0 && flag != 0) {
            VU0_MOVE_VF(vf11, vf10);
            VU0_MOVE_VF(vf10, vf0);
            VU0_CLEAR_W(vf10);
            VU0_SCALAR_OP_CLOBBER(sdfSinPoly(data->transitionWork->angle) * 0.01f, "vaddx.x vf10, vf0, vf2x");
            VU0_SCALAR_OP_CLOBBER(sdfSinPoly(data->transitionWork->angle) * 0.004f, "vaddx.y vf10, vf0, vf2x");
            VU0_MUL(vf10, vf10, vf11);
            VU0_ADD(vf10, vf10, vf11);
        }
        mdlStoreTertiaryVectorVU(model);
        VU0_LOAD_VF(vf10, &obj->source[0x14]);
        effMiscNormalizeVU();
        if (data->transitionWork != 0 && flag != 0) {
            VU0_MOVE_VF(vf11, vf10);
            VU0_LOAD_VF(vf10, &axis);
            effMiscAxisAngleToQuaternionVU(sdfSinPoly(data->transitionWork->angle) * 0.006f);
            effMiscQuatMultiplyVU();
        }
        if (data->word18 & 1) {
            VU0_LOAD_VF(vf11, &D_0039F720);
            effMiscQuatMultiplyVU();
        }
        mdlUpdateContextRotationBasisFromQuaternion(model);
        if (effObjTestNodeFlags(obj->source, 8)) {
            VU0_LOAD_VF(vf10, &obj->source[0x1C]);
            VU0_SET_W_ONE(vf10);
        } else {
            VU0_LOAD_VF(vf10, &obj->source[0x10]);
            VU0_SET_W_ONE(vf10);
        }
        mdlStorePrimaryVectorVU(model);
        effObjInnerVecBackup(obj->source);
    }
    if (!dds3TestObjectFlags(obj, 0x100)) {
        func_00113AF0(obj);
    }
    if (dds3TestObjectFlags(obj, 0x2000)) {
        if (data->pendingValue != 0) {
            func_001131E0(obj, func_00117650(dds3FindWorldObjectNodeByKey(dds3GetWorldSecondaryObject(), data->pendingValue, 0x11)));
        } else {
            func_00113338(obj);
        }
    }
    return 1;
}

typedef struct FollowTargetInfo {
    u32 flags;       /* 0x00 */
    u8 pad04[0x14];
    s32 mapRecord;   /* 0x18 */
} FollowTargetInfo;

typedef struct FollowTarget {
    u8 pad00[0x8C];
    FollowTargetInfo *info; /* 0x8C */
    u8 pad90[0x43];
    u8 height;       /* 0xD3 */
    f32 offsetY;     /* 0xD4 */
} FollowTarget;



extern void func_001122F0(void *, EffectObject *);
extern void func_0011ECC8(EffectObject *);
extern s32 sdfLoadMapRecordPositionVector(s32, s32);
extern void func_0011E280(s32, f32, f32, f32, f32);
extern u8 D_00325788[];

s32 dds3UpdateEffectObjectFollowParameters(EffectObject *obj) {
    f32 vec[4];
    FollowTarget *target;
    MdlCtx *config;
    s32 level;
    s32 pickMode;

    memset(vec, 0, 0x10);
    vec[3] = 1.0f;
    if (dds3TestObjectFlags(obj, 1)) {
        return 1;
    }
    target = (FollowTarget *)effObjGetTransitionWork(obj);
    if (dds3TestObjectFlags(obj, 0x200) && target != NULL && !(target->info->flags & 1)) {
        func_0011ECC8(obj);
    }
    if ((s32)obj->data->word14 == -1) {
        func_001122F0(D_00325788, obj);
    } else {
        func_001122F0(D_00325788 + (s32)obj->data->word14 * 0x10, obj);
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
    config = (MdlCtx *)dds3GetEffectObjectModelHolder((EffWorldNode *)obj)->resourceHandle;
    if (dds3TestObjectFlags(obj, 0x4000)) {
        level = target->height;
    } else {
        level = config->inner->color >> 24;
    }
    pickMode = dds3TestObjectFlags(obj, 0x8000) != 0;
    if (sdfLoadMapRecordPositionVector(target->info->mapRecord, 0)) {
        VU0_STORE_VF(vf10, vec);
        vec[1] = pickMode == 1 ? target->offsetY : obj->source[0x11];
        func_0011E280(level, vec[0], vec[1], vec[2], obj->source[0x31]);
    } else {
        if (effObjTestNodeFlags(obj->source, 8)) {
            vec[0] = obj->source[0x1C];
            vec[1] = obj->source[0x1D];
            vec[2] = obj->source[0x1E];
        } else {
            vec[0] = obj->source[0x10];
            vec[1] = obj->source[0x11];
            vec[2] = obj->source[0x12];
        }
        if (pickMode == 1) {
            vec[1] = target->offsetY;
        }
        func_0011E280(level, vec[0], vec[1], vec[2], obj->source[0x31]);
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

extern void *dds3CopyWorldListToValueChain();
extern s32 dds3GetWorldValueCount(void *);
extern s32 dds3ResetObjectValueCursor(void *);
extern u32 dds3ReadIndexedWorldObjectWord(void *);
extern s32 dds3AdvanceObjectValueCursor(void *);
struct NodeB;
extern void dds3DestroyWorldIndexNode(struct NodeB *node);
extern s32 func_0010F9A8(f32 *, f32 *);
void func_00113AF0(EffectObject *obj) {
    EffectObjectData *data = obj->data;
    u64 world = dds3GetWorldSecondaryObject();
    void *list;
    EffectValueObject *other;
    EffectValueData *otherData;

    if (world == 0) {
        return;
    }
    list = dds3CopyWorldListToValueChain(world, 9);
    if (list == NULL) {
        return;
    }
    if (dds3GetWorldValueCount(list) == 0) {
        dds3DestroyWorldIndexNode((struct NodeB *)list);
        return;
    }
    if (dds3ResetObjectValueCursor(list) != 0) {
        do {
            other = (EffectValueObject *)dds3ReadIndexedWorldObjectWord(list);
            otherData = other->data;
            if (!(otherData->flags & 4) &&
                func_0010F9A8(obj->source, other->source) == 0 &&
                data->activeId == other->valueId) {
                evtEndUnitValueTransitionForObject((EffWorldNode *)obj, 10);
                data->activeId = -1;
            }
        } while (dds3AdvanceObjectValueCursor(list) != 0);
    }
    dds3DestroyWorldIndexNode((struct NodeB *)list);

    if (data->activeId == -1) {
        list = dds3CopyWorldListToValueChain(world, 9);
        if (dds3ResetObjectValueCursor(list) == 0) {
            goto destroy_list;
        }
        do {
            other = (EffectValueObject *)dds3ReadIndexedWorldObjectWord(list);
            otherData = other->data;
            if (!(otherData->flags & 4) &&
                func_0010F9A8(obj->source, other->source) != 0) {
                goto attach_transition;
            }
        } while (dds3AdvanceObjectValueCursor(list) != 0);

destroy_list:
        dds3DestroyWorldIndexNode((struct NodeB *)list);
        return;

attach_transition:
        evtSetUnitValueTransitionForObject(other, (EffWorldNode *)obj, 10);
        data->activeId = other->valueId;
        dds3DestroyWorldIndexNode((struct NodeB *)list);
    }
}

u32 effObjGetDataHandle(EffectObject *obj) {
    return obj->data->handle;
}

typedef struct WorldSubState {
    u8 unk0[0x10];   /* 0x0 */
    f32 vec[4];      /* 0x10 */
} WorldSubState;

typedef struct WorldObj {
    u8 unk0[4];           /* 0x0 */
    s32 unk4;             /* 0x4 */
    u8 unk8[0x10];        /* 0x8 */
    WorldSubState *state; /* 0x18 */
    f32 *source;           /* 0x1C */
} WorldObj;

extern WorldObj *dds3AppendWorldObjectNode();
extern void dds3EnsureSlotData();
extern void effObjSetInnerFirstVec();
extern void effObjSetInnerSecondVec();
extern void effObjInnerVecBackup();

WorldObj *dds3SpawnInnerVecObj6(s32 a, f32 *vec, void *second) {
    f32 zero[4];
    WorldObj *obj;

    memset(zero, 0, 0x10);
    zero[3] = 1.0f;
    obj = dds3AppendWorldObjectNode(6);
    obj->unk4 = a;
    dds3EnsureSlotData(obj);
    effObjSetInnerSecondVec(obj, second);
    effObjSetInnerFirstVec(obj, zero);
    effObjInnerVecBackup(obj->source);
    obj->state->vec[0] = vec[0];
    obj->state->vec[1] = vec[1];
    obj->state->vec[2] = vec[2];
    obj->state->vec[3] = vec[3];
    return obj;
}

void func_00113DA8(void) {
    dds3RemoveWorldObjectNode();
}

void effObjSetModelHolder(EffectObject *obj, u32 value) {
    obj->data->modelHolder = (ObjBase *)value;
}

void func_00113DD0(EffectObject *obj, u32 value) {
    obj->data->word04 = value;
}

u32 func_00113DE0(u64 id) {
    EffectObject *obj;
    u64 world;

    world = dds3GetWorldSecondaryObject();
    obj = (EffectObject *)dds3FindWorldObjectNodeByKey(world, id, 6);
    return obj->data->word04;
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

void func_00113E40(u32 value) {
    D_003BA9D0 = value;
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
    data->flags = 0;
    data->opacityMode = 0;
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

INCLUDE_RODATA(const s32, "game/code_001130E0", D_0039F720);

INCLUDE_RODATA(const s32, "game/code_001130E0", D_0039F730);

INCLUDE_ASM(const s32, "game/code_001130E0", func_00113F28);

extern void fldSelectDisplayBuffer(s32 index);
extern void fldSubmitFrameQuad(s32, s32, s32, s32, s32, s32, s32, s32);
extern EffectObject *fldPlayerObject;
extern u8 D_00325808[];

/* Submit the normal pass and the opacity-mode pass, temporarily neutralizing
 * the tint while the player is hidden. */
s32 effObjSubmitTransformOpacityPasses(EffectObject *obj) {
    EffectTransformData *data;
    u32 opacityMode;

    if (D_003BA9D0 == 0) {
        return 1;
    }
    data = (EffectTransformData *)obj->data;
    if (data->activeId == -1) {
        fldSelectDisplayBuffer(0x27);
        fldSubmitFrameQuad(1, 5, 0x60, 1, 0, 0, 1, 2);
        func_001122F0(D_00325788, obj);
        fldSelectDisplayBuffer(0x27);
        fldSubmitFrameQuad(1, 5, 0x80, 1, 0, 0, 1, 2);
    } else {
        fldSelectDisplayBuffer(0x22);
        fldSubmitFrameQuad(1, 5, 0x60, 1, 0, 0, 1, 2);
        opacityMode = data->opacityMode;
        if (opacityMode < 5) {
            if (opacityMode >= 3) {
                if (obj->color != 0x80808080) {
                    if (dds3TestObjectFlags(fldPlayerObject, 1)) {
                        u32 savedColor = obj->color;

                        obj->color = 0x80808080;
                        func_001122F0(D_00325788 + data->activeId * 0x10, obj);
                        obj->color = savedColor;
                    } else {
                        func_001122F0(D_00325808, obj);
                    }
                } else {
                    func_001122F0(D_00325788 + data->activeId * 0x10, obj);
                }
            }
        }
        func_001122F0(D_00325788 + data->activeId * 0x10, obj);
        fldSelectDisplayBuffer(0x22);
        fldSubmitFrameQuad(1, 5, 0x80, 1, 0, 0, 1, 2);
    }
    return 1;
}

/* Refresh the object's stored xyz from the source vector. */
void dds3RefreshStoredVec3(WorldObj *obj) {
    f32 *src = obj->source;
    WorldSubState *dst = obj->state;
    dst->vec[0] = src[0x10];
    dst->vec[1] = src[0x11];
    dst->vec[2] = src[0x12];
}

struct EffEventWork;
extern void effDestroyNode(struct EffNode *);
extern void billDispatchByKind(BillObj *);
extern void effEventReleaseNode(struct EffEventWork *);
extern void func_00190118(SoundMixer *);
extern void sdfReleaseChipBlock(void *);

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
            func_00190118(data->handle);
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

INCLUDE_SDATA(const s32, "game/code_001130E0", D_003BA9D0);

