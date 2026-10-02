#include "common.h"
#include "pcp_vu0.h"

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

extern u32 D_003BA9D0;

extern u64 dds3GetWorldSecondaryObject(void);

extern s32 dds3FindWorldObjectNodeByKey(u64, u64, u64);
extern void effObjInnerCreate();
extern void *sdfAllocSizeClassBlock(s32 size);
extern void *dds3CreateSlotResourceState();
extern void dds3SetObjectFlags(EffectObject *, s32);

u32 dds3GetEffectDataHandle(EffectObject *obj) {
    return obj->data->handle;
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

INCLUDE_ASM(const s32, "game/code_001130E0", func_001131E0);

INCLUDE_ASM(const s32, "game/code_001130E0", func_00113338);

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

    effObjInnerCreate(object);
    work = sdfAllocSizeClassBlock(sizeof(EffectObjectData));
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

void evtDestroyEffectObjectData(EffectObject *obj) {
    EffectObjectData *data;

    evtEndObjectValueTransition();
    effObjFreeInner(obj);
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
extern s32 dds3TestObjectFlags(EffectObject *, s32);
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
extern void func_001131E0(EffectObject *, s32);
extern s32 func_00117650(s32);

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

extern void func_001122F0(void *, EffectObject *);
extern void func_0011ECC8(EffectObject *);
extern FollowTransition *func_00113008(EffectObject *);
extern s32 sdfLoadMapRecordPositionVector(s32, s32);
extern void func_0011E280(s32, f32, f32, f32, f32);
extern u8 D_00325788[];

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
    config = func_00113008(obj)->config;
    if (dds3TestObjectFlags(obj, 0x4000)) {
        level = target->height;
    } else {
        level = config->level->value;
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

void evtEndObjectValueTransition(EffectObject *obj) {
    EffectObjectData *data;

    data = obj->data;
    if (data->activeId != -1) {
        evtEndUnitValueTransitionForObject(obj, 10);
        data->activeId = 0xffffffff;
    }
}

INCLUDE_ASM(const s32, "game/code_001130E0", func_00113AF0);

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
    obj->data->modelHolder = (EffModelHolder *)value;
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

void evtSetObjectTransitionWork(EffectObject *obj, u32 value) {
    obj->data->transitionWork = (EffFollowRec *)value;
}

u32 evtGetObjectTransitionWork(EffectObject *obj) {
    return (u32)obj->data->transitionWork;
}

void func_00113E40(u32 value) {
    D_003BA9D0 = value;
}

s32 effObjInitializeTransformData(EffectObject *object) {
    EffectTransformData *data;
    void *work;

    effObjInnerCreate(object);
    work = sdfAllocSizeClassBlock(sizeof(EffectTransformData));
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

void evtReleaseEffectObjectHandleAndData(EffectObject *obj) {
    EffectTransformData *data;

    effObjFreeInner();
    data = (EffectTransformData *)obj->data;
    dds3DestroyObjectBase(data->resourceState);
    sdfReleaseChipBlock(data);
}

INCLUDE_RODATA(const s32, "game/code_001130E0", D_0039F720);

INCLUDE_RODATA(const s32, "game/code_001130E0", D_0039F730);

INCLUDE_ASM(const s32, "game/code_001130E0", func_00113F28);

INCLUDE_ASM(const s32, "game/code_001130E0", func_001141C0);

/* Refresh the object's stored xyz from the source vector. */
void dds3RefreshStoredVec3(WorldObj *obj) {
    f32 *src = obj->source;
    WorldSubState *dst = obj->state;
    dst->vec[0] = src[0x10];
    dst->vec[1] = src[0x11];
    dst->vec[2] = src[0x12];
}

INCLUDE_ASM(const s32, "game/code_001130E0", func_001143D8);


s32 evtInitializeEffectObjectData(EffectObject *obj) {
    EffectObjectData *data;

    effObjInnerCreate(obj);
    obj->data = sdfAllocSizeClassBlock(0x50);
    memset(obj->data, 0, 0x50);
    data = obj->data;
    data->handle = (u32)dds3CreateSlotResourceState(obj);
    dds3SetObjectFlags(obj, 0x60);
    return 1;
}

INCLUDE_SDATA(const s32, "game/code_001130E0", D_003BA9D0);

