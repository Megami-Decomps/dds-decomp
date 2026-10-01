#include "common.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

typedef struct EvtUnitMotion {
    u8 pad0[0x30];
    u8 mode;           /* 0x30 */
} EvtUnitMotion;

typedef struct EvtUnitOwner {
    u8 pad0[0x1C];
    EvtUnitMotion *motion; /* 0x1c */
} EvtUnitOwner;

typedef struct EvtTargetInfo {
    u8 pad0[0x64];
    u32 flags;         /* 0x64: bit 3 selects the unit's own vector */
} EvtTargetInfo;

typedef struct EvtTarget {
    u8 pad0[0x18];
    EvtTargetInfo *info; /* 0x18 */
} EvtTarget;

extern f32 *D_00324770[];
extern u8 D_00324780[];

typedef struct EvtEffObj {
    u8 pad00[0x1C];
    u8 *data;          /* 0x1c: point/direction records (+0x40, +0x50) */
} EvtEffObj;

/* Event unit: flag bits at 0xa8 drive status queries below. */
typedef struct EvtUnit {
    u32 color;         /* 0x0 */
    u8 pad4[0xC];      /* 0x4 */
    f32 vec10[4];      /* 0x10 */
    f32 vec20[4];      /* 0x20 */
    f32 vec30[4];      /* 0x30 */
    u8 pad40[0x10];   /* 0x40 */
    u32 color50;       /* 0x50 */
    u8 pad54[0x18];   /* 0x54 */
    u32 value;         /* 0x6c: changed by evtUnitSetValueAndFlag */
    f32 vec70[4];      /* 0x70 */
    EvtEffObj *effObj; /* 0x80: effect object the vectors are written to */
    s32 currentTransitionValue; /* 0x84 */
    s32 previousTransitionValue; /* 0x88 */
    EvtUnitOwner *owner; /* 0x8c */
    u8 pad90[4];      /* 0x90 */
    s32 unk94;         /* 0x94 */
    u8 pad98[8];      /* 0x98 */
    s32 pathId;        /* 0xa0 */
    u8 padA4[4];      /* 0xa4 */
    u32 flags;         /* 0xa8 */
    s16 unkAC;         /* 0xac */
    u8 padAE[4];      /* 0xae */
    s16 unkB2;         /* 0xb2 */
    s16 unkB4;         /* 0xb4 */
    s16 unkB6;         /* 0xb6 */
    u8 padB8[4];      /* 0xb8 */
    u16 unkBC;         /* 0xbc */
    u8 padBE[6];      /* 0xbe */
    s16 unkC4;         /* 0xc4 */
    s16 unkC6;         /* 0xc6 */
    s16 unkC8;         /* 0xc8 */
    u8 padCA[0x16];   /* 0xca */
    u8 slotFlags[12];  /* 0xe0 */
    u8 padEC[0x1C];   /* 0xec */
    s16 slotA[12];     /* 0x108 */
    s16 slotB[12];     /* 0x120 */
    s16 slotC[12];     /* 0x138 */
    s16 unk150;        /* 0x150 */
    s16 unk152;        /* 0x152 */
    u8 pad154[8];     /* 0x154 */
    s16 transitionElapsed; /* 0x15c */
    s16 transitionDuration; /* 0x15e */
} EvtUnit;

extern void func_001174C0(s32 path);
extern void func_00116F38(s32 path);
extern void dds3PreparePathVectorPair(s32 path);
extern void effObjSetInnerFirstVec(void *obj, void *vec);
extern void effObjSetInnerSecondVec(void *obj, void *vec);
extern void func_002E7F20(f32, f32, f32);
extern void effMiscQuatMultiplyVU();
extern void effMiscQuaternionToMatrixVU(void);
extern void effObjAddInnerFirstVec(void *obj, void *vec);
extern void func_00220600(EvtUnit *unit);
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
                VU0_LOAD_VF(vf10, D_00324780);
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
    unit->unk152 = arg;
    __asm__ volatile(
        ".set noreorder\n\t"
        "vmulx.w vf10, vf10, vf0x\n\t"
        "vmul.xyz vf2, vf10, vf10\n\t"
        "vmulax.w ACC, vf0, vf2x\n\t"
        "vmadday.w ACC, vf0, vf2y\n\t"
        "vmaddz.w vf2, vf0, vf2z\n\t"
        "vrsqrt Q, vf0w, vf2w\n\t"
        "vwaitq\n\t"
        "vmulq.xyz vf10, vf10, Q\n\t"
        ".set reorder");
        VU0_STORE_VF_UNCLOBBERED(vf10, unit->vec30);
    evtLoadUnitDirectionVectorVU(unit);
        VU0_STORE_VF_UNCLOBBERED(vf10, unit->vec20);
    unit->flags = (unit->flags | 0x2400) & ~0x4000;
}

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00221E08);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00221EF0);

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
    u8 *obj = (u8 *)func_00110A48(dds3GetWorldObject(), id, 5);

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

