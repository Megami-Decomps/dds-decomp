#include "btl_motion_transform.h"
#include "common.h"
#include "btl_stage_task_cleanup.h"
#include "mdl_motion_api.h"
#include "sdf_motion.h"
#include "sdf_packet_list.h"
#include "sdf_packet_builders.h"
#include "btl_effect_position.h"
#include "sdf_chip.h"
#include "snd_slot.h"
#include "kwln.h"
#include "btl_task_state.h"
#include "btl_task_condition.h"
#include "sdf_resource.h"
#include "sdf_model.h"
#include "btl.h"
#include "sdf_texture_offset_list.h"
#include "btl_state.h"
#include "btl_model_record.h"
#include "btl_task_args.h"
#include "btl_sound.h"
#include "eff_field_color.h"
#include "dds3obj.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "fpu.h"
#include "dat_state.h"
#include "evt_unit.h"
#include "mdl.h"
#include "mdl_asset_request.h"
#include "btl_action.h"
#include "btl_unit_tasks.h"
#include "sdf.h"
#include "file.h"
#include "dat_command.h"
#include "file_request_api.h"
#include "dat_affinity.h"

enum {
    BTL_SOUND_ENTRY_COUNT = 0x31,
    BTL_SELECTED_UNIT_EFFECT_SOUND_SLOT = 0x26
};

void btlPrepareSavedCommandTargets(BtlTask *, BattleIndexWork *);

extern s32 btlIsUnitInActiveList(void *unit);

typedef struct SceneAiEntry {
    u8 kind;
    u8 pad01;
    u16 slot;
    u8 pad04[0x158];
} SceneAiEntry;

extern SceneAiEntry *datEnemyAiRecords;

extern s32 btlAllocAndCheck(s32);

extern u32 btlAssignTaskResultAndArgument(s32);

extern void btlBindActorSlot(void *, s32);

extern s32 btlRunRandomWeightedAiTableAction(BtlTask *);

extern void btlDebugPrintf(const char *, ...);

extern void btlBossDebugPrintf(const char *format, ...);

extern u64 btlStartTask(void *);

extern void btlDispatchStateHandler(void *, s32);

extern s32 btlCountTasksForOwner(s64);

extern u64 btlAdvanceRuntimeSequenceCounter();

extern u8 *btlAllocateIndexedUnitEffectTask(u8 *, s32, s32, f32);

/* Battle runtime prefix: actor/task/sound registrations and packed battle tint. */
typedef struct BtlActorWork {
    u8 pad_00[0x208];
    s32 unk208;
    u8 pad20C[0x1C];
    BtlUnit *actorList;
    struct SoundTask *taskTail; /* Newest registration; reverse traversal. */
    struct SoundTask *taskHead; /* Oldest registration; forward traversal. */
    struct SoundResourceNode *soundResourceHead; /* Allocated resource nodes. */
    struct ActiveSoundNode *soundList;           /* Independent active-node list. */
    struct SoundSlotOwner *soundSlotOwners;     /* Shared category/id owners. */
    u8 pad240[0x24];
    u32 unk264;
    s32 unk268;
    u8 pad26C[0x38];
    s32 unk2A4;
    u8 pad2A8[0x210];
    struct SoundResourceNode *soundResourceSlots[BTL_SOUND_ENTRY_COUNT];
    u8 pad57C[8];
    u8 fadeEnabled; /* 0 raises the tint, 1 lowers it; refreshed by the frame updater. */
    u8 pad585[3];
    u32 fadeColor; /* Packed tint; retain the original whole-word arithmetic. */
    u8 pad58C[0x58];
    s32 (*hook5E4)(BtlUnit *); /* Actor-state predicate consulted by the defeat transition. */
    s32 (*hook5E8)(BtlUnit *); /* Fallback predicate consulted when hook5E4 returns 0. */
} BtlActorWork;

/* Tagged scheduler predicate, embedded for task entry and exit. */
typedef struct TaskCondition {
    u8 kind; /* 0 never, 1 always, 2 counter threshold, 3-10 task queries. */
    u8 pad01[7];
    union {
        s32 count;    /* Kind 2: signed threshold for the supplied counter. */
        u64 handle;   /* Kinds 3-5: task handle. */
        u64 owner;    /* Kinds 6-8: task owner; queries select its oldest task. */
        u16 taskKind; /* Kinds 9-10: registered task kind. */
    } value;
} TaskCondition;

/* Generic scheduler header; task-specific arguments follow at byte 0x70.
 * next/prev link registration order, independently of the deferred queue. */
typedef struct SoundTask {
    TaskCondition startCondition;
    TaskCondition endCondition;
    u16 taskId;
    u16 state; /* 0 waiting, 1 start delay, 2 running, 3 end delay. */
    u16 flags;
    u8 unk_26[2];
    s32 startDelay;
    s32 endDelay;
    u32 pollCount; /* Eligible scheduler polls, including wait/delay phases. */
    u32 runCount;  /* Callback updates that continued the running phase. */
    u64 handle; /* Installed by btlStartTask; independent of owner. */
    u64 owner;
    void (*onStart)(u32);
    union {
        void (*update)(void);
        s32 (*playSound)(u32 *);
        u32 (*process)(void);
        u32 (*commandSound)(u32 *);
        u32 (*playCustomSound)(u8 *);
        s32 (*releaseSound)(u16 *);
        s32 (*acquireSound)(u32 *);
        s32 (*run)(void *);
    } callback;
    void (*onFinish)(u32 *);
    void *args;
    struct SoundTask *next;
    struct SoundTask *prev;
    struct SoundTask *deferNext;
    struct SoundTask *deferPrev;
    u8 pad68[8];
} SoundTask;

typedef struct ActiveSoundNode {
    u32 flags;
    u8 unk_04[8];
    struct ActiveSoundNode *previous;
    struct ActiveSoundNode *next;
} ActiveSoundNode;

extern void *btlCreateMoveOtherUnitsTask(u8 *, u32);

extern SoundResourceNode *sndAllocResourceNode(void);

extern s32 func_00214868(void);

u8 *fldCreateSceneGroupAction(u8 *, u32, s32);

extern DatEnemyRecord *datEnemyRecords;

extern void func_001B83D8(BtlTask *, s8, s8);

extern s32 btlGetRuntime(void);

extern s32 datActionAnimationRecords;

void func_001A1948();

extern void func_001B83D8(BtlTask *, s8, s8);

extern void sndSetStationedSeVolume(u32);

extern s32 btlRepositionPartyAroundBattleCenter(void);

extern s32 btlMarkInactiveActorCandidates(void);

s32 btlBothSidesActive(BtlUnit *unit);

void btlClearAllActorEntrySlots(BtlUnit *unit);

void btlClearSceneTaskActiveFlag(s32 arg0);

s32 btlGetEntryFlagsUnlessDisabled(DatPartyRecord *entry);

extern s32 btlTestActorStatusPredicate(BtlUnit *);

extern void btlRefreshUnitMotionSelection(BtlUnit *unit);

extern s32 btlGetSlotRateKind(u8 *, s32);

extern u8 *btlCreateStiffenDamageShakeTask(u8 *, f32);

s32 btlGetLoggedIndexedCommandItem(s32 index);

s32 btlGetSideIndexedActorStatusTable(s32 arg0, s32 arg1);

extern s32 btlCheckSpecialAbility(DatPartyRecord *, s32);

s32 btlIsUnitDefeatTriggeredByValueDelta(BtlUnit *actor, s32 delta);

void btlSyncPlayerWork(BtlUnit *actor);

void fldAppendTaskToGroup(BtlTask *task);

void fldCreateSceneSpriteTask(s32 arg0);

void func_001A1960(DatPartyRecord *record, s32 mask);

void btlActionSeqStateSelect(u8 *task) {
    u8 *work = (u8 *)btlGetRuntime();
    BtlUnit *unit = *(BtlUnit **)(task + 0x18);
    s32 (*hook)(u8 *);
    s32 next;
    u32 flags;
    *(u32 *)(task + 8) &= ~0x20;
    if (*(u16 *)(task + 4) == 0) {
        btlDispatchStateHandler(task, 0x1A);
        btlBossDebugPrintf("btl:actnum 0 [%p]\n", task);
        return;
    }
    hook = *(s32 (**)(u8 *))(work + 0x5FC);
    if (hook != 0) {
        next = hook(task);
        if (next != -1) {
            btlDispatchStateHandler(task, next);
            return;
        }
    }
    if (*(u32 *)(task + 8) & 0x40) {
        btlDispatchStateHandler(task, 0xA);
    } else {
        flags = (u32)unit->status.flags;
        if (flags & 0x200) {
            if (*(u32 *)(work + 0x1F4) & 0x8000) {
                btlDispatchStateHandler(task, 9);
            } else {
                btlDispatchStateHandler(task, 6);
            }
        } else if (flags & 0x400) {
            if (!(*(u32 *)(work + 0x1FC) & 1)) {
                btlDispatchStateHandler(task, 8);
            } else {
                btlDispatchStateHandler(task, 6);
            }
        }
    }
}

extern s32 effOffsetIfOwnerFlagClear();

void btlUnitTurnEndStateSelect(u8 *task) {
    BtlUnit *unit = *(BtlUnit **)(task + 0x18);
    u32 flags = (u32)unit->status.flags;
    if (flags & 0x200) {
        if (flags & 0x1000) {
            if ((unit->status.stateFlags & 0x40) && !(*(u16 *)((u8 *)unit + 0x12E) & 0x7C0E) &&
                !(*(u32 *)(task + 8) & 0x100)) {
                *(u16 *)(task + 0x50) = 4;
                *(s32 *)(task + 0x54) = effOffsetIfOwnerFlagClear(unit, 0xA4);
                unit->status.flags = ((u32)unit->status.flags & ~0x20) | 0x400000;
                *(u16 *)((u8 *)unit + 0x120) |= 0x4000;
                unit->status.stateFlags |= 0x2000;
                btlDispatchStateHandler(task, 0x10);
            } else {
                btlDispatchStateHandler(task, 0x1D);
            }
            unit->status.stateFlags &= ~0x40;
        } else {
            btlDispatchStateHandler(task, 0x1D);
        }
    } else {
        btlDispatchStateHandler(task, 0x1D);
    }
}

extern void sndFreeResourceNode(SoundResourceNode *);

extern void sndFreeListNode(ActiveSoundNode *);

extern s32 btlCountTasksByKind(u16 kind);

s32 fldCheckSceneResourcesIdle(BtlUnit *self) {
    BtlActorWork *scene = (BtlActorWork *)btlGetRuntime();
    BtlUnit *actor;

    for (actor = scene->actorList; actor != 0; actor = actor->next) {
        if (actor->resourceNode != 0) {
            if (sndIsResourceNodeReferencedOrActive(actor->resourceNode) != 0) {
                if (actor == self) {
                    return 0;
                }
                sndGetResourceStatus(actor->resourceNode);
                return 0;
            }
            sndFreeResourceNode(actor->resourceNode);
            actor->resourceNode = 0;
        }
    }
    if (self->listNode != 0) {
        if (sndHasResourceFlagsOneOrEight(self->listNode) != 0) {
            return 0;
        }
        sndFreeListNode(self->listNode);
        self->listNode = 0;
    }
    for (actor = scene->actorList; actor != 0; actor = actor->next) {
        if (actor->status.flags & 0x200) {
            if (actor->status.flags & 2) {
                if ((actor->gunResourceFlags & 8) == 0) {
                    return 0;
                }
            }
        }
    }
    if (btlCountTasksByKind(0x23) != 0) return 0;
    if (btlCountTasksByKind(0x3D) != 0) return 0;
    if (btlCountTasksByKind(0x3E) != 0) return 0;
    if (btlCountTasksByKind(0x3C) != 0) return 0;
    if (btlCountTasksByKind(0x33) != 0) return 0;
    if (btlCountTasksByKind(0x34) != 0) return 0;
    if (btlCountTasksByKind(0x24) != 0) return 0;
    return btlCountTasksByKind(0x2B) == 0;
}

s32 fldReleaseIdleSceneActorResources(BtlUnit *actor) {
    if (actor->resourceNode != 0) {
        if (sndIsResourceNodeReferencedOrActive(actor->resourceNode) != 0) {
            return 0;
        }
        sndFreeResourceNode(actor->resourceNode);
        actor->resourceNode = 0;
    }
    if (actor->listNode != 0) {
        if (sndHasResourceFlagsOneOrEight(actor->listNode) != 0) {
            return 0;
        }
        sndFreeListNode(actor->listNode);
        actor->listNode = 0;
    }
    if (actor->effectLink.referenceCount != 0) {
        return 0;
    }
    if (btlIsUnitInActiveList(actor) != 0) {
        btlResetActiveUnitList();
        return 0;
    }
    if (btlCountTasksForOwner(actor->identity) != 0) {
        return 0;
    }
    return btlCountTasksByKind(0x2B) == 0;
}

void func_001C8D38(void) {
}

void func_001C8D40(void) {
}

void func_001C8D48(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) & 0xfffffdff;
}

void btlReleaseIdleUnitSoundAndAdvanceTask(BtlTask *task) {
    BtlUnit *unit = task->unit;
    u32 flags;
    u32 masked;
    SoundResourceNode *resource = unit->resourceNode;

    task->flags &= ~0x100;
    if (resource != 0) {
        if (sndIsResourceNodeReferencedOrActive(resource) == 0) {
            sndFreeResourceNode(unit->resourceNode);
            unit->resourceNode = 0;
        }
    }
    flags = unit->status.flags;
    if ((flags & 0x400) == 0 && (unit->partyRecord.status & 0x4000) == 0) {
        if (unit->species == 0x1F) {
            func_001A1948(&unit->partyRecord, 0x1000);
            flags = unit->status.flags;
        }
        masked = flags & ~0x20;
        masked &= ~0x08000000;
        unit->status.flags = masked;
        btlFlagUnitDefeatCandidate(unit);
        btlRefreshUnitMotionSelection(unit);
        btlStartTask(btlAllocateIndexedUnitEffectTask(unit, 0xE, 0, 1.0f));
        fldAppendTaskToGroup(task);
        btlDispatchStateHandler(task, 2);
    }
    return;
}

void func_001C8E60(s32 arg0) {
    *(u32 *)(arg0 + 8) = (*(u32 *)(arg0 + 8) | 0x10) & ~0x200;
}

void btlTaskUpdateFlags(BtlTask *task) {
    BtlUnit *unit = task->unit;
    u32 flags;
    if (!(unit->status.flags & 0x20)) {
        task->flags &= ~0x100;
    }
    if (unit->resourceNode != 0 && sndIsResourceNodeReferencedOrActive(unit->resourceNode) == 0) {
        sndFreeResourceNode(unit->resourceNode);
        unit->resourceNode = 0;
    }
    flags = unit->status.flags;
    if (flags & 0x20000000) {
        btlDispatchStateHandler(task, 0x11);
    } else if (flags & 0x400000) {
        btlDispatchStateHandler(task, 0x10);
    } else if (flags & 0x10000000) {
        btlDispatchStateHandler(task, 0x12);
    } else if (flags & 0x20) {
        btlUnitTurnEndStateSelect(task);
    }
}

void func_001C8F88(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) & 0xffffffef;
}

void btlActionSeqCheckDispatch(u8 *task) {
    BtlState *scene = (BtlState *)btlGetRuntime();
    u32 flags = scene->battleFlags;
    BtlUnit *unit = ((BtlTask *)task)->unit;
    BtlUnit *actor;
    if (!(flags & 0x20)) {
        for (actor = scene->units; actor != 0; actor = actor->next) {
            u32 actorFlags = actor->status.flags;
            if (actorFlags & 0x4000) {
                return;
            }
            if (actorFlags & 0x30400000) {
                return;
            }
        }
        if (!(flags & 0x8000) || fldCheckSceneResourcesIdle(unit) != 0) {
            if (btlBothSidesActive(unit) == 0) {
                btlDispatchStateHandler(task, 0x1C);
                return;
            }
            if (func_001FCAC0(task) != 0) {
                btlDispatchStateHandler(task, 5);
            } else {
                btlActionSeqStateSelect(task);
            }
        }
    }
}

