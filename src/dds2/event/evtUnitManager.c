#include "common.h"
#include "evt_unit.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

typedef struct EventUnitData {
    u8 pad00[8];
    s32 value08;
} EventUnitData;

typedef struct EvtUnitMotion {
    u8 pad0[0x30];
    u8 mode;           /* 0x30 */
} EvtUnitMotion;


typedef struct EvtTargetInfo {
    u8 pad0[0x64];
    u32 flags;         /* 0x64: bit 3 selects the unit's own vector */
} EvtTargetInfo;

typedef struct EvtTarget {
    u8 pad0[0x18];
    EvtTargetInfo *info; /* 0x18 */
} EvtTarget;

extern f32 *D_0037F770[];
extern u8 D_0037F780[];

/* World-list node, not EvtUnit work: the nested address is in its data record.
 * In particular, this +0x18 pointer is not the work's float vector at +0x10. */
typedef struct EvtUnitNode {
    u8 pad00[0x18];
    EventUnitData *data;
} EvtUnitNode;

extern void sdfStepWrappingFloatCounter(s32 path);
extern void func_001171A0(s32 path);
extern void dds3PreparePathVectorPair(s32 path);
extern void effObjSetInnerFirstVec(void *obj, void *vec);
extern void effObjSetInnerSecondVec(void *obj, void *vec);
extern void func_00340DC8(f32, f32, f32);
extern void effMiscQuatMultiplyVU();
extern void effMiscQuaternionToMatrixVU(void);
extern void effObjAddInnerFirstVec(void *obj, void *vec);
extern void evtComputePlanarTargetDirectionVu(EvtUnit *unit);
s32 func_0023D030(EvtUnit *unit, f32 *dir, f32 angle);
extern f32 evtGetValueScaleFactor(s32 path);
extern void evtScaleValueByMultiplier(s32 path, f32 multiplier);

void func_0023C870(EvtUnit *unit, s32 a, s32 b, s32 c);

typedef struct PcpScatterWork4 PcpScatterWork4;

/* func_001730D0 */
struct PcpScatterWork4 {
    u8 pad00[0x40];
    s128 unk40;
    u8 pad50[0x12C];
    f32 unk17C;
    u32 unk180;
    u32 scatterObject;
    u32 ownedBuffer;
};

/* Length of the path's vec4 trajectory sampled at 20 steps of the value multiplier. */
f32 evtMeasurePathTrajectoryLength(s32 path) {
    f32 saved;
    f32 length = 0.0f;
    f32 step = 0.05f;
    f32 t = step;
    f32 segment;

    saved = evtGetValueScaleFactor(path);
    evtScaleValueByMultiplier(path, 0.0f);
    func_001171A0(path);
    do {
        VU0_MOVE_VF(vf11, vf10);
        evtScaleValueByMultiplier(path, t);
        func_001171A0(path);
        VU0_MOVE_VF(vf12, vf10);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(segment);
        length += segment;
        VU0_MOVE_VF(vf10, vf12);
        t += step;
    } while (t <= 1.0f);
    evtScaleValueByMultiplier(path, saved);
    return length;
}

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023B480);

/* vu0 routine: load vf10 with the unit's colour (flag 0x100), its target's vector, or the default */
void evtLoadUnitFirstColorVectorVU(EvtUnit *unit) {
    EvtTargetInfo *info = 0;
    s32 ownVector = 0;
    s32 color;
    f32 scale;

    if (unit->currentTransitionValue != 0 && (unit->flags & 0x40000)) {
        info = ((EvtTarget *)unit->currentTransitionValue)->info;
        if (info->flags & 0x8) {
            ownVector = 1;
        }
    }
    if (unit->flags & 0x100) {
        scale = 0.0078125f;
        color = unit->color;
        EE_MMI_RGBA_UNPACK(&color, scale);
    } else if (ownVector) {
        VU0_LOAD_VF(vf10, info);
    } else {
        VU0_LOAD_VF(vf10, D_0037F770[0]);
    }
}

