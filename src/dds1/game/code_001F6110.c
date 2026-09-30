#include "common.h"
#include "ee_mmi.h"

extern void func_001D54C0(s32 actor);

extern u32 func_001DAE48(s32 actor);
extern s32 func_001DAE50(s32 actor, u32 index);
extern void btlFlagUnitDefeatCandidate(s32 actor);

extern s32 D_00360348[];
extern s32 D_0035FFE0[];
extern s32 D_003BB6B8;
extern s32 D_003BD854;
extern s32 func_002EB028(s32, u32 *, s32);
extern void btlCmdSimpleB(s32, u16);
extern void btlCmdSimpleA(s32, u16);
extern void btlCmdSimpleE(s32, u16);
extern void btlCmdSimpleD(s32, u16);
extern void btlCmdSimpleJ(s32, u16);
extern void func_001FF030(s32, u16);

extern s8 D_003BB870;

extern s8 D_003A5440[];

extern s8 D_003A5460[];

extern s8 D_003A5488[];

extern u8 D_003BB818[];

extern u8 D_003BD476;

extern void *func_002CFEB8(s32 size);

extern s32 sceDopen(char *);

extern s32 D_003BD868;

extern s8 D_003A54A8[];

extern void func_001FB0A8(s32, ...);

extern s8 D_003A54C8[];

extern s8 D_003A54F0[];

extern s8 D_003A5518[];

extern s8 D_003A5538[];

extern s8 D_003A5558[];

extern u32 D_003BB6C8;

extern void func_003014F0();

extern u8 D_003BB820[];

extern u64 func_001D9718(void);

typedef struct BtlUnit {
    u8 unk_00[0x70];
    f32 rotation[4];
    f32 scale;
    u8 unk_84[4];
    f32 zOffset;
    u8 unk_8C[4];
    f32 muzzleOffset[4];
    f32 localBodyPosition[4];
    f32 height;
    f32 reach;
    u8 unk_B8[0x30];
    u32 stateFlags; /* 0xE8 */
    u8 unk_EC[4];
    s32 effectState; /* 0xF0 */
    u8 unk_F4[4];
    s16 effectTimerA; /* 0xF8 */
    s16 effectTimerB; /* 0xFA */
    s32 effectArgA; /* 0xFC */
    s32 effectArgB; /* 0x100 */
    f32 effectValue; /* 0x104 */
    u64 unitId; /* 0x108: compared against the battle command's unit ID */
    u32 flags;
    u32 effectFlags; /* 0x114 */
    u8 unk_118[8];
    u16 unk_120;
    u8 unk_122[2];
    u16 mode;
    u8 unk_126[0x1F6];
    u32 effectObject;
    u32 statusEffectHandle;
    u8 unk_324[0x20];
    struct BtlUnit *next;
} BtlUnit;

typedef struct BtlActor {
    u8 unk_00[0x18];
    BtlUnit *unit;
} BtlActor;

typedef struct BtlList {
    u8 unk_00[0x20];
    s32 count;
} BtlList;

typedef struct BtlState {
    u8 unk_000[0x228];
    BtlUnit *units;
    u8 unk_22C[0x70];
    BtlList *list;
    u8 unk_2A0[4];
    s32 slot;
    u8 unk_2A8[0x20C];
    u32 buttonTextureHandle; /* 0x4B4: retained until battle UI releases it */
    u8 unk_4B8[0x138];
    void (*updateCallback)(void); /* 0x5F0 */
} BtlState;

typedef struct BtlCmdCtx {
    u8 unk_00[0x18];
    BtlUnit *unit;
    u8 unk_1C[4];
    s32 commandMode;
    s32 commandValue;
    u8 unk_28[4];
    s32 unk_2C;
    s32 unk_30;
} BtlCmdCtx;

extern f32 btlUnitGetTopY(BtlUnit *);

typedef struct BtlVec3 {
    f32 x, y, z;
} BtlVec3;

extern f32 bfWaitReadArgFloat(s32);

extern s32 func_001A17F8(void *);
extern s32 func_001A1838(void *);

extern void btlGetUnitWorldPos(BtlUnit *, f32 *);
extern void effMiscQuaternionToMatrixVU(void);
extern void effObjFetchInnerFirstVec(u32);

extern f32 func_001F79A0(f32 *, f32 *, f32 *);

typedef union BtlVec4 {
    f32 f[4];
    u128 q;
} BtlVec4;

extern u64 func_001D9780(void);

extern u64 btlCreateCommandSoundTask(u64, u64);

extern s32 D_003BB3D8;

extern u32 func_0020A3F0(void);

extern u32 btlGetEffectValue(void);

extern u32 btlGetEffectActive(void);

extern u32 func_002099A0(void);

extern u32 func_00208C68(void);

extern u64 func_001ACAE0(void);

extern u16 scrReadIntParameter(u32);

extern s32 func_0010D6A8(void);

extern u32 D_003BD858;

extern u32 func_0029BF88(u32, u32);

extern s32 func_001A17F0(void);
extern s32 func_001FEC68(s32 context, s32 actor, u32 mask);
extern void func_0010D5F0();
extern void btlCmdSimpleC(s32, u16);
extern u8 *btlAllocTask(s32);
extern void func_001F60E8(void);

typedef struct BtlControlObject {
    u8 enabled; /* 0x00 */
    u8 pad01[0x0F];
    u8 state; /* 0x10 */
    u8 pad11[0x0F];
    u16 type; /* 0x20 */
    u8 pad22[0x26];
    s32 status; /* 0x48 */
    void (*update)(void); /* 0x4C */
} BtlControlObject;

u8 *btlCreateControlObject(void) {
    BtlControlObject *object;
    object = (BtlControlObject *)btlAllocTask(0);
    object->enabled = 1;
    object->update = func_001F60E8;
    object->type = 0x60;
    object->status = 0;
    object->state = 0;
    return (u8 *)object;
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F6158);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F6300);

void btlUnitGetMuzzlePosVU(BtlUnit *unit) {
    f32 pos[4];
    btlGetUnitWorldPos(unit, pos);
    pos[2] += unit->zOffset;
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(unit->rotation));
    effMiscQuaternionToMatrixVU();
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(unit->muzzleOffset));
    __asm__ volatile(".set noreorder\n\tqmtc2.ni %0, vf2\n\t.set reorder" : : "r"(unit->scale));
    __asm__ volatile(
        ".set noreorder\n\t"
        "vmulx.xyzw vf10, vf10, vf2x\n\t"
        "vmulax.xyzw ACC, vf28, vf10x\n\t"
        "vmadday.xyzw ACC, vf29, vf10y\n\t"
        "vmaddaz.xyzw ACC, vf30, vf10z\n\t"
        "vmaddw.xyzw vf10, vf31, vf10w\n\t"
        "lqc2 vf11, 0(%0)\n\t"
        "vadd.xyzw vf10, vf10, vf11\n\t"
        ".set reorder"
        :
        : "r"(pos));
}


void btlUnitGetBodyPosVU(BtlUnit *unit) {
    f32 pos[4];
    btlGetUnitWorldPos(unit, pos);
    pos[2] += unit->zOffset;
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(unit->rotation));
    effMiscQuaternionToMatrixVU();
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(unit->localBodyPosition));
    __asm__ volatile(".set noreorder\n\tqmtc2.ni %0, vf2\n\t.set reorder" : : "r"(unit->scale));
    __asm__ volatile(
        ".set noreorder\n\t"
        "vmulx.xyzw vf10, vf10, vf2x\n\t"
        "vmulax.xyzw ACC, vf28, vf10x\n\t"
        "vmadday.xyzw ACC, vf29, vf10y\n\t"
        "vmaddaz.xyzw ACC, vf30, vf10z\n\t"
        "vmaddw.xyzw vf10, vf31, vf10w\n\t"
        "lqc2 vf11, 0(%0)\n\t"
        "vadd.xyzw vf10, vf10, vf11\n\t"
        ".set reorder"
        :
        : "r"(pos));
}