void func_001C9090(void) {
}

void *btlCreateActorParameterDeltaTask(BtlUnit *owner, BtlOperandEntry *spec);

extern s32 evtRunContext(s32, s32, s32, s32, u16);

extern s32 btlRollAiBucket(void);

extern BtlRuntimeTask *btlCreateEffObjA(BtlUnit *, s32);

extern BtlRuntimeTask *btlCreateEffObjB(BtlUnit *, s32);

extern BtlRuntimeTask *btlCreateEffObjD(BtlUnit *, s32);

/* Try to clear the unit's condition: 2 and 4 always clear, 1 needs battle mode 2, and the
 * others roll a script-supplied chance (capped at 70, scaled by ability 0x232). */
void func_001C9098(BtlTask *link) {
    BtlOperandEntry spec;
    BtlUnit *unit;
    s32 chance;
    f32 scale;
    BtlRuntimeTask *task;

    if (btlCountTasksByKind(0x45) != 0) {
        return;
    }
    unit = link->unit;
    unit->status.flags |= 0x4000;
    switch (unit->partyRecord.status & 0x7FFF) {
    case 8:
    case 0x20:
    case 0x200:
    case 0x1000:
        if (unit->status.stateFlags & 4) {
            unit->status.stateFlags &= ~4;
            break;
        }
        /* fallthrough */
    case 1:
        unit->status.stateFlags &= ~4;
        switch (unit->partyRecord.status & 0x7FFF) {
        case 0x1000:
            chance = evtRunContext(0xE, (s32)&unit->partyRecord, 0, 0, 0);
            break;
        case 0x200:
            chance = evtRunContext(0xF, (s32)&unit->partyRecord, 0, 0, 0);
            break;
        case 0x20:
            chance = evtRunContext(0x10, (s32)&unit->partyRecord, 0, 0, 0);
            break;
        case 8:
            chance = evtRunContext(0x11, (s32)&unit->partyRecord, 0, 0, 0);
            break;
        case 1:
            chance = ((BtlState *)btlGetRuntime())->mode == 2 ? 100 : 0;
            break;
        default:
            chance = 0;
            break;
        }
        scale = 1.0f;
        if (btlCheckSpecialAbility(&unit->partyRecord, 0x232)) {
            scale = datAbilityParameters[0x232 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        chance = chance * scale;
        if ((unit->partyRecord.status & 0x7FFF) != 1 && chance > 70) {
            chance = 70;
        }
        btlBossDebugPrintf("btl:bad recovery=%d%%[ratio=%.2f]\n", chance, scale);
        if (btlRollAiBucket() >= chance) {
            break;
        }
        /* fallthrough */
    case 2:
    case 4:
        memset(&spec, 0, sizeof(spec));
        spec.removedStatus = 0x122F;
        btlStartTask(btlCreateActorParameterDeltaTask(unit, &spec));
        if (unit->partyRecord.status & 0x1000) {
            unit->status.flags |= 0x20000000;
            task = btlCreateEffObjB(link->unit, 0xCA);
            task->ownerId = btlAdvanceRuntimeSequenceCounter();
            btlStartTask(task);
            btlStartTask(btlCreateCommandSoundUpdateTask());
            btlStartTask(btlCreateSecondaryCommandSoundTask());
            btlStartTask(btlCreateCommandSoundTask((s32)link, 3));
        }
        break;
    }
    btlDispatchStateHandler(link, 0x1B);
}

void func_001C93A0(void) {
}

void btlStartCommandSoundAndEffectTasks(u8 *arg0) {
    BtlUnit *ctx;
    u8 *task;
    u64 value;
    s32 count;

    if (sndHasActiveActor() != 0) {
        return;
    }
    if (btlCountTasksByKind(0x45) != 0) {
        return;
    }
    ctx = *(BtlUnit **)(arg0 + 0x18);
    value = btlAdvanceRuntimeSequenceCounter();
    btlStartTask(btlCreateCommandSoundUpdateTask());
    btlStartTask(btlCreateSecondaryCommandSoundTask());
    if ((u32)ctx->status.flags & 0x200) {
        btlStartTask(btlCreateCommandSoundTask(arg0, 9));
    } else {
        btlStartTask(btlCreateCommandSoundTask(arg0, 3));
    }
    switch (*(u16 *)((u8 *)ctx + 0x12E) & 0x7FFF) {
    case 0x200:
        func_001FF0C8(arg0, 0);
        btlDispatchStateHandler(arg0, 0xC);
        break;
    case 0x2000:
        func_001FF0C8(arg0, 1);
        btlDispatchStateHandler(arg0, 0xC);
        break;
    case 0x20:
        if ((u32)((BtlTask *)arg0)->unit->status.flags & 0x200) {
            func_001FF0C8(arg0, 2);
        } else {
            func_001FF0C8(arg0, 3);
        }
        btlDispatchStateHandler(arg0, 0xC);
        break;
    case 0x40:
        func_001FF0C8(arg0, 4);
        btlDispatchStateHandler(arg0, 0xC);
        break;
    case 1:
        *(s32 *)(arg0 + 0x20) = 0xD;
        btlDispatchStateHandler(arg0, 0xC);
        break;
    case 8:
        task = fldCreateSceneGroupAction(arg0, 0x64, 1);
        *task = 7;
        *(u64 *)(task + 8) = value;
        *(u64 *)(task + 0x40) = *(u64 *)((u8 *)ctx + 0x108);
        btlStartTask(task);
        btlDispatchStateHandler(arg0, 0x1A);
        break;
    case 0x800:
        task = fldCreateSceneGroupAction(arg0, 0x64, 1);
        *task = 7;
        *(u64 *)(task + 8) = value;
        *(u64 *)(task + 0x40) = *(u64 *)((u8 *)ctx + 0x108);
        btlStartTask(task);
        btlDispatchStateHandler(arg0, 0x1A);
        break;
    }
    count = func_001FD170(arg0);
    if (count > 0) {
        task = (u8 *)btlCreateEffObjB(ctx, count);
        *(u64 *)(task + 0x40) = value;
        btlStartTask(task);
    }
    *(u32 *)(arg0 + 8) |= 0x200;
}

void func_001C9628(u32 arg0) {
    btlGetRuntime();
    *(u32 *)((s32)arg0 + 8) = *(u32 *)((s32)arg0 + 8) & 0xfffffffb;
    btlOpenBattleCommandPanel(arg0);
}

extern s32 fldGetSceneObjectState(void);

extern void fldSetSceneObjectAndGroupStates(void);

extern s32 btlAiCheckStatusRollEligibility(BtlTask *task);

void func_001C9660(BtlTask *task) {
    BtlState *scene = (BtlState *)btlGetRuntime();
    s32 state;

    if (scene->battleFlags & 0x20) {
        return;
    }
    if (!(task->flags & 4) && sndHasActiveActor() == 0 &&
        btlCountTasksByKind(0x2B) == 0) {
        btlStartTask(btlCreateCommandSoundUpdateTask());
        btlStartTask(btlCreateSecondaryCommandSoundTask());
        btlStartTask(btlCreateCommandSoundTask((s32)task, 9));
        task->flags |= 4;
    }

    state = fldGetSceneObjectState();
    if (state == 3 || state == 8) {
        if (btlIsSupportedCommandKind(&task->indexWork.phase) != 0) {
            btlDispatchStateHandler(task, 7);
        } else {
            fldSetSceneObjectAndGroupStates();
            if (btlAiCheckStatusRollEligibility(task) != 0) {
                btlDispatchStateHandler(task, 0xB);
            } else {
                btlDispatchStateHandler(task, 0xC);
            }
        }
    } else if (scene->battleFlags & 0x8000) {
        fldSetSceneObjectAndGroupStates();
        btlDispatchStateHandler(task, 9);
    }
}

void btlCommandResultEffectSelect(u8 *task) {
    s32 sel;
    s32 reason;
    s32 state;

    *(u32 *)(task + 8) &= ~4;
    state = *(s32 *)(task + 0x20);
    switch (state) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 7:
    case 8:
        if (state == 4) {
            sel = btlGetLoggedIndexedCommandItem(*(s32 *)(task + 0x28));
        } else {
            sel = *(s32 *)(task + 0x24);
        }
        reason = btlGetCommandBlockReason(task, sel);
        switch (reason) {
        case 2:
            btlStartTask(btlCreateEffObjB(*(BtlUnit **)(task + 0x18), 0x82));
            sndSetStationedSeVolume(0xD);
            btlDispatchStateHandler(task, 6);
            return;
        case 6:
            btlStartTask(btlCreateEffObjB(*(BtlUnit **)(task + 0x18), 0xB0));
            sndSetStationedSeVolume(0xD);
            btlDispatchStateHandler(task, 6);
            return;
        case 7:
            btlStartTask(btlCreateEffObjB(*(BtlUnit **)(task + 0x18), 0xB2));
            sndSetStationedSeVolume(0xD);
            btlDispatchStateHandler(task, 6);
            return;
        case 9:
            btlStartTask(btlCreateEffObjB(*(BtlUnit **)(task + 0x18), 0xD0));
            sndSetStationedSeVolume(0xD);
            btlDispatchStateHandler(task, 6);
            return;
        }
        break;
    }
    fldCreateSceneSpriteTask((s32)task);
}

extern s32 fldGetSceneScriptState(void);

extern BtlIndexList *fldGetSceneScriptValue(void);

extern s32 btlGetCommandTargetEligibility(BtlIndexList *indices, s32 selection);

extern void fldMarkActiveSceneScriptState(void);

extern s32 btlSetTaskPhase2(void);

void btlInitializeActorCommandState(BtlTask *task) {
    BtlState *work = (BtlState *)btlGetRuntime();
    s32 state;
    BtlIndexList *selected;
    s32 selection;
    s32 reason;

    if (work->battleFlags & 0x20) {
        return;
    }
    state = fldGetSceneScriptState();
    if ((task->flags & 4) == 0 && state != 3 && sndHasActiveActor() == 0) {
        if (btlGetActiveUnitId() != 9) {
            btlStartTask(btlCreateCommandSoundUpdateTask());
            btlStartTask(btlCreateSecondaryCommandSoundTask());
        }
        btlStartTask(btlCreateCommandSoundTask((s32)task, 0xA));
        task->flags |= 4;
    }
    if (state == 3) {
        selected = fldGetSceneScriptValue();
        switch (task->indexWork.phase) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 7:
        case 8:
            if (task->indexWork.phase == 4) {
                selection = btlGetLoggedIndexedCommandItem(task->indexWork.reference);
            } else {
                selection = task->indexWork.skillId;
            }
            reason = btlGetCommandTargetEligibility(selected, selection);
            switch (reason) {
            case 3:
            case 4:
                btlStartTask(btlCreateEffObjB(task->unit, 0x6A));
                sndSetStationedSeVolume(0xD);
                btlSetTaskPhase2();
                return;
            case 5:
                btlStartTask(btlCreateEffObjB(task->unit, 0xA6));
                sndSetStationedSeVolume(0xD);
                btlSetTaskPhase2();
                return;
            case 8:
                btlStartTask(btlCreateEffObjB(task->unit, 0xCC));
                sndSetStationedSeVolume(0xD);
                btlSetTaskPhase2();
                return;
            }
            break;
        }
        if (task->indexWork.phase != 9) {
            sndSetStationedSeVolume(8);
        }
        btlCopyIndexList(task->indexWork.indices, selected);
        fldSetSceneObjectAndGroupStates();
        fldMarkActiveSceneScriptState();
        if (btlAiCheckStatusRollEligibility(task) != 0) {
            btlDispatchStateHandler(task, 0xB);
        } else {
            btlDispatchStateHandler(task, 0xC);
        }
    } else if (state == 4) {
        fldMarkActiveSceneScriptState();
        btlDispatchStateHandler(task, 6);
    } else if (work->battleFlags & 0x8000) {
        fldSetSceneObjectAndGroupStates();
        fldMarkActiveSceneScriptState();
        btlDispatchStateHandler(task, 9);
    }
}

void func_001C9C20(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) & 0xffffff7f;
}

/* Bind the acting unit's AI slot once, then wait for its script task. */
s32 btlAiTaskUpdate(BtlTask *task) {
    BtlState *scene = (BtlState *)btlGetRuntime();
    u16 index;

    if (!(scene->battleFlags & 0x20)) {
        if (sndHasActiveActor() == 0) {
            if (fldCheckSceneResourcesIdle(task->unit) != 0) {
                if (!(task->flags & 0x80)) {
                    index = task->unit->partyRecord.unitId;
                    scene->boundTask = 0;
                    if (datEnemyAiRecords[index].kind != 1 && btlAllocAndCheck((s32)task) != 0) {
                        btlAssignTaskResultAndArgument((s32)task);
                    } else if (datEnemyAiRecords[index].slot != 0) {
                        btlBindActorSlot(task, datEnemyAiRecords[index].slot);
                    } else {
                        btlRunRandomWeightedAiTableAction(task);
                    }
                    task->flags |= 0x80;
                    scene->battleFlags &= ~0x100000;
                }
                if (scene->boundTask == 0) {
                    scene->battleFlags |= 0x100000;
                    if (btlAiCheckStatusRollEligibility(task) != 0) {
                        btlDispatchStateHandler(task, 0xB);
                    } else {
                        btlDispatchStateHandler(task, 0xC);
                    }
                } else if (kwlnTaskIsRegistered((KwlnTask *)scene->boundTask) == 0) {
                    if (task->indexWork.phase == -1) {
                        btlBossDebugPrintf("btl:AI script return NULL[%p]\n", task);
                        btlDebugPrintf("AI script return NULL\n");
                        btlRunRandomWeightedAiTableAction(task);
                    }
                    scene->battleFlags |= 0x100000;
                    if (btlAiCheckStatusRollEligibility(task) != 0) {
                        btlDispatchStateHandler(task, 0xB);
                    } else {
                        btlDispatchStateHandler(task, 0xC);
                    }
                    scene->boundTask = 0;
                }
            }
        }
    }
}

void btlMarkSceneTaskAfterReset(BtlTask *task) {
    func_001B83D8(task, 0, 0);
    task->flags |= 0x20;
}

s32 btlCommandStateSelectB(s32 arg0) {
    if (sndHasActiveActor() == 0) {
        btlPrepareSavedCommandTargets((BtlTask *)arg0, &((BtlTask *)arg0)->indexWork);
        if (btlAiCheckStatusRollEligibility(arg0) != 0) {
            btlDispatchStateHandler(arg0, 0xB);
        } else {
            btlDispatchStateHandler(arg0, 0xC);
        }
    }
}

void func_001C9EC0(void) {
}

void btlChooseActorStateFromFirstLinkedUnit(u8 *actor) {
    BtlUnit *model;
    if (sndHasActiveActor() != 0) {
        return;
    }
    model = (BtlUnit *)btlGetIndexListEntry(*(struct BtlIndexList **)(actor + 0x60), 0);
    if (*(u32 *)(actor + 0x20) == 1 &&
        (btlIsActiveActor((s32)model) == 0 ||
         (btlUnitStatusPair(model) & 0xE1) != 1)) {
        btlDispatchStateHandler(actor, 26);
    } else {
        btlDispatchStateHandler(actor, 12);
    }
}

