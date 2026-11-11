#include "sdk_ps2.h"
#include "common.h"
#include "sdf_chip.h"
#include "dds3_path.h"
#include "dds3obj.h"
#include "eff_transform.h"
#include "eff.h"
#include "dds3_path_state.h"
#include "dds3_world_secondary.h"
#include "ps2asm.h"
#include "eff_quaternion_nlerp.h"
#include "evt_run_context_api.h"
#include "scr.h"
#include "sdf_channel.h"
#include "scr_work_task_api.h"
#include "dat_save_words.h"
#include "sdf_float_counter.h"
#include "eff_random_mod.h"
#include "dds3_task_create.h"
#include "sdf_heap_api.h"
#include "mnu_flag_snapshot.h"
#include "evt_world_source_transform.h"
#include "btl_action.h"
#include "dat_state.h"
#include "dat_affinity_selector.h"
#include "dat_effective_affinity_api.h"
#include "dat_world_field_flags.h"
#include "dat_unit_has_skill_api.h"
#include "evt_randomize_entry_api.h"
#include "dat_command.h"
#include "sdf.h"
#include "sdf_dev_state.h"
#include "dds3_task_lifecycle.h"
#include "fld_runtime_reset_api.h"
#include "mdl_flag_api.h"

#include "sdf_debug_api.h"

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

dds3Basic_t *dds3AddPathBasic(s32 key, Dds3PathCurveTable *work, const char *name) {
    dds3Basic_t *obj = dds3AddSceneBasicData(EFF_WORLD_KIND_ACTION_10);

    obj->key = key;
    obj->name = name;
    obj->workdata = work;
    return obj;
}

extern EffPrimitiveCurve *dds3AllocSplineHandle(f32 *data, u32 count, s32 mode);