void btlUnitGetEffectPosVU(BtlUnit *unit) {
    f32 pos[4];
    if (!(unit->flags & 2)) {
        btlUnitGetMuzzlePosVU(unit);
        return;
    }
    effObjFetchInnerFirstVec(unit->effectObject);
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(pos) : "memory");
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(unit->rotation));
    effMiscQuaternionToMatrixVU();
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(unit->muzzleOffset));
    __asm__ volatile(".set noreorder\n\tqmtc2.ni %0, vf2\n\t.set reorder" : : "r"(unit->scale));
    __asm__ volatile(
        ".set noreorder\n\t"
        "vmulx.xyzw vf10, vf10, vf2x\n\t"
        "vmulax.xyzw ACC, vf28, vf10x\n\t"
        "vmadday.xyzw ACC, vf29, vf10y\n\t"
        "vmaddaz.xyzw ACC, vf30, vf10z\n\t"
        "vmaddw.xyzw vf10, vf31, vf10w\n\t"
        "lqc2 vf11, 0(%0)\n\t"
        "vadd.xyzw vf10, vf10, vf11\n\t"
        ".set reorder"
        :
        : "r"(pos));
}


f32 btlUnitGetMaxScaledExtent(BtlUnit *unit) {
    f32 reach;
    f32 height;

    reach = unit->reach;
    height = unit->height;
    if (height < reach) {
        return reach * unit->scale;
    }
    return height * unit->scale;
}

f32 btlUnitGetTopY(BtlUnit *unit) {
    f32 pos[4];
    btlUnitGetMuzzlePosVU(unit);
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(pos) : "memory");
    return unit->height * unit->scale * 0.5f - pos[1];
}


f32 btlUnitGetBottomY(BtlUnit *unit) {
    f32 pos[4];
    btlUnitGetMuzzlePosVU(unit);
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(pos) : "memory");
    return -pos[1] - unit->height * unit->scale * 0.5f;
}


INCLUDE_ASM(const s32, "game/code_001F6110", func_001F66D8);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F6970);

f32 btlGetMaxUnitTop(u32 mask) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    f32 best = 0.0f;
    s32 first = 1;
    while (unit != NULL) {
        if ((unit->flags & 1) && (unit->flags & mask)) {
            f32 value = btlUnitGetTopY(unit);
            if (first) {
                best = value;
                first = 0;
            } else if (best < value) {
                best = value;
            }
        }
        unit = unit->next;
    }
    return best;
}



f32 btlGetMaxUnitReach(u32 mask) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    f32 best = 0.0f;
    s32 first = 1;
    while (unit != NULL) {
        if ((unit->flags & 1) && (unit->flags & mask)) {
            f32 value = unit->reach * unit->scale;
            if (first) {
                best = value;
                first = 0;
            } else if (best < value) {
                best = value;
            }
        }
        unit = unit->next;
    }
    return best;
}

f32 btlGetExtremeUnitY(u32 mask) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    f32 best = 0.0f;
    s32 first = 1;
    f32 pos[4];
    f32 value;
    while (unit != NULL) {
        if ((unit->flags & 1) && (unit->flags & mask)) {
            btlUnitGetMuzzlePosVU(unit);
            __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(pos) : "memory");
            if (mask & 0x200) {
                value = pos[2] + unit->reach * unit->scale;
                if (first) {
                    best = value;
                    first = 0;
                } else if (best < value) {
                    best = value;
                }
            } else {
                value = pos[2] - unit->reach * unit->scale;
                if (first) {
                    best = value;
                    first = 0;
                } else if (value < best) {
                    best = value;
                }
            }
        }
        unit = unit->next;
    }
    return best;
}


INCLUDE_ASM(const s32, "game/code_001F6110", func_001F6E28);

/* Compare VU-computed distances from the target's muzzle to eligible units. */
BtlUnit *btlFindNearestUnit(u32 mask, BtlUnit *target) {
    BtlState *state = (BtlState *)func_001A17F0();
    BtlUnit *unit;
    BtlUnit *nearest;
    s32 first;
    f32 nearestDistance;
    f32 distance;
    BtlVec4 targetPos;
    btlUnitGetMuzzlePosVU(target);
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(&targetPos) : "memory");
    unit = state->units;
    nearest = NULL;
    first = 1;
    nearestDistance = 0.0f;
    while (unit != NULL) {
        if (unit->flags & 1) {
            if (!(unit->flags & 0xC0)) {
                if (target != unit) {
                    if (unit->flags & mask) {
                        btlUnitGetMuzzlePosVU(unit);
                        __asm__ volatile(".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, $vf2\n\tvaddx.y $vf10, $vf0, $vf2x\n\t.set reorder" : : "f"(targetPos.f[1]) : "$2");
                        __asm__ volatile(".set noreorder\n\tlqc2 $vf11, 0(%0)\n\t.set reorder" : : "r"(&targetPos));
                        __asm__ volatile(".set noreorder\n\tvsub.xyzw $vf10, $vf10, $vf11\n\tvmul.xyz $vf2, $vf10, $vf10\n\tvaddy.x $vf2, $vf2, $vf2y\n\tvaddz.x $vf2, $vf2, $vf2z\n\tvsqrt Q, $vf2x\n\tvwaitq\n\tcfc2.ni $2, $vi22\n\tmtc1 $2, %0\n\t.set reorder" : "=f"(distance) : : "$2");
                        if (first) {
                            nearestDistance = distance;
                            nearest = unit;
                            first = 0;
                        } else if (distance < nearestDistance) {
                            nearestDistance = distance;
                            nearest = unit;
                        }
                    }
                }
            }
        }
        unit = unit->next;
    }
    return nearest;
}

/* Return the eligible unit farthest from point, using the VU distance. */
BtlUnit *btlFindFarthestUnit(u32 mask, f32 *point) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    BtlUnit *farthest = NULL;
    f32 farthestDistance = 0.0f;
    f32 distance;
    while (unit != NULL) {
        if (unit->flags & 1) {
            if (!(unit->flags & 0xC0)) {
                if (unit->flags & mask) {
                    btlUnitGetMuzzlePosVU(unit);
                    __asm__ volatile(
                        ".set noreorder\n\t"
                        "mfc1 $2, %2\n\t"
                        "qmtc2.ni $2, vf2\n\t"
                        "vaddx.y vf10, vf0, vf2x\n\t"
                        "lqc2 vf11, 0(%1)\n\t"
                        "vsub.xyzw vf10, vf10, vf11\n\t"
                        "vmul.xyz vf2, vf10, vf10\n\t"
                        "vaddy.x vf2, vf2, vf2y\n\t"
                        "vaddz.x vf2, vf2, vf2z\n\t"
                        "vsqrt Q, vf2x\n\t"
                        "vwaitq\n\t"
                        "cfc2.ni $2, $vi22\n\t"
                        "mtc1 $2, %0\n\t"
                        ".set reorder"
                        : "=f"(distance)
                        : "r"(point), "f"(point[1]));
                    if (farthestDistance < distance) {
                        farthestDistance = distance;
                        farthest = unit;
                    }
                }
            }
        }
        unit = unit->next;
    }
    return farthest;
}


INCLUDE_ASM(const s32, "game/code_001F6110", func_001F72C8);

void func_001F73E0(void) {
    BtlUnit *unit;

    for (unit = ((BtlState *)func_001A17F0())->units; unit != NULL; unit = unit->next) {
        btlFlagUnitDefeatCandidate((s32)unit);
    }
}

void func_001F7428(void) {
    BtlUnit *unit;

    for (unit = ((BtlState *)func_001A17F0())->units; unit != NULL; unit = unit->next) {
        func_001D54C0((s32)unit);
    }
}

