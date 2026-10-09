#include "btl_motion_transform.h"
#include "common.h"
#include "sdf_packet_list.h"
#include "sdf_packet_builders.h"
#include "btl_effect_position.h"
#include "sdf_chip.h"
#include "snd_slot.h"
#include "btl_task_state.h"
#include "btl_task_condition.h"
#include "sdf_resource.h"
#include "sdf_model.h"
#include "btl.h"
#include "sdf_texture_offset_list.h"
#include "btl_model_record.h"
#include "btl_command.h"
#include "btl_state.h"
#include "btl_task_args.h"
#include "btl_sound.h"
#include "eff_field_color.h"
#include "file.h"
#include "eff_update_flags.h"
#include "sdf.h"
#include "btl_action.h"
#include "btl_unit_tasks.h"
#include "dds3obj.h"
#include "evt_unit.h"
#include "eff_transform.h"
#include "eff_object.h"
#include "mdl.h"
#include "file_request_api.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "fpu.h"
#include "dat_state.h"
#include "dat_command.h"

extern void btlClearAllUnitDefeatCandidates(void);

extern s64 mnuGetSoundBufferStateLocked(void);

extern void func_00336538(f32);

typedef struct BtlUnit BtlUnit;

typedef struct BtlAt3Entry {
    u8 volume;
    u8 pad01[3];
    char fileName[12];
} BtlAt3Entry;

extern BtlAt3Entry D_003E0F60[];

extern u32 D_00436AD4;

extern s32 btlGetRuntime(void);

extern struct BtlRuntimeTask *btlDeferredTaskHead;

extern void btlBossDebugPrintf(const char *format, ...);

extern f32 func_00208000(s32, f32 *, f32 *);

extern s32 func_0035C860(char *, const char *, ...);

extern char D_00436AE8[];

extern u32 btlTintTransitionHoldCount;

typedef struct {
    union {
        void *actor;
        s32 value;
    };
    union {
        s32 option;
        u16 optionId;
        f32 scale;
    };
    u32 unk_08;
    union {
        u32 unk_0C;
        f32 scale2;
        s8 mode;
    };
    u32 unk_10;
    union {
        u32 unk_14;
        f32 scale14;
    };
    union {
        u32 unk_18;
        struct {
            u8 flag18;
            u8 flag19;
        };
    };
    u32 unk_1C;
    u32 unk_20;
    u32 unk_24;
} SoundTaskArgs;

extern BtlRuntimeTask *btlAllocTask(s32);

extern void effMiscQuaternionToMatrixVU(void);

extern u8 D_003E9130[];

extern u8 D_003E9120[];

extern void btlUnitGetMuzzlePosVU(BtlUnit *);

extern s32 btlGetSideIndexedActorStatusTable(s32, s32);

extern s32 btlRepositionPartyAroundBattleCenter(void);

extern void mnuReleaseSoundBufferLocked(void);

extern void func_002A27A8(s32, s32, u8);

extern void btlSetUnitPosition(BtlUnit *unit, f32 *position);

extern void btlSetUnitRotation(BtlUnit *unit, s128 *rotation);

extern s32 btlGetSideIndexedActorStatusTable(s32, s32);

extern s32 btlAimHorizontalDirectionVU(f32 *, f32 *);

extern void btlSetUnitRotation(BtlUnit *, s128 *);

typedef struct {
    u8 unk00[0x110];
    u32 flags;
    u8 unk114[0x10];
    u16 objectId;
} BattleEntryHeader;

extern char D_00436A28[];

extern u32 D_00436A98;

extern s32 D_00436A9C;

extern s32 D_00436AA0;

extern void btlFlagMatchingUnitsDefeatCandidate(s32);

extern void func_00208750(BtlIndexList *, s32, s32);

extern s16 D_00436AB0[];

extern s16 D_00436AB8[];

extern s16 D_00436AC0[];

extern f32 D_00436AD0;

extern const char D_00436AE0[];

extern char D_004192F8[];

extern char D_00419308[];

extern char D_00419318[];

extern void mnuClearInactiveSoundBufferState(void);

void btlAdvanceTitleState(void);

void btlClearUnitDefeatCandidate(BtlUnit *unit);

void btlFlagUnitDefeatCandidate(BtlUnit *unit);

/* Return the number of live entries, not the allocated capacity. */ u32 btlGetIndexListCount(BtlIndexList *list);

/* The caller supplies an in-range index. */ void *btlGetIndexListEntry(BtlIndexList *list, s32 index);

/* Return the argument address recorded by allocation (zero for no arguments). */ void *btlGetTaskArguments(void *task);

void btlUnitFaceTarget(BtlUnit *unit, BtlUnit *target);

void func_001E3108(void *object, f32 *dst);

s32 sndLoadAndPlayStationedSe(u32 soundId);

