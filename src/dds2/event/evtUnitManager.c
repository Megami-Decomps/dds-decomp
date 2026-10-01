#include "common.h"
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

extern f32 *D_0037F770[];
extern u8 D_0037F780[];

typedef struct EventUnit {
    u8 pad0[0x18];
    EventUnitData *data;
    u8 pad1C[0x50];
    u32 value6C;
    u8 pad70[0x1C];
    EvtUnitOwner *owner;
    u8 pad90[0x18];
    u32 flags;
    s16 valueAC;
    u8 padAE[4];
    s16 valueB2;
    u8 padB4[4];
    f32 valueB8;
    u16 valueBC;
    s16 valueBE;
    s16 valueC0;
    u8 padC2[0xE];
    s8 valueD0;
    s8 valueD1;
} EventUnit;

/* Transition task payload; kept in step with the DDS1 event unit. */
typedef struct EvtTransitionWork {
    u8 pad00[0x68];
    s32 allocationHandle; /* 0x68 */
    u8 pad6C[0x34];
    s32 pathObject;       /* 0xA0 */
    u8 padA4[0x14];
    f32 motionScale;      /* 0xB8 */
    u8 padBC[2];
    s16 motionParamA;     /* 0xBE */
    s16 motionParamB;     /* 0xC0 */
    u8 padC2[0xE];
    s8 firstSlot;         /* 0xD0 */
    s8 secondSlot;        /* 0xD1 */
} EvtTransitionWork;

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
    u32 value;         /* 0x6c: changed by evtSetUnitValueAndFlag */
    f32 vec70[4];      /* 0x70 */
    EvtEffObj *effObj; /* 0x80: effect object the vectors are written to */
    s32 currentTransitionValue; /* 0x84 */
    s32 previousTransitionValue; /* 0x88 */
    u8 pad8C[8];      /* 0x8c */
    s32 unused94;       /* 0x94: cleared by evtPrepareUnitMotionState, never read */
    u8 pad98[8];      /* 0x98 */
    s32 pathId;        /* 0xa0 */
    u8 padA4[4];      /* 0xa4 */
    u32 flags;         /* 0xa8 */
    s16 motionState;    /* 0xac: evtGetUnitMotionState; 0 = idle, 2 = transition */
    u8 padAE[4];      /* 0xae */
    s16 motionTicks;    /* 0xb2: >0 keeps a timed motion out of the idle state */
    s16 directionScale;/* 0xb4: multiplied by 0.1 for the effect direction vector */
    s16 directionOffset;/* 0xb6: multiplied by 0.01 to offset the plan target */
    u8 padB8[4];      /* 0xb8 */
    u16 slotSelect;     /* 0xbc: written by func_0023D1C8 */
    u8 padBE[6];      /* 0xbe */
    s16 transitionArg0; /* 0xc4 */
    s16 transitionArg1; /* 0xc6 */
    s16 transitionArg2; /* 0xc8 */
    u8 padCA[0x16];   /* 0xca */
    u8 slotFlags[12];  /* 0xe0 */
    u8 padEC[0x7C];   /* 0xec */
    s16 slotA[12];     /* 0x168 */
    s16 slotB[12];     /* 0x180 */
    s16 slotC[12];     /* 0x198 */
    s16 unused1B0;      /* 0x1b0: never read or written */
    s16 directionMode;  /* 0x1b2: passed in by evtSetUnitNormalizedDirection */
    u8 pad1B4[8];     /* 0x1b4 */
    s16 transitionElapsed; /* 0x1bc */
    s16 transitionDuration; /* 0x1be */
    f32 speedY;        /* 0x1c0 */
    s16 stepCount;     /* 0x1c4 */
    u8 pad1C6[10];    /* 0x1c6 */
} EvtUnit;

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