INCLUDE_ASM(const s32, "event/evtUnitManager", func_002220F0);

s32 evtReleaseUnitTransitionWork(u8 *work) {
    s32 handle;

    if (work == NULL) {
        return 1;
    }
    handle = *(s32 *)(work + 0x68);
    *(s32 *)(*(u8 **)(*(u8 **)(work + 0x8C) + 0x18) + 0x80) = 0;
    if (handle != 0) {
        sdfReleaseChipBlock(handle);
        *(s32 *)(work + 0x68) = 0;
    }
    if (*(s32 *)(work + 0xA0) != 0) {
        dds3FreePathObject(*(s32 *)(work + 0xA0));
        *(s32 *)(work + 0xA0) = 0;
    }
    sdfReleaseChipBlock(work);
    return 1;
}

s32 evtGetUnitMotionState(EvtUnit *unit) {
    return unit->unkAC;
}

void func_00222278(EvtUnit *unit, u16 value)
{
    unit->unkBC = value;
}

void func_00222280(u8 *work, f32 value) {
    *(f32 *)(work + 0xB8) = value;
}

void evtStoreUnitMotionShortParameters(u8 *work, s32 a, s32 b) {
    *(s16 *)(work + 0xBE) = a;
    *(s16 *)(work + 0xC0) = b;
}

s32 evtIsUnitMotionIdleOrTimedMode(EvtUnit *unit) {
    s32 state = evtGetUnitMotionState(unit);

    if (state == 0) {
        return 1;
    }
    if (state == 2 && unit->unkB2 > 0 && unit->owner->motion->mode == 5) {
        return 1;
    }
    return 0;
}

void evtStoreUnitMotionSlotSelection(u8 *work, s32 a, s32 b) {
    *(s8 *)(work + 0xD0) = a;
    *(s8 *)(work + 0xD1) = b;
}

void evtActivateStoredUnitMotionSlot(u8 *work) {
    evtConfigureUnitMotionSlot(work, *(s8 *)(work + 0xD0), *(s8 *)(work + 0xD1), 0, 0, 2);
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
    unit->unkAC = 2;
    unit->unkC4 = a;
    unit->unkC6 = b;
    unit->unkC8 = c;
    unit->flags |= 0x80;
    unit->unkB2 = 0;
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
    VU0_LOAD_VF(vf11, unit->effObj->data + 0x40);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, v);
    return func_00222498(unit, v, angle);
}

s32 evtApplyUnitDirectionOffset(EvtUnit *unit) {
    f32 v[4];
    f32 scale;
    EvtEffObj *obj;

    func_00222498(unit, unit->vec70, unit->unkB6 * 0.01f);
    if (unit->unkB6 != 0) {
        func_00220600(unit);
        obj = unit->effObj;
    } else {
        VU0_LOAD_VF(vf10, unit->vec70);
        obj = unit->effObj;
        VU0_LOAD_VF(vf11, obj->data + 0x40);
        VU0_SUB(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10();
    }
    scale = unit->unkB4 * 0.1f;
    VU0_SCALAR_OP(scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_STORE_VF(vf10, v);
    effObjAddInnerFirstVec(obj, v);
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_003AC060);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_003AC070);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_003AC080);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_002227C8);

s32 evtUnitApplyPathVectors(EvtUnit *unit) {
    f32 v[4];

    func_001174C0(unit->pathId);
    func_00116F38(unit->pathId);
    VU0_STORE_VF($vf10, v);
    effObjSetInnerFirstVec(unit->effObj, v);
    if (unit->flags & 0x10) {
        dds3PreparePathVectorPair(unit->pathId);
        VU0_MOVE_VF(vf11, vf10);
        func_002E7F20(0.0f, 3.14159265f, 0.0f);
        effMiscQuatMultiplyVU();
        VU0_STORE_VF($vf10, v);
        effObjSetInnerSecondVec(unit->effObj, v);
    }
    return 1;
}

void evtCopyUnitTargetVector(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x70, src);
}
