#include "common.h"

extern u64 dds3GetWorldSecondaryObject(void);

extern s32 func_00110C70(u64, u64, u64);

extern u32 D_00435DA0;

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

extern u32 func_001119D0();

u32 func_00113308(EffectObject *object) {
    return object->data->handle;
}

u32 func_00113318(EffectObject *object) {
    return object->data->word08;
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

INCLUDE_ASM(const s32, "game/code_00113308", func_001136B8);

void evtDestroyEffectObjectData(EffectObject *object) {
    EffectObjectData *data;

    evtEndObjectValueTransition();
    effObjFreeInner(object);
    data = object->data;
    if (data->handle != -1) {
        data->handle = -1;
    }
    if (data->word08 != 0) {
        evtReleaseUnitTransitionWork(data->word08);
        data->word08 = 0;
    }
    func_00111D68(object);
    func_00111A68(data->word0C);
    sdfReleaseChipBlock(object->data);
}

INCLUDE_ASM(const s32, "game/code_00113308", func_001137D8);

INCLUDE_ASM(const s32, "game/code_00113308", func_00113AB0);

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

extern WorldObj *func_00110AA8();

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
    obj = func_00110AA8(6);
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
    func_00110B50();
}

void func_00113FE8(EffectObject *object, u32 value) {
    object->data->word0C = value;
}

void func_00113FF8(EffectObject *object, u32 value) {
    object->data->word04 = value;
}

u32 func_00114008(u64 id) {
    EffectObject *object;
    u64 world;

    world = dds3GetWorldSecondaryObject();
    object = (EffectObject *)func_00110C70(world, id, 6);
    return object->data->word04;
}

void func_00114048(EffectObject *object, u32 value) {
    object->data->word08 = value;
}

u32 func_00114058(EffectObject *object) {
    return object->data->word08;
}

void func_00114068(u32 value) {
    D_00435DA0 = value;
}

INCLUDE_ASM(const s32, "game/code_00113308", func_00114070);

void evtReleaseEffectObjectHandleAndData(EffectObject *object) {
    EffectObjectData *data;

    effObjFreeInner();
    data = object->data;
    func_00111A68(data->handle);
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
    data->handle = func_001119D0(obj);
    dds3SetObjectFlags(obj, 0x60);
    return 1;
}

INCLUDE_SDATA(const s32, "game/code_00113308", D_00435DA0);

