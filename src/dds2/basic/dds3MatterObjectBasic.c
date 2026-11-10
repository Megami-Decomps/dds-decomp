#include "sdk_ps2.h"
#include "mdl.h"
#include "common.h"
#include "sdf_map_position_api.h"
#include "ps2asm.h"
#include "sdf_vu_matrix.h"
#include "sdf_trig.h"
#include "sdf_model.h"
#include "sdf_chunk.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "dds3_basic_array_cursor.h"
#include "dds3_object_flags.h"
#include "eff_dependency.h"
#include "eff_object.h"
#include "evt_world_source_transform.h"
#include "evt_unit.h"
#include "evt_unit_motion_step_result.h"
#include "eff.h"
#include "eff_node.h"
#include "eff_event.h"
#include "eff_event_sound.h"
#include "btl_sound.h"
#include "bill_object_api.h"
#include "effect_value_object.h"
#include "fld_actor_motion_step_result_api.h"

#include "dds3_world_secondary.h"
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
extern void dds3FreePoseSet(dds3Basic_t *node);
extern void dds3ObjectBaseFreeControlWorkData(dds3ObjectBaseControlWorkData_t *base);
extern void dds3OffPoseSetFlag(dds3PoseSet_t *node, u32 flags);
extern void effObjApplyTransformParameters(sdfGrOt_t **, dds3Basic_t *);
extern sdfGrOt_t *sdfModelOtTable[13][4];

dds3ObjectBaseControlWorkData_t *dds3MatterObjectBasicGetControlWorkData(const dds3Basic_t *object) {
    const EffectTransformData *data = object->workdata;

    return data->baseControlWorkData;
}

/* Spawn a world object of kind 6 and seed its stored vector. */
dds3Basic_t *dds3AddMatterObjectBasic(s32 key, f32 *offset, f32 *rotation) {
    f32 zero[4];
    dds3Basic_t *obj;

    memset(zero, 0, 0x10);
    zero[3] = 1.0f;
    obj = dds3AddSceneBasicData(EFF_WORLD_KIND_EFFECT_TRANSFORM);
    obj->key = (u32)key;
    dds3ObjectBaseCreateMover(obj);
    dds3SetRotPoseSet(obj, (u128 *)rotation);
    dds3SetPosPoseSet(obj, (u128 *)zero);
    dds3SetRadiusPoseSet(obj->poseset);
    ((EffectTransformData *)obj->workdata)->offset[0] = offset[0];
    ((EffectTransformData *)obj->workdata)->offset[1] = offset[1];
    ((EffectTransformData *)obj->workdata)->offset[2] = offset[2];
    ((EffectTransformData *)obj->workdata)->offset[3] = offset[3];
    return obj;
}

void dds3DeleteMatterObjectBasic(dds3Basic_t *node) {
    dds3DeleteSceneBasicData(node);
}

void dds3MatterObjectBasicSetOt(dds3Basic_t *object, s32 activeId) {
    ((EffectTransformData *)object->workdata)->activeId = activeId;
}

void dds3MatterObjectBasicSetHojiID(dds3Basic_t *object, u32 roomNumber) {
    ((EffectTransformData *)object->workdata)->roomNumber = roomNumber;
}

u32 dds3MatterObjectBasicGetHojiID(u32 id) {
    dds3Basic_t *world;
    dds3Basic_t *node;
    EffectTransformData *data;

    world = dds3GetControlScene();
    node = dds3SearchSceneBasic(world, id, 6);
    data = (EffectTransformData *)node->workdata;
    return data->roomNumber;
}

#include "dds3_object_payload.h"

/* Payload word 8 is interpreted by object kind, not always as a pointer. */
void dds3MatterObjectBasicSetAlpMode(dds3Basic_t *object, u32 value) {
    u32 *payload = object->workdata;

    payload[2] = value;
}

u32 dds3MatterObjectBasicGetAlpMode(const dds3Basic_t *object) {
    const u32 *payload = object->workdata;

    return payload[2];
}

void effObjSetOpacityPassEnabled(u32 value) {
    effObjOpacityPassEnabled = value;
}

s32 dds3MatterObjectBasicConstructor(dds3Basic_t *object) {
    EffectTransformData *data;
    void *work;

    dds3InitPoseSet(object);
    work = sdfChipAllocDirect(sizeof(EffectTransformData));
    object->workdata = work;
    memset(work, 0, sizeof(EffectTransformData));
    data = object->workdata;
    data->baseControlWorkData = dds3ObjectBaseInitControlWorkData(object);
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
    dds3ObjectBaseOnFlag(object, 0xE2);
    return 1;
}

