#include "common.h"
#include "btl_state.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"

extern void btlClearUnitDefeatCandidate(s32 actor);

extern u32 btlGetIndexListCount(s32 actor);
extern s32 btlGetIndexListEntry(s32 actor, u32 index);
extern void btlFlagUnitDefeatCandidate(s32 actor);

extern s32 D_00360348[];
extern s32 D_0035FFE0[];
extern s32 D_003BB6B8;
extern s32 D_003BD854;
extern s32 sdfReadNamedResource(s32, u32 *, s32);
extern void btlCmdSimpleB(s32, u16);
extern void btlCmdSimpleA(s32, u16);
extern void btlCmdSimpleE(s32, u16);
extern void btlCmdSimpleD(s32, u16);
extern void btlCmdSimpleJ(s32, u16);
extern void btlRunWeightedAiAction(s32, u16);

extern s8 btlHistoryCounter;

extern s8 D_003A5440[];

extern s8 D_003A5460[];

extern s8 D_003A5488[];

extern u8 D_003BB818[];

extern u8 sdfPfsDebugMode;

extern void *sdfAllocSizeClassBlock(s32 size);

extern s32 sceDopen(char *);

extern s32 D_003BD868;

extern s8 D_003A54A8[];

extern void btlBossDebugPrintf(s32, ...);

extern s8 D_003A54C8[];

extern s8 D_003A54F0[];

extern s8 D_003A5518[];

extern s8 D_003A5538[];

extern s8 D_003A5558[];

extern u32 D_003BB6C8;

extern void func_003014F0();

extern u8 D_003BB820[];

extern u64 btlCreateCommandSoundUpdateTask(void);


typedef struct BtlActor {
    u8 pad00[0x18];
    BtlUnit *unit;
} BtlActor;

typedef struct BtlList {
    u8 pad00[0x20];
    s32 count;
} BtlList;


typedef struct BtlCmdCtx {
    u8 pad00[0xC];
    u32 flags; /* 0x0C: command-context flags */
    u8 pad10[8];
    BtlUnit *unit;
    u8 pad1C[4];
    s32 commandMode;
    s32 commandValue;
    u8 pad28[4];
    void *actionProbeFirst;
    void *actionProbeSecond;
    u8 pad34[4];
    u32 parameter38; /* 0x38 */
    u8 pad3C[0x50];
    u32 parameter8C; /* 0x8C */
} BtlCmdCtx;

extern f32 btlUnitGetTopY(BtlUnit *);

typedef struct BtlVec3 {
    f32 x, y, z;
} BtlVec3;

extern f32 bfWaitReadArgFloat(s32);

extern s32 btlReadCurrentUnitHp(void *);
extern s32 btlComputeSkillAdjustedMaxHp(void *);

extern void btlGetUnitWorldPos(BtlUnit *, f32 *);
extern void effMiscQuaternionToMatrixVU(void);
extern void effObjFetchInnerFirstVec(u32);

extern f32 btlTriangleNormalDotEdge(f32 *, f32 *, f32 *);

typedef union BtlVec4 {
    f32 f[4];
    u128 q;
} BtlVec4;

extern u64 btlCreateSecondaryCommandSoundTask(void);

extern u64 btlCreateCommandSoundTask(u64, u64);

extern s32 btlTrackedTaskHandles;

extern u32 btlGetEventEffectValue(void);

extern u32 btlGetEffectValue(void);

extern u32 btlGetEffectActive(void);

extern u32 btlGetSelectedBossEffectId(void);

extern u32 btlGetSpecialModeEffectValue(void);

extern u64 func_001ACAE0(void);

extern u16 scrReadIntParameter(u32);

extern s32 scrGetCurrentCommandWork(void);

extern u32 btlButtonIconTexture;

extern u32 effCloneSharedReferenceWithValue(u32, u32);

extern s32 btlGetRuntime(void);
extern s32 btlDispatchPackedActionWithScratch(s32 context, s32 actor, u32 mask);
extern void scrSetIntegerReturnValue();
extern void btlCmdSimpleC(s32, u16);
extern u8 *btlAllocTask(s32);
extern void btlCommandRecenterParty(void);

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
    object->update = btlCommandRecenterParty;
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
    VU0_LOAD_VF(vf10, unit->orientation);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, unit->bodyOffset);
    VU0_SET_VF2X(unit->scale);
    VU0_MUL_VF2X(vf10, vf10);
    VU0_APPLY_MATRIX(vf10, vf10);
    VU0_LOAD_VF(vf11, pos);
    VU0_ADD(vf10, vf10, vf11);
}


void btlUnitGetBodyPosVU(BtlUnit *unit) {
    f32 pos[4];
    btlGetUnitWorldPos(unit, pos);
    pos[2] += unit->zOffset;
    VU0_LOAD_VF(vf10, unit->orientation);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, unit->muzzleOffset);
    VU0_SET_VF2X(unit->scale);
    VU0_MUL_VF2X(vf10, vf10);
    VU0_APPLY_MATRIX(vf10, vf10);
    VU0_LOAD_VF(vf11, pos);
    VU0_ADD(vf10, vf10, vf11);
}


void btlUnitGetEffectPosVU(BtlUnit *unit) {
    f32 pos[4];
    if (!(unit->flags & 2)) {
        btlUnitGetMuzzlePosVU(unit);
        return;
    }
    effObjFetchInnerFirstVec(unit->effectObject);
    VU0_STORE_VF(vf10, pos);
    VU0_LOAD_VF(vf10, unit->orientation);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, unit->bodyOffset);
    VU0_SET_VF2X(unit->scale);
    VU0_MUL_VF2X(vf10, vf10);
    VU0_APPLY_MATRIX(vf10, vf10);
    VU0_LOAD_VF(vf11, pos);
    VU0_ADD(vf10, vf10, vf11);
}


f32 btlUnitGetLargestScaledExtent(BtlUnit *unit) {
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
    VU0_STORE_VF(vf10, pos);
    return unit->height * unit->scale * 0.5f - pos[1];
}


f32 btlUnitGetBottomY(BtlUnit *unit) {
    f32 pos[4];
    btlUnitGetMuzzlePosVU(unit);
    VU0_STORE_VF(vf10, pos);
    return -pos[1] - unit->height * unit->scale * 0.5f;
}


INCLUDE_ASM(const s32, "game/code_001F6110", func_001F66D8);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F6970);