void btlResetCommandIndexWork(BtlTask *task) {
    func_001ACC20();
    btlResetIndexWork(&task->indexWork);
    task->unit->unk2F4 = -1;
}

void btlCommandStartSoundTasks(u8 *task) {
    BtlUnit *unit;
    s64 ownerId;
    s32 effect;
    u8 *object;
    if (sndHasActiveActor() == 0 && btlCountTasksByKind(0x45) == 0) {
        unit = *(BtlUnit **)(task + 0x18);
        ownerId = btlAdvanceRuntimeSequenceCounter();
        btlStartTask(btlCreateCommandSoundUpdateTask());
        btlStartTask(btlCreateSecondaryCommandSoundTask());
        if ((u32)unit->status.flags & 0x200) {
            btlStartTask(btlCreateCommandSoundTask(task, 9));
        } else {
            btlStartTask(btlCreateCommandSoundTask(task, 3));
        }
        if ((*(u16 *)((u8 *)unit + 0x12E) & 0x7FFF) == 0x20) {
            if ((u32)((BtlTask *)task)->unit->status.flags & 0x200) {
                func_001FF0C8(task, 2);
            } else {
                func_001FF0C8(task, 3);
            }
            btlDispatchStateHandler(task, 0xC);
        }
        effect = func_001FD170(task);
        if (effect > 0) {
            object = (u8 *)btlCreateEffObjB(unit, effect);
            *(s64 *)(object + 0x40) = ownerId;
            btlStartTask(object);
        }
        *(u32 *)(task + 8) |= 0x200;
    }
}

extern char D_003A3648[]; /* "btl:command=%d\n" */

extern char D_003A3648[]; /* "btl:command=%d\n" */

void func_001D12A0(BtlTask *task, BattleIndexWork *work);

void btlCommandPrintAndFetchOwner(BtlTask *task) {
    BtlTask *link = task;
    BattleIndexWork *commandWork;
    s32 command;
    btlBossDebugPrintf(D_003A3648, link->indexWork.phase);
    commandWork = &link->indexWork;
    func_001D12A0(task, commandWork);
    command = commandWork->phase;
    if (command <= 0) {
        return;
    }
    if (command >= 4) {
        if (command >= 9) {
            return;
        }
        if (command < 7) {
            return;
        }
    }
    if (btlGetIndexListCount(link->indexWork.indices) == 1) {
        link->indexWork.ownerId = ((BtlUnit *)btlGetIndexListEntry(link->indexWork.indices, 0))->identity;
    }
}

void btlDispatchEffectCommandWhenActorReady(u8 *command) {
    u8 *actor = *(u8 **)(command + 0x18);
    if (fldCheckSceneResourcesIdle((BtlUnit *)actor) == 0) {
        return;
    }
    if (*(u16 *)(command + 0x50) == 2) {
        btlStartTask(btlCreateEffObjB((BtlUnit *)actor, *(u32 *)(command + 0x54)));
    }
    func_001F0CA0(command, command + 0x20);
    btlDispatchCommandViaHookOrDefault(command, command + 0x20);
}

void func_001CA1F0(void) {
}

/* ATTACK command scheduling; descriptive names inferred from retail consumers. */
extern s32 datRosterDetails;
extern f32 btlGetActorEffectScale(BtlTask *);
extern s32 btlGetSideIndexedActorStatusTable(s32, s32);
extern void *btlCreateTargetedCommandSoundTask(s32, s32, u32);
extern u8 *btlAllocateApproachTargetTask(u8 *, s32, f32);
extern s32 btlIsBattleRecordEligible(u8 *, u8 *, s32, s32);
extern SoundResourceNode *sndCreateResourceNode(SoundMixer *);
extern BtlRuntimeTask *func_001F12E8(SoundResourceNode *, BtlUnit *, BtlUnit *, u16);
extern const char D_003A3658[];
extern const char D_003A3670[];
extern s32 btlGetSlotRateKind(u8 *, s32);
extern s32 func_001A6AA0(BtlUnit *, s32);
extern s32 btlResolveSkillCategory(s32, u32);
extern s32 func_001D6050(BtlUnit *, s32);
extern s32 btlActionEntryIsEmpty(s32, BtlOperandGroup *, BtlOperandEntry *);
extern void *btlCreateDeferredActorStatsTask(BtlUnit *, BtlOperandEntry *);
extern u8 *btlCreateActorSoundOptionTask(BtlUnit *, s32);
extern u8 *btlScheduleEpPacketTask(u8 *, s32);
extern void *btlScheduleMoneyPacketTask(u8 *, u32);
extern void *btlCreateRefreshEligibleActorsTask(void);
extern void *btlCreateImmediateCompletionTask(void);
extern SoundTask *btlCreateUpdateUnitEffectsTask(void);
extern void *btlCreateWaitUnitListIdleTask(u32);
extern void *btlCreateApplyToActiveActorsTask(u32);
extern SoundTask *btlCreateFadeStateResetTask(void);
extern BtlRuntimeTask *btlCreateEffectCounterTask(BtlUnit *, s32);
extern BtlRuntimeTask *btlCreateEffectTask3E(BtlUnit *, u16);
extern BtlRuntimeTask *btlCreateEffObjC(BtlUnit *, s32);
extern BtlRuntimeTask *sndCreateTimedUnitEffectTask(SoundResourceNode *, BtlUnit *, u16, s32, u32);
extern SoundTask *sndCreateSetStateTask(void);
extern SoundTask *sndCreateClearStateTask(void);
extern void *btlScheduleActorUpdate(u8 *);
extern void *func_001D3618(BtlUnit *);
extern u8 *func_001D9E48(u8 *);
extern BtlRuntimeTask *sndCreateStationedSeTask(u32);
extern BtlRuntimeTask *btlCreateLinkedEffectTask(BtlUnit *, s32, u8);

