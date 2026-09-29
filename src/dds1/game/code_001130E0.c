#include "common.h"

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

extern u32 D_003BA9D0;

extern u64 dds3GetWorldSecondaryObject(void);

extern s32 func_00110A48(u64, u64, u64);

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

void func_00113438(EffectObject *obj, u32 value) {
    EffectObjectData *data;

    data = obj->data;
    dds3SetObjectFlags(obj, 0x2000);
    data->pendingValue = value;
    data->timer = 0;
}

void func_00113478(EffectObject *obj) {
    EffectObjectData *data;

    data = obj->data;
    data->timer = 0x1e;
    data->pendingValue = 0;
}

INCLUDE_ASM(const s32, "game/code_001130E0", func_00113490);

void func_00113538(EffectObject *obj) {
    EffectObjectData *data;

    func_00113AA8();
    effObjFreeInner(obj);
    data = obj->data;
    if (data->handle != -1) {
        data->handle = -1;
    }
    if (data->word08 != 0) {
        func_00222200(data->word08);
        data->word08 = 0;
    }
    func_00111B40(obj);
    func_00111840(data->word0C);
    func_002CFF98(obj->data);
}

INCLUDE_ASM(const s32, "game/code_001130E0", func_001135B0);

INCLUDE_ASM(const s32, "game/code_001130E0", func_00113888);

void func_00113AA8(EffectObject *obj) {
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

extern WorldObj *func_00110880();
extern void dds3EnsureSlotData();
extern void effObjSetInnerFirstVec();
extern void effObjSetInnerSecondVec();
extern void effObjInnerVecBackup();

WorldObj *dds3SpawnInnerVecObj6(s32 a, f32 *vec, void *second) {
    f32 zero[4];
    WorldObj *obj;

    memset(zero, 0, 0x10);
    zero[3] = 1.0f;
    obj = func_00110880(6);
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
    func_00110928();
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
    obj = (EffectObject *)func_00110A48(world, id, 6);
    return obj->data->word04;
}

void func_00113E20(EffectObject *obj, u32 value) {
    obj->data->word08 = value;
}

u32 func_00113E30(EffectObject *obj) {
    return obj->data->word08;
}

void func_00113E40(u32 value) {
    D_003BA9D0 = value;
}

INCLUDE_ASM(const s32, "game/code_001130E0", func_00113E48);

void func_00113EE8(EffectObject *obj) {
    EffectObjectData *data;

    effObjFreeInner();
    data = obj->data;
    func_00111840(data->handle);
    func_002CFF98(data);
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

s32 func_00114508(EffectObject *obj) {
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

