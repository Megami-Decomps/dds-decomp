#include "btl_command.h"
#include "btl_effect_position.h"
#include "kwln_sprite.h"
#include "common.h"
#include "btl_vec3.h"
#include "scr_integer_return_value_api.h"
#include "btl_marked_command_requested_state_api.h"
#include "btl_skill_max_api.h"
#include "btl_blend_color_api.h"
#include "btl_effect_active_api.h"
#include "kwln_task_add_child.h"
#include "btl_command_partner_output.h"
#include "sdf_trig.h"
#include "dds3obj.h"
#include "evt_unit.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "sdf_packet_builders.h"
#include "sdf_texture_draw_packet.h"
#include "btl_scene_fade.h"
#include "btl_resource_browser.h"
#include "eff_ref_obj.h"
#include "btl_task_condition.h"
#include "sdf_resource.h"
#include "bill_object_api.h"
#include "btl_state.h"
#include "btl_action.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"
#include "sdf_vu_transform_api.h"
#include "kwln.h"
#include "scr.h"
#include "scr_arg_read.h"
#include "sdf.h"
#include "btl_resource_name.h"
#include "dat_command.h"
#include "sce_io.h"
#include "fpu.h"
#include "sdf_texture_file.h"
#include "sdf_vu_matrix.h"

extern s32 sceDclose(s32 directory);
extern s32 sceDread(s32 directory, SceDirent *entry);

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
    BTL_RESOURCE_ENTRY_BYTES = sizeof(BtlResourceEntry),
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
extern u32 btlButtonIconTexture;



extern u16 scrReadIntParameter(u32);


extern s32 btlTrackedTaskHandles;

extern u64 btlCreateCommandSoundUpdateTask(void);

extern u64 btlCreateSecondaryCommandSoundTask(void);

extern u64 btlCreateCommandSoundTask(u64, u64);

extern u32 D_00436B00;

extern s32 D_00436AF0;

extern s32 D_00438F6C;


extern void btlBossDebugPrintf(const char *format, ...);

extern void btlPickCommandElementCheckedTarget(s32, s32);
extern void btlPickCommandTargetExcludingActor(s32, s32);

extern void btlPickCommandLowestHpBlockTarget(s32, u16);

extern void btlPickCommandTargetWithoutStatusMask(s32, u16);

extern void btlFilterCommandTargetsByUnitId(s32, u16);

extern void btlPickCommandElementBlockTarget(s32, u16);

extern s8 btlHistoryCounter;

extern void btlRunWeightedAiAction(s32, u16);

extern const char D_00419570[];

extern const char D_00419590[];

extern const char D_004195B8[];

extern const char D_004195D8[];

extern const char D_004195F8[];

extern const char D_00419620[];

extern const char D_00419648[];

extern const char D_00419668[];

extern const char D_00419688[];

extern s32 D_003BEA48[];

extern s32 D_003BE6E0[];

extern u8 sdfPfsDebugMode;

extern s32 sceDopen(char *);

extern s32 D_00438F80;

extern void sprintf();



extern u32 btlGetEffectValue(void);

extern u32 btlGetSubtaskTargetMode(void);

extern void btlPickCommandTargetWithStatusMask(s32, u16);

extern s32 btlDispatchPackedActionWithScratch(s32 context, s32 actor, u32 mask);

extern BtlRuntimeTask *btlAllocTask(s32);

extern s32 btlCommandRecenterParty(void);

extern void btlFlagUnitDefeatCandidate(BtlUnit *unit);

extern void btlClearUnitDefeatCandidate(BtlUnit *unit);


extern f32 btlMeasureUnitIndexListBounds(BtlIndexList *list, f32 *maxTop, f32 *minTop);

extern void btlChoosePartyDerivedShadowSkill(s32);

extern void btlCmdAppendCurrentUnitIdToCommandIndices(s32);

extern void btlSelectCommandTargetsByMode(s32, u16);

extern void btlSetSpecialBattleEffectActorByte(u8);

extern s32 btlIsLinkedActionSceneStateActive(void);

extern BtlRuntimeTask *btlCreateFloatTask28(ActionStateLink *, f32, f32, f32, f32, f32, f32, f32, f32);

extern s32 btlScheduleContextReset(void);

extern f32 D_00452F90[];

extern f32 D_00452FB0[];

