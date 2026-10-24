#include "sdk_ps2.h"
#include "common.h"
#include "sdf_chip.h"
#include "dds3_path.h"
#include "dds3obj.h"
#include "eff_transform.h"
#include "eff.h"
#include "dds3_path_state.h"

#include "dds3_world_secondary.h"
typedef struct ActionSub {
    u8 unk0[0x10];
    s32 unk10;
} ActionSub;


dds3Basic_t *dds3AddModelRangeBasicNew(s32 objectKey, void *resourceData, const f32 *position, const char *objectName) {
    dds3Basic_t *obj = dds3AddSceneBasicData(EFF_WORLD_KIND_ACTION_B);

    obj->value = objectName;
    ((ActionSub *)obj->data)->unk10 = 0;
    obj->key = objectKey;
    return obj;
}

void dds3SetPathBasicSpline(dds3Basic_t *path, u32 value) {
    ((Dds3PathStatePayload *)path->data)->value = value;
}

u32 dds3SearchSceneBasicWorkData(u32 id) {
    dds3Basic_t *path;
    dds3Basic_t *world;

    world = dds3GetControlScene();
    path = dds3SearchSceneBasic(world, id, 6);
    return ((Dds3PathStatePayload *)path->data)->value;
}

dds3Basic_t *dds3AddEventBasic(s32 key, const char *resourcePath, const char *name) {
    dds3Basic_t *obj = dds3AddSceneBasicData(EFF_WORLD_KIND_ACTION_D);

    obj->data = resourcePath;
    obj->key = key;
    obj->value = name;
    return obj;
}

const char *dds3EventBasicGetResource(dds3Basic_t *object) {
    return object->data;
}

void dds3PathEvalSpline(u32 *segment, f32 *weight, const Dds3PathKeyframes *keys, f32 frame) {
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

dds3Basic_t *dds3AddPathBasic(s32 key, void *work, const char *name) {
    dds3Basic_t *obj = dds3AddSceneBasicData(EFF_WORLD_KIND_ACTION_10);

    obj->key = key;
    obj->value = name;
    obj->data = work;
    return obj;
}

extern EffPrimitiveCurve *dds3AllocSplineHandle(f32 *data, u32 count, s32 mode);

dds3PathBasicControl_t *dds3AllocPathBasicWorkData(dds3Basic_t *object) {
    Dds3PathCurveTable *table = object->data;
    Dds3PathCurveEntry *entry;
    Dds3PathKeyframes *keys;
    dds3PathBasicControl_t *work;
    Dds3PathPositionSample *positionSamples;
    u32 i;

    if (table->count == 0) {
        return NULL;
    }
    entry = table->entries;
    work = sdfChipAllocDirect(sizeof(*work));
    i = 0;
    memset(work, 0, sizeof(*work));
    work->counter.direction = 0;
    work->counter.time = 0.0f;
    work->counter.duration = 0.0f;
    for (; i < table->count; i++, entry++) {
        u32 lastFrame;

        keys = entry->keys;
        switch (entry->kind) {
        case DDS3_PATH_CURVE_SCALAR:
            work->scalarKeys = keys;
            work->counter.flags |= DDS3_PATH_SCALAR_CHANNEL;
            break;
        case DDS3_PATH_CURVE_POSITION:
            positionSamples = keys->data.position;
            work->counter.flags |= DDS3_PATH_POSITION_CHANNEL;
            work->positionKeys = keys;
            work->primitiveCurve = dds3AllocSplineHandle(positionSamples[0], keys->count, 1);
            break;
        case DDS3_PATH_CURVE_ROTATION:
            work->rotationKeys = keys;
            work->counter.flags |= DDS3_PATH_ROTATION_CHANNEL;
            break;
        case DDS3_PATH_CURVE_WORLD_TRANSFORM:
            work->transformKeys = keys;
            work->counter.flags |= DDS3_PATH_WORLD_TRANSFORM_CHANNEL;
            break;
        }
        lastFrame = keys->frames[keys->count - 1];
        if (work->counter.duration < (f32)lastFrame) {
            work->counter.duration = (f32)lastFrame;
        }
    }
    return work;
}