/* Return whether a not-yet-file-ready owner still has an outstanding request. */
s32 sndHasOccupiedNodeSlots(void) {
    SoundSlotOwner *owner;
    u32 i;
    for (owner = ((BtlState *)btlGetRuntime())->soundSlotOwners; owner != 0; owner = owner->next) {
        if (!(owner->flags & 2)) {
            for (i = 0; i < 0x1D; i++) {
                if (owner->work.fileRequests[i] != 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

s32 sndWaitForEarringPlayback(void) {
    s32 ready;
    s64 status;

    status = mnuGetSoundBufferStateLocked();
    ready = 1;
    if (status != 0) {
        if (status == 2) {
            mnuClearInactiveSoundBufferState();
            ready = 0;
        }
        else {
            ready = 0;
        }
    }
    return ready;
}

BtlRuntimeTask *sndCreateEarringTask(void) {
    BtlRuntimeTask *task = btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback = sndWaitForEarringPlayback;
    task->taskId = 0x5C;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

extern char D_00419468[]; /* "/soundat3/%s.at3" */

s32 sndPollAtrac3SELoadTask(Atrac3LoadTaskArgs *args) {
    char path[0x80];
    struct SdfMemBlock *resource;
    s32 data;
    s32 size;
    if (args->state == 0) {
        func_0035C860(path, D_00419468, D_003E0F60[args->entryIndex].fileName);
        args->request = fileQueueDefaultCallbackRequest(path);
        btlBossDebugPrintf("btl:atrac3 SE load[%s]\n", path);
    } else if (fileIsRequestReadyInCurrentMode(args->request) != 0) {
        if (mnuGetSoundBufferStateLocked() != 0) {
            mnuReleaseSoundBufferLocked();
        }
        resource = (struct SdfMemBlock *)fileGetResourceHandle(args->request);
        data = sdfResourceRetainAddress(resource);
        size = (s32)fileGetResourceSize(args->request);
        filePollEntryCleanup(args->request);
        func_002A27A8(data, size, D_003E0F60[args->entryIndex].volume);
        sdfReleaseResourceAllocation(resource);
        btlBossDebugPrintf("btl:atrac3 SE load end\n");
        return 1;
    }
    args->state++;
    return 0;
}

BtlRuntimeTask *sndCreateAtracEffectLoadTask(s32 entryIndex) {
    BtlRuntimeTask *task = btlAllocTask(sizeof(Atrac3LoadTaskArgs));
    Atrac3LoadTaskArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x5D;
    task->flags &= ~BTL_TASK_FLAG_REGISTERED;
    task->callback = sndPollAtrac3SELoadTask;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    args = btlGetTaskArguments(task);
    args->entryIndex = entryIndex;
    args->state = 0;
    args->request = 0;
    return task;
}

void sndStartDeadAtracLoad(BtlDeadLoadArgs *args) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlUnit *unit;
    s32 id;
    char path[0x70];
    if (work->earringPlaybackCount == 0) {
        if (mnuGetSoundBufferStateLocked() != 0) {
            mnuReleaseSoundBufferLocked();
        }
        unit = args->unit;
        if (unit->status.flags & 0x200) {
            id = unit->partyRecord.unitId;
            if (unit->partyRecord.flags & 0x10) {
                id += 0x20;
            } else if (unit->status.flags & 0x1000) {
                id += 0x10;
            }
            func_0035C860(path, D_004192F8, D_00419308, id);
        } else {
            func_0035C860(path, D_00419318, D_00419308, unit->partyRecord.unitId);
        }
        args->request = fileQueueDefaultCallbackRequest(path);
        btlBossDebugPrintf("btl:ATRAC3 dead load start[%s]\n", path);
    }
    work->earringPlaybackCount++;
}

s32 sndDeadAtracPlaybackTask(BtlDeadLoadArgs *args) {
    s32 data;
    s32 size;
    if (args->request == 0) {
        return 1;
    }
    if (args->resourceAllocation == 0) {
        if (fileIsRequestReadyInCurrentMode(args->request) != 0) {
            args->resourceAllocation =
                (struct SdfMemBlock *)fileGetResourceHandle(args->request);
            data = sdfResourceRetainAddress(args->resourceAllocation);
            size = (s32)fileGetResourceSize(args->request);
            filePollEntryCleanup(args->request);
            func_002A27A8(data, size, 2);
            mnuClearInactiveSoundBufferState();
            btlBossDebugPrintf("btl:ATRAC3 dead load end\n");
        }
        return 0;
    }
    if (mnuGetSoundBufferStateLocked() == 0) {
        btlBossDebugPrintf("btl:ATRAC3 dead play end\n");
        return 1;
    }
    return 0;
}

void sndFinishEarringPlaybackTask(BtlDeadLoadArgs *taskArgs) {
    u8 *work = (u8 *)btlGetRuntime();
    if (taskArgs->resourceAllocation != 0) {
        sdfReleaseResourceAllocation(taskArgs->resourceAllocation);
    }
    ((BtlState *)work)->earringPlaybackCount += 0xFFFF;
}

BtlRuntimeTask *sndCreateEarringPlaybackTask(BtlUnit *unit) {
    BtlRuntimeTask *task = btlAllocTask(sizeof(BtlDeadLoadArgs));
    BtlDeadLoadArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->taskId = 0x5E;
    task->flags &= ~BTL_TASK_FLAG_REGISTERED;
    task->ownerId = unit->owner;
    task->onStart = sndStartDeadAtracLoad;
    task->callback = sndDeadAtracPlaybackTask;
    task->onFinish = sndFinishEarringPlaybackTask;
    args = btlGetTaskArguments(task);
    args->unit = unit;
    args->request = 0;
    args->resourceAllocation = 0;
    return task;
}

s32 btlPlayStationedSe1C(void) {
    sndLoadAndPlayStationedSe(0x1c);
    return 1;
}

BtlRuntimeTask *btlCreateStationedSe1CTask(void) {
    BtlRuntimeTask *task = btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback = btlPlayStationedSe1C;
    task->taskId = 0x5F;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

s32 btlAdvanceTitleStateAfterSound(void) {
    btlAdvanceTitleState();
    return 1;
}

BtlRuntimeTask *btlCreateAdvanceTitleStateTask(void) {
    BtlRuntimeTask *task = btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback = btlAdvanceTitleStateAfterSound;
    task->taskId = 0x60;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

extern f32 D_003BE070[4];

extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);

extern f32 sdfSinPoly(f32);

/* Orient party actors and expand their formation when the marked count grows. */
s32 btlExpandPartyFormationAroundCenter(f32 *center) {
    BtlUnit *actors[16];
    f32 position[4];
    f32 direction[4];
    f32 target[4];
    BtlState *battle;
    BtlUnit *unit;
    s32 count = 0;
    s32 markedCount = 0;
    s32 i;
    f32 angle;
    f32 step;
    f32 radius;
    f32 adjustedAngle;

    battle = (BtlState *)btlGetRuntime();
    for (unit = battle->units; unit != 0; unit = unit->nextActor) {
        s32 flags = unit->status.flags;
        if (flags & 0x200) {
            actors[count++] = unit;
            if ((flags & 1) || battle->unk268 == 3) {
                markedCount++;
            }
        }
    }
    func_00208000(0x400, 0, 0);
    VU0_STORE_VF_UNCLOBBERED(vf10, target);
    for (i = count - 1; i >= 0; i--) {
        unit = actors[i];
        if (!(unit->status.flags & 0x80000)) {
            PCP_COPY_VECTOR(unit->rotation, D_003BE070);
        } else if (unit->status.flags & 0xE0) {
            PCP_COPY_VECTOR(unit->rotation, D_003BE070);
        } else {
            btlUnitGetMuzzlePosVU(unit);
            VU0_STORE_VF_UNCLOBBERED(vf10, position);
            if (btlAimHorizontalDirectionVU(position, target) != 0) {
                VU0_STORE_VF_UNCLOBBERED(vf10, direction);
                btlSetUnitRotation(unit, (s128 *)direction);
            }
        }
    }
    if (battle->unk268 >= markedCount) {
        return 0;
    }
    if (markedCount >= 2) {
        angle = (markedCount - 1) * 0.6981316805f * 0.5f;
    } else {
        angle = 0;
    }
    step = -0.6981316805f;
    radius = 400.0f;
    for (i = count - 1; i >= 0; i--) {
        unit = actors[i];
        if (unit->status.flags & 1) {
            position[0] = center[0] - sdfSinPoly(angle) * radius;
            position[1] = center[1];
            position[2] = center[2] - sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
        } else {
            adjustedAngle = angle - step * 0.25f;
            position[0] = center[0] - sdfSinPoly(adjustedAngle) * radius;
            position[1] = center[1];
            position[2] = center[2] - sdfEvaluateCosineViaSinePhaseShift(adjustedAngle) * radius;
        }
        PCP_COPY_VECTOR(unit->position, position);
        btlSetUnitPosition(unit, position);
        angle += step;
    }
    if (battle->unk268 < markedCount) {
        battle->unk268 = markedCount;
        return 1;
    }
    return 0;
}

extern f32 D_003BE080[4];

extern f32 D_003BE090[4];

extern f32 func_00352DB0(f32);

s32 func_00205CC8(s32 filter) {
    f32 position[4];
    f32 rotation[4];
    BtlUnit *actors[16];
    BtlState *battle;
    BtlUnit *unit;
    u32 count = 0;
    u32 i;
    f32 totalWidth = 0;
    f32 radius;
    f32 minSpacing;
    f32 maxSpacing;
    f32 spacing;
    f32 spacingAngle;
    f32 angle;
    f32 totalArcAngle;
    f32 halfAngle;
    f32 width;
    f32 distance;
    f32 direction;
    s32 isParty;

    battle = (BtlState *)btlGetRuntime();
    for (unit = battle->units; unit != 0; unit = unit->nextActor) {
        s32 flags = unit->status.flags;
        if ((flags & filter) && (flags & 1)) {
            f32 halfWidth = unit->unkBC * unit->scale;
            actors[count++] = unit;
            totalWidth += halfWidth + halfWidth;
        }
    }
    if (count == 0) {
        return 0;
    }
    if (battle->cameraActorHighWater < count) {
        battle->cameraActorHighWater = count;
    }
    isParty = filter & 0x200;
    if (isParty) {
        maxSpacing = 75.0f;
        minSpacing = 75.0f;
        radius = -800.0f;
        PCP_COPY_VECTOR(rotation, D_003BE080);
    } else {
        radius = 1000.0f;
        maxSpacing = 100.0f;
        minSpacing = 50.0f;
        PCP_COPY_VECTOR(rotation, D_003BE090);
    }
    if (count >= 2) {
        spacing = 1200.0f;
        if (spacing < totalWidth) {
            spacing = minSpacing;
        } else {
            spacing -= totalWidth;
            spacing /= count - 1;
            if (spacing < minSpacing) {
                spacing = minSpacing;
            } else if (spacing > maxSpacing) {
                spacing = maxSpacing;
            }
        }
    } else {
        spacing = 0;
    }
    spacingAngle = func_00352DB0(spacing / radius);
    totalArcAngle = -spacingAngle;
    for (i = 0; i < count; i++) {
        unit = actors[i];
        distance = unit->unkBC * unit->scale;
        halfAngle = func_00352DB0(distance / radius);
        totalArcAngle += halfAngle + halfAngle;
        totalArcAngle += spacingAngle;
    }
    width = totalWidth;
    if (width < 400.0f) {
        width = 400.0f;
    } else if (width > 500.0f) {
        width = 500.0f;
    }
    angle = -totalArcAngle * 0.5f;
    distance = radius - width * 0.5f;
    direction = 1.0f;
    if (!isParty) {
        direction = -1.0f;
    }
    for (i = count - 1; i != (u32)-1; i--) {
        unit = actors[i];
        halfAngle = func_00352DB0(unit->unkBC * unit->scale / radius);
        angle += halfAngle;
        position[0] = sdfSinPoly(angle) * radius;
        position[1] = 0.0f;
        position[2] = (distance - sdfEvaluateCosineViaSinePhaseShift(angle) * radius) * direction;
        PCP_COPY_VECTOR(unit->position, position);
        btlSetUnitPosition(unit, position);
        PCP_COPY_VECTOR(unit->rotation, rotation);
        angle += halfAngle;
        btlSetUnitRotation(unit, (s128 *)rotation);
        angle += spacingAngle;
    }
    if (battle->postPlacementCallback != 0) {
        battle->postPlacementCallback();
    }
    return 1;
}

s32 btlRepositionPartyAroundBattleCenter(void) {
    s128 v;
    PCP_COPY_VECTOR(&v, btlGetRuntime());
    return btlExpandPartyFormationAroundCenter((f32 *)&v);
}

s32 func_00206090(void) {
    return func_00205CC8(0x400);
}

void btlMoveOtherUnitsAway(ActionStateLink *link) {
    f32 pos[4];
    BtlUnit *other = ((BtlState *)btlGetRuntime())->units;
    u32 mask = link->unit->status.flags & 0x600;
    for (; other != NULL; other = other->nextActor) {
        if ((other->status.flags & 1) && (other->status.flags & mask) && other != link->unit) {
            btlClearUnitDefeatCandidate(other);
            if ((btlUnitStatusPair(other) & 0x102) == 0x102) {
                func_001E3108(other, pos);
                pos[1] += 1000000.0f;
                pos[0] = 0;
                effObjSetInnerPosition(other->effectObject, (u128 *)pos);
                btlSetUnitPosition(other, pos);
            }
        }
    }
    func_001E3108(link->unit, pos);
    pos[0] = 0;
    btlSetUnitPosition(link->unit, pos);
}

extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);

extern f32 sdfSinPoly(f32);

/* Place the three actor slots around the common battle center supplied in vf10. */
void btlPlaceTripleFormationAroundCenter(ActionStateLink *link, BtlUnit *first, BtlUnit *second) {
    BtlUnit *slot[3];
    f32 center[4];
    f32 pos[4];
    f32 rotation[4];
    f32 radius;
    f32 angle;

    slot[0] = NULL;
    slot[1] = NULL;
    slot[2] = NULL;
    slot[link->unit->lookupId] = link->unit;
    slot[first->lookupId] = first;
    slot[second->lookupId] = second;
    radius = func_00208000(0x400, 0, 0) + 200.0f;
    VU0_STORE_VF_UNCLOBBERED(vf10, center);
    pos[0] = center[0];
    pos[1] = 0.0f;
    pos[2] = center[2] - radius;
    btlSetUnitPosition(slot[1], pos);
    if (btlAimHorizontalDirectionVU((f32 *)pos, (f32 *)center) != 0) {
        VU0_STORE_VF_UNCLOBBERED(vf10, rotation);
        btlSetUnitRotation(slot[1], (s128 *)rotation);
    }
    angle = 30.0f * 0.017453293f;
    pos[0] = center[0] - sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
    pos[1] = 0.0f;
    pos[2] = center[2] - sdfSinPoly(angle) * radius;
    btlSetUnitPosition(slot[0], pos);
    if (btlAimHorizontalDirectionVU((f32 *)pos, (f32 *)center) != 0) {
        VU0_STORE_VF_UNCLOBBERED(vf10, rotation);
        btlSetUnitRotation(slot[0], (s128 *)rotation);
    }
    pos[0] = center[0] + sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
    pos[1] = 0.0f;
    pos[2] = center[2] - sdfSinPoly(angle) * radius;
    btlSetUnitPosition(slot[2], pos);
    if (btlAimHorizontalDirectionVU((f32 *)pos, (f32 *)center) != 0) {
        VU0_STORE_VF_UNCLOBBERED(vf10, rotation);
        btlSetUnitRotation(slot[2], (s128 *)rotation);
    }
}

void btlPlaceTripleFormationAroundTarget(ActionStateLink *link, BtlUnit *first, BtlUnit *second) {
    BtlUnit *slot[3];
    f32 center[4];
    f32 pos[4];
    f32 radius;
    BtlUnit *target;
    if (btlGetIndexListCount(link->indexWork.indices) == 1) {
        target = (BtlUnit *)btlGetIndexListEntry(link->indexWork.indices, 0);
        btlFlagAllUnitsDefeatCandidate();
        btlClearMatchingUnitDefeatCandidates(target->status.flags & 0x600);
        btlFlagUnitDefeatCandidate(target);
        slot[0] = 0;
        slot[1] = 0;
        slot[2] = 0;
        slot[link->unit->lookupId] = link->unit;
        slot[first->lookupId] = first;
        slot[second->lookupId] = second;
        btlUnitGetMuzzlePosVU(target);
        VU0_STORE_VF(vf10, center);
        center[1] = 0.0f;
        radius = target->unkBC * target->scale;
        radius += 100.0f;
        if (radius < 500.0f) {
            radius = 500.0f;
        }
        VU0_LOAD_VF(vf10, target->orientation);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, D_003E9130);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_SCALAR_OP(radius, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_LOAD_VF(vf11, center);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, pos);
        btlSetUnitPosition(slot[1], pos);
        btlUnitFaceTarget(slot[1], target);
        VU0_LOAD_VF(vf10, D_003E9120);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_SCALAR_OP(radius, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_LOAD_VF(vf11, center);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, pos);
        btlSetUnitPosition(slot[0], pos);
        btlUnitFaceTarget(slot[0], target);
        VU0_LOAD_VF(vf10, D_003E9120);
        VU0_NEGATE_XYZ(vf10);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_SCALAR_OP(radius, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_LOAD_VF(vf11, center);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, pos);
        btlSetUnitPosition(slot[2], pos);
        btlUnitFaceTarget(slot[2], target);
    }
}

void func_00206570(ActionStateLink *link, BtlUnit *first, BtlUnit *second) {
    BtlUnit *slot[3];
    f32 center[4];
    f32 pos[4];
    f32 dir[4];
    f32 rot[4];
    BtlUnit *unit;
    f32 radius;
    u32 i;
    BtlState *work = (BtlState *)btlGetRuntime();
    btlClearAllUnitDefeatCandidates();
    btlFlagMatchingUnitsDefeatCandidate(link->unit->status.flags & 0x600);
    slot[0] = 0;
    slot[1] = 0;
    slot[2] = 0;
    if (first != 0 && second != 0) {
        slot[link->unit->lookupId] = link->unit;
        slot[first->lookupId] = first;
        slot[second->lookupId] = second;
    } else {
        for (unit = work->units; unit != NULL; unit = unit->nextActor) {
            u32 flags = unit->status.flags;
            if (flags & 1) {
                if (flags & 0x200) {
                    slot[unit->lookupId] = unit;
                }
            }
        }
    }
    if (slot[1] != 0) {
        VU0_LOAD_VF(vf10, (u8 *)slot[1] + 0x70);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, D_003E9130);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_MOVE_VF(vf11, vf10);
        VU0_NEGATE_XYZ(vf11);
        VU0_STORE_VF(vf11, dir);
        radius = slot[1]->unkBC * slot[1]->scale;
        radius += 100.0f;
        VU0_SCALAR_OP(radius, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_STORE_VF_UNCLOBBERED(vf10, center);
        btlUnitGetMuzzlePosVU(slot[1]);
        VU0_LOAD_VF(vf11, center);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, center);
        center[1] = 0.0f;
    } else {
        center[0] = 0.0f;
        center[1] = 0.0f;
        center[2] = -230.0f;
        dir[0] = 0.0f;
        dir[1] = 0.0f;
        dir[2] = -1.0f;
    }
    for (i = 0; i < 3; i++) {
        if (slot[i] != 0) {
            radius = slot[i]->unkBC * slot[i]->scale;
            radius += 100.0f;
            if (i != 1) {
                if (i == 0) {
                    func_00336538(2.0943951f);
                } else if (i == 2) {
                    func_00336538(-2.0943951f);
                }
                VU0_LOAD_VF(vf10, dir);
                VU0_ROTATE_VEC(vf10, vf10);
            } else {
                VU0_LOAD_VF(vf10, dir);
            }
            VU0_SCALAR_OP(radius, "vmulx.xyzw vf10, vf10, vf2x");
            VU0_LOAD_VF(vf11, center);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF_UNCLOBBERED(vf10, pos);
            btlSetUnitPosition(slot[i], pos);
            if (btlAimHorizontalDirectionVU((f32 *)pos, (f32 *)center) != 0) {
                VU0_STORE_VF_UNCLOBBERED(vf10, rot);
                btlSetUnitRotation(slot[i], (s128 *)rot);
            }
        }
    }
}

void btlOrientFrontAndBackUnitsTowardTargets(ActionStateLink *link, BtlUnit *a, BtlUnit *b) {
    BtlUnit *front = 0;
    BtlUnit *back = 0;
    BtlUnit *target;
    s128 vec[3];
    u32 count;
    if (link->unit->status.flags & 0x1000) {
        back = link->unit;
    } else {
        front = link->unit;
    }
    if (a != 0) {
        if (a->status.flags & 0x1000) {
            back = a;
        } else {
            front = a;
        }
    }
    if (b != 0) {
        if (b->status.flags & 0x1000) {
            back = b;
        } else {
            front = b;
        }
    }
    count = btlGetIndexListCount(link->indexWork.indices);
    target = (BtlUnit *)btlGetIndexListEntry(link->indexWork.indices, 0);
    if (count == 1) {
        btlUnitFaceTarget(front, target);
    } else {
        btlUnitGetMuzzlePosVU(front);
        VU0_STORE_VF(vf10, &vec[0]);
        func_00208000(target->status.flags & 0x600, 0, 0);
        VU0_STORE_VF_UNCLOBBERED(vf10, &vec[1]);
        if (btlAimHorizontalDirectionVU((f32 *)&vec[0], (f32 *)&vec[1]) != 0) {
            VU0_STORE_VF_UNCLOBBERED(vf10, &vec[2]);
            btlSetUnitRotation(front, &vec[2]);
        }
    }
    btlUnitFaceTarget(back, front);
}

extern s32 btlGetBossSceneStateWhenActive(void);

/* vu0 routine: measure the center actor's displacement from its sole target. */
void btlAlignTripleFormationWithTarget(ActionStateLink *link, BtlUnit *first, BtlUnit *second) {
    BtlUnit *slot[3];
    f32 position[4];
    f32 targetPosition[4];
    BtlState *work;
    BtlUnit *target;
    f32 offsetX;
    f32 offset;
    u32 i;

    work = (BtlState *)btlGetRuntime();
    if (btlGetIndexListCount(link->indexWork.indices) == 1) {
        target = (BtlUnit *)btlGetIndexListEntry(link->indexWork.indices, 0);
        btlFlagAllUnitsDefeatCandidate();
        btlClearMatchingUnitDefeatCandidates(target->status.flags & 0x600);
        btlFlagUnitDefeatCandidate(target);
        slot[0] = NULL;
        slot[1] = NULL;
        slot[2] = NULL;
        slot[link->unit->lookupId] = link->unit;
        slot[first->lookupId] = first;
        slot[second->lookupId] = second;
        if ((work->commandRestrictFlags & 0x400000) != 0 &&
            btlGetBossSceneStateWhenActive() == 0) {
            switch (target->partyRecord.unitId) {
            case 0x111:
                offset = -550.0f;
                position[0] = offset;
                position[1] = 0.0f;
                position[2] = 350.0f;
                btlSetUnitPosition(slot[0], position);
                btlUnitFaceTarget(slot[0], target);
                position[0] = offset;
                position[1] = 0.0f;
                position[2] = 120.0f;
                btlSetUnitPosition(slot[1], position);
                btlUnitFaceTarget(slot[1], target);
                position[0] = offset;
                position[1] = 0.0f;
                position[2] = -110.0f;
                btlSetUnitPosition(slot[2], position);
                btlUnitFaceTarget(slot[2], target);
                return;
            case 0x112:
                offset = 550.0f;
                position[0] = offset;
                position[1] = 0.0f;
                position[2] = 350.0f;
                btlSetUnitPosition(slot[0], position);
                btlUnitFaceTarget(slot[0], target);
                position[0] = offset;
                position[1] = 0.0f;
                position[2] = 120.0f;
                btlSetUnitPosition(slot[1], position);
                btlUnitFaceTarget(slot[1], target);
                position[0] = offset;
                position[1] = 0.0f;
                position[2] = -110.0f;
                btlSetUnitPosition(slot[2], position);
                btlUnitFaceTarget(slot[2], target);
                return;
            }
        }
        btlUnitGetMuzzlePosVU(slot[1]);
        VU0_STORE_VF_UNCLOBBERED(vf10, position);
        btlUnitGetMuzzlePosVU(target);
        VU0_STORE_VF_UNCLOBBERED(vf10, targetPosition);
        VU0_LOAD_VF(vf11, position);
        VU0_SUB(vf10, vf10, vf11);
        VU0_GET_VF10_X(offsetX);
        targetPosition[2] -= target->reach * target->scale;
        offset = -(500.0f - (targetPosition[2] - position[2]));
        if (work->commandRestrictFlags & 0x200) {
            offset = 0.0f;
        }
        for (i = 0; i < 3; i++) {
            func_001E3108(slot[i], position);
            position[0] += offsetX;
            position[2] += offset;
            btlSetUnitPosition(slot[i], position);
            btlUnitFaceTarget(slot[i], target);
        }
    }
}

void func_00206C10(ActionStateLink *link, BtlUnit *first, BtlUnit *second) {
}

extern s128 D_003BE0A0;

void func_00206C18(ActionStateLink *link, BtlUnit *other) {
    BtlUnit *slot[2];
    f32 pos[4];
    f32 dir[4];
    f32 rot[4];
    f32 muzzle[4];
    BtlUnit *unit;
    f32 radius;
    u32 i;
    BtlUnit *linked = link->unit;
    if (linked->lookupId < other->lookupId) {
        slot[0] = linked;
        slot[1] = other;
    } else {
        slot[0] = other;
        slot[1] = linked;
    }
    for (unit = ((BtlState *)btlGetRuntime())->units; unit != NULL; unit = unit->nextActor) {
        u32 flags = unit->status.flags;
        if (flags & 1) {
            if (flags & 0x200) {
                if (unit != slot[0] && unit != slot[1]) {
                    btlClearUnitDefeatCandidate(unit);
                    btlSetUnitPosition(unit, (f32 *)&D_003BE0A0);
                    if ((btlUnitStatusPair(unit) & 0x102) == 0x102) {
                        PCP_COPY_VECTOR(pos, &D_003BE0A0);
                        pos[1] += 1000000.0f;
                        effObjSetInnerPosition(unit->effectObject, (u128 *)pos);
                    }
                }
            }
        }
    }
    if (btlGetIndexListCount(link->indexWork.indices) == 1) {
        btlUnitGetMuzzlePosVU((BtlUnit *)btlGetIndexListEntry(link->indexWork.indices, 0));
    } else {
        func_00208750(link->indexWork.indices, 0, 0);
    }
    VU0_STORE_VF(vf10, muzzle);
    VU0_SCALAR_OP_CLOBBER(0.0f, "vaddx.y vf10, vf0, vf2x");
    VU0_LOAD_VF(vf11, &D_003BE0A0);
    VU0_SUB(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_NEGATE_XYZ(vf10);
    VU0_STORE_VF(vf10, dir);
    for (i = 0; i < 2; i++) {
        radius = slot[i]->unkBC * slot[i]->scale;
        radius += 100.0f;
        if (i == 0) {
            func_00336538(1.0471975f);
        } else {
            func_00336538(-1.0471975f);
        }
        VU0_LOAD_VF(vf10, dir);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_SCALAR_OP(radius, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_LOAD_VF(vf11, &D_003BE0A0);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, pos);
        btlSetUnitPosition(slot[i], pos);
        if (btlAimHorizontalDirectionVU((f32 *)pos, (f32 *)muzzle) != 0) {
            VU0_STORE_VF_UNCLOBBERED(vf10, rot);
            btlSetUnitRotation(slot[i], (s128 *)rot);
        }
    }
}

extern s128 D_003BE0B0;
extern s128 D_003BE0C0;

void func_00206EA8(ActionStateLink *link, BtlUnit *other) {
    BtlUnit *slots[2];
    f32 position[4] __attribute__((aligned(16)));
    f32 rotation[4] __attribute__((aligned(16)));
    f32 center[4] __attribute__((aligned(16)));
    f32 muzzle[4] __attribute__((aligned(16)));
    f32 firstPosition[4] __attribute__((aligned(16)));
    f32 secondPosition[4] __attribute__((aligned(16)));
    BtlState *state;
    BtlUnit *unit;
    BtlUnit *linked;
    s32 special;
    f32 reach;
    f32 radius;
    u32 i;

    state = (BtlState *)btlGetRuntime();
    linked = link->unit;
    if (linked->lookupId < other->lookupId) {
        slots[0] = linked;
        slots[1] = other;
    } else {
        slots[0] = other;
        slots[1] = linked;
    }
    special = 0;
    if ((state->commandRestrictFlags & 0x400000) && btlGetBossSceneStateWhenActive() == 0) {
        unit = (BtlUnit *)btlGetIndexListEntry(link->indexWork.indices, 0);
        switch (unit->partyRecord.unitId) {
        case 0x111:
            special = 1;
            center[0] = -407.0f;
            center[1] = 0.0f;
            center[2] = -206.0f;
            firstPosition[0] = -557.0f;
            firstPosition[1] = 0.0f;
            firstPosition[2] = -300.0f;
            secondPosition[0] = -250.0f;
            secondPosition[1] = 0.0f;
            secondPosition[2] = -300.0f;
            break;
        case 0x112:
            special = 1;
            center[0] = 407.0f;
            center[1] = 0.0f;
            center[2] = -206.0f;
            secondPosition[0] = 557.0f;
            secondPosition[1] = 0.0f;
            secondPosition[2] = -300.0f;
            firstPosition[0] = 250.0f;
            firstPosition[1] = 0.0f;
            firstPosition[2] = -300.0f;
            break;
        }
    }
    if (!special) {
        if (btlGetIndexListCount(link->indexWork.indices) == 1) {
            unit = (BtlUnit *)btlGetIndexListEntry(link->indexWork.indices, 0);
            btlUnitGetMuzzlePosVU(unit);
            reach = unit->reach * unit->scale;
        } else {
            func_00208750(link->indexWork.indices, 0, 0);
            reach = 0.0f;
        }
        VU0_STORE_VF(vf10, muzzle);
        VU0_SCALAR_OP_CLOBBER(0.0f, "vaddx.y vf10, vf0, vf2x");
        VU0_LOAD_VF(vf11, &D_003BE0B0);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, center);
        center[2] -= reach;
    }
    for (unit = state->units; unit != NULL; unit = unit->nextActor) {
        u32 flags = unit->status.flags;
        if (flags & 1) {
            if (flags & 0x200) {
                if (unit != slots[0] && unit != slots[1]) {
                    btlClearUnitDefeatCandidate(unit);
                    btlSetUnitPosition(unit, center);
                    if ((btlUnitStatusPair(unit) & 0x102) == 0x102) {
                        PCP_COPY_VECTOR(position, center);
                        position[1] += 1000000.0f;
                        effObjSetInnerPosition(unit->effectObject, (u128 *)position);
                    }
                }
            }
        }
    }
    for (i = 0; i < 2; i++) {
        if (special) {
            if (i == 0) {
                position[0] = firstPosition[0];
                position[1] = firstPosition[1];
                position[2] = firstPosition[2];
            } else {
                position[0] = secondPosition[0];
                position[1] = secondPosition[1];
                position[2] = secondPosition[2];
            }
        } else {
            radius = slots[i]->unkBC * slots[i]->scale;
            radius += 100.0f;
            if (i == 0) {
                func_00336538(1.0471975f);
            } else {
                func_00336538(-1.0471975f);
            }
            VU0_LOAD_VF(vf10, &D_003BE0C0);
            VU0_ROTATE_VEC(vf10, vf10);
            VU0_SCALAR_OP_CLOBBER(radius, "vmulx.xyzw vf10, vf10, vf2x");
            VU0_LOAD_VF(vf11, center);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, position);
        }
        btlSetUnitPosition(slots[i], position);
        if (btlAimHorizontalDirectionVU(position, muzzle) != 0) {
            VU0_STORE_VF_UNCLOBBERED(vf10, rotation);
            btlSetUnitRotation(slots[i], (s128 *)rotation);
        }
    }
}



INCLUDE_ASM(const s32, "game/code_002053C0", func_00207268);

extern f32 D_003E9110[4];

/* Arrange the two marked three-slot groups around the battle origin. */
void func_00207438(ActionStateLink *link, BtlUnit *firstActor, BtlUnit *secondActor) {
    BtlState *state = (BtlState *)btlGetRuntime();
    BtlUnit *first[3];
    BtlUnit *second[3];
    f32 position[4];
    f32 target[4];
    f32 firstDirection[4];
    f32 secondDirection[4];
    f32 rotation[4];
    BtlUnit *unit;
    u32 slot;
    f32 radius;

    first[0] = first[1] = first[2] = NULL;
    second[0] = second[1] = second[2] = NULL;
    unit = state->units;
    if (unit != NULL) {
        BtlUnit **firstEntry = first;
        BtlUnit **secondEntry = second;
        do {
            u32 flags = unit->status.flags;
            if (flags & 1) {
                if (flags & 0x200) {
                    *firstEntry++ = unit;
                }
                if (flags & 0x400) {
                    *secondEntry++ = unit;
                }
            }
            unit = unit->nextActor;
        } while (unit != NULL);
    }
    PCP_COPY_VECTOR(firstDirection, D_003E9130);
    PCP_COPY_VECTOR(secondDirection, D_003E9130);
    func_00336538(1.0471975f);
    VU0_LOAD_VF(vf10, firstDirection);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF_UNCLOBBERED(vf10, firstDirection);
    func_00336538(2.0943951f);

    for (slot = 0; slot < 3; slot++) {
        if (second[slot] != NULL) {
            radius = second[slot]->unkBC * second[slot]->scale;
            radius += 500.0f;
            VU0_LOAD_VF(vf10, secondDirection);
            VU0_SCALE_VF(vf10, radius);
            VU0_LOAD_VF(vf11, D_003E9110);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF_UNCLOBBERED(vf10, position);
            btlSetUnitPosition(second[slot], position);
            if (btlAimHorizontalDirectionVU(position, D_003E9110)) {
                VU0_STORE_VF_UNCLOBBERED(vf10, rotation);
                btlSetUnitRotation(second[slot], (s128 *)rotation);
            }
        }
        if (first[slot] != NULL) {
            radius = first[slot]->unkBC * first[slot]->scale;
            radius += 100.0f;
            VU0_LOAD_VF(vf10, firstDirection);
            VU0_MOVE_VF(vf11, vf10);
            VU0_SCALE_VF(vf10, radius);
            VU0_SCALE_VF(vf11, 200.0f);
            VU0_ADD(vf11, vf11, vf10);
            VU0_STORE_VF_UNCLOBBERED(vf11, target);
            VU0_LOAD_VF(vf11, D_003E9110);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF_UNCLOBBERED(vf10, position);
            btlSetUnitPosition(first[slot], position);
            if (btlAimHorizontalDirectionVU(position, target)) {
                VU0_STORE_VF_UNCLOBBERED(vf10, rotation);
                btlSetUnitRotation(first[slot], (s128 *)rotation);
            }
        }
        VU0_LOAD_VF(vf10, secondDirection);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf10, secondDirection);
        VU0_LOAD_VF(vf10, firstDirection);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf10, firstDirection);
    }
}