void func_001F7470(s32 mask) {
    BtlUnit *unit;

    unit = ((BtlState *)func_001A17F0())->units;
    if (unit != NULL) {
        do {
            if (unit->flags & mask) {
                btlFlagUnitDefeatCandidate((s32)unit);
            }
            unit = unit->next;
        } while (unit != NULL);
    }
}

void func_001F74D0(s32 mask) {
    BtlUnit *unit;

    unit = ((BtlState *)func_001A17F0())->units;
    if (unit != NULL) {
        do {
            if (unit->flags & mask) {
                func_001D54C0((s32)unit);
            }
            unit = unit->next;
        } while (unit != NULL);
    }
}

void func_001F7530(s32 actor) {
    u32 i = 0;
    u32 count = func_001DAE48(actor);
    if (count != 0) {
        do {
            btlFlagUnitDefeatCandidate(func_001DAE50(actor, i));
            i++;
        } while (i < count);
    }
}


void func_001F7598(s32 actor) {
    u32 i = 0;
    u32 count = func_001DAE48(actor);
    if (count != 0) {
        do {
            func_001D54C0(func_001DAE50(actor, i));
            i++;
        } while (i < count);
    }
}


extern s32 func_001D5D58(s32);
extern void btlSetUnitPosition(s32, s32);
extern void btlSetUnitRotation(s32, s32);
extern void func_001D5990(s32);
extern void func_001D5578(s32, s32, s32, f32);

void btlUpdateUnitActors(void) {
    BtlState *state = (BtlState *)func_001A17F0();
    BtlUnit *actor = state->units;
    while (actor != NULL) {
        btlFlagUnitDefeatCandidate((s32)actor);
        btlSetUnitPosition((s32)actor, (s32)((u8 *)actor + 0x30));
        btlSetUnitRotation((s32)actor, (s32)((u8 *)actor + 0x40));
        if ((func_001D5D58((s32)actor) == 0 && actor->effectState != 0) ||
            (actor->stateFlags & 2) != 0) {
            func_001D5990((s32)actor);
            actor->effectTimerA = 0;
            actor->effectTimerB = 0;
            func_001D5578((s32)actor, actor->effectArgA, actor->effectArgB,
                          actor->effectValue);
        }
        actor->effectFlags &= ~0x8000;
        actor = actor->next;
    }
    {
        void (*callback)(void) = state->updateCallback;
        if (callback != 0) {
            callback();
        }
    }
}

extern void evtSetUnitStatusFlags(u32);
extern void func_00221D98(u32, s32);

void btlRefreshUnitEffects(void) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    u32 handle;
    while (unit != NULL) {
        if (unit->flags & 2) {
            if (unit->effectFlags & 0x10) {
                handle = unit->statusEffectHandle;
                evtSetUnitStatusFlags(handle);
                __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(unit));
                func_00221D98(handle, 0);
            }
        }
        unit = unit->next;
    }
}


s32 btlCountActiveUnitsWithFlags(s32 mask) {
    BtlUnit *unit;
    s32 count = 0;
    s32 flags;

    unit = ((BtlState *)func_001A17F0())->units;
    if (unit != NULL) {
        do {
            flags = unit->flags;
            if (((flags & mask) != 0) && ((flags & 0x20) == 0)) {
                count += flags & 1;
            }
            unit = unit->next;
        } while (unit != NULL);
    }
    return count;
}

extern void func_002E7F20(f32, f32, f32);
extern f32 func_002FA1F0(f32, f32);
extern u128 D_0035F9E0;

s32 func_001F77D8(f32 *from, f32 *to) {
    f32 delta[4];
    delta[0] = to[0] - from[0];
    delta[2] = to[2] - from[2];
    if (delta[0] != 0.0f || delta[2] != 0.0f) {
        func_002E7F20(0.0f, func_002FA1F0(delta[0], delta[2]), 0.0f);
        return 1;
    }
    __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(&D_0035F9E0));
    return 0;
}


INCLUDE_ASM(const s32, "game/code_001F6110", func_001F7868);

/* Unit normal of the triangle (a, b, c); result in vf10 (VU register convention). */
/* vu0 routine: unit normal of triangle (a, b, c) into vf10 */
void btlTriangleNormalVU(f32 *a, f32 *b, f32 *c) {
    __asm__ volatile(
        ".set noreorder\n\t"
        "lqc2 vf10, 0(%1)\n\t"
        "lqc2 vf11, 0(%0)\n\t"
        "vsub.xyzw vf10, vf10, vf11\n\t"
        "vmove.xyzw vf12, vf10\n\t"
        "lqc2 vf11, 0(%2)\n\t"
        "lqc2 vf10, 0(%0)\n\t"
        "vsub.xyzw vf11, vf11, vf10\n\t"
        "vmove.xyzw vf10, vf12\n\t"
        "vopmula.xyz ACC, vf10, vf11\n\t"
        "vopmsub.xyz vf10, vf11, vf10\n\t"
        "vmove.xyzw vf11, vf10\n\t"
        "vmove.xyzw vf10, vf12\n\t"
        "vopmula.xyz ACC, vf10, vf11\n\t"
        "vopmsub.xyz vf10, vf11, vf10\n\t"
        "vmul.xyz vf2, vf10, vf10\n\t"
        "vmulax.w ACC, vf0, vf2x\n\t"
        "vmadday.w ACC, vf0, vf2y\n\t"
        "vmaddz.w vf2, vf0, vf2z\n\t"
        "vrsqrt Q, vf0w, vf2w\n\t"
        "vwaitq\n\t"
        "vmulq.xyz vf10, vf10, Q\n\t"
        ".set reorder"
        :
        : "r"(a), "r"(b), "r"(c)
        : "memory");
}


f32 func_001F79A0(f32 *a, f32 *b, f32 *c) {
    f32 normal[4];
    f32 dot;
    btlTriangleNormalVU(a, b, c);
    __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(normal) : "memory");
    __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(a));
    __asm__ volatile(".set noreorder\n\tlqc2 $vf11, 0(%0)\n\t.set reorder" : : "r"(c));
    __asm__ volatile(".set noreorder\n\tvsub.xyzw $vf10, $vf10, $vf11\n\tvmove.xyzw $vf11, $vf10\n\t.set reorder");
    __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(normal));
    __asm__ volatile(".set noreorder\n\tvmul.xyz $vf2, $vf10, $vf11\n\tvaddy.x $vf2, $vf2, $vf2y\n\tvaddz.x $vf2, $vf2, $vf2z\n\tqmfc2.ni $2, $vf2\n\tmtc1 $2, %0\n\t.set reorder" : "=f"(dot) : : "$2");
    return dot;
}

/* vu0 routine: vf10 = c + normalize(d - c) * dist */
void btlPointOffPlaneVU(f32 *a, f32 *b, f32 *c, f32 *d) {
    f32 dist = func_001F79A0(a, b, c);
    __asm__ volatile(
        ".set noreorder\n\t"
        "lqc2 vf10, 0(%0)\n\t"
        "lqc2 vf11, 0(%1)\n\t"
        "vsub.xyzw vf10, vf10, vf11\n\t"
        "vmul.xyz vf2, vf10, vf10\n\t"
        "vmulax.w ACC, vf0, vf2x\n\t"
        "vmadday.w ACC, vf0, vf2y\n\t"
        "vmaddz.w vf2, vf0, vf2z\n\t"
        "vrsqrt Q, vf0w, vf2w\n\t"
        "vwaitq\n\t"
        "vmulq.xyz vf10, vf10, Q\n\t"
        ".set reorder"
        :
        : "r"(d), "r"(c));
    __asm__ volatile(
        ".set noreorder\n\t"
        "mfc1 $2, %0\n\t"
        "qmtc2.ni $2, vf2\n\t"
        "vmulx.xyzw vf10, vf10, vf2x\n\t"
        "lqc2 vf11, 0(%1)\n\t"
        "vadd.xyzw vf10, vf10, vf11\n\t"
        ".set reorder"
        :
        : "f"(dist), "r"(c));
}