/* vu0 routine: as above for the second colour (flag 0x200) and the vectors at +0x40 */
void evtLoadUnitSecondColorVectorVU(EvtUnit *unit) {
    EvtTargetInfo *info = 0;
    s32 ownVector = 0;
    s32 color;
    f32 scale;

    if (unit->currentTransitionValue != 0 && (unit->flags & 0x40000)) {
        info = ((EvtTarget *)unit->currentTransitionValue)->info;
        if (info->flags & 0x8) {
            ownVector = 1;
        }
    }
    if (unit->flags & 0x200) {
        scale = 0.0078125f;
        color = unit->color50;
        EE_MMI_RGBA_UNPACK(&color, scale);
    } else if (ownVector) {
        VU0_LOAD_VF(vf10, (u8 *)info + 0x40);
    } else {
        VU0_LOAD_VF(vf10, D_0037F780);
    }
}

/* vu0 routine: load vf10 with the unit's vector, or the default one */
void evtLoadUnitDirectionVectorVU(EvtUnit *unit) {
    s32 ownVector = 0;

    if (unit->currentTransitionValue != 0 && (unit->flags & 0x40000)) {
        if (((EvtTarget *)unit->currentTransitionValue)->info->flags & 0x8) {
            ownVector = 1;
        }
    }
    if (unit->flags & 0x400) {
        VU0_LOAD_VF(vf10, unit->vec10);
    } else if (ownVector) {
        VU0_LOAD_VF(vf10, unit->vec10);
    } else {
        VU0_LOAD_VF(vf10, D_0037F770[0] + 4);
    }
}

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023C5F0);

void evtSetUnitValueTransition(EvtUnit *unit, s32 value, s32 duration) {
    unit->flags |= 0x40000;
    unit->previousTransitionValue = unit->currentTransitionValue;
    unit->currentTransitionValue = value;
    if (duration == 0) {
        unit->previousTransitionValue = 0;
        unit->transitionElapsed = 0;
        unit->transitionDuration = 0;
        unit->flags &= ~0x180000;
    } else {
        unit->flags |= 0x80000;
        unit->flags &= ~0x100000;
        unit->transitionDuration = duration;
        unit->transitionElapsed = 0;
    }
}

void evtEndUnitValueTransition(EvtUnit *unit, s32 duration) {
    if (unit->currentTransitionValue != 0) {
        if (duration == 0) {
            unit->flags &= ~0x40000;
            unit->flags &= ~0x180000;
            unit->currentTransitionValue = 0;
        } else {
            unit->flags &= ~0x80000;
            unit->transitionDuration = duration;
            unit->flags |= 0x100000;
            unit->transitionElapsed = 0;
        }
    }
}

void evtUnitSetValueAndFlag(EvtUnit *unit, u32 value) {
    unit->value = value;
    unit->flags = unit->flags | 0x20000;
}

void evtClearUnitValueChangeFlag(EvtUnit *unit) {
    unit->flags = unit->flags & ~0x20000;
    func_0023C5F0(unit);
}

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023C870);

/* vu0 routine: normalize the direction in vf10, store it to vec30, then vf10 from evtLoadUnitDirectionVectorVU to vec20 */
void evtSetUnitNormalizedDirection(EvtUnit *unit, s32 arg) {
    unit->directionMode = arg;
    VU0_CLEAR_W(vf10);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF_UNCLOBBERED(vf10, unit->vec30);
    evtLoadUnitDirectionVectorVU(unit);
    VU0_STORE_VF_UNCLOBBERED(vf10, unit->vec20);
    unit->flags = (unit->flags | 0x2400) & ~0x4000;
}

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023C978);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023CA60);

u8 evtTestUnitStatusFlags(EvtUnit *unit) {
    return (unit->flags & 0x7800) != 0;
}

void evtSetUnitStatusFlags(EvtUnit *unit) {
    unit->flags = unit->flags | 0x300;
}

