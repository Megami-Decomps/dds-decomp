#include "common.h"
#include "sdf_chip.h"
#include "dds3_path.h"
#include "dds3obj.h"
#include "eff_transform.h"
#include "eff.h"

extern void *dds3GetWorldSecondaryObject(void);

typedef struct {
    u8 pad00[0x10];
    u32 value;
} PathState;

typedef struct {
    u8 pad00[0x18];
    PathState *state;
} PathObject;


typedef struct ActionSub {
    u8 unk0[0x10];
    s32 unk10;
} ActionSub;


EffWorldNode *evtSpawnActionObjB(s32 a, s32 b, s32 c, s32 d) {
    EffWorldNode *obj = dds3AppendWorldObjectNode(EFF_WORLD_KIND_ACTION_B);

    obj->value = d;
    ((ActionSub *)obj->data)->unk10 = 0;
    obj->key = a;
    return obj;
}

void dds3SetPathStateValue(PathObject *path, u32 value) {
    path->state->value = value;
}

u32 dds3GetPathStateValueById(u32 id) {
    PathObject *path;
    EffWorldNode *world;

    world = dds3GetWorldSecondaryObject();
    path = (PathObject *)dds3FindWorldObjectNodeByKey(world, id, 6);
    return path->state->value;
}

EffWorldNode *evtSpawnActionObjD(s32 a, void *work, s32 c) {
    EffWorldNode *obj = dds3AppendWorldObjectNode(EFF_WORLD_KIND_ACTION_D);

    obj->data = work;
    obj->key = a;
    obj->value = c;
    return obj;
}

void *dds3GetWorldObjectPayload(EffWorldNode *object) {
    return object->data;
}

void dds3SamplePathKeyframeInterval(u32 *segment, f32 *weight, Dds3PathKeyframes *keys, f32 frame) {
    u32 count = keys->count;
    u32 i;
    u32 *frames = keys->frames;

    for (i = 0; i < count - 1; i++) {
        if ((f32)frames[i] <= frame && frame < (f32)frames[i + 1]) {
            *weight = 1.0f / (f32)(frames[i + 1] - frames[i]);
            *segment = i;
            *weight *= frame - (f32)frames[i];
            return;
        }
    }
    *segment = count - 2;
    *weight = 1.0f;
}

EffWorldNode *evtSpawnActionObj10(s32 a, void *work, s32 c) {
    EffWorldNode *obj = dds3AppendWorldObjectNode(EFF_WORLD_KIND_ACTION_10);

    obj->key = a;
    obj->value = c;
    obj->data = work;
    return obj;
}

typedef struct Dds3PathCurveEntry {
    u32 kind;
    Dds3PathKeyframes *keys;
} Dds3PathCurveEntry;

typedef struct Dds3PathCurveTable {
    u32 count;
    Dds3PathCurveEntry entries[1];
} Dds3PathCurveTable;

extern void *memset(void *destination, s32 value, u32 bytes);
extern EffPrimitiveCurve *effCreatePrimitiveCurve(f32 *data, u32 count, s32 mode);

Dds3PathCurveWork *dds3CreatePathCurveWork(EffWorldNode *object) {
    Dds3PathCurveTable *table = object->data;
    Dds3PathCurveEntry *entry;
    Dds3PathKeyframes *keys;
    Dds3PathCurveWork *work;
    u32 i;

    if (table->count == 0) {
        return NULL;
    }
    entry = table->entries;
    work = sdfAllocSizeClassBlock(sizeof(*work));
    i = 0;
    memset(work, 0, sizeof(*work));
    work->direction = 0;
    work->time = 0.0f;
    work->duration = 0.0f;
    for (; i < table->count; i++, entry++) {
        u32 lastFrame;

        keys = entry->keys;
        switch (entry->kind) {
        case DDS3_PATH_CURVE_SCALAR:
            work->scalarKeys = keys;
            work->flags |= DDS3_PATH_SCALAR_CHANNEL;
            break;
        case DDS3_PATH_CURVE_POSITION:
            work->flags |= DDS3_PATH_POSITION_CHANNEL;
            work->positionKeys = keys;
            work->primitiveCurve = effCreatePrimitiveCurve(keys->data, keys->count, 1);
            break;
        case DDS3_PATH_CURVE_ROTATION:
            work->rotationKeys = keys;
            work->flags |= DDS3_PATH_ROTATION_CHANNEL;
            break;
        case DDS3_PATH_CURVE_WORLD_TRANSFORM:
            work->transformKeys = keys;
            work->flags |= DDS3_PATH_WORLD_TRANSFORM_CHANNEL;
            break;
        }
        lastFrame = keys->frames[keys->count - 1];
        if (work->duration < (f32)lastFrame) {
            work->duration = (f32)lastFrame;
        }
    }
    return work;
}