/* vu0 routine: vf10 = c + n * dot(n, a - c), n = unit normal of triangle (a, b, c) */
void btlProjectOnPlaneVU(f32 *a, f32 *b, f32 *c) {
    f32 normal[4];
    f32 dot;
    btlTriangleNormalVU(a, b, c);
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(normal) : "memory");
    __asm__ volatile(
        ".set noreorder\n\t"
        "lqc2 vf10, 0(%1)\n\t"
        "lqc2 vf11, 0(%2)\n\t"
        "vsub.xyzw vf10, vf10, vf11\n\t"
        "vmove.xyzw vf11, vf10\n\t"
        "lqc2 vf10, 0(%3)\n\t"
        "vmul.xyz vf2, vf10, vf11\n\t"
        "vaddy.x vf2, vf2, vf2y\n\t"
        "vaddz.x vf2, vf2, vf2z\n\t"
        "qmfc2.ni $2, vf2\n\t"
        "mtc1 $2, %0\n\t"
        ".set reorder"
        : "=f"(dot)
        : "r"(a), "r"(c), "r"(normal));
    __asm__ volatile(
        ".set noreorder\n\t"
        "mfc1 $2, %0\n\t"
        "qmtc2.ni $2, vf2\n\t"
        "vmulx.xyzw vf10, vf10, vf2x\n\t"
        "lqc2 vf11, 0(%1)\n\t"
        "vadd.xyzw vf10, vf10, vf11\n\t"
        ".set reorder"
        :
        : "f"(dot), "r"(c));
}



s32 btlPointInBox(BtlVec3 *a, BtlVec3 *b, BtlVec3 *p) {
    if (((a->x >= p->x && p->x >= b->x) || (a->x <= p->x && p->x <= b->x))
        && ((a->y >= p->y && p->y >= b->y) || (a->y <= p->y && p->y <= b->y))
        && ((a->z >= p->z && p->z >= b->z) || (a->z <= p->z && p->z <= b->z))) {
        return 1;
    }
    return 0;
}


/* vu0 routine: blend two RGBA8888 colours by t (lerp in float, packed back to RGBA8888) */
u32 btlBlendColor(u32 colorA, u32 colorB, f32 t) {
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    if (t >= 1.0f) {
        return colorB;
    }
    unit = 0x3C000000;
    color1[0] = colorB;
    EE_MMI_RGBA_UNPACK(color1, unit);
    __asm__ volatile (".set noreorder\n\tvmove.xyzw vf11, vf10\n\t.set reorder");
    color2[0] = colorA;
    EE_MMI_RGBA_UNPACK(color2, unit);
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %0\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        ".set reorder"
        : : "f"(1.0f - t) : "$2");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $3, %0\n"
        "qmtc2.ni $3, vf2\n"
        "vmulx.xyzw vf11, vf11, vf2x\n"
        "vadd.xyzw vf10, vf10, vf11\n"
        ".set reorder"
        : : "f"(t) : "$3");
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    
    return packed;
}


/* Blend two RGBA float vectors (0..1 scale) by t and pack to 8-bit channels. */
/* vu0 routine: blend two RGBA float vectors by t, packed to RGBA8888 */
u32 btlBlendColorVec(f32 *a, f32 *b, f32 t) {
    u32 packed;
    s32 blended[4];
    __asm__ volatile(
        ".set noreorder\n\t"
        "lqc2 vf10, 0(%0)\n\t"
        "lqc2 vf11, 0(%1)\n\t"
        ".set reorder"
        : : "r"(a), "r"(b));
    __asm__ volatile(
        ".set noreorder\n\t"
        "mfc1 $2, %0\n\t"
        "qmtc2.ni $2, vf2\n\t"
        "vmulx.xyzw vf10, vf10, vf2x\n\t"
        ".set reorder"
        : : "f"(1.0f - t));
    __asm__ volatile(
        ".set noreorder\n\t"
        "mfc1 $3, %0\n\t"
        "qmtc2.ni $3, vf2\n\t"
        "vmulx.xyzw vf11, vf11, vf2x\n\t"
        "vadd.xyzw vf10, vf10, vf11\n\t"
        ".set reorder"
        : : "f"(t));
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    return blended[0];
}


/* Scalar interpolation state shared by the initialization routines below. */
typedef struct BtlScalarRange {
    f32 start;       /* 0x00 */
    f32 end;         /* 0x04 */
    f32 inverseSpan; /* 0x08 */
    f32 zero;        /* 0x0C */
    f32 target;      /* 0x10 */
} BtlScalarRange;

void func_001F7CC8(s32 arg0, f32 arg1) {
    ((BtlScalarRange *)arg0)->start = arg1;
    ((BtlScalarRange *)arg0)->end = 0;
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F7CD8);

void func_001F7D30(s32 arg0, f32 arg1) {
    f32 zero;

    ((BtlScalarRange *)arg0)->zero = 0.0f;
    ((BtlScalarRange *)arg0)->start = arg1;
    zero = ((BtlScalarRange *)arg0)->zero;
    ((BtlScalarRange *)arg0)->end = arg1;
    ((BtlScalarRange *)arg0)->target = zero;
    if (arg1 == zero) {
        return;
    }
    ((BtlScalarRange *)arg0)->inverseSpan = 1.0f / (arg1 * arg1 * 0.25f);
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F7D80);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F7DF8);

INCLUDE_ASM(const s32, "game/code_001F6110", btlDrawButtonIcon);

void func_001F8280(void) {
    func_001FB0A8((s32)"btl:[%s]\n", D_003BB6B8);
    D_003BD854 = func_002EB028(D_003BB6B8, &D_003BD858, 0);
}

/* Cache and later release the loaded battle resource in battle state. */
void btlRetainButtonTexture(void) {
    BtlState *state;
    u32 resource;

    state = (BtlState *)func_001A17F0();
    resource = func_0029BF88(D_003BD858, 0x10000);
    state->buttonTextureHandle = resource;
}

void btlReleaseButtonTexture(void) {
    BtlState *state;

    state = (BtlState *)func_001A17F0();
    effReleaseSharedReference(state->buttonTextureHandle);
    state->buttonTextureHandle = 0;
}

/* Script command handlers set the command state at +0x20 and its
 * argument at +0x24; their numeric opcodes are still unidentified. */
u32 func_001F8328(void) {
    BtlCmdCtx *context;

    context = (BtlCmdCtx *)func_0010D6A8();
    context->commandMode = 1;
    context->commandValue = 0;
    return 1;
}

u32 func_001F8358(void) {
    BtlCmdCtx *context;

    context = (BtlCmdCtx *)func_0010D6A8();
    context->commandMode = 6;
    context->commandValue = 0xc2;
    return 1;
}

u32 func_001F8388(void) {
    BtlCmdCtx *context;

    context = (BtlCmdCtx *)func_0010D6A8();
    context->commandValue = 1;
    context->commandMode = 0xd;
    return 1;
}

u32 func_001F83B8(void) {
    BtlCmdCtx *context;

    context = (BtlCmdCtx *)func_0010D6A8();
    context->commandValue = 1;
    context->commandMode = 10;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F83E8);

u32 func_001F84A8(void) {
    s32 context = func_0010D6A8();
    u16 first = scrReadIntParameter(0);
    u32 second = scrReadIntParameter(1);
    *(u32 *)(context + 0x20) = 2;
    *(u32 *)(context + 0x24) = first;
    *(u32 *)(context + 0x8c) = second;
    *(u32 *)(context + 0x38) = second;
    return 1;
}

u32 func_001F8508(void) {
    s32 context;

    context = func_0010D6A8();
    *(u32 *)(context + 0xc) = *(u32 *)(context + 0xc) | 1;
    return 1;
}