s32 btlMoveOtherUnitsForCategory(u32 *command) {
    s32 category = command[1];
    if (category >= 0x1AB) {
        return 1;
    }
    if (category >= 0x5E) {
        return 1;
    }
    if (category >= 0x5B) {
        btlMoveOtherUnitsAway((ActionStateLink *)command[0]);
    }
    return 1;
}

BtlRuntimeTask *btlCreateMoveOtherUnitsTask(ActionStateLink *link, s32 option, s32 target) {
    BtlRuntimeTask *task = btlAllocTask(12);
    SoundTaskArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->callback = btlMoveOtherUnitsForCategory;
    task->taskId = 0x64;
    task->onStart = 0;
    task->ownerId = link->unit->owner;
    args = btlGetTaskArguments(task);
    args->actor = link;
    args->option = option;
    args->unk_08 = target;
    return task;
}

typedef struct {
    u8 unknown00[2];
    u16 kind;
    u8 unknown04[4];
} BtlTaskCategoryRow;

typedef char BtlTaskCategoryRow_size[(sizeof(BtlTaskCategoryRow) == 8) ? 1 : -1];

extern BtlTaskCategoryRow *D_00435E34;
extern void func_00206EA8(ActionStateLink *, BtlUnit *);

/* The formation task allocates five words, independently of the sound work. */
typedef struct BtlFormationTaskArgs {
    ActionStateLink *actor;
    BtlUnit *first;
    BtlUnit *second;
    u32 commandId;
    u32 unknown10;
} BtlFormationTaskArgs;