void func_001CA1F8(BtlTask *action) {
    BtlState *state = (BtlState *)btlGetRuntime();
    BtlUnit *indexedTarget;
    BtlUnit *resultTarget = NULL;
    u8 *statusTable;
    BtlIndexList *indices;
    SoundResourceNode *resource;
    BtlOperandGroup *group;
    u64 masterOwner, operandOwner, commandOwner, moveHandle, lastOperandHandle;
    BtlRuntimeTask *operandTask;
    BtlRuntimeTask *task;
    BtlRuntimeTask *groupTask;
    s32 spacing;
    s32 repeatCount;
    u32 elapsed;
    BtlUnit *currentTarget;
    s32 reflectedStarted;
    s32 resourceNormal;
    s32 resourceTimed;
    s32 skipMovement;
    s32 categoryKind, extraKind;
    u32 targetCount, groupIndex, operandIndex, operandCount;
    s32 operandDelay, commandMap, actionFrames, groupDelay, initialDelay;
    s32 endDelay;
    s32 effectFrameOffset;
    f32 scale = btlGetActorEffectScale(action);

    resourceNormal = 0;
    resourceTimed = 0;
    endDelay = 0;
    skipMovement = 0;
    if (scale != 1.0f) {
        if ((btlUnitStatusPair(action->unit) & 0x1200) == 0x1200)
            action->indexWork.slot = 0x19;
        skipMovement = 1;
    }
    indices = action->indexWork.indices;
    targetCount = btlGetIndexListCount(indices);
    group = action->indexWork.groups;
    masterOwner = btlAdvanceRuntimeSequenceCounter();
    commandOwner = btlAdvanceRuntimeSequenceCounter();
    operandOwner = btlAdvanceRuntimeSequenceCounter();
    lastOperandHandle = btlAdvanceRuntimeSequenceCounter();
    statusTable = (u8 *)btlGetSideIndexedActorStatusTable(action->unit->resourceKind, action->unit->species);
    if (skipMovement == 0) {
        operandTask = btlCreateMoveOtherUnitsTask((u8 *)action->unit, action->indexWork.slot);
        btlStartTask(operandTask);
        moveHandle = operandTask->handle;
    } else {
        moveHandle = btlAdvanceRuntimeSequenceCounter();
    }
    task = (BtlRuntimeTask *)sndCreateSetStateTask();
    task->startCondition.kind = 4;
    task->startCondition.value.handle = moveHandle;
    btlStartTask(task);
    task = (BtlRuntimeTask *)btlCreateCommandSoundUpdateTask();
    task->startCondition.kind = 4;
    task->startCondition.value.handle = moveHandle;
    btlStartTask(task);
    task = (BtlRuntimeTask *)btlCreateSecondaryCommandSoundTask();
    task->startCondition.kind = 4;
    task->startCondition.value.handle = moveHandle;
    btlStartTask(task);
    task = (BtlRuntimeTask *)btlAllocateIndexedUnitEffectTask((u8 *)action->unit, action->indexWork.slot,
        btlGetSlotRateKind((u8 *)action->unit, action->indexWork.slot), scale);
    task->startCondition.kind = 4;
    task->startCondition.value.handle = moveHandle;
    btlStartTask(task);
    task = btlCreateTargetedCommandSoundTask((s32)action, 4, 0);
    task->startCondition.kind = 4;
    task->startCondition.value.handle = moveHandle;
    btlStartTask(task);
    if (action->unit->unk2F4 == -1)
        task = btlCreateEffObjD(action->unit, action->indexWork.skillId);
    else
        task = btlCreateEffObjD(action->unit, action->unit->unk2F4);
    task->startCondition.kind = 4;
    task->startCondition.value.handle = moveHandle;
    task->ownerId = btlAdvanceRuntimeSequenceCounter();
    btlStartTask(task);
    if (targetCount == 1) {
        indexedTarget = (BtlUnit *)btlGetIndexListEntry(indices, 0);
        if (btlIsBattleRecordEligible((u8 *)action->unit, (u8 *)indexedTarget,
                action->indexWork.slot, action->indexWork.skillId) != 0) {
            if ((action->unit->status.flags & indexedTarget->status.flags & 0x600) == 0 ||
                    (indexedTarget->status.flags & 0x200) != 0) {
                task = (BtlRuntimeTask *)btlAllocateApproachTargetTask((u8 *)action->unit,
                    (s32)indexedTarget, state->modelFrameScale);
                task->startCondition.kind = 4;
                task->startCondition.value.handle = moveHandle;
                btlStartTask(task);
            }
        }
    }
    resource = NULL;
    groupDelay = (s32)(func_001D6050(action->unit, action->indexWork.slot) / scale);
    initialDelay = groupDelay;
    btlBossDebugPrintf(D_003A3658, groupDelay);
    if ((action->unit->status.flags & 0x200) != 0 &&
            ((action->unit->status.flags & 0x1000) == 0 || action->indexWork.slot == 0x17)) {
        resource = action->unit->resourceNode = sndCreateResourceNode((SoundMixer *)action->unit->gunResource);
        if ((action->unit->status.flags & 0x1000) == 0) resourceNormal = 1;
        else resourceTimed = 1;
    }
    currentTarget = action->unit;
    reflectedStarted = 0;
    actionFrames = func_001A6AA0(action->unit, action->indexWork.skillId);
    commandMap = btlResolveSkillCategory((s32)action->unit, action->indexWork.skillId);
    for (groupIndex = 0; groupIndex < targetCount; groupIndex++, group++) {
        if (resourceNormal == 0 && resourceTimed == 0) {
            s32 resourceIndex = group->parameter != 2 ? 26 : 27;
            resource = state->resources[resourceIndex];
        }
        operandCount = group->count;
        indexedTarget = (BtlUnit *)btlGetIndexListEntry(indices, groupIndex);
        currentTarget = indexedTarget;
        if (((btlUnitStatusPair(action->unit) & 0x1200) == 0x1200 && action->indexWork.slot == 3) ||
                (action->unit->status.flags & 0x400) != 0) {
            if ((action->unit->status.flags & 0x200) != 0) {
                repeatCount = ((EventRosterStat *)datRosterDetails)[action->unit->partyRecord.unitId].attackCount;
                spacing = ((EventRosterStat *)datRosterDetails)[action->unit->partyRecord.unitId].attackSpacing;
            } else {
                repeatCount = datEnemyRecords[action->unit->species].tickCount;
                spacing = datEnemyRecords[action->unit->species].unk48;
                if (repeatCount <= 0) repeatCount = 1;
            }
            spacing = (s32)(spacing / *(f32 *)(statusTable + action->indexWork.slot * 20 + 0x34));
            spacing = (s32)(spacing / scale);
            if (spacing <= 0) spacing = 1;
            elapsed = 0xFFFFFF;
            if (group->kind != 2 && group->kind != 0x40000 && group->reflected == 0) {
                for (operandIndex = 1; operandIndex < repeatCount; operandIndex++) {
                    elapsed += spacing;
                    if (elapsed >= 3) {
                        elapsed = 0;
                        groupTask = func_001F12E8(resource, action->unit, currentTarget, 2);
                        groupTask->startCondition.kind = 4;
                        groupTask->startCondition.value.handle = moveHandle;
                        groupTask->startDelay = groupDelay - spacing * operandIndex;
                        groupTask->ownerId = masterOwner;
                        btlStartTask(groupTask);
                        groupTask = sndCreateStationedSeTask(group->parameter != 2 ? 0x1000A : 0x1000B);
                        groupTask->startCondition.kind = 4;
                        groupTask->startCondition.value.handle = moveHandle;
                        groupTask->startDelay = groupDelay - spacing * operandIndex;
                        groupTask->ownerId = masterOwner;
                        btlStartTask(groupTask);
                        operandTask = (BtlRuntimeTask *)btlAllocateIndexedUnitEffectTask((u8 *)currentTarget,
                            group->reactionCode, 0, 1.0f);
                        operandTask->startCondition.kind = 4;
                        operandTask->startCondition.value.handle = moveHandle;
                        operandTask->startDelay = groupDelay - spacing * operandIndex;
                        btlStartTask(operandTask);
                    }
                }
            } else if (group->kind == 2) {
                groupDelay -= spacing * (repeatCount - 1);
            }
        }
        if (group->reflected != 0) currentTarget = action->unit;
        if (group->targetSpecialHit != 0) {
            task = func_001D3618(indexedTarget);
            task->startDelay = groupDelay;
            btlStartTask(task);
        }
        if (group->sourceSpecialHit != 0) {
            task = func_001D3618(currentTarget);
            task->startDelay = groupDelay;
            btlStartTask(task);
        }
        if (group->inactiveOrStatusChanged != 0 && (state->battleFlags & 0x80) != 0) {
            endDelay = 18;
            btlBossDebugPrintf(D_003A3670, currentTarget);
            {
                u64 owner = action->unit->identity;
                endDelay = action->indexWork.unk2E != 0 ? endDelay : 0;
                commandOwner = owner;
            }
        } else {
            endDelay = 18;
            commandOwner = btlAdvanceRuntimeSequenceCounter();
            endDelay = skipMovement == 0 ? endDelay : 0;
        }
        if (group->kind != 0x40000) {
            if (group->kind != 2 && (group->reflected == 0 || reflectedStarted == 0)) {
                groupTask = func_001F12E8(resource, action->unit, currentTarget, 2);
                groupTask->startCondition.kind = 4;
                groupTask->startCondition.value.handle = moveHandle;
                groupTask->startDelay = groupDelay;
                groupTask->ownerId = masterOwner;
                btlStartTask(groupTask);
                if (resourceNormal == 0 && resourceTimed == 0) {
                    groupTask = sndCreateStationedSeTask(group->parameter != 2 ? 0x1000A : 0x1000B);
                    groupTask->startCondition.kind = 4;
                    groupTask->startCondition.value.handle = moveHandle;
                    groupTask->startDelay = groupDelay;
                    groupTask->ownerId = masterOwner;
                    btlStartTask(groupTask);
                }
                if (group->reflected != 0) reflectedStarted = 1;
            } else {
                groupTask = btlCreateImmediateCompletionTask();
                groupTask->startCondition.kind = 4;
                groupTask->startCondition.value.handle = moveHandle;
                groupTask->startDelay = groupDelay;
                groupTask->ownerId = masterOwner;
                btlStartTask(groupTask);
            }
        } else {
            groupTask = func_001F12E8(state->resources[30], action->unit, currentTarget, 2);
            groupTask->startCondition.kind = 4;
            groupTask->startCondition.value.handle = moveHandle;
            groupTask->startDelay = groupDelay;
            groupTask->ownerId = masterOwner;
            btlStartTask(groupTask);
            groupTask = sndCreateStationedSeTask(0x1000D);
            groupTask->startCondition.kind = 4;
            groupTask->startCondition.value.handle = moveHandle;
            groupTask->startDelay = groupDelay;
            groupTask->ownerId = masterOwner;
            btlStartTask(groupTask);
        }
        if (group->reflected != 0) {
            groupTask = func_001F12E8(state->resources[28], action->unit, indexedTarget, 2);
            groupTask->startCondition.kind = 4;
            groupTask->startCondition.value.handle = moveHandle;
            groupTask->startDelay = groupDelay;
            groupTask->ownerId = masterOwner;
            btlStartTask(groupTask);
            groupTask = sndCreateStationedSeTask(0x1000C);
            groupTask->startCondition.kind = 4;
            groupTask->startCondition.value.handle = moveHandle;
            groupTask->startDelay = groupDelay;
            groupTask->ownerId = masterOwner;
            btlStartTask(groupTask);
        }
        for (operandIndex = 0, operandDelay = 0, effectFrameOffset = 0; operandIndex < operandCount; operandIndex++) {
            s32 selection;
            if (btlActionEntryIsEmpty(action->indexWork.skillId, group, &group->entries[operandIndex]) != 0) {
                selection = -1;
                categoryKind = 4;
                extraKind = 1;
            } else {
                selection = group->reactionCode;
                categoryKind = group->kind;
                extraKind = group->parameter;
            }
            if (resourceTimed == 0) {
                operandTask = (BtlRuntimeTask *)btlAllocateIndexedUnitEffectTask((u8 *)currentTarget, selection, 0, 1.0f);
                operandTask->startCondition.kind = 4;
                operandTask->startCondition.value.handle = moveHandle;
                operandTask->startDelay = groupTask->startDelay + operandDelay;
                btlStartTask(operandTask);
            } else {
                operandTask = sndCreateTimedUnitEffectTask(resource, currentTarget, 2, selection, 0);
                operandTask->startCondition.kind = 5;
                operandTask->startCondition.value.handle = groupTask->handle;
                operandTask->startDelay = operandDelay;
                btlStartTask(operandTask);
            }
            lastOperandHandle = operandTask->handle;
            if (group->kind == 2 && operandIndex == 0) {
                task = (BtlRuntimeTask *)func_001D9E48((u8 *)currentTarget);
                task->startCondition.kind = 4;
                task->startCondition.value.handle = operandTask->handle;
                btlStartTask(task);
            }
            if (action->indexWork.stage == 1 && groupIndex == 0 && operandIndex == 0) {
                BtlRuntimeTask *stageTask;
                effectFrameOffset += 0xC;
                stageTask = btlCreateEffObjB(action->unit, action->indexWork.parameter);
                stageTask->startCondition.kind = 4;
                stageTask->startCondition.value.handle = operandTask->handle;
                btlStartTask(stageTask);
            }
            if (action->indexWork.unk50 != 0 && groupIndex == 0 && operandIndex == operandCount - 1) {
                task = btlCreateEffectTask3E(action->unit, (u16)action->indexWork.unk50);
                task->startCondition.kind = 4;
                task->startDelay = effectFrameOffset + 0xC;
                task->startCondition.value.handle = operandTask->handle;
                btlStartTask(task);
            }
            if ((indexedTarget->status.flags & 0x10) != 0 && operandIndex == 0) {
                task = btlScheduleActorUpdate((u8 *)indexedTarget);
                task->startCondition.kind = 4;
                task->startCondition.value.handle = operandTask->handle;
                btlStartTask(task);
            }
            if (groupIndex == 0 && operandIndex == operandCount - 1) {
                task = (BtlRuntimeTask *)fldCreateSceneGroupAction((u8 *)action, action->indexWork.adjustedValue, (u8)action->indexWork.resultKind);
                task->startCondition.kind = 4;
                task->startCondition.value.handle = operandTask->handle;
                task->endDelay = endDelay;
                task->ownerId = action->unit->identity;
                btlStartTask(task);
            }
            if (groupIndex == targetCount - 1 && operandIndex == operandCount - 1) {
                task = (BtlRuntimeTask *)btlCreateActorSoundOptionTask(action->unit, action->indexWork.unk50);
                task->startCondition.kind = 4;
                task->startCondition.value.handle = operandTask->handle;
                task->ownerId = commandOwner;
                btlStartTask(task);
            }
            if (groupIndex == 0 && operandIndex == operandCount - 1) {
                task = (BtlRuntimeTask *)btlScheduleEpPacketTask((u8 *)action->unit, action->indexWork.unk54);
                task->startCondition.kind = 4;
                                task->startCondition.value.handle = operandTask->handle;
                task->ownerId = commandOwner;
                btlStartTask(task);
                task = btlScheduleMoneyPacketTask((u8 *)action->unit, action->indexWork.unk58);
                task->startCondition.kind = 4;
                task->startCondition.value.handle = operandTask->handle;
                task->ownerId = commandOwner;
                btlStartTask(task);
            }

            task = (BtlRuntimeTask *)btlCreateActorParameterDeltaTask(currentTarget, &group->entries[operandIndex]);
            task->startCondition.kind = 4;
                task->startCondition.value.handle = operandTask->handle;
                task->ownerId = commandOwner;
            btlStartTask(task);
            task = btlCreateDeferredActorStatsTask(action->unit, &group->entries[operandIndex]);
            task->startCondition.kind = 4;
            task->startDelay = commandMap;
            task->startCondition.value.handle = operandTask->handle;
            task->ownerId = commandOwner;
            btlStartTask(task);

            if (group->entries[operandIndex].hpDelta != 0) {
                task = btlCreateLinkedEffectTask(currentTarget, group->entries[operandIndex].hpDelta, 0);
                task->startCondition.kind = 4;
                task->startDelay = 1;
                task->startCondition.value.handle = operandTask->handle;
                task->ownerId = operandOwner;
                btlStartTask(task);
            }
            if (group->entries[operandIndex].mpDelta != 0) {
                task = btlCreateLinkedEffectTask(currentTarget, group->entries[operandIndex].mpDelta, 1);
                task->startCondition.kind = 4;
                task->startDelay = 1;
                task->startCondition.value.handle = operandTask->handle;
                task->ownerId = operandOwner;
                btlStartTask(task);
            }
            if (group->entries[operandIndex].hpRecovery != 0) {
                task = btlCreateLinkedEffectTask(action->unit, group->entries[operandIndex].hpRecovery, 0);
                task->startCondition.kind = 4;
                task->startDelay = commandMap;
                task->startCondition.value.handle = operandTask->handle;
                task->ownerId = operandOwner;
                btlStartTask(task);
            }
            if (group->entries[operandIndex].mpRecovery != 0) {
                task = btlCreateLinkedEffectTask(action->unit, group->entries[operandIndex].mpRecovery, 1);
                task->startCondition.kind = 4;
                task->startDelay = commandMap;
                task->startCondition.value.handle = operandTask->handle;
                task->ownerId = operandOwner;
                btlStartTask(task);
            }

            if ((group->entries[operandIndex].flags & 0x1000) == 0) {
                if (categoryKind == 0x10000 || categoryKind == 4) {
                    if (categoryKind == 0x10000) task = btlCreateEffectCounterTask(currentTarget, 4);
                    else task = btlCreateEffectCounterTask(currentTarget, 3);
                    task->startCondition.kind = 4;
                    task->startDelay = 1;
                    task->startCondition.value.handle = operandTask->handle;
                    task->ownerId = operandOwner;
                    btlStartTask(task);
                }
                if (extraKind == 2 || extraKind == 4) {
                    if (extraKind == 2) task = btlCreateEffectCounterTask(currentTarget, 1);
                    else task = btlCreateEffectCounterTask(currentTarget, 2);
                    task->startCondition.kind = 4;
                    task->startDelay = 1;
                    task->startCondition.value.handle = operandTask->handle;
                    task->ownerId = operandOwner;
                    btlStartTask(task);
                }
            }
            if ((group->entries[operandIndex].addedStatus & 1) != 0 && currentTarget->partyRecord.status == 0) {
                resultTarget = currentTarget;
            }
            operandDelay += group->interval;
        }
        if (state->postTargetHook != NULL)
            state->postTargetHook(action, action->indexWork.skillId, indexedTarget, masterOwner, lastOperandHandle, -1);
        groupDelay += actionFrames;
    }
    if (action->indexWork.unk5E != 0) {
        task = btlCreateRefreshEligibleActorsTask();
        task->startCondition.kind = 7;
        task->startCondition.value.handle = commandOwner;
        btlStartTask(task);
    }
    if (resourceNormal != 0 && resource != NULL) {
        task = func_001F12E8(resource, action->unit, currentTarget, 1);
        task->startCondition.kind = 4;
        task->startCondition.value.handle = moveHandle;
        task->startDelay = initialDelay;
        task->ownerId = masterOwner;
        btlStartTask(task);
    }
    if (resultTarget != NULL && action->indexWork.wait >= 0 && (action->flags & 0x20) == 0) {
        task = btlCreateEffObjC(resultTarget, action->indexWork.wait);
        task->startCondition.kind = 7;
        task->startCondition.value.handle = masterOwner;
        btlStartTask(task);
    }
    if (skipMovement == 0) {
        task = (BtlRuntimeTask *)btlCreateUpdateUnitEffectsTask();
        task->startCondition.kind = 7;
        task->startCondition.value.handle = masterOwner;
        btlStartTask(task);
    }
    task = btlCreateWaitUnitListIdleTask(12);
    task->startCondition.kind = 7;
    task->startCondition.value.handle = masterOwner;
    task->startDelay = endDelay;
    btlStartTask(task);
    task = btlCreateApplyToActiveActorsTask(12);
    task->startCondition.kind = 7;
    task->startCondition.value.handle = masterOwner;
    task->startDelay = endDelay;
    btlStartTask(task);
    task = (BtlRuntimeTask *)sndCreateClearStateTask();
    task->startCondition.kind = 7;
    task->startCondition.value.handle = masterOwner;
    task->startDelay = endDelay;
    btlStartTask(task);
    task = (BtlRuntimeTask *)btlCreateFadeStateResetTask();
    task->startCondition.kind = 7;
    task->startCondition.value.handle = masterOwner;
    task->startDelay = endDelay;
    btlStartTask(task);
    if ((action->unit->partyRecord.status & 0x480) != 0) btlDispatchStateHandler(action, 24);
    else btlDispatchStateHandler(action, 26);
}

void func_001CB408(void) {
}

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3648);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3658);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3670);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001CB410);

void func_001CCD10(void) {
}

typedef struct BtlSceneLightParams {
    f32 position[3]; f32 unk0C;
    f32 color[3]; f32 unk1C;
    f32 secondaryColor[3]; f32 unk2C;
} BtlSceneLightParams;
extern char D_003A36A8[];
extern BtlRuntimeTask *sndCreateReleaseTask(u32);
extern BtlRuntimeTask *func_001F0920(u32);
extern u64 btlAdvanceRuntimeSequenceCounter(void);
extern s32 btlGetSlotRateKind(u8 *, s32);
extern s32 func_001A6AA0(BtlUnit *, s32);
extern s32 btlResolveSkillCategory(s32, u32);
extern s32 func_001D6050(BtlUnit *, s32);
extern s32 btlActionEntryIsEmpty(s32, BtlOperandGroup *, BtlOperandEntry *);
extern void *btlCreateDeferredActorStatsTask(BtlUnit *, BtlOperandEntry *);
extern u8 *btlCreateActorSoundOptionTask(BtlUnit *, s32);
extern u8 *btlScheduleEpPacketTask(u8 *, s32);
extern void *btlScheduleMoneyPacketTask(u8 *, u32);
extern void *btlCreateRefreshEligibleActorsTask(void);
extern void *btlCreateImmediateCompletionTask(void);
extern SoundTask *btlCreateUpdateUnitEffectsTask(void);
extern void *btlCreateWaitUnitListIdleTask(u32);
extern void *btlCreateApplyToActiveActorsTask(u32);
extern SoundTask *btlCreateFadeStateResetTask(void);
extern BtlRuntimeTask *btlCreateEffectCounterTask(BtlUnit *, s32);
extern BtlRuntimeTask *btlCreateEffectTask3E(BtlUnit *, u16);
extern BtlRuntimeTask *btlCreateEffObjC(BtlUnit *, s32);
extern BtlRuntimeTask *btlCreateEffObjD(BtlUnit *, s32);
extern BtlRuntimeTask *sndCreateTimedUnitEffectTask(
    SoundResourceNode *, BtlUnit *, u16, s32, u32);
extern SoundTask *sndCreateSetStateTask(void);
extern SoundTask *sndCreateClearStateTask(void);
extern void *btlScheduleActorUpdate(u8 *);
extern void *func_001D3618(BtlUnit *);
extern u8 *func_001D9E48(u8 *);
extern const char D_003A3670[];

extern BtlRuntimeTask *sndCreateStationedSeTask(u32);
void btlBuildActionLightParameters(s32 index, BtlSceneLightParams *light);
extern void *btlCreateEffectTaskWithSourceParams(u8 *, u32);
s32 btlGetActionDefaultOrOverride(s32 index);
extern u8 *btlCreateSoundPlaybackTask(u8 *, u32, u32, u32, u32);
extern void *btlCreateCommandSoundWithArguments(s32, s32, u32, u32, u32);
s32 sndLookupResourceType(s32 actor, s32 resourceIndex);
BtlRuntimeTask *sndCreateActorEffectTask(SoundResourceNode *source,
        BtlUnit *owner, u32 duration);
s32 sndMapResourceType(s32 category, s32 id);
extern void *btlCreateCategoryStatDamageTask(u8 *, u32, u32);
extern u8 *btlCreatePermittedBattleVoiceTask(s32, s32);
s32 btlFormatActionEventFilename(s32 source, char *output);
extern BtlRuntimeTask *sndCreateEffectLoadTask(SoundResourceNode *, const char *);
struct ActiveSoundNode *sndAllocListNode(void);
extern BtlRuntimeTask *sndCreateEffectWithTargets(SoundResourceNode *, BtlUnit *, s32, s32, BtlUnit *, u16);
extern SoundTask *sndCreateSkillSeTask(s32, u16);
extern void *func_001D3400(BtlUnit *, BtlOperandEntry *);
extern void *func_001D3510(BtlUnit *, BtlOperandEntry *);

extern BtlRuntimeTask *btlCreateLinkedEffectTask(BtlUnit *, s32, u8);

/* Schedule the LINKAGE action task chain (inferred role). */
INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A36A8);

