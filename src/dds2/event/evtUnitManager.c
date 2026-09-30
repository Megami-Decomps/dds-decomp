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
    u8 pad70[0x10];   /* 0x70 */
    void *effObj;      /* 0x80: effect object the vectors are written to */
    s32 currentTransitionValue; /* 0x84 */
    s32 previousTransitionValue; /* 0x88 */
    u8 pad8C[8];      /* 0x8c */
    s32 unk94;         /* 0x94 */
    u8 pad98[8];      /* 0x98 */
    s32 pathId;        /* 0xa0 */
    u8 padA4[4];      /* 0xa4 */
    u32 flags;         /* 0xa8 */
    s16 unkAC;         /* 0xac */
    u8 padAE[4];      /* 0xae */
    s16 unkB2;         /* 0xb2 */
    u8 padB4[8];      /* 0xb4 */
    u16 unkBC;         /* 0xbc */
    u8 padBE[6];      /* 0xbe */
    s16 unkC4;         /* 0xc4 */
    s16 unkC6;         /* 0xc6 */
    s16 unkC8;         /* 0xc8 */
    u8 padCA[0x16];   /* 0xca */
    u8 slotFlags[12];  /* 0xe0 */
    u8 padEC[0x7C];   /* 0xec */
    s16 slotA[12];     /* 0x168 */
    s16 slotB[12];     /* 0x180 */
    s16 slotC[12];     /* 0x198 */
    s16 unk1B0;        /* 0x1b0 */
    s16 unk1B2;        /* 0x1b2 */
    u8 pad1B4[8];     /* 0x1b4 */
    s16 transitionElapsed; /* 0x1bc */
    s16 transitionDuration; /* 0x1be */
} EvtUnit;

extern void func_00117728(s32 path);
extern void func_001171A0(s32 path);
extern void dds3PreparePathVectorPair(s32 path);
extern void effObjSetInnerFirstVec(void *obj, void *vec);
extern void effObjSetInnerSecondVec(void *obj, void *vec);
extern void func_00340DC8(f32, f32, f32);
extern void effMiscQuatMultiplyVU();

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

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023B3A0);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023B480);

/* vu0 routine: load vf10 with the unit's colour (flag 0x100), its target's vector, or the default */
void func_0023C400(EvtUnit *unit) {
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
        __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(info));
    } else {
        __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(D_0037F770[0]));
    }
}

/* vu0 routine: as above for the second colour (flag 0x200) and the vectors at +0x40 */
void func_0023C4B0(EvtUnit *unit) {
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
        __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"((u8 *)info + 0x40));
    } else {
        __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(D_0037F780));
    }
}

/* vu0 routine: load vf10 with the unit's vector, or the default one */
void func_0023C568(EvtUnit *unit) {
    s32 ownVector = 0;

    if (unit->currentTransitionValue != 0 && (unit->flags & 0x40000)) {
        if (((EvtTarget *)unit->currentTransitionValue)->info->flags & 0x8) {
            ownVector = 1;
        }
    }
    if (unit->flags & 0x400) {
        __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(unit->vec10));
    } else if (ownVector) {
        __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(unit->vec10));
    } else {
        __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(D_0037F770[0] + 4));
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

/* vu0 routine: normalize the direction in vf10, store it to vec30, then vf10 from func_0023C568 to vec20 */
void func_0023C908(EvtUnit *unit, s32 arg) {
    unit->unk1B2 = arg;
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
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(unit->vec30));
    func_0023C568(unit);
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(unit->vec20));
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

void func_0023CB68(EvtUnit *unit, s32 arg) {
    if (arg == 0) {
        unit->flags &= ~0x700;
        unit->flags &= ~0x5000;
        unit->flags &= ~0x2800;
    } else {
        func_0023C870(unit, arg, 0, 0);
        unit->flags = (unit->flags & ~0x800) | 0x1000;
        __asm__ volatile(".set noreorder\n\tvmove.xyzw vf10, vf0\n\t.set reorder");
        func_0023C908(unit, arg);
        unit->flags = (unit->flags & ~0x2000) | 0x4000;
    }
}

s32 func_0023CC00(s32 id) {
    u8 *obj = (u8 *)func_00110C70(dds3GetWorldObject(), id, 5);

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

s32 func_0023CD98(u8 *work) {
    s32 handle;

    if (work == NULL) {
        return 1;
    }
    handle = *(s32 *)(work + 0x68);
    *(s32 *)(*(u8 **)(*(u8 **)(work + 0x8C) + 0x18) + 0x80) = 0;
    if (handle != 0) {
        func_00328E48(handle);
        *(s32 *)(work + 0x68) = 0;
    }
    if (*(s32 *)(work + 0xA0) != 0) {
        dds3FreePathObject(*(s32 *)(work + 0xA0));
        *(s32 *)(work + 0xA0) = 0;
    }
    func_00328E48(work);
    return 1;
}

s32 func_0023CE08(EventUnit *unit) {
    return unit->valueAC;
}

void func_0023CE10(EventUnit *unit, u16 value) {
    unit->valueBC = value;
}

void func_0023CE18(EventUnit *unit, f32 value) {
    unit->valueB8 = value;
}

void func_0023CE20(EventUnit *unit, s32 a, s32 b) {
    unit->valueBE = a;
    unit->valueC0 = b;
}

s32 func_0023CE30(EventUnit *unit) {
    s32 state = func_0023CE08(unit);

    if (state == 0) {
        return 1;
    }
    if (state == 2 && unit->valueB2 > 0 && unit->owner->motion->mode == 5) {
        return 1;
    }
    return 0;
}

void func_0023CE98(EventUnit *unit, s32 first, s32 second) {
    unit->valueD0 = first;
    unit->valueD1 = second;
}

void func_0023CEA8(u8 *work) {
    func_0023CED8(work, *(s8 *)(work + 0xD0), *(s8 *)(work + 0xD1), 0, 0, 2);
}

void func_0023CED8(EvtUnit *unit, s32 slot, s32 a, s32 b, s32 c, s32 mode) {
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

void func_0023CF70(EvtUnit *unit, s32 a, s32 b, s32 c, s32 mode) {
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

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023D030);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023D1F0);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023D298);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_004215D0);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_004215E0);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_004215F0);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023D360);

s32 func_0023D5B8(EvtUnit *unit) {
    f32 v[4];

    func_00117728(unit->pathId);
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

INCLUDE_ASM(const s32, "event/evtUnitManager", func_0023D640);
