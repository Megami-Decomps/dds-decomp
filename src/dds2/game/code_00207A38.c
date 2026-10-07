#include "common.h"
#include "btl_state.h"
#include "btl_action.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"
#include "kwln.h"
#include "scr.h"
#include "sdf.h"

extern u64 btlStartTask();

enum {
    BTL_RESOURCE_DESCRIPTOR_BYTES = 0x48,
    BTL_RESOURCE_NAME_RECORD_BYTES = 0x38,
    BTL_PACKED_SCREEN_X_BIAS = 0x7000,
    BTL_PACKED_SCREEN_Y_BIAS = 0x7900,
    BTL_HP_PERCENT_SCALE = 100,
    BTL_DIRECTORY_PATH_BYTES = 0x70,
    BTL_BUILTIN_NAME_COUNT = 8,
    BTL_DIRECTORY_FLAG_CLEAR = 0x1000,
    BTL_RESOURCE_ENTRY_BYTES = 0x44,
    BTL_BUTTON_ICON_SIZE = 0x40,
    /* High-bit selectors are game-specific callback-table positions. */
    BTL_PACKED_ENEMY_COUNT_LIMIT = 0x1C00000,
    BTL_PACKED_ENEMY_QUEUED_QUERY = 0x12000000,
    BTL_PACKED_PARTY_QUEUED_QUERY = 0x12400000,
    BTL_PACKED_PARTY_ELEMENT_BLOCK = 0x11C00000,
    BTL_PACKED_PARTY_EMPTY_MP = 0x12800000
};

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


extern u16 scrReadIntParameter(u32);

extern s32 func_001B76F0(void);

extern u64 func_00220958(void);

extern s32 btlTrackedTaskHandles;

extern u64 btlCreateCommandSoundUpdateTask(void);

extern u64 btlCreateSecondaryCommandSoundTask(void);

extern u64 btlCreateCommandSoundTask(u64, u64);

extern u32 D_00436B00;

extern s32 D_00436AF0;

extern s32 D_00438F6C;

extern s32 sdfReadNamedResource(s32, u32 *, s32);

extern void btlBossDebugPrintf(const char *format, ...);

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

extern BtlRuntimeTask *btlAllocTask(s32);

extern s32 btlCommandRecenterParty(void);

extern void btlFlagUnitDefeatCandidate(BtlUnit *unit);

extern void btlClearUnitDefeatCandidate(BtlUnit *unit);


extern void func_00208750(BtlIndexList *list, s32, s32);

extern void func_0021F3E8(s32);

extern void func_00211108(s32);

extern void func_002110E8(s32, u16);

extern void btlSetSpecialBattleEffectActorByte(u8);

extern s32 btlIsLinkedActionSceneStateActive(void);

typedef struct BtlVec3 {
    f32 x, y, z;
} BtlVec3;

extern f32 bfWaitReadArgFloat(s32);

extern BtlRuntimeTask *btlCreateFloatTask28(BtlTask *, f32, f32, f32, f32, f32, f32, f32, f32);

extern s32 btlScheduleContextReset(void);

extern f32 D_00452F90[];

extern f32 D_00452FB0[];

extern BtlRuntimeTask *btlCreateFloatTask29(BtlTask *, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

extern void evtConfigureUnitTransition(s32, s32);

/* Resource-browser entries are linked in scan order and occupy 0x44 bytes. */
typedef struct BtlResourceEntry {
    s32 category;
    s32 value; /* Category-specific index or metadata. */
    s32 id;
    char name[0x30];
    struct BtlResourceEntry *prev;
    struct BtlResourceEntry *next;
} BtlResourceEntry;

typedef struct BtlResourceEntryList {
    s32 count;
    char *pathPrefix; /* Owned copy of the directory-scan path, or NULL. */
    BtlResourceEntry *head;
} BtlResourceEntryList;

/* Browser viewport/selection and texture ownership; the entry list is borrowed. */
typedef struct BtlResourceDescriptor {
    s32 word00;             // 0x00
    s32 word04;             // 0x04
    u32 word08;             // 0x08
    s32 entryCount;         // 0x0C
    u32 word10[5];          // 0x10
    u32 word24;             // 0x24
    u32 word28;             // 0x28
    u32 word2C;             // 0x2C
    BtlResourceEntry *firstVisibleEntry; /* 0x30 */
    BtlResourceEntry *selectedEntry; /* 0x34 */
    u32 word38;             // 0x38
    s32 handle;             // 0x3C
    s32 ownsHandle;         // 0x40
    BtlResourceEntryList *entryList; /* 0x44 */
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

extern s32 btlReadCurrentUnitHp(DatPartyRecord *);

extern s32 btlComputeSkillAdjustedMaxHp(DatPartyRecord *);

extern s32 D_00435E7C;


BtlRuntimeTask *btlCreateControlObject(void) {
    BtlRuntimeTask *task;
    task = btlAllocTask(0);
    task->startCondition.kind = 1;
    task->callback = btlCommandRecenterParty;
    task->taskId = 0x66;
    task->onStart = NULL;
    task->endCondition.kind = 0;
    return task;
}

extern f32 sdfViewTargetVector[];
extern f32 sdfViewMatrix[];
extern f32 sdfProjectionMatrix[];
extern f32 D_0037F660[];
extern void sdfInvertRigidVuTransform(void);
extern void sdfPostmultiplyVuMatrixFromMemory(f32 *);

/* vu0 routine: forward-cone projection from vf10 to integer screen XY */
s32 btlProjectForwardPositionToScreen(s32 *out) {
    f32 distance;
    f32 facing;
    f32 *projection;
    BtlVec4 position;

    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf11, sdfViewTargetVector);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(distance);
    if (!(distance > 16.0)) {
        return 0;
    }
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_MATRIX(sdfViewMatrix);
    VU0_MOVE_MATRIX_TO_B();
    sdfInvertRigidVuTransform();
    VU0_MOVE_VF(vf10, vf30);
    VU0_NORMALIZE_VF10();
    VU0_DOT_XYZ(facing, vf10, vf11);
    if (facing <= 0.5f) {
        return 0;
    }
    VU0_MOVE_VF(vf10, vf12);
    VU0_MOVE_VF(vf28, vf24);
    VU0_MOVE_VF(vf29, vf25);
    VU0_MOVE_VF(vf30, vf26);
    VU0_MOVE_VF(vf31, vf27);
    projection = sdfProjectionMatrix;
    sdfPostmultiplyVuMatrixFromMemory(projection);
    VU0_TRANSFORM_POINT(vf10, vf10);
    VU0_PERSPECTIVE_DIVIDE_VF10();
    projection += 16;
    VU0_LOAD_VF(vf11, projection);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_0037F660);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, &position);
    out[0] = (s32)position.f[0] - 0x700;
    out[1] = (s32)position.f[1] * 2 - 0xF20;
    return 1;
}