void func_001CCD18(BtlTask *action) {
    BtlUnit *actors[3];
    BtlSceneLightParams light;
    s32 actorFrames[3];
    char resourceName[0x70];
    BtlState *state;
    BtlUnit *unit = action->unit;
    BtlUnit *companionA = action->indexWork.companionA;
    BtlUnit *companionB = action->indexWork.companionB;
    BtlUnit *indexedTarget;
    BtlUnit *currentTarget;
    BtlOperandGroup *groups;
    BtlUnit *effectTarget = unit;
    BtlUnit *resultTarget = NULL;
    BtlIndexList *indices;
    BtlRuntimeTask *task;
    BtlRuntimeTask *groupTask;
    u64 masterOwner, actionOwner, operandOwner, commandOwner, soundHandle, latestOperandHandle;
    s32 slot, maxFrames;
    s32 reflectedStarted, blendStarted;
    u32 groupIndex, operandIndex, targetCount, operandCount;
    s32 operandDelay, commandMap, skill, groupDelay, actionFrames;
    s32 categoryKind, extraKind, effectFrameOffset, mode;
    actors[0] = unit;
    actors[1] = companionA;
    actors[2] = companionB;
    if ((unit->status.flags & 0x1000) != 0) {
        if (companionA != NULL && (companionA->status.flags & 0x1000) == 0) {
            actors[0] = companionA;
            actors[1] = unit;
        } else if (companionB != NULL && (companionB->status.flags & 0x1000) == 0) {
            actors[0] = companionB;
            actors[1] = unit;
            actors[2] = companionA;
        }
    }
    state = (BtlState *)btlGetRuntime();
    indices = action->indexWork.indices;
    targetCount = btlGetIndexListCount(indices);
    maxFrames = 0;
    mode = -1;
    groups = action->indexWork.groups;
    slot = action->indexWork.slot;
    skill = action->indexWork.skillId;
    masterOwner = btlAdvanceRuntimeSequenceCounter();
    actionOwner = btlAdvanceRuntimeSequenceCounter();
    commandOwner = btlAdvanceRuntimeSequenceCounter();
    operandOwner = btlAdvanceRuntimeSequenceCounter();
    soundHandle = btlAdvanceRuntimeSequenceCounter();
    latestOperandHandle = btlAdvanceRuntimeSequenceCounter();
    btlStartTask(sndCreateSetStateTask());
    for (groupIndex = 0; groupIndex < 3; groupIndex++) {
        if (actors[groupIndex] != NULL) {
            actorFrames[groupIndex] = func_001D6050(actors[groupIndex], slot);
            if (actorFrames[groupIndex] < 20) actorFrames[groupIndex] = 20;
            if (maxFrames < actorFrames[groupIndex]) maxFrames = actorFrames[groupIndex];
        }
    }
    btlBossDebugPrintf(D_003A36A8, maxFrames);
    btlStartTask(btlCreateCommandSoundUpdateTask());
    btlStartTask(btlCreateSecondaryCommandSoundTask());
    btlBuildActionLightParameters(skill, &light);
    btlStartTask(btlCreateEffectTaskWithSourceParams((u8 *)&light, 12));
    btlStartTask(sndCreateAcquireTask(btlGetActionDefaultOrOverride(skill), 12));
    if ((unit->status.flags & 0x400) != 0) {
        btlStartTask(btlCreateEffObjB(unit, 0xC9));
        task = btlCreateEffObjD(unit, skill);
        task->startDelay = action->indexWork.unk2D == 0 ? 24 : 12;
        task->ownerId = actionOwner;
        btlStartTask(task);
    } else {
        task = btlCreateEffObjD(unit, skill);
        task->ownerId = actionOwner;
        btlStartTask(task);
    }
    btlStartTask(btlCreateSoundPlaybackTask((u8 *)action, (u32)companionA, (u32)companionB, skill, 0));
    btlStartTask(btlCreateCommandSoundWithArguments((s32)action, (s32)companionA, (s32)companionB, 7, skill));
    for (groupIndex = 0; groupIndex < 3; groupIndex++) {
        if (actors[groupIndex] != NULL) {
            s32 resourceIndex;
            if (action->indexWork.unk2D == 0) {
                BtlRuntimeTask *soundTask = btlCreateMoveOtherUnitsTask((u8 *)actors[groupIndex], slot);
                soundTask->startDelay = maxFrames - actorFrames[groupIndex];
                btlStartTask(soundTask);
                soundHandle = soundTask->handle;
            }
            task = (BtlRuntimeTask *)btlAllocateIndexedUnitEffectTask((u8 *)actors[groupIndex], slot,
                btlGetSlotRateKind((u8 *)actors[groupIndex], slot), 1.0f);
            task->startCondition.kind = 4;
            task->startCondition.value.handle = soundHandle;
            btlStartTask(task);
            resourceIndex = sndLookupResourceType((s32)actors[groupIndex], skill);
            if (resourceIndex != 0) {
                btlStartTask(sndCreateActorEffectTask(state->resources[resourceIndex], actors[groupIndex], maxFrames + 6));
                if (groupIndex == 0) {
                    s32 sound = sndMapResourceType((s32)actors[0], skill);
                    if (sound != -1) btlStartTask(sndCreateStationedSeTask(sound));
                }
            }
            btlStartTask(btlCreateCategoryStatDamageTask((u8 *)actors[groupIndex], skill, action->indexWork.stageValue));
            if (action->indexWork.phase == 4) {
                btlStartTask(btlCreatePermittedBattleVoiceTask((s32)actors[groupIndex], action->indexWork.reference));
            }
        }
    }
    btlFormatActionEventFilename(skill, resourceName);
    unit->resourceNode = sndAllocResourceNode();
    task = sndCreateEffectLoadTask(unit->resourceNode, resourceName);
    task->startCondition.kind = 4;
    task->startDelay = maxFrames > 0 ? maxFrames - 1 : 0;
    task->startCondition.value.handle = soundHandle;
    task->ownerId = masterOwner;
    btlStartTask(task);
    sndFormatResourceNameFromIndex(skill, resourceName);
    unit->listNode = sndAllocListNode();
    task = sndCreateFileLoadTask((struct SoundLoadNode *)unit->listNode, skill, resourceName);
    task->startCondition.kind = 4;
    task->startCondition.value.handle = soundHandle;
    task->ownerId = masterOwner;
    btlStartTask(task);
    if (action->indexWork.stage == 3) {
        task = btlCreateEffObjB(unit, action->indexWork.parameter);
        task->startCondition.kind = 7;
        task->startCondition.value.handle = masterOwner;
        if (action->indexWork.unk2D == 0) task->ownerId = actionOwner;
        btlStartTask(task);
    }
    if (action->indexWork.unk2D == 0) {
        task = btlCreateCommandSoundWithArguments((s32)action, (s32)companionA, (s32)companionB, 8, skill);
        task->startCondition.kind = 7;
        task->startCondition.value.handle = masterOwner;
        task->flags |= 2;
        if (state->commandSoundDelay != NULL) task->startDelay = state->commandSoundDelay(skill);
        btlStartTask(task);
        if (state->preActionHook != NULL) state->preActionHook(action, skill, actionOwner, soundHandle, masterOwner);
        actionFrames = func_001A6AA0(unit, skill);
        commandMap = btlResolveSkillCategory((s32)unit, skill);
        blendStarted = 0;
        reflectedStarted = 0;
        groupDelay = 0;
        for (groupIndex = 0; groupIndex < targetCount; groupIndex++, groups++) {
            BtlOperandGroup *group = groups;
            currentTarget = unit;
            operandCount = group->count;
            indexedTarget = (BtlUnit *)btlGetIndexListEntry(indices, groupIndex);
            if (group->reflected == 0) currentTarget = indexedTarget;
            if (group->reflected != 0 || group->kind == 0x40000)
                mode = ((BtlActionAnimationRecord *)datActionAnimationRecords)[skill].effectKind;
            effectTarget = mode == 3 ? indexedTarget : currentTarget;
            if (group->inactiveOrStatusChanged != 0 && (state->battleFlags & 0x80) != 0) {
                btlBossDebugPrintf(D_003A3670, currentTarget);
                commandOwner = unit->identity;
            } else commandOwner = btlAdvanceRuntimeSequenceCounter();
            if (group->targetSpecialHit != 0) {
                task = func_001D3618(indexedTarget);
                task->startCondition.kind = 7;
                task->startCondition.value.handle = commandOwner;
                btlStartTask(task);
            }
            if (group->sourceSpecialHit != 0) {
                task = func_001D3618(currentTarget);
                task->startCondition.kind = 7;
                task->startCondition.value.handle = commandOwner;
                btlStartTask(task);
            }
            if ((group->reflected == 0 || reflectedStarted == 0) && mode != 1) {
                groupTask = sndCreateEffectWithTargets(unit->resourceNode, actors[0], (s32)actors[1], (s32)actors[2], effectTarget, 2);
                groupTask->startCondition.kind = 7;
                groupTask->startCondition.value.handle = masterOwner;
                groupTask->startDelay = groupDelay;
                groupTask->ownerId = actionOwner;
                btlStartTask(groupTask);
                task = (BtlRuntimeTask *)sndCreateSkillSeTask((s32)unit->listNode, 2);
                task->startCondition.kind = 7;
                task->startCondition.value.handle = masterOwner;
                task->startDelay = groupDelay;
                task->ownerId = actionOwner;
                btlStartTask(task);
                if (group->reflected != 0 && mode != 3) reflectedStarted = 1;
            } else {
                groupTask = btlCreateImmediateCompletionTask();
                groupTask->startCondition.kind = 7;
                groupTask->startCondition.value.handle = masterOwner;
                groupTask->startDelay = groupDelay;
                groupTask->ownerId = actionOwner;
                btlStartTask(groupTask);
            }
            for (operandIndex = 0, operandDelay = 0, effectFrameOffset = 0; operandIndex < operandCount; operandIndex++) {
                s32 selection;
                BtlRuntimeTask *operandTask;
                if (btlActionEntryIsEmpty(skill, group, &group->entries[operandIndex]) != 0) {
                    selection = -1;
                    categoryKind = 4;
                    extraKind = 1;
                } else {
                    selection = group->reactionCode;
                    categoryKind = group->kind;
                    extraKind = group->parameter;
                }
                operandTask = sndCreateTimedUnitEffectTask(unit->resourceNode, currentTarget, 2, (u32)(mode - 3) < 2 ? -1 : selection, 0);
                operandTask->startCondition.kind = 5;
                operandTask->startDelay = operandDelay;
                operandTask->startCondition.value.handle = groupTask->handle;
                btlStartTask(operandTask);
                if (group->reflected != 0 && (u32)(mode - 3) < 2 && operandIndex == operandCount - 1 && ((u32)actionFrames >= 2 || blendStarted == 0)) {
                    if (targetCount == 1) {
                        task = btlCreateActorModelBlendTask(currentTarget, indexedTarget, selection, 0, 1.0f);
                    } else {
                        task = btlCreateActorModelBlendTask(currentTarget, 0, selection, 0, 1.0f);
                    }
                    task->startCondition.kind = 4;
                    blendStarted = 1;
                    task->startCondition.value.handle = operandTask->handle;
                    btlStartTask(task);
                }
                latestOperandHandle = operandTask->handle;
                if (group->kind == 2 && operandIndex == 0) {
                    task = (BtlRuntimeTask *)func_001D9E48((u8 *)currentTarget);
                    task->startCondition.kind = 4;
                    task->startCondition.value.handle = operandTask->handle;
                    btlStartTask(task);
                }
                if (action->indexWork.stage == 1 && groupIndex == 0 && operandIndex == 0) {
                    BtlRuntimeTask *stageTask;
                    effectFrameOffset += 0xC;
                    stageTask = btlCreateEffObjB(unit, action->indexWork.parameter);
                    stageTask->startCondition.kind = 4;
                    stageTask->startCondition.value.handle = operandTask->handle;
                    btlStartTask(stageTask);
                }
                if (action->indexWork.unk50 != 0 && groupIndex == 0 && operandIndex == operandCount - 1) {
                    task = btlCreateEffectTask3E(unit, (u16)action->indexWork.unk50);
                    task->startCondition.kind = 4;
                    task->startDelay = effectFrameOffset + 0xC;
                    task->startCondition.value.handle = operandTask->handle;
                    btlStartTask(task);
                }
                if ((indexedTarget->status.flags & 0x10) != 0 && operandIndex == 0) {
                    task = btlScheduleActorUpdate((u8 *)indexedTarget);
                    task->startCondition.kind = 4;
                    task->startCondition.value.handle = operandTask->handle;
                    btlStartTask(task);
                }
                if (groupIndex == targetCount - 1 && operandIndex == 0) {
                    task = btlCreateImmediateCompletionTask();
                    task->startCondition.kind = 4;
                    task->startCondition.value.handle = operandTask->handle;
                    task->startDelay = action->indexWork.unk2E == 0 ? 10 : 18;
                    task->ownerId = unit->identity;
                    btlStartTask(task);
                }
                task = (BtlRuntimeTask *)btlCreateActorParameterDeltaTask(currentTarget, &group->entries[operandIndex]);
                task->startCondition.kind = 4;
                task->startCondition.value.handle = operandTask->handle;
                task->ownerId = commandOwner;
                btlStartTask(task);
                task = btlCreateDeferredActorStatsTask(unit, &group->entries[operandIndex]);
                task->startCondition.kind = 4;
                task->startDelay = commandMap;
                task->startCondition.value.handle = operandTask->handle;
                task->ownerId = commandOwner;
                btlStartTask(task);

                task = func_001D3400(currentTarget, &group->entries[operandIndex]);
                task->startCondition.kind = 4;
                task->startCondition.value.handle = operandTask->handle;
                task->ownerId = commandOwner;
                btlStartTask(task);
                task = func_001D3510(currentTarget, &group->entries[operandIndex]);
                task->startCondition.kind = 4;
                task->startCondition.value.handle = operandTask->handle;
                task->ownerId = commandOwner;
                btlStartTask(task);
                if (groupIndex == targetCount - 1 && operandIndex == 0) {
                    task = (BtlRuntimeTask *)btlCreateActorSoundOptionTask(unit, action->indexWork.unk50);
                    task->startCondition.kind = 4;
                    task->startCondition.value.handle = operandTask->handle;
                    task->ownerId = commandOwner;
                    btlStartTask(task);
                }
                if (groupIndex == 0 && operandIndex == operandCount - 1) {
                    task = (BtlRuntimeTask *)btlScheduleEpPacketTask((u8 *)unit, action->indexWork.unk54);
                    task->startCondition.kind = 4;
                    task->startCondition.value.handle = operandTask->handle;
                    task->ownerId = commandOwner;
                    btlStartTask(task);
                    task = btlScheduleMoneyPacketTask((u8 *)unit, action->indexWork.unk58);
                    task->startCondition.kind = 4;
                    task->startCondition.value.handle = operandTask->handle;
                    task->ownerId = commandOwner;
                    btlStartTask(task);
                }

                if (group->entries[operandIndex].hpDelta != 0) {
                    task = btlCreateLinkedEffectTask(currentTarget, group->entries[operandIndex].hpDelta, 0);
                    task->startCondition.kind = 4;
                    task->startDelay = 1;
                    task->startCondition.value.handle = operandTask->handle;
                    task->ownerId = operandOwner;
                    btlStartTask(task);
                }
                if (group->entries[operandIndex].mpDelta != 0) {
                    task = btlCreateLinkedEffectTask(currentTarget, group->entries[operandIndex].mpDelta, 1);
                    task->startCondition.kind = 4;
                    task->startDelay = 1;
                    task->startCondition.value.handle = operandTask->handle;
                    task->ownerId = operandOwner;
                    btlStartTask(task);
                }
                if (group->entries[operandIndex].hpRecovery != 0) {
                    task = btlCreateLinkedEffectTask(unit, group->entries[operandIndex].hpRecovery, 0);
                    task->startCondition.kind = 4;
                    task->startDelay = commandMap;
                    task->startCondition.value.handle = operandTask->handle;
                    task->ownerId = operandOwner;
                    btlStartTask(task);
                }
                if (group->entries[operandIndex].mpRecovery != 0) {
                    task = btlCreateLinkedEffectTask(unit, group->entries[operandIndex].mpRecovery, 1);
                    task->startCondition.kind = 4;
                    task->startDelay = commandMap;
                    task->startCondition.value.handle = operandTask->handle;
                    task->ownerId = operandOwner;
                    btlStartTask(task);
                }

                if ((group->entries[operandIndex].flags & 0x1000) == 0) {
                    if (categoryKind == 0x10000 || categoryKind == 4) {
                        if (categoryKind == 0x10000) task = btlCreateEffectCounterTask(currentTarget, 4);
                        else task = btlCreateEffectCounterTask(currentTarget, 3);
                        task->startCondition.kind = 4;
                        task->startDelay = 1;
                        task->startCondition.value.handle = operandTask->handle;
                        task->ownerId = operandOwner;
                        btlStartTask(task);
                    }
                    if (extraKind == 2 || extraKind == 4) {
                        if (extraKind == 2) task = btlCreateEffectCounterTask(currentTarget, 1);
                        else task = btlCreateEffectCounterTask(currentTarget, 2);
                        task->startCondition.kind = 4;
                        task->startDelay = 1;
                        task->startCondition.value.handle = operandTask->handle;
                        task->ownerId = operandOwner;
                        btlStartTask(task);
                    }
                }
                if ((group->entries[operandIndex].addedStatus & 1) != 0 && currentTarget->partyRecord.status == 0) {
                    resultTarget = currentTarget;
                }
                operandDelay += group->interval;
            }
            if (state->postTargetHook != NULL) state->postTargetHook(action, skill, indexedTarget, masterOwner, latestOperandHandle, mode);
            if (group->kind == 0x40000) {
                BtlRuntimeTask *effect = sndCreateEffectWithTargets(state->resources[30], actors[0], (s32)actors[1], (s32)actors[2], currentTarget, 2);
                switch (mode) {
                case 1:
                    effect->startCondition.kind = 5;
                    effect->startCondition.value.handle = groupTask->handle;
                    break;
                case 2: case 3: case 4:
                    effect->startCondition.kind = 4;
                    effect->startCondition.value.handle = latestOperandHandle;
                    break;
                case 0: default:
                    effect->startCondition.kind = 5;
                    effect->startCondition.value.handle = groupTask->handle;
                    break;
                }
                effect->ownerId = actionOwner;
                btlStartTask(effect);
                task = sndCreateStationedSeTask(0x1000D);
                task->startCondition.kind = 5;
                task->startCondition.value.handle = effect->handle;
                task->ownerId = actionOwner;
                btlStartTask(task);
            }
            if (group->reflected != 0) {
                BtlRuntimeTask *effect = sndCreateEffectWithTargets(state->resources[29], actors[0], (s32)actors[1], (s32)actors[2], indexedTarget, 2);
                switch (mode) {
                case 1:
                    effect->startCondition.kind = 5;
                    effect->startCondition.value.handle = groupTask->handle;
                    break;
                case 2: case 3: case 4:
                    effect->startCondition.kind = 4;
                    effect->startCondition.value.handle = latestOperandHandle;
                    break;
                case 0: default:
                    effect->startCondition.kind = 5;
                    effect->startCondition.value.handle = groupTask->handle;
                    break;
                }
                effect->ownerId = actionOwner;
                btlStartTask(effect);
                task = sndCreateStationedSeTask(0x1000C);
                task->startCondition.kind = 5;
                task->startCondition.value.handle = effect->handle;
                task->ownerId = actionOwner;
                btlStartTask(task);
            }
            groupDelay += actionFrames;
        }
        if (mode != 1) {
            groupTask = sndCreateEffectWithTargets(unit->resourceNode, actors[0], (s32)actors[1], (s32)actors[2], effectTarget, 1);
            groupTask->startCondition.kind = 7;
            groupTask->startCondition.value.handle = masterOwner;
            groupTask->endCondition.kind = 7;
            groupTask->endCondition.value.handle = actionOwner;
            groupTask->ownerId = btlAdvanceRuntimeSequenceCounter();
            btlStartTask(groupTask);
            task = (BtlRuntimeTask *)sndCreateSkillSeTask((s32)unit->listNode, 1);
            task->startCondition.kind = 7;
            task->startCondition.value.handle = masterOwner;
            task->endCondition.kind = 7;
            task->endCondition.value.handle = actionOwner;
            task->ownerId = btlAdvanceRuntimeSequenceCounter();
            btlStartTask(task);
        }
        if (action->indexWork.unk5E != 0) {
            task = btlCreateRefreshEligibleActorsTask();
            task->startCondition.kind = 7;
            task->startCondition.value.handle = commandOwner;
            btlStartTask(task);
        }
    }
    task = (BtlRuntimeTask *)fldCreateSceneGroupAction((u8 *)action, action->indexWork.adjustedValue, (u8)action->indexWork.resultKind);
    switch (mode) {
    case 1:
        task->startCondition.kind = 7;
        task->startCondition.value.handle = masterOwner;
        break;
    case -1: case 2: case 3: case 4:
        task->startCondition.kind = 4;
        task->startCondition.value.handle = latestOperandHandle;
        break;
    case 0: default:
        task->startCondition.kind = 7;
        task->startCondition.value.handle = masterOwner;
        break;
    }
    task->ownerId = unit->identity;
    btlStartTask(task);
    if (resultTarget != NULL && action->indexWork.wait >= 0 && (action->flags & 0x20) == 0) {
        currentTarget = resultTarget;
        task = btlCreateEffObjC(currentTarget, action->indexWork.wait);
        task->startCondition.kind = 7;
        task->startCondition.value.handle = actionOwner;
        btlStartTask(task);
    }
    task = (BtlRuntimeTask *)btlCreateUpdateUnitEffectsTask();
    task->startCondition.kind = 7;
    task->startCondition.value.handle = commandOwner;
    task->startDelay = 30;
    btlStartTask(task);
    task = sndCreateReleaseTask(12);
    task->startCondition.kind = 7;
    task->startCondition.value.handle = actionOwner;
    btlStartTask(task);
    task = func_001F0920(12);
    task->startCondition.kind = 7;
    task->startCondition.value.handle = actionOwner;
    btlStartTask(task);
    task = btlCreateWaitUnitListIdleTask(12);
    task->startCondition.kind = 7;
    task->startCondition.value.handle = actionOwner;
    btlStartTask(task);
    task = btlCreateApplyToActiveActorsTask(12);
    task->startCondition.kind = 7;
    task->startCondition.value.handle = actionOwner;
    btlStartTask(task);
    task = (BtlRuntimeTask *)sndCreateClearStateTask();
    task->startCondition.kind = 7;
    task->startCondition.value.handle = actionOwner;
    btlStartTask(task);
    task = (BtlRuntimeTask *)btlCreateFadeStateResetTask();
    task->startCondition.kind = 7;
    task->startCondition.value.handle = actionOwner;
    btlStartTask(task);
    if ((action->unit->partyRecord.status & 0x480) != 0) {
        btlDispatchStateHandler(action, 0x18);
        return;
    }
    btlDispatchStateHandler(action, 0x1A);
}

