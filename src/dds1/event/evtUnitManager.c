#include "common.h"
#include "evt_unit.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

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

extern f32 *D_00324770[];
extern u8 kwlnDefaultColorVector[];


extern void sdfStepWrappingFloatCounter(s32 path);
extern void func_00116F38(s32 path);
extern void dds3PreparePathVectorPair(s32 path);
extern void effObjSetInnerFirstVec(void *obj, void *vec);
extern void effObjSetInnerSecondVec(void *obj, void *vec);
extern void func_002E7F20(f32, f32, f32);
extern void effMiscQuatMultiplyVU();
extern void effMiscQuaternionToMatrixVU(void);
extern void effObjAddInnerFirstVec(void *obj, void *vec);
extern void evtComputePlanarTargetDirectionVu(EvtUnit *unit);
s32 func_00222498(EvtUnit *unit, f32 *dir, f32 scale);
extern f32 evtGetValueScaleFactor(s32 path);
extern void evtScaleValueByMultiplier(s32 path, f32 multiplier);

void func_00221D00(EvtUnit *unit, s32 a, s32 b, s32 c);

/* Length of the path's vec4 trajectory sampled at 20 steps of the value multiplier. */
f32 evtMeasurePathTrajectoryLength(s32 path) {
    f32 saved;
    f32 length = 0.0f;
    f32 step = 0.05f;
    f32 t = step;
    f32 segment;

    saved = evtGetValueScaleFactor(path);
    evtScaleValueByMultiplier(path, 0.0f);
    func_00116F38(path);
    do {
        VU0_MOVE_VF(vf11, vf10);
        evtScaleValueByMultiplier(path, t);
        func_00116F38(path);
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

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00220910);

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
                VU0_LOAD_VF(vf10, D_00324770[0]);
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
                VU0_LOAD_VF(vf10, kwlnDefaultColorVector);
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
                VU0_LOAD_VF(vf10, D_00324770[0] + 4);
    }
}

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00221A80);

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

void evtUnitSetValueAndFlag(EvtUnit *unit, u32 value)
{
    unit->value = value;
    unit->flags = unit->flags | 0x20000;
}

void evtClearUnitValueChangeFlag(EvtUnit *unit) {
    unit->flags = unit->flags & ~0x20000;
    func_00221A80(unit);
}

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00221D00);

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

typedef struct MdlCtx MdlCtx;
extern u32 mdlGetBroadcastValue(MdlCtx *);
extern void mdlBroadcastMasked(MdlCtx *, u32);

void evtSetUnitRgbTransition(EvtUnit *unit, s32 duration, u32 color) {
    u8 *work = (u8 *)unit;

    *(s16 *)(work + 0x156) = duration;
    *(s16 *)(work + 0x154) = 0;
    if (duration == 0) {
        mdlBroadcastMasked((MdlCtx *)unit->owner,
            (mdlGetBroadcastValue((MdlCtx *)unit->owner) & 0xFF000000) | (color & 0xFFFFFF));
        *(u32 *)(work + 0x60) = (*(u32 *)(work + 0x60) & 0xFF000000) | (color & 0xFFFFFF);
        unit->flags &= ~0x8000;
    } else {
        u32 currentRgb = mdlGetBroadcastValue((MdlCtx *)unit->owner) & 0xFFFFFF;

        *(u32 *)(work + 0x60) = (*(u32 *)(work + 0x60) & 0xFF000000) | currentRgb;
        *(u32 *)(work + 0x64) = (*(u32 *)(work + 0x64) & 0xFF000000) | (color & 0xFFFFFF);
        unit->flags |= 0x8000;
    }
}

void func_00221EF0(EvtUnit *unit, s32 duration, u32 color) {
    u8 *work = (u8 *)unit;

    *(s16 *)(work + 0x15A) = duration;
    *(s16 *)(work + 0x158) = 0;
    if (duration == 0) {
        mdlBroadcastMasked((MdlCtx *)unit->owner,
            (mdlGetBroadcastValue((MdlCtx *)unit->owner) & 0xFFFFFF) | (color & 0xFF000000));
        *(u32 *)(work + 0x60) = (*(u32 *)(work + 0x60) & 0xFFFFFF) | (color & 0xFF000000);
        unit->flags &= ~0x10000;
    } else {
        u32 currentAlpha = mdlGetBroadcastValue((MdlCtx *)unit->owner) & 0xFF000000;

        *(u32 *)(work + 0x60) = (*(u32 *)(work + 0x60) & 0xFFFFFF) | currentAlpha;
        *(u32 *)(work + 0x64) = (*(u32 *)(work + 0x64) & 0xFFFFFF) | (color & 0xFF000000);
        unit->flags |= 0x10000;
    }
}

u8 evtTestUnitStatusFlags(EvtUnit *unit)
{
    return (unit->flags & 0x7800) != 0;
}

void evtSetUnitStatusFlags(EvtUnit *unit)
{
    unit->flags = unit->flags | 0x300;
}