u32 func_001F8538(void) {
    u16 value;
    s32 context;

    context = func_0010D6A8();
    value = scrReadIntParameter(0);
    *(u16 *)(*(s32 *)(context + 0x18) + 0x122) = value;
    return 1;
}

u32 func_001F8578(void) {
    s32 battle;
    s16 value;

    battle = func_001A17F0();
    func_0010D6A8();
    value = scrReadIntParameter(0);
    *(u8 *)(battle + 0x25e) = 4;
    *(s32 *)(battle + 0x280) = value;
    return 1;
}

u32 func_001F85C8(void) {
    btlCmdWithArgA(func_0010D6A8());
    return 1;
}

u32 func_001F85F0(void) {
    s32 context = func_0010D6A8();
    u16 value = scrReadIntParameter(0);
    btlCmdSimpleB(context, value);
    return 1;
}

u32 func_001F8630(void) {
    btlCmdWithArgB(func_0010D6A8());
    return 1;
}

u32 func_001F8658(void) {
    btlCmdWithArgC(func_0010D6A8());
    return 1;
}

u32 func_001F8680(void) {
    s32 context = func_0010D6A8();
    u16 value = scrReadIntParameter(0);
    btlCmdSimpleA(context, value);
    return 1;
}

u32 func_001F86C0(void) {
    btlCmdWithArgE(func_0010D6A8());
    return 1;
}

u32 func_001F86E8(void) {
    btlCmdWithArgD(func_0010D6A8());
    return 1;
}

u32 func_001F8710(void) {
    s32 context = func_0010D6A8();
    u16 value = scrReadIntParameter(0);
    btlCmdSimpleC(context, value);
    return 1;
}

u32 func_001F8750(void) {
    s32 context = func_0010D6A8();
    u16 value = scrReadIntParameter(0);
    btlCmdSimpleD(context, value);
    return 1;
}

u32 func_001F8790(void) {
    s32 context = func_0010D6A8();
    u16 value = scrReadIntParameter(0);
    btlCmdSimpleE(context, value);
    return 1;
}

u32 func_001F87D0(void) {
    btlCmdWithArgA(func_0010D6A8());
    return 1;
}

u32 func_001F87F8(void) {
    btlCmdSimpleF(func_0010D6A8(), 0);
    return 1;
}

u32 func_001F8828(void) {
    btlCmdSimpleG(func_0010D6A8(), 0);
    return 1;
}

u32 func_001F8858(void) {
    btlCmdSimpleH(func_0010D6A8(), 0);
    return 1;
}

u32 func_001F8888(void) {
    btlCmdWithArgF(func_0010D6A8());
    return 1;
}

u32 func_001F88B0(void) {
    s32 context = func_0010D6A8();
    u16 value = scrReadIntParameter(0);
    btlCmdSimpleJ(context, value);
    return 1;
}

u32 func_001F88F0(void) {
    btlCmdSimpleI(func_0010D6A8(), 0);
    return 1;
}

/* Script command selection: low/high flag words gate the stored choices. */
typedef struct BtlCommandContext {
    u8 pad00[0x18];
    s32 actor; /* 0x18 */
    u8 pad1C[0x74];
    u32 selectionFlagsA; /* 0x90 */
    u32 selectionFlagsB; /* 0x94 */
    s32 choicesA[26]; /* 0x98..0xFC */
    s32 choicesB[17]; /* 0x100..0x140 */
} BtlCommandContext;

u32 btlNbScriptCheckActorFlag(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x400000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 1;
        ((BtlCommandContext *)context)->choicesA[0] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~1;
    }
    return 1;
}

u32 func_001F89B0(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x6000000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 2;
        ((BtlCommandContext *)context)->choicesA[1] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~2;
    }
    return 1;
}

u32 func_001F8A40(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x800000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 4;
        ((BtlCommandContext *)context)->choicesA[2] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~4;
    }
    return 1;
}
u32 func_001F8AD0(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x1400000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 8;
        ((BtlCommandContext *)context)->choicesA[3] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~8;
    }
    return 1;
}
u32 func_001F8B60(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x1800000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x10;
        ((BtlCommandContext *)context)->choicesA[4] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x10;
    }
    return 1;
}
u32 func_001F8BF0(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x1C00000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x20;
        ((BtlCommandContext *)context)->choicesA[5] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x20;
    }
    return 1;
}
u32 func_001F8C80(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x2000000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x40;
        ((BtlCommandContext *)context)->choicesA[6] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x40;
    }
    return 1;
}
u32 func_001F8D10(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x2400000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x80;
        ((BtlCommandContext *)context)->choicesA[7] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x80;
    }
    return 1;
}
u32 func_001F8DA0(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x2800000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x100;
        ((BtlCommandContext *)context)->choicesA[8] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x100;
    }
    return 1;
}
u32 func_001F8E30(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x2C00000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x200;
        ((BtlCommandContext *)context)->choicesA[9] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x200;
    }
    return 1;
}
u32 func_001F8EC0(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x3000000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x400;
        ((BtlCommandContext *)context)->choicesA[10] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x400;
    }
    return 1;
}
u32 func_001F8F50(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x3400000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x800;
        ((BtlCommandContext *)context)->choicesA[11] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x800;
    }
    return 1;
}
u32 func_001F8FE0(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x3800000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x1000;
        ((BtlCommandContext *)context)->choicesA[12] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x1000;
    }
    return 1;
}

u32 func_001F9070(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, 0x4800000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x2000;
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x2000;
    }
    return 1;
}

u32 func_001F90E8(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, 0x5C00000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x2000000;
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x2000000;
    }
    return 1;
}

u32 func_001F9168(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x4C00000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x4000;
        ((BtlCommandContext *)context)->choicesA[14] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x4000;
    }
    return 1;
}
u32 func_001F91F8(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x5000000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x4000;
        ((BtlCommandContext *)context)->choicesA[14] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x4000;
    }
    return 1;
}

u32 func_001F9288(void) {
    s32 context;

    context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, 0x6c00000) != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001F92D8(void) {
    s32 context;

    context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, 0x9400000) != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001F9328(void) {
    s32 context;

    context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, 0x9c00000) != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001F9378(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x8C00000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x40000;
        ((BtlCommandContext *)context)->choicesA[18] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x40000;
    }
    return 1;
}
u32 func_001F9410(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x9000000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x80000;
        ((BtlCommandContext *)context)->choicesA[19] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x80000;
    }
    return 1;
}

u32 func_001F94A8(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x10400000)) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001F9518(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x10800000)) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001F9588(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0xA000000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x100000;
        ((BtlCommandContext *)context)->choicesA[20] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x100000;
    }
    return 1;
}
u32 func_001F9620(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0xA400000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x200000;
        ((BtlCommandContext *)context)->choicesA[21] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x200000;
    }
    return 1;
}
u32 func_001F96B8(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0xA800000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x400000;
        ((BtlCommandContext *)context)->choicesA[22] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x400000;
    }
    return 1;
}
u32 func_001F9750(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0xAC00000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x800000;
        ((BtlCommandContext *)context)->choicesA[23] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x800000;
    }
    return 1;
}

u32 func_001F97E8(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0xB800000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 1;
        ((BtlCommandContext *)context)->choicesB[1] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~1;
    }
    return 1;
}
u32 func_001F9878(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0xBC00000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 2;
        ((BtlCommandContext *)context)->choicesB[2] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~2;
    }
    return 1;
}
u32 func_001F9908(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0xC000000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 4;
        ((BtlCommandContext *)context)->choicesB[3] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~4;
    }
    return 1;
}
u32 func_001F9998(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0xC400000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 8;
        ((BtlCommandContext *)context)->choicesB[4] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~8;
    }
    return 1;
}
u32 func_001F9A28(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0xC800000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 0x10;
        ((BtlCommandContext *)context)->choicesB[5] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~0x10;
    }
    return 1;
}
u32 func_001F9AB8(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0xCC00000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 0x20;
        ((BtlCommandContext *)context)->choicesB[6] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~0x20;
    }
    return 1;
}
u32 func_001F9B48(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0xD400000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 0x400;
        ((BtlCommandContext *)context)->choicesB[11] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~0x400;
    }
    return 1;
}
u32 func_001F9BD8(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0xD800000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 0x800;
        ((BtlCommandContext *)context)->choicesB[12] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~0x800;
    }
    return 1;
}