f32 btlGetMaxUnitTop(u32 mask) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
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
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
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
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    f32 best = 0.0f;
    s32 first = 1;
    f32 pos[4];
    f32 value;
    while (unit != NULL) {
        if ((unit->flags & 1) && (unit->flags & mask)) {
            btlUnitGetMuzzlePosVU(unit);
            VU0_STORE_VF(vf10, pos);
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
    BtlState *state = (BtlState *)btlGetRuntime();
    BtlUnit *unit;
    BtlUnit *nearest;
    s32 first;
    f32 nearestDistance;
    f32 distance;
    BtlVec4 targetPos;
    btlUnitGetMuzzlePosVU(target);
    VU0_STORE_VF(vf10, &targetPos);
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
                        VU0_SCALAR_OP(targetPos.f[1], "vaddx.y vf10, vf0, vf2x");
                        VU0_LOAD_VF($vf11, &targetPos);
                        VU0_SUB(vf10, vf10, vf11);
                        VU0_LENGTH_VF10(distance);
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
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    BtlUnit *farthest = NULL;
    f32 farthestDistance = 0.0f;
    f32 distance;
    while (unit != NULL) {
        if (unit->flags & 1) {
            if (!(unit->flags & 0xC0)) {
                if (unit->flags & mask) {
                    btlUnitGetMuzzlePosVU(unit);
                    VU0_SCALAR_OP(point[1], "vaddx.y vf10, vf0, vf2x");
                    VU0_LOAD_VF(vf11, point);
                    VU0_SUB(vf10, vf10, vf11);
                    VU0_LENGTH_VF10(distance);
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


/* vu0 routine: returns the selected muzzle position in vf10. */
BtlUnit *btlSelectUnitAtExtremeX(BtlUnit *reference, s32 actor) {
    BtlVec4 position, selectedPosition;
    BtlUnit *selected = NULL;
    BtlUnit *unit;
    u32 i = 0;
    u32 count = btlGetIndexListCount(actor);

    for (; i < count; i++) {
        unit = (BtlUnit *)btlGetIndexListEntry(actor, i);
        btlUnitGetMuzzlePosVU(unit);
        if (selected == NULL) {
            selected = unit;
            VU0_STORE_VF(vf10, &selectedPosition);
        } else {
            VU0_STORE_VF(vf10, &position);
            if (reference->flags & 0x200) {
                if (position.f[0] < selectedPosition.f[0]) {
                    selected = unit;
                    PCP_COPY_VECTOR(&selectedPosition, &position);
                }
            } else {
                if (position.f[0] > selectedPosition.f[0]) {
                    selected = unit;
                    PCP_COPY_VECTOR(&selectedPosition, &position);
                }
            }
        }
    }
    VU0_LOAD_VF(vf10, &selectedPosition);
    return selected;
}

void btlFlagAllUnitsDefeatCandidate(void) {
    BtlUnit *unit;

    for (unit = ((BtlState *)btlGetRuntime())->units; unit != NULL; unit = unit->next) {
        btlFlagUnitDefeatCandidate((s32)unit);
    }
}

void btlClearAllUnitDefeatCandidates(void) {
    BtlUnit *unit;

    for (unit = ((BtlState *)btlGetRuntime())->units; unit != NULL; unit = unit->next) {
        btlClearUnitDefeatCandidate((s32)unit);
    }
}

void btlFlagMatchingUnitsDefeatCandidate(s32 mask) {
    BtlUnit *unit;

    unit = ((BtlState *)btlGetRuntime())->units;
    if (unit != NULL) {
        do {
            if (unit->flags & mask) {
                btlFlagUnitDefeatCandidate((s32)unit);
            }
            unit = unit->next;
        } while (unit != NULL);
    }
}

void btlClearMatchingUnitDefeatCandidates(s32 mask) {
    BtlUnit *unit;

    unit = ((BtlState *)btlGetRuntime())->units;
    if (unit != NULL) {
        do {
            if (unit->flags & mask) {
                btlClearUnitDefeatCandidate((s32)unit);
            }
            unit = unit->next;
        } while (unit != NULL);
    }
}

void btlFlagActorUnitsDefeatCandidate(s32 actor) {
    u32 i = 0;
    u32 count = btlGetIndexListCount(actor);
    if (count != 0) {
        do {
            btlFlagUnitDefeatCandidate(btlGetIndexListEntry(actor, i));
            i++;
        } while (i < count);
    }
}


void btlClearActorUnitDefeatCandidates(s32 actor) {
    u32 i = 0;
    u32 count = btlGetIndexListCount(actor);
    if (count != 0) {
        do {
            btlClearUnitDefeatCandidate(btlGetIndexListEntry(actor, i));
            i++;
        } while (i < count);
    }
}


extern s32 btlIsActorModeAcceptedByBattleHook(s32);
extern void btlSetUnitPosition(s32, s32);
extern void btlSetUnitRotation(s32, s32);
extern void func_001D5990(s32);
extern void func_001D5578(s32, s32, s32, f32);

void btlUpdateUnitActors(void) {
    BtlState *state = (BtlState *)btlGetRuntime();
    BtlUnit *actor = state->units;
    while (actor != NULL) {
        btlFlagUnitDefeatCandidate((s32)actor);
        btlSetUnitPosition((s32)actor, (s32)((u8 *)actor + 0x30));
        btlSetUnitRotation((s32)actor, (s32)((u8 *)actor + 0x40));
        if ((btlIsActorModeAcceptedByBattleHook((s32)actor) == 0 && actor->effectState != 0) ||
            (actor->updateFlags & 2) != 0) {
            func_001D5990((s32)actor);
            actor->effectTimerA = 0;
            actor->effectTimerB = 0;
            func_001D5578((s32)actor, actor->effectArgA, actor->effectArgB,
                          actor->effectValue);
        }
        actor->stateFlags &= ~0x8000;
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
extern void evtSetUnitNormalizedDirection(u32, s32);

void btlRefreshUnitEffects(void) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    u32 handle;
    while (unit != NULL) {
        if (unit->flags & 2) {
            if (unit->stateFlags & 0x10) {
                handle = (u32)unit->ext;
                evtSetUnitStatusFlags(handle);
                VU0_LOAD_VF(vf10, unit);
                evtSetUnitNormalizedDirection(handle, 0);
            }
        }
        unit = unit->next;
    }
}


s32 btlCountActiveUnitsWithFlags(s32 mask) {
    BtlUnit *unit;
    s32 count = 0;
    s32 flags;

    unit = ((BtlState *)btlGetRuntime())->units;
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

s32 btlAimHorizontalDirectionVU(f32 *from, f32 *to) {
    f32 delta[4];
    delta[0] = to[0] - from[0];
    delta[2] = to[2] - from[2];
    if (delta[0] != 0.0f || delta[2] != 0.0f) {
        func_002E7F20(0.0f, func_002FA1F0(delta[0], delta[2]), 0.0f);
        return 1;
    }
    VU0_LOAD_VF($vf10, &D_0035F9E0);
    return 0;
}


INCLUDE_ASM(const s32, "game/code_001F6110", func_001F7868);

/* Unit normal of the triangle (a, b, c); result in vf10 (VU register convention). */
/* vu0 routine: unit normal of triangle (a, b, c) into vf10 */
void btlTriangleNormalVU(f32 *a, f32 *b, f32 *c) {
    VU0_LOAD_VF(vf10, b);
    VU0_LOAD_VF(vf11, a);
    VU0_SUB(vf10, vf10, vf11);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf11, c);
    VU0_LOAD_VF_MEMORY(vf10, a);
    VU0_SUB(vf11, vf11, vf10);
    VU0_MOVE_VF(vf10, vf12);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_MOVE_VF(vf11, vf10);
    VU0_MOVE_VF(vf10, vf12);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
}


f32 btlTriangleNormalDotEdge(f32 *a, f32 *b, f32 *c) {
    f32 normal[4];
    f32 dot;
    btlTriangleNormalVU(a, b, c);
    VU0_STORE_VF($vf10, normal);
    VU0_LOAD_VF($vf10, a);
    VU0_LOAD_VF($vf11, c);
    VU0_SUB(vf10, vf10, vf11);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF($vf10, normal);
        VU0_DOT_XYZ(dot, vf10, vf11);
    return dot;
}

/* vu0 routine: vf10 = c + normalize(d - c) * dist */
void btlPointOffPlaneVU(f32 *a, f32 *b, f32 *c, f32 *d) {
    f32 dist = btlTriangleNormalDotEdge(a, b, c);
    VU0_LOAD_VF(vf10, d);
    VU0_LOAD_VF(vf11, c);
    VU0_SUB(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_SCALAR_OP(dist, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_LOAD_VF(vf11, c);
    VU0_ADD(vf10, vf10, vf11);
}


/* vu0 routine: vf10 = c + n * dot(n, a - c), n = unit normal of triangle (a, b, c) */
void btlProjectOnPlaneVU(f32 *a, f32 *b, f32 *c) {
    f32 normal[4];
    f32 dot;
    btlTriangleNormalVU(a, b, c);
    VU0_STORE_VF(vf10, normal);
    VU0_LOAD_VF(vf10, a);
    VU0_LOAD_VF(vf11, c);
    VU0_SUB(vf10, vf10, vf11);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, normal);
    VU0_DOT_XYZ(dot, vf10, vf11);
    VU0_SCALAR_OP(dot, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_LOAD_VF(vf11, c);
    VU0_ADD(vf10, vf10, vf11);
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
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = colorA;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_SCALAR_OP_CLOBBER(1.0f - t, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALAR_OP_R3_CLOBBER(t, "vmulx.xyzw vf11, vf11, vf2x");
    VU0_ADD(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    
    return packed;
}


/* Blend two RGBA float vectors (0..1 scale) by t and pack to 8-bit channels. */
/* vu0 routine: blend two RGBA float vectors by t, packed to RGBA8888 */
u32 btlBlendColorVec(f32 *a, f32 *b, f32 t) {
    u32 packed;
    s32 blended[4];
    VU0_LOAD_VF(vf10, a);
    VU0_LOAD_VF(vf11, b);
    VU0_SCALAR_OP(1.0f - t, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALAR_OP_R3(t, "vmulx.xyzw vf11, vf11, vf2x");
    VU0_ADD(vf10, vf10, vf11);
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

void btlScalarRangeSetStartClearEnd(s32 range, f32 start) {
    ((BtlScalarRange *)range)->start = start;
    ((BtlScalarRange *)range)->end = 0;
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F7CD8);

void btlScalarRangeInitQuadratic(s32 range, f32 start) {
    f32 zero;

    ((BtlScalarRange *)range)->zero = 0.0f;
    ((BtlScalarRange *)range)->start = start;
    zero = ((BtlScalarRange *)range)->zero;
    ((BtlScalarRange *)range)->end = start;
    ((BtlScalarRange *)range)->target = zero;
    if (start == zero) {
        return;
    }
    ((BtlScalarRange *)range)->inverseSpan = 1.0f / (start * start * 0.25f);
}

f32 func_001F7D80(BtlScalarRange *state, f32 step) {
    f32 target = state->target;
    f32 rate = state->zero;
    f32 remaining = state->end;

    remaining -= step;

    if (remaining <= 0.0f) {
        return 1.0f;
    }
    if (remaining < state->start * 0.5f) {
        rate -= state->inverseSpan * step;
    } else {
        rate += state->inverseSpan * step;
    }
    state->end = remaining;
    state->zero = rate;
    target += rate * step;
    state->target = target;
    return target;
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F7DF8);

typedef struct BtnUv {
    s32 u;
    s32 v;
} BtnUv;

typedef struct BtnSurface {
    u8 pad00[0x10];
    void (*submit)(struct BtnSurface *, void *);
} BtnSurface;

extern BtnUv D_0035FA30[];
extern BtnUv D_0035FA38[];
extern BtnUv D_0035FA40[];
extern BtnUv D_0035FA48[];
extern BtnUv D_0035FA50[];
extern BtnUv D_0035FA58[];
extern BtnUv D_0035FA60[];
extern BtnUv D_0035FA68[];
extern BtnUv D_0035FA70[];
extern BtnUv D_0035FA78[];
extern BtnUv D_0035FA80[];
extern BtnUv D_0035FA88[];
extern BtnUv D_0035FA90[];
extern BtnUv D_0035FA98[];
extern BtnUv D_0035FAA0[];
extern s32 func_0029C048();
extern void sdfQueueGouraudTexturedQuad();
extern void *sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(void *);
extern void sdfConsCreateDrawPacket();

/* Draw a button glyph using its UV pair; screen coordinates are GS fixed-point. */
void btlDrawButtonIcon(BtnSurface *surface, s32 x, s32 y, s32 topLeftColor, s32 topRightColor, s32 bottomLeftColor, s32 bottomRightColor, s32 button) {
    BtnUv *uv;
    s32 texture;
    void *packet;
    s32 xFixed;
    s32 yFixed;

    button &= 0x7FFF;
    if (button != 0) {
        switch (button) {
        case 1: uv = D_0035FA30; break;
        case 2: uv = D_0035FA38; break;
        case 4: uv = D_0035FA40; break;
        case 8: uv = D_0035FA48; break;
        case 0x10: uv = D_0035FA50; break;
        case 0x20: uv = D_0035FA58; break;
        case 0x40: uv = D_0035FA60; break;
        case 0x80: uv = D_0035FA68; break;
        case 0x100: uv = D_0035FA70; break;
        case 0x200: uv = D_0035FA78; break;
        case 0x400: uv = D_0035FA80; break;
        case 0x800: uv = D_0035FA88; break;
        case 0x1000: uv = D_0035FA90; break;
        case 0x2000: uv = D_0035FA98; break;
        case 0x4000: uv = D_0035FAA0; break;
        default: uv = 0; break;
        }
        texture = func_0029C048(surface, ((BtlState *)btlGetRuntime())->buttonTextureHandle);
        packet = sdfAllocPacketAligned(0x20);
        sdfInitPacketList(packet);
        sdfConsCreateDrawPacket(packet, texture, 0);
        xFixed = x * 0x10;
        yFixed = y * 8;
        sdfQueueGouraudTexturedQuad(packet, 0x40,
                      xFixed + 0x7000, yFixed + 0x7900, uv->u * 0x10, uv->v * 0x10, topLeftColor,
                      xFixed + 0x7200, yFixed + 0x7900, uv->u * 0x10 + 0x200, uv->v * 0x10, topRightColor,
                      xFixed + 0x7000, yFixed + 0x7A00, uv->u * 0x10, uv->v * 0x10 + 0x200, bottomLeftColor,
                      xFixed + 0x7200, yFixed + 0x7A00, uv->u * 0x10 + 0x200, uv->v * 0x10 + 0x200, bottomRightColor,
                      0xFF0000, 0);
        surface->submit(surface, packet);
    }
}

void btlOpenButtonIconResource(void) {
    btlBossDebugPrintf((s32)"btl:[%s]\n", D_003BB6B8);
    D_003BD854 = sdfReadNamedResource(D_003BB6B8, &btlButtonIconTexture, 0);
}

/* Cache and later release the loaded battle resource in battle state. */
void btlRetainButtonTexture(void) {
    BtlState *state;
    u32 resource;

    state = (BtlState *)btlGetRuntime();
    resource = effCloneSharedReferenceWithValue(btlButtonIconTexture, 0x10000);
    state->buttonTextureHandle = resource;
}

void btlReleaseButtonTexture(void) {
    BtlState *state;

    state = (BtlState *)btlGetRuntime();
    effReleaseSharedReference(state->buttonTextureHandle);
    state->buttonTextureHandle = 0;
}

/* Script command handlers set the command state at +0x20 and its
 * argument at +0x24; their numeric opcodes are still unidentified. */
u32 btlScriptSelectEmptyCommand(void) {
    BtlCmdCtx *context;

    context = (BtlCmdCtx *)scrGetCurrentCommandWork();
    context->commandMode = 1;
    context->commandValue = 0;
    return 1;
}

u32 func_001F8358(void) {
    BtlCmdCtx *context;

    context = (BtlCmdCtx *)scrGetCurrentCommandWork();
    context->commandMode = 6;
    context->commandValue = 0xc2;
    return 1;
}

u32 btlScriptSelectModeDValueOne(void) {
    BtlCmdCtx *context;

    context = (BtlCmdCtx *)scrGetCurrentCommandWork();
    context->commandValue = 1;
    context->commandMode = 0xd;
    return 1;
}

u32 btlScriptSelectModeAValueOne(void) {
    BtlCmdCtx *context;

    context = (BtlCmdCtx *)scrGetCurrentCommandWork();
    context->commandValue = 1;
    context->commandMode = 10;
    return 1;
}

typedef struct BtlActionProbe {
    u16 id;         /* 0x00 */
    u8 pad_02[2];
    void *actionProbeFirst;    /* 0x04 */
    void *actionProbeSecond;   /* 0x08 */
} BtlActionProbe;

extern s8 *datCommandSelectors;
extern s32 func_001A8A30(void *, s32);
extern s32 func_001A3CE0(void *, s32, BtlActionProbe *);

u32 btlScriptSelectActionEntry(void) {
    BtlCmdCtx *context;
    BtlActionProbe probe;
    s32 index;

    context = (BtlCmdCtx *)scrGetCurrentCommandWork();
    index = scrReadIntParameter(0);
    if (datCommandSelectors[index * 2 + 1] == 1) {
        if (func_001A8A30(context->unit, index) != 0 &&
            func_001A3CE0(context->unit, index, &probe) != 0) {
            context->actionProbeFirst = probe.actionProbeFirst;
            context->commandMode = 3;
            context->commandValue = probe.id;
            context->actionProbeSecond = probe.actionProbeSecond;
        } else {
            context->commandValue = 0;
            context->commandMode = 1;
        }
    } else {
        context->commandMode = 2;
        context->commandValue = index;
    }
    return 1;
}

u32 btlScriptSetCommandValueAndParameter(void) {
    s32 context = scrGetCurrentCommandWork();
    u16 first = scrReadIntParameter(0);
    u32 second = scrReadIntParameter(1);
    ((BtlCmdCtx *)context)->commandMode = 2;
    ((BtlCmdCtx *)context)->commandValue = first;
    ((BtlCmdCtx *)context)->parameter8C = second;
    ((BtlCmdCtx *)context)->parameter38 = second;
    return 1;
}

u32 btlCmdSetContextFlagOne(void) {
    s32 context;

    context = scrGetCurrentCommandWork();
    ((BtlCmdCtx *)context)->flags = ((BtlCmdCtx *)context)->flags | 1;
    return 1;
}

u32 btlScriptSetActorUnitParameter(void) {
    u16 value;
    s32 context;

    context = scrGetCurrentCommandWork();
    value = scrReadIntParameter(0);
    ((BtlCmdCtx *)context)->unit->unk122 = value;
    return 1;
}

u32 btlScriptSetBattleWorkParameter(void) {
    s32 battle;
    s16 value;

    battle = btlGetRuntime();
    scrGetCurrentCommandWork();
    value = scrReadIntParameter(0);
    ((BtlState *)battle)->requestMode = 4;
    ((BtlState *)battle)->requestArgument = value;
    return 1;
}

u32 btlScriptCmdWithArgA(void) {
    btlCmdWithArgA(scrGetCurrentCommandWork());
    return 1;
}

u32 btlScriptCmdSimpleB(void) {
    s32 context = scrGetCurrentCommandWork();
    u16 value = scrReadIntParameter(0);
    btlCmdSimpleB(context, value);
    return 1;
}

u32 btlScriptCmdWithArgB(void) {
    btlCmdWithArgB(scrGetCurrentCommandWork());
    return 1;
}

u32 btlScriptCmdWithArgC(void) {
    btlCmdWithArgC(scrGetCurrentCommandWork());
    return 1;
}

u32 btlScriptCmdSimpleA(void) {
    s32 context = scrGetCurrentCommandWork();
    u16 value = scrReadIntParameter(0);
    btlCmdSimpleA(context, value);
    return 1;
}

u32 btlScriptCmdWithArgE(void) {
    btlCmdWithArgE(scrGetCurrentCommandWork());
    return 1;
}

u32 btlScriptCmdWithArgD(void) {
    btlCmdWithArgD(scrGetCurrentCommandWork());
    return 1;
}

u32 btlScriptCmdSimpleC(void) {
    s32 context = scrGetCurrentCommandWork();
    u16 value = scrReadIntParameter(0);
    btlCmdSimpleC(context, value);
    return 1;
}

u32 btlScriptCmdSimpleD(void) {
    s32 context = scrGetCurrentCommandWork();
    u16 value = scrReadIntParameter(0);
    btlCmdSimpleD(context, value);
    return 1;
}

u32 btlScriptCmdSimpleE(void) {
    s32 context = scrGetCurrentCommandWork();
    u16 value = scrReadIntParameter(0);
    btlCmdSimpleE(context, value);
    return 1;
}

u32 func_001F87D0(void) {
    btlCmdWithArgA(scrGetCurrentCommandWork());
    return 1;
}

u32 btlScriptCmdSimpleF(void) {
    btlCmdSimpleF(scrGetCurrentCommandWork(), 0);
    return 1;
}

u32 btlScriptCmdSimpleG(void) {
    btlCmdSimpleG(scrGetCurrentCommandWork(), 0);
    return 1;
}

u32 btlScriptCmdSimpleH(void) {
    btlCmdSimpleH(scrGetCurrentCommandWork(), 0);
    return 1;
}

u32 btlScriptCmdWithArgF(void) {
    btlCmdWithArgF(scrGetCurrentCommandWork());
    return 1;
}

u32 btlScriptCmdSimpleJ(void) {
    s32 context = scrGetCurrentCommandWork();
    u16 value = scrReadIntParameter(0);
    btlCmdSimpleJ(context, value);
    return 1;
}

u32 btlScriptCmdSimpleI(void) {
    btlCmdSimpleI(scrGetCurrentCommandWork(), 0);
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
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x400000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 1;
        ((BtlCommandContext *)context)->choicesA[0] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~1;
    }
    return 1;
}

u32 btlCmdSelectActorActionAndStoreChoice(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x6000000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 2;
        ((BtlCommandContext *)context)->choicesA[1] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~2;
    }
    return 1;
}

u32 btlCmdStoreBossHealthCheckChoice(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x800000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 4;
        ((BtlCommandContext *)context)->choicesA[2] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~4;
    }
    return 1;
}
u32 func_001F8AD0(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x1400000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 8;
        ((BtlCommandContext *)context)->choicesA[3] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~8;
    }
    return 1;
}
u32 btlCmdStorePartyCountCheckChoice(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x1800000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x10;
        ((BtlCommandContext *)context)->choicesA[4] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x10;
    }
    return 1;
}
u32 btlCmdStoreActorActionMaskChoice(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x1C00000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x20;
        ((BtlCommandContext *)context)->choicesA[5] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x20;
    }
    return 1;
}
u32 btlCmdStoreEnemyActionMaskChoice(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x2000000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x40;
        ((BtlCommandContext *)context)->choicesA[6] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x40;
    }
    return 1;
}
u32 btlCmdStorePartyActionMaskChoice(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x2400000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x80;
        ((BtlCommandContext *)context)->choicesA[7] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x80;
    }
    return 1;
}
u32 btlCmdRememberAllLowerActionBits(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x2800000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x100;
        ((BtlCommandContext *)context)->choicesA[8] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x100;
    }
    return 1;
}
u32 btlCmdRememberLowerUnitMode(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x2C00000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x200;
        ((BtlCommandContext *)context)->choicesA[9] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x200;
    }
    return 1;
}
u32 btlCmdRememberOtherUpperUnitMode(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x3000000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x400;
        ((BtlCommandContext *)context)->choicesA[10] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x400;
    }
    return 1;
}
u32 btlCmdRememberLowerEntryCondition(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x3400000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x800;
        ((BtlCommandContext *)context)->choicesA[11] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x800;
    }
    return 1;
}
u32 btlCmdRememberUpperEntryCondition(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x3800000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x1000;
        ((BtlCommandContext *)context)->choicesA[12] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x1000;
    }
    return 1;
}