void btlMarkLinkedActorStatusFlag(s32 arg0) {
    ((BtlTask *)arg0)->unit->status.flags = (u32)((BtlTask *)arg0)->unit->status.flags | 0x4000
    ;
}

/* State-handler table entry (0x359BEC); the table holds s32 handlers (btlCommandStateSelectB) and this one
 * returns without a value, as retail's missing sibling calls show (DDS1 twin of dds2 func_001DA210).
 * Gun-change command (slot 0x11): once no blocking tasks remain, swap the unit to its gun model, refresh linked
 * allies and reload their models, start the gun effects, and continue to state 0x18/0x1A; a unit already
 * holding the gun only restores its motion, swaps back and continues to 0x1B/0x1A. */
s32 btlCommandGunChangeStart(BtlTask *task) {
    BtlUnit *unit;
    BtlState *state;
    BtlUnit *other;
    SoundTask *sound;
    SoundTask *change;
    SoundTask *spawned;
    BtlRuntimeTask *load;
    u16 *status;
    s32 motion;

    if (btlCountTasksByKind(0x1A) != 0 || btlCountTasksByKind(0x18) != 0 || btlCountTasksByKind(0x23) != 0) {
        return;
    }
    state = (BtlState *)btlGetRuntime();
    unit = task->unit;
    status = (u16 *)btlGetSideIndexedActorStatusTable(unit->resourceKind, unit->species);
    if (!(unit->status.flags & 0x20)) {
        motion = func_001D6050(unit, 0x11) + 0x14;
    } else {
        motion = status[0x15];
    }
    if (!(unit->status.flags & 0x400020)) {
        unit->status.flags &= ~0x1000;
        unit->partyRecord.flags &= ~0x1000;
        sound = (SoundTask *)btlCreateMoveOtherUnitsTask((u8 *)unit, 0x11);
        btlStartTask(sound);
        for (other = state->units; other != NULL; other = other->next) {
            if (other != unit && (btlUnitStatusPair(other) & 0x202) == 0x202) {
                spawned = (SoundTask *)btlScheduleRefreshTask((u8 *)other);
                spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
                spawned->startDelay = 1;
                spawned->startCondition.value.handle = sound->handle;
                spawned->owner = unit->identity;
                btlStartTask(spawned);
            }
        }
        change = (SoundTask *)btlCreateModelChangeTask((u8 *)unit, unit->modelId, unit->modelVariant, motion, 0x18,
                                                       0);
        change->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
        change->startCondition.value.handle = sound->handle;
        btlStartTask(change);
        for (other = state->units; other != NULL; other = other->next) {
            if (other != unit && (btlUnitStatusPair(other) & 0x202) == 0x202) {
                load = btlCreateModelLoadPollTask(other, other->resourceKind, other->species, 0);
                load->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
                load->startCondition.value.handle = change->handle;
                load->ownerId = unit->identity;
                btlStartTask(load);
                spawned = (SoundTask *)btlCreateUnitFadeInTask((u8 *)other, 0, 0);
                spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
                spawned->startCondition.value.handle = load->handle;
                spawned->owner = unit->identity;
                btlStartTask(spawned);
            }
        }
        btlStartTask(btlCreateGunLoadPollTask((u8 *)unit));
        btlStartTask(sndCreateEffectSourceTask(state->resources[45], unit, change->handle));
        btlStartTask(sndCreateStationedSeTask(0x1000F));
        spawned = (SoundTask *)btlCreateEffObjA(unit, task->indexWork.phase);
        spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = btlCreateCommandSoundUpdateTask();
        spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = (SoundTask *)btlCreateSecondaryCommandSoundTask();
        spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = (SoundTask *)btlCreateCommandSoundTask((s32)task, 0xE);
        spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = (SoundTask *)btlAllocateIndexedUnitEffectTask((u8 *)unit, 0x11,
                                                                btlGetSlotRateKind((u8 *)unit, 0x11), 1.0f);
        spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = (SoundTask *)fldCreateSceneGroupAction((u8 *)task, 0x64, 1);
        spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
        spawned->startCondition.value.handle = sound->handle;
        spawned->owner = unit->identity;
        btlStartTask(spawned);
        if (task->unit->partyRecord.status & 0x480) {
            btlDispatchStateHandler(task, 0x18);
        } else {
            btlDispatchStateHandler(task, 0x1A);
        }
    } else {
        if ((unit->partyRecord.status & 0x7FFF) != 0x4000) {
            unit->status.flags &= ~0x20;
            if (unit->partyRecord.hp == 0) {
                unit->partyRecord.hp = 1;
                btlRefreshUnitMotionSelection(unit);
            }
        }
        if (task->indexWork.stage == 4) {
            if (!btlCheckSpecialAbility(&unit->partyRecord, 0x231)) {
                btlStartTask(btlCreateEffObjB(unit, task->indexWork.parameter));
                task->indexWork.stage = 0;
            } else {
                btlStartTask(btlCreateEffObjD(unit, 0x231));
            }
        }
        unit->status.flags &= ~0x1000;
        unit->partyRecord.flags &= ~0x1000;
        change = (SoundTask *)btlCreateModelChangeTask((u8 *)unit, unit->modelId, unit->modelVariant, motion, 0x12,
                                                       3);
        btlStartTask(change);
        btlStartTask(btlCreateGunLoadPollTask((u8 *)unit));
        btlStartTask(sndCreateEffectSourceTask(state->resources[47], unit, change->handle));
        btlStartTask(sndCreateStationedSeTask(0x1000F));
        unit->status.flags &= ~0x400000;
        if (task->flags & 0x10) {
            btlDispatchStateHandler(task, 0x1B);
        } else {
            btlDispatchStateHandler(task, 0x1A);
        }
    }
}

void func_001CE5D8(s32 arg0) {
    ((BtlTask *)arg0)->unit->status.flags = (u32)((BtlTask *)arg0)->unit->status.flags | 0x4000
    ;
}

/* Slot-0x10 model-change command: the same flow as btlCommandGunChangeStart, swapping to the unit's
 * unkDC/displaySpecies model and back; the return path only clears flag 0x20000000. */
void func_001CE5F0(BtlTask *task) {
    BtlUnit *unit;
    BtlState *state;
    BtlUnit *other;
    SoundTask *sound;
    SoundTask *change;
    SoundTask *spawned;
    BtlRuntimeTask *load;
    u16 *status;
    s32 motion;

    if (btlCountTasksByKind(0x1A) != 0 || btlCountTasksByKind(0x18) != 0 || btlCountTasksByKind(0x23) != 0) {
        return;
    }
    state = (BtlState *)btlGetRuntime();
    unit = task->unit;
    status = (u16 *)btlGetSideIndexedActorStatusTable(unit->resourceKind, unit->species);
    if (!(unit->status.flags & 0x20)) {
        motion = func_001D6050(unit, 0x10) + 0x14;
    } else {
        motion = status[0x15];
    }
    if (!(unit->status.flags & 0x20000020)) {
        unit->status.flags |= 0x1000;
        unit->partyRecord.flags |= 0x1000;
        sound = (SoundTask *)btlCreateMoveOtherUnitsTask((u8 *)unit, 0x10);
        btlStartTask(sound);
        for (other = state->units; other != NULL; other = other->next) {
            if (other != unit && (btlUnitStatusPair(other) & 0x202) == 0x202) {
                spawned = (SoundTask *)btlScheduleRefreshTask((u8 *)other);
                spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
                spawned->startDelay = 1;
                spawned->startCondition.value.handle = sound->handle;
                spawned->owner = unit->identity;
                btlStartTask(spawned);
            }
        }
        change = (SoundTask *)btlCreateModelChangeTask((u8 *)unit, unit->unkDC, unit->displaySpecies, motion, 0x18,
                                                       0);
        change->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
        change->startCondition.value.handle = sound->handle;
        btlStartTask(change);
        for (other = state->units; other != NULL; other = other->next) {
            if (other != unit && (btlUnitStatusPair(other) & 0x202) == 0x202) {
                load = btlCreateModelLoadPollTask(other, other->resourceKind, other->species, 0);
                load->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
                load->startCondition.value.handle = change->handle;
                load->ownerId = unit->identity;
                btlStartTask(load);
                spawned = (SoundTask *)btlCreateUnitFadeInTask((u8 *)other, 0, 0);
                spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
                spawned->startCondition.value.handle = load->handle;
                spawned->owner = unit->identity;
                btlStartTask(spawned);
            }
        }
        btlStartTask(btlCreateGunLoadPollTask((u8 *)unit));
        btlStartTask(sndCreateEffectSourceTask(state->resources[45], unit, change->handle));
        btlStartTask(sndCreateStationedSeTask(0x1000F));
        spawned = (SoundTask *)btlCreateEffObjA(unit, task->indexWork.phase);
        spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = btlCreateCommandSoundUpdateTask();
        spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = (SoundTask *)btlCreateSecondaryCommandSoundTask();
        spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = (SoundTask *)btlCreateCommandSoundTask((s32)task, 0xE);
        spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = (SoundTask *)btlAllocateIndexedUnitEffectTask((u8 *)unit, 0x10,
                                                                btlGetSlotRateKind((u8 *)unit, 0x10), 1.0f);
        spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = (SoundTask *)fldCreateSceneGroupAction((u8 *)task, 0x64, 1);
        spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
        spawned->startCondition.value.handle = sound->handle;
        spawned->owner = unit->identity;
        btlStartTask(spawned);
        if (task->unit->partyRecord.status & 0x480) {
            btlDispatchStateHandler(task, 0x18);
        } else {
            btlDispatchStateHandler(task, 0x1A);
        }
    } else {
        unit->status.flags |= 0x1000;
        unit->partyRecord.flags |= 0x1000;
        change = (SoundTask *)btlCreateModelChangeTask((u8 *)unit, unit->unkDC, unit->displaySpecies, motion, 0x12,
                                                       3);
        btlStartTask(change);
        btlStartTask(btlCreateGunLoadPollTask((u8 *)unit));
        btlStartTask(sndCreateEffectSourceTask(state->resources[47], unit, change->handle));
        btlStartTask(sndCreateStationedSeTask(0x1000F));
        unit->status.flags &= ~0x20000000;
        if (task->flags & 0x10) {
            btlDispatchStateHandler(task, 0x1B);
        } else {
            btlDispatchStateHandler(task, 0x1A);
        }
    }
}

