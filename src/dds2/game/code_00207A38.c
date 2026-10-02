#include "common.h"
#include "btl_state.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"

typedef struct BtlActor {
    u8 pad00[0x18];
    BtlUnit *unit;
} BtlActor;

typedef struct BtlList {
    u8 pad00[0x20];
    s32 count;
} BtlList;

typedef struct BtnUv {
    s32 u;
    s32 v;
} BtnUv;

typedef struct BtnSurface {
    u8 pad00[0x10];
    void (*submit)(struct BtnSurface *, void *);
} BtnSurface;



typedef struct BtlCommandCtx {
    u8 pad00[0xC];
    u32 stateFlags;
    u8 pad10[8];
    s32 actor;
    u8 pad1C[4];
    u32 commandMode;
    u32 commandValue;
    u8 pad28[4];
    void *actionProbeFirst;  /* 0x2C: first pointer from an action probe */
    void *actionProbeSecond; /* 0x30: second pointer from an action probe */
    u8 pad34[4];
    s32 selectedValue; /* 0x38: command selection */
    u8 pad3C[0x58];
    s32 pendingValue;  /* 0x94: mirrored command selection */
    u32 selectionFlagsA;
    u32 selectionFlagsB;
    u32 choicesA[8];
    u32 choicesExtra[5]; /* 0xC0–0xD0 */
    u8 padD4[4];
    u32 choiceD8;
    u8 padDC[0xC];
    u32 savedChoices[6]; /* 0xE8–0xFC */
    u32 choice100; /* 0x100 */
    u8 pad104[4];
    u32 choice108; /* 0x108 */
    u32 choicesB[4];
    u32 choicesC[12]; /* 0x11C–0x148 */
} BtlCommandCtx;

extern s32 btlRollAiBucket(void);

extern s32 btlGetRuntime(void);

extern u32 btlButtonIconTexture;

extern u32 effCloneSharedReferenceWithValue(u32, u32);

extern s32 scrGetCurrentCommandWork(void);

extern u16 scrReadIntParameter(u32);

extern u64 func_001B76F0(void);

extern u64 func_00220958(void);

extern s32 btlTrackedTaskHandles;

extern u64 btlCreateCommandSoundUpdateTask(void);

extern u64 btlCreateSecondaryCommandSoundTask(void);

extern u64 btlCreateCommandSoundTask(u64, u64);

extern u32 D_00436B00;

extern s32 D_00436AF0;

extern s32 D_00438F6C;

extern s32 sdfReadNamedResource(s32, u32 *, s32);

extern void btlBossDebugPrintf(s32, ...);

extern void btlCmdSimpleB(s32, u16);

extern void btlCmdSimpleA(s32, u16);

extern void btlCmdSimpleD(s32, u16);

extern void btlCmdSimpleE(s32, u16);

extern void btlCmdSimpleJ(s32, u16);

extern s8 btlHistoryCounter;

extern void btlRunWeightedAiAction(s32, u16);

extern s8 D_00419570[];

extern s8 D_00419590[];

extern s8 D_004195B8[];

extern s8 D_004195D8[];

extern s8 D_004195F8[];

extern s8 D_00419620[];

extern s8 D_00419648[];

extern s8 D_00419668[];

extern s8 D_00419688[];

extern s32 D_003BEA48[];

extern s32 D_003BE6E0[];

extern u8 sdfPfsDebugMode;

extern s32 sceDopen(char *);

extern s32 D_00438F80;

extern void func_0035C860();

extern void *sdfAllocSizeClassBlock(s32 size);

extern u32 btlGetEffectActive(void);

extern u32 btlGetEffectValue(void);

extern u32 btlGetSubtaskTargetMode(void);

extern void btlCmdSimpleC(s32, u16);

extern s32 btlDispatchPackedActionWithScratch(s32 context, s32 actor, u32 mask);

extern void scrSetIntegerReturnValue();

extern u8 *btlAllocTask(s32);

extern void btlCommandRecenterParty(void);

extern void btlFlagUnitDefeatCandidate(s32 actor);

extern void btlClearUnitDefeatCandidate(s32 actor);

extern u32 btlGetIndexListCount(s32 actor);

extern s32 btlGetIndexListEntry(s32 actor, u32 index);

extern void func_0021F3E8(s32);

extern void func_00211108(s32);

extern void func_002110E8(s32, u16);

extern void btlSetSpecialBattleEffectActorByte(u8);

extern s32 btlIsLinkedActionSceneStateActive(void);

typedef struct BtlVec3 {
    f32 x, y, z;
} BtlVec3;

extern f32 bfWaitReadArgFloat(s32);

extern s32 btlCreateFloatTask28(s32, f32, f32, f32, f32, f32, f32, f32, f32);

extern s32 btlScheduleContextReset(void);

extern f32 D_00452F90[];

extern f32 D_00452FB0[];

extern s32 btlCreateFloatTask29(s32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

extern void evtConfigureUnitTransition(s32, s32);

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
    s32 word00;             // 0x00
    s32 word04;             // 0x04
    u32 word08;             // 0x08
    s32 entryCount;         // 0x0C
    u32 word10[5];          // 0x10
    u32 word24;             // 0x24
    u32 word28;             // 0x28
    u32 word2C;             // 0x2C
    struct BtlEntry *head;  // 0x30
    BtlResourcePath *path;  // 0x34
    u32 word38;             // 0x38
    s32 handle;             // 0x3C
    s32 ownsHandle;         // 0x40
    BtlResourceSelector *selector; // 0x44
} BtlResourceDescriptor;

extern u8 D_00436C50[];

extern u8 D_00436C58[];

extern void sdfTexReleaseReferenceViaHandler(s32);

extern void sdfReleaseResourceAllocation(s32);

void btlReplaceResourceHandle(BtlResourceDescriptor *, s32);

extern s32 sdfTexAcquireResourceTexture(s32);

extern char *D_003BEA80[];

extern s32 func_0036B588(void);

typedef struct BtlReader {
    u32 flags;
    u8 unk_04[0x3C];
    char name[0x40];
} BtlReader;

extern f32 btlTriangleNormalDotEdge(f32 *, f32 *, f32 *);

extern void btlGetUnitWorldPos(BtlUnit *, f32 *);

extern void effMiscQuaternionToMatrixVU(void);

extern void effObjFetchInnerFirstVec(u32);

extern f32 btlUnitGetTopY(BtlUnit *);

typedef union BtlVec4 {
    f32 f[4];
    u128 q;
} BtlVec4;

