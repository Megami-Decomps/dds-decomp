#include "common.h"
#include "pcp_vu0.h"

typedef struct EffectObjectData {
    u32 handle;
    u32 word04;
    u32 word08;
    u32 word0C;
    s32 activeId;
    u32 word14;
    u32 word18;
    u32 pendingValue;
    u32 timer;
} EffectObjectData;

typedef struct EffectObject {
    u8 pad00[0x18];
    EffectObjectData *data;
    f32 *source; /* 0x1C */
} EffectObject;

extern u32 D_003BA9D0;

extern u64 dds3GetWorldSecondaryObject(void);

extern s32 dds3FindWorldObjectNodeByKey(u64, u64, u64);

u32 func_001130E0(EffectObject *obj) {
    return obj->data->handle;
}

u32 func_001130F0(EffectObject *obj) {
    return obj->data->word08;
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

INCLUDE_ASM(const s32, "game/code_001130E0", func_00113490);

void evtDestroyEffectObjectData(EffectObject *obj) {
    EffectObjectData *data;

    evtEndObjectValueTransition();
    effObjFreeInner(obj);
    data = obj->data;
    if (data->handle != -1) {
        data->handle = -1;
    }
    if (data->word08 != 0) {
        evtReleaseUnitTransitionWork(data->word08);
        data->word08 = 0;
    }
    func_00111B40(obj);
    func_00111840(data->word0C);
    sdfReleaseChipBlock(obj->data);
}

INCLUDE_ASM(const s32, "game/code_001130E0", func_001135B0);

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

extern s32 dds3TestObjectFlags(EffectObject *, s32);
extern void func_001122F0(void *, EffectObject *);
extern void func_0011ECC8(EffectObject *);
extern FollowTransition *func_00113008(EffectObject *);
extern s32 sdfLoadMapRecordPositionVector(s32, s32);
extern s32 effObjTestNodeFlags(f32 *, s32);
extern void func_0011E280(s32, f32, f32, f32, f32);
extern u8 D_00325788[];

s32 func_00113888(EffectObject *obj) {
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
    target = (FollowTarget *)func_001130F0(obj);
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

u32 func_00113CD8(EffectObject *obj) {
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

void func_00113DC0(EffectObject *obj, u32 value) {
    obj->data->word0C = value;
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
    obj->data->word08 = value;
}

u32 evtGetObjectTransitionWork(EffectObject *obj) {
    return obj->data->word08;
}

void func_00113E40(u32 value) {
    D_003BA9D0 = value;
}

INCLUDE_ASM(const s32, "game/code_001130E0", func_00113E48);

void evtReleaseEffectObjectHandleAndData(EffectObject *obj) {
    EffectObjectData *data;

    effObjFreeInner();
    data = obj->data;
    func_00111840(data->handle);
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

extern void effObjInnerCreate();
extern void *func_002CFEB8(s32 size);
extern u32 func_001117A8();

s32 evtInitializeEffectObjectData(EffectObject *obj) {
    EffectObjectData *data;

    effObjInnerCreate(obj);
    obj->data = func_002CFEB8(0x50);
    memset(obj->data, 0, 0x50);
    data = obj->data;
    data->handle = func_001117A8(obj);
    dds3SetObjectFlags(obj, 0x60);
    return 1;
}

INCLUDE_SDATA(const s32, "game/code_001130E0", D_003BA9D0);