u32 func_001F9070(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, 0x4800000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x2000;
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x2000;
    }
    return 1;
}

u32 btlCmdReturnReadyWithoutTurnsCheck(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, 0x5C00000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x2000000;
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x2000000;
    }
    return 1;
}

u32 btlCmdRememberCounterLimit(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x4C00000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x4000;
        ((BtlCommandContext *)context)->choicesA[14] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x4000;
    }
    return 1;
}
u32 btlCmdStoreCounterReachedLimitChoice(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x5000000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x4000;
        ((BtlCommandContext *)context)->choicesA[14] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x4000;
    }
    return 1;
}

u32 btlCmdReturnLowHpActionReadiness(void) {
    s32 context;

    context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, 0x6c00000) != 0) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 btlCmdReturnActorEligibility(void) {
    s32 context;

    context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, 0x9400000) != 0) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 func_001F9328(void) {
    s32 context;

    context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, 0x9c00000) != 0) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 btlCmdRememberLowerElementAccess(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x8C00000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x40000;
        ((BtlCommandContext *)context)->choicesA[18] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x40000;
    }
    return 1;
}
u32 btlCmdRememberUpperElementAccess(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x9000000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x80000;
        ((BtlCommandContext *)context)->choicesA[19] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x80000;
    }
    return 1;
}