extern s32 btlReadCurrentUnitHp(void *);

extern s32 btlComputeSkillAdjustedMaxHp(void *);

extern s32 D_00435E7C;

typedef struct BattleScriptTask {
    u8 active;
    u8 pad01[0xF];
    u8 startFlag;
    u8 pad11[0xF];
    u16 kind;
    u8 pad22[0x26];
    s32 work;
    void (*callback)(void);
} BattleScriptTask;

BattleScriptTask *btlCreateControlObject(void) {
    BattleScriptTask *task;
    task = (BattleScriptTask *)btlAllocTask(0);
    task->active = 1;
    task->callback = btlCommandRecenterParty;
    task->kind = 0x66;
    task->work = 0;
    task->startFlag = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00207A80);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00207C28);

void btlUnitGetMuzzlePosVU(BtlUnit *unit) {
    f32 pos[4];
    btlGetUnitWorldPos(unit, pos);
    pos[2] += unit->positionZOffset;
    VU0_LOAD_VF(vf10, unit->orientation);;
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, unit->bodyOffset);;
    VU0_SET_VF2X(unit->scale);
    VU0_MUL_VF2X(vf10, vf10);
    VU0_APPLY_MATRIX(vf10, vf10);
    VU0_LOAD_VF(vf11, pos);
    VU0_ADD(vf10, vf10, vf11);
}

void btlUnitGetBodyPosVU(BtlUnit *unit) {
    f32 pos[4];
    btlGetUnitWorldPos(unit, pos);
    pos[2] += unit->positionZOffset;
    VU0_LOAD_VF(vf10, unit->orientation);;
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, unit->muzzleOffset);;
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
    f32 second;
    f32 first;

    second = unit->reach;
    first = unit->height;
    if (first < second) {
        return second * unit->scale;
    }
    return first * unit->scale;
}

f32 btlUnitGetTopY(BtlUnit *unit) {
    f32 pos[4];
    btlUnitGetMuzzlePosVU(unit);
    VU0_STORE_VF(vf10, pos);;
    return unit->height * unit->scale * 0.5f - pos[1];
}

f32 btlUnitGetBottomY(BtlUnit *unit) {
    f32 pos[4];
    btlUnitGetMuzzlePosVU(unit);
    VU0_STORE_VF(vf10, pos);;
    return -pos[1] - unit->height * unit->scale * 0.5f;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00208000);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00208298);

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
        unit = unit->nextActor;
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
        unit = unit->nextActor;
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
            VU0_STORE_VF(vf10, pos);;
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
        unit = unit->nextActor;
    }
    return best;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00208750);

BtlUnit *btlFindNearestUnit(u32 mask, BtlUnit *target) {
    BtlState *state = (BtlState *)btlGetRuntime();
    BtlUnit *unit;
    BtlUnit *nearest;
    s32 first;
    f32 best;
    f32 dist;
    BtlVec4 pos;
    btlUnitGetMuzzlePosVU(target);
    VU0_STORE_VF(vf10, &pos);;
    unit = state->units;
    nearest = NULL;
    first = 1;
    best = 0.0f;
    while (unit != NULL) {
        if (unit->flags & 1) {
            if (!(unit->flags & 0xC0)) {
                if (target != unit) {
                    if (unit->flags & mask) {
                        btlUnitGetMuzzlePosVU(unit);
                                        VU0_SCALAR_OP_CLOBBER(pos.f[1], "vaddx.y vf10, vf0, vf2x");
                        VU0_LOAD_VF(vf11, &pos);;
                        VU0_SUB(vf10, vf10, vf11);
                        VU0_LENGTH_VF10(dist);
                        if (first) {
                            best = dist;
                            nearest = unit;
                            first = 0;
                        } else if (dist < best) {
                            best = dist;
                            nearest = unit;
                        }
                    }
                }
            }
        }
        unit = unit->nextActor;
    }
    return nearest;
}

BtlUnit *btlFindFarthestUnit(u32 mask, f32 *point) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    BtlUnit *farthest = NULL;
    f32 best = 0.0f;
    f32 dist;
    while (unit != NULL) {
        if (unit->flags & 1) {
            if (!(unit->flags & 0xC0)) {
                if (unit->flags & mask) {
                    btlUnitGetMuzzlePosVU(unit);
                    VU0_SCALAR_OP(point[1], "vaddx.y vf10, vf0, vf2x");
                    VU0_LOAD_VF(vf11, point);
                    VU0_SUB(vf10, vf10, vf11);
                    VU0_LENGTH_VF10(dist);
                    if (best < dist) {
                        best = dist;
                        farthest = unit;
                    }
                }
            }
        }
        unit = unit->nextActor;
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
        PCP_COPY_VECTOR(&position, &unit->positionX);
        position.f[2] += unit->positionZOffset;
        /* The indexed DDS2 path uses the vector at 0x40 and scale at 0x50,
         * unlike the ordinary muzzle accessor's 0x70/0x80 fields. */
        VU0_LOAD_VF(vf10, (u8 *)unit + 0x40);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, unit->bodyOffset);
        VU0_SET_VF2X(unit->unk50);
        VU0_MUL_VF2X(vf10, vf10);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_LOAD_VF(vf11, &position);
        VU0_ADD(vf10, vf10, vf11);
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
    BtlState *work = (BtlState *)btlGetRuntime();
    for (unit = work->units; unit != NULL; unit = unit->nextActor) {
        btlFlagUnitDefeatCandidate((s32)unit);
    }
}

void btlClearAllUnitDefeatCandidates(void) {
    BtlUnit *unit;
    BtlState *work = (BtlState *)btlGetRuntime();
    for (unit = work->units; unit != NULL; unit = unit->nextActor) {
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
            unit = unit->nextActor;
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
            unit = unit->nextActor;
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

extern void func_001E22D8(u8 *, s32, s32, f32);

void btlUpdateUnitActors(void) {
    BtlState *state = (BtlState *)btlGetRuntime();
    BtlUnit *actor = state->units;

    while (actor != NULL) {
        btlFlagUnitDefeatCandidate((s32)actor);
        btlSetUnitPosition((s32)actor, (s32)((u8 *)actor + 0x30));
        btlSetUnitRotation((s32)actor, (s32)((u8 *)actor + 0x40));
        if ((btlIsActorModeAcceptedByBattleHook((s32)actor) == 0 && actor->effectState != 0) ||
            (actor->updateFlags & 2) != 0) {
            func_001E2758((s32)actor);
            actor->unkF8 = 0;
            actor->unkFA = 0;
            func_001E22D8((u8 *)actor, actor->effectIndex, actor->effectParameter, actor->effectScale);
        }
        actor->stateFlags &= ~0x8000;
        actor->stateFlags &= ~0x200000;
        actor = actor->nextActor;
    }
    {
        void (*callback)(void) = *(void (**)(void))((u8 *)state + 0x624);
        if (callback != 0) {
            callback();
        }
    }
}
INCLUDE_ASM(const s32, "game/code_00207A38", func_00209078);


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
            unit = unit->nextActor;
        } while (unit != NULL);
    }
    return count;
}