typedef char BtlFormationTaskArgs_size[(sizeof(BtlFormationTaskArgs) == 0x14) ? 1 : -1];

s32 func_002077C0(u32 *rawArgs) {
    BtlFormationTaskArgs *args = (BtlFormationTaskArgs *)rawArgs;
    BtlState *work = (BtlState *)btlGetRuntime();
    u16 category;

    if (args->first == NULL && args->second == NULL) {
        return 1;
    }
    if (args->actor->unit->status.flags & 0x400) {
        category = D_00435E34[args->commandId - 0x1AB].kind;
        switch (category) {
        case 7:
            func_00206C10(args->actor, args->first, args->second);
            break;
        case 11:
            func_00207438(args->actor, args->first, args->second);
            break;
        }
        return 1;
    }
    category = D_00435E34[args->commandId - 0x1AB].kind;
    switch (category) {
    case 0:
    case 5:
    case 7:
    case 10:
        break;
    case 1:
        if ((work->commandRestrictFlags & 0x200) == 0) {
            btlPlaceTripleFormationAroundCenter(args->actor, args->first, args->second);
        }
        break;
    case 2:
        if ((work->commandRestrictFlags & 0x200) == 0) {
            btlPlaceTripleFormationAroundTarget(args->actor, args->first, args->second);
        }
        break;
    case 3:
        func_00206570(args->actor, args->first, args->second);
        break;
    case 4:
        btlOrientFrontAndBackUnitsTowardTargets(args->actor, args->first, args->second);
        break;
    case 6:
        btlAlignTripleFormationWithTarget(args->actor, args->first, args->second);
        break;
    case 8:
        func_00206C18(args->actor, args->first);
        break;
    case 9:
        func_00206EA8(args->actor, args->first);
        break;
    }
    return 1;
}

BtlRuntimeTask *btlCreateSoundPlaybackTask(ActionStateLink *link, u32 soundId, u32 variant, u32 channel, u32 flags) {
    BtlRuntimeTask *task = btlAllocTask(20);
    BtlFormationTaskArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->callback = func_002077C0;
    task->taskId = 0x65;
    task->onStart = 0;
    task->ownerId = link->unit->owner;
    args = btlGetTaskArguments(task);
    args->actor = link;
    args->first = (BtlUnit *)soundId;
    args->second = (BtlUnit *)variant;
    args->commandId = channel;
    args->unknown10 = flags;
    return task;
}

INCLUDE_RODATA(const s32, "game/code_002053C0", D_00419540);