u32 func_001F9C68(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x10C00000)) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001F9CD8(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x11000000)) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001F9D48(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x9800000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 0x40;
        ((BtlCommandContext *)context)->choicesB[7] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~0x40;
    }
    return 1;
}
u32 func_001F9DD8(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0xDC00000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 0x1000;
        ((BtlCommandContext *)context)->choicesB[13] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~0x1000;
    }
    return 1;
}

u32 func_001F9E68(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, 0xD000000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 0x80;
        ((BtlCommandContext *)context)->choicesB[8] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~0x80;
    }
    return 1;
}

u32 func_001F9EF0(void) {
    s32 context;

    context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, 0xe800000) != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001F9F40(void) {
    s32 context;

    context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, 0xec00000) != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001F9F90(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, 0xB000000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x1000000;
        ((BtlCommandContext *)context)->choicesA[24] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x1000000;
    }
    return 1;
}

u32 func_001FA018(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, 0x0b400000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x04000000;
        ((BtlCommandContext *)context)->choicesB[0] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x04000000;
    }
    return 1;
}

u32 func_001FA0A0(void) {
    if (func_002099E0() != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001FA0E0(void) {
    if (btlHasEffectActor() != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001FA120(void) {
    if (func_00207C18() != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 btlCmdCheckHpPercent(void) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    s32 side = scrReadIntParameter(0);
    s32 id = scrReadIntParameter(1);
    s32 percent = scrReadIntParameter(2);
    u32 mask = 0x200;
    if (side) {
        mask = 0x400;
    }
    while (unit != NULL) {
        if ((unit->flags & 1) && (unit->flags & mask) && !(unit->flags & 0x20) && unit->unitId == id) {
            u8 *stats = (u8 *)unit + 0x120;
            s32 current = func_001A17F8(stats);
            s32 maximum = func_001A1838(stats);
            if (!((u32)(maximum * percent) < (u32)(current * 100))) {
                func_0010D5F0(1);
                return 1;
            }
        }
        unit = unit->next;
    }
    func_0010D5F0(0);
    return 1;
}


u32 func_001FA270(void) {
    if (func_0020A418() != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001FA2B0(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x0f800000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 0x4000;
        ((BtlCommandContext *)context)->choicesB[15] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~0x4000;
    }
    return 1;
}

u32 func_001FA340(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x0fc00000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 0x8000;
        ((BtlCommandContext *)context)->choicesB[16] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~0x8000;
    }
    return 1;
}

u32 func_001FA3D0(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x0f400000)) {
        func_0010D5F0(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 0x2000;
        ((BtlCommandContext *)context)->choicesB[14] = scrReadIntParameter(0);
    } else {
        func_0010D5F0(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~0x2000;
    }
    return 1;
}

u32 func_001FA460(void) {
    s32 context;

    context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), 0x10000000) != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001FA4B0(void) {
    u64 result;

    result = func_001ACAE0();
    func_0010D5F0(result);
    return 1;
}

u32 func_001FA4D8(void) {
    s32 battle;

    battle = func_001A17F0();
    func_0010D5F0(*(u32 *)(battle + 0x250));
    return 1;
}

u32 func_001FA500(void) {
    s32 context;

    context = func_0010D6A8();
    func_0010D5F0(*(u16 *)(*(s32 *)(context + 0x18) + 0x134));
    return 1;
}

u32 func_001FA530(void) {
    u32 result;

    result = func_00208C68();
    func_0010D5F0(result);
    return 1;
}

u32 func_001FA558(void) {
    u32 result;

    result = func_002099A0();
    func_0010D5F0(result);
    return 1;
}

u32 func_001FA580(void) {
    u32 active;

    active = btlGetEffectActive();
    func_0010D5F0(active);
    return 1;
}

u32 func_001FA5A8(void) {
    s32 battle;

    battle = func_001A17F0();
    func_0010D5F0(*(u16 *)(battle + 0x25c));
    return 1;
}

u32 func_001FA5D0(void) {
    u32 value;

    value = btlGetEffectValue();
    func_0010D5F0(value);
    return 1;
}

u32 func_001FA5F8(void) {
    s32 context;

    context = func_0010D6A8();
    func_0010D5F0(*(s8 *)(context + 0x146));
    return 1;
}

u32 func_001FA620(void) {
    func_0010D5F0(D_003BB870);
    return 1;
}

u32 func_001FA648(void) {
    u32 result;

    result = func_0020A3F0();
    func_0010D5F0(result);
    return 1;
}

u32 func_001FA670(void) {
    func_001FED20(func_0010D6A8());
    return 1;
}

u32 func_001FA698(void) {
    s32 context = func_0010D6A8();
    u16 value = scrReadIntParameter(0);
    func_001FF030(context, value);
    return 1;
}

u32 func_001FA6D8(void) {
    s32 state;
    s32 context;

    context = func_0010D6A8();
    state = D_003BB3D8;
    *(u32 *)(context + 0x20) = 0x10;
    *(u32 *)(context + 0x24) = 0;
    *(u8 *)(state + 0x54) = 1;
    return 1;
}

u32 func_001FA718(void) {
    func_001C44D0();
    return 1;
}

u32 func_001FA738(void) {
    fldBeginSceneTransition();
    return 1;
}

u32 func_001FA758(void) {
    func_001F73E0();
    return 1;
}

u32 func_001FA778(void) {
    func_001F7428();
    func_001F7470(0x200);
    return 1;
}

u32 func_001FA7A0(void) {
    func_001F7428();
    func_001F7470(0x400);
    return 1;
}

extern s32 btlCreateFloatTask28(s32, f32, f32, f32, f32, f32, f32, f32, f32);
extern s32 btlScheduleContextReset(void);

u32 btlCmdCameraMove(void) {
    f32 pos[3];
    f32 target[4];
    pos[0] = bfWaitReadArgFloat(0);
    pos[1] = bfWaitReadArgFloat(1);
    pos[2] = bfWaitReadArgFloat(2);
    target[0] = bfWaitReadArgFloat(3);
    target[1] = bfWaitReadArgFloat(4);
    target[2] = bfWaitReadArgFloat(5);
    target[3] = bfWaitReadArgFloat(6);
    btlStartTask(func_001D9718());
    btlStartTask(func_001D9780());
    btlStartTask(btlCreateFloatTask28(0, pos[0], pos[1], pos[2], target[0], target[1], target[2], target[3], 40.0f));
    btlStartTask(btlScheduleContextReset());
    return 1;
}


u32 func_001FA898(void) {
    u64 task;

    task = func_001D9718();
    btlStartTask(task);
    task = func_001D9780();
    btlStartTask(task);
    task = btlCreateCommandSoundTask(func_0010D6A8(), 0x11);
    btlStartTask(task);
    return 1;
}

u32 func_001FA8F0(void) {
    u64 task;

    task = func_001D9718();
    btlStartTask(task);
    task = func_001D9780();
    btlStartTask(task);
    task = btlCreateCommandSoundTask(0, 3);
    btlStartTask(task);
    return 1;
}

extern f32 D_003D74E0[];
extern f32 D_003D7500[];

s32 func_001FA940(void) {
    D_003D74E0[0] = bfWaitReadArgFloat(0);
    D_003D74E0[1] = bfWaitReadArgFloat(1);
    D_003D74E0[2] = bfWaitReadArgFloat(2);
    D_003D7500[0] = bfWaitReadArgFloat(3);
    D_003D7500[1] = bfWaitReadArgFloat(4);
    D_003D7500[2] = bfWaitReadArgFloat(5);
    D_003D7500[3] = bfWaitReadArgFloat(6);
    return 1;
}

s32 func_001FA9C8(void) {
    D_003D74E0[4] = bfWaitReadArgFloat(0);
    D_003D74E0[5] = bfWaitReadArgFloat(1);
    D_003D74E0[6] = bfWaitReadArgFloat(2);
    D_003D7500[4] = bfWaitReadArgFloat(3);
    D_003D7500[5] = bfWaitReadArgFloat(4);
    D_003D7500[6] = bfWaitReadArgFloat(5);
    D_003D7500[7] = bfWaitReadArgFloat(6);
    return 1;
}

extern s32 btlCreateFloatTask29(s32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

u32 btlCmdCameraMoveBlend(void) {
    f32 timeA = bfWaitReadArgFloat(0);
    f32 timeB = bfWaitReadArgFloat(1);
    btlStartTask(func_001D9718());
    btlStartTask(func_001D9780());
    btlStartTask(btlCreateFloatTask29(0, D_003D74E0[0], D_003D74E0[1], D_003D74E0[2], D_003D7500[0], D_003D7500[1],
                                D_003D7500[2], D_003D7500[3], D_003D74E0[4], D_003D74E0[5], D_003D74E0[6],
                                D_003D7500[4], D_003D7500[5], D_003D7500[6], D_003D7500[7], timeA, timeB));
    btlStartTask(btlScheduleContextReset());
    return 1;
}


u32 func_001FAB38(void) {
    func_001FB0A8(D_003A5440);
    func_0010D5F0(0);
    return 1;
}

u32 func_001FAB68(void) {
    func_001FB0A8(D_003A5460);
    func_0010D5F0(0x14);
    return 1;
}

u32 func_001FAB98(void) {
    func_0010D6A8();
    return 1;
}

u32 func_001FABB8(void) {
    func_001FB0A8(D_003A5488);
    func_0010D5F0(0);
    return 1;
}

u32 func_001FABE8(void) {
    func_001FB0A8(D_003A54A8);
    return 1;
}

u32 func_001FAC10(void) {
    func_001FB0A8(D_003A54C8);
    return 1;
}

u32 func_001FAC38(void) {
    func_001FB0A8(D_003A54F0);
    return 1;
}

u32 func_001FAC60(void) {
    func_001FB0A8(D_003A5518);
    return 1;
}

u32 func_001FAC88(void) {
    func_001FB0A8(D_003A5538);
    return 1;
}

u32 func_001FACB0(void) {
    func_001FB0A8(D_003A5558);
    return 1;
}

u32 func_001FACD8(void) {
    func_00204FE0();
    return 1;
}

extern s32 D_003BAAA8;

void btlBindActorSlot(BtlActor *actor, s32 arg1) {
    BtlState *state = (BtlState *)func_001A17F0();
    s32 slot = scrCreateTaskForProcessId(state->list->count - 1, D_003BAAA8, arg1);
    s32 handle;
    scrSetCurrentActor(slot, actor);
    handle = *(s32 *)((u8 *)func_00101A70(slot) + 0xCC);
    if (handle >= 0) {
        BtlUnit *unit = actor->unit;
        func_0019C590(handle, 0, unit->mode, (unit->unk_120 & 0x20) ? 1 : 2);
    }
    func_00101A80(state->list, slot);
    state->slot = slot;
}


void func_001FADA8(void) {
}

void func_001FADB0(void) {
}

void func_001FADB8(void) {
}

u32 func_001FADC0(void) {
    D_003BB6C8 = D_003BB6C8 | 0x2000000;
    return 0;
}

u32 func_001FADD8(u32 arg0) {
    return arg0;
}

void func_001FADE0(void) {
}

void func_001FADE8(void) {
}

void func_001FADF0(void) {
}

extern void func_00221FF8(s32, s32);

void func_001FADF8(void) {
    s32 state = func_001A17F0();
    if ((*(u32 *)(state + 0x1f4) & 0x40000000) == 0) {
        s32 actor = *(s32 *)(state + 0x228);
        while (actor != 0) {
            if ((*(u32 *)(actor + 0x110) & 2) != 0) {
                s32 effect = *(s32 *)(actor + 0x320);
                if (effect != 0) {
                    func_00221FF8(effect, 0);
                }
            }
            actor = *(s32 *)(actor + 0x344);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FAE78);

void func_001FB050(void) {
    D_00360348[4] &= ~0x80;
    D_0035FFE0[4] &= ~0x80;
}

void func_001FB080(void) {
}

void func_001FB088(void) {
}

void func_001FB090(void) {
}

void func_001FB098(void) {
}

void func_001FB0A0(void) {
}

void func_001FB0A8(s32 arg0, ...) {
}

void func_001FB0F0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, ...) {
}

void func_001FB130(void) {
}

void func_001FB138(void) {
}

void func_001FB140(void) {
}

void func_001FB148(void) {
}

void func_001FB150(void) {
}

void func_001FB158(void) {
}

void func_001FB160(void) {
}

void func_001FB168(void) {
}

void func_001FB170(void) {
}

void func_001FB178(void) {
}

void func_001FB180(void) {
}

void func_001FB188(void) {
}

void func_001FB190(void) {
}

void func_001FB198(void) {
}

void func_001FB1A0(void) {
}

void func_001FB1A8(void) {
}

void func_001FB1B0(void) {
}

void func_001FB1B8(void) {
}

void func_001FB1C0(void) {
}

void func_001FB1C8(void) {
}

void func_001FB1D0(void) {
}

void func_001FB1D8(void) {
}

void func_001FB1E0(void) {
}

void func_001FB1E8(void) {
}

void func_001FB1F0(void) {
}

u8 *btlFindEntryByCommand(u8 *list, s32 command) {
    s32 i;
    u8 *entry = *(u8 **)(list + 0x14);
    for (i = 0; i < *(s32 *)(list + 0x18); i++) {
        if (*(s32 *)(entry + 8) == command) {
            return entry;
        }
        entry += 0x18;
    }
    return entry;
}


s32 btlCountNonPartOpcodes(u8 *list) {
    u32 count;
    u8 *entry;
    u32 i;
    s32 found;
    if (list == NULL) {
        return 0;
    }
    count = *(u32 *)(list + 0x18);
    if (count == 0) {
        return 0;
    }
    entry = *(u8 **)(list + 0x14);
    found = 0;
    for (i = 0; i < count; i++) {
        switch (entry[4]) {
        case 0x14:
        case 0x15:
            break;
        default:
            found++;
            break;
        }
        entry += 0x18;
    }
    return found;
}

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5440);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5460);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5488);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A54A8);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A54C8);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A54F0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5518);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5538);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5558);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5580);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5590);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A55A0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A55B0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A55C0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A55D0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A55E0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A55F0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5600);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5610);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5620);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5630);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5640);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5650);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5660);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5670);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5680);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5690);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A56A0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A56B0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A56C0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A56D0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A56E0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A56F0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5700);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5710);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5720);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5730);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5740);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5750);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5760);