extern void func_00340DC8(f32, f32, f32);
extern f32 func_003532E8(f32, f32);
extern u128 D_003BE0D0;

s32 btlAimHorizontalDirectionVU(f32 *from, f32 *to) {
    f32 delta[4];
    delta[0] = to[0] - from[0];
    delta[2] = to[2] - from[2];
    if (delta[0] != 0.0f || delta[2] != 0.0f) {
        func_00340DC8(0.0f, func_003532E8(delta[0], delta[2]), 0.0f);
        return 1;
    }
    VU0_LOAD_VF($vf10, &D_003BE0D0);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00209258);

/* vu0 routine: unit normal of the triangle (a, b, c); result in vf10 (VU register convention) */
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
    VU0_STORE_VF(vf10, normal);;
    VU0_LOAD_VF(vf10, a);;
    VU0_LOAD_VF(vf11, c);;
    VU0_SUB(vf10, vf10, vf11);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, normal);;
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
    VU0_STORE_VF(vf10, normal);;
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
    VU0_MOVE_VF(vf11, vf10);;
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

INCLUDE_ASM(const s32, "game/code_00207A38", func_002096C8);

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

INCLUDE_ASM(const s32, "game/code_00207A38", func_00209770);

INCLUDE_ASM(const s32, "game/code_00207A38", btlDrawIconAtSize);

extern void btlDrawIconAtSize(BtnSurface *, s32, s32, s32, s32, s32, s32, s32, s32, u16);

void btlDrawButtonIconFixed64(BtnSurface *surface, s32 x, s32 y, s32 c0, s32 c1, s32 c2, s32 c3, s32 index) {
    btlDrawIconAtSize(surface, x, y, 0x40, 0x40, c0, c1, c2, c3, index);
}

extern BtnUv D_003BE130[];
extern BtnUv D_003BE138[];
extern BtnUv D_003BE140[];
extern BtnUv D_003BE148[];
extern BtnUv D_003BE150[];
extern BtnUv D_003BE158[];
extern BtnUv D_003BE160[];
extern BtnUv D_003BE168[];
extern BtnUv D_003BE170[];
extern BtnUv D_003BE178[];
extern BtnUv D_003BE180[];
extern BtnUv D_003BE188[];
extern BtnUv D_003BE190[];
extern BtnUv D_003BE198[];
extern BtnUv D_003BE1A0[];
extern s32 func_002DDD60();
extern void sdfQueueGouraudTexturedQuad();
extern void *sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(void *);

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
        case 1: uv = D_003BE130; break;
        case 2: uv = D_003BE138; break;
        case 4: uv = D_003BE140; break;
        case 8: uv = D_003BE148; break;
        case 0x10: uv = D_003BE150; break;
        case 0x20: uv = D_003BE158; break;
        case 0x40: uv = D_003BE160; break;
        case 0x80: uv = D_003BE168; break;
        case 0x100: uv = D_003BE170; break;
        case 0x200: uv = D_003BE178; break;
        case 0x400: uv = D_003BE180; break;
        case 0x800: uv = D_003BE188; break;
        case 0x1000: uv = D_003BE190; break;
        case 0x2000: uv = D_003BE198; break;
        case 0x4000: uv = D_003BE1A0; break;
        default: uv = 0; break;
        }
        texture = func_002DDD60(surface, ((BtlState *)btlGetRuntime())->buttonTextureHandle);
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
    btlBossDebugPrintf((s32)"btl:[%s]\n", D_00436AF0);
    D_00438F6C = sdfReadNamedResource(D_00436AF0, &btlButtonIconTexture, 0);
}

void btlRetainButtonTexture(void) {
    BtlState *state;
    u32 handle;

    state = (BtlState *)btlGetRuntime();
    handle = effCloneSharedReferenceWithValue(btlButtonIconTexture, 0x10000);
    state->buttonTextureHandle = handle;
}

void btlReleaseButtonTexture(void) {
    BtlState *state;

    state = (BtlState *)btlGetRuntime();
    effReleaseSharedReference(state->buttonTextureHandle);
    state->buttonTextureHandle = 0;
}

u32 btlScriptSelectEmptyCommand(void) {
    BtlCommandCtx *context;

    context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    context->commandMode = 1;
    context->commandValue = 0;
    return 1;
}

u32 func_00209DA8(void) {
    BtlCommandCtx *context;

    context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    context->commandMode = 6;
    context->commandValue = 0xc2;
    return 1;
}

u32 btlScriptSelectModeDValueOne(void) {
    BtlCommandCtx *context;

    context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    context->commandValue = 1;
    context->commandMode = 0xd;
    return 1;
}

u32 btlCmdSetContextFlagOne(void) {
    BtlCommandCtx *context;

    context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    context->commandValue = 1;
    context->commandMode = 0x12;
    return 1;
}

u32 btlScriptSelectModeAValueOne(void) {
    BtlCommandCtx *context;

    context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    context->commandValue = 1;
    context->commandMode = 10;
    return 1;
}

typedef struct BtlActionProbe {
    u16 id;         // 0x00
    u8 pad_02[2];
    void *actionProbeFirst;    // 0x04
    void *actionProbeSecond;   // 0x08
} BtlActionProbe;

extern s8 *datCommandSelectors;
extern s32 func_001B2F50(void *, s32);
extern s32 func_001ACD10(void *, s32, BtlActionProbe *);