u32 btlCmdReturnPartyElementQueryCheck(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x10400000)) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 btlCmdReturnEnemyElementQueryCheck(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x10800000)) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 btlCmdRememberLowerElementExclusion(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0xA000000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x100000;
        ((BtlCommandContext *)context)->choicesA[20] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x100000;
    }
    return 1;
}
u32 btlCmdRememberUpperElementExclusion(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0xA400000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x200000;
        ((BtlCommandContext *)context)->choicesA[21] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x200000;
    }
    return 1;
}
u32 btlCmdRememberUpperActionAvailability(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0xA800000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x400000;
        ((BtlCommandContext *)context)->choicesA[22] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x400000;
    }
    return 1;
}
u32 func_001F9750(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0xAC00000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x800000;
        ((BtlCommandContext *)context)->choicesA[23] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x800000;
    }
    return 1;
}

u32 btlCmdStoreDefaultUnitActionChoice(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0xB800000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 1;
        ((BtlCommandContext *)context)->choicesB[1] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~1;
    }
    return 1;
}
u32 btlCmdStoreAlternateUnitActionChoice(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0xBC00000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 2;
        ((BtlCommandContext *)context)->choicesB[2] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~2;
    }
    return 1;
}
u32 btlCmdStorePartyEntryCheckChoice(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0xC000000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 4;
        ((BtlCommandContext *)context)->choicesB[3] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~4;
    }
    return 1;
}
u32 btlCmdStoreInversePartyEntryChoice(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0xC400000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 8;
        ((BtlCommandContext *)context)->choicesB[4] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~8;
    }
    return 1;
}
u32 btlCmdRememberUpperEntryCodeMatch(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0xC800000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 0x10;
        ((BtlCommandContext *)context)->choicesB[5] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~0x10;
    }
    return 1;
}
u32 btlCmdRememberUpperEntryCodeInverse(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0xCC00000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 0x20;
        ((BtlCommandContext *)context)->choicesB[6] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~0x20;
    }
    return 1;
}
u32 btlCmdRememberLowerEntryCodeMismatch(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0xD400000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 0x400;
        ((BtlCommandContext *)context)->choicesB[11] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~0x400;
    }
    return 1;
}
u32 btlCmdRememberUpperEntryCodeMismatch(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0xD800000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 0x800;
        ((BtlCommandContext *)context)->choicesB[12] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~0x800;
    }
    return 1;
}