s32 func_001FB2A0(s32 arg0) {
    char buf[0x70];

    if (D_003BD476 != 0) {
        func_003014F0(buf, "pfs0:/%s", arg0);
        return sceDopen(buf);
    }
    D_003BD868 = 0;
    return 0;
}

void func_001FB2F0(void) {
    if (D_003BD476 == 0) {
        return;
    }
    func_003101B8();
}


extern char *D_00360380[];
extern s32 func_00310320(void);

typedef struct BtlReader {
    u32 flags;
    u8 unk_04[0x3C];
    char name[0x40];
} BtlReader;

s32 func_001FB320(s32 unused, BtlReader *reader) {
    if (D_003BD476 != 0) {
        return func_00310320();
    }
    if ((u32)D_003BD868 >= 8) {
        return 0;
    }
    strcpy(reader->name, D_00360380[D_003BD868]);
    reader->flags &= ~0x1000;
    D_003BD868++;
    return strlen(reader->name);
}


INCLUDE_ASM(const s32, "game/code_001F6110", btlScanDirectory);

typedef struct BtlEntry {
    s32 category;
    s32 flags;
    s32 id;
    char name[0x30];
    struct BtlEntry *prev;
    struct BtlEntry *next;
} BtlEntry;

typedef struct BtlEntryList {
    s32 count;
    s32 unk_04;
    BtlEntry *head;
} BtlEntryList;