void dds3MatterObjectBasicDestructor(dds3Basic_t *object) {
    EffectTransformData *data;

    dds3FreePoseSet(object);
    data = object->workdata;
    dds3ObjectBaseFreeControlWorkData(data->baseControlWorkData);
    sdfChipFreeDirect(data);
}

extern void dds3GetTMPoseSet(dds3Basic_t *object);

s32 effUpdateFadingModelTransform(dds3Basic_t *object) {
    EffectTransformData *data = object->workdata;
    sdfModelRootNode_t *model;
    u32 mode = data->opacityMode;
    s32 alpha;
    f32 coordinates[3];
    dds3PoseSet_t *inner;

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

    model = (sdfModelRootNode_t *)dds3ObjectBaseGetModelHandle(object);
    if (model != NULL) {
        dds3OffPoseSetFlag(object->poseset, OBJECT_TRANSFORM_FLAG_UPDATE_PENDING);
        dds3GetRotPoseSet(object);
        SDF_VU0_STOREVECTOR(model->rotationQuaternion);
        dds3GetTMPoseSet(object);
        dds3GetPosPoseSet(object);
        if (fldStepActorMotionByKey(object->key, &coordinates[0], &coordinates[1], &coordinates[2]) !=
            FLD_ACTOR_MOTION_STEP_NOT_APPLIED) {
            f32 x = coordinates[0];
            f32 y = coordinates[1];
            f32 z = coordinates[2];
            inner = object->poseset;
            inner->pos[0] = data->offset[0] + data->position[0] + x;
            inner->pos[1] = data->offset[1] + data->position[1] + y;
            inner->pos[2] = data->offset[2] + data->position[2] + z;
        } else {
            inner = object->poseset;
            inner->pos[0] = data->offset[0] + data->position[0];
            inner->pos[1] = data->offset[1] + data->position[1];
            inner->pos[2] = data->offset[2] + data->position[2];
        }
        SDF_VU0_SET_ROW3();
        SDF_VU0_STOREMATRIX(model->matrix);
        dds3SetRadiusPoseSet(inner);
        sdfModelEvalTm(model);
    }
    return 1;
}

extern void rsetOtOfs(u32);
extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern struct dds3Basic_t *fldPlayerObject;

/* Submit the normal pass and the opacity-mode pass, temporarily neutralizing
 * the tint while the player is hidden. */
s32 effObjSubmitTransformOpacityPasses(dds3Basic_t *object) {
    EffectTransformData *data;
    u32 opacityMode;

    if (effObjOpacityPassEnabled == 0) {
        return 1;
    }
    data = object->workdata;
    if (data->activeId == -1) {
        rsetOtOfs(0x27);
        rsetTest(1, 5, 0x60, 1, 0, 0, 1, 2);
        effObjApplyTransformParameters(sdfModelOtTable[0], object);
        rsetOtOfs(0x27);
        rsetTest(1, 5, 0x80, 1, 0, 0, 1, 2);
    } else {
        rsetOtOfs(0x22);
        rsetTest(1, 5, 0x60, 1, 0, 0, 1, 2);
        opacityMode = data->opacityMode;
        if (opacityMode < EFFECT_OPACITY_MODE_ALPHA_FADE_END) {
            if (opacityMode >= EFFECT_OPACITY_MODE_ALPHA_FADE_START) {
                if (object->color != EFFECT_OPACITY_COLOR_NEUTRAL) {
                    if (dds3ObjectBaseCheckFlag(fldPlayerObject, 1)) {
                        u32 savedColor = object->color;

                        object->color = EFFECT_OPACITY_COLOR_NEUTRAL;
                        effObjApplyTransformParameters(sdfModelOtTable[data->activeId], object);
                        object->color = savedColor;
                    } else {
                        effObjApplyTransformParameters(sdfModelOtTable[8], object);
                    }
                } else {
                    effObjApplyTransformParameters(sdfModelOtTable[data->activeId], object);
                }
            }
        }
        effObjApplyTransformParameters(sdfModelOtTable[data->activeId], object);
        rsetOtOfs(0x22);
        rsetTest(1, 5, 0x80, 1, 0, 0, 1, 2);
    }
    return 1;
}

/* Refresh the object's stored xyz from the source vector. */
void dds3RefreshStoredVec3(dds3Basic_t *object) {
    dds3PoseSet_t *source = object->poseset;
    EffectTransformData *destination = object->workdata;

    destination->offset[0] = source->pos[0];
    destination->offset[1] = source->pos[1];
    destination->offset[2] = source->pos[2];
}

INCLUDE_SDATA(const s32, "basic/dds3MatterObjectBasic", effObjOpacityPassEnabled);
