#include "btl_command.h"
#include "kwln_sprite.h"
#include "common.h"
#include "dds3obj.h"
#include "evt_unit.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "sdf_packet_builders.h"
#include "sdf_texture_draw_packet.h"
#include "btl_scene_fade.h"
#include "btl_resource_browser.h"
#include "eff_ref_obj.h"
#include "sdf_resource.h"
#include "btl_state.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"
#include "kwln.h"
#include "scr.h"
#include "sdf.h"
#include "btl_resource_name.h"
#include "dat_command.h"
#include "sce_io.h"
#include "fpu.h"
#include "sdf_texture_file.h"

extern s32 func_003101B8(s32 directory);
extern s32 func_00310320(s32 directory, SceDirent *entry);

extern void sdfTexReleaseReferenceViaHandler(SdfTex *texture);

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
    /* High-bit selectors are game-specific callback-table positions. */
    BTL_PACKED_ENEMY_COUNT_LIMIT = 0x1400000,
    BTL_PACKED_ENEMY_QUEUED_QUERY = 0x0F800000,
    BTL_PACKED_PARTY_QUEUED_QUERY = 0x0FC00000,
    BTL_PACKED_PARTY_ELEMENT_BLOCK = 0x0F400000,
    BTL_PACKED_PARTY_EMPTY_MP = 0x10000000
};

extern void btlClearUnitDefeatCandidate(BtlUnit *unit);

extern f32 func_001F6E28(BtlIndexList *list, f32 *maxTop, f32 *minTop);
extern void btlFlagUnitDefeatCandidate(BtlUnit *unit);

extern s32 D_00360348[];
extern s32 D_0035FFE0[];
extern s32 D_003BB6B8;
extern s32 D_003BD854;
extern void btlCmdSimpleB(s32, s32);
extern void btlCmdSimpleI(s32, s32);
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


extern s32 sceDopen(char *);

extern s32 D_003BD868;

extern s8 D_003A54A8[];

extern void btlBossDebugPrintf(const char *format, ...);

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

extern s32 btlReadCurrentUnitHp(DatPartyRecord *);
extern u32 btlComputeSkillAdjustedMaxHp(DatPartyRecord *);

extern void btlGetUnitWorldPos(BtlUnit *, f32 *);
extern void effMiscQuaternionToMatrixVU(void);

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


extern u16 scrReadIntParameter(u32);


extern u32 btlButtonIconTexture;


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

extern f32 sdfViewTargetVector[];
extern f32 sdfViewMatrix[];
extern f32 sdfProjectionMatrix[];
extern f32 D_00324660[];
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
    VU0_LOAD_VF(vf11, D_00324660);
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
    VU0_LOAD_VF(vf11, D_00324660);
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
    if (!(unit->status.flags & 2)) {
        btlUnitGetMuzzlePosVU(unit);
        return;
    }
    effObjFetchInnerPosition(unit->effectObject);
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
    VU0_STORE_VF(vf10, position);
    return unit->height * unit->scale * 0.5f - position[1];
}


/* Lower Y bound uses the same negated position Y and scaled half-height. */
f32 btlUnitGetBottomY(BtlUnit *unit) {
    f32 position[4];
    btlUnitGetMuzzlePosVU(unit);
    VU0_STORE_VF(vf10, position);
    return -position[1] - unit->height * unit->scale * 0.5f;
}


INCLUDE_ASM(const s32, "game/code_001F6110", func_001F66D8);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F6970);

/* Maximum top bound among active units matching any mask bit; zero if none. */
f32 btlGetMaxUnitTop(u32 mask) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    f32 maxTopY = 0.0f;
    s32 needsFirstSample = 1;
    while (unit != NULL) {
        if ((unit->status.flags & 1) && (unit->status.flags & mask)) {
            f32 topY = btlUnitGetTopY(unit);
            if (needsFirstSample) {
                maxTopY = topY;
                needsFirstSample = 0;
            } else if (maxTopY < topY) {
                maxTopY = topY;
            }
        }
        unit = unit->next;
    }
    return maxTopY;
}



/* Maximum scaled reach among active units matching any mask bit; zero if none. */
f32 btlGetMaxUnitReach(u32 mask) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    f32 maxReach = 0.0f;
    s32 needsFirstSample = 1;
    while (unit != NULL) {
        if ((unit->status.flags & 1) && (unit->status.flags & mask)) {
            f32 scaledReach = unit->reach * unit->scale;
            if (needsFirstSample) {
                maxReach = scaledReach;
                needsFirstSample = 0;
            } else if (maxReach < scaledReach) {
                maxReach = scaledReach;
            }
        }
        unit = unit->next;
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
        if ((unit->status.flags & 1) && (unit->status.flags & mask)) {
            btlUnitGetMuzzlePosVU(unit);
            VU0_STORE_VF(vf10, position);
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
        unit = unit->next;
    }
    return extremeZ;
}