void evtConfigureUnitTransition(EvtUnit *unit, s32 arg) {
    if (arg == 0) {
        unit->flags &= ~0x700;
        unit->flags &= ~0x5000;
        unit->flags &= ~0x2800;
    } else {
        func_00221D00(unit, arg, 0, 0);
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

s32 evtUnitGetNestedValue(u8 *obj) {
    if (obj == NULL) {
        return 0;
    }
    return *(s32 *)(*(u8 **)(obj + 0x18) + 8);
}

extern void *sdfAllocSizeClassBlock(s32 size);
extern const s32 D_003AC060[];

EvtUnit *func_002220F0(EvtEffObj *effObj, EvtUnitOwner *owner) {
    EvtUnit *work;
    void *endpoint;
    f32 defaultVector[4];

    memcpy(defaultVector, (const f32 *)D_003AC060, sizeof(defaultVector));
    work = sdfAllocSizeClassBlock(sizeof(EvtUnit));
    memset(work, 0, sizeof(EvtUnit));
    work->motionState = 0;
    work->transitionSourceKind = 1;
    work->unkB8 = 1.0f;
    work->effObj = effObj;
    work->owner = owner;
    work->unkC0 = 10;
    work->unkC8 = 10;
    work->unkBE = 0;
    work->unkC6 = 0;

    VU0_LOAD_VF(vf10, defaultVector);
    VU0_STORE_VF_UNCLOBBERED(vf10, work->vec10);
    VU0_STORE_VF_UNCLOBBERED(vf10, (f32 *)((u8 *)work + 0x40));
    /* The initialized color words at +0x0C are still untyped padding. */
    *(u32 *)((u8 *)work + 0x0C) = 0x00B2B2B2;
    work->color = 0x00B2B2B2;
    *(u32 *)((u8 *)work + 0x5C) = 0x80303030;
    work->color50 = 0x80303030;
    endpoint = sdfAllocSizeClassBlock(0xE0);
    work->endpointWorkAddress = (s32)endpoint;
    memset(endpoint, 0, 0xE0);
    work->value = 0;
    *(u32 *)((u8 *)work + 0xD8) = 0;
    *(u32 *)((u8 *)work + 0xDC) = 0;
    return work;
}

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

void evtUnitSetStoredParameter(EvtUnit *unit, u16 value)
{
    unit->unkBC = value;
}

void evtSetTransitionMotionScale(EvtUnit *work, f32 value) {
    work->unkB8 = value;
}

void evtStoreUnitMotionShortParameters(EvtUnit *work, s32 a, s32 b) {
    work->unkBE = a;
    work->unkC0 = b;
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

void evtStoreUnitMotionSlotSelection(EvtUnit *work, s32 a, s32 b) {
    work->firstSlot = a;
    work->secondSlot = b;
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

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00222498);

/* Flat, negated direction of the rotation in quat, offset by the effect object's point, aimed with func_00222498. */
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
    return func_00222498(unit, v, angle);
}

s32 evtApplyUnitDirectionOffset(EvtUnit *unit) {
    f32 v[4];
    f32 scale;
    EvtEffObj *obj;

    func_00222498(unit, unit->targetVector, unit->directionOffset * 0.01f);
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


extern s32 func_00220678(EvtUnit *unit);
extern void func_003003F0(const char *fmt, ...);

/* Run a copy of the unit until func_00220678 reports done, and derive its per-step Y speed. */
INCLUDE_RODATA(const s32, "event/evtUnitManager", D_003AC060);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_003AC070);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_003AC080);

s32 evtUnitPrepareVerticalMoveSteps(EvtUnit *unit) {
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
    while (func_00220678(&copy) == 0) {
        count++;
        evtApplyUnitDirectionOffset(&copy);
    }
    for (i = 0; i < 4; i++) {
        unit->effObj->data->position[i] = savedA[i];
        unit->effObj->data->orientation[i] = savedB[i];
    }
    if (count == 0) {
        func_003003F0("ymove frameno = 0\n");
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
    func_003003F0("ymove frameno = %d addvalue = %f total yzahyo=%f\n", count, unit->speedY, delta[1]);
    return count;
}

s32 evtUnitApplyPathVectors(EvtUnit *unit) {
    f32 v[4];

    sdfStepWrappingFloatCounter(unit->pathHandle);
    func_00116F38(unit->pathHandle);
    VU0_STORE_VF($vf10, v);
    effObjSetInnerFirstVec(unit->effObj, v);
    if (unit->flags & 0x10) {
        dds3PreparePathVectorPair(unit->pathHandle);
        VU0_MOVE_VF(vf11, vf10);
        func_002E7F20(0.0f, 3.14159265f, 0.0f);
        effMiscQuatMultiplyVU();
        VU0_STORE_VF($vf10, v);
        effObjSetInnerSecondVec(unit->effObj, v);
    }
    return 1;
}

void evtCopyUnitTargetVector(EvtUnit *work, void *src) {
    PCP_COPY_VECTOR(work->targetVector, src);
}