void func_001CEA58(s32 arg0) {
    ((BtlTask *)arg0)->unit->status.flags = (u32)((BtlTask *)arg0)->unit->status.flags | 0x4000
    ;
}

extern u8 *btlCreateModelChangeTask(u8 *, s32, s32, s32, s32, u8);

void func_001CEA70(u8 *task) {
    BtlUnit *actor;
    u8 *effectTask;
    u8 *modelTask;
    BtlActorStatusRecord *statusTable;
    s32 model;

    if (btlCountTasksByKind(0x1A) != 0 ||
        btlCountTasksByKind(0x18) != 0 ||
        btlCountTasksByKind(0x23) != 0) {
        return;
    }

    btlGetRuntime();
    actor = *(BtlUnit **)(task + 0x18);
    statusTable = (BtlActorStatusRecord *)btlGetSideIndexedActorStatusTable(
        *(s32 *)((u8 *)actor + 0xC4), *(s32 *)((u8 *)actor + 0xC8));
    model = statusTable->model;
    effectTask = (u8 *)btlCreateEffObjB(actor, 0x7E);
    btlStartTask(effectTask);
    modelTask = btlCreateModelChangeTask(actor, 0, 0x1F, model, 0x12, 1);
    btlStartTask(modelTask);

    actor->status.flags = ((u32)actor->status.flags | 0x1000) & 0xEFFFFFFF;
    *(u16 *)((u8 *)actor + 0x120) |= 0x1000;
    if (*(u32 *)(task + 8) & 0x10) {
        btlDispatchStateHandler(task, 0x1B);
    } else {
        btlDispatchStateHandler(task, 0x1A);
    }
}

void func_001CEB78(void) {
}

void btlCommandTaskStartEffects(u8 *task) {
    s32 countdown;
    s32 effectId;

    if (((u32)((BtlTask *)task)->unit->status.flags & 0x200) != 0 &&
        (*(u32 *)(task + 8) & 0x20) != 0) {
        btlStartTask(btlCreateCommandSoundUpdateTask());
        btlStartTask(btlCreateSecondaryCommandSoundTask());
        btlStartTask(btlCreateCommandSoundTask((s32)task, 9));
    }
    switch (*(s32 *)(task + 0x20)) {
    case 14:
        ((BtlTask *)task)->unit->status.flags = (u32)((BtlTask *)task)->unit->status.flags | (0x2000000);
        effectId = 0xF;
        countdown = 0x64;
        break;
    case 10:
        if (((u32)((BtlTask *)task)->unit->status.flags & 0x400) != 0) {
            btlStartTask(btlCreateCommandSoundUpdateTask());
            btlStartTask(btlCreateSecondaryCommandSoundTask());
            btlStartTask(btlCreateCommandSoundTask((s32)task, 3));
        }
        effectId = ((u32)((BtlTask *)task)->unit->status.flags & 0x200) ? 0xF : 0x1E;
        countdown = 0x32;
        break;
    case 13:
        effectId = 0xF;
        countdown = 0x64;
        break;
    default:
        effectId = 0;
        countdown = 0;
        break;
    }

    btlStartTask(btlCreateEffObjA(0, *(s32 *)(task + 0x20)));
    {
        u8 *object = fldCreateSceneGroupAction(task, countdown, 1);
        *(s32 *)(object + 0x28) = effectId;
        *(s64 *)(object + 0x40) = *(s64 *)(*(u8 **)(task + 0x18) + 0x108);
        btlStartTask(object);
    }
    if ((*(u16 *)(*(u8 **)(task + 0x18) + 0x12E) & 0x480) != 0) {
        btlDispatchStateHandler(task, 0x18);
    } else {
        btlDispatchStateHandler(task, 0x1A);
    }
}

u64 btlCommandTaskReturnStart(u8 *task) {
    BtlUnit *linked = *(BtlUnit **)(task + 0x34);
    BtlUnit *actor;
    u8 *object;

    if (linked != 0) {
        actor = linked;
    } else {
        actor = *(BtlUnit **)(task + 0x18);
    }
    if ((u32)actor->status.flags & 0x200) {
        btlBossDebugPrintf("return:player=%X[%X]\n", ((u8 *)actor)[0x2C4], *(u16 *)((u8 *)actor + 0x124));
    } else {
        btlBossDebugPrintf("return:enemy=%X\n", *(u16 *)((u8 *)actor + 0x124));
    }
    if ((u32)actor->status.flags & 0x400) {
        btlStartTask(btlCreateCommandSoundUpdateTask());
        btlStartTask(btlCreateSecondaryCommandSoundTask());
    }
    if ((u32)actor->status.flags & 0x200) {
        btlStartTask(btlCreateCommandSoundTask((s32)task, 9));
        btlStartTask(btlCreateEffObjA(actor, *(u32 *)(task + 0x20)));
    } else if ((*(u32 *)(task + 8) & 0x200) == 0) {
        btlStartTask(btlCreateCommandSoundTask((s32)task, 0x10));
        btlStartTask(btlCreateEffObjB(actor, 0xF));
    }
    if ((u32)actor->status.flags & 0x200) {
        btlSyncPlayerWork(actor);
    }
    object = fldCreateSceneGroupAction(task, 0x64, 1);
    *(s32 *)(object + 0x28) = 0xF;
    *(u64 *)(object + 0x40) = *(u64 *)(*(u8 **)(task + 0x18) + 0x108);
    btlStartTask(object);
    object = func_001D9468(actor, 1);
    *(s32 *)(object + 0x28) = 0xF;
    return btlStartTask(object);
}

extern s32 btlCountTasksByKind(u16 kind);

extern s32 btlRepositionPartyAroundBattleCenter(void);

extern s32 btlMarkInactiveActorCandidates(void);

extern void fldUpdateSceneGroupTask(s32 task);

extern void btlRemoveTaskFromSceneGroup(BtlTask *task);

void btlCommandTaskReturnUpdate(s32 task) {
    BtlUnit *unit = *(BtlUnit **)(task + 0x18);
    u32 flags = (u32)unit->status.flags;

    unit->status.flags = flags & ~1;
    if (flags & 0x200) {
        btlRepositionPartyAroundBattleCenter();
        btlMarkInactiveActorCandidates();
    }
    if (btlCountTasksByKind(0x3C) != 0) {
        return;
    }
    if (btlCountTasksByKind(0x42) != 0) {
        return;
    }
    if (((u32)unit->status.flags & 0x40) == 0) {
        return;
    }
    if (fldReleaseIdleSceneActorResources((BtlUnit *)*(s32 *)(task + 0x18)) != 0) {
        if ((u32)unit->status.flags & 0x200) {
            func_001A1960(&unit->partyRecord, 8);
            btlRemovePartyActorAndShiftEntries(unit);
        }
        fldUpdateSceneGroupTask(task);
        btlRemoveTaskFromSceneGroup((BtlTask *)task);
        btlDispatchStateHandler(task, 0x1E);
    }
}