void evtUnitSetValueAndFlag(EventUnit *unit, u32 value) {
    unit->value6C = value;
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

u8 evtTestUnitStatusFlags(EventUnit *unit) {
    return (unit->flags & 0x7800) != 0;
}

void evtSetUnitStatusFlags(EventUnit *unit) {
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

s32 evtUnitGetNestedValue(EventUnit *unit) {
    if (unit == NULL) {
        return 0;
    }
    return unit->data->value08;
}

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023CC60);

s32 evtReleaseUnitTransitionWork(u8 *work) {
    s32 handle;

    if (work == NULL) {
        return 1;
    }
    handle = ((EvtTransitionWork *)work)->allocationHandle;
    *(s32 *)(*(u8 **)(*(u8 **)(work + 0x8C) + 0x18) + 0x80) = 0;
    if (handle != 0) {
        sdfReleaseChipBlock(handle);
        ((EvtTransitionWork *)work)->allocationHandle = 0;
    }
    if (((EvtTransitionWork *)work)->pathObject != 0) {
        dds3FreePathObject(((EvtTransitionWork *)work)->pathObject);
        ((EvtTransitionWork *)work)->pathObject = 0;
    }
    sdfReleaseChipBlock(work);
    return 1;
}

s32 evtGetUnitMotionState(EventUnit *unit) {
    return unit->valueAC;
}

void func_0023CE10(EventUnit *unit, u16 value) {
    unit->valueBC = value;
}

void evtSetTransitionMotionScale(EventUnit *unit, f32 value) {
    unit->valueB8 = value;
}

void evtStoreUnitMotionShortParameters(EventUnit *unit, s32 a, s32 b) {
    unit->valueBE = a;
    unit->valueC0 = b;
}

s32 evtIsUnitMotionIdleOrTimedMode(EventUnit *unit) {
    s32 state = evtGetUnitMotionState(unit);

    if (state == 0) {
        return 1;
    }
    if (state == 2 && unit->valueB2 > 0 && unit->owner->motion->mode == 5) {
        return 1;
    }
    return 0;
}

void evtStoreUnitMotionSlotSelection(EventUnit *unit, s32 first, s32 second) {
    unit->valueD0 = first;
    unit->valueD1 = second;
}

void evtActivateStoredUnitMotionSlot(u8 *work) {
    evtConfigureUnitMotionSlot(work, ((EvtTransitionWork *)work)->firstSlot, ((EvtTransitionWork *)work)->secondSlot, 0, 0, 2);
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
    unit->transitionArg0 = a;
    unit->transitionArg1 = b;
    unit->transitionArg2 = c;
    unit->flags |= 0x80;
    unit->motionTicks = 0;
    unit->unused94 = 0;
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
    VU0_LOAD_VF(vf11, unit->effObj->data + 0x40);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, v);
    return func_0023D030(unit, v, angle);
}

s32 evtApplyUnitDirectionOffset(EvtUnit *unit) {
    f32 v[4];
    f32 scale;
    EvtEffObj *obj;

    func_0023D030(unit, unit->vec70, unit->directionOffset * 0.01f);
    if (unit->directionOffset != 0) {
        evtComputePlanarTargetDirectionVu(unit);
        obj = unit->effObj;
    } else {
        VU0_LOAD_VF(vf10, unit->vec70);
        obj = unit->effObj;
        VU0_LOAD_VF(vf11, obj->data + 0x40);
        VU0_SUB(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10();
    }
    scale = unit->directionScale * 0.1f;
    VU0_SCALAR_OP(scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_STORE_VF(vf10, v);
    effObjAddInnerFirstVec(obj, v);
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_004215D0);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_004215E0);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_004215F0);

typedef struct EvtEffVecs {
    u8 pad00[0x40];
    f32 pos[4];
    f32 dir[4];
} EvtEffVecs;

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
        savedA[i] = ((EvtEffVecs *)unit->effObj->data)->pos[i];
        savedB[i] = ((EvtEffVecs *)unit->effObj->data)->dir[i];
    }
    copy = *unit;
    while (func_0023B1E8(&copy) == 0) {
        count++;
        evtApplyUnitDirectionOffset(&copy);
    }
    for (i = 0; i < 4; i++) {
        ((EvtEffVecs *)unit->effObj->data)->pos[i] = savedA[i];
        ((EvtEffVecs *)unit->effObj->data)->dir[i] = savedB[i];
    }
    if (count == 0) {
        func_0035B6E0("ymove frameno = 0\n");
        unit->stepCount = 0;
        unit->speedY = 0;
        return 0;
    }
    VU0_LOAD_VF(vf10, unit->vec70);
    VU0_LOAD_VF(vf11, unit->effObj->data + 0x40);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, delta);
    unit->stepCount = count;
    unit->speedY = delta[1] / (f32)count;
    func_0035B6E0("ymove frameno = %d addvalue = %f total yzahyo=%f\n", count, unit->speedY, delta[1]);
    return count;
}

s32 evtUnitApplyPathVectors(EvtUnit *unit) {
    f32 v[4];

    sdfStepWrappingFloatCounter(unit->pathId);
    func_001171A0(unit->pathId);
    VU0_STORE_VF($vf10, v);
    effObjSetInnerFirstVec(unit->effObj, v);
    if (unit->flags & 0x10) {
        dds3PreparePathVectorPair(unit->pathId);
        VU0_MOVE_VF(vf11, vf10);
        func_00340DC8(0.0f, 3.14159265f, 0.0f);
        effMiscQuatMultiplyVU();
        VU0_STORE_VF($vf10, v);
        effObjSetInnerSecondVec(unit->effObj, v);
    }
    return 1;
}

void evtCopyUnitTargetVector(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x70, src);
}