u32 btlScriptSelectActionEntry(void) {
    BtlCommandCtx *context;
    BtlActionProbe probe;
    s32 index;

    context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    index = scrReadIntParameter(0);
    if (datCommandSelectors[index * 2 + 1] == 1) {
        if (func_001B2F50((void *)context->actor, index) != 0 &&
            func_001ACD10((void *)context->actor, index, &probe) != 0) {
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

u32 btlScriptSelectByKind(void) {
    BtlCommandCtx *context;
    s32 kind;
    s32 value;

    context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    kind = scrReadIntParameter(0);
    value = scrReadIntParameter(1);
    context->commandMode = 2;
    context->commandValue = kind;
    if (kind == 0x196) {
        value = func_0021F698();
    }
    context->pendingValue = value;
    context->selectedValue = value;
    return 1;
}

u32 btlScriptSelectWeightedEntry(void) {
    BtlCommandCtx *context;
    s32 value;

    context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    value = scrReadIntParameter(0);
    context->commandMode = 5;
    context->commandValue = 0;
    value = btlPickWeightedEntry((u16)value);
    context->pendingValue = value;
    context->selectedValue = value;
    return 1;
}

u32 btlScriptSelectDirect(void) {
    BtlCommandCtx *context;
    s32 value;

    context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    value = scrReadIntParameter(0);
    context->commandMode = 5;
    context->commandValue = 0;
    context->pendingValue = value;
    context->selectedValue = value;
    return 1;
}

s32 func_0020A048(void) {
    func_0021F3E8(scrGetCurrentCommandWork());
    return 1;
}

u32 btlEnableCommandStateFlag(void) {
    BtlCommandCtx *context;

    context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    context->stateFlags = context->stateFlags | 1;
    return 1;
}

u32 btlScriptSetActorUnitParameter(void) {
    u16 value;
    s32 context;

    context = scrGetCurrentCommandWork();
    value = scrReadIntParameter(0);
    ((BtlUnit *)((BtlCommandCtx *)context)->actor)->unk122 = value;
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

u32 func_0020A338(void) {
    btlCmdWithArgA(scrGetCurrentCommandWork());
    return 1;
}

u32 btlScriptCmdSimpleG(void) {
    btlCmdSimpleG(scrGetCurrentCommandWork(), 0);
    return 1;
}

u32 btlScriptCmdSimpleF(void) {
    func_00211228(scrGetCurrentCommandWork(), 0);
    return 1;
}

u32 btlScriptCmdSimpleH(void) {
    btlCmdSimpleH(scrGetCurrentCommandWork(), 0);
    return 1;
}

s32 func_0020A3F0(void) {
    s32 context = scrGetCurrentCommandWork();
    func_002110E8(context, scrReadIntParameter(0));
    return 1;
}

u32 btlScriptCmdWithArgF(void) {
    btlCmdWithArgF(scrGetCurrentCommandWork());
    return 1;
}

s32 func_0020A458(void) {
    func_00211108(scrGetCurrentCommandWork());
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

s32 func_0020A4F0(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0x400000)) {
        scrSetIntegerReturnValue(1);
        context->choicesA[0] = choice;
        context->selectionFlagsA |= 1;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsA &= ~1U;
    }
    return 1;
}

s32 func_0020A580(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0x7000000)) {
        scrSetIntegerReturnValue(1);
        context->choicesA[1] = choice;
        context->selectionFlagsA |= 2;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsA &= ~2U;
    }
    return 1;
}

s32 func_0020A610(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0x7000000)) {
        scrSetIntegerReturnValue(1);
        context->choicesA[1] = choice;
        context->selectionFlagsA |= 2;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsA &= ~2U;
    }
    return 1;
}

s32 func_0020A6A0(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0x800000)) {
        scrSetIntegerReturnValue(1);
        context->choicesA[2] = choice;
        context->selectionFlagsA |= 4;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsA &= ~4U;
    }
    return 1;
}

s32 btlCmdSelectChoiceSlot2Code0C(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0xc00000)) {
        scrSetIntegerReturnValue(1);
        context->choicesA[2] = choice;
        context->selectionFlagsA |= 4;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsA &= ~4U;
    }
    return 1;
}

s32 func_0020A7C0(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0x1000000)) {
        scrSetIntegerReturnValue(1);
        context->choicesA[2] = choice;
        context->selectionFlagsA |= 4;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsA &= ~4U;
    }
    return 1;
}

s32 btlCmdSelectChoiceSlot3Code1C(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0x1c00000)) {
        scrSetIntegerReturnValue(1);
        context->choicesA[3] = choice;
        context->selectionFlagsA |= 8;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsA &= ~8U;
    }
    return 1;
}

s32 func_0020A8E0(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0x2000000)) {
        scrSetIntegerReturnValue(1);
        context->choicesA[4] = choice;
        context->selectionFlagsA |= 0x10;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsA &= ~0x10U;
    }
    return 1;
}

s32 func_0020A970(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0x2400000)) {
        scrSetIntegerReturnValue(1);
        context->choicesA[5] = choice;
        context->selectionFlagsA |= 0x20;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsA &= ~0x20U;
    }
    return 1;
}

s32 func_0020AA00(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0x2800000)) {
        scrSetIntegerReturnValue(1);
        context->choicesA[6] = choice;
        context->selectionFlagsA |= 0x40;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsA &= ~0x40U;
    }
    return 1;
}

u32 btlScriptSetChoiceFlag(void) {
    BtlCommandCtx *context;
    s32 first;
    s32 second;

    context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    first = scrReadIntParameter(0);
    second = scrReadIntParameter(1);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, ((first & 0x3F) << 16) | (u16)second | 0x2C00000)) {
        scrSetIntegerReturnValue(1);
        context->choicesA[6] = first;
        context->selectionFlagsA |= 0x40;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsA &= ~0x40;
    }
    return 1;
}

s32 func_0020AB38(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0x3000000)) {
        scrSetIntegerReturnValue(1);
        context->choicesA[7] = choice;
        context->selectionFlagsA |= 0x80;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsA &= ~0x80U;
    }
    return 1;
}

s32 btlCmdRememberAllLowerActionBits(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0x3400000)) {
        scrSetIntegerReturnValue(1);
        context->choicesExtra[0] = choice;
        context->selectionFlagsA |= 0x100;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsA &= ~0x100U;
    }
    return 1;
}

s32 btlCmdRememberLowerUnitMode(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0x3800000)) {
        scrSetIntegerReturnValue(1);
        context->choicesExtra[1] = choice;
        context->selectionFlagsA |= 0x200;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsA &= ~0x200U;
    }
    return 1;
}

s32 btlCmdRememberOtherUpperUnitMode(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0x3C00000)) {
        scrSetIntegerReturnValue(1);
        context->choicesExtra[2] = choice;
        context->selectionFlagsA |= 0x400;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsA &= ~0x400U;
    }
    return 1;
}

s32 btlCmdRememberLowerEntryCondition(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0x4000000)) {
        scrSetIntegerReturnValue(1);
        context->choicesExtra[3] = choice;
        context->selectionFlagsA |= 0x800;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsA &= ~0x800U;
    }
    return 1;
}