extern BtlRuntimeTask *btlCreateFloatTask29(ActionStateLink *, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

extern void evtConfigureUnitTransition(s32, s32);

extern u8 D_00436C50[];

extern u8 D_00436C58[];
void btlReplaceResourceHandle(BtlResourceDescriptor *, void *);

extern char *D_003BEA80[];


extern f32 btlTriangleNormalDotEdge(f32 *, f32 *, f32 *);

extern void btlGetUnitWorldPos(BtlUnit *, f32 *);


extern f32 btlUnitGetTopY(BtlUnit *);

typedef union BtlVec4 {
    f32 f[4];
    u128 q;
} BtlVec4;

extern s32 btlReadCurrentUnitHp(DatPartyRecord *);

extern s32 D_00435E7C;


BtlRuntimeTask *btlCreateControlObject(void) {
    BtlRuntimeTask *task;
    task = btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback = btlCommandRecenterParty;
    task->taskId = 0x66;
    task->onStart = NULL;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

extern f32 sdfViewTargetVector[];
extern f32 sdfViewMatrix[];
extern f32 sdfProjectionMatrix[];
extern f32 D_0037F660[];

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

INCLUDE_ASM(const s32, "game/code_00207A38", btlMeasureFilteredUnitGroupBounds);

INCLUDE_ASM(const s32, "game/code_00207A38", btlMeasureEffectTargetGroupBounds);

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
        if ((unit->status.flags & 1) && (unit->status.flags & mask)) {
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
        if ((unit->status.flags & 1) && (unit->status.flags & mask)) {
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

INCLUDE_ASM(const s32, "game/code_00207A38", btlMeasureUnitIndexListBounds);

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
        if (unit->status.flags & 1) {
            if (!(unit->status.flags & 0xC0)) {
                if (target != unit) {
                    if (unit->status.flags & mask) {
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
            if (unit->status.flags & mask) {
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
            if (unit->status.flags & mask) {
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

extern void btlRefreshUnitMotionSelection(BtlUnit *);
extern void btlApplyUnitMotionSelection(BtlUnit *, u32, s32, f32);
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
            btlRefreshUnitMotionSelection(unit);
            unit->unkF8 = 0;
            unit->unkFA = 0;
            btlApplyUnitMotionSelection(unit, unit->effectIndex, unit->effectParameter, unit->effectScale);
        }
        unit->status.stateFlags &= ~0x8000;
        unit->status.stateFlags &= ~0x200000;
        unit = unit->nextActor;
    }
    {
        void (*callback)(void) = state->afterUnitUpdate;
        if (callback != 0) {
            callback();
        }
    }
}



extern s32 btlGetEntryFlagsUnlessDisabled(DatPartyRecord *);
extern u32 kwlnDrawControlFlags;

/* Refresh active actor lighting and restore eligible enemy colors. */
void btlRefreshUnitEffects(void) {
    BtlState *state = (BtlState *)btlGetRuntime();
    BtlUnit *unit = state->units;
    struct EvtUnit *effect;

    while (unit != NULL) {
        if (unit->status.flags & 2) {
            if (unit->status.stateFlags & 0x10) {
                effect = unit->ext;
                evtSetUnitStatusFlags(effect);
                VU0_LOAD_VF(vf10, unit->lightDirection);
                evtSetUnitNormalizedDirection(effect, 0);
                if ((btlUnitStatusPair(unit) & 0x4E0) == 0x420) {
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
            unitFlags = unit->status.flags;
            if (((unitFlags & mask) != 0) && ((unitFlags & 0x20) == 0)) {
                activeCount += unitFlags & 1;
            }
            unit = unit->nextActor;
        } while (unit != NULL);
    }
    return activeCount;
}

extern f32 atan2f(f32, f32);
extern u128 D_003BE0D0;

/* Aim along the horizontal origin-to-target delta; zero delta loads the default vf10. */
s32 btlAimHorizontalDirectionVU(f32 *origin, f32 *targetPosition) {
    f32 delta[4];
    delta[0] = targetPosition[0] - origin[0];
    delta[2] = targetPosition[2] - origin[2];
    if (delta[0] != 0.0f || delta[2] != 0.0f) {
        sdfConvertEulerAnglesToQuaternionVU(0.0f, atan2f(delta[0], delta[2]), 0.0f);
        return 1;
    }
    VU0_LOAD_VF($vf10, &D_003BE0D0);
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
        sdfConvertEulerAnglesToQuaternionVU(0.0f, atan2f(x, delta[2]), 0.0f);
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
f32 btlProjectOnPlaneVU(f32 *vertexA, f32 *vertexB, f32 *vertexC) {
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

struct RefObj;
extern SdfTex *sdfGetOrCreateTextureForReference(void *, struct RefObj *);
#include "sdf_packet_alloc.h"
extern s32 D_003BE0E0[];

/* Draw an indexed glyph with independently sized geometry and fixed atlas UVs. */
void btlDrawIconAtSize(SdfPoolNode *surface, s32 x, s32 y,
                      s32 width, s32 height, s32 topLeftColor, s32 topRightColor,
                      s32 bottomLeftColor, s32 bottomRightColor,
                      u16 buttonIndex) {
    BtlState *battle;
    SdfTex *texture;
    SdfListHead *packet;
    s32 uvIndex;
    s32 xFixed;
    s32 yFixed;

    battle = (BtlState *)btlGetRuntime();
    texture = sdfGetOrCreateTextureForReference(surface, (struct RefObj *)battle->buttonTextureHandle);
    packet = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(packet);
    sdfConsCreateDrawPacket(packet, texture, 0);

    xFixed = x * 16;
    yFixed = y * 8;
    uvIndex = buttonIndex * 2;
    sdfQueueGouraudTexturedQuad(packet, 0x40,
        xFixed + 0x7000, yFixed + 0x7900,
        D_003BE0E0[uvIndex] * 16, D_003BE0E0[uvIndex + 1] * 16, topLeftColor,
        xFixed + width * 16 + 0x7000, yFixed + 0x7900,
        D_003BE0E0[uvIndex] * 16 + 0x400, D_003BE0E0[uvIndex + 1] * 16, topRightColor,
        xFixed + 0x7000, yFixed + height * 8 + 0x7900,
        D_003BE0E0[uvIndex] * 16, D_003BE0E0[uvIndex + 1] * 16 + 0x400, bottomLeftColor,
        xFixed + width * 16 + 0x7000, yFixed + height * 8 + 0x7900,
        D_003BE0E0[uvIndex] * 16 + 0x400, D_003BE0E0[uvIndex + 1] * 16 + 0x400, bottomRightColor,
        0xFF0000, 0);
    surface->append(surface, packet);
}


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
        texture = sdfGetOrCreateTextureForReference(surface, (struct RefObj *)((BtlState *)btlGetRuntime())->buttonTextureHandle);
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
    btlBossDebugPrintf("btl:[%s]\n", D_00436AF0);
    D_00438F6C = sdfReadNamedResource(D_00436AF0, &btlButtonIconTexture, 0);
}

void btlRetainButtonTexture(void) {
    BtlState *state;
    u32 handle;

    state = (BtlState *)btlGetRuntime();
    handle = (u32)effCloneSharedReferenceWithValue((struct SdfTextureFileHeader *)btlButtonIconTexture, 0x10000);
    state->buttonTextureHandle = handle;
}

void btlReleaseButtonTexture(void) {
    BtlState *state;

    state = (BtlState *)btlGetRuntime();
    effReleaseSharedReference((RefObj *)state->buttonTextureHandle);
    state->buttonTextureHandle = 0;
}

u32 btlScriptSelectEmptyCommand(void) {
    BtlCommandCtx *context;

    context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    context->commandMode = 1;
    context->commandValue = 0;
    return 1;
}

/* Select the fixed mode-6/value-0xC2 command and continue the script VM. */
u32 btlScriptSelectFixedCommand(void) {
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


extern s32 btlHasRequiredCommandAffinityPartners(void *, s32);
extern s32 func_001ACD10(void *, s32, BtlCommandPartnerOutput *);

/* Resolve a command-table entry through its probe, or store the direct command id. */
u32 btlScriptSelectActionEntry(void) {
    BtlCommandCtx *context;
    BtlCommandPartnerOutput probe;
    s32 commandId;

    context = (BtlCommandCtx *)scrGetCurrentCommandWork();
    commandId = scrReadIntParameter(0);
    if (datCommandSelectors[commandId].kind == 1) {
        if (btlHasRequiredCommandAffinityPartners((void *)context->actor, commandId) != 0 &&
            func_001ACD10((void *)context->actor, commandId, &probe) != 0) {
            context->actionProbeFirst = probe.firstPartner;
            context->commandMode = 3;
            context->commandValue = probe.commandId;
            context->actionProbeSecond = probe.secondPartner;
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
        selectionValue = btlChoosePartyMemberShadowId();
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

/* Select a permitted party-derived shadow skill, or retain the empty command. */
s32 btlScriptSelectShadowSkill(void) {
    btlChoosePartyDerivedShadowSkill(scrGetCurrentCommandWork());
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
    void *context;

    context = scrGetCurrentCommandWork();
    parameterValue = scrReadIntParameter(0);
    ((BtlUnit *)((BtlCommandCtx *)context)->actor)->partyRecord.affinityTableIndex = parameterValue;
    return 1;
}

u32 btlScriptSetBattleWorkParameter(void) {
    s32 battleAddress;
    s16 requestArgument;

    battleAddress = (s32)(u32)btlGetRuntime();
    scrGetCurrentCommandWork();
    requestArgument = scrReadIntParameter(0);
    ((BtlState *)battleAddress)->requestMode = 4;
    ((BtlState *)battleAddress)->requestArgument = requestArgument;
    return 1;
}

/* Select targets for the current command's action; return VM success. */
u32 btlScriptChooseActionTargets(void) {
    btlChooseCommandActionTargets(scrGetCurrentCommandWork());
    return 1;
}

/* Parameter 0 supplies the group element-check input; return VM success. */
u32 btlScriptPickElementCheckedTarget(void) {
    void *context = scrGetCurrentCommandWork();
    u16 value = scrReadIntParameter(0);
    btlPickCommandElementCheckedTarget((s32)(u32)context, value);
    return 1;
}

/* Select the current command's minimum nonzero-HP candidate; return VM success. */
u32 btlScriptPickLowestHpTarget(void) {
    btlPickCommandLowestHpTarget(scrGetCurrentCommandWork());
    return 1;
}

/* Select the current command's minimum HP-percentage candidate; return VM success. */
u32 btlScriptPickLowestHpRatioTarget(void) {
    btlPickCommandLowestHpRatioTarget(scrGetCurrentCommandWork());
    return 1;
}

/* Parameter 0 feeds lowest-HP element-block selection; return VM success. */
u32 btlScriptPickLowestHpBlockTarget(void) {
    void *context = scrGetCurrentCommandWork();
    u16 value = scrReadIntParameter(0);
    btlPickCommandLowestHpBlockTarget((s32)(u32)context, value);
    return 1;
}

/* Pick the current command's highest-MP target and return VM completion. */
u32 btlScriptPickHighestMpTarget(void) {
    btlPickCommandHighestMpTarget(scrGetCurrentCommandWork());
    return 1;
}

/* Run the current command's minimum-value target selector; return VM success. */
u32 btlScriptPickLowestLevelTarget(void) {
    btlPickCommandLowestLevelTarget(scrGetCurrentCommandWork());
    return 1;
}

/* Parameter 0 is a status-presence mask for command target selection; return VM success. */
u32 btlScriptPickTargetWithStatusMask(void) {
    void *context = scrGetCurrentCommandWork();
    u16 value = scrReadIntParameter(0);
    btlPickCommandTargetWithStatusMask((s32)(u32)context, value);
    return 1;
}

/* Parameter 0 is a status-absence mask for command target selection; return VM success. */
u32 btlScriptPickTargetWithoutStatusMask(void) {
    void *context = scrGetCurrentCommandWork();
    u16 value = scrReadIntParameter(0);
    btlPickCommandTargetWithoutStatusMask((s32)(u32)context, value);
    return 1;
}

/* Apply script parameter 0 as the target unit-ID filter; return VM success. */
u32 btlScriptFilterTargetsByUnitId(void) {
    void *context = scrGetCurrentCommandWork();
    u16 value = scrReadIntParameter(0);
    btlFilterCommandTargetsByUnitId((s32)(u32)context, value);
    return 1;
}

/* Separate builtin entry with the same action-target selection body; return VM success. */
u32 btlScriptChooseActionTargetsDuplicate(void) {
    btlChooseCommandActionTargets(scrGetCurrentCommandWork());
    return 1;
}

/* Run unlinked-party selection; the old F/G attribution was reversed in DDS2. */
u32 btlScriptPickUnlinkedPartyTarget(void) {
    btlPickUnlinkedCommandTarget(scrGetCurrentCommandWork(), 0);
    return 1;
}

/* Run linked-party selection; the old F/G attribution was reversed in DDS2. */
u32 btlScriptPickLinkedPartyTarget(void) {
    btlPickLinkedCommandTarget(scrGetCurrentCommandWork(), 0);
    return 1;
}

/* Append this command's own actor after target scan; return VM success. */
u32 btlScriptAppendActorToTargets(void) {
    btlAppendActorToCommandTargets(scrGetCurrentCommandWork(), 0);
    return 1;
}

/* Apply script parameter 0 as the command target-selection mode; return VM success. */
s32 btlScriptCmdSelectTargetsByMode(void) {
    void *context = scrGetCurrentCommandWork();
    btlSelectCommandTargetsByMode((s32)(u32)context, scrReadIntParameter(0));
    return 1;
}

/* Append the global effect actor to this command's targets; return VM success. */
u32 btlScriptAddEffectActorTarget(void) {
    btlAddEffectActorTargetToCommand(scrGetCurrentCommandWork());
    return 1;
}

/* Append the selected-or-current actor to the script command target list. */
s32 btlScriptAppendSelectedOrCurrentTarget(void) {
    btlCmdAppendCurrentUnitIdToCommandIndices(scrGetCurrentCommandWork());
    return 1;
}

/* Parameter 0 feeds element-block target selection; return VM success. */
u32 btlScriptPickElementBlockTarget(void) {
    void *context = scrGetCurrentCommandWork();
    u16 value = scrReadIntParameter(0);
    btlPickCommandElementBlockTarget((s32)(u32)context, value);
    return 1;
}

/* Prefer a non-actor target, retaining native self fallback; return VM success. */
u32 btlScriptPickTargetExcludingActor(void) {
    btlPickCommandTargetExcludingActor(scrGetCurrentCommandWork(), 0);
    return 1;
}

/* Remember the choice only when the actor satisfies the HP-percentage bound. */
s32 btlNbScriptCheckActorFlag(void) {
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

/* Remember the choice when the packed MP-percentage condition succeeds. */
s32 btlCmdStoreActorMpCheckChoice(void) {
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

/* Identical unregistered copy of the preceding MP-choice command. */
s32 btlCmdStoreActorMpCheckChoiceUnregistered(void) {
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

/* Remember the percentage choice when an eligible party unit reaches that HP proportion. */
s32 btlCmdRememberPartyHighHpChoice(void) {
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

/* Remember the actor entry-code-10 gate result; failure clears only its selection flag. */
u32 btlCmdRememberActorActionGate(void) {
    void *context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch((s32)(u32)context, ((BtlCommandCtx *)context)->actor, 0x5400000)) {
        scrSetIntegerReturnValue(1);
        ((BtlCommandCtx *)context)->selectionFlagsA |= 0x2000;
    } else {
        scrSetIntegerReturnValue(0);
        ((BtlCommandCtx *)context)->selectionFlagsA &= ~0x2000;
    }
    return 1;
}

u32 btlCmdReturnReadyWithoutTurnsCheck(void) {
    void *context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch((s32)(u32)context, ((BtlCommandCtx *)context)->actor, 0x6C00000)) {
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
    void *context;

    context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch((s32)(u32)context, ((BtlCommandCtx *)context)->actor, 0x8800000) != 0) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 btlCmdReturnActorEligibility(void) {
    void *context;

    context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch((s32)(u32)context, ((BtlCommandCtx *)context)->actor, 0xB000000) != 0) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

/* Report whether active-enemy and cumulative-spawn limits permit another enemy. */
u32 btlCmdReportEnemySpawnCapacity(void) {
    void *context;

    context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch((s32)(u32)context, ((BtlCommandCtx *)context)->actor, 0xB800000) != 0) {
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
    void *context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch((s32)(u32)context, ((BtlCommandCtx *)context)->actor, scrReadIntParameter(0) | 0x12C00000)) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 btlCmdReturnEnemyElementQueryCheck(void) {
    void *context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch((s32)(u32)context, ((BtlCommandCtx *)context)->actor, scrReadIntParameter(0) | 0x13000000)) {
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

/* Remember the actor entry-code condition choice only when its query succeeds. */
s32 btlCmdRememberActorEntryCondition(void) {
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
    void *context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch((s32)(u32)context, ((BtlCommandCtx *)context)->actor, scrReadIntParameter(0) | 0x13400000)) {
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

/* Remember the choice when an eligible party unit's queued action matches its category query. */
s32 btlCmdRememberPartyQueuedActionCategory(void) {
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

/* Report a queued party action with nonzero target type and options equal to two. */
u32 btlCmdReturnQueuedSpecialAction(void) {
    void *context;

    context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch((s32)(u32)context, ((BtlCommandCtx *)context)->actor, 0x10800000) != 0) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

/* Report extended bit 0x800000 on the actor, or an eligible enemy when requested. */
u32 btlCmdReturnExtendedStatusCheck(void) {
    void *context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch((s32)(u32)context, ((BtlCommandCtx *)context)->actor, scrReadIntParameter(0) | 0x10C00000)) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 btlCmdReturnPartyMissingFlagCheck(void) {
    void *context;

    context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch((s32)(u32)context, ((BtlCommandCtx *)context)->actor, 0x11000000) != 0) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 btlCmdReturnPartyHeldFlagCheck(void) {
    void *context;

    context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch((s32)(u32)context, ((BtlCommandCtx *)context)->actor, 0x11400000) != 0) {
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

/* Complete zero-operand builtin 435 without publishing a script result. */
u32 btlScriptBuiltin435NoOp(void) {
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
        if ((unit->status.flags & 1) && (unit->status.flags & sideMask) && !(unit->status.flags & 0x20) && unit->owner == lookupId) {
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

/* Complete zero-operand builtin 450 without publishing a script result. */
u32 btlScriptBuiltin450NoOp(void) {
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
    void *context = scrGetCurrentCommandWork();
    if (btlDispatchPackedActionWithScratch((s32)(u32)context, ((BtlCommandCtx *)context)->actor, BTL_PACKED_PARTY_EMPTY_MP)) {
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

u32 btlScriptReturnSceneSlotCount(void) {
    s32 result;

    result = btlCountSceneSlots();
    scrSetIntegerReturnValue(result);
    return 1;
}

/* Publish the battle turn count to the script integer result; return VM success. */
u32 btlScriptReturnTurnCount(void) {
    s32 battle;

    battle = (s32)(u32)btlGetRuntime();
    scrSetIntegerReturnValue(((BtlState *)battle)->turnCount);
    return 1;
}

/* Publish the actor's roster-record level, not an action timer; return VM success. */
u32 btlScriptReturnActorLevel(void) {
    void *context;

    context = scrGetCurrentCommandWork();
    scrSetIntegerReturnValue(((BtlUnit *)((BtlCommandCtx *)context)->actor)->partyRecord.level);
    return 1;
}

/* Complete zero-operand builtin 422 without publishing a script result. */
u32 btlScriptBuiltin422NoOp(void) {
    return 1;
}

/* Complete zero-operand builtin 437 without publishing a script result. */
u32 btlScriptBuiltin437NoOp(void) {
    return 1;
}

u32 btlScriptReturnEffectActive(void) {
    u32 active;

    active = btlGetEffectActive();
    scrSetIntegerReturnValue(active);
    return 1;
}

/* Report the requested byte of mode 0x314's marked-command state; return script success. */
u32 btlScriptReturnMarkedCommandRequestedState(void) {
    u32 result;

    result = btlGetMarkedCommandRequestedState();
    scrSetIntegerReturnValue(result);
    return 1;
}

u32 btlCmdReturnBattlePhase(void) {
    s32 battle;

    battle = (s32)(u32)btlGetRuntime();
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
    void *context;

    context = scrGetCurrentCommandWork();
    scrSetIntegerReturnValue(*(s8 *)((u8 *)context + 0x14E));
    return 1;
}

u32 btlScriptReturnGlobalDebugValue(void) {
    scrSetIntegerReturnValue(btlHistoryCounter);
    return 1;
}

/* Complete zero-operand builtin 451 without publishing a script result. */
u32 btlScriptBuiltin451NoOp(void) {
    return 1;
}

u32 btlCmdRunWeightedAiSelection(void) {
    btlRunRandomWeightedAiTableAction(scrGetCurrentCommandWork());
    return 1;
}

u32 btlScriptSetBattleCommand(void) {
    void *context = scrGetCurrentCommandWork();
    u16 value = scrReadIntParameter(0);
    btlRunWeightedAiAction((s32)(u32)context, value);
    return 1;
}

/* Forward the script parameter to mode 0x31B's stat selector and continue the VM. */
s32 btlScriptSetBossStatSelector(void) {
    btlSetSpecialBattleEffectActorByte(scrReadIntParameter(0));
    return 1;
}

u32 btlResetCommandContextAndSetStateFlag(void) {
    s32 state;
    void *context;

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
extern void itfMesSetTextSlotFromValue(s32, s32, s32, s32);

void btlBindActorSlot(s32 actor, s32 option) {
    s32 battle = (s32)(u32)btlGetRuntime();
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
        itfMesSetTextSlotFromValue(window, 0, ((BtlUnit *)unit)->partyRecord.unitId, width);
    }
    kwlnTaskAddChild(((BtlState *)battle)->scriptOwner, (KwlnTask *)task);
    ((BtlState *)battle)->boundTask = task;
}

void func_0020CE28(void) {
}

void func_0020CE30(void) {
}

void func_0020CE38(void) {
}

/* EFFECT EDIT menu callback requests the editor through the shared debug flags. */
u32 btlRequestEffectEditor(void) {
    D_00436B00 = D_00436B00 | 0x2000000;
    return 0;
}

/* Return the supplied value unchanged. */
u32 btlReturnInputValue(u32 value) {
    return value;
}

void func_0020CE60(void) {
}

void func_0020CE68(void) {
}

/* Empty callback registered by the POINT LIGHT debug menu. */
void btlPointLightMenuNoOp(void) {
}

void btlReleaseActiveUnitEffectsUnlessPaused(void) {
    s32 state = (s32)(u32)btlGetRuntime();
    if ((((BtlState *)state)->battleFlags & 0x40000000) == 0) {
        s32 actor = (s32)((BtlState *)state)->units;
        while (actor != 0) {
            if ((((BtlUnit *)actor)->status.flags & 2) != 0) {
                s32 effect = ((BtlUnit *)actor)->ext;
                if (effect != 0) {
                    evtConfigureUnitTransition(effect, 0);
                }
            }
            actor = (s32)((BtlUnit *)actor)->nextActor;
        }
    }
}

extern s8 sdfPadButtonStates[32];
extern f32 D_00436BA0;
extern f32 D_00436BA4;
extern u8 D_00436BA8;
extern f32 D_00452FF0[2][4];
extern f32 D_00453010[2][4];

/* Battle debug camera: one button saves the world camera's position and
 * rotation into one of two slots (alternating), the other toggles a camera
 * command built from both saved slots with a 40-degree field of view. The
 * debug-menu descriptors mirror the command's active bit. */
void func_0020CEF8(void) {
    BtlState *state = btlGetRuntime();
    EffWorldNode *camera;

    if (!(state->battleFlags & 2)) {
        return;
    }
    camera = dds3GetWorldCameraObject(dds3GetWorldObject());
    if (camera == NULL) {
        return;
    }
    if (sdfPadButtonStates[0] < 0) {
        if ((u8)(D_00436BA8 & 1) == 0) {
            PCP_COPY_VECTOR_UNCLOBBERED(D_00452FF0[0], camera->inner->position);
            PCP_COPY_VECTOR_UNCLOBBERED(D_00453010[0], camera->inner->rotation);
        } else {
            PCP_COPY_VECTOR_UNCLOBBERED(D_00452FF0[1], camera->inner->position);
            PCP_COPY_VECTOR_UNCLOBBERED(D_00453010[1], camera->inner->rotation);
        }
        D_00436BA8 ^= 1;
    } else if (sdfPadButtonStates[1] < 0) {
        if (state->cameraCommand.flags & 1) {
            state->cameraCommand.flags &= ~1;
        } else {
            f32 fov;

            btlInitMotionTransformFromVectors(&state->cameraCommand.frontCamera, D_00452FF0[0], D_00453010[0]);
            btlInitMotionTransformFromVectors(&state->cameraCommand.backCamera, D_00452FF0[1], D_00453010[1]);
            fov = D_00436BA0 * 0.0174532925f;
            state->cameraCommand.flags = 0x41;
            state->cameraCommand.motionParameter = D_00436BA4;
            state->cameraCommand.state = 0;
            state->cameraCommand.frontCamera.fov = fov;
            state->cameraCommand.backCamera.fov = fov;
        }
    }
    if (state->cameraCommand.flags & 1) {
        D_003BEA48[4] |= 0x80;
        D_003BE6E0[4] |= 0x80;
    } else {
        D_003BEA48[4] &= ~0x80;
        D_003BE6E0[4] &= ~0x80;
    }
}

/* Clear bit 0x80 in both battle debug-menu descriptors. */
void btlResetBattleDebugMenuFlags(void) {
    D_003BEA48[4] &= ~0x80;
    D_003BE6E0[4] &= ~0x80;
}

void func_0020D100(void) {
}

/* Empty hook between active actor updates and the finished-task sweep. */
void btlUpdateActiveFrameNoOp(void) {
}

/* Empty hook between event playback and battle-field presentation. */
void btlUpdateFieldPresentationNoOp(void) {
}

/* Empty hook at the end of battle-runtime initialization. */
void btlInitializeRuntimeNoOp(void) {
}

void func_0020D120(void) {
}

/* Retail discards debug output but retains the format-string/varargs ABI. */
void btlBossDebugPrintf(const char *format, ...) {
}

void btlBossDebugPrintfN(s32 a, s32 b, s32 c, const char *format, ...) {
}

/* Retail discards debug text while retaining the callers' four-argument ABI. */
void btlDebugDrawText(s32 x, s32 y, s32 style, const char *text) {
}

/* Retail discards debug rectangles; name-editor callers pass position and size. */
void btlDebugDrawRectangle(s32 x, s32 y, s32 width, s32 height) {
}

/* Retail discards menu-frame rectangles; callers supply geometry and two colors. */
void btlDebugDrawTwoColorRectangle(s32 x, s32 y, s32 width, s32 height, u32 color0, u32 color1) {
}

/* Retail no-op for the colored span between adjacent mapping-plot knots. */
void btlDebugDrawMappingColorSpan(s32 x, s32 y, s32 width, s32 height, u32 color0, u32 color1) {
}

/* Retail no-op: plot guides supply two-dimensional endpoints and packed color. */
void btlDebugDrawLine2D(s32 x0, s32 y0, s32 x1, s32 y1, u32 color) {
}

void func_0020D1D8(void) {
}

/* Retail ignores the scalar-curve knot's pixel coordinates and packed color. */
void btlDebugDrawPoint2DNoOp(s32 x, s32 y, u32 color) {
}

/* Retail stub for the endpoint marker submitted by camera diagnostics. */
void btlDebugDrawPoint(f32 *position) {
}

void func_0020D1F0(void) {
}

/* Retail stub for a centered radius guide; preserve the mixed-argument order. */
void btlDebugDrawRadiusGuide(f32 *position, f32 radius, u32 color, s32 flags) {
}

/* Retail stub for the two extents and orientation submitted by camera diagnostics. */
void btlDebugDrawOrientedExtentGuide(f32 *start, f32 firstExtent, f32 secondExtent, u32 color, f32 (*matrix)[4]) {
}

/* Retail stub for the position, color, mode and scale submitted by the actor editor. */
void btlDebugDrawScaledPositionGuide(f32 *position, u32 color, s32 mode, f32 scale) {
}

void func_0020D210(void) {
}

/* Retail stub for a colored segment between two diagnostic guide endpoints. */
void btlDebugDrawSegment(f32 *from, f32 *to, u32 color) {
}

void func_0020D220(void) {
}

/* Empty stage immediately before the per-context debug-menu entry. */
void btlDebugMenuBeforeRecordNoOp(void) {
}

/* Empty stage immediately after the per-context debug-menu entry. */
void btlDebugMenuAfterRecordNoOp(void) {
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

/* First debug-menu stage; return the supplied context word unchanged. */
u32 btlReturnDebugMenuSetupWord(u32 value) {
    return value;
}

void func_0020D268(void) {
}

/* Empty per-context stage; its sequence position does not imply recording. */
void btlDebugMenuRecordNoOp(u32 value) {
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

const char D_00419570[] __attribute__((aligned(8))) = "\314\244\274\302\301\365\245\271\245\257\245\352\245\327\245\310\241\247AI_CHK_BA\n";

const char D_00419590[] __attribute__((aligned(8))) = "\314\244\274\302\301\365\245\271\245\257\245\352\245\327\245\310\241\247AI_GET_ALLKOUDO\n";

const char D_004195B8[] __attribute__((aligned(8))) = "\314\244\274\302\301\365\245\271\245\257\245\352\245\327\245\310\241\247AI_GET_FUREA\n";

const char D_004195D8[] __attribute__((aligned(8))) = "\314\244\274\302\301\365\245\271\245\257\245\352\245\327\245\310\241\247BTL_CAM_ORG\n";

const char D_004195F8[] __attribute__((aligned(8))) = "\314\244\274\302\301\365\245\271\245\257\245\352\245\327\245\310\241\247BTL_CAM_2SHOT\n";

const char D_00419620[] __attribute__((aligned(8))) = "\314\244\274\302\301\365\245\271\245\257\245\352\245\327\245\310\241\247BTL_CAM_BOUGAI\n";

const char D_00419648[] __attribute__((aligned(8))) = "\314\244\274\302\301\365\245\271\245\257\245\352\245\327\245\310\241\247BTL_EVENT\n";

const char D_00419668[] __attribute__((aligned(8))) = "\314\244\274\302\301\365\245\271\245\257\245\352\245\327\245\310\241\247BTL_TAIKYO\n";

const char D_00419688[] __attribute__((aligned(8))) = "\314\244\274\302\301\365\245\271\245\257\245\352\245\327\245\310\241\247BTL_ALLTAIKYO\n";

const char D_004196B0[] __attribute__((aligned(8))) = "EFFECT EDIT";

const char D_004196C0[] __attribute__((aligned(8))) = "POINT LIGHT";

const char D_004196D0[] __attribute__((aligned(8))) = "FIELD LIGHT";

const char D_004196E0[] __attribute__((aligned(8))) = "FLD FILTER";

const char D_004196F0[] __attribute__((aligned(8))) = "CUT23 FIX ";

const char D_00419700[] __attribute__((aligned(8))) = "ENEMY PARTY";

const char D_00419710[] __attribute__((aligned(8))) = "1 SHOT    ";

const char D_00419720[] __attribute__((aligned(8))) = "CYLINDER F";

const char D_00419730[] __attribute__((aligned(8))) = "CYLINDER C";

const char D_00419740[] __attribute__((aligned(8))) = "ALL HIDE  ";

const char D_00419750[] __attribute__((aligned(8))) = "CAMERA    ";

const char D_00419760[] __attribute__((aligned(8))) = "BGM       ";

const char D_00419770[] __attribute__((aligned(8))) = "AI        ";

const char D_00419780[] __attribute__((aligned(8))) = "ENEMY COMM";

const char D_00419790[] __attribute__((aligned(8))) = "ALL SELECT";

const char D_004197A0[] __attribute__((aligned(8))) = "ALL SKILL ";

const char D_004197B0[] __attribute__((aligned(8))) = "ALL DEAD  ";

const char D_004197C0[] __attribute__((aligned(8))) = "ALL CRTICL";

const char D_004197D0[] __attribute__((aligned(8))) = "ALL HIT   ";

const char D_004197E0[] __attribute__((aligned(8))) = "ALL SAKE  ";

const char D_004197F0[] __attribute__((aligned(8))) = "ALL HANSYA";

const char D_00419800[] __attribute__((aligned(8))) = "ALL KYUSYU";

const char D_00419810[] __attribute__((aligned(8))) = "BAD       ";

const char D_00419820[] __attribute__((aligned(8))) = "PRESS     ";

const char D_00419830[] __attribute__((aligned(8))) = "DAMAGE    ";

const char D_00419840[] __attribute__((aligned(8))) = "CHANGE    ";

const char D_00419850[] __attribute__((aligned(8))) = "DEBUG MENU";

const char D_00419860[] __attribute__((aligned(8))) = "TEST.TLP";

const char D_00419870[] __attribute__((aligned(8))) = "TEST.P2A";

const char D_00419880[] __attribute__((aligned(8))) = "TEST.EPL";

const char D_00419890[] __attribute__((aligned(8))) = "test.tmx";

s32 btlOpenPfsDebugDirectory(const char *directoryName) {
    char pathBuffer[BTL_DIRECTORY_PATH_BYTES];

    /* Debug mode opens a formatted pfs0 path; built-in mode resets name iteration. */

    if (sdfPfsDebugMode != 0) {
        sprintf(pathBuffer, "pfs0:/%s", directoryName);
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
    sceDclose(directoryHandle);
}

/* Debug mode delegates to the native reader. Built-in mode returns each name's length,
   clears the directory-type bit, and returns zero when the eight-name table is exhausted. */
s32 btlReadBattleResourceDirectoryEntry(s32 directoryHandle, SceDirent *reader) {
    if (sdfPfsDebugMode != 0) {
        return sceDread(directoryHandle, reader);
    }
    if ((u32)D_00438F80 >= BTL_BUILTIN_NAME_COUNT) {
        return 0;
    }
    strcpy(reader->name, D_003BEA80[D_00438F80]);
    reader->stat.mode &= ~BTL_DIRECTORY_FLAG_CLEAR;
    D_00438F80++;
    return strlen(reader->name);
}

/* Build a scan-order list of built-in names and enabled resource-file categories. */
BtlResourceEntryList *btlScanDirectory(const char *path, s32 flags) {
    SceDirent reader;
    BtlResourceEntryList *list;
    BtlResourceEntry *entry;
    BtlResourceEntry *tail;
    s32 directoryHandle;
    s32 generalCount;
    s32 index;
    s32 length;
    s32 category;

    directoryHandle = btlOpenPfsDebugDirectory(path);
    list = sdfAllocSizeClassBlock(sizeof(BtlResourceEntryList));
    if (path != NULL) {
        list->pathPrefix = sdfAllocSizeClassBlock(strlen(path) + 1);
        strcpy(list->pathPrefix, path);
    } else {
        list->pathPrefix = NULL;
    }
    list->head = NULL;
    tail = NULL;
    list->count = 0;
    if (path == NULL) {
        return list;
    }
    if (flags & 8) {
        index = 0;
        generalCount = effGetGeneralResourceCount();
        for (; index < generalCount; index++) {
            entry = sdfAllocSizeClassBlock(BTL_RESOURCE_ENTRY_BYTES);
            entry->category = 8;
            entry->value = index;
            entry->id = 0;
            sprintf(entry->name, "GENERAL %d", index);
            if (tail != NULL) {
                tail->next = entry;
            } else {
                list->head = entry;
            }
            entry->prev = tail;
            entry->next = NULL;
            tail = entry;
            list->count++;
        }
    }
    while ((length = btlReadBattleResourceDirectoryEntry(directoryHandle, &reader)) > 0) {
        if ((reader.stat.mode & 0xF000) == 0x1000) {
            continue;
        }
        if (length < 4) {
            continue;
        }
        if (length >= 0x2F) {
            continue;
        }
        if ((flags & 1)
            && (reader.name[length - 3] == 't' || reader.name[length - 3] == 'T')
            && (reader.name[length - 2] == 'm' || reader.name[length - 2] == 'M')
            && (reader.name[length - 1] == 'x' || reader.name[length - 1] == 'X')) {
            category = 1;
        } else if ((flags & 2)
            && (reader.name[length - 3] == 'p' || reader.name[length - 3] == 'P')
            && reader.name[length - 2] == '2'
            && (reader.name[length - 1] == 'a' || reader.name[length - 1] == 'A')) {
            category = 2;
        } else if ((flags & 4)
            && (reader.name[length - 2] == 'p' || reader.name[length - 2] == 'P')
            && (reader.name[length - 1] == 'b' || reader.name[length - 1] == 'B')) {
            category = 4;
        } else if ((flags & 0x20)
            && (reader.name[length - 2] == 'e' || reader.name[length - 2] == 'E')
            && (reader.name[length - 1] == 'p' || reader.name[length - 1] == 'P')) {
            category = 0x20;
        } else if ((flags & 0x10)
            && (reader.name[length - 3] == 'e' || reader.name[length - 3] == 'E')
            && (reader.name[length - 2] == 'p' || reader.name[length - 2] == 'P')
            && (reader.name[length - 1] == 'l' || reader.name[length - 1] == 'L')) {
            category = 0x10;
        } else if ((flags & 0x40)
            && (reader.name[length - 3] == 't' || reader.name[length - 3] == 'T')
            && (reader.name[length - 2] == 'l' || reader.name[length - 2] == 'L')
            && (reader.name[length - 1] == 'p' || reader.name[length - 1] == 'P')) {
            category = 0x40;
        } else if ((flags & 0x80)
            && (reader.name[length - 2] == 'f' || reader.name[length - 2] == 'F')
            && reader.name[length - 1] == '2') {
            category = 0x80;
        } else {
            continue;
        }
        if (category != 0) {
            entry = sdfAllocSizeClassBlock(BTL_RESOURCE_ENTRY_BYTES);
            entry->category = category;
            entry->value = 0;
            entry->id = 0;
            strcpy(entry->name, reader.name);
            if (tail != NULL) {
                tail->next = entry;
            } else {
                list->head = entry;
            }
            entry->next = NULL;
            entry->prev = tail;
            list->count++;
            tail = entry;
        }
    }
    btlClosePfsDebugDirectory(directoryHandle);
    return list;
}

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

extern s8 sdfPadButtonStates[0x20];
extern s8 D_0040B7DB[];
extern SdfPoolNode kwlnDrawSurfaces[];
extern void *sdfCreateBorderedRectanglePacket(s32, s32, s32, s32, s32, u32, u32);
s32 btlFormatSelectedResourceName(BtlResourceDescriptor *, char *);
void btlLoadAndReplaceResourceHandle(BtlResourceDescriptor *, const char *);

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
        if (D_0040B7DB[0] < 0) {
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
        } else if (sdfPadButtonStates[3] < 0) {
            descriptor->selectionStatus = BTL_RESOURCE_SELECTION_CANCELED;
        } else if (sdfPadButtonStates[1] < 0) {
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
    sdfAppendPacket(packetList, (u32)sdfCreateBorderedRectanglePacket(
        (descriptor->originX << 4) + 0x7000,
        (descriptor->originY << 3) + 0x7900,
        0xFEFFFF, 0xCC0, 0x740, descriptor->fillColor, descriptor->borderColor));

    if (descriptor->previewActive != 0) {
        sdfAppendPacket(packetList, (u32)sdfCreateBorderedRectanglePacket(
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

/* Read the browser descriptor's zero-based selected entry index. */
u32 btlGetResourceBrowserSelectedIndex(s32 recordAddress) {
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
        sprintf(output, D_00436C50, pathPrefix, descriptor->selectedEntry->name);
    } else {
        sprintf(output, D_00436C58, descriptor->selectedEntry->name);
    }
    return descriptor->selectedEntry->category;
}

/* Strip from the first '.' onward; return the selected entry's category. */
s32 btlTrimResourceName(BtlResourceDescriptor *descriptor, char *output) {
    u32 nameLength;
    u32 characterIndex;
    sprintf(output, D_00436C58, descriptor->selectedEntry->name);
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

extern void *sdfConsAllocateColumnPacket(s32 arg0);
extern s32 sdfConsMeasurePacketWithHeader(s32 arg0);

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

/* Release the chip allocation used for an editable resource-name record. */
void btlDestroyResourceNameRecord(struct BtlResourceNameRecord *record) {
    sdfReleaseChipBlock(record);
}

extern u8 D_003BEAA0[];
extern char D_00436C60[8];
extern char D_00436C68[];
extern char D_00436C70[];

/* Three 13-cell rows end in a blank; BS/OK follow the grid. */
#define EDITOR_KEY_COLS 13
#define EDITOR_KEY_ROWS 3
#define EDITOR_KEY_CELLS (EDITOR_KEY_COLS * EDITOR_KEY_ROWS)
#define EDITOR_CHARACTER_COUNT (EDITOR_KEY_CELLS - 1)
#define EDITOR_BLANK_CELL (EDITOR_KEY_CELLS - 1)
#define EDITOR_BACKSPACE_CELL EDITOR_KEY_CELLS
#define EDITOR_CONFIRM_CELL (EDITOR_KEY_CELLS + 1)
#define EDITOR_DELETE_CODE (EDITOR_KEY_CELLS + 3)

void btlUpdateResourceNameEditor(struct BtlResourceNameRecord *record) {
    char text[0x20];
    s32 x = record->x;
    s32 y = record->y;
    s32 editCode = -1;
    s32 deleteCharacter;
    s32 editLimit;
    s32 selection;
    u32 index;
    s32 row;
    s32 column;

    if (record->selectionStatus != BTL_RESOURCE_SELECTION_ACCEPTED) {
        if ((sdfPadButtonStates[6] & 2) != 0) {
            if (record->selection == EDITOR_BACKSPACE_CELL) {
                record->selection = EDITOR_KEY_CELLS - 2;
            } else if (record->selection == EDITOR_CONFIRM_CELL) {
                record->selection = EDITOR_BACKSPACE_CELL;
            } else {
                record->selection -= EDITOR_KEY_COLS;
                if (record->selection < 0) {
                    record->selection = EDITOR_CONFIRM_CELL;
                }
            }
        } else if ((sdfPadButtonStates[7] & 2) != 0) {
            if (record->selection == EDITOR_BACKSPACE_CELL) {
                record->selection = EDITOR_CONFIRM_CELL;
            } else if (record->selection == EDITOR_CONFIRM_CELL) {
                record->selection = EDITOR_KEY_COLS - 1;
            } else {
                record->selection += EDITOR_KEY_COLS;
                if ((u32)record->selection >= EDITOR_BACKSPACE_CELL) {
                    record->selection = EDITOR_BACKSPACE_CELL;
                }
            }
        } else if ((sdfPadButtonStates[4] & 2) != 0) {
            selection = record->selection;
            if ((u32)(selection - EDITOR_BACKSPACE_CELL) >= 2) {
                if (selection % EDITOR_KEY_COLS == 0) {
                    record->selection = selection + EDITOR_KEY_COLS - 1;
                } else {
                    record->selection = selection - 1;
                }
            }
        } else if ((sdfPadButtonStates[5] & 2) != 0) {
            selection = record->selection;
            if ((u32)(selection - EDITOR_BACKSPACE_CELL) >= 2) {
                if (selection % EDITOR_KEY_COLS == EDITOR_KEY_COLS - 1) {
                    record->selection = selection - (EDITOR_KEY_COLS - 1);
                } else {
                    record->selection = selection + 1;
                }
            }
        } else if (sdfPadButtonStates[1] < 0) {
            selection = record->selection;
            if (selection == EDITOR_CONFIRM_CELL) {
                if (record->nameLength > 0) {
                    record->selectionStatus = BTL_RESOURCE_SELECTION_ACCEPTED;
                }
            } else if (selection == EDITOR_BACKSPACE_CELL) {
                editCode = EDITOR_DELETE_CODE;
            } else if (selection == EDITOR_BLANK_CELL) {
                /* Native0020E568 branches directly to shared0020E5B8. */
                goto next_cell;
            } else {
                editCode = selection;
            }
        } else if (sdfPadButtonStates[3] < 0) {
            record->selectionStatus = BTL_RESOURCE_SELECTION_CANCELED;
        }
        if (record->selection == EDITOR_BLANK_CELL) {
        next_cell:
            record->selection++;
        }
        /* Stage the comparison bound before expansion; see docs/idioms.md,
         * "Trace-justified comparison staging: resource-name editor". */
        editLimit = EDITOR_DELETE_CODE - 1;
        deleteCharacter = editLimit < editCode;
        if (deleteCharacter && record->nameLength > 0) {
            record->nameLength--;
        } else if ((u32)editCode < EDITOR_CHARACTER_COUNT) {
            record->resourceName[record->nameLength] = D_003BEAA0[editCode];
            if (record->nameLength < (s32)record->maximumNameLength) {
                record->nameLength++;
            }
        }
        record->resourceName[record->nameLength] = 0;
    }
    btlDebugDrawRectangle(x, y, 0xA4, 0x14);
    sprintf(text, D_00436C50, record->resourceName, record->extension);
    btlDebugDrawText(x + 4, y + 4, 0, text);
    y += 0x18;
    btlDebugDrawRectangle(x, y, 0xA4, 0x44);
    if (record->overwritePromptActive == 0) {
        x += 4;
        y += 4;
        for (index = 0, row = 0; row < EDITOR_KEY_ROWS; row++) {
            for (column = EDITOR_KEY_COLS - 1; column >= 0; column--) {
                if (index < EDITOR_CHARACTER_COUNT) {
                    s32 color = record->selection == index ? 4 : 0;
                    D_00436C60[0] = D_003BEAA0[index];
                    btlDebugDrawText(x, y, color, D_00436C60);
                }
                x += 12;
                index++;
            }
            x -= EDITOR_KEY_COLS * 12;
            y += 12;
        }
        x += 0x84;
        btlDebugDrawText(x, y, record->selection == index++ ? 4 : 0, D_00436C68);
        btlDebugDrawText(x, y + 12, record->selection == index ? 4 : 0, D_00436C70);
    }
}

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
    sprintf(output, D_00436C50, record->resourceName, record->extension);
}

void btlFormatResourceNameWithoutExtension(struct BtlResourceNameRecord *record, char *output) {
    sprintf(output, D_00436C58, record->resourceName);
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