u32 btlCmdReturnPartyActionPresence(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x10C00000)) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 btlCmdReturnEnemyActionPresence(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x11000000)) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 func_001F9D48(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x9800000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 0x40;
        ((BtlCommandContext *)context)->choicesB[7] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~0x40;
    }
    return 1;
}
u32 func_001F9DD8(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0xDC00000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 0x1000;
        ((BtlCommandContext *)context)->choicesB[13] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~0x1000;
    }
    return 1;
}

u32 btlCmdRememberLowerMissingStatus(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, 0xD000000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 0x80;
        ((BtlCommandContext *)context)->choicesB[8] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~0x80;
    }
    return 1;
}

u32 btlCmdReturnPartyMissingFlagCheck(void) {
    s32 context;

    context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, 0xe800000) != 0) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 btlCmdReturnPartyHeldFlagCheck(void) {
    s32 context;

    context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, 0xec00000) != 0) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 func_001F9F90(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, 0xB000000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x1000000;
        ((BtlCommandContext *)context)->choicesA[24] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x1000000;
    }
    return 1;
}

u32 btlCmdRememberContextSecondaryFlag(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, 0x0b400000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsA |= 0x04000000;
        ((BtlCommandContext *)context)->choicesB[0] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsA &= ~0x04000000;
    }
    return 1;
}