void btlDestroyEntryList(BtlEntryList *list) {
    BtlEntry *entry;

    if (list->head != NULL) {
        entry = list->head;
        do {
            BtlEntry *next = entry->next;
            func_002CFF98(entry);
            entry = next;
        } while (entry != NULL);
    }
    func_002CFF98(list->unk_04);
    func_002CFF98(list);
}

void btlAppendEntry(BtlEntryList *list, char *name, s32 category, s32 flags, s32 id) {
    BtlEntry *entry = func_002CFEB8(0x44);
    BtlEntry *tail;
    entry->category = category;
    entry->flags = flags;
    entry->id = id;
    strcpy(entry->name, name);
    if (list->head == NULL) {
        list->head = entry;
        entry->prev = NULL;
        entry->next = NULL;
    } else {
        tail = list->head;
        while (tail->next != NULL) {
            tail = tail->next;
        }
        tail->next = entry;
        entry->prev = tail;
        entry->next = NULL;
    }
    list->count++;
}


INCLUDE_ASM(const s32, "game/code_001F6110", func_001FB9A8);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FBA38);

typedef struct BtlResourcePath {
    s32 id;
    u32 variant;
    u8 pad8[4];
    char name[1];
} BtlResourcePath;

typedef struct BtlResourceSelector {
    s32 unk0;
    s32 index;
} BtlResourceSelector;

typedef struct BtlResourceDescriptor {
    u8 pad0[0x34];
    BtlResourcePath *path;
    u8 pad38[4];
    s32 handle;
    s32 ownsHandle;
    BtlResourceSelector *selector;
} BtlResourceDescriptor;

void btlDestroyResourceDescriptor(BtlResourceDescriptor *resource) {
    s32 handle = resource->handle;
    if (handle != 0 && resource->ownsHandle == 1) {
        sdfTexReleaseReferenceViaHandler(handle);
    }
    func_002CFF98(resource);
}

/* Header of the 0x38-byte resource-name record; trailing bytes hold the name. */
typedef struct BtlResourceNameRecord {
    s32 word00;     /* 0x00: initialized to 8 */
    s32 word04;     /* 0x04: initialized to 8 */
    u32 word08;     /* 0x08 */
    s32 word0C;     /* 0x0C */
    s32 nameLength; /* 0x10: length of text written at 0x21 */
    u32 word14;     /* 0x14: initialized to 9 */
    s32 word18;     /* 0x18 */
    char name[0x1C]; /* 0x1C */
} BtlResourceNameRecord;

void func_001FBF30(s32 arg0, s32 arg1, s32 arg2) {
    ((BtlResourceNameRecord *)arg0)->word00 = arg1;
    ((BtlResourceNameRecord *)arg0)->word04 = arg2;
}

u32 func_001FBF40(s32 arg0) {
    return ((BtlResourceNameRecord *)arg0)->word14;
}

u32 func_001FBF48(s32 arg0) {
    return ((BtlResourceNameRecord *)arg0)->word08;
}

s32 btlFormatSelectedResourceName(BtlResourceDescriptor *resource, char *output) {
    s32 index = resource->selector->index;
    if (index != 0) {
        func_003014F0(output, D_003BB818, index, resource->path->name);
    } else {
        func_003014F0(output, D_003BB820, resource->path->name);
    }
    return resource->path->id;
}

s32 btlTrimResourceName(BtlResourceDescriptor *resource, char *output) {
    u32 length;
    u32 i;
    func_003014F0(output, D_003BB820, resource->path->name);
    length = strlen(output);
    for (i = 0; i < length && output[i] != '.'; i++) {
    }
    if (length != i) {
        output[i] = 0;
    }
    return resource->path->id;
}


u32 btlGetResourcePathVariant(BtlResourceDescriptor *resource) {
    return resource->path->variant;
}

extern void sdfTexReleaseReferenceViaHandler(s32);
extern void func_002D0918(s32);
void btlReplaceResourceHandle(BtlResourceDescriptor *, s32);

void btlLoadAndReplaceResourceHandle(BtlResourceDescriptor *resource, s32 name) {
    u32 loaded;
    s32 handle = resource->handle;
    s32 buffer;
    if (handle != 0 && resource->ownsHandle == 1) {
        sdfTexReleaseReferenceViaHandler(handle);
        resource->handle = 0;
    }
    buffer = func_002EB028(name, &loaded, 0);
    btlReplaceResourceHandle(resource, loaded);
    func_002D0918(buffer);
}

extern s32 func_002D3288(s32);

void btlReplaceResourceHandle(BtlResourceDescriptor *resource, s32 name) {
    s32 handle = resource->handle;
    if (handle != 0 && resource->ownsHandle == 1) {
        sdfTexReleaseReferenceViaHandler(handle);
        resource->handle = 0;
    }
    resource->handle = func_002D3288(name);
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FC160);

s32 func_001FC280(s32 name) {
    s32 recordAddress;

    recordAddress = (s32)func_002CFEB8(0x38);
    ((BtlResourceNameRecord *)recordAddress)->word14 = 9;
    ((BtlResourceNameRecord *)recordAddress)->word00 = 8;
    ((BtlResourceNameRecord *)recordAddress)->word04 = 8;
    ((BtlResourceNameRecord *)recordAddress)->word08 = 0;
    ((BtlResourceNameRecord *)recordAddress)->nameLength = 0;
    ((BtlResourceNameRecord *)recordAddress)->word0C = 0;
    ((BtlResourceNameRecord *)recordAddress)->word18 = 0;
    strcpy(recordAddress + 0x1c, name);
    return recordAddress;
}

void func_001FC2E8(void) {
    func_002CFF98();
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FC300);

void func_001FC720(s32 arg0, s32 arg1, s32 arg2) {
    ((BtlResourceNameRecord *)arg0)->word00 = arg1;
    ((BtlResourceNameRecord *)arg0)->word04 = arg2;
}

u32 func_001FC730(s32 arg0) {
    return ((BtlResourceNameRecord *)arg0)->word08;
}

void func_001FC738(char *arg0, char *arg1) {
    strcpy(arg0 + 0x21, arg1);
    ((BtlResourceNameRecord *)arg0)->nameLength = strlen(arg1);
}

void func_001FC778(s32 arg0, void *arg1) {
    func_003014F0(arg1, D_003BB818, arg0 + 0x21, arg0 + 0x1c);
}

void func_001FC7A8(s32 arg0, void *arg1) {
    func_003014F0(arg1, D_003BB820, arg0 + 0x21);
}

void func_001FC7D0(s32 arg0, u32 arg1) {
    ((BtlResourceNameRecord *)arg0)->word14 = arg1;
}

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6B8);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6C8);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6CC);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6D0);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6D4);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6D8);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6E0);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6E4);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6E8);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6F0);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6F8);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB700);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB708);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB710);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB718);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB720);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB724);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB728);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB730);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB738);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB740);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB748);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB750);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB758);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB760);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB768);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB76C);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB770);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB778);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB780);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB788);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB790);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB798);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB7A0);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB7A8);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB7B0);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB7B8);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB7C0);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB7C8);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB7D0);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB7D8);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB7E0);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB7F8);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB800);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB808);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB810);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB818);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB820);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB828);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB830);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB838);

