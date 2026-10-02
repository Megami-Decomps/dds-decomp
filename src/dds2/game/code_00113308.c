#include "common.h"
#include "pcp_vu0.h"

extern u64 dds3GetWorldSecondaryObject(void);

extern s32 dds3FindWorldObjectNodeByKey(u64, u64, u64);

extern u32 D_00435DA0;

/* Follow record the object tracks (angle, flags and kind at the end of a longer record). */
typedef struct EffFollowRec {
    u8 pad00[0x98];
    f32 angle;   /* 0x98 */
    u8 pad9C[0xC];
    u32 flags;   /* 0xA8 */
    s16 kind;    /* 0xAC */
} EffFollowRec;

typedef struct EffModelHolder {
    u8 pad00[0xC];
    void *model; /* 0x0C */
} EffModelHolder;

typedef struct EffectObjectData {
    u32 handle;
    u32 word04;
    EffFollowRec *transitionWork; /* 0x08: object transition work */
    EffModelHolder *modelHolder; /* 0x0C: effect model holder */
    s32 activeId;
    u32 word14;
    u32 word18;
    s32 pendingValue;
    u32 timer;
    u32 word24;
    u32 word28;
    f32 limitMin2C;
    f32 limitMax30;
    u32 word34;
    f32 limitMin38;
    f32 limitMax3C;
} EffectObjectData;

typedef struct EffectTransformData {
    void *resourceState;
    u32 flags;
    s32 opacityMode;
    s32 activeId;
    f32 offset[4];
    f32 position[4];
} EffectTransformData;

typedef struct EffectObject {
    u8 pad00[0x18];
    EffectObjectData *data;
    f32 *source; /* 0x1C */
} EffectObject;

typedef struct WorldSubState {
    u8 pad00[0x10];
    f32 vec[4];
} WorldSubState;

typedef struct WorldObj {
    u8 pad00[4];
    s32 unk4;             /* 0x4 world counter copied on spawn */
    u8 pad08[0x10];
    WorldSubState *state; /* 0x18 */
    f32 *source;          /* 0x1C */
} WorldObj;

extern void effObjInnerCreate();

extern void *func_00328D68(s32 size);

extern void *dds3CreateSlotResourceState();
extern void dds3SetObjectFlags(EffectObject *, s32);

u32 dds3GetEffectDataHandle(EffectObject *object) {
    return object->data->handle;
}

u32 func_00113318(EffectObject *object) {
    return (u32)object->data->transitionWork;
}

