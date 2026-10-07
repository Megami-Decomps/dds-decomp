#include "common.h"
#include "dds3_path.h"
#include "evt_world.h"
#include "evt_unit.h"
#include "pcp_vu0.h"

extern void *dds3GetSlot(void *obj, s32 index);
struct ObjectWithResource;
struct EvtScaledValue;
extern Dds3PathCurveWork *dds3GetObjectResourceHandle(struct ObjectWithResource *slot);
extern u32 sdfGetFloatCounterDirection(u32 *handle);
extern f32 evtGetValueScaleFactor(struct EvtScaledValue *handle);

s32 evtCheckWorldObjectResourceScale(void *obj) {
    f32 target = 1.0f;
    void *slot;
    Dds3PathCurveWork *handle;
    s32 kind;

    if (obj == NULL) {
        return -1;
    }
    slot = dds3GetSlot(obj, 1);
    if (slot == NULL) {
        return -1;
    }
    handle = dds3GetObjectResourceHandle(slot);
    if (handle == NULL) {
        return -1;
    }
    kind = sdfGetFloatCounterDirection((u32 *)&handle->direction);
    if (kind != 0) {
        if (kind != 1) {
            return -1;
        }
        target = 0.0f;
    }
    if (evtGetValueScaleFactor((struct EvtScaledValue *)handle) == target) {
        return 0;
    }
    return 1;
}

extern Dds3PathCurveWork *dds3GetSlot1Data(void *obj);
extern void sdfEnableFloatCounterWrap(struct EvtScaledValue *data);
extern void sdfDisableFloatCounterWrap(struct EvtScaledValue *data);

void evtToggleWorldSlotScaledValueFlag(void *obj, s32 flag) {
    Dds3PathCurveWork *data = dds3GetSlot1Data(obj);

    if (data == NULL) {
        return;
    }
    if (flag != 0) {
        sdfEnableFloatCounterWrap((struct EvtScaledValue *)data);
    } else {
        sdfDisableFloatCounterWrap((struct EvtScaledValue *)data);
    }
}

extern u32 dds3AdvanceWorldCounter(void);
extern s32 dds3CreateCameraObject(s32 world, f32 *pos, f32 *rot);
extern void effObjSetInnerFloat(s32 obj, f32 value);

s32 evtCreateWorldObjectAtTransform(f32 *pos, f32 *rot) {
    s32 obj = dds3CreateCameraObject(dds3AdvanceWorldCounter(), pos, rot);

    if (obj == 0) {
        return obj;
    }
    effObjSetInnerFloat(obj, 1.0f);
    return obj;
}

typedef struct EvtNodeInner {
    u8 pad00[0x08];
    s32 kind;           /* 0x08 */
    void *node;         /* 0x0C */
} EvtNodeInner;

typedef struct EvtNodeOwner {
    u8 pad00[0x18];
    EvtNodeInner *inner; /* 0x18 */
} EvtNodeOwner;

extern void effDispatchOptionalNodeFlag();

void evtDispatchSupportedNodeOnClear(EvtNodeOwner *owner, s32 flag) {
    EvtNodeInner *inner = owner->inner;
    void *node;

    switch (inner->kind) {
    case 1:
    case 7:
    case 8:
        break;
    default:
        return;
    }
    node = inner->node;
    if (flag == 0) {
        effDispatchOptionalNodeFlag(node, flag);
    }
}



extern EffWorldNode *dds3GetWorldObject(void);
extern void dds3SetObjectModeAndDefaultWeight(void *node, s32 value);

s32 evtApplyIndexValueToWorldNodes(s32 index, s32 base) {
    EffWorldNode *world = dds3GetWorldObject();
    EffWorldNode *node;
    s32 value;

    if (world == NULL) {
        return 0;
    }
    value = index * 3 + base;
    for (node = ((EvtWorldTable *)world->data)->slots[6].head; node != NULL; node = node->next) {
        dds3SetObjectModeAndDefaultWeight(node, value);
    }
    return 1;
}


extern void effMiscQuaternionToMatrixVU(void);

/* vf10 = (a - b) with y replaced by 0 */
void evtComputeHorizontalDisplacementVu(f32 *a, f32 *b) {
    VU0_LOAD_VF(vf10, a);
    VU0_LOAD_VF(vf11, b);
    VU0_SUB(vf10, vf10, vf11);
    VU0_SCALAR_OP(0.0f, "vaddx.y vf10, vf0, vf2x");
}

/* vf10 = the unit's rotated y axis flattened to the ground plane, negated */
void evtComputePlanarTargetDirectionVu(EvtUnit *unit) {
    f32 v[4];

    VU0_LOAD_VF(vf10, unit->effObj->data->orientation);
    effMiscQuaternionToMatrixVU();
    VU0_STORE_VF(vf30, v);
    v[1] = 0.0f;
    v[3] = 1.0f;
    VU0_LOAD_VF(vf10, v);
    VU0_NORMALIZE_VF10();
    VU0_SCALAR_OP(-1.0f, "vmulx.xyzw vf10, vf10, vf2x");
}

/* vu0 routine: test target range or the selected path endpoint. */
s32 func_00220678(EvtUnit *unit) {
    f32 value;

    switch (unit->motionSubmode) {
    case 0:
    case 1:
    case 2:
        evtComputeHorizontalDisplacementVu(unit->targetVector, unit->effObj->data->position);
        VU0_LENGTH_VF10(value);
        if (value <= 20.0f) {
            return 1;
        }
        break;
    case 3:
        value = evtGetValueScaleFactor((void *)unit->pathHandle);
        if (unit->flags & 4) {
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

extern void evtScaleValueByMultiplier(void *value, f32 multiplier);
extern void dds3InterpolatePathVectorVU(void *value);

s32 evtUnitStepScaledValue(EvtUnit *unit) {
    f32 t;

    if (unit->motionState != EVT_UNIT_MOTION_STATE_SOURCE) {
        return 0;
    }
    switch (unit->transitionSourceKind) {
    case 0:
    case 1:
        break;
    case 2:
        t = evtGetValueScaleFactor((void *)unit->pathHandle);
        if (unit->flags & 4) {
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
        evtScaleValueByMultiplier((void *)unit->pathHandle, t);
        dds3InterpolatePathVectorVU((void *)unit->pathHandle);
        VU0_STORE_VF($vf10, unit->targetVector);
        return 1;
    }
    return 0;
}