/* vu0 routine: project the vf10 position into four screen-coordinate words with four fractional bits.
   Return zero outside the distance/forward-cone checks, without writing the output. */
s32 btlProjectForwardPositionToPackedScreen(s32 screenPosition[4]) {
    f32 viewDistance, forwardDot;
    f32 *projectionData;

    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf11, sdfViewTargetVector);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(viewDistance);
    if (!(viewDistance > 16.0))
        return 0;

    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_MATRIX(sdfViewMatrix);
    VU0_MOVE_MATRIX_TO_B();
    sdfInvertRigidVuTransform();
    VU0_MOVE_VF(vf10, vf30);
    VU0_NORMALIZE_VF10();
    VU0_DOT_XYZ(forwardDot, vf10, vf11);
    if (forwardDot <= 0.5f)
        return 0;

    VU0_MOVE_VF(vf10, vf12);
    VU0_MOVE_VF(vf28, vf24);
    VU0_MOVE_VF(vf29, vf25);
    VU0_MOVE_VF(vf30, vf26);
    VU0_MOVE_VF(vf31, vf27);
    projectionData = sdfProjectionMatrix;
    sdfPostmultiplyVuMatrixFromMemory(projectionData);
    VU0_TRANSFORM_POINT(vf10, vf10);
    VU0_PERSPECTIVE_DIVIDE_VF10();
    projectionData += 16;
    VU0_LOAD_VF(vf11, projectionData);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_0037F660);
    VU0_ADD(vf10, vf10, vf11);
    VU0_FTOI4(vf10, vf10);
    VU0_STORE_VF(vf10, screenPosition);
    screenPosition[0] -= BTL_PACKED_SCREEN_X_BIAS;
    screenPosition[1] -= BTL_PACKED_SCREEN_Y_BIAS;
    return 1;
}

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

/* Return the larger unscaled extent multiplied by the unit scale. */
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

/* Upper Y bound: scaled half-height minus the sampled position's Y. */
f32 btlUnitGetTopY(BtlUnit *unit) {
    f32 position[4];
    btlUnitGetMuzzlePosVU(unit);
    VU0_STORE_VF(vf10, position);;
    return unit->height * unit->scale * 0.5f - position[1];
}

/* Lower Y bound uses the same negated position Y and scaled half-height. */
f32 btlUnitGetBottomY(BtlUnit *unit) {
    f32 position[4];
    btlUnitGetMuzzlePosVU(unit);
    VU0_STORE_VF(vf10, position);;
    return -position[1] - unit->height * unit->scale * 0.5f;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00208000);

INCLUDE_ASM(const s32, "game/code_00207A38", func_00208298);

/* Maximum top bound among active units matching any mask bit; zero if none. */
f32 btlGetMaxUnitTop(u32 mask) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    f32 maxTopY = 0.0f;
    s32 needsFirstSample = 1;
    while (unit != NULL) {
        if ((unit->flags & 1) && (unit->flags & mask)) {
            f32 topY = btlUnitGetTopY(unit);
            if (needsFirstSample) {
                maxTopY = topY;
                needsFirstSample = 0;
            } else if (maxTopY < topY) {
                maxTopY = topY;
            }
        }
        unit = unit->nextActor;
    }
    return maxTopY;
}

/* Maximum scaled reach among active units matching any mask bit; zero if none. */
f32 btlGetMaxUnitReach(u32 mask) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    f32 maxReach = 0.0f;
    s32 needsFirstSample = 1;
    while (unit != NULL) {
        if ((unit->flags & 1) && (unit->flags & mask)) {
            f32 scaledReach = unit->reach * unit->scale;
            if (needsFirstSample) {
                maxReach = scaledReach;
                needsFirstSample = 0;
            } else if (maxReach < scaledReach) {
                maxReach = scaledReach;
            }
        }
        unit = unit->nextActor;
    }
    return maxReach;
}

/* Despite the legacy Y name, this selects a Z edge from position[2].
 * Mask bit 0x200 selects the maximum plus reach; otherwise use the minimum
 * minus reach. No matching active unit leaves the zero result. */
f32 btlGetExtremeUnitY(u32 mask) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    f32 extremeZ = 0.0f;
    s32 needsFirstSample = 1;
    f32 position[4];
    f32 edgeZ;
    while (unit != NULL) {
        if ((unit->flags & 1) && (unit->flags & mask)) {
            btlUnitGetMuzzlePosVU(unit);
            VU0_STORE_VF(vf10, position);;
            if (mask & 0x200) {
                edgeZ = position[2] + unit->reach * unit->scale;
                if (needsFirstSample) {
                    extremeZ = edgeZ;
                    needsFirstSample = 0;
                } else if (extremeZ < edgeZ) {
                    extremeZ = edgeZ;
                }
            } else {
                edgeZ = position[2] - unit->reach * unit->scale;
                if (needsFirstSample) {
                    extremeZ = edgeZ;
                    needsFirstSample = 0;
                } else if (edgeZ < extremeZ) {
                    extremeZ = edgeZ;
                }
            }
        }
        unit = unit->nextActor;
    }
    return extremeZ;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_00208750);