u32 btlCommandReportBossEffectOption(void) {
    if (btlHasBossEffectOption() != 0) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 btlCmdTestEffectActor(void) {
    if (btlHasEffectActor() != 0) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 btlCommandReportSpecialEnemyLink(void) {
    if (btlIsSpecialEnemyEffectLinkSatisfied() != 0) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 btlCmdCheckHpPercent(void) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    s32 side = scrReadIntParameter(0);
    s32 id = scrReadIntParameter(1);
    s32 percent = scrReadIntParameter(2);
    u32 mask = 0x200;
    if (side) {
        mask = 0x400;
    }
    while (unit != NULL) {
        if ((unit->flags & 1) && (unit->flags & mask) && !(unit->flags & 0x20) && unit->identity == id) {
            u8 *stats = (u8 *)unit + 0x120;
            s32 current = btlReadCurrentUnitHp(stats);
            s32 maximum = btlComputeSkillAdjustedMaxHp(stats);
            if (!((u32)(maximum * percent) < (u32)(current * 100))) {
                scrSetIntegerReturnValue(1);
                return 1;
            }
        }
        unit = unit->next;
    }
    scrSetIntegerReturnValue(0);
    return 1;
}


u32 btlScriptReturnFirstSpecialEnemySpecies(void) {
    if (btlHasFirstSpecialEnemySpecies() != 0) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 func_001FA2B0(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x0f800000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 0x4000;
        ((BtlCommandContext *)context)->choicesB[15] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~0x4000;
    }
    return 1;
}

u32 func_001FA340(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x0fc00000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 0x8000;
        ((BtlCommandContext *)context)->choicesB[16] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~0x8000;
    }
    return 1;
}

u32 func_001FA3D0(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | 0x0f400000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 0x2000;
        ((BtlCommandContext *)context)->choicesB[14] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~0x2000;
    }
    return 1;
}

u32 func_001FA460(void) {
    s32 context;

    context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, 0x10000000) != 0) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 btlScriptReturnBattleValue(void) {
    u64 result;

    result = func_001ACAE0();
    scrSetIntegerReturnValue(result);
    return 1;
}

u32 btlScriptReturnWorkParameter(void) {
    s32 battle;

    battle = btlGetRuntime();
    scrSetIntegerReturnValue(((BtlState *)battle)->turnCount);
    return 1;
}

u32 btlCmdReturnActorActionTime(void) {
    s32 context;

    context = scrGetCurrentCommandWork();
    scrSetIntegerReturnValue(*(u16 *)(*(s32 *)(context + 0x18) + 0x134));
    return 1;
}

u32 btlCommandReportSpecialModeValue(void) {
    u32 result;

    result = btlGetSpecialModeEffectValue();
    scrSetIntegerReturnValue(result);
    return 1;
}

u32 btlScriptReturnSelectedBossEffectId(void) {
    u32 result;

    result = btlGetSelectedBossEffectId();
    scrSetIntegerReturnValue(result);
    return 1;
}

u32 btlScriptReturnEffectActive(void) {
    u32 active;

    active = btlGetEffectActive();
    scrSetIntegerReturnValue(active);
    return 1;
}

u32 btlCmdReturnBattlePhase(void) {
    s32 battle;

    battle = btlGetRuntime();
    scrSetIntegerReturnValue(((BtlState *)battle)->phase);
    return 1;
}

u32 btlScriptReturnEffectValue(void) {
    u32 value;

    value = btlGetEffectValue();
    scrSetIntegerReturnValue(value);
    return 1;
}

u32 btlScriptReturnContextSignedByte(void) {
    s32 context;

    context = scrGetCurrentCommandWork();
    scrSetIntegerReturnValue(*(s8 *)(context + 0x146));
    return 1;
}

u32 btlScriptReturnGlobalDebugValue(void) {
    scrSetIntegerReturnValue(btlHistoryCounter);
    return 1;
}

u32 btlCommandReportEventEffectValue(void) {
    u32 result;

    result = btlGetEventEffectValue();
    scrSetIntegerReturnValue(result);
    return 1;
}

u32 btlCmdRunWeightedAiSelection(void) {
    btlRunRandomWeightedAiTableAction(scrGetCurrentCommandWork());
    return 1;
}

u32 btlScriptSetBattleCommand(void) {
    s32 context = scrGetCurrentCommandWork();
    u16 value = scrReadIntParameter(0);
    btlRunWeightedAiAction(context, value);
    return 1;
}

u32 btlResetCommandContextAndSetStateFlag(void) {
    s32 state;
    s32 context;

    context = scrGetCurrentCommandWork();
    state = btlTrackedTaskHandles;
    ((BtlCmdCtx *)context)->commandMode = 0x10;
    ((BtlCmdCtx *)context)->commandValue = 0;
    *(u8 *)(state + 0x54) = 1;
    return 1;
}