s32 btlCmdRememberUpperEntryCondition(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0x4400000)) {
        scrSetIntegerReturnValue(1);
        context->choicesExtra[4] = choice;
        context->selectionFlagsA |= 0x1000;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsA &= ~0x1000U;
    }
    return 1;
}

u32 func_0020AE98(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandCtx *)context)->actor, 0x5400000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandCtx *)context)->selectionFlagsA |= 0x2000;
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandCtx *)context)->selectionFlagsA &= ~0x2000;
    }
    return 1;
}

u32 func_0020AF10(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandCtx *)context)->actor, 0x6C00000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandCtx *)context)->selectionFlagsA |= 0x2000000;
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandCtx *)context)->selectionFlagsA &= ~0x2000000;
    }
    return 1;
}

s32 btlCmdRememberCounterLimit(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0x5800000)) {
        scrSetIntegerReturnValue(1);
        context->choiceD8 = choice;
        context->selectionFlagsA |= 0x4000;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsA &= ~0x4000U;
    }
    return 1;
}

s32 func_0020B020(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0x5C00000)) {
        scrSetIntegerReturnValue(1);
        context->choiceD8 = choice;
        context->selectionFlagsA |= 0x4000;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsA &= ~0x4000U;
    }
    return 1;
}

u32 func_0020B0B0(void) {
    s32 context;

    context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandCtx *)context)->actor, 0x8800000) != 0) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 func_0020B100(void) {
    s32 context;

    context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandCtx *)context)->actor, 0xB000000) != 0) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 func_0020B150(void) {
    s32 context;

    context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandCtx *)context)->actor, 0xB800000) != 0) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

s32 btlCmdRememberLowerElementAccess(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0x0A800000)) {
        scrSetIntegerReturnValue(1);
        context->savedChoices[0] = choice;
        context->selectionFlagsA |= 0x40000;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsA &= ~0x40000U;
    }
    return 1;
}

s32 btlCmdRememberUpperElementAccess(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0x0AC00000)) {
        scrSetIntegerReturnValue(1);
        context->savedChoices[1] = choice;
        context->selectionFlagsA |= 0x80000;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsA &= ~0x80000U;
    }
    return 1;
}

u32 func_0020B2E0(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandCtx *)context)->actor, scrReadIntParameter(0) | 0x12C00000)) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 func_0020B350(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandCtx *)context)->actor, scrReadIntParameter(0) | 0x13000000)) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

s32 btlCmdRememberLowerElementExclusion(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0x0BC00000)) {
        scrSetIntegerReturnValue(1);
        context->savedChoices[2] = choice;
        context->selectionFlagsA |= 0x100000;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsA &= ~0x100000U;
    }
    return 1;
}

s32 btlCmdRememberUpperElementExclusion(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0x0C000000)) {
        scrSetIntegerReturnValue(1);
        context->savedChoices[3] = choice;
        context->selectionFlagsA |= 0x200000;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsA &= ~0x200000U;
    }
    return 1;
}

s32 btlCmdRememberUpperActionAvailability(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0x0C400000)) {
        scrSetIntegerReturnValue(1);
        context->savedChoices[4] = choice;
        context->selectionFlagsA |= 0x400000;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsA &= ~0x400000U;
    }
    return 1;
}

s32 btlCmdRememberUpperActorTripleCheck(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0x0C800000)) {
        scrSetIntegerReturnValue(1);
        context->savedChoices[4] = choice;
        context->selectionFlagsA |= 0x400000;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsA &= ~0x400000U;
    }
    return 1;
}

s32 func_0020B640(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0x0CC00000)) {
        scrSetIntegerReturnValue(1);
        context->savedChoices[5] = choice;
        context->selectionFlagsA |= 0x800000;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsA &= ~0x800000U;
    }
    return 1;
}

s32 func_0020B6E0(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0xd800000)) {
        scrSetIntegerReturnValue(1);
        context->choicesB[0] = choice;
        context->selectionFlagsB |= 1;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsB &= ~1U;
    }
    return 1;
}

s32 func_0020B770(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0xdc00000)) {
        scrSetIntegerReturnValue(1);
        context->choicesB[1] = choice;
        context->selectionFlagsB |= 2;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsB &= ~2U;
    }
    return 1;
}

s32 func_0020B800(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0xe000000)) {
        scrSetIntegerReturnValue(1);
        context->choicesB[2] = choice;
        context->selectionFlagsB |= 4;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsB &= ~4U;
    }
    return 1;
}

s32 func_0020B890(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0xe400000)) {
        scrSetIntegerReturnValue(1);
        context->choicesB[3] = choice;
        context->selectionFlagsB |= 8;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsB &= ~8U;
    }
    return 1;
}

s32 btlCmdRememberUpperEntryCodeMatch(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0xE800000)) {
        scrSetIntegerReturnValue(1);
        context->choicesC[0] = choice;
        context->selectionFlagsB |= 0x10;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsB &= ~0x10U;
    }
    return 1;
}

s32 btlCmdRememberUpperEntryCodeInverse(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0xEC00000)) {
        scrSetIntegerReturnValue(1);
        context->choicesC[1] = choice;
        context->selectionFlagsB |= 0x20;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsB &= ~0x20U;
    }
    return 1;
}

s32 btlCmdRememberLowerEntryCodeMismatch(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0xF400000)) {
        scrSetIntegerReturnValue(1);
        context->choicesC[6] = choice;
        context->selectionFlagsB |= 0x400;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsB &= ~0x400U;
    }
    return 1;
}

s32 btlCmdRememberUpperEntryCodeMismatch(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | 0xF800000)) {
        scrSetIntegerReturnValue(1);
        context->choicesC[7] = choice;
        context->selectionFlagsB |= 0x800;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsB &= ~0x800U;
    }
    return 1;
}

u32 func_0020BB60(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandCtx *)context)->actor, scrReadIntParameter(0) | 0x13400000)) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

s32 func_0020BBD0(void) {
    u8 *context = (u8 *)scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch((s32)context, ((BtlCommandCtx *)context)->actor, scrReadIntParameter(0) | 0x13800000)) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

s32 btlCmdRememberUpperSelectedElement(void) {
    u8 *context = (u8 *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, ((BtlCommandCtx *)context)->actor, choice | 0xB400000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandCtx *)context)->choicesC[2] = choice;
        ((BtlCommandCtx *)context)->selectionFlagsB |= 0x40;
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandCtx *)context)->selectionFlagsB &= ~0x40U;
    }
    return 1;
}