/* Compare VU-computed distances from the target's muzzle to eligible units. */
BtlUnit *btlFindNearestUnit(u32 mask, BtlUnit *target) {
    BtlState *state = (BtlState *)btlGetRuntime();
    BtlUnit *unit;
    BtlUnit *nearest;
    s32 needsFirstSample;
    f32 nearestDistance;
    f32 distance;
    BtlVec4 targetPos;
    btlUnitGetMuzzlePosVU(target);
    VU0_STORE_VF(vf10, &targetPos);;
    unit = state->units;
    nearest = NULL;
    needsFirstSample = 1;
    nearestDistance = 0.0f;
    while (unit != NULL) {
        if (unit->flags & 1) {
            if (!(unit->flags & 0xC0)) {
                if (target != unit) {
                    if (unit->flags & mask) {
                        btlUnitGetMuzzlePosVU(unit);
                                        VU0_SCALAR_OP_CLOBBER(targetPos.f[1], "vaddx.y vf10, vf0, vf2x");
                        VU0_LOAD_VF(vf11, &targetPos);;
                        VU0_SUB(vf10, vf10, vf11);
                        VU0_LENGTH_VF10(distance);
                        if (needsFirstSample) {
                            nearestDistance = distance;
                            nearest = unit;
                            needsFirstSample = 0;
                        } else if (distance < nearestDistance) {
                            nearestDistance = distance;
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

/* Return the eligible unit farthest from point, using the VU distance.
 * The zero baseline leaves NULL when no candidate has a positive distance. */
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
        unit = unit->nextActor;
    }
    return farthest;
}

/* Select the smallest X for reference flag 0x200, otherwise the largest.
 * An empty indexed list returns NULL but still loads the native scratch vector. */
/* vu0 routine: returns the selected muzzle position in vf10. */
BtlUnit *btlSelectUnitAtExtremeX(BtlUnit *reference, BtlIndexList *list) {
    BtlVec4 position, selectedPosition;
    BtlUnit *selected = NULL;
    BtlUnit *unit;
    u32 entryIndex = 0;
    u32 entryCount = btlGetIndexListCount(list);

    for (; entryIndex < entryCount; entryIndex++) {
        unit = btlGetIndexListEntry(list, entryIndex);
        PCP_COPY_VECTOR(&position, unit->position);
        position.f[2] += unit->positionZOffset;
        /* The indexed DDS2 path uses the vector at 0x40 and scale at 0x50,
         * unlike the ordinary muzzle accessor's 0x70/0x80 fields. */
        VU0_LOAD_VF(vf10, unit->rotation);
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
        btlFlagUnitDefeatCandidate(unit);
    }
}

void btlClearAllUnitDefeatCandidates(void) {
    BtlUnit *unit;
    BtlState *work = (BtlState *)btlGetRuntime();
    for (unit = work->units; unit != NULL; unit = unit->nextActor) {
        btlClearUnitDefeatCandidate(unit);
    }
}

void btlFlagMatchingUnitsDefeatCandidate(s32 mask) {
    BtlUnit *unit;

    unit = ((BtlState *)btlGetRuntime())->units;
    if (unit != NULL) {
        do {
            if (unit->flags & mask) {
                btlFlagUnitDefeatCandidate(unit);
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
                btlClearUnitDefeatCandidate(unit);
            }
            unit = unit->nextActor;
        } while (unit != NULL);
    }
}

/* Mark every unit in the indexed list as a defeat candidate. */
void btlFlagActorUnitsDefeatCandidate(BtlIndexList *list) {
    u32 entryIndex = 0;
    u32 entryCount = btlGetIndexListCount(list);
    if (entryCount != 0) {
        do {
            btlFlagUnitDefeatCandidate(btlGetIndexListEntry(list, entryIndex));
            entryIndex++;
        } while (entryIndex < entryCount);
    }
}

/* Clear the defeat-candidate state of every unit in the indexed list. */
void btlClearActorUnitDefeatCandidates(BtlIndexList *list) {
    u32 entryIndex = 0;
    u32 entryCount = btlGetIndexListCount(list);
    if (entryCount != 0) {
        do {
            btlClearUnitDefeatCandidate(btlGetIndexListEntry(list, entryIndex));
            entryIndex++;
        } while (entryIndex < entryCount);
    }
}

extern void btlApplyUnitMotionSelection(u8 *, u32, s32, f32);
extern void btlSetUnitPosition(BtlUnit *, f32 *);
extern void btlSetUnitRotation(BtlUnit *, s128 *);

/* Refresh each unit's transform/effect state, then invoke the runtime callback. */
void btlUpdateUnitActors(void) {
    BtlState *state = (BtlState *)btlGetRuntime();
    BtlUnit *unit = state->units;

    while (unit != NULL) {
        btlFlagUnitDefeatCandidate(unit);
        btlSetUnitPosition(unit, unit->position);
        btlSetUnitRotation(unit, (s128 *)unit->rotation);
        if ((btlIsActorModeAcceptedByBattleHook((s32)unit) == 0 && unit->effectState != 0) ||
            (unit->updateFlags & 2) != 0) {
            btlRefreshUnitMotionSelection((s32)unit);
            unit->unkF8 = 0;
            unit->unkFA = 0;
            btlApplyUnitMotionSelection((u8 *)unit, unit->effectIndex, unit->effectParameter, unit->effectScale);
        }
        unit->stateFlags &= ~0x8000;
        unit->stateFlags &= ~0x200000;
        unit = unit->nextActor;
    }
    {
        void (*callback)(void) = state->afterUnitUpdate;
        if (callback != 0) {
            callback();
        }
    }
}
extern void evtSetUnitStatusFlags(struct EvtUnit *);
extern void evtSetUnitNormalizedDirection(struct EvtUnit *, s32);
extern void evtSetUnitRgbTransition(struct EvtUnit *, s32, u32);
extern s32 btlGetEntryFlagsUnlessDisabled(DatPartyRecord *);
extern u32 kwlnDrawControlFlags;

/* Refresh active actor lighting and restore eligible enemy colors. */
void func_00209078(void) {
    BtlState *state = (BtlState *)btlGetRuntime();
    BtlUnit *unit = state->units;
    struct EvtUnit *effect;

    while (unit != NULL) {
        if (unit->flags & 2) {
            if (unit->stateFlags & 0x10) {
                effect = unit->ext;
                evtSetUnitStatusFlags(effect);
                VU0_LOAD_VF(vf10, unit->lightDirection);
                evtSetUnitNormalizedDirection(effect, 0);
                if ((*(u64 *)&unit->flags & 0x4E0) == 0x420) {
                    if (btlGetEntryFlagsUnlessDisabled(&unit->partyRecord) & 0x200) {
                        evtSetUnitRgbTransition(unit->ext, 0, unit->baseColor);
                    }
                }
            }
        }
        unit = unit->nextActor;
    }
    if (state->commandRestrictFlags & 0x800000) {
        kwlnDrawControlFlags |= 0x2000000;
    }
}


/* Count matching units with bit 0 set, excluding every unit with bit 0x20. */
s32 btlCountActiveUnitsWithFlags(s32 mask) {
    BtlUnit *unit;
    s32 activeCount = 0;
    s32 unitFlags;

    unit = ((BtlState *)btlGetRuntime())->units;
    if (unit != NULL) {
        do {
            unitFlags = unit->flags;
            if (((unitFlags & mask) != 0) && ((unitFlags & 0x20) == 0)) {
                activeCount += unitFlags & 1;
            }
            unit = unit->nextActor;
        } while (unit != NULL);
    }
    return activeCount;
}

extern void func_00340DC8(f32, f32, f32);
extern f32 func_003532E8(f32, f32);
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);
extern f32 sdfSinPoly(f32);
extern u128 D_003BE0D0;

/* Aim along the horizontal origin-to-target delta; zero delta loads the default vf10. */
s32 btlAimHorizontalDirectionVU(f32 *origin, f32 *targetPosition) {
    f32 delta[4];
    delta[0] = targetPosition[0] - origin[0];
    delta[2] = targetPosition[2] - origin[2];
    if (delta[0] != 0.0f || delta[2] != 0.0f) {
        func_00340DC8(0.0f, func_003532E8(delta[0], delta[2]), 0.0f);
        return 1;
    }
    VU0_LOAD_VF($vf10, &D_003BE0D0);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00207A38", btlAimHorizontalDirectionClampedVU);

/* vu0 routine: vf10.xyz = normalize(e x (e x f)),
 * e = vertexB - vertexA, f = vertexC - vertexA.
 * The double cross lies in the triangle plane, despite the legacy normal name. */
void btlTriangleNormalVU(f32 *vertexA, f32 *vertexB, f32 *vertexC) {
    VU0_LOAD_VF(vf10, vertexB);
    VU0_LOAD_VF(vf11, vertexA);
    VU0_SUB(vf10, vf10, vf11);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf11, vertexC);
    VU0_LOAD_VF_MEMORY(vf10, vertexA);
    VU0_SUB(vf11, vf11, vf10);
    VU0_MOVE_VF(vf10, vf12);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_MOVE_VF(vf11, vf10);
    VU0_MOVE_VF(vf10, vf12);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
}

/* Dot the normalized double-cross direction with vertexA - vertexC. */
f32 btlTriangleNormalDotEdge(f32 *vertexA, f32 *vertexB, f32 *vertexC) {
    f32 direction[4];
    f32 projection;
    btlTriangleNormalVU(vertexA, vertexB, vertexC);
    VU0_STORE_VF(vf10, direction);;
    VU0_LOAD_VF(vf10, vertexA);;
    VU0_LOAD_VF(vf11, vertexC);;
    VU0_SUB(vf10, vf10, vf11);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, direction);;
        VU0_DOT_XYZ(projection, vf10, vf11);
    return projection;
}

/* vu0 routine: vf10 = vertexC + normalize(point - vertexC) * projection,
 * where projection is the double-cross direction dotted with vertexA - vertexC. */
void btlPointOffPlaneVU(f32 *vertexA, f32 *vertexB, f32 *vertexC, f32 *point) {
    f32 projection = btlTriangleNormalDotEdge(vertexA, vertexB, vertexC);
    VU0_LOAD_VF(vf10, point);
    VU0_LOAD_VF(vf11, vertexC);
    VU0_SUB(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_SCALAR_OP(projection, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_LOAD_VF(vf11, vertexC);
    VU0_ADD(vf10, vf10, vf11);
}

/* vu0 routine: vf10 = vertexC + direction * dot(direction, vertexA - vertexC).
 * Direction is the normalized in-plane double cross, not a triangle normal. */
void btlProjectOnPlaneVU(f32 *vertexA, f32 *vertexB, f32 *vertexC) {
    f32 direction[4];
    f32 projection;
    btlTriangleNormalVU(vertexA, vertexB, vertexC);
    VU0_STORE_VF(vf10, direction);;
    VU0_LOAD_VF(vf10, vertexA);
    VU0_LOAD_VF(vf11, vertexC);
    VU0_SUB(vf10, vf10, vf11);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, direction);
    VU0_DOT_XYZ(projection, vf10, vf11);
    VU0_SCALAR_OP(projection, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_LOAD_VF(vf11, vertexC);
    VU0_ADD(vf10, vf10, vf11);
}

/* Inclusive box test; either corner order is accepted independently per axis. */
s32 btlPointInBox(BtlVec3 *cornerA, BtlVec3 *cornerB, BtlVec3 *point) {
    if (((cornerA->x >= point->x && point->x >= cornerB->x) || (cornerA->x <= point->x && point->x <= cornerB->x))
        && ((cornerA->y >= point->y && point->y >= cornerB->y) || (cornerA->y <= point->y && point->y <= cornerB->y))
        && ((cornerA->z >= point->z && point->z >= cornerB->z) || (cornerA->z <= point->z && point->z <= cornerB->z))) {
        return 1;
    }
    return 0;
}

/* vu0 routine: interpolate packed RGBA bytes using a 1/128 channel scale.
 * Factors at or above one return colorB directly; negative factors are not clamped. */
u32 btlBlendColor(u32 colorA, u32 colorB, f32 blendFactor) {
    s32 colorBStorage[4];
    s32 colorAStorage[4];
    s32 packedStorage[4];
    u32 packed;
    u32 channelScaleBits;
    if (blendFactor >= 1.0f) {
        return colorB;
    }
    channelScaleBits = 0x3C000000;
    colorBStorage[0] = colorB;
    EE_MMI_RGBA_UNPACK(colorBStorage, channelScaleBits);
    VU0_MOVE_VF(vf11, vf10);;
    colorAStorage[0] = colorA;
    EE_MMI_RGBA_UNPACK(colorAStorage, channelScaleBits);
    VU0_SCALAR_OP_CLOBBER(1.0f - blendFactor, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALAR_OP_R3_CLOBBER(blendFactor, "vmulx.xyzw vf11, vf11, vf2x");
    VU0_ADD(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK(packed);
    packedStorage[0] = packed;
    
    return packed;
}

/* vu0 routine: interpolate RGBA float vectors, multiply channels by 128,
 * then truncate and pack. Unlike the packed-color routine, neither endpoint
 * is returned early and the blend factor is not clamped. */
u32 btlBlendColorVec(f32 *colorA, f32 *colorB, f32 blendFactor) {
    u32 packed;
    s32 packedStorage[4];
    VU0_LOAD_VF(vf10, colorA);
    VU0_LOAD_VF(vf11, colorB);
    VU0_SCALAR_OP(1.0f - blendFactor, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALAR_OP_R3(blendFactor, "vmulx.xyzw vf11, vf11, vf2x");
    VU0_ADD(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK(packed);
    packedStorage[0] = packed;
    return packedStorage[0];
}

/* Scalar interpolation state shared by the initialization routines below. */
typedef struct BtlScalarRange {
    f32 start;       /* 0x00 */
    f32 end;         /* 0x04 */
    f32 inverseSpan; /* 0x08 */
    f32 zero;        /* 0x0C */
    f32 target;      /* 0x10 */
} BtlScalarRange;

/* Store the starting span and clear end; the other state fields are untouched. */
void btlScalarRangeSetStartClearEnd(s32 rangeAddress, f32 start) {
    ((BtlScalarRange *)rangeAddress)->start = start;
    ((BtlScalarRange *)rangeAddress)->end = 0;
}

/* Advance progress by (1 - progress) / span and return at most one.
   Only the return is capped: stored progress is raw. A nonpositive span is unchanged. */
f32 btlScalarRangeStepExponential(s32 rangeAddress) {
    BtlScalarRange *state = (BtlScalarRange *)rangeAddress;
    f32 result = 0.0f;
    f32 span = state->start;
    f32 progress = state->end;
    f32 one;

    if (span <= result) {
        result = 1.0f;
    } else {
        one = 1.0f;
        progress = (progress * (span - one) + one) / span;
        state->end = progress;
        result = progress;
        if (!(result < one)) {
            result = one;
        }
    }
    return result;
}

/* Initialize the quadratic accumulator; a zero span leaves inverseSpan unchanged. */
void btlScalarRangeInitQuadratic(s32 rangeAddress, f32 start) {
    f32 initialValue;

    ((BtlScalarRange *)rangeAddress)->zero = 0.0f;
    ((BtlScalarRange *)rangeAddress)->start = start;
    initialValue = ((BtlScalarRange *)rangeAddress)->zero;
    ((BtlScalarRange *)rangeAddress)->end = start;
    ((BtlScalarRange *)rangeAddress)->target = initialValue;
    if (start == initialValue) {
        return;
    }
    ((BtlScalarRange *)rangeAddress)->inverseSpan = 1.0f / (start * start * 0.25f);
}

/* Integrate the quadratic accumulator, reversing acceleration after the midpoint.
   Completion returns one without updating state; intermediate values are not capped. */
f32 btlScalarRangeStepQuadratic(BtlScalarRange *state, f32 timeStep) {
    f32 accumulatedValue = state->target;
    f32 velocity = state->zero;
    f32 remainingSpan = state->end;

    remainingSpan -= timeStep;

    if (remainingSpan <= 0.0f) {
        return 1.0f;
    }
    if (remainingSpan < state->start * 0.5f) {
        velocity -= state->inverseSpan * timeStep;
    } else {
        velocity += state->inverseSpan * timeStep;
    }
    state->end = remainingSpan;
    state->zero = velocity;
    accumulatedValue += velocity * timeStep;
    state->target = accumulatedValue;
    return accumulatedValue;
}

INCLUDE_ASM(const s32, "game/code_00207A38", btlDrawIconAtSize);

extern void btlDrawIconAtSize(SdfPoolNode *, s32, s32, s32, s32, s32, s32, s32, s32, u16);

/* Draw a button glyph at the fixed 64-pixel size using the four supplied corner colors. */
void btlDrawButtonIconFixed64(SdfPoolNode *surface, s32 x, s32 y, s32 topLeftColor, s32 topRightColor, s32 bottomLeftColor, s32 bottomRightColor, s32 button) {
    btlDrawIconAtSize(surface, x, y, BTL_BUTTON_ICON_SIZE, BTL_BUTTON_ICON_SIZE, topLeftColor, topRightColor, bottomLeftColor, bottomRightColor, button);
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
struct RefObj;
extern SdfTex *func_002DDD60(void *, struct RefObj *);
extern void sdfQueueGouraudTexturedQuad(s32, s32, s32, s32, s32, s32,
    s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32,
    s32, s32, s32, s32, s32, s32 (*)(s32));
extern s32 sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(SdfListHead *);
extern s32 sdfConsCreateDrawPacket(SdfListHead *, SdfTex *, s32);

/* Draw a button glyph using its UV pair; screen coordinates are GS fixed-point. */
void btlDrawButtonIcon(SdfPoolNode *surface, s32 x, s32 y, s32 topLeftColor, s32 topRightColor, s32 bottomLeftColor, s32 bottomRightColor, s32 button) {
    BtnUv *uv;
    SdfTex *texture;
    SdfListHead *packet;
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
        texture = func_002DDD60(surface, (struct RefObj *)((BtlState *)btlGetRuntime())->buttonTextureHandle);
        packet = (SdfListHead *)sdfAllocPacketAligned(0x20);
        sdfInitPacketList(packet);
        sdfConsCreateDrawPacket(packet, texture, 0);
        xFixed = x * 0x10;
        yFixed = y * 8;
        sdfQueueGouraudTexturedQuad((s32)packet, 0x40,
                      xFixed + 0x7000, yFixed + 0x7900, uv->u * 0x10, uv->v * 0x10, topLeftColor,
                      xFixed + 0x7200, yFixed + 0x7900, uv->u * 0x10 + 0x200, uv->v * 0x10, topRightColor,
                      xFixed + 0x7000, yFixed + 0x7A00, uv->u * 0x10, uv->v * 0x10 + 0x200, bottomLeftColor,
                      xFixed + 0x7200, yFixed + 0x7A00, uv->u * 0x10 + 0x200, uv->v * 0x10 + 0x200, bottomRightColor,
                      0xFF0000, 0);
        surface->append((SdfListHead *)surface, packet);
    }
}

void btlOpenButtonIconResource(void) {
    btlBossDebugPrintf("btl:[%s]\n", D_00436AF0);
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

/* Resolve a command-table entry through its probe, or store the direct command id. */
u32 btlScriptSelectActionEntry(void) {
    BtlCommandCtx *context;
    BtlActionProbe probe;
    s32 commandId;

    context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    commandId = scrReadIntParameter(0);
    if (datCommandSelectors[commandId * 2 + 1] == 1) {
        if (func_001B2F50((void *)context->actor, commandId) != 0 &&
            func_001ACD10((void *)context->actor, commandId, &probe) != 0) {
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
        context->commandValue = commandId;
    }
    return 1;
}

u32 btlScriptSelectByKind(void) {
    BtlCommandCtx *context;
    s32 commandId;
    s32 selectionValue;

    context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    commandId = scrReadIntParameter(0);
    selectionValue = scrReadIntParameter(1);
    context->commandMode = 2;
    context->commandValue = commandId;
    if (commandId == 0x196) {
        selectionValue = func_0021F698();
    }
    context->pendingValue = selectionValue;
    context->selectedValue = selectionValue;
    return 1;
}

/* value starts as the weighted-table selector and is then replaced by the chosen entry. */
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
    s32 selectionValue;

    context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    selectionValue = scrReadIntParameter(0);
    context->commandMode = 5;
    context->commandValue = 0;
    context->pendingValue = selectionValue;
    context->selectedValue = selectionValue;
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
    u16 parameterValue;
    s32 context;

    context = scrGetCurrentCommandWork();
    parameterValue = scrReadIntParameter(0);
    ((BtlUnit *)((BtlCommandCtx *)context)->actor)->partyRecord.affinityTableIndex = parameterValue;
    return 1;
}

u32 btlScriptSetBattleWorkParameter(void) {
    s32 battleAddress;
    s16 requestArgument;

    battleAddress = btlGetRuntime();
    scrGetCurrentCommandWork();
    requestArgument = scrReadIntParameter(0);
    ((BtlState *)battleAddress)->requestMode = 4;
    ((BtlState *)battleAddress)->requestArgument = requestArgument;
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

s32 btlCmdStoreBossHealthCheckChoice(void) {
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

/* Remember the count limit only when the eligible enemy count is at most that limit.
   Failure clears the selection flag but leaves the previous stored choice intact. */
s32 btlCmdStoreEnemyCountCheckChoice(void) {
    BtlCommandCtx *context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, context->actor, choice | BTL_PACKED_ENEMY_COUNT_LIMIT)) {
        scrSetIntegerReturnValue(1);
        context->choicesA[3] = choice;
        context->selectionFlagsA |= 8;
    } else {
        scrSetIntegerReturnValue(0);
        context->selectionFlagsA &= ~8U;
    }
    return 1;
}

s32 btlCmdStorePartyCountCheckChoice(void) {
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

s32 btlCmdStoreActorActionMaskChoice(void) {
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

s32 btlCmdStoreEnemyActionMaskChoice(void) {
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

s32 btlCmdStorePartyActionMaskChoice(void) {
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

u32 btlCmdReturnReadyWithoutTurnsCheck(void) {
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

s32 btlCmdStoreCounterReachedLimitChoice(void) {
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

u32 btlCmdReturnLowHpActionReadiness(void) {
    s32 context;

    context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandCtx *)context)->actor, 0x8800000) != 0) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 btlCmdReturnActorEligibility(void) {
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

u32 btlCmdReturnPartyElementQueryCheck(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandCtx *)context)->actor, scrReadIntParameter(0) | 0x12C00000)) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 btlCmdReturnEnemyElementQueryCheck(void) {
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

s32 btlCmdStoreDefaultUnitActionChoice(void) {
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

s32 btlCmdStoreAlternateUnitActionChoice(void) {
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

s32 btlCmdStorePartyEntryCheckChoice(void) {
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

s32 btlCmdStoreInversePartyEntryChoice(void) {
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

u32 btlCmdReturnPartyActionPresence(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandCtx *)context)->actor, scrReadIntParameter(0) | 0x13400000)) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

s32 btlCmdReturnEnemyActionPresence(void) {
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

u32 btlCmdReturnPartyMissingFlagCheck(void) {
    s32 context;

    context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandCtx *)context)->actor, 0x11000000) != 0) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 btlCmdReturnPartyHeldFlagCheck(void) {
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

/* Report an eligible matching unit at or below the scripted HP percentage.
   Preserve the native signed products and their unsigned comparison. */
u32 btlScriptReturnUnitHpRatioPercent(void) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    s32 sideChoice = scrReadIntParameter(0);
    s32 lookupId = scrReadIntParameter(1);
    s32 hpPercentThreshold = scrReadIntParameter(2);
    u32 sideMask = 0x200;
    if (sideChoice) {
        sideMask = 0x400;
    }
    while (unit != NULL) {
        if ((unit->flags & 1) && (unit->flags & sideMask) && !(unit->flags & 0x20) && unit->owner == lookupId) {
            DatPartyRecord *unitStats = &unit->partyRecord;
            s32 currentHp = btlReadCurrentUnitHp(unitStats);
            s32 maximumHp = btlComputeSkillAdjustedMaxHp(unitStats);
            if (!((u32)(maximumHp * hpPercentThreshold) < (u32)(currentHp * BTL_HP_PERCENT_SCALE))) {
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

/* Remember the choice when an enemy's queued query matches; failure clears only its flag. */
s32 btlCmdRememberUpperQueuedQuery(void) {
    u8 *context = (u8 *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, ((BtlCommandCtx *)context)->actor, choice | BTL_PACKED_ENEMY_QUEUED_QUERY)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandCtx *)context)->choicesC[10] = choice;
        ((BtlCommandCtx *)context)->selectionFlagsB |= 0x4000;
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandCtx *)context)->selectionFlagsB &= ~0x4000U;
    }
    return 1;
}

/* Remember the choice when a party unit's queued query matches; failure clears only its flag. */
s32 btlCmdRememberLowerQueuedQuery(void) {
    u8 *context = (u8 *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, ((BtlCommandCtx *)context)->actor, choice | BTL_PACKED_PARTY_QUEUED_QUERY)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandCtx *)context)->choicesC[11] = choice;
        ((BtlCommandCtx *)context)->selectionFlagsB |= 0x8000;
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandCtx *)context)->selectionFlagsB &= ~0x8000U;
    }
    return 1;
}

/* Remember the choice on a successful party-element block check; failure clears only its flag. */
s32 btlCmdRememberLowerElementBlock(void) {
    u8 *context = (u8 *)scrGetCurrentCommandWork();
    u32 choice = scrReadIntParameter(0);
    if (btlDispatchPackedActionWithScratch((s32)context, ((BtlCommandCtx *)context)->actor, choice | BTL_PACKED_PARTY_ELEMENT_BLOCK)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandCtx *)context)->choicesC[9] = choice;
        ((BtlCommandCtx *)context)->selectionFlagsB |= 0x2000;
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandCtx *)context)->selectionFlagsB &= ~0x2000U;
    }
    return 1;
}

/* Report whether an eligible party unit has zero current MP; the handler tests unit +0x12A. */
s32 btlCmdReportPartyEmptyMp(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandCtx *)context)->actor, BTL_PACKED_PARTY_EMPTY_MP)) {
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
    s32 result;

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
    scrSetIntegerReturnValue(((BtlUnit *)((BtlCommandCtx *)context)->actor)->partyRecord.level);
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
    scrSetIntegerReturnValue(((BtlUnit *)context->actor)->partyRecord.affinityTableIndex);
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
extern u32 kwlnTaskGetUserValue(KwlnTask *task);
extern void func_00101968(s32, s32);
extern void func_001A45C0(s32, s32, s32, s32);

void btlBindActorSlot(s32 actor, s32 option) {
    s32 battle = btlGetRuntime();
    s32 task;
    s32 window;

    task = scrCreateTaskForProcessId((s32)((BtlState *)battle)->scriptOwner->priority - 1, D_00435E7C, option);
    scrSetCurrentActor((KwlnTask *)task, (void *)actor);
    window = ((ScrData *)kwlnTaskGetUserValue((KwlnTask *)task))->resourceIndex;
    if (window >= 0) {
        s32 unit = (s32)((BtlActor *)actor)->unit;
        s32 width = 2;

        if (((BtlUnit *)unit)->partyRecord.flags & 0x20) {
            width = 1;
        }
        func_001A45C0(window, 0, ((BtlUnit *)unit)->partyRecord.unitId, width);
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

/* Retail discards debug output but retains the format-string/varargs ABI. */
void btlBossDebugPrintf(const char *format, ...) {
}

void btlBossDebugPrintfN(s32 a, s32 b, s32 c, s32 d, ...) {
}

/* Retail discards debug text while retaining the callers' four-argument ABI. */
void func_0020D1B0(s32 x, s32 y, s32 style, const char *text) {
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
    BtlCommandEntry *entries; /* 0x14 */
    s32 count;   /* 0x18 */
} BtlCommandEntryList;

/* Return the matching entry, or the one-past-end address when none matches. */
u8 *btlFindEntryByCommand(BtlCommandEntryList *list, s32 command) {
    s32 i;
    BtlCommandEntry *entry = list->entries;
    for (i = 0; i < list->count; i++) {
        if (entry->command == command) {
            return (u8 *)entry;
        }
        entry++;
    }
    return (u8 *)entry;
}

/* Count entries other than the two part opcodes, 0x14 and 0x15. */
s32 btlCountNonPartOpcodes(BtlCommandEntryList *list) {
    u32 count;
    BtlCommandEntry *entry;
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
        switch (entry->opcode) {
        case 0x14:
        case 0x15:
            break;
        default:
            found++;
            break;
        }
        entry++;
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

s32 btlOpenPfsDebugDirectory(s32 directoryName) {
    char pathBuffer[BTL_DIRECTORY_PATH_BYTES];

    /* Debug mode opens a formatted pfs0 path; built-in mode resets name iteration. */

    if (sdfPfsDebugMode != 0) {
        func_0035C860(pathBuffer, "pfs0:/%s", directoryName);
        return sceDopen(pathBuffer);
    }
    D_00438F80 = 0;
    return 0;
}

/* Close the scanned directory only in debug filesystem mode; ignore the native result. */
void btlClosePfsDebugDirectory(s32 directoryHandle) {
    if (sdfPfsDebugMode == 0) {
        return;
    }
    func_0036B420();
}

/* Debug mode delegates to the native reader. Built-in mode returns each name's length,
   clears the directory-type bit, and returns zero when the eight-name table is exhausted. */
s32 btlReadBattleResourceDirectoryEntry(s32 directoryHandle, BtlReader *reader) {
    if (sdfPfsDebugMode != 0) {
        return func_0036B588();
    }
    if ((u32)D_00438F80 >= BTL_BUILTIN_NAME_COUNT) {
        return 0;
    }
    strcpy(reader->name, D_003BEA80[D_00438F80]);
    reader->flags &= ~BTL_DIRECTORY_FLAG_CLEAR;
    D_00438F80++;
    return strlen(reader->name);
}

INCLUDE_ASM(const s32, "game/code_00207A38", btlScanDirectory);

/* Release all scanned entries, the owned path prefix, and the list header. */
void btlDestroyEntryList(BtlResourceEntryList *list) {
    BtlResourceEntry *entry;

    if (list->head != NULL) {
        entry = list->head;
        do {
            BtlResourceEntry *next = entry->next;
            sdfReleaseChipBlock(entry);
            entry = next;
        } while (entry != NULL);
    }
    sdfReleaseChipBlock(list->pathPrefix);
    sdfReleaseChipBlock(list);
}

/* Append a named browser entry without changing the current scan order. */
void btlAppendEntry(BtlResourceEntryList *list, char *name, s32 category, s32 value, s32 id) {
    BtlResourceEntry *entry = sdfAllocSizeClassBlock(BTL_RESOURCE_ENTRY_BYTES);
    BtlResourceEntry *tail;
    entry->category = category;
    entry->value = value;
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

/* Initialize a browser descriptor at the first entry of the borrowed list.
 * The native constructor leaves ownsHandle untouched. */
BtlResourceDescriptor *btlCreateResourceDescriptor(BtlResourceEntryList *list) {
    BtlResourceDescriptor *descriptor = sdfAllocSizeClassBlock(BTL_RESOURCE_DESCRIPTOR_BYTES);

    descriptor->word00 = 8;
    descriptor->word04 = 8;
    descriptor->word08 = 0;
    descriptor->entryCount = list->count;
    descriptor->word10[0] = 0;
    descriptor->word10[1] = 0;
    descriptor->word10[2] = 0;
    descriptor->word10[3] = 0;
    descriptor->word10[4] = 0;
    descriptor->word24 = 0x60;
    descriptor->word28 = 0x80806020;
    descriptor->word2C = 0x60000000;
    descriptor->firstVisibleEntry = list->head;
    descriptor->selectedEntry = list->head;
    descriptor->word38 = 0;
    descriptor->handle = 0;
    descriptor->entryList = list;
    return descriptor;
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020DAB8);

/* Release an owned texture handle and the descriptor, but not its entry list. */
void btlDestroyResourceDescriptor(BtlResourceDescriptor *descriptor) {
    s32 textureHandle = descriptor->handle;
    if (textureHandle != 0 && descriptor->ownsHandle == 1) {
        sdfTexReleaseReferenceViaHandler(textureHandle);
    }
    sdfReleaseChipBlock(descriptor);
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

u32 func_0020DFC0(s32 recordAddress) {
    return ((BtlResourceNameRecord *)recordAddress)->word14;
}

u32 func_0020DFC8(s32 recordAddress) {
    return ((BtlResourceNameRecord *)recordAddress)->word08;
}

/* Format prefix + selected name; return its resource category, not its id. */
s32 btlFormatSelectedResourceName(BtlResourceDescriptor *descriptor, char *output) {
    char *pathPrefix = descriptor->entryList->pathPrefix;
    if (pathPrefix != NULL) {
        func_0035C860(output, D_00436C50, pathPrefix, descriptor->selectedEntry->name);
    } else {
        func_0035C860(output, D_00436C58, descriptor->selectedEntry->name);
    }
    return descriptor->selectedEntry->category;
}

/* Strip from the first '.' onward; return the selected entry's category. */
s32 btlTrimResourceName(BtlResourceDescriptor *descriptor, char *output) {
    u32 nameLength;
    u32 characterIndex;
    func_0035C860(output, D_00436C58, descriptor->selectedEntry->name);
    nameLength = strlen(output);
    for (characterIndex = 0; characterIndex < nameLength && output[characterIndex] != '.'; characterIndex++) {
    }
    if (nameLength != characterIndex) {
        output[characterIndex] = 0;
    }
    return descriptor->selectedEntry->category;
}

/* Return the selected entry's category-specific metadata word unchanged. */
u32 btlGetResourcePathVariant(BtlResourceDescriptor *resource) {
    return resource->selectedEntry->value;
}

/* Replace an owned old texture, acquire the named resource's texture,
 * then release the temporary allocation returned by the resource reader. */
void btlLoadAndReplaceResourceHandle(BtlResourceDescriptor *descriptor, s32 nameAddress) {
    u32 loadedResource;
    s32 textureHandle = descriptor->handle;
    s32 allocationHandle;
    if (textureHandle != 0 && descriptor->ownsHandle == 1) {
        sdfTexReleaseReferenceViaHandler(textureHandle);
        descriptor->handle = 0;
    }
    allocationHandle = sdfReadNamedResource(nameAddress, &loadedResource, 0);
    btlReplaceResourceHandle(descriptor, loadedResource);
    sdfReleaseResourceAllocation(allocationHandle);
}

/* Acquire a texture from the supplied resource, releasing an owned old handle. */
void btlReplaceResourceHandle(BtlResourceDescriptor *descriptor, s32 textureResource) {
    s32 textureHandle = descriptor->handle;
    if (textureHandle != 0 && descriptor->ownsHandle == 1) {
        sdfTexReleaseReferenceViaHandler(textureHandle);
        descriptor->handle = 0;
    }
    descriptor->handle = sdfTexAcquireResourceTexture(textureResource);
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020E1E0);

/* Allocate the native name record and copy the initial text at byte 0x1C.
 * Preserve the raw address ABI and the constructor's individual field writes. */
s32 btlCreateResourceNameRecord(s32 nameAddress) {
    s32 recordAddress;

    recordAddress = (s32)sdfAllocSizeClassBlock(BTL_RESOURCE_NAME_RECORD_BYTES);
    ((BtlResourceNameRecord *)recordAddress)->word14 = 9;
    ((BtlResourceNameRecord *)recordAddress)->word00 = 8;
    ((BtlResourceNameRecord *)recordAddress)->word04 = 8;
    ((BtlResourceNameRecord *)recordAddress)->word08 = 0;
    ((BtlResourceNameRecord *)recordAddress)->nameLength = 0;
    ((BtlResourceNameRecord *)recordAddress)->word0C = 0;
    ((BtlResourceNameRecord *)recordAddress)->word18 = 0;
    strcpy(recordAddress + 0x1c, nameAddress);
    return recordAddress;
}

void func_0020E368(void *allocation) {
    sdfReleaseChipBlock(allocation);
}

INCLUDE_ASM(const s32, "game/code_00207A38", func_0020E380);

void btlSetResourceNameHeaderPairAlternate(s32 recordAddress, s32 firstWord, s32 secondWord) {
    ((BtlResourceNameRecord *)recordAddress)->word00 = firstWord;
    ((BtlResourceNameRecord *)recordAddress)->word04 = secondWord;
}

u32 func_0020E7B0(s32 recordAddress) {
    return ((BtlResourceNameRecord *)recordAddress)->word08;
}

/* Copy text at native byte 0x21 and record its length, unlike the constructor's 0x1C copy. */
void btlResourceRecordSetName(char *recordBytes, char *text) {
    strcpy(recordBytes + 0x21, text);
    ((BtlResourceNameRecord *)recordBytes)->nameLength = strlen(text);
}

void btlFormatResourceNameWithPrefix(s32 recordAddress, void *output) {
    func_0035C860(output, D_00436C50, recordAddress + 0x21, recordAddress + 0x1c);
}

void btlFormatResourceNameWithoutPrefix(s32 recordAddress, void *output) {
    func_0035C860(output, D_00436C58, recordAddress + 0x21);
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