void func_00113328(EffectObject *object, u32 value) {
    object->data->word14 = value;
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

INCLUDE_ASM(const s32, "game/code_00113308", func_00113408);

INCLUDE_ASM(const s32, "game/code_00113308", func_00113560);

void evtArmEffectObjectPendingValue(EffectObject *object, u32 value) {
    EffectObjectData *data;

    data = object->data;
    dds3SetObjectFlags(object, 0x2000);
    data->pendingValue = value;
    data->timer = 0;
}

void evtResetObjectPendingValue(EffectObject *object) {
    EffectObjectData *data;

    data = object->data;
    data->timer = 0x1e;
    data->pendingValue = 0;
}

s32 func_001136B8(EffectObject *object) {
    EffectObjectData *data;
    void *work;

    effObjInnerCreate(object);
    work = func_00328D68(sizeof(EffectObjectData));
    object->data = work;
    memset(work, 0, sizeof(EffectObjectData));
    data = object->data;
    data->modelHolder = (EffModelHolder *)dds3CreateSlotResourceState(object);
    data->transitionWork = NULL;
    data->activeId = -1;
    data->word14 = -1;
    data->pendingValue = 0;
    data->timer = 0;
    data->word28 = 0;
    data->limitMin2C = -45.0f;
    data->limitMax30 = 45.0f;
    data->word34 = 0;
    data->limitMin38 = -45.0f;
    data->limitMax3C = 45.0f;
    dds3SetObjectFlags(object, 0x42);
    return 1;
}

void evtDestroyEffectObjectData(EffectObject *object) {
    EffectObjectData *data;

    evtEndObjectValueTransition();
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
extern s32 dds3TestObjectFlags(EffectObject *, s32);
extern s32 effObjTestNodeFlags(f32 *, s32);
extern void effObjInnerVecInit(EffLocalNode *);
extern s32 func_0023DA70(EffLocalNode *, EffectObject *);
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
extern void func_00113D18(EffectObject *);
extern void func_00113560(EffectObject *);
extern void func_00113408(EffectObject *, s32);
extern s32 func_001178B8(s32);

/* Per-frame refresh of a model effect object: rebuild the child transform from the follow record (a tilt that wobbles with its angle), then run the timed callbacks. */
s32 effUpdateFollowModelTransform(EffectObject *obj) {
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
    model = data->modelHolder->model;
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
            VU0_LOAD_VF(vf11, &D_004128A0);
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
        func_00113D18(obj);
    }
    if (dds3TestObjectFlags(obj, 0x2000)) {
        if (data->pendingValue != 0) {
            func_00113408(obj, func_001178B8(dds3FindWorldObjectNodeByKey(dds3GetWorldSecondaryObject(), data->pendingValue, 0x11)));
        } else {
            func_00113560(obj);
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

typedef struct FollowLevel {
    u8 pad00[0x1F];
    u8 value;        /* 0x1F */
} FollowLevel;

typedef struct FollowConfig {
    u8 pad00[0x18];
    FollowLevel *level; /* 0x18 */
} FollowConfig;

typedef struct FollowTransition {
    u8 pad00[0xC];
    FollowConfig *config; /* 0x0C */
} FollowTransition;

extern void func_00112518(void *, EffectObject *);
extern void func_00120B88(EffectObject *);
extern FollowTransition *func_00113230(EffectObject *);
extern s32 sdfLoadMapRecordPositionVector(s32, s32);
extern void func_001200E8(s32, f32, f32, f32, f32);
extern u8 D_00380788[];

s32 dds3UpdateEffectObjectFollowParameters(EffectObject *obj) {
    f32 vec[4];
    FollowTarget *target;
    FollowConfig *config;
    s32 level;
    s32 pickMode;

    memset(vec, 0, 0x10);
    vec[3] = 1.0f;
    if (dds3TestObjectFlags(obj, 1)) {
        return 1;
    }
    target = (FollowTarget *)func_00113318(obj);
    if (dds3TestObjectFlags(obj, 0x200) && target != NULL && !(target->info->flags & 1)) {
        func_00120B88(obj);
    }
    if ((s32)obj->data->word14 == -1) {
        func_00112518(D_00380788, obj);
    } else {
        func_00112518(D_00380788 + (s32)obj->data->word14 * 0x10, obj);
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
    config = func_00113230(obj)->config;
    if (dds3TestObjectFlags(obj, 0x4000)) {
        level = target->height;
    } else {
        level = config->level->value;
    }
    pickMode = dds3TestObjectFlags(obj, 0x8000) != 0;
    if (sdfLoadMapRecordPositionVector(target->info->mapRecord, 0)) {
        VU0_STORE_VF(vf10, vec);
        vec[1] = pickMode == 1 ? target->offsetY : obj->source[0x11];
        func_001200E8(level, vec[0], vec[1], vec[2], obj->source[0x31]);
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
        func_001200E8(level, vec[0], vec[1], vec[2], obj->source[0x31]);
    }
    return 1;
}

void evtEndObjectValueTransition(EffectObject *object) {
    EffectObjectData *data;

    data = object->data;
    if (data->activeId != -1) {
        evtEndUnitValueTransitionForObject(object, 10);
        data->activeId = 0xffffffff;
    }
}

INCLUDE_ASM(const s32, "game/code_00113308", func_00113D18);

u32 func_00113F00(EffectObject *object) {
    return object->data->handle;
}

extern WorldObj *dds3AppendWorldObjectNode();

extern void dds3EnsureSlotData();

extern void effObjSetInnerFirstVec();

extern void effObjSetInnerSecondVec();

extern void effObjInnerVecBackup();

/* Spawn a world object of kind 6 and seed its stored vector. */
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

void func_00113FD0(void) {
    dds3RemoveWorldObjectNode();
}

void func_00113FE8(EffectObject *object, u32 value) {
    object->data->modelHolder = (EffModelHolder *)value;
}

void func_00113FF8(EffectObject *object, u32 value) {
    object->data->word04 = value;
}

u32 func_00114008(u64 id) {
    EffectObject *object;
    u64 world;

    world = dds3GetWorldSecondaryObject();
    object = (EffectObject *)dds3FindWorldObjectNodeByKey(world, id, 6);
    return object->data->word04;
}

void evtSetObjectTransitionWork(EffectObject *object, u32 value) {
    object->data->transitionWork = (EffFollowRec *)value;
}

u32 evtGetObjectTransitionWork(EffectObject *object) {
    return (u32)object->data->transitionWork;
}

void func_00114068(u32 value) {
    D_00435DA0 = value;
}

s32 func_00114070(EffectObject *object) {
    EffectTransformData *data;
    void *work;

    effObjInnerCreate(object);
    work = func_00328D68(sizeof(EffectTransformData));
    object->data = work;
    memset(work, 0, sizeof(EffectTransformData));
    data = (EffectTransformData *)object->data;
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

void evtReleaseEffectObjectHandleAndData(EffectObject *object) {
    EffectTransformData *data;

    effObjFreeInner();
    data = (EffectTransformData *)object->data;
    dds3DestroyObjectBase(data->resourceState);
    sdfReleaseChipBlock(data);
}

INCLUDE_RODATA(const s32, "game/code_00113308", D_004128A0);

INCLUDE_RODATA(const s32, "game/code_00113308", D_004128B0);

INCLUDE_ASM(const s32, "game/code_00113308", func_00114150);

INCLUDE_ASM(const s32, "game/code_00113308", func_00114428);

/* Refresh the object's stored xyz from the source vector. */
void dds3RefreshStoredVec3(WorldObj *obj) {
    f32 *src = obj->source;
    WorldSubState *dst = obj->state;
    dst->vec[0] = src[0x10];
    dst->vec[1] = src[0x11];
    dst->vec[2] = src[0x12];
}

INCLUDE_ASM(const s32, "game/code_00113308", func_00114640);

s32 evtInitializeEffectObjectData(EffectObject *obj) {
    EffectObjectData *data;

    effObjInnerCreate(obj);
    obj->data = func_00328D68(0x50);
    memset(obj->data, 0, 0x50);
    data = obj->data;
    data->handle = (u32)dds3CreateSlotResourceState(obj);
    dds3SetObjectFlags(obj, 0x60);
    return 1;
}

INCLUDE_SDATA(const s32, "game/code_00113308", D_00435DA0);