s32 btlCmdRememberLowerLeadingElement(void) {
    u8 *context = (u8 *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, ((BtlCommandCtx *)context)->actor, choice | 0xFC00000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandCtx *)context)->choicesC[8] = choice;
        ((BtlCommandCtx *)context)->selectionFlagsB |= 0x1000;
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandCtx *)context)->selectionFlagsB &= ~0x1000U;
    }
    return 1;
}

s32 btlCmdRememberLowerMissingStatus(void) {
    u8 *context = (u8 *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, ((BtlCommandCtx *)context)->actor, 0xF000000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandCtx *)context)->choicesC[3] = choice;
        ((BtlCommandCtx *)context)->selectionFlagsB |= 0x80;
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandCtx *)context)->selectionFlagsB &= ~0x80U;
    }
    return 1;
}

u32 func_0020BDE8(void) {
    s32 context;

    context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandCtx *)context)->actor, 0x10800000) != 0) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 func_0020BE38(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandCtx *)context)->actor, scrReadIntParameter(0) | 0x10C00000)) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 func_0020BEA8(void) {
    s32 context;

    context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandCtx *)context)->actor, 0x11000000) != 0) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 func_0020BEF8(void) {
    s32 context;

    context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandCtx *)context)->actor, 0x11400000) != 0) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

s32 btlCmdRememberNonLowerStatFlag(void) {
    u8 *context = (u8 *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, ((BtlCommandCtx *)context)->actor, 0xD000000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandCtx *)context)->choice100 = choice;
        ((BtlCommandCtx *)context)->selectionFlagsA |= 0x1000000;
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandCtx *)context)->selectionFlagsA &= ~0x1000000U;
    }
    return 1;
}

s32 btlCmdRememberContextSecondaryFlag(void) {
    u8 *context = (u8 *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, ((BtlCommandCtx *)context)->actor, 0xD400000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandCtx *)context)->choice108 = choice;
        ((BtlCommandCtx *)context)->selectionFlagsA |= 0x4000000;
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandCtx *)context)->selectionFlagsA &= ~0x4000000U;
    }
    return 1;
}

u32 func_0020C078(void) {
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

u32 btlScriptReturnActiveSubtaskPresence(void) {
    if (btlHasActiveSubtask() != 0) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 btlScriptReturnMarkedActionSceneActive(void) {
    if (btlIsMarkedActionSceneStateActive() != 0) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 btlScriptReturnUnitHpRatioPercent(void) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    s32 side = scrReadIntParameter(0);
    s32 id = scrReadIntParameter(1);
    s32 percent = scrReadIntParameter(2);
    u32 mask = 0x200;
    if (side) {
        mask = 0x400;
    }
    while (unit != NULL) {
        if ((unit->flags & 1) && (unit->flags & mask) && !(unit->flags & 0x20) && unit->owner == id) {
            u8 *stats = (u8 *)unit + 0x120;
            s32 current = btlReadCurrentUnitHp(stats);
            s32 maximum = btlComputeSkillAdjustedMaxHp(stats);
            if (!((u32)(maximum * percent) < (u32)(current * 100))) {
                scrSetIntegerReturnValue(1);
                return 1;
            }
        }
        unit = unit->nextActor;
    }
    scrSetIntegerReturnValue(0);
    return 1;
}

u32 func_0020C290(void) {
    return 1;
}

s32 btlCmdRememberUpperQueuedQuery(void) {
    u8 *context = (u8 *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, ((BtlCommandCtx *)context)->actor, choice | 0x12000000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandCtx *)context)->choicesC[10] = choice;
        ((BtlCommandCtx *)context)->selectionFlagsB |= 0x4000;
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandCtx *)context)->selectionFlagsB &= ~0x4000U;
    }
    return 1;
}

s32 btlCmdRememberLowerQueuedQuery(void) {
    u8 *context = (u8 *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, ((BtlCommandCtx *)context)->actor, choice | 0x12400000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandCtx *)context)->choicesC[11] = choice;
        ((BtlCommandCtx *)context)->selectionFlagsB |= 0x8000;
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandCtx *)context)->selectionFlagsB &= ~0x8000U;
    }
    return 1;
}

s32 btlCmdRememberLowerElementBlock(void) {
    u8 *context = (u8 *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, ((BtlCommandCtx *)context)->actor, choice | 0x11C00000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandCtx *)context)->choicesC[9] = choice;
        ((BtlCommandCtx *)context)->selectionFlagsB |= 0x2000;
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandCtx *)context)->selectionFlagsB &= ~0x2000U;
    }
    return 1;
}

s32 func_0020C450(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandCtx *)context)->actor, 0x12800000)) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