INCLUDE_ASM(const s32, "game/code_001F6110", func_001F6E28);

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
    VU0_STORE_VF(vf10, &targetPos);
    unit = state->units;
    nearest = NULL;
    needsFirstSample = 1;
    nearestDistance = 0.0f;
    while (unit != NULL) {
        if (unit->status.flags & 1) {
            if (!(unit->status.flags & 0xC0)) {
                if (target != unit) {
                    if (unit->status.flags & mask) {
                        btlUnitGetMuzzlePosVU(unit);
                        VU0_SCALAR_OP(targetPos.f[1], "vaddx.y vf10, vf0, vf2x");
                        VU0_LOAD_VF($vf11, &targetPos);
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
        unit = unit->next;
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
        if (unit->status.flags & 1) {
            if (!(unit->status.flags & 0xC0)) {
                if (unit->status.flags & mask) {
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
        btlUnitGetMuzzlePosVU(unit);
        if (selected == NULL) {
            selected = unit;
            VU0_STORE_VF(vf10, &selectedPosition);
        } else {
            VU0_STORE_VF(vf10, &position);
            if (reference->status.flags & 0x200) {
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
        btlFlagUnitDefeatCandidate(unit);
    }
}

void btlClearAllUnitDefeatCandidates(void) {
    BtlUnit *unit;

    for (unit = ((BtlState *)btlGetRuntime())->units; unit != NULL; unit = unit->next) {
        btlClearUnitDefeatCandidate(unit);
    }
}

void btlFlagMatchingUnitsDefeatCandidate(s32 mask) {
    BtlUnit *unit;

    unit = ((BtlState *)btlGetRuntime())->units;
    if (unit != NULL) {
        do {
            if (unit->status.flags & mask) {
                btlFlagUnitDefeatCandidate(unit);
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
            if (unit->status.flags & mask) {
                btlClearUnitDefeatCandidate(unit);
            }
            unit = unit->next;
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


extern s32 btlIsActorModeAcceptedByBattleHook(BtlUnit *object);
extern void btlSetUnitPosition(BtlUnit *object, void *position);
extern void btlSetUnitRotation(BtlUnit *object, void *rotation);
extern void btlRefreshUnitMotionSelection(BtlUnit *unit);
extern void btlApplyUnitMotionSelection(BtlUnit *, u32, s32, f32);

/* Refresh each unit's transform/effect state, then invoke the runtime callback. */
void btlUpdateUnitActors(void) {
    BtlState *state = (BtlState *)btlGetRuntime();
    BtlUnit *unit = state->units;
    while (unit != NULL) {
        btlFlagUnitDefeatCandidate(unit);
        btlSetUnitPosition(unit, unit->position);
        btlSetUnitRotation(unit, unit->rotation);
        if ((btlIsActorModeAcceptedByBattleHook(unit) == 0 && unit->effectState != 0) ||
            (unit->updateFlags & 2) != 0) {
            btlRefreshUnitMotionSelection(unit);
            unit->effectTimerA = 0;
            unit->effectTimerB = 0;
            btlApplyUnitMotionSelection(unit, unit->effectArgA, unit->effectArgB,
                          unit->effectValue);
        }
        unit->status.stateFlags &= ~0x8000;
        unit = unit->next;
    }
    {
        void (*callback)(void) = state->updateCallback;
        if (callback != 0) {
            callback();
        }
    }
}


void btlRefreshUnitEffects(void) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    u32 handle;
    while (unit != NULL) {
        if (unit->status.flags & 2) {
            if (unit->status.stateFlags & 0x10) {
                handle = (u32)unit->ext;
                evtSetUnitStatusFlags((EvtUnit *)handle);
                VU0_LOAD_VF(vf10, unit);
                evtSetUnitNormalizedDirection((EvtUnit *)handle, 0);
            }
        }
        unit = unit->next;
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
            unitFlags = unit->status.flags;
            if (((unitFlags & mask) != 0) && ((unitFlags & 0x20) == 0)) {
                activeCount += unitFlags & 1;
            }
            unit = unit->next;
        } while (unit != NULL);
    }
    return activeCount;
}

extern void sdfConvertEulerAnglesToQuaternionVU(f32, f32, f32);
extern f32 func_002FA1F0(f32, f32);
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);
extern f32 sdfSinPoly(f32);
extern u128 D_0035F9E0;

/* Aim along the horizontal origin-to-target delta; zero delta loads the default vf10. */
s32 btlAimHorizontalDirectionVU(f32 *origin, f32 *targetPosition) {
    f32 delta[4];
    delta[0] = targetPosition[0] - origin[0];
    delta[2] = targetPosition[2] - origin[2];
    if (delta[0] != 0.0f || delta[2] != 0.0f) {
        sdfConvertEulerAnglesToQuaternionVU(0.0f, func_002FA1F0(delta[0], delta[2]), 0.0f);
        return 1;
    }
    VU0_LOAD_VF($vf10, &D_0035F9E0);
    return 0;
}


s32 btlAimHorizontalDirectionClampedVU(f32 *origin, f32 *targetPosition, f32 angle) {
    f32 delta[4];
    f32 sine;
    f32 tangent;
    f32 x;

    delta[0] = targetPosition[0] - origin[0];
    delta[2] = targetPosition[2] - origin[2];
    if (delta[0] != 0.0f || delta[2] != 0.0f) {
        sine = sdfSinPoly(angle);
        tangent = sine / sdfEvaluateCosineViaSinePhaseShift(angle);
        x = tangent * delta[2];
        if (ffabsf(delta[0]) < ffabsf(x)) {
            x = delta[0];
        } else if (0.0f <= delta[0]) {
            x = ffabsf(x);
        } else {
            x = -ffabsf(x);
        }
        sdfConvertEulerAnglesToQuaternionVU(0.0f, func_002FA1F0(x, delta[2]), 0.0f);
        return 1;
    }
    return 0;
}

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
    VU0_STORE_VF($vf10, direction);
    VU0_LOAD_VF($vf10, vertexA);
    VU0_LOAD_VF($vf11, vertexC);
    VU0_SUB(vf10, vf10, vf11);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF($vf10, direction);
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
f32 btlProjectOnPlaneVU(f32 *vertexA, f32 *vertexB, f32 *vertexC) {
    f32 direction[4];
    f32 projection;
    btlTriangleNormalVU(vertexA, vertexB, vertexC);
    VU0_STORE_VF(vf10, direction);
    VU0_LOAD_VF(vf10, vertexA);
    VU0_LOAD_VF(vf11, vertexC);
    VU0_SUB(vf10, vf10, vf11);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, direction);
    VU0_DOT_XYZ(projection, vf10, vf11);
    VU0_SCALAR_OP(projection, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_LOAD_VF(vf11, vertexC);
    VU0_ADD(vf10, vf10, vf11);
    return projection;
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
    VU0_MOVE_VF(vf11, vf10);
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



/* Store the exponential span and clear its progress. */
void btlScalarRangeSetStartClearEnd(BtlExponentialRange *state, f32 start) {
    state->start = start;
    state->end = 0;
}

/* Advance progress by (1 - progress) / span and return at most one.
   Only the return is capped: stored progress is raw. A nonpositive span is unchanged. */
f32 btlScalarRangeStepExponential(BtlExponentialRange *state) {
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
void btlScalarRangeInitQuadratic(BtlScalarRange *state, f32 start) {
    f32 initialValue;

    state->velocity = 0.0f;
    state->start = start;
    initialValue = state->velocity;
    state->end = start;
    state->value = initialValue;
    if (start == initialValue) {
        return;
    }
    state->inverseSpan = 1.0f / (start * start * 0.25f);
}

/* Integrate the quadratic accumulator, reversing acceleration after the midpoint.
   Completion returns one without updating state; intermediate values are not capped. */
f32 btlScalarRangeStepQuadratic(BtlScalarRange *state, f32 timeStep) {
    f32 accumulatedValue = state->value;
    f32 velocity = state->velocity;
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
    state->velocity = velocity;
    accumulatedValue += velocity * timeStep;
    state->value = accumulatedValue;
    return accumulatedValue;
}

extern s32 D_0035F9F0[16];
struct RefObj;
extern SdfTex *func_0029C048(void *, struct RefObj *);
extern s32 sdfAllocPacketAligned(s32);

/* Draw an indexed 64-pixel glyph with independent corner colors. */
void func_001F7DF8(SdfPoolNode *surface, s32 x, s32 y,
                   s32 topLeftColor, s32 topRightColor,
                   s32 bottomLeftColor, s32 bottomRightColor,
                   s32 buttonIndex) {
    BtlState *battle;
    SdfTex *texture;
    SdfListHead *packet;
    s32 uvIndex;
    s32 xFixed;
    s32 yFixed;

    buttonIndex &= 0xFFFF;
    battle = (BtlState *)btlGetRuntime();
    texture = func_0029C048(surface, (struct RefObj *)battle->buttonTextureHandle);
    packet = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(packet);
    sdfConsCreateDrawPacket(packet, texture, 0);

    xFixed = x * 16;
    yFixed = y * 8;
    uvIndex = buttonIndex * 2;
    sdfQueueGouraudTexturedQuad(packet, 0x40,
        xFixed + 0x7000, yFixed + 0x7900,
        D_0035F9F0[uvIndex] * 16, D_0035F9F0[uvIndex + 1] * 16, topLeftColor,
        xFixed + 0x7400, yFixed + 0x7900,
        D_0035F9F0[uvIndex] * 16 + 0x400, D_0035F9F0[uvIndex + 1] * 16, topRightColor,
        xFixed + 0x7000, yFixed + 0x7B00,
        D_0035F9F0[uvIndex] * 16, D_0035F9F0[uvIndex + 1] * 16 + 0x400, bottomLeftColor,
        xFixed + 0x7400, yFixed + 0x7B00,
        D_0035F9F0[uvIndex] * 16 + 0x400, D_0035F9F0[uvIndex + 1] * 16 + 0x400, bottomRightColor,
        0xFF0000, 0);
    surface->append(surface, packet);
}

typedef struct BtnUv {
    s32 u;
    s32 v;
} BtnUv;


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
        texture = func_0029C048(surface, (struct RefObj *)((BtlState *)btlGetRuntime())->buttonTextureHandle);
        packet = (SdfListHead *)sdfAllocPacketAligned(0x20);
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
        surface->append(surface, packet);
    }
}

void btlOpenButtonIconResource(void) {
    btlBossDebugPrintf("btl:[%s]\n", D_003BB6B8);
    D_003BD854 = sdfReadNamedResource(D_003BB6B8, &btlButtonIconTexture, 0);
}

/* Cache and later release the loaded battle resource in battle state. */
void btlRetainButtonTexture(void) {
    BtlState *state;
    u32 resource;

    state = (BtlState *)btlGetRuntime();
    resource = (u32)effCloneSharedReferenceWithValue((struct SdfTextureFileHeader *)btlButtonIconTexture, 0x10000);
    state->buttonTextureHandle = resource;
}

void btlReleaseButtonTexture(void) {
    BtlState *state;

    state = (BtlState *)btlGetRuntime();
    effReleaseSharedReference((RefObj *)state->buttonTextureHandle);
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

extern s32 func_001A8A30(void *, s32);
extern s32 func_001A3CE0(void *, s32, BtlActionProbe *);

/* Resolve a command-table entry through its probe, or store the direct command id. */
u32 btlScriptSelectActionEntry(void) {
    BtlCmdCtx *context;
    BtlActionProbe probe;
    s32 commandId;

    context = (BtlCmdCtx *)scrGetCurrentCommandWork();
    commandId = scrReadIntParameter(0);
    if (datCommandSelectors[commandId].kind == 1) {
        if (func_001A8A30(context->unit, commandId) != 0 &&
            func_001A3CE0(context->unit, commandId, &probe) != 0) {
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

u32 btlScriptSetCommandValueAndParameter(void) {
    s32 context = scrGetCurrentCommandWork();
    u16 commandId = scrReadIntParameter(0);
    u32 parameterValue = scrReadIntParameter(1);
    ((BtlCmdCtx *)context)->commandMode = 2;
    ((BtlCmdCtx *)context)->commandValue = commandId;
    ((BtlCmdCtx *)context)->parameter8C = parameterValue;
    ((BtlCmdCtx *)context)->parameter38 = parameterValue;
    return 1;
}

u32 btlCmdSetContextFlagOne(void) {
    s32 context;

    context = scrGetCurrentCommandWork();
    ((BtlCmdCtx *)context)->flags = ((BtlCmdCtx *)context)->flags | 1;
    return 1;
}

u32 btlScriptSetActorUnitParameter(void) {
    u16 parameterValue;
    s32 context;

    context = scrGetCurrentCommandWork();
    parameterValue = scrReadIntParameter(0);
    ((BtlCmdCtx *)context)->unit->partyRecord.affinityTableIndex = parameterValue;
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
/* Remember the count limit only when the eligible enemy count is at most that limit.
   Failure clears the selection flag but leaves the previous stored choice intact. */
u32 btlCmdStoreEnemyCountCheckChoice(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | BTL_PACKED_ENEMY_COUNT_LIMIT)) {
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

/* Report an eligible matching unit at or below the scripted HP percentage.
   Preserve the native signed products and their unsigned comparison. */
u32 btlCmdCheckHpPercent(void) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    s32 sideChoice = scrReadIntParameter(0);
    s32 lookupId = scrReadIntParameter(1);
    s32 hpPercentThreshold = scrReadIntParameter(2);
    u32 sideMask = 0x200;
    if (sideChoice) {
        sideMask = 0x400;
    }
    while (unit != NULL) {
        if ((unit->status.flags & 1) && (unit->status.flags & sideMask) && !(unit->status.flags & 0x20) && unit->identity == lookupId) {
            DatPartyRecord *unitStats = &unit->partyRecord;
            s32 currentHp = btlReadCurrentUnitHp(unitStats);
            s32 maximumHp = btlComputeSkillAdjustedMaxHp(unitStats);
            if (!((u32)(maximumHp * hpPercentThreshold) < (u32)(currentHp * BTL_HP_PERCENT_SCALE))) {
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

/* Remember the choice when an enemy's queued query matches; failure clears only its flag. */
u32 btlCmdRememberUpperQueuedQuery(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | BTL_PACKED_ENEMY_QUEUED_QUERY)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 0x4000;
        ((BtlCommandContext *)context)->choicesB[15] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~0x4000;
    }
    return 1;
}

/* Remember the choice when a party unit's queued query matches; failure clears only its flag. */
u32 btlCmdRememberLowerQueuedQuery(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | BTL_PACKED_PARTY_QUEUED_QUERY)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 0x8000;
        ((BtlCommandContext *)context)->choicesB[16] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~0x8000;
    }
    return 1;
}

/* Remember the choice on a successful party-element block check; failure clears only its flag. */
u32 btlCmdRememberLowerElementBlock(void) {
    s32 context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, scrReadIntParameter(0) | BTL_PACKED_PARTY_ELEMENT_BLOCK)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandContext *)context)->selectionFlagsB |= 0x2000;
        ((BtlCommandContext *)context)->choicesB[14] = scrReadIntParameter(0);
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandContext *)context)->selectionFlagsB &= ~0x2000;
    }
    return 1;
}

/* Report whether an eligible party unit has zero current MP; the handler tests unit +0x12A. */
u32 btlCmdReportPartyEmptyMp(void) {
    s32 context;

    context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch(context, ((BtlCommandContext *)context)->actor, BTL_PACKED_PARTY_EMPTY_MP) != 0) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 btlScriptReturnSceneSlotCount(void) {
    s32 result;

    result = btlCountSceneSlots();
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
extern void itfMesSetTextSlotFromValue(s32, s32, s32, s32);

void btlBindActorSlot(BtlActor *actor, s32 taskArg) {
    BtlState *state = (BtlState *)btlGetRuntime();
    s32 slot = scrCreateTaskForProcessId((s32)state->scriptOwner->priority - 1, D_003BAAA8, taskArg);
    s32 handle;
    scrSetCurrentActor((KwlnTask *)slot, actor);
    handle = ((ScrData *)kwlnTaskGetUserValue((KwlnTask *)slot))->resourceIndex;
    if (handle >= 0) {
        BtlUnit *unit = actor->unit;
        itfMesSetTextSlotFromValue(handle, 0, unit->partyRecord.unitId, (unit->partyRecord.flags & 0x20) ? 1 : 2);
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
            if ((actor->status.flags & 2) != 0) {
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

/* Retail discards debug output but retains the format-string/varargs ABI. */
void btlBossDebugPrintf(const char *format, ...) {
}

void btlBossDebugPrintfN(s32 arg0, s32 arg1, s32 arg2, s32 arg3, ...) {
}

void func_001FB130(s32 x, s32 y, s32 style, const char *text) {
}

void func_001FB138(s32 x, s32 y, s32 width, s32 height) {
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

void func_001FB168(f32 *position) {
}

void func_001FB170(void) {
}

void func_001FB178(f32 *position, u32 color, s32 flags, f32 radius) {
}

void func_001FB180(void) {
}

void func_001FB188(void) {
}

void func_001FB190(void) {
}

void func_001FB198(f32 *from, f32 *to, u32 color) {
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

void func_001FB1E0(void *context) {
    (void)context;
}

void func_001FB1E8(void) {
}

void func_001FB1F0(void *context) {
    (void)context;
}


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

s32 btlOpenPfsDebugDirectory(const char *directoryName) {
    char pathBuffer[BTL_DIRECTORY_PATH_BYTES];

    /* Debug mode opens a formatted pfs0 path; built-in mode resets name iteration. */

    if (sdfPfsDebugMode != 0) {
        func_003014F0(pathBuffer, "pfs0:/%s", directoryName);
        return sceDopen(pathBuffer);
    }
    D_003BD868 = 0;
    return 0;
}

/* Close the scanned directory only in debug filesystem mode; ignore the native result. */
void btlClosePfsDebugDirectory(s32 directoryHandle) {
    if (sdfPfsDebugMode == 0) {
        return;
    }
    func_003101B8(directoryHandle);
}


extern char *D_00360380[];

/* Debug mode delegates to the native reader. Built-in mode returns each name's length,
   clears the directory-type bit, and returns zero when the eight-name table is exhausted. */
s32 btlReadBattleResourceDirectoryEntry(s32 directoryHandle, SceDirent *reader) {
    if (sdfPfsDebugMode != 0) {
        return func_00310320(directoryHandle, reader);
    }
    if ((u32)D_003BD868 >= BTL_BUILTIN_NAME_COUNT) {
        return 0;
    }
    strcpy(reader->name, D_00360380[D_003BD868]);
    reader->stat.mode &= ~BTL_DIRECTORY_FLAG_CLEAR;
    D_003BD868++;
    return strlen(reader->name);
}


INCLUDE_ASM(const s32, "game/code_001F6110", btlScanDirectory);

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
void btlAppendEntry(BtlResourceEntryList *list, const char *name, s32 category, s32 value, s32 id) {
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
 * The native constructor leaves textureCategory untouched. */
BtlResourceDescriptor *btlCreateResourceDescriptor(BtlResourceEntryList *list) {
    BtlResourceDescriptor *descriptor = sdfAllocSizeClassBlock(BTL_RESOURCE_DESCRIPTOR_BYTES);

    descriptor->originX = 8;
    descriptor->originY = 8;
    descriptor->selectionStatus = BTL_RESOURCE_SELECTION_PENDING;
    descriptor->entryCount = list->count;
    descriptor->word10 = 0;
    descriptor->selectedIndex = 0;
    descriptor->visibleIndex = 0;
    descriptor->previewActive = 0;
    descriptor->repeatDelay = 0;
    descriptor->drawSurfaceIndex = 0x60;
    descriptor->borderColor = 0x80806020;
    descriptor->fillColor = 0x60000000;
    descriptor->firstVisibleEntry = list->head;
    descriptor->selectedEntry = list->head;
    descriptor->cachedEntry = NULL;
    descriptor->texture = 0;
    descriptor->entryList = list;
    return descriptor;
}

extern u8 sdfPadButtonStates[0x20];
extern s8 D_0039862B[];
extern SdfPoolNode kwlnDrawSurfaces[];
extern void *func_0011D3E8(s32, s32, s32, s32, s32, u32, u32);
extern SdfTex *effGetBillResourceTexture(s32);
s32 btlFormatSelectedResourceName(BtlResourceDescriptor *, char *);
void btlLoadAndReplaceResourceHandle(BtlResourceDescriptor *, const char *);
void btlReplaceResourceHandle(BtlResourceDescriptor *, void *);

/* Update list selection and submit the visible browser rows to its surface. */
s32 btlUpdateAndDrawResourceBrowser(BtlResourceDescriptor *descriptor) {
    BtlResourceEntry *visibleEntry;
    SdfListHead *packetList;
    SdfPoolNode *surface;
    s32 drawnRows;
    s32 topRow;
    s32 contentRow;
    s32 packedY;

    if (descriptor->repeatDelay != 0) {
        descriptor->repeatDelay--;
    } else if (descriptor->firstVisibleEntry == NULL) {
        if (D_0039862B[0] < 0) {
            descriptor->selectionStatus = BTL_RESOURCE_SELECTION_CANCELED;
        }
    } else if (descriptor->selectionStatus == BTL_RESOURCE_SELECTION_PENDING) {
        BtlResourceEntry *navigationEntry;
        BtlResourceEntry *selectedEntry;
        s32 navigationStep;
        if ((sdfPadButtonStates[6] & 2) != 0) {
            if (descriptor->selectedEntry->prev != NULL) {
                descriptor->selectedEntry = descriptor->selectedEntry->prev;
                descriptor->selectedIndex--;
                if (descriptor->visibleIndex != 0) {
                    descriptor->visibleIndex--;
                }
                if (descriptor->selectedEntry == descriptor->firstVisibleEntry) {
                    if (descriptor->selectedEntry->prev != NULL) {
                        descriptor->firstVisibleEntry = descriptor->selectedEntry->prev;
                        descriptor->visibleIndex = 1;
                    }
                }
            }
        } else if ((sdfPadButtonStates[7] & 2) != 0) {
            if (descriptor->selectedEntry->next != NULL) {
                descriptor->selectedEntry = descriptor->selectedEntry->next;
                descriptor->selectedIndex++;
                descriptor->visibleIndex++;
                if (descriptor->visibleIndex == 0x11) {
                    if (descriptor->firstVisibleEntry->next != NULL) {
                        descriptor->firstVisibleEntry = descriptor->firstVisibleEntry->next;
                        descriptor->visibleIndex = 0x10;
                    }
                }
            }
        } else if ((sdfPadButtonStates[9] & 2) != 0) {
            selectedEntry = descriptor->selectedEntry;
            for (navigationStep = 0; navigationStep < 0x12; navigationStep++) {
                navigationEntry = selectedEntry->prev;
                if (navigationEntry != NULL) {
                    descriptor->selectedEntry = navigationEntry;
                    descriptor->selectedIndex--;
                    if (descriptor->visibleIndex != 0) {
                        descriptor->visibleIndex--;
                    }
                    selectedEntry = descriptor->selectedEntry;
                    if (selectedEntry == descriptor->firstVisibleEntry) {
                        BtlResourceEntry *previous = selectedEntry->prev;
                        if (previous != NULL) {
                            descriptor->firstVisibleEntry = previous;
                            descriptor->visibleIndex = 1;
                        }
                    }
                }
            }
        } else if ((sdfPadButtonStates[11] & 2) != 0) {
            for (navigationStep = 0; navigationStep < 0x12; navigationStep++) {
                navigationEntry = descriptor->selectedEntry->next;
                if (navigationEntry != NULL) {
                    descriptor->selectedEntry = navigationEntry;
                    descriptor->selectedIndex++;
                    descriptor->visibleIndex++;
                    if (descriptor->visibleIndex == 0x11) {
                        BtlResourceEntry *next = descriptor->firstVisibleEntry->next;
                        if (next != NULL) {
                            descriptor->firstVisibleEntry = next;
                            descriptor->visibleIndex = 0x10;
                        }
                    }
                }
            }
        } else if ((s8)sdfPadButtonStates[3] < 0) {
            descriptor->selectionStatus = BTL_RESOURCE_SELECTION_CANCELED;
        } else if ((s8)sdfPadButtonStates[1] < 0) {
            descriptor->selectionStatus = BTL_RESOURCE_SELECTION_ACCEPTED;
        } else {
            BtlResourceEntry *cachedEntry = descriptor->cachedEntry;
            selectedEntry = descriptor->selectedEntry;
            if (cachedEntry != selectedEntry) {
                s32 category;
                category = selectedEntry->category;
                switch (category) {
                case BTL_RESOURCE_ENTRY_CATEGORY_TMX:
                    if (selectedEntry->id == 0) {
                        char resourceName[0x70];
                        btlFormatSelectedResourceName(descriptor, resourceName);
                        btlLoadAndReplaceResourceHandle(descriptor, resourceName);
                    } else {
                        btlReplaceResourceHandle(descriptor, (void *)(u32)selectedEntry->id);
                    }
                    selectedEntry = descriptor->selectedEntry;
                    descriptor->previewActive = descriptor->textureCategory = 1;
                    break;
                case BTL_RESOURCE_ENTRY_CATEGORY_GENERAL: {
                    SdfTex *texture = effGetBillResourceTexture(selectedEntry->value);
                    descriptor->textureCategory = category;
                    descriptor->texture = texture;
                    descriptor->previewActive = 1;
                    selectedEntry = descriptor->selectedEntry;
                    break;
                }
                case BTL_RESOURCE_ENTRY_CATEGORY_P2A:
                default:
                    descriptor->previewActive = 0;
                    break;
                }
                descriptor->cachedEntry = selectedEntry;
            }
            if (descriptor->previewActive != 0) {
                btlDrawResourcePreview(descriptor);
            }
        }
    }

    packetList = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(packetList);
    sdfAppendPacket(packetList, (u32)func_0011D3E8(
        (descriptor->originX << 4) + 0x7000,
        (descriptor->originY << 3) + 0x7900,
        0xFEFFFF, 0xCC0, 0x740, descriptor->fillColor, descriptor->borderColor));

    if (descriptor->previewActive != 0) {
        sdfAppendPacket(packetList, (u32)func_0011D3E8(
            (descriptor->originX << 4) + 0x7000,
            (descriptor->originY << 3) + 0x8050,
            0xFEFFFF, 0x800, 0x400, 0, descriptor->borderColor));
    }

    drawnRows = 0;
    topRow = descriptor->originY;
    contentRow = topRow + 2;
    if (descriptor->entryList->pathPrefix != NULL) {
        sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(
            (descriptor->originX << 4) + 0x7020,
            (contentRow << 3) + 0x7900, 0xFF0000, 5,
            descriptor->entryList->pathPrefix));
        contentRow = topRow + 0xE;
    }
    packedY = (contentRow << 3) + 0x7900;
    visibleEntry = descriptor->firstVisibleEntry;
    while (visibleEntry != NULL) {
        s32 selectionFlags = visibleEntry == descriptor->selectedEntry ? 4 : 0;
        sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(
            (descriptor->originX << 4) + 0x7020,
            packedY, 0xFF0000, selectionFlags, visibleEntry->name));
        drawnRows++;
        packedY += 0x60;
        if (drawnRows >= 0x12) {
            break;
        }
        visibleEntry = visibleEntry->next;
    }

    surface = &kwlnDrawSurfaces[descriptor->drawSurfaceIndex];
    surface->append(surface, packetList);
    return descriptor->selectionStatus;
}

/* Release an owned texture handle and the descriptor, but not its entry list. */
void btlDestroyResourceDescriptor(BtlResourceDescriptor *descriptor) {
    SdfTex *texture = descriptor->texture;
    if (texture != NULL && descriptor->textureCategory == BTL_RESOURCE_ENTRY_CATEGORY_TMX) {
        sdfTexReleaseReferenceViaHandler(texture);
    }
    sdfReleaseChipBlock(descriptor);
}

void btlSetResourceBrowserPosition(BtlResourceDescriptor *descriptor, s32 x, s32 y) {
    descriptor->originX = x;
    descriptor->originY = y;
}

u32 func_001FBF40(s32 recordAddress) {
    const u32 *recordWords = (const u32 *)recordAddress;
    return recordWords[5];
}

u32 btlGetResourceBrowserSelectionStatus(const BtlResourceDescriptor *descriptor) {
    return descriptor->selectionStatus;
}

/* Format prefix + selected name; return its resource category, not its id. */
s32 btlFormatSelectedResourceName(BtlResourceDescriptor *descriptor, char *output) {
    char *pathPrefix = descriptor->entryList->pathPrefix;
    if (pathPrefix != NULL) {
        func_003014F0(output, D_003BB818, pathPrefix, descriptor->selectedEntry->name);
    } else {
        func_003014F0(output, D_003BB820, descriptor->selectedEntry->name);
    }
    return descriptor->selectedEntry->category;
}

/* Strip from the first '.' onward; return the selected entry's category. */
s32 btlTrimResourceName(BtlResourceDescriptor *descriptor, char *output) {
    u32 nameLength;
    u32 characterIndex;
    func_003014F0(output, D_003BB820, descriptor->selectedEntry->name);
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

void btlReplaceResourceHandle(BtlResourceDescriptor *, void *);

/* Replace an owned old texture, acquire the named resource's texture,
 * then release the temporary allocation returned by the resource reader. */
void btlLoadAndReplaceResourceHandle(BtlResourceDescriptor *descriptor, const char *resourceName) {
    u32 loadedResource;
    SdfTex *texture = descriptor->texture;
    struct SdfMemBlock *allocation;
    if (texture != NULL && descriptor->textureCategory == BTL_RESOURCE_ENTRY_CATEGORY_TMX) {
        sdfTexReleaseReferenceViaHandler(texture);
        descriptor->texture = 0;
    }
    allocation = sdfReadNamedResource(resourceName, &loadedResource, 0);
    btlReplaceResourceHandle(descriptor, (void *)loadedResource);
    sdfReleaseResourceAllocation(allocation);
}

/* Acquire a texture from the supplied resource, releasing an owned old handle. */
void btlReplaceResourceHandle(BtlResourceDescriptor *descriptor, void *textureResource) {
    SdfTex *texture = descriptor->texture;
    if (texture != NULL && descriptor->textureCategory == BTL_RESOURCE_ENTRY_CATEGORY_TMX) {
        sdfTexReleaseReferenceViaHandler(texture);
        descriptor->texture = 0;
    }
    descriptor->texture = sdfTexAcquireResourceTexture((SdfTextureFileHeader *)(textureResource));
}

extern void *sdfConsAllocateColumnPacket(s32 loopCount);
extern s32 sdfConsMeasurePacketWithHeader(s32 packetAddress);

/* Queue the descriptor's preview texture as a quad at its screen position. */
void btlDrawResourcePreview(BtlResourceDescriptor *descriptor) {
    SdfListHead *list;
    void *sprite;
    KwlnSpriteVertex *vertex;
    SdfTex *texture;
    SdfPoolNode *surface;

    if (descriptor->texture != 0) {
        list = (SdfListHead *)sdfAllocPacketAligned(0x20);
        sdfInitPacketList(list);
        sprite = sdfConsAllocateColumnPacket(1);
        vertex = (KwlnSpriteVertex *)sdfConsMeasurePacketWithHeader((s32)sprite);
        vertex->a = 0x80;
        vertex->b = 0x80;
        vertex->g = 0x80;
        vertex->r = 0x80;
        texture = descriptor->texture;
        vertex->corner[1].u = texture->width << 4;
        vertex->corner[1].v = texture->height << 4;
        vertex->corner[0].u = 0;
        vertex->corner[0].v = 0;
        vertex->corner[1].flag = 0;
        vertex->corner[0].flag = 0;
        vertex->corner[0].x = (descriptor->originX << 4) + 0x7000;
        vertex->corner[0].y = ((descriptor->originY + 0xEA) << 3) + 0x7900;
        vertex->corner[0].mask = 0xFF0000;
        vertex->corner[1].x = vertex->corner[0].x + 0x800;
        vertex->corner[1].y = vertex->corner[0].y + 0x400;
        vertex->corner[1].mask = 0xFF0000;
        sdfConsCreateDrawPacket(list, texture, 0);
        sdfAppendPacket(list, (u32)sprite);
        surface = &kwlnDrawSurfaces[descriptor->drawSurfaceIndex];
        surface->append(surface, list);
    }
}


/* Allocate the native name record and initialize its header and extension. */
struct BtlResourceNameRecord *btlCreateResourceNameRecord(const char *extension) {
    struct BtlResourceNameRecord *record;

    record = (struct BtlResourceNameRecord *)sdfAllocSizeClassBlock(BTL_RESOURCE_NAME_RECORD_BYTES);
    record->maximumNameLength = 9;
    record->x = 8;
    record->y = 8;
    record->selectionStatus = BTL_RESOURCE_SELECTION_PENDING;
    record->nameLength = 0;
    record->selection = 0;
    record->overwritePromptActive = 0;
    strcpy(record->extension, extension);
    return record;
}

void func_001FC2E8(void *allocation) {
    sdfReleaseChipBlock(allocation);
}

INCLUDE_ASM(const s32, "game/code_001F6110", btlUpdateResourceNameEditor);

void btlSetResourceNamePosition(struct BtlResourceNameRecord *record, s32 x, s32 y) {
    record->x = x;
    record->y = y;
}

u32 btlGetResourceNameSelectionStatus(const struct BtlResourceNameRecord *record) {
    return record->selectionStatus;
}

/* Copy the resource name at +0x21 and store its length. */
void btlResourceRecordSetName(struct BtlResourceNameRecord *record, const char *name) {
    strcpy(record->resourceName, name);
    record->nameLength = strlen(name);
}

void btlFormatResourceNameWithExtension(struct BtlResourceNameRecord *record, char *output) {
    func_003014F0(output, D_003BB818, record->resourceName, record->extension);
}

void btlFormatResourceNameWithoutExtension(struct BtlResourceNameRecord *record, char *output) {
    func_003014F0(output, D_003BB820, record->resourceName);
}

void btlSetResourceNameLengthLimit(struct BtlResourceNameRecord *record, u32 maximumNameLength) {
    record->maximumNameLength = maximumNameLength;
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