u32 btlScriptClearSceneTransition(void) {
    fldClearSceneTransition();
    return 1;
}

u32 btlScriptBeginSceneTransition(void) {
    fldBeginSceneTransition();
    return 1;
}

u32 btlScriptFlagAllUnitsDefeatCandidates(void) {
    btlFlagAllUnitsDefeatCandidate();
    return 1;
}

u32 btlScriptFlagPlayerDefeatCandidates(void) {
    btlClearAllUnitDefeatCandidates();
    btlFlagMatchingUnitsDefeatCandidate(0x200);
    return 1;
}

u32 btlScriptFlagEnemyDefeatCandidates(void) {
    btlClearAllUnitDefeatCandidates();
    btlFlagMatchingUnitsDefeatCandidate(0x400);
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
    btlStartTask(btlCreateCommandSoundUpdateTask());
    btlStartTask(btlCreateSecondaryCommandSoundTask());
    btlStartTask(btlCreateFloatTask28(0, pos[0], pos[1], pos[2], target[0], target[1], target[2], target[3], 40.0f));
    btlStartTask(btlScheduleContextReset());
    return 1;
}


u32 btlScriptQueueActorCommandSound(void) {
    u64 task;

    task = btlCreateCommandSoundUpdateTask();
    btlStartTask(task);
    task = btlCreateSecondaryCommandSoundTask();
    btlStartTask(task);
    task = btlCreateCommandSoundTask(scrGetCurrentCommandWork(), 0x11);
    btlStartTask(task);
    return 1;
}

u32 btlScriptQueueUnboundCommandSound(void) {
    u64 task;

    task = btlCreateCommandSoundUpdateTask();
    btlStartTask(task);
    task = btlCreateSecondaryCommandSoundTask();
    btlStartTask(task);
    task = btlCreateCommandSoundTask(0, 3);
    btlStartTask(task);
    return 1;
}

extern f32 D_003D74E0[];
extern f32 D_003D7500[];

s32 btlScriptSetCameraBlendStart(void) {
    D_003D74E0[0] = bfWaitReadArgFloat(0);
    D_003D74E0[1] = bfWaitReadArgFloat(1);
    D_003D74E0[2] = bfWaitReadArgFloat(2);
    D_003D7500[0] = bfWaitReadArgFloat(3);
    D_003D7500[1] = bfWaitReadArgFloat(4);
    D_003D7500[2] = bfWaitReadArgFloat(5);
    D_003D7500[3] = bfWaitReadArgFloat(6);
    return 1;
}

s32 btlScriptSetCameraBlendEnd(void) {
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
    btlStartTask(btlCreateCommandSoundUpdateTask());
    btlStartTask(btlCreateSecondaryCommandSoundTask());
    btlStartTask(btlCreateFloatTask29(0, D_003D74E0[0], D_003D74E0[1], D_003D74E0[2], D_003D7500[0], D_003D7500[1],
                                D_003D7500[2], D_003D7500[3], D_003D74E0[4], D_003D74E0[5], D_003D74E0[6],
                                D_003D7500[4], D_003D7500[5], D_003D7500[6], D_003D7500[7], timeA, timeB));
    btlStartTask(btlScheduleContextReset());
    return 1;
}


u32 btlAiCheckBa(void) {
    btlBossDebugPrintf(D_003A5440);
    scrSetIntegerReturnValue(0);
    return 1;
}

u32 btlAiGetAllKoudo(void) {
    btlBossDebugPrintf(D_003A5460);
    scrSetIntegerReturnValue(0x14);
    return 1;
}

u32 func_001FAB98(void) {
    scrGetCurrentCommandWork();
    return 1;
}

u32 btlAiGetFurea(void) {
    btlBossDebugPrintf(D_003A5488);
    scrSetIntegerReturnValue(0);
    return 1;
}

u32 btlCmdReportUnsupportedCameraOrigin(void) {
    btlBossDebugPrintf(D_003A54A8);
    return 1;
}

u32 btlCmdReportUnsupportedCameraTwoShot(void) {
    btlBossDebugPrintf(D_003A54C8);
    return 1;
}

u32 btlCmdReportUnsupportedCameraBougai(void) {
    btlBossDebugPrintf(D_003A54F0);
    return 1;
}

u32 btlCmdReportUnsupportedEvent(void) {
    btlBossDebugPrintf(D_003A5518);
    return 1;
}

u32 btlCmdReportUnsupportedTaikyo(void) {
    btlBossDebugPrintf(D_003A5538);
    return 1;
}

u32 btlCmdReportUnsupportedAllTaikyo(void) {
    btlBossDebugPrintf(D_003A5558);
    return 1;
}

u32 btlCmdClearSpecialEnemyFlags(void) {
    btlClearSpecialEnemyEntryFlags();
    return 1;
}

extern s32 D_003BAAA8;