s32 btlCommandReportLinkedActionScene(void) {
    if (btlIsLinkedActionSceneStateActive()) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 btlScriptReturnBattleValue(void) {
    u64 result;

    result = func_001B76F0();
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
    scrSetIntegerReturnValue(((BtlUnit *)((BtlCommandCtx *)context)->actor)->actionTime);
    return 1;
}

u32 func_0020C560(void) {
    return 1;
}

u32 func_0020C568(void) {
    return 1;
}

u32 btlScriptReturnEffectActive(void) {
    u32 active;

    active = btlGetEffectActive();
    scrSetIntegerReturnValue(active);
    return 1;
}

u32 btlCommandReportSpecialModeValue(void) {
    u64 result;

    result = func_00220958();
    scrSetIntegerReturnValue(result);
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

u32 btlCmdGetSubtaskTargetMode(void) {
    u32 mode;

    mode = btlGetSubtaskTargetMode();
    scrSetIntegerReturnValue(mode);
    return 1;
}

u32 btlScriptReturnContextSignedByte(void) {
    s32 context;

    context = scrGetCurrentCommandWork();
    scrSetIntegerReturnValue(*(s8 *)(context + 0x14E));
    return 1;
}

u32 btlScriptReturnGlobalDebugValue(void) {
    scrSetIntegerReturnValue(btlHistoryCounter);
    return 1;
}

u32 func_0020C688(void) {
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

s32 func_0020C6F8(void) {
    btlSetSpecialBattleEffectActorByte(scrReadIntParameter(0));
    return 1;
}

u32 btlResetCommandContextAndSetStateFlag(void) {
    s32 state;
    s32 context;

    context = scrGetCurrentCommandWork();
    state = btlTrackedTaskHandles;
    ((BtlCommandCtx *)context)->commandMode = 0x10;
    ((BtlCommandCtx *)context)->commandValue = 0;
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

s32 btlScriptSetCameraBlendStart(void) {
    D_00452F90[0] = bfWaitReadArgFloat(0);
    D_00452F90[1] = bfWaitReadArgFloat(1);
    D_00452F90[2] = bfWaitReadArgFloat(2);
    D_00452FB0[0] = bfWaitReadArgFloat(3);
    D_00452FB0[1] = bfWaitReadArgFloat(4);
    D_00452FB0[2] = bfWaitReadArgFloat(5);
    D_00452FB0[3] = bfWaitReadArgFloat(6);
    return 1;
}

s32 btlScriptSetCameraBlendEnd(void) {
    D_00452F90[4] = bfWaitReadArgFloat(0);
    D_00452F90[5] = bfWaitReadArgFloat(1);
    D_00452F90[6] = bfWaitReadArgFloat(2);
    D_00452FB0[4] = bfWaitReadArgFloat(3);
    D_00452FB0[5] = bfWaitReadArgFloat(4);
    D_00452FB0[6] = bfWaitReadArgFloat(5);
    D_00452FB0[7] = bfWaitReadArgFloat(6);
    return 1;
}

u32 btlCmdCameraMoveBlend(void) {
    f32 timeA = bfWaitReadArgFloat(0);
    f32 timeB = bfWaitReadArgFloat(1);
    btlStartTask(btlCreateCommandSoundUpdateTask());
    btlStartTask(btlCreateSecondaryCommandSoundTask());
    btlStartTask(btlCreateFloatTask29(0, D_00452F90[0], D_00452F90[1], D_00452F90[2], D_00452FB0[0], D_00452FB0[1],
                                D_00452FB0[2], D_00452FB0[3], D_00452F90[4], D_00452F90[5], D_00452F90[6],
                                D_00452FB0[4], D_00452FB0[5], D_00452FB0[6], D_00452FB0[7], timeA, timeB));
    btlStartTask(btlScheduleContextReset());
    return 1;
}

u32 btlAiCheckBa(void) {
    btlBossDebugPrintf(D_00419570);
    scrSetIntegerReturnValue(0);
    return 1;
}

u32 btlAiGetAllKoudo(void) {
    btlBossDebugPrintf(D_00419590);
    scrSetIntegerReturnValue(0x14);
    return 1;
}

u32 btlScriptReturnActorUnitParameter(void) {
    BtlCommandCtx *context;

    context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    scrSetIntegerReturnValue(((BtlUnit *)context->actor)->unk122);
    return 1;
}

u32 btlAiGetFurea(void) {
    btlBossDebugPrintf(D_004195B8);
    scrSetIntegerReturnValue(0);
    return 1;
}

u32 btlCmdReportUnsupportedCameraOrigin(void) {
    btlBossDebugPrintf(D_004195D8);
    return 1;
}

u32 btlCmdReportUnsupportedCameraTwoShot(void) {
    btlBossDebugPrintf(D_004195F8);
    return 1;
}

u32 btlCmdReportUnsupportedCameraBougai(void) {
    btlBossDebugPrintf(D_00419620);
    return 1;
}

u32 btlCmdReportUnsupportedEvent(void) {
    btlBossDebugPrintf(D_00419648);
    return 1;
}

u32 btlCmdReportUnsupportedTaikyo(void) {
    btlBossDebugPrintf(D_00419668);
    return 1;
}

u32 btlCmdReportUnsupportedAllTaikyo(void) {
    btlBossDebugPrintf(D_00419688);
    return 1;
}

u32 btlCmdClearSpecialEnemyFlags(void) {
    btlClearSpecialEnemyEntryFlags();
    return 1;
}

u32 btlScriptReturnOneBasedAiBucket(void) {
    s32 index;

    index = btlRollAiBucket();
    scrSetIntegerReturnValue(index + 1);
    return 1;
}

extern s32 scrCreateTaskForProcessId(s32, s32, s32);
extern void scrSetCurrentActor(u32, u32);
extern s32 kwlnTaskGetUserValue(s32);
extern void func_00101968(s32, s32);
extern void func_001A45C0(s32, s32, s32, s32);

void btlBindActorSlot(s32 actor, s32 option) {
    s32 battle = btlGetRuntime();
    s32 task;
    s32 window;

    task = scrCreateTaskForProcessId(((BtlList *)((BtlState *)battle)->scriptOwner)->count - 1, D_00435E7C, option);
    scrSetCurrentActor(task, actor);
    window = *(s32 *)(kwlnTaskGetUserValue(task) + 0xCC);
    if (window >= 0) {
        s32 unit = (s32)((BtlActor *)actor)->unit;
        s32 width = 2;

        if (((BtlUnit *)unit)->statBits & 0x20) {
            width = 1;
        }
        func_001A45C0(window, 0, ((BtlUnit *)unit)->mode, width);
    }
    func_00101968((s32)((BtlState *)battle)->scriptOwner, task);
    ((BtlState *)battle)->boundTask = task;
}

void func_0020CE28(void) {
}

void func_0020CE30(void) {
}

void func_0020CE38(void) {
}

u32 func_0020CE40(void) {
    D_00436B00 = D_00436B00 | 0x2000000;
    return 0;
}

u32 func_0020CE58(u32 value) {
    return value;
}

void func_0020CE60(void) {
}

void func_0020CE68(void) {
}

void func_0020CE70(void) {
}

void btlReleaseActiveUnitEffectsUnlessPaused(void) {
    s32 state = btlGetRuntime();
    if ((((BtlState *)state)->battleFlags & 0x40000000) == 0) {
        s32 actor = (s32)((BtlState *)state)->units;
        while (actor != 0) {
            if ((((BtlUnit *)actor)->flags & 2) != 0) {
                s32 effect = ((BtlUnit *)actor)->ext;
                if (effect != 0) {
                    evtConfigureUnitTransition(effect, 0);
                }
            }
            actor = (s32)((BtlUnit *)actor)->nextActor;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020CEF8);

void func_0020D0D0(void) {
    D_003BEA48[4] &= ~0x80;
    D_003BE6E0[4] &= ~0x80;
}

void func_0020D100(void) {
}

void func_0020D108(void) {
}

void func_0020D110(void) {
}

void func_0020D118(void) {
}

void func_0020D120(void) {
}

void btlBossDebugPrintf(s32 format, ...) {
}

void btlBossDebugPrintfN(s32 a, s32 b, s32 c, s32 d, ...) {
}

void func_0020D1B0(void) {
}

void func_0020D1B8(void) {
}

void func_0020D1C0(void) {
}

void func_0020D1C8(void) {
}

void func_0020D1D0(void) {
}

void func_0020D1D8(void) {
}

void func_0020D1E0(void) {
}

void func_0020D1E8(void) {
}

void func_0020D1F0(void) {
}

void func_0020D1F8(void) {
}

void func_0020D200(void) {
}

void func_0020D208(void) {
}

void func_0020D210(void) {
}

void func_0020D218(void) {
}

void func_0020D220(void) {
}

void func_0020D228(void) {
}

void func_0020D230(void) {
}

void func_0020D238(void) {
}

void func_0020D240(void) {
}

void func_0020D248(void) {
}

void func_0020D250(void) {
}

void func_0020D258(void) {
}

u32 func_0020D260(u32 value) {
    return value;
}

void func_0020D268(void) {
}

void func_0020D270(void) {
}

/* Script command table: 0x18-byte rows, with the command key at row +8. */
typedef struct BtlCommandTable {
    u8 pad00[0x14];
    u8 *entries; /* 0x14 */
    s32 count;   /* 0x18 */
} BtlCommandTable;

typedef struct BtlCommandEntryView {
    u8 pad00[8];
    s32 command; /* 0x08 */
} BtlCommandEntryView;

u8 *btlFindEntryByCommand(u8 *list, s32 command) {
    s32 i;
    u8 *entry = ((BtlCommandTable *)list)->entries;
    for (i = 0; i < ((BtlCommandTable *)list)->count; i++) {
        if (((BtlCommandEntryView *)entry)->command == command) {
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
    count = ((BtlCommandTable *)list)->count;
    if (count == 0) {
        return 0;
    }
    entry = ((BtlCommandTable *)list)->entries;
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

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419570);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419590);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004195B8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004195D8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004195F8);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419620);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419648);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419668);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419688);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004196B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004196C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004196D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004196E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004196F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419700);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419710);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419720);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419730);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419740);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419750);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419760);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419770);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419780);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419790);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004197A0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004197B0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004197C0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004197D0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004197E0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_004197F0);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419800);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419810);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419820);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419830);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419840);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419850);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419860);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419870);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419880);