dds3PathBasicControl_t *dds3AllocPathBasicWorkData(dds3Basic_t *object) {
    Dds3PathCurveTable *table = object->workdata;
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

void dds3FreeSplineHandle(struct EffPrimitiveCurve *primitive);

void dds3DeletePathControl(dds3PathBasicControl_t *path) {
    dds3FreeSplineHandle(path->primitiveCurve);
    sdfChipFreeDirect(path);
}

/* vu0 routine: interpolate the path's XYZ keys into vf10. */
void dds3PathBasicControlGetPos(dds3PathBasicControl_t *path) {
    u32 segmentIndex;
    f32 interpolationWeight;
    Dds3PathKeyframes *keyframes;
    Dds3PathPositionSample *entries;

    if (path->counter.flags & DDS3_PATH_POSITION_CHANNEL) {
        keyframes = path->positionKeys;
        dds3PathEvalSpline(&segmentIndex, &interpolationWeight, keyframes, path->counter.time);
        entries = keyframes->data.position;
        SDF_VU0_SETVECTOR_X(entries[segmentIndex + 1][0]);
        SDF_VU0_SETVECTOR_Y(entries[segmentIndex + 1][1]);
        SDF_VU0_SETVECTOR_Z(entries[segmentIndex + 1][2]);
        SDF_VU0_SCALEVECTOR_V0(interpolationWeight);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_SETVECTOR_X(entries[segmentIndex][0]);
        SDF_VU0_SETVECTOR_Y(entries[segmentIndex][1]);
        SDF_VU0_SETVECTOR_Z(entries[segmentIndex][2]);
        SDF_VU0_SCALEVECTOR_V0(1.0f - interpolationWeight);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_VECTOR_W_SET_ZERO();
    } else {
        SDF_VU0_LOADIDENTITYVECTOR();
    }
}

void dds3PathBasicControlGetRot(dds3PathBasicControl_t *path) {
    u32 segmentIndex;
    f32 interpolationWeight;
    Dds3PathKeyframes *keyframes;
    Dds3PathRotationSample *entries;
    Dds3PathRotationSample *firstKey;
    Dds3PathRotationSample *secondKey;
    if (path->counter.flags & DDS3_PATH_ROTATION_CHANNEL) {
        keyframes = path->rotationKeys;
        dds3PathEvalSpline(&segmentIndex, &interpolationWeight, keyframes, path->counter.time);
        entries = keyframes->data.rotation;
        firstKey = &entries[segmentIndex];
        SDF_VU0_LOADVECTOR_BARRIER(firstKey);
        secondKey = &entries[segmentIndex] + 1;
        SDF_VU0_LOADVECTOR_B_BARRIER(secondKey);
        Lerp(interpolationWeight);
    } else {
        SDF_VU0_LOADIDENTITYVECTOR();
    }
}

/* Interpolate rotation, position, and scale at the current path time, or clear the output. */
void dds3PathBasicControlNextStep(const dds3PathBasicControl_t *path, WorldTransformParams *transformParams) {
    u32 segmentIndex;
    f32 interpolationWeight;
    const Dds3PathKeyframes *keyframes;
    const WorldTransformParams *entries;

    if (path->counter.flags & DDS3_PATH_WORLD_TRANSFORM_CHANNEL) {
        keyframes = path->transformKeys;
        dds3PathEvalSpline(&segmentIndex, &interpolationWeight, keyframes, path->counter.time);
        entries = keyframes->data.worldTransform;
        SDF_VU0_SETVECTOR_X(entries[segmentIndex + 1].rotation[0]);
        SDF_VU0_SETVECTOR_Y(entries[segmentIndex + 1].rotation[1]);
        SDF_VU0_SETVECTOR_Z(entries[segmentIndex + 1].rotation[2]);
        SDF_VU0_SETVECTOR_W(entries[segmentIndex + 1].rotation[3]);
        SDF_VU0_SCALEVECTOR_V0(interpolationWeight);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_SETVECTOR_X(entries[segmentIndex].rotation[0]);
        SDF_VU0_SETVECTOR_Y(entries[segmentIndex].rotation[1]);
        SDF_VU0_SETVECTOR_Z(entries[segmentIndex].rotation[2]);
        SDF_VU0_SETVECTOR_W(entries[segmentIndex].rotation[3]);
        SDF_VU0_SCALEVECTOR_V0(1.0f - interpolationWeight);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_GETVECTOR_X(transformParams->rotation[0]);
        SDF_VU0_GETVECTOR_Y(transformParams->rotation[1]);
        SDF_VU0_GETVECTOR_Z(transformParams->rotation[2]);
        SDF_VU0_GETVECTOR_W(transformParams->rotation[3]);
        SDF_VU0_SETVECTOR_X(entries[segmentIndex + 1].position[0]);
        SDF_VU0_SETVECTOR_Y(entries[segmentIndex + 1].position[1]);
        SDF_VU0_SETVECTOR_Z(entries[segmentIndex + 1].position[2]);
        SDF_VU0_SCALEVECTOR_V0(interpolationWeight);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_SETVECTOR_X(entries[segmentIndex].position[0]);
        SDF_VU0_SETVECTOR_Y(entries[segmentIndex].position[1]);
        SDF_VU0_SETVECTOR_Z(entries[segmentIndex].position[2]);
        SDF_VU0_SCALEVECTOR_V0(1.0f - interpolationWeight);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_GETVECTOR_X(transformParams->position[0]);
        SDF_VU0_GETVECTOR_Y(transformParams->position[1]);
        SDF_VU0_GETVECTOR_Z(transformParams->position[2]);
        SDF_VU0_SETVECTOR_X(entries[segmentIndex + 1].scale[0]);
        SDF_VU0_SETVECTOR_Y(entries[segmentIndex + 1].scale[1]);
        SDF_VU0_SETVECTOR_Z(entries[segmentIndex + 1].scale[2]);
        SDF_VU0_SCALEVECTOR_V0(interpolationWeight);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_SETVECTOR_X(entries[segmentIndex].scale[0]);
        SDF_VU0_SETVECTOR_Y(entries[segmentIndex].scale[1]);
        SDF_VU0_SETVECTOR_Z(entries[segmentIndex].scale[2]);
        SDF_VU0_SCALEVECTOR_V0(1.0f - interpolationWeight);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_GETVECTOR_X(transformParams->scale[0]);
        SDF_VU0_GETVECTOR_Y(transformParams->scale[1]);
        SDF_VU0_GETVECTOR_Z(transformParams->scale[2]);
    } else {
        memset(transformParams, 0, 0x28);
    }
}

/* Linearly interpolated curve sample at `time`; 0 when no curve is active. */
f32 dds3PathBasicControlGetFovy(dds3PathBasicControl_t *user) {
    u32 index;
    f32 fraction;
    Dds3PathKeyframes *curve;
    f32 *values;

    if (user->counter.flags & DDS3_PATH_SCALAR_CHANNEL) {
        curve = user->scalarKeys;
        dds3PathEvalSpline(&index, &fraction, curve, user->counter.time);
        values = curve->data.scalar;
        return values[index] * (1.0f - fraction) + values[index + 1] * fraction;
    }
    return 0.0f;
}

#define SDF_FLOAT_COUNTER_FROZEN_FLAG 0x00000008
#define SDF_FLOAT_COUNTER_WRAP_FLAG 0x00000020

/* Step the counter by one; return zero when a non-looping endpoint is reached. */
s32 dds3PathBasicControlIsFinished(SdfFloatCounter *counter) {
    s32 result = SDF_FLOAT_COUNTER_STEP_CONTINUES;

    if (counter->flags & SDF_FLOAT_COUNTER_FROZEN_FLAG) {
        return SDF_FLOAT_COUNTER_STEP_CONTINUES;
    }
    if (counter->direction == 0) {
        if (counter->duration > counter->time) {
            counter->time = counter->time + 1.0f;
        } else if (counter->flags & SDF_FLOAT_COUNTER_WRAP_FLAG) {
            counter->time = 0.0f;
        } else {
            result = SDF_FLOAT_COUNTER_STEP_FINISHED;
        }
    } else if (counter->direction == 1) {
        if (counter->time > 0.0f) {
            counter->time = counter->time - 1.0f;
        } else if (counter->flags & SDF_FLOAT_COUNTER_WRAP_FLAG) {
            counter->time = counter->duration;
        } else {
            result = SDF_FLOAT_COUNTER_STEP_FINISHED;
        }
    }
    return result;
}

void dds3PathBasicControlAlterMode(SdfFloatCounter *counter, u32 direction) {
    counter->direction = direction;
}

u32 dds3PathBasicControlGetMode(const SdfFloatCounter *counter) {
    return counter->direction;
}

void dds3PathBasicControlSetRatio(SdfFloatCounter *value, float multiplier) {
    value->time = multiplier * value->duration;
}

float dds3PathBasicControlGetRatio(const SdfFloatCounter *value) {
    return value->time / value->duration;
}

void dds3PathBasicPauseOn(SdfFloatCounter *value) {
    value->flags = value->flags | SDF_FLOAT_COUNTER_FROZEN_FLAG;
}

void dds3PathBasicPauseOff(SdfFloatCounter *value) {
    value->flags = value->flags & ~SDF_FLOAT_COUNTER_FROZEN_FLAG;
}

void dds3PathBasicLoopOn(SdfFloatCounter *value) {
    value->flags = value->flags | SDF_FLOAT_COUNTER_WRAP_FLAG;
}

void dds3PathBasicLoopOff(SdfFloatCounter *value) {
    value->flags = value->flags & ~SDF_FLOAT_COUNTER_WRAP_FLAG;
}