void btlBindActorSlot(BtlActor *actor, s32 taskArg) {
    BtlState *state = (BtlState *)btlGetRuntime();
    s32 slot = scrCreateTaskForProcessId(((BtlList *)state->scriptOwner)->count - 1, D_003BAAA8, taskArg);
    s32 handle;
    scrSetCurrentActor(slot, actor);
    handle = *(s32 *)((u8 *)kwlnTaskGetUserValue(slot) + 0xCC);
    if (handle >= 0) {
        BtlUnit *unit = actor->unit;
        func_0019C590(handle, 0, unit->mode, (unit->statBits & 0x20) ? 1 : 2);
    }
    func_00101A80(state->scriptOwner, slot);
    state->boundTask = slot;
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

u32 func_001FADD8(u32 value) {
    return value;
}

void func_001FADE0(void) {
}

void func_001FADE8(void) {
}

void func_001FADF0(void) {
}

extern void evtConfigureUnitTransition(s32, s32);

void btlReleaseActiveUnitEffectsUnlessPaused(void) {
    s32 state = btlGetRuntime();
    if ((((BtlState *)state)->battleFlags & 0x40000000) == 0) {
        BtlUnit *actor = ((BtlState *)state)->units;
        while (actor != 0) {
            if ((actor->flags & 2) != 0) {
                s32 effect = (s32)actor->ext;
                if (effect != 0) {
                    evtConfigureUnitTransition(effect, 0);
                }
            }
            actor = actor->next;
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

void btlBossDebugPrintf(s32 arg0, ...) {
}

void btlBossDebugPrintfN(s32 arg0, s32 arg1, s32 arg2, s32 arg3, ...) {
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

/* Packed command entries have a 0x18-byte stride. */
typedef struct BtlCommandEntry {
    u8 pad00[4];
    u8 opcode; /* 0x04 */
    u8 pad05[3];
    s32 command; /* 0x08 */
    u8 pad0C[0xC];
} BtlCommandEntry;

typedef struct BtlCommandEntryList {
    u8 pad00[0x14];
    u8 *entries; /* 0x14 */
    s32 count;   /* 0x18 */
} BtlCommandEntryList;

u8 *btlFindEntryByCommand(BtlCommandEntryList *list, s32 command) {
    s32 i;
    u8 *entry = list->entries;
    for (i = 0; i < list->count; i++) {
        if (((BtlCommandEntry *)entry)->command == command) {
            return entry;
        }
        entry += 0x18;
    }
    return entry;
}


s32 btlCountNonPartOpcodes(BtlCommandEntryList *list) {
    u32 count;
    u8 *entry;
    u32 i;
    s32 found;
    if (list == NULL) {
        return 0;
    }
    count = list->count;
    if (count == 0) {
        return 0;
    }
    entry = list->entries;
    found = 0;
    for (i = 0; i < count; i++) {
        switch (((BtlCommandEntry *)entry)->opcode) {
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

s32 btlOpenPfsDebugDirectory(s32 directory) {
    char buf[0x70];

    if (sdfPfsDebugMode != 0) {
        func_003014F0(buf, "pfs0:/%s", directory);
        return sceDopen(buf);
    }
    D_003BD868 = 0;
    return 0;
}

void func_001FB2F0(void) {
    if (sdfPfsDebugMode == 0) {
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

s32 btlReadBattleResourceDirectoryEntry(s32 unused, BtlReader *reader) {
    if (sdfPfsDebugMode != 0) {
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
            sdfReleaseChipBlock(entry);
            entry = next;
        } while (entry != NULL);
    }
    sdfReleaseChipBlock(list->unk_04);
    sdfReleaseChipBlock(list);
}

void btlAppendEntry(BtlEntryList *list, char *name, s32 category, s32 flags, s32 id) {
    BtlEntry *entry = sdfAllocSizeClassBlock(0x44);
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
    s32 word00;             /* 0x00 */
    s32 word04;             /* 0x04 */
    u32 word08;             /* 0x08 */
    s32 entryCount;         /* 0x0C */
    u32 word10[5];          /* 0x10 */
    u32 word24;             /* 0x24 */
    u32 word28;             /* 0x28 */
    u32 word2C;             /* 0x2C */
    struct BtlEntry *head;  /* 0x30 */
    BtlResourcePath *path;  /* 0x34 */
    u32 word38;             /* 0x38 */
    s32 handle;             /* 0x3C */
    s32 ownsHandle;         /* 0x40 */
    BtlResourceSelector *selector; /* 0x44 */
} BtlResourceDescriptor;

BtlResourceDescriptor *btlCreateResourceDescriptor(BtlEntryList *list) {
    BtlResourceDescriptor *resource = sdfAllocSizeClassBlock(0x48);

    resource->word00 = 8;
    resource->word04 = 8;
    resource->word08 = 0;
    resource->entryCount = list->count;
    resource->word10[0] = 0;
    resource->word10[1] = 0;
    resource->word10[2] = 0;
    resource->word10[3] = 0;
    resource->word10[4] = 0;
    resource->word24 = 0x60;
    resource->word28 = 0x80806020;
    resource->word2C = 0x60000000;
    resource->head = list->head;
    resource->path = (BtlResourcePath *)list->head;
    resource->word38 = 0;
    resource->handle = 0;
    resource->selector = (BtlResourceSelector *)list;
    return resource;
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FBA38);

void btlDestroyResourceDescriptor(BtlResourceDescriptor *resource) {
    s32 handle = resource->handle;
    if (handle != 0 && resource->ownsHandle == 1) {
        sdfTexReleaseReferenceViaHandler(handle);
    }
    sdfReleaseChipBlock(resource);
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

void btlSetResourceNameHeaderPair(s32 recordAddress, s32 firstWord, s32 secondWord) {
    ((BtlResourceNameRecord *)recordAddress)->word00 = firstWord;
    ((BtlResourceNameRecord *)recordAddress)->word04 = secondWord;
}

u32 func_001FBF40(s32 recordAddress) {
    return ((BtlResourceNameRecord *)recordAddress)->word14;
}

u32 func_001FBF48(s32 recordAddress) {
    return ((BtlResourceNameRecord *)recordAddress)->word08;
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
extern void sdfReleaseResourceAllocation(s32);
void btlReplaceResourceHandle(BtlResourceDescriptor *, s32);

void btlLoadAndReplaceResourceHandle(BtlResourceDescriptor *resource, s32 name) {
    u32 loaded;
    s32 handle = resource->handle;
    s32 buffer;
    if (handle != 0 && resource->ownsHandle == 1) {
        sdfTexReleaseReferenceViaHandler(handle);
        resource->handle = 0;
    }
    buffer = sdfReadNamedResource(name, &loaded, 0);
    btlReplaceResourceHandle(resource, loaded);
    sdfReleaseResourceAllocation(buffer);
}

extern s32 sdfTexAcquireResourceTexture(s32);

void btlReplaceResourceHandle(BtlResourceDescriptor *resource, s32 name) {
    s32 handle = resource->handle;
    if (handle != 0 && resource->ownsHandle == 1) {
        sdfTexReleaseReferenceViaHandler(handle);
        resource->handle = 0;
    }
    resource->handle = sdfTexAcquireResourceTexture(name);
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FC160);

s32 btlCreateResourceNameRecord(s32 name) {
    s32 recordAddress;

    recordAddress = (s32)sdfAllocSizeClassBlock(0x38);
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
    sdfReleaseChipBlock();
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FC300);

void btlSetResourceNameHeaderPairAlternate(s32 recordAddress, s32 firstWord, s32 secondWord) {
    ((BtlResourceNameRecord *)recordAddress)->word00 = firstWord;
    ((BtlResourceNameRecord *)recordAddress)->word04 = secondWord;
}

u32 func_001FC730(s32 recordAddress) {
    return ((BtlResourceNameRecord *)recordAddress)->word08;
}

void btlResourceRecordSetName(char *record, char *name) {
    strcpy(record + 0x21, name);
    ((BtlResourceNameRecord *)record)->nameLength = strlen(name);
}

void btlFormatResourceNameWithPrefix(s32 recordAddress, void *output) {
    func_003014F0(output, D_003BB818, recordAddress + 0x21, recordAddress + 0x1c);
}

void btlFormatResourceNameWithoutPrefix(s32 recordAddress, void *output) {
    func_003014F0(output, D_003BB820, recordAddress + 0x21);
}

void func_001FC7D0(s32 recordAddress, u32 value) {
    ((BtlResourceNameRecord *)recordAddress)->word14 = value;
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