void evtConfigureUnitTransition(EvtUnit *unit, s32 arg) {
    if (arg == 0) {
        unit->flags &= ~0x700;
        unit->flags &= ~0x5000;
        unit->flags &= ~0x2800;
    } else {
        func_0023C870(unit, arg, 0, 0);
        unit->flags = (unit->flags & ~0x800) | 0x1000;
        VU0_MOVE_VF(vf10, vf0);
        evtSetUnitNormalizedDirection(unit, arg);
        unit->flags = (unit->flags & ~0x2000) | 0x4000;
    }
}

s32 evtGetWorldUnitNestedValue(s32 id) {
    u8 *obj = (u8 *)dds3FindWorldObjectNodeByKey(dds3GetWorldObject(), id, 5);

    if (obj != NULL) {
        return *(s32 *)(*(u8 **)(obj + 0x18) + 8);
    }
    return (s32)obj;
}

s32 evtUnitGetNestedValue(EvtUnitNode *unit) {
    if (unit == NULL) {
        return 0;
    }
    return unit->data->value08;
}

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023CC60);

s32 evtReleaseUnitTransitionWork(EvtUnit *work) {
    s32 handle;

    if (work == NULL) {
        return 1;
    }
    handle = work->endpointWorkAddress;
    *(s32 *)((u8 *)work->owner->data + 0x80) = 0;
    if (handle != 0) {
        sdfReleaseChipBlock(handle);
        work->endpointWorkAddress = 0;
    }
    if (work->pathHandle != 0) {
        dds3FreePathObject(work->pathHandle);
        work->pathHandle = 0;
    }
    sdfReleaseChipBlock(work);
    return 1;
}

s32 evtGetUnitMotionState(EvtUnit *unit) {
    return unit->motionState;
}

void func_0023CE10(EvtUnit *unit, u16 value) {
    unit->unkBC = value;
}

void evtSetTransitionMotionScale(EvtUnit *unit, f32 value) {
    unit->unkB8 = value;
}

void evtStoreUnitMotionShortParameters(EvtUnit *unit, s32 a, s32 b) {
    unit->unkBE = a;
    unit->unkC0 = b;
}

s32 evtIsUnitMotionIdleOrTimedMode(EvtUnit *unit) {
    s32 state = evtGetUnitMotionState(unit);

    if (state == 0) {
        return 1;
    }
    if (state == 2 && unit->motionTicks > 0 && unit->owner->motion->mode == 5) {
        return 1;
    }
    return 0;
}

void evtStoreUnitMotionSlotSelection(EvtUnit *unit, s32 first, s32 second) {
    unit->firstSlot = first;
    unit->secondSlot = second;
}

void evtActivateStoredUnitMotionSlot(EvtUnit *work) {
    evtConfigureUnitMotionSlot(work, work->firstSlot, work->secondSlot, 0, 0, 2);
}

void evtConfigureUnitMotionSlot(EvtUnit *unit, s32 slot, s32 a, s32 b, s32 c, s32 mode) {
    unit->slotFlags[slot] = 3;
    unit->slotA[slot] = a;
    unit->slotB[slot] = b;
    unit->slotC[slot] = c;
    switch (mode) {
    case 0:
        unit->slotFlags[slot] |= 0x4;
        break;
    case 1:
        unit->slotFlags[slot] |= 0x8;
        break;
    case 2:
        unit->slotFlags[slot] |= 0x10;
        break;
    }
}

void evtPrepareUnitMotionState(EvtUnit *unit, s32 a, s32 b, s32 c, s32 mode) {
    unit->flags &= ~0x1;
    unit->flags &= ~0x20;
    unit->flags &= ~0x40;
    unit->flags &= ~0x400000;
    unit->flags &= ~0x800000;
    unit->motionState = 2;
    unit->unkC4 = a;
    unit->unkC6 = b;
    unit->unkC8 = c;
    unit->flags |= 0x80;
    unit->motionTicks = 0;
    unit->unk94 = 0;
    switch (mode) {
    case 0:
        unit->flags |= 0x20;
        break;
    case 1:
        unit->flags |= 0x1;
        break;
    case 2:
        break;
    case 3:
        unit->flags |= 0x40;
        break;
    }
}

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023D030);