void btlStartLinkedActorEffectTask(s32 task) {
    BtlUnit *unit = *(BtlUnit **)(task + 0x18);
    BtlRuntimeTask *entry;
    if (((u32)unit->status.flags & 0x200) == 0 && func_001A8CE0(unit) == 0) {
        entry = btlCreateEffObjB(*(BtlUnit **)(task + 0x18), 0x67);
        entry->startCondition.kind = BTL_TASK_CONDITION_KIND_ABSENT;
        entry->startCondition.value.taskKind = 0x40;
        btlStartTask(entry);
        if ((*(u16 *)(*(s32 *)(task + 0x18) + 0x12E) & 0x480) != 0) {
            btlDispatchStateHandler(task, 0x18);
        } else {
            btlDispatchStateHandler(task, 0x1A);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001CF050);

void btlStartOwnerEffectTasks(s32 *arguments) {
    BtlUnit *owner = (BtlUnit *)arguments[0x34 / 4];
    BtlRuntimeTask *value = btlCreateEffObjA(owner, arguments[0x20 / 4]);
    btlStartTask(value);
    value = (BtlRuntimeTask *)func_001D9468((u8 *)owner, 1);
    value->startDelay = 7;
    btlStartTask(value);
}

extern void func_001A2608(BtlUnit *, u8);

extern BtlRuntimeTask *btlCreateModelLoadPollTask(BtlUnit *, u32, u32, s8);

extern u8 *btlCreateGunLoadPollTask(u8 *);

extern BtlRuntimeTask *btlCreateUnitPositionLerpTowardTargetTask(BtlUnit *, f32 *, f32);

extern BtlRuntimeTask *btlCreateUnitRotationInterpolationTask(BtlUnit *, f32 *, f32);

extern u8 *func_001D9038(u8 *, u32);

extern BtlRuntimeTask *btlCreateUnitBaseLightTask(BtlUnit *);

extern void btlResetUnitLinks(BtlUnit *);

extern void btlReleaseUnitResources(BtlUnit *);

void btlReloadLinkedUnitAndQueueEffects(BtlTask *task) {
    BtlUnit *unit = task->indexWork.linkedUnit;
    DatPartyRecord *record;
    BtlRuntimeTask *load;
    BtlRuntimeTask *spawned;
    BtlRuntimeTask *delayed;
    u32 species;
    u32 flags;
    u32 unitId;
    f32 rate;

    if (!(unit->status.flags & 0x40)) {
        return;
    }
    if (fldReleaseIdleSceneActorResources(unit) == 0) {
        return;
    }
    unit->status.flags &= ~0xC0;
    record = &unit->partyRecord;
    func_001A1960(record, 8);
    if (unit->status.flags & 0x200) {
        btlSyncPlayerWork(unit);
    }
    btlReleaseUnitResources(unit);
    btlResetUnitLinks(unit);
    func_001A2608(unit, (u8)task->indexWork.unk18);
    flags = unit->status.flags;
    unit->status.flags = flags | 0x301;
    if (record->flags & 0x1000) {
        unit->status.flags = flags | 0x1301;
    }
    if (record->flags & 0x4000) {
        unit->status.stateFlags |= 0x2000;
    }
    if (unit->partyRecord.status & 0x4000) {
        unit->status.flags |= 0x20;
    }
    unitId = unit->partyRecord.unitId;
    unit->modelVariant = unitId;
    unit->displaySpecies = unitId + 0x10;
    unit->modelId = 0;
    unit->unkDC = 0;
    species = (unit->status.flags & 0x1000) ? unit->displaySpecies : unitId;
    ((BtlTask *)btlFindUnitByActor((s32)unit))->flags |= 0x100;
    unit->baseColor = 0x80808080;
    unit->overlayColor = 0x80808080;
    if (!(unit->partyRecord.status & 0x1000)) {
        load = btlCreateModelLoadPollTask(unit, 0, species, 0);
    } else {
        load = btlCreateModelLoadPollTask(unit, 0, 0x1F, 0);
    }
    rate = 1.0f;
    load->ownerId = task->unit->identity;
    btlStartTask(load);
    spawned = (BtlRuntimeTask *)btlCreateGunLoadPollTask((u8 *)unit);
    spawned->ownerId = task->unit->identity;
    btlStartTask(spawned);
    spawned = btlCreateUnitPositionLerpTowardTargetTask(unit, unit->currentPosition, rate);
    spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
    spawned->startCondition.value.handle = load->handle;
    btlStartTask(spawned);
    if (!(unit->status.flags & 0xE0)) {
        spawned = btlCreateUnitRotationInterpolationTask(unit, unit->orientation, rate);
    } else {
        spawned = btlCreateUnitRotationInterpolationTask(unit, unit->rotation, rate);
    }
    spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
    spawned->startCondition.value.handle = load->handle;
    btlStartTask(spawned);
    delayed = (BtlRuntimeTask *)func_001D9038((u8 *)unit, 0x12);
    delayed->ownerId = task->unit->identity;
    delayed->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
    delayed->startDelay = 2;
    delayed->startCondition.value.handle = load->handle;
    btlStartTask(delayed);
    spawned = btlCreateUnitBaseLightTask(unit);
    spawned->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
    spawned->startCondition.value.handle = delayed->handle;
    btlStartTask(spawned);
    spawned = (BtlRuntimeTask *)fldCreateSceneGroupAction((u8 *)task, 0x64, 1);
    spawned->ownerId = task->unit->identity;
    btlStartTask(spawned);
    if ((task->unit->partyRecord.status & 0x480) && task->indexWork.linkedUnit != task->unit) {
        btlDispatchStateHandler(task, 0x18);
    } else {
        btlDispatchStateHandler(task, 0x1A);
    }
}

extern s32 btlRollEscapeChance(BtlUnit *);

void btlRecordLinkedActorOutcome(BtlTask *object) {
    s32 context = btlGetRuntime();
    BtlUnit *target = object->unit;
    *(s32 *)(context + 0x254) += 1;
    if (btlRollEscapeChance(target)) {
        *(u32 *)(context + 0x1F4) |= 0x2000;
    } else {
        *(u32 *)(context + 0x1F4) |= 0x1000;
    }
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001CFB10);

void func_001CFD70(void) {
}

extern s32 btlCountTasksForOwner(s64);

extern s32 btlSumOtherTargetHitAmounts(u8 *);

extern s32 btlComputeStatusPenaltyFifth(BtlUnit *);

extern BtlRuntimeTask *btlCreateLinkedEffectTask(BtlUnit *, s32, u8);



void func_001CFD78(BtlTask *action) {
    BtlOperandEntry spec;
    s32 amount;
    BtlUnit *unit;
    BtlRuntimeTask *deltaTask;
    BtlRuntimeTask *effectTask;

    if (btlCountTasksByKind(0x45) != 0) return;
    if (btlCountTasksByKind(0x46) != 0) return;
    if (btlCountTasksByKind(0x47) != 0) return;
    unit = action->unit;
    if (btlCountTasksForOwner(unit->identity) != 0) return;
    if (action->indexWork.phase == 15) {
        effectTask = (BtlRuntimeTask *)fldCreateSceneGroupAction((u8 *)action, 100, 1);
        effectTask->ownerId = unit->identity;
        btlStartTask(effectTask);
    }
    switch (unit->partyRecord.status & 0x7FFF) {
    case 0x400:
        amount = btlSumOtherTargetHitAmounts((u8 *)action);
        if (amount < 0) {
            btlStartTask(sndCreateStationedSeTask(0x1000A));
            memset(&spec, 0, sizeof(spec));
            spec.hpDelta = amount;
            deltaTask = btlCreateActorParameterDeltaTask(unit, &spec);
            btlStartTask(deltaTask);
            if (spec.hpDelta != 0) {
                effectTask = btlCreateLinkedEffectTask(unit, spec.hpDelta, 0);
                effectTask->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
                effectTask->startCondition.value.handle = deltaTask->handle;
                effectTask->ownerId = btlAdvanceRuntimeSequenceCounter();
                btlStartTask(effectTask);
            }
            if ((unit->status.flags & 0x200) != 0 && btlIsUnitDefeatTriggeredByValueDelta(unit, spec.hpDelta) != 0) {
                btlStartTask(btlCreateActorModelBlendTask(unit, 0, 11, 2, 1.0f));
                btlStartTask(btlCreateMoveOtherUnitsTask((u8 *)unit, 11));
            } else {
                btlStartTask(btlCreateStiffenDamageShakeTask((u8 *)unit, 8.0f));
            }
        }
        btlDispatchStateHandler(action, 0x1A);
        return;
    case 0x80:
    case 0x2000:
        btlStartTask(sndCreateStationedSeTask(0x1000A));
        memset(&spec, 0, sizeof(spec));
        spec.hpDelta = btlComputeStatusPenaltyFifth(unit);
        deltaTask = btlCreateActorParameterDeltaTask(unit, &spec);
        btlStartTask(deltaTask);
        if (spec.hpDelta != 0) {
            effectTask = btlCreateLinkedEffectTask(unit, spec.hpDelta, 0);
            effectTask->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
            effectTask->startCondition.value.handle = deltaTask->handle;
            effectTask->ownerId = btlAdvanceRuntimeSequenceCounter();
            btlStartTask(effectTask);
        }
        if ((unit->status.flags & 0x200) != 0 && btlIsUnitDefeatTriggeredByValueDelta(unit, spec.hpDelta) != 0) {
            btlStartTask(btlCreateActorModelBlendTask(unit, 0, 11, 2, 1.0f));
            btlStartTask(btlCreateMoveOtherUnitsTask((u8 *)unit, 11));
        } else {
            btlStartTask(btlCreateStiffenDamageShakeTask((u8 *)unit, 8.0f));
        }
        btlDispatchStateHandler(action, 0x1A);
        return;
    }
    btlDispatchStateHandler(action, 0x1A);
}

void btlSpawnSceneActionAndSwitchState(void) {
}

void func_001D0048(u8 *arg0) {
    u8 *task = fldCreateSceneGroupAction(arg0, 0x1194, 1);

    btlStartTask(task);
    btlDispatchStateHandler(arg0, 0x1a);
}

void func_001D0088(void) {
}

extern s32 btlCountTasksForOwner(s64);

void btlAdvanceStateWhenLinkedTasksFinish(s32 object) {
    s32 owner = *(s32 *)(object + 0x18);
    if (btlCountTasksForOwner(*(s64 *)(owner + 0x108)) == 0) {
        btlDispatchStateHandler(object, 0x1C);
    }
}

void btlAdvanceLinkedUnitWhenOwnerIdle(void) {
}

void func_001D00E0(s32 object) {
    BtlUnit *owner = *(BtlUnit **)(object + 0x18);
    if (btlCountTasksForOwner(*(s64 *)((u8 *)owner + 0x108)) == 0) {
        owner->status.flags = (u32)owner->status.flags & (~0x4000);
        btlDispatchStateHandler(object, 2);
    }
}

void btlFinalizeLinkedActionAndAdvanceHistory(void) {
}

extern void btlAdvanceHistoryCounter(s32 task);

void btlUnitTurnEndCommit(s32 task) {
    void (*hook)(s32) = *(void (**)(s32))(btlGetRuntime() + 0x600);
    BtlTask *command = (BtlTask *)task;
    BtlUnit *owner = command->unit;

    if (hook != 0) {
        hook(task);
    }
    btlAdvanceHistoryCounter(task);
    btlResetIndexWork(&command->indexWork);
    owner->unk2F4 = -1;
    command->options &= ~1;
    command->unk14 += 1;
    owner->status.flags &= ~0x4000;
    fldUpdateSceneGroupTask(task);
    if (command->unit->status.flags & 0x20) {
        btlUnitTurnEndStateSelect((u8 *)task);
    } else {
        btlDispatchStateHandler(task, 2);
    }
}

extern void btlAccumulateEnemyDefeatRewards(s32);


extern u8 *btlCreateSelectedEffectUpdateTask(u8 *);

extern u8 *func_001D9468(u8 *, u32);


void btlStartActorDefeatTransition(s32 command) {
    BtlActorWork *work = (BtlActorWork *)btlGetRuntime();
    BtlTask *commandTask = (BtlTask *)command;
    BtlUnit *actor = commandTask->unit;
    DatPartyRecord *profile = &actor->partyRecord;
    SoundTask *soundTask;
    BtlRuntimeTask *object;
    s64 sequence;
    s32 entryFlags;
    s32 result;

    actor->selectedEntryIndex = -1;
    func_001A1948(&actor->partyRecord, 0x4000);
    btlGetSideIndexedActorStatusTable(actor->resourceKind, actor->species);
    if (!(commandTask->flags & 0x100)) {
        soundTask = (SoundTask *)btlCreateMoveOtherUnitsTask((u8 *)actor, 11);
        btlStartTask(soundTask);
        sequence = soundTask->handle;
    } else {
        sequence = btlAdvanceRuntimeSequenceCounter();
    }
    if (actor->status.flags & 0x200) {
        if (!(commandTask->flags & 0x100)) {
            if (!(actor->status.flags & 0x8000000) && actor->unkEC != 11) {
                object = btlCreateActorModelBlendTask(actor, 0, 11, 2, 1.0f);
                object->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
                object->startCondition.value.handle = sequence;
                btlStartTask(object);
            }
            btlRefreshUnitMotionSelection(actor);
        }
    } else if (actor->status.flags & 0x400) {
        btlAccumulateEnemyDefeatRewards((s32)actor);
        if (actor->status.flags & 0x8000000) {
            object = (BtlRuntimeTask *)btlCreateSelectedEffectUpdateTask((u8 *)actor);
            object->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
            object->startCondition.value.handle = sequence;
            btlStartTask(object);
            object = sndCreateStationedSeTask(0x1000E);
            object->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
            object->startCondition.value.handle = sequence;
            btlStartTask(object);
            actor->status.flags &= ~1;
        } else {
            entryFlags = btlGetEntryFlagsUnlessDisabled(profile);
            result = 0;
            if (work->hook5E4 != 0) {
                result = work->hook5E4(actor);
            }
            if ((entryFlags & 0x200) && result == 0) {
                result = 1;
                if (work->hook5E8 != 0) {
                    result = work->hook5E8(actor);
                }
                if (result != 0) {
                    if (actor->unkEC != 11) {
                        object = btlCreateActorModelBlendTask(actor, 0, 11, 2, 1.0f);
                        object->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
                        object->startCondition.value.handle = sequence;
                        btlStartTask(object);
                    }
                    btlRefreshUnitMotionSelection(actor);
                }
            } else {
                object = (BtlRuntimeTask *)func_001D9468((u8 *)actor, 0);
                object->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
                object->startCondition.value.handle = sequence;
                btlStartTask(object);
                actor->status.flags &= ~1;
            }
        }
        actor->partyRecord.flags &= ~2;
    }
}

void btlRemoveEligibleActorSceneTask(BtlTask *task) {
    BtlUnit *actor = task->unit;
    s32 entryFlags;

    if (actor->status.flags & 0x200) {
        if (fldReleaseIdleSceneActorResources(actor) == 0) {
            return;
        }
        btlResetIndexWork(&task->indexWork);
        actor->unk2F4 = -1;
        btlClearAllActorEntrySlots(actor);
        fldUpdateSceneGroupTask(task);
        btlRemoveTaskFromSceneGroup(task);
        btlDispatchStateHandler(task, 1);
    } else {
        if ((actor->status.flags & 0x400) == 0) {
            return;
        }
        entryFlags = btlGetEntryFlagsUnlessDisabled(&actor->partyRecord);
        if ((actor->status.flags & 0x40) == 0 && !(entryFlags & 0x200)) {
            return;
        }
        fldUpdateSceneGroupTask(task);
        btlRemoveTaskFromSceneGroup(task);
        if ((entryFlags & 0x200) && (actor->status.flags & 0x40) == 0) {
            btlDispatchStateHandler(task, 1);
        } else {
            btlDispatchStateHandler(task, 0x1E);
        }
    }
}

void func_001D0590(void) {
}

void btlCommandTaskReleaseActor(s32 arg0) {
    if ((*(u32 *)(arg0 + 8) & 8) != 0) {
        if (fldReleaseIdleSceneActorResources((BtlUnit *)*(s32 *)(arg0 + 0x18)) == 0) {
            return;
        }
        if (*(s32 *)(arg0 + 0x18) != 0) {
            btlReleaseUnitResources((BtlUnit *)*(s32 *)(arg0 + 0x18));
            {
                BtlUnit *fx = (BtlUnit *)*(s32 *)(arg0 + 0x18);
                *(u32 *)((u8 *)fx + 0x54) = 0x80808080;
                *(u32 *)((u8 *)fx + 0x84) = 0x80808080;
                fx->status.flags = (u32)fx->status.flags & 0x700;
                btlInitUnitFxDefaults(fx);
            }
            {
                u8 *model = (u8 *)*(s32 *)(arg0 + 0x18);
                PCP_COPY_VECTOR(model + 0x60, model + 0x30);
                PCP_COPY_VECTOR(model + 0x70, model + 0x40);
                *(s32 *)(model + 0x2F0) = -1;
            }
            *(u32 *)(arg0 + 0x18) = 0;
            *(u32 *)(arg0 + 8) &= ~8;
        }
    }
    btlClearSceneTaskActiveFlag(arg0);
    *(u32 *)(arg0 + 8) |= 2;
}

void btlFlagLinkedActorActionInProgress(s32 arg0) {
    ((BtlTask *)arg0)->unit->status.flags = (u32)((BtlTask *)arg0)->unit->status.flags | 0x4000
    ;
}

extern s32 btlGetRuntime(void);

void btlRunHookAndAdvanceUnitState(s32 object) {
    void (*callback)(s32) = *(void (**)(s32))(btlGetRuntime() + 0x660);
    if (callback != 0) {
        callback(object);
    }
    btlDispatchStateHandler(object, 0x1A);
}

void func_001D06C0(void) {
}

void func_001D06C8(void) {
}

void func_001D06D0(void) {
    btlInitDrawTables();
}

void btlAdvanceUnitWhenActionGateClears(u32 arg0) {
    s64 temp_v0;

    temp_v0 = func_00214868();
    if (temp_v0 == 0) {
        btlDispatchStateHandler(arg0, 6);
        return;
    }
}

typedef struct BattleActionState {
    void (*start)(void *);
    void (*update)(void *);
    const char *name; /* +8: debug label, not a finish callback. */
} BattleActionState;

extern BattleActionState D_00359B28[];

void btlDispatchStateHandler(void *object, s32 kind) {
    ((s32 *)object)[0] = kind;
    ((s32 *)object)[4] = 0;
    D_00359B28[kind].start(object);
}

extern char D_003A3788[]; /* "btl:action seq create[%p]\n" */

BtlTask *btlCreateActionSeq(void) {
    BtlTask *sequence = sdfAllocAndClearQuadwords(sizeof(*sequence));
    BtlState *context;
    BtlTask *head;

    sequence->actionNumber = 1;
    btlInitBattleIndexWork(&sequence->indexWork);
    context = (BtlState *)btlGetRuntime();
    sequence->prev = 0;
    head = context->tasks;
    if (head != 0) {
        head->prev = sequence;
        sequence->next = context->tasks;
    } else {
        sequence->next = 0;
    }
    context->tasks = sequence;
    btlDispatchStateHandler(sequence, 0);
    btlBossDebugPrintf(D_003A3788, sequence);
    return sequence;
}

extern char D_003A37A8[]; /* "btl:action seq delete[%p]\n" */

void btlDestroyActionSeq(BtlTask *object) {
    btlBossDebugPrintf(D_003A37A8, object);
    btlReleaseObjectBuffers(&object->indexWork);
    if (object->next != 0) {
        object->next->prev = object->prev;
    }
    if (object->prev != 0) {
        object->prev->next = object->next;
    } else {
        BtlState *context = (BtlState *)btlGetRuntime();
        context->tasks = object->next;
    }
    sdfReleaseChipBlock(object);
}

void btlUpdateActionSeqs(void) {
    BtlTask *action = ((BtlState *)btlGetRuntime())->tasks;
    while (action != 0) {
        s32 flags = action->flags;
        BtlTask *next = action->next;
        if (flags & 1) {
            D_00359B28[action->state].update(action);
            action->stateTime += 1;
        } else if (flags & 2) {
            btlDestroyActionSeq(action);
        }
        action = next;
    }
}

void btlDestroyAllActionSeqs(void) {
    BtlTask *node = ((BtlState *)btlGetRuntime())->tasks;
    while (node != 0) {
        BtlTask *next = node->next;
        btlDestroyActionSeq(node);
        node = next;
    }
}

BtlTask *btlFindUnitByActor(BtlUnit *target) {
    BtlTask *node;

    node = ((BtlState *)btlGetRuntime())->tasks;
    while (node != 0) {
        if (node->unit == target) {
            return node;
        }
        node = node->next;
    }
    return 0;
}

extern void btlBossDebugPrintfN(s32, s32, s32, s32, ...);

extern char D_003BB5E0[];

extern char D_003BB5E8[];

extern char D_003A37C8[];

/* Display the eight slot entries, retaining each group's last valid task. */
void btlDebugPrintActionOrder(s32 x, s32 y) {
    BtlState *controller = (BtlState *)btlGetRuntime();
    BtlTask **primary;
    BtlTask **secondary;
    BtlTask **tertiary;
    BtlTask *task;
    s32 color;
    u32 i;

    if ((controller->battleFlags & 4) == 0) {
        return;
    }
    btlBossDebugPrintfN(x, y, 0, (s32)D_003A37C8);
    primary = controller->groupPrimary;
    secondary = controller->groupSecondary;
    tertiary = controller->groupTertiary;
    for (i = 0; i < 8; i++) {
        switch (controller->slots[i].group) {
        case 1:
            color = 0;
            task = *primary;
            if (primary[1] != NULL) {
                primary++;
            }
            break;
        case 2:
            task = *secondary;
            color = 5;
            if (secondary[1] != NULL) {
                secondary++;
            }
            break;
        case 3:
            task = *tertiary;
            color = 6;
            if (tertiary[1] != NULL) {
                tertiary++;
            }
            break;
        default:
            task = NULL;
            color = 0;
            break;
        }
        if (task == NULL || task->unit == NULL) {
            continue;
        }
        if (controller->slots[i].remaining == 100) {
            btlBossDebugPrintfN(x, y + (i + 1) * 12, color, (s32)D_003BB5E0,
                               D_00359B28[task->state].name);
        } else {
            btlBossDebugPrintfN(x, y + (i + 1) * 12, color, (s32)D_003BB5E8,
                               D_00359B28[task->state].name);
        }
    }
}

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3738);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3748);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3758);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3768);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3778);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3788);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A37A8);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A37C8);

INCLUDE_SDATA(const s32, "game/code_001C8890", D_003BB5E0);

INCLUDE_SDATA(const s32, "game/code_001C8890", D_003BB5E8);

INCLUDE_SDATA(const s32, "game/code_001C8890", btlDeferredTaskTail);

