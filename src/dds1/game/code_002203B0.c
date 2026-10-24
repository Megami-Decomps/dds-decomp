#include "common.h"
#include "sdf_vu0.h"
#include "sdf_float_counter.h"
#include "eff_node.h"
#include "eff_dependency.h"
#include "dds3obj.h"
#include "dds3_object_mode.h"
#include "dds3_path.h"
#include "evt_world.h"
#include "evt_unit.h"
#include "pcp_vu0.h"
#include "dds3_world_counter.h"
#include "sdf_vu_matrix.h"


EvtPathStatus evtCheckPathStat(dds3Basic_t *obj) {
    f32 target = 1.0f;
    dds3Basic_t *slot;
    dds3PathBasicControl_t *handle;
    s32 kind;

    if (obj == NULL) {
        return EVT_PATH_STATUS_INVALID;
    }
    slot = dds3ObjectBaseGetControlBasic(obj, 1);
    if (slot == NULL) {
        return EVT_PATH_STATUS_INVALID;
    }
    handle = dds3MoverBasicGetPathControl(slot);
    if (handle == NULL) {
        return EVT_PATH_STATUS_INVALID;
    }
    kind = dds3PathBasicControlGetMode(&handle->counter);
    if (kind != 0) {
        if (kind != 1) {
            return EVT_PATH_STATUS_INVALID;
        }
        target = 0.0f;
    }
    if (dds3PathBasicControlGetRatio(&handle->counter) == target) {
        return EVT_PATH_STATUS_AT_ENDPOINT;
    }
    return EVT_PATH_STATUS_IN_PROGRESS;
}

void evtPathLoop(dds3Basic_t *worldObject, s32 enabled) {
    dds3PathBasicControl_t *data = dds3ObjectBaseGetPathControl(worldObject);

    if (data == NULL) {
        return;
    }
    if (enabled != 0) {
        dds3PathBasicLoopOn(&data->counter);
    } else {
        dds3PathBasicLoopOff(&data->counter);
    }
}

extern dds3Basic_t *dds3AddCameraObjectBasicTypeFree(s32 world, f32 *pos, f32 *rot);
extern void dds3GetRadiusPoseSet(dds3Basic_t *obj, f32 value);

dds3Basic_t *evtAddCamera(f32 *pos, f32 *rot) {
    dds3Basic_t *obj = dds3AddCameraObjectBasicTypeFree(dds3AllocObjectId(), pos, rot);

    if (obj == 0) {
        return obj;
    }
    dds3GetRadiusPoseSet(obj, 1.0f);
    return obj;
}

void evtEffectLoopControl(dds3Basic_t *owner, s32 nodeFlag) {
    EffectDependencyState *state = owner->data;
    s32 stateKind = state->state;
    EffNode *node;

    switch (stateKind) {
    case EFF_DEPENDENCY_STATE_BOUND_EFFECT_NODE:
    case EFF_DEPENDENCY_STATE_MAGATUHI_TWO_ROWS:
    case EFF_DEPENDENCY_STATE_MAGATUHI_FOUR_ROWS:
        break;
    default:
        return;
    }
    node = (EffNode *)state->handle;
    if (nodeFlag == 0) {
        effDispatchOptionalNodeFlag(node, nodeFlag);
    }
}



#include "dds3_world_primary.h"

s32 evtStageAnimeControl(s32 modeSelector, s32 modeValue) {
    dds3Basic_t *world = dds3GetActiveScene();
    dds3Basic_t *node;
    s32 combinedMode;

    if (world == NULL) {
        return 0;
    }
    combinedMode = modeSelector * 3 + modeValue;
    for (node = ((EvtWorldTable *)world->data)->slots[6].head; node != NULL; node = node->next) {
        dds3ObjectBaseMatterAnimeSpeed(node, combinedMode);
    }
    return 1;
}



/* vf10 = (a - b) with y replaced by 0 */
void evtVu0SubVectorXZAxis(f32 *a, f32 *b) {
    SDF_VU0_LOADVECTOR(a);
    SDF_VU0_LOADVECTOR_B(b);
    SDF_VU0_SUBVECTOR();
    VU0_SCALAR_OP(0.0f, "vaddx.y vf10, vf0, vf2x");
}

/* vf10 = the unit's rotated y axis flattened to the ground plane, negated */
void evtGetUnitDirVector(evtUnitHandle_t *unit) {
    f32 v[4];

    SDF_VU0_LOADVECTOR(unit->effObj->inner->rotation);
    effMiscQuaternionToMatrixVU();
    SDF_VU0_STORE_ROW2(v);
    v[1] = 0.0f;
    v[3] = 1.0f;
    SDF_VU0_LOADVECTOR(v);
    SDF_VU0_NORMALIZE();
    VU0_SCALAR_OP(-1.0f, "vmulx.xyzw vf10, vf10, vf2x");
}

/* vu0 routine: test target range or the selected path endpoint. */
s32 evtCheckMoveEnd(evtUnitHandle_t *unit) {
    f32 value;

    switch (unit->motionSubmode) {
    case 0:
    case 1:
    case 2:
        evtVu0SubVectorXZAxis(unit->targetVector, unit->effObj->inner->position);
        SDF_VU0_GETLENGTH(value);
        if (value <= 20.0f) {
            return 1;
        }
        break;
    case 3:
        value = dds3PathBasicControlGetRatio(&unit->pathControl->counter);
        if (unit->flags & EVT_UNIT_FLAG_PATH_REVERSE) {
            if (value == 0.0f) {
                return 1;
            }
        } else if (value == 1.0f) {
            return 1;
        }
        break;
    }
    return 0;
}


s32 evtGetNextMovePos(evtUnitHandle_t *unit) {
    f32 t;

    if (unit->motionState != EVT_UNIT_MOTION_STATE_SOURCE) {
        return 0;
    }
    switch (unit->transitionSourceKind) {
    case EVT_UNIT_TRANSITION_SOURCE_COPIED_VECTOR:
    case EVT_UNIT_TRANSITION_SOURCE_WORLD_NODE:
        break;
    case EVT_UNIT_TRANSITION_SOURCE_PATH_CURVE:
        t = dds3PathBasicControlGetRatio(&unit->pathControl->counter);
        if (unit->flags & EVT_UNIT_FLAG_PATH_REVERSE) {
            if (t == 0.0f) {
                return 0;
            }
        } else {
            if (t == 1.0f) {
                return 0;
            }
        }
        t += unit->pathSpeed;
        if (t < 0.0f) {
            t = 0.0f;
        }
        if (1.0f < t) {
            t = 1.0f;
        }
        dds3PathBasicControlSetRatio(&unit->pathControl->counter, t);
        dds3PathBasicControlGetPos(unit->pathControl);
        VU0_STORE_VF($vf10, unit->targetVector);
        return 1;
    }
    return 0;
}
