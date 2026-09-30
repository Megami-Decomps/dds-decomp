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
    u8 pad70[0x14];   /* 0x70 */
    s32 currentTransitionValue; /* 0x84 */
    s32 previousTransitionValue; /* 0x88 */
    EvtUnitOwner *owner; /* 0x8c */
    u8 pad90[4];      /* 0x90 */
    s32 unk94;         /* 0x94 */
    u8 pad98[0x10];   /* 0x98 */
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

void func_00221D00(EvtUnit *unit, s32 a, s32 b, s32 c);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00220830);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00220910);

/* vu0 routine: load vf10 with the unit's colour (flag 0x100), its target's vector, or the default */
void func_00221890(EvtUnit *unit) {
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
        __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(D_00324770[0]));
    }
}

/* vu0 routine: as above for the second colour (flag 0x200) and the vectors at +0x40 */
void func_00221940(EvtUnit *unit) {
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
        __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(D_00324780));
    }
}

/* vu0 routine: load vf10 with the unit's vector, or the default one */
void func_002219F8(EvtUnit *unit) {
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
        __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(D_00324770[0] + 4));
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

/* vu0 routine: normalize the direction in vf10, store it to vec30, then vf10 from func_002219F8 to vec20 */
void func_00221D98(EvtUnit *unit, s32 arg) {
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
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(unit->vec30));
    func_002219F8(unit);
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(unit->vec20));
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

void func_00221FF8(EvtUnit *unit, s32 arg) {
    if (arg == 0) {
        unit->flags &= ~0x700;
        unit->flags &= ~0x5000;
        unit->flags &= ~0x2800;
    } else {
        func_00221D00(unit, arg, 0, 0);
        unit->flags = (unit->flags & ~0x800) | 0x1000;
        __asm__ volatile(".set noreorder\n\tvmove.xyzw vf10, vf0\n\t.set reorder");
        func_00221D98(unit, arg);
        unit->flags = (unit->flags & ~0x2000) | 0x4000;
    }
}

s32 func_00222090(s32 id) {
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

s32 func_00222200(u8 *work) {
    s32 handle;

    if (work == NULL) {
        return 1;
    }
    handle = *(s32 *)(work + 0x68);
    *(s32 *)(*(u8 **)(*(u8 **)(work + 0x8C) + 0x18) + 0x80) = 0;
    if (handle != 0) {
        func_002CFF98(handle);
        *(s32 *)(work + 0x68) = 0;
    }
    if (*(s32 *)(work + 0xA0) != 0) {
        dds3FreePathObject(*(s32 *)(work + 0xA0));
        *(s32 *)(work + 0xA0) = 0;
    }
    func_002CFF98(work);
    return 1;
}

s32 func_00222270(EvtUnit *unit) {
    return unit->unkAC;
}

void func_00222278(EvtUnit *unit, u16 value)
{
    unit->unkBC = value;
}

void func_00222280(u8 *work, f32 value) {
    *(f32 *)(work + 0xB8) = value;
}

void func_00222288(u8 *work, s32 a, s32 b) {
    *(s16 *)(work + 0xBE) = a;
    *(s16 *)(work + 0xC0) = b;
}

s32 func_00222298(EvtUnit *unit) {
    s32 state = func_00222270(unit);

    if (state == 0) {
        return 1;
    }
    if (state == 2 && unit->unkB2 > 0 && unit->owner->motion->mode == 5) {
        return 1;
    }
    return 0;
}

void func_00222300(u8 *work, s32 a, s32 b) {
    *(s8 *)(work + 0xD0) = a;
    *(s8 *)(work + 0xD1) = b;
}

void func_00222310(u8 *work) {
    func_00222340(work, *(s8 *)(work + 0xD0), *(s8 *)(work + 0xD1), 0, 0, 2);
}

void func_00222340(EvtUnit *unit, s32 slot, s32 a, s32 b, s32 c, s32 mode) {
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

void func_002223D8(EvtUnit *unit, s32 a, s32 b, s32 c, s32 mode) {
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

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00222658);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00222700);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_003AC060);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_003AC070);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_003AC080);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_002227C8);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_00222A20);

void func_00222AA8(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x70, src);
}