/* Flat, negated direction of the rotation in quat, offset by the effect object's point, aimed with func_0023D030. */
s32 evtAimUnitFromFlatQuaternion(EvtUnit *unit, f32 *quat, f32 angle) {
    f32 v[4];

    VU0_LOAD_VF(vf10, quat);
    effMiscQuaternionToMatrixVU();
    VU0_MOVE_VF(vf10, vf30);
    VU0_SET_AXIS_CLEAR_W(0.0f, y);
    VU0_NORMALIZE_VF10();
    VU0_SCALAR_OP(-1.0f, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_LOAD_VF(vf11, unit->effObj->data->position);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, v);
    return func_0023D030(unit, v, angle);
}

s32 evtApplyUnitDirectionOffset(EvtUnit *unit) {
    f32 v[4];
    f32 scale;
    EvtEffObj *obj;

    func_0023D030(unit, unit->targetVector, unit->directionOffset * 0.01f);
    if (unit->directionOffset != 0) {
        evtComputePlanarTargetDirectionVu(unit);
        obj = unit->effObj;
    } else {
        VU0_LOAD_VF(vf10, unit->targetVector);
        obj = unit->effObj;
        VU0_LOAD_VF(vf11, obj->data->position);
        VU0_SUB(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10();
    }
    scale = unit->motionParameter * 0.1f;
    VU0_SCALAR_OP(scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_STORE_VF(vf10, v);
    effObjAddInnerFirstVec(obj, v);
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_004215D0);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_004215E0);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_004215F0);


extern s32 func_0023B1E8(EvtUnit *unit);
extern void func_0035B6E0(const char *fmt, ...);

/* Run a copy of the unit until func_0023B1E8 reports done, and derive its per-step Y speed. */
s32 func_0023D360(EvtUnit *unit) {
    EvtUnit copy;
    f32 delta[4];
    f32 savedA[4];
    f32 savedB[4];
    s32 count = 0;
    s32 i;
    for (i = 0; i < 4; i++) {
        savedA[i] = unit->effObj->data->position[i];
        savedB[i] = unit->effObj->data->orientation[i];
    }
    copy = *unit;
    while (func_0023B1E8(&copy) == 0) {
        count++;
        evtApplyUnitDirectionOffset(&copy);
    }
    for (i = 0; i < 4; i++) {
        unit->effObj->data->position[i] = savedA[i];
        unit->effObj->data->orientation[i] = savedB[i];
    }
    if (count == 0) {
        func_0035B6E0("ymove frameno = 0\n");
        unit->stepCount = 0;
        unit->speedY = 0;
        return 0;
    }
    VU0_LOAD_VF(vf10, unit->targetVector);
    VU0_LOAD_VF(vf11, unit->effObj->data->position);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, delta);
    unit->stepCount = count;
    unit->speedY = delta[1] / (f32)count;
    func_0035B6E0("ymove frameno = %d addvalue = %f total yzahyo=%f\n", count, unit->speedY, delta[1]);
    return count;
}

s32 evtUnitApplyPathVectors(EvtUnit *unit) {
    f32 v[4];

    sdfStepWrappingFloatCounter(unit->pathHandle);
    func_001171A0(unit->pathHandle);
    VU0_STORE_VF($vf10, v);
    effObjSetInnerFirstVec(unit->effObj, v);
    if (unit->flags & 0x10) {
        dds3PreparePathVectorPair(unit->pathHandle);
        VU0_MOVE_VF(vf11, vf10);
        func_00340DC8(0.0f, 3.14159265f, 0.0f);
        effMiscQuatMultiplyVU();
        VU0_STORE_VF($vf10, v);
        effObjSetInnerSecondVec(unit->effObj, v);
    }
    return 1;
}

void evtCopyUnitTargetVector(EvtUnit *work, void *src) {
    PCP_COPY_VECTOR(work->targetVector, src);
}