INCLUDE_RODATA(const s32, "game/code_00207A38", D_00419890);

s32 btlOpenPfsDebugDirectory(s32 path) {
    char buf[0x70];

    if (sdfPfsDebugMode != 0) {
        func_0035C860(buf, "pfs0:/%s", path);
        return sceDopen(buf);
    }
    D_00438F80 = 0;
    return 0;
}

void func_0020D370(s32 dir) {
    if (sdfPfsDebugMode == 0) {
        return;
    }
    func_0036B420();
}

s32 btlReadBattleResourceDirectoryEntry(s32 unused, BtlReader *reader) {
    if (sdfPfsDebugMode != 0) {
        return func_0036B588();
    }
    if ((u32)D_00438F80 >= 8) {
        return 0;
    }
    strcpy(reader->name, D_003BEA80[D_00438F80]);
    reader->flags &= ~0x1000;
    D_00438F80++;
    return strlen(reader->name);
}

INCLUDE_ASM(const s32, "game/code_00207A38", btlScanDirectory);

void btlDestroyEntryList(s32 list) {
    s32 entry;

    if (((BtlEntryList *)list)->head != 0) {
        entry = (s32)((BtlEntryList *)list)->head;
        do {
            s32 nextEntry = (s32)((BtlEntry *)entry)->next;
            sdfReleaseChipBlock(entry);
            entry = nextEntry;
        } while (entry != 0);
    }
    sdfReleaseChipBlock(((BtlEntryList *)list)->unk_04);
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

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020DAB8);

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

void btlSetResourceNameHeaderPair(s32 record, s32 first, s32 second) {
    ((BtlResourceNameRecord *)record)->word00 = first;
    ((BtlResourceNameRecord *)record)->word04 = second;
}

u32 func_0020DFC0(s32 record) {
    return ((BtlResourceNameRecord *)record)->word14;
}

u32 func_0020DFC8(s32 record) {
    return ((BtlResourceNameRecord *)record)->word08;
}

s32 btlFormatSelectedResourceName(BtlResourceDescriptor *resource, char *output) {
    s32 index = resource->selector->index;
    if (index != 0) {
        func_0035C860(output, D_00436C50, index, resource->path->name);
    } else {
        func_0035C860(output, D_00436C58, resource->path->name);
    }
    return resource->path->id;
}

s32 btlTrimResourceName(BtlResourceDescriptor *resource, char *output) {
    u32 length;
    u32 i;
    func_0035C860(output, D_00436C58, resource->path->name);
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

void btlReplaceResourceHandle(BtlResourceDescriptor *resource, s32 name) {
    s32 handle = resource->handle;
    if (handle != 0 && resource->ownsHandle == 1) {
        sdfTexReleaseReferenceViaHandler(handle);
        resource->handle = 0;
    }
    resource->handle = sdfTexAcquireResourceTexture(name);
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020E1E0);

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

void func_0020E368(void) {
    sdfReleaseChipBlock();
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020E380);

void btlSetResourceNameHeaderPairAlternate(s32 record, s32 first, s32 second) {
    ((BtlResourceNameRecord *)record)->word00 = first;
    ((BtlResourceNameRecord *)record)->word04 = second;
}

u32 func_0020E7B0(s32 record) {
    return ((BtlResourceNameRecord *)record)->word08;
}

void btlResourceRecordSetName(char *record, char *name) {
    strcpy(record + 0x21, name);
    ((BtlResourceNameRecord *)record)->nameLength = strlen(name);
}

void btlFormatResourceNameWithPrefix(s32 record, void *output) {
    func_0035C860(output, D_00436C50, record + 0x21, record + 0x1c);
}

void btlFormatResourceNameWithoutPrefix(s32 record, void *output) {
    func_0035C860(output, D_00436C58, record + 0x21);
}

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436AF0);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B00);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B04);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B08);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B0C);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B10);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B18);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B1C);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B20);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B28);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B30);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B38);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B40);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B48);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B50);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B58);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B5C);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B60);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B68);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B70);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B78);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B80);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B88);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B90);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436B98);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436BA0);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436BA4);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436BA8);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436BB0);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436BB8);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436BC0);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436BC8);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436BD0);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436BD8);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436BE0);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436BE8);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436BF0);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436BF8);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436C00);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436C08);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436C10);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436C18);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436C30);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436C38);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436C40);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436C48);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436C50);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436C58);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436C60);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436C68);

INCLUDE_SDATA(const s32, "game/code_00207A38", D_00436C70);

